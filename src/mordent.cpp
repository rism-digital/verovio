/////////////////////////////////////////////////////////////////////////////
// Name:        mordent.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mordent.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "horizontalaligner.h"
#include "smufl.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mordent
//----------------------------------------------------------------------------

Mordent::Mordent()
    : ControlElement("mordent-"), TimePointInterface(), AttColor(), AttOrnamentAccid(), AttPlacement(), AttMordentLog()
{
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_ORNAMENTACCID);
    RegisterAttClass(ATT_PLACEMENT);
    RegisterAttClass(ATT_MORDENTLOG);
    RegisterAttClass(ATT_EXTSYM);

    Reset();
}

Mordent::~Mordent() {}

void Mordent::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    ResetColor();
    ResetOrnamentAccid();
    ResetPlacement();
    ResetMordentLog();
    ResetExtSym();
}

wchar_t Mordent::GetMordentGlyph() const
{
    // order of glyphs in the map: [0] long, [1] long inverted, [2] short, [3] short inverted
    static std::map<std::string, std::vector<wchar_t> > mordentGlyphs = { 
        { "default",
            { SMUFL_E56E_ornamentTremblement, SMUFL_E56E_ornamentTremblement,
              SMUFL_E56C_ornamentMordent, SMUFL_E56D_ornamentMordentInverted } },
        { "appproach_above", 
            { SMUFL_E56E_ornamentTremblement, SMUFL_E56E_ornamentTremblement,
              SMUFL_E56C_ornamentMordent, SMUFL_E56D_ornamentMordentInverted } },
        { "approach_below",
            { SMUFL_E56E_ornamentTremblement, SMUFL_E56E_ornamentTremblement, 
              SMUFL_E56C_ornamentMordent, SMUFL_E56D_ornamentMordentInverted } },
        { "depart_above",
            { SMUFL_E56E_ornamentTremblement, SMUFL_E56E_ornamentTremblement, 
              SMUFL_E56C_ornamentMordent, SMUFL_E56D_ornamentMordentInverted } },
        { "depart_below",
            { SMUFL_E56E_ornamentTremblement, SMUFL_E56E_ornamentTremblement, 
              SMUFL_E56C_ornamentMordent, SMUFL_E56D_ornamentMordentInverted } }
        };
    // If there's no glyph.name, get default category which encompases 4 simple glyphs:
    //   mordent, inverted mordent and their long counterparts
    std::string glyphName = HasGlyphName() ? GetGlyphName()  : "default";
    if (mordentGlyphs.end() != mordentGlyphs.find(glyphName)) {
        return GetMordentGlyph(mordentGlyphs[glyphName]);
    }
    // If glyph.name is not in the map, then we can either process it further here or return default glyph
    if (glyphName == "schleifer") {
        return SMUFL_E56D_ornamentMordentInverted; // TODO: should be schleifer glyph here
    }
    return SMUFL_E56D_ornamentMordentInverted;
}

wchar_t Mordent::GetMordentGlyph(std::vector<wchar_t>& glyphs) const
{
    // order of glyphs in the vector is as following:
    // [0] long, [1] long inverted, [2] short, [3] short inverted
    if (GetLong() == BOOLEAN_true) {
        return GetForm() == mordentLog_FORM_upper ? glyphs[0] : glyphs[1];
    }
    return GetForm() == mordentLog_FORM_upper ? glyphs[2] : glyphs[3];
}

//----------------------------------------------------------------------------
// Mordent functor methods
//----------------------------------------------------------------------------

} // namespace vrv
