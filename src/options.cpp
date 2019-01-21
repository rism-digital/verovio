/////////////////////////////////////////////////////////////////////////////
// Name:        options.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "options.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <sstream>

//----------------------------------------------------------------------------

#include "att.h"
#include "vrv.h"

namespace vrv {

std::map<int, std::string> Option::s_breaks
    = { { BREAKS_none, "none" }, { BREAKS_auto, "auto" }, { BREAKS_encoded, "encoded" } };

std::map<int, std::string> Option::s_measureNumber
    = { { MEASURENUMBER_system, "system" }, { MEASURENUMBER_interval, "interval" } };

//----------------------------------------------------------------------------
// Option
//----------------------------------------------------------------------------

void Option::CopyTo(Option *option)
{
    // Make sure it is overriden
    assert(false);
}

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

bool Option::SetValueArray(const std::vector<std::string> &values)
{
    // If not overriden
    LogError("Unsupported type array for %s", m_key.c_str());
    return false;
}

bool Option::SetValue(std::string value)
{
    // If not overriden
    LogError("Unsupported type string for %s", m_key.c_str());
    return false;
}

std::string Option::GetStrValue() const
{
    // If not overriden
    assert(false);
    return "[unspecified]";
}

std::string Option::GetDefaultStrValue() const
{
    // If not overriden
    assert(false);
    return "[unspecified]";
}

//----------------------------------------------------------------------------
// OptionBool
//----------------------------------------------------------------------------

void OptionBool::CopyTo(Option *option)
{
    OptionBool *child = dynamic_cast<OptionBool *>(option);
    assert(child);
    *child = *this;
}

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

std::string OptionBool::GetStrValue() const
{
    return (m_value) ? "true" : "false";
}

std::string OptionBool::GetDefaultStrValue() const
{
    return (m_defaultValue) ? "true" : "false";
}

bool OptionBool::SetValue(bool value)
{
    m_value = value;
    return true;
}

//----------------------------------------------------------------------------
// OptionDbl
//----------------------------------------------------------------------------

void OptionDbl::CopyTo(Option *option)
{
    OptionDbl *child = dynamic_cast<OptionDbl *>(option);
    assert(child);
    *child = *this;
}

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

std::string OptionDbl::GetStrValue() const
{
    return StringFormat("%f", m_value);
}

std::string OptionDbl::GetDefaultStrValue() const
{
    return StringFormat("%f", m_defaultValue);
}

bool OptionDbl::SetValueDbl(double value)
{
    return SetValue(value);
}

bool OptionDbl::SetValue(double value)
{
    if ((value < m_minValue) || (value > m_maxValue)) {
        LogError("Parameter value %f for '%s' out of bounds; default is %f, minimum %f, and maximum %f", value,
            GetKey().c_str(), m_defaultValue, m_minValue, m_maxValue);
        return false;
    }
    m_value = value;
    return true;
}

//----------------------------------------------------------------------------
// OptionInt
//----------------------------------------------------------------------------

void OptionInt::CopyTo(Option *option)
{
    OptionInt *child = dynamic_cast<OptionInt *>(option);
    assert(child);
    *child = *this;
}

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

std::string OptionInt::GetStrValue() const
{
    return StringFormat("%d", m_value);
}

std::string OptionInt::GetDefaultStrValue() const
{
    return StringFormat("%d", m_defaultValue);
}

int OptionInt::GetValue() const
{
    return (m_definitionFactor) ? m_value * DEFINITION_FACTOR : m_value;
}

int OptionInt::GetUnfactoredValue() const
{
    return m_value;
}

bool OptionInt::SetValue(int value)
{
    if ((value < m_minValue) || (value > m_maxValue)) {
        LogError("Parameter value %d for '%s' out of bounds; default is %d, minimum %d, and maximum %d", value,
            GetKey().c_str(), m_defaultValue, m_minValue, m_maxValue);
        return false;
    }
    m_value = value;
    return true;
}

//----------------------------------------------------------------------------
// OptionString
//----------------------------------------------------------------------------

void OptionString::CopyTo(Option *option)
{
    OptionString *child = dynamic_cast<OptionString *>(option);
    assert(child);
    *child = *this;
}

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

void OptionArray::CopyTo(Option *option)
{
    OptionArray *child = dynamic_cast<OptionArray *>(option);
    assert(child);
    *child = *this;
}

void OptionArray::Init()
{
    m_values.empty();
    m_defaultValues.empty();
}

bool OptionArray::SetValueArray(const std::vector<std::string> &values)
{
    m_values = values;
    // m_values.erase(std::remove_if(m_values.begin(), m_values.end(),
    //                                       [](const std::string &s) { return s.empty(); }),
    //                        m_values.end());
    return true;
}

bool OptionArray::SetValue(std::string value)
{
    // Passing a single value to an array option adds it to the values and to not replace them
    if (!value.empty()) {
        m_values.push_back(value);
    }
    return true;
}

std::string OptionArray::GetStrValue() const
{
    std::stringstream ss;
    int i;
    for (i = 0; i < (int)m_values.size(); ++i) {
        if (i != 0) {
            ss << ", ";
        }
        ss << "\"" << m_values.at(i) << "\"";
    }
    return ss.str();
}

std::string OptionArray::GetDefaultStrValue() const
{
    std::stringstream ss;
    int i;
    for (i = 0; i < (int)m_defaultValues.size(); ++i) {
        if (i != 0) {
            ss << ", ";
        }
        ss << "\"" << m_defaultValues.at(i) << "\"";
    }
    return ss.str();
}

bool OptionArray::SetValue(std::vector<std::string> const &values)
{
    m_values = values;
    m_values.erase(std::remove_if(m_values.begin(), m_values.end(), [](const std::string &s) { return s.empty(); }),
        m_values.end());
    return true;
}

//----------------------------------------------------------------------------
// OptionIntMap
//----------------------------------------------------------------------------

OptionIntMap::OptionIntMap()
{
    m_value = 0;
    m_defaultValue = 0;
    
    m_values = NULL;
}

void OptionIntMap::CopyTo(Option *option)
{
    OptionIntMap *child = dynamic_cast<OptionIntMap *>(option);
    assert(child);
    *child = *this;
}

void OptionIntMap::Init(int defaultValue, std::map<int, std::string> *values)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;

    m_values = values;
}

bool OptionIntMap::SetValue(std::string value)
{
    assert(m_values);

    std::map<int, std::string>::iterator it;
    for (it = m_values->begin(); it != m_values->end(); ++it)
        if (it->second == value) {
            m_value = it->first;
            return true;
        }
    LogError("Parameter '%s' not valid for '%s'", value.c_str(), GetKey().c_str());
    return false;
}

std::string OptionIntMap::GetStrValue() const
{
    assert(m_values);
    assert(m_values->count(m_value));

    return (m_values->at(m_value));
}

std::string OptionIntMap::GetDefaultStrValue() const
{
    assert(m_values);
    assert(m_values->count(m_defaultValue));

    return (m_values->at(m_defaultValue));
}

bool OptionIntMap::SetValue(int value)
{
    assert(m_values);
    assert(m_values->count(value));

    m_value = value;

    return true;
}

std::vector<std::string> OptionIntMap::GetStrValues(bool withoutDefault) const
{
    assert(m_values);

    std::vector<std::string> strValues;
    strValues.reserve(m_values->size());
    std::map<int, std::string>::iterator it;
    for (it = m_values->begin(); it != m_values->end(); ++it) {
        if (withoutDefault && (it->first == m_defaultValue)) {
            continue;
        }
        strValues.push_back(it->second);
    }

    return strValues;
}

std::string OptionIntMap::GetStrValuesAsStr(bool withoutDefault) const
{
    std::vector<std::string> strValues = GetStrValues(withoutDefault);
    std::stringstream ss;
    int i;
    for (i = 0; i < (int)strValues.size(); ++i) {
        if (i != 0) {
            ss << ", ";
        }
        ss << "\"" << strValues.at(i) << "\"";
    }
    return ss.str();
}

//----------------------------------------------------------------------------
// OptionStaffrel
//----------------------------------------------------------------------------

void OptionStaffrel::CopyTo(Option *option)
{
    OptionStaffrel *child = dynamic_cast<OptionStaffrel *>(option);
    assert(child);
    *child = *this;
}

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

std::string OptionStaffrel::GetStrValue() const
{
    Att converter;
    return converter.StaffrelToStr(m_value);
}

std::string OptionStaffrel::GetDefaultStrValue() const
{
    Att converter;
    return converter.StaffrelToStr(m_defaultValue);
}

//----------------------------------------------------------------------------
// Options
//----------------------------------------------------------------------------

Options::Options()
{
    /********* General *********/

    m_general.SetLabel("Input and page layout options", "1-general");
    m_grps.push_back(&m_general);

    m_adjustPageHeight.SetInfo("Adjust page height", "Crop the page height to the height of the content");
    m_adjustPageHeight.Init(false);
    this->Register(&m_adjustPageHeight, "adjustPageHeight", &m_general);

    m_breaks.SetInfo("Breaks", "Define page and system breaks layout");
    m_breaks.Init(BREAKS_auto, &Option::s_breaks);
    this->Register(&m_breaks, "breaks", &m_general);

    m_evenNoteSpacing.SetInfo("Even note spacing", "Specify the linear spacing factor");
    m_evenNoteSpacing.Init(false);
    this->Register(&m_evenNoteSpacing, "evenNoteSpacing", &m_general);

    m_humType.SetInfo("Humdrum type", "Include type attributes when importing from Humdrum");
    m_humType.Init(false);
    this->Register(&m_humType, "humType", &m_general);

    m_landscape.SetInfo("Landscape orientation", "The landscape paper orientation flag");
    m_landscape.Init(false);
    this->Register(&m_landscape, "landscape", &m_general);

    m_mensuralToMeasure.SetInfo("Mensural to measure", "Convert mensural sections to measure-based MEI");
    m_mensuralToMeasure.Init(false);
    this->Register(&m_mensuralToMeasure, "mensuralToMeasure", &m_general);

    m_mmOutput.SetInfo("MM output", "Specify that the output in the SVG is given in mm (default is px)");
    m_mmOutput.Init(false);
    this->Register(&m_mmOutput, "mmOutput", &m_general);

    m_noFooter.SetInfo("No footer", "Do not add any footer");
    m_noFooter.Init(false);
    this->Register(&m_noFooter, "noFooter", &m_general);

    m_noHeader.SetInfo("No header", "Do not add any header");
    m_noHeader.Init(false);
    this->Register(&m_noHeader, "noHeader", &m_general);

    m_noJustification.SetInfo("No justification", "Do not justify the system");
    m_noJustification.Init(false);
    this->Register(&m_noJustification, "noJustification", &m_general);
    
    m_openControlEvents.SetInfo("Open control event", "Render open control events");
    m_openControlEvents.Init(false);
    this->Register(&m_openControlEvents, "openControlEvents", &m_general);

    m_pageHeight.SetInfo("Page height", "The page height");
    m_pageHeight.Init(2970, 100, 60000, true);
    this->Register(&m_pageHeight, "pageHeight", &m_general);

    m_pageMarginBottom.SetInfo("Page bottom margin", "The page bottom margin");
    m_pageMarginBottom.Init(50, 0, 500, true);
    this->Register(&m_pageMarginBottom, "pageMarginBottom", &m_general);

    m_pageMarginLeft.SetInfo("Page left margin", "The page left margin");
    m_pageMarginLeft.Init(50, 0, 500, true);
    this->Register(&m_pageMarginLeft, "pageMarginLeft", &m_general);

    m_pageMarginRight.SetInfo("Page right margin", "The page right margin");
    m_pageMarginRight.Init(50, 0, 500, true);
    this->Register(&m_pageMarginRight, "pageMarginRight", &m_general);

    m_pageMarginTop.SetInfo("Page top margin", "The page top margin");
    m_pageMarginTop.Init(50, 0, 500, true);
    this->Register(&m_pageMarginTop, "pageMarginTop", &m_general);

    m_pageWidth.SetInfo("Page width", "The page width");
    m_pageWidth.Init(2100, 100, 60000, true);
    this->Register(&m_pageWidth, "pageWidth", &m_general);

    m_unit.SetInfo("Unit", "The MEI unit (1⁄2 of the distance between the staff lines)");
    m_unit.Init(9, 6, 20, true);
    this->Register(&m_unit, "unit", &m_general);

    /********* General layout *********/

    m_generalLayout.SetLabel("General layout options", "2-generalLayout");
    m_grps.push_back(&m_generalLayout);

    m_barLineWidth.SetInfo("Bar line width", "The barLine width");
    m_barLineWidth.Init(0.30, 0.10, 0.80);
    this->Register(&m_barLineWidth, "barLineWidth", &m_generalLayout);

    m_beamMaxSlope.SetInfo("Beam max slope", "The maximum beam slope");
    m_beamMaxSlope.Init(10, 1, 20);
    this->Register(&m_beamMaxSlope, "beamMaxSlope", &m_generalLayout);

    m_beamMinSlope.SetInfo("Beam min slope", "The minimum beam slope");
    m_beamMinSlope.Init(0, 0, 0);
    this->Register(&m_beamMinSlope, "beamMinSlope", &m_generalLayout);

    m_font.SetInfo("Font", "Set the music font");
    m_font.Init("Leipzig");
    this->Register(&m_font, "font", &m_generalLayout);

    m_graceFactor.SetInfo("Grace factor", "The grace size ratio numerator");
    m_graceFactor.Init(0.75, 0.5, 1.0);
    this->Register(&m_graceFactor, "graceFactor", &m_generalLayout);

    m_graceRhythmAlign.SetInfo("Grace rhythmic alignment", "Align grace notes rhythmically with all staves");
    m_graceRhythmAlign.Init(false);
    this->Register(&m_graceRhythmAlign, "graceRhythmAlign", &m_generalLayout);

    m_graceRightAlign.SetInfo("Grace right alignment", "Align the right position of a grace group with all staves");
    m_graceRightAlign.Init(false);
    this->Register(&m_graceRightAlign, "graceRightAlign", &m_generalLayout);

    m_hairpinSize.SetInfo("Hairpin size", "The haripin size in MEI units");
    m_hairpinSize.Init(3.0, 1.0, 8.0);
    this->Register(&m_hairpinSize, "hairpinSize", &m_generalLayout);

    m_leftPosition.SetInfo("Left position", "The left position");
    m_leftPosition.Init(0.8, 0.0, 2.0);
    this->Register(&m_leftPosition, "leftPosition", &m_generalLayout);

    m_lyricHyphenLength.SetInfo("Lyric hyphen length", "The lyric hyphen and dash length");
    m_lyricHyphenLength.Init(1.20, 0.50, 3.00);
    this->Register(&m_lyricHyphenLength, "lyricHyphenLength", &m_generalLayout);
    
    m_lyricHyphenWidth.SetInfo("Lyric hyphen width", "The lyric hyphen and dash width");
    m_lyricHyphenWidth.Init(0.20, 0.10, 0.50);
    this->Register(&m_lyricHyphenWidth, "lyricHyphenWidth", &m_generalLayout);

    m_lyricNoStartHyphen.SetInfo("Lyric no start hyphen", "Do not show hyphens at the beginning of a system");
    m_lyricNoStartHyphen.Init(false);
    this->Register(&m_lyricNoStartHyphen, "lyricNoStartHyphen", &m_generalLayout);
    
    m_lyricSize.SetInfo("Lyric size", "The lyrics size in MEI units");
    m_lyricSize.Init(4.5, 2.0, 8.0);
    this->Register(&m_lyricSize, "lyricSize", &m_generalLayout);

    m_lyricTopMinMargin.SetInfo("Lyric top min margin", "The minmal margin above the lyrics in MEI units");
    m_lyricTopMinMargin.Init(2.0, 0.0, 8.0);
    this->Register(&m_lyricTopMinMargin, "lyricTopMinMargin", &m_generalLayout);
    
    m_lyricWordSpace.SetInfo("Lyric word space", "The lyric word space length");
    m_lyricWordSpace.Init(1.20, 0.50, 3.00);
    this->Register(&m_lyricWordSpace, "lyricWordSpace", &m_generalLayout);

    m_measureMinWidth.SetInfo("Measure min width", "The minimal measure width in MEI units");
    m_measureMinWidth.Init(15, 1, 30);
    this->Register(&m_measureMinWidth, "minMeasureWidth", &m_generalLayout);

    m_measureNumber.SetInfo("Measure number", "The measure numbering rule (unused)");
    m_measureNumber.Init(MEASURENUMBER_system, &Option::s_measureNumber);
    this->Register(&m_measureNumber, "measureNumber", &m_generalLayout);

    m_slurControlPoints.SetInfo(
        "Slur control points", "Slur control points - higher value means more curved at the end");
    m_slurControlPoints.Init(5, 1, 10);
    this->Register(&m_slurControlPoints, "slurControlPointsr", &m_generalLayout);

    m_slurCurveFactor.SetInfo("Slur curve factor", "Slur curve factor - high value means rounder slurs");
    m_slurCurveFactor.Init(10, 1, 100);
    this->Register(&m_slurCurveFactor, "slurCurveFactor", &m_generalLayout);

    m_slurHeightFactor.SetInfo("Slur height factor", "Slur height factor -  high value means flatter slurs");
    m_slurHeightFactor.Init(5, 1, 100);
    this->Register(&m_slurHeightFactor, "slurHeightFactor", &m_generalLayout);

    m_slurMinHeight.SetInfo("Slur min height", "The minimum slur height in MEI units");
    m_slurMinHeight.Init(1.2, 0.3, 2.0);
    this->Register(&m_slurMinHeight, "slurMinHeight", &m_generalLayout);

    m_slurMaxHeight.SetInfo("Slur max height", "The maximum slur height in MEI units");
    m_slurMaxHeight.Init(3.0, 2.0, 6.0);
    this->Register(&m_slurMaxHeight, "slurMaxHeight", &m_generalLayout);

    m_slurMaxSlope.SetInfo("Slur max slope", "The maximum slur slope in degrees");
    m_slurMaxSlope.Init(20, 0, 45);
    this->Register(&m_slurMaxSlope, "slurMaxSlope", &m_generalLayout);

    m_slurThickness.SetInfo("Slur thickness", "The slur thickness in MEI units");
    m_slurThickness.Init(0.6, 0.2, 1.2);
    this->Register(&m_slurThickness, "slurThickness", &m_generalLayout);

    m_spacingDurDetection.SetInfo("Spacing dur detection", "Detect long duration for adjusting spacing");
    m_spacingDurDetection.Init(false);
    this->Register(&m_spacingDurDetection, "spacingDurDetection", &m_generalLayout);
    
    m_spacingLinear.SetInfo("Spacing linear", "Specify the linear spacing factor");
    m_spacingLinear.Init(0.25, 0.0, 1.0);
    this->Register(&m_spacingLinear, "spacingLinear", &m_generalLayout);

    m_spacingNonLinear.SetInfo("Spacing non linear", "Specify the non-linear spacing factor");
    m_spacingNonLinear.Init(0.6, 0.0, 1.0);
    this->Register(&m_spacingNonLinear, "spacingNonLinear", &m_generalLayout);

    m_spacingStaff.SetInfo("Spacing staff", "The staff minimal spacing in MEI units");
    m_spacingStaff.Init(8, 0, 24);
    this->Register(&m_spacingStaff, "spacingStaff", &m_generalLayout);

    m_spacingSystem.SetInfo("Spacing system", "The system minimal spacing in MEI units");
    m_spacingSystem.Init(3, 0, 12);
    this->Register(&m_spacingSystem, "spacingSystem", &m_generalLayout);

    m_staffLineWidth.SetInfo("Staff line width", "The staff line width in unit");
    m_staffLineWidth.Init(0.15, 0.10, 0.30);
    this->Register(&m_staffLineWidth, "staffLineWidth", &m_generalLayout);

    m_stemWidth.SetInfo("Stem width", "The stem width");
    m_stemWidth.Init(0.20, 0.10, 0.50);
    this->Register(&m_stemWidth, "stemWidth", &m_generalLayout);

    m_tieThickness.SetInfo("Tie thickness", "The tie thickness in MEI units");
    m_tieThickness.Init(0.5, 0.2, 1.0);
    this->Register(&m_tieThickness, "tieThickness", &m_generalLayout);

    /********* selectors *********/

    m_selectors.SetLabel("Element selectors", "3-selectors");
    m_grps.push_back(&m_selectors);

    m_appXPathQuery.SetInfo("App xPath query",
        "Set the xPath query for selecting <app> child elements, for example: "
        "\"./rdg[contains(@source, 'source-id')]\"; by default the <lem> or the "
        "first <rdg> is selected");
    m_appXPathQuery.Init();
    this->Register(&m_appXPathQuery, "appXPathQuery", &m_selectors);

    m_choiceXPathQuery.SetInfo("Choice xPath query",
        "Set the xPath query for selecting <choice> child elements, for "
        "example: \"./orig\"; by default the first child is selected");
    m_choiceXPathQuery.Init();
    this->Register(&m_choiceXPathQuery, "choiceXPathQuery", &m_selectors);

    m_mdivXPathQuery.SetInfo("Mdiv xPath query",
        "Set the xPath query for selecting the <mdiv> to be rendered; only one <mdiv> can be rendered");
    m_mdivXPathQuery.Init("");
    this->Register(&m_mdivXPathQuery, "mdivXPathQuery", &m_selectors);

    m_substXPathQuery.SetInfo("Subst xPath query",
        "Set the xPath query for selecting <subst> child elements, for "
        "example: \"./del\"; by default the first child is selected");
    m_substXPathQuery.Init();
    this->Register(&m_substXPathQuery, "substXPathQuery", &m_selectors);

    /********* The layout left margin by element *********/

    m_elementMargins.SetLabel("Element margins", "4-elementMargins");
    m_grps.push_back(&m_elementMargins);

    m_defaultBottomMargin.SetInfo("Default bottom margin", "The default bottom margin");
    m_defaultBottomMargin.Init(0.5, 0.0, 5.0);
    this->Register(&m_defaultBottomMargin, "defaultBottomMargin", &m_elementMargins);

    m_defaultLeftMargin.SetInfo("Default left margin", "The default left margin");
    m_defaultLeftMargin.Init(0.0, 0.0, 2.0);
    this->Register(&m_defaultLeftMargin, "defaultLeftMargin", &m_elementMargins);

    m_defaultRightMargin.SetInfo("Default right margin", "The default right margin");
    m_defaultRightMargin.Init(0.0, 0.0, 2.0);
    this->Register(&m_defaultRightMargin, "defaultRightMargin", &m_elementMargins);

    m_defaultTopMargin.SetInfo("Default top margin", "The default top margin");
    m_defaultTopMargin.Init(0.5, 0.0, 6.0);
    this->Register(&m_defaultTopMargin, "defaultTopMargin", &m_elementMargins);

    m_leftMarginAccid.SetInfo("Left margin accid", "The margin for accid in MEI units");
    m_leftMarginAccid.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginAccid, "leftMarginAccid", &m_elementMargins);

    m_leftMarginBarLine.SetInfo("Left margin barLine", "The margin for barLine in MEI units");
    m_leftMarginBarLine.Init(0.0, 0.0, 2.0);
    this->Register(&m_leftMarginBarLine, "leftMarginBarLine", &m_elementMargins);

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

    m_leftMarginLeftBarLine.SetInfo("Left margin left barLine", "The margin for left barLine in MEI units");
    m_leftMarginLeftBarLine.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginLeftBarLine, "leftMarginLeftBarLine", &m_elementMargins);

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

    m_leftMarginRightBarLine.SetInfo("Left margin right barLine", "The margin for right barLine in MEI units");
    m_leftMarginRightBarLine.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginRightBarLine, "leftMarginRightBarLine", &m_elementMargins);

    m_rightMarginAccid.SetInfo("Right margin accid", "The right margin for accid in MEI units");
    m_rightMarginAccid.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginAccid, "rightMarginAccid", &m_elementMargins);

    m_rightMarginBarLine.SetInfo("Right margin barLine", "The right margin for barLine in MEI units");
    m_rightMarginBarLine.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginBarLine, "rightMarginBarLine", &m_elementMargins);

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

    m_rightMarginLeftBarLine.SetInfo("Right margin left barLine", "The right margin for left barLine in MEI units");
    m_rightMarginLeftBarLine.Init(1.0, 0.0, 2.0);
    this->Register(&m_rightMarginLeftBarLine, "rightMarginLeftBarLine", &m_elementMargins);

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

    m_rightMarginRightBarLine.SetInfo("Right margin right barLine", "The right margin for right barLine in MEI units");
    m_rightMarginRightBarLine.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginRightBarLine, "rightMarginRightBarLine", &m_elementMargins);

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

Options::Options(const Options &options)
{
    LogDebug("Copy constructor not implemented for Options");
    assert(false);
}

Options &Options::operator=(const Options &options)
{
    // not self assignement
    if (this == &options) {
        return *this;
    }

    // Make sure we copy only the option values and not the pointers to members

    MapOfStrOptions const *items = options.GetItems();
    MapOfStrOptions::const_iterator iter;
    for (iter = items->begin(); iter != items->end(); ++iter) {
        Option *opt = m_items.at(iter->first);
        assert(opt);
        iter->second->CopyTo(opt);
    }

    return *this;
}

Options::~Options() {}

void Options::Register(Option *option, std::string key, OptionGrp *grp)
{
    assert(option);
    assert(grp);

    m_items[key] = option;
    option->SetKey(key);
    grp->AddOption(option);
}

} // namespace vrv
