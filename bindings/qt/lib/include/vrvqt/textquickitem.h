/////////////////////////////////////////////////////////////////////////////
// Name:        textquickitem.h
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_QT_TEXTQUICKITEM_H__
#define __VRV_QT_TEXTQUICKITEM_H__

//----------------------------------------------------------------------------

#include <QQuickPaintedItem>

namespace vrvQt {

//----------------------------------------------------------------------------
// TextQuickItem
//----------------------------------------------------------------------------

/**
 * This class implements a custom QQuickItem for drawing text. It is used for Music-Text (e.g. notes) and normal text
 * (e.g. lyrics).
 */
class TextQuickItem : public QQuickPaintedItem {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    TextQuickItem();
    virtual ~TextQuickItem() = default;
    ///@}

    /**
     * Appends the given text with the specified font to this text item.
     */
    void appendText(QString text, QFont font);

    /**
     * Setter and Getter for the text alignment.
     */
    void setAlignment(Qt::Alignment alignment) { m_alignment = alignment; }
    Qt::Alignment getAlignment() { return m_alignment; }

    /**
     * This function calculates the correct position (x and y) and the bounding box. This is done by evaluating the
     * alignment and the font metrics.
     */
    void calcPos();

    /**
     * Sets the text color to the given value.
     */
    void setColor(QColor color);

    /**
      * Returns if the text item is empty, i.e. it has no text elements.
      */
    bool isEmpty();

    /**
     * Overrides the paint function from QQuickPaintedItem. This function is called for drawing the text.
     */
    void paint(QPainter *painter) override;

private:
    // Internally used structure for grouping text, font and offset.
    struct TextWithFont {
        TextWithFont(QString text_, QFont font_) : text(text_), font(font_) {}
        int offset{ 0 };
        QString text;
        QFont font;
    };

    // Stores all text that belongs to this text item.
    QList<TextWithFont> m_textParts;

    int m_paintOffsetX{ 0 };
    int m_paintOffsetY{ 0 };
    Qt::Alignment m_alignment{ Qt::AlignLeft };
    QColor m_color{ Qt::black };
};
} // namespace vrvQt

#endif // __VRV_QT_TEXTQUICKITEM_H__
