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

#include "editorial.h"
#include "elementpart.h"
#include "note.h"
#include "rest.h"
#include "tuplet.h"
#include "vrv.h"

namespace vrv {
    
    
//----------------------------------------------------------------------------
// BeamDrawingParams
//----------------------------------------------------------------------------

    
BeamDrawingParams::BeamDrawingParams()
{
    Reset();
}

void BeamDrawingParams::Reset()
{
    m_changingDur = false;
    m_beamHasChord = false;
    m_hasMultipleStemDir = false;
    m_cueSize = false;
    m_shortestDur = 0;
    m_stemDir = STEMDIRECTION_NONE;
}

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
    
    ClearCoords();
}

void Beam::AddChild(Object *child)
{
    if (child->Is(BEAM)) {
        assert(dynamic_cast<Beam *>(child));
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
    else if (child->Is(REST)) {
        assert(dynamic_cast<Rest *>(child));
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
    m_children.push_back(child);
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
            LayerElement *element = dynamic_cast<LayerElement*>(*iter);
            assert(element);
            // if we are at the beginning of the beam
            // and the note is cueSize
            // assume all the beam is of grace notes
            if (childList->begin() == iter) {
                if (element->IsGraceNote()) firstNoteGrace = true;
            }
            // if the first note in beam was NOT a grace
            // we have grace notes embedded in a beam
            // drop them
            if (!firstNoteGrace && element->IsGraceNote()) {
                iter = childList->erase(iter);
                continue;
            }
            // also remove notes within chords
            if (element->Is(NOTE)) {
                Note *note = dynamic_cast<Note *>(element);
                assert(note);
                if (note->IsChordTone()) {
                    iter = childList->erase(iter);
                    continue;
                }
            }
            iter++;
        }
    }

    InitCoords(childList);
}

int Beam::GetPosition(LayerElement *element)
{
    this->GetList(this);
    int position = this->GetListIndex(element);
    // Check if this is a note in the chord
    if ((position == -1) && (element->Is(NOTE))) {
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
    
    if (childList->empty()) {
        return;
    }
    
    // duration variables
    int lastDur, currentDur;
    
    m_beamElementCoords.reserve(childList->size());
    int i;
    for (i = 0; i < (int)childList->size(); i++) {
        m_beamElementCoords.push_back(new BeamElementCoord());
    }
    
    // current point to the first Note in the layed out layer
    LayerElement *current = dynamic_cast<LayerElement *>(childList->front());
    // Beam list should contain only DurationInterface objects
    assert(current->GetDurationInterface());
    
    lastDur = (current->GetDurationInterface())->GetActualDur();
    
    /******************************************************************/
    // Populate BeamElementCoord for each element in the beam
    // This could be moved to Beam::InitCoord for optimization because there should be no
    // need for redoing it everytime it is drawn.
    
    data_STEMDIRECTION currentStemDir;
    
    int elementCount = 0;
    
    ListOfObjects::iterator iter = childList->begin();
    do {
        // Beam list should contain only DurationInterface objects
        assert(current->GetDurationInterface());
        currentDur = (current->GetDurationInterface())->GetActualDur();
        
        if (current->Is(CHORD)) {
            m_drawingParams.m_beamHasChord = true;
        }
        
        // Can it happen? With rests?
        if (currentDur > DUR_4) {
            m_beamElementCoords.at(elementCount)->m_element = current;
            current->m_beamElementCoord = m_beamElementCoords.at(elementCount);
            m_beamElementCoords.at(elementCount)->m_dur = currentDur;
            
            // Look at beam breaks
            m_beamElementCoords.at(elementCount)->m_breaksec = 0;
            AttBeamsecondary *beamsecondary = dynamic_cast<AttBeamsecondary *>(current);
            if (beamsecondary && beamsecondary->HasBreaksec()) {
                if (!m_drawingParams.m_changingDur) m_drawingParams.m_changingDur = true;
                m_beamElementCoords.at(elementCount)->m_breaksec = beamsecondary->GetBreaksec();
            }
            
            // Skip rests
            if (current->Is({ NOTE, CHORD })) {
                // look at the stemDir to see if we have multiple stem Dir
                if (!m_drawingParams.m_hasMultipleStemDir) {
                    StemmedDrawingInterface *interface = current->GetStemmedDrawingInterface();
                    assert(interface);
                    Stem *stem = interface->GetDrawingStem();
                    currentStemDir = STEMDIRECTION_NONE;
                    if (stem) {
                        assert(dynamic_cast<AttStems *>(stem));
                        currentStemDir = (dynamic_cast<AttStems *>(stem))->GetStemDir();
                    }
                    if (currentStemDir != STEMDIRECTION_NONE) {
                        if ((m_drawingParams.m_stemDir != STEMDIRECTION_NONE) && (m_drawingParams.m_stemDir != currentStemDir)) {
                            m_drawingParams.m_hasMultipleStemDir = true;
                        }
                        m_drawingParams.m_stemDir = currentStemDir;
                    }
                }
            }
            // keep the shortest dur in the beam
            m_drawingParams.m_shortestDur = std::max(currentDur, m_drawingParams.m_shortestDur);
            // check if we have more than duration in the beam
            if (!m_drawingParams.m_changingDur && currentDur != lastDur) m_drawingParams.m_changingDur = true;
            lastDur = currentDur;
            
            elementCount++;
        }
        
        iter++;
        if (iter == childList->end()) {
            break;
        }
        current = dynamic_cast<LayerElement *>(*iter);
        if (current == NULL) {
            LogDebug("Error accessing element in Beam list");
            return;
        }
        
    } while (1);
    
    // elementCount must be greater than 0 here
    if (elementCount == 0) {
        LogDebug("Beam with no notes of duration > 8 detected. Exiting DrawBeam.");
        return;
    }
    
    int last = elementCount - 1;
    
    // We look only at the last note for checking if cue-sized. Somehow arbitrarily
    m_drawingParams.m_cueSize = m_beamElementCoords.at(last)->m_element->IsCueSize();

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
