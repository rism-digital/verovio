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
    : ControlElement(BEAMSPAN)
    , BeamDrawingInterface()
    , PlistInterface()
    , TimeSpanningInterface()
    , AttBeamedWith()
    , AttBeamRend()
{
    this->RegisterInterface(PlistInterface::GetAttClasses(), PlistInterface::IsInterface());
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_BEAMEDWITH);
    this->RegisterAttClass(ATT_BEAMREND);

    this->Reset();
    this->InitBeamSegments();
}

BeamSpan::~BeamSpan()
{
    this->ClearBeamSegments();
}

void BeamSpan::Reset()
{
    ControlElement::Reset();
    BeamDrawingInterface::Reset();
    PlistInterface::Reset();
    TimeSpanningInterface::Reset();
    this->ResetBeamedWith();
    this->ResetBeamRend();

    this->ClearBeamSegments();
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

FunctorCode BeamSpan::Accept(Functor &functor)
{
    return functor.VisitBeamSpan(this);
}

FunctorCode BeamSpan::Accept(ConstFunctor &functor) const
{
    return functor.VisitBeamSpan(this);
}

FunctorCode BeamSpan::AcceptEnd(Functor &functor)
{
    return functor.VisitBeamSpanEnd(this);
}

FunctorCode BeamSpan::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitBeamSpanEnd(this);
}

} // namespace vrv
