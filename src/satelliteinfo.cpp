#include <QDateTime>
#include <QDebug>
#include <QStringList>

#include "logfile.h"
#include "wakelock.h"

#include "satelliteinfo.h"

SatelliteInfo::SatelliteID SatelliteInfo::makeId(const QGeoSatelliteInfo &sat)
{
    return (SatelliteID(sat.satelliteSystem()) << 32) | quint32(sat.satelliteIdentifier());
}

SatelliteInfo::SatelliteData SatelliteInfo::dataFrom(const QGeoSatelliteInfo &sat)
{
    SatelliteData sd;

    sd.id = sat.satelliteIdentifier();
    sd.azimuth = sat.hasAttribute(QGeoSatelliteInfo::Azimuth) ? sat.attribute(QGeoSatelliteInfo::Azimuth) : 0;
    sd.elevation = sat.hasAttribute(QGeoSatelliteInfo::Elevation) ? sat.attribute(QGeoSatelliteInfo::Elevation) : 0;
    sd.signalStrength = sat.signalStrength();
    sd.satelliteSystem = sat.satelliteSystem();
    sd.lastUpdated = QDateTime::currentMSecsSinceEpoch();

    return sd;
}

void SatelliteInfo::mergeSatellite(const QGeoSatelliteInfo &sat)
{
    SatelliteData sd = dataFrom(sat);
    SatelliteMap::iterator iter = m_satellites.find(makeId(sat));

    if(iter != m_satellites.end()) {
        iter->updateFrom(sd);
    } else {
        m_satellites.insert(makeId(sat), sd);
    }
}

void SatelliteInfo::updateUsedFlags()
{
    for(SatelliteMap::iterator iter = m_satellites.begin(); iter != m_satellites.end(); ++iter) {
        iter->inUse = m_used.contains(iter.key());
    }
}

void SatelliteInfo::updateMinMax()
{
    if(m_satellites.isEmpty()) {
        m_minSignal = 0;
        m_maxSignal = 1;
        return;
    }

    m_minSignal =  1e20;
    m_maxSignal = -1e20;

    for(const SatelliteData &sd: std::as_const(m_satellites)) {
        if(sd.signalStrength < m_minSignal) {
            m_minSignal = sd.signalStrength;
        }

        if(sd.signalStrength > m_maxSignal) {
            m_maxSignal = sd.signalStrength;
        }
    }

    // prevent zero range
    if(m_maxSignal == m_minSignal) {
        m_minSignal -= 1;
    }
}

void SatelliteInfo::logData()
{
    if(isLogging()) {
        /* Log file line format:
         * timestamp<tab>satid:signal:inuse,satid:signal:inuse,[...]\n
         */
        QStringList satInfo;

        for(const SatelliteData &sd: std::as_const(m_satellites)) {
            satInfo.append(QStringLiteral("%1:%2:%3")
                                   .arg(sd.label())
                                   .arg(sd.signalStrength)
                                   .arg(int(sd.inUse)));
        }

        QString line = QStringLiteral("%1\t%2\n")
                               .arg(QDateTime::currentMSecsSinceEpoch()/1000.0, 0, 'f', 3)
                               .arg(satInfo.join(QLatin1Char(',')));

        m_logFile.write(line.toUtf8());
    }
}

SatelliteInfo::SatelliteInfo(QObject *parent) :
    QObject(parent)
{
    m_source = QGeoSatelliteInfoSource::createDefaultSource(this);

    if(!m_source) {
        qWarning() << "No satellite info source available";
        return;
    }

    connect(m_source, &QGeoSatelliteInfoSource::satellitesInViewUpdated, this, &SatelliteInfo::setSatellitesInView);
    connect(m_source, &QGeoSatelliteInfoSource::satellitesInUseUpdated, this, &SatelliteInfo::setSatellitesInUse);
}

int SatelliteInfo::getNumberOfUsedSatellites() const
{
    int count = 0;

    for(const SatelliteData &sd: m_satellites) {
        if(sd.inUse) {
            count++;
        }
    }

    return count;
}

// add missing satellites and update the used flags
void SatelliteInfo::setSatellitesInUse(const QList<QGeoSatelliteInfo> &satellites)
{
    m_used.clear();

    for(const QGeoSatelliteInfo &sat: satellites) {
        m_used.insert(makeId(sat));
        mergeSatellite(sat);
    }

    updateUsedFlags();
    updateMinMax();

    emit newDataAvailable();
}

// add missing satellites and remove the ones timed out
void SatelliteInfo::setSatellitesInView(const QList<QGeoSatelliteInfo> &satellites)
{
    for(const QGeoSatelliteInfo &sat: satellites) {
        mergeSatellite(sat);
    }

    // remove old satellites, and forget that they were used
    qint64 threshold = QDateTime::currentMSecsSinceEpoch() - TIMEOUT;

    for(SatelliteMap::iterator iter = m_satellites.begin(); iter != m_satellites.end(); ) {
        if(iter->lastUpdated < threshold) {
            m_used.remove(iter.key());
            iter = m_satellites.erase(iter);
        } else {
            ++iter;
        }
    }

    updateUsedFlags();
    updateMinMax();

    logData();

    emit newDataAvailable();
}


void SatelliteInfo::SatelliteData::updateFrom(const SatelliteInfo::SatelliteData &other)
{
    // copy all attributes except inUse
    id = other.id;
    azimuth = other.azimuth;
    elevation = other.elevation;
    signalStrength = other.signalStrength;
    satelliteSystem = other.satelliteSystem;
    lastUpdated = other.lastUpdated;
}

QString SatelliteInfo::SatelliteData::label() const
{
    QChar system;

    switch(satelliteSystem) {
        case QGeoSatelliteInfo::GPS:     system = QLatin1Char('G'); break;
        case QGeoSatelliteInfo::GLONASS: system = QLatin1Char('R'); break;
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
        case QGeoSatelliteInfo::GALILEO: system = QLatin1Char('E'); break;
        case QGeoSatelliteInfo::BEIDOU:  system = QLatin1Char('C'); break;
        case QGeoSatelliteInfo::QZSS:    system = QLatin1Char('J'); break;
#endif
        default:                         return QString::number(id);
    }

    return system + QStringLiteral("%1").arg(id, 2, 10, QLatin1Char('0'));
}

void SatelliteInfo::activate(unsigned requestingPart)
{
    Activateable::activate(requestingPart);

    if(m_source && !m_sourceActive) {
        qDebug() << "Satellite info source started";
        m_source->startUpdates();
        m_sourceActive = true;
    }

    if(requestingPart == PART_PAGE) {
        WakeLock::instance().activateScreenLock(WakeLock::PART_SATELLITES);
    }

    if(requestingPart == PART_LOGGING) {
        WakeLock::instance().activateBackground(WakeLock::PART_SATELLITES);
    }
}

void SatelliteInfo::deactivate(unsigned requestingPart)
{
    Activateable::deactivate(requestingPart);

    // stop the source if all parts are deactivated
    if(!isActive() && m_sourceActive) {
        qDebug() << "Satellite info source stopped";
        m_source->stopUpdates();
        m_sourceActive = false;
    }

    // the wake lock parts are bits, so only release them with the last user
    if(requestingPart == PART_PAGE && !isPartActive(PART_PAGE)) {
        WakeLock::instance().deactivateScreenLock(WakeLock::PART_SATELLITES);
    }

    if(requestingPart == PART_LOGGING && !isPartActive(PART_LOGGING)) {
        WakeLock::instance().deactivateBackground(WakeLock::PART_SATELLITES);
    }
}

void SatelliteInfo::startLogging()
{
    if(isLogging()) {
        return;
    }

    QString error;

    if(!openNewLogFile(m_logFile, QStringLiteral("satellites"), &error)) {
        qWarning() << error;
        emit loggingFailed(error);
        return;
    }

    activate(Activateable::PART_LOGGING);

    emit isLoggingChanged(true);
}

void SatelliteInfo::stopLogging()
{
    if(!isLogging()) {
        return;
    }

    deactivate(Activateable::PART_LOGGING);

    m_logFile.close();

    emit isLoggingChanged(false);
}
