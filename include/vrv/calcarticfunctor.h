/////////////////////////////////////////////////////////////////////////////
// Name:        calcarticfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCARTICFUNCTOR_H__
#define __VRV_CALCARTICFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcArticFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the position of the outside articulations.
 */
class CalcArticFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcArticFunctor(Doc *doc);
    virtual ~CalcArticFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitArtic(Artic *artic) override;
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitNote(Note *note) override;
    ///@}

protected:
    //
private:
    // Calculate shift for the articulation based on its type and presence of other articulations
    int CalculateHorizontalShift(const Artic *artic, bool virtualStem) const;
    // Include the parent beam staff in the calculation of the above and below staff
    void IncludeBeamStaff(LayerElement *layerElement);

public:
    //
private:
    // The layer element parent
    LayerElement *m_parent;
    // The current stem direction
    data_STEMDIRECTION m_stemDir;
    // The above and below staff
    Staff *m_staffAbove;
    Staff *m_staffBelow;
    // The above and below layer
    Layer *m_layerAbove;
    Layer *m_layerBelow;
    // True if above/below is cross staff
    bool m_crossStaffAbove;
    bool m_crossStaffBelow;
};

} // namespace vrv

#endif // __VRV_CALCARTICFUNCTOR_H__
