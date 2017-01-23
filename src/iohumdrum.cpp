/////////////////////////////////////////////////////////////////////////////
// Name:        iohumdrum.cpp
// Author:      Craig Stuart Sapp
// Created:     06/06/2016
// vim:         ts=4
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////
//
// References:
//    http://humlib.humdrum.org
//    http://music-encoding.org/support/tutorials/mei-1st
//    http://music-encoding.org/support/guidelines
//    http://music-encoding.org/documentation/2.1.1/elements
//    http://music-encoding.org/documentation/3.0.0/elements
//    http://music-encoding.org/documentation/2.1.1/atts
//    http://music-encoding.org/documentation/3.0.0/atts
//    http://pugixml.org/docs/manual.html
//

#include "iohumdrum.h"

#ifndef NO_HUMDRUM_SUPPORT

#include "humlib.h"
#include <math.h>

#endif /* NO_HUMDRUM_SUPPORT */

//----------------------------------------------------------------------------

#ifndef NO_HUMDRUM_SUPPORT

#include <algorithm>
#include <assert.h>
#include <cctype>
#include <regex>
#include <sstream>
#include <vector>

#endif /* NO_HUMDRUM_SUPPORT */

//----------------------------------------------------------------------------

#include "doc.h"

#ifndef NO_HUMDRUM_SUPPORT

#include "accid.h"
#include "artic.h"
#include "beam.h"
#include "chord.h"
#include "dir.h"
#include "dynam.h"
#include "editorial.h"
#include "ending.h"
#include "fermata.h"
#include "hairpin.h"
#include "harm.h"
#include "iomei.h"
#include "layer.h"
#include "measure.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "octave.h"
#include "page.h"
#include "pedal.h"
#include "rest.h"
#include "score.h"
#include "section.h"
#include "slur.h"
#include "space.h"
#include "staff.h"
#include "syl.h"
#include "system.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "tuplet.h"
#include "verse.h"
#include "vrv.h"
//#include "attcomparison.h"

using namespace hum; // humlib  namespace
using namespace vrv; // verovio namespace
using namespace pugi; // pugixml namespace
using namespace std;

#endif /* NO_HUMDRUM_SUPPORT */

namespace vrv {

#ifndef NO_HUMDRUM_SUPPORT

//----------------------------------------------------------------------------
// namespace for local IoHumdrum classes
//----------------------------------------------------------------------------

namespace humaux {

    /////////////////////////////////////////////////////////////////////

    HumdrumTie::HumdrumTie(void)
    {
        m_endmeasure = m_startmeasure = NULL;
        m_inserted = false;
        m_pitch = 0;
        m_layer = -1;
    }

    HumdrumTie::HumdrumTie(const HumdrumTie &anothertie)
    {
        m_starttoken = anothertie.m_starttoken;
        m_endtoken = anothertie.m_endtoken;
        m_starttime = anothertie.m_starttime;
        m_endtime = anothertie.m_endtime;
        m_inserted = anothertie.m_inserted;
        m_startid = anothertie.m_startid;
        m_endid = anothertie.m_endid;
        m_startmeasure = anothertie.m_startmeasure;
        m_endmeasure = anothertie.m_endmeasure;
        m_pitch = anothertie.m_pitch;
        m_layer = anothertie.m_layer;
    }

    HumdrumTie::~HumdrumTie(void) { clear(); }

    HumdrumTie &HumdrumTie::operator=(const HumdrumTie &anothertie)
    {
        if (this == &anothertie) {
            return *this;
        }
        m_starttoken = anothertie.m_starttoken;
        m_endtoken = anothertie.m_endtoken;
        m_starttime = anothertie.m_starttime;
        m_endtime = anothertie.m_endtime;
        m_inserted = anothertie.m_inserted;
        m_startid = anothertie.m_startid;
        m_endid = anothertie.m_endid;
        m_startmeasure = anothertie.m_startmeasure;
        m_endmeasure = anothertie.m_endmeasure;
        m_pitch = anothertie.m_pitch;
        m_layer = anothertie.m_layer;
        return *this;
    }

    void HumdrumTie::clear(void)
    {
        m_endmeasure = m_startmeasure = NULL;
        m_inserted = false;
        m_startid.clear();
        m_endid.clear();
    }

    void HumdrumTie::insertTieIntoDom(void)
    {
        if (m_inserted) {
            // don't insert again
            return;
        }
        if ((m_startmeasure == NULL) && (m_endmeasure == NULL)) {
            // What are you trying to do?
            return;
        }
        if (m_startmeasure == NULL) {
            // This is a tie with no start.  Don't know what to do with this
            // for now (but is possible due to repeated music).
            return;
        }
        if (m_endmeasure == NULL) {
            // This is a tie with no end.  Don't know what to do with this
            // for now (but is possible due to repeated music).
            return;
        }

        vrv::Tie *tie = new vrv::Tie;
        tie->SetStartid("#" + m_startid);
        tie->SetEndid("#" + m_endid);

        bool samemeasure = false;
        if (m_startmeasure == m_endmeasure) {
            samemeasure = true;
        }

        if (samemeasure) {
            m_startmeasure->AddChild(tie);
            m_inserted = true;
        }
        else {
            // The tie starts in one measure and goes to another.
            // Probably handled the same as when in the same measure.
            m_startmeasure->AddChild(tie);
            m_inserted = true;
        }
    }

    void HumdrumTie::setStart(const string &id, Measure *starting, int layer, const string &token, int pitch,
        HumNum starttime, HumNum endtime)
    {
        m_starttoken = token;
        m_starttime = starttime;
        m_endtime = endtime;
        m_pitch = pitch;
        m_layer = layer;
        m_startmeasure = starting;
        m_startid = id;
    }

    void HumdrumTie::setEnd(const string &id, Measure *ending, const string &token)
    {
        m_endtoken = token;
        m_endmeasure = ending;
        m_endid = id;
    }

    void HumdrumTie::setEndAndInsert(const string &id, Measure *ending, const string &token)
    {
        setEnd(id, ending, token);
        insertTieIntoDom();
    }

    bool HumdrumTie::isInserted(void) { return m_inserted; }

    int HumdrumTie::getPitch(void) { return m_pitch; }

    int HumdrumTie::getLayer(void) { return m_layer; }

    HumNum HumdrumTie::getStartTime(void) { return m_starttime; }

    HumNum HumdrumTie::getEndTime(void) { return m_endtime; }

    HumNum HumdrumTie::getDuration(void) { return m_endtime - m_starttime; }

    string HumdrumTie::getStartToken(void) { return m_starttoken; }

    string HumdrumTie::getEndToken(void) { return m_endtoken; }

    /////////////////////////////////////////////////////////////////////

    StaffStateVariables::StaffStateVariables(void) { clear(); }
    StaffStateVariables::~StaffStateVariables() { clear(); }
    void StaffStateVariables::clear(void)
    {
        verse = false;
        suppress_beam_tuplet = false;
        suppress_bracket_tuplet = false;
        ottavanotestart = ottavanoteend = NULL;
        ottavameasure = NULL;
        ties.clear();
        meter_bottom = 4;
    }

} // end namespace humaux

#endif /* NO_HUMDRUM_SUPPORT */

//----------------------------------------------------------------------------
// HumdrumInput
//----------------------------------------------------------------------------

//////////////////////////////
//
// HumdrumInput::HumdrumInput -- Constructor.
//

HumdrumInput::HumdrumInput(Doc *doc, std::string filename) : FileInputStream(doc)
{

#ifndef NO_HUMDRUM_SUPPORT
    m_filename = filename;
#endif /* NO_HUMDRUM_SUPPORT */
}

//////////////////////////////
//
// HumdrumInput::~HumdrumInput -- Deconstructor.
//

HumdrumInput::~HumdrumInput()
{

#ifndef NO_HUMDRUM_SUPPORT
    clear();
#endif /* NO_HUMDRUM_SUPPORT */
}

//////////////////////////////
//
// HumdrumInput::ImportFile -- Read a Humdrum file from a file.
//

bool HumdrumInput::ImportFile()
{

#ifndef NO_HUMDRUM_SUPPORT
    try {
        m_doc->Reset();
        HumdrumFile &infile = m_infile;
        bool result = infile.read(m_filename);
        if (!result) {
            return false;
        }

        return convertHumdrum();
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }
#else
    return true;
#endif /* NO_HUMDRUM_SUPPORT */
}

//////////////////////////////
//
// HumdrumInput::ImportString -- Read a Humdrum file from a text string.
//

bool HumdrumInput::ImportString(std::string const &content)
{

#ifndef NO_HUMDRUM_SUPPORT
    try {
        m_doc->Reset();
        HumdrumFile &infile = m_infile;

        // Auto-detect CSV Humdrum file. Maybe later move to the humlib parser.
        string exinterp;
        bool found = false;
        int comma = 0;
        int tab = 0;
        for (int i = 0; i < (int)content.size() - 3; i++) {
            if (((content[i] == '\n') || (content[i] == 0x0d)) && (content[i + 1] == '*') && (content[i + 2] == '*')) {
                found = true;
                i += 2;
                exinterp = "**";
                continue;
            }
            else if ((i == 0) && (content[i] == '*') && (content[i + 1] == '*')) {
                found = true;
                i += 2;
                exinterp = "**";
                continue;
            }

            if (!found) {
                continue;
            }
            if (content[i] == 0x0a) {
                break;
            }
            exinterp.push_back(content[i]);
            if (content[i] == '\t') {
                tab++;
            }
            if (content[i] == ',') {
                comma++;
            }
        }

        bool result;
        if (comma < tab) {
            result = infile.readString(content);
        }
        else {
            result = infile.readStringCsv(content);
        }

        if (!result) {
            return false;
        }
        return convertHumdrum();
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }

#else
    return true;
#endif /* NO_HUMDRUM_SUPPORT */
}

///////////////////////////////////////////////////////////////////////////
//
// Protected functions.
//

#ifndef NO_HUMDRUM_SUPPORT

//////////////////////////////
//
// HumdrumInput::GetHumdrumString -- direct Humdrum output before
//    convesion.
//

string HumdrumInput::GetHumdrumString(void)
{
    stringstream tempout;
    tempout << m_infile;
    return tempout.str();
}

//////////////////////////////
//
// HumdrumInput::convertHumdrum -- Top level method called from ImportFile or
//     ImportString.  Convert a HumdrumFile structure into an MEI
//     structure.  Returns false if there was an error in the conversion
//     process.
//
// Reference:
//     http://music-encoding.org/documentation/2.1.1/cmn
//

bool HumdrumInput::convertHumdrum(void)
{
    HumdrumFile &infile = m_infile;

    if (GetOutputFormat() == "humdrum") {
        // allow for filtering in toolkit.
        return true;
    }

    // apply Humdrum tools if there are any filters in the file.
    if (infile.hasFilters()) {
        Tool_filter filter;
        filter.run(infile);
        if (filter.hasHumdrumText()) {
            infile.readString(filter.getHumdrumText());
        }
        else {
            // humdrum structure not always correct in output from tools
            // yet, so reload.
            stringstream tempdata;
            tempdata << infile;
            infile.readString(tempdata.str());
        }
    }

    m_multirest = analyzeMultiRest(infile);

    infile.analyzeKernSlurs();
    parseSignifiers(infile);

    bool status = true; // for keeping track of problems in conversion process.

    prepareTimeSigDur();
    setupMeiDocument();

    // Create a list of the parts and which spine represents them.
    vector<HTp> &kernstarts = m_kernstarts;
    kernstarts = infile.getKernSpineStartList();

    vector<HTp> spinestarts;
    infile.getSpineStartList(spinestarts);
    for (auto it : spinestarts) {
        if (it->isDataType("**mxhm")) {
            m_harm = true;
        }
    }

    if (kernstarts.size() == 0) {
        // no parts in file, give up.  Perhaps return an error.
        return status;
    }

    // Reverse the order, since top part is last spine.
    reverse(kernstarts.begin(), kernstarts.end());
    calculateReverseKernIndex();

    m_staffstates.resize(kernstarts.size());

    prepareVerses();
    prepareEndings();
    prepareStaffGroup();

    // m_meausreIndex not currently used but might be useful sometime.
    m_measureIndex = 0;
    int line = kernstarts[0]->getLineIndex();
    while (line < infile.getLineCount() - 1 && (line >= 0)) {
        m_measureIndex++;
        status &= convertSystemMeasure(line);
    }

    createHeader();

    // calculateLayout();

    m_doc->ConvertToPageBasedDoc();

    if (m_debug) {
        cout << GetMeiString();
    }

    // If the document has <pb/> and <sb/> elements you can call:
    //    m_doc->CastOffEncodingDoc();
    // which will cast off the document according to these breaks.
    // Adding <pb/> and <sb/> are done with
    // Pb pb = new Pb();
    // section->AddChild(pb);

    return status;
}

//////////////////////////////
//
// HumdrumInput::createHeader --
//
// References:
//      http://music-encoding.org/wp-content/uploads/2015/03/CheatSheetHeader.pdf
//      http://music-encoding.org/support/tutorials/mei-1st/exploring-the-mei-header
//
// <meiHead>
//     <altId>  . . . . . . . . . . Alternate bibliographic identifier.
//     <fileDesc> . . . . . . . . . Full bibliographic description of file.
//        <titleStmt> (required) == Title and responsibility container.
//           <title>  (required) == Title of bibliographic entry.
//           <respStmt>          == Names of those repsonsible for
// intellectual/artistic content.
//        <pubStmt>   (required) == Pub. name, address, date and related info.
//
//        <sourceDesc>           == Sources used to create electronic file.
//           <source n="1">
//              <identifier> <titleStmt> <editionStmt> <putStmt> <physDesc>
//              <seriesStmt> <noteStmt> <history> <langUsage> <key> <tempo>
//              <meter> <perfMedium> <classification> <contents> <relatedItem>
//
//      <encodingDesc>  . . . . . . Relation of file to sources and how it was
// created.
//         <appInfo>? <editorialDecl>?
//
//     <workDesc>  . . . . . . . . Groupling for non-bibliograhpic aspects of
// text.
//        <identifier> <titleStmt> <history> <langUsage> <key> <tempo> <meter>
//        <perfMedium> <notesStmt> <classification> <castList> <incipit>
//        <contents> <relatedItem>
//
//     <revisionDesc>  . . . . . . Alterations to the file.
//        <change>
//

void HumdrumInput::createHeader(void)
{
    HumdrumFile &infile = m_infile;
    vector<HumdrumLine *> references = infile.getReferenceRecords();
    vector<vector<string> > respPeople;
    getRespPeople(respPeople, references);

    // <fileDesc> /////////////
    pugi::xml_node fileDesc = m_doc->m_header.append_child("fileDesc");
    pugi::xml_node titleStmt = fileDesc.append_child("titleStmt");

    insertTitle(titleStmt, references);
    if (respPeople.size() > 0) {
        insertRespStmt(titleStmt, respPeople);
    }

    pugi::xml_node pubStmt = fileDesc.append_child("pubStmt");
    pugi::xml_node date = pubStmt.append_child("date");

    // <encodingDesc> /////////

    // <workDesc> /////////////

    // <encodingDesc> /////////
    pugi::xml_node encodingDesc = m_doc->m_header.append_child("encodingDesc");
    pugi::xml_node projectDesc = encodingDesc.append_child("projectDesc");

    pugi::xml_node p1 = projectDesc.append_child("p");
    p1.append_child(pugi::node_pcdata)
        .set_value(StringFormat("Transcoded from Humdrum with Verovio version %s", GetVersion().c_str()).c_str());
    string ENC = getReferenceValue("ENC", references);
    if (ENC.size()) {
        ENC = "Encoded by: " + ENC;
        pugi::xml_node p2 = projectDesc.append_child("p");
        p2.append_child(pugi::node_pcdata).set_value(ENC.c_str());
    }

    string datestr = getDateString();
    date.append_child(pugi::node_pcdata).set_value(datestr.c_str());

    if (references.size() > 0) {
        insertExtMeta(references);
    }
}

//////////////////////////////
//
// HumdrumInput::getReferenceValue --
//

string HumdrumInput::getReferenceValue(const string &key, vector<HumdrumLine *> &references)
{
    for (int i = 0; i < (int)references.size(); i++) {
        if (key == references[i]->getReferenceKey()) {
            return references[i]->getReferenceValue();
        }
    }
    string empty;
    return empty;
}

//////////////////////////////
//
// HumdrumInput::getDateSting -- Return the current time and date as a string.
//

string HumdrumInput::getDateString(void)
{
    time_t t = time(0); // get time now
    struct tm *now = localtime(&t);
    string dateStr = StringFormat("%d-%02d-%02d %02d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
        now->tm_hour, now->tm_min, now->tm_sec);
    return dateStr;
}

//////////////////////////////
//
// HumdrumInput::insertRespStmt -- Print a list of composer and/or lyricist.
//

void HumdrumInput::insertRespStmt(pugi::xml_node &titleStmt, vector<vector<string> > &respPeople)
{
    if (respPeople.size() == 0) {
        return;
    }
    pugi::xml_node respStmt = titleStmt.append_child("respStmt");
    for (int i = 0; i < (int)respPeople.size(); i++) {
        pugi::xml_node person = respStmt.append_child("persName");
        person.append_attribute("role") = unescapeHtmlEntities(respPeople[i][1]).c_str();
        person.text().set(unescapeHtmlEntities(respPeople[i][0]).c_str());
    }
}

//////////////////////////////
//
// HumdrumInput::getRespPeople -- Get the respStmnt people, such as the composer
// and/or lyricist.
//
// Roles (4th parameter in addPerson(), is free-form, but should use the roles
// are listed in these two webpages:
//    http://www.loc.gov/marc/relators/relacode.html
//       list of three-letter relator codes
//    http://www.loc.gov/marc/relators/relaterm.html
//       short descriptions of relator codes
//

void HumdrumInput::getRespPeople(vector<vector<string> > &respPeople, vector<HumdrumLine *> &references)
{

    // precalculate a reference map here to make more O(N) rather than O(N^2)
    addPerson(respPeople, references, "COM", "Composer"); // cmp
    addPerson(respPeople, references, "COA", "attributed composer");
    addPerson(respPeople, references, "COS", "suspected composer");
    addPerson(respPeople, references, "LYR", "Lyricist"); // lyr
    addPerson(respPeople, references, "LIB", "Librettist"); // lbt
    addPerson(respPeople, references, "LAR", "Arranger"); // arr
    addPerson(respPeople, references, "LOR", "Adapter"); // orchestrator, adp
    addPerson(respPeople, references, "RPN", "Producer"); // pro
    addPerson(respPeople, references, "MPN", "Performer"); // prf, also: Singer/Instrumentalist
    addPerson(respPeople, references, "MCN", "Conductor"); // cnd
    addPerson(respPeople, references, "ODE", "Dedicatee"); // dte
    addPerson(respPeople, references, "OCO", "Patron"); // commissioner, pat
    addPerson(respPeople, references, "OCL", "Collector"); // col
    addPerson(respPeople, references, "EED", "digital editor");
    // ENC added to encodingDesc
    // addPerson(respPeople, references, "ENC", "encoder");           // mrk,
    // Markup editor
}

//////////////////////////////
//
// HumdrumInput::addPerson --
//

void HumdrumInput::addPerson(
    vector<vector<string> > &respPeople, vector<HumdrumLine *> &references, const string &key, const string &role)
{
    for (int i = 0; i < (int)references.size(); i++) {
        if (references[i]->getReferenceKey() == key) {
            respPeople.resize(respPeople.size() + 1);
            respPeople.back().resize(2);
            respPeople.back()[0] = references[i]->getReferenceValue();
            respPeople.back()[1] = role;
        }
    }
}

//////////////////////////////
//
// HumdrumInput::insertExtMeta -- Print Humdrum reference records as XML into
//     <extMeta> element.
//

void HumdrumInput::insertExtMeta(vector<HumdrumLine *> &references)
{
    stringstream xmldata;
    xmldata << "<extMeta>\n";
    xmldata << "\t<frames xmlns:humxml=\"http://www.humdrum.org/ns/humxml\">\n";
    for (int i = 0; i < (int)references.size(); i++) {
        references[i]->printXml(xmldata, 4);
    }
    xmldata << "\t</frames>\n";
    xmldata << "</extMeta>\n";

    pugi::xml_document tmpdoc;
    pugi::xml_parse_result result = tmpdoc.load(xmldata.str().c_str());
    if (!result) {
        // some sort of error, so give up;
        cerr << "ExtMeta parse error: " << result.description() << endl;
        cerr << xmldata.str();
        return;
    }

    m_doc->m_header.append_copy(tmpdoc.document_element());
}

//////////////////////////////
//
// HumdrumInput::insertTitle --
//

void HumdrumInput::insertTitle(pugi::xml_node &titleStmt, const vector<HumdrumLine *> &references)
{
    string key;
    string value;
    bool lang;
    bool plang;
    string language;

    int titlecount = 0;

    for (int i = 0; i < (int)references.size(); i++) {
        plang = false;
        lang = false;
        key = references[i]->getReferenceKey();
        if (key.compare(0, 3, "OTL") != 0) {
            continue;
        }
        value = references[i]->getReferenceValue();
        if (value.empty()) {
            continue;
        }
        auto loc = key.find("@");
        if (loc != string::npos) {
            lang = true;
            if (key.find("@@") != string::npos) {
                plang = true;
                language = key.substr(loc + 2);
                if (language.empty()) {
                    plang = false;
                    lang = false;
                }
            }
            else {
                language = key.substr(loc + 1);
                if (language.empty()) {
                    plang = false;
                    lang = false;
                }
            }
        }

        for (int j = 0; j < (int)language.size(); j++) {
            if (language[j] == '-') {
                // don't force to lower case after first dash
                // as BCP 47 country codes are in upper case, and
                // variant codes usually start capitalized.
                break;
            }
            language[j] = std::tolower(language[j]);
        }

        pugi::xml_node title = titleStmt.append_child("title");
        titlecount++;
        title.text().set(unescapeHtmlEntities(value).c_str());
        if (lang) {
            title.append_attribute("xml:lang") = language.c_str();
            if (plang) {
                title.append_attribute("type") = "main";
            }
            else {
                title.append_attribute("type") = "translated";
            }
        }
    }

    if (!titlecount) {
        // Put in a required empty <title/> tag:
        titleStmt.append_child("title");
    }
}

//////////////////////////////
//
// HumdrumInput::prepareVerses -- Assumes that m_kernstarts has been
//      filled already.
//

void HumdrumInput::prepareVerses(void)
{
    int i, j;
    vector<humaux::StaffStateVariables> &ss = m_staffstates;
    // ss[*].verse should already be set to false.

    vector<HTp> &kern = m_kernstarts;

    if (kern.size() == 0) {
        return;
    }

    HumdrumLine &line = *kern[0]->getLine();
    int field;

    for (i = 0; i < (int)kern.size(); i++) {
        field = kern[i]->getFieldIndex();
        for (j = field + 1; j < line.getFieldCount(); j++) {
            if (line.token(j)->isKern()) {
                break;
            }
            else if (line.token(j)->isDataType("**text")) {
                ss[i].verse = true;
            }
            else if (line.token(j)->isDataType("**silbe")) {
                ss[i].verse = true;
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::prepareTimeSigDur -- create a list of the durations of time
//      signatures in the file, indexed by HumdrumFile line number.  Only the
//      first spine in the file is considered.
//

void HumdrumInput::prepareTimeSigDur(void)
{
    vector<HumNum> &sigdurs = m_timesigdurs;
    HumdrumFile &infile = m_infile;
    vector<HTp> spinestarts;

    sigdurs.resize(infile.getLineCount());
    std::fill(sigdurs.begin(), sigdurs.end(), -1);

    infile.getKernSpineStartList(spinestarts);
    HTp kernspine = NULL;
    if (spinestarts.size() == 0) {
        infile.getSpineStartList(spinestarts, "**recip");
        if (spinestarts.size() == 0) {
            // no **kern or **recip so give up
            return;
        }
        else {
            kernspine = spinestarts[0];
        }
    }
    else {
        kernspine = spinestarts[0];
    }
    if (kernspine == NULL) {
        return;
    }

    HumNum curdur = -1;
    int top;
    int bot;
    int bot2;
    int line;

    kernspine = kernspine->getNextToken();
    while (kernspine) {
        line = kernspine->getLineIndex();
        if (!kernspine->isInterpretation()) {
            sigdurs[line] = curdur;
            kernspine = kernspine->getNextToken();
            continue;
        }

        if (sscanf(kernspine->c_str(), "*M%d/%d%%%d", &top, &bot, &bot2) == 3) {
            // deal with triplet-whole note beats later
        }
        else if (sscanf(kernspine->c_str(), "*M%d/%d", &top, &bot) == 2) {
            curdur = top;
            if (bot == 0) { // breve
                curdur *= 2;
            }
            else {
                curdur /= bot;
            }
            curdur *= 4; // convert to quarter note units;
        }
        sigdurs[line] = curdur;
        kernspine = kernspine->getNextToken();
    }

    sigdurs.back() = curdur;
    for (int i = (int)sigdurs.size() - 2; i >= 0; i--) {
        if (infile[i].getDuration() == 0) {
            sigdurs[i] = sigdurs[i + 1];
        }
    }
}

//////////////////////////////
//
// HumdrumInput::calculateReverseKernIndex --
//

void HumdrumInput::calculateReverseKernIndex(void)
{
    vector<int> &rkern = m_rkern;
    HumdrumFile &infile = m_infile;
    const vector<HTp> &kernstarts = m_kernstarts;

    rkern.resize(infile.getSpineCount() + 1);
    std::fill(rkern.begin(), rkern.end(), -1);
    for (int i = 0; i < (int)kernstarts.size(); i++) {
        rkern[kernstarts[i]->getTrack()] = i;
    }
}

//////////////////////////////
//
// HumdrumInput::prepareStaffGroup --  Add information about each part.
//

void HumdrumInput::prepareStaffGroup(void)
{
    const vector<HTp> &kernstarts = m_kernstarts;

    m_staffgroup = new StaffGrp();
    m_doc->m_scoreDef.AddChild(m_staffgroup);
    for (int i = 0; i < (int)kernstarts.size(); i++) {
        m_staffdef.push_back(new StaffDef());
        m_staffgroup->AddChild(m_staffdef.back());
        fillPartInfo(kernstarts[i], i + 1);
    }
    if (kernstarts.size() > 0) {
        addMidiTempo(m_doc->m_scoreDef, kernstarts[0]);
    }
    if (kernstarts.size() == 2) {
        m_staffgroup->SetSymbol(staffgroupingsym_SYMBOL_brace);
        m_staffgroup->SetBarthru(BOOLEAN_true);
    }
    else if (kernstarts.size() > 2) {
        m_staffgroup->SetSymbol(staffgroupingsym_SYMBOL_bracket);
    }
}

//////////////////////////////
//
// HumdrumInput::addMidiTempo --
//

void HumdrumInput::addMidiTempo(ScoreDef &m_scoreDef, HTp kernpart)
{
    while (kernpart != NULL) {
        if (kernpart->isData()) {
            break;
            ;
        }
        if (!kernpart->isInterpretation()) {
            kernpart = kernpart->getNextToken();
            continue;
        }
        if (kernpart->compare(0, 3, "*MM") == 0) {
            if (kernpart->size() > 3) {
                if (::isdigit((*kernpart)[3])) {
                    int tempo = stoi(kernpart->substr(3));
                    // string tempostr = to_string(tempo);
                    m_scoreDef.SetMidiBpm(tempo);
                }
            }
            break;
        }
        kernpart = kernpart->getNextToken();
    }
}

//////////////////////////////
//
// HumdrumInput::fillPartInfo -- Should use regular expressions
//    in the future.
//

void HumdrumInput::fillPartInfo(HTp partstart, int partnumber)
{
    vector<humaux::StaffStateVariables> &ss = m_staffstates;

    string label;
    string abbreviation;
    string clef;
    string keysig;
    string key;
    string timesig;
    string metersig;
    int top = 0;
    int bot = 0;
    pair<int, HTp> oclef;

    HTp part = partstart;
    while (part && !part->getLine()->isData()) {
        if (part->compare(0, 5, "*clef") == 0) {
            clef = *part;
        }
        else if (part->compare(0, 6, "*oclef") == 0) {
            m_oclef.emplace_back(partnumber, part);
        }
        else if (part->compare(0, 3, "*k[") == 0) {
            keysig = *part;
        }
        else if (part->compare(0, 3, "*I'") == 0) {
            abbreviation = part->substr(3);
        }
        else if (part->compare(0, 3, "*I\"") == 0) {
            label = part->substr(3);
        }
        else if (part->compare(0, 5, "*met(") == 0) {
            auto ploc = part->rfind(")");
            if (ploc != string::npos) {
                metersig = part->substr(5, ploc - 5);
            }
        }
        else if (sscanf(part->c_str(), "*M%d/%d", &top, &bot) == 2) {
            timesig = *part;
            ss[partnumber - 1].meter_bottom = bot;
        }
        part = part->getNextToken();
    }

    m_staffdef.back()->SetN(partnumber);
    m_staffdef.back()->SetLines(5);

    if (clef.size() > 0) {
        setClef(m_staffdef.back(), clef);
    }
    else {
        string autoclef = getAutoClef(partstart, partnumber);
        setClef(m_staffdef.back(), autoclef);
    }

    if (label.size() > 0) {
        m_staffdef.back()->SetLabel(label);
    }

    if (abbreviation.size() > 0) {
        m_staffdef.back()->SetLabelAbbr(abbreviation);
    }

    if (keysig.size() > 0) {
        setKeySig(m_staffdef.back(), keysig);
    }

    if (timesig.size() > 0) {
        setTimeSig(m_staffdef.back(), timesig);
    }

    if (metersig.size() > 0) {
        setMeterSymbol(m_staffdef.back(), metersig);
    }

    addInstrumentDefinition(m_staffdef.back(), partstart);
}

//////////////////////////////
//
// HumdrumInput::getAutoClef -- extimate a clef for a part
//     which does not have a specified clef.  Choice will be
//     treble or bass.
//

string HumdrumInput::getAutoClef(HTp partstart, int partnumber)
{
    HTp tok = partstart;
    int ptrack = partstart->getTrack();
    vector<int> dhist(100, 0);
    int diatonic;
    while (tok) {
        if (tok->isInterpretation()) {
            if (tok->compare(0, 5, "*clef") == 0) {
                break;
            }
        }
        if (!tok->isData()) {
            tok = tok->getNextToken();
            continue;
        }
        if (!(tok->isNull() || tok->isRest())) {
            diatonic = Convert::kernToBase7(tok);
            if ((diatonic > 0) && (diatonic < 100)) {
                dhist[diatonic]++;
            }
        }
        HTp ftok = tok->getNextFieldToken();
        while (ftok && (ptrack == ftok->getTrack())) {
            if (!(ftok->isNull() || ftok->isRest())) {
                diatonic = Convert::kernToBase7(ftok);
                if ((diatonic > 0) && (diatonic < 100)) {
                    dhist[diatonic]++;
                }
            }
            ftok = ftok->getNextFieldToken();
        }

        tok = tok->getNextToken();
    }

    int low = 100;
    int high = 0;
    for (int i = 0; i < 100; i++) {
        if (dhist[i]) {
            low = i;
            break;
        }
    }
    for (int i = 99; i >= 0; i--) {
        if (dhist[i]) {
            high = i;
            break;
        }
    }
    if (high < low) {
        return "";
    }
    int E3 = Convert::kernToBase7("E");
    int G4 = Convert::kernToBase7("g");
    if ((low > E3) && (high > G4)) {
        return "clefG2";
    }
    if ((low < E3) && (high < G4)) {
        return "clefF4";
    }
    // calculate weighted mean
    int wsum = 0;
    for (int i = low; i <= high; i++) {
        wsum += dhist[i];
    }
    if (wsum == 0) {
        // no pitches...
        return "";
    }
    int topsum = 0;
    for (int i = low; i <= high; i++) {
        topsum += i * wsum;
    }
    int C4 = Convert::kernToBase7("c");
    double wm = (double)topsum / (double)wsum;
    if (wm < C4) {
        return "clefF4";
    }
    else {
        return "clefG2";
    }
}

//////////////////////////////
//
// HumdrumInput::addInstrumentDefinition --
//
// <staffDef>
//    <instrDef @midi.channel @midi.instrnum @midi.instrname>
//

void HumdrumInput::addInstrumentDefinition(StaffDef *staffdef, HTp partstart)
{

    HTp instcode = NULL;
    while (partstart != NULL) {
        if (partstart->isData()) {
            break;
            ;
        }
        if (!partstart->isInterpretation()) {
            partstart = partstart->getNextToken();
            continue;
        }
        if (partstart->compare(0, 2, "*I") == 0) {
            if (partstart->size() < 2) {
                partstart = partstart->getNextToken();
                continue;
            }
            if (!::islower((*partstart)[2])) {
                // instrument class, name, abbrevation or similar
                partstart = partstart->getNextToken();
                continue;
            }
            instcode = partstart;
            break;
        }
        partstart = partstart->getNextToken();
    }

    if (instcode == NULL) {
        return;
    }

    // InstrDef* idef = new InstrDef;
    // staffdef->AddInstrDef(idef);

    // Allowing users to assign MIDI instrument numbers in data would be useful.
    //	static HumInstrument imap;
    //	int gmpc = imap.getGM(*instcode);

    //   m_staffdef.push_back(new StaffDef());
    //   m_staffgroup->AddStaffDef(m_staffdef.back());

    //   gmpc is -1 if no mapping (probably assign to 0 in those cases).
    //   if (instcode) {
    //   std::cerr << "GOT HERE " << *instcode << " " << gmpc << std::endl;
    //   }
}

//////////////////////////////
//
// HumdrumInput::setMeterSymbol -- common time or cut time.
//    (no other mensurations for now).
//

void HumdrumInput::setMeterSymbol(StaffDef *part, const string &metersig)
{
    if (metersig == "C") {
        // This is used more strictly for C mensuration.
        part->SetMeterSym(METERSIGN_common);
    }
    else if (metersig == "c") {
        part->SetMeterSym(METERSIGN_common);
    }
    else if (metersig == "c|") {
        part->SetMeterSym(METERSIGN_cut);
    }
    else if (metersig == "C|") {
        // This is used more strictly for Cut-C mensuration.
        part->SetMeterSym(METERSIGN_cut);
    }
}

//////////////////////////////
//
// HumdrumInput::setTimeSig -- Convert a Humdrum timesig to an MEI timesig.
//

void HumdrumInput::setTimeSig(StaffDef *part, const string &timesig)
{
    int top = -1000;
    int bot = -1000;
    int bot2 = -1000;
    if (sscanf(timesig.c_str(), "*M%d/%d%%%d", &top, &bot, &bot2) == 3) {
        // Such as three-triplet whole notes in a 2/1 measure
        // deal with this later
    }
    else if (sscanf(timesig.c_str(), "*M%d/%d", &top, &bot) == 2) {
        part->SetMeterCount(top);
        part->SetMeterUnit(bot);
    }
    else {
        // some strange time signature which should never occur.
    }
}

//////////////////////////////
//
// HumdrumInput::setKeySig -- Convert a Humdrum keysig to an MEI keysig.
//

template <class ELEMENT> void HumdrumInput::setKeySig(ELEMENT element, const string &keysig)
{
    bool fs = keysig.find("f#") != string::npos;
    bool cs = keysig.find("c#") != string::npos;
    bool gs = keysig.find("g#") != string::npos;
    bool ds = keysig.find("d#") != string::npos;
    bool as = keysig.find("a#") != string::npos;
    bool es = keysig.find("e#") != string::npos;
    bool bs = keysig.find("b#") != string::npos;

    bool bb = keysig.find("b-") != string::npos;
    bool eb = keysig.find("e-") != string::npos;
    bool ab = keysig.find("a-") != string::npos;
    bool db = keysig.find("d-") != string::npos;
    bool gb = keysig.find("g-") != string::npos;
    bool cb = keysig.find("c-") != string::npos;
    bool fb = keysig.find("f-") != string::npos;

    if (fs && !cs && !gs && !ds && !as && !es && !bs) {
        element->SetKeySig(KEYSIGNATURE_1s);
    }
    else if (fs && cs && !gs && !ds && !as && !es && !bs) {
        element->SetKeySig(KEYSIGNATURE_2s);
    }
    else if (fs && cs && gs && !ds && !as && !es && !bs) {
        element->SetKeySig(KEYSIGNATURE_3s);
    }
    else if (fs && cs && gs && ds && !as && !es && !bs) {
        element->SetKeySig(KEYSIGNATURE_4s);
    }
    else if (fs && cs && gs && ds && as && !es && !bs) {
        element->SetKeySig(KEYSIGNATURE_5s);
    }
    else if (fs && cs && gs && ds && as && es && !bs) {
        element->SetKeySig(KEYSIGNATURE_6s);
    }
    else if (fs && cs && gs && ds && as && es && bs) {
        element->SetKeySig(KEYSIGNATURE_7s);
    }
    else if (bb && !eb && !ab && !db && !gb && !cb && !fb) {
        element->SetKeySig(KEYSIGNATURE_1f);
    }
    else if (bb && eb && !ab && !db && !gb && !cb && !fb) {
        element->SetKeySig(KEYSIGNATURE_2f);
    }
    else if (bb && eb && ab && !db && !gb && !cb && !fb) {
        element->SetKeySig(KEYSIGNATURE_3f);
    }
    else if (bb && eb && ab && db && !gb && !cb && !fb) {
        element->SetKeySig(KEYSIGNATURE_4f);
    }
    else if (bb && eb && ab && db && gb && !cb && !fb) {
        element->SetKeySig(KEYSIGNATURE_5f);
    }
    else if (bb && eb && ab && db && gb && cb && !fb) {
        element->SetKeySig(KEYSIGNATURE_6f);
    }
    else if (bb && eb && ab && db && gb && cb && fb) {
        element->SetKeySig(KEYSIGNATURE_7f);
    }
    else if (!bb && !eb && !ab && !db && !gb && !cb && !fb && !fs && !cs && !gs && !ds && !as && !es && !bs) {
        element->SetKeySig(KEYSIGNATURE_0);
    }
    else {
        // nonstandard keysignature, so give a NONE style.
        element->SetKeySig(KEYSIGNATURE_NONE);
    }
}

//////////////////////////////
//
// HumdrumInput::setClef -- Convert a Humdrum clef to an MEI clef.
//

void HumdrumInput::setClef(StaffDef *part, const string &clef)
{
    if (clef.find("clefGG") != string::npos) {
        part->SetClefShape(CLEFSHAPE_GG);
    }
    else if (clef.find("clefG") != string::npos) {
        part->SetClefShape(CLEFSHAPE_G);
    }
    else if (clef.find("clefF") != string::npos) {
        part->SetClefShape(CLEFSHAPE_F);
    }
    else if (clef.find("clefC") != string::npos) {
        part->SetClefShape(CLEFSHAPE_C);
    }
    if (clef.find("clefX") != string::npos) {
        part->SetClefShape(CLEFSHAPE_perc);
        // by default place on 3rd line (unless another numbe is given):
        part->SetClefLine(3);
    }

    if (clef.find("2") != string::npos) {
        part->SetClefLine(2);
    }
    else if (clef.find("4") != string::npos) {
        part->SetClefLine(4);
    }
    else if (clef.find("3") != string::npos) {
        part->SetClefLine(3);
    }
    else if (clef.find("5") != string::npos) {
        part->SetClefLine(5);
    }
    else if (clef.find("1") != string::npos) {
        part->SetClefLine(1);
    }

    if (clef.find("vv") != string::npos) {
        part->SetClefDis(OCTAVE_DIS_15);
        part->SetClefDisPlace(PLACE_below);
    }
    else if (clef.find("v") != string::npos) {
        part->SetClefDis(OCTAVE_DIS_8);
        part->SetClefDisPlace(PLACE_below);
    }
    else if (clef.find("^^") != string::npos) {
        part->SetClefDis(OCTAVE_DIS_15);
        part->SetClefDisPlace(PLACE_above);
    }
    else if (clef.find("^") != string::npos) {
        part->SetClefDis(OCTAVE_DIS_8);
        part->SetClefDisPlace(PLACE_above);
    }
}

//////////////////////////////
//
// HumdrumInput::convertSystemMeasure --
//

bool HumdrumInput::convertSystemMeasure(int &line)
{
    int startline = line;
    int endline = getMeasureEndLine(startline);
    if (endline < 0) {
        // empty measure, skip it.  This can happen at the start of
        // a score if there is an invisible measure before the start of the
        // data, or if there is an ending bar before the ending of the data.
        line = -endline;
        return true;
    }
    else if (m_multirest[line] < 0) {
        // this is a whole-measure rest, but it is part of a multirest sequence.
        line = endline;
        return true;
    }
    else {
        line = endline;
    }

    setupSystemMeasure(startline, endline);

    storeStaffLayerTokensForMeasure(startline, endline);

    auto status = convertMeasureStaves(startline, endline);
    return status;
}

//////////////////////////////
//
// HumdrumInput::checkForOmd --
//

void HumdrumInput::checkForOmd(int startline, int endline)
{
    if (m_omd) {
        return;
    }
    m_omd = true;

    const vector<HTp> &kernstarts = m_kernstarts;
    if (kernstarts.size() == 0) {
        return;
    }
    HumdrumFile &infile = m_infile;
    string key;
    string value;
    for (int i = 0; i < infile.getLineCount(); i++) {
        if (infile[i].isData()) {
            break;
        }
        if (!infile[i].isReference()) {
            continue;
        }
        key = infile[i].getReferenceKey();
        if (key == "OMD") {
            value = infile[i].getReferenceValue();
            Tempo *tempo = new Tempo;
            m_measure->AddChild(tempo);
            addTextElement(tempo, value);
            tempo->SetTstamp(1.0);
            setStaff(tempo, 1);
        }
    }
}

//////////////////////////////
//
// HumdrumInput::setStaff -- Set the staff attribute to given integer.
//

template <class ELEMENT> void HumdrumInput::setStaff(ELEMENT element, int staffnum)
{
    xsdPositiveInteger_List stafflist;
    stafflist.push_back(staffnum);
    element->SetStaff(stafflist);
}

//////////////////////////////
//
// HumdrumInput::storeStaffLayerTokensForMeasure -- Store lists of notation
//   data by staff and layer.
//

void HumdrumInput::storeStaffLayerTokensForMeasure(int startline, int endline)
{
    HumdrumFile &infile = m_infile;
    const vector<HTp> &kernstarts = m_kernstarts;
    const vector<int> &rkern = m_rkern;
    vector<vector<vector<HTp> > > &lt = m_layertokens;

    lt.clear();
    lt.resize(kernstarts.size());

    int i, j;
    for (i = 0; i < (int)kernstarts.size(); i++) {
        lt[i].clear();
    }

    int lasttrack = -1;
    int track = -1;
    int staffindex = -1;
    int layerindex = 0;
    for (i = startline; i <= endline; i++) {
        if (!infile[i].hasSpines()) {
            continue;
        }
        lasttrack = -1;
        for (j = 0; j < infile[i].getFieldCount(); j++) {
            track = infile[i].token(j)->getTrack();
            if (track < 1) {
                // not a kern spine.
                continue;
            }
            staffindex = rkern[track];
            if (staffindex < 0) {
                continue;
            }
            if (track != lasttrack) {
                layerindex = 0;
            }
            else {
                layerindex++;
            }
            lasttrack = track;
            if (infile[i].token(j)->isData() && infile[i].token(j)->isNull()) {
                // keeping null interpretations to search for clef
                // in primary layer for secondary layer duplication.
                continue;
            }
            if (infile[i].token(j)->isCommentLocal() && infile[i].token(j)->isNull()) {
                // don't store empty comments as well. (maybe ignore all
                // comments anyway).
                continue;
            }
            if ((int)lt[staffindex].size() < layerindex + 1) {
                lt[staffindex].resize(lt[staffindex].size() + 1);
                lt[staffindex].back().clear(); // probably not necessary
            }

            lt[staffindex][layerindex].push_back(infile[i].token(j));
        }
    }

    // printMeasureTokens();
}

//////////////////////////////
//
// HumdrumInput::convertMeasureStaves -- fill in a measure with the
//    individual staff elements for each part.
//

bool HumdrumInput::convertMeasureStaves(int startline, int endline)
{
    const vector<HTp> &kernstarts = m_kernstarts;

    vector<int> layers = getStaffLayerCounts();

    int i;

    // pre-allocate
    vector<Staff *> stafflist(kernstarts.size());
    for (i = 0; i < (int)kernstarts.size(); i++) {
        stafflist[i] = new Staff();
        m_measure->AddChild(stafflist[i]);
    }

    checkForOmd(startline, endline);

    bool status = true;
    for (i = 0; i < (int)kernstarts.size(); i++) {
        m_currentstaff = i + 1;
        m_staff = stafflist[i];
        m_staff->SetN(m_currentstaff);

        status &= convertMeasureStaff(kernstarts[i]->getTrack(), startline, endline, i + 1, layers[i]);
        if (!status) {
            break;
        }
    }

    if (m_harm) {
        addHarmFloatsForMeasure(startline, endline);
    }

    return status;
}

//////////////////////////////
//
// HumdrumInput::addHarmFloatsForMeasure --
//

void HumdrumInput::addHarmFloatsForMeasure(int startline, int endline)
{
    if (!m_measure) {
        return;
    }
    HumdrumFile &infile = m_infile;
    for (int i = startline; i < endline; i++) {
        if (!infile[i].isData()) {
            continue;
        }
        int track = 0;
        for (int j = 0; j < infile[i].getFieldCount(); j++) {
            HTp token = infile.token(i, j);
            if (token->isDataType("**kern")) {
                track = token->getTrack();
            }
            if (token->isNull()) {
                continue;
            }
            if (!token->isDataType("**mxhm")) {
                continue;
            }
            Harm *harm = new Harm;
            Text *text = new Text;
            string content = cleanHarmString(*token);
            text->SetText(UTF8to16(content));
            harm->AddChild(text);
            m_measure->AddChild(harm);
            int staffindex = m_rkern[track];
            HumNum tstamp = getMeasureTstamp(token, staffindex);
            harm->SetTstamp(tstamp.getFloat());
            setStaff(harm, staffindex + 1);
            setLocationId(harm, token, -1);
        }
    }
}

//////////////////////////////
//
// HumdrumInput::cleanHarmString --
//

string HumdrumInput::cleanHarmString(const string &content)
{
    string root;
    string kind;
    string bass;

    bool foundspace = false;
    bool foundslash = false;
    for (int i = 0; i < (int)content.size(); i++) {
        if (content[i] == '/') {
            foundslash = true;
        }
        if (foundspace && !foundslash) {
            kind.push_back(content[i]);
            continue;
        }
        if (content[i] == ' ') {
            foundspace = true;
            continue;
        }
        if (content[i] == '/') {
            foundslash = true;
            continue;
        }
        if (!foundspace) {
            if (content[i] == '-') {
                root += "\u266D"; // unicode flat
            }
            else if (content[i] == '#') {
                root += "\u266F"; // unicode sharp
            }
            else {
                root.push_back(content[i]);
            }
        }
        else if (foundslash) {
            if (content[i] == '-') {
                bass += "\u266D"; // unicode flat
            }
            else if (content[i] == '#') {
                bass += "\u266F"; // unicode sharp
            }
            else {
                bass.push_back(content[i]);
            }
        }
        else {
            cerr << "should not get here with correct input " << content << endl;
        }
    }

    bool replacing = false;
    if (kind == "major-minor") {
        kind = "Mm7";
        replacing = true;
    }
    else if (kind == "minor-major") {
        kind = "mM7";
        replacing = true;
    }

    if (replace(kind, "major-", "M")) {
        replacing = true;
    }
    else if (replace(kind, "minor-", "m")) {
        replacing = true;
    }
    else if (replace(kind, "dominant-", "dom")) {
        replacing = true;
    }
    else if (replace(kind, "augmented-", "+")) {
        replacing = true;
    }
    else if (replace(kind, "suspended-", "sus")) {
        replacing = true;
    }
    else if (replace(kind, "diminished-", "\00B0")) { // degree sign
        replacing = true;
    }
    if (replace(kind, "seventh", "7")) {
        replacing = true;
    }
    else if (replace(kind, "ninth", "9")) {
        replacing = true;
    }
    else if (replace(kind, "11th", "11")) {
        replacing = true;
    }
    else if (replace(kind, "13th", "13")) {
        replacing = true;
    }
    else if (replace(kind, "second", "2")) {
        replacing = true;
    }
    else if (replace(kind, "fourth", "4")) {
        replacing = true;
    }
    else if (replace(kind, "sixth", "6")) {
        replacing = true;
    }

    if (kind == "major") {
        kind = "";
        replacing = true;
    }
    else if (kind == "maj") {
        kind = "";
        replacing = true;
    }
    else if (kind == "ma") {
        kind = ""; // degree sign
        replacing = true;
    }
    else if (kind == "minor") {
        kind = "m";
        replacing = true;
    }
    else if (kind == "min") {
        kind = "m";
        replacing = true;
    }
    else if (kind == "augmented") {
        kind = "+";
        replacing = true;
    }
    else if (kind == "minor-seventh") {
        kind = "m7";
        replacing = true;
    }
    else if (kind == "major-seventh") {
        kind = "M7";
        replacing = true;
    }
    else if (kind == "dominant-11th") {
        kind = "dom11";
        replacing = true;
    }
    else if (kind == "dominant-13th") {
        kind = "dom13";
        replacing = true;
    }
    else if (kind == "dominant-ninth") {
        kind = "dom9";
        replacing = true;
    }
    else if (kind == "half-diminished") {
        kind = "\u00F8"; // o-slash
        replacing = true;
    }
    else if (kind == "diminished") {
        kind = "\u00B0"; // degree sign
        replacing = true;
    }
    else if (kind == "dominant") {
        kind = "dom";
        replacing = true;
    }
    else if (kind == "m7b5") {
        replacing = true;
        kind = "m7\u266D"
               "5";
    }
    if ((kind != "") && !replacing) {
        root += ' ';
    }
    if (bass != "") {
        kind += '/';
    }
    string output = root + kind + bass;
    return output;
}

//////////////////////////////
//
// HumdrumInput::replace -- simple substring replacement implementation.
//    Returns true if there was any replacment done.
//

bool HumdrumInput::replace(string &str, const string &oldStr, const string &newStr)
{
    string::size_type pos = 0u;
    bool output = false;
    while ((pos = str.find(oldStr, pos)) != string::npos) {
        output = true;
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::convertMeasureStaff -- print a particular staff in a
//     particular measure.
//

bool HumdrumInput::convertMeasureStaff(int track, int startline, int endline, int n, int layercount)
{
    bool status = true;
    for (int i = 0; i < layercount; i++) {
        status &= convertStaffLayer(track, startline, endline, i);
        if (!status) {
            break;
        }
    }

    return status;
}

//////////////////////////////
//
// HumdrumInput::convertStaffLayer -- Prepare a layer element in the current
//   staff and then fill it with data.
//

bool HumdrumInput::convertStaffLayer(int track, int startline, int endline, int layerindex)
{
    m_layer = new Layer();
    m_currentlayer = layerindex + 1;

    m_layer->SetN(layerindex + 1);
    m_staff->AddChild(m_layer);

    vector<int> &rkern = m_rkern;
    int staffindex = rkern[track];
    vector<HTp> &layerdata = m_layertokens[staffindex][layerindex];

    if (layerdata.size() > 0) {
        if (layerdata[0]->size() > 0) {
            setLocationId(m_layer, layerdata[0], -1);
        }
    }

    if ((layerindex == 0) && (layerdata.size() > 0)) {
        if ((layerdata[0]->size() > 0) && (layerdata[0]->at(0) == '=')) {
            setLocationId(m_staff, layerdata[0], -1);
        }
    }

    if (m_comment) {
        string comment;
        comment += " kern: ";
        for (int i = 0; i < (int)layerdata.size(); i++) {
            comment += *layerdata[i];
            if (i < (int)layerdata.size() - 1) {
                comment += "  ";
            }
        }
        comment += " ";
        m_layer->SetComment(comment);
    }

    return fillContentsOfLayer(track, startline, endline, layerindex);
}

//////////////////////////////
//
// HumdrumInput::printGroupInfo --
//

void HumdrumInput::printGroupInfo(vector<humaux::HumdrumBeamAndTuplet> &tg, const vector<HTp> &layerdata)
{
    if (layerdata.size() != tg.size()) {
        cerr << "LAYER SIZE = " << layerdata.size() << "\tTGSIZE" << tg.size() << endl;
        return;
    }
    cerr << "TOK\tGRP\tBRAK\tNUM\tNBASE\tNSCAL\tBSTART\tBEND\tGBST\tGBEND\tTSTART"
            "\tTEND\tPRIORITY\n";
    for (int i = 0; i < (int)tg.size(); i++) {
        cerr << *layerdata[i] << "\t";
        cerr << tg[i].group << "\t";
        cerr << tg[i].bracket << "\t";
        cerr << tg[i].num << "\t";
        cerr << tg[i].numbase << "\t";
        cerr << tg[i].numscale << "\t";
        cerr << tg[i].beamstart << "\t";
        cerr << tg[i].beamend << "\t";
        cerr << tg[i].gbeamstart << "\t";
        cerr << tg[i].gbeamend << "\t";
        cerr << tg[i].tupletstart << "\t";
        cerr << tg[i].tupletend << "\t";
        cerr << tg[i].priority;
        cerr << endl;
    }
    cerr << "============================================" << endl;
}

//////////////////////////////
//
// HumdrumInput::handleGroupStarts --
//

void HumdrumInput::handleGroupStarts(
    const humaux::HumdrumBeamAndTuplet &tg, vector<string> &elements, vector<void *> &pointers, HTp token)
{
    vector<humaux::StaffStateVariables> &ss = m_staffstates;
    int staffindex = m_currentstaff - 1;

    if (tg.beamstart && tg.tupletstart) {
        if (tg.priority == 'T') {
            insertTuplet(elements, pointers, tg, token, ss[staffindex].suppress_beam_tuplet);
            insertBeam(elements, pointers, tg);
        }
        else {
            insertBeam(elements, pointers, tg);
            insertTuplet(elements, pointers, tg, token, ss[staffindex].suppress_beam_tuplet);
        }
    }
    else if (tg.beamstart) {
        insertBeam(elements, pointers, tg);
    }
    else if (tg.tupletstart) {
        insertTuplet(elements, pointers, tg, token, ss[staffindex].suppress_bracket_tuplet);
    }

    if (tg.gbeamstart) {
        // Grace note beams should not interact with
        // regular beams or tuplets.
        insertGBeam(elements, pointers, tg);
    }
}

//////////////////////////////
//
// HumdrumInput::handleGroupEnds --
//

void HumdrumInput::handleGroupEnds(
    const humaux::HumdrumBeamAndTuplet &tg, vector<string> &elements, vector<void *> &pointers)
{
    if (tg.beamend && tg.tupletend) {
        if (tg.priority == 'T') {
            removeTuplet(elements, pointers);
            removeBeam(elements, pointers);
        }
        else {
            removeBeam(elements, pointers);
            removeTuplet(elements, pointers);
        }
    }
    else if (tg.beamend) {
        removeBeam(elements, pointers);
    }
    else if (tg.tupletend) {
        removeTuplet(elements, pointers);
    }

    if (tg.gbeamend) {
        // Grace note beams should not interact with
        // regular beams or tuplets.
        removeGBeam(elements, pointers);
    }
}

//////////////////////////////
//
// HumdrumInput::fillContentsOfLayer -- Fill the layer with musical data.
//

bool HumdrumInput::fillContentsOfLayer(int track, int startline, int endline, int layerindex)
{
    int i;
    HumdrumFile &infile = m_infile;
    vector<HumNum> &timesigdurs = m_timesigdurs;
    vector<int> &rkern = m_rkern;
    int staffindex = rkern[track];
    if (staffindex < 0) {
        // not a kern spine.
        return false;
    }
    vector<HTp> &layerdata = m_layertokens[staffindex][layerindex];
    Layer *&layer = m_layer;

    if (layerdata.size() == 0) {
        // nothing to do.
        return true;
    }

    HumNum starttime = infile[startline].getDurationFromStart();
    HumNum endtime = infile[endline].getDurationFromStart() + infile[endline].getDuration();
    HumNum duration = endtime - starttime;

    /* Why not allowed?
    if (timesigdurs[startline] != duration) {
            m_measure->SetMetcon(BOOLEAN_false);
    }
    */

    HumNum layerstarttime = infile[startline].getDurationFromStart();
    HumNum layerendtime = infile[endline].getDurationFromStart();

    vector<HumNum> prespace;
    getTimingInformation(prespace, layerdata, layerstarttime, layerendtime);

    if (emptyMeasures()) {
        if (timesigdurs[startline] == duration) {
            MRest *mrest = new MRest();
            m_layer->AddChild(mrest);
            // Assign a Humdrum ID here.
        }
        else {
            Rest *rest = new Rest();
            m_layer->AddChild(rest);
            setDuration(rest, duration);
        }
        return true;
    }

    vector<string> elements;
    vector<void *> pointers;
    elements.push_back("layer");
    pointers.push_back((void *)layer);

    // If the layer contains only a single rest and the rest
    // is the same duration as the time signature, then
    // create a full measure rest (mrest).
    // Deal with time/clef/key changes in measures with
    // whole-measure rests later.  Also have to deal with
    // pedal mark in mrest meaures.
    if (hasFullMeasureRest(layerdata, timesigdurs[startline], duration)) {
        if (m_multirest[startline] > 1) {
            MultiRest *multirest = new MultiRest();
            multirest->SetNum(m_multirest[startline]);
            appendElement(layer, multirest);
        }
        else {
            MRest *mrest = new MRest();
            appendElement(layer, mrest);
        }

        // Basic compensation for clef change (can be improved later):
        for (i = 0; i < (int)layerdata.size(); i++) {
            if (!layerdata[i]->isClef()) {
                continue;
            }
            if (layerdata[i]->getDurationFromBarline() > 0) {
                // only insert a clef change after the whole-note rest
                // if the clef change is not really an initial clef.
                insertClefElement(elements, pointers, layerdata[i]);
            }
        }

        // Uncomment this when MRest::SetFermata() is implemented:
        // for (int z = 0; z < (int)layerdata.size(); z++) {
        //    if (!layerdata[z]->isData()) {
        //        continue;
        //    }
        //    if (!layerdata[z]->isRest()) {
        //        continue;
        //    }
        //    if (layerdata[z]->find(";") != string::npos) {
        //        mrest->SetFermata(PLACE_above);
        //    }
        //}

        // probably better to mark the rest in Humdrum data
        // as a full-measure rest here, and then process the
        // measure as normal below.
        return true;
    }

    vector<humaux::HumdrumBeamAndTuplet> tg;
    prepareBeamAndTupletGroups(layerdata, tg);

    if (m_debug) {
        printGroupInfo(tg, layerdata);
    }

    m_tupletscaling = 1;

    Note *note = NULL;

    for (i = 0; i < (int)layerdata.size(); i++) {
        if (prespace[i] > 0) {
            addSpace(elements, pointers, prespace[i]);
        }
        if (layerdata[i]->isData() && layerdata[i]->isNull()) {
            continue;
        }
        if (layerdata[i]->isInterpretation()) {
            handleOttavaMark(layerdata[i], note);
            handlePedalMark(layerdata[i], note);
            handleStaffStateVariables(layerdata[i]);
            if (layerdata[i]->getDurationFromStart() != 0) {
                if (layerdata[i]->isClef()) {
                    Clef *clef = insertClefElement(elements, pointers, layerdata[i]);
                    setLocationId(clef, layerdata[i], -1);
                }
                else if (layerdata[i]->isNull()) {
                    // duplicate clef changes in secondary layers
                    int xtrack = layerdata[i]->getTrack();
                    HTp tok = layerdata[i]->getPreviousFieldToken();
                    while (tok) {
                        int ttrack = tok->getTrack();
                        if (ttrack == xtrack) {
                            if (tok->isClef()) {
                                Clef *clef = insertClefElement(elements, pointers, tok);
                                setLocationId(clef, layerdata[i], -1);
                                // Uncomment when clef->SetSameas() is available:
                                // string sameas = "#clef-L";
                                // sameas += to_string(tok->getLineNumber());
                                // sameas += "F";
                                // sameas += to_string(tok->getFieldNumber());
                                // clef->SetSameas(sameas);
                                break;
                            }
                        }
                        tok = tok->getPreviousFieldToken();
                    }
                }
            }
            if (layerdata[i]->isTimeSignature()) {
                // Now done at the measure level.  This location might
                // be good for time signatures which change in the
                // middle of measures.
                // insertMeterSigElement(elements, pointers, layerdata, i);
            }
        }
        if (!layerdata[i]->isData()) {
            continue;
        }

        handleGroupStarts(tg[i], elements, pointers, layerdata[i]);
        addOrnamentMarkers(layerdata[i]);

        if (layerdata[i]->isChord()) {
            Chord *chord = new Chord;
            setLocationId(chord, layerdata[i], -1);
            appendElement(elements, pointers, chord);
            elements.push_back("chord");
            pointers.push_back((void *)chord);
            convertChord(chord, layerdata[i], staffindex);
            elements.pop_back();
            pointers.pop_back();
            processSlur(layerdata[i]);
            processDynamics(layerdata[i], staffindex);
            processDirection(layerdata[i], staffindex);
            addArticulations(chord, layerdata[i]);
            addFermata(layerdata[i]);
        }
        else if (layerdata[i]->isRest()) {
            if (layerdata[i]->find("yy") != string::npos) {
                // Invisible rest (or note which should be invisible.
                Space *irest = new Space;
                setLocationId(irest, layerdata[i], -1);
                appendElement(elements, pointers, irest);
                convertRhythm(irest, layerdata[i]);
                processSlur(layerdata[i]);
                processDynamics(layerdata[i], staffindex);
                processDirection(layerdata[i], staffindex);
            }
            else {
                Rest *rest = new Rest;
                setLocationId(rest, layerdata[i], -1);
                appendElement(elements, pointers, rest);
                convertRest(rest, layerdata[i]);
                processSlur(layerdata[i]);
                processDynamics(layerdata[i], staffindex);
                processDirection(layerdata[i], staffindex);
            }
        }
        else {
            // should be a note
            note = new Note;
            note->SetSVGClass("highlight");
            setLocationId(note, layerdata[i], -1);
            appendElement(elements, pointers, note);
            convertNote(note, layerdata[i], staffindex);
            processSlur(layerdata[i]);
            processDynamics(layerdata[i], staffindex);
            processDirection(layerdata[i], staffindex);
            if (m_signifiers.nostem && layerdata[i]->find(m_signifiers.nostem) != string::npos) {
                note->SetStemLen(0);
            }
            colorNote(note, layerdata[i]);
            addArticulations(note, layerdata[i]);
            addFermata(layerdata[i]);
        }

        handleGroupEnds(tg[i], elements, pointers);
    }

    if (prespace.size() > layerdata.size()) {
        if (prespace.back() > 0) {
            // if there is empty space at the end of the layer.  The layer is
            // rhythmically too short, so add a space element to match the
            // amount of underfilling.
            addSpace(elements, pointers, prespace.back());
        }
    }

    return true;
}

//////////////////////////////
//
// HumdrumInput::addArticulations --
//
// from: libmei/atttypes.h:
// enum data_ARTICULATION {
//    ARTICULATION_NONE = 0,             implemented:
//    ARTICULATION_acc,                        *
//    ARTICULATION_stacc,                      *
//    ARTICULATION_ten,                        *
//    ARTICULATION_stacciss,                   *
//    ARTICULATION_marc,                       *
//    ARTICULATION_marc_stacc,                 *
//    ARTICULATION_spicc,
//    ARTICULATION_doit,
//    ARTICULATION_scoop,
//    ARTICULATION_rip,
//    ARTICULATION_plop,
//    ARTICULATION_fall,
//    ARTICULATION_longfall,
//    ARTICULATION_bend,
//    ARTICULATION_flip,
//    ARTICULATION_smear,
//    ARTICULATION_shake,
//    ARTICULATION_dnbow,
//    ARTICULATION_upbow,
//    ARTICULATION_harm,                       *
//    ARTICULATION_snap,
//    ARTICULATION_fingernail,
//    ARTICULATION_ten_stacc,                  *
//    ARTICULATION_damp,
//    ARTICULATION_dampall,
//    ARTICULATION_open,
//    ARTICULATION_stop,
//    ARTICULATION_dbltongue,
//    ARTICULATION_trpltongue,
//    ARTICULATION_heel,
//    ARTICULATION_toe,
//    ARTICULATION_tap,
//    ARTICULATION_lhpizz,
//    ARTICULATION_dot,
//    ARTICULATION_stroke,
//};
//

template <class ELEMENT> void HumdrumInput::addArticulations(ELEMENT element, hum::HTp token)
{
    std::vector<data_ARTICULATION> artics;

    if (token->find('\'') != string::npos) {
        if (token->find("''") != string::npos) {
            artics.push_back(ARTICULATION_stacciss);
        }
        else if (token->find("^^") != string::npos) {
            artics.push_back(ARTICULATION_marc_stacc);
            //} else if (token->find('^') != string::npos) {
            //	artics.push_back(ARTICULATION_acc_stacc);
        }
        else if (token->find('~') != string::npos) {
            artics.push_back(ARTICULATION_ten_stacc);
        }
        else {
            artics.push_back(ARTICULATION_stacc);
        }
    }
    else if (token->find('`') != string::npos) {
        artics.push_back(ARTICULATION_stacciss);
    }
    else if (token->find('^') != string::npos) {
        if (token->find("^^") != string::npos) {
            artics.push_back(ARTICULATION_marc);
        }
        else {
            artics.push_back(ARTICULATION_acc);
        }
    }
    else if (token->find('~') != string::npos) {
        artics.push_back(ARTICULATION_ten);
    }
    else if (token->find('o') != string::npos) {
        artics.push_back(ARTICULATION_harm);
    }

    if (artics.size() > 0) {
        Artic *artic = new Artic;
        appendElement(element, artic);
        artic->SetArtic(artics);
    }
}

//////////////////////////////
//
// HumdrumInput::colorNote --
//

void HumdrumInput::colorNote(Note *note, HTp token)
{
    for (int i = 0; i < m_signifiers.mark.size(); i++) {
        if (token->find(m_signifiers.mark[i]) != string::npos) {
            note->SetColor(m_signifiers.mcolor[i]);
            break;
        }
    }
}

//////////////////////////////
//
// HumdrumInput::addOrnamentMarkers -- Temporarily convert
//   mordents and trills to text markers (<dir>) for DH.
//   These markers will overwrite any other existing text directions.
//

void HumdrumInput::addOrnamentMarkers(HTp token)
{
    if (!token) {
        return;
    }
    if (strchr(token->c_str(), 'm') != NULL) {
        token->setValue("LO", "TX", "t", "m");
        token->setValue("LO", "TX", "a", "true");
    }
    else if (strchr(token->c_str(), 'M') != NULL) {
        token->setValue("LO", "TX", "t", "M");
        token->setValue("LO", "TX", "a", "true");
    }
    else if (strchr(token->c_str(), 't') != NULL) {
        token->setValue("LO", "TX", "t", "t");
        token->setValue("LO", "TX", "a", "true");
    }
    else if (strchr(token->c_str(), 'T') != NULL) {
        token->setValue("LO", "TX", "t", "T");
        token->setValue("LO", "TX", "a", "true");
    }
    else if (strchr(token->c_str(), 'O') != NULL) {
        token->setValue("LO", "TX", "t", "*");
        token->setValue("LO", "TX", "a", "true");
    }
}

//////////////////////////////
//
// HumdrumInput::addSpace -- Add one or more space elements
//    to match the required duration.
//

void HumdrumInput::addSpace(vector<string> &elements, vector<void *> &pointers, HumNum duration)
{
    while (duration > 0) {
        Space *space = new Space;
        appendElement(elements, pointers, space);
        duration -= setDuration(space, duration);
    }
}

//////////////////////////////
//
// HumdrumInput::processDirection --
//

void HumdrumInput::processDirection(HTp token, int staffindex)
{
    string text = token->getValue("LO", "TX", "t");
    if (text.size() == 0) {
        return;
    }

    int zparam = token->isDefined("LO", "TX", "Z");
    int yparam = token->isDefined("LO", "TX", "Y");

    bool aparam = token->getValueBool("LO", "TX", "a");
    bool bparam = token->getValueBool("LO", "TX", "b");

    double Y = 0.0;
    double Z = 0.0;
    string placement;
    if (aparam) {
        placement = "above";
    }
    else if (bparam) {
        placement = "below";
    }
    else if (zparam) {
        Z = token->getValueInt("LO", "TX", "Z");
        if (Z > 0) {
            placement = "above";
        }
        else {
            placement = "below";
        }
    }
    else if (yparam) {
        Y = token->getValueInt("LO", "TX", "Y");
        if (Y > 0) {
            placement = "below";
        }
        else {
            placement = "above";
        }
    }

    Dir *dir = new Dir;
    m_measure->AddChild(dir);
    setStaff(dir, m_currentstaff);
    addTextElement(dir, text);
    HumNum tstamp = getMeasureTstamp(token, staffindex);
    dir->SetTstamp(tstamp.getFloat());

    if (placement == "above") {
        dir->SetPlace(STAFFREL_above);
    }
    else if (placement == "below") {
        dir->SetPlace(STAFFREL_below);
    }
}

/////////////////////////////
//
// HumdrumInput::processDynamics --
//

void HumdrumInput::processDynamics(HTp token, int staffindex)
{
    string tok;
    string dynamic;
    HumdrumLine *line = token->getLine();
    vector<humaux::StaffStateVariables> &ss = m_staffstates;
    if (line == NULL) {
        return;
    }
    int track = token->getTrack();
    int ttrack;
    int startfield = token->getFieldIndex() + 1;

    bool forceAboveQ = false;
    if (ss[staffindex].verse) {
        forceAboveQ = true;
    }

    // Handle "z" for sforzando (sf). Could also be rendered as (sfz), but deal
    // with that later, such as maybe make "zz" mean (sfz).

    if (token->find("z") != string::npos) {
        if (token->find("zy") == string::npos) { // don't show invisible sfz.

            bool aboveQ = token->getValueBool("LO", "DY", "a");
            bool belowQ = token->getValueBool("LO", "DY", "b");
            if (token->isDefined("LO", "DY", "Z")) {
                aboveQ = true;
            }
            else if (token->isDefined("LO", "DY", "Y")) {
                belowQ = true;
            }

            Dynam *dynam = new Dynam;
            m_measure->AddChild(dynam);
            setStaff(dynam, m_currentstaff);
            addTextElement(dynam, "sf");
            HumNum barstamp = getMeasureTstamp(token, staffindex);
            dynam->SetTstamp(barstamp.getFloat());
            if (aboveQ) {
                dynam->SetPlace(STAFFREL_above);
            }
            else if (belowQ) {
                dynam->SetPlace(STAFFREL_below);
            }
            else if (forceAboveQ) {
                dynam->SetPlace(STAFFREL_above);
            }
        }
    }

    for (int i = startfield; i < line->getFieldCount(); i++) {
        if (line->token(i)->isKern()) {
            ttrack = line->token(i)->getTrack();
            if (ttrack != track) {
                break;
            }
            // Break if this is not the last layer for the current spine
            if (!line->token(i)->isNull()) {
                break;
            }
        }
        if (!(line->token(i)->isDataType("**dynam") || line->token(i)->isDataType("**dyn"))) {
            continue;
        }
        // Don't skipp NULL tokens, because this algorithm only prints dynamics
        // after the last layer, and there could be notes in earlier layer
        // that need the dynamic.
        // if (line->token(i)->isNull()) {
        //     continue;
        // }

        string tok = *line->token(i);

        if (line->token(i)->getValueBool("auto", "DY", "processed")) {
            return;
        }
        line->token(i)->setValue("auto", "DY", "processed", "true");

        string hairpins;
        string letters;
        for (int i = 0; i < (int)tok.size(); i++) {
            if (isalpha(tok[i])) {
                letters.push_back(tok[i]);
            }
            else {
                hairpins.push_back(tok[i]);
            }
        }

        if (letters == "p") {
            dynamic = "p";
        }
        else if (letters == "pp") {
            dynamic = "pp";
        }
        else if (letters == "ppp") {
            dynamic = "ppp";
        }
        else if (letters == "pppp") {
            dynamic = "pppp";
        }
        else if (letters == "f") {
            dynamic = "f";
        }
        else if (letters == "ff") {
            dynamic = "ff";
        }
        else if (letters == "fff") {
            dynamic = "fff";
        }
        else if (letters == "ffff") {
            dynamic = "ffff";
        }
        else if (letters == "mp") {
            dynamic = "mp";
        }
        else if (letters == "mf") {
            dynamic = "mf";
        }
        else if (letters == "fz") {
            dynamic = "fz";
        }
        else if (letters == "fp") {
            dynamic = "fp";
        }
        else if (letters == "ffp") {
            dynamic = "ffp";
        }
        else if (letters == "sfp") {
            dynamic = "sfp";
        }
        else if (letters == "sf") {
            // for a sf which is shared betwee staves in piano music;
            // otherwise "z" on **kern notes is probably better.
            dynamic = "sf";
        }
        else if (letters == "sfz") {
            // for a sf which is shared betwee staves in piano music;
            // otherwise "zz" on **kern notes is probably better.
            dynamic = "sfz";
        }
        else if (letters == "rf") {
            dynamic = "rf";
        }
        else if (letters == "rfz") {
            dynamic = "rfz";
        }

        if (!dynamic.empty()) {
            bool aboveQ = line->token(i)->getValueBool("LO", "DY", "a");
            bool belowQ = line->token(i)->getValueBool("LO", "DY", "b");
            if (line->token(i)->isDefined("LO", "DY", "Z")) {
                aboveQ = true;
            }
            else if (line->token(i)->isDefined("LO", "DY", "Y")) {
                belowQ = true;
            }

            Dynam *dynam = new Dynam;
            m_measure->AddChild(dynam);
            setStaff(dynam, m_currentstaff);
            addTextElement(dynam, dynamic);
            HumNum barstamp = getMeasureTstamp(token, staffindex);
            dynam->SetTstamp(barstamp.getFloat());

            if (aboveQ) {
                dynam->SetPlace(STAFFREL_above);
            }
            else if (belowQ) {
                dynam->SetPlace(STAFFREL_below);
            }
            else if (forceAboveQ) {
                dynam->SetPlace(STAFFREL_above);
            }
        }
        if (hairpins.find("<") != string::npos) {
            HTp endtok = NULL;
            if (hairpins.find("<[") != string::npos) {
                endtok = token;
            }
            else {
                endtok = getCrescendoEnd(line->token(i));
            }
            if (endtok != NULL) {
                bool aboveQ = line->token(i)->getValueBool("LO", "DY", "a");
                bool belowQ = line->token(i)->getValueBool("LO", "DY", "b");
                if (line->token(i)->isDefined("LO", "DY", "Z")) {
                    aboveQ = true;
                }
                else if (line->token(i)->isDefined("LO", "DY", "Y")) {
                    belowQ = true;
                }
                Hairpin *hairpin = new Hairpin;
                setStaff(hairpin, m_currentstaff);
                HumNum tstamp = getMeasureTstamp(line->token(i), staffindex);
                HumNum tstamp2 = getMeasureTstamp(endtok, staffindex);
                int measures = getMeasureDifference(line->token(i), endtok);
                hairpin->SetTstamp(tstamp.getFloat());
                pair<int, double> ts2(measures, tstamp2.getFloat());
                hairpin->SetTstamp2(ts2);
                hairpin->SetForm(hairpinLog_FORM_cres);
                m_measure->AddChild(hairpin);
                if (aboveQ) {
                    hairpin->SetPlace(STAFFREL_above);
                }
                else if (belowQ) {
                    hairpin->SetPlace(STAFFREL_below);
                }
                else if (forceAboveQ) {
                    hairpin->SetPlace(STAFFREL_above);
                }
            }
        }
        else if (hairpins.find(">") != string::npos) {
            HTp endtok = NULL;
            if (hairpins.find(">]") != string::npos) {
                endtok = token;
            }
            else {
                endtok = getDecrescendoEnd(line->token(i));
            }
            if (endtok != NULL) {
                bool aboveQ = line->token(i)->getValueBool("LO", "DY", "a");
                bool belowQ = line->token(i)->getValueBool("LO", "DY", "b");
                if (line->token(i)->isDefined("LO", "DY", "Z")) {
                    aboveQ = true;
                }
                else if (line->token(i)->isDefined("LO", "DY", "Y")) {
                    belowQ = true;
                }
                Hairpin *hairpin = new Hairpin;
                setStaff(hairpin, m_currentstaff);
                HumNum tstamp = getMeasureTstamp(line->token(i), staffindex);
                HumNum tstamp2 = getMeasureTstamp(endtok, staffindex);
                int measures = getMeasureDifference(line->token(i), endtok);
                hairpin->SetTstamp(tstamp.getFloat());
                pair<int, double> ts2(measures, tstamp2.getFloat());
                hairpin->SetTstamp2(ts2);
                hairpin->SetForm(hairpinLog_FORM_dim);
                m_measure->AddChild(hairpin);
                if (aboveQ) {
                    hairpin->SetPlace(STAFFREL_above);
                }
                else if (belowQ) {
                    hairpin->SetPlace(STAFFREL_below);
                }
                else if (forceAboveQ) {
                    hairpin->SetPlace(STAFFREL_above);
                }
            }
        }
    }

    token = token->getNextToken();
    if (token == NULL) {
        return;
    }
    while (token && !token->isData()) {
        token = token->getNextToken();
    }
    if (token == NULL) {
        return;
    }
    if (!token->isNull()) {
        return;
    }
    // re-run this function on null tokens after the main note since
    // there may be dynamics unattached to a note (for various often
    // legitimate reasons).  Maybe make this more efficient later, such as
    // do a separate parse of dynamics data in a different loop.
    processDynamics(token, staffindex);
}

//////////////////////////////
//
// HumdrumInput::getMeasureDifference --
//

int HumdrumInput::getMeasureDifference(HTp starttok, HTp endtok)
{
    HumdrumLine *line = starttok->getOwner();
    if (line == NULL) {
        return 0;
    }
    HumdrumFile *file = line->getOwner();
    if (file == NULL) {
        return 0;
    }
    HumdrumFile &infile = *file;
    int startline = starttok->getLineIndex();
    int endline = endtok->getLineIndex();
    int counter = 0;
    for (int i = startline; i <= endline; i++) {
        if (infile[i].isBarline()) {
            counter++;
        }
    }
    return counter;
}

//////////////////////////////
//
// HumdrumInput::getCrescendoEnd --
//

HTp HumdrumInput::getCrescendoEnd(HTp token)
{
    return getHairpinEnd(token, "[");
}

//////////////////////////////
//
// HumdrumInput::getDecrescendoEnd --
//

HTp HumdrumInput::getDecrescendoEnd(HTp token)
{
    return getHairpinEnd(token, "]");
}

//////////////////////////////
//
// HumdrumInput::getHairpinEnd --
//

HTp HumdrumInput::getHairpinEnd(HTp token, const string &endchar)
{
    if (token == NULL) {
        return NULL;
    }
    token = token->getNextNonNullDataToken();
    int badtoken = 0;
    while (token != NULL) {
        if (token->find(endchar) != string::npos) {
            return token;
        }
        badtoken = 0;
        for (int i = 0; i < (int)token->size(); i++) {
            if (isalpha((*token)[i])) {
                badtoken = 1;
            }
            else if ((*token)[i] == '<') {
                badtoken = 1;
            }
            else if ((*token)[i] == '>') {
                badtoken = 1;
            }
            if (badtoken) {
                // maybe return the bad token for a weak ending
                // to a hairpin...
                return NULL;
            }
        }
        token = token->getNextNonNullDataToken();
    }
    return NULL;
}

//////////////////////////////
//
// HumdrumInput::getMeasureTstamp --
//

HumNum HumdrumInput::getMeasureTstamp(HTp token, int staffindex)
{
    vector<humaux::StaffStateVariables> &ss = m_staffstates;
    HumNum qbeat = token->getDurationFromBarline();
    HumNum mfactor = ss[staffindex].meter_bottom / 4;
    HumNum mbeat = qbeat * mfactor + 1;
    return mbeat;
}

/////////////////////////////
//
// HumdumInput::addTextElement -- Append text to a regular element.
//

template <class ELEMENT> void HumdrumInput::addTextElement(ELEMENT *element, const string &content)
{
    Text *text = new Text;
    element->AddChild(text);
    text->SetText(UTF8to16(unescapeHtmlEntities(content)));
}

/////////////////////////////
//
// HumdrumInput::processSlur --
//

void HumdrumInput::processSlur(HTp token)
{
    HTp slurstart = token->getSlurStartToken();
    if (!slurstart) {
        return;
    }

    int mindex = slurstart->getValueInt("MEI", "measureIndex");
    Measure *startmeasure = m_measures[mindex];

    Slur *slur = new Slur;
    slur->SetEndid("#" + token->getValue("MEI", "xml:id"));
    slur->SetStartid("#" + slurstart->getValue("MEI", "xml:id"));

    startmeasure->AddChild(slur);
    setStaff(slur, m_currentstaff);

    if (slurstart->getValueBool("LO", "S", "a")) {
        slur->SetCurvedir(curvature_CURVEDIR_above);
    }
    else if (slurstart->getValueBool("LO", "S", "b")) {
        slur->SetCurvedir(curvature_CURVEDIR_below);
    }
    else if (m_signifiers.slurabove && (slurstart->find(m_signifiers.slurabove) != string::npos)) {
        slur->SetCurvedir(curvature_CURVEDIR_above);
    }
    else if (m_signifiers.slurbelow && (slurstart->find(m_signifiers.slurbelow) != string::npos)) {
        slur->SetCurvedir(curvature_CURVEDIR_below);
    }
}

/////////////////////////////
//
// HumdrumInput::insertMeterSigElement -- A time signature or meter signature
//    that starts after the beginning of a movement.
//

void HumdrumInput::insertMeterSigElement(
    vector<string> &elements, vector<void *> &pointers, vector<HTp> &layerdata, int index)
{
    HTp tsig = layerdata[index];
    if (tsig->getDurationFromStart() <= 0) {
        return;
    }
    smatch matches;
    int count = -1;
    int unit = -1;
    if (regex_search(*tsig, matches, regex(R"(^\*M(\d+)/(\d+))"))) {
        count = stoi(matches[1]);
        unit = stoi(matches[2]);
    }
    else if (regex_search(*tsig, matches, regex(R"(^\*M(\d+)"))) {
        count = stoi(matches[1]);
    }
    // deal with non-rational units here.
    if (count < 0) {
        return;
    }
    MeterSig *msig = new MeterSig;
    appendElement(elements, pointers, msig);
    msig->SetCount(count);
    if (unit > 0) {
        msig->SetUnit(unit);
    }
    // check for mensuration here.
}

/////////////////////////////
//
// HumdrumInput::addSystemKeyTimeChange -- Add key or time signature changes
//    for a measure.  The scoreDef element is inserted before the measure in
//    which the change occurs. Presuming all staves have same changes for now.
//

void HumdrumInput::addSystemKeyTimeChange(int startline, int endline)
{
    HumdrumFile &infile = m_infile;

    HTp keysig = NULL;
    HTp timesig = NULL;

    for (int i = startline; i <= endline; i++) {
        if (infile[i].isData()) {
            break;
        }
        if (!infile[i].isInterpretation()) {
            continue;
        }
        for (int j = 0; j < infile[i].getFieldCount(); j++) {
            if ((!timesig) && regex_search(*infile.token(i, j), regex(R"(^\*M\d+/\d+)"))) {
                timesig = infile.token(i, j);
            }
            if ((!keysig) && regex_search(*infile.token(i, j), regex(R"(^\*k\[.*\])"))) {
                keysig = infile.token(i, j);
            }
        }
    }

    if ((keysig == NULL) && (timesig == NULL)) {
        // no key or time signature changes.
        return;
    }

    ScoreDef *scoreDef = new ScoreDef;
    m_sections.back()->AddChild(scoreDef);

    //    <scoreDef meter.count="6" meter.unit="8" key.sig="1f"/>

    if (timesig) {
        // cerr << "TIMESIG = " << timesig << endl;
        int count = -1;
        int unit = -1;
        smatch matches;
        if (regex_search(*timesig, matches, regex(R"(^\*M(\d+)/(\d+))"))) {
            count = stoi(matches[1]);
            unit = stoi(matches[2]);
            scoreDef->SetMeterCount(count);
            scoreDef->SetMeterUnit(unit);
        }
    }

    if (keysig) {
        // cerr << "KEYSIG = " << keysig << endl;
        setKeySig(scoreDef, *((string *)keysig));
    }
}

/////////////////////////////
//
// HumdrumInput::insertClefElement -- A clef which starts after the beginning of
// the movement.
//

Clef *HumdrumInput::insertClefElement(vector<string> &elements, vector<void *> &pointers, HTp token)
{
    Clef *clef = new Clef;
    appendElement(elements, pointers, clef);

    if (token->find("clefG") != string::npos) {
        clef->SetShape(CLEFSHAPE_G);
    }
    else if (token->find("clefF") != string::npos) {
        clef->SetShape(CLEFSHAPE_F);
    }
    else if (token->find("clefC") != string::npos) {
        clef->SetShape(CLEFSHAPE_C);
    }

    if (token->find("2") != string::npos) {
        clef->SetLine(2);
    }
    else if (token->find("4") != string::npos) {
        clef->SetLine(4);
    }
    else if (token->find("3") != string::npos) {
        clef->SetLine(3);
    }
    else if (token->find("5") != string::npos) {
        clef->SetLine(5);
    }
    else if (token->find("1") != string::npos) {
        clef->SetLine(1);
    }

    if (token->find("v") != string::npos) {
        clef->SetDis(OCTAVE_DIS_8);
        clef->SetDisPlace(PLACE_below);
    }

    return clef;
}

//////////////////////////////
//
// HumdrumInput::analyzeLayerBeams --
//

void HumdrumInput::analyzeLayerBeams(vector<int> &beamnum, vector<int> &gbeamnum, const vector<HTp> &layerdata)
{

    vector<int> beamstate(layerdata.size(), 0);
    vector<int> gbeamstate(layerdata.size(), 0); // for grace notes
    int negativeQ = 0;
    int gnegativeQ = 0;

    int i;
    for (i = 0; i < (int)beamstate.size(); i++) {
        if (!layerdata[i]->isData()) {
            if (i > 0) {
                beamstate[i] = beamstate[i - 1];
                gbeamstate[i] = gbeamstate[i - 1];
            }
            else {
                beamstate[i] = 0;
                gbeamstate[i] = 0;
            }
            continue;
        }
        if (layerdata[i]->isNull()) {
            // shouldn't get to this state
            beamstate[i] = 0;
            gbeamstate[i] = 0;
            continue;
        }
        if (layerdata[i]->isGrace()) {
            gbeamstate[i] = characterCount(*layerdata[i], 'L');
            gbeamstate[i] -= characterCount(*layerdata[i], 'J');
            // beamstate[i] = 0;
        }
        else {
            beamstate[i] = characterCount(*layerdata[i], 'L');
            beamstate[i] -= characterCount(*layerdata[i], 'J');
        }
        if (i > 0) {
            beamstate[i] += beamstate[i - 1];
            gbeamstate[i] += gbeamstate[i - 1];
        }
        if (beamstate[i] < 0) {
            negativeQ = 1;
        }
        if (gbeamstate[i] < 0) {
            negativeQ = 1;
        }
    }

    // Convert to beam enumerations.  Beamstates are nonzero for the
    // notes in a beam, but the last one is zero.
    int bcounter = 1;
    beamnum.resize(beamstate.size());
    std::fill(beamnum.begin(), beamnum.end(), 0);
    if (beamstate[0]) {
        beamnum[0] = bcounter;
    }
    else {
        beamnum[0] = 0;
    }
    for (i = 1; i < (int)beamstate.size(); i++) {
        if (beamstate[i]) {
            beamnum[i] = bcounter;
        }
        if ((!beamstate[i]) && beamstate[i - 1]) {
            beamnum[i] = bcounter++;
        }
    }

    bcounter = 1;
    gbeamnum.resize(beamstate.size());
    std::fill(gbeamnum.begin(), gbeamnum.end(), 0);
    if (gbeamstate[0]) {
        gbeamnum[0] = bcounter;
    }
    else {
        gbeamnum[0] = 0;
    }
    for (i = 1; i < (int)gbeamstate.size(); i++) {
        if (gbeamstate[i]) {
            gbeamnum[i] = bcounter;
        }
        if ((!gbeamstate[i]) && gbeamstate[i - 1]) {
            gbeamnum[i] = bcounter++;
        }
    }

    if (negativeQ || (beamstate.back() != 0)) {
        // something wrong with the beaming, either incorrect or
        // the beaming crosses a barline or layer.  Don't try to
        // beam anything.
        std::fill(beamstate.begin(), beamstate.end(), 0);
        std::fill(beamnum.begin(), beamnum.end(), 0);
    }

    if (gnegativeQ || (gbeamstate.back() != 0)) {
        // something wrong with the graceote beaming, either incorrect or
        // the beaming crosses a barline or layer.  Don't try to
        // beam anything.
        std::fill(gbeamstate.begin(), gbeamstate.end(), 0);
        std::fill(gbeamnum.begin(), gbeamnum.end(), 0);
    }
}

//////////////////////////////
//
// HumdrumInput::insertTuplet --
//

void HumdrumInput::insertTuplet(vector<string> &elements, vector<void *> &pointers,
    const humaux::HumdrumBeamAndTuplet &tg, HTp token, bool suppress)
{
    vector<humaux::StaffStateVariables> &ss = m_staffstates;

    Tuplet *tuplet = new Tuplet;
    appendElement(elements, pointers, tuplet);
    elements.push_back("tuplet");
    pointers.push_back((void *)tuplet);

    int staff = m_rkern[token->getTrack()];
    if (ss[staff].verse) {
        // If the music contains lyrics, force the tuplet above the staff.
        tuplet->SetBracketPlace(PLACE_above);
    }

    tuplet->SetNum(tg.num * tg.numscale);
    tuplet->SetNumbase(tg.numbase * tg.numscale);
    if (suppress) {
        // This shouldn't be needed, but just in case.
        // The visibility of the bracket is determined
        // earlier when there is a beam to attach
        // the tuplet to.
        tuplet->SetBracketVisible(BOOLEAN_false);
    }
    else {
        if (tg.bracket) {
            tuplet->SetBracketVisible(BOOLEAN_true);
        }
        else {
            tuplet->SetBracketVisible(BOOLEAN_false);
        }
    }
    if (suppress) {
        // Number is visible by default, so only hide
        // it if necessary.
        tuplet->SetNumVisible(BOOLEAN_false);
    }
    HumNum base = tg.numbase;
    if (!base.isPowerOfTwo()) {
        tuplet->SetNumFormat(tupletVis_NUMFORMAT_ratio);
    }
    else {
        tuplet->SetNumFormat(tupletVis_NUMFORMAT_count);
    }
    m_tupletscaling = tg.num;
    m_tupletscaling /= tg.numbase;
}

//////////////////////////////
//
// HumdrumInput::insertBeam --
//

void HumdrumInput::insertBeam(
    vector<string> &elements, vector<void *> &pointers, const humaux::HumdrumBeamAndTuplet &tg)
{
    Beam *beam = new Beam;
    appendElement(elements, pointers, beam);
    elements.push_back("beam");
    pointers.push_back((void *)beam);
}

//////////////////////////////
//
// HumdrumInput::insertGBeam --
//

void HumdrumInput::insertGBeam(
    vector<string> &elements, vector<void *> &pointers, const humaux::HumdrumBeamAndTuplet &tg)
{
    Beam *gbeam = new Beam;
    appendElement(elements, pointers, gbeam);
    elements.push_back("gbeam");
    pointers.push_back((void *)gbeam);
}

//////////////////////////////
//
// HumdrumInput::removeBeam --
//

void HumdrumInput::removeBeam(vector<string> &elements, vector<void *> &pointers)
{
    if (elements.back() != "beam") {
        cerr << "ERROR REMOVING BEAM" << endl;
        cerr << "ELEMENT STACK:" << endl;
        for (int i = (int)elements.size() - 1; i >= 0; i--) {
            cerr << i << ":\t" << elements[i] << endl;
        }
        return;
    }
    elements.pop_back();
    pointers.pop_back();
}

//////////////////////////////
//
// HumdrumInput::removeGBeam --
//

void HumdrumInput::removeGBeam(vector<string> &elements, vector<void *> &pointers)
{
    if (elements.back() != "gbeam") {
        cerr << "ERROR REMOVING GBEAM" << endl;
        cerr << "ELEMENT STACK:" << endl;
        for (int i = (int)elements.size() - 1; i >= 0; i--) {
            cerr << i << ":\t" << elements[i] << endl;
        }
        return;
    }
    elements.pop_back();
    pointers.pop_back();
}

//////////////////////////////
//
// HumdrumInput::removeTuplet --
//

void HumdrumInput::removeTuplet(vector<string> &elements, vector<void *> &pointers)
{
    if (elements.back() != "tuplet") {
        cerr << "ERROR REMOVING Tuplet" << endl;
        cerr << "ELEMENT BACK IS " << elements.back() << endl;
        cerr << "ELEMENT STACK:" << endl;
        for (int i = (int)elements.size() - 1; i >= 0; i--) {
            cerr << i << ":\t" << elements[i] << endl;
        }
        return;
    }
    elements.pop_back();
    pointers.pop_back();

    // Need to fix this when nested tuplets are allowed:
    m_tupletscaling = 1;
}

//////////////////////////////
//
// HumdrumInput::prepareBeamAndTupletGroups -- Calculate beam and tuplet
//     groupings for a layer.
//

void HumdrumInput::prepareBeamAndTupletGroups(
    const vector<hum::HTp> &layerdata, vector<humaux::HumdrumBeamAndTuplet> &tg)
{

    vector<int> beamnum;
    vector<int> gbeamnum;
    analyzeLayerBeams(beamnum, gbeamnum, layerdata);

    int i;
    tg.clear();

    // duritems == a list of items in the layer which have duration.
    // Grace notes, barlines, interpretations, local comments, global comments,
    // etc. are filtered out for the analysis.
    vector<HTp> duritems;

    // indexmapping == maping from a duritem index to a layerdata index.
    vector<int> indexmapping;

    // indexmapping2 == mapping from a layerdata index to a duritem index,
    // with -1 meaning no mapping.
    vector<int> indexmapping2;

    // durbeamnum == beam numbers for durational items only.
    vector<int> durbeamnum;

    // Extract a list of the layer items which have duration:
    for (i = 0; i < (int)layerdata.size(); i++) {
        if (!layerdata[i]->isData()) {
            indexmapping2.push_back(-1);
            continue;
        }
        if (layerdata[i]->isNull()) {
            indexmapping2.push_back(-1);
            continue;
        }
        if (layerdata[i]->isGrace()) {
            indexmapping2.push_back(-1);
            continue;
        }
        // don't consider notes without durations
        HumNum dur = Convert::recipToDuration(layerdata[i]);
        if (dur == 0) {
            indexmapping2.push_back(-1);
            continue;
        }
        indexmapping.push_back(i);
        indexmapping2.push_back((int)indexmapping.size() - 1);
        duritems.push_back(layerdata[i]);
        durbeamnum.push_back(beamnum[i]);
    }

    // poweroftwo == keeps track whether durations are based on a power
    // (non-tuplet) or not (tuplet).  Notes/rests with false poweroftwo
    // will be grouped into tuplets.
    vector<bool> poweroftwo(duritems.size());
    bool hastupletQ = false;
    vector<HumNum> dotlessdur(duritems.size());
    for (i = 0; i < (int)duritems.size(); i++) {
        HumNum duration = Convert::recipToDurationNoDots(*duritems[i]);
        dotlessdur[i] = duration;
        poweroftwo[i] = duration.isPowerOfTwo();
        hastupletQ |= !poweroftwo[i];
    }

    // Count the number of beams.  The durbeamnum vector contains a list
    // of beam numbers starting from 1 (or 0 if a note/rest has no beam).
    int beamcount = 0;
    for (i = 0; i < (int)durbeamnum.size(); i++) {
        if (durbeamnum[i] > beamcount) {
            beamcount = durbeamnum[i];
        }
    }

    // beamstarts and beamends are lists of the starting and ending
    // index for beams of duration items in the layer.  The index is
    // into the durlist vector (list of items which posses duration).
    vector<int> beamstarts(beamcount, -1);
    vector<int> beamends(beamcount, 0);
    for (i = 0; i < (int)durbeamnum.size(); i++) {
        if (durbeamnum[i]) {
            if (beamstarts[durbeamnum[i] - 1] < 0) {
                beamstarts[durbeamnum[i] - 1] = i;
            }
            beamends[durbeamnum[i] - 1] = i;
        }
    }

    // beamstartboolean == starting of a beam on a particular note
    // beamendboolean == ending of a beam on a particular note
    vector<int> beamstartboolean(durbeamnum.size(), 0);
    vector<int> beamendboolean(durbeamnum.size(), 0);
    for (i = 0; i < (int)beamstarts.size(); i++) {
        beamstartboolean[beamstarts[i]] = i + 1;
        beamendboolean[beamends[i]] = i + 1;
    }

    // Calculate grace note beam starts and ends.
    // Presuming no clef changes, etc. found between notes in
    // a gracenote beam.  Generalize further if so.
    // gbeamstart == boolean for starting of a grace note beam
    // gbeamend == boolean ending of a grace note beam
    vector<int> gbeamstart(layerdata.size(), 0);
    vector<int> gbeamend(layerdata.size(), 0);
    vector<int> gstate(layerdata.size(), 0);
    for (i = 0; i < (int)gbeamnum.size(); i++) {
        if (!gbeamnum[i]) {
            continue;
        }
        else if (gstate[gbeamnum[i]]) {
            continue;
        }
        else {
            gstate[gbeamnum[i]] = 1;
            gbeamstart[i] = gbeamnum[i];
        }
    }

    std::fill(gstate.begin(), gstate.end(), 0);
    for (i = (int)gbeamnum.size() - 1; i >= 0; i--) {
        if (!gbeamnum[i]) {
            continue;
        }
        else if (gstate[gbeamnum[i]]) {
            continue;
        }
        else {
            gstate[gbeamnum[i]] = 1;
            gbeamend[i] = gbeamnum[i];
        }
    }

    if (!hastupletQ) {
        tg.resize(layerdata.size());
        for (i = 0; i < (int)layerdata.size(); i++) {
            tg[i].gbeamstart = gbeamstart[i];
            tg[i].gbeamend = gbeamend[i];
            if (indexmapping2[i] < 0) {
                continue;
            }
            tg[i].beamstart = beamstartboolean[indexmapping2[i]];
            tg[i].beamend = beamendboolean[indexmapping2[i]];
        }
        return;
    }

    // fulldur == full duration of the note/rest including augmentation dots.
    vector<HumNum> fulldur(duritems.size());

    // dursum = a cumulative sum of the full durs, starting at 0 for
    // the first index.
    vector<HumNum> dursum(duritems.size());

    HumNum sum = 0;
    vector<int> twocounttop(dotlessdur.size(), 0);
    vector<int> twocountbot(dotlessdur.size(), 0);
    for (i = 0; i < (int)dotlessdur.size(); i++) {
        dotlessdur[i] = removeFactorsOfTwo(dotlessdur[i], twocounttop[i], twocountbot[i]);
        fulldur[i] = Convert::recipToDuration(*duritems[i]);
        dursum[i] = sum;
        sum += fulldur[i];
    }

    // beamdur = a list of the durations for each beam.
    vector<HumNum> beamdur(beamstarts.size());
    for (i = 0; i < (int)beamdur.size(); i++) {
        beamdur[i] = dursum[beamends[i]] - dursum[beamstarts[i]] + fulldur[beamends[i]];
    }

    // beampowdot == the number of augmentation dots on a power of two for
    // the duration of the beam.  -1 means could not be made power of two with
    // dots.
    vector<int> beampowdot(beamstarts.size(), -1);
    for (i = 0; i < (int)beampowdot.size(); i++) {
        beampowdot[i] = getDotPowerOfTwo(beamdur[i]);
    }

    vector<bool> binarybeams(beamstarts.size(), false);
    for (i = 0; i < (int)binarybeams.size(); i++) {
        if (poweroftwo[beamstarts[i]]) {
            binarybeams[i] = true;
        }
    }

    // Assume that tuplet beams which can fit into a power of two will form
    // a tuplet group.  Perhaps bias towards beampowdot being 0, and try to
    // beam groups to include non-beamed tuplets into lower powdots.
    // Should check that the factors of notes in the beam group all match...
    vector<int> tupletgroups(poweroftwo.size(), 0);

    // tupletbracket == boolean for if the tuplet group requires a bracket.
    // It will require a bracket if they are not all enclosed in a beam.
    vector<int> tupletbracket(poweroftwo.size(), -1);
    int tupletnum = 1;
    int j;

    // adjusted tuplet number by tuplet group
    vector<int> adjustcount;

    HumNum tupletdur = 0;
    int tupletcount = 0;
    int samedurtup = true;
    for (int i = 0; i < (int)beampowdot.size(); i++) {
        if (binarybeams[i]) {
            continue;
        }
        bool ingroup = false;
        if (beampowdot[i] >= 0) {
            for (j = beamstarts[i]; j <= beamends[i]; j++) {
                // may have to deal with dotted triplets (which appear to be powers of two)
                if (poweroftwo[j]) {
                    if (ingroup) {
                        ingroup = false;
                        tupletnum++;
                    }
                    tupletbracket[j] = 0;
                    continue;
                }
                ingroup = true;
                tupletgroups[j] = tupletnum;
                if (tupletcount == 0) {
                    samedurtup = true;
                    tupletdur = fulldur[j];
                }
                else if (tupletdur != fulldur[j]) {
                    samedurtup = false;
                }
                tupletcount++;

                tupletbracket[j] = 0;
            }
            if (samedurtup) {
                if (tupletnum >= adjustcount.size()) {
                    int oldsize = (int)adjustcount.size();
                    adjustcount.resize(tupletnum + 1);
                    for (int z = (int)oldsize; z < adjustcount.size(); z++) {
                        adjustcount[z] = 0;
                    }
                }
                adjustcount[tupletnum] = tupletcount;
            }
            tupletnum++;
            tupletcount = 0;
        }
    }

    vector<bool> beamstarts2(poweroftwo.size(), false);
    for (int i = 0; i < beamstarts.size(); i++) {
        beamstarts2[beamstarts[i]] = true;
    }

    vector<bool> beamends2(poweroftwo.size(), false);
    for (int i = 0; i < beamends.size(); i++) {
        beamends2[beamends[i]] = true;
    }

    // beamstate == boolean for keeping track of whether or not a beam
    // is currently active.
    bool beamstate = false;

    // Go back and link all partial beamed tuplets and non-beamed tuplets.
    HumNum groupdur;
    bool hasRest = false;
    for (int i = 0; i < (int)poweroftwo.size(); i++) {
        if ((!beamstarts.empty()) && beamstarts2[i]) {
            beamstate = true;
        }

        if (poweroftwo[i]) {
            // not a tuplet
            continue;
        }
        if (tupletgroups[i]) {
            // already in a tuplet group
            continue;
        }
        // At a tuplet which is not already in a tuplet group
        // search for how long the group should occur.
        j = i + 1;
        int ending = (int)poweroftwo.size() - 1;
        groupdur = 0;
        while (j < (int)poweroftwo.size()) {
            if ((!hasRest) && (duritems[j]->isRest())) {
                hasRest = true;
            }
            if (poweroftwo[j]) {
                ending = j - 1;
                break;
            }
            if (tupletgroups[i]) {
                ending = j - 1;
                break;
            }
            if (dotlessdur[j] != dotlessdur[i]) {
                ending = j - 1;
                break;
            }
            groupdur = dursum[j] - dursum[i] + fulldur[j];
            if (groupdur.isPowerOfTwo()) {
                ending = j;
                break;
            }
            j++;
        }
        if (ending >= 0) {
            // create a new tuplet group (which will require a bracket).
            for (j = i; j <= ending; j++) {
                tupletgroups[j] = tupletnum;
                // Only turn on a tuplet bracket if the tuplet is not inside
                // of a beam (may have to change if a tuplet bracket is
                // desired within a beam).
                if (!hasRest) {
                    tupletbracket[j] = true;
                }
            }
            tupletnum++;
            i = ending;
        }

        if ((!beamends2.empty()) && beamends2[i]) {
            beamstate = false;
        }
    }

    // tupletstartboolean == starting of a tuplet group
    // tupletendboolean == ending of a tuplet group
    vector<int> tupletstartboolean(tupletgroups.size(), 0);
    vector<int> tupletendboolean(tupletgroups.size(), 0);
    vector<bool> tstart(tupletgroups.size(), false);
    vector<bool> tend(tupletgroups.size(), false);
    for (i = 0; i < (int)tupletgroups.size(); i++) {
        if (!tupletgroups[i]) {
            continue;
        }
        if (!tstart[tupletgroups[i] - 1]) {
            tupletstartboolean[i] = tupletgroups[i];
            tstart[tupletgroups[i] - 1] = true;
        }
    }
    for (i = (int)tupletgroups.size() - 1; i >= 0; i--) {
        if (!tupletgroups[i]) {
            continue;
        }
        if (!tend[tupletgroups[i] - 1]) {
            tupletendboolean[i] = tupletgroups[i];
            tend[tupletgroups[i] - 1] = true;
        }
    }

    vector<int> tuptop(tupletgroups.size(), -1);
    vector<int> tupbot(tupletgroups.size(), -1);
    for (i = 0; i < (int)tupletgroups.size(); i++) {
        if (!tupletgroups[i]) {
            continue;
        }
        if (dotlessdur[i].getNumerator() == 1) {
            tuptop[i] = dotlessdur[i].getDenominator();
            tupbot[i] = nextLowerPowerOfTwo(tuptop[i]);
        }
        else {
            // this still needs to be fixed: dotted tuplets.
            tuptop[i] = dotlessdur[i].getDenominator();
            tupbot[i] = dotlessdur[i].getNumerator();
        }
        if ((tuptop[i] == 1) && (tupbot[i] == 1)) {
            tuptop[i] = 0;
            tupbot[i] = 0;
            cerr << "NOT A TUPLET " << endl;
        }
    }

    // tupletscale == 3 for three triplets, 6 for six sextuplets.
    int xmin = 0;
    int state = 0;
    int value = 0;
    int starti = -1;
    HumNum vdur;
    HumNum val2;
    vector<int> tupletscale;
    for (i = 0; i < (int)tupletstartboolean.size(); i++) {
        if (tupletstartboolean[i]) {
            state = 1;
            xmin = twocountbot[i];
            starti = i;
            continue;
        }
        if (!state) {
            continue;
        }
        if (twocountbot[i] < xmin) {
            xmin = twocountbot[i];
        }
        if (tupletendboolean[i]) {
            state = 0;
            value = (1 << xmin);
            vdur = dursum[i] - dursum[starti] + fulldur[i];
            if (vdur < 1) {
                val2 = vdur * value;
                if (val2.isInteger()) {
                    tupletscale.push_back(val2.getNumerator());
                }
                else {
                    tupletscale.push_back(value);
                }
            }
            else if (vdur / 3 * 2 == 1) {
                tupletscale.push_back(1);
            }
            else {
                tupletscale.push_back(value);
            }
        }
    }

    tg.resize(layerdata.size());
    for (i = 0; i < (int)layerdata.size(); i++) {
        if (indexmapping2[i] < 0) {
            tg[i].group = -1;
            tg[i].bracket = -1;
            tg[i].num = -1;
            tg[i].numbase = -1;
            tg[i].beamstart = 0;
            tg[i].beamend = 0;
            tg[i].gbeamstart = gbeamstart[i];
            tg[i].gbeamend = gbeamend[i];
            tg[i].tupletstart = 0;
            tg[i].tupletend = 0;
            tg[i].priority = ' ';
        }
        else {
            tg[i].group = tupletgroups[indexmapping2[i]];
            tg[i].bracket = tupletbracket[indexmapping2[i]];
            tg[i].num = tuptop[indexmapping2[i]];
            tg[i].numbase = tupbot[indexmapping2[i]];
            tg[i].beamstart = beamstartboolean[indexmapping2[i]];
            tg[i].beamend = beamendboolean[indexmapping2[i]];
            tg[i].gbeamstart = gbeamstart[i];
            tg[i].gbeamend = gbeamend[i];
            tg[i].tupletstart = tupletstartboolean[indexmapping2[i]];
            tg[i].tupletend = tupletendboolean[indexmapping2[i]];
            if (tg[i].group > 0) {
                tg[i].numscale = tupletscale[tg[i].group - 1];
            }
            else {
                tg[i].numscale = 1;
            }
            if (tg[i].group > 0) {
                if (tg[i].group < (int)adjustcount.size()) {
                    if (adjustcount[tg[i].group] % tg[i].num == 0) {
                        tg[i].numscale = adjustcount[tg[i].group] / tg[i].num;
                    }
                    if (tg[i].numscale == 0) {
                        tg[i].numscale = 1;
                    }
                }
            }
            if (tg[i].beamstart && tg[i].tupletstart) {
                if (tg[i].bracket > 0) {
                    tg[i].priority = 'T'; // open tuplet first
                }
                else {
                    tg[i].priority = 'B'; // open beam first
                }
            }
            else if (tg[i].beamend && tg[i].tupletend) {
                if (tg[i].bracket > 0) {
                    // tg[i].priority = 'B'; // close beam first
                    tg[i].priority = 'T'; // maybe close tuplet first (check more cases)
                }
                else {
                    tg[i].priority = 'T'; // close tuplet first
                }
            }
            else {
                tg[i].priority = ' ';
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::nextLowerPowerOfTwo --
//

int HumdrumInput::nextLowerPowerOfTwo(int x)
{
    if (x < 1) {
        return 1;
    }
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x - (x >> 1);
}

//////////////////////////////
//
// HumdrumInput::getDotPowerOfTwo -- Checks up to 3 augmentation dots.
//

int HumdrumInput::getDotPowerOfTwo(HumNum value)
{
    if (value.isPowerOfTwo()) {
        return 0;
    }
    // check for one dot
    HumNum tval = value * 2 / 3;
    if (tval.isPowerOfTwo()) {
        return 1;
    }
    tval = value * 4 / 7;
    if (tval.isPowerOfTwo()) {
        return 2;
    }
    tval = value * 8 / 15;
    if (tval.isPowerOfTwo()) {
        return 3;
    }

    return -1;
}

//////////////////////////////
//
// HumdrumInput::removeFactorsOfTwo --
//

HumNum HumdrumInput::removeFactorsOfTwo(HumNum value, int &tcount, int &bcount)
{
    int top = value.getNumerator();
    int bot = value.getDenominator();
    tcount = 0;
    bcount = 0;
    if (top > 0) {
        while (top % 2 == 0) {
            top = top >> 1;
            tcount++;
        }
    }
    if (bot > 0) {
        while (bot % 2 == 0) {
            bot = bot >> 1;
            bcount++;
        }
    }
    HumNum output(top, bot);
    return output;
}

//////////////////////////////
//
// HumdrumInput::handleStaffStateVariables -- Deal with simple boolean switches
//     that are turned on/off by interpretation tokes.
//
// Controls that this function deals with:
// *Xtuplet = suppress beam and bracket tuplet numbers
// *tuplet = display beam and bracket tuplet numbers
// *Xbeamtup = suppress beam tuplet numbers
// *beamtup  = display beam tuplet numbers
// *Xbrackettup = suppress tuplet brackets
// *brackettup  = display tuplet brackets
//

void HumdrumInput::handleStaffStateVariables(HTp token)
{
    int staffindex = m_currentstaff - 1;
    string value = *token;
    vector<humaux::StaffStateVariables> &ss = m_staffstates;
    if (value == "*Xbeamtup") {
        ss[staffindex].suppress_beam_tuplet = true;
    }
    else if (value == "*beamtup") {
        ss[staffindex].suppress_beam_tuplet = false;
    }
    if (value == "*Xbrackettup") {
        ss[staffindex].suppress_bracket_tuplet = true;
    }
    else if (value == "*brackettup") {
        ss[staffindex].suppress_bracket_tuplet = false;
    }
    if (value == "*Xtuplet") {
        ss[staffindex].suppress_beam_tuplet = true;
        ss[staffindex].suppress_bracket_tuplet = true;
    }
    if (value == "*tuplet") {
        ss[staffindex].suppress_beam_tuplet = false;
        ss[staffindex].suppress_bracket_tuplet = false;
    }
}

//////////////////////////////
//
// HumdrumInput::handleOttavaMark --  *8va turns on and *X8va tuns off.
//    Generalized latter to 15ma, 8va basso, 15ma basso.  IF the *X8va is
//    (incorrectly) placed at the start of the next measure before a note,
//    then this algorithm may not work (need to keep track of the last note
//    in the previous measure among all layers).
//

void HumdrumInput::handleOttavaMark(HTp token, Note *note)
{
    vector<humaux::StaffStateVariables> &ss = m_staffstates;
    int staffindex = m_currentstaff - 1;

    if (*token == "*8va") {
        // turn on ottava
        ss[staffindex].ottavameasure = m_measure;
        ss[staffindex].ottavanotestart = NULL;
        ss[staffindex].ottavanoteend = NULL;
        // When a new note is read, check if ottavameasure
        // is non-null, and if so, store the new note in ottavanotestart.
    }
    else if (*token == "*X8va") {
        // turn off ottava
        if ((ss[staffindex].ottavameasure != NULL) && (ss[staffindex].ottavanotestart != NULL)
            && (ss[staffindex].ottavanoteend != NULL)) {
            Octave *octave = new Octave;
            ss[staffindex].ottavameasure->AddChild(octave);
            setStaff(octave, staffindex + 1);
            octave->SetDis(OCTAVE_DIS_8);
            octave->SetStartid("#" + ss[staffindex].ottavanotestart->GetUuid());
            octave->SetEndid("#" + ss[staffindex].ottavanoteend->GetUuid());
            octave->SetDisPlace(PLACE_above);
        }
        ss[staffindex].ottavanotestart = NULL;
        ss[staffindex].ottavanoteend = NULL;
        ss[staffindex].ottavameasure = NULL;
    }
}

//////////////////////////////
//
// HumdrumInput::handlePedalMark --  *ped turns on and *Xped tuns off.
//    IF the *X8va is (incorrectly) placed at the start of the next measure
//    before a note, then this algorithm may not work (need to keep track
//    of the last note in the previous measure among all layers).
//

void HumdrumInput::handlePedalMark(HTp token, Note *note)
{
    int staffindex = m_currentstaff - 1;

    if (*token == "*ped") {
        // turn on pedal
        Pedal *pedal = new Pedal;
        m_measure->AddChild(pedal);
        HumNum tstamp = getMeasureTstamp(token, staffindex);
        pedal->SetTstamp(tstamp.getFloat());
        pedal->SetDir(pedalLog_DIR_down);
        setStaff(pedal, m_currentstaff);
    }
    else if (*token == "*Xped") {
        // turn off pedal
        HTp pdata = getPreviousDataToken(token);
        if (pdata != NULL) {
            Pedal *pedal = new Pedal;
            m_measure->AddChild(pedal);
            HumNum tstamp = getMeasureTstamp(pdata, staffindex);
            pedal->SetTstamp(tstamp.getFloat());
            pedal->SetDir(pedalLog_DIR_up);
            setStaff(pedal, m_currentstaff);
        }
    }
}

//////////////////////////////
//
// HumdrumInput::getPreviousDataToken --
//

HTp HumdrumInput::getPreviousDataToken(HTp token)
{
    if (token == NULL) {
        return NULL;
    }
    HTp current = token;
    current = current->getPreviousToken();
    while (current) {
        if (current == NULL) {
            break;
        }
        if (current->isData()) {
            break;
        }
        current = current->getPreviousToken();
    }
    return current;
}

//////////////////////////////
//
// HumdrumInput::convertChord --
//

void HumdrumInput::convertChord(Chord *chord, HTp token, int staffindex)
{
    int scount = token->getSubtokenCount();
    for (int j = 0; j < scount; j++) {
        Note *note = new Note;
        setLocationId(note, token, j);
        appendElement(chord, note);
        convertNote(note, token, staffindex, j);
    }

    convertRhythm(chord, token);

    // Stem direction of the chord.  If both up and down, then show up.
    if (token->find("/") != string::npos) {
        chord->SetStemDir(STEMDIRECTION_up);
    }
    else if (token->find("\\") != string::npos) {
        chord->SetStemDir(STEMDIRECTION_down);
    }

    token->setValue("MEI", "xml:id", chord->GetUuid());
    int index = (int)m_measures.size() - 1;
    token->setValue("MEI", "measureIndex", index);
}

//////////////////////////////
//
// HumdrumInput::getTimingInformation -- Calculate the start time and duration
//     of each event so that partial layers can be filled in with <space>
//     elements if necessary.

void HumdrumInput::getTimingInformation(
    vector<HumNum> &prespace, vector<HTp> &layerdata, HumNum layerstarttime, HumNum layerendtime)
{
    prespace.resize(layerdata.size());
    if (prespace.size() > 0) {
        prespace[0] = 0;
    }
    vector<HumNum> startdur(layerdata.size());
    vector<HumNum> duration(layerdata.size());
    HumNum correction = 0;
    for (int i = 0; i < (int)layerdata.size(); i++) {
        startdur[i] = layerdata[i]->getDurationFromStart();
        if (!layerdata[i]->isData()) {
            duration[i] = 0;
        }
        else {
            duration[i] = layerdata[i]->getDuration();
        }
    }

    if (layerdata.size() > 0) {
        prespace[0] = startdur[0] - layerstarttime;
    }
    for (int i = 1; i < (int)layerdata.size(); i++) {
        prespace[i] = startdur[i] - startdur[i - 1] - duration[i - 1];
        if (prespace[i] < 0) {
            correction += prespace[i];
            prespace[i] = 0;
        }
        else if (prespace[i] > 0) {
            prespace[i] += correction;
            correction = 0;
        }
    }
    if (layerdata.size() > 0) {
        prespace.resize(prespace.size() + 1);
        prespace.back() = layerendtime - startdur.back() - duration.back();
    }
}

//////////////////////////////
//
// HumdrumInput::hasFullMeasureRest -- Returns true if a single
//     rest in the measure which has the same duration as the
//     time signature.
//

bool HumdrumInput::hasFullMeasureRest(vector<HTp> &layerdata, HumNum timesigdur, HumNum measuredur)
{
    if (timesigdur != measuredur) {
        return false;
    }
    int datacount = 0;
    for (int i = 0; i < (int)layerdata.size(); i++) {
        if (!layerdata[i]->isData()) {
            continue;
        }
        if (layerdata[i]->isNull()) {
            continue;
        }
        // deal with grace notes in same measure as mrest?
        datacount++;
        if (datacount > 1) {
            return false;
        }
        if (!layerdata[i]->isRest()) {
            return false;
        }
        if (layerdata[i]->find("yy") != string::npos) {
            // treat invisible full measure rest as a space later.
            return false;
        }
    }
    return true;
}

//////////////////////////////
//
// HumdrumInput::appendElement --
//

template <class PARENT, class CHILD> void HumdrumInput::appendElement(PARENT parent, CHILD child)
{
    parent->AddChild(child);
}

/////////////////////////////
//
// HumdrumInput::appendElement --
//

template <class CHILD>
void HumdrumInput::appendElement(const vector<string> &name, const vector<void *> &pointers, CHILD child)
{
    if (name.back() == "beam") {
        appendElement((Beam *)pointers.back(), child);
    }
    else if (name.back() == "gbeam") {
        appendElement((Beam *)pointers.back(), child);
    }
    else if (name.back() == "layer") {
        appendElement((Layer *)pointers.back(), child);
    }
    else if (name.back() == "tuplet") {
        appendElement((Tuplet *)pointers.back(), child);
    }
    else if (name.back() == "chord") {
        appendElement((Chord *)pointers.back(), child);
    }
}

/////////////////////////////
//
// HumdrumInput::convertRest --
//

void HumdrumInput::convertRest(Rest *rest, HTp token, int subtoken)
{

    // Shouldn't be in a chord, so add rest duration here.
    // Also full-measure rests are handled elsewhere.
    convertRhythm(rest, token, subtoken);

    string tstring;
    if (subtoken < 0) {
        tstring = *token;
    }
    else {
        tstring = token->getSubtoken(subtoken);
    }

    int layer = m_currentlayer;

    if (tstring.find("jj") != string::npos) {
        int newstaff = m_currentstaff - 1;
        if ((newstaff > 0) && (newstaff <= (int)m_kernstarts.size())) {
            setStaff(rest, newstaff);
        }
    }
    else if (tstring.find("j") != string::npos) {
        int newstaff = m_currentstaff + 1;
        if ((newstaff > 0) && (newstaff <= (int)m_kernstarts.size())) {
            setStaff(rest, newstaff);
        }
    }

    if (tstring.find(";") != string::npos) {
        if ((tstring.find("yy") == string::npos) && (tstring.find(";y") == string::npos)) {
            if (layer == 1) {
                rest->SetFermata(PLACE_above);
            }
            else if (layer == 2) {
                rest->SetFermata(PLACE_below);
            }
            else {
                // who knows, maybe check the stem direction or see
                // if another note/rest in a different layer already
                // has a fermata (so you would not want to overwrite them).
                rest->SetFermata(PLACE_above);
            }
        }
    }

    token->setValue("MEI", "xml:id", rest->GetUuid());
    int index = (int)m_measures.size() - 1;
    token->setValue("MEI", "measureIndex", index);
}

/////////////////////////////
//
// HumdrumInput::convertNote --
//    default value:
//       subtoken = -1 (use the first subtoken);
//

void HumdrumInput::convertNote(Note *note, HTp token, int staffindex, int subtoken)
{
    vector<humaux::StaffStateVariables> &ss = m_staffstates;

    string tstring;
    int stindex = 0;
    if (subtoken < 0) {
        tstring = *token;
    }
    else {
        tstring = token->getSubtoken(subtoken);
        stindex = subtoken;
    }

    bool chordQ = token->isChord();

    bool octaveupQ = ss[staffindex].ottavameasure ? true : false;

    if ((ss[staffindex].ottavameasure != NULL) && (ss[staffindex].ottavanotestart == NULL)) {
        ss[staffindex].ottavanotestart = note;
    }
    ss[staffindex].ottavanoteend = note;

    // acc/unacc need to be switched in verovio, so switch also here later:
    if (tstring.find("qq") != string::npos) {
        note->SetGrace(GRACE_unacc);
        // set the visual duration to an eighth note if there
        // is no rhythm specified (will be overwritten later
        // if there is a rhythm).
        note->SetDur(DURATION_8);
    }
    else if (tstring.find("q") != string::npos) {
        note->SetGrace(GRACE_acc);
        // set the visual duration to an eighth note if there
        // is no rhythm specified (will be overwritten later
        // if there is a rhythm).
        note->SetDur(DURATION_8);
    }

    // Add the pitch information
    int diatonic = Convert::kernToBase7(tstring);
    int octave = diatonic / 7;
    if (octaveupQ) {
        // @oct required even if @oct.ges given.
        note->SetOctGes(octave);
        note->SetOct(octave - 1);
    }
    else {
        note->SetOct(octave);
    }

    switch (diatonic % 7) {
        case 0: note->SetPname(PITCHNAME_c); break;
        case 1: note->SetPname(PITCHNAME_d); break;
        case 2: note->SetPname(PITCHNAME_e); break;
        case 3: note->SetPname(PITCHNAME_f); break;
        case 4: note->SetPname(PITCHNAME_g); break;
        case 5: note->SetPname(PITCHNAME_a); break;
        case 6: note->SetPname(PITCHNAME_b); break;
    }

    bool editorial = false;
    if (m_signifiers.editacc) {
        if (token->find(m_signifiers.editacc) != string::npos) {
            editorial = true;
        }
    }

    int accidCount = Convert::kernToAccidentalCount(tstring);
    bool showInAccid = token->hasVisibleAccidental(stindex);
    bool showInAccidGes = !showInAccid;
    if (!editorial) {
        // don't mark cautionary accidentals if the note has
        // an editorial accidental.
        if (token->hasCautionaryAccidental(stindex)) {
            addCautionaryAccidental(note, token, accidCount);
            showInAccidGes = true;
            showInAccid = false;
        }
    }

    if (!editorial) {
        if (showInAccid) {
            switch (accidCount) {
                // case +3: note->SetAccid(ACCIDENTAL_EXPLICIT_ts); break;
                case +2: note->SetAccid(ACCIDENTAL_EXPLICIT_x); break;
                case +1: note->SetAccid(ACCIDENTAL_EXPLICIT_s); break;
                case 0: note->SetAccid(ACCIDENTAL_EXPLICIT_n); break;
                case -1: note->SetAccid(ACCIDENTAL_EXPLICIT_f); break;
                case -2:
                    note->SetAccid(ACCIDENTAL_EXPLICIT_ff);
                    break;
                    // case -3: note->SetAccid(ACCIDENTAL_EXPLICIT_tf); break;
            }
        }
    }
    else {
        Accid *accid = new Accid;
        appendElement(note, accid);
        accid->SetFunc(accidLog_FUNC_edit);

        switch (accidCount) {
            case +2: accid->SetAccid(ACCIDENTAL_EXPLICIT_x); break;
            case +1: accid->SetAccid(ACCIDENTAL_EXPLICIT_s); break;
            case 0: accid->SetAccid(ACCIDENTAL_EXPLICIT_n); break;
            case -1: accid->SetAccid(ACCIDENTAL_EXPLICIT_f); break;
            case -2: accid->SetAccid(ACCIDENTAL_EXPLICIT_ff); break;
        }
    }

    if (showInAccidGes) {
        switch (accidCount) {
            // case +3: note->SetAccidGes(ACCIDENTAL_IMPLICIT_ts); break;
            case +2: note->SetAccidGes(ACCIDENTAL_IMPLICIT_ss); break;
            case +1: note->SetAccidGes(ACCIDENTAL_IMPLICIT_s); break;
            case 0: note->SetAccidGes(ACCIDENTAL_IMPLICIT_n); break;
            case -1: note->SetAccidGes(ACCIDENTAL_IMPLICIT_f); break;
            case -2:
                note->SetAccidGes(ACCIDENTAL_IMPLICIT_ff);
                break;
                // case -3: note->SetAccidGes(ACCIDENTAL_IMPLICIT_tf); break;
        }
    }

    if (!chordQ) {
        HumNum dur = convertRhythm(note, token, subtoken);
        if (dur == 0) {
            note->SetDur(DURATION_4);
            note->SetStemLen(0);
            // if you want a stemless grace note, then set the
            // stemlength to zero explicitly.
        }
    }

    if (!chordQ) {
        if (tstring.find("/") != string::npos) {
            note->SetStemDir(STEMDIRECTION_up);
        }
        else if (tstring.find("\\") != string::npos) {
            note->SetStemDir(STEMDIRECTION_down);
        }
    }

    if (tstring.find("yy") != string::npos) {
        note->SetVisible(BOOLEAN_false);
    }

    // handle ties
    if ((tstring.find("[") != string::npos) || (tstring.find("_") != string::npos)) {
        processTieStart(note, token, tstring);
    }

    if ((tstring.find("_") != string::npos) || (tstring.find("]") != string::npos)) {
        processTieEnd(note, token, tstring);
    }

    if (tstring.find("jj") != string::npos) {
        int newstaff = m_currentstaff - 1;
        if ((newstaff > 0) && (newstaff <= (int)m_kernstarts.size())) {
            setStaff(note, newstaff);
        }
    }
    else if (tstring.find("j") != string::npos) {
        int newstaff = m_currentstaff + 1;
        if ((newstaff > 0) && (newstaff <= (int)m_kernstarts.size())) {
            setStaff(note, newstaff);
        }
    }

    convertVerses(note, token, subtoken);

    // maybe organize by sub-token index, but consider as chord for now
    if (!chordQ) {
        token->setValue("MEI", "xml:id", note->GetUuid());
        int index = (int)m_measures.size() - 1;
        token->setValue("MEI", "measureIndex", index);
    }
}

//////////////////////////////
//
// HumdrumInput::addCautionaryAccidental --
//

void HumdrumInput::addCautionaryAccidental(Note *note, HTp token, int acount)
{
    Accid *accid = new Accid;
    note->AddChild(accid);
    accid->SetFunc(accidLog_FUNC_caution);
    switch (acount) {
        case +3: accid->SetAccid(ACCIDENTAL_EXPLICIT_ts); break;
        case +2: accid->SetAccid(ACCIDENTAL_EXPLICIT_x); break;
        case +1: accid->SetAccid(ACCIDENTAL_EXPLICIT_s); break;
        case 0: accid->SetAccid(ACCIDENTAL_EXPLICIT_n); break;
        case -1: accid->SetAccid(ACCIDENTAL_EXPLICIT_f); break;
        case -2: accid->SetAccid(ACCIDENTAL_EXPLICIT_ff); break;
        case -3: accid->SetAccid(ACCIDENTAL_EXPLICIT_tf); break;
    }
}

//////////////////////////////
//
// HumdrumInput::convertVerses --
//

void HumdrumInput::convertVerses(Note *note, HTp token, int subtoken)
{
    int staff = m_rkern[token->getTrack()];
    vector<humaux::StaffStateVariables> &ss = m_staffstates;
    if (!ss[staff].verse) {
        return;
    }

    HumdrumLine &line = *token->getLine();
    int track = token->getTrack();
    int ttrack;
    int versenum = 0;
    int startfield = token->getFieldIndex() + 1;
    for (int i = startfield; i < line.getFieldCount(); i++) {
        if (line.token(i)->isKern()) {
            ttrack = line.token(i)->getTrack();
            if (ttrack != track) {
                break;
            }
        }
        else if (line.token(i)->isDataType("**text") || line.token(i)->isDataType("**silbe")) {
            versenum++;
            if (line.token(i)->isNull()) {
                continue;
            }
            Verse *verse = new Verse;
            setLocationId(verse, line.token(i), -1);
            appendElement(note, verse);
            verse->SetN(versenum);
            Syl *syl = new Syl;
            setLocationId(syl, line.token(i), -1);
            appendElement(verse, syl);
            string content = *line.token(i);
            bool dashbegin = false;
            bool dashend = false;
            bool extender = false;
            if (content.back() == '-') {
                dashend = true;
                content.pop_back();
            }
            if ((content.size() > 0) && (content[0] == '-')) {
                dashbegin = true;
                content.erase(0, 1);
            }
            if (content.back() == '_') {
                extender = true;
                content.pop_back();
            }
            if (dashbegin && dashend) {
                syl->SetWordpos(sylLog_WORDPOS_m);
                syl->SetCon(sylLog_CON_d);
            }
            else if (dashbegin) {
                syl->SetWordpos(sylLog_WORDPOS_t);
            }
            else if (dashend) {
                syl->SetWordpos(sylLog_WORDPOS_i);
                syl->SetCon(sylLog_CON_d);
            }
            else if (extender) {
                syl->SetWordpos(sylLog_WORDPOS_t);
                syl->SetCon(sylLog_CON_u);
            }
            // remove the last dash in a line (double dash which indicates
            // a dash in the original word separator).
            if ((!content.empty()) && content.back() == '-') {
                content.resize(content.size() - 1);
            }
            addTextElement(syl, content);
        }
    }
}

//////////////////////////////
//
// HumdrumInput::convertRhythm --
//     default value:
//         subtoken = -1;
//         isnote = true
//

template <class ELEMENT> HumNum HumdrumInput::convertRhythm(ELEMENT element, HTp token, int subtoken)
{
    string tstring;
    if (subtoken < 0) {
        tstring = *token;
    }
    else {
        tstring = token->getSubtoken(subtoken);
    }

    // Remove grace note information (for generating printed duration)
    bool grace = false;
    if (tstring.find('q') != string::npos) {
        grace = true;
        tstring.erase(std::remove(tstring.begin(), tstring.end(), 'q'), tstring.end());
    }

    int dotcount = characterCountInSubtoken(tstring, '.');
    if (dotcount > 0) {
        element->SetDots(dotcount);
    }

    // Tuplet durations are not handled below yet.
    // dur is in units of quarter notes.
    HumNum dur = Convert::recipToDurationNoDots(tstring);
    dur /= 4; // duration is now in whole note units;

    dur *= m_tupletscaling;

    if ((!grace) && (dur == 0) && (element)) {
        return 0;
        // duration should be set to "1" to make it look like
        // a stemless note with a quarter note duration to make
        // it a quarter notehead shape (maybe just set the notehead
        // shape to a quarter note).  But cannot set the stem
        // length in this function since non-notes can have
        // durations (rests and spaces), so the calling function
        // has to take character.
    }
    else if (dur.isInteger()) {
        switch (dur.getNumerator()) {
            case 1: element->SetDur(DURATION_1); break;
            case 2: element->SetDur(DURATION_breve); break;
            case 4: element->SetDur(DURATION_long); break;
            case 8: element->SetDur(DURATION_maxima); break;
        }
    }
    else if (dur.getNumerator() == 1) {
        switch (dur.getDenominator()) {
            case 2: element->SetDur(DURATION_2); break;
            case 4: element->SetDur(DURATION_4); break;
            case 8: element->SetDur(DURATION_8); break;
            case 16: element->SetDur(DURATION_16); break;
            case 32: element->SetDur(DURATION_32); break;
            case 64: element->SetDur(DURATION_64); break;
            case 128: element->SetDur(DURATION_128); break;
            case 256: element->SetDur(DURATION_256); break;
            case 512: element->SetDur(DURATION_512); break;
            case 1024: element->SetDur(DURATION_1024); break;
            case 2048: element->SetDur(DURATION_2048); break;
        }
    }
    return dur;
}

//////////////////////////////
//
// HumdrumInput::addFermata -- Add floating fermatas for note/chord.
//

void HumdrumInput::addFermata(HTp token)
{
    int layer = m_currentlayer;
    int staff = m_currentstaff;

    if (token->find(";") != string::npos) {
        if ((token->find("yy") == string::npos) && (token->find(";y") == string::npos)) {
            Fermata *fermata = new Fermata;
            appendElement(m_measure, fermata);
            setStaff(fermata, staff);
            HumNum tstamp = getMeasureTstamp(token, staff - 1);
            fermata->SetTstamp(tstamp.getFloat());

            if (layer == 1) {
                // note->SetFermata(PLACE_above);
                fermata->SetPlace(STAFFREL_above);
            }
            else if (layer == 2) {
                // note->SetFermata(PLACE_below);
                fermata->SetPlace(STAFFREL_below);
            }
            // else {
            //    // who knows, maybe check the stem direction or see
            //    // if another note in a different layer already
            //    // has a fermata (so you would not want to overwrite them).
            //    note->SetFermata(PLACE_above);
            //}
        }
    }
}

//////////////////////////////
//
// HumdrumInput::processTieStart --
//

void HumdrumInput::processTieStart(Note *note, HTp token, const string &tstring)
{
    vector<humaux::StaffStateVariables> &ss = m_staffstates;
    HumNum timestamp = token->getDurationFromStart();
    HumNum endtime = timestamp + token->getDuration();
    int track = token->getTrack();
    int rtrack = m_rkern[track];
    string noteuuid = note->GetUuid();
    int cl = m_currentlayer;
    int pitch = Convert::kernToMidiNoteNumber(tstring);

    ss[rtrack].ties.emplace_back();
    ss[rtrack].ties.back().setStart(noteuuid, m_measure, cl, tstring, pitch, timestamp, endtime);
}

//////////////////////////////
//
// processTieEnd --
//

void HumdrumInput::processTieEnd(Note *note, HTp token, const string &tstring)
{
    vector<humaux::StaffStateVariables> &ss = m_staffstates;
    HumNum timestamp = token->getDurationFromStart();
    int track = token->getTrack();
    int staffnum = m_rkern[track];
    string noteuuid = note->GetUuid();

    int pitch = Convert::kernToMidiNoteNumber(tstring);
    int layer = m_currentlayer;
    auto found = ss[staffnum].ties.end();

    // search for open tie in current layer
    for (auto it = ss[staffnum].ties.begin(); it != ss[staffnum].ties.end(); it++) {
        if (it->getLayer() != layer) {
            continue;
        }
        if (it->getPitch() != pitch) {
            continue;
        }
        if (it->getEndTime() == timestamp) {
            found = it;
            break;
        }
    }

    // search for open tie in current staff outside of current layer.
    if (found == ss[staffnum].ties.end()) {
        for (auto it = ss[staffnum].ties.begin(); it != ss[staffnum].ties.end(); it++) {
            if (it->getPitch() != pitch) {
                continue;
            }
            if (it->getEndTime() == timestamp) {
                found = it;
                break;
            }
        }
    }

    if (found == ss[staffnum].ties.end()) {
        // can't find start of slur so give up.
        return;
    }

    found->setEndAndInsert(noteuuid, m_measure, tstring);
    if (found->isInserted()) {
        // Only deleting the finished tie if it was successful.  Undeleted
        // ones can be checked later.  They are either encoding errors, or
        // hanging ties, or arpeggiation ties (the latter should be encoded
        // with [[, ]] rather than [, ]).
        ss[staffnum].ties.erase(found);
    }
}

/////////////////////////////
//
// HumdrumInput::characterCount --
//

int HumdrumInput::characterCount(const string &text, char symbol)
{
    return (int)std::count(text.begin(), text.end(), symbol);
}

int HumdrumInput::characterCount(HTp token, char symbol)
{
    return (int)std::count(token->begin(), token->end(), symbol);
}

/////////////////////////////
//
// HumdrumInput::characterCountInSubtoken --
//

int HumdrumInput::characterCountInSubtoken(const string &text, char symbol)
{
    int sum = 0;
    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] == symbol) {
            sum++;
        }
        if (text[i] == ' ') {
            break;
        }
    }
    return sum;
}

int HumdrumInput::characterCountInSubtoken(HTp token, char symbol)
{
    return characterCountInSubtoken(*token, symbol);
}

/////////////////////////////
//
// HumdrumInput::printMeasureTokens -- For debugging.
//

void HumdrumInput::printMeasureTokens(void)
{
    vector<vector<vector<hum::HTp> > > &lt = m_layertokens;
    int i, j, k;
    cerr << endl;
    for (i = 0; i < (int)lt.size(); i++) {
        cerr << "STAFF " << i + 1 << "\t";
        for (j = 0; j < (int)lt[i].size(); j++) {
            cerr << "LAYER " << j + 1 << ":\t";
            for (k = 0; k < (int)lt[i][j].size(); k++) {
                cout << " " << *lt[i][j][k];
            }
            cerr << endl;
        }
    }
}

/////////////////////////////
//
// HumdrumInput::setDuration --  Incoming duration is in quarter notes units.
//    This function needs to be generalized (and how to deal with tuplets)?
//    Mostly for <space> elements.
//

template <class ELEMENT> HumNum HumdrumInput::setDuration(ELEMENT element, HumNum duration)
{
    if (duration == 3) {
        element->SetDur(DURATION_2);
        element->SetDots(1);
        return duration;
    }
    else if (duration == 2) {
        element->SetDur(DURATION_2);
        return duration;
    }
    else if (duration == 1) {
        element->SetDur(DURATION_4);
        return duration;
    }
    else if (duration == 4) {
        element->SetDur(DURATION_1);
        return duration;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 2)) {
        element->SetDur(DURATION_8);
        return duration;
    }
    else if ((duration.getNumerator() == 3) && (duration.getDenominator() == 2)) {
        element->SetDur(DURATION_4);
        element->SetDots(1);
        return duration;
    }
    else if ((duration.getNumerator() == 3) && (duration.getDenominator() == 4)) {
        element->SetDur(DURATION_8);
        element->SetDots(1);
        return duration;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 4)) {
        element->SetDur(DURATION_16);
        return duration;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 8)) {
        element->SetDur(DURATION_32);
        return duration;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 16)) {
        element->SetDur(DURATION_64);
        return duration;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 32)) {
        element->SetDur(DURATION_128);
        return duration;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 64)) {
        element->SetDur(DURATION_256);
        return duration;
    }
    if (duration > 4) {
        element->SetDur(DURATION_1);
        return 4;
    }
    if (duration > 2) {
        element->SetDur(DURATION_2);
        return 2;
    }
    if (duration > 1) {
        element->SetDur(DURATION_4);
        return 1;
    }
    // Don't know what to do, so return duration
    // There will be an error in the data.
    cerr << "Unprintable rhythm: " << duration << endl;
    return duration;
}

//////////////////////////////
//
// HumdrumInput::getStaffLayerCounts -- Return the maximum layer count for each
//    part within the measure.
//

vector<int> HumdrumInput::getStaffLayerCounts(void)
{
    vector<vector<vector<hum::HTp> > > &lt = m_layertokens;
    vector<int> output(lt.size(), 0);

    int i;
    for (i = 0; i < (int)lt.size(); i++) {
        output[i] = (int)lt[i].size();
    }

    return output;
}

//////////////////////////////
//
// HumdrumInput::setupSystemMeasure -- prepare a new system measure.
//   Also checks if the key or time signatures change at the start
//   of the measures (other than the first measure of a score).
//

void HumdrumInput::setupSystemMeasure(int startline, int endline)
{
    if (m_oclef.size()) {
        storeOriginalClefApp();
    }

    if (m_infile[startline].getDurationFromStart() > 0) {
        addSystemKeyTimeChange(startline, endline);
    }

    m_measure = new Measure();
    if ((m_ending[startline] != 0) && (m_endingnum != m_ending[startline])) {
        // create a new ending
        m_currentending = new Ending;
        m_currentending->SetN(m_ending[startline]);
        m_sections.back()->AddChild(m_currentending);
        m_currentending->AddChild(m_measure);
    }
    else if (m_ending[startline]) {
        // inside a current ending
        m_currentending->AddChild(m_measure);
    }
    else {
        // outside of an ending
        m_sections.back()->AddChild(m_measure);
    }
    m_endingnum = m_ending[startline];
    m_measures.push_back(m_measure);

    if (m_leftbarstyle != BARRENDITION_NONE) {
        m_measure->SetLeft(m_leftbarstyle);
        m_leftbarstyle = BARRENDITION_NONE;
    }

    setLocationId(m_measure, startline, -1, -1);

    int measurenumber = getMeasureNumber(startline, endline);
    if (measurenumber >= 0) {
        m_measure->SetN(measurenumber);
    }

    setSystemMeasureStyle(startline, endline);
}

//////////////////////////////
//
// HumdrumInput::storeOriginalClefApp -- If there are any original clefs, create
//   an app for them.
//
// <app>
//    <lem></lem>
//    <rdg label="original">
//       <scoreDef>
//          <staffGrp>
//             <staffDef clef.shape="C" clef.line="4" n="1"/>
//          </staffGrp>
//       </scoreDef>
//    </rdg>
// </app>
//

void HumdrumInput::storeOriginalClefApp(void)
{
    if (m_oclef.empty()) {
        return;
    }

    App *app = new App;
    m_sections.back()->AddChild(app);

    Lem *lem = new Lem;
    app->AddChild(lem);

    Rdg *rdg = new Rdg;
    app->AddChild(rdg);
    rdg->SetLabel("original-clef");

    ScoreDef *scoredef = new ScoreDef;
    rdg->AddChild(scoredef);

    StaffGrp *staffgrp = new StaffGrp;
    scoredef->AddChild(staffgrp);

    for (int i = 0; i < (int)m_oclef.size(); i++) {
        StaffDef *staffdef = new StaffDef;
        staffgrp->AddChild(staffdef);
        setClef(staffdef, *m_oclef[i].second);
        staffdef->SetN(m_oclef[i].first);
    }

    m_oclef.clear();
}

//////////////////////////////
//
// HumdrumInput::setSystemMeasureStyle -- Set the style of the left and/or
//    right barline for the measure.
//
// Bar types listed in ../libmei/atttypes.h
//     BARRENDITION_NONE = 0,
//     BARRENDITION_dashed,
//     BARRENDITION_dotted,
//     BARRENDITION_dbl,
//     BARRENDITION_dbldashed,
//     BARRENDITION_dbldotted,
//     BARRENDITION_end,
//     BARRENDITION_invis,
//     BARRENDITION_rptstart,
//     BARRENDITION_rptboth,
//     BARRENDITION_rptend,
//     BARRENDITION_single,
// See also:
//      http://music-encoding.org/documentation/2.1.1/data.BARRENDITION
//      http://music-encoding.org/guidelines/3.0.0/Images/ExampleImages/barline-20100510.png
//

void HumdrumInput::setSystemMeasureStyle(int startline, int endline)
{
    HumdrumFile &infile = m_infile;

    string endbar = infile[endline].getTokenString(0);
    string startbar = infile[startline].getTokenString(0);
    if (endbar.compare(0, 2, "==") == 0) {
        m_measure->SetRight(BARRENDITION_end);
    }
    else if (endbar.find(":|!|:") != string::npos) {
        // m_measure->SetRight(BARRENDITION_rptboth);
        m_measure->SetRight(BARRENDITION_rptend);
        setNextLeftBarStyle(BARRENDITION_rptstart);
    }
    else if (endbar.find(":!!:") != string::npos) {
        // m_measure->SetRight(BARRENDITION_rptboth);
        m_measure->SetRight(BARRENDITION_rptend);
        setNextLeftBarStyle(BARRENDITION_rptstart);
    }
    else if (endbar.find(":||:") != string::npos) {
        // m_measure->SetRight(BARRENDITION_rptboth);
        m_measure->SetRight(BARRENDITION_rptend);
        setNextLeftBarStyle(BARRENDITION_rptstart);
    }
    else if (endbar.find(":!:") != string::npos) {
        // m_measure->SetRight(BARRENDITION_rptboth);
        m_measure->SetRight(BARRENDITION_rptend);
        setNextLeftBarStyle(BARRENDITION_rptstart);
    }
    else if (endbar.find(":|:") != string::npos) {
        // m_measure->SetRight(BARRENDITION_rptboth);
        m_measure->SetRight(BARRENDITION_rptend);
        setNextLeftBarStyle(BARRENDITION_rptstart);
    }
    else if (endbar.find(":|") != string::npos) {
        m_measure->SetRight(BARRENDITION_rptend);
    }
    else if (endbar.find(":!") != string::npos) {
        m_measure->SetRight(BARRENDITION_rptend);
    }
    else if (startbar.find("!:") != string::npos) {
        // m_measure->SetLeft(BARRENDITION_rptstart);
        setNextLeftBarStyle(BARRENDITION_rptstart);
    }
    else if (endbar.find("|:") != string::npos) {
        // m_measure->SetLeft(BARRENDITION_rptstart);
        setNextLeftBarStyle(BARRENDITION_rptstart);
    }
    else if (endbar.find("||") != string::npos) {
        m_measure->SetRight(BARRENDITION_dbl);
        // setNextLeftBarStyle(BARRENDITION_dbl);
    }
    else if (endbar.find("-") != string::npos) {
        m_measure->SetRight(BARRENDITION_invis);
    }
}

//////////////////////////////
//
// HumdrumInput::setNextLeftBarStyle -- Store a bar style to apply
//      on the left side of the next measure.
//

void HumdrumInput::setNextLeftBarStyle(data_BARRENDITION style)
{
    m_leftbarstyle = style;
}

//////////////////////////////
//
// HumdrumInput::getMeasureEndLine -- Return the line index of the
//   ending of a given measure.  This is usually a barline, but can be
//   the end of a file if there is no terminal barline in the **kern
//   data.  Returns a negative line number if there is no data in the
//   measure.
//

int HumdrumInput::getMeasureEndLine(int startline)
{
    HumdrumFile &infile = m_infile;
    int endline = infile.getLineCount() - 1;
    bool foundDataQ = false;
    int i = startline + 1;
    while (i < infile.getLineCount()) {
        if (infile[i].isData()) {
            foundDataQ = true;
        }
        else if (infile[i].isBarline()) {
            endline = i;
            break;
        }
        endline = i;
        i++;
    }

    if (foundDataQ) {
        return endline;
    }
    else {
        return -endline;
    }
}

//////////////////////////////
//
// HumdrumInput::setupMeiDocument -- Add a page and a system on the page to
//     get things started.
//

void HumdrumInput::setupMeiDocument(void)
{
    m_score = m_doc->CreateScoreBuffer();

    Section *section = new Section();
    m_sections.push_back(section);
    m_score->AddChild(m_sections.back());
    m_leftbarstyle = BARRENDITION_NONE;
}

//////////////////////////////
//
// HumdrumInput::clear -- clear contents of object (m_doc handled by
//    parent class).
//

void HumdrumInput::clear(void)
{
    m_filename = "";
    m_tupletscaling = 1;
}

//////////////////////////////
//
// HumdrumInput::getMeasureNumber -- Return the current barline's measure
//     number, or return -1 if no measure number.  Returns 0 if a
//     pickup measure.
//

int HumdrumInput::getMeasureNumber(int startline, int endline)
{
    HumdrumFile &infile = m_infile;

    if (infile[startline].getTokenString(0).compare(0, 2, "**") == 0) {
        // create HumdrumFile.hasPickup() function and uncomment out below:
        // if (infile.hasPickup()) {
        //    return 1;
        // } else {
        return 0;
        // }
    }

    int number;
    if (sscanf(infile[startline].getTokenString(0).c_str(), "=%d", &number) == 1) {
        return number;
    }
    else {
        return -1;
    }
}

//////////////////////////////
//
// HumdrumInput::calculateLayout -- Have verovio figure out the
//     line break (and page break) locations in the data.  Do this
//     after the Humdrum data has finished loading.
//

void HumdrumInput::calculateLayout(void)
{
    // m_doc->CastOff();
}

//////////////////////////////
//
// HumdrumInput::emptyMeasures -- For initial development, maybe convert to
//     an option.
//

bool HumdrumInput::emptyMeasures(void)
{
    return false;
}

//////////////////////////////
//
// HumdrumInput::unescapeHtmlEntities -- from
//    http://www.codecodex.com/wiki/Unescape_HTML_special_characters_from_a_String
//

typedef struct {
    const char *Name;
    unsigned int Value;
} EntityNameEntry;

// list of entity names defined in HTML 4.0 spec
static const EntityNameEntry StaticEntityNames[] = {
    { "nbsp", 160 }, { "iexcl", 161 }, { "cent", 162 }, { "pound", 163 }, { "curren", 164 }, { "yen", 165 },
    { "brvbar", 166 }, { "sect", 167 }, { "uml", 168 }, { "copy", 169 }, { "ordf", 170 }, { "laquo", 171 },
    { "not", 172 }, { "shy", 173 }, { "reg", 174 }, { "macr", 175 }, { "deg", 176 }, { "plusmn", 177 }, { "sup2", 178 },
    { "sup3", 179 }, { "acute", 180 }, { "micro", 181 }, { "para", 182 }, { "middot", 183 }, { "cedil", 184 },
    { "sup1", 185 }, { "ordm", 186 }, { "raquo", 187 }, { "frac14", 188 }, { "frac12", 189 }, { "frac34", 190 },
    { "iquest", 191 }, { "Agrave", 192 }, { "Aacute", 193 }, { "Acirc", 194 }, { "Atilde", 195 }, { "Auml", 196 },
    { "Aring", 197 }, { "AElig", 198 }, { "Ccedil", 199 }, { "Egrave", 200 }, { "Eacute", 201 }, { "Ecirc", 202 },
    { "Euml", 203 }, { "Igrave", 204 }, { "Iacute", 205 }, { "Icirc", 206 }, { "Iuml", 207 }, { "ETH", 208 },
    { "Ntilde", 209 }, { "Ograve", 210 }, { "Oacute", 211 }, { "Ocirc", 212 }, { "Otilde", 213 }, { "Ouml", 214 },
    { "times", 215 }, { "Oslash", 216 }, { "Ugrave", 217 }, { "Uacute", 218 }, { "Ucirc", 219 }, { "Uuml", 220 },
    { "Yacute", 221 }, { "THORN", 222 }, { "szlig", 223 }, { "agrave", 224 }, { "aacute", 225 }, { "acirc", 226 },
    { "atilde", 227 }, { "auml", 228 }, { "aring", 229 }, { "aelig", 230 }, { "ccedil", 231 }, { "egrave", 232 },
    { "eacute", 233 }, { "ecirc", 234 }, { "euml", 235 }, { "igrave", 236 }, { "iacute", 237 }, { "icirc", 238 },
    { "iuml", 239 }, { "eth", 240 }, { "ntilde", 241 }, { "ograve", 242 }, { "oacute", 243 }, { "ocirc", 244 },
    { "otilde", 245 }, { "ouml", 246 }, { "divide", 247 }, { "oslash", 248 }, { "ugrave", 249 }, { "uacute", 250 },
    { "ucirc", 251 }, { "uuml", 252 }, { "yacute", 253 }, { "thorn", 254 }, { "yuml", 255 }, { "fnof", 402 },
    /* Greek */ { "Alpha", 913 }, { "Beta", 914 }, { "Gamma", 915 }, { "Delta", 916 }, { "Epsilon", 917 },
    { "Zeta", 918 }, { "Eta", 919 }, { "Theta", 920 }, { "Iota", 921 }, { "Kappa", 922 }, { "Lambda", 923 },
    { "Mu", 924 }, { "Nu", 925 }, { "Xi", 926 }, { "Omicron", 927 }, { "Pi", 928 }, { "Rho", 929 }, { "Sigma", 931 },
    { "Tau", 932 }, { "Upsilon", 933 }, { "Phi", 934 }, { "Chi", 935 }, { "Psi", 936 }, { "Omega", 937 },
    { "alpha", 945 }, { "beta", 946 }, { "gamma", 947 }, { "delta", 948 }, { "epsilon", 949 }, { "zeta", 950 },
    { "eta", 951 }, { "theta", 952 }, { "iota", 953 }, { "kappa", 954 }, { "lambda", 955 }, { "mu", 956 },
    { "nu", 957 }, { "xi", 958 }, { "omicron", 959 }, { "pi", 960 }, { "rho", 961 }, { "sigmaf", 962 },
    { "sigma", 963 }, { "tau", 964 }, { "upsilon", 965 }, { "phi", 966 }, { "chi", 967 }, { "psi", 968 },
    { "omega", 969 }, { "thetasym", 977 }, { "upsih", 978 }, { "piv", 982 },
    /* General Punctuation */
    { "bull", 8226 }, { "hellip", 8230 }, { "prime", 8242 }, { "Prime", 8243 }, { "oline", 8254 }, { "frasl", 8260 },
    /* Letterlike Symbols */
    { "weierp", 8472 }, { "image", 8465 }, { "real", 8476 }, { "trade", 8482 }, { "alefsym", 8501 },
    /* Arrows */
    { "larr", 8592 }, { "uarr", 8593 }, { "rarr", 8594 }, { "darr", 8595 }, { "harr", 8596 }, { "crarr", 8629 },
    { "lArr", 8656 }, { "uArr", 8657 }, { "rArr", 8658 }, { "dArr", 8659 }, { "hArr", 8660 },
    /* Mathematical Operators */
    { "forall", 8704 }, { "part", 8706 }, { "exist", 8707 }, { "empty", 8709 }, { "nabla", 8711 }, { "isin", 8712 },
    { "notin", 8713 }, { "ni", 8715 }, { "prod", 8719 }, { "sum", 8721 }, { "minus", 8722 }, { "lowast", 8727 },
    { "radic", 8730 }, { "prop", 8733 }, { "infin", 8734 }, { "and", 8743 }, { "or", 8744 }, { "cap", 8745 },
    { "cup", 8746 }, { "int", 8747 }, { "there4", 8756 }, { "sim", 8764 }, { "cong", 8773 }, { "asymp", 8776 },
    { "ne", 8800 }, { "equiv", 8801 }, { "le", 8804 }, { "ge", 8805 }, { "sub", 8834 }, { "sup", 8835 },
    { "nsub", 8836 }, { "sube", 8838 }, { "supe", 8839 }, { "oplus", 8853 }, { "otimes", 8855 }, { "perp", 8869 },
    { "sdot", 8901 },
    /* Miscellaneous Technical */
    { "lceil", 8968 }, { "rceil", 8969 }, { "lfloor", 8970 }, { "rfloor", 8971 }, { "lang", 9001 }, { "rang", 9002 },
    /* Geometric Shapes */
    { "loz", 9674 },
    /* Miscellaneous Symbols */
    { "spades", 9824 }, { "clubs", 9827 }, { "hearts", 9829 }, { "diams", 9830 }, { "quot", 34 }, { "amp", 38 },
    { "lt", 60 }, { "gt", 62 },
    /* Latin Extended-A */
    { "OElig", 338 }, { "oelig", 339 }, { "Scaron", 352 }, { "scaron", 353 }, { "Yuml", 376 },
    /* Spacing Modifier Letters */
    { "circ", 710 }, { "tilde", 732 },
    /* General Punctuation */
    { "ensp", 8194 }, { "emsp", 8195 }, { "thinsp", 8201 }, { "zwnj", 8204 }, { "zwj", 8205 }, { "lrm", 8206 },
    { "rlm", 8207 }, { "ndash", 8211 }, { "mdash", 8212 }, { "lsquo", 8216 }, { "rsquo", 8217 }, { "sbquo", 8218 },
    { "ldquo", 8220 }, { "rdquo", 8221 }, { "bdquo", 8222 }, { "dagger", 8224 }, { "Dagger", 8225 }, { "permil", 8240 },
    { "lsaquo", 8249 }, { "rsaquo", 8250 }, { "euro", 8364 }, { NULL, 0 } /* marks end of list */
} /*StaticEntityNames*/;

typedef std::map<std::string, unsigned int> EntityNameMap;
typedef std::pair<std::string, unsigned int> EntityNamePair;
static EntityNameMap EntityNames;

//////////////////////////////
//
// HumdrumInput::unescapeHtmlEntities --
//

string HumdrumInput::unescapeHtmlEntities(const string &input)
{
    stringstream indata;
    indata << input;
    stringstream outdata;
    UnquoteHTML(indata, outdata);
    return outdata.str();
}

//////////////////////////////
//
// HumdrumInput::WriteUTF8 -- writes Ch in UTF-8 encoding to Out.
//     Note this version only deals with characters up to 16 bits.
//

void HumdrumInput::WriteUTF8(std::ostream &Out, unsigned int Ch)
{
    if (Ch >= 0x800) {
        Out.put(0xE0 | ((Ch >> 12) & 0x0F));
        Out.put(0x80 | ((Ch >> 6) & 0x3F));
        Out.put(0x80 | (Ch & 0x3F));
    }
    else if (Ch >= 0x80) {
        Out.put(0xC0 | ((Ch >> 6) & 0x1F));
        Out.put(0x80 | (Ch & 0x3F));
    }
    else {
        Out.put(Ch);
    } /*if*/
} /*WriteUTF8*/

//////////////////////////////
//
// HumdrumInput::UnquoteHTML -- Copies In to Out, expanding any HTML entity
//     references into literal UTF-8 characters.
//

void HumdrumInput::UnquoteHTML(std::istream &In, std::ostream &Out)
{
    enum {
        NoMatch,
        MatchBegin,
        MatchName,
        MatchNumber,
        MatchDecimalNumber,
        MatchHexNumber,
    } MatchState;
    std::string MatchingName;
    unsigned int CharCode = 0;
    bool ProcessedChar, GotCharCode;
    MatchState = NoMatch;
    for (;;) {
        const unsigned char ThisCh = In.get();
        if (In.eof()) break;
        ProcessedChar = false; /* to begin with */
        GotCharCode = false; /* to begin with */
        switch ((int)MatchState) {
            case (int)MatchBegin:
                if (ThisCh == '#') {
                    MatchState = MatchNumber;
                    ProcessedChar = true;
                }
                else if ((ThisCh >= 'a' && ThisCh <= 'z') || (ThisCh >= 'A' && ThisCh <= 'Z')) {
                    MatchingName.append(1, ThisCh);
                    MatchState = MatchName;
                    ProcessedChar = true;
                }
                else {
                    Out.put('&');
                    MatchState = NoMatch;
                }
                break;
            case (int)MatchName:
                if ((ThisCh >= 'a' && ThisCh <= 'z') || (ThisCh >= 'A' && ThisCh <= 'Z')
                    || (ThisCh >= '0' && ThisCh <= '9')) {
                    MatchingName.append(1, ThisCh);
                    ProcessedChar = true;
                }
                else if (ThisCh == ';') {
                    if (EntityNames.empty()) {
                        /* first use, load EntityNames from StaticEntityNames */
                        const EntityNameEntry *ThisEntry;
                        ThisEntry = StaticEntityNames;
                        for (;;) {
                            if (ThisEntry->Name == NULL) break;
                            EntityNames.insert(EntityNamePair(std::string(ThisEntry->Name), ThisEntry->Value));
                            ++ThisEntry;
                        }
                    }
                    const EntityNameMap::const_iterator NameEntry = EntityNames.find(MatchingName);
                    if (NameEntry != EntityNames.end()) {
                        CharCode = NameEntry->second;
                        ProcessedChar = true;
                        GotCharCode = true;
                    }
                }
                if (!ProcessedChar) {
                    Out.put('&');
                    for (unsigned int i = 0; i < MatchingName.size(); ++i) {
                        Out.put(MatchingName[i]);
                    } /*for*/
                    MatchState = NoMatch;
                }
                break;
            case (int)MatchNumber:
                if (ThisCh == 'x' || ThisCh == 'X') {
                    ProcessedChar = true;
                    MatchState = MatchHexNumber;
                    CharCode = 0;
                }
                else if (ThisCh >= '0' && ThisCh <= '9') {
                    CharCode = ThisCh - '0';
                    MatchState = MatchDecimalNumber;
                    ProcessedChar = true;
                }
                else {
                    MatchState = NoMatch;
                }
                break;
            case (int)MatchDecimalNumber:
                if (ThisCh >= '0' && ThisCh <= '9') {
                    CharCode = CharCode * 10 + ThisCh - '0';
                    ProcessedChar = true;
                }
                else if (ThisCh == ';') {
                    ProcessedChar = true;
                    GotCharCode = true;
                }
                else {
                    MatchState = NoMatch;
                }
                break;
            case (int)MatchHexNumber:
                if (ThisCh >= '0' && ThisCh <= '9') {
                    CharCode = CharCode * 16 + ThisCh - '0';
                    ProcessedChar = true;
                }
                else if (ThisCh >= 'a' && ThisCh <= 'f') {
                    CharCode = CharCode * 16 + ThisCh - 'a' + 10;
                    ProcessedChar = true;
                }
                else if (ThisCh >= 'A' && ThisCh <= 'F') {
                    CharCode = CharCode * 16 + ThisCh - 'A' + 10;
                    ProcessedChar = true;
                }
                else if (ThisCh == ';') {
                    ProcessedChar = true;
                    GotCharCode = true;
                }
                else {
                    MatchState = NoMatch;
                }
                break;
        }
        if (GotCharCode) {
            WriteUTF8(Out, CharCode);
            MatchState = NoMatch;
        }
        else if (!ProcessedChar && MatchState == NoMatch) {
            if (ThisCh == '&') {
                MatchState = MatchBegin;
                MatchingName.erase();
            }
            else {
                Out.put(ThisCh);
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::GetMeiString -- Return the converted MEI content.
//

std::string HumdrumInput::GetMeiString(void)
{
    MeiOutput meioutput(m_doc, "");
    meioutput.SetScoreBasedMEI(true);
    return meioutput.GetOutput();
}

//////////////////////////////
//
// HumdrumInput::setLocationId -- use the file location of the item
//    for the ID.
//

void HumdrumInput::setLocationId(Object *object, HTp token, int subtoken)
{
    int line = token->getLineIndex() + 1;
    int field = token->getFieldIndex() + 1;
    string id = object->GetClassName();
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
    id += "-L" + to_string(line);
    id += "F" + to_string(field);
    if (subtoken >= 0) {
        id += "S" + to_string(subtoken + 1);
    }
    object->SetUuid(id);
}

void HumdrumInput::setLocationId(Object *object, int lineindex, int fieldindex, int subtokenindex)
{
    int line = lineindex + 1;
    int field = fieldindex + 1;
    int subtoken = subtokenindex + 1;
    string id = object->GetClassName();
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
    if (line > 0) {
        id += "-L" + to_string(line);
    }
    if (field > 0) {
        id += "F" + to_string(field);
    }
    if (subtoken > 0) {
        id += "S" + to_string(subtoken);
    }
    object->SetUuid(id);
}

//////////////////////////////
//
// HumdrumInput::parseSignifiers -- search for !!!RDF records which
//     define special purpose notational features in Humdrum data spines.
//     Currently recognized ones are:
// !!!RDF**kern: i = no stem
// only single-character signifiers are allowed (could be made a string)
//

void HumdrumInput::parseSignifiers(HumdrumFile &infile)
{
    HumRegex hre;
    vector<HumdrumLine *> refs = infile.getReferenceRecords();
    for (int i = 0; i < (int)refs.size(); i++) {
        string key = refs[i]->getReferenceKey();
        if (key != "RDF**kern") {
            continue;
        }
        string value = refs[i]->getReferenceValue();
        auto equals = value.find('=');
        if (equals == string::npos) {
            continue;
        }
        char signifier = 0;
        for (int j = 0; j < equals; j++) {
            if (isspace(value[j])) {
                continue;
            }
            signifier = value[j];
            break;
        }
        if (!signifier) {
            continue;
        }
        // check for known signifier meanings:

        // stemless note:
        // !!!RDF**kern: i = no stem
        if (value.find("no stem", equals) != string::npos) {
            m_signifiers.nostem = signifier;
            continue;
        }

        // slur directions
        if (value.find("slur above", equals) != string::npos) {
            m_signifiers.slurabove = signifier;
            continue;
        }
        if (value.find("slur below", equals) != string::npos) {
            m_signifiers.slurbelow = signifier;
            continue;
        }

        // editorial accidentals:
        if (value.find("editorial accidental", equals) != string::npos) {
            m_signifiers.editacc = signifier;
            continue;
        }

        // colored notes
        // !!!RDF**kern: i = marked note, color="#ff0000"
        // !!!RDF**kern: i = matched note, color=blue
        if (hre.search(value, "color\\s*=\\s*\"?([^\"\\s]+)\"?")) {
            m_signifiers.mark.push_back(signifier);
            m_signifiers.mcolor.push_back(hre.getMatch(1));
        }
        else if (hre.search(value, "marked note|matched note")) {
            m_signifiers.mark.push_back(signifier);
            m_signifiers.mcolor.push_back("red");
        }
    }
}

//////////////////////////////
//
// HumdrumInput::analyzeMultiRest --
//

vector<int> HumdrumInput::analyzeMultiRest(HumdrumFile &infile)
{
    vector<int> barindex(1, 0); // line number of barline
    vector<int> datacount(1, 0);
    vector<int> dataline(1, 0);
    vector<HumNum> bardur(1, 0);

    // count number of non-null data lines for each measure.
    for (int i = 0; i < infile.getLineCount(); i++) {
        if (infile[i].isData()) {
            if (!infile[i].isAllNull()) {
                datacount.back() = datacount.back() + 1;
                if (datacount.back() == 1) {
                    dataline.back() = i;
                }
            }
            else {
                // line is all null tokens.
            }
        }
        if (!infile[i].isBarline()) {
            continue;
        }
        barindex.push_back(i);
        datacount.push_back(0);
        dataline.push_back(0);
        bardur.push_back(infile[i].getDurationToBarline());
    }

    // check to see if measures with single data item is a rest.
    vector<int> wholerest(barindex.size(), 0);
    bool restQ;
    int line;
    for (int i = 0; i < barindex.size(); i++) {
        if (datacount[i] == 1) {
            restQ = true;
            line = dataline[i];
            for (int j = 0; j < infile[line].getFieldCount(); j++) {
                if (!infile.token(line, j)->isKern()) {
                    continue;
                }
                if (!infile.token(line, j)->isRest()) {
                    restQ = false;
                    break;
                }
            }
            if (restQ) {
                wholerest[i] = 1;
            }
        }
    }

    for (int i = (int)wholerest.size() - 2; i >= 0; i--) {
        if (bardur[i] != bardur[i + 1]) {
            continue;
        }
        if (regex_search(*infile.token(barindex[i + 1], 0), regex("[^=0-9]"))) {
            continue;
        }
        if (wholerest[i] && wholerest[i + 1]) {
            wholerest[i] += wholerest[i + 1];
            wholerest[i + 1] = -1;
        }
    }

    vector<int> output(infile.getLineCount(), 0);
    for (int i = 0; i < wholerest.size(); i++) {
        output[dataline[i]] = wholerest[i];
    }
    for (int i = infile.getLineCount() - 2; i >= 0; i--) {
        if (!infile[i + 1].isBarline()) {
            if (output[i + 1] != 0) {
                output[i] = output[i + 1];
            }
        }
    }

    // for (int i = 0; i < infile.getLineCount(); i++) {
    //     cout << infile[i] << "\t" << output[i] << "\n";
    // }
    // Example analysis, with measure 4 staring a rest with num="6".
    // Measures 5-9 marked as whole-measure rests which will be merged into
    // the multi rest.
    //	**kern	**kern	0
    //	*M4/4	*M4/4	0
    //	=1-	=1-	0
    //	1c	1d	0
    //	=2	=2	0
    //	1d	1g	0
    //	=3	=3	0
    //	1r	1e	0
    //	=4	=4	6
    //	1r	1r	6
    //	=5	=5	-1
    //	1r	1r	-1
    //	=6	=6	-1
    //	1r	1r	-1
    //	=7	=7	-1
    //	1r	1r	-1
    //	=8	=8	-1
    //	1r	1r	-1
    //	=9	=9	-1
    //	1r	1r	-1
    //	=10	=10	0
    //	1f	1r	0
    //	=11	=11	0
    //	1g	1g	0
    //	=12	=12	0
    //	1a	1g	0
    //	==	==	0
    //	*-	*-	0

    return output;
}

//////////////////////////////
//
// HumdrumInput::prepareEndings --
//

void HumdrumInput::prepareEndings(void)
{
    vector<int> &ending = m_ending;
    HumdrumFile &infile = m_infile;

    ending.resize(infile.getLineCount());
    std::fill(ending.begin(), ending.end(), 0);
    int endnum = 0;

    for (int i = 0; i < infile.getLineCount(); i++) {
        ending[i] = endnum;
        if (!infile[i].isInterpretation()) {
            continue;
        }
        if (infile.token(i, 0)->compare(0, 2, "*>") != 0) {
            continue;
        }
        if (infile.token(i, 0)->find("[") != string::npos) {
            // ignore expansion lists
            continue;
        }
        smatch matches;
        if (regex_search(*((string *)infile.token(i, 0)), matches, regex("(\\d+)$"))) {
            endnum = stoi(matches[1]);
            ending[i] = endnum;
        }
        else {
            endnum = 0;
            ending[i] = endnum;
        }
        for (int j = i - 1; j >= 0; j--) {
            if (infile[j].isData()) {
                break;
            }
            ending[j] = ending[i];
        }
    }
}

#endif /* NO_HUMDRUM_SUPPORT */

} // namespace vrv
