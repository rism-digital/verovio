/////////////////////////////////////////////////////////////////////////////
// Name:        mrest.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mrest.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "fermata.h"
#include "functor.h"
#include "layer.h"
#include "pitchinterface.h"
#include "rest.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRest
//----------------------------------------------------------------------------

static const ClassRegistrar<MRest> s_factory("mRest", MREST);

MRest::MRest() : LayerElement(MREST), PositionInterface(), AttColor(), AttCue(), AttFermataPresent(), AttVisibility()
{
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_CUE);
    this->RegisterAttClass(ATT_FERMATAPRESENT);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
}

MRest::~MRest() {}

void MRest::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    this->ResetColor();
    this->ResetCue();
    this->ResetFermataPresent();
    this->ResetVisibility();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode MRest::Accept(Functor &functor)
{
    return functor.VisitMRest(this);
}

FunctorCode MRest::Accept(ConstFunctor &functor) const
{
    return functor.VisitMRest(this);
}

FunctorCode MRest::AcceptEnd(Functor &functor)
{
    return functor.VisitMRestEnd(this);
}

FunctorCode MRest::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitMRestEnd(this);
}

int MRest::GetOptimalLayerLocation(const Layer *layer, int defaultLocation) const
{
    if (!layer) return defaultLocation;
    const Staff *parentStaff = this->GetAncestorStaff();

    // handle rest positioning for 2 layers. 3 layers and more are much more complex to solve
    if (parentStaff->GetChildCount(LAYER) != 2) return defaultLocation;

    ListOfConstObjects layers = parentStaff->FindAllDescendantsByType(LAYER, false);
    const bool isTopLayer = (vrv_cast<const Layer *>(*layers.begin())->GetN() == layer->GetN());

    ListOfConstObjects::iterator otherLayerIter = isTopLayer ? std::prev(layers.end()) : layers.begin();
    ListOfConstObjects collidingElementsList
        = vrv_cast<const Layer *>(*otherLayerIter)->GetLayerElementsForTimeSpanOf(this);

    // find all locations for other layer
    std::vector<int> locations;
    for (const Object *element : collidingElementsList) {
        if (element->Is({ CHORD, NOTE })) {
            const LayerElement *layerElement = vrv_cast<const LayerElement *>(element);
            int loc = PitchInterface::CalcLoc(layerElement, layer, layerElement, isTopLayer);
            locations.push_back(loc);
        }
        else if (element->Is(REST)) {
            const Rest *rest = vrv_cast<const Rest *>(element);
            int loc = rest->GetDrawingLoc();
            locations.push_back(loc);
        }
        else if (element->Is(MREST)) {
            locations.push_back(4);
        }
    }
    // if there are no other elements - just return default location
    if (locations.empty()) return defaultLocation;

    const int locAdjust = isTopLayer ? 4 : -3;
    int extremePoint = isTopLayer ? *std::max_element(locations.begin(), locations.end())
                                  : *std::min_element(locations.begin(), locations.end());
    extremePoint += locAdjust;
    if (extremePoint % 2 != 0) {
        extremePoint += isTopLayer ? 1 : -1;
    }
    // Make sure that lower layer don't go above centre, and vice versa for upper layer.
    // Hardcoded, so for the time being this is going to properly adjust mRests only on the 5-line staves
    if (isTopLayer && (extremePoint < 6)) {
        extremePoint = 6;
    }
    else if (!isTopLayer && (extremePoint > 4)) {
        extremePoint = 4;
    }

    return extremePoint;
}

} // namespace vrv
