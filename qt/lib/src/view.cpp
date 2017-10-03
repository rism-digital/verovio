/////////////////////////////////////////////////////////////////////////////
// Name:        view.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "vrvqt/view.h"

//----------------------------------------------------------------------------

#include <QSGNode>
#include <memory>

//----------------------------------------------------------------------------

#include "vrvqt/scenegraphdevicecontext.h"
#include "vrvqt/toolkit.h"

namespace vrvQt {
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

void View::setToolkit(Toolkit *toolkit)
{
    if (m_toolkit != toolkit) {
        if (m_documentLayoutChangedConnection) {
            disconnect(m_documentLayoutChangedConnection);
        }
        m_toolkit = toolkit;
        m_documentLayoutChangedConnection = connect(m_toolkit, &Toolkit::documentLayoutChanged, [this]() {
            m_verovioRenderingDirty = true;
            update();
        });

        emit toolkitChanged(m_toolkit);
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
        m_qtDeviceContext = std::make_unique<SceneGraphDeviceContext>(this, node);
    }

    if (m_verovioRenderingDirty) {

        node->removeAllChildNodes();
        for (auto child : childItems()) {
            delete child;
        }
        m_qtDeviceContext->Clear();

        m_toolkit->renderPage(m_pageNumber, m_qtDeviceContext.get());
        m_verovioRenderingDirty = false;

        m_toolkit->setAdjustedDisplayHeightForPage(m_pageNumber, m_qtDeviceContext->GetScaledHeight());
        setHeight(m_toolkit->adjustedDisplayHeightForPage(m_pageNumber));
    }

    return node;
}
} // namespace vrvQt
