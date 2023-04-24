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

Annot::Annot() : EditorialElement(ANNOT, "annot-"), TextListInterface(), AttPlist(), AttSource()
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

bool Annot::IsSupportedChild(Object *child)
{
    if (child->IsTextElement()) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->Is(ANNOT)) {
        assert(dynamic_cast<Annot *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
