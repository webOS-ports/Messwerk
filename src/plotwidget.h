#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QQuickPaintedItem>
#include <QColor>
#include <QList>

class PlotWidget : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QColor plotColor READ plotColor WRITE setplotColor NOTIFY plotColorChanged)
    Q_PROPERTY(QColor scaleColor READ scaleColor WRITE setscaleColor NOTIFY scaleColorChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE settextColor NOTIFY textColorChanged)
    Q_PROPERTY(unsigned scrollStep READ scrollStep WRITE setscrollStep NOTIFY scrollStepChanged)

private:
    typedef QList<qreal> ValueList;

    QColor m_plotColor;
    QColor m_scaleColor;
    QColor m_textColor;

    ValueList m_values;

    qreal m_minValue = 0;
    qreal m_maxValue = 1;

    unsigned m_scrollStep = 3;

    void updateRange();

public:
    static const unsigned NUM_SCALE_LINES = 5; // number of lines in the background indicating the plot scale

    explicit PlotWidget(QQuickItem *parent = nullptr);

    void paint(QPainter *painter) override;

    void setplotColor(const QColor &color);
    void setscaleColor(const QColor &color);
    void settextColor(const QColor &color);
    void setscrollStep(unsigned step);

    QColor plotColor() const { return m_plotColor; }
    QColor scaleColor() const { return m_scaleColor; }
    QColor textColor() const { return m_textColor; }
    unsigned scrollStep() const { return m_scrollStep; }

signals:
    void plotColorChanged();
    void scaleColorChanged();
    void textColorChanged();
    void scrollStepChanged();

public slots:
    void addValue(qreal v);
    void reset();
};

#endif // PLOTWIDGET_H
