/////////////////////////////////////////////////////////////////////////////
// Name:        calcslurdirectionfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCSLURDIRECTIONFUNCTOR_H__
#define __VRV_CALCSLURDIRECTIONFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcSlurDirectionFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the slur direction.
 */
class CalcSlurDirectionFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcSlurDirectionFunctor(Doc *doc);
    virtual ~CalcSlurDirectionFunctor() = default;
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
    FunctorCode VisitSlur(Slur *slur) override;
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

#endif // __VRV_CALCSLURDIRECTIONFUNCTOR_H__
