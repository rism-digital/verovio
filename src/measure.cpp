/////////////////////////////////////////////////////////////////////////////
// Name:        measure.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "measure.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "attcomparison.h"
#include "boundary.h"
#include "controlelement.h"
#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "functorparams.h"
#include "page.h"
#include "staff.h"
#include "syl.h"
#include "system.h"
#include "tempo.h"
#include "timeinterface.h"
#include "timestamp.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Measure
//----------------------------------------------------------------------------

Measure::Measure(bool measureMusic, int logMeasureNb)
    : Object("measure-"), AttMeasureLog(), AttNNumberLike(), AttPointing(), AttTyped()
{
    RegisterAttClass(ATT_MEASURELOG);
    RegisterAttClass(ATT_NNUMBERLIKE);
    RegisterAttClass(ATT_POINTING);
    RegisterAttClass(ATT_TYPED);

    m_measuredMusic = measureMusic;
    // We set parent to it because we want to access the parent doc from the aligners
    m_measureAligner.SetParent(this);
    // Idem for timestamps
    m_timestampAligner.SetParent(this);
    // Idem for barlines
    m_leftBarLine.SetParent(this);
    m_rightBarLine.SetParent(this);

    // owned pointers need to be set to NULL;
    m_drawingScoreDef = NULL;

    // Make the left barLine a left one...
    m_leftBarLine.SetLeft();

    Reset();

    if (!measureMusic) this->SetRight(BARRENDITION_invis);
}

Measure::~Measure()
{
    // We need to delete own objects
    Reset();
}

void Measure::Reset()
{
    Object::Reset();
    ResetMeasureLog();
    ResetNNumberLike();
    ResetPointing();
    ResetTyped();

    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    m_timestampAligner.Reset();
    m_xAbs = VRV_UNSET;
    m_xAbs2 = VRV_UNSET;
    m_drawingXRel = 0;

    // by default, we have a single barLine on the right (none on the left)
    m_rightBarLine.SetForm(this->GetRight());
    m_leftBarLine.SetForm(this->GetLeft());

    if (!m_measuredMusic) {
        m_xAbs = VRV_UNSET;
        m_xAbs2 = VRV_UNSET;
    }

    m_drawingEnding = NULL;
    m_hasAlignmentRefWithMultipleLayers = false;

    m_scoreTimeOffset.clear();
    m_realTimeOffsetMilliseconds.clear();
    m_currentTempo = 120;
}

void Measure::AddChild(Object *child)
{
    if (child->IsControlElement()) {
        assert(dynamic_cast<ControlElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else if (child->Is(STAFF)) {
        Staff *staff = dynamic_cast<Staff *>(child);
        assert(staff);
        if (staff && (staff->GetN() < 1)) {
            // This is not 100% safe if we have a <app> and <rdg> with more than
            // one staff as a previous child.
            staff->SetN(this->GetChildCount());
        }
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

int Measure::GetDrawingX() const
{
    if (m_xAbs != VRV_UNSET) return m_xAbs;

    if (m_cachedDrawingX != VRV_UNSET) return m_cachedDrawingX;

    System *system = dynamic_cast<System *>(this->GetFirstParent(SYSTEM));
    assert(system);
    m_cachedDrawingX = system->GetDrawingX() + this->GetDrawingXRel();
    return m_cachedDrawingX;
}

void Measure::SetDrawingXRel(int drawingXRel)
{
    ResetCachedDrawingX();
    m_drawingXRel = drawingXRel;
}

int Measure::GetLeftBarLineXRel() const
{
    if (m_measureAligner.GetLeftBarLineAlignment()) {
        return m_measureAligner.GetLeftBarLineAlignment()->GetXRel();
    }
    return 0;
}

int Measure::GetLeftBarLineLeft() const
{
    int x = GetLeftBarLineXRel();
    if (m_leftBarLine.HasUpdatedBB()) {
        x += m_leftBarLine.GetContentX1();
    }
    return x;
}

int Measure::GetLeftBarLineRight() const
{
    int x = GetLeftBarLineXRel();
    if (m_leftBarLine.HasUpdatedBB()) {
        x += m_leftBarLine.GetContentX2();
    }
    return x;
}

int Measure::GetRightBarLineXRel() const
{
    if (m_measureAligner.GetRightBarLineAlignment()) {
        return m_measureAligner.GetRightBarLineAlignment()->GetXRel();
    }
    return 0;
}

int Measure::GetRightBarLineLeft() const
{
    int x = GetRightBarLineXRel();
    if (m_rightBarLine.HasUpdatedBB()) {
        x += m_rightBarLine.GetContentX1();
    }
    return x;
}

int Measure::GetRightBarLineRight() const
{
    int x = GetRightBarLineXRel();
    if (m_rightBarLine.HasUpdatedBB()) {
        x += m_rightBarLine.GetContentX2();
    }
    return x;
}

int Measure::GetWidth() const
{
    if (this->m_xAbs2 != VRV_UNSET) return (m_xAbs2 - m_xAbs);

    assert(m_measureAligner.GetRightAlignment());
    return m_measureAligner.GetRightAlignment()->GetXRel();
}

int Measure::GetInnerWidth() const
{
    return (this->GetRightBarLineLeft() - this->GetLeftBarLineRight());
}

int Measure::GetInnerCenterX() const
{
    return (this->GetDrawingX() + this->GetLeftBarLineRight() + this->GetInnerWidth() / 2);
}

void Measure::SetDrawingScoreDef(ScoreDef *drawingScoreDef)
{
    assert(!m_drawingScoreDef); // We should always call UnsetCurrentScoreDef before

    m_drawingScoreDef = new ScoreDef();
    *m_drawingScoreDef = *drawingScoreDef;
}

std::vector<Staff *> Measure::GetFirstStaffGrpStaves(ScoreDef *scoreDef)
{
    assert(scoreDef);

    std::vector<Staff *> staves;
    std::vector<int>::iterator iter;
    std::vector<int> staffList;

    // First get all the staffGrps
    AttComparison matchType(STAFFGRP);
    ArrayOfObjects staffGrps;
    ArrayOfObjects::iterator staffGrpIter;
    scoreDef->FindAllChildByAttComparison(&staffGrps, &matchType);

    // Then the @n of each first staffDef
    for (staffGrpIter = staffGrps.begin(); staffGrpIter != staffGrps.end(); staffGrpIter++) {
        StaffDef *staffDef = dynamic_cast<StaffDef *>((*staffGrpIter)->GetFirst(STAFFDEF));
        if (staffDef) staffList.push_back(staffDef->GetN());
    }

    // Get the corresponding staves in the measure
    for (iter = staffList.begin(); iter != staffList.end(); iter++) {
        AttNIntegerComparison matchN(STAFF, *iter);
        Staff *staff = dynamic_cast<Staff *>(this->FindChildByAttComparison(&matchN, 1));
        if (!staff) {
            // LogDebug("Staff with @n '%d' not found in measure '%s'", *iter, measure->GetUuid().c_str());
            continue;
        }
        staves.push_back(staff);
    }
    if (staves.empty()) LogDebug("Empty @staff array");
    return staves;
}

int Measure::EnclosesTime(int time) const
{
    int repeat = 1;
    int timeDuration = int(
        m_measureAligner.GetRightAlignment()->GetTime() * DURATION_4 / DUR_MAX * 60.0 / m_currentTempo * 1000.0 + 0.5);
    std::vector<int>::const_iterator iter;
    for (iter = m_realTimeOffsetMilliseconds.begin(); iter != m_realTimeOffsetMilliseconds.end(); iter++) {
        if ((time >= *iter) && (time <= *iter + timeDuration)) return repeat;
        repeat++;
    }
    return 0;
}

int Measure::GetRealTimeOffsetMilliseconds(int repeat) const
{
    if ((repeat < 1) || repeat > (int)m_realTimeOffsetMilliseconds.size()) return 0;
    return m_realTimeOffsetMilliseconds.at(repeat - 1);
}

//----------------------------------------------------------------------------
// Measure functor methods
//----------------------------------------------------------------------------

int Measure::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = dynamic_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    // Move itself to the pageBasedSystem - do not process children
    this->MoveItselfTo(params->m_pageBasedSystem);

    return FUNCTOR_SIBLINGS;
}

int Measure::Save(FunctorParams *functorParams)
{
    if (this->IsMeasuredMusic())
        return Object::Save(functorParams);
    else
        return FUNCTOR_CONTINUE;
}

int Measure::SaveEnd(FunctorParams *functorParams)
{
    if (this->IsMeasuredMusic())
        return Object::SaveEnd(functorParams);
    else
        return FUNCTOR_CONTINUE;
}

int Measure::UnsetCurrentScoreDef(FunctorParams *functorParams)
{
    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    return FUNCTOR_CONTINUE;
};

void Measure::SetDrawingBarLines(Measure *previous, bool systemBreak, bool scoreDefInsert)
{
    // First set the right barline. If none then set a single one.
    data_BARRENDITION rightBarline = (this->HasRight()) ? this->GetRight() : BARRENDITION_single;
    this->SetDrawingRightBarLine(rightBarline);

    // Now adjust the right barline of the previous measure (if any) and the left one
    if (!previous) {
        this->SetDrawingLeftBarLine(this->GetLeft());
    }
    else if (systemBreak) {
        // we have rptboth on one of the two sides, split them (ignore any other value)
        if ((previous->GetRight() == BARRENDITION_rptboth) || (this->GetLeft() == BARRENDITION_rptboth)) {
            previous->SetDrawingRightBarLine(BARRENDITION_rptend);
            this->SetDrawingLeftBarLine(BARRENDITION_rptstart);
        }
        // nothing to do with any other value?
        else {
            this->SetDrawingLeftBarLine(this->GetLeft());
        }
    }
    else if (!scoreDefInsert) {
        // we have rptboth split in the two measures, make them one rptboth
        if ((previous->GetRight() == BARRENDITION_rptend) && (this->GetLeft() == BARRENDITION_rptstart)) {
            previous->SetDrawingRightBarLine(BARRENDITION_rptboth);
            this->SetDrawingLeftBarLine(BARRENDITION_NONE);
        }
        // we have an rptend before, make sure there in none on the left (ignore any other value)
        else if (previous->GetRight() == BARRENDITION_rptend) {
            this->SetDrawingLeftBarLine(BARRENDITION_NONE);
        }
        // we have an rptstart coming, make sure there is none on the right before (ignore any other value)
        else if (this->GetLeft() == BARRENDITION_rptstart) {
            // always set the right barline to invis for spacing
            previous->SetDrawingRightBarLine(BARRENDITION_invis);
            this->SetDrawingLeftBarLine(BARRENDITION_rptstart);
        }
        // we have an rptboth coming, make sure there is none on the right before (ignore any other value)
        else if (this->GetLeft() == BARRENDITION_rptboth) {
            // always set the right barline to invis for spacing
            previous->SetDrawingRightBarLine(BARRENDITION_invis);
            this->SetDrawingLeftBarLine(BARRENDITION_rptboth);
        }
        // nothing we can do with any other value?
        else {
            this->SetDrawingLeftBarLine(this->GetLeft());
        }
    }
    else {
        // with a scoredef inbetween always set it to what we have in the encoding
        this->SetDrawingLeftBarLine(this->GetLeft());
    }
}

int Measure::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    SetDrawingXRel(0);
    if (m_measureAligner.GetLeftAlignment()) {
        m_measureAligner.GetLeftAlignment()->SetXRel(0);
    }
    if (m_measureAligner.GetRightAlignment()) {
        m_measureAligner.GetRightAlignment()->SetXRel(0);
    }

    Functor resetHorizontalAlignment(&Object::ResetHorizontalAlignment);
    m_timestampAligner.Process(&resetHorizontalAlignment, NULL);

    m_hasAlignmentRefWithMultipleLayers = false;

    return FUNCTOR_CONTINUE;
}

int Measure::ApplyPPUFactor(FunctorParams *functorParams)
{
    ApplyPPUFactorParams *params = dynamic_cast<ApplyPPUFactorParams *>(functorParams);
    assert(params);

    if (m_xAbs != VRV_UNSET) m_xAbs /= params->m_page->GetPPUFactor();
    if (m_xAbs2 != VRV_UNSET) m_xAbs2 /= params->m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

int Measure::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = dynamic_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    // clear the content of the measureAligner
    m_measureAligner.Reset();

    // point to it
    params->m_measureAligner = &m_measureAligner;
    params->m_hasMultipleLayer = false;

    if (m_leftBarLine.SetAlignment(m_measureAligner.GetLeftBarLineAlignment())) params->m_hasMultipleLayer = true;
    if (m_rightBarLine.SetAlignment(m_measureAligner.GetRightBarLineAlignment())) params->m_hasMultipleLayer = true;

    assert(params->m_measureAligner);

    return FUNCTOR_CONTINUE;
}

int Measure::AlignHorizontallyEnd(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = dynamic_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    // We also need to align the timestamps - we do it at the end since we need the *meterSig to be initialized by a
    // Layer. Obviously this will not work with different time signature. However, I am not sure how this would work
    // in
    // MEI anyway.
    m_timestampAligner.Process(params->m_functor, params);

    // Next scoreDef will be INTERMEDIATE_SCOREDEF (See Layer::AlignHorizontally)
    params->m_isFirstMeasure = false;

    if (params->m_hasMultipleLayer) m_hasAlignmentRefWithMultipleLayers = true;

    return FUNCTOR_CONTINUE;
}

int Measure::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = dynamic_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    // we also need to reset the staffNb
    params->m_staffIdx = 0;

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustLayers(FunctorParams *functorParams)
{
    AdjustLayersParams *params = dynamic_cast<AdjustLayersParams *>(functorParams);
    assert(params);

    if (!m_hasAlignmentRefWithMultipleLayers) return FUNCTOR_SIBLINGS;

    std::vector<int>::iterator iter;
    std::vector<AttComparison *> filters;
    for (iter = params->m_staffNs.begin(); iter != params->m_staffNs.end(); iter++) {
        filters.clear();
        // Create ad comparison object for each type / @n
        std::vector<int> ns;
        // -1 for barline attributes that need to be taken into account each time
        ns.push_back(-1);
        ns.push_back(*iter);
        AttNIntegerComparisonAny matchStaff(ALIGNMENT_REFERENCE, ns);
        filters.push_back(&matchStaff);

        m_measureAligner.Process(params->m_functor, params, NULL, &filters);
    }

    return FUNCTOR_SIBLINGS;
}

int Measure::AdjustAccidX(FunctorParams *functorParams)
{
    AdjustAccidXParams *params = dynamic_cast<AdjustAccidXParams *>(functorParams);
    assert(params);

    params->m_currentMeasure = this;

    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustGraceXPos(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = dynamic_cast<AdjustGraceXPosParams *>(functorParams);
    assert(params);

    m_measureAligner.PushAlignmentsRight();

    params->m_rightDefaultAlignment = NULL;

    // We process it backward because we want to get the rightDefaultAlignment
    m_measureAligner.Process(params->m_functor, params, params->m_functorEnd, NULL, UNLIMITED_DEPTH, BACKWARD);

    return FUNCTOR_SIBLINGS;
}

int Measure::AdjustXPos(FunctorParams *functorParams)
{
    AdjustXPosParams *params = dynamic_cast<AdjustXPosParams *>(functorParams);
    assert(params);

    params->m_minPos = 0;
    params->m_upcomingMinPos = VRV_UNSET;
    params->m_cumulatedXShift = 0;

    std::vector<int>::iterator iter;
    std::vector<AttComparison *> filters;
    for (iter = params->m_staffNs.begin(); iter != params->m_staffNs.end(); iter++) {
        params->m_minPos = 0;
        params->m_upcomingMinPos = VRV_UNSET;
        params->m_cumulatedXShift = 0;
        params->m_staffN = (*iter);
        filters.clear();
        // Create ad comparison object for each type / @n
        std::vector<int> ns;
        // -1 for barline attributes that need to be taken into account each time
        ns.push_back(-1);
        ns.push_back(*iter);
        AttNIntegerComparisonAny matchStaff(ALIGNMENT_REFERENCE, ns);
        filters.push_back(&matchStaff);

        m_measureAligner.Process(params->m_functor, params, params->m_functorEnd, &filters);
    }

    // m_measureAligner.Process(params->m_functor, params, params->m_functorEnd);

    int minMeasureWidth = params->m_doc->m_drawingMinMeasureWidth;
    // First try to see if we have a double measure length element
    MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_FULLMEASURE2);
    Alignment *fullMeasure2
        = dynamic_cast<Alignment *>(m_measureAligner.FindChildByAttComparison(&alignmentComparison, 1));
    if (fullMeasure2 != NULL) minMeasureWidth *= 2;

    int currentMeasureWidth = this->GetRightBarLineLeft() - this->GetLeftBarLineRight();
    if (currentMeasureWidth < minMeasureWidth) {
        ArrayOfAdjustmentTuples boundaries{ std::make_tuple(this->GetLeftBarLine()->GetAlignment(),
            this->GetRightBarLine()->GetAlignment(), minMeasureWidth - currentMeasureWidth) };
        m_measureAligner.AdjustProportionally(boundaries);
    }

    return FUNCTOR_SIBLINGS;
}

int Measure::AdjustSylSpacingEnd(FunctorParams *functorParams)
{
    AdjustSylSpacingParams *params = dynamic_cast<AdjustSylSpacingParams *>(functorParams);
    assert(params);

    // Here we also need to handle the last syl or the measure - we check the alignment with the right barline
    if (params->m_previousSyl) {
        int overlap = params->m_previousSyl->GetSelfRight() - this->GetRightBarLine()->GetAlignment()->GetXRel();
        if (overlap > 0) {
            params->m_overlapingSyl.push_back(std::make_tuple(
                params->m_previousSyl->GetAlignment(), this->GetRightBarLine()->GetAlignment(), overlap));
        }
    }

    // Ajust the postion of the alignment according to what we have collected for this verse
    m_measureAligner.AdjustProportionally(params->m_overlapingSyl);
    params->m_overlapingSyl.clear();

    return FUNCTOR_CONTINUE;
}

int Measure::SetAlignmentXPos(FunctorParams *functorParams)
{
    SetAlignmentXPosParams *params = dynamic_cast<SetAlignmentXPosParams *>(functorParams);
    assert(params);

    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int Measure::JustifyX(FunctorParams *functorParams)
{
    JustifyXParams *params = dynamic_cast<JustifyXParams *>(functorParams);
    assert(params);

    if (params->m_measureXRel > 0) {
        SetDrawingXRel(params->m_measureXRel);
    }
    else {
        params->m_measureXRel = GetDrawingXRel();
    }

    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int Measure::AlignMeasures(FunctorParams *functorParams)
{
    AlignMeasuresParams *params = dynamic_cast<AlignMeasuresParams *>(functorParams);
    assert(params);

    SetDrawingXRel(params->m_shift);

    params->m_shift += this->GetWidth();
    params->m_justifiableWidth += this->GetRightBarLineXRel() - this->GetLeftBarLineXRel();

    return FUNCTOR_SIBLINGS;
}

int Measure::ResetDrawing(FunctorParams *functorParams)
{
    this->m_leftBarLine.Reset();
    this->m_rightBarLine.Reset();
    this->m_timestampAligner.Reset();
    m_drawingEnding = NULL;
    return FUNCTOR_CONTINUE;
};

int Measure::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = dynamic_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    if ((params->m_currentSystem->GetChildCount() > 0)
        && (this->m_drawingXRel + this->GetWidth() + params->m_currentScoreDefWidth - params->m_shift
               > params->m_systemWidth)) {
        params->m_currentSystem = new System();
        params->m_page->AddChild(params->m_currentSystem);
        params->m_shift = this->m_drawingXRel;
    }

    // First add all pendings objects
    ArrayOfObjects::iterator iter;
    for (iter = params->m_pendingObjects.begin(); iter != params->m_pendingObjects.end(); iter++) {
        params->m_currentSystem->AddChild(*iter);
    }
    params->m_pendingObjects.clear();

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    Measure *measure = dynamic_cast<Measure *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    assert(measure);
    params->m_currentSystem->AddChild(measure);

    return FUNCTOR_SIBLINGS;
}

int Measure::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = dynamic_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

int Measure::FillStaffCurrentTimeSpanningEnd(FunctorParams *functorParams)
{
    FillStaffCurrentTimeSpanningParams *params = dynamic_cast<FillStaffCurrentTimeSpanningParams *>(functorParams);
    assert(params);

    std::vector<Object *>::iterator iter = params->m_timeSpanningElements.begin();
    while (iter != params->m_timeSpanningElements.end()) {
        TimeSpanningInterface *interface = (*iter)->GetTimeSpanningInterface();
        assert(interface);
        Measure *endParent = dynamic_cast<Measure *>(interface->GetEnd()->GetFirstParent(MEASURE));
        assert(endParent);
        // We have reached the end of the spanning - remove it from the list of running elements
        if (endParent == this) {
            iter = params->m_timeSpanningElements.erase(iter);
        }
        else {
            iter++;
        }
    }
    return FUNCTOR_CONTINUE;
}

int Measure::PrepareBoundaries(FunctorParams *functorParams)
{
    PrepareBoundariesParams *params = dynamic_cast<PrepareBoundariesParams *>(functorParams);
    assert(params);

    std::vector<BoundaryStartInterface *>::iterator iter;
    for (iter = params->m_startBoundaries.begin(); iter != params->m_startBoundaries.end(); iter++) {
        (*iter)->SetMeasure(this);
    }
    params->m_startBoundaries.clear();

    if (params->m_currentEnding) {
        // Set the ending to each measure in between
        this->m_drawingEnding = params->m_currentEnding;
    }

    // Keep a pointer to the measure for when we are reaching the end (see BoundaryEnd::PrepareBoundaries)
    params->m_lastMeasure = this;

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareCrossStaff(FunctorParams *functorParams)
{
    PrepareCrossStaffParams *params = dynamic_cast<PrepareCrossStaffParams *>(functorParams);
    assert(params);

    params->m_currentMeasure = this;

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = dynamic_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    if (params->m_previousEnding) {
        // We have a measure in between endings and the previous one was group, so we need to increase the grpId
        if (params->m_previousEnding->GetDrawingGrpId() > DRAWING_GRP_NONE) params->m_drawingGrpId++;
        params->m_previousEnding = NULL;
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimePointingEnd(FunctorParams *functorParams)
{
    PrepareTimePointingParams *params = dynamic_cast<PrepareTimePointingParams *>(functorParams);
    assert(params);

    if (!params->m_timePointingInterfaces.empty()) {
        LogWarning("%d time pointing element(s) could not be matched in measure %s",
            params->m_timePointingInterfaces.size(), this->GetUuid().c_str());
    }

    ArrayOfPointingInterClassIdPairs::iterator iter = params->m_timePointingInterfaces.begin();
    while (iter != params->m_timePointingInterfaces.end()) {
        iter = params->m_timePointingInterfaces.erase(iter);
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimeSpanningEnd(FunctorParams *functorParams)
{
    PrepareTimeSpanningParams *params = dynamic_cast<PrepareTimeSpanningParams *>(functorParams);
    assert(params);

    ArrayOfSpanningInterClassIdPairs::iterator iter = params->m_timeSpanningInterfaces.begin();
    while (iter != params->m_timeSpanningInterfaces.end()) {
        // At the end of the measure (going backward) we remove element for which we do not need to match the end
        // (for
        // now). Eventually, we could consider them, for example if we want to display their spanning or for
        // improved
        // midi output
        if ((iter->second == DIR) || (iter->second == DYNAM) || (iter->second == HARM)) {
            iter = params->m_timeSpanningInterfaces.erase(iter);
        }
        else {
            iter++;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimestampsEnd(FunctorParams *functorParams)
{
    PrepareTimestampsParams *params = dynamic_cast<PrepareTimestampsParams *>(functorParams);
    assert(params);

    ArrayOfObjectBeatPairs::iterator iter = params->m_tstamps.begin();
    // Loop throught the object/beat pairs and create the TimestampAttr when necessary
    while (iter != params->m_tstamps.end()) {
        // -1 means that we have a @tstamp (start) to add to the current measure
        if ((*iter).second.first == -1) {
            TimePointInterface *interface = ((*iter).first)->GetTimePointInterface();
            assert(interface);
            TimestampAttr *timestampAttr = m_timestampAligner.GetTimestampAtTime((*iter).second.second);
            interface->SetStart(timestampAttr);
            // purge the list of unmatched element is this is a TimeSpanningInterface element
            if ((*iter).first->HasInterface(INTERFACE_TIME_SPANNING)) {
                TimeSpanningInterface *tsInterface = ((*iter).first)->GetTimeSpanningInterface();
                assert(tsInterface);
                if (tsInterface->HasStartAndEnd()) {
                    auto item
                        = std::find_if(params->m_timeSpanningInterfaces.begin(), params->m_timeSpanningInterfaces.end(),
                            [tsInterface](std::pair<TimeSpanningInterface *, ClassId> pair) {
                                return (pair.first == tsInterface);
                            });
                    if (item != params->m_timeSpanningInterfaces.end()) {
                        // LogDebug("Found it!");
                        params->m_timeSpanningInterfaces.erase(item);
                    }
                }
            }
            // remove it
            iter = params->m_tstamps.erase(iter);
        }
        // 0 means that we have a @tstamp2 (end) to add to the current measure
        else if ((*iter).second.first == 0) {
            TimeSpanningInterface *interface = ((*iter).first)->GetTimeSpanningInterface();
            assert(interface);
            TimestampAttr *timestampAttr = m_timestampAligner.GetTimestampAtTime((*iter).second.second);
            interface->SetEnd(timestampAttr);
            // We can check if the interface is now fully mapped (start / end) and purge the list of unmatched
            // elements
            if (interface->HasStartAndEnd()) {
                auto item
                    = std::find_if(params->m_timeSpanningInterfaces.begin(), params->m_timeSpanningInterfaces.end(),
                        [interface](
                            std::pair<TimeSpanningInterface *, ClassId> pair) { return (pair.first == interface); });
                if (item != params->m_timeSpanningInterfaces.end()) {
                    // LogDebug("Found it!");
                    params->m_timeSpanningInterfaces.erase(item);
                }
            }
            iter = params->m_tstamps.erase(iter);
        }
        // we have not reached the correct end measure yet
        else {
            (*iter).second.first--;
            iter++;
        }
    }

    return FUNCTOR_CONTINUE;
};

int Measure::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = dynamic_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    // Here we need to update the m_totalTime from the starting time of the measure.
    params->m_totalTime = m_scoreTimeOffset.back();

    if (m_currentTempo != params->m_currentTempo) {
        params->m_midiFile->addTempo(0, m_scoreTimeOffset.back() * params->m_midiFile->getTPQ(), m_currentTempo);
        params->m_currentTempo = m_currentTempo;
    }

    return FUNCTOR_CONTINUE;
}

int Measure::GenerateTimemap(FunctorParams *functorParams)
{
    GenerateTimemapParams *params = dynamic_cast<GenerateTimemapParams *>(functorParams);
    assert(params);

    // Deal with repeated music later, for now get the last times.
    params->m_scoreTimeOffset = m_scoreTimeOffset.back();
    params->m_realTimeOffsetMilliseconds = m_realTimeOffsetMilliseconds.back();
    params->m_currentTempo = m_currentTempo;

    return FUNCTOR_CONTINUE;
}

int Measure::CalcMaxMeasureDuration(FunctorParams *functorParams)
{
    CalcMaxMeasureDurationParams *params = dynamic_cast<CalcMaxMeasureDurationParams *>(functorParams);
    assert(params);

    m_scoreTimeOffset.clear();
    m_scoreTimeOffset.push_back(params->m_maxCurrentScoreTime);
    params->m_maxCurrentScoreTime += m_measureAligner.GetRightAlignment()->GetTime() * DURATION_4 / DUR_MAX;

    // search for tempo marks in the measure
    Tempo *tempo = dynamic_cast<Tempo *>(this->FindChildByType(TEMPO));
    if (tempo && tempo->HasMidiBpm()) {
        params->m_currentTempo = tempo->GetMidiBpm();
    }
    m_currentTempo = params->m_currentTempo;

    m_realTimeOffsetMilliseconds.clear();
    m_realTimeOffsetMilliseconds.push_back(int(params->m_maxCurrentRealTimeSeconds * 1000.0 + 0.5));
    params->m_maxCurrentRealTimeSeconds
        += (m_measureAligner.GetRightAlignment()->GetTime() * DURATION_4 / DUR_MAX) * 60.0 / m_currentTempo;

    return FUNCTOR_CONTINUE;
}

int Measure::CalcOnsetOffset(FunctorParams *functorParams)
{
    CalcOnsetOffsetParams *params = dynamic_cast<CalcOnsetOffsetParams *>(functorParams);
    assert(params);

    params->m_currentTempo = m_currentTempo;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
