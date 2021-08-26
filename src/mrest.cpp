/////////////////////////////////////////////////////////////////////////////
// Name:        mrest.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mrest.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "fermata.h"
#include "functorparams.h"
#include "layer.h"
#include "pitchinterface.h"
#include "rest.h"
#include "staff.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRest
//----------------------------------------------------------------------------

static const ClassRegistrar<MRest> s_factory("mRest", MREST);

MRest::MRest()
    : LayerElement(MREST, "mrest-"), PositionInterface(), AttColor(), AttCue(), AttFermataPresent(), AttVisibility()
{
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CUE);
    RegisterAttClass(ATT_FERMATAPRESENT);
    RegisterAttClass(ATT_VISIBILITY);

    Reset();
}

MRest::~MRest() {}

void MRest::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    ResetColor();
    ResetCue();
    ResetFermataPresent();
    ResetVisibility();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int MRest::ConvertMarkupAnalytical(FunctorParams *functorParams)
{
    ConvertMarkupAnalyticalParams *params = vrv_params_cast<ConvertMarkupAnalyticalParams *>(functorParams);
    assert(params);

    if (this->HasFermata()) {
        Fermata *fermata = new Fermata();
        fermata->ConvertFromAnalyticalMarkup(this, this->GetUuid(), params);
    }

    return FUNCTOR_CONTINUE;
}

int MRest::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);
    PositionInterface::InterfaceResetDrawing(functorParams, this);

    return FUNCTOR_CONTINUE;
}

int MRest::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    return FUNCTOR_CONTINUE;
}

int MRest::GetOptimalLayerLocation(Staff *staff, Layer *layer, int defaultLocation)
{
    if (!layer) return defaultLocation;
    Staff *parentStaff = vrv_cast<Staff *>(GetFirstAncestor(STAFF));
    assert(parentStaff);

    // handle rest positioning for 2 layers. 3 layers and more are much more complex to solve
    if (parentStaff->GetChildCount(LAYER) != 2) return defaultLocation;

    ListOfObjects layers;
    ClassIdComparison matchType(LAYER);
    parentStaff->FindAllDescendantByComparison(&layers, &matchType);
    const bool isTopLayer = (vrv_cast<Layer *>(*layers.begin())->GetN() == layer->GetN());

    ListOfObjects::iterator otherLayerIter = isTopLayer ? std::prev(layers.end()) : layers.begin();
    ListOfObjects collidingElementsList = vrv_cast<Layer *>(*otherLayerIter)->GetLayerElementsForTimeSpanOf(this);

    // find all locations for other layer
    std::vector<int> locations;
    for (auto element : collidingElementsList) {
        if (element->Is({ CHORD, NOTE })) {
            LayerElement *layerElement = vrv_cast<LayerElement *>(element);
            int loc = PitchInterface::CalcLoc(layerElement, layer, layerElement, isTopLayer);
            locations.push_back(loc);
        }
        else if (element->Is(REST)) {
            Rest *rest = vrv_cast<Rest *>(element);
            int loc = rest->GetDrawingLoc();
            locations.push_back(loc);
        }
    }
    // if there are no other elements - just return default location
    if (locations.empty()) return defaultLocation;

    const int locAdjust = isTopLayer ? 3 : -2;
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
