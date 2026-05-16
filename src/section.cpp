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

Section::Section() : SystemElement(SECTION), SystemMilestoneInterface(), AttNNumberLike(), AttSectionVis()
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

bool Section::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ DIV, MEASURE, SCOREDEF };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsSystemElement(classId)) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
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
