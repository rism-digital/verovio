/////////////////////////////////////////////////////////////////////////////
// Name:        style.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_STYLE_H__
#define __VRV_STYLE_H__

namespace vrv {
    
//----------------------------------------------------------------------------
// Default layout values
//----------------------------------------------------------------------------
    
#define DEFINITON_FACTOR 10
#define MARGIN_DENOMINATOR 10

#define DEFAULT_UNIT 9
#define MIN_UNIT 6
#define MAX_UNIT 18
    
#define DEFAULT_PAGE_RIGHT_MAR 50
#define MIN_PAGE_RIGHT_MAR 0
#define MAX_PAGE_RIGHT_MAR 500
    
#define DEFAULT_PAGE_LEFT_MAR 50
#define MIN_PAGE_LEFT_MAR 0
#define MAX_PAGE_LEFT_MAR 500
    
#define DEFAULT_PAGE_TOP_MAR 50
#define MIN_PAGE_TOP_MAR 0
#define MAX_PAGE_TOP_MAR 500
    
#define DEFAULT_PAGE_HEIGHT 2970
#define MIN_PAGE_HEIGHT 100
#define MAX_PAGE_HEIGHT 60000
    
#define DEFAULT_PAGE_WIDTH 2100
#define MIN_PAGE_WIDTH 100
#define MAX_PAGE_WIDTH 60000
    
#define MIN_TIE_HEIGHT 12
#define MIN_TIE_THICKNESS 6
    
//----------------------------------------------------------------------------
// Default scaling (%) and spacing (units) values
//----------------------------------------------------------------------------
    
#define DEFAULT_SCALE 100
#define MIN_SCALE 1
#define MAX_SCALE 1000
    
#define DEFAULT_SPACING_STAFF 8
#define MIN_SPACING_STAFF 0
#define MAX_SPACING_STAFF 12
    
#define DEFAULT_SPACING_SYSTEM 0
#define MIN_SPACING_SYSTEM 0
#define MAX_SPACING_SYSTEM 12
    
//----------------------------------------------------------------------------
// Style
//----------------------------------------------------------------------------

/**
 * This class contains the document default environment variables.
 * Some of them are not available as is in MEI - to be solved
 */
class Style
{
public:
    // constructors and destructors
    Style();
    virtual ~Style();
    
    
public:
    /** The unit (1Ú2 of the distance between staff lines) **/
    int m_unit;
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

#endif // __VRV_DEF_H__