/////////////////////////////////////////////////////////////////////////////
// Name:        rest.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "rest.h"

//----------------------------------------------------------------------------

#include <cassert>

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
#include "timemap.h"
#include "transposition.h"
#include "vrv.h"

namespace vrv {

typedef std::map<RestLayer,
    std::map<RestAccidental, std::map<RestLayerPlace, std::map<RestNotePlace, std::map<int, int>>>>>
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
                        { { DUR_1, -1 }, { DUR_2, 1 }, { DUR_4, 1 }, { DUR_8, 1 }, { DUR_16, 3 }, { DUR_32, 3 },
                            { DUR_64, 5 }, { DUR_128, 5 }, { DUR_LG, 3 }, { DUR_BR, 1 } } },
                      { RNP_noteOnLine,
                          { { DUR_1, 0 }, { DUR_2, 0 }, { DUR_4, 2 }, { DUR_8, 2 }, { DUR_16, 2 }, { DUR_32, 2 },
                              { DUR_64, 4 }, { DUR_128, 4 }, { DUR_LG, 2 }, { DUR_BR, 2 } } } } },
                { RLP_restOnBottomLayer,
                    { { RNP_noteInSpace,
                          { { DUR_1, -3 }, { DUR_2, -1 }, { DUR_4, -1 }, { DUR_8, -1 }, { DUR_16, -1 }, { DUR_32, -3 },
                              { DUR_64, -3 }, { DUR_128, -5 }, { DUR_LG, -3 }, { DUR_BR, -3 } } },
                        { RNP_noteOnLine,
                            { { DUR_1, -2 }, { DUR_2, -2 }, { DUR_4, -2 }, { DUR_8, -2 }, { DUR_16, -2 },
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

static const ClassRegistrar<Rest> s_factory("rest", REST);

Rest::Rest()
    : LayerElement(REST, "rest-")
    , DurationInterface()
    , PositionInterface()
    , AttColor()
    , AttCue()
    , AttExtSym()
    , AttRestVisMensural()
{
    this->RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_CUE);
    this->RegisterAttClass(ATT_EXTSYM);
    this->RegisterAttClass(ATT_RESTVISMENSURAL);
    this->Reset();
}

Rest::~Rest() {}

void Rest::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
    PositionInterface::Reset();
    this->ResetColor();
    this->ResetCue();
    this->ResetExtSym();
    this->ResetRestVisMensural();
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

    ArrayOfObjects &children = this->GetChildrenForModification();

    // Dots are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is(DOTS)) {
        children.insert(children.begin(), child);
    }
    else {
        children.push_back(child);
    }
    Modify();
}

char32_t Rest::GetRestGlyph() const
{
    return this->GetRestGlyph(this->GetActualDur());
}

char32_t Rest::GetRestGlyph(const int duration) const
{
    const Resources *resources = this->GetDocResources();
    if (!resources) return 0;

    // If there is glyph.num, prioritize it
    if (this->HasGlyphNum()) {
        char32_t code = this->GetGlyphNum();
        if (NULL != resources->GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (this->HasGlyphName()) {
        char32_t code = resources->GetGlyphCode(this->GetGlyphName());
        if (NULL != resources->GetGlyph(code)) return code;
    }

    if (this->IsMensuralDur()) {
        switch (duration) {
            case DUR_MX: return SMUFL_E9F0_mensuralRestMaxima; break;
            case DUR_LG: return SMUFL_E9F2_mensuralRestLongaImperfecta; break;
            case DUR_BR: return SMUFL_E9F3_mensuralRestBrevis; break;
            case DUR_1: return SMUFL_E9F4_mensuralRestSemibrevis; break;
            case DUR_2: return SMUFL_E9F5_mensuralRestMinima; break;
            case DUR_4: return SMUFL_E9F6_mensuralRestSemiminima; break;
            case DUR_8: return SMUFL_E9F7_mensuralRestFusa; break;
            case DUR_16: return SMUFL_E9F8_mensuralRestSemifusa; break;
        }
    }
    else {
        switch (duration) {
            case DUR_LG: return SMUFL_E4E1_restLonga; break;
            case DUR_BR: return SMUFL_E4E2_restDoubleWhole; break;
            case DUR_1: return SMUFL_E4E3_restWhole; break;
            case DUR_2: return SMUFL_E4E4_restHalf; break;
            case DUR_4: return SMUFL_E4E5_restQuarter; break;
            case DUR_8: return SMUFL_E4E6_rest8th; break;
            case DUR_16: return SMUFL_E4E7_rest16th; break;
            case DUR_32: return SMUFL_E4E8_rest32nd; break;
            case DUR_64: return SMUFL_E4E9_rest64th; break;
            case DUR_128: return SMUFL_E4EA_rest128th; break;
            case DUR_256: return SMUFL_E4EB_rest256th; break;
            case DUR_512: return SMUFL_E4EC_rest512th; break;
            case DUR_1024: return SMUFL_E4ED_rest1024th; break;
        }
    }

    return 0;
}

void Rest::UpdateFromTransLoc(const TransPitch &tp)
{
    if (this->HasOloc() && this->HasPloc()) {
        this->SetPloc(tp.GetPitchName());

        if (this->GetOloc() != tp.m_oct) {
            this->SetOloc(tp.m_oct);
        }
    }
}

int Rest::GetOptimalLayerLocation(const Staff *staff, const Layer *layer, int defaultLocation) const
{
    const Layer *parentLayer = vrv_cast<const Layer *>(this->GetFirstAncestor(LAYER));
    if (!layer) return defaultLocation;
    const std::set<int> layersN = parentLayer->GetLayersNForTimeSpanOf(this);
    // handle rest positioning for 2 layers. 3 layers and more are much more complex to solve
    if (layersN.size() != 2) return defaultLocation;

    const bool isTopLayer
        = m_crossStaff ? (staff->GetN() < m_crossStaff->GetN()) : (layer->GetN() == *layersN.cbegin());

    // find best rest location relative to elements on other layers
    const Staff *realStaff = m_crossStaff ? m_crossStaff : staff;
    ListOfConstObjects layers = realStaff->FindAllDescendantsByType(LAYER, false);
    bool restOverlap = true;
    const auto otherLayerRelativeLocationInfo
        = this->GetLocationRelativeToOtherLayers(layers, layer, isTopLayer, restOverlap);
    int currentLayerRelativeLocation = this->GetLocationRelativeToCurrentLayer(staff, layer, isTopLayer);
    int otherLayerRelativeLocation = otherLayerRelativeLocationInfo.first
        + this->GetRestOffsetFromOptions(RL_otherLayer, otherLayerRelativeLocationInfo, isTopLayer);
    if (currentLayerRelativeLocation == VRV_UNSET) {
        currentLayerRelativeLocation = defaultLocation;
    }
    else {
        std::pair<int, RestAccidental> currentLayerRelativeLocationInfo(currentLayerRelativeLocation, RA_none);
        currentLayerRelativeLocation
            += this->GetRestOffsetFromOptions(RL_sameLayer, currentLayerRelativeLocationInfo, isTopLayer);
    }
    if (m_crossStaff) {
        if (isTopLayer) {
            otherLayerRelativeLocation += defaultLocation + 2;
        }
        else {
            otherLayerRelativeLocation -= defaultLocation + 2;
        }
    }

    const int marginLocation = this->GetMarginLayerLocation(isTopLayer, restOverlap);
    const int optimalLocation = isTopLayer
        ? std::max({ otherLayerRelativeLocation, currentLayerRelativeLocation, defaultLocation, marginLocation })
        : std::min({ otherLayerRelativeLocation, currentLayerRelativeLocation, defaultLocation, marginLocation });

    return optimalLocation;
}

std::pair<int, RestAccidental> Rest::GetLocationRelativeToOtherLayers(
    const ListOfConstObjects &layersList, const Layer *currentLayer, bool isTopLayer, bool &restOverlap) const
{
    if (!currentLayer) return { VRV_UNSET, RA_none };

    // Get iterator to another layer. We're going to find coliding elements there
    auto layerIter = std::find_if(layersList.begin(), layersList.end(),
        [&](const Object *foundLayer) { return vrv_cast<const Layer *>(foundLayer)->GetN() != currentLayer->GetN(); });
    if (layerIter == layersList.end()) {
        if (!m_crossStaff) return { VRV_UNSET, RA_none };
        // if we're dealing with cross-staff item, get first/last layer, depending whether rest is on top or bottom
        layerIter = isTopLayer ? layersList.begin() : std::prev(layersList.end());
    }
    auto collidingElementsList = vrv_cast<const Layer *>(*layerIter)->GetLayerElementsForTimeSpanOf(this);

    std::pair<int, RestAccidental> finalElementInfo = { VRV_UNSET, RA_none };
    // Go through each colliding element and figure out optimal location for the rest
    for (const Object *object : collidingElementsList) {
        if (object->Is(NOTE)) restOverlap = false;
        auto currentElementInfo = this->GetElementLocation(object, vrv_cast<const Layer *>(*layerIter), isTopLayer);
        if (currentElementInfo.first == VRV_UNSET) continue;
        //  If note on other layer is not on the same x position as rest - ignore its accidental
        if (this->GetAlignment()->GetTime() != vrv_cast<const LayerElement *>(object)->GetAlignment()->GetTime()) {
            currentElementInfo.second = RA_none;
            // limit how much rest can be offset when there is duration overlap, but no x position overlap
            if ((isTopLayer && (currentElementInfo.first > 12)) || (!isTopLayer && (currentElementInfo.first < -4))) {
                if (finalElementInfo.first != VRV_UNSET) continue;
                currentElementInfo.first = isTopLayer ? 12 : -4;
            }
        }
        if ((VRV_UNSET == finalElementInfo.first) || (isTopLayer && (finalElementInfo.first < currentElementInfo.first))
            || (!isTopLayer && (finalElementInfo.first > currentElementInfo.first))) {
            std::swap(finalElementInfo, currentElementInfo);
        }
    }

    return finalElementInfo;
}

int Rest::GetLocationRelativeToCurrentLayer(const Staff *currentStaff, const Layer *currentLayer, bool isTopLayer) const
{
    if (!currentStaff || !currentLayer) return VRV_UNSET;

    Functor getRelativeLayerElement(&Object::GetRelativeLayerElement);
    GetRelativeLayerElementParams getRelativeLayerElementParams(this->GetIdx(), BACKWARD, false);

    const Object *previousElement = NULL;
    const Object *nextElement = NULL;
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
        ? this->GetElementLocation(previousElement, currentLayer, !isTopLayer).first
        : this->GetFirstRelativeElementLocation(currentStaff, currentLayer, true, isTopLayer);
    const int nextElementLoc = nextElement
        ? this->GetElementLocation(nextElement, currentLayer, !isTopLayer).first
        : this->GetFirstRelativeElementLocation(currentStaff, currentLayer, false, isTopLayer);

    // Calculate optimal location depending on existence of values for previous and next element location
    int currentOptimalLocation = 0;
    if (VRV_UNSET == previousElementLoc) {
        if (VRV_UNSET == nextElementLoc) {
            return VRV_UNSET;
        }
        else {
            currentOptimalLocation = nextElementLoc;
        }
    }
    else {
        if (VRV_UNSET == nextElementLoc) {
            currentOptimalLocation = previousElementLoc;
        }
        else {
            currentOptimalLocation = (previousElementLoc + nextElementLoc) / 2;
        }
    }

    const int marginLocation = isTopLayer ? 10 : -2;
    currentOptimalLocation = isTopLayer ? std::min(currentOptimalLocation, marginLocation)
                                        : std::max(currentOptimalLocation, marginLocation);

    return currentOptimalLocation;
}

int Rest::GetFirstRelativeElementLocation(
    const Staff *currentStaff, const Layer *currentLayer, bool isPrevious, bool isTopLayer) const
{
    // current system
    const System *system = vrv_cast<const System *>(this->GetFirstAncestor(SYSTEM));
    assert(system);
    // current measure
    const Measure *measure = vrv_cast<const Measure *>(this->GetFirstAncestor(MEASURE));
    assert(measure);

    const int index = system->GetChildIndex(measure);
    const Object *relativeMeasure = system->GetChild(isPrevious ? index - 1 : index + 1);
    if (!relativeMeasure || !relativeMeasure->Is(MEASURE)) return VRV_UNSET;

    // Find staff with the same N as current staff
    AttNIntegerComparison snc(STAFF, currentStaff->GetN());
    const Staff *previousStaff = vrv_cast<const Staff *>(relativeMeasure->FindDescendantByComparison(&snc));
    if (!previousStaff) return VRV_UNSET;

    // Compare number of layers in the next/previous staff and if it's the same - find layer with same N
    ListOfConstObjects layers = previousStaff->FindAllDescendantsByType(LAYER, false);
    auto layerIter = std::find_if(layers.begin(), layers.end(),
        [&](const Object *foundLayer) { return vrv_cast<const Layer *>(foundLayer)->GetN() == currentLayer->GetN(); });
    if (((int)layers.size() != currentStaff->GetChildCount(LAYER)) || (layerIter == layers.end())) return VRV_UNSET;

    // Get last element if it's previous layer, get first one otherwise
    Functor getRelativeLayerElement(&Object::GetRelativeLayerElement);
    GetRelativeLayerElementParams getRelativeLayerElementParams(this->GetIdx(), !isPrevious, true);
    (*layerIter)
        ->Process(&getRelativeLayerElement, &getRelativeLayerElementParams, NULL, NULL, UNLIMITED_DEPTH, !isPrevious);

    const Object *lastLayerElement = getRelativeLayerElementParams.m_relativeElement;
    if (lastLayerElement && lastLayerElement->Is({ NOTE, CHORD, FTREM })) {
        return this->GetElementLocation(lastLayerElement, vrv_cast<const Layer *>(*layerIter), !isTopLayer).first;
    }

    return VRV_UNSET;
}

std::pair<int, RestAccidental> Rest::GetElementLocation(const Object *object, const Layer *layer, bool isTopLayer) const
{
    if (object->Is(NOTE)) {
        const Note *note = vrv_cast<const Note *>(object);
        assert(note);
        const Accid *accid = note->GetDrawingAccid();
        return { PitchInterface::CalcLoc(note, layer, note),
            (accid && accid->GetAccid() != 0) ? MeiAccidentalToRestAccidental(accid->GetAccid()) : RA_none };
    }
    if (object->Is(CHORD)) {
        const Chord *chord = vrv_cast<const Chord *>(object);
        assert(chord);
        const Note *relevantNote = isTopLayer ? chord->GetTopNote() : chord->GetBottomNote();
        const Accid *accid = relevantNote->GetDrawingAccid();
        return { PitchInterface::CalcLoc(chord, layer, relevantNote, isTopLayer),
            (accid && accid->GetAccid() != 0) ? MeiAccidentalToRestAccidental(accid->GetAccid()) : RA_none };
    }
    if (object->Is(FTREM)) {
        std::vector<std::pair<int, RestAccidental>> btremElements;
        for (int i = 0; i < object->GetChildCount(); ++i) {
            btremElements.emplace_back(this->GetElementLocation(object->GetChild(i), layer, isTopLayer));
        }
        return isTopLayer ? *std::max_element(btremElements.begin(), btremElements.end())
                          : *std::min_element(btremElements.begin(), btremElements.end());
    }
    if (object->Is(REST)) {
        if (!m_crossStaff) return { VRV_UNSET, RA_none };
        const Rest *rest = vrv_cast<const Rest *>(object);
        assert(rest);
        return { rest->GetDrawingLoc(), RA_none };
    }
    return { VRV_UNSET, RA_none };
}

int Rest::GetMarginLayerLocation(bool isTopLayer, bool restOverlap) const
{
    int marginLocation = isTopLayer ? 6 : 2;
    if ((this->GetDur() == DURATION_long) || ((this->GetDur() == DURATION_4) && restOverlap)) {
        marginLocation = isTopLayer ? 8 : 0;
    }
    else if (this->GetDur() >= DURATION_8) {
        marginLocation
            = isTopLayer ? (6 + (this->GetDur() - DURATION_4) / 2 * 2) : (2 - (this->GetDur() - DURATION_8) / 2 * 2);
    }
    if (this->GetDur() >= DURATION_1024) {
        marginLocation -= 2;
    }

    return marginLocation;
}

int Rest::GetRestOffsetFromOptions(
    RestLayer layer, const std::pair<int, RestAccidental> &location, bool isTopLayer) const
{
    int duration = this->GetActualDur();
    if (duration > DURATION_128) duration = DURATION_128;
    return g_defaultRests.at(layer)
        .at(RL_sameLayer == layer ? location.second : RA_none)
        .at(isTopLayer ? RLP_restOnTopLayer : RLP_restOnBottomLayer)
        .at(0 == location.first % 2 ? RNP_noteOnLine : RNP_noteInSpace)
        .at(duration);
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Rest::AdjustBeams(FunctorParams *functorParams)
{
    AdjustBeamParams *params = vrv_params_cast<AdjustBeamParams *>(functorParams);
    assert(params);

    if (!params->m_beam) return FUNCTOR_SIBLINGS;

    // Calculate possible overlap for the rest with beams
    int leftMargin = 0, rightMargin = 0;
    const int beams = vrv_cast<Beam *>(params->m_beam)->GetBeamPartDuration(this, false) - DUR_4;
    const int beamWidth = vrv_cast<Beam *>(params->m_beam)->m_beamWidth;
    if (params->m_directionBias > 0) {
        leftMargin = params->m_y1 - beams * beamWidth - this->GetSelfTop();
        rightMargin = params->m_y2 - beams * beamWidth - this->GetSelfTop();
    }
    else {
        leftMargin = this->GetSelfBottom() - params->m_y1 - beams * beamWidth;
        rightMargin = this->GetSelfBottom() - params->m_y2 - beams * beamWidth;
    }

    // Adjust drawing location for the rest based on the overlap with beams.
    // Adjustment should be an even number, so that the rest is positioned properly
    const int overlapMargin = std::min(leftMargin, rightMargin);
    if (overlapMargin >= 0) return FUNCTOR_CONTINUE;

    Staff *staff = this->GetAncestorStaff();

    if ((!this->HasOloc() || !this->HasPloc()) && !this->HasLoc()) {
        // constants
        const int unit = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
        // calculate new and old locations for the rest
        const int locAdjust = (params->m_directionBias * (overlapMargin - 2 * unit + 1) / unit);
        const int oldLoc = this->GetDrawingLoc();
        const int newLoc = oldLoc + locAdjust - locAdjust % 2;
        if (staff->GetChildCount(LAYER) == 1) {
            this->SetDrawingLoc(newLoc);
            this->SetDrawingYRel(staff->CalcPitchPosYRel(params->m_doc, newLoc));
            // If there are dots, adjust their location as well
            if (this->GetDots() > 0) {
                Dots *dots = vrv_cast<Dots *>(this->FindDescendantByType(DOTS, 1));
                if (dots) {
                    std::set<int> &dotLocs = dots->ModifyDotLocsForStaff(staff);
                    const int dotLoc = (oldLoc % 2) ? oldLoc : oldLoc + 1;
                    if (std::find(dotLocs.cbegin(), dotLocs.cend(), dotLoc) != dotLocs.cend()) {
                        dotLocs.erase(dotLoc);
                        dotLocs.insert(newLoc);
                    }
                }
            }
            return FUNCTOR_CONTINUE;
        }
    }

    const int unit = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize);
    const int unitChangeNumber = ((std::abs(overlapMargin) + unit / 6) / unit);
    if (unitChangeNumber > 0) {
        const int adjust = unitChangeNumber * unit * params->m_directionBias;
        if (std::abs(adjust) > std::abs(params->m_overlapMargin)) params->m_overlapMargin = adjust;
    }

    return FUNCTOR_CONTINUE;
}

int Rest::ConvertMarkupAnalytical(FunctorParams *functorParams)
{
    ConvertMarkupAnalyticalParams *params = vrv_params_cast<ConvertMarkupAnalyticalParams *>(functorParams);
    assert(params);

    if (this->HasFermata()) {
        Fermata *fermata = new Fermata();
        fermata->ConvertFromAnalyticalMarkup(this, this->GetID(), params);
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

    Functor prepareCueSize(&Object::PrepareCueSize);
    this->Process(&prepareCueSize, NULL);

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

    Staff *staff = this->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    const bool drawingCueSize = this->GetDrawingCueSize();
    const int staffSize = staff->m_drawingStaffSize;

    // For single rests we need here to set the dot loc
    Dots *dots = vrv_cast<Dots *>(this->FindDescendantByType(DOTS, 1));
    assert(dots);

    std::set<int> &dotLocs = dots->ModifyDotLocsForStaff(staff);
    int loc = this->GetDrawingLoc();

    // if it's on a staff line to start with, we need to compensate here and add a full unit like DrawDots would
    if ((loc % 2) == 0) {
        loc += 1;
    }

    switch (this->GetActualDur()) {
        case DUR_32:
        case DUR_64: loc += 2; break;
        case DUR_128:
        case DUR_256: loc += 4; break;
        case DUR_512: loc += 6; break;
        case DUR_1024: loc += 8; break;
        default: break;
    }

    dotLocs.insert(loc);

    // HARDCODED
    int xRel = params->m_doc->GetDrawingUnit(staffSize) * 2.5;
    if (drawingCueSize) xRel = params->m_doc->GetCueSize(xRel);
    if (this->GetDur() > DUR_2) {
        xRel = params->m_doc->GetGlyphWidth(this->GetRestGlyph(), staff->m_drawingStaffSize, drawingCueSize);
    }
    dots->SetDrawingXRel(std::max(dots->GetDrawingXRel(), xRel));

    return FUNCTOR_SIBLINGS;
}

int Rest::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetData(functorParams);
    PositionInterface::InterfaceResetData(functorParams, this);

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

    if ((!this->HasOloc() || !this->HasPloc()) && !this->HasLoc()) return FUNCTOR_SIBLINGS;

    // Find whether current layer is top, middle (either one if multiple) or bottom
    Staff *parentStaff = this->GetAncestorStaff();
    Layer *parentLayer = vrv_cast<Layer *>(this->GetFirstAncestor(LAYER));
    assert(parentLayer);

    ListOfObjects objects = parentStaff->FindAllDescendantsByType(LAYER, false);
    const int layerCount = (int)objects.size();

    Layer *firstLayer = vrv_cast<Layer *>(objects.front());
    Layer *lastLayer = vrv_cast<Layer *>(objects.back());

    const bool isTopLayer = (firstLayer->GetN() == parentLayer->GetN());
    const bool isBottomLayer = (lastLayer->GetN() == parentLayer->GetN());

    // transpose based on @oloc and @ploc
    if (this->HasOloc() && this->HasPloc()) {
        const TransPitch centralLocation(6, 0, 4); // middle location of the staff
        TransPitch restLoc(this->GetPloc() - PITCHNAME_c, 0, this->GetOloc());
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

        this->UpdateFromTransLoc(restLoc);
    }
    // transpose based on @loc
    else if (this->HasLoc()) {
        constexpr int centralLocation(4);
        int transval = params->m_transposer->GetTranspositionIntervalClass();
        int diatonic;
        int chromatic;
        params->m_transposer->IntervalToDiatonicChromatic(diatonic, chromatic, transval);
        int transposedLoc = this->GetLoc() + diatonic;
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
        this->SetLoc(transposedLoc);
    }

    return FUNCTOR_SIBLINGS;
}

int Rest::GenerateTimemap(FunctorParams *functorParams)
{
    GenerateTimemapParams *params = vrv_params_cast<GenerateTimemapParams *>(functorParams);
    assert(params);

    params->m_timemap->AddEntry(this, params);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
