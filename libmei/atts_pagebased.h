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

#ifndef __VRV_ATTS_PAGEBASED_H__
#define __VRV_ATTS_PAGEBASED_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttSurface
//----------------------------------------------------------------------------

class AttSurface : public Att {
public:
    AttSurface();
    virtual ~AttSurface();

    /** Reset the default values for the attribute class **/
    void ResetSurface();

    /** Read the values for the attribute class **/
    bool ReadSurface(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSurface(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSurface(std::string surface_) { m_surface = surface_; }
    std::string GetSurface() const { return m_surface; }
    bool HasSurface() const;
    ///@}

private:
    /** Contains a reference to a surface element **/
    std::string m_surface;

    /* include <attsurface> */
};

} // vrv namespace

#endif // __VRV_ATTS_PAGEBASED_H__
