/////////////////////////////////////////////////////////////////////////////
// Name:        textelement.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "textelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {
    
//----------------------------------------------------------------------------
// TextElement
//----------------------------------------------------------------------------

TextElement::TextElement():
    DocObject("te-")
{
    Reset();
}

TextElement::TextElement(std::string classid):
    DocObject(classid)
{
    Reset();
}

TextElement::~TextElement()
{
}

void TextElement::Reset()
{
    DocObject::Reset();
}

} // namespace vrv
