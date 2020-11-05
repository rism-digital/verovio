/////////////////////////////////////////////////////////////////////////////
// Name:        beamspan.cpp
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "beamspan.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "barline.h"
#include "comparison.h"
#include "functorparams.h"
#include "layer.h"
#include "measure.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeamSpan
//----------------------------------------------------------------------------

BeamSpan::BeamSpan()
    : ControlElement("beamspan-")
    , BeamDrawingInterface()
    , PlistInterface()
    , TimeSpanningInterface()
    , AttBeamedWith()
    , AttBeamRend()
    , AttColor()
{
    RegisterInterface(PlistInterface::GetAttClasses(), PlistInterface::IsInterface());
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_BEAMEDWITH);
    RegisterAttClass(ATT_BEAMREND);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_PLIST);

    Reset();
}

BeamSpan::~BeamSpan() {}

void BeamSpan::Reset()
{
    ControlElement::Reset();
    BeamDrawingInterface::Reset();
    PlistInterface::Reset();
    TimeSpanningInterface::Reset();
    ResetBeamedWith();
    ResetBeamRend();
    ResetColor();
    ResetPlist();

    m_spanningType = SPANNING_START_END;
}

void BeamSpan::AppendSpanningCoordinates(Measure *measure)
{
    const int spanningType = GetSpanningType();
    if (SPANNING_START_END == spanningType) return;
    
    BarLine *bar = measure->GetRightBarLine();
    const int rightSide = bar->GetDrawingX();
    BeamElementCoord *front = m_beamSegment.m_beamElementCoordRefs.front();
    BeamElementCoord *back = m_beamSegment.m_beamElementCoordRefs.back();
    const double slope = (double)(back->m_yBeam - front->m_yBeam) / (double)(back->m_x - front->m_x);

    if ((SPANNING_START == spanningType) || (SPANNING_MIDDLE == spanningType)) {
        BeamElementCoord *right = new BeamElementCoord(*back);
        const int distance = rightSide - back->m_x;
        right->m_x = rightSide;
        right->m_yBeam += distance * slope;
        m_beamSegment.m_beamElementCoordRefs.push_back(right);
    }
    if ((SPANNING_END == spanningType) || (SPANNING_MIDDLE == spanningType)) {
        BeamElementCoord *left = new BeamElementCoord(*front);
        const int width = measure->GetInnerWidth();
        const int offset = (back->m_x - front->m_x) / (m_beamSegment.m_beamElementCoordRefs.size() - 1) / 2;
        left->m_x -= offset;
        left->m_yBeam -= offset * slope;
        m_beamSegment.m_beamElementCoordRefs.insert(m_beamSegment.m_beamElementCoordRefs.begin(), left);
    }
}

ArrayOfObjects BeamSpan::GetBeamSpanElementList(Layer *layer, Staff *staff)
{
    // find all elements between startId and endId of the beamSpan
    ClassIdsComparison classIds({ NOTE, CHORD });
    ListOfObjects objects;
    layer->FindAllDescendantBetween(&objects, &classIds, GetStart(), GetEnd(), true, false);

    ArrayOfObjects beamSpanElements(objects.begin(), objects.end());
    // If last element is not equal to the end, there is high chance that this beamSpan is cross-measure.
    // Look for the same N-staff N-layer in next measure and try finding end there
    if (beamSpanElements.back() != GetEnd()) {
        Measure *measure = vrv_cast<Measure *>(GetStart()->GetFirstAncestor(MEASURE));
        Object *parent = measure->GetParent();

        const int index = parent->GetChildIndex(measure);
        Measure *nextMeasure = vrv_cast<Measure *>(parent->GetNext(measure, MEASURE));
        if (nextMeasure) {
            AttNIntegerComparison snc(STAFF, staff->GetN());
            Staff *nextStaff = vrv_cast<Staff *>(nextMeasure->FindDescendantByComparison(&snc));
            if (nextStaff) {
                AttNIntegerComparison lnc(LAYER, layer->GetN());
                Layer *nextStaffLayer = vrv_cast<Layer *>(nextStaff->FindDescendantByComparison(&lnc));
                if (nextStaffLayer) {
                    // find all elements between startId and endId of the beamSpan
                    ClassIdsComparison classIds({ NOTE, CHORD });
                    ListOfObjects nextLayerObjects;
                    // pass NULL as starting element to add all elements until end is reached
                    nextStaffLayer->FindAllDescendantBetween(&nextLayerObjects, &classIds, NULL, GetEnd(), true, false);
                    // Handle only next measure for the time being
                    if (nextLayerObjects.back() == GetEnd()) {
                        beamSpanElements.insert(
                            beamSpanElements.end(), nextLayerObjects.begin(), nextLayerObjects.end());
                    }
                }
            }
        }
    }

    return beamSpanElements;
}

bool BeamSpan::SeparateSpanningElements(ArrayOfObjects &newElements, Object *first, Doc *doc, bool clearElements)
{
    Layer *layer = vrv_cast<Layer *>(first->GetFirstAncestor(LAYER));
    Staff *staff = vrv_cast<Staff *>(first->GetFirstAncestor(STAFF));
    if (!layer || !staff) return false;

    m_beamedElements = newElements;
    if (clearElements) m_beamElementCoords.clear();
    InitCoords(&m_beamedElements, staff, GetPlace());

    m_beamSegment.InitCoordRefs(&m_beamElementCoords);
    m_beamSegment.CalcBeam(layer, staff, doc, this, GetPlace());
    for (auto element : newElements) {
        vrv_cast<LayerElement *>(element)->m_referencedElement = this;
    }

    // Reset Start and End based on the new elements of the beamSpan
    TimePointInterface::Reset();
    TimeSpanningInterface::Reset();
    SetStart(vrv_cast<LayerElement *>(newElements.front()));
    SetEnd(vrv_cast<LayerElement *>(newElements.back()));
    return true;
}

void BeamSpan::SetSpanningType(int systemIndex, int systemCount)
{
    if (0 == systemIndex) {
        m_spanningType = SPANNING_START;
    }
    else if ((systemCount - 1) == systemIndex) {
        m_spanningType = SPANNING_END;
    }
    else {
        m_spanningType = SPANNING_MIDDLE;
    }
}

//----------//
// Functors //
//----------//

int BeamSpan::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    if (!GetStart()) return FUNCTOR_CONTINUE;

    Layer *layer = vrv_cast<Layer *>(GetStart()->GetFirstAncestor(LAYER));
    Staff *staff = vrv_cast<Staff *>(GetStart()->GetFirstAncestor(STAFF));

    InitCoords(&m_beamedElements, staff, GetPlace());

    m_beamSegment.InitCoordRefs(&m_beamElementCoords);
    m_beamSegment.CalcBeam(layer, staff, params->m_doc, this, GetPlace());

    return FUNCTOR_CONTINUE;
}

int BeamSpan::ResolveBeamSpanElements(FunctorParams *functorParams)
{
    if (!m_beamedElements.empty() || !GetStart()) return FUNCTOR_CONTINUE;

    Layer *layer = vrv_cast<Layer *>(GetStart()->GetFirstAncestor(LAYER));
    Staff *staff = vrv_cast<Staff *>(GetStart()->GetFirstAncestor(STAFF));
    if (!layer || !staff) return FUNCTOR_SIBLINGS;

    m_beamedElements = HasPlist() ? *GetRefs() : GetBeamSpanElementList(layer, staff);

    // Find whether this beamSpan is crossStaff - if there are beamed elements that belong
    // to differen staffs we can consider this beamSpan as such
    bool isCrossStaff = false;
    for (const auto element : m_beamedElements) {
        if (element->GetFirstAncestor(STAFF) != staff) {
            isCrossStaff = true;
            break;
        }
    }

    // set current beamSpan as referencedElement for all beamed elemenents (for the
    // sake of figuring if corresponding element is in beamSpan)
    for (const auto element : m_beamedElements) {
        LayerElement *layerElem = vrv_cast<LayerElement *>(element);
        if (!layerElem) continue;

        layerElem->m_referencedElement = this;

        if (isCrossStaff) {
            Layer *elementLayer = vrv_cast<Layer *>(layerElem->GetFirstAncestor(LAYER));
            Staff *elementStaff = vrv_cast<Staff *>(layerElem->GetFirstAncestor(STAFF));
            if (!layer || !staff || layerElem->m_crossStaff) continue;

            layerElem->m_crossStaff = elementStaff;
            layerElem->m_crossLayer = elementLayer;
        }
    }

    return FUNCTOR_CONTINUE;
}

int BeamSpan::ResolveSpanningBeamSpans(FunctorParams *functorParams)
{
    FunctorDocParams *params = vrv_params_cast<FunctorDocParams *>(functorParams);
    assert(params);

    if (m_beamedElements.empty() || !GetStart()) return FUNCTOR_CONTINUE;

    Object *startSystem = GetStart()->GetFirstAncestor(SYSTEM);
    Object *endSystem = GetEnd()->GetFirstAncestor(SYSTEM);
    assert(startSystem && endSystem);
    if (startSystem == endSystem) return FUNCTOR_CONTINUE;

    // Find layerElements that belong to another system and store then in the vector alongside
    // the system they belong to. This will allow us to break down beamSpan based on the systems
    auto iter = m_beamedElements.begin();
    std::vector<std::pair<vrv::ArrayOfObjects::iterator, Object *> > elements;
    Object *firstSystem = startSystem;
    while (iter != m_beamedElements.end()) {
        elements.push_back({ iter, firstSystem });
        iter = find_if(iter, m_beamedElements.end(), [&firstSystem](Object *element) {
            Object *parentSystem = element->GetFirstAncestor(SYSTEM);
            if (firstSystem == parentSystem) return false;
            firstSystem = parentSystem;
            return true;
        });
    }
    elements.push_back({ m_beamedElements.end(), NULL });

    // Iterator for the elements are based on the initial order of the elements, so skip current system when
    // found and process it separatelly in the end
    Object *currentSystemn = GetFirstAncestor(SYSTEM);
    int currentSystemIndex = 0;
    for (int i = 0; i < elements.size() - 1; ++i) {
        if (elements.at(i).second == currentSystemn) {
            currentSystemIndex = i;
            continue;
        }

        BeamSpan *beamSpan = vrv_cast<BeamSpan *>(Clone());
        ArrayOfObjects newElements(elements.at(i).first, elements.at(i + 1).first);
        beamSpan->SeparateSpanningElements(newElements, *elements.at(i).first, params->m_doc, true);
        beamSpan->SetSpanningType(i, elements.size() - 1);

        Measure *measure = vrv_cast<Measure *>((*elements.at(i).first)->GetFirstAncestor(MEASURE));
        measure->AddChild(beamSpan);
    }

    ArrayOfObjects newElements(elements.at(currentSystemIndex).first, elements.at(currentSystemIndex + 1).first);
    SeparateSpanningElements(newElements, GetStart(), params->m_doc, false);
    SetSpanningType(currentSystemIndex, elements.size() - 1);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv