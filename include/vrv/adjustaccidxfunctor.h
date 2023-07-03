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
    /*
     * Get the accidentals for adjustment
     */
    std::vector<Accid *> GetAccidentalsForAdjustment(AlignmentReference *alignmentReference) const;

    /**
     * Sets whether the accidental should be aligned with all elements of the alignmentReference
     * or elements from the same layer only.
     */
    void SetAccidLayerAlignment(Accid *accid, const AlignmentReference *alignmentReference) const;

    /**
     * Adjust an accidental horizontally
     */
    void AdjustAccidWithSpace(Accid *accid, AlignmentReference *alignmentReference, int staffSize);

public:
    //
private:
    // The current measure
    Measure *m_currentMeasure;

    // The accidentals that were already adjusted
    std::set<Accid *> m_adjustedAccids;
};

} // namespace vrv

#endif // __VRV_ADJUSTACCIDXFUNCTOR_H__
