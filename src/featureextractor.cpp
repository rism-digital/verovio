/////////////////////////////////////////////////////////////////////////////
// Name:        featureextractor.cpp
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "featureextractor.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>

//----------------------------------------------------------------------------

#include "chord.h"
#include "doc.h"
#include "gracegrp.h"
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
    Reset();
}

FeatureExtractor::~FeatureExtractor() {}

void FeatureExtractor::Reset()
{
    m_previousNote = NULL;
}

void FeatureExtractor::Extract(Object *object, GenerateFeaturesParams *params)
{
    if (object->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(object);
        assert(note);

        // Check if the note is tied to a previous one and skip it if yes
        if (note->GetScoreTimeTiedDuration() == -1.0) return;

        note->CalcMIDIPitch(0);

        std::stringstream pitch;

        data_OCTAVE oct = note->GetOct();
        char octSign = (oct > 3) ? '\'' : ',';
        int signCount = (oct > 3) ? (oct - 3) : (4 - oct);
        pitch << std::string(signCount, octSign);

        Accid *accid = dynamic_cast<Accid *>(note->FindDescendantByType(ACCID));
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
        }

        std::string pname = note->AttPitch::PitchnameToStr(note->GetPname());
        std::transform(pname.begin(), pname.end(), pname.begin(), ::toupper);
        pitch << pname;

        m_pitches << pitch.str();
        jsonxx::Array pitchesIds;
        pitchesIds << note->GetUuid();
        m_pitchesIds << jsonxx::Value(pitchesIds);

        // We have a previous note, so we can calculate an interval
        if (m_previousNote) {
            std::string interval = StringFormat("%d", note->GetMIDIPitch() - m_previousNote->GetMIDIPitch());
            m_intervals << interval;
            jsonxx::Array intervalsIds;
            intervalsIds << m_previousNote->GetUuid();
            intervalsIds << note->GetUuid();
            m_intervalsIds << jsonxx::Value(intervalsIds);
        }
        m_previousNote = note;
    }
}

void FeatureExtractor::ToJson(std::string &output)
{
    jsonxx::Object o;
    o << "pitches" << m_pitches;
    o << "pitchesIds" << m_pitchesIds;
    o << "intervals" << m_intervals;
    o << "intervalsIds" << m_intervalsIds;
    output = o.json();
    LogDebug("%s", output.c_str());
}

} // namespace vrv
