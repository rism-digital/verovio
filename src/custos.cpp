/////////////////////////////////////////////////////////////////////////////
// Name:        custos.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "custos.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "smufl.h"

namespace vrv {

//----------------------------------------------------------------------------
// Custos
//----------------------------------------------------------------------------

static const ClassRegistrar<Custos> s_factory("custos", CUSTOS);

Custos::Custos() : LayerElement(CUSTOS, "custos-"), PitchInterface(), PositionInterface(), AttColor(), AttExtSym()
{
    this->RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTSYM);

    this->Reset();
}

Custos::~Custos() {}

void Custos::Reset()
{
    LayerElement::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    this->ResetColor();
    this->ResetExtSym();
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

char32_t Custos::GetCustosGlyph(const data_NOTATIONTYPE notationtype) const
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

int Custos::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetData(functorParams);
    PositionInterface::InterfaceResetData(functorParams, this);

    return FUNCTOR_CONTINUE;
}

int Custos::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
