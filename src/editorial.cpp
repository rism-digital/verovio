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
#include "functor.h"
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

FunctorCode EditorialElement::Accept(MutableFunctor &functor)
{
    return functor.VisitEditorialElement(this);
}

FunctorCode EditorialElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitEditorialElement(this);
}

FunctorCode EditorialElement::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitEditorialElementEnd(this);
}

FunctorCode EditorialElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitEditorialElementEnd(this);
}

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

} // namespace vrv
