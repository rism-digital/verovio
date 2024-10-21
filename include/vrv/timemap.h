/////////////////////////////////////////////////////////////////////////////
// Name:        timemap.h
// Author:      Laurent Pugin
// Created:     29/01/2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TIMEMAP_H__
#define __VRV_TIMEMAP_H__

#include <cassert>
#include <map>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "horizontalaligner.h"

//----------------------------------------------------------------------------

#include "jsonxx.h"

namespace vrv {

class Object;

//----------------------------------------------------------------------------
// TimemapEntry
//----------------------------------------------------------------------------

/**
 * Helper struct to store timemap entries
 */
struct TimemapEntry {
    double tempo = -1000.0;
    double tstamp;
    std::vector<std::string> notesOn;
    std::vector<std::string> notesOff;
    std::vector<std::string> restsOn;
    std::vector<std::string> restsOff;
    std::string measureOn;
};

//----------------------------------------------------------------------------
// Timemap
//----------------------------------------------------------------------------

/**
 * This class holds a timemap for exporting onset / offset values.
 */
class Timemap {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    Timemap();
    virtual ~Timemap();
    ///@}

    /** Resets the timemap */
    void Reset();

    /**
     * Return (and possibly add) an entry for the given time.
     */
    TimemapEntry &GetEntry(const Fraction &time) { return m_map[time]; }

    /**
     * Write the current timemap to a JSON string
     */
    void ToJson(std::string &output, bool includeRests, bool includeMeasures, bool useFractions);

    static jsonxx::Array ToArray(const Fraction &fraction);

private:
    //
public:
    //
private:
    /** The map with time values as keys */
    std::map<Fraction, TimemapEntry> m_map;

}; // class Timemap

} // namespace vrv

#endif // __VRV_TIMEMAP_H__
