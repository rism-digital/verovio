/////////////////////////////////////////////////////////////////////////////
// Name:        controlelement.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "controlelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "text.h"

namespace vrv {

//----------------------------------------------------------------------------
// ControlElement
//----------------------------------------------------------------------------

ControlElement::ControlElement() : FloatingObject("me"), AttCommon(), AttTyped()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

ControlElement::ControlElement(std::string classid) : FloatingObject(classid), AttCommon(), AttTyped()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

ControlElement::~ControlElement()
{
}

void ControlElement::Reset()
{
    FloatingObject::Reset();
    ResetCommon();
    ResetTyped();
}
    
char ControlElement::GetAlignment()
{
    Rend *rend = dynamic_cast<Rend*>(this->FindChildByType(REND));
    if (!rend || !rend->HasHalign()) return 0;
    
    switch (rend->GetHalign()) {
        case(HORIZONTALALIGNMENT_center): return CENTER;
        case(HORIZONTALALIGNMENT_right): return RIGHT;
        case(HORIZONTALALIGNMENT_left): return LEFT;
        default: return 0;
    }
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
