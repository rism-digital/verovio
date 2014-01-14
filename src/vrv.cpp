/////////////////////////////////////////////////////////////////////////////
// Name:        mus.cpp
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "vrv.h"

//----------------------------------------------------------------------------

#include <cmath>
#include <stdarg.h>

//----------------------------------------------------------------------------

#include "vrvdef.h"

//----------------------------------------------------------------------------

// AX_APP is define is the Aruspix wxWidget application only
#ifdef AX_APP
#include "wx/wxprec.h"
#include "app/axapp.h"
#endif

namespace vrv {

// Initialize static respource path
std::string Vrv::m_respath = "/usr/local/share/aruspix";

//----------------------------------------------------------------------------
// Vrv
//----------------------------------------------------------------------------


bool Vrv::AreEqual(double dFirstVal, double dSecondVal)
{
    return std::fabs(dFirstVal - dSecondVal) < 1E-3;
}

std::string Vrv::GetAxVersion() {
#ifndef AX_APP
    return std::string("command line"); // we need to add versioning
#else
    return AxApp::s_version.mb_str();
#endif
}

std::string Vrv::GetResourcesPath() {
#ifndef AX_APP
    //hardcode galore
    return m_respath;
#else
    return wxGetApp().m_resourcesPath.mb_str();
#endif
}

std::string Vrv::GetMusicFontDescStr() {
#ifndef AX_APP
    return std::string("0;13;70;90;90;0;Leipzig 4.7;33");
#else
    return wxGetApp().m_musicFontDesc.mb_str();
#endif
}

std::string Vrv::GetLyricFontDescStr() {
#ifndef AX_APP
    return std::string("0;12;70;93;90;0;Garamond;0");
#else
    return wxGetApp().m_lyricFontDesc.mb_str();
#endif
}


int Vrv::GetFontPosCorrection(){
#ifndef AX_APP
    return 0;
#else
    return wxGetApp().m_fontPosCorrection;
#endif
}

std::string Vrv::GetFileVersion(int vmaj, int vmin, int vrev) {
    return Vrv::StringFormat("%04d.%04d.%04d", vmaj, vmin, vrev );
}

std::string Vrv::StringFormat(const char *fmt, ...)
{
    std::string str( 2048, 0 );
    va_list args;
    va_start ( args, fmt );
    vsnprintf ( &str[0], 2048, fmt, args );
    va_end ( args );
    str.resize( strlen( str.data() ) );
    return str;
}

std::string Vrv::GetFilename( std::string fullpath )
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

void Vrv::LogDebug(const char *fmt, ...)
{
#ifdef EMSCRIPTEN
    return;
#else
    va_list args;
    va_start ( args, fmt );
#ifndef AX_APP
    #if defined(DEBUG)
    printf("[Debug] ");
    vprintf( fmt, args );
    printf("\n");
    #endif
#else
    wxVLogDebug( fmt, args );
#endif
    va_end ( args );
#endif
}

void Vrv::LogError(const char *fmt, ...)
{
#ifdef EMSCRIPTEN
    return;
#else
    va_list args;
    va_start ( args, fmt );
#ifndef AX_APP
    printf("[Error] ");
    vprintf( fmt, args );
    printf("\n");
#else
    wxVLogError( fmt, args );
#endif
    va_end ( args );
#endif
}

void Vrv::LogMessage(const char *fmt, ...)
{
#ifdef EMSCRIPTEN
    return;
#else
    va_list args;
    va_start ( args, fmt );
#ifndef AX_APP
    printf("[Message] ");
    vprintf( fmt, args );
    printf("\n");
#else
    wxVLogMessage( fmt, args );
#endif
    va_end ( args );
#endif
}

void Vrv::LogWarning(const char *fmt, ...)
{
#ifdef EMSCRIPTEN
    return;
#else
    va_list args;
    va_start ( args, fmt );
#ifndef AX_APP
    printf("[Warning] ");
    vprintf( fmt, args );
    printf("\n");
#else
    wxVLogWarning( fmt, args );
#endif
    va_end ( args );
#endif
}

std::string Vrv::UTF16to8(const wchar_t * in)
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

std::wstring Vrv::UTF8to16(const char * in)
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

//----------------------------------------------------------------------------
// MusEnv
//----------------------------------------------------------------------------

MusEnv::MusEnv()
{
    
    m_interlDefin = 18;
    m_landscape = false;
    m_staffLineWidth = 2;
    m_stemWidth = 2;
    m_barlineWidth = 2;
    m_beamWidth = 8;
    m_beamWhiteWidth = 6;
    m_beamMaxSlope = 30;
    m_beamMinSlope = 10;
    
    // originally in MusParameters2
    m_smallStaffNum = 16;
    m_smallStaffDen = 20;
    m_graceNum = 3;
    m_graceDen = 4;
	m_stemCorrection = 0;
    m_headerType = 0;
    
    m_pageHeight = 2970;
    m_pageWidth = 2100;
    m_pageRightMar = 0;
    m_pageLeftMar = 0;
    m_pageTopMar = 0;
    
    // additional parameters
    m_notationMode = MUS_MENSURAL_MODE;
    //m_notationMode = MUS_CMN_MODE;
}

MusEnv::~MusEnv()
{
}

} // namespace vrv