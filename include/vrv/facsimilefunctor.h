/////////////////////////////////////////////////////////////////////////////
// Name:        facsimilefunctor.h
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FACSIMILEFUNCTOR_H__
#define __VRV_FACSIMILEFUNCTOR_H__

#include "functor.h"

namespace vrv {

class LayerElement;
class Measure;
class Page;
class Pb;
class Sb;
class Staff;
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
    SyncFromFacsimileFunctor();
    virtual ~SyncFromFacsimileFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitPage(Page *page) override;
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
    //
    Page *m_currentPage;
    System *m_currentSystem;
};

} // namespace vrv

#endif // __VRV_FACSIMILEFUNCTOR_H__
