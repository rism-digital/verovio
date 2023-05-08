/////////////////////////////////////////////////////////////////////////////
// Name:        systemelement.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "systemelement.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "system.h"

namespace vrv {

//----------------------------------------------------------------------------
// SystemElement
//----------------------------------------------------------------------------

SystemElement::SystemElement() : FloatingObject(SYSTEM_ELEMENT, "se"), AttTyped()
{
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

SystemElement::SystemElement(ClassId classId) : FloatingObject(classId, "se"), AttTyped()
{
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

SystemElement::SystemElement(ClassId classId, const std::string &classIdStr)
    : FloatingObject(classId, classIdStr), AttTyped()
{
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

SystemElement::~SystemElement() {}

void SystemElement::Reset()
{
    FloatingObject::Reset();
    this->ResetTyped();

    m_visibility = Visible;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode SystemElement::Accept(Functor &functor)
{
    return functor.VisitSystemElement(this);
}

FunctorCode SystemElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitSystemElement(this);
}

FunctorCode SystemElement::AcceptEnd(Functor &functor)
{
    return functor.VisitSystemElementEnd(this);
}

FunctorCode SystemElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSystemElementEnd(this);
}

} // namespace vrv
