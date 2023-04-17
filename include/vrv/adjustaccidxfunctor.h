/////////////////////////////////////////////////////////////////////////////
// Name:        adjustaccidxfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTACCIDXFUNCTOR_H__
#define __VRV_ADJUSTACCIDXFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustAccidXFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the X position of accidentals.
 */
class AdjustAccidXFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustAccidXFunctor(Doc *doc);
    virtual ~AdjustAccidXFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAlignment(Alignment *alignment) override;
    FunctorCode VisitAlignmentReference(AlignmentReference *alignmentReference) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current measure
    Measure *m_currentMeasure;
};

} // namespace vrv

#endif // __VRV_ADJUSTACCIDXFUNCTOR_H__
