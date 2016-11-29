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

#ifndef __VRV_ATTS_EXTERNALSYMBOLS_H__
#define __VRV_ATTS_EXTERNALSYMBOLS_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttExtsym
//----------------------------------------------------------------------------

class AttExtsym : public Att {
public:
    AttExtsym();
    virtual ~AttExtsym();

    /** Reset the default values for the attribute class **/
    void ResetExtsym();

    /** Read the values for the attribute class **/
    bool ReadExtsym(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteExtsym(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetGlyphname(std::string glyphname_) { m_glyphname = glyphname_; }
    std::string GetGlyphname() const { return m_glyphname; }
    bool HasGlyphname() const;
    //
    void SetGlyphnum(wchar_t glyphnum_) { m_glyphnum = glyphnum_; }
    wchar_t GetGlyphnum() const { return m_glyphnum; }
    bool HasGlyphnum() const;
    ///@}

private:
    /** Glyph name. **/
    std::string m_glyphname;
    /**
     * Numeric glyph reference in hexadecimal notation, e.g.
     * "#xE000" or "U+E000". N.B. SMuFL version 1.18 uses the range U+E000 - U+ECBF.
     **/
    wchar_t m_glyphnum;

    /* include <attglyphnum> */
};

} // vrv namespace

#endif // __VRV_ATTS_EXTERNALSYMBOLS_H__
