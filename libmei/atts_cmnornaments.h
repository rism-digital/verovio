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

#ifndef __VRV_ATTS_CMNORNAMENTS_H__
#define __VRV_ATTS_CMNORNAMENTS_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttMordentLog
//----------------------------------------------------------------------------

class AttMordentLog : public Att {
public:
    AttMordentLog();
    virtual ~AttMordentLog();

    /** Reset the default values for the attribute class **/
    void ResetMordentLog();

    /** Read the values for the attribute class **/
    bool ReadMordentLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMordentLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(mordentLog_FORM form_) { m_form = form_; }
    mordentLog_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    //
    void SetLong(data_BOOLEAN long_) { m_long = long_; }
    data_BOOLEAN GetLong() const { return m_long; }
    bool HasLong() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    mordentLog_FORM m_form;
    /**
     * When set to 'true', a double or long mordent, sometimes called a "pincé double",
     * consisting of 5 notes, is indicated.
     **/
    data_BOOLEAN m_long;

    /* include <attlong> */
};

//----------------------------------------------------------------------------
// AttOrnamPresent
//----------------------------------------------------------------------------

class AttOrnamPresent : public Att {
public:
    AttOrnamPresent();
    virtual ~AttOrnamPresent();

    /** Reset the default values for the attribute class **/
    void ResetOrnamPresent();

    /** Read the values for the attribute class **/
    bool ReadOrnamPresent(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOrnamPresent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOrnam(std::string ornam_) { m_ornam = ornam_; }
    std::string GetOrnam() const { return m_ornam; }
    bool HasOrnam() const;
    ///@}

private:
    /**
     * Indicates that this element has an attached ornament.
     * If visual information about the ornament is needed, then one of the elements
     * that represents an ornament (mordent, trill, or turn) should be employed.
     **/
    std::string m_ornam;

    /* include <attornam> */
};

//----------------------------------------------------------------------------
// AttOrnamentAccid
//----------------------------------------------------------------------------

class AttOrnamentAccid : public Att {
public:
    AttOrnamentAccid();
    virtual ~AttOrnamentAccid();

    /** Reset the default values for the attribute class **/
    void ResetOrnamentAccid();

    /** Read the values for the attribute class **/
    bool ReadOrnamentAccid(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteOrnamentAccid(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAccidupper(data_ACCIDENTAL_WRITTEN accidupper_) { m_accidupper = accidupper_; }
    data_ACCIDENTAL_WRITTEN GetAccidupper() const { return m_accidupper; }
    bool HasAccidupper() const;
    //
    void SetAccidlower(data_ACCIDENTAL_WRITTEN accidlower_) { m_accidlower = accidlower_; }
    data_ACCIDENTAL_WRITTEN GetAccidlower() const { return m_accidlower; }
    bool HasAccidlower() const;
    ///@}

private:
    /** Records the written accidental associated with an upper neighboring note. **/
    data_ACCIDENTAL_WRITTEN m_accidupper;
    /** Records the written accidental associated with a lower neighboring note. **/
    data_ACCIDENTAL_WRITTEN m_accidlower;

    /* include <attaccidlower> */
};

//----------------------------------------------------------------------------
// AttTurnLog
//----------------------------------------------------------------------------

class AttTurnLog : public Att {
public:
    AttTurnLog();
    virtual ~AttTurnLog();

    /** Reset the default values for the attribute class **/
    void ResetTurnLog();

    /** Read the values for the attribute class **/
    bool ReadTurnLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTurnLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDelayed(data_BOOLEAN delayed_) { m_delayed = delayed_; }
    data_BOOLEAN GetDelayed() const { return m_delayed; }
    bool HasDelayed() const;
    //
    void SetForm(turnLog_FORM form_) { m_form = form_; }
    turnLog_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** When set to 'true', the turn begins on the second half of the beat. **/
    data_BOOLEAN m_delayed;
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    turnLog_FORM m_form;

    /* include <attform> */
};

} // vrv namespace

#endif // __VRV_ATTS_CMNORNAMENTS_H__
