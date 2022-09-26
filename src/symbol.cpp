/////////////////////////////////////////////////////////////////////////////
// Name:        symbol.cpp
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "symbol.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "resources.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Symbol
//----------------------------------------------------------------------------

static const ClassRegistrar<Symbol> s_factory("symbol", SYMBOL);

Symbol::Symbol() : TextElement(SYMBOL, "symbol-"), AttColor(), AttExtSym(), AttTypography()
{
    this->Reset();

    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTSYM);
    this->RegisterAttClass(ATT_TYPOGRAPHY);
}

Symbol::~Symbol() {}

void Symbol::Reset()
{
    TextElement::Reset();

    this->ResetColor();
    this->ResetExtSym();
    this->ResetTypography();
}

bool Symbol::IsSupportedChild(Object *child)
{
    return false;
}

char32_t Symbol::GetSymbolGlyph() const
{
    const Resources *resources = this->GetDocResources();
    if (!resources) return 0;

    // If there is glyph.num, prioritize it
    if (this->HasGlyphNum()) {
        char32_t code = this->GetGlyphNum();
        if (NULL != resources->GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (this->HasGlyphName()) {
        char32_t code = resources->GetGlyphCode(this->GetGlyphName());
        if (NULL != resources->GetGlyph(code)) return code;
    }

    return 0;
}

} // namespace vrv
