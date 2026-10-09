#include <QDebug>

#include "logfile.h"
#include "wakelock.h"

#include "sensor.h"


unsigned Sensor::sensorTypeToWakeLockPart(Sensor::Type type)
{
    switch(type) {
        case Accelerometer: return WakeLock::PART_ACCELEROMETER;
        case Gyroscope:     return WakeLock::PART_GYROSCOPE;
        case Magnetometer:  return WakeLock::PART_MAGNETOMETER;
        case PressureSensor:return WakeLock::PART_PRESSURESENSOR;
        case Rotation:      return WakeLock::PART_ROTATION;
        case Light:         return WakeLock::PART_LIGHT;
        case Proximity:     return WakeLock::PART_PROXIMITY;
        default:            return 0;
    }
}

Sensor::Sensor(Sensor::Type type, QObject *parent) :
    QObject(parent), m_type(type)
{
}

void Sensor::initSensor(bool updateInternally)
{
    Q_ASSERT(m_sensor);

    // Only tells whether the device has such a sensor at all; start() still
    // decides whether it actually runs
    m_available = m_sensor->connectToBackend();

    if(!m_available) {
        qWarning() << "No backend for" << m_sensor->type();
    }

    if(updateInternally) {
        connect(m_sensor, &QSensor::readingChanged, this, &Sensor::refresh);
    }

    connect(m_sensor, &QSensor::readingChanged, this, &Sensor::logValues);
}

void Sensor::activate(unsigned requestingPart)
{
    Q_ASSERT(m_sensor);

    Activateable::activate(requestingPart);

    if(!m_sensor->isActive()) {
        qDebug() << "Sensor started";
        m_sensor->setAlwaysOn(true);
        m_sensor->start();
    }

    if(requestingPart == PART_PAGE) {
        WakeLock::instance().activateScreenLock(sensorTypeToWakeLockPart(m_type));
    }

    if(requestingPart == PART_LOGGING) {
        WakeLock::instance().activateBackground(sensorTypeToWakeLockPart(m_type));
    }
}

void Sensor::deactivate(unsigned requestingPart)
{
    Q_ASSERT(m_sensor);

    Activateable::deactivate(requestingPart);

    // stop the sensor if all parts are deactivated
    if(!isActive() && m_sensor->isActive()) {
        qDebug() << "Sensor stopped";
        m_sensor->stop();
    }

    // the wake lock parts are bits, so only release them with the last user
    if(requestingPart == PART_PAGE && !isPartActive(PART_PAGE)) {
        WakeLock::instance().deactivateScreenLock(sensorTypeToWakeLockPart(m_type));
    }

    if(requestingPart == PART_LOGGING && !isPartActive(PART_LOGGING)) {
        WakeLock::instance().deactivateBackground(sensorTypeToWakeLockPart(m_type));
    }
}

void Sensor::startLogging()
{
    if(isLogging()) {
        return;
    }

    QString error;

    if(!openNewLogFile(m_logFile, m_logBaseName, &error)) {
        qWarning() << error;
        emit loggingFailed(error);
        return;
    }

    activate(Activateable::PART_LOGGING);

    emit isLoggingChanged(true);
}

void Sensor::stopLogging()
{
    if(!isLogging()) {
        return;
    }

    deactivate(Activateable::PART_LOGGING);

    m_logFile.close();

    emit isLoggingChanged(false);
}

bool Sensor::isLogging() const
{
    return isPartActive(Activateable::PART_LOGGING);
}

QString Sensor::logFileName() const
{
    return isLogging() ? m_logFile.fileName() : QString();
}

void Sensor::writeLogLine(const QString &line)
{
    if(m_logFile.isOpen()) {
        m_logFile.write(line.toUtf8());
    }
}
