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
    /* 00 */ {1, PITCHNAME_c}, {1, PITCHNAME_d}, {1, PITCHNAME_e}, {1, PITCHNAME_f}, {1, PITCHNAME_g}, {1, PITCHNAME_a}, {1, PITCHNAME_b},
    /* 07 */ {2, PITCHNAME_c}, {2, PITCHNAME_d}, {2, PITCHNAME_e}, {2, PITCHNAME_f}, {2, PITCHNAME_g}, {2, PITCHNAME_a}, {2, PITCHNAME_b},
    /* 14 */ {3, PITCHNAME_c}, {3, PITCHNAME_d}, {3, PITCHNAME_e}, {3, PITCHNAME_f}, {3, PITCHNAME_g}, {3, PITCHNAME_a}, {3, PITCHNAME_b},
    /* 21 */ {4, PITCHNAME_c}, {4, PITCHNAME_d}, {4, PITCHNAME_e}, {4, PITCHNAME_f}, {4, PITCHNAME_g}, {4, PITCHNAME_a}, {4, PITCHNAME_b},
    /* 28 */ {5, PITCHNAME_c}, {5, PITCHNAME_d}, {5, PITCHNAME_e}, {5, PITCHNAME_f}, {5, PITCHNAME_g}, {5, PITCHNAME_a}, {5, PITCHNAME_b},
    /* 35 */ {6, PITCHNAME_c}, {6, PITCHNAME_d}, {6, PITCHNAME_e}, {6, PITCHNAME_f}, {6, PITCHNAME_g}, {6, PITCHNAME_a}, {6, PITCHNAME_b},
    /* 42 */ {7, PITCHNAME_c}, {7, PITCHNAME_d}, {7, PITCHNAME_e}, {7, PITCHNAME_f}, {7, PITCHNAME_g}, {7, PITCHNAME_a}, {7, PITCHNAME_b},
    /* 49 */ {8, PITCHNAME_c}, {8, PITCHNAME_d}, {8, PITCHNAME_e}, {8, PITCHNAME_f}, {8, PITCHNAME_g}, {8, PITCHNAME_a}, {8, PITCHNAME_b},
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
    int flats[] = {PITCHNAME_b, PITCHNAME_e, PITCHNAME_a, PITCHNAME_d, PITCHNAME_g, PITCHNAME_c, PITCHNAME_f};
    int sharps[] = {PITCHNAME_f, PITCHNAME_c, PITCHNAME_g, PITCHNAME_d, PITCHNAME_a, PITCHNAME_e, PITCHNAME_b};
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
    
    KeySig *k = new KeySig(quantity, accid);
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
        meter->SetSign( MENSURATIONSIGN_C );
        if (data[pos + 1] == '/') {
            pos++;
            meter->SetSlash(1);
        }
        pos++;
    } else if (data[pos] == 'O') {
        if (data[pos + 1] == '/') {
            pos++;
            LogWarning("DarmsInput: O/ not supported");
        }
        meter->SetSign( MENSURATIONSIGN_O );
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
        meter->SetNumbaseInt(n1);
        
        // we expect the next char a ':', or make a single digit meter
        // mini-hack in some cases it is a '-'...
        if (data[pos + 1] != ':' && data[pos + 1] != '-') {
            pos++;
            meter->SetNumbaseInt(1);
        } else {
            pos++;
            if (data[pos] == '-') LogWarning("DarmsInput: Time sig numbers should be divided with ':'.");
            // same as above, get one or two nums
            n1 = data[++pos] - ASCII_NUMBER_OFFSET; // old school conversion to int
            if (isdigit(data[pos + 1])) {
                n2 = data[++pos] - ASCII_NUMBER_OFFSET; // idem
                n1 = (n1 * 10) + n2;
            }
            
            meter->SetNumbaseInt(n1);
        }
        LogDebug("DarmsInput: Meter is: %i %i", meter->GetNumbaseInt(), meter->GetNumbaseInt());
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
        mclef->SetShape(CLEFSHAPE_C);
        switch (position) {
            case 1: mclef->SetLine(1); break;
            case 3: mclef->SetLine(2); break;
            case 5: mclef->SetLine(3); break;
            case 7: mclef->SetLine(4); break;
            default: LogWarning("DarmsInput: Invalid C clef on line %i", position); break;
        }
        m_clef_offset = 21 - position; // 21 is the position in the array, position is of the clef
    } else if (data[pos] == 'G') {
        mclef->SetShape(CLEFSHAPE_G);
        switch (position) {
            case 1: mclef->SetLine(1); break;
            case 3: mclef->SetLine(2); break;
            default: LogWarning("DarmsInput: Invalid G clef on line %i", position); break;
        }
        m_clef_offset = 25 - position;
    } else if (data[pos] == 'F') {
        mclef->SetShape(CLEFSHAPE_F);
        switch (position) {
            case 3: mclef->SetLine(3); break;
            case 5: mclef->SetLine(4);; break;
            case 7: mclef->SetLine(5); break;
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
        rest->SetDur(duration);
        rest->m_durGes = DUR_8;
        rest->m_pname = REST_AUTO;
        rest->SetDots( dot );
        m_layer->AddElement(rest);
    } else {
        
        if ((position + m_clef_offset) > sizeof(PitchMap))
            position = 0;
        
        Note *note = new Note;
        note->SetDur(duration);
        note->m_durGes = DUR_8;
        note->m_accid = accidental;
        note->m_oct = PitchMap[position + m_clef_offset].oct;
        note->m_pname = PitchMap[position + m_clef_offset].pitch;
        note->SetDots( dot );
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
    m_layer = new Layer( );
    m_layer->SetN( 1 );
    
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
            m_staff->notAnc = m_antique_notation;
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
