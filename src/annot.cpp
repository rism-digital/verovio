/////////////////////////////////////////////////////////////////////////////
// Name:        annot.cpp
// Author:      Klaus Rettinghaus
// Created:     2018/03/28
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "annot.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Annot
//----------------------------------------------------------------------------

Annot::Annot() : EditorialElement("annot-"), TextListInterface(), AttPlist(), AttSource()
{
    RegisterAttClass(ATT_PLIST);
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Annot::~Annot() {}

void Annot::Reset()
{
    EditorialElement::Reset();
    ResetPlist();
    ResetSource();
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
