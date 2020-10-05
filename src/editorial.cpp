/////////////////////////////////////////////////////////////////////////////
// Name:        editorial.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editorial.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "controlelement.h"
#include "fig.h"
#include "functorparams.h"
#include "layer.h"
#include "measure.h"
#include "scoredef.h"
#include "section.h"
#include "staff.h"
#include "system.h"
#include "text.h"
#include "textelement.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// EditorialElement
//----------------------------------------------------------------------------

EditorialElement::EditorialElement() : Object("ee-"), BoundaryStartInterface(), AttLabelled(), AttTyped()
{
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

EditorialElement::EditorialElement(const std::string &classid)
    : Object(classid), BoundaryStartInterface(), AttLabelled(), AttTyped()
{
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

void EditorialElement::Reset()
{
    Object::Reset();
    BoundaryStartInterface::Reset();
    ResetLabelled();
    ResetTyped();

    m_visibility = Visible;
}

EditorialElement::~EditorialElement() {}

bool EditorialElement::IsSupportedChild(Object *child)
{
    if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else if (child->IsSystemElement()) {
        assert(dynamic_cast<SystemElement *>(child));
    }
    else if (child->IsControlElement()) {
        assert(dynamic_cast<ControlElement *>(child));
    }
    else if (child->IsLayerElement()) {
        assert(dynamic_cast<LayerElement *>(child));
    }
    else if (child->IsTextElement()) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->Is(LAYER)) {
        assert(dynamic_cast<Layer *>(child));
    }
    else if (child->Is(MEASURE)) {
        assert(dynamic_cast<Measure *>(child));
    }
    else if (child->Is(SCOREDEF)) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else if (child->Is(STAFF)) {
        assert(dynamic_cast<Staff *>(child));
    }
    else if (child->Is(STAFFDEF)) {
        assert(dynamic_cast<Staff *>(child));
    }
    else if (child->Is(STAFFGRP)) {
        assert(dynamic_cast<Staff *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// EditorialElement functor methods
//----------------------------------------------------------------------------

int EditorialElement::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    this->MoveItselfTo(params->m_pageBasedSystem);

    return FUNCTOR_CONTINUE;
}

int EditorialElement::ConvertToPageBasedEnd(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    if (this->m_visibility == Visible) ConvertToPageBasedBoundary(this, params->m_pageBasedSystem);

    return FUNCTOR_CONTINUE;
}

int EditorialElement::PrepareBoundaries(FunctorParams *functorParams)
{
    if (this->IsBoundary()) {
        this->BoundaryStartInterface::InterfacePrepareBoundaries(functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int EditorialElement::ResetDrawing(FunctorParams *functorParams)
{
    if (this->IsBoundary()) {
        this->BoundaryStartInterface::InterfaceResetDrawing(functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int EditorialElement::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->GetParent()));

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    EditorialElement *editorialElement
        = vrv_cast<EditorialElement *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    assert(editorialElement);
    // move as pending since we want it at the beginning of the system in case of system break coming
    params->m_pendingObjects.push_back(editorialElement);

    return FUNCTOR_SIBLINGS;
}

int EditorialElement::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
