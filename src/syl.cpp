/////////////////////////////////////////////////////////////////////////////
// Name:        syl.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "syl.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "editorial.h"
#include "functor.h"
#include "functorparams.h"
#include "measure.h"
#include "note.h"
#include "smufl.h"
#include "staff.h"
#include "text.h"
#include "textelement.h"
#include "verse.h"
#include "vrv.h"
#include "zone.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Syl
//----------------------------------------------------------------------------

static const ClassRegistrar<Syl> s_factory("syl", SYL);

Syl::Syl()
    : LayerElement(SYL, "syl-"), TextListInterface(), TimeSpanningInterface(), AttLang(), AttTypography(), AttSylLog()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_LANG);
    this->RegisterAttClass(ATT_TYPOGRAPHY);
    this->RegisterAttClass(ATT_SYLLOG);

    this->Reset();
}

Syl::~Syl() {}

void Syl::Reset()
{
    LayerElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetLang();
    this->ResetTypography();
    this->ResetSylLog();

    m_drawingVerse = 1;
    m_nextWordSyl = NULL;
}

bool Syl::IsSupportedChild(Object *child)
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
        return false;
    }
    return true;
}

int Syl::CalcConnectorSpacing(Doc *doc, int staffSize)
{
    assert(doc);

    sylLog_WORDPOS pos = this->GetWordpos();
    sylLog_CON con = this->GetCon();

    int spacing = 0;

    // We have a word connector - the space have to be wide enough
    if ((pos == sylLog_WORDPOS_i) || (pos == sylLog_WORDPOS_m)) {
        int hyphen = doc->GetDrawingUnit(staffSize) * doc->GetOptions()->m_lyricHyphenLength.GetValue();
        // Adjust it proportionally to the lyric size
        hyphen *= doc->GetOptions()->m_lyricSize.GetValue() / doc->GetOptions()->m_lyricSize.GetDefault();
        spacing = (2 * hyphen);
    }
    // Elision
    else if (con == sylLog_CON_b) {
        if (doc->GetOptions()->m_lyricElision.GetValue() == ELISION_unicode) {
            // Equivalent spacing with 0x230F
            spacing += doc->GetDrawingUnit(staffSize) * 2.2;
        }
        else {
            // Calculate the elision space with the current music font
            int elisionSpace = doc->GetGlyphAdvX(doc->GetOptions()->m_lyricElision.GetValue(), staffSize, false);
            // Adjust it proportionally to the lyric size
            elisionSpace *= doc->GetOptions()->m_lyricSize.GetValue() / doc->GetOptions()->m_lyricSize.GetDefault();
            spacing = elisionSpace;
        }
    }
    // Spacing of words as set in the staff according to the staff and font sizes
    else {
        int wordSpace = doc->GetDrawingUnit(staffSize) * doc->GetOptions()->m_lyricWordSpace.GetValue();
        // Adjust it proportionally to the lyric size
        wordSpace *= doc->GetOptions()->m_lyricSize.GetValue() / doc->GetOptions()->m_lyricSize.GetDefault();
        spacing = wordSpace;
    }

    return spacing;
}

int Syl::GetDrawingWidth() const
{
    if (this->HasFacs()) {
        return FacsimileInterface::GetWidth();
    }
    return 0;
}

int Syl::GetDrawingHeight() const
{
    if (this->HasFacs()) {
        return FacsimileInterface::GetHeight();
    }
    return 0;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Syl::Accept(MutableFunctor &functor)
{
    return functor.VisitSyl(this);
}

FunctorCode Syl::Accept(ConstFunctor &functor) const
{
    return functor.VisitSyl(this);
}

FunctorCode Syl::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitSylEnd(this);
}

FunctorCode Syl::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitSylEnd(this);
}

int Syl::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    const int startTime = params->m_totalTime + params->m_lastNote->GetScoreTimeOnset();
    Text *text = vrv_cast<Text *>(this->GetChild(0, TEXT));
    const std::string sylText = UTF32to8(text->GetText());

    params->m_midiFile->addLyric(params->m_midiTrack, startTime * params->m_midiFile->getTPQ(), sylText);

    return FUNCTOR_SIBLINGS;
}

bool Syl::CreateDefaultZone(Doc *doc)
{
    const int offsetUly = 100;
    const int offsetLrx = 100;
    const int offsetLry = 200;

    LayerElement *syllable = vrv_cast<LayerElement *>(this->GetFirstAncestor(SYLLABLE));
    if (syllable == NULL) { // Only do this for neume notation
        return false;
    }

    Zone *zone = new Zone();

    if (syllable->HasFacs()) {
        Zone *tempZone = syllable->GetZone();
        assert(tempZone);
        zone->SetUlx(tempZone->GetUlx());
        zone->SetUly(tempZone->GetUly() + offsetUly);
        zone->SetLrx(tempZone->GetLrx() + offsetLrx);
        zone->SetLry(tempZone->GetLry() + offsetLry);
    }
    else {
        int ulx, uly, lrx, lry;
        if (syllable->GenerateZoneBounds(&ulx, &uly, &lrx, &lry)) {
            if (ulx == 0 || uly == 0 || lrx == 0 || lry == 0) {
                LogWarning("Zero value when generating bbox from %s: (%d, %d, %d, %d)", syllable->GetID().c_str(), ulx,
                    uly, lrx, lry);
            }
            zone->SetUlx(ulx);
            zone->SetUly(uly + offsetUly);
            zone->SetLrx(lrx + offsetLrx);
            zone->SetLry(lry + offsetLry);
        }
        else {
            LogWarning("Failed to create zone for %s of type %s", this->GetID().c_str(), this->GetClassName().c_str());
            delete zone;
            return false;
        }
    }
    Object *surface = doc->GetFacsimile()->FindDescendantByType(SURFACE);
    assert(surface);
    surface->AddChild(zone);
    this->AttachZone(zone);
    return true;
}

} // namespace vrv
