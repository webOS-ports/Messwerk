#include <QPainter>
#include <QFontInfo>
#include <QLocale>
#include <QtNumeric>

#include "plotwidget.h"

PlotWidget::PlotWidget(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
}

void PlotWidget::paint(QPainter *painter)
{
    int minX = 0;

    QPen scalePen(m_scaleColor);
    scalePen.setWidth(1);

    QPen textPen(m_textColor.isValid() ? m_textColor : m_plotColor);

    QPen plotPen(m_plotColor);
    plotPen.setWidth(2);


    // draw NUM_SCALE_LINES lines for scale indication
    QFont font = painter->font();
    // pixelSize() is -1 for a font given in points; QFontInfo always knows
    font.setPixelSize(qMax(1, int(QFontInfo(font).pixelSize() * 0.7)));
    font.setBold(true);
    painter->setFont(font);

    painter->setRenderHint(QPainter::Antialiasing, true);

    QFontMetrics fontMetrics(font);

    qreal step = (m_maxValue - m_minValue) / NUM_SCALE_LINES;

    for(unsigned i = 0; i < NUM_SCALE_LINES; i++) {
        qreal v = m_minValue + (i + 0.5) * step;

        int ypos = (1.0 - (i + 0.5) / NUM_SCALE_LINES) * this->height() - 1;

        QString text = QLocale::system().toString(v, 'g', 3);
        int startX = fontMetrics.boundingRect(text).width() + 5;

        if(startX > minX) {
            minX = startX;
        }

        painter->setPen(scalePen);
        painter->drawLine(startX, ypos, this->width(), ypos);

        painter->setPen(textPen);
        painter->drawText(QRect(1, ypos, 0, 0), Qt::AlignVCenter | Qt::AlignLeft | Qt::TextDontClip, text);
    }

    // draw the plot, newest value at the right edge
    painter->setPen(plotPen);

    QPointF prevPoint;
    bool havePrevPoint = false;
    qreal x = this->width();

    for(int i = m_values.size() - 1; i >= 0 && x >= minX; i--, x -= m_scrollStep) {
        qreal v = m_values.at(i);

        if(!qIsFinite(v)) {
            havePrevPoint = false;
            continue;
        }

        // normalized value between 0 and 1
        qreal vnorm = (v - m_minValue) / (m_maxValue - m_minValue);

        QPointF curPoint(x, this->height() * (1 - vnorm));

        if(havePrevPoint) {
            painter->drawLine(prevPoint, curPoint);
        }

        prevPoint = curPoint;
        havePrevPoint = true;
    }
}

void PlotWidget::updateRange()
{
    m_minValue =  1e27;
    m_maxValue = -1e27;

    for(qreal value: std::as_const(m_values)) {
        if(!qIsFinite(value)) {
            continue;
        }

        if(value < m_minValue) {
            m_minValue = value;
        }

        if(value > m_maxValue) {
            m_maxValue = value;
        }
    }

    if(m_minValue > m_maxValue) {
        // nothing finite to show
        m_minValue = 0;
        m_maxValue = 1;
    } else if(m_minValue == m_maxValue) {
        m_minValue -= 0.5;
        m_maxValue += 0.5;
    }
}

void PlotWidget::addValue(qreal v)
{
    m_values.append(v);

    // one more than fits, so the oldest line still runs into the left edge
    qsizetype valuesRequired = qsizetype(this->width() / m_scrollStep) + 2;

    if(m_values.size() > valuesRequired) {
        m_values.erase(m_values.begin(), m_values.end() - valuesRequired);
    }

    updateRange();
}

void PlotWidget::reset()
{
    m_values.clear();
    updateRange();
    update();
}

void PlotWidget::setplotColor(const QColor &color)
{
    if(m_plotColor != color) {
        m_plotColor = color;
        emit plotColorChanged();
        update();
    }
}

void PlotWidget::setscaleColor(const QColor &color)
{
    if(m_scaleColor != color) {
        m_scaleColor = color;
        emit scaleColorChanged();
        update();
    }
}

void PlotWidget::settextColor(const QColor &color)
{
    if(m_textColor != color) {
        m_textColor = color;
        emit textColorChanged();
        update();
    }
}

void PlotWidget::setscrollStep(unsigned step)
{
    // a step of 0 would divide by zero in addValue()
    step = qMax(1u, step);

    if(m_scrollStep != step) {
        m_scrollStep = step;
        emit scrollStepChanged();
        update();
    }
}
