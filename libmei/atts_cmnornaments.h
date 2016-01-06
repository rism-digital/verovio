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



namespace vrv {
    
//----------------------------------------------------------------------------
// AttMordentLog
//----------------------------------------------------------------------------

class AttMordentLog: public Att
{
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
    void SetForm(mordentLog_FORM form_) { m_form = form_; };
    mordentLog_FORM GetForm() const { return m_form; };    
    bool HasForm();
    
    //
    void SetLong(data_BOOLEAN long_) { m_long = long_; };
    data_BOOLEAN GetLong() const { return m_long; };    
    bool HasLong();
    
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    mordentLog_FORM m_form;
    /**
     * When the long attribute is set to 'yes', a double or long mordent, consisting of
     * 5 notes, is indicated.
     **/
    data_BOOLEAN m_long;

/* include <attlong> */
};

//----------------------------------------------------------------------------
// AttOrnam
//----------------------------------------------------------------------------

class AttOrnam: public Att
{
public:
    AttOrnam();
    virtual ~AttOrnam();
    
    /** Reset the default values for the attribute class **/
    void ResetOrnam();
    
    /** Read the values for the attribute class **/
    bool ReadOrnam(pugi::xml_node element);
    
    /** Write the values for the attribute class **/
    bool WriteOrnam(pugi::xml_node element);
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetOrnam(data_ORNAM_cmn ornam_) { m_ornam = ornam_; };
    data_ORNAM_cmn GetOrnam() const { return m_ornam; };    
    bool HasOrnam();
    
    ///@}

private:
    /**
     * Indicates that this element has an attached ornament.
     * If visual information about the ornament is needed, then one of the elements
     * that represents an ornament (mordent, trill, or turn) should be employed.
     **/
    data_ORNAM_cmn m_ornam;

/* include <attornam> */
};

//----------------------------------------------------------------------------
// AttOrnamentaccid
//----------------------------------------------------------------------------

class AttOrnamentaccid: public Att
{
public:
    AttOrnamentaccid();
    virtual ~AttOrnamentaccid();
    
    /** Reset the default values for the attribute class **/
    void ResetOrnamentaccid();
    
    /** Read the values for the attribute class **/
    bool ReadOrnamentaccid(pugi::xml_node element);
    
    /** Write the values for the attribute class **/
    bool WriteOrnamentaccid(pugi::xml_node element);
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetAccidupper(data_ACCIDENTAL_EXPLICIT accidupper_) { m_accidupper = accidupper_; };
    data_ACCIDENTAL_EXPLICIT GetAccidupper() const { return m_accidupper; };    
    bool HasAccidupper();
    
    //
    void SetAccidlower(data_ACCIDENTAL_EXPLICIT accidlower_) { m_accidlower = accidlower_; };
    data_ACCIDENTAL_EXPLICIT GetAccidlower() const { return m_accidlower; };    
    bool HasAccidlower();
    
    ///@}

private:
    /** Records the written accidental associated with an upper neighboring note. **/
    data_ACCIDENTAL_EXPLICIT m_accidupper;
    /** Records the written accidental associated with a lower neighboring note. **/
    data_ACCIDENTAL_EXPLICIT m_accidlower;

/* include <attaccidlower> */
};

//----------------------------------------------------------------------------
// AttTurnLog
//----------------------------------------------------------------------------

class AttTurnLog: public Att
{
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
    void SetDelayed(data_BOOLEAN delayed_) { m_delayed = delayed_; };
    data_BOOLEAN GetDelayed() const { return m_delayed; };    
    bool HasDelayed();
    
    //
    void SetForm(turnLog_FORM form_) { m_form = form_; };
    turnLog_FORM GetForm() const { return m_form; };    
    bool HasForm();
    
    ///@}

private:
    /**
     * When the delayed attribute is set to 'true', the turn begins on the second half
     * of the beat.
     * See Read, p. 246.
     **/
    data_BOOLEAN m_delayed;
    /** Records the appearance and usually the function of the bar line. **/
    turnLog_FORM m_form;

/* include <attform> */
};

} // vrv namespace

#endif  // __VRV_ATTS_CMNORNAMENTS_H__

