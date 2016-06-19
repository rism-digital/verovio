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
#include "doc.h"
#include "iomei.h"
#include "layer.h"
#include "measure.h"
#include "mrest.h"
#include "note.h"
#include "page.h"
#include "rest.h"
#include "staff.h"
#include "system.h"
#include "vrv.h"

//#include "attcomparison.h"
//#include "chord.h"
//#include "mrest.h"
//#include "slur.h"
//#include "syl.h"
//#include "text.h"
//#include "tie.h"
//#include "tuplet.h"
//#include "verse.h"
//#include "pugixml.hpp"

using namespace hum; // humlib  namespace
using namespace vrv; // verovio namespace
using namespace pugi; // pugixml namespace
using namespace std;

namespace vrv {

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
        m_doc->Reset(Raw);
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
        m_doc->Reset(Raw);
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

    for (i = sigdurs.size() - 2; i >= 0; i--) {
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
    HumdrumFile &infile = m_infile;
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
    HumdrumFile &infile = m_infile;
    const vector<HTp> &kernstarts = m_kernstarts;
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

    return convertMeasureStaves(startline, endline);
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

    int i, j, k;
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
    HumdrumFile &infile = m_infile;
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
    HumdrumFile &infile = m_infile;
    m_layer = new Layer();
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

    HumNum starttime = infile[startline].getDurationFromStart();
    HumNum endtime = infile[endline].getDurationFromStart() + infile[endline].getDuration();
    HumNum duration = endtime - starttime;

    /* Why not allowed?
    if (timesigdurs[startline] != duration) {
            m_measure->SetMetcon(BOOLEAN_false);
    }
    */

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

    if (1 == 0) {
        cout << "BEAMSTATE: ";
        for (i = 0; i < (int)beamstate.size(); i++) {
            cout << beamstate[i] << " ";
        }
        cout << endl;
    }

    Layer *&layer = m_layer;
    Beam *beam = NULL;
    int spacecount = 0;
    bool turnoffbeam = false;
    for (i = 0; i < (int)layerdata.size(); i++) {
        if (layerdata[i]->isNull()) {
            continue;
        }
        if (!layerdata[i]->getLine()->isData()) {
            continue;
        }
        if ((i == 0) && (beamstate[i] > 0)) {
            beam = new Beam;
            layer->AddLayerElement(beam);
        }
        else if ((beamstate[i - 1] == 0) && (beamstate[i] > 0)) {
            beam = new Beam;
            layer->AddLayerElement(beam);
        }
        else if (beamstate[i] == 0) {
            turnoffbeam = true;
        }
        spacecount = characterCount(layerdata[i], ' ');
        if (spacecount) {
            // if (beam != NULL) {
            // 	insertChord(beam, layerdata[i]);
            // } else {
            // 		insertChord(layer, layerdata[i]);
            //	}
        }
        else {
            if (beam != NULL) {
                if (layerdata[i]->isRest()) {
                    insertRestInBeam(beam, layerdata[i]);
                }
                else {
                    insertNoteInBeam(beam, layerdata[i]);
                }
            }
            else {
                if (layerdata[i]->isRest()) {
                    insertRestInLayer(layer, layerdata[i]);
                }
                else {
                    insertNoteInLayer(layer, layerdata[i]);
                }
            }
        }
        if (turnoffbeam) {
            turnoffbeam = false;
            beam = NULL;
        }
    }

    return true;
}

/*
//////////////////////////////
//
// HumdrumInput::insertChord --
//

void HumdrumInput::insertChord(Layer *element, HTp token) {
    // do nothing for now until testing an import of a file
        // containing chords.
}
*/

//////////////////////////////
//
// HumdrumInput::insertNoteOrRest --
//

void HumdrumInput::insertNoteInLayer(Layer *element, HTp token)
{
    Note *note = new Note;
    element->AddLayerElement(note);
    convertNote(note, token);
}

void HumdrumInput::insertNoteInBeam(Beam *element, HTp token)
{
    Note *note = new Note;
    element->AddLayerElement(note);
    convertNote(note, token);
}

void HumdrumInput::insertRestInLayer(Layer *element, HTp token)
{
    Rest *rest = new Rest;
    element->AddLayerElement(rest);
}

void HumdrumInput::insertRestInBeam(Beam *element, HTp token)
{
    Rest *rest = new Rest;
    element->AddLayerElement(rest);
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
    int dotcount = characterCount(tstring, '.');
    if (dotcount > 0) {
        note->SetDots(dotcount);
    }

    if (tstring.find("/") != string::npos) {
        note->SetStemDir(STEMDIRECTION_up);
    }
    else if (tstring.find("\\") != string::npos) {
        note->SetStemDir(STEMDIRECTION_down);
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

    // Tuplet durations are not handled below yet.
    // dur is in units of quarter notes.
    HumNum dur = Convert::recipToDurationNoDots(tstring);
    dur /= 4; // duration is now in whole note units;
    if (dur.isInteger()) {
        switch (dur.getNumerator()) {
            case 1: note->SetDur(DURATION_1); break;
            case 2: note->SetDur(DURATION_breve); break;
            case 4: note->SetDur(DURATION_long); break;
            case 8: note->SetDur(DURATION_maxima); break;
        }
    }
    else if (dur.getNumerator() == 1) {
        switch (dur.getDenominator()) {
            case 2: note->SetDur(DURATION_2); break;
            case 4: note->SetDur(DURATION_4); break;
            case 8: note->SetDur(DURATION_8); break;
            case 16: note->SetDur(DURATION_16); break;
            case 32: note->SetDur(DURATION_32); break;
            case 64: note->SetDur(DURATION_64); break;
            case 128: note->SetDur(DURATION_128); break;
            case 256: note->SetDur(DURATION_256); break;
            case 512: note->SetDur(DURATION_512); break;
            case 1024: note->SetDur(DURATION_1024); break;
            case 2048: note->SetDur(DURATION_2048); break;
        }
    }
}

/////////////////////////////
//
// HumdrumInput::characterCount --
//

int HumdrumInput::characterCount(const string &text, char symbol)
{
    return std::count(text.begin(), text.end(), symbol);
}

int HumdrumInput::characterCount(HTp token, char symbol)
{
    return std::count(token->begin(), token->end(), symbol);
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
    for (i = 0; i < lt.size(); i++) {
        cerr << "STAFF " << i + 1 << "\t";
        for (j = 0; j < lt[i].size(); j++) {
            cerr << "LAYER " << j + 1 << ":\t";
            for (k = 0; k < lt[i][j].size(); k++) {
                cout << " " << *lt[i][j][k];
            }
            cerr << endl;
        }
    }
}

/////////////////////////////
//
// HumdrumInput::setDuration --  Incoming duration is in quarter notes.
//

void HumdrumInput::setDuration(Rest *rest, HumNum duration)
{
    if (duration == 3) {
        rest->SetDur(DURATION_2);
        rest->SetDots(1);
    }
    else if (duration == 2) {
        rest->SetDur(DURATION_2);
    }
    else if (duration == 1) {
        rest->SetDur(DURATION_4);
    }
    else if (duration == 4) {
        rest->SetDur(DURATION_1);
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

    int i, j;
    for (i = 0; i < (int)lt.size(); i++) {
        output[i] = lt[i].size();
    }

    return output;
}

//////////////////////////////
//
// HumdrumInput::setupSystemMeasure -- prepare a new system measure.
//

void HumdrumInput::setupSystemMeasure(int startline, int endline)
{
    HumdrumFile &infile = m_infile;

    m_measure = new Measure();
    m_system->AddMeasure(m_measure);

    if (1 == 0) {
        string comment = "startline: ";
        comment += to_string(startline);
        comment += ", endline: ";
        comment += to_string(endline);
        m_measure->SetComment(comment);
    }

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
    const vector<HTp> &kernstarts = m_kernstarts;

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
        m_measure->SetLeft(BARRENDITION_invis);
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
    m_doc->CastOff();
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

} // namespace vrv

#endif /* NO_HUMDRUM_SUPPORT */
