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

#ifndef __VRV_ATTS_STRINGTAB_H__
#define __VRV_ATTS_STRINGTAB_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttStringtab
//----------------------------------------------------------------------------

class AttStringtab : public Att {
public:
    AttStringtab();
    virtual ~AttStringtab();

    /** Reset the default values for the attribute class **/
    void ResetStringtab();

    /** Read the values for the attribute class **/
    bool ReadStringtab(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStringtab(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTabFing(data_FINGER_FRET tabFing_) { m_tabFing = tabFing_; }
    data_FINGER_FRET GetTabFing() const { return m_tabFing; }
    bool HasTabFing() const;
    //
    void SetTabFret(data_FRETNUMBER tabFret_) { m_tabFret = tabFret_; }
    data_FRETNUMBER GetTabFret() const { return m_tabFret; }
    bool HasTabFret() const;
    //
    void SetTabString(data_STRINGNUMBER tabString_) { m_tabString = tabString_; }
    data_STRINGNUMBER GetTabString() const { return m_tabString; }
    bool HasTabString() const;
    ///@}

private:
    /**
     * Indicates which finger, if any, should be used to play an individual string.
     * The index, middle, ring, and little fingers are represented by the values 1-4,
     * while 't' is for the thumb. The values 'x' and 'o' indicate muffled and open
     * strings, respectively.
     **/
    data_FINGER_FRET m_tabFing;
    /** Records the location at which a string should be stopped against a fret. **/
    data_FRETNUMBER m_tabFret;
    /** Records which string is to be played. **/
    data_STRINGNUMBER m_tabString;

    /* include <atttab.string> */
};

//----------------------------------------------------------------------------
// AttStringtabPosition
//----------------------------------------------------------------------------

class AttStringtabPosition : public Att {
public:
    AttStringtabPosition();
    virtual ~AttStringtabPosition();

    /** Reset the default values for the attribute class **/
    void ResetStringtabPosition();

    /** Read the values for the attribute class **/
    bool ReadStringtabPosition(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStringtabPosition(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTabPos(int tabPos_) { m_tabPos = tabPos_; }
    int GetTabPos() const { return m_tabPos; }
    bool HasTabPos() const;
    ///@}

private:
    /** Records fret position. **/
    int m_tabPos;

    /* include <atttab.pos> */
};

//----------------------------------------------------------------------------
// AttStringtabTuning
//----------------------------------------------------------------------------

class AttStringtabTuning : public Att {
public:
    AttStringtabTuning();
    virtual ~AttStringtabTuning();

    /** Reset the default values for the attribute class **/
    void ResetStringtabTuning();

    /** Read the values for the attribute class **/
    bool ReadStringtabTuning(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStringtabTuning(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTabStrings(std::string tabStrings_) { m_tabStrings = tabStrings_; }
    std::string GetTabStrings() const { return m_tabStrings; }
    bool HasTabStrings() const;
    ///@}

private:
    /** Provides a *written* pitch and octave for each open string or course of strings. **/
    std::string m_tabStrings;

    /* include <atttab.strings> */
};

} // vrv namespace

#endif // __VRV_ATTS_STRINGTAB_H__
