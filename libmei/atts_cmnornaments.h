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
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

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
    bool ReadMordentLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMordentLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() const { return m_form; };
    //
    void SetLong(std::string long_) { m_long = long_; };
    std::string GetLong() const { return m_long; };
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;
    /**
     * When the long attribute is set to 'yes', a double or long mordent, consisting of
     * 5 notes, is indicated.
     **/
    std::string m_long;

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
    bool ReadOrnam( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteOrnam( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetOrnam(std::string ornam_) { m_ornam = ornam_; };
    std::string GetOrnam() const { return m_ornam; };
    ///@}

protected:
    /**
     * Indicates that this element has an attached ornament.
     * If visual information about the ornament is needed, then one of the elements
     * that represents an ornament (mordent, trill, or turn) should be employed.
     **/
    std::string m_ornam;

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
    bool ReadOrnamentaccid( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteOrnamentaccid( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetAccidupper(std::string accidupper_) { m_accidupper = accidupper_; };
    std::string GetAccidupper() const { return m_accidupper; };
    //
    void SetAccidlower(std::string accidlower_) { m_accidlower = accidlower_; };
    std::string GetAccidlower() const { return m_accidlower; };
    ///@}

protected:
    /** Records the written accidental associated with an upper neighboring note. **/
    std::string m_accidupper;
    /** Records the written accidental associated with a lower neighboring note. **/
    std::string m_accidlower;

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
    bool ReadTurnLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTurnLog( pugi::xml_node element );
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetDelayed(std::string delayed_) { m_delayed = delayed_; };
    std::string GetDelayed() const { return m_delayed; };
    //
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() const { return m_form; };
    ///@}

protected:
    /**
     * When the delayed attribute is set to 'true', the turn begins on the second half
     * of the beat.
     * See Read, p. 246.
     **/
    std::string m_delayed;
    /** Records the function of the dot. **/
    std::string m_form;

/* include <attform> */
};

} // vrv namespace

#endif  // __VRV_ATTS_CMNORNAMENTS_H__

