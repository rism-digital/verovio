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

#include "artic.h"
#include "beam.h"
#include "chord.h"
#include "clef.h"
#include "doc.h"
#include "editorial.h"
#include "harm.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "metersig.h"
#include "multirest.h"
#include "note.h"
#include "rest.h"
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
#include "trill.h"
#include "tuplet.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#ifndef NO_ABC_SUPPORT
#include <regex>
#endif /* NO_ABC_SUPPORT */

namespace vrv {

#ifndef NO_ABC_SUPPORT

// Global variables:
char abcLine[10001] = { 0 };
#define MAX_DATA_LEN 1024 // One line of the abc file would not be that long!
char dataKey[MAX_DATA_LEN];
char dataValue[MAX_DATA_LEN]; // ditto as above

std::string pitch = "abcdefgABCDEFG";
std::string shorthandDecoration = ".~HLMOPSTuv";

//----------------------------------------------------------------------------
// AbcInput
//----------------------------------------------------------------------------

AbcInput::AbcInput(Doc *doc, std::string filename)
    : // This is pretty bad. We open a bad fileoinputstream as we don't use it
    FileInputStream(doc)
{
    m_filename = filename;
    m_staff = NULL;
    m_measure = NULL;
    m_layer = NULL;
    m_last_tied_note = NULL;
    m_is_in_chord = false;
}

AbcInput::~AbcInput()
{
}

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
    // buffers
    std::string music;

    std::string s_key;
    abc::Measure current_measure;
    Clef *staffDefClef = NULL;

    std::vector<abc::Measure> staff;

    // start with minimal MEI
    m_doc->SetType(Raw);
    Score *score = m_doc->CreateScoreBuffer();
    // the section
    Section *section = new Section();
    score->AddChild(section);

    // find first tune
    while (abcLine[0] != 'X') {
        if (abcLine[0] == '%') {
            // LogWarning("comment: %s", section);
        }
        infile.getline(abcLine, 10000);
        m_lineNum++;
    }
    // read tune header
    readInformationField('X', &abcLine[2], score);
    while (abcLine[0] != 'K') {
        infile.getline(abcLine, 10000);
        m_lineNum++;
        readInformationField(abcLine[0], &abcLine[2], score);
    }
    // calculate default unit note length
    if (m_durDefault == DURATION_NONE) {
        calcUnitNoteLength();
    }
    // read music code
    while (!infile.eof()) {
        infile.getline(abcLine, 10000);
        m_lineNum++;
        if (abcLine[0] == 'X' && m_doc->m_scoreDef.HasType()) {
            LogDebug("Reading only first tune in file");
            break;
        }
        else if (abcLine[1] == ':' && abcLine[0] != '|') {
            LogWarning("ABC input: Information fields in music code not supported");
            break;
        }
        else {
            readMusicCode(abcLine, section);
        }
    }

    // we need to add the last measure if it has no barLine at the end
    if (current_measure.notes.size() != 0) {
        // current_measure.barLine = "=-";
        staff.push_back(current_measure);
        current_measure.notes.clear();
    }

    int measure_count = 1;

    std::vector<abc::Measure>::iterator it;
    for (it = staff.begin(); it < staff.end(); it++) {

        m_staff = new Staff(1);
        m_measure = new Measure(true, measure_count);

        m_staff->AddChild(m_layer);
        m_measure->AddChild(m_staff);

        abc::Measure obj = *it;

        section->AddChild(m_measure);

        convertMeasure(&obj);
        measure_count++;
    }

    // add minimal scoreDef
    StaffGrp *staffGrp = new StaffGrp();
    StaffDef *staffDef = new StaffDef();
    staffDef->SetN(1);
    staffDef->SetLines(5);
    if (staffDefClef) {
        staffDef->SetClefShape(staffDefClef->GetShape());
        staffDef->SetClefLine(staffDefClef->GetLine());
        staffDef->SetClefDis(staffDefClef->GetDis());
        staffDef->SetClefDisPlace(staffDefClef->GetDisPlace());
        delete staffDefClef;
    }
    staffGrp->AddChild(staffDef);
    m_doc->m_scoreDef.AddChild(staffGrp);

    m_doc->ConvertToPageBasedDoc();
}

/**********************************
 *
 * getBarLine -- read the barLine.
 * Translation from ABC to verovio representaion:
 *
 BARRENDITION_single     |
 BARRENDITION_end        |]
 BARRENDITION_rptboth    ::
 BARRENDITION_rptend     :|
 BARRENDITION_rptstart   |:
 BARRENDITION_dbl        ||
 */

int AbcInput::getBarLine(const char *music, data_BARRENDITION *output, int index)
{

    bool is_barline_rptboth = false;
    bool is_barline_rptend = false;
    bool is_barline_rptstart = false;
    bool is_barline_dbl = false;

    if (strncmp(music + index, ":||:", 4) == 0) {
        is_barline_rptboth = true;
    }

    if (strncmp(music + index, ":|", 2) == 0) {
        is_barline_rptend = true;
    }

    if (strncmp(music + index, "|:", 2) == 0) {
        is_barline_rptstart = true;
    }

    if (strncmp(music + index, "||", 2) == 0) {
        is_barline_dbl = true;
    }

    int i = 0; // number of characters
    if (is_barline_rptboth) {
        *output = BARRENDITION_rptboth;
        i = 3;
    }
    else if (is_barline_rptstart) {
        *output = BARRENDITION_rptstart;
        i = 2;
    }
    else if (is_barline_rptend) {
        *output = BARRENDITION_rptend;
        i = 2;
    }
    else if (is_barline_dbl) {
        *output = BARRENDITION_dbl;
        i = 1;
    }
    else {
        *output = BARRENDITION_single;
        i = 0;
    }
    return i;
}

//////////////////////////////
//
// convertMeasure --
//

void AbcInput::convertMeasure(abc::Measure *measure)
{
    if (measure->clef != NULL) {
        m_layer->AddChild(measure->clef);
    }

    m_layerElements.clear();

    // Set barLine
    m_measure->SetRight(measure->barLine);
}

void AbcInput::calcUnitNoteLength()
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

void AbcInput::pushContainer(LayerElement *container)
{
    addLayerElement(container);
    m_layerElements.push_back(container);
}

void AbcInput::popContainer()
{
    // assert(m_layerElements.size() > 0);
    if (m_layerElements.size() == 0) {
        LogError("ABC input: tried to pop an object from empty stack. "
                 "Cross-measure objects (tuplets, beams) are not supported.");
    }
    else {
        m_layerElements.pop_back();
    }
}

void AbcInput::addLayerElement(LayerElement *element)
{
    if (m_layerElements.size() > 0) {
        m_layerElements.back()->AddChild(element);
    }
    else {
        m_layer->AddChild(element);
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
        std::vector<LayerElement *>::iterator iter;
        for (iter = m_noteStack.begin(); iter != m_noteStack.end(); iter++) {
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
        std::vector<LayerElement *>::iterator iter;
        for (iter = m_noteStack.begin(); iter != m_noteStack.end(); iter++) {
            tuplet->AddChild(*iter);
        }
        m_layer->AddChild(tuplet);
    }
    m_noteStack.clear();
}

void AbcInput::parseDecoration(std::string decorationString)
{
    if (atoi(decorationString.c_str())) {
        LogWarning("ABC input: Fingering not supported", decorationString.c_str());
        return;
    }
    if (!strcmp(decorationString.c_str(), "."))
        m_artic.push_back(ARTICULATION_stacc);
    else if (!strcmp(decorationString.c_str(), ">"))
        m_artic.push_back(ARTICULATION_acc);
    else if (!strcmp(decorationString.c_str(), "accent"))
        m_artic.push_back(ARTICULATION_acc);
    else if (!strcmp(decorationString.c_str(), "emphasis"))
        m_artic.push_back(ARTICULATION_acc);
    else if (!strcmp(decorationString.c_str(), "tenuto"))
        m_artic.push_back(ARTICULATION_ten);
    else if (!strcmp(decorationString.c_str(), "+"))
        m_artic.push_back(ARTICULATION_stop);
    else if (!strcmp(decorationString.c_str(), "plus"))
        m_artic.push_back(ARTICULATION_stop);
    else if (!strcmp(decorationString.c_str(), "snap"))
        m_artic.push_back(ARTICULATION_snap);
    else if (!strcmp(decorationString.c_str(), "upbow"))
        m_artic.push_back(ARTICULATION_upbow);
    else if (!strcmp(decorationString.c_str(), "downbow"))
        m_artic.push_back(ARTICULATION_dnbow);
    else if (!strcmp(decorationString.c_str(), "open"))
        m_artic.push_back(ARTICULATION_open);
    else
        LogWarning("ABC input: Decoration '%s' not supported", decorationString.c_str());
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
    else if (!strncmp(instruction.c_str(), "linebreak", 8)) {
        LogWarning("ABC input: Default linebreak is used for now.");
    }
    else if (!strncmp(instruction.c_str(), "decoration", 10)) {
        m_decoration = instruction[11];
    }
}

void AbcInput::parseKey(std::string keyString)
{
    int i = 0;
    data_MODE mode = MODE_NONE;
    while (isspace(keyString[i])) i++;

    // set key.pname
    if (pitch.find(keyString[i]) != std::string::npos) {
        keyString[i] = tolower(keyString[i]);
        m_doc->m_scoreDef.SetKeyPname((m_doc->m_scoreDef).AttKeySigDefaultLog::StrToPitchname(keyString.substr(i, 1)));
        i += 1;
    }
    while (isspace(keyString[i])) i++;

    // set key.accid
    switch (keyString[i]) {
        case '#':
            m_doc->m_scoreDef.SetKeyAccid(ACCIDENTAL_GESTURAL_s);
            i += 1;
            break;
        case 'b':
            m_doc->m_scoreDef.SetKeyAccid(ACCIDENTAL_GESTURAL_f);
            i += 1;
            break;
        default: break;
    }

    // set key.mode
    if (m_doc->m_scoreDef.HasKeyPname()) {
        // when no mode is indicated, major is assumed
        mode = MODE_major;
        while (keyString[i] == ' ') i += 1;

        if (keyString[i]) {
            std::string modeString(&keyString[i]);
            // capitalisation is ignored for the modes
            // and in fact only the first three letters of each mode are parsed
            modeString = modeString.substr(0, 3);
            for (char &c : modeString) {
                c = tolower(c);
            }

            if (modeString == "min" || modeString == "m")
                mode = MODE_minor;
            else if (modeString == "dor")
                mode = MODE_dorian;
            else if (modeString == "phr")
                mode = MODE_phrygian;
            else if (modeString == "lyd")
                mode = MODE_lydian;
            else if (modeString == "mix")
                mode = MODE_mixolydian;
            else if (modeString == "aeo")
                mode = MODE_aeolian;
            else if (modeString == "mix")
                mode = MODE_locrian;
        }
    }
    m_doc->m_scoreDef.SetKeyMode(mode);

    // set clef
    // <clef name> - may be treble, alto, tenor, bass, perc or none. perc selects the drum clef. clef= may be omitted.
    // [<line number>] - indicates on which staff line the base clef is written. Defaults are: treble: 2; alto: 3;
    // tenor: 4; bass: 4.
    // [+8 | -8] - draws '8' above or below the staff. The player will transpose the notes one octave higher or lower.

    // for now only default treble clef
    m_doc->m_scoreDef.SetClefShape(CLEFSHAPE_G);
    m_doc->m_scoreDef.SetClefLine(2);

    if (keyString.find("transpose=", i) != std::string::npos) {
        i = int(keyString.find("transpose=", i)) + 10;
        std::string transStr;
        int trans = 1;
        if (keyString[i] == '-') {
            trans = -1;
            i++;
        }
        while (isdigit(keyString[i])) {
            transStr.push_back(keyString[i]);
            i++;
        }
        trans = trans * atoi(transStr.c_str());
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
    if (meterString.find('C') != std::string::npos) {
        if (meterString[meterString.find('C') + 1] == '|') {
            m_doc->m_scoreDef.SetMeterSym(METERSIGN_cut);
            m_doc->m_scoreDef.SetMeterCount(2);
            m_doc->m_scoreDef.SetMeterUnit(2);
        }
        else {
            m_doc->m_scoreDef.SetMeterSym(METERSIGN_common);
            m_doc->m_scoreDef.SetMeterCount(4);
            m_doc->m_scoreDef.SetMeterUnit(4);
        }
    }
    else if (meterString.find('/')) {
        std::string meterCount = meterString.substr(0, meterString.find('/'));
        if (meterCount.front() == '(' && meterCount.back() == ')')
            meterCount = meterCount.substr(1, meterCount.length() - 1);
        // this is a little "hack", until libMEI is fixed
        m_doc->m_scoreDef.SetMeterCount(atoi(meterCount.c_str()));
        m_doc->m_scoreDef.SetMeterUnit(atoi(&meterString[meterString.find('/') + 1]));
    }
}

void AbcInput::parseTempo(std::string tempoString)
{
    Tempo *tempo = new Tempo;
    m_controlElements.push_back(tempo);
    LogWarning("ABC input: tempo definitions are not supported yet");
}

void AbcInput::parseReferenceNumber(std::string referenceNumberString)
{
    // as it is not possible to SetN on mdiv we put it on scoreDef for now
    m_doc->m_scoreDef.SetType(("X:" + referenceNumberString).c_str());
}

//////////////////////////////
//
// readInformationField --
// information fields always
// start with a letter
// followed by a single colon
//

void AbcInput::readInformationField(char dataKey, std::string value, Score *score)
{
    assert(score);

    // remove comments
    if (dataKey == '%')
        return;
    else if (value.find('%') != std::string::npos) {
        value = value.substr(0, value.find('%'));
        while (isspace(value[value.length() - 1])) value.pop_back();
    }

    if (dataKey == 'I') {
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
    else if (dataKey == 'Q') {
        parseTempo(value);
    }
    else if (dataKey == 'X') {
        score->SetN(value);
        // re-adding dataKey for complete string
        parseReferenceNumber(value);
    }
    else
        LogWarning("ABC input: information field %c is ignored", dataKey);
}

//////////////////////////////
//
// readMusicCode --
// parse abc music code
//

void AbcInput::readMusicCode(const char *musicCode, Section *section)
{
    assert(section);

    int length = int(strlen(musicCode));
    int i = 0;
    bool sysBreak = true;

    data_GRACE grace = GRACE_NONE;
    Chord *chord = NULL;

    if (length == 0) return;

    Measure *measure = new Measure();
    Staff *staff = new Staff();
    m_layer = new Layer();
    m_layer->SetN(1);

    while (i < length) {
        // eat the input...

        if (musicCode[i] == '`') {
            // keeps a beam
        }
        if (isspace(musicCode[i])) {
            // always ends a beam
            AddBeam();
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
            i++;
            std::string decorationString;
            while (musicCode[i] != m_decoration) {
                decorationString.push_back(musicCode[i]);
                i++;
            }
            parseDecoration(decorationString);
        }

        // slurs are read when adding the note
        else if (musicCode[i] == '(' || musicCode[i] == ')') {
        }

        // chords
        else if (musicCode[i] == '[' && !isdigit(musicCode[i + 1]) && musicCode[i + 2] != ':') {
            // start chord
            chord = new Chord();
        }
        else if (musicCode[i] == ']' && musicCode[i - 1] != '|') {
            // end chord
            m_noteStack.push_back(chord);
            chord = NULL;
        }

        // grace notes
        else if ((musicCode[i] == '\{') || (musicCode[i] == '}')) {
            // !to be refined when graceGrp is added!
            // start grace group
            if ((musicCode[i] == '\{')) {
                grace = GRACE_acc;
                if ((musicCode[i + 1] == '/')) {
                    grace = GRACE_unacc;
                    i++;
                }
            }
            // end grace group
            else
                grace = GRACE_NONE;
        }

        // note
        // getNote also creates a new note object
        else if (pitch.find(musicCode[i]) != std::string::npos) {
            int oct = 0;
            Note *note = new Note;

            // accidentals
            if (musicCode[i - 1] == '^' || musicCode[i - 1] == '=' || musicCode[i - 1] == '_') {
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
                i++;
            }
            note->SetOct(oct);

            // set duration
            std::string numStr, numbaseStr;
            int dots = 0;
            if (musicCode[i + 1] == '>') {
                i++;
                LogWarning("ABC input: Broken rhythms not supported");
            }
            while (isdigit(musicCode[i + 1])) {
                i++;
                numStr.push_back(musicCode[i]);
            }
            if (musicCode[i + 1] == '/') {
                i++;
                while (isdigit(musicCode[i + 1])) {
                    i++;
                    numbaseStr.push_back(musicCode[i]);
                }
            }
            int num = (numStr.empty()) ? 1 : std::atoi(numStr.c_str());
            int numbase = (numbaseStr.empty()) ? 1 : std::atoi(numbaseStr.c_str());
            while ((num & (num - 1)) != 0) {
                dots++;
                // won't work for num > 12
                num = num - num / 3;
            }
            if ((numbase & (numbase - 1)) != 0) LogError("ABC input: note length divider must be power of 2");

            // set grace
            if (grace != GRACE_NONE) {
                note->SetGrace(grace);
                // "The unit duration to use for gracenotes is not specified by the abc file"
                // setting it to an eighth by default for now
                note->SetDur(DURATION_8);
            }

            // add articulation
            if (!m_artic.empty()) {
                Artic *artic = new Artic();
                artic->SetArtic(m_artic);
                note->AddChild(artic);
                m_artic.clear();
            }

            if (chord) {
                chord->AddChild(note);
                if (!chord->HasDur()) {
                    chord->SetDots(dots);
                    chord->SetDur(note->AttDurationLogical::StrToDuration(std::to_string(m_unitDur * numbase / num)));
                }
            }
            else {
                note->SetDots(dots);
                note->SetDur(note->AttDurationLogical::StrToDuration(std::to_string(m_unitDur * numbase / num)));
                if (note->GetDur() < DURATION_8) {
                    // if note cannot beamed, write it directly to the layer
                    if (m_noteStack.size() > 0) AddBeam();
                    m_layer->AddChild(note);
                }
                else
                    m_noteStack.push_back(note);
            }
            if (!m_harmStack.empty() && !m_harmStack.back()->HasStartid()) {
                m_harmStack.back()->SetStartid("#" + note->GetUuid());
            }
        }

        // Spaces
        else if (musicCode[i] == 'x') {
            Space *space = new Space();

            // set duration
            std::string numStr, numbaseStr;
            int dots = 0;
            if (musicCode[i + 1] == '>') {
                i++;
                LogWarning("ABC input: Broken rhythms not supported");
            }
            while (isdigit(musicCode[i + 1])) {
                i++;
                numStr.push_back(musicCode[i]);
            }
            if (musicCode[i + 1] == '/') {
                i++;
                while (isdigit(musicCode[i + 1])) {
                    i++;
                    numbaseStr.push_back(musicCode[i]);
                }
            }
            int num = (numStr.empty()) ? 1 : std::atoi(numStr.c_str());
            int numbase = (numbaseStr.empty()) ? 1 : std::atoi(numbaseStr.c_str());
            while ((num & (num - 1)) != 0) {
                dots++;
                // won't work for num > 12
                num = num - num / 3;
            }
            if ((numbase & (numbase - 1)) != 0) LogError("ABC input: note length divider must be power of 2");
            space->SetDots(dots);
            space->SetDur(space->AttDurationLogical::StrToDuration(std::to_string(m_unitDur * numbase / num)));

            m_noteStack.push_back(space);
        }

        // Padding
        else if (musicCode[i] == 'y') {
            // Pad *pad = new Pad;
        }

        // Rests
        else if (musicCode[i] == 'z') {
            Rest *rest = new Rest();

            // set duration
            std::string numStr, numbaseStr;
            int dots = 0;
            if (musicCode[i + 1] == '>') {
                i++;
                LogWarning("ABC input: Broken rhythms not supported");
            }
            while (isdigit(musicCode[i + 1])) {
                i++;
                numStr.push_back(musicCode[i]);
            }
            if (musicCode[i + 1] == '/') {
                i++;
                while (isdigit(musicCode[i + 1])) {
                    i++;
                    numbaseStr.push_back(musicCode[i]);
                }
            }
            int num = (numStr.empty()) ? 1 : std::atoi(numStr.c_str());
            int numbase = (numbaseStr.empty()) ? 1 : std::atoi(numbaseStr.c_str());
            while ((num & (num - 1)) != 0) {
                dots++;
                // won't work for num > 12
                num = num - num / 3;
            }
            if ((numbase & (numbase - 1)) != 0) LogError("ABC input: note length divider must be power of 2");
            rest->SetDots(dots);
            rest->SetDur(rest->AttDurationLogical::StrToDuration(std::to_string(m_unitDur * numbase / num)));

            m_noteStack.push_back(rest);
        }

        // Multi-measure rests
        else if (musicCode[i] == 'Z') {
            MultiRest *multiRest = new MultiRest();
            std::string numString;
            while (isdigit(musicCode[i + 1])) {
                numString.push_back(musicCode[i + 1]);
                i++;
            }
            multiRest->SetNum(atoi(numString.c_str()));
            m_layer->AddChild(multiRest);
        }

        // text elements
        else if (musicCode[i] == '\"') {
            i++;
            Harm *harm = new Harm();
            std::string chordSymbol;
            while (musicCode[i] != '\"') {
                chordSymbol.push_back(musicCode[i]);
                i++;
            }
            Text *text = new Text();
            text->SetText(UTF8to16(chordSymbol));
            harm->AddChild(text);
            m_harmStack.push_back(harm);
        }

        // suppressing score line-breaks
        else if (musicCode[i] == '\\') {
            sysBreak = false;
        }

        // comments
        else if (musicCode[i] == '%') {
            break;
        }

        // remarks
        else if (musicCode[i] == 'r') {
            i += 2;
            Annot *annot = new Annot();
            std::string remark;
            while (musicCode[i] != ']') {
                remark.push_back(musicCode[i]);
                i++;
            }
            Text *text = new Text();
            text->SetText(UTF8to16(remark));
            annot->AddChild(text);
            if (chord) {
                chord->AddChild(annot);
            }
            else {
                // will not work within beams
                m_layer->AddChild(annot);
            }
        }

        // barLine
        else if (musicCode[i] == '|') {
            // add stacked elements to layer
            AddBeam();
            if (musicCode[i + 1] == ':')
                measure->SetLeft(BARRENDITION_rptstart);
            else {
                if (musicCode[i - 1] == ':')
                    measure->SetRight(BARRENDITION_rptend);
                else if (musicCode[i + 1] == '|') {
                    i++;
                    measure->SetRight(BARRENDITION_dbl);
                }
                else if (musicCode[i + 1] == ']') {
                    i++;
                    measure->SetRight(BARRENDITION_end);
                }
                else
                    measure->SetRight(BARRENDITION_single);
                staff->AddChild(m_layer);
                measure->AddChild(staff);
                if (!m_harmStack.empty()) {
                    std::vector<Harm *>::iterator iter;
                    for (iter = m_harmStack.begin(); iter != m_harmStack.end(); iter++) {
                        measure->AddChild(*iter);
                    }
                    m_harmStack.clear();
                }
                section->AddChild(measure);
                measure = new Measure();
                staff = new Staff();
                m_layer = new Layer();
            }
        }

        i++;
    }

    // in case of not empty layer
    if (m_layer->GetChildCount()) {
        staff->AddChild(m_layer);
        measure->AddChild(staff);
        section->AddChild(measure);
    }

    // by default, line-breaks in the code generate line-breaks in the typeset score
    // has to be refined later
    if (sysBreak) {
        Sb *sb = new Sb();
        section->AddChild(sb);
    }
}

#endif /* NO_ABC_SUPPORT */

} // namespace vrv
