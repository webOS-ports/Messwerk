#ifndef MAGNETOMETER_H
#define MAGNETOMETER_H

#include <QMagnetometer>

#include "sensor.h"

class Magnetometer : public Sensor
{
    Q_OBJECT

    Q_PROPERTY(qreal mx        READ mx        NOTIFY mxChanged)
    Q_PROPERTY(qreal my        READ my        NOTIFY myChanged)
    Q_PROPERTY(qreal mz        READ mz        NOTIFY mzChanged)
    Q_PROPERTY(qreal precision READ precision NOTIFY precisionChanged)

private:
    qreal m_mx = 0;
    qreal m_my = 0;
    qreal m_mz = 0;
    qreal m_precision = 0;

    QMagnetometerReading *currentReading() const;

public:
    explicit Magnetometer(bool updateInternally = false, QObject *parent = nullptr);

    qreal mx() const { return m_mx; }
    qreal my() const { return m_my; }
    qreal mz() const { return m_mz; }
    qreal precision() const { return m_precision; }

public slots:
    void refresh() override;

protected slots:
    void logValues() override;

signals:
    void mxChanged();
    void myChanged();
    void mzChanged();
    void precisionChanged();
};

#endif // MAGNETOMETER_H
