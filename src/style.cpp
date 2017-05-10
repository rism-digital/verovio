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

#include "attconverter.h"
#include "vrv.h"
#include "vrvdef.h"

namespace vrv {

std::map<style_MEASURENUMBER, std::string> StyleParamMeasureNumber::values
    = { { MEASURENUMBER_system, "system" }, { MEASURENUMBER_interval, "interval" } };

//----------------------------------------------------------------------------
// Style
//----------------------------------------------------------------------------
    
void StyleParam::SetInfo(std::string title, std::string description)
{
    m_title = title;
    m_description = description;
}
    
void StyleParamBool::Init(bool defaultValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
}
    
bool StyleParamBool::SetValue(bool value)
{
    m_value = value;
    return true;
}

void StyleParamDbl::Init(double defaultValue, double minValue, double maxValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
    m_minValue = minValue;
    m_maxValue = maxValue;
}
    
bool StyleParamDbl::SetValue(double value)
{
    if ((value < m_minValue) || (value > m_maxValue)) {
        LogError("Parameter out of bounds; default is %f, minimum is %f, and maximum is %f", m_defaultValue, m_minValue, m_maxValue);
        return false;
    }
    m_value = value;
    return true;
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

bool StyleParamInt::SetValue(int value)
{
    if ((value < m_minValue) || (value > m_maxValue)) {
        LogError("Parameter out of bounds; default is %d, minimum is %d, and maximum is %d", m_defaultValue, m_minValue, m_maxValue);
        return false;
    }
    m_value = value;
    return true;
}
    
void StyleParamMeasureNumber::Init(style_MEASURENUMBER defaultValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
}

bool StyleParamMeasureNumber::SetValue(std::string value)
{
    std::map<style_MEASURENUMBER, std::string>::iterator it;
    for (it = StyleParamMeasureNumber::values.begin(); it != StyleParamMeasureNumber::values.end(); ++it)
        if (it->second == value) {
            m_value = it->first;
            return true;
        }
    LogError("Parameter '%s' not valie", value.c_str());
    return false;
}
    
void StyleParamStaffrel::Init(data_STAFFREL defaultValue, const std::vector<data_STAFFREL> &values)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
    m_values = values;
}

bool StyleParamStaffrel::SetValue(std::string value)
{
    AttConverter converter;
    data_STAFFREL staffrel = converter.StrToStaffrel(value);
    if (std::find(m_values.begin(), m_values.end(), staffrel) == m_values.end()) {
        LogError("Parameter '%s' not valie", value.c_str());
        return false;
    }
    m_value = staffrel;
    return true;
}

Style::Style()
{
    m_unit.SetInfo("Unit", "The MEI unit (1⁄2 of the distance between the staff lines)");
    m_unit.Init(9, 6, 20, true);
    m_params["unit"] = &m_unit;
    
    m_landscape.SetInfo("Landscape orientation", "The landscape paper orientation flag");
    m_landscape.Init(false);
    m_params["landscape"] = &m_landscape;
    
    m_staffLineWidth.SetInfo("Staff line width", "The staff line width in unit");
    m_staffLineWidth.Init(0.20, 0.10, 0.30);
    m_params["staffLineWidth"] = &m_staffLineWidth;
    
    /** The stem width */
    m_stemWidth.Init(0.20, 0.10, 0.);
    m_params["stemWidth"] = &m_stemWidth;
    
    /** The barLine width */
    m_barLineWidth.Init(0.30, 0.10, 0.80);
    m_params["barLineWidth"] = &m_barLineWidth;
    
    /** The maximum beam slope */
    m_beamMaxSlope.Init(10, 1, 20);
    m_params["beamMaxSlope"] = &m_beamMaxSlope;
    
    /** The minimum beam slope */
    m_beamMinSlope.Init(0, 0, 0);
    m_params["beamMinSlope"] = &m_beamMinSlope;
    
    /** The grace size ratio numerator */
    m_graceFactor.Init(0.75, 0.5, 1.0);
    m_params["graceFactor"] = &m_graceFactor;
   
    /** The page height */
    m_pageHeight.Init(2970, 100, 60000, true);
    m_params["pageHeight"] = &m_pageHeight;
    
    /** The page width */
    m_pageWidth.Init(2100, 100, 60000, true);
    m_params["pageWidth"] = &m_pageWidth;
   
    /** The page left margin */
    m_pageLeftMar.Init(50, 0, 500, true);
    m_params["pageLeftMar"] = &m_pageLeftMar;
    
    /** The page right margin */
    m_pageRightMar.Init(50, 0, 500, true);
    m_params["pageRightMar"] = &m_pageRightMar;
    
    /** The page top margin */
    m_pageTopMar.Init(50, 0, 500, true);
    m_params["pageTopMar"] = &m_pageTopMar;
    
    /** The staff minimal spacing (in MEI units) */
    m_spacingStaff.Init(10, 0, 24);
    m_params["spacingStaff"] = &m_spacingStaff;
    
    /** The system minimal spacing */
    m_spacingSystem.Init(6, 0, 12);
    m_params["spacingSystem"] = &m_spacingSystem;
    
    /** The minimal measure width in units */
    m_minMeasureWidth.Init(15, 1, 30);
    m_params["minMeasureWidth"] = &m_minMeasureWidth;
    
    /** The left position */
    m_leftPosition.Init(0.8, 0.0, 2.0);
    m_params["leftPosition"] = &m_leftPosition;
    
    /** The lyrics size (in units) */
    m_lyricSize.Init(4.5, 2.0, 8.0);
    m_params["lyricSize"] = &m_lyricSize;
    
    /** haripin size (in units) */
    m_hairpinSize.Init(3.0, 1.0, 8.0);
    m_params["hairpinSize"] = &m_hairpinSize;
    
    /********* ties and slurs *********/
    
    m_tieThickness.Init(0.5, 0.2, 1.0);
    m_params["tieThickness"] = &m_tieThickness;
    
    m_minSlurHeight.Init(1.2, 0.3, 2.0);
    m_params["minSlurHeight"] = &m_minSlurHeight;
    
    m_maxSlurHeight.Init(3.0, 2.0, 4.0);
    m_params["maxSlurHeight"] = &m_maxSlurHeight;
    
    m_slurThickness.Init(0.6, 0.2, 1.2);
    m_params["slurThickness"] = &m_slurThickness;
    
    /********* The layout left margin by element *********/
    
     StyleParamDbl m_leftMarginAccid;
    m_params[""] = &;
    StyleParamDbl m_leftMarginBarLine;
    m_params[""] = &;
    StyleParamDbl m_leftMarginBarLineAttrLeft;
    m_params[""] = &;
    StyleParamDbl m_leftMarginBarLineAttrRight;
    m_params[""] = &;
    StyleParamDbl m_leftMarginBeatRpt;
    m_params[""] = &;
    StyleParamDbl m_leftMarginChord;
    m_params[""] = &;
    StyleParamDbl m_leftMarginClef;
    m_params[""] = &;
    StyleParamDbl m_leftMarginKeySig;
    m_params[""] = &;
    StyleParamDbl m_leftMarginMensur;
    m_params[""] = &;
    StyleParamDbl m_leftMarginMeterSig;
    m_params[""] = &;
    StyleParamDbl m_leftMarginMRest;
    m_params[""] = &;
    StyleParamDbl m_leftMarginMRpt2;
    m_params[""] = &;
    StyleParamDbl m_leftMarginMultiRest;
    m_params[""] = &;
    StyleParamDbl m_leftMarginMultiRpt;
    m_params[""] = &;
    StyleParamDbl m_leftMarginNote;
    m_params[""] = &;
    StyleParamDbl m_leftMarginRest;
    
    /** The default left margin *
    StyleParamDbl m_leftMarginDefault;
    m_params[""] = &;
    
    /********* The layout right margin by element *********/
    
    StyleParamDbl m_rightMarginAccid;
    m_params[""] = &;
    StyleParamDbl m_rightMarginBarLine;
    m_params[""] = &;
    StyleParamDbl m_rightMarginBarLineAttrLeft;
    m_params[""] = &;
    StyleParamDbl m_rightMarginBarLineAttrRight;
    m_params[""] = &;
    StyleParamDbl m_rightMarginBeatRpt;
    m_params[""] = &;
    StyleParamDbl m_rightMarginChord;
    m_params[""] = &;
    StyleParamDbl m_rightMarginClef;
    m_params[""] = &;
    StyleParamDbl m_rightMarginKeySig;
    m_params[""] = &;
    StyleParamDbl m_rightMarginMensur;
    m_params[""] = &;
    StyleParamDbl m_rightMarginMeterSig;
    m_params[""] = &;
    StyleParamDbl m_rightMarginMRest;
    m_params[""] = &;
    StyleParamDbl m_rightMarginMRpt2;
    m_params[""] = &;
    StyleParamDbl m_rightMarginMultiRest;
    m_params[""] = &;
    StyleParamDbl m_rightMarginMultiRpt;
    m_params[""] = &;
    StyleParamDbl m_rightMarginNote;
    m_params[""] = &;
    StyleParamDbl m_rightMarginRest;
    /** The default right margin *
    StyleParamDbl m_rightMarginDefault;
    m_params[""] = &;
    
    /** The default right margin *
    StyleParamDbl m_bottomMarginDefault;
    m_params[""] = &;
    
    /** The default right margin *
    StyleParamDbl m_topMarginDefault;
    m_params[""] = &;
     */
    
    m_margin.Init(2100, 10, 21000);
    m_params["margin"] = &m_margin;
    m_measureNumber.Init(MEASURENUMBER_system);
    m_params["measureNumber"] = &m_measureNumber;
    
    m_measureNumber.Read("interval");

    // Example of a staffRel param
    StyleParamStaffrel rel;
    rel.Init(STAFFREL_above, {STAFFREL_above, STAFFREL_below});
    // within is not allowed for this param
    rel.Read("within");
    
    /*

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
