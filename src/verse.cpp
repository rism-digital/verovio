/////////////////////////////////////////////////////////////////////////////
// Name:        verse.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "verse.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "label.h"
#include "labelabbr.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

static const ClassRegistrar<Verse> s_factory("verse", VERSE);

Verse::Verse() : LyricElement(VERSE), AttNInteger()
{
    this->RegisterAttClass(ATT_NINTEGER);
    this->Reset();
}

Verse::~Verse() {}

void Verse::Reset()
{
    LyricElement::Reset();
    this->ResetNInteger();

    m_drawingLabelAbbr = NULL;
}

bool Verse::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ LABEL, LABELABBR };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return LyricElement::IsSupportedChild(classId);
    }
}

//----------------------------------------------------------------------------
// Verse functor methods
//----------------------------------------------------------------------------

FunctorCode Verse::Accept(Functor &functor)
{
    return functor.VisitVerse(this);
}

FunctorCode Verse::Accept(ConstFunctor &functor) const
{
    return functor.VisitVerse(this);
}

FunctorCode Verse::AcceptEnd(Functor &functor)
{
    return functor.VisitVerseEnd(this);
}

FunctorCode Verse::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitVerseEnd(this);
}

} // namespace vrv
