/////////////////////////////////////////////////////////////////////////////
// Name:        vrv.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "vrv.h"

//----------------------------------------------------------------------------

#include <cmath>
#include <stdarg.h>
#include <vector>

//----------------------------------------------------------------------------

#include "vrvdef.h"

//----------------------------------------------------------------------------

#define STRING_FORMAT_MAX_LEN 2048

namespace vrv {
        
// Initialize static members
std::string Resources::m_path = "/usr/local/share/verovio";
std::string Resources::m_musicFontDesc = "0;13;70;90;90;0;Leipzig 4.9;33";
std::string Resources::m_lyricFontDesc = "0;12;70;93;90;0;Garamond;0";
 
// Global for LogElapsedTimeXXX functions (debugging purposes)
struct timeval start;

#ifdef EMSCRIPTEN
    std::vector<std::string> _log_buffer;
#endif
    
bool AreEqual(double dFirstVal, double dSecondVal)
{
    return std::fabs(dFirstVal - dSecondVal) < 1E-3;
}

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
#ifdef EMSCRIPTEN
    std::string s;
    va_list args;
    va_start ( args, fmt );
    s = "[Debug] " + StringFormatVariable(fmt, args) + "\n";
    AppendLogBuffer(true, s);
    va_end ( args );
#else
#if defined(DEBUG)
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
    
#ifdef EMSCRIPTEN
void ResetLogBuffer() {
    _log_buffer.clear();
}
    
std::vector<std::string> GetLogBuffer() {
    return _log_buffer;
}
    
bool LogBufferContains(std::string s) {
    std::vector<std::string>::iterator iter = _log_buffer.begin();
    
    while ( iter != _log_buffer.end()) {
        std::string s2 = *iter;
        if (s2 == s)
            return true;
        ++iter;
    }
    
    return false;
}

void AppendLogBuffer(bool checkDuplicate, std::string message) {
    
    if (checkDuplicate) {
        if (!LogBufferContains(message))
            _log_buffer.push_back(message);
    } else
        _log_buffer.push_back(message);
    
}
    

#endif
    
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
    return StringFormat("%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION );
}

//----------------------------------------------------------------------------
// DefaultEnv
//----------------------------------------------------------------------------

DefaultEnv::DefaultEnv()
{
    
    m_interlDefin = DEFAULT_UNIT * 2;
    m_landscape = false;
    m_staffLineWidth = 2;
    m_stemWidth = 2;
    m_barlineWidth = 2;
    m_beamMaxSlope = 30;
    m_beamMinSlope = 10;
    
    // originally in WG Parameters2
    m_smallStaffNum = 3;
    m_smallStaffDen = 4;
    m_graceNum = 3;
    m_graceDen = 4;
    m_headerType = 0;
    
    m_pageHeight = DEFAULT_PAGE_HEIGHT;
    m_pageWidth = DEFAULT_PAGEWIDTH;
    m_pageRightMar = DEFAULT_PAGE_RIGHT_MAR;
    m_pageLeftMar = DEFAULT_PAGE_LEFT_MAR;
    m_pageTopMar = DEFAULT_PAGE_TOP_MAR;
    
    // additional parameters
    m_notationMode = MENSURAL_MODE;
    //m_notationMode = CMN_MODE;
}

DefaultEnv::~DefaultEnv()
{
}

} // namespace vrv