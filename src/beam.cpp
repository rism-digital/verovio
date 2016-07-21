/////////////////////////////////////////////////////////////////////////////
// Name:        beam.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "beam.h"

//----------------------------------------------------------------------------

#include "assert.h"

//----------------------------------------------------------------------------

#include "note.h"

namespace vrv {

//----------------------------------------------------------------------------
// Beam
//----------------------------------------------------------------------------

Beam::Beam() : LayerElement("beam-"), ObjectListInterface()
{
    Reset();
}

Beam::~Beam()
{
    ClearCoords();
}

void Beam::Reset()
{
    LayerElement::Reset();
}

void Beam::AddLayerElement(LayerElement *element)
{

    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}

void Beam::FilterList(ListOfObjects *childList)
{
    bool firstNoteGrace = false;
    // We want to keep only notes and rests
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->IsLayerElement()) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc)
            iter = childList->erase(iter);
            continue;
        }
        if (!(*iter)->HasInterface(INTERFACE_DURATION)) {
            // remove anything that has not a DurationInterface
            iter = childList->erase(iter);
            continue;
        }
        else {
            // Drop notes that are signaled as grace notes

            if ((*iter)->Is() == NOTE) {
                Note *n = dynamic_cast<Note *>(*iter);
                assert(n);
                // if we are at the beginning of the beam
                // and the note is cueSize
                // assume all the beam is of grace notes
                if (childList->begin() == iter) {
                    if (n->HasGrace()) firstNoteGrace = true;
                }

                // if the first note in beam was NOT a grace
                // we have grace notes embedded in a beam
                // drop them
                if (!firstNoteGrace && n->HasGrace() == true) iter = childList->erase(iter);
                // also remove notes within chords
                else if (n->IsChordTone())
                    iter = childList->erase(iter);
                else
                    iter++;
            }
            else {
                // if it is a Rest, do not drop
                iter++;
            }
        }
    }

    InitCoords(childList);
}

int Beam::GetPosition(LayerElement *element)
{
    this->GetList(this);
    int position = this->GetListIndex(element);
    // Check if this is a note in the chord
    if ((position == -1) && (element->Is() == NOTE)) {
        Note *note = dynamic_cast<Note *>(element);
        assert(note);
        Chord *chord = note->IsChordTone();
        if (chord) position = this->GetListIndex(chord);
    }
    return position;
}

bool Beam::IsFirstInBeam(LayerElement *element)
{
    this->GetList(this);
    int position = this->GetPosition(element);
    // This method should be called only if the note is part of a beam
    assert(position != -1);
    // this is the first one
    if (position == 0) return true;
    return false;
}

bool Beam::IsLastInBeam(LayerElement *element)
{
    int size = (int)this->GetList(this)->size();
    int position = this->GetPosition(element);
    // This method should be called only if the note is part of a beam
    assert(position != -1);
    // this is the last one
    if (position == (size - 1)) return true;
    return false;
}

void Beam::InitCoords(ListOfObjects *childList)
{
    ClearCoords();
    m_beamElementCoords.reserve(childList->size());
    int i;
    for (i = 0; i < (int)childList->size(); i++) {
        m_beamElementCoords.push_back(new BeamElementCoord());
    }
}

void Beam::ClearCoords()
{
    ArrayOfBeamElementCoords::iterator iter;
    for (iter = m_beamElementCoords.begin(); iter != m_beamElementCoords.end(); ++iter) {
        delete *iter;
    }
    m_beamElementCoords.clear();
}

//----------------------------------------------------------------------------
// BeamElementCoord
//----------------------------------------------------------------------------

BeamElementCoord::~BeamElementCoord()
{
    if (m_element) m_element->m_beamElementCoord = NULL;
}

} // namespace vrv
