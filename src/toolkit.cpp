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
#include "iodarms.h"
#include "iomei.h"
#include "iomusxml.h"
#include "iopae.h"
#include "layer.h"
#include "measure.h"
#include "note.h"
#include "page.h"
#include "slur.h"
#include "style.h"
#include "svgdevicecontext.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

const char *UTF_16_BE_BOM = "\xFE\xFF";
const char *UTF_16_LE_BOM = "\xFF\xFE";

//----------------------------------------------------------------------------
// Toolkit
//----------------------------------------------------------------------------

Toolkit::Toolkit(bool initFont)
{

    m_scale = DEFAULT_SCALE;
    m_format = MEI;

    // default page size
    m_pageHeight = DEFAULT_PAGE_HEIGHT;
    m_pageWidth = DEFAULT_PAGE_WIDTH;
    m_border = DEFAULT_PAGE_LEFT_MAR;
    m_spacingLinear = DEFAULT_SPACING_LINEAR;
    m_spacingNonLinear = DEFAULT_SPACING_NON_LINEAR;
    m_spacingStaff = DEFAULT_SPACING_STAFF;
    m_spacingSystem = DEFAULT_SPACING_SYSTEM;

    m_noLayout = false;
    m_ignoreLayout = false;
    m_adjustPageHeight = false;
    m_noJustification = false;
    m_evenNoteSpacing = false;
    m_showBoundingBoxes = false;
    m_scoreBasedMei = false;

    m_cString = NULL;

    if (initFont) {
        Resources::InitFonts();
    }
}

Toolkit::~Toolkit()
{
    if (m_cString) {
        free(m_cString);
    }
}

bool Toolkit::SetResourcePath(const std::string &path)
{
    Resources::SetPath(path);
    return Resources::InitFonts();
};

bool Toolkit::SetBorder(int border)
{
    // We use left margin values because for now we cannot specify different values for each margin
    if (border < MIN_PAGE_LEFT_MAR || border > MAX_PAGE_LEFT_MAR) {
        LogError("Border out of bounds; default is %d, minimum is %d, and maximum is %d", DEFAULT_PAGE_LEFT_MAR,
            MIN_PAGE_LEFT_MAR, MAX_PAGE_LEFT_MAR);
        return false;
    }
    m_border = border;
    return true;
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

bool Toolkit::SetPageHeight(int h)
{
    if (h < MIN_PAGE_HEIGHT || h > MAX_PAGE_HEIGHT) {
        LogError("Page height out of bounds; default is %d, minimum is %d, and maximum is %d", DEFAULT_PAGE_HEIGHT,
            MIN_PAGE_HEIGHT, MAX_PAGE_HEIGHT);
        return false;
    }
    m_pageHeight = h;
    return true;
}

bool Toolkit::SetPageWidth(int w)
{
    if (w < MIN_PAGE_WIDTH || w > MAX_PAGE_WIDTH) {
        LogError("Page width out of bounds; default is %d, minimum is %d, and maximum is %d", DEFAULT_PAGE_WIDTH,
            MIN_PAGE_WIDTH, MAX_PAGE_WIDTH);
        return false;
    }
    m_pageWidth = w;
    return true;
};

bool Toolkit::SetSpacingStaff(int spacingStaff)
{
    if (spacingStaff < MIN_SPACING_STAFF || spacingStaff > MAX_SPACING_STAFF) {
        LogError("Spacing staff out of bounds; default is %d, minimum is %d, and maximum is %d", DEFAULT_SPACING_STAFF,
            MIN_SPACING_STAFF, MAX_SPACING_STAFF);
        return false;
    }
    m_spacingStaff = spacingStaff;
    return true;
}

bool Toolkit::SetSpacingSystem(int spacingSystem)
{
    if (spacingSystem < MIN_SPACING_SYSTEM || spacingSystem > MAX_SPACING_SYSTEM) {
        LogError("Spacing system out of bounds; default is %d, minimum is %d, and maximum is %d",
            DEFAULT_SPACING_SYSTEM, MIN_SPACING_SYSTEM, MAX_SPACING_SYSTEM);
        return false;
    }
    m_spacingSystem = spacingSystem;
    return true;
}

bool Toolkit::SetSpacingLinear(float spacingLinear)
{
    if (spacingLinear < MIN_SPACING_LINEAR || spacingLinear > MAX_SPACING_LINEAR) {
        LogError("Spacing (linear) out of bounds; default is %d, minimum is %d, and maximum is %d",
            DEFAULT_SPACING_LINEAR, MIN_SPACING_LINEAR, MAX_SPACING_LINEAR);
        return false;
    }
    m_spacingLinear = spacingLinear;
    return true;
}

bool Toolkit::SetSpacingNonLinear(float spacingNonLinear)
{
    if (spacingNonLinear < MIN_SPACING_NON_LINEAR || spacingNonLinear > MAX_SPACING_NON_LINEAR) {
        LogError("Spacing (non-linear) out of bounds; default is %d, minimum is %d, and maximum is %d",
            DEFAULT_SPACING_NON_LINEAR, MIN_SPACING_NON_LINEAR, MAX_SPACING_NON_LINEAR);
        return false;
    }
    m_spacingNonLinear = spacingNonLinear;
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
    else if (informat == "mei") {
        m_format = MEI;
    }
    else if (informat == "musicxml") {
        m_format = MUSICXML;
    }
    else {
        LogError("Input format can only be: mei, pae, xml or darms");
        return false;
    }
    return true;
};

bool Toolkit::SetFont(std::string const &font)
{
    return Resources::SetFont(font);
};

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

    // read the file into the string:
    std::string content(fileSize, 0);
    in.read(&content[0], fileSize);

    return LoadString(content);
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

    return LoadString(utf8line);
}

bool Toolkit::LoadString(const std::string &data)
{
    FileInputStream *input = NULL;
    if (m_format == PAE) {
        input = new PaeInput(&m_doc, "");
    }
    else if (m_format == DARMS) {
        input = new DarmsInput(&m_doc, "");
    }
    else if (m_format == MEI) {
        input = new MeiInput(&m_doc, "");
    }
    else if (m_format == MUSICXML) {
        input = new MusicXmlInput(&m_doc, "");
    }
    else {
        LogError("Unknown format");
        return false;
    }

    // something went wrong
    if (!input) {
        LogError("Unknown error");
        return false;
    }

    // ignore layout?
    if (m_ignoreLayout || m_noLayout) {
        input->IgnoreLayoutInformation();
    }

    // app xpath query?
    if (m_appXPathQuery.length() > 0) {
        input->SetAppXPathQuery(m_appXPathQuery);
    }

    // load the file
    if (!input->ImportString(data)) {
        LogError("Error importing data");
        delete input;
        return false;
    }

    m_doc.SetPageHeight(this->GetPageHeight());
    m_doc.SetPageWidth(this->GetPageWidth());
    m_doc.SetPageRightMar(this->GetBorder());
    m_doc.SetPageLeftMar(this->GetBorder());
    m_doc.SetPageTopMar(this->GetBorder());
    m_doc.SetSpacingLinear(this->GetSpacingLinear());
    m_doc.SetSpacingNonLinear(this->GetSpacingNonLinear());
    m_doc.SetSpacingStaff(this->GetSpacingStaff());
    m_doc.SetSpacingSystem(this->GetSpacingSystem());
    m_doc.SetEvenSpacing(this->GetEvenNoteSpacing());

    m_doc.PrepareDrawing();

    if (input->HasMeasureWithinEditoMarkup() && !m_noLayout) {
        LogWarning(
            "Only continous layout is possible with <measure> within editorial markup, switching to --no-layout");
        this->SetNoLayout(true);
    }

    // Do the layout? this depends on the options and the file. PAE and
    // DARMS have no layout information. MEI files _can_ have it, but it
    // might have been ignored because of the --ignore-layout option.
    // Regardless, we won't do layout if the --no-layout option was set.
    if (!input->HasLayoutInformation() && !m_noLayout) {
        // LogElapsedTimeStart();
        m_doc.CastOff();
        // LogElapsedTimeEnd("layout");
    }

    // disable justification if there's no layout or no justification
    if (m_noLayout || m_noJustification) {
        m_doc.SetJustificationX(false);
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

bool Toolkit::ParseOptions(const std::string &json_options)
{
#ifdef USE_EMSCRIPTEN

    jsonxx::Object json;

    // Read JSON options
    if (!json.parse(json_options)) {
        LogError("Can not parse JSON string.");
        return false;
    }

    if (json.has<jsonxx::String>("inputFormat")) SetFormat(json.get<jsonxx::String>("inputFormat"));

    if (json.has<jsonxx::Number>("scale")) SetScale(json.get<jsonxx::Number>("scale"));

    if (json.has<jsonxx::Number>("border")) SetBorder(json.get<jsonxx::Number>("border"));

    if (json.has<jsonxx::String>("font")) SetFont(json.get<jsonxx::String>("font"));

    if (json.has<jsonxx::Number>("pageWidth")) SetPageWidth(json.get<jsonxx::Number>("pageWidth"));

    if (json.has<jsonxx::Number>("pageHeight")) SetPageHeight(json.get<jsonxx::Number>("pageHeight"));

    if (json.has<jsonxx::Number>("spacingLinear")) SetSpacingLinear(json.get<jsonxx::Number>("spacingLinear"));

    if (json.has<jsonxx::Number>("spacingNonLinear")) SetSpacingNonLinear(json.get<jsonxx::Number>("spacingNonLinear"));

    if (json.has<jsonxx::Number>("spacingStaff")) SetSpacingStaff(json.get<jsonxx::Number>("spacingStaff"));

    if (json.has<jsonxx::Number>("spacingSystem")) SetSpacingSystem(json.get<jsonxx::Number>("spacingSystem"));

    if (json.has<jsonxx::String>("appXPathQuery")) SetAppXPathQuery(json.get<jsonxx::String>("appXPathQuery"));

    // Parse the various flags
    // Note: it seems that there is a bug with jsonxx and emscripten
    // Boolean value false won't be parsed properly. We have to use Number instead

    if (json.has<jsonxx::Number>("noLayout")) SetNoLayout(json.get<jsonxx::Number>("noLayout"));

    if (json.has<jsonxx::Number>("ignoreLayout")) SetIgnoreLayout(json.get<jsonxx::Number>("ignoreLayout"));

    if (json.has<jsonxx::Number>("adjustPageHeight")) SetAdjustPageHeight(json.get<jsonxx::Number>("adjustPageHeight"));

    if (json.has<jsonxx::Number>("noJustification")) SetNoJustification(json.get<jsonxx::Number>("noJustification"));

    if (json.has<jsonxx::Number>("showBoundingBoxes"))
        SetShowBoundingBoxes(json.get<jsonxx::Number>("showBoundingBoxes"));

    return true;

#else
    // The non-js version of the app should not use this function.
    return false;
#endif
}

std::string Toolkit::GetElementAttr(const std::string &xmlId)
{
#ifdef USE_EMSCRIPTEN
    jsonxx::Object o;

    if (!m_doc.GetDrawingPage()) return o.json();
    Object *element = m_doc.GetDrawingPage()->FindChildByUuid(xmlId);
    if (!element) {
        LogMessage("Element with id '%s' could not be found", xmlId.c_str());
        return o.json();
    }

    // Fill the attribute array (pair of string) by looking at attributes for all available MEI modules
    ArrayOfStrAttr attributes;
    element->GetAttributes(&attributes);

    // Fill the JSON object
    ArrayOfStrAttr::iterator iter;
    for (iter = attributes.begin(); iter != attributes.end(); iter++) {
        o << (*iter).first << (*iter).second;
        // LogMessage("Element %s - %s", (*iter).first.c_str(), (*iter).second.c_str());
    }
    return o.json();

#else
    // The non-js version of the app should not use this function.
    return "";
#endif
}

bool Toolkit::Edit(const std::string &json_editorAction)
{
#ifdef USE_EMSCRIPTEN

    jsonxx::Object json;

    // Read JSON actions
    if (!json.parse(json_editorAction)) {
        LogError("Can not parse JSON string.");
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

std::string Toolkit::GetLogString()
{
#ifdef USE_EMSCRIPTEN
    std::string str;
    std::vector<std::string>::iterator iter;
    for (iter = logBuffer.begin(); iter != logBuffer.end(); iter++) {
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
    m_doc.SetPageHeight(this->GetPageHeight());
    m_doc.SetPageWidth(this->GetPageWidth());
    m_doc.SetPageRightMar(this->GetBorder());
    m_doc.SetPageLeftMar(this->GetBorder());
    m_doc.SetPageTopMar(this->GetBorder());
    m_doc.SetSpacingStaff(this->GetSpacingStaff());
    m_doc.SetSpacingSystem(this->GetSpacingSystem());

    m_doc.UnCastOff();
    m_doc.CastOff();
}

std::string Toolkit::RenderToSvg(int pageNo, bool xml_declaration)
{
    // Page number is one-based - correct it to 0-based first
    pageNo--;

    // Get the current system for the SVG clipping size
    m_view.SetPage(pageNo);

    // Adjusting page width and height according to the options
    int width = m_pageWidth;
    if (m_noLayout) {
        width = m_doc.GetAdjustedDrawingPageWidth();
    }

    int height = m_pageHeight;
    if (m_adjustPageHeight || m_noLayout) {
        height = m_doc.GetAdjustedDrawingPageHeight();
    }

    // Create the SVG object, h & w come from the system
    // We will need to set the size of the page after having drawn it depending on the options
    SvgDeviceContext svg(width, height);

    // set scale and border from user options
    svg.SetUserScale((double)m_scale / 100, (double)m_scale / 100);

    // debug BB?
    svg.SetDrawBoundingBoxes(m_showBoundingBoxes);

    // render the page
    m_view.DrawCurrentPage(&svg, false);

    std::string out_str = svg.GetStringSVG(xml_declaration);
    return out_str;
}

bool Toolkit::RenderToSvgFile(const std::string &filename, int pageNo)
{
    std::string output = RenderToSvg(pageNo, true);

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

std::string Toolkit::RenderToMidi()
{
    MidiFile outputfile;
    outputfile.absoluteTicks();
    m_doc.ExportMIDI(&outputfile);
    outputfile.sortTracks();

    stringstream strstrem;
    outputfile.write(strstrem);
    std::string outputstr = Base64Encode(
        reinterpret_cast<const unsigned char *>(strstrem.str().c_str()), (unsigned int)strstrem.str().length());

    return outputstr;
}

std::string Toolkit::GetElementsAtTime(int millisec)
{
#ifdef USE_EMSCRIPTEN
    jsonxx::Object o;
    jsonxx::Array a;

    double time = (double)(millisec * 120 / 1000);
    AttNoteOnsetOffsetComparison matchTime(time);
    ArrayOfObjects notes;
    // Here we would need to check that the midi export is done
    if (m_doc.GetMidiExportDone()) {
        m_doc.FindAllChildByAttComparison(&notes, &matchTime);

        // Get the pageNo from the first note (if any)
        int pageNo = -1;
        if (notes.size() > 0) {
            Page *page = dynamic_cast<Page *>(notes.at(0)->GetFirstParent(PAGE));
            if (page) pageNo = page->GetIdx() + 1;
        }

        // Fill the JSON object
        ArrayOfObjects::iterator iter;
        for (iter = notes.begin(); iter != notes.end(); iter++) {
            a << (*iter)->GetUuid();
        }
        o << "notes" << a;
        o << "page" << pageNo;
    }
    return o.json();
#else
    // The non-js version of the app should not use this function.
    return "";
#endif
}

bool Toolkit::RenderToMidiFile(const std::string &filename)
{
    MidiFile outputfile;
    outputfile.absoluteTicks();
    m_doc.ExportMIDI(&outputfile);
    outputfile.sortTracks();
    outputfile.write(filename);

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

double Toolkit::GetTimeForElement(const std::string &xmlId)
{
    Object *element = m_doc.FindChildByUuid(xmlId);
    double timeofElement = 0.0;
    if (element->Is() == NOTE) {
        Note *note = dynamic_cast<Note *>(element);
        assert(note);
        timeofElement = note->m_playingOnset * 1000 / 120;
    }
    return timeofElement;
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

bool Toolkit::Drag(std::string elementId, int x, int y)
{
    if (!m_doc.GetDrawingPage()) return false;
    Object *element = m_doc.GetDrawingPage()->FindChildByUuid(elementId);
    if (element->Is() == NOTE) {
        Note *note = dynamic_cast<Note *>(element);
        assert(note);
        Layer *layer = dynamic_cast<Layer *>(note->GetFirstParent(LAYER));
        if (!layer) return false;
        int oct;
        data_PITCHNAME pname
            = (data_PITCHNAME)m_view.CalculatePitchCode(layer, m_view.ToLogicalY(y), note->GetDrawingX(), &oct);
        note->SetPname(pname);
        note->SetOct(oct);
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
        measure->AddFloatingElement(slur);
        m_doc.PrepareDrawing();
        return true;
    }
    return false;
}

bool Toolkit::Set(std::string elementId, std::string attrType, std::string attrValue)
{
    if (!m_doc.GetDrawingPage()) return false;
    Object *element = m_doc.GetDrawingPage()->FindChildByUuid(elementId);
    if (Att::SetCmn(element, attrType, attrValue)) return true;
    if (Att::SetCritapp(element, attrType, attrValue)) return true;
    if (Att::SetMensural(element, attrType, attrValue)) return true;
    if (Att::SetPagebased(element, attrType, attrValue)) return true;
    if (Att::SetShared(element, attrType, attrValue)) return true;
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

} // namespace vrv
