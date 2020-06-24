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
#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "neume.h"
#include "syl.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Syllable
//----------------------------------------------------------------------------

Syllable::Syllable() : LayerElement("syllable-"), ObjectListInterface(), AttColor(), AttSlashCount()
{
    Init();
}

void Syllable::Init()
{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_SLASHCOUNT);

    Reset();
}

bool Syllable::IsSupportedChild(Object *child)
{
    if (child->Is(SYL)) {
        assert(dynamic_cast<Syl *>(child));
    }
    else if (child->Is(NEUME)) {
        assert(dynamic_cast<Neume *>(child));
    }
    else {
        return false;
    }
    return true;
}

Syllable::~Syllable() {}

void Syllable::Reset()
{
    LayerElement::Reset();
    ResetColor();
    ResetSlashCount();
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

} // namespace vrv
