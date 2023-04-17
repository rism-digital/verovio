/////////////////////////////////////////////////////////////////////////////
// Name:        textelement.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "textelement.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextElement
//----------------------------------------------------------------------------

TextElement::TextElement() : Object(TEXT_ELEMENT, "te-"), AttLabelled(), AttTyped()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

TextElement::TextElement(ClassId classId) : Object(classId, "te-"), AttLabelled(), AttTyped()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

TextElement::TextElement(ClassId classId, const std::string &classIdStr)
    : Object(classId, classIdStr), AttLabelled(), AttTyped()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

TextElement::~TextElement() {}

void TextElement::Reset()
{
    Object::Reset();
    this->ResetLabelled();
    this->ResetTyped();

    m_drawingYRel = 0;
    m_drawingXRel = 0;
}

int TextElement::GetDrawingX() const
{
    // No cache for text elements

    // First get the first layerElement parent (if any) and use its position if they share the same alignment
    const Object *textElement = this->GetFirstAncestorInRange(TEXT_ELEMENT, TEXT_ELEMENT_max);
    if (textElement) {
        return (textElement->GetDrawingX() + this->GetDrawingXRel());
    }

    // Otherwise get the running element parent - no cast to RunningElement is necessary
    const Object *runningElement = this->GetFirstAncestorInRange(RUNNING_ELEMENT, RUNNING_ELEMENT_max);
    if (runningElement) {
        return (runningElement->GetDrawingX() + this->GetDrawingXRel());
    }

    return Object::GetDrawingX();
}

int TextElement::GetDrawingY() const
{
    // No cache for text elements

    // First get the first layerElement parent (if any) and use its position if they share the same alignment
    const Object *textElement = this->GetFirstAncestorInRange(TEXT_ELEMENT, TEXT_ELEMENT_max);
    if (textElement) {
        return (textElement->GetDrawingY() + this->GetDrawingYRel());
    }

    // Otherwise get the running element parent - no cast to RunningElement is necessary
    const Object *runningElement = this->GetFirstAncestorInRange(RUNNING_ELEMENT, RUNNING_ELEMENT_max);
    if (runningElement) {
        return (runningElement->GetDrawingY() + this->GetDrawingYRel());
    }

    // TextElement not within RunningElement;
    return Object::GetDrawingY();
}

void TextElement::SetDrawingXRel(int drawingXRel)
{
    this->ResetCachedDrawingX();
    m_drawingXRel = drawingXRel;
}

void TextElement::SetDrawingYRel(int drawingYRel)
{
    this->ResetCachedDrawingY();
    m_drawingYRel = drawingYRel;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode TextElement::Accept(MutableFunctor &functor)
{
    return functor.VisitTextElement(this);
}

FunctorCode TextElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitTextElement(this);
}

FunctorCode TextElement::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitTextElementEnd(this);
}

FunctorCode TextElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTextElementEnd(this);
}

} // namespace vrv
