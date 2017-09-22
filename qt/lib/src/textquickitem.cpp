/////////////////////////////////////////////////////////////////////////////
// Name:        textquickitem.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "textquickitem.h"

//----------------------------------------------------------------------------

#include <QPainter>

namespace vrv {

TextQuickItem::TextQuickItem()
{
    // TODO: Try following optimization hints on different platforms
    // setRenderTarget(QQuickPaintedItem::FramebufferObject);
    // setPerformanceHint(QQuickPaintedItem::FastFBOResizing);
    // setAntialiasing(true);
    // setMipmap(true);
}

void TextQuickItem::appendText(QString text, QFont font)
{
    m_textParts.append(TextWithFont(text, font));
}

void TextQuickItem::calcPos()
{
    int textWidth = 0;
    int textHeight = 0;
    int textAscent = 0;
    int textXShift = 0;

    bool firstLoop = true;
    for (TextWithFont &textWithFont : m_textParts) {

        // Note: Initial construction of QFontMetrics is expensive (This function takes ~95% of first updatePaintNode
        // call - e.g 48,5 of 51 ms)! Subsequent calls are much faster, so Qt seems to already have a caching mechanism
        // in place. There is no significant difference between using tightBoundingRect, boundingRect or merely
        // height/width.
        // TODO: use already existing font calculation from BBoxDeviceContext (which is currently not stored anywhere)
        // for ascent, descent, height, width
        QFontMetrics fm(textWithFont.font);

        // Note: tightBoundingRect is not recommended on Windows due to performance reasons
        auto boundingRect = fm.tightBoundingRect(textWithFont.text);

        textWithFont.offset = textWidth;
        textWidth += boundingRect.width();
        textHeight = std::max(textHeight, boundingRect.height());
        textAscent = std::max(textAscent, boundingRect.top() * -1);
        if (firstLoop) {
            // Note that the bounding rect can start at a negative x (e.g. italic font)
            textXShift = boundingRect.left();
            firstLoop = false;
        }
    }
    qreal newX = x() + textXShift;
    if (m_alignment == Qt::AlignRight) {
        setX(newX - textWidth);
    }
    else if (m_alignment == Qt::AlignHCenter) {
        setX(newX - textWidth / 2);
    }
    else {
        setX(newX);
    }
    setY(y() - textAscent);

    m_paintOffsetY += textAscent;
    m_paintOffsetX += textXShift * -1;

    setImplicitWidth(textWidth);
    setImplicitHeight(textHeight);
}

void TextQuickItem::setColor(QColor color)
{
    m_color = color;
    update();
}

void TextQuickItem::paint(QPainter *painter)
{
    painter->setPen(m_color);

    for (TextWithFont textWithFont : m_textParts) {
        painter->setFont(textWithFont.font);
        painter->drawText(m_paintOffsetX + textWithFont.offset, m_paintOffsetY, textWithFont.text);
    }
}
} // namespace vrv
