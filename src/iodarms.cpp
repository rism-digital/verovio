/////////////////////////////////////////////////////////////////////////////
// Name:        iodarms.cpp
// Author:      Rodolfo Zitellini
// Created:     02/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "iodarms.h"

//----------------------------------------------------------------------------

#include <fstream>

//----------------------------------------------------------------------------

#ifndef NO_DARMS_SUPPORT

#include "clef.h"
#include "doc.h"
#include "keysig.h"
#include "layer.h"
#include "mdiv.h"
#include "measure.h"
#include "mensur.h"
#include "note.h"
#include "rest.h"
#include "score.h"
#include "section.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "tie.h"
#include "vrv.h"

#define MAX_DARMS_BUFFER 10000

#endif /* NO_DARMS_SUPPORT */

namespace vrv {

#ifndef NO_DARMS_SUPPORT

// Ok, this is ugly, but since this is static data, why not?
pitchmap DarmsInput::PitchMap[] = {
    /* 00 */ { 1, PITCHNAME_c },
    { 1, PITCHNAME_d },
    { 1, PITCHNAME_e },
    { 1, PITCHNAME_f },
    { 1, PITCHNAME_g },
    { 1, PITCHNAME_a },
    { 1, PITCHNAME_b },
    /* 07 */ { 2, PITCHNAME_c },
    { 2, PITCHNAME_d },
    { 2, PITCHNAME_e },
    { 2, PITCHNAME_f },
    { 2, PITCHNAME_g },
    { 2, PITCHNAME_a },
    { 2, PITCHNAME_b },
    /* 14 */ { 3, PITCHNAME_c },
    { 3, PITCHNAME_d },
    { 3, PITCHNAME_e },
    { 3, PITCHNAME_f },
    { 3, PITCHNAME_g },
    { 3, PITCHNAME_a },
    { 3, PITCHNAME_b },
    /* 21 */ { 4, PITCHNAME_c },
    { 4, PITCHNAME_d },
    { 4, PITCHNAME_e },
    { 4, PITCHNAME_f },
    { 4, PITCHNAME_g },
    { 4, PITCHNAME_a },
    { 4, PITCHNAME_b },
    /* 28 */ { 5, PITCHNAME_c },
    { 5, PITCHNAME_d },
    { 5, PITCHNAME_e },
    { 5, PITCHNAME_f },
    { 5, PITCHNAME_g },
    { 5, PITCHNAME_a },
    { 5, PITCHNAME_b },
    /* 35 */ { 6, PITCHNAME_c },
    { 6, PITCHNAME_d },
    { 6, PITCHNAME_e },
    { 6, PITCHNAME_f },
    { 6, PITCHNAME_g },
    { 6, PITCHNAME_a },
    { 6, PITCHNAME_b },
    /* 42 */ { 7, PITCHNAME_c },
    { 7, PITCHNAME_d },
    { 7, PITCHNAME_e },
    { 7, PITCHNAME_f },
    { 7, PITCHNAME_g },
    { 7, PITCHNAME_a },
    { 7, PITCHNAME_b },
    /* 49 */ { 8, PITCHNAME_c },
    { 8, PITCHNAME_d },
    { 8, PITCHNAME_e },
    { 8, PITCHNAME_f },
    { 8, PITCHNAME_g },
    { 8, PITCHNAME_a },
    { 8, PITCHNAME_b },
};

DarmsInput::DarmsInput(Doc *doc) : Input(doc)
{
    m_layer = NULL;
    m_measure = NULL;
    m_staff = NULL;
}

DarmsInput::~DarmsInput() {}

void DarmsInput::UnrollKeysig(int quantity, char alter)
{
    // data_PITCHNAME flats[]
    //    = { PITCHNAME_b, PITCHNAME_e, PITCHNAME_a, PITCHNAME_d, PITCHNAME_g, PITCHNAME_c, PITCHNAME_f };
    // data_PITCHNAME sharps[]
    //    = { PITCHNAME_f, PITCHNAME_c, PITCHNAME_g, PITCHNAME_d, PITCHNAME_a, PITCHNAME_e, PITCHNAME_b };
    // data_PITCHNAME *alteration_set;
    data_ACCIDENTAL_WRITTEN accid = ACCIDENTAL_WRITTEN_NONE;

    if (quantity == 0) quantity++;

    if (alter == '-') {
        // alteration_set = flats;
        accid = ACCIDENTAL_WRITTEN_f;
    }
    else {
        // alteration_set = sharps;
        accid = ACCIDENTAL_WRITTEN_s;
    }

    KeySig *k = new KeySig;
    k->IsAttribute(true);
    k->SetSig(std::make_pair(quantity, accid));
    m_layer->AddChild(k);
    return;
    //////
    /*
    for (int i = 0; i < quantity; ++i) {
        Accid *alter = new Accid();
        alter->SetOloc(4);
        alter->SetPloc(alteration_set[i]);
        alter->SetAccid(accid);
        m_layer->AddChild(alter);
    }
    */
}

/*
 Read the meter
 */
int DarmsInput::parseMeter(int pos, const char *data)
{

    Mensur *meter = new Mensur;

    pos++;
    if (data[pos] == 'C') {
        meter->SetSign(MENSURATIONSIGN_C);
        if (data[pos + 1] == '/') {
            pos++;
            meter->SetSlash(1);
        }
        pos++;
    }
    else if (data[pos] == 'O') {
        if (data[pos + 1] == '/') {
            pos++;
            LogWarning("DARMS import: O/ not supported");
        }
        meter->SetSign(MENSURATIONSIGN_O);
        pos++;
    }

    // See if followed by numerical meter
    if (isdigit(data[pos])) { // Coupound meter
        int n1, n2;
        n1 = data[pos] - ASCII_NUMBER_OFFSET; // old school conversion to int
        if (isdigit(data[pos + 1])) {
            n2 = data[++pos] - ASCII_NUMBER_OFFSET; // idem
            n1 = (n1 * 10) + n2;
        }
        meter->SetNumbase(n1);

        // we expect the next char a ':', or make a single digit meter
        // mini-hack in some cases it is a '-'...
        if (data[pos + 1] != ':' && data[pos + 1] != '-') {
            pos++;
            meter->SetNumbase(1);
        }
        else {
            pos++;
            if (data[pos] == '-') LogWarning("DARMS import: Time signature numbers should be divided with ':'.");
            // same as above, get one or two nums
            n1 = data[++pos] - ASCII_NUMBER_OFFSET; // old school conversion to int
            if (isdigit(data[pos + 1])) {
                n2 = data[++pos] - ASCII_NUMBER_OFFSET; // idem
                n1 = (n1 * 10) + n2;
            }

            meter->SetNumbase(n1);
        }
        LogDebug("DARMS import: Meter is: %i %i", meter->GetNumbase(), meter->GetNumbase());
    }

    m_layer->AddChild(meter);
    return pos;
}

/*
 Process the various headings: !I, !K, !N, !M
*/
int DarmsInput::do_globalSpec(int pos, const char *data)
{
    char digit = data[++pos];
    int quantity = 0;

    switch (digit) {
        case 'I': // Voice nr.
            // the next digit should be a number, but we do not care what
            if (!isdigit(data[++pos])) {
                LogWarning("DARMS import: Expected number after 'I'");
            }
            break;

        case 'K': // key sig, !K2- = two flats
            if (isdigit(data[pos + 1])) { // is followed by number?
                pos++; // move forward
                quantity = data[pos] - ASCII_NUMBER_OFFSET; // get number from ascii char
            }
            // next we expect a flat or sharp, - or #
            pos++;
            if (data[pos] == '-' || data[pos] == '#') {
                UnrollKeysig(quantity, data[pos]);
            }
            else {
                LogWarning("DARMS import: Invalid char for 'K': %c", data[pos]);
            }
            break;

        case 'M': // meter
            pos = parseMeter(pos, data);
            break;

        case 'N': // notehead type:
            /*
             N0	notehead missing
             N1	stem missing
             N2	double notehead
             N3	triangle notehead
             N4	square notehead
             N6	"X" notehead
             N7	diamond notehead, stem centered
             N8	diamond notehead, stem to side
             NR	rest in place of notehead
             */
            if (!isdigit(data[++pos]) && data[++pos] != 'R') {
                LogWarning("DARMS import: Expected number or 'R' after N");
            }
            else { // we honor only notehead 7, diamond
                if (data[pos] == 0x07 + ASCII_NUMBER_OFFSET) m_antique_notation = true;
            }
            break;

        default: break;
    }

    return pos;
}

int DarmsInput::do_Clef(int pos, const char *data)
{
    int position = data[pos] - ASCII_NUMBER_OFFSET; // manual conversion from ASCII to int

    pos = pos + 2; // skip the '!' 3!F

    Clef *mclef = new Clef();

    if (data[pos] == 'C') {
        mclef->SetShape(CLEFSHAPE_C);
        switch (position) {
            case 1: mclef->SetLine(1); break;
            case 3: mclef->SetLine(2); break;
            case 5: mclef->SetLine(3); break;
            case 7: mclef->SetLine(4); break;
            default: LogWarning("DARMS import: Invalid C clef on line %i", position); break;
        }
        m_clef_offset = 21 - position; // 21 is the position in the array, position is of the clef
    }
    else if (data[pos] == 'G') {
        mclef->SetShape(CLEFSHAPE_G);
        switch (position) {
            case 1: mclef->SetLine(1); break;
            case 3: mclef->SetLine(2); break;
            default: LogWarning("DARMS import: Invalid G clef on line %i", position); break;
        }
        m_clef_offset = 25 - position;
    }
    else if (data[pos] == 'F') {
        mclef->SetShape(CLEFSHAPE_F);
        switch (position) {
            case 3: mclef->SetLine(3); break;
            case 5:
                mclef->SetLine(4);
                ;
                break;
            case 7: mclef->SetLine(5); break;
            default: LogWarning("DARMS import: Invalid F clef on line %i", position); break;
        }
        m_clef_offset = 15 - position;
    }
    else {
        // what the...
        LogWarning("DARMS import: Invalid clef specification: %c", data[pos]);
        // avoiding memory leak
        delete mclef;
        return 0; // fail
    }

    m_layer->AddChild(mclef);
    return pos;
}

int DarmsInput::do_Note(int pos, const char *data, bool rest)
{
    int position;
    data_ACCIDENTAL_WRITTEN accidental = ACCIDENTAL_WRITTEN_NONE;
    data_DURATION duration;
    int dot = 0;
    int tie = 0;

    // pos points to the first digit of the note
    // it can be 5W, 12W, -1W

    // Negative number, only '-' and one digit
    if (data[pos] == '-') {
        // be sure following char is a number
        if (!isdigit(data[pos + 1])) return 0;
        position = -(data[++pos] - ASCII_NUMBER_OFFSET);
    }
    else {
        // as above
        if (!isdigit(data[pos]) && data[pos] != 'R') return 0; // this should not happen, as it is checked in the caller
        // positive number
        position = data[pos] - ASCII_NUMBER_OFFSET;
        // check for second digit
        if (isdigit(data[pos + 1])) {
            pos++;
            position = (position * 10) + (data[pos] - ASCII_NUMBER_OFFSET);
        }
    }

    if (data[pos + 1] == '-') {
        accidental = ACCIDENTAL_WRITTEN_f;
        pos++;
    }
    else if (data[pos + 1] == '#') {
        accidental = ACCIDENTAL_WRITTEN_s;
        pos++;
    }
    else if (data[pos + 1] == '*') {
        accidental = ACCIDENTAL_WRITTEN_n;
        pos++;
    }

    switch (data[++pos]) {
        case 'W':
            duration = DURATION_1;
            // wholes can be repeated, yes this way is not nice
            if (data[pos + 1] == 'W') { // WW = BREVE
                duration = DURATION_breve;
                pos++;
                if (data[pos + 1] == 'W') { // WWW - longa
                    pos++;
                    duration = DURATION_long;
                }
            }
            break;
        case 'H': duration = DURATION_2; break;
        case 'Q': duration = DURATION_4; break;
        case 'E': duration = DURATION_8; break;
        case 'S': duration = DURATION_16; break;
        case 'T': duration = DURATION_32; break;
        case 'X': duration = DURATION_64; break;
        case 'Y': duration = DURATION_128; break;
        case 'Z': duration = DURATION_256; break;

        default: LogWarning("DARMS import: Unknown note duration: %c", data[pos]); return 0;
    }

    if (data[pos + 1] == '.') {
        pos++;
        dot = 1;
    }

    // tie with the following note
    if (data[pos + 1] == 'L' || data[pos + 1] == 'J') {
        pos++;
        tie = 1;
    }

    if (rest) {
        Rest *rest = new Rest;
        rest->SetDur(duration);
        rest->SetDurGes(DURATION_8);
        rest->SetDots(dot);
        m_layer->AddChild(rest);
    }
    else {
        if ((position + m_clef_offset) > sizeof(PitchMap)) position = 0;

        Note *note = new Note;
        note->SetDur(duration);
        note->SetDurGes(DURATION_8);
        if (accidental != ACCIDENTAL_WRITTEN_NONE) {
            Accid *accid = new Accid();
            accid->SetAccid(accidental);
            note->AddChild(accid);
        }
        note->SetOct(PitchMap[position + m_clef_offset].oct);
        note->SetPname(PitchMap[position + m_clef_offset].pitch);
        note->SetDots(dot);
        m_layer->AddChild(note);

        // Ties are between two notes and have a reference to the two notes
        // if more than two notes are tied, the m_second note of the first
        // tie will contain the same note as the m_first in the second tie:
        // NOTE1 tie1 NOTE2 tie2 NOTE3
        // tie1->m_first = NOTE1, tie1->m_second = NOTE2
        // tie2->m_first = NOTE2, tie2->m_second = NOTE3
        if (tie) {
            // cur tie !NULL, so we add this note as second note there
            if (m_current_tie) {
                m_current_tie->SetEnd(note);
            }
            // create a new mus tie with this note
            m_current_tie = new Tie;
            m_current_tie->SetStart(note);
        }
        else {
            // no tie (L or J) specified for not
            // but if cur tie !NULL we need to close the tie
            // and set cur tie to NULL
            if (m_current_tie) {
                m_current_tie->SetEnd(note);
                m_current_tie = NULL;
            }
        }
    }

    return pos;
}

bool DarmsInput::Import(const std::string &data_str)
{
    int len;
    int res;
    int pos = 0;
    const char *data = data_str.c_str();
    len = (int)data_str.length();

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

    m_staff = new Staff(1);
    m_measure = new Measure(true, 1);
    m_layer = new Layer();
    m_layer->SetN(1);

    m_current_tie = NULL;
    m_staff->AddChild(m_layer);
    m_measure->AddChild(m_staff);
    section->AddChild(m_measure);

    // do this the C style, char by char
    while (pos < len) {
        char c = data[pos];

        if (c == '!') {
            LogDebug("DARMS import: Global spec. at %i", pos);
            res = do_globalSpec(pos, data);
            if (res) pos = res;
            // if notehead type was specified in the !Nx option preserve it
            // m_staff->notAnc = m_antique_notation;
        }
        else if (isdigit(c) || c == '-') { // check for '-' too as note positions can be negative
            // is number followed by '!' ? it is a clef
            if (data[pos + 1] == '!') {
                res = do_Clef(pos, data);
                if (res) pos = res;
            }
            else { // we assume it is a note
                res = do_Note(pos, data, false);
                if (res) pos = res;
            }
        }
        else if (c == 'R') {
            res = do_Note(pos, data, true);
            if (res) pos = res;
        }
        else {
            // if (!isspace(c))
            // LogMessage("Other %c", c);
        }

        pos++;
    }

    // add minimal scoreDef
    StaffGrp *staffGrp = new StaffGrp();
    StaffDef *staffDef = new StaffDef();
    staffDef->SetN(1);
    staffGrp->AddChild(staffDef);
    m_doc->m_mdivScoreDef.AddChild(staffGrp);

    m_doc->ConvertToPageBasedDoc();

    return true;
}

#endif /* NO_DARMS_SUPPORT */

} // namespace vrv
