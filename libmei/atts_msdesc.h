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

#ifndef __VRV_ATTS_MSDESC_H__
#define __VRV_ATTS_MSDESC_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttComponentType
//----------------------------------------------------------------------------

class AttComponentType : public Att {
public:
    AttComponentType();
    virtual ~AttComponentType();

    /** Reset the default values for the attribute class **/
    void ResetComponentType();

    /** Read the values for the attribute class **/
    bool ReadComponentType(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteComponentType(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetComptype(componentType_COMPTYPE comptype_) { m_comptype = comptype_; }
    componentType_COMPTYPE GetComptype() const { return m_comptype; }
    bool HasComptype() const;
    ///@}

private:
    /** --- **/
    componentType_COMPTYPE m_comptype;

    /* include <attcomptype> */
};

} // vrv namespace

#endif // __VRV_ATTS_MSDESC_H__
