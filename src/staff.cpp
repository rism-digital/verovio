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
#include "tuning.h"
#include "verse.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

static const ClassRegistrar<Staff> s_factory("staff", STAFF);

Staff::Staff(int n) : Object(STAFF, "staff-"), FacsimileInterface(), AttNInteger(), AttTyped(), AttVisibility()
{
    RegisterAttClass(ATT_NINTEGER);
    RegisterAttClass(ATT_TYPED);
    RegisterAttClass(ATT_VISIBILITY);
    RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());

    Reset();
    SetN(n);
}

Staff::~Staff() {}

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
    m_drawingTuning = NULL;

    ClearLedgerLines();
}

void Staff::CloneReset()
{
    Object::CloneReset();

    m_drawingStaffSize = 100;
    m_drawingLines = 5;
    m_drawingNotationType = NOTATIONTYPE_NONE;
    m_staffAlignment = NULL;
    m_timeSpanningElements.clear();
    m_drawingStaffDef = NULL;
    m_drawingTuning = NULL;
}

const ArrayOfObjects *Staff::GetChildren(bool docChildren) const
{
    return Object::GetChildren(true);
}

void Staff::ClearLedgerLines()
{
    m_ledgerLinesAbove.clear();
    m_ledgerLinesBelow.clear();
    m_ledgerLinesAboveCue.clear();
    m_ledgerLinesBelowCue.clear();
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
            m_drawingStaffSize = 100 * yDiff / (doc->GetOptions()->m_unit.GetValue() * 2 * (m_drawingLines - 1));
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

bool Staff::IsMensural()
{
    bool isMensural
        = (m_drawingNotationType == NOTATIONTYPE_mensural || m_drawingNotationType == NOTATIONTYPE_mensural_white
            || m_drawingNotationType == NOTATIONTYPE_mensural_black);
    return isMensural;
}

bool Staff::IsNeume()
{
    bool isNeume = (m_drawingNotationType == NOTATIONTYPE_neume);
    return isNeume;
}

bool Staff::IsTablature()
{
    bool isTablature = (m_drawingNotationType == NOTATIONTYPE_tab || m_drawingNotationType == NOTATIONTYPE_tab_guitar
        || m_drawingNotationType == NOTATIONTYPE_tab_lute_italian
        || m_drawingNotationType == NOTATIONTYPE_tab_lute_french
        || m_drawingNotationType == NOTATIONTYPE_tab_lute_german);
    return isTablature;
}

int Staff::CalcPitchPosYRel(Doc *doc, int loc)
{
    assert(doc);

    // the staff loc offset is based on the number of lines: 0 with 1 line, 2 with 2, etc
    int staffLocOffset = (m_drawingLines - 1) * 2;
    return (loc - staffLocOffset) * doc->GetDrawingUnit(m_drawingStaffSize);
}

void Staff::AddLedgerLineAbove(int count, int left, int right, int extension, bool cueSize)
{
    AddLedgerLines(cueSize ? m_ledgerLinesAboveCue : m_ledgerLinesAbove, count, left, right, extension);
}

void Staff::AddLedgerLineBelow(int count, int left, int right, int extension, bool cueSize)
{
    AddLedgerLines(cueSize ? m_ledgerLinesBelowCue : m_ledgerLinesBelow, count, left, right, extension);
}

void Staff::AddLedgerLines(ArrayOfLedgerLines &lines, int count, int left, int right, int extension)
{
    assert(left < right);

    if ((int)lines.size() < count) lines.resize(count);
    int i = 0;
    for (i = 0; i < count; ++i) {
        lines.at(i).AddDash(left, right, extension);
    }
}

void Staff::AdjustLedgerLines(
    ArrayOfLedgerLines &lines, ArrayOfLedgerLines &cueLines, double cueScaling, int extension, int minExtension)
{
    assert(minExtension <= extension);

    // By construction, any overlaps or small gaps in outer dash lines must also occur in the most inner dash line.
    // Thus it suffices to resolve any problems in the inner dash line and apply the adjustments to corresponding
    // dashes further away from the staff.

    struct Adjustment {
        int left; // the left boundary of the dash
        int right; // the right boundary of the dash
        bool isCue; // whether the dash is cue
        int delta; // the shortening which will be applied on both sides
    };

    // For each dash on the inner line (both cue and normal) we construct an adjustment with zero delta
    // and sort them
    std::vector<Adjustment> adjustments;
    using DashType = std::pair<int, int>;
    if (!lines.empty()) {
        for (const DashType &dash : lines.at(0).m_dashes) {
            adjustments.push_back({ dash.first, dash.second, false, 0 });
        }
    }
    if (!cueLines.empty()) {
        for (const DashType &dash : cueLines.at(0).m_dashes) {
            adjustments.push_back({ dash.first, dash.second, true, 0 });
        }
    }

    std::sort(adjustments.begin(), adjustments.end(), [](const Adjustment &adj1, const Adjustment &adj2) {
        if (adj1.left < adj2.left) return true;
        return ((adj1.left == adj2.left) && (adj1.right < adj2.right));
    });

    // By comparing successive dashes we compute the necessary adjustment (delta) for each of them
    const int defaultGap = 100 * extension; // A large value which should not trigger any adjustments
    int leftGapProportion = defaultGap; // The proportion of the left gap belonging to the current dash
    int rightGapProportion = defaultGap; // The proportion of the right gap belonging to the current dash

    using IterType = std::vector<Adjustment>::iterator;
    for (IterType iterAdj = adjustments.begin(); iterAdj != adjustments.end(); ++iterAdj) {
        // Calculate the right gap and whether the next dash is cue
        IterType iterNextAdj = std::next(iterAdj);
        const int rightGap = (iterNextAdj != adjustments.end()) ? iterNextAdj->left - iterAdj->right : defaultGap;
        const bool nextIsCue = (iterNextAdj != adjustments.end()) ? iterNextAdj->isCue : false;

        // Calculate the proportion of the right gap belonging to the current dash and the proportion belonging to the
        // next dash
        const double currentCueScale = (iterAdj->isCue) ? cueScaling : 1.0;
        const double nextCueScale = nextIsCue ? cueScaling : 1.0;
        rightGapProportion = currentCueScale / (currentCueScale + nextCueScale) * rightGap;
        const int nextLeftGapProportion = nextCueScale / (currentCueScale + nextCueScale) * rightGap;

        // The gap between successive dashes should be at least one extension
        const int minGapProportion = std::min(leftGapProportion, rightGapProportion);
        if (minGapProportion < currentCueScale * extension / 2.0) {
            const int minTotal = minGapProportion + currentCueScale * extension;
            const int newExtension = std::max<int>(2 * minTotal / 3, currentCueScale * minExtension);
            iterAdj->delta = currentCueScale * extension - newExtension;
            assert(iterAdj->delta >= 0);
        }

        leftGapProportion = nextLeftGapProportion;
    }

    // Finally, we transfer the adjustments to all ledger lines
    // We thus ensure that all dashes on the same note/chord obtain the same ledger line extension
    for (const Adjustment &adjustment : adjustments) {
        if (adjustment.delta > 0) {
            ArrayOfLedgerLines &linesToAdjust = adjustment.isCue ? cueLines : lines;
            for (LedgerLine &line : linesToAdjust) {
                std::list<DashType>::iterator iterDash
                    = std::find_if(line.m_dashes.begin(), line.m_dashes.end(), [&adjustment](const DashType &dash) {
                          return ((dash.first >= adjustment.left) && (dash.second <= adjustment.right));
                      });
                if (iterDash != line.m_dashes.end()) {
                    iterDash->first += adjustment.delta;
                    iterDash->second -= adjustment.delta;
                }
            }
        }
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

bool Staff::IsOnStaffLine(int y, Doc *doc)
{
    assert(doc);

    return ((y - this->GetDrawingY()) % (2 * doc->GetDrawingUnit(m_drawingStaffSize)) == 0);
}

int Staff::GetNearestInterStaffPosition(int y, Doc *doc, data_STAFFREL place)
{
    assert(doc);

    int yPos = y - this->GetDrawingY();
    int distance = yPos % doc->GetDrawingUnit(m_drawingStaffSize);
    if (place == STAFFREL_above) {
        if (distance > 0) distance = doc->GetDrawingUnit(m_drawingStaffSize) - distance;
        return y - distance + doc->GetDrawingUnit(m_drawingStaffSize);
    }
    else {
        if (distance < 0) distance = doc->GetDrawingUnit(m_drawingStaffSize) + distance;
        return y - distance - doc->GetDrawingUnit(m_drawingStaffSize);
    }
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

void LedgerLine::AddDash(int left, int right, int extension)
{
    assert(left < right);

    std::list<std::pair<int, int>>::iterator iter;

    // First add the dash
    for (iter = m_dashes.begin(); iter != m_dashes.end(); ++iter) {
        if (iter->first > left) break;
    }
    m_dashes.insert(iter, std::make_pair(left, right));

    // Merge dashes which overlap by more than 1.5 extensions
    // => Dashes belonging to the same chord overlap at least by two extensions and will get merged
    // => Overlapping dashes of adjacent notes will not get merged
    std::list<std::pair<int, int>>::iterator previous = m_dashes.begin();
    iter = m_dashes.begin();
    ++iter;
    while (iter != m_dashes.end()) {
        if (previous->second > iter->first + 1.5 * extension) {
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

int Staff::ScoreDefUnsetCurrent(FunctorParams *functorParams)
{
    m_drawingStaffDef = NULL;
    m_drawingTuning = NULL;

    return FUNCTOR_CONTINUE;
}

int Staff::ScoreDefOptimize(FunctorParams *functorParams)
{
    ScoreDefOptimizeParams *params = vrv_params_cast<ScoreDefOptimizeParams *>(functorParams);
    assert(params);

    assert(params->m_currentScoreDef);
    StaffDef *staffDef = params->m_currentScoreDef->GetStaffDef(this->GetN());

    if (!staffDef) {
        LogDebug(
            "Could not find staffDef for staff (%d) when optimizing scoreDef in Staff::ScoreDefOptimize", this->GetN());
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
    IsEmptyComparison matchTypeLayer(LAYER);
    matchTypeLayer.ReverseComparison();
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

    assert(m_drawingStaffDef);

    if (m_drawingStaffDef->HasNotationtype()) {
        params->m_notationType = m_drawingStaffDef->GetNotationtype();
    }
    else {
        params->m_notationType = NOTATIONTYPE_cmn;
    }
    Measure *parentMeasure = vrv_cast<Measure *>(GetFirstAncestor(MEASURE));
    if (parentMeasure) m_drawingStaffDef->AlternateCurrentMeterSig(parentMeasure);

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
        alignment->AddVerseN(v->GetN());
    }

    // for next staff
    params->m_staffIdx++;

    return FUNCTOR_CONTINUE;
}

int Staff::CalcLedgerLinesEnd(FunctorParams *functorParams)
{
    FunctorDocParams *params = vrv_params_cast<FunctorDocParams *>(functorParams);
    assert(params);

    const int extension = params->m_doc->GetDrawingLedgerLineExtension(m_drawingStaffSize, false);
    const int minExtension = params->m_doc->GetDrawingMinimalLedgerLineExtension(m_drawingStaffSize, false);
    const double cueScaling = params->m_doc->GetCueScaling();
    AdjustLedgerLines(m_ledgerLinesAbove, m_ledgerLinesAboveCue, cueScaling, extension, minExtension);
    AdjustLedgerLines(m_ledgerLinesBelow, m_ledgerLinesBelowCue, cueScaling, extension, minExtension);

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
    m_timeSpanningElements.clear();
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
    if (StaffDef *staffDef = params->m_doc->GetCurrentScoreDef()->GetStaffDef(this->GetN())) {
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

    assert(m_drawingStaffDef);

    if (m_drawingStaffDef->HasNotationtype()) {
        params->m_notationType = m_drawingStaffDef->GetNotationtype();
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
    params->m_staffSize = m_drawingStaffSize;

    return FUNCTOR_CONTINUE;
}

int Staff::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    params->m_expandedNotes.clear();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
