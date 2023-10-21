/////////////////////////////////////////////////////////////////////////////
// Name:        tabgrp.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tabgrp.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "note.h"
#include "tabdursym.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabGrp
//----------------------------------------------------------------------------

static const ClassRegistrar<TabGrp> s_factory("tabGrp", TABGRP);

TabGrp::TabGrp() : LayerElement(TABGRP, "tabgrp-"), ObjectListInterface(), DurationInterface()
{
    this->RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());

    this->Reset();
}

TabGrp::~TabGrp() {}

void TabGrp::Reset()
{
    LayerElement::Reset();
    DurationInterface::Reset();
}

bool TabGrp::IsSupportedChild(Object *child)
{
    if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->Is(TABDURSYM)) {
        assert(dynamic_cast<TabDurSym *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

void TabGrp::FilterList(ListOfConstObjects &childList) const
{
    // Retain only note children of chords
    ListOfConstObjects::iterator iter = childList.begin();

    while (iter != childList.end()) {
        if ((*iter)->Is(NOTE)) {
            ++iter;
        }
        else {
            iter = childList.erase(iter);
        }
    }

    childList.sort(TabCourseSort());
}

int TabGrp::GetYTop() const
{
    // The last note is the top
    return this->GetListBack()->GetDrawingY();
}

int TabGrp::GetYBottom() const
{
    // The first note is the bottom
    return this->GetListFront()->GetDrawingY();
}

Note *TabGrp::GetTopNote()
{
    return const_cast<Note *>(std::as_const(*this).GetTopNote());
}

const Note *TabGrp::GetTopNote() const
{
    const Note *topNote = vrv_cast<const Note *>(this->GetListBack());
    assert(topNote);
    return topNote;
}

Note *TabGrp::GetBottomNote()
{
    return const_cast<Note *>(std::as_const(*this).GetBottomNote());
}

const Note *TabGrp::GetBottomNote() const
{
    // The first note is the bottom
    const Note *bottomNote = vrv_cast<const Note *>(this->GetListFront());
    assert(bottomNote);
    return bottomNote;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode TabGrp::Accept(Functor &functor)
{
    return functor.VisitTabGrp(this);
}

FunctorCode TabGrp::Accept(ConstFunctor &functor) const
{
    return functor.VisitTabGrp(this);
}

FunctorCode TabGrp::AcceptEnd(Functor &functor)
{
    return functor.VisitTabGrpEnd(this);
}

FunctorCode TabGrp::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTabGrpEnd(this);
}

} // namespace vrv
