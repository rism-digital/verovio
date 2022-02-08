/////////////////////////////////////////////////////////////////////////////
// Name:        vrv.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "vrv.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
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

#include "glyph.h"
#include "smufl.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include "checked.h"
#include "pugixml.hpp"
#include "unchecked.h"

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
// Static members with some default values
//----------------------------------------------------------------------------

thread_local std::string Resources::s_path = "/usr/local/share/verovio";
thread_local std::string Resources::s_smuflFontName = "";
thread_local Resources::GlyphTextMap Resources::s_textFont;
thread_local Resources::GlyphTable Resources::s_fontGlyphTable;
thread_local Resources::GlyphNameTable Resources::s_glyphNameTable;
thread_local Resources::StyleAttributes Resources::s_currentStyle;
const Resources::StyleAttributes Resources::k_defaultStyle{ data_FONTWEIGHT::FONTWEIGHT_normal,
    data_FONTSTYLE::FONTSTYLE_normal };

//----------------------------------------------------------------------------
// Font related methods
//----------------------------------------------------------------------------

bool Resources::InitFonts()
{
    // We will need to rethink this for adding the option to add custom fonts
    // Font Bravura first since it is expected to have always all symbols
    if (!LoadFont("Bravura")) LogError("Bravura font could not be loaded.");
    // The Leipzig as the default font
    if (!LoadFont("Leipzig")) LogError("Leipzig font could not be loaded.");

    struct TextFontInfo_type {
        const StyleAttributes m_style;
        const std::string m_fileName;
        bool m_isMandatory;
    };

    static const TextFontInfo_type textFontInfos[] = { { k_defaultStyle, "Times", true },
        { k_defaultStyle, "VerovioText-1.0", true }, { { FONTWEIGHT_bold, FONTSTYLE_normal }, "Times-bold", false },
        { { FONTWEIGHT_bold, FONTSTYLE_normal }, "VerovioText-1.0", false },
        { { FONTWEIGHT_bold, FONTSTYLE_italic }, "Times-bold-italic", false },
        { { FONTWEIGHT_bold, FONTSTYLE_italic }, "VerovioText-1.0", false },
        { { FONTWEIGHT_normal, FONTSTYLE_italic }, "Times-italic", false },
        { { FONTWEIGHT_normal, FONTSTYLE_italic }, "VerovioText-1.0", false } };

    for (const auto &textFontInfo : textFontInfos) {
        if (!InitTextFont(textFontInfo.m_fileName, textFontInfo.m_style) && textFontInfo.m_isMandatory) {
            LogError("Text font could not be initialized.");
            return false;
        }
    }

    s_currentStyle = k_defaultStyle;

    return true;
}

bool Resources::SetFont(const std::string &fontName)
{
    return LoadFont(fontName);
}

Glyph *Resources::GetGlyph(wchar_t smuflCode)
{
    return s_fontGlyphTable.count(smuflCode) ? &s_fontGlyphTable.at(smuflCode) : NULL;
}

Glyph *Resources::GetGlyph(const std::string &smuflName)
{
    return s_glyphNameTable.count(smuflName) ? &s_fontGlyphTable.at(s_glyphNameTable.at(smuflName)) : NULL;
}

wchar_t Resources::GetGlyphCode(const std::string &smuflName)
{
    return s_glyphNameTable.count(smuflName) ? s_glyphNameTable.at(smuflName) : 0;
}

void Resources::SelectTextFont(data_FONTWEIGHT fontWeight, data_FONTSTYLE fontStyle)
{
    if (fontWeight == FONTWEIGHT_NONE) {
        fontWeight = FONTWEIGHT_normal;
    }

    if (fontStyle == FONTSTYLE_NONE) {
        fontStyle = FONTSTYLE_normal;
    }

    s_currentStyle = { fontWeight, fontStyle };
    if (s_textFont.count(s_currentStyle) == 0) {
        LogWarning("Text font for style (%d, %d) is not loaded. Use default", fontWeight, fontStyle);
        s_currentStyle = k_defaultStyle;
    }
}

Glyph *Resources::GetTextGlyph(wchar_t code)
{
    GlyphTable *currentTable = &s_textFont[s_textFont.count(s_currentStyle) != 0 ? s_currentStyle : k_defaultStyle];

    if (currentTable->count(code) == 0) {
        return NULL;
    }

    return &currentTable->at(code);
}

bool Resources::LoadFont(const std::string &fontName)
{
    // set font name
    s_smuflFontName = fontName;

    pugi::xml_document doc;
    const std::string filename = Resources::GetPath() + "/" + fontName + ".xml";
    pugi::xml_parse_result parseResult = doc.load_file(filename.c_str());
    if (!parseResult) {
        // File not found, default bounding boxes will be used
        LogError("Failed to load font and glyph bounding boxes");
        return false;
    }
    pugi::xml_node root = doc.first_child();
    if (!root.attribute("units-per-em")) {
        LogError("No units-per-em attribute in bouding box file");
        return false;
    }

    const int unitsPerEm = atoi(root.attribute("units-per-em").value());

    for (pugi::xml_node current = root.child("g"); current; current = current.next_sibling("g")) {
        pugi::xml_attribute c_attribute = current.attribute("c");
        pugi::xml_attribute n_attribute = current.attribute("n");
        if (!c_attribute || !n_attribute) continue;

        Glyph glyph;
        glyph.SetUnitsPerEm(unitsPerEm * 10);
        glyph.SetCodeStr(c_attribute.value());
        float x = 0.0, y = 0.0, width = 0.0, height = 0.0;
        if (current.attribute("x")) x = current.attribute("x").as_float();
        if (current.attribute("y")) y = current.attribute("y").as_float();
        if (current.attribute("w")) width = current.attribute("w").as_float();
        if (current.attribute("h")) height = current.attribute("h").as_float();
        glyph.SetBoundingBox(x, y, width, height);
        if (current.attribute("h-a-x")) glyph.SetHorizAdvX(current.attribute("h-a-x").as_float());

        // load anchors
        pugi::xml_node anchor;
        for (anchor = current.child("a"); anchor; anchor = anchor.next_sibling("a")) {
            if (anchor.attribute("n")) {
                std::string name = std::string(anchor.attribute("n").value());
                // No check for possible x and y missing attributes - not very safe.

                glyph.SetAnchor(name, anchor.attribute("x").as_float(), anchor.attribute("y").as_float());
            }
        }

        const wchar_t smuflCode = (wchar_t)strtol(c_attribute.value(), NULL, 16);
        s_fontGlyphTable[smuflCode] = glyph;
        s_glyphNameTable[n_attribute.value()] = smuflCode;
    }

    return true;
}

bool Resources::InitTextFont(const std::string &fontName, const StyleAttributes &style)
{
    // For the text font, we load the bounding boxes only
    pugi::xml_document doc;
    // For now, we have only Times bounding boxes for ASCII chars
    // For any other char, we currently use 'o' bounding box
    std::string filename = Resources::GetPath() + "/text/" + fontName + ".xml";
    pugi::xml_parse_result result = doc.load_file(filename.c_str());
    if (!result) {
        // File not found, default bounding boxes will be used
        LogMessage("Cannot load bounding boxes for text font '%s'", filename.c_str());
        return false;
    }
    pugi::xml_node root = doc.first_child();
    if (!root.attribute("units-per-em")) {
        LogWarning("No units-per-em attribute in bouding box file");
        return false;
    }
    const int unitsPerEm = root.attribute("units-per-em").as_int();
    pugi::xml_node current;
    if (s_textFont.count(style) == 0) {
        s_textFont[style] = {};
    }
    GlyphTable &currentTable = s_textFont.at(style);
    for (current = root.child("g"); current; current = current.next_sibling("g")) {
        if (current.attribute("c")) {
            wchar_t code = (wchar_t)strtol(current.attribute("c").value(), NULL, 16);
            // We create a glyph with only the units per em which is the only info we need for
            // the bounding boxes; path and codeStr will remain [unset]
            Glyph glyph(unitsPerEm);
            float x = 0.0, y = 0.0, width = 0.0, height = 0.0;
            // Not check for missing values...
            if (current.attribute("x")) x = current.attribute("x").as_float();
            if (current.attribute("y")) y = current.attribute("y").as_float();
            if (current.attribute("w")) width = current.attribute("w").as_float();
            if (current.attribute("h")) height = current.attribute("h").as_float();
            glyph.SetBoundingBox(x, y, width, height);

            if (current.attribute("h-a-x")) glyph.SetHorizAdvX(current.attribute("h-a-x").as_float());
            if (currentTable.count(code) > 0) {
                LogDebug("Redefining %d with %s", code, fontName.c_str());
            }
            currentTable[code] = glyph;
        }
    }
    return true;
}

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

std::string ExtractUuidFragment(std::string refUuid)
{
    size_t pos = refUuid.find_last_of("#");
    if ((pos != std::string::npos) && (pos < refUuid.length() - 1)) {
        refUuid = refUuid.substr(pos + 1);
    }
    return refUuid;
}

std::string UTF16to8(const std::wstring &in)
{
    std::string out;

    utf8::utf16to8(in.begin(), in.end(), std::back_inserter(out));

    return out;
}

std::wstring UTF8to16(const std::string &in)
{
    std::wstring out;
    std::stringstream sin(in.c_str());

    utf8::utf8to16(std::istreambuf_iterator<char>(sin), std::istreambuf_iterator<char>(), std::back_inserter(out));

    return out;
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

std::string BaseEncodeInt(int value, int base)
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
