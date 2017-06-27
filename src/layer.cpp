/////////////////////////////////////////////////////////////////////////////
// Name:        layer.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "layer.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "clef.h"
#include "custos.h"
#include "doc.h"
#include "editorial.h"
#include "functorparams.h"
#include "keysig.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "note.h"
#include "rpt.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Layer
//----------------------------------------------------------------------------

Layer::Layer(int n)
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
    SetN(n);
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

void Layer::AddChild(Object *child)
{
    if (child->IsLayerElement()) {
        assert(dynamic_cast<LayerElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
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

    LayerElement *element = dynamic_cast<LayerElement *>(first);
    assert(element);
    if (element->GetDrawingX() > x) return NULL;

    Object *next;
    while ((next = this->GetNext())) {
        if (!next->IsLayerElement()) continue;
        LayerElement *nextLayerElement = dynamic_cast<LayerElement *>(next);
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
        Clef *clef = dynamic_cast<Clef *>(testObject);
        assert(clef);
        return clef;
    }

    return GetCurrentClef();
}

int Layer::GetClefLocOffset(LayerElement *test)
{
    Clef *clef = GetClef(test);
    if (!clef) return 0;
    return clef->GetClefLocOffset();
}

data_STEMDIRECTION Layer::GetDrawingStemDir(LayerElement *element)
{
    return m_drawingStemDir;
}

data_STEMDIRECTION Layer::GetDrawingStemDir(const ArrayOfBeamElementCoords *coords)
{
    return m_drawingStemDir;
}

Clef *Layer::GetCurrentClef() const
{
    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff && staff->m_drawingStaffDef && staff->m_drawingStaffDef->GetCurrentClef());
    return staff->m_drawingStaffDef->GetCurrentClef();
}

KeySig *Layer::GetCurrentKeySig() const
{
    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff && staff->m_drawingStaffDef);
    return staff->m_drawingStaffDef->GetCurrentKeySig();
}

Mensur *Layer::GetCurrentMensur() const
{
    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff && staff->m_drawingStaffDef);
    return staff->m_drawingStaffDef->GetCurrentMensur();
}

MeterSig *Layer::GetCurrentMeterSig() const
{
    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
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

int Layer::UnsetCurrentScoreDef(FunctorParams *functorParams)
{
    ResetStaffDefObjects();

    return FUNCTOR_CONTINUE;
};

int Layer::AlignHorizontally(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = dynamic_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    params->m_currentMensur = GetCurrentMensur();
    params->m_currentMeterSig = GetCurrentMeterSig();

    // We are starting a new layer, reset the time;
    // We set it to -1.0 for the scoreDef attributes since they have to be aligned before any timestamp event (-1.0)
    params->m_time = DUR_MAX * -1.0;

    if (params->m_isFirstMeasure)
        params->m_scoreDefRole = SYSTEM_SCOREDEF;
    else
        params->m_scoreDefRole = INTERMEDIATE_SCOREDEF;

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

    params->m_scoreDefRole = NONE;

    // Now we have to set it to 0.0 since we will start aligning muscial content
    params->m_time = 0.0;

    return FUNCTOR_CONTINUE;
}

int Layer::AlignHorizontallyEnd(FunctorParams *functorParams)
{
    AlignHorizontallyParams *params = dynamic_cast<AlignHorizontallyParams *>(functorParams);
    assert(params);

    params->m_scoreDefRole = CAUTIONARY_SCOREDEF;
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

    params->m_scoreDefRole = NONE;

    int i;
    for (i = 0; i < params->m_measureAligner->GetChildCount(); i++) {
        Alignment *alignment = dynamic_cast<Alignment *>(params->m_measureAligner->GetChild(i));
        assert(alignment);
        if (alignment->HasGraceAligner()) {
            alignment->GetGraceAligner()->AlignStack();
        }
    }

    return FUNCTOR_CONTINUE;
}

int Layer::PrepareProcessingLists(FunctorParams *functorParams)
{
    PrepareProcessingListsParams *params = dynamic_cast<PrepareProcessingListsParams *>(functorParams);
    assert(params);

    // Alternate solution with StaffN_LayerN_VerseN_t
    // StaffN_LayerN_VerseN_t *tree = static_cast<StaffN_LayerN_VerseN_t*>((*params).at(0));

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);
    params->m_layerTree.child[staff->GetN()].child[this->GetN()];

    return FUNCTOR_CONTINUE;
}

int Layer::PrepareRpt(FunctorParams *functorParams)
{
    PrepareRptParams *params = dynamic_cast<PrepareRptParams *>(functorParams);
    assert(params);

    // If we have encountered a mRpt before and there is none is this layer, reset it to NULL
    if (params->m_currentMRpt && !this->FindChildByType(MRPT)) {
        params->m_currentMRpt = NULL;
    }
    return FUNCTOR_CONTINUE;
}

int Layer::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = dynamic_cast<CalcStemParams *>(functorParams);
    assert(params);

    // setting the layer stem direction
    if (this->GetParent()->GetChildCount(LAYER) > 1) {
        if (this->GetParent()->FindChildByType(LAYER) == this) {
            this->SetDrawingStemDir(STEMDIRECTION_up);
        }
        else {
            this->SetDrawingStemDir(STEMDIRECTION_down);
        }
    }

    return FUNCTOR_CONTINUE;
}

int Layer::AdjustSylSpacing(FunctorParams *functorParams)
{
    AdjustSylSpacingParams *params = dynamic_cast<AdjustSylSpacingParams *>(functorParams);
    assert(params);

    // reset it
    params->m_overlapingSyl.clear();
    params->m_previousSyl = NULL;

    return FUNCTOR_CONTINUE;
}

int Layer::CalcOnsetOffset(FunctorParams *functorParams)
{
    CalcOnsetOffsetParams *params = dynamic_cast<CalcOnsetOffsetParams *>(functorParams);
    assert(params);

    params->m_currentScoreTime = 0.0;
    params->m_currentRealTimeSeconds = 0.0;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
