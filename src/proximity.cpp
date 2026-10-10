#include <QDateTime>

#include "proximity.h"


Proximity::Proximity(bool updateInternally, QObject *parent)
    : Sensor(Sensor::Proximity, parent)
{
    m_sensor = new QProximitySensor(this);
    m_logBaseName = QStringLiteral("proximity");

    initSensor(updateInternally);
}

QProximityReading *Proximity::currentReading() const
{
    // null until the backend delivered its first reading: before that the
    // reading object exists but holds zeros, which plotted as a spike from 0
    if(!haveReading()) {
        return nullptr;
    }

    return static_cast<QProximitySensor*>(m_sensor)->reading();
}

void Proximity::logValues()
{
    if(!isLogging()) {
        return;
    }

    QProximityReading *reading = currentReading();

    if(!reading) {
        return;
    }

    QString line = QStringLiteral("%1\t%2\n")
                       .arg(QDateTime::currentMSecsSinceEpoch()/1000.0, 0, 'f', 3)
                       .arg(int(reading->close()));

    writeLogLine(line);
}

void Proximity::refresh()
{
    if(!m_sensor->isActive()) {
        return;
    }

    QProximityReading *reading = currentReading();

    if(!reading) {
        return;
    }

    m_detected = reading->close();

    emit detectedChanged();

    emit readingUpdated();
}
