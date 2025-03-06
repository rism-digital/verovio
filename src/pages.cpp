/////////////////////////////////////////////////////////////////////////////
// Name:        pages.cpp
// Author:      Laurent Pugin
// Created:     2018/02/15
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pages.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functor.h"
#include "page.h"
#include "score.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pages
//----------------------------------------------------------------------------

Pages::Pages() : Object(PAGES, "pages-"), AttLabelled(), AttNNumberLike()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Pages::~Pages() {}

void Pages::Reset()
{
    Object::Reset();
    this->ResetLabelled();
    this->ResetNNumberLike();
}

bool Pages::IsSupportedChild(Object *child)
{
    if (child->Is(PAGE)) {
        assert(dynamic_cast<Page *>(child));
    }
    else if (child->Is(SCOREDEF)) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else {
        return false;
    }
    return true;
}

void Pages::ConvertFrom(Score *score)
{
    score->SwapID(this);
    this->AttLabelled::operator=(*score);
    this->AttNNumberLike::operator=(*score);
}

void Pages::LayoutAll()
{
    Doc *doc = vrv_cast<Doc*>(this->GetFirstAncestor(DOC));
    
    for (auto child : this->GetChildren()) {
        Page *page = vrv_cast<Page*>(child);
        page->InvalidLayout();
        doc->SetDrawingPage(page->GetIdx(), false);
        page->LayOut();
    }
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Pages::Accept(Functor &functor)
{
    return functor.VisitPages(this);
}

FunctorCode Pages::Accept(ConstFunctor &functor) const
{
    return functor.VisitPages(this);
}

FunctorCode Pages::AcceptEnd(Functor &functor)
{
    return functor.VisitPagesEnd(this);
}

FunctorCode Pages::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitPagesEnd(this);
}

//----------------------------------------------------------------------------
// FocusSet
//----------------------------------------------------------------------------

FocusSet::FocusSet(Doc *doc) : Pages()
{
    assert(doc);
    m_doc = doc;
    
    this->Reset();
    
    this->SetAsReferenceObject();

}

FocusSet::~FocusSet()
{

}

void FocusSet::Reset()
{
    Pages::Reset();
    m_focus = NULL;
}

void FocusSet::SetAsFocus(Page *page)
{
    m_focus = page;
}

void FocusSet::Layout()
{
    assert(m_focus);
    
    for (auto child : this->GetChildren()) {
        Page *page = vrv_cast<Page*>(child);
        page->InvalidLayout();
        m_doc->SetDrawingPage(page->GetIdx(), false);
        page->LayOut();
    }
    m_doc->SetDrawingPage(m_focus->GetIdx(), false);
}


} // namespace vrv
