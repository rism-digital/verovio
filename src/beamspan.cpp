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
    for (auto segment : m_beamSegments) {
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

    for (auto segment : m_beamSegments) {
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

int BeamSpan::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    this->ClearBeamSegments();
    this->InitBeamSegments();

    return ControlElement::ResetHorizontalAlignment(functorParams);
}

int BeamSpan::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    if (!this->GetStart() || !this->GetEnd() || m_beamedElements.empty()) return FUNCTOR_CONTINUE;

    Layer *layer = vrv_cast<Layer *>(this->GetStart()->GetFirstAncestor(LAYER));
    Staff *staff = vrv_cast<Staff *>(this->GetStart()->GetFirstAncestor(STAFF));
    Measure *measure = vrv_cast<Measure *>(this->GetStart()->GetFirstAncestor(MEASURE));

    this->InitCoords(m_beamedElements, staff, this->GetPlace());

    m_beamSegments.at(0)->SetMeasure(measure);
    m_beamSegments.at(0)->SetStaff(staff);
    m_beamSegments.at(0)->SetLayer(layer);
    m_beamSegments.at(0)->SetBeginCoord(*m_beamElementCoords.begin());
    m_beamSegments.at(0)->SetEndCoord(*m_beamElementCoords.rbegin());
    ArrayOfBeamElementCoords coord(m_beamElementCoords.begin(), m_beamElementCoords.end());
    m_beamSegments.at(0)->InitCoordRefs(&coord);
    m_beamSegments.at(0)->CalcBeam(layer, staff, params->m_doc, this, this->GetPlace());

    return FUNCTOR_CONTINUE;
}

int BeamSpan::CalcSpanningBeamSpans(FunctorParams *functorParams)
{
    FunctorDocParams *params = vrv_params_cast<FunctorDocParams *>(functorParams);
    assert(params);

    if (m_beamedElements.empty() || !this->GetStart() || !this->GetEnd()) return FUNCTOR_CONTINUE;

    Object *startSystem = this->GetStart()->GetFirstAncestor(SYSTEM);
    Object *endSystem = this->GetEnd()->GetFirstAncestor(SYSTEM);
    assert(startSystem && endSystem);
    if (startSystem == endSystem) return FUNCTOR_CONTINUE;

    // Find layerElements that belong to another system and store then in the vector alongside
    // the system they belong to. This will allow us to break down beamSpan based on the systems
    auto iter = m_beamedElements.begin();
    SpanIndexVector elements;
    Object *firstSystem = startSystem;
    while (iter != m_beamedElements.end()) {
        elements.push_back({ iter, firstSystem });
        iter = std::find_if(iter, m_beamedElements.end(), [&firstSystem](Object *element) {
            Object *parentSystem = element->GetFirstAncestor(SYSTEM);
            if (firstSystem == parentSystem) return false;
            firstSystem = parentSystem;
            return true;
        });
    }
    elements.push_back({ m_beamedElements.end(), NULL });

    // Iterator for the elements are based on the initial order of the elements, so skip current system when
    // found and process it separately in the end
    Object *currentSystem = this->GetFirstAncestor(SYSTEM);
    int currentSystemIndex = 0;
    for (int i = 0; i < (int)elements.size() - 1; ++i) {
        if (elements.at(i).second == currentSystem) {
            currentSystemIndex = i;
            continue;
        }
        this->AddSpanningSegment(params->m_doc, elements, i);
    }
    this->AddSpanningSegment(params->m_doc, elements, currentSystemIndex, false);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
