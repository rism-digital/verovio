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

#include "att.h"
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
    
void StyleParamStaffrel::Init(data_STAFFREL defaultValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
}

bool StyleParamStaffrel::SetValue(std::string value)
{
    Att converter;
    data_STAFFREL staffrel = converter.StrToStaffrel(value);
    if (!staffrel.HasValue()) {
        LogError("Parameter '%s' not valid", value.c_str());
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
    m_staffLineWidth.Init(0.15, 0.10, 0.30);
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
    m_spacingStaff.Init(8, 0, 24);
    m_params["spacingStaff"] = &m_spacingStaff;
    
    /** The system minimal spacing */
    m_spacingSystem.Init(3, 0, 12);
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
    
    m_leftMarginAccid.Init(1.0, 0.0, 2.0);
    m_params["leftMarginAccid"] = &m_leftMarginAccid;
    
    m_leftMarginBarLine.Init(0.0, 0.0, 2.0);
    m_params["leftMarginBarLine"] = &m_leftMarginBarLine;
    
    m_leftMarginBarLineAttrLeft.Init(0.0, 0.0, 2.0);
    m_params["leftMarginBarLineAttrLeft"] = &m_leftMarginBarLineAttrLeft;
    
    m_leftMarginBarLineAttrRight.Init(1.0, 0.0, 2.0);
    m_params["leftMarginBarLineAttrRight"] = &m_leftMarginBarLineAttrRight;
    
    m_leftMarginBeatRpt.Init(2.0, 0.0, 2.0);
    m_params["leftMarginBeatRpt"] = &m_leftMarginBeatRpt;
    
    m_leftMarginChord.Init(1.0, 0.0, 2.0);
    m_params["leftMarginChord"] = &m_leftMarginChord;
    
    m_leftMarginClef.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginClef"] = &m_leftMarginClef;
    
    m_leftMarginKeySig.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginKeySig"] = &m_leftMarginKeySig;
    
    m_leftMarginMensur.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginMensur"] = &m_leftMarginMensur;
    
    m_leftMarginMeterSig.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginMeterSig"] = &m_leftMarginMeterSig;
    
    m_leftMarginMRest.Init(0.0, 0.0, 2.0);;
    m_params["leftMarginMRest"] = &m_leftMarginMRest;
    
    m_leftMarginMRpt2.Init(0.0, 0.0, 2.0);;
    m_params["leftMarginMRpt2"] = &m_leftMarginMRpt2;
    
    m_leftMarginMultiRest.Init(0.0, 0.0, 2.0);;
    m_params["leftMarginMultiRest"] = &m_leftMarginMultiRest;
    
    m_leftMarginMultiRpt.Init(0.0, 0.0, 2.0);;
    m_params["leftMarginMultiRpt"] = &m_leftMarginMultiRpt;
    
    m_leftMarginNote.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginNote"] = &m_leftMarginNote;
    
    m_leftMarginRest.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginRest"] = &m_leftMarginRest;
    
    /** The default left margin */
    m_leftMarginDefault.Init(0.0, 0.0, 2.0);;
    m_params["leftMarginDefault"] = &m_leftMarginDefault;
    
    /********* The layout right margin by element *********/
    
    m_rightMarginAccid.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginAccid"] = &m_rightMarginAccid;
    
    m_rightMarginBarLine.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginBarLine"] = &m_rightMarginBarLine;
    
    m_rightMarginBarLineAttrLeft.Init(1.0, 0.0, 2.0);;
    m_params["rightMarginBarLineAttrLeft"] = &m_rightMarginBarLineAttrLeft;
    
    m_rightMarginBarLineAttrRight.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginBarLineAttrRight"] = &m_rightMarginBarLineAttrRight;
    
    m_rightMarginBeatRpt.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginBeatRpt"] = &m_rightMarginBeatRpt;
    
    m_rightMarginChord.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginChord"] = &m_rightMarginChord;
    
    m_rightMarginClef.Init(1.0, 0.0, 2.0);;
    m_params["rightMarginClef"] = &m_rightMarginClef;
    
    m_rightMarginKeySig.Init(1.0, 0.0, 2.0);;
    m_params["rightMarginKeySig"] = &m_rightMarginKeySig;
    
    m_rightMarginMensur.Init(1.0, 0.0, 2.0);;
    m_params["rightMarginMensur"] = &m_rightMarginMensur;
    
    m_rightMarginMeterSig.Init(1.0, 0.0, 2.0);;
    m_params["rightMarginMeterSig"] = &m_rightMarginMeterSig;
    
    m_rightMarginMRest.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginMRest"] = &m_rightMarginMRest;
    
    m_rightMarginMRpt2.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginMRpt2"] = &m_rightMarginMRpt2;
    
    m_rightMarginMultiRest.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginMultiRest"] = &m_rightMarginMultiRest;
    
    m_rightMarginMultiRpt.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginMultiRpt"] = &m_rightMarginMultiRpt;
    
    m_rightMarginNote.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginNote"] = &m_rightMarginNote;
    
    /** The default right margin */
    m_rightMarginDefault.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginRest"] = &m_rightMarginRest;
    
    /** The default right margin */
    m_bottomMarginDefault.Init(0.5, 0.0, 5.0);;
    m_params["bottomMarginDefault"] = &m_bottomMarginDefault;
    
    /** The default right margin */
    m_topMarginDefault.Init(0.5, 0.0, 6.0);;
    m_params["topMarginDefault"] = &m_topMarginDefault;
    
    m_margin.Init(2100, 10, 21000);
    m_params["margin"] = &m_margin;
    
    m_measureNumber.Init(MEASURENUMBER_system);
    m_params["measureNumber"] = &m_measureNumber;
    
    //m_measureNumber.SetValue("interval");

    // Example of a staffRel param
    StyleParamStaffrel rel;
    data_STAFFREL staffrel;
    staffrel.SetBasic(STAFFREL_basic_above);
    rel.Init(staffrel);
    // within is not allowed for this param
    //rel.SetValue("within");
    
    if (rel.GetValueAlternate()->GetType() == STAFFREL_basic)
        LogMessage("Prout");
}

Style::~Style()
{
}

} // namespace vrv
