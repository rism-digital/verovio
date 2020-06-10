/////////////////////////////////////////////////////////////////////////////
// Name:        textquickitem.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "vrvqt/textquickitem.h"

//----------------------------------------------------------------------------

#include <QPainter>

//#define DRAW_DEBUG_BOUNDING_BOXES

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
    // Note: It is not possible to re-use the existing bounding-box of the verovio-object because the stored
    // values may be out-of-date. Thus we have to recalculate the correct position and baseline by examining the font.

    int textWidth = 0;
    int textAscent = 0;
    int textDescent = 0;
    int textXShift = 0;

    bool hasBoldFont = false;
    bool hasItalicFont = false;

    bool firstLoop = true;
    for (TextWithFont &textWithFont : m_textParts) {

        // Note: Initial construction of QFontMetrics is expensive (This function takes ~95% of first updatePaintNode
        // call - e.g 48,5 of 51 ms)! Subsequent calls are much faster, so Qt seems to already have a caching mechanism
        // in place. There is no significant difference between using tightBoundingRect, boundingRect or merely
        // height/width.
        QFontMetrics fm(textWithFont.font);

        // QFontMetrics::boundingRect(QChar) is more accurate than the QString overload, thus we prefer it if we only
        // have 1 symbol (which is often the case, e.g. for music text). Using tightBoundingRect would result in many
        // bounding boxes that are too narrow.
        QRect boundingRect;
        if (textWithFont.text.size() == 1)
            boundingRect = fm.boundingRect(textWithFont.text.front());
        else
            boundingRect = fm.boundingRect(textWithFont.text);

        textWithFont.offset = textWidth;
        textWidth += boundingRect.right() - boundingRect.left();
        textAscent = std::max(textAscent, boundingRect.top() * -1);
        textDescent = std::max(textDescent, boundingRect.bottom());
        if (firstLoop) {
            // Note that the bounding rect can start at a negative x (e.g. italic font)
            textXShift = boundingRect.left();
            if (textXShift < 0) textWidth -= textXShift;
            firstLoop = false;
        }

        if (textWithFont.font.bold()) hasBoldFont = true;
        if (textWithFont.font.italic()) hasItalicFont = true;
    }

    // Security margin for bold/italic fonts because the bounding rects are sometimes too narrow
    if (hasBoldFont || hasItalicFont) {
        textWidth += 4;
        textAscent += 4;
        textDescent += 1;
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

    m_paintOffsetY = textAscent;
    m_paintOffsetX = -textXShift;

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

#ifdef DRAW_DEBUG_BOUNDING_BOXES
    painter->setPen(QColor(Qt::red));
    painter->setBrush(QColor(255, 0, 0, 100));
    painter->drawRect(0, 0, implicitWidth(), implicitHeight());
#endif
}
} // namespace vrvQt
