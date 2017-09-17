/////////////////////////////////////////////////////////////////////////////
// Name:        veroviopage.h
// Author:      Jonathan Schluessler
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VEROVIOPAGE_H__
#define __VEROVIOPAGE_H__

//----------------------------------------------------------------------------

#include <QQuickItem>
#include <memory>

//----------------------------------------------------------------------------

#include "qtscenegraphdevicecontext.h"

namespace vrv {
class VerovioDoc;

//----------------------------------------------------------------------------
// VerovioPage
//----------------------------------------------------------------------------

/**
 * This class represents one page of a Verovio document.
 */
class VerovioPage : public QQuickItem {
    Q_OBJECT

    // Note: The namespace is required when specifying Qt properties
    Q_PROPERTY(vrv::VerovioDoc *verovioDoc MEMBER m_verovioDoc WRITE setVerovioDoc)
    Q_PROPERTY(int pageNumber MEMBER m_pageNumber WRITE setPageNumber)

public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    VerovioPage();
    virtual ~VerovioPage();
    ///@}

public slots:

    /**
     * @name Setters for public properties.
     */
    ///@{
    void setVerovioDoc(VerovioDoc *verovioDoc);
    void setPageNumber(int pageNumber);
    ///@}

protected:
    /**
     * @name Reimplemented function from QQuickItem.
     */
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

    // The device context for Qt
    std::unique_ptr<QtSceneGraphDeviceContext> m_qtDeviceContext{ nullptr };

private:
    // variables to store the public properties
    VerovioDoc *m_verovioDoc{ nullptr };
    int m_pageNumber{ 0 };

    // stores if the rendering has to be redone
    bool m_verovioRenderingDirty{ true };

    // handle for registered documentChanged connection.
    QMetaObject::Connection m_documentLayoutChangedConnection;
};
} // namespace vrv

#endif // __VEROVIOPAGE_H__
