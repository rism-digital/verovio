/////////////////////////////////////////////////////////////////////////////
// Name:        fing.h
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fing.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "layerelement.h"
#include "text.h"

namespace vrv {
//----------------------------------------------------------------------------
// Fing
//----------------------------------------------------------------------------

static const ClassRegistrar<Fing> s_factory("fing", FING);

Fing::Fing() : ControlElement(FING, "fing-"), TimePointInterface(), TextDirInterface(), AttNNumberLike()
{
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Fing::~Fing() {}

void Fing::Reset()
{
    ControlElement::Reset();
    TimePointInterface::Reset();
    TextDirInterface::Reset();
    this->ResetNNumberLike();
}

bool Fing::IsSupportedChild(Object *child)
{
    if (child->Is({ LB, REND, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

bool Fing::IsCloserToStaffThan(const FloatingObject *other, data_STAFFREL drawingPlace) const
{
    if (!other->Is(FING)) return false;
    const Fing *otherFing = vrv_cast<const Fing *>(other);

    const LayerElement *element = dynamic_cast<const LayerElement *>(this->GetStart());
    const LayerElement *otherElement = dynamic_cast<const LayerElement *>(otherFing->GetStart());
    if (!element || !otherElement) return false;

    if (drawingPlace == STAFFREL_above) {
        return (element->GetDrawingY() < otherElement->GetDrawingY());
    }
    else if (drawingPlace == STAFFREL_below) {
        return (element->GetDrawingY() > otherElement->GetDrawingY());
    }
    else {
        return false;
    }
}

} // namespace vrv
