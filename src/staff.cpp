/////////////////////////////////////////////////////////////////////////////
// Name:        staff.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "staff.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <vector>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "functorparams.h"
#include "hairpin.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "note.h"
#include "page.h"
#include "staffdef.h"
#include "syl.h"
#include "system.h"
#include "timeinterface.h"
#include "verse.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

Staff::Staff(int n) : Object("staff-"), FacsimileInterface(), AttNInteger(), AttTyped(), AttVisibility()
{
    RegisterAttClass(ATT_NINTEGER);
    RegisterAttClass(ATT_TYPED);
    RegisterAttClass(ATT_VISIBILITY);
    RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());
    // owned pointers need to be set to NULL;
    m_ledgerLinesAbove = NULL;
    m_ledgerLinesBelow = NULL;
    m_ledgerLinesAboveCue = NULL;
    m_ledgerLinesBelowCue = NULL;

    Reset();
    SetN(n);
}

Staff::~Staff()
{
    ClearLedgerLines();
}

void Staff::Reset()
{
    Object::Reset();
    FacsimileInterface::Reset();
    ResetNInteger();
    ResetTyped();
    ResetVisibility();

    m_yAbs = VRV_UNSET;

    m_drawingStaffSize = 100;
    m_drawingLines = 5;
    m_drawingNotationType = NOTATIONTYPE_NONE;
    m_staffAlignment = NULL;
    m_timeSpanningElements.clear();
    m_drawingStaffDef = NULL;

    ClearLedgerLines();
}

void Staff::CloneReset()
{
    Object::CloneReset();

    m_ledgerLinesAbove = NULL;
    m_ledgerLinesBelow = NULL;
    m_ledgerLinesAboveCue = NULL;
    m_ledgerLinesBelowCue = NULL;

    m_drawingStaffSize = 100;
    m_drawingLines = 5;
    m_drawingNotationType = NOTATIONTYPE_NONE;
    m_staffAlignment = NULL;
    m_timeSpanningElements.clear();
    m_drawingStaffDef = NULL;
}

const ArrayOfObjects *Staff::GetChildren(bool docChildren) const
{
    return Object::GetChildren(true);
}

void Staff::ClearLedgerLines()
{
    if (m_ledgerLinesAbove) {
        delete m_ledgerLinesAbove;
        m_ledgerLinesAbove = NULL;
    }
    if (m_ledgerLinesBelow) {
        delete m_ledgerLinesBelow;
        m_ledgerLinesBelow = NULL;
    }
    if (m_ledgerLinesAboveCue) {
        delete m_ledgerLinesAboveCue;
        m_ledgerLinesAboveCue = NULL;
    }
    if (m_ledgerLinesBelowCue) {
        delete m_ledgerLinesBelowCue;
        m_ledgerLinesBelowCue = NULL;
    }
}

bool Staff::IsSupportedChild(Object *child)
{
    if (child->Is(LAYER)) {
        Layer *layer = vrv_cast<Layer *>(child);
        assert(layer);
        if (layer && !layer->HasN()) {
            // This is not 100% safe if we have a <app> and <rdg> with more than
            // one layer as a previous child.
            layer->SetN(this->GetChildCount(LAYER) + 1);
        }
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

int Staff::GetDrawingX() const
{
    if (this->HasFacs()) {
        Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
        assert(doc);
        if (doc->GetType() == Facs) {
            return FacsimileInterface::GetDrawingX();
        }
    }
    return Object::GetDrawingX();
}

int Staff::GetDrawingY() const
{
    if (this->HasFacs()) {
        Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
        assert(DOC);
        if (doc->GetType() == Facs) {
            return FacsimileInterface::GetDrawingY();
        }
    }

    if (m_yAbs != VRV_UNSET) return m_yAbs;

    if (!m_staffAlignment) return 0;

    if (m_cachedDrawingY != VRV_UNSET) return m_cachedDrawingY;

    System *system = vrv_cast<System *>(this->GetFirstAncestor(SYSTEM));
    assert(system);

    m_cachedDrawingY = system->GetDrawingY() + m_staffAlignment->GetYRel();
    return m_cachedDrawingY;
}

double Staff::GetDrawingRotate() const
{
    if (this->HasFacs()) {
        Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
        assert(doc);
        if (doc->GetType() == Facs) {
            return FacsimileInterface::GetDrawingRotate();
        }
    }
    return 0;
}

void Staff::AdjustDrawingStaffSize()
{
    if (this->HasFacs()) {
        Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
        assert(doc);
        if (doc->GetType() == Facs) {
            double rotate = this->GetDrawingRotate();
            Zone *zone = this->GetZone();
            assert(zone);
            int yDiff
                = zone->GetLry() - zone->GetUly() - (zone->GetLrx() - zone->GetUlx()) * tan(abs(rotate) * M_PI / 180.0);
            this->m_drawingStaffSize = 100 * yDiff / (doc->GetOptions()->m_unit.GetValue() * 2 * (m_drawingLines - 1));
        }
    }
}

bool Staff::DrawingIsVisible()
{
    System *system = vrv_cast<System *>(this->GetFirstAncestor(SYSTEM));
    assert(system);
    assert(system->GetDrawingScoreDef());

    StaffDef *staffDef = system->GetDrawingScoreDef()->GetStaffDef(this->GetN());
    assert(staffDef);
    return (staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN);
}

int Staff::CalcPitchPosYRel(Doc *doc, int loc)
{
    assert(doc);

    // the staff loc offset is based on the number of lines: 0 with 1 line, 2 with 2, etc
    int staffLocOffset = (this->m_drawingLines - 1) * 2;
    return (loc - staffLocOffset) * doc->GetDrawingUnit(this->m_drawingStaffSize);
}

void Staff::AddLedgerLineAbove(int count, int left, int right, bool cueSize)
{
    if (cueSize) {
        if (m_ledgerLinesAboveCue == NULL) m_ledgerLinesAboveCue = new ArrayOfLedgerLines;
        AddLedgerLines(m_ledgerLinesAboveCue, count, left, right);
    }
    else {
        if (m_ledgerLinesAbove == NULL) m_ledgerLinesAbove = new ArrayOfLedgerLines;
        AddLedgerLines(m_ledgerLinesAbove, count, left, right);
    }
}

void Staff::AddLedgerLineBelow(int count, int left, int right, bool cueSize)
{
    if (cueSize) {
        if (m_ledgerLinesBelowCue == NULL) m_ledgerLinesBelowCue = new ArrayOfLedgerLines;
        AddLedgerLines(m_ledgerLinesBelowCue, count, left, right);
    }
    else {
        if (m_ledgerLinesBelow == NULL) m_ledgerLinesBelow = new ArrayOfLedgerLines;
        AddLedgerLines(m_ledgerLinesBelow, count, left, right);
    }
}

void Staff::AddLedgerLines(ArrayOfLedgerLines *lines, int count, int left, int right)
{
    assert(lines);

    if ((int)lines->size() < count) lines->resize(count);
    int i = 0;
    for (i = 0; i < count; ++i) {
        lines->at(i).AddDash(left, right);
    }
}

void Staff::SetFromFacsimile(Doc *doc)
{
    if (!this->HasFacs()) return;
    if (this->GetZone() == NULL) {
        assert(doc);
        Zone *zone = doc->GetFacsimile()->FindZoneByUuid(this->GetFacs());
        assert(zone);
        this->SetZone(zone);
    }
    this->AdjustDrawingStaffSize();
}

//----------------------------------------------------------------------------
// LedgerLine
//----------------------------------------------------------------------------

LedgerLine::LedgerLine()
{
    Reset();
}

LedgerLine::~LedgerLine() {}

void LedgerLine::Reset()
{
    m_dashes.clear();
}

void LedgerLine::AddDash(int left, int right)
{
    assert(left < right);

    std::list<std::pair<int, int> >::iterator iter;

    // First add the dash
    for (iter = m_dashes.begin(); iter != m_dashes.end(); ++iter) {
        if (iter->first > left) break;
    }
    m_dashes.insert(iter, std::make_pair(left, right));

    // Merge overlapping dashes
    std::list<std::pair<int, int> >::iterator previous = m_dashes.begin();
    iter = m_dashes.begin();
    ++iter;
    while (iter != m_dashes.end()) {
        if (previous->second > iter->first) {
            previous->second = std::max(iter->second, previous->second);
            iter = m_dashes.erase(iter);
        }
        else {
            previous = iter;
            ++iter;
        }
    }
}

//----------------------------------------------------------------------------
// Staff functor methods
//----------------------------------------------------------------------------

int Staff::ConvertToCastOffMensural(FunctorParams *functorParams)
{
    ConvertToCastOffMensuralParams *params = vrv_params_cast<ConvertToCastOffMensuralParams *>(functorParams);
    assert(params);

    params->m_targetStaff = new Staff(*this);
    params->m_targetStaff->ClearChildren();
    params->m_targetStaff->CloneReset();
    // Keep the xml:id of the staff in the first staff segment
    params->m_targetStaff->SwapUuid(this);
    assert(params->m_targetMeasure);
    params->m_targetMeasure->AddChild(params->m_targetStaff);

    return FUNCTOR_CONTINUE;
}

int Staff::UnsetCurrentScoreDef(FunctorParams *functorParams)
{
    m_drawingStaffDef = NULL;

    return FUNCTOR_CONTINUE;
}

int Staff::OptimizeScoreDef(FunctorParams *functorParams)
{
    OptimizeScoreDefParams *params = vrv_params_cast<OptimizeScoreDefParams *>(functorParams);
    assert(params);

    assert(params->m_currentScoreDef);
    StaffDef *staffDef = params->m_currentScoreDef->GetStaffDef(this->GetN());

    if (!staffDef) {
        LogDebug(
            "Could not find staffDef for staff (%d) when optimizing scoreDef in Staff::OptimizeScoreDef", this->GetN());
        return FUNCTOR_SIBLINGS;
    }

    // Always show all staves when there is a fermata or a tempo
    // (without checking if the fermata is actually on that staff)
    if (params->m_hasFermata || params->m_hasTempo) {
        staffDef->SetDrawingVisibility(OPTIMIZATION_SHOW);
    }

    if (staffDef->GetDrawingVisibility() == OPTIMIZATION_SHOW) {
        return FUNCTOR_SIBLINGS;
    }

    staffDef->SetDrawingVisibility(OPTIMIZATION_HIDDEN);

    // Ignore layers that are empty (or with @sameas)
    ListOfObjects layers;
    IsEmptyComparison matchTypeLayer(LAYER, true);
    this->FindAllDescendantByComparison(&layers, &matchTypeLayer);

    ListOfObjects mRests;
    ClassIdComparison matchTypeMRest(MREST);
    this->FindAllDescendantByComparison(&mRests, &matchTypeMRest);

    // Show the staff only if no layer with content or only mRests
    if (layers.empty() || (mRests.size() != layers.size())) {
        staffDef->SetDrawingVisibility(OPTIMIZATION_SHOW);
    }

    return FUNCTOR_SIBLINGS;
}

int Staff::ResetVerticalAlignment(FunctorParams *functorParams)
{
    m_staffAlignment = NULL;

    ClearLedgerLines();

    return FUNCTOR_CONTINUE;
}

int Staff::ApplyPPUFactor(FunctorParams *functorParams)
{
    ApplyPPUFactorParams *params = vrv_params_cast<ApplyPPUFactorParams *>(functorParams);
    assert(params);

    if (m_yAbs != VRV_UNSET) m_yAbs /= params->m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

int Staff::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = vrv_params_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    assert(this->m_drawingStaffDef);

    if (this->m_drawingStaffDef->HasNotationtype()) {
        params->m_notationType = this->m_drawingStaffDef->GetNotationtype();
    }
    else {
        params->m_notationType = NOTATIONTYPE_cmn;
    }

    return FUNCTOR_CONTINUE;
}

int Staff::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = vrv_params_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    if (!this->DrawingIsVisible()) {
        return FUNCTOR_SIBLINGS;
    }

    params->m_staffN = this->GetN();

    // this gets (or creates) the measureAligner for the measure
    StaffAlignment *alignment = params->m_systemAligner->GetStaffAlignment(params->m_staffIdx, this, params->m_doc);

    assert(alignment);

    // Set the pointer of the m_alignment
    m_staffAlignment = alignment;

    std::vector<Object *>::iterator it;
    it = std::find_if(m_timeSpanningElements.begin(), m_timeSpanningElements.end(), ObjectComparison(VERSE));
    if (it != m_timeSpanningElements.end()) {
        Verse *v = vrv_cast<Verse *>(*it);
        assert(v);
        alignment->SetVerseCount(v->GetN());
    }

    // for next staff
    params->m_staffIdx++;

    return FUNCTOR_CONTINUE;
}

int Staff::FillStaffCurrentTimeSpanning(FunctorParams *functorParams)
{
    FillStaffCurrentTimeSpanningParams *params = vrv_params_cast<FillStaffCurrentTimeSpanningParams *>(functorParams);
    assert(params);

    std::vector<Object *>::iterator iter = params->m_timeSpanningElements.begin();
    while (iter != params->m_timeSpanningElements.end()) {
        TimeSpanningInterface *interface = (*iter)->GetTimeSpanningInterface();
        assert(interface);
        Measure *currentMeasure = vrv_cast<Measure *>(this->GetFirstAncestor(MEASURE));
        assert(currentMeasure);
        // We need to make sure we are in the next measure (and not just a staff below because of some cross staff
        // notation
        if ((interface->GetStartMeasure() != currentMeasure) && (interface->IsOnStaff(this->GetN()))) {
            m_timeSpanningElements.push_back(*iter);
        }
        ++iter;
    }
    return FUNCTOR_CONTINUE;
}

int Staff::ResetDrawing(FunctorParams *functorParams)
{
    this->m_timeSpanningElements.clear();
    ClearLedgerLines();
    return FUNCTOR_CONTINUE;
}

int Staff::PrepareRpt(FunctorParams *functorParams)
{
    PrepareRptParams *params = vrv_params_cast<PrepareRptParams *>(functorParams);
    assert(params);

    // If multiNumber is set, we already know that nothing needs to be done
    // Futhermore, if @multi.number is false, the functor should have stopped (see below)
    if (params->m_multiNumber != BOOLEAN_NONE) {
        return FUNCTOR_CONTINUE;
    }

    // This is happening only for the first staff element of the staff @n
    if (StaffDef *staffDef = params->m_currentScoreDef->GetStaffDef(this->GetN())) {
        if ((staffDef->HasMultiNumber()) && (staffDef->GetMultiNumber() == BOOLEAN_false)) {
            // Set it just in case, but stopping the functor should do it for this staff @n
            params->m_multiNumber = BOOLEAN_false;
            return FUNCTOR_STOP;
        }
    }
    params->m_multiNumber = BOOLEAN_true;
    return FUNCTOR_CONTINUE;
}

int Staff::CalcOnsetOffset(FunctorParams *functorParams)
{
    CalcOnsetOffsetParams *params = vrv_params_cast<CalcOnsetOffsetParams *>(functorParams);
    assert(params);

    assert(this->m_drawingStaffDef);

    if (this->m_drawingStaffDef->HasNotationtype()) {
        params->m_notationType = this->m_drawingStaffDef->GetNotationtype();
    }
    else {
        params->m_notationType = NOTATIONTYPE_cmn;
    }

    return FUNCTOR_CONTINUE;
}

int Staff::CalcStem(FunctorParams *)
{
    ClassIdComparison isLayer(LAYER);
    ListOfObjects layers;
    this->FindAllDescendantByComparison(&layers, &isLayer);

    if (layers.empty()) {
        return FUNCTOR_CONTINUE;
    }

    // Not more than one layer - drawing stem dir remains unset unless there is cross-staff content
    if (layers.size() < 2) {
        Layer *layer = vrv_cast<Layer *>(layers.front());
        assert(layer);
        if (layer->HasCrossStaffFromBelow()) {
            layer->SetDrawingStemDir(STEMDIRECTION_up);
        }
        else if (layer->HasCrossStaffFromAbove()) {
            layer->SetDrawingStemDir(STEMDIRECTION_down);
        }
        return FUNCTOR_CONTINUE;
    }

    // Detecting empty layers (empty layers can also have @sameas) which have to be ignored for stem direction
    IsEmptyComparison isEmptyElement(LAYER);
    ListOfObjects emptyLayers;
    this->FindAllDescendantByComparison(&emptyLayers, &isEmptyElement);

    // We have only one layer (or less) with content - drawing stem dir remains unset
    if ((layers.size() < 3) && (emptyLayers.size() > 0)) {
        return FUNCTOR_CONTINUE;
    }

    if (!emptyLayers.empty()) {
        ListOfObjects nonEmptyLayers;
        // not need to sort since it already sorted
        std::set_difference(layers.begin(), layers.end(), emptyLayers.begin(), emptyLayers.end(),
            std::inserter(nonEmptyLayers, nonEmptyLayers.begin()));
        layers = nonEmptyLayers;
    }

    for (auto &object : layers) {
        // Alter stem direction between even and odd numbered layers
        Layer *layer = dynamic_cast<Layer *>(object);
        layer->SetDrawingStemDir(layer->GetN() % 2 ? STEMDIRECTION_up : STEMDIRECTION_down);
    }

    return FUNCTOR_CONTINUE;
}

int Staff::AdjustSylSpacing(FunctorParams *functorParams)
{
    AdjustSylSpacingParams *params = vrv_params_cast<AdjustSylSpacingParams *>(functorParams);
    assert(params);

    // Set the staff size for this pass
    params->m_staffSize = this->m_drawingStaffSize;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
