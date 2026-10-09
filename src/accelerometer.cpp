#include <QDateTime>

#include <QtMath>

#include "accelerometer.h"


Accelerometer::Accelerometer(bool updateInternally, QObject *parent)
    : Sensor(Sensor::Accelerometer, parent)
{
    m_sensor = new QAccelerometer(this);
    m_logBaseName = QStringLiteral("accelerometer");

    initSensor(updateInternally);
}

QAccelerometerReading *Accelerometer::currentReading() const
{
    // null until the backend delivered its first reading: before that the
    // reading object exists but holds zeros, which plotted as a spike from 0
    if(!haveReading()) {
        return nullptr;
    }

    return static_cast<QAccelerometer*>(m_sensor)->reading();
}

void Accelerometer::logValues()
{
    if(!isLogging()) {
        return;
    }

    QAccelerometerReading *reading = currentReading();

    if(!reading) {
        return;
    }

    QString line = QStringLiteral("%1\t%2\t%3\t%4\n")
                       .arg(QDateTime::currentMSecsSinceEpoch()/1000.0, 0, 'f', 3)
                       .arg(reading->x())
                       .arg(reading->y())
                       .arg(reading->z());

    writeLogLine(line);
}

void Accelerometer::refresh()
{
    if(!m_sensor->isActive()) {
        return;
    }

    QAccelerometerReading *reading = currentReading();

    if(!reading) {
        return;
    }

    m_ax = reading->x();
    m_ay = reading->y();
    m_az = reading->z();

    m_abs = qSqrt(m_ax*m_ax + m_ay*m_ay + m_az*m_az);

    emit axChanged();
    emit ayChanged();
    emit azChanged();
    emit absChanged();

    emit readingUpdated();
}
