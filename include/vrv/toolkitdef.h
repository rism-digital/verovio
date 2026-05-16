/////////////////////////////////////////////////////////////////////////////
// Name:        toolkitdef.h
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TOOLKITDEF_H__
#define __VRV_TOOLKITDEF_H__

namespace vrv {

enum FileFormat {
    UNKNOWN = 0,
    AUTO,
    MEI,
    HUMDRUM,
    HUMMEI,
    HUMMIDI,
    PAE,
    ABC,
    CMME,
    DARMS,
    VOLPIANO,
    MUSICXML,
    MUSICXMLHUM,
    MEIHUM,
    MUSEDATAHUM,
    ESAC,
    MIDI,
    TIMEMAP,
    EXPANSIONMAP,
    SERIALIZATION
};

enum { LOG_OFF = 0, LOG_ERROR, LOG_WARNING, LOG_INFO, LOG_DEBUG };

typedef int LogLevel;

/**
 * Functions defined here to be available in SWIG bindings
 */

//----------------------------------------------------------------------------
// Function implemented in resources.cpp
//----------------------------------------------------------------------------

void SetDefaultResourcePath(const std::string &path);

//----------------------------------------------------------------------------
// Function implemented in vrv.cpp
//----------------------------------------------------------------------------

void EnableLog(LogLevel level);
void EnableLogToBuffer(bool value);

} // namespace vrv

#endif // __VRV_TOOLKITDEF_H__
