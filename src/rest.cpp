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
#include "vrv.h"

namespace vrv {

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

    // Dots are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is(DOTS)) {
        m_children.insert(m_children.begin(), child);
    }
    else {
        m_children.push_back(child);
    }
    Modify();
}

wchar_t Rest::GetRestGlyph() const
{
    int symc = 0;
    switch (this->GetActualDur()) {
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
        case DUR_MX: loc -= 0; break;
        case DUR_LG: loc -= 0; break;
        case DUR_BR: loc += 0; break;
        case DUR_1: loc += 2; break;
        case DUR_2: loc += 0; break;
        case DUR_4: loc -= 2; break;
        case DUR_8: loc -= 2; break;
        case DUR_16: loc -= 2; break;
        case DUR_32: loc -= 2; break;
        case DUR_64: loc -= 2; break;
        case DUR_128: loc -= 2; break;
        case DUR_256: loc -= 2; break;
        default: loc -= 1; break;
    }

    return loc;
}

int Rest::GetOptimalLayerLocation(Staff *staff, Layer *layer, int defaultLocation)
{
    Layer *parentLayer = dynamic_cast<Layer *>(this->GetFirstAncestor(LAYER));
    if (!layer) return defaultLocation;
    const int layerCount = parentLayer->GetLayerCountForTimeSpanOf(this);
    // handle rest positioning for 2 layers. 3 layers and more are much more complex to solve
    if (layerCount != 2) return defaultLocation;

    ListOfObjects layers;
    ClassIdComparison matchType(LAYER);
    staff->FindAllDescendantByComparison(&layers, &matchType);
    const bool isTopLayer(dynamic_cast<Layer *>(*layers.begin())->GetN() == layer->GetN());

    // find best rest location relative to elements on other layers
    const auto otherLayerRelativeLocationInfo = GetLocationRelativeToOtherLayers(layers, layer);
    int currentLayerRelativeLocation = GetLocationRelativeToCurrentLayer(staff, layer, isTopLayer);
    int otherLayerRelativeLocation = otherLayerRelativeLocationInfo.first
        + GetRestOffsetFromOptions("otherLayer", otherLayerRelativeLocationInfo, isTopLayer);
    if (currentLayerRelativeLocation == VRV_UNSET) {
        currentLayerRelativeLocation = defaultLocation;
    }
    else {
        std::pair<int, std::string> currentLayerRelativeLocationInfo(currentLayerRelativeLocation, "noAccidental");
        currentLayerRelativeLocation
            += GetRestOffsetFromOptions("sameLayer", currentLayerRelativeLocationInfo, isTopLayer);
    }

    return isTopLayer ? std::max({ otherLayerRelativeLocation, currentLayerRelativeLocation, defaultLocation })
                      : std::min({ otherLayerRelativeLocation, currentLayerRelativeLocation, defaultLocation });
}

std::pair<int, std::string> Rest::GetLocationRelativeToOtherLayers(const ListOfObjects &layersList, Layer *currentLayer)
{
    if (!currentLayer) return { VRV_UNSET, "noAccidental" };
    const bool isTopLayer(dynamic_cast<Layer *>(*layersList.begin())->GetN() == currentLayer->GetN());

    // Get iterator to another layer. We're going to find coliding elements there
    auto layerIter = std::find_if(layersList.begin(), layersList.end(),
        [&](Object *foundLayer) { return dynamic_cast<Layer *>(foundLayer)->GetN() != currentLayer->GetN(); });
    if (layerIter == layersList.end()) return { VRV_UNSET, "noAccidental" };
    auto collidingElementsList = dynamic_cast<Layer *>(*layerIter)->GetLayerElementsForTimeSpanOf(this);
    
    std::pair<int, std::string> finalElementInfo = { VRV_UNSET, "noAccidental" };
    // Go through each colliding element and figure out optimal location for the rest
    for (Object *object : collidingElementsList) {
        auto currentElementInfo = GetElementLocation(object, dynamic_cast<Layer *>(*layerIter), isTopLayer);
        if (currentElementInfo.first == VRV_UNSET) continue;
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

    // Get previous and next elements from the current layer
    currentLayer->Process(
        &getRelativeLayerElement, &getRelativeLayerElementParams, NULL, NULL, UNLIMITED_DEPTH, BACKWARD);
    Object *previousElement
        = getRelativeLayerElementParams.m_relativeElement;
    // reset and search in other direction
    getRelativeLayerElementParams.m_relativeElement = NULL;
    getRelativeLayerElementParams.m_searchDirection = FORWARD;
    getRelativeLayerElement.m_returnCode = FUNCTOR_CONTINUE;
    currentLayer->Process(
        &getRelativeLayerElement, &getRelativeLayerElementParams, NULL, NULL, UNLIMITED_DEPTH, FORWARD);
    Object *nextElement = getRelativeLayerElementParams.m_relativeElement;

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
    // current doc
    Doc *doc = dynamic_cast<Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);
    // current measure
    Measure *measure = dynamic_cast<Measure *>(this->GetFirstAncestor(MEASURE));
    assert(measure);

    ClassIdComparison ac(MEASURE);
    Measure *relativeMeasure = dynamic_cast<Measure *>(
        isPrevious ? doc->FindPreviousChild(&ac, measure) : doc->FindNextChild(&ac, measure));

    if (!relativeMeasure) return VRV_UNSET;

    // Find staff with the same N as current staff
    AttNIntegerComparison snc(STAFF, currentStaff->GetN());
    Staff *previousStaff = dynamic_cast<Staff *>(relativeMeasure->FindDescendantByComparison(&snc));
    if (!previousStaff) return VRV_UNSET;

    // Compare number of layers in the next/previous staff and if it's the same - find layer with same N
    ListOfObjects layers;
    ClassIdComparison matchType(LAYER);
    previousStaff->FindAllDescendantByComparison(&layers, &matchType);
    auto layerIter = std::find_if(layers.begin(), layers.end(), [&](Object *foundLayer) {
        return dynamic_cast<Layer *>(foundLayer)->GetN() == currentLayer->GetN();
    });
    if (((int)layers.size() != currentStaff->GetChildCount(LAYER)) || (layerIter == layers.end())) return VRV_UNSET;

    // Get last element if it's previous layer, get first one otherwise
    Functor getRelativeLayerElement(&Object::GetRelativeLayerElement);
    GetRelativeLayerElementParams getRelativeLayerElementParams(GetIdx(), !isPrevious, true);
    (*layerIter)->Process(&getRelativeLayerElement, &getRelativeLayerElementParams, NULL, NULL, UNLIMITED_DEPTH, !isPrevious);

    Object *lastLayerElement = getRelativeLayerElementParams.m_relativeElement;
    if (lastLayerElement && lastLayerElement->Is({ NOTE, CHORD, FTREM })) {
        return GetElementLocation(lastLayerElement, dynamic_cast<Layer *>(*layerIter), !isTopLayer).first;
    }

    return VRV_UNSET;
}

std::pair<int, std::string> Rest::GetElementLocation(Object *object, Layer *layer, bool isTopLayer)
{
    AttConverter converter;
    if (object->Is(NOTE)) {
        Note *note = dynamic_cast<Note *>(object);
        assert(note);
        Accid* accid = note->GetDrawingAccid();
        return { PitchInterface::CalcLoc(note, dynamic_cast<Layer *>(layer), note),
            (accid && accid->GetAccid() != 0) ? converter.AccidentalWrittenToStr(accid->GetAccid()) : "noAccidental" };
    }
    if (object->Is(CHORD)) {
        Chord* chord = dynamic_cast<Chord *>(object);
        assert(chord);
        Note* relevantNote = isTopLayer ? chord->GetTopNote() : chord->GetBottomNote();
        Accid* accid = relevantNote->GetDrawingAccid();
        return { PitchInterface::CalcLoc(chord, dynamic_cast<Layer *>(layer), relevantNote, isTopLayer),
            (accid && accid->GetAccid() != 0) ? converter.AccidentalWrittenToStr(accid->GetAccid())
                                              : "noAccidental" };
    }
    if (object->Is(FTREM)) {
        std::vector<std::pair<int, std::string> > btremElements;
        for (int i = 0; i < object->GetChildCount(); ++i) {
            btremElements.emplace_back(GetElementLocation(object->GetChild(i), layer, isTopLayer));
        }
        return isTopLayer ? *std::max_element(btremElements.begin(), btremElements.end())
                          : *std::min_element(btremElements.begin(), btremElements.end());
    }
    return { VRV_UNSET, "noAccidental" };
}

int Rest::GetRestOffsetFromOptions(
    const std::string &layer, const std::pair<int, std::string>& location, bool isTopLayer) const
{
    Doc *doc = dynamic_cast<Doc *>(this->GetFirstAncestor(DOC));
    assert(doc);

    std::vector<std::string> jsonNodePath{ layer };
    if (layer == "otherLayer") jsonNodePath.emplace_back(location.second);
    jsonNodePath.insert(jsonNodePath.end(),
        { isTopLayer ? "restOnTopLayer" : "restOnBottomLayer", 0 == location.first % 2 ? "noteOnLine" : "noteInSpace",
            this->AttDurationLogical::DurationToStr(data_DURATION(GetActualDur())) });
    return doc->GetOptions()->m_restLayerOffsets.GetIntValue(jsonNodePath);
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Rest::ConvertMarkupAnalytical(FunctorParams *functorParams)
{
    ConvertMarkupAnalyticalParams *params = dynamic_cast<ConvertMarkupAnalyticalParams *>(functorParams);
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
    CalcDotsParams *params = dynamic_cast<CalcDotsParams *>(functorParams);
    assert(params);

    // We currently have no dots object with mensural rests
    if (this->IsMensuralDur()) {
        return FUNCTOR_SIBLINGS;
    }

    // Nothing to do
    if ((this->GetDur() <= DUR_BR) || (this->GetDots() < 1)) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    if (this->m_crossStaff) staff = this->m_crossStaff;

    bool drawingCueSize = this->GetDrawingCueSize();
    int staffSize = staff->m_drawingStaffSize;

    // For single rests we need here to set the dot loc
    Dots *dots = dynamic_cast<Dots *>(this->FindDescendantByType(DOTS, 1));
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
        case DUR_4: loc += 2; break;
        case DUR_8: loc += 2; break;
        case DUR_16: loc += 2; break;
        case DUR_32: loc += 4; break;
        case DUR_64: loc += 4; break;
        case DUR_128: loc += 6; break;
        case DUR_256: loc += 6; break;
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

} // namespace vrv
