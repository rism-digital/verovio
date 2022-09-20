/////////////////////////////////////////////////////////////////////////////
// Name:        vrv.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "vrv.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <cmath>
#include <codecvt>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <vector>

#ifndef _WIN32
#include <dirent.h>
#else
#include "win_dirent.h"
#include "win_time.h"
#endif

//----------------------------------------------------------------------------

// Windows has no Bourne shell (sh), therefore no "git_commit.h" is created.
#ifndef _WIN32
#ifdef COCOAPODS
#define GIT_COMMIT "[cocoapods]"
#else
#include "git_commit.h"
#endif
#else
#define GIT_COMMIT "[undefined]"
#endif

#include "vrvdef.h"

//----------------------------------------------------------------------------

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef ANDROID
#include <android/log.h>
#define android_log_puts(prio, msg) __android_log_print(prio, "Verovio", "%s", msg)
#endif

#define STRING_FORMAT_MAX_LEN 2048

namespace vrv {

//----------------------------------------------------------------------------
// Logging related methods
//----------------------------------------------------------------------------

/** Global for LogElapsedTimeXXX functions (debugging purposes) */
struct timeval start;

/** For disabling log */
bool logging = true;

/** By default log to stderr or JS console */
bool loggingToBuffer = false;

std::vector<std::string> logBuffer;

void LogElapsedTimeStart()
{
    gettimeofday(&start, NULL);
}

void LogElapsedTimeEnd(const char *msg)
{
    double elapsedTime;
    struct timeval end;
    gettimeofday(&end, NULL);
    elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0; // sec to ms
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0; // us to ms
    LogMessage("Elapsed time (%s): %.3fs", msg, elapsedTime / 1000);
}

void LogDebug(const char *fmt, ...)
{
    if (!logging) return;

#if defined(DEBUG)
    std::string s;
    va_list args;
    va_start(args, fmt);
    s = "[Debug] " + StringFormatVariable(fmt, args) + "\n";
    LogString(s, CONSOLE_DEBUG);
    va_end(args);
#endif
}

void LogError(const char *fmt, ...)
{
    if (!logging) return;

    std::string s;
    va_list args;
    va_start(args, fmt);
    s = "[Error] " + StringFormatVariable(fmt, args) + "\n";
    LogString(s, CONSOLE_ERROR);
    va_end(args);
}

void LogMessage(const char *fmt, ...)
{
    if (!logging) return;

    std::string s;
    va_list args;
    va_start(args, fmt);
    s = "[Message] " + StringFormatVariable(fmt, args) + "\n";
    LogString(s, CONSOLE_INFO);
    va_end(args);
}

void LogWarning(const char *fmt, ...)
{
    if (!logging) return;

    std::string s;
    va_list args;
    va_start(args, fmt);
    s = "[Warning] " + StringFormatVariable(fmt, args) + "\n";
    LogString(s, CONSOLE_WARN);
    va_end(args);
}

void EnableLog(bool value)
{
    logging = value;
}

void EnableLogToBuffer(bool value)
{
    loggingToBuffer = value;
}

void LogString(std::string message, consoleLogLevel level)
{
    if (loggingToBuffer) {
        if (LogBufferContains(message)) return;
        logBuffer.push_back(message);
    }
    else {
#ifdef __EMSCRIPTEN__
        switch (level) {
            case CONSOLE_DEBUG: EM_ASM_ARGS({ console.debug(UTF8ToString($0)); }, message.c_str()); break;
            case CONSOLE_ERROR: EM_ASM_ARGS({ console.error(UTF8ToString($0)); }, message.c_str()); break;
            case CONSOLE_WARN: EM_ASM_ARGS({ console.warn(UTF8ToString($0)); }, message.c_str()); break;
            case CONSOLE_INFO: EM_ASM_ARGS({ console.info(UTF8ToString($0)); }, message.c_str()); break;
            default: EM_ASM_ARGS({ console.log(UTF8ToString($0)); }, message.c_str()); break;
        }
#elif defined ANDROID
        switch (level) {
            case CONSOLE_DEBUG: android_log_puts(ANDROID_LOG_DEBUG, message.c_str()); break;
            case CONSOLE_ERROR: android_log_puts(ANDROID_LOG_ERROR, message.c_str()); break;
            case CONSOLE_WARN: android_log_puts(ANDROID_LOG_WARN, message.c_str()); break;
            case CONSOLE_INFO:
            default: android_log_puts(ANDROID_LOG_INFO, message.c_str()); break;
        }
#else
        fputs(message.c_str(), stderr);
#endif
    }
}

bool LogBufferContains(const std::string &s)
{
    std::vector<std::string>::iterator iter = logBuffer.begin();
    while (iter != logBuffer.end()) {
        if ((*iter) == s) return true;
        ++iter;
    }
    return false;
}

bool Check(Object *object)
{
    assert(object);
    return (object != NULL);
}

//----------------------------------------------------------------------------
// Various helpers
//----------------------------------------------------------------------------

std::string StringFormat(const char *fmt, ...)
{
    std::string str(STRING_FORMAT_MAX_LEN, 0);
    va_list args;
    va_start(args, fmt);
    vsnprintf(&str[0], STRING_FORMAT_MAX_LEN, fmt, args);
    va_end(args);
    str.resize(strlen(str.data()));
    return str;
}

std::string StringFormatVariable(const char *format, va_list arg)
{
    std::string str(STRING_FORMAT_MAX_LEN, 0);
    vsnprintf(&str[0], STRING_FORMAT_MAX_LEN, format, arg);
    str.resize(strlen(str.data()));
    return str;
}

bool AreEqual(double dFirstVal, double dSecondVal)
{
    return std::fabs(dFirstVal - dSecondVal) < 1E-3;
}

std::string ExtractIDFragment(std::string refID)
{
    size_t pos = refID.find_last_of("#");
    if ((pos != std::string::npos) && (pos < refID.length() - 1)) {
        refID = refID.substr(pos + 1);
    }
    return refID;
}

std::string UTF32to8(const std::u32string &in)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> strCnv;
    return strCnv.to_bytes(in);
}

std::u32string UTF8to32(const std::string &in)
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> strCnv;
    return strCnv.from_bytes(in);
}

std::string GetFileVersion(int vmaj, int vmin, int vrev)
{
    return StringFormat("%04d.%04d.%04d", vmaj, vmin, vrev);
}

std::string GetFilename(std::string &fullpath)
{
    // remove extension
    std::string name = fullpath;
    size_t lastdot = name.find_last_of(".");
    if (lastdot != std::string::npos) {
        name = name.substr(0, lastdot);
    }
    size_t lastslash = name.find_last_of("/");
    if (lastslash != std::string::npos) {
        name = name.substr(lastslash + 1, std::string::npos);
    }
    return name;
}

std::string GetVersion()
{
    std::string dev;
    if (VERSION_DEV) dev = "-dev";
    return StringFormat("%d.%d.%d%s-%s", VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, dev.c_str(), GIT_COMMIT);
}

static const std::string base62Chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

std::string BaseEncodeInt(unsigned int value, unsigned int base)
{
    assert(base > 10);
    assert(base < 63);

    std::string base62;
    if (value < base) return std::string(1, base62Chars[value]);

    while (value) {
        base62 += base62Chars[value % base];
        value /= base;
    }

    reverse(base62.begin(), base62.end());
    return base62;
}

//----------------------------------------------------------------------------
// Base64 code borrowed
//----------------------------------------------------------------------------

/*
 base64.cpp and base64.h

 Copyright (C) 2004-2008 René Nyffenegger

 This source code is provided 'as-is', without any express or implied
 warranty. In no event will the author be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this source code must not be misrepresented; you must not
 claim that you wrote the original source code. If you use this source code
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original source code.

 3. This notice may not be removed or altered from any source distribution.

 René Nyffenegger rene.nyffenegger@adp-gmbh.ch

 */

// See also https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c

static const std::string base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                       "abcdefghijklmnopqrstuvwxyz"
                                       "0123456789+/";

static inline bool isBase64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string Base64Encode(unsigned char const *bytesToEncode, unsigned int inLen)
{
    std::string ret;
    int i = 0;
    unsigned char charArray3[3];
    unsigned char charArray4[4];

    while (inLen--) {
        charArray3[i++] = *(bytesToEncode++);
        if (i == 3) {
            charArray4[0] = (charArray3[0] & 0xfc) >> 2;
            charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
            charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
            charArray4[3] = charArray3[2] & 0x3f;

            for (i = 0; (i < 4); i++) ret += base64Chars[charArray4[i]];
            i = 0;
        }
    }

    if (i) {
        for (int j = i; j < 3; j++) charArray3[j] = '\0';

        charArray4[0] = (charArray3[0] & 0xfc) >> 2;
        charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
        charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
        charArray4[3] = charArray3[2] & 0x3f;

        for (int j = 0; (j < i + 1); j++) ret += base64Chars[charArray4[j]];

        while ((i++ < 3)) ret += '=';
    }

    return ret;
}

std::vector<unsigned char> Base64Decode(std::string const &encodedString)
{
    int inLen = (int)encodedString.size();
    int i = 0;
    int in_ = 0;
    unsigned char charArray4[4], charArray3[3];
    std::vector<unsigned char> ret;

    while (inLen-- && (encodedString[in_] != '=') && isBase64(encodedString[in_])) {
        charArray4[i++] = encodedString[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; ++i) {
                charArray4[i] = base64Chars.find(charArray4[i]);
            }

            charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
            charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
            charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

            for (i = 0; (i < 3); ++i) {
                ret.push_back(charArray3[i]);
            }
            i = 0;
        }
    }

    if (i) {
        for (int j = i; j < 4; ++j) {
            charArray4[j] = 0;
        }

        for (int j = 0; j < 4; ++j) {
            charArray4[j] = base64Chars.find(charArray4[j]);
        }

        charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
        charArray3[1] = ((charArray4[1] & 0xf) << 4) + ((charArray4[2] & 0x3c) >> 2);
        charArray3[2] = ((charArray4[2] & 0x3) << 6) + charArray4[3];

        for (int j = 0; (j < i - 1); ++j) ret.push_back(charArray3[j]);
    }

    return ret;
}

} // namespace vrv
