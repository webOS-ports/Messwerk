#ifndef POSITION_H
#define POSITION_H

#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QFile>
#include <QtNumeric>

#include "activateable.h"

class Position : public QObject, public Activateable
{
    Q_OBJECT

    Q_PROPERTY(bool isLogging READ isLogging NOTIFY isLoggingChanged)
    Q_PROPERTY(QString logFileName READ logFileName NOTIFY isLoggingChanged)
    Q_PROPERTY(bool available READ available CONSTANT)

    Q_PROPERTY(qreal   latitude     READ latitude      NOTIFY latitudeChanged)
    Q_PROPERTY(qreal   longitude    READ longitude     NOTIFY longitudeChanged)
    Q_PROPERTY(qreal   altitude     READ altitude      NOTIFY altitudeChanged)
    Q_PROPERTY(QString maidenhead   READ maidenhead    NOTIFY maidenheadChanged)
    Q_PROPERTY(int     coordType    READ coordType     NOTIFY coordTypeChanged)
    Q_PROPERTY(qreal   horzAccuracy READ horzAccuracy  NOTIFY horzAccuracyChanged)
    Q_PROPERTY(qreal   vertAccuracy READ vertAccuracy  NOTIFY vertAccuracyChanged)

private:
    // null when the device has no position source at all
    QGeoPositionInfoSource *m_source = nullptr;
    bool                    m_sourceActive = false;

    QFile m_logFile;

    // NaN until the first fix, like an invalid QGeoCoordinate
    qreal   m_latitude = qQNaN();
    qreal   m_longitude = qQNaN();
    qreal   m_altitude = qQNaN();
    QString m_maidenhead;
    int     m_coordType = QGeoCoordinate::InvalidCoordinate;
    qreal   m_horzAccuracy = -1;
    qreal   m_vertAccuracy = -1;

    void logData();

    void updateMaidenhead();

public:
    explicit Position(QObject *parent = nullptr);

    Q_INVOKABLE void activate(unsigned requestingPart) override;
    Q_INVOKABLE void deactivate(unsigned requestingPart) override;

    Q_INVOKABLE void startLogging();
    Q_INVOKABLE void stopLogging();

    bool isLogging() const { return isPartActive(Activateable::PART_LOGGING); }
    QString logFileName() const { return isLogging() ? m_logFile.fileName() : QString(); }
    bool available() const { return m_source != nullptr; }

    qreal          latitude() const     { return m_latitude; }
    qreal          longitude() const    { return m_longitude; }
    qreal          altitude() const     { return m_altitude; }
    const QString& maidenhead() const   { return m_maidenhead; }
    int            coordType() const    { return m_coordType; }
    qreal          horzAccuracy() const { return m_horzAccuracy; }
    qreal          vertAccuracy() const { return m_vertAccuracy; }

signals:
    void isLoggingChanged(bool);
    void loggingFailed(const QString &errorString);

    void latitudeChanged();
    void longitudeChanged();
    void altitudeChanged();
    void maidenheadChanged();
    void coordTypeChanged();
    void horzAccuracyChanged();
    void vertAccuracyChanged();

public slots:
    void positionUpdated(const QGeoPositionInfo &posInfo);
};

#endif // POSITION_H
