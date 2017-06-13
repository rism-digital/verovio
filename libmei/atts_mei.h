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

#ifndef __VRV_ATTS_MEI_H__
#define __VRV_ATTS_MEI_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttNotationType
//----------------------------------------------------------------------------

class AttNotationType : public Att {
public:
    AttNotationType();
    virtual ~AttNotationType();

    /** Reset the default values for the attribute class **/
    void ResetNotationType();

    /** Read the values for the attribute class **/
    bool ReadNotationType(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNotationType(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetNotationtype(data_NOTATIONTYPE notationtype_) { m_notationtype = notationtype_; }
    data_NOTATIONTYPE GetNotationtype() const { return m_notationtype; }
    bool HasNotationtype() const;
    //
    void SetNotationsubtype(std::string notationsubtype_) { m_notationsubtype = notationsubtype_; }
    std::string GetNotationsubtype() const { return m_notationsubtype; }
    bool HasNotationsubtype() const;
    ///@}

private:
    /**
     * Contains classification of the notation contained or described by the element
     * bearing this attribute.
     **/
    data_NOTATIONTYPE m_notationtype;
    /**
     * Provides any sub-classification of the notation contained or described by the
     * element, additional to that given by its notationtype attribute.
     **/
    std::string m_notationsubtype;

    /* include <attnotationsubtype> */
};

} // vrv namespace

#endif // __VRV_ATTS_MEI_H__
