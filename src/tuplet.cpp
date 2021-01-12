/////////////////////////////////////////////////////////////////////////////
// Name:        tuplet.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tuplet.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "btrem.h"
#include "chord.h"
#include "comparison.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
#include "ftrem.h"
#include "functorparams.h"
#include "note.h"
#include "rest.h"
#include "space.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tuplet
//----------------------------------------------------------------------------

Tuplet::Tuplet()
    : LayerElement("tuplet-")
    , ObjectListInterface()
    , AttColor()
    , AttDurationRatio()
    , AttNumberPlacement()
    , AttTupletVis()
{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_DURATIONRATIO);
    RegisterAttClass(ATT_NUMBERPLACEMENT);
    RegisterAttClass(ATT_TUPLETVIS);

    Reset();
}

Tuplet::~Tuplet() {}

void Tuplet::Reset()
{
    LayerElement::Reset();
    ResetColor();
    ResetDurationRatio();
    ResetNumberPlacement();
    ResetTupletVis();

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

    ArrayOfObjects *children = this->GetChildrenForModification();

    // Num and bracket are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is({ TUPLET_BRACKET, TUPLET_NUM })) {
        children->insert(children->begin(), child);
    }
    else {
        children->push_back(child);
    }

    Modify();
}

void Tuplet::FilterList(ArrayOfObjects *childList)
{
    // We want to keep only notes and rests
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ArrayOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->IsLayerElement() || !(*iter)->HasInterface(INTERFACE_DURATION)) {
            iter = childList->erase(iter);
        }
        else {
            ++iter;
        }
    }
}

void Tuplet::CalcDrawingBracketAndNumPos()
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

    const ArrayOfObjects *tupletChildren = this->GetList(this);

    // There are unbeamed notes of two different beams
    // treat all the notes as unbeamed
    int ups = 0, downs = 0; // quantity of up- and down-stems

    // The first step is to calculate all the stem directions
    // cycle into the elements and count the up and down dirs
    ArrayOfObjects::const_iterator iter = tupletChildren->begin();
    while (iter != tupletChildren->end()) {
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

    // also use it for the num unless it is already set
    if (m_drawingNumPos == STAFFREL_basic_NONE) {
        m_drawingNumPos = m_drawingBracketPos;
    }

    return;
}

void Tuplet::GetDrawingLeftRightXRel(int &XRelLeft, int &XRelRight, Doc *doc)
{
    assert(m_drawingLeft);
    assert(m_drawingRight);

    XRelLeft = 0;

    if (m_drawingLeft->Is(NOTE)) {
        //
    }
    else if (m_drawingLeft->Is(REST)) {
        //
    }
    else if (m_drawingLeft->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(m_drawingLeft);
        assert(chord);
        XRelLeft = chord->GetXMin() - m_drawingLeft->GetDrawingX();
    }

    XRelRight = 0;

    if (m_drawingRight->Is(NOTE)) {
        XRelRight += (2 * m_drawingRight->GetDrawingRadius(doc));
    }
    else if (m_drawingRight->Is(REST)) {
        XRelRight += m_drawingRight->GetSelfX2();
    }
    else if (m_drawingRight->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(m_drawingRight);
        assert(chord);
        XRelRight = chord->GetXMax() - chord->GetDrawingX() + (2 * chord->GetDrawingRadius(doc));
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Tuplet::PrepareLayerElementParts(FunctorParams *functorParams)
{
    TupletBracket *currentBracket = dynamic_cast<TupletBracket *>(this->FindDescendantByType(TUPLET_BRACKET, 1));
    TupletNum *currentNum = dynamic_cast<TupletNum *>(this->FindDescendantByType(TUPLET_NUM, 1));

    bool beamed = false;
    // Are we contained in a beam?
    if (this->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH)) {
        // is only the tuplet beamed? (will not work with nested tuplets)
        Beam *currentBeam = dynamic_cast<Beam *>(this->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
        if (currentBeam->GetChildCount() == 1) {
            beamed = true;
        }
    }
    // Is a beam the only child? (will not work with editorial elements)
    if ((this->GetChildCount() == 1) && (this->GetChildCount(BEAM) == 1)) beamed = true;

    if ((!this->HasBracketVisible() && !beamed) || (this->GetBracketVisible() == BOOLEAN_true)) {
        if (!currentBracket) {
            currentBracket = new TupletBracket();
            this->AddChild(currentBracket);
        }
        currentBracket->AttTupletVis::operator=(*this);
    }
    // This will happen only if the @bracket.visible value has changed
    else if (currentBracket) {
        if (this->DeleteChild(currentBracket)) {
            currentBracket = NULL;
        }
    }

    if (this->HasNum() && (!this->HasNumVisible() || (this->GetNumVisible() == BOOLEAN_true))) {
        if (!currentNum) {
            currentNum = new TupletNum();
            this->AddChild(currentNum);
        }
        currentNum->AttNumberPlacement::operator=(*this);
        currentNum->AttTupletVis::operator=(*this);
    }
    // This will happen only if the @num.visible value has changed
    else if (currentNum) {
        if (this->DeleteChild(currentNum)) {
            currentNum = NULL;
        }
    }

    /************ Prepare the drawing cue size ************/

    Functor prepareDrawingCueSize(&Object::PrepareDrawingCueSize);
    this->Process(&prepareDrawingCueSize, NULL);

    /*********** Get the left and right element ***********/

    ClassIdsComparison comparison({ CHORD, NOTE, REST });
    m_drawingLeft = dynamic_cast<LayerElement *>(this->FindDescendantByComparison(&comparison));
    m_drawingRight
        = dynamic_cast<LayerElement *>(this->FindDescendantByComparison(&comparison, UNLIMITED_DEPTH, BACKWARD));

    return FUNCTOR_CONTINUE;
}

int Tuplet::AdjustTupletsX(FunctorParams *functorParams)
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

    // Nothing we can to the pointers to the left and right are not set
    if (!this->GetDrawingLeft() || !this->GetDrawingRight()) {
        return FUNCTOR_SIBLINGS;
    }

    assert(m_drawingBracketPos != STAFFREL_basic_NONE);

    // Carefull: this will not work if the tuplet has editorial markup (one child) and then notes + one beam
    Beam *beamParent = dynamic_cast<Beam *>(this->GetFirstAncestor(BEAM, MAX_BEAM_DEPTH));
    // Are we contained in a beam?
    if (beamParent) {
        m_bracketAlignedBeam = beamParent;
    }
    Beam *beamChild = dynamic_cast<Beam *>(this->FindDescendantByType(BEAM));
    // Do we contain a beam?
    if (beamChild) {
        if ((this->GetChildCount(NOTE) == 0) && (this->GetChildCount(CHORD) == 0) && (this->GetChildCount(BEAM) == 1)) {
            m_bracketAlignedBeam = beamChild;
        }
    }

    m_numAlignedBeam = m_bracketAlignedBeam;

    // Cancel alignment of the bracket with the beam if position and stemdirection are not concordant
    if (m_bracketAlignedBeam && (m_bracketAlignedBeam->m_drawingPlace == BEAMPLACE_above)
        && (m_drawingBracketPos == STAFFREL_basic_below)) {
        m_bracketAlignedBeam = NULL;
    }
    else if (m_bracketAlignedBeam
        && ((m_bracketAlignedBeam->m_drawingPlace == BEAMPLACE_below)
            && (m_drawingBracketPos == STAFFREL_basic_above))) {
        m_bracketAlignedBeam = NULL;
    }

    // Cancel alignment of the num with the beam if position and stemdirection are not concordant
    if (m_numAlignedBeam && (m_numAlignedBeam->m_drawingPlace == BEAMPLACE_above)
        && (m_drawingNumPos == STAFFREL_basic_below)) {
        m_numAlignedBeam = NULL;
    }
    else if (m_numAlignedBeam
        && ((m_numAlignedBeam->m_drawingPlace == BEAMPLACE_below) && (m_drawingNumPos == STAFFREL_basic_above))) {
        m_numAlignedBeam = NULL;
    }

    int XRelLeft;
    int XRelRight;
    this->GetDrawingLeftRightXRel(XRelLeft, XRelRight, params->m_doc);

    TupletBracket *tupletBracket = dynamic_cast<TupletBracket *>(this->FindDescendantByType(TUPLET_BRACKET));
    if (tupletBracket && (this->GetBracketVisible() != BOOLEAN_false)) {
        tupletBracket->SetDrawingXRelLeft(XRelLeft);
        tupletBracket->SetDrawingXRelRight(XRelRight);
    }

    TupletNum *tupletNum = dynamic_cast<TupletNum *>(this->FindDescendantByType(TUPLET_NUM));
    if (tupletNum && (this->GetNumVisible() != BOOLEAN_false)) {
        // We have a bracket and the num is not on its opposite side
        if (tupletBracket && (m_drawingNumPos == m_drawingBracketPos)) {
            tupletNum->SetAlignedBracket(tupletBracket);
        }
        else {
            tupletNum->SetAlignedBracket(NULL);
        }
    }

    return FUNCTOR_SIBLINGS;
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

    Staff *staff = vrv_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);
    int staffSize = staff->m_drawingStaffSize;

    assert(m_drawingBracketPos != STAFFREL_basic_NONE);

    int verticalLine = params->m_doc->GetDrawingUnit(staffSize);
    int verticalMargin = 2 * verticalLine;
    const int yReference = m_crossStaff ? m_crossStaff->GetDrawingY() : staff->GetDrawingY();

    TupletBracket *tupletBracket = dynamic_cast<TupletBracket *>(this->FindDescendantByType(TUPLET_BRACKET));
    if (tupletBracket && (this->GetBracketVisible() != BOOLEAN_false)) {

        int bracketVerticalMargin = verticalMargin;
        bracketVerticalMargin *= (m_drawingBracketPos == STAFFREL_basic_above) ? 1 : -1;

        Beam *beam = this->GetBracketAlignedBeam();
        if (beam) {
            // Check for possible articulations
            ListOfObjects artics;
            ClassIdsComparison comparison({ ARTIC, ARTIC_PART });
            this->FindAllDescendantByComparison(&artics, &comparison);

            int articPadding = 0;
            for (auto &artic : artics) {
                if (!artic->HasSelfBB()) continue;
                if (m_drawingBracketPos == STAFFREL_basic_above) {
                    // Left point when slope is going up and right when going down
                    int relevantX
                        = (beam->m_beamSegment.m_beamSlope > 0) ? artic->GetSelfLeft() : artic->GetSelfRight();
                    int currentYRel = beam->m_beamSegment.m_startingY
                        + beam->m_beamSegment.m_beamSlope * (relevantX - beam->m_beamSegment.m_startingX);
                    int articYRel = artic->GetSelfTop();
                    articPadding = std::min(currentYRel - articYRel, articPadding);
                }
                else {
                    // Right point when slope is going up and left when going down
                    int relevantX
                        = (beam->m_beamSegment.m_beamSlope > 0) ? artic->GetSelfRight() : artic->GetSelfLeft();
                    int currentYRel = beam->m_beamSegment.m_startingY
                        + beam->m_beamSegment.m_beamSlope * (relevantX - beam->m_beamSegment.m_startingX);
                    int articYRel = artic->GetSelfBottom();
                    articPadding = std::max(currentYRel - articYRel, articPadding);
                }
            }

            tupletBracket->SetDrawingYRel(tupletBracket->GetDrawingYRel() - articPadding + bracketVerticalMargin);
        }
        else {
            // Default position is above or below the staff
            int yRel
                = (m_drawingBracketPos == STAFFREL_basic_above) ? 0 : -params->m_doc->GetDrawingStaffSize(staffSize);

            // Check for overlap with content
            // Possible issue with beam above the tuplet - not sure this will be noticable
            ListOfObjects descendants;
            ClassIdsComparison comparison({ ARTIC, ARTIC_PART, ACCID, BEAM, DOT, FLAG, NOTE, REST, STEM });
            this->FindAllDescendantByComparison(&descendants, &comparison);

            // Possible fix for beam above tuplet
            /*
            Object *parentBeam = this->GetFirstAncestor(BEAM);
            if (parentBeam) {
                descendants.push_back(parentBeam);
            }
            */

            for (auto &descendant : descendants) {
                if (!descendant->HasSelfBB()) continue;
                if (m_drawingBracketPos == STAFFREL_basic_above) {
                    int dist = descendant->GetSelfTop() - yReference;
                    if (yRel < dist) yRel = dist;
                }
                else {
                    int dist = descendant->GetSelfBottom() - yReference;
                    if (yRel > dist) yRel = dist;
                }
            }

            tupletBracket->SetDrawingYRel(tupletBracket->GetDrawingYRel() + yRel + bracketVerticalMargin);
        }
    }

    TupletNum *tupletNum = dynamic_cast<TupletNum *>(this->FindDescendantByType(TUPLET_NUM));
    if (tupletNum && (this->GetNumVisible() != BOOLEAN_false)) {
        // The num is within a bracket
        if (tupletNum->GetAlignedBracket()) {
            // yRel is not used for drawing but we need to adjust it for the bounding box to follow the changes
            tupletNum->SetDrawingYRel(tupletNum->GetAlignedBracket()->GetDrawingYRel());
        }
        // The num is on its own
        else {
            int numVerticalMargin = verticalMargin;
            numVerticalMargin *= (m_drawingNumPos == STAFFREL_basic_above) ? 1 : -1;

            Beam *beam = this->GetNumAlignedBeam();
            // If we have a beam first move it to the appropriate postion
            if (beam) {

                int xMid = tupletNum->GetDrawingXMid(params->m_doc);
                int yMid = beam->m_beamSegment.m_startingY
                    + beam->m_beamSegment.m_beamSlope * (xMid - beam->m_beamSegment.m_startingX);
                int yMidRel = yMid - yReference;

                tupletNum->SetDrawingYRel(tupletNum->GetDrawingYRel() + yMidRel);
            }
            else {
                int yRel = (m_drawingBracketPos == STAFFREL_basic_above)
                    ? 0
                    : -params->m_doc->GetDrawingStaffSize(staffSize);
                tupletNum->SetDrawingYRel(yRel);
            }

            // Find if there is a mix of cross-staf and non-cross-staff elements in the tuplet
            ListOfObjects descendants;
            ClassIdsComparison comparison({ CHORD, NOTE, REST });
            this->FindAllDescendantByComparison(&descendants, &comparison);

            auto it = std::find_if(descendants.begin(), descendants.end(), [](Object *object) {
                LayerElement *element = vrv_cast<LayerElement *>(object);
                if (!element) return false;
                return !element->m_crossStaff;
            });

            // Calculate relative Y for the tupletNum
            AdjustTupletNumOverlapParams adjustTupletNumOverlapParams(tupletNum);
            adjustTupletNumOverlapParams.m_drawingNumPos = m_drawingNumPos;
            adjustTupletNumOverlapParams.m_ignoreCrossStaff = false;
            adjustTupletNumOverlapParams.m_yRel = tupletNum->GetDrawingY();
            adjustTupletNumOverlapParams.m_ignoreCrossStaff = (descendants.end() != it);
            Functor adjustTupletNumOverlap(&Object::AdjustTupletNumOverlap);
            this->Process(&adjustTupletNumOverlap, &adjustTupletNumOverlapParams);

            const int yRel = adjustTupletNumOverlapParams.m_yRel - yReference;
            tupletNum->SetDrawingYRel(yRel + numVerticalMargin);
        }
    }

    return FUNCTOR_SIBLINGS;
}

int Tuplet::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    // We want the list of the ObjectListInterface to be re-generated
    this->Modify();

    m_drawingLeft = NULL;
    m_drawingRight = NULL;

    return FUNCTOR_CONTINUE;
}

int Tuplet::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetHorizontalAlignment(functorParams);

    m_drawingBracketPos = STAFFREL_basic_NONE;
    m_bracketAlignedBeam = NULL;
    m_numAlignedBeam = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
