/////////////////////////////////////////////////////////////////////////////
// Name:        textquickitem.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "vrvqt/textquickitem.h"

//----------------------------------------------------------------------------

#include <QPainter>

namespace vrvQt {

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
    int textAscent = 0;
    int textDescent = 0;
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

        auto boundingRect = fm.boundingRect(textWithFont.text);

        textWithFont.offset = textWidth;
        textWidth += fm.width(textWithFont.text);
        textAscent = std::max(textAscent, boundingRect.top() * -1);
        textDescent = std::max(textDescent, boundingRect.bottom());
        if (firstLoop) {
            // Note that the bounding rect can start at a negative x (e.g. italic font)
            textXShift = boundingRect.left();
            textWidth -= textXShift;
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

    int textHeight = textDescent + textAscent + 1;

    setImplicitWidth(textWidth);
    setImplicitHeight(textHeight);
}

void TextQuickItem::setColor(QColor color)
{
    m_color = color;
    update();
}

bool TextQuickItem::isEmpty()
{
    return m_textParts.empty();
}

void TextQuickItem::paint(QPainter *painter)
{
    painter->setPen(m_color);

    for (TextWithFont textWithFont : m_textParts) {
        painter->setFont(textWithFont.font);
        painter->drawText(m_paintOffsetX + textWithFont.offset, m_paintOffsetY, textWithFont.text);
    }
}
} // namespace vrvQt
