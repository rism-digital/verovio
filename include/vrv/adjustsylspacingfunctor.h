/////////////////////////////////////////////////////////////////////////////
// Name:        adjustsylspacingfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTSYLSPACINGFUNCTOR_H__
#define __VRV_ADJUSTSYLSPACINGFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustSylSpacingFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the spacing of the syl processing verse by verse.
 */
class AdjustSylSpacingFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustSylSpacingFunctor(Doc *doc);
    virtual ~AdjustSylSpacingFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitSystemEnd(System *system) override;
    FunctorCode VisitVerse(Verse *verse) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // List of adjustment tuples (Alignment start|Alignment end|distance)
    ArrayOfAdjustmentTuples m_overlappingSyl;
    // The previous verse
    Verse *m_previousVerse;
    // The previous syl
    Syl *m_lastSyl;
    // The previous measure
    Measure *m_previousMeasure;
    // The current LabelAbbr
    LabelAbbr *m_currentLabelAbbr;
    // Amount of free space
    int m_freeSpace;
    // The staff size
    int m_staffSize;
};

} // namespace vrv

#endif // __VRV_ADJUSTSYLSPACINGFUNCTOR_H__
