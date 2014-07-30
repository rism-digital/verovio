/////////////////////////////////////////////////////////////////////////////
// Authors:     Laurent Pugin and Rodolfo Zitellini
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
//
// Code generated using a modified version of libmei 
// by Andrew Hankinson, Alastair Porter, and Others
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////// 
// NOTE: this file was generated with the Verovio libmei version and 
// should not be edited because changes will be lost.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ATTS_LYRICS_H__
#define __VRV_ATTS_LYRICS_H__

#include "vrvdef.h"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttVerseLog
//----------------------------------------------------------------------------

class AttVerseLog 
{
public:
    AttVerseLog();
    virtual ~AttVerseLog();
    
    /** Reset the default values for the attribute class **/
    void ResetVerseLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetRefrain(std::string refrain_) { m_refrain = refrain_; };
    std::string GetRefrain() { return m_refrain; };
    //
    void SetRhythm(std::string rhythm_) { m_rhythm = rhythm_; };
    std::string GetRhythm() { return m_rhythm; };
    ///@}

protected:
    /**
     * Used to indicate a common, usually centered, refrain (Mup User's Guide, p.
     * 44).
     **/
    std::string m_refrain;
    /**
     * Used to specify a rhythm for the lyric syllables that differs from that of the
     * notes on the staff, e.g.
     * '4,4,4,4' when the rhythm of the notes is '4.,8,4.,8'.
     **/
    std::string m_rhythm;

/* include <attrhythm> */
};

} // vrv namespace

#endif  // __VRV_ATTS_LYRICS_H__

