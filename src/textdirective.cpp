/////////////////////////////////////////////////////////////////////////////
// Name:        textdirective.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "textdirective.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "floatingelement.h"
#include "text.h"

namespace vrv {

//----------------------------------------------------------------------------
// AnchoredText
//----------------------------------------------------------------------------

AnchoredText::AnchoredText() : FloatingElement("anchtext-"), TextDirInterface()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());

    Reset();
}

AnchoredText::~AnchoredText()
{
}

void AnchoredText::Reset()
{
    FloatingElement::Reset();
    TextDirInterface::Reset();
}

void AnchoredText::AddTextElement(TextElement *element)
{
    assert(dynamic_cast<TextElement *>(element) || dynamic_cast<EditorialElement *>(element));
    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}

//----------------------------------------------------------------------------
// Tempo
//----------------------------------------------------------------------------

Tempo::Tempo() : FloatingElement("tempo-"), TextDirInterface(), TimePointInterface()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());

    Reset();
}

Tempo::~Tempo()
{
}

void Tempo::Reset()
{
    FloatingElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
}

void Tempo::AddTextElement(TextElement *element)
{
    assert(dynamic_cast<TextElement *>(element) || dynamic_cast<EditorialElement *>(element));
    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}

} // namespace vrv
