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

#ifndef __LIBMEI_ATTS_CRITAPP_H__
#define __LIBMEI_ATTS_CRITAPP_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttCrit
//----------------------------------------------------------------------------

class AttCrit : public Att {
protected:
    AttCrit();
    ~AttCrit() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetCrit();

    /** Read the values for the attribute class **/
    bool ReadCrit(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteCrit(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCause(std::string cause_) { m_cause = cause_; }
    std::string GetCause() const { return m_cause; }
    bool HasCause() const;
    ///@}

private:
    /**
     * Classifies the cause for the variant reading, according to any appropriate
     * typology of possible origins.
     **/
    std::string m_cause;
};

//----------------------------------------------------------------------------
// InstCrit
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttCrit
 */

class InstCrit : public AttCrit {
public:
    InstCrit() = default;
    virtual ~InstCrit() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_CRITAPP_H__
