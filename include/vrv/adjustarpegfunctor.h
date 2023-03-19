/////////////////////////////////////////////////////////////////////////////
// Name:        adjustarpegfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTARPEGFUNCTOR_H__
#define __VRV_ADJUSTARPEGFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustArpegFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the Arpeg position.
 */
class AdjustArpegFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustArpegFunctor(Doc *doc);
    virtual ~AdjustArpegFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAlignment(Alignment *alignment) override;
    FunctorCode VisitArpeg(Arpeg *arpeg) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The array of Alignment|Arpeg|StaffN|bool tuples
    ArrayOfAlignmentArpegTuples m_alignmentArpegTuples;
    // The current MeasureAligner
    MeasureAligner *m_measureAligner;
};

} // namespace vrv

#endif // __VRV_ADJUSTARPEGFUNCTOR_H__
