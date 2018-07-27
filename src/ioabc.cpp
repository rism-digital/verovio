/////////////////////////////////////////////////////////////////////////////
// Name:        ioabc.cpp
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Klaus Rettinghaus. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ioabc.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <sstream>
#include <string>

//----------------------------------------------------------------------------

#include "annot.h"
#include "artic.h"
#include "beam.h"
#include "chord.h"
#include "doc.h"
#include "dynam.h"
#include "editorial.h"
#include "fermata.h"
#include "harm.h"
#include "layer.h"
#include "mdiv.h"
#include "measure.h"
#include "metersig.h"
#include "mordent.h"
#include "multirest.h"
#include "note.h"
#include "pb.h"
#include "pghead.h"
#include "rend.h"
#include "rest.h"
#include "sb.h"
#include "score.h"
#include "scoredef.h"
#include "section.h"
#include "slur.h"
#include "space.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "tempo.h"
#include "text.h"
#include "tie.h"
#include "trill.h"
#include "tuplet.h"
#include "turn.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#ifndef NO_ABC_SUPPORT
#endif /* NO_ABC_SUPPORT */

namespace vrv {

#ifndef NO_ABC_SUPPORT

// Global variables:
char abcLine[10001] = { 0 };
#define MAX_DATA_LEN 1024 // One line of the abc file would not be that long!
char dataKey[MAX_DATA_LEN];
char dataValue[MAX_DATA_LEN]; // ditto as above

std::string pitch = "FCGDAEB";
std::string shorthandDecoration = ".~HLMOPSTuv";

//----------------------------------------------------------------------------
// AbcInput
//----------------------------------------------------------------------------

AbcInput::AbcInput(Doc *doc, std::string filename) : FileInputStream(doc)
{
    m_filename = filename;
    m_hasLayoutInformation = true;
}

AbcInput::~AbcInput() {}

//////////////////////////////////////////////////////////////////////////

bool AbcInput::ImportFile()
{
    std::ifstream infile;
    infile.open(m_filename.c_str());
    parseABC(infile);
    return true;
}

bool AbcInput::ImportString(std::string const &abc)
{
    std::istringstream in_stream(abc);
    parseABC(in_stream);
    return true;
}

//////////////////////////////
//
// parseABC --
//

void AbcInput::parseABC(std::istream &infile)
{
    // initialize doc
    m_doc->Reset();
    m_doc->SetType(Raw);

    // read file header
    while (abcLine[0] != 'X') {
        infile.getline(abcLine, 10000);
        ++m_lineNum;
        if (abcLine[0] == 'X') break;
        if ((abcLine[0] == '%') && (abcLine[1] == '%'))
            LogWarning("ABC input: Stylesheet directives are ignored");
        else if (abcLine[1] == ':')
            readInformationField(abcLine[0], &abcLine[2]);
    }
    CreateHeader();

    // read tune header
    readInformationField('X', &abcLine[2]);
    // create score
    assert(m_mdiv != NULL);
    Score *score = new Score();
    m_mdiv->AddChild(score);
    while (abcLine[0] != 'K') {
        infile.getline(abcLine, 10000);
        ++m_lineNum;
        readInformationField(abcLine[0], &abcLine[2]);
        if (infile.eof()) {
            LogWarning("ABC input: No music found");
            break;
        }
    }
    // add work entry to meiHead
    pugi::xpath_node meiHead = m_doc->m_header.select_single_node("meiHead");
    m_workDesc = meiHead.node().append_child("workDesc");
    CreateWorkEntry();

    // create page head
    PrintInformationFields();
    StaffGrp *staffGrp = new StaffGrp();
    m_doc->m_scoreDef.AddChild(staffGrp);
    StaffDef *staffDef = new StaffDef();
    staffDef->SetN(1);
    staffDef->SetLines(m_stafflines);
    staffGrp->AddChild(staffDef);
    if (m_meter) {
        m_doc->m_scoreDef.SetMeterCount(m_meter->GetCount());
        m_doc->m_scoreDef.SetMeterUnit(m_meter->GetUnit());
        m_doc->m_scoreDef.SetMeterSym(m_meter->GetSym());
        delete m_meter;
        m_meter = NULL;
    }
    // create section
    Section *section = new Section();
    // start with a new page
    if (m_linebreak != '\0') {
        Pb *pb = new Pb();
        pb->SetUuid(StringFormat("abcLine%02d", m_lineNum + 1));
        section->AddChild(pb);
    }
    // calculate default unit note length
    if (m_durDefault == DURATION_NONE) {
        CalcUnitNoteLength();
    }

    // read music code
    while (!infile.eof()) {
        infile.getline(abcLine, 10000);
        ++m_lineNum;
        // if (abcLine[0] == '\0') break;
        if (abcLine[0] == 'X') {
            LogDebug("ABC input: Reading only first tune in file");
            break;
        }
        else if (abcLine[0] == '%')
            continue;
        else if (abcLine[1] == ':' && abcLine[0] != '|') {
            if (abcLine[0] != 'K') {
                readInformationField(abcLine[0], &abcLine[2]);
            }
            else {
                LogWarning("ABC input: Key changes not supported", abcLine[0]);
            }
        }
        else {
            readMusicCode(abcLine, section);
        }
    }

    // add ornaments, ties, and slur
    Measure *measure = NULL;
    for (auto iter = m_controlElements.begin(); iter != m_controlElements.end(); ++iter) {
        if (!measure || (measure->GetUuid() != iter->first)) {
            measure = dynamic_cast<Measure *>(section->FindChildByUuid(iter->first));
        }
        if (!measure) {
            LogWarning("ABC input: Element '%s' could not be added to measure '%s'",
                iter->second->GetClassName().c_str(), iter->first.c_str());
            continue;
        }
        measure->AddChild(iter->second);
    }

    score->AddChild(section);

    m_doc->ConvertToPageBasedDoc();
    m_composer.clear();
    m_title.clear();
}

/**********************************
 *
 * SetBarLine
 * Translation from ABC to verovio representaion:
 *
 BARRENDITION_single     |
 BARRENDITION_end        |]
 BARRENDITION_rptboth    ::
 BARRENDITION_rptend     :|
 BARRENDITION_rptstart   |:
 BARRENDITION_dbl        ||
 */

int AbcInput::SetBarLine(const char *musicCode, Measure *measure, int i)
{
    data_BARRENDITION barLine;
    if (musicCode[i - 1] == ':')
        barLine = BARRENDITION_rptend;
    else {
        switch (musicCode[i + 1]) {
            case ':':
                barLine = BARRENDITION_rptstart;
                ++i;
                break;
            case '|':
                barLine = BARRENDITION_dbl;
                ++i;
                break;
            case ']':
                barLine = BARRENDITION_end;
                ++i;
                break;
            default: barLine = BARRENDITION_single; break;
        }
    }
    // if the measure is still empty, put the bar line on the left
    if (!m_layer->GetChildCount())
        measure->SetLeft(barLine);
    else
        measure->SetRight(barLine);
    return i;
}

void AbcInput::CalcUnitNoteLength()
{
    if (!m_doc->m_scoreDef.HasMeterUnit()
        || double(m_doc->m_scoreDef.GetMeterCount()) / double(m_doc->m_scoreDef.GetMeterUnit()) >= 0.75) {
        m_unitDur = 8;
        m_durDefault = DURATION_8;
        // m_doc->m_scoreDef.SetDurDefault(DURATION_8);
    }
    else {
        m_unitDur = 16;
        m_durDefault = DURATION_16;
        // m_doc->m_scoreDef.SetDurDefault(DURATION_16);
    }
}

void AbcInput::AddBeam()
{
    if (!m_noteStack.size()) {
        return;
    }
    else if (m_noteStack.size() == 1) {
        m_layer->AddChild(m_noteStack.back());
    }
    else {
        Beam *beam = new Beam();
        for (auto iter = m_noteStack.begin(); iter != m_noteStack.end(); ++iter) {
            beam->AddChild(*iter);
        }
        m_layer->AddChild(beam);
    }
    m_noteStack.clear();
}

void AbcInput::AddTuplet()
{
    if (!m_noteStack.size()) {
        return;
    }
    else {
        Tuplet *tuplet = new Tuplet();
        for (auto iter = m_noteStack.begin(); iter != m_noteStack.end(); ++iter) {
            tuplet->AddChild(*iter);
        }
        m_layer->AddChild(tuplet);
    }
    m_noteStack.clear();
}

void AbcInput::AddAnnot(std::string remark)
{
    // remarks
    Annot *annot = new Annot();
    Text *text = new Text();
    text->SetText(UTF8to16(remark));
    annot->AddChild(text);
    // todo: add to correct place
    m_layer->AddChild(annot);
}

void AbcInput::AddArticulation(LayerElement *element, Measure *measure)
{
    assert(element);
    assert(measure);

    Artic *artic = new Artic();
    artic->SetArtic(m_artic);
    element->AddChild(artic);

    m_artic.clear();
}

void AbcInput::AddDynamic(LayerElement *element, Measure *measure)
{
    assert(element);
    assert(measure);

    for (auto it = m_dynam.begin(); it != m_dynam.end(); ++it) {
        Dynam *dynam = new Dynam();
        dynam->SetStartid("#" + element->GetUuid());
        Text *text = new Text();
        text->SetText(UTF8to16(*it));
        dynam->AddChild(text);
        m_controlElements.push_back(std::make_pair(measure->GetUuid(), dynam));
    }

    m_dynam.clear();
}

void AbcInput::AddFermata(LayerElement *element, Measure *measure)
{
    assert(element);
    assert(measure);

    Fermata *fermata = new Fermata();
    fermata->SetStartid("#" + element->GetUuid());
    fermata->GetPlaceAlternate()->SetBasic(m_fermata);
    m_controlElements.push_back(std::make_pair(measure->GetUuid(), fermata));

    m_fermata = STAFFREL_basic_NONE;
}

void AbcInput::AddOrnaments(LayerElement *element, Measure *measure)
{
    assert(element);
    assert(measure);

    std::string refId = "#" + element->GetUuid();
    // note->SetOrnam(m_ornam);
    if (m_ornam.find("m") != std::string::npos) {
        Mordent *mordent = new Mordent();
        mordent->SetStartid(refId);
        mordent->SetForm(mordentLog_FORM_lower);
        m_controlElements.push_back(std::make_pair(measure->GetUuid(), mordent));
    }
    if (m_ornam.find("M") != std::string::npos) {
        Mordent *mordent = new Mordent();
        mordent->SetStartid(refId);
        mordent->SetForm(mordentLog_FORM_upper);
        m_controlElements.push_back(std::make_pair(measure->GetUuid(), mordent));
    }
    if (m_ornam.find("s") != std::string::npos) {
        Turn *turn = new Turn();
        turn->SetStartid(refId);
        turn->SetForm(turnLog_FORM_lower);
        m_controlElements.push_back(std::make_pair(measure->GetUuid(), turn));
    }
    if (m_ornam.find("S") != std::string::npos) {
        Turn *turn = new Turn();
        turn->SetStartid(refId);
        turn->SetForm(turnLog_FORM_upper);
        m_controlElements.push_back(std::make_pair(measure->GetUuid(), turn));
    }
    if (m_ornam.find("T") != std::string::npos) {
        Trill *trill = new Trill();
        trill->SetStartid(refId);
        m_controlElements.push_back(std::make_pair(measure->GetUuid(), trill));
    }

    m_ornam.clear();
}

void AbcInput::AddTie(Measure *measure)
{
    assert(measure);

    if (!m_tieStack.empty()) {
        LogWarning("ABC input: '%s' already tied", m_ID.c_str());
        return;
    }
    Tie *tie = new Tie();
    tie->SetStartid(m_ID);
    m_tieStack.push_back(tie);
    m_controlElements.push_back(std::make_pair(measure->GetUuid(), tie));
}

void AbcInput::StartSlur(Measure *measure)
{
    assert(measure);

    Slur *openSlur = new Slur();
    m_slurStack.push_back(openSlur);
    m_controlElements.push_back(std::make_pair(measure->GetUuid(), openSlur));
}

void AbcInput::EndSlur()
{
    if (!m_slurStack.empty()) {
        if (!m_slurStack.back()->HasStartid()) {
            LogError("ABC input: Empty slur found");
            m_slurStack.pop_back();
            return;
        }
        for (auto riter = m_slurStack.rbegin(); riter != m_slurStack.rend(); ++riter) {
            if (((*riter)->GetStartid()).find(m_ID) == std::string::npos) {
                (*riter)->SetEndid("#" + m_ID);
                m_slurStack.erase((riter + 1).base());
                break;
            }
        }
        return;
    }
    LogWarning("ABC input: Closing slur for element '%s' could not be matched", m_ID.c_str());
}

void AbcInput::parseDecoration(std::string decorationString)
{
    // shorthand decorations hard-coded !
    if (isdigit(decorationString[0])) {
        LogWarning("ABC input: Fingering not supported", decorationString.c_str());
        return;
    }
    if (!strcmp(decorationString.c_str(), "."))
        m_artic.push_back(ARTICULATION_stacc);
    else if (!strcmp(decorationString.c_str(), "trill") || !strcmp(decorationString.c_str(), "T"))
        m_ornam.push_back('T');
    else if (!strcmp(decorationString.c_str(), "mordent") || !strcmp(decorationString.c_str(), "lowermordent")
        || !strcmp(decorationString.c_str(), "M"))
        m_ornam.push_back('m');
    else if (!strcmp(decorationString.c_str(), "pralltriller") || !strcmp(decorationString.c_str(), "uppermordent")
        || !strcmp(decorationString.c_str(), "P"))
        m_ornam.push_back('M');
    else if (!strcmp(decorationString.c_str(), "turn"))
        m_ornam.push_back('S');
    else if (!strcmp(decorationString.c_str(), "invertedturn"))
        m_ornam.push_back('s');
    else if (!strcmp(decorationString.c_str(), ">"))
        m_artic.push_back(ARTICULATION_acc);
    else if (!strcmp(decorationString.c_str(), "accent"))
        m_artic.push_back(ARTICULATION_acc);
    else if (!strcmp(decorationString.c_str(), "emphasis"))
        m_artic.push_back(ARTICULATION_acc);
    else if (!strcmp(decorationString.c_str(), "fermata") || !strcmp(decorationString.c_str(), "H"))
        m_fermata = STAFFREL_basic_above;
    else if (!strcmp(decorationString.c_str(), "invertedfermata"))
        m_fermata = STAFFREL_basic_below;
    else if (!strcmp(decorationString.c_str(), "tenuto"))
        m_artic.push_back(ARTICULATION_ten);
    else if (!strcmp(decorationString.c_str(), "+"))
        m_artic.push_back(ARTICULATION_stop);
    else if (!strcmp(decorationString.c_str(), "plus"))
        m_artic.push_back(ARTICULATION_stop);
    else if (!strcmp(decorationString.c_str(), "snap"))
        m_artic.push_back(ARTICULATION_snap);
    else if (!strcmp(decorationString.c_str(), "upbow") || !strcmp(decorationString.c_str(), "u"))
        m_artic.push_back(ARTICULATION_upbow);
    else if (!strcmp(decorationString.c_str(), "downbow") || !strcmp(decorationString.c_str(), "v"))
        m_artic.push_back(ARTICULATION_dnbow);
    else if (!strcmp(decorationString.c_str(), "open"))
        m_artic.push_back(ARTICULATION_open);
    else if (!strcmp(decorationString.c_str(), "pppp") || !strcmp(decorationString.c_str(), "ppp")
        || !strcmp(decorationString.c_str(), "pp") || !strcmp(decorationString.c_str(), "p")
        || !strcmp(decorationString.c_str(), "mp") || !strcmp(decorationString.c_str(), "mf")
        || !strcmp(decorationString.c_str(), "f") || !strcmp(decorationString.c_str(), "ff")
        || !strcmp(decorationString.c_str(), "ffff") || !strcmp(decorationString.c_str(), "ffff")
        || !strcmp(decorationString.c_str(), "sfz"))
        m_dynam.push_back(decorationString);
    else
        LogWarning("ABC input: Decoration %s not supported", decorationString.c_str());
}

//////////////////////////////
//
// parse information fields
//

void AbcInput::parseInstruction(std::string instruction)
{
    if (!strncmp(instruction.c_str(), "abc-include", 11)) {
        LogWarning("ABC input: Include field is ignored");
    }
    else if (!strncmp(instruction.c_str(), "linebreak", 9)) {
        if (instruction.find("<none>") != std::string::npos) {
            m_linebreak = '\0';
            m_hasLayoutInformation = false;
        }
        else {
            m_linebreak = '$';
            m_hasLayoutInformation = true;
            LogWarning("ABC input: Default linebreaks are used for now.");
        }
    }
    else if (!strncmp(instruction.c_str(), "decoration", 10)) {
        m_decoration = instruction[11];
    }
}

void AbcInput::parseKey(std::string keyString)
{
    int i = 0;
    short int accidNum = 0;
    data_MODE mode = MODE_NONE;
    while (isspace(keyString[i])) ++i;

    // set key.pname
    if (pitch.find(keyString[i]) != std::string::npos) {
        accidNum = int(pitch.find(keyString[i])) - 1;
        keyString[i] = tolower(keyString[i]);
        m_doc->m_scoreDef.SetKeyPname((m_doc->m_scoreDef).AttKeySigDefaultLog::StrToPitchname(keyString.substr(i, 1)));
        ++i;
    }
    while (isspace(keyString[i])) ++i;

    // set key.accid
    switch (keyString[i]) {
        case '#':
            m_doc->m_scoreDef.SetKeyAccid(ACCIDENTAL_GESTURAL_s);
            accidNum += 7;
            ++i;
            break;
        case 'b':
            m_doc->m_scoreDef.SetKeyAccid(ACCIDENTAL_GESTURAL_f);
            accidNum -= 7;
            ++i;
            break;
        default: break;
    }

    // set key.mode
    if (m_doc->m_scoreDef.HasKeyPname()) {
        // when no mode is indicated, major is assumed
        mode = MODE_major;
        while (isspace(keyString[i])) ++i;

        if (keyString[i]) {
            std::string modeString(&keyString[i]);
            // capitalisation is ignored for the modes
            // and in fact only the first three letters of each mode are parsed
            modeString = modeString.substr(0, 3);
            for (char &c : modeString) {
                c = tolower(c);
            }

            if (modeString[0] == 'm' && modeString[2] != 'x') { // should mean "min"
                mode = MODE_minor;
                accidNum -= 3;
            }
            else if (modeString == "ion") {
                mode = MODE_dorian;
            }
            else if (modeString == "dor") {
                mode = MODE_dorian;
                accidNum -= 2;
            }
            else if (modeString == "phr") {
                mode = MODE_phrygian;
                accidNum -= 4;
            }
            else if (modeString == "lyd") {
                mode = MODE_lydian;
                accidNum += 1;
            }
            else if (modeString == "mix") {
                mode = MODE_mixolydian;
                accidNum -= 1;
            }
            else if (modeString == "aeo") {
                mode = MODE_aeolian;
                accidNum -= 3;
            }
            else if (modeString == "loc") {
                mode = MODE_locrian;
                accidNum -= 5;
            }
        }
    }
    m_doc->m_scoreDef.SetKeyMode(mode);

    // we need set @key.sig for correct rendering
    if (accidNum != 0) {
        std::string keySig;
        if (accidNum < 0)
            keySig = StringFormat("%df", abs(accidNum));
        else if (accidNum > 0)
            keySig = StringFormat("%ds", accidNum);
        m_doc->m_scoreDef.SetKeySig((m_doc->m_scoreDef).AttKeySigDefaultLog::StrToKeysignature(keySig));
    }

    // set clef
    // <clef name> - may be treble, alto, tenor, bass, perc or none. perc selects the drum clef. clef= may be omitted.
    // [<line number>] - indicates on which staff line the base clef is written. Defaults are: treble: 2; alto: 3;
    // tenor: 4; bass: 4.
    // [+8 | -8] - draws '8' above or below the staff. The player will transpose the notes one octave higher or lower.
    if (keyString.find("clef") != std::string::npos) LogWarning("ABC input: 'clef' is not supported yet.");

    // for now only default treble clef
    m_doc->m_scoreDef.SetClefShape(CLEFSHAPE_G);
    m_doc->m_scoreDef.SetClefLine(2);

    if (keyString.find("transpose=", i) != std::string::npos) {
        i = int(keyString.find("transpose=", i)) + 10;
        std::string transStr;
        int trans = 1;
        if (keyString[i] == '-') {
            trans = -1;
            ++i;
        }
        while (isdigit(keyString[i])) {
            transStr.push_back(keyString[i]);
            ++i;
        }
        trans = trans * atoi(transStr.c_str());
    }

    // stafflines
    if (keyString.find("stafflines=", i) != std::string::npos) {
        int pos = keyString.find("stafflines=", i) + 11;
        m_stafflines = atoi(keyString.substr(pos, keyString.find_first_not_of("0123456789", pos)).c_str());
    }
}

void AbcInput::parseUnitNoteLength(std::string unitNoteLength)
{
    if (unitNoteLength.find('/'))
        m_unitDur = atoi(&unitNoteLength[unitNoteLength.find('/') + 1]);
    else if (atoi(unitNoteLength.c_str()) == 1)
        m_unitDur = 1;
    switch (m_unitDur) {
        case 1: m_durDefault = DURATION_1; break;
        case 2: m_durDefault = DURATION_2; break;
        case 4: m_durDefault = DURATION_4; break;
        case 8: m_durDefault = DURATION_8; break;
        case 16: m_durDefault = DURATION_16; break;
        case 32: m_durDefault = DURATION_32; break;
        case 64: m_durDefault = DURATION_64; break;
        case 128: m_durDefault = DURATION_128; break;
        case 256: m_durDefault = DURATION_256; break;
        default: break;
    }
    // m_doc->m_scoreDef.SetDurDefault(m_durDefault);
}

void AbcInput::parseMeter(std::string meterString)
{
    m_meter = new MeterSig();
    if (meterString.find('C') != std::string::npos) {
        if (meterString[meterString.find('C') + 1] == '|') {
            m_meter->SetSym(METERSIGN_cut);
            m_meter->SetCount(2);
            m_meter->SetUnit(2);
        }
        else {
            m_meter->SetSym(METERSIGN_common);
            m_meter->SetCount(4);
            m_meter->SetUnit(4);
        }
    }
    else if (meterString.find('/')) {
        std::string meterCount = meterString.substr(0, meterString.find('/'));
        if (meterCount.front() == '(' && meterCount.back() == ')')
            meterCount = meterCount.substr(1, meterCount.length() - 1);
        // this is a little "hack", until libMEI is fixed
        m_meter->SetCount(atoi(meterCount.c_str()));
        m_meter->SetUnit(atoi(&meterString[meterString.find('/') + 1]));
    }
}

void AbcInput::parseTempo(std::string tempoString)
{
    Tempo *tempo = new Tempo();
    if (tempoString.find('\"') != std::string::npos) {
        std::string tempoWord = tempoString.substr(tempoString.find('\"') + 1);
        tempoWord = tempoWord.substr(0, tempoWord.find('\"'));
        Text *text = new Text();
        text->SetText(UTF8to16(tempoWord));
        tempo->AddChild(text);
        // this has to be fixed
        tempo->SetTstamp(1);
    }
    m_tempoStack.push_back(tempo);
    LogWarning("ABC input: Tempo definitions are not fully supported yet");
}

void AbcInput::parseReferenceNumber(std::string referenceNumberString)
{
    // The X: field is also used to indicate the start of the tune
    m_mdiv = new Mdiv();
    m_mdiv->m_visibility = Visible;
    if (!referenceNumberString.empty()) {
        int mdivNum = atoi(referenceNumberString.c_str());
        if (mdivNum < 1) {
            LogError("ABC input: reference number should be a positive integer");
        }
        m_mdiv->SetN(std::to_string(mdivNum));
    }
    m_doc->AddChild(m_mdiv);

    // reset information fields
    m_composer.clear();
    m_history.clear();
    m_notes.clear();
    m_origin.clear();
    m_title.clear();
}

void AbcInput::PrintInformationFields()
{
    PgHead *pgHead = new PgHead();
    for (auto it = m_title.begin(); it != m_title.end(); ++it) {
        Rend *titleRend = new Rend();
        titleRend->SetHalign(HORIZONTALALIGNMENT_center);
        titleRend->SetValign(VERTICALALIGNMENT_middle);
        if (it != m_title.begin()) {
            data_FONTSIZE fontsize;
            fontsize.SetTerm(FONTSIZETERM_small);
            titleRend->SetFontsize(fontsize);
        }
        Text *text = new Text();
        text->SetText(UTF8to16(it->first));
        titleRend->AddChild(text);
        pgHead->AddChild(titleRend);
    }
    for (auto it = m_composer.begin(); it != m_composer.end(); ++it) {
        Rend *compRend = new Rend();
        compRend->SetHalign(HORIZONTALALIGNMENT_right);
        compRend->SetValign(VERTICALALIGNMENT_bottom);
        Text *composer = new Text();
        composer->SetText(UTF8to16(it->first));
        compRend->AddChild(composer);
        if (!m_origin.empty()) {
            Text *origin = new Text();
            origin->SetText(UTF8to16(" (" + m_origin.front().first + ")"));
            compRend->AddChild(origin);
        }
        pgHead->AddChild(compRend);
    }
    m_doc->m_scoreDef.AddChild(pgHead);
}

void AbcInput::CreateHeader()
{
    pugi::xml_node meiHead = m_doc->m_header.append_child("meiHead");

    // <fileDesc> //
    pugi::xml_node fileDesc = meiHead.append_child("fileDesc");
    pugi::xml_node fileTitleStmt = fileDesc.append_child("titleStmt");
    pugi::xml_node fileTitle = fileTitleStmt.append_child("title");
    fileTitle.text().set(m_filename.c_str());
    if (!m_composer.empty()) {
        for (auto it = m_composer.begin(); it != m_composer.end(); ++it) {
            pugi::xml_node composer = fileTitleStmt.append_child("composer");
            composer.text().set((it->first).c_str());
            composer.append_attribute("xml:id").set_value(StringFormat("abcLine%02d", it->second).c_str());
            composer.append_attribute("analog").set_value("abc:C");
        }
    }

    pugi::xml_node pubStmt = fileDesc.append_child("pubStmt");
    pubStmt.append_child(pugi::node_pcdata);

    // <notesStmt> //
    if (!m_notes.empty()) {
        pugi::xml_node notes = fileDesc.append_child("notesStmt");
        for (auto it = m_notes.begin(); it != m_notes.end(); ++it) {
            pugi::xml_node annot = notes.append_child("annot");
            annot.text().set((it->first).c_str());
            annot.append_attribute("xml:id").set_value(StringFormat("abcLine%02d", it->second).c_str());
            annot.append_attribute("analog").set_value("abc:N");
        }
    }

    // <encodingDesc> //
    pugi::xml_node encodingDesc = meiHead.append_child("encodingDesc");
    pugi::xml_node appInfo = encodingDesc.append_child("appInfo");
    pugi::xml_node app = appInfo.append_child("application");
    pugi::xml_node appName = app.append_child("name");
    appName.append_child(pugi::node_pcdata).set_value("Verovio");
    pugi::xml_node appText = app.append_child("p");
    appText.append_child(pugi::node_pcdata).set_value("Transcoded from abc music");

    // isodate and version //
    time_t t = time(0); // get time now
    struct tm *now = localtime(&t);
    std::string dateStr = StringFormat("%d-%02d-%02dT%02d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1,
        now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
    app.append_attribute("isodate").set_value(dateStr.c_str());
    app.append_attribute("version").set_value(GetVersion().c_str());
}

void AbcInput::CreateWorkEntry()
{
    // <work> //
    pugi::xml_node work = m_workDesc.append_child("work");
    work.append_attribute("n").set_value(m_mdiv->GetN().c_str());
    work.append_attribute("data").set_value(StringFormat("#%s", m_mdiv->GetUuid().c_str()).c_str());
    for (auto it = m_title.begin(); it != m_title.end(); ++it) {
        pugi::xml_node title = work.append_child("title");
        title.text().set((it->first).c_str());
        title.append_attribute("xml:id").set_value(StringFormat("abcLine%02d", it->second).c_str());
        title.append_attribute("analog").set_value("abc:T");
        if (it == m_title.begin()) {
            title.append_attribute("type").set_value("main");
        }
        else {
            title.append_attribute("type").set_value("alternative");
        }
    }
    if (!m_composer.empty()) {
        for (auto it = m_composer.begin(); it != m_composer.end(); ++it) {
            pugi::xml_node composer = work.append_child("composer");
            composer.text().set((it->first).c_str());
            composer.append_attribute("xml:id").set_value(StringFormat("abcLine%02d", it->second).c_str());
            composer.append_attribute("analog").set_value("abc:C");
        }
    }
    if (!m_history.empty()) {
        pugi::xml_node history = work.append_child("history");
        history.append_attribute("analog").set_value("abc:H");
        for (auto it = m_history.begin(); it != m_history.end(); ++it) {
            pugi::xml_node histLine = history.append_child("p");
            histLine.text().set((it->first).c_str());
            histLine.append_attribute("xml:id").set_value(StringFormat("abcLine%02d", it->second).c_str());
        }
    }
    if (!m_notes.empty()) {
        pugi::xml_node notes = work.append_child("notesStmt");
        for (auto it = m_notes.begin(); it != m_notes.end(); ++it) {
            pugi::xml_node annot = notes.append_child("annot");
            annot.text().set((it->first).c_str());
            annot.append_attribute("xml:id").set_value(StringFormat("abcLine%02d", it->second).c_str());
            annot.append_attribute("analog").set_value("abc:N");
        }
    }
}

//////////////////////////////
//
// readInformationField --
// information fields always
// start with a letter
// followed by a single colon
//

void AbcInput::readInformationField(char dataKey, std::string value)
{
    // remove comments and trim
    if (dataKey == '%' || dataKey == '\0')
        return;
    else if (value.find('%') != std::string::npos) {
        value = value.substr(0, value.find('%'));
    }
    while (isspace(value[value.length() - 1])) value.pop_back();
    if (value.empty()) return;
    while (isspace(value[0])) value = value.substr(1);

    if (dataKey == 'C') {
        m_composer.push_back(std::make_pair(value, m_lineNum));
    }
    else if (dataKey == 'H') {
        m_history.push_back(std::make_pair(value, m_lineNum));
    }
    else if (dataKey == 'I') {
        parseInstruction(value);
    }
    else if (dataKey == 'K') {
        parseKey(value);
    }
    else if (dataKey == 'L') {
        parseUnitNoteLength(value);
    }
    else if (dataKey == 'M') {
        parseMeter(value);
    }
    else if (dataKey == 'N') {
        m_notes.push_back(std::make_pair(value, m_lineNum));
    }
    else if (dataKey == 'O') {
        m_origin.push_back(std::make_pair(value, m_lineNum));
    }
    else if (dataKey == 'Q') {
        parseTempo(value);
    }
    else if (dataKey == 'T') {
        m_title.push_back(std::make_pair(value, m_lineNum));
    }
    else if (dataKey == 'U') {
        LogWarning("ABC input: User defined sympols are not supported");
    }
    else if (dataKey == 'V') {
        LogWarning("ABC input: Multi-voice music is not supported");
    }
    else if (dataKey == 'W') {
        LogWarning("ABC input: Lyrics are not supported yet");
    }
    else if (dataKey == 'X') {
        parseReferenceNumber(value);
    }
    else
        LogWarning("ABC input: Information field %c is ignored", dataKey);
}

//////////////////////////////
//
// readMusicCode --
// parse abc music code
//

void AbcInput::readMusicCode(const char *musicCode, Section *section)
{
    assert(section);

    int i = 0;
    bool sysBreak = true;

    data_GRACE grace = GRACE_NONE;
    Chord *chord = NULL;

    if (strlen(musicCode) < 1) return;

    Measure *measure = new Measure();
    Staff *staff = new Staff();
    m_layer = new Layer();
    m_layer->SetN(1);

    while (i < int(strlen(musicCode))) {
        // eat the input...

        if (musicCode[i] == '`') {
            // keeps a beam
        }
        if (isspace(musicCode[i])) {
            // always ends a beam
            AddBeam();
        }

        // comments
        else if (musicCode[i] == '%') {
            break;
        }

        // endings
        else if (musicCode[i] == '[' && isdigit(musicCode[i + 1])) {
            ++i;
            // Ending *ending = new Ending;
            // ending->SetN(musicCode[i]);
            ++i;
        }

        // inline fields
        else if (musicCode[i] == '[' && musicCode[i + 2] == ':') {
            ++i;
            char dataKey = musicCode[i];
            ++i;
            ++i;
            std::string information;
            while (musicCode[i] != ']') {
                information.push_back(musicCode[i]);
                ++i;
            }
            if (dataKey == 'r')
                AddAnnot(information);
            else
                readInformationField(dataKey, information);
        }

        // linebreaks
        else if (musicCode[i] == m_linebreak) {
            Sb *sb = new Sb();
            section->AddChild(sb);
        }

        // decorations
        else if (shorthandDecoration.find(musicCode[i]) != std::string::npos) {
            std::string shorthandDecorationString;
            shorthandDecorationString.push_back(musicCode[i]);
            parseDecoration(shorthandDecorationString);
        }
        else if (musicCode[i] == m_decoration) {
            ++i;
            if (!isspace(musicCode[i])) {
                std::string decorationString;
                while (musicCode[i] != m_decoration) {
                    decorationString.push_back(musicCode[i]);
                    ++i;
                }
                parseDecoration(decorationString);
            }
        }

        // tuplets
        else if (musicCode[i] == '(' && isdigit(musicCode[i + 1])) {
            LogWarning("ABC input: Tuplets not supported yet");
            // AddTuplet();
        }

        // slurs and ties
        else if (musicCode[i] == '(') {
            StartSlur(measure);
        }
        else if (musicCode[i] == ')') {
            EndSlur();
        }
        else if (musicCode[i] == '-') {
            AddTie(measure);
        }

        // chords
        else if (musicCode[i] == '[' && musicCode[i + 1] != '|') {
            // start chord
            chord = new Chord();

            // add articulation
            if (!m_artic.empty()) {
                AddArticulation(chord, measure);
            }

            // add dynamics
            if (!m_dynam.empty()) {
                AddDynamic(chord, measure);
            }

            // add fermata
            if (m_fermata != STAFFREL_basic_NONE) {
                AddFermata(chord, measure);
            }
        }
        else if (musicCode[i] == ']' && musicCode[i - 1] != '|') {
            // end chord
            if (chord->GetDur() < DURATION_8) {
                // if chord cannot be beamed, write it directly to the layer
                if (m_noteStack.size() > 0) AddBeam();
                m_layer->AddChild(chord);
            }
            else
                m_noteStack.push_back(chord);
            chord = NULL;
        }

        // grace notes
        else if ((musicCode[i] == '{') || (musicCode[i] == '}')) {
            // !to be refined when graceGrp is added!
            // start grace group
            if ((musicCode[i] == '{')) {
                grace = GRACE_acc;
                if ((musicCode[i + 1] == '/')) {
                    grace = GRACE_unacc;
                    ++i;
                }
            }
            // end grace group
            else {
                if ((m_gracecount > 1) || (grace == GRACE_unacc)) AddBeam();
                grace = GRACE_NONE;
                m_gracecount = 0;
            }
        }

        // note
        else if (pitch.find(toupper(musicCode[i])) != std::string::npos) {
            int oct = 0;
            Note *note = new Note;
            m_ID = note->GetUuid();

            // accidentals
            if (musicCode[i - 1] == '^' || musicCode[i - 1] == '=' || musicCode[i - 1] == '_') {
                // todo: double sharps and double flats
                Accid *accid = new Accid();
                switch (musicCode[i - 1]) {
                    case '^': accid->SetAccid(ACCIDENTAL_WRITTEN_s); break;
                    case '=': accid->SetAccid(ACCIDENTAL_WRITTEN_n); break;
                    case '_': accid->SetAccid(ACCIDENTAL_WRITTEN_f); break;
                    default: break;
                }
                note->AddChild(accid);
            }

            // set pitch name
            if (isupper(musicCode[i]))
                oct = 4;
            else
                oct = 5;
            note->SetPname(note->AttPitch::StrToPitchname(std::string(1, tolower(musicCode[i]))));

            // set octave
            while (musicCode[i + 1] == '\'' || musicCode[i + 1] == ',') {
                if (musicCode[i + 1] == ',')
                    oct -= 1;
                else
                    oct += 1;
                ++i;
            }
            note->SetOct(oct);

            // set duration
            std::string numStr, numbaseStr;
            int dots = 0;
            int numbase = 1;
            if ((m_broken < 0) && (grace == GRACE_NONE)) {
                dots = -m_broken;
                m_broken = 0;
            }
            while (musicCode[i + 1] == '>') {
                ++i;
                ++m_broken;
                ++dots;
            }
            while (musicCode[i + 1] == '<') {
                ++i;
                --m_broken;
            }
            while (isdigit(musicCode[i + 1])) {
                ++i;
                numStr.push_back(musicCode[i]);
            }
            while (musicCode[i + 1] == '/') {
                ++i;
                numbase *= 2;
            }
            while (isdigit(musicCode[i + 1])) {
                ++i;
                numbaseStr.push_back(musicCode[i]);
            }
            int num = (numStr.empty()) ? 1 : std::atoi(numStr.c_str());
            numbase = (numbaseStr.empty()) ? numbase : std::atoi(numbaseStr.c_str());
            while ((num & (num - 1)) != 0) {
                ++dots;
                // won't work for num > 12
                num = num - num / 3;
            }
            if ((numbase & (numbase - 1)) != 0) LogError("ABC input: note length divider must be power of 2");
            int dur = (num == 0) ? 4 : m_unitDur * numbase / num;
            data_DURATION meiDur
                = (dur == 0) ? DURATION_breve : note->AttDurationLogical::StrToDuration(std::to_string(dur));

            // set grace
            if (grace != GRACE_NONE) {
                ++m_gracecount;
                note->SetGrace(grace);
                // "The unit duration to use for gracenotes is not specified by the abc file"
                // setting it to an eighth by default for now
                note->SetDur(DURATION_8);
                if (grace == GRACE_unacc) note->SetStemMod(STEMMODIFIER_1slash);
            }

            // add articulation
            if (!m_artic.empty()) {
                AddArticulation(note, measure);
            }

            // add dynamics
            if (!m_dynam.empty()) {
                AddDynamic(note, measure);
            }

            // add fermata
            if (m_fermata != STAFFREL_basic_NONE) {
                AddFermata(note, measure);
            }

            // add ornaments
            if (!m_ornam.empty()) {
                AddOrnaments(note, measure);
            }

            if (chord) {
                chord->AddChild(note);
                if (!chord->HasDur()) {
                    if (dots > 0) chord->SetDots(dots);
                    if (num == 0) chord->SetStemVisible(BOOLEAN_false);
                    chord->SetDur(meiDur);
                }
            }
            else {
                if (dots > 0) note->SetDots(dots);
                if (num == 0) note->SetStemVisible(BOOLEAN_false);
                if ((m_broken < 0) && (grace == GRACE_NONE)) {
                    for (int i = 0; i != -m_broken; ++i) dur = dur * 2;
                }
                else if (!note->HasDots() && (m_broken > 0) && (grace == GRACE_NONE)) {
                    for (; m_broken != 0; --m_broken) dur = dur * 2;
                }
                note->SetDur(meiDur);
                if (note->GetDur() < DURATION_8) {
                    // if note cannot be beamed, write it directly to the layer
                    if (m_noteStack.size() > 0) AddBeam();
                    m_layer->AddChild(note);
                }
                else
                    m_noteStack.push_back(note);
            }
            // there should always only be one element in the harmony stack
            if (!m_harmStack.empty() && !m_harmStack.back()->HasStartid()) {
                m_harmStack.back()->SetStartid("#" + m_ID);
                m_harmStack.clear();
            }
            if (!m_tieStack.empty()) {
                m_tieStack.back()->SetEndid("#" + m_ID);
                m_tieStack.clear();
            }
            for (auto it = m_slurStack.begin(); it != m_slurStack.end(); ++it) {
                if (!((*it)->HasStartid())) {
                    (*it)->SetStartid("#" + m_ID);
                }
            }
        }

        // spaces
        else if (musicCode[i] == 'x') {
            Space *space = new Space();
            m_ID = space->GetUuid();

            // set duration
            std::string numStr, numbaseStr;
            int dots = 0;
            int numbase = 1;
            if (musicCode[i + 1] == '>') {
                ++i;
                LogWarning("ABC input: Broken rhythms not supported");
            }
            while (isdigit(musicCode[i + 1])) {
                ++i;
                numStr.push_back(musicCode[i]);
            }
            if (musicCode[i + 1] == '/') {
                ++i;
                while (isdigit(musicCode[i + 1])) {
                    ++i;
                    numbaseStr.push_back(musicCode[i]);
                }
            }
            int num = (numStr.empty()) ? 1 : std::atoi(numStr.c_str());
            numbase = (numbaseStr.empty()) ? numbase : std::atoi(numbaseStr.c_str());
            while ((num & (num - 1)) != 0) {
                ++dots;
                // won't work for num > 12
                num = num - num / 3;
            }
            if ((numbase & (numbase - 1)) != 0) LogError("ABC input: note length divider must be power of 2");
            if (dots > 0) space->SetDots(dots);
            space->SetDur(space->AttDurationLogical::StrToDuration(std::to_string(m_unitDur * numbase / num)));

            m_noteStack.push_back(space);
        }

        // padding
        else if (musicCode[i] == 'y') {
            // Pad *pad = new Pad;
            LogWarning("ABC input: Extra space not supported");
        }

        // rests
        else if (musicCode[i] == 'z') {
            Rest *rest = new Rest();
            m_ID = rest->GetUuid();

            // add Fermata
            if (m_fermata != STAFFREL_basic_NONE) {
                AddFermata(rest, measure);
            }

            // set duration
            std::string numStr, numbaseStr;
            int dots = 0;
            int numbase = 1;
            if ((m_broken < 0) && (grace == GRACE_NONE)) {
                dots = -m_broken;
                m_broken = 0;
            }
            while (musicCode[i + 1] == '>') {
                ++i;
                ++m_broken;
                ++dots;
            }
            while (musicCode[i + 1] == '<') {
                ++i;
                --m_broken;
            }
            while (isdigit(musicCode[i + 1])) {
                ++i;
                numStr.push_back(musicCode[i]);
            }
            while (musicCode[i + 1] == '/') {
                ++i;
                numbase *= 2;
            }
            while (isdigit(musicCode[i + 1])) {
                ++i;
                numbaseStr.push_back(musicCode[i]);
            }
            int num = (numStr.empty()) ? 1 : std::atoi(numStr.c_str());
            numbase = (numbaseStr.empty()) ? numbase : std::atoi(numbaseStr.c_str());
            while ((num & (num - 1)) != 0) {
                ++dots;
                // won't work for num > 12
                num = num - num / 3;
            }
            if ((numbase & (numbase - 1)) != 0) LogError("ABC input: note length divider must be power of 2");
            int dur = m_unitDur * numbase / num;
            data_DURATION meiDur
                = (dur == 0) ? DURATION_breve : rest->AttDurationLogical::StrToDuration(std::to_string(dur));

            if (dots > 0) rest->SetDots(dots);
            if ((m_broken < 0) && (grace == GRACE_NONE)) {
                for (int i = 0; i != -m_broken; ++i) dur = dur * 2;
            }
            else if (!rest->HasDots() && (m_broken > 0) && (grace == GRACE_NONE)) {
                for (; m_broken != 0; --m_broken) dur = dur * 2;
            }
            rest->SetDur(meiDur);

            m_noteStack.push_back(rest);
        }

        // multi-measure rests
        else if (musicCode[i] == 'Z') {
            MultiRest *multiRest = new MultiRest();
            std::string numString;
            while (isdigit(musicCode[i + 1])) {
                numString.push_back(musicCode[i + 1]);
                ++i;
            }
            multiRest->SetNum(atoi(numString.c_str()));
            m_layer->AddChild(multiRest);
        }

        // text elements
        else if (musicCode[i] == '\"') {
            ++i;
            if (musicCode[i] == '^' || musicCode[i] == '_' || musicCode[i] == '<' || musicCode[i] == '>'
                || musicCode[i] == '@') {
                LogWarning("ABC input: Annotations are not fully support yet");
                ++i;
            }
            std::string chordSymbol;
            while (musicCode[i] != '\"') {
                chordSymbol.push_back(musicCode[i]);
                ++i;
            }
            Harm *harm = new Harm();
            Text *text = new Text();
            text->SetText(UTF8to16(chordSymbol));
            harm->AddChild(text);
            m_harmStack.push_back(harm);
            m_controlElements.push_back(std::make_pair(measure->GetUuid(), harm));
        }

        // suppressing score line-breaks
        else if (musicCode[i] == '\\') {
            sysBreak = false;
        }

        // barLine
        else if (musicCode[i] == '|') {
            // add stacked elements to layer
            AddBeam();
            i = SetBarLine(musicCode, measure, i);
            if (m_layer->GetChildCount()) {
                staff->AddChild(m_layer);
                measure->AddChild(staff);
                for (auto it = m_tempoStack.begin(); it != m_tempoStack.end(); ++it) {
                    measure->AddChild(*it);
                }
                m_tempoStack.clear();
                section->AddChild(measure);
                measure = new Measure();
                staff = new Staff();
                m_layer = new Layer();
            }
            else {
                if (musicCode[i + 1] == ':') measure->SetLeft(BARRENDITION_rptstart);
            }
        }

        ++i;

        // check if there is a change in meter
        if (m_meter) {
            // todo: apply meter changes to staves
            ScoreDef *scoreDef = new ScoreDef();
            scoreDef->SetMeterCount(m_meter->GetCount());
            scoreDef->SetMeterUnit(m_meter->GetUnit());
            scoreDef->SetMeterSym(m_meter->GetSym());
            section->AddChild(scoreDef);
            delete m_meter;
            m_meter = NULL;
        }
    }

    // by default, line-breaks in the code generate line-breaks in the typeset score
    // has to be refined later
    if (sysBreak && (m_linebreak != '\0')) {
        Sb *sb = new Sb();
        sb->SetUuid(StringFormat("abcLine%02d", m_lineNum + 1));
        section->AddChild(sb);
    }

    // in case of not empty layer
    if (m_layer->GetChildCount()) {
        staff->AddChild(m_layer);
        measure->AddChild(staff);
        section->AddChild(measure);
    }
    else {
        delete staff;
        delete measure;
        delete m_layer;
        m_layer = NULL;
    }
}

#endif /* NO_ABC_SUPPORT */

} // namespace vrv
