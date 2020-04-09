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
    int previousRight = 0;
    Staff *staff = dynamic_cast<Staff *>(this->GetFirstAncestor(STAFF));
    assert(staff);

    const ArrayOfObjects *notes = this->GetList(this);
    assert(notes);
    Note *previousNote = NULL;
    int n1 = 0;
    int n2 = 1;

    for (auto &iter : *notes) {

        Note *note = dynamic_cast<Note *>(iter);
        assert(note);

        int width = (note->GetDrawingRadius(params->m_doc) * 2)
            - params->m_doc->GetDrawingStemWidth(staff->m_drawingStaffSize);
        note->SetDrawingXRel(previousRight);
        previousRight += width;
        m_drawingShapes.push_back(LIGATURE_DEFAULT);

        if (!previousNote) {
            previousNote = note;
            continue;
        }

        int dur1 = previousNote->GetActualDur();
        int dur2 = note->GetActualDur();

        int diatonicStep = note->GetDiatonicPitch() - previousNote->GetDiatonicPitch();
        bool up = (diatonicStep > 0);
        bool isLastNote = (note == lastNote);

        if (dur1 == DUR_LG) previousNote->SetColor("red");
        if (dur1 == DUR_BR) previousNote->SetColor("green");
        if (dur2 == DUR_LG) note->SetColor("red");
        if (dur2 == DUR_BR) note->SetColor("green");

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
            else {
                m_drawingShapes.at(n1) = LIGATURE_OBLIQUE;
                // make sure we previous one is not oblique
                if (n1 > 0) m_drawingShapes.at(n1 - 1) &= ~LIGATURE_OBLIQUE;
            }
        }
        // B - B
        else if ((dur1 == DUR_BR) && (dur2 == DUR_BR)) {
            if (up) {
                // nothing to change
            }
            else {
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

        /*
        if (isLastNote && up && (diatonicStep > 2)) {
            note->SetDrawingXRel(previousRight - 2 * width);
            //note->SetColor("red");
            return FUNCTOR_SIBLINGS;
        }
        */

        // if (up) note->SetColor("blue");
        // else note->SetColor("green");

        previousNote = note;
        n1++;
        n2++;
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
