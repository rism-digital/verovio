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

#include "comparison.h"
#include "custos.h"
#include "editortoolkit_cmn.h"
#include "editortoolkit_mensural.h"
#include "editortoolkit_neume.h"
#include "functorparams.h"
#include "ioabc.h"
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

std::map<std::string, ClassId> Toolkit::s_MEItoClassIdMap
    = { { "chord", CHORD }, { "rest", REST }, { "mRest", MREST }, { "mRpt", MRPT }, { "mRpt2", MRPT2 },
          { "multiRest", MULTIREST }, { "mulitRpt", MULTIRPT }, { "note", NOTE }, { "space", SPACE } };

void SetDefaultResourcePath(const std::string &path)
{
    Resources::SetPath(path);
}

//----------------------------------------------------------------------------
// Toolkit
//----------------------------------------------------------------------------

char *Toolkit::m_humdrumBuffer = NULL;

Toolkit::Toolkit(bool initFont)
{
    m_scale = DEFAULT_SCALE;
    m_inputFrom = AUTO;

    m_humdrumBuffer = NULL;
    m_cString = NULL;

    if (initFont) {
        Resources::InitFonts();
    }

    m_options = m_doc.GetOptions();

    m_editorToolkit = NULL;
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
    if (m_editorToolkit) {
        delete m_editorToolkit;
        m_editorToolkit = NULL;
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

bool Toolkit::SetOutputTo(std::string const &outputTo)
{
    if ((outputTo == "humdrum") || (outputTo == "hum")) {
        m_outputTo = HUMDRUM;
    }
    else if (outputTo == "mei") {
        m_outputTo = MEI;
    }
    else if (outputTo == "pb-mei") {
        m_outputTo = MEI;
    }
    else if (outputTo == "midi") {
        m_outputTo = MIDI;
    }
    else if (outputTo == "timemap") {
        m_outputTo = TIMEMAP;
    }
    else if (outputTo == "pae") {
        m_outputTo = PAE;
    }
    else if (outputTo != "svg") {
        LogError("Output format can only be: mei, pb-mei, humdrum, midi, timemap or svg");
        return false;
    }
    return true;
}

bool Toolkit::SetInputFrom(std::string const &inputFrom)
{
    if (inputFrom == "abc") {
        m_inputFrom = ABC;
    }
    else if (inputFrom == "pae") {
        m_inputFrom = PAE;
    }
    else if (inputFrom == "darms") {
        m_inputFrom = DARMS;
    }
    else if ((inputFrom == "humdrum") || (inputFrom == "hum")) {
        m_inputFrom = HUMDRUM;
    }
    else if (inputFrom == "mei") {
        m_inputFrom = MEI;
    }
    else if ((inputFrom == "musicxml") || (inputFrom == "xml")) {
        m_inputFrom = MUSICXML;
    }
    else if (inputFrom == "md") {
        m_inputFrom = MUSEDATAHUM;
    }
    else if (inputFrom == "musedata") {
        m_inputFrom = MUSEDATAHUM;
    }
    else if (inputFrom == "musedata-hum") {
        m_inputFrom = MUSEDATAHUM;
    }
    else if (inputFrom == "musicxml-hum") {
        m_inputFrom = MUSICXMLHUM;
    }
    else if (inputFrom == "mei-hum") {
        m_inputFrom = MEIHUM;
    }
    else if (inputFrom == "esac") {
        m_inputFrom = ESAC;
    }
    else if (inputFrom == "auto") {
        m_inputFrom = AUTO;
    }
    else {
        LogError("Input format can only be: mei, humdrum, pae, abc, musicxml or darms");
        return false;
    }
    return true;
}

FileFormat Toolkit::IdentifyInputFrom(const std::string &data)
{
#ifdef MUSICXML_DEFAULT_HUMDRUM
    FileFormat musicxmlDefault = MUSICXMLHUM;
#else
    FileFormat musicxmlDefault = MUSICXML;
#endif

    if (data.empty()) {
        return UNKNOWN;
    }
    if (data[0] == 0) {
        return UNKNOWN;
    }
    std::string excerpt = data.substr(0, 2000);
    std::string::size_type found = excerpt.find("Group memberships:");
    if (found != std::string::npos) {
        // MuseData may contain '@' as first character, so needs
        // to be checked before PAE identification.
        return MUSEDATAHUM;
    }
    if (data[0] == '@') {
        return PAE;
    }
    if (data[0] == '*' || data[0] == '!') {
        return HUMDRUM;
    }
    if (data[0] == 'X' || data[0] == '%') {
        return ABC;
    }
    if ((unsigned char)data[0] == 0xff || (unsigned char)data[0] == 0xfe) {
        // Handle UTF-16 content here later.
        std::cerr << "Warning: Cannot yet auto-detect format of UTF-16 data files." << std::endl;
        return UNKNOWN;
    }
    if (data[0] == '<') {
        size_t searchLimit = 600;
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
        if (initial.find("<!DOCTYPE score-partwise ") != std::string::npos) {
            return musicxmlDefault;
        }
        if (initial.find("<!DOCTYPE score-timewise ") != std::string::npos) {
            return musicxmlDefault;
        }
        if (initial.find("<!DOCTYPE opus ") != std::string::npos) {
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

    m_doc.m_expansionMap.Reset();

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

void Toolkit::GetClassIds(const std::vector<std::string> &classStrings, std::vector<ClassId> &classIds)
{
    // one we use magic_enum.hpp we can do :
    /*
    for (auto str : classStrings) {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        auto classId = magic_enum::enum_cast<ClassId>(str);
        if (classId.has_value()) {
            classIds.push_back(classId.value());
          // color.value() -> Color::GREEN
        }
        else {
            LogError("Class name '%s' could not be matched", str.c_str());
        }
    }
    */
    // For now, map a few by hand... - there must be a better way to do this
    for (auto str : classStrings) {
        if (Toolkit::s_MEItoClassIdMap.count(str) > 0) {
            classIds.push_back(Toolkit::s_MEItoClassIdMap.at(str));
        }
        else {
            LogDebug("Class name '%s' could not be matched", str.c_str());
        }
    }
}

bool Toolkit::LoadData(const std::string &data)
{
    std::string newData;
    Input *input = NULL;

    auto inputFormat = m_inputFrom;
    if (inputFormat == AUTO) {
        inputFormat = IdentifyInputFrom(data);
    }
    if (inputFormat == ABC) {
#ifndef NO_ABC_SUPPORT
        input = new ABCInput(&m_doc);
#else
        LogError("ABC import is not supported in this build.");
        return false;
#endif
    }
    else if (inputFormat == PAE) {
#ifndef NO_PAE_SUPPORT
        input = new PAEInput(&m_doc);
#else
        LogError("Plaine & Easie import is not supported in this build.");
        return false;
#endif
    }
    else if (inputFormat == DARMS) {
#ifndef NO_DARMS_SUPPORT
        input = new DarmsInput(&m_doc);
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
        HumdrumInput *tempinput = new HumdrumInput(&tempdoc);
        if (GetOutputTo() == HUMDRUM) {
            tempinput->SetOutputFormat("humdrum");
        }

        if (!tempinput->Import(data)) {
            LogError("Error importing Humdrum data (1)");
            delete tempinput;
            return false;
        }

        SetHumdrumBuffer(tempinput->GetHumdrumString().c_str());

        if (GetOutputTo() == HUMDRUM) {
            return true;
        }

        MEIOutput meioutput(&tempdoc);
        meioutput.SetScoreBasedMEI(true);
        newData = meioutput.GetOutput();

        // Read embedded options from input Humdrum file:
        tempinput->parseEmbeddedOptions(m_doc);
        delete tempinput;

        input = new MEIInput(&m_doc);
    }
#endif
    else if (inputFormat == MEI) {
        input = new MEIInput(&m_doc);
    }
    else if (inputFormat == MUSICXML) {
        // This is the direct converter from MusicXML to MEI using iomusicxml:
        input = new MusicXmlInput(&m_doc);
    }
#ifndef NO_HUMDRUM_SUPPORT
    else if (inputFormat == MUSICXMLHUM) {
        // This is the indirect converter from MusicXML to MEI using iohumdrum:
        hum::Tool_musicxml2hum converter;
        pugi::xml_document xmlfile;
        xmlfile.load_string(data.c_str());
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
        Input *tempinput = new HumdrumInput(&tempdoc);
        if (!tempinput->Import(conversion.str())) {
            LogError("Error importing Humdrum data (2)");
            delete tempinput;
            return false;
        }
        MEIOutput meioutput(&tempdoc);
        meioutput.SetScoreBasedMEI(true);
        newData = meioutput.GetOutput();
        delete tempinput;
        input = new MEIInput(&m_doc);
    }

    else if (inputFormat == MEIHUM) {
        // This is the indirect converter from MusicXML to MEI using iohumdrum:
        hum::Tool_mei2hum converter;
        pugi::xml_document xmlfile;
        xmlfile.load_string(data.c_str());
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
        Input *tempinput = new HumdrumInput(&tempdoc);
        if (!tempinput->Import(conversion.str())) {
            LogError("Error importing Humdrum data (3)");
            delete tempinput;
            return false;
        }
        MEIOutput meioutput(&tempdoc);
        meioutput.SetScoreBasedMEI(true);
        newData = meioutput.GetOutput();
        delete tempinput;
        input = new MEIInput(&m_doc);
    }

    else if (inputFormat == MUSEDATAHUM) {
        // This is the indirect converter from MuseData to MEI using iohumdrum:
        hum::Tool_musedata2hum converter;
        stringstream conversion;
        bool status = converter.convertString(conversion, data);
        if (!status) {
            LogError("Error converting MuseData data");
            return false;
        }
        std::string buffer = conversion.str();
        SetHumdrumBuffer(buffer.c_str());

        // Now convert Humdrum into MEI:
        Doc tempdoc;
        tempdoc.SetOptions(m_doc.GetOptions());
        Input *tempinput = new HumdrumInput(&tempdoc);
        if (!tempinput->Import(conversion.str())) {
            LogError("Error importing Humdrum data (4)");
            delete tempinput;
            return false;
        }
        MEIOutput meioutput(&tempdoc);
        meioutput.SetScoreBasedMEI(true);
        newData = meioutput.GetOutput();
        delete tempinput;
        input = new MEIInput(&m_doc);
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
        Input *tempinput = new HumdrumInput(&tempdoc);
        if (!tempinput->Import(conversion.str())) {
            LogError("Error importing Humdrum data (5)");
            delete tempinput;
            return false;
        }
        MEIOutput meioutput(&tempdoc);
        meioutput.SetScoreBasedMEI(true);
        newData = meioutput.GetOutput();
        delete tempinput;
        input = new MEIInput(&m_doc);
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
    if (!input->Import(newData.size() ? newData : data)) {
        LogError("Error importing data");
        delete input;
        return false;
    }

    bool adjustPageHeight = m_options->m_adjustPageHeight.GetValue();
    int footerOption = m_options->m_footer.GetValue();
    // With adjusted page height, show the footer if explicitly set (i.e., not with "auto")
    // generate the page header and footer if necessary
    if ((!adjustPageHeight && (footerOption == FOOTER_auto)) || (footerOption == FOOTER_always)) {
        m_doc.GenerateFooter();
    }
    if (m_options->m_header.GetValue() == HEADER_auto) {
        m_doc.GenerateHeader();
    }

    // generate missing measure numbers
    m_doc.GenerateMeasureNumbers();

    // transpose the content if necessary
    if (m_options->m_transpose.GetValue() != "") {
        m_doc.PrepareDrawing();
        m_doc.TransposeDoc();
    }

    m_doc.PrepareDrawing();

    // Convert pseudo-measures into distinct segments based on barLine elements
    if (m_doc.IsMensuralMusicOnly()) {
        m_doc.ConvertToCastOffMensuralDoc();
    }

    // Do the layout? this depends on the options and the file. PAE and
    // DARMS have no layout information. MEI files _can_ have it, but it
    // might have been ignored because of the --breaks auto option.
    // Regardless, we won't do layout if the --breaks none option was set.
    int breaks = m_options->m_breaks.GetValue();
    // Always set breaks to 'none' with Transcription or Facs rendering - rendering them differenty requires the MEI to
    // be converted
    if (m_doc.GetType() == Transcription || m_doc.GetType() == Facs) breaks = BREAKS_none;
    if (breaks != BREAKS_none) {
        if (input->HasLayoutInformation() && (breaks == BREAKS_encoded || breaks == BREAKS_line)) {
            if (breaks == BREAKS_encoded) {
                // LogElapsedTimeStart();
                m_doc.CastOffEncodingDoc();
                // LogElapsedTimeEnd("layout");
            }
            else if (breaks == BREAKS_line) {
                m_doc.CastOffLineDoc();
            }
        }
        else {
            if (breaks == BREAKS_encoded) {
                LogWarning("Requesting layout with encoded breaks but nothing provided in the data");
            }
            else if (breaks == BREAKS_line) {
                LogWarning("Requesting layout with line breaks but nothing provided in the data");
            }
            // LogElapsedTimeStart();
            m_doc.CastOffDoc();
            // LogElapsedTimeEnd("layout");
        }
    }

    delete input;
    m_view.SetDoc(&m_doc);

#if defined NO_HUMDRUM_SUPPORT
    // Create editor toolkit based on notation type.
    if (m_editorToolkit != NULL) {
        delete m_editorToolkit;
    }
    switch (m_doc.m_notationType) {
        case NOTATIONTYPE_neume: m_editorToolkit = new EditorToolkitNeume(&m_doc, &m_view); break;
        case NOTATIONTYPE_mensural:
        case NOTATIONTYPE_mensural_black:
        case NOTATIONTYPE_mensural_white: m_editorToolkit = new EditorToolkitMensural(&m_doc, &m_view); break;
        case NOTATIONTYPE_cmn: m_editorToolkit = new EditorToolkitCMN(&m_doc, &m_view); break;
        default: m_editorToolkit = new EditorToolkitCMN(&m_doc, &m_view);
    }
#endif

    return true;
}

std::string Toolkit::GetMEI(const std::string &jsonOptions)
{
    bool scoreBased = true;
    int pageNo = 0;
    bool removeIds = false;

    jsonxx::Object json;

    // Read JSON options
    if (!json.parse(jsonOptions)) {
        LogWarning("Cannot parse JSON std::string. Using default options.");
    }
    else {
        if (json.has<jsonxx::Boolean>("scoreBased")) scoreBased = json.get<jsonxx::Boolean>("scoreBased");
        if (json.has<jsonxx::Number>("pageNo")) pageNo = json.get<jsonxx::Number>("pageNo");
        if (json.has<jsonxx::Boolean>("removeIds")) removeIds = json.get<jsonxx::Boolean>("removeIds");
    }

    if (GetPageCount() == 0) {
        LogWarning("No data loaded");
        return "";
    }

    int initialPageNo = (m_doc.GetDrawingPage() == NULL) ? -1 : m_doc.GetDrawingPage()->GetIdx();
    // Page number is one-based - correct it to 0-based first
    pageNo--;

    MEIOutput meioutput(&m_doc);
    meioutput.SetScoreBasedMEI(scoreBased);

    int indent = (m_options->m_outputIndentTab.GetValue()) ? -1 : m_options->m_outputIndent.GetValue();
    meioutput.SetIndent(indent);
    meioutput.SetRemoveIds(removeIds);

    std::string output = meioutput.GetOutput(pageNo);
    if (initialPageNo >= 0) m_doc.SetDrawingPage(initialPageNo);
    return output;
}

bool Toolkit::SaveFile(const std::string &filename, const std::string &jsonOptions)
{
    std::string output = GetMEI(jsonOptions);

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

bool Toolkit::SetOptions(const std::string &jsonOptions)
{
    jsonxx::Object json;

    // Read JSON options
    if (!json.parse(jsonOptions)) {
        LogError("Cannot parse JSON std::string.");
        return false;
    }

    std::map<std::string, jsonxx::Value *> jsonMap = json.kv_map();
    std::map<std::string, jsonxx::Value *>::const_iterator iter;
    for (iter = jsonMap.begin(); iter != jsonMap.end(); ++iter) {
        if (m_options->GetItems()->count(iter->first) == 0) {
            // Base options
            if (iter->first == "format") {
                LogWarning("Option format is deprecated; use from instead");
                if (json.has<jsonxx::String>("format")) {
                    SetInputFrom(json.get<jsonxx::String>("format"));
                }
            }
            if (iter->first == "from") {
                if (json.has<jsonxx::String>("from")) {
                    SetInputFrom(json.get<jsonxx::String>("from"));
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
            else if (iter->first == "condenseEncoded") {
                LogWarning("Option condenseEncoded is deprecated; use condense \"encoded\" instead");
                Option *opt = NULL;
                opt = m_options->GetItems()->at("condense");
                assert(opt);
                if (json.has<jsonxx::Number>("condenseEncoded")) {
                    if ((int)json.get<jsonxx::Number>("condenseEncoded") == 1) {
                        opt->SetValue("encoded");
                    }
                    else {
                        opt->SetValue("auto");
                    }
                }
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
            else if (iter->first == "inputFormat") {
                LogWarning("Option inputFormat is deprecated; use from instead");
                if (json.has<jsonxx::String>("inputFormat")) {
                    SetInputFrom(json.get<jsonxx::String>("inputFormat"));
                }
            }
            else if (iter->first == "noFooter") {
                LogWarning("Option noFooter is deprecated; use footer: \"auto\"|\"encoded\"|\"none\" instead");
                Option *opt = NULL;
                opt = m_options->GetItems()->at("footer");
                assert(opt);
                if (json.has<jsonxx::Number>("noFooter")) {
                    if ((int)json.get<jsonxx::Number>("noFooter") == 1) {
                        opt->SetValue("none");
                    }
                    else {
                        opt->SetValue("auto");
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
            else if (iter->first == "noHeader") {
                LogWarning("Option noHeader is deprecated; use header: \"auto\"|\"encoded\"|\"none\" instead");
                Option *opt = NULL;
                opt = m_options->GetItems()->at("header");
                assert(opt);
                if (json.has<jsonxx::Number>("noHeader")) {
                    if ((int)json.get<jsonxx::Number>("noHeader") == 1) {
                        opt->SetValue("none");
                    }
                    else {
                        opt->SetValue("auto");
                    }
                }
            }
            else if (iter->first == "slurThickness") {
                LogWarning("Option slurThickness is deprecated; use slurMidpointThickness instead");
                Option *opt = NULL;
                if (json.has<jsonxx::Number>("slurThickness")) {
                    double thickness = json.get<jsonxx::Number>("slurThickness");
                    opt = m_options->GetItems()->at("slurMidpointThickness");
                    assert(opt);
                    opt->SetValueDbl(thickness);
                }
            }
            else if (iter->first == "tieThickness") {
                vrv::LogWarning("Option tieThickness is deprecated; use tieMidpointThickness instead");
                Option *opt = NULL;
                if (json.has<jsonxx::Number>("tieThickness")) {
                    double thickness = json.get<jsonxx::Number>("tieThickness");
                    opt = m_options->GetItems()->at("tieMidpointThickness");
                    assert(opt);
                    opt->SetValueDbl(thickness);
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

    // Forcing font to be reset. Warning: SetOption("font") as a single option will not work.
    // This needs to be fixed
    if (!Resources::SetFont(m_options->m_font.GetValue())) {
        LogWarning("Font '%s' could not be loaded", m_options->m_font.GetValue().c_str());
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

    Object *element = NULL;

    // Try to get the element on the current drawing page - it is usually the case and fast
    if (m_doc.GetDrawingPage()) {
        element = m_doc.GetDrawingPage()->FindDescendantByUuid(xmlId);
    }
    // If it wasn't there, try on the whole doc
    if (!element) {
        element = m_doc.FindDescendantByUuid(xmlId);
    }
    // If not found at all
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

std::string Toolkit::GetNotatedIdForElement(const std::string &xmlId)
{
    if (m_doc.m_expansionMap.HasExpansionMap())
        return m_doc.m_expansionMap.GetExpansionIdsForElement(xmlId).front();
    else
        return xmlId;
}

std::string Toolkit::GetExpansionIdsForElement(const std::string &xmlId)
{
    jsonxx::Array a;
    if (m_doc.m_expansionMap.HasExpansionMap()) {
        for (std::string id : m_doc.m_expansionMap.GetExpansionIdsForElement(xmlId)) {
            a << id;
        }
    }
    else {
        a << "";
    }
    return a.json();
}

bool Toolkit::Edit(const std::string &json_editorAction)
{
    return m_editorToolkit->ParseEditorAction(json_editorAction);
}

std::string Toolkit::EditInfo()
{
    return m_editorToolkit->EditInfo();
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
    if ((GetPageCount() == 0) || (m_doc.GetType() == Transcription) || (m_doc.GetType() == Facs)) {
        LogWarning("No data to re-layout");
        return;
    }

    m_doc.UnCastOffDoc();
    if (m_options->m_breaks.GetValue() == BREAKS_line) {
        m_doc.CastOffLineDoc();
    }
    else {
        m_doc.CastOffDoc();
    }
}

void Toolkit::RedoPagePitchPosLayout()
{
    Page *page = m_doc.GetDrawingPage();

    if (!page) {
        LogWarning("No page to re-layout");
        return;
    }

    page->LayOutPitchPos();
}

bool Toolkit::RenderToDeviceContext(int pageNo, DeviceContext *deviceContext)
{
    if (pageNo > GetPageCount()) {
        LogWarning("Page %d does not exist", pageNo);
        return false;
    }

    // Page number is one-based - correct it to 0-based first
    pageNo--;

    // Get the current system for the SVG clipping size
    m_view.SetPage(pageNo);

    // Adjusting page width and height according to the options
    int width = m_options->m_pageWidth.GetUnfactoredValue();
    int height = m_options->m_pageHeight.GetUnfactoredValue();
    int breaks = m_options->m_breaks.GetValue();
    bool adjustHeight = m_options->m_adjustPageHeight.GetValue();
    bool adjustWidth = m_options->m_adjustPageWidth.GetValue();

    if (adjustWidth || (breaks == BREAKS_none)) width = m_doc.GetAdjustedDrawingPageWidth();
    if (adjustHeight || (breaks == BREAKS_none)) height = m_doc.GetAdjustedDrawingPageHeight();

    if (m_doc.GetType() == Transcription) {
        width = m_doc.GetAdjustedDrawingPageWidth();
        height = m_doc.GetAdjustedDrawingPageHeight();
    }

    // set dimensions
    deviceContext->SetWidth(width);
    deviceContext->SetHeight(height);
    double userScale = m_view.GetPPUFactor() * m_scale / 100;
    deviceContext->SetUserScale(userScale, userScale);

    if (m_doc.GetType() == Facs) {
        deviceContext->SetWidth(m_doc.GetFacsimile()->GetMaxX());
        deviceContext->SetHeight(m_doc.GetFacsimile()->GetMaxY());
    }

    // render the page
    m_view.DrawCurrentPage(deviceContext, false);

    return true;
}

std::string Toolkit::RenderToSVG(int pageNo, bool xml_declaration)
{
    int initialPageNo = (m_doc.GetDrawingPage() == NULL) ? -1 : m_doc.GetDrawingPage()->GetIdx();
    // Create the SVG object, h & w come from the system
    // We will need to set the size of the page after having drawn it depending on the options
    SvgDeviceContext svg;

    int indent = (m_options->m_outputIndentTab.GetValue()) ? -1 : m_options->m_outputIndent.GetValue();
    svg.SetIndent(indent);

    if (m_options->m_mmOutput.GetValue()) {
        svg.SetMMOutput(true);
    }

    if (m_doc.GetType() == Facs) {
        svg.SetFacsimile(true);
    }

    // set the option to use viewbox on svg root
    if (m_options->m_svgBoundingBoxes.GetValue()) {
        svg.SetSvgBoundingBoxes(true);
    }

    if (m_options->m_svgViewBox.GetValue()) {
        svg.SetSvgViewBox(true);
    }

    svg.SetHtml5(m_options->m_svgHtml5.GetValue());

    // render the page
    RenderToDeviceContext(pageNo, &svg);

    std::string out_str = svg.GetStringSVG(xml_declaration);
    if (initialPageNo >= 0) m_doc.SetDrawingPage(initialPageNo);
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

std::string Toolkit::RenderToPAE()
{
    if (GetPageCount() == 0) {
        LogWarning("No data loaded");
        return "";
    }

    PAEOutput paeOutput(&m_doc);
    std::string output;
    if (!paeOutput.Export(output)) {
        LogError("Export to PAE failed");
    }
    return output;
}

bool Toolkit::RenderToPAEFile(const std::string &filename)
{
    std::string outputString = this->RenderToPAE();

    std::ofstream output(filename.c_str());
    if (!output.is_open()) {
        return false;
    }
    output << outputString;

    return true;
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
        // generate MIDI timemap before progressing
        m_doc.CalculateMidiTimemap();
    }

    MeasureOnsetOffsetComparison matchMeasureTime(millisec);
    Measure *measure = dynamic_cast<Measure *>(m_doc.FindDescendantByComparison(&matchMeasureTime));

    if (!measure) {
        return o.json();
    }

    int repeat = measure->EnclosesTime(millisec);
    int measureTimeOffset = measure->GetRealTimeOffsetMilliseconds(repeat);

    // Get the pageNo from the first note (if any)
    int pageNo = -1;
    Page *page = dynamic_cast<Page *>(measure->GetFirstAncestor(PAGE));
    if (page) pageNo = page->GetIdx() + 1;

    NoteOnsetOffsetComparison matchNoteTime(millisec - measureTimeOffset);
    ListOfObjects notes;

    measure->FindAllDescendantByComparison(&notes, &matchNoteTime);

    // Fill the JSON object
    ListOfObjects::iterator iter;
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
    Object *element = m_doc.FindDescendantByUuid(xmlId);
    if (!element) {
        return 0;
    }
    Page *page = dynamic_cast<Page *>(element->GetFirstAncestor(PAGE));
    if (!page) {
        return 0;
    }
    return page->GetIdx() + 1;
}

int Toolkit::GetTimeForElement(const std::string &xmlId)
{
    Object *element = m_doc.FindDescendantByUuid(xmlId);

    if (!element) {
        LogWarning("Element '%s' not found", xmlId.c_str());
        return 0;
    }

    int timeofElement = 0;
    if (element->Is(NOTE)) {
        if (!m_doc.HasMidiTimemap()) {
            // generate MIDI timemap before progressing
            m_doc.CalculateMidiTimemap();
        }
        if (!m_doc.HasMidiTimemap()) {
            LogWarning("Calculation of MIDI timemap failed, time value is invalid.");
        }
        Note *note = vrv_cast<Note *>(element);
        assert(note);
        Measure *measure = vrv_cast<Measure *>(note->GetFirstAncestor(MEASURE));
        assert(measure);
        // For now ignore repeats and access always the first
        timeofElement = measure->GetRealTimeOffsetMilliseconds(1);
        timeofElement += note->GetRealTimeOnsetMilliseconds();
    }
    return timeofElement;
}

std::string Toolkit::GetMIDIValuesForElement(const std::string &xmlId)
{
    Object *element = m_doc.FindDescendantByUuid(xmlId);

    if (!element) {
        LogWarning("Element '%s' not found", xmlId.c_str());
        return 0;
    }

    jsonxx::Object o;
    if (element->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(element);
        assert(note);
        int timeOfElement = this->GetTimeForElement(xmlId);
        int pitchOfElement = note->GetMIDIPitch();
        int durationOfElement = note->GetRealTimeOffsetMilliseconds() - note->GetRealTimeOnsetMilliseconds();
        o << "time" << timeOfElement;
        o << "pitch" << pitchOfElement;
        o << "duration" << durationOfElement;
    }
    return o.json();
}

void Toolkit::SetHumdrumBuffer(const char *data)
{
    if (m_humdrumBuffer) {
        free(m_humdrumBuffer);
        m_humdrumBuffer = NULL;
    }
    size_t size = strlen(data) + 1;
    m_humdrumBuffer = (char *)malloc(size);
    if (!m_humdrumBuffer) {
        // something went wrong
        std::cerr << "m_humdrumBuffer is NULL (out of memory?)" << std::endl;
        return;
    }
    strcpy(m_humdrumBuffer, data);
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
