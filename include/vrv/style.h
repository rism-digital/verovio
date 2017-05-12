/////////////////////////////////////////////////////////////////////////////
// Name:        style.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_STYLE_H__
#define __VRV_STYLE_H__

#ifdef CUSTOM_VEROVIO_STYLE
#include "custom_style.h"
#else

#include <map>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "atttypes.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Default layout values
//----------------------------------------------------------------------------


#define DEFAULT_SPACING_LINEAR 0.25
#define MIN_SPACING_LINEAR 0.0
#define MAX_SPACING_LINEAR 1.0

#define DEFAULT_SPACING_NON_LINEAR 0.6
#define MIN_SPACING_NON_LINEAR 0.0
#define MAX_SPACING_NON_LINEAR 1.0

//----------------------------------------------------------------------------
// Default scaling (%) and spacing (units) values
//----------------------------------------------------------------------------

#define DEFAULT_SCALE 100
#define MIN_SCALE 1
#define MAX_SCALE 1000

/** The default position at the beginning of a measure */
#define DEFAULT_LEFT_POSITION 0.8

/** Left margins */

#define DEFAULT_LEFT_MARGIN_DEFAULT 0.0
#define MIN_LEFT_MARGIN_DEFAULT -10.0
#define MAX_LEFT_MARGIN_DEFAULT 10.0

#define DEFAULT_LEFT_MARGIN_ACCID 1.0
#define DEFAULT_LEFT_MARGIN_BARLINE DEFAULT_LEFT_MARGIN_DEFAULT
#define DEFAULT_LEFT_MARGIN_BARLINE_ATTR_LEFT 0.0
#define DEFAULT_LEFT_MARGIN_BARLINE_ATTR_RIGHT 1.0
#define DEFAULT_LEFT_MARGIN_BEATRPT 2.0
#define DEFAULT_LEFT_MARGIN_CHORD 1.0
#define DEFAULT_LEFT_MARGIN_CLEF 1.0
#define DEFAULT_LEFT_MARGIN_KEYSIG 1.0
#define DEFAULT_LEFT_MARGIN_MENSUR 1.0
#define DEFAULT_LEFT_MARGIN_METERSIG 1.0
#define DEFAULT_LEFT_MARGIN_MREST DEFAULT_LEFT_MARGIN_DEFAULT
#define DEFAULT_LEFT_MARGIN_MRPT2 DEFAULT_LEFT_MARGIN_DEFAULT
#define DEFAULT_LEFT_MARGIN_MULTIREST DEFAULT_RIGHT_MARGIN_DEFAULT
#define DEFAULT_LEFT_MARGIN_MULTIRPT DEFAULT_LEFT_MARGIN_DEFAULT
#define DEFAULT_LEFT_MARGIN_NOTE 1.0
#define DEFAULT_LEFT_MARGIN_REST 1.0

/** Right margins */

#define DEFAULT_RIGHT_MARGIN_DEFAULT 0.0
#define MIN_RIGHT_MARGIN_DEFAULT 0.0
#define MAX_RIGHT_MARGIN_DEFAULT 10.0

#define DEFAULT_RIGHT_MARGIN_ACCID 0.0
#define DEFAULT_RIGHT_MARGIN_BARLINE 0.0
#define DEFAULT_RIGHT_MARGIN_BARLINE_ATTR_LEFT 1.0
#define DEFAULT_RIGHT_MARGIN_BARLINE_ATTR_RIGHT 0.0
#define DEFAULT_RIGHT_MARGIN_BEATRPT DEFAULT_RIGHT_MARGIN_DEFAULT
#define DEFAULT_RIGHT_MARGIN_CHORD 0.0
#define DEFAULT_RIGHT_MARGIN_CLEF 1.0
#define DEFAULT_RIGHT_MARGIN_KEYSIG 1.0
#define DEFAULT_RIGHT_MARGIN_MENSUR 1.0
#define DEFAULT_RIGHT_MARGIN_METERSIG 1.0
#define DEFAULT_RIGHT_MARGIN_MREST 0.0
#define DEFAULT_RIGHT_MARGIN_MRPT2 0.0
#define DEFAULT_RIGHT_MARGIN_MULTIREST 0.0
#define DEFAULT_RIGHT_MARGIN_MULTIRPT 0.0
#define DEFAULT_RIGHT_MARGIN_NOTE 0.0
#define DEFAULT_RIGHT_MARGIN_REST 0.0

/** Bottom margins */

#define DEFAULT_BOTTOM_MARGIN_DEFAULT 0.5
#define MIN_BOTTOM_MARGIN_DEFAULT 0.0
#define MAX_BOTTOM_MARGIN_DEFAULT 5.0

/** Top margins */

#define DEFAULT_TOP_MARGIN_DEFAULT 0.5
#define MIN_TOP_MARGIN_DEFAULT 0.0
#define MAX_TOP_MARGIN_DEFAULT 5.0

//----------------------------------------------------------------------------
// Style
//----------------------------------------------------------------------------

// the space between each lyric line in units
#define TEMP_LYRIC_LINE_SPACE 5.0

// the key signature spacing factor
#define TEMP_KEYSIG_STEP 1.3

// the maximum angle of a slur
#define TEMP_SLUR_MAX_SLOPE (45 * M_PI / 180)
#define TEMP_SLUR_CURVE_FACTOR 40 // a factor to allow more (0) or less (100) curved slurs
#define TEMP_SLUR_HEIGHT_FACTOR 8 // high value means flatter slurs
#define TEMP_SLUR_CONTROL_POINT_FACTOR 5 // higher value means more curved at the end

/* Style parameters for mensural notation */
// Ratios of mensural notehead, accidental, aug. dot size to CMN for the same staff size
#define TEMP_MNOTEHEAD_SIZE_FACTOR 1.0
#define TEMP_MACCID_SIZE_FACTOR 1.0
#define TEMP_MAUGDOT_SIZE_FACTOR 1.0
// Width of the minima diamond relative to its height
#define TEMP_MINIMA_WIDTH_FACTOR 1.0
// Size of mensuration sign circle relative to space between staff lines
#define MSIGN_CIRCLE_DIAM 1.7
// Vertical position of center of mensuration sign as distance below top of the staff
#define MSIGN_STAFFLINES_BELOW_TOP 2.0
// Size of dot inside mensuration signs relative to space between staff lines
#define MSIGN_DOT_DIAM 0.4
// Relative size of figures in proportions
#define PROPRT_SIZE_FACTOR 1.0
// Linewidth for staff lines in mensural notation, rel. to "normal" width of staff lines */
#define MENSURAL_LINEWIDTH_FACTOR 1.0

//----------------------------------------------------------------------------
// Style defines
//----------------------------------------------------------------------------

enum style_MEASURENUMBER
{
    MEASURENUMBER_system = 0,
    MEASURENUMBER_interval
};
    
//----------------------------------------------------------------------------
// StyleParam
//----------------------------------------------------------------------------
    
/**
 * This class is a base class of each styling parameter
 */
class StyleParam {
public:
    // constructors and destructors
    StyleParam() {}
    virtual ~StyleParam() {}
    
    void SetInfo(std::string title, std::string description);
    
public:
    //
protected:
    std::string m_title;
    std::string m_description;
};

//----------------------------------------------------------------------------
// StyleParamBool
//----------------------------------------------------------------------------

/**
 * This class is for boolean styling params
 */
class StyleParamBool : public StyleParam {
public:
    // constructors and destructors
    StyleParamBool() {}
    virtual ~StyleParamBool() {}
    void Init(bool defaultValue);
    
    bool GetValue() { return m_value; }
    bool GetDefault() { return m_defaultValue; }
    bool SetValue(bool m_value);
  
private:
    //
public:
    //
private:
    bool m_value;
    bool m_defaultValue;
};

//----------------------------------------------------------------------------
// StyleParamDbl
//----------------------------------------------------------------------------

/**
 * This class is for integer styling params
 */
class StyleParamDbl : public StyleParam {
public:
    // constructors and destructors
    StyleParamDbl() {}
    virtual ~StyleParamDbl() {}
    void Init(double defaultValue, double minValue, double maxValue);
    
    double GetValue() { return m_value; }
    double GetDefault() { return m_defaultValue; }
    double GetMin() { return m_minValue; }
    double GetMax() { return m_maxValue; }
    bool SetValue(double value);

private:
    //
public:
    //
private:
    double m_value;
    double m_defaultValue;
    double m_minValue;
    double m_maxValue;
};
    
//----------------------------------------------------------------------------
// StyleParamInt
//----------------------------------------------------------------------------

/**
 * This class is for integer styling params
 */
class StyleParamInt : public StyleParam {
public:
    // constructors and destructors
    StyleParamInt() {}
    virtual ~StyleParamInt() {}
    void Init(int defaultValue, int minValue, int maxValue, bool definitionFactor = false);
    
    int GetValue();
    int GetDefault() { return m_defaultValue; }
    int GetMin() { return m_minValue; }
    int GetMax() { return m_maxValue; }
    bool SetValue(int value);

private:
    //
public:
    //
private:
    int m_value;
    int m_defaultValue;
    int m_minValue;
    int m_maxValue;
    bool m_definitionFactor;
};
    
    
//----------------------------------------------------------------------------
// StyleParamMeasureNumber
//----------------------------------------------------------------------------

/**
 * This class is for map styling params
 */
class StyleParamMeasureNumber : public StyleParam {
public:
    // constructors and destructors
    StyleParamMeasureNumber() {};
    virtual ~StyleParamMeasureNumber() {};

    void Init(style_MEASURENUMBER defaultValue);
    bool SetValue(std::string value);
    
    style_MEASURENUMBER GetValue() { return m_value; }
    style_MEASURENUMBER GetDefault() { return m_defaultValue; }

private:
    //
public:
    static std::map<style_MEASURENUMBER, std::string> values;
private:
    style_MEASURENUMBER m_value;
    style_MEASURENUMBER m_defaultValue;
};
    
//----------------------------------------------------------------------------
// StyleParamStaffrel
//----------------------------------------------------------------------------

/**
 * This class is for map styling params
 */
class StyleParamStaffrel : public StyleParam {
public:
    // constructors and destructors
    StyleParamStaffrel() {};
    virtual ~StyleParamStaffrel() {};

    void Init(data_STAFFREL defaultValue, const std::vector<data_STAFFREL> &values);
    bool SetValue(std::string value);
    
    data_STAFFREL GetValue() { return m_value; }
    data_STAFFREL GetDefault() { return m_defaultValue; }

private:
    //
public:
    //
private:
    std::vector<data_STAFFREL> m_values;
    data_STAFFREL m_value;
    data_STAFFREL m_defaultValue;
};
    
//----------------------------------------------------------------------------
// Style
//----------------------------------------------------------------------------
    
/**
 * This class contains the document styling parameters.
 */
class Style {
public:
    // constructors and destructors
    Style();
    virtual ~Style();

public:
    
    /** The unit (1Ú2 of the distance between staff lines) **/
    StyleParamInt m_unit;
    /** The landscape paper orientation flag */
    StyleParamBool m_landscape;
    /** The staff line width */
    StyleParamDbl m_staffLineWidth;
    /** The stem width */
    StyleParamDbl m_stemWidth;
    /** The barLine width */
    StyleParamDbl m_barLineWidth;
    /** The maximum beam slope */
    StyleParamInt m_beamMaxSlope;
    /** The minimum beam slope */
    StyleParamInt m_beamMinSlope;
    /** The grace size ratio numerator */
    StyleParamDbl m_graceFactor;
    /** The page height */
    StyleParamInt m_pageHeight;
    /** The page width */
    StyleParamInt m_pageWidth;
    /** The page left margin */
    StyleParamInt m_pageLeftMar;
    /** The page right margin */
    StyleParamInt m_pageRightMar;
    /** The page top margin */
    StyleParamInt m_pageTopMar;
    /** The staff minimal spacing */
    StyleParamInt m_spacingStaff;
    /** The system minimal spacing */
    StyleParamInt m_spacingSystem;

    /** The minimal measure width (in units) */
    StyleParamInt m_minMeasureWidth;
    /** The lyrics size (in units) */
    StyleParamDbl m_lyricSize;
    /** haripin size (in units) */
    StyleParamDbl m_hairpinSize;

    /** ties and slurs */
    StyleParamDbl m_tieThickness;
    StyleParamDbl m_minSlurHeight;
    StyleParamDbl m_maxSlurHeight;
    StyleParamDbl m_slurThickness;

    /** The left position */
    StyleParamDbl m_leftPosition;

    /** The layout left margin by element */
    StyleParamDbl m_leftMarginAccid;
    StyleParamDbl m_leftMarginBarLine;
    StyleParamDbl m_leftMarginBarLineAttrLeft;
    StyleParamDbl m_leftMarginBarLineAttrRight;
    StyleParamDbl m_leftMarginBeatRpt;
    StyleParamDbl m_leftMarginChord;
    StyleParamDbl m_leftMarginClef;
    StyleParamDbl m_leftMarginKeySig;
    StyleParamDbl m_leftMarginMensur;
    StyleParamDbl m_leftMarginMeterSig;
    StyleParamDbl m_leftMarginMRest;
    StyleParamDbl m_leftMarginMRpt2;
    StyleParamDbl m_leftMarginMultiRest;
    StyleParamDbl m_leftMarginMultiRpt;
    StyleParamDbl m_leftMarginNote;
    StyleParamDbl m_leftMarginRest;
    /** The default left margin */
    StyleParamDbl m_leftMarginDefault;

    /** The layout right margin by element */
    StyleParamDbl m_rightMarginAccid;
    StyleParamDbl m_rightMarginBarLine;
    StyleParamDbl m_rightMarginBarLineAttrLeft;
    StyleParamDbl m_rightMarginBarLineAttrRight;
    StyleParamDbl m_rightMarginBeatRpt;
    StyleParamDbl m_rightMarginChord;
    StyleParamDbl m_rightMarginClef;
    StyleParamDbl m_rightMarginKeySig;
    StyleParamDbl m_rightMarginMensur;
    StyleParamDbl m_rightMarginMeterSig;
    StyleParamDbl m_rightMarginMRest;
    StyleParamDbl m_rightMarginMRpt2;
    StyleParamDbl m_rightMarginMultiRest;
    StyleParamDbl m_rightMarginMultiRpt;
    StyleParamDbl m_rightMarginNote;
    StyleParamDbl m_rightMarginRest;
    /** The default right margin */
    StyleParamDbl m_rightMarginDefault;

    /** The default right margin */
    StyleParamDbl m_bottomMarginDefault;

    /** The default right margin */
    StyleParamDbl m_topMarginDefault;
    
private:
    /** The array of style parameters */
    std::map<std::string, const StyleParam*> m_params;
    
    StyleParamInt m_margin;
    StyleParamMeasureNumber m_measureNumber;
};

} // namespace vrv

#endif // CUSTOM_VEROVIO_STYLE

#endif // __VRV_DEF_H__
