 /////////////////////////////////////////////////////////////////////////////
// Name:        vrv.h
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_H__
#define __VRV_H__

#include <cstring>
#include <stdio.h>
#include <string>

namespace vrv {

/**
 * The following functions are helpers for formating, conversion, or loggin
 * Most of them differ if they are used in Aruspix or in the command line tool
 */
        
    
void LogDebug(  const char *fmt, ... );
void LogError(  const char *fmt, ... );
void LogMessage(  const char *fmt, ... );
void LogWarning(  const char *fmt, ... );

/**
 * Utility for comparing doubles
 */
bool AreEqual(double dFirstVal, double dSecondVal);

/**
 * Utility for converting UTF16 (std::wstring) to UTF-8
 */
std::string UTF16to8(const wchar_t * in);

/**
 * Utility for converting UTF-8 to UTF16 (std::wstring)
 */
std::wstring UTF8to16(const char * in);
    
/**
 * Format a string using vsnprintf.
 * The maximum length is giving by STRING_FORMAT_MAX_LEN
 */
std::string StringFormat( const char *fmt, ... );
    
/**
 * Return a formatted version (####.####.####) of the file version.
 * This can be used for comparing if the file version is < or >
 */
std::string GetFileVersion(int vmaj, int vmin, int vrev);
 
/**
 * Return a the filename (without extension) extracted from the fullpath
 */
std::string GetFilename( std::string fullpath );

/**
 * Return the version number (X.X.X)
 */
std::string GetVersion();

//----------------------------------------------------------------------------
// Resources
//----------------------------------------------------------------------------

/**
 * This class provides static resource values.
 * The default values can be changed by setters.
 */

class Resources
{
public:
    // Some statis utiliry functions
    static std::string GetPath() { return m_path; };
    static void SetPath(std::string path) {m_path = path;};
    static std::string GetMusicFontDescStr() { return m_musicFontDesc; };
    static void GetMusicFontDescStr(std::string lyricFontDesc) {m_musicFontDesc = lyricFontDesc;};
    static std::string GetLyricFontDescStr() { return m_lyricFontDesc; };
    static void GetLyricFontDescStr(std::string lyricFontDesc) {m_lyricFontDesc = lyricFontDesc;};
    static int GetFontPosCorrection() { return m_fontPosCorrection; };
    static void GetFontPosCorrection(int fontPosCorrection) {m_fontPosCorrection = fontPosCorrection;};
    
private:
    static std::string m_path;
    static std::string m_musicFontDesc;
    static std::string m_lyricFontDesc;
    static int m_fontPosCorrection;
};


//----------------------------------------------------------------------------
// DefaultEnv
//----------------------------------------------------------------------------

/** 
 * This class contains the document default environment variables.
 * Some of them are not available as is in MEI - to be solved
 */
class DefaultEnv 
{
public:
    // constructors and destructors
    DefaultEnv();
    virtual ~DefaultEnv();
    
    
public:
    /** The reference interline definition  */
    int m_interlDefin;
    /** The landscape paper orientation flag */
    char m_landscape;
    /** The staff line width */
    unsigned char m_staffLineWidth;
    /** The stem width */
    unsigned char m_stemWidth;
    /** The barline width */
    unsigned char m_barlineWidth;
    /** The beam width */
    unsigned char m_beamWidth;
    /** The beam white width */
    unsigned char m_beamWhiteWidth;
    /** The maximum beam slope */
    unsigned char m_beamMaxSlope;
    /** The minimum beam slope */
    unsigned char m_beamMinSlope;     
    /** The small staff size ratio numerator */
    unsigned char m_smallStaffNum;
    /** The small staff size ratio denominator */
    unsigned char m_smallStaffDen;
    /** The grace size ratio numerator */
    unsigned char m_graceNum;
    /** The grace size ratio denominator */
    unsigned char m_graceDen;
    /** The stem position correction */
    signed char m_stemCorrection;
    /** The header and footer type */
    unsigned int m_headerType;
    /** The notation mode (cmn or mensural) */
    int m_notationMode;
    /** The current page height */
    int m_pageHeight;
    /** The current page height */
    int m_pageWidth;
    /** The current page witdth */
    int m_pageLeftMar;
    /** The current page left margin */
    int m_pageRightMar;
    /** The current page right margin */
    int m_pageTopMar;
};

} // namespace vrv
    
#endif
