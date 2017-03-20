/////////////////////////////////////////////////////////////////////////////
// Name:        elementpart.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "elementpart.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "chord.h"
#include "doc.h"
#include "functorparams.h"
#include "layer.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Flag
//----------------------------------------------------------------------------

Flag::Flag() : LayerElement("flag-")
{

    Reset();
}

Flag::~Flag()
{
}

void Flag::Reset()
{
    LayerElement::Reset();
}

//----------------------------------------------------------------------------
// NoteHead
//----------------------------------------------------------------------------

NoteHead::NoteHead() : LayerElement("notehead-")
{

    Reset();
}

NoteHead::~NoteHead()
{
}

void NoteHead::Reset()
{
    LayerElement::Reset();
}

//----------------------------------------------------------------------------
// Stem
//----------------------------------------------------------------------------

Stem::Stem() : LayerElement("stem-"), AttStems(), AttStemsCmn()
{
    RegisterAttClass(ATT_STEMS);
    RegisterAttClass(ATT_STEMSCMN);

    Reset();
}

Stem::~Stem()
{
}

void Stem::Reset()
{
    LayerElement::Reset();
    ResetStems();
    ResetStemsCmn();

    m_drawingStemDir = STEMDIRECTION_NONE;
    m_drawingStemLen = 0;
}

void Stem::AddChild(Object *child)
{
    if (child->Is(FLAG)) {
        assert(dynamic_cast<Flag *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Flag::CalcDrawingStemDir(FunctorParams *functorParams)
{
    CalcDrawingStemDirParams *params = dynamic_cast<CalcDrawingStemDirParams *>(functorParams);
    assert(params);

    if (!params->m_currentStaff || !params->m_currentLayer) {
        LogDebug("Staff or layer are NULL, something went wrong");
        return FUNCTOR_CONTINUE;
    }

    if (!params->m_currentChord && !params->m_currentNote) {
        LogDebug("Both chord or note are NULL, something went wrong");
        return FUNCTOR_CONTINUE;
    }

    // At least one of the two should be null;
    assert(!params->m_currentChord || !params->m_currentNote);

    int drawingDur
        = (params->m_currentChord) ? params->m_currentChord->GetActualDur() : params->m_currentNote->GetActualDur();

    if (drawingDur < DUR_8) return FUNCTOR_CONTINUE;

    Stem *stem = dynamic_cast<Stem *>(this->GetFirstParent(STEM));
    assert(stem);

    int staffSize = params->m_currentStaff->m_drawingStaffSize;
    bool drawingCueSize = this->IsCueSize();

    /************ Set the direction ************/

    this->SetDrawingYRel(-stem->GetDrawingStemLen());

    /************ Set the direction ************/

    // Eventually we should change this to distinct SMuFL glyphs
    int flagStemHeight = params->m_doc->GetDrawingUnit(staffSize);
    if (drawingCueSize) {
        flagStemHeight = params->m_doc->GetGraceSize(flagStemHeight);
    }

    this->m_drawingNbFlags = drawingDur - DUR_4;
    // Trick for shortening the stem with DUR_8
    int totalFlagStemHeight = flagStemHeight * (this->m_drawingNbFlags * 2 - 3);

    if (stem->GetDrawingStemDir() == STEMDIRECTION_up)
        stem->SetDrawingStemLen(stem->GetDrawingStemLen() - totalFlagStemHeight);
    else
        stem->SetDrawingStemLen(stem->GetDrawingStemLen() + totalFlagStemHeight);

    return FUNCTOR_CONTINUE;
}

int Flag::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    m_drawingNbFlags = 0;

    return FUNCTOR_CONTINUE;
};

int Stem::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    m_drawingStemDir = STEMDIRECTION_NONE;
    m_drawingStemLen = 0;

    return FUNCTOR_CONTINUE;
};

} // namespace vrv
