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

AnchoredText::AnchoredText() : ControlElement(ANCHOREDTEXT), TextDirInterface()
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

bool AnchoredText::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ LB, REND, TEXT };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
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
