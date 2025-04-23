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
#include "functor.h"
#include "smufl.h"

namespace vrv {

//----------------------------------------------------------------------------
// Custos
//----------------------------------------------------------------------------

static const ClassRegistrar<Custos> s_factory("custos", CUSTOS);

Custos::Custos()
    : LayerElement(CUSTOS), PitchInterface(), PositionInterface(), AttColor(), AttExtSymAuth(), AttExtSymNames()
{
    this->RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTSYMAUTH);
    this->RegisterAttClass(ATT_EXTSYMNAMES);

    this->Reset();
}

Custos::~Custos() {}

void Custos::Reset()
{
    LayerElement::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    this->ResetColor();
    this->ResetExtSymAuth();
    this->ResetExtSymNames();
}

bool Custos::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ ACCID };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
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

FunctorCode Custos::Accept(Functor &functor)
{
    return functor.VisitCustos(this);
}

FunctorCode Custos::Accept(ConstFunctor &functor) const
{
    return functor.VisitCustos(this);
}

FunctorCode Custos::AcceptEnd(Functor &functor)
{
    return functor.VisitCustosEnd(this);
}

FunctorCode Custos::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitCustosEnd(this);
}

} // namespace vrv
