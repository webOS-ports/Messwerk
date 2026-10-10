#include <QPainter>
#include <QFontInfo>
#include <QLocale>

#include "satellitestrengthwidget.h"

SatelliteStrengthWidget::SatelliteStrengthWidget(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
}

void SatelliteStrengthWidget::setSatelliteInfo(SatelliteInfo *si)
{
    if(m_satelliteInfo == si) {
        return;
    }

    if(m_satelliteInfo) {
        disconnect(m_satelliteInfo, nullptr, this, nullptr);
    }

    m_satelliteInfo = si;

    if(m_satelliteInfo) {
        connect(m_satelliteInfo, &SatelliteInfo::newDataAvailable, this, [this]() { update(); });
    }

    emit satelliteInfoChanged();
    update();
}

void SatelliteStrengthWidget::paint(QPainter *painter)
{
    if(!m_satelliteInfo) {
        return;
    }

    // init pens
    QPen scalePen(m_scaleColor);
    scalePen.setWidth(1);

    QPen usedPen(m_usedColor);
    QPen visiblePen(m_visibleColor);

    // init brushes
    QBrush usedBrush(m_usedColor);
    QBrush visibleBrush(m_visibleColor);

    // init font; pixelSize() is -1 for a font given in points
    QFont font = painter->font();
    font.setPixelSize(qMax(1, int(QFontInfo(font).pixelSize() * 0.7)));
    painter->setFont(font);

    QFontMetrics fontMetrics(font);

    // determine height of graph text
    int textheight = fontMetrics.height();

    // label texts
    int minSignal = qRound(m_satelliteInfo->minSignal());
    int maxSignal = qRound(m_satelliteInfo->maxSignal());

    if(maxSignal <= minSignal) {
        maxSignal = minSignal + 1;
    }

    QString minLabel(QLocale::system().toString(minSignal));
    QString maxLabel(QLocale::system().toString(maxSignal));

    // determine number of label lines to use
    int scaleLabelWidth = qMax(fontMetrics.boundingRect(minLabel).width(), fontMetrics.boundingRect(maxLabel).width());

    int maxLabelWidth = fontMetrics.boundingRect(QStringLiteral("G00")).width();

    int numSatellites = m_satelliteInfo->getNumberOfSatellites();

    // draw scale
    painter->setPen(scalePen);

    if(numSatellites == 0) {
        painter->drawText(QRectF(0, 0, this->width(), this->height()), Qt::AlignCenter, tr("No satellites in view"));
        return;
    }

    // at least one pixel per bar, so a crowded sky does not divide by zero
    int widthPerBar = qMax(1, int(this->width() - scaleLabelWidth) / numSatellites);

    int lines = 1 + maxLabelWidth / widthPerBar;

    int maxY = this->height() - lines * textheight - PADDING;

    painter->drawLine(0, 0, this->width(), 0);
    painter->drawLine(0, maxY, this->width(), maxY);

    painter->drawText(QRectF(this->width(), PADDING, 0, 0), Qt::AlignTop | Qt::AlignRight | Qt::TextDontClip, maxLabel);
    painter->drawText(QRectF(this->width(), maxY - PADDING, 0, 0), Qt::AlignBottom | Qt::AlignRight | Qt::TextDontClip, minLabel);

    // draw satellite bars
    int barCenter = widthPerBar / 2;
    int padding = widthPerBar * 1 / 10;

    int curLine = 0;

    for(const SatelliteInfo::SatelliteData &sd: m_satelliteInfo->satellites()) {
        // draw bar
        qreal barSize = qBound(0.0, qreal(sd.signalStrength - minSignal) / (maxSignal - minSignal), 1.0);

        int left = barCenter - (widthPerBar - padding) / 2;
        int width = widthPerBar - padding;
        int top = maxY * (1 - barSize);
        int height = maxY * barSize;

        QRect barRect(left, top, width, height);

        painter->fillRect(barRect, sd.inUse ? usedBrush : visibleBrush);

        // draw label
        painter->setPen(sd.inUse ? usedPen : visiblePen);

        painter->drawText(QRectF(barCenter, maxY + PADDING + curLine*textheight, 0, 0), Qt::AlignTop | Qt::AlignHCenter | Qt::TextDontClip, sd.label());

        barCenter += widthPerBar;

        curLine++;
        curLine %= lines;
    }
}
