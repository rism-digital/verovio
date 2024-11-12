/////////////////////////////////////////////////////////////////////////////
// Name:        facsimilefunctor.h
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FACSIMILEFUNCTOR_H__
#define __VRV_FACSIMILEFUNCTOR_H__

#include "functor.h"
#include "view.h"

namespace vrv {

class LayerElement;
class Measure;
class Page;
class Pb;
class Sb;
class Staff;
class Surface;
class System;

//----------------------------------------------------------------------------
// SyncFromFacsimileFunctor
//----------------------------------------------------------------------------

/**
 * This class sync the layout encoded in the facsimile to m_Abs members
 */
class SyncFromFacsimileFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    SyncFromFacsimileFunctor(Doc *doc);
    virtual ~SyncFromFacsimileFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitPage(Page *page) override;
    FunctorCode VisitPageEnd(Page *page) override;
    FunctorCode VisitPb(Pb *pb) override;
    FunctorCode VisitSb(Sb *sb) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    /** The doc */
    Doc *m_doc;
    //
    View m_view;
    //
    Page *m_currentPage;
    System *m_currentSystem;
    Measure *m_currentNeumeLine;
    /** map to store the zone corresponding to a staff */
    std::map<Staff *, Zone *> m_staffZones;
    //
    int m_pageMarginTop;
    int m_pageMarginLeft;
    //
    double m_ppuFactor;
};

//----------------------------------------------------------------------------
// SyncToFacsimileFunctor
//----------------------------------------------------------------------------

/**
 * This class sync the layout calculated to the facsimile
 */
class SyncToFacsimileFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    SyncToFacsimileFunctor(Doc *doc, double ppuFactor);
    virtual ~SyncToFacsimileFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitPage(Page *page) override;
    FunctorCode VisitPageEnd(Page *page) override;
    FunctorCode VisitPb(Pb *pb) override;
    FunctorCode VisitSb(Sb *sb) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    /** Create zone if not exist */
    Zone *GetZone(FacsimileInterface *interface, std::string type);

public:
    //
private:
    /** The doc */
    Doc *m_doc;
    //
    View m_view;
    /** The surface we are going to add / update zone  */
    Surface *m_surface;
    //
    Page *m_currentPage;
    System *m_currentSystem;
    //
    int m_pageMarginTop;
    int m_pageMarginLeft;
    // A flag indicating we are dealing with a neume line
    bool m_currentNeumeLine;
    //
    double m_ppuFactor;
};

} // namespace vrv

#endif // __VRV_FACSIMILEFUNCTOR_H__
