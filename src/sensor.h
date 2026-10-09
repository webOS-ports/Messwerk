#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QSensor>
#include <QFile>

#include "activateable.h"

class Sensor : public QObject, public Activateable
{
    Q_OBJECT

    Q_PROPERTY(bool isLogging READ isLogging NOTIFY isLoggingChanged)
    Q_PROPERTY(QString logFileName READ logFileName NOTIFY isLoggingChanged)
    Q_PROPERTY(bool available READ available CONSTANT)

public:
    enum Type {
        Accelerometer,
        Gyroscope,
        Magnetometer,
        PressureSensor,
        Rotation,
        Light,
        Proximity
    };

private:
    static unsigned sensorTypeToWakeLockPart(Type type);

    bool m_available = false;

protected:
    // owned through the QObject parent, set by the subclass constructor
    QSensor *m_sensor = nullptr;
    QFile    m_logFile;
    QString  m_logBaseName;
    Type     m_type;

    // To be called once m_sensor is set
    void initSensor(bool updateInternally);

    void writeLogLine(const QString &line);

public:
    explicit Sensor(Type type, QObject *parent = nullptr);

    Q_INVOKABLE void activate(unsigned requestingPart) override;
    Q_INVOKABLE void deactivate(unsigned requestingPart) override;

    Q_INVOKABLE void startLogging();
    Q_INVOKABLE void stopLogging();
    bool isLogging() const;

    QString logFileName() const;
    bool available() const { return m_available; }

signals:
    void isLoggingChanged(bool);
    // emitted once per refresh, after every value of the reading is updated
    void readingUpdated();
    void loggingFailed(const QString &errorString);

public slots:
    virtual void refresh() = 0;

protected slots:
    virtual void logValues() = 0;
};

#endif // SENSOR_H
