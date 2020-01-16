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

#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "fermata.h"
#include "functorparams.h"
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

void Rest::AddChild(Object *child)
{
    if (child->Is(DOTS)) {
        assert(dynamic_cast<Dots *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);

    // Dots are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is(DOTS))
        m_children.insert(m_children.begin(), child);
    else
        m_children.push_back(child);
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

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Rest::ConvertAnalyticalMarkup(FunctorParams *functorParams)
{
    ConvertAnalyticalMarkupParams *params = dynamic_cast<ConvertAnalyticalMarkupParams *>(functorParams);
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
    if (this->IsMensural()) {
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
        case DUR_1: loc -= 2; break;
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
