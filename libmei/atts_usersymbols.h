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

#ifndef __VRV_ATTS_USERSYMBOLS_H__
#define __VRV_ATTS_USERSYMBOLS_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttAltsym
//----------------------------------------------------------------------------

class AttAltsym: public Att
{
public:
    AttAltsym();
    virtual ~AttAltsym();
    
    /** Reset the default values for the attribute class **/
    void ResetAltsym();
    
    /** Read the values for the attribute class **/
    bool ReadAltsym(pugi::xml_node element);
    
    /** Write the values for the attribute class **/
    bool WriteAltsym(pugi::xml_node element);
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetAltsym(std::string altsym_) { m_altsym = altsym_; };
    std::string GetAltsym() const { return m_altsym; };    
    bool HasAltsym();
    
    ///@}

private:
    /**
     * Provides a way of pointing to a user-defined symbol.
     * It must contain an ID of a <symbolDef> element elsewhere in the document.
     **/
    std::string m_altsym;

/* include <attaltsym> */
};

} // vrv namespace

#endif  // __VRV_ATTS_USERSYMBOLS_H__

