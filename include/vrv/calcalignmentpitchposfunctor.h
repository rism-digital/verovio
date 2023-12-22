/////////////////////////////////////////////////////////////////////////////
// Name:        calcalignmentpitchposfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCALIGNMENTPITCHPOSFUNCTOR_H__
#define __VRV_CALCALIGNMENTPITCHPOSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcAlignmentPitchPosFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the drawing position (m_drawingX and m_drawingY) values
 * for objects.
 */
class CalcAlignmentPitchPosFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcAlignmentPitchPosFunctor(Doc *doc);
    virtual ~CalcAlignmentPitchPosFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitStaffDef(StaffDef *staffDef) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current scoreDef default octave
    data_OCTAVE m_octDefault;
    // The current staffDef default octaves
    std::map<int, data_OCTAVE> m_octDefaultForStaffN;
};

} // namespace vrv

#endif // __VRV_CALCALIGNMENTPITCHPOSFUNCTOR_H__
