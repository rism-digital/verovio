/////////////////////////////////////////////////////////////////////////////
// Name:        editorial.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editorial.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "artic.h"
#include "controlelement.h"
#include "fig.h"
#include "functorparams.h"
#include "layer.h"
#include "measure.h"
#include "scoredef.h"
#include "section.h"
#include "staff.h"
#include "symbol.h"
#include "system.h"
#include "text.h"
#include "textelement.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// EditorialElement
//----------------------------------------------------------------------------

EditorialElement::EditorialElement()
    : Object(EDITORIAL_ELEMENT, "ee-"), SystemMilestoneInterface(), AttLabelled(), AttTyped()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

EditorialElement::EditorialElement(ClassId classId)
    : Object(classId, "ee-"), SystemMilestoneInterface(), AttLabelled(), AttTyped()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

EditorialElement::EditorialElement(ClassId classId, const std::string &classIdStr)
    : Object(classId, classIdStr), SystemMilestoneInterface(), AttLabelled(), AttTyped()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

void EditorialElement::Reset()
{
    Object::Reset();
    SystemMilestoneInterface::Reset();
    this->ResetLabelled();
    this->ResetTyped();

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
    else if (child->Is(SYMBOL)) {
        assert(dynamic_cast<Symbol *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// EditorialElement functor methods
//----------------------------------------------------------------------------

int EditorialElement::Save(FunctorParams *functorParams)
{
    SaveParams *params = vrv_params_cast<SaveParams *>(functorParams);
    assert(params);

    // When writing MEI basic, only visible elements within editorial markup a saved
    if (params->m_basic && this->m_visibility == Hidden) {
        return FUNCTOR_SIBLINGS;
    }
    else {
        return Object::Save(functorParams);
    }
}

int EditorialElement::SaveEnd(FunctorParams *functorParams)
{
    SaveParams *params = vrv_params_cast<SaveParams *>(functorParams);
    assert(params);

    // Same as above
    if (params->m_basic && this->m_visibility == Hidden) {
        return FUNCTOR_SIBLINGS;
    }
    else {
        return Object::SaveEnd(functorParams);
    }
}

int EditorialElement::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    assert(params->m_currentSystem);
    this->MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_CONTINUE;
}

int EditorialElement::ConvertToPageBasedEnd(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    if (m_visibility == Visible) ConvertToPageBasedMilestone(this, params->m_currentSystem);

    return FUNCTOR_CONTINUE;
}

int EditorialElement::PrepareMilestones(FunctorParams *functorParams)
{
    if (this->IsSystemMilestone()) {
        this->SystemMilestoneInterface::InterfacePrepareMilestones(functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int EditorialElement::ResetData(FunctorParams *functorParams)
{
    if (this->IsSystemMilestone()) {
        this->SystemMilestoneInterface::InterfaceResetData(functorParams);
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
    params->m_pendingElements.push_back(editorialElement);

    return FUNCTOR_SIBLINGS;
}

int EditorialElement::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    // Only move editorial elements that are a child of the system
    if (this->GetParent() && this->GetParent()->Is(SYSTEM)) {
        MoveItselfTo(params->m_currentSystem);
    }

    return FUNCTOR_SIBLINGS;
}

int EditorialElement::CastOffToSelection(FunctorParams *functorParams)
{
    CastOffToSelectionParams *params = vrv_params_cast<CastOffToSelectionParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
