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

    InitBeamSegments();

    Reset();
}

BeamSpan::~BeamSpan() 
{
    ClearBeamSegments();
}

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
}

void BeamSpan::InitBeamSegments() 
{
    // BeamSpan should have at least one segment to begin with
    m_beamSegments.emplace_back(new BeamSegment());
}

void BeamSpan::ClearBeamSegments()
{
    for (auto segment : m_beamSegments) {
        delete segment;
    }
    m_beamSegments.clear();
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

bool BeamSpan::AddSpanningSegment(Doc *doc, const SpanIndexVector &elements, int index, bool newSegment)
{
    Layer *layer = vrv_cast<Layer *>((*elements.at(index).first)->GetFirstAncestor(LAYER));
    Staff *staff = vrv_cast<Staff *>((*elements.at(index).first)->GetFirstAncestor(STAFF));
    Measure *measure = vrv_cast<Measure *>((*elements.at(index).first)->GetFirstAncestor(MEASURE));
    if (!layer || !staff || !measure) return false;

    // get iterators for first and last coordinates in the range for the segment
    auto coordsFirst = std::find_if(m_beamElementCoords.begin(), m_beamElementCoords.end(),
        [&](BeamElementCoord *coord) { return coord->m_element == (*elements.at(index).first); });
    auto coordsLast = std::find_if(m_beamElementCoords.begin(), m_beamElementCoords.end(),
        [&](BeamElementCoord *coord) { return coord->m_element == *(elements.at(index + 1).first - 1); });
    if ((coordsFirst == m_beamElementCoords.end()) || (coordsLast == m_beamElementCoords.end())) return false;

    
    BeamSegment *segment = NULL;
    if (newSegment) {
        segment = new BeamSegment();
    }
    else {
        segment = m_beamSegments.at(0);
    }

    // Init segment with placement information (measure, staff, etc.) as well as begin/end coordinates
    ArrayOfBeamElementCoords coords(coordsFirst, coordsLast + 1);
    segment->InitPlacementInformation(measure, staff, layer);
    segment->m_placementInfo->m_begin = coordsFirst;
    segment->m_placementInfo->m_end = coordsLast + 1;
    segment->InitCoordRefs(&coords);
    segment->CalcBeam(layer, staff, doc, this, GetPlace());
    segment->m_placementInfo->SetSpanningType(index, elements.size() - 1);

    if (newSegment) {
        m_beamSegments.push_back(segment);
    }

    return true;
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
    Measure *measure = vrv_cast<Measure *>(GetStart()->GetFirstAncestor(MEASURE));

    InitCoords(&m_beamedElements, staff, GetPlace());

    m_beamSegments.at(0)->InitPlacementInformation(measure, staff, layer);
    m_beamSegments.at(0)->m_placementInfo->m_begin = m_beamElementCoords.begin();
    m_beamSegments.at(0)->m_placementInfo->m_end = m_beamElementCoords.end();
    ArrayOfBeamElementCoords coord(
        m_beamSegments.at(0)->m_placementInfo->m_begin, m_beamSegments.at(0)->m_placementInfo->m_end);
    m_beamSegments.at(0)->InitCoordRefs(&coord);
    m_beamSegments.at(0)->CalcBeam(layer, staff, params->m_doc, this, GetPlace());

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
    SpanIndexVector elements;
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
        AddSpanningSegment(params->m_doc, elements, i);
    }
    AddSpanningSegment(params->m_doc, elements, currentSystemIndex, false);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv