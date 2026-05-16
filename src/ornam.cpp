/////////////////////////////////////////////////////////////////////////////
// Name:        ornam.cpp
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ornam.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "layerelement.h"
#include "resources.h"
#include "smufl.h"
#include "symbol.h"
#include "text.h"

namespace vrv {

//----------------------------------------------------------------------------
// Ornam
//----------------------------------------------------------------------------

static const ClassRegistrar<Ornam> s_factory("ornam", ORNAM);

Ornam::Ornam()
    : ControlElement(ORNAM), TextListInterface(), TextDirInterface(), TimePointInterface(), AttOrnamentAccid()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_ORNAMENTACCID);

    this->Reset();
}

Ornam::~Ornam() {}

void Ornam::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
    this->ResetOrnamentAccid();
}

bool Ornam::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ LB, REND, SYMBOL, TEXT };

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

//----------------------------------------------------------------------------
// Ornam functor methods
//----------------------------------------------------------------------------

FunctorCode Ornam::Accept(Functor &functor)
{
    return functor.VisitOrnam(this);
}

FunctorCode Ornam::Accept(ConstFunctor &functor) const
{
    return functor.VisitOrnam(this);
}

FunctorCode Ornam::AcceptEnd(Functor &functor)
{
    return functor.VisitOrnamEnd(this);
}

FunctorCode Ornam::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitOrnamEnd(this);
}

} // namespace vrv
