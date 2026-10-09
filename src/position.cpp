#include <QDateTime>
#include <QDebug>
#include <QStringList>

#include "logfile.h"
#include "wakelock.h"

#include "position.h"

void Position::logData()
{
    if(isLogging()) {
        /* Log file line format:
         * timestamp<tab>latitude<tab>longitude<tab>altitude\n
         */
        QString line = QStringLiteral("%1\t%2\t%3\t%4\n")
                               .arg(QDateTime::currentMSecsSinceEpoch()/1000.0, 0, 'f', 3)
                               .arg(m_latitude)
                               .arg(m_longitude)
                               .arg(m_altitude);

        m_logFile.write(line.toUtf8());
    }
}

void Position::updateMaidenhead()
{
    const unsigned FIELDS     = 18;
    const unsigned SQUARES    = 10;
    const unsigned SUBSQUARES = 24;

    const unsigned GRIDLEN = FIELDS * SQUARES * SUBSQUARES;

    m_maidenhead.clear();

    // a NaN or out of range value would be undefined behaviour in the
    // conversion to unsigned below
    if(!(m_latitude >= -90 && m_latitude <= 90 && m_longitude >= -180 && m_longitude <= 180)) {
        return;
    }

    // the north pole and the antimeridian belong to the last field, not to a 19th
    unsigned gridIdxLon = qMin(unsigned((m_longitude + 180) * GRIDLEN / 360.0), GRIDLEN - 1);
    unsigned gridIdxLat = qMin(unsigned((m_latitude  + 90)  * GRIDLEN / 180.0), GRIDLEN - 1);

    unsigned fieldLon = gridIdxLon / (SQUARES * SUBSQUARES);
    unsigned fieldLat = gridIdxLat / (SQUARES * SUBSQUARES);

    unsigned squareLon = (gridIdxLon / SUBSQUARES) % SQUARES;
    unsigned squareLat = (gridIdxLat / SUBSQUARES) % SQUARES;

    unsigned subSquareLon = gridIdxLon % SUBSQUARES;
    unsigned subSquareLat = gridIdxLat % SUBSQUARES;

    m_maidenhead += QChar('A' + fieldLon);
    m_maidenhead += QChar('A' + fieldLat);

    m_maidenhead += QChar('0' + squareLon);
    m_maidenhead += QChar('0' + squareLat);

    m_maidenhead += QChar('a' + subSquareLon);
    m_maidenhead += QChar('a' + subSquareLat);
}

Position::Position(QObject *parent) :
    QObject(parent)
{
    m_source = QGeoPositionInfoSource::createDefaultSource(this);

    if(!m_source) {
        qWarning() << "No position source available";
        return;
    }

    connect(m_source, &QGeoPositionInfoSource::positionUpdated, this, &Position::positionUpdated);
}


void Position::activate(unsigned requestingPart)
{
    Activateable::activate(requestingPart);

    if(m_source && !m_sourceActive) {
        qDebug() << "Position info source started";
        m_source->startUpdates();
        m_sourceActive = true;
    }

    if(requestingPart == PART_PAGE) {
        WakeLock::instance().activateScreenLock(WakeLock::PART_POSITION);
    }

    if(requestingPart == PART_LOGGING) {
        WakeLock::instance().activateBackground(WakeLock::PART_POSITION);
    }
}

void Position::deactivate(unsigned requestingPart)
{
    Activateable::deactivate(requestingPart);

    // stop the source if all parts are deactivated
    if(!isActive() && m_sourceActive) {
        qDebug() << "Position info source stopped";
        m_source->stopUpdates();
        m_sourceActive = false;
    }

    // the wake lock parts are bits, so only release them with the last user
    if(requestingPart == PART_PAGE && !isPartActive(PART_PAGE)) {
        WakeLock::instance().deactivateScreenLock(WakeLock::PART_POSITION);
    }

    if(requestingPart == PART_LOGGING && !isPartActive(PART_LOGGING)) {
        WakeLock::instance().deactivateBackground(WakeLock::PART_POSITION);
    }
}

void Position::startLogging()
{
    if(isLogging()) {
        return;
    }

    QString error;

    if(!openNewLogFile(m_logFile, QStringLiteral("position"), &error)) {
        qWarning() << error;
        emit loggingFailed(error);
        return;
    }

    activate(Activateable::PART_LOGGING);

    emit isLoggingChanged(true);
}

void Position::stopLogging()
{
    if(!isLogging()) {
        return;
    }

    deactivate(Activateable::PART_LOGGING);

    m_logFile.close();

    emit isLoggingChanged(false);
}

void Position::positionUpdated(const QGeoPositionInfo &posInfo)
{
    // update coordinates
    m_latitude  = posInfo.coordinate().latitude();
    m_longitude = posInfo.coordinate().longitude();
    m_altitude  = posInfo.coordinate().altitude();
    m_coordType = posInfo.coordinate().type();

    emit latitudeChanged();
    emit longitudeChanged();
    emit altitudeChanged();
    emit coordTypeChanged();

    // update attributes
    if(posInfo.hasAttribute(QGeoPositionInfo::HorizontalAccuracy)) {
        m_horzAccuracy = posInfo.attribute(QGeoPositionInfo::HorizontalAccuracy);
    } else {
        m_horzAccuracy = -1;
    }

    if(posInfo.hasAttribute(QGeoPositionInfo::VerticalAccuracy)) {
        m_vertAccuracy = posInfo.attribute(QGeoPositionInfo::VerticalAccuracy);
    } else {
        m_vertAccuracy = -1;
    }

    emit horzAccuracyChanged();
    emit vertAccuracyChanged();

    // recalculate Maidenhead locator
    updateMaidenhead();

    emit maidenheadChanged();

    logData();
}
