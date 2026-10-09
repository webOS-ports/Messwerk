#include <QDateTime>

#include "gyroscope.h"


Gyroscope::Gyroscope(bool updateInternally, QObject *parent)
    : Sensor(Sensor::Gyroscope, parent)
{
    m_sensor = new QGyroscope(this);
    m_logBaseName = QStringLiteral("gyroscope");

    initSensor(updateInternally);
}

QGyroscopeReading *Gyroscope::currentReading() const
{
    // null until the backend delivered its first reading
    return static_cast<QGyroscope*>(m_sensor)->reading();
}

void Gyroscope::logValues()
{
    if(!isLogging()) {
        return;
    }

    QGyroscopeReading *reading = currentReading();

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

void Gyroscope::refresh()
{
    if(!m_sensor->isActive()) {
        return;
    }

    QGyroscopeReading *reading = currentReading();

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
