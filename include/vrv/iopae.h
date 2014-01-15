/////////////////////////////////////////////////////////////////////////////
// Name:        musiopae.h
// Author:      Rodolfo Zitellini
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_IOPAE_H__
#define __VRV_IOPAE_H__

#include <string>
#include <vector>

#include "io.h"

namespace vrv {

class Beam;
class Clef;
class Layer;
class Measure;
class Mensur;
class Staff;
class Tie;
class Tuplet;


class NoteObject {
public:
    NoteObject(const NoteObject &old) { // for STL vector
        //mnote = old.mnote;
        //mrest = old.mrest;         
        tie = old.tie;
        acciaccatura = old.acciaccatura;
        appoggiatura = old.appoggiatura;
        appoggiatura_multiple = old.appoggiatura_multiple;
        fermata = old.fermata;
        trill = old.trill;
        
        octave = old.octave;
        beam = old.beam;
        pitch = old.pitch;
        duration = old.duration;
        accidental = old.accidental;
        dots = old.dots;
        rest = old.rest;
        
        clef = old.clef;

        tuplet_notes = old.tuplet_notes;
        tuplet_note = old.tuplet_note;
    }
    NoteObject(void) { clear(); };
    void   clear(void) {
        appoggiatura = 0;
        acciaccatura = appoggiatura_multiple = fermata = trill = false;
        tie = 0;
        
        octave = 4;
        beam = 0;
        pitch = 0;
        duration = 0;
        accidental = 0;
        dots = 0;
        rest = false;
        
        tuplet_notes = 0;
        tuplet_note = 0;
        
        clef = NULL;
    };
    
    NoteObject& operator=(const NoteObject& d){ // for STL vector
        //mnote = d.mnote;
        //mrest = d.mrest;         
        tie = d.tie;
        acciaccatura = d.acciaccatura;
        appoggiatura = d.appoggiatura;
        appoggiatura_multiple = d.appoggiatura_multiple;
        fermata = d.fermata;
        trill = d.trill;
        
        octave = d.octave;
        beam = d.beam;
        pitch = d.pitch;
        duration = d.duration;
        accidental = d.accidental;
        dots = d.dots;
        rest = d.rest;
        
        clef = d.clef;
        
        tuplet_notes = d.tuplet_notes;
        tuplet_note = d.tuplet_note;
        
        return *this;
    }
    
    //Note *mnote;
    //Rest *mrest; // this is not too nice

    // tuplet stuff
    int tuplet_notes; // quantity of notes in the tuplet
    int tuplet_note; // indicates this note is the nth in the tuplet
    
    int    tie;
    bool   acciaccatura;
    int    appoggiatura;
    bool   appoggiatura_multiple;
    bool   fermata;
    bool   trill;
    
    unsigned char octave;
    unsigned char beam;
    unsigned char pitch;
    unsigned char duration;
    unsigned char accidental;
    unsigned int dots;
    bool rest;
    
    Clef *clef;
};


class MeasureObject {
public:
    
    MeasureObject(const MeasureObject& d){ // for STL vector
        clef = d.clef;
        time = d.time;
        notes = d.notes;
        
        key = d.key;
        key_alteration = d.key_alteration;
        
        durations = d.durations;
        dots = d.dots; 
        durations_offset = d.durations_offset;
        barline = d.barline;
        abbreviation_offset = d.abbreviation_offset;  
        wholerest = d.wholerest;
    } 
    MeasureObject(void) { clear(); };
    
    MeasureObject& operator=(const MeasureObject& d){ // for STL vector
        clef = d.clef;
        time = d.time;
        notes = d.notes;
        
        key = d.key;
        key_alteration = d.key_alteration;
        
        durations = d.durations;
        dots = d.dots; 
        durations_offset = d.durations_offset;
        barline = d.barline;
        abbreviation_offset = d.abbreviation_offset;  
        wholerest = d.wholerest;   
        return *this;
    } 
     
    void   clear(void) {
        durations.clear();
        dots.clear();
        notes.clear();
        durations_offset = 0;
        reset();
    };
    void   reset(void) {
        clef = NULL;
        time = NULL;
        key.clear();
        notes.clear();
        barline = "";
        wholerest = 0; 
        abbreviation_offset = -1;
    };
    Clef *clef;
    Mensur *time;
    std::vector<NoteObject> notes;

    std::vector<int> key;
    char key_alteration;
    
    std::vector<int> durations;
    std::vector<int> dots; // use the same offset as durations, they are used in parallel
    unsigned int durations_offset;
    std::string barline;
    int    abbreviation_offset;  
    int    wholerest;   // number of whole rests to process
};


//////////////////////////////////////////////////////////////////////////









//----------------------------------------------------------------------------
// PaeInput
//----------------------------------------------------------------------------


class PaeInput: public FileInputStream
{
public:
    // constructors and destructors
    PaeInput( Doc *doc, std::string filename );
    virtual ~PaeInput();
    
    virtual bool ImportFile( );
    virtual bool ImportString(std::string pae);
    
private:
    // function declarations:
    
     void      convertPlainAndEasyToKern( std::istream &infile, std::ostream &outfile);
     
     // parsing functions
     int       getKeyInfo          (const char* incipit, MeasureObject *measure, int index = 0);
     int       getTimeInfo         (const char* incipit, MeasureObject *measure, int index = 0);
     int       getClefInfo         (const char* incipit, Clef *mus_clef, int index = 0 );
     int       getBarline          (const char* incipit, std::string *output, int index = 0 );
     int       getAccidental       (const char* incipit, unsigned char *accident, int index = 0);
     int       getOctave           (const char* incipit, unsigned char *octave, int index = 0 );
     int       getDurations        (const char* incipit, MeasureObject *measure, int index = 0);
     int       getDuration         (const char* incipit, int *duration, int *dot, int index );
     int       getTupletFermata    (const char* incipit, NoteObject *note, int index = 0);
     int       getTupletFermataEnd (const char* incipit, NoteObject *note, int index = 0);
     int       getGraceNote        (const char* incipit, NoteObject *note, int index = 0);
     int       getWholeRest        (const char* incipit, int *wholerest, int index );
     int       getAbbreviation     (const char* incipit, MeasureObject *measure, int index = 0 ); 
     int       getNote             (const char* incipit, NoteObject *note, MeasureObject *measure, int index = 0 );
     
     int       getPitch            (char c_note );
     
     // output functions
     void      AddLayerElement     (LayerElement *element);
     void      parseNote           (NoteObject note);
     void      PopContainer        ();
     void      printMeasure        (std::ostream& out, MeasureObject *measure);
     void      PushContainer       (LayerElement *container);


     // input functions
     void      getAtRecordKeyValue (char *key, char* value, const char* input);
     
    
public:
    
private:
    std::string m_filename;
	Staff *m_staff;
    Measure *m_measure;
	Layer *m_layer;
    Tie *m_current_tie;

    std::vector<LayerElement *> m_nested_objects;   
    
    //unsigned char m_rest_position;
    //unsigned int m_rest_octave;
};

} // namespace vrv

#endif
