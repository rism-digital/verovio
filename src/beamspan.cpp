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

#include "comparison.h"
#include "functorparams.h"
#include "layer.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeamSpan
//----------------------------------------------------------------------------

BeamSpan::BeamSpan()
    : ControlElement("beamspan-")
    , TimeSpanningInterface()
    , BeamDrawingInterface()
    , AttColor()
    , AttBeamedWith()
    , AttBeamRend()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_BEAMEDWITH);
    RegisterAttClass(ATT_BEAMREND);

    Reset();
}

BeamSpan::~BeamSpan() {}

void BeamSpan::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    BeamDrawingInterface::Reset();
    ResetColor();
    ResetBeamedWith();
    ResetBeamRend();
}

int BeamSpan::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    Layer *layer = vrv_cast<Layer *>(GetStart()->GetFirstAncestor(LAYER));
    Staff *staff = vrv_cast<Staff *>(GetStart()->GetFirstAncestor(STAFF));
    if (!layer || !staff) return FUNCTOR_SIBLINGS;

    // find all elements between startId and endId of the beamSpan
    ClassIdsComparison classIds({ NOTE, CHORD });
    ListOfObjects objects;
    layer->FindAllDescendantBetween(&objects, &classIds, GetStart(), GetEnd(), true, false);

    // Initialize coordinates and caclculate beam segment based on the array of element for beamSpan
    ArrayOfObjects beamSpanElements(objects.begin(), objects.end());
    InitCoords(&beamSpanElements, staff, GetPlace());

    m_beamSegment.InitCoordRefs(&m_beamElementCoords);
    m_beamSegment.CalcBeam(layer, staff, params->m_doc, this, GetPlace());

    return FUNCTOR_CONTINUE;
}

} // namespace vrv