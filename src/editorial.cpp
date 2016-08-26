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

#include "floatingelement.h"
#include "functorparams.h"
#include "layer.h"
#include "measure.h"
#include "scoredef.h"
#include "section.h"
#include "staff.h"
#include "system.h"
#include "textelement.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// EditorialElement
//----------------------------------------------------------------------------

EditorialElement::EditorialElement() : Object("ee-"), BoundaryStartInterface(), AttCommon(), AttCommonPart()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_COMMONPART);

    Reset();
}

EditorialElement::EditorialElement(std::string classid) : Object(classid), AttCommon()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_COMMONPART);

    Reset();
}

void EditorialElement::Reset()
{
    Object::Reset();
    BoundaryStartInterface::Reset();
    ResetCommon();
    ResetCommonPart();

    m_visibility = Visible;
}

EditorialElement::~EditorialElement()
{
}

void EditorialElement::AddChild(Object *child)
{
    if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else if (child->IsFloatingElement()) {
        assert(dynamic_cast<FloatingElement *>(child));
    }
    else if (child->IsLayerElement()) {
        assert(dynamic_cast<LayerElement *>(child));
    }
    else if (child->IsTextElement()) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->Is() == LAYER) {
        assert(dynamic_cast<Layer *>(child));
    }
    else if (child->Is() == MEASURE) {
        assert(dynamic_cast<Measure *>(child));
    }
    else if (child->Is() == SECTION) {
        assert(dynamic_cast<Section *>(child));
    }
    else if (child->Is() == SCOREDEF) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else if (child->Is() == STAFF) {
        assert(dynamic_cast<Staff *>(child));
    }
    else if (child->Is() == STAFFDEF) {
        assert(dynamic_cast<Staff *>(child));
    }
    else if (child->Is() == STAFFGRP) {
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

Abbr::~Abbr()
{
}

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

Add::~Add()
{
}

void Add::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Annot
//----------------------------------------------------------------------------

Annot::Annot() : EditorialElement("annot-"), AttPlist(), AttSource()
{
    RegisterAttClass(ATT_PLIST);
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Annot::~Annot()
{
}

void Annot::Reset()
{
    EditorialElement::Reset();
    ResetPlist();
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

App::~App()
{
}

void App::AddChild(Object *child)
{
    if (child->Is() == LEM) {
        assert(dynamic_cast<Lem *>(child));
    }
    else if (child->Is() == RDG) {
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
// Corr
//----------------------------------------------------------------------------

Corr::Corr() : EditorialElement("corr-"), AttSource()
{
    RegisterAttClass(ATT_SOURCE);

    Reset();
}

Corr::~Corr()
{
}

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

Damage::~Damage()
{
}

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

Del::~Del()
{
}

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

Expan::~Expan()
{
}

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

Lem::~Lem()
{
}

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

Orig::~Orig()
{
}

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

Rdg::~Rdg()
{
}

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

Reg::~Reg()
{
}

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

Restore::~Restore()
{
}

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

Sic::~Sic()
{
}

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

Supplied::~Supplied()
{
}

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

Unclear::~Unclear()
{
}

void Unclear::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// EditorialElement functor methods
//----------------------------------------------------------------------------

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
};

int EditorialElement::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = dynamic_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->m_parent));

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

} // namespace vrv
