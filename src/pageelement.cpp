/////////////////////////////////////////////////////////////////////////////
// Name:        pageelement.cpp
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pageelement.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "functorparams.h"
#include "page.h"

namespace vrv {

//----------------------------------------------------------------------------
// PageElement
//----------------------------------------------------------------------------

PageElement::PageElement() : Object(PAGE_ELEMENT, "pe"), AttTyped()
{
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

PageElement::PageElement(ClassId classId) : Object(classId, "pe"), AttTyped()
{
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

PageElement::PageElement(ClassId classId, const std::string &classIdStr) : Object(classId, classIdStr), AttTyped()
{
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

PageElement::~PageElement() {}

void PageElement::Reset()
{
    Object::Reset();
    this->ResetTyped();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode PageElement::Accept(MutableFunctor &functor)
{
    return functor.VisitPageElement(this);
}

FunctorCode PageElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitPageElement(this);
}

FunctorCode PageElement::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitPageElementEnd(this);
}

FunctorCode PageElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPageElementEnd(this);
}

} // namespace vrv
