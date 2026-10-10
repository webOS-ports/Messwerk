#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <QGyroscope>

#include "sensor.h"

class Gyroscope : public Sensor
{
    Q_OBJECT

    Q_PROPERTY(qreal rx READ rx NOTIFY rxChanged)
    Q_PROPERTY(qreal ry READ ry NOTIFY ryChanged)
    Q_PROPERTY(qreal rz READ rz NOTIFY rzChanged)

private:
    qreal m_rx = 0;
    qreal m_ry = 0;
    qreal m_rz = 0;

    QGyroscopeReading *currentReading() const;

public:
    explicit Gyroscope(bool updateInternally = false, QObject *parent = nullptr);

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

#endif // GYROSCOPE_H
