#include <QDateTime>

#include "magnetometer.h"


Magnetometer::Magnetometer(bool updateInternally, QObject *parent)
    : Sensor(Sensor::Magnetometer, parent)
{
    m_sensor = new QMagnetometer(this);
    m_logBaseName = QStringLiteral("magnetometer");

    initSensor(updateInternally);
}

QMagnetometerReading *Magnetometer::currentReading() const
{
    // null until the backend delivered its first reading: before that the
    // reading object exists but holds zeros, which plotted as a spike from 0
    if(!haveReading()) {
        return nullptr;
    }

    return static_cast<QMagnetometer*>(m_sensor)->reading();
}

void Magnetometer::logValues()
{
    if(!isLogging()) {
        return;
    }

    QMagnetometerReading *reading = currentReading();

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

void Magnetometer::refresh()
{
    if(!m_sensor->isActive()) {
        return;
    }

    QMagnetometerReading *reading = currentReading();

    if(!reading) {
        return;
    }

    m_mx = reading->x();
    m_my = reading->y();
    m_mz = reading->z();
    m_precision = reading->calibrationLevel();

    emit mxChanged();
    emit myChanged();
    emit mzChanged();
    emit precisionChanged();

    emit readingUpdated();
}
