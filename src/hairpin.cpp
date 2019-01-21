/////////////////////////////////////////////////////////////////////////////
// Name:        hairpin.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "hairpin.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "dynam.h"
#include "functorparams.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Hairpin
//----------------------------------------------------------------------------

Hairpin::Hairpin()
    : ControlElement("hairpin-")
    , TimeSpanningInterface()
    , AttColor()
    , AttHairpinLog()
    , AttPlacement()
    , AttVerticalGroup()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_HAIRPINLOG);
    RegisterAttClass(ATT_PLACEMENT);
    RegisterAttClass(ATT_VERTICALGROUP);

    Reset();
}

Hairpin::~Hairpin() {}

void Hairpin::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    ResetColor();
    ResetHairpinLog();
    ResetPlacement();
    ResetVerticalGroup();

    m_leftLink = NULL;
    m_rightLink = NULL;
    m_drawingLength = 0;
}

int Hairpin::CalcHeight(
    Doc *doc, int staffSize, char spanningType, FloatingPositioner *leftPositioner, FloatingPositioner *rightPositioner)
{
    assert(doc);

    int endY = doc->GetDrawingHairpinSize(staffSize, false);

    // Something is probably wrong before...
    if (!this->GetDrawingLength()) return endY;

    // Do not adjust heigt when not a full hairpin
    if (spanningType != SPANNING_START_END) return endY;

    int length = this->GetDrawingLength();

    // Second of a <>
    if ((this->GetForm() == hairpinLog_FORM_dim) && m_leftLink && m_leftLink->Is(HAIRPIN)) {
        // Do no ajust height when previous hairpin is not a full hairpin
        if (!leftPositioner || (leftPositioner->GetSpanningType() != SPANNING_START_END)) return endY;
        Hairpin *left = dynamic_cast<Hairpin *>(m_leftLink);
        assert(left);
        // Take into account its length only if the left one is actually a <
        if (left->GetForm() == hairpinLog_FORM_cres) {
            ;
            length = std::max(length, left->GetDrawingLength());
        }
    }

    // First of a <>
    if ((this->GetForm() == hairpinLog_FORM_cres) && m_rightLink && m_rightLink->Is(HAIRPIN)) {
        // Do no ajust height when next hairpin is not a full hairpin
        if (!rightPositioner || (rightPositioner->GetSpanningType() != SPANNING_START_END)) return endY;
        Hairpin *right = dynamic_cast<Hairpin *>(m_rightLink);
        assert(right);
        // Take into account its length only if the right one is actually a >
        if (right->GetForm() == hairpinLog_FORM_dim) {
            length = std::max(length, right->GetDrawingLength());
        }
    }

    // Something wrong..
    if (length <= 0) return endY;

    /************** cap the angle of hairpins **************/

    // Given height and width, calculate hairpin angle
    double theta = 2.0 * atan((endY / 2.0) / length);
    // Convert to Radians
    theta *= (360.0 / (2.0 * M_PI));
    // If the angle is too big, restrict endY
    if (theta > 16) {
        theta = 16;
        endY = 2 * (length)*tan((M_PI / 360) * theta);
    }

    return endY;
}

void Hairpin::SetLeftLink(ControlElement *leftLink)
{
    m_leftLink = leftLink;
    if (this->GetDrawingGrpId() != 0) {
        // LogDebug("Grp id LF already set %d", this->GetDrawingGrpId());
        return;
    }

    int grpId = leftLink->GetDrawingGrpId();
    if (grpId == 0) {
        grpId = leftLink->SetDrawingGrpObject(leftLink);
    }
    this->SetDrawingGrpId(grpId);
}

void Hairpin::SetRightLink(ControlElement *rightLink)
{
    m_rightLink = rightLink;
    int grpId = this->GetDrawingGrpId();
    if (grpId == 0) {
        grpId = this->SetDrawingGrpObject(this);
    }

    if (rightLink->GetDrawingGrpId() != 0) {
        // LogDebug("Grp id RL already set %d", rightLink->GetDrawingGrpId());
        return;
    }
    rightLink->SetDrawingGrpId(grpId);
}

//----------------------------------------------------------------------------
// Hairpin functor methods
//----------------------------------------------------------------------------

int Hairpin::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = dynamic_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    if (this->HasVgrp()) {
        this->SetDrawingGrpId(-this->GetVgrp());
    }

    // Only try to link them if start and end are resolved
    if (!this->GetStart() || !this->GetEnd()) return FUNCTOR_CONTINUE;

    for (auto &dynam : params->m_dynams) {
        if ((dynam->GetStart() == this->GetStart()) && (dynam->GetStaff() == this->GetStaff())) {
            if (!this->m_leftLink) this->SetLeftLink(dynam);
        }
        else if ((dynam->GetStart() == this->GetEnd()) && (dynam->GetStaff() == this->GetStaff())) {
            if (!this->m_rightLink) this->SetRightLink(dynam);
        }
    }

    for (auto &hairpin : params->m_hairpins) {
        if ((hairpin->GetEnd() == this->GetStart()) && (hairpin->GetStaff() == this->GetStaff())) {
            if (!this->m_leftLink) this->SetLeftLink(hairpin);
            if (!hairpin->GetRightLink()) hairpin->SetRightLink(this);
        }
        if ((hairpin->GetStart() == this->GetEnd()) && (hairpin->GetStaff() == this->GetStaff())) {
            if (!hairpin->GetLeftLink()) hairpin->SetLeftLink(this);
            if (!this->m_rightLink) this->SetRightLink(hairpin);
        }
    }

    params->m_hairpins.push_back(this);

    return FUNCTOR_CONTINUE;
}

int Hairpin::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetDrawing(functorParams);

    m_leftLink = NULL;
    m_rightLink = NULL;
    m_drawingLength = 0;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
