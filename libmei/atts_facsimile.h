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

#ifndef __VRV_FACSIMILEMIXIN_H__
#define __VRV_FACSIMILEMIXIN_H__

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttFacsimile
//----------------------------------------------------------------------------

class AttFacsimile 
{
public:
    AttFacsimile();
    virtual ~AttFacsimile();
    
    /** Reset the default values for the attribute class **/
    void ResetFacsimile();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetFacs(std::string facs_) { m_facs = facs_; };
    std::string GetFacs() { return m_facs; };
    ///@}

protected:
    /**
     * Permits the current element to reference a facsimile image or image zone which
     * corresponds to it.
     **/
    std::string m_facs;

	/**
	 * Test
	 **/
    void getCoords();
};

} // vrv namespace

#endif  // __VRV_FACSIMILEMIXIN_H__

