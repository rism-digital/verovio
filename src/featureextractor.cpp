/////////////////////////////////////////////////////////////////////////////
// Name:        featureextractor.cpp
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "featureextractor.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>

//----------------------------------------------------------------------------

#include "chord.h"
#include "doc.h"
#include "gracegrp.h"
#include "iopae.h"
#include "layer.h"
#include "mdiv.h"
#include "measure.h"
#include "metersig.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "rest.h"
#include "space.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// FeatureExtractor
//----------------------------------------------------------------------------

FeatureExtractor::FeatureExtractor(const std::string &options)
{
    // We currently have no option support
    this->Reset();
}

FeatureExtractor::~FeatureExtractor() {}

void FeatureExtractor::Reset()
{
    m_previousNotes.clear();
}

void FeatureExtractor::Extract(const Object *object)
{
    if (object->Is(NOTE)) {
        const Note *note = vrv_cast<const Note *>(object);
        assert(note);

        // Check if the note is part of a chord and return if it is not its top one
        const Chord *chord = note->IsChordTone();
        if (chord && (note != chord->GetTopNote())) return;

        // Check if the note is tied to a previous one and skip it if yes
        if (note->GetScoreTimeTiedDuration() == -1) {
            // Check if we need to add it to the previous interval ids
            const int intervalsIdsSize = (int)m_intervalsIds.size();
            if (intervalsIdsSize > 0) m_intervalsIds.get<jsonxx::Array>(intervalsIdsSize - 1) << note->GetID();
            // Same for pitch ids
            const int pitchesIdsSize = (int)m_pitchesIds.size();
            if (pitchesIdsSize > 0) m_pitchesIds.get<jsonxx::Array>(pitchesIdsSize - 1) << note->GetID();
            m_previousNotes.push_back(note);
            return;
        }

        std::stringstream pitch;
        std::stringstream pitchWithDuration;

        pitchWithDuration << PAEOutput::GetPaeDur(note->GetDur(), note->GetDots());

        data_OCTAVE oct = note->GetOct();
        char octSign = (oct > 3) ? '\'' : ',';
        int signCount = (oct > 3) ? (oct - 3) : (4 - oct);
        std::string octaves = std::string(signCount, octSign);
        pitch << octaves;
        pitchWithDuration << octaves;

        const Accid *accid = vrv_cast<const Accid *>(note->FindDescendantByType(ACCID));
        if (accid) {
            // We need to check both written and gestural accidentals
            std::string accidStrWritten;
            switch (accid->GetAccid()) {
                case (ACCIDENTAL_WRITTEN_s): accidStrWritten = "x"; break;
                case (ACCIDENTAL_WRITTEN_f): accidStrWritten = "b"; break;
                case (ACCIDENTAL_WRITTEN_ss): accidStrWritten = "xx"; break;
                case (ACCIDENTAL_WRITTEN_x): accidStrWritten = "xx"; break;
                case (ACCIDENTAL_WRITTEN_ff): accidStrWritten = "bb"; break;
                // case (ACCIDENTAL_WRITTEN_n): accidStrWritten = "n"; break;
                default: accidStrWritten = "";
            }
            std::string accidStr;
            switch (accid->GetAccidGes()) {
                case (ACCIDENTAL_GESTURAL_s): accidStr = "x"; break;
                case (ACCIDENTAL_GESTURAL_f): accidStr = "b"; break;
                case (ACCIDENTAL_GESTURAL_ss): accidStr = "xx"; break;
                case (ACCIDENTAL_GESTURAL_ff): accidStr = "bb"; break;
                // case (ACCIDENTAL_GESTURAL_n): accidStr = "n"; break;
                default: accidStr = accidStrWritten;
            }
            pitch << accidStr;
            pitchWithDuration << accidStr;
        }

        std::string pname = note->AttPitch::PitchnameToStr(note->GetPname());
        std::transform(pname.begin(), pname.end(), pname.begin(), ::toupper);
        pitch << pname;
        pitchWithDuration << pname;

        m_pitchesChromatic << pitch.str();
        m_pitchesChromaticWithDuration << pitchWithDuration.str();
        m_pitchesDiatonic << pname;
        jsonxx::Array pitchesIds;
        pitchesIds << note->GetID();
        m_pitchesIds << jsonxx::Value(pitchesIds);

        // We have a previous note (or more with tied notes), so we can calculate an interval
        if (!m_previousNotes.empty()) {
            const int intervalChromatic = note->GetMIDIPitch() - m_previousNotes.front()->GetMIDIPitch();
            if (intervalChromatic == 0) {
                m_intervalGrossContour << "s";
                m_intervalRefinedContour << "s";
            }
            else if (intervalChromatic < 0) {
                m_intervalGrossContour << "D";
                m_intervalRefinedContour << ((intervalChromatic < -2) ? "D" : "d");
            }
            else {
                m_intervalGrossContour << "U";
                m_intervalRefinedContour << ((intervalChromatic > 2) ? "U" : "u");
            }
            m_intervalsChromatic << StringFormat("%d", intervalChromatic);
            std::string intervalDiatonicStr
                = StringFormat("%d", note->GetDiatonicPitch() - m_previousNotes.front()->GetDiatonicPitch());
            m_intervalsDiatonic << intervalDiatonicStr;
            jsonxx::Array intervalsIds;
            for (const Note *previousNote : m_previousNotes) {
                intervalsIds << previousNote->GetID();
            }
            intervalsIds << note->GetID();
            m_intervalsIds << jsonxx::Value(intervalsIds);
        }
        m_previousNotes.clear();
        m_previousNotes.push_back(note);
    }
}

void FeatureExtractor::ToJson(std::string &output)
{
    jsonxx::Object o;

    o << "pitchesChromaticWithDuration" << m_pitchesChromaticWithDuration;
    o << "pitchesChromatic" << m_pitchesChromatic;
    o << "pitchesDiatonic" << m_pitchesDiatonic;
    o << "pitchesIds" << m_pitchesIds;

    o << "intervalsChromatic" << m_intervalsChromatic;
    o << "intervalsDiatonic" << m_intervalsDiatonic;
    o << "intervalGrossContour" << m_intervalGrossContour;
    o << "intervalRefinedContour" << m_intervalRefinedContour;
    o << "intervalsIds" << m_intervalsIds;

    output = o.json();
    LogDebug("%s", output.c_str());
}

} // namespace vrv
