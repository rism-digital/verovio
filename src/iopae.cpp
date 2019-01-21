/////////////////////////////////////////////////////////////////////////////
// Name:        iopae.cpp
// Author:      Rodolfo Zitellini // this is the important stuff!
// Created:     2012
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iopae.h"

//----------------------------------------------------------------------------

#ifndef NO_PAE_SUPPORT

#include <assert.h>
#include <regex>
#include <sstream>
#include <string>

#endif /* NO_PAE_SUPPORT */

//----------------------------------------------------------------------------

#include "beam.h"
#include "chord.h"
#include "clef.h"
#include "doc.h"
#include "dot.h"
#include "fermata.h"
#include "keysig.h"
#include "layer.h"
#include "mdiv.h"
#include "measure.h"
#include "metersig.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "rest.h"
#include "score.h"
#include "scoredef.h"
#include "section.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "tie.h"
#include "trill.h"
#include "tuplet.h"
#include "vrv.h"

//----------------------------------------------------------------------------

namespace vrv {

#ifndef NO_PAE_SUPPORT

#define BEAM_INITIAL 0x01
#define BEAM_MEDIAL 0x02
#define BEAM_TUPLET 0x03
#define BEAM_TERMINAL 0x04

// User interface variables:
int debugQ = 0; // used with --debug option
int stdoutQ = 0;
char outdir[1024] = { 0 }; // used with -d option
char extension[1024] = { 0 }; // used with -e option
char hum2abc[1024] = { 0 }; // used with -a option
int quietQ = 0; // used with -q option
int quiet2Q = 0; // used with -Q option

// Global variables:
char data_line[10001] = { 0 };
#define MAX_DATA_LEN 1024 // One line of the pae file would not be that long!
char data_key[MAX_DATA_LEN];
char data_value[MAX_DATA_LEN]; // ditto as above

//----------------------------------------------------------------------------
// PaeInput
//----------------------------------------------------------------------------

PaeInput::PaeInput(Doc *doc, std::string filename)
    : // This is pretty bad. We open a bad fileoinputstream as we don't use it
    FileInputStream(doc)
{
    m_filename = filename;
    m_staff = NULL;
    m_measure = NULL;
    m_layer = NULL;
    m_tie = NULL;
    m_is_in_chord = false;
    m_is_mensural = false;
}

PaeInput::~PaeInput() {}

//////////////////////////////////////////////////////////////////////////

bool PaeInput::ImportFile()
{
    std::ifstream infile;
    infile.open(m_filename.c_str());
    parsePlainAndEasy(infile);
    return true;
}

bool PaeInput::ImportString(std::string const &pae)
{
    std::istringstream in_stream(pae);
    parsePlainAndEasy(in_stream);
    return true;
}

//////////////////////////////
//
// parsePlainAndEasy --
//

void PaeInput::parsePlainAndEasy(std::istream &infile)
{
    // buffers
    char c_clef[1024] = { 0 };
    char c_key[1024] = { 0 };
    char c_keysig[1024] = { 0 };
    char c_timesig[1024] = { 0 };
    char c_alttimesig[1024] = { 0 };
    char incipit[10001] = { 0 };
    int in_beam = 0;

    std::string s_key;
    pae::Measure current_measure;
    pae::Note current_note;
    Clef *staffDefClef = NULL;
    MeterSig *scoreDefMeterSig = NULL;
    Mensur *scoreDefMensur = NULL;
    KeySig *scoreDefKeySig = NULL;

    std::vector<pae::Measure> staff;

    // read values
    while (!infile.eof()) {
        infile.getline(data_line, 10000);
        if (infile.eof()) {
            // LogDebug("Truncated file or ending tag missing");
            // exit(1);
        }
        getAtRecordKeyValue(data_key, data_value, data_line);
        if (strcmp(data_key, "end") == 0) {
            break;
        }
        else if (strcmp(data_key, "clef") == 0) {
            strcpy(c_clef, data_value);
        }
        else if (strcmp(data_key, "key") == 0) {
            strcpy(c_key, data_value);
        }
        else if (strcmp(data_key, "keysig") == 0) {
            strcpy(c_keysig, data_value);
        }
        else if (strcmp(data_key, "timesig") == 0) {
            strcpy(c_timesig, data_value);
        }
        else if (strcmp(data_key, "alttimesig") == 0) {
            strcpy(c_alttimesig, data_value);
        }
        else if (strcmp(data_key, "data") == 0) {
            strcpy(incipit, data_value);
        }
    }

    if (strlen(c_clef)) {
        Clef *c = new Clef;
        getClefInfo(c_clef, c); // do we need to put a default clef?
        if (!staffDefClef)
            staffDefClef = c;
        else
            current_measure.clef = c;
    }

    if (strlen(c_keysig)) {
        KeySig *k = new KeySig();
        getKeyInfo(c_keysig, k);
        if (!scoreDefKeySig) {
            scoreDefKeySig = k;
        }
        else {
            if (current_measure.key) {
                delete current_measure.key;
            }
            current_measure.key = k;
        }
    }
    if (strlen(c_timesig)) {
        if (m_is_mensural) {
            Mensur *mensur = new Mensur();
            getTimeInfo(c_timesig, NULL, mensur);
            if (!scoreDefMensur) {
                scoreDefMensur = mensur;
            }
            else {
                if (current_measure.mensur) {
                    delete current_measure.mensur;
                }
                current_measure.mensur = mensur;
            }
        }
        else {
            MeterSig *meter = new MeterSig;
            getTimeInfo(c_timesig, meter, NULL);
            if (!scoreDefMeterSig) {
                scoreDefMeterSig = meter;
            }
            else {
                if (current_measure.meter) {
                    delete current_measure.meter;
                }
                current_measure.meter = meter;
            }
        }
    }

    // read the incipit string
    int length = (int)strlen(incipit);
    int i = 0;
    while (i < length) {
        // eat the input...

        if (incipit[i] == ' ') {
            // just skip
            i++;
        }

        // octaves
        if ((incipit[i] == '\'') || (incipit[i] == ',')) {
            i += getOctave(incipit, &current_note.octave, i);
        }

        // rhythmic values
        else if (isdigit(incipit[i]) != 0) {
            i += getDurations(incipit, &current_measure, i);
        }

        // accidentals (1 = n; 2 = x; 3 = xx; 4 = b; 5 = bb)
        else if (incipit[i] == 'n' || incipit[i] == 'x' || incipit[i] == 'b') {
            i += getAccidental(incipit, &current_note.accidental, i);
        }

        //
        // beaming starts
        else if (incipit[i] == '{') {
            // current_note.beam = 1;
            if (current_note.tuplet_note > 0)
                current_note.beam = BEAM_TUPLET;
            else
                current_note.beam = BEAM_INITIAL;
            in_beam++;
        }

        // beaming ends
        else if (incipit[i] == '}' && in_beam > 0) {
            current_measure.notes[current_measure.notes.size() - 1].beam = BEAM_TERMINAL;
            current_note.beam = 0;
            in_beam--;
        }

        // ties are read when adding the note
        else if (incipit[i] == '+') {
        }

        // beginning tuplets & fermatas
        else if (incipit[i] == '(') {
            i += getTupletFermata(incipit, &current_note, i);
        }

        // end of tuplets
        else if ((incipit[i] == ';') || (incipit[i] == ')')) {
            i += getTupletFermataEnd(incipit, &current_note, i);
        }

        // trills are read when adding the note
        else if (incipit[i] == 't') {
        }

        // grace notes
        else if ((incipit[i] == 'g') || (incipit[i] == 'q')) {
            i += getGraceNote(incipit, &current_note, i);
        }

        // end of appogiatura
        else if (incipit[i] == 'r') {
            current_note.appoggiatura = 0; // should not have to be done, but just in case
        }

        // note and rest
        // getNote also creates a new note object
        else if (((incipit[i] - 'A' >= 0) && (incipit[i] - 'A' < 7)) || (incipit[i] == '-')) {
            i += getNote(incipit, &current_note, &current_measure, i);
        }

        // whole rest
        else if (incipit[i] == '=') {
            i += getWholeRest(incipit, &current_measure.wholerest, i);
        }

        // abbreviation
        else if (incipit[i] == '!') {
            i += getAbbreviation(incipit, &current_measure, i);
        }

        // measure repetition
        else if ((incipit[i] == 'i') && staff.size() > 0) {
            pae::Measure last_measure = staff[staff.size() - 1];
            current_measure.notes = last_measure.notes;
            current_measure.wholerest = last_measure.wholerest;
        }

        // barLine
        else if ((incipit[i] == ':') || (incipit[i] == '/')) {
            i += getBarLine(incipit, &current_measure.barLine, i);
            current_measure.abbreviation_offset = 0; // just in case...
            staff.push_back(current_measure);
            current_measure.reset();
        }

        // clef change
        else if ((incipit[i] == '%') && (i + 1 < length)) {
            Clef *c = new Clef;
            i += getClefInfo(incipit, c, i + 1);
            //
            if (!staffDefClef) {
                staffDefClef = c;
            }
            // If there are no notes yet in the measure
            // attach this clef change to the measure
            else if (current_measure.notes.size() == 0) {
                // If a clef was already assigned, remove it
                if (current_measure.clef) delete current_measure.clef;

                current_measure.clef = c;
            }
            else {
                // as above
                if (current_note.clef) delete current_note.clef;

                current_note.clef = c;
            }
        }

        // time signature change
        else if ((incipit[i] == '@') && (i + 1 < length)) {
            if (m_is_mensural) {
                Mensur *mensur = new Mensur();
                i += getTimeInfo(incipit, NULL, mensur, i + 1);
                if (current_note.mensur) {
                    delete current_note.mensur;
                }
                current_note.mensur = mensur;
            }
            else {
                MeterSig *meter = new MeterSig;
                i += getTimeInfo(incipit, meter, NULL, i + 1);
                if (current_measure.notes.size() == 0) {
                    if (current_measure.meter) {
                        delete current_measure.meter;
                    }
                    // When will this be deleted? Potential memory leak? LP
                    current_measure.meter = meter;
                }
                else {
                    if (current_note.meter) {
                        delete current_note.meter;
                    }
                    current_note.meter = meter;
                }
            }
        }

        // key signature change
        else if ((incipit[i] == '$') && (i + 1 < length)) {
            KeySig *k = new KeySig;
            i += getKeyInfo(incipit, k, i + 1);
            if (current_measure.notes.size() == 0) {
                if (current_measure.key) delete current_measure.key;

                current_measure.key = k;
            }
            else {
                if (current_note.key) delete current_note.key;

                current_note.key = k;
            }
        }

        i++;
    }

    // we need to add the last measure if it has no barLine at the end
    if (current_measure.notes.size() != 0) {
        // current_measure.barLine = "=-";
        staff.push_back(current_measure);
        current_measure.notes.clear();
    }

    m_doc->Reset();
    m_doc->SetType(Raw);
    // The mdiv
    Mdiv *mdiv = new Mdiv();
    mdiv->m_visibility = Visible;
    m_doc->AddChild(mdiv);
    // The score
    Score *score = new Score();
    mdiv->AddChild(score);
    // the section
    Section *section = new Section();
    score->AddChild(section);

    int measure_count = 1;

    std::vector<pae::Measure>::iterator it;
    for (it = staff.begin(); it < staff.end(); ++it) {

        m_staff = new Staff(1);
        m_measure = new Measure(true, measure_count);
        m_layer = new Layer();
        m_layer->SetN(1);

        m_staff->AddChild(m_layer);
        m_measure->AddChild(m_staff);

        pae::Measure obj = *it;

        // Add a score def if we have a new key sig or meter sig
        if (obj.key || obj.meter) {
            ScoreDef *scoreDef = new ScoreDef();
            if (obj.key) {
                scoreDef->SetKeySig(obj.key->ConvertToKeySigLog());
                delete obj.key;
                obj.key = NULL;
            }
            if (obj.meter) {
                scoreDef->SetMeterCount(obj.meter->GetCount());
                scoreDef->SetMeterUnit(obj.meter->GetUnit());
                scoreDef->SetMeterSym(obj.meter->GetSym());
                delete obj.meter;
                obj.meter = NULL;
            }
            section->AddChild(scoreDef);
        }

        section->AddChild(m_measure);

        convertMeasure(&obj);
        measure_count++;
    }

    // add minimal scoreDef
    StaffGrp *staffGrp = new StaffGrp();
    StaffDef *staffDef = new StaffDef();
    staffDef->SetN(1);
    staffDef->SetLines(5);
    if (m_is_mensural) {
        staffDef->SetNotationtype(NOTATIONTYPE_mensural);
    }
    if (staffDefClef) {
        staffDef->SetClefShape(staffDefClef->GetShape());
        staffDef->SetClefLine(staffDefClef->GetLine());
        staffDef->SetClefDis(staffDefClef->GetDis());
        staffDef->SetClefDisPlace(staffDefClef->GetDisPlace());
        delete staffDefClef;
    }
    if (scoreDefKeySig) {
        m_doc->m_scoreDef.SetKeySig(scoreDefKeySig->ConvertToKeySigLog());
        delete scoreDefKeySig;
    }
    if (scoreDefMeterSig) {
        m_doc->m_scoreDef.SetMeterCount(scoreDefMeterSig->GetCount());
        m_doc->m_scoreDef.SetMeterUnit(scoreDefMeterSig->GetUnit());
        m_doc->m_scoreDef.SetMeterSym(scoreDefMeterSig->GetSym());
        delete scoreDefMeterSig;
    }
    if (scoreDefMensur) {
        m_doc->m_scoreDef.SetMensurDot(scoreDefMensur->GetDot());
        m_doc->m_scoreDef.SetMensurSign(scoreDefMensur->GetSign());
        m_doc->m_scoreDef.SetMensurSlash(scoreDefMensur->GetSlash());
        m_doc->m_scoreDef.SetProportNum(scoreDefMensur->GetNum());
        m_doc->m_scoreDef.SetProportNumbase(scoreDefMensur->GetNumbase());
        delete scoreDefMensur;
    }
    if (m_tie != NULL) {
        delete m_tie;
        m_tie = NULL;
    }
    staffGrp->AddChild(staffDef);
    m_doc->m_scoreDef.AddChild(staffGrp);

    m_doc->ConvertToPageBasedDoc();
}

//////////////////////////////
//
// getOctave --
//
#define BASE_OCT 4
int PaeInput::getOctave(const char *incipit, char *octave, int index)
{
    int i = index;
    int length = (int)strlen(incipit);
    if (incipit[i] == '\'') {
        *octave = BASE_OCT;
        while ((i + 1 < length) && (incipit[i + 1] == '\'')) {
            (*octave)++;
            i++;
        }
    }
    else if (incipit[i] == ',') {
        // negative octave
        *octave = BASE_OCT - 1;
        while ((i + 1 < length) && (incipit[i + 1] == ',')) {
            (*octave)--;
            i++;
        }
    }

    return i - index;
}

//////////////////////////////
//
// getDuration --
//

int PaeInput::getDuration(const char *incipit, data_DURATION *duration, int *dot, int index)
{

    int i = index;
    int length = (int)strlen(incipit);

    if (m_is_mensural) {
        switch (incipit[i]) {
            case '0': *duration = DURATION_longa; break;
            case '1': *duration = DURATION_semibrevis; break;
            case '2': *duration = DURATION_minima; break;
            case '3':
                *duration = DURATION_breve;
                LogWarning("Duration 3 unsupported with mensural notation, using breve instead");
                break;
            case '4': *duration = DURATION_semiminima; break;
            case '5':
                *duration = DURATION_breve;
                LogWarning("Duration 5 unsupported with mensural notation, using breve instead");
                break;
            case '6': *duration = DURATION_semifusa; break;
            case '7': *duration = DURATION_breve; break;
            case '8': *duration = DURATION_fusa; break;
            case '9': *duration = DURATION_brevis; break;
        }
    }
    else {
        switch (incipit[i]) {
            case '0': *duration = DURATION_long; break;
            case '1': *duration = DURATION_1; break;
            case '2': *duration = DURATION_2; break;
            case '3': *duration = DURATION_32; break;
            case '4': *duration = DURATION_4; break;
            case '5': *duration = DURATION_64; break;
            case '6': *duration = DURATION_16; break;
            case '7': *duration = DURATION_128; break;
            case '8': *duration = DURATION_8; break;
            case '9': *duration = DURATION_breve; break;
        }
    }

    *dot = 0;
    while ((i + 1 < length) && (incipit[i + 1] == '.')) {
        (*dot)++;
        i++;
    }
    if ((*dot == 1) && (incipit[i] == 7)) {
        // neumatic notation
        *duration = DURATION_breve;
        *dot = 0;
        LogWarning("Plaine & Easie import: neumatic notation unsupported, using breve instead");
    }

    return i - index;
}

//////////////////////////////
//
// getDurations --
//

int PaeInput::getDurations(const char *incipit, pae::Measure *measure, int index)
{
    int i = index;
    int length = (int)strlen(incipit);

    measure->durations_offset = 0;
    measure->durations.clear();
    measure->dots.clear();

    // int j = 0;
    do {
        int dot;
        data_DURATION dur = DURATION_4;
        // measure->dots.setSize(j+1);
        i += getDuration(incipit, &dur, &dot, i);
        measure->durations.push_back(dur);
        measure->dots.push_back(dot);
        // j++;
        if ((i + 1 < length) && isdigit(incipit[i + 1])) {
            i++;
        }
        else {
            break;
        }
    } while (1);

    return i - index;
}

//////////////////////////////
//
// getAccidental --
//

int PaeInput::getAccidental(const char *incipit, data_ACCIDENTAL_WRITTEN *accident, int index)
{
    int i = index;
    int length = (int)strlen(incipit);

    if (incipit[i] == 'n') {
        *accident = ACCIDENTAL_WRITTEN_n;
    }
    else if (incipit[i] == 'x') {
        *accident = ACCIDENTAL_WRITTEN_s;
        if ((i + 1 < length) && (incipit[i + 1] == 'x')) {
            *accident = ACCIDENTAL_WRITTEN_x;
            i++;
        }
    }
    else if (incipit[i] == 'b') {
        *accident = ACCIDENTAL_WRITTEN_f;
        if ((i + 1 < length) && (incipit[i + 1] == 'b')) {
            *accident = ACCIDENTAL_WRITTEN_ff;
            i++;
        }
    }
    return i - index;
}

//////////////////////////////
//
// getTupletOrFermata --
//

int PaeInput::getTupletFermata(const char *incipit, pae::Note *note, int index)
{
    int i = index;
    int length = (int)strlen(incipit);

    // Detect if it is a fermata or a tuplet.
    //
    // std::regex_constants::ECMAScript is the default syntax, so optional.
    // Previously these were extended regex syntax, but this case
    // is the same in ECMAScript syntax.
    std::regex exp("^([^)]*[ABCDEFG-][^)]*[ABCDEFG-][^)]*)", std::regex_constants::ECMAScript);
    bool is_tuplet = regex_search(incipit + i, exp);

    if (is_tuplet) {
        int t = i;
        int t2 = 0;
        int tuplet_val = 3; // triplets are default
        char *buf;

        // Triplets are in the form (4ABC)
        // index points to the '(', so we look back
        // if the resut is a number or dot, it means we have the long format
        // i.e. 4(6ABC;5) or 4.(6ABC;5)
        if ((index != 0) && (isdigit(incipit[index - 1]) || incipit[index - 1] == '.')) {

            // create the buffer so we can convert the tuplet nr to int
            buf = (char *)malloc(length + 1); // allocate it with space for 0x00
            memset(buf, 0x00, length + 1); // wipe it up

            // move until we find the ;
            while ((t < length) && (incipit[t] != ';')) {

                // we should not find any close paren before the ';' !
                // FIXME find a graceful way to exit signaling this to user
                if (incipit[t] == ')') {
                    LogDebug("Plaine & Easie import: You have a ')' before the ';' in a tuplet!");
                    free(buf);
                    return i - index;
                }

                t++;
            }

            // t + 1 should point to the number
            t++; // move one char to the number
            while (((t + t2) < length) && (incipit[t + t2] != ')')) {

                // If we have extraneous chars, exit here
                if (!isdigit(incipit[t + t2])) {
                    LogDebug("Plaine & Easie import: non-number in tuplet number found");
                    free(buf);
                    return i - index;
                }

                // copy the number char-by-char
                buf[t2] = incipit[t + t2];
                t2++;
            }

            tuplet_val = atoi(buf);
            free(buf); // dispose of the buffer
        }

        // this is the first note, the total number of notes = tuplet_val
        note->tuplet_notes = tuplet_val;
        // but also the note counter
        note->tuplet_note = tuplet_val;
    }
    else {
        if (note->tuplet_notes > 0) {
            LogWarning("Plaine & Easie import: fermatas within tuplets won't be handled correctly");
        }
        note->fermata = true;
    }

    return i - index;
}

//////////////////////////////
//
// getTupletFermataEnd --
//
// this can be deleted in the future?
int PaeInput::getTupletFermataEnd(const char *incipit, pae::Note *note, int index)
{
    int i = index;
    // int length = strlen(incipit);

    // TODO: fermatas inside tuplets won't be currently handled correctly
    note->fermata = false;

    return i - index;
}

//////////////////////////////
//
// getGraceNote --
//

int PaeInput::getGraceNote(const char *incipit, pae::Note *note, int index)
{
    int i = index;
    int length = (int)strlen(incipit);

    // acciaccatura
    if (incipit[i] == 'g') {
        note->acciaccatura = true;
    }

    // appoggiatura
    else if (incipit[i] == 'q') {
        note->appoggiatura = 1;
        if ((i + 1 < length) && (incipit[i + 1] == 'q')) {
            i++;
            int r = i;
            while ((r < length) && (incipit[r] != 'r')) {
                if ((incipit[r] - 'A' >= 0) && (incipit[r] - 'A' < 7)) {
                    note->appoggiatura++;
                }
                r++;
            }
        }
    }
    return i - index;
}

//////////////////////////////
//
// getPitch --
//

data_PITCHNAME PaeInput::getPitch(char c_note)
{
    data_PITCHNAME pitch = PITCHNAME_c;

    switch (c_note) {
        case 'A': pitch = PITCHNAME_a; break;
        case 'B': pitch = PITCHNAME_b; break;
        case 'C': pitch = PITCHNAME_c; break;
        case 'D': pitch = PITCHNAME_d; break;
        case 'E': pitch = PITCHNAME_e; break;
        case 'F': pitch = PITCHNAME_f; break;
        case 'G': pitch = PITCHNAME_g; break;
        case '-': pitch = PITCHNAME_NONE; break;
        default: break;
    }
    return pitch;
}

//////////////////////////////
//
// getTimeInfo -- read the time signature.
//

int PaeInput::getTimeInfo(const char *incipit, MeterSig *meter, Mensur *mensur, int index)
{
    int i = index;
    int length = (int)strlen(incipit);

    if (!meter && !mensur) {
        return 0;
    }

    if (!isdigit(incipit[i]) && (incipit[i] != 'c') && (incipit[i] != 'o')) {
        return 0;
    }

    // find the end of time signature
    i++; // the time signature length is a least 1
    while (i < length) {
        if (!isdigit(incipit[i]) && (incipit[i] != '/') && (incipit[i] != '.')) {
            break;
        }
        i++;
    }

    // use a substring for the time signature
    char timesig_str[1024];
    memset(timesig_str, 0, 1024);
    // strncpy not always put the \0 in the end!
    strncpy(timesig_str, incipit + index, i - index);

    std::ostringstream sout;

    // regex_match matches to the entire input string (regex_search does
    // partial matches.  In this case cmatch is used to store the submatches
    // (enclosed in parentheses) for later reference.  Use std::smatch when
    // dealing with strings, or std::wmatch with wstrings.
    std::cmatch matches;
    if (meter) {
        if (regex_match(timesig_str, matches, std::regex("(\\d+)/(\\d+)"))) {
            meter->SetCount(std::stoi(matches[1]));
            meter->SetUnit(std::stoi(matches[2]));
        }
        else if (regex_match(timesig_str, matches, std::regex("\\d+"))) {
            meter->SetCount(std::stoi(timesig_str));
        }
        else if (strcmp(timesig_str, "c") == 0) {
            // C
            meter->SetSym(METERSIGN_common);
        }
        else if (strcmp(timesig_str, "c/") == 0) {
            // C|
            meter->SetSym(METERSIGN_cut);
        }
        else if (strcmp(timesig_str, "c3") == 0) {
            // C3
            meter->SetSym(METERSIGN_common);
            meter->SetCount(3);
        }
        else if (strcmp(timesig_str, "c3/2") == 0) {
            // C3/2
            meter->SetSym(METERSIGN_common); // ??
            meter->SetCount(3);
            meter->SetUnit(2);
        }
        else {
            LogWarning("Plaine & Easie import: unsupported time signature: %s", timesig_str);
        }
    }
    else {
        if (regex_match(timesig_str, matches, std::regex("(\\d+)/(\\d+)"))) {
            mensur->SetNum(std::stoi(matches[1]));
            mensur->SetNumbase(std::stoi(matches[2]));
        }
        else if (regex_match(timesig_str, matches, std::regex("\\d+"))) {
            mensur->SetNum(std::stoi(timesig_str));
        }
        else if (regex_match(timesig_str, matches, std::regex("([co])([\\./]?)([\\./]?)(\\d*)/?(\\d*)"))) {
            // C
            if (matches[1] == "c") {
                mensur->SetSign(MENSURATIONSIGN_C);
            }
            // O
            else {
                mensur->SetSign(MENSURATIONSIGN_O);
            }
            // Dot (second or third match since order between . and / is not defined in PAE)
            if ((matches[2] == ".") || (matches[3] == ".")) {
                mensur->SetDot(BOOLEAN_true);
            }
            // Slash (second or third match, ditto)
            if ((matches[2] == "/") || (matches[3] == "/")) {
                mensur->SetSlash(1);
            }
            // Num
            if (matches[4] != "") {
                mensur->SetNum(std::stoi(matches[4]));
            }
            // Numbase (but only if Num is given)
            if ((matches[4] != "") && (matches[5] != "")) {
                mensur->SetNumbase(std::stoi(matches[5]));
            }
        }
        else {
            LogWarning("Plaine & Easie import: unsupported time signature: %s", timesig_str);
        }
    }

    return i - index;
}

//////////////////////////////
//
// getClefInfo -- read the clef.
//

int PaeInput::getClefInfo(const char *incipit, Clef *mclef, int index)
{
    // a clef is maximum 3 character length
    // go through the 3 character and retrieve the letter (clef) and the line
    int length = (int)strlen(incipit);
    int i = 0;
    char clef = 'G';
    char line = '2';
    while ((index < length) && (i < 3)) {
        if (i == 0) {
            clef = incipit[index];
        }
        else if (i == 2) {
            line = incipit[index];
        }
        if (incipit[index] == '+') {
            m_is_mensural = true;
        }
        i++;
        index++;
    }

    if (clef == 'G') {
        mclef->SetShape(CLEFSHAPE_G);
        mclef->SetLine(line - 48);
    }
    else if (clef == 'C') {
        mclef->SetShape(CLEFSHAPE_C);
        mclef->SetLine(line - 48);
    }
    else if (clef == 'F') {
        mclef->SetShape(CLEFSHAPE_F);
        mclef->SetLine(line - 48);
    }
    else if (clef == 'g') {
        mclef->SetShape(CLEFSHAPE_G);
        mclef->SetLine(line - 48);
        mclef->SetDis(OCTAVE_DIS_8);
        mclef->SetDisPlace(STAFFREL_basic_below);
    }
    else {
        LogDebug("Plaine & Easie import: undefined clef %c", clef);
    }

    // measure->clef = mclef;

    return i;
}

//////////////////////////////
//
// getWholeRest -- read the getWholeRest.
//

int PaeInput::getWholeRest(const char *incipit, int *wholerest, int index)
{
    int length = (int)strlen(incipit);
    int i = index;

    *wholerest = 1;
    if ((i + 1 < length) && isdigit(incipit[i + 1])) {
        sscanf(&(incipit[i + 1]), "%d", wholerest);
        char buf[10];
        memset(buf, 0, 10);
        sprintf(buf, "%d", *wholerest);
        i += strlen(buf);
    }
    return i - index;
}

/**********************************
 *
 * getBarLine -- read the barLine.
 * Translation from PAE to verovio representaion:
 *
 BARRENDITION_single     /
 BARRENDITION_end        does not exist
 BARRENDITION_rptboth    ://:
 BARRENDITION_rptend     ://
 BARRENDITION_rptstart   //:
 BARRENDITION_dbl        //
 */

int PaeInput::getBarLine(const char *incipit, data_BARRENDITION *output, int index)
{

    bool is_barline_rptboth = false;
    bool is_barline_rptend = false;
    bool is_barline_rptstart = false;
    bool is_barline_dbl = false;

    if (strncmp(incipit + index, "://:", 4) == 0) {
        is_barline_rptboth = true;
    }

    if (strncmp(incipit + index, "://", 3) == 0) {
        is_barline_rptend = true;
    }

    if (strncmp(incipit + index, "//:", 3) == 0) {
        is_barline_rptstart = true;
    }

    if (strncmp(incipit + index, "//", 2) == 0) {
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
// getAbbreviation -- read abbreviation
//

int PaeInput::getAbbreviation(const char *incipit, pae::Measure *measure, int index)
{
    int length = (int)strlen(incipit);
    int i = index;
    int j;

    if (measure->abbreviation_offset == -1) { // start
        measure->abbreviation_offset = (int)measure->notes.size();
    }
    else { //
        int abbreviation_stop = (int)measure->notes.size();
        while ((i + 1 < length) && (incipit[i + 1] == 'f')) {
            i++;
            for (j = measure->abbreviation_offset; j < abbreviation_stop; ++j) {
                measure->notes.push_back(measure->notes[j]);
            }
        }
        measure->abbreviation_offset = -1;
    }

    return i - index;
}

//////////////////////////////
//
// getKeyInfo -- read the key signature.
//

int PaeInput::getKeyInfo(const char *incipit, KeySig *key, int index)
{
    int alt_nr = 0;
    m_keySigString = "";

    // at the key information line, extract data
    int length = (int)strlen(incipit);
    int i = index;
    bool end_of_keysig = false;
    while ((i < length) && (!end_of_keysig)) {
        switch (incipit[i]) {
            case 'b': key->SetAlterationType(ACCIDENTAL_WRITTEN_f); break;
            case 'x': key->SetAlterationType(ACCIDENTAL_WRITTEN_s); break;
            case 'n': key->SetAlterationType(ACCIDENTAL_WRITTEN_n); break;
            case '[': break;
            case 'F':
            case 'C':
            case 'G':
            case 'D':
            case 'A':
            case 'E':
            case 'B': alt_nr++; break;
            default: end_of_keysig = true; break;
        }
        if (!end_of_keysig) {
            m_keySigString.push_back(incipit[i]);
            i++;
        }
    }

    if (key->GetAlterationType() != ACCIDENTAL_WRITTEN_n) {
        key->SetAlterationNumber(alt_nr);
    }

    key->ConvertToMei();

    return i - index;
}

//////////////////////////////
//
// getNote --
//

int PaeInput::getNote(const char *incipit, pae::Note *note, pae::Measure *measure, int index)
{
    int oct;
    int i = index;
    int app;
    int tuplet_num;

    if (note->acciaccatura) {
        // acciaccaturas are always eights regardless
        // and have no dots
        note->duration = DURATION_8;
    }
    else {
        if (measure->durations.size() == 0) {
            note->duration = DURATION_4;
            note->dots = 0;
            LogWarning("Plaine & Easie import: found note before duration was specified");
        }
        else {
            note->duration = measure->durations[measure->durations_offset];
            note->dots = measure->dots[measure->durations_offset];
        }
    }
    note->pitch = getPitch(incipit[i]);

    if (m_keySigString.find(incipit[i]) != std::string::npos) {
        if (m_keySigString[0] == 'x')
            note->accidGes = ACCIDENTAL_GESTURAL_s;
        else if (m_keySigString[0] == 'b')
            note->accidGes = ACCIDENTAL_GESTURAL_f;
    }

    // lookout, hack. If a rest (PITCHNAME_NONE val) then create rest object.
    // it will be added instead of the note
    if (note->pitch == PITCHNAME_NONE) {
        note->rest = true;
    }

    // chord
    if (regex_search(incipit + i + 1, std::regex("^[^A-G]*\\^"))) {
        note->chord = true;
    }

    // tie
    if (regex_search(incipit + i + 1, std::regex("^[^A-G]*\\+"))) {
        note->tie = true;
    }

    // trills
    if (regex_search(incipit + i + 1, std::regex("^[^A-G]*t"))) {
        note->trill = true;
    }

    oct = note->octave;
    measure->notes.push_back(*note);

    app = note->appoggiatura;
    tuplet_num = note->tuplet_note;

    // Reset note to defaults
    note->clear();

    // write back the values we need to save
    note->octave = oct; // save octave

    // tuplets. Decrease the current number if we are in a tuplet
    // i.e. tuplet_num > 0
    // al the other values just need to be in the first note
    if (tuplet_num > 0) {
        note->tuplet_note = --tuplet_num;
    }

    // grace notes
    note->acciaccatura = false;
    if (app > 0) {
        note->appoggiatura = --app;
    }
    // durations
    if (measure->durations.size() > 0) {
        measure->durations_offset++;
        if (measure->durations_offset >= (int)measure->durations.size()) {
            measure->durations_offset = 0;
        }
    }

    note->fermata = false; // only one note per fermata;
    note->trill = false;

    return i - index;
}

//////////////////////////////
//
// convertMeasure --
//

void PaeInput::convertMeasure(pae::Measure *measure)
{
    if (measure->clef != NULL) {
        m_layer->AddChild(measure->clef);
    }

    if (measure->wholerest > 0) {
        if (measure->wholerest == 1) {
            MRest *mRest = new MRest;
            m_layer->AddChild(mRest);
        }
        else {
            MultiRest *multiRest = new MultiRest();
            multiRest->SetNum(measure->wholerest);
            m_layer->AddChild(multiRest);
        }
    }

    m_nested_objects.clear();

    for (unsigned int i = 0; i < measure->notes.size(); ++i) {
        pae::Note *note = &measure->notes[i];
        parseNote(note);
    }

    // Set barLine
    m_measure->SetRight(measure->barLine);
}

void PaeInput::parseNote(pae::Note *note)
{

    LayerElement *element;

    if (note->rest) {
        Rest *rest = new Rest();

        rest->SetDur(note->duration);

        if (!m_is_mensural && note->dots != 0) {
            rest->SetDots(note->dots);
        }

        if (note->fermata) {
            Fermata *fermata = new Fermata();
            fermata->SetStartid(rest->GetUuid());
            m_measure->AddChild(fermata);
        }

        element = rest;
    }
    else {
        Note *mnote = new Note();

        mnote->SetPname(note->pitch);
        mnote->SetOct(note->octave);
        if (note->accidental != ACCIDENTAL_WRITTEN_NONE) {
            Accid *accid = new Accid();
            accid->SetAccid(note->accidental);
            mnote->AddChild(accid);
        }
        if (!mnote->FindChildByType(ACCID)) {
            Accid *accid = new Accid();
            mnote->AddChild(accid);
            accid->IsAttribute(true);
            accid->SetAccidGes(note->accidGes);
        }

        mnote->SetDur(note->duration);

        if (!m_is_mensural && note->dots != 0) {
            mnote->SetDots(note->dots);
        }

        // pseudo chant notation with 7. in PAE - make quater notes without stem
        if ((mnote->GetDur() == DURATION_128) && (mnote->GetDots() == 1)) {
            mnote->SetDur(DURATION_4);
            mnote->SetDots(0);
            mnote->SetStemLen(0);
            mnote->SetStemVisible(BOOLEAN_false);
        }

        if (note->fermata) {
            Fermata *fermata = new Fermata();
            fermata->SetStartid(mnote->GetUuid());
            m_measure->AddChild(fermata);
        }

        if (note->trill) {
            Trill *trill = new Trill();
            trill->SetStartid(mnote->GetUuid());
            m_measure->AddChild(trill);
        }

        if (m_tie != NULL) {
            m_tie->SetEndid(mnote->GetUuid());
            m_measure->AddChild(m_tie);
            m_tie = NULL;
        }

        if (note->tie) {
            m_tie = new Tie();
            m_tie->SetStartid(mnote->GetUuid());
        }

        element = mnote;
    }

    // Does this note have a clef change? push it before everything else
    if (note->clef) {
        addLayerElement(note->clef);
    }

    // Same thing for time changes
    // You can find this sometimes
    if (note->meter) {
        addLayerElement(note->meter);
    }
    if (note->mensur) {
        addLayerElement(note->mensur);
    }

    // Handle key change. Evil if done in a beam
    if (note->key) {
        addLayerElement(note->key);
    }

    // Acciaccaturas are similar but do not get beamed (do they)
    // this case is simpler. NOTE a note can not be acciacctura AND appoggiatura
    // Acciaccatura rests do not exist
    if (note->acciaccatura && (element->Is(NOTE))) {
        Note *mnote = dynamic_cast<Note *>(element);
        assert(mnote);
        mnote->SetDur(DURATION_8);
        mnote->SetGrace(GRACE_unacc);
        mnote->SetStemDir(STEMDIRECTION_up);
    }

    if ((note->appoggiatura > 0) && (element->Is(NOTE))) {
        Note *mnote = dynamic_cast<Note *>(element);
        assert(mnote);
        mnote->SetGrace(GRACE_acc);
        mnote->SetStemDir(STEMDIRECTION_up);
    }

    if ((note->beam == BEAM_INITIAL) && !m_is_mensural) {
        pushContainer(new Beam());
    }

    // we have a tuplet, the tuplet_note is > 0
    // which means we are counting a tuplet
    if (note->tuplet_note > 0 && note->tuplet_notes == note->tuplet_note) { // first elem in tuplet
        Tuplet *newTuplet = new Tuplet();
        newTuplet->SetNum(note->tuplet_notes);
        newTuplet->SetNumbase(2);
        pushContainer(newTuplet);
    }

    if ((note->beam == BEAM_TUPLET) && !m_is_mensural) {
        pushContainer(new Beam());
    }

    // note in a chord
    if (note->chord) {
        Note *mnote = dynamic_cast<Note *>(element);
        assert(mnote);
        // first note?
        if (!m_is_in_chord) {
            Chord *chord = new Chord();
            chord->SetDots(mnote->GetDots());
            chord->SetDur(mnote->GetDur());
            pushContainer(chord);
            m_is_in_chord = true;
        }
        mnote->ResetAugmentDots();
        mnote->ResetDurationLogical();
    }

    // Add the note to the current container
    addLayerElement(element);

    // Add mensural dot
    if (m_is_mensural && note->dots > 0) {
        Dot *dot = new Dot();
        addLayerElement(dot);
    }

    // the last note counts always '1'
    // insert the tuplet elem
    // and reset the tuplet counter
    if (note->tuplet_note == 1) {
        popContainer();
    }

    if ((note->beam == BEAM_TERMINAL) && !m_is_mensural) {
        popContainer();
    }

    // last note of a chord
    if (!note->chord && m_is_in_chord) {
        Note *mnote = dynamic_cast<Note *>(element);
        assert(mnote);
        mnote->ResetAugmentDots();
        mnote->ResetDurationLogical();
        popContainer();
        m_is_in_chord = false;
    }
}

void PaeInput::pushContainer(LayerElement *container)
{
    addLayerElement(container);
    m_nested_objects.push_back(container);
}

void PaeInput::popContainer()
{
    // assert(m_nested_objects.size() > 0);
    if (m_nested_objects.size() == 0) {
        LogError("Plaine & Easie import: tried to pop an object from empty stack. "
                 "Cross-measure objects (tuplets, beams) are not supported.");
    }
    else {
        m_nested_objects.pop_back();
    }
}

void PaeInput::addLayerElement(LayerElement *element)
{
    if (m_nested_objects.size() > 0) {
        m_nested_objects.back()->AddChild(element);
    }
    else {
        m_layer->AddChild(element);
    }
}

//////////////////////////////
//
// getAtRecordKeyValue --
//   Formats: @key: value
//            @key:value
//            @key :value
//            @ key :value
//            @ key : value
//   only one per line
//

void PaeInput::getAtRecordKeyValue(char *key, char *value, const char *input)
{

#define SKIPSPACE                                                                                                      \
    while ((index < length) && isspace(input[index])) {                                                                \
        index++;                                                                                                       \
    }

    char MARKER = '@';
    char SEPARATOR = ':';
    char EMPTY = '\0';

    int length = (int)strlen(input);
    int count = 0;

    // zero out strings
    memset(key, EMPTY, MAX_DATA_LEN);
    memset(value, EMPTY, MAX_DATA_LEN);

    if (length == 0) {
        return;
    }

    char ch;
    int index = 0;

    // find starting @ symbol (ignoring any starting space)
    SKIPSPACE
    if (input[index] != MARKER) {
        // invalid record format since it does not start with @
        return;
    }
    index++;
    SKIPSPACE

    // start storing the key value:
    while ((index < length) && (input[index] != SEPARATOR)) {
        if (isspace(input[index])) {
            continue;
        }
        ch = input[index];

        // Should never happen
        if (count >= MAX_DATA_LEN) return;

        key[count] = ch;
        count++;
        index++;
    }
    // check to see if valid format: (:) must be the current character
    if (input[index] != SEPARATOR) {
        key[0] = EMPTY;
        return;
    }
    index++;
    SKIPSPACE

    // Also should never happen
    if (strlen(&input[index]) > MAX_DATA_LEN) return;

    strcpy(value, &input[index]);

    // Truncate string to first space
    size_t i;
    for (i = strlen(value) - 2; i > 0; i--) {
        if (isspace(value[i])) {
            value[i] = EMPTY;
            continue;
        }
        break;
    }
}

#endif // NO_PAE_SUPPORT

} // namespace vrv
