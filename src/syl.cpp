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

#include "doc.h"
#include "editorial.h"
#include "functorparams.h"
#include "measure.h"
#include "note.h"
#include "staff.h"
#include "textelement.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Syl
//----------------------------------------------------------------------------

Syl::Syl() : LayerElement("syl-"), FacsimileInterface(), TextListInterface(), TimeSpanningInterface(), AttLang(), AttTypography(), AttSylLog()
{
    RegisterInterface(FacsimileInterface::GetAttClasses(), FacsimileInterface::IsInterface());
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_LANG);
    RegisterAttClass(ATT_TYPOGRAPHY);
    RegisterAttClass(ATT_SYLLOG);

    Reset();
}

Syl::~Syl() {}

void Syl::Reset()
{
    LayerElement::Reset();
    FacsimileInterface::Reset();
    TimeSpanningInterface::Reset();
    ResetLang();
    ResetTypography();
    ResetSylLog();

    m_drawingVerse = 1;
    m_nextWordSyl = NULL;
}

void Syl::AddChild(Object *child)
{
    if (child->Is({ REND, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else if (child->Is(REND)) {
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

int Syl::CalcHorizontalAdjustment(int &overlap, AdjustSylSpacingParams *params)
{
    sylLog_WORDPOS pos = this->GetWordpos();
    int nextFreeSpace = 0;

    // We have a word connector - the space have to be wide enough
    if ((pos == sylLog_WORDPOS_i) || (pos == sylLog_WORDPOS_m)) {
        int hyphen = params->m_doc->GetDrawingUnit(params->m_staffSize)
            * params->m_doc->GetOptions()->m_lyricHyphenLength.GetValue();
        // Adjust it proportionally to the lyric size
        hyphen *= params->m_doc->GetOptions()->m_lyricSize.GetValue()
            / params->m_doc->GetOptions()->m_lyricSize.GetDefault();
        overlap += (2 * hyphen);
    }
    // Spacing of words as set in the staff according to the staff and font sizes
    else {
        int wordSpace = params->m_doc->GetDrawingUnit(params->m_staffSize)
            * params->m_doc->GetOptions()->m_lyricWordSpace.GetValue();
        // Adjust it proportionally to the lyric size
        wordSpace *= params->m_doc->GetOptions()->m_lyricSize.GetValue()
            / params->m_doc->GetOptions()->m_lyricSize.GetDefault();
        overlap += wordSpace;
    }

    if (overlap > 0) {
        // We have enough space to absorb the overla completely
        if (params->m_freeSpace > overlap) {
            params->m_previousSyl->SetDrawingXRel(params->m_previousSyl->GetDrawingXRel() - overlap);
            // The space is set to 0. This means that consecutive overlaps will not be recursively absorbed.
            // Only the first preceeding syl will be moved.
            nextFreeSpace = 0;
            overlap = 0;
        }
        else if (params->m_freeSpace > 0) {
            params->m_previousSyl->SetDrawingXRel(params->m_previousSyl->GetDrawingXRel() - params->m_freeSpace);
            overlap -= params->m_freeSpace;
            nextFreeSpace = 0;
        }
    }
    else {
        nextFreeSpace = std::min(-overlap, 3 * params->m_doc->GetDrawingUnit(100));
    }

    return nextFreeSpace;
}

int Syl::GetDrawingX() const
{
    if (this->HasFacs()) {
        return FacsimileInterface::GetDrawingX();
    }
    else {
        return LayerElement::GetDrawingX();
    }
}

int Syl::GetDrawingY() const
{
    if (this->HasFacs()) {
        return FacsimileInterface::GetDrawingY();
    }
    else {
        return LayerElement::GetDrawingY();
    }
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
            params->m_currentSyl->m_nextWordSyl = this;
        }
        // The previous syl was a underscore -> the previous but one was the end
        else if (params->m_currentSyl->GetCon() == sylLog_CON_u) {
            if (params->m_currentSyl->GetStart() == params->m_lastButOneNote)
                LogWarning("Syllable with underline extender under one single note '%s'",
                    params->m_currentSyl->GetStart()->GetUuid().c_str());
            else
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

int Syl::AdjustSylSpacing(FunctorParams *functorParams)
{
    AdjustSylSpacingParams *params = dynamic_cast<AdjustSylSpacingParams *>(functorParams);
    assert(params);

    if (!this->HasContentHorizontalBB()) {
        LogDebug("Syl %s is skipped in alignment - it is probably empty", this->GetUuid().c_str());
        return FUNCTOR_CONTINUE;
    }

    int shift = params->m_doc->GetDrawingUnit(params->m_staffSize);
    // Adjust it proportionally to the lyric size
    shift
        *= params->m_doc->GetOptions()->m_lyricSize.GetValue() / params->m_doc->GetOptions()->m_lyricSize.GetDefault();

    this->SetDrawingXRel(-1 * shift);

    // Not much to do when we hit the first syllable of the system
    if (params->m_previousSyl == NULL) {
        params->m_previousSyl = this;
        // No free space because we never move the first one back
        params->m_freeSpace = 0;
        params->m_previousMeasure = NULL;
        return FUNCTOR_CONTINUE;
    }

    int xShift = 0;

    // We have a previous syllable from the previous measure - we need to add the measure with because the measures are
    // not aligned yet
    if (params->m_previousMeasure) {
        xShift = params->m_previousMeasure->GetWidth();
    }

    int overlap = params->m_previousSyl->GetContentRight() - (this->GetContentLeft() + xShift);
    int nextFreeSpace = params->m_previousSyl->CalcHorizontalAdjustment(overlap, params);

    if (overlap > 0) {
        // We are adjusting syl in two different measures - move only the to right barline of the first measure
        if (params->m_previousMeasure) {
            params->m_overlapingSyl.push_back(std::make_tuple(params->m_previousSyl->GetAlignment(),
                params->m_previousMeasure->GetRightBarLine()->GetAlignment(), overlap));
            // Do it now
            params->m_previousMeasure->m_measureAligner.AdjustProportionally(params->m_overlapingSyl);
            params->m_overlapingSyl.clear();
        }
        else {
            // Normal case, both in the same measure
            params->m_overlapingSyl.push_back(
                std::make_tuple(params->m_previousSyl->GetAlignment(), this->GetAlignment(), overlap));
        }
    }

    params->m_previousSyl = this;
    params->m_freeSpace = nextFreeSpace;
    params->m_previousMeasure = NULL;

    return FUNCTOR_CONTINUE;
}

int Syl::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);

    m_nextWordSyl = NULL;

    // Pass it to the pseudo functor of the interface
    return TimeSpanningInterface::InterfaceResetDrawing(functorParams, this);
}

} // namespace vrv
