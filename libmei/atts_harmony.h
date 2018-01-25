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

#ifndef __VRV_ATTS_HARMONY_H__
#define __VRV_ATTS_HARMONY_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttChordDefLog
//----------------------------------------------------------------------------

class AttChordDefLog : public Att {
public:
    AttChordDefLog();
    virtual ~AttChordDefLog();

    /** Reset the default values for the attribute class **/
    void ResetChordDefLog();

    /** Read the values for the attribute class **/
    bool ReadChordDefLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteChordDefLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPos(int pos_) { m_pos = pos_; }
    int GetPos() const { return m_pos; }
    bool HasPos() const;
    ///@}

private:
    /** Records the fret position at which the chord tablature is to be played. **/
    int m_pos;

    /* include <attpos> */
};

//----------------------------------------------------------------------------
// AttChordMemberLog
//----------------------------------------------------------------------------

class AttChordMemberLog : public Att {
public:
    AttChordMemberLog();
    virtual ~AttChordMemberLog();

    /** Reset the default values for the attribute class **/
    void ResetChordMemberLog();

    /** Read the values for the attribute class **/
    bool ReadChordMemberLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteChordMemberLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetString(int string_) { m_string = string_; }
    int GetString() const { return m_string; }
    bool HasString() const;
    //
    void SetFret(int fret_) { m_fret = fret_; }
    int GetFret() const { return m_fret; }
    bool HasFret() const;
    //
    void SetFing(data_FINGER_FRET fing_) { m_fing = fing_; }
    data_FINGER_FRET GetFing() const { return m_fing; }
    bool HasFing() const;
    ///@}

private:
    /** Holds the number of the string to be played. **/
    int m_string;
    /** Records the location at which a string should be stopped against a fret. **/
    int m_fret;
    /**
     * Indicates which finger, if any, should be used to play an individual string.
     * The index, middle, ring, and little fingers are represented by the values 1-4,
     * while 't' is for the thumb. The values 'x' and 'o' indicate muffled and open
     * strings, respectively.
     **/
    data_FINGER_FRET m_fing;

    /* include <attfing> */
};

//----------------------------------------------------------------------------
// AttHarmLog
//----------------------------------------------------------------------------

class AttHarmLog : public Att {
public:
    AttHarmLog();
    virtual ~AttHarmLog();

    /** Reset the default values for the attribute class **/
    void ResetHarmLog();

    /** Read the values for the attribute class **/
    bool ReadHarmLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHarmLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetChordref(std::string chordref_) { m_chordref = chordref_; }
    std::string GetChordref() const { return m_chordref; }
    bool HasChordref() const;
    ///@}

private:
    /** Contains a reference to a **/
    std::string m_chordref;

    /* include <attchordref> */
};

} // vrv namespace

#endif // __VRV_ATTS_HARMONY_H__
