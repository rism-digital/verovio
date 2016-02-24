/////////////////////////////////////////////////////////////////////////////
// Name:        dynam.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "dynam.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "text.h"

namespace vrv {

//----------------------------------------------------------------------------
// Dynam
//----------------------------------------------------------------------------

Dynam::Dynam() : FloatingElement("dynam-"), TextDirInterface(), TimeSpanningInterface()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());

    Reset();
}

Dynam::~Dynam()
{
}

void Dynam::Reset()
{
    FloatingElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
}

void Dynam::AddTextElement(TextElement *element)
{
    assert(vrv_cast(TextElement *)(element) || vrv_cast(EditorialElement *)(element));
    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}

} // namespace vrv
