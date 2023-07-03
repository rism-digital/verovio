/////////////////////////////////////////////////////////////////////////////
// Name:        adjustharmgrpsspacingfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTHARMGRPSSPACINGFUNCTOR_H__
#define __VRV_ADJUSTHARMGRPSSPACINGFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustHarmGrpsSpacingFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the horizontal position of harms by groups in order to avoid overlapping.
 */
class AdjustHarmGrpsSpacingFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustHarmGrpsSpacingFunctor(Doc *doc);
    virtual ~AdjustHarmGrpsSpacingFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitHarm(Harm *harm) override;
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitSystemEnd(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The grpIds of harms in the system
    std::vector<int> m_grpIds;
    // The current grp id (0 for the first pass)
    int m_currentGrp;
    // The adjustment tuples (Alignment start|Alignment end|distance)
    ArrayOfAdjustmentTuples m_overlappingHarm;
    // The previous harm positioner (if any)
    FloatingPositioner *m_previousHarmPositioner;
    // The previous harm start (if any)
    LayerElement *m_previousHarmStart;
    // The previous measure (if any)
    Measure *m_previousMeasure;
    // The current system
    System *m_currentSystem;
};

} // namespace vrv

#endif // __VRV_ADJUSTHARMGRPSSPACINGFUNCTOR_H__
