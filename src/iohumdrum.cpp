////////////////////////////////////////////////////////////////////////////
// Name:        iohumdrum.cpp
// Author:      Craig Stuart Sapp
// Created:     06/06/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////
//
// References:
//    http://humlib.humdrum.org
//    http://music-encoding.org/support/tutorials/mei-1st
//    http://music-encoding.org/support/guidelines
//    http://music-encoding.org/documentation/3.0.0/elements
//    http://music-encoding.org/documentation/3.0.0/atts
//    http://pugixml.org/docs/manual.html
//

#include "iohumdrum.h"
#include "iomei.h"

//----------------------------------------------------------------------------

#ifndef NO_HUMDRUM_SUPPORT

#include <algorithm>
#include <assert.h>
#include <cctype>
#include <cmath>
#include <locale>
#include <regex>
#include <sstream>
#include <vector>

#endif /* NO_HUMDRUM_SUPPORT */

//----------------------------------------------------------------------------

#include "doc.h"

#ifndef NO_HUMDRUM_SUPPORT

#include "accid.h"
#include "app.h"
#include "arpeg.h"
#include "artic.h"
#include "att.h"
#include "beam.h"
#include "bracketspan.h"
#include "breath.h"
#include "btrem.h"
#include "chord.h"
#include "dir.h"
#include "dot.h"
#include "dynam.h"
#include "editorial.h"
#include "ending.h"
#include "expansion.h"
#include "f.h"
#include "fb.h"
#include "fermata.h"
#include "fig.h"
#include "ftrem.h"
#include "hairpin.h"
#include "harm.h"
#include "instrdef.h"
#include "iomei.h"
#include "keyaccid.h"
#include "keysig.h"
#include "label.h"
#include "labelabbr.h"
#include "layer.h"
#include "lb.h"
#include "lem.h"
#include "ligature.h"
#include "mdiv.h"
#include "measure.h"
#include "mordent.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "num.h"
#include "octave.h"
#include "page.h"
#include "pb.h"
#include "pedal.h"
#include "pghead.h"
#include "rdg.h"
#include "rend.h"
#include "rest.h"
#include "sb.h"
#include "score.h"
#include "section.h"
#include "slur.h"
#include "space.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "supplied.h"
#include "syl.h"
#include "system.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "trill.h"
#include "tuplet.h"
#include "turn.h"
#include "verse.h"
#include "vrv.h"
//#include "attcomparison.h"

#endif /* NO_HUMDRUM_SUPPORT */

namespace vrv {

#ifndef NO_HUMDRUM_SUPPORT

#define VGRP_DYNAM_DEFAULT 100
#define VGRP_PEDAL_DEFAULT 200

//----------------------------------------------------------------------------
// namespace for local IoHumdrum classes
//----------------------------------------------------------------------------

namespace humaux {

    /////////////////////////////////////////////////////////////////////

    HumdrumTie::HumdrumTie()
    {
        m_endmeasure = m_startmeasure = NULL;
        m_inserted = false;
        m_above = false;
        m_below = false;
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
        m_above = anothertie.m_above;
        m_below = anothertie.m_below;
        m_startid = anothertie.m_startid;
        m_endid = anothertie.m_endid;
        m_startmeasure = anothertie.m_startmeasure;
        m_endmeasure = anothertie.m_endmeasure;
        m_pitch = anothertie.m_pitch;
        m_layer = anothertie.m_layer;
    }

    HumdrumTie::~HumdrumTie() { clear(); }

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
        m_above = anothertie.m_above;
        m_below = anothertie.m_below;
        m_startid = anothertie.m_startid;
        m_endid = anothertie.m_endid;
        m_startmeasure = anothertie.m_startmeasure;
        m_endmeasure = anothertie.m_endmeasure;
        m_pitch = anothertie.m_pitch;
        m_layer = anothertie.m_layer;
        return *this;
    }

    void HumdrumTie::clear()
    {
        m_endmeasure = m_startmeasure = NULL;
        m_inserted = false;
        m_above = false;
        m_below = false;
        m_startid.clear();
        m_endid.clear();
    }

    void HumdrumTie::setTieAbove() { m_above = true; }

    void HumdrumTie::setTieBelow() { m_below = true; }

    Tie *HumdrumTie::insertTieIntoDom()
    {
        if (m_inserted) {
            // don't insert again
            return NULL;
        }
        if ((m_startmeasure == NULL) && (m_endmeasure == NULL)) {
            // What are you trying to do?
            return NULL;
        }
        if (m_startmeasure == NULL) {
            // This is a tie with no start.  Don't know what to do with this
            // for now (but is possible due to repeated music).
            return NULL;
        }
        if (m_endmeasure == NULL) {
            // This is a tie with no end.  Don't know what to do with this
            // for now (but is possible due to repeated music).
            return NULL;
        }

        vrv::Tie *tie = new vrv::Tie;
        tie->SetStartid("#" + m_startid);
        tie->SetEndid("#" + m_endid);

        if (m_above) {
            tie->SetCurvedir(curvature_CURVEDIR_above);
        }
        if (m_below) {
            tie->SetCurvedir(curvature_CURVEDIR_below);
        }

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

        return tie;
    }

    void HumdrumTie::setStart(const std::string &id, Measure *starting, int layer, const std::string &token, int pitch,
        hum::HumNum starttime, hum::HumNum endtime, int subindex, hum::HTp starttok)
    {
        m_starttoken = token;
        m_starttime = starttime;
        m_endtime = endtime;
        m_pitch = pitch;
        m_layer = layer;
        m_startmeasure = starting;
        m_startid = id;
        m_subindex = subindex;
        m_starttokenpointer = starttok;
    }

    void HumdrumTie::setEnd(const std::string &id, Measure *ending, const std::string &token)
    {
        m_endtoken = token;
        m_endmeasure = ending;
        m_endid = id;
    }

    Tie *HumdrumTie::setEndAndInsert(const std::string &id, Measure *ending, const std::string &token)
    {
        setEnd(id, ending, token);
        return insertTieIntoDom();
    }

    bool HumdrumTie::isInserted() { return m_inserted; }

    int HumdrumTie::getPitch() { return m_pitch; }

    int HumdrumTie::getLayer() { return m_layer; }

    hum::HumNum HumdrumTie::getStartTime() { return m_starttime; }

    hum::HumNum HumdrumTie::getEndTime() { return m_endtime; }

    hum::HumNum HumdrumTie::getDuration() { return m_endtime - m_starttime; }

    std::string HumdrumTie::getStartToken() { return m_starttoken; }

    hum::HTp HumdrumTie::getStartTokenPointer() { return m_starttokenpointer; }

    int HumdrumTie::getStartSubindex() { return m_subindex; }

    std::string HumdrumTie::getEndToken() { return m_endtoken; }

    /////////////////////////////////////////////////////////////////////

    StaffStateVariables::StaffStateVariables()
    {
        cue_size.resize(100);
        stem_type.resize(100);
        clear();
    }
    StaffStateVariables::~StaffStateVariables() { clear(); }
    void StaffStateVariables::clear()
    {
        verse = false;
        suppress_beam_tuplet = false;
        suppress_bracket_tuplet = false;
        tremolo = false;
        righthalfstem = false;

        ottavanotestart = ottavanoteend = NULL;
        ottavaendtimestamp = 0;
        ottavameasure = NULL;

        ottavadownnotestart = ottavadownnoteend = NULL;
        ottavadownendtimestamp = 0;
        ottavadownmeasure = NULL;

        ottava2notestart = ottava2noteend = NULL;
        ottava2endtimestamp = 0;
        ottava2measure = NULL;

        ottava2downnotestart = ottava2downnoteend = NULL;
        ottava2downendtimestamp = 0;
        ottava2downmeasure = NULL;

        acclev = 1;
        last_clef = "";

        ties.clear();
        meter_bottom = 4;
        meter_top = 4;
        std::fill(cue_size.begin(), cue_size.end(), false);
        std::fill(stem_type.begin(), stem_type.end(), 'X');
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
    m_placement.resize(1000);
    std::fill(m_placement.begin(), m_placement.end(), 0);
    m_reverse.resize(1000);
    std::fill(m_reverse.begin(), m_reverse.end(), 0);
    m_absolute.resize(1000);
    std::fill(m_absolute.begin(), m_absolute.end(), 0);
    m_slash.resize(1000);
    std::fill(m_slash.begin(), m_slash.end(), 1);
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
        hum::HumdrumFileSet &infiles = m_infiles;
        bool result = infiles.readFile(m_filename);
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
// HumdrumInput::ImportString -- Read a Humdrum file set from a text string.
//

bool HumdrumInput::ImportString(const std::string &content)
{

#ifndef NO_HUMDRUM_SUPPORT
    try {
        m_doc->Reset();

        // Auto-detect CSV Humdrum file. Maybe later move to the humlib parser.
        std::string exinterp;
        bool found = false;
        int comma = 0;
        int tab = 0;
        for (int i = 0; i < (int)content.size() - 3; ++i) {
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
        if (comma <= tab) {
            result = m_infiles.readString(content);
        }
        else {
            result = m_infiles.readStringCsv(content);
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

string HumdrumInput::GetHumdrumString()
{
    stringstream tempout;
    for (int i = 0; i < m_infiles.getCount(); i++) {
        tempout << m_infiles[i];
    }
    return tempout.str();
}

//////////////////////////////
//
// HumdrumInput::convertHumdrum -- Top level method called from ImportFile or
//     ImportString.  Convert a hum::HumdrumFile structure into an MEI
//     structure.  Returns false if there was an error in the conversion
//     process.
//
// Reference:
//     http://music-encoding.org/documentation/2.1.1/cmn
//

bool HumdrumInput::convertHumdrum()
{
    if (GetOutputFormat() == "humdrum") {
        // allow for filtering within toolkit.
        return true;
    }
    if (m_infiles.getCount() == 0) {
        return false;
    }

    // apply Humdrum tools if there are any filters in the file.
    hum::Tool_filter filter;
    for (int i = 0; i < m_infiles.getCount(); i++) {
        if (m_infiles[i].hasGlobalFilters()) {
            filter.run(m_infiles[i]);
            if (filter.hasHumdrumText()) {
                m_infiles[i].readString(filter.getHumdrumText());
            }
            else {
                // should have auto updated itself in the filter.
            }
        }
    }

    // Apply Humdrum tools to the entire set if they are
    // at the universal level.
    if (m_infiles.hasUniversalFilters()) {
        filter.runUniversal(m_infiles);
        if (filter.hasHumdrumText()) {
            m_infiles.readString(filter.getHumdrumText());
        }
    }

    hum::HumdrumFile &infile = m_infiles[0];

    m_multirest = analyzeMultiRest(infile);
    m_breaks = analyzeBreaks(infile);

    infile.analyzeSlurs();
    infile.analyzeKernTies();
    infile.analyzeKernStemLengths();
    infile.analyzeRestPositions();
    infile.analyzeKernAccidentals();
    parseSignifiers(infile);
    checkForColorSpine(infile);
    infile.analyzeRScale();
    infile.analyzeCrossStaffStemDirections();
    m_spine_color.resize(infile.getMaxTrack() + 1);
    initializeSpineColor(infile);
    initializeIgnoreVector(infile);

    bool status = true; // for keeping track of problems in conversion process.

    extractNullInformation(m_nulls, infile);

    prepareTimeSigDur();
    setupMeiDocument();

    // Create a list of the parts and which spine represents them.
    std::vector<hum::HTp> &staffstarts = m_staffstarts;
    std::vector<string> stafftypes;
    stafftypes.push_back("**kern");
    stafftypes.push_back("**mens");
    infile.getSpineStartList(staffstarts, stafftypes);

    m_fbstates.resize(staffstarts.size());
    std::fill(m_fbstates.begin(), m_fbstates.end(), 0);

    m_fbstaff.resize(staffstarts.size());
    std::fill(m_fbstaff.begin(), m_fbstaff.end(), false);

    std::vector<hum::HTp> spinestarts;
    infile.getSpineStartList(spinestarts);
    int spineindex = -1;
    for (auto it : spinestarts) {
        if (it->isDataType("**kern")) {
            spineindex++;
        }
        else if (it->isDataType("**mxhm")) {
            m_harm = true;
        }
        else if (it->isDataType("**mxhm")) {
            m_harm = true;
        }
        else if (it->isDataType("**fing")) {
            m_fing = true;
        }
        else if (it->isDataType("**string")) {
            m_string = true;
        }
        else if (it->isDataType("**mens")) {
            spineindex++;
            m_mens = true;
        }
        else if (it->isDataType("**harm")) {
            m_harm = true;
        }
        else if (it->isDataType("**rhrm")) { // **recip + **harm
            m_harm = true;
        }
        else if (it->getDataType().compare(0, 7, "**cdata") == 0) {
            m_harm = true;
        }
        else if (it->isDataType("**fb")) {
            m_fb = true;
            if (spineindex >= 0) {
                m_fbstates[spineindex] = -1;
                m_fbstaff[spineindex] = true;
            }
        }
        else if (it->isDataType("**fba")) {
            m_fb = true;
            if (spineindex >= 0) {
                m_fbstates[spineindex] = +1;
                m_fbstaff[spineindex] = true;
            }
        }
        else if (it->isDataType("**Bnum")) {
            // older name
            m_fb = true;
            if (spineindex >= 0) {
                m_fbstates[spineindex] = -1;
                m_fbstaff[spineindex] = true;
            }
        }
    }

    if (staffstarts.size() == 0) {
        // no parts in file, give up.  Perhaps return an error.
        return status;
    }

    // Reverse the order, since top part is last spine.
    reverse(staffstarts.begin(), staffstarts.end());
    calculateReverseKernIndex();

    m_staffstates.resize(staffstarts.size());

    m_transpose.resize(staffstarts.size());
    std::fill(m_transpose.begin(), m_transpose.end(), 0);

    prepareVerses();
    prepareSections();

    prepareStaffGroups();
    prepareHeaderFooter();

    // m_meausreIndex not currently used but might be useful sometime.
    m_measureIndex = 0;
    int line = staffstarts[0]->getLineIndex();
    while (line < infile.getLineCount() - 1 && (line >= 0)) {
        m_measureIndex++;
        status &= convertSystemMeasure(line);
        if ((line < infile.getLineCount() - 1) && (infile[line + 1].isGlobalComment())) {
            // Check for page/system break, and add <sb/> if found.
            // (currently mapping page breaks to system breaks.)
            hum::HTp token = infile.token(line + 1, 0);
            if (token->compare(0, 12, "!!linebreak:") == 0) {
                Sb *sb = new Sb;
                m_sections.back()->AddChild(sb);
                if (token->find("original")) {
                    // maybe allow other types of system breaks here
                    sb->SetType("original");
                }
            }
            else if (token->compare(0, 12, "!!pagebreak:") == 0) {
                Sb *sb = new Sb;
                m_sections.back()->AddChild(sb);
                if (token->find("original")) {
                    // maybe allow other types of page breaks here
                    sb->SetType("original");
                }
            }
        }
    }

    createHeader();

    // calculateLayout();

    m_doc->ConvertToPageBasedDoc();
    promoteInstrumentAbbreviationsToGroup();
    promoteInstrumentNamesToGroup();

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
// HumdrumInput::initializeIgnoreVector -- Mark areas of the input file that
//     should not be converted into
//

void HumdrumInput::initializeIgnoreVector(hum::HumdrumFile &infile)
{
    m_ignore.resize(infile.getLineCount());
    int state = false;
    for (int i = 0; i < infile.getLineCount(); i++) {
        m_ignore[i] = state;
        if (!infile[i].isGlobalComment()) {
            continue;
        }
        if ((std::string)infile[i] == "!!ignore") {
            state = true;
        }
        else if ((std::string)infile[i] == "!!Xignore") {
            state = false;
        }
    }
}

//////////////////////////////
//
// HumdrumInput::extractNullInformation --
//

void HumdrumInput::extractNullInformation(vector<bool> &nulls, hum::HumdrumFile &infile)
{
    nulls.resize(infile.getLineCount());
    for (int i = 0; i < infile.getLineCount(); i++) {
        if (!infile[i].isData()) {
            // only keeping track of data null-lines.
            nulls[i] = false;
            continue;
        }
        // Maybe create an HumdrumLine::isAllKernNull() function for this one.
        // But this is probably not a good idea, since there is still a need to
        // place dynamics at fractional positions in relation to the notes sometimes.
        nulls[i] = infile[i].isAllNull();
    }

    m_duradj.resize(infile.getLineCount());
    for (int i = 0; i < (int)m_duradj.size(); i++) {
        m_duradj[i] = 0;
    }
    hum::HumNum sum = 0;
    for (int i = 0; i < infile.getLineCount(); i++) {
        if (infile[i].isBarline()) {
            // Probably suppress null corrections across barlines...
            sum = 0;
            continue;
        }
        if (!infile[i].isData()) {
            continue;
        }
        if (infile[i].getDuration() == 0) {
            continue;
        }
        if (m_nulls[i]) {
            // A null line: collect the duration and move to the next
            // data line that is not null or has 0 duration.
            sum += infile[i].getDuration();
        }
        else if (sum > 0) {
            // A non-null line that precedes some null data lines: store the
            // accumulated duration of the null lines for prespace analysis in
            // fillContentsOfLayer().
            m_duradj[i] = sum;
            sum = 0;
        }
    }
}

//////////////////////////////
//
// HumdrumInput::parseEmbeddedOptions --
//

void HumdrumInput::parseEmbeddedOptions(Doc &doc)
{
    Options *opts = doc.GetOptions();
    if (!opts) {
        return;
    }
    if (m_infiles.getCount() == 0) {
        return;
    }
    hum::HumdrumFile &infile = m_infiles[0];
    hum::HumRegex hre;
    // find the last !!!verovio-parameter-groups: entry in the file
    // (only the last one will be read).
    std::vector<std::string> pgroups;
    for (int i = infile.getLineCount() - 1; i >= 0; i--) {
        if (!infile[i].isReference()) {
            continue;
        }
        std::string key = infile[i].getReferenceKey();
        if (key != "verovio-parameter-group") {
            continue;
        }
        std::string groups = infile[i].getReferenceValue();
        if (groups.empty()) {
            break;
        }
        hre.split(pgroups, groups, "[\\s,]+");
        break;
    }

    map<std::string, std::string> inputparameters;
    // Now read through the file searching for verovio parameters
    // that are either unassigned to a group, or is in one of the
    // given groups.
    std::string pkey;
    std::string pvalue;
    std::string value;
    for (int i = 0; i < infile.getLineCount(); i++) {
        if (!infile[i].isReference()) {
            continue;
        }
        std::string key = infile[i].getReferenceKey();
        if (key == "verovio-parameter-group") {
            continue;
        }
        if (key.compare(0, 7, "verovio") != 0) {
            continue;
        }
        if (key == "verovio") {
            // in the global group, so process always:
            value = infile[i].getReferenceValue();

            if (!hre.search(value, "\\s*([^\\s]+)\\s+(.*)\\s*$")) {
                continue;
            }
            std::string pkey = hre.getMatch(1);
            std::string pvalue = hre.getMatch(2);
            if (value.empty()) {
                cerr << "Warning: value is empty for parameter " << key << endl;
                continue;
            }
            inputparameters[pkey] = pvalue;
        }
        else {
            // check to see if parameter is in desired group
            for (int j = 0; j < (int)pgroups.size(); j++) {
                if (key.compare(8, std::string::npos, pgroups[j]) != 0) {
                    continue;
                }
                value = infile[i].getReferenceValue();

                if (!hre.search(value, "\\s*([^\\s]+)\\s+(.*)\\s*$")) {
                    continue;
                }
                std::string pkey = hre.getMatch(1);
                std::string pvalue = hre.getMatch(2);
                if (value.empty()) {
                    cerr << "Warning: value is empty for parameter " << key << endl;
                    continue;
                }
                inputparameters[pkey] = pvalue;
                break;
            }
        }
    }

    const MapOfStrOptions *optionlist = opts->GetItems();
    for (auto inputoption : inputparameters) {
        auto entry = optionlist->find(inputoption.first);
        if (entry == optionlist->end()) {
            cerr << "Warning: option " << inputoption.first << " is not recognized" << endl;
            continue;
        }
        // cerr << "SETTING OPTION " << inputoption.first << " TO " << inputoption.second << endl;
        entry->second->SetValue(inputoption.second);
    }
}

//////////////////////////////
//
// HumdrumInput::initializeSpineColor -- Look for *color: interpretations before data.
//

void HumdrumInput::initializeSpineColor(hum::HumdrumFile &infile)
{
    hum::HumRegex hre;
    for (int i = 0; i < infile.getLineCount(); i++) {
        if (infile[i].isData()) {
            break;
        }
        if (infile[i].isInterpretation()) {
            for (int j = 0; j < infile[i].getFieldCount(); j++) {
                if (hre.search(infile.token(i, j), "^\\*color:(.*)")) {
                    int ctrack = infile.token(i, j)->getTrack();
                    m_spine_color[ctrack] = hre.getMatch(1);
                }
            }
        }
    }
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
// changed to <workList> in MEI 4.0:
//     <workDesc>  . . . . . . . . Groupling for non-bibliograhpic aspects of
// text.
//        <identifier> <titleStmt> <history> <langUsage> <key> <tempo> <meter>
//        <perfMedium> <notesStmt> <classification> <castList> <incipit>
//        <contents> <relatedItem>
//
//     <revisionDesc>  . . . . . . Alterations to the file.
//        <change>
//

void HumdrumInput::createHeader()
{
    hum::HumdrumFile &infile = m_infiles[0];
    std::vector<hum::HumdrumLine *> references = infile.getReferenceRecords();
    std::vector<std::vector<string> > respPeople;
    getRespPeople(respPeople, references);
    pugi::xml_node meiHead = m_doc->m_header.append_child("meiHead");

    // <fileDesc> /////////////
    pugi::xml_node fileDesc = meiHead.append_child("fileDesc");
    pugi::xml_node fileTitle = fileDesc.append_child("titleStmt");

    std::string OTL = getReferenceValue("OTL", references);
    pugi::xml_node title = fileTitle.append_child("title");
    if (!OTL.empty()) {
        title.append_child(pugi::node_pcdata).set_value(unescapeHtmlEntities(OTL).c_str());
    }

    // <pubStmt> /////////////
    pugi::xml_node pubStmt = fileDesc.append_child("pubStmt");
    pugi::xml_document availability;
    for (int i = 0; i < (int)references.size(); ++i) {
        std::string refKey = references[i]->getReferenceKey();
        if (refKey.compare(0, 2, "YE") && refKey.compare(0, 3, "EED")) {
            continue;
        }
        else if (!refKey.compare(0, 3, "EED")) {
            pugi::xml_node pubRespStmt = pubStmt.prepend_child("respStmt");
            pugi::xml_node editor = pubRespStmt.append_child("persName");
            editor.append_attribute("xml:id") = StringFormat("persname-L%d", references[i]->getLineNumber()).c_str();
            editor.append_attribute("analog") = "humdrum:EED";
            editor.append_attribute("role") = "editor";
            editor.append_child(pugi::node_pcdata).set_value(references[i]->getReferenceValue().c_str());
        }
        else if (!refKey.compare(2, 1, "C")) {
            pugi::xml_node useRestrict = availability.append_child("useRestrict");
            useRestrict.append_attribute("xml:id")
                = StringFormat("userestrict-L%d", references[i]->getLineNumber()).c_str();
            useRestrict.append_attribute("analog") = "humdrum:YEC";
            useRestrict.append_child(pugi::node_pcdata).set_value(references[i]->getReferenceValue().c_str());
        }
        else if (!refKey.compare(2, 1, "M")) {
            pugi::xml_node useRestrict = availability.append_child("useRestrict");
            useRestrict.append_attribute("xml:id")
                = StringFormat("userestrict-L%d", references[i]->getLineNumber()).c_str();
            useRestrict.append_attribute("analog") = "humdrum:YEM";
            useRestrict.append_child(pugi::node_pcdata).set_value(references[i]->getReferenceValue().c_str());
        }
        else if (!refKey.compare(2, 1, "N")) {
            pugi::xml_node pubPlace = pubStmt.append_child("pubPlace");
            pubPlace.append_attribute("xml:id") = StringFormat("pubplace-L%d", references[i]->getLineNumber()).c_str();
            pubPlace.append_attribute("analog") = "humdrum:YEN";
            pubPlace.append_child(pugi::node_pcdata).set_value(references[i]->getReferenceValue().c_str());
        }
        else if (!refKey.compare(2, 1, "P")) {
            pugi::xml_node publisher = pubStmt.append_child("publisher");
            publisher.append_attribute("xml:id")
                = StringFormat("publisher-L%d", references[i]->getLineNumber()).c_str();
            publisher.append_attribute("analog") = "humdrum:YEP";
            publisher.append_child(pugi::node_pcdata).set_value(references[i]->getReferenceValue().c_str());
        }
        else if (!refKey.compare(2, 1, "R")) {
            pugi::xml_node pubDate = pubStmt.append_child("date");
            pubDate.append_attribute("xml:id") = StringFormat("date-L%d", references[i]->getLineNumber()).c_str();
            pubDate.append_attribute("analog") = "humdrum:YER";
            pubDate.append_child(pugi::node_pcdata).set_value(references[i]->getReferenceValue().c_str());
        }
    }
    if (availability.first_child()) {
        pugi::xml_node copyright = pubStmt.append_child("availability");
        for (pugi::xml_node child = availability.first_child(); child; child = child.next_sibling()) {
            copyright.append_copy(child);
        }
        availability.reset();
    }

    // <encodingDesc> /////////
    pugi::xml_node encodingDesc = meiHead.append_child("encodingDesc");

    // <appInfo> /////////
    pugi::xml_node appInfo = encodingDesc.append_child("appInfo");
    pugi::xml_node application = appInfo.append_child("application");
    application.append_attribute("isodate") = getDateString().c_str();
    application.append_attribute("version") = GetVersion().c_str();
    pugi::xml_node name = application.append_child("name");
    name.append_child(pugi::node_pcdata).set_value("Verovio");
    pugi::xml_node p1 = application.append_child("p");
    p1.append_child(pugi::node_pcdata).set_value("Transcoded from Humdrum");
    // <editorialDecl> /////////
    std::string RNB = getReferenceValue("RNB", references);
    std::string RWG = getReferenceValue("RWG", references);
    if (!RNB.empty() || !RWG.empty()) {
        pugi::xml_node editorialDecl = encodingDesc.append_child("editorialDecl");
        for (int i = 0; i < (int)references.size(); ++i) {
            std::string key = references[i]->getReferenceKey();
            if (key == "RNB") {
                pugi::xml_node note = editorialDecl.append_child("p");
                note.append_child(pugi::node_pcdata).set_value(references[i]->getReferenceValue().c_str());
                note.append_attribute("label") = "note";
            }
            if (key == "RWG") {
                pugi::xml_node warning = editorialDecl.append_child("p");
                warning.append_child(pugi::node_pcdata).set_value(references[i]->getReferenceValue().c_str());
                warning.append_attribute("label") = "warning";
            }
        }
    }
    // <projectDesc> /////////
    std::string ENC = getReferenceValue("ENC", references);
    std::string EEV = getReferenceValue("EEV", references);
    if (!ENC.empty() || !EEV.empty()) {
        pugi::xml_node projectDesc = encodingDesc.append_child("projectDesc");
        if (!ENC.empty()) {
            ENC = "Encoded by: " + ENC;
            pugi::xml_node p2 = projectDesc.append_child("p");
            p2.append_child(pugi::node_pcdata).set_value(ENC.c_str());
        }
        if (!EEV.empty()) {
            EEV = "Version: " + EEV;
            pugi::xml_node p3 = projectDesc.append_child("p");
            p3.append_child(pugi::node_pcdata).set_value(EEV.c_str());
        }
    }

    // <sourceDesc> /////////

    // <workDesc> /////////////
    // <workDesc> changed to <workList> in MEI 4.0
    // pugi::xml_node workDesc = meiHead.append_child("workDesc");
    pugi::xml_node workList = meiHead.append_child("workList");
    // pugi::xml_node work = workDesc.append_child("work");
    pugi::xml_node work = workList.append_child("work");

    std::string SCT = getReferenceValue("SCT", references);
    if (!SCT.empty()) {
        pugi::xml_node identifier = work.append_child("identifier");
        identifier.append_attribute("analog") = "humdrum:SCT";
        identifier.append_child(pugi::node_pcdata).set_value(SCT.c_str());
    }
    // <titleStmt> removed in MEI 4.0
    // pugi::xml_node titleStmt = work.append_child("titleStmt");
    // pugi::xml_node titleStmt = work.append_child("titleStmt");
    // insertTitle(titleStmt, references);
    insertTitle(work, references);
    if (respPeople.size() > 0) {
        // insertRespStmt(titleStmt, respPeople);
        // Update for MEI 4.0:
        insertPeople(work, respPeople);
    }
    std::string ODT = getReferenceValue("ODT", references);
    std::string OCY = getReferenceValue("OCY", references);
    std::string OPC = getReferenceValue("OPC", references);
    if (!ODT.empty() || !OCY.empty() || !OPC.empty()) {
        pugi::xml_node creation = work.append_child("creation");
        if (!ODT.empty()) {
            pugi::xml_node date = creation.append_child("date");
            date.append_attribute("analog") = "humdrum:ODT";
            date.append_child(pugi::node_pcdata).set_value(ODT.c_str());
        }
        if (!OCY.empty()) {
            pugi::xml_node country = creation.append_child("geogName");
            country.append_attribute("analog") = "humdrum:OCY";
            country.append_child(pugi::node_pcdata).set_value(OCY.c_str());
        }
        if (!OPC.empty()) {
            pugi::xml_node place = creation.append_child("geogName");
            place.append_attribute("analog") = "humdrum:OPC";
            place.append_child(pugi::node_pcdata).set_value(OPC.c_str());
        }
    }
    std::string HAO = getReferenceValue("HAO", references);
    if (!HAO.empty()) {
        pugi::xml_node history = work.append_child("history");
        history.append_attribute("analog") = "humdrum:HAO";
        for (int i = 0; i < (int)references.size(); ++i) {
            std::string refKey = references[i]->getReferenceKey();
            if (refKey.compare(0, 3, "HAO")) {
                continue;
            }
            else {
                pugi::xml_node histLine = history.append_child("p");
                histLine.append_attribute("xml:id") = StringFormat("p-L%d", references[i]->getLineNumber()).c_str();
                histLine.append_child(pugi::node_pcdata).set_value(references[i]->getReferenceValue().c_str());
            }
        }
    }

    // <extMeta> /////////////
    if (references.size() > 0) {
        insertExtMeta(references);
    }

    // pugi::xml_node creation = work.append_child("creation");
}

//////////////////////////////
//
// HumdrumInput::getReferenceValue --
//

string HumdrumInput::getReferenceValue(const std::string &key, std::vector<hum::HumdrumLine *> &references)
{
    for (int i = 0; i < (int)references.size(); ++i) {
        if (key == references[i]->getReferenceKey()) {
            return references[i]->getReferenceValue();
        }
    }
    std::string empty;
    return empty;
}

//////////////////////////////
//
// HumdrumInput::getDateSting -- Return the current time and date as a string.
//

string HumdrumInput::getDateString()
{
    time_t t = time(0); // get time now
    struct tm *now = localtime(&t);
    std::string dateStr = StringFormat("%d-%02d-%02dT%02d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1,
        now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
    return dateStr;
}

//////////////////////////////
//
// HumdrumInput::insertRespStmt -- Print a list of composer and/or lyricist.
//     This is for MEI 3.0 and no longer used, so should eventually be deleted.
//

void HumdrumInput::insertRespStmt(pugi::xml_node &titleStmt, std::vector<std::vector<string> > &respPeople)
{
    if (respPeople.size() == 0) {
        return;
    }
    pugi::xml_node respStmt = titleStmt.append_child("respStmt");
    for (int i = 0; i < (int)respPeople.size(); ++i) {
        pugi::xml_node person = respStmt.append_child("persName");
        person.append_attribute("xml:id") = StringFormat("persname-L%s", respPeople[i][3].c_str()).c_str();
        person.append_attribute("analog") = StringFormat("humdrum:%s", respPeople[i][2].c_str()).c_str();
        person.append_attribute("role") = unescapeHtmlEntities(respPeople[i][1]).c_str();
        person.text().set(unescapeHtmlEntities(respPeople[i][0]).c_str());
    }
}

//////////////////////////////
//
// HumdrumInput::insertPeople -- Print a list of composer and/or lyricist.
//    Adapted from HumdrumInput::insertRespStmt().
//    Each entry in respPeople:
//   [0] = Person's name
//   [1] = role:
//          COM = composer
//          COA = attributed composer
//          COS = suspected composer
//          LYR = lyricist composer
//          see definition of getRespPeople() for more roles.
//   [2] = @analog
//   [3] = Line number for xml:id creation
//

void HumdrumInput::insertPeople(pugi::xml_node &work, std::vector<std::vector<string> > &respPeople)
{
    if (respPeople.size() == 0) {
        return;
    }
    // pugi::xml_node respStmt = titleStmt.append_child("respStmt");
    bool created = false;
    pugi::xml_node person;
    for (int i = 0; i < (int)respPeople.size(); ++i) {
        created = false;
        if (respPeople[i][1] == "composer") {
            person = work.append_child("composer");
            created = true;
        }
        if (respPeople[i][1] == "attributed composer") {
            person = work.append_child("composer");
            person.append_attribute("cert") = "unknown";
            created = true;
        }
        if (respPeople[i][1] == "suspected composer") {
            person = work.append_child("composer");
            person.append_attribute("cert") = "unknown";
            created = true;
        }
        else if (respPeople[i][1] == "lyricist") {
            person = work.append_child("lyricist");
            created = true;
        }
        else if (respPeople[i][1] == "librettist") {
            person = work.append_child("librettist");
            created = true;
        }

        if (created) {
            person.text().set(unescapeHtmlEntities(respPeople[i][0]).c_str());
            if (!respPeople[i][2].empty()) {
                person.append_attribute("analog") = StringFormat("humdrum:%s", respPeople[i][2].c_str()).c_str();
            }
            person.append_attribute("xml:id") = StringFormat("person-L%s", respPeople[i][3].c_str()).c_str();
        }
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

void HumdrumInput::getRespPeople(
    std::vector<std::vector<string> > &respPeople, std::vector<hum::HumdrumLine *> &references)
{

    // precalculate a reference map here to make more O(N) rather than O(N^2)
    addPerson(respPeople, references, "COM", "composer"); // cmp
    addPerson(respPeople, references, "COA", "attributed composer");
    addPerson(respPeople, references, "COS", "suspected composer");
    addPerson(respPeople, references, "LYR", "lyricist"); // lyr
    addPerson(respPeople, references, "LIB", "librettist"); // lbt
    addPerson(respPeople, references, "LAR", "arranger"); // arr
    addPerson(respPeople, references, "LOR", "adapter"); // orchestrator, adp
    addPerson(respPeople, references, "RPN", "producer"); // pro
    addPerson(respPeople, references, "MPN", "performer"); // prf, also: Singer/Instrumentalist
    addPerson(respPeople, references, "MCN", "conductor"); // cnd
    addPerson(respPeople, references, "ODE", "dedicatee"); // dte
    addPerson(respPeople, references, "OCO", "patron"); // commissioner, pat
    addPerson(respPeople, references, "OCL", "collector"); // col
    // ENC and EED are handled separately
    // addPerson(respPeople, references, "EED", "digital editor");
    // addPerson(respPeople, references, "ENC", "encoder"); // mrk,
    // Markup editor
}

//////////////////////////////
//
// HumdrumInput::addPerson --
//

void HumdrumInput::addPerson(std::vector<std::vector<string> > &respPeople, std::vector<hum::HumdrumLine *> &references,
    const std::string &key, const std::string &role)
{
    for (int i = 0; i < (int)references.size(); ++i) {
        if (references[i]->getReferenceKey() == key) {
            respPeople.resize(respPeople.size() + 1);
            respPeople.back().resize(4);
            respPeople.back()[0] = references[i]->getReferenceValue();
            respPeople.back()[1] = role;
            respPeople.back()[2] = key;
            respPeople.back()[3] = std::to_string(references[i]->getLineNumber());
        }
    }
}

//////////////////////////////
//
// HumdrumInput::insertExtMeta -- Print Humdrum reference records as XML into
//     <extMeta> element.
//

void HumdrumInput::insertExtMeta(std::vector<hum::HumdrumLine *> &references)
{
    stringstream xmldata;
    xmldata << "<extMeta>\n";
    xmldata << "\t<frames xmlns=\"http://www.humdrum.org/ns/humxml\">\n";
    for (int i = 0; i < (int)references.size(); ++i) {
        std::string refKey = references[i]->getReferenceKey();
        // Keep all reference records for round-trip conversions:
        // if (!(refKey.compare(0, 3, "EED") && refKey.compare(0, 2, "HA") && refKey.compare(0, 2, "OT")
        //         && refKey.compare(0, 2, "YE") && refKey.compare(0, 1, "X"))) {
        //     continue;
        // }
        references[i]->printXml(xmldata, 4);
    }
    xmldata << "\t</frames>\n";
    xmldata << "</extMeta>\n";

    pugi::xml_document tmpdoc;
    pugi::xml_parse_result result = tmpdoc.load_string(xmldata.str().c_str());
    if (!result) {
        // some sort of error, so give up;
        cerr << "ExtMeta parse error: " << result.description() << endl;
        cerr << xmldata.str();
        return;
    }

    m_doc->m_header.first_child().append_copy(tmpdoc.document_element());
}

//////////////////////////////
//
// HumdrumInput::insertTitle --
//

void HumdrumInput::insertTitle(pugi::xml_node &work, const std::vector<hum::HumdrumLine *> &references)
{
    std::string key;
    std::string value;
    bool lang;
    bool plang;
    std::string language;

    int titlecount = 0;

    for (int i = 0; i < (int)references.size(); ++i) {
        plang = false;
        lang = false;
        key = references[i]->getReferenceKey();
        if (key.compare(0, 2, "OT") && key.compare(0, 1, "X")) {
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

        for (int j = 0; j < (int)language.size(); ++j) {
            if (language[j] == '-') {
                // don't force to lower case after first dash
                // as BCP 47 country codes are in upper case, and
                // variant codes usually start capitalized.
                break;
            }
            language[j] = std::tolower(language[j]);
        }

        pugi::xml_node title = work.append_child("title");
        titlecount++;
        title.text().set(unescapeHtmlEntities(value).c_str());
        title.append_attribute("xml:id") = StringFormat("title-L%d", references[i]->getLineNumber()).c_str();
        title.append_attribute("analog") = StringFormat("humdrum:%s", key.substr(0, 3).c_str()).c_str();
        if (key.compare(0, 3, "OTL") == 0) {
            if (!lang || plang) {
                title.append_attribute("type") = "main";
            }
            else {
                title.append_attribute("type") = "translated";
            }
            if (lang) {
                title.append_attribute("xml:lang") = language.c_str();
            }
        }
        else if (key.compare(0, 3, "OTA") == 0) {
            title.append_attribute("type") = "alternative";
            if (lang) {
                title.append_attribute("xml:lang") = language.c_str();
            }
        }
        else if (key.compare(0, 3, "OTP") == 0) {
            title.append_attribute("type") = "alternative";
            title.append_attribute("label") = "popular";
            if (lang) {
                title.append_attribute("xml:lang") = language.c_str();
            }
        }
        else {
            title.append_attribute("type") = "translated";
        }
    }

    if (!titlecount) {
        // Put in a required empty <title/> tag:
        work.append_child("title");
    }
}

//////////////////////////////
//
// HumdrumInput::prepareVerses -- Assumes that m_staffstarts has been
//      filled already.
//

void HumdrumInput::prepareVerses()
{
    int i, j;
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    // ss[*].verse should already be set to false.

    std::vector<hum::HTp> &kern = m_staffstarts;

    if (kern.size() == 0) {
        return;
    }

    hum::HumdrumLine &line = *kern[0]->getLine();
    int field;

    for (i = 0; i < (int)kern.size(); ++i) {
        field = kern[i]->getFieldIndex();
        for (j = field + 1; j < line.getFieldCount(); ++j) {
            if (line.token(j)->isKern()) {
                break;
            }
            else if (line.token(j)->isDataType("**text")) {
                ss[i].verse = true;
            }
            else if (line.token(j)->isDataType("**silbe")) {
                ss[i].verse = true;
            }
            else if (line.token(j)->getDataType().compare(0, 7, "**vdata") == 0) {
                ss[i].verse = true;
            }
            else if (line.token(j)->getDataType().compare(0, 8, "**vvdata") == 0) {
                ss[i].verse = true;
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::prepareTimeSigDur -- create a list of the durations of time
//      signatures in the file, indexed by hum::HumdrumFile line number.  Only
//      the first spine in the file is considered.
//

void HumdrumInput::prepareTimeSigDur()
{
    std::vector<hum::HumNum> &sigdurs = m_timesigdurs;
    hum::HumdrumFile &infile = m_infiles[0];
    std::vector<hum::HTp> spinestarts;

    sigdurs.resize(infile.getLineCount());
    std::fill(sigdurs.begin(), sigdurs.end(), -1);

    infile.getKernSpineStartList(spinestarts);
    hum::HTp kernspine = NULL;
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

    hum::HumNum curdur = -1;
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

void HumdrumInput::calculateReverseKernIndex()
{
    std::vector<int> &rkern = m_rkern;
    hum::HumdrumFile &infile = m_infiles[0];
    const std::vector<hum::HTp> &staffstarts = m_staffstarts;

    rkern.resize(infile.getSpineCount() + 1);
    std::fill(rkern.begin(), rkern.end(), -1);
    for (int i = 0; i < (int)staffstarts.size(); ++i) {
        rkern[staffstarts[i]->getTrack()] = i;
    }
}

//////////////////////////////
//
// HumdrumInput::prepareStaffGroups --  Add information about each part and
//    group by brackets/bar groupings
//

void HumdrumInput::prepareStaffGroups()
{
    const std::vector<hum::HTp> &staffstarts = m_staffstarts;

    if (staffstarts.size() > 0) {
        addMidiTempo(m_doc->m_scoreDef, staffstarts[0]);
    }
    for (int i = 0; i < (int)staffstarts.size(); ++i) {
        m_staffdef.push_back(new StaffDef());
        // m_staffgroup->AddChild(m_staffdef.back());
        fillPartInfo(staffstarts[i], i + 1, (int)staffstarts.size());
    }

    string decoration = getSystemDecoration("system-decoration");

    if (decoration == "") {
        if (staffstarts.size() == 2) {
            StaffGrp *sg = new StaffGrp();
            m_doc->m_scoreDef.AddChild(sg);
            sg->SetSymbol(staffGroupingSym_SYMBOL_brace);
            sg->SetBarThru(BOOLEAN_true);
            sg->AddChild(m_staffdef[0]);
            sg->AddChild(m_staffdef[1]);
        }

        else if (staffstarts.size() > 2) {
            StaffGrp *sg = new StaffGrp();
            m_doc->m_scoreDef.AddChild(sg);
            sg->SetSymbol(staffGroupingSym_SYMBOL_bracket);
            for (int i = 0; i < (int)m_staffdef.size(); ++i) {
                sg->AddChild(m_staffdef[i]);
            }
        }

        else if (staffstarts.size() == 1) {
            StaffGrp *sg = new StaffGrp();
            m_doc->m_scoreDef.AddChild(sg);
            sg->AddChild(m_staffdef[0]);
        }
        // do something if there is no staff in the score?
    }
    else {
        processStaffDecoration(decoration);
    }
}

//////////////////////////////
//
// HumdrumInput::promoteInstrumentNamesToGroup -- If two staves are in a staff group and only one
//    has an instrument label (or both have identical labels), then move the label to the group
//    level, so that the instrument name/abbreviation is centered between the staves.
//
// scoreDef
//    staffDef
//    staffGrp
//       staffDef
//       staffGrp
//          staffDef
//          staffDef
//

void HumdrumInput::promoteInstrumentNamesToGroup()
{
    ScoreDef &sdf = m_doc->m_scoreDef;
    int count = sdf.GetChildCount();

    for (int i = 0; i < count; i++) {
        Object *obj = sdf.GetChild(i);
        std::string name = obj->GetClassName();
        if (name != "StaffGrp") {
            continue;
        }
        StaffGrp *sg = (StaffGrp *)obj;
        promoteInstrumentsForStaffGroup(sg);
    }
}

//////////////////////////////
//
// HumdrumInput::promoteInstrumentsForStaffGroup --
//
//

void HumdrumInput::promoteInstrumentsForStaffGroup(StaffGrp *group)
{
    int count = group->GetChildCount();
    std::vector<std::string> names;
    string name;
    vector<StaffDef *> sds;
    sds.clear();
    for (int i = 0; i < count; i++) {
        Object *obj = group->GetChild(i);
        name = obj->GetClassName();
        if (name == "StaffGrp") {
            promoteInstrumentsForStaffGroup((StaffGrp *)obj);
        }
        if (name != "StaffDef") {
            continue;
        }
        StaffDef *sd = (StaffDef *)obj;
        sds.push_back(sd);
        std::string label = getInstrumentName(sd);
        names.push_back(label);
    }
    bool allsame = true;
    if (names.size() <= 1) {
        allsame = false;
    }
    if (sds.size() != 2) {
        return;
    }
    string nonempty = names[0];
    for (int i = 1; i < (int)names[i].size(); i++) {
        if (names[i] == "") {
            continue;
        }
        if (nonempty == "") {
            nonempty = names[i];
            continue;
        }
        if (names[i] != nonempty) {
            allsame = false;
            break;
        }
    }
    if (allsame) {
        setInstrumentName(group, nonempty);
        for (int i = 0; i < (int)sds.size(); i++) {
            removeInstrumentName(sds[i]);
        }
    }
}

//////////////////////////////
//
// HumdrumInput::promoteInstrumentAbbreviationsToGroup --
//

void HumdrumInput::promoteInstrumentAbbreviationsToGroup()
{
    ScoreDef &sdf = m_doc->m_scoreDef;
    int count = sdf.GetChildCount();

    for (int i = 0; i < count; i++) {
        Object *obj = sdf.GetChild(i);
        std::string name = obj->GetClassName();
        if (name != "StaffGrp") {
            continue;
        }
        StaffGrp *sg = (StaffGrp *)obj;
        promoteInstrumentAbbreviationsForStaffGroup(sg);
    }
}

//////////////////////////////
//
// HumdrumInput::promoteInstrumentAbbreviationsForStaffGroup --
//
//

void HumdrumInput::promoteInstrumentAbbreviationsForStaffGroup(StaffGrp *group)
{
    int count = group->GetChildCount();
    std::vector<std::string> names;
    string name;
    vector<StaffDef *> sds;
    sds.clear();
    for (int i = 0; i < count; i++) {
        Object *obj = group->GetChild(i);
        name = obj->GetClassName();
        if (name == "StaffGrp") {
            promoteInstrumentAbbreviationsForStaffGroup((StaffGrp *)obj);
        }
        if (name != "StaffDef") {
            continue;
        }
        StaffDef *sd = (StaffDef *)obj;
        sds.push_back(sd);
        std::string label = getInstrumentAbbreviation(sd);
        names.push_back(label);
    }
    bool allsame = true;
    if (names.size() <= 1) {
        allsame = false;
    }
    if (sds.size() != 2) {
        return;
    }
    string nonempty = names[0];
    for (int i = 1; i < (int)names[i].size(); i++) {
        if (names[i] == "") {
            continue;
        }
        if (nonempty == "") {
            nonempty = names[i];
            continue;
        }
        if (names[i] != nonempty) {
            allsame = false;
            break;
        }
    }
    if (allsame) {
        setInstrumentAbbreviation(group, nonempty, NULL);
        for (int i = 0; i < (int)sds.size(); i++) {
            if (names.at(i).empty()) {
                continue;
            }
            removeInstrumentAbbreviation(sds[i]);
        }
    }
}

//////////////////////////////
//
// HumdrumInput::removeInstrumentName -- Assuming only one label for staffDef.
//

void HumdrumInput::removeInstrumentName(StaffDef *sd)
{
    Object *obj = sd->FindDescendantByType(ClassId::LABEL);
    if (!obj) {
        return;
    }
    sd->DeleteChild(obj);
}

//////////////////////////////
//
// HumdrumInput::removeInstrumentAbbreviation -- Assuming only one abbreviation for staffDef.
//

void HumdrumInput::removeInstrumentAbbreviation(StaffDef *sd)
{
    Object *obj = sd->FindDescendantByType(ClassId::LABELABBR);
    if (!obj) {
        return;
    }
    sd->DeleteChild(obj);
}

//////////////////////////////
//
// HumdrumInput::getInstrumentName --
//

std::string HumdrumInput::getInstrumentName(StaffDef *sd)
{
    Object *label = sd->FindDescendantByType(ClassId::LABEL);
    if (!label) {
        return "";
    }
    Object *obj = label->FindDescendantByType(ClassId::TEXT);
    if (!obj) {
        return "";
    }
    Text *text = (Text *)obj;
    std::string name = UTF16to8(text->GetText());
    if (name == "    ") {
        name = "";
    }
    return name;
}

//////////////////////////////
//
// HumdrumInput::getInstrumentAbbreviation --
//

std::string HumdrumInput::getInstrumentAbbreviation(StaffDef *sd)
{
    Object *label = sd->FindDescendantByType(ClassId::LABELABBR);
    if (!label) {
        return "";
    }
    Object *obj = label->FindDescendantByType(ClassId::TEXT);
    if (!obj) {
        return "";
    }
    Text *text = (Text *)obj;
    std::string name = UTF16to8(text->GetText());
    return name;
}

//////////////////////////////
//
// HumdrumInput::processStaffDecoration -- Currently only one level
//    of bracing is allowed.  Probably allow remapping of staff order
//    with the system decoration, and possible merge two kern spines
//    onto a single staff (such as two similar instruments sharing
//    a common staff).
//

void HumdrumInput::processStaffDecoration(const string &decoration)
{
    const std::vector<hum::HTp> &staffstarts = m_staffstarts;

    bool validQ = true;

    // If decoration prefixes number with "s", then match to a kern
    // start which contains *staff# before any data content.  If the
    // number is not prefixed by "s", then assume that it is a kern
    // spine enumeration.  Staff enumerations can be utilized if a smaller
    // group of parts are extracted, but kern enumerations can become
    // invalid if extracting sub-scores.  In both cases the enumeration
    // goes from the top of the staff to the bottom, which means from right
    // to left on the Humdrum line.  If the order of the staff or kern
    // enumeration in the decoration is not monotonic covering every staff
    // in the score, then the results may have problems.

    map<int, int> staffToSpineMapping;
    for (int i = 0; i < (int)staffstarts.size(); ++i) {
        int staff = getStaffNumberLabel(staffstarts[i]);
        if (staff <= 0) {
            continue;
        }
        staffToSpineMapping[staff] = i;
    }

    vector<int> spine; // kernstart index
    vector<bool> barstart; // true if bar group starts at staff.
    vector<bool> barend; // true if bar group stops at staff.

    string d = decoration;
    d += ' ';

    vector<vector<int> > bargroups;
    vector<char> groupstyle;

    groupstyle.resize(1);
    groupstyle.back() = ' ';
    bargroups.resize(1);

    bool staffQ = false;
    int value = 0;

    for (int i = 0; i < (int)d.size() - 1; ++i) {
        if (d[i] == '[') {
            groupstyle.back() = '[';
        }
        else if (d[i] == '{') {
            groupstyle.back() = '{';
        }
        else if (d[i] == 's') {
            staffQ = true;
        }
        else if (d[i] == ')') {
            // end of a bar group
            bargroups.resize(bargroups.size() + 1);
            groupstyle.push_back(' ');
        }
        else if (std::isdigit(d[i])) {
            value = value * 10 + (d[i] - '0');
            if (!std::isdigit(d[i + 1])) {
                if (staffQ) {
                    auto it = staffToSpineMapping.find(value);
                    if (it != staffToSpineMapping.end()) {
                        value = (*it).second;
                    }
                    else {
                        value = -1;
                    }
                }
                else {
                    value = value - 1;
                    if (value >= (int)staffstarts.size()) {
                        value = -1;
                    }
                }
                staffQ = false;
                if (value < 0) {
                    // Spine does not exist in score, so skip
                    value = 0;
                    continue;
                }
                else {
                    bargroups.back().push_back(value);
                }
                value = 0;
            }
        }
    }

    // Remove bracket/brace for any group which has only a single staff:
    for (int i = 0; i < (int)bargroups.size(); ++i) {
        if (bargroups.size() <= 1) {
            groupstyle[i] = ' ';
        }
    }

    // Pull out all non-zero staff groups:
    vector<vector<int> > newgroups;
    vector<char> newstyles;
    for (int i = 0; i < (int)bargroups.size(); ++i) {
        if (bargroups[i].empty()) {
            continue;
        }
        newgroups.push_back(bargroups[i]);
        newstyles.push_back(groupstyle[i]);
    }

    // Check to see that all staffstarts are represented in system decoration;
    // otherwise, declare that it is invalid and print a simple decoration.
    vector<int> found(staffstarts.size(), 0);

    for (int i = 0; i < (int)newgroups.size(); ++i) {
        for (int j = 0; j < (int)newgroups[i].size(); ++j) {
            found.at(newgroups[i][j])++;
        }
    }

    // probably also require the spine indexes to be monotonic.
    for (int i = 0; i < (int)found.size(); ++i) {
        if (found[i] != 1) {
            cerr << "I:" << i << "\t=\t" << found[i] << endl;
            validQ = false;
            break;
        }
    }

    if (!validQ) {
        cerr << "DECORATION IS INVALID " << decoration << endl;
        StaffGrp *sg = new StaffGrp();
        sg->SetSymbol(staffGroupingSym_SYMBOL_bracket);
        m_doc->m_scoreDef.AddChild(sg);
        for (int i = 0; i < (int)m_staffdef.size(); ++i) {
            sg->AddChild(m_staffdef[i]);
        }
        return;
    }

    // Build system groups based on system decoration instructions
    if (newgroups.size() == 1) {
        // only one group
        StaffGrp *sg = new StaffGrp();
        m_doc->m_scoreDef.AddChild(sg);
        // currently required to be barred:
        sg->SetBarThru(BOOLEAN_true);
        if (newstyles[0] == '[') {
            sg->SetSymbol(staffGroupingSym_SYMBOL_bracket);
        }
        else if (newstyles[0] == '{') {
            sg->SetSymbol(staffGroupingSym_SYMBOL_brace);
        }
        for (int i = 0; i < (int)newgroups[0].size(); ++i) {
            sg->AddChild(m_staffdef[newgroups[0][i]]);
        }
    }
    else {
        // multiple barred groups, the outer currently cannot be styled, so just plain
        // and no barring, so outer group is:
        //    <staffGrp barthru="false">
        //    </staffGrp>
        // If there is only one staff in a group, it will be given as a bare
        // staffDef in the root_sg group.
        StaffGrp *root_sg = new StaffGrp();
        root_sg->SetBarThru(BOOLEAN_false);
        m_doc->m_scoreDef.AddChild(root_sg);
        for (int i = 0; i < (int)newgroups.size(); ++i) {
            if (newgroups[i].size() == 1) {
                // insert staffDef directly in root_sg:
                root_sg->AddChild(m_staffdef[newgroups[i][0]]);
            }
            else {
                // create staffGrp and then insert staffDefs for group
                StaffGrp *sg = new StaffGrp();
                root_sg->AddChild(sg);
                sg->SetBarThru(BOOLEAN_true);
                if (newstyles[i] == '[') {
                    sg->SetSymbol(staffGroupingSym_SYMBOL_bracket);
                }
                else if (newstyles[i] == '{') {
                    sg->SetSymbol(staffGroupingSym_SYMBOL_brace);
                }
                for (int j = 0; j < (int)newgroups[i].size(); ++j) {
                    sg->AddChild(m_staffdef[newgroups[i][j]]);
                }
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::prepareHeaderFooter --
//
//

void HumdrumInput::prepareHeaderFooter()
{
    hum::HumdrumFile &infile = m_infiles[0];
    std::vector<std::pair<string, string> > biblist;

    std::vector<hum::HumdrumLine *> records = infile.getReferenceRecords();
    biblist.reserve(records.size());
    std::map<std::string, std::string> refmap;
    for (int i = 0; i < (int)records.size(); ++i) {
        string key = records[i]->getReferenceKey();
        string value = records[i]->getReferenceValue();
        refmap[key] = value;
        biblist.emplace_back(std::make_pair(key, value));
    }

    prepareHeader(biblist, refmap);
    prepareFooter(biblist, refmap);
}

//////////////////////////////
//
// HumdrumInput::processTemplateOperator --
//      Operators:
//           n = full name
//           i = initials for given names and full last name
//           l = last name
//           f = first name
//

std::string HumdrumInput::processTemplateOperator(const std::string &value, const std::string &op)
{
    string input = value;
    string output;
    hum::HumRegex hre;

    if (op.find("U") != std::string::npos) {
        // Force all upper case (cannot handle HTML accents, so should be unescaped first)
        std::locale loc;
        for (int i = 0; i < (int)input.size(); ++i) {
            input[i] = std::toupper(input[i], loc);
        }
    }

    if (op.find("n") != std::string::npos) {
        // full name: reverse strings separated by comma if there is one.
        auto pos = input.find(",");
        if (pos == std::string::npos) {
            output = input;
        }
        else {
            std::string lastname = input.substr(0, pos);
            std::string firstname = input.substr(pos + 1);
            output = firstname + " " + lastname;
        }
    }
    else if (op.find("l") != std::string::npos) {
        // last name: text before comma if there is one.
        auto pos = input.find(",");
        if (pos == std::string::npos) {
            output = input;
        }
        else {
            std::string lastname = input.substr(0, pos);
            output = lastname;
        }
    }
    else if (op.find("f") != std::string::npos) {
        // first name: text after comma if there is one.
        auto pos = input.find(",");
        if (pos == std::string::npos) {
            output = input;
        }
        else {
            std::string firstname = input.substr(pos + 1);
            output = firstname;
        }
    }
    else if (op.find("i") != std::string::npos) {
        // initialed name: first name converted to initials
        auto pos = input.find(",");
        if (pos == std::string::npos) {
            output = input;
        }
        else {
            std::string lastname = input.substr(0, pos);
            std::string firstname = input.substr(pos + 1);
            std::vector<std::string> pieces;
            hre.split(pieces, firstname, " ");
            firstname.clear();
            for (int i = 0; i < (int)pieces.size(); ++i) {
                if (pieces[i].empty()) {
                    continue;
                }
                if (std::isupper(pieces[i][0])) {
                    pieces[i] = pieces[i][0];
                    pieces[i] += ".";
                    if (firstname.size() > 1) {
                        if (firstname[firstname.size() - 2] == '.') {
                            firstname.pop_back();
                            firstname += pieces[i] + " ";
                        }
                        else {
                            firstname += pieces[i] + " ";
                        }
                    }
                    else {
                        firstname += pieces[i] + " ";
                    }
                }
                else {
                    firstname += pieces[i] + " ";
                }
            }
            output = firstname + lastname;
        }
    }
    else if (op.find("y") != std::string::npos) {
        // Show only years for birth/death, and shorten if in same century.
        // Still need to include circa, flourish, and birth/death only.
        string cdates = input;
        string outputdate;
        string birth;
        string death;
        auto pos = cdates.find("-");
        if (pos != std::string::npos) {
            birth = cdates.substr(0, pos);
            death = cdates.substr(pos + 1);
            int birthyear = 0;
            int deathyear = 0;
            if (hre.search(birth, "(\\d\\d\\d\\d)")) {
                birthyear = hre.getMatchInt(1);
            }
            if (hre.search(death, "(\\d\\d\\d\\d)")) {
                deathyear = hre.getMatchInt(1);
            }
            if ((deathyear > 0) && (birthyear > 0)) {
                outputdate = to_string(birthyear);
                outputdate += "&#8211;";
                if ((deathyear / 100) == (birthyear / 100)) {
                    outputdate += to_string(deathyear % 100);
                }
                else {
                    outputdate += to_string(deathyear);
                }
            }
        }
        output = outputdate;
    }
    else if (op.find("Y") != std::string::npos) {
        // Show only years for birth/death, but do not shorten if in same century.
        // Still need to include circa, flourish, and birth/death only.
        string cdates = input;
        string outputdate;
        string birth;
        string death;
        auto pos = cdates.find("-");
        if (pos != std::string::npos) {
            birth = cdates.substr(0, pos);
            death = cdates.substr(pos + 1);
            int birthyear = 0;
            int deathyear = 0;
            if (hre.search(birth, "(\\d\\d\\d\\d)")) {
                birthyear = hre.getMatchInt(1);
            }
            if (hre.search(death, "(\\d\\d\\d\\d)")) {
                deathyear = hre.getMatchInt(1);
            }
            if ((deathyear > 0) && (birthyear > 0)) {
                outputdate = to_string(birthyear);
                outputdate += "&#8211;";
                outputdate += to_string(deathyear);
            }
        }
        output = outputdate;
    }
    else {
        // unknown operator, so just echo input
        output = input;
    }

    return output;
}

//////////////////////////////
//
// HumdrumInput::processReferenceTemplate --
//

std::string HumdrumInput::processReferenceTemplate(const std::string &input,
    std::vector<std::pair<string, string> > &biblist, std::map<std::string, std::string> &refmap)
{
    std::string text = input;
    hum::HumRegex hre;
    std::string prefix;
    std::string suffix;
    std::string match;
    std::string key;
    std::string op;
    while (hre.search(text, "@\\{(.*?)\\}")) {
        match = hre.getMatch(1);
        prefix = hre.getPrefix();
        suffix = hre.getSuffix();
        auto pos = match.find(":");
        if (pos != std::string::npos) {
            key = match.substr(0, pos);
            op = match.substr(pos + 1);
            match = processTemplateOperator(refmap[key], op);
        }
        else {
            match = refmap[match];
        }
        text = prefix;
        text += match;
        text += suffix;
    }
    return unescapeHtmlEntities(text);
}

//////////////////////////////
//
// HumdrumInput::prepareFooter --
//

bool HumdrumInput::prepareFooter(
    std::vector<std::pair<string, string> > &biblist, std::map<std::string, std::string> &refmap)
{

    std::string footleft;
    std::string footcenter;
    std::string footright;
    std::vector<std::string> pieces;
    std::string tstring;
    hum::HumRegex hre;

    //   <rend halign="left" valign="bottom">
    //      <rend fontsize="small">Footer L1</rend><lb/>
    //      <rend fontsize="small">Footer L2</rend><lb/>
    //      <rend fontsize="small">Footer L3</rend>
    //   </rend>

    auto itL = refmap.find("footer-left");
    if (itL != refmap.end()) {
        hre.split(pieces, itL->second, "\\\\n");
        footleft += "<rend halign=\"left\" valign=\"bottom\">\n";
        for (int i = 0; i < (int)pieces.size(); ++i) {
            footleft += "<rend fontsize=\"small\">";
            tstring = processReferenceTemplate(pieces[i], biblist, refmap);
            if (pieces[i].empty()) {
                footleft += "&#160;";
            }
            else {
                footleft += tstring;
            }
            footleft += "</rend>\n";
            if (i < (int)pieces.size() - 1) {
                footleft += "<lb/>\n";
            }
        }
        footleft += "</rend>\n";
    }

    auto itC = refmap.find("footer-center");
    if (itC != refmap.end()) {
        hre.split(pieces, itC->second, "\\\\n");
        footcenter += "<rend halign=\"center\" valign=\"bottom\">\n";
        for (int i = 0; i < (int)pieces.size(); ++i) {
            footcenter += "<rend fontsize=\"small\">";
            tstring = processReferenceTemplate(pieces[i], biblist, refmap);
            if (pieces[i].empty()) {
                footcenter += "&#160;";
            }
            else {
                footcenter += tstring;
            }
            footcenter += "</rend>\n";
            if (i < (int)pieces.size() - 1) {
                footcenter += "<lb/>\n";
            }
        }
        footcenter += "</rend>\n";
    }

    auto itR = refmap.find("footer-right");
    if (itR != refmap.end()) {
        hre.split(pieces, itR->second, "\\\\n");
        footright += "<rend halign=\"right\" valign=\"bottom\">\n";
        for (int i = 0; i < (int)pieces.size(); ++i) {
            footright += "<rend fontsize=\"small\">";
            tstring = processReferenceTemplate(pieces[i], biblist, refmap);
            if (pieces[i].empty()) {
                footright += "&#160;";
            }
            else {
                footright += tstring;
            }
            footright += "</rend>\n";
            if (i < (int)pieces.size() - 1) {
                footright += "<lb/>\n";
            }
        }
        footright += "</rend>\n";
    }

    string footer;
    footer += footleft;
    footer += footcenter;
    footer += footright;

    if (footer.empty()) {
        return false;
    }

    hre.replaceDestructive(footer, "</rend>", "</i>", "g");
    hre.replaceDestructive(footer, "<rend fontstyle=\"italic\">", "<i>", "g");
    hre.replaceDestructive(footer, "<rend><num label=\"page\">#</num></rend>", "%P", "g");

    string meifile = "<mei xmlns=\"http://www.music-encoding.org/ns/mei\" meiversion=\"4.0.0\">\n";
    meifile += "<meiHead></meiHead>";
    meifile += "<music><body><mdiv><score><scoreDef>\n";
    meifile += "<pgFoot>\n";
    meifile += footer;
    meifile += "</pgFoot>\n";
    // Always putting footer on all pages for now:
    meifile += "<pgFoot2>\n";
    meifile += footer;
    meifile += "</pgFoot2>\n";
    meifile += "</scoreDef></score></mdiv></body></music></mei>\n";

    Doc tempdoc;
    MeiInput input(&tempdoc, "");
    if (!input.ImportString(meifile)) {
        LogError("Error importing data");
        return false;
    }

    // MeiOutput meioutput(&tempdoc, "");
    // meioutput.SetScoreBasedMEI(true);
    // string meicontent = meioutput.GetOutput();
    // std::cout << "MEI CONTENT " << meicontent << std::endl;

    Object *pgfoot = tempdoc.m_scoreDef.FindDescendantByType(ClassId::PGFOOT);
    if (pgfoot == NULL) {
        return false;
    }

    int index = pgfoot->GetIdx();
    if (index < 0) {
        return false;
    }
    Object *detached = pgfoot->GetParent()->DetachChild(index);
    if (detached != pgfoot) {
        std::cerr << "Detached element is not the pgHead" << std::endl;
        if (detached) {
            delete detached;
        }
        return false;
    }

    m_doc->m_scoreDef.AddChild(pgfoot);

    Object *pgfoot2 = tempdoc.m_scoreDef.FindDescendantByType(ClassId::PGFOOT2);
    if (pgfoot2 == NULL) {
        return true;
    }

    index = pgfoot2->GetIdx();
    if (index < 0) {
        return true;
    }
    detached = pgfoot2->GetParent()->DetachChild(index);
    if (detached != pgfoot2) {
        std::cerr << "Detached element is not a pgFoot2 element" << std::endl;
        if (detached) {
            delete detached;
        }
        return true;
    }

    m_doc->m_scoreDef.AddChild(pgfoot2);

    return true;
}

//////////////////////////////
//
// HumdrumInput::prepareHeader --
//
// <pgHead>
//     <rend halign="center" valign="middle">
//         <rend fontsize="x-large">Non è questa la mano (<rend fontstyle="italic">Rime</rend> 47)</rend>
//         <lb/>
//         <rend fontsize="small"><rend fontstyle="italic">Il primo libro de madrigali a quattro voci. </rend>(Venice:
//         Gardano, 1579)</rend>
//     </rend>
//     <rend halign="right" valign="bottom">Benedetto Pallavicino</rend>
// </pgHead>
//

bool HumdrumInput::prepareHeader(
    std::vector<std::pair<string, string> > &biblist, std::map<std::string, std::string> &refmap)
{
    std::string headleft;
    std::string headcenter;
    std::string headright;
    std::string tstring;
    hum::HumRegex hre;
    std::vector<std::string> pieces;

    auto ithc = refmap.find("header-center");
    if (ithc != refmap.end()) {
        hre.split(pieces, ithc->second, "\\\\n");
        headcenter = "<rend halign=\"center\" valign=\"middle\">\n";
        for (int i = 0; i < (int)pieces.size(); ++i) {
            if (i == 0) {
                headcenter += "<rend fontsize=\"large\">";
            }
            else {
                headcenter += "<rend fontsize=\"normal\">";
            }
            tstring = processReferenceTemplate(pieces[i], biblist, refmap);
            if (pieces[i].empty()) {
                headcenter += "&#160;";
            }
            else {
                headcenter += tstring;
            }
            headcenter += "</rend>\n";
            if (i < (int)pieces.size() - 1) {
                headcenter += "<lb/>\n";
            }
        }
        headcenter += "</rend>\n";
    }
    else {
        headcenter = automaticHeaderCenter(biblist, refmap);
    }

    int linecount = 0;
    auto ithr = refmap.find("header-right");
    if (ithr != refmap.end()) {
        hre.split(pieces, ithr->second, "\\\\n");
        headright = "<rend halign=\"right\" valign=\"bottom\">\n";
        for (int i = 0; i < (int)pieces.size(); ++i) {
            headright += "<rend fontsize=\"small\">";
            tstring = processReferenceTemplate(pieces[i], biblist, refmap);
            if (pieces[i].empty()) {
                headright += "&#160;";
            }
            else {
                headright += tstring;
            }
            headright += "</rend>\n";
            if (i < (int)pieces.size() - 1) {
                headright += "<lb/>\n";
            }
        }
        headright += "</rend>\n";
    }
    else {
        headright = automaticHeaderRight(biblist, refmap, linecount);
    }

    auto ithl = refmap.find("header-left");
    if (ithl != refmap.end()) {
        hre.split(pieces, ithl->second, "\\\\n");
        headleft = "<rend halign=\"left\" valign=\"bottom\">\n";
        for (int i = 0; i < (int)pieces.size(); ++i) {
            headleft += "<rend fontsize=\"small\">";
            tstring = processReferenceTemplate(pieces[i], biblist, refmap);
            if (pieces[i].empty()) {
                headleft += "&#160;";
            }
            else {
                headleft += tstring;
            }
            headleft += "</rend>\n";
            if (i < (int)pieces.size() - 1) {
                headleft += "<lb/>\n";
            }
        }
        headleft += "</rend>\n";
    }
    else {
        headleft = automaticHeaderLeft(biblist, refmap, linecount);
    }

    std::string head = headcenter + headleft + headright;

    if (head.empty()) {
        return false;
    }

    hre.replaceDestructive(head, "</rend>", "</i>", "g");
    hre.replaceDestructive(head, "<rend fontstyle=\"italic\">", "<i>", "g");
    hre.replaceDestructive(head, "<rend><num label=\"page\">#</num></rend>", "%P", "g");

    string meifile = "<mei xmlns=\"http://www.music-encoding.org/ns/mei\" meiversion=\"4.0.0\">\n";
    meifile += "<meiHead></meiHead>";
    meifile += "<music><body><mdiv><score><scoreDef><pgHead>\n";
    meifile += head;
    meifile += "</pgHead></scoreDef></score></mdiv></body></music></mei>\n";

    Doc tempdoc;
    MeiInput input(&tempdoc, "");
    if (!input.ImportString(meifile)) {
        LogError("Error importing data");
        return false;
    }

    // MeiOutput meioutput(&tempdoc, "");
    // meioutput.SetScoreBasedMEI(true);
    // string meicontent = meioutput.GetOutput();
    // std::cout << "MEI CONTENT " << meicontent << std::endl;

    Object *pghead = tempdoc.m_scoreDef.FindDescendantByType(ClassId::PGHEAD);
    if (pghead == NULL) {
        return false;
    }

    int index = pghead->GetIdx();
    if (index < 0) {
        return false;
    }
    Object *detached = pghead->GetParent()->DetachChild(index);
    if (detached != pghead) {
        std::cerr << "Detached element is not the pgHead" << std::endl;
        if (detached) {
            delete detached;
        }
        return false;
    }

    m_doc->m_scoreDef.AddChild(pghead);

    return true;
}

//////////////////////////////
//
// HumdrumInput::automaticHeaderLeft -- If there is no reference record "header-left",
//     Then any "LYR" (lyricist) will be displayed, and if LDT (lyric composition date)
//     is found, then that will be added in parenthese after the lyricist's name.
//     The linecount variable is used to align the lyricist with the composer, depending
//     on whether or not the composer's date are displayed.
//

std::string HumdrumInput::automaticHeaderLeft(
    std::vector<std::pair<string, string> > &biblist, std::map<std::string, std::string> &refmap, int linecount)
{
    std::string output;
    std::string lyricist;

    auto itL = refmap.find("LYR");
    if (itL != refmap.end()) {
        std::string name = itL->second;
        auto pos = name.find(",");
        if (pos == std::string::npos) {
            lyricist = name;
        }
        else {
            std::string lastname = name.substr(0, pos);
            std::string firstname = name.substr(pos + 1);
            lyricist = firstname + " " + lastname;
        }
        auto itLD = refmap.find("LDT");
        if (itLD != refmap.end()) {
            lyricist += " (" + itLD->second + ")";
        }
    }
    if (!lyricist.empty()) {
        output += "<rend fontsize=\"small\" halign=\"left\" valign=\"bottom\">";
        output += unescapeHtmlEntities(lyricist);
        output += "</rend>\n";
        if (linecount > 1) {
            output += "<rend fontsize=\"small\" halign=\"left\" valign=\"bottom\">";
            output += "&#160;";
            output += "</rend>\n";
        }
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::automaticHeaderRight -- If there is no reference record "header-right",
//     Then the "COM" (composer) will be displayed.  The full name will be displayed,
//     and if the ordering of the name is "last, first" then it will be reversed to
//     "first last".   If "CDT" is found (composer's dates), then it will be added
//     underneath the composer's name.
//     The linecount variable is used to align the lyricist with the composer, depending
//     on whether or not the composer's date are displayed.
//

std::string HumdrumInput::automaticHeaderRight(
    std::vector<std::pair<string, string> > &biblist, std::map<std::string, std::string> &refmap, int &linecount)
{

    linecount = 0;
    std::string output;

    std::string composer;
    auto it2 = refmap.find("COM");
    if (it2 != refmap.end()) {
        std::string name = it2->second;
        auto pos = name.find(",");
        if (pos == std::string::npos) {
            composer = name;
        }
        else {
            std::string lastname = name.substr(0, pos);
            std::string firstname = name.substr(pos + 1);
            composer = firstname + " " + lastname;
        }
    }
    // <rend fontsize="small" halign="right" valign="bottom">Composer's name</rend>
    if (!composer.empty()) {
        output += "<rend fontsize=\"small\" halign=\"right\" valign=\"bottom\">";
        output += unescapeHtmlEntities(composer);
        output += "</rend>\n";
        linecount++;
    }
    // Composer's dates
    std::string outputdate = processReferenceTemplate("@{CDT:y}", biblist, refmap);

    if (!outputdate.empty()) {
        output += "<rend fontsize=\"small\" halign=\"right\" valign=\"bottom\">";
        output += outputdate;
        output += "</rend>\n";
        linecount++;
    }

    return output;
}

//////////////////////////////
//
// HumdrumInput::automaticHeaderCenter --
//

std::string HumdrumInput::automaticHeaderCenter(
    std::vector<std::pair<string, string> > &biblist, std::map<std::string, std::string> &refmap)
{
    std::string output;
    std::string title;

    auto it = refmap.find("OTL");
    if (it != refmap.end()) {
        title = it->second;
    }
    if (title.empty()) {
        for (int i = 0; i < (int)biblist.size(); ++i) {
            if (biblist[i].first.substr(0, 3) == "OTL") {
                title = biblist[i].second;
                break;
            }
        }
    }

    // Special code for Tasso in Music Project: Add rime number after title.
    std::string rime;
    auto itrime = refmap.find("rime");
    if (itrime != refmap.end()) {
        rime = itrime->second;
    }
    // <rend fontsize="x-large">Non è questa la mano (<rend fontstyle="italic">Rime</rend> 47)</rend>
    if (!title.empty()) {
        output += "<rend halign=\"center\" valign=\"middle\">\n";
        output += "   <rend fontsize=\"large\">";
        output += unescapeHtmlEntities(title);
        if (!rime.empty()) {
            output += " (<rend fontstyle=\"italic\">Rime</rend>&#160;";
            output += rime;
            output += ")";
        }
        output += "</rend>\n";
        output += "<lb/>&#160;\n<lb/>&#160;\n";
        output += "</rend>\n";
    }

    return output;
}

//////////////////////////////
//
// HumdrumInput::getStaffNumberLabel -- Return number 12 in pattern *staff12.
//

int HumdrumInput::getStaffNumberLabel(hum::HTp spinestart)
{
    hum::HTp tok = spinestart;
    while (tok) {
        if (tok->isData()) {
            break;
        }
        if (!tok->isInterpretation()) {
            tok = tok->getNextToken();
            continue;
        }
        if (tok->compare(0, 6, "*staff") != 0) {
            tok = tok->getNextToken();
            continue;
        }
        if (tok->size() <= 6) {
            tok = tok->getNextToken();
            continue;
        }
        string number = tok->substr(6, string::npos);
        if (!std::isdigit(number[0])) {
            tok = tok->getNextToken();
            continue;
        }
        return stoi(number);
    }
    return 0;
}

//////////////////////////////
//
// HumdrumInput::getSystemDecoration --
//

string HumdrumInput::getSystemDecoration(const string &tag)
{
    hum::HumdrumFile &infile = m_infiles[0];
    for (int i = 0; i < infile.getLineCount(); ++i) {
        if (!infile[i].isReference()) {
            continue;
        }
        string key = infile[i].getReferenceKey();
        if (key != tag) {
            continue;
        }
        string value = infile[i].getReferenceValue();
        string output;
        for (int j = 0; j < (int)value.size(); ++j) {
            if (std::isspace(value[j])) {
                continue;
            }
            output.push_back(value[j]);
        }
        return output;
    }
    return "";
}

//////////////////////////////
//
// HumdrumInput::addMidiTempo --
//

void HumdrumInput::addMidiTempo(ScoreDef &m_scoreDef, hum::HTp kernpart)
{
    bool foundtempo = false;
    while (kernpart != NULL) {
        if (kernpart->isData()) {
            break;
        }
        if (!kernpart->isInterpretation()) {
            kernpart = kernpart->getNextToken();
            continue;
        }
        if (kernpart->compare(0, 3, "*MM") == 0) {
            if (kernpart->size() > 3) {
                if (::isdigit((*kernpart)[3])) {
                    int tempo = stoi(kernpart->substr(3));
                    // std::string tempostr = to_string(tempo);
                    m_scoreDef.SetMidiBpm(tempo);
                    foundtempo = true;
                }
            }
            break;
        }
        kernpart = kernpart->getNextToken();
    }
    if (!foundtempo) {
        addDefaultTempo(m_scoreDef);
    }
}

//////////////////////////////
//
// HumdrumInput::addDefaultTempo --  Add MM400 if average rhythm is more than
//    a half note (for basic Renaissance default tempo).
//

void HumdrumInput::addDefaultTempo(ScoreDef &m_scoreDef)
{
    double sum = 0.0;
    int count = 0;
    hum::HumdrumFile &infile = m_infiles[0];
    for (int i = 0; i < infile.getLineCount(); ++i) {
        if (infile[i].getDuration() == 0) {
            continue;
        }
        count++;
        sum += infile[i].getDuration().getFloat();
    }
    double avgdur = sum / count;
    if (avgdur > 1.0) {
        m_scoreDef.SetMidiBpm(400);
    }
}

//////////////////////////////
//
// HumdrumInput::fillPartInfo -- Should use regular expressions
//    in the future.
//

void HumdrumInput::fillPartInfo(hum::HTp partstart, int partnumber, int partcount)
{
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;

    std::string primarymensuration;
    bool haslabel = false;
    std::string label;
    hum::HTp labeltok = NULL;
    std::string abbreviation;
    hum::HTp abbrtok = NULL;
    std::string clef;
    hum::HTp cleftok = NULL;
    std::string keysig;
    hum::HTp keytok = NULL;
    std::string key;
    std::string transpose;
    std::string itranspose;
    std::string timesig;
    hum::HTp timetok = NULL;
    std::string metersig;
    hum::HTp metertok = NULL;
    int top = 0;
    int bot = 0;

    hum::HumRegex hre;
    hum::HTp part = partstart;
    while (part && !part->getLine()->isData()) {
        if (part->compare(0, 5, "*clef") == 0) {
            if (hre.search(part, 5, "\\d")) {
                clef = *part;
                cleftok = part;
            }
        }
        else if (part->compare(0, 6, "*oclef") == 0) {
            if (hre.search(part, 6, "\\d")) {
                m_oclef.emplace_back(partnumber, part);
            }
        }
        else if (part->compare(0, 5, "*omet") == 0) {
            m_omet.emplace_back(partnumber, part);
        }
        else if (part->compare(0, 3, "*k[") == 0) {
            keysig = *part;
            keytok = part;
        }
        else if (part->compare(0, 4, "*Trd") == 0) {
            transpose = *part;
        }
        else if (part->compare(0, 5, "*ITrd") == 0) {
            itranspose = *part;
        }
        else if (part->compare(0, 3, "*I'") == 0) {
            if (partcount > 1) {
                // Avoid encoding the part abbreviation when there is only one
                // part in order to suppress the display of the abbreviation.
                abbreviation = part->substr(3);
                abbrtok = part;
            }
        }
        else if (part->compare(0, 3, "*I\"") == 0) {
            label = part->substr(3);
            labeltok = part;
            haslabel = true;
        }
        else if (part->compare(0, 5, "*met(") == 0) {
            auto ploc = part->rfind(")");
            if (ploc != string::npos) {
                metersig = part->substr(5, ploc - 5);
                metertok = part;
            }
        }
        else if (sscanf(part->c_str(), "*M%d/%d", &top, &bot) == 2) {
            timesig = *part;
            timetok = part;
            ss[partnumber - 1].meter_bottom = bot;
            ss[partnumber - 1].meter_top = top;
            if (bot == 0) {
                ss[partnumber - 1].meter_bottom = 1;
                ss[partnumber - 1].meter_top *= 2;
            }
        }
        else if (part->find("acclev") != string::npos) {
            storeAcclev(*part, partnumber - 1);
        }
        else if (part->compare(0, 5, "*stem") == 0) {
            storeStemInterpretation(*part, partnumber - 1, 1);
        }

        hum::HumdrumFile *hf = part->getOwner()->getOwner();
        int line = part->getLineIndex();
        for (int ii = line + 1; ii < hf->getLineCount(); ++ii) {
            if ((*hf)[ii].isGlobalComment()) {
                if ((*hf)[ii].compare(0, 22, "!!primary-mensuration:") == 0) {
                    string pmen = (*hf)[ii];
                    auto ploc1 = pmen.find("met(");
                    if (ploc1 != string::npos) {
                        auto ploc2 = pmen.rfind(")");
                        if (ploc2 != string::npos) {
                            primarymensuration = pmen.substr(ploc1 + 4, ploc2 - ploc1 - 4);
                        }
                    }
                }
            }
            else {
                break;
            }
        }

        part = part->getNextToken();
    }

    // short-circuit *clef with *oclef for **mens data
    if (partstart->isMens()) {
        if ((!m_oclef.empty()) && (partnumber == m_oclef.back().first)) {
            clef = *m_oclef.back().second;
            cleftok = m_oclef.back().second;
        }
    }

    // short-circuit *met with *omet for **mens data
    if (partstart->isMens()) {
        if ((!m_omet.empty()) && (partnumber == m_omet.back().first)) {
            metersig = *m_omet.back().second;
            metertok = m_omet.back().second;
        }
    }

    m_staffdef.back()->SetN(partnumber);
    m_staffdef.back()->SetLines(5);

    if (clef.size() > 0) {
        setClef(m_staffdef.back(), clef, cleftok);
        ss.at(partnumber - 1).last_clef = clef;
    }
    else {
        std::string autoclef = getAutoClef(partstart, partnumber);
        setClef(m_staffdef.back(), autoclef);
        ss.at(partnumber - 1).last_clef = clef;
    }

    if (transpose.size() > 0) {
        setTransposition(m_staffdef.back(), transpose);
    }

    if (itranspose.size() > 0) {
        // This has to be above setKeySig():
        setDynamicTransposition(partnumber - 1, m_staffdef.back(), itranspose);
    }

    if (abbreviation.size() > 0) {
        setInstrumentAbbreviation(m_staffdef.back(), abbreviation, abbrtok);
    }

    if (haslabel) {
        if (hasIndent(labeltok)) {
            setInstrumentName(m_staffdef.back(), "   ");
        }
        else {
            setInstrumentName(m_staffdef.back(), label, labeltok);
        }
    }
    else if (partnumber == 1) {
        if (hasIndent(partstart)) {
            setInstrumentName(m_staffdef.back(), "   ");
        }
        // setInstrumentName(m_staffdef.back(), "&#160;&#160;&#160;");
        // setInstrumentName(m_staffdef.back(), "\xc2\xa0\xc2\xa0\xc2\xa0\xc2\xa0");
    }
    else if (hasIndent(partstart)) {
        setInstrumentName(m_staffdef.back(), "   ");
    }

    if (keysig.size() > 0) {
        setKeySig(partnumber - 1, m_staffdef.back(), keysig, keytok, false);
    }

    if (primarymensuration.empty()) {
        if (timesig.size() > 0) {
            setTimeSig(m_staffdef.back(), timesig, metersig, partstart, timetok);
        }
        if (metersig.size() > 0) {
            setMeterSymbol(m_staffdef.back(), metersig, partstart, metertok);
        }
    }
    else {
        if ((primarymensuration == "C|") || (primarymensuration == "c|")) {
            setTimeSig(m_staffdef.back(), "*M2/1", metersig, partstart);
            setMeterSymbol(m_staffdef.back(), primarymensuration, partstart, metertok);
        }
        else if ((primarymensuration == "C") || (primarymensuration == "c")) {
            setTimeSig(m_staffdef.back(), "*M4/1", metersig, partstart, metertok);
            setMeterSymbol(m_staffdef.back(), primarymensuration, partstart);
        }
        else if ((primarymensuration == "O") || (primarymensuration == "o")) {
            setTimeSig(m_staffdef.back(), "*M3/1", metersig, partstart, metertok);
            setMeterSymbol(m_staffdef.back(), primarymensuration, partstart);
        }
    }

    addInstrumentDefinition(m_staffdef.back(), partstart);

    if (partstart->isMens()) {
        m_staffdef.back()->SetNotationtype(NOTATIONTYPE_mensural_white);
    }
}

//////////////////////////////
//
// hasIndent -- true if *indent tandem interpretation before first data token.
//

bool HumdrumInput::hasIndent(hum::HTp tok)
{
    hum::HTp current = tok;
    while (current) {
        if (!current->isInterpretation()) {
            current = current->getNextToken();
            continue;
        }
        if (*current == "*indent") {
            return true;
        }
        if (*current == "*I\"") {
            // alternate version (empty part name)
            return true;
        }
        current = current->getNextToken();
        continue;
    }
    return false;
}

//////////////////////////////
//
// HumdrumInput::setInstrumentName -- for staffDef or staffGrp.
//

template <class ELEMENT> void HumdrumInput::setInstrumentName(ELEMENT *element, const string &name, hum::HTp labeltok)
{
    if (name.empty()) {
        // no instrument name to display
        return;
    }
    // hum::HumRegex hre;
    // "\xc2\xa0" is a non-breaking space
    string newname;
    Label *label = new Label();
    Text *text = new Text;
    if (name == "   ") {
        text->SetText(L"\u00a0\u00a0\u00a0");
    }
    else {
        // newname = hre.replaceCopy(name, "\xc2\xa0", " ", "g");
        text->SetText(UTF8to16(name));
    }
    if (labeltok) {
        setLocationId(label, labeltok);
    }
    label->AddChild(text);
    element->InsertChild(label, 0);
}

//////////////////////////////
//
// HumdrumInput::setInstrumentAbbreviation -- for staffDef or staffGrp.
//

template <class ELEMENT>
void HumdrumInput::setInstrumentAbbreviation(ELEMENT *element, const string &name, hum::HTp abbrtok)
{
    LabelAbbr *label = new LabelAbbr();
    Text *text = new Text;
    if (abbrtok) {
        setLocationId(label, abbrtok);
    }
    text->SetText(UTF8to16(name));
    label->AddChild(text);
    element->InsertChild(label, 0);
}

//////////////////////////////
//
// HumdrumInput::getAutoClef -- estimate a clef for a part
//     that does not have a specified clef.  Choice will be
//     treble or bass.
//

string HumdrumInput::getAutoClef(hum::HTp partstart, int partnumber)
{
    hum::HTp tok = partstart;
    int ptrack = partstart->getTrack();
    std::vector<int> dhist(100, 0);
    int diatonic;
    hum::HumRegex hre;
    while (tok) {
        if (tok->isInterpretation()) {
            if (tok->compare(0, 5, "*clef") == 0) {
                if (hre.search(tok, 5, "\\d")) {
                    break;
                }
            }
        }
        if (!tok->isData()) {
            tok = tok->getNextToken();
            continue;
        }
        if (!(tok->isNull() || tok->isRest())) {
            diatonic = hum::Convert::kernToBase7(tok);
            if ((diatonic > 0) && (diatonic < 100)) {
                dhist[diatonic]++;
            }
        }
        hum::HTp ftok = tok->getNextFieldToken();
        while (ftok && (ptrack == ftok->getTrack())) {
            if (!(ftok->isNull() || ftok->isRest())) {
                diatonic = hum::Convert::kernToBase7(ftok);
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
    for (int i = 0; i < 100; ++i) {
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
    int E3 = hum::Convert::kernToBase7("E");
    int G4 = hum::Convert::kernToBase7("g");
    if ((low > E3) && (high > G4)) {
        return "clefG2";
    }
    if ((low < E3) && (high < G4)) {
        return "clefF4";
    }
    // calculate weighted mean
    int wsum = 0;
    for (int i = low; i <= high; ++i) {
        wsum += dhist[i];
    }
    if (wsum == 0) {
        // no pitches...
        return "";
    }
    int topsum = 0;
    for (int i = low; i <= high; ++i) {
        topsum += i * wsum;
    }
    int C4 = hum::Convert::kernToBase7("c");
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

void HumdrumInput::addInstrumentDefinition(StaffDef *staffdef, hum::HTp partstart)
{

    hum::HTp instcode = NULL;
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

    // Allowing users to assign MIDI instrument numbers in data would be useful, but
    // currently only allowed via insturment codes.
    static hum::HumInstrument imap;
    int gmpc = imap.getGM(*instcode);

    //   gmpc is -1 if no mapping, so don't add General MIDI insturment number in that case
    if (gmpc < 0) {
        return;
    }

    InstrDef *idef = new InstrDef;
    staffdef->AddChild(idef);
    int offset = 0; // 1 in MEI 3, and 0 in MEI 4
    idef->SetMidiInstrnum(gmpc + offset);
    data_MIDINAMES idval = (data_MIDINAMES)(gmpc + 1);
    idef->SetMidiInstrname(idval);
}

//////////////////////////////
//
// HumdrumInput::setMeterSymbol -- common time or cut time for CMN.
//

template <class ELEMENT>
void HumdrumInput::setMeterSymbol(ELEMENT *element, const std::string &metersig, hum::HTp partstart, hum::HTp metertok)
{
    if ((partstart != NULL) && partstart->isMens()) {
        setMensurationSymbol(element, metersig, metertok);
        return;
    }

    // handle mensuration displays:
    if (metersig.find("C") != std::string::npos) {
        setMensurationSymbol(element, metersig, metertok);
        return;
    }
    if (metersig.find("O") != std::string::npos) {
        setMensurationSymbol(element, metersig, metertok);
        return;
    }

    MeterSig *vrvmetersig = getMeterSig(element);
    if (!vrvmetersig) {
        return;
    }

    if (metersig == "C") {
        // This is used more strictly for C mensuration.
        vrvmetersig->SetSym(METERSIGN_common);
    }
    else if (metersig == "c") {
        vrvmetersig->SetSym(METERSIGN_common);
    }
    else if (metersig == "c|") {
        vrvmetersig->SetSym(METERSIGN_cut);
    }
    else if (metersig == "C|") {
        // This is used more strictly for Cut-C mensuration.
        vrvmetersig->SetSym(METERSIGN_cut);
    }
    else if (metersig == "*omet(C)") {
        // This is used more strictly for C mensuration.
        vrvmetersig->SetSym(METERSIGN_common);
    }
    else if (metersig == "*omet(c)") {
        vrvmetersig->SetSym(METERSIGN_common);
    }
    else if (metersig == "*omet(c|)") {
        vrvmetersig->SetSym(METERSIGN_cut);
    }
    else if (metersig == "*omet(C|)") {
        // This is used more strictly for Cut-C mensuration.
        vrvmetersig->SetSym(METERSIGN_cut);
    }
}

//////////////////////////////
//
// HumdrumInput::setMensurationSymbol --
//

template <class ELEMENT>
void HumdrumInput::setMensurationSymbol(ELEMENT *element, const std::string &metersig, hum::HTp mensurtok)
{

    Mensur *vrvmensur = getMensur(element);
    if (!vrvmensur) {
        return;
    }
    if (mensurtok) {
        setLocationId(vrvmensur, mensurtok);
    }

    if (metersig.find('C') != std::string::npos) {
        vrvmensur->SetSign(MENSURATIONSIGN_C);
    }
    else if (metersig.find('O') != std::string::npos) {
        vrvmensur->SetSign(MENSURATIONSIGN_O);
    }
    else {
        std::cerr << "Warning: do not understand mensuration " << metersig << std::endl;
        return;
    }

    if (metersig.find('|') != std::string::npos) {
        vrvmensur->SetSlash(1);
    }
    if (metersig.find('.') != std::string::npos) {
        vrvmensur->SetDot(BOOLEAN_true);
    }
    if (metersig.find('r') != std::string::npos) {
        vrvmensur->SetOrient(ORIENTATION_reversed);
    }

    hum::HumRegex hre;
    if (hre.search(metersig, "(\\d+)/(\\d+)")) {
        vrvmensur->SetNum(hre.getMatchInt(1));
        vrvmensur->SetNumbase(hre.getMatchInt(2));
    }
    else if (hre.search(metersig, "/(\\d+)")) {
        vrvmensur->SetNumbase(hre.getMatchInt(1));
    }
    else if (hre.search(metersig, "(\\d+)")) {
        vrvmensur->SetNum(hre.getMatchInt(1));
    }
}

//////////////////////////////
//
// HumdrumInput::setTimeSig -- Convert a Humdrum timesig to an MEI timesig.
//

void HumdrumInput::setTimeSig(
    StaffDef *part, const std::string &timesig, const std::string &metersig, hum::HTp partstart, hum::HTp timetok)
{
    if ((partstart != NULL) && partstart->isMens()) {
        // Don't display time signatures in mensural notation.
        return;
    }

    // Search for a MeterSig child in StaffDef and add one if it does not exist.
    MeterSig *vrvmeter = getMeterSig(part);
    if (!vrvmeter) {
        return;
    }
    if (timetok) {
        setLocationId(vrvmeter, timetok);
    }

    // Don't store time signature if there is a mensuration to show
    // (verivio will display both mensuration and time signature.
    bool mensuration = false;
    if (metersig.find("C") != std::string::npos) {
        mensuration = true;
        ;
    }
    if (metersig.find("O") != std::string::npos) {
        mensuration = true;
        ;
    }

    int top = -1000;
    int bot = -1000;
    int bot2 = -1000;
    if (sscanf(timesig.c_str(), "*M%d/%d%%%d", &top, &bot, &bot2) == 3) {
        // Such as three-triplet whole notes in a 2/1 measure
        // deal with this later
    }
    else if (sscanf(timesig.c_str(), "*M%d/%d", &top, &bot) == 2) {
        if (bot == 0) {
            if (!mensuration) {
                // Can't add if there is a mensuration; otherwise,
                // a time signature will be shown.
                vrvmeter->SetCount(top * 2);
            }
            vrvmeter->SetUnit(1);
        }
        else {
            if (!mensuration) {
                // Can't add if there is a mensuration; otherwise,
                // a time signature will be shown.
                vrvmeter->SetCount(top);
            }
            vrvmeter->SetUnit(bot);
        }
    }
    else {
        // some strange time signature which should never occur.
    }
}

//////////////////////////////
//
// HumdrumInput::setKeySig -- Convert a Humdrum keysig to an MEI keysig.
//

template <class ELEMENT>
void HumdrumInput::setKeySig(int partindex, ELEMENT element, const std::string &keysig, hum::HTp keytok, bool secondary)
{

    std::string ks = keysig;
    auto pos = ks.find("]");
    if (pos != std::string::npos) {
        ks = ks.substr(0, pos);
        pos = ks.find("[");
        if (pos != std::string::npos) {
            ks = ks.substr(pos + 1);
        }
    }

    int keynum = -1000;
    if (ks == "") {
        keynum = 0;
    }
    else if (ks == "b-") {
        keynum = -1;
    }
    else if (ks == "f#") {
        keynum = +1;
    }
    else if (ks == "b-e-") {
        keynum = -2;
    }
    else if (ks == "f#c#") {
        keynum = +2;
    }
    else if (ks == "b-e-a-") {
        keynum = -3;
    }
    else if (ks == "f#c#g#") {
        keynum = +3;
    }
    else if (ks == "b-e-a-d-") {
        keynum = -4;
    }
    else if (ks == "f#c#g#d#") {
        keynum = +4;
    }
    else if (ks == "b-e-a-d-g-") {
        keynum = -5;
    }
    else if (ks == "f#c#g#d#a#") {
        keynum = +5;
    }
    else if (ks == "b-e-a-d-g-c-") {
        keynum = -6;
    }
    else if (ks == "f#c#g#d#a#e#") {
        keynum = +6;
    }
    else if (ks == "b-e-a-d-g-c-f-") {
        keynum = -7;
    }
    else if (ks == "f#c#g#d#a#e#b#") {
        keynum = +7;
    }

    // Search for a KeySig child in StaffDef and add one if it does not exist.
    KeySig *vrvkeysig = getKeySig(element);
    if (!vrvkeysig) {
        return;
    }
    if (keytok) {
        setLocationId(vrvkeysig, keytok);
    }

    int keyvalue = keynum;
    if ((partindex >= 0) && (m_transpose[partindex] != 0)) {
        keyvalue += hum::Convert::base40IntervalToLineOfFifths(m_transpose[partindex]);
    }
    if ((keyvalue >= -7) && (keyvalue <= +7)) {
        // standard key signature
        if (keyvalue < 0) {
            vrvkeysig->SetSig(std::make_pair(-keyvalue, ACCIDENTAL_WRITTEN_f));
        }
        else if (keyvalue > 0) {
            vrvkeysig->SetSig(std::make_pair(keyvalue, ACCIDENTAL_WRITTEN_s));
        }
        else if (keyvalue == 0) {
            vrvkeysig->SetSig(std::make_pair(keyvalue, ACCIDENTAL_WRITTEN_NONE));
        }
        else {
            vrvkeysig->SetSig(std::make_pair(-1, ACCIDENTAL_WRITTEN_NONE));
        }
    }
    else {
        // Non-standard keysignature, so give a NONE style (deal with it later).
        prepareNonStandardKeySignature(vrvkeysig, ks, keytok);
        return;
    }

    if (secondary && (keyvalue == 0)) {
        // Force cancellation keysignature when there are no
        // sharps/flats in key signature change.
        vrvkeysig->SetSigShowchange(BOOLEAN_true);
    }
    else if (m_show_cautionary_keysig) {
        vrvkeysig->SetSigShowchange(BOOLEAN_true);
    }
}

//////////////////////////////
//
// prepareNonStandardKeySignature --
//

void HumdrumInput::prepareNonStandardKeySignature(KeySig *vrvkeysig, const std::string &ks, hum::HTp keytok)
{
    if (!vrvkeysig) {
        return;
    }
    std::vector<std::string> pieces;

    for (int i = 0; i < (int)ks.size(); i++) {
        if ((ks[i] >= 'a') && (ks[i] <= 'g')) {
            pieces.resize(pieces.size() + 1);
        }
        if (pieces.size() == 0) {
            continue;
        }
        pieces.back() += ks[i];
    }

    for (int i = 0; i < (int)pieces.size(); i++) {
        if (pieces[i].empty()) {
            // strange error, ignore
            continue;
        }
        KeyAccid *kacc = new KeyAccid;
        vrvkeysig->AddChild(kacc);

        int pclass = pieces[i][0] - 'a';
        switch (pclass) {
            case 0: kacc->SetPname(PITCHNAME_a); break;
            case 1: kacc->SetPname(PITCHNAME_b); break;
            case 2: kacc->SetPname(PITCHNAME_c); break;
            case 3: kacc->SetPname(PITCHNAME_d); break;
            case 4: kacc->SetPname(PITCHNAME_e); break;
            case 5: kacc->SetPname(PITCHNAME_f); break;
            case 6: kacc->SetPname(PITCHNAME_g); break;
        }

        for (int j = 0; j < (int)pieces[i].size(); j++) {
            if (std::isdigit(pieces[i][j])) {
                int octave = pieces[i][j] - '0';
                kacc->SetOct(octave);
                break;
            }
        }

        if (pieces[i].find("##") != std::string::npos) {
            kacc->SetAccid(ACCIDENTAL_WRITTEN_x);
        }
        else if (pieces[i].find("--") != std::string::npos) {
            kacc->SetAccid(ACCIDENTAL_WRITTEN_ff);
        }
        else if (pieces[i].find("#") != std::string::npos) {
            kacc->SetAccid(ACCIDENTAL_WRITTEN_s);
        }
        else if (pieces[i].find("-") != std::string::npos) {
            kacc->SetAccid(ACCIDENTAL_WRITTEN_f);
        }
        else if (pieces[i].find("n") != std::string::npos) {
            kacc->SetAccid(ACCIDENTAL_WRITTEN_n);
        }
        else {
            kacc->SetAccid(ACCIDENTAL_WRITTEN_n);
        }

        if (pieces[i].find("X") != std::string::npos) {
            kacc->SetEnclose(ENCLOSURE_brack);
        }
    }
}

//////////////////////////////
//
// HumdrumInput::setTransposition -- Set the transposition to sounding score.
//

void HumdrumInput::setTransposition(StaffDef *part, const std::string &transpose)
{
    int chromatic = 0;
    int diatonic = 0;
    if (sscanf(transpose.c_str(), "*Trd%dc%dd", &diatonic, &chromatic) != 2) {
        // Transposition is not formatted correctly
        return;
    }
    part->SetTransDiat(-diatonic);
    part->SetTransSemi(-chromatic);
}

//////////////////////////////
//
// HumdrumInput::setDynamicTransposition --
//

void HumdrumInput::setDynamicTransposition(int partindex, StaffDef *part, const std::string &itranspose)
{
    int chromatic = 0;
    int diatonic = 0;
    if (sscanf(itranspose.c_str(), "*ITrd%dc%dd", &diatonic, &chromatic) != 2) {
        // Transposition is not formatted correctly
        return;
    }
    part->SetTransDiat(-diatonic);
    part->SetTransSemi(-chromatic);

    // Store dynamic transposition to go from sounding score to written:
    m_transpose[partindex] = hum::Convert::transToBase40(itranspose);
}

//////////////////////////////
//
// HumdrumInput::setClef -- Convert a Humdrum clef to an MEI clef.
//

void HumdrumInput::setClef(StaffDef *part, const std::string &clef, hum::HTp cleftok)
{
    // Search for a Clef child in StaffDef and add one if it does not exist.
    Clef *vrvclef = getClef(part);
    if (!vrvclef) {
        return;
    }
    if (cleftok) {
        setLocationId(vrvclef, cleftok);
    }

    if (clef.find("clefGG") != string::npos) {
        vrvclef->SetShape(CLEFSHAPE_GG);
    }
    else if (clef.find("clefG") != string::npos) {
        vrvclef->SetShape(CLEFSHAPE_G);
    }
    else if (clef.find("clefF") != string::npos) {
        vrvclef->SetShape(CLEFSHAPE_F);
    }
    else if (clef.find("clefC") != string::npos) {
        vrvclef->SetShape(CLEFSHAPE_C);
    }
    if (clef.find("clefX") != string::npos) {
        vrvclef->SetShape(CLEFSHAPE_perc);
        // by default place on 3rd line (unless another numbe is given):
        vrvclef->SetLine(3);
    }

    if (clef.find("2") != string::npos) {
        vrvclef->SetLine(2);
    }
    else if (clef.find("4") != string::npos) {
        vrvclef->SetLine(4);
    }
    else if (clef.find("3") != string::npos) {
        vrvclef->SetLine(3);
    }
    else if (clef.find("5") != string::npos) {
        vrvclef->SetLine(5);
    }
    else if (clef.find("1") != string::npos) {
        vrvclef->SetLine(1);
    }

    if (clef.find("vv") != string::npos) {
        vrvclef->SetDis(OCTAVE_DIS_15);
        vrvclef->SetDisPlace(STAFFREL_basic_below);
    }
    else if (clef.find("v") != string::npos) {
        vrvclef->SetDis(OCTAVE_DIS_8);
        vrvclef->SetDisPlace(STAFFREL_basic_below);
    }
    else if (clef.find("^^") != string::npos) {
        vrvclef->SetDis(OCTAVE_DIS_15);
        vrvclef->SetDisPlace(STAFFREL_basic_above);
    }
    else if (clef.find("^") != string::npos) {
        vrvclef->SetDis(OCTAVE_DIS_8);
        vrvclef->SetDisPlace(STAFFREL_basic_above);
    }
}

//////////////////////////////
//
// HumdrumInput::convertSystemMeasure -- Convert one measure of
//     a Humdrum score into an MEI measure element.
//

bool HumdrumInput::convertSystemMeasure(int &line)
{
    hum::HumdrumFile &infile = m_infiles[0];
    int startline = line;
    int endline = getMeasureEndLine(startline);
    if (endline > infile.getLineCount()) {
        return false;
    }
    if (endline < 0) {
        // empty measure, skip it.  This can happen at the start of
        // a score if there is an invisible measure before the start of the
        // data, or if there is an ending bar before the ending of the data.
        line = -endline;
        return true;
    }
    else if (m_ignore[startline]) {
        // don't convert this measure (!!ignore/!!Xignore toggles)
        line = endline;
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

    bool founddatabefore = false;
    for (int q = startline; q >= 0; q--) {
        if (infile[q].isData()) {
            founddatabefore = true;
            break;
        }
    }
    if (!founddatabefore) {
        startline = 0;
    }

    setupSystemMeasure(startline, endline);

    storeStaffLayerTokensForMeasure(startline, endline);

    auto status = convertMeasureStaves(startline, endline);

    if (m_breaks) {
        checkForLayoutBreak(endline);
    }
    return status;
}

//////////////////////////////
//
// HumdrumInput::checkForLayoutBreak --
//

void HumdrumInput::checkForLayoutBreak(int line)
{
    hum::HumdrumFile &infile = m_infiles[0];
    if (line >= infile.getLineCount()) {
        return;
    }
    if (!infile[line].isBarline()) {
        return;
    }
    hum::HTp token = infile.token(line, 0);
    string group;

    group = token->getLayoutParameter("LB", "g");
    if (!group.empty()) {
        std::string tstring = removeCommas(group);
        Sb *sb = new Sb;
        m_sections.back()->AddChild(sb);
        sb->SetType(tstring);
        return;
    }

    group = token->getLayoutParameter("PB", "g");
    if (!group.empty()) {
        std::string tstring = removeCommas(group);
        Sb *sb = new Sb;
        m_sections.back()->AddChild(sb);
        sb->SetType(tstring);
        return;
    }
}

//////////////////////////////
//
// HumdrumInput::removeCommas --
//

std::string HumdrumInput::removeCommas(const std::string &input)
{
    std::string output = input;
    for (int i = 0; i < (int)output.size(); i++) {
        if (output[i] == ',') {
            output[i] = ' ';
        }
        else if (output[i] == ';') {
            output[i] = ' ';
        }
        else if (output[i] == ':') {
            output[i] = ' ';
        }
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::checkForOmd --
//

void HumdrumInput::checkForOmd(int startline, int endline)
{
    hum::HumdrumFile &infile = m_infiles[0];
    if (m_omd > infile[startline].getDurationFromStart()) {
        return;
    }
    if (m_omd < 0) {
        startline = 0;
    }

    const std::vector<hum::HTp> &staffstarts = m_staffstarts;
    if (staffstarts.size() == 0) {
        return;
    }
    std::string key;
    std::string value;
    for (int i = startline; i <= endline; ++i) {
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
            setLocationId(tempo, infile.token(i, 0));
            m_measure->AddChildBack(tempo);
            addTextElement(tempo, value);
            tempo->SetTstamp(1.0);
            setStaff(tempo, 1);
            m_omd = infile[i].getDurationFromStart();
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
// HumdrumInput::setN --
//

template <class ELEMENT> void HumdrumInput::setN(ELEMENT element, int nvalue, hum::HTp tok)
{
    std::string name = element->GetClassName();
    if (tok && (name == "Ending")) {
        // Check if there is a LO:TX text to replace number.
        std::string textlabel = tok->getLayoutParameter("TX", "t");
        if (!textlabel.empty()) {
            element->SetN(textlabel);
        }
        else {
            element->SetN(to_string(nvalue));
        }
    }
    else {
        element->SetN(to_string(nvalue));
    }
}

//////////////////////////////
//
// HumdrumInput::storeStaffLayerTokensForMeasure -- Store lists of notation
//   data by staff and layer.
//

void HumdrumInput::storeStaffLayerTokensForMeasure(int startline, int endline)
{
    hum::HumdrumFile &infile = m_infiles[0];
    const std::vector<hum::HTp> &staffstarts = m_staffstarts;
    const std::vector<int> &rkern = m_rkern;
    std::vector<std::vector<std::vector<hum::HTp> > > &lt = m_layertokens;

    lt.clear();
    lt.resize(staffstarts.size());

    int i, j;
    for (i = 0; i < (int)staffstarts.size(); ++i) {
        lt[i].clear();
    }

    int lasttrack = -1;
    int track = -1;
    int staffindex = -1;
    int layerindex = 0;
    for (i = startline; i <= endline; ++i) {
        if (!infile[i].hasSpines()) {
            continue;
        }
        lasttrack = -1;
        for (j = 0; j < infile[i].getFieldCount(); ++j) {
            track = infile[i].token(j)->getTrack();
            if (track < 1) {
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
                if (infile[i].token(j)->getLinkedParameterCount() == 0) {
                    continue;
                }
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
            if ((layerindex == 0) && (infile[i].token(j)->isClef())) {
                // Duplicate clef in all layers (needed for cases when
                // a secondary layer ends before the end of a measure.
                for (int k = 1; k < (int)lt[staffindex].size(); k++) {
                    lt[staffindex][k].push_back(infile[i].token(j));
                }
            }
        }
    }

    if (m_debug) {
        printMeasureTokens();
    }
}

//////////////////////////////
//
// HumdrumInput::convertMeasureStaves -- fill in a measure with the
//    individual staff elements for each part.
//

bool HumdrumInput::convertMeasureStaves(int startline, int endline)
{
    const std::vector<hum::HTp> &staffstarts = m_staffstarts;

    std::vector<int> layers = getStaffLayerCounts();

    int i;

    // pre-allocate
    std::vector<Staff *> stafflist(staffstarts.size());
    for (i = 0; i < (int)staffstarts.size(); ++i) {
        stafflist[i] = new Staff();
        m_measure->AddChild(stafflist[i]);
    }

    checkForOmd(startline, endline);

    bool status = true;
    for (i = 0; i < (int)staffstarts.size(); ++i) {
        m_currentstaff = i + 1;
        m_staff = stafflist[i];
        m_staff->SetN(m_currentstaff);

        status &= convertMeasureStaff(staffstarts[i]->getTrack(), startline, endline, i + 1, layers[i]);
        if (!status) {
            break;
        }
    }

    if (m_harm) {
        addHarmFloatsForMeasure(startline, endline);
    }

    if (m_fing) {
        addFingeringsForMeasure(startline, endline);
    }

    if (m_string) {
        addStringNumbersForMeasure(startline, endline);
    }

    if (m_fb) {
        addFiguredBassForMeasure(startline, endline);
    }

    return status;
}

//////////////////////////////
//
// checkForLineContinuations -- Temporary code until fb line
//      extensions are implemented.  If the previous **fb token
//      contains a line extension for this null token, then add
//      underscores.
//

void HumdrumInput::checkForLineContinuations(hum::HTp token)
{
    if (!token->isNull()) {
        return;
    }
    hum::HTp resolved = token->resolveNull();
    if (resolved->find("_") == std::string::npos) {
        return;
    }

    int spinetrack = token->getTrack();
    Harm *harm = new Harm;
    Fb *fb = new Fb;

    if (token->isDataType("**fba")) {
        if (m_placement[spinetrack] == 0) {
            setPlace(harm, "above");
        }
        else if (m_placement[spinetrack] == -1) {
            setPlace(harm, "below");
        }
        else if (m_placement[spinetrack] == +1) {
            setPlace(harm, "above");
        }
    }
    else {
        if (m_placement[spinetrack] == -1) {
            setPlace(harm, "below");
        }
        else if (m_placement[spinetrack] == +1) {
            setPlace(harm, "above");
        }
    }
    harm->AddChild(fb);

    hum::HumRegex hre;
    std::vector<std::string> pieces;
    hre.split(pieces, *resolved, " ");
    for (int i = 0; i < (int)pieces.size(); i++) {
        if (pieces[i].find("_") != std::string::npos) {
            F *f = new F();
            Text *text = new Text();
            std::wstring wtext = L"_";
            text->SetText(wtext);
            f->AddChild(text);
            fb->AddChild(f);
        }
        else {
            // place holder
            F *f = new F();
            fb->AddChild(f);
        }
    }

    int kerntrack = -1;
    hum::HTp current = token;
    while (current) {
        if (current->isKern()) {
            kerntrack = current->getTrack();
        }
        current = current->getPreviousFieldToken();
    }

    m_measure->AddChild(harm);
    int staffindex = 0;
    if (kerntrack >= 0) {
        staffindex = m_rkern[kerntrack];
    }
    hum::HumNum tstamp = getMeasureTstamp(token, staffindex);
    harm->SetTstamp(tstamp.getFloat());
    setStaff(harm, staffindex + 1);
    setLocationId(harm, token);
    setLocationId(fb, token);
}

//////////////////////////////
//
// HumdrumInput::addFiguredBassForMeasure --
//

void HumdrumInput::addFiguredBassForMeasure(int startline, int endline)
{
    if (!m_measure) {
        return;
    }
    hum::HumdrumFile &infile = m_infiles[0];

    for (int i = startline; i < endline; ++i) {
        if (infile[i].isInterpretation()) {
            for (int j = 0; j < infile[i].getFieldCount(); ++j) {
                if (j >= 999) {
                    break;
                }
                hum::HTp token = infile.token(i, j);
                if (!(token->isDataType("**fb") || token->isDataType("**fba") || token->isDataType("**Bnum"))) {
                    continue;
                }
                int track = token->getTrack();
                if (token->compare("*above") == 0) {
                    m_placement[track] = +1;
                }
                else if (token->compare("*below") == 0) {
                    m_placement[track] = -1;
                }
                else if (token->compare("*auto") == 0) {
                    m_placement[track] = 0;
                }
                else if (token->compare("*reverse") == 0) {
                    m_reverse[track] = +1;
                }
                else if (token->compare("*Xreverse") == 0) {
                    m_reverse[track] = 0;
                }
                else if (token->compare("*absolute") == 0) {
                    m_absolute[track] = +1;
                }
                else if (token->compare("*Xabsolute") == 0) {
                    m_absolute[track] = 0;
                }
                else if (token->compare("*slash") == 0) {
                    m_slash[track] = +1;
                }
                else if (token->compare("*Xslash") == 0) {
                    m_slash[track] = 0;
                }
            }
            continue;
        }

        if (!infile[i].isData()) {
            continue;
        }
        int kerntrack = 0;
        int spinetrack = 0;

        for (int j = 0; j < infile[i].getFieldCount(); ++j) {
            hum::HTp token = infile.token(i, j);
            if (token->isDataType("**kern")) {
                kerntrack = token->getTrack();
            }
            if (!(token->isDataType("**fb") || token->isDataType("**fba") || token->isDataType("**Bnum"))) {
                continue;
            }
            if (token->isNull()) {
                checkForLineContinuations(token);
                continue;
            }
            if (token->compare("_") == 0) {
                continue;
            }
            spinetrack = token->getTrack();
            Harm *harm = new Harm;
            Fb *fb = new Fb;

            if (token->isDataType("**fba")) {
                if (m_placement[spinetrack] == 0) {
                    setPlace(harm, "above");
                }
                else if (m_placement[spinetrack] == -1) {
                    setPlace(harm, "below");
                }
                else if (m_placement[spinetrack] == +1) {
                    setPlace(harm, "above");
                }
            }
            else {
                if (m_placement[spinetrack] == -1) {
                    setPlace(harm, "below");
                }
                else if (m_placement[spinetrack] == +1) {
                    setPlace(harm, "above");
                }
            }
            harm->AddChild(fb);

            std::vector<std::string> pieces = splitFBString(*token, " ");
            std::vector<std::wstring> content = cleanFBString(pieces, token);
            if (content.empty()) {
                // do not include an empty fb see issue #1096
                continue;
            }

            for (int k = 0; k < (int)content.size(); ++k) {
                F *f = new F();
                if ((pieces[k] == "x") || (pieces[k] == "X")) {
                    // suppress contents of <f>
                }
                else {
                    Text *text = new Text();
                    auto pos = pieces[k].find(":");
                    if (pos != std::string::npos) {
                        std::vector<std::string> subpieces = splitFBString(pieces[k], ":");
                        std::wstring newtext = cleanFBString2(subpieces, token);
                        text->SetText(newtext);
                    }
                    else {
                        text->SetText(content[k]);
                    }
                    f->AddChild(text);
                }
                fb->AddChild(f);
                if (pieces[k].find("_") != std::string::npos) {
                    // Does not display anything at the moment
                    // so a "_" character was added to content[k].
                    // This can be removed when extenders are
                    // implemented.
                    // f->SetExtender(BOOLEAN_true);
                }
                if (content.size() == 1) {
                    setLocationId(f, token);
                }
                else {
                    setLocationIdNSuffix(f, token, k + 1);
                }
            }

            m_measure->AddChild(harm);
            int staffindex = m_rkern[kerntrack];
            if (m_placement.at(spinetrack)) {
                m_fbstates.at(staffindex) = m_placement.at(spinetrack);
            }
            hum::HumNum tstamp = getMeasureTstamp(token, staffindex);
            harm->SetTstamp(tstamp.getFloat());
            setStaff(harm, staffindex + 1);
            setLocationId(harm, token);
            setLocationId(fb, token);
        }
    }
}

//////////////////////////////
//
// HumdrumInput::addFingeringsForMeasure --
//

void HumdrumInput::addFingeringsForMeasure(int startline, int endline)
{
    if (!m_measure) {
        return;
    }
    const std::vector<hum::HTp> &staffstarts = m_staffstarts;
    hum::HumdrumFile &infile = m_infiles[0];
    bool aboveQ = true;
    hum::HumRegex hre;
    vector<std::string> nums;

    for (int i = startline; i < endline; ++i) {
        if (!infile[i].isData()) {
            continue;
        }
        int track = 0;
        for (int j = 0; j < infile[i].getFieldCount(); ++j) {
            hum::HTp token = infile.token(i, j);
            if (token->isDataType("**kern")) {
                track = token->getTrack();
            }
            if (token->isNull()) {
                continue;
            }
            if (!token->isDataType("**fing")) {
                continue;
            }
            std::string content = token->getText();
            if (content.empty()) {
                continue;
            }

            hre.split(nums, content, " ");

            int ztrack = token->getTrack();
            aboveQ = true;
            if ((j > 0) && infile.token(i, j - 1)->isDataType("**fing")) {
                int ptrack = infile.token(i, j - 1)->getTrack();
                if (ztrack == ptrack) {
                    aboveQ = false;
                }
            }

            int staffindex = m_rkern[track];
            int maxstaff = (int)staffstarts.size();

            if (aboveQ) {
                for (int k = 0; k < (int)nums.size(); k++) {
                    insertFingerNumberInMeasure(nums[k], staffindex, token, maxstaff, aboveQ);
                }
            }
            else {
                for (int k = 0; k < (int)nums.size(); k++) {
                    insertFingerNumberInMeasure(nums[k], staffindex, token, maxstaff, aboveQ);
                }
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::insertFingerNumberInMeasure --
//

void HumdrumInput::insertFingerNumberInMeasure(
    const std::string &text, int staffindex, hum::HTp token, int maxstaff, bool aboveQ)
{

    Dir *dir = new Dir;
    int xstaffindex = 0;
    if (staffindex >= 0) {
        xstaffindex = staffindex;
        setStaff(dir, staffindex + 1);
    }
    else {
        // data is not attached to a **kern spine since it comes before
        // any **kern data.  Treat it as attached to the bottom staff.
        // (or the top staff depending on @place="above|below".
        xstaffindex = maxstaff - 1;
        setStaff(dir, xstaffindex + 1);
    }

    Rend *rend = new Rend;
    data_FONTSIZE fs;
    fs.SetTerm(FONTSIZETERM_x_small);
    rend->SetFontsize(fs);
    rend->SetFontstyle(FONTSTYLE_normal);
    addTextElement(rend, text);
    dir->AddChild(rend);
    appendTypeTag(dir, "fingering");
    if (aboveQ) {
        setPlace(dir, "above");
    }
    else {
        setPlace(dir, "below");
    }
    m_measure->AddChild(dir);
    setLocationId(dir, token);

    // Previously used @tstamp, now use @startid of note/chord;
    // hum::HumNum tstamp = getMeasureTstamp(token, xstaffindex);
    // dir->SetTstamp(tstamp.getFloat());
    linkFingeringToNote(dir, token, xstaffindex);
}

//////////////////////////////
//
// HumdrumInput::linkFingeringToNote -- link to the note/chord in the highest layer
//    (left-most spine) that is not a null data token. If no notes/chords at
//    the position of the finger (such as perhaps due to a finger-change, then
//    use a @tstamp rather than a @startid to place the fingering.
//

void HumdrumInput::linkFingeringToNote(Dir *dir, hum::HTp token, int xstaffindex)
{
    // token should be a **fing, so search for the **kern that it
    // matches to the left, and then search for the last non-null
    // kern token to link to the fingering (figure out later how to link
    // to notes in secondary layers which will probably be done by
    // placing fingerings in subspines of the **fing spine.

    hum::HTp linkednote = NULL;
    int linktrack = -1;

    hum::HumdrumLine &line = *token->getLine();
    int startfield = token->getFieldIndex();

    for (int i = startfield - 1; i >= 0; i--) {
        hum::HTp testtok = line.token(i);
        if (!testtok->isKern()) {
            continue;
        }
        linktrack = testtok->getTrack();
        for (int j = i; j >= 0; j--) {
            testtok = line.token(j);
            int ttrack = testtok->getTrack();
            if (ttrack != linktrack) {
                break;
            }
            if (!testtok->isNull()) {
                linkednote = testtok;
            }
        }
        break;
    }
    if (!linkednote) {
        // use a timestamp to place the fingering
        hum::HumNum tstamp = getMeasureTstamp(token, xstaffindex);
        dir->SetTstamp(tstamp.getFloat());
    }
    else {
        std::string startid;
        if (linkednote->find(" ") != std::string::npos) {
            startid = getLocationId("chord", linkednote);
        }
        else {
            startid = getLocationId("note", linkednote);
        }
        dir->SetStartid("#" + startid);
    }
}

//////////////////////////////
//
// HumdrumInput::addStringNumbersForMeasure --
//

void HumdrumInput::addStringNumbersForMeasure(int startline, int endline)
{
    if (!m_measure) {
        return;
    }
    int xstaffindex;
    const std::vector<hum::HTp> &staffstarts = m_staffstarts;
    hum::HumdrumFile &infile = m_infiles[0];
    for (int i = startline; i < endline; ++i) {
        if (!infile[i].isData()) {
            continue;
        }
        int track = 0;
        for (int j = 0; j < infile[i].getFieldCount(); ++j) {
            hum::HTp token = infile.token(i, j);
            if (token->isDataType("**kern")) {
                track = token->getTrack();
            }
            if (token->isNull()) {
                continue;
            }
            if (!token->isDataType("**string")) {
                continue;
            }
            Harm *harm = new Harm;
            Text *text = new Text;

            int staffindex = m_rkern[track];

            if (staffindex >= 0) {
                xstaffindex = staffindex;
                setStaff(harm, staffindex + 1);
            }
            else {
                // data is not attached to a **kern spine since it comes before
                // any **kern data.  Treat it as attached to the bottom staff.
                // (or the top staff depending on @place="above|below".
                xstaffindex = (int)staffstarts.size() - 1;
                setStaff(harm, xstaffindex + 1);
            }
            std::wstring content;
            content = cleanStringString(*token);
            text->SetText(content);
            harm->AddChild(text);
            m_measure->AddChild(harm);
            hum::HumNum tstamp = getMeasureTstamp(token, xstaffindex);
            harm->SetTstamp(tstamp.getFloat());
            appendTypeTag(harm, "string");
            setLocationId(harm, token);
        }
    }
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
    hum::HumRegex hre;
    int xstaffindex;
    const std::vector<hum::HTp> &staffstarts = m_staffstarts;
    hum::HumdrumFile &infile = m_infiles[0];
    for (int i = startline; i < endline; ++i) {
        if (infile[i].isInterpretation()) {
            for (int j = 0; j < infile[i].getFieldCount(); j++) {
                if (hre.search(infile.token(i, j), "^\\*color:(.*)")) {
                    int ctrack = infile.token(i, j)->getTrack();
                    m_spine_color[ctrack] = hre.getMatch(1);
                }
            }
        }
        if (!infile[i].isData()) {
            continue;
        }
        int track = 0;
        for (int j = 0; j < infile[i].getFieldCount(); ++j) {
            hum::HTp token = infile.token(i, j);
            if (token->isDataType("**kern")) {
                track = token->getTrack();
            }
            if (token->isNull()) {
                continue;
            }
            if (!(token->isDataType("**mxhm") || token->isDataType("**harm") || token->isDataType("**rhrm")
                    || (token->getDataType().compare(0, 7, "**cdata") == 0))) {
                continue;
            }
            Harm *harm = new Harm;
            Text *text = new Text;

            m_measure->AddChild(harm);

            int line = token->getLineIndex();
            int field = token->getFieldIndex();
            string ccolor = getSpineColor(line, field);
            if (!ccolor.empty()) {
                Rend *rend = new Rend;
                rend->SetColor(ccolor);
                harm->AddChild(rend);
                rend->AddChild(text);
            }
            else {
                harm->AddChild(text);
            }
            string tracktext = getTrackText(token);
            harm->SetN(tracktext);

            int staffindex = m_rkern[track];

            if (staffindex >= 0) {
                xstaffindex = staffindex;
                setStaff(harm, staffindex + 1);
            }
            else {
                // data is not attached to a **kern spine since it comes before
                // any **kern data.  Treat it as attached to the bottom staff.
                // (or the top staff depending on @place="above|below".
                xstaffindex = (int)staffstarts.size() - 1;
                setStaff(harm, xstaffindex + 1);
            }

            string datatype = token->getDataType();
            if (datatype.compare(0, 8, "**cdata-") == 0) {
                string subdatatype = datatype.substr(8);
                if (!subdatatype.empty()) {
                    appendTypeTag(harm, subdatatype);
                }
            }

            std::wstring content;
            if (token->isDataType("**harm")) {
                setPlace(harm, "below");
                content = cleanHarmString2(*token);
            }
            else if (token->isDataType("**rhrm")) {
                setPlace(harm, "below");
                content = cleanHarmString3(*token);
            }
            else {
                content = cleanHarmString(*token);
            }
            text->SetText(content);

            hum::HumNum tstamp = getMeasureTstamp(token, xstaffindex);
            harm->SetTstamp(tstamp.getFloat());

            setLocationId(harm, token);
        }
    }
}

//////////////////////////////
//
// HumdrumInput::getTrackText --
//

std::string HumdrumInput::getTrackText(hum::HTp token)
{
    std::string trackinfo = token->getSpineInfo();
    int track = token->getTrack();
    std::string output = to_string(track);
    std::string extension = "";
    for (int i = 0; i < (int)trackinfo.size(); i++) {
        if (trackinfo[i] == 'a') {
            extension += 'a';
        }
        else if (trackinfo[i] == 'b') {
            extension += 'b';
        }
    }
    if (extension != "a") {
        output += extension;
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::splitFBString --
//    default value: separator = " "
//

std::vector<std::string> HumdrumInput::splitFBString(const std::string &content, const string &separator)
{
    hum::HumRegex hre;
    std::vector<std::string> pieces;
    hre.split(pieces, content, separator);
    return pieces;
}

//////////////////////////////
//
// HumdrumInput::cleanFBString --
//

std::vector<std::wstring> HumdrumInput::cleanFBString(std::vector<std::string> &pieces, hum::HTp token)
{
    std::vector<bool> todelete(pieces.size(), false);
    std::vector<std::wstring> output(pieces.size());
    for (int i = 0; i < (int)pieces.size(); i++) {
        output[i] = convertFBNumber(pieces[i], token);
        if ((pieces[i].find("K") != std::string::npos)
            && ((pieces[i].find("x") == std::string::npos) && (pieces[i].find("X") == std::string::npos))) {
            // this figure slot needs to be deleted
            todelete[i] = true;
        }
    }
    // delete unwanted slots.
    for (int i = (int)todelete.size() - 1; i >= 0; i--) {
        if (!todelete[i]) {
            continue;
        }
        output.erase(output.begin() + i);
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::cleanFBString2 -- Used to concantenate multiple figures
//   together with hyphens.
//

std::wstring HumdrumInput::cleanFBString2(std::vector<std::string> &pieces, hum::HTp token)
{
    std::wstring output;
    for (int i = 0; i < (int)pieces.size(); i++) {
        output += convertFBNumber(pieces[i], token);
        if (i < (int)pieces.size() - 1) {
            if (pieces[i + 1] == "") {
                output += L" ";
            }
            output += L"-";
        }
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::convertFBNumber --
//

std::wstring HumdrumInput::convertFBNumber(const string &input, hum::HTp token)
{
    std::wstring output;

    int track = token->getTrack();
    int reverse = m_reverse[track];

    if (input.find("K") != std::string::npos) {
        return output;
    }

    bool found = false;
    int digit = 0;
    for (int i = 0; i < (int)input.size(); i++) {
        if (isdigit(input[i])) {
            // digits have to be adjacent
            found = true;
            if ((i > 0) && isdigit(input[i - 1])) {
                digit = 10 * digit + (input[i] - '0');
            }
            else {
                digit = input[i] - '0';
            }
        }
    }
    if (!found) {
        digit = -1;
    }

    int slash = 0;
    // slash = 1 :: forward slash /
    // slash = 2 :: back slash \ .
    // slash = 3 :: vertical slash |
    if (input.find("/") != std::string::npos) {
        slash = 1;
    }
    else if (input.find("\\") != std::string::npos) {
        slash = 2;
    }
    else if (input.find("|") != std::string::npos) {
        slash = 3;
    }

    if (!m_slash[track]) {
        // disable slashes
        slash = 0;
    }

    if (digit > 9) {
        // Don't allow slashes on multi-digit numbers.
        // Maybe allow in the future.
        slash = 0;
    }

    int accidental = 0;
    // accidental = 1 :: double-flat
    // accidental = 2 :: flat (or lower)
    // accidental = 3 :: natural
    // accidental = 4 :: sharp (or raise)
    // accidental = 5 :: double-sharp
    // accidental = 6 :: plus (sharp or raise))
    // also allow "+" later
    if (input.find("+") != std::string::npos) {
        accidental = 6;
    }
    else if (input.find("--") != std::string::npos) {
        accidental = 1;
    }
    else if (input.find("##") != std::string::npos) {
        accidental = 5;
    }
    else if (input.find("-") != std::string::npos) {
        accidental = 2;
    }
    else if (input.find("#") != std::string::npos) {
        accidental = 4;
    }
    else if (input.find("n") != std::string::npos) {
        // other accidentals have priority over "n"
        // since they would be visual accidentals in the
        // case where "n" is also present.
        accidental = 3;
    }

    // Override visual accidentals if m_absolute is true:
    if (m_absolute[track]) {
        if (input.find("n") != std::string::npos) {
            accidental = 3;
        }
    }

    if (input.find("k") != std::string::npos) {
        // suppress display of accidental if "k" signifier is present.
        accidental = 0;
    }

    // accidental in front of number unless an "r" is present:
    if ((!slash) && (input.find("r") == std::string::npos) && (!reverse)) {
        std::wstring accid = getVisualFBAccidental(accidental);
        if (accidental && (input.find("i") != std::string::npos)) {
            accid = L"[" + accid + L"]";
        }
        else if (accidental && (input.find("j") != std::string::npos)) {
            accid = L"(" + accid + L")";
        }
        output += accid;
    }

    if (!slash) {
        // print regular number, slashed number must have accidental qualifier
        output += convertNumberToWstring(digit);
    }
    else {
        // slash should be drawn on number (but some numbers
        // do not has slashes available).
        // See: https://www.smufl.org/version/latest/range/figuredBass
        switch (digit) {
            case 0: output += L"\uEA50"; break; // draw without slash
            case 1: output += L"\uEA51"; break; // draw without slash
            case 2: output += L"\uEA53"; break;
            case 3: output += L"\uEA54"; break; // draw without slash
            case 4: output += L"\uEA56"; break; // only one style of slash
            case 5:
                switch (slash) {
                    case 1: output += L"\uEA5A"; break; // 5/
                    case 2: output += L"\uEA59"; break; // 5\ .
                    case 3: output += L"\uEA58"; break; // 5|
                    default: output += L"\uEA57"; break; // 5
                }
                break;
            case 6:
                switch (slash) {
                    case 1: output += L"\uEA5C"; break; // 6/
                    case 2: output += L"\uEA5C"; break; // 6\ .
                    case 3: output += L"\uEA5C"; break; // 6|
                    default: output += L"\uEA5B"; break; // 6
                }
                break;
            case 7:
                switch (slash) {
                    case 1: output += L"\uECC0"; break; // 7/
                    case 2: output += L"\uEA5F"; break; // 7\ .
                    case 3: output += L"\uEA5E"; break; // 7|
                    default: output += L"\uEA5D"; break; // 7
                }
                break;
            case 8: output += L"\uEA60"; break; // draw without slash
            case 9: output += L"\uEA62"; break; // only one style of slash
        }
    }

    // accidental after number if an "r" is present:
    if ((!slash) && ((input.find("r") != std::string::npos) || reverse)) {
        std::wstring accid = getVisualFBAccidental(accidental);
        if (accidental && (input.find("i") != std::string::npos)) {
            accid = L"[" + accid + L"]";
        }
        else if (accidental && (input.find("j") != std::string::npos)) {
            accid = L"(" + accid + L")";
        }
        output += accid;
    }

    if (input.find("J") != std::string::npos) {
        output = L"(" + output + L")";
    }
    else if (input.find("I") != std::string::npos) {
        output = L"[" + output + L"]";
    }
    else if (slash) {
        if (input.find("j") != std::string::npos) {
            output = L"(" + output + L")";
        }
        else if (input.find("i") != std::string::npos) {
            output = L"[" + output + L"]";
        }
    }

    // extension lines are not yet available for figured bass,
    // so display an underscore after the figure to indicate
    // that one should be added in the future:
    if (input.find("_") != std::string::npos) {
        output += L" _";
    }
    // A "=" character indicates that there is a the figure (should be
    // centered between current figure and next one, but not available yet).
    // Technically this is a sort of extender, but f@extension is
    // boolean, so various styles cannot be encoded in it.
    if (input.find("=") != std::string::npos) {
        output += L" -";
    }

    /*
                To convert a free-form string to UTF16:
                string tdee;
                output.back() += UTF8to16(tdee);
    */

    return output;
}

//////////////////////////////
//
// HumdrumInput::convertNumberToWstring --
//

std::wstring HumdrumInput::convertNumberToWstring(int number)
{
    if (number < 0) {
        return L"";
    }
    std::string value = to_string(number);
    std::wstring output;
    for (int i = 0; i < (int)value.size(); i++) {
        switch (value[i]) {
            case '0': output += L"\uEA50"; break;
            case '1': output += L"\uEA51"; break;
            case '2': output += L"\uEA52"; break;
            case '3': output += L"\uEA54"; break;
            case '4': output += L"\uEA55"; break;
            case '5': output += L"\uEA57"; break;
            case '6': output += L"\uEA5B"; break;
            case '7': output += L"\uEA5D"; break;
            case '8': output += L"\uEA60"; break;
            case '9': output += L"\uEA61"; break;
        }
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::getVisualFBAccidental --
//

std::wstring HumdrumInput::getVisualFBAccidental(int accidental)
{
    std::wstring output;
    switch (accidental) {
        case 1:
            output = L"\uE264"; // SMUFL double-flat
            break;
        case 2:
            // output = L"\u266D"; // unicode flat
            output = L"\uE260"; // SMUFL flat
            break;
        case 3:
            // output = L"\u266E"; // unicode natural
            output = L"\uE261"; // SMUFL natural
            break;
        case 4:
            // output = L"\u266F"; // unicode sharp
            output = L"\uE262"; // SMUFL sharp
            break;
        case 5:
            output = L"\uE263"; // SMUFL double-sharp
            break;
        case 6:
            output = L"+"; // UTF-7 +
            break;
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::cleanHarmString3 -- Adjust **rhrm text to remove
//     **recip data and then clean with cleanHarmString2.
//     **rhrm cannot contain alternate chords using [vi] syntax,
//     because [, _, ], are for tied notes, **rhrm tokens with _ or ]
//     will be suppressed.
//

std::wstring HumdrumInput::cleanHarmString3(const std::string &content)
{
    std::string temp;

    // hide **rhrm token if not a harmony "attack":

    if (content.find("_") != string::npos) {
        return L"";
    }
    if (content.find("]") != string::npos) {
        return L"";
    }

    // skip over **recip data:
    int i;
    for (i = 0; i < (int)content.size(); ++i) {
        if ((content[i] == '-') || (content[i] == '#')) {
            break;
        }
        if (isalpha(content[i])) {
            // V, I, ii, vi, Lt, Gn, Fr, N
            break;
        }
    }

    int foundstartsquare = false;
    for (int ii = i; ii < (int)content.size(); ++ii) {
        if (content[ii] == '[') {
            foundstartsquare = true;
        }
        if (content[ii] == ']') {
            if (!foundstartsquare) {
                continue;
            }
        }
        if (content[ii] == '_') {
            continue;
        }
        temp += content[i];
    }

    return cleanHarmString2(temp);
}

//////////////////////////////
//
// HumdrumInput::cleanHarmString2 -- Adjust **harm text
//

std::wstring HumdrumInput::cleanHarmString2(const std::string &content)
{
    std::wstring output;
    bool nonrhythm = false;
    for (int i = 0; i < (int)content.size(); ++i) {
        if (!nonrhythm) {
            if (isdigit(content[i])) {
                continue;
            }
            if (content[i] == '%') {
                continue;
            }
            if (isdigit(content[i] == '.')) {
                continue;
            }
        }
        nonrhythm = true;
        if (content[i] == '-') {
            output += L"\u266D"; // unicode flat
        }
        else if (content[i] == '#') {
            output += L"\u266F"; // unicode sharp
        }
        else if (content[i] == 'D') {
            output += L"\u00F8"; // o-slash
        }
        else if (content[i] == 'o') {
            output += L"\u00B0"; // degree sign
        }
        else {
            string tdee;
            tdee = content[i];
            output += UTF8to16(tdee);
        }
    }

    return output;
}

//////////////////////////////
//
// HumdrumInput::cleanStringString -- Add circles around string numbers.
//    see: https://www.fileformat.info/info/unicode/block/enclosed_alphanumerics/utf8test.htm
//

std::wstring HumdrumInput::cleanStringString(const std::string &content)
{
    std::wstring output;
    std::string value;
    for (int i = 0; i < (int)content.size(); i++) {
        switch (content[i]) {
            case '0': output += L"\u24ea"; break; // 0 in circle
            case '1': output += L"\u2460"; break; // 1 in circle
            case '2': output += L"\u2461"; break; // 2 in circle
            case '3': output += L"\u2462"; break; // 3 in circle
            case '4': output += L"\u2463"; break; // 4 in circle
            case '5': output += L"\u2464"; break; // 5 in circle
            case '6': output += L"\u2465"; break; // 6 in circle
            case '7': output += L"\u2466"; break; // 7 in circle
            case '8': output += L"\u2467"; break; // 8 in circle
            case '9': output += L"\u2468"; break; // 9 in circle
            default:
                value.clear();
                value.push_back(content[i]);
                output += UTF8to16(value);
        }
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::cleanHarmString --
//

std::wstring HumdrumInput::cleanHarmString(const std::string &content)
{
    std::wstring root;
    std::wstring kind;
    std::wstring bass;

    bool foundspace = false;
    bool foundslash = false;
    for (int i = 0; i < (int)content.size(); ++i) {
        if (content[i] == '/') {
            foundslash = true;
        }
        if (foundspace && !foundslash) {
            string tdee;
            tdee = content[i];
            kind += UTF8to16(tdee);
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
                root += L"\u266D"; // unicode flat
            }
            else if (content[i] == '#') {
                root += L"\u266F"; // unicode sharp
            }
            else {
                string tdee;
                tdee = content[i];
                root += UTF8to16(tdee);
            }
        }
        else if (foundslash) {
            if (content[i] == '-') {
                bass += L"\u266D"; // unicode flat
            }
            else if (content[i] == '#') {
                bass += L"\u266F"; // unicode sharp
            }
            else {
                string tdee;
                tdee = content[i];
                bass += UTF8to16(tdee);
            }
        }
        else {
            cerr << "should not get here with correct input " << content << endl;
        }
    }

    bool replacing = false;
    if (kind == L"major-minor") {
        kind = L"Mm7";
        replacing = true;
    }
    else if (kind == L"minor-major") {
        kind = L"mM7";
        replacing = true;
    }

    if (replace(kind, L"major-", L"maj")) {
        replacing = true;
    }
    else if (replace(kind, L"minor-", L"m")) {
        replacing = true;
    }
    else if (replace(kind, L"dominant-", L"dom")) {
        replacing = true;
    }
    else if (replace(kind, L"augmented-", L"+")) {
        replacing = true;
    }
    else if (replace(kind, L"suspended-", L"sus")) {
        replacing = true;
    }
    else if (replace(kind, L"diminished-", L"\u00B0")) { // degree sign
        replacing = true;
    }
    if (replace(kind, L"seventh", L"7")) {
        replacing = true;
    }
    else if (replace(kind, L"ninth", L"9")) {
        replacing = true;
    }
    else if (replace(kind, L"11th", L"11")) {
        replacing = true;
    }
    else if (replace(kind, L"13th", L"13")) {
        replacing = true;
    }
    else if (replace(kind, L"second", L"2")) {
        replacing = true;
    }
    else if (replace(kind, L"fourth", L"4")) {
        replacing = true;
    }
    else if (replace(kind, L"sixth", L"6")) {
        replacing = true;
    }

    if (kind == L"major") {
        kind = L"";
        replacing = true;
    }
    else if (kind == L"maj") {
        kind = L"";
        replacing = true;
    }
    else if (kind == L"ma") {
        kind = L""; // degree sign
        replacing = true;
    }
    else if (kind == L"minor") {
        kind = L"m";
        replacing = true;
    }
    else if (kind == L"min") {
        kind = L"m";
        replacing = true;
    }
    else if (kind == L"augmented") {
        kind = L"+";
        replacing = true;
    }
    else if (kind == L"minor-seventh") {
        kind = L"m7";
        replacing = true;
    }
    else if (kind == L"major-seventh") {
        kind = L"maj7";
        replacing = true;
    }
    else if (kind == L"dom11") {
        kind = L"11";
        replacing = true;
    }
    else if (kind == L"dom13") {
        kind = L"13";
        replacing = true;
    }
    else if (kind == L"dom9") {
        kind = L"9";
        replacing = true;
    }
    else if (kind == L"half-diminished") {
        kind = L"\u00F8"; // o-slash
        replacing = true;
    }
    else if (kind == L"diminished") {
        kind = L"\u00B0"; // degree sign
        replacing = true;
    }
    else if (kind == L"dominant") {
        kind = L"7";
        replacing = true;
    }
    else if (kind == L"power") {
        kind = L"5";
        replacing = true;
    }
    else if (kind == L"m7b5") {
        replacing = true;
        kind = L"m7\u266D"
               L"5";
    }
    if ((kind != L"") && !replacing) {
        root += L' ';
    }
    if (bass != L"") {
        kind += L'/';
    }
    std::wstring output = root + kind + bass;
    return output;
}

//////////////////////////////
//
// HumdrumInput::replace -- simple substring replacement implementation.
//    Returns true if there was any replacment done.
//

bool HumdrumInput::replace(string &str, const std::string &oldStr, const std::string &newStr)
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

bool HumdrumInput::replace(std::wstring &str, const std::wstring &oldStr, const std::wstring &newStr)
{
    std::wstring::size_type pos = 0u;
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
    for (int i = 0; i < layercount; ++i) {
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

    std::vector<int> &rkern = m_rkern;
    int staffindex = rkern[track];
    std::vector<hum::HTp> &layerdata = m_layertokens[staffindex][layerindex];

    if (layerdata.size() > 0) {
        if (layerdata[0]->size() > 0) {
            setLocationIdNSuffix(m_layer, layerdata[0], layerindex + 1);
        }
    }

    if ((layerindex == 0) && (layerdata.size() > 0)) {
        if ((layerdata[0]->size() > 0) && (layerdata[0]->at(0) == '=')) {
            setLocationIdNSuffix(m_staff, layerdata[0], layerindex + 1);
        }
    }

    if (m_comment) {
        std::string comment;
        comment += " kern: ";
        for (int i = 0; i < (int)layerdata.size(); ++i) {
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
// HumdrumInput::fixLargeTuplets -- fix triple-breve/triplet-wholenote cases.
//

void HumdrumInput::fixLargeTuplets(std::vector<humaux::HumdrumBeamAndTuplet> &tg)
{
    // triplet-whole + triplet-breve cases
    for (int i = 1; i < (int)tg.size(); i++) {
        if ((tg[i].tupletstart == 2) && (tg[i].tupletend == 1) && (tg[i - 1].tupletstart == 1)
            && (tg[i - 1].tupletend == 1)) {
            tg[i].tupletstart = 0;
            tg[i - 1].tupletend = 0;
        }
    }

    // two triplet-halfs + triplet-breve case
    for (int i = 2; i < (int)tg.size(); i++) {
        if ((tg[i].tupletstart == 2) && (tg[i].tupletend == 1) && (tg[i - 1].tupletstart == 0)
            && (tg[i - 1].tupletend == 1) && (tg[i - 2].tupletstart == 1) && (tg[i - 2].tupletend == 0)) {
            tg[i - 1].numscale = 1;
            tg[i - 2].numscale = 1;
            tg[i].tupletstart = 0;
            tg[i - 1].tupletend = 0;
            tg[i].numbase = 2;
        }
    }

    // two triplet-halfs + triplet-breve case + two triplet-halfs
    for (int i = 2; i < (int)tg.size(); i++) {
        if ((tg[i].tupletstart == 0) && (tg[i].tupletend == 2) && (tg[i - 1].tupletstart == 2)
            && (tg[i - 1].tupletend == 0) && (tg[i - 2].tupletstart == 1) && (tg[i - 2].tupletend == 1)) {
            tg[i].tupletend = 1;
            tg[i - 1].tupletstart = 0;
            tg[i - 2].tupletend = 0;
            tg[i - 2].numbase = 2;
            tg[i].numscale = 1;
            tg[i - 1].numscale = 1;
        }
    }
}

//////////////////////////////
//
// HumdrumInput::printGroupInfo --
//

void HumdrumInput::printGroupInfo(std::vector<humaux::HumdrumBeamAndTuplet> &tg, const std::vector<hum::HTp> &layerdata)
{
    if (layerdata.size() != tg.size()) {
        cerr << "LAYER SIZE = " << layerdata.size() << "\tTGSIZE" << tg.size() << endl;
        return;
    }
    cerr << "TOK\t\tGRP\tBRAK\tNUM\tNBASE\tNSCAL\tBSTART\tBEND\tGBST\tGBEND\tTSTART"
            "\tTEND\tPRIORITY\n";
    for (int i = 0; i < (int)tg.size(); ++i) {
        cerr << *layerdata[i] << "\t";
        if (layerdata[i]->size() < 8) {
            cerr << "\t";
        }
        cerr << tg[i].group << "\t";
        cerr << tg[i].bracket << "\t";
        cerr << tg[i].num << "\t";
        cerr << tg[i].numbase << "\t";
        cerr << tg[i].numscale << "\t";
        cerr << tg[i].beamstart << "\t";
        cerr << tg[i].beamend << "\t";
        cerr << tg[i].gbeamstart << "\t";
        cerr << tg[i].gbeamend << "\t";
        cerr << "TS:" << tg[i].tupletstart << "\t";
        cerr << "TE:" << tg[i].tupletend << "\t";
        cerr << tg[i].priority;
        cerr << endl;
    }
    cerr << "============================================" << endl;
}

//////////////////////////////
//
// HumdrumInput::setBeamDirection -- Set a beam up or down.
//

void HumdrumInput::setBeamDirection(int direction, const std::vector<humaux::HumdrumBeamAndTuplet> &tgs,
    std::vector<hum::HTp> &layerdata, int layerindex, bool grace)
{
    const humaux::HumdrumBeamAndTuplet &tg = tgs[layerindex];
    int beamstart = tg.beamstart;
    if (grace) {
        beamstart = tg.gbeamstart;
    }
    int beamend;

    for (int i = layerindex; i < (int)layerdata.size(); ++i) {
        beamend = tgs[i].beamend;
        if (grace) {
            beamend = tgs[i].gbeamend;
        }
        if (!layerdata[i]->isData()) {
            continue;
        }
        if (layerdata[i]->isNull()) {
            continue;
        }
        if (layerdata[i]->isRest()) {
            // not adding stem direction to rests
            continue;
        }
        if ((layerdata[i]->getDuration() == 0) && !grace) {
            // ignore grace note beams
            continue;
        }
        else if ((layerdata[i]->getDuration() != 0) && grace) {
            // ignore non-grace note beams
            continue;
        }
        layerdata[i]->setValue("auto", "stem.dir", to_string(direction));
        if (beamend == beamstart) {
            // last note of beam so exit
            break;
        }
    }
}

//////////////////////////////
//
// HumdrumInput::checkForTremolo --  Check to see if a beamed group of notes
//    can be converted into a tremolo. (Decision to convert to tremolo is done
//    outside of this function and is activated by the *tremolo tandem interpretation).
//

bool HumdrumInput::checkForTremolo(
    std::vector<hum::HTp> &layerdata, const std::vector<humaux::HumdrumBeamAndTuplet> &tgs, int startindex)
{
    int beamnumber = tgs[startindex].beamstart;
    std::vector<hum::HTp> notes;
    for (int i = startindex; i < (int)layerdata.size(); i++) {
        if (layerdata[i]->isNote()) {
            notes.push_back(layerdata[i]);
        }
        if (tgs[i].beamend == beamnumber) {
            break;
        }
    }
    if (notes.empty()) {
        return false;
    }

    hum::HumNum duration = notes[0]->getDuration();
    hum::HumNum testdur = duration;
    std::vector<std::vector<int> > pitches(notes.size());

    for (int i = 0; i < (int)notes.size(); i++) {
        if (i > 0) {
            testdur = notes[i]->getDuration();
            if (testdur != duration) {
                // All durations must be the same for a tremolo.
                return false;
            }
        }
        int scount = notes[i]->getSubtokenCount();
        for (int j = 0; j < scount; j++) {
            std::string subtok = notes[i]->getSubtoken(j);
            pitches[i].emplace_back(hum::Convert::kernToBase40(subtok));
        }
    }
    // Should also disallow any case where there is a tie present.

    // Check for <bTrem> case.
    bool allpequal = true;
    for (int i = 1; i < (int)pitches.size(); i++) {

        if (pitches[i].size() != pitches[i - 1].size()) {
            allpequal = false;
            break;
        }
        // Check if each note in the successive chords is the same.
        // The ordering of notes in each chord is assumed to be the same
        // (i.e., this function is not going to waste time sorting
        // the pitches to check if the chords are equivalent).
        for (int j = 0; j < (int)pitches[i].size(); j++) {
            if (pitches[i][j] != pitches[i - 1][j]) {
                allpequal = false;
                break;
            }
        }
        if (allpequal == false) {
            break;
        }
    }

    if (allpequal) {
        // beam group should be converted into a <bTrem> tremolo
        hum::HumNum tdur = duration * (int)notes.size();
        std::string recip = hum::Convert::durationToRecip(tdur);

        int slashes = log(duration.getFloat()) / log(2.0);
        int noteslash = log(tdur.getFloat()) / log(2.0);
        if (noteslash < 0) {
            slashes = slashes - noteslash;
        }
        slashes = -slashes;
        if (slashes <= 0) {
            // something went wrong calculating durations.
            return false;
        }

        notes[0]->setValue("auto", "tremolo", "1");
        notes[0]->setValue("auto", "recip", recip);
        notes[0]->setValue("auto", "slashes", slashes);
        for (int i = 1; i < (int)notes.size(); i++) {
            notes[i]->setValue("auto", "suppress", "1");
        }

        return true;
    }

    // Check for <fTrem> case.
    // allowing odd-length sequences (3, 5, 7, etc) which can in theory
    // be represented by fTrem, but I have not see such cases.

    if (pitches.size() < 3) {
        // fTrem cannot exist on only two notes/chords.
        return false;
    }

    // check to see that all even notes/chords are the same
    for (int i = 2; i < (int)pitches.size(); i++) {
        if (pitches[i].size() != pitches[i - 2].size()) {
            return false;
        }
        // Check if each note in the successive chords is the same.
        // The ordering of notes in each chord is assumed to be the same
        // (i.e., this function is not going to waste time sorting
        // the pitches to check if the chords are equivalent).
        for (int j = 0; j < (int)pitches[i].size(); j++) {
            if (pitches[i][j] != pitches[i - 2][j]) {
                return false;
            }
        }
    }

    // If got to this point, create an fTrem.

    hum::HumNum tdur = duration * (int)notes.size();
    std::string recip = hum::Convert::durationToRecip(tdur);
    std::string unitrecip = hum::Convert::durationToRecip(duration);

    // Eventually also allow calculating of beam.float
    // (mostly for styling half note tremolos).
    int beams = -log(duration.getFloat()) / log(2.0);
    if (beams <= 0) {
        // something went wrong calculating durations.
        cerr << "PROBLEM WITH TREMOLO2 CALCULATION: " << beams << endl;
        return false;
    }

    notes[0]->setValue("auto", "tremolo2", "1");
    notes[0]->setValue("auto", "recip", recip);
    notes[0]->setValue("auto", "unit", unitrecip); // problem if dotted...
    notes[0]->setValue("auto", "beams", beams);
    notes[1]->setValue("auto", "tremoloAux", "1");
    notes[1]->setValue("auto", "recip", recip);

    for (int i = 1; i < (int)notes.size(); i++) {
        notes[i]->setValue("auto", "suppress", "1");
    }

    return true;
}

//////////////////////////////
//
// HumdrumInput::handleGroupStarts --
//

void HumdrumInput::handleGroupStarts(const std::vector<humaux::HumdrumBeamAndTuplet> &tgs,
    std::vector<string> &elements, std::vector<void *> &pointers, std::vector<hum::HTp> &layerdata, int layerindex)
{
    Beam *beam;
    const humaux::HumdrumBeamAndTuplet &tg = tgs[layerindex];
    hum::HTp token = layerdata[layerindex];
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    int staffindex = m_currentstaff - 1;

    if (ss[staffindex].tremolo) {
        if (token->find("L") != std::string::npos) {
            bool status = checkForTremolo(layerdata, tgs, layerindex);
            if (status) {
                // beamed group converted into tremolo
                return;
            }
        }
    }

    if (tg.beamstart || tg.gbeamstart) {
        int direction = 0;
        if (m_signifiers.above) {
            std::string pattern = "[LJKk]+";
            pattern.push_back(m_signifiers.above);
            if (regex_search(*token, regex(pattern))) {
                direction = 1;
            }
        }
        if (m_signifiers.below) {
            std::string pattern = "[LJKk]+";
            pattern.push_back(m_signifiers.below);
            if (regex_search(*token, regex(pattern))) {
                direction = -1;
            }
        }
        if (direction) {
            if (tg.beamstart) {
                setBeamDirection(direction, tgs, layerdata, layerindex, false);
            }
            else {
                setBeamDirection(direction, tgs, layerdata, layerindex, true);
            }
        }
    }

    if (tg.beamstart && tg.tupletstart) {
        if (tg.priority == 'T') {
            insertTuplet(elements, pointers, tgs, layerdata, layerindex, ss[staffindex].suppress_beam_tuplet);
            beam = insertBeam(elements, pointers, tg);
            checkBeamWith(beam, tgs, layerdata, layerindex);
            setBeamLocationId(beam, tgs, layerdata, layerindex);
        }
        else {
            beam = insertBeam(elements, pointers, tg);
            setBeamLocationId(beam, tgs, layerdata, layerindex);
            checkBeamWith(beam, tgs, layerdata, layerindex);
            insertTuplet(elements, pointers, tgs, layerdata, layerindex, ss[staffindex].suppress_beam_tuplet);
        }
    }
    else if (tg.beamstart) {
        beam = insertBeam(elements, pointers, tg);
        checkBeamWith(beam, tgs, layerdata, layerindex);
        setBeamLocationId(beam, tgs, layerdata, layerindex);
    }
    else if (tg.tupletstart) {
        insertTuplet(elements, pointers, tgs, layerdata, layerindex, ss[staffindex].suppress_bracket_tuplet);
    }

    if (tg.gbeamstart) {
        // Grace note beams should not interact with
        // regular beams or tuplets.
        beam = insertGBeam(elements, pointers, tg);
        checkBeamWith(beam, tgs, layerdata, layerindex);
        setBeamLocationId(beam, tgs, layerdata, layerindex);
    }
}

//////////////////////////////
//
// HumdrumInput::handleGroupEnds --
//

void HumdrumInput::handleGroupEnds(
    const humaux::HumdrumBeamAndTuplet &tg, std::vector<string> &elements, std::vector<void *> &pointers)
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
    hum::HumdrumFile &infile = m_infiles[0];
    std::vector<hum::HumNum> &timesigdurs = m_timesigdurs;
    std::vector<int> &rkern = m_rkern;
    int staffindex = rkern[track];
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;

    if (staffindex < 0) {
        // not a kern spine.
        return false;
    }
    std::vector<hum::HTp> &layerdata = m_layertokens[staffindex][layerindex];
    Layer *&layer = m_layer;

    if (layerdata.size() == 0) {
        // nothing to do.
        return true;
    }

    prepareInitialOttavas(layerdata[0]);

    hum::HumNum starttime = infile[startline].getDurationFromStart();
    hum::HumNum endtime = infile[endline].getDurationFromStart() + infile[endline].getDuration();
    hum::HumNum duration = endtime - starttime;

    /* Why not allowed?
    if (timesigdurs[startline] != duration) {
            m_measure->SetMetcon(BOOLEAN_false);
    }
    */

    hum::HumNum layerstarttime = infile[startline].getDurationFromStart();
    hum::HumNum layerendtime = infile[endline].getDurationFromStart();

    std::vector<hum::HumNum> prespace;
    getTimingInformation(prespace, layerdata, layerstarttime, layerendtime);

    if (emptyMeasures()) {
        if (timesigdurs[startline] == duration) {
            MRest *mrest = new MRest();
            m_layer->AddChild(mrest);
            // Assign a Humdrum ID here.
            hum::HTp trest = NULL;
            for (int i = 0; i < (int)layerdata.size(); i++) {
                if (layerdata[i]->isRest()) {
                    trest = layerdata[i];
                    break;
                }
            }
            if (trest) {
                setLocationId(mrest, trest);
                if (m_doc->GetOptions()->m_humType.GetValue()) {
                    embedQstampInClass(mrest, trest, *trest);
                }
            }
        }
        else {
            Rest *rest = new Rest();
            m_layer->AddChild(rest);
            setDuration(rest, duration);
        }
        return true;
    }

    std::vector<string> elements;
    std::vector<void *> pointers;
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
            for (int j = 0; j < (int)layerdata.size(); j++) {
                if (!layerdata[j]->isData()) {
                    continue;
                }
                processDirections(layerdata[j], staffindex);
            }
        }
        else {
            MRest *mrest = new MRest();
            appendElement(layer, mrest);
            hum::HTp trest = NULL;
            for (int i = 0; i < (int)layerdata.size(); i++) {
                if (!layerdata[i]->isData()) {
                    continue;
                }
                if (layerdata[i]->isRest()) {
                    trest = layerdata[i];
                    break;
                }
            }
            if (trest) {
                convertMRest(mrest, trest, -1, staffindex);
            }

            for (int z = 0; z < (int)layerdata.size(); ++z) {
                // Don't look at first item in full-measure rest if it is a barline.
                // This is to prevent text directions for previous measure being duplicated
                // at the end of the mesure.  Disallowing initial measure token should
                // be ok.
                if ((z == 0) && layerdata[z]->isBarline()) {
                    continue;
                }

                processDirections(layerdata[z], staffindex);

                if (layerdata[z]->isInterpretation()) {
                    handlePedalMark(layerdata[z]);
                }
            }
        }

        // Basic compensation for clef change (can be improved later):
        for (int i = 0; i < (int)layerdata.size(); ++i) {
            if (!layerdata[i]->isClef()) {
                continue;
            }
            if (layerdata[i]->getDurationFromBarline() > 0) {
                // only insert a clef change after the whole-note rest
                // if the clef change is not really an initial clef.
                insertClefElement(elements, pointers, layerdata[i]);
            }
        }

        // check for fermatas to be placed on next barline
        if ((layerindex == 0) && (!layerdata.empty()) && (layerdata.back()->at(0) == '=')) {
            if (layerdata.back()->find(";") != std::string::npos) {
                addFermata(layerdata.back(), NULL);
            }
            if (layerdata.back()->find(",") != std::string::npos) {
                addBreath(layerdata.back(), NULL);
            }
        }

        // probably better to mark the rest in Humdrum data
        // as a full-measure rest here, and then process the
        // measure as normal below.
        return true;
    }

    std::vector<humaux::HumdrumBeamAndTuplet> tgs;
    prepareBeamAndTupletGroups(layerdata, tgs);
    fixLargeTuplets(tgs);

    if (m_debug) {
        printGroupInfo(tgs, layerdata);
    }

    m_tupletscaling = 1;

    Note *note = NULL;

    // Used for splitting an invisible rest across a clef:
    hum::HTp spaceSplitToken = NULL;
    hum::HumNum remainingSplitDur;

    hum::HumRegex hre;
    // ggg processGlobalDirections(token, staffindex);

    for (int i = 0; i < (int)layerdata.size(); ++i) {
        if (prespace[i] > 0) {
            addSpace(elements, pointers, prespace[i]);
        }
        if (layerdata[i]->isData() && layerdata[i]->isNull()) {
            // print any global text directions attacked to the null token
            // and then skip to next token.
            processDirections(layerdata[i], staffindex);
            continue;
        }
        if (layerdata[i]->isInterpretation()) {
            handleOttavaMark(layerdata[i], note);
            handleLigature(layerdata[i]);
            handleColoration(layerdata[i]);
            handlePedalMark(layerdata[i]);
            handleStaffStateVariables(layerdata[i]);
            handleStaffDynamStateVariables(layerdata[i]);
            if (hre.search(layerdata[i], "^\\*color:(.*)")) {
                int track = layerdata[i]->getTrack();
                m_spine_color[track] = hre.getMatch(1);
            }
            if (layerdata[i]->isMens()) {
                if (layerdata[i]->isClef()) {
                    if (ss.at(m_currentstaff - 1).last_clef != *layerdata[i]) {
                        Clef *clef = insertClefElement(elements, pointers, layerdata[i]);
                        setLocationId(clef, layerdata[i]);
                    }
                }
            }
            else if (layerdata[i]->getDurationFromStart() != 0) {
                if (layerdata[i]->isClef()) {
                    int subtrack = layerdata[i]->getSubtrack();
                    if (subtrack) {
                        subtrack--;
                    }
                    if (subtrack) {
                        // ignore clef changes in subtracks and only
                        // use the one in the primary track
                        continue;
                    }
                    Clef *clef = insertClefElement(elements, pointers, layerdata[i]);
                    setLocationId(clef, layerdata[i]);
                    int diff = layerindex - subtrack;
                    if (diff > 0) {
                        std::string letter;
                        letter.push_back('a' + diff);
                        std::string uid = clef->GetUuid();
                        uid += letter;
                        clef->SetUuid(uid);
                    }
                    if (spaceSplitToken != NULL) {
                        // Add the second part of a split invisible rest:
                        Space *irest = new Space;
                        if (m_doc->GetOptions()->m_humType.GetValue()) {
                            embedQstampInClass(irest, spaceSplitToken, *spaceSplitToken);
                        }
                        setLocationId(irest, spaceSplitToken);
                        std::string uid = irest->GetUuid();
                        uid += "b";
                        irest->SetUuid(uid);
                        appendElement(elements, pointers, irest);
                        // convertRhythm(irest, spaceSplitToken);
                        setRhythmFromDuration(irest, remainingSplitDur);
                        // processSlurs(spaceSplitToken);
                        // processDynamics(spaceSplitToken, staffindex);
                        // processDirections(spaceSplitToken, staffindex);
                        // Store rest here to complete the split after the clef change.
                        spaceSplitToken = NULL;
                        remainingSplitDur = 0;
                    }
                }
                else if (layerdata[i]->isNull()) {
                    if ((i > 0) && (layerdata[i]->getLineIndex() == layerdata[i - 1]->getLineIndex())) {
                        // do nothing: duplicate layer clefs are handled elsewhere
                    }
                    else {
                        // duplicate clef changes in secondary layers
                        int xtrack = layerdata[i]->getTrack();
                        hum::HTp tok = layerdata[i]->getPreviousFieldToken();
                        while (tok) {
                            int ttrack = tok->getTrack();
                            if (ttrack == xtrack) {
                                if (tok->isClef()) {
                                    Clef *clef = insertClefElement(elements, pointers, tok);
                                    setLocationId(clef, layerdata[i]);
                                    // Uncomment when clef->SetSameas() is available:
                                    // std::string sameas = "#clef-L";
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

        if (layerdata[i]->isMens()) {
            convertMensuralToken(elements, pointers, layerdata[i], staffindex);
            continue;
        }

        handleGroupStarts(tgs, elements, pointers, layerdata, i);

        if (layerdata[i]->getValueInt("auto", "suppress")) {
            continue;
        }

        // conversion of **kern data to MEI:
        if (layerdata[i]->isChord()) {
            int chordnotecount = getChordNoteCount(layerdata[i]);
            if (chordnotecount < 1) {
                // invalid chord, so put a space in its place.
                if (m_signifiers.irest_color.empty() && m_signifiers.space_color.empty()) {
                    Space *irest = new Space;
                    if (m_doc->GetOptions()->m_humType.GetValue()) {
                        embedQstampInClass(irest, layerdata[i], *layerdata[i]);
                    }
                    setLocationId(irest, layerdata[i]);
                    appendElement(elements, pointers, irest);
                    convertRhythm(irest, layerdata[i]);
                }
                else {
                    // force invisible rest to be displayed
                    Rest *rest = new Rest;
                    setLocationId(rest, layerdata[i]);
                    appendElement(elements, pointers, rest);
                    convertRest(rest, layerdata[i]);
                    int line = layerdata[i]->getLineIndex();
                    int field = layerdata[i]->getFieldIndex();
                    colorRest(rest, *layerdata[i], line, field);
                    verticalRest(rest, *layerdata[i]);
                }
            }
            else {
                Chord *chord = new Chord;
                setLocationId(chord, layerdata[i]);

                if (m_hasTremolo && layerdata[i]->getValueBool("auto", "tremolo")) {
                    BTrem *btrem = new BTrem;
                    setBeamLocationId(btrem, tgs, layerdata, i);
                    int slashes = layerdata[i]->getValueInt("auto", "slashes");
                    switch (slashes) {
                        case 1: btrem->SetUnitdur(DURATION_8); break;
                        case 2: btrem->SetUnitdur(DURATION_16); break;
                        case 3: btrem->SetUnitdur(DURATION_32); break;
                        case 4: btrem->SetUnitdur(DURATION_64); break;
                        case 5: btrem->SetUnitdur(DURATION_128); break;
                    }
                    appendElement(btrem, chord);
                    appendElement(elements, pointers, btrem);
                }
                else if (m_hasTremolo && layerdata[i]->getValueBool("auto", "tremolo2")) {
                    FTrem *ftrem = new FTrem;
                    setBeamLocationId(ftrem, tgs, layerdata, i);
                    int beams = layerdata[i]->getValueInt("auto", "beams");
                    ftrem->SetBeams(beams);
                    int unit = layerdata[i]->getValueInt("auto", "unit");
                    switch (unit) {
                        case 8: ftrem->SetUnitdur(DURATION_8); break;
                        case 16: ftrem->SetUnitdur(DURATION_16); break;
                        case 32: ftrem->SetUnitdur(DURATION_32); break;
                        case 64: ftrem->SetUnitdur(DURATION_64); break;
                        case 128: ftrem->SetUnitdur(DURATION_128); break;
                    }
                    hum::HTp second = NULL;
                    for (int z = i + 1; z < (int)layerdata.size(); z++) {
                        if (layerdata[z]->getValueInt("auto", "tremoloAux")) {
                            second = layerdata[z];
                            break;
                        }
                    }
                    appendElement(ftrem, chord);
                    if (second) {
                        // ignoring slurs, ties, ornaments, articulations
                        if (second->isChord()) {
                            Chord *chord2 = new Chord;
                            setLocationId(chord2, second);
                            appendElement(ftrem, chord2);
                            convertChord(chord2, second, staffindex);
                        }
                        else {
                            Note *note2 = new Note;
                            setLocationId(note2, second);
                            appendElement(ftrem, note2);
                            convertNote(note2, second, staffindex, 0);
                        }
                    }
                    appendElement(elements, pointers, ftrem);
                }
                else {
                    appendElement(elements, pointers, chord);
                }

                elements.push_back("chord");
                pointers.push_back((void *)chord);
                processChordSignifiers(chord, layerdata[i], staffindex);

                convertChord(chord, layerdata[i], staffindex);
                popElementStack(elements, pointers);
                // maybe an extra pop here for tremolos?
                processSlurs(layerdata[i]);
                processDynamics(layerdata[i], staffindex);
                assignAutomaticStem(chord, layerdata[i], staffindex);
                addArticulations(chord, layerdata[i]);
                addOrnaments(chord, layerdata[i]);
                addArpeggio(chord, layerdata[i]);
                processDirections(layerdata[i], staffindex);
            }
        }
        else if (layerdata[i]->isRest()) {
            if ((layerdata[i]->find("yy") != string::npos) && m_signifiers.irest_color.empty()
                && m_signifiers.space_color.empty()) {
                // Invisible rest (or note which should be invisible.
                if ((i < (int)layerdata.size() - 1) && layerdata[i + 1]->isClef()) {
                    hum::HumNum dur = layerdata[i]->getDuration();
                    hum::HumNum ndur = layerdata[i + 1]->getDurationFromStart() - layerdata[i]->getDurationFromStart();

                    if (ndur < dur) {
                        // create a split invisible rest so that an intervening clef
                        // can be positioned properly.
                        // split the space into two pieces, this is the firsthalf.
                        hum::HumNum splitdur = dur - ndur;
                        Space *irest = new Space;
                        if (m_doc->GetOptions()->m_humType.GetValue()) {
                            embedQstampInClass(irest, layerdata[i], *layerdata[i]);
                        }
                        setLocationId(irest, layerdata[i]);
                        appendElement(elements, pointers, irest);
                        // convertRhythm(irest, layerdata[i]);
                        setRhythmFromDuration(irest, splitdur);
                        processSlurs(layerdata[i]);
                        processDynamics(layerdata[i], staffindex);
                        processDirections(layerdata[i], staffindex);
                        // Store rest here to complete the split after the clef change.
                        spaceSplitToken = layerdata[i];
                        remainingSplitDur = ndur;
                    }
                    else {
                        // normal space
                        Space *irest = new Space;
                        if (m_doc->GetOptions()->m_humType.GetValue()) {
                            embedQstampInClass(irest, layerdata[i], *layerdata[i]);
                        }
                        setLocationId(irest, layerdata[i]);
                        appendElement(elements, pointers, irest);
                        convertRhythm(irest, layerdata[i]);
                        processSlurs(layerdata[i]);
                        processDynamics(layerdata[i], staffindex);
                        processDirections(layerdata[i], staffindex);
                    }
                }
                else {
                    Space *irest = new Space;
                    if (m_doc->GetOptions()->m_humType.GetValue()) {
                        embedQstampInClass(irest, layerdata[i], *layerdata[i]);
                    }
                    setLocationId(irest, layerdata[i]);
                    appendElement(elements, pointers, irest);
                    convertRhythm(irest, layerdata[i]);
                    processSlurs(layerdata[i]);
                    processDynamics(layerdata[i], staffindex);
                    processDirections(layerdata[i], staffindex);
                }
            }
            else {
                Rest *rest = new Rest;
                setLocationId(rest, layerdata[i]);
                appendElement(elements, pointers, rest);
                convertRest(rest, layerdata[i]);
                processSlurs(layerdata[i]);
                processDynamics(layerdata[i], staffindex);
                processDirections(layerdata[i], staffindex);
                int line = layerdata[i]->getLineIndex();
                int field = layerdata[i]->getFieldIndex();
                colorRest(rest, *layerdata[i], line, field);
                verticalRest(rest, *layerdata[i]);
            }
        }
        else if (!layerdata[i]->isNote()) {

            // this is probably a **recip value without note or rest information
            // so print it as a space (invisible rest).
            if ((!m_signifiers.rspace_color.empty()) || (!m_signifiers.space_color.empty())) {
                // This should be invisible but someone wants it visible
                // and colored.
                Rest *rest = new Rest;
                setLocationId(rest, layerdata[i]);
                appendElement(elements, pointers, rest);
                convertRest(rest, layerdata[i]);
                processSlurs(layerdata[i]);
                processDynamics(layerdata[i], staffindex);
                processDirections(layerdata[i], staffindex);
                int line = layerdata[i]->getLineIndex();
                int field = layerdata[i]->getFieldIndex();
                colorRest(rest, *layerdata[i], line, field);
            }
            else {
                Space *irest = new Space;
                if (m_doc->GetOptions()->m_humType.GetValue()) {
                    embedQstampInClass(irest, layerdata[i], *layerdata[i]);
                }
                setLocationId(irest, layerdata[i]);
                appendElement(elements, pointers, irest);
                convertRhythm(irest, layerdata[i]);
                processSlurs(layerdata[i]);
                processDynamics(layerdata[i], staffindex);
                processDirections(layerdata[i], staffindex);
            }
        }
        else {
            // should be a note
            note = new Note;
            setStemLength(note, layerdata[i]);
            setLocationId(note, layerdata[i]);

            if (m_hasTremolo && layerdata[i]->getValueBool("auto", "tremolo")) {
                BTrem *btrem = new BTrem;
                setBeamLocationId(btrem, tgs, layerdata, i);
                int slashes = layerdata[i]->getValueInt("auto", "slashes");
                switch (slashes) {
                    case 1: btrem->SetUnitdur(DURATION_8); break;
                    case 2: btrem->SetUnitdur(DURATION_16); break;
                    case 3: btrem->SetUnitdur(DURATION_32); break;
                    case 4: btrem->SetUnitdur(DURATION_64); break;
                    case 5: btrem->SetUnitdur(DURATION_128); break;
                }
                appendElement(btrem, note);
                appendElement(elements, pointers, btrem);
            }
            else if (m_hasTremolo && layerdata[i]->getValueBool("auto", "tremolo2")) {
                FTrem *ftrem = new FTrem;
                setBeamLocationId(ftrem, tgs, layerdata, i);
                int beams = layerdata[i]->getValueInt("auto", "beams");
                ftrem->SetBeams(beams);
                int unit = layerdata[i]->getValueInt("auto", "unit");
                switch (unit) {
                    case 8: ftrem->SetUnitdur(DURATION_8); break;
                    case 16: ftrem->SetUnitdur(DURATION_16); break;
                    case 32: ftrem->SetUnitdur(DURATION_32); break;
                    case 64: ftrem->SetUnitdur(DURATION_64); break;
                    case 128: ftrem->SetUnitdur(DURATION_128); break;
                }
                hum::HTp second = NULL;
                for (int z = i + 1; z < (int)layerdata.size(); z++) {
                    if (layerdata[z]->getValueInt("auto", "tremoloAux")) {
                        second = layerdata[z];
                        break;
                    }
                }
                appendElement(ftrem, note);
                if (second) {
                    // ignoring slurs, ties, ornaments, articulations
                    if (second->isChord()) {
                        Chord *chord2 = new Chord;
                        setLocationId(chord2, second);
                        appendElement(ftrem, chord2);
                        convertChord(chord2, second, staffindex);
                    }
                    else {
                        Note *note2 = new Note;
                        setLocationId(note2, second);
                        appendElement(ftrem, note2);
                        convertNote(note2, second, staffindex, 0);
                    }
                }
                appendElement(elements, pointers, ftrem);
            }
            else {
                appendElement(elements, pointers, note);
            }

            convertNote(note, layerdata[i], 0, staffindex);
            processSlurs(layerdata[i]);
            processDynamics(layerdata[i], staffindex);
            assignAutomaticStem(note, layerdata[i], staffindex);
            if (m_signifiers.nostem && layerdata[i]->find(m_signifiers.nostem) != string::npos) {
                note->SetStemLen(0);
            }
            if (m_signifiers.cuesize && layerdata[i]->find(m_signifiers.cuesize) != string::npos) {
                note->SetCue(BOOLEAN_true);
            }
            else if (m_staffstates.at(staffindex).cue_size.at(m_currentlayer)) {
                note->SetCue(BOOLEAN_true);
            }
            addArticulations(note, layerdata[i]);
            addOrnaments(note, layerdata[i]);
            addArpeggio(note, layerdata[i]);
            processDirections(layerdata[i], staffindex);
        }

        handleGroupEnds(tgs[i], elements, pointers);
    }

    if (prespace.size() > layerdata.size()) {
        if (prespace.back() > 0) {
            // if there is empty space at the end of the layer.  The layer is
            // rhythmically too short, so add a space element to match the
            // amount of underfilling.
            addSpace(elements, pointers, prespace.back());
        }
    }

    // check for fermatas to be placed on next barline
    if ((layerindex == 0) && (!layerdata.empty()) && (layerdata.back()->at(0) == '=')) {
        if (layerdata.back()->find(";") != std::string::npos) {
            addFermata(layerdata.back(), NULL);
        }
        if (layerdata.back()->find(",") != std::string::npos) {
            addBreath(layerdata.back(), NULL);
        }

        // check for rptend here, since the one for the last measure in
        // the music is missed by the inline processing.  But maybe limit
        // this one to only checking for the last measure.  Or move barline
        // styling here...
        if ((layerdata.back()->find(":|") != std::string::npos)
            || (layerdata.back()->find(":!") != std::string::npos)) {
            m_measure->SetRight(BARRENDITION_rptend);
        }
    }
    if ((layerindex == 0) && (!layerdata.empty()) && (layerdata[0]->at(0) == '=')) {
        if ((layerdata[0]->find("|:") != std::string::npos) || (layerdata[0]->find("!:") != std::string::npos)) {
            m_measure->SetLeft(BARRENDITION_rptstart);
        }
    }

    if ((layerdata.size() > 0) && layerdata.back()->isBarline()) {
        processDirections(layerdata.back(), staffindex);
    }

    return true;
}

//////////////////////////////
//
// HumdrumInput::handleLigature --
//

void HumdrumInput::handleLigature(hum::HTp token)
{
    if (token->compare("*lig") != 0) {
        return;
    }

    // find end interpretation of ligature
    hum::HTp ligend = token->getNextToken();
    hum::HTp firstnote = NULL;
    hum::HTp lastnote = NULL;
    while (ligend) {
        if (ligend->compare("*Xlig") == 0) {
            break;
        }
        if (ligend->isNote()) {
            if (!firstnote) {
                firstnote = ligend;
            }
            lastnote = ligend;
        }
        ligend = ligend->getNextToken();
    }
    if (!ligend) {
        // could not find a matching ending for the
        // ligature start.  For now ignore it.  Perhaps
        // later a partial ligature bracket could be
        // rendered (but more likely a data error).
        return;
    }
    if (!firstnote) {
        // strange case: no notes in ligature
        return;
    }
    if (!lastnote) {
        // strange case: no notes in ligature
        return;
    }

    BracketSpan *ligature = new BracketSpan;

    int startline = token->getLineNumber();
    int startfield = token->getFieldNumber();
    int stopline = ligend->getLineNumber();
    int stopfield = ligend->getFieldNumber();
    std::string id = "ligature";
    id += "-L" + to_string(startline);
    id += "F" + to_string(startfield);
    id += "-L" + to_string(stopline);
    id += "F" + to_string(stopfield);
    ligature->SetUuid(id);

    // not considering if notes are in chords (which they should not)
    std::string startid = getLocationId("note", firstnote);
    ligature->SetStartid("#" + startid);
    std::string endid = getLocationId("note", lastnote);
    ligature->SetEndid("#" + endid);

    ligature->SetLform(LINEFORM_solid);
    ligature->SetFunc("ligature");

    if (m_measure) {
        m_measure->AddChild(ligature);
    }
}

//////////////////////////////
//
// HumdrumInput::handleColoration --
//

void HumdrumInput::handleColoration(hum::HTp token)
{
    if (token->compare("*col") != 0) {
        return;
    }

    // find end interpretation of coloration
    hum::HTp colend = token->getNextToken();
    hum::HTp firstnote = NULL;
    hum::HTp lastnote = NULL;
    while (colend) {
        if (colend->compare("*Xcol") == 0) {
            break;
        }
        if (colend->isNote()) {
            if (!firstnote) {
                firstnote = colend;
            }
            lastnote = colend;
        }
        colend = colend->getNextToken();
    }
    if (!colend) {
        // could not find a matching ending for the
        // coloration start.  For now ignore it.  Perhaps
        // later a partial coloration bracket could be
        // rendered (but more likely a data error).
        return;
    }
    if (!firstnote) {
        // strange case: no notes in coloration
        return;
    }
    if (!lastnote) {
        // strange case: no notes in coloration
        return;
    }

    BracketSpan *coloration = new BracketSpan;

    int startline = token->getLineNumber();
    int startfield = token->getFieldNumber();
    int stopline = colend->getLineNumber();
    int stopfield = colend->getFieldNumber();
    std::string id = "coloration";
    id += "-L" + to_string(startline);
    id += "F" + to_string(startfield);
    id += "-L" + to_string(stopline);
    id += "F" + to_string(stopfield);
    coloration->SetUuid(id);

    // not considering if notes are in chords (which they should not)
    std::string startid = getLocationId("note", firstnote);
    coloration->SetStartid("#" + startid);
    std::string endid = getLocationId("note", lastnote);
    coloration->SetEndid("#" + endid);

    // data_LINEWIDTH lw;
    // lw.SetLineWidthTerm(LINEWIDTHTERM_medium);
    // coloration->SetLwidth(lw);
    coloration->SetFunc("coloration");

    if (m_measure) {
        m_measure->AddChild(coloration);
    }
}

//////////////////////////////
//
// HumdrumInput::assignAutomaticStem --
//   X = no automatic stem assignment
//   x = no no stem
//   \ = stem down
//   / = stem up
//

template <class ELEMENT> void HumdrumInput::assignAutomaticStem(ELEMENT element, hum::HTp tok, int staffindex)
{
    char value = m_staffstates.at(staffindex).stem_type.at(m_currentlayer);
    if (value != 'X') {
        char hasstem = tok->hasStemDirection();
        if (!hasstem) {
            switch (value) {
                case '/': element->SetStemDir(STEMDIRECTION_up); break; // force stem up
                case '\\': element->SetStemDir(STEMDIRECTION_down); break; // force stem down
                case 'x': element->SetStemLen(0); break; // force no stem
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::prepareInitialOttavas -- Check for an ottava mark start.
//
//

void HumdrumInput::prepareInitialOttavas(hum::HTp token)
{
    if (!token) {
        return;
    }
    hum::HumNum starttime = token->getDurationFromStart();
    if (starttime > 0) {
        return;
    }
    int subtrack = token->getSubtrack();
    if (subtrack > 1) {
        // only check for ottavas in the first layer.
        return;
    }

    hum::HTp tok = token->getPreviousToken();
    while (tok) {
        if (!tok->isInterpretation()) {
            tok = tok->getPreviousToken();
        }
        if (*tok == "*8va") {
            handleOttavaMark(tok, NULL);
            break;
        }
        else if (*tok == "*8ba") {
            handleOttavaMark(tok, NULL);
            break;
        }
        else if (*tok == "*15ma") {
            handleOttavaMark(tok, NULL);
            break;
        }
        else if (*tok == "*15ba") {
            handleOttavaMark(tok, NULL);
            break;
        }
        tok = tok->getPreviousToken();
    }
}

/////////////////////////////
//
// HumdrumInput::popElementStack --
//

void HumdrumInput::popElementStack(std::vector<string> &elements, std::vector<void *> &pointers)
{
    elements.pop_back();
    pointers.pop_back();
}

//////////////////////////////
//
// HumdrumInput::convertMensuralToken --  Convert a mensural token to MEI.
//     Not dealing with chords yet.
//

void HumdrumInput::convertMensuralToken(
    std::vector<string> &elements, std::vector<void *> &pointers, hum::HTp token, int staffindex)
{
    if (token->isNull()) {
        return;
    }
    if (!token->isMens()) {
        return;
    }

    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;

    bool roff = token->hasRectaLigatureEnd();
    bool ooff = token->hasObliquaLigatureEnd();
    bool ron = token->hasRectaLigatureBegin();
    bool oon = token->hasObliquaLigatureBegin();
    bool turnoffrecta = false;
    bool embeddedobliqua = false;

    if (roff) {
        turnoffrecta = true;
    }
    if (ooff) {
        if ((elements.back() == "ligature") && !ss[staffindex].ligature_obliqua) {
            embeddedobliqua = true;
        }
    }
    if (ooff && ss[staffindex].ligature_recta) {
        embeddedobliqua = true;
    }

    if (oon && (elements.back() == "ligature")) {
        embeddedobliqua = true;
    }

    if (token->hasLigatureBegin()) {
        if (ron) {
            ss[staffindex].ligature_recta = true;
        }
        if (ron && oon) {
            ss[staffindex].ligature_recta = true;
            ss[staffindex].ligature_obliqua = true;
            // if both at same time, then assume obliqua is the start
            // of a compound ligature
            Ligature *ligature = new Ligature;
            string id = getLocationId("ligature", token);
            ligature->SetUuid(id);
            ligature->SetForm(LIGATUREFORM_recta);
            appendElement(elements, pointers, ligature);
            elements.push_back("ligature");
            pointers.push_back((void *)ligature);
            embeddedobliqua = true;
        }
        else if (oon && elements.back() == "ligature") {
            // if already in a ligature, encode obliqua differently
            // this state variable will set note@lig="obliqua" further below
            ss[staffindex].ligature_obliqua = true;
        }
        else if (oon) {
            // create a new obliqua ligature
            ss[staffindex].ligature_obliqua = true;
            Ligature *ligature = new Ligature;
            string id = getLocationId("ligature", token);
            ligature->SetUuid(id);
            ligature->SetForm(LIGATUREFORM_obliqua);
            appendElement(elements, pointers, ligature);
            elements.push_back("ligature");
            pointers.push_back((void *)ligature);
        }
        else {
            // create a new recta ligature (which could be compoound and
            // contain an obliqua, which will be handled above.
            Ligature *ligature = new Ligature;
            string id = getLocationId("ligature", token);
            ligature->SetUuid(id);
            ligature->SetForm(LIGATUREFORM_recta);
            appendElement(elements, pointers, ligature);
            elements.push_back("ligature");
            pointers.push_back((void *)ligature);
            ss[staffindex].ligature_recta = true;
        }
    }

    if (token->isRest()) {
        Rest *rest = new Rest;
        setLocationId(rest, token);
        appendElement(elements, pointers, rest);
        convertRest(rest, token, -1);
    }
    else if (token->isNote()) {
        Note *note = new Note;
        setLocationId(note, token);
        if (embeddedobliqua) {
            note->SetLig(noteAnlMensural_LIG_obliqua);
        }
        appendElement(elements, pointers, note);
        convertNote(note, token, 0, staffindex);
        processSlurs(token);
        processDirections(token, staffindex);
        bool hasstem = false;
        string text = *token;
        for (int i = 0; i < (int)text.size(); i++) {
            switch (text[i]) {
                case 'M': hasstem = true; break;
                case 'm': hasstem = true; break;
                case 'U': hasstem = true; break;
                case 'u': hasstem = true; break;
                case 'L': hasstem = true; break;
                case 'X': hasstem = true; break;
            }
            if (hasstem) {
                break;
            }
        }
        if (hasstem) {
            assignAutomaticStem(note, token, staffindex);
        }

        if (token->find(':') != std::string::npos) {
            Dot *dot = new Dot();
            appendElement(elements, pointers, dot);
        }
    }

    if (roff || ooff) {
        if (elements.back() == "ligature") {
            if (roff) {
                popElementStack(elements, pointers);
            }
            else if (ooff && !turnoffrecta && !ss[staffindex].ligature_recta) {
                popElementStack(elements, pointers);
            }
        }
        else {
            std::cerr << "WARNING: unmatched ligature ending" << std::endl;
        }
    }
    if (roff) {
        ss[staffindex].ligature_recta = false;
    }
    if (ooff) {
        ss[staffindex].ligature_obliqua = false;
    }
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
//    ARTICULATION_marc_stacc,                 * or use "marc stacc"
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
//    ARTICULATION_ten_stacc,                  * or use "ten stacc"
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
    // store artics in random access grid, along with their staff positions:
    vector<int> articloc(256, 0);
    vector<int> articpos(256, 0);
    vector<int> articges(256, 0); // is it a gestural articulation?
    char ch;
    char posch;
    char pos2ch;
    char pos3ch;
    int tsize = (int)((string *)token)->size();
    for (int i = 0; i < tsize; ++i) {
        ch = token->at(i);
        if (isdigit(ch)) {
            continue;
        }
        posch = i < tsize - 1 ? token->at(i + 1) : 0;
        if ((ch == '^') && (posch == '^')) {
            // use 6 slot in array for "^^" (heavy accent)
            ch = 6;
            posch = i < tsize - 2 ? token->at(i + 2) : 'g';
            i++;
        }
        else if ((ch == '\'') && (posch == '\'')) {
            // staccatissimo alternate (eventually remove)
            ch = '`';
            posch = i < tsize - 2 ? token->at(i + 2) : 'g';
            i++;
        }
        articloc.at(ch) = i + 1;

        if (posch) {
            // check for gestural articulations
            pos2ch = i < tsize - 2 ? token->at(i + 2) : 0;
            pos3ch = i < tsize - 3 ? token->at(i + 3) : 0;
            if ((posch == 'y') && (pos2ch != 'y')) {
                articges[ch] = 1;
            }
            else if ((posch == m_signifiers.above) && (pos2ch == 'y') && (pos3ch != 'y')) {
                articges[ch] = 1;
            }
            else if ((posch == m_signifiers.below) && (pos2ch == 'y') && (pos3ch != 'y')) {
                articges[ch] = 1;
            }
        }

        if ((posch != 0) && (posch == m_signifiers.above)) {
            articpos.at(ch) = 1;
        }
        else if ((posch != 0) && (posch == m_signifiers.below)) {
            articpos.at(ch) = -1;
        }
        else {
            articpos.at(ch) = 0;
        }
    }

    // second position is the staff position (-1=below, 0=undefined, 1=above)
    std::vector<data_ARTICULATION> artics;
    std::vector<int> positions;
    std::vector<int> gestural;

    // place articulations in stacking order (nearest to furthest from note):
    if (articloc['\'']) {
        artics.push_back(ARTICULATION_stacc);
        positions.push_back(articpos['\'']);
        gestural.push_back(articges['\'']);
    }
    if (articloc['`']) {
        artics.push_back(ARTICULATION_stacciss);
        positions.push_back(articpos['`']);
        gestural.push_back(articges['`']);
    }
    if (articloc['~']) {
        artics.push_back(ARTICULATION_ten);
        positions.push_back(articpos['~']);
        gestural.push_back(articges['~']);
    }
    if (articloc[6]) {
        artics.push_back(ARTICULATION_marc);
        positions.push_back(articpos[6]);
        gestural.push_back(articges['6']);
    }
    if (articloc['^']) {
        artics.push_back(ARTICULATION_acc);
        positions.push_back(articpos['^']);
        gestural.push_back(articges['^']);
    }
    if (articloc['o']) {
        artics.push_back(ARTICULATION_harm);
        positions.push_back(articpos['o']);
        gestural.push_back(articges['o']);
    }
    if (articloc['v']) {
        artics.push_back(ARTICULATION_upbow);
        positions.push_back(articpos['v']);
        gestural.push_back(articges['v']);
    }
    if (articloc['u']) {
        artics.push_back(ARTICULATION_dnbow);
        positions.push_back(articpos['u']);
        gestural.push_back(articges['u']);
    }

    if (artics.empty()) {
        return;
    }

    Artic *artic = NULL;
    if (artics.size() == 1) {
        // single articulation, so no problem
        if (gestural.at(0)) {
            // When enabled in artic, set gestrual articulation.
            // For now do not store gestural articulation in MEI:
            // artic->SetArticGes(artics);
            return;
        }
        else {
            artic = new Artic;
            appendElement(element, artic);
            artic->SetArtic(artics);
        }
        if (positions.at(0) > 0) {
            setPlace(artic, "above");
        }
        else if (positions.at(0) < 0) {
            setPlace(artic, "below");
        }
        setLocationId(artic, token);
        return;
    }

    if (artic == NULL) {
        artic = new Artic;
        appendElement(element, artic);
    }

    // Handle gestural articulations below when there are multiple articulations.

    // more than one articulation, so categorize them by placement.

    std::vector<data_ARTICULATION> articsabove;
    std::vector<data_ARTICULATION> articsbelow;
    std::vector<data_ARTICULATION> articsdefault; // no placment parameter

    for (int i = 0; i < (int)artics.size(); ++i) {
        if (positions[i] > 0) {
            articsabove.push_back(artics[i]);
        }
        else if (positions[i] < 0) {
            articsbelow.push_back(artics[i]);
        }
        else {
            articsdefault.push_back(artics[i]);
        }
    }

    if (!articsabove.empty()) {
        Artic *artic = new Artic;
        appendElement(element, artic);
        artic->SetArtic(articsabove);
        setPlace(artic, "above");
        artic->SetUuid(getLocationId(element, token, 0) + "-above");
    }

    if (!articsbelow.empty()) {
        Artic *artic = new Artic;
        appendElement(element, artic);
        artic->SetArtic(articsbelow);
        setPlace(artic, "below");
        artic->SetUuid(getLocationId(element, token, 0) + "-below");
    }

    if (!articsdefault.empty()) {
        Artic *artic = new Artic;
        appendElement(element, artic);
        artic->SetArtic(articsdefault);
        setLocationId(artic, token);
    }
}

//////////////////////////////
//
// HumdrumInput::getDirection --
//    0  = no direction specified
//    1  = place above
//    -1 = place below
//

int HumdrumInput::getDirection(const string &token, const std::string &target)
{
    string newtarget;

    if (m_signifiers.above) {
        newtarget = target;
        newtarget.push_back(m_signifiers.above);
        if (token.find(newtarget) != std::string::npos) {
            return +1;
        }
    }

    if (m_signifiers.below) {
        newtarget = target;
        newtarget.push_back(m_signifiers.below);
        if (token.find(newtarget) != std::string::npos) {
            return -1;
        }
    }

    return 0;
}

//////////////////////////////
//
// HumdrumInput::embedPitchInformationInClass --
//

void HumdrumInput::embedPitchInformationInClass(Note *note, const std::string &token)
{
    if (token.find("r") != string::npos) {
        return;
    }
    if (token == ".") {
        return;
    }
    int base40 = hum::Convert::kernToBase40(token);
    int base40chroma = base40 % 40;
    int oct = base40 / 40;
    int acc = hum::Convert::base40ToAccidental(base40);
    int base12chroma = hum::Convert::base40ToMidiNoteNumber(base40) % 12;
    int base7chroma = hum::Convert::base40ToDiatonic(base40) % 7;
    string pname;
    switch (base7chroma) {
        case 0: pname = "c"; break;
        case 1: pname = "d"; break;
        case 2: pname = "e"; break;
        case 3: pname = "f"; break;
        case 4: pname = "g"; break;
        case 5: pname = "a"; break;
        case 6: pname = "b"; break;
    }

    string accid;
    switch (acc) {
        case 0: accid = "n"; break;
        case 1: accid = "s"; break;
        case -1: accid = "f"; break;
        case 2: accid = "ss"; break;
        case -2: accid = "ff"; break;
        case 3: accid = "sss"; break;
        case -3: accid = "fff"; break;
    }

    stringstream ss;
    ss << "pname-" << pname;
    ss << " ";
    ss << "acc-" << accid;
    ss << " ";
    ss << "oct-" << oct;
    ss << " ";
    ss << "b40c-" << base40chroma;
    ss << " ";
    ss << "b12c-" << base12chroma;
    ss << " ";
    appendTypeTag(note, ss.str());
}

//////////////////////////////
//
// HumdrumInput::embedQstampInClass --
//

void HumdrumInput::embedQstampInClass(Note *note, hum::HTp token, const std::string &tstring)
{
    hum::HumNum starttime = token->getDurationFromStart();
    hum::HumNum endtime = starttime + token->getDuration();
    stringstream sson;
    stringstream ssoff;
    sson << "qon-" << starttime.getNumerator();
    if (starttime.getDenominator() != 1) {
        sson << "_" << starttime.getDenominator();
    }
    ssoff << "qoff-" << endtime.getNumerator();
    if (endtime.getDenominator() != 1) {
        ssoff << "_" << endtime.getDenominator();
    }
    appendTypeTag(note, sson.str());
    appendTypeTag(note, ssoff.str());

    /*
            if (tstring.find("[") != std::string::npos) {
            hum::HumNum realendtime = starttime + token->getTiedDuration();
            stringstream ssofftied;
            ssofftied << "qoff-tied-" << realendtime.getNumerator();
            if (realendtime.getDenominator() != 1) {
                    ssoff << "_" << realendtime.getDenominator();
            }
            }
    */
}

//
// Rest version (rqon/rqoff variant)
//

void HumdrumInput::embedQstampInClass(Rest *rest, hum::HTp token, const std::string &tstring)
{
    hum::HumNum starttime = token->getDurationFromStart();
    hum::HumNum endtime = starttime + token->getDuration();
    stringstream sson;
    stringstream ssoff;
    sson << "rqon-" << starttime.getNumerator();
    if (starttime.getDenominator() != 1) {
        sson << "_" << starttime.getDenominator();
    }
    ssoff << "rqoff-" << endtime.getNumerator();
    if (endtime.getDenominator() != 1) {
        ssoff << "_" << endtime.getDenominator();
    }
    appendTypeTag(rest, sson.str());
    appendTypeTag(rest, ssoff.str());
}

//
// MRest version (rqon/rqoff variant)
//

void HumdrumInput::embedQstampInClass(MRest *mrest, hum::HTp token, const std::string &tstring)
{
    hum::HumNum starttime = token->getDurationFromStart();
    hum::HumNum endtime = starttime + token->getDuration();
    stringstream sson;
    stringstream ssoff;
    sson << "rqon-" << starttime.getNumerator();
    if (starttime.getDenominator() != 1) {
        sson << "_" << starttime.getDenominator();
    }
    ssoff << "rqoff-" << endtime.getNumerator();
    if (endtime.getDenominator() != 1) {
        ssoff << "_" << endtime.getDenominator();
    }
    appendTypeTag(mrest, sson.str());
    appendTypeTag(mrest, ssoff.str());
}

//
// Space version (rqon/rqoff variant)
//

void HumdrumInput::embedQstampInClass(Space *irest, hum::HTp token, const std::string &tstring)
{
    hum::HumNum starttime = token->getDurationFromStart();
    hum::HumNum endtime = starttime + token->getDuration();
    stringstream sson;
    stringstream ssoff;
    sson << "rqon-" << starttime.getNumerator();
    if (starttime.getDenominator() != 1) {
        sson << "_" << starttime.getDenominator();
    }
    ssoff << "rqoff-" << endtime.getNumerator();
    if (endtime.getDenominator() != 1) {
        ssoff << "_" << endtime.getDenominator();
    }
    appendTypeTag(irest, sson.str());
    appendTypeTag(irest, ssoff.str());
}

//////////////////////////////
//
// HumdrumInput::embedTieInformation --
//

void HumdrumInput::embedTieInformation(Note *note, const std::string &token)
{
    if (token.find("[") != std::string::npos) {
        appendTypeTag(note, "tie-start");
    }
    else if (token.find("]") != std::string::npos) {
        appendTypeTag(note, "tie-stop");
    }
    else if (token.find("_") != std::string::npos) {
        appendTypeTag(note, "tie-cont");
    }
}

//////////////////////////////
//
// HumdrumInput::colorNote --
//

void HumdrumInput::colorNote(Note *note, hum::HTp token, const std::string &subtoken, int line, int field)
{
    std::string spinecolor = getSpineColor(line, field);
    if (spinecolor != "") {
        note->SetColor(spinecolor);
    }

    int justification = 0;
    for (int i = 0; i < (int)m_signifiers.mark.size(); ++i) {
        if (subtoken.find(m_signifiers.mark[i]) != std::string::npos) {
            note->SetColor(m_signifiers.mcolor[i]);
            appendTypeTag(note, "marked");
            if (!m_signifiers.markdir[i].empty()) {
                bool bold = true;
                bool italic = false;
                int staffindex = m_rkern[token->getTrack()];
                addDirection(m_signifiers.markdir[i], "above", bold, italic, token, staffindex, justification,
                    m_signifiers.mcolor[i]);
            }
            break;
        }
    }
}

//////////////////////////////
//
// HumdrumInput::colorVerse --
//

void HumdrumInput::colorVerse(Verse *verse, std::string &token)
{
    hum::HumRegex hre;
    for (int i = 0; i < (int)m_signifiers.textmark.size(); ++i) {
        if (token.find(m_signifiers.textmark[i]) != std::string::npos) {
            verse->SetColor(m_signifiers.textcolor[i]);
            // appendTypeTag(verse, "marked");
            // remove mark character from text (so that it does not display):
            std::string mystring;
            mystring += m_signifiers.textmark[i];
            hre.replaceDestructive(token, "", mystring, "g");
            return;
        }
    }
    if (m_signifiers.mark.empty()) {
        return;
    }

    // force the verse text black so that it is not
    // highlighted when notes are highlighted.
    verse->SetColor("black");
}

//////////////////////////////
//
// HumdrumInput::appendTypeTag -- add a space before tag if there is
//     already content in @type.
//

template <class ELEMENT> void HumdrumInput::appendTypeTag(ELEMENT *note, const std::string &tag)
{
    if (note->GetType().empty()) {
        note->SetType(tag); // Allow type to be set from data later.
    }
    else {
        std::string newtag = note->GetType();
        newtag += " ";
        newtag += tag;
        note->SetType(newtag);
    }
}

//////////////////////////////
//
// setPlace --
//

template <class ELEMENT> void HumdrumInput::setPlace(ELEMENT *element, const std::string &place)
{
    element->SetPlace(element->AttPlacement::StrToStaffrel(place));
}

/////////////////////////////
//
// HumdrumInput::verticalRest -- If the rest has pitch information, set the vertical
//    position of the rest from the pitch.
//

template <class ELEMENT> void HumdrumInput::verticalRest(ELEMENT element, const std::string &token)
{
    return; // calculating this in humlib now.
    /*
        hum::HumRegex hre;
        if (!hre.search(token, "([A-Ga-g]+)")) {
            return;
        }
        string result = hre.getMatch(1);

        int base40 = hum::Convert::kernToBase40(result);
        int oct = base40 / 40;
        int base7chroma = hum::Convert::base40ToDiatonic(base40) % 7;
        switch (base7chroma) {
            case 0: element->SetPloc(PITCHNAME_c); break;
            case 1: element->SetPloc(PITCHNAME_d); break;
            case 2: element->SetPloc(PITCHNAME_e); break;
            case 3: element->SetPloc(PITCHNAME_f); break;
            case 4: element->SetPloc(PITCHNAME_g); break;
            case 5: element->SetPloc(PITCHNAME_a); break;
            case 6: element->SetPloc(PITCHNAME_b); break;
        }

        element->SetOloc(oct);
    */
}

//////////////////////////////
//
// HumdrumInput::colorRest --
//

void HumdrumInput::colorRest(Rest *rest, const std::string &token, int line, int field)
{
    std::string spinecolor;
    if ((line >= 0) && (field >= 0)) {
        spinecolor = getSpineColor(line, field);
    }
    if (spinecolor != "") {
        rest->SetColor(spinecolor);
    }

    for (int i = 0; i < (int)m_signifiers.mark.size(); ++i) {
        if (token.find(m_signifiers.mark[i]) != string::npos) {
            rest->SetColor(m_signifiers.mcolor[i]);
            break;
        }
    }
    if (token.find("yy") != std::string::npos) {
        // invisible rest that was probably sent here specifically be colored
        if (!m_signifiers.irest_color.empty()) {
            rest->SetColor(m_signifiers.irest_color);
        }
        else if (!m_signifiers.space_color.empty()) {
            rest->SetColor(m_signifiers.space_color);
        }
    }
    else if (token == "") {
        // implicit space that should be colored as a rest
        if (!m_signifiers.ispace_color.empty()) {
            rest->SetColor(m_signifiers.ispace_color);
        }
        else if (!m_signifiers.space_color.empty()) {
            rest->SetColor(m_signifiers.space_color);
        }
    }
    else if (token.find("r") == std::string::npos) {
        // explicit space that should be colored as a rest
        if (!m_signifiers.rspace_color.empty()) {
            rest->SetColor(m_signifiers.rspace_color);
        }
        else if (!m_signifiers.space_color.empty()) {
            rest->SetColor(m_signifiers.space_color);
        }
    }
}

//////////////////////////////
//
// HumdrumInput::getSpineColor --  But suppress black colors which are
//     the default color of notes.
//

string HumdrumInput::getSpineColor(int line, int field)
{
    hum::HumdrumFile &infile = m_infiles[0];
    std::string output;
    int track = infile.token(line, field)->getTrack();
    if (!m_spine_color[track].empty()) {
        if ((m_spine_color[track] != "black") && (m_spine_color[track] != "#000000")
            && (m_spine_color[track] != "#000")) {
            output = m_spine_color[track];
        }
    }
    if (!m_has_color_spine) {
        return output;
    }
    for (int i = field + 1; i < infile[line].getFieldCount(); ++i) {
        if (!infile.token(line, i)->isDataType("**color")) {
            continue;
        }
        output = *infile.token(line, i)->resolveNull();
        if (output == ".") {
            output = "";
        }
        else if (output == "black") {
            output = "";
        }
        else if (output == "#000000") {
            output = "";
        }
        else if (output == "#000") {
            output = "";
        }
        break;
    }

    return output;
}

//////////////////////////////
//
// HumdrumInput::addOrnamentMarkers -- Temporarily convert
//   mordents and trills to text markers (<dir>) for DH.
//   These markers will overwrite any other existing text directions.
//

void HumdrumInput::addOrnamentMarkers(hum::HTp token)
{
    if (!token) {
        return;
    }

    if (strchr(token->c_str(), 'O') != NULL) { // generic ornament
        token->setValue("LO", "TX", "t", "*");
        token->setValue("LO", "TX", "a", "true");
    }
}

//////////////////////////////
//
// HumdrumInput::addSpace -- Add one or more space elements
//    to match the required duration.
//

void HumdrumInput::addSpace(std::vector<string> &elements, std::vector<void *> &pointers, hum::HumNum duration)
{
    bool visible = false;
    if ((!m_signifiers.ispace_color.empty()) || (!m_signifiers.space_color.empty())) {
        visible = true;
    }

    while (duration > 0) {
        if (visible) {
            Rest *rest = new Rest;
            // setLocationId(rest, layerdata[i]);
            // convertRest(rest, layerdata[i]);
            // processSlurs(layerdata[i]);
            // processDynamics(layerdata[i], staffindex);
            // processDirections(layerdata[i], staffindex);
            // int line = layerdata[i]->getLineIndex();
            // int field = layerdata[i]->getFieldIndex();
            // colorRest(rest, "", line, field);
            colorRest(rest, "", -1, -1);
            appendElement(elements, pointers, rest);
            duration -= setDuration(rest, duration);
        }
        else {
            Space *space = new Space;
            // if (m_doc->GetOptions()->m_humType.GetValue()) {
            //    embedQstampInClass(space, layerdata[i], *layerdata[i]);
            // }
            appendElement(elements, pointers, space);
            duration -= setDuration(space, duration);
        }
    }
}

//////////////////////////////
//
// processTerminalLong -- Not set up for chords yet.
//

void HumdrumInput::processTerminalLong(hum::HTp token)
{
    if (!m_signifiers.terminallong) {
        return;
    }
    if (token->find(m_signifiers.terminallong) == string::npos) {
        return;
    }
    token->setValue("LO", "N", "vis", "00");
    if ((token->find('[') != string::npos) || (token->find('_') != string::npos)) {
        removeCharacter(token, '[');
        removeCharacter(token, '_');
        int pitch = hum::Convert::kernToBase40(token);
        hum::HTp testtok = token->getNextToken();
        while (testtok) {
            if (testtok->isBarline()) {
                // make measure invisible:
                testtok->setText(*testtok + "-");
            }
            else if (testtok->isData()) {
                if (testtok->isNull()) {
                    testtok = testtok->getNextToken();
                    continue;
                }
                int tpitch = hum::Convert::kernToBase40(testtok);
                if (tpitch != pitch) {
                    break;
                }
                if ((testtok->find(']') == string::npos) && (testtok->find('_') == string::npos)) {
                    break;
                }
                // make note invisible:
                testtok->setText(*testtok + "yy");
                if (testtok->find("_") != string::npos) {
                    removeCharacter(testtok, '_');
                    testtok = testtok->getNextToken();
                    continue;
                }
                else if (testtok->find("]") != string::npos) {
                    removeCharacter(testtok, ']');
                    break;
                }
            }
            testtok = testtok->getNextToken();
        }
    }
    // If token is tied, then follow ties to attached notes and make invisible.
}

//////////////////////////////
//
// HumdrumInput:: removeCharacter --
//

void HumdrumInput::removeCharacter(hum::HTp token, char removechar)
{
    string output;
    for (char ch : *token) {
        if (ch == removechar) {
            continue;
        }
        output += ch;
    }

    token->setText(output);
}

//////////////////////////////
//
// HumdrumInput::processChordSignifiers --
//

void HumdrumInput::processChordSignifiers(Chord *chord, hum::HTp token, int staffindex)
{
    if (m_signifiers.nostem && token->find(m_signifiers.nostem) != string::npos) {
        chord->SetStemLen(0);
    }

    if (m_signifiers.cuesize) {
        int tcount = 1;
        int cuecount = 0;
        for (int i = 0; i < (int)token->size(); ++i) {
            if ((*token)[i] == m_signifiers.cuesize) {
                cuecount++;
            }
            if ((*token)[i] == ' ') {
                tcount++;
            }
        }
        if ((cuecount > 0) && (tcount == cuecount)) {
            chord->SetCue(BOOLEAN_true);
        }
    }
    else if (m_staffstates.at(staffindex).cue_size.at(m_currentlayer)) {
        chord->SetCue(BOOLEAN_true);
    }

    processTerminalLong(token); // Not tested and probably won't work yet on chords.
}

//////////////////////////////
//
// HumdrumInput::processGlobalDirections --
//

void HumdrumInput::processGlobalDirections(hum::HTp token, int staffindex)
{
    const std::vector<hum::HTp> &staffstarts = m_staffstarts;
    int staffcount = (int)staffstarts.size();

    // only put above or below system (no cases of intermediate
    // directions allowed for now)
    if ((staffindex > 0) && (staffindex < staffcount - 1)) {
        return;
    }

    hum::HumdrumLine *hline = token->getOwner();
    if (hline == NULL) {
        return;
    }

    // Generalize the code below to not duplicate processDirections():

    std::string text = hline->getValue("LO", "TX", "t");
    if (text.size() == 0) {
        return;
    }

    bool zparam = hline->isDefined("LO", "TX", "Z");
    bool yparam = hline->isDefined("LO", "TX", "Y");

    bool aparam = hline->getValueBool("LO", "TX", "a"); // place above staff
    bool bparam = hline->getValueBool("LO", "TX", "b"); // place below staff

    // default font for text string (later check for embedded fonts)
    bool italic = false;
    bool bold = false;

    if (hline->isDefined("LO", "TX", "i")) { // italic
        italic = true;
    }
    if (hline->isDefined("LO", "TX", "B")) { // bold
        bold = true;
    }
    if (hline->isDefined("LO", "TX", "bi")) { // bold-italic
        bold = true;
        italic = true;
    }
    if (hline->isDefined("LO", "TX", "ib")) { // bold-italic
        bold = true;
        italic = true;
    }
    if (hline->isDefined("LO", "TX", "Bi")) { // bold-italic
        bold = true;
        italic = true;
    }
    if (hline->isDefined("LO", "TX", "iB")) { // italic
        bold = true;
        italic = true;
    }

    double Y = 0.0;
    double Z = 0.0;
    std::string placement;
    if (aparam) {
        placement = "above";
    }
    else if (bparam) {
        placement = "below";
    }
    else if (zparam) {
        Z = hline->getValueInt("LO", "TX", "Z");
        if (Z > 0) {
            placement = "above";
        }
        else {
            placement = "below";
        }
    }
    else if (yparam) {
        Y = hline->getValueInt("LO", "TX", "Y");
        if (Y > 0) {
            placement = "below";
        }
        else {
            placement = "above";
        }
    }

    Dir *dir = new Dir;
    setStaff(dir, m_currentstaff);
    setLocationId(dir, token);
    hum::HumNum tstamp = getMeasureTstamp(token, staffindex);
    dir->SetTstamp(tstamp.getFloat());

    if (placement == "above") {
        setPlace(dir, "above");
        m_measure->AddChildBack(dir);
    }
    else if (placement == "below") {
        setPlace(dir, "below");
        m_measure->AddChild(dir);
    }
    else {
        m_measure->AddChild(dir);
    }
    if ((!italic) || bold) {
        Rend *rend = new Rend;
        dir->AddChild(rend);
        addTextElement(rend, text);
        if (!italic) {
            rend->SetFontstyle(FONTSTYLE_normal);
        }
        if (bold) {
            rend->SetFontweight(FONTWEIGHT_bold);
        }
    }
    else {
        addTextElement(dir, text);
    }
}

//////////////////////////////
//
// HumdrumInput::processDirections --
//

void HumdrumInput::processDirections(hum::HTp token, int staffindex)
{
    int lcount = token->getLinkedParameterCount();
    for (int i = 0; i < lcount; ++i) {
        processLinkedDirection(i, token, staffindex);
    }

    // process an internal text direction (no longer common):
    std::string text = token->getValue("LO", "TX", "t");
    if (text.size() == 0) {
        return;
    }

    // maybe add center justification as an option later
    // justification == 0 means no explicit justification (mostly left justified)
    // justification == 1 means right justified
    int justification = 0;
    if (token->isDefined("LO", "TX", "rj")) {
        justification = 1;
    }

    bool zparam = token->isDefined("LO", "TX", "Z");
    bool yparam = token->isDefined("LO", "TX", "Y");

    bool aparam = token->getValueBool("LO", "TX", "a"); // place above staff
    bool bparam = token->getValueBool("LO", "TX", "b"); // place below staff

    // default font for text string (later check for embedded fonts)
    bool italic = false;
    bool bold = false;

    if (token->isDefined("LO", "TX", "i")) { // italic
        italic = true;
    }
    if (token->isDefined("LO", "TX", "B")) { // bold
        bold = true;
    }
    if (token->isDefined("LO", "TX", "bi")) { // bold-italic
        bold = true;
        italic = true;
    }
    if (token->isDefined("LO", "TX", "ib")) { // bold-italic
        bold = true;
        italic = true;
    }
    if (token->isDefined("LO", "TX", "Bi")) { // bold-italic
        bold = true;
        italic = true;
    }
    if (token->isDefined("LO", "TX", "iB")) { // italic
        bold = true;
        italic = true;
    }

    double Y = 0.0;
    double Z = 0.0;
    std::string placement;
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

    addDirection(text, placement, bold, italic, token, staffindex, justification);
}

//////////////////////////////
//
// HumdrumInput::isFirstTokenOnStaff -- Used to control global
//     directions: only one token will be used to generate a direction.
//

bool HumdrumInput::isFirstTokenOnStaff(hum::HTp token)
{
    int target = token->getTrack();
    int track;
    hum::HTp tok = token->getPreviousFieldToken();
    while (tok != NULL) {
        track = tok->getTrack();
        if (track != target) {
            return true;
        }
        if (tok->isNull()) {
            // need to check further
        }
        else {
            return false;
        }
        tok = tok->getPreviousFieldToken();
    }
    return true;
}

//////////////////////////////
//
// HumdrumInput::processLinkedDirection --
//

void HumdrumInput::processLinkedDirection(int index, hum::HTp token, int staffindex)
{
    bool globalQ = token->linkedParameterIsGlobal(index);
    bool firstQ = true;
    if (globalQ) {
        firstQ = isFirstTokenOnStaff(token);
    }

    if (!firstQ) {
        // Don't insert multiple global directions.
        return;
    }

    hum::HumParamSet *hps = token->getLinkedParameter(index);
    if (hps == NULL) {
        return;
    }

    if (hps->getNamespace1() != "LO") {
        return;
    }
    if (hps->getNamespace2() != "TX") {
        return;
    }

    // default font for text string (later check for embedded fonts)
    bool italic = false;
    bool bold = false;
    bool zparam = false;
    bool yparam = false;
    bool aparam = false;
    bool bparam = false;

    // maybe add center justification as an option later
    // justification == 0 means no explicit justification (mostly left justified)
    // justification == 1 means right justified
    int justification = 0;

    string text;

    string key;
    string value;
    for (int i = 0; i < hps->getCount(); ++i) {
        key = hps->getParameterName(i);
        value = hps->getParameterValue(i);
        if (key == "a") {
            aparam = true;
        }
        else if (key == "b") {
            bparam = true;
        }
        else if (key == "t") {
            text = value;
            if (text.size() == 0) {
                // nothing to display
                return;
            }
        }
        else if (key == "Y") {
            yparam = true;
        }
        else if (key == "Z") {
            zparam = true;
        }
        else if (key == "i") {
            italic = true;
        }
        else if (key == "B") {
            bold = true;
        }
        else if (key == "Bi") {
            italic = true;
            bold = true;
        }
        else if (key == "bi") {
            italic = true;
            bold = true;
        }
        else if (key == "iB") {
            italic = true;
            bold = true;
        }
        else if (key == "ib") {
            italic = true;
            bold = true;
        }

        if (key == "rj") {
            justification = 1;
        }
    }

    double Y = 0.0;
    double Z = 0.0;
    std::string placement;
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

    if (token->linkedParameterIsGlobal(index)) {
        int maxstaff = (int)m_staffstarts.size() - 1;

        if ((placement == "below") && (staffindex == maxstaff)) {
            addDirection(text, placement, bold, italic, token, staffindex, justification);
        }
        else if ((placement == "above") && (staffindex == 0)) {
            addDirection(text, placement, bold, italic, token, staffindex, justification);
        }
    }
    else {
        addDirection(text, placement, bold, italic, token, staffindex, justification);
    }
}

//////////////////////////////
//
// HumdrumInput::addDirection --
//     default value: color = "";
//

void HumdrumInput::addDirection(const string &text, const string &placement, bool bold, bool italic, hum::HTp token,
    int staffindex, int justification, const std::string &color)
{

    Dir *dir = new Dir;
    setStaff(dir, m_currentstaff);
    setLocationId(dir, token);
    hum::HumNum tstamp = getMeasureTstamp(token, staffindex);
    if (token->isMens()) {
        // Attach to note, not with measure timestamp.
        // Need to handle text on chords (will currently have a problem attaching to chords)
        string startid = getLocationId("note", token);
        dir->SetStartid("#" + startid);
    }
    else {
        dir->SetTstamp(tstamp.getFloat());
    }

    bool problemQ = false;
    std::string problem = token->getLayoutParameter("TX", "problem");
    if (problem == "true") {
        problemQ = true;
        dir->SetType("problem");
    }

    std::string typevalue = token->getLayoutParameter("TX", "type");
    if (!typevalue.empty()) {
        dir->SetType(typevalue);
    }

    std::string tcolor = token->getLayoutParameter("TX", "color");

    m_measure->AddChild(dir);
    if (placement == "above") {
        setPlace(dir, "above");
    }
    else if (placement == "below") {
        setPlace(dir, "below");
    }
    bool plain = !(italic || bold);
    bool needrend = plain || bold || justification || tcolor.size();
    if (needrend) {
        Rend *rend = new Rend;
        if (!tcolor.empty()) {
            rend->SetColor(tcolor);
        }
        else if (!color.empty()) {
            rend->SetColor(color);
        }
        else if (problemQ) {
            rend->SetColor("red");
        }
        dir->AddChild(rend);
        addTextElement(rend, text);
        if (!italic) {
            rend->SetFontstyle(FONTSTYLE_normal);
        }
        if (bold) {
            rend->SetFontweight(FONTWEIGHT_bold);
        }
        if (justification == 1) {
            rend->SetHalign(HORIZONTALALIGNMENT_right);
        }
    }
    else {
        addTextElement(dir, text);
    }
}

/////////////////////////////
//
// HumdrumInput::processDynamics --
//

void HumdrumInput::processDynamics(hum::HTp token, int staffindex)
{
    std::string tok;
    std::string dynamic;
    bool graceQ = token->isGrace();
    hum::HumdrumLine *line = token->getLine();
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    if (line == NULL) {
        return;
    }
    int track = token->getTrack();
    int lasttrack = track;
    int ttrack;
    int startfield = token->getFieldIndex() + 1;

    bool forceAboveQ = false;
    bool forceBelowQ = false;
    int forcebelowadj = 0;
    // int forceaboveadj = 0;
    int trackdiff = 0;
    int belowadj = 0;
    // int aboveadj = 0;
    int forceQ = false;

    if (ss[staffindex].m_dynampos > 0) {
        forceQ = true;
        forceAboveQ = true;
        // forceaboveadj = -(ss[staffindex].m_dynampos - 1);
    }
    else if (ss[staffindex].m_dynampos < 0) {
        forceQ = true;
        forceBelowQ = true;
        forcebelowadj = -(ss[staffindex].m_dynampos + 1);
    }
    else if (ss[staffindex].verse) {
        forceAboveQ = true;
    }

    int justification = 0;
    if (token->getLayoutParameter("DY", "rj") == "true") {
        justification = 1;
    }

    std::string dcolor = token->getLayoutParameter("DY", "color");

    bool needsrend = justification || dcolor.size();

    // Handle "z" for sforzando (sf), or "zz" for sfz:

    bool aboveQ = false;
    bool belowQ = false;

    auto loc = token->rfind("z");
    if (token->find("zy") != string::npos) {
        // don't show invisible sfz.
        // do nothing
    }
    else if (loc != string::npos) {
        int subtrack = token->getSubtrack();
        switch (subtrack) {
            case 1:
                aboveQ = true;
                belowQ = false;
                break;
            case 2:
                belowQ = true;
                aboveQ = false;
                break;
        }
        if (hasAboveParameter(token, "DY")) {
            aboveQ = true;
            belowQ = false;
        }
        if (hasBelowParameter(token, "DY", belowadj)) {
            aboveQ = false;
            belowQ = true;
            if (belowQ && belowadj) {
                belowadj--;
            }
            else if (forceQ && forceBelowQ) {
                belowadj = forcebelowadj;
            }
        }
        else if (forceQ && forceBelowQ) {
            belowadj = forcebelowadj;
        }
        if (m_signifiers.below && (loc < token->size() - 1) && (token->at(loc + 1) == m_signifiers.below)) {
            aboveQ = false;
            belowQ = true;
        }
        if (m_signifiers.above && (loc < token->size() - 1) && (token->at(loc + 1) == m_signifiers.above)) {
            aboveQ = true;
            belowQ = false;
        }

        Dynam *dynam = new Dynam;
        m_measure->AddChild(dynam);
        setStaff(dynam, m_currentstaff + belowadj);

        if (needsrend) {
            Rend *rend = new Rend;
            dynam->AddChild(rend);
            rend->SetFontweight(rend->AttTypography::StrToFontweight("bold"));
            data_FONTSIZE fs;
            fs.SetTerm(FONTSIZETERM_large);
            rend->SetFontsize(fs);
            if (token->find("zz") != string::npos) {
                addTextElement(rend, "sfz&#160;");
            }
            else {
                addTextElement(rend, "sf&#160;");
            }
            if (!dcolor.empty()) {
                rend->SetColor(dcolor);
            }
            if (justification == 1) {
                rend->SetHalign(HORIZONTALALIGNMENT_right);
            }
        }
        else {
            if (token->find("zz") != string::npos) {
                addTextElement(dynam, "sfz");
            }
            else {
                addTextElement(dynam, "sf");
            }
        }

        setLocationId(dynam, token, -1);
        hum::HumNum barstamp = getMeasureTstamp(token, staffindex);
        dynam->SetTstamp(barstamp.getFloat());

        if (aboveQ) {
            setPlace(dynam, "above");
        }
        else if (belowQ) {
            setPlace(dynam, "below");
        }
        else if (forceAboveQ) {
            setPlace(dynam, "above");
        }
        else if (forceBelowQ) {
            setPlace(dynam, "below");
        }
    }

    for (int i = startfield; i < line->getFieldCount(); ++i) {
        belowadj = 0;
        if (line->token(i)->isKern()) {
            ttrack = line->token(i)->getTrack();
            if (ttrack != track) {
                if (ttrack != lasttrack) {
                    trackdiff++;
                    lasttrack = ttrack;
                }
                if (graceQ) {
                    continue;
                }
                else {
                    break;
                }
            }
            // Break if this is not the last layer for the current spine
            if (!line->token(i)->isNull()) {
                break;
            }
        }
        if (!(line->token(i)->isDataType("**dynam") || line->token(i)->isDataType("**dyn"))) {
            continue;
        }
        // Don't skip NULL tokens, because this algorithm only prints dynamics
        // after the last layer, and there could be notes in earlier layer
        // that need the dynamic.
        // if (line->token(i)->isNull()) {
        //     continue;
        // }

        std::string tok = *line->token(i);
        if (line->token(i)->getValueBool("auto", "DY", "processed")) {
            return;
        }
        line->token(i)->setValue("auto", "DY", "processed", "true");

        // int pcount = line->token(i)->getLinkedParameterCount();

        std::string hairpins;
        std::string letters;
        for (int i = 0; i < (int)tok.size(); ++i) {
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
        else if (letters == "ffz") {
            dynamic = "ffz";
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
            belowadj = 0;
            bool aboveQ = hasAboveParameter(line->token(i), "DY");
            bool belowQ = hasBelowParameter(line->token(i), "DY", belowadj);
            if (belowQ && belowadj) {
                belowadj--;
            }
            else if (forceQ && forceBelowQ && !aboveQ) {
                belowadj = forcebelowadj;
            }

            // if pcount > 0, then search for prefix and postfix text
            // to add to the dynamic.
            // string prefix = "aaa ";
            // string postfix = " bbb";
            // See https://github.com/music-encoding/music-encoding/issues/540

            int justification = 0;
            if (line->token(i)->getLayoutParameter("DY", "rj") == "true") {
                justification = 1;
            }

            std::string dcolor = line->token(i)->getLayoutParameter("DY", "color");
            int needsrend = justification || dcolor.size();

            Dynam *dynam = new Dynam;
            m_measure->AddChild(dynam);
            setStaff(dynam, m_currentstaff + belowadj);
            setLocationId(dynam, line->token(i), -1);

            if (needsrend) {
                Rend *rend = new Rend;
                dynam->AddChild(rend);
                rend->SetFontweight(rend->AttTypography::StrToFontweight("bold"));
                data_FONTSIZE fs;
                fs.SetTerm(FONTSIZETERM_large);
                rend->SetFontsize(fs);
                // addTextElement(rend, prefix);
                std::string newtext = dynamic + "&#160;";
                addTextElement(rend, newtext);
                // addTextElement(rend, postfix);
                if (!dcolor.empty()) {
                    rend->SetColor(dcolor);
                }
                if (justification == 1) {
                    rend->SetHalign(HORIZONTALALIGNMENT_right);
                }
            }
            else {
                // addTextElement(dynam, prefix);
                addTextElement(dynam, dynamic);
                // addTextElement(dynam, postfix);
            }

            hum::HumNum linedur = line->getDuration();
            if (linedur == 0) {
                // Grace note line, so attach to the note rather than
                // the timestamp.
                if (token->isChord()) {
                    dynam->SetStartid("#" + getLocationId("chord", token));
                }
                else {
                    // maybe check if a null token instead of a note here.
                    dynam->SetStartid("#" + getLocationId("note", token));
                }
            }
            else {
                hum::HumNum barstamp = getMeasureTstamp(token, staffindex);
                dynam->SetTstamp(barstamp.getFloat());
            }

            std::string verticalgroup = line->token(i)->getLayoutParameter("DY", "vg");
            if (verticalgroup.empty()) {
                // 100 is the default group for dynamics:
                dynam->SetVgrp(VGRP_DYNAM_DEFAULT);
            }
            else if (std::isdigit(verticalgroup[0])) {
                dynam->SetVgrp(stoi(verticalgroup));
            }
            else {
                // don't set a vertical group for this token
            }

            if (trackdiff == 1) {
                // case needed for grace notes in the bottom staff of a grand staff.
                setPlace(dynam, "above");
            }
            if (aboveQ) {
                setPlace(dynam, "above");
            }
            else if (belowQ) {
                setPlace(dynam, "below");
            }
            else if (forceAboveQ) {
                setPlace(dynam, "above");
            }
            else if (forceBelowQ) {
                setPlace(dynam, "below");
            }
        }
        if (hairpins.find("<") != string::npos) {
            int endline = false;
            hum::HTp endtok = NULL;
            if (hairpins.find("<[") != string::npos) {
                endtok = token;
                endline = true;
            }
            else {
                endtok = getCrescendoEnd(line->token(i));
            }

            belowadj = 0;
            bool aboveQ = hasAboveParameter(line->token(i), "HP");
            bool belowQ = hasBelowParameter(line->token(i), "HP", belowadj);
            if (belowQ && belowadj) {
                belowadj--;
            }
            else if (forceQ && forceBelowQ) {
                belowadj = forcebelowadj;
            }

            if (endtok != NULL) {
                Hairpin *hairpin = new Hairpin;
                setStaff(hairpin, m_currentstaff + belowadj);
                setLocationId(hairpin, line->token(i), -1);
                hum::HumNum tstamp = getMeasureTstamp(line->token(i), staffindex);
                hum::HumNum tstamp2 = getMeasureTstamp(endtok, staffindex);
                if (endline || (endtok->find("[[") != std::string::npos)) {
                    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
                    hum::HumNum mfactor = ss[staffindex].meter_bottom / 4;
                    tstamp2 += endtok->getLine()->getDuration() * mfactor;
                }
                int measures = getMeasureDifference(line->token(i), endtok);
                hairpin->SetTstamp(tstamp.getFloat());
                pair<int, double> ts2(measures, tstamp2.getFloat());
                hairpin->SetTstamp2(ts2);
                hairpin->SetForm(hairpinLog_FORM_cres);

                std::string verticalgroup = line->token(i)->getLayoutParameter("HP", "vg");
                if (verticalgroup.empty()) {
                    // 100 is the default group for dynamics:
                    hairpin->SetVgrp(VGRP_DYNAM_DEFAULT);
                }
                else if (std::isdigit(verticalgroup[0])) {
                    hairpin->SetVgrp(stoi(verticalgroup));
                }
                else {
                    // don't set a vertical group for this token
                }

                m_measure->AddChild(hairpin);

                if (aboveQ) {
                    setPlace(hairpin, "above");
                }
                else if (belowQ) {
                    setPlace(hairpin, "below");
                }
                else if (forceAboveQ) {
                    setPlace(hairpin, "above");
                }
                else if (forceBelowQ) {
                    setPlace(hairpin, "below");
                }
            }
            else {
                // no endpoint so print as the word "cresc."
                Dir *dir = new Dir;
                m_measure->AddChild(dir);
                setStaff(dir, m_currentstaff + belowadj);
                setLocationId(dir, line->token(i));
                hum::HumNum tstamp = getMeasureTstamp(line->token(i), staffindex);
                dir->SetTstamp(tstamp.getFloat());
                std::string fontstyle;
                std::string content = "cresc.";
                if (!m_signifiers.cresctext.empty()) {
                    content = m_signifiers.cresctext;
                    fontstyle = m_signifiers.crescfontstyle;
                }
                addTextElement(dir, content, fontstyle);
            }
        }
        else if (hairpins.find(">") != string::npos) {
            int endline = false;
            hum::HTp endtok = NULL;
            if (hairpins.find(">]") != string::npos) {
                endtok = token;
                endline = true;
            }
            else {
                endtok = getDecrescendoEnd(line->token(i));
            }

            belowadj = 0;
            bool aboveQ = hasAboveParameter(line->token(i), "HP");
            bool belowQ = hasBelowParameter(line->token(i), "HP", belowadj);
            if (belowQ && belowadj) {
                belowadj--;
            }
            else if (forceQ && forceBelowQ) {
                belowadj = forcebelowadj;
            }
            if (endtok != NULL) {
                Hairpin *hairpin = new Hairpin;
                setStaff(hairpin, m_currentstaff + belowadj);
                setLocationId(hairpin, line->token(i), -1);
                hum::HumNum tstamp = getMeasureTstamp(line->token(i), staffindex);
                hum::HumNum tstamp2 = getMeasureTstamp(endtok, staffindex);
                if (endline || (endtok->find("]]") != std::string::npos)) {
                    tstamp2 += endtok->getLine()->getDuration();
                }
                int measures = getMeasureDifference(line->token(i), endtok);
                hairpin->SetTstamp(tstamp.getFloat());
                pair<int, double> ts2(measures, tstamp2.getFloat());
                hairpin->SetTstamp2(ts2);
                hairpin->SetForm(hairpinLog_FORM_dim);
                m_measure->AddChild(hairpin);

                std::string verticalgroup = line->token(i)->getLayoutParameter("HP", "vg");
                if (verticalgroup.empty()) {
                    // 100 is the default group for dynamics:
                    hairpin->SetVgrp(VGRP_DYNAM_DEFAULT);
                }
                else if (std::isdigit(verticalgroup[0])) {
                    hairpin->SetVgrp(stoi(verticalgroup));
                }
                else {
                    // don't set a vertical group for this token
                }

                if (aboveQ) {
                    setPlace(hairpin, "above");
                }
                else if (belowQ) {
                    setPlace(hairpin, "below");
                }
                else if (forceAboveQ) {
                    setPlace(hairpin, "above");
                }
            }
            else {
                // no endpoint so print as the word "decresc."
                Dir *dir = new Dir;
                m_measure->AddChild(dir);
                setStaff(dir, m_currentstaff + belowadj);
                setLocationId(dir, line->token(i));
                bool aboveQ = hasAboveParameter(line->token(i), "HP");
                if (aboveQ) {
                    setPlace(dir, "above");
                }
                hum::HumNum tstamp = getMeasureTstamp(line->token(i), staffindex);
                dir->SetTstamp(tstamp.getFloat());
                std::string fontstyle = "";
                std::string content = "decresc.";
                if (!m_signifiers.decresctext.empty()) {
                    content = m_signifiers.decresctext;
                    fontstyle = m_signifiers.decrescfontstyle;
                }
                addTextElement(dir, content, fontstyle);
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
// HumdrumInput::hasAboveParameter -- true if has an "a" parameter or has a "Z" parameter set to anything.
//

bool HumdrumInput::hasAboveParameter(hum::HTp token, const string &category)
{
    int lcount = token->getLinkedParameterCount();
    if (lcount == 0) {
        return 0;
    }

    for (int p = 0; p < token->getLinkedParameterCount(); ++p) {
        hum::HumParamSet *hps = token->getLinkedParameter(p);
        if (hps == NULL) {
            continue;
        }
        if (hps->getNamespace1() != "LO") {
            continue;
        }
        if (hps->getNamespace2() != category) {
            continue;
        }
        for (int q = 0; q < hps->getCount(); ++q) {
            string key = hps->getParameterName(q);
            string value = hps->getParameterValue(q);
            if (key == "a") {
                return true;
            }
            if (key == "Z") {
                return true;
            }
        }
    }
    return false;
}

//////////////////////////////
//
// HumdrumInput::hasBelowParameter -- true if has an "b" parameter or has a "Z" parameter set to anything.
//

bool HumdrumInput::hasBelowParameter(hum::HTp token, const string &category)
{
    int lcount = token->getLinkedParameterCount();
    if (lcount == 0) {
        return 0;
    }

    for (int p = 0; p < token->getLinkedParameterCount(); ++p) {
        hum::HumParamSet *hps = token->getLinkedParameter(p);
        if (hps == NULL) {
            continue;
        }
        if (hps->getNamespace1() != "LO") {
            continue;
        }
        if (hps->getNamespace2() != category) {
            continue;
        }
        for (int q = 0; q < hps->getCount(); ++q) {
            string key = hps->getParameterName(q);
            string value = hps->getParameterValue(q);
            if (key == "b") {
                return true;
            }
            if (key == "Y") {
                return true;
            }
        }
    }
    return false;
}

bool HumdrumInput::hasBelowParameter(hum::HTp token, const string &category, int &output)
{
    output = 0;
    int lcount = token->getLinkedParameterCount();
    if (lcount == 0) {
        return 0;
    }

    for (int p = 0; p < token->getLinkedParameterCount(); ++p) {
        hum::HumParamSet *hps = token->getLinkedParameter(p);
        if (hps == NULL) {
            continue;
        }
        if (hps->getNamespace1() != "LO") {
            continue;
        }
        if (hps->getNamespace2() != category) {
            continue;
        }
        for (int q = 0; q < hps->getCount(); ++q) {
            string key = hps->getParameterName(q);
            string value = hps->getParameterValue(q);
            if (key == "b") {
                if (value == "true") {
                    output = 1;
                }
                else if (!value.empty()) {
                    if (isdigit(value[0])) {
                        output = stoi(value);
                    }
                }
                return true;
            }
            if (key == "Y") {
                return true;
            }
        }
    }
    return false;
}

//////////////////////////////
//
// HumdrumInput::getMeasureDifference --
//

int HumdrumInput::getMeasureDifference(hum::HTp starttok, hum::HTp endtok)
{
    if (endtok == NULL) {
        return 0;
    }
    hum::HumdrumLine *line = starttok->getOwner();
    if (line == NULL) {
        return 0;
    }
    hum::HumdrumFile *file = line->getOwner();
    if (file == NULL) {
        return 0;
    }
    hum::HumdrumFile &infile = *file;
    int startline = starttok->getLineIndex();
    int endline = endtok->getLineIndex();
    int counter = 0;
    for (int i = startline; i <= endline; ++i) {
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

hum::HTp HumdrumInput::getCrescendoEnd(hum::HTp token)
{
    return getHairpinEnd(token, "[");
}

//////////////////////////////
//
// HumdrumInput::getDecrescendoEnd --
//

hum::HTp HumdrumInput::getDecrescendoEnd(hum::HTp token)
{
    return getHairpinEnd(token, "]");
}

//////////////////////////////
//
// HumdrumInput::getHairpinEnd --
//

hum::HTp HumdrumInput::getHairpinEnd(hum::HTp token, const std::string &endchar)
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
        for (int i = 0; i < (int)token->size(); ++i) {
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
//     default value: fract = 0.0;
//

hum::HumNum HumdrumInput::getMeasureTstamp(hum::HTp token, int staffindex, hum::HumNum fract)
{
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    hum::HumNum qbeat = token->getDurationFromBarline();
    if (fract > 0) {
        // what is this for? Causes problems with pedal markings.
        // qbeat += fract * token->getDuration().getAbs();
    }
    hum::HumNum mfactor = ss[staffindex].meter_bottom / 4;
    // if (ss[staffindex].meter_bottom == 0) {
    //  mfactor = 1;
    //  mfactor /= 8;
    // }
    hum::HumNum mbeat = qbeat * mfactor + 1;
    return mbeat;
}

//////////////////////////////
//
// HumdrumInput::getMeasureFactor -- Get the metric unit of the current measure.
//

hum::HumNum HumdrumInput::getMeasureFactor(int staffindex)
{
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    hum::HumNum mfactor = ss[staffindex].meter_bottom / 4;
    return mfactor;
}

//////////////////////////////
//
// HumdrumInput::getMeasureTstampPlusDur --  Similar to getMeasureTstamp, but also include
//     duration of token (to get endpoint of token in measure).
//     default value: fract = 0.0;
//

hum::HumNum HumdrumInput::getMeasureTstampPlusDur(hum::HTp token, int staffindex, hum::HumNum fract)
{
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    hum::HumNum qbeat = token->getDurationFromBarline() + token->getDuration();
    if (fract > 0) {
        // what is this for? Causes problems with pedal markings.
        // qbeat += fract * token->getDuration().getAbs();
    }
    hum::HumNum mfactor = ss[staffindex].meter_bottom / 4;
    // if (ss[staffindex].meter_bottom == 0) {
    //  mfactor = 1;
    //  mfactor /= 8;
    // }
    hum::HumNum mbeat = qbeat * mfactor + 1;
    return mbeat;
}

//////////////////////////////
//
// HumdrumInput::getMeasureEndTstamp -- Return the tstamp of the end of the
//     measure (the next barline). (@meter.count + 1)
//     default value: fract = 0.0;
//

hum::HumNum HumdrumInput::getMeasureEndTstamp(int staffindex)
{
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    return ss[staffindex].meter_top + 1;
}

/////////////////////////////
//
// HumdumInput::addTextElement -- Append text to a regular element.
//   default value: fontstyle == ""
//

template <class ELEMENT>
void HumdrumInput::addTextElement(ELEMENT *element, const std::string &content, const std::string &fontstyle)
{
    Text *text = new Text;
    std::string data = content;
    if (element->GetClassName() == "Syl") {
        // Approximate centering of single-letter text on noteheads.
        // currently the text is left justified to the left edge of the notehead.
        if (content.size() == 1) {
            data = "&#160;" + data;
        }
    }

    if (data.find("[") != std::string::npos) {
        data = replaceMusicShapes(data);
    }
    data = unescapeHtmlEntities(data);
    text->SetText(UTF8to16(data));

    if (fontstyle.empty()) {
        element->AddChild(text);
    }
    else {
        Rend *rend = new Rend;
        element->AddChild(rend);
        rend->AddChild(text);
        if (fontstyle == "normal") {
            rend->SetFontstyle(rend->AttTypography::StrToFontstyle("normal"));
        }
        else if (fontstyle == "bold") {
            rend->SetFontweight(rend->AttTypography::StrToFontweight("bold"));
            rend->SetFontstyle(rend->AttTypography::StrToFontstyle("normal"));
        }
        else if (fontstyle == "bold-italic") {
            rend->SetFontweight(rend->AttTypography::StrToFontweight("bold"));
        }
    }
}

//////////////////////////////
//
// HumdrumInput::replaceMusicShapes --
//    [thirtysecond] => &#X1d162;
//    [sixteenth]    => &#X1d161;
//    [eighth]       => &#x266a; or &#x1d160;
//    [quarter]      => &#x2669; or &#x1d15f; (\xF0\x9D\x85\xBD)
//    [half]         =>  or &#x1d15e;
//    [whole]        =>  or &#x1d15d;
//    [breve]        =>  or &#x1d15c;
//    [ped]          => &#x1d1ae; (\xF0\x9D\x86\xAE)
//    [segno]        => &#x1d10b; (\xF0\x9D\x84\x8B)
//    [coda]         => &#x1d10c; (\xF0\x9D\x84\x8C)
// long values do not work, so only quarter and eight at the moment.
//

std::string HumdrumInput::replaceMusicShapes(const std::string input)
{
    std::string output = input;
    hum::HumRegex hre;
    // SMUFL: hre.replaceDestructive(output, "&#xe1d5;", "\\[quarter\\]", "g");
    // hre.replaceDestructive(output, "&#x1d15d;", "\\[whole\\]", "g");
    // hre.replaceDestructive(output, "&#x1d15e;", "\\[half\\]", "g");
    hre.replaceDestructive(output, "&#x2669;.", "\\[quarter-dot\\]", "g");
    hre.replaceDestructive(output, "&#x266a;.", "\\[eighth-dot\\]", "g");
    hre.replaceDestructive(output, "&#x2669;", "\\[quarter\\]", "g");
    hre.replaceDestructive(output, "&#x266a;", "\\[eighth\\]", "g");
    hre.replaceDestructive(output, "\xF0\x9D\x86\xAE", "\\[[Pp]ed\\.?\\]", "g");
    hre.replaceDestructive(output, "\xF0\x9D\x84\x8B", "\\[[Ss]egno\\]", "g");
    hre.replaceDestructive(output, "\xF0\x9D\x84\x8C", "\\[[Cc]oda\\]", "g");
    // hre.replaceDestructive(output, "&#x1d161;", "\\[sixteenth\\]", "g");
    return output;
}

/////////////////////////////
//
// HumdrumInput::processSlurs --
//

void HumdrumInput::processSlurs(hum::HTp slurend)
{
    int startcount = slurend->getValueInt("auto", "slurStartCount");
    if (startcount <= 0) {
        return;
    }

    // slurstarts contains a list of the correctly paired slurs
    // attached to the note/chord.  Deal with unopened slurs
    // here later (such as an excerpt of music where the opening
    // of the slur is not present in the data).
    std::vector<hum::HTp> slurstarts;
    for (int i = 0; i < startcount; i++) {
        hum::HTp tok;
        tok = slurend->getSlurStartToken(i + 1);
        if (tok) {
            slurstarts.push_back(tok);
        }
    }

    // slurindex contains a list of the indexes into slurstarts,
    // with all identical slur starts placed on the first
    // position that the note/chord is found in the slurstarts list.
    std::vector<std::vector<int> > slurindex;
    slurindex.resize(slurstarts.size());
    for (int i = 0; i < (int)slurstarts.size(); i++) {
        for (int j = 0; j <= i; j++) {
            if (slurstarts[i] == slurstarts[j]) {
                slurindex[j].push_back(i);
                break;
            }
        }
    }

    std::vector<bool> indexused(32, false);

    std::vector<pair<int, bool> > slurendnoteinfo;
    extractSlurNoteAttachmentInformation(slurendnoteinfo, slurend, ')');

    int endsubtokcount = slurend->getSubtokenCount();
    std::vector<int> endpitches;
    for (int i = 0; i < endsubtokcount; i++) {
        std::string subtok = slurend->getSubtoken(i);
        if (subtok.find("r") != std::string::npos) {
            endpitches.push_back(0);
        }
        else {
            endpitches.push_back(hum::Convert::kernToBase7(subtok));
        }
    }
    std::vector<pair<int, int> > endchordsorted;
    endchordsorted.reserve(endsubtokcount);
    pair<int, int> v;
    for (int i = 0; i < endsubtokcount; i++) {
        v.first = endpitches[i];
        v.second = i;
        endchordsorted.push_back(v);
    }
    std::sort(endchordsorted.begin(), endchordsorted.end());

    int startsubtokcount;
    std::vector<int> startpitches;

    for (int i = 0; i < (int)slurindex.size(); i++) {
        std::vector<pair<int, bool> > slurstartnoteinfo;
        extractSlurNoteAttachmentInformation(slurstartnoteinfo, slurstarts.at(i), '(');

        startsubtokcount = slurstarts[i]->getSubtokenCount();
        startpitches.clear();
        for (int j = 0; j < startsubtokcount; j++) {
            std::string subtok = slurstarts[i]->getSubtoken(j);
            if (subtok.find("r") != std::string::npos) {
                startpitches.push_back(0);
            }
            else {
                startpitches.push_back(hum::Convert::kernToBase7(subtok));
            }
        }
        std::vector<std::pair<int, int> > startchordsorted;
        startchordsorted.reserve(startsubtokcount);

        pair<int, int> v;
        for (int i = 0; i < startsubtokcount; i++) {
            v.first = startpitches[i];
            v.second = i;
            startchordsorted.push_back(v);
        }
        std::sort(startchordsorted.begin(), startchordsorted.end());
        for (int j = 0; j < (int)slurindex[i].size(); j++) {
            hum::HTp slurstart = slurstarts[slurindex[i][j]];

            std::vector<pair<int, bool> > slurstartnoteinfo;
            extractSlurNoteAttachmentInformation(slurstartnoteinfo, slurstart, '(');
            if (!slurstart) {
                // should never occur...
                return;
            }

            int mindex;
            std::string mindexstring = slurstart->getValue("MEI", "measureIndex");
            if (mindexstring == "") {
                // cross-layer sluring into later layer.  The beginning of the slur
                // is in the same measure since it has not yet been processed.
                mindex = slurend->getValueInt("MEI", "measureIndex");
            }
            else {
                mindex = slurstart->getValueInt("MEI", "measureIndex");
            }

            Measure *startmeasure = m_measures[mindex];
            Slur *slur = new Slur;

            addLineFormStyle(slur, slurstart, "S");

            // start ID can sometimes not be set yet due to cross layer slurs.
            std::string startid = slurstart->getValue("MEI", "xml:id");
            std::string endid = slurend->getValue("MEI", "xml:id");

            if (startid == "") {
                if (slurstart->isChord()) {
                    startid = "chord-L";
                }
                else {
                    startid = "note-L";
                }
                startid += to_string(slurstart->getLineNumber());
                startid += "F";
                startid += to_string(slurstart->getFieldNumber());
                slurstart->setValue("MEI", "xml:id", startid);
                startid = slurstart->getValue("MEI", "xml:id");
            }

            if (slurindex[i].size() > 1) {
                if (endpitches.size() > 1) {
                    calculateNoteIdForSlur(endid, endchordsorted, j);
                }
                if (startpitches.size() > 1) {
                    calculateNoteIdForSlur(startid, startchordsorted, j);
                }
            }

            if (slurendnoteinfo.at(i).second) {
                if (endid.find("chord") != std::string::npos) {
                    hum::HumRegex hre;
                    hre.replaceDestructive(endid, "note", "chord");
                    endid += "S";
                    endid += to_string(slurendnoteinfo[i].first + 1);
                }
            }

            if (slurstartnoteinfo.at(j).second) {
                if (startid.find("chord") != std::string::npos) {
                    hum::HumRegex hre;
                    hre.replaceDestructive(startid, "note", "chord");
                    startid += "S";
                    startid += to_string(slurstartnoteinfo[i].first + 1);
                }
            }

            slur->SetEndid("#" + endid);
            slur->SetStartid("#" + startid);
            setSlurLocationId(slur, slurstart, slurend, j);

            startmeasure->AddChild(slur);
            if (slurstart->getTrack() == slurend->getTrack()) {
                // If the slur starts and ends on different staves,
                // do not specify the staff attribute, but later
                // add a list of the two staves involved.
                setStaff(slur, m_currentstaff);
            }

            if (hasAboveParameter(slurstart, "S")) {
                slur->SetCurvedir(curvature_CURVEDIR_above);
            }
            else if (hasBelowParameter(slurstart, "S")) {
                slur->SetCurvedir(curvature_CURVEDIR_below);
            }

            std::string eid = slurend->getValue("auto", "id");
            int sluridx = getSlurEndIndex(slurstart, eid, indexused);
            if (sluridx < 0) {
                continue;
            }
            indexused.at(sluridx) = true;

            // Calculate if the slur should be forced above or below
            // this is the case for doubly slured chords.  Only the first
            // two slurs between a pair of notes/chords will be oriented
            // (other slurs will need to be manually adjusted and probably
            // linked to individual notes to avoid overstriking the first
            // two slurs.
            if (slurindex[i].size() >= 2) {
                if (slurstarts[slurindex[i][0]] == slurstarts[slurindex[i][1]]) {
                    if (j == 0) {
                        slur->SetCurvedir(curvature_CURVEDIR_above);
                    }
                    else {
                        slur->SetCurvedir(curvature_CURVEDIR_below);
                    }
                }
            }

            if (m_signifiers.above) {
                int count = -1;
                for (int k = (int)slurstart->size() - 2; k >= 0; k--) {
                    if (slurstart->at(k) == '(') {
                        count++;
                    }
                    if (count == sluridx) {
                        if (slurstart->at(k + 1) == m_signifiers.above) {
                            slur->SetCurvedir(curvature_CURVEDIR_above);
                        }
                        break;
                    }
                }
            }

            if (m_signifiers.below) {
                int count = -1;
                for (int k = (int)slurstart->size() - 2; k >= 0; k--) {
                    if (slurstart->at(k) == '(') {
                        count++;
                    }
                    if (count == sluridx) {
                        if (slurstart->at(k + 1) == m_signifiers.below) {
                            slur->SetCurvedir(curvature_CURVEDIR_below);
                        }
                        break;
                    }
                }
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::addLineFormStyle -- Add dotted or dashed line information to an element from
//    layout parameters.
//        Default parameter: index = 0.
//

template <class ELEMENT>
void HumdrumInput::addLineFormStyle(ELEMENT element, hum::HTp token, const string &layout, int index)
{
    string dashed = token->getLayoutParameter(layout, "dash");
    string dotted = token->getLayoutParameter(layout, "dot");
    if (!dotted.empty()) {
        element->SetLform(LINEFORM_dotted);
    }
    else if (!dashed.empty()) {
        element->SetLform(LINEFORM_dashed);
    }
}

//////////////////////////////
//
// HumdrumInput::extractSlurNoteAttachmentInformation --
//   Vector indexed by slur (open or close depending on slurtype) with data being the subtoken number and
//   boolean for attached to note instead of chord.
//

void HumdrumInput::extractSlurNoteAttachmentInformation(
    std::vector<std::pair<int, bool> > &data, hum::HTp token, char slurtype)
{
    // slurtype == '(' for slur start
    // slurtype == ')' for slur end
    data.clear();
    int subtokindex = 0;
    int slurnumber = 0;
    int toksize = (int)token->size();
    bool notestate;
    for (int i = 0; i < toksize; i++) {
        if (token->at(i) == ' ') {
            subtokindex++;
        }
        else if (token->at(i) == ')') {
            slurnumber++;
            if (slurtype == ')') {
                notestate = getNoteState(token, slurnumber);
                data.emplace_back(std::make_pair(subtokindex, notestate));
            }
        }
        else if (token->at(i) == '(') {
            slurnumber++;
            if (slurtype == '(') {
                notestate = getNoteState(token, slurnumber);
                data.emplace_back(std::make_pair(subtokindex, notestate));
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::getNoteState -- Return any slur attachment to a note parameter in a layout command for slurs.
//

bool HumdrumInput::getNoteState(hum::HTp token, int slurnumber)
{
    std::string data = token->getSlurLayoutParameter("note", slurnumber - 1);
    if (data == "true") {
        return true;
    }
    else {
        return false;
    }
}

//////////////////////////////
//
// HumdrumInput::calculateNoteIdForSlur --
//

void HumdrumInput::calculateNoteIdForSlur(std::string &idstring, std::vector<pair<int, int> > &sortednotes, int index)
{
    int notecount = (int)sortednotes.size();
    hum::HumRegex hre;
    if (notecount == 1) {
        hre.replaceDestructive(idstring, "note-", "chord-");
    }

    /* Not attaching multiple slurs to note anymore, but leaving code
            here in case there is a later need for it:

        int position = 0;

        // odd:
        // input (index):    0 1 2 3 4
        // output       :    0 2 4 3 1
        // even:
        // input (index):    0 1 2 3
        // output       :    0 2 3 1

        if (index < (notecount + 1) / 2) {
            position = index * 2;
        }
        else {
            position = (notecount - index) * 2 - 1;
        }
        if (position < 0) {
            position += 100 * notecount;
            position %= notecount;
        }
        int subtoken = sortednotes.at(position).second;
        // collapse cases where there are more slurs than notes:
        subtoken = subtoken % notecount;
        // offset from 1 instead of 0:
        subtoken++;
        idstring += "S" + to_string(subtoken);
    */
}

/////////////////////////////
//
// HumdrumInput::getSlurEndIndex --
//

int HumdrumInput::getSlurEndIndex(hum::HTp token, std::string targetid, std::vector<bool> &indexused)
{
    int endcount = token->getValueInt("auto", "slurEndCount");
    std::string parameter;
    std::string endid;

    int save = -1;
    for (int i = 0; i < endcount; ++i) {
        parameter = "slurEnd";
        if (i > 0) {
            parameter += to_string(i + 1);
        }
        endid = token->getValue("auto", parameter);
        if (endid == targetid) {
            if (indexused.at(i)) {
                save = i;
                continue;
            }
            return i;
        }
    }

    return save;
}

/////////////////////////////
//
// HumdrumInput::insertMeterSigElement -- A time signature or meter signature
//    that starts after the beginning of a movement.
//

void HumdrumInput::insertMeterSigElement(
    std::vector<string> &elements, std::vector<void *> &pointers, std::vector<hum::HTp> &layerdata, int index)
{
    hum::HTp tsig = layerdata[index];
    if (!tsig) {
        return;
    }
    if (tsig->getDurationFromStart() <= 0) {
        return;
    }
    std::smatch matches;
    int count = -1;
    int unit = -1;
    if (regex_search(*tsig, matches, regex("^\\*M(\\d+)/(\\d+)"))) {
        count = stoi(matches[1]);
        unit = stoi(matches[2]);
    }
    else if (regex_search(*tsig, matches, regex("^\\*M(\\d+)"))) {
        count = stoi(matches[1]);
    }
    // deal with non-rational units here.
    if (count < 0) {
        return;
    }
    MeterSig *msig = new MeterSig;
    if (tsig) {
        setLocationId(msig, tsig);
    }
    appendElement(elements, pointers, msig);
    msig->SetCount(count);
    if (unit > 0) {
        msig->SetUnit(unit);
    }
    // check for mensuration here.
}

//////////////////////////////
//
// HumdrumInput::getMeterSig -- Return the MeterSig element of
//   the given object, or if there is none create it and
//   return a pointer to it. ELEMENT can be ScoreDef or StaffDef.
//

template <class ELEMENT> MeterSig *HumdrumInput::getMeterSig(ELEMENT element)
{
    MeterSig *output = (MeterSig *)element->FindDescendantByType(ClassId::METERSIG);
    if (!output) {
        output = new MeterSig;
        element->AddChild(output);
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::getKeySig -- Return the KeySig element of
//   the given object, or if there is none create it and
//   return a pointer to it. ELEMENT can be ScoreDef or StaffDef.
//

template <class ELEMENT> KeySig *HumdrumInput::getKeySig(ELEMENT element)
{
    KeySig *output = (KeySig *)element->FindDescendantByType(ClassId::KEYSIG);
    if (!output) {
        output = new KeySig;
        element->AddChild(output);
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::getClef -- Return the Clef element of
//   the given object, or if there is none create it and
//   return a pointer to it. ELEMENT can be ScoreDef or StaffDef.
//

template <class ELEMENT> Clef *HumdrumInput::getClef(ELEMENT element)
{
    Clef *output = (Clef *)element->FindDescendantByType(ClassId::KEYSIG);
    if (!output) {
        output = new Clef;
        element->AddChild(output);
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::getMensur -- Return the Mensur element of
//   the given object, or if there is none create it and
//   return a pointer to it. ELEMENT can be ScoreDef or StaffDef.
//

template <class ELEMENT> Mensur *HumdrumInput::getMensur(ELEMENT element)
{
    Mensur *output = (Mensur *)element->FindDescendantByType(ClassId::MENSUR);
    if (!output) {
        output = new Mensur;
        element->AddChild(output);
    }
    return output;
}

/////////////////////////////
//
// HumdrumInput::addSystemKeyTimeChange -- Add key or time signature changes
//    for a measure.  The scoreDef element is inserted before the measure in
//    which the change occurs. Presuming all staves have same changes for now.
//

void HumdrumInput::addSystemKeyTimeChange(int startline, int endline)
{
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    hum::HumdrumFile &infile = m_infiles[0];

    hum::HTp keysig = NULL;
    hum::HTp timesig = NULL;
    hum::HTp metersig = NULL;

    for (int i = startline; i <= endline; ++i) {
        if (infile[i].isData()) {
            break;
        }
        if (!infile[i].isInterpretation()) {
            continue;
        }
        for (int j = 0; j < infile[i].getFieldCount(); ++j) {
            if ((!timesig) && regex_search(*infile.token(i, j), regex("^\\*M\\d+/\\d+"))) {
                timesig = infile.token(i, j);
            }
            if ((!keysig) && regex_search(*infile.token(i, j), regex("^\\*k\\[.*\\]"))) {
                keysig = infile.token(i, j);
            }
            if (timesig && regex_search(*infile.token(i, j), regex("^\\*met\\(.*\\)"))) {
                metersig = infile.token(i, j);
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
        std::smatch matches;
        if (regex_search(*timesig, matches, regex("^\\*M(\\d+)/(\\d+)"))) {
            if (!metersig) {
                count = stoi(matches[1]);
                unit = stoi(matches[2]);
                MeterSig *vrvmetersig = getMeterSig(scoreDef);
                vrvmetersig->SetCount(count);
                vrvmetersig->SetUnit(unit);
            }
            else if (metersig && (metersig->find('C') == std::string::npos)
                && (metersig->find('O') == std::string::npos)) {
                // Only storing the time signature if there is no mensuration
                // otherwise verovio will display both.
                count = stoi(matches[1]);
                unit = stoi(matches[2]);
                MeterSig *vrvmetersig = getMeterSig(scoreDef);
                vrvmetersig->SetCount(count);
                vrvmetersig->SetUnit(unit);
            }
            else {
                // But always need to provide @meter.unit since timestamps
                // are in reference to it (can't add meter.count since
                // this will also print a time signature.
                unit = stoi(matches[2]);
                MeterSig *vrvmetersig = getMeterSig(scoreDef);
                vrvmetersig->SetUnit(unit);
            }
            if (metersig) {
                auto ploc = metersig->rfind(")");
                if (ploc != string::npos) {
                    string mstring = metersig->substr(5, ploc - 5);
                    setMeterSymbol(scoreDef, mstring);
                }
            }

            for (int i = 0; i < (int)ss.size(); ++i) {
                // assuming only a single time sig. at a time.
                ss[i].meter_top = count;
                ss[i].meter_bottom = unit;
            }
        }
    }
    if (keysig) {
        // cerr << "KEYSIG = " << keysig << endl;
        setKeySig(-1, scoreDef, *((string *)keysig), keysig, true);
    }
}

/////////////////////////////
//
// HumdrumInput::insertClefElement -- A clef which starts after the beginning of
// the movement.
//

Clef *HumdrumInput::insertClefElement(std::vector<string> &elements, std::vector<void *> &pointers, hum::HTp token)
{
    bool iseditorial = getBooleanParameter(token, "CL", "ed");
    std::string color = getStringParameter(token, "CL", "color");
    Clef *clef = new Clef;
    if (iseditorial) {
        Supplied *supplied = new Supplied;
        appendElement(supplied, clef);
        appendElement(elements, pointers, supplied);
        if (color.empty()) {
            clef->SetColor("#aaa"); // hard-code to gray by default for now
        }
        else {
            clef->SetColor(color);
        }
        clef->SetType("editorial");
    }
    else {
        appendElement(elements, pointers, clef);
        if (!color.empty()) {
            clef->SetColor(color);
        }
    }

    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    ss.at(m_currentstaff - 1).last_clef = *token;

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
        clef->SetDisPlace(STAFFREL_basic_below);
    }

    return clef;
}

//////////////////////////////
//
// HumdrumInput::getBooleanParameter --
//

bool HumdrumInput::getBooleanParameter(hum::HTp token, const string &category, const string &key)
{
    int lcount = token->getLinkedParameterCount();
    for (int i = 0; i < lcount; ++i) {
        hum::HumParamSet *hps = token->getLinkedParameter(i);
        if (hps == NULL) {
            continue;
        }
        if (hps->getNamespace1() != "LO") {
            continue;
        }
        if (hps->getNamespace2() != category) {
            continue;
        }
        string pkey;
        // string value;
        for (int i = 0; i < hps->getCount(); ++i) {
            pkey = hps->getParameterName(i);
            // value = hps->getParameterValue(i);
            if (pkey == key) {
                return true;
            }
        }
    }
    return false;
}

//////////////////////////////
//
// HumdrumInput::getStringParameter --
//

std::string HumdrumInput::getStringParameter(hum::HTp token, const string &category, const string &key)
{
    int lcount = token->getLinkedParameterCount();
    for (int i = 0; i < lcount; ++i) {
        hum::HumParamSet *hps = token->getLinkedParameter(i);
        if (hps == NULL) {
            continue;
        }
        if (hps->getNamespace1() != "LO") {
            continue;
        }
        if (hps->getNamespace2() != category) {
            continue;
        }
        string pkey;
        string value;
        for (int i = 0; i < hps->getCount(); ++i) {
            pkey = hps->getParameterName(i);
            if (pkey == key) {
                value = hps->getParameterValue(i);
                return value;
            }
        }
    }
    return "";
}

//////////////////////////////
//
// HumdrumInput::storeBreaksec -- Look for cases where sub-beams are broken.
//

void HumdrumInput::storeBreaksec(
    std::vector<int> &beamstate, std::vector<int> &beamnum, const std::vector<hum::HTp> &layerdata, bool grace)
{

    std::vector<std::vector<int> > beamednotes;
    int bnum = 0;
    for (int i = 0; i < (int)layerdata.size(); i++) {
        if (!beamnum[i]) {
            // not in a beam
            continue;
        }
        if (!layerdata[i]->isData()) {
            // not a note or rest in the beam
            continue;
        }
        if (layerdata[i]->isNull()) {
            // shouldn't happen, but just in case.
            continue;
        }
        if ((!grace) && layerdata[i]->isGrace()) {
            // ignore grace notes in this analysis
            continue;
        }
        if (grace && !layerdata[i]->isGrace()) {
            // ignore grace notes in this analysis
            continue;
        }
        if (bnum != beamnum[i]) {
            // create a new list of notes.
            beamednotes.resize(beamednotes.size() + 1);
            bnum = beamnum[i];
        }
        beamednotes.back().push_back(i);
    }

    for (int i = 0; i < (int)beamednotes.size(); i++) {
        for (int j = 1; j < (int)beamednotes[i].size() - 1; j++) {
            int index1 = beamednotes[i][j - 1];
            int index2 = beamednotes[i][j];
            int index3 = beamednotes[i][j + 1];
            int bcount1 = beamstate[index1];
            int bcount2 = beamstate[index2];
            int bcount3 = beamstate[index3];
            if ((bcount2 < bcount1) && (bcount2 < bcount3)) {
                // mark a breaksec for the given note/chord/rest.
                layerdata[index2]->setValue("", "auto", "breaksec", to_string(bcount2));
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::analyzeLayerBeams --
//

void HumdrumInput::analyzeLayerBeams(
    std::vector<int> &beamnum, std::vector<int> &gbeamnum, const std::vector<hum::HTp> &layerdata)
{

    std::vector<int> beamstate(layerdata.size(), 0);
    std::vector<int> gbeamstate(layerdata.size(), 0); // for grace notes
    int negativeQ = 0;
    int gnegativeQ = 0;

    int i;
    for (i = 0; i < (int)beamstate.size(); ++i) {
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
    for (i = 1; i < (int)beamstate.size(); ++i) {
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
    for (i = 1; i < (int)gbeamstate.size(); ++i) {
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

    storeBreaksec(beamstate, beamnum, layerdata);
    storeBreaksec(gbeamstate, gbeamnum, layerdata, true);
}

//////////////////////////////
//
// HumdrumInput::shouldHideBeamBracket --
//

bool HumdrumInput::shouldHideBeamBracket(
    const std::vector<humaux::HumdrumBeamAndTuplet> &tgs, std::vector<hum::HTp> &layerdata, int layerindex)
{
    hum::HTp starttok = layerdata[layerindex];
    if (starttok->find("L") == std::string::npos) {
        return false;
    }
    bool beamedge = false;
    int targettup = tgs[layerindex].tupletstart;
    int i = layerindex + 1;
    hum::HTp endtok = NULL;
    while (i < (int)layerdata.size()) {
        if (tgs[i].tupletend == targettup) {
            endtok = layerdata[i];
            break;
        }
        if (tgs[i].beamstart) {
            beamedge = true;
        }
        if (tgs[i].beamend) {
            beamedge = true;
        }
        i++;
    }
    if (!endtok) {
        return false;
    }
    if (beamedge) {
        return false;
    }
    if (endtok->find("J") == std::string::npos) {
        return false;
    }
    return true;
}

//////////////////////////////
//
// HumdrumInput::insertTuplet --
//

void HumdrumInput::insertTuplet(std::vector<std::string> &elements, std::vector<void *> &pointers,
    const std::vector<humaux::HumdrumBeamAndTuplet> &tgs, std::vector<hum::HTp> layerdata, int layerindex,
    bool suppress)
{
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;

    hum::HTp token = layerdata[layerindex];
    const humaux::HumdrumBeamAndTuplet &tg = tgs[layerindex];

    Tuplet *tuplet = new Tuplet;
    setTupletLocationId(tuplet, tgs, layerdata, layerindex);
    appendElement(elements, pointers, tuplet);
    elements.push_back("tuplet");
    pointers.push_back((void *)tuplet);

    int staffindex = m_rkern[token->getTrack()];
    int placement = 0;

    int rstaffindex = (int)m_fbstaff.size() - staffindex - 1;
    if (m_fbstaff[rstaffindex]) {
        placement = -m_fbstates.at(rstaffindex);
    }

    if (hasAboveParameter(layerdata[layerindex], "TUP")) {
        placement = +1;
    }
    else if (hasBelowParameter(layerdata[layerindex], "TUP")) {
        placement = -1;
    }
    // add head and stem placement in the future.
    if (placement) {
        switch (placement) {
            case -1: tuplet->SetBracketPlace(STAFFREL_basic_below); break;
            case +1: tuplet->SetBracketPlace(STAFFREL_basic_above); break;
        }
    }
    if (ss[staffindex].verse) {
        // If the music contains lyrics, force the tuplet above the staff.
        tuplet->SetBracketPlace(STAFFREL_basic_above);
    }
    double scale = tg.numscale;
    if (scale == 0.0) {
        scale = 1.0;
    }
    if (scale < 0) {
        scale = -scale;
    }
    tuplet->SetNum(tg.num * scale);
    tuplet->SetNumbase(tg.numbase * scale);
    if (suppress) {
        tuplet->SetBracketVisible(BOOLEAN_false);
    }
    if (shouldHideBeamBracket(tgs, layerdata, layerindex)) {
        tuplet->SetBracketVisible(BOOLEAN_false);
    }
    // Brackets will be displayed automatically, so don't turn on:
    // else {
    //     if (tg.bracket) {
    //         tuplet->SetBracketVisible(BOOLEAN_true);
    //     }
    //     else {
    //         tuplet->SetBracketVisible(BOOLEAN_false);
    //     }
    // }
    if (suppress) {
        // Number is visible by default, so only hide
        // if explicitly requested:
        tuplet->SetNumVisible(BOOLEAN_false);
    }
    hum::HumNum base = tg.numbase;
    // if (!base.isPowerOfTwo()) {
    //     tuplet->SetNumFormat(tupletVis_NUMFORMAT_ratio);
    // }
    // else {
    tuplet->SetNumFormat(tupletVis_NUMFORMAT_count);
    // }
    m_tupletscaling = tg.num;
    m_tupletscaling /= tg.numbase;
}

//////////////////////////////
//
// HumdrumInput::insertBeam --
//

Beam *HumdrumInput::insertBeam(
    std::vector<string> &elements, std::vector<void *> &pointers, const humaux::HumdrumBeamAndTuplet &tg)
{
    Beam *beam = new Beam;
    appendElement(elements, pointers, beam);
    elements.push_back("beam");
    pointers.push_back((void *)beam);
    return beam;
}

//////////////////////////////
//
// HumdrumInput::insertGBeam --
//

Beam *HumdrumInput::insertGBeam(
    std::vector<string> &elements, std::vector<void *> &pointers, const humaux::HumdrumBeamAndTuplet &tg)
{
    Beam *gbeam = new Beam;
    appendElement(elements, pointers, gbeam);
    elements.push_back("gbeam");
    pointers.push_back((void *)gbeam);
    return gbeam;
}

//////////////////////////////
//
// HumdrumInput::removeBeam --
//

void HumdrumInput::removeBeam(std::vector<string> &elements, std::vector<void *> &pointers)
{
    if (elements.back() != "beam") {
        cerr << "ERROR REMOVING BEAM" << endl;
        cerr << "ELEMENT STACK:" << endl;
        for (int i = (int)elements.size() - 1; i >= 0; i--) {
            cerr << i << ":\t" << elements[i] << endl;
        }
        return;
    }
    popElementStack(elements, pointers);
}

//////////////////////////////
//
// HumdrumInput::removeGBeam --
//

void HumdrumInput::removeGBeam(std::vector<string> &elements, std::vector<void *> &pointers)
{
    if (elements.back() != "gbeam") {
        cerr << "ERROR REMOVING GBEAM" << endl;
        cerr << "ELEMENT STACK:" << endl;
        for (int i = (int)elements.size() - 1; i >= 0; i--) {
            cerr << i << ":\t" << elements[i] << endl;
        }
        return;
    }
    popElementStack(elements, pointers);
}

//////////////////////////////
//
// HumdrumInput::removeTuplet --
//

void HumdrumInput::removeTuplet(std::vector<string> &elements, std::vector<void *> &pointers)
{
    if (elements.back() != "tuplet") {
        cerr << "ERROR REMOVING TUPLET" << endl;
        cerr << "ELEMENT BACK IS " << elements.back() << endl;
        cerr << "ELEMENT STACK:" << endl;
        for (int i = (int)elements.size() - 1; i >= 0; i--) {
            cerr << i << ":\t" << elements[i] << endl;
        }
        return;
    }
    popElementStack(elements, pointers);

    // Need to fix this when nested tuplets are allowed:
    m_tupletscaling = 1;
}

//////////////////////////////
//
// HumdrumInput::prepareBeamAndTupletGroups -- Calculate beam and tuplet
//     groupings for a layer.
//

void HumdrumInput::prepareBeamAndTupletGroups(
    const std::vector<hum::HTp> &layerdata, std::vector<humaux::HumdrumBeamAndTuplet> &tg)
{
    std::vector<int> beamnum;
    std::vector<int> gbeamnum;
    analyzeLayerBeams(beamnum, gbeamnum, layerdata);

    tg.clear();

    // duritems == a list of items in the layer which have duration.
    // Grace notes, barlines, interpretations, local comments, global comments,
    // etc. are filtered out for the analysis.
    std::vector<hum::HTp> duritems;

    // indexmapping == maping from a duritem index to a layerdata index.
    std::vector<int> indexmapping;

    // indexmapping2 == mapping from a layerdata index to a duritem index,
    // with -1 meaning no mapping.
    std::vector<int> indexmapping2;

    // durbeamnum == beam numbers for durational items only.
    std::vector<int> durbeamnum;

    // Extract a list of the layer items that have duration:
    for (int i = 0; i < (int)layerdata.size(); ++i) {
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
        hum::HumNum dur = hum::Convert::recipToDuration(layerdata[i]);
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
    std::vector<bool> poweroftwo(duritems.size());
    bool hastupletQ = false;
    std::vector<hum::HumNum> dotlessdur(duritems.size());
    for (int i = 0; i < (int)duritems.size(); ++i) {
        hum::HumNum duration = hum::Convert::recipToDurationNoDots(*duritems[i]);
        dotlessdur[i] = duration / 4;
        poweroftwo[i] = duration.isPowerOfTwo();
        hastupletQ |= !poweroftwo[i];
    }

    // Count the number of beams.  The durbeamnum std::vector contains a list
    // of beam numbers starting from 1 (or 0 if a note/rest has no beam).
    int beamcount = 0;
    for (int i = 0; i < (int)durbeamnum.size(); ++i) {
        if (durbeamnum[i] > beamcount) {
            beamcount = durbeamnum[i];
        }
    }

    // beamstarts and beamends are lists of the starting and ending
    // index for beams of duration items in the layer.  The index is
    // into the durlist std::vector (list of items which posses duration).
    std::vector<int> beamstarts(beamcount, -1);
    std::vector<int> beamends(beamcount, 0);
    for (int i = 0; i < (int)durbeamnum.size(); ++i) {
        if (durbeamnum[i]) {
            if (beamstarts[durbeamnum[i] - 1] < 0) {
                beamstarts[durbeamnum[i] - 1] = i;
            }
            beamends[durbeamnum[i] - 1] = i;
        }
    }

    // beamstartboolean == starting of a beam on a particular note
    // beamendboolean == ending of a beam on a particular note
    std::vector<int> beamstartboolean(durbeamnum.size(), 0);
    std::vector<int> beamendboolean(durbeamnum.size(), 0);
    for (int i = 0; i < (int)beamstarts.size(); ++i) {
        beamstartboolean[beamstarts[i]] = i + 1;
        beamendboolean[beamends[i]] = i + 1;
    }

    // Calculate grace note beam starts and ends.
    // Presuming no clef changes, etc. found between notes in
    // a gracenote beam.  Generalize further if so.
    // gbeamstart == boolean for starting of a grace note beam
    // gbeamend == boolean ending of a grace note beam
    std::vector<int> gbeamstart(layerdata.size(), 0);
    std::vector<int> gbeamend(layerdata.size(), 0);
    std::vector<int> gstate(layerdata.size(), 0);
    for (int i = 0; i < (int)gbeamnum.size(); ++i) {
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
    for (int i = (int)gbeamnum.size() - 1; i >= 0; i--) {
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
        for (int i = 0; i < (int)layerdata.size(); ++i) {
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
    std::vector<hum::HumNum> fulldur(duritems.size());

    // dursum = a cumulative sum of the full durs, starting at 0 for
    // the first index.
    std::vector<hum::HumNum> dursum(duritems.size());

    hum::HumNum sum = 0;
    std::vector<int> twocounttop(dotlessdur.size(), 0);
    std::vector<int> twocountbot(dotlessdur.size(), 0);
    for (int i = 0; i < (int)dotlessdur.size(); ++i) {
        fulldur[i] = hum::Convert::recipToDuration(*duritems[i]);
        dursum[i] = sum;
        sum += fulldur[i];
    }

    // beamdur = a list of the durations for each beam.
    std::vector<hum::HumNum> beamdur(beamstarts.size());
    for (int i = 0; i < (int)beamdur.size(); ++i) {
        beamdur[i] = dursum[beamends[i]] - dursum[beamstarts[i]] + fulldur[beamends[i]];
    }

    // beampowdot == the number of augmentation dots on a power of two for
    // the duration of the beam.  -1 means could not be made power of two with
    // dots.
    std::vector<int> beampowdot(beamstarts.size(), -1);
    for (int i = 0; i < (int)beampowdot.size(); ++i) {
        beampowdot[i] = getDotPowerOfTwo(beamdur[i]);
    }

    std::vector<bool> binarybeams(beamstarts.size(), false);
    for (int i = 0; i < (int)binarybeams.size(); ++i) {
        if (poweroftwo[beamstarts[i]]) {
            binarybeams[i] = true;
        }
    }

    // Assume that tuplet beams which can fit into a power of two will form
    // a tuplet group.  Perhaps bias towards beampowdot being 0, and try to
    // beam groups to include non-beamed tuplets into lower powdots.
    // Should check that the factors of notes in the beam group all match...
    std::vector<int> tupletgroups(poweroftwo.size(), 0);

    // tupletbracket == boolean for if the tuplet group requires a bracket.
    // It will require a bracket if they are not all enclosed in a beam.
    std::vector<int> tupletbracket(poweroftwo.size(), -1);
    int tupletnum = 1;

    // adjusted tuplet number by tuplet group
    std::vector<int> adjustcount;

    hum::HumNum tupletdur = 0;
    int tupletcount = 0;
    int samedurtup = true;

    for (int i = 0; i < (int)beampowdot.size(); ++i) {
        if (binarybeams[i]) {
            continue;
        }
        bool ingroup = false;
        if (beampowdot[i] >= 0) {
            for (int j = beamstarts[i]; j <= beamends[i]; ++j) {

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
                if (tupletnum >= (int)adjustcount.size()) {
                    int oldsize = (int)adjustcount.size();
                    adjustcount.resize(tupletnum + 1);
                    for (int z = (int)oldsize; z < (int)adjustcount.size(); ++z) {
                        adjustcount[z] = 0;
                    }
                }
                adjustcount[tupletnum] = tupletcount;
            }
            tupletnum++;
            tupletcount = 0;
        }
    }

    int tcorrection = 0;
    for (int i = 0; i < (int)tupletgroups.size(); i++) {
        if (checkForTupletForcedBreak(duritems, i)) {
            tcorrection++;
        }
        if (tupletgroups[i]) {
            tupletgroups[i] += tcorrection;
        }
    }
    if (tcorrection) {
        // invalidate adjustcount
        for (int i = 0; i < (int)adjustcount.size(); i++) {
            adjustcount[i] = 0;
        }
    }

    std::vector<bool> beamstarts2(poweroftwo.size(), false);
    for (int i = 0; i < (int)beamstarts.size(); ++i) {
        beamstarts2[beamstarts[i]] = true;
    }

    std::vector<bool> beamends2(poweroftwo.size(), false);
    for (int i = 0; i < (int)beamends.size(); ++i) {
        beamends2[beamends[i]] = true;
    }

    // beamstate == boolean for keeping track of whether or not a beam
    // is currently active.
    // bool beamstate = false;

    // Go back and link all partial beamed tuplets and non-beamed tuplets.
    // Need to consider this case: (12,24,12.) where the (12,24) are a power-of-two duration,
    // and the (12.) is also a power of two.  Maybe also merge single-tuplets into adjacent
    // tuplets and/or have a preference for grouping at the beat level rather than sub-beat.
    hum::HumNum groupdur;
    bool hasRest = false;
    for (int i = 0; i < (int)poweroftwo.size(); ++i) {
        if ((!beamstarts.empty()) && beamstarts2[i]) {
            // beamstate = true;
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
        int j = i + 1;
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
            if (tupletgroups[j]) {
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
            for (j = i; j <= ending; ++j) {
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
            // beamstate = false;
        }
    }

    // tupletstartboolean == starting of a tuplet group
    // tupletendboolean == ending of a tuplet group
    std::vector<int> tupletstartboolean(tupletgroups.size(), 0);
    std::vector<int> tupletendboolean(tupletgroups.size(), 0);
    std::vector<bool> tstart(tupletgroups.size(), false);
    std::vector<bool> tend(tupletgroups.size(), false);
    for (int i = 0; i < (int)tupletgroups.size(); ++i) {
        if (!tupletgroups[i]) {
            continue;
        }
        if (!tstart[tupletgroups[i] - 1]) {
            tupletstartboolean[i] = tupletgroups[i];
            tstart[tupletgroups[i] - 1] = true;
        }
    }

    for (int i = (int)tupletgroups.size() - 1; i >= 0; i--) {
        if (!tupletgroups[i]) {
            continue;
        }
        if (!tend[tupletgroups[i] - 1]) {
            tupletendboolean[i] = tupletgroups[i];
            tend[tupletgroups[i] - 1] = true;
        }
    }

    std::vector<int> tuptop(tupletgroups.size(), -1);
    std::vector<int> tupbot(tupletgroups.size(), -1);
    for (int i = 0; i < (int)tupletgroups.size(); ++i) {
        if (!tupletgroups[i]) {
            continue;
        }
        hum::HumNum nextpowoftwo;
        if (dotlessdur[i] < 1) {
            nextpowoftwo = nextHigherPowerOfTwo(dotlessdur[i]);
        }
        else {
            nextpowoftwo = nextLowerPowerOfTwo((double)tuptop[i] / tupbot[i]);
        }
        hum::HumNum value = dotlessdur[i] / nextpowoftwo;
        tuptop[i] = value.getDenominator();
        tupbot[i] = value.getNumerator();
    }

    // adjust tupletgroups based on tuptop and tupbot changes
    int correction = 0;
    for (int i = 1; i < (int)tuptop.size(); i++) {
        if ((tuptop[i] == 1) && (tupbot[i] == 1)) {
            continue;
        }
        if ((tuptop[i] == -1) && (tupbot[i] == -1)) {
            continue;
        }
        if ((tuptop[i - 1] == 1) && (tupbot[i - 1] == 1)) {
            continue;
        }
        if ((tuptop[i - 1] == -1) && (tupbot[i - 1] == -1)) {
            continue;
        }

        if ((tuptop[i] != tuptop[i - 1]) || (tupbot[i] != tupbot[i - 1])) {
            if (tupletgroups[i] == tupletgroups[i - 1]) {
                correction++;
                tupletstartboolean[i] = true;
                tupletendboolean[i - 1] = true;
            }
        }
        tupletgroups[i] += correction;
    }

    for (int i = 0; i < (int)tuptop.size(); i++) {
        if (tuptop[i] < 0) {
            tuptop[i] = -tuptop[i];
        }
    }

    // tupletscale == 3 for three triplets, 6 for six sextuplets.
    // int xmin = 0;
    // int state = 0;
    // int value = 0;
    // int starti = -1;
    hum::HumNum vdur;
    hum::HumNum val2;
    std::vector<int> tupletscale(tupletstartboolean.size(), 1);

    for (int i = 0; i < (int)tupletstartboolean.size(); ++i) {
        hum::HumNum xx = groupdur / 4 / dotlessdur[i] / tuptop[i];
        tupletscale[i] = xx.getNumerator();
    }

    tg.resize(layerdata.size());
    for (int i = 0; i < (int)layerdata.size(); ++i) {
        if (indexmapping2[i] < 0) {
            tg[i].group = -1;
            tg[i].bracket = -1;
            tg[i].num = -1;
            tg[i].numbase = -1;
            tg[i].numscale = 1;
            tg[i].beamstart = 0;
            tg[i].beamend = 0;
            tg[i].gbeamstart = gbeamstart[i];
            tg[i].gbeamend = gbeamend[i];
            tg[i].tupletstart = 0;
            tg[i].tupletend = 0;
            tg[i].priority = ' ';
        }
        else {
            tg[i].group = tupletgroups.at(indexmapping2[i]);
            tg[i].bracket = tupletbracket.at(indexmapping2[i]);
            tg[i].num = tuptop.at(indexmapping2[i]);
            tg[i].numbase = tupbot.at(indexmapping2[i]);
            tg[i].beamstart = beamstartboolean.at(indexmapping2[i]);
            tg[i].beamend = beamendboolean.at(indexmapping2[i]);
            tg[i].gbeamstart = gbeamstart.at(i);
            tg[i].gbeamend = gbeamend.at(i);
            tg[i].tupletstart = tupletstartboolean.at(indexmapping2[i]);
            tg[i].tupletend = tupletendboolean.at(indexmapping2[i]);
            tg[i].numscale = 1; // initialize numscale
            if (tg[i].group > 0) {
                tg[i].numscale = tupletscale.at(indexmapping2[i]);
                if (tg[i].numscale == 0) {
                    tg[i].numscale = 1;
                }
                else if (tg[i].numscale < 0) {
                    tg[i].numscale *= -1;
                }
            }
            else {
                tg[i].numscale = 1;
                if (tg[i].numscale == 0) {
                    tg[i].numscale = 1;
                }
                else if (tg[i].numscale < 0) {
                    tg[i].numscale *= -1;
                }
            }
            if (tg[i].group > 0) {
                if (tg[i].group < (int)adjustcount.size()) {
                    if (adjustcount[tg[i].group] % tg[i].num == 0) {
                        tg[i].numscale = adjustcount[tg[i].group] / tg[i].num;
                    }
                    if (tg[i].numscale == 0) {
                        tg[i].numscale = 1;
                    }
                    else if (tg[i].numscale < 0) {
                        tg[i].numscale *= -1;
                    }
                }
            }
        }
    }

    // Renumber tuplet groups in sequence (otherwise the mergeTupletCuttingBeam()
    // function will delete the 1st group if it is not the first tuplet.
    int tcounter = 0;
    for (int i = 0; i < (int)tg.size(); i++) {
        if (tg[i].tupletstart) {
            tg[i].tupletstart = ++tcounter;
        }
        else if (tg[i].tupletend) {
            tg[i].tupletend = tcounter;
        }
    }

    mergeTupletsCuttingBeam(tg);
    resolveTupletBeamTie(tg);
}

//////////////////////////////
//
// HumdrumInput::checkForTupletForcedBreak --
//

bool HumdrumInput::checkForTupletForcedBreak(const std::vector<hum::HTp> &duritems, int index)
{
    if (index == 0) {
        return false;
    }
    if (index > (int)duritems.size()) {
        return false;
    }

    hum::HTp starttok = duritems[index];
    hum::HTp endtok = duritems[index - 1];
    int stopline = endtok->getLineIndex();
    int curline = starttok->getLineIndex();
    hum::HTp cur = starttok->getPreviousToken();
    while (cur && (curline > stopline)) {
        if (cur->isInterpretation() && (*cur == "*tupbreak")) {
            return true;
        }
        cur = cur->getPreviousToken();
        curline = cur->getLineIndex();
        if (cur == endtok) {
            break;
        }
    }
    return false;
}

//////////////////////////////
//
// HumdrumInput::mergeTupletCuttingBeam -- When a tuplet ends on a beamed note,
//     but it can be continued with another tuplet of the same type, then merge the
//     two tuplets.  These cases are caused by groupings needed at a higher level
//     than the beat according to the time signature.
//

void HumdrumInput::mergeTupletsCuttingBeam(std::vector<humaux::HumdrumBeamAndTuplet> &tg)
{
    vector<humaux::HumdrumBeamAndTuplet *> newtg;
    for (int i = 0; i < (int)tg.size(); i++) {
        if (tg[i].group >= 0) {
            newtg.push_back(&tg[i]);
        }
    }

    std::vector<int> inbeam(newtg.size(), 0);
    for (int i = 0; i < (int)inbeam.size(); i++) {
        if (newtg[i]->beamstart) {
            inbeam[i] = newtg[i]->beamstart;
        }
        else if (newtg[i]->beamend) {
            inbeam[i] = 0;
        }
        else if (i > 0) {
            inbeam[i] = inbeam[i - 1];
        }
        else {
            inbeam[i] = 0;
        }
    }

    std::vector<int> scaleadj(newtg.size(), 1);
    for (int i = 0; i < (int)newtg.size(); i++) {
        if (!(inbeam[i] && newtg[i]->tupletend)) {
            continue;
        }
        if (i >= (int)newtg.size() - 1) {
            continue;
        }
        if (!newtg[i + 1]->tupletstart) {
            continue;
        }
        if (newtg[i]->num != newtg[i + 1]->num) {
            continue;
        }
        if (newtg[i]->numbase != newtg[i + 1]->numbase) {
            continue;
        }
        // Need to merge adjacent tuplets (only can merge one pair at a time
        // properly with scaleadj at the moment).
        int target = newtg[i]->tupletend;
        for (int j = i; j >= 0; j--) {
            if (!newtg[j]->tupletstart) {
                scaleadj[j] = 2;
                continue;
            }
            if (target == newtg[j]->tupletstart) {
                scaleadj[j] = 2;
                break;
            }
            cerr << "SOMETHING STANGE HAPPENED HERE" << endl;
        }
        target = newtg[i + 1]->tupletstart;
        scaleadj[i] = 2;
        scaleadj[i + 1] = 2;
        for (int j = i + 1; j < (int)newtg.size(); j++) {
            if (newtg[j]->group < 0) {
                continue;
            }
            if (!newtg[j]->tupletend) {
                scaleadj[j] = 2;
                continue;
            }
            if (target == newtg[j]->tupletend) {
                scaleadj[j] = 2;
                break;
            }
            cerr << "SOMETHING STANGE HAPPENED HERE2" << endl;
        }

        newtg[i]->tupletend = 0;
        newtg[i + 1]->tupletstart = 0;
        for (int j = i + 2; j < (int)newtg.size(); j++) {
            if (newtg[j]->tupletstart) {
                newtg[j]->tupletstart--;
            }
            if (newtg[j]->tupletend) {
                newtg[j]->tupletend--;
            }
        }
    }

    if (m_debug) {
        cerr << "INDEX\tBEAM\tTSTART\tTEND\tNUM\tNUMBASE\n";
        for (int i = 0; i < (int)tg.size(); i++) {
            cerr << "I " << i << ":\t" << inbeam[i] << "\t" << tg[i].tupletstart << "\t" << tg[i].tupletend << "\t"
                 << tg[i].num << "\t" << tg[i].numbase << "\tSA=" << scaleadj[i] << endl;
        }
    }

    for (int i = 0; i < (int)tg.size(); i++) {
        if (tg[i].group < 0) {
            continue;
        }
        tg[i].numscale *= scaleadj[i];
    }
}

//////////////////////////////
//
// HumdrumInput::resolveTupletBeamTie -- When a tuplet and beam both start or
// end
//   on the same note, figure out which one should be first, last.
//

void HumdrumInput::resolveTupletBeamTie(std::vector<humaux::HumdrumBeamAndTuplet> &tg)
{
    for (int i = 0; i < (int)tg.size(); ++i) {
        if (tg[i].beamstart && tg[i].tupletstart) {
            resolveTupletBeamStartTie(tg, i);
        }
        else if (tg[i].beamend && tg[i].tupletend) {
            resolveTupletBeamEndTie(tg, i);
        }
        else {
            tg[i].priority = ' ';
        }
    }
}

//////////////////////////////
//
// HumdrumInput::resolveTupletBeamStartTie -- When a tuplet and a beam start
//    on the same note, determine which one should be opened first (the one
//    which is contained by the other will be opened last, and if there is a
//    tie,
//    the tuplet will be opened first.
//

void HumdrumInput::resolveTupletBeamStartTie(std::vector<humaux::HumdrumBeamAndTuplet> &tg, int index)
{

    // presumably tumnum and beamnum are non-zero...
    int tupnum = tg[index].tupletstart;
    int beamnum = tg[index].beamstart;
    for (int i = index; i < (int)tg.size(); ++i) {
        if (tg[i].beamend == beamnum) {
            // beam ends before tuplet does, so tuplet should be give priority.
            tg[index].priority = 'T'; // open tuplet first
            return;
        }
        if (tg[i].tupletend == tupnum) {
            // tuplet ends before beam does, so beam should be opened first.
            tg[index].priority = 'B'; // open beam first
            return;
        }
    }
    // strange problem
    tg[index].priority = ' ';
}

//////////////////////////////
//
// HumdrumInput::resolveTupletBeamEndTie -- When a tuplet and a beam end
//    on the same note, determine which one should be closed first (the one
//    which is contained by the other will be closed first, and if there is a
//    tie,
//    the tuplet will be closed last.
//

void HumdrumInput::resolveTupletBeamEndTie(std::vector<humaux::HumdrumBeamAndTuplet> &tg, int index)
{

    // presumably tumnum and beamnum are non-zero...
    int tupnum = tg[index].tupletend;
    int beamnum = tg[index].beamend;
    for (int i = index; i >= 0; i--) {
        if (tg[i].beamstart == beamnum) {
            // beam starts after tuplet does, so beam should be closed first.
            tg[index].priority = 'B'; // close beam first
            return;
        }
        if (tg[i].tupletstart == tupnum) {
            // tuplet starts after beam does, so tuplet should be closed first.
            tg[index].priority = 'T'; // close tuplet first
            return;
        }
    }
    // strange problem
    tg[index].priority = ' ';
}

//////////////////////////////
//
// HumdrumInput::nextHigherPowerOfTwo -- Use with values between 0 and 1.
//

hum::HumNum HumdrumInput::nextHigherPowerOfTwo(hum::HumNum x)
{
    double value = log(x.getFloat()) / log(2.0);
    value = -value;
    int denom = int(value);
    hum::HumNum rval = 1;
    rval /= (int)pow(2.0, denom);
    return rval;
}

//////////////////////////////
//
// HumdrumInput::nextLowerPowerOfTwo -- For integers above 1.
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

int HumdrumInput::getDotPowerOfTwo(hum::HumNum value)
{
    if (value.isPowerOfTwo()) {
        return 0;
    }
    // check for one dot
    hum::HumNum tval = value * 2 / 3;
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

hum::HumNum HumdrumInput::removeFactorsOfTwo(hum::HumNum value, int &tcount, int &bcount)
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
    hum::HumNum output(top, bot);
    return output;
}

//////////////////////////////
//
// HumdrumInput::handleStaffStateVariables -- Deal with simple boolean switches
//     that are turned on/off by interpretation tokens.
//
// Controls that this function deals with:
//    *Xtuplet     = suppress beam and bracket tuplet numbers
//    *tuplet      = display beam and bracket tuplet numbers
//    *Xtremolo    = terminal *tremelo contraction
//    *tremolo     = merge possible beam groups into tremolos
//    *Xbeamtup    = suppress beam tuplet numbers
//    *beamtup     = display beam tuplet numbers
//    *Xbrackettup = suppress tuplet brackets
//    *brackettup  = display tuplet brackets
//    *Xcue        = notes back to regular size (operates at layer level rather than staff level).
//    *cue         = display notes in cue size (operates at layer level rather than staff level).
//    *kcancel     = display cancellation key signatures
//    *Xkcancel    = do not display cancellation key signatures (default)
//    *2\right     = place stems on right side of half notes when stem is down.
//    *2\left      = place stems on left side of half notes when stem is down.
//    *stem:       = automatic assignment of stems if there are no stems on the note already.
//       *stem:X   = no automatic assignment
//       *stem:x   = no stem
//       *stem:/   = no stem up
//       *stem:\   = no stem down
//

void HumdrumInput::handleStaffStateVariables(hum::HTp token)
{
    int staffindex = m_currentstaff - 1;
    int layernum = m_currentlayer;
    std::string value = *token;
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
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
    else if (value.compare(0, 7, "*tuplet") == 0) {
        ss[staffindex].suppress_beam_tuplet = false;
        ss[staffindex].suppress_bracket_tuplet = false;
    }

    if (value == "*Xtremolo") {
        ss[staffindex].tremolo = false;
    }
    else if (value == "*tremolo") {
        ss[staffindex].tremolo = true;
        m_hasTremolo = true;
    }

    if (value == "*Xcue") {
        ss[staffindex].cue_size.at(layernum) = false;
    }
    else if (value == "*cue") {
        ss[staffindex].cue_size.at(layernum) = true;
    }

    else if (value.substr(0, 5) == "*stem") {
        storeStemInterpretation(value, staffindex, m_currentlayer);
    }

    else if (value.find("acclev") != string::npos) {
        storeAcclev(value, staffindex);
    }

    else if (value == "*2\\left") {
        ss[staffindex].righthalfstem = false;
    }
    else if (value == "*2\\right") {
        ss[staffindex].righthalfstem = true;
    }

    // Key cancellation option is currently global to all staves:
    if (value == "*Xkcancel") {
        m_show_cautionary_keysig = false;
    }
    else if (value == "*kcancel") {
        m_show_cautionary_keysig = true;
    }
}

//////////////////////////////
//
// HumdrumInput::storeStemInterpretation --
//

void HumdrumInput::storeStemInterpretation(const std::string &value, int staffindex, int layernumber)
{
    if (value.find("stem") == string::npos) {
        return;
    }

    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;

    string ending = value.substr(6);
    if (ending == "x") {
        ss[staffindex].stem_type.at(layernumber) = 'x';
    }
    else if (ending == "/") {
        ss[staffindex].stem_type.at(layernumber) = '/';
    }
    else if (ending == "\\") {
        ss[staffindex].stem_type.at(layernumber) = '\\';
    }
    else {
        ss[staffindex].stem_type.at(layernumber) = 'X';
    }
}

//////////////////////////////
//
// HumdrumInput::storeAcclev -- Used for **mens accidental conversion to @accid+@edit or @accid.ges.
//

void HumdrumInput::storeAcclev(const std::string value, int staffindex)
{
    if (value.find("acclev") == string::npos) {
        return;
    }

    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;

    if ((value.size() > 8) && (value.substr(0, 8) == "*acclev:")) {
        string state = value.substr(8);
        if (!state.empty()) {
            if (isdigit(state[0])) {
                ss[staffindex].acclev = state[0] - '0';
            }
            else if (state == "YY") {
                ss[staffindex].acclev = 1;
            }
            else if (state == "Y") {
                ss[staffindex].acclev = 2;
            }
            else if (state == "yy") {
                ss[staffindex].acclev = 3;
            }
            else if (state == "y") {
                ss[staffindex].acclev = 4;
            }
        }
    }
    else if (value == "*acclev:") {
        ss[staffindex].acclev = 0;
    }
    else if (value == "*acclev") {
        ss[staffindex].acclev = 0;
    }
    else if (value == "*Xacclev") {
        ss[staffindex].acclev = 0;
    }
}

//////////////////////////////
//
// HumdrumInput::handleStaffDynamStateVariables -- Deal with simple boolean switches
//     that are turned on/off by interpretation tokens in **dynam spines.
//
// NB: need to set to part level rather than staff level?
//
// Controls that this function deals with:
//    *above   = Force all dynamics above staff.
//    *above:2 = Force all dynamics above staff below top one
//    *below   = Force all dynamics below the staff.
//    *below:2 = Force all dynamics below staff below top one
//    *center  = Force all dynamics to be centered between this staff and the one below.
//

void HumdrumInput::handleStaffDynamStateVariables(hum::HTp token)
{
    int staffindex = m_currentstaff - 1;
    std::string value = *token;
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;

    hum::HTp tok = token->getNextFieldToken();
    while ((tok != NULL) && (!tok->isKern())) {
        if (!tok->isDataType("**dynam")) {
            tok = tok->getNextFieldToken();
            continue;
        }
        if (*tok == "*above") {
            ss[staffindex].m_dynampos = +1;
        }
        else if (*tok == "*above:2") {
            ss[staffindex].m_dynampos = +2;
        }
        else if (*tok == "*below:2") {
            ss[staffindex].m_dynampos = -2;
        }
        else if (*tok == "*below") {
            ss[staffindex].m_dynampos = -1;
        }
        else if (*tok == "*centered") {
            ss[staffindex].m_dynampos = 0;
        }
        tok = tok->getNextFieldToken();
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

void HumdrumInput::handleOttavaMark(hum::HTp token, Note *note)
{
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    int staffindex = m_currentstaff - 1;

    if (*token == "*8va") {
        // turn on ottava
        ss[staffindex].ottavameasure = m_measure;
        ss[staffindex].ottavanotestart = NULL;
        ss[staffindex].ottavanoteend = NULL;
        ss[staffindex].ottavaendtimestamp = token->getDurationFromStart();
        // When a new note is read, check if ottavameasure
        // is non-null, and if so, store the new note in ottavanotestart.
    }
    else if (*token == "*8ba") {
        // turn on ottava down
        ss[staffindex].ottavadownmeasure = m_measure;
        ss[staffindex].ottavadownnotestart = NULL;
        ss[staffindex].ottavadownnoteend = NULL;
        ss[staffindex].ottavadownendtimestamp = token->getDurationFromStart();
        // When a new note is read, check if ottavadownmeasure
        // is non-null, and if so, store the new note in ottavadownnotestart.
    }
    else if (*token == "*15ba") {
        // turn on two ottava down
        ss[staffindex].ottava2downmeasure = m_measure;
        ss[staffindex].ottava2downnotestart = NULL;
        ss[staffindex].ottava2downnoteend = NULL;
        ss[staffindex].ottava2downendtimestamp = token->getDurationFromStart();
        // When a new note is read, check if ottava2downmeasure
        // is non-null, and if so, store the new note in ottava2downnotestart.
    }
    else if (*token == "*15ma") {
        // turn on ottava
        ss[staffindex].ottava2measure = m_measure;
        ss[staffindex].ottava2notestart = NULL;
        ss[staffindex].ottava2noteend = NULL;
        ss[staffindex].ottava2endtimestamp = token->getDurationFromStart();
        // When a new note is read, check if ottava2measure
        // is non-null, and if so, store the new note in ottava2notestart.
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
            std::string endid = getEndIdForOttava(token);
            if (endid != "") {
                octave->SetEndid("#" + endid);
            }
            else {
                octave->SetEndid("#" + ss[staffindex].ottavanoteend->GetUuid());
            }
            octave->SetDisPlace(STAFFREL_basic_above);
        }
        ss[staffindex].ottavanotestart = NULL;
        ss[staffindex].ottavanoteend = NULL;
        ss[staffindex].ottavameasure = NULL;
        ss[staffindex].ottavaendtimestamp = 0;
    }
    else if (*token == "*X8ba") {
        // turn off ottava down
        if ((ss[staffindex].ottavadownmeasure != NULL) && (ss[staffindex].ottavadownnotestart != NULL)
            && (ss[staffindex].ottavadownnoteend != NULL)) {
            Octave *octave = new Octave;
            ss[staffindex].ottavadownmeasure->AddChild(octave);
            setStaff(octave, staffindex + 1);
            octave->SetDis(OCTAVE_DIS_8);
            octave->SetStartid("#" + ss[staffindex].ottavadownnotestart->GetUuid());
            std::string endid = getEndIdForOttava(token);
            if (endid != "") {
                octave->SetEndid("#" + endid);
            }
            else {
                octave->SetEndid("#" + ss[staffindex].ottavadownnoteend->GetUuid());
            }
            octave->SetDisPlace(STAFFREL_basic_below);
        }
        ss[staffindex].ottavadownnotestart = NULL;
        ss[staffindex].ottavadownnoteend = NULL;
        ss[staffindex].ottavadownmeasure = NULL;
        ss[staffindex].ottavadownendtimestamp = 0;
    }
    else if (*token == "*X15ma") {
        // turn off ottava2 up
        if ((ss[staffindex].ottava2measure != NULL) && (ss[staffindex].ottava2notestart != NULL)
            && (ss[staffindex].ottava2noteend != NULL)) {
            Octave *octave = new Octave;
            ss[staffindex].ottava2measure->AddChild(octave);
            setStaff(octave, staffindex + 1);
            octave->SetDis(OCTAVE_DIS_15);
            octave->SetStartid("#" + ss[staffindex].ottava2notestart->GetUuid());
            octave->SetEndid("#" + ss[staffindex].ottava2noteend->GetUuid());
            std::string endid = getEndIdForOttava(token);
            if (endid != "") {
                octave->SetEndid("#" + endid);
            }
            else {
                octave->SetEndid("#" + ss[staffindex].ottava2noteend->GetUuid());
            }
            octave->SetDisPlace(STAFFREL_basic_above);
        }
        ss[staffindex].ottava2notestart = NULL;
        ss[staffindex].ottava2noteend = NULL;
        ss[staffindex].ottava2measure = NULL;
        ss[staffindex].ottava2endtimestamp = 0;
    }
    else if (*token == "*X15ba") {
        // turn off ottava2 down
        if ((ss[staffindex].ottava2downmeasure != NULL) && (ss[staffindex].ottava2downnotestart != NULL)
            && (ss[staffindex].ottava2downnoteend != NULL)) {
            Octave *octave = new Octave;
            ss[staffindex].ottava2downmeasure->AddChild(octave);
            setStaff(octave, staffindex + 1);
            octave->SetDis(OCTAVE_DIS_15);
            octave->SetStartid("#" + ss[staffindex].ottava2downnotestart->GetUuid());
            std::string endid = getEndIdForOttava(token);
            if (endid != "") {
                octave->SetEndid("#" + endid);
            }
            else {
                octave->SetEndid("#" + ss[staffindex].ottava2downnoteend->GetUuid());
            }
            octave->SetDisPlace(STAFFREL_basic_below);
        }
        ss[staffindex].ottava2downnotestart = NULL;
        ss[staffindex].ottava2downnoteend = NULL;
        ss[staffindex].ottava2downmeasure = NULL;
        ss[staffindex].ottava2downendtimestamp = 0;
    }
}

//////////////////////////////
//
// HumdrumInput::getEndIdForOttava -- Find the last note before an ottava
//     line end across multiple layers.  If there is a tie between the
//     durations of the notes in different layers, choose the note from
//     a lower layer.  Do this by searching backwards in the token's strand
//     to find the first data token, then search accross all subspines
//     for the latest note.  It is assumed that the ottava marking is
//     placed in the position of the first layer (this is similar to
//     the assumption for clef changes).
//

std::string HumdrumInput::getEndIdForOttava(hum::HTp token)
{
    hum::HTp tok = token->getPreviousToken();
    while (tok && !tok->isData()) {
        tok = tok->getPreviousToken();
    }
    if (!tok) {
        // couldn't find a previous data line
        return "";
    }
    int track = tok->getTrack();
    int ttrack = track;
    std::vector<hum::HTp> notes;
    std::vector<hum::HumNum> timestamps;
    // for now counting rests as notes.
    while (ttrack == track) {
        hum::HTp xtok = tok;
        if (xtok->isNull()) {
            xtok = xtok->resolveNull();
        }
        if (!xtok) {
            tok = tok->getNextFieldToken();
            if (!tok) {
                break;
            }
            ttrack = tok->getTrack();
            continue;
        }
        notes.push_back(xtok);
        hum::HumNum timestamp = xtok->getDurationFromStart();
        timestamps.push_back(timestamp);
        tok = tok->getNextFieldToken();
        if (!tok) {
            break;
        }
        ttrack = tok->getTrack();
    }

    if (notes.empty()) {
        return "";
    }

    int bestindex = 0;
    for (int i = 1; i < (int)notes.size(); i++) {
        if (timestamps[i] > timestamps[bestindex]) {
            bestindex = i;
        }
    }

    hum::HTp target = notes[bestindex];
    if (!target) {
        return "";
    }

    std::string prefix = "note";
    if (target->isRest()) {
        if (target->find("yy") != std::string::npos) {
            prefix = "space";
        }
        else {
            prefix = "rest";
        }
    }
    else if (target->isChord()) {
        prefix = "chord";
    }

    return getLocationId(prefix, target);
}

//////////////////////////////
//
// HumdrumInput::handlePedalMark --  *ped turns on and *Xped tuns off.
//    IF the *X8va is (incorrectly) placed at the start of the next measure
//    before a note, then this algorithm may not work (need to keep track
//    of the last note in the previous measure among all layers).
//

void HumdrumInput::handlePedalMark(hum::HTp token)
{
    int staffindex = m_currentstaff - 1;

    if (*token == "*ped") {
        // turn on pedal
        Pedal *pedal = new Pedal;
        m_measure->AddChild(pedal);
        hum::HumNum tstamp = getMeasureTstamp(token, staffindex);
        pedal->SetTstamp(tstamp.getFloat());
        pedal->SetDir(pedalLog_DIR_down);
        assignVerticalGroup(pedal, token);
        setStaff(pedal, m_currentstaff);
    }
    else if (*token == "*Xped") {
        // turn off pedal
        // hum::HTp pdata = getPreviousDataToken(token);
        // if (pdata != NULL) {
        Pedal *pedal = new Pedal;
        m_measure->AddChild(pedal);
        hum::HumNum tstamp = getMeasureTstamp(token, staffindex, hum::HumNum(1, 1));
        pedal->SetTstamp(tstamp.getFloat());
        pedal->SetDir(pedalLog_DIR_up);
        assignVerticalGroup(pedal, token);
        setStaff(pedal, m_currentstaff);
        // }
    }
}

//////////////////////////////
//
// HumdrumInput::assignVerticalGroup -- Check for a layout parameter that sets the vertical
//    group for the pedal mark.  Use "!LO:PED:vg=none" to explicitly prevent a value
//    to be stored.  Use "!LO:PED:vg=default" for using the default group (which is currently
//    set to 200). "!LO:PED:vg=0" is equivalent to "vg=default", and using a negative integer
//    (or any other string) is equivalent to "vg=none".  In the future, there will be a
//    tandem interpretation (probably called *vgp, for vertical group pedal) that will control
//    the default vertical groupping (currenly hard-wired to the default (200) group if
//    a layout parameter is not given to alter it).
//

void HumdrumInput::assignVerticalGroup(Pedal *pedal, hum::HTp token)
{
    std::string vg = token->getLayoutParameter("PED", "vg");
    if (vg.empty()) {
        // use the default group
        pedal->SetVgrp(VGRP_PEDAL_DEFAULT);
    }
    else if (std::isdigit(vg[0])) {
        int value = stoi(vg);
        if (value < 0) {
            // do not assign a vertical group
        }
        else if (value == 0) {
            // use the default group
            pedal->SetVgrp(VGRP_PEDAL_DEFAULT);
        }
        else {
            // use the specified group
            pedal->SetVgrp(value);
        }
    }
    else if (vg == "default") {
        // use the default group
        pedal->SetVgrp(VGRP_PEDAL_DEFAULT);
    }
    else {
        // do not store a vertical group parameter for this pedal marking
    }
}

//////////////////////////////
//
// HumdrumInput::getPreviousDataToken --
//

hum::HTp HumdrumInput::getPreviousDataToken(hum::HTp token)
{
    if (token == NULL) {
        return NULL;
    }
    hum::HTp current = token;
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
// HumdrumInput::getChordNoteCount -- Count the number of notes in a chord,
//    removing invalid null chord notes and rests.
//

int HumdrumInput::getChordNoteCount(hum::HTp token)
{
    int scount = token->getSubtokenCount();
    bool isnote = false;
    int count = 0;
    std::string tstring;
    for (int i = 0; i < scount; ++i) {
        isnote = false;
        tstring = token->getSubtoken(i);
        if (tstring == "") {
            continue;
        }
        for (int k = 0; k < (int)tstring.size(); ++k) {
            if ((tstring[k] >= 'a') && (tstring[k] <= 'g')) {
                isnote = true;
                break;
            }
            else if ((tstring[k] >= 'A') && (tstring[k] <= 'G')) {
                isnote = true;
                break;
            }
        }
        if (isnote) {
            count++;
        }
    }
    return count;
}

//////////////////////////////
//
// HumdrumInput::convertChord --
//

void HumdrumInput::convertChord(Chord *chord, hum::HTp token, int staffindex)
{
    int scount = token->getSubtokenCount();
    int staffadj = getStaffAdjustment(token);
    if (staffadj != 0) {
        int staffnum = staffindex + 1 + staffadj;
        setStaff(chord, staffnum);
    }
    int k;
    bool isnote = false;
    bool isrest = false;
    bool isrecip = false;
    bool allinvis = true;

    std::vector<std::string> tstrings = token->getSubtokens();
    for (int i = 0; i < (int)tstrings.size(); i++) {
        if (tstrings[i].find("yy") == std::string::npos) {
            allinvis = false;
            break;
        }
    }

    for (int j = 0; j < scount; ++j) {
        isnote = false;
        isrest = false;
        isrecip = false;
        std::string tstring = tstrings[j];
        if (tstring == "") {
            continue;
        }
        for (k = 0; k < (int)tstring.size(); ++k) {
            if (tstring[k] == 'r') {
                isrest = true;
            }
            else if ((tstring[k] >= 'a') && (tstring[k] <= 'g')) {
                isnote = true;
            }
            else if ((tstring[k] >= 'A') && (tstring[k] <= 'G')) {
                isnote = true;
            }
            else if ((tstring[k] >= '0') && (tstring[k] <= '9')) {
                isrecip = true;
            }
        }
        if (!(isnote || isrest || isrecip)) {
            continue;
        }

        if (isrest) {
            // <rest> not allowed in <chord>
            // (but chords are allowed in <rest>s somehow...)
            continue;
        }

        if (isrecip && !isnote) {
            // <space> not allowed in <chord>
            // (but chords are allowed in <space>es somehow...)
            continue;
        }

        // Suppress conversion of invisible notes.  This is becuase
        // verovio keeps the the stem visible for the invisible notes
        // in a chord.  Maybe allow when stems portions are also
        // invisible.
        if ((!allinvis) && (tstrings[j].find("yy") != std::string::npos)) {
            continue;
        }

        Note *note = new Note;
        setLocationId(note, token, j);
        appendElement(chord, note);
        convertNote(note, token, staffadj, staffindex, j);
    }

    if (allinvis) {
        chord->SetVisible(BOOLEAN_false);
    }

    // grace notes need to be done before rhythm since default
    // duration is set to an eighth note.
    if (token->find("qq") != string::npos) {
        chord->SetGrace(GRACE_acc);
        // set the visual duration to an eighth note if there
        // is no rhythm specified (will be overwritten later
        // if there is a rhythm).
        chord->SetDur(DURATION_8);
    }
    else if (token->find("q") != string::npos) {
        chord->SetGrace(GRACE_unacc);
        // set the visual duration to an eighth note if there
        // is no rhythm specified (will be overwritten later
        // if there is a rhythm).
        chord->SetDur(DURATION_8);
    }

    hum::HumNum dur;
    if (m_hasTremolo && token->getValueBool("auto", "tremolo")) {
        hum::HumdrumToken newtok(token->getValue("auto", "recip"));
        dur = convertRhythm(chord, &newtok, 0);
    }
    else if (m_hasTremolo && token->getValueBool("auto", "tremolo2")) {
        hum::HumdrumToken newtok(token->getValue("auto", "recip"));
        dur = convertRhythm(chord, &newtok, 0);
    }
    else if (m_hasTremolo && token->getValueBool("auto", "tremoloAux")) {
        hum::HumdrumToken newtok(token->getValue("auto", "recip"));
        dur = convertRhythm(chord, &newtok, 0);
    }
    else {
        dur = convertRhythm(chord, token);
    }

    if (m_setrightstem) {
        m_setrightstem = false;
        chord->SetStemPos(STEMPOSITION_right);
    }

    // Stem direction of the chord.  If both up and down, then show up.
    int crossdir = token->getValueInt("auto", "stem.dir");
    if (crossdir == 1) {
        chord->SetStemDir(STEMDIRECTION_up);
    }
    else if (crossdir == -1) {
        chord->SetStemDir(STEMDIRECTION_down);
    }
    // Overwrite cross-stem direction if there is an explicit stem direction.
    if (token->find("/") != string::npos) {
        chord->SetStemDir(STEMDIRECTION_up);
    }
    else if (token->find("\\") != string::npos) {
        chord->SetStemDir(STEMDIRECTION_down);
    }

    checkForAutoStem(chord, token);

    token->setValue("MEI", "xml:id", chord->GetUuid());
    int index = (int)m_measures.size() - 1;
    token->setValue("MEI", "measureIndex", index);

    // Add beam break information:
    int breaksec = token->getValueInt("", "auto", "breaksec");
    if (breaksec) {
        chord->SetBreaksec(breaksec);
    }
}

//////////////////////////////
//
// HumdrumInput::getTimingInformation -- Calculate the start time and duration
//     of each event so that partial layers can be filled in with <space>
//     elements if necessary.

void HumdrumInput::getTimingInformation(std::vector<hum::HumNum> &prespace, std::vector<hum::HTp> &layerdata,
    hum::HumNum layerstarttime, hum::HumNum layerendtime)
{
    prespace.resize(layerdata.size());
    if (prespace.size() > 0) {
        prespace[0] = 0;
    }
    std::vector<hum::HumNum> startdur(layerdata.size());
    std::vector<hum::HumNum> duration(layerdata.size());
    hum::HumNum correction = 0;
    for (int i = 0; i < (int)layerdata.size(); ++i) {
        startdur[i] = layerdata[i]->getDurationFromStart();
        if (!layerdata[i]->isData()) {
            duration[i] = 0;
        }
        else if (layerdata[i]->isNull()) {
            duration[i] = 0;
        }
        else {
            duration[i] = layerdata[i]->getDuration();
        }
    }

    if (layerdata.size() > 0) {
        prespace[0] = startdur[0] - layerstarttime;
    }
    for (int i = 1; i < (int)layerdata.size(); ++i) {
        prespace[i] = startdur[i] - startdur[i - 1] - duration[i - 1];
        prespace[i] -= m_duradj[layerdata[i]->getLineIndex()];
        if (prespace[i] < 0) {
            correction += prespace[i];
            prespace[i] = 0;
        }
        else if (prespace[i] > 0) {
            prespace[i] += correction;
            if (*layerdata[i] != "*") {
                correction = 0;
            }
        }
    }
    if (layerdata.size() > 0) {
        prespace.resize(prespace.size() + 1);
        prespace.back() = layerendtime - startdur.back() - duration.back();
    }

    // See https://github.com/humdrum-tools/verovio-humdrum-viewer/issues/124
    // This solution may need to be changed for a more general solution.
    for (int i = 0; i < (int)prespace.size() - 1; i++) {
        if (prespace[i] == 0) {
            continue;
        }
        if ((prespace[i] + prespace[i + 1]) == 0) {
            prespace[i] = 0;
            prespace[i + 1] = 0;
        }
    }
}

//////////////////////////////
//
// HumdrumInput::hasFullMeasureRest -- Returns true if a single
//     rest in the measure which has the same duration as the
//     time signature.
//

bool HumdrumInput::hasFullMeasureRest(std::vector<hum::HTp> &layerdata, hum::HumNum timesigdur, hum::HumNum measuredur)
{
    if (timesigdur != measuredur) {
        return false;
    }
    int datacount = 0;
    for (int i = 0; i < (int)layerdata.size(); ++i) {
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
        // Don't convert full-measure rests into spaces since
        // due to cases such as 5/4 measure rests.  Use @visible="false"
        // instead.
        // if (layerdata[i]->find("yy") != string::npos) {
        //    // treat invisible full-measure rest as a space later.
        //    return false;
        //}
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
void HumdrumInput::appendElement(const std::vector<string> &name, const std::vector<void *> &pointers, CHILD child)
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
    else if (name.back() == "ligature") {
        appendElement((Ligature *)pointers.back(), child);
    }
    else {
        std::cerr << "WARNING: Cannot append to unknown element: " << name.back() << std::endl;
    }
}

/////////////////////////////
//
// HumdrumInput::convertMRest --
//

void HumdrumInput::convertMRest(MRest *rest, hum::HTp token, int subtoken, int staffindex)
{

    string oloc = token->getValue("auto", "oloc");
    string ploc = token->getValue("auto", "ploc");
    int ottava = token->getValueInt("auto", "ottava");

    if ((!oloc.empty()) && (!ploc.empty())) {
        int olocint = stoi(oloc);
        olocint -= ottava;
        rest->SetOloc(olocint);
        if (ploc == "C") {
            rest->SetPloc(PITCHNAME_c);
        }
        else if (ploc == "D") {
            rest->SetPloc(PITCHNAME_d);
        }
        else if (ploc == "E") {
            rest->SetPloc(PITCHNAME_e);
        }
        else if (ploc == "F") {
            rest->SetPloc(PITCHNAME_f);
        }
        else if (ploc == "G") {
            rest->SetPloc(PITCHNAME_g);
        }
        else if (ploc == "A") {
            rest->SetPloc(PITCHNAME_a);
        }
        else if (ploc == "B") {
            rest->SetPloc(PITCHNAME_b);
        }
    }

    std::string tstring;
    if (subtoken < 0) {
        tstring = *token;
    }
    else {
        tstring = token->getSubtoken(subtoken);
    }

    int layer = m_currentlayer;

    if (tstring.find(";") != string::npos) {
        if ((tstring.find("yy") == string::npos) && (tstring.find(";y") == string::npos)) {
            int direction = getDirection(tstring, ";");
            if (direction < 0) {
                rest->SetFermata(STAFFREL_basic_below);
            }
            else if (direction > 0) {
                rest->SetFermata(STAFFREL_basic_above);
            }
            else if (layer == 1) {
                rest->SetFermata(STAFFREL_basic_above);
            }
            else if (layer == 2) {
                rest->SetFermata(STAFFREL_basic_below);
            }
            else {
                // who knows, maybe check the stem direction or see
                // if another note/rest in a different layer already
                // has a fermata (so you would not want to overwrite them).
                rest->SetFermata(STAFFREL_basic_above);
            }
        }
    }

    processDynamics(token, staffindex);
    setLocationId(rest, token);
    if (m_doc->GetOptions()->m_humType.GetValue()) {
        embedQstampInClass(rest, token, *token);
    }

    if (token->find("yy") != string::npos) {
        rest->SetVisible(BOOLEAN_false);
    }
}

/////////////////////////////
//
// HumdrumInput::convertRest --
//

void HumdrumInput::convertRest(Rest *rest, hum::HTp token, int subtoken)
{

    // Shouldn't be in a chord, so add rest duration here.
    // Also full-measure rests are handled elsewhere.
    convertRhythm(rest, token, subtoken);

    string oloc = token->getValue("auto", "oloc");
    string ploc = token->getValue("auto", "ploc");
    int ottava = token->getValueInt("auto", "ottava");

    if ((!oloc.empty()) && (!ploc.empty())) {
        int olocint = stoi(oloc);
        olocint -= ottava;
        rest->SetOloc(olocint);
        if (ploc == "C") {
            rest->SetPloc(PITCHNAME_c);
        }
        else if (ploc == "D") {
            rest->SetPloc(PITCHNAME_d);
        }
        else if (ploc == "E") {
            rest->SetPloc(PITCHNAME_e);
        }
        else if (ploc == "F") {
            rest->SetPloc(PITCHNAME_f);
        }
        else if (ploc == "G") {
            rest->SetPloc(PITCHNAME_g);
        }
        else if (ploc == "A") {
            rest->SetPloc(PITCHNAME_a);
        }
        else if (ploc == "B") {
            rest->SetPloc(PITCHNAME_b);
        }
    }

    std::string tstring;
    if (subtoken < 0) {
        tstring = *token;
    }
    else {
        tstring = token->getSubtoken(subtoken);
    }

    int layer = m_currentlayer;

    if (m_signifiers.above) {
        std::string pattern = "[ra-gA-G]+[-#nxXyY\\/]*";
        pattern.push_back(m_signifiers.above);
        if (regex_search(tstring, regex(pattern))) {
            int newstaff = m_currentstaff - 1;
            if ((newstaff > 0) && (newstaff <= (int)m_staffstarts.size())) {
                setStaff(rest, newstaff);
            }
        }
    }
    if (m_signifiers.below) {
        std::string pattern = "[ra-gA-G]+[-#nxXyY\\/]*";
        pattern.push_back(m_signifiers.below);
        if (regex_search(tstring, regex(pattern))) {
            int newstaff = m_currentstaff + 1;
            if ((newstaff > 0) && (newstaff <= (int)m_staffstarts.size())) {
                setStaff(rest, newstaff);
            }
        }
    }

    if (tstring.find(";") != string::npos) {
        if ((tstring.find("yy") == string::npos) && (tstring.find(";y") == string::npos)) {
            int direction = getDirection(tstring, ";");
            if (direction < 0) {
                rest->SetFermata(STAFFREL_basic_below);
            }
            else if (direction > 0) {
                rest->SetFermata(STAFFREL_basic_above);
            }
            else if (layer == 1) {
                rest->SetFermata(STAFFREL_basic_above);
            }
            else if (layer == 2) {
                rest->SetFermata(STAFFREL_basic_below);
            }
            else {
                // who knows, maybe check the stem direction or see
                // if another note/rest in a different layer already
                // has a fermata (so you would not want to overwrite them).
                rest->SetFermata(STAFFREL_basic_above);
            }
        }
    }

    if (m_doc->GetOptions()->m_humType.GetValue()) {
        embedQstampInClass(rest, token, tstring);
    }

    // If the rest is the start or stop of an analytic phrase,
    // then color the rest (may change later, or be done with a label).
    bool phraseStart = token->find('{') != string::npos ? true : false;
    bool phraseStop = token->find('}') != string::npos ? true : false;
    if (phraseStart && phraseStop) {
        rest->SetType("phraseStop phraseStart");
    }
    else if (phraseStart) {
        rest->SetType("phraseStart");
    }
    else if (phraseStop) {
        rest->SetType("phraseStop");
    }

    token->setValue("MEI", "xml:id", rest->GetUuid());
    int index = (int)m_measures.size() - 1;
    token->setValue("MEI", "measureIndex", index);
}

//////////////////////////////
//
// HumdrumInput::checkForAutoStem -- For notes and chords (and possibly Rests).
//

template <class ELEMENT> void HumdrumInput::checkForAutoStem(ELEMENT element, hum::HTp token)
{
    int stemdir = token->getValueInt("auto", "stem.dir");
    if (stemdir == 1) {
        element->SetStemDir(STEMDIRECTION_up);
    }
    else if (stemdir == -1) {
        element->SetStemDir(STEMDIRECTION_down);
    }
}

//////////////////////////////
//
// HumdrumInput::setStemLength --
//

void HumdrumInput::setStemLength(Note *note, hum::HTp token)
{
    string stemlen = token->getValue("auto", "stemlen");
    if (stemlen.empty()) {
        return;
    }
    double value = std::stof(stemlen);
    note->SetStemLen(value);
}

/////////////////////////////
//
// HumdrumInput::convertNote --
//    default value:
//       subtoken = -1 (use the first subtoken);
//

void HumdrumInput::convertNote(Note *note, hum::HTp token, int staffadj, int staffindex, int subtoken)
{
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;

    std::string tstring;
    int stindex = 0;
    if (subtoken < 0) {
        tstring = *token;
    }
    else {
        tstring = token->getSubtoken(subtoken);
        stindex = subtoken;
    }

    bool chordQ = token->isChord();

    if (!chordQ) {
        setStemLength(note, token);
    }

    // Add beam break information:
    if (!token->isChord()) {
        int breaksec = token->getValueInt("", "auto", "breaksec");
        if (breaksec) {
            note->SetBreaksec(breaksec);
        }
    }

    processTerminalLong(token); // do this before assigning rhythmic value.

    int line = token->getLineIndex();
    int field = token->getFieldIndex();
    colorNote(note, token, tstring, line, field);
    if (m_doc->GetOptions()->m_humType.GetValue()) {
        embedQstampInClass(note, token, tstring);
        embedPitchInformationInClass(note, tstring);
        embedTieInformation(note, tstring);
    }

    int ottava = token->getValueInt("auto", "ottava");
    bool octaveupQ = ottava == +1 ? true : false;
    bool octavedownQ = ottava == -1 ? true : false;
    bool octave2upQ = ottava == +2 ? true : false;
    bool octave2downQ = ottava == -2 ? true : false;

    if ((ss[staffindex].ottavameasure != NULL) && (ss[staffindex].ottavanotestart == NULL)) {
        ss[staffindex].ottavanotestart = note;
    }
    if ((ss[staffindex].ottavadownmeasure != NULL) && (ss[staffindex].ottavadownnotestart == NULL)) {
        ss[staffindex].ottavadownnotestart = note;
    }
    if ((ss[staffindex].ottava2measure != NULL) && (ss[staffindex].ottava2notestart == NULL)) {
        ss[staffindex].ottava2notestart = note;
    }
    if ((ss[staffindex].ottava2downmeasure != NULL) && (ss[staffindex].ottava2downnotestart == NULL)) {
        ss[staffindex].ottava2downnotestart = note;
    }

    hum::HumNum timestamp = token->getDurationFromStart();

    if (ss[staffindex].ottavanoteend == NULL) {
        ss[staffindex].ottavanoteend = note;
    }
    else if (timestamp > ss[staffindex].ottavaendtimestamp) {
        ss[staffindex].ottavanoteend = note;
    }

    if (timestamp > ss[staffindex].ottavadownendtimestamp) {
        ss[staffindex].ottavadownnoteend = note;
    }
    if (timestamp > ss[staffindex].ottava2endtimestamp) {
        ss[staffindex].ottava2noteend = note;
    }
    if (timestamp > ss[staffindex].ottava2downendtimestamp) {
        ss[staffindex].ottava2downnoteend = note;
    }

    if (!chordQ) {
        if (tstring.find("qq") != string::npos) {
            note->SetGrace(GRACE_acc);
            // set the visual duration to an eighth note if there
            // is no rhythm specified (will be overwritten later
            // if there is a rhythm).
            note->SetDur(DURATION_8);
        }
        else if (tstring.find("q") != string::npos) {
            note->SetGrace(GRACE_unacc);
            // set the visual duration to an eighth note if there
            // is no rhythm specified (will be overwritten later
            // if there is a rhythm).
            note->SetDur(DURATION_8);
        }
    }

    // Add the pitch information
    int base40 = hum::Convert::kernToBase40(tstring);
    base40 += m_transpose[staffindex];
    int diatonic = hum::Convert::base40ToDiatonic(base40);
    int octave = diatonic / 7;

    int testaccid = 0;
    for (int i = 0; i < (int)tstring.size(); ++i) {
        if (tstring[i] == '-') {
            testaccid--;
        }
        else if (tstring[i] == '#') {
            testaccid++;
        }
    }

    if ((testaccid > 2) || (testaccid < -2)) {
        // reconsider notes that cannot be represented in base-40
        diatonic = -1;
        string dia;
        for (int i = 0; i < (int)tstring.size(); ++i) {
            switch (tstring[i]) {
                case 'c':
                case 'C':
                    diatonic = 0;
                    dia += tstring[i];
                    break;
                case 'd':
                case 'D':
                    diatonic = 1;
                    dia += tstring[i];
                    break;
                case 'e':
                case 'E':
                    diatonic = 2;
                    dia += tstring[i];
                    break;
                case 'f':
                case 'F':
                    diatonic = 3;
                    dia += tstring[i];
                    break;
                case 'g':
                case 'G':
                    diatonic = 4;
                    dia += tstring[i];
                    break;
                case 'a':
                case 'A':
                    diatonic = 5;
                    dia += tstring[i];
                    break;
                case 'b':
                case 'B':
                    diatonic = 6;
                    dia += tstring[i];
                    break;
            }
        }
        octave = (int)dia.size();
        if (octave > 0) {
            if (std::islower(dia[0])) {
                octave = octave + 3;
            }
            else {
                octave = 4 - octave;
            }
        }
    }

    if (octaveupQ) {
        // @oct required even if @oct.ges given.
        note->SetOctGes(octave);
        note->SetOct(octave - 1);
    }
    else if (octavedownQ) {
        // @oct required even if @oct.ges given.
        note->SetOctGes(octave);
        note->SetOct(octave + 1);
    }
    else if (octave2upQ) {
        // @oct required even if @oct.ges given.
        note->SetOctGes(octave);
        note->SetOct(octave - 2);
    }
    else if (octave2downQ) {
        // @oct required even if @oct.ges given.
        note->SetOctGes(octave);
        note->SetOct(octave + 2);
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

    bool cautionaryQ = false;
    bool editorialQ = false;
    string edittype;
    if (!m_signifiers.editacc.empty()) {
        for (int x = 0; x < (int)m_signifiers.editacc.size(); ++x) {
            if (tstring.find(m_signifiers.editacc[x]) != string::npos) {
                editorialQ = true;
                edittype = m_signifiers.edittype[x];
                break;
            }
        }
    }

    bool mensit = false;
    bool gesturalQ = false;
    bool hasAccidental = false;
    int accidlevel = 0;
    if (m_mens && token->isMens()) {
        // mensural notes are indicated differently, so check here for their method.
        if ((tstring.find("n") != string::npos) || (tstring.find("-") != string::npos)
            || (tstring.find("#") != string::npos)) {
            hasAccidental = true;
        }

        mensit = true;
        if (tstring.find("YY") != string::npos) {
            accidlevel = 1;
        }
        else if (tstring.find("Y") != string::npos) {
            accidlevel = 2;
        }
        else if (tstring.find("yy") != string::npos) {
            accidlevel = 3;
        }
        else if (tstring.find("y") != string::npos) {
            accidlevel = 4;
        }
        if (accidlevel <= ss[staffindex].acclev) {
            gesturalQ = false;
        }
        else {
            gesturalQ = true;
        }
    }

    int accidCount = hum::Convert::base40ToAccidental(base40);
    if ((testaccid > 2) || (testaccid < -2)) {
        accidCount = testaccid;
    }
    // int accidCount = hum::Convert::kernToAccidentalCount(tstring);
    bool showInAccid = token->hasVisibleAccidental(stindex);
    bool showInAccidGes = !showInAccid;
    string loaccid = token->getLayoutParameter("N", "acc", subtoken);
    if (!loaccid.empty()) {
        // show the performance accidental in @accid.ges, and the
        // loaccid will be shown in @accid (the following false
        // will be used to set disable normal visual accid display system)
        showInAccid = false;
        showInAccidGes = true;
    }

    if (mensit && hasAccidental) {
        Accid *accid = new Accid;
        appendElement(note, accid);
        setLocationId(accid, token, subtoken);

        if (gesturalQ) {
            switch (accidCount) {
                case +2: accid->SetAccidGes(ACCIDENTAL_GESTURAL_ss); break;
                case +1: accid->SetAccidGes(ACCIDENTAL_GESTURAL_s); break;
                case 0: accid->SetAccidGes(ACCIDENTAL_GESTURAL_n); break;
                case -1: accid->SetAccidGes(ACCIDENTAL_GESTURAL_f); break;
                case -2: accid->SetAccidGes(ACCIDENTAL_GESTURAL_ff); break;
            }
        }
        else {
            switch (accidCount) {
                case +3: accid->SetAccid(ACCIDENTAL_WRITTEN_xs); break;
                case +2: accid->SetAccid(ACCIDENTAL_WRITTEN_x); break;
                case +1: accid->SetAccid(ACCIDENTAL_WRITTEN_s); break;
                case 0:
                    if (tstring.find("n") != string::npos) {
                        accid->SetAccid(ACCIDENTAL_WRITTEN_n);
                    }
                    break;
                case -1: accid->SetAccid(ACCIDENTAL_WRITTEN_f); break;
                case -2: accid->SetAccid(ACCIDENTAL_WRITTEN_ff); break;
                case -3: accid->SetAccid(ACCIDENTAL_WRITTEN_tf); break;
                default: std::cerr << "Do not know how to convert accidental: " << accidCount << endl;
            }
            if (accidlevel != 0) {
                accid->SetFunc(accidLog_FUNC_edit);
            }
        }
    }
    else if (!mensit) {
        Accid *accid = new Accid;
        appendElement(note, accid);
        setLocationId(accid, token, subtoken);

        if (!editorialQ) {
            // don't mark cautionary accidentals if the note has
            // an editorial accidental.
            if (token->hasCautionaryAccidental(stindex)) {
                addCautionaryAccidental(accid, token, accidCount);
                cautionaryQ = true;
                showInAccidGes = false;
                showInAccid = false;
            }
        }

        if (!editorialQ) {
            if (showInAccid) {
                switch (accidCount) {
                    case +3: accid->SetAccid(ACCIDENTAL_WRITTEN_xs); break;
                    case +2: accid->SetAccid(ACCIDENTAL_WRITTEN_x); break;
                    case +1: accid->SetAccid(ACCIDENTAL_WRITTEN_s); break;
                    case 0: accid->SetAccid(ACCIDENTAL_WRITTEN_n); break;
                    case -1: accid->SetAccid(ACCIDENTAL_WRITTEN_f); break;
                    case -2: accid->SetAccid(ACCIDENTAL_WRITTEN_ff); break;
                    case -3: accid->SetAccid(ACCIDENTAL_WRITTEN_tf); break;
                    default: std::cerr << "Do not know how to convert accidental: " << accidCount << endl;
                }
            }
            else if (!loaccid.empty()) {
                if (loaccid == "n#") {
                    accid->SetAccid(ACCIDENTAL_WRITTEN_ns);
                }
                else if (loaccid == "#") {
                    accid->SetAccid(ACCIDENTAL_WRITTEN_s);
                }
                else if (loaccid == "n") {
                    accid->SetAccid(ACCIDENTAL_WRITTEN_n);
                }
                else if (loaccid == "##") {
                    accid->SetAccid(ACCIDENTAL_WRITTEN_ss);
                }
                else if (loaccid == "x") {
                    accid->SetAccid(ACCIDENTAL_WRITTEN_x);
                }
                else if (loaccid == "-") {
                    accid->SetAccid(ACCIDENTAL_WRITTEN_f);
                }
                else if (loaccid == "--") {
                    accid->SetAccid(ACCIDENTAL_WRITTEN_ff);
                }
                else if (loaccid == "#x") {
                    accid->SetAccid(ACCIDENTAL_WRITTEN_sx);
                }
                else if (loaccid == "###") {
                    accid->SetAccid(ACCIDENTAL_WRITTEN_ts);
                }
                else if (loaccid == "n-") {
                    accid->SetAccid(ACCIDENTAL_WRITTEN_nf);
                }
                else if (loaccid == "---") {
                    accid->SetAccid(ACCIDENTAL_WRITTEN_tf);
                }
                else {
                    std::cerr << "Warning: unknown accidental type " << std::endl;
                }
                // add more accidentals here as necessary.  Mostly left are quarter tones
                // which are not dealt with directly in **kern data: su, sd, fu, fd, nu,
                // nd, 1qf, 3qf, 1qs, 3qs
                // http://music-encoding.org/guidelines/v3/data-types/data.accidental.explicit.html
            }
        }
        else {
            if (edittype == "") {
                accid->SetFunc(accidLog_FUNC_edit);
            }
            else if (edittype == "brack") {
                // enclose="brack" cannot be present with func="edit" at the moment...
                accid->SetEnclose(ENCLOSURE_brack);
            }
            else if (edittype == "paren") {
                // enclose="paren" cannot be present with func="edit" at the moment...
                accid->SetEnclose(ENCLOSURE_paren);
            }
            switch (accidCount) {
                case +2: accid->SetAccid(ACCIDENTAL_WRITTEN_x); break;
                case +1: accid->SetAccid(ACCIDENTAL_WRITTEN_s); break;
                case 0: accid->SetAccid(ACCIDENTAL_WRITTEN_n); break;
                case -1: accid->SetAccid(ACCIDENTAL_WRITTEN_f); break;
                case -2: accid->SetAccid(ACCIDENTAL_WRITTEN_ff); break;
            }
        }

        if (showInAccidGes) {
            switch (accidCount) {
                // case +3: note->SetAccidGes(ACCIDENTAL_GESTURAL_ts); break;
                // case -3: note->SetAccidGes(ACCIDENTAL_GESTURAL_tf); break;
                case +2: accid->SetAccidGes(ACCIDENTAL_GESTURAL_ss); break;
                case +1: accid->SetAccidGes(ACCIDENTAL_GESTURAL_s); break;
                case 0: accid->SetAccidGes(ACCIDENTAL_GESTURAL_n); break;
                case -1: accid->SetAccidGes(ACCIDENTAL_GESTURAL_f); break;
                case -2: accid->SetAccidGes(ACCIDENTAL_GESTURAL_ff); break;
            }
        }

        if (!(editorialQ || cautionaryQ)) {
            // No need for sub-element so make them attributes of the note:
            accid->IsAttribute(true);
        }
    }

    if (!chordQ) {
        hum::HumNum dur;
        if (m_hasTremolo && token->getValueBool("auto", "tremolo")) {
            hum::HumdrumToken newtok(token->getValue("auto", "recip"));
            dur = convertRhythm(note, &newtok, 0);
        }
        else if (m_hasTremolo && token->getValueBool("auto", "tremolo2")) {
            hum::HumdrumToken newtok(token->getValue("auto", "recip"));
            dur = convertRhythm(note, &newtok, 0);
        }
        else if (m_hasTremolo && token->getValueBool("auto", "tremoloAux")) {
            hum::HumdrumToken newtok(token->getValue("auto", "recip"));
            dur = convertRhythm(note, &newtok, 0);
        }
        else {
            dur = convertRhythm(note, token, subtoken);
        }
        if (m_setrightstem) {
            m_setrightstem = false;
            note->SetStemPos(STEMPOSITION_right);
        }
        if (dur == 0) {
            note->SetDur(DURATION_4);
            note->SetStemLen(0);
            // if you want a stemless grace note, then set the
            // stemlength to zero explicitly.
        }
    }
    else {
        // deal with visual rhythms on a note that are different from the chord
        std::string chordvis = token->getVisualDurationChord();
        if (chordvis.empty()) {
            std::string notevis = token->getVisualDuration(subtoken);
            if (!notevis.empty()) {
                convertRhythm(note, token, subtoken);
            }
        }
    }

    if (!chordQ) {
        if (tstring.find("/") != string::npos) {
            note->SetStemDir(STEMDIRECTION_up);
        }
        else if (tstring.find("\\") != string::npos) {
            note->SetStemDir(STEMDIRECTION_down);
        }
        checkForAutoStem(note, token);
    }

    if (!mensit) {
        // yy means make invisible in **kern, but is used for accidental levels in **mens.
        if (tstring.find("yy") != string::npos) {
            note->SetVisible(BOOLEAN_false);
        }
    }

    // handle ties
    if ((tstring.find("[") != string::npos) || (tstring.find("_") != string::npos)) {
        processTieStart(note, token, tstring, subtoken);
    }

    if ((tstring.find("_") != string::npos) || (tstring.find("]") != string::npos)) {
        processTieEnd(note, token, tstring, subtoken);
    }

    if (m_signifiers.above) {
        std::string pattern = "[ra-gA-G]+[-#nxXyY]*";
        pattern.push_back(m_signifiers.above);
        if (regex_search(tstring, regex(pattern))) {
            int newstaff = m_currentstaff - 1;
            if ((staffadj == 0) && (newstaff > 0) && (newstaff <= (int)m_staffstarts.size())) {
                setStaff(note, newstaff);
            }
        }
    }
    if (m_signifiers.below) {
        std::string pattern = "[ra-gA-G]+[-#nxXyY]*";
        pattern.push_back(m_signifiers.below);
        if (regex_search(tstring, regex(pattern))) {
            int newstaff = m_currentstaff + 1;
            if ((staffadj == 0) && (newstaff > 0) && (newstaff <= (int)m_staffstarts.size())) {
                setStaff(note, newstaff);
            }
        }
    }

    convertVerses(note, token, subtoken);

    // maybe organize by sub-token index, but consider as chord for now
    if (!chordQ) {
        token->setValue("MEI", "xml:id", note->GetUuid());
        int index = (int)m_measures.size() - 1;
        token->setValue("MEI", "measureIndex", index);
    }

    // check for cue-size signifier:
    if (m_signifiers.cuesize && tstring.find(m_signifiers.cuesize) != string::npos) {
        note->SetCue(BOOLEAN_true);
    }
    else if (m_staffstates.at(staffindex).cue_size.at(m_currentlayer)) {
        note->SetCue(BOOLEAN_true);
    }

    // If the note is the start or stop of an analytic phrase,
    // then color the note (may change later, or be done with a label).
    bool phraseStart = token->find('{') != string::npos ? true : false;
    bool phraseStop = token->find('}') != string::npos ? true : false;
    if (phraseStart && phraseStop) {
        note->SetType("phraseStart phraseStop");
    }
    else if (phraseStart) {
        note->SetType("phraseStart");
    }
    else if (phraseStop) {
        note->SetType("phraseStop");
    }
}

//////////////////////////////
//
// HumdrumInput::addCautionaryAccidental --
//

void HumdrumInput::addCautionaryAccidental(Accid *accid, hum::HTp token, int acount)
{
    accid->SetFunc(accidLog_FUNC_caution);
    switch (acount) {
        case +3: accid->SetAccid(ACCIDENTAL_WRITTEN_ts); break;
        case +2: accid->SetAccid(ACCIDENTAL_WRITTEN_x); break;
        case +1: accid->SetAccid(ACCIDENTAL_WRITTEN_s); break;
        case 0: accid->SetAccid(ACCIDENTAL_WRITTEN_n); break;
        case -1: accid->SetAccid(ACCIDENTAL_WRITTEN_f); break;
        case -2: accid->SetAccid(ACCIDENTAL_WRITTEN_ff); break;
        case -3: accid->SetAccid(ACCIDENTAL_WRITTEN_tf); break;
    }
}

//////////////////////////////
//
// HumdrumInput::convertVerses --
//

void HumdrumInput::convertVerses(Note *note, hum::HTp token, int subtoken)
{
    int staff = m_rkern[token->getTrack()];
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    if (!ss[staff].verse) {
        return;
    }

    int subtrack = token->getSubtrack();
    if (subtrack > 1) {
        if (token->noteInLowerSubtrack()) {
            // don't print a lyric for secondary layers unless
            // all of the lower layers do not have a note attacking
            // or tied at the same time.  This is because verovio
            // will incorrectly overstrike syllables shared between
            // layers if there is an offset of a second between the layers.
            return;
            // probably also have to deal with chords containing seconds...
        }
    }

    hum::HumRegex hre;
    vector<string> vtexts;
    string vcolor;
    std::string content;
    hum::HumdrumLine &line = *token->getLine();
    int track = token->getTrack();
    int ttrack;
    int versenum = 0;
    bool vvdataQ;
    bool vdataQ;
    bool lyricQ;
    int startfield = token->getFieldIndex() + 1;
    for (int i = startfield; i < line.getFieldCount(); ++i) {
        if (line.token(i)->isKern()) {
            ttrack = line.token(i)->getTrack();
            if (ttrack != track) {
                break;
            }
        }

        lyricQ = false;
        vdataQ = false;
        vvdataQ = false;
        if (line.token(i)->isDataType("**text")) {
            lyricQ = true;
        }
        else if (line.token(i)->isDataType("**silbe")) {
            lyricQ = true;
        }
        else if (line.token(i)->getDataType().compare(0, 7, "**vdata") == 0) {
            vdataQ = true;
            lyricQ = true;
        }
        else if (line.token(i)->getDataType().compare(0, 8, "**vvdata") == 0) {
            vvdataQ = true;
            lyricQ = true;
        }

        if (!lyricQ) {
            continue;
        }

        if (line.token(i)->isNull()) {
            versenum++;
            continue;
        }
        if (line.token(i)->isDataType("**silbe")) {
            if (line.token(i)->getText() == "|") {
                versenum++;
                continue;
            }
        }

        vtexts.clear();
        vcolor.clear();
        int track = line.token(i)->getTrack();
        if (line.token(i)->isDataType("**silbe")) {
            string value = line.token(i)->getText();
            hre.replaceDestructive(value, "", "\\|", "g");
            hre.replaceDestructive(value, "&uuml;", "u2", "g");
            hre.replaceDestructive(value, "&auml;", "a2", "g");
            hre.replaceDestructive(value, "&ouml;", "o2", "g");
            vtexts.push_back(value);
            vcolor = m_spine_color[track];
        }
        else {
            vtexts.push_back(*line.token(i));
            vcolor = m_spine_color[track];
        }
        if (vvdataQ) {
            splitSyllableBySpaces(vtexts);
        }

        for (int j = 0; j < (int)vtexts.size(); ++j) {
            content = vtexts[j];
            versenum++;
            if (content == "") {
                continue;
            }

            Verse *verse = new Verse;
            if (!vcolor.empty() && (vcolor != "black") && (vcolor != "#000") && (vcolor != "#000000")) {
                verse->SetColor(vcolor);
            }
            if (vvdataQ) {
                setLocationId(verse, line.token(i), j + 1);
            }
            else {
                setLocationId(verse, line.token(i), -1);
            }
            appendElement(note, verse);
            verse->SetN(versenum);
            Syl *syl = new Syl;
            string datatype = line.token(i)->getDataType();

            if (datatype.compare(0, 8, "**vdata-") == 0) {
                string subdatatype = datatype.substr(8);
                if (!subdatatype.empty()) {
                    appendTypeTag(syl, subdatatype);
                }
            }
            else if (datatype.compare(0, 9, "**vdata-") == 0) {
                string subdatatype = datatype.substr(9);
                if (!subdatatype.empty()) {
                    appendTypeTag(syl, subdatatype);
                }
            }

            if (vvdataQ) {
                setLocationId(syl, line.token(i), j + 1);
            }
            else {
                setLocationId(syl, line.token(i), -1);
            }

            appendElement(verse, syl);

            if (vdataQ || vvdataQ) {
                addTextElement(syl, content);
                continue;
            }

            colorVerse(verse, content);

            bool dashbegin = false;
            bool dashend = false;
            bool extender = false;

            for (int z = 1; z < (int)content.size() - 1; ++z) {
                // Use underscore for elision symbol
                // (later use @con="b" when verovio allows it).
                // Also possibly make elision symbols optional.
                if ((content[z] == ' ') && (content[z + 1] != '\'')) {
                    // the later condition is to not to elide "ma 'l"
                    content[z] = '_';
                }
            }

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
                if (m_doc->GetOptions()->m_humType.GetValue()) {
                    appendTypeTag(syl, "m");
                }
            }
            else if (dashbegin) {
                syl->SetWordpos(sylLog_WORDPOS_t);
                if (m_doc->GetOptions()->m_humType.GetValue()) {
                    appendTypeTag(syl, "t");
                }
            }
            else if (dashend) {
                syl->SetWordpos(sylLog_WORDPOS_i);
                syl->SetCon(sylLog_CON_d);
                if (m_doc->GetOptions()->m_humType.GetValue()) {
                    appendTypeTag(syl, "i");
                }
            }
            else if (extender) {
                syl->SetWordpos(sylLog_WORDPOS_t);
                syl->SetCon(sylLog_CON_u);
                if (m_doc->GetOptions()->m_humType.GetValue()) {
                    appendTypeTag(syl, "t");
                }
            }
            else {
                if (m_doc->GetOptions()->m_humType.GetValue()) {
                    appendTypeTag(syl, "t");
                }
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
// HumdrumInput::splitSyllableBySpaces -- Split a string into pieces
//    according to spaces.  Default value spacer = ' ');
//

void HumdrumInput::splitSyllableBySpaces(vector<string> &vtext, char spacer)
{
    if (vtext[0].find(spacer) == string::npos) {
        return;
    }
    if (vtext.size() != 1) {
        // invalid size
        return;
    }
    string original = vtext[0];
    vtext[0] = "";
    for (int i = 0; i < (int)original.size(); ++i) {
        if (original[i] != spacer) {
            vtext.back().push_back(original[i]);
            continue;
        }
        // new string needs to be made
        vtext.push_back("");
    }
}

//////////////////////////////
//
// HumdrumInput::convertMensuralRhythm --
//

template <class ELEMENT> hum::HumNum HumdrumInput::convertMensuralRhythm(ELEMENT element, hum::HTp token, int subtoken)
{

    std::string tstring;
    if (subtoken < 0) {
        tstring = *token;
        // strip off any leading spaces
        auto first = tstring.find_first_not_of(' ');
        if (first != string::npos) {
            tstring = tstring.substr(first);
        }
    }
    else {
        tstring = token->getSubtoken(subtoken);
    }

    string vstring = token->getVisualDuration(subtoken);

    if (!vstring.empty()) {
        int dotcount = characterCountInSubtoken(vstring, '.');
        if (dotcount > 0) {
            element->SetDots(dotcount);
        }
        // dotcount = characterCountInSubtoken(tstring, '.');
        // if (dotcount > 0) {
        //    element->SetDotsGes(dotcount);
        //}
    }
    else {
        int dotcount = characterCountInSubtoken(tstring, ':');
        if (dotcount > 0) {
            element->SetDots(dotcount);
        }
    }

    // Tuplet durations are not handled below yet.
    // dur is in units of quarter notes.

    hum::HumNum dur;
    hum::HumNum durges;

    if (vstring.empty()) {
        dur = hum::Convert::mensToDurationNoDots(tstring);
        dur /= 4; // duration is now in whole note units;
        dur *= m_tupletscaling;
    }
    else {
        dur = hum::Convert::mensToDurationNoDots(vstring);
        dur /= 4; // duration is now in whole note units;
        dur *= m_tupletscaling;

        durges = hum::Convert::mensToDurationNoDots(tstring);
        durges /= 4; // duration is now in whole note units;
        durges *= m_tupletscaling;
    }

    if (dur.isInteger()) {
        switch (dur.getNumerator()) {
            case 1: element->SetDur(DURATION_semibrevis); break;
            case 2: element->SetDur(DURATION_brevis); break;
            case 4: element->SetDur(DURATION_longa); break;
            case 8: element->SetDur(DURATION_maxima); break;
        }
    }
    else if (dur.getNumerator() == 1) {
        switch (dur.getDenominator()) {
            case 2: element->SetDur(DURATION_minima); break;
            case 4: element->SetDur(DURATION_semiminima); break;
            case 8: element->SetDur(DURATION_fusa); break;
            case 16: element->SetDur(DURATION_semifusa); break;
            case 32: element->SetDur(DURATION_32); break;
            case 64: element->SetDur(DURATION_64); break;
            case 128: element->SetDur(DURATION_128); break;
            case 256: element->SetDur(DURATION_256); break;
            case 512: element->SetDur(DURATION_512); break;
            case 1024: element->SetDur(DURATION_1024); break;
            case 2048: element->SetDur(DURATION_2048); break;
        }
    }

    if (!vstring.empty()) {
        if (durges.isInteger()) {
            switch (durges.getNumerator()) {
                case 1: element->SetDurGes(DURATION_semibrevis); break;
                case 2: element->SetDurGes(DURATION_brevis); break;
                case 4: element->SetDurGes(DURATION_longa); break;
                case 8: element->SetDurGes(DURATION_maxima); break;
            }
        }
        else if (durges.getNumerator() == 1) {
            switch (durges.getDenominator()) {
                case 2: element->SetDurGes(DURATION_minima); break;
                case 4: element->SetDurGes(DURATION_semiminima); break;
                case 8: element->SetDurGes(DURATION_fusa); break;
                case 16: element->SetDurGes(DURATION_semifusa); break;
                case 32: element->SetDurGes(DURATION_32); break;
                case 64: element->SetDurGes(DURATION_64); break;
                case 128: element->SetDurGes(DURATION_128); break;
                case 256: element->SetDurGes(DURATION_256); break;
                case 512: element->SetDurGes(DURATION_512); break;
                case 1024: element->SetDurGes(DURATION_1024); break;
                case 2048: element->SetDurGes(DURATION_2048); break;
            }
        }
        return durges;
    }
    else {
        /*
                if (dur.isInteger()) {
                        switch (dur.getNumerator()) {
                        case 1: element->SetDurGes(DURATION_1); break;
                        case 2: element->SetDurGes(DURATION_breve); break;
                        case 4: element->SetDurGes(DURATION_long); break;
                        case 8: element->SetDurGes(DURATION_maxima); break;
                        }
                }
                else if (dur.getNumerator() == 1) {
                        switch (dur.getDenominator()) {
                        case 2: element->SetDurGes(DURATION_2); break;
                        case 4: element->SetDurGes(DURATION_4); break;
                        case 8: element->SetDurGes(DURATION_8); break;
                        case 16: element->SetDurGes(DURATION_16); break;
                        case 32: element->SetDurGes(DURATION_32); break;
                        case 64: element->SetDurGes(DURATION_64); break;
                        case 128: element->SetDurGes(DURATION_128); break;
                        case 256: element->SetDurGes(DURATION_256); break;
                        case 512: element->SetDurGes(DURATION_512); break;
                        case 1024: element->SetDurGes(DURATION_1024); break;
                        case 2048: element->SetDurGes(DURATION_2048); break;
                        }
                }
        */
    }

    return dur;
}

//////////////////////////////
//
// HumdrumInput::setRhythmFromDuration -- Used for splitting invisible rests across
//    clef changes, so a simple algorithm.  Currently does not allow for tuplets or
//    dotted rhythms.
//

template <class ELEMENT> void HumdrumInput::setRhythmFromDuration(ELEMENT element, hum::HumNum dur)
{
    dur /= 4; // convert to whole-note units

    if (dur.isInteger()) {
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
}

//////////////////////////////
//
// HumdrumInput::convertRhythm --
//     default value:
//         subtoken = -1;
//         isnote = true
//

template <class ELEMENT> hum::HumNum HumdrumInput::convertRhythm(ELEMENT element, hum::HTp token, int subtoken)
{
    if (token->isMens()) {
        return convertMensuralRhythm(element, token, subtoken);
    }

    std::string tstring;
    if (subtoken < 0) {
        tstring = *token;
        // strip off any leading spaces
        auto first = tstring.find_first_not_of(' ');
        if (first != string::npos) {
            tstring = tstring.substr(first);
        }
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

    std::string vstring;
    if (subtoken < 0) {
        vstring = token->getVisualDurationChord();
    }
    else {
        vstring = token->getVisualDuration(subtoken);
    }
    if (!vstring.empty()) {
        int visualdotcount = characterCountInSubtoken(vstring, '.');
        if (visualdotcount > 0) {
            element->SetDots(visualdotcount);
        }
        int gesturaldotcount = characterCountInSubtoken(tstring, '.');
        if (gesturaldotcount != visualdotcount) {
            element->SetDotsGes(gesturaldotcount);
            element->SetDots(visualdotcount);
        }
    }
    else {
        int dotcount = characterCountInSubtoken(tstring, '.');
        if (dotcount > 0) {
            element->SetDots(dotcount);
        }
    }

    // Tuplet durations are not handled below yet.
    // dur is in units of quarter notes.

    hum::HumNum dur;
    hum::HumNum durges;

    if (vstring.empty()) {
        dur = hum::Convert::recipToDurationNoDots(tstring);
        dur /= 4; // duration is now in whole note units;
        if (!grace) {
            dur *= m_tupletscaling;
        }
    }
    else {
        dur = hum::Convert::recipToDurationNoDots(vstring);
        dur /= 4; // duration is now in whole note units;
        if (!grace) {
            dur *= m_tupletscaling;
        }

        durges = hum::Convert::recipToDurationNoDots(tstring);
        durges /= 4; // duration is now in whole note units;
        if (!grace) {
            durges *= m_tupletscaling;
        }
    }

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

    if (dur.isInteger()) {
        switch (dur.getNumerator()) {
            case 1: element->SetDur(DURATION_1); break;
            case 2: element->SetDur(DURATION_breve); break;
            case 4: element->SetDur(DURATION_long); break;
            case 8: element->SetDur(DURATION_maxima); break;
        }
    }
    else if (dur.getNumerator() == 1) {
        switch (dur.getDenominator()) {
            case 2: {
                element->SetDur(DURATION_2);
                // handle stem position tandem insterpretations
                int staff = m_currentstaff;
                int staffindex = staff - 1;
                std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
                if (ss[staffindex].righthalfstem
                    && ((element->GetClassName() == "Note") || (element->GetClassName() == "Chord"))) {
                    m_setrightstem = true;
                }
            } break;
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

    if (!vstring.empty()) {
        if (durges.isInteger()) {
            switch (durges.getNumerator()) {
                case 1: element->SetDurGes(DURATION_1); break;
                case 2: element->SetDurGes(DURATION_breve); break;
                case 4: element->SetDurGes(DURATION_long); break;
                case 8: element->SetDurGes(DURATION_maxima); break;
            }
        }
        else if (durges.getNumerator() == 1) {
            switch (durges.getDenominator()) {
                case 2: element->SetDurGes(DURATION_2); break;
                case 4: element->SetDurGes(DURATION_4); break;
                case 8: element->SetDurGes(DURATION_8); break;
                case 16: element->SetDurGes(DURATION_16); break;
                case 32: element->SetDurGes(DURATION_32); break;
                case 64: element->SetDurGes(DURATION_64); break;
                case 128: element->SetDurGes(DURATION_128); break;
                case 256: element->SetDurGes(DURATION_256); break;
                case 512: element->SetDurGes(DURATION_512); break;
                case 1024: element->SetDurGes(DURATION_1024); break;
                case 2048: element->SetDurGes(DURATION_2048); break;
            }
        }
        return durges;
    }

    return dur;
}

//////////////////////////////
//
// HumdrumInput::addBreath -- Add floating breath for note/chord.
//     default value: parent = NULL
//

void HumdrumInput::addBreath(hum::HTp token, Object *parent)
{
    int layer = m_currentlayer;
    int staff = m_currentstaff;

    if (token->find(",") == std::string::npos) {
        return;
    }

    if ((token->find("yy") == std::string::npos) && (token->find(",y") == std::string::npos)) {
        Breath *breath = new Breath;
        appendElement(m_measure, breath);
        setStaff(breath, staff);

        if (parent && (token->find("q") != std::string::npos)) {
            // grace notes cannot be addressed with @tstamp, so
            // have to use @startid.  Maybe allow @tstamp, since
            // @startid will probably shift to the correct grace note
            // position.
            std::string id = "#" + parent->GetUuid();
            breath->SetStartid(id);
        }
        else if (!token->empty() && (token->at(0) == '=')) {
            // barline breath
            if (parent) {
                std::string id = "#" + parent->GetUuid();
                breath->SetStartid(id);
            }
            else {
                hum::HumNum tstamp = getMeasureEndTstamp(staff - 1);
                breath->SetTstamp(tstamp.getFloat());
            }
        }
        else {
            hum::HumNum start = getMeasureTstamp(token, staff - 1);
            hum::HumNum dur = token->getDuration() * getMeasureFactor(staff - 1);
            hum::HumNum tstamp = start + ((dur * 4) / 5);
            breath->SetTstamp(tstamp.getFloat());
        }
        setLocationId(breath, token);

        int direction = getDirection(*token, ",");
        if (direction < 0) {
            setPlace(breath, "below");
        }
        else if (direction > 0) {
            setPlace(breath, "above");
        }
        else if (layer == 1) {
            setPlace(breath, "above");
        }
        else if (layer == 2) {
            setPlace(breath, "below");
        }
    }
}

//////////////////////////////
//
// HumdrumInput::getStaffAdjustment -- +1 if all notes are staff below
//     and -1 if all notes are staff above.
//

int HumdrumInput::getStaffAdjustment(hum::HTp token)
{
    hum::HumRegex hre;
    // Check for notes in chords that are all on a different staff.  If so,
    // then move the fermata to the different staff.
    std::vector<string> subtokens;
    int scount = token->getSubtokenCount();
    for (int i = 0; i < scount; i++) {
        subtokens.emplace_back(token->getSubtoken(i));
    }
    int allabove = true;
    int allbelow = true;
    std::string upquery = "[A-Ga-gr][#n-]*[xXyY]*";
    upquery += m_signifiers.above;
    std::string downquery = "[A-Ga-gr][#n-]*[xXyY]*";
    downquery += m_signifiers.below;
    if (m_signifiers.above) {
        for (int i = 0; i < scount; i++) {
            if (!hre.search(subtokens[i], upquery)) {
                allabove = false;
                break;
            }
        }
    }
    else {
        allabove = false;
    }
    if (m_signifiers.below && !allabove) {
        for (int i = 0; i < scount; i++) {
            if (!hre.search(subtokens[i], downquery)) {
                allbelow = false;
                break;
            }
        }
    }
    else {
        allbelow = false;
    }
    int staffadj = 0;
    if (allabove) {
        staffadj = -1; // -1 means up
    }
    else if (allbelow) {
        staffadj = +1; // +1 means down
    }
    return staffadj;
}

//////////////////////////////
//
// HumdrumInput::addFermata -- Add floating fermatas for note/chord.
//     default value: parent = NULL
//

void HumdrumInput::addFermata(hum::HTp token, Object *parent)
{
    int layer = m_currentlayer;
    int staff = m_currentstaff;

    if (token->find(";") == std::string::npos) {
        return;
    }

    int staffadj = getStaffAdjustment(token);

    if ((token->find("yy") == std::string::npos) && (token->find(";y") == std::string::npos)) {
        Fermata *fermata = new Fermata;
        appendElement(m_measure, fermata);
        setStaff(fermata, staff + staffadj);

        Fermata *fermata2 = NULL;
        if (token->find(";;") != std::string::npos) {
            fermata2 = new Fermata;
            appendElement(m_measure, fermata2);
            setStaff(fermata2, staff + staffadj);
        }

        if (parent && (token->find("q") != std::string::npos)) {
            // grace notes cannot be addressed with @tstamp, so
            // have to use @startid.  Maybe allow @tstamp, since
            // @startid will probably shift to the correct grace note
            // position.
            std::string id = "#" + parent->GetUuid();
            fermata->SetStartid(id);
            if (fermata2) {
                fermata2->SetStartid(id);
            }
        }
        else if (!token->empty() && (token->at(0) == '=')) {
            // barline fermata
            if (parent) {
                std::string id = "#" + parent->GetUuid();
                fermata->SetStartid(id);
                if (fermata2) {
                    fermata2->SetStartid(id);
                }
            }
            else {
                hum::HumNum tstamp = getMeasureEndTstamp(staff - 1);
                fermata->SetTstamp(tstamp.getFloat());
                if (fermata2) {
                    fermata2->SetTstamp(tstamp.getFloat());
                }
            }
        }
        else {
            hum::HumNum tstamp = getMeasureTstamp(token, staff - 1);
            if (parent) {
                std::string id = "#" + parent->GetUuid();
                fermata->SetStartid(id);
                if (fermata2) {
                    fermata2->SetStartid(id);
                }
            }
            else {
                fermata->SetTstamp(tstamp.getFloat());
                if (fermata2) {
                    fermata2->SetTstamp(tstamp.getFloat());
                }
            }
        }
        // if a barline, then can have two fermatas
        if (fermata2) {
            setLocationIdNSuffix(fermata, token, 1);
            setLocationIdNSuffix(fermata2, token, 2);
        }
        else {
            setLocationId(fermata, token);
        }

        if (fermata2) {
            setPlace(fermata, "above");
            setPlace(fermata2, "below");
            return;
        }

        int direction = getDirection(*token, ";");
        if (direction < 0) {
            setPlace(fermata, "below");
        }
        else if (direction > 0) {
            setPlace(fermata, "above");
        }
        else if (layer == 1) {
            setPlace(fermata, "above");
        }
        else if (layer == 2) {
            setPlace(fermata, "below");
        }
    }
}

//////////////////////////////
//
// HumdrumInput::addArpeggio --
//   : = arpeggio which may cross between layers on a staff.
//   :: = arpeggio which crosses staves on a single system.
//

void HumdrumInput::addArpeggio(Object *object, hum::HTp token)
{

    bool systemQ = false;
    bool staffQ = false;
    hum::HTp earp = NULL;
    if (token->find("::") != string::npos) {
        if (!leftmostSystemArpeggio(token)) {
            return;
        }
        systemQ = true;
        earp = getRightmostSystemArpeggio(token);
        if (earp == NULL) {
            // no system arpeggio actually found
            return;
        }
    }
    else if (token->find(":") != string::npos) {
        if (!leftmostStaffArpeggio(token)) {
            return;
        }
        staffQ = true;
        earp = getRightmostStaffArpeggio(token);
        if (earp == NULL) {
            // no system arpeggio actually found
            return;
        }
    }
    else {
        return; // no arpeggio on this note/chord
    }

    // int layer = m_currentlayer;
    // int staff = m_currentstaff;

    if (systemQ || staffQ) {
        Arpeg *arpeg = new Arpeg;
        appendElement(m_measure, arpeg);
        // no staff attachment, or list both endpoint staves or all staves involved?
        setLocationId(arpeg, token);
        // arpeg->SetStartid("#" + object->GetUuid());
        string firstid = object->GetUuid();
        string secondid;
        if (earp->find(" ") != std::string::npos) {
            secondid = getLocationId("chord", earp);
        }
        else {
            secondid = getLocationId("note", earp);
        }
        // string plist = "#" + firstid + " #" + secondid;
        arpeg->AddRef("#" + firstid);
        arpeg->AddRef("#" + secondid);
    }
}

//////////////////////////////
//
// HumdrumInput::getRightmostStaffArpeggio -- Assuming a single contiguous
//     arpeggio across all layers from first to last marker.
//

hum::HTp HumdrumInput::getRightmostStaffArpeggio(hum::HTp token)
{
    hum::HTp output = NULL;
    if ((token->find(":") != std::string::npos) && (token->find("::") == std::string::npos)) {
        output = token;
    }
    int track = token->getTrack();
    token = token->getNextFieldToken();
    int ntrack;
    if (token) {
        ntrack = token->getTrack();
    }
    while (token != NULL) {
        if (ntrack != track) {
            break;
        }
        if (!token->isKern()) {
            token = token->getNextFieldToken();
            continue;
        }
        if ((token->find(":") != std::string::npos) && (token->find("::") == std::string::npos)) {
            output = token;
        }
        token = token->getNextFieldToken();
        if (token) {
            ntrack = token->getTrack();
        }
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::leftmostStaffArpeggio --
//

bool HumdrumInput::leftmostStaffArpeggio(hum::HTp token)
{
    int track = token->getTrack();
    token = token->getPreviousFieldToken();
    int ntrack;
    if (token) {
        ntrack = token->getTrack();
    }
    while (token != NULL) {
        if (track != ntrack) {
            break;
        }
        if (!token->isKern()) {
            token = token->getPreviousFieldToken();
            if (token) {
                ntrack = token->getTrack();
            }
            continue;
        }
        if (token->find(":") != std::string::npos) {
            return false;
        }
        token = token->getPreviousFieldToken();
    }
    return true;
}

//////////////////////////////
//
// HumdrumInput::getRightmostSystemArpeggio -- Assuming a single contiguous
//     arpeggio across all staves from first to last marker.  Will probably have
//     to adjust for layers (which are ordered reversed compared to staves).
//

hum::HTp HumdrumInput::getRightmostSystemArpeggio(hum::HTp token)
{
    hum::HTp output = NULL;
    if (token->find("::") != std::string::npos) {
        output = token;
    }
    token = token->getNextFieldToken();
    while (token != NULL) {
        if (!token->isKern()) {
            token = token->getNextFieldToken();
            continue;
        }
        if (token->find("::") != std::string::npos) {
            output = token;
        }
        token = token->getNextFieldToken();
    }
    return output;
}

//////////////////////////////
//
// HumdrumInput::leftmostSystemArpeggio -- Not checking for contiguous staves
//  (i.e., only one cross-staff arpeggio is allowed at a time for now).  Will
//  probably have to adjust for layers (which are ordered reverse of staves).
//

bool HumdrumInput::leftmostSystemArpeggio(hum::HTp token)
{
    token = token->getPreviousFieldToken();
    while (token != NULL) {
        if (!token->isKern()) {
            token = token->getPreviousFieldToken();
            continue;
        }
        if (token->find("::") != std::string::npos) {
            return false;
        }
        token = token->getPreviousFieldToken();
    }
    return true;
}

//////////////////////////////
//
// HumdrumInput::addOrnaments --
//   M  = mordent, major second for top interval
//   m  = mordent, minor second for top interval
//   W  = inverted mordent, major second for top interval
//   w  = inverted mordent, minor second for top interval
//   T  = trill, major second
//   t  = trill, minor second
//   TT  = trill, major second with wavy line after it
//   tt  = trill, minor second with wavy line after it
//   TTT = trill, continuing a major second with wavy line after it
//   ttt = trill, continuing a minor second with wavy line after it
//
//   S[Ss]?[Ss]? = turn
//   $[Ss]?[Ss]? = inverted turn
//
//   These are not used anymore:
//   SS = turn centered between two notes
//   $$ = inverted turn centered between two notes
//

void HumdrumInput::addOrnaments(Object *object, hum::HTp token)
{
    vector<bool> chartable(256, false);
    for (int i = 0; i < (int)token->size(); ++i) {
        chartable[token->at(i)] = true;
    }

    if (chartable['T'] || chartable['t']) {
        addTrill(token);
    }
    if (chartable[';']) {
        addFermata(token, object);
    }
    if (chartable[',']) {
        addBreath(token, object);
    }
    if (chartable['W'] || chartable['w'] || chartable['M'] || chartable['m']) {
        addMordent(object, token);
    }
    if (chartable['s'] || chartable['S'] || chartable['$']) {
        addTurn(object, token);
    }

    addOrnamentMarkers(token);
}

//////////////////////////////
//
// HumdrumInput::addTurn -- Add turn for note.
//  only one of these four possibilities:
//      S([Ss][Ss])?  = delayed turn
//      sS([Ss][Ss])? = undelayed turn
//      $([Ss][Ss])?  = delayed inverted turn
//      s$([Ss][Ss])? = undelayed inverted turn
//
//  Not used anymore:
//      SS = turn, centered between two notes
//      $$ = inverted turn, centered between two notes
//
// Assuming not in chord for now.
//

void HumdrumInput::addTurn(Object *linked, hum::HTp token)
{
    std::string &tok = *token;
    int turnstart = -1;
    int turnend = -1;

    for (int i = 0; i < (int)tok.size(); i++) {
        if ((tok[i] == 's') || (tok[i] == 'S') || (tok[i] == '$')) {
            turnstart = i;
            turnend = i;
            for (int j = i + 1; j < (int)tok.size(); j++) {
                if (!((tok[i] == 's') || (tok[i] == 'S') || (tok[i] == '$'))) {
                    break;
                }
                turnend = j;
            }
            break;
        }
    }

    std::string turnstr = tok.substr(turnstart, turnend - turnstart + 1);

    bool delayedQ = turnstr[0] == 's' ? false : true;

    if ((!delayedQ) && turnstr.size() == 1) {
        // not an invalid turn indication
        return;
    }

    bool invertedQ = false;
    if (((!delayedQ) && turnstr[1] == '$') || (turnstr[0] == '$')) {
        invertedQ = true;
    }

    // int layer = m_currentlayer; // maybe place below if in layer 2
    int staff = m_currentstaff;
    int staffindex = staff - 1;
    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;

    Turn *turn = new Turn;
    appendElement(m_measure, turn);
    setStaff(turn, staff);

    hum::HumNum tstamp = getMeasureTstamp(token, staffindex);
    if (delayedQ) {
        hum::HumNum duration = token->getDuration();
        // if (ss[staffindex].meter_bottom == 0) {
        //    duration /= 2;
        // } else {
        duration *= ss[staffindex].meter_bottom;
        // }
        duration /= 4;
        duration /= 2;
        tstamp += duration;
        turn->SetTstamp(tstamp.getFloat());
    }
    else if (!linked) {
        turn->SetTstamp(tstamp.getFloat());
    }
    else {
        turn->SetStartid("#" + linked->GetUuid());
    }

    if (invertedQ) {
        turn->SetForm(turnLog_FORM_upper);
    }
    else {
        turn->SetForm(turnLog_FORM_lower);
    }

    setLocationId(turn, token);

    if (m_signifiers.above) {
        if (turnend < (int)token->size() - 1) {
            if ((*token)[turnend + 1] == m_signifiers.above) {
                setPlace(turn, "above");
            }
        }
    }
    if (m_signifiers.below) {
        if (turnend < (int)token->size() - 1) {
            if ((*token)[turnend + 1] == m_signifiers.below) {
                setPlace(turn, "below");
            }
        }
    }

    int subtok = 0;
    int tokindex = subtok;
    if (subtok < 0) {
        tokindex = 0;
    }

    // check for lower accidental on turn
    std::string loweraccid = token->getValue("auto", to_string(tokindex), "turnLowerAccidental");
    bool hasloweraccid = loweraccid.empty() ? false : true;
    int loweraccidval = 0;
    if (hasloweraccid) {
        loweraccidval = stoi(loweraccid);
        switch (loweraccidval) {
            case -1: turn->SetAccidlower(ACCIDENTAL_WRITTEN_f); break;
            case 0: turn->SetAccidlower(ACCIDENTAL_WRITTEN_n); break;
            case +1: turn->SetAccidlower(ACCIDENTAL_WRITTEN_s); break;
            case -2: turn->SetAccidlower(ACCIDENTAL_WRITTEN_ff); break;
            case +2: turn->SetAccidlower(ACCIDENTAL_WRITTEN_x); break;
        }
    }

    // check for upper accidental on turn
    std::string upperaccid = token->getValue("auto", to_string(tokindex), "turnUpperAccidental");
    bool hasupperaccid = upperaccid.empty() ? false : true;
    int upperaccidval = 0;
    if (hasupperaccid) {
        upperaccidval = stoi(upperaccid);
        switch (upperaccidval) {
            case -1: turn->SetAccidupper(ACCIDENTAL_WRITTEN_f); break;
            case 0: turn->SetAccidupper(ACCIDENTAL_WRITTEN_n); break;
            case +1: turn->SetAccidupper(ACCIDENTAL_WRITTEN_s); break;
            case -2: turn->SetAccidupper(ACCIDENTAL_WRITTEN_ff); break;
            case +2: turn->SetAccidupper(ACCIDENTAL_WRITTEN_x); break;
        }
    }
}

//////////////////////////////
//
// HumdrumInput::addMordent -- Add mordent for note.
//      M = upper mordent, major second interval
//      m = upper mordent, minor second interval
//      W = lower mordent, major second interval
//      w = lower mordent, minor second interval
//
//

void HumdrumInput::addMordent(Object *linked, hum::HTp token)
{
    bool lowerQ = false;
    int subtok = 0;
    size_t tpos = std::string::npos;
    for (int i = 0; i < (int)token->size(); ++i) {
        char chit = token->at(i);
        if (chit == ' ') {
            subtok++;
            continue;
        }
        if (chit == 'w') {
            tpos = i;
            lowerQ = true;
            break;
        }
        else if (chit == 'm') {
            tpos = i;
            break;
        }
        else if (chit == 'W') {
            tpos = i;
            lowerQ = true;
            break;
        }
        else if (chit == 'M') {
            tpos = i;
            break;
        }
    }

    if ((subtok == 0) && token->find(" ") == std::string::npos) {
        subtok = -1;
    }

    if (tpos == std::string::npos) {
        // no mordent on note
        return;
    }

    // int layer = m_currentlayer; // maybe place below if in layer 2
    int staff = m_currentstaff;
    Mordent *mordent = new Mordent;
    appendElement(m_measure, mordent);
    setStaff(mordent, staff);
    mordent->SetStartid("#" + getLocationId("note", token, subtok));
    // if (linked) {
    //     mordent->SetStartid("#" + linked->GetUuid());
    // }
    // else {
    //     hum::HumNum tstamp = getMeasureTstamp(token, staff - 1);
    //     mordent->SetTstamp(tstamp.getFloat());
    // }
    setLocationId(mordent, token, subtok);

    if (!lowerQ) {
        // reversing for now
        // 300: mordent->SetForm(mordentLog_FORM_inv);
        mordent->SetForm(mordentLog_FORM_upper);
        // } else {
        //    mordent->SetForm(mordentLog_FORM_norm);
    }
    // also "long" form to consider

    if (m_signifiers.above) {
        if (tpos < token->size() - 1) {
            if ((*token)[tpos + 1] == m_signifiers.above) {
                setPlace(mordent, "above");
            }
        }
    }
    if (m_signifiers.below) {
        if (tpos < token->size() - 1) {
            if ((*token)[tpos + 1] == m_signifiers.below) {
                setPlace(mordent, "below");
            }
        }
    }

    int tokindex = subtok;
    if (tokindex < 0) {
        tokindex = 0;
    }

    if (std::tolower(token->at(tpos)) == 'w') {
        // lower mordent
        std::string accid = token->getValue("auto", to_string(tokindex), "mordentLowerAccidental");
        bool hasaccid = accid.empty() ? false : true;
        int accidval = 0;
        if (hasaccid) {
            accidval = stoi(accid);
            switch (accidval) {
                case -1: mordent->SetAccidlower(ACCIDENTAL_WRITTEN_f); break;
                case 0: mordent->SetAccidlower(ACCIDENTAL_WRITTEN_n); break;
                case +1: mordent->SetAccidlower(ACCIDENTAL_WRITTEN_s); break;
                case -2: mordent->SetAccidlower(ACCIDENTAL_WRITTEN_ff); break;
                case +2: mordent->SetAccidlower(ACCIDENTAL_WRITTEN_x); break;
            }
        }
    }
    else {
        // upper mordent
        std::string accid = token->getValue("auto", to_string(tokindex), "mordentUpperAccidental");
        bool hasaccid = accid.empty() ? false : true;
        int accidval = 0;
        if (hasaccid) {
            accidval = stoi(accid);
            switch (accidval) {
                case -1: mordent->SetAccidupper(ACCIDENTAL_WRITTEN_f); break;
                case 0: mordent->SetAccidupper(ACCIDENTAL_WRITTEN_n); break;
                case +1: mordent->SetAccidupper(ACCIDENTAL_WRITTEN_s); break;
                case -2: mordent->SetAccidupper(ACCIDENTAL_WRITTEN_ff); break;
                case +2: mordent->SetAccidupper(ACCIDENTAL_WRITTEN_x); break;
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::addTrill -- Add trill for note.
//

void HumdrumInput::addTrill(hum::HTp token)
{
    int subtok = 0;
    size_t tpos = std::string::npos;
    for (int i = 0; i < (int)token->size(); ++i) {
        if (token->at(i) == ' ') {
            subtok++;
            continue;
        }
        if ((token->at(i) == 't') || (token->at(i) == 'T')) {
            tpos = i;
            if (i < (int)token->size() - 1) {
                // deal with TT or tt for trills with wavy lines
                if ((token->at(i + 1) == 't') || (token->at(i + 1) == 'T')) {
                    tpos++;
                }
            }
            break;
        }
    }
    if (tpos == std::string::npos) {
        // no trill on a note
        return;
    }

    if (token->find("TTT") != std::string::npos) {
        // continuation trill, so don't start a new one
        return;
    }
    if (token->find("ttt") != std::string::npos) {
        // continuation trill, so don't start a new one
        return;
    }

    if ((subtok == 0) && token->find(" ") == std::string::npos) {
        subtok = -1;
    }

    // int layer = m_currentlayer; // maybe place below if in layer 2
    int staff = m_currentstaff;
    Trill *trill = new Trill;
    appendElement(m_measure, trill);
    setStaff(trill, staff);

    int staffindex = m_currentstaff - 1;
    int layer = m_currentlayer;

    if (layer == 2) {
        setPlace(trill, "below");
    }

    trill->SetStartid("#" + getLocationId("note", token, subtok));
    // Setting trill@tstamp:
    // hum::HumNum tstamp = getMeasureTstamp(token, staffindex);
    // trill->SetTstamp(tstamp.getFloat());

    setLocationId(trill, token, subtok);
    if (m_signifiers.above) {
        if (tpos < token->size() - 1) {
            if ((*token)[tpos + 1] == m_signifiers.above) {
                setPlace(trill, "above");
            }
        }
    }
    if (m_signifiers.below) {
        if (tpos < token->size() - 1) {
            if ((*token)[tpos + 1] == m_signifiers.below) {
                setPlace(trill, "below");
            }
        }
    }

    int tokindex = subtok;
    if (tokindex < 0) {
        tokindex = 0;
    }
    std::string accid = token->getValue("auto", to_string(tokindex), "trillAccidental");
    bool hasaccid = accid.empty() ? false : true;
    int accidval = 0;
    if (hasaccid) {
        accidval = stoi(accid);
        switch (accidval) {
            case -1: trill->SetAccidupper(ACCIDENTAL_WRITTEN_f); break;
            case 0: trill->SetAccidupper(ACCIDENTAL_WRITTEN_n); break;
            case 1: trill->SetAccidupper(ACCIDENTAL_WRITTEN_s); break;
            case -2: trill->SetAccidupper(ACCIDENTAL_WRITTEN_ff); break;
            case 2: trill->SetAccidupper(ACCIDENTAL_WRITTEN_x); break;
        }
    }

    // replace the trill accidental if different in layout parameters, such as:
    //    !LO:TR:acc=##
    // for a double sharp, or
    //    !LO:TR:acc=none
    // for no accidental
    int lcount = token->getLinkedParameterCount();
    bool foundQ = false;
    std::string value;
    for (int p = 0; p < lcount; ++p) {
        hum::HumParamSet *hps = token->getLinkedParameter(p);
        if (hps == NULL) {
            continue;
        }
        if (hps->getNamespace1() != "LO") {
            continue;
        }
        if (hps->getNamespace2() != "TR") {
            continue;
        }
        for (int q = 0; q < hps->getCount(); ++q) {
            string key = hps->getParameterName(q);
            if (key == "acc") {
                value = hps->getParameterValue(q);
                foundQ = true;
                break;
            }
        }
    }
    if (foundQ) {
        if (value == "none") {
            trill->SetAccidupper(ACCIDENTAL_WRITTEN_NONE);
        }
        else if (value == "#") {
            trill->SetAccidupper(ACCIDENTAL_WRITTEN_s);
        }
        else if (value == "-") {
            trill->SetAccidupper(ACCIDENTAL_WRITTEN_f);
        }
        else if (value == "n") {
            trill->SetAccidupper(ACCIDENTAL_WRITTEN_n);
        }
        else if (value == "--") {
            trill->SetAccidupper(ACCIDENTAL_WRITTEN_ff);
        }
        else if (value == "##") {
            trill->SetAccidupper(ACCIDENTAL_WRITTEN_x);
        }
        else {
            // other trills are possible to add here; otherwise,
            // the invalid trill will be ignored (not replaced
            // with an empty accidental, for example).
        }
    }

    if ((token->find("TT") == std::string::npos) && (token->find("tt") == std::string::npos)) {
        // no line extension needed fro the trill
        return;
    }

    // Find the ending note after the trill line.  Multiple trill line extensions for chord notes
    // are not handled by this algorithm, but these should be rare in notation.  Trills that cross
    // barlines require @tstamp2 rather than @endid to display (possibly a bug in verovio).
    hum::HTp endtok = token->getNextToken();
    hum::HTp lasttok = token;
    hum::HTp lastnote = token;
    hum::HTp lastnoteorbar = token;
    hum::HTp bartok = NULL;
    int barlinecount = 0;
    bool foundbarline = false;
    bool nextnoteafterbarline = false;
    while (endtok) {
        if (endtok->isBarline()) {
            foundbarline = true;
            nextnoteafterbarline = true;
            bartok = endtok;
            barlinecount++;
            lastnoteorbar = endtok;
        }
        if (!endtok->isData()) {
            endtok = endtok->getNextToken();
            nextnoteafterbarline = false;
            continue;
        }
        if (endtok->isNull()) {
            endtok = endtok->getNextToken();
            continue;
        }
        if (endtok->isGrace()) {
            // check to see if the next non-grace note/rest has a TTT or ttt on it.
            // if so, then do not terminate the trill extension line at this
            // grace notes.
            hum::HTp ntok = endtok->getNextToken();
            while (ntok) {
                if (ntok->isBarline()) {
                    lastnoteorbar = ntok;
                }
                if (!ntok->isData()) {
                    ntok = ntok->getNextToken();
                    continue;
                }
                if (ntok->isGrace()) {
                    ntok = ntok->getNextToken();
                    continue;
                }
                lastnoteorbar = ntok;
                lastnote = ntok;
                // at this point ntok is a durational note or rest
                if ((ntok->find("TTT") == std::string::npos) && (ntok->find("ttt") == std::string::npos)) {
                    break;
                }
                ntok = ntok->getNextToken();
            }
        }
        lastnoteorbar = endtok;
        lastnote = endtok;
        if ((endtok->find("TTT") == std::string::npos) && (endtok->find("ttt") == std::string::npos)) {
            break;
        }
        lasttok = endtok;
        endtok = endtok->getNextToken();
    }

    if ((!endtok) && lastnoteorbar && lastnoteorbar->isBarline()) {
        // reached the end of the music, so calculate the duration
        // of the trill extension line from the position of the last
        // barline.
        hum::HumNum tstamp2 = getMeasureTstampPlusDur(bartok, staffindex);
        std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
        hum::HumNum mfactor = ss[staffindex].meter_bottom / 4;
        tstamp2 += mfactor;
        int measures = getMeasureDifference(token, bartok);
        measures--;
        std::pair<int, double> ts2(measures, tstamp2.getFloat());
        trill->SetTstamp2(ts2);
        return;
    }

    if ((!endtok) && lastnoteorbar && lastnoteorbar->isData()) {
        // reached the end of the music, so calculate the duration
        // of the trill extension line from the position of the last
        // note.
        hum::HumNum tstamp2 = getMeasureTstampPlusDur(lastnote, staffindex);
        int measures = getMeasureDifference(token, bartok);
        std::pair<int, double> ts2(measures, tstamp2.getFloat());
        trill->SetTstamp2(ts2);
        return;
    }

    bool isgracenote = endtok->find('q') == std::string::npos ? false : true;
    bool isrest = endtok->isRest();

    if (isgracenote) {
        trill->SetEndid("#" + getLocationId("note", endtok, -1));
    }
    else if (isrest && !foundbarline) {
        // use @tstamp2 because @endid does not work in verovio for rests.
        hum::HumNum tstamp2 = getMeasureTstampPlusDur(lasttok, staffindex);
        int measures = getMeasureDifference(token, lasttok);
        std::pair<int, double> ts2(measures, tstamp2.getFloat());
        trill->SetTstamp2(ts2);
    }
    else if (foundbarline && (barlinecount == 1) && nextnoteafterbarline) {
        // end trill extender before barline
        hum::HumNum tstamp2 = getMeasureTstampPlusDur(bartok->getPreviousToken(), staffindex);
        int measures = 0;
        std::pair<int, double> ts2(measures, tstamp2.getFloat());
        trill->SetTstamp2(ts2);
    }
    else {
        // The line extension crosses a barline, so need to use @tstamp2;
        // otherwise verovio will not display the line extension.
        hum::HumNum tstamp2 = getMeasureTstampPlusDur(lasttok, staffindex);
        int measures = getMeasureDifference(token, lasttok);
        std::pair<int, double> ts2(measures, tstamp2.getFloat());
        trill->SetTstamp2(ts2);
    }
    // Need to add another case when extender crosses a barline and ends before
    // a grace note.  This requires @endid since @tstamp2 will not work, as that
    // will extend to the note after the grace note(s) at the same tstamp.
}

//////////////////////////////
//
// HumdrumInput::processTieStart -- linked slurs not allowed in chords yet.
//

void HumdrumInput::processTieStart(Note *note, hum::HTp token, const std::string &tstring, int subindex)
{
    std::string endtag = "tieEnd";
    if (subindex >= 0) {
        endtag += to_string(subindex + 1);
    }
    hum::HTp tieend = token->getValueHTp("auto", endtag);
    if (tieend) {
        // A linked tie which can be inserted immediately (and
        // not stored in the list of tie starts for later processing).

        std::string endnumtag = "tieEndSubtokenNumber";
        int endn = subindex + 1;
        if (token->isChord()) {
            if (endn > 0) {
                endnumtag += to_string(endn);
            }
        }
        int endnumber = token->getValueInt("auto", endnumtag);
        if (endnumber <= 0) {
            endnumber = 1;
        }

        vrv::Tie *tie = new Tie;

        addLineFormStyle(tie, token, "T");

        m_measure->AddChild(tie);
        int endsubindex = endnumber - 1;
        if (endsubindex < 0) {
            endsubindex = 0;
        }
        setTieLocationId(tie, token, subindex, tieend, endsubindex);
        std::string startid = getLocationId("note", token);
        std::string endid = getLocationId("note", tieend);
        if (token->isChord()) {
            startid += "S" + to_string(subindex + 1);
        }
        if (tieend->isChord()) {
            if (endnumber > 0) {
                endid += "S" + to_string(endnumber);
            }
        }
        tie->SetStartid("#" + startid);
        tie->SetEndid("#" + endid);

        std::string marker1 = "[";
        std::string marker2 = "[";
        std::string marker3 = "_";
        std::string marker4 = "_";

        if (m_signifiers.above) {
            marker1 += m_signifiers.above;
            marker3 += m_signifiers.above;
        }
        if (m_signifiers.below) {
            marker2 += m_signifiers.below;
            marker4 += m_signifiers.below;
        }

        if (m_signifiers.above && tstring.find(marker1) != std::string::npos) {
            tie->SetCurvedir(curvature_CURVEDIR_above);
        }
        else if (m_signifiers.below && tstring.find(marker2) != std::string::npos) {
            tie->SetCurvedir(curvature_CURVEDIR_below);
        }
        else if (m_signifiers.above && tstring.find(marker3) != std::string::npos) {
            tie->SetCurvedir(curvature_CURVEDIR_above);
        }
        else if (m_signifiers.below && tstring.find(marker4) != std::string::npos) {
            tie->SetCurvedir(curvature_CURVEDIR_below);
        }

        return;
    }

    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    hum::HumNum timestamp = token->getDurationFromStart();
    hum::HumNum endtime = timestamp + token->getDuration();
    int track = token->getTrack();
    int rtrack = m_rkern[track];
    std::string noteuuid = note->GetUuid();
    int cl = m_currentlayer;
    int pitch = hum::Convert::kernToMidiNoteNumber(tstring);

    ss[rtrack].ties.emplace_back();
    ss[rtrack].ties.back().setStart(noteuuid, m_measure, cl, tstring, pitch, timestamp, endtime, subindex, token);

    if (m_signifiers.above) {
        std::string marker = "[";
        if (tstring.find("_") != string::npos) {
            marker = "_";
        }
        marker.push_back(m_signifiers.above);
        if (tstring.find(marker) != string::npos) {
            ss[rtrack].ties.back().setTieAbove();
        }
    }
    if (m_signifiers.below) {
        std::string marker = "[";
        if (tstring.find("_") != string::npos) {
            marker = "_";
        }
        marker.push_back(m_signifiers.below);
        if (tstring.find(marker) != string::npos) {
            ss[rtrack].ties.back().setTieBelow();
        }
    }
}

//////////////////////////////
//
// processTieEnd --
//

void HumdrumInput::processTieEnd(Note *note, hum::HTp token, const std::string &tstring, int subindex)
{
    std::string starttag = "tieStart";
    if (token->isChord()) {
        starttag += to_string(subindex + 1);
    }
    hum::HTp tiestart = token->getValueHTp("auto", starttag);
    if (tiestart) {
        // linked ties are handled in processTieStart().
        return;
    }

    std::vector<humaux::StaffStateVariables> &ss = m_staffstates;
    hum::HumNum timestamp = token->getDurationFromStart();
    int track = token->getTrack();
    int staffnum = m_rkern[track];
    std::string noteuuid = note->GetUuid();
    bool disjunct = token->find("]]") != std::string::npos;
    if (token->find("__") != std::string::npos) {
        disjunct = true;
    }

    int pitch = hum::Convert::kernToMidiNoteNumber(tstring);
    int layer = m_currentlayer;
    auto found = ss[staffnum].ties.end();

    // search for open tie in current layer
    for (auto it = ss[staffnum].ties.begin(); it != ss[staffnum].ties.end(); ++it) {
        if (it->getLayer() != layer) {
            continue;
        }
        if (it->getPitch() != pitch) {
            continue;
        }
        if (disjunct && (it->getStartTokenPointer()->find("[[") != std::string::npos)) {
            found = it;
            break;
        }
        if (disjunct && (it->getStartTokenPointer()->find("__") != std::string::npos)) {
            found = it;
            break;
        }
        else if (it->getEndTime() == timestamp) {
            found = it;
            break;
        }
    }

    // search for open tie in current staff outside of current layer.
    if (found == ss[staffnum].ties.end()) {
        for (auto it = ss[staffnum].ties.begin(); it != ss[staffnum].ties.end(); ++it) {
            if (it->getPitch() != pitch) {
                continue;
            }
            if (disjunct && (it->getStartTokenPointer()->find("[[") != std::string::npos)) {
                found = it;
                break;
            }
            else if (it->getEndTime() == timestamp) {
                found = it;
                break;
            }
        }
    }

    if (found == ss[staffnum].ties.end()) {
        // can't find start of slur so give up.
        return;
    }

    Tie *tie = found->setEndAndInsert(noteuuid, m_measure, tstring);

    hum::HTp starttoken = found->getStartTokenPointer();
    if (starttoken) {
        addLineFormStyle(tie, starttoken, "T");
    }

    setTieLocationId(tie, found->getStartTokenPointer(), found->getStartSubindex(), token, subindex);

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

int HumdrumInput::characterCount(const std::string &text, char symbol)
{
    return (int)std::count(text.begin(), text.end(), symbol);
}

int HumdrumInput::characterCount(hum::HTp token, char symbol)
{
    return (int)std::count(token->begin(), token->end(), symbol);
}

/////////////////////////////
//
// HumdrumInput::characterCountInSubtoken --
//

int HumdrumInput::characterCountInSubtoken(const std::string &text, char symbol)
{
    int sum = 0;
    for (int i = 0; i < (int)text.size(); ++i) {
        if (text[i] == symbol) {
            sum++;
        }
        if (text[i] == ' ') {
            break;
        }
    }
    return sum;
}

int HumdrumInput::characterCountInSubtoken(hum::HTp token, char symbol)
{
    return characterCountInSubtoken(*token, symbol);
}

/////////////////////////////
//
// HumdrumInput::printMeasureTokens -- For debugging.
//

void HumdrumInput::printMeasureTokens()
{
    std::vector<std::vector<std::vector<hum::HTp> > > &lt = m_layertokens;
    int i, j, k;
    cerr << endl;
    for (i = 0; i < (int)lt.size(); ++i) {
        cerr << "STAFF " << i + 1 << "\t";
        for (j = 0; j < (int)lt[i].size(); ++j) {
            cerr << "LAYER " << j + 1 << ":\t";
            for (k = 0; k < (int)lt[i][j].size(); ++k) {
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

template <class ELEMENT> hum::HumNum HumdrumInput::setDuration(ELEMENT element, hum::HumNum duration)
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

std::vector<int> HumdrumInput::getStaffLayerCounts()
{
    std::vector<std::vector<std::vector<hum::HTp> > > &lt = m_layertokens;
    std::vector<int> output(lt.size(), 0);

    int i;
    for (i = 0; i < (int)lt.size(); ++i) {
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
    hum::HumdrumFile &infile = m_infiles[0];

    if (m_oclef.size() || m_omet.size()) {
        storeOriginalClefMensurationApp();
    }

    if (infile[startline].getDurationFromStart() > 0) {
        addSystemKeyTimeChange(startline, endline);
    }

    string previoussection = m_lastsection;
    string currentsection;
    if (m_sectionlabels[startline]) {
        currentsection = *m_sectionlabels[startline];
        if (currentsection.compare(0, 2, "*>") == 0) {
            currentsection = currentsection.substr(2);
        }
        currentsection = "label-" + currentsection;
    }
    else {
        currentsection = "";
    }

    m_measure = new Measure();

    int endnum = 0;
    bool ending = false;
    bool newsection = false;
    if (isdigit(currentsection.back())) {
        ending = true;
        std::smatch matches;
        if (regex_search(currentsection, matches, regex("(\\d+)$"))) {
            endnum = stoi(matches[1]);
        }
        else {
            endnum = 0;
        }
    }
    else if (currentsection != m_lastsection) {
        newsection = true;
        if (m_lastsection != currentsection) {
            if (m_sections.size() > 1) {
                // keep movement-level section in stack.
                m_sections.pop_back();
            }
        }
        if (m_sectionlabels[startline]) {
            m_lastsection = currentsection;
        }
        else {
            m_lastsection = "";
        }
    }

    if (ending && (m_endingnum != endnum)) {
        // create a new ending
        m_currentending = new Ending;
        std::string endingid = *m_sectionlabels[startline];
        if (endingid.compare(0, 2, "*>") == 0) {
            endingid = endingid.substr(2);
        }
        endingid = "label-" + endingid;
        setN(m_currentending, endnum, m_sectionlabels[startline]);
        // sanitize id if not valid:
        m_currentending->SetUuid(endingid);
        if (m_sections.size() > 1) {
            // assuming the ending does not start at beginning
            // of music.
            m_sections.pop_back();
        }
        m_sections.back()->AddChild(m_currentending);
        m_currentending->AddChild(m_measure);
    }
    else if (isdigit(currentsection.back())) {
        // inside a current ending
        m_currentending->AddChild(m_measure);
    }
    else if (newsection) {
        // start a new section
        m_currentsection = new Section;
        m_currentsection->AddChild(m_measure);
        m_currentsection->SetUuid(m_lastsection);
        m_sections.back()->AddChild(m_currentsection);
        m_sections.push_back(m_currentsection);
    }
    else {
        // outside of an ending
        m_sections.back()->AddChild(m_measure);
    }
    m_endingnum = endnum;
    m_measures.push_back(m_measure);

    if (m_leftbarstyle != BARRENDITION_NONE) {
        m_measure->SetLeft(m_leftbarstyle);
        m_leftbarstyle = BARRENDITION_NONE;
    }

    setLocationId(m_measure, startline, -1, -1);

    int measurenumber = getMeasureNumber(startline, endline);
    if (measurenumber >= 0) {
        setN(m_measure, measurenumber);
    }

    if (m_doc->GetOptions()->m_humType.GetValue()) {
        stringstream measuretag;
        measuretag << "m-" << measurenumber;
        appendTypeTag(m_measure, measuretag.str());
    }

    setSystemMeasureStyle(startline, endline);
}

//////////////////////////////
//
// HumdrumInput::storeOriginalClefMensurationApp -- If there are any original
// clefs or mensuration signs, create an app for them.
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

void HumdrumInput::storeOriginalClefMensurationApp()
{
    if (m_oclef.empty() && m_omet.empty()) {
        return;
    }

    int kerncount = 0;
    int menscount = 0;
    for (int i = 0; i < (int)m_oclef.size(); i++) {
        if (m_oclef[i].second->isKern()) {
            kerncount++;
        }
        else if (m_oclef[i].second->isMens()) {
            menscount++;
        }
    }

    if (kerncount == 0) {
        // Don't produce <app> for **mens data for now.
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

    if (m_oclef.size() > 0) {
        for (int i = 0; i < (int)m_oclef.size(); ++i) {
            StaffDef *staffdef = new StaffDef;
            staffgrp->AddChild(staffdef);
            setClef(staffdef, *m_oclef[i].second);
            staffdef->SetN(m_oclef[i].first);
            for (int j = 0; j < (int)m_omet.size(); ++j) {
                if (m_omet[j].first != m_oclef[i].first) {
                    continue;
                }
                setMeterSymbol(staffdef, *m_omet[j].second);
            }
        }
    }
    else if (m_omet.size() > 0) {
        // No oclefs, just omets.
        for (int i = 0; i < (int)m_oclef.size(); ++i) {
            StaffDef *staffdef = new StaffDef;
            staffgrp->AddChild(staffdef);
            setMeterSymbol(staffdef, *m_omet[i].second);
            staffdef->SetN(m_omet[i].first);
        }
    }

    m_oclef.clear();
    m_omet.clear();
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
    hum::HumdrumFile &infile = m_infiles[0];

    std::string endbar = infile[endline].getTokenString(0);
    std::string startbar = infile[startline].getTokenString(0);
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
    hum::HumdrumFile &infile = m_infiles[0];
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

void HumdrumInput::setupMeiDocument()
{
    hum::HumdrumFile &infile = m_infiles[0];
    m_doc->Reset();
    m_doc->SetType(Raw);
    // The mdiv
    Mdiv *mdiv = new Mdiv();
    mdiv->m_visibility = Visible;
    m_doc->AddChild(mdiv);
    // The score
    m_score = new Score();
    mdiv->AddChild(m_score);
    // the section
    Section *section = new Section();
    hum::HTp starting = infile.getTrackStart(1);
    if (starting) {
        section->SetUuid(getLocationId(section, starting, -1));
        storeExpansionLists(section, starting);
    }
    m_sections.push_back(section);
    m_score->AddChild(m_sections.back());
    m_leftbarstyle = BARRENDITION_NONE;
    if (m_breaks) {
        // An initial page break is required in order for the system
        // breaks encoded in the file to be activated, so adding a
        // dummy page break here:
        Pb *pb = new Pb;
        section->AddChild(pb);
    }
}

//////////////////////////////
//
// HumdrumInput::clear -- clear contents of object (m_doc handled by
//    parent class).
//

void HumdrumInput::clear()
{
    m_filename = "";
    m_tupletscaling = 1;
    m_breaks = false;
    m_duradj.clear();
    m_nulls.clear();
    m_fbstates.clear();
}

//////////////////////////////
//
// HumdrumInput::getMeasureNumber -- Return the current barline's measure
//     number, or return -1 if no measure number.  Returns 0 if a
//     pickup measure.
//

int HumdrumInput::getMeasureNumber(int startline, int endline)
{
    hum::HumdrumFile &infile = m_infiles[0];

    if (infile[startline].getTokenString(0).compare(0, 2, "**") == 0) {
        // create hum::HumdrumFile.hasPickup() function and uncomment out below:
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

void HumdrumInput::calculateLayout()
{
    // m_doc->CastOff();
}

//////////////////////////////
//
// HumdrumInput::emptyMeasures -- For initial development, maybe convert to
//     an option.
//

bool HumdrumInput::emptyMeasures()
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

string HumdrumInput::unescapeHtmlEntities(const std::string &input)
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

std::string HumdrumInput::GetMeiString()
{
    MeiOutput meioutput(m_doc, "");
    meioutput.SetScoreBasedMEI(true);
    return meioutput.GetOutput();
}

//////////////////////////////
//
// HumdrumInput::setLocationId -- use the file location of the item
//    for the ID.
//     default value: subtoken = -1 (don't display subtoken number);
//

void HumdrumInput::setLocationId(Object *object, hum::HTp token, int subtoken)
{
    object->SetUuid(getLocationId(object, token, subtoken));
}

void HumdrumInput::setLocationId(Object *object, int lineindex, int fieldindex, int subtokenindex)
{
    object->SetUuid(getLocationId(object, lineindex, fieldindex, subtokenindex));
}

///////////////////////////////////
//
// HumdrumInput::getLocationId --
//

std::string HumdrumInput::getLocationId(Object *object, hum::HTp token, int subtoken)
{
    int line = token->getLineIndex() + 1;
    int field = token->getFieldIndex() + 1;
    std::string id = object->GetClassName();
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
    id += "-L" + to_string(line);
    id += "F" + to_string(field);
    if (subtoken >= 0) {
        id += "S" + to_string(subtoken + 1);
    }
    return id;
}

std::string HumdrumInput::getLocationId(const string &prefix, hum::HTp token, int subtoken)
{
    int line = token->getLineIndex() + 1;
    int field = token->getFieldIndex() + 1;
    std::string id = prefix;
    id += "-L" + to_string(line);
    id += "F" + to_string(field);
    if (subtoken >= 0) {
        id += "S" + to_string(subtoken + 1);
    }
    return id;
}

std::string HumdrumInput::getLocationId(Object *object, int lineindex, int fieldindex, int subtokenindex)
{
    int line = lineindex + 1;
    int field = fieldindex + 1;
    int subtoken = subtokenindex + 1;
    std::string id = object->GetClassName();
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
    return id;
}

std::string HumdrumInput::getLocationId(const string &prefix, int lineindex, int fieldindex, int subtokenindex)
{
    int line = lineindex + 1;
    int field = fieldindex + 1;
    int subtoken = subtokenindex + 1;
    std::string id = prefix;
    if (line > 0) {
        id += "-L" + to_string(line);
    }
    if (field > 0) {
        id += "F" + to_string(field);
    }
    if (subtoken > 0) {
        id += "S" + to_string(subtoken);
    }
    return id;
}

//////////////////////////////
//
// HumdrumInput::setLocationIdNSuffix --
//

void HumdrumInput::setLocationIdNSuffix(Object *object, hum::HTp token, int number)
{
    int line = token->getLineIndex() + 1;
    int field = token->getFieldIndex() + 1;
    std::string id = object->GetClassName();
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
    id += "-L" + to_string(line);
    id += "F" + to_string(field);
    id += "N" + to_string(number);
    object->SetUuid(id);
}

/////////////////////////////
//
// HumdrumInput::checkBeamWith -- If all notes of a beam are not on the home staff,
//    then use beam@beam.width="below|above" to move it to the adjacent staff (it
//    will not be moved there automatically by verovio).
//

void HumdrumInput::checkBeamWith(
    Beam *beam, const std::vector<humaux::HumdrumBeamAndTuplet> &tgs, std::vector<hum::HTp> &layerdata, int startindex)
{
    if (!(m_signifiers.above || m_signifiers.below)) {
        return;
    }
    hum::HumRegex hre;
    int targetEnd = tgs[startindex].beamstart;
    bool isAbove = true;
    bool isBelow = true;
    bool foundAbove = false;
    bool foundBelow = false;
    string aboveSearch = "[A-Ga-gn#-][XxYy]?";
    string belowSearch = "[A-Ga-gn#-][XxYy]?";
    aboveSearch += m_signifiers.above;
    belowSearch += m_signifiers.below;
    for (int i = startindex; i < (int)layerdata.size(); i++) {
        if (!(layerdata[i]->isNote() || layerdata[i]->isRest())) {
            continue;
        }
        if (isAbove) {
            if (hre.search(layerdata[i], aboveSearch)) {
                foundAbove = true;
            }
            else {
                foundAbove = false;
            }
        }
        if (isBelow) {
            if (hre.search(layerdata[i], belowSearch)) {
                foundBelow = true;
            }
            else {
                foundBelow = false;
            }
        }
        isAbove &= foundAbove;
        isBelow &= foundBelow;
        if (!(isAbove || isBelow)) {
            break;
        }
        if (tgs[i].beamend == targetEnd) {
            break;
        }
    }
    if (!(isAbove || isBelow)) {
        return;
    }
    if (isAbove && isBelow) {
        // something strange happened
    }
    if (isAbove) {
        beam->SetBeamWith(OTHERSTAFF_above);
    }
    if (isBelow) {
        beam->SetBeamWith(OTHERSTAFF_below);
    }
}

/////////////////////////////
//
// HumdrumInput::setBeamLocationId --
//

void HumdrumInput::setBeamLocationId(Object *object, const std::vector<humaux::HumdrumBeamAndTuplet> &tgs,
    std::vector<hum::HTp> &layerdata, int startindex)
{
    int startnum = tgs[startindex].beamstart;
    hum::HTp starttoken = layerdata[startindex];
    int startline = starttoken->getLineNumber();
    int startfield = starttoken->getFieldNumber();

    std::string id = object->GetClassName();
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
    id += "-L" + to_string(startline);
    id += "F" + to_string(startfield);

    // int endnum = -1;
    int endindex = -1;
    for (int i = startindex + 1; i < (int)tgs.size(); ++i) {
        if (tgs[i].beamend == startnum) {
            // endnum = startnum;
            endindex = i;
            break;
        }
    }

    if (endindex > 0) {
        hum::HTp endtoken = layerdata[endindex];
        int endline = endtoken->getLineNumber();
        int endfield = endtoken->getFieldNumber();
        id += "-L" + to_string(endline);
        id += "F" + to_string(endfield);
    }

    object->SetUuid(id);
}

/////////////////////////////
//
// HumdrumInput::setTupletLocationId --
//

void HumdrumInput::setTupletLocationId(Object *object, const std::vector<humaux::HumdrumBeamAndTuplet> &tgs,
    std::vector<hum::HTp> &layerdata, int startindex)
{
    int startnum = tgs[startindex].tupletstart;
    hum::HTp starttoken = layerdata[startindex];
    int startline = starttoken->getLineNumber();
    int startfield = starttoken->getFieldNumber();

    std::string id = object->GetClassName();
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
    id += "-L" + to_string(startline);
    id += "F" + to_string(startfield);

    // int endnum = -1;
    int endindex = -1;
    for (int i = startindex + 1; i < (int)tgs.size(); ++i) {
        if (tgs[i].tupletend == startnum) {
            // endnum = startnum;
            endindex = i;
            break;
        }
    }

    if (endindex > 0) {
        hum::HTp endtoken = layerdata[endindex];
        int endline = endtoken->getLineNumber();
        int endfield = endtoken->getFieldNumber();
        id += "-L" + to_string(endline);
        id += "F" + to_string(endfield);
    }

    object->SetUuid(id);
}

/////////////////////////////
//
// HumdrumInput::setTieLocationId --
//

void HumdrumInput::setTieLocationId(Object *object, hum::HTp tiestart, int sindex, hum::HTp tieend, int eindex)
{

    int startline = tiestart->getLineNumber();
    int startfield = tiestart->getFieldNumber();
    int endline = tieend->getLineNumber();
    int endfield = tieend->getFieldNumber();

    std::string id = object->GetClassName();
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);

    id += "-L" + to_string(startline);
    id += "F" + to_string(startfield);
    if (sindex >= 0) {
        id += "S" + to_string(sindex + 1);
    }

    id += "-L" + to_string(endline);
    id += "F" + to_string(endfield);
    if (eindex >= 0) {
        id += "S" + to_string(eindex + 1);
    }

    object->SetUuid(id);
}

/////////////////////////////
//
// HumdrumInput::setSlurLocationId -- Not dealing with cross-over slurs yet,
// such as &( and &).
//

void HumdrumInput::setSlurLocationId(Object *object, hum::HTp slurstart, hum::HTp slurend, int eindex)
{
    int startline = slurstart->getLineNumber();
    int startfield = slurstart->getFieldNumber();
    std::string id = object->GetClassName();
    std::transform(id.begin(), id.end(), id.begin(), ::tolower);
    id += "-L" + to_string(startline);
    id += "F" + to_string(startfield);
    int count1 = slurstart->getValueInt("auto", "slurEndCount");
    int count2 = slurend->getValueInt("auto", "slurStartCount");
    int num1 = 0;
    int num2 = eindex + 1;

    if ((count1 > 1) || (count2 > 1)) {
        // resolve which slur is being created.
        std::string endid = slurend->getValue("auto", "id");
        std::string startid = slurstart->getValue("auto", "id");
        std::vector<int> index1(count1, 0);
        std::vector<int> index2(count2, 0);
        int counter;

        if (count1 > 1) {
            counter = 0;
            for (int i = 0; i < count1; ++i) {
                std::string param = "slurEnd";
                if (i > 0) {
                    param += to_string(i + 1);
                }
                std::string tid = slurstart->getValue("auto", param);
                if (tid == endid) {
                    index1[i] = ++counter;
                }
            }
        }

        if (count2 > 1) {
            counter = 0;
            for (int i = 0; i < count2; ++i) {
                std::string param = "slurStart";
                if (i > 0) {
                    param += to_string(i + 1);
                }
                std::string tid = slurend->getValue("auto", param);
                if (tid == startid) {
                    index2[i] = ++counter;
                }
            }
        }

        int targetindex = index2[eindex];
        auto location = std::find(index1.begin(), index1.end(), targetindex);
        if (location != index1.end()) {
            num1 = int(index1.end() - location);
        }
    }

    if (count1 > 1) {
        if (num1 > 0) {
            id += "N";
            id += to_string(num1);
        }
    }

    int endline = slurend->getLineNumber();
    int endfield = slurend->getFieldNumber();

    id += "-L";
    id += to_string(endline);
    id += "F";
    id += to_string(endfield);

    if (count2 > 1) {
        if (num2 > 0) {
            id += "N";
            id += to_string(num2);
        }
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

void HumdrumInput::parseSignifiers(hum::HumdrumFile &infile)
{
    hum::HumRegex hre;
    std::vector<hum::HumdrumLine *> refs = infile.getReferenceRecords();
    for (int i = 0; i < (int)refs.size(); ++i) {
        std::string key = refs[i]->getReferenceKey();
        std::string value = refs[i]->getReferenceValue();
        auto equals = value.substr(0, 8).find('=');
        std::string afterequals;
        if (equals != std::string::npos) {
            afterequals = value.substr(equals + 1);
        }
        else {
            afterequals = value;
        }

        if ((equals == string::npos) && (key == "RDF**kern")) {
            // meta signifiers (no actual signifier)

            // colored spaces (meta signifiers)
            // !!!RDF**kern: show spaces color=hotpink
            // !!!RDF**kern: show invisible rests color=chartreuse
            // !!!RDF**kern: show implicit spaces color=purple
            // !!!RDF**kern: show recip spaces color=royalblue
            if (value.find("show space") != string::npos) {
                if (hre.search(value, "color\\s*=\\s*\"?([^\"\\s]+)\"?")) {
                    m_signifiers.space_color = hre.getMatch(1);
                }
                else {
                    m_signifiers.space_color = "hotpink";
                }
            }
            if (value.find("show invisible rest") != string::npos) {
                if (hre.search(value, "color\\s*=\\s*\"?([^\"\\s]+)\"?")) {
                    m_signifiers.irest_color = hre.getMatch(1);
                }
                else {
                    m_signifiers.irest_color = "chartreuse";
                }
            }
            if (value.find("show implicit space") != string::npos) {
                if (hre.search(value, "color\\s*=\\s*\"?([^\"\\s]+)\"?")) {
                    m_signifiers.ispace_color = hre.getMatch(1);
                }
                else {
                    m_signifiers.ispace_color = "blueviolet";
                }
            }
            if (value.find("show recip space") != string::npos) {
                if (hre.search(value, "color\\s*=\\s*\"?([^\"\\s]+)\"?")) {
                    m_signifiers.rspace_color = hre.getMatch(1);
                }
                else {
                    m_signifiers.rspace_color = "royalblue";
                }
            }

            continue;
        }

        char signifier = 0;
        for (int j = 0; j < (int)equals; ++j) {
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

        if (((key == "RDF**silbe") || (key == "RDF**text")) && hre.search(value, "marked text|matched text")) {
            // for **text and **silbe
            m_signifiers.textmark.push_back(signifier);
            if (hre.search(value, "color\\s*=\\s*\"?([^\"\\s]+)\"?")) {
                m_signifiers.textcolor.push_back(hre.getMatch(1));
            }
            else {
                m_signifiers.textcolor.push_back("red");
            }
        }
        else if (key == "RDF**dynam") {
            if (signifier == '>') {
                if (hre.search(afterequals, "^\\s*\"\\s*([^\"]+)\\s*\"")) {
                    m_signifiers.decresctext = hre.getMatch(1);
                }
                else if (hre.search(afterequals, "^\\s*([^\\s]+)")) {
                    m_signifiers.decresctext = hre.getMatch(1);
                }
                if (hre.search(afterequals, "fontstyle\\s*=\\s*\"?\\s*([^\"\\s]+)\\s*\"?")) {
                    m_signifiers.decrescfontstyle = hre.getMatch(1);
                    if (m_signifiers.crescfontstyle == "italic") {
                        // default is italic for verovio, so suppress:
                        m_signifiers.crescfontstyle = "";
                    }
                }
            }
            else if (signifier == '<') {
                if (hre.search(afterequals, "^\\s*\"\\s*([^\"]+)\\s*\"")) {
                    m_signifiers.cresctext = hre.getMatch(1);
                }
                else if (hre.search(afterequals, "^\\s*([^\\s]+)")) {
                    m_signifiers.cresctext = hre.getMatch(1);
                }
                if (hre.search(afterequals, "fontstyle\\s*=\\s*\"?\\s*([^\"\\s]+)\\s*\"?")) {
                    m_signifiers.crescfontstyle = hre.getMatch(1);
                    if (m_signifiers.crescfontstyle == "italic") {
                        // default is italic for verovio, so suppress:
                        m_signifiers.crescfontstyle = "";
                    }
                }
            }
        }

        if (key != "RDF**kern") {
            continue;
        }

        // stemless note:
        // !!!RDF**kern: i = no stem
        if (value.find("no stem", equals) != string::npos) {
            m_signifiers.nostem = signifier;
        }

        // cue-sized note:
        // !!!RDF**kern: i = cue size
        if (value.find("cue size", equals) != string::npos) {
            m_signifiers.cuesize = signifier;
        }

        // terminal longs
        // !!!RDF**kern: i = terminal long
        if (value.find("terminal long", equals) != string::npos) {
            m_signifiers.terminallong = signifier;
        }
        else if (value.find("long note", equals) != string::npos) {
            m_signifiers.terminallong = signifier;
        }

        // slur directions
        if (value.find("above", equals) != string::npos) {
            m_signifiers.above = signifier;
        }
        if (value.find("below", equals) != string::npos) {
            m_signifiers.below = signifier;
        }

        // editorial accidentals:
        if (value.find("editorial accidental", equals) != string::npos) {
            m_signifiers.editacc.push_back(signifier);
            if (value.find("brack") != string::npos) {
                m_signifiers.edittype.push_back("brack");
            }
            else if (value.find("paren") != string::npos) {
                m_signifiers.edittype.push_back("paren");
            }
            else {
                m_signifiers.edittype.push_back("");
            }
        }

        // colored notes
        // !!!RDF**kern: i = marked note, color="#ff0000"
        // !!!RDF**kern: i = matched note, color=blue
        if (hre.search(value, "color\\s*=\\s*\"?([^\"\\s]+)\"?")) {
            m_signifiers.mark.push_back(signifier);
            m_signifiers.mcolor.push_back(hre.getMatch(1));

            if (hre.search(value, "text\\s*=\\s*\"?([^\"]+)\"?")) {
                m_signifiers.markdir.push_back(hre.getMatch(1));
            }
            else {
                m_signifiers.markdir.push_back("");
            }
        }
        else if (hre.search(value, "marked note|matched note")) {
            m_signifiers.mark.push_back(signifier);
            m_signifiers.mcolor.push_back("red");

            if (hre.search(value, "text\\s*=\\s*\"?([^\"]+)\"?")) {
                m_signifiers.markdir.push_back(hre.getMatch(1));
            }
            else {
                m_signifiers.markdir.push_back("");
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::analyzeBreaks -- Returns true if there are page or
//   system breaks in the data.
//

bool HumdrumInput::analyzeBreaks(hum::HumdrumFile &infile)
{

    // check for informal breaking markers such as:
    // !!pagebreak:original
    // !!linebreak:original
    for (int i = 0; i < infile.getLineCount(); i++) {
        if (!infile[i].isGlobalComment()) {
            continue;
        }
        hum::HTp token = infile.token(i, 0);
        if (token->compare(0, 12, "!!pagebreak:") == 0) {
            return true;
        }
        if (token->compare(0, 12, "!!linebreak:") == 0) {
            return true;
        }
    }

    // check for formal breaking markers such as:
    // !!LO:PB:g=original
    // !!LO:LB:g=original
    // !LO:PB:g=original
    // !LO:LB:g=original
    for (int i = 0; i < infile.getLineCount(); i++) {
        if (!infile[i].isComment()) {
            continue;
        }
        hum::HTp token = infile.token(i, 0);
        if (token->find("!LO:LB") != std::string::npos) {
            return true;
        }
        if (token->find("!LO:PB") != std::string::npos) {
            return true;
        }
    }

    return false;
}

//////////////////////////////
//
// HumdrumInput::analyzeMultiRest --
//

std::vector<int> HumdrumInput::analyzeMultiRest(hum::HumdrumFile &infile)
{
    std::vector<int> barindex(1, 0); // line number of barline
    std::vector<int> datacount(1, 0);
    std::vector<int> dataline(1, 0);
    std::vector<hum::HumNum> bardur(1, 0);

    // count number of non-null data lines for each measure.
    for (int i = 0; i < infile.getLineCount(); ++i) {
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
    std::vector<int> wholerest(barindex.size(), 0);
    std::vector<int> textrest(barindex.size(), 0);
    bool restQ;
    int line;
    for (int i = 0; i < (int)barindex.size(); ++i) {
        if (datacount[i] == 1) {
            restQ = true;
            line = dataline[i];
            for (int j = 0; j < infile[line].getFieldCount(); ++j) {
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

    // remove cases where **mxhm text is present at the same
    // time as the whole-measure rest.  There will still be cases
    // where there is a chord/dynamic which does not start
    // at the same time as the rest that need to be accounted for.
    for (int i = 0; i < (int)wholerest.size(); i++) {
        if (wholerest[i] != 1) {
            continue;
        }
        bool hasitem = false;
        line = dataline[i];
        for (int j = 0; j < infile[line].getFieldCount(); ++j) {
            hum::HTp tok = infile.token(line, j);
            if (tok->isNull()) {
                continue;
            }
            if (tok->isDataType("**mxhm")) {
                hasitem = true;
                break;
            }
            else if (tok->isDataType("**dynam")) {
                hasitem = true;
                break;
            }
            else if (tok->isDataType("**text")) {
                hasitem = true;
                break;
            }
        }
        if (hasitem) {
            wholerest[i] = 0;
            textrest[i] = 1;
        }
    }

    // remove cases where there is text attached to the whole-measure rest
    for (int i = 0; i < (int)wholerest.size(); i++) {
        if (wholerest[i] != 1) {
            continue;
        }
        bool hastext = false;
        line = dataline[i];
        for (int j = 0; j < infile[line].getFieldCount(); ++j) {
            hum::HTp tok = infile.token(line, j);
            if (tok->isNull()) {
                continue;
            }
            if (!tok->isKern()) {
                continue;
            }
            std::string text = tok->getLayoutParameter("TX", "t");
            if (!text.empty()) {
                hastext = true;
                break;
            }
        }
        if (hastext) {
            wholerest[i] = 0;
            textrest[i] = 1;
        }
    }

    // group sequences of whole-meaure rests
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

    // Expand backwards to include a whole-measure rest with a
    // measure that has text.
    for (int i = 0; i < (int)wholerest.size() - 1; i++) {
        if (bardur[i] != bardur[i + 1]) {
            continue;
        }
        if ((textrest[i] == 1) && (wholerest[i + 1] >= 1)) {
            wholerest[i] = wholerest[i + 1] + 1;
            wholerest[i + 1] = -1;
        }
    }

    std::vector<int> output(infile.getLineCount(), 0);
    for (int i = 0; i < (int)wholerest.size(); ++i) {
        output[dataline[i]] = wholerest[i];
    }
    for (int i = infile.getLineCount() - 2; i >= 0; i--) {
        if (!infile[i + 1].isBarline()) {
            if (output[i + 1] != 0) {
                output[i] = output[i + 1];
            }
        }
    }

    // for (int i = 0; i < infile.getLineCount(); ++i) {
    //    cout << infile[i] << "\t" << output[i] << "\n";
    //}
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
// HumdrumInput::prepareSections --
//

void HumdrumInput::prepareSections()
{
    std::vector<hum::HTp> &sectionlabels = m_sectionlabels;
    hum::HumdrumFile &infile = m_infiles[0];

    sectionlabels.resize(infile.getLineCount());
    for (int i = 0; i < (int)sectionlabels.size(); i++) {
        sectionlabels[i] = NULL;
    }
    hum::HTp secname = NULL;

    for (int i = 0; i < infile.getLineCount(); ++i) {
        sectionlabels[i] = secname;
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

        //        std::smatch matches;
        //        if (regex_search(*((string *)infile.token(i, 0)), matches, regex("(\\d+)$"))) {
        //            endnum = stoi(matches[1]);
        //            ending[i] = endnum;
        //        }
        //        else {
        //            endnum = 0;
        //            ending[i] = endnum;
        //        }

        secname = infile.token(i, 0);
        sectionlabels[i] = secname;
        for (int j = i - 1; j >= 0; j--) {
            if (infile[j].isData()) {
                break;
            }
            sectionlabels[j] = sectionlabels[i];
        }
    }
}

//////////////////////////////
//
// HumdrumInput::checkForColorSpine -- Look for a **color spine in the input
// data.
//

void HumdrumInput::checkForColorSpine(hum::HumdrumFile &infile)
{
    std::vector<hum::HTp> colorspines;
    infile.getSpineStartList(colorspines, "**color");
    m_has_color_spine = colorspines.empty() ? false : true;
}

//////////////////////////////
//
// storeExpansionLists --
//

void HumdrumInput::storeExpansionLists(Section *section, hum::HTp starting)
{
    hum::HTp current = starting;
    while (current) {
        if (current->isData()) {
            // only look for expansion lists before first data line
            break;
        }
        if (!current->isInterpretation()) {
            current = current->getNextToken();
            continue;
        }
        if (current->compare(0, 2, "*>") != 0) {
            current = current->getNextToken();
            continue;
        }
        if (current->find("[") == std::string::npos) {
            current = current->getNextToken();
            continue;
        }
        storeExpansionList(section, current);
        current = current->getNextToken();
    }
}

//////////////////////////////
//
// storeExpansionList --
//

void HumdrumInput::storeExpansionList(Section *section, hum::HTp etok)
{
    string expansion = *etok;
    string variant;
    int startindex = -1;
    for (int i = 2; i < (int)expansion.size(); i++) {
        if (expansion[i] == '[') {
            startindex = i + 1;
            break;
        }
        variant += expansion[i];
    }
    if (startindex < 0) {
        return;
    }
    std::vector<std::string> labels(1);
    for (int i = startindex; i < (int)expansion.size(); i++) {
        if (isspace(expansion[i])) {
            continue;
        }
        else if (expansion[i] == '"') {
            // invalid character
            continue;
        }
        else if (expansion[i] == '\'') {
            // invalid character
            continue;
        }
        else if (expansion[i] == ',') {
            if (!labels.back().empty()) {
                // avoid syntax error from a null label.
                labels.push_back("");
            }
        }
        else if (expansion[i] == ']') {
            break;
        }
        else {
            labels.back() += expansion[i];
        }
    }

    if (labels.empty()) {
        return;
    }
    if ((labels.size() == 1) && labels[0].empty()) {
        return;
    }

    Expansion *exp = new Expansion;
    exp->SetUuid(getLocationId(exp, etok, -1));
    section->AddChild(exp);
    if (!variant.empty()) {
        exp->SetType(variant);
    }

    for (int i = 0; i < (int)labels.size(); i++) {
        string ref = "#label-" + labels[i];
        exp->AddRefAllowDuplicate(ref);
    }
}

#endif /* NO_HUMDRUM_SUPPORT */

} // namespace vrv
