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
// AttExtSym
//----------------------------------------------------------------------------

class AttExtSym : public Att {
public:
    AttExtSym();
    virtual ~AttExtSym();

    /** Reset the default values for the attribute class **/
    void ResetExtSym();

    /** Read the values for the attribute class **/
    bool ReadExtSym(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteExtSym(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetGlyphAuth(std::string glyphAuth_) { m_glyphAuth = glyphAuth_; }
    std::string GetGlyphAuth() const { return m_glyphAuth; }
    bool HasGlyphAuth() const;
    //
    void SetGlyphName(std::string glyphName_) { m_glyphName = glyphName_; }
    std::string GetGlyphName() const { return m_glyphName; }
    bool HasGlyphName() const;
    //
    void SetGlyphNum(data_HEXNUM glyphNum_) { m_glyphNum = glyphNum_; }
    data_HEXNUM GetGlyphNum() const { return m_glyphNum; }
    bool HasGlyphNum() const;
    //
    void SetGlyphUri(std::string glyphUri_) { m_glyphUri = glyphUri_; }
    std::string GetGlyphUri() const { return m_glyphUri; }
    bool HasGlyphUri() const;
    ///@}

private:
    /**
     * A name or label associated with the controlled vocabulary from which the value
     * of
     **/
    std::string m_glyphAuth;
    /** Glyph name. **/
    std::string m_glyphName;
    /**
     * Numeric glyph reference in hexadecimal notation, e.g.
     * "#xE000" or "U+E000". N.B. SMuFL version 1.18 uses the range U+E000 - U+ECBF.
     **/
    data_HEXNUM m_glyphNum;
    /** The web-accessible location of the controlled vocabulary from which the value of **/
    std::string m_glyphUri;

    /* include <attglyph.uri> */
};

} // vrv namespace

#endif // __VRV_ATTS_EXTERNALSYMBOLS_H__
