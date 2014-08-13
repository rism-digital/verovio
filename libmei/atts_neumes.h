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

#ifndef __VRV_ATTS_NEUMES_H__
#define __VRV_ATTS_NEUMES_H__

#include "att.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttIneumeLog
//----------------------------------------------------------------------------

class AttIneumeLog: public Att
{
public:
    AttIneumeLog();
    virtual ~AttIneumeLog();
    
    /** Reset the default values for the attribute class **/
    void ResetIneumeLog();
    
    /** Read the values for the attribute class **/
    bool ReadIneumeLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteIneumeLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() const { return m_form; };    
    bool HasForm( );
    
    //
    void SetName(std::string name_) { m_name = name_; };
    std::string GetName() const { return m_name; };    
    bool HasName( );
    
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;
    /** Records the name of the neume. **/
    std::string m_name;

/* include <attname> */
};

//----------------------------------------------------------------------------
// AttUneumeLog
//----------------------------------------------------------------------------

class AttUneumeLog: public Att
{
public:
    AttUneumeLog();
    virtual ~AttUneumeLog();
    
    /** Reset the default values for the attribute class **/
    void ResetUneumeLog();
    
    /** Read the values for the attribute class **/
    bool ReadUneumeLog( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteUneumeLog( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() const { return m_form; };    
    bool HasForm( );
    
    //
    void SetName(std::string name_) { m_name = name_; };
    std::string GetName() const { return m_name; };    
    bool HasName( );
    
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;
    /** Records the name of the neume. **/
    std::string m_name;

/* include <attname> */
};

} // vrv namespace

#endif  // __VRV_ATTS_NEUMES_H__

