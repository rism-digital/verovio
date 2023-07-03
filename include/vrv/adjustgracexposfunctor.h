/////////////////////////////////////////////////////////////////////////////
// Name:        adjustgracexposfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTGRACEXPOSFUNCTOR_H__
#define __VRV_ADJUSTGRACEXPOSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustGraceXPosFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the X positions of the grace notes looking at the bounding boxes.
 * The functor is redirected from the MeasureAligner and then from the appropriate
 * alignment to the GraceAligner
 */
class AdjustGraceXPosFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustGraceXPosFunctor(Doc *doc, const std::vector<int> &staffNs);
    virtual ~AdjustGraceXPosFunctor() = default;
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
    FunctorCode VisitAlignmentEnd(Alignment *alignment) override;
    FunctorCode VisitAlignmentReference(AlignmentReference *alignmentReference) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitScore(Score *score) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The list of staffN in the top-level scoreDef
    std::vector<int> m_staffNs;
    // The maximum position
    int m_graceMaxPos;
    // The upcoming maximum position (i.e., the min pos for the next element)
    int m_graceUpcomingMaxPos;
    // The cumulated shift on the previous aligners
    int m_graceCumulatedXShift;
    // Indicates whether the alignment is in a Measure or in a Grace
    bool m_isGraceAlignment;
    // The list of tie endpoints for the current measure
    MeasureTieEndpoints m_measureTieEndpoints;
    // The right ALIGNMENT_DEFAULT (if any)
    Alignment *m_rightDefaultAlignment;
};

} // namespace vrv

#endif // __VRV_ADJUSTGRACEXPOSFUNCTOR_H__
