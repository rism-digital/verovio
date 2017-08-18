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
// Dots
//----------------------------------------------------------------------------

Dots::Dots() : LayerElement("dots-"), vrv::AttAugmentDots()
{
    RegisterAttClass(ATT_AUGMENTDOTS);

    Reset();
}

Dots::~Dots()
{
}

void Dots::Reset()
{
    LayerElement::Reset();
    ResetAugmentDots();

    m_dotLocsByStaff.clear();
}

std::list<int> *Dots::GetDotLocsForStaff(Staff *staff)
{
    return &m_dotLocsByStaff[staff];
}

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

    m_drawingNbFlags = 0;
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
// Stem
//----------------------------------------------------------------------------

Stem::Stem() : LayerElement("stem-"), AttGraced(), AttStems(), AttStemsCmn()
{
    RegisterAttClass(ATT_GRACED);
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
    ResetGraced();
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

int Dots::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    m_dotLocsByStaff.clear();

    return FUNCTOR_CONTINUE;
};

int Dots::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);

    m_dotLocsByStaff.clear();

    return FUNCTOR_CONTINUE;
}

int Flag::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    m_drawingNbFlags = 0;

    return FUNCTOR_CONTINUE;
};

int Stem::CalcStem(FunctorParams *functorParams)
{
    CalcStemParams *params = dynamic_cast<CalcStemParams *>(functorParams);
    assert(params);

    assert(params->m_staff);
    assert(params->m_layer);
    assert(params->m_interface);

    int staffSize = params->m_staff->m_drawingStaffSize;
    bool drawingCueSize = this->GetDrawingCueSize();

    /************ Set the position, the length and adjust to the note head ************/

    int baseStem = 0;
    // Use the given one if any
    if (this->HasStemLen()) {
        baseStem = this->GetStemLen() * -params->m_doc->GetDrawingUnit(staffSize);
    }
    else {
        baseStem = -params->m_doc->GetDrawingUnit(staffSize) * STANDARD_STEMLENGTH;
        if (drawingCueSize) baseStem = params->m_doc->GetCueSize(baseStem);
    }
    // Even if a stem length is given we add the length of the chord content (however only if not 0)
    // Also, the given stem length is understood as being measured from the center of the note.
    // This means that it will be adjusted according to the note head (see below
    if (!this->HasStemLen() || (this->GetStemLen() != 0)) {
        baseStem += params->m_chordStemLength;

        if (this->GetDrawingStemDir() == STEMDIRECTION_up) {
            Point p = params->m_interface->GetStemUpSE(params->m_doc, staffSize, drawingCueSize);
            baseStem += p.y;
            this->SetDrawingYRel(this->GetDrawingYRel() + p.y);
            this->SetDrawingXRel(p.x);
            this->SetDrawingStemLen(baseStem);
        }
        else {
            Point p = params->m_interface->GetStemDownNW(params->m_doc, staffSize, drawingCueSize);
            baseStem -= p.y;
            this->SetDrawingYRel(this->GetDrawingYRel() + p.y);
            this->SetDrawingXRel(p.x);
            this->SetDrawingStemLen(-baseStem);
        }
    }

    /************ Set the flag (if necessary) and adjust the length ************/

    Flag *flag = NULL;

    // SMUFL flags cover some additional stem length from the 32th only
    if (params->m_dur > DUR_4) {
        flag = dynamic_cast<Flag *>(this->FindChildByType(FLAG));
        assert(flag);
        flag->m_drawingNbFlags = params->m_dur - DUR_4;
        flag->SetDrawingYRel(-this->GetDrawingStemLen());
    }

    // Do not adjust the length if given in the encoding - however, the stem will be extend with the SMuFL
    // extension from 32th - this can be improved
    if (this->HasStemLen()) {
        if ((this->GetStemLen() == 0) && flag) flag->m_drawingNbFlags = 0;
        return FUNCTOR_CONTINUE;
    }

    // Do not adjust the length of grace notes - this is debatable and should probably become as styling option
    if (params->m_isGraceNote) return FUNCTOR_CONTINUE;

    int flagHeight = 0;

    // SMUFL flags cover some additional stem length from the 32th only
    if (params->m_dur > DUR_16) {
        assert(flag);
        Point stemEnd;
        wchar_t flagCode = 0;
        if (this->GetDrawingStemDir() == STEMDIRECTION_up)
            stemEnd = flag->GetStemUpSE(params->m_doc, staffSize, drawingCueSize, flagCode);
        else
            stemEnd = flag->GetStemDownNW(params->m_doc, staffSize, drawingCueSize, flagCode);
        // Trick for shortening the stem with DUR_8
        flagHeight = stemEnd.y;
    }

    int endY = this->GetDrawingY() - this->GetDrawingStemLen() + flagHeight;
    bool adjust = false;
    if ((this->GetDrawingStemDir() == STEMDIRECTION_up) && (endY < params->m_verticalCenter))
        adjust = true;
    else if ((this->GetDrawingStemDir() == STEMDIRECTION_down) && (endY > params->m_verticalCenter))
        adjust = true;

    if (adjust) {
        this->SetDrawingStemLen(this->GetDrawingStemLen() + (endY - params->m_verticalCenter));
        if (flag) flag->SetDrawingYRel(-this->GetDrawingStemLen());
    }

    return FUNCTOR_CONTINUE;
}

int Stem::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    m_drawingStemDir = STEMDIRECTION_NONE;
    m_drawingStemLen = 0;

    return FUNCTOR_CONTINUE;
};

} // namespace vrv
