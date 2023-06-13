/////////////////////////////////////////////////////////////////////////////
// Name:        repeatmark.cpp
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "repeatmark.h"

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
// RepeatMark
//----------------------------------------------------------------------------

static const ClassRegistrar<RepeatMark> s_factory("repeatMark", REPEATMARK);

RepeatMark::RepeatMark()
    : ControlElement(REPEATMARK, "repeatMark-")
    , TextListInterface()
    , TextDirInterface()
    , TimePointInterface()
    , AttColor()
    , AttExtSymAuth()
    , AttExtSymNames()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTSYMAUTH);
    this->RegisterAttClass(ATT_EXTSYMNAMES);

    this->Reset();
}

RepeatMark::~RepeatMark() {}

void RepeatMark::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
    this->ResetColor();
    this->ResetExtSymAuth();
    this->ResetExtSymNames();
}

bool RepeatMark::IsSupportedChild(Object *child)
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

char32_t RepeatMark::GetMarkGlyph() const
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
// RepeatMark functor methods
//----------------------------------------------------------------------------

FunctorCode RepeatMark::Accept(Functor &functor)
{
    return functor.VisitRepeatMark(this);
}

FunctorCode RepeatMark::Accept(ConstFunctor &functor) const
{
    return functor.VisitRepeatMark(this);
}

FunctorCode RepeatMark::AcceptEnd(Functor &functor)
{
    return functor.VisitRepeatMarkEnd(this);
}

FunctorCode RepeatMark::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitRepeatMarkEnd(this);
}

} // namespace vrv
