#include <QDateTime>

#include "rotation.h"


Rotation::Rotation(bool updateInternally, QObject *parent)
    : Sensor(Sensor::Rotation, parent)
{
    m_sensor = new QRotationSensor(this);
    m_logBaseName = QStringLiteral("rotation");

    initSensor(updateInternally);
}

QRotationReading *Rotation::currentReading() const
{
    // null until the backend delivered its first reading: before that the
    // reading object exists but holds zeros, which plotted as a spike from 0
    if(!haveReading()) {
        return nullptr;
    }

    return static_cast<QRotationSensor*>(m_sensor)->reading();
}

void Rotation::logValues()
{
    if(!isLogging()) {
        return;
    }

    QRotationReading *reading = currentReading();

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

void Rotation::refresh()
{
    if(!m_sensor->isActive()) {
        return;
    }

    QRotationReading *reading = currentReading();

    if(!reading) {
        return;
    }

    m_rx = reading->x();
    m_ry = reading->y();
    m_rz = reading->z();

    emit rxChanged();
    emit ryChanged();
    emit rzChanged();

    emit readingUpdated();
}
