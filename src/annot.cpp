/////////////////////////////////////////////////////////////////////////////
// Name:        annot.cpp
// Author:      Klaus Rettinghaus
// Created:     2018/03/28
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "annot.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Annot
//----------------------------------------------------------------------------

static const ClassRegistrar<Annot> s_factory("annot", ANNOT);

Annot::Annot() : EditorialElement(ANNOT), TextListInterface(), AttPlist(), AttSource()
{
    this->RegisterAttClass(ATT_PLIST);
    this->RegisterAttClass(ATT_SOURCE);

    this->Reset();
}

Annot::~Annot() {}

void Annot::Reset()
{
    EditorialElement::Reset();
    this->ResetPlist();
    this->ResetSource();
}

bool Annot::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ ANNOT };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsTextElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
