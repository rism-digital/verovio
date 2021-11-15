/////////////////////////////////////////////////////////////////////////////
// Name:        custos.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "custos.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"

namespace vrv {

//----------------------------------------------------------------------------
// Custos
//----------------------------------------------------------------------------

static const ClassRegistrar<Custos> s_factory("custos", CUSTOS);

Custos::Custos() : LayerElement(CUSTOS, "custos-"), PitchInterface(), PositionInterface(), AttColor(), AttExtSym()
{
    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_EXTSYM);

    Reset();
}

Custos::~Custos() {}

void Custos::Reset()
{
    LayerElement::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    ResetColor();
    ResetExtSym();
}

bool Custos::IsSupportedChild(Object *child)
{
    if (child->Is(ACCID)) {
        assert(dynamic_cast<Accid *>(child));
    }
    else {
        return false;
    }
    return true;
}

wchar_t Custos::GetCustosGlyph(const data_NOTATIONTYPE notationtype) const
{
    // If there is glyph.num, prioritize it
    if (HasGlyphNum()) {
        wchar_t code = GetGlyphNum();
        if (NULL != Resources::GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (HasGlyphName()) {
        wchar_t code = Resources::GetGlyphCode(GetGlyphName());
        if (NULL != Resources::GetGlyph(code)) return code;
    }
    
    switch (notationtype) {
        case NOTATIONTYPE_neume:
            return SMUFL_EA06_chantCustosStemUpPosMiddle; // chantCustosStemUpPosMiddle
            break;
        default:
            return SMUFL_EA02_mensuralCustosUp; // mensuralCustosUp
            break;
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Custos::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);
    PositionInterface::InterfaceResetDrawing(functorParams, this);

    return FUNCTOR_CONTINUE;
}

int Custos::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
