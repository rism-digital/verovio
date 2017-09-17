/////////////////////////////////////////////////////////////////////////////
// Name:        veroviopagedemo.cpp
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "veroviopagedemo.h"

//----------------------------------------------------------------------------

#include <QSGFlatColorMaterial>

//----------------------------------------------------------------------------

#include "textquickitem.h"

VerovioPageDemo::VerovioPageDemo()
{
    setAcceptedMouseButtons(Qt::AllButtons);
}

QSGNode *VerovioPageDemo::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{
    node = VerovioPage::updatePaintNode(node, updatePaintNodeData);
    if (!m_highlightElementsCurrent.isEmpty() || !m_highlightElementsToggle.isEmpty()) {
        updateHighlighting();
    }
    return node;
}

void VerovioPageDemo::mousePressEvent(QMouseEvent *ev)
{
    if (m_qtDeviceContext == nullptr) return;

    auto child = childAt(ev->x(), ev->y());

    if (child) {
        QStringList ids = m_qtDeviceContext->GetIdsForQuickItem(child);
        auto siter = std::find_if(ids.begin(), ids.end(), [](QString string) {
            qDebug() << string;
            return string.startsWith("note-") || string.startsWith("rest-");
        });
        if (siter != ids.end()) {
            m_highlightElementsToggle << *siter;
        }
        update();
    }
}

void VerovioPageDemo::updateHighlighting()
{
    for (auto highlightElement : m_highlightElementsToggle) {
        if (m_highlightElementsCurrent.contains(highlightElement)) {
            setQsgNodeColor(highlightElement, Qt::black);
            setQuickItemColor(highlightElement, Qt::black);
            m_highlightElementsCurrent.removeAll(highlightElement);
        }
        else {
            m_highlightElementsCurrent << highlightElement;
        }
    }
    m_highlightElementsToggle.clear();

    for (auto highlightElement : m_highlightElementsCurrent) {
        setQsgNodeColor(highlightElement, Qt::red);
        setQuickItemColor(highlightElement, Qt::red);
    }
}

void VerovioPageDemo::setQsgNodeColor(QString elementId, QColor color)
{
    auto nodes = m_qtDeviceContext->GetGeometryNodesForId(elementId);
    for (auto node : nodes) {
        QSGFlatColorMaterial *material = dynamic_cast<QSGFlatColorMaterial *>(node->material());
        if (!material) {
            material = new QSGFlatColorMaterial;
            node->setMaterial(material);
        }
        if (material->color() != color) {
            material->setColor(color);
            node->markDirty(QSGNode::DirtyMaterial);
        }
    }
}

void VerovioPageDemo::setQuickItemColor(QString elementId, QColor color)
{
    auto quickItems = m_qtDeviceContext->GetQuickItemsForId(elementId);
    for (auto quickItem : quickItems) {
        quickItem->setColor(color);
    }
}
