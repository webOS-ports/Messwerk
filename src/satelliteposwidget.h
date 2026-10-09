#ifndef SATELLITEPOSWIDGET_H
#define SATELLITEPOSWIDGET_H

#include <QQuickPaintedItem>
#include <QPointer>

#include "satelliteinfo.h"

class SatellitePosWidget : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QColor visibleColor READ visibleColor WRITE setvisibleColor)
    Q_PROPERTY(QColor usedColor READ usedColor WRITE setusedColor)
    Q_PROPERTY(QColor scaleColor READ scaleColor WRITE setscaleColor)
    Q_PROPERTY(QColor northColor READ northColor WRITE setnorthColor)
    Q_PROPERTY(qreal northDirection READ northDirection WRITE setnorthDirection NOTIFY northDirectionChanged)
    Q_PROPERTY(SatelliteInfo *satelliteInfo READ satelliteInfo WRITE setSatelliteInfo NOTIFY satelliteInfoChanged)

private:
    QColor m_usedColor;
    QColor m_visibleColor;
    QColor m_scaleColor;
    QColor m_northColor;
    qreal m_northDirection = 0;

    // Cleared by QPointer if the source goes away before this item does
    QPointer<SatelliteInfo> m_satelliteInfo;

public:
    explicit SatellitePosWidget(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

    SatelliteInfo *satelliteInfo() const { return m_satelliteInfo.data(); }
    void setSatelliteInfo(SatelliteInfo *si);

    void setvisibleColor(const QColor &color) { m_visibleColor = color; update(); }
    void setusedColor(const QColor &color) { m_usedColor = color; update(); }
    void setscaleColor(const QColor &color) { m_scaleColor = color; update(); }
    void setnorthColor(const QColor &color) { m_northColor = color; update(); }

    QColor visibleColor() const { return m_visibleColor; }
    QColor usedColor() const { return m_usedColor; }
    QColor scaleColor() const { return m_scaleColor; }
    QColor northColor() const { return m_northColor; }

    qreal northDirection() const { return m_northDirection; }

public slots:
    void setnorthDirection(qreal arg);

signals:
    void northDirectionChanged(qreal arg);
    void satelliteInfoChanged();
};

#endif // SATELLITEPOSWIDGET_H
