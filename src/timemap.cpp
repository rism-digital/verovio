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

#include "jsonxx.h"
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

void Timemap::ToJson(std::string &output, bool includeRests, bool includeMeasures, bool useFractions)
{
    double currentTempo = -1000.0;
    double newTempo;

    jsonxx::Array timemap;

    for (auto &[qstamp, entry] : m_map) {
        jsonxx::Object o;
        if (useFractions) {
            o << "qfrac" << Timemap::ToArray(qstamp);
        }
        else {
            o << "qstamp" << qstamp.ToDouble();
        }
        o << "tstamp" << entry.tstamp;

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
                o << "tempo" << currentTempo;
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

jsonxx::Array Timemap::ToArray(const Fraction &fraction)
{
    jsonxx::Array array;
    array << fraction.GetNumerator();
    array << fraction.GetDenominator();
    return array;
}

} // namespace vrv
