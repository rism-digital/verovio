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
    // If there is glyph.num, prioritize it, otherwise check glyph.name
    if (HasGlyphNum()) {
        wchar_t code = GetGlyphNum();
        if (NULL != Resources::GetGlyph(code)) return code;
    }

    // If there is no glyph.name, handle glyph based on other attributes
    if (!HasGlyphName()) {
        if (GetLong() == BOOLEAN_true) {
            return GetForm() == mordentLog_FORM_upper ? SMUFL_E56E_ornamentTremblement : SMUFL_E56E_ornamentTremblement;
        }
        return GetForm() == mordentLog_FORM_upper ? SMUFL_E56C_ornamentMordent : SMUFL_E56D_ornamentMordentInverted;
    }

    // If there's glyph.name,
    return GetOrnamentGlyphByName(GetGlyphName());
}

wchar_t Mordent::GetOrnamentGlyphByName(const std::string& glyphName) const {
    static std::map<std::string, wchar_t> extendedOrnaments = { 
        { "ornamentSchleifer", SMUFL_E56D_ornamentMordentInverted },
        { "ornamentPrecompAppoggTrill", SMUFL_E56D_ornamentMordentInverted },
        { "ornamentPrecompPortDeVoixMordent", SMUFL_E56D_ornamentMordentInverted },
        { "ornamentPrecompInvertedMordentUpperPrefix", SMUFL_E56D_ornamentMordentInverted },
        //{ "", SMUFL_E56D_ornamentMordentInverted }, #depart_below
        { "ornamentPrecompTrillSuffixDandrieu", SMUFL_E56D_ornamentMordentInverted },
        { "ornamentPrecompDoubleCadenceUpperPrefix", SMUFL_E56D_ornamentMordentInverted }
    };

    if (extendedOrnaments.end() != extendedOrnaments.find(glyphName)) {
        return extendedOrnaments[glyphName];
    }

    return SMUFL_E56D_ornamentMordentInverted;
}

//----------------------------------------------------------------------------
// Mordent functor methods
//----------------------------------------------------------------------------

} // namespace vrv
