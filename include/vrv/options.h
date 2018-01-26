/////////////////////////////////////////////////////////////////////////////
// Name:        options.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_OPTIONS_H__
#define __VRV_OPTIONS_H__

#ifdef CUSTOM_VEROVIO_OPTIONS
#include "custom_options.h"
#else

#include <map>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "attalternates.h"
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

#define DEFAULT_SPACING_STAFF 8
#define MIN_SPACING_STAFF 0
#define MAX_SPACING_STAFF 24

#define DEFAULT_SPACING_SYSTEM 3
#define MIN_SPACING_SYSTEM 0
#define MAX_SPACING_SYSTEM 12

//----------------------------------------------------------------------------
// Options
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

/* Options parameters for mensural notation */
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
// Options defines
//----------------------------------------------------------------------------

enum style_MEASURENUMBER
{
    MEASURENUMBER_system = 0,
    MEASURENUMBER_interval
};
    
//----------------------------------------------------------------------------
// Option
//----------------------------------------------------------------------------
    
/**
 * This class is a base class of each styling parameter
 */
class Option {
public:
    // constructors and destructors
    Option() {}
    virtual ~Option() {}
    
    virtual bool SetValue(std::string) = 0;
    
    void SetInfo(std::string title, std::string description);
    
public:
    //
protected:
    std::string m_title;
    std::string m_description;
};

//----------------------------------------------------------------------------
// OptionBool
//----------------------------------------------------------------------------

/**
 * This class is for boolean styling params
 */
class OptionBool : public Option {
public:
    // constructors and destructors
    OptionBool() {}
    virtual ~OptionBool() {}
    void Init(bool defaultValue);
    
    virtual bool SetValue(std::string value);
    
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
// OptionDbl
//----------------------------------------------------------------------------

/**
 * This class is for integer styling params
 */
class OptionDbl : public Option {
public:
    // constructors and destructors
    OptionDbl() {}
    virtual ~OptionDbl() {}
    void Init(double defaultValue, double minValue, double maxValue);
    
    virtual bool SetValue(std::string value);
    
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
// OptionInt
//----------------------------------------------------------------------------

/**
 * This class is for integer styling params
 */
class OptionInt : public Option {
public:
    // constructors and destructors
    OptionInt() {}
    virtual ~OptionInt() {}
    void Init(int defaultValue, int minValue, int maxValue, bool definitionFactor = false);
    
    virtual bool SetValue(std::string value);
    
    int GetValue();
    int GetUnfactoredValue();
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
// OptionString
//----------------------------------------------------------------------------

/**
 * This class is for string styling params
 */
class OptionString : public Option {
public:
    // constructors and destructors
    OptionString() {}
    virtual ~OptionString() {}
    void Init(std::string defaultValue);
    
    virtual bool SetValue(std::string value);
    
    std::string GetValue() { return m_value; }
    std::string GetDefault() { return m_defaultValue; }

private:
    //
public:
    //
private:
    std::string m_value;
    std::string m_defaultValue;
};
    
//----------------------------------------------------------------------------
// OptionArray
//----------------------------------------------------------------------------

/**
 * This class is for array (string) styling params
 */
class OptionArray : public Option {
public:
    // constructors and destructors
    OptionArray() {}
    virtual ~OptionArray() {}
    void Init();
    
    virtual bool SetValue(std::string value);
    
    std::vector<std::string> GetValue() { return m_values; }
    std::vector<std::string> GetDefault() { return m_defaultValues; }
    bool SetValue(std::vector<std::string> const &values);

private:
    //
public:
    //
private:
    std::vector<std::string> m_values;
    std::vector<std::string> m_defaultValues;
};
    
//----------------------------------------------------------------------------
// OptionMeasureNumber
//----------------------------------------------------------------------------

/**
 * This class is for map styling params
 */
class OptionMeasureNumber : public Option {
public:
    // constructors and destructors
    OptionMeasureNumber() {};
    virtual ~OptionMeasureNumber() {};

    void Init(style_MEASURENUMBER defaultValue);
    
    virtual bool SetValue(std::string value);
    
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
// OptionStaffrel
//----------------------------------------------------------------------------

/**
 * This class is for map styling params
 */
class OptionStaffrel : public Option {
public:
    // constructors and destructors
    OptionStaffrel() {};
    virtual ~OptionStaffrel() {};
    
    // Alternate type style cannot have a restricted list of possible values
    void Init(data_STAFFREL defaultValue);
    
    virtual bool SetValue(std::string value);
    
    // For altenate types return a reference to the value
    // Alternatively we can have a values vector for each sub-type
    data_STAFFREL *GetValueAlternate() { return &m_value; }
    data_STAFFREL *GetDefaultAlernate() { return &m_defaultValue; }
    
private:
    //
public:
    //
private:
    data_STAFFREL m_value;
    data_STAFFREL m_defaultValue;
};
    
//----------------------------------------------------------------------------
// OptionStaffrelBasic
//----------------------------------------------------------------------------

/**
 * This class is for map styling params
 */
class OptionStaffrelBasic : public Option {
public:
    // constructors and destructors
    OptionStaffrelBasic() {};
    virtual ~OptionStaffrelBasic() {};

    void Init(data_STAFFREL_basic defaultValue, const std::vector<data_STAFFREL_basic> &values);

    virtual bool SetValue(std::string value);
    
    data_STAFFREL_basic GetValue() { return m_value; }
    data_STAFFREL_basic GetDefault() { return m_defaultValue; }

private:
    //
public:
    //
private:
    std::vector<data_STAFFREL_basic> m_values;
    data_STAFFREL_basic m_value;
    data_STAFFREL_basic m_defaultValue;
};
    
//----------------------------------------------------------------------------
// Options
//----------------------------------------------------------------------------
    
/**
 * This class contains the document styling parameters.
 */
class Options {
public:
    // constructors and destructors
    Options();
    virtual ~Options();
    
    const MapOfStrStyleParams *GetParams() { return &m_params; }

public:
    
    /** */
    OptionArray m_appXPathQuery;
    OptionArray m_choiceXPathQuery;
    OptionString m_mdivXPathQuery;
    OptionBool m_evenNoteSpacing;
    OptionString m_font;

    OptionBool m_adjustPageHeight;
    OptionBool m_ignoreLayout;
    OptionBool m_noLayout;
    
    OptionBool m_mmOutput;
    
    OptionDbl m_spacingLinear;
    OptionDbl m_spacingNonLinear;
    
    /** The unit (1Ú2 of the distance between staff lines) **/
    OptionInt m_unit;
    /** The landscape paper orientation flag */
    OptionBool m_landscape;
    /** The staff line width */
    OptionDbl m_staffLineWidth;
    /** The stem width */
    OptionDbl m_stemWidth;
    /** The barLine width */
    OptionDbl m_barLineWidth;
    /** The maximum beam slope */
    OptionInt m_beamMaxSlope;
    /** The minimum beam slope */
    OptionInt m_beamMinSlope;
    /** The grace size ratio numerator */
    OptionDbl m_graceFactor;
    /** The page height */
    OptionInt m_pageHeight;
    /** The page width */
    OptionInt m_pageWidth;
    /** The page left margin */
    OptionInt m_pageLeftMar;
    /** The page right margin */
    OptionInt m_pageRightMar;
    /** The page top margin */
    OptionInt m_pageTopMar;
    /** The staff minimal spacing */
    OptionInt m_spacingStaff;
    /** The system minimal spacing */
    OptionInt m_spacingSystem;

    /** The minimal measure width (in units) */
    OptionInt m_minMeasureWidth;
    /** The lyrics size (in units) */
    OptionDbl m_lyricSize;
    /** haripin size (in units) */
    OptionDbl m_hairpinSize;

    /** ties and slurs */
    OptionDbl m_tieThickness;
    OptionDbl m_minSlurHeight;
    OptionDbl m_maxSlurHeight;
    OptionDbl m_slurThickness;

    /** The left position */
    OptionDbl m_leftPosition;

    /** The layout left margin by element */
    OptionDbl m_leftMarginAccid;
    OptionDbl m_leftMarginBarLine;
    OptionDbl m_leftMarginBarLineAttrLeft;
    OptionDbl m_leftMarginBarLineAttrRight;
    OptionDbl m_leftMarginBeatRpt;
    OptionDbl m_leftMarginChord;
    OptionDbl m_leftMarginClef;
    OptionDbl m_leftMarginKeySig;
    OptionDbl m_leftMarginMensur;
    OptionDbl m_leftMarginMeterSig;
    OptionDbl m_leftMarginMRest;
    OptionDbl m_leftMarginMRpt2;
    OptionDbl m_leftMarginMultiRest;
    OptionDbl m_leftMarginMultiRpt;
    OptionDbl m_leftMarginNote;
    OptionDbl m_leftMarginRest;
    /** The default left margin */
    OptionDbl m_leftMarginDefault;

    /** The layout right margin by element */
    OptionDbl m_rightMarginAccid;
    OptionDbl m_rightMarginBarLine;
    OptionDbl m_rightMarginBarLineAttrLeft;
    OptionDbl m_rightMarginBarLineAttrRight;
    OptionDbl m_rightMarginBeatRpt;
    OptionDbl m_rightMarginChord;
    OptionDbl m_rightMarginClef;
    OptionDbl m_rightMarginKeySig;
    OptionDbl m_rightMarginMensur;
    OptionDbl m_rightMarginMeterSig;
    OptionDbl m_rightMarginMRest;
    OptionDbl m_rightMarginMRpt2;
    OptionDbl m_rightMarginMultiRest;
    OptionDbl m_rightMarginMultiRpt;
    OptionDbl m_rightMarginNote;
    OptionDbl m_rightMarginRest;
    /** The default right margin */
    OptionDbl m_rightMarginDefault;

    /** The default right margin */
    OptionDbl m_bottomMarginDefault;

    /** The default right margin */
    OptionDbl m_topMarginDefault;
    
private:
    /** The array of style parameters */
    MapOfStrStyleParams m_params;
    
    OptionInt m_margin;
    OptionMeasureNumber m_measureNumber;
};

} // namespace vrv

#endif // CUSTOM_VEROVIO_OPTIONS

#endif // __VRV_DEF_H__
