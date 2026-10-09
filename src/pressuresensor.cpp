#include <QDateTime>

#include "pressuresensor.h"


PressureSensor::PressureSensor(bool updateInternally, QObject *parent)
    : Sensor(Sensor::PressureSensor, parent)
{
    m_sensor = new QPressureSensor(this);
    m_logBaseName = QStringLiteral("pressuresensor");

    initSensor(updateInternally);
}

QPressureReading *PressureSensor::currentReading() const
{
    // null until the backend delivered its first reading
    return static_cast<QPressureSensor*>(m_sensor)->reading();
}

void PressureSensor::logValues()
{
    if(!isLogging()) {
        return;
    }

    QPressureReading *reading = currentReading();

    if(!reading) {
        return;
    }

    QString line = QStringLiteral("%1\t%2\n")
                       .arg(QDateTime::currentMSecsSinceEpoch()/1000.0, 0, 'f', 3)
                       .arg(reading->pressure());

    writeLogLine(line);
}

void PressureSensor::refresh()
{
    if(!m_sensor->isActive()) {
        return;
    }

    QPressureReading *reading = currentReading();

    if(!reading) {
        return;
    }

    m_pressure = reading->pressure();

    emit pressureChanged();

    emit readingUpdated();
}
