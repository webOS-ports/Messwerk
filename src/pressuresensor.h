#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include <QPressureSensor>

#include "sensor.h"

class PressureSensor : public Sensor
{
    Q_OBJECT

    Q_PROPERTY(qreal pressure READ pressure NOTIFY pressureChanged)

private:
    qreal m_pressure = 0;

    QPressureReading *currentReading() const;

public:
    explicit PressureSensor(bool updateInternally = false, QObject *parent = nullptr);

    qreal pressure() const { return m_pressure; }

public slots:
    void refresh() override;

protected slots:
    void logValues() override;

signals:
    void pressureChanged();
};

#endif // PRESSURESENSOR_H
