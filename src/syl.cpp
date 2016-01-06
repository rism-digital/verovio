/////////////////////////////////////////////////////////////////////////////
// Name:        syl.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "syl.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "note.h"
#include "verse.h"
#include "staff.h"
#include "textelement.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Syl
//----------------------------------------------------------------------------

Syl::Syl() : LayerElement("syl-"), TextListInterface(), TimeSpanningInterface(), AttTypography(), AttSylLog()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_TYPOGRAPHY);
    RegisterAttClass(ATT_SYLLOG);

    Reset();
}

Syl::~Syl()
{
}

void Syl::Reset()
{
    LayerElement::Reset();
    TimeSpanningInterface::Reset();
    ResetTypography();
    ResetSylLog();

    m_drawingVerse = 1;
}

void Syl::AddTextElement(TextElement *element)
{
    assert(dynamic_cast<TextElement *>(element) || dynamic_cast<EditorialElement *>(element));
    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Syl::PrepareLyrics(ArrayPtrVoid *params)
{
    // param 0: the current Syl
    // param 1: the last Note
    // param 2: the last but one Note
    Syl **currentSyl = static_cast<Syl **>((*params).at(0));
    Note **lastNote = static_cast<Note **>((*params).at(1));
    Note **lastButOneNote = static_cast<Note **>((*params).at(2));

    Verse *verse = dynamic_cast<Verse *>(this->GetFirstParent(VERSE, MAX_NOTE_DEPTH));
    if (verse) {
        m_drawingVerse = std::max(verse->GetN(), 1);
    }

    this->SetStart(dynamic_cast<LayerElement *>(this->GetFirstParent(NOTE, MAX_NOTE_DEPTH)));

    // At this stage currentSyl is actually the previous one that is ending here
    if ((*currentSyl)) {
        // The previous syl was an initial or median -> The note we just parsed is the end
        if (((*currentSyl)->GetWordpos() == sylLog_WORDPOS_i) || ((*currentSyl)->GetWordpos() == sylLog_WORDPOS_m)) {
            (*currentSyl)->SetEnd(*lastNote);
        }
        // The previous syl was a underscore -> the previous but one was the end
        else if ((*currentSyl)->GetCon() == sylLog_CON_u) {
            (*currentSyl)->SetEnd(*lastButOneNote);
        }
    }

    // Now decide what to do with the starting syl and check if it has a forward connector
    if ((this->GetWordpos() == sylLog_WORDPOS_i) || (this->GetWordpos() == sylLog_WORDPOS_m)) {
        (*currentSyl) = this;
        return FUNCTOR_CONTINUE;
    }
    else if (this->GetCon() == sylLog_CON_u) {
        (*currentSyl) = this;
        return FUNCTOR_CONTINUE;
    }
    else {
        (*currentSyl) = NULL;
    }

    return FUNCTOR_CONTINUE;
}

int Syl::FillStaffCurrentTimeSpanning(ArrayPtrVoid *params)
{
    // Pass it to the pseudo functor of the interface
    return TimeSpanningInterface::InterfaceFillStaffCurrentTimeSpanning(params, this);
}

int Syl::ResetDrawing(ArrayPtrVoid *params)
{
    // Pass it to the pseudo functor of the interface
    return TimeSpanningInterface::InterfaceResetDrawing(params, this);
};

} // namespace vrv
