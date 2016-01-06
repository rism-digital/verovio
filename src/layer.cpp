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

Layer::Layer():
	DocObject("layer-"), DrawingListInterface(), ObjectListInterface(), StaffDefDrawingInterface(),
    AttCommon()
{
    RegisterAttClass(ATT_COMMON);

    Reset();
}

Layer::~Layer()
{
}

void Layer::Reset()
{
    DocObject::Reset();
    DrawingListInterface::Reset();
    StaffDefDrawingInterface::Reset();
    ResetCommon();

    m_drawingStemDir = STEMDIRECTION_NONE;
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
    if (!element || this->GetList(this)->empty())
        return NULL;
    
    return dynamic_cast<LayerElement*>(GetListPrevious(element));
}

LayerElement *Layer::GetAtPos(int x)
{
	LayerElement *element = dynamic_cast<LayerElement*>(this->GetFirst());
	if (!element || element->GetDrawingX() > x) return NULL;
    
    LayerElement *next = NULL;
	while ((next = dynamic_cast<LayerElement*>(this->GetNext()))) {
		if (next->GetDrawingX() > x) return element;
        element = next;
	}
	return element;
}

void Layer::SetDrawingAndCurrentValues( StaffDef *currentStaffDef)
{
    if (!currentStaffDef) {
        LogDebug("staffDef not found");
        return;
    }
    
    // Remove any previous value in the Layer
    this->StaffDefDrawingInterface::Reset();
    
    // Special case with C-major / A-minor key signature (0) : if key cancellation is false, we are at the beginning
    // of a new system, and hence we should not draw it. Maybe this can be improved?
    bool drawKeySig = currentStaffDef->DrawKeySig();
    if (currentStaffDef->GetCurrentKeySig() && (currentStaffDef->GetCurrentKeySig()->GetAlterationNumber() == 0)) {
        if (currentStaffDef->DrawKeySigCancellation() == false) {
            drawKeySig = false;
        }
    }
    
    this->SetDrawClef(currentStaffDef->DrawClef());
    this->SetDrawKeySig(drawKeySig); // see above
    this->SetDrawMensur(currentStaffDef->DrawMensur());
    this->SetDrawMeterSig(currentStaffDef->DrawMeterSig());
    this->SetDrawKeySigCancellation(currentStaffDef->DrawKeySigCancellation());
    // Don't draw on the next one
    currentStaffDef->SetDrawClef(false);
    currentStaffDef->SetDrawKeySig(false);
    currentStaffDef->SetDrawMensur(false);
    currentStaffDef->SetDrawMeterSig(false);
    currentStaffDef->SetDrawKeySigCancellation(false);

    if (currentStaffDef->GetCurrentClef()) {
        this->SetCurrentClef(new Clef(*currentStaffDef->GetCurrentClef()));
    }
    if (currentStaffDef->GetCurrentKeySig()) {
        this->SetCurrentKeySig(new KeySig(*currentStaffDef->GetCurrentKeySig()));
    }
    if (currentStaffDef->GetCurrentMensur()) {
        this->SetCurrentMensur(new Mensur(*currentStaffDef->GetCurrentMensur()));
    }
    if (currentStaffDef->GetCurrentMeterSig()) {
        this->SetCurrentMeterSig(new MeterSig(*currentStaffDef->GetCurrentMeterSig()));
    }
}

Clef* Layer::GetClef(LayerElement *test)
{
    Object *testObject = test;
    
    if (!test) {
        return GetCurrentClef();
    }
	
    //make sure list is set
    ResetList(this);
    if (test->Is() != CLEF) {
        testObject = GetListFirstBackward(testObject, CLEF);
    }
    
    if (testObject && testObject->Is() == CLEF) {
        Clef *clef = dynamic_cast<Clef*>(testObject);
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
    
//----------------------------------------------------------------------------
// Layer functor methods
//----------------------------------------------------------------------------

int Layer::AlignHorizontally(ArrayPtrVoid *params)
{
    // param 0: the measureAligner (unused)
    // param 1: the time
    // param 2: the current Mensur
    // param 3: the current MeterSig
    double *time = static_cast<double*>((*params).at(1));
    Mensur **currentMensur = static_cast<Mensur**>((*params).at(2));
    MeterSig **currentMeterSig = static_cast<MeterSig**>((*params).at(3));
    
    // we need to call it because we are overriding Object::AlignHorizontally
    this->ResetHorizontalAlignment();

    // we are starting a new layer, reset the time;
    (*time) = 0.0;
    
    (*currentMensur) = GetCurrentMensur();
    (*currentMeterSig) = GetCurrentMeterSig();
    
    if (DrawClef() && GetCurrentClef()) {
        GetCurrentClef()->AlignHorizontally(params);
    }
    if (DrawKeySig() && GetCurrentKeySig()) {
        GetCurrentKeySig()->AlignHorizontally(params);
    }
    if (DrawMensur() && GetCurrentMensur()) {
        GetCurrentMensur()->AlignHorizontally(params);
    }
    if (DrawMeterSig() && GetCurrentMeterSig()) {
        GetCurrentMeterSig()->AlignHorizontally(params);
    }

    return FUNCTOR_CONTINUE;
}
    
int Layer::AlignHorizontallyEnd(ArrayPtrVoid *params)
{
    // param 0: the measureAligner
    // param 1: the time  (unused)
    // param 2: the current Mensur (unused)
    // param 3: the current MeterSig (unused)
    MeasureAligner **measureAligner = static_cast<MeasureAligner**>((*params).at(0));
    
    int i;
    for(i = 0; i < (int)(*measureAligner)->m_children.size(); i++) {
        Alignment *alignment = dynamic_cast<Alignment*>((*measureAligner)->m_children.at(i));
        if (alignment && alignment->HasGraceAligner()) {
            alignment->GetGraceAligner()->AlignStack();
        }
    }
    
    return FUNCTOR_CONTINUE;
}
    
int Layer::PrepareProcessingLists(ArrayPtrVoid *params)
{
    // param 0: the IntTree* for staff/layer/verse (unused)
    // param 1: the IntTree* for staff/layer
    IntTree *tree = static_cast<IntTree*>((*params).at(1));
    // Alternate solution with StaffN_LayerN_VerseN_t
    //StaffN_LayerN_VerseN_t *tree = static_cast<StaffN_LayerN_VerseN_t*>((*params).at(0));
    
    Staff *staff = dynamic_cast<Staff*>(this->GetFirstParent(STAFF));
    assert(staff);
    tree->child[ staff->GetN() ].child[ this->GetN() ];
    
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
    Measure **currentMeasure = static_cast<Measure**>((*params).at(2));
    Layer **currentLayer = static_cast<Layer**>((*params).at(4));
    bool *processLayerElements = static_cast<bool*>((*params).at(6));
    
    (*currentLayer) = this;
    
    // Second pass where we do just process layer elements
    if ((*processLayerElements)) {
        return FUNCTOR_CONTINUE;
    }
    
    // set the values for the scoreDef elements when required
    if (this->GetDrawingClef()) {
        this->GetDrawingClef()->SetDrawingX(this->GetDrawingClef()->GetXRel() + (*currentMeasure)->GetDrawingX());
    }
    if (this->GetDrawingKeySig()) {
        this->GetDrawingKeySig()->SetDrawingX(this->GetDrawingKeySig()->GetXRel() + (*currentMeasure)->GetDrawingX());
    }
    if (this->GetDrawingMensur()) {
        this->GetDrawingMensur()->SetDrawingX(this->GetDrawingMensur()->GetXRel() + (*currentMeasure)->GetDrawingX());
    }
    if (this->GetDrawingMeterSig()) {
        this->GetDrawingMeterSig()->SetDrawingX(this->GetDrawingMeterSig()->GetXRel() + (*currentMeasure)->GetDrawingX());
    }
    
    return FUNCTOR_CONTINUE;
}
    
int Layer::PrepareRpt(ArrayPtrVoid *params)
{
    // param 0: a pointer to the current MRpt pointer
    // param 1: a pointer to the data_BOOLEAN indicating if multiNumber (unused)
    // param 2: a pointer to the doc scoreDef (unused)
    MRpt **currentMRpt =  static_cast<MRpt**>((*params).at(0));

    // If we have encountered a mRpt before and there is none is this layer, reset it to NULL
    if ((*currentMRpt) && !this->FindChildByType(MRPT)) {
        (*currentMRpt) = NULL;
    }
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
