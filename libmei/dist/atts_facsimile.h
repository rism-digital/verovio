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

#ifndef __LIBMEI_ATTS_FACSIMILE_H__
#define __LIBMEI_ATTS_FACSIMILE_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttFacsimile
//----------------------------------------------------------------------------

class AttFacsimile : public Att {
protected:
    AttFacsimile();
    ~AttFacsimile() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetFacsimile();

    /** Read the values for the attribute class **/
    bool ReadFacsimile(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteFacsimile(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFacs(std::string facs_) { m_facs = facs_; }
    std::string GetFacs() const { return m_facs; }
    bool HasFacs() const;
    ///@}

private:
    /**
     * Points to one or more images, portions of an image, or surfaces which correspond
     * to the current element.
     **/
    std::string m_facs;
};

//----------------------------------------------------------------------------
// InstFacsimile
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttFacsimile
 */

class InstFacsimile : public AttFacsimile {
public:
    InstFacsimile() = default;
    virtual ~InstFacsimile() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_FACSIMILE_H__
