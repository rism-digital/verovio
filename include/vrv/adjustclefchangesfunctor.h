/////////////////////////////////////////////////////////////////////////////
// Name:        adjustclefchangesfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTCLEFCHANGESFUNCTOR_H__
#define __VRV_ADJUSTCLEFCHANGESFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustClefChangesFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the spacing for clef changes.
 */
class AdjustClefChangesFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustClefChangesFunctor(Doc *doc);
    virtual ~AdjustClefChangesFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitClef(Clef *clef) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current MeasureAligner
    MeasureAligner *m_aligner;
};

} // namespace vrv

#endif // __VRV_ADJUSTCLEFCHANGESFUNCTOR_H__
