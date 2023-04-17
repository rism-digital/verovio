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
#include "functorparams.h"
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

FunctorCode SystemElement::Accept(MutableFunctor &functor)
{
    return functor.VisitSystemElement(this);
}

FunctorCode SystemElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitSystemElement(this);
}

FunctorCode SystemElement::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitSystemElementEnd(this);
}

FunctorCode SystemElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSystemElementEnd(this);
}

int SystemElement::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    assert(params->m_currentSystem);
    this->MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_CONTINUE;
}

int SystemElement::ConvertToCastOffMensural(FunctorParams *functorParams)
{
    ConvertToCastOffMensuralParams *params = vrv_params_cast<ConvertToCastOffMensuralParams *>(functorParams);
    assert(params);

    assert(params->m_targetSystem);
    this->MoveItselfTo(params->m_targetSystem);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
