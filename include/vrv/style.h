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

#define DEFAULT_UNIT 9
#define MIN_UNIT 6
#define MAX_UNIT 18

#define DEFAULT_PAGE_LEFT_MAR 50
#define MIN_PAGE_LEFT_MAR 0
#define MAX_PAGE_LEFT_MAR 500

#define DEFAULT_PAGE_RIGHT_MAR 50
#define MIN_PAGE_RIGHT_MAR 0
#define MAX_PAGE_RIGHT_MAR 500

#define DEFAULT_PAGE_TOP_MAR 50
#define MIN_PAGE_TOP_MAR 0
#define MAX_PAGE_TOP_MAR 500

#define DEFAULT_PAGE_HEIGHT 2970
#define MIN_PAGE_HEIGHT 100
#define MAX_PAGE_HEIGHT 60000

#define DEFAULT_PAGE_WIDTH 2100
#define MIN_PAGE_WIDTH 100
#define MAX_PAGE_WIDTH 60000

#define DEFAULT_BARLINE_WITDH 3.0
#define MIN_BARLINE_WIDTH 1.0
#define MAX_BARLINE_WIDTH 8.0

#define DEFAULT_STAFFLINE_WITDH 2.0
#define MIN_STAFFLINE_WIDTH 1.0
#define MAX_STAFFLINE_WIDTH 8.0

#define DEFAULT_STEM_WITDH 2.0
#define MIN_STEM_WIDTH 1.0
#define MAX_STEM_WIDTH 5.0

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

#define DEFAULT_SPACING_STAFF 10
#define MIN_SPACING_STAFF 0
#define MAX_SPACING_STAFF 24

#define DEFAULT_SPACING_SYSTEM 6
#define MIN_SPACING_SYSTEM 0
#define MAX_SPACING_SYSTEM 12

#define DEFAULT_HAIRPIN_SIZE 3.0
#define MIN_HAIRPIN_SIZE 2.0
#define MAX_HAIRPIN_SIZE 8.0

#define DEFAULT_LYRIC_SIZE 4.5
#define MIN_LYRIC_SIZE 2.0
#define MAX_LYRIC_SIZE 8.0

#define DEFAULT_MEASURE_WIDTH 15.0
#define MIN_MEASURE_WIDTH 1.0
#define MAX_MEASURE_WIDTH 30.0

#define DEFAULT_TIE_THICKNESS 0.5

#define DEFAULT_MIN_SLUR_HEIGHT 1.2
#define DEFAULT_MAX_SLUR_HEIGHT 3.0
#define DEFAULT_SLUR_THICKNESS 0.6

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
#define DEFAULT_LEFT_MARGIN_CLEF 0.5
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
#define DEFAULT_RIGHT_MARGIN_BARLINE 2.0
#define DEFAULT_RIGHT_MARGIN_BARLINE_ATTR_LEFT 0.0
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
#define TEMP_LYRIC_LINE_SPACE 5.0 * PARAM_DENOMINATOR

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

/**
 * This class contains the document default environment variables.
 * FIXME: Some of them are not available as is in MEI - to be solved
 */
class Style {
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
    unsigned short m_staffLineWidth;
    /** The stem width */
    unsigned short m_stemWidth;
    /** The barLine width */
    unsigned short m_barLineWidth;
    /** The maximum beam slope */
    unsigned char m_beamMaxSlope;
    /** The minimum beam slope */
    unsigned char m_beamMinSlope;
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

    /** The minimal measure width in units / PARAM_DENOMINATOR */
    short m_minMeasureWidth;
    /** The lyrics size (in units / PARAM_DENOMINATOR) */
    int m_lyricSize;
    /** haripin size (in units / PARAM_DENOMINATOR) */
    int m_hairpinSize;

    /** ties and slurs */
    char m_tieThickness;
    char m_minSlurHeight;
    char m_maxSlurHeight;
    char m_slurThickness;

    /** The left position */
    char m_leftPosition;

    /** The layout left margin by element */
    char m_leftMarginAccid;
    char m_leftMarginBarLine;
    char m_leftMarginBarLineAttrLeft;
    char m_leftMarginBarLineAttrRight;
    char m_leftMarginBeatRpt;
    char m_leftMarginChord;
    char m_leftMarginClef;
    char m_leftMarginKeySig;
    char m_leftMarginMensur;
    char m_leftMarginMeterSig;
    char m_leftMarginMRest;
    char m_leftMarginMRpt2;
    char m_leftMarginMultiRest;
    char m_leftMarginMultiRpt;
    char m_leftMarginNote;
    char m_leftMarginRest;
    /** The default left margin */
    char m_leftMarginDefault;

    /** The layout right margin by element */
    char m_rightMarginAccid;
    char m_rightMarginBarLine;
    char m_rightMarginBarLineAttrLeft;
    char m_rightMarginBarLineAttrRight;
    char m_rightMarginBeatRpt;
    char m_rightMarginChord;
    char m_rightMarginClef;
    char m_rightMarginKeySig;
    char m_rightMarginMensur;
    char m_rightMarginMeterSig;
    char m_rightMarginMRest;
    char m_rightMarginMRpt2;
    char m_rightMarginMultiRest;
    char m_rightMarginMultiRpt;
    char m_rightMarginNote;
    char m_rightMarginRest;

    /** The default right margin */
    char m_rightMarginDefault;

    /** The default right margin */
    char m_bottomMarginDefault;

    /** The default right margin */
    char m_topMarginDefault;
};

} // namespace vrv

#endif // __VRV_DEF_H__
