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

#ifndef __VRV_ATTS_NEUMES_H__
#define __VRV_ATTS_NEUMES_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttNeumeLog
//----------------------------------------------------------------------------

class AttNeumeLog : public Att {
public:
    AttNeumeLog();
    virtual ~AttNeumeLog();

    /** Reset the default values for the attribute class **/
    void ResetNeumeLog();

    /** Read the values for the attribute class **/
    bool ReadNeumeLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNeumeLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetName(std::string name_) { m_name = name_; }
    std::string GetName() const { return m_name; }
    bool HasName() const;
    ///@}

private:
    /** Name of the attribute. **/
    std::string m_name;

    /* include <attname> */
};

} // vrv namespace

#endif // __VRV_ATTS_NEUMES_H__
