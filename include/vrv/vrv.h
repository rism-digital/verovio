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
#include <sys/time.h>
#include <vector>

namespace vrv {
    
class Object;
    
bool IsNote( Object *object );

/**
 * The following functions are helpers for formating, conversion, or loging
 * Most of them differ if they are used in the command line tool or in emscripten
 */
        
void LogDebug(  const char *fmt, ... );
void LogError(  const char *fmt, ... );
void LogMessage(  const char *fmt, ... );
void LogWarning(  const char *fmt, ... );
void DisableLog( );

/**
 * Member and functions specific to emscripten loging that uses a vector of string to buffer the logs.
 */
#ifdef EMSCRIPTEN
extern std::vector<std::string> logBuffer;
bool LogBufferContains(std::string s);
void AppendLogBuffer(bool checkDuplicate, std::string message);
#endif

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
// This is the implementation callable with variable arguments
std::string StringFormatVariable( const char * format, va_list arg );
    
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

/**
 *
 */
extern bool noLog;
 
/**
 * Functions for logging in milliseconds the elapsed time of an
 * operation (for debugging purposes).
 * LogElapsedTimeStart needs to be called before the operation
 * 
 * Ex:
 * 
 * LogElapsedTimeStart( );
 * ... Do something
 * LogElapsedTimeEnd( "name of the operation" );
 */
extern struct timeval start;
void LogElapsedTimeStart( );
void LogElapsedTimeEnd (const char *msg = "unspecified operation" );

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

    /**
     * @name Setters and getters for static environment variables
     */
    ///@{
    static std::string GetPath( ) { return m_path; };
    static void SetPath( std::string path ) { m_path = path; };
    static std::string GetMusicFontDescStr( ) { return m_musicFontDesc; };
    static void SetMusicFontDescStr( std::string lyricFontDesc ) { m_musicFontDesc = lyricFontDesc; };
    static std::string GetLyricFontDescStr( ) { return m_lyricFontDesc; };
    static void SetLyricFontDescStr( std::string lyricFontDesc ) { m_lyricFontDesc = lyricFontDesc; };
    ///@}
    
    
private:
    /** The path to the resources directory (e.g., for the svg/ subdirectory with fonts as XML */
    static std::string m_path;
    /** The FontInfo string for the music font */
    static std::string m_musicFontDesc;
    /** The FontInfo string for the default lyric font */
    static std::string m_lyricFontDesc;
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
    /** The barLine width */
    unsigned char m_barlineWidth;
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
    /** The header and footer type */
    unsigned int m_headerType;
    /** The notation mode (cmn or mensural) */
    int m_notationMode;
    /** The page height */
    int m_pageHeight;
    /** The page width */
    int m_pageWidth;
    /** The page left margin */
    short m_pageLeftMar;
    /** The page right margin */
    short m_pageRightMar;
    /** The page top margin */
    short m_pageTopMar;
    /** The staff minimal spacing */
    short m_spacingStaff;
    /** The system minimal spacing */
    short m_spacingSystem;
    
};

} // namespace vrv
    
#endif
