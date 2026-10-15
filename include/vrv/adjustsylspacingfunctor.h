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
 * A lyric line covers all layers of a staff, plus the verses above the next staff, processed from left to right.
 */
class AdjustSylSpacingFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustSylSpacingFunctor(Doc *doc, int voltaTrack, const std::pair<int, data_STAFFREL> &lyricLine,
        const std::map<int, int> &previousStaffNs);
    virtual ~AdjustSylSpacingFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /**
     * Return the lyric line (staff \@n and placement) of verses with the given placement.
     * Verses above a staff are on the line below the previous staff, even if the vertical layout does not share it.
     */
    static std::pair<int, data_STAFFREL> GetLyricLine(
        int staffN, data_STAFFREL place, const std::map<int, int> &previousStaffNs);

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitMeasureEnd(Measure *measure) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitSystemEnd(System *system) override;
    FunctorCode VisitLyricElement(LyricElement *lyricElement) override;
    FunctorCode VisitRefrain(Refrain *refrain) override;
    FunctorCode VisitVerse(Verse *verse) override;
    ///@}

protected:
    //
private:
    /**
     * Adjust the spacing between the previous lyric element and the current one
     */
    void AdjustLyricElement(LyricElement *lyricElement);

    /**
     * Move the previous lyric container back into the free space, together with the ones at the same position
     */
    int AdjustPreviousLyricPosition(int &overlap);

public:
    //
private:
    // List of adjustment tuples (Alignment start|Alignment end|distance)
    ArrayOfAdjustmentTuples m_overlappingSyl;
    // The previous verse or volta lyric container
    LayerElement *m_previousContainer;
    // The lyric containers in other staves or layers at the same position as the previous one
    std::vector<LayerElement *> m_simultaneousContainers;
    // The previous syl
    Syl *m_lastSyl;
    // The previous measure
    Measure *m_previousMeasure;
    // The current LabelAbbr for each staff and layer @n
    std::map<std::pair<int, int>, LabelAbbr *> m_currentLabelAbbrs;
    // The staff and layer @n with a syllable already in the current system
    std::set<std::pair<int, int>> m_systemLyricSources;
    // Amount of free space
    int m_freeSpace;
    // 0 for direct verse content, otherwise the normalized volta alternative.
    int m_voltaTrack;
    // The staff @n and the placement of the lyric line being processed
    std::pair<int, data_STAFFREL> m_lyricLine;
    // The previous staff @n of each staff @n in drawing order
    const std::map<int, int> &m_previousStaffNs;
    // The lyric elements of the current measure
    std::vector<LyricElement *> m_measureLyricElements;
};

} // namespace vrv

#endif // __VRV_ADJUSTSYLSPACINGFUNCTOR_H__
