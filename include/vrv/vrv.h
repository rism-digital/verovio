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

class Glyph;
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
 * Extract the uuid from a any uri string
 */
std::string ExtractUuidFragment(std::string refUuid);

/**
 * Utility for converting UTF16 (std::wstring) to UTF-8
 */
std::string UTF16to8(const std::wstring &in);

/**
 * Utility for converting UTF-8 to UTF16 (std::wstring)
 */
std::wstring UTF8to16(const std::string &in);

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
std::string BaseEncodeInt(int value, int base);

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
// Resources
//----------------------------------------------------------------------------

/**
 * This class provides resource values.
 * It manages fonts and glyph tables.
 */

class Resources {
public:
    using StyleAttributes = std::pair<data_FONTWEIGHT, data_FONTSTYLE>;
    using GlyphTable = std::unordered_map<wchar_t, Glyph>;
    using GlyphNameTable = std::unordered_map<std::string, wchar_t>;
    using GlyphTextMap = std::map<StyleAttributes, GlyphTable>;

    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    Resources();
    virtual ~Resources() = default;
    ///@}

    /**
     * @name Setters and getters
     */
    ///@{
    static std::string GetDefaultPath() { return s_defaultPath; }
    static void SetDefaultPath(const std::string &path) { s_defaultPath = path; }

    std::string GetPath() const { return m_path; }
    void SetPath(const std::string &path) { m_path = path; }
    ///@}

    /**
     * Font initialization
     */
    ///@{
    /** Init the SMufL music and text fonts */
    bool InitFonts();
    /** Init the text font (bounding boxes and ASCII only) */
    bool InitTextFont(const std::string &fontName, const StyleAttributes &style);
    /** Select a particular font */
    bool SetFont(const std::string &fontName);
    ///@}

    /**
     * Retrieving glyphs
     */
    ///@{
    /** Returns the glyph (if exists) for a glyph code in the current SMuFL font */
    const Glyph *GetGlyph(wchar_t smuflCode) const;
    /** Returns the glyph (if exists) for a glyph name in the current SMuFL font */
    const Glyph *GetGlyph(const std::string &smuflName) const;
    /** Returns the glyph (if exists) for a glyph name in the current SMuFL font */
    wchar_t GetGlyphCode(const std::string &smuflName) const;
    ///@}

    /**
     * Text fonts
     */
    ///@{
    /** Set current text style*/
    void SelectTextFont(data_FONTWEIGHT fontWeight, data_FONTSTYLE fontStyle) const;
    /** Returns the glyph (if exists) for the text font (bounding box and ASCII only) */
    const Glyph *GetTextGlyph(wchar_t code) const;
    ///@}

private:
    bool LoadFont(const std::string &fontName);

private:
    /** The path to the resources directory (e.g., for the svg/ subdirectory with fonts as XML */
    std::string m_path;
    /** The loaded SMuFL font */
    GlyphTable m_fontGlyphTable;
    /** A text font used for bounding box calculations */
    GlyphTextMap m_textFont;
    mutable StyleAttributes m_currentStyle;
    /**
     * A map of glyph name / code
     */
    GlyphNameTable m_glyphNameTable;

    //----------------//
    // Static members //
    //----------------//

    /** The default path to the resources directory (e.g., for the svg/ subdirectory with fonts as XML */
    static thread_local std::string s_defaultPath;

    /** The default font style */
    static const StyleAttributes k_defaultStyle;
};

//----------------------------------------------------------------------------
// Base64 code borrowed
//----------------------------------------------------------------------------

std::string Base64Encode(unsigned char const *bytesToEncode, unsigned int len);
std::vector<unsigned char> Base64Decode(std::string const &encodedString);

} // namespace vrv

#endif
