#ifndef LIGHT_H
#define LIGHT_H

#include <QLightSensor>

#include "sensor.h"

class Light : public Sensor
{
    Q_OBJECT

    Q_PROPERTY(qreal brightness READ brightness NOTIFY brightnessChanged)

private:
    qreal m_brightness = 0;

    QLightReading *currentReading() const;

public:
    explicit Light(bool updateInternally = false, QObject *parent = nullptr);

    qreal brightness() const { return m_brightness; }

public slots:
    void refresh() override;

protected slots:
    void logValues() override;

signals:
    void brightnessChanged();
};

#endif // LIGHT_H
