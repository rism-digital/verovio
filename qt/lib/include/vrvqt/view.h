/////////////////////////////////////////////////////////////////////////////
// Name:        view.h
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_QT_VIEW_H__
#define __VRV_QT_VIEW_H__

//----------------------------------------------------------------------------

#include <QQuickItem>
#include <memory>

//----------------------------------------------------------------------------

#include "scenegraphdevicecontext.h"

namespace vrvQt {
class Toolkit;

//----------------------------------------------------------------------------
// VerovioPage
//----------------------------------------------------------------------------

/**
 * This class represents a view of a Verovio document, it corresponds to vrv::View.
 */
class View : public QQuickItem {
    Q_OBJECT

    // Note: The namespace is required when specifying Qt properties
    Q_PROPERTY(vrvQt::Toolkit *verovioDoc MEMBER m_verovioDoc WRITE setVerovioDoc)
    Q_PROPERTY(int pageNumber MEMBER m_pageNumber WRITE setPageNumber)

public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    View();
    virtual ~View();
    ///@}

public slots:

    /**
     * @name Setters for public properties.
     */
    ///@{
    void setVerovioDoc(Toolkit *verovioDoc);
    void setPageNumber(int pageNumber);
    ///@}

signals:
    /**
     * @name This signal is emitted when the attached verovioDoc changes.
     */
    void verovioDocChanged(Toolkit *verovicoDoc);

protected:
    /**
     * @name Reimplemented function from QQuickItem.
     */
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

    // The device context for Qt
    std::unique_ptr<SceneGraphDeviceContext> m_qtDeviceContext{ nullptr };

private:
    // variables to store the public properties
    Toolkit *m_verovioDoc{ nullptr };
    int m_pageNumber{ 0 };

    // stores if the rendering has to be redone
    bool m_verovioRenderingDirty{ true };

    // handle for registered documentChanged connection.
    QMetaObject::Connection m_documentLayoutChangedConnection;
};
} // namespace vrvQt

#endif // __VRV_QT_VIEW_H__
