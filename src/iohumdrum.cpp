/////////////////////////////////////////////////////////////////////////////
// Name:        iohumdrum.cpp
// Author:      Craig Stuart Sapp
// Created:     06/06/2015
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

#ifndef NO_HUMDRUM_SUPPORT

#include "iohumdrum.h"
#include "humlib.h"
#include <math.h>

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <sstream>
#include <vector>

//----------------------------------------------------------------------------

#include "beam.h"
#include "chord.h"
#include "doc.h"
#include "iomei.h"
#include "layer.h"
#include "measure.h"
#include "mrest.h"
#include "note.h"
#include "page.h"
#include "rest.h"
#include "space.h"
#include "staff.h"
#include "syl.h"
#include "system.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "verse.h"
#include "vrv.h"

//#include "attcomparison.h"
//#include "slur.h"
//#include "tuplet.h"
//#include "pugixml.hpp"

using namespace hum; // humlib  namespace
using namespace vrv; // verovio namespace
using namespace pugi; // pugixml namespace
using namespace std;

namespace vrv {

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
            // This is a tie with no end.  Don't know what to do with this for now
            // (but is possible due to repeated music).
            return;
        }

        vrv::Tie *tie = new vrv::Tie;
        tie->SetStartid(m_startid);
        tie->SetEndid(m_endid);

        bool samemeasure = false;
        if (m_startmeasure == m_endmeasure) {
            samemeasure = true;
        }

        if (samemeasure) {
            m_startmeasure->AddFloatingElement(tie);
            m_inserted = true;
        }
        else {
            // The tie starts in one measure and goes to another.
            // Probably handled the same as when in the same measure.
            m_startmeasure->AddFloatingElement(tie);
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

} // end namespace humaux

//----------------------------------------------------------------------------
// HumdrumInput
//----------------------------------------------------------------------------

//////////////////////////////
//
// HumdrumInput::HumdrumInput -- Constructor.
//

HumdrumInput::HumdrumInput(Doc *doc, std::string filename) : FileInputStream(doc)
{
    m_filename = filename;

    m_staffgroup = NULL;
    // m_staffdef is a vector

    m_page = NULL;
    m_system = NULL;
    m_measure = NULL;
    m_staff = NULL;
    m_layer = NULL;
    m_currentlayer = -1;

    m_debug = 1;
}

//////////////////////////////
//
// HumdrumInput::~HumdrumInput -- Deconstructor.
//

HumdrumInput::~HumdrumInput()
{
    clear();
}

//////////////////////////////
//
// HumdrumInput::ImportFile -- Read a Humdrum file from a file.
//

bool HumdrumInput::ImportFile()
{
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
}

//////////////////////////////
//
// HumdrumInput::ImportString -- Read a Humdrum file from a text string.
//

bool HumdrumInput::ImportString(const std::string content)
{
    try {
        m_doc->Reset();
        HumdrumFile &infile = m_infile;
        bool result = infile.readString(content);
        if (!result) {
            return false;
        }
        return convertHumdrum();
    }
    catch (char *str) {
        LogError("%s", str);
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////
//
// Protected functions.
//

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

    bool status = true; // for keeping track of problems in conversion process.

    prepareTimeSigDur();
    setupMeiDocument();

    // Create a list of the parts and which spine represents them.
    vector<HTp> &kernstarts = m_kernstarts;
    kernstarts = infile.getKernSpineStartList();

    if (kernstarts.size() == 0) {
        // no parts in file, give up.  Perhaps return an error.
        return status;
    }

    // Reverse the order, since top part is last spine.
    reverse(kernstarts.begin(), kernstarts.end());
    calculateReverseKernIndex();

    m_ties.resize(kernstarts.size());

    prepareVerses();
    prepareStaffGroup();

    int line = kernstarts[0]->getLineIndex();
    while (line < infile.getLineCount() - 1 && (line >= 0)) {
        status &= convertSystemMeasure(line);
    }

    // calculateLayout();

    if (m_debug) {
        MeiOutput meioutput(m_doc, "");
        meioutput.SetScoreBasedMEI(true);
        string mei = meioutput.GetOutput();
        cout << mei;
    }

    return status;
}

//////////////////////////////
//
// HumdrumInput::prepareVerses -- Assumes that m_kernstarts has been
//      filled already.
//

void HumdrumInput::prepareVerses(void)
{
    int i, j;
    vector<bool> &verses = m_verses;
    vector<HTp> &kern = m_kernstarts;

    verses.resize(kern.size());
    std::fill(verses.begin(), verses.end(), false);

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
                verses[i] = true;
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::prepareTimeSigDur -- create a list of the duration of time
//      signatures in the file, indexed by HumdrumFile line number.  Only the
//      first spine in the file is considered.
//

void HumdrumInput::prepareTimeSigDur(void)
{
    vector<HumNum> &sigdurs = m_timesigdurs;
    HumdrumFile &infile = m_infile;
    sigdurs.resize(infile.getLineCount());
    std::fill(sigdurs.begin(), sigdurs.end(), -1);
    HumNum curdur = -1;
    int top;
    int bot;
    int bot2;
    int i;
    for (i = 0; i < infile.getLineCount(); i++) {
        if (!infile[i].isInterpretation()) {
            sigdurs[i] = curdur;
        }
        if (sscanf(infile[i].token(0)->c_str(), "*M%d/%d%%%d", &top, &bot, &bot2) == 3) {
            // deal with triplet-whole note beats later
        }
        else if (sscanf(infile[i].token(0)->c_str(), "*M%d/%d", &top, &bot) == 2) {
            curdur = top;
            if (bot == 0) { // breve
                curdur *= 2;
            }
            else {
                curdur /= bot;
            }
            curdur *= 4; // convert to quarter note units;
        }
        sigdurs[i] = curdur;
    }

    for (i = (int)sigdurs.size() - 2; i >= 0; i--) {
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
    m_doc->m_scoreDef.AddStaffGrp(m_staffgroup);
    for (int i = 0; i < (int)kernstarts.size(); i++) {
        m_staffdef.push_back(new StaffDef());
        m_staffgroup->AddStaffDef(m_staffdef.back());
        fillPartInfo(kernstarts[i], i + 1);
    }
    if (kernstarts.size() == 2) {
        m_staffgroup->SetSymbol(staffgroupingsym_SYMBOL_brace);
    }
    else if (kernstarts.size() > 2) {
        m_staffgroup->SetSymbol(staffgroupingsym_SYMBOL_bracket);
    }
}

//////////////////////////////
//
// HumdrumInput::fillPartInfo -- Should use regular expressions
//    in the future.
//

void HumdrumInput::fillPartInfo(HTp partstart, int partnumber)
{

    string label;
    string abbreviation;
    string oclef;
    string clef;
    string keysig;
    string key;
    string timesig;
    string metersig;
    int top = 0;
    int bot = 0;

    HTp part = partstart;
    while (part && !part->getLine()->isData()) {
        if (part->compare(0, 5, "*clef") == 0) {
            clef = *part;
        }
        else if (part->compare(0, 6, "*oclef") == 0) {
            oclef = *part;
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
        else if (sscanf(part->c_str(), "*M%d/%d", &top, &bot) == 2) {
            timesig = *part;
        }
        part = part->getNextToken();
    }

    m_staffdef.back()->SetN(partnumber);
    m_staffdef.back()->SetLines(5);

    if (clef.size() > 0) {
        setClef(m_staffdef.back(), clef);
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

    // m_staffdef.back()->SetMeterSym(METERSIGN_common);
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

void HumdrumInput::setKeySig(StaffDef *part, const string &keysig)
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
        part->SetKeySig(KEYSIGNATURE_1s);
    }
    else if (fs && cs && !gs && !ds && !as && !es && !bs) {
        part->SetKeySig(KEYSIGNATURE_2s);
    }
    else if (fs && cs && gs && !ds && !as && !es && !bs) {
        part->SetKeySig(KEYSIGNATURE_3s);
    }
    else if (fs && cs && gs && ds && !as && !es && !bs) {
        part->SetKeySig(KEYSIGNATURE_4s);
    }
    else if (fs && cs && gs && ds && as && !es && !bs) {
        part->SetKeySig(KEYSIGNATURE_5s);
    }
    else if (fs && cs && gs && ds && as && es && !bs) {
        part->SetKeySig(KEYSIGNATURE_6s);
    }
    else if (fs && cs && gs && ds && as && es && bs) {
        part->SetKeySig(KEYSIGNATURE_7s);
    }
    else if (bb && !eb && !ab && !db && !gb && !cb && !fb) {
        part->SetKeySig(KEYSIGNATURE_1f);
    }
    else if (bb && eb && !ab && !db && !gb && !cb && !fb) {
        part->SetKeySig(KEYSIGNATURE_2f);
    }
    else if (bb && eb && ab && !db && !gb && !cb && !fb) {
        part->SetKeySig(KEYSIGNATURE_3f);
    }
    else if (bb && eb && ab && db && !gb && !cb && !fb) {
        part->SetKeySig(KEYSIGNATURE_4f);
    }
    else if (bb && eb && ab && db && gb && !cb && !fb) {
        part->SetKeySig(KEYSIGNATURE_5f);
    }
    else if (bb && eb && ab && db && gb && cb && !fb) {
        part->SetKeySig(KEYSIGNATURE_6f);
    }
    else if (bb && eb && ab && db && gb && cb && fb) {
        part->SetKeySig(KEYSIGNATURE_7f);
    }
    else if (!bb && !eb && !ab && !db && !gb && !cb && !fb && !fs && !cs && !gs && !ds && !as && !es && !bs) {
        part->SetKeySig(KEYSIGNATURE_0);
    }
    else {
        // nonstandard keysignature, so give a NONE style.
        part->SetKeySig(KEYSIGNATURE_NONE);
    }
}

//////////////////////////////
//
// HumdrumInput::setClef -- Convert a Humdrum clef to an MEI clef.
//

void HumdrumInput::setClef(StaffDef *part, const string &clef)
{
    if (clef.find("clefG") != string::npos) {
        part->SetClefShape(CLEFSHAPE_G);
    }
    else if (clef.find("clefF") != string::npos) {
        part->SetClefShape(CLEFSHAPE_F);
    }
    else if (clef.find("clefC") != string::npos) {
        part->SetClefShape(CLEFSHAPE_C);
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

    if (clef.find("v") != string::npos) {
        part->SetClefDis(OCTAVE_DIS_8);
        part->SetClefDisPlace(PLACE_below);
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
    else {
        line = endline;
    }

    setupSystemMeasure(startline, endline);

    storeStaffLayerTokensForMeasure(startline, endline);

    checkForOmd(startline, endline);

    return convertMeasureStaves(startline, endline);
}

//////////////////////////////
//
// HumdrumInput::checkForOmd --
//

void HumdrumInput::checkForOmd(int startline, int endline)
{
    const vector<HTp> &kernstarts = m_kernstarts;
    if (kernstarts.size() == 0) {
        return;
    }
    if (startline != kernstarts[0]->getLineIndex()) {
        // not at the start of the file.
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
            m_measure->AddFloatingElement(tempo);
            Text *text = new Text;
            tempo->AddTextElement(text);
            text->SetText(UTF8to16(unescapeHtmlEntities(value)));
            // tempo->ResetStaffident();  // clear out previous staff owner(s).
            tempo->AddStaff(1);
        }
    }
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
            if (infile[i].token(j)->isNull()) {
                continue;
            }
            if (lt[staffindex].size() < layerindex + 1) {
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

    bool status = true;
    for (int i = 0; i < (int)kernstarts.size(); i++) {
        status &= convertMeasureStaff(kernstarts[i]->getTrack(), startline, endline, i + 1, layers[i]);
        if (!status) {
            break;
        }
    }

    return status;
}

//////////////////////////////
//
// HumdrumInput::convertMeasureStaff -- print a particular staff in a
//     particular measure.
//

bool HumdrumInput::convertMeasureStaff(int track, int startline, int endline, int n, int layercount)
{

    m_staff = new Staff();
    m_measure->AddStaff(m_staff);
    m_staff->SetN(n);

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
    m_staff->AddLayer(m_layer);

    if (m_debug) {
        vector<int> &rkern = m_rkern;
        int staffindex = rkern[track];
        vector<HTp> &layerdata = m_layertokens[staffindex][layerindex];
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
        // some strange unexpected problem
        return false;
    }
    vector<HTp> &layerdata = m_layertokens[staffindex][layerindex];
    Layer *&layer = m_layer;

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
            m_layer->AddLayerElement(mrest);
        }
        else {
            Rest *rest = new Rest();
            m_layer->AddLayerElement(rest);
            setDuration(rest, duration);
        }
        return true;
    }

    // If the layer contains only a single rest and the rest
    // is the same duration as the time signature, then
    // create a full measure rest (mrest).
    // Deal with time/clef/key changes in measures with
    // whole-measure rests later.
    if (hasFullMeasureRest(layerdata, timesigdurs[startline], duration)) {
        MRest *mrest = new MRest();
        appendElement(layer, mrest);
        return true;
    }

    // setup beam states
    vector<int> beamstate(layerdata.size(), 0);
    int negativeQ = 0;
    for (i = 0; i < (int)beamstate.size(); i++) {
        if (!layerdata[i]->getLine()->isData()) {
            beamstate[i] = 0;
        }
        else {
            beamstate[i] = characterCount(*layerdata[i], 'L');
            beamstate[i] -= characterCount(*layerdata[i], 'J');
        }
        if (i > 0) {
            beamstate[i] += beamstate[i - 1];
        }
        if (beamstate[i] < 0) {
            negativeQ = 1;
        }
    }
    if (negativeQ || (beamstate.back() != 0)) {
        // something wrong with the beaming, either incorrect or
        // the beaming crosses a barline or layer.  Don't try to
        // beam anything.
        std::fill(beamstate.begin(), beamstate.end(), 0);
    }

    Beam *beam = NULL;
    bool turnoffbeam = false;
    for (i = 0; i < (int)layerdata.size(); i++) {
        if (prespace[i] > 0) {
            Space *space = new Space;
            if (beam != NULL) {
                appendElement(beam, space);
            }
            else {
                appendElement(layer, space);
            }
            setDuration(space, prespace[i]);
        }
        if (layerdata[i]->isNull()) {
            continue;
        }
        if (!layerdata[i]->getLine()->isData()) {
            continue;
        }
        if ((i == 0) && (beamstate[i] > 0)) {
            beam = new Beam;
            appendElement(layer, beam);
        }
        else if ((beamstate[i - 1] == 0) && (beamstate[i] > 0)) {
            beam = new Beam;
            appendElement(layer, beam);
        }
        else if (beamstate[i] == 0) {
            turnoffbeam = true;
        }
        if (layerdata[i]->isChord()) {
            Chord *chord = new Chord;
            if (beam != NULL) {
                appendElement(beam, chord);
            }
            else {
                appendElement(layer, chord);
            }
            convertChord(chord, layerdata[i]);
        }
        else if (layerdata[i]->isRest()) {
            if (layerdata[i]->find("yy") != string::npos) {
                // Invisible rest (or note which should be invisible.
                Space *irest = new Space;
                if (beam != NULL) {
                    appendElement(beam, irest);
                }
                else {
                    appendElement(layer, irest);
                }
                convertRhythm(irest, layerdata[i]);
            }
            else {
                Rest *rest = new Rest;
                if (beam != NULL) {
                    appendElement(beam, rest);
                }
                else {
                    appendElement(layer, rest);
                }
                convertRest(rest, layerdata[i]);
            }
        }
        else {
            // should be a note
            Note *note = new Note;
            if (beam != NULL) {
                appendElement(beam, note);
            }
            else {
                appendElement(layer, note);
            }
            convertNote(note, layerdata[i]);
        }

        if (turnoffbeam) {
            turnoffbeam = false;
            beam = NULL;
        }
    }

    if (prespace.size() > layerdata.size()) {
        if (prespace.back() > 0) {
            Space *space = new Space;
            if (beam != NULL) {
                appendElement(beam, space);
            }
            else {
                appendElement(layer, space);
            }
            setDuration(space, prespace.back());
        }
    }

    return true;
}

//////////////////////////////
//
// HumdrumInput::convertChord --
//

void HumdrumInput::convertChord(Chord *chord, HTp token)
{
    int scount = token->getSubtokenCount();
    for (int j = 0; j < scount; j++) {
        Note *note = new Note;
        appendElement(chord, note);
        convertNote(note, token, j);
    }

    convertRhythm(chord, token);

    // Stem direction of the chord.  If both up and down, then show up.
    if (token->find("/") != string::npos) {
        chord->SetStemDir(STEMDIRECTION_up);
    }
    else if (token->find("\\") != string::npos) {
        chord->SetStemDir(STEMDIRECTION_down);
    }
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
    }
    return true;
}

//////////////////////////////
//
// HumdrumInput::appendElement --
//

template <class PARENT, class CHILD> void HumdrumInput::appendElement(PARENT parent, CHILD child)
{
    parent->AddLayerElement(child);
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
}

/////////////////////////////
//
// HumdrumInput::convertNote --
//    default value:
//       subtoken = -1 (use the first subtoken);
//

void HumdrumInput::convertNote(Note *note, HTp token, int subtoken)
{
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

    // Add the pitch information
    int diatonic = Convert::kernToBase7(tstring);
    int octave = diatonic / 7;
    note->SetOct(octave);

    switch (diatonic % 7) {
        case 0: note->SetPname(PITCHNAME_c); break;
        case 1: note->SetPname(PITCHNAME_d); break;
        case 2: note->SetPname(PITCHNAME_e); break;
        case 3: note->SetPname(PITCHNAME_f); break;
        case 4: note->SetPname(PITCHNAME_g); break;
        case 5: note->SetPname(PITCHNAME_a); break;
        case 6: note->SetPname(PITCHNAME_b); break;
    }

    bool showInAccid = token->hasVisibleAccidental(stindex);
    bool showInAccidGes = !showInAccid;
    if (token->hasCautionaryAccidental(stindex)) {
        showInAccidGes = true;
    }

    int accidCount = Convert::kernToAccidentalCount(tstring);
    if (showInAccid) {
        switch (accidCount) {
            case +2: note->SetAccid(ACCIDENTAL_EXPLICIT_x); break;
            case +1: note->SetAccid(ACCIDENTAL_EXPLICIT_s); break;
            case 0: note->SetAccid(ACCIDENTAL_EXPLICIT_n); break;
            case -1: note->SetAccid(ACCIDENTAL_EXPLICIT_f); break;
            case -2: note->SetAccid(ACCIDENTAL_EXPLICIT_ff); break;
        }
    }
    if (showInAccidGes) {
        switch (accidCount) {
            case +2: note->SetAccidGes(ACCIDENTAL_IMPLICIT_ss); break;
            case +1: note->SetAccidGes(ACCIDENTAL_IMPLICIT_s); break;
            case 0: note->SetAccidGes(ACCIDENTAL_IMPLICIT_n); break;
            case -1: note->SetAccidGes(ACCIDENTAL_IMPLICIT_f); break;
            case -2: note->SetAccidGes(ACCIDENTAL_IMPLICIT_ff); break;
        }
    }

    if (!chordQ) {
        convertRhythm(note, token, subtoken);
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

    // handle note articulations
    printNoteArticulations(note, token, tstring);

    // handle ties
    if ((tstring.find("[") != string::npos) || (tstring.find("_") != string::npos)) {
        processTieStart(note, token, tstring);
    }

    if ((tstring.find("_") != string::npos) || (tstring.find("]") != string::npos)) {
        processTieEnd(note, token, tstring);
    }

    convertVerses(note, token, subtoken);
}

//////////////////////////////
//
// HumdrumInput::convertVerses --
//

void HumdrumInput::convertVerses(Note *note, HTp token, int subtoken)
{
    int staff = m_rkern[token->getTrack()];
    if (!m_verses[staff]) {
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
        else if (line.token(i)->isDataType("**text")) {
            versenum++;
            if (line.token(i)->isNull()) {
                continue;
            }
            Verse *verse = new Verse;
            appendElement(note, verse);
            verse->SetN(versenum);
            Syl *syl = new Syl;
            appendElement(verse, syl);
            Text *text = new Text;
            syl->AddTextElement(text);
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

            text->SetText(UTF8to16(unescapeHtmlEntities(content)));
        }
    }
}

//////////////////////////////
//
// HumdrumInput::convertRhythm --
//     default value:
//         subtoken = -1;
//

template <class ELEMENT> void HumdrumInput::convertRhythm(ELEMENT element, HTp token, int subtoken)
{
    string tstring;
    int stindex = 0;
    if (subtoken < 0) {
        tstring = *token;
    }
    else {
        tstring = token->getSubtoken(subtoken);
        stindex = subtoken;
    }

    int dotcount = characterCountInSubtoken(tstring, '.');
    if (dotcount > 0) {
        element->SetDots(dotcount);
    }

    // Tuplet durations are not handled below yet.
    // dur is in units of quarter notes.
    HumNum dur = Convert::recipToDurationNoDots(tstring);
    dur /= 4; // duration is now in whole note units;
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
// HumdrumInput::printNoteArticulations -- Print articulations for notes.
//

void HumdrumInput::printNoteArticulations(Note *note, HTp token, const string &tstring)
{
    // bool chordQ = token->isChord();
    int layer = m_currentlayer;
    if (tstring.find(";") != string::npos) {
        if ((tstring.find("yy") == string::npos) && (tstring.find(";y") == string::npos)) {
            if (layer == 1) {
                note->SetFermata(PLACE_above);
            }
            else if (layer == 2) {
                note->SetFermata(PLACE_below);
            }
            else {
                // who knows, maybe check the stem direction or see
                // if another note in a different layer already
                // has a fermata (so you would not want to overwrite them).
                note->SetFermata(PLACE_above);
            }
        }
    }
}

//////////////////////////////
//
// HumdrumInput::processTieStart --
//

void HumdrumInput::processTieStart(Note *note, HTp token, const string &tstring)
{
    HumNum timestamp = token->getDurationFromStart();
    HumNum endtime = timestamp + token->getDuration();
    int track = token->getTrack();
    int rtrack = m_rkern[track];
    string noteuuid = note->GetUuid();
    int cl = m_currentlayer;
    int pitch = Convert::kernToMidiNoteNumber(tstring);

    m_ties[rtrack].emplace_back();
    m_ties[rtrack].back().setStart(noteuuid, m_measure, cl, tstring, pitch, timestamp, endtime);
}

//////////////////////////////
//
// processTieEnd --
//

void HumdrumInput::processTieEnd(Note *note, HTp token, const string &tstring)
{
    HumNum timestamp = token->getDurationFromStart();
    int track = token->getTrack();
    int staffnum = m_rkern[track];
    string noteuuid = note->GetUuid();

    int pitch = Convert::kernToMidiNoteNumber(tstring);
    int layer = m_currentlayer;
    auto found = m_ties[staffnum].end();
    HumNum tstamp;

    // search for open tie in current layer
    for (auto it = m_ties[staffnum].begin(); it != m_ties[staffnum].end(); it++) {
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
    if (found == m_ties[staffnum].end()) {
        for (auto it = m_ties[staffnum].begin(); it != m_ties[staffnum].end(); it++) {
            if (it->getPitch() != pitch) {
                continue;
            }
            if (it->getEndTime() == timestamp) {
                found = it;
                break;
            }
        }
    }

    if (found == m_ties[staffnum].end()) {
        // can't find start of slur so give up.
        return;
    }

    found->setEndAndInsert(noteuuid, m_measure, tstring);
    if (found->isInserted()) {
        // Only deleting the finished tie if it was successful.  Undeleted
        // ones can be checked later.  They are either encoding errors, or
        // hanging ties, or arpeggiation ties (the latter should be encoded
        // with [[, ]] rather than [, ]).
        m_ties[staffnum].erase(found);
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

template <class ELEMENT> void HumdrumInput::setDuration(ELEMENT element, HumNum duration)
{
    if (duration == 3) {
        element->SetDur(DURATION_2);
        element->SetDots(1);
        return;
    }
    else if (duration == 2) {
        element->SetDur(DURATION_2);
        return;
    }
    else if (duration == 1) {
        element->SetDur(DURATION_4);
        return;
    }
    else if (duration == 4) {
        element->SetDur(DURATION_1);
        return;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 2)) {
        element->SetDur(DURATION_8);
        return;
    }
    else if ((duration.getNumerator() == 3) && (duration.getDenominator() == 2)) {
        element->SetDur(DURATION_4);
        element->SetDots(1);
        return;
    }
    else if ((duration.getNumerator() == 3) && (duration.getDenominator() == 4)) {
        element->SetDur(DURATION_8);
        element->SetDots(1);
        return;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 4)) {
        element->SetDur(DURATION_16);
        return;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 8)) {
        element->SetDur(DURATION_32);
        return;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 16)) {
        element->SetDur(DURATION_64);
        return;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 32)) {
        element->SetDur(DURATION_128);
        return;
    }
    else if ((duration.getNumerator() == 1) && (duration.getDenominator() == 64)) {
        element->SetDur(DURATION_256);
        return;
    }
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
//

void HumdrumInput::setupSystemMeasure(int startline, int endline)
{
    m_measure = new Measure();
    m_system->AddMeasure(m_measure);

    int measurenumber = getMeasureNumber(startline, endline);
    if (measurenumber >= 0) {
        m_measure->SetN(measurenumber);
    }

    setSystemMeasureStyle(startline, endline);
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
        m_measure->SetRight(BARRENDITION_rptend);
    }
    else if (endbar.find(":!!:") != string::npos) {
        m_measure->SetRight(BARRENDITION_rptend);
    }
    else if (endbar.find(":||:") != string::npos) {
        m_measure->SetRight(BARRENDITION_rptend);
    }
    else if (endbar.find(":!:") != string::npos) {
        m_measure->SetRight(BARRENDITION_rptend);
    }
    else if (endbar.find(":|:") != string::npos) {
        m_measure->SetRight(BARRENDITION_rptend);
    }
    else if (endbar.find(":|") != string::npos) {
        m_measure->SetRight(BARRENDITION_rptend);
    }
    else if (endbar.find(":!") != string::npos) {
        m_measure->SetRight(BARRENDITION_rptend);
    }
    else if (endbar.find("-") != string::npos) {
        m_measure->SetRight(BARRENDITION_invis);
    }

    if (startbar.find(":|!|:") != string::npos) {
        m_measure->SetLeft(BARRENDITION_rptstart);
    }
    else if (startbar.find(":!!:") != string::npos) {
        m_measure->SetLeft(BARRENDITION_rptstart);
    }
    else if (startbar.find(":||:") != string::npos) {
        m_measure->SetLeft(BARRENDITION_rptstart);
    }
    else if (startbar.find(":!:") != string::npos) {
        m_measure->SetLeft(BARRENDITION_rptstart);
    }
    else if (startbar.find(":|:") != string::npos) {
        m_measure->SetLeft(BARRENDITION_rptstart);
    }
    else if (startbar.find("|:") != string::npos) {
        m_measure->SetLeft(BARRENDITION_rptend);
    }
    else if (startbar.find("!:") != string::npos) {
        m_measure->SetLeft(BARRENDITION_rptend);
    }
    else if (startbar.find("-") != string::npos) {
        // This is the default state of @left, so don't specify.
        // m_measure->SetLeft(BARRENDITION_invis);
    }
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
// HumdumInput::setupMeiDocument -- Add a page and a system on the page to
//     get things started.
//

void HumdrumInput::setupMeiDocument(void)
{
    m_page = new Page();
    m_doc->AddPage(m_page);
    m_system = new System();
    m_page->AddSystem(m_system);
}

//////////////////////////////
//
// HumdrumInput::clear -- clear contents of object (m_doc handled by
//    parent class).
//

void HumdrumInput::clear(void)
{
    m_filename = "";
    m_page = NULL;
    m_system = NULL;
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
    if (sscanf(infile[startline].getTokenString(0).c_str(), "=%d", &number)) {
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
    unsigned int CharCode;
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
                else if ((ThisCh >= 'a' and ThisCh <= 'z') or (ThisCh >= 'A' and ThisCh <= 'Z')) {
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
                if ((ThisCh >= 'a' and ThisCh <= 'z') or (ThisCh >= 'A' and ThisCh <= 'Z')
                    or (ThisCh >= '0' and ThisCh <= '9')) {
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
                if (not ProcessedChar) {
                    Out.put('&');
                    for (unsigned int i = 0; i < MatchingName.size(); ++i) {
                        Out.put(MatchingName[i]);
                    } /*for*/
                    MatchState = NoMatch;
                }
                break;
            case (int)MatchNumber:
                if (ThisCh == 'x' or ThisCh == 'X') {
                    ProcessedChar = true;
                    MatchState = MatchHexNumber;
                    CharCode = 0;
                }
                else if (ThisCh >= '0' and ThisCh <= '9') {
                    CharCode = ThisCh - '0';
                    MatchState = MatchDecimalNumber;
                    ProcessedChar = true;
                }
                else {
                    MatchState = NoMatch;
                }
                break;
            case (int)MatchDecimalNumber:
                if (ThisCh >= '0' and ThisCh <= '9') {
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
                if (ThisCh >= '0' and ThisCh <= '9') {
                    CharCode = CharCode * 16 + ThisCh - '0';
                    ProcessedChar = true;
                }
                else if (ThisCh >= 'a' and ThisCh <= 'f') {
                    CharCode = CharCode * 16 + ThisCh - 'a' + 10;
                    ProcessedChar = true;
                }
                else if (ThisCh >= 'A' and ThisCh <= 'F') {
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
        else if (not ProcessedChar and MatchState == NoMatch) {
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

} // namespace vrv

#endif /* NO_HUMDRUM_SUPPORT */
