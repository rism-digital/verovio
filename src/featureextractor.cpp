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

FeatureExtractor::FeatureExtractor()
{
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
        note->CalcMIDIPitch(0);

        std::string pitch = note->AttPitch::PitchnameToStr(note->GetPname());
        pitch += note->AttPitch::OctaveToStr(note->GetOct());
        m_pitches << pitch;

        // We have a previous note, so we can calculate an interval
        if (m_previousNote) {
            std::string interval = StringFormat("%d", note->GetMIDIPitch() - m_previousNote->GetMIDIPitch());
            m_intervals << interval;
        }
        m_previousNote = note;
    }
}

void FeatureExtractor::ToJson(std::string &output)
{
    jsonxx::Object o;
    o << "pitches" << m_pitches;
    o << "intervals" << m_intervals;
    output = o.json();
    LogDebug("%s", output.c_str());
}

} // namespace vrv
