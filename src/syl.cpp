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

#include "editorial.h"
#include "functorparams.h"
#include "note.h"
#include "staff.h"
#include "textelement.h"
#include "verse.h"

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
// Functor methods
//----------------------------------------------------------------------------

int Syl::PrepareLyrics(FunctorParams *functorParams)
{
    PrepareLyricsParams *params = dynamic_cast<PrepareLyricsParams *>(functorParams);
    assert(params);

    Verse *verse = dynamic_cast<Verse *>(this->GetFirstParent(VERSE, MAX_NOTE_DEPTH));
    if (verse) {
        m_drawingVerse = std::max(verse->GetN(), 1);
    }

    this->SetStart(dynamic_cast<LayerElement *>(this->GetFirstParent(NOTE, MAX_NOTE_DEPTH)));

    // At this stage currentSyl is actually the previous one that is ending here
    if (params->m_currentSyl) {
        // The previous syl was an initial or median -> The note we just parsed is the end
        if ((params->m_currentSyl->GetWordpos() == sylLog_WORDPOS_i)
            || (params->m_currentSyl->GetWordpos() == sylLog_WORDPOS_m)) {
            params->m_currentSyl->SetEnd(params->m_lastNote);
        }
        // The previous syl was a underscore -> the previous but one was the end
        else if (params->m_currentSyl->GetCon() == sylLog_CON_u) {
            params->m_currentSyl->SetEnd(params->m_lastButOneNote);
        }
    }

    // Now decide what to do with the starting syl and check if it has a forward connector
    if ((this->GetWordpos() == sylLog_WORDPOS_i) || (this->GetWordpos() == sylLog_WORDPOS_m)) {
        params->m_currentSyl = this;
        return FUNCTOR_CONTINUE;
    }
    else if (this->GetCon() == sylLog_CON_u) {
        params->m_currentSyl = this;
        return FUNCTOR_CONTINUE;
    }
    else {
        params->m_currentSyl = NULL;
    }

    return FUNCTOR_CONTINUE;
}

int Syl::FillStaffCurrentTimeSpanning(FunctorParams *functorParams)
{
    // Pass it to the pseudo functor of the interface
    return TimeSpanningInterface::InterfaceFillStaffCurrentTimeSpanning(functorParams, this);
}

int Syl::ResetDrawing(FunctorParams *functorParams)
{
    // Pass it to the pseudo functor of the interface
    return TimeSpanningInterface::InterfaceResetDrawing(functorParams, this);
};

} // namespace vrv
