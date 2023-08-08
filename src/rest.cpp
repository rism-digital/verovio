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
#include "findlayerelementsfunctor.h"
#include "layer.h"
#include "smufl.h"
#include "staff.h"
#include "system.h"
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
            { RA_n,
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
    , AttExtSymAuth()
    , AttExtSymNames()
    , AttRestVisMensural()
{
    this->RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_CUE);
    this->RegisterAttClass(ATT_EXTSYMAUTH);
    this->RegisterAttClass(ATT_EXTSYMNAMES);
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
    this->ResetExtSymAuth();
    this->ResetExtSymNames();
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

bool Rest::DetermineRestPosition(const Staff *staff, const Layer *layer, bool &isTopLayer) const
{
    ListOfConstObjects elements = layer->GetLayerElementsForTimeSpanOf(this, true);
    if (elements.empty()) return false;

    const LayerElement *firstElement = NULL;
    std::set<int> layers;
    for (const Object *element : elements) {
        const LayerElement *layerElement = vrv_cast<const LayerElement *>(element);
        layers.insert(layerElement->GetAlignmentLayerN());
        if (!firstElement) firstElement = layerElement;
    }

    // handle rest positioning for 2 layers. 3 layers and more are much more complex to solve
    if (layers.size() == 1) {
        if (m_crossStaff) {
            isTopLayer = staff->GetN() < m_crossStaff->GetN();
        }
        else if (layer->GetN() < (*layers.begin())) {
            isTopLayer = true;
        }
        else {
            if (*layers.begin() < 0) {
                isTopLayer = staff->GetN() < firstElement->GetAncestorStaff()->GetN();
            }
            else {
                isTopLayer = false;
            }
        }
        return true;
    }
    return false;
}

int Rest::GetOptimalLayerLocation(const Staff *staff, const Layer *layer, int defaultLocation) const
{
    if (!layer || this->HasSameasLink()) return defaultLocation;
    bool isTopLayer = false;
    if (!this->DetermineRestPosition(staff, layer, isTopLayer)) return defaultLocation;

    // find best rest location relative to elements on other layers
    bool restOverlap = true;
    const auto otherLayerRelativeLocationInfo = this->GetLocationRelativeToOtherLayers(layer, isTopLayer, restOverlap);
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
            otherLayerRelativeLocation -= 2;
        }
    }

    const int marginLocation = this->GetMarginLayerLocation(isTopLayer, restOverlap);
    const int optimalLocation = isTopLayer
        ? std::max({ otherLayerRelativeLocation, currentLayerRelativeLocation, defaultLocation, marginLocation })
        : std::min({ otherLayerRelativeLocation, currentLayerRelativeLocation, defaultLocation, marginLocation });

    return optimalLocation;
}

std::pair<int, RestAccidental> Rest::GetLocationRelativeToOtherLayers(
    const Layer *currentLayer, bool isTopLayer, bool &restOverlap) const
{
    if (!currentLayer) return { VRV_UNSET, RA_none };

    auto collidingElementsList = currentLayer->GetLayerElementsForTimeSpanOf(this, true);
    if (collidingElementsList.empty()) return { VRV_UNSET, RA_none };

    std::pair<int, RestAccidental> finalElementInfo = { VRV_UNSET, RA_none };
    // Go through each colliding element and figure out optimal location for the rest
    for (const Object *object : collidingElementsList) {
        const LayerElement *layerElement = vrv_cast<const LayerElement *>(object);
        const Layer *objectLayer = layerElement->m_crossLayer
            ? layerElement->m_crossLayer
            : vrv_cast<const Layer *>(object->GetFirstAncestor(LAYER));
        if (object->Is(NOTE)) restOverlap = false;
        auto currentElementInfo = this->GetElementLocation(object, objectLayer, isTopLayer);
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

    const Object *previousElement = NULL;
    const Object *nextElement = NULL;
    // Get previous and next elements from the current layer
    if (currentLayer->GetFirstChildNot(REST)) {
        GetRelativeLayerElementFunctor getRelativeLayerElementBackwards(this->GetIdx(), false);
        getRelativeLayerElementBackwards.SetDirection(BACKWARD);
        currentLayer->Process(getRelativeLayerElementBackwards);
        previousElement = getRelativeLayerElementBackwards.GetRelativeElement();

        // search in other direction
        GetRelativeLayerElementFunctor getRelativeLayerElementForwards(this->GetIdx(), false);
        currentLayer->Process(getRelativeLayerElementForwards);
        nextElement = getRelativeLayerElementForwards.GetRelativeElement();
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
    GetRelativeLayerElementFunctor getRelativeLayerElement(this->GetIdx(), true);
    getRelativeLayerElement.SetDirection(!isPrevious);
    (*layerIter)->Process(getRelativeLayerElement);

    const Object *lastLayerElement = getRelativeLayerElement.GetRelativeElement();
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

FunctorCode Rest::Accept(Functor &functor)
{
    return functor.VisitRest(this);
}

FunctorCode Rest::Accept(ConstFunctor &functor) const
{
    return functor.VisitRest(this);
}

FunctorCode Rest::AcceptEnd(Functor &functor)
{
    return functor.VisitRestEnd(this);
}

FunctorCode Rest::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitRestEnd(this);
}

} // namespace vrv
