/////////////////////////////////////////////////////////////////////////////
// Name:        ligature.cpp
// Author:      Don Byrd
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ligature.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "doc.h"
#include "editorial.h"
#include "functorparams.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Ligature
//----------------------------------------------------------------------------

Ligature::Ligature() : LayerElement("ligature-"), ObjectListInterface(), AttLigatureLog()
{
    RegisterAttClass(ATT_LIGATURELOG);

    Reset();
}

Ligature::~Ligature()
{
    ClearClusters();
}

void Ligature::Reset()
{
    LayerElement::Reset();
    ResetLigatureLog();

    ClearClusters();
}

void Ligature::ClearClusters() {}

void Ligature::AddChild(Object *child)
{
    if (child->Is(NOTE)) {
        assert(dynamic_cast<LayerElement *>(child));
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

void Ligature::FilterList(ArrayOfObjects *childList)
{
    // Retain only note children of ligatures
    ArrayOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->IsLayerElement()) {
            // remove anything that is not an LayerElement
            iter = childList->erase(iter);
            continue;
        }
        LayerElement *currentElement = dynamic_cast<LayerElement *>(*iter);
        assert(currentElement);
        if (!currentElement->HasInterface(INTERFACE_DURATION)) {
            iter = childList->erase(iter);
        }
        else {
            Note *n = dynamic_cast<Note *>(currentElement);

            if (n) {
                ++iter;
            }
            else {
                // if it is not a note, drop it
                iter = childList->erase(iter);
            }
        }
    }

    iter = childList->begin();

    this->ClearClusters();
}

int Ligature::GetDrawingNoteShape(Note *note)
{
    assert(note);
    int position = this->GetListIndex(note);
    if (position == -1) return -1;

    // no check because m_drawingShapes will have been filled by then
    return m_drawingShapes.at(position);
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Ligature::CalcLigatureNotePos(FunctorParams *functorParams)
{
    FunctorDocParams *params = dynamic_cast<FunctorDocParams *>(functorParams);
    assert(params);

    m_drawingShapes.clear();

    Note *lastNote = dynamic_cast<Note *>(this->GetList(this)->back());
    Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    const ArrayOfObjects *notes = this->GetList(this);
    assert(notes);
    Note *previousNote = NULL;
    int n1 = 0;
    int n2 = 1;

    bool oblique = false;
    if ((notes->size() == 2) && this->GetForm() == LIGATUREFORM_obliqua) oblique = true;

    // For better clarify, we loop withing the Ligature::CalcLigatureNotePos instead of
    // implementing Note::CalcLigatureNotePos.

    for (auto &iter : *notes) {

        Note *note = dynamic_cast<Note *>(iter);
        assert(note);

        m_drawingShapes.push_back(LIGATURE_DEFAULT);

        if (!previousNote) {
            previousNote = note;
            continue;
        }

        // Look at the @lig attribute on the previous note
        if (previousNote->GetLig() == noteAnlMensural_LIG_obliqua) oblique = true;
        int dur1 = previousNote->GetActualDur();
        int dur2 = note->GetActualDur();
        // Same treatment for maximas and longa execpt for positionning, which is done above
        if (dur1 == DUR_MX) dur1 = DUR_LG;
        if (dur2 == DUR_MX) dur2 = DUR_LG;

        int diatonicStep = note->GetDiatonicPitch() - previousNote->GetDiatonicPitch();
        bool up = (diatonicStep > 0);
        bool isLastNote = (note == lastNote);

        // L - L
        if ((dur1 == DUR_LG) && (dur2 == DUR_LG)) {
            if (up) {
                m_drawingShapes.at(n1) = LIGATURE_STEM_RIGHT_DOWN;
                m_drawingShapes.at(n2) = LIGATURE_STEM_RIGHT_DOWN;
            }
            else {
                // nothing to change
            }
        }
        // L - B
        else if ((dur1 == DUR_LG) && (dur2 == DUR_BR)) {
            if (up) {
                m_drawingShapes.at(n1) = LIGATURE_STEM_RIGHT_DOWN;
            }
            // automatically set oblique on B only at the beginning and end
            else if ((n1 == 0) || (isLastNote)) {
                m_drawingShapes.at(n1) = LIGATURE_OBLIQUE;
                // make sure we previous one is not oblique
                if (n1 > 0) {
                    m_drawingShapes.at(n1 - 1) &= ~LIGATURE_OBLIQUE;
                }
            }
        }
        // B - B
        else if ((dur1 == DUR_BR) && (dur2 == DUR_BR)) {
            if (up) {
                // nothing to change
            }
            // automatically set oblique on B only at the beginning and end
            else if ((n1 == 0) || (isLastNote)) {
                m_drawingShapes.at(n1) = LIGATURE_OBLIQUE;
                // make sure we previous one is not oblique
                if (n1 > 0) {
                    m_drawingShapes.at(n1 - 1) &= ~LIGATURE_OBLIQUE;
                }
                else {
                    m_drawingShapes.at(n1) |= LIGATURE_STEM_LEFT_DOWN;
                }
            }
        }
        // B - L
        else if ((dur1 == DUR_BR) && (dur2 == DUR_LG)) {
            if (up) {
                m_drawingShapes.at(n2) = LIGATURE_STEM_RIGHT_DOWN;
            }
            else {
                if (!isLastNote) {
                    m_drawingShapes.at(n2) = LIGATURE_STEM_RIGHT_DOWN;
                }
                if (n1 == 0) {
                    m_drawingShapes.at(n1) = LIGATURE_STEM_LEFT_DOWN;
                }
            }
        }
        // SB - SB
        else if ((dur1 == DUR_1) && (dur2 == DUR_1)) {
            m_drawingShapes.at(n1) = LIGATURE_STEM_LEFT_UP;
        }
        // SB - L (this should not happen on the first two notes, but this is an encoding problem)
        else if ((dur1 == DUR_1) && (dur2 == DUR_LG)) {
            if (up) {
                m_drawingShapes.at(n2) = LIGATURE_STEM_RIGHT_DOWN;
            }
            else {
                // nothing to change
            }
        }
        // SB - B (this should not happen on the first two notes, but this is an encoding problem)
        else if ((dur1 == DUR_1) && (dur2 == DUR_BR)) {
            if (up) {
                // nothing to change
            }
            // only set the oblique with the SB if the following B is not the start of an oblique
            else if (note->GetLig() != noteAnlMensural_LIG_obliqua) {
                m_drawingShapes.at(n1) = LIGATURE_OBLIQUE;
                if (n1 > 0) {
                    m_drawingShapes.at(n1 - 1) &= ~LIGATURE_OBLIQUE;
                }
            }
        }

        // Blindly set the oblique shape wihout trying to deal with encoding problems
        if (oblique) {
            m_drawingShapes.at(n1) += LIGATURE_OBLIQUE;
            if (n1 > 0) {
                m_drawingShapes.at(n1 - 1) &= ~LIGATURE_OBLIQUE;
            }
        }

        /*
        if (isLastNote && up && (diatonicStep > 2)) {
            note->SetDrawingXRel(previousRight - 2 * width);
            //note->SetColor("red");
            return FUNCTOR_SIBLINGS;
        }
        */

        oblique = false;
        previousNote = note;
        n1++;
        n2++;
    }

    /**** Set the xRel position for each note ****/

    int previousRight = 0;
    previousNote = NULL;
    n1 = 0;

    for (auto &iter : *notes) {

        Note *note = dynamic_cast<Note *>(iter);
        assert(note);

        // previousRight is 0 for the first note
        int width = (note->GetDrawingRadius(params->m_doc, true) * 2)
            - params->m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        note->SetDrawingXRel(previousRight);
        previousRight += width;

        if (!previousNote) {
            previousNote = note;
            continue;
        }

        int diatonicStep = note->GetDiatonicPitch() - previousNote->GetDiatonicPitch();

        // For large interval and oblique, adjust the x position to limit the angle
        if ((m_drawingShapes.at(n1) & LIGATURE_OBLIQUE) && (abs(diatonicStep) > 2)) {
            // angle stay the same from third onward (2 / 3 or a brevis per diatonic step)
            int shift = (abs(diatonicStep) - 2) * width * 2 / 3;
            note->SetDrawingXRel(note->GetDrawingXRel() + shift);
            previousRight += shift;
        }
        previousNote = note;
        n1++;
    }

    return FUNCTOR_SIBLINGS;
}

int Ligature::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    m_drawingShapes.clear();

    // We want the list of the ObjectListInterface to be re-generated
    this->Modify();
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
