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

class LyricElement;

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
    AdjustSylSpacingFunctor(Doc *doc, int voltaTrack = 0);
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
    FunctorCode VisitLyricElement(LyricElement *lyricElement) override;
    FunctorCode VisitRefrain(Refrain *refrain) override;
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
    // The previous verse or volta lyric container
    LayerElement *m_previousContainer;
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
    // 0 for direct verse content, otherwise the normalized volta alternative.
    int m_voltaTrack;
};

} // namespace vrv

#endif // __VRV_ADJUSTSYLSPACINGFUNCTOR_H__
