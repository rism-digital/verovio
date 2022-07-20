/////////////////////////////////////////////////////////////////////////////
// Name:        reh.cpp
// Author:      Klaus Rettinghaus
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "reh.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "measure.h"
#include "text.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Reh
//----------------------------------------------------------------------------

static const ClassRegistrar<Reh> s_factory("reh", REH);

Reh::Reh()
    : ControlElement(REH, "reh-"), TextDirInterface(), TimePointInterface(), AttColor(), AttLang(), AttVerticalGroup()
{
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_LANG);
    this->RegisterAttClass(ATT_VERTICALGROUP);

    this->Reset();
}

Reh::~Reh() {}

void Reh::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimePointInterface::Reset();
    this->ResetColor();
    this->ResetLang();
    this->ResetVerticalGroup();
}

bool Reh::IsSupportedChild(Object *child)
{
    if (child->Is({ LB, REND, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
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
// Reh functor methods
//----------------------------------------------------------------------------

int Reh::PrepareRehPosition(FunctorParams *)
{
    if (!this->HasStart() && !this->HasTstamp()) {
        Measure *measure = vrv_cast<Measure *>(this->GetFirstAncestor(MEASURE));
        if (measure->GetLeftBarLine()) this->SetStart(measure->GetLeftBarLine());
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
