#ifndef PROXIMITY_H
#define PROXIMITY_H

#include <QProximitySensor>

#include "sensor.h"

class Proximity : public Sensor
{
    Q_OBJECT

    Q_PROPERTY(bool detected READ detected NOTIFY detectedChanged)

private:
    bool m_detected = false;

    QProximityReading *currentReading() const;

public:
    explicit Proximity(bool updateInternally = false, QObject *parent = nullptr);

    bool detected() const { return m_detected; }

public slots:
    void refresh() override;

protected slots:
    void logValues() override;

signals:
    void detectedChanged();
};

#endif // PROXIMITY_H
