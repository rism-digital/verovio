/////////////////////////////////////////////////////////////////////////////
// Name:        fig.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fig.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "svg.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fig
//----------------------------------------------------------------------------

Fig::Fig() : Object("fig-"), AttTyped()
{
    RegisterAttClass(ATT_TYPED);
    
    Reset();
}

Fig::~Fig()
{
}

void Fig::Reset()
{
    Object::Reset();
    ResetTyped();
    
    m_drawingYRel = 0;
    m_drawingXRel = 0;
}

void Fig::AddChild(Object *child)
{
    if (child->Is(SVG)) {
        assert(dynamic_cast<Svg *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}
    
    
int Fig::GetDrawingX() const
{
    // No cache for fig elements

    // Get the running element parent - no cast to RunningElement is necessary
    Object *runningElement = this->GetFirstParentInRange(RUNNING_ELEMENT, RUNNING_ELEMENT_max);
    if (runningElement) {
        return (runningElement->GetDrawingX() + this->GetDrawingXRel());
    }
    
    return Object::GetDrawingX();
}
    
int Fig::GetDrawingY() const
{
    // No cache for fig elements

    // Get the running element parent - no cast to RunningElement is necessary
    Object *runningElement = this->GetFirstParentInRange(RUNNING_ELEMENT, RUNNING_ELEMENT_max);
    if (runningElement) {
        return (runningElement->GetDrawingY() + this->GetDrawingYRel());
    }
    
    // TextElement not within RunningElement;
    return Object::GetDrawingY();
}

void Fig::SetDrawingXRel(int drawingXRel)
{
    ResetCachedDrawingX();
    m_drawingXRel = drawingXRel;
}

void Fig::SetDrawingYRel(int drawingYRel)
{
    ResetCachedDrawingY();
    m_drawingYRel = drawingYRel;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------
    
int Fig::ResetVerticalAlignment(FunctorParams *functorParams)
{
    SetDrawingXRel(0);
    SetDrawingYRel(0);

    return FUNCTOR_CONTINUE;
}
    
int Fig::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = dynamic_cast<AlignVerticallyParams *>(functorParams);
    assert(params);
    
    /*
    if (this->GetHalign()) {
        switch (this->GetHalign()) {
            case (HORIZONTALALIGNMENT_right): this->SetDrawingXRel(params->m_pageWidth); break;
            case (HORIZONTALALIGNMENT_center): this->SetDrawingXRel(params->m_pageWidth / 2); break;
            default: break;
        }
    }
    */

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
