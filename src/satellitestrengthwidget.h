#ifndef SATELLITESTRENGTHWIDGET_H
#define SATELLITESTRENGTHWIDGET_H

#include <QQuickPaintedItem>
#include <QPointer>

#include "satelliteinfo.h"

class SatelliteStrengthWidget : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QColor visibleColor READ visibleColor WRITE setvisibleColor)
    Q_PROPERTY(QColor usedColor READ usedColor WRITE setusedColor)
    Q_PROPERTY(QColor scaleColor READ scaleColor WRITE setscaleColor)
    Q_PROPERTY(SatelliteInfo *satelliteInfo READ satelliteInfo WRITE setSatelliteInfo NOTIFY satelliteInfoChanged)

private:
    static const int PADDING = 1;

    QColor m_usedColor;
    QColor m_visibleColor;
    QColor m_scaleColor;

    // Cleared by QPointer if the source goes away before this item does
    QPointer<SatelliteInfo> m_satelliteInfo;

public:
    explicit SatelliteStrengthWidget(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

    SatelliteInfo *satelliteInfo() const { return m_satelliteInfo.data(); }
    void setSatelliteInfo(SatelliteInfo *si);

    void setvisibleColor(const QColor &color) { m_visibleColor = color; update(); }
    void setusedColor(const QColor &color) { m_usedColor = color; update(); }
    void setscaleColor(const QColor &color) { m_scaleColor = color; update(); }

    QColor visibleColor() const { return m_visibleColor; }
    QColor usedColor() const { return m_usedColor; }
    QColor scaleColor() const { return m_scaleColor; }

signals:
    void satelliteInfoChanged();
};

#endif // SATELLITESTRENGTHWIDGET_H
