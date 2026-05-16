/////////////////////////////////////////////////////////////////////////////
// Name:        syllable.cpp
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "syllable.h"
#include "scoredefinterface.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <cassert>
#include <iostream>

//----------------------------------------------------------------------------

#include "divline.h"
#include "functor.h"
#include "neume.h"
#include "syl.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Syllable
//----------------------------------------------------------------------------

static const ClassRegistrar<Syllable> s_factory("syllable", SYLLABLE);

Syllable::Syllable() : LayerElement(SYLLABLE), ObjectListInterface(), AttColor(), AttSlashCount()
{
    this->Init();
}

void Syllable::Init()
{
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_SLASHCOUNT);

    this->Reset();
}

bool Syllable::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{
        ACCID,
        CLEF,
        DIVLINE,
        NEUME,
        SYL,
    };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

Syllable::~Syllable() {}

void Syllable::Reset()
{
    LayerElement::Reset();
    this->ResetColor();
    this->ResetSlashCount();
}

bool Syllable::MarkupAddSyl()
{
    Object *obj = this->FindDescendantByType(SYL);
    ArrayOfStrAttr attributes;
    this->GetAttributes(&attributes);
    bool noFollows = std::find_if(attributes.begin(), attributes.end(), [](auto att) -> bool {
        return (std::string{ "follows" }.compare(att.first) == 0);
    }) == attributes.end();
    if (noFollows && (obj == NULL)) {
        Syl *syl = new Syl();
        Text *text = new Text();
        syl->AddChild(text);
        this->AddChild(syl);
        return true;
    }
    return false;
}

FunctorCode Syllable::Accept(Functor &functor)
{
    return functor.VisitSyllable(this);
}

FunctorCode Syllable::Accept(ConstFunctor &functor) const
{
    return functor.VisitSyllable(this);
}

FunctorCode Syllable::AcceptEnd(Functor &functor)
{
    return functor.VisitSyllableEnd(this);
}

FunctorCode Syllable::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSyllableEnd(this);
}

} // namespace vrv
