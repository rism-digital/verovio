/////////////////////////////////////////////////////////////////////////////
// Name:        adjustdotsfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTDOTSFUNCTOR_H__
#define __VRV_ADJUSTDOTSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustDotsFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of the dots for multiple layers.
 */
class AdjustDotsFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustDotsFunctor(Doc *doc, const std::vector<int> &staffNs);
    virtual ~AdjustDotsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAlignmentEnd(Alignment *alignment) override;
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
    // The list of all elements (except for dots) for the alignment in the staff
    std::vector<LayerElement *> m_elements;
    // The list of all dots for the alignment in the staff
    std::vector<Dots *> m_dots;
};

} // namespace vrv

#endif // __VRV_ADJUSTDOTSFUNCTOR_H__
