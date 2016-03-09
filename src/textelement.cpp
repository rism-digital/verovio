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

TextElement::TextElement() :  Object("te-")
{
    Reset();
}

TextElement::TextElement(std::string classid) :  Object(classid)
{
    Reset();
}

TextElement::~TextElement()
{
}

void TextElement::Reset()
{
     Object::Reset();
}

} // namespace vrv
