/////////////////////////////////////////////////////////////////////////////
// Name:        layer.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "layer.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "beam.h"
#include "clef.h"
#include "comparison.h"
#include "custos.h"
#include "doc.h"
#include "editorial.h"
#include "functorparams.h"
#include "keysig.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mrpt.h"
#include "note.h"
#include "staff.h"
#include "staffdef.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Layer
//----------------------------------------------------------------------------

Layer::Layer()
    : Object("layer-"), DrawingListInterface(), ObjectListInterface(), AttNInteger(), AttTyped(), AttVisibility()
{
    RegisterAttClass(ATT_NINTEGER);
    RegisterAttClass(ATT_TYPED);
    RegisterAttClass(ATT_VISIBILITY);

    // owned pointers need to be set to NULL;
    m_staffDefClef = NULL;
    m_staffDefKeySig = NULL;
    m_staffDefMensur = NULL;
    m_staffDefMeterSig = NULL;
    m_cautionStaffDefClef = NULL;
    m_cautionStaffDefKeySig = NULL;
    m_cautionStaffDefMensur = NULL;
    m_cautionStaffDefMeterSig = NULL;

    Reset();
}

Layer::~Layer()
{
    // We need to delete own objects
    Reset();
}

void Layer::Reset()
{
    Object::Reset();
    DrawingListInterface::Reset();
    ResetNInteger();
    ResetTyped();
    ResetVisibility();

    ResetStaffDefObjects();

    m_drawingStemDir = STEMDIRECTION_NONE;
    m_crossStaffFromAbove = false;
    m_crossStaffFromBelow = false;
}

void Layer::CloneReset()
{
    Object::CloneReset();

    m_drawKeySigCancellation = false;
    m_staffDefClef = NULL;
    m_staffDefKeySig = NULL;
    m_staffDefMensur = NULL;
    m_staffDefMeterSig = NULL;
    m_drawCautionKeySigCancel = false;
    m_cautionStaffDefClef = NULL;
    m_cautionStaffDefKeySig = NULL;
    m_cautionStaffDefMensur = NULL;
    m_cautionStaffDefMeterSig = NULL;

    m_drawingStemDir = STEMDIRECTION_NONE;
    m_crossStaffFromAbove = false;
    m_crossStaffFromBelow = false;
}

void Layer::ResetStaffDefObjects()
{
    m_drawKeySigCancellation = false;
    if (m_staffDefClef) {
        delete m_staffDefClef;
        m_staffDefClef = NULL;
    }
    if (m_staffDefKeySig) {
        delete m_staffDefKeySig;
        m_staffDefKeySig = NULL;
    }
    if (m_staffDefMensur) {
        delete m_staffDefMensur;
        m_staffDefMensur = NULL;
    }
    if (m_staffDefMeterSig) {
        delete m_staffDefMeterSig;
        m_staffDefMeterSig = NULL;
    }
    // cautionary values
    m_drawCautionKeySigCancel = false;
    if (m_cautionStaffDefClef) {
        delete m_cautionStaffDefClef;
        m_cautionStaffDefClef = NULL;
    }
    if (m_cautionStaffDefKeySig) {
        delete m_cautionStaffDefKeySig;
        m_cautionStaffDefKeySig = NULL;
    }
    if (m_cautionStaffDefMensur) {
        delete m_cautionStaffDefMensur;
        m_cautionStaffDefMensur = NULL;
    }
    if (m_cautionStaffDefMeterSig) {
        delete m_cautionStaffDefMeterSig;
        m_cautionStaffDefMeterSig = NULL;
    }
}

bool Layer::IsSupportedChild(Object *child)
{
    if (child->IsLayerElement()) {
        assert(dynamic_cast<LayerElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

LayerElement *Layer::GetPrevious(LayerElement *element)
{
    this->ResetList(this);
    if (!element || this->GetList(this)->empty()) return NULL;

    return dynamic_cast<LayerElement *>(GetListPrevious(element));
}

LayerElement *Layer::GetAtPos(int x)
{
    Object *first = this->GetFirst();
    if (!first || !first->IsLayerElement()) return NULL;

    LayerElement *element = vrv_cast<LayerElement *>(first);
    assert(element);
    if (element->GetDrawingX() > x) return NULL;

    Object *next;
    while ((next = this->GetNext())) {
        if (!next->IsLayerElement()) continue;
        LayerElement *nextLayerElement = vrv_cast<LayerElement *>(next);
        assert(nextLayerElement);
        if (nextLayerElement->GetDrawingX() > x) return element;
        element = nextLayerElement;
    }
    // This can be NULL if the layer is empty
    return element;
}

Clef *Layer::GetClef(LayerElement *test)
{
    Object *testObject = test;

    if (!test) {
        return GetCurrentClef();
    }

    // make sure list is set
    ResetList(this);
    if (!test->Is(CLEF)) {
        testObject = GetListFirstBackward(testObject, CLEF);
    }

    if (testObject && testObject->Is(CLEF)) {
        Clef *clef = vrv_cast<Clef *>(testObject);
        assert(clef);
        return clef;
    }
    Clef *facsClef = this->GetClefFacs(test);
    if (facsClef != NULL) {
        return facsClef;
    }
    return GetCurrentClef();
}

Clef *Layer::GetClefFacs(LayerElement *test)
{
    Doc *doc = vrv_cast<Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);
    if (doc->GetType() == Facs) {
        ListOfObjects clefs;
        ClassIdComparison ac(CLEF);
        doc->FindAllDescendantBetween(&clefs, &ac, doc->GetFirst(CLEF), test);
        if (clefs.size() > 0) {
            return dynamic_cast<Clef *>(*clefs.rbegin());
        }
    }
    return NULL;
}

int Layer::GetClefLocOffset(LayerElement *test)
{
    Clef *clef = GetClef(test);
    if (!clef) return 0;
    return clef->GetClefLocOffset();
}

data_STEMDIRECTION Layer::GetDrawingStemDir(LayerElement *element)
{
    assert(element);

    if (this->GetLayerCountForTimeSpanOf(element) < 2) {
        return STEMDIRECTION_NONE;
    }
    else {
        // If the element is cross-staff, then do not use the layer m_drawingStemDir
        if (element->m_crossStaff) {
            // Instead look if the cross-staff is from below or above
            if (this->m_crossStaffFromBelow) {
                return STEMDIRECTION_down;
            }
            else if (this->m_crossStaffFromAbove) {
                return STEMDIRECTION_up;
            }
            else {
                // This should actually not happen
                return STEMDIRECTION_NONE;
            }
        }
        return m_drawingStemDir;
    }
}

data_STEMDIRECTION Layer::GetDrawingStemDir(const ArrayOfBeamElementCoords *coords)
{
    assert(!coords->empty());

    // Adjust the x position of the first and last element for taking into account the stem width
    LayerElement *first = dynamic_cast<LayerElement *>(coords->front()->m_element);
    LayerElement *last = dynamic_cast<LayerElement *>(coords->back()->m_element);

    if (!first || !last) {
        return m_drawingStemDir;
    }

    Measure *measure = vrv_cast<Measure *>(this->GetFirstAncestor(MEASURE));
    assert(measure);

    Alignment *alignmentFirst = first->GetAlignment();
    assert(alignmentFirst);
    Alignment *alignmentLast = last->GetAlignment();
    assert(alignmentLast);

    // We are ignoring cross-staff situation here because this should not be called if we have one
    Staff *staff = vrv_cast<Staff *>(first->GetFirstAncestor(STAFF));
    assert(staff);

    double time = alignmentFirst->GetTime();
    double duration = alignmentLast->GetTime() - time + last->GetAlignmentDuration();
    duration = durRound(duration);

    if (this->GetLayerCountInTimeSpan(time, duration, measure, staff->GetN()) < 2) {
        return STEMDIRECTION_NONE;
    }
    else {
        return m_drawingStemDir;
    }
}

int Layer::GetLayerCountForTimeSpanOf(LayerElement *element)
{
    assert(element);

    Measure *measure = vrv_cast<Measure *>(this->GetFirstAncestor(MEASURE));
    assert(measure);

    Alignment *alignment = element->GetAlignment();
    assert(alignment);

    Layer *layer = NULL;
    Staff *staff = element->GetCrossStaff(layer);
    if (!staff) {
        staff = dynamic_cast<Staff *>(element->GetFirstAncestor(STAFF));
    }
    // At this stage we have the parent or the cross-staff
    assert(staff);

    return this->GetLayerCountInTimeSpan(alignment->GetTime(), element->GetAlignmentDuration(), measure, staff->GetN());
}

int Layer::GetLayerCountInTimeSpan(double time, double duration, Measure *measure, int staff)
{
    assert(measure);

    Functor layerCountInTimeSpan(&Object::LayerCountInTimeSpan);
    LayerCountInTimeSpanParams layerCountInTimeSpanParams(
        GetCurrentMeterSig(), GetCurrentMensur(), &layerCountInTimeSpan);
    layerCountInTimeSpanParams.m_time = time;
    layerCountInTimeSpanParams.m_duration = duration;

    ArrayOfComparisons filters;
    AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staff);
    filters.push_back(&matchStaff);

    measure->m_measureAligner.Process(&layerCountInTimeSpan, &layerCountInTimeSpanParams, NULL, &filters);

    return (int)layerCountInTimeSpanParams.m_layers.size();
}

ListOfObjects Layer::GetLayerElementsForTimeSpanOf(LayerElement *element)
{
    assert(element);

    Measure *measure = static_cast<Measure *>(this->GetFirstAncestor(MEASURE));
    assert(measure);

    Alignment *alignment = element->GetAlignment();
    assert(alignment);

    Layer *layer = NULL;
    Staff *staff = element->GetCrossStaff(layer);
    if (!staff) {
        staff = static_cast<Staff *>(element->GetFirstAncestor(STAFF));
    }
    // At this stage we have the parent or the cross-staff
    assert(staff);

    return GetLayerElementsInTimeSpan(alignment->GetTime(), element->GetAlignmentDuration(), measure, staff->GetN());
}

ListOfObjects Layer::GetLayerElementsInTimeSpan(double time, double duration, Measure *measure, int staff)
{
    assert(measure);

    Functor layerElementsInTimeSpan(&Object::LayerElementsInTimeSpan);
    LayerElementsInTimeSpanParams layerElementsInTimeSpanParams(GetCurrentMeterSig(), GetCurrentMensur(), this);
    layerElementsInTimeSpanParams.m_time = time;
    layerElementsInTimeSpanParams.m_duration = duration;

    ArrayOfComparisons filters;
    AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staff);
    filters.push_back(&matchStaff);

    measure->m_measureAligner.Process(&layerElementsInTimeSpan, &layerElementsInTimeSpanParams, NULL, &filters);

    return layerElementsInTimeSpanParams.m_elements;
}

Clef *Layer::GetCurrentClef() const
{
    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff && staff->m_drawingStaffDef && staff->m_drawingStaffDef->GetCurrentClef());
    return staff->m_drawingStaffDef->GetCurrentClef();
}

KeySig *Layer::GetCurrentKeySig() const
{
    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff && staff->m_drawingStaffDef);
    return staff->m_drawingStaffDef->GetCurrentKeySig();
}

Mensur *Layer::GetCurrentMensur() const
{
    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff && staff->m_drawingStaffDef);
    return staff->m_drawingStaffDef->GetCurrentMensur();
}

MeterSig *Layer::GetCurrentMeterSig() const
{
    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff && staff->m_drawingStaffDef);
    return staff->m_drawingStaffDef->GetCurrentMeterSig();
}

void Layer::SetDrawingStaffDefValues(StaffDef *currentStaffDef)
{
    if (!currentStaffDef) {
        LogDebug("staffDef not found");
        return;
    }

    // Remove any previous value in the Layer
    this->ResetStaffDefObjects();

    if (currentStaffDef->DrawClef()) {
        this->m_staffDefClef = new Clef(*currentStaffDef->GetCurrentClef());
        this->m_staffDefClef->SetParent(this);
    }
    if (currentStaffDef->DrawKeySig()) {
        this->m_staffDefKeySig = new KeySig(*currentStaffDef->GetCurrentKeySig());
        this->m_staffDefKeySig->SetParent(this);
    }
    if (currentStaffDef->DrawMensur()) {
        this->m_staffDefMensur = new Mensur(*currentStaffDef->GetCurrentMensur());
        this->m_staffDefMensur->SetParent(this);
    }
    if (currentStaffDef->DrawMeterSig()) {
        this->m_staffDefMeterSig = new MeterSig(*currentStaffDef->GetCurrentMeterSig());
        this->m_staffDefMeterSig->SetParent(this);
    }

    // Don't draw on the next one
    currentStaffDef->SetDrawClef(false);
    currentStaffDef->SetDrawKeySig(false);
    currentStaffDef->SetDrawMensur(false);
    currentStaffDef->SetDrawMeterSig(false);
}

void Layer::SetDrawingCautionValues(StaffDef *currentStaffDef)
{
    if (!currentStaffDef) {
        LogDebug("staffDef not found");
        return;
    }

    if (currentStaffDef->DrawClef()) {
        this->m_cautionStaffDefClef = new Clef(*currentStaffDef->GetCurrentClef());
        this->m_cautionStaffDefClef->SetParent(this);
    }
    // special case - see above
    if (currentStaffDef->DrawKeySig()) {
        this->m_cautionStaffDefKeySig = new KeySig(*currentStaffDef->GetCurrentKeySig());
        this->m_cautionStaffDefKeySig->SetParent(this);
    }
    if (currentStaffDef->DrawMensur()) {
        this->m_cautionStaffDefMensur = new Mensur(*currentStaffDef->GetCurrentMensur());
        this->m_cautionStaffDefMensur->SetParent(this);
    }
    if (currentStaffDef->DrawMeterSig()) {
        this->m_cautionStaffDefMeterSig = new MeterSig(*currentStaffDef->GetCurrentMeterSig());
        this->m_cautionStaffDefMeterSig->SetParent(this);
    }

    // Don't draw on the next one
    currentStaffDef->SetDrawClef(false);
    currentStaffDef->SetDrawKeySig(false);
    currentStaffDef->SetDrawMensur(false);
    currentStaffDef->SetDrawMeterSig(false);
}

//----------------------------------------------------------------------------
// Layer functor methods
//----------------------------------------------------------------------------

int Layer::ConvertToCastOffMensural(FunctorParams *functorParams)
{
    ConvertToCastOffMensuralParams *params = vrv_params_cast<ConvertToCastOffMensuralParams *>(functorParams);
    assert(params);

    params->m_contentLayer = this;

    params->m_targetLayer = new Layer(*this);
    params->m_targetLayer->ClearChildren();
    params->m_targetLayer->CloneReset();
    // Keep the xml:id of the layer in the first segment
    params->m_targetLayer->SwapUuid(this);
    assert(params->m_targetStaff);
    params->m_targetStaff->AddChild(params->m_targetLayer);

    return FUNCTOR_CONTINUE;
}

int Layer::ConvertToUnCastOffMensural(FunctorParams *functorParams)
{
    ConvertToUnCastOffMensuralParams *params = vrv_params_cast<ConvertToUnCastOffMensuralParams *>(functorParams);
    assert(params);

    if (params->m_contentLayer == NULL) {
        params->m_contentLayer = this;
    }
    else {
        params->m_contentLayer->MoveChildrenFrom(this);
    }

    return FUNCTOR_SIBLINGS;
}

int Layer::UnsetCurrentScoreDef(FunctorParams *functorParams)
{
    ResetStaffDefObjects();

    return FUNCTOR_CONTINUE;
}

int Layer::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    if (this->GetStaffDefClef()) {
        GetStaffDefClef()->ResetHorizontalAlignment(functorParams);
    }
    if (this->GetStaffDefKeySig()) {
        GetStaffDefKeySig()->ResetHorizontalAlignment(functorParams);
    }
    if (this->GetStaffDefMensur()) {
        GetStaffDefMensur()->ResetHorizontalAlignment(functorParams);
    }
    if (this->GetStaffDefMeterSig()) {
        GetStaffDefMeterSig()->ResetHorizontalAlignment(functorParams);
    }

    if (this->GetCautionStaffDefClef()) {
        GetCautionStaffDefClef()->ResetHorizontalAlignment(functorParams);
    }
    if (this->GetCautionStaffDefKeySig()) {
        GetCautionStaffDefKeySig()->ResetHorizontalAlignment(functorParams);
    }
    if (this->GetCautionStaffDefMensur()) {
        GetCautionStaffDefMensur()->ResetHorizontalAlignment(functorParams);
    }
    if (this->GetCautionStaffDefMeterSig()) {
        GetCautionStaffDefMeterSig()->ResetHorizontalAlignment(functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int Layer::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = vrv_params_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    params->m_currentMensur = GetCurrentMensur();
    params->m_currentMeterSig = GetCurrentMeterSig();

    // We are starting a new layer, reset the time;
    // We set it to -1.0 for the scoreDef attributes since they have to be aligned before any timestamp event (-1.0)
    params->m_time = DUR_MAX * -1.0;

    if (params->m_isFirstMeasure)
        params->m_scoreDefRole = SCOREDEF_SYSTEM;
    else
        params->m_scoreDefRole = SCOREDEF_INTERMEDIATE;

    if (this->GetStaffDefClef()) {
        GetStaffDefClef()->AlignHorizontally(params);
    }
    if (this->GetStaffDefKeySig()) {
        GetStaffDefKeySig()->AlignHorizontally(params);
    }
    if (this->GetStaffDefMensur()) {
        GetStaffDefMensur()->AlignHorizontally(params);
    }
    if (this->GetStaffDefMeterSig()) {
        GetStaffDefMeterSig()->AlignHorizontally(params);
    }

    params->m_scoreDefRole = SCOREDEF_NONE;

    // Now we have to set it to 0.0 since we will start aligning muscial content
    params->m_time = 0.0;

    return FUNCTOR_CONTINUE;
}

int Layer::AlignHorizontallyEnd(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = vrv_params_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    params->m_scoreDefRole = SCOREDEF_CAUTIONARY;
    params->m_time = params->m_measureAligner->GetMaxTime();

    if (this->GetCautionStaffDefClef()) {
        GetCautionStaffDefClef()->AlignHorizontally(params);
    }
    if (this->GetCautionStaffDefKeySig()) {
        GetCautionStaffDefKeySig()->AlignHorizontally(params);
    }
    if (this->GetCautionStaffDefMensur()) {
        GetCautionStaffDefMensur()->AlignHorizontally(params);
    }
    if (this->GetCautionStaffDefMeterSig()) {
        GetCautionStaffDefMeterSig()->AlignHorizontally(params);
    }

    params->m_scoreDefRole = SCOREDEF_NONE;

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    int graceAlignerId = params->m_doc->GetOptions()->m_graceRhythmAlign.GetValue() ? 0 : staff->GetN();

    int i;
    for (i = 0; i < params->m_measureAligner->GetChildCount(); ++i) {
        Alignment *alignment = vrv_cast<Alignment *>(params->m_measureAligner->GetChild(i));
        assert(alignment);
        if (alignment->HasGraceAligner(graceAlignerId)) {
            alignment->GetGraceAligner(graceAlignerId)->AlignStack();
        }
    }

    return FUNCTOR_CONTINUE;
}

int Layer::PrepareProcessingLists(FunctorParams *functorParams)
{
    PrepareProcessingListsParams *params = vrv_params_cast<PrepareProcessingListsParams *>(functorParams);
    assert(params);

    // Alternate solution with StaffN_LayerN_VerseN_t
    // StaffN_LayerN_VerseN_t *tree = vrv_cast<StaffN_LayerN_VerseN_t*>((*params).at(0));

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    params->m_layerTree.child[staff->GetN()].child[this->GetN()];

    return FUNCTOR_CONTINUE;
}

int Layer::PrepareRpt(FunctorParams *functorParams)
{
    PrepareRptParams *params = vrv_params_cast<PrepareRptParams *>(functorParams);
    assert(params);

    // If we have encountered a mRpt before and there is none is this layer, reset it to NULL
    if (params->m_currentMRpt && !this->FindDescendantByType(MRPT)) {
        params->m_currentMRpt = NULL;
    }
    return FUNCTOR_CONTINUE;
}

int Layer::CalcOnsetOffset(FunctorParams *functorParams)
{
    CalcOnsetOffsetParams *params = vrv_params_cast<CalcOnsetOffsetParams *>(functorParams);
    assert(params);

    params->m_currentScoreTime = 0.0;
    params->m_currentRealTimeSeconds = 0.0;

    params->m_currentMensur = GetCurrentMensur();
    params->m_currentMeterSig = GetCurrentMeterSig();

    return FUNCTOR_CONTINUE;
}

int Layer::ResetDrawing(FunctorParams *functorParams)
{
    this->m_crossStaffFromBelow = false;
    this->m_crossStaffFromAbove = false;
    return FUNCTOR_CONTINUE;
}

/*
int Layer::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    if (this->HasSameasLink()) {
        assert(this->GetSameasLink());
        this->GetSameasLink()->Process(params->m_functor, functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int Layer::GenerateTimemap(FunctorParams *functorParams)
{
    GenerateTimemapParams *params = vrv_params_cast<GenerateTimemapParams *>(functorParams);
    assert(params);

    if (this->HasSameasLink()) {
        assert(this->GetSameasLink());
        this->GetSameasLink()->Process(params->m_functor, functorParams);
    }

    return FUNCTOR_CONTINUE;
 }
 */

} // namespace vrv
