#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <QAccelerometer>

#include "sensor.h"

class Accelerometer : public Sensor
{
    Q_OBJECT

    Q_PROPERTY(qreal ax  READ ax  NOTIFY axChanged)
    Q_PROPERTY(qreal ay  READ ay  NOTIFY ayChanged)
    Q_PROPERTY(qreal az  READ az  NOTIFY azChanged)
    Q_PROPERTY(qreal abs READ abs NOTIFY absChanged)

private:
    qreal m_ax = 0;
    qreal m_ay = 0;
    qreal m_az = 0;
    qreal m_abs = 0;

    QAccelerometerReading *currentReading() const;

public:
    explicit Accelerometer(bool updateInternally = false, QObject *parent = nullptr);

    qreal ax() const { return m_ax; }
    qreal ay() const { return m_ay; }
    qreal az() const { return m_az; }
    qreal abs() const { return m_abs; }

public slots:
    void refresh() override;

protected slots:
    void logValues() override;

signals:
    void axChanged();
    void ayChanged();
    void azChanged();
    void absChanged();
};

#endif // ACCELEROMETER_H
