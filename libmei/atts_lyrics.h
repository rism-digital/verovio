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

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttVerseLog
//----------------------------------------------------------------------------

class AttVerseLog : public Att {
public:
    AttVerseLog();
    virtual ~AttVerseLog();

    /** Reset the default values for the attribute class **/
    void ResetVerseLog();

    /** Read the values for the attribute class **/
    bool ReadVerseLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteVerseLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetRefrain(data_BOOLEAN refrain_) { m_refrain = refrain_; };
    data_BOOLEAN GetRefrain() const { return m_refrain; };
    bool HasRefrain() const;
    //
    void SetRhythm(std::string rhythm_) { m_rhythm = rhythm_; };
    std::string GetRhythm() const { return m_rhythm; };
    bool HasRhythm() const;
    ///@}

private:
    /** Used to indicate a common, usually centered, refrain. **/
    data_BOOLEAN m_refrain;
    /**
     * Used to specify a rhythm for the lyric syllables that differs from that of the
     * notes on the staff, e.g.
     * '4,4,4,4' when the rhythm of the notes is '4.,8,4.,8'.
     **/
    std::string m_rhythm;

    /* include <attrhythm> */
};

} // vrv namespace

#endif // __VRV_ATTS_LYRICS_H__
