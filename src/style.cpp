/////////////////////////////////////////////////////////////////////////////
// Name:        style.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "style.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrvdef.h"

namespace vrv {

std::map<style_MEASURENUMBER, std::string> StyleParamMeasureNumber::values
    = { { MEASURENUMBER_system, "system" }, { MEASURENUMBER_interval, "interval" } };

//----------------------------------------------------------------------------
// Style
//----------------------------------------------------------------------------

    
void StyleParamBool::Init(bool defaultValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
}

void StyleParamDbl::Init(double defaultValue, double minValue, double maxValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
    m_minValue = minValue;
    m_maxValue = maxValue;
}
    
void StyleParamInt::Init(int defaultValue, int minValue, int maxValue, bool definitionFactor)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
    m_minValue = minValue;
    m_maxValue = maxValue;
    m_definitionFactor = definitionFactor;
}
    
int StyleParamInt::GetValue()
{
    return (m_definitionFactor) ? m_value * DEFINITION_FACTOR : m_value;
}

void StyleParamMeasureNumber::Init(style_MEASURENUMBER defaultValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
}

bool StyleParamMeasureNumber::Read(std::string value)
{

    std::map<style_MEASURENUMBER, std::string>::iterator it;
    for (it = StyleParamMeasureNumber::values.begin(); it != StyleParamMeasureNumber::values.end(); ++it)
        if (it->second == value) {
            m_value = it->first;
            return true;
        }
    return false;
}

Style::Style()
{

    /** The unit (1⁄2 of the distance between staff lines) **/
    m_unit.Init(9, 6, 20, true);
    m_params["unit"] = &m_unit;
    /** The landscape paper orientation flag */
    m_landscape.Init(false);
    m_params["landscape"] = &m_landscape;
    /** The staff line width */
    m_staffLineWidth.Init(0.22, 0.10, 0.30);
    m_params["staffLineWidth"] = &m_staffLineWidth;
    /** The stem width */
    StyleParamDbl m_stemWidth;
    m_params["stemWidth"] = &m_stemWidth;
    /** The barLine width */
    StyleParamDbl m_barLineWidth;
    m_params["barLineWidth"] = &m_barLineWidth;
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
    
    /** The minimal measure width in units / PARAM_DENOMINATOR */
    StyleParamDbl m_minMeasureWidth;
    /** The lyrics size (in units / PARAM_DENOMINATOR) */
    StyleParamDbl m_lyricSize;
    /** haripin size (in units / PARAM_DENOMINATOR) */
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
    
    m_margin.Init(2100, 10, 21000);
    m_params["margin"] = &m_margin;
    m_measureNumber.Init(MEASURENUMBER_system);
    m_params["measureNumber"] = &m_measureNumber;
    
    m_measureNumber.Read("interval");

    /*
    m_unit = DEFAULT_UNIT * DEFINITION_FACTOR;
    m_landscape = false;
    m_barLineWidth = (unsigned short)(DEFAULT_BARLINE_WITDH * DEFINITION_FACTOR);
    m_staffLineWidth = (unsigned short)(DEFAULT_STAFFLINE_WITDH * DEFINITION_FACTOR);
    m_stemWidth = (unsigned short)(DEFAULT_STEM_WITDH * DEFINITION_FACTOR);
     */

    /**
     * Used for initializing Doc::m_drawingBeamMin/MaxSlope and used in
     * View::DrawBeamPostponed
     *
    m_beamMaxSlope = 10;
    m_beamMinSlope = 0;

    /**
     * graceNum and Den for cue-sized notes.
     *
    m_graceNum = 5;
    m_graceDen = 7;

    m_pageHeight = DEFAULT_PAGE_HEIGHT * DEFINITION_FACTOR;
    m_pageWidth = DEFAULT_PAGE_WIDTH * DEFINITION_FACTOR;
    m_pageRightMar = DEFAULT_PAGE_RIGHT_MAR * DEFINITION_FACTOR;

    m_pageLeftMar = DEFAULT_PAGE_LEFT_MAR * DEFINITION_FACTOR;
    m_pageTopMar = DEFAULT_PAGE_TOP_MAR * DEFINITION_FACTOR;
    m_spacingStaff = DEFAULT_SPACING_STAFF;
    m_spacingSystem = DEFAULT_SPACING_SYSTEM;

    m_lyricSize = DEFAULT_LYRIC_SIZE * PARAM_DENOMINATOR;
    m_hairpinSize = DEFAULT_HAIRPIN_SIZE * PARAM_DENOMINATOR;

    m_minMeasureWidth = (short)(DEFAULT_MEASURE_WIDTH * PARAM_DENOMINATOR);

    /** ties and slurs *
    m_tieThickness = DEFAULT_TIE_THICKNESS * PARAM_DENOMINATOR;
    m_minSlurHeight = DEFAULT_MIN_SLUR_HEIGHT * PARAM_DENOMINATOR;
    m_maxSlurHeight = DEFAULT_MAX_SLUR_HEIGHT * PARAM_DENOMINATOR;

    m_slurThickness = DEFAULT_SLUR_THICKNESS * PARAM_DENOMINATOR;

    /** The left spacing *
    m_leftPosition = DEFAULT_LEFT_POSITION * PARAM_DENOMINATOR;

    /** The layout left margin by element *
    m_leftMarginAccid = DEFAULT_LEFT_MARGIN_ACCID * PARAM_DENOMINATOR;
    m_leftMarginBarLine = DEFAULT_LEFT_MARGIN_BARLINE * PARAM_DENOMINATOR;
    m_leftMarginBarLineAttrLeft = DEFAULT_LEFT_MARGIN_BARLINE_ATTR_LEFT * PARAM_DENOMINATOR;
    m_leftMarginBarLineAttrRight = DEFAULT_LEFT_MARGIN_BARLINE_ATTR_RIGHT * PARAM_DENOMINATOR;
    m_leftMarginBeatRpt = DEFAULT_LEFT_MARGIN_BEATRPT * PARAM_DENOMINATOR;
    m_leftMarginChord = DEFAULT_LEFT_MARGIN_CHORD * PARAM_DENOMINATOR;
    m_leftMarginClef = DEFAULT_LEFT_MARGIN_CLEF * PARAM_DENOMINATOR;
    m_leftMarginKeySig = DEFAULT_LEFT_MARGIN_KEYSIG * PARAM_DENOMINATOR;
    m_leftMarginMensur = DEFAULT_LEFT_MARGIN_MENSUR * PARAM_DENOMINATOR;
    m_leftMarginMeterSig = DEFAULT_LEFT_MARGIN_METERSIG * PARAM_DENOMINATOR;
    m_leftMarginMRest = DEFAULT_LEFT_MARGIN_MREST * PARAM_DENOMINATOR;
    m_leftMarginMRpt2 = DEFAULT_LEFT_MARGIN_MRPT2 * PARAM_DENOMINATOR;
    m_leftMarginMultiRest = DEFAULT_LEFT_MARGIN_MULTIREST * PARAM_DENOMINATOR;
    m_leftMarginMultiRpt = DEFAULT_LEFT_MARGIN_MULTIRPT * PARAM_DENOMINATOR;
    m_leftMarginNote = DEFAULT_LEFT_MARGIN_NOTE * PARAM_DENOMINATOR;
    m_leftMarginRest = DEFAULT_LEFT_MARGIN_REST * PARAM_DENOMINATOR;
    /** The default left margin *
    m_leftMarginDefault = DEFAULT_LEFT_MARGIN_DEFAULT * PARAM_DENOMINATOR;

    /** The layout right margin by element *
    m_rightMarginAccid = DEFAULT_RIGHT_MARGIN_ACCID * PARAM_DENOMINATOR;
    m_rightMarginBarLine = DEFAULT_RIGHT_MARGIN_BARLINE * PARAM_DENOMINATOR;
    m_rightMarginBarLineAttrLeft = DEFAULT_RIGHT_MARGIN_BARLINE_ATTR_LEFT * PARAM_DENOMINATOR;
    m_rightMarginBarLineAttrRight = DEFAULT_RIGHT_MARGIN_BARLINE_ATTR_RIGHT * PARAM_DENOMINATOR;
    m_rightMarginBeatRpt = DEFAULT_RIGHT_MARGIN_BEATRPT * PARAM_DENOMINATOR;
    m_rightMarginChord = DEFAULT_RIGHT_MARGIN_CHORD * PARAM_DENOMINATOR;
    m_rightMarginClef = DEFAULT_RIGHT_MARGIN_CLEF * PARAM_DENOMINATOR;
    m_rightMarginKeySig = DEFAULT_RIGHT_MARGIN_KEYSIG * PARAM_DENOMINATOR;
    m_rightMarginMensur = DEFAULT_RIGHT_MARGIN_MENSUR * PARAM_DENOMINATOR;
    m_rightMarginMeterSig = DEFAULT_RIGHT_MARGIN_METERSIG * PARAM_DENOMINATOR;
    m_rightMarginMRest = DEFAULT_RIGHT_MARGIN_MREST * PARAM_DENOMINATOR;
    m_rightMarginMRpt2 = DEFAULT_RIGHT_MARGIN_MRPT2 * PARAM_DENOMINATOR;
    m_rightMarginMultiRest = DEFAULT_RIGHT_MARGIN_MULTIREST * PARAM_DENOMINATOR;
    m_rightMarginMultiRpt = DEFAULT_RIGHT_MARGIN_MULTIRPT * PARAM_DENOMINATOR;
    m_rightMarginNote = DEFAULT_RIGHT_MARGIN_NOTE * PARAM_DENOMINATOR;
    m_rightMarginRest = DEFAULT_RIGHT_MARGIN_REST * PARAM_DENOMINATOR;
    /** The default right margin *
    m_rightMarginDefault = DEFAULT_RIGHT_MARGIN_DEFAULT * PARAM_DENOMINATOR;

    /** The default right margin *
    m_bottomMarginDefault = DEFAULT_BOTTOM_MARGIN_DEFAULT * PARAM_DENOMINATOR;

    /** The default right margin *
    m_topMarginDefault = DEFAULT_TOP_MARGIN_DEFAULT * PARAM_DENOMINATOR;
    */
}

Style::~Style()
{
}

} // namespace vrv
