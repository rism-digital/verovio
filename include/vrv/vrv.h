/////////////////////////////////////////////////////////////////////////////
// Name:        vrv.h
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_H__
#define __VRV_H__

#include <cstring>
#include <map>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef _WIN32
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "atttypes.h"

namespace vrv {

class Object;

/**
 * The following functions are helpers for formatting, conversion, or logging.
 * Most of them differ if they are used in the command line tool or in emscripten
 */

void LogDebug(const char *fmt, ...);
void LogError(const char *fmt, ...);
void LogMessage(const char *fmt, ...);
void LogWarning(const char *fmt, ...);

/**
 * Member and functions specific to loging that uses a vector of string to buffer the logs.
 */
enum consoleLogLevel { CONSOLE_LOG = 0, CONSOLE_INFO, CONSOLE_WARN, CONSOLE_ERROR, CONSOLE_DEBUG };
extern std::vector<std::string> logBuffer;
bool LogBufferContains(const std::string &s);
void LogString(std::string message, consoleLogLevel level);

/**
 * Utility for comparing doubles
 */
bool AreEqual(double dFirstVal, double dSecondVal);

/**
 * Extract the ID from any URI
 */
std::string ExtractIDFragment(std::string refID);

/**
 * Utility for converting UTF32 (std::u32string) to UTF-8
 */
std::string UTF32to8(const std::u32string &in);

/**
 * Utility for converting UTF-8 to UTF32 (std::u32string)
 */
std::u32string UTF8to32(const std::string &in);

/**
 * Format a string using vsnprintf.
 * The maximum length is giving by STRING_FORMAT_MAX_LEN
 */
std::string StringFormat(const char *fmt, ...);
// This is the implementation callable with variable arguments
std::string StringFormatVariable(const char *format, va_list arg);

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
std::string BaseEncodeInt(unsigned int value, unsigned int base);

/**
 *
 */
extern bool logging;
extern bool loggingToBuffer;

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
void LogElapsedTimeEnd(const char *msg = "unspecified operation");

/**
 * Method that simply checks if the Object is not NULL
 * Also asserts it for stopping in debug mode
 */
bool Check(Object *object);

//----------------------------------------------------------------------------
// Base64 code borrowed
//----------------------------------------------------------------------------

std::string Base64Encode(unsigned char const *bytesToEncode, unsigned int len);
std::vector<unsigned char> Base64Decode(std::string const &encodedString);

} // namespace vrv

#endif
