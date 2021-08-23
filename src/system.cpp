/////////////////////////////////////////////////////////////////////////////
// Name:        system.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "system.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "dir.h"
#include "doc.h"
#include "dynam.h"
#include "editorial.h"
#include "ending.h"
#include "functorparams.h"
#include "layer.h"
#include "measure.h"
#include "page.h"
#include "pages.h"
#include "pedal.h"
#include "section.h"
#include "slur.h"
#include "staff.h"
#include "syl.h"
#include "systemboundary.h"
#include "trill.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// System
//----------------------------------------------------------------------------

System::System() : Object("system-"), DrawingListInterface(), AttTyped()
{
    RegisterAttClass(ATT_TYPED);

    // We set parent to it because we want to access the parent doc from the aligners
    m_systemAligner.SetParent(this);

    // owned pointers need to be set to NULL;
    m_drawingScoreDef = NULL;

    Reset();
}

System::~System()
{
    // We need to delete own objects
    Reset();
}

void System::Reset()
{
    Object::Reset();
    DrawingListInterface::Reset();
    ResetTyped();

    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    m_systemLeftMar = 0;
    m_systemRightMar = 0;
    m_xAbs = VRV_UNSET;
    m_drawingXRel = 0;
    m_yAbs = VRV_UNSET;
    m_drawingYRel = 0;
    m_drawingTotalWidth = 0;
    m_drawingJustifiableWidth = 0;
    m_drawingAbbrLabelsWidth = 0;
    m_drawingIsOptimized = false;
}

bool System::IsSupportedChild(Object *child)
{
    if (child->Is(MEASURE)) {
        assert(dynamic_cast<Measure *>(child));
    }
    else if (child->Is(SCOREDEF)) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else if (child->IsSystemElement()) {
        assert(dynamic_cast<SystemElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

int System::GetDrawingX() const
{
    if (m_xAbs != VRV_UNSET) return m_xAbs;

    m_cachedDrawingX = 0;
    return m_drawingXRel;
}

int System::GetDrawingY() const
{
    if (m_yAbs != VRV_UNSET) return m_yAbs;

    m_cachedDrawingY = 0;
    return m_drawingYRel;
}

void System::SetDrawingXRel(int drawingXRel)
{
    ResetCachedDrawingX();
    m_drawingXRel = drawingXRel;
}

void System::SetDrawingYRel(int drawingYRel)
{
    ResetCachedDrawingY();
    m_drawingYRel = drawingYRel;
}

int System::GetHeight() const
{
    if (m_systemAligner.GetBottomAlignment()) {
        return -m_systemAligner.GetBottomAlignment()->GetYRel();
    }
    return 0;
}

int System::GetMinimumSystemSpacing(const Doc *doc) const
{
    const auto &spacingSystem = doc->GetOptions()->m_spacingSystem;
    if (!spacingSystem.IsSet()) {
        assert(m_drawingScoreDef);
        if (m_drawingScoreDef->HasSpacingSystem()) {
            return m_drawingScoreDef->GetSpacingSystem() * doc->GetDrawingUnit(100);
        }
    }

    return spacingSystem.GetValue() * doc->GetDrawingUnit(100);
}

int System::GetDrawingLabelsWidth() const
{
    return (m_drawingScoreDef) ? m_drawingScoreDef->GetDrawingLabelsWidth() : 0;
}

void System::SetDrawingLabelsWidth(int width)
{
    assert(m_drawingScoreDef);
    m_drawingScoreDef->SetDrawingLabelsWidth(width);
}

void System::SetDrawingAbbrLabelsWidth(int width)
{
    if (m_drawingAbbrLabelsWidth < width) {
        m_drawingAbbrLabelsWidth = width;
    }
}

bool System::SetCurrentFloatingPositioner(
    int staffN, FloatingObject *object, Object *objectX, Object *objectY, char spanningType)
{
    assert(object);

    // If we have only the bottom alignment, then nothing to do (yet)
    if (m_systemAligner.GetChildCount() == 1) return false;
    StaffAlignment *alignment = m_systemAligner.GetStaffAlignmentForStaffN(staffN);
    if (!alignment) {
        LogError("Staff @n='%d' for rendering control event %s %s not found", staffN, object->GetClassName().c_str(),
            object->GetUuid().c_str());
        return false;
    }
    alignment->SetCurrentFloatingPositioner(object, objectX, objectY, spanningType);
    return true;
}

void System::SetDrawingScoreDef(ScoreDef *drawingScoreDef)
{
    assert(!m_drawingScoreDef); // We should always call UnscoreDefSetCurrent before

    m_drawingScoreDef = new ScoreDef();
    *m_drawingScoreDef = *drawingScoreDef;
    m_drawingScoreDef->SetParent(this);
}

bool System::HasMixedDrawingStemDir(LayerElement *start, LayerElement *end)
{
    assert(start);
    assert(end);

    // It is too inefficient to look for chord and notes over the entire system
    // We need first to get a list of measures
    Object *measureStart = start->GetFirstAncestor(MEASURE);
    assert(measureStart);
    Object *measureEnd = end->GetFirstAncestor(MEASURE);
    assert(measureEnd);
    ListOfObjects measures;

    // start and end are in the same measure, this is the only one we need
    if (measureStart == measureEnd) {
        measures.push_back(measureStart);
    }
    // otherwise look for a measures in between
    else {
        ClassIdComparison isMeasure(MEASURE);
        Functor findAllBetween(&Object::FindAllBetween);
        FindAllBetweenParams findAllBetweenParams(&isMeasure, &measures, measureStart, measureEnd);
        this->Process(&findAllBetween, &findAllBetweenParams, NULL, NULL, 1);
    }

    // Now we can look for chords and note
    ClassIdsComparison matchType({ CHORD, NOTE });
    ListOfObjects children;
    for (auto &measure : measures) {
        Object *curStart = (measure == measureStart) ? start : measure->GetFirst();
        Object *curEnd = (measure == measureEnd) ? end : measure->GetLast();
        measure->FindAllDescendantBetween(&children, &matchType, curStart, curEnd, false);
    }

    Layer *layerStart = vrv_cast<Layer *>(start->GetFirstAncestor(LAYER));
    assert(layerStart);
    Staff *staffStart = vrv_cast<Staff *>(layerStart->GetFirstAncestor(STAFF));
    assert(staffStart);

    data_STEMDIRECTION stemDir = STEMDIRECTION_NONE;

    for (auto &child : children) {
        Layer *layer = vrv_cast<Layer *>((child)->GetFirstAncestor(LAYER));
        assert(layer);
        Staff *staff = vrv_cast<Staff *>((child)->GetFirstAncestor(STAFF));
        assert(staff);

        // If the slur is spanning over several measure, the the children list will include note and chords
        // from other staves and layers, so we need to skip them.
        // Alternatively we could process by staff / layer, but the current solution might be better
        // if we want to look for slurs starting / ending on different staff / layer
        if ((staff->GetN() != staffStart->GetN()) || (layer->GetN() != layerStart->GetN())) {
            continue;
        }

        StemmedDrawingInterface *interface = dynamic_cast<StemmedDrawingInterface *>(child);
        assert(interface);

        // First pass
        if (stemDir == STEMDIRECTION_NONE) {
            stemDir = interface->GetDrawingStemDir();
        }
        else if (stemDir != interface->GetDrawingStemDir()) {
            return true;
        }
    }

    return false;
}

curvature_CURVEDIR System::GetPreferredCurveDirection(LayerElement *start, LayerElement *end, Slur *slur)
{
    FindSpannedLayerElementsParams findSpannedLayerElementsParams(slur);
    findSpannedLayerElementsParams.m_minPos = start->GetDrawingX();
    findSpannedLayerElementsParams.m_maxPos = end->GetDrawingX();
    findSpannedLayerElementsParams.m_classIds = { CHORD, NOTE };

    Layer *layerStart = vrv_cast<Layer *>(start->GetFirstAncestor(LAYER));
    assert(layerStart);

    Functor findSpannedLayerElements(&Object::FindSpannedLayerElements);
    Process(&findSpannedLayerElements, &findSpannedLayerElementsParams, NULL);

    curvature_CURVEDIR preferredDirection = curvature_CURVEDIR_NONE;
    for (auto element : findSpannedLayerElementsParams.m_elements) {
        Layer *layer = vrv_cast<Layer *>((element)->GetFirstAncestor(LAYER));
        assert(layer);
        if (layer == layerStart) continue;

        if (curvature_CURVEDIR_NONE == preferredDirection) {
            if (layer->GetN() > layerStart->GetN()) {
                preferredDirection = curvature_CURVEDIR_above;
            }
            else {
                preferredDirection = curvature_CURVEDIR_below;
            }
        }
        // if there are layers both above and below - discard previous location and return - we'll use default direction
        else if (((curvature_CURVEDIR_above == preferredDirection) && (layer->GetN() < layerStart->GetN()))
            || ((curvature_CURVEDIR_below == preferredDirection) && (layer->GetN() > layerStart->GetN()))) {
            preferredDirection = curvature_CURVEDIR_NONE;
            break;
        }
    }

    return preferredDirection;
}

void System::AddToDrawingListIfNeccessary(Object *object)
{
    assert(object);

    if (!object->HasInterface(INTERFACE_TIME_SPANNING)) return;

    if (object->Is({ BRACKETSPAN, FIGURE, GLISS, HAIRPIN, LV, OCTAVE, PHRASE, PITCHINFLECTION, SLUR, SYL, TIE })) {
        this->AddToDrawingList(object);
    }
    else if (object->Is(DIR)) {
        Dir *dir = vrv_cast<Dir *>(object);
        assert(dir);
        if (dir->GetEnd() || (dir->GetNextLink() && (dir->GetExtender() == BOOLEAN_true))) {
            this->AddToDrawingList(dir);
        }
    }
    else if (object->Is(DYNAM)) {
        Dynam *dynam = vrv_cast<Dynam *>(object);
        assert(dynam);
        if (dynam->GetEnd() || (dynam->GetNextLink() && (dynam->GetExtender() == BOOLEAN_true))) {
            this->AddToDrawingList(dynam);
        }
    }
    else if (object->Is(PEDAL)) {
        Pedal *pedal = vrv_cast<Pedal *>(object);
        assert(pedal);
        if (pedal->GetEnd()) {
            this->AddToDrawingList(pedal);
        }
    }
    else if (object->Is(TRILL)) {
        Trill *trill = vrv_cast<Trill *>(object);
        assert(trill);
        if (trill->GetEnd() && (trill->GetExtender() != BOOLEAN_false)) {
            this->AddToDrawingList(trill);
        }
    }
}

bool System::IsFirstInPage()
{
    assert(this->GetParent());
    return (this->GetParent()->GetFirst(SYSTEM) == this);
}

bool System::IsLastInPage()
{
    assert(this->GetParent());
    return (this->GetParent()->GetLast(SYSTEM) == this);
}

bool System::IsFirstOfMdiv()
{
    assert(this->GetParent());
    Object *nextSibling = this->GetParent()->GetPrevious(this);
    return (nextSibling && nextSibling->IsPageElement());
}

bool System::IsLastOfMdiv()
{
    assert(this->GetParent());
    Object *nextSibling = this->GetParent()->GetNext(this);
    return (nextSibling && nextSibling->IsPageElement());
}

void System::ConvertToCastOffMensuralSystem(Doc *doc, System *targetSystem)
{
    assert(doc);
    assert(targetSystem);

    // We need to populate processing lists for processing the document by Layer
    PrepareProcessingListsParams prepareProcessingListsParams;
    Functor prepareProcessingLists(&Object::PrepareProcessingLists);
    this->Process(&prepareProcessingLists, &prepareProcessingListsParams);

    // The means no content? Checking just in case
    if (prepareProcessingListsParams.m_layerTree.child.empty()) return;

    ConvertToCastOffMensuralParams convertToCastOffMensuralParams(
        doc, targetSystem, &prepareProcessingListsParams.m_layerTree);
    // Store the list of staff N for detecting barLines that are on all systems
    for (auto const &staves : prepareProcessingListsParams.m_layerTree.child) {
        convertToCastOffMensuralParams.m_staffNs.push_back(staves.first);
    }

    Functor convertToCastOffMensural(&Object::ConvertToCastOffMensural);
    this->Process(&convertToCastOffMensural, &convertToCastOffMensuralParams);
}

void System::ConvertToUnCastOffMensuralSystem()
{
    // We need to populate processing lists for processing the document by Layer
    PrepareProcessingListsParams prepareProcessingListsParams;
    Functor prepareProcessingLists(&Object::PrepareProcessingLists);
    this->Process(&prepareProcessingLists, &prepareProcessingListsParams);

    // The means no content? Checking just in case
    if (prepareProcessingListsParams.m_layerTree.child.empty()) return;

    ConvertToUnCastOffMensuralParams convertToUnCastOffMensuralParams;

    ArrayOfComparisons filters;
    // Now we can process by layer and move their content to (measure) segments
    for (auto const &staves : prepareProcessingListsParams.m_layerTree.child) {
        for (auto const &layers : staves.second.child) {
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves.first);
            AttNIntegerComparison matchLayer(LAYER, layers.first);
            filters = { &matchStaff, &matchLayer };

            convertToUnCastOffMensuralParams.m_contentMeasure = NULL;
            convertToUnCastOffMensuralParams.m_contentLayer = NULL;

            Functor convertToUnCastOffMensural(&Object::ConvertToUnCastOffMensural);
            this->Process(&convertToUnCastOffMensural, &convertToUnCastOffMensuralParams, NULL, &filters);

            convertToUnCastOffMensuralParams.m_addSegmentsToDelete = false;
        }
    }

    // Detach the contentPage
    for (auto &measure : convertToUnCastOffMensuralParams.m_segmentsToDelete) {
        this->DeleteChild(measure);
    }
}

//----------------------------------------------------------------------------
// System functor methods
//----------------------------------------------------------------------------

int System::ScoreDefUnsetCurrent(FunctorParams *functorParams)
{
    if (m_drawingScoreDef) {
        delete m_drawingScoreDef;
        m_drawingScoreDef = NULL;
    }

    m_drawingIsOptimized = false;

    return FUNCTOR_CONTINUE;
}

int System::ScoreDefOptimize(FunctorParams *functorParams)
{
    ScoreDefOptimizeParams *params = vrv_params_cast<ScoreDefOptimizeParams *>(functorParams);
    assert(params);

    this->IsDrawingOptimized(true);

    if (params->m_firstScoreDef) {
        params->m_firstScoreDef = false;
        if (!params->m_doc->GetOptions()->m_condenseFirstPage.GetValue()) {
            return FUNCTOR_SIBLINGS;
        }
    }

    params->m_currentScoreDef = this->GetDrawingScoreDef();
    assert(params->m_currentScoreDef);

    return FUNCTOR_CONTINUE;
}

int System::ScoreDefOptimizeEnd(FunctorParams *functorParams)
{
    ScoreDefOptimizeParams *params = vrv_params_cast<ScoreDefOptimizeParams *>(functorParams);
    assert(params);

    params->m_currentScoreDef->Process(params->m_functor, params, params->m_functorEnd);
    m_systemAligner.SetSpacing(params->m_currentScoreDef);

    return FUNCTOR_CONTINUE;
}

int System::ScoreDefSetGrpSym(FunctorParams *functorParams)
{
    ScoreDefSetGrpSymParams *params = vrv_params_cast<ScoreDefSetGrpSymParams *>(functorParams);
    assert(params);

    if (m_drawingScoreDef) m_drawingScoreDef->Process(params->m_functor, functorParams);

    return FUNCTOR_CONTINUE;
}

int System::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    SetDrawingXRel(0);
    m_drawingAbbrLabelsWidth = 0;

    return FUNCTOR_CONTINUE;
}

int System::ResetVerticalAlignment(FunctorParams *functorParams)
{
    SetDrawingYRel(0);

    m_systemAligner.Reset();

    return FUNCTOR_CONTINUE;
}

int System::ApplyPPUFactor(FunctorParams *functorParams)
{
    ApplyPPUFactorParams *params = vrv_params_cast<ApplyPPUFactorParams *>(functorParams);
    assert(params);

    if (m_xAbs != VRV_UNSET) m_xAbs /= params->m_page->GetPPUFactor();
    if (m_yAbs != VRV_UNSET) m_yAbs /= params->m_page->GetPPUFactor();
    m_systemLeftMar *= params->m_page->GetPPUFactor();
    m_systemRightMar *= params->m_page->GetPPUFactor();

    return FUNCTOR_CONTINUE;
}

int System::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = vrv_params_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    // since we are starting a new system its first scoreDef will need to be a SYSTEM_SCOREDEF
    params->m_isFirstMeasure = true;

    return FUNCTOR_CONTINUE;
}

int System::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = vrv_params_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    params->m_systemAligner = &m_systemAligner;

    return FUNCTOR_CONTINUE;
}

int System::AlignVerticallyEnd(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = vrv_params_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    params->m_cumulatedShift = 0;
    params->m_staffIdx = 0;

    m_systemAligner.Process(params->m_functorEnd, params);

    return FUNCTOR_SIBLINGS;
}

int System::AdjustXOverflow(FunctorParams *functorParams)
{
    AdjustXOverflowParams *params = vrv_params_cast<AdjustXOverflowParams *>(functorParams);
    assert(params);

    params->m_currentSystem = this;
    params->m_lastMeasure = NULL;
    params->m_currentWidest = NULL;

    return FUNCTOR_CONTINUE;
}

int System::AdjustXOverflowEnd(FunctorParams *functorParams)
{
    AdjustXOverflowParams *params = vrv_params_cast<AdjustXOverflowParams *>(functorParams);
    assert(params);

    // Continue if no measure of not widest element
    if (!params->m_lastMeasure || !params->m_currentWidest) {
        return FUNCTOR_CONTINUE;
    }

    // Continue if the right position of the measure is larger than the widest element right
    int measureRightX
        = params->m_lastMeasure->GetDrawingX() + params->m_lastMeasure->GetRightBarLineLeft() - params->m_margin;
    if (measureRightX > params->m_currentWidest->GetContentRight()) {
        return FUNCTOR_CONTINUE;
    }

    LayerElement *objectX = dynamic_cast<LayerElement *>(params->m_currentWidest->GetObjectX());
    if (!objectX) {
        return FUNCTOR_CONTINUE;
    }
    Alignment *left = objectX->GetAlignment();
    Measure *objectXMeasure = dynamic_cast<Measure *>(objectX->GetFirstAncestor(MEASURE));
    if (objectXMeasure != params->m_lastMeasure) {
        left = params->m_lastMeasure->GetLeftBarLine()->GetAlignment();
    }

    int overflow = params->m_currentWidest->GetContentRight() - measureRightX;
    if (overflow > 0) {
        ArrayOfAdjustmentTuples boundaries{ std::make_tuple(
            left, params->m_lastMeasure->GetRightBarLine()->GetAlignment(), overflow) };
        params->m_lastMeasure->m_measureAligner.AdjustProportionally(boundaries);
    }

    return FUNCTOR_CONTINUE;
}

int System::AdjustHarmGrpsSpacing(FunctorParams *functorParams)
{
    AdjustHarmGrpsSpacingParams *params = vrv_params_cast<AdjustHarmGrpsSpacingParams *>(functorParams);
    assert(params);

    // reset it, but not the current grpId!
    params->m_currentSystem = this;
    params->m_overlapingHarm.clear();
    params->m_previousHarmPositioner = NULL;
    params->m_previousHarmStart = NULL;
    params->m_previousMeasure = NULL;

    return FUNCTOR_CONTINUE;
}

int System::AdjustHarmGrpsSpacingEnd(FunctorParams *functorParams)
{
    AdjustHarmGrpsSpacingParams *params = vrv_params_cast<AdjustHarmGrpsSpacingParams *>(functorParams);
    assert(params);

    // End of the first pass - loop over for each group id
    if (params->m_currentGrp == 0) {
        for (auto grpId : params->m_grpIds) {
            params->m_currentGrp = grpId;
            this->Process(params->m_functor, functorParams, params->m_functorEnd);
        }
        // Make sure we reset it for the next system
        params->m_currentGrp = 0;
        return FUNCTOR_CONTINUE;
    }

    /************** End of a system when actually adjusting **************/

    if (!params->m_previousMeasure) {
        return FUNCTOR_CONTINUE;
    }

    // Here we also need to handle the last harm of the measure - we check the alignment with the right barline
    if (params->m_previousHarmPositioner) {
        int overlap = params->m_previousHarmPositioner->GetContentRight()
            - params->m_previousMeasure->GetRightBarLine()->GetAlignment()->GetXRel();

        if (overlap > 0) {
            params->m_overlapingHarm.push_back(std::make_tuple(params->m_previousHarmStart->GetAlignment(),
                params->m_previousMeasure->GetRightBarLine()->GetAlignment(), overlap));
        }
    }

    // Ajust the postion of the alignment according to what we have collected for this harm group id
    params->m_previousMeasure->m_measureAligner.AdjustProportionally(params->m_overlapingHarm);
    params->m_overlapingHarm.clear();

    return FUNCTOR_CONTINUE;
}

int System::AdjustSylSpacing(FunctorParams *functorParams)
{
    AdjustSylSpacingParams *params = vrv_params_cast<AdjustSylSpacingParams *>(functorParams);
    assert(params);

    // reset it
    params->m_overlapingSyl.clear();
    params->m_previousVerse = NULL;
    params->m_previousMeasure = NULL;
    params->m_freeSpace = 0;
    params->m_staffSize = 100;

    return FUNCTOR_CONTINUE;
}

int System::AdjustSylSpacingEnd(FunctorParams *functorParams)
{
    AdjustSylSpacingParams *params = vrv_params_cast<AdjustSylSpacingParams *>(functorParams);
    assert(params);

    if (!params->m_previousMeasure) {
        return FUNCTOR_CONTINUE;
    }

    // Here we also need to handle the last syl of the measure - we check the alignment with the right barline
    if (params->m_previousVerse && params->m_lastSyl) {
        int overlap = params->m_lastSyl->GetContentRight()
            - params->m_previousMeasure->GetRightBarLine()->GetAlignment()->GetXRel();
        params->m_previousVerse->AdjustPosition(overlap, params->m_freeSpace, params->m_doc);

        if (overlap > 0) {
            params->m_overlapingSyl.push_back(std::make_tuple(params->m_previousVerse->GetAlignment(),
                params->m_previousMeasure->GetRightBarLine()->GetAlignment(), overlap));
        }
    }

    // Ajust the postion of the alignment according to what we have collected for this harm grp
    params->m_previousMeasure->m_measureAligner.AdjustProportionally(params->m_overlapingSyl);
    params->m_overlapingSyl.clear();

    return FUNCTOR_CONTINUE;
}

int System::AdjustTempo(FunctorParams *functorParams)
{
    AdjustTempoParams *params = vrv_params_cast<AdjustTempoParams *>(functorParams);
    assert(params);

    params->m_systemAligner = &m_systemAligner;

    return FUNCTOR_CONTINUE;
}

int System::AdjustYPos(FunctorParams *functorParams)
{
    AdjustYPosParams *params = vrv_params_cast<AdjustYPosParams *>(functorParams);
    assert(params);

    // We need to call this explicitly because changing the YRel of the StaffAligner (below in the functor)
    // will not trigger it
    this->ResetCachedDrawingY();

    params->m_cumulatedShift = 0;

    m_systemAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int System::AlignMeasures(FunctorParams *functorParams)
{
    AlignMeasuresParams *params = vrv_params_cast<AlignMeasuresParams *>(functorParams);
    assert(params);

    SetDrawingXRel(m_systemLeftMar + this->GetDrawingLabelsWidth());
    params->m_shift = 0;
    params->m_justifiableWidth = 0;

    return FUNCTOR_CONTINUE;
}

int System::AlignMeasuresEnd(FunctorParams *functorParams)
{
    AlignMeasuresParams *params = vrv_params_cast<AlignMeasuresParams *>(functorParams);
    assert(params);

    m_drawingTotalWidth = params->m_shift + this->GetDrawingLabelsWidth();
    m_drawingJustifiableWidth = params->m_justifiableWidth;

    return FUNCTOR_CONTINUE;
}

int System::AlignSystems(FunctorParams *functorParams)
{
    AlignSystemsParams *params = vrv_params_cast<AlignSystemsParams *>(functorParams);
    assert(params);
    assert(m_systemAligner.GetBottomAlignment());

    int systemMargin = this->IsFirstInPage() ? 0 : params->m_systemMargin;
    if (systemMargin) {
        const int margin
            = systemMargin - (params->m_prevBottomOverflow + m_systemAligner.GetOverflowAbove(params->m_doc));
        params->m_shift -= margin > 0 ? margin : 0;
    }

    SetDrawingYRel(params->m_shift);

    params->m_shift += m_systemAligner.GetBottomAlignment()->GetYRel();

    params->m_justificationSum += m_systemAligner.GetJustificationSum(params->m_doc);
    if (this->IsFirstInPage()) {
        // remove extra system justification factor to get exactly (systemsCount-1)*justificationSystem
        params->m_justificationSum -= params->m_doc->GetOptions()->m_justificationSystem.GetValue();
    }

    params->m_prevBottomOverflow = m_systemAligner.GetOverflowBelow(params->m_doc);

    return FUNCTOR_SIBLINGS;
}

int System::JustifyX(FunctorParams *functorParams)
{
    JustifyXParams *params = vrv_params_cast<JustifyXParams *>(functorParams);
    assert(params);

    params->m_measureXRel = 0;
    int margins = m_systemLeftMar + m_systemRightMar;
    int nonJustifiableWidth
        = margins + (m_drawingTotalWidth - m_drawingJustifiableWidth); // m_drawingTotalWidth includes the labels
    params->m_justifiableRatio
        = (double)(params->m_systemFullWidth - nonJustifiableWidth) / ((double)m_drawingJustifiableWidth);

    if (params->m_justifiableRatio < 0.8) {
        // Arbitrary value for avoiding over-compressed justification
        LogWarning("Justification is highly compressed (ratio smaller than 0.8: %lf)", params->m_justifiableRatio);
        LogWarning("\tSystem full width: %d", params->m_systemFullWidth);
        LogWarning("\tNon-justifiable width: %d", nonJustifiableWidth);
        LogWarning("\tDrawing justifiable width: %d", m_drawingJustifiableWidth);
    }

    // Check if we are on the last system of an mdiv.
    // Do not justify it if the non-justified width is less than a specified percent.
    if (this->IsLastOfMdiv()) {
        double minLastJust = params->m_doc->GetOptions()->m_minLastJustification.GetValue();
        if ((minLastJust > 0) && (params->m_justifiableRatio > (1 / minLastJust))) {
            return FUNCTOR_STOP;
        }
    }

    return FUNCTOR_CONTINUE;
}

int System::JustifyY(FunctorParams *functorParams)
{
    JustifyYParams *params = vrv_params_cast<JustifyYParams *>(functorParams);
    assert(params);
    if (params->m_justificationSum <= 0.0) return FUNCTOR_STOP;
    if (params->m_spaceToDistribute <= 0) return FUNCTOR_STOP;

    const double systemJustificationFactor = params->m_doc->GetOptions()->m_justificationSystem.GetValue();
    const double shift = systemJustificationFactor / params->m_justificationSum * params->m_spaceToDistribute;

    if (!this->IsFirstInPage()) {
        params->m_cumulatedShift += shift;
    }

    this->SetDrawingYRel(this->GetDrawingY() - params->m_cumulatedShift);

    params->m_relativeShift = 0;
    m_systemAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int System::AdjustStaffOverlap(FunctorParams *functorParams)
{
    AdjustStaffOverlapParams *params = vrv_params_cast<AdjustStaffOverlapParams *>(functorParams);
    assert(params);

    params->m_previous = NULL;
    m_systemAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int System::AdjustFloatingPositioners(FunctorParams *functorParams)
{
    AdjustFloatingPositionersParams *params = vrv_params_cast<AdjustFloatingPositionersParams *>(functorParams);
    assert(params);

    params->m_inBetween = false;

    AdjustFloatingPositionerGrpsParams adjustFloatingPositionerGrpsParams(params->m_doc);
    Functor adjustFloatingPositionerGrps(&Object::AdjustFloatingPositionerGrps);

    params->m_classId = GLISS;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = LV;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = TIE;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = SLUR;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = PHRASE;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = MORDENT;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = TURN;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = TRILL;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = DYNAM;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = HAIRPIN;
    m_systemAligner.Process(params->m_functor, params);

    adjustFloatingPositionerGrpsParams.m_classIds.clear();
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(DYNAM);
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(HAIRPIN);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_above;
    m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_below;
    m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);

    params->m_classId = BRACKETSPAN;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = OCTAVE;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = BREATH;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = FERMATA;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = FING;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = DIR;
    m_systemAligner.Process(params->m_functor, params);

    adjustFloatingPositionerGrpsParams.m_classIds.clear();
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(DIR);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_above;
    m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_below;
    m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);

    params->m_classId = TEMPO;
    m_systemAligner.Process(params->m_functor, params);

    params->m_classId = PEDAL;
    m_systemAligner.Process(params->m_functor, params);

    adjustFloatingPositionerGrpsParams.m_classIds.clear();
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(PEDAL);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_above;
    m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_below;
    m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);

    params->m_classId = HARM;
    m_systemAligner.Process(params->m_functor, params);

    adjustFloatingPositionerGrpsParams.m_classIds.clear();
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(HARM);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_above;
    m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_below;
    m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);

    params->m_classId = ENDING;
    m_systemAligner.Process(params->m_functor, params);

    adjustFloatingPositionerGrpsParams.m_classIds.clear();
    adjustFloatingPositionerGrpsParams.m_classIds.push_back(ENDING);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_above;
    m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);
    adjustFloatingPositionerGrpsParams.m_place = STAFFREL_below;
    m_systemAligner.Process(&adjustFloatingPositionerGrps, &adjustFloatingPositionerGrpsParams);

    params->m_classId = REH;
    m_systemAligner.Process(params->m_functor, params);

    // SYL check if they are some lyrics and make space for them if any
    params->m_classId = SYL;
    m_systemAligner.Process(params->m_functor, params);

    /**** Process elements that needs to be put in between ****/

    params->m_inBetween = true;
    // All of them with no particular processing order.
    // The resulting layout order will correspond to the order in the encoding.
    params->m_classId = OBJECT;
    m_systemAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int System::AdjustFloatingPositionersBetween(FunctorParams *functorParams)
{
    AdjustFloatingPositionersBetweenParams *params
        = vrv_params_cast<AdjustFloatingPositionersBetweenParams *>(functorParams);
    assert(params);

    params->m_previousStaffPositioners = NULL;
    params->m_previousStaffAlignment = NULL;
    m_systemAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int System::AdjustSlurs(FunctorParams *functorParams)
{
    AdjustSlursParams *params = vrv_params_cast<AdjustSlursParams *>(functorParams);
    assert(params);

    m_systemAligner.Process(params->m_functor, params);

    return FUNCTOR_SIBLINGS;
}

int System::CastOffPages(FunctorParams *functorParams)
{
    CastOffPagesParams *params = vrv_params_cast<CastOffPagesParams *>(functorParams);
    assert(params);

    int currentShift = params->m_shift;
    // We use params->m_pageHeadHeight to check if we have passed the first page already
    if (params->m_pgHeadHeight != VRV_UNSET) {
        currentShift += params->m_pgHeadHeight + params->m_pgFootHeight;
    }
    else {
        currentShift += params->m_pgHead2Height + params->m_pgFoot2Height;
    }

    const int systemMaxPerPage = params->m_doc->GetOptions()->m_systemMaxPerPage.GetValue();
    const int childCount = params->m_currentPage->GetChildCount();
    if ((systemMaxPerPage && systemMaxPerPage == childCount)
        || (childCount > 0 && (this->m_drawingYRel - this->GetHeight() - currentShift < 0))) {
        // If this is last system in the list, it doesn't fit the page and it's leftover system (has just one last
        // measure) - get measure out of that system and try adding it to the previous system
        Object *nextSystem = params->m_contentPage->GetNext(this, SYSTEM);
        if ((NULL == nextSystem) && (this == params->m_leftoverSystem)) {
            Measure *measure = dynamic_cast<Measure *>(Relinquish(GetFirst(MEASURE)->GetIdx()));
            System *lastSystem = dynamic_cast<System *>(params->m_currentPage->GetLast());
            if (measure && lastSystem) lastSystem->AddChild(measure);
            return FUNCTOR_SIBLINGS;
        }
        else {
            params->m_currentPage = new Page();
            // Use VRV_UNSET value as a flag
            params->m_pgHeadHeight = VRV_UNSET;
            assert(params->m_doc->GetPages());
            params->m_doc->GetPages()->AddChild(params->m_currentPage);
            params->m_shift = this->m_drawingYRel - params->m_pageHeight;
        }
    }

    // First add all pending objects
    ArrayOfObjects::iterator iter;
    for (iter = params->m_pendingPageElements.begin(); iter != params->m_pendingPageElements.end(); ++iter) {
        params->m_currentPage->AddChild(*iter);
    }
    params->m_pendingPageElements.clear();

    // Special case where we use the Relinquish method.
    // We want to move the system to the currentPage. However, we cannot use DetachChild
    // from the contentPage because this screws up the iterator. Relinquish gives up
    // the ownership of the system - the contentPage itself will be deleted afterwards.
    System *system = vrv_cast<System *>(params->m_contentPage->Relinquish(this->GetIdx()));
    assert(system);
    params->m_currentPage->AddChild(system);

    return FUNCTOR_SIBLINGS;
}

int System::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // We are starting a new system we need to cast off
    params->m_contentSystem = this;
    // We also need to create a new target system and add it to the page
    System *system = new System();
    params->m_page->AddChild(system);
    params->m_currentSystem = system;

    params->m_shift = -this->GetDrawingLabelsWidth();
    params->m_currentScoreDefWidth
        = params->m_page->m_drawingScoreDef.GetDrawingWidth() + this->GetDrawingAbbrLabelsWidth();

    return FUNCTOR_CONTINUE;
}

int System::CastOffSystemsEnd(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    if (params->m_pendingElements.empty()) return FUNCTOR_CONTINUE;

    // Otherwise add all pendings objects
    ArrayOfObjects::iterator iter;
    for (iter = params->m_pendingElements.begin(); iter != params->m_pendingElements.end(); ++iter) {
        params->m_currentSystem->AddChild(*iter);
    }

    return FUNCTOR_CONTINUE;
}

int System::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    // We are starting a new system we need to cast off
    params->m_contentSystem = this;
    // Create the new system but do not add it to the page yet.
    // It will be added when reaching a pb / sb or at the end of the score in PageElementEnd::CastOffEncoding
    assert(!params->m_currentSystem);
    params->m_currentSystem = new System();

    return FUNCTOR_CONTINUE;
}

int System::UnCastOff(FunctorParams *functorParams)
{
    UnCastOffParams *params = vrv_params_cast<UnCastOffParams *>(functorParams);
    assert(params);

    // Just move all the content of the system to the continuous one (parameter)
    // Use the MoveChildrenFrom method that moves and relinquishes them
    // See Object::Relinquish
    params->m_currentSystem->MoveChildrenFrom(this);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
