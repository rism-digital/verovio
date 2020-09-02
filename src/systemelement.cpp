/////////////////////////////////////////////////////////////////////////////
// Name:        systemelement.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "systemelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "system.h"

namespace vrv {

//----------------------------------------------------------------------------
// SystemElement
//----------------------------------------------------------------------------

SystemElement::SystemElement() : FloatingObject("se"), AttTyped()
{
    RegisterAttClass(ATT_TYPED);

    Reset();
}

SystemElement::SystemElement(const std::string &classid) : FloatingObject(classid), AttTyped()
{
    RegisterAttClass(ATT_TYPED);

    Reset();
}

SystemElement::~SystemElement() {}

void SystemElement::Reset()
{
    FloatingObject::Reset();
    ResetTyped();

    m_visibility = Visible;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int SystemElement::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    this->MoveItselfTo(params->m_pageBasedSystem);

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

int SystemElement::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->GetParent()));

    // Special case where we use the Relinquish method.
    SystemElement *element = dynamic_cast<SystemElement *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    // move as pending since we want it at the beginning of the system in case of system break coming
    params->m_pendingObjects.push_back(element);

    return FUNCTOR_SIBLINGS;
}

int SystemElement::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
