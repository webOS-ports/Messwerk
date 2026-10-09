#ifndef ROTATION_H
#define ROTATION_H

#include <QRotationSensor>

#include "sensor.h"

class Rotation : public Sensor
{
    Q_OBJECT

    Q_PROPERTY(qreal rx READ rx NOTIFY rxChanged)
    Q_PROPERTY(qreal ry READ ry NOTIFY ryChanged)
    Q_PROPERTY(qreal rz READ rz NOTIFY rzChanged)

private:
    qreal m_rx = 0;
    qreal m_ry = 0;
    qreal m_rz = 0;

    QRotationReading *currentReading() const;

public:
    explicit Rotation(bool updateInternally = false, QObject *parent = nullptr);

    qreal rx() const { return m_rx; }
    qreal ry() const { return m_ry; }
    qreal rz() const { return m_rz; }

public slots:
    void refresh() override;

protected slots:
    void logValues() override;

signals:
    void rxChanged();
    void ryChanged();
    void rzChanged();
};

#endif // ROTATION_H
