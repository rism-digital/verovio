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
        Measure *nextMeasure = vrv_cast<Measure *>(parent->GetChild(index + 1));
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

//----------//
// Functors //
//----------//

int BeamSpan::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = vrv_params_cast<CalcStemParams *>(functorParams);
    assert(params);

    Layer *layer = vrv_cast<Layer *>(GetStart()->GetFirstAncestor(LAYER));
    Staff *staff = vrv_cast<Staff *>(GetStart()->GetFirstAncestor(STAFF));

    InitCoords(&m_beamedElements, staff, GetPlace());

    m_beamSegment.InitCoordRefs(&m_beamElementCoords);
    m_beamSegment.CalcBeam(layer, staff, params->m_doc, this, GetPlace());

    return FUNCTOR_CONTINUE;
}

int BeamSpan::ResolveBeamSpanElements(FunctorParams *functorParams)
{
    if (!m_beamedElements.empty()) return FUNCTOR_CONTINUE;

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

} // namespace vrv