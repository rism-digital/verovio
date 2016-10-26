/////////////////////////////////////////////////////////////////////////////
// Name:        note.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "note.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functorparams.h"
#include "slur.h"
#include "syl.h"
#include "tie.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Note
//----------------------------------------------------------------------------

Note::Note()
    : LayerElement("note-")
    , StemmedDrawingInterface()
    , DurationInterface()
    , PitchInterface()
    , AttAccidentalPerformed()
    , AttColor()
    , AttColoration()
    , AttGraced()
    , AttNoteLogMensural()
    , AttStems()
    , AttStemsCmn()
    , AttTiepresent()
    , AttVisibility()
{
    RegisterInterface(DurationInterface::GetAttClasses(), DurationInterface::IsInterface());
    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterAttClass(ATT_ACCIDENTALPERFORMED);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_COLORATION);
    RegisterAttClass(ATT_GRACED);
    RegisterAttClass(ATT_NOTELOGMENSURAL);
    RegisterAttClass(ATT_STEMS);
    RegisterAttClass(ATT_STEMSCMN);
    RegisterAttClass(ATT_TIEPRESENT);
    RegisterAttClass(ATT_VISIBILITY);

    m_drawingTieAttr = NULL;
    m_drawingAccid = NULL;
    m_isDrawingAccidAttr = false;

    Reset();
}

Note::~Note()
{
    // This deletes the Tie, Slur, and Accid objects if necessary
    if (m_drawingTieAttr) {
        delete m_drawingTieAttr;
    }
    // We delete it only if it is an attribute - otherwise we do not own it
    if (m_drawingAccid && m_isDrawingAccidAttr) {
        delete m_drawingAccid;
    }
}

void Note::Reset()
{
    LayerElement::Reset();
    StemmedDrawingInterface::Reset();
    DurationInterface::Reset();
    PitchInterface::Reset();
    ResetAccidentalPerformed();
    ResetColor();
    ResetColoration();
    ResetGraced();
    ResetNoteLogMensural();
    ResetStems();
    ResetStemsCmn();
    ResetTiepresent();
    ResetVisibility();

    // TO BE REMOVED
    m_embellishment = EMB_NONE;
    // tie pointers
    ResetDrawingTieAttr();
    // accid pointer
    ResetDrawingAccid();

    m_drawingStemDir = STEMDIRECTION_NONE;
    d_stemLen = 0;
    m_clusterPosition = 0;
    m_cluster = NULL;
    m_graceAlignment = NULL;

    m_playingOnset = 0.0;
    m_playingOffset = 0.0;
}

void Note::AddChild(Object *child)
{
    if (child->Is() == ACCID) {
        assert(dynamic_cast<Accid *>(child));
    }
    else if (child->Is() == SYL) {
        assert(dynamic_cast<Syl *>(child));
    }
    else if (child->Is() == VERSE) {
        assert(dynamic_cast<Verse *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

Alignment *Note::GetGraceAlignment()
{
    assert(m_graceAlignment);
    return m_graceAlignment;
}

void Note::SetGraceAlignment(Alignment *graceAlignment)
{
    assert(!m_graceAlignment && graceAlignment);
    m_graceAlignment = graceAlignment;
}

void Note::SetDrawingTieAttr()
{
    assert(!this->m_drawingTieAttr);
    if (m_drawingTieAttr) return;
    m_drawingTieAttr = new Tie();
    m_drawingTieAttr->SetStart(this);
}

void Note::ResetDrawingTieAttr()
{
    if (m_drawingTieAttr) {
        delete m_drawingTieAttr;
        m_drawingTieAttr = NULL;
    }
}

void Note::ResetDrawingAccid()
{
    if (m_drawingAccid) {
        // We delete it only if it is an attribute - otherwise we do not own it
        if (m_isDrawingAccidAttr) delete m_drawingAccid;
        m_drawingAccid = NULL;
        m_isDrawingAccidAttr = false;
    }
    // we should never have no m_drawingAccid but have the attr flag to true
    assert(!m_isDrawingAccidAttr);
}

Chord *Note::IsChordTone()
{
    return dynamic_cast<Chord *>(this->GetFirstParent(CHORD, MAX_CHORD_DEPTH));
}

int Note::GetDrawingDur()
{
    Chord *chordParent = dynamic_cast<Chord *>(this->GetFirstParent(CHORD, MAX_CHORD_DEPTH));
    if (chordParent) {
        return chordParent->GetActualDur();
    }
    else {
        return GetActualDur();
    }
}

bool Note::IsClusterExtreme() const
{
    ChordCluster *cluster = this->m_cluster;
    if (this == cluster->at(0)) return true;
    if (this == cluster->at(cluster->size() - 1))
        return true;
    else
        return false;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Note::PrepareTieAttr(FunctorParams *functorParams)
{
    PrepareTieAttrParams *params = dynamic_cast<PrepareTieAttrParams *>(functorParams);
    assert(params);

    AttTiepresent *check = this;
    // Use the parent chord if there is no @tie on the note
    if (!this->HasTie() && params->m_currentChord) {
        check = params->m_currentChord;
    }
    assert(check);

    std::vector<Note *>::iterator iter = params->m_currentNotes.begin();
    while (iter != params->m_currentNotes.end()) {
        // same octave and same pitch - this is the one!
        if ((this->GetOct() == (*iter)->GetOct()) && (this->GetPname() == (*iter)->GetPname())) {
            // right flag
            if ((check->GetTie() == TIE_m) || (check->GetTie() == TIE_t)) {
                assert((*iter)->GetDrawingTieAttr());
                (*iter)->GetDrawingTieAttr()->SetEnd(this);
            }
            else {
                LogWarning("Expected @tie median or terminal in note '%s', skipping it", this->GetUuid().c_str());
                (*iter)->ResetDrawingTieAttr();
            }
            iter = params->m_currentNotes.erase(iter);
            // we are done for this note
            break;
        }
        iter++;
    }

    if ((check->GetTie() == TIE_m) || (check->GetTie() == TIE_i)) {
        this->SetDrawingTieAttr();
        params->m_currentNotes.push_back(this);
    }

    return FUNCTOR_CONTINUE;
}

int Note::FillStaffCurrentTimeSpanning(FunctorParams *functorParams)
{
    // Pass it to the pseudo functor of the interface
    if (this->m_drawingTieAttr) {
        return this->m_drawingTieAttr->FillStaffCurrentTimeSpanning(functorParams);
    }
    return FUNCTOR_CONTINUE;
}

int Note::PrepareLyrics(FunctorParams *functorParams)
{
    PrepareLyricsParams *params = dynamic_cast<PrepareLyricsParams *>(functorParams);
    assert(params);

    params->m_lastButOneNote = params->m_lastNote;
    params->m_lastNote = this;

    return FUNCTOR_CONTINUE;
}

int Note::PreparePointersByLayer(FunctorParams *functorParams)
{
    PreparePointersByLayerParams *params = dynamic_cast<PreparePointersByLayerParams *>(functorParams);
    assert(params);

    this->ResetDrawingAccid();
    // LogDebug("PreparePointersByLayer: accid=%d ACC_EXP_NONE=%d", this->GetAccid(), ACCIDENTAL_EXPLICIT_NONE);
    if (this->GetAccid() != ACCIDENTAL_EXPLICIT_NONE) {
        this->m_isDrawingAccidAttr = true;
        this->m_drawingAccid = new Accid();
        this->m_drawingAccid->SetOloc(this->GetOct());
        this->m_drawingAccid->SetPloc(this->GetPname());
        this->m_drawingAccid->SetAccid(this->GetAccid());
        // We need to set the drawing cue size since there will be no access to the note
        this->m_drawingAccid->m_drawingCueSize = this->HasGrace();
    }

    params->m_currentNote = this;

    return FUNCTOR_CONTINUE;
}

int Note::ResetDrawing(FunctorParams *functorParams)
{
    this->ResetDrawingTieAttr();
    return FUNCTOR_CONTINUE;
};

} // namespace vrv
