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

EditorialElement::EditorialElement(std::string classid)
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

void EditorialElement::AddChild(Object *child)
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
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// Abbr
//----------------------------------------------------------------------------

Abbr::Abbr() : EditorialElement("abbr-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Abbr::~Abbr() {}

void Abbr::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Add
//----------------------------------------------------------------------------

Add::Add() : EditorialElement("add-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Add::~Add() {}

void Add::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

App::App() : EditorialElement("app-")
{
    m_level = EDITORIAL_UNDEFINED;

    Reset();
}

App::App(EditorialLevel level) : EditorialElement("app-")
{
    m_level = level;

    Reset();
}

void App::Reset()
{
    EditorialElement::Reset();
}

App::~App() {}

void App::AddChild(Object *child)
{
    if (child->Is(LEM)) {
        assert(dynamic_cast<Lem *>(child));
    }
    else if (child->Is(RDG)) {
        assert(dynamic_cast<Rdg *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// Choice
//----------------------------------------------------------------------------

Choice::Choice() : EditorialElement("choice-")
{
    m_level = EDITORIAL_UNDEFINED;

    Reset();
}

Choice::Choice(EditorialLevel level) : EditorialElement("choice-")
{
    m_level = level;

    Reset();
}

void Choice::Reset()
{
    EditorialElement::Reset();
}

Choice::~Choice() {}

void Choice::AddChild(Object *child)
{
    if (child->Is(ABBR)) {
        assert(dynamic_cast<Abbr *>(child));
    }
    else if (child->Is(CHOICE)) {
        assert(dynamic_cast<Choice *>(child));
    }
    else if (child->Is(CORR)) {
        assert(dynamic_cast<Corr *>(child));
    }
    else if (child->Is(EXPAN)) {
        assert(dynamic_cast<Expan *>(child));
    }
    else if (child->Is(ORIG)) {
        assert(dynamic_cast<Orig *>(child));
    }
    else if (child->Is(REG)) {
        assert(dynamic_cast<Reg *>(child));
    }
    else if (child->Is(SIC)) {
        assert(dynamic_cast<Sic *>(child));
    }
    else if (child->Is(UNCLEAR)) {
        assert(dynamic_cast<Unclear *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// Corr
//----------------------------------------------------------------------------

Corr::Corr() : EditorialElement("corr-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Corr::~Corr() {}

void Corr::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Damage
//----------------------------------------------------------------------------

Damage::Damage() : EditorialElement("lem-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Damage::~Damage() {}

void Damage::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Del
//----------------------------------------------------------------------------

Del::Del() : EditorialElement("del-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Del::~Del() {}

void Del::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Expan
//----------------------------------------------------------------------------

Expan::Expan() : EditorialElement("expan-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Expan::~Expan() {}

void Expan::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Lem
//----------------------------------------------------------------------------

Lem::Lem() : EditorialElement("lem-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Lem::~Lem() {}

void Lem::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Orig
//----------------------------------------------------------------------------

Orig::Orig() : EditorialElement("orig-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Orig::~Orig() {}

void Orig::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Rdg
//----------------------------------------------------------------------------

Rdg::Rdg() : EditorialElement("rdg-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Rdg::~Rdg() {}

void Rdg::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Reg
//----------------------------------------------------------------------------

Reg::Reg() : EditorialElement("reg-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Reg::~Reg() {}

void Reg::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Restore
//----------------------------------------------------------------------------

Restore::Restore() : EditorialElement("restore-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Restore::~Restore() {}

void Restore::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Sic
//----------------------------------------------------------------------------

Sic::Sic() : EditorialElement("sic-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Sic::~Sic() {}

void Sic::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Supplied
//----------------------------------------------------------------------------

Supplied::Supplied() : EditorialElement("supplied-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Supplied::~Supplied() {}

void Supplied::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Unclear
//----------------------------------------------------------------------------

Unclear::Unclear() : EditorialElement("unclear-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Unclear::~Unclear() {}

void Unclear::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// EditorialElement functor methods
//----------------------------------------------------------------------------

int EditorialElement::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = dynamic_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    this->MoveItselfTo(params->m_pageBasedSystem);

    return FUNCTOR_CONTINUE;
}

int EditorialElement::ConvertToPageBasedEnd(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = dynamic_cast<ConvertToPageBasedParams *>(functorParams);
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
    CastOffSystemsParams *params = dynamic_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->GetParent()));

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    EditorialElement *editorialElement
        = dynamic_cast<EditorialElement *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    assert(editorialElement);
    // move as pending since we want it at the beginning of the system in case of system break coming
    params->m_pendingObjects.push_back(editorialElement);

    return FUNCTOR_SIBLINGS;
}

int EditorialElement::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = dynamic_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
