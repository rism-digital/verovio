/////////////////////////////////////////////////////////////////////////////
// Name:        anchoredtext.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "anchoredtext.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// AnchoredText
//----------------------------------------------------------------------------

static const ClassRegistrar<AnchoredText> s_factory("anchoredText", ANCHOREDTEXT);

AnchoredText::AnchoredText() : ControlElement(ANCHOREDTEXT, "anchtxt-"), TextDirInterface()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());

    this->Reset();
}

AnchoredText::~AnchoredText() {}

void AnchoredText::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
}

bool AnchoredText::IsSupportedChild(Object *child)
{
    if (child->Is({ LB, REND, TEXT })) {
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

FunctorCode AnchoredText::Accept(Functor &functor)
{
    return functor.VisitAnchoredText(this);
}

FunctorCode AnchoredText::Accept(ConstFunctor &functor) const
{
    return functor.VisitAnchoredText(this);
}

FunctorCode AnchoredText::AcceptEnd(Functor &functor)
{
    return functor.VisitAnchoredTextEnd(this);
}

FunctorCode AnchoredText::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitAnchoredTextEnd(this);
}

} // namespace vrv
