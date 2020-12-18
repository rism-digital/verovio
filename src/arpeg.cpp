/////////////////////////////////////////////////////////////////////////////
// Name:        arpeg.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "arpeg.h"

//----------------------------------------------------------------------------

#include <assert.h>

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

Arpeg::Arpeg()
    : ControlElement("arpeg-"), PlistInterface(), TimePointInterface(), AttArpegLog(), AttArpegVis(), AttColor()
{
    RegisterInterface(PlistInterface::GetAttClasses(), PlistInterface::IsInterface());
    RegisterInterface(TimePointInterface::GetAttClasses(), TimePointInterface::IsInterface());
    RegisterAttClass(ATT_ARPEGLOG);
    RegisterAttClass(ATT_ARPEGVIS);
    RegisterAttClass(ATT_COLOR);

    Reset();
}

Arpeg::~Arpeg() {}

void Arpeg::Reset()
{
    ControlElement::Reset();
    PlistInterface::Reset();
    TimePointInterface::Reset();
    ResetArpegLog();
    ResetArpegVis();
    ResetColor();

    m_drawingXRel = 0;
}

int Arpeg::GetDrawingX() const
{
    // Getting the position of the current positionner (we have only one for arpeg because values in
    // @staff are not taken into account for arpeg (only @plist)
    // The positionner for Arpeg uses the top note as objectX
    if (this->GetCurrentFloatingPositioner()) {
        return (GetCurrentFloatingPositioner()->GetDrawingX());
    }

    // Otherwise get the measure - no cast to Measure is necessary
    LogDebug("Accessing an arpeg x without positionner");
    Object *measure = this->GetFirstAncestor(MEASURE);
    assert(measure);

    // This will be very arbitrary positionned...
    return measure->GetDrawingX() + this->GetDrawingXRel();
}

bool Arpeg::IsValidRef(Object *ref)
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
    ResetCachedDrawingX();

    m_drawingXRel = drawingXRel;
    // Also update the positonner drawingXRel - this is a duplcation but we need it in
    // the positionner too for the bounding box calculation and for the DrawingX value
    // See GetDrawingX
    if (GetCurrentFloatingPositioner()) {
        GetCurrentFloatingPositioner()->SetDrawingXRel(m_drawingXRel);
    }
}

void Arpeg::GetDrawingTopBottomNotes(Note *&top, Note *&bottom)
{
    top = NULL;
    bottom = NULL;

    Object *front = NULL;
    Object *back = NULL;

    if (this->GetStart()) {
        front = this->GetStart();
        back = this->GetStart();
    }
    else if (!this->GetRefs()->empty()) {
        front = this->GetRefs()->front();
        back = this->GetRefs()->back();
    }

    // Cannot draw an arpeg that has no target
    if (!front || !back) return;

    // Cannot draw an arpeg not pointing to a chord or a note
    if (!front->Is({ CHORD, NOTE }) || !back->Is({ CHORD, NOTE })) return;

    // Pointing to a single element
    if (front == back) {
        // It has to be a chord in this case
        if (front->Is(NOTE)) return;
        Chord *chord = vrv_cast<Chord *>(front);
        assert(chord);
        top = chord->GetTopNote();
        bottom = chord->GetBottomNote();
        return;
    }

    Chord *chord1 = NULL;
    Chord *chord2 = NULL;
    Note *note1 = NULL;
    Note *note2 = NULL;

    // Get the first and second chord or note
    if (front->Is(CHORD)) {
        chord1 = vrv_cast<Chord *>(front);
        assert(chord1);
    }
    else {
        note1 = vrv_cast<Note *>(front);
        assert(note1);
    }
    if (back->Is(CHORD)) {
        chord2 = vrv_cast<Chord *>(back);
        assert(chord2);
    }
    else {
        note2 = vrv_cast<Note *>(back);
        assert(note2);
    }

    // Note get the top and bottom note accordingly
    if (chord1 && chord2) {
        top = (chord1->GetTopNote()->GetDrawingY() > chord2->GetTopNote()->GetDrawingY()) ? chord1->GetTopNote()
                                                                                          : chord2->GetTopNote();
        bottom = (chord1->GetBottomNote()->GetDrawingY() < chord2->GetBottomNote()->GetDrawingY())
            ? chord1->GetBottomNote()
            : chord2->GetBottomNote();
    }
    else if (chord1 && note2) {
        top = (chord1->GetTopNote()->GetDrawingY() > note2->GetDrawingY()) ? chord1->GetTopNote() : note2;
        bottom = (chord1->GetBottomNote()->GetDrawingY() < note2->GetDrawingY()) ? chord1->GetBottomNote() : note2;
    }
    else if (note1 && chord2) {
        top = (note1->GetDrawingY() > chord2->GetTopNote()->GetDrawingY()) ? note1 : chord2->GetTopNote();
        bottom = (note1->GetDrawingY() < chord2->GetBottomNote()->GetDrawingY()) ? note1 : chord2->GetBottomNote();
    }
    else {
        top = (note1->GetDrawingY() > note2->GetDrawingY()) ? note1 : note2;
        bottom = (note1->GetDrawingY() < note2->GetDrawingY()) ? note1 : note2;
    }
}

Staff *Arpeg::GetCrossStaff()
{
    const ArrayOfObjects *refs = GetRefs();
    if (refs->empty()) return NULL;

    // Find if there is at least one element that is not cross staff
    auto iter = std::find_if(refs->begin(), refs->end(), [](Object *obj) {
        LayerElement *element = vrv_cast<LayerElement *>(obj);
        assert(element);
        return !element->m_crossStaff;
    });

    // If that's the case - return NULL, we can base arpegio location on the original staff
    if (iter != refs->end()) return NULL;

    // Otherwise return cross staff of the front element from the references
    LayerElement *front = vrv_cast<LayerElement *>(refs->front());
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

    Staff *topStaff = vrv_cast<Staff *>(topNote->GetFirstAncestor(STAFF));
    assert(topStaff);

    Staff *bottomStaff = vrv_cast<Staff *>(bottomNote->GetFirstAncestor(STAFF));
    assert(bottomStaff);

    Staff *crossStaff = GetCrossStaff();
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
        dist += (params->m_doc->GetDrawingUnit(topStaff->m_drawingStaffSize));
        this->SetDrawingXRel(-dist);
    }

    return FUNCTOR_CONTINUE;
}

int Arpeg::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    ControlElement::ResetDrawing(functorParams);

    PlistInterface *interface = this->GetPlistInterface();
    assert(interface);
    return interface->InterfaceResetDrawing(functorParams, this);
}

} // namespace vrv
