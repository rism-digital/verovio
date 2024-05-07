/////////////////////////////////////////////////////////////////////////////
// Name:        options.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "options.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <fstream>
#include <functional>
#include <iterator>
#include <sstream>

//----------------------------------------------------------------------------

#include "attconverter.h"
#include "vrv.h"
#include "vrvdef.h"

namespace vrv {

const std::map<int, std::string> Option::s_breaks = { { BREAKS_none, "none" }, { BREAKS_auto, "auto" },
    { BREAKS_line, "line" }, { BREAKS_smart, "smart" }, { BREAKS_encoded, "encoded" } };

const std::map<int, std::string> Option::s_condense
    = { { CONDENSE_none, "none" }, { CONDENSE_auto, "auto" }, { CONDENSE_encoded, "encoded" } };

const std::map<int, std::string> Option::s_elision = { { ELISION_regular, "regular" }, { ELISION_narrow, "narrow" },
    { ELISION_wide, "wide" }, { ELISION_unicode, "unicode" } };

const std::map<int, std::string> Option::s_fontFallback
    = { { FONT_FALLBACK_Leipzig, "Leipzig" }, { FONT_FALLBACK_Bravura, "Bravura" } };

const std::map<int, std::string> Option::s_footer
    = { { FOOTER_none, "none" }, { FOOTER_auto, "auto" }, { FOOTER_encoded, "encoded" }, { FOOTER_always, "always" } };

const std::map<int, std::string> Option::s_header
    = { { HEADER_none, "none" }, { HEADER_auto, "auto" }, { HEADER_encoded, "encoded" } };

const std::map<int, std::string> Option::s_multiRestStyle = { { MULTIRESTSTYLE_auto, "auto" },
    { MULTIRESTSTYLE_default, "default" }, { MULTIRESTSTYLE_block, "block" }, { MULTIRESTSTYLE_symbols, "symbols" } };

const std::map<int, std::string> Option::s_pedalStyle = { { PEDALSTYLE_NONE, "auto" }, { PEDALSTYLE_line, "line" },
    { PEDALSTYLE_pedstar, "pedstar" }, { PEDALSTYLE_altpedstar, "altpedstar" } };

const std::map<int, std::string> Option::s_systemDivider = { { SYSTEMDIVIDER_none, "none" },
    { SYSTEMDIVIDER_auto, "auto" }, { SYSTEMDIVIDER_left, "left" }, { SYSTEMDIVIDER_left_right, "left-right" } };

const std::map<int, std::string> Option::s_smuflTextFont
    = { { SMUFLTEXTFONT_embedded, "embedded" }, { SMUFLTEXTFONT_linked, "linked" }, { SMUFLTEXTFONT_none, "none" } };

//----------------------------------------------------------------------------
// Option
//----------------------------------------------------------------------------

void Option::SetInfo(const std::string &title, const std::string &description)
{
    m_title = title;
    m_description = description;
}

void Option::SetShortOption(char shortOption, bool isCmdOnly)
{
    m_shortOption = shortOption;
    m_isCmdOnly = isCmdOnly;
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

bool Option::SetValue(const std::string &value)
{
    // If not overriden
    LogError("Unsupported type string for %s", m_key.c_str());
    return false;
}

jsonxx::Object Option::ToJson() const
{
    jsonxx::Object opt;
    opt << "title" << this->GetTitle();
    opt << "description" << this->GetDescription();

    const OptionDbl *optDbl = dynamic_cast<const OptionDbl *>(this);
    const OptionInt *optInt = dynamic_cast<const OptionInt *>(this);
    const OptionIntMap *optIntMap = dynamic_cast<const OptionIntMap *>(this);
    const OptionString *optString = dynamic_cast<const OptionString *>(this);
    const OptionArray *optArray = dynamic_cast<const OptionArray *>(this);
    const OptionBool *optBool = dynamic_cast<const OptionBool *>(this);

    if (optBool) {
        opt << "type" << "bool";
        opt << "default" << optBool->GetDefault();
    }
    else if (optDbl) {
        opt << "type" << "double";
        jsonxx::Value value(optDbl->GetDefault());
        value.precision_ = 2;
        opt << "default" << value;
        value = optDbl->GetMin();
        value.precision_ = 2;
        opt << "min" << value;
        value = optDbl->GetMax();
        value.precision_ = 2;
        opt << "max" << value;
    }
    else if (optInt) {
        opt << "type" << "int";
        opt << "default" << optInt->GetDefault();
        opt << "min" << optInt->GetMin();
        opt << "max" << optInt->GetMax();
    }
    else if (optString) {
        opt << "type" << "std::string";
        opt << "default" << optString->GetDefault();
    }
    else if (optArray) {
        opt << "type" << "array";
        std::vector<std::string> strValues = optArray->GetDefault();
        std::vector<std::string>::iterator strIter;
        jsonxx::Array values;
        for (strIter = strValues.begin(); strIter != strValues.end(); ++strIter) {
            values << (*strIter);
        }
        opt << "default" << values;
    }
    else if (optIntMap) {
        opt << "type" << "std::string-list";
        opt << "default" << optIntMap->GetDefaultStrValue();
        std::vector<std::string> strValues = optIntMap->GetStrValues(false);
        std::vector<std::string>::iterator strIter;
        jsonxx::Array values;
        for (strIter = strValues.begin(); strIter != strValues.end(); ++strIter) {
            values << (*strIter);
        }
        opt << "values" << values;
    }

    if (this->IsCmdOnly()) {
        opt << "cmdOnly" << true;
    }
    if (this->GetShortOption()) {
        opt << "shortOption" << std::string(1, this->GetShortOption());
    }

    return opt;
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

bool OptionBool::SetValue(const std::string &value)
{
    bool b = (value == "true") ? true : false;
    return this->SetValue(b);
}

bool OptionBool::SetValueDbl(double value)
{
    return this->SetValue((bool)value);
}

bool OptionBool::SetValueBool(bool value)
{
    return this->SetValue(value);
}

std::string OptionBool::GetStrValue() const
{
    return (m_value) ? "true" : "false";
}

std::string OptionBool::GetDefaultStrValue() const
{
    return (m_defaultValue) ? "true" : "false";
}

void OptionBool::Reset()
{
    m_value = m_defaultValue;
}

bool OptionBool::IsSet() const
{
    return (m_value != m_defaultValue);
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

void OptionDbl::Init(double defaultValue, double minValue, double maxValue, bool definitionFactor)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
    m_minValue = minValue;
    m_maxValue = maxValue;
    m_definitionFactor = definitionFactor;
}

bool OptionDbl::SetValue(const std::string &value)
{
    if (!IsValidDouble(value)) {
        LogError("Unable to set parameter value %s for '%s'; conversion to double failed", value.c_str(),
            this->GetKey().c_str());
        return false;
    }
    // Convert string to double
    double number = std::strtod(value.c_str(), NULL);

    // Check bounds and set the value
    return this->SetValue(number);
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
    return this->SetValue(value);
}

double OptionDbl::GetValue() const
{
    return (m_definitionFactor) ? m_value * DEFINITION_FACTOR : m_value;
}

double OptionDbl::GetUnfactoredValue() const
{
    return m_value;
}

bool OptionDbl::SetValue(double value)
{
    if ((value < m_minValue) || (value > m_maxValue)) {
        LogError("Parameter value %f for '%s' out of bounds; default is %f, minimum %f, and maximum %f", value,
            this->GetKey().c_str(), m_defaultValue, m_minValue, m_maxValue);
        return false;
    }
    m_value = value;
    return true;
}

void OptionDbl::Reset()
{
    m_value = m_defaultValue;
}

bool OptionDbl::IsSet() const
{
    return (m_value != m_defaultValue);
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
    return this->SetValue((int)value);
}

bool OptionInt::SetValue(const std::string &value)
{
    if (!IsValidInteger(value)) {
        LogError("Unable to set parameter value %s for '%s'; conversion to integer failed", value.c_str(),
            this->GetKey().c_str());
        return false;
    }
    // Convert string to int
    int number = (int)std::strtol(value.c_str(), NULL, 10);

    // Check bounds and set the value
    return this->SetValue(number);
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
            this->GetKey().c_str(), m_defaultValue, m_minValue, m_maxValue);
        return false;
    }
    m_value = value;
    return true;
}

void OptionInt::Reset()
{
    m_value = m_defaultValue;
}

bool OptionInt::IsSet() const
{
    return (m_value != m_defaultValue);
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

void OptionString::Init(const std::string &defaultValue)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;
}

bool OptionString::SetValue(const std::string &value)
{
    m_value = value;
    return true;
}

void OptionString::Reset()
{
    m_value = m_defaultValue;
}

bool OptionString::IsSet() const
{
    return (m_value != m_defaultValue);
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
    m_values.clear();
    m_defaultValues.clear();
}

bool OptionArray::SetValueArray(const std::vector<std::string> &values)
{
    m_values = values;
    return true;
}

bool OptionArray::SetValue(const std::string &value)
{
    // Passing a single value to an array option adds it to the values and to not replace them
    if (!value.empty()) {
        m_values.push_back(value);
    }
    return true;
}

std::string OptionArray::GetStrValue() const
{
    return this->GetStr(m_values);
}

std::string OptionArray::GetDefaultStrValue() const
{
    return this->GetStr(m_defaultValues);
}

bool OptionArray::SetValue(std::vector<std::string> const &values)
{
    m_values = values;
    m_values.erase(std::remove_if(m_values.begin(), m_values.end(), [](const std::string &s) { return s.empty(); }),
        m_values.end());
    return true;
}

void OptionArray::Reset()
{
    m_values.clear();
}

bool OptionArray::IsSet() const
{
    return !m_values.empty();
}

std::string OptionArray::GetStr(const std::vector<std::string> &values) const
{
    std::stringstream ss;
    int i = 0;
    for (std::string const &value : values) {
        if (i != 0) {
            ss << ", ";
        }
        ss << "\"" << value << "\"";
        ++i;
    }
    return ss.str();
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

void OptionIntMap::Init(int defaultValue, const std::map<int, std::string> *values)
{
    m_value = defaultValue;
    m_defaultValue = defaultValue;

    m_values = values;
}

bool OptionIntMap::SetValue(const std::string &value)
{
    assert(m_values);

    std::map<int, std::string>::const_iterator it;
    for (it = m_values->cbegin(); it != m_values->cend(); ++it)
        if (it->second == value) {
            m_value = it->first;
            return true;
        }
    LogError("Parameter '%s' not valid for '%s'", value.c_str(), this->GetKey().c_str());
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
    std::map<int, std::string>::const_iterator it;
    for (it = m_values->cbegin(); it != m_values->cend(); ++it) {
        if (withoutDefault && (it->first == m_defaultValue)) {
            continue;
        }
        strValues.push_back(it->second);
    }

    return strValues;
}

std::string OptionIntMap::GetStrValuesAsStr(bool withoutDefault) const
{
    std::vector<std::string> strValues = this->GetStrValues(withoutDefault);
    std::stringstream ss;
    for (int i = 0; i < (int)strValues.size(); ++i) {
        if (i != 0) {
            ss << ", ";
        }
        ss << "\"" << strValues.at(i) << "\"";
    }
    return ss.str();
}

void OptionIntMap::Reset()
{
    m_value = m_defaultValue;
}

bool OptionIntMap::IsSet() const
{
    return (m_value != m_defaultValue);
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

bool OptionStaffrel::SetValue(const std::string &value)
{
    AttConverter converter;
    data_STAFFREL staffrel = converter.StrToStaffrel(value);
    if (staffrel == STAFFREL_NONE) {
        LogError("Parameter '%s' not valid", value.c_str());
        return false;
    }
    m_value = staffrel;
    return true;
}

std::string OptionStaffrel::GetStrValue() const
{
    AttConverter converter;
    return converter.StaffrelToStr(m_value);
}

std::string OptionStaffrel::GetDefaultStrValue() const
{
    AttConverter converter;
    return converter.StaffrelToStr(m_defaultValue);
}

void OptionStaffrel::Reset()
{
    m_value = m_defaultValue;
}

bool OptionStaffrel::IsSet() const
{
    return (m_value != m_defaultValue);
}

//----------------------------------------------------------------------------
// OptionJson
//----------------------------------------------------------------------------

void OptionJson::CopyTo(Option *option)
{
    OptionJson *child = dynamic_cast<OptionJson *>(option);
    assert(child);
    *child = *this;
}

void OptionJson::Init(JsonSource source, const std::string &defaultValue)
{
    m_source = source;
    this->ReadJson(m_defaultValues, defaultValue);
}

JsonSource OptionJson::GetSource() const
{
    return m_source;
}

jsonxx::Object OptionJson::GetValue(bool getDefault) const
{
    return getDefault ? m_defaultValues : m_values;
}

bool OptionJson::SetValue(const std::string &value)
{
    bool ok = this->ReadJson(m_values, value);
    if (!ok) {
        if (m_source == JsonSource::String) {
            LogError("Input json is not valid or contains errors");
        }
        else {
            // Input is file path
            if (value.empty()) {
                ok = true;
            }
            else {
                LogError("Input file '%s' is not valid or contains errors", value.c_str());
            }
        }
    }
    return ok;
}

std::string OptionJson::GetStrValue() const
{
    return m_values.json();
}

std::string OptionJson::GetDefaultStrValue() const
{
    return m_defaultValues.json();
}

void OptionJson::Reset()
{
    m_values.reset();
}

bool OptionJson::IsSet() const
{
    return (this->GetStrValue() != this->GetDefaultStrValue());
}

bool OptionJson::ReadJson(jsonxx::Object &output, const std::string &input) const
{
    bool ok = false;
    jsonxx::Object content;
    if (m_source == JsonSource::String) {
        ok = content.parse(input);
    }
    else {
        // Input is file path
        std::ifstream in(input.c_str());
        if (!in.is_open()) {
            return false;
        }
        ok = content.parse(in);
        in.close();
    }

    if (ok) {
        output = content;
    }
    return ok;
}

bool OptionJson::HasValue(const std::vector<std::string> &jsonNodePath) const
{
    const JsonPath valPath = StringPath2NodePath(m_values, jsonNodePath);
    const JsonPath defPath = StringPath2NodePath(m_defaultValues, jsonNodePath);

    return (valPath.size() == jsonNodePath.size()) || (defPath.size() == jsonNodePath.size());
}

int OptionJson::GetIntValue(const std::vector<std::string> &jsonNodePath, bool getDefault) const
{
    return static_cast<int>(this->GetDblValue(jsonNodePath, getDefault));
}

double OptionJson::GetDblValue(const std::vector<std::string> &jsonNodePath, bool getDefault) const
{
    JsonPath path = StringPath2NodePath(getDefault ? m_defaultValues : m_values, jsonNodePath);

    if (path.size() != jsonNodePath.size() && !getDefault) {
        path = StringPath2NodePath(m_defaultValues, jsonNodePath);
    }

    if ((path.size() != jsonNodePath.size()) || !path.back().get().is<jsonxx::Number>()) return 0;

    return path.back().get().get<jsonxx::Number>();
}

std::string OptionJson::GetStrValue(const std::vector<std::string> &jsonNodePath, bool getDefault) const
{
    JsonPath path = StringPath2NodePath(getDefault ? m_defaultValues : m_values, jsonNodePath);

    if ((path.size() != jsonNodePath.size()) && !getDefault) {
        path = StringPath2NodePath(m_defaultValues, jsonNodePath);
    }

    if ((path.size() != jsonNodePath.size()) || !path.back().get().is<jsonxx::String>()) return "";

    return path.back().get().get<jsonxx::String>();
}

bool OptionJson::UpdateNodeValue(const std::vector<std::string> &jsonNodePath, const std::string &value)
{
    if (jsonNodePath.empty()) {
        return false;
    }

    JsonPath path = StringPath2NodePath(m_values, jsonNodePath);
    if (path.size() != jsonNodePath.size()) {
        path = StringPath2NodePath(m_defaultValues, jsonNodePath);
    }

    if (path.size() != jsonNodePath.size()) {
        return false;
    }

    path.back().get().parse(value);
    return true;
}

std::set<std::string> OptionJson::GetKeys() const
{
    std::set<std::string> keys;
    for (const auto &mapEntry : m_values.kv_map()) {
        keys.insert(mapEntry.first);
    }
    return keys;
}

std::set<std::string> OptionJson::GetKeysByNode(const std::string &nodeName, std::list<std::string> &jsonNodePath) const
{
    std::set<std::string> keys;

    // Try to find node by the name, otherwise use root node
    const jsonxx::Object *node = FindNodeByName(m_values, nodeName, jsonNodePath);
    if (!node) node = &m_values;

    for (const auto &mapEntry : node->kv_map()) {
        keys.insert(mapEntry.first);
    }

    return keys;
}

OptionJson::JsonPath OptionJson::StringPath2NodePath(
    const jsonxx::Object &obj, const std::vector<std::string> &jsonNodePath) const
{
    JsonPath path;
    if (jsonNodePath.empty() || !obj.has<jsonxx::Value>(jsonNodePath.front())) {
        return path;
    }
    path.reserve(jsonNodePath.size());
    path.push_back(const_cast<jsonxx::Value &>(obj.get<jsonxx::Value>(jsonNodePath.front())));
    for (const std::string &jsonNode : jsonNodePath) {
        jsonxx::Value &val = path.back();
        if (val.is<jsonxx::Object>() && val.get<jsonxx::Object>().has<jsonxx::Value>(jsonNode)) {
            path.push_back(val.get<jsonxx::Object>().get<jsonxx::Value>(jsonNode));
        }
        else if (val.is<jsonxx::Array>()) {
            if (IsValidInteger(jsonNode)) {
                const int index = (int)std::strtol(jsonNode.c_str(), NULL, 10);
                if (!val.get<jsonxx::Array>().has<jsonxx::Value>(index)) break;

                path.push_back(val.get<jsonxx::Array>().get<jsonxx::Value>(index));
            }
            else {
                // invalid index, leaving
                break;
            }
        }
    }

    return path;
}

const jsonxx::Object *OptionJson::FindNodeByName(
    const jsonxx::Object &obj, const std::string &jsonNodeName, std::list<std::string> &jsonNodePath) const
{
    for (const auto &mapEntry : obj.kv_map()) {
        // skip non-objects
        if (!mapEntry.second->is<jsonxx::Object>()) continue;
        // return current object if name matches
        if (mapEntry.first == jsonNodeName) {
            jsonNodePath.emplace_back(mapEntry.first);
            return &mapEntry.second->get<jsonxx::Object>();
        }
        // otherwise recursively process current object
        else {
            const jsonxx::Object *result
                = FindNodeByName(mapEntry.second->get<jsonxx::Object>(), jsonNodeName, jsonNodePath);
            if (result) {
                jsonNodePath.emplace_front(mapEntry.first);
                return result;
            }
        }
    }

    return NULL;
}

//----------------------------------------------------------------------------
// Options
//----------------------------------------------------------------------------

Options::Options()
{
    /********* Base (short) option *********/

    // These are not registered in a group and not listed in Toolkit::GetOptions
    // There are listed in Toolkit::GetAvailableOptions through Options::GetBaseOptGrp

    m_baseOptions.SetLabel("Base short options", "0-base");
    m_baseOptions.SetCategory(OptionsCategory::Base);

    m_standardOutput.SetInfo(
        "Standard output", "Use \"-\" as input file or set the \"--stdin\" option for reading from the standard input");
    m_standardOutput.Init(false);
    m_standardOutput.SetKey("stdin");
    m_standardOutput.SetShortOption(' ', true);
    m_baseOptions.AddOption(&m_standardOutput);

    m_help.SetInfo("Help", "Display this message");
    m_help.Init("");
    m_help.SetKey("help");
    m_help.SetShortOption('h', true);
    m_baseOptions.AddOption(&m_help);

    m_allPages.SetInfo("All pages", "Output all pages");
    m_allPages.Init(false);
    m_allPages.SetKey("allPages");
    m_allPages.SetShortOption('a', true);
    m_baseOptions.AddOption(&m_allPages);

    m_inputFrom.SetInfo("Input from",
        "Select input format from: \"abc\", \"darms\", \"humdrum\", \"mei\", \"pae\", \"xml\" (musicxml)");
    m_inputFrom.Init("mei");
    m_inputFrom.SetKey("inputFrom");
    m_inputFrom.SetShortOption('f', false);
    m_baseOptions.AddOption(&m_inputFrom);

    m_logLevel.SetInfo("Log level", "Set the log level: \"off\", \"error\", \"warning\", \"info\", or \"debug\"");
    m_logLevel.Init("warning");
    m_logLevel.SetKey("logLevel");
    m_logLevel.SetShortOption('l', true);
    m_baseOptions.AddOption(&m_logLevel);

    m_outfile.SetInfo("Output file", "Output file name (use \"-\" as file name for standard output)");
    m_outfile.Init("svg");
    m_outfile.SetKey("outfile");
    m_outfile.SetShortOption('o', true);
    m_baseOptions.AddOption(&m_outfile);

    m_page.SetInfo("Page", "Select the page to engrave (default is 1)");
    m_page.Init(0, 0, 0);
    m_page.SetKey("page");
    m_page.SetShortOption('p', true);
    m_baseOptions.AddOption(&m_page);

    m_resourcePath.SetInfo("Resource path", "Path to the directory with Verovio resources");
    m_resourcePath.Init(VRV_RESOURCE_DIR);
    m_resourcePath.SetKey("resourcePath");
    m_resourcePath.SetShortOption('r', true);
    m_baseOptions.AddOption(&m_resourcePath);

    m_scale.SetInfo("Scale percent", "Scale of the output in percent (100 is normal size)");
    m_scale.Init(DEFAULT_SCALE, MIN_SCALE, MAX_SCALE);
    m_scale.SetKey("scale");
    m_scale.SetShortOption('s', false);
    m_baseOptions.AddOption(&m_scale);

    m_outputTo.SetInfo("Output to",
        "Select output format to: \"mei\", \"mei-pb\", \"mei-facs\", \"mei-basic\", \"svg\", \"midi\", \"timemap\", "
        "\"expansionmap\", \"humdrum\" or "
        "\"pae\"");
    m_outputTo.Init("svg");
    m_outputTo.SetKey("outputTo");
    m_outputTo.SetShortOption('t', true);
    m_baseOptions.AddOption(&m_outputTo);

    m_version.SetInfo("Version number", "Display the version number");
    m_version.Init(false);
    m_version.SetKey("version");
    m_version.SetShortOption('v', true);
    m_baseOptions.AddOption(&m_version);

    m_xmlIdSeed.SetInfo("XML IDs seed", "Seed the random number generator for XML IDs (default is random)");
    m_xmlIdSeed.Init(0, 0, -VRV_UNSET);
    m_xmlIdSeed.SetKey("xmlIdSeed");
    m_xmlIdSeed.SetShortOption('x', false);
    m_baseOptions.AddOption(&m_xmlIdSeed);

    /********* General *********/

    m_general.SetLabel("Input and page configuration options", "1-general");
    m_general.SetCategory(OptionsCategory::General);
    m_grps.push_back(&m_general);

    m_adjustPageHeight.SetInfo("Adjust page height", "Adjust the page height to the height of the content");
    m_adjustPageHeight.Init(false);
    this->Register(&m_adjustPageHeight, "adjustPageHeight", &m_general);

    m_adjustPageWidth.SetInfo("Adjust page width", "Adjust the page width to the width of the content");
    m_adjustPageWidth.Init(false);
    this->Register(&m_adjustPageWidth, "adjustPageWidth", &m_general);

    m_breaks.SetInfo("Breaks", "Define page and system breaks layout");
    m_breaks.Init(BREAKS_auto, &Option::s_breaks);
    this->Register(&m_breaks, "breaks", &m_general);

    m_breaksSmartSb.SetInfo("Smart breaks sb usage threshold",
        "In smart breaks mode, the portion of system width usage at which an encoded sb will be used");
    m_breaksSmartSb.Init(0.66, 0.0, 1.0);
    this->Register(&m_breaksSmartSb, "breaksSmartSb", &m_general);

    m_condense.SetInfo("Condense", "Control condensed score layout");
    m_condense.Init(CONDENSE_auto, &Option::s_condense);
    this->Register(&m_condense, "condense", &m_general);

    m_condenseFirstPage.SetInfo("Condense first page", "When condensing a score also condense the first page");
    m_condenseFirstPage.Init(false);
    this->Register(&m_condenseFirstPage, "condenseFirstPage", &m_general);

    m_condenseNotLastSystem.SetInfo(
        "Condense not last system", "When condensing a score never condense the last system");
    m_condenseNotLastSystem.Init(false);
    this->Register(&m_condenseNotLastSystem, "condenseNotLastSystem", &m_general);

    m_condenseTempoPages.SetInfo(
        "Condense tempo pages", "When condensing a score also condense pages with a tempo change");
    m_condenseTempoPages.Init(false);
    this->Register(&m_condenseTempoPages, "condenseTempoPages", &m_general);

    m_evenNoteSpacing.SetInfo("Even note spacing", "Align notes and rests without adding duration based space");
    m_evenNoteSpacing.Init(false);
    this->Register(&m_evenNoteSpacing, "evenNoteSpacing", &m_general);

    m_expand.SetInfo("Expand expansion", "Expand all referenced elements in the expansion <xml:id>");
    m_expand.Init("");
    this->Register(&m_expand, "expand", &m_general);

    m_footer.SetInfo("Footer", "Control footer layout");
    m_footer.Init(FOOTER_auto, &Option::s_footer);
    this->Register(&m_footer, "footer", &m_general);

    m_header.SetInfo("Header", "Control header layout");
    m_header.Init(HEADER_auto, &Option::s_header);
    this->Register(&m_header, "header", &m_general);

    m_humType.SetInfo("Humdrum type", "Include type attributes when importing from Humdrum");
    m_humType.Init(false);
    this->Register(&m_humType, "humType", &m_general);

    m_incip.SetInfo("Incip", "Read <incip> elements as data input");
    m_incip.Init(false);
    this->Register(&m_incip, "incip", &m_general);

    m_justifyVertically.SetInfo("Justify vertically", "Justify spacing vertically to fill the page");
    m_justifyVertically.Init(false);
    this->Register(&m_justifyVertically, "justifyVertically", &m_general);

    m_landscape.SetInfo("Landscape orientation", "Swap the values for page height and page width");
    m_landscape.Init(false);
    this->Register(&m_landscape, "landscape", &m_general);

    m_minLastJustification.SetInfo("Minimum last-system-justification width",
        "The last system is only justified if the unjustified width is greater than this percent");
    m_minLastJustification.Init(0.8, 0.0, 1.0);
    this->Register(&m_minLastJustification, "minLastJustification", &m_general);

    m_mmOutput.SetInfo("MM output", "Specify that the output in the SVG is given in mm (default is px)");
    m_mmOutput.Init(false);
    this->Register(&m_mmOutput, "mmOutput", &m_general);

    m_moveScoreDefinitionToStaff.SetInfo("Move score definition to staff",
        "Move score definition (clef, keySig, meterSig, etc.) from scoreDef to staffDef");
    m_moveScoreDefinitionToStaff.Init(false);
    this->Register(&m_moveScoreDefinitionToStaff, "moveScoreDefinitionToStaff", &m_general);

    m_neumeAsNote.SetInfo("Neume as note", "Render neumes as note heads instead of original notation");
    m_neumeAsNote.Init(false);
    this->Register(&m_neumeAsNote, "neumeAsNote", &m_general);

    m_noJustification.SetInfo("No justification", "Do not justify the system");
    m_noJustification.Init(false);
    this->Register(&m_noJustification, "noJustification", &m_general);

    m_openControlEvents.SetInfo("Open control event", "Render open control events");
    m_openControlEvents.Init(false);
    this->Register(&m_openControlEvents, "openControlEvents", &m_general);

    m_outputIndent.SetInfo("Output indentation", "Output indentation value for MEI and SVG");
    m_outputIndent.Init(3, 1, 10);
    this->Register(&m_outputIndent, "outputIndent", &m_general);

    m_outputFormatRaw.SetInfo(
        "Raw formatting for MEI output", "Writes MEI out with no line indenting or non-content newlines.");
    m_outputFormatRaw.Init(false);
    this->Register(&m_outputFormatRaw, "outputFormatRaw", &m_general);

    m_outputIndentTab.SetInfo("Output indentation with tab", "Output indentation with tabulation for MEI and SVG");
    m_outputIndentTab.Init(false);
    this->Register(&m_outputIndentTab, "outputIndentTab", &m_general);

    m_outputSmuflXmlEntities.SetInfo(
        "Output SMuFL XML entities", "Output SMuFL characters as XML entities instead of hex byte codes ");
    m_outputSmuflXmlEntities.Init(false);
    this->Register(&m_outputSmuflXmlEntities, "outputSmuflXmlEntities", &m_general);

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
    m_pageWidth.Init(2100, 100, 100000, true);
    this->Register(&m_pageWidth, "pageWidth", &m_general);

    m_pedalStyle.SetInfo("Pedal style", "The global pedal style");
    m_pedalStyle.Init(PEDALSTYLE_NONE, &Option::s_pedalStyle);
    this->Register(&m_pedalStyle, "pedalStyle", &m_general);

    m_preserveAnalyticalMarkup.SetInfo("Preserve analytical markup", "Preserves the analytical markup in MEI");
    m_preserveAnalyticalMarkup.Init(false);
    this->Register(&m_preserveAnalyticalMarkup, "preserveAnalyticalMarkup", &m_general);

    m_removeIds.SetInfo("Remove IDs in MEI", "Remove XML IDs in the MEI output that are not referenced");
    m_removeIds.Init(false);
    this->Register(&m_removeIds, "removeIds", &m_general);

    m_scaleToPageSize.SetInfo(
        "Scale to fit the page size", "Scale the content within the page instead of scaling the page itself");
    m_scaleToPageSize.Init(false);
    this->Register(&m_scaleToPageSize, "scaleToPageSize", &m_general);

    m_setLocale.SetInfo("Set the global locale", "Changes the global locale to C (this is not thread-safe)");
    m_setLocale.Init(false);
    this->Register(&m_setLocale, "setLocale", &m_general);

    m_showRuntime.SetInfo("Show runtime on CLI", "Display the total runtime on command-line");
    m_showRuntime.Init(false);
    this->Register(&m_showRuntime, "showRuntime", &m_general);

    m_shrinkToFit.SetInfo("Shrink content to fit page", "Scale down page content to fit the page height if needed");
    m_shrinkToFit.Init(false);
    this->Register(&m_shrinkToFit, "shrinkToFit", &m_general);

    m_smuflTextFont.SetInfo("Smufl text font", "Specify if the smufl text font is embedded, linked, or ignored");
    m_smuflTextFont.Init(SMUFLTEXTFONT_embedded, &Option::s_smuflTextFont);
    this->Register(&m_smuflTextFont, "smuflTextFont", &m_general);

    m_staccatoCenter.SetInfo(
        "Center staccato", "Align staccato and staccatissimo articulations with center of the note");
    m_staccatoCenter.Init(false);
    this->Register(&m_staccatoCenter, "staccatoCenter", &m_general);

    m_svgBoundingBoxes.SetInfo("Svg bounding boxes viewbox on svg root", "Include bounding boxes in SVG output");
    m_svgBoundingBoxes.Init(false);
    this->Register(&m_svgBoundingBoxes, "svgBoundingBoxes", &m_general);

    m_svgCss.SetInfo("SVG additional CSS", "CSS (as a string) to be added to the SVG output");
    m_svgCss.Init("");
    this->Register(&m_svgCss, "svgCss", &m_general);

    m_svgViewBox.SetInfo("Use viewbox on svg root", "Use viewBox on svg root element for easy scaling of document");
    m_svgViewBox.Init(false);
    this->Register(&m_svgViewBox, "svgViewBox", &m_general);

    m_svgHtml5.SetInfo("Output SVG for HTML5 embedding",
        "Write data-id and data-class attributes for JS usage and id clash avoidance");
    m_svgHtml5.Init(false);
    this->Register(&m_svgHtml5, "svgHtml5", &m_general);

    m_svgFormatRaw.SetInfo(
        "Raw formatting for SVG output", "Writes SVG out with no line indenting or non-content newlines");
    m_svgFormatRaw.Init(false);
    this->Register(&m_svgFormatRaw, "svgFormatRaw", &m_general);

    m_svgRemoveXlink.SetInfo("Remove xlink: from href attributes",
        "Removes the xlink: prefix on href attributes for compatibility with some newer browsers");
    m_svgRemoveXlink.Init(false);
    this->Register(&m_svgRemoveXlink, "svgRemoveXlink", &m_general);

    m_svgAdditionalAttribute.SetInfo("Add additional attribute in SVG",
        "Add additional attribute for graphical elements in SVG as \"data-*\", for "
        "example, \"note@pname\" would add a \"data-pname\" to all note elements");
    m_svgAdditionalAttribute.Init();
    this->Register(&m_svgAdditionalAttribute, "svgAdditionalAttribute", &m_general);

    m_unit.SetInfo("Unit", "The MEI unit (1⁄2 of the distance between the staff lines)");
    m_unit.Init(9.0, 4.5, 12.0, true);
    this->Register(&m_unit, "unit", &m_general);

    m_useBraceGlyph.SetInfo("Use Brace Glyph", "Use brace glyph from current font");
    m_useBraceGlyph.Init(false);
    this->Register(&m_useBraceGlyph, "useBraceGlyph", &m_general);

    m_useFacsimile.SetInfo(
        "Use facsimile for layout", "Use information in the <facsimile> element to control the layout");
    m_useFacsimile.Init(false);
    this->Register(&m_useFacsimile, "useFacsimile", &m_general);

    m_usePgFooterForAll.SetInfo("Use PgFooter for all", "Use the pgFooter for all pages");
    m_usePgFooterForAll.Init(false);
    this->Register(&m_usePgFooterForAll, "usePgFooterForAll", &m_general);

    m_usePgHeaderForAll.SetInfo("Use PgHeader for all", "Use the pgHeader for all pages");
    m_usePgHeaderForAll.Init(false);
    this->Register(&m_usePgHeaderForAll, "usePgHeaderForAll", &m_general);

    m_xmlIdChecksum.SetInfo(
        "XML IDs based on checksum", "Seed the generator for XML IDs using the checksum of the input data");
    m_xmlIdChecksum.Init(false);
    this->Register(&m_xmlIdChecksum, "xmlIdChecksum", &m_general);

    /********* General layout *********/

    m_generalLayout.SetLabel("General layout options", "2-generalLayout");
    m_generalLayout.SetCategory(OptionsCategory::Layout);
    m_grps.push_back(&m_generalLayout);

    m_barLineSeparation.SetInfo(
        "Barline separation", "The default distance between multiple barlines when locked together");
    m_barLineSeparation.Init(0.8, 0.5, 2.0);
    this->Register(&m_barLineSeparation, "barLineSeparation", &m_generalLayout);

    m_barLineWidth.SetInfo("Barline width", "The barline width");
    m_barLineWidth.Init(0.30, 0.10, 0.80);
    this->Register(&m_barLineWidth, "barLineWidth", &m_generalLayout);

    m_beamFrenchStyle.SetInfo(
        "French style of beams", "For notes in beams, stems will stop at first outermost sub-beam without crossing it");
    m_beamFrenchStyle.Init(false);
    this->Register(&m_beamFrenchStyle, "beamFrenchStyle", &m_generalLayout);

    m_beamMaxSlope.SetInfo("Beam max slope", "The maximum beam slope");
    m_beamMaxSlope.Init(10, 0, 20);
    this->Register(&m_beamMaxSlope, "beamMaxSlope", &m_generalLayout);

    m_beamMixedPreserve.SetInfo("Preserve mixed beams", "Mixed beams will be drawn even if there is not enough space");
    m_beamMixedPreserve.Init(false);
    this->Register(&m_beamMixedPreserve, "beamMixedPreserve", &m_generalLayout);

    m_beamMixedStemMin.SetInfo(
        "Minimal stem length of mixed beams", "The minimal stem length in MEI units used to draw mixed beams");
    m_beamMixedStemMin.Init(3.5, 1.0, 8.0);
    this->Register(&m_beamMixedStemMin, "beamMixedStemMin", &m_generalLayout);

    m_bracketThickness.SetInfo("Bracket thickness", "The thickness of the system bracket");
    m_bracketThickness.Init(1.0, 0.5, 2.0);
    this->Register(&m_bracketThickness, "bracketThickness", &m_generalLayout);

    m_breaksNoWidow.SetInfo(
        "Breaks no widow", "Prevent single measures on the last page by fitting it into previous system");
    m_breaksNoWidow.Init(false);
    this->Register(&m_breaksNoWidow, "breaksNoWidow", &m_generalLayout);

    // Optimized for five line staves
    constexpr double dashedBarLineLengthDefault = 8.0 / 7.0;
    m_dashedBarLineDashLength.SetInfo("Dashed barline dash length", "The dash length of dashed barlines");
    m_dashedBarLineDashLength.Init(dashedBarLineLengthDefault, 0.1, 5.0);
    this->Register(&m_dashedBarLineDashLength, "dashedBarLineDashLength", &m_generalLayout);

    m_dashedBarLineGapLength.SetInfo("Dashed barline gap length", "The gap length of dashed barlines");
    m_dashedBarLineGapLength.Init(dashedBarLineLengthDefault, 0.1, 5.0);
    this->Register(&m_dashedBarLineGapLength, "dashedBarLineGapLength", &m_generalLayout);

    m_dynamDist.SetInfo("Dynam dist", "The default distance from the staff for dynamic marks");
    m_dynamDist.Init(1.0, 0.5, 16.0);
    this->Register(&m_dynamDist, "dynamDist", &m_generalLayout);

    m_dynamSingleGlyphs.SetInfo("Dynam single glyphs", "Don't use SMuFL's predefined dynamics glyph combinations");
    m_dynamSingleGlyphs.Init(false);
    this->Register(&m_dynamSingleGlyphs, "dynamSingleGlyphs", &m_generalLayout);

    m_engravingDefaults.SetInfo("Engraving defaults", "Json describing defaults for engraving SMuFL elements");
    m_engravingDefaults.Init(JsonSource::String, "{}");
    this->Register(&m_engravingDefaults, "engravingDefaults", &m_generalLayout);

    m_engravingDefaultsFile.SetInfo(
        "Engraving defaults file", "Path to json file describing defaults for engraving SMuFL elements");
    m_engravingDefaultsFile.Init(JsonSource::FilePath, "");
    this->Register(&m_engravingDefaultsFile, "engravingDefaultsFile", &m_generalLayout);

    m_extenderLineMinSpace.SetInfo(
        "Extender line minimum space", "Minimum space required for extender line to be drawn");
    m_extenderLineMinSpace.Init(1.5, 1.5, 10.0);
    this->Register(&m_extenderLineMinSpace, "extenderLineMinSpace", &m_generalLayout);

    m_fingeringScale.SetInfo("Fingering scale", "The scale of fingering font compared to default font size");
    m_fingeringScale.Init(0.75, 0.25, 1);
    this->Register(&m_fingeringScale, "fingeringScale", &m_generalLayout);

    m_font.SetInfo("Font", "Set the music font");
    m_font.Init("Leipzig");
    this->Register(&m_font, "font", &m_generalLayout);

    m_fontAddCustom.SetInfo("Add custom font", "Add a custom music font as zip file");
    m_fontAddCustom.Init();
    this->Register(&m_fontAddCustom, "fontAddCustom", &m_generalLayout);

    m_fontFallback.SetInfo("Font fallback", "The music font fallback for missing glyphs");
    m_fontFallback.Init(FONT_FALLBACK_Leipzig, &Option::s_fontFallback);
    this->Register(&m_fontFallback, "fontFallback", &m_generalLayout);

    m_fontLoadAll.SetInfo("Font init all", "Load all music fonts");
    m_fontLoadAll.Init(false);
    this->Register(&m_fontLoadAll, "fontLoadAll", &m_generalLayout);

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

    m_hairpinThickness.SetInfo("Hairpin thickness", "The thickness of the hairpin");
    m_hairpinThickness.Init(0.2, 0.1, 0.8);
    this->Register(&m_hairpinThickness, "hairpinThickness", &m_generalLayout);

    m_handwrittenFont.SetInfo("Handwritten font", "Fonts that emulate hand writing and require special handling");
    m_handwrittenFont.Init();
    m_handwrittenFont.SetValue("Petaluma");
    this->Register(&m_handwrittenFont, "handwrittenFont", &m_generalLayout);

    m_harmDist.SetInfo("Harm dist", "The default distance from the staff of harmonic indications");
    m_harmDist.Init(1.0, 0.5, 16.0);
    this->Register(&m_harmDist, "harmDist", &m_generalLayout);

    m_justificationStaff.SetInfo("Spacing staff justification", "The staff justification");
    m_justificationStaff.Init(1., 0., 10.);
    this->Register(&m_justificationStaff, "justificationStaff", &m_generalLayout);

    m_justificationSystem.SetInfo("Spacing system justification", "The system spacing justification");
    m_justificationSystem.Init(1., 0., 10.);
    this->Register(&m_justificationSystem, "justificationSystem", &m_generalLayout);

    m_justificationBracketGroup.SetInfo(
        "Spacing bracket group justification", "Space between staves inside a bracketed group justification");
    m_justificationBracketGroup.Init(1., 0., 10.);
    this->Register(&m_justificationBracketGroup, "justificationBracketGroup", &m_generalLayout);

    m_justificationBraceGroup.SetInfo(
        "Spacing brace group justification", "Space between staves inside a braced group justification");
    m_justificationBraceGroup.Init(1., 0., 10.);
    this->Register(&m_justificationBraceGroup, "justificationBraceGroup", &m_generalLayout);

    m_justificationMaxVertical.SetInfo("Maximum ratio of justifiable height for page",
        "Maximum ratio of justifiable height to page height that can be used for the vertical justification");
    m_justificationMaxVertical.Init(0.3, 0.0, 1.0);
    this->Register(&m_justificationMaxVertical, "justificationMaxVertical", &m_generalLayout);

    m_ledgerLineThickness.SetInfo("Ledger line thickness", "The thickness of the ledger lines");
    m_ledgerLineThickness.Init(0.25, 0.10, 0.50);
    this->Register(&m_ledgerLineThickness, "ledgerLineThickness", &m_generalLayout);

    m_ledgerLineExtension.SetInfo(
        "Ledger line extension", "The amount by which a ledger line should extend either side of a notehead");
    m_ledgerLineExtension.Init(0.54, 0.20, 1.00);
    this->Register(&m_ledgerLineExtension, "ledgerLineExtension", &m_generalLayout);

    m_lyricElision.SetInfo("Lyric elision", "The lyric elision width");
    m_lyricElision.Init(ELISION_regular, &Option::s_elision);
    this->Register(&m_lyricElision, "lyricElision", &m_generalLayout);

    m_lyricLineThickness.SetInfo("Lyric line thickness", "The lyric extender line thickness");
    m_lyricLineThickness.Init(0.25, 0.10, 0.50);
    this->Register(&m_lyricLineThickness, "lyricLineThickness", &m_generalLayout);

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
    m_lyricWordSpace.Init(1.20, 0.00, 10.00);
    this->Register(&m_lyricWordSpace, "lyricWordSpace", &m_generalLayout);

    m_lyricVerseCollapse.SetInfo("Lyric verse collapse", "Collapse empty verse lines in lyrics");
    m_lyricVerseCollapse.Init(false);
    this->Register(&m_lyricVerseCollapse, "lyricVerseCollapse", &m_generalLayout);

    m_measureMinWidth.SetInfo("Measure min width", "The minimal measure width in MEI units");
    m_measureMinWidth.Init(15, 1, 30);
    this->Register(&m_measureMinWidth, "measureMinWidth", &m_generalLayout);

    m_mnumInterval.SetInfo("Measure Number Interval", "How frequently to place measure numbers");
    m_mnumInterval.Init(0, 0, 64, false);
    this->Register(&m_mnumInterval, "mnumInterval", &m_generalLayout);

    m_multiRestStyle.SetInfo("Multi rest style", "Rendering style of multiple measure rests");
    m_multiRestStyle.Init(MULTIRESTSTYLE_auto, &Option::s_multiRestStyle);
    this->Register(&m_multiRestStyle, "multiRestStyle", &m_generalLayout);

    m_multiRestThickness.SetInfo("Multi rest thickness", "The thickness of the multi rest in MEI units");
    m_multiRestThickness.Init(2.0, 0.50, 6.00);
    this->Register(&m_multiRestThickness, "multiRestThickness", &m_generalLayout);

    m_octaveAlternativeSymbols.SetInfo("Alternative octave symbols", "Use alternative symbols for displaying octaves");
    m_octaveAlternativeSymbols.Init(false);
    this->Register(&m_octaveAlternativeSymbols, "octaveAlternativeSymbols", &m_generalLayout);

    m_octaveLineThickness.SetInfo("Octave line thickness", "The thickness of the line used for an octave line");
    m_octaveLineThickness.Init(0.20, 0.10, 1.00);
    this->Register(&m_octaveLineThickness, "octaveLineThickness", &m_generalLayout);

    m_octaveNoSpanningParentheses.SetInfo("No parentheses on spanning octaves",
        "Do not enclose octaves that are spanning over systems with parentheses.");
    m_octaveNoSpanningParentheses.Init(false);
    this->Register(&m_octaveNoSpanningParentheses, "octaveNoSpanningParentheses", &m_generalLayout);

    m_pedalLineThickness.SetInfo("Pedal line thickness", "The thickness of the line used for piano pedaling");
    m_pedalLineThickness.Init(0.20, 0.10, 1.00);
    this->Register(&m_pedalLineThickness, "pedalLineThickness", &m_generalLayout);

    m_repeatBarLineDotSeparation.SetInfo("Repeat barline dot separation",
        "The default horizontal distance between the dots and the inner barline of a repeat barline");
    m_repeatBarLineDotSeparation.Init(0.36, 0.10, 1.00);
    this->Register(&m_repeatBarLineDotSeparation, "repeatBarLineDotSeparation", &m_generalLayout);

    m_repeatEndingLineThickness.SetInfo("Repeat ending line thickness", "Repeat and ending line thickness");
    m_repeatEndingLineThickness.Init(0.15, 0.10, 2.0);
    this->Register(&m_repeatEndingLineThickness, "repeatEndingLineThickness", &m_generalLayout);

    m_slurCurveFactor.SetInfo("Slur curve factor", "Slur curve factor - high value means rounder slurs");
    m_slurCurveFactor.Init(1.0, 0.2, 5.0);
    this->Register(&m_slurCurveFactor, "slurCurveFactor", &m_generalLayout);

    m_slurEndpointFlexibility.SetInfo(
        "Slur endpoint flexibility", "Slur endpoint flexibility - allow more endpoint movement during adjustment");
    m_slurEndpointFlexibility.Init(0.0, 0.0, 1.0);
    this->Register(&m_slurEndpointFlexibility, "slurEndpointFlexibility", &m_generalLayout);

    m_slurEndpointThickness.SetInfo("Slur endpoint thickness", "The endpoint slur thickness in MEI units");
    m_slurEndpointThickness.Init(0.1, 0.05, 0.25);
    this->Register(&m_slurEndpointThickness, "slurEndpointThickness", &m_generalLayout);

    m_slurMargin.SetInfo("Slur margin", "Slur safety distance in MEI units to obstacles");
    m_slurMargin.Init(1.0, 0.1, 4.0);
    this->Register(&m_slurMargin, "slurMargin", &m_generalLayout);

    m_slurMaxSlope.SetInfo("Slur max slope", "The maximum slur slope in degrees");
    m_slurMaxSlope.Init(60, 30, 85);
    this->Register(&m_slurMaxSlope, "slurMaxSlope", &m_generalLayout);

    m_slurMidpointThickness.SetInfo("Slur midpoint thickness", "The midpoint slur thickness in MEI units");
    m_slurMidpointThickness.Init(0.6, 0.2, 1.2);
    this->Register(&m_slurMidpointThickness, "slurMidpointThickness", &m_generalLayout);

    m_slurSymmetry.SetInfo("Slur symmetry", "Slur symmetry - high value means more symmetric slurs");
    m_slurSymmetry.Init(0.0, 0.0, 1.0);
    this->Register(&m_slurSymmetry, "slurSymmetry", &m_generalLayout);

    m_spacingBraceGroup.SetInfo(
        "Spacing brace group", "Minimum space between staves inside a braced group in MEI units");
    m_spacingBraceGroup.Init(12, 0, 48);
    this->Register(&m_spacingBraceGroup, "spacingBraceGroup", &m_generalLayout);

    m_spacingBracketGroup.SetInfo(
        "Spacing bracket group", "Minimum space between staves inside a bracketed group in MEI units");
    m_spacingBracketGroup.Init(12, 0, 48);
    this->Register(&m_spacingBracketGroup, "spacingBracketGroup", &m_generalLayout);

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
    m_spacingStaff.Init(12, 0, 48);
    this->Register(&m_spacingStaff, "spacingStaff", &m_generalLayout);

    m_spacingSystem.SetInfo("Spacing system", "The system minimal spacing in MEI units");
    m_spacingSystem.Init(4, 0, 48);
    this->Register(&m_spacingSystem, "spacingSystem", &m_generalLayout);

    m_staffLineWidth.SetInfo("Staff line width", "The staff line width in MEI units");
    m_staffLineWidth.Init(0.15, 0.10, 0.30);
    this->Register(&m_staffLineWidth, "staffLineWidth", &m_generalLayout);

    m_stemWidth.SetInfo("Stem width", "The stem width");
    m_stemWidth.Init(0.20, 0.10, 0.50);
    this->Register(&m_stemWidth, "stemWidth", &m_generalLayout);

    m_subBracketThickness.SetInfo("Sub bracket thickness", "The thickness of system sub-bracket");
    m_subBracketThickness.Init(0.20, 0.10, 2.0);
    this->Register(&m_subBracketThickness, "subBracketThickness", &m_generalLayout);

    m_systemDivider.SetInfo("System divider", "The display of system dividers");
    m_systemDivider.Init(SYSTEMDIVIDER_auto, &Option::s_systemDivider);
    this->Register(&m_systemDivider, "systemDivider", &m_generalLayout);

    m_systemMaxPerPage.SetInfo("Max. System per Page", "Maximum number of systems per page");
    m_systemMaxPerPage.Init(0, 0, 24);
    this->Register(&m_systemMaxPerPage, "systemMaxPerPage", &m_generalLayout);

    m_textEnclosureThickness.SetInfo("Text box line thickness", "The thickness of the line text enclosing box");
    m_textEnclosureThickness.Init(0.2, 0.10, 0.80);
    this->Register(&m_textEnclosureThickness, "textEnclosureThickness", &m_generalLayout);

    m_thickBarlineThickness.SetInfo("Thick barline thickness", "The thickness of the thick barline");
    m_thickBarlineThickness.Init(1.0, 0.5, 2.0);
    this->Register(&m_thickBarlineThickness, "thickBarlineThickness", &m_generalLayout);

    m_tieEndpointThickness.SetInfo("Tie Endpoint thickness", "The Endpoint tie thickness in MEI units");
    m_tieEndpointThickness.Init(0.1, 0.05, 0.25);
    this->Register(&m_tieEndpointThickness, "tieEndpointThickness", &m_generalLayout);

    m_tieMidpointThickness.SetInfo("Tie midpoint thickness", "The midpoint tie thickness in MEI units");
    m_tieMidpointThickness.Init(0.5, 0.2, 1.0);
    this->Register(&m_tieMidpointThickness, "tieMidpointThickness", &m_generalLayout);

    m_tieMinLength.SetInfo("Tie minimum length", "The minimum length of tie in MEI units");
    m_tieMinLength.Init(2.0, 0.0, 10.0);
    this->Register(&m_tieMinLength, "tieMinLength", &m_generalLayout);

    m_tupletAngledOnBeams.SetInfo("Tuplet angled on beams", "Tuplet brackets angled on beams only");
    m_tupletAngledOnBeams.Init(false);
    this->Register(&m_tupletAngledOnBeams, "tupletAngledOnBeams", &m_generalLayout);

    m_tupletBracketThickness.SetInfo("Tuplet bracket thickness", "The thickness of the tuplet bracket");
    m_tupletBracketThickness.Init(0.2, 0.1, 0.8);
    this->Register(&m_tupletBracketThickness, "tupletBracketThickness", &m_generalLayout);

    m_tupletNumHead.SetInfo("Tuplet number on head", "Placement of tuplet number on the side of the note head");
    m_tupletNumHead.Init(false);
    this->Register(&m_tupletNumHead, "tupletNumHead", &m_generalLayout);

    /********* selectors *********/

    m_selectors.SetLabel("Element selectors and processing", "3-selectors");
    m_selectors.SetCategory(OptionsCategory::Selectors);
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

    m_loadSelectedMdivOnly.SetInfo(
        "Load selected Mdiv only", "Load only the selected mdiv; the content of the other is skipped");
    m_loadSelectedMdivOnly.Init(false);
    this->Register(&m_loadSelectedMdivOnly, "loadSelectedMdivOnly", &m_selectors);

    m_mdivAll.SetInfo("Mdiv all", "Load and render all <mdiv> elements in the MEI files");
    m_mdivAll.Init(false);
    this->Register(&m_mdivAll, "mdivAll", &m_selectors);

    m_mdivXPathQuery.SetInfo("Mdiv xPath query",
        "Set the xPath query for selecting the <mdiv> to be rendered; only one <mdiv> can be rendered");
    m_mdivXPathQuery.Init("");
    this->Register(&m_mdivXPathQuery, "mdivXPathQuery", &m_selectors);

    m_substXPathQuery.SetInfo("Subst xPath query",
        "Set the xPath query for selecting <subst> child elements, for "
        "example: \"./del\"; by default the first child is selected");
    m_substXPathQuery.Init();
    this->Register(&m_substXPathQuery, "substXPathQuery", &m_selectors);

    m_transpose.SetInfo("Transpose the content", "Transpose the entire content");
    m_transpose.Init("");
    this->Register(&m_transpose, "transpose", &m_selectors);

    m_transposeMdiv.SetInfo(
        "Transpose individual mdivs", "Json mapping the mdiv ids to the corresponding transposition");
    m_transposeMdiv.Init(JsonSource::String, "{}");
    this->Register(&m_transposeMdiv, "transposeMdiv", &m_selectors);

    m_transposeSelectedOnly.SetInfo(
        "Transpose selected only", "Transpose only the selected content and ignore unselected editorial content");
    m_transposeSelectedOnly.Init(false);
    this->Register(&m_transposeSelectedOnly, "transposeSelectedOnly", &m_selectors);

    m_transposeToSoundingPitch.SetInfo(
        "Transpose to sounding pitch", "Transpose to sounding pitch by evaluating @trans.semi");
    m_transposeToSoundingPitch.Init(false);
    this->Register(&m_transposeToSoundingPitch, "transposeToSoundingPitch", &m_selectors);

    /********* The layout margins by element *********/

    m_elementMargins.SetLabel("Element margins", "4-elementMargins");
    m_elementMargins.SetCategory(OptionsCategory::Margins);
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

    /// custom bottom

    m_bottomMarginArtic.SetInfo("Bottom margin artic", "The margin for artic in MEI units");
    m_bottomMarginArtic.Init(0.75, 0.0, 10.0);
    this->Register(&m_bottomMarginArtic, "bottomMarginArtic", &m_elementMargins);

    m_bottomMarginHarm.SetInfo("Bottom margin harm", "The margin for harm in MEI units");
    m_bottomMarginHarm.Init(1.0, 0.0, 10.0);
    this->Register(&m_bottomMarginHarm, "bottomMarginHarm", &m_elementMargins);

    m_bottomMarginOctave.SetInfo("Bottom margin octave", "The margin for octave in MEI units");
    m_bottomMarginOctave.Init(1.0, 0.0, 10.0);
    this->Register(&m_bottomMarginOctave, "bottomMarginOctave", &m_elementMargins);

    m_bottomMarginPgHead.SetInfo("Bottom margin header", "The margin for header in MEI units");
    m_bottomMarginPgHead.Init(2.0, 0.0, 24.0);
    this->Register(&m_bottomMarginPgHead, "bottomMarginHeader", &m_elementMargins);

    /// custom left

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

    m_leftMarginTabDurSym.SetInfo("Left margin tabRhyhtm", "The margin for tabDurSym in MEI units");
    m_leftMarginTabDurSym.Init(1.0, 0.0, 2.0);
    this->Register(&m_leftMarginTabDurSym, "leftMarginTabDurSym", &m_elementMargins);

    /// custom right

    m_rightMarginAccid.SetInfo("Right margin accid", "The right margin for accid in MEI units");
    m_rightMarginAccid.Init(0.5, 0.0, 2.0);
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

    m_rightMarginTabDurSym.SetInfo("Right margin tabRhyhtm", "The right margin for tabDurSym in MEI units");
    m_rightMarginTabDurSym.Init(0.0, 0.0, 2.0);
    this->Register(&m_rightMarginTabDurSym, "rightMarginTabDurSym", &m_elementMargins);

    /// custom top

    m_topMarginArtic.SetInfo("Top margin artic", "The margin for artic in MEI units");
    m_topMarginArtic.Init(0.75, 0.0, 10.0);
    this->Register(&m_topMarginArtic, "topMarginArtic", &m_elementMargins);

    m_topMarginHarm.SetInfo("Top margin harm", "The margin for harm in MEI units");
    m_topMarginHarm.Init(1.0, 0.0, 10.0);
    this->Register(&m_topMarginHarm, "topMarginHarm", &m_elementMargins);

    m_topMarginPgFooter.SetInfo("Top margin footer", "The margin for footer in MEI units");
    m_topMarginPgFooter.Init(2.0, 0.0, 24.0);
    this->Register(&m_topMarginPgFooter, "topMarginPgFooter", &m_elementMargins);

    /********* midi *********/

    m_midi.SetLabel("Midi options", "5-midi");
    m_midi.SetCategory(OptionsCategory::Midi);
    m_grps.push_back(&m_midi);

    m_midiNoCue.SetInfo("MIDI playback of cue notes", "Skip cue notes in MIDI output");
    m_midiNoCue.Init(false);
    this->Register(&m_midiNoCue, "midiNoCue", &m_midi);

    m_midiTempoAdjustment.SetInfo("MIDI tempo adjustment", "The MIDI tempo adjustment factor");
    m_midiTempoAdjustment.Init(1.0, 0.2, 4.0);
    this->Register(&m_midiTempoAdjustment, "midiTempoAdjustment", &m_midi);

    /********* General *********/

    m_mensural.SetLabel("Mensural notation options", "6-mensural");
    m_mensural.SetCategory(OptionsCategory::Mensural);
    m_grps.push_back(&m_mensural);

    m_ligatureAsBracket.SetInfo("Ligature as bracket", "Render ligatures as bracket instead of original notation");
    m_ligatureAsBracket.Init(false);
    this->Register(&m_ligatureAsBracket, "ligatureAsBracket", &m_mensural);

    m_mensuralToMeasure.SetInfo("Mensural to measure", "Convert mensural sections to measure-based MEI");
    m_mensuralToMeasure.Init(false);
    this->Register(&m_mensuralToMeasure, "mensuralToMeasure", &m_mensural);

    /********* Deprecated options *********/

    /*
    m_deprecated.SetLabel("Deprecated options", "Deprecated");
    m_grps.push_back(&m_deprecated);

    m_condenseEncoded.SetInfo("Condense encoded", "Condense encoded layout rendering");
    m_condenseEncoded.Init(false);
    this->Register(&m_condenseEncoded, "condenseEncoded", &m_deprecated);
    */

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

void Options::Sync()
{
    if (!m_engravingDefaults.IsSet() && !m_engravingDefaultsFile.IsSet()) return;

    // We track all unmatched keys to generate appropriate errors later on
    std::set<std::string> unmatchedKeys = m_engravingDefaults.GetKeys();
    std::list<std::string> engravingDefaultsPath;
    std::set<std::string> otherKeys = m_engravingDefaultsFile.GetKeysByNode("engravingDefaults", engravingDefaultsPath);
    std::set_union(unmatchedKeys.begin(), unmatchedKeys.end(), otherKeys.begin(), otherKeys.end(),
        std::inserter(unmatchedKeys, unmatchedKeys.end()));

    // Override default or passed engravingDefaults with explicitly set values
    std::list<std::pair<std::string, OptionDbl *>> engravingDefaults = {
        { "staffLineThickness", &m_staffLineWidth }, //
        { "stemThickness", &m_stemWidth }, //
        { "legerLineThickness", &m_ledgerLineThickness }, //
        { "legerLineExtension", &m_ledgerLineExtension }, //
        { "slurEndpointThickness", &m_slurEndpointThickness }, //
        { "slurMidpointThickness", &m_slurMidpointThickness }, //
        { "tieEndpointThickness", &m_tieEndpointThickness }, //
        { "tieMidpointThickness", &m_tieMidpointThickness }, //
        { "thinBarlineThickness", &m_barLineWidth }, //
        { "thickBarlineThickness", &m_thickBarlineThickness }, //
        { "barlineSeparation", &m_barLineSeparation }, //
        { "repeatBarlineDotSeparation", &m_repeatBarLineDotSeparation }, //
        { "dashedBarlineDashLength", &m_dashedBarLineDashLength }, //
        { "dashedBarlineGapLength", &m_dashedBarLineGapLength }, //
        { "bracketThickness", &m_bracketThickness }, //
        { "subBracketThickness", &m_subBracketThickness }, //
        { "hairpinThickness", &m_hairpinThickness }, //
        { "octaveLineThickness", &m_octaveLineThickness }, //
        { "pedalLineThickness", &m_pedalLineThickness }, //
        { "repeatEndingLineThickness", &m_repeatEndingLineThickness }, //
        { "lyricLineThickness", &m_lyricLineThickness }, //
        { "tupletBracketThickness", &m_tupletBracketThickness }, //
        { "textEnclosureThickness", &m_textEnclosureThickness }, //
        { "hBarThickness", &m_multiRestThickness } //
    };

    for (const auto &pair : engravingDefaults) {
        std::vector<std::string> jsonNodePath = { engravingDefaultsPath.begin(), engravingDefaultsPath.end() };
        jsonNodePath.emplace_back(pair.first);

        double jsonValue = 0.0;
        if (m_engravingDefaultsFile.HasValue(jsonNodePath)) {
            jsonValue = m_engravingDefaultsFile.GetDblValue(jsonNodePath);
        }
        else if (m_engravingDefaults.HasValue({ pair.first })) {
            jsonValue = m_engravingDefaults.GetDblValue({ pair.first });
        }
        else {
            continue;
        }

        if (!pair.second->IsSet()) {
            pair.second->SetValueDbl(jsonValue * 2.0); // convert from staff spaces to MEI units
        }
        else if (jsonValue * 2.0 != pair.second->GetValue()) {
            LogWarning(
                "The engraving default '%s' is skipped because the corresponding option '%s' was set before to %f.",
                pair.first.c_str(), pair.second->GetKey().c_str(), pair.second->GetValue());
        }
        unmatchedKeys.erase(pair.first);
    }

    std::for_each(unmatchedKeys.cbegin(), unmatchedKeys.cend(),
        [](const std::string &key) { LogError("Unsupported engraving default '%s'", key.c_str()); });
}

void Options::Register(Option *option, const std::string &key, OptionGrp *grp)
{
    assert(option);
    assert(grp);

    m_items[key] = option;
    option->SetKey(key);
    grp->AddOption(option);
}

jsonxx::Object Options::GetBaseOptGrp()
{
    jsonxx::Object grpBase;
    jsonxx::Object baseOpts;
    grpBase << "name" << m_baseOptions.GetLabel();

    const std::vector<Option *> *options = this->GetBaseOptions();
    for (Option *option : *options) {
        baseOpts << option->GetKey() << option->ToJson();
    }

    grpBase << "options" << baseOpts;

    return grpBase;
}

const std::vector<Option *> *Options::GetBaseOptions() const
{
    return m_baseOptions.GetOptions();
}

} // namespace vrv
