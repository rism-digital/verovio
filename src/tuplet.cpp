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
#include "chord.h"
#include "doc.h"
#include "editorial.h"
#include "elementpart.h"
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
    m_alignedBeam = NULL;
}

void Tuplet::AddChild(Object *child)
{
    if (child->Is(BEAM)) {
        assert(dynamic_cast<Beam *>(child));
    }
    else if (child->Is(TUPLET_BRACKET)) {
        assert(dynamic_cast<TupletBracket *>(child));
    }
    else if (child->Is(CHORD)) {
        assert(dynamic_cast<Chord *>(child));
    }
    else if (child->Is(CLEF)) {
        assert(dynamic_cast<Clef *>(child));
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
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    
    // Num and bracket are always added by PrepareLayerElementParts (for now) and we want them to be in the front
    // for the drawing order in the SVG output
    if (child->Is({ TUPLET_BRACKET, TUPLET_NUM }))
        m_children.insert(m_children.begin(), child);
    else
        m_children.push_back(child);
    
    Modify();
}

void Tuplet::FilterList(ListOfObjects *childList)
{
    // We want to keep only notes and rests
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->IsLayerElement() || !(*iter)->HasInterface(INTERFACE_DURATION)) {
            iter = childList->erase(iter);
        }
        else {
            ++iter;
        }
    }
}

data_STAFFREL_basic Tuplet::CalcDrawingBracketPos()
{
    if (this->HasBracketPlace()) {
        return this->GetBracketPlace();
    }
    
    const ListOfObjects *tupletChildren = this->GetList(this);
    
    // There are unbeamed notes of two different beams
    // treat all the notes as unbeamed
    int ups = 0, downs = 0; // quantity of up- and down-stems
    
    // The first step is to calculate all the stem directions
    // cycle into the elements and count the up and down dirs
    ListOfObjects::const_iterator iter = tupletChildren->begin();
    while (iter != tupletChildren->end()) {
        if ((*iter)->Is(CHORD)) {
            Chord *currentChord = dynamic_cast<Chord *>(*iter);
            assert(currentChord);
            if (currentChord->GetDrawingStemDir() == STEMDIRECTION_up) {
                ups++;
            }
            else {
                downs++;
            }
        }
        else if ((*iter)->Is(NOTE)){
            Note *currentNote = dynamic_cast<Note *>(*iter);
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
    data_STAFFREL_basic position = ups > downs ? STAFFREL_basic_above : STAFFREL_basic_below;
    
    return position;
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
        Chord *chord = dynamic_cast<Chord *>(m_drawingLeft);
        assert(chord);
        XRelLeft = chord->GetXMin() - m_drawingLeft->GetDrawingX() ;
    }
    
    XRelRight = 0;
    
    if (m_drawingRight->Is(NOTE)) {
        XRelRight += (2 * m_drawingRight->GetDrawingRadius(doc));
    }
    else if (m_drawingRight->Is(REST)) {
        XRelRight += m_drawingRight->GetSelfX2();
    }
    else if (m_drawingRight->Is(CHORD)) {
        Chord *chord = dynamic_cast<Chord *>(m_drawingRight);
        assert(chord);
        XRelRight = chord->GetXMax() - chord->GetDrawingX() + (2 * chord->GetDrawingRadius(doc));
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------
    
int Tuplet::PrepareLayerElementParts(FunctorParams *functorParams)
{
    TupletBracket *currentBracket = dynamic_cast<TupletBracket *>(this->FindChildByType(TUPLET_BRACKET, 1));
    TupletNum *currentNum = dynamic_cast<TupletNum *>(this->FindChildByType(TUPLET_NUM, 1));

    if (!currentBracket) {
        currentBracket = new TupletBracket();
        this->AddChild(currentBracket);
    }
    currentBracket->AttTupletVis::operator=(*this);
    
    if (!currentNum) {
        currentNum = new TupletNum();
        this->AddChild(currentNum);
    }
    currentNum->AttNumberPlacement::operator=(*this);
    currentNum->AttTupletVis::operator=(*this);
    
    /*
    if (!this->HasBracketVisible() || (this->GetBracketVisible() == BOOLEAN_true)) {
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

    if (!this->HasNumVisible() || (this->GetNumVisible() == BOOLEAN_true)) {
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
    */

    /************ Prepare the drawing cue size ************/

    Functor prepareDrawingCueSize(&Object::PrepareDrawingCueSize);
    this->Process(&prepareDrawingCueSize, NULL);
    
    /*********** Get the left and right element ***********/
    
    ClassIdsComparison comparison({CHORD, NOTE, REST});
    m_drawingLeft = dynamic_cast<LayerElement *>(this->FindChildByComparison(&comparison));
    m_drawingRight = dynamic_cast<LayerElement *>(this->FindChildByComparison(&comparison, UNLIMITED_DEPTH, BACKWARD));

    return FUNCTOR_CONTINUE;
};

int Tuplet::AdjustTupletsX(FunctorParams *functorParams)
{
    FunctorDocParams *params = dynamic_cast<FunctorDocParams *>(functorParams);
    assert(params);
    
    // Nothing to do if the bracket and the num are not visible
    if ((this->GetBracketVisible() == BOOLEAN_false) && (this->GetNumVisible() == BOOLEAN_false)) {
        return FUNCTOR_SIBLINGS;
    }
    
    // Nothing we can to the pointers to the left and right are not set
    if (!this->GetDrawingLeft() || !this->GetDrawingRight()) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);
    int staffSize = staff->m_drawingStaffSize;
    
    m_drawingBracketPos = this->GetDrawingBracketPos();
    assert(m_drawingBracketPos != STAFFREL_basic_NONE);
    
    // Carefull: this will not work if the tuplet has editorial markup (one child) and then notes + one beam
    Beam *beamParent = dynamic_cast<Beam *>(this->GetFirstParent(BEAM, MAX_BEAM_DEPTH));
    // Are we contained in a beam?
    if (beamParent) {
        // is only the tuplet beamed and no other tuplet contained?
        if ((beamParent->GetChildCount() == 1) && (this->GetChildCount(TUPLET) == 0)) {
            m_alignedBeam = beamParent;
        }
    }
    Beam *beamChild = dynamic_cast<Beam *>(this->FindChildByType(BEAM));
    // Do we contain a beam?
    if (beamChild) {
        if ((this->GetChildCount(NOTE) == 0) && (this->GetChildCount(CHORD) == 0) && (this->GetChildCount(BEAM) == 1)) {
            m_alignedBeam = beamChild;
        }
    }
    
    // Cancel alignment with the beam if position and stemdirection are not concordant
    if (m_alignedBeam && (m_alignedBeam->m_drawingParams.m_stemDir == STEMDIRECTION_up) && (m_drawingBracketPos == STAFFREL_basic_below)) {
        m_alignedBeam = NULL;
    }
    else if (m_alignedBeam && ((m_alignedBeam->m_drawingParams.m_stemDir == STEMDIRECTION_down) && (m_drawingBracketPos == STAFFREL_basic_above))) {
        m_alignedBeam = NULL;
    }
    
    int XRelLeft;
    int XRelRight;
    this->GetDrawingLeftRightXRel(XRelLeft, XRelRight, params->m_doc);

    TupletBracket *tupletBracket = dynamic_cast<TupletBracket *>(this->FindChildByType(TUPLET_BRACKET));
    if (tupletBracket && (this->GetBracketVisible() != BOOLEAN_false)) {
        tupletBracket->SetDrawingXRelLeft(XRelLeft);
        tupletBracket->SetDrawingXRelRight(XRelRight);
    }
  
    /*
    TupletNum *tupletNum = dynamic_cast<TupletNum *>(this->FindChildByType(TUPLET_NUM));
    if (tupletNum && (this->GetNumVisible() != BOOLEAN_false)) {
        // We have no bracket or the num is on its opposite side
        if (!tupletBracket || (tupletNum->HasNumPlace() && tupletNum->GetNumPlace() != position)) {
            
        }
        else {
            int xLeft = this->GetDrawingLeft()->GetDrawingX() + XRelLeft;
            int xRight = this->GetDrawingRight()->GetDrawingX() + XRelRight;
            int width = xRight - xLeft;
            int xRelMid = (width / 2) + XRelLeft;
            tupletNum->SetDrawingXRel(xRelMid);
            int yRelMid = (tupletBracket->GetDrawingYRelLeft() + tupletBracket->GetDrawingYRelRight()) / 2;
            tupletNum->SetDrawingYRel(yRelMid);
        }
    
    }
    */
    
    return FUNCTOR_SIBLINGS;
}
    
int Tuplet::AdjustTupletsY(FunctorParams *functorParams)
{
    FunctorDocParams *params = dynamic_cast<FunctorDocParams *>(functorParams);
    assert(params);
    
    // Nothing to do if the bracket and the num are not visible
    if ((this->GetBracketVisible() == BOOLEAN_false) && (this->GetNumVisible() == BOOLEAN_false)) {
        return FUNCTOR_SIBLINGS;
    }
    
    if (!this->GetDrawingLeft() || !this->GetDrawingRight()) {
        return FUNCTOR_SIBLINGS;
    }

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    assert(staff);
    int staffSize = staff->m_drawingStaffSize;
    
    data_STAFFREL_basic position = this->GetDrawingBracketPos();
    //assert(position != STAFFREL_basic_NONE);
    position = STAFFREL_basic_above;
    
    /*
    Beam *beam = this->IsSingleBeamTuplet();
    bool alignWithBeam = false;
    if (beam && ((beam->m_drawingParams.m_stemDir == STEMDIRECTION_up) && (position == STAFFREL_basic_above))) {
        alignWithBeam = true;
    }
    else if (beam && ((beam->m_drawingParams.m_stemDir == STEMDIRECTION_down) && (position == STAFFREL_basic_below))) {
        alignWithBeam = true;
    }
             
    //int XRelLeft = tupletBracket->GetDrawingXRelLeft();
    //int XRelRight = tupletBracket->GetDrawingYRelLeft();
    //this->GetDrawingLeftRightXRel(XRelLeft, XRelRight, params->m_doc);

    TupletBracket *tupletBracket = dynamic_cast<TupletBracket *>(this->FindChildByType(TUPLET_BRACKET));
    if (tupletBracket && (this->GetBracketVisible() != BOOLEAN_false)) {
        int XRelLeft = tupletBracket->GetDrawingXRelLeft();
        int XRelRight = tupletBracket->GetDrawingYRelLeft();
        
        //tupletBracket->SetDrawingXRelLeft(XRelLeft);
        //tupletBracket->SetDrawingXRelRight(XRelRight);
        
        int verticalLine = params->m_doc->GetDrawingUnit(staffSize);
        int verticalMargin = 2 * verticalLine;
        verticalMargin *= (position == STAFFREL_basic_above) ? 1 : -1;
        int yReference = staff->GetDrawingY();
        
        if (alignWithBeam) {
            // Calculate the x and y points aligning with the beam
            int xLeft = this->GetDrawingLeft()->GetDrawingX() + XRelLeft;
            int yLeft = beam->m_drawingParams.m_startingY + beam->m_drawingParams.m_beamSlope *
                (xLeft - beam->m_drawingParams.m_startingX);
            int xRight = this->GetDrawingRight()->GetDrawingX() + XRelRight;
            int yRight = beam->m_drawingParams.m_startingY + beam->m_drawingParams.m_beamSlope *
                (xRight - beam->m_drawingParams.m_startingX);
            
            // Check for possible articulations
            ArrayOfObjects artics;
            ClassIdsComparison comparison({ARTIC, ARTIC_PART});
            this->FindAllChildByComparison(&artics, &comparison);
            
            int articPadding = 0;
            for (auto &artic : artics) {
                if (!artic->HasSelfBB()) continue;
                if (position == STAFFREL_basic_above) {
                    // Left point when slope is going up and right when going down
                    int relevantX = (beam->m_drawingParams.m_beamSlope > 0) ? artic->GetSelfLeft() : artic->GetSelfRight();
                    int currentYRel = beam->m_drawingParams.m_startingY + beam->m_drawingParams.m_beamSlope *
                        (relevantX - beam->m_drawingParams.m_startingX);
                    int articYRel = artic->GetSelfTop();
                    articPadding = std::min(currentYRel - articYRel, articPadding);
                }
                else {
                    // Right point when slope is going up and left when going down
                    int relevantX = (beam->m_drawingParams.m_beamSlope > 0) ? artic->GetSelfRight() : artic->GetSelfLeft();
                    int currentYRel = beam->m_drawingParams.m_startingY + beam->m_drawingParams.m_beamSlope *
                        (relevantX - beam->m_drawingParams.m_startingX);
                    int articYRel = artic->GetSelfBottom();
                    articPadding = std::max(currentYRel - articYRel, articPadding);
                }
            }
            
            tupletBracket->SetDrawingYRelLeft(yLeft - yReference + verticalMargin - articPadding);
            tupletBracket->SetDrawingYRelRight(yRight - yReference + verticalMargin - articPadding);
        }
        else {
            // Default position is above or below the staff
            int yRel = (position == STAFFREL_basic_above) ? 0 : -params->m_doc->GetDrawingStaffSize(staffSize);

            // Check for overlap with content
            // Possible issue with beam above the tuplet - not sure this will be noticable
            ArrayOfObjects descendants;
            ClassIdsComparison comparison({ARTIC, ARTIC_PART, ACCID, BEAM, DOT, FLAG, NOTE, REST, STEM});
            this->FindAllChildByComparison(&descendants, &comparison);
            
            for (auto &descendant : descendants) {
                if (!descendant->HasSelfBB()) continue;
                if (position == STAFFREL_basic_above) {
                    int dist = descendant->GetSelfTop() - yReference;
                    if (yRel < dist) yRel = dist;
                }
                else {
                    int dist = descendant->GetSelfBottom() - yReference;
                    if (yRel > dist) yRel = dist;
                }
            }
            
            tupletBracket->SetDrawingYRelLeft(yRel + verticalMargin);
            tupletBracket->SetDrawingYRelRight(yRel + verticalMargin);
        }
    }
  
    TupletNum *tupletNum = dynamic_cast<TupletNum *>(this->FindChildByType(TUPLET_NUM));
    if (tupletNum && (this->GetNumVisible() != BOOLEAN_false)) {
        // We have no bracket or the num is on its opposite side
        if (!tupletBracket || (tupletNum->HasNumPlace() && tupletNum->GetNumPlace() != position)) {
            
        }
        else {
            int xLeft = this->GetDrawingLeft()->GetDrawingX() + XRelLeft;
            int xRight = this->GetDrawingRight()->GetDrawingX() + XRelRight;
            int width = xRight - xLeft;
            int xRelMid = (width / 2) + XRelLeft;
            tupletNum->SetDrawingXRel(xRelMid);
            int yRelMid = (tupletBracket->GetDrawingYRelLeft() + tupletBracket->GetDrawingYRelRight()) / 2;
            tupletNum->SetDrawingYRel(yRelMid);
        }
    
    }
    */
    
    return FUNCTOR_SIBLINGS;
}

int Tuplet::ResetDrawing(FunctorParams *functorParams)
{
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
    m_alignedBeam = NULL;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
