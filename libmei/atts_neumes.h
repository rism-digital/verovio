/////////////////////////////////////////////////////////////////////////////
// Name:        attributes.h
// Author:      Laurent Pugin
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

#ifndef __VRV_NEUMESMIXIN_H__
#define __VRV_NEUMESMIXIN_H__

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttIneumeLog
//----------------------------------------------------------------------------

class AttIneumeLog 
{
public:
    AttIneumeLog();
    virtual ~AttIneumeLog();
    
    /** Reset the default values for the attribute class **/
    void ResetIneumeLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() { return m_form; };
    //
    void SetName(std::string name_) { m_name = name_; };
    std::string GetName() { return m_name; };
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

class AttUneumeLog 
{
public:
    AttUneumeLog();
    virtual ~AttUneumeLog();
    
    /** Reset the default values for the attribute class **/
    void ResetUneumeLog();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetForm(std::string form_) { m_form = form_; };
    std::string GetForm() { return m_form; };
    //
    void SetName(std::string name_) { m_name = name_; };
    std::string GetName() { return m_name; };
    ///@}

protected:
    /** Records the function of the dot. **/
    std::string m_form;
    /** Records the name of the neume. **/
    std::string m_name;

/* include <attname> */
};

} // vrv namespace

#endif  // __VRV_NEUMESMIXIN_H__

