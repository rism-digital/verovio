/////////////////////////////////////////////////////////////////////////////
// Name:        section.h
// Author:      Laurent Pugin
// Created:     24/08/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "section.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "div.h"
#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "functor.h"
#include "measure.h"
#include "page.h"
#include "pages.h"
#include "scoredef.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Section
//----------------------------------------------------------------------------

static const ClassRegistrar<Section> s_factory("section", SECTION);

Section::Section() : SystemElement(SECTION, "section-"), SystemMilestoneInterface(), AttNNumberLike(), AttSectionVis()
{
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->RegisterAttClass(ATT_SECTIONVIS);

    this->Reset();
}

Section::~Section() {}

void Section::Reset()
{
    SystemElement::Reset();
    SystemMilestoneInterface::Reset();
    this->ResetNNumberLike();
    this->ResetSectionVis();
}

bool Section::IsSupportedChild(Object *child)
{
    if (child->Is(MEASURE)) {
        assert(dynamic_cast<Measure *>(child));
    }
    else if (child->Is(SCOREDEF)) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else if (child->IsSystemElement()) {
        assert(dynamic_cast<SystemElement *>(child));
    }
    else if (child->Is(DIV)) {
        assert(dynamic_cast<Div *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// Section functor methods
//----------------------------------------------------------------------------

FunctorCode Section::Accept(Functor &functor)
{
    return functor.VisitSection(this);
}

FunctorCode Section::Accept(ConstFunctor &functor) const
{
    return functor.VisitSection(this);
}

FunctorCode Section::AcceptEnd(Functor &functor)
{
    return functor.VisitSectionEnd(this);
}

FunctorCode Section::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSectionEnd(this);
}

} // namespace vrv
