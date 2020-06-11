/////////////////////////////////////////////////////////////////////////////
// Name:        anchoredtext.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "anchoredtext.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "controlelement.h"
#include "editorial.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// AnchoredText
//----------------------------------------------------------------------------

AnchoredText::AnchoredText() : ControlElement("anchtxt-"), TextDirInterface()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());

    Reset();
}

AnchoredText::~AnchoredText() {}

void AnchoredText::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
}

bool AnchoredText::IsSupportedChild(Object *child)
{
    if (child->Is(TEXT)) {
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

} // namespace vrv
