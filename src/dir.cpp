/////////////////////////////////////////////////////////////////////////////
// Name:        dir.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "dir.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "aligner.h"
#include "editorial.h"
#include "text.h"

namespace vrv {

//----------------------------------------------------------------------------
// Dir
//----------------------------------------------------------------------------

Dir::Dir() : FloatingElement("dir-"), TextListInterface(), TextDirInterface(), TimeSpanningInterface()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());

    Reset();
}

Dir::~Dir()
{
}

void Dir::Reset()
{
    FloatingElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
}

void Dir::AddTextElement(TextElement *element)
{
    assert(dynamic_cast<TextElement *>(element) || dynamic_cast<EditorialElement *>(element));
    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}

//----------------------------------------------------------------------------
// Dir functor methods
//----------------------------------------------------------------------------

} // namespace vrv
