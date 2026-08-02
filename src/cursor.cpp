/////////////////////////////////////////////////////////////////////////////
// Name:        cursor.cpp
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "cursor.h"
//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>

//----------------------------------------------------------------------------

#include "functor.h"
#include "horizontalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Cursor
//----------------------------------------------------------------------------

Cursor::Cursor() : Note()
{
    this->Reset();
}

Cursor::~Cursor() {}

void Cursor::Reset()
{
    Note::Reset();
    m_accid.Reset();

    m_accid.SetParent(this);
    m_accid.SetDrawingCueSize(true);
    m_accid.SetFunc(accidLog_FUNC_edit);

    m_position = NULL;
    m_isAccidImplicit = false;
    m_yRelPitchC = 0;

    m_restMode = false;
    m_inputMode = InputMode::PITCH_FIRST;
    m_chordMode = ChordMode::CHORD_NONE;
    m_tieMode = TieMode::TIE_NONE;

    // Default pitch and duration
    this->SetPname(PITCHNAME_c);
    this->SetOct(4);
    this->SetDur(DURATION_4);
}

void Cursor::CloneReset()
{
    Note::CloneReset();

    this->ResetCursorAlignment();
    m_position = NULL;
    m_tieMode = TieMode::TIE_NONE;
    m_container = {};

    m_accid.SetParent(this);
}

void Cursor::SetAccidImplicit(bool isAccidImplicit)
{
    m_isAccidImplicit = isAccidImplicit;
    data_ENCLOSURE enclosure = (m_isAccidImplicit) ? ENCLOSURE_paren : ENCLOSURE_NONE;
    m_accid.SetEnclose(enclosure);
}

void Cursor::SetAccid(data_ACCIDENTAL_WRITTEN accid)
{
    m_accid.SetAccid(accid);
    this->SetAccidImplicit(false);
}

void Cursor::SetAccidValue(const Accid *accid)
{
    if (!accid) return;

    if (accid->HasAccidGes()) {
        this->SetAccid(Att::AccidentalGesturalToWritten(accid->GetAccidGes()));
    }
    else if (accid->HasAccid()) {
        this->SetAccid(accid->GetAccid());
    }
    this->SetAccidImplicit(true);
}

std::pair<data_ACCIDENTAL_WRITTEN, data_ACCIDENTAL_GESTURAL> Cursor::GetAccidValue()
{
    if (!this->HasAccid()) return { ACCIDENTAL_WRITTEN_NONE, ACCIDENTAL_GESTURAL_NONE };

    data_ACCIDENTAL_WRITTEN accid = ACCIDENTAL_WRITTEN_NONE;
    data_ACCIDENTAL_GESTURAL accidGes = ACCIDENTAL_GESTURAL_NONE;
    if (this->IsAccidImplicit()) {
        if (this->GetAccid() != ACCIDENTAL_WRITTEN_n) accidGes = Att::AccidentalWrittenToGestural(this->GetAccid());
    }
    else {
        accid = this->GetAccid();
    }

    return { accid, accidGes };
}

int Cursor::GetDrawingX() const
{
    if (this->IsChordEditMode()) return m_position->GetDrawingX();

    return Note::GetDrawingX();
}

void Cursor::SetCursorAlignment(Alignment *alignment)
{
    this->SetAlignment(alignment);
    m_accid.SetAlignment(alignment);
}

void Cursor::ResetCursorAlignment()
{
    m_accid.SetAlignment(NULL);
    // m_yRelPitchC = 0;
}

void Cursor::SetRestMode(bool restMode)
{
    m_chordMode = ChordMode::CHORD_NONE;
    m_restMode = restMode;
    // if (restMode) this->SetAccid(ACCIDENTAL_WRITTEN_NONE);
}

void Cursor::SetChordMode(ChordMode chordMode)
{
    m_chordMode = chordMode;
    m_restMode = false;
    if (m_alignment) {
        AlignmentType type = (this->IsChordEditMode()) ? ALIGNMENT_CURSOR_CHORD : ALIGNMENT_CURSOR;
        m_alignment->SetType(type);
    }
}

void Cursor::SetInputMode(InputMode intpuMode)
{
    m_inputMode = intpuMode;
}

void Cursor::SetTieMode(TieMode tieMode)
{
    m_tieMode = tieMode;
    m_restMode = false;
}

bool Cursor::Veto(const std::string &attribute) const
{
    if (this->IsChordEditMode() && (attribute == "dur" || attribute == "dots")) return true;

    return false;
}

void Cursor::OnSet(const std::string &attribute)
{
    if (attribute == "dur") this->ResetAugmentDots();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Cursor::Accept(Functor &functor)
{
    return functor.VisitCursor(this);
}

FunctorCode Cursor::Accept(ConstFunctor &functor) const
{
    return functor.VisitCursor(this);
}

FunctorCode Cursor::AcceptEnd(Functor &functor)
{
    return functor.VisitCursorEnd(this);
}

FunctorCode Cursor::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitCursorEnd(this);
}

} // namespace vrv
