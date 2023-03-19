/////////////////////////////////////////////////////////////////////////////
// Name:        calcspanningbeamspansfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCSPANNINGBEAMSPANSFUNCTOR_H__
#define __VRV_CALCSPANNINGBEAMSPANSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcSpanningBeamSpansFunctor
//----------------------------------------------------------------------------

/**
 * This class resolves spanning beamspans by breaking it into separate parts, each belonging to the
 * corresponding system/measure. BeamSpans get elements reassigned, so that each beamSpan can be drawn
 * as control element. This allows free placement of beamSpan in the MEI tree and ensures that beamSpan
 * will be drawn properly.
 */
class CalcSpanningBeamSpansFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcSpanningBeamSpansFunctor(Doc *doc);
    virtual ~CalcSpanningBeamSpansFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitBeamSpan(BeamSpan *beamSpan) override;
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

#endif // __VRV_CALCSPANNINGBEAMSPANSFUNCTOR_H__
