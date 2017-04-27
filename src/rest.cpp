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
#include "functorparams.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Rest
//----------------------------------------------------------------------------

Rest::Rest()
    : LayerElement("rest-")
    , DurationInterface()
    , PositionInterface()
    , AttColor()
    , AttRelativesize()
    , AttRestVisMensural()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_RELATIVESIZE);
    RegisterAttClass(ATT_RESTVISMENSURAL);
    Reset();
}

Rest::~Rest()
{
}

void Rest::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
    PositionInterface::Reset();
    ResetColor();
    ResetRelativesize();
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
    
int Rest::GetDefaultLoc(bool hasMultipleLayer, bool isFirstLayer)
{
    int loc = 4;
    
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
        case DUR_64: loc -= 3; break;
        case DUR_128: loc -= 3; break;
        case DUR_256: loc -= 4; break;
        default:
            loc -= 1;
            break;
    }
    if (hasMultipleLayer) {
        if (isFirstLayer)
            loc += 2;
        else
            loc -= 2;
    }
    
    return loc;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Rest::PrepareLayerElementParts(FunctorParams *functorParams)
{
    Dots *currentDots = dynamic_cast<Dots *>(this->FindChildByType(DOTS, 1));

    if ((this->GetDur() > DUR_BR) && this->HasDots()) {
        if (!currentDots) {
            currentDots = new Dots();
            this->AddChild(currentDots);
        }
        currentDots->AttAugmentdots::operator=(*this);
    }
    // This will happen only if the duration has changed
    else if (currentDots) {
        if (this->DeleteChild(currentDots)) {
            currentDots = NULL;
        }
    }

    return FUNCTOR_CONTINUE;
};

int Rest::CalcDots(FunctorParams *functorParams)
{
    CalcDotsParams *params = dynamic_cast<CalcDotsParams *>(functorParams);
    assert(params);

    // We currently have no dots object with mensural rests
    if (this->IsMensural()) {
        return FUNCTOR_SIBLINGS;
    }

    // Nothing to do
    if ((this->GetDur() <= DUR_BR) || !this->HasDots()) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);

    if (this->m_crossStaff) staff = this->m_crossStaff;

    bool drawingCueSize = this->IsCueSize();
    int staffSize = staff->m_drawingStaffSize;

    Dots *dots = NULL;

    // For single notes we need here to set the dot loc
    dots = dynamic_cast<Dots *>(this->FindChildByType(DOTS, 1));
    assert(dots);

    std::list<int> *dotLocs = dots->GetDotLocsForStaff(staff);
    int loc = this->GetDrawingLoc();

    // if it's on a staff line to start with, we need to compensate here and add a full unit like DrawDots would
    if ((loc % 2) == 0) {
        loc += 1;
    }
    dotLocs->push_back(loc);

    // HARDCODED
    int xRel = params->m_doc->GetDrawingUnit(staffSize) * 1.5;
    if (drawingCueSize) xRel = params->m_doc->GetCueSize(xRel);
    dots->SetDrawingXRel(std::max(dots->GetDrawingXRel(), xRel));

    return FUNCTOR_SIBLINGS;
}

int Rest::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);
    PositionInterface::InterfaceResetDrawing(functorParams, this);

    return FUNCTOR_CONTINUE;
};

int Rest::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
