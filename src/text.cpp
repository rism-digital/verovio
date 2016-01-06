/////////////////////////////////////////////////////////////////////////////
// Name:        text.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "text.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"

namespace vrv {
    
//----------------------------------------------------------------------------
// Rend
//----------------------------------------------------------------------------

Rend::Rend( ):
    TextElement("rend-"),
    AttCommon(),
    AttTypography()
    
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_TYPOGRAPHY);
    
    Reset();
}

Rend::~Rend()
{
}

void Rend::Reset()
{
    TextElement::Reset();
    ResetCommon();
    ResetTypography();
}
    
void Rend::AddTextElement(TextElement *element)
{
    assert(dynamic_cast<Rend*>(element)
           || dynamic_cast<Text*>(element)
           || dynamic_cast<EditorialElement*>(element) );
    element->SetParent( this );
    m_children.push_back(element);
    Modify();
}
    
//----------------------------------------------------------------------------
// Text
//----------------------------------------------------------------------------
    
Text::Text( ):
    TextElement("text-")
{
    Reset();
}

Text::~Text()
{
}

void Text::Reset()
{
    TextElement::Reset();
}
    
} // namespace vrv
