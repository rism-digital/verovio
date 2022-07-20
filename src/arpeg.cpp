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
#include "functorparams.h"
#include "horizontalaligner.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Arpeg
//----------------------------------------------------------------------------

static const ClassRegistrar<Arpeg> s_factory("arpeg", ARPEG);

Arpeg::Arpeg()
    : ControlElement(ARPEG, "arpeg-"), PlistInterface(), TimePointInterface(), AttArpegLog(), AttArpegVis(), AttColor()
{
    this->RegisterInterface(PlistInterface::GetAttClasses(), PlistInterface::IsInterface());
    this->RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    this->RegisterAttClass(ATT_ARPEGLOG);
    this->RegisterAttClass(ATT_ARPEGVIS);
    this->RegisterAttClass(ATT_COLOR);
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
    this->ResetColor();
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
            const ListOfConstObjects &childList = chord->GetList(chord);
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

int Arpeg::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    m_drawingXRel = 0;

    return ControlElement::ResetHorizontalAlignment(functorParams);
}

int Arpeg::AdjustArpeg(FunctorParams *functorParams)
{
    AdjustArpegParams *params = vrv_params_cast<AdjustArpegParams *>(functorParams);
    assert(params);

    Note *topNote = NULL;
    Note *bottomNote = NULL;

    this->GetDrawingTopBottomNotes(topNote, bottomNote);

    // Nothing to do
    if (!topNote || !bottomNote) return FUNCTOR_CONTINUE;

    // We should have call DrawArpeg before
    assert(this->GetCurrentFloatingPositioner());

    Staff *topStaff = topNote->GetAncestorStaff();
    Staff *bottomStaff = bottomNote->GetAncestorStaff();

    Staff *crossStaff = this->GetCrossStaff();
    const int staffN = (crossStaff != NULL) ? crossStaff->GetN() : topStaff->GetN();

    int minTopLeft, maxTopRight;
    topNote->GetAlignment()->GetLeftRight(staffN, minTopLeft, maxTopRight);

    params->m_alignmentArpegTuples.push_back(std::make_tuple(topNote->GetAlignment(), this, topStaff->GetN(), false));

    if (topStaff != bottomStaff) {
        int minBottomLeft, maxBottomRight;
        topNote->GetAlignment()->GetLeftRight(bottomStaff->GetN(), minBottomLeft, maxBottomRight);
        minTopLeft = std::min(minTopLeft, minBottomLeft);

        params->m_alignmentArpegTuples.push_back(
            std::make_tuple(topNote->GetAlignment(), this, bottomStaff->GetN(), false));
    }

    if (minTopLeft != -VRV_UNSET) {
        int dist = topNote->GetDrawingX() - minTopLeft;
        // HARDCODED
        double unitFactor = 1.0;
        if ((this->GetEnclose() == ENCLOSURE_brack) || (this->GetEnclose() == ENCLOSURE_box)) unitFactor += 0.75;
        if (this->GetArrow() == BOOLEAN_true) unitFactor += 0.33;
        dist += unitFactor * params->m_doc->GetDrawingUnit(topStaff->m_drawingStaffSize);
        this->SetDrawingXRel(-dist);
    }

    return FUNCTOR_CONTINUE;
}

int Arpeg::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetData(functorParams);

    PlistInterface *interface = this->GetPlistInterface();
    assert(interface);
    return interface->InterfaceResetData(functorParams, this);
}

int Arpeg::CacheHorizontalLayout(FunctorParams *functorParams)
{
    CacheHorizontalLayoutParams *params = vrv_params_cast<CacheHorizontalLayoutParams *>(functorParams);
    assert(params);

    if (params->m_restore) {
        m_drawingXRel = m_cachedXRel;
    }
    else {
        m_cachedXRel = m_drawingXRel;
    }

    return FUNCTOR_CONTINUE;
}

int Arpeg::InitMIDI(FunctorParams *functorParams)
{
    InitMIDIParams *params = vrv_params_cast<InitMIDIParams *>(functorParams);
    assert(params);

    // Sort the involved notes by playing order
    const bool playTopDown = (this->GetOrder() == arpegLog_ORDER_down);
    std::set<Note *> notes = this->GetNotes();
    std::vector<Note *> sortedNotes;
    std::copy(notes.begin(), notes.end(), std::back_inserter(sortedNotes));
    std::sort(sortedNotes.begin(), sortedNotes.end(), [playTopDown](Note *note1, Note *note2) {
        const int pitch1 = note1->GetMIDIPitch();
        const int pitch2 = note2->GetMIDIPitch();
        return playTopDown ? (pitch1 > pitch2) : (pitch1 < pitch2);
    });

    // Defer the notes in playing order
    double shift = 0.0;
    const double increment = UNACC_GRACENOTE_DUR * params->m_currentTempo / 60000.0;
    for (Note *note : sortedNotes) {
        if (shift > 0.0) params->m_deferredNotes[note] = shift;
        shift += increment;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
