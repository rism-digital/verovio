/////////////////////////////////////////////////////////////////////////////
// Name:        tuplet.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tuplet.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "beam.h"
#include "btrem.h"
#include "chord.h"
#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "ftrem.h"
#include "functor.h"
#include "note.h"
#include "rest.h"
#include "space.h"
#include "staff.h"
#include "tabgrp.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tuplet
//----------------------------------------------------------------------------

static const ClassRegistrar<Tuplet> s_factory("tuplet", TUPLET);

Tuplet::Tuplet()
    : LayerElement(TUPLET), ObjectListInterface(), AttColor(), AttDurationRatio(), AttNumberPlacement(), AttTupletVis()
{
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_DURATIONRATIO);
    this->RegisterAttClass(ATT_NUMBERPLACEMENT);
    this->RegisterAttClass(ATT_TUPLETVIS);

    this->Reset();
}

Tuplet::~Tuplet() {}

void Tuplet::Reset()
{
    LayerElement::Reset();
    this->ResetColor();
    this->ResetDurationRatio();
    this->ResetNumberPlacement();
    this->ResetTupletVis();

    m_drawingLeft = NULL;
    m_drawingRight = NULL;
    m_drawingBracketPos = STAFFREL_basic_NONE;
    m_bracketAlignedBeam = NULL;
    m_numAlignedBeam = NULL;
}

bool Tuplet::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ BEAM, TUPLET_BRACKET, BTREM, CHORD, CLEF, FTREM, NOTE, TUPLET_NUM,
        REST, SPACE, TABGRP, TUPLET };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

void Tuplet::AddChild(Object *child)
{
    if (!this->IsSupportedChild(child->GetClassId()) || !this->AddChildAdditionalCheck(child)) {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return;
    }

    child->SetParent(this);

    ArrayOfObjects &children = this->GetChildrenForModification();

    // Num and bracket are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is({ TUPLET_BRACKET, TUPLET_NUM })) {
        children.insert(children.begin(), child);
    }
    else {
        children.push_back(child);
    }

    this->Modify();
}

void Tuplet::FilterList(ListOfConstObjects &childList) const
{
    // We want to keep only notes and rests
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ListOfConstObjects::iterator iter = childList.begin();

    while (iter != childList.end()) {
        if (!(*iter)->IsLayerElement() || !(*iter)->HasInterface(INTERFACE_DURATION)) {
            iter = childList.erase(iter);
        }
        else {
            ++iter;
        }
    }
}

MelodicDirection Tuplet::GetMelodicDirection() const
{
    const LayerElement *leftElement = this->GetDrawingLeft();
    const Note *leftNote = NULL;
    if (leftElement->Is(NOTE)) leftNote = vrv_cast<const Note *>(leftElement);
    if (leftElement->Is(CHORD)) leftNote = vrv_cast<const Chord *>(leftElement)->GetTopNote();

    const LayerElement *rightElement = this->GetDrawingRight();
    const Note *rightNote = NULL;
    if (rightElement->Is(NOTE)) rightNote = vrv_cast<const Note *>(rightElement);
    if (rightElement->Is(CHORD)) rightNote = vrv_cast<const Chord *>(rightElement)->GetTopNote();

    if (leftNote && rightNote) {
        const int leftPitch = leftNote->GetDiatonicPitch();
        const int rightPitch = rightNote->GetDiatonicPitch();
        if (leftPitch < rightPitch) return MelodicDirection::Up;
        if (leftPitch > rightPitch) return MelodicDirection::Down;
    }
    return MelodicDirection::None;
}

void Tuplet::CalculateTupletNumCrossStaff(LayerElement *layerElement)
{
    assert(layerElement);
    // If tuplet is fully cross-staff, just return it - it's enough
    if (m_crossStaff) {
        layerElement->m_crossStaff = m_crossStaff;
        layerElement->m_crossLayer = m_crossLayer;
        return;
    };

    Staff *staff = this->GetAncestorStaff();
    // Find if there is a mix of cross-staff and non-cross-staff elements in the tuplet
    ListOfObjects descendants;
    ClassIdsComparison comparison({ CHORD, NOTE, REST });
    this->FindAllDescendantsByComparison(&descendants, &comparison);

    Staff *crossStaff = NULL;
    Layer *crossLayer = NULL;
    int crossStaffCount = 0;
    for (Object *object : descendants) {
        LayerElement *durElement = vrv_cast<LayerElement *>(object);
        assert(durElement);
        if (crossStaff && durElement->m_crossStaff && (durElement->m_crossStaff != crossStaff)) {
            crossStaff = NULL;
            // We can stop here
            break;
        }
        else if (durElement->m_crossStaff) {
            ++crossStaffCount;
            crossStaff = durElement->m_crossStaff;
            crossLayer = durElement->m_crossLayer;
        }
    }
    if (!crossStaff) return;

    // In case if most elements of the tuplet are cross-staff we need to make sure there for proper positioning of the
    // tuplet number - otherwise tuplet number can end up with extreme adjustments
    const int descendantCount = static_cast<int>(descendants.size());
    const bool isMostlyCrossStaff = crossStaff && (crossStaffCount > descendantCount / 2);
    if ((isMostlyCrossStaff && this->HasValidTupletNumPosition(crossStaff, staff))
        || (!isMostlyCrossStaff && !this->HasValidTupletNumPosition(staff, crossStaff))) {
        layerElement->m_crossStaff = crossStaff;
        layerElement->m_crossLayer = crossLayer;
    }
}

bool Tuplet::HasValidTupletNumPosition(const Staff *preferredStaff, const Staff *otherStaff) const
{
    const Beam *beam = this->GetNumAlignedBeam();
    if (!beam) return true;
    if (beam->m_drawingPlace == BEAMPLACE_mixed) return false;

    if (preferredStaff->GetN() < otherStaff->GetN()) {
        if ((beam->m_drawingPlace == BEAMPLACE_below) && (m_drawingNumPos == STAFFREL_basic_below)) return false;
    }
    else {
        if ((beam->m_drawingPlace == BEAMPLACE_above) && (m_drawingNumPos == STAFFREL_basic_above)) return false;
    }

    return true;
}

void Tuplet::CalcDrawingBracketAndNumPos(bool tupletNumHead)
{
    m_drawingBracketPos = STAFFREL_basic_NONE;

    if (this->HasBracketPlace()) {
        m_drawingBracketPos = this->GetBracketPlace();
    }

    if (this->HasNumPlace()) {
        m_drawingNumPos = this->GetNumPlace();
    }
    else {
        m_drawingNumPos = m_drawingBracketPos;
    }

    // if both are given we are all good (num is set in any case if bracket is)
    if (m_drawingBracketPos != STAFFREL_basic_NONE) {
        return;
    }

    const ListOfObjects &tupletChildren = this->GetList();

    // There are unbeamed notes of two different beams
    // treat all the notes as unbeamed
    int ups = 0, downs = 0; // quantity of up- and down-stems

    // The first step is to calculate all the stem directions
    // cycle into the elements and count the up and down dirs
    for (Object *child : tupletChildren) {
        if (child->Is(CHORD)) {
            Chord *currentChord = vrv_cast<Chord *>(child);
            assert(currentChord);
            if (currentChord->GetDrawingStemDir() == STEMDIRECTION_up) {
                ++ups;
            }
            else {
                ++downs;
            }
        }
        else if (child->Is(NOTE)) {
            Note *currentNote = vrv_cast<Note *>(child);
            assert(currentNote);
            if (!currentNote->IsChordTone() && (currentNote->GetDrawingStemDir() == STEMDIRECTION_up)) {
                ++ups;
            }
            if (!currentNote->IsChordTone() && (currentNote->GetDrawingStemDir() == STEMDIRECTION_down)) {
                ++downs;
            }
        }
    }
    // true means up
    m_drawingBracketPos = ups > downs ? STAFFREL_basic_above : STAFFREL_basic_below;

    if (tupletNumHead) {
        m_drawingBracketPos
            = (m_drawingBracketPos == STAFFREL_basic_below) ? STAFFREL_basic_above : STAFFREL_basic_below;
    }

    // also use it for the num unless it is already set
    if (m_drawingNumPos == STAFFREL_basic_NONE) {
        m_drawingNumPos = m_drawingBracketPos;
    }
}

void Tuplet::GetDrawingLeftRightXRel(int &xRelLeft, int &xRelRight, const Doc *doc) const
{
    assert(m_drawingLeft);
    assert(m_drawingRight);

    xRelLeft = 0;

    if (m_drawingLeft->Is(NOTE)) {
        //
    }
    else if (m_drawingLeft->Is(REST)) {
        //
    }
    else if (m_drawingLeft->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(m_drawingLeft);
        assert(chord);
        xRelLeft = chord->GetXMin() - m_drawingLeft->GetDrawingX();
    }

    xRelRight = 0;

    if (m_drawingRight->Is(NOTE)) {
        xRelRight += (2 * m_drawingRight->GetDrawingRadius(doc));
    }
    else if (m_drawingRight->Is(REST)) {
        xRelRight += m_drawingRight->GetSelfX2();
    }
    else if (m_drawingRight->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(m_drawingRight);
        assert(chord);
        xRelRight = chord->GetXMax() - chord->GetDrawingX() + (2 * chord->GetDrawingRadius(doc));
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Tuplet::Accept(Functor &functor)
{
    return functor.VisitTuplet(this);
}

FunctorCode Tuplet::Accept(ConstFunctor &functor) const
{
    return functor.VisitTuplet(this);
}

FunctorCode Tuplet::AcceptEnd(Functor &functor)
{
    return functor.VisitTupletEnd(this);
}

FunctorCode Tuplet::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTupletEnd(this);
}

} // namespace vrv
