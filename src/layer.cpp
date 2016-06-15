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

Layer::Layer() : Object("layer-"), DrawingListInterface(), ObjectListInterface(), AttCommon()
{
    RegisterAttClass(ATT_COMMON);

    // owned pointers need to be set to NULL;
    m_staffDefClef = NULL;
    m_staffDefKeySig = NULL;
    m_staffDefMensur = NULL;
    m_staffDefMeterSig = NULL;

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
    ResetCommon();

    ResetStaffDefOjects();

    m_drawingStemDir = STEMDIRECTION_NONE;
}

void Layer::ResetStaffDefOjects()
{
    m_drawKeySigCancellation = false;
    // Resert(m_staffDefClef);
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
}

void Layer::AddLayerElement(LayerElement *element, int idx)
{
    element->SetParent(this);
    if (idx == -1) {
        m_children.push_back(element);
    }
    else {
        InsertChild(element, idx);
    }
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
    if (test->Is() != CLEF) {
        testObject = GetListFirstBackward(testObject, CLEF);
    }

    if (testObject && testObject->Is() == CLEF) {
        Clef *clef = dynamic_cast<Clef *>(testObject);
        assert(clef);
        return clef;
    }

    return GetCurrentClef();
}

int Layer::GetClefOffset(LayerElement *test)
{
    Clef *clef = GetClef(test);
    if (!clef) return 0;
    return clef->GetClefOffset();
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

void Layer::SetDrawingAndCurrentValues(StaffDef *currentStaffDef)
{
    if (!currentStaffDef) {
        LogDebug("staffDef not found");
        return;
    }

    // Remove any previous value in the Layer
    this->ResetStaffDefOjects();

    // Special case with C-major / A-minor key signature (0) : if key cancellation is false, we are at the beginning
    // of a new system, and hence we should not draw it. Maybe this can be improved?
    bool drawKeySig = currentStaffDef->DrawKeySig();
    if (currentStaffDef->GetCurrentKeySig() && (currentStaffDef->GetCurrentKeySig()->GetAlterationNumber() == 0)) {
        if (currentStaffDef->DrawKeySigCancellation() == false) {
            drawKeySig = false;
        }
    }

    if (currentStaffDef->DrawClef()) this->m_staffDefClef = new Clef(*currentStaffDef->GetCurrentClef());
    // special case - see above
    if (drawKeySig) this->m_staffDefKeySig = new KeySig(*currentStaffDef->GetCurrentKeySig());
    if (currentStaffDef->DrawMensur()) this->m_staffDefMensur = new Mensur(*currentStaffDef->GetCurrentMensur());
    if (currentStaffDef->DrawMeterSig())
        this->m_staffDefMeterSig = new MeterSig(*currentStaffDef->GetCurrentMeterSig());
    this->SetDrawKeySigCancellation(currentStaffDef->DrawKeySigCancellation());

    // Don't draw on the next one
    currentStaffDef->SetDrawClef(false);
    currentStaffDef->SetDrawKeySig(false);
    currentStaffDef->SetDrawMensur(false);
    currentStaffDef->SetDrawMeterSig(false);
    currentStaffDef->SetDrawKeySigCancellation(false);
}

//----------------------------------------------------------------------------
// Layer functor methods
//----------------------------------------------------------------------------

int Layer::UnsetCurrentScoreDef(ArrayPtrVoid *params)
{
    ResetStaffDefOjects();

    return FUNCTOR_CONTINUE;
};

int Layer::AlignHorizontally(ArrayPtrVoid *params)
{
    // param 0: the measureAligner (unused)
    // param 1: the time
    // param 2: the current Mensur
    // param 3: the current MeterSig
    // param 4: the functor for passing it to the TimeStampAligner (unused)
    double *time = static_cast<double *>((*params).at(1));
    Mensur **currentMensur = static_cast<Mensur **>((*params).at(2));
    MeterSig **currentMeterSig = static_cast<MeterSig **>((*params).at(3));

    (*currentMensur) = GetCurrentMensur();
    (*currentMeterSig) = GetCurrentMeterSig();

    // We are starting a new layer, reset the time;
    int meterUnit = 4;
    if (*currentMeterSig && (*currentMeterSig)->HasUnit()) meterUnit = (*currentMeterSig)->GetUnit();
    // We set it to -1.5 for the scoreDef attributes since they have to be aligned before any timestamp event (-1.0)
    (*time) = DUR_MAX * -1.5;

    if (this->GetStaffDefClef()) {
        GetStaffDefClef()->AlignHorizontally(params);
    }
    if (this->GetStaffDefKeySig()) {
        GetStaffDefKeySig()->AlignHorizontally(params);
    }
    if (this->GetStaffDefMensur()) {
        GetStaffDefMensur()->AlignHorizontally(params);
    }
    if (GetStaffDefMeterSig()) {
        GetStaffDefMeterSig()->AlignHorizontally(params);
    }

    // Now we have to set it to 0.0 since we will start aligning muscial content
    (*time) = 0.0;

    return FUNCTOR_CONTINUE;
}

int Layer::AlignHorizontallyEnd(ArrayPtrVoid *params)
{
    // param 0: the measureAligner
    // param 1: the time  (unused)
    // param 2: the current Mensur (unused)
    // param 3: the current MeterSig (unused)
    MeasureAligner **measureAligner = static_cast<MeasureAligner **>((*params).at(0));

    int i;
    for (i = 0; i < (*measureAligner)->GetChildCount(); i++) {
        Alignment *alignment = dynamic_cast<Alignment *>((*measureAligner)->GetChild(i));
        assert(alignment);
        if (alignment->HasGraceAligner()) {
            alignment->GetGraceAligner()->AlignStack();
        }
    }

    return FUNCTOR_CONTINUE;
}

int Layer::PrepareProcessingLists(ArrayPtrVoid *params)
{
    // param 0: the IntTree* for staff/layer/verse (unused)
    // param 1: the IntTree* for staff/layer
    IntTree *tree = static_cast<IntTree *>((*params).at(1));
    // Alternate solution with StaffN_LayerN_VerseN_t
    // StaffN_LayerN_VerseN_t *tree = static_cast<StaffN_LayerN_VerseN_t*>((*params).at(0));

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);
    tree->child[staff->GetN()].child[this->GetN()];

    return FUNCTOR_CONTINUE;
}

int Layer::SetDrawingXY(ArrayPtrVoid *params)
{
    // param 0: a pointer doc (unused)
    // param 1: a pointer to the current system (unused)
    // param 2: a pointer to the current measure
    // param 3: a pointer to the current staff (unused)
    // param 4: a pointer to the current layer
    // param 5: a pointer to the view (unused)
    // param 6: a bool indicating if we are processing layer elements or not
    // param 7: a pointer to the functor for passing it to the timestamps (unused)
    Measure **currentMeasure = static_cast<Measure **>((*params).at(2));
    Layer **currentLayer = static_cast<Layer **>((*params).at(4));
    bool *processLayerElements = static_cast<bool *>((*params).at(6));

    (*currentLayer) = this;

    // Second pass where we do just process layer elements
    if ((*processLayerElements)) {
        return FUNCTOR_CONTINUE;
    }

    // set the values for the scoreDef elements when required
    if (this->GetStaffDefClef()) {
        this->GetStaffDefClef()->SetDrawingX(this->GetStaffDefClef()->GetXRel() + (*currentMeasure)->GetDrawingX());
    }
    if (this->GetStaffDefKeySig()) {
        this->GetStaffDefKeySig()->SetDrawingX(this->GetStaffDefKeySig()->GetXRel() + (*currentMeasure)->GetDrawingX());
    }
    if (this->GetStaffDefMensur()) {
        this->GetStaffDefMensur()->SetDrawingX(this->GetStaffDefMensur()->GetXRel() + (*currentMeasure)->GetDrawingX());
    }
    if (this->GetStaffDefMeterSig()) {
        this->GetStaffDefMeterSig()->SetDrawingX(
            this->GetStaffDefMeterSig()->GetXRel() + (*currentMeasure)->GetDrawingX());
    }

    return FUNCTOR_CONTINUE;
}

int Layer::PrepareRpt(ArrayPtrVoid *params)
{
    // param 0: a pointer to the current MRpt pointer
    // param 1: a pointer to the data_BOOLEAN indicating if multiNumber (unused)
    // param 2: a pointer to the doc scoreDef (unused)
    MRpt **currentMRpt = static_cast<MRpt **>((*params).at(0));

    // If we have encountered a mRpt before and there is none is this layer, reset it to NULL
    if ((*currentMRpt) && !this->FindChildByType(MRPT)) {
        (*currentMRpt) = NULL;
    }
    return FUNCTOR_CONTINUE;
}

int Layer::CalcMaxMeasureDuration(ArrayPtrVoid *params)
{
    // param 0: std::vector<double>: a stack of maximum duration filled by the functor (unused)
    // param 1: double: the duration of the current measure
    double *currentValue = static_cast<double *>((*params).at(1));

    // reset it
    (*currentValue) = 0.0;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
