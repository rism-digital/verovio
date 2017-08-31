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
#include "editorial.h"
#include "elementpart.h"
#include "note.h"
#include "rest.h"
#include "space.h"
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

Tuplet::~Tuplet()
{
}

void Tuplet::Reset()
{
    LayerElement::Reset();
    ResetColor();
    ResetDurationRatio();
    ResetNumberPlacement();
    ResetTupletVis();
}

void Tuplet::AddChild(Object *child)
{
    if (child->Is(BEAM)) {
        assert(dynamic_cast<Beam *>(child));
    }
    else if (child->Is(BRACKET)) {
        assert(dynamic_cast<Bracket *>(child));
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
    else if (child->Is(NUM)) {
        assert(dynamic_cast<Num *>(child));
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
    if (child->Is({ BRACKET, NUM }))
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
            iter++;
        }
    }
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------
    
int Tuplet::PrepareLayerElementParts(FunctorParams *functorParams)
{
    Bracket *currentBracket = dynamic_cast<Bracket *>(this->FindChildByType(BRACKET, 1));
    Num *currentNum = dynamic_cast<Num *>(this->FindChildByType(NUM, 1));

    if (!this->HasBracketVisible() || (this->GetBracketVisible() == BOOLEAN_true)) {
        if (!currentBracket) {
            currentBracket = new Bracket();
            this->AddChild(currentBracket);
        }
        //currentStem->AttGraced::operator=(*this);
        //currentStem->AttStems::operator=(*this);
        //currentStem->AttStemsCmn::operator=(*this);
    }
    // This will happen only if the @bracket.visible value has changed
    else if (currentBracket) {
        if (this->DeleteChild(currentBracket)) {
            currentBracket = NULL;
        }
    }

    if (!this->HasNumVisible() || (this->GetNumVisible() == BOOLEAN_true)) {
        if (!currentNum) {
            currentNum = new Num();
            this->AddChild(currentNum);
        }
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

    return FUNCTOR_CONTINUE;
};

} // namespace vrv
