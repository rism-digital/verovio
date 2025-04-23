/////////////////////////////////////////////////////////////////////////////
// Name:        div.cpp
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "div.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Div
//----------------------------------------------------------------------------

static const ClassRegistrar<Div> s_factory("div", DIV);

Div::Div() : TextLayoutElement(DIV)
{
    this->Reset();
}

Div::~Div() {}

void Div::Reset()
{
    TextLayoutElement::Reset();

    m_drawingInline = false;
}

int Div::GetDrawingX() const
{
    const Object *parent = this->GetParent();
    assert(parent);
    if (m_drawingInline && parent) {
        return parent->GetDrawingX() + this->GetDrawingXRel();
    }
    return parent->GetDrawingX();
}

int Div::GetDrawingY() const
{
    const Object *parent = this->GetParent();
    assert(parent);
    if (m_drawingInline) {
        return parent->GetDrawingY() + this->GetDrawingYRel();
    }
    return parent->GetDrawingY();
}

void Div::SetDrawingXRel(int drawingXRel)
{
    m_drawingXRel = drawingXRel;
}

void Div::SetDrawingYRel(int drawingYRel)
{
    m_drawingYRel = drawingYRel;
}

int Div::GetTotalHeight(const Doc *doc) const
{
    assert(doc);

    int height = this->GetContentHeight();

    return height;
}

int Div::GetTotalWidth(const Doc *doc) const
{
    if (!m_drawingInline) {
        return (doc->m_drawingPageContentWidth);
    }
    else {
        int width = this->GetContentWidth();
        return width;
    }
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Div::Accept(Functor &functor)
{
    return functor.VisitDiv(this);
}

FunctorCode Div::Accept(ConstFunctor &functor) const
{
    return functor.VisitDiv(this);
}

FunctorCode Div::AcceptEnd(Functor &functor)
{
    return functor.VisitDivEnd(this);
}

FunctorCode Div::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitDivEnd(this);
}

} // namespace vrv
