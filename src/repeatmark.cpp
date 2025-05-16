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
    : ControlElement(REPEATMARK)
    , TextListInterface()
    , TextDirInterface()
    , TimePointInterface()
    , AttExtSymAuth()
    , AttExtSymNames()
    , AttRepeatMarkLog()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_EXTSYMAUTH);
    this->RegisterAttClass(ATT_EXTSYMNAMES);
    this->RegisterAttClass(ATT_REPEATMARKLOG);

    this->Reset();
}

RepeatMark::~RepeatMark() {}

void RepeatMark::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
    this->ResetExtSymAuth();
    this->ResetExtSymNames();
    this->ResetRepeatMarkLog();
}

bool RepeatMark::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ LB, REND, SYMBOL, TEXT };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
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

    switch (this->GetFunc()) {
        case (repeatMarkLog_FUNC_coda): return SMUFL_E048_coda;
        case (repeatMarkLog_FUNC_segno): return SMUFL_E047_segno;
        case (repeatMarkLog_FUNC_daCapo): return SMUFL_E046_daCapo;
        case (repeatMarkLog_FUNC_dalSegno): return SMUFL_E045_dalSegno;
        default: return SMUFL_E047_segno;
    }
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
