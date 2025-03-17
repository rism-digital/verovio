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

bool EditorialElement::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ LAYER, MEASURE, SCOREDEF, STAFF, STAFFDEF, STAFFGRP };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else if (Object::IsSystemElement(classId)) {
        return true;
    }
    else if (Object::IsControlElement(classId)) {
        return true;
    }
    else if (Object::IsLayerElement(classId)) {
        return true;
    }
    else if (Object::IsTextElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

//----------------------------------------------------------------------------
// EditorialElement functor methods
//----------------------------------------------------------------------------

FunctorCode EditorialElement::Accept(Functor &functor)
{
    return functor.VisitEditorialElement(this);
}

FunctorCode EditorialElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitEditorialElement(this);
}

FunctorCode EditorialElement::AcceptEnd(Functor &functor)
{
    return functor.VisitEditorialElementEnd(this);
}

FunctorCode EditorialElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitEditorialElementEnd(this);
}

} // namespace vrv
