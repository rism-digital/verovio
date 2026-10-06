/////////////////////////////////////////////////////////////////////////////
// Name:        vrv.h
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_H__
#define __VRV_H__

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <format>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef _WIN32
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "attdef.h"
#include "atttypes.h"
#include "toolkitdef.h"
#include "vrvdef.h"


/**
 * A formatter for all enums types.
 * Simply format the enum as its numerical value, but with C++26, it will be
 * possible to use reflection to format as the enum name.
 */
template <typename E>
requires std::is_enum_v<E>
struct std::formatter<E> : std::formatter<std::string> {
    template<class FormatContext>
	constexpr auto format (const E& e, FormatContext& ctx) const
	{
		return std::formatter<std::string>::format(std::to_string(e), ctx);
	}
};


namespace vrv {

class Object;
/**
 * Member and functions specific to logging that uses a vector of string to buffer the logs.
 */
extern std::vector<std::string> logBuffer;
extern LogLevel logLevel;
extern bool loggingToBuffer;
bool LogBufferContains(const std::string &s);
void LogString(std::string message, LogLevel level);

/**
 * Convert a string to a logLevel
 */
LogLevel StrToLogLevel(const std::string &level);

/**
 * Utility for comparing doubles
 */
bool ApproximatelyEqual(double firstVal, double secondVal);

/**
 * Utility to check if the string is a valid integer for std::stoi
 */
bool IsValidInteger(const std::string &value);

/**
 * Utility to check if the string is valid double for std::stod
 */
bool IsValidDouble(const std::string &value);

/**
 * Utility to check if the string contains only digits (positive integer without sign)
 */
bool IsDigits(const std::string &value);

/**
 * Extract the ID from any URI
 */
std::string ExtractIDFragment(std::string refID);

/**
 * Concatentate a list of IDs into a string of URIs
 */
std::string ConcatenateIDs(const ListOfConstObjects &objects);

/**
 * Utility for converting UTF32 (std::u32string) to UTF-8
 */
std::string UTF32to8(const std::u32string &in);

/**
 * Utility for converting UTF-8 to UTF32 (std::u32string)
 */
std::u32string UTF8to32(const std::string &in);

/**
 * Utility for converting UTF16 (std::u16string) to UTF-8
 */
std::string UTF16to8(const std::u16string &in);

/**
 * Format a string using std::format.
 */
template <typename... T> std::string StringFormat(std::format_string<T...> fmt, T &&...args)
{
    return std::format(fmt, std::forward<T>(args)...);
}

/**
 * Format a string using std::vformat, the format string can be chosen at runtime unlike StringFormat..
 * Prefer StringFormat when possible, as the format string will be checked at compile-time.
 */
template <typename... T> std::string StringFormatDynamic(std::string_view fmt, T &&...args)
{
    return std::vformat(fmt, std::forward<T>(args)...);
}


/**
 * The following functions ("Log*") are helpers for formatting, conversion, or logging.
 * Most of them differ if they are used in the command line tool or in emscripten
 */

template<typename ...T>
void LogDebug(std::format_string<T...> fmt, T&& ...args)
{
    if (logLevel < LOG_DEBUG) return;

#if defined(DEBUG)
    std::string s;
    s = "[Debug] " + StringFormat(fmt, std::forward<T>(args)...) + "\n";
    LogString(s, LOG_DEBUG);
#endif
}

template<typename ...T>
void LogError(std::format_string<T...> fmt, T&& ...args)
{
    if (logLevel < LOG_ERROR) return;

    std::string s;
    s = "[Error] " + StringFormat(fmt, std::forward<T>(args)...) + "\n";
    LogString(s, LOG_ERROR);
}

template<typename ...T>
void LogInfo(std::format_string<T...> fmt, T&&... args)
{
    if (logLevel < LOG_INFO) return;

    std::string s;
    s = "[Info] " + StringFormat(fmt, std::forward<T>(args)...) + "\n";
    LogString(s, LOG_INFO);
}

template<typename ...T>
void LogWarning(std::format_string<T...> fmt, T&&... args)
{
    if (logLevel < LOG_WARNING) return;

    std::string s;
    s = "[Warning] " + StringFormat(fmt, std::forward<T>(args)...) + "\n";
    LogString(s, LOG_WARNING);
}

/**
 * Return a formatted version (####.####.####) of the file version.
 * This can be used for comparing if the file version is < or >
 */
std::string GetFileVersion(int vmaj, int vmin, int vrev);

/**
 * Return the filename (without extension) extracted from the fullpath
 */
std::string GetFilename(std::string fullpath);

/**
 * Return the version number (X.X.X)
 */
std::string GetVersion();

/**
 * Encode the integer value using the specified base (max is 62)
 * Base 36 uses 0-9 and a-z, base 62 also A-Z.
 */
std::string BaseEncodeInt(uint32_t value, uint8_t base);

/**
 * Convert string from camelCase.
 */
std::string FromCamelCase(const std::string &s);

/**
 * Convert string to camelCase.
 */
std::string ToCamelCase(const std::string &s);

/**
 * Min / Max for data_DURATION
 */
inline data_DURATION DurationMin(data_DURATION dur1, data_DURATION dur2)
{
    return std::min(dur1, dur2);
}
inline data_DURATION DurationMax(data_DURATION dur1, data_DURATION dur2)
{
    return std::max(dur1, dur2);
}


/**
 * Functions for logging in milliseconds the elapsed time of an
 * operation (for debugging purposes).
 * LogElapsedTimeStart needs to be called before the operation
 *
 * Ex:
 *
 * LogElapsedTimeStart();
 * ... Do something
 * LogElapsedTimeEnd("name of the operation");
 */
extern struct timeval start;
void LogElapsedTimeStart();
void LogElapsedTimeStop(const char *msg = "unspecified operation");

//----------------------------------------------------------------------------
// Notation type checks
//----------------------------------------------------------------------------

bool IsMensuralType(data_NOTATIONTYPE notationType);
bool IsNeumeType(data_NOTATIONTYPE notationType);
bool IsTabType(data_NOTATIONTYPE notationType);

//----------------------------------------------------------------------------
// Base64 code borrowed
//----------------------------------------------------------------------------

std::string Base64Encode(unsigned char const *bytesToEncode, unsigned int len);
std::vector<unsigned char> Base64Decode(std::string const &encodedString);

} // namespace vrv

#endif
