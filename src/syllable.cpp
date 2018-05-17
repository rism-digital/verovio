/////////////////////////////////////////////////////////////////////////////
// Name:        syllable.cpp
// Author:      Andrew Tran
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "syllable.h"
#include "scoredefinterface.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "neume.h"
#include "syl.h"
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

void Syllable::AddChild(Object *child)
{
    if (child->Is(SYL)) {
        assert(dynamic_cast<Syl *>(child));
    }
    else if (child->Is(NEUME)) {
        assert(dynamic_cast<Neume *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

Syllable::~Syllable() {}

void Syllable::Reset()
{
    LayerElement::Reset();
    ResetColor();
    ResetSlashCount();
}

} // namespace vrv
