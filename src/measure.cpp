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
#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "functorparams.h"
#include "controlelement.h"
#include "page.h"
#include "staff.h"
#include "system.h"
#include "timeinterface.h"
#include "timestamp.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Measure
//----------------------------------------------------------------------------

Measure::Measure(bool measureMusic, int logMeasureNb) : Object("measure-"), AttCommon(), AttMeasureLog(), AttPointing()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_MEASURELOG);
    RegisterAttClass(ATT_POINTING);

    m_measuredMusic = measureMusic;
    // We set parent to it because we want to access the parent doc from the aligners
    m_measureAligner.SetParent(this);
    // Idem for timestamps
    m_timestampAligner.SetParent(this);

    // owned pointers need to be set to NULL;
    m_drawingScoreDef = NULL;

    // Make the left barLine a left one...
    m_leftBarLine.SetLeft();

    Reset();
}

Measure::~Measure()
{
    // We need to delete own objects
    Reset();
}

void Measure::Reset()
{
    Object::Reset();
    ResetCommon();
    ResetMeasureLog();
    ResetPointing();

    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    m_timestampAligner.Reset();
    m_xAbs = VRV_UNSET;
    m_drawingXRel = 0;
    m_drawingX = 0;

    // by default, we have a single barLine on the right (none on the left)
    m_rightBarLine.SetForm(this->GetRight());
    m_leftBarLine.SetForm(this->GetLeft());

    if (!m_measuredMusic) {
        m_xAbs = 0;
    }

    m_drawingEnding = NULL;
}

void Measure::AddChild(Object *child)
{
    if (child->IsControlElement()) {
        assert(dynamic_cast<ControlElement *>(child));
    }
    else if (child->Is() == STAFF) {
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

int Measure::GetLeftBarLineXRel() const
{
    if (m_measureAligner.GetLeftBarLineAlignment()) {
        return m_measureAligner.GetLeftBarLineAlignment()->GetXRel();
    }
    return 0;
}

int Measure::GetLeftBarLineX1Rel() const
{
    int x = GetLeftBarLineXRel();
    if (m_leftBarLine.HasUpdatedBB()) {
        x += m_leftBarLine.m_contentBB_x1;
    }
    return x;
}

int Measure::GetLeftBarLineX2Rel() const
{
    int x = GetLeftBarLineXRel();
    if (m_leftBarLine.HasUpdatedBB()) {
        x += m_leftBarLine.m_contentBB_x2;
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

int Measure::GetRightBarLineX1Rel() const
{
    int x = GetRightBarLineXRel();
    if (m_rightBarLine.HasUpdatedBB()) {
        x += m_rightBarLine.m_contentBB_x1;
    }
    return x;
}

int Measure::GetRightBarLineX2Rel() const
{
    int x = GetRightBarLineXRel();
    if (m_rightBarLine.HasUpdatedBB()) {
        x += m_rightBarLine.m_contentBB_x2;
    }
    return x;
}

int Measure::GetWidth() const
{
    assert(m_measureAligner.GetRightAlignment());
    if (m_measureAligner.GetRightAlignment()) {
        return m_measureAligner.GetRightAlignment()->GetXRel();
    }
    return 0;
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
        AttCommonNComparison matchN(STAFF, *iter);
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

int Measure::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    m_drawingXRel = 0;
    m_drawingX = 0;
    if (m_measureAligner.GetLeftAlignment()) {
        m_measureAligner.GetLeftAlignment()->SetXRel(0);
    }
    if (m_measureAligner.GetRightAlignment()) {
        m_measureAligner.GetRightAlignment()->SetXRel(0);
    }

    return FUNCTOR_CONTINUE;
}

int Measure::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = dynamic_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    // clear the content of the measureAligner
    m_measureAligner.Reset();

    // here we transfer the @left and @right values to the barLine objects
    this->SetLeftBarLineType(this->GetLeft());
    this->SetRightBarLineType(this->GetRight());

    // point to it
    params->m_measureAligner = &m_measureAligner;

    if (m_leftBarLine.GetForm() != BARRENDITION_NONE) {
        m_leftBarLine.SetAlignment(m_measureAligner.GetLeftBarLineAlignment());
    }

    if (m_rightBarLine.GetForm() != BARRENDITION_NONE) {
        m_rightBarLine.SetAlignment(m_measureAligner.GetRightBarLineAlignment());
    }

    // LogDebug("\n ***** Align measure %d", this->GetN());

    assert(params->m_measureAligner);

    return FUNCTOR_CONTINUE;
}

int Measure::AlignHorizontallyEnd(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = dynamic_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    // We also need to align the timestamps - we do it at the end since we need the *meterSig to be initialized by a
    // Layer. Obviously this will not work with different time signature. However, I am not sure how this would work in
    // MEI anyway.
    m_timestampAligner.Process(params->m_functor, params);

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

int Measure::SetBoundingBoxXShift(FunctorParams *functorParams)
{
    SetBoundingBoxXShiftParams *params = dynamic_cast<SetBoundingBoxXShiftParams *>(functorParams);
    assert(params);

    // we reset the measure width and the minimum position
    params->m_measureWidth = 0;
    params->m_layerMinPos = 0;
    params->m_minPos = 0;

    // Process the left scoreDef elements and the left barLine
    m_measureAligner.Process(params->m_functor, params);

    params->m_layerMinPos = params->m_minPos;

    return FUNCTOR_CONTINUE;
}

int Measure::SetBoundingBoxXShiftEnd(FunctorParams *functorParams)
{
    SetBoundingBoxXShiftParams *params = dynamic_cast<SetBoundingBoxXShiftParams *>(functorParams);
    assert(params);

    // use the measure width as minimum position of the barLine
    params->m_minPos = params->m_measureWidth;

    // Process the right barLine and the right scoreDef elements
    m_measureAligner.Process(params->m_functorEnd, params);

    return FUNCTOR_CONTINUE;
}

int Measure::IntegrateBoundingBoxGraceXShift(FunctorParams *functorParams)
{
    IntegrateBoundingBoxGraceXShiftParams *params
        = dynamic_cast<IntegrateBoundingBoxGraceXShiftParams *>(functorParams);
    assert(params);

    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int Measure::IntegrateBoundingBoxXShift(FunctorParams *functorParams)
{
    IntegrateBoundingBoxXShiftParams *params = dynamic_cast<IntegrateBoundingBoxXShiftParams *>(functorParams);
    assert(params);

    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
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
        this->m_drawingXRel = params->m_measureXRel;
    }
    else {
        params->m_measureXRel = this->m_drawingXRel;
    }

    m_measureAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int Measure::AlignMeasures(FunctorParams *functorParams)
{
    AlignMeasuresParams *params = dynamic_cast<AlignMeasuresParams *>(functorParams);
    assert(params);

    this->m_drawingXRel = params->m_shift;

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

int Measure::SetDrawingXY(FunctorParams *functorParams)
{
    SetDrawingXYParams *params = dynamic_cast<SetDrawingXYParams *>(functorParams);
    assert(params);

    params->m_currentMeasure = this;

    // Second pass where we do just process layer elements
    if (params->m_processLayerElements) {
        // However, we need to process the timestamps
        m_timestampAligner.Process(params->m_functor, params);
        return FUNCTOR_CONTINUE;
    }

    // Here we set the appropriate y value to be used for drawing
    // With Raw documents, we use m_drawingXRel that is calculated by the layout algorithm
    // With Transcription documents, we use the m_xAbs
    if (this->m_xAbs == VRV_UNSET) {
        assert(params->m_doc->GetType() == Raw);
        this->SetDrawingX(this->m_drawingXRel + params->m_currentSystem->GetDrawingX());
    }
    else {
        assert(params->m_doc->GetType() == Transcription);
        this->SetDrawingX(this->m_xAbs);
    }

    // Process the timestamps - we can do it already since the first pass in only taking care of X position for the
    // LayerElements
    m_timestampAligner.Process(params->m_functor, params);

    return FUNCTOR_CONTINUE;
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

int Measure::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = dynamic_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    if (params->m_previousEnding) {
        // We have a measure in between endings and the previous one was group, so we need to increase the grpId
        if (params->m_previousEnding->GetDrawingGrpId() > 0) params->m_drawingGrpId++;
        params->m_previousEnding = NULL;
    }

    return FUNCTOR_CONTINUE;
}

int Measure::PrepareTimeSpanningEnd(FunctorParams *functorParams)
{
    PrepareTimeSpanningParams *params = dynamic_cast<PrepareTimeSpanningParams *>(functorParams);
    assert(params);

    ArrayOfInterfaceClassIdPairs::iterator iter = params->m_timeSpanningInterfaces.begin();
    while (iter != params->m_timeSpanningInterfaces.end()) {
        // At the end of the measure (going backward) we remove element for which we do not need to match the end
        // (for
        // now). Eventually, we could consider them, for example if we want to display their spanning or for
        // improved
        // midi output
        if ((iter->second == DIR) || (iter->second == DYNAM)) {
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
                        [interface](std::pair<TimeSpanningInterface *, ClassId> pair) {
                            return (pair.first == interface);
                        });
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

    // Here we need to reset the currentMeasureTime because we are starting a new measure
    params->m_currentMeasureTime = 0;

    return FUNCTOR_CONTINUE;
}

int Measure::GenerateMIDIEnd(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = dynamic_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    // We a to the total time the maximum duration of the measure so if there is no layer, if the layer is not full
    // or
    // if there is an encoding error in the measure, the next one will be properly aligned
    assert(!params->m_maxValues.empty());
    params->m_totalTime += params->m_maxValues.front();
    params->m_maxValues.erase(params->m_maxValues.begin());

    return FUNCTOR_CONTINUE;
}

int Measure::CalcMaxMeasureDuration(FunctorParams *functorParams)
{
    CalcMaxMeasureDurationParams *params = dynamic_cast<CalcMaxMeasureDurationParams *>(functorParams);
    assert(params);

    // We just need to add a value to the stack
    params->m_maxValues.push_back(0.0);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
