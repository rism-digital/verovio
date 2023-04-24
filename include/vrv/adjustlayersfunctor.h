/////////////////////////////////////////////////////////////////////////////
// Name:        adjustlayersfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTLAYERSFUNCTOR_H__
#define __VRV_ADJUSTLAYERSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustLayersFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of notes and chords for multiple layers.
 */
class AdjustLayersFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustLayersFunctor(Doc *doc, const std::vector<int> &staffNs);
    virtual ~AdjustLayersFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Set the ignore dots flag
     */
    void IgnoreDots(bool ignoreDots) { m_ignoreDots = ignoreDots; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAlignmentReference(AlignmentReference *alignmentReference) override;
    FunctorCode VisitAlignmentReferenceEnd(AlignmentReference *alignmentReference) override;
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
    // The current layerN set in the AlignmentRef (negative values for cross-staff)
    int m_currentLayerN;
    // The elements for the previous layer(s)
    std::vector<LayerElement *> m_previous;
    // The elements of the current layer
    std::vector<LayerElement *> m_current;
    // Indicates whether the element is in unison
    bool m_unison;
    // Indicates whether dots should be ignored
    bool m_ignoreDots;
    // Indicates whether the element (note) has a stem.sameas note
    bool m_stemSameas;
    // The total shift of the current note or chord
    int m_accumulatedShift;
};

} // namespace vrv

#endif // __VRV_ADJUSTLAYERSFUNCTOR_H__
