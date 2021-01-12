/////////////////////////////////////////////////////////////////////////////
// Name:        measure.cpp
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "measure.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "boundary.h"
#include "comparison.h"
#include "controlelement.h"
#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "functorparams.h"
#include "hairpin.h"
#include "page.h"
#include "staff.h"
#include "staffdef.h"
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
    : Object("measure-")
    , AttBarring()
    , AttMeasureLog()
    , AttMeterConformanceBar()
    , AttNNumberLike()
    , AttPointing()
    , AttTyped()
{
    RegisterAttClass(ATT_BARRING);
    RegisterAttClass(ATT_MEASURELOG);
    RegisterAttClass(ATT_METERCONFORMANCEBAR);
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

void Measure::CloneReset()
{
    Object::CloneReset();

    m_measureAligner.Reset();
    m_measureAligner.SetParent(this);
    // Idem for timestamps
    m_timestampAligner.SetParent(this);
    // Idem for barlines
    m_leftBarLine.SetParent(this);
    m_rightBarLine.SetParent(this);

    // owned pointers need to be set to NULL;
    m_drawingScoreDef = NULL;
}

void Measure::Reset()
{
    Object::Reset();
    ResetMeasureLog();
    ResetMeterConformanceBar();
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

bool Measure::IsSupportedChild(Object *child)
{
    if (child->IsControlElement()) {
        assert(dynamic_cast<ControlElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else if (child->Is(STAFF)) {
        Staff *staff = vrv_cast<Staff *>(child);
        assert(staff);
        if (staff && (staff->GetN() < 1)) {
            // This is not 100% safe if we have a <app> and <rdg> with more than
            // one staff as a previous child.
            staff->SetN(this->GetChildCount());
        }
    }
    else {
        return false;
    }
    return true;
}

void Measure::AddChildBack(Object *child)
{
    if (!this->IsSupportedChild(child)) {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return;
    }

    child->SetParent(this);
    ArrayOfObjects *children = this->GetChildrenForModification();
    if (children->empty()) {
        children->push_back(child);
    }
    else if (children->back()->Is(STAFF)) {
        children->push_back(child);
    }
    else {
        for (auto it = children->begin(); it != children->end(); ++it) {
            if (!(*it)->Is(STAFF)) {
                children->insert(it, child);
                break;
            }
        }
    }
    Modify();
}

int Measure::GetDrawingX() const
{
    if (!this->IsMeasuredMusic()) {
        System *system = vrv_cast<System *>(this->GetFirstAncestor(SYSTEM));
        assert(system);
        if (system->m_yAbs != VRV_UNSET) {
            return (system->m_systemLeftMar);
        }
    }

    if (m_xAbs != VRV_UNSET) return m_xAbs;

    if (m_cachedDrawingX != VRV_UNSET) return m_cachedDrawingX;

    System *system = vrv_cast<System *>(this->GetFirstAncestor(SYSTEM));
    assert(system);
    m_cachedDrawingX = system->GetDrawingX() + this->GetDrawingXRel();
    return m_cachedDrawingX;
}

void Measure::ResetCachedDrawingX() const
{
    Object::ResetCachedDrawingX();
    m_timestampAligner.ResetCachedDrawingX();
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
    if (m_leftBarLine.HasSelfBB()) {
        x += m_leftBarLine.GetContentX1();
    }
    return x;
}

int Measure::GetLeftBarLineRight() const
{
    int x = GetLeftBarLineXRel();
    if (m_leftBarLine.HasSelfBB()) {
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
    if (m_rightBarLine.HasSelfBB()) {
        x += m_rightBarLine.GetContentX1();
    }
    return x;
}

int Measure::GetRightBarLineRight() const
{
    int x = GetRightBarLineXRel();
    if (m_rightBarLine.HasSelfBB()) {
        x += m_rightBarLine.GetContentX2();
    }
    return x;
}

int Measure::GetWidth() const
{
    if (!this->IsMeasuredMusic()) {
        System *system = vrv_cast<System *>(this->GetFirstAncestor(SYSTEM));
        assert(system);
        if (system->m_yAbs != VRV_UNSET) {
            Page *page = vrv_cast<Page *>(system->GetFirstAncestor(PAGE));
            assert(page);
            // xAbs2 =  page->m_pageWidth - system->m_systemRightMar;
            return page->m_pageWidth - system->m_systemLeftMar - system->m_systemRightMar;
        }
    }

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

int Measure::GetDrawingOverflow()
{
    Functor adjustXOverlfow(&Object::AdjustXOverflow);
    Functor adjustXOverlfowEnd(&Object::AdjustXOverflowEnd);
    AdjustXOverflowParams adjustXOverflowParams(0);
    adjustXOverflowParams.m_currentSystem = vrv_cast<System *>(this->GetFirstAncestor(SYSTEM));
    assert(adjustXOverflowParams.m_currentSystem);
    adjustXOverflowParams.m_lastMeasure = this;
    this->Process(&adjustXOverlfow, &adjustXOverflowParams, &adjustXOverlfowEnd);
    if (!adjustXOverflowParams.m_currentWidest) return 0;

    int measureRightX = this->GetDrawingX() + this->GetWidth();
    int overflow = adjustXOverflowParams.m_currentWidest->GetContentRight() - measureRightX;
    return std::max(0, overflow);
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
    ClassIdComparison matchType(STAFFGRP);
    ListOfObjects staffGrps;
    scoreDef->FindAllDescendantByComparison(&staffGrps, &matchType);

    // Then the @n of each first staffDef
    for (auto &staffGrp : staffGrps) {
        StaffDef *staffDef = dynamic_cast<StaffDef *>((staffGrp)->GetFirst(STAFFDEF));
        if (staffDef) staffList.push_back(staffDef->GetN());
    }

    // Get the corresponding staves in the measure
    for (iter = staffList.begin(); iter != staffList.end(); ++iter) {
        AttNIntegerComparison matchN(STAFF, *iter);
        Staff *staff = dynamic_cast<Staff *>(this->FindDescendantByComparison(&matchN, 1));
        if (!staff) {
            // LogDebug("Staff with @n '%d' not found in measure '%s'", *iter, measure->GetUuid().c_str());
            continue;
        }
        staves.push_back(staff);
    }
    if (staves.empty()) LogDebug("Empty @staff array");
    return staves;
}

Staff *Measure::GetTopVisibleStaff()
{
    Staff *staff = NULL;
    ListOfObjects staves;
    ClassIdComparison matchType(STAFF);
    this->FindAllDescendantByComparison(&staves, &matchType, 1);
    for (auto &child : staves) {
        staff = vrv_cast<Staff *>(child);
        assert(staff);
        if (staff->DrawingIsVisible()) {
            break;
        }
        staff = NULL;
    }
    return staff;
}

Staff *Measure::GetBottomVisibleStaff()
{
    Staff *bottomStaff = NULL;
    ListOfObjects staves;
    ClassIdComparison matchType(STAFF);
    this->FindAllDescendantByComparison(&staves, &matchType, 1);
    for (const auto child : staves) {
        Staff *staff = vrv_cast<Staff *>(child);
        assert(staff);
        if (!staff->DrawingIsVisible()) {
            continue;
        }
        bottomStaff = staff;
    }
    return bottomStaff;
}

int Measure::EnclosesTime(int time) const
{
    int repeat = 1;
    int timeDuration = int(
        m_measureAligner.GetRightAlignment()->GetTime() * DURATION_4 / DUR_MAX * 60.0 / m_currentTempo * 1000.0 + 0.5);
    std::vector<double>::const_iterator iter;
    for (iter = m_realTimeOffsetMilliseconds.begin(); iter != m_realTimeOffsetMilliseconds.end(); ++iter) {
        if ((time >= *iter) && (time <= *iter + timeDuration)) return repeat;
        repeat++;
    }
    return 0;
}

double Measure::GetRealTimeOffsetMilliseconds(int repeat) const
{
    if ((repeat < 1) || repeat > (int)m_realTimeOffsetMilliseconds.size()) return 0;
    return m_realTimeOffsetMilliseconds.at(repeat - 1);
}

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

//----------------------------------------------------------------------------
// Measure functor methods
//----------------------------------------------------------------------------

int Measure::ConvertMarkupAnalyticalEnd(FunctorParams *functorParams)
{
    ConvertMarkupAnalyticalParams *params = vrv_params_cast<ConvertMarkupAnalyticalParams *>(functorParams);
    assert(params);

    ArrayOfObjects::iterator iter;
    for (iter = params->m_controlEvents.begin(); iter != params->m_controlEvents.end(); ++iter) {
        this->AddChild(*iter);
    }

    params->m_controlEvents.clear();

    return FUNCTOR_CONTINUE;
}

int Measure::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    // Move itself to the pageBasedSystem - do not process children
    this->MoveItselfTo(params->m_pageBasedSystem);

    return FUNCTOR_SIBLINGS;
}

int Measure::ConvertToCastOffMensural(FunctorParams *functorParams)
{
    ConvertToCastOffMensuralParams *params = vrv_params_cast<ConvertToCastOffMensuralParams *>(functorParams);
    assert(params);

    // We are processing by staff/layer from the call below - we obviously do not want to loop...
    if (params->m_targetMeasure) {
        return FUNCTOR_CONTINUE;
    }

    bool convertToMeasured = params->m_doc->GetOptions()->m_mensuralToMeasure.GetValue();

    assert(params->m_targetSystem);
    assert(params->m_layerTree);

    // Create a temporary subsystem for receiving the measure segments
    System targetSubSystem;
    params->m_targetSubSystem = &targetSubSystem;

    // Create the first measure segment - problem: we are dropping the section element - we should create a score-based
    // MEI file instead
    Measure *measure = new Measure(convertToMeasured);
    if (convertToMeasured) {
        measure->SetN(StringFormat("%d", params->m_segmentTotal + 1));
    }
    params->m_targetSubSystem->AddChild(measure);

    ArrayOfComparisons filters;
    // Now we can process by layer and move their content to (measure) segments
    for (auto const &staves : params->m_layerTree->child) {
        for (auto const &layers : staves.second.child) {
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves.first);
            AttNIntegerComparison matchLayer(LAYER, layers.first);
            filters = { &matchStaff, &matchLayer };

            params->m_segmentIdx = 1;
            params->m_targetMeasure = measure;

            Functor convertToCastOffMensural(&Object::ConvertToCastOffMensural);
            this->Process(&convertToCastOffMensural, params, NULL, &filters);
        }
    }

    params->m_targetMeasure = NULL;
    params->m_targetSubSystem = NULL;
    params->m_segmentTotal = targetSubSystem.GetChildCount();
    // Copy the measure segments to the final target segment
    params->m_targetSystem->MoveChildrenFrom(&targetSubSystem);

    return FUNCTOR_SIBLINGS;
}

int Measure::ConvertToUnCastOffMensural(FunctorParams *functorParams)
{
    ConvertToUnCastOffMensuralParams *params = vrv_params_cast<ConvertToUnCastOffMensuralParams *>(functorParams);
    assert(params);

    if (params->m_contentMeasure == NULL) {
        params->m_contentMeasure = this;
    }
    else if (params->m_addSegmentsToDelete) {
        params->m_segmentsToDelete.push_back(this);
    }

    return FUNCTOR_CONTINUE;
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
    UnsetCurrentScoreDefParams *params = vrv_params_cast<UnsetCurrentScoreDefParams *>(functorParams);
    assert(params);

    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    // We also need to remove scoreDef elements in the AlignmentReference objects
    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_CONTINUE;
}

int Measure::OptimizeScoreDef(FunctorParams *functorParams)
{
    OptimizeScoreDefParams *params = vrv_params_cast<OptimizeScoreDefParams *>(functorParams);
    assert(params);

    if (!params->m_doc->GetOptions()->m_condenseTempoPages.GetValue()) {
        return FUNCTOR_CONTINUE;
    }

    params->m_hasFermata = (this->FindDescendantByType(FERMATA));
    params->m_hasTempo = (this->FindDescendantByType(TEMPO));

    return FUNCTOR_CONTINUE;
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
    ApplyPPUFactorParams *params = vrv_params_cast<ApplyPPUFactorParams *>(functorParams);
    assert(params);

    if (m_xAbs != VRV_UNSET) m_xAbs /= params->m_page->GetPPUFactor();
    if (m_xAbs2 != VRV_UNSET) m_xAbs2 /= params->m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

int Measure::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = vrv_params_cast<AlignHorizontallyParams *>(functorParams);
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
    AlignHorizontallyParams *params = vrv_params_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    // We also need to align the timestamps - we do it at the end since we need the *meterSig to be initialized by a
    // Layer. Obviously this will not work with different time signature. However, I am not sure how this would work
    // in MEI anyway.
    m_timestampAligner.Process(params->m_functor, params);

    // Next scoreDef will be INTERMEDIATE_SCOREDEF (See Layer::AlignHorizontally)
    params->m_isFirstMeasure = false;

    if (params->m_hasMultipleLayer) m_hasAlignmentRefWithMultipleLayers = true;

    return FUNCTOR_CONTINUE;
}

int Measure::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = vrv_params_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    // we also need to reset the staffNb
    params->m_staffIdx = 0;

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustArpegEnd(FunctorParams *functorParams)
{
    AdjustArpegParams *params = vrv_params_cast<AdjustArpegParams *>(functorParams);
    assert(params);

    if (!params->m_alignmentArpegTuples.empty()) {
        params->m_measureAligner = &m_measureAligner;
        m_measureAligner.Process(params->m_functor, params, NULL, NULL, UNLIMITED_DEPTH, BACKWARD);
        params->m_alignmentArpegTuples.clear();
    }

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustLayers(FunctorParams *functorParams)
{
    AdjustLayersParams *params = vrv_params_cast<AdjustLayersParams *>(functorParams);
    assert(params);

    if (!m_hasAlignmentRefWithMultipleLayers) return FUNCTOR_SIBLINGS;

    std::vector<int>::iterator iter;
    ArrayOfComparisons filters;
    for (iter = params->m_staffNs.begin(); iter != params->m_staffNs.end(); ++iter) {
        filters.clear();
        // Create ad comparison object for each type / @n
        std::vector<int> ns;
        // -1 for barline attributes that need to be taken into account each time
        ns.push_back(-1);
        ns.push_back(*iter);
        AttNIntegerAnyComparison matchStaff(ALIGNMENT_REFERENCE, ns);
        filters.push_back(&matchStaff);

        m_measureAligner.Process(params->m_functor, params, NULL, &filters);
    }

    return FUNCTOR_SIBLINGS;
}

int Measure::AdjustAccidX(FunctorParams *functorParams)
{
    AdjustAccidXParams *params = vrv_params_cast<AdjustAccidXParams *>(functorParams);
    assert(params);

    params->m_currentMeasure = this;

    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustGraceXPos(FunctorParams *functorParams)
{
    AdjustGraceXPosParams *params = vrv_params_cast<AdjustGraceXPosParams *>(functorParams);
    assert(params);

    m_measureAligner.PushAlignmentsRight();
    params->m_rightDefaultAlignment = NULL;

    // We process it backward because we want to get the rightDefaultAlignment
    m_measureAligner.Process(params->m_functor, params, params->m_functorEnd, NULL, UNLIMITED_DEPTH, BACKWARD);

    // We need to process the staves in the reverse order
    std::vector<int> staffNs = params->m_staffNs;
    std::vector<int> staffNsReversed;
    staffNsReversed.resize(staffNs.size());
    std::reverse_copy(staffNs.begin(), staffNs.end(), staffNsReversed.begin());

    m_measureAligner.PushAlignmentsRight();
    params->m_rightDefaultAlignment = NULL;

    params->m_staffNs = staffNsReversed;
    m_measureAligner.Process(params->m_functor, params, params->m_functorEnd, NULL, UNLIMITED_DEPTH, BACKWARD);

    // Put params back
    params->m_staffNs = staffNs;

    return FUNCTOR_SIBLINGS;
}

int Measure::AdjustXPos(FunctorParams *functorParams)
{
    AdjustXPosParams *params = vrv_params_cast<AdjustXPosParams *>(functorParams);
    assert(params);

    params->m_minPos = 0;
    params->m_upcomingMinPos = VRV_UNSET;
    params->m_cumulatedXShift = 0;

    std::vector<int>::iterator iter;
    ArrayOfComparisons filters;
    for (iter = params->m_staffNs.begin(); iter != params->m_staffNs.end(); ++iter) {
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
        AttNIntegerAnyComparison matchStaff(ALIGNMENT_REFERENCE, ns);
        filters.push_back(&matchStaff);

        m_measureAligner.Process(params->m_functor, params, params->m_functorEnd, &filters);
    }

    // m_measureAligner.Process(params->m_functor, params, params->m_functorEnd);

    int minMeasureWidth
        = params->m_doc->GetOptions()->m_unit.GetValue() * params->m_doc->GetOptions()->m_measureMinWidth.GetValue();
    // First try to see if we have a double measure length element
    MeasureAlignerTypeComparison alignmentComparison(ALIGNMENT_FULLMEASURE2);
    Alignment *fullMeasure2
        = dynamic_cast<Alignment *>(m_measureAligner.FindDescendantByComparison(&alignmentComparison, 1));

    // With a double measure with element (mRpt2, multiRpt)
    if (fullMeasure2 != NULL) {
        minMeasureWidth *= 2;
    }
    // Nothing if the measure has at least one note or @metcon="false"
    else if ((this->FindDescendantByType(NOTE) != NULL) || (this->GetMetcon() == BOOLEAN_false)) {
        minMeasureWidth = 0;
    }

    int currentMeasureWidth = this->GetRightBarLineLeft() - this->GetLeftBarLineRight();
    if (currentMeasureWidth < minMeasureWidth) {
        ArrayOfAdjustmentTuples boundaries{ std::make_tuple(this->GetLeftBarLine()->GetAlignment(),
            this->GetRightBarLine()->GetAlignment(), minMeasureWidth - currentMeasureWidth) };
        m_measureAligner.AdjustProportionally(boundaries);
    }

    return FUNCTOR_SIBLINGS;
}

int Measure::AdjustHarmGrpsSpacingEnd(FunctorParams *functorParams)
{
    AdjustHarmGrpsSpacingParams *params = vrv_params_cast<AdjustHarmGrpsSpacingParams *>(functorParams);
    assert(params);

    // At the end of the measure - pass it along for overlapping verses
    params->m_previousMeasure = this;

    // Ajust the postion of the alignment according to what we have collected for this harm gpr
    m_measureAligner.AdjustProportionally(params->m_overlapingHarm);
    params->m_overlapingHarm.clear();

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustSylSpacingEnd(FunctorParams *functorParams)
{
    AdjustSylSpacingParams *params = vrv_params_cast<AdjustSylSpacingParams *>(functorParams);
    assert(params);

    // At the end of the measure - pass it along for overlapping verses
    params->m_previousMeasure = this;

    // Ajust the postion of the alignment according to what we have collected for this verse
    m_measureAligner.AdjustProportionally(params->m_overlapingSyl);
    params->m_overlapingSyl.clear();

    return FUNCTOR_CONTINUE;
}

int Measure::AdjustXOverflow(FunctorParams *functorParams)
{
    AdjustXOverflowParams *params = vrv_params_cast<AdjustXOverflowParams *>(functorParams);
    assert(params);

    params->m_lastMeasure = this;
    // For now look only at the content of the last measure, so discard any previous control event.
    // We need to do this because AdjustXOverflow is run before measures are aligned, so the right
    // position comparison do not actually tell us which one is the longest. This is not optimal
    // and can be improved.
    params->m_currentWidest = NULL;

    return FUNCTOR_CONTINUE;
}

int Measure::SetAlignmentXPos(FunctorParams *functorParams)
{
    SetAlignmentXPosParams *params = vrv_params_cast<SetAlignmentXPosParams *>(functorParams);
    assert(params);

    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int Measure::JustifyX(FunctorParams *functorParams)
{
    JustifyXParams *params = vrv_params_cast<JustifyXParams *>(functorParams);
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
    AlignMeasuresParams *params = vrv_params_cast<AlignMeasuresParams *>(functorParams);
    assert(params);

    SetDrawingXRel(params->m_shift);

    params->m_shift += this->GetWidth();
    params->m_justifiableWidth += this->GetRightBarLineXRel() - this->GetLeftBarLineXRel();

    return FUNCTOR_SIBLINGS;
}

int Measure::ResetDrawing(FunctorParams *functorParams)
{
    this->m_timestampAligner.Reset();
    m_drawingEnding = NULL;
    return FUNCTOR_CONTINUE;
}

int Measure::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Check if the measure has some overlfowing control elements
    int overflow = this->GetDrawingOverflow();

    if (params->m_currentSystem->GetChildCount() > 0) {
        // We have overflowing content (dir, dynam, tempo) larger than 5 units, keep it as pending
        if (overflow > (params->m_doc->GetDrawingUnit(100) * 5)) {
            Measure *measure = dynamic_cast<Measure *>(params->m_contentSystem->Relinquish(this->GetIdx()));
            assert(measure);
            // move as pending since we want it not to be broken with the next measure
            params->m_pendingObjects.push_back(measure);
            // continue
            return FUNCTOR_SIBLINGS;
        }
        // Break it if necessary
        else if (this->m_drawingXRel + this->GetWidth() + params->m_currentScoreDefWidth - params->m_shift
            > params->m_systemWidth) {
            params->m_currentSystem = new System();
            params->m_page->AddChild(params->m_currentSystem);
            params->m_shift = this->m_drawingXRel;
            for (Object *oneOfPendingObjects : params->m_pendingObjects) {
                if (oneOfPendingObjects->Is(MEASURE)) {
                    Measure *firstPendingMesure = vrv_cast<Measure *>(oneOfPendingObjects);
                    assert(firstPendingMesure);
                    params->m_shift = firstPendingMesure->m_drawingXRel;
                    // it has to be first measure
                    break;
                }
            }
        }
    }

    // First add all pendings objects
    ArrayOfObjects::iterator iter;
    for (iter = params->m_pendingObjects.begin(); iter != params->m_pendingObjects.end(); ++iter) {
        params->m_currentSystem->AddChild(*iter);
    }
    params->m_pendingObjects.clear();

    // Special case where we use the Relinquish method.
    Measure *measure = dynamic_cast<Measure *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    assert(measure);
    params->m_currentSystem->AddChild(measure);

    return FUNCTOR_SIBLINGS;
}

int Measure::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

int Measure::FillStaffCurrentTimeSpanningEnd(FunctorParams *functorParams)
{
    FillStaffCurrentTimeSpanningParams *params = vrv_params_cast<FillStaffCurrentTimeSpanningParams *>(functorParams);
    assert(params);

    std::vector<Object *>::iterator iter = params->m_timeSpanningElements.begin();
    while (iter != params->m_timeSpanningElements.end()) {
        Measure *endParent = NULL;
        if ((*iter)->HasInterface(INTERFACE_TIME_SPANNING)) {
            TimeSpanningInterface *interface = (*iter)->GetTimeSpanningInterface();
            assert(interface);
            if (interface->GetEnd()) {
                endParent = dynamic_cast<Measure *>(interface->GetEnd()->GetFirstAncestor(MEASURE));
            }
        }
        if (!endParent && (*iter)->HasInterface(INTERFACE_LINKING)) {
            LinkingInterface *interface = (*iter)->GetLinkingInterface();
            assert(interface);
            if (interface->GetNextLink()) {
                // We should have one because we allow only control Event (dir and dynam) to be linked as target
                TimePointInterface *nextInterface = interface->GetNextLink()->GetTimePointInterface();
                assert(nextInterface);
                endParent = dynamic_cast<Measure *>(nextInterface->GetStart()->GetFirstAncestor(MEASURE));
            }
        }
        assert(endParent);
        // We have reached the end of the spanning - remove it from the list of running elements
        if (endParent == this) {
            iter = params->m_timeSpanningElements.erase(iter);
        }
        else {
            ++iter;
        }
    }
    return FUNCTOR_CONTINUE;
}

int Measure::PrepareBoundaries(FunctorParams *functorParams)
{
    PrepareBoundariesParams *params = vrv_params_cast<PrepareBoundariesParams *>(functorParams);
    assert(params);

    std::vector<BoundaryStartInterface *>::iterator iter;
    for (iter = params->m_startBoundaries.begin(); iter != params->m_startBoundaries.end(); ++iter) {
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
    PrepareCrossStaffParams *params = vrv_params_cast<PrepareCrossStaffParams *>(functorParams);
    assert(params);

    params->m_currentMeasure = this;

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = vrv_params_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    if (params->m_previousEnding) {
        // We have a measure in between endings and the previous one was group, just reset pointer to NULL
        params->m_previousEnding = NULL;
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareFloatingGrpsEnd(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = vrv_params_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    params->m_dynams.clear();

    std::vector<Hairpin *>::iterator iter = params->m_hairpins.begin();
    while (iter != params->m_hairpins.end()) {
        assert((*iter)->GetEnd());
        Measure *measureEnd = dynamic_cast<Measure *>((*iter)->GetEnd()->GetFirstAncestor(MEASURE));
        if (measureEnd == this) {
            iter = params->m_hairpins.erase(iter);
        }
        else {
            ++iter;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimePointingEnd(FunctorParams *functorParams)
{
    PrepareTimePointingParams *params = vrv_params_cast<PrepareTimePointingParams *>(functorParams);
    assert(params);

    if (!params->m_timePointingInterfaces.empty()) {
        LogWarning("%d time pointing element(s) could not be matched in measure %s",
            params->m_timePointingInterfaces.size(), this->GetUuid().c_str());
    }

    ListOfPointingInterClassIdPairs::iterator iter = params->m_timePointingInterfaces.begin();
    while (iter != params->m_timePointingInterfaces.end()) {
        iter = params->m_timePointingInterfaces.erase(iter);
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimeSpanningEnd(FunctorParams *functorParams)
{
    PrepareTimeSpanningParams *params = vrv_params_cast<PrepareTimeSpanningParams *>(functorParams);
    assert(params);

    ListOfSpanningInterClassIdPairs::iterator iter = params->m_timeSpanningInterfaces.begin();
    while (iter != params->m_timeSpanningInterfaces.end()) {
        // At the end of the measure (going backward) we remove element for which we do not need to match the end (for
        // now). Eventually, we could consider them, for example if we want to display their spanning or for improved
        // midi output
        if (iter->second == HARM) {
            iter = params->m_timeSpanningInterfaces.erase(iter);
        }
        else {
            ++iter;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimestampsEnd(FunctorParams *functorParams)
{
    PrepareTimestampsParams *params = vrv_params_cast<PrepareTimestampsParams *>(functorParams);
    assert(params);

    ListOfObjectBeatPairs::iterator iter = params->m_tstamps.begin();
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
            ++iter;
        }
    }

    return FUNCTOR_CONTINUE;
}

int Measure::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
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
    GenerateTimemapParams *params = vrv_params_cast<GenerateTimemapParams *>(functorParams);
    assert(params);

    // Deal with repeated music later, for now get the last times.
    params->m_scoreTimeOffset = m_scoreTimeOffset.back();
    params->m_realTimeOffsetMilliseconds = m_realTimeOffsetMilliseconds.back();
    params->m_currentTempo = m_currentTempo;

    return FUNCTOR_CONTINUE;
}

int Measure::CalcMaxMeasureDuration(FunctorParams *functorParams)
{
    CalcMaxMeasureDurationParams *params = vrv_params_cast<CalcMaxMeasureDurationParams *>(functorParams);
    assert(params);

    m_scoreTimeOffset.clear();
    m_scoreTimeOffset.push_back(params->m_maxCurrentScoreTime);
    params->m_maxCurrentScoreTime += m_measureAligner.GetRightAlignment()->GetTime() * DURATION_4 / DUR_MAX;

    // search for tempo marks in the measure
    Tempo *tempo = dynamic_cast<Tempo *>(this->FindDescendantByType(TEMPO));
    if (tempo && tempo->HasMidiBpm()) {
        params->m_currentTempo = tempo->GetMidiBpm();
    }
    else if (tempo && tempo->HasMm()) {
        int mm = tempo->GetMm();
        int mmUnit = 4;
        if (tempo->HasMmUnit() && (tempo->GetMmUnit() > DURATION_breve)) {
            mmUnit = pow(2, (int)tempo->GetMmUnit() - 2);
        }
        if (tempo->HasMmDots()) {
            mmUnit = 2 * mmUnit - (mmUnit / pow(2, tempo->GetMmDots()));
        }
        params->m_currentTempo = int(mm * 4.0 / mmUnit + 0.5);
    }
    m_currentTempo = params->m_currentTempo * params->m_tempoAdjustment;

    m_realTimeOffsetMilliseconds.clear();
    // m_realTimeOffsetMilliseconds.push_back(int(params->m_maxCurrentRealTimeSeconds * 1000.0 + 0.5));
    m_realTimeOffsetMilliseconds.push_back(params->m_maxCurrentRealTimeSeconds * 1000.0);
    params->m_maxCurrentRealTimeSeconds
        += (m_measureAligner.GetRightAlignment()->GetTime() * DURATION_4 / DUR_MAX) * 60.0 / m_currentTempo;

    return FUNCTOR_CONTINUE;
}

int Measure::CalcOnsetOffset(FunctorParams *functorParams)
{
    CalcOnsetOffsetParams *params = vrv_params_cast<CalcOnsetOffsetParams *>(functorParams);
    assert(params);

    params->m_currentTempo = m_currentTempo;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
