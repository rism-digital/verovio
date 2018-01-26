/////////////////////////////////////////////////////////////////////////////
// Name:        options.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "options.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "att.h"
#include "vrv.h"
#include "vrvdef.h"

namespace vrv {

std::map<style_MEASURENUMBER, std::string> OptionMeasureNumber::values
    = { { MEASURENUMBER_system, "system" }, { MEASURENUMBER_interval, "interval" } };

//----------------------------------------------------------------------------
// Option
//----------------------------------------------------------------------------
    
void Option::SetInfo(std::string title, std::string description)
{
    m_title = title;
    m_description = description;
}
    
//----------------------------------------------------------------------------
// OptionBool
//----------------------------------------------------------------------------

void OptionBool::Init(bool defaultValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
}

bool OptionBool::SetValue(std::string value)
{
    bool b = (value == "true") ? true : false;
    return SetValue(b);
}

bool OptionBool::SetValue(bool value)
{
    m_value = value;
    return true;
}
    
//----------------------------------------------------------------------------
// OptionDbl
//----------------------------------------------------------------------------

void OptionDbl::Init(double defaultValue, double minValue, double maxValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
    m_minValue = minValue;
    m_maxValue = maxValue;
}
 
bool OptionDbl::SetValue(std::string value)
{
    return SetValue(atof(value.c_str()));
}
    
bool OptionDbl::SetValue(double value)
{
    if ((value < m_minValue) || (value > m_maxValue)) {
        LogError("Parameter out of bounds; default is %f, minimum is %f, and maximum is %f", m_defaultValue, m_minValue, m_maxValue);
        return false;
    }
    m_value = value;
    return true;
}
    
//----------------------------------------------------------------------------
// OptionInt
//----------------------------------------------------------------------------
    
void OptionInt::Init(int defaultValue, int minValue, int maxValue, bool definitionFactor)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
    m_minValue = minValue;
    m_maxValue = maxValue;
    m_definitionFactor = definitionFactor;
}
 
bool OptionInt::SetValue(std::string value)
{
    return SetValue(atoi(value.c_str()));
}
    
int OptionInt::GetValue()
{
    return (m_definitionFactor) ? m_value * DEFINITION_FACTOR : m_value;
}
    
int OptionInt::GetUnfactoredValue()
{
    assert(m_definitionFactor);
    return m_value;
}

bool OptionInt::SetValue(int value)
{
    if ((value < m_minValue) || (value > m_maxValue)) {
        LogError("Parameter out of bounds; default is %d, minimum is %d, and maximum is %d", m_defaultValue, m_minValue, m_maxValue);
        return false;
    }
    m_value = value;
    return true;
}
    
//----------------------------------------------------------------------------
// OptionMeasureNumber
//----------------------------------------------------------------------------
    
void OptionMeasureNumber::Init(style_MEASURENUMBER defaultValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
}

bool OptionMeasureNumber::SetValue(std::string value)
{
    std::map<style_MEASURENUMBER, std::string>::iterator it;
    for (it = OptionMeasureNumber::values.begin(); it != OptionMeasureNumber::values.end(); ++it)
        if (it->second == value) {
            m_value = it->first;
            return true;
        }
    LogError("Parameter '%s' not valie", value.c_str());
    return false;
}
    
//----------------------------------------------------------------------------
// OptionString
//----------------------------------------------------------------------------
    
void OptionString::Init(std::string defaultValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
}

bool OptionString::SetValue(std::string value)
{
    m_value = value;
    return true;
}
 
//----------------------------------------------------------------------------
// OptionArray
//----------------------------------------------------------------------------
    
void OptionArray::Init()
{
    m_values.empty();
    m_defaultValues.empty();
}

bool OptionArray::SetValue(std::string value)
{
    m_values.push_back(value);
    return true;
}
    
bool OptionArray::SetValue(std::vector<std::string> const &values)
{
    m_values = values;
    m_values.erase(std::remove_if(m_values.begin(), m_values.end(),
                                [](const std::string &s) { return s.empty(); }),
        m_values.end());
    return true;
}
    
//----------------------------------------------------------------------------
// OptionStaffRel
//----------------------------------------------------------------------------
    
void OptionStaffrel::Init(data_STAFFREL defaultValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
}

bool OptionStaffrel::SetValue(std::string value)
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

//----------------------------------------------------------------------------
// Options
//----------------------------------------------------------------------------
    
Options::Options()
{
    /********* selectors *********/
    
    m_appXPathQuery.SetInfo("App xPath query", "Set the xPath query for selecting <app> child elements, for example: \"./rdg[contains(@source, 'source-id')]\"; by default the <lem> or the first <rdg> is selected");
    m_appXPathQuery.Init();
    m_params["appXPathQuery"] = &m_appXPathQuery;
    
    m_choiceXPathQuery.SetInfo("Choice xPath query", "Set the xPath query for selecting <choice> child elements, for example: \"./orig\"; by default the first child is selected");
    m_choiceXPathQuery.Init();
    m_params["choiceXPathQuery"] = &m_choiceXPathQuery;
    
    m_mdivXPathQuery.SetInfo("Mdiv xPath query", "Set the xPath query for selecting the <mdiv> to be rendered; only one <mdiv> can be rendered");
    m_mdivXPathQuery.Init("");
    m_params["mdivXPathQuery"] = &m_mdivXPathQuery;
    
    /********* layout *********/
    
    m_adjustPageHeight.SetInfo("Adjust page height", "Crop the page height to the height of the content");
    m_adjustPageHeight.Init(false);
    m_params["adjustPageHeight"] = &m_adjustPageHeight;
    
    m_evenNoteSpacing.SetInfo("Even note spacing", "Specify the linear spacing factor");
    m_evenNoteSpacing.Init(false);
    m_params["evenNoteSpacing"] = &m_evenNoteSpacing;
    
    m_font.SetInfo("Font", "Set the music font");
    m_font.Init("Leipzig");
    m_params["font"] = &m_font;

    m_ignoreLayout.SetInfo("Ignore layout", "Ignore all encoded layout information (if any) and fully recalculate the layout");
    m_ignoreLayout.Init(false);
    m_params["ignoreLayout"] = &m_ignoreLayout;
    
    m_mmOutput.SetInfo("MM output", "Specify that the output in the SVG is given in mm (default is px)");
    m_mmOutput.Init(false);
    m_params["mmOutput"] = &m_mmOutput;
    
    m_noLayout.SetInfo("No layout", "Ignore all encoded layout information (if any) and output one single page with one single system");
    m_noLayout.Init(false);
    m_params["noLayout"] = &m_noLayout;

    m_spacingLinear.SetInfo("Spacing linear", "Specify the linear spacing factor");
    m_spacingLinear.Init(0.25, 0.0, 1.0);
    m_params["spacingLinear"] = &m_spacingLinear;
    
    m_spacingNonLinear.SetInfo("Spacing non linear", "Specify the non-linear spacing factor");
    m_spacingNonLinear.Init(0.6, 0.0, 1.0);
    m_params["spacingNonLinear"] = &m_spacingNonLinear;
    
    m_unit.SetInfo("Unit", "The MEI unit (1⁄2 of the distance between the staff lines)");
    m_unit.Init(9, 6, 20, true);
    m_params["unit"] = &m_unit;
    
    m_landscape.SetInfo("Landscape orientation", "The landscape paper orientation flag");
    m_landscape.Init(false);
    m_params["landscape"] = &m_landscape;
    
    m_staffLineWidth.SetInfo("Staff line width", "The staff line width in unit");
    m_staffLineWidth.Init(0.15, 0.10, 0.30);
    m_params["staffLineWidth"] = &m_staffLineWidth;
    
    m_stemWidth.SetInfo("Stem width", "The stem width");
    m_stemWidth.Init(0.20, 0.10, 0.);
    m_params["stemWidth"] = &m_stemWidth;
    
    m_barLineWidth.SetInfo("Bar line width", "The barLine width");
    m_barLineWidth.Init(0.30, 0.10, 0.80);
    m_params["barLineWidth"] = &m_barLineWidth;
    
    m_beamMaxSlope.SetInfo("Beam max slope", "The maximum beam slope");
    m_beamMaxSlope.Init(10, 1, 20);
    m_params["beamMaxSlope"] = &m_beamMaxSlope;
    
    m_beamMinSlope.SetInfo("Beam min slope", "The minimum beam slope");
    m_beamMinSlope.Init(0, 0, 0);
    m_params["beamMinSlope"] = &m_beamMinSlope;
    
    m_graceFactor.SetInfo("Grace factor", "The grace size ratio numerator");
    m_graceFactor.Init(0.75, 0.5, 1.0);
    m_params["graceFactor"] = &m_graceFactor;
   
    m_pageHeight.SetInfo("Page height", "The page height");
    m_pageHeight.Init(2970, 100, 60000, true);
    m_params["pageHeight"] = &m_pageHeight;
    
    m_pageWidth.SetInfo("Page width", "The page width");
    m_pageWidth.Init(2100, 100, 60000, true);
    m_params["pageWidth"] = &m_pageWidth;
   
    m_pageLeftMar.SetInfo("Page left mar", "The page left margin");
    m_pageLeftMar.Init(50, 0, 500, true);
    m_params["pageLeftMar"] = &m_pageLeftMar;
    
    m_pageRightMar.SetInfo("Page right mar", "The page right margin");
    m_pageRightMar.Init(50, 0, 500, true);
    m_params["pageRightMar"] = &m_pageRightMar;
    
    m_pageTopMar.SetInfo("Page top mar", "The page top margin");
    m_pageTopMar.Init(50, 0, 500, true);
    m_params["pageTopMar"] = &m_pageTopMar;
    
    m_spacingStaff.SetInfo("Spacing staff", "The staff minimal spacing in MEI units");
    m_spacingStaff.Init(8, 0, 24);
    m_params["spacingStaff"] = &m_spacingStaff;
    
    m_spacingSystem.SetInfo("Spacing system", "The system minimal spacing");
    m_spacingSystem.Init(3, 0, 12);
    m_params["spacingSystem"] = &m_spacingSystem;
    
    m_minMeasureWidth.SetInfo("Min measure width", "The minimal measure width in MEI units");
    m_minMeasureWidth.Init(15, 1, 30);
    m_params["minMeasureWidth"] = &m_minMeasureWidth;
    
    m_leftPosition.SetInfo("Left position", "The left position");
    m_leftPosition.Init(0.8, 0.0, 2.0);
    m_params["leftPosition"] = &m_leftPosition;
    
    m_lyricSize.SetInfo("Lyric size", "The lyrics size in MEI units");
    m_lyricSize.Init(4.5, 2.0, 8.0);
    m_params["lyricSize"] = &m_lyricSize;
    
    m_hairpinSize.SetInfo("Hairpin size", "The haripin size in MEI units");
    m_hairpinSize.Init(3.0, 1.0, 8.0);
    m_params["hairpinSize"] = &m_hairpinSize;
    
    /********* ties and slurs *********/
    
    m_tieThickness.SetInfo("Tie thickness", "The tie thickness in MEI units");
    m_tieThickness.Init(0.5, 0.2, 1.0);
    m_params["tieThickness"] = &m_tieThickness;
    
    m_minSlurHeight.SetInfo("Min slur height", "The minimum slur height in MEI units");
    m_minSlurHeight.Init(1.2, 0.3, 2.0);
    m_params["minSlurHeight"] = &m_minSlurHeight;
    
    m_maxSlurHeight.SetInfo("Max slur height", "The maximum slur height in MEI units");
    m_maxSlurHeight.Init(3.0, 2.0, 4.0);
    m_params["maxSlurHeight"] = &m_maxSlurHeight;
    
    m_slurThickness.SetInfo("Slur thickness", "The slur thickness in MEI units");
    m_slurThickness.Init(0.6, 0.2, 1.2);
    m_params["slurThickness"] = &m_slurThickness;
    
    /********* The layout left margin by element *********/
    
    m_leftMarginAccid.SetInfo("Left margin accid", "The margin for accid in MEI units");
    m_leftMarginAccid.Init(1.0, 0.0, 2.0);
    m_params["leftMarginAccid"] = &m_leftMarginAccid;
    
    m_leftMarginBarLine.SetInfo("Left margin barLine", "The margin for barLine in MEI units");
    m_leftMarginBarLine.Init(0.0, 0.0, 2.0);
    m_params["leftMarginBarLine"] = &m_leftMarginBarLine;
    
    m_leftMarginLeftBarLine.SetInfo("Left margin left barLine", "The margin for left barLine in MEI units");
    m_leftMarginLeftBarLine.Init(0.0, 0.0, 2.0);
    m_params["leftMarginLeftBarLine"] = &m_leftMarginLeftBarLine;
    
    m_leftMarginRightBarLine.SetInfo("Left margin right barLine", "The margin for right barLine in MEI units");
    m_leftMarginRightBarLine.Init(1.0, 0.0, 2.0);
    m_params["leftMarginRightBarLine"] = &m_leftMarginRightBarLine;
    
    m_leftMarginBeatRpt.SetInfo("Left margin beatRpt", "The margin for beatRpt in MEI units");
    m_leftMarginBeatRpt.Init(2.0, 0.0, 2.0);
    m_params["leftMarginBeatRpt"] = &m_leftMarginBeatRpt;
    
    m_leftMarginChord.SetInfo("Left margin chord", "The margin for chord in MEI units");
    m_leftMarginChord.Init(1.0, 0.0, 2.0);
    m_params["leftMarginChord"] = &m_leftMarginChord;
    
    m_leftMarginClef.SetInfo("Left margin clef", "The margin for clef in MEI units");
    m_leftMarginClef.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginClef"] = &m_leftMarginClef;
    
    m_leftMarginKeySig.SetInfo("Left margin keySig", "The margin for keySig in MEI units");
    m_leftMarginKeySig.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginKeySig"] = &m_leftMarginKeySig;
    
    m_leftMarginMensur.SetInfo("Left margin mensur", "The margin for mensur in MEI units");
    m_leftMarginMensur.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginMensur"] = &m_leftMarginMensur;
    
    m_leftMarginMeterSig.SetInfo("Left margin meterSig", "The margin for meterSig in MEI units");
    m_leftMarginMeterSig.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginMeterSig"] = &m_leftMarginMeterSig;
    
    m_leftMarginMRest.SetInfo("Left margin mRest", "The margin for mRest in MEI units");
    m_leftMarginMRest.Init(0.0, 0.0, 2.0);;
    m_params["leftMarginMRest"] = &m_leftMarginMRest;
    
    m_leftMarginMRpt2.SetInfo("Left margin mRpt2", "The margin for mRpt2 in MEI units");
    m_leftMarginMRpt2.Init(0.0, 0.0, 2.0);;
    m_params["leftMarginMRpt2"] = &m_leftMarginMRpt2;
    
    m_leftMarginMultiRest.SetInfo("Left margin multiRest", "The margin for multiRest in MEI units");
    m_leftMarginMultiRest.Init(0.0, 0.0, 2.0);
    m_params["leftMarginMultiRest"] = &m_leftMarginMultiRest;
    
    m_leftMarginMultiRpt.SetInfo("Left margin multiRpt", "The margin for multiRpt in MEI units");
    m_leftMarginMultiRpt.Init(0.0, 0.0, 2.0);;
    m_params["leftMarginMultiRpt"] = &m_leftMarginMultiRpt;
    
    m_leftMarginNote.SetInfo("Left margin note", "The margin for note in MEI units");
    m_leftMarginNote.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginNote"] = &m_leftMarginNote;
    
    m_leftMarginRest.SetInfo("Left margin rest", "The margin for rest in MEI units");
    m_leftMarginRest.Init(1.0, 0.0, 2.0);;
    m_params["leftMarginRest"] = &m_leftMarginRest;
    
    m_leftMarginDefault.SetInfo("Left margin default", "The default left margin");
    m_leftMarginDefault.Init(0.0, 0.0, 2.0);;
    m_params["leftMarginDefault"] = &m_leftMarginDefault;
    
    /********* The layout right margin by element *********/

    m_rightMarginAccid.SetInfo("Right margin accid", "The right margin for accid in MEI units");
    m_rightMarginAccid.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginAccid"] = &m_rightMarginAccid;
    
    m_rightMarginBarLine.SetInfo("Right margin barLine", "The right margin for barLine in MEI units");
    m_rightMarginBarLine.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginBarLine"] = &m_rightMarginBarLine;
    
    m_rightMarginLeftBarLine.SetInfo("Right margin left barLine", "The right margin for left barLine in MEI units");
    m_rightMarginLeftBarLine.Init(1.0, 0.0, 2.0);;
    m_params["rightMarginLeftBarLine"] = &m_rightMarginLeftBarLine;
    
    m_rightMarginRightBarLine.SetInfo("Right margin right barLine", "The right margin for right barLine in MEI units");
    m_rightMarginRightBarLine.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginRightBarLine"] = &m_rightMarginRightBarLine;
    
    m_rightMarginBeatRpt.SetInfo("Right margin beatRpt", "The right margin for beatRpt in MEI units");
    m_rightMarginBeatRpt.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginBeatRpt"] = &m_rightMarginBeatRpt;
    
    m_rightMarginChord.SetInfo("Right margin chord", "The right margin for chord in MEI units");
    m_rightMarginChord.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginChord"] = &m_rightMarginChord;
    
    m_rightMarginClef.SetInfo("Right margin clef", "The right margin for clef in MEI units");
    m_rightMarginClef.Init(1.0, 0.0, 2.0);;
    m_params["rightMarginClef"] = &m_rightMarginClef;
    
    m_rightMarginKeySig.SetInfo("Right margin keySig", "The right margin for keySig in MEI units");
    m_rightMarginKeySig.Init(1.0, 0.0, 2.0);;
    m_params["rightMarginKeySig"] = &m_rightMarginKeySig;
    
    m_rightMarginMensur.SetInfo("Right margin mensur", "The right margin for mensur in MEI units");
    m_rightMarginMensur.Init(1.0, 0.0, 2.0);;
    m_params["rightMarginMensur"] = &m_rightMarginMensur;
    
    m_rightMarginMeterSig.SetInfo("Right margin meterSig", "The right margin for meterSig in MEI units");
    m_rightMarginMeterSig.Init(1.0, 0.0, 2.0);;
    m_params["rightMarginMeterSig"] = &m_rightMarginMeterSig;
    
    m_rightMarginMRest.SetInfo("Right margin mRest", "The right margin for mRest in MEI units");
    m_rightMarginMRest.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginMRest"] = &m_rightMarginMRest;
    
    m_rightMarginMRpt2.SetInfo("Right margin mRpt2", "The right margin for mRpt2 in MEI units");
    m_rightMarginMRpt2.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginMRpt2"] = &m_rightMarginMRpt2;
    
    m_rightMarginMultiRest.SetInfo("Right margin multiRest", "The right margin for multiRest in MEI units");
    m_rightMarginMultiRest.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginMultiRest"] = &m_rightMarginMultiRest;
    
    m_rightMarginMultiRpt.SetInfo("Right margin multiRpt", "The right margin for multiRpt in MEI units");
    m_rightMarginMultiRpt.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginMultiRpt"] = &m_rightMarginMultiRpt;
    
    m_rightMarginNote.SetInfo("Right margin note", "The right margin for note in MEI units");
    m_rightMarginNote.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginNote"] = &m_rightMarginNote;

    m_rightMarginRest.SetInfo("Right margin rest", "The right margin for rest in MEI units");
    m_rightMarginRest.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginRest"] = &m_rightMarginRest;

    m_rightMarginDefault.SetInfo("Right margin default", "The default right margin");
    m_rightMarginDefault.Init(0.0, 0.0, 2.0);;
    m_params["rightMarginDefault"] = &m_rightMarginDefault;
    
    /********* The layout top margin by element *********/
    
    m_topMarginDefault.SetInfo("Top margin default", "The default top margin");
    m_topMarginDefault.Init(0.5, 0.0, 6.0);;
    m_params["topMarginDefault"] = &m_topMarginDefault;
    
    /********* The layout bottom margin by element *********/
    
    m_bottomMarginDefault.SetInfo("Bottom margin default", "The default bottom margin");
    m_bottomMarginDefault.Init(0.5, 0.0, 5.0);;
    m_params["bottomMarginDefault"] = &m_bottomMarginDefault;
    
    /********* Display options *********/
    
    m_measureNumber.SetInfo("Measure number","The measure numbering rule (unused)");
    m_measureNumber.Init(MEASURENUMBER_system);
    m_params["measureNumber"] = &m_measureNumber;

    /*
    // Example of a staffRel param
    OptionStaffrel rel;
    data_STAFFREL staffrel;
    staffrel.SetBasic(STAFFREL_basic_above);
    rel.Init(staffrel);
    // within is not allowed for this param
    //rel.SetValue("within");
    */
}

Options::~Options()
{
}

} // namespace vrv
