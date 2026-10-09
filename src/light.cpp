#include <QDateTime>

#include "light.h"


Light::Light(bool updateInternally, QObject *parent)
    : Sensor(Sensor::Light, parent)
{
    m_sensor = new QLightSensor(this);
    m_logBaseName = QStringLiteral("light");

    initSensor(updateInternally);
}

QLightReading *Light::currentReading() const
{
    // null until the backend delivered its first reading: before that the
    // reading object exists but holds zeros, which plotted as a spike from 0
    if(!haveReading()) {
        return nullptr;
    }

    return static_cast<QLightSensor*>(m_sensor)->reading();
}

void Light::logValues()
{
    if(!isLogging()) {
        return;
    }

    QLightReading *reading = currentReading();

    if(!reading) {
        return;
    }

    QString line = QStringLiteral("%1\t%2\n")
                       .arg(QDateTime::currentMSecsSinceEpoch()/1000.0, 0, 'f', 3)
                       .arg(reading->lux());

    writeLogLine(line);
}

void Light::refresh()
{
    if(!m_sensor->isActive()) {
        return;
    }

    QLightReading *reading = currentReading();

    if(!reading) {
        return;
    }

    m_brightness = reading->lux();

    emit brightnessChanged();

    emit readingUpdated();
}
