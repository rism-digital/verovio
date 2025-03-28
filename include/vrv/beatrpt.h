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
    std::string GetClassName() const override { return "beatRpt"; }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Returns the duration (in Fraction) for the BeatRpt.
     */

    Fraction GetBeatRptAlignmentDuration(data_DURATION meterUnit) const;

    /**
     * MIDI timing information
     */
    ///@{
    void SetScoreTimeOnset(Fraction scoreTime);
    Fraction GetScoreTimeOnset() const;

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
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
    Fraction m_scoreTimeOnset;
};

} // namespace vrv

#endif
