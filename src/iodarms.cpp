/////////////////////////////////////////////////////////////////////////////
// Name:        iodarms.cpp
// Author:      Rodolfo Zitellini
// Created:     02/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "iodarms.h"

//----------------------------------------------------------------------------

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

//----------------------------------------------------------------------------

#include "clef.h"
#include "doc.h"
#include "keysig.h"
#include "measure.h"
#include "mensur.h"
#include "note.h"
#include "page.h"
#include "rest.h"
#include "symbol.h"
#include "system.h"
#include "staff.h"
#include "tie.h"
#include "tuplet.h"

namespace vrv {

// Ok, this is ugly, but since this is static data, why not?
pitchmap DarmsInput::PitchMap[] = {
    /* 00 */ {1, PITCH_C}, {1, PITCH_D}, {1, PITCH_E}, {1, PITCH_F}, {1, PITCH_G}, {1, PITCH_A}, {1, PITCH_B},
    /* 07 */ {2, PITCH_C}, {2, PITCH_D}, {2, PITCH_E}, {2, PITCH_F}, {2, PITCH_G}, {2, PITCH_A}, {2, PITCH_B},
    /* 14 */ {3, PITCH_C}, {3, PITCH_D}, {3, PITCH_E}, {3, PITCH_F}, {3, PITCH_G}, {3, PITCH_A}, {3, PITCH_B},
    /* 21 */ {4, PITCH_C}, {4, PITCH_D}, {4, PITCH_E}, {4, PITCH_F}, {4, PITCH_G}, {4, PITCH_A}, {4, PITCH_B},
    /* 28 */ {5, PITCH_C}, {5, PITCH_D}, {5, PITCH_E}, {5, PITCH_F}, {5, PITCH_G}, {5, PITCH_A}, {5, PITCH_B},
    /* 35 */ {6, PITCH_C}, {6, PITCH_D}, {6, PITCH_E}, {6, PITCH_F}, {6, PITCH_G}, {6, PITCH_A}, {6, PITCH_B},
    /* 42 */ {7, PITCH_C}, {7, PITCH_D}, {7, PITCH_E}, {7, PITCH_F}, {7, PITCH_G}, {7, PITCH_A}, {7, PITCH_B},
    /* 49 */ {8, PITCH_C}, {8, PITCH_D}, {8, PITCH_E}, {8, PITCH_F}, {8, PITCH_G}, {8, PITCH_A}, {8, PITCH_B},
};

DarmsInput::DarmsInput( Doc *doc, std::string filename ) :
FileInputStream( doc )
{	
    m_layer = NULL;
    m_measure = NULL;
    m_staff = NULL;
    
    m_filename = filename;
}

DarmsInput::~DarmsInput() {
    
}

void DarmsInput::UnrollKeysig(int quantity, char alter) {
    int flats[] = {PITCH_B, PITCH_E, PITCH_A, PITCH_D, PITCH_G, PITCH_C, PITCH_F};
    int sharps[] = {PITCH_F, PITCH_C, PITCH_G, PITCH_D, PITCH_A, PITCH_E, PITCH_B};
    int *alteration_set;
    unsigned char accid = ACCID_FLAT;
    
    if (quantity == 0) quantity++;
    
    if (alter == '-') {
        alteration_set = flats;
        accid = ACCID_FLAT;
    } else {
        alteration_set = sharps;
        accid = ACCID_SHARP;
    }
    
    KeySignature *k = new KeySignature(quantity, accid);
    m_layer->AddElement(k);
    return;
    //////
    for (int i = 0; i < quantity; i++) {
        Symbol *alter = new Symbol( SYMBOL_ACCID );
        alter->m_oct = 4;
        alter->m_pname = alteration_set[i];
        alter->m_accid = accid;
        m_layer->AddElement(alter);
    }

}

/*
 Read the meter
 */
int DarmsInput::parseMeter(int pos, const char* data) {
 
    Mensur *meter = new Mensur;
    
    pos++;
    if (data[pos] == 'C') {
        if (data[pos + 1] == '/') {
            pos++;
            meter->m_meterSymb = METER_SYMB_CUT;
        } else {
            meter->m_meterSymb = METER_SYMB_COMMON;
        }
        pos++;
    } else if (data[pos] == 'O') {
        if (data[pos + 1] == '/') {
            pos++;
            LogWarning("DarmsInput: O/ not supported");
        }
        meter->m_sign = MENSUR_SIGN_O;
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
        meter->m_num = n1;
        
        // we expect the next char a ':', or make a single digit meter
        // mini-hack in some cases it is a '-'...
        if (data[pos + 1] != ':' && data[pos + 1] != '-') {
            pos++;
            meter->m_numBase = 1;
        } else {
            pos++;
            if (data[pos] == '-') LogWarning("DarmsInput: Time sig numbers should be divided with ':'.");
            // same as above, get one or two nums
            n1 = data[++pos] - ASCII_NUMBER_OFFSET; // old school conversion to int
            if (isdigit(data[pos + 1])) {
                n2 = data[++pos] - ASCII_NUMBER_OFFSET; // idem
                n1 = (n1 * 10) + n2;
            }
            
            meter->m_numBase = n1;
        }
        LogDebug("DarmsInput: Meter is: %i %i", meter->m_num, meter->m_numBase);
    }
    
    m_layer->AddElement(meter);
    return pos;
}

/*
 Process the various headings: !I, !K, !N, !M
*/
int DarmsInput::do_globalSpec(int pos, const char* data) {
    char digit = data[++pos];
    int quantity = 0;
    
    switch (digit) {
        case 'I': // Voice nr.
            //the next digit should be a number, but we do not care what
            if (!isdigit(data[++pos])) {
                LogWarning("DarmsInput: Expected number after I");
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
            } else {
                LogWarning("DarmsInput: Invalid char for K: %c", data[pos]);
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
            if (!isdigit(data[++pos])) {
                LogWarning("DarmsInput: Expected number after N");
            } else { // we honor only notehead 7, diamond
                if (data[pos] == 0x07 + ASCII_NUMBER_OFFSET)
                    m_antique_notation = true;
            }
            break;
        
        default:
            break;
    }
    
    return pos;
}

int DarmsInput::do_Clef(int pos, const char* data) {
    int position = data[pos] - ASCII_NUMBER_OFFSET; // manual conversion from ASCII to int
    
    pos = pos + 2; // skip the '!' 3!F
    
    Clef *mclef = new Clef();
    
    if (data[pos] == 'C') {
        switch (position) {
            case 1: mclef->m_clefId = UT1; break;
            case 3: mclef->m_clefId = UT2; break;
            case 5: mclef->m_clefId = UT3; break;
            case 7: mclef->m_clefId = UT4; break;
            default: LogWarning("DarmsInput: Invalid C clef on line %i", position); break;
        }
        m_clef_offset = 21 - position; // 21 is the position in the array, position is of the clef
    } else if (data[pos] == 'G') {
        switch (position) {
            case 1: mclef->m_clefId = SOL1; break;
            case 3: mclef->m_clefId = SOL2; break;
            default: LogWarning("DarmsInput: Invalid G clef on line %i", position); break;
        }
        m_clef_offset = 25 - position;
    } else if (data[pos] == 'F') {
        switch (position) {
            case 3: mclef->m_clefId = FA3; break;
            case 5: mclef->m_clefId = FA4; break;
            case 7: mclef->m_clefId = FA5; break;
            default: LogWarning("DarmsInput: Invalid F clef on line %i", position); break;
        }
        m_clef_offset = 15 - position;
    } else {
        // what the...
        LogWarning("DarmsInput: Invalid clef specification: %c", data[pos]);
        return 0; // fail
    }
    
    m_layer->AddElement(mclef);
    return pos;
}

int DarmsInput::do_Note(int pos, const char* data, bool rest) {
    int position;
    int accidental = 0;
    int duration;
    int dot = 0;
    int tie = 0;
    
    //pos points to the first digit of the note
    // it can be 5W, 12W, -1W
    
    // Negative number, only '-' and one digit
    if (data[pos] == '-') {
        // be sure following char is a number
        if (!isdigit(data[pos + 1])) return 0;
        position = -(data[++pos] - ASCII_NUMBER_OFFSET);
    } else {
        // as above
        if (!isdigit(data[pos]) && data[pos] != 'R') return 0; // this should not happen, as it is checked in the caller
        // positive number
        position = data[pos] - ASCII_NUMBER_OFFSET;
        //check for second digit
        if (isdigit(data[pos + 1])) {
            pos++;
            position = (position * 10) + (data[pos] - ASCII_NUMBER_OFFSET);
        }
    }
    
    if (data[pos + 1] == '-') {
        accidental = ACCID_FLAT;
        pos++;
    } else if (data[pos + 1] == '#') {
        accidental = ACCID_SHARP;
        pos++;
    } else if (data[pos + 1] == '*') {
        accidental = ACCID_NATURAL;
        pos++;
    }
    
    switch (data[++pos]) {
        case 'W':
            duration = DUR_1;
            // wholes can be repeated, yes this way is not nice
            if (data[pos + 1] == 'W') { // WW = BREVE
                duration = DUR_BR;
                pos++;
                if (data[pos + 1] == 'W') { // WWW - longa
                    pos++;
                    duration = DUR_LG;
                }
            }
            break;
        case 'H': duration = DUR_2; break;
        case 'Q': duration = DUR_4; break;
        case 'E': duration = DUR_8; break;
        case 'S': duration = DUR_16; break;
        case 'T': duration = DUR_32; break;
        case 'X': duration = DUR_64; break;
        case 'Y': duration = DUR_128; break;
        case 'Z': duration = DUR_256; break;
            
        default:
            LogWarning("DarmsInput: Unkown note duration: %c", data[pos]);
            return 0;
            break;
    }
    
    if (data[pos + 1] =='.') {
        pos++;
        dot = 1;
    }
    
    // tie with following note
    if (data[pos + 1] =='L' || data[pos + 1] =='J') {
        pos++;
        tie = 1;
    }
    
    if (rest) {
        Rest *rest =  new Rest;
        rest->m_dur = duration;
        rest->m_durGes = DUR_8;
        rest->m_pname = REST_AUTO;
        rest->m_dots = dot;
        m_layer->AddElement(rest);
    } else {
        
        if ((position + m_clef_offset) > sizeof(PitchMap))
            position = 0;
        
        Note *note = new Note;
        note->m_dur = duration;
        note->m_durGes = DUR_8;
        note->m_accid = accidental;
        note->m_oct = PitchMap[position + m_clef_offset].oct;
        note->m_pname = PitchMap[position + m_clef_offset].pitch;
        note->m_dots = dot;
        m_layer->AddElement(note);
        
        // Ties are between two notes and have a reference to the two notes
        // if more than two notes are ties, the m_second note of the first
        // tie will containn the same note as the m_first in the second tie:
        // NOTE1 tie1 NOTE2 tie2 NOTE3
        // tie1->m_first = NOTE1, tie1->m_second = NOTE2
        // tie2->m_first = NOTE2, tie2->m_second = NOTE3
        if (tie) {
            // cur tie !NULL, so we add this note as second note there
            if (m_current_tie) {
                m_current_tie->SetSecondNote( note );
            }
            // create a new mus tie with this note
            m_current_tie = new Tie;
            m_current_tie->SetFirstNote( note );
            m_layer->AddElement(m_current_tie);
        } else {
            // no tie (L or J) specified for not
            // but if cur tie !NULL we need to close the tie
            // and set cur tie to NULL
            if (m_current_tie) {
                m_current_tie->SetSecondNote( note );
                m_current_tie = NULL;
            }
        }
        
    }
    
    return pos;
}

bool DarmsInput::ImportFile() {
    char data[10000];
    int len;
    
    std::ifstream infile;
    
    infile.open(m_filename.c_str());
    
    if (infile.eof()) {
        infile.close();
        return false;
    }
    
    infile.getline(data, sizeof(data), '\n');
    len = strlen(data);
    infile.close();
    
    return ImportString(data);
}
    
bool DarmsInput::ImportString(std::string data_str) {
    int len, res;
    int pos = 0;
    const char *data = data_str.c_str();
    len = data_str.length();
    
    m_doc->Reset( Raw );
    System *system = new System();
    Page *page = new Page();
    m_staff = new Staff( 1 );
    m_measure = new Measure( true, 1 );
    m_layer = new Layer( 1 );
    
    m_current_tie = NULL;
    m_staff->AddLayer(m_layer);
    m_measure->AddStaff( m_staff );
    system->AddMeasure( m_measure );
    
    // do this the C style, char by char
    while (pos < len) {
        char c = data[pos];
        
        if (c == '!') {
            LogDebug("DarmsInput: Global spec. at %i", pos);
            res = do_globalSpec(pos, data);
            if (res) pos = res;
            // if notehead type was specified in the !Nx option preserve it
            m_staff->m_mensuralNotation = m_antique_notation;
        } else if (isdigit(c) || c == '-' ) { // check for '-' too as note positions can be negative
            //is number followed by '!' ? it is a clef
            if (data[pos + 1] == '!') {
                res = do_Clef(pos, data);
                if (res) pos = res;
            } else { // we assume it is a note
                res = do_Note(pos, data, false);
                if (res) pos = res;
            }
        } else if (c == 'R') {
            res = do_Note(pos, data, true);
            if (res) pos = res;
        } else {
            //if (!isspace(c))
                //LogMessage("Other %c", c);
        }
 
        pos++;
    }
    
    // add miniaml scoreDef
    StaffGrp *staffGrp = new StaffGrp();
    StaffDef *staffDef = new StaffDef();
    staffDef->SetStaffNo( 1 );
    staffGrp->AddStaffDef( staffDef );
    m_doc->m_scoreDef.AddStaffGrp( staffGrp );
    
    page->AddSystem( system );
    m_doc->AddPage( page );
    
    return true;
}

} // namespace vrv
