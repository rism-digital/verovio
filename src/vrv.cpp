/////////////////////////////////////////////////////////////////////////////
// Name:        vrv.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "vrv.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <dirent.h>
#include <cmath>
#include <stdarg.h>
#include <stdlib.h>
#include <vector>

//----------------------------------------------------------------------------

#include "git_commit.h"
#include "glyph.h"
#include "pugixml.hpp"
#include "smufl.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#define STRING_FORMAT_MAX_LEN 2048

namespace vrv {
   
//----------------------------------------------------------------------------
// Static members with some default values
//----------------------------------------------------------------------------
    
std::string Resources::m_path = "/usr/local/share/verovio";
std::map<wchar_t, Glyph> Resources::m_font;
std::map<wchar_t, Glyph> Resources::m_textFont;
  
//----------------------------------------------------------------------------
// Font related methods
//----------------------------------------------------------------------------
    
bool Resources::InitFonts()
{
    // We will need to rethink this for adding the option to add custom fonts
    // Font Bravura first since it is expected to have always all symbols
    if (!LoadFont("Bravura")) LogError("Bravura font could not be loaded");
    // The Leipzig as the default font
    if (!LoadFont("Leipzig")) LogError("Leipzig font could not be loaded");
    
    if ( m_font.size() < SMUFL_COUNT ) {
        LogError("All default SMUFL glyphs could not be loaded");
        return false;
    }
    
    if ( !InitTextFont() ) {
        LogError("Text font could not be initialized");
        return false;
    }
    
    return true;
}
    
bool Resources::SetFont(std::string fontName)
{
    return LoadFont(fontName);
}

Glyph *Resources::GetGlyph(wchar_t smuflCode)
{
    if (!m_font.count(smuflCode)) return NULL;
    return &m_font[smuflCode];
}

Glyph *Resources::GetTextGlyph(wchar_t code)
{
    if (!m_textFont.count(code)) return NULL;
    return &m_textFont[code];
}
    
bool Resources::LoadFont(std::string fontName)
{
    DIR*    dir;
    dirent* pdir;
    std::string dirname =  Resources::GetPath() + "/" + fontName;
    dir = opendir(dirname.c_str());
    
    if (!dir) {
        LogError("Font directory '%s' cannot be read", dirname.c_str());
        return false;
    }
    
    // First loop through the fontName directory and load each glyph
    // Since the filename starts with the Unicode code, it is used
    // to assign the glyph to the corresponding position in m_fonts
    while ((pdir = readdir(dir))) {
        if ( strstr( pdir->d_name, ".xml" )) {
            // E.g, : E053-gClef8va.xml => strtol extracts E053 as hex
            wchar_t smuflCode = (wchar_t)strtol( pdir->d_name, NULL, 16);
            if (smuflCode == 0) {
                LogError("Invalid SMUFL code (0)");
                continue;
            }
            std::string codeStr = pdir->d_name;
            codeStr = codeStr.substr(0, 4);
            Glyph glyph( Resources::GetPath() + "/" + fontName + "/" + pdir->d_name , codeStr );
            m_font[smuflCode] = glyph;
        }
    }
    
    closedir(dir);
    
    
    // Then load the bounding boxes (if bounding box file is provided)
    pugi::xml_document doc;
    std::string filename = Resources::GetPath() + "/" + fontName + ".xml";
    pugi::xml_parse_result result = doc.load_file( filename.c_str() );
    if (!result)
    {
        // File not found, default bounding boxes will be used
        LogMessage("Font loaded without bounding boxes");
        return true;
    }
    pugi::xml_node root = doc.first_child();
    if (!root.attribute("units-per-em")) {
        LogWarning("No units-per-em attribute in bouding box file");
        return true;
    }
    int unitsPerEm = atoi(root.attribute("units-per-em").value());
    pugi::xml_node current;
    for( current = root.child("glyph"); current; current = current.next_sibling("glyph") ) {
        if ( current.attribute( "glyph-code" ) ) {
            wchar_t smuflCode = (wchar_t)strtol( current.attribute( "glyph-code" ).value(), NULL, 16);
            if (!m_font.count(smuflCode)) {
                LogWarning("Glyph with code '%d' not found.", smuflCode);
                continue;
            }
            Glyph *glyph = &m_font[smuflCode];
            if (glyph->GetUnitsPerEm() != unitsPerEm * 10) {
                LogWarning("Glyph and bounding box units-per-em for code '%d' miss-match (bounding box: %d)", smuflCode, unitsPerEm);
                continue;
            }
            double x = 0.0, y = 0.0, width = 0.0, height = 0.0;
            // Not check for missing values...
            if ( current.attribute( "x" ) ) x = atof( current.attribute( "x" ).value() );
            if ( current.attribute( "y" ) ) y = atof( current.attribute( "y" ).value() );
            if ( current.attribute( "width" ) ) width = atof( current.attribute( "width" ).value() );
            if ( current.attribute( "height" ) ) height = atof( current.attribute( "height" ).value() );
            glyph->SetBoundingBox(x, y, width, height);
        }
    }
    
    return true;
}

    
bool Resources::InitTextFont()
{
    // For the text font, we load the bounding boxes only
    pugi::xml_document doc;
    // For now, we have only Georgia.xml bounding boxes for ASCII chars
    // For any other char, we currently use 'o' bounding box
    std::string filename = Resources::GetPath() + "/text/Georgia.xml";
    pugi::xml_parse_result result = doc.load_file( filename.c_str() );
    if (!result)
    {
        // File not found, default bounding boxes will be used
        LogMessage("Cannot load bounding boxes for text font '%s'", filename.c_str());
        return false;
    }
    pugi::xml_node root = doc.first_child();
    if (!root.attribute("units-per-em")) {
        LogWarning("No units-per-em attribute in bouding box file");
        return false;
    }
    int unitsPerEm = atoi(root.attribute("units-per-em").value());
    pugi::xml_node current;
    for( current = root.child("glyph"); current; current = current.next_sibling("glyph") ) {
        if ( current.attribute( "glyph-code" ) ) {
            wchar_t code = (wchar_t)strtol( current.attribute( "glyph-code" ).value(), NULL, 16);
            // We create a glyph with only the units per em which is the only info we need for
            // the bounding boxes; path and codeStr will remain [unset]
            Glyph glyph( unitsPerEm );
            double x = 0.0, y = 0.0, width = 0.0, height = 0.0;
            // Not check for missing values...
            if ( current.attribute( "x" ) ) x = atof( current.attribute( "x" ).value() );
            if ( current.attribute( "y" ) ) y = atof( current.attribute( "y" ).value() );
            if ( current.attribute( "width" ) ) width = atof( current.attribute( "width" ).value() );
            if ( current.attribute( "height" ) ) height = atof( current.attribute( "height" ).value() );
            glyph.SetBoundingBox(x, y, width, height);
            m_textFont[code] = glyph;
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
bool noLog = false;
    
#ifdef EMSCRIPTEN
std::vector<std::string> logBuffer;
#endif

void LogElapsedTimeStart( )
{
    gettimeofday(&start, NULL);
}
    
void LogElapsedTimeEnd( const char *msg )
{
    double elapsedTime;
    struct timeval end;
    gettimeofday(&end, NULL);
    elapsedTime = (end.tv_sec - start.tv_sec) * 1000.0;  // sec to ms
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000.0;   // us to ms
    LogMessage("Elapsed time (%s): %.3fs", msg, elapsedTime / 1000 );
}
    
void LogDebug(const char *fmt, ...)
{
    if (noLog) return;
#if defined(DEBUG)
#ifdef EMSCRIPTEN
    std::string s;
    va_list args;
    va_start ( args, fmt );
    s = "[Debug] " + StringFormatVariable(fmt, args) + "\n";
    AppendLogBuffer(true, s);
    va_end ( args );
#else
    va_list args;
    va_start ( args, fmt );
    printf("[Debug] ");
    vprintf( fmt, args );
    printf("\n");
    va_end ( args );
#endif
#endif
}

void LogError(const char *fmt, ...)
{
    if (noLog) return;
#ifdef EMSCRIPTEN
    std::string s;
    va_list args;
    va_start ( args, fmt );
    s = "[Error] " + StringFormatVariable(fmt, args) + "\n";
    AppendLogBuffer(true, s);
    va_end ( args );
#else
    va_list args;
    va_start ( args, fmt );
    printf("[Error] ");
    vprintf( fmt, args );
    printf("\n");
    va_end ( args );
#endif
}

void LogMessage(const char *fmt, ...)
{
    if (noLog) return;
#ifdef EMSCRIPTEN
    std::string s;
    va_list args;
    va_start ( args, fmt );
    s = "[Message] " + StringFormatVariable(fmt, args) + "\n";
    AppendLogBuffer(true, s);
    va_end ( args );
#else
    va_list args;
    va_start ( args, fmt );
    printf("[Message] ");
    vprintf( fmt, args );
    printf("\n");
    va_end ( args );
#endif
}

void LogWarning(const char *fmt, ...)
{
    if (noLog) return;
#ifdef EMSCRIPTEN
    std::string s;
    va_list args;
    va_start ( args, fmt );
    s = "[Warning] " + StringFormatVariable(fmt, args) + "\n";
    AppendLogBuffer(true, s);
    va_end ( args );
#else
    va_list args;
    va_start ( args, fmt );
    printf("[Warning] ");
    vprintf( fmt, args );
    printf("\n");
    va_end ( args );
#endif
}
    
void DisableLog( )
{
    noLog = true;
}
    
#ifdef EMSCRIPTEN
bool LogBufferContains(std::string s)
{
    std::vector<std::string>::iterator iter = logBuffer.begin();
    while (iter != logBuffer.end()) {
        if ((*iter) == s)
            return true;
        ++iter;
    }
    return false;
}

void AppendLogBuffer(bool checkDuplicate, std::string message)
{    
    if (checkDuplicate) {
        if (!LogBufferContains(message))
            logBuffer.push_back(message);
    } else {
        logBuffer.push_back(message);
    }
}
#endif
    
bool Check( Object *object ) {
    assert( object );
    return ( object != NULL );
}
    
//----------------------------------------------------------------------------
// Various helpers
//----------------------------------------------------------------------------

std::string StringFormat(const char *fmt, ...)
{
    std::string str( STRING_FORMAT_MAX_LEN, 0 );
    va_list args;
    va_start ( args, fmt );
    vsnprintf ( &str[0], STRING_FORMAT_MAX_LEN,fmt, args );
    va_end ( args );
    str.resize( strlen( str.data() ) );
    return str;
}

std::string StringFormatVariable(const char * format, va_list arg)
{
    std::string str( STRING_FORMAT_MAX_LEN, 0 );
    vsnprintf ( &str[0], STRING_FORMAT_MAX_LEN, format, arg );
    str.resize( strlen( str.data() ) );
    return str;
}
    
bool AreEqual(double dFirstVal, double dSecondVal)
{
    return std::fabs(dFirstVal - dSecondVal) < 1E-3;
}
    
std::string UTF16to8(const wchar_t * in)
{
    std::string out;
    unsigned int codepoint = 0;
    for (;  *in != 0;  ++in)
    {
        if (*in >= 0xd800 && *in <= 0xdbff)
            codepoint = ((*in - 0xd800) << 10) + 0x10000;
        else
        {
            if (*in >= 0xdc00 && *in <= 0xdfff)
                codepoint |= *in - 0xdc00;
            else
                codepoint = *in;
            
            if (codepoint <= 0x7f)
                out.append(1, static_cast<char>(codepoint));
            else if (codepoint <= 0x7ff)
            {
                out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            else if (codepoint <= 0xffff)
            {
                out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            else
            {
                out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
            }
            codepoint = 0;
        }
    }
    return out;
}

std::wstring UTF8to16(const char * in)
{
    std::wstring out;
    if (in == NULL)
        return out;
    
    unsigned int codepoint;
    while (*in != 0)
    {
        unsigned char ch = static_cast<unsigned char>(*in);
        if (ch <= 0x7f)
            codepoint = ch;
        else if (ch <= 0xbf)
            codepoint = (codepoint << 6) | (ch & 0x3f);
        else if (ch <= 0xdf)
            codepoint = ch & 0x1f;
        else if (ch <= 0xef)
            codepoint = ch & 0x0f;
        else
            codepoint = ch & 0x07;
        ++in;
        if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
        {
            if (codepoint > 0xffff)
            {
                out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
                out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
            }
            else if (codepoint < 0xd800 || codepoint >= 0xe000)
                out.append(1, static_cast<wchar_t>(codepoint));
        }
    }
    return out;
}
    
    
std::string GetFileVersion(int vmaj, int vmin, int vrev) {
    return StringFormat("%04d.%04d.%04d", vmaj, vmin, vrev );
}
    
    
std::string GetFilename( std::string fullpath )
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
    
std::string GetVersion() {
    return StringFormat("%d.%d.%d-%s", VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, GIT_COMMIT );
}

} // namespace vrv