/////////////////////////////////////////////////////////////////////////////
// Name:        barline.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "barline.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "functor.h"
#include "horizontalaligner.h"
#include "layer.h"
#include "measure.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BarLine
//----------------------------------------------------------------------------

static const ClassRegistrar<BarLine> s_factory("barLine", BARLINE);

BarLine::BarLine()
    : LayerElement(BARLINE), AttBarLineLog(), AttBarLineVis(), AttColor(), AttNNumberLike(), AttVisibility()
{
    this->RegisterAttClass(ATT_BARLINELOG);
    this->RegisterAttClass(ATT_BARLINEVIS);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
}

BarLine::BarLine(ClassId classId)
    : LayerElement(classId), AttBarLineLog(), AttBarLineVis(), AttColor(), AttNNumberLike(), AttVisibility()
{
    this->RegisterAttClass(ATT_BARLINELOG);
    this->RegisterAttClass(ATT_BARLINEVIS);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
}

BarLine::~BarLine() {}

void BarLine::Reset()
{
    LayerElement::Reset();

    this->ResetBarLineLog();
    this->ResetBarLineVis();
    this->ResetColor();
    this->ResetVisibility();

    m_position = BarLinePosition::None;
}

bool BarLine::SetAlignment(Alignment *alignment)
{
    m_alignment = alignment;
    return (m_alignment->AddLayerElementRef(this));
}

bool BarLine::HasRepetitionDots() const
{
    if (this->GetForm() == BARRENDITION_rptstart || this->GetForm() == BARRENDITION_rptend
        || this->GetForm() == BARRENDITION_rptboth) {
        return true;
    }
    return false;
}

bool BarLine::IsDrawnThrough(const StaffGrp *staffGrp) const
{
    while (staffGrp) {
        if (staffGrp->HasBarThru()) {
            return (staffGrp->GetBarThru() == BOOLEAN_true);
        }
        staffGrp = dynamic_cast<const StaffGrp *>(staffGrp->GetParent());
    }
    return false;
}

std::pair<bool, double> BarLine::GetLengthFromContext(const StaffDef *staffDef) const
{
    // First check the parent measure
    const Measure *measure = dynamic_cast<const Measure *>(this->GetParent());
    if (measure && measure->HasBarLen()) {
        return { true, measure->GetBarLen() };
    }

    // Then check the staffDef and its ancestors
    const Object *object = staffDef;
    while (object) {
        if (object->HasAttClass(ATT_BARRING)) {
            const AttBarring *att = dynamic_cast<const AttBarring *>(object);
            assert(att);
            if (att->HasBarLen()) {
                return { true, att->GetBarLen() };
            }
        }
        if (object->Is(SCOREDEF)) break;
        object = object->GetParent();
    }

    return { false, 0.0 };
}

std::pair<bool, data_BARMETHOD> BarLine::GetMethodFromContext(const StaffDef *staffDef) const
{
    // First check the parent measure
    const Measure *measure = dynamic_cast<const Measure *>(this->GetParent());
    if (measure && measure->HasBarMethod()) {
        return { true, measure->GetBarMethod() };
    }

    // Then check the staffDef and its ancestors
    const Object *object = staffDef;
    while (object) {
        if (object->HasAttClass(ATT_BARRING)) {
            const AttBarring *att = dynamic_cast<const AttBarring *>(object);
            assert(att);
            if (att->HasBarMethod()) {
                return { true, att->GetBarMethod() };
            }
        }
        if (object->Is(SCOREDEF)) break;
        object = object->GetParent();
    }

    return { false, BARMETHOD_NONE };
}

std::pair<bool, int> BarLine::GetPlaceFromContext(const StaffDef *staffDef) const
{
    // First check the parent measure
    const Measure *measure = dynamic_cast<const Measure *>(this->GetParent());
    if (measure && measure->HasBarPlace()) {
        return { true, measure->GetBarPlace() };
    }

    // Then check the staffDef and its ancestors
    const Object *object = staffDef;
    while (object) {
        if (object->HasAttClass(ATT_BARRING)) {
            const AttBarring *att = dynamic_cast<const AttBarring *>(object);
            assert(att);
            if (att->HasBarPlace()) {
                return { true, att->GetBarPlace() };
            }
        }
        if (object->Is(SCOREDEF)) break;
        object = object->GetParent();
    }

    return { false, 0 };
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode BarLine::Accept(Functor &functor)
{
    return functor.VisitBarLine(this);
}

FunctorCode BarLine::Accept(ConstFunctor &functor) const
{
    return functor.VisitBarLine(this);
}

FunctorCode BarLine::AcceptEnd(Functor &functor)
{
    return functor.VisitBarLineEnd(this);
}

FunctorCode BarLine::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitBarLineEnd(this);
}

} // namespace vrv
