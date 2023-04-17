/////////////////////////////////////////////////////////////////////////////
// Name:        controlelement.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "controlelement.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "functorparams.h"
#include "layer.h"
#include "rend.h"
#include "system.h"
#include "text.h"
#include "timeinterface.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// ControlElement
//----------------------------------------------------------------------------

ControlElement::ControlElement()
    : FloatingObject(CONTROL_ELEMENT, "ce"), AltSymInterface(), LinkingInterface(), AttLabelled(), AttTyped()
{
    this->RegisterInterface(AltSymInterface::GetAttClasses(), AltSymInterface::IsInterface());
    this->RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

ControlElement::ControlElement(ClassId classId)
    : FloatingObject(classId, "ce"), AltSymInterface(), LinkingInterface(), AttLabelled(), AttTyped()
{
    this->RegisterInterface(AltSymInterface::GetAttClasses(), AltSymInterface::IsInterface());
    this->RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

ControlElement::ControlElement(ClassId classId, const std::string &classIdStr)
    : FloatingObject(classId, classIdStr), AltSymInterface(), LinkingInterface(), AttLabelled(), AttTyped()
{
    this->RegisterInterface(AltSymInterface::GetAttClasses(), AltSymInterface::IsInterface());
    this->RegisterInterface(LinkingInterface::GetAttClasses(), LinkingInterface::IsInterface());
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

ControlElement::~ControlElement() {}

void ControlElement::Reset()
{
    FloatingObject::Reset();
    AltSymInterface::Reset();
    LinkingInterface::Reset();
    this->ResetLabelled();
    this->ResetTyped();
}

data_HORIZONTALALIGNMENT ControlElement::GetChildRendAlignment() const
{
    const Rend *rend = vrv_cast<const Rend *>(this->FindDescendantByType(REND));
    if (!rend || !rend->HasHalign()) return HORIZONTALALIGNMENT_NONE;

    return rend->GetHalign();
}

data_STAFFREL ControlElement::GetLayerPlace(data_STAFFREL defaultValue) const
{
    // Do this only for the following elements
    if (!this->Is({ TRILL, MORDENT, ORNAM, TURN })) return defaultValue;

    const TimePointInterface *interface = this->GetTimePointInterface();
    assert(interface);

    const LayerElement *start = interface->GetStart();
    if (!start || start->Is(TIMESTAMP_ATTR)) return defaultValue;

    const Layer *layer = vrv_cast<const Layer *>(start->GetFirstAncestor(LAYER));
    // We are only looking that the element cross-staff. We could use LayerElement::GetCrossStaff(Layer  *&)
    if (start->m_crossLayer) layer = start->m_crossLayer;
    assert(layer);

    data_STAFFREL value = defaultValue;

    data_STEMDIRECTION stemDir = layer->GetDrawingStemDir(start);
    switch (stemDir) {
        case (STEMDIRECTION_up): value = STAFFREL_above; break;
        case (STEMDIRECTION_down): value = STAFFREL_below; break;
        default: break;
    }

    return value;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode ControlElement::Accept(MutableFunctor &functor)
{
    return functor.VisitControlElement(this);
}

FunctorCode ControlElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitControlElement(this);
}

FunctorCode ControlElement::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitControlElementEnd(this);
}

FunctorCode ControlElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitControlElementEnd(this);
}

} // namespace vrv
