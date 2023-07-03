/////////////////////////////////////////////////////////////////////////////
// Name:        calcslurdirectionfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCSLURDIRECTIONFUNCTOR_H__
#define __VRV_CALCSLURDIRECTIONFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcSlurDirectionFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the slur direction.
 */
class CalcSlurDirectionFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcSlurDirectionFunctor(Doc *doc);
    virtual ~CalcSlurDirectionFunctor() = default;
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
    FunctorCode VisitSlur(Slur *slur) override;
    ///@}

protected:
    //
private:
    // Determine curve direction for the slurs that start at grace note
    curvature_CURVEDIR GetGraceCurveDirection(const Slur *slur) const;
    // Get preferred curve direction based on various conditions
    curvature_CURVEDIR GetPreferredCurveDirection(
        const Slur *slur, data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter, bool isGraceToNoteSlur) const;

public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_CALCSLURDIRECTIONFUNCTOR_H__
