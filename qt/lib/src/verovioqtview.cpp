/////////////////////////////////////////////////////////////////////////////
// Name:        veroviopage.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "verovioqtview.h"

//----------------------------------------------------------------------------

#include <QSGNode>
#include <memory>

//----------------------------------------------------------------------------

#include "qtscenegraphdevicecontext.h"
#include "verovioqttoolkit.h"

namespace vrv_qt {
View::View()
{
    setFlag(ItemHasContents, true);
}

View::~View()
{
    if (m_documentLayoutChangedConnection) {
        disconnect(m_documentLayoutChangedConnection);
    }
}

void View::setVerovioDoc(Toolkit *verovioDoc)
{
    if (m_verovioDoc != verovioDoc) {
        if (m_documentLayoutChangedConnection) {
            disconnect(m_documentLayoutChangedConnection);
        }
        m_verovioDoc = verovioDoc;
        m_documentLayoutChangedConnection = connect(m_verovioDoc, &Toolkit::documentLayoutChanged, [this]() {
            m_verovioRenderingDirty = true;
            update();
        });

        emit verovioDocChanged(m_verovioDoc);
    }
}

void View::setPageNumber(int pageNumber)
{
    if (m_pageNumber != pageNumber) {
        m_pageNumber = pageNumber;
        update();
    };
}

QSGNode *View::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
    if (node == nullptr) {
        node = new QSGNode();
    }
    if (m_qtDeviceContext == nullptr) {
        m_qtDeviceContext = std::make_unique<QtSceneGraphDeviceContext>(this, node);
    }

    if (m_verovioRenderingDirty) {

        node->removeAllChildNodes();
        for (auto child : childItems()) {
            delete child;
        }
        m_qtDeviceContext->Clear();

        m_verovioDoc->renderPage(m_pageNumber, m_qtDeviceContext.get());
        m_verovioRenderingDirty = false;

        m_verovioDoc->setAdjustedPageHeightForPage(m_pageNumber, m_qtDeviceContext->GetScaledHeight());
        setHeight(m_verovioDoc->adjustedPageHeightForPage(m_pageNumber));
    }

    return node;
}
} // namespace vrv_qt
