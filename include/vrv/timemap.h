/////////////////////////////////////////////////////////////////////////////
// Name:        timemap.h
// Author:      Laurent Pugin
// Created:     29/01/2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TIMEMAP_H__
#define __VRV_TIMEMAP_H__

#include <assert.h>
#include <map>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

namespace vrv {

class Object;
class GenerateTimemapParams;

//----------------------------------------------------------------------------
// TimemapEntry
//----------------------------------------------------------------------------

/**
 * Helper struct to store timemap entries
 */
struct TimemapEntry {
    double tempo = -1000.0;
    double qstamp;
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
 * This class represents a clock to measure runtime.
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

    /** Resets the clock */
    void Reset();

    void AddEntry(Object *object, GenerateTimemapParams *params);

    /**
     * Write the current timemap to a JSON string
     */
    void ToJson(std::string &output);

private:
    //
public:
    //
private:
    /** The time point at which the clock was started */
    std::map<double, TimemapEntry> m_map;

}; // class Timemap

} // namespace vrv

#endif // __VRV_RUNTIMECLOCK_H__
