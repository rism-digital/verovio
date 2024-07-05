/////////////////////////////////////////////////////////////////////////////
// Name:        adjustxposfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTXPOSFUNCTOR_H__
#define __VRV_ADJUSTXPOSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustXPosAlignmentOffset
//----------------------------------------------------------------------------

/**
 * This struct holds information about alignment, possible offset and overlapping bounding box. This should
 * be used solely with AdjustXPosFunctor to store information regarding current/previous alignment.
 */
struct AdjustXPosAlignmentOffset {
    AdjustXPosAlignmentOffset() { this->Reset(); }

    void Reset()
    {
        m_alignment = NULL;
        m_offset = 0;
        m_overlappingBB = NULL;
    }

    // data members
    Alignment *m_alignment;
    int m_offset;
    BoundingBox *m_overlappingBB;
};

//----------------------------------------------------------------------------
// AdjustXPosFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the X positions of the staff content by looking at the bounding boxes.
 * The functor processes by aligned-staff content, that is from a redirection in the
 * MeasureAligner and then staff by staff but taking into account cross-staff elements.
 */
class AdjustXPosFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustXPosFunctor(Doc *doc, const std::vector<int> &staffNs);
    virtual ~AdjustXPosFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Set the included and excluded classes
     */
    ///@{
    void SetIncluded(const std::vector<ClassId> &classIDs) { m_includes = classIDs; }
    void ClearIncluded() { m_includes.clear(); }
    void SetExcluded(const std::vector<ClassId> &classIDs) { m_excludes = classIDs; }
    void ClearExcluded() { m_excludes.clear(); }
    ///@}

    /*
     * Set the right bar lines only flag
     */
    void SetRightBarLinesOnly(bool rightBarLinesOnly) { m_rightBarLinesOnly = rightBarLinesOnly; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAlignment(Alignment *alignment) override;
    FunctorCode VisitAlignmentEnd(Alignment *alignment) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitScore(Score *score) override;
    ///@}

protected:
    //
private:
    /**
     * Calculate offset and left overlap of the element
     */
    std::pair<int, int> CalculateXPosOffset(LayerElement *layerElement);

public:
    //
private:
    // The minimum position (i.e., the width of the previous element)
    int m_minPos;
    // The upcoming minimum position (i.e., the min pos for the next element)
    int m_upcomingMinPos;
    // The cumulated shift on the previous aligners
    int m_cumulatedXShift;
    // The current staff @n (used for grace note alignment)
    int m_staffN;
    // The current staff size
    int m_staffSize;
    // The current staff is neume
    bool m_isNeumeStaff;
    // The list of staffN in the top-level scoreDef
    std::vector<int> m_staffNs;
    // The bounding boxes in the previous aligner
    std::vector<BoundingBox *> m_boundingBoxes;
    // The upcoming bounding boxes (to be used in the next aligner)
    std::vector<BoundingBox *> m_upcomingBoundingBoxes;
    // The list of types to include
    std::vector<ClassId> m_includes;
    // The list of types to exclude
    std::vector<ClassId> m_excludes;
    // Indicates whether only right bar line positions should be considered
    bool m_rightBarLinesOnly;
    // The list of tie endpoints for the current measure
    MeasureTieEndpoints m_measureTieEndpoints;
    // The current alignment
    AdjustXPosAlignmentOffset m_currentAlignment;
    // The preceeding alignment
    AdjustXPosAlignmentOffset m_previousAlignment;
    // The current measure
    Measure *m_measure;
};

} // namespace vrv

#endif // __VRV_ADJUSTXPOSFUNCTOR_H__
