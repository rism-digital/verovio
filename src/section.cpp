/////////////////////////////////////////////////////////////////////////////
// Name:        section.h
// Author:      Laurent Pugin
// Created:     14/07/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "section.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Section
//----------------------------------------------------------------------------

Section::Section() : Object("section-"), BoundaryStartInterface(), AttCommon(), AttCommonPart()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_COMMONPART);

    Reset();
}

Section::~Section()
{
}

void Section::Reset()
{
    Object::Reset();
    BoundaryStartInterface::Reset();
    ResetCommon();
    ResetCommonPart();
}

//----------------------------------------------------------------------------
// Section functor methods
//----------------------------------------------------------------------------

int Section::PrepareBoundaries(FunctorParams *functorParams)
{
    if (this->IsBoundary()) {
        this->BoundaryStartInterface::InterfacePrepareBoundaries(functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int Section::ResetDrawing(FunctorParams *functorParams)
{
    if (this->IsBoundary()) {
        this->BoundaryStartInterface::InterfaceResetDrawing(functorParams);
    }

    return FUNCTOR_CONTINUE;
};

int Section::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = dynamic_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->m_parent));

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    Section *section = dynamic_cast<Section *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    // move as pSection since we want it at the beginning of the system in case of system break coming
    params->m_pendingObjects.push_back(section);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv