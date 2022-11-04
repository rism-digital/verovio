/////////////////////////////////////////////////////////////////////////////
// Name:        beatrpt.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BEATRPT_H__
#define __VRV_BEATRPT_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeatRpt (beat repeat)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <beatRpt> element.
 */
class BeatRpt : public LayerElement, public AttColor, public AttBeatRptLog, public AttBeatRptVis {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    BeatRpt();
    virtual ~BeatRpt();
    Object *Clone() const override { return new BeatRpt(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "BeatRpt"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Returns the duration (in double) for the BeatRpt.
     */

    double GetBeatRptAlignmentDuration(int meterUnit) const;

    /**
     * MIDI timing information
     */
    ///@{
    void SetScoreTimeOnset(double scoreTime);
    double GetScoreTimeOnset() const;

    //----------//
    // Functors //
    //----------//

    /**
     * @name See Object::GenerateMIDI
     */
    ///@{
    int GenerateMIDI(FunctorParams *functorParams) override;
    ///@}

private:
    //
public:
    //
private:
    /**
     * The score-time onset of the note in the measure (duration from the start of measure in
     * quarter notes).
     */
    double m_scoreTimeOnset;
};

} // namespace vrv

#endif
