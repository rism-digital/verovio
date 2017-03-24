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
#include "smufl.h"
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
    
wchar_t Flag::GetSmuflCode(data_STEMDIRECTION stemDir)
{
    if (stemDir == STEMDIRECTION_up) {
        switch (m_drawingNbFlags) {
            case 1: return SMUFL_E240_flag8thUp;
            case 2: return SMUFL_E242_flag16thUp;
            case 3: return SMUFL_E244_flag32ndUp;
            case 4: return SMUFL_E246_flag64thUp;
            case 5: return SMUFL_E248_flag128thUp;
            case 6: return SMUFL_E24A_flag256thUp;
            default: return 0; break;
        }
    }
    else {
        switch (m_drawingNbFlags) {
            case 1: return SMUFL_E241_flag8thDown;
            case 2: return SMUFL_E243_flag16thDown;
            case 3: return SMUFL_E245_flag32ndDown;
            case 4: return SMUFL_E247_flag64thDown;
            case 5: return SMUFL_E249_flag128thDown;
            case 6: return SMUFL_E24B_flag256thDown;
            default: return 0; break;
        }
    }
}
    
Point Flag::GetStemUpSE(Doc *doc, int staffSize, bool graceSize, wchar_t &code)
{
    code = this->GetSmuflCode(STEMDIRECTION_up);
    
    int h = doc->GetGlyphHeight(code, staffSize, graceSize);
    return Point(0, h + doc->GetGlyphDescender(code, staffSize, graceSize));
}

Point Flag::GetStemDownNW(Doc *doc, int staffSize, bool graceSize, wchar_t &code)
{
    code = this->GetSmuflCode(STEMDIRECTION_down);

    return Point(0, doc->GetGlyphDescender(code, staffSize, graceSize));
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

int Flag::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = dynamic_cast<CalcStemParams *>(functorParams);
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
    
    this->m_drawingNbFlags = drawingDur - DUR_4;

    /************ Set the direction ************/

    this->SetDrawingYRel(-stem->GetDrawingStemLen());

    /************ Adjust the if necessary ************/
    
    // Do not adjust the length if given in the encoding
    if (stem->HasStemLen()) return FUNCTOR_CONTINUE;

    Point stemEnd;
    wchar_t flagCode = 0;
    if (stem->GetDrawingStemDir() == STEMDIRECTION_up)
        stemEnd = this->GetStemUpSE(params->m_doc, staffSize, drawingCueSize, flagCode);
    else
        stemEnd = this->GetStemDownNW(params->m_doc, staffSize, drawingCueSize, flagCode);
    
    // Trick for shortening the stem with DUR_8
    int totalFlagStemHeight = stemEnd.y;

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
