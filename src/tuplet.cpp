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
#include "functorparams.h"
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
    : LayerElement(TUPLET, "tuplet-")
    , ObjectListInterface()
    , AttColor()
    , AttDurationRatio()
    , AttNumberPlacement()
    , AttTupletVis()
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

bool Tuplet::IsSupportedChild(Object *child)
{
    if (child->Is(BEAM)) {
        assert(dynamic_cast<Beam *>(child));
    }
    else if (child->Is(TUPLET_BRACKET)) {
        assert(dynamic_cast<TupletBracket *>(child));
    }
    else if (child->Is(BTREM)) {
        assert(dynamic_cast<BTrem *>(child));
    }
    else if (child->Is(CHORD)) {
        assert(dynamic_cast<Chord *>(child));
    }
    else if (child->Is(CLEF)) {
        assert(dynamic_cast<Clef *>(child));
    }
    else if (child->Is(FTREM)) {
        assert(dynamic_cast<FTrem *>(child));
    }
    else if (child->Is(NOTE)) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->Is(TUPLET_NUM)) {
        assert(dynamic_cast<TupletNum *>(child));
    }
    else if (child->Is(REST)) {
        assert(dynamic_cast<Rest *>(child));
    }
    else if (child->Is(SPACE)) {
        assert(dynamic_cast<Space *>(child));
    }
    else if (child->Is(TABGRP)) {
        assert(dynamic_cast<TabGrp *>(child));
    }
    else if (child->Is(TUPLET)) {
        assert(dynamic_cast<Tuplet *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

void Tuplet::AddChild(Object *child)
{
    if (!this->IsSupportedChild(child)) {
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

    Modify();
}

void Tuplet::AdjustTupletBracketY(const Doc *doc, const Staff *staff)
{
    TupletBracket *tupletBracket = vrv_cast<TupletBracket *>(this->GetFirst(TUPLET_BRACKET));
    if (!tupletBracket || (this->GetBracketVisible() == BOOLEAN_false)) return;

    // if bracket is used for beam elements - process that part separately
    Beam *beam = this->GetBracketAlignedBeam();
    if (beam) return this->AdjustTupletBracketBeamY(doc, staff, tupletBracket, beam);

    const int staffSize = staff->m_drawingStaffSize;
    // Default position is above or below the staff
    int yRel = (m_drawingBracketPos == STAFFREL_basic_above) ? 0 : -doc->GetDrawingStaffSize(staffSize);

    // Check for overlap with content
    // Possible issue with beam above the tuplet - not sure this will be noticeable
    ListOfObjects descendants;
    ClassIdsComparison comparison({ ARTIC, ACCID, BEAM, DOT, FLAG, NOTE, REST, STEM });
    this->FindAllDescendantsByComparison(&descendants, &comparison);

    const int yReference = staff->GetDrawingY();
    for (Object *descendant : descendants) {
        if (!descendant->HasSelfBB()) continue;
        if (vrv_cast<LayerElement *>(descendant)->m_crossStaff) continue;
        if (m_drawingBracketPos == STAFFREL_basic_above) {
            int dist = descendant->GetSelfTop() - yReference;
            if (yRel < dist) yRel = dist;
        }
        else {
            int dist = descendant->GetSelfBottom() - yReference;
            if (yRel > dist) yRel = dist;
        }
    }

    const int sign = (m_drawingBracketPos == STAFFREL_basic_above) ? 1 : -1;
    const int bracketVerticalMargin = sign * doc->GetDrawingDoubleUnit(staffSize);
    tupletBracket->SetDrawingYRel(tupletBracket->GetDrawingYRel() + yRel + bracketVerticalMargin);
}

void Tuplet::AdjustTupletBracketBeamY(const Doc *doc, const Staff *staff, TupletBracket *bracket, const Beam *beam)
{
    const int staffSize = staff->m_drawingStaffSize;
    const int doubleUnit = doc->GetDrawingDoubleUnit(staffSize);
    const int sign = (m_drawingBracketPos == STAFFREL_basic_above) ? 1 : -1;
    int bracketVerticalMargin = sign * doubleUnit;

    // Check for possible articulations
    ListOfObjects artics = this->FindAllDescendantsByType(ARTIC);

    int articPadding = 0;
    for (Object *artic : artics) {
        if (!artic->HasSelfBB()) continue;
        if (m_drawingBracketPos == STAFFREL_basic_above) {
            // Left point when slope is going up and right when going down
            int relevantX = (beam->m_beamSegment.m_beamSlope > 0) ? artic->GetSelfLeft() : artic->GetSelfRight();
            int currentYRel = beam->m_beamSegment.GetStartingY()
                + beam->m_beamSegment.m_beamSlope * (relevantX - beam->m_beamSegment.GetStartingX());
            int articYRel = artic->GetSelfTop();
            articPadding = std::min(currentYRel - articYRel, articPadding);
        }
        else {
            // Right point when slope is going up and left when going down
            int relevantX = (beam->m_beamSegment.m_beamSlope > 0) ? artic->GetSelfRight() : artic->GetSelfLeft();
            int currentYRel = beam->m_beamSegment.GetStartingY()
                + beam->m_beamSegment.m_beamSlope * (relevantX - beam->m_beamSegment.GetStartingX());
            int articYRel = artic->GetSelfBottom();
            articPadding = std::max(currentYRel - articYRel, articPadding);
        }
    }

    // Check for overlap with rest elements. This might happen when tuplet has rest and beam children that are
    // on the same level in encoding - there might be overlap of bracket with rest in that case
    ListOfObjects descendants = this->FindAllDescendantsByType(REST);

    int restAdjust = 0;
    const int bracketRel = bracket->GetDrawingYRel() - articPadding + bracketVerticalMargin;
    const int bracketPosition = (bracket->GetSelfTop() + bracket->GetSelfBottom() + bracketRel) / 2;
    for (Object *descendant : descendants) {
        if (descendant->GetFirstAncestor(BEAM) || !descendant->HasSelfBB()) continue;
        if (m_drawingBracketPos == STAFFREL_basic_above) {
            if (bracketPosition < descendant->GetSelfTop()) {
                const int verticalShift = descendant->GetSelfTop() - bracketPosition;
                if ((restAdjust == 0) || (restAdjust < verticalShift)) restAdjust = verticalShift;
            }
        }
        else {
            if (bracketPosition > descendant->GetSelfBottom()) {
                const int verticalShift = descendant->GetSelfBottom() - bracketPosition;
                if ((restAdjust == 0) || (restAdjust > verticalShift)) restAdjust = verticalShift;
            }
        }
    }
    if (restAdjust) bracketVerticalMargin += restAdjust;

    const int yReference = staff->GetDrawingY();
    bracket->SetDrawingYRel(bracket->GetDrawingYRel() - articPadding + bracketVerticalMargin);

    // Make sure that there are no overlaps with staff lines
    const int staffMargin = (m_drawingBracketPos == STAFFREL_basic_above)
        ? yReference + doubleUnit
        : yReference - doc->GetDrawingStaffSize(staffSize) - doubleUnit;

    const int leftMargin = sign * (staffMargin - bracket->GetDrawingYLeft());
    const int rightMargin = sign * (staffMargin - bracket->GetDrawingYRight());
    const int maxMargin = std::max(leftMargin, rightMargin);

    if (maxMargin > 0) {
        int bracketAdjust = 0;
        if ((leftMargin > 0) && (rightMargin > 0)) {
            bracketAdjust = std::min(leftMargin, rightMargin);
        }

        if (bracketAdjust > 0) bracket->SetDrawingYRel(bracket->GetDrawingYRel() + sign * bracketAdjust);
        if (leftMargin > 0) bracket->SetDrawingYRelLeft(sign * (leftMargin - bracketAdjust));
        if (rightMargin > 0) bracket->SetDrawingYRelRight(sign * (rightMargin - bracketAdjust));
    }

    if (beam->m_crossStaffContent) {
        if ((m_drawingBracketPos == STAFFREL_basic_below) && (beam->m_crossStaffContent->GetN() > staff->GetN())) {
            bracket->SetDrawingYRelLeft(bracket->GetDrawingYRelLeft() - doubleUnit / 4);
            bracket->SetDrawingYRelRight(bracket->GetDrawingYRelRight() - doubleUnit / 4);
        }
    }
}

void Tuplet::AdjustTupletNumY(const Doc *doc, const Staff *staff)
{
    TupletNum *tupletNum = vrv_cast<TupletNum *>(this->GetFirst(TUPLET_NUM));
    if (!tupletNum || (this->GetNumVisible() == BOOLEAN_false)) return;

    // The num is within a bracket
    if (tupletNum->GetAlignedBracket()) {
        // yRel is not used for drawing but we need to adjust it for the bounding box to follow the changes
        tupletNum->SetDrawingYRel(tupletNum->GetAlignedBracket()->GetDrawingYRel());
        return;
    }

    this->CalculateTupletNumCrossStaff(tupletNum);

    const Staff *tupletNumStaff = tupletNum->m_crossStaff ? tupletNum->m_crossStaff : staff;
    const int staffSize = staff->m_drawingStaffSize;
    const int yReference = tupletNumStaff->GetDrawingY();
    const int doubleUnit = doc->GetDrawingDoubleUnit(staffSize);

    // The num is on its own
    const int numVerticalMargin = (m_drawingNumPos == STAFFREL_basic_above) ? doubleUnit : -doubleUnit;
    const int staffHeight = doc->GetDrawingStaffSize(staffSize);
    const int adjustedPosition = (m_drawingNumPos == STAFFREL_basic_above) ? 0 : -staffHeight;
    Beam *beam = this->GetNumAlignedBeam();
    if (!beam) {
        tupletNum->SetDrawingYRel(adjustedPosition);
    }

    // Calculate relative Y for the tupletNum
    AdjustTupletNumOverlapParams adjustTupletNumOverlapParams(tupletNum, tupletNumStaff);
    adjustTupletNumOverlapParams.m_horizontalMargin = 2 * doc->GetDrawingUnit(staffSize);
    adjustTupletNumOverlapParams.m_drawingNumPos = m_drawingNumPos;
    adjustTupletNumOverlapParams.m_yRel = tupletNum->GetDrawingY();
    Functor adjustTupletNumOverlap(&Object::AdjustTupletNumOverlap);
    this->Process(&adjustTupletNumOverlap, &adjustTupletNumOverlapParams);
    int yRel = adjustTupletNumOverlapParams.m_yRel - yReference;

    // If we have a beam, see if we can move it to more appropriate position
    if (beam && !m_crossStaff && !FindDescendantByType(ARTIC)) {
        const int xMid = tupletNum->GetDrawingXMid(doc);
        const int yMid = beam->m_beamSegment.GetStartingY()
            + beam->m_beamSegment.m_beamSlope * (xMid - beam->m_beamSegment.GetStartingX());
        const int beamYRel = yMid - yReference + numVerticalMargin;
        if (((m_drawingNumPos == STAFFREL_basic_above) && (beamYRel > 0))
            || ((m_drawingNumPos == STAFFREL_basic_below) && (beamYRel < -staffHeight))) {
            yRel = beamYRel;
        }
    }
    else {
        yRel += numVerticalMargin;
    }

    // If yRel turns out to be too far from the tuplet - try to adjust it accordingly, aligning with the staff
    // top/bottom sides, unless doing so will make tuplet number overlap
    if (((m_drawingNumPos == STAFFREL_basic_below) && (yRel > adjustedPosition))
        || ((m_drawingNumPos == STAFFREL_basic_above) && (yRel < adjustedPosition))) {
        yRel = adjustedPosition;
    }

    tupletNum->SetDrawingYRel(yRel);
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

    const ListOfObjects &tupletChildren = this->GetList(this);

    // There are unbeamed notes of two different beams
    // treat all the notes as unbeamed
    int ups = 0, downs = 0; // quantity of up- and down-stems

    // The first step is to calculate all the stem directions
    // cycle into the elements and count the up and down dirs
    ListOfObjects::const_iterator iter = tupletChildren.begin();
    while (iter != tupletChildren.end()) {
        if ((*iter)->Is(CHORD)) {
            Chord *currentChord = vrv_cast<Chord *>(*iter);
            assert(currentChord);
            if (currentChord->GetDrawingStemDir() == STEMDIRECTION_up) {
                ups++;
            }
            else {
                downs++;
            }
        }
        else if ((*iter)->Is(NOTE)) {
            Note *currentNote = vrv_cast<Note *>(*iter);
            assert(currentNote);
            if (!currentNote->IsChordTone() && (currentNote->GetDrawingStemDir() == STEMDIRECTION_up)) {
                ups++;
            }
            if (!currentNote->IsChordTone() && (currentNote->GetDrawingStemDir() == STEMDIRECTION_down)) {
                downs++;
            }
        }
        ++iter;
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

    return;
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

FunctorCode Tuplet::Accept(MutableFunctor &functor)
{
    return functor.VisitTuplet(this);
}

FunctorCode Tuplet::Accept(ConstFunctor &functor) const
{
    return functor.VisitTuplet(this);
}

FunctorCode Tuplet::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitTupletEnd(this);
}

FunctorCode Tuplet::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTupletEnd(this);
}

int Tuplet::AdjustTupletsY(FunctorParams *functorParams)
{
    FunctorDocParams *params = vrv_params_cast<FunctorDocParams *>(functorParams);
    assert(params);

    // Nothing to do if there is no number
    if (!this->HasNum()) {
        return FUNCTOR_SIBLINGS;
    }

    // Nothing to do if the bracket and the num are not visible
    if ((this->GetBracketVisible() == BOOLEAN_false) && (this->GetNumVisible() == BOOLEAN_false)) {
        return FUNCTOR_SIBLINGS;
    }

    if (!this->GetDrawingLeft() || !this->GetDrawingRight()) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = this->GetAncestorStaff();

    assert(m_drawingBracketPos != STAFFREL_basic_NONE);

    Staff *relevantStaff = m_crossStaff ? m_crossStaff : staff;

    this->AdjustTupletBracketY(params->m_doc, relevantStaff);

    this->AdjustTupletNumY(params->m_doc, relevantStaff);

    return FUNCTOR_SIBLINGS;
}

int Tuplet::AdjustTupletWithSlurs(FunctorParams *functorParams)
{
    FunctorDocParams *params = vrv_params_cast<FunctorDocParams *>(functorParams);
    assert(params);

    TupletBracket *tupletBracket = vrv_cast<TupletBracket *>(this->GetFirst(TUPLET_BRACKET));
    if (!tupletBracket || m_innerSlurs.empty()) {
        return FUNCTOR_SIBLINGS;
    }
    TupletNum *tupletNum = vrv_cast<TupletNum *>(this->GetFirst(TUPLET_NUM));

    const Staff *staff = this->GetAncestorStaff(RESOLVE_CROSS_STAFF);
    const int margin = params->m_doc->GetDrawingUnit(staff->m_drawingStaffSize) / 2;
    const int sign = (m_drawingBracketPos == STAFFREL_basic_above) ? 1 : -1;

    const int xLeft = this->GetDrawingLeft()->GetDrawingX() + tupletBracket->GetDrawingXRelLeft();
    const int xRight = this->GetDrawingRight()->GetDrawingX() + tupletBracket->GetDrawingXRelRight();
    const int yLeft = tupletBracket->GetDrawingYLeft();
    const int yRight = tupletBracket->GetDrawingYRight();
    const double tupletSlope = double(yRight - yLeft) / double(xRight - xLeft);
    int tupletShift = 0;

    for (auto curve : m_innerSlurs) {
        const int shift = tupletBracket->Intersects(curve, CONTENT, margin) * sign;
        if (shift > 0) {
            // The shift is calculated from the entire bounding box of the tuplet bracket.
            // If the bracket is angled and the slur is short, then this might be too coarse.
            // We reduce the shift by the height of the subbox that cannot be hit.
            Point points[4];
            curve->GetPoints(points);
            const int curveXLeft = std::max(points[0].x, xLeft);
            const int curveXRight = std::min(points[3].x, xRight);
            const int curveYLeft = tupletSlope * (curveXLeft - xLeft) + yLeft;
            const int curveYRight = tupletSlope * (curveXRight - xLeft) + yLeft;

            int reduction = 0;
            if (m_drawingBracketPos == STAFFREL_basic_above) {
                reduction = std::min(curveYLeft, curveYRight) - std::min(yLeft, yRight);
            }
            else {
                reduction = std::max(yLeft, yRight) - std::max(curveYLeft, curveYRight);
            }
            tupletShift = std::max(shift - reduction, tupletShift);
        }
    }

    // Apply the tuplet shift from slurs
    if (tupletShift) {
        tupletBracket->SetDrawingYRel(tupletBracket->GetDrawingYRel() + sign * tupletShift);
        if (tupletNum) tupletNum->SetDrawingYRel(tupletNum->GetDrawingYRel() + sign * tupletShift);
    }

    return FUNCTOR_SIBLINGS;
}

int Tuplet::ResetVerticalAlignment(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetVerticalAlignment(functorParams);

    this->ResetInnerSlurs();

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
