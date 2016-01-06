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

#ifndef __VRV_ATTS_HEADER_H__
#define __VRV_ATTS_HEADER_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------



namespace vrv {
    
//----------------------------------------------------------------------------
// AttRegularmethod
//----------------------------------------------------------------------------

class AttRegularmethod: public Att
{
public:
    AttRegularmethod();
    virtual ~AttRegularmethod();
    
    /** Reset the default values for the attribute class **/
    void ResetRegularmethod();
    
    /** Read the values for the attribute class **/
    bool ReadRegularmethod(pugi::xml_node element);
    
    /** Write the values for the attribute class **/
    bool WriteRegularmethod(pugi::xml_node element);
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMethod(regularmethod_METHOD method_) { m_method = method_; };
    regularmethod_METHOD GetMethod() const { return m_method; };    
    bool HasMethod();
    
    ///@}

private:
    /** Indicates the method employed to mark corrections and normalizations. **/
    regularmethod_METHOD m_method;

/* include <attmethod> */
};

} // vrv namespace

#endif  // __VRV_ATTS_HEADER_H__

