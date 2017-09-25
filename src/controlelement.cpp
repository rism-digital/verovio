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

#include "rend.h"
#include "text.h"

namespace vrv {

//----------------------------------------------------------------------------
// ControlElement
//----------------------------------------------------------------------------

ControlElement::ControlElement() : FloatingObject("me"), AttLabelled(), AttTyped()
{
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

ControlElement::ControlElement(std::string classid) : FloatingObject(classid), AttLabelled(), AttTyped()
{
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

ControlElement::~ControlElement()
{
}

void ControlElement::Reset()
{
    FloatingObject::Reset();
    ResetLabelled();
    ResetTyped();
}

data_HORIZONTALALIGNMENT ControlElement::GetChildRendAlignment()
{
    Rend *rend = dynamic_cast<Rend *>(this->FindChildByType(REND));
    if (!rend || !rend->HasHalign()) return HORIZONTALALIGNMENT_NONE;
    
    return rend->GetHalign();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
