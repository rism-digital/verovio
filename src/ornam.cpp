/////////////////////////////////////////////////////////////////////////////
// Name:        ornam.cpp
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ornam.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "layerelement.h"
#include "resources.h"
#include "smufl.h"
#include "symbol.h"
#include "text.h"

namespace vrv {

//----------------------------------------------------------------------------
// Ornam
//----------------------------------------------------------------------------

static const ClassRegistrar<Ornam> s_factory("ornam", ORNAM);

Ornam::Ornam()
    : ControlElement(ORNAM, "ornam-")
    , TextListInterface()
    , TextDirInterface()
    , TimePointInterface()
    , AttColor()
    , AttExtSym()
    , AttOrnamentAccid()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTSYM);
    this->RegisterAttClass(ATT_ORNAMENTACCID);

    this->Reset();
}

Ornam::~Ornam() {}

void Ornam::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
    this->ResetColor();
    this->ResetExtSym();
    this->ResetOrnamentAccid();
    this->ResetPlacementRelStaff();
}

bool Ornam::IsSupportedChild(Object *child)
{
    if (child->Is({ LB, REND, SYMBOL, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

char32_t Ornam::GetOrnamGlyph() const
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

    return SMUFL_E567_ornamentTurn;
}

//----------------------------------------------------------------------------
// Ornam functor methods
//----------------------------------------------------------------------------

FunctorCode Ornam::Accept(MutableFunctor &functor)
{
    return functor.VisitOrnam(this);
}

FunctorCode Ornam::Accept(ConstFunctor &functor) const
{
    return functor.VisitOrnam(this);
}

FunctorCode Ornam::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitOrnamEnd(this);
}

FunctorCode Ornam::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitOrnamEnd(this);
}

} // namespace vrv
