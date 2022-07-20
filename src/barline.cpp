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
#include "functorparams.h"
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

BarLine::BarLine() : LayerElement(BARLINE, "bline-"), AttBarLineLog(), AttColor(), AttNNumberLike(), AttVisibility()
{
    this->RegisterAttClass(ATT_BARLINELOG);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
}

BarLine::BarLine(ClassId classId)
    : LayerElement(classId, "bline-"), AttBarLineLog(), AttColor(), AttNNumberLike(), AttVisibility()
{
    this->RegisterAttClass(ATT_BARLINELOG);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
}

BarLine::~BarLine() {}

void BarLine::Reset()
{
    LayerElement::Reset();

    this->ResetBarLineLog();
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

std::pair<bool, double> BarLine::GetLength(const StaffDef *staffDef) const
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

std::pair<bool, data_BARMETHOD> BarLine::GetMethod(const StaffDef *staffDef) const
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

std::pair<bool, int> BarLine::GetPlace(const StaffDef *staffDef) const
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

int BarLine::ConvertToCastOffMensural(FunctorParams *functorParams)
{
    ConvertToCastOffMensuralParams *params = vrv_params_cast<ConvertToCastOffMensuralParams *>(functorParams);
    assert(params);

    assert(m_alignment);
    assert(params->m_targetSubSystem);
    assert(params->m_targetLayer);

    // If this is the last barline of the layer, we will just move it and do not create a new segment
    bool isLast = (params->m_contentLayer->GetLast() == this) ? true : false;
    Object *next = params->m_contentLayer->GetNext(this);
    bool nextIsBarline = (next && next->Is(BARLINE)) ? true : false;

    // See what if we create proper measures and what to do with the barLine
    bool convertToMeasured = params->m_doc->GetOptions()->m_mensuralToMeasure.GetValue();

    if (convertToMeasured) {
        // barLine object will be deleted
        params->m_targetMeasure->SetRight(this->GetForm());
    }
    else {
        this->MoveItselfTo(params->m_targetLayer);
    }

    // Now we can return if this the end barLine
    if (isLast || nextIsBarline) return FUNCTOR_SIBLINGS;

    for (auto const &staffN : params->m_staffNs) {
        // The barline is missing in at least one of the staves - do not break here
        if (!m_alignment->HasAlignmentReference(staffN)) {
            // LogDebug("BarLine not on all staves %d %s", params->m_targetStaff->GetN(), this->GetClassName().c_str());
            return FUNCTOR_SIBLINGS;
        }
    }

    // Make a segment break
    // First case: new need to add a new measure segment (e.g, first pass)
    if (params->m_targetSubSystem->GetChildCount() <= params->m_segmentIdx) {
        params->m_targetMeasure = new Measure(convertToMeasured);
        if (convertToMeasured) {
            params->m_targetMeasure->SetN(StringFormat("%d", params->m_segmentTotal + 1 + params->m_segmentIdx));
        }
        params->m_targetSubSystem->AddChild(params->m_targetMeasure);
        // Add a staff with same attribute as in the previous segment
        params->m_targetStaff = new Staff(*params->m_targetStaff);
        params->m_targetStaff->ClearChildren();
        params->m_targetStaff->CloneReset();
        params->m_targetMeasure->AddChild(params->m_targetStaff);
        // Add a layer also with the same attribute as in the previous segment
        params->m_targetLayer = new Layer(*params->m_targetLayer);
        params->m_targetLayer->ClearChildren();
        params->m_targetLayer->CloneReset();
        params->m_targetStaff->AddChild(params->m_targetLayer);
    }
    // Second case: retrieve the approrpiate segment
    else {
        params->m_targetMeasure = dynamic_cast<Measure *>(params->m_targetSubSystem->GetChild(params->m_segmentIdx));
        // It must be there
        assert(params->m_targetMeasure);

        // Look if we already have the staff (e.g., with more than one layer)
        AttNIntegerComparison comparisonStaffN(STAFF, params->m_targetStaff->GetN());
        Staff *staff = dynamic_cast<Staff *>(params->m_targetMeasure->FindDescendantByComparison(&comparisonStaffN));
        if (!staff) {
            staff = new Staff(*params->m_targetStaff);
            staff->ClearChildren();
            staff->CloneReset();
            params->m_targetMeasure->AddChild(staff);
        }
        params->m_targetStaff = staff;

        // Add a new layer as the new target
        params->m_targetLayer = new Layer(*params->m_targetLayer);
        params->m_targetLayer->ClearChildren();
        params->m_targetLayer->CloneReset();
        params->m_targetStaff->AddChild(params->m_targetLayer);
    }
    params->m_segmentIdx++;
    // LogDebug("BarLine staff %d - Idx %d", params->m_targetStaff->GetN(), params->m_segmentIdx);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
