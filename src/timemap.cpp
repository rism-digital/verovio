/////////////////////////////////////////////////////////////////////////////
// Name:        timemap.cpp
// Author:      Laurent Pugin
// Created:     29/01/2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "timemap.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "measure.h"
#include "note.h"
#include "rest.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Timemap
//----------------------------------------------------------------------------

Timemap::Timemap()
{
    this->Reset();
}

Timemap::~Timemap() {}

void Timemap::Reset()
{
    m_map.clear();
}

void Timemap::AddEntry(Object *object, GenerateTimemapParams *params)
{
    assert(object);
    assert(params);

    // It is a bit weird to have a timemap parameter, but we need it to call this method from the functor.
    // Just make sure they are the same because below we use m_map (and not params->m_timemap)
    assert(params->m_timemap == this);

    TimemapEntry emptyEntry;

    if (object->Is({ NOTE, REST })) {
        DurationInterface *interface = object->GetDurationInterface();
        assert(interface);

        double realTimeStart = params->m_realTimeOffsetMilliseconds + interface->GetRealTimeOnsetMilliseconds();
        double scoreTimeStart = params->m_scoreTimeOffset + interface->GetScoreTimeOnset();

        double realTimeEnd = params->m_realTimeOffsetMilliseconds + interface->GetRealTimeOffsetMilliseconds();
        double scoreTimeEnd = params->m_scoreTimeOffset + interface->GetScoreTimeOffset();

        bool isRest = (object->Is(REST));

        /*********** start values ***********/

        if (m_map.count(realTimeStart) == 0) {
            m_map[realTimeStart] = emptyEntry;
        }
        TimemapEntry *startEntry = &m_map.at(realTimeStart);

        // Should check if value for realTimeStart already exists and if so, then
        // ensure that it is equal to scoreTimeStart:
        startEntry->qstamp = scoreTimeStart;

        // Store the element ID in list to turn on at given time - note or rest
        if (!isRest) startEntry->notesOn.push_back(object->GetID());
        if (isRest) startEntry->restsOn.push_back(object->GetID());

        // Also add the tempo the
        startEntry->tempo = params->m_currentTempo;

        /*********** end values ***********/

        if (m_map.count(realTimeEnd) == 0) {
            m_map[realTimeEnd] = emptyEntry;
        }
        TimemapEntry *endEntry = &m_map.at(realTimeEnd);

        // Should check if value for realTimeEnd already exists and if so, then
        // ensure that it is equal to scoreTimeEnd:
        endEntry->qstamp = scoreTimeEnd;

        // Store the element ID in list to turn off at given time - notes or rest
        if (!isRest) endEntry->notesOff.push_back(object->GetID());
        if (isRest) endEntry->restsOff.push_back(object->GetID());
    }
    else if (object->Is(MEASURE)) {

        Measure *measure = vrv_cast<Measure *>(object);
        assert(measure);

        // Deal with repeated music later, for now get the last times.
        double scoreTimeStart = params->m_scoreTimeOffset;
        double realTimeStart = params->m_realTimeOffsetMilliseconds;

        if (m_map.count(realTimeStart) == 0) {
            m_map[realTimeStart] = emptyEntry;
        }
        TimemapEntry *startEntry = &m_map.at(realTimeStart);

        // Should check if value for realTimeStart already exists and if so, then
        // ensure that it is equal to scoreTimeStart:
        startEntry->qstamp = scoreTimeStart;

        // Add the measureOn
        startEntry->measureOn = measure->GetID();
    }
}

void Timemap::ToJson(std::string &output, bool includeRests, bool includeMeasures)
{
    double currentTempo = -1000.0;
    double newTempo;

    jsonxx::Array timemap;

    for (auto &[tstamp, entry] : m_map) {
        jsonxx::Object o;
        o << "tstamp" << tstamp;
        o << "qstamp" << entry.qstamp;

        // on / off
        if (!entry.notesOn.empty()) {
            jsonxx::Array notesOn;
            for (auto note : entry.notesOn) notesOn << note;
            o << "on" << notesOn;
        }
        if (!entry.notesOff.empty()) {
            jsonxx::Array notesOff;
            for (auto note : entry.notesOff) notesOff << note;
            o << "off" << notesOff;
        }

        // restsOn / restsOff
        if (includeRests) {
            if (!entry.restsOn.empty()) {
                jsonxx::Array restsOn;
                for (auto rest : entry.restsOn) restsOn << rest;
                o << "restsOn" << restsOn;
            }
            if (!entry.restsOff.empty()) {
                jsonxx::Array restsOff;
                for (auto rest : entry.restsOff) restsOff << rest;
                o << "restsOff" << restsOff;
            }
        }

        // tempo
        if (entry.tempo != -1000.0) {
            newTempo = entry.tempo;
            if (newTempo != currentTempo) {
                currentTempo = newTempo;
                o << "tempo" << std::to_string(currentTempo);
            }
        }

        // measureOn
        if (includeMeasures && !entry.measureOn.empty()) {
            o << "measureOn" << entry.measureOn;
        }

        timemap << o;
    }
    output = timemap.json();
}

} // namespace vrv
