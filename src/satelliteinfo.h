#ifndef SATELLITEINFO_H
#define SATELLITEINFO_H

#include <QObject>
#include <QGeoSatelliteInfo>
#include <QGeoSatelliteInfoSource>
#include <QMap>
#include <QSet>
#include <QFile>

#include "activateable.h"

class SatelliteInfo : public QObject, public Activateable
{
    Q_OBJECT

    Q_PROPERTY(bool isLogging READ isLogging NOTIFY isLoggingChanged)
    Q_PROPERTY(QString logFileName READ logFileName NOTIFY isLoggingChanged)
    Q_PROPERTY(bool available READ available CONSTANT)
    Q_PROPERTY(int numberOfSatellites READ getNumberOfSatellites NOTIFY newDataAvailable)
    Q_PROPERTY(int numberOfUsedSatellites READ getNumberOfUsedSatellites NOTIFY newDataAvailable)

public:
    struct SatelliteData {
        int    id = 0;               // PRN, only unique within one system
        qreal  azimuth = 0;          // in Degrees
        qreal  elevation = 0;        // in Degrees
        int    signalStrength = 0;   // in dB
        bool   inUse = false;
        qint64 lastUpdated = 0;      // in milliseconds
        QGeoSatelliteInfo::SatelliteSystem satelliteSystem = QGeoSatelliteInfo::Undefined;

        void updateFrom(const SatelliteData& other);

        // "G12", "R05", ...: the system letter used in RINEX and NMEA
        QString label() const;
    };

    // The system in the upper bits and the PRN in the lower ones: GPS 5 and
    // GLONASS 5 are two different satellites
    typedef qint64 SatelliteID;
    typedef QMap<SatelliteID, SatelliteData> SatelliteMap;

    static const qint64 TIMEOUT = 10000; // timeout of satellites in milliseconds

private:
    typedef QSet<SatelliteID> SatelliteIDSet;

    SatelliteMap m_satellites;

    SatelliteIDSet m_used;

    qreal m_minSignal = 0;
    qreal m_maxSignal = 1;

    // null when the device has no satellite source at all
    QGeoSatelliteInfoSource *m_source = nullptr;
    bool                     m_sourceActive = false;

    QFile m_logFile;

    static SatelliteID makeId(const QGeoSatelliteInfo &sat);
    static SatelliteData dataFrom(const QGeoSatelliteInfo &sat);

    void mergeSatellite(const QGeoSatelliteInfo &sat);
    void updateUsedFlags();
    void updateMinMax();
    void logData();

public:
    explicit SatelliteInfo(QObject *parent = nullptr);

    int getNumberOfSatellites() const { return m_satellites.size(); }
    int getNumberOfUsedSatellites() const;

    // Only valid until the next update; iterate it, do not keep it
    const SatelliteMap &satellites() const { return m_satellites; }

    qreal minSignal() const { return m_minSignal; }
    qreal maxSignal() const { return m_maxSignal; }

    bool available() const { return m_source != nullptr; }

    Q_INVOKABLE void activate(unsigned requestingPart) override;
    Q_INVOKABLE void deactivate(unsigned requestingPart) override;

    Q_INVOKABLE void startLogging();
    Q_INVOKABLE void stopLogging();

    bool isLogging() const { return isPartActive(Activateable::PART_LOGGING); }
    QString logFileName() const { return isLogging() ? m_logFile.fileName() : QString(); }

signals:
    void newDataAvailable();
    void isLoggingChanged(bool);
    void loggingFailed(const QString &errorString);

public slots:
    void setSatellitesInUse(const QList<QGeoSatelliteInfo> &satellites);
    void setSatellitesInView(const QList<QGeoSatelliteInfo> &satellites);
};

#endif // SATELLITEINFO_H
