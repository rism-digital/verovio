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
#include "staff.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pages
//----------------------------------------------------------------------------

Pages::Pages() : Object(PAGES), AttLabelled(), AttNNumberLike()
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

bool Pages::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ PAGE, SCOREDEF };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

void Pages::ConvertFrom(Score *score)
{
    score->SwapID(this);
    this->AttLabelled::operator=(*score);
    this->AttNNumberLike::operator=(*score);
}

void Pages::LayOutAll()
{
    for (auto child : this->GetChildren()) {
        Page *page = vrv_cast<Page *>(child);
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
// PageRange
//----------------------------------------------------------------------------

PageRange::PageRange(Doc *doc) : Pages()
{
    assert(doc);
    m_doc = doc;

    this->Reset();

    this->SetAsReferenceObject();
}

PageRange::~PageRange() {}

void PageRange::Reset()
{
    Pages::Reset();
    m_focusPage = NULL;
    m_pageBefore.clear();
    m_pageAfter.clear();
}

void PageRange::SetAsFocus(Page *page)
{
    assert(page);

    m_focusPage = page;

    const Object *firstMeasure = page->FindDescendantByType(MEASURE);
    if (firstMeasure) EvaluateSpanningElementsIn(firstMeasure);

    const Measure *lastMeasure
        = vrv_cast<const Measure *>(page->FindDescendantByType(MEASURE, UNLIMITED_DEPTH, BACKWARD));
    if (lastMeasure) {
        this->EvaluateSpanningElementsIn(lastMeasure);
        ListOfConstObjects timeSpanningObjects;
        InterfaceComparison ic(INTERFACE_TIME_SPANNING);
        lastMeasure->FindAllDescendantsByComparison(&timeSpanningObjects, &ic);
        for (const Object *object : timeSpanningObjects) {
            this->Evaluate(object);
        }
    }

    ArrayOfObjects pages = m_doc->GetPages()->GetChildren();

    // Find position of p1 in v1
    auto p1_it = std::find(pages.begin(), pages.end(), m_focusPage);
    // Should not happen
    if (p1_it == pages.end()) return;

    // Find the furthest element in l1 (earliest in v1)
    auto furthestBefore = pages.end();
    for (Page *page : m_pageBefore) {
        auto it = std::find(pages.begin(), pages.end(), page);
        if (it != pages.end() && it < p1_it && it < furthestBefore) {
            furthestBefore = it;
        }
    }

    // Create the list of pages before p1 up to the furthest element
    if (furthestBefore != pages.end()) {
        for (auto it = furthestBefore; it < p1_it; ++it) {
            this->AddChild(*it);
        }
    }

    this->AddChild(m_focusPage);

    // Find the furthest element in l1 (earliest in v1)
    auto furthestAfter = pages.begin();
    for (Page *page : m_pageAfter) {
        auto it = std::find(p1_it, pages.end(), page);
        if (it != pages.end() && it > p1_it && it > furthestAfter) {
            furthestAfter = it;
        }
    }

    // Create the list of pages before p1 up to the furthest element
    if (furthestAfter != pages.begin()) {
        for (auto it = p1_it + 1; it <= furthestAfter; ++it) {
            this->AddChild(*it);
        }
    }

    m_pageBefore.clear();
    m_pageAfter.clear();
}

void PageRange::EvaluateSpanningElementsIn(const Object *measure)
{
    assert(measure);

    ListOfConstObjects staves = measure->FindAllDescendantsByType(STAFF);
    for (const Object *object : staves) {
        const Staff *staff = vrv_cast<const Staff *>(object);
        for (Object *object : staff->m_timeSpanningElements) {
            this->Evaluate(object);
        }
    }
}

void PageRange::Evaluate(const Object *object)
{
    if (!object->HasInterface(INTERFACE_TIME_SPANNING)) return;

    const TimeSpanningInterface *interface = object->GetTimeSpanningInterface();
    assert(interface);
    if (interface->GetStart() && interface->GetStart()->GetFirstAncestor(PAGE) != m_focusPage) {
        const Page *page = vrv_cast<const Page *>(interface->GetStart()->GetFirstAncestor(PAGE));
        assert(page);
        if (std::find(m_pageBefore.begin(), m_pageBefore.end(), page) == m_pageBefore.end()) {
            m_pageBefore.push_back(const_cast<Page *>(page));
        }
    }
    if (interface->GetEnd() && interface->GetEnd()->GetFirstAncestor(PAGE) != m_focusPage) {
        const Page *page = vrv_cast<const Page *>(interface->GetEnd()->GetFirstAncestor(PAGE));
        assert(page);
        if (std::find(m_pageAfter.begin(), m_pageAfter.end(), page) == m_pageAfter.end()) {
            m_pageAfter.push_back(const_cast<Page *>(page));
        }
    }
}

} // namespace vrv
