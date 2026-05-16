/////////////////////////////////////////////////////////////////////////////
// Name:        adjustbeamsfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTBEAMSFUNCTOR_H__
#define __VRV_ADJUSTBEAMSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustBeamsFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the vertical position adjustment for beams if they overlap with layer elements.
 */
class AdjustBeamsFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustBeamsFunctor(Doc *doc);
    virtual ~AdjustBeamsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitBeam(Beam *beam) override;
    FunctorCode VisitBeamEnd(Beam *beam) override;
    FunctorCode VisitClef(Clef *clef) override;
    FunctorCode VisitFTrem(FTrem *fTrem) override;
    FunctorCode VisitFTremEnd(FTrem *fTrem) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitRest(Rest *rest) override;
    ///@}

protected:
    //
private:
    // Get the drawing interface of the outer beam or the outer ftrem
    BeamDrawingInterface *GetOuterBeamInterface() const;

    /**
     * Calculate the overlap with other layer elements that
     * are placed within the duration of the element
     */
    int CalcLayerOverlap(const LayerElement *beamElement) const;

    // Rounds the overlap to the closest multiple of a half unit
    int AdjustOverlapToHalfUnit(int overlap, int unit) const;

public:
    //
private:
    // The top-level beam or ftrem that should be adjusted
    Beam *m_outerBeam;
    FTrem *m_outerFTrem;
    // The y coordinates of the beam left and right side
    int m_y1;
    int m_y2;
    // The x coordinates of the beam left and right side
    int m_x1;
    int m_x2;
    // The slope of the beam
    double m_beamSlope;
    // The direction bias
    int m_directionBias;
    // The overlap margin that the beam needs to be displaced by
    int m_overlapMargin;
    // Indicates whether an element from a different layer is processed
    bool m_isOtherLayer;
};

} // namespace vrv

#endif // __VRV_ADJUSTBEAMSFUNCTOR_H__
