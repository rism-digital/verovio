/////////////////////////////////////////////////////////////////////////////
// Name:        layer.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "layer.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "beam.h"
#include "clef.h"
#include "comparison.h"
#include "custos.h"
#include "divline.h"
#include "doc.h"
#include "editorial.h"
#include "findlayerelementsfunctor.h"
#include "functor.h"
#include "keysig.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "metersiggrp.h"
#include "mrpt.h"
#include "note.h"
#include "staff.h"
#include "staffdef.h"
#include "vrv.h"

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Layer
//----------------------------------------------------------------------------

static const ClassRegistrar<Layer> s_factory("layer", LAYER);

Layer::Layer()
    : Object(LAYER), DrawingListInterface(), ObjectListInterface(), AttCue(), AttNInteger(), AttTyped(), AttVisibility()
{
    this->RegisterAttClass(ATT_CUE);
    this->RegisterAttClass(ATT_NINTEGER);
    this->RegisterAttClass(ATT_TYPED);
    this->RegisterAttClass(ATT_VISIBILITY);

    // owned pointers need to be set to NULL;
    m_staffDefClef = NULL;
    m_staffDefKeySig = NULL;
    m_staffDefMensur = NULL;
    m_staffDefMeterSig = NULL;
    m_staffDefMeterSigGrp = NULL;
    m_cautionStaffDefClef = NULL;
    m_cautionStaffDefKeySig = NULL;
    m_cautionStaffDefMensur = NULL;
    m_cautionStaffDefMeterSig = NULL;

    this->Reset();
}

Layer::~Layer()
{
    // We need to delete own objects
    this->Reset();
}

void Layer::Reset()
{
    Object::Reset();
    DrawingListInterface::Reset();
    this->ResetCue();
    this->ResetNInteger();
    this->ResetTyped();
    this->ResetVisibility();

    this->ResetStaffDefObjects();

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
    m_staffDefMeterSigGrp = NULL;
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
    if (m_staffDefMeterSigGrp) {
        delete m_staffDefMeterSigGrp;
        m_staffDefMeterSigGrp = NULL;
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

bool Layer::IsSupportedChild(ClassId classId)
{
    if (Object::IsLayerElement(classId)) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

LayerElement *Layer::GetPrevious(const LayerElement *element)
{
    return const_cast<LayerElement *>(std::as_const(*this).GetPrevious(element));
}

const LayerElement *Layer::GetPrevious(const LayerElement *element) const
{
    this->ResetList();
    if (!element || this->HasEmptyList()) return NULL;

    return dynamic_cast<const LayerElement *>(this->GetListPrevious(element));
}

LayerElement *Layer::GetAtPos(int x)
{
    return const_cast<LayerElement *>(std::as_const(*this).GetAtPos(x));
}

const LayerElement *Layer::GetAtPos(int x) const
{
    const Object *first = this->GetFirst();
    if (!first) return NULL;

    if (first->IsEditorialElement()) {
        IsEditorialElementComparison cmp;
        cmp.ReverseComparison();
        first = this->FindDescendantByComparison(&cmp);
    }
    if (!first || !first->IsLayerElement()) return NULL;

    const LayerElement *element = vrv_cast<const LayerElement *>(first);
    assert(element);
    if (element->GetDrawingX() > x) return NULL;

    const Object *next;
    while ((next = this->GetNext())) {
        const LayerElement *nextLayerElement = NULL;
        if (next->IsLayerElement()) {
            nextLayerElement = vrv_cast<const LayerElement *>(next);
        }
        else if (next->IsEditorialElement()) {
            IsEditorialElementComparison cmp;
            cmp.ReverseComparison();
            nextLayerElement = vrv_cast<const LayerElement *>(next->FindDescendantByComparison(&cmp));
            if (!nextLayerElement) continue;
        }
        else {
            continue;
        }
        assert(nextLayerElement);
        if (nextLayerElement->GetDrawingX() > x) return element;
        element = nextLayerElement;
    }
    // This can be NULL if the layer is empty
    return element;
}

Clef *Layer::GetClef(const LayerElement *test)
{
    return const_cast<Clef *>(std::as_const(*this).GetClef(test));
}

const Clef *Layer::GetClef(const LayerElement *test) const
{
    const Object *testObject = test;

    if (!test) {
        return this->GetCurrentClef();
    }

    // make sure list is set
    this->ResetList();
    if (!test->Is(CLEF)) {
        testObject = this->GetListFirstBackward(testObject, CLEF);
    }

    if (testObject && testObject->Is(CLEF)) {
        const Clef *clef = vrv_cast<const Clef *>(testObject);
        assert(clef);
        return clef;
    }
    const Clef *facsClef = this->GetClefFacs(test);
    if (facsClef != NULL) {
        return facsClef;
    }
    return this->GetCurrentClef();
}

Clef *Layer::GetClefFacs(const LayerElement *test)
{
    return const_cast<Clef *>(std::as_const(*this).GetClefFacs(test));
}

const Clef *Layer::GetClefFacs(const LayerElement *test) const
{
    const Doc *doc = vrv_cast<const Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);
    if (doc->IsFacs()) {
        ListOfConstObjects clefs;
        ClassIdComparison ac(CLEF);
        doc->FindAllDescendantsBetween(&clefs, &ac, doc->GetFirst(CLEF), test);
        if (clefs.size() > 0) {
            return dynamic_cast<const Clef *>(*clefs.rbegin());
        }
    }
    return NULL;
}

int Layer::GetClefLocOffset(const LayerElement *test) const
{
    const Clef *clef = this->GetClef(test);
    if (!clef) return 0;
    return clef->GetClefLocOffset();
}

int Layer::GetCrossStaffClefLocOffset(const LayerElement *element, int currentOffset) const
{
    if (element->m_crossStaff) {
        this->ResetList();
        if (!element->Is(CLEF)) {
            const Clef *clef = vrv_cast<const Clef *>(GetListFirstBackward(element, CLEF));
            if (clef && clef->m_crossStaff) {
                return clef->GetClefLocOffset();
            }
        }
    }
    return currentOffset;
}

data_STEMDIRECTION Layer::GetDrawingStemDir(const LayerElement *element) const
{
    assert(element);

    if (this->GetLayerCountForTimeSpanOf(element) < 2) {
        return STEMDIRECTION_NONE;
    }
    else {
        if (m_crossStaffFromBelow) {
            return (element->m_crossStaff) ? STEMDIRECTION_down : STEMDIRECTION_up;
        }
        else if (m_crossStaffFromAbove) {
            return (element->m_crossStaff) ? STEMDIRECTION_up : STEMDIRECTION_down;
        }
        else {
            return m_drawingStemDir;
        }
    }
}

data_STEMDIRECTION Layer::GetDrawingStemDir(const ArrayOfBeamElementCoords *coords) const
{
    assert(!coords->empty());

    // Adjust the x position of the first and last element for taking into account the stem width
    const LayerElement *first = dynamic_cast<const LayerElement *>(coords->front()->m_element);
    const LayerElement *last = dynamic_cast<const LayerElement *>(coords->back()->m_element);

    if (!first || !last) {
        return m_drawingStemDir;
    }

    const Measure *measure = vrv_cast<const Measure *>(this->GetFirstAncestor(MEASURE));
    assert(measure);

    const Alignment *alignmentFirst = first->GetAlignment();
    assert(alignmentFirst);
    const Alignment *alignmentLast = last->GetAlignment();
    assert(alignmentLast);

    // We are ignoring cross-staff situation here because this should not be called if we have one
    const Staff *staff = first->GetAncestorStaff();

    Fraction time = alignmentFirst->GetTime();
    Fraction duration;
    // For the sake of counting number of layers consider only current measure. If first and last elements' layers are
    // different, take only time within current measure to run GetLayerCountInTimeSpan.
    const Measure *lastMeasure = vrv_cast<const Measure *>(last->GetFirstAncestor(MEASURE));
    if (lastMeasure == measure) {
        duration = alignmentLast->GetTime() - time + last->GetAlignmentDuration();
    }
    else {
        duration = measure->m_measureAligner.GetRightAlignment()->GetTime() - time;
    }

    if (this->GetLayerCountInTimeSpan(time, duration, measure, staff->GetN()) < 2) {
        return STEMDIRECTION_NONE;
    }
    else {
        return m_drawingStemDir;
    }
}

std::set<int> Layer::GetLayersNForTimeSpanOf(const LayerElement *element) const
{
    assert(element);

    const Measure *measure = vrv_cast<const Measure *>(this->GetFirstAncestor(MEASURE));
    assert(measure);

    const Alignment *alignment = element->GetAlignment();
    assert(alignment);

    const Staff *staff = element->GetAncestorStaff(RESOLVE_CROSS_STAFF);

    return this->GetLayersNInTimeSpan(alignment->GetTime(), element->GetAlignmentDuration(), measure, staff->GetN());
}

int Layer::GetLayerCountForTimeSpanOf(const LayerElement *element) const
{
    return static_cast<int>(this->GetLayersNForTimeSpanOf(element).size());
}

std::set<int> Layer::GetLayersNInTimeSpan(
    const Fraction &time, const Fraction &duration, const Measure *measure, int staff) const
{
    assert(measure);

    LayersInTimeSpanFunctor layersInTimeSpan(this->GetCurrentMeterSig(), this->GetCurrentMensur());
    layersInTimeSpan.SetEvent(time, duration);

    Filters filters;
    AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staff);
    filters.Add(&matchStaff);
    layersInTimeSpan.SetFilters(&filters);

    measure->m_measureAligner.Process(layersInTimeSpan);

    return layersInTimeSpan.GetLayers();
}

int Layer::GetLayerCountInTimeSpan(
    const Fraction &time, const Fraction &duration, const Measure *measure, int staff) const
{
    return static_cast<int>(this->GetLayersNInTimeSpan(time, duration, measure, staff).size());
}

ListOfObjects Layer::GetLayerElementsForTimeSpanOf(const LayerElement *element, bool excludeCurrent)
{
    ListOfConstObjects elements = std::as_const(*this).GetLayerElementsForTimeSpanOf(element, excludeCurrent);
    ListOfObjects objects;
    std::for_each(elements.begin(), elements.end(),
        [&objects](const Object *element) { objects.push_back(const_cast<Object *>(element)); });
    return objects;
}

ListOfConstObjects Layer::GetLayerElementsForTimeSpanOf(const LayerElement *element, bool excludeCurrent) const
{
    assert(element);

    const Measure *measure = vrv_cast<const Measure *>(this->GetFirstAncestor(MEASURE));
    assert(measure);

    Fraction time;
    Fraction duration;
    const Alignment *alignment = element->GetAlignment();
    // Get duration and time if element has alignment
    if (alignment) {
        time = alignment->GetTime();
        duration = element->GetAlignmentDuration();
    }
    // If it is Beam, try to get alignments for first and last elements and calculate
    // the duration of the beam based on those
    else if (element->Is(BEAM)) {
        const Beam *beam = vrv_cast<const Beam *>(element);

        const LayerElement *first = vrv_cast<const LayerElement *>(beam->GetListFront());
        const LayerElement *last = vrv_cast<const LayerElement *>(beam->GetListBack());

        if (!first || !last) return {};

        time = first->GetAlignment()->GetTime();
        Fraction lastTime = last->GetAlignment()->GetTime();
        duration = lastTime - time + last->GetAlignmentDuration();
    }
    else {
        return {};
    }

    const Staff *staff = element->GetAncestorStaff(RESOLVE_CROSS_STAFF);

    return this->GetLayerElementsInTimeSpan(time, duration, measure, staff->GetN(), excludeCurrent);
}

ListOfObjects Layer::GetLayerElementsInTimeSpan(
    const Fraction &time, const Fraction &duration, const Measure *measure, int staff, bool excludeCurrent)
{
    ListOfConstObjects elements
        = std::as_const(*this).GetLayerElementsInTimeSpan(time, duration, measure, staff, excludeCurrent);
    ListOfObjects objects;
    std::for_each(elements.begin(), elements.end(),
        [&objects](const Object *element) { objects.push_back(const_cast<Object *>(element)); });
    return objects;
}

ListOfConstObjects Layer::GetLayerElementsInTimeSpan(
    const Fraction &time, const Fraction &duration, const Measure *measure, int staff, bool excludeCurrent) const
{
    assert(measure);

    LayerElementsInTimeSpanFunctor layerElementsInTimeSpan(this->GetCurrentMeterSig(), this->GetCurrentMensur(), this);
    layerElementsInTimeSpan.SetEvent(time, duration);
    if (excludeCurrent) layerElementsInTimeSpan.ConsiderAllLayersButCurrent();

    Filters filters;
    AttNIntegerComparison matchStaff(ALIGNMENT_REFERENCE, staff);
    filters.Add(&matchStaff);
    layerElementsInTimeSpan.SetFilters(&filters);

    measure->m_measureAligner.Process(layerElementsInTimeSpan);

    return layerElementsInTimeSpan.GetElements();
}

Clef *Layer::GetCurrentClef()
{
    return const_cast<Clef *>(std::as_const(*this).GetCurrentClef());
}

const Clef *Layer::GetCurrentClef() const
{
    const Staff *staff = vrv_cast<const Staff *>(this->GetFirstAncestor(STAFF));
    if (staff && staff->m_drawingStaffDef) {
        return staff->m_drawingStaffDef->GetCurrentClef();
    }
    return NULL;
}

KeySig *Layer::GetCurrentKeySig()
{
    return const_cast<KeySig *>(std::as_const(*this).GetCurrentKeySig());
}

const KeySig *Layer::GetCurrentKeySig() const
{
    const Staff *staff = vrv_cast<const Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff && staff->m_drawingStaffDef);
    return staff->m_drawingStaffDef->GetCurrentKeySig();
}

Mensur *Layer::GetCurrentMensur()
{
    return const_cast<Mensur *>(std::as_const(*this).GetCurrentMensur());
}

const Mensur *Layer::GetCurrentMensur() const
{
    const Staff *staff = vrv_cast<const Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff && staff->m_drawingStaffDef);
    return staff->m_drawingStaffDef->GetCurrentMensur();
}

MeterSig *Layer::GetCurrentMeterSig()
{
    return const_cast<MeterSig *>(std::as_const(*this).GetCurrentMeterSig());
}

const MeterSig *Layer::GetCurrentMeterSig() const
{
    const Staff *staff = vrv_cast<const Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff && staff->m_drawingStaffDef);
    return staff->m_drawingStaffDef->GetCurrentMeterSig();
}

Proport *Layer::GetCurrentProport()
{
    return const_cast<Proport *>(std::as_const(*this).GetCurrentProport());
}

const Proport *Layer::GetCurrentProport() const
{
    const Staff *staff = vrv_cast<const Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff && staff->m_drawingStaffDef);
    return staff->m_drawingStaffDef->GetCurrentProport();
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
        m_staffDefClef = new Clef(*currentStaffDef->GetCurrentClef());
        m_staffDefClef->SetParent(this);
    }
    if (currentStaffDef->DrawKeySig()) {
        m_staffDefKeySig = new KeySig(*currentStaffDef->GetCurrentKeySig());
        m_staffDefKeySig->SetParent(this);
    }
    if (currentStaffDef->DrawMensur()) {
        m_staffDefMensur = new Mensur(*currentStaffDef->GetCurrentMensur());
        m_staffDefMensur->SetParent(this);
    }
    if (currentStaffDef->DrawMeterSigGrp()) {
        m_staffDefMeterSigGrp = new MeterSigGrp(*currentStaffDef->GetCurrentMeterSigGrp());
        m_staffDefMeterSigGrp->SetParent(this);
    }
    else if (currentStaffDef->DrawMeterSig()) {
        m_staffDefMeterSig = new MeterSig(*currentStaffDef->GetCurrentMeterSig());
        m_staffDefMeterSig->SetParent(this);
    }

    // Don't draw on the next one
    currentStaffDef->SetDrawClef(false);
    currentStaffDef->SetDrawKeySig(false);
    currentStaffDef->SetDrawMensur(false);
    currentStaffDef->SetDrawMeterSig(false);
    currentStaffDef->SetDrawMeterSigGrp(false);
}

void Layer::SetDrawingCautionValues(StaffDef *currentStaffDef)
{
    if (!currentStaffDef) {
        LogDebug("staffDef not found");
        return;
    }

    if (currentStaffDef->DrawClef()) {
        m_cautionStaffDefClef = new Clef(*currentStaffDef->GetCurrentClef());
        m_cautionStaffDefClef->SetParent(this);
    }
    // special case - see above
    if (currentStaffDef->DrawKeySig()) {
        m_cautionStaffDefKeySig = new KeySig(*currentStaffDef->GetCurrentKeySig());
        m_cautionStaffDefKeySig->SetDrawingClef(currentStaffDef->GetCurrentClef());
        m_cautionStaffDefKeySig->SetParent(this);
    }
    if (currentStaffDef->DrawMensur()) {
        m_cautionStaffDefMensur = new Mensur(*currentStaffDef->GetCurrentMensur());
        m_cautionStaffDefMensur->SetParent(this);
    }
    if (currentStaffDef->DrawMeterSig()) {
        m_cautionStaffDefMeterSig = new MeterSig(*currentStaffDef->GetCurrentMeterSig());
        m_cautionStaffDefMeterSig->SetParent(this);
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

FunctorCode Layer::Accept(Functor &functor)
{
    return functor.VisitLayer(this);
}

FunctorCode Layer::Accept(ConstFunctor &functor) const
{
    return functor.VisitLayer(this);
}

FunctorCode Layer::AcceptEnd(Functor &functor)
{
    return functor.VisitLayerEnd(this);
}

FunctorCode Layer::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitLayerEnd(this);
}

} // namespace vrv
