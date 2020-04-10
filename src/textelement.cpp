/////////////////////////////////////////////////////////////////////////////
// Name:        textelement.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "textelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// TextElement
//----------------------------------------------------------------------------

TextElement::TextElement() : Object("te-"), AttLabelled(), AttTyped()
{
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

TextElement::TextElement(const std::string &classid) : Object(classid), AttLabelled(), AttTyped()
{
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

TextElement::~TextElement() {}

void TextElement::Reset()
{
    Object::Reset();
    ResetLabelled();
    ResetTyped();

    m_drawingYRel = 0;
    m_drawingXRel = 0;
}

int TextElement::GetDrawingX() const
{
    // No cache for text elements

    // First get the first layerElement parent (if any) and use its position if they share the same alignment
    Object *textElement = this->GetFirstAncestorInRange(TEXT_ELEMENT, TEXT_ELEMENT_max);
    if (textElement) {
        return (textElement->GetDrawingX() + this->GetDrawingXRel());
    }

    // Otherwise get the running element parent - no cast to RunningElement is necessary
    Object *runningElement = this->GetFirstAncestorInRange(RUNNING_ELEMENT, RUNNING_ELEMENT_max);
    if (runningElement) {
        return (runningElement->GetDrawingX() + this->GetDrawingXRel());
    }

    return Object::GetDrawingX();
}

int TextElement::GetDrawingY() const
{
    // No cache for text elements

    // First get the first layerElement parent (if any) and use its position if they share the same alignment
    Object *textElement = this->GetFirstAncestorInRange(TEXT_ELEMENT, TEXT_ELEMENT_max);
    if (textElement) {
        return (textElement->GetDrawingY() + this->GetDrawingYRel());
    }

    // Otherwise get the running element parent - no cast to RunningElement is necessary
    Object *runningElement = this->GetFirstAncestorInRange(RUNNING_ELEMENT, RUNNING_ELEMENT_max);
    if (runningElement) {
        return (runningElement->GetDrawingY() + this->GetDrawingYRel());
    }

    // TextElement not within RunningElement;
    return Object::GetDrawingY();
}

void TextElement::SetDrawingXRel(int drawingXRel)
{
    ResetCachedDrawingX();
    m_drawingXRel = drawingXRel;
}

void TextElement::SetDrawingYRel(int drawingYRel)
{
    ResetCachedDrawingY();
    m_drawingYRel = drawingYRel;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int TextElement::ResetVerticalAlignment(FunctorParams *functorParams)
{
    SetDrawingXRel(0);
    SetDrawingYRel(0);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
