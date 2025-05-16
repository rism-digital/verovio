/////////////////////////////////////////////////////////////////////////////
// Name:        arpeg.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "arpeg.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "chord.h"
#include "doc.h"
#include "functor.h"
#include "horizontalaligner.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Arpeg
//----------------------------------------------------------------------------

static const ClassRegistrar<Arpeg> s_factory("arpeg", ARPEG);

Arpeg::Arpeg() : ControlElement(ARPEG), PlistInterface(), TimePointInterface(), AttArpegLog(), AttArpegVis()
{
    this->RegisterInterface(PlistInterface::GetAttClasses(), PlistInterface::IsInterface());
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_ARPEGLOG);
    this->RegisterAttClass(ATT_ARPEGVIS);
    this->RegisterAttClass(ATT_ENCLOSINGCHARS);

    this->Reset();
}

Arpeg::~Arpeg() {}

void Arpeg::Reset()
{
    ControlElement::Reset();
    PlistInterface::Reset();
    TimePointInterface::Reset();
    this->ResetArpegLog();
    this->ResetArpegVis();
    this->ResetEnclosingChars();

    m_drawingXRel = 0;
    m_cachedXRel = VRV_UNSET;
}

int Arpeg::GetDrawingX() const
{
    // Getting the position of the current positioner (we have only one for arpeg because values in
    // @staff are not taken into account for arpeg (only @plist)
    // The positioner for Arpeg uses the top note as objectX
    if (this->GetCurrentFloatingPositioner()) {
        return (this->GetCurrentFloatingPositioner()->GetDrawingX());
    }

    // Otherwise get the measure - no cast to Measure is necessary
    LogDebug("Accessing an arpeg x without positioner");
    const Object *measure = this->GetFirstAncestor(MEASURE);
    assert(measure);

    // This will be very arbitrary positionned...
    return measure->GetDrawingX() + this->GetDrawingXRel();
}

bool Arpeg::IsValidRef(const Object *ref) const
{
    if (!ref->Is({ CHORD, NOTE })) {
        LogWarning(
            "%s is not supported as @plist target for %s", ref->GetClassName().c_str(), this->GetClassName().c_str());
        return false;
    }
    return true;
}

void Arpeg::SetDrawingXRel(int drawingXRel)
{
    // Cache is currently not used for Arpeg
    this->ResetCachedDrawingX();

    m_drawingXRel = drawingXRel;
    // Also update the positioner drawingXRel - this is a duplication but we need it in
    // the positioner too for the bounding box calculation and for the DrawingX value
    // See GetDrawingX
    if (this->GetCurrentFloatingPositioner()) {
        this->GetCurrentFloatingPositioner()->SetDrawingXRel(m_drawingXRel);
    }
}

void Arpeg::CacheXRel(bool restore)
{
    if (restore) {
        m_drawingXRel = m_cachedXRel;
    }
    else {
        m_cachedXRel = m_drawingXRel;
    }
}

std::set<Note *> Arpeg::GetNotes()
{
    std::set<Note *> result;
    std::set<const Note *> notes = std::as_const(*this).GetNotes();
    std::for_each(notes.begin(), notes.end(), [&result](const Note *note) { result.insert(const_cast<Note *>(note)); });
    return result;
}

std::set<const Note *> Arpeg::GetNotes() const
{
    std::set<const Note *> notes;
    auto extractNotes = [&notes](const Object *object) {
        if (!object) return;
        if (object->Is(NOTE)) {
            const Note *note = vrv_cast<const Note *>(object);
            assert(note);
            notes.insert(note);
        }
        else if (object->Is(CHORD)) {
            const Chord *chord = vrv_cast<const Chord *>(object);
            const ListOfConstObjects &childList = chord->GetList();
            for (const Object *child : childList) {
                const Note *note = vrv_cast<const Note *>(child);
                assert(note);
                notes.insert(note);
            }
        }
    };
    extractNotes(this->GetStart());
    const ArrayOfConstObjects &refs = this->GetRefs();
    std::for_each(refs.begin(), refs.end(), extractNotes);
    return notes;
}

void Arpeg::GetDrawingTopBottomNotes(Note *&top, Note *&bottom)
{
    std::set<Note *> notes = this->GetNotes();
    if (notes.size() > 1) {
        // Sort the involved notes by drawing Y position
        std::vector<Note *> sortedNotes;
        std::copy(notes.begin(), notes.end(), std::back_inserter(sortedNotes));
        std::sort(sortedNotes.begin(), sortedNotes.end(),
            [](Note *note1, Note *note2) { return (note1->GetDrawingY() > note2->GetDrawingY()); });

        top = sortedNotes.front();
        bottom = sortedNotes.back();
    }
    else {
        top = NULL;
        bottom = NULL;
    }
}

Staff *Arpeg::GetCrossStaff()
{
    return const_cast<Staff *>(std::as_const(*this).GetCrossStaff());
}

const Staff *Arpeg::GetCrossStaff() const
{
    const ArrayOfConstObjects &refs = this->GetRefs();
    if (refs.empty()) return NULL;

    // Find if there is at least one element that is not cross staff
    auto iter = std::find_if(refs.begin(), refs.end(), [](const Object *obj) {
        const LayerElement *element = vrv_cast<const LayerElement *>(obj);
        assert(element);
        return !element->m_crossStaff;
    });

    // If that's the case - return NULL, we can base arpeggio location on the original staff
    if (iter != refs.end()) return NULL;

    // Otherwise return cross staff of the front element from the references
    const LayerElement *front = vrv_cast<const LayerElement *>(refs.front());
    assert(front);
    return front->m_crossStaff;
}

//----------------------------------------------------------------------------
// Arpeg functor methods
//----------------------------------------------------------------------------

FunctorCode Arpeg::Accept(Functor &functor)
{
    return functor.VisitArpeg(this);
}

FunctorCode Arpeg::Accept(ConstFunctor &functor) const
{
    return functor.VisitArpeg(this);
}

FunctorCode Arpeg::AcceptEnd(Functor &functor)
{
    return functor.VisitArpegEnd(this);
}

FunctorCode Arpeg::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitArpegEnd(this);
}

} // namespace vrv
