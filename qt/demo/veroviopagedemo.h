/////////////////////////////////////////////////////////////////////////////
// Name:        veroviopagedemo.h
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VEROVIOPAGEDEMO_H__
#define __VEROVIOPAGEDEMO_H__

//----------------------------------------------------------------------------

#include <QColor>
#include <QMouseEvent>
#include <QString>
#include <QStringList>

//----------------------------------------------------------------------------

#include "verovio-qt/veroviopage.h"

//----------------------------------------------------------------------------
// VerovioPageDemo
//----------------------------------------------------------------------------

/**
 * This is a small demo example to show how to extend the VerovioPage by additional functionality.
 */
class VerovioPageDemo : public vrv::VerovioPage {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    VerovioPageDemo();
    virtual ~VerovioPageDemo() = default;
    ///@}

protected:
    /**
     * @name Reimplemented function from VerovioPage.
     */
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *updatePaintNodeData) override;

    /**
     * @name Reimplemented function from QQuickItem.
     */
    void mousePressEvent(QMouseEvent *ev) override;

private:
    // The following functions update the highlighting of notes.
    void updateHighlighting();
    void setQuickItemColor(QString elementId, QColor color);
    void setQsgNodeColor(QString elementId, QColor color);

    // Stores which elements are highlighted and which should be toggled.
    QStringList m_highlightElementsCurrent;
    QStringList m_highlightElementsToggle;
};

#endif // __VEROVIOPAGEDEMO_H__
