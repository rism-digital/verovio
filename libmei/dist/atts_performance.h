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

#ifndef __LIBMEI_ATTS_PERFORMANCE_H__
#define __LIBMEI_ATTS_PERFORMANCE_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttAlignment
//----------------------------------------------------------------------------

class AttAlignment : public Att {
protected:
    AttAlignment();
    ~AttAlignment() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetAlignment();

    /** Read the values for the attribute class **/
    bool ReadAlignment(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteAlignment(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetWhen(std::string when_) { m_when = when_; }
    std::string GetWhen() const { return m_when; }
    bool HasWhen() const;
    ///@}

private:
    /**
     * Indicates the point of occurrence of this feature along a time line.
     * Its value must be the ID of a when element elsewhere in the document.
     **/
    std::string m_when;
};

//----------------------------------------------------------------------------
// InstAlignment
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttAlignment
 */

class InstAlignment : public AttAlignment {
public:
    InstAlignment() = default;
    virtual ~InstAlignment() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_PERFORMANCE_H__
