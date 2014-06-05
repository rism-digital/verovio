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

#ifndef __VRV_HEADERMIXIN_H__
#define __VRV_HEADERMIXIN_H__

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttRegularmethod
//----------------------------------------------------------------------------

class AttRegularmethod 
{
public:
    AttRegularmethod();
    virtual ~AttRegularmethod();
    
    /** Reset the default values for the attribute class **/
    void ResetRegularmethod();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMethod(std::string method_) { m_method = method_; };
    std::string GetMethod() { return m_method; };
    ///@}

protected:
    /** Indicates the method employed to mark corrections and normalizations. **/
    std::string m_method;

/* include <attmethod> */
};

} // vrv namespace

#endif  // __VRV_HEADERMIXIN_H__

