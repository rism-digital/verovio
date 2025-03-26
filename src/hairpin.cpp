/////////////////////////////////////////////////////////////////////////////
// Name:        hairpin.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "hairpin.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "devicecontext.h"
#include "doc.h"
#include "dynam.h"
#include "functor.h"
#include "measure.h"
#include "system.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Hairpin
//----------------------------------------------------------------------------

static const ClassRegistrar<Hairpin> s_factory("hairpin", HAIRPIN);

Hairpin::Hairpin()
    : ControlElement(HAIRPIN)
    , TimeSpanningInterface()
    , AttHairpinLog()
    , AttHairpinVis()
    , AttPlacementRelStaff()
    , AttVerticalGroup()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_HAIRPINLOG);
    this->RegisterAttClass(ATT_HAIRPINVIS);
    this->RegisterAttClass(ATT_LINERENDBASE);
    this->RegisterAttClass(ATT_PLACEMENTRELSTAFF);
    this->RegisterAttClass(ATT_VERTICALGROUP);

    this->Reset();
}

Hairpin::~Hairpin() {}

void Hairpin::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetHairpinLog();
    this->ResetHairpinVis();
    this->ResetLineRendBase();
    this->ResetPlacementRelStaff();
    this->ResetVerticalGroup();

    m_leftLink = NULL;
    m_rightLink = NULL;
    m_drawingLength = 0;
}

int Hairpin::CalcHeight(const Doc *doc, int staffSize, char spanningType, const FloatingPositioner *leftPositioner,
    const FloatingPositioner *rightPositioner) const
{
    assert(doc);

    int endY = doc->GetDrawingHairpinSize(staffSize, false);

    if (this->HasOpening()) {
        if (this->GetOpening().GetType() == MEASUREMENTTYPE_px) {
            endY = this->GetOpening().GetPx();
        }
        else {
            endY = this->GetOpening().GetVu() * doc->GetDrawingUnit(staffSize);
        }
    }

    // Something is probably wrong before...
    if (!this->GetDrawingLength()) return endY;

    // Do not adjust height when not a full hairpin
    if (spanningType != SPANNING_START_END) return endY;

    int length = this->GetDrawingLength();

    // Second of a <>
    if ((this->GetForm() == hairpinLog_FORM_dim) && m_leftLink && m_leftLink->Is(HAIRPIN)) {
        // Don't adjust height when previous hairpin is not a full hairpin
        if (!leftPositioner || (leftPositioner->GetSpanningType() != SPANNING_START_END)) return endY;
        const Hairpin *left = vrv_cast<const Hairpin *>(m_leftLink);
        assert(left);
        // Take into account its length only if the left one is actually a <
        if (left->GetForm() == hairpinLog_FORM_cres) {
            length = std::max(length, left->GetDrawingLength());
        }
    }

    // First of a <>
    if ((this->GetForm() == hairpinLog_FORM_cres) && m_rightLink && m_rightLink->Is(HAIRPIN)) {
        // Don't adjust height when next hairpin is not a full hairpin
        if (!rightPositioner || (rightPositioner->GetSpanningType() != SPANNING_START_END)) return endY;
        const Hairpin *right = vrv_cast<const Hairpin *>(m_rightLink);
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
    if (!leftLink) return;

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
    if (!rightLink) return;

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

std::pair<int, int> Hairpin::GetBarlineOverlapAdjustment(int doubleUnit, int leftX, int rightX, int spanningType) const
{
    const Measure *startMeasure = vrv_cast<const Measure *>(this->GetStart()->GetFirstAncestor(MEASURE));
    const Measure *endMeasure = vrv_cast<const Measure *>(this->GetEnd()->GetFirstAncestor(MEASURE));

    if (!startMeasure || !endMeasure) return { 0, 0 };

    // Calculate adjustment that needs to be made for hairpin not to touch the left barline. We take doubleUnit for the
    // default margin to consider them overlapping, which is adjusted in case we have wider barline on the left
    int leftAdjustment = 0;
    const BarLine *leftBarline = startMeasure->GetLeftBarLine();
    if (leftBarline && (spanningType == SPANNING_START_END || spanningType == SPANNING_START)) {
        int margin = doubleUnit;
        const int leftBarlineX = leftBarline->GetDrawingX();
        const int diff = leftX - leftBarlineX;
        if (leftBarline->GetForm() == BARRENDITION_rptstart) margin *= 1.5;
        if (diff < margin) leftAdjustment = margin - diff;
    }

    // Similar calculation is done for the right barline, with it having two barline forms that we need to consider
    // as opposed to only one for the left barline. Additionally, when we have spanning hairpins, correct barline should
    // be selected - when processing start of the spanning hairpin, we should check for the last measure of the current
    // system, instead of the endMeasure
    int rightAdjustment = 0;
    const BarLine *rightBarline = NULL;
    if (spanningType == SPANNING_START_END || spanningType == SPANNING_END) {
        rightBarline = endMeasure->GetRightBarLine();
    }
    else if (spanningType == SPANNING_START) {
        const System *startSystem = vrv_cast<const System *>(this->GetStart()->GetFirstAncestor(SYSTEM));
        if (startSystem) {
            ClassIdComparison cmp(MEASURE);
            const Measure *measure
                = vrv_cast<const Measure *>(startSystem->FindDescendantByComparison(&cmp, UNLIMITED_DEPTH, BACKWARD));
            if (measure) rightBarline = measure->GetRightBarLine();
        }
    }
    if (rightBarline) {
        int margin = doubleUnit;
        const int rightBarlineX = rightBarline->GetDrawingX();
        const int diff = rightBarlineX - rightX;
        if ((rightBarline->GetForm() == BARRENDITION_rptend) || (rightBarline->GetForm() == BARRENDITION_end)) {
            margin *= 1.5;
        }
        if (diff < margin) rightAdjustment = margin - diff;
    }

    return { leftAdjustment, rightAdjustment };
}

//----------------------------------------------------------------------------
// Hairpin functor methods
//----------------------------------------------------------------------------

FunctorCode Hairpin::Accept(Functor &functor)
{
    return functor.VisitHairpin(this);
}

FunctorCode Hairpin::Accept(ConstFunctor &functor) const
{
    return functor.VisitHairpin(this);
}

FunctorCode Hairpin::AcceptEnd(Functor &functor)
{
    return functor.VisitHairpinEnd(this);
}

FunctorCode Hairpin::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitHairpinEnd(this);
}

} // namespace vrv
