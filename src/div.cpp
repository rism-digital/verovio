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

Div::Div() : TextLayoutElement(DIV, "div-")
{
    this->Reset();
}

Div::~Div() {}

void Div::Reset()
{
    TextLayoutElement::Reset();
}

int Div::GetTotalHeight(const Doc *doc) const
{
    assert(doc);

    int height = this->GetContentHeight();

    return height;
}

int Div::GetTotalWidth(const Doc *doc) const
{
    return (doc->m_drawingPageContentWidth);
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
