/////////////////////////////////////////////////////////////////////////////
// Name:        ligature.cpp
// Author:      Don Byrd
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ligature.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>

//----------------------------------------------------------------------------

#include "doc.h"
#include "dot.h"
#include "editorial.h"
#include "functor.h"
#include "functorparams.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Ligature
//----------------------------------------------------------------------------

static const ClassRegistrar<Ligature> s_factory("ligature", LIGATURE);

Ligature::Ligature() : LayerElement(LIGATURE, "ligature-"), ObjectListInterface(), AttLigatureVis()
{
    this->RegisterAttClass(ATT_LIGATUREVIS);

    this->Reset();
}

Ligature::~Ligature() {}

void Ligature::Reset()
{
    LayerElement::Reset();
    this->ResetLigatureVis();
}

bool Ligature::IsSupportedChild(Object *child)
{
    if (child->Is(DOT)) {
        assert(dynamic_cast<Dot *>(child));
    }
    else if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

Note *Ligature::GetFirstNote()
{
    return const_cast<Note *>(std::as_const(*this).GetFirstNote());
}

const Note *Ligature::GetFirstNote() const
{
    const Note *firstNote = vrv_cast<const Note *>(this->GetListFront(this));
    assert(firstNote);
    return firstNote;
}

Note *Ligature::GetLastNote()
{
    return const_cast<Note *>(std::as_const(*this).GetLastNote());
}

const Note *Ligature::GetLastNote() const
{
    // The first note is the bottom
    const Note *lastNote = vrv_cast<const Note *>(this->GetListBack(this));
    assert(lastNote);
    return lastNote;
}

void Ligature::FilterList(ListOfConstObjects &childList) const
{
    // Retain only note children of ligatures
    ListOfConstObjects::iterator iter = childList.begin();

    while (iter != childList.end()) {
        if (!(*iter)->Is(NOTE)) {
            // remove anything that is not an LayerElement
            iter = childList.erase(iter);
        }
        else {
            // assert that we keep only notes
            assert(dynamic_cast<const Note *>(*iter));
            ++iter;
        }
    }
}

int Ligature::GetDrawingNoteShape(const Note *note) const
{
    assert(note);
    int position = this->GetListIndex(note);
    if (position == -1) return -1;

    // no check because m_drawingShapes will have been filled by then
    return m_drawingShapes.at(position);
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Ligature::Accept(MutableFunctor &functor)
{
    return functor.VisitLigature(this);
}

FunctorCode Ligature::Accept(ConstFunctor &functor) const
{
    return functor.VisitLigature(this);
}

FunctorCode Ligature::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitLigatureEnd(this);
}

FunctorCode Ligature::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitLigatureEnd(this);
}

} // namespace vrv
