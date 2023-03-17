/////////////////////////////////////////////////////////////////////////////
// Name:        beamspan.cpp
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "beamspan.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "barline.h"
#include "comparison.h"
#include "functor.h"
#include "functorparams.h"
#include "layer.h"
#include "measure.h"
#include "staff.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeamSpan
//----------------------------------------------------------------------------
static const ClassRegistrar<BeamSpan> s_factory("beamSpan", BEAMSPAN);

BeamSpan::BeamSpan()
    : ControlElement(BEAMSPAN, "beamspan-")
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

    Reset();
    InitBeamSegments();
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

    ClearBeamSegments();
}

void BeamSpan::InitBeamSegments()
{
    // BeamSpan should have at least one segment to begin with
    m_beamSegments.emplace_back(new BeamSpanSegment());

    m_isSpanningElement = true;
}

void BeamSpan::ClearBeamSegments()
{
    for (BeamSpanSegment *segment : m_beamSegments) {
        delete segment;
    }
    m_beamSegments.clear();
}

BeamSpanSegment *BeamSpan::GetSegmentForSystem(const System *system)
{
    return const_cast<BeamSpanSegment *>(std::as_const(*this).GetSegmentForSystem(system));
}

const BeamSpanSegment *BeamSpan::GetSegmentForSystem(const System *system) const
{
    assert(system);

    for (BeamSpanSegment *segment : m_beamSegments) {
        // make sure to process only segments for current system
        const Measure *segmentSystem = segment->GetMeasure();
        if (segmentSystem && vrv_cast<const System *>(segmentSystem->GetFirstAncestor(SYSTEM)) == system)
            return segment;
    }
    return NULL;
}

ArrayOfObjects BeamSpan::GetBeamSpanElementList(Layer *layer, const Staff *staff)
{
    // find all elements between startId and endId of the beamSpan
    ClassIdsComparison classIds({ NOTE, CHORD });
    ListOfObjects objects;
    layer->FindAllDescendantsBetween(&objects, &classIds, this->GetStart(), this->GetEnd(), true, 3);
    // To make sure that notes from tuplets and btrems are included, lookup for decendants is done up to depth of 3.
    // However this might result in notes from chords being added as standalone elements. To avoid this we remove any
    // note that is in the span and is a chord tone. Same happens when nextLayerObjects are being processed.
    objects.erase(std::remove_if(objects.begin(), objects.end(),
                      [](Object *object) { return object->Is(NOTE) && vrv_cast<Note *>(object)->IsChordTone(); }),
        objects.end());

    if (objects.empty()) return {};

    ArrayOfObjects beamSpanElements(objects.begin(), objects.end());
    // If last element is not equal to the end, there is high chance that this beamSpan is cross-measure.
    // Look for the same N-staff N-layer in next measure and try finding end there
    Measure *startMeasure = vrv_cast<Measure *>(this->GetStart()->GetFirstAncestor(MEASURE));
    Measure *endMeasure = vrv_cast<Measure *>(this->GetEnd()->GetFirstAncestor(MEASURE));
    Measure *nextMeasure = NULL;
    while ((beamSpanElements.back() != this->GetEnd()) && (startMeasure != endMeasure)) {
        Object *parent = startMeasure->GetParent();

        nextMeasure = vrv_cast<Measure *>(parent->GetNext(startMeasure, MEASURE));
        if (!nextMeasure) break;

        AttNIntegerComparison snc(STAFF, staff->GetN());
        Staff *nextStaff = vrv_cast<Staff *>(nextMeasure->FindDescendantByComparison(&snc));
        if (!nextStaff) break;

        AttNIntegerComparison lnc(LAYER, layer->GetN());
        Layer *nextStaffLayer = vrv_cast<Layer *>(nextStaff->FindDescendantByComparison(&lnc));
        if (!nextStaffLayer) break;

        // find all elements between startId and endId of the beamSpan
        ClassIdsComparison classIds({ NOTE, CHORD });
        ListOfObjects nextLayerObjects;
        // pass NULL as starting element to add all elements until end is reached
        if (endMeasure == nextMeasure) {
            nextStaffLayer->FindAllDescendantsBetween(&nextLayerObjects, &classIds, NULL, this->GetEnd(), true, 3);
            nextLayerObjects.erase(
                std::remove_if(nextLayerObjects.begin(), nextLayerObjects.end(),
                    [](Object *object) { return object->Is(NOTE) && vrv_cast<Note *>(object)->IsChordTone(); }),
                nextLayerObjects.end());
            // Handle only next measure for the time being
            if (nextLayerObjects.back() == this->GetEnd()) {
                beamSpanElements.insert(beamSpanElements.end(), nextLayerObjects.begin(), nextLayerObjects.end());
            }
        }
        else {
            nextStaffLayer->FindAllDescendantsByComparison(&nextLayerObjects, &classIds);
            beamSpanElements.insert(beamSpanElements.end(), nextLayerObjects.begin(), nextLayerObjects.end());
        }

        startMeasure = nextMeasure;
    }

    return beamSpanElements;
}

bool BeamSpan::AddSpanningSegment(const Doc *doc, const SpanIndexVector &elements, int index, bool newSegment)
{
    Layer *layer = vrv_cast<Layer *>((*elements.at(index).first)->GetFirstAncestor(LAYER));
    Staff *staff = vrv_cast<Staff *>((*elements.at(index).first)->GetFirstAncestor(STAFF));
    if (!layer || !staff) return false;

    // get iterators for first and last coordinates in the range for the segment
    const auto coordsFirst = std::find_if(m_beamElementCoords.begin(), m_beamElementCoords.end(),
        [&](BeamElementCoord *coord) { return coord->m_element == (*elements.at(index).first); });
    const auto coordsLast = std::find_if(m_beamElementCoords.begin(), m_beamElementCoords.end(),
        [&](BeamElementCoord *coord) { return coord->m_element == *(elements.at(index + 1).first - 1); });
    if ((coordsFirst == m_beamElementCoords.end()) || (coordsLast == m_beamElementCoords.end())) return false;

    BeamSpanSegment *segment = NULL;
    if (newSegment) {
        segment = new BeamSpanSegment();
    }
    else {
        segment = m_beamSegments.at(0);
    }

    // Init segment with placement information (measure, staff, etc.) as well as begin/end coordinates
    ArrayOfBeamElementCoords coords(coordsFirst, coordsLast + 1);
    segment->SetStaff(staff);
    segment->SetLayer(layer);
    segment->SetBeginCoord(*coordsFirst);
    segment->SetEndCoord(*coordsLast);
    segment->InitCoordRefs(&coords);
    segment->CalcBeam(layer, staff, doc, this, m_drawingPlace);
    segment->SetSpanningType(index, (int)elements.size() - 1);
    Object *currentSystem = layer->GetFirstAncestor(SYSTEM);
    if (segment->GetSpanningType() == SPANNING_START) {
        segment->SetMeasure(vrv_cast<Measure *>(currentSystem->GetLast(MEASURE)));
    }
    else if (segment->GetSpanningType() == SPANNING_END) {
        segment->SetMeasure(vrv_cast<Measure *>(currentSystem->GetFirst(MEASURE)));
    }
    else {
        segment->SetMeasure(vrv_cast<Measure *>((*elements.at(index).first)->GetFirstAncestor(MEASURE)));
    }

    if (newSegment) {
        m_beamSegments.push_back(segment);
    }

    return true;
}

//----------//
// Functors //
//----------//

FunctorCode BeamSpan::Accept(MutableFunctor &functor)
{
    return functor.VisitBeamSpan(this);
}

FunctorCode BeamSpan::Accept(ConstFunctor &functor) const
{
    return functor.VisitBeamSpan(this);
}

FunctorCode BeamSpan::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitBeamSpanEnd(this);
}

FunctorCode BeamSpan::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitBeamSpanEnd(this);
}

} // namespace vrv
