/////////////////////////////////////////////////////////////////////////////
// Name:        toolkit.cpp
// Author:      Laurent Pugin
// Created:     17/10/2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "toolkit.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "attcomparison.h"
#include "custos.h"
#include "functorparams.h"
#include "iodarms.h"
#include "iohumdrum.h"
#include "iomei.h"
#include "iomusxml.h"
#include "iopae.h"
#include "layer.h"
#include "measure.h"
#include "nc.h"
#include "neume.h"
#include "note.h"
#include "options.h"
#include "page.h"
#include "slur.h"
#include "staff.h"
#include "svgdevicecontext.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"
#include "checked.h"
#include "jsonxx.h"
#include "unchecked.h"

namespace vrv {

const char *UTF_16_BE_BOM = "\xFE\xFF";
const char *UTF_16_LE_BOM = "\xFF\xFE";

//----------------------------------------------------------------------------
// Toolkit
//----------------------------------------------------------------------------

char *Toolkit::m_humdrumBuffer = NULL;

Toolkit::Toolkit(bool initFont)
{
    m_scale = DEFAULT_SCALE;
    m_format = AUTO;

    // default page size
    m_scoreBasedMei = false;

    m_humdrumBuffer = NULL;
    m_cString = NULL;

    if (initFont) {
        Resources::InitFonts();
    }

    m_options = m_doc.GetOptions();
}

Toolkit::~Toolkit()
{
    if (m_humdrumBuffer) {
        free(m_humdrumBuffer);
        m_humdrumBuffer = NULL;
    }
    if (m_cString) {
        free(m_cString);
        m_cString = NULL;
    }
}

bool Toolkit::SetResourcePath(const std::string &path)
{
    Resources::SetPath(path);
    return Resources::InitFonts();
}

bool Toolkit::SetScale(int scale)
{
    if (scale < MIN_SCALE || scale > MAX_SCALE) {
        LogError("Scale out of bounds; default is %d, minimum is %d, and maximum is %d", DEFAULT_SCALE, MIN_SCALE,
            MAX_SCALE);
        return false;
    }
    m_scale = scale;
    return true;
}

bool Toolkit::SetOutputFormat(std::string const &outformat)
{
    if ((outformat == "humdrum") || (outformat == "hum")) {
        m_outformat = HUMDRUM;
    }
    else if (outformat == "mei") {
        m_outformat = MEI;
    }
    else if (outformat == "midi") {
        m_outformat = MIDI;
    }
    else if (outformat == "timemap") {
        m_outformat = TIMEMAP;
    }
    else if (outformat != "svg") {
        LogError("Output format can only be: mei, humdrum, midi, timemap or svg");
        return false;
    }
    return true;
}

bool Toolkit::SetFormat(std::string const &informat)
{
    if (informat == "pae") {
        m_format = PAE;
    }
    else if (informat == "darms") {
        m_format = DARMS;
    }
    else if ((informat == "humdrum") || (informat == "hum")) {
        m_format = HUMDRUM;
    }
    else if (informat == "mei") {
        m_format = MEI;
    }
    else if ((informat == "musicxml") || (informat == "xml")) {
        m_format = MUSICXML;
    }
    else if (informat == "musicxml-hum") {
        m_format = MUSICXMLHUM;
    }
    else if (informat == "mei-hum") {
        m_format = MEIHUM;
    }
    else if (informat == "esac") {
        m_format = ESAC;
    }
    else if (informat == "auto") {
        m_format = AUTO;
    }
    else {
        LogError("Input format can only be: mei, humdrum, pae, musicxml or darms");
        return false;
    }
    return true;
}

FileFormat Toolkit::IdentifyInputFormat(const std::string &data)
{
#ifdef MUSICXML_DEFAULT_HUMDRUM
    FileFormat musicxmlDefault = MUSICXMLHUM;
#else
    FileFormat musicxmlDefault = MUSICXML;
#endif

    size_t searchLimit = 600;
    if (data.size() == 0) {
        return UNKNOWN;
    }
    if (data[0] == 0) {
        return UNKNOWN;
    }
    if (data[0] == '@') {
        return PAE;
    }
    if (data[0] == '*' || data[0] == '!') {
        return HUMDRUM;
    }
    if ((unsigned char)data[0] == 0xff || (unsigned char)data[0] == 0xfe) {
        // Handle UTF-16 content here later.
        std::cerr << "Warning: Cannot yet auto-detect format of UTF-16 data files." << std::endl;
        return UNKNOWN;
    }
    if (data[0] == '<') {
        // <mei> == root node for standard organization of MEI data
        // <pages> == root node for pages organization of MEI data
        // <score-partwise> == root node for part-wise organization of MusicXML data
        // <score-timewise> == root node for time-wise organization of MusicXML data
        // <opus> == root node for multi-movement/work organization of MusicXML data
        std::string initial = data.substr(0, searchLimit);

        if (initial.find("<mei ") != std::string::npos) {
            return MEI;
        }
        if (initial.find("<mei>") != std::string::npos) {
            return MEI;
        }
        if (initial.find("<music>") != std::string::npos) {
            return MEI;
        }
        if (initial.find("<music ") != std::string::npos) {
            return MEI;
        }
        if (initial.find("<pages>") != std::string::npos) {
            return MEI;
        }
        if (initial.find("<pages ") != std::string::npos) {
            return MEI;
        }
        if (initial.find("<score-partwise>") != std::string::npos) {
            return musicxmlDefault;
        }
        if (initial.find("<score-timewise>") != std::string::npos) {
            return musicxmlDefault;
        }
        if (initial.find("<opus>") != std::string::npos) {
            return musicxmlDefault;
        }
        if (initial.find("<score-partwise ") != std::string::npos) {
            return musicxmlDefault;
        }
        if (initial.find("<score-timewise ") != std::string::npos) {
            return musicxmlDefault;
        }
        if (initial.find("<opus ") != std::string::npos) {
            return musicxmlDefault;
        }

        std::cerr << "Warning: Trying to load unknown XML data which cannot be identified." << std::endl;
        return UNKNOWN;
    }

    // Assume that the input is MEI if other input types were not detected.
    // This means that DARMS cannot be auto detected.
    return MEI;
}

bool Toolkit::LoadFile(const std::string &filename)
{
    if (IsUTF16(filename)) {
        return LoadUTF16File(filename);
    }

    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        return false;
    }

    in.seekg(0, std::ios::end);
    std::streamsize fileSize = (std::streamsize)in.tellg();
    in.clear();
    in.seekg(0, std::ios::beg);

    // read the file into the std::string:
    std::string content(fileSize, 0);
    in.read(&content[0], fileSize);

    return LoadData(content);
}

bool Toolkit::IsUTF16(const std::string &filename)
{
    std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary);
    if (!fin.is_open()) {
        return false;
    }

    char data[2];
    memset(data, 0, 2);
    fin.read(data, 2);
    fin.close();

    if (memcmp(data, UTF_16_LE_BOM, 2) == 0) return true;
    if (memcmp(data, UTF_16_BE_BOM, 2) == 0) return true;

    return false;
}

bool Toolkit::LoadUTF16File(const std::string &filename)
{
    /// Loading a UTF-16 file with basic conversion ot UTF-8
    /// This is called after checking if the file has a UTF-16 BOM

    LogWarning("The file seems to be UTF-16 - trying to convert to UTF-8");

    std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary);
    if (!fin.is_open()) {
        return false;
    }

    fin.seekg(0, std::ios::end);
    std::streamsize wfileSize = (std::streamsize)fin.tellg();
    fin.clear();
    fin.seekg(0, std::wios::beg);

    std::vector<unsigned short> utf16line;
    utf16line.reserve(wfileSize / 2 + 1);

    unsigned short buffer;
    while (fin.read((char *)&buffer, sizeof(unsigned short))) {
        utf16line.push_back(buffer);
    }
    // LogDebug("%d %d", wfileSize, utf8line.size());

    std::string utf8line;
    utf8::utf16to8(utf16line.begin(), utf16line.end(), back_inserter(utf8line));

    return LoadData(utf8line);
}

bool Toolkit::LoadData(const std::string &data)
{
    std::string newData;
    FileInputStream *input = NULL;

    auto inputFormat = m_format;
    if (inputFormat == AUTO) {
        inputFormat = IdentifyInputFormat(data);
    }

    if (inputFormat == PAE) {
#ifndef NO_PAE_SUPPORT
        input = new PaeInput(&m_doc, "");
#else
        LogError("Plaine & Easie import is not supported in this build.");
        return false;
#endif
    }
    else if (inputFormat == DARMS) {
#ifndef NO_DARMS_SUPPORT
        input = new DarmsInput(&m_doc, "");
#else
        LogError("DARMS import is not supported in this build.");
        return false;
#endif
    }
#ifndef NO_HUMDRUM_SUPPORT
    else if (inputFormat == HUMDRUM) {
        // LogMessage("Importing Humdrum data");

        Doc tempdoc;
        tempdoc.SetOptions(m_doc.GetOptions());
        HumdrumInput *tempinput = new HumdrumInput(&tempdoc, "");
        if (GetOutputFormat() == HUMDRUM) {
            tempinput->SetOutputFormat("humdrum");
        }

        if (!tempinput->ImportString(data)) {
            LogError("Error importing Humdrum data");
            delete tempinput;
            return false;
        }

        SetHumdrumBuffer(tempinput->GetHumdrumString().c_str());

        if (GetOutputFormat() == HUMDRUM) {
            return true;
        }

        MeiOutput meioutput(&tempdoc, "");
        meioutput.SetScoreBasedMEI(true);
        newData = meioutput.GetOutput();
        delete tempinput;

        input = new MeiInput(&m_doc, "");
    }
#endif
    else if (inputFormat == MEI) {
        input = new MeiInput(&m_doc, "");
    }
    else if (inputFormat == MUSICXML) {
        // This is the direct converter from MusicXML to MEI using iomusicxml:
        input = new MusicXmlInput(&m_doc, "");
    }
#ifndef NO_HUMDRUM_SUPPORT
    else if (inputFormat == MUSICXMLHUM) {
        // This is the indirect converter from MusicXML to MEI using iohumdrum:
        hum::Tool_musicxml2hum converter;
        pugi::xml_document xmlfile;
        xmlfile.load(data.c_str());
        stringstream conversion;
        bool status = converter.convert(conversion, xmlfile);
        if (!status) {
            LogError("Error converting MusicXML data");
            return false;
        }
        std::string buffer = conversion.str();
        SetHumdrumBuffer(buffer.c_str());

        // Now convert Humdrum into MEI:
        Doc tempdoc;
        tempdoc.SetOptions(m_doc.GetOptions());
        FileInputStream *tempinput = new HumdrumInput(&tempdoc, "");
        if (!tempinput->ImportString(conversion.str())) {
            LogError("Error importing Humdrum data");
            delete tempinput;
            return false;
        }
        MeiOutput meioutput(&tempdoc, "");
        meioutput.SetScoreBasedMEI(true);
        newData = meioutput.GetOutput();
        delete tempinput;
        input = new MeiInput(&m_doc, "");
    }

    else if (inputFormat == MEIHUM) {
        // This is the indirect converter from MusicXML to MEI using iohumdrum:
        hum::Tool_mei2hum converter;
        pugi::xml_document xmlfile;
        xmlfile.load(data.c_str());
        stringstream conversion;
        bool status = converter.convert(conversion, xmlfile);
        if (!status) {
            LogError("Error converting MEI data");
            return false;
        }
        std::string buffer = conversion.str();
        SetHumdrumBuffer(buffer.c_str());

        // Now convert Humdrum into MEI:
        Doc tempdoc;
        tempdoc.SetOptions(m_doc.GetOptions());
        FileInputStream *tempinput = new HumdrumInput(&tempdoc, "");
        if (!tempinput->ImportString(conversion.str())) {
            LogError("Error importing Humdrum data");
            delete tempinput;
            return false;
        }
        MeiOutput meioutput(&tempdoc, "");
        meioutput.SetScoreBasedMEI(true);
        newData = meioutput.GetOutput();
        delete tempinput;
        input = new MeiInput(&m_doc, "");
    }

    else if (inputFormat == ESAC) {
        // This is the indirect converter from EsAC to MEI using iohumdrum:
        hum::Tool_esac2hum converter;
        stringstream conversion;
        bool status = converter.convert(conversion, data);
        if (!status) {
            LogError("Error converting EsAC data");
            return false;
        }
        std::string buffer = conversion.str();
        SetHumdrumBuffer(buffer.c_str());

        // Now convert Humdrum into MEI:
        Doc tempdoc;
        tempdoc.SetOptions(m_doc.GetOptions());
        FileInputStream *tempinput = new HumdrumInput(&tempdoc, "");
        if (!tempinput->ImportString(conversion.str())) {
            LogError("Error importing Humdrum data");
            delete tempinput;
            return false;
        }
        MeiOutput meioutput(&tempdoc, "");
        meioutput.SetScoreBasedMEI(true);
        newData = meioutput.GetOutput();
        delete tempinput;
        input = new MeiInput(&m_doc, "");
    }
#endif
    else {
        LogMessage("Unsupported format");
        return false;
    }

    // something went wrong
    if (!input) {
        LogError("Unknown error");
        return false;
    }

    // load the file
    if (!input->ImportString(newData.size() ? newData : data)) {
        LogError("Error importing data");
        delete input;
        return false;
    }

    // generate the page header and footer if necessary
    if (true) { // change this to an option
        m_doc.GenerateHeaderAndFooter();
    }

    // generate missing measure numbers
    m_doc.GenerateMeasureNumbers();

    m_doc.PrepareDrawing();

    // Convert pseudo-measures into distinct segments based on barLine elements
    if (m_doc.IsMensuralMusicOnly()) {
        m_doc.ConvertToCastOffMensuralDoc();
    }

    // Do the layout? this depends on the options and the file. PAE and
    // DARMS have no layout information. MEI files _can_ have it, but it
    // might have been ignored because of the --breaks auto option.
    // Regardless, we won't do layout if the --breaks none option was set.
    if (m_options->m_breaks.GetValue() != BREAKS_none) {
        if (input->HasLayoutInformation() && (m_options->m_breaks.GetValue() == BREAKS_encoded)) {
            // LogElapsedTimeStart();
            m_doc.CastOffEncodingDoc();
            // LogElapsedTimeEnd("layout");
        }
        else {
            if (m_options->m_breaks.GetValue() == BREAKS_encoded) {
                LogWarning("Requesting layout with encoded breaks but nothing provided in the data");
            }
            // LogElapsedTimeStart();
            m_doc.CastOffDoc();
            // LogElapsedTimeEnd("layout");
        }
    }

    delete input;
    m_view.SetDoc(&m_doc);

    return true;
}

std::string Toolkit::GetMEI(int pageNo, bool scoreBased)
{
    // Page number is one-based - correct it to 0-based first
    pageNo--;

    MeiOutput meioutput(&m_doc, "");
    meioutput.SetScoreBasedMEI(scoreBased);
    return meioutput.GetOutput(pageNo);
}

bool Toolkit::SaveFile(const std::string &filename)
{
    MeiOutput meioutput(&m_doc, filename.c_str());
    meioutput.SetScoreBasedMEI(m_scoreBasedMei);
    if (!meioutput.ExportFile()) {
        LogError("Unknown error");
        return false;
    }
    return true;
}

std::string Toolkit::GetOptions(bool defaultValues) const
{
    jsonxx::Object o;

    const MapOfStrOptions *params = m_options->GetItems();
    MapOfStrOptions::const_iterator iter;
    for (iter = params->begin(); iter != params->end(); ++iter) {
        const OptionDbl *optDbl = dynamic_cast<const OptionDbl *>(iter->second);
        const OptionInt *optInt = dynamic_cast<const OptionInt *>(iter->second);
        const OptionBool *optBool = dynamic_cast<const OptionBool *>(iter->second);
        const OptionArray *optArray = dynamic_cast<const OptionArray *>(iter->second);

        if (optDbl) {
            double dblValue = (defaultValues) ? optDbl->GetDefault() : optDbl->GetValue();
            jsonxx::Value value(dblValue);
            value.precision_ = 2;
            o << iter->first << value;
        }
        else if (optInt) {
            int intValue = (defaultValues) ? optInt->GetDefault() : optInt->GetUnfactoredValue();
            o << iter->first << intValue;
        }
        else if (optBool) {
            bool boolValue = (defaultValues) ? optBool->GetDefault() : optBool->GetValue();
            o << iter->first << boolValue;
        }
        else if (optArray) {
            std::vector<std::string> strValues = (defaultValues) ? optArray->GetDefault() : optArray->GetValue();
            std::vector<std::string>::iterator strIter;
            jsonxx::Array values;
            for (strIter = strValues.begin(); strIter != strValues.end(); ++strIter) {
                values << (*strIter);
            }
            o << iter->first << values;
        }
        else {
            std::string stringValue
                = (defaultValues) ? iter->second->GetDefaultStrValue() : iter->second->GetStrValue();
            o << iter->first << stringValue;
        }
    }

    return o.json();
}

std::string Toolkit::GetAvailableOptions() const
{
    jsonxx::Object o;
    jsonxx::Object grps;

    std::vector<OptionGrp *> *grp = m_options->GetGrps();
    std::vector<OptionGrp *>::iterator grpIter;

    for (grpIter = grp->begin(); grpIter != grp->end(); ++grpIter) {

        jsonxx::Object grp;
        grp << "name" << (*grpIter)->GetLabel();

        jsonxx::Object opts;

        const std::vector<Option *> *options = (*grpIter)->GetOptions();
        std::vector<Option *>::const_iterator iter;

        for (iter = options->begin(); iter != options->end(); ++iter) {

            jsonxx::Object opt;
            opt << "title" << (*iter)->GetTitle();
            opt << "description" << (*iter)->GetDescription();

            const OptionDbl *optDbl = dynamic_cast<const OptionDbl *>(*iter);
            const OptionInt *optInt = dynamic_cast<const OptionInt *>(*iter);
            const OptionIntMap *optIntMap = dynamic_cast<const OptionIntMap *>(*iter);
            const OptionString *optString = dynamic_cast<const OptionString *>(*iter);
            const OptionArray *optArray = dynamic_cast<const OptionArray *>(*iter);
            const OptionBool *optBool = dynamic_cast<const OptionBool *>(*iter);

            if (optBool) {
                opt << "type"
                    << "bool";
                opt << "default" << optBool->GetDefault();
            }
            else if (optDbl) {
                opt << "type"
                    << "double";
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
                opt << "type"
                    << "int";
                opt << "default" << optInt->GetDefault();
                opt << "min" << optInt->GetMin();
                opt << "max" << optInt->GetMax();
            }
            else if (optString) {
                opt << "type"
                    << "std::string";
                opt << "default" << optString->GetDefault();
            }
            else if (optArray) {
                opt << "type"
                    << "array";
                std::vector<std::string> strValues = optArray->GetDefault();
                std::vector<std::string>::iterator strIter;
                jsonxx::Array values;
                for (strIter = strValues.begin(); strIter != strValues.end(); ++strIter) {
                    values << (*strIter);
                }
                opt << "default" << values;
            }
            else if (optIntMap) {
                opt << "type"
                    << "std::string-list";
                opt << "default" << optIntMap->GetDefaultStrValue();
                std::vector<std::string> strValues = optIntMap->GetStrValues(false);
                std::vector<std::string>::iterator strIter;
                jsonxx::Array values;
                for (strIter = strValues.begin(); strIter != strValues.end(); ++strIter) {
                    values << (*strIter);
                }
                opt << "values" << values;
            }

            opts << (*iter)->GetKey() << opt;
        }

        grp << "options" << opts;
        grps << (*grpIter)->GetId() << grp;
    }

    o << "groups" << grps;

    return o.json();
}

bool Toolkit::SetOptions(const std::string &json_options)
{
    jsonxx::Object json;

    // Read JSON options
    if (!json.parse(json_options)) {
        LogError("Can not parse JSON std::string.");
        return false;
    }

    std::map<std::string, jsonxx::Value *> jsonMap = json.kv_map();
    std::map<std::string, jsonxx::Value *>::const_iterator iter;
    for (iter = jsonMap.begin(); iter != jsonMap.end(); ++iter) {
        if (m_options->GetItems()->count(iter->first) == 0) {
            // Base options
            if (iter->first == "inputFormat") {
                if (json.has<jsonxx::String>("inputFormat")) {
                    SetFormat(json.get<jsonxx::String>("inputFormat"));
                }
            }
            else if (iter->first == "scale") {
                if (json.has<jsonxx::Number>("scale")) {
                    SetScale(json.get<jsonxx::Number>("scale"));
                }
            }
            else if (iter->first == "xmlIdSeed") {
                if (json.has<jsonxx::Number>("xmlIdSeed")) {
                    Object::SeedUuid(json.get<jsonxx::Number>("xmlIdSeed"));
                }
            }
            // Deprecated option
            else if (iter->first == "appXPathQueries") {
                LogWarning("Option appXPathQueries is deprecated; use appXPathQuery with an array instead");
                jsonxx::Array values = json.get<jsonxx::Array>("appXPathQueries");
                std::vector<std::string> queries;
                Option *opt = m_options->GetItems()->at("appXPathQuery");
                assert(opt);
                int i;
                for (i = 0; i < (int)values.size(); ++i) {
                    if (values.has<jsonxx::String>(i)) queries.push_back(values.get<jsonxx::String>(i));
                }
                opt->SetValueArray(queries);
            }
            else if (iter->first == "border") {
                LogWarning("Option border is deprecated; use pageMarginBottom, pageMarginLeft, pageMarginRight and "
                           "pageMarginTop instead");
                Option *opt = NULL;
                if (json.has<jsonxx::Number>("border")) {
                    double border = json.get<jsonxx::Number>("border");
                    opt = m_options->GetItems()->at("pageMarginBottom");
                    assert(opt);
                    opt->SetValueDbl(border);
                    opt = m_options->GetItems()->at("pageMarginLeft");
                    assert(opt);
                    opt->SetValueDbl(border);
                    opt = m_options->GetItems()->at("pageMarginRight");
                    assert(opt);
                    opt->SetValueDbl(border);
                    opt = m_options->GetItems()->at("pageMarginTop");
                    assert(opt);
                    opt->SetValueDbl(border);
                }
            }
            else if (iter->first == "choiceXPathQueries") {
                LogWarning("Option choiceXPathQueries is deprecated; use choiceXPathQuery with an array instead");
                jsonxx::Array values = json.get<jsonxx::Array>("choiceXPathQueries");
                std::vector<std::string> queries;
                Option *opt = m_options->GetItems()->at("choiceXPathQuery");
                assert(opt);
                int i;
                for (i = 0; i < (int)values.size(); ++i) {
                    if (values.has<jsonxx::String>(i)) queries.push_back(values.get<jsonxx::String>(i));
                }
                opt->SetValueArray(queries);
            }
            else if (iter->first == "ignoreLayout") {
                LogWarning("Option ignoreLayout is deprecated; use breaks: \"auto\"|\"encoded\" instead");
                Option *opt = NULL;
                opt = m_options->GetItems()->at("breaks");
                assert(opt);
                if (json.has<jsonxx::Number>("ignoreLayout")) {
                    if ((int)json.get<jsonxx::Number>("ignoreLayout") == 1) {
                        opt->SetValue("auto");
                    }
                    else {
                        opt->SetValue("encoded");
                    }
                }
            }
            else if (iter->first == "noLayout") {
                LogWarning("Option noLayout is deprecated; use breaks: \"auto\"|\"none\" instead");
                Option *opt = NULL;
                opt = m_options->GetItems()->at("breaks");
                assert(opt);
                if (json.has<jsonxx::Number>("noLayout")) {
                    if ((int)json.get<jsonxx::Number>("noLayout") == 1) {
                        opt->SetValue("none");
                    }
                    else {
                        opt->SetValue("auto");
                    }
                }
            }
            else {
                LogError("Unsupported option '%s'", iter->first.c_str());
            }
            continue;
        }

        // Mapped options

        Option *opt = m_options->GetItems()->at(iter->first);
        assert(opt);

        if (json.has<jsonxx::Number>(iter->first)) {
            opt->SetValueDbl(json.get<jsonxx::Number>(iter->first));
            // LogMessage("Double: %f", json.get<jsonxx::Number>(iter->first));
        }
        else if (json.has<jsonxx::Boolean>(iter->first)) {
            opt->SetValueBool(json.get<jsonxx::Boolean>(iter->first));
            // LogMessage("Bool: %d", json.get<jsonxx::Boolean>(iter->first));
        }
        else if (json.has<jsonxx::String>(iter->first)) {
            opt->SetValue(json.get<jsonxx::String>(iter->first));
            // LogMessage("String: %s", json.get<jsonxx::String>(iter->first).c_str());
        }
        else if (json.has<jsonxx::Array>(iter->first)) {
            jsonxx::Array values = json.get<jsonxx::Array>(iter->first);
            std::vector<std::string> strValues;
            int i;
            for (i = 0; i < (int)values.size(); ++i) {
                if (values.has<jsonxx::String>(i)) strValues.push_back(values.get<jsonxx::String>(i));
                // LogDebug("String: %s", values.get<jsonxx::String>(i).c_str());
            }
            opt->SetValueArray(strValues);
        }
        else {
            LogError("Unsupported type for option '%s'", iter->first.c_str());
        }
    }

    return true;
}

std::string Toolkit::GetOption(const std::string &option, bool defaultValue) const
{
    if (m_options->GetItems()->count(option) == 0) {
        LogError("Unsupported option '%s'", option.c_str());
        return "[unspecified]";
    }
    Option *opt = m_options->GetItems()->at(option);
    assert(opt);
    return (defaultValue) ? opt->GetDefaultStrValue() : opt->GetStrValue();
}

bool Toolkit::SetOption(const std::string &option, const std::string &value)
{
    if (m_options->GetItems()->count(option) == 0) {
        LogError("Unsupported option '%s'", option.c_str());
        return false;
    }
    Option *opt = m_options->GetItems()->at(option);
    assert(opt);
    return opt->SetValue(value);
}

std::string Toolkit::GetElementAttr(const std::string &xmlId)
{
    jsonxx::Object o;

    if (!m_doc.GetDrawingPage()) return o.json();
    Object *element = m_doc.GetDrawingPage()->FindChildByUuid(xmlId);
    if (!element) {
        LogMessage("Element with id '%s' could not be found", xmlId.c_str());
        return o.json();
    }

    // Fill the attribute array (pair of std::string) by looking at attributes for all available MEI modules
    ArrayOfStrAttr attributes;
    element->GetAttributes(&attributes);

    // Fill the JSON object
    ArrayOfStrAttr::iterator iter;
    for (iter = attributes.begin(); iter != attributes.end(); ++iter) {
        o << (*iter).first << (*iter).second;
        // LogMessage("Element %s - %s", (*iter).first.c_str(), (*iter).second.c_str());
    }
    return o.json();
}

bool Toolkit::Edit(const std::string &json_editorAction)
{
#ifdef USE_EMSCRIPTEN

    jsonxx::Object json;

    // Read JSON actions
    if (!json.parse(json_editorAction)) {
        LogError("Can not parse JSON std::string.");
        return false;
    }

    if (json.has<jsonxx::String>("action") && json.has<jsonxx::Object>("param")) {
        if (json.get<jsonxx::String>("action") == "drag") {
            std::string elementId;
            int x, y;
            if (this->ParseDragAction(json.get<jsonxx::Object>("param"), &elementId, &x, &y)) {
                return this->Drag(elementId, x, y);
            }
        }
        else if (json.get<jsonxx::String>("action") == "insert") {
            LogMessage("insert...");
            std::string elementType, startid, endid;
            if (this->ParseInsertAction(json.get<jsonxx::Object>("param"), &elementType, &startid, &endid)) {
                return this->Insert(elementType, startid, endid);
            }
            else {
                LogMessage("Insert!!!! %s %s %s", elementType.c_str(), startid.c_str(), endid.c_str());
            }
        }
        else if (json.get<jsonxx::String>("action") == "set") {
            std::string elementId, attrType, attrValue;
            if (this->ParseSetAction(json.get<jsonxx::Object>("param"), &elementId, &attrType, &attrValue)) {
                return this->Set(elementId, attrType, attrValue);
            }
        }
    }
    LogError("Does not understand action.");
    return false;

#else
    // The non-js version of the app should not use this function.
    return false;
#endif
}

std::string Toolkit::GetLog()
{
#ifdef USE_EMSCRIPTEN
    std::string str;
    std::vector<std::string>::iterator iter;
    for (iter = logBuffer.begin(); iter != logBuffer.end(); ++iter) {
        str += (*iter);
    }
    return str;
#else
    // The non-js version of the app should not use this function.
    return "";
#endif
}

std::string Toolkit::GetVersion()
{
    return vrv::GetVersion();
}

void Toolkit::ResetLogBuffer()
{
#ifdef USE_EMSCRIPTEN
    vrv::logBuffer.clear();
#endif
}

void Toolkit::RedoLayout()
{
    if (m_doc.GetType() == Transcription) {
        return;
    }

    m_doc.UnCastOffDoc();
    m_doc.CastOffDoc();
}

void Toolkit::RedoPagePitchPosLayout()
{
    Page *page = m_doc.GetDrawingPage();

    if (!page) {
        LogError("No page to re-layout");
        return;
    }

    page->LayOutPitchPos();
}

bool Toolkit::RenderToDeviceContext(int pageNo, DeviceContext *deviceContext)
{
    // Page number is one-based - correct it to 0-based first
    pageNo--;

    // Get the current system for the SVG clipping size
    m_view.SetPage(pageNo);

    // Adjusting page width and height according to the options
    int width = m_options->m_pageWidth.GetUnfactoredValue();
    int height = m_options->m_pageHeight.GetUnfactoredValue();

    if (m_options->m_breaks.GetValue() == BREAKS_none) width = m_doc.GetAdjustedDrawingPageWidth();
    if (m_options->m_adjustPageHeight.GetValue() || (m_options->m_breaks.GetValue() == BREAKS_none))
        height = m_doc.GetAdjustedDrawingPageHeight();

    // set dimensions
    deviceContext->SetWidth(width);
    deviceContext->SetHeight(height);
    double userScale = m_view.GetPPUFactor() * m_scale / 100;
    deviceContext->SetUserScale(userScale, userScale);

    // render the page
    m_view.DrawCurrentPage(deviceContext, false);

    return true;
}

std::string Toolkit::RenderToSVG(int pageNo, bool xml_declaration)
{
    // Create the SVG object, h & w come from the system
    // We will need to set the size of the page after having drawn it depending on the options
    SvgDeviceContext svg;

    if (m_options->m_mmOutput.GetValue()) {
        svg.SetMMOutput(true);
    }

    // render the page
    RenderToDeviceContext(pageNo, &svg);

    std::string out_str = svg.GetStringSVG(xml_declaration);
    return out_str;
}

bool Toolkit::RenderToSVGFile(const std::string &filename, int pageNo)
{
    std::string output = RenderToSVG(pageNo, true);

    std::ofstream outfile;
    outfile.open(filename.c_str());

    if (!outfile.is_open()) {
        // add message?
        return false;
    }

    outfile << output;
    outfile.close();
    return true;
}

std::string Toolkit::GetHumdrum()
{
    return GetHumdrumBuffer();
}

bool Toolkit::GetHumdrumFile(const std::string &filename)
{
    std::ofstream output;
    output.open(filename.c_str());

    if (!output.is_open()) {
        // add message?
        return false;
    }

    GetHumdrum(output);
    output.close();
    return true;
}

void Toolkit::GetHumdrum(std::ostream &output)
{
    output << GetHumdrumBuffer();
}

std::string Toolkit::RenderToMIDI()
{
    smf::MidiFile outputfile;
    outputfile.absoluteTicks();
    m_doc.ExportMIDI(&outputfile);
    outputfile.sortTracks();

    std::stringstream strstrem;
    outputfile.write(strstrem);
    std::string outputstr = Base64Encode(
        reinterpret_cast<const unsigned char *>(strstrem.str().c_str()), (unsigned int)strstrem.str().length());

    return outputstr;
}

std::string Toolkit::RenderToTimemap()
{
    std::string output;
    m_doc.ExportTimemap(output);
    return output;
}

std::string Toolkit::GetElementsAtTime(int millisec)
{
    jsonxx::Object o;
    jsonxx::Array a;

    // Here we need to check that the midi timemap is done
    if (!m_doc.HasMidiTimemap()) {
        return o.json();
    }

    MeasureOnsetOffsetComparison matchMeasureTime(millisec);
    Measure *measure = dynamic_cast<Measure *>(m_doc.FindChildByComparison(&matchMeasureTime));

    if (!measure) {
        return o.json();
    }

    int repeat = measure->EnclosesTime(millisec);
    int measureTimeOffset = measure->GetRealTimeOffsetMilliseconds(repeat);

    // Get the pageNo from the first note (if any)
    int pageNo = -1;
    Page *page = dynamic_cast<Page *>(measure->GetFirstParent(PAGE));
    if (page) pageNo = page->GetIdx() + 1;

    NoteOnsetOffsetComparison matchNoteTime(millisec - measureTimeOffset);
    ArrayOfObjects notes;

    measure->FindAllChildByComparison(&notes, &matchNoteTime);

    // Fill the JSON object
    ArrayOfObjects::iterator iter;
    for (iter = notes.begin(); iter != notes.end(); ++iter) {
        a << (*iter)->GetUuid();
    }
    o << "notes" << a;
    o << "page" << pageNo;

    return o.json();
}

bool Toolkit::RenderToMIDIFile(const std::string &filename)
{
    smf::MidiFile outputfile;
    outputfile.absoluteTicks();
    m_doc.ExportMIDI(&outputfile);
    outputfile.sortTracks();
    outputfile.write(filename);

    return true;
}

bool Toolkit::RenderToTimemapFile(const std::string &filename)
{
    std::string outputString;
    m_doc.ExportTimemap(outputString);

    std::ofstream output(filename.c_str());
    if (!output.is_open()) {
        return false;
    }
    output << outputString;

    return true;
}

int Toolkit::GetPageCount()
{
    return m_doc.GetPageCount();
}

int Toolkit::GetPageWithElement(const std::string &xmlId)
{
    Object *element = m_doc.FindChildByUuid(xmlId);
    if (!element) {
        return 0;
    }
    Page *page = dynamic_cast<Page *>(element->GetFirstParent(PAGE));
    if (!page) {
        return 0;
    }
    return page->GetIdx() + 1;
}

int Toolkit::GetTimeForElement(const std::string &xmlId)
{
    Object *element = m_doc.FindChildByUuid(xmlId);
    int timeofElement = 0;
    if (element->Is(NOTE)) {
        if (!m_doc.HasMidiTimemap()) {
            // generate MIDI timemap before progressing
            m_doc.CalculateMidiTimemap();
        }
        if (!m_doc.HasMidiTimemap()) {
            LogWarning("Calculation of MIDI timemap failed, time value is invalid.");
        }
        Note *note = dynamic_cast<Note *>(element);
        assert(note);
        Measure *measure = dynamic_cast<Measure *>(note->GetFirstParent(MEASURE));
        assert(measure);
        // For now ignore repeats and access always the first
        timeofElement = measure->GetRealTimeOffsetMilliseconds(1);
        timeofElement += note->GetRealTimeOnsetMilliseconds();
    }
    return timeofElement;
}

void Toolkit::SetHumdrumBuffer(const char *data)
{
    if (m_humdrumBuffer) {
        free(m_humdrumBuffer);
        m_humdrumBuffer = NULL;
    }

#ifndef NO_HUMDRUM_SUPPORT
    hum::HumdrumFile file;
    file.readString(data);
    // apply Humdrum tools if there are any filters in the file.
    if (file.hasFilters()) {
        std::string output;
        hum::Tool_filter filter;
        filter.run(file);
        if (filter.hasHumdrumText()) {
            output = filter.getHumdrumText();
        }
        else {
            // humdrum structure not always correct in output from tools
            // yet, so reload.
            stringstream tempdata;
            tempdata << file;
            output = tempdata.str();
        }
        m_humdrumBuffer = (char *)malloc(output.size() + 1);
        if (!m_humdrumBuffer) {
            // something went wrong
            return;
        }
        strcpy(m_humdrumBuffer, output.c_str());
    }
    else {
        int size = (int)strlen(data) + 1;
        m_humdrumBuffer = (char *)malloc(size);
        if (!m_humdrumBuffer) {
            // something went wrong
            return;
        }
        strcpy(m_humdrumBuffer, data);
    }

#else
    size_t size = (int)strlen(data) + 1;
    m_humdrumBuffer = (char *)malloc(size);
    if (!m_humdrumBuffer) {
        // something went wrong
        return;
    }
    strcpy(m_humdrumBuffer, data);
#endif
}
const char *Toolkit::GetHumdrumBuffer()
{
    if (m_humdrumBuffer) {
        return m_humdrumBuffer;
    }
    else {
        return "[empty]";
    }
}

bool Toolkit::Drag(std::string elementId, int x, int y)
{
    if (!m_doc.GetDrawingPage()) return false;

    // Try to get the element on the current drawing page
    Object *element = m_doc.GetDrawingPage()->FindChildByUuid(elementId);

    // If it wasn't there, go back up to the whole doc
    if (!element) {
        element = m_doc.FindChildByUuid(elementId);
    }
    // For elements whose y-position corresponds to a certain pitch
    if (element->HasInterface(INTERFACE_PITCH)) {
        Layer *layer = dynamic_cast<Layer *>(element->GetFirstParent(LAYER));
        if (!layer) return false;
        int oct;
        data_PITCHNAME pname
            = (data_PITCHNAME)m_view.CalculatePitchCode(layer, m_view.ToLogicalY(y), element->GetDrawingX(), &oct);
        element->GetPitchInterface()->SetPname(pname);
        element->GetPitchInterface()->SetOct(oct);
        if (element->HasAttClass(ATT_COORDINATED)) {
            AttCoordinated *att = dynamic_cast<AttCoordinated *>(element);
            att->SetUlx(x);
        }
        return true;
    }
    if (element->Is(NEUME)) {
        // Requires a relative x and y
        Neume *neume = dynamic_cast<Neume *>(element);
        assert(neume);
        Layer *layer = dynamic_cast<Layer *>(neume->GetFirstParent(LAYER));
        if (!layer) return false;
        Staff *staff = dynamic_cast<Staff *>(layer->GetFirstParent(STAFF));
        assert(staff);
        // Calculate difference in pitch based on y difference
        int pitchDifference = round((double)y / (double)staff->m_drawingStaffSize);

        // Get components of neume
        AttComparison ac(NC);
        ArrayOfObjects objects;
        neume->FindAllChildByComparison(&objects, &ac);

        for (auto it = objects.begin(); it != objects.end(); ++it) {
            Nc *nc = dynamic_cast<Nc *>(*it);
            // Update the neume component
            nc->AdjustPitchByOffset(pitchDifference);
            //// Temporarily removing ULX attributes for coordinate refactor
            // nc->SetUlx(nc->GetUlx() - x);
        }
        return true;
    }
    if (element->Is(CLEF)) {
        Clef *clef = dynamic_cast<Clef *>(element);
        assert(clef);
        Layer *layer = dynamic_cast<Layer *>(clef->GetFirstParent(LAYER));
        if (!layer) return false;

        Staff *staff = dynamic_cast<Staff *>(layer->GetFirstParent(STAFF));
        assert(staff);
        // Note that y param is relative to initial position for clefs
        int initialClefLine = clef->GetLine();
        int clefLine
            = round((double)y / (double)m_doc.GetDrawingDoubleUnit(staff->m_drawingStaffSize) + initialClefLine);
        clef->SetLine(clefLine);
        //// Temporarily removing ULX attributes for coordinate refactor
        // clef->SetUlx(x);

        if (initialClefLine != clefLine) { // adjust notes so they stay in the same position
            int lineDiff = clefLine - initialClefLine;
            ArrayOfObjects objects;
            InterfaceComparison ic(INTERFACE_PITCH);

            layer->FindAllChildByComparison(&objects, &ic);

            // Adjust all elements who are positioned relative to clef by pitch
            for (auto it = objects.begin(); it != objects.end(); ++it) {
                Object *child = dynamic_cast<Object *>(*it);
                if (child == nullptr) continue;
                PitchInterface *pi = child->GetPitchInterface();
                assert(pi);
                pi->AdjustPitchByOffset(-2 * lineDiff); // One line -> 2 pitches
            }
        }

        return true;
    }
    return false;
}

bool Toolkit::Insert(std::string elementType, std::string startid, std::string endid)
{
    LogMessage("Insert!");
    if (!m_doc.GetDrawingPage()) return false;
    Object *start = m_doc.GetDrawingPage()->FindChildByUuid(startid);
    Object *end = m_doc.GetDrawingPage()->FindChildByUuid(endid);
    // Check if both start and end elements exist
    if (!start || !end) {
        LogMessage("Elements start and end ids '%s' and '%s' could not be found", startid.c_str(), endid.c_str());
        return false;
    }
    // Check if it is a LayerElement
    if (!dynamic_cast<LayerElement *>(start)) {
        LogMessage("Element '%s' is not supported as start element", start->GetClassName().c_str());
        return false;
    }
    if (!dynamic_cast<LayerElement *>(end)) {
        LogMessage("Element '%s' is not supported as end element", start->GetClassName().c_str());
        return false;
    }

    Measure *measure = dynamic_cast<Measure *>(start->GetFirstParent(MEASURE));
    assert(measure);
    if (elementType == "slur") {
        Slur *slur = new Slur();
        slur->SetStartid(startid);
        slur->SetEndid(endid);
        measure->AddChild(slur);
        m_doc.PrepareDrawing();
        return true;
    }
    return false;
}

bool Toolkit::Set(std::string elementId, std::string attrType, std::string attrValue)
{
    if (!m_doc.GetDrawingPage()) return false;
    Object *element = m_doc.GetDrawingPage()->FindChildByUuid(elementId);
    bool success = false;
    if (Att::SetAnalytical(element, attrType, attrValue))
        success = true;
    else if (Att::SetCmn(element, attrType, attrValue))
        success = true;
    else if (Att::SetCmnornaments(element, attrType, attrValue))
        success = true;
    else if (Att::SetCritapp(element, attrType, attrValue))
        success = true;
    else if (Att::SetExternalsymbols(element, attrType, attrValue))
        success = true;
    else if (Att::SetGestural(element, attrType, attrValue))
        success = true;
    else if (Att::SetMei(element, attrType, attrValue))
        success = true;
    else if (Att::SetMensural(element, attrType, attrValue))
        success = true;
    else if (Att::SetMidi(element, attrType, attrValue))
        success = true;
    else if (Att::SetPagebased(element, attrType, attrValue))
        success = true;
    else if (Att::SetShared(element, attrType, attrValue))
        success = true;
    else if (Att::SetVisual(element, attrType, attrValue))
        success = true;
    if (success) {
        m_doc.PrepareDrawing();
        m_doc.GetDrawingPage()->LayOut(true);
        return true;
    }
    return false;
}

#ifdef USE_EMSCRIPTEN
bool Toolkit::ParseDragAction(jsonxx::Object param, std::string *elementId, int *x, int *y)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::Number>("x")) return false;
    (*x) = param.get<jsonxx::Number>("x");
    if (!param.has<jsonxx::Number>("y")) return false;
    (*y) = param.get<jsonxx::Number>("y");
    return true;
}

bool Toolkit::ParseInsertAction(
    jsonxx::Object param, std::string *elementType, std::string *startid, std::string *endid)
{
    if (!param.has<jsonxx::String>("elementType")) return false;
    (*elementType) = param.get<jsonxx::String>("elementType");
    if (!param.has<jsonxx::String>("startid")) return false;
    (*startid) = param.get<jsonxx::String>("startid");
    if (!param.has<jsonxx::String>("endid")) return false;
    (*endid) = param.get<jsonxx::String>("endid");
    return true;
}

bool Toolkit::ParseSetAction(
    jsonxx::Object param, std::string *elementId, std::string *attrType, std::string *attrValue)
{
    if (!param.has<jsonxx::String>("elementId")) return false;
    (*elementId) = param.get<jsonxx::String>("elementId");
    if (!param.has<jsonxx::String>("attrType")) return false;
    (*attrType) = param.get<jsonxx::String>("attrType");
    if (!param.has<jsonxx::String>("attrValue")) return false;
    (*attrValue) = param.get<jsonxx::String>("attrValue");
    return true;
}
#endif

void Toolkit::SetCString(const std::string &data)
{
    if (m_cString) {
        free(m_cString);
        m_cString = NULL;
    }

    m_cString = (char *)malloc(strlen(data.c_str()) + 1);

    // something went wrong
    if (!m_cString) {
        return;
    }
    strcpy(m_cString, data.c_str());
}

const char *Toolkit::GetCString()
{
    if (m_cString) {
        return m_cString;
    }
    else {
        return "[unspecified]";
    }
}

} // namespace vrv
