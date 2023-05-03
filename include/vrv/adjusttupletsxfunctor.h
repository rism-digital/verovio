/////////////////////////////////////////////////////////////////////////////
// Name:        adjusttupletsxfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTTUPLETSXFUNCTOR_H__
#define __VRV_ADJUSTTUPLETSXFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustTupletsXFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the X position of tuplet brackets and num.
 */
class AdjustTupletsXFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustTupletsXFunctor(Doc *doc);
    virtual ~AdjustTupletsXFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitTuplet(Tuplet *tuplet) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_ADJUSTTUPLETSXFUNCTOR_H__
