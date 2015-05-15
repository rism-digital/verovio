/////////////////////////////////////////////////////////////////////////////
// Name:        iopae.cpp
// Author:      Rodolfo Zitellini // this is the important stuff!
// Created:     2012
// Copyright (c) Laurent Pugin. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "iopae.h"

//----------------------------------------------------------------------------

#include <sstream>

//----------------------------------------------------------------------------

#include "beam.h"
#include "doc.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "metersig.h"
#include "multirest.h"
#include "note.h"
#include "page.h"
#include "rest.h"
#include "scoredef.h"
#include "system.h"
#include "staff.h"
#include "tuplet.h"
#include "vrv.h"

//----------------------------------------------------------------------------


#include <regex.h>

namespace vrv {

#define BEAM_INITIAL    0x01
#define BEAM_MEDIAL     0x02
#define BEAM_TERMINAL      0x04

 
// User interface variables:
int       debugQ = 0;                // used with --debug option
int       stdoutQ = 0;
char      outdir[1024] = {0};        // used with -d option
char      extension[1024] = {0};     // used with -e option
char      hum2abc[1024] = {0};       // used with -a option
int       quietQ  = 0;               // used with -q option
int       quiet2Q = 0;               // used with -Q option

// Global variables:
char data_line[10001] = {0};
#define MAX_DATA_LEN 1024 // One line of the pae file whould not be this long!
char data_key[MAX_DATA_LEN]; 
char data_value[MAX_DATA_LEN]; //ditto as above


//////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// PaeInput
//----------------------------------------------------------------------------

PaeInput::PaeInput( Doc *doc, std::string filename ) :
// This is pretty bad. We open a bad fileoinputstream as we don't use it
FileInputStream( doc )
{
    m_filename = filename;
	m_staff = NULL;
    m_measure = NULL;
	m_layer = NULL;
    m_last_tied_note = NULL;
}

PaeInput::~PaeInput()
{
}

//////////////////////////////////////////////////////////////////////////

bool PaeInput::ImportFile()
{
    
    std::ifstream infile;
    infile.open(m_filename.c_str());
    
    parsePlainAndEasy(infile);
    
    return true;
}

bool PaeInput::ImportString(std::string pae)
{
    
    std::istringstream in_stream(pae);
    
    parsePlainAndEasy(in_stream);
    
    return true;
}

//////////////////////////////
//
// parsePlainAndEasy --
//

void PaeInput::parsePlainAndEasy(std::istream &infile) {
    // buffers
    char c_clef[1024] = {0};
    char c_key[1024] = {0};
    char c_keysig[1024] = {0};
    char c_timesig[1024] = {0};
    char c_alttimesig[1024] = {0};
    char incipit[10001] = {0};
    int in_beam = 0;
    
    std::string s_key;
    MeasureObject current_measure;
    NoteObject current_note;
    Clef *staffDefClef = NULL;
    
    std::vector<MeasureObject> staff;
    
    // read values
    while (!infile.eof()) {
        infile.getline(data_line, 10000);
        if (infile.eof()) {
            LogDebug("Truncated file or ending tag missing");
            //exit(1);
        }
        getAtRecordKeyValue(data_key, data_value, data_line);
        if (strcmp(data_key,"end")==0) {   
            break;
        } else if (strcmp(data_key,"clef")==0) { 
            strcpy( c_clef, data_value );
        } else if (strcmp(data_key,"key")==0) { 
            strcpy( c_key, data_value );
        } else if (strcmp(data_key,"keysig")==0) { 
            strcpy( c_keysig, data_value );
        } else if (strcmp(data_key,"timesig")==0) { 
            strcpy( c_timesig, data_value );
        } else if (strcmp(data_key,"alttimesig")==0) { 
            strcpy( c_alttimesig, data_value );
        } else if (strcmp(data_key,"data")==0) { 
            strcpy( incipit, data_value );
        }
    }
    
    if (strlen(c_clef)) {
        Clef *c = new Clef;
        getClefInfo(c_clef, c );    // do we need to put a default clef?
        if (!staffDefClef) staffDefClef = c;
        else current_measure.clef = c;
    }

    if (strlen(c_keysig)) {
        KeySig *k = new KeySig();
        getKeyInfo( c_keysig, k);
        current_measure.key = k;
    }
    if (strlen(c_timesig)) {
        MeterSig *meter = new MeterSig;
        getTimeInfo( c_timesig, meter);
        // What about previous values? Potential memory leak? LP
        current_measure.meter = meter;
    }   
    
    // read the incipit string
    size_t length = strlen(incipit);
    int i = 0;
	while(i < length) {
        // eat the input...
        
		if (incipit[i] == ' ') {
            // just skip
            i++;
        }
        
        // octaves
        if ((incipit[i] == '\'') || (incipit[i] == ',')) {
            i += getOctave( incipit, &current_note.octave, i );
        }
        
        // rhythmic values
        else if (isdigit(incipit[i]) != 0) {
            i += getDurations( incipit, &current_measure, i );
        }
        
        //accidentals (1 = n; 2 = x; 3 = xx; 4 = b; 5 = bb)    
        else if (incipit[i] == 'n' || incipit[i] == 'x' || incipit[i] == 'b') {
            i += getAccidental( incipit, &current_note.accidental, i );
        }
        
        //
        // beaming starts
		else if (incipit[i] == '{') {
			//current_note.beam = 1;
            current_note.beam = BEAM_INITIAL;
            in_beam++;
        }
        
        // beaming ends
		else if (incipit[i] == '}' && in_beam > 0) {
            current_measure.notes[current_measure.notes.size() - 1].beam = BEAM_TERMINAL;
            current_note.beam = 0;
            in_beam--;
		}
		
        // slurs are read when adding the note
		else if (incipit[i] == '+') {
        }
        
		// beginning tuplets & fermatas
		else if (incipit[i] == '(') {
            i += getTupletFermata( incipit, &current_note, i );
		}
        
        // end of tuplets
		else if ((incipit[i] == ';') || (incipit[i] == ')')) {
            i += getTupletFermataEnd( incipit, &current_note, i );
		}
        
		// trills are read when adding the note
		else if (incipit[i] == 't') {
        }
        
		//grace notes
		else if ((incipit[i] == 'g') || (incipit[i] == 'q')) {
			i += getGraceNote( incipit, &current_note, i );
		}
		
        
        // end of appogiatura
		else if (incipit[i] == 'r') {
			current_note.appoggiatura = 0; // should not have to be done, but just in case
        }
        
        //note and rest
        // getNote also creates a new note object
        else if (((incipit[i]-'A'>=0) && (incipit[i]-'A'<7)) || (incipit[i]=='-')) {
            i += getNote( incipit, &current_note, &current_measure, i );
        }
        
  		// whole rest
		else if (incipit[i] == '=') {
            i += getWholeRest( incipit, &current_measure.wholerest, i );		
		} 
        
		// abbreviation
        else if (incipit[i] == '!') {
            i += getAbbreviation( incipit, &current_measure, i );
        }
        
        // measure repetition
        else if ((incipit[i] == 'i') && staff.size() > 0) {
            MeasureObject last_measure = staff[staff.size() - 1];
            current_measure.notes = last_measure.notes;
            current_measure.wholerest = last_measure.wholerest;
            
            // if old measure does not end with a tie
            // force the first note of the newly copied measure to be without tie
            // this is to prevent copying tie closes which are invalid
            if (last_measure.notes.size() > 0 && last_measure.notes[last_measure.notes.capacity() - 1].tie == 0)
                current_measure.notes[0].tie = 0;
        }
        
        //barLine
        else if ((incipit[i] == ':') || (incipit[i] == '/')) {
            i += getBarline(incipit, &current_measure.barLine, i);
            current_measure.abbreviation_offset = 0; // just in case...
            staff.push_back( current_measure );
            current_measure.reset();
        }
        
		//clef change
		else if ((incipit[i] == '%') && (i+1 < length)) {
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
                if (current_measure.clef)
                    delete current_measure.clef;
                
                current_measure.clef = c;
            } else {
                // as above
                if (current_note.clef)
                    delete current_note.clef;
                
                current_note.clef = c;
            }
        }
        
		//time signature change
		else if ((incipit[i] == '@') && (i+1 < length)) {
            MeterSig *meter = new MeterSig;
            i += getTimeInfo( incipit, meter, i + 1);
            if (current_measure.notes.size() == 0) {
                if (current_measure.meter) {
                    delete current_measure.meter;
                }
                // When will this be deleted? Potential memory leak? LP
                current_measure.meter = meter;
            } else {
                if (current_note.meter) {
                    delete current_note.meter;
                }
                current_note.meter = meter;
            }
        } 
        
  		//key signature change
		else if ((incipit[i] == '$') && (i+1 < length)) {
            KeySig *k = new KeySig;
            i += getKeyInfo( incipit, k, i + 1);
            if (current_measure.notes.size() == 0) {
                if (current_measure.key)
                    delete current_measure.key;
                
                current_measure.key = k;
            } else {
                if (current_note.key)
                    delete current_note.key;
                
                current_note.key = k;
            }
		}
            
        i++;
    }
    
    // we need to add the last measure if it has no barLine at the end
    if (current_measure.notes.size() != 0) {
        //current_measure.barLine = "=-";
        staff.push_back( current_measure );
        current_measure.notes.clear();
    }
    
    
    m_doc->Reset( Raw );
    Page *page = new Page();
    System *system = new System();
    
    int measure_count = 1;
    
    std::vector<MeasureObject>::iterator it;
    for ( it = staff.begin() ; it < staff.end(); it++ ) {
        
        m_staff = new Staff( 1 );
        m_measure = new Measure( true, measure_count );
        m_layer = new Layer( );
        m_layer->SetN( 1 );
        
        m_staff->AddLayer(m_layer);
        m_measure->AddMeasureElement( m_staff );
        system->AddMeasure( m_measure );
        
        MeasureObject obj = *it;
        convertMeasure( &obj );
        measure_count++;
    }

    // add miniaml scoreDef
    StaffGrp *staffGrp = new StaffGrp();
    StaffDef *staffDef = new StaffDef();
    staffDef->SetN( 1 );
    if (staffDefClef) {
        ClefAttr clefAttr;
        clefAttr.SetClefShape(staffDefClef->GetShape());
        clefAttr.SetClefLine(staffDefClef->GetLine());
        clefAttr.SetClefDis(staffDefClef->GetDis());
        clefAttr.SetClefDisPlace(staffDefClef->GetDisPlace());
        staffDef->ReplaceClef( &clefAttr );
        delete staffDefClef;
    }
    staffGrp->AddStaffDef( staffDef );
    m_doc->m_scoreDef.AddStaffGrp( staffGrp );

    page->AddSystem( system );
    m_doc->AddPage( page );
}



//////////////////////////////
//
// getOctave --
//
#define BASE_OCT 4
int PaeInput::getOctave (const char* incipit, char *octave, int index ) {
    
    int i = index;
    size_t length = strlen(incipit);
    if (incipit[i] == '\'') {
        *octave = BASE_OCT;
        while ((i+1 < length) && (incipit[i+1] == '\'')) {
            (*octave)++;
            i++;
        }
    } else if (incipit[i] == ',') {
        //negative octave
        *octave = BASE_OCT - 1;
        while ((i+1 < length) && (incipit[i+1] == ',')) {
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

int PaeInput::getDuration(const char* incipit, data_DURATION *duration, int *dot, int index ) {
    
    int i = index;
    size_t length = strlen(incipit);
    
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
     
    
    *dot=0;
    if ((i+1 < length) && (incipit[i+1] == '.')) {
        // one dot
        (*dot)++;
        i++;
    }
    if ((i+1 < length) && (incipit[i+1] == '.')) {
        // two dots
        (*dot)++;
        i++;
    }
    if ((*dot == 1) && (*duration == 7)) {
        // neumatic notation
        *duration = DURATION_breve;
        *dot = 0;
        LogWarning("Found a note in neumatic notation (7.), using quarter note instead");				
    }
    
    return i - index;
}



//////////////////////////////
//
// getDurations --
//

int PaeInput::getDurations(const char* incipit, MeasureObject* measure, int index ) {
    
    int i = index;
    size_t length = strlen(incipit);
    
    measure->durations_offset = 0;
    measure->durations.clear();
    measure->dots.clear();
    
    //int j = 0;
    do {
        int dot;
        data_DURATION dur;
        //measure->dots.setSize(j+1);
        i += getDuration(incipit, &dur, &dot, i );
        measure->durations.push_back(dur);
        measure->dots.push_back(dot);
        //j++;
        if ((i+1 < length) && isdigit(incipit[i+1])) {
            i++;
        } else {
            break;
        }
    } while ( 1 );
    
    return i - index;
}



//////////////////////////////
//
// getAccidental --
//

int PaeInput::getAccidental(const char* incipit, data_ACCIDENTAL_EXPLICIT *accident, int index ) {
    
    int i = index;
    size_t length = strlen(incipit);
    
    if (incipit[i] == 'n') {
        *accident = ACCIDENTAL_EXPLICIT_n;
    } else if (incipit[i] == 'x') {
        *accident = ACCIDENTAL_EXPLICIT_s;
        if ((i+1 < length) && (incipit[i+1] == 'x')) {
            *accident = ACCIDENTAL_EXPLICIT_ss;
            i++;
        }
    } else if (incipit[i] == 'b') {
        *accident = ACCIDENTAL_EXPLICIT_f;
        if ((i+1 < length) && (incipit[i+1] == 'b')) {
            *accident = ACCIDENTAL_EXPLICIT_ff;
            i++;
        }
    }
    return i - index;
}



//////////////////////////////
//
// getTupletOrFermata --
//

int PaeInput::getTupletFermata(const char* incipit, NoteObject* note, int index ) {
    
    int i = index;
    size_t length = strlen(incipit);
    
    // detect if it is a fermata or a tuplet
    regex_t re;
    regcomp(&re, "^([^)]*[ABCDEFG-][^)]*[ABCDEFG-][^)]*)", REG_EXTENDED);
    int is_tuplet = regexec(&re, incipit + i, 0, NULL, 0);
    regfree(&re);
    
    if (is_tuplet == 0) {
        int t = i;
        int t2 = 0;
        int tuplet_val = 0;
        char *buf;
        
        // Triplets are in the form (4ABC)
        // index points to the '(', so we look back
        // if the resut is a number or dot, it means we have the long format
        // i.e. 4(6ABC;5) or 4.(6ABC;5)
        if ((index != 0) && (isdigit(incipit[index - 1]) || incipit[index - 1] == '.')) {
            
            // create the buffer so we can convert the tuplet nr to int
            buf = (char*)malloc(length + 1); // allocate it with space for 0x00
            memset(buf, 0x00, length + 1); // wipe it up
            
            // move until we find the ;
            while ((t < length) && (incipit[t] != ';')) {
                
                // we should not find the parens before the ';' !
                // FIXME find a graceful way to exit signaling this to user
                if (incipit[t] == ')') {
                    LogDebug("You have a ) before the ; in a tuplet!");
                    free(buf);
                    return i - index;
                }
                
                t++;
            }
            
            //t + 1 should point to the number
            t++; // move one char to the number
            while (( (t + t2) < length) && (incipit[t + t2] != ')')) {
                
                // If we have extraneous chars, exit here
                if (!isdigit(incipit[t + t2])) {
                    LogDebug("You have a non-number in a tuplet number");
                    free(buf);
                    return i - index;
                }
                
                // copy the number char-by-char
                buf[t2] = incipit[t + t2];
                t2++;
            }
            
            tuplet_val = atoi(buf);
            free(buf); // dispose of the buffer
        
        } else { // it is a triplet
            // don't care to parse all the stuff
            tuplet_val = 3;
        }
        
        // this is the first note, the total number of notes = tuplet_val
        note->tuplet_notes = tuplet_val;
        // but also the note counter
        note->tuplet_note =  tuplet_val;
        
    } else {
        if ( note->tuplet_notes > 0 ) {
            LogWarning("Fermata within a tuplet. Won't be handled correctly");
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
int PaeInput::getTupletFermataEnd(const char* incipit, NoteObject *note, int index ) {
    
    int i = index;
    //int length = strlen(incipit);
        
    // TODO currently fermatas inside tuplets won't be handled correctly
    note->fermata = false;
        
    return i - index;
}



//////////////////////////////
//
// getGraceNote --
//

int PaeInput::getGraceNote(const char* incipit, NoteObject *note, int index ) {
    
    int i = index;
    size_t length = strlen(incipit);
    
    //acciaccatura
    if (incipit[i] == 'g') {
        note->acciaccatura = true;
    }
    
    // appoggiatura
    else if (incipit[i] == 'q') {
        note->appoggiatura = 1;
        if ((i+1 < length) && (incipit[i+1] == 'q')) {
            i++;
            int r = i;
            while ((r < length) && (incipit[r] != 'r')) {
                if ((incipit[r]-'A'>=0) && (incipit[r]-'A'<7)) {
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

data_PITCHNAME PaeInput::getPitch( char c_note ) {
    
    data_PITCHNAME pitch = PITCHNAME_c;
    
    switch (c_note) {
        case 'A':
            pitch = PITCHNAME_a;
            break;
        case 'B': 
            pitch = PITCHNAME_b;
            break;
        case 'C': 
            pitch = PITCHNAME_c;
            break;
        case 'D': 
            pitch = PITCHNAME_d;
            break;
        case 'E': 
            pitch = PITCHNAME_e;
            break;
        case 'F': 
            pitch = PITCHNAME_f;
            break;
        case 'G': 
            pitch = PITCHNAME_g;
            break;
        case '-': pitch = PITCHNAME_NONE; break;
        default:
            break;
    }
    return pitch;
}


//////////////////////////////
//
// getTimeInfo -- read the key signature.
//

int PaeInput::getTimeInfo( const char* incipit, MeterSig *meter, int index) {
    
    int i = index;
    size_t length = strlen(incipit);
    
    if (!isdigit(incipit[i]) && (incipit[i] != 'c') && (incipit[i] != 'o'))
        return 0;
    
    // find the end of time signature end
    i++; // the time signature length is a least 1
    while (i < length) {
        if (!isdigit(incipit[i]) && (incipit[i] != '/') && (incipit[i] != '.')) {
            break;
        }
        i++;
    }
        
    // use a substring for the time signature 
    char timesig_str[1024];
    memset( timesig_str, 0, 1024 );
    // strncpy not always put the \0 in the end!
    strncpy( timesig_str, incipit + index, i - index); 
    
    std::ostringstream sout;
    regex_t re;
    
    // check if format X/X or one digit only
    regcomp(&re, "^[0-9]*/[0-9]*$", REG_EXTENDED);
    int is_standard = regexec(&re, timesig_str, 0, NULL, 0);
    regfree(&re);
    regcomp(&re, "^[0-9]*$", REG_EXTENDED);
    int is_one_number = regexec(&re, timesig_str, 0, NULL, 0);
    regfree(&re);
    
    if ( is_standard == 0) {
        char buf_str[1024];
        strcpy(buf_str, timesig_str);
        int beats = atoi(strtok(buf_str, "/"));
        int note_value = atoi(strtok(NULL, "/")); 
        meter->SetCount(beats);
        meter->SetUnit(note_value);
    } else if ( is_one_number == 0) {
        int beats = atoi(timesig_str);
        meter->SetCount(beats);
        meter->SetUnit(1);
    } else if (strcmp(timesig_str, "c") == 0) {
        // C
        meter->SetSym(METERSIGN_common);
    } else if (strcmp(timesig_str, "c/") == 0) {
        // C|
        meter->SetSym(METERSIGN_cut);
    } else if (strcmp(timesig_str, "c3") == 0) {
        // C3
        meter->SetSym(METERSIGN_common);
        meter->SetCount(3);
    } else if (strcmp(timesig_str, "c3/2") == 0) {
        // C3/2
        meter->SetSym(METERSIGN_common); // ??
        meter->SetCount(3);
        meter->SetUnit(2);
    } else {
        LogWarning("Unknown time signature: %s", timesig_str);
    }
    
    return i - index;
}



//////////////////////////////
//
// getClefInfo -- read the key signature.
//

int PaeInput::getClefInfo( const char *incipit, Clef *mclef, int index ) {
    
    // a clef is maximum 3 character length
    // go through the 3 character and retrieve the letter (clef) and the line
    // mensural clef (with + in between) currently ignored
    // clef with octava correct?
    size_t length = strlen(incipit);
    int i = 0;
    char clef = 'G';
    char line = '2';
    while ((index < length) && (i < 3)) {
        if (i == 0) {
            clef = incipit[index];
        } else if (i == 2) {
            line = incipit[index];
        }
        i++;
        index++;
    }

    
    if (clef == 'C' || clef == 'c') {
        mclef->SetShape(CLEFSHAPE_C);
        mclef->SetLine(line - 48);
    } else if (clef == 'G') {
        mclef->SetShape(CLEFSHAPE_G);
        mclef->SetLine(line - 48);
    } else if (clef == 'g') {
        mclef->SetShape(CLEFSHAPE_G);
        mclef->SetLine(line - 48);
        mclef->SetDis(OCTAVE_DIS_8);
        mclef->SetDisPlace(PLACE_below);
    } else if (clef == 'F' || clef == 'f') {
        mclef->SetShape(CLEFSHAPE_F);
        mclef->SetLine(line - 48);
    } else {
        // what the...
        LogDebug("Clef %c is Undefined", clef);
    }
    
    //measure->clef = mclef;
        
    return i;
}



//////////////////////////////
//
// getWholeRest -- read the getWholeRest.
//

int PaeInput::getWholeRest( const char *incipit, int *wholerest, int index ) {
    
    size_t length = strlen(incipit);
    int i = index;
    
    *wholerest = 1;
    if ((i+1 < length) && isdigit(incipit[i+1])) {
        sscanf(&(incipit[i+1]), "%d", wholerest);
        char buf[10];
        memset( buf, 0, 10 );
        sprintf( buf, "%d", *wholerest );
        i += strlen( buf );
    }
    return i - index;
}



/**********************************
 *
 * getBarline -- read the barLine.
 * Translation from PAE to verovio representaion:
 *
 BARRENDITION_single     /
 BARRENDITION_end        does not exist
 BARRENDITION_rptboth    ://:
 BARRENDITION_rptend     ://
 BARRENDITION_rptstart   //:
 BARRENDITION_dbl        //
 */

int PaeInput::getBarline( const char *incipit, data_BARRENDITION *output, int index ) {
    regex_t re;
    
    regcomp(&re, "^://:", REG_EXTENDED);
    int is_barline_rptboth = regexec(&re, incipit + index, 0, NULL, 0);
    regfree(&re);
    
    regcomp(&re, "^://", REG_EXTENDED);
    int is_barline_rptend = regexec(&re, incipit + index, 0, NULL, 0);
    regfree(&re);
    
    regcomp(&re, "^//:", REG_EXTENDED);
    int is_barline_rptstart = regexec(&re, incipit + index, 0, NULL, 0);
    regfree(&re);
    
    regcomp(&re, "^//", REG_EXTENDED);
    int is_barline_dbl = regexec(&re, incipit + index, 0, NULL, 0);
    regfree(&re);
    
    int i = 0; // number of characters
    if (is_barline_rptboth == 0) {
        *output = BARRENDITION_rptboth;
        i = 3;
    } else if (is_barline_rptstart == 0) {
        *output = BARRENDITION_rptstart;
        i = 2;
    } else if (is_barline_rptend == 0) {
        *output = BARRENDITION_rptend;
        i = 2;
    } else if (is_barline_dbl == 0) {
        *output = BARRENDITION_dbl;
        i = 1;
    } else {
        *output = BARRENDITION_single;
        i = 0;
    }
    return i;
}



//////////////////////////////
//
// getAbbreviation -- read abbreviation
//

int PaeInput::getAbbreviation(const char* incipit, MeasureObject *measure, int index ) {
    
    size_t length = strlen(incipit);
    int i = index;
    int j;
    
    if (measure->abbreviation_offset == -1) { // start
        measure->abbreviation_offset = (int)measure->notes.size();
    } else { //
        int abbreviation_stop = (int)measure->notes.size();
        while ((i+1 < length) && (incipit[i+1]=='f')) {
            i++;
            for(j=measure->abbreviation_offset; j<abbreviation_stop; j++) {
                measure->notes.push_back( measure->notes[j] );
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

int PaeInput::getKeyInfo(const char *incipit, KeySig *key, int index ) {
    int alt_nr = 0;

    // at the key information line, extract data
    size_t length = strlen(incipit);
    int i = index;
    bool end_of_keysig = false;
    while ((i < length) && (!end_of_keysig)) {
        switch (incipit[i]) {
            case 'b': key->SetAlteration(ACCID_FLAT); break;
            case 'x': key->SetAlteration(ACCID_SHARP); break;
            case 'F':
            case 'C':
            case 'G':
            case 'D':
            case 'A':
            case 'E':
            case 'B': alt_nr++; break;
            default:
                end_of_keysig = true;
                break;
        }
        if (!end_of_keysig)
            i++;
    }
    
    key->SetAlterationNumber(alt_nr);
    
    key->ConvertToMei();
    
    return i - index;
}



//////////////////////////////
//
// getNote --
//

int PaeInput::getNote( const char* incipit, NoteObject *note, MeasureObject *measure, int index ) {
    
    regex_t re;
    int oct;
    int i = index;
    bool acc;
    int app;
    int tuplet_num;
    
    if (note->acciaccatura) {
        // acciaccaturas are always eights regardless
        // and have no dots
        note->duration = DURATION_8;
    } else {
        if (measure->durations.size() == 0) {
            note->duration = DURATION_4;
            note->dots = 0;
            LogWarning("Got a note before a duration was specified");
        } else {
            note->duration = measure->durations[measure->durations_offset];
            note->dots = measure->dots[measure->durations_offset];
        }
    }
    note->pitch = getPitch( incipit[i] );
    
    // lookout, hack. If it is a rest (PITCHNAME_NONE val) then create the rest object.
    // it will be added instead of the note
    if (note->pitch == PITCHNAME_NONE)
        note->rest = true;
    
    // trills
    regcomp(&re, "^[^ABCDEFG]*t", REG_EXTENDED);
    int has_trill = regexec(&re, incipit + i + 1, 0, NULL, 0);
    regfree(&re);
    if ( has_trill == 0 ) {
        note->trill = true;
    }
    
    // tie
    regcomp(&re, "^[^ABCDEFG]*\\+", REG_EXTENDED);
    int has_tie = regexec(&re, incipit + i + 1, 0, NULL, 0);
    regfree(&re);
    if ( has_tie == 0) {
        if (note->tie == 0)
            note->tie = 1; // reset 1 for the first note, >1 for next ones is incremented under
    }
    
    oct = note->octave;
    measure->notes.push_back( *note );
    
    acc = note->acciaccatura;
    app = note->appoggiatura;
    tuplet_num = note->tuplet_note;
    
    // Reset note to defaults
    note->clear();
    
    // write back the values we need to save

    note->octave = oct; // save octave
    
    // tuplets. Decrease the current number is we are in a tuplet
    // i.e. tuplet_num > 0
    // al the other values just need to be in the first note
    if (tuplet_num > 0)
        note->tuplet_note = --tuplet_num;
    
    // grace notes
    note->acciaccatura = false;
    if (app > 0) {
        note->appoggiatura = --app;
    }
    // durations
    if (measure->durations.size() > 0) {
        measure->durations_offset++;
        if (measure->durations_offset >= measure->durations.size()) {
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

void PaeInput::convertMeasure(MeasureObject *measure ) {
    
    if ( measure->clef != NULL ) {
        m_layer->AddLayerElement(measure->clef);
    }
    
    if ( measure->key != NULL) {
        m_layer->AddLayerElement(measure->key);
    }
    
    if ( measure->meter != NULL ) {
        m_layer->AddLayerElement(measure->meter);
    }
    
    if ( measure->wholerest > 0 ) { 
        MultiRest *mr = new MultiRest();
        mr->SetNum(measure->wholerest);
        m_layer->AddLayerElement(mr);
    }
    
    m_nested_objects.clear();

    for (unsigned int i=0; i<measure->notes.size(); i++) {
        NoteObject note = measure->notes[i];
        parseNote(note);
    }
    
    // Set barLine
    // FIXME use flags for proper barLine identification
    Barline *bline = m_measure->GetRightBarline();
    bline->SetRend( measure->barLine );

}

void PaeInput::parseNote(NoteObject note) {
    
    LayerElement *element;
    
    if (note.rest) {
        Rest *rest =  new Rest();
        
        rest->SetDots( note.dots );
        rest->SetDur(note.duration);

        if (note.fermata) {
            rest->SetFermata(PLACE_above); // always above for now
        }
        
        element = rest;
    } else {
        Note *mnote = new Note();
        
        mnote->SetPname(note.pitch);
        mnote->SetOct(note.octave);
        mnote->SetAccid(note.accidental);
        
        mnote->SetDots( note.dots );
        mnote->SetDur(note.duration);
        
        if (note.fermata) {
            mnote->SetFermata(PLACE_above); // always above for now
        }
        
        if (note.trill == true)
            mnote->m_embellishment = EMB_TRILL;
        
        if (m_last_tied_note != NULL) {
            mnote->SetTie(TIE_t);
            m_last_tied_note = NULL;
        }
        
        if (note.tie) {
            if (mnote->GetTie()==TIE_t) mnote->SetTie(TIE_m);
            else mnote->SetTie(TIE_i);
            m_last_tied_note = mnote;
        }
        
        element = mnote;
    }
    
    // Does this note have a clef change? push it before everyting else
    if (note.clef)
        addLayerElement(note.clef);

    // Same thing for time changes
    // You can find this sometimes
    if (note.meter)
        addLayerElement(note.meter);
    
    // Handle key change. Evil if done in a beam
    if (note.key)
        addLayerElement(note.key);
    
    // Acciaccaturas are similar but do not get beamed (do they)
    // this case is simpler. NOTE a note can not be acciacctura AND appoggiatura
    // Acciaccatura rests do not exist
    if (note.acciaccatura && dynamic_cast<Note *>(element)) {
        element->m_cueSize = true;
        dynamic_cast<Note *>(element)->m_acciaccatura = true;
    }
    
    if (note.appoggiatura > 0) {
        element->m_cueSize = true;
    }

    if (note.beam == BEAM_INITIAL) {
        pushContainer(new Beam());
    }
    
    // we have a tuplet, the tuplet_note is > 0
    // which means we are counting a tuplet
    if (note.tuplet_note > 0 && note.tuplet_notes == note.tuplet_note) { // first elem in tuplet
        Tuplet *newTuplet = new Tuplet();
        newTuplet->SetNum(note.tuplet_notes);
        newTuplet->SetNumbase(note.tuplet_notes);
        pushContainer(newTuplet);
    }
    
    
    // Add the note to the current container
    addLayerElement(element);
    
    // the last note counts always '1'
    // insert the tuplet elem
    // and reset the tuplet counter
    if (note.tuplet_note == 1)
        popContainer();
    
    if (note.beam == BEAM_TERMINAL)
        popContainer();
}

void PaeInput::pushContainer(LayerElement *container) {
    addLayerElement(container);
    m_nested_objects.push_back(container);
}

void PaeInput::popContainer() {
    //assert(m_nested_objects.size() > 0);
    if (m_nested_objects.size() == 0)
        LogError("PaeInput::popContainer: tried to pop an object from empty stack. Cross-measure objects (tuplets, beams) are not supported.");
    else
        m_nested_objects.pop_back();
}

void PaeInput::addLayerElement(LayerElement *element) {
    
    if (m_nested_objects.size() > 0) {
        LayerElement *bottom = m_nested_objects.back();
        
        if ( dynamic_cast<Beam*>( bottom ) ) {
            ((Beam*)bottom)->AddLayerElement( element );
        }
        else if ( dynamic_cast<Tuplet*>( bottom ) ) {
            ((Tuplet*)bottom)->AddLayerElement( element );
        }
        
    } else {
        m_layer->AddLayerElement(element);
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

void PaeInput::getAtRecordKeyValue(char *key, char* value,
                                      const char* input) {
    
#define SKIPSPACE while((index<length)&&isspace(input[index])){index++;}
    
    char MARKER    = '@';
    char SEPARATOR = ':';
    char EMPTY     = '\0';
    
    size_t length = strlen(input);
    int count = 0;
    
    // zero out strings
    memset(key, EMPTY, MAX_DATA_LEN);
    memset(value, EMPTY, MAX_DATA_LEN);

    
    if (length == 0)
        return;
    
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
        if (count >= MAX_DATA_LEN)
            return;
        
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
    if (strlen(&input[index]) > MAX_DATA_LEN)
        return;
    strcpy(value, &input[index]);
    
    // Thruncate string to first space
    size_t i;
    for (i = strlen(value) - 2; i > 0; i--) {
        if (isspace(value[i])) {
            value[i] = EMPTY;
            continue;
        }
        break;
    }
}

} // namespace vrv
