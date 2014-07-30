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

#include "vrvdef.h"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttNoteGesTablature
//----------------------------------------------------------------------------

class AttNoteGesTablature 
{
public:
    AttNoteGesTablature();
    virtual ~AttNoteGesTablature();
    
    /** Reset the default values for the attribute class **/
    void ResetNoteGesTablature();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTabFret(std::string tabFret_) { m_tabFret = tabFret_; };
    std::string GetTabFret() { return m_tabFret; };
    //
    void SetTabString(std::string tabString_) { m_tabString = tabString_; };
    std::string GetTabString() { return m_tabString; };
    ///@}

protected:
    /** Records the fret at which a string should be stopped. **/
    std::string m_tabFret;
    /** Records which string is to be played. **/
    std::string m_tabString;

/* include <atttab.string> */
};

//----------------------------------------------------------------------------
// AttStaffDefGesTablature
//----------------------------------------------------------------------------

class AttStaffDefGesTablature 
{
public:
    AttStaffDefGesTablature();
    virtual ~AttStaffDefGesTablature();
    
    /** Reset the default values for the attribute class **/
    void ResetStaffDefGesTablature();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetTabStrings(std::string tabStrings_) { m_tabStrings = tabStrings_; };
    std::string GetTabStrings() { return m_tabStrings; };
    ///@}

protected:
    /** Provides a *written* pitch and octave for each open string. **/
    std::string m_tabStrings;

/* include <atttab.strings> */
};

} // vrv namespace

#endif  // __VRV_ATTS_TABLATURE_H__

