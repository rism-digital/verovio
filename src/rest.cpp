/////////////////////////////////////////////////////////////////////////////
// Name:        rest.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "rest.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "fermata.h"
#include "functorparams.h"
#include "layer.h"
#include "smufl.h"
#include "staff.h"
#include "system.h"
#include "transposition.h"
#include "vrv.h"

namespace vrv {

typedef std::map<RestLayer,
    std::map<RestAccidental, std::map<RestLayerPlace, std::map<RestNotePlace, std::map<int, int> > > > >
    RestOffsets;

RestOffsets g_defaultRests{
    { RL_otherLayer,
        { { RA_none,
              { { RLP_restOnTopLayer,
                    { { RNP_noteInSpace,
                          { { DUR_1, 3 }, { DUR_2, 3 }, { DUR_4, 5 }, { DUR_8, 5 }, { DUR_16, 7 }, { DUR_32, 7 },
                              { DUR_64, 9 }, { DUR_128, 9 }, { DUR_LG, 5 }, { DUR_BR, 5 } } },
                        { RNP_noteOnLine,
                            { { DUR_1, 2 }, { DUR_2, 4 }, { DUR_4, 6 }, { DUR_8, 4 }, { DUR_16, 6 }, { DUR_32, 6 },
                                { DUR_64, 8 }, { DUR_128, 8 }, { DUR_LG, 6 }, { DUR_BR, 4 } } } } },
                  { RLP_restOnBottomLayer,
                      { { RNP_noteInSpace,
                            { { DUR_1, -5 }, { DUR_2, -5 }, { DUR_4, -5 }, { DUR_8, -5 }, { DUR_16, -5 },
                                { DUR_32, -7 }, { DUR_64, -7 }, { DUR_128, -9 }, { DUR_LG, -5 }, { DUR_BR, -5 } } },
                          { RNP_noteOnLine,
                              { { DUR_1, -6 }, { DUR_2, -6 }, { DUR_4, -6 }, { DUR_8, -4 }, { DUR_16, -4 },
                                  { DUR_32, -6 }, { DUR_64, -6 }, { DUR_128, -8 }, { DUR_LG, -6 },
                                  { DUR_BR, -6 } } } } } } },
            { RA_s,
                { { RLP_restOnTopLayer,
                      { { RNP_noteInSpace,
                            { { DUR_1, 3 }, { DUR_2, 5 }, { DUR_4, 7 }, { DUR_8, 5 }, { DUR_16, 7 }, { DUR_32, 7 },
                                { DUR_64, 9 }, { DUR_128, 9 }, { DUR_LG, 5 }, { DUR_BR, 5 } } },
                          { RNP_noteOnLine,
                              { { DUR_1, 2 }, { DUR_2, 4 }, { DUR_4, 6 }, { DUR_8, 6 }, { DUR_16, 8 }, { DUR_32, 8 },
                                  { DUR_64, 10 }, { DUR_128, 10 }, { DUR_LG, 6 }, { DUR_BR, 4 } } } } },
                    { RLP_restOnBottomLayer,
                        { { RNP_noteInSpace,
                              { { DUR_1, -5 }, { DUR_2, -5 }, { DUR_4, -5 }, { DUR_8, -5 }, { DUR_16, -5 },
                                  { DUR_32, -7 }, { DUR_64, -7 }, { DUR_128, -9 }, { DUR_LG, -5 }, { DUR_BR, -5 } } },
                            { RNP_noteOnLine,
                                { { DUR_1, -6 }, { DUR_2, -6 }, { DUR_4, -6 }, { DUR_8, -6 }, { DUR_16, -6 },
                                    { DUR_32, -6 }, { DUR_64, -6 }, { DUR_128, -8 }, { DUR_LG, -6 },
                                    { DUR_BR, -6 } } } } } } },
            { RA_f,
                { { RLP_restOnTopLayer,
                      { { RNP_noteInSpace,
                            { { DUR_1, 3 }, { DUR_2, 5 }, { DUR_4, 5 }, { DUR_8, 5 }, { DUR_16, 7 }, { DUR_32, 7 },
                                { DUR_64, 9 }, { DUR_128, 9 }, { DUR_LG, 5 }, { DUR_BR, 5 } } },
                          { RNP_noteOnLine,
                              { { DUR_1, 4 }, { DUR_2, 4 }, { DUR_4, 6 }, { DUR_8, 6 }, { DUR_16, 8 }, { DUR_32, 8 },
                                  { DUR_64, 10 }, { DUR_128, 10 }, { DUR_LG, 6 }, { DUR_BR, 4 } } } } },
                    { RLP_restOnBottomLayer,
                        { { RNP_noteInSpace,
                              { { DUR_1, -5 }, { DUR_2, -5 }, { DUR_4, -5 }, { DUR_8, -5 }, { DUR_16, -5 },
                                  { DUR_32, -7 }, { DUR_64, -7 }, { DUR_128, -9 }, { DUR_LG, -5 }, { DUR_BR, -5 } } },
                            { RNP_noteOnLine,
                                { { DUR_1, -6 }, { DUR_2, -6 }, { DUR_4, -6 }, { DUR_8, -4 }, { DUR_16, -4 },
                                    { DUR_32, -6 }, { DUR_64, -6 }, { DUR_128, -8 }, { DUR_LG, -6 },
                                    { DUR_BR, -6 } } } } } } },
            { RA_x,
                { { RLP_restOnTopLayer,
                      { { RNP_noteInSpace,
                            { { DUR_1, 3 }, { DUR_2, 3 }, { DUR_4, 5 }, { DUR_8, 5 }, { DUR_16, 7 }, { DUR_32, 7 },
                                { DUR_64, 9 }, { DUR_128, 9 }, { DUR_LG, 5 }, { DUR_BR, 5 } } },
                          { RNP_noteOnLine,
                              { { DUR_1, 2 }, { DUR_2, 4 }, { DUR_4, 6 }, { DUR_8, 6 }, { DUR_16, 8 }, { DUR_32, 8 },
                                  { DUR_64, 10 }, { DUR_128, 10 }, { DUR_LG, 6 }, { DUR_BR, 4 } } } } },
                    { RLP_restOnBottomLayer,
                        { { RNP_noteInSpace,
                              { { DUR_1, -5 }, { DUR_2, -5 }, { DUR_4, -5 }, { DUR_8, -5 }, { DUR_16, -5 },
                                  { DUR_32, -7 }, { DUR_64, -7 }, { DUR_128, -9 }, { DUR_LG, -5 }, { DUR_BR, -5 } } },
                            { RNP_noteOnLine,
                                { { DUR_1, -6 }, { DUR_2, -4 }, { DUR_4, -6 }, { DUR_8, -4 }, { DUR_16, -4 },
                                    { DUR_32, -6 }, { DUR_64, -6 }, { DUR_128, -8 }, { DUR_LG, -6 },
                                    { DUR_BR, -6 } } } } } } },
            { RA_x,
                { { RLP_restOnTopLayer,
                      { { RNP_noteInSpace,
                            { { DUR_1, 3 }, { DUR_2, 3 }, { DUR_4, 5 }, { DUR_8, 5 }, { DUR_16, 7 }, { DUR_32, 7 },
                                { DUR_64, 9 }, { DUR_128, 9 }, { DUR_LG, 5 }, { DUR_BR, 5 } } },
                          { RNP_noteOnLine,
                              { { DUR_1, 2 }, { DUR_2, 6 }, { DUR_4, 6 }, { DUR_8, 6 }, { DUR_16, 8 }, { DUR_32, 8 },
                                  { DUR_64, 10 }, { DUR_128, 10 }, { DUR_LG, 6 }, { DUR_BR, 4 } } } } },
                    { RLP_restOnBottomLayer,
                        { { RNP_noteInSpace,
                              { { DUR_1, -7 }, { DUR_2, -5 }, { DUR_4, -7 }, { DUR_8, -5 }, { DUR_16, -5 },
                                  { DUR_32, -7 }, { DUR_64, -7 }, { DUR_128, -9 }, { DUR_LG, -5 }, { DUR_BR, -5 } } },
                            { RNP_noteOnLine,
                                { { DUR_1, -6 }, { DUR_2, -6 }, { DUR_4, -6 }, { DUR_8, -6 }, { DUR_16, -6 },
                                    { DUR_32, -6 }, { DUR_64, -6 }, { DUR_128, -8 }, { DUR_LG, -6 },
                                    { DUR_BR, -6 } } } } } } } } },
    { RL_sameLayer,
        { { RA_none,
            { { RLP_restOnTopLayer,
                  { { RNP_noteInSpace,
                        { { DUR_1, -1 }, { DUR_2, 1 }, { DUR_4, 3 }, { DUR_8, 1 }, { DUR_16, 3 }, { DUR_32, 3 },
                            { DUR_64, 5 }, { DUR_128, 5 }, { DUR_LG, 3 }, { DUR_BR, 1 } } },
                      { RNP_noteOnLine,
                          { { DUR_1, 0 }, { DUR_2, 0 }, { DUR_4, 2 }, { DUR_8, 2 }, { DUR_16, 2 }, { DUR_32, 2 },
                              { DUR_64, 4 }, { DUR_128, 4 }, { DUR_LG, 2 }, { DUR_BR, 2 } } } } },
                { RLP_restOnBottomLayer,
                    { { RNP_noteInSpace,
                          { { DUR_1, -3 }, { DUR_2, -1 }, { DUR_4, -3 }, { DUR_8, -1 }, { DUR_16, -1 }, { DUR_32, -3 },
                              { DUR_64, -3 }, { DUR_128, -5 }, { DUR_LG, -3 }, { DUR_BR, -3 } } },
                        { RNP_noteOnLine,
                            { { DUR_1, -2 }, { DUR_2, -2 }, { DUR_4, -4 }, { DUR_8, -2 }, { DUR_16, -2 },
                                { DUR_32, -4 }, { DUR_64, -4 }, { DUR_128, -6 }, { DUR_LG, -2 },
                                { DUR_BR, -2 } } } } } } } } }
};

// helper function for conversion
RestAccidental MeiAccidentalToRestAccidental(data_ACCIDENTAL_WRITTEN accidental)
{
    switch (accidental) {
        case ACCIDENTAL_WRITTEN_s: return RA_s;
        case ACCIDENTAL_WRITTEN_f: return RA_f;
        case ACCIDENTAL_WRITTEN_x: return RA_x;
        case ACCIDENTAL_WRITTEN_n: return RA_n;
        default: return RA_none;
    }
}

//----------------------------------------------------------------------------
// Rest
//----------------------------------------------------------------------------

Rest::Rest()
    : LayerElement("rest-"), DurationInterface(), PositionInterface(), AttColor(), AttCue(), AttRestVisMensural()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CUE);
    RegisterAttClass(ATT_RESTVISMENSURAL);
    Reset();
}

Rest::~Rest() {}

void Rest::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
    PositionInterface::Reset();
    ResetColor();
    ResetCue();
    ResetRestVisMensural();
}

bool Rest::IsSupportedChild(Object *child)
{
    if (child->Is(DOTS)) {
        assert(dynamic_cast<Dots *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

void Rest::AddChild(Object *child)
{
    if (!this->IsSupportedChild(child)) {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return;
    }

    child->SetParent(this);

    ArrayOfObjects *children = this->GetChildrenForModification();

    // Dots are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is(DOTS)) {
        children->insert(children->begin(), child);
    }
    else {
        children->push_back(child);
    }
    Modify();
}

wchar_t Rest::GetRestGlyph() const
{
    int symc = 0;
    switch (this->GetActualDur()) {
        case DUR_LG: symc = SMUFL_E4E1_restLonga; break;
        case DUR_BR: symc = SMUFL_E4E2_restDoubleWhole; break;
        case DUR_1: symc = SMUFL_E4E3_restWhole; break;
        case DUR_2: symc = SMUFL_E4E4_restHalf; break;
        case DUR_4: symc = SMUFL_E4E5_restQuarter; break;
        case DUR_8: symc = SMUFL_E4E6_rest8th; break;
        case DUR_16: symc = SMUFL_E4E7_rest16th; break;
        case DUR_32: symc = SMUFL_E4E8_rest32nd; break;
        case DUR_64: symc = SMUFL_E4E9_rest64th; break;
        case DUR_128: symc = SMUFL_E4EA_rest128th; break;
        case DUR_256: symc = SMUFL_E4EB_rest256th; break;
        case DUR_512: symc = SMUFL_E4EC_rest512th; break;
        case DUR_1024: symc = SMUFL_E4ED_rest1024th; break;
    }
    return symc;
}

int Rest::GetRestLocOffset(int loc)
{
    switch (this->GetActualDur()) {
        case DUR_1: loc += 2; break;
        default: loc += 0; break;
    }

    return loc;
}

void Rest::UpdateFromTransLoc(const TransPitch &tp)
{
    if (HasOloc() && HasPloc()) {
        SetPloc(tp.GetPitchName());

        if (GetOloc() != tp.m_oct) {
            SetOloc(tp.m_oct);
        }
    }
}

int Rest::GetOptimalLayerLocation(Staff *staff, Layer *layer, int defaultLocation)
{
    Layer *parentLayer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    if (!layer) return defaultLocation;
    const int layerCount = parentLayer->GetLayerCountForTimeSpanOf(this);
    // handle rest positioning for 2 layers. 3 layers and more are much more complex to solve
    if (layerCount != 2) return defaultLocation;

    ListOfObjects layers;
    ClassIdComparison matchType(LAYER);
    staff->FindAllDescendantByComparison(&layers, &matchType);
    const bool isTopLayer(vrv_cast<Layer *>(*layers.begin())->GetN() == layer->GetN());

    // find best rest location relative to elements on other layers
    const auto otherLayerRelativeLocationInfo = GetLocationRelativeToOtherLayers(layers, layer);
    int currentLayerRelativeLocation = GetLocationRelativeToCurrentLayer(staff, layer, isTopLayer);
    int otherLayerRelativeLocation = otherLayerRelativeLocationInfo.first
        + GetRestOffsetFromOptions(RL_otherLayer, otherLayerRelativeLocationInfo, isTopLayer);
    if (currentLayerRelativeLocation == VRV_UNSET) {
        currentLayerRelativeLocation = defaultLocation;
    }
    else {
        std::pair<int, RestAccidental> currentLayerRelativeLocationInfo(currentLayerRelativeLocation, RA_none);
        currentLayerRelativeLocation
            += GetRestOffsetFromOptions(RL_sameLayer, currentLayerRelativeLocationInfo, isTopLayer);
    }

    return isTopLayer ? std::max({ otherLayerRelativeLocation, currentLayerRelativeLocation, defaultLocation })
                      : std::min({ otherLayerRelativeLocation, currentLayerRelativeLocation, defaultLocation });
}

std::pair<int, RestAccidental> Rest::GetLocationRelativeToOtherLayers(
    const ListOfObjects &layersList, Layer *currentLayer)
{
    if (!currentLayer) return { VRV_UNSET, RA_none };
    const bool isTopLayer(vrv_cast<Layer *>(*layersList.begin())->GetN() == currentLayer->GetN());

    // Get iterator to another layer. We're going to find coliding elements there
    auto layerIter = std::find_if(layersList.begin(), layersList.end(),
        [&](Object *foundLayer) { return vrv_cast<Layer *>(foundLayer)->GetN() != currentLayer->GetN(); });
    if (layerIter == layersList.end()) return { VRV_UNSET, RA_none };
    auto collidingElementsList = vrv_cast<Layer *>(*layerIter)->GetLayerElementsForTimeSpanOf(this);

    std::pair<int, RestAccidental> finalElementInfo = { VRV_UNSET, RA_none };
    // Go through each colliding element and figure out optimal location for the rest
    for (Object *object : collidingElementsList) {
        auto currentElementInfo = GetElementLocation(object, vrv_cast<Layer *>(*layerIter), isTopLayer);
        if (currentElementInfo.first == VRV_UNSET) continue;
        //  If note on other layer is not on the same x position as rest - ignore its accidental
        if (GetAlignment()->GetTime() != vrv_cast<LayerElement *>(object)->GetAlignment()->GetTime()) {
            currentElementInfo.second = RA_none;
        }
        if ((VRV_UNSET == finalElementInfo.first) || (isTopLayer && (finalElementInfo.first < currentElementInfo.first))
            || (!isTopLayer && (finalElementInfo.first > currentElementInfo.first))) {
            std::swap(finalElementInfo, currentElementInfo);
        }
    }

    return finalElementInfo;
}

int Rest::GetLocationRelativeToCurrentLayer(Staff *currentStaff, Layer *currentLayer, bool isTopLayer)
{
    if (!currentStaff || !currentLayer) return VRV_UNSET;

    Functor getRelativeLayerElement(&Object::GetRelativeLayerElement);
    GetRelativeLayerElementParams getRelativeLayerElementParams(GetIdx(), BACKWARD, false);

    Object *previousElement = NULL;
    Object *nextElement = NULL;
    // Get previous and next elements from the current layer
    if (currentLayer->GetFirstChildNot(REST)) {
        currentLayer->Process(
            &getRelativeLayerElement, &getRelativeLayerElementParams, NULL, NULL, UNLIMITED_DEPTH, BACKWARD);
        previousElement = getRelativeLayerElementParams.m_relativeElement;
        // reset and search in other direction
        getRelativeLayerElementParams.m_relativeElement = NULL;
        getRelativeLayerElementParams.m_searchDirection = FORWARD;
        getRelativeLayerElement.m_returnCode = FUNCTOR_CONTINUE;
        currentLayer->Process(
            &getRelativeLayerElement, &getRelativeLayerElementParams, NULL, NULL, UNLIMITED_DEPTH, FORWARD);
        nextElement = getRelativeLayerElementParams.m_relativeElement;
    }

    // For chords we want to get the closest element to opposite layer, hence we pass negative 'isTopLayer' value
    // That way we'll get bottom chord note for top layer and top chord note for bottom layer
    const int previousElementLoc = previousElement
        ? GetElementLocation(previousElement, currentLayer, !isTopLayer).first
        : GetFirstRelativeElementLocation(currentStaff, currentLayer, true, isTopLayer);
    const int nextElementLoc = nextElement
        ? GetElementLocation(nextElement, currentLayer, !isTopLayer).first
        : GetFirstRelativeElementLocation(currentStaff, currentLayer, false, isTopLayer);

    if (VRV_UNSET == previousElementLoc) return nextElementLoc;
    if (VRV_UNSET == nextElementLoc) return previousElementLoc;

    return isTopLayer ? std::min(previousElementLoc, nextElementLoc) : std::max(previousElementLoc, nextElementLoc);
}

int Rest::GetFirstRelativeElementLocation(Staff *currentStaff, Layer *currentLayer, bool isPrevious, bool isTopLayer)
{
    // current system
    System *system = vrv_cast<System *>(GetFirstAncestor(SYSTEM));
    assert(system);
    // current measure
    Measure *measure = vrv_cast<Measure *>(GetFirstAncestor(MEASURE));
    assert(measure);

    const int index = system->GetChildIndex(measure);
    Object *relativeMeasure = system->GetChild(isPrevious ? index - 1 : index + 1);
    if (!relativeMeasure || !relativeMeasure->Is(MEASURE)) return VRV_UNSET;

    // Find staff with the same N as current staff
    AttNIntegerComparison snc(STAFF, currentStaff->GetN());
    Staff *previousStaff = vrv_cast<Staff *>(relativeMeasure->FindDescendantByComparison(&snc));
    if (!previousStaff) return VRV_UNSET;

    // Compare number of layers in the next/previous staff and if it's the same - find layer with same N
    ListOfObjects layers;
    ClassIdComparison matchType(LAYER);
    previousStaff->FindAllDescendantByComparison(&layers, &matchType);
    auto layerIter = std::find_if(layers.begin(), layers.end(),
        [&](Object *foundLayer) { return vrv_cast<Layer *>(foundLayer)->GetN() == currentLayer->GetN(); });
    if (((int)layers.size() != currentStaff->GetChildCount(LAYER)) || (layerIter == layers.end())) return VRV_UNSET;

    // Get last element if it's previous layer, get first one otherwise
    Functor getRelativeLayerElement(&Object::GetRelativeLayerElement);
    GetRelativeLayerElementParams getRelativeLayerElementParams(GetIdx(), !isPrevious, true);
    (*layerIter)
        ->Process(&getRelativeLayerElement, &getRelativeLayerElementParams, NULL, NULL, UNLIMITED_DEPTH, !isPrevious);

    Object *lastLayerElement = getRelativeLayerElementParams.m_relativeElement;
    if (lastLayerElement && lastLayerElement->Is({ NOTE, CHORD, FTREM })) {
        return GetElementLocation(lastLayerElement, vrv_cast<Layer *>(*layerIter), !isTopLayer).first;
    }

    return VRV_UNSET;
}

std::pair<int, RestAccidental> Rest::GetElementLocation(Object *object, Layer *layer, bool isTopLayer)
{
    if (object->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(object);
        assert(note);
        Accid *accid = note->GetDrawingAccid();
        return { PitchInterface::CalcLoc(note, layer, note),
            (accid && accid->GetAccid() != 0) ? MeiAccidentalToRestAccidental(accid->GetAccid()) : RA_none };
    }
    if (object->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(object);
        assert(chord);
        Note *relevantNote = isTopLayer ? chord->GetTopNote() : chord->GetBottomNote();
        Accid *accid = relevantNote->GetDrawingAccid();
        return { PitchInterface::CalcLoc(chord, layer, relevantNote, isTopLayer),
            (accid && accid->GetAccid() != 0) ? MeiAccidentalToRestAccidental(accid->GetAccid()) : RA_none };
    }
    if (object->Is(FTREM)) {
        std::vector<std::pair<int, RestAccidental> > btremElements;
        for (int i = 0; i < object->GetChildCount(); ++i) {
            btremElements.emplace_back(GetElementLocation(object->GetChild(i), layer, isTopLayer));
        }
        return isTopLayer ? *std::max_element(btremElements.begin(), btremElements.end())
                          : *std::min_element(btremElements.begin(), btremElements.end());
    }
    return { VRV_UNSET, RA_none };
}

int Rest::GetRestOffsetFromOptions(
    RestLayer layer, const std::pair<int, RestAccidental> &location, bool isTopLayer) const
{
    return g_defaultRests.at(layer)
        .at(RL_sameLayer == layer ? location.second : RA_none)
        .at(isTopLayer ? RLP_restOnTopLayer : RLP_restOnBottomLayer)
        .at(0 == location.first % 2 ? RNP_noteOnLine : RNP_noteInSpace)
        .at(GetActualDur());
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Rest::ConvertMarkupAnalytical(FunctorParams *functorParams)
{
    ConvertMarkupAnalyticalParams *params = vrv_params_cast<ConvertMarkupAnalyticalParams *>(functorParams);
    assert(params);

    if (this->HasFermata()) {
        Fermata *fermata = new Fermata();
        fermata->ConvertFromAnalyticalMarkup(this, this->GetUuid(), params);
    }

    return FUNCTOR_CONTINUE;
}

int Rest::PrepareLayerElementParts(FunctorParams *functorParams)
{
    Dots *currentDots = dynamic_cast<Dots *>(this->FindDescendantByType(DOTS, 1));

    if ((this->GetDur() > DUR_BR) && (this->GetDots() > 0)) {
        if (!currentDots) {
            currentDots = new Dots();
            this->AddChild(currentDots);
        }
        currentDots->AttAugmentDots::operator=(*this);
    }
    // This will happen only if the duration has changed
    else if (currentDots) {
        if (this->DeleteChild(currentDots)) {
            currentDots = NULL;
        }
    }

    /************ Prepare the drawing cue size ************/

    Functor prepareDrawingCueSize(&Object::PrepareDrawingCueSize);
    this->Process(&prepareDrawingCueSize, NULL);

    return FUNCTOR_CONTINUE;
}

int Rest::CalcDots(FunctorParams *functorParams)
{
    CalcDotsParams *params = vrv_params_cast<CalcDotsParams *>(functorParams);
    assert(params);

    // We currently have no dots object with mensural rests
    if (this->IsMensuralDur()) {
        return FUNCTOR_SIBLINGS;
    }

    // Nothing to do
    if ((this->GetDur() <= DUR_BR) || (this->GetDots() < 1)) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    if (this->m_crossStaff) staff = this->m_crossStaff;

    bool drawingCueSize = this->GetDrawingCueSize();
    int staffSize = staff->m_drawingStaffSize;

    // For single rests we need here to set the dot loc
    Dots *dots = vrv_cast<Dots *>(this->FindDescendantByType(DOTS, 1));
    assert(dots);

    std::list<int> *dotLocs = dots->GetDotLocsForStaff(staff);
    int loc = this->GetDrawingLoc();

    // if it's on a staff line to start with, we need to compensate here and add a full unit like DrawDots would
    if ((loc % 2) == 0) {
        loc += 1;
    }

    switch (this->GetActualDur()) {
        case DUR_1: loc += 0; break;
        case DUR_2: loc += 0; break;
        case DUR_4: loc += 0; break;
        case DUR_8: loc += 0; break;
        case DUR_16: loc += 0; break;
        case DUR_32: loc += 2; break;
        case DUR_64: loc += 2; break;
        case DUR_128: loc += 4; break;
        case DUR_256: loc += 4; break;
        default: break;
    }

    dotLocs->push_back(loc);

    // HARDCODED
    int xRel = params->m_doc->GetDrawingUnit(staffSize) * 2.5;
    if (drawingCueSize) xRel = params->m_doc->GetCueSize(xRel);
    if (this->GetDur() > DUR_2) {
        xRel = params->m_doc->GetGlyphWidth(this->GetRestGlyph(), staff->m_drawingStaffSize, drawingCueSize);
    }
    dots->SetDrawingXRel(std::max(dots->GetDrawingXRel(), xRel));

    return FUNCTOR_SIBLINGS;
}

int Rest::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);
    PositionInterface::InterfaceResetDrawing(functorParams, this);

    return FUNCTOR_CONTINUE;
}

int Rest::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    return FUNCTOR_CONTINUE;
}

int Rest::Transpose(FunctorParams *functorParams)
{
    TransposeParams *params = vrv_params_cast<TransposeParams *>(functorParams);
    assert(params);

    if ((!HasOloc() || !HasPloc()) && !HasLoc()) return FUNCTOR_SIBLINGS;

    // Find whether current layer is top, middle (either one if multiple) or bottom
    Staff *parentStaff = vrv_cast<Staff *>(GetFirstAncestor(STAFF));
    assert(parentStaff);

    Layer *parentLayer = vrv_cast<Layer *>(GetFirstAncestor(LAYER));
    assert(parentLayer);

    ListOfObjects objects;
    ClassIdComparison matchClassId(LAYER);
    parentStaff->FindAllDescendantByComparison(&objects, &matchClassId);
    const int layerCount = (int)objects.size();

    Layer *firstLayer = vrv_cast<Layer *>(objects.front());
    Layer *lastLayer = vrv_cast<Layer *>(objects.back());

    const bool isTopLayer = (firstLayer->GetN() == parentLayer->GetN());
    const bool isBottomLayer = (lastLayer->GetN() == parentLayer->GetN());

    // transpose based on @oloc and @ploc
    if (HasOloc() && HasPloc()) {
        const TransPitch centralLocation(6, 0, 4); // middle location of the staff
        TransPitch restLoc(GetPloc() - PITCHNAME_c, 0, GetOloc());
        params->m_transposer->Transpose(restLoc);
        const bool isRestOnSpace = static_cast<bool>((restLoc.m_oct * 7 + restLoc.m_pname) % 2);
        // on outer layers move rest on odd locations one line further
        // in middle layers tolerate even locations to not risk collisions
        if (layerCount > 1) {
            if (isTopLayer && isRestOnSpace) {
                restLoc++;
            }
            else if (isBottomLayer && isRestOnSpace) {
                restLoc--;
            }
            if ((isTopLayer && (restLoc < centralLocation)) || (isBottomLayer && (restLoc > centralLocation))) {
                restLoc = centralLocation;
            }
        }

        UpdateFromTransLoc(restLoc);
    }
    // transpose based on @loc
    else if (HasLoc()) {
        constexpr int centralLocation(4);
        int transval = params->m_transposer->GetTranspositionIntervalClass();
        int diatonic;
        int chromatic;
        params->m_transposer->IntervalToDiatonicChromatic(diatonic, chromatic, transval);
        int transposedLoc = GetLoc() + diatonic;
        // on outer layers move rest on odd locations one line further
        // in middle layers tolerate even locations to not risk collisions
        if (layerCount > 1) {
            if (isTopLayer)
                transposedLoc += abs(transposedLoc % 2);
            else if (isBottomLayer)
                transposedLoc -= abs(transposedLoc % 2);
            if ((isTopLayer && (transposedLoc < centralLocation))
                || (isBottomLayer && (transposedLoc > centralLocation))) {
                transposedLoc = centralLocation;
            }
        }
        SetLoc(transposedLoc);
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
