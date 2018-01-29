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

bool Option::SetValueBool(bool value)
{
    // If not overriden
    LogError("Unsupported type bool for %s", m_key.c_str());
    return false;
}
    
bool Option::SetValueDbl(double value)
{
    // If not overriden
    LogError("Unsupported type double for %s", m_key.c_str());
    return false;
}
    
bool Option::SetValue(std::string value)
{
    // If not overriden
    LogError("Unsupported type string for %s", m_key.c_str());
    return false;
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
    
bool OptionBool::SetValueDbl(double value)
{
    return SetValue((bool)value);
}

bool OptionBool::SetValueBool(bool value)
{
    return SetValue(value);
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
    
bool OptionDbl::SetValueDbl(double value)
{
    return SetValue(value);
}
    
bool OptionDbl::SetValue(double value)
{
    if ((value < m_minValue) || (value > m_maxValue)) {
        LogError("Parameter value %f for '%s' out of bounds; default is %f, minimum %f, and maximum %f", value, m_title.c_str(), m_defaultValue, m_minValue, m_maxValue);
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

bool OptionInt::SetValueDbl(double value)
{
    return SetValue((int)value);
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
        LogError("Parameter value %d for '%s' out of bounds; default is %d, minimum %d, and maximum %d", value, m_title.c_str(), m_defaultValue, m_minValue, m_maxValue);
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
    LogError("Parameter '%s' not valid", value.c_str());
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
    
void OptionArray::Init(OptionGrp *grp)
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
    /********* layout *********/
    
    m_generalLayout.SetLabel("General layout options");
    m_grps.push_back(&m_generalLayout);
    
    m_adjustPageHeight.SetInfo("Adjust page height", "Crop the page height to the height of the content");
    m_adjustPageHeight.Init(false);
    this->Register(&m_adjustPageHeight, "adjustPageHeight", &m_generalLayout);
    
    m_evenNoteSpacing.SetInfo("Even note spacing", "Specify the linear spacing factor");
    m_evenNoteSpacing.Init(false);
    this->Register(&m_evenNoteSpacing, "evenNoteSpacing", &m_generalLayout);
    
    m_font.SetInfo("Font", "Set the music font");
    m_font.Init("Leipzig");
    this->Register(&m_font, "font", &m_generalLayout);

    m_ignoreLayout.SetInfo("Ignore layout", "Ignore all encoded layout information (if any) and fully recalculate the layout");
    m_ignoreLayout.Init(false);
    this->Register(&m_ignoreLayout, "ignoreLayout", &m_generalLayout);
    
    m_mmOutput.SetInfo("MM output", "Specify that the output in the SVG is given in mm (default is px)");
    m_mmOutput.Init(false);
    this->Register(&m_mmOutput, "mmOutput", &m_generalLayout);
    
    m_noLayout.SetInfo("No layout", "Ignore all encoded layout information (if any) and output one single page with one single system");
    m_noLayout.Init(false);
    this->Register(&m_noLayout, "noLayout", &m_generalLayout);

    m_spacingLinear.SetInfo("Spacing linear", "Specify the linear spacing factor");
    m_spacingLinear.Init(0.25, 0.0, 1.0);
    this->Register(&m_spacingLinear, "spacingLinear", &m_generalLayout);
    
    m_spacingNonLinear.SetInfo("Spacing non linear", "Specify the non-linear spacing factor");
    m_spacingNonLinear.Init(0.6, 0.0, 1.0);
    this->Register(&m_spacingNonLinear, "spacingNonLinear", &m_generalLayout);
    
    m_unit.SetInfo("Unit", "The MEI unit (1⁄2 of the distance between the staff lines)");
    m_unit.Init(9, 6, 20, true);
    this->Register(&m_unit, "unit", &m_generalLayout);
    
    m_landscape.SetInfo("Landscape orientation", "The landscape paper orientation flag");
    m_landscape.Init(false);
    this->Register(&m_landscape, "landscape", &m_generalLayout);
    
    m_staffLineWidth.SetInfo("Staff line width", "The staff line width in unit");
    m_staffLineWidth.Init(0.15, 0.10, 0.30);
    this->Register(&m_staffLineWidth, "staffLineWidth", &m_generalLayout);
    
    m_stemWidth.SetInfo("Stem width", "The stem width");
    m_stemWidth.Init(0.20, 0.10, 0.0);
    this->Register(&m_stemWidth, "stemWidth", &m_generalLayout);
    
    m_barLineWidth.SetInfo("Bar line width", "The barLine width");
    m_barLineWidth.Init(0.30, 0.10, 0.80);
    this->Register(&m_barLineWidth, "barLineWidth", &m_generalLayout);
    
    m_beamMaxSlope.SetInfo("Beam max slope", "The maximum beam slope");
    m_beamMaxSlope.Init(10, 1, 20);
    this->Register(&m_beamMaxSlope, "beamMaxSlope", &m_generalLayout);
    
    m_beamMinSlope.SetInfo("Beam min slope", "The minimum beam slope");
    m_beamMinSlope.Init(0, 0, 0);
    this->Register(&m_beamMinSlope, "beamMinSlope", &m_generalLayout);
    
    m_graceFactor.SetInfo("Grace factor", "The grace size ratio numerator");
    m_graceFactor.Init(0.75, 0.5, 1.0);
    this->Register(&m_graceFactor, "graceFactor", &m_generalLayout);
   
    m_pageHeight.SetInfo("Page height", "The page height");
    m_pageHeight.Init(2970, 100, 60000, true);
    this->Register(&m_pageHeight, "pageHeight", &m_generalLayout);
    
    m_pageWidth.SetInfo("Page width", "The page width");
    m_pageWidth.Init(2100, 100, 60000, true);
    this->Register(&m_pageWidth, "pageWidth", &m_generalLayout);
   
    m_pageLeftMar.SetInfo("Page left mar", "The page left margin");
    m_pageLeftMar.Init(50, 0, 500, true);
    this->Register(&m_pageLeftMar, "pageLeftMar", &m_generalLayout);
    
    m_pageRightMar.SetInfo("Page right mar", "The page right margin");
    m_pageRightMar.Init(50, 0, 500, true);
    this->Register(&m_pageRightMar, "pageRightMar", &m_generalLayout);
    
    m_pageTopMar.SetInfo("Page top mar", "The page top margin");
    m_pageTopMar.Init(50, 0, 500, true);
    this->Register(&m_pageTopMar, "pageTopMar", &m_generalLayout);
    
    m_spacingStaff.SetInfo("Spacing staff", "The staff minimal spacing in MEI units");
    m_spacingStaff.Init(8, 0, 24);
    this->Register(&m_spacingStaff, "spacingStaff", &m_generalLayout);
    
    m_spacingSystem.SetInfo("Spacing system", "The system minimal spacing in MEI units");
    m_spacingSystem.Init(3, 0, 12);
    this->Register(&m_spacingSystem, "spacingSystem", &m_generalLayout);
    
    m_minMeasureWidth.SetInfo("Min measure width", "The minimal measure width in MEI units");
    m_minMeasureWidth.Init(15, 1, 30);
    this->Register(&m_minMeasureWidth, "minMeasureWidth", &m_generalLayout);
    
    m_leftPosition.SetInfo("Left position", "The left position");
    m_leftPosition.Init(0.8, 0.0, 2.0);
    this->Register(&m_leftPosition, "leftPosition", &m_generalLayout);
    
    m_lyricSize.SetInfo("Lyric size", "The lyrics size in MEI units");
    m_lyricSize.Init(4.5, 2.0, 8.0);
    this->Register(&m_lyricSize, "lyricSize", &m_generalLayout);
    
    m_hairpinSize.SetInfo("Hairpin size", "The haripin size in MEI units");
    m_hairpinSize.Init(3.0, 1.0, 8.0);
    this->Register(&m_hairpinSize, "hairpinSize", &m_generalLayout);
    
    m_tieThickness.SetInfo("Tie thickness", "The tie thickness in MEI units");
    m_tieThickness.Init(0.5, 0.2, 1.0);
    this->Register(&m_tieThickness, "tieThickness", &m_generalLayout);
    
    m_minSlurHeight.SetInfo("Min slur height", "The minimum slur height in MEI units");
    m_minSlurHeight.Init(1.2, 0.3, 2.0);
    this->Register(&m_minSlurHeight, "minSlurHeight", &m_generalLayout);
    
    m_maxSlurHeight.SetInfo("Max slur height", "The maximum slur height in MEI units");
    m_maxSlurHeight.Init(3.0, 2.0, 4.0);
    this->Register(&m_maxSlurHeight, "maxSlurHeight", &m_generalLayout);
    
    m_slurThickness.SetInfo("Slur thickness", "The slur thickness in MEI units");
    m_slurThickness.Init(0.6, 0.2, 1.2);
    this->Register(&m_slurThickness, "slurThickness", &m_generalLayout);
    
    m_measureNumber.SetInfo("Measure number","The measure numbering rule (unused)");
    m_measureNumber.Init(MEASURENUMBER_system);
    this->Register(&m_measureNumber, "measureNumber", &m_generalLayout);
    
    /********* selectors *********/
    
    m_selectors.SetLabel("Element selectors");
    m_grps.push_back(&m_selectors);
    
    m_appXPathQuery.SetInfo("App xPath query", "Set the xPath query for selecting <app> child elements, for example: \"./rdg[contains(@source, 'source-id')]\"; by default the <lem> or the first <rdg> is selected");
    m_appXPathQuery.Init();
    this->Register(&m_appXPathQuery, "appXPathQuery", &m_selectors);
    
    m_choiceXPathQuery.SetInfo("Choice xPath query", "Set the xPath query for selecting <choice> child elements, for example: \"./orig\"; by default the first child is selected");
    m_choiceXPathQuery.Init();
    this->Register(&m_choiceXPathQuery, "choiceXPathQuery", &m_selectors);
    
    m_mdivXPathQuery.SetInfo("Mdiv xPath query", "Set the xPath query for selecting the <mdiv> to be rendered; only one <mdiv> can be rendered");
    m_mdivXPathQuery.Init("");
    this->Register(&m_mdivXPathQuery, "mdivXPathQuery", &m_selectors);
    
    /********* The layout left margin by element *********/
    
    m_elementMargins.SetLabel("Element margins");
    m_grps.push_back(&m_elementMargins);
    
    m_leftMarginAccid.SetInfo("Left margin accid", "The margin for accid in MEI units");
    m_leftMarginAccid.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginAccid, "leftMarginAccid", &m_elementMargins);
    
    m_leftMarginBarLine.SetInfo("Left margin barLine", "The margin for barLine in MEI units");
    m_leftMarginBarLine.Init(0.0, 0.0, 2.0);
    this->Register(&m_leftMarginBarLine, "leftMarginBarLine", &m_elementMargins);
    
    m_leftMarginLeftBarLine.SetInfo("Left margin left barLine", "The margin for left barLine in MEI units");
    m_leftMarginLeftBarLine.Init(0.0, 0.0, 2.0);
    this->Register(&m_leftMarginLeftBarLine, "leftMarginLeftBarLine", &m_elementMargins);
    
    m_leftMarginRightBarLine.SetInfo("Left margin right barLine", "The margin for right barLine in MEI units");
    m_leftMarginRightBarLine.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginRightBarLine, "leftMarginRightBarLine", &m_elementMargins);
    
    m_leftMarginBeatRpt.SetInfo("Left margin beatRpt", "The margin for beatRpt in MEI units");
    m_leftMarginBeatRpt.Init(2.0, 0.0, 2.0);
    this->Register(&m_leftMarginBeatRpt, "leftMarginBeatRpt", &m_elementMargins);
    
    m_leftMarginChord.SetInfo("Left margin chord", "The margin for chord in MEI units");
    m_leftMarginChord.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginChord, "leftMarginChord", &m_elementMargins);
    
    m_leftMarginClef.SetInfo("Left margin clef", "The margin for clef in MEI units");
    m_leftMarginClef.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginClef, "leftMarginClef", &m_elementMargins);
    
    m_leftMarginKeySig.SetInfo("Left margin keySig", "The margin for keySig in MEI units");
    m_leftMarginKeySig.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginKeySig, "leftMarginKeySig", &m_elementMargins);
    
    m_leftMarginMensur.SetInfo("Left margin mensur", "The margin for mensur in MEI units");
    m_leftMarginMensur.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginMensur, "leftMarginMensur", &m_elementMargins);
    
    m_leftMarginMeterSig.SetInfo("Left margin meterSig", "The margin for meterSig in MEI units");
    m_leftMarginMeterSig.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginMeterSig, "leftMarginMeterSig", &m_elementMargins);
    
    m_leftMarginMRest.SetInfo("Left margin mRest", "The margin for mRest in MEI units");
    m_leftMarginMRest.Init(0.0, 0.0, 2.0);
    this->Register(&m_leftMarginMRest, "leftMarginMRest", &m_elementMargins);
    
    m_leftMarginMRpt2.SetInfo("Left margin mRpt2", "The margin for mRpt2 in MEI units");
    m_leftMarginMRpt2.Init(0.0, 0.0, 2.0);
    this->Register(&m_leftMarginMRpt2, "leftMarginMRpt2", &m_elementMargins);
    
    m_leftMarginMultiRest.SetInfo("Left margin multiRest", "The margin for multiRest in MEI units");
    m_leftMarginMultiRest.Init(0.0, 0.0, 2.0);
    this->Register(&m_leftMarginMultiRest, "leftMarginMultiRest", &m_elementMargins);
    
    m_leftMarginMultiRpt.SetInfo("Left margin multiRpt", "The margin for multiRpt in MEI units");
    m_leftMarginMultiRpt.Init(0.0, 0.0, 2.0);
    this->Register(&m_leftMarginMultiRpt, "leftMarginMultiRpt", &m_elementMargins);
    
    m_leftMarginNote.SetInfo("Left margin note", "The margin for note in MEI units");
    m_leftMarginNote.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginNote, "leftMarginNote", &m_elementMargins);
    
    m_leftMarginRest.SetInfo("Left margin rest", "The margin for rest in MEI units");
    m_leftMarginRest.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginRest, "leftMarginRest", &m_elementMargins);
    
    m_leftMarginDefault.SetInfo("Left margin default", "The default left margin");
    m_leftMarginDefault.Init(0.0, 0.0, 2.0);
    this->Register(&m_leftMarginDefault, "leftMarginDefault", &m_elementMargins);
    
    /********* The layout right margin by element *********/

    m_rightMarginAccid.SetInfo("Right margin accid", "The right margin for accid in MEI units");
    m_rightMarginAccid.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginAccid, "rightMarginAccid", &m_elementMargins);
    
    m_rightMarginBarLine.SetInfo("Right margin barLine", "The right margin for barLine in MEI units");
    m_rightMarginBarLine.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginBarLine, "rightMarginBarLine", &m_elementMargins);
    
    m_rightMarginLeftBarLine.SetInfo("Right margin left barLine", "The right margin for left barLine in MEI units");
    m_rightMarginLeftBarLine.Init(1.0, 0.0, 2.0);
    this->Register(&m_rightMarginLeftBarLine, "rightMarginLeftBarLine", &m_elementMargins);
    
    m_rightMarginRightBarLine.SetInfo("Right margin right barLine", "The right margin for right barLine in MEI units");
    m_rightMarginRightBarLine.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginRightBarLine, "rightMarginRightBarLine", &m_elementMargins);
    
    m_rightMarginBeatRpt.SetInfo("Right margin beatRpt", "The right margin for beatRpt in MEI units");
    m_rightMarginBeatRpt.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginBeatRpt, "rightMarginBeatRpt", &m_elementMargins);
    
    m_rightMarginChord.SetInfo("Right margin chord", "The right margin for chord in MEI units");
    m_rightMarginChord.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginChord, "rightMarginChord", &m_elementMargins);
    
    m_rightMarginClef.SetInfo("Right margin clef", "The right margin for clef in MEI units");
    m_rightMarginClef.Init(1.0, 0.0, 2.0);
    this->Register(&m_rightMarginClef, "rightMarginClef", &m_elementMargins);
    
    m_rightMarginKeySig.SetInfo("Right margin keySig", "The right margin for keySig in MEI units");
    m_rightMarginKeySig.Init(1.0, 0.0, 2.0);
    this->Register(&m_rightMarginKeySig, "rightMarginKeySig", &m_elementMargins);
    
    m_rightMarginMensur.SetInfo("Right margin mensur", "The right margin for mensur in MEI units");
    m_rightMarginMensur.Init(1.0, 0.0, 2.0);
    this->Register(&m_rightMarginMensur, "rightMarginMensur", &m_elementMargins);
    
    m_rightMarginMeterSig.SetInfo("Right margin meterSig", "The right margin for meterSig in MEI units");
    m_rightMarginMeterSig.Init(1.0, 0.0, 2.0);
    this->Register(&m_rightMarginMeterSig, "rightMarginMeterSig", &m_elementMargins);
    
    m_rightMarginMRest.SetInfo("Right margin mRest", "The right margin for mRest in MEI units");
    m_rightMarginMRest.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginMRest, "rightMarginMRest", &m_elementMargins);
    
    m_rightMarginMRpt2.SetInfo("Right margin mRpt2", "The right margin for mRpt2 in MEI units");
    m_rightMarginMRpt2.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginMRpt2, "rightMarginMRpt2", &m_elementMargins);
    
    m_rightMarginMultiRest.SetInfo("Right margin multiRest", "The right margin for multiRest in MEI units");
    m_rightMarginMultiRest.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginMultiRest, "rightMarginMultiRest", &m_elementMargins);
    
    m_rightMarginMultiRpt.SetInfo("Right margin multiRpt", "The right margin for multiRpt in MEI units");
    m_rightMarginMultiRpt.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginMultiRpt, "rightMarginMultiRpt", &m_elementMargins);
    
    m_rightMarginNote.SetInfo("Right margin note", "The right margin for note in MEI units");
    m_rightMarginNote.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginNote, "rightMarginNote", &m_elementMargins);

    m_rightMarginRest.SetInfo("Right margin rest", "The right margin for rest in MEI units");
    m_rightMarginRest.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginRest, "rightMarginRest", &m_elementMargins);

    m_rightMarginDefault.SetInfo("Right margin default", "The default right margin");
    m_rightMarginDefault.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginDefault, "rightMarginDefault", &m_elementMargins);
    
    /********* The layout top margin by element *********/
    
    m_topMarginDefault.SetInfo("Top margin default", "The default top margin");
    m_topMarginDefault.Init(0.5, 0.0, 6.0);
    this->Register(&m_topMarginDefault, "topMarginDefault", &m_elementMargins);
    
    /********* The layout bottom margin by element *********/
    
    m_bottomMarginDefault.SetInfo("Bottom margin default", "The default bottom margin");
    m_bottomMarginDefault.Init(0.5, 0.0, 5.0);
    this->Register(&m_bottomMarginDefault, "bottomMarginDefault", &m_elementMargins);


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
    
void Options::Register(Option *option, std::string key, OptionGrp *grp)
{
    assert(option);
    assert(grp);
    
    m_items[key] = option;
    option->SetKey(key);
    grp->AddOption(option);
}

} // namespace vrv
