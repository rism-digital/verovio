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

#ifndef __VRV_ATTS_TABLATURE_H__
#define __VRV_ATTS_TABLATURE_H__

#include "att.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttNoteGesTablature
//----------------------------------------------------------------------------

#define ATT_NOTEGESTABLATURE 201

class AttNoteGesTablature: public Att
{
public:
    AttNoteGesTablature();
    virtual ~AttNoteGesTablature();
    
    /** Reset the default values for the attribute class **/
    void ResetNoteGesTablature();
    
    /** Read the values for the attribute class **/
    bool ReadNoteGesTablature( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteNoteGesTablature( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTabFret(std::string tabFret_) { m_tabFret = tabFret_; };
    std::string GetTabFret() const { return m_tabFret; };    
    bool HasTabFret( );
    
    //
    void SetTabString(std::string tabString_) { m_tabString = tabString_; };
    std::string GetTabString() const { return m_tabString; };    
    bool HasTabString( );
    
    ///@}

private:
    /** Records the fret at which a string should be stopped. **/
    std::string m_tabFret;
    /** Records which string is to be played. **/
    std::string m_tabString;

/* include <atttab.string> */
};

//----------------------------------------------------------------------------
// AttStaffDefGesTablature
//----------------------------------------------------------------------------

#define ATT_STAFFDEFGESTABLATURE 202

class AttStaffDefGesTablature: public Att
{
public:
    AttStaffDefGesTablature();
    virtual ~AttStaffDefGesTablature();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffDefGesTablature();
    
    /** Read the values for the attribute class **/
    bool ReadStaffDefGesTablature( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteStaffDefGesTablature( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetTabStrings(std::string tabStrings_) { m_tabStrings = tabStrings_; };
    std::string GetTabStrings() const { return m_tabStrings; };    
    bool HasTabStrings( );
    
    ///@}

private:
    /** Provides a *written* pitch and octave for each open string. **/
    std::string m_tabStrings;

/* include <atttab.strings> */
};

} // vrv namespace

#endif  // __VRV_ATTS_TABLATURE_H__

