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
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Rend
//----------------------------------------------------------------------------

Rend::Rend() : TextElement("rend-"), AttColor(), AttCommon(), AttLang(), AttTypography()

{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_LANG);
    RegisterAttClass(ATT_TYPOGRAPHY);

    Reset();
}

Rend::~Rend()
{
}

void Rend::Reset()
{
    TextElement::Reset();
    ResetColor();
    ResetCommon();
    ResetLang();
    ResetTypography();
}

void Rend::AddChild(Object *child)
{
    if (child->Is() == REND) {
        assert(dynamic_cast<Rend *>(child));
    }
    else if (child->Is() == TEXT) {
        assert(dynamic_cast<Text *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// Text
//----------------------------------------------------------------------------

Text::Text() : TextElement("text-")
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
