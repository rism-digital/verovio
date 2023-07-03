/////////////////////////////////////////////////////////////////////////////
// Name:        calcbboxoverflowsfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCBBOXOVERFLOWSFUNCTOR_H__
#define __VRV_CALCBBOXOVERFLOWSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcBBoxOverflowsFunctor
//----------------------------------------------------------------------------

/**
 * This class fills the arrays of bounding boxes (above and below) for each staff alignment
 * for which the box overflows.
 */
class CalcBBoxOverflowsFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcBBoxOverflowsFunctor(Doc *doc);
    virtual ~CalcBBoxOverflowsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerEnd(Layer *layer) override;
    FunctorCode VisitObject(Object *object) override;
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

#endif // __VRV_CALCBBOXOVERFLOWSFUNCTOR_H__
