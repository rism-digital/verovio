/////////////////////////////////////////////////////////////////////////////
// Name:        toolkit.cpp
// Author:      Laurent Pugin
// Created:     17/10/2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "toolkit.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <locale>
#include <regex>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "custos.h"
#include "editortoolkit_cmn.h"
#include "editortoolkit_mensural.h"
#include "editortoolkit_neume.h"
#include "filereader.h"
#include "findfunctor.h"
#include "ioabc.h"
#include "iocmme.h"
#include "iodarms.h"
#include "iohumdrum.h"
#include "iomei.h"
#include "iomusxml.h"
#include "iopae.h"
#include "iovolpiano.h"
#include "layer.h"
#include "measure.h"
#include "nc.h"
#include "neume.h"
#include "note.h"
#include "options.h"
#include "page.h"
#include "runtimeclock.h"
#include "score.h"
#include "slur.h"
#include "staff.h"
#include "svgdevicecontext.h"
#include "timemap.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"
#include "crc.h"
#include "jsonxx.h"

namespace vrv {

const char *UTF_16_BE_BOM = "\xFE\xFF";
const char *UTF_16_LE_BOM = "\xFF\xFE";
const char *ZIP_SIGNATURE = "\x50\x4B\x03\x04";

//----------------------------------------------------------------------------
// Toolkit
//----------------------------------------------------------------------------

char *Toolkit::m_humdrumBuffer = NULL;

Toolkit::Toolkit(bool initFont)
{
    m_inputFrom = AUTO;
    m_outputTo = UNKNOWN;

    m_humdrumBuffer = NULL;
    m_cString = NULL;

    m_cerrOriginalBuf = NULL;

    if (initFont) {
        Resources &resources = m_doc.GetResourcesForModification();
        resources.InitFonts();
    }

    m_options = m_doc.GetOptions();

    m_editorToolkit = NULL;

#ifndef NO_RUNTIME
    m_runtimeClock = NULL;
#endif
}

Toolkit::~Toolkit()
{
    this->ResetLocale();

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
#ifndef NO_RUNTIME
    if (m_runtimeClock) {
        delete m_runtimeClock;
        m_runtimeClock = NULL;
    }
#endif
}

std::string Toolkit::GetResourcePath() const
{
    return m_doc.GetResources().GetPath();
}

bool Toolkit::SetResourcePath(const std::string &path)
{
    Resources &resources = m_doc.GetResourcesForModification();
    resources.SetPath(path);
    bool success = resources.InitFonts();
    if (m_options->m_fontAddCustom.IsSet()) {
        success = success && resources.AddCustom(m_options->m_fontAddCustom.GetValue());
    }
    if (m_options->m_font.IsSet()) {
        success = success && this->SetFont(m_options->m_font.GetValue());
    }
    if (m_options->m_fontFallback.IsSet()) {
        resources.SetFallbackFont(m_options->m_fontFallback.GetStrValue());
    }
    if (m_options->m_fontLoadAll.IsSet()) {
        success = success && resources.LoadAll();
    }
    if (m_options->m_fontTextLiberation.IsSet()) {
        resources.UseLiberationTextFont(m_options->m_fontTextLiberation.GetValue());
    }
    return success;
}

bool Toolkit::SetFont(const std::string &fontName)
{
    Resources &resources = m_doc.GetResourcesForModification();
    const bool ok = resources.SetCurrentFont(fontName, true);
    if (!ok) LogWarning("Font '%s' could not be loaded", fontName.c_str());
    return ok;
}

bool Toolkit::SetScale(int scale)
{
    return m_options->m_scale.SetValue(scale);
}

bool Toolkit::Select(const std::string &selection)
{
    return m_docSelection.Parse(selection);
}

bool Toolkit::SetOutputTo(std::string const &outputTo)
{
    if ((outputTo == "humdrum") || (outputTo == "hum")) {
        m_outputTo = HUMDRUM;
    }
    else if (outputTo == "mei") {
        m_outputTo = MEI;
    }
    else if (outputTo == "mei-basic") {
        m_outputTo = MEI;
    }
    else if (outputTo == "mei-pb") {
        m_outputTo = MEI;
    }
    else if (outputTo == "mei-facs") {
        m_outputTo = MEI;
    }
    else if (outputTo == "midi") {
        m_outputTo = MIDI;
    }
    else if (outputTo == "hummidi") {
        m_outputTo = HUMMIDI;
    }
    else if (outputTo == "timemap") {
        m_outputTo = TIMEMAP;
    }
    else if (outputTo == "expansionmap") {
        m_outputTo = EXPANSIONMAP;
    }
    else if (outputTo == "pae") {
        m_outputTo = PAE;
    }
    else if (outputTo == "mei-pb-serialized") {
        m_outputTo = SERIALIZATION;
    }
    else if (outputTo != "svg") {
        LogError("Output format '%s' is not supported", outputTo.c_str());
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
    else if (inputFrom == "volpiano") {
        m_inputFrom = VOLPIANO;
    }
    else if (inputFrom == "cmme.xml") {
        m_inputFrom = CMME;
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
    else if (inputFrom == "mei-pb-serialized") {
        m_inputFrom = SERIALIZATION;
    }
    else if (inputFrom == "auto") {
        m_inputFrom = AUTO;
    }
    else {
        LogError("Input format '%s' is not supported", inputFrom.c_str());
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
    if (data[0] == '@' || data[0] == '{') {
        return PAE;
    }
    if (data[0] == '*' || data[0] == '!') {
        return HUMDRUM;
    }
    if (data[0] == 'X') {
        return ABC;
    }
    if (data[0] == '%' && data.size() > 1) {
        return (data[1] == 'a') ? ABC : PAE;
    }
    if ((unsigned char)data[0] == 0xff || (unsigned char)data[0] == 0xfe) {
        // Handle UTF-16 content here later.
        std::cerr << "Warning: Cannot yet auto-detect format of UTF-16 data files." << std::endl;
        return UNKNOWN;
    }
    const int searchLimit = 600;
    std::string initial = data.substr(0, searchLimit);
    if (data[0] == '<') {
        // <mei> == root node for standard organization of MEI data
        // <pages> == root node for pages organization of MEI data
        // <score-partwise> == root node for part-wise organization of MusicXML data
        // <score-timewise> == root node for time-wise organization of MusicXML data
        // <opus> == root node for multi-movement/work organization of MusicXML data

        if (std::regex_search(initial, std::regex("<(verovio-serialization)[\\s>]"))) {
            return SERIALIZATION;
        }
        if (std::regex_search(initial, std::regex("<(mei|music|pages)[\\s>]"))) {
            return MEI;
        }
        if (std::regex_search(initial, std::regex("<(!DOCTYPE )?(score-partwise|opus|score-timewise)[\\s>]"))) {
            return musicxmlDefault;
        }
        if (std::regex_search(initial, std::regex("<(Piece xmlns=\"http://www.cmme.org\")[\\s>]"))) {
            return CMME;
        }
        LogWarning("Warning: Trying to load unknown XML data which cannot be identified.");
        return UNKNOWN;
    }
    if (initial.find("\n!!") != std::string::npos) {
        // Case where there are empty lines before content in Humdrum files.
        return HUMDRUM;
    }
    if (initial.find("\n**") != std::string::npos) {
        // Case where there are empty lines before content in Humdrum files.
        return HUMDRUM;
    }
    if (initial.find("\nCUT[") != std::string::npos) {
        // Title record for a melody in EsAC format.
        return ESAC;
    }

    // Assume that the input is MEI if other input types were not detected.
    // This means that DARMS cannot be auto-detected.
    return MEI;
}

bool Toolkit::LoadFile(const std::string &filename)
{
    this->ResetLogBuffer();

    if (this->IsUTF16(filename)) {
        return this->LoadUTF16File(filename);
    }
    if (this->IsZip(filename)) {
        return this->LoadZipFile(filename);
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

    return this->LoadData(content, false);
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

    LogInfo("The file seems to be UTF-16 - trying to convert to UTF-8");

    std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary);
    if (!fin.is_open()) {
        return false;
    }

    fin.seekg(0, std::ios::end);
    std::streamsize wfileSize = (std::streamsize)fin.tellg();
    fin.clear();

    fin.seekg(0, std::wios::beg);

    std::u16string u16data((wfileSize / 2) + 1, '\0');
    fin.read((char *)&u16data[0], wfileSize);

    // order of the bytes has to be flipped
    if (u16data.at(0) == u'\uFFFE') {
        LogInfo("The file seems to have been loaded as little endian - trying to convert to big endian");
        // convert to big endian (swap bytes)
        std::transform(std::begin(u16data), std::end(u16data), std::begin(u16data), [](char16_t c) {
            auto p = reinterpret_cast<char *>(&c);
            std::swap(p[0], p[1]);
            return c;
        });
    }

    // Skip the BOM
    if (u16data.at(0) == u'\uFEFF') {
        u16data.erase(0, 1);
    }

    // std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> convert;
    std::string utf8line = vrv::UTF16to8(u16data); // convert.to_bytes(u16data);

    return this->LoadData(utf8line, false);
}

std::string UTF16toUTF8(const std::u16string &input)
{
    std::string output;
    // Placeholder for manual conversion logic
    // Real conversion logic here should handle actual UTF-16 to UTF-8 conversion
    for (char16_t c : input) {
        if (c < 0x80) { // Handle basic ASCII conversion
            output.push_back(static_cast<char8_t>(c));
        }
        else {
            // Extend this block to handle non-ASCII characters
        }
    }
    return output;
}

bool Toolkit::IsZip(const std::string &filename)
{
    std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary);
    if (!fin.is_open()) {
        return false;
    }

    char data[4];
    memset(data, 0, 4);
    fin.read(data, 4);
    fin.close();

    if (memcmp(data, ZIP_SIGNATURE, 4) == 0) return true;

    return false;
}

bool Toolkit::LoadZipFile(const std::string &filename)
{
    std::ifstream fin(filename.c_str(), std::ios::in | std::ios::binary);
    if (!fin.is_open()) {
        return false;
    }

    fin.seekg(0, std::ios::end);
    std::streamsize fileSize = (std::streamsize)fin.tellg();
    fin.clear();
    fin.seekg(0, std::wios::beg);

    std::vector<unsigned char> bytes;
    bytes.reserve(fileSize + 1);

    unsigned char buffer;
    while (fin.read((char *)&buffer, sizeof(unsigned char))) {
        bytes.push_back(buffer);
    }

    return this->LoadZipData(bytes);
}

bool Toolkit::LoadZipData(const std::vector<unsigned char> &bytes)
{
    this->ResetLogBuffer();
#ifndef NO_MXL_SUPPORT
    ZipFileReader zipFileReader;
    zipFileReader.LoadBytes(bytes);

    const std::string metaInf = "META-INF/container.xml";
    if (!zipFileReader.HasFile(metaInf)) {
        LogError("No '%s' file to load found in the archive", metaInf.c_str());
        return false;
    }
    std::string containerXml = zipFileReader.ReadTextFile("META-INF/container.xml");
    pugi::xml_document doc;
    doc.load_buffer(containerXml.c_str(), containerXml.size());
    pugi::xml_node root = doc.first_child();
    pugi::xml_node rootfile = root.select_node("/container/rootfiles/rootfile").node();
    std::string filename = rootfile.attribute("full-path").value();

    if (!filename.empty()) {
        LogInfo("Loading file '%s' in the archive", filename.c_str());
        return this->LoadData(zipFileReader.ReadTextFile(filename), false);
    }
    else {
        LogError("No file to load found in the archive");
        return false;
    }
#else
    LogError("MXL import is not supported in this build.");
    return false;
#endif
}

bool Toolkit::LoadZipDataBase64(const std::string &data)
{
    std::vector<unsigned char> bytes = Base64Decode(data);
    return this->LoadZipData(bytes);
}

bool Toolkit::LoadZipDataBuffer(const unsigned char *data, int length)
{
    std::vector<unsigned char> bytes(data, data + length);
    return this->LoadZipData(bytes);
}

bool Toolkit::LoadData(const std::string &data)
{
    return this->LoadData(data, true);
}

void Toolkit::SetViewAndEditor()
{
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
}

bool Toolkit::LoadData(const std::string &data, bool resetLogBuffer)
{
    const Resources &resources = m_doc.GetResources();
    if (!resources.Ok()) {
        LogError("The data cannot be loaded because the font resources are not available");
        return false;
    }

    std::string newData;
    Input *input = NULL;

    if (resetLogBuffer) {
        this->ResetLogBuffer();
    }

    m_doc.m_expansionMap.Reset();

    if (m_options->m_xmlIdChecksum.GetValue()) {
        crcInit();
        unsigned int cr = crcFast((unsigned char *)data.c_str(), (int)data.size());
        Object::SeedID(cr);
    }

#ifndef NO_HUMDRUM_SUPPORT
    this->ClearHumdrumBuffer();
#endif

    auto inputFrom = m_inputFrom;
    if (inputFrom == AUTO) {
        inputFrom = IdentifyInputFrom(data);
    }
    if (inputFrom == ABC) {
#ifndef NO_ABC_SUPPORT
        input = new ABCInput(&m_doc);
#else
        LogError("ABC import is not supported in this build.");
        return false;
#endif
    }
    else if (inputFrom == PAE) {
#ifndef NO_PAE_SUPPORT
        input = new PAEInput(&m_doc);
#else
        LogError("Plaine & Easie import is not supported in this build.");
        return false;
#endif
    }
    else if (inputFrom == DARMS) {
#ifndef NO_DARMS_SUPPORT
        input = new DarmsInput(&m_doc);
#else
        LogError("DARMS import is not supported in this build.");
        return false;
#endif
    }
    else if (inputFrom == VOLPIANO) {
        input = new VolpianoInput(&m_doc);
    }
    else if (inputFrom == CMME) {
        if (m_options->m_durationEquivalence.GetValue() != DURATION_EQ_minima) {
            LogWarning("CMME input uses 'minima' duration equivalence, changing the option accordingly.");
            m_options->m_durationEquivalence.SetValue(DURATION_EQ_minima);
        }
        input = new CmmeInput(&m_doc);
    }
#ifndef NO_HUMDRUM_SUPPORT
    else if (inputFrom == HUMDRUM) {
        // LogInfo("Importing Humdrum data");

        // HumdrumInput *input = new HumdrumInput(&m_doc);
        input = new HumdrumInput(&m_doc);
        if (this->GetOutputTo() == HUMDRUM) {
            input->SetOutputFormat("humdrum");
        }

        if (!input->Import(data)) {
            LogError("Error importing Humdrum data (1)");
            delete input;
            return false;
        }
        this->SetHumdrumBuffer(((HumdrumInput *)input)->GetHumdrumString().c_str());
        if (this->GetOutputTo() == HUMDRUM) {
            // Humdrum data will be output (post-filtering data),
            // So not continuing converting to SVG.
            delete input;
            return true;
        }

        // Read embedded options from input Humdrum file:
        ((HumdrumInput *)input)->parseEmbeddedOptions(&m_doc);
    }
    else if (inputFrom == HUMMEI) {
        // convert first to MEI and then load MEI data via MEIInput.  This
        // allows using XPath processing.
        // LogInfo("Importing Humdrum data via MEI");
        Doc tempdoc;
        tempdoc.SetOptions(m_doc.GetOptions());
        HumdrumInput *tempinput = new HumdrumInput(&tempdoc);
        if (this->GetOutputTo() == HUMDRUM) {
            tempinput->SetOutputFormat("humdrum");
        }

        if (!tempinput->Import(data)) {
            LogError("Error importing Humdrum data (1)");
            delete tempinput;
            return false;
        }

        this->SetHumdrumBuffer(tempinput->GetHumdrumString().c_str());

        if (this->GetOutputTo() == HUMDRUM) {
            delete tempinput;
            return true;
        }

        MEIOutput meioutput(&tempdoc);
        meioutput.SetScoreBasedMEI(true);
        newData = meioutput.Export();

        // Read embedded options from input Humdrum file:
        tempinput->parseEmbeddedOptions(&m_doc);
        delete tempinput;

        input = new MEIInput(&m_doc);
    }
#endif
    else if (inputFrom == MEI) {
        input = new MEIInput(&m_doc);
    }
    else if (inputFrom == SERIALIZATION) {
        MEIInput *meiInput = new MEIInput(&m_doc);
        meiInput->SetDeserializing(true);
        input = meiInput;
    }
    else if (inputFrom == MUSICXML) {
        // This is the direct converter from MusicXML to MEI using iomusicxml:
        input = new MusicXmlInput(&m_doc);
    }
#ifndef NO_HUMDRUM_SUPPORT
    else if (inputFrom == MUSICXMLHUM) {
        // This is the indirect converter from MusicXML to MEI using iohumdrum:
        hum::Tool_musicxml2hum converter;
        pugi::xml_document xmlfile;
        xmlfile.load_string(data.c_str());
        stringstream conversion;

        this->LogRedirectStart();
        bool status = converter.convert(conversion, xmlfile);
        this->LogRedirectStop();
        if (!status) {
            LogWarning("Problem converting MusicXML to Humdrum (see warning above this line for possible reasons");
        }

        if (!status) {
            LogError("Error converting MusicXML data");
            return false;
        }
        std::string buffer = conversion.str();
        this->SetHumdrumBuffer(buffer.c_str());

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
        newData = meioutput.Export();
        delete tempinput;
        input = new MEIInput(&m_doc);
    }

    else if (inputFrom == MEIHUM) {
        this->ConvertMEIToHumdrum(data);

        // Now convert Humdrum into MEI:
        std::string conversion = this->GetHumdrumBuffer();
        Doc tempdoc;
        tempdoc.SetOptions(m_doc.GetOptions());
        Input *tempinput = new HumdrumInput(&tempdoc);
        if (!tempinput->Import(conversion)) {
            LogError("Error importing Humdrum data (3)");
            delete tempinput;
            return false;
        }
        MEIOutput meioutput(&tempdoc);
        meioutput.SetScoreBasedMEI(true);
        newData = meioutput.Export();
        delete tempinput;
        input = new MEIInput(&m_doc);
    }

    else if (inputFrom == MUSEDATAHUM) {
        // This is the indirect converter from MuseData to MEI using iohumdrum:
        hum::Tool_musedata2hum converter;
        stringstream conversion;

        this->LogRedirectStart();
        bool status = converter.convertString(conversion, data);
        this->LogRedirectStop();
        if (!status) {
            LogWarning("Problem converting MuseData to Humdrum (see warning above this line for possible reasons");
        }

        if (!status) {
            LogError("Error converting MuseData data");
            return false;
        }
        std::string buffer = conversion.str();
        this->SetHumdrumBuffer(buffer.c_str());

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
        newData = meioutput.Export();
        delete tempinput;
        input = new MEIInput(&m_doc);
    }

    else if (inputFrom == ESAC) {
        // This is the indirect converter from EsAC to MEI using iohumdrum:
        hum::Tool_esac2hum converter;
        std::stringstream conversion;

        this->LogRedirectStart();
        bool status = converter.convert(conversion, data);
        this->LogRedirectStop();
        if (!status) {
            LogWarning("Problem converting EsAC to Humdrum (see warning above this line for possible reasons");
        }

        if (!status) {
            LogError("Error converting EsAC data");
            return false;
        }
        std::string buffer = conversion.str();
        this->SetHumdrumBuffer(buffer.c_str());

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
        newData = meioutput.Export();
        delete tempinput;
        input = new MEIInput(&m_doc);
    }
#endif
    else {
        LogInfo("Unsupported format");
        return false;
    }

    // something went wrong
    if (!input) {
        LogError("Unknown error");
        return false;
    }

    // load the file
    if (inputFrom != HUMDRUM) {
        if (!input->Import(newData.size() ? newData : data)) {
            LogError("Error importing data");
            delete input;
            return false;
        }
    }

    if (inputFrom == SERIALIZATION) {
        input->Import(data);
        m_doc.PrepareData();
        m_doc.ScoreDefSetCurrentDoc(true);
        delete input;
        this->SetViewAndEditor();
        return true;
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
    // TODO better move this to PrepareData()
    m_doc.GenerateMeasureNumbers();

    // transpose the content if necessary
    if (m_options->m_transpose.IsSet() || m_options->m_transposeMdiv.IsSet()
        || m_options->m_transposeToSoundingPitch.IsSet()) {
        m_doc.PrepareData();
        m_doc.TransposeDoc();
    }

    m_doc.PrepareData();
    m_doc.InitSelectionDoc(m_docSelection, true);

    // Convert pseudo-measures into distinct segments based on barLine elements
    if (m_doc.IsMensuralMusicOnly()) {
        if (m_options->m_mensuralScoreUp.GetValue()) {
            m_doc.ScoringUpDoc();
        }
        if (m_options->m_mensuralResponsiveView.GetValue()) {
            m_doc.ConvertToMensuralViewDoc();
        }
        else if (m_options->m_mensuralToCmn.GetValue()) {
            m_doc.ConvertToCmnDoc();
        }
        else {
            m_doc.ConvertToCastOffMensuralDoc(true);
        }
    }

    // Do the layout? this depends on the options and the file. PAE and
    // DARMS have no layout information. MEI files _can_ have it, but it
    // might have been ignored because of the --breaks auto option.
    // Regardless, we won't do layout if the --breaks none option was set.
    int breaks = m_options->m_breaks.GetValue();

    // When loading page-based MEI, the layout is marked as done
    // In this case, we do not cast-off the document (breaks is expected to be not set)
    if (input->GetLayoutInformation() == LAYOUT_DONE) {
        if (breaks != BREAKS_auto) {
            LogWarning("Requesting layout with specific breaks but the layout is already done");
        }
        // We set it to 'none' for no cast-off process to be triggered
        breaks = BREAKS_none;
    }

    // Always set breaks to 'none' with Facs rendering
    if (m_doc.IsFacs()) breaks = BREAKS_none;

    // Always set breaks to 'none' or 'encoded' with Transcription rendering
    // rendering them differenty requires the MEI
    if (m_doc.IsTranscription()) {
        breaks = (m_doc.HasFacsimile()) ? BREAKS_encoded : BREAKS_none;
    }

    if (breaks != BREAKS_none) {
        if (input->GetLayoutInformation() == LAYOUT_ENCODED
            && (breaks == BREAKS_encoded || breaks == BREAKS_line || breaks == BREAKS_smart)) {
            if (breaks == BREAKS_encoded) {
                // LogElapsedTimeStart();
                m_doc.CastOffEncodingDoc();
                // LogElapsedTimeEnd("layout");
            }
            else if (breaks == BREAKS_line) {
                m_doc.CastOffLineDoc();
            }
            else if (breaks == BREAKS_smart) {
                m_doc.CastOffSmartDoc();
            }
        }
        else {
            if (breaks == BREAKS_encoded) {
                LogWarning("Requesting layout with encoded breaks but nothing provided in the data");
            }
            else if (breaks == BREAKS_line) {
                LogWarning("Requesting layout with line breaks but nothing provided in the data");
            }
            else if (breaks == BREAKS_smart) {
                LogWarning("Requesting layout with smart breaks but nothing provided in the data");
            }
            // LogElapsedTimeStart();
            m_doc.CastOffDoc();
            // LogElapsedTimeEnd("cast-off");
        }
    }
    else {
        // We need at least this to be done with breaks auto
        m_doc.ScoreDefSetCurrentDoc();
    }

    if (m_doc.IsTranscription() && m_doc.HasFacsimile()) {
        m_doc.SyncFromFacsimileDoc();
    }

    delete input;
    this->SetViewAndEditor();

    return true;
}

std::string Toolkit::GetMEI(const std::string &jsonOptions)
{
    bool scoreBased = true;
    bool basic = false;
    bool ignoreHeader = false;
    bool removeIds = m_options->m_removeIds.GetValue();
    int firstPage = 0;
    int lastPage = 0;
    std::string firstMeasure;
    std::string lastMeasure;
    std::string mdiv;
    bool generateFacs = false;
    bool serialized = false;

    jsonxx::Object json;

    // Read JSON options if not empty
    if (!jsonOptions.empty()) {
        if (!json.parse(jsonOptions)) {
            LogWarning("Cannot parse JSON std::string. Using default options.");
        }
        else {
            if (json.has<jsonxx::Boolean>("scoreBased")) scoreBased = json.get<jsonxx::Boolean>("scoreBased");
            if (json.has<jsonxx::Boolean>("basic")) basic = json.get<jsonxx::Boolean>("basic");
            if (json.has<jsonxx::Boolean>("ignoreHeader")) ignoreHeader = json.get<jsonxx::Boolean>("ignoreHeader");
            if (json.has<jsonxx::Boolean>("removeIds")) removeIds = json.get<jsonxx::Boolean>("removeIds");
            if (json.has<jsonxx::Number>("firstPage")) firstPage = json.get<jsonxx::Number>("firstPage");
            if (json.has<jsonxx::Number>("lastPage")) lastPage = json.get<jsonxx::Number>("lastPage");
            if (json.has<jsonxx::Number>("pageNo")) {
                firstPage = json.get<jsonxx::Number>("pageNo");
                lastPage = firstPage;
            }
            if (json.has<jsonxx::String>("firstMeasure")) firstMeasure = json.get<jsonxx::String>("firstMeasure");
            if (json.has<jsonxx::String>("lastMeasure")) lastMeasure = json.get<jsonxx::String>("lastMeasure");
            if (json.has<jsonxx::String>("mdiv")) mdiv = json.get<jsonxx::String>("mdiv");
            if (json.has<jsonxx::Boolean>("generateFacs")) generateFacs = json.get<jsonxx::Boolean>("generateFacs");
            if (json.has<jsonxx::Boolean>("serialized")) serialized = json.get<jsonxx::Boolean>("serialized");
        }
    }

    if (this->GetPageCount() == 0) {
        LogWarning("No data loaded");
        return "";
    }

    int initialPageNo = (m_doc.GetDrawingPage() == NULL) ? -1 : m_doc.GetDrawingPage()->GetIdx();

    bool hadSelection = false;
    if (m_doc.HasSelection()) {
        if (!scoreBased || serialized) {
            LogError("Page-based MEI or serialized output is not possible when a selection is set.");
            return "";
        }
        hadSelection = true;
        m_doc.DeactiveateSelection();
    }

    MEIOutput meioutput(&m_doc);
    meioutput.SetScoreBasedMEI(scoreBased);
    meioutput.SetBasic(basic);

    int indent = (m_options->m_outputIndentTab.GetValue()) ? -1 : m_options->m_outputIndent.GetValue();
    meioutput.SetIndent(indent);
    meioutput.SetIgnoreHeader(ignoreHeader);
    meioutput.SetRemoveIds(removeIds);

    if (firstPage > 0) meioutput.SetFirstPage(firstPage);
    if (lastPage > 0) meioutput.SetLastPage(lastPage);
    if (!firstMeasure.empty()) meioutput.SetFirstMeasure(firstMeasure);
    if (!lastMeasure.empty()) meioutput.SetLastMeasure(lastMeasure);
    if (!mdiv.empty()) meioutput.SetMdiv(mdiv);

    if (generateFacs) {
        if (meioutput.HasFilter() || !scoreBased || (m_options->m_breaks.GetValue() != BREAKS_encoded)
            || m_doc.HasSelection()) {
            LogError("Generating facsimile is only possible with all pages, encoded breaks, score-based output and "
                     "without selection.");
            return "";
        }
        m_doc.SyncToFacsimileDoc();
    }

    if (serialized) {
        if (meioutput.HasFilter()) {
            LogError("Page-base MEI serialization is only possible with all pages.");
            return "";
        }
        meioutput.SetSerializing(true);
        // Force these flags instead of cumbersome combination checking
        meioutput.SetBasic(false);
        meioutput.SetScoreBasedMEI(false);
    }

    std::string output = meioutput.Export();

    if (hadSelection) m_doc.ReactivateSelection(false);

    if (initialPageNo >= 0) m_doc.SetDrawingPage(initialPageNo);
    return output;
}

std::string Toolkit::ValidatePAEFile(const std::string &filename)
{
    std::ifstream inFile;
    inFile.open(filename);

    std::stringstream sstream;
    sstream << inFile.rdbuf();
    return this->ValidatePAE(sstream.str());
}

std::string Toolkit::ValidatePAE(const std::string &data)
{
    PAEInput input(&m_doc);
    input.Import(data);
    m_doc.Reset();
    return input.GetValidationLog().json();
}

bool Toolkit::SaveFile(const std::string &filename, const std::string &jsonOptions)
{
    std::string output = this->GetMEI(jsonOptions);
    if (output.empty()) {
        return false;
    }

    std::ofstream outfile;
    outfile.open(filename.c_str());

    if (!outfile.is_open()) {
        LogError("Unable to write MEI to %s", filename.c_str());
        return false;
    }

    outfile << output;
    outfile.close();
    return true;
}

std::string Toolkit::GetOptions() const
{
    return this->GetOptions(false);
}

std::string Toolkit::GetDefaultOptions() const
{
    return this->GetOptions(true);
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
        const OptionJson *optJson = dynamic_cast<const OptionJson *>(iter->second);

        if (optDbl) {
            double dblValue = (defaultValues) ? optDbl->GetDefault() : optDbl->GetUnfactoredValue();
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
            jsonxx::Array values;
            for (const std::string &strValue : strValues) {
                values << strValue;
            }
            o << iter->first << values;
        }
        else if (optJson) {
            // Reading json from file is not supported in toolkit
            if (optJson->GetSource() == JsonSource::String) {
                o << iter->first << optJson->GetValue(defaultValues);
            }
        }
        else {
            std::string stringValue
                = (defaultValues) ? iter->second->GetDefaultStrValue() : iter->second->GetStrValue();
            o << iter->first << stringValue;
        }
    }

    // Other base options
    int scaleValue = (defaultValues) ? m_options->m_scale.GetDefault() : m_options->m_scale.GetUnfactoredValue();
    o << "scale" << scaleValue;

    int xmlIdSeedValue
        = (defaultValues) ? m_options->m_xmlIdSeed.GetDefault() : m_options->m_xmlIdSeed.GetUnfactoredValue();
    o << "xmlIdSeed" << xmlIdSeedValue;

    return o.json();
}

std::string Toolkit::GetAvailableOptions() const
{
    jsonxx::Object o;
    jsonxx::Object grps;

    grps << "0-base" << m_options->GetBaseOptGrp();

    const std::vector<OptionGrp *> *optionGrps = m_options->GetGrps();
    for (OptionGrp *optionGrp : *optionGrps) {

        jsonxx::Object grp;
        grp << "name" << optionGrp->GetLabel();

        jsonxx::Object opts;

        const std::vector<Option *> *options = optionGrp->GetOptions();

        for (Option *option : *options) {
            assert(option);
            // Reading json from file is not supported in toolkit
            const OptionJson *optJson = dynamic_cast<const OptionJson *>(option);
            if (optJson && (optJson->GetSource() == JsonSource::FilePath)) continue;

            opts << option->GetKey() << option->ToJson();
        }

        grp << "options" << opts;
        grps << optionGrp->GetId() << grp;
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
            if (iter->first == "inputFrom") {
                if (json.has<jsonxx::String>("inputFrom")) {
                    this->SetInputFrom(json.get<jsonxx::String>("inputFrom"));
                }
            }
            else if (iter->first == "outputTo") {
                if (json.has<jsonxx::String>("outputTo")) {
                    this->SetOutputTo(json.get<jsonxx::String>("outputTo"));
                }
            }
            else if (iter->first == "scale") {
                if (json.has<jsonxx::Number>("scale")) {
                    this->SetScale(json.get<jsonxx::Number>("scale"));
                }
            }
            else if (iter->first == "xmlIdSeed") {
                if (json.has<jsonxx::Number>("xmlIdSeed")) {
                    m_options->m_xmlIdSeed.SetValue(json.get<jsonxx::Number>("xmlIdSeed"));
                    Object::SeedID(m_options->m_xmlIdSeed.GetValue());
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
            // LogInfo("Double: %f", json.get<jsonxx::Number>(iter->first));
        }
        else if (json.has<jsonxx::Boolean>(iter->first)) {
            opt->SetValueBool(json.get<jsonxx::Boolean>(iter->first));
            // LogInfo("Bool: %d", json.get<jsonxx::Boolean>(iter->first));
        }
        else if (json.has<jsonxx::String>(iter->first)) {
            opt->SetValue(json.get<jsonxx::String>(iter->first));
            // LogInfo("String: %s", json.get<jsonxx::String>(iter->first).c_str());
        }
        else if (json.has<jsonxx::Array>(iter->first)) {
            jsonxx::Array values = json.get<jsonxx::Array>(iter->first);
            std::vector<std::string> strValues;
            for (int i = 0; i < (int)values.size(); ++i) {
                if (values.has<jsonxx::String>(i)) strValues.push_back(values.get<jsonxx::String>(i));
                // LogDebug("String: %s", values.get<jsonxx::String>(i).c_str());
            }
            opt->SetValueArray(strValues);
        }
        else if (json.has<jsonxx::Object>(iter->first)) {
            const OptionJson *optJson = dynamic_cast<OptionJson *>(opt);
            if (optJson && (optJson->GetSource() == JsonSource::String)) {
                const jsonxx::Object value = json.get<jsonxx::Object>(iter->first);
                opt->SetValue(value.json());
            }
        }
        else {
            LogError("Unsupported type for option '%s'", iter->first.c_str());
        }
    }

    m_options->Sync();

    this->SetLocale();

    // Forcing font resource to be reset if the font is given in the options
    if (json.has<jsonxx::Array>("fontAddCustom")) {
        Resources &resources = m_doc.GetResourcesForModification();
        resources.AddCustom(m_options->m_fontAddCustom.GetValue());
    }
    if (json.has<jsonxx::String>("font")) {
        this->SetFont(m_options->m_font.GetValue());
    }
    if (json.has<jsonxx::String>("fontFallback")) {
        Resources &resources = m_doc.GetResourcesForModification();
        resources.SetFallbackFont(m_options->m_fontFallback.GetStrValue());
    }
    if (json.has<jsonxx::Boolean>("fontLoadAll")) {
        Resources &resources = m_doc.GetResourcesForModification();
        resources.LoadAll();
    }
    if (json.has<jsonxx::String>("fontTextLiberation")) {
        Resources &resources = m_doc.GetResourcesForModification();
        resources.UseLiberationTextFont(m_options->m_fontTextLiberation.GetValue());
    }

    return true;
}

void Toolkit::ResetOptions()
{
    std::for_each(m_options->GetItems()->begin(), m_options->GetItems()->end(),
        [](const MapOfStrOptions::value_type &opt) { opt.second->Reset(); });

    // Set the (default) font
    this->SetFont(m_options->m_font.GetValue());
}

void Toolkit::PrintOptionUsageOutput(const vrv::Option *option, std::ostream &output) const
{
    if (!option) return;
    std::string option_str = " ";
    if (option->GetShortOption()) {
        option_str.append("-");
        option_str.push_back(option->GetShortOption());
        option_str.append(", ");
    }

    if (!option->GetKey().empty()) {
        option_str.append("--");
        option_str.append(vrv::FromCamelCase(option->GetKey()));
    }

    const vrv::OptionDbl *optDbl = dynamic_cast<const vrv::OptionDbl *>(option);
    const vrv::OptionInt *optInt = dynamic_cast<const vrv::OptionInt *>(option);
    const vrv::OptionIntMap *optIntMap = dynamic_cast<const vrv::OptionIntMap *>(option);
    const vrv::OptionString *optString = dynamic_cast<const vrv::OptionString *>(option);
    const vrv::OptionArray *optArray = dynamic_cast<const vrv::OptionArray *>(option);
    const vrv::OptionBool *optBool = dynamic_cast<const vrv::OptionBool *>(option);

    if (optDbl) {
        option_str.append(" <f>");
    }
    else if (optInt) {
        option_str.append(" <i>");
    }
    else if (optString) {
        option_str.append(" <s>");
    }
    else if (optArray) {
        option_str.append("* <s>");
    }
    else if (!optBool) {
        option_str.append(" <s>");
    }

    const int helpTabs = 32;
    if (option_str.size() < helpTabs) {
        option_str.insert(option_str.end(), helpTabs - option_str.size(), ' ');
    }
    else {
        option_str.append("\t");
    }

    output << option_str << option->GetDescription();
    if (optInt && (optInt->GetMin() != optInt->GetMax())) {
        output << " (default: " << optInt->GetDefault();
        output << "; min: " << optInt->GetMin();
        output << "; max: " << optInt->GetMax() << ")";
    }
    if (optDbl && (optDbl->GetMin() != optDbl->GetMax())) {
        output << std::fixed << " (default: " << optDbl->GetDefault();
        output << "; min: " << optDbl->GetMin();
        output << "; max: " << optDbl->GetMax() << ")";
    }
    if (optString) {
        output << " (default: \"" << optString->GetDefault() << "\")";
    }
    if (optIntMap) {
        output << " (default: \"" << optIntMap->GetDefaultStrValue()
               << "\"; other values: " << optIntMap->GetStrValuesAsStr(true) << ")";
    }
    output << std::endl;
}

void Toolkit::PrintOptionUsage(const std::string &category, std::ostream &output) const
{
    // map of all categories and expected string arguments for them
    const std::map<vrv::OptionsCategory, std::string> categories = { { vrv::OptionsCategory::Base, "base" },
        { vrv::OptionsCategory::General, "general" }, { vrv::OptionsCategory::Json, "json" },
        { vrv::OptionsCategory::Layout, "layout" }, { vrv::OptionsCategory::Margins, "margins" },
        { vrv::OptionsCategory::Mensural, "mensural" }, { vrv::OptionsCategory::Midi, "midi" },
        { vrv::OptionsCategory::Selectors, "selectors" }, { vrv::OptionsCategory::Full, "full" } };

    output.precision(2);
    // display_version();
    output << "Verovio " << this->GetVersion() << std::endl;
    output << std::endl << "Example usage:" << std::endl << std::endl;
    output << " verovio [-s scale] [-r resource-path] [-o outfile] infile" << std::endl << std::endl;

    auto it = std::find_if(
        categories.begin(), categories.end(), [&category](const std::pair<vrv::OptionsCategory, std::string> &value) {
            return std::equal(value.second.begin(), value.second.end(), category.begin(), category.end(),
                [](const char a, const char b) { return a == tolower(b); });
        });

    if (it == categories.end()) {
        std::string optionStr;
        output << "Help manual categories: " << std::endl;
        // Print base group options
        optionStr.append(" -h ");
        optionStr.append(categories.at(m_options->m_baseOptions.GetCategory()));
        optionStr.append("\t");
        optionStr.append(m_options->m_baseOptions.GetLabel());
        optionStr.append("\n");

        const std::vector<vrv::OptionGrp *> *grps = m_options->GetGrps();
        // Print each group one by one
        for (const auto group : *grps) {
            optionStr.append(" -h ");
            optionStr.append(categories.at(group->GetCategory()));
            optionStr.append("\t");
            optionStr.append(group->GetLabel());
            optionStr.append("\n");
        }
        optionStr.append(" -h full\tPrint all help manual and exit");
        output << optionStr << std::endl;
    }
    else {
        output << "Options (marked as * are repeatable)" << std::endl;
        if ((it->first == vrv::OptionsCategory::Base) || (it->first == vrv::OptionsCategory::Full)) {
            const std::vector<vrv::Option *> *baseOptions = m_options->GetBaseOptions();
            for (vrv::Option *option : *baseOptions) {
                this->PrintOptionUsageOutput(option, output);
            }
        }
        const std::vector<vrv::OptionGrp *> *grps = m_options->GetGrps();
        for (const auto group : *grps) {
            if (it->first == group->GetCategory() || (it->first == vrv::OptionsCategory::Full)) {
                // Options with long forms only
                output << std::endl << group->GetLabel() << std::endl;
                const std::vector<vrv::Option *> *options = group->GetOptions();

                for (vrv::Option *option : *options) {
                    this->PrintOptionUsageOutput(option, output);
                }
            }
        }
    }
}

std::string Toolkit::GetOptionUsageString() const
{
    std::stringstream ss;
    this->PrintOptionUsage("full", ss);
    return ss.str();
}

std::string Toolkit::GetElementAttr(const std::string &xmlId)
{
    jsonxx::Object o;

    const Object *element = NULL;

    // Try to get the element on the current drawing page - it is usually the case and fast
    if (m_doc.GetDrawingPage()) {
        element = m_doc.GetDrawingPage()->FindDescendantByID(xmlId);
    }
    // If it wasn't there, try on the whole doc
    if (!element) {
        element = m_doc.FindDescendantByID(xmlId);
    }
    // If not found again, try looking in the layer staffdefs
    if (!element) {
        // This will also look in the score/scoreDef
        FindElementInLayerStaffDefFunctor findElementInLayerStaffDef(xmlId);
        // Check drawing page elements first
        if (m_doc.GetDrawingPage()) {
            m_doc.GetDrawingPage()->Process(findElementInLayerStaffDef);
            element = findElementInLayerStaffDef.GetElement();
        }
        if (!element) {
            m_doc.Process(findElementInLayerStaffDef);
            element = findElementInLayerStaffDef.GetElement();
        }
        // If element is found within layer staffdef - check for the linking interface @corresp attribute to find
        // original ID of the element
        if (element) {
            // If element has @corresp set, try finding its origin
            const LinkingInterface *link = element->GetLinkingInterface();
            if (link && link->HasCorresp()) {
                const std::string correspId = ExtractIDFragment(link->GetCorresp());
                Object *origin = m_doc.FindDescendantByID(correspId);
                // if no original element was found, try searching through scoredef in score (only for certain elements)
                if (!origin && element->Is({ CLEF, GRPSYM, KEYSIG, MENSUR, METERSIG, METERSIGGRP })) {
                    Page *page = vrv_cast<Page *>(m_doc.FindDescendantByType(PAGE));
                    if (page && page->m_score && page->m_score->GetScoreDef()) {
                        origin = page->m_score->GetScoreDef()->FindDescendantByID(correspId);
                    }
                }
                if (origin) element = origin;
            }
        }
    }
    // If not found at all
    if (!element) {
        LogWarning("Element '%s' not found", xmlId.c_str());
        return o.json();
    }

    // Fill the attribute array (pair of std::string) by looking at attributes for all available MEI modules
    ArrayOfStrAttr attributes;
    element->GetAttributes(&attributes);

    // Fill the JSON object
    for (const auto &attribute : attributes) {
        o << attribute.first << attribute.second;
        // LogInfo("Element %s - %s", attribute.first.c_str(), attribute.second.c_str());
    }
    return o.json();
}

std::string Toolkit::GetNotatedIdForElement(const std::string &xmlId)
{
    if (m_doc.m_expansionMap.HasExpansionMap()) {
        return m_doc.m_expansionMap.GetExpansionIDsForElement(xmlId).front();
    }
    else {
        return xmlId;
    }
}

std::string Toolkit::GetExpansionIdsForElement(const std::string &xmlId)
{
    jsonxx::Array a;
    if (m_doc.m_expansionMap.HasExpansionMap()) {
        for (std::string id : m_doc.m_expansionMap.GetExpansionIDsForElement(xmlId)) {
            a << id;
        }
    }
    else {
        a << "";
    }
    return a.json();
}

bool Toolkit::Edit(const std::string &editorAction)
{
    this->ResetLogBuffer();

    return m_editorToolkit->ParseEditorAction(editorAction);
}

std::string Toolkit::EditInfo()
{
    return m_editorToolkit->EditInfo();
}

std::string Toolkit::GetLog()
{
    std::string str;
    for (const std::string &logStr : logBuffer) {
        str += logStr;
    }
    this->ResetLogBuffer();
    return str;
}

std::string Toolkit::GetVersion() const
{
    return vrv::GetVersion();
}

void Toolkit::ResetXmlIdSeed(int seed)
{
    m_options->m_xmlIdSeed.SetValue(seed);
    Object::SeedID(m_options->m_xmlIdSeed.GetValue());
}

void Toolkit::ResetLogBuffer()
{
    logBuffer.clear();
}

void Toolkit::LogRedirectStart()
{
    if (m_cerrOriginalBuf) {
        vrv::LogError("In Toolkit::LogRedirectStart: Only one log redirect can be active at a time.");
        return;
    }
    if (!m_cerrCaptured.str().empty()) {
        vrv::LogWarning("In Toolkit::LogRedirectStart: Log capture buffer not empty, sending current contents to "
                        "LogWarning and resetting.");
        vrv::LogWarning(m_cerrCaptured.str().c_str());
        m_cerrCaptured.str("");
    }
    m_cerrOriginalBuf = std::cerr.rdbuf();
    std::cerr.rdbuf(m_cerrCaptured.rdbuf());
}

void Toolkit::LogRedirectStop()
{
    if (!m_cerrCaptured.str().empty()) {
        vrv::LogWarning(m_cerrCaptured.str().c_str());
        m_cerrCaptured.str("");
    }

    if (m_cerrOriginalBuf) {
        std::cerr.rdbuf(m_cerrOriginalBuf);
        m_cerrOriginalBuf = NULL;
    }
}

void Toolkit::RedoLayout(const std::string &jsonOptions)
{
    bool resetCache = true;

    jsonxx::Object json;

    // Read JSON options if not empty
    if (!jsonOptions.empty()) {
        if (!json.parse(jsonOptions)) {
            LogWarning("Cannot parse JSON std::string. Using default options.");
        }
        else {
            if (json.has<jsonxx::Boolean>("resetCache")) resetCache = json.get<jsonxx::Boolean>("resetCache");
        }
    }

    this->ResetLogBuffer();

    if ((this->GetPageCount() == 0) || m_doc.IsTranscription() || m_doc.IsFacs()) {
        LogWarning("No data to re-layout");
        return;
    }

    if (m_docSelection.m_isPending) {
        m_doc.InitSelectionDoc(m_docSelection, resetCache);
    }
    else {
        m_doc.UnCastOffDoc(resetCache);
    }

    if (m_options->m_breaks.GetValue() == BREAKS_line) {
        m_doc.CastOffLineDoc();
    }
    else if (m_options->m_breaks.GetValue() == BREAKS_encoded && m_doc.FindDescendantByType(PB)) {
        m_doc.CastOffEncodingDoc();
    }
    else if (m_options->m_breaks.GetValue() == BREAKS_smart) {
        m_doc.CastOffSmartDoc();
    }
    else if (m_options->m_breaks.GetValue() != BREAKS_none) {
        m_doc.CastOffDoc();
    }
}

void Toolkit::RedoPagePitchPosLayout()
{
    this->ResetLogBuffer();

    Page *page = m_doc.GetDrawingPage();

    if (!page) {
        LogWarning("No page to re-layout");
        return;
    }

    page->LayOutPitchPos();
}

bool Toolkit::RenderToDeviceContext(int pageNo, DeviceContext *deviceContext)
{
    if (pageNo > this->GetPageCount()) {
        LogWarning("Page %d does not exist", pageNo);
        return false;
    }

    // Page number is one-based - correct it to 0-based first
    pageNo--;

    m_doc.SetDrawingPage(pageNo, true);
    m_view.SetPage(m_doc.GetDrawingPage(), true);

    // Adjusting page width and height according to the options
    int width = m_options->m_pageWidth.GetUnfactoredValue();
    int height = m_options->m_pageHeight.GetUnfactoredValue();
    int breaks = m_options->m_breaks.GetValue();
    bool adjustHeight = m_options->m_adjustPageHeight.GetValue();
    bool adjustWidth = m_options->m_adjustPageWidth.GetValue();

    if (adjustWidth || (breaks == BREAKS_none)) width = m_doc.GetAdjustedDrawingPageWidth();
    if (adjustHeight || (breaks == BREAKS_none)) height = m_doc.GetAdjustedDrawingPageHeight();

    if (m_doc.IsTranscription()) {
        width = m_doc.GetAdjustedDrawingPageWidth();
        height = m_doc.GetAdjustedDrawingPageHeight();
    }

    // set dimensions
    if (m_options->m_landscape.GetValue()) {
        std::swap(height, width);
    }

    double userScale = m_options->m_scale.GetValue() / 100.0;
    assert(userScale != 0.0);

    if (m_options->m_scaleToPageSize.GetValue()) {
        deviceContext->SetBaseSize(width, height);
        height *= (1.0 / userScale);
        width *= (1.0 / userScale);
    }

    deviceContext->SetUserScale(userScale, userScale);
    deviceContext->SetWidth(width);
    deviceContext->SetHeight(height);
    deviceContext->SetViewBoxFactor(m_view.GetPPUFactor());

    if (m_doc.IsFacs()) {
        deviceContext->SetWidth(m_doc.GetFacsimile()->GetMaxX());
        deviceContext->SetHeight(m_doc.GetFacsimile()->GetMaxY());
    }

    // render the page
    m_view.DrawCurrentPage(deviceContext, false);

    return true;
}

std::string Toolkit::RenderData(const std::string &data, const std::string &jsonOptions)
{
    if (this->SetOptions(jsonOptions) && this->LoadData(data, false)) return this->RenderToSVG(1);

    // Otherwise just return an empty string.
    return "";
}

std::string Toolkit::RenderToSVG(int pageNo, bool xmlDeclaration)
{
    this->ResetLogBuffer();

    // Create the SVG object, h & w come from the system
    // We will need to set the size of the page after having drawn it depending on the options
    SvgDeviceContext svg(m_doc.GetID());
    svg.SetResources(&m_doc.GetResources());

    int indent = (m_options->m_outputIndentTab.GetValue()) ? -1 : m_options->m_outputIndent.GetValue();
    svg.SetIndent(indent);

    if (m_options->m_mmOutput.GetValue()) {
        svg.SetMMOutput(true);
    }

    if (m_doc.IsFacs()) {
        svg.SetFacsimile(true);
    }

    // set the option to use viewbox on svg root
    if (m_options->m_svgBoundingBoxes.GetValue()) {
        svg.SetSvgBoundingBoxes(true);
    }

    // set the additional CSS if any
    if (!m_options->m_svgCss.GetValue().empty()) {
        svg.SetCss(m_options->m_svgCss.GetValue());
    }

    if (m_options->m_svgViewBox.GetValue()) {
        svg.SetSvgViewBox(true);
    }

    if (m_options->m_fontTextLiberation.GetValue()) {
        svg.SetUseLiberation(true);
    }

    svg.SetHtml5(m_options->m_svgHtml5.GetValue());
    svg.SetFormatRaw(m_options->m_svgFormatRaw.GetValue());
    svg.SetRemoveXlink(m_options->m_svgRemoveXlink.GetValue());
    svg.SetAdditionalAttributes(m_options->m_svgAdditionalAttribute.GetValue());
    svg.SetSmuflTextFont((option_SMUFLTEXTFONT)m_options->m_smuflTextFont.GetValue());

    // render the page
    this->RenderToDeviceContext(pageNo, &svg);

    std::string out_str = svg.GetStringSVG(xmlDeclaration);
    return out_str;
}

bool Toolkit::RenderToSVGFile(const std::string &filename, int pageNo)
{
    this->ResetLogBuffer();

    std::string output = this->RenderToSVG(pageNo, true);

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
    return this->GetHumdrumBuffer();
}

bool Toolkit::GetHumdrumFile(const std::string &filename)
{
    std::ofstream output;
    output.open(filename.c_str());

    if (!output.is_open()) {
        // add message?
        return false;
    }

    this->GetHumdrum(output);
    output.close();
    return true;
}

void Toolkit::GetHumdrum(std::ostream &output)
{
    output << this->GetHumdrumBuffer();
}

std::string Toolkit::RenderToMIDI()
{
    this->ResetLogBuffer();

    smf::MidiFile outputfile;
    m_doc.ExportMIDI(&outputfile);

    std::stringstream stream;
    outputfile.write(stream);
    std::string outputstr = Base64Encode(
        reinterpret_cast<const unsigned char *>(stream.str().c_str()), (unsigned int)stream.str().length());

    return outputstr;
}

std::string Toolkit::RenderToPAE()
{
    this->ResetLogBuffer();

    if (this->GetPageCount() == 0) {
        LogWarning("No data loaded");
        return "";
    }

    PAEOutput paeOutput(&m_doc);
    std::string output = paeOutput.Export();
    if (output.empty()) {
        LogError("Export to PAE failed");
    }
    return output;
}

bool Toolkit::RenderToPAEFile(const std::string &filename)
{
    this->ResetLogBuffer();

    std::string outputString = this->RenderToPAE();

    std::ofstream output(filename.c_str());
    if (!output.is_open()) {
        return false;
    }
    output << outputString;

    return true;
}

std::string Toolkit::RenderToTimemap(const std::string &jsonOptions)
{
    bool includeMeasures = false;
    bool includeRests = false;
    bool useFractions = false;

    jsonxx::Object json;

    // Read JSON options if not empty
    if (!jsonOptions.empty()) {
        if (!json.parse(jsonOptions)) {
            LogWarning("Cannot parse JSON std::string. Using default options.");
        }
        else {
            if (json.has<jsonxx::Boolean>("includeMeasures"))
                includeMeasures = json.get<jsonxx::Boolean>("includeMeasures");
            if (json.has<jsonxx::Boolean>("includeRests")) includeRests = json.get<jsonxx::Boolean>("includeRests");
            if (json.has<jsonxx::Boolean>("useFractions")) useFractions = json.get<jsonxx::Boolean>("useFractions");
        }
    }

    this->ResetLogBuffer();

    std::string output;
    m_doc.ExportTimemap(output, includeRests, includeMeasures, useFractions);
    return output;
}

std::string Toolkit::RenderToExpansionMap()
{
    this->ResetLogBuffer();

    std::string output;
    m_doc.ExportExpansionMap(output);
    return output;
}

std::string Toolkit::GetElementsAtTime(int millisec)
{
    this->ResetLogBuffer();

    jsonxx::Object o;
    jsonxx::Array noteArray;
    jsonxx::Array chordArray;
    jsonxx::Array restArray;

    // Here we need to check that the MIDI timemap is done
    if (!m_doc.HasTimemap()) {
        // generate MIDI timemap before progressing
        m_doc.CalculateTimemap();
    }

    MeasureOnsetOffsetComparison matchMeasureTime(millisec);
    Measure *measure = dynamic_cast<Measure *>(m_doc.FindDescendantByComparison(&matchMeasureTime));

    if (!measure) {
        return o.json();
    }

    int repeat = measure->EnclosesTime(millisec);
    int measureTimeOffset = measure->GetRealTimeOnsetMilliseconds(repeat);

    // Get the pageNo from the first note (if any)
    int pageNo = -1;
    Page *page = vrv_cast<Page *>(measure->GetFirstAncestor(PAGE));
    if (page) pageNo = page->GetIdx() + 1;

    NoteOrRestOnsetOffsetComparison matchTime(millisec - measureTimeOffset);
    ListOfObjects notesOrRests;
    ListOfObjects chords;

    measure->FindAllDescendantsByComparison(&notesOrRests, &matchTime);
    ClassIdsComparison mRestComparison({ MULTIREST, MREST });
    measure->FindAllDescendantsByComparison(&notesOrRests, &mRestComparison, UNLIMITED_DEPTH, FORWARD, false);

    // Fill the JSON object
    for (Object *object : notesOrRests) {
        if (object->Is(NOTE)) {
            noteArray << object->GetID();
            Note *note = vrv_cast<Note *>(object);
            assert(note);
            Chord *chord = note->IsChordTone();
            if (chord) chords.push_back(chord);
        }
        else if (object->Is({ MREST, MULTIREST, REST })) {
            restArray << object->GetID();
        }
    }
    chords.unique();
    for (Object *object : chords) {
        chordArray << object->GetID();
    }

    o << "notes" << noteArray;
    o << "chords" << chordArray;
    o << "rests" << restArray;
    o << "page" << pageNo;
    o << "measure" << measure->GetID();

    return o.json();
}

bool Toolkit::RenderToMIDIFile(const std::string &filename)
{
    this->ResetLogBuffer();

    smf::MidiFile outputfile;
    m_doc.ExportMIDI(&outputfile);
    outputfile.write(filename);

    return true;
}

bool Toolkit::RenderToTimemapFile(const std::string &filename, const std::string &jsonOptions)
{
    std::string outputString = this->RenderToTimemap(jsonOptions);

    std::ofstream output(filename.c_str());
    if (!output.is_open()) {
        return false;
    }
    output << outputString;

    return true;
}

bool Toolkit::RenderToExpansionMapFile(const std::string &filename)
{
    std::string outputString = this->RenderToExpansionMap();

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

std::string Toolkit::GetDescriptiveFeatures(const std::string &options)
{
    // For now do not handle any option
    std::string output;
    m_doc.ExportFeatures(output, options);
    return output;
}

int Toolkit::GetPageWithElement(const std::string &xmlId)
{
    Object *element = m_doc.FindDescendantByID(xmlId);
    if (!element) {
        LogWarning("Element '%s' not found", xmlId.c_str());
        return 0;
    }
    Page *page = vrv_cast<Page *>(element->GetFirstAncestor(PAGE));
    if (!page) {
        return 0;
    }
    return page->GetIdx() + 1;
}

int Toolkit::GetTimeForElement(const std::string &xmlId)
{
    this->ResetLogBuffer();

    Object *element = m_doc.FindDescendantByID(xmlId);

    if (!element) {
        LogWarning("Element '%s' not found", xmlId.c_str());
        return 0;
    }

    int timeofElement = 0;
    if (!m_doc.HasTimemap()) {
        // generate MIDI timemap before progressing
        m_doc.CalculateTimemap();
    }
    if (!m_doc.HasTimemap()) {
        LogWarning("Calculation of MIDI timemap failed, time value is invalid.");
    }
    if (element->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(element);
        assert(note);
        Measure *measure = vrv_cast<Measure *>(note->GetFirstAncestor(MEASURE));
        assert(measure);
        // For now ignore repeats and access always the first
        timeofElement = measure->GetRealTimeOnsetMilliseconds(1);
        timeofElement += note->GetRealTimeOnsetMilliseconds();
    }
    else if (element->Is(MEASURE)) {
        Measure *measure = vrv_cast<Measure *>(element);
        assert(measure);
        // For now ignore repeats and access always the first
        timeofElement = measure->GetRealTimeOnsetMilliseconds(1);
    }
    else if (element->Is(CHORD)) {
        Chord *chord = vrv_cast<Chord *>(element);
        assert(chord);
        Note *note = vrv_cast<Note *>(chord->FindDescendantByType(NOTE));
        assert(note);
        Measure *measure = vrv_cast<Measure *>(note->GetFirstAncestor(MEASURE));
        assert(measure);
        // For now ignore repeats and access always the first
        timeofElement = measure->GetRealTimeOnsetMilliseconds(1);
        timeofElement += note->GetRealTimeOnsetMilliseconds();
    }
    return timeofElement;
}

std::string Toolkit::GetTimesForElement(const std::string &xmlId)
{
    this->ResetLogBuffer();

    Object *element = m_doc.FindDescendantByID(xmlId);
    jsonxx::Object o;

    if (!element) {
        LogWarning("Element '%s' not found", xmlId.c_str());
        return o.json();
    }

    jsonxx::Array scoreTimeOnset;
    jsonxx::Array scoreTimeOffset;
    jsonxx::Array scoreTimeDuration;
    jsonxx::Array scoreTimeTiedDuration;
    jsonxx::Array realTimeOnsetMilliseconds;
    jsonxx::Array realTimeOffsetMilliseconds;

    if (!m_doc.HasTimemap()) {
        // generate MIDI timemap before progressing
        m_doc.CalculateTimemap();
    }
    if (!m_doc.HasTimemap()) {
        LogWarning("Calculation of MIDI timemap failed, time value is invalid.");
        return o.json();
    }
    if (element->Is(NOTE)) {

        Note *note = vrv_cast<Note *>(element);
        assert(note);
        Measure *measure = vrv_cast<Measure *>(note->GetFirstAncestor(MEASURE));
        assert(measure);

        // For now ignore repeats and access always the first
        double mRealTimeOnsetMilliseconds = measure->GetRealTimeOnsetMilliseconds(1);
        realTimeOnsetMilliseconds << mRealTimeOnsetMilliseconds + note->GetRealTimeOnsetMilliseconds();
        realTimeOffsetMilliseconds << mRealTimeOnsetMilliseconds + note->GetRealTimeOffsetMilliseconds();

        Fraction mScoreTimeOnset = measure->GetScoreTimeOnset(1);
        scoreTimeOnset << jsonxx::Value(Timemap::ToArray(mScoreTimeOnset + note->GetScoreTimeOnset()));
        scoreTimeOffset << jsonxx::Value(Timemap::ToArray(mScoreTimeOnset + note->GetScoreTimeOffset()));
        scoreTimeDuration << jsonxx::Value(Timemap::ToArray(note->GetScoreTimeDuration()));
        scoreTimeTiedDuration << jsonxx::Value(Timemap::ToArray(note->GetScoreTimeTiedDuration()));

        o << "qfracOn" << scoreTimeOnset;
        o << "qfracOff" << scoreTimeOffset;
        o << "qfracDuration" << scoreTimeDuration;
        o << "qfracTiedDuration" << scoreTimeTiedDuration;
        o << "tstampOn" << realTimeOnsetMilliseconds;
        o << "tstampOff" << realTimeOffsetMilliseconds;
    }
    return o.json();
}

std::string Toolkit::GetMIDIValuesForElement(const std::string &xmlId)
{
    this->ResetLogBuffer();

    Object *element = m_doc.FindDescendantByID(xmlId);
    jsonxx::Object o;

    if (!element) {
        LogWarning("Element '%s' not found", xmlId.c_str());
        return o.json();
    }

    if (element->Is(NOTE)) {
        if (!m_doc.HasTimemap()) {
            // generate MIDI timemap before progressing
            m_doc.CalculateTimemap();
        }
        if (!m_doc.HasTimemap()) {
            LogWarning("Calculation of MIDI timemap failed, time value is invalid.");
            return o.json();
        }
        Note *note = vrv_cast<Note *>(element);
        assert(note);
        const int timeOfElement = this->GetTimeForElement(xmlId);
        const int pitchOfElement = note->GetMIDIPitch();
        const int durationOfElement = note->GetRealTimeOffsetMilliseconds() - note->GetRealTimeOnsetMilliseconds();
        o << "time" << timeOfElement;
        o << "pitch" << pitchOfElement;
        o << "duration" << durationOfElement;
    }
    return o.json();
}

void Toolkit::SetHumdrumBuffer(const char *data)
{
    this->ClearHumdrumBuffer();
    int size = (int)strlen(data) + 1;
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
#ifndef NO_HUMDRUM_SUPPORT
        // Convert from MEI to Humdrum
        MEIOutput meioutput(&m_doc);
        meioutput.SetScoreBasedMEI(true);
        std::string meidata = meioutput.Export();
        pugi::xml_document infile;
        infile.load_string(meidata.c_str());
        stringstream out;
        hum::Tool_mei2hum converter;

        this->LogRedirectStart();
        bool status = converter.convert(out, infile);
        this->LogRedirectStop();
        if (!status) {
            LogWarning("Problem converting MEI to Humdrum (see warning above this line for possible reasons");
        }

        this->SetHumdrumBuffer(out.str().c_str());
#endif
        if (m_humdrumBuffer) {
            return m_humdrumBuffer;
        }
        else {
            return "[empty]";
        }
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

void Toolkit::ClearHumdrumBuffer()
{
#ifndef NO_HUMDRUM_SUPPORT
    if (m_humdrumBuffer) {
        free(m_humdrumBuffer);
        m_humdrumBuffer = NULL;
    }
#endif
}

std::string Toolkit::ConvertMEIToHumdrum(const std::string &meiData)
{
#ifndef NO_HUMDRUM_SUPPORT
    hum::Tool_mei2hum converter;
    pugi::xml_document xmlfile;
    xmlfile.load_string(meiData.c_str());
    std::stringstream conversion;

    this->LogRedirectStart();
    bool status = converter.convert(conversion, xmlfile);
    this->LogRedirectStop();

    if (!status) {
        LogError("Error converting MEI data to Humdrum: %s", conversion.str().c_str());
    }
    this->SetHumdrumBuffer(conversion.str().c_str());
    return conversion.str();
#else
    return "";
#endif
}

std::string Toolkit::ConvertHumdrumToHumdrum(const std::string &humdrumData)
{
#ifndef NO_HUMDRUM_SUPPORT

    hum::HumdrumFileSet infiles;
    // bool result = infiles.readString(humdrumData);
    bool result = infiles.readString(humdrumData);
    if (!result) {
        this->SetHumdrumBuffer("");
        return "";
    }
    if (infiles.getCount() == 0) {
        this->SetHumdrumBuffer("");
        return "";
    }

    // Apply Humdrum tools if there are any filters in the file.
    hum::Tool_filter filter;
    for (int i = 0; i < infiles.getCount(); ++i) {
        if (infiles[i].hasGlobalFilters()) {
            filter.run(infiles[i]);
            if (filter.hasHumdrumText()) {
                infiles[i].readString(filter.getHumdrumText());
            }
            else {
                // should have auto updated itself in the filter.
            }
        }
    }

    // Apply Humdrum tools to the entire set if they are
    // at the universal level.
    if (infiles.hasUniversalFilters()) {
        filter.runUniversal(infiles);
        if (filter.hasHumdrumText()) {
            infiles.readString(filter.getHumdrumText());
        }
    }

    hum::HumdrumFile &infile = infiles[0];
    std::stringstream humout;
    humout << infile;
    this->SetHumdrumBuffer(humout.str().c_str());
    return humout.str();
#else
    return "";
#endif
}

std::string Toolkit::ConvertHumdrumToMIDI(const std::string &humdrumData)
{
#ifndef NO_HUMDRUM_SUPPORT
    return "TVRoZAAAAAYAAQAAAGRNVHJrAAAADQCQPHCBSJA8AAD/LwA=";
#else
    return "TVRoZAAAAAYAAQAAAGRNVHJrAAAADQCQPHCBSJA8AAD/LwA=";
#endif
}

void Toolkit::SetLocale()
{
    if (m_options->m_setLocale.GetValue() && !m_previousLocale) {
        // Required for proper formatting, e.g., in StringFormat (see vrv.cpp)
        m_previousLocale = std::locale::global(std::locale::classic());
    }
}

void Toolkit::ResetLocale()
{
    if (m_previousLocale) {
        std::locale::global(*m_previousLocale);
    }
}

void Toolkit::InitClock()
{
#ifndef NO_RUNTIME
    if (!m_runtimeClock) {
        m_runtimeClock = new RuntimeClock();
    }
#else
    LogError("Runtime clock is not supported in this build.");
#endif
}

void Toolkit::ResetClock()
{
#ifndef NO_RUNTIME
    if (m_runtimeClock) {
        m_runtimeClock->Reset();
    }
    else {
        LogWarning("No clock available. Please call 'InitClock' to create one.");
    }
#else
    LogError("Runtime clock is not supported in this build.");
#endif
}

double Toolkit::GetRuntimeInSeconds() const
{
#ifndef NO_RUNTIME
    if (m_runtimeClock) {
        return m_runtimeClock->GetSeconds();
    }
    else {
        LogWarning("No clock available. Please call 'InitClock' to create one.");
        return 0.0;
    }
#else
    LogError("Runtime clock is not supported in this build.");
    return 0.0;
#endif
}

void Toolkit::LogRuntime() const
{
#ifndef NO_RUNTIME
    if (m_runtimeClock) {
        double seconds = m_runtimeClock->GetSeconds();
        const int minutes = seconds / 60.0;
        if (minutes > 0) {
            seconds -= 60.0 * minutes;
            LogInfo("Total runtime is %d min %.3f s.", minutes, seconds);
        }
        else {
            LogInfo("Total runtime is %.3f s.", seconds);
        }
    }
    else {
        LogWarning("No clock available. Please call 'InitClock' to create one.");
    }
#else
    LogError("Runtime clock is not supported in this build.");
#endif
}

} // namespace vrv
