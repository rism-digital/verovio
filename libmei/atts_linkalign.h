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

#ifndef __VRV_ATTS_LINKALIGN_H__
#define __VRV_ATTS_LINKALIGN_H__

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttAlignment
//----------------------------------------------------------------------------

class AttAlignment 
{
public:
    AttAlignment();
    virtual ~AttAlignment();
    
    /** Reset the default values for the attribute class **/
    void ResetAlignment();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetWhen(std::string when_) { m_when = when_; };
    std::string GetWhen() { return m_when; };
    ///@}

protected:
    /**
     * Indicates the point of occurrence of this feature along a time line.
     * Its value must be the ID of a <when> element.
     **/
    std::string m_when;

/* include <attwhen> */
};

} // vrv namespace

#endif  // __VRV_ATTS_LINKALIGN_H__

