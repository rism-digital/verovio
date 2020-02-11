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

//----------------------------------------------------------------------------

#include "io.h"
#include "vrvdef.h"

namespace vrv {

class Beam;
class Clef;
class Layer;
class LayerElement;
class Measure;
class MeterSig;
class Mensur;
class Note;
class Staff;
class Tie;
class Tuplet;
class KeySig;
class BarLine;

//----------------------------------------------------------------------------
// namespace for local Plain and Easy classes
//----------------------------------------------------------------------------

namespace pae {

    class Note {

    public:
        Note(const pae::Note &old)
        { // for STL vector
            // mnote = old.mnote;
            // mrest = old.mrest;
            tie = old.tie;
            acciaccatura = old.acciaccatura;
            appoggiatura = old.appoggiatura;
            fermata = old.fermata;
            trill = old.trill;
            chord = old.chord;

            octave = old.octave;
            beam = old.beam;
            pitch = old.pitch;
            duration = old.duration;
            accidental = old.accidental;
            accidGes = old.accidGes;
            dots = old.dots;
            rest = old.rest;

            clef = old.clef;
            mensur = old.mensur;
            meter = old.meter;
            key = old.key;

            tuplet_notes = old.tuplet_notes;
            tuplet_note = old.tuplet_note;
            tuplet_val = old.tuplet_val;
        }
        Note() { clear(); }
        void clear()
        {
            appoggiatura = 0;
            acciaccatura = fermata = tie = trill = chord = false;

            octave = 4;
            beam = 0;
            pitch = PITCHNAME_NONE;
            duration = DURATION_NONE;
            accidental = ACCIDENTAL_WRITTEN_NONE;
            accidGes = false;
            dots = 0;
            rest = false;

            tuplet_notes = 0;
            tuplet_note = 0;
            tuplet_val = 0;

            clef = NULL;
            mensur = NULL;
            meter = NULL;
            key = NULL;
        }

        Note &operator=(const Note &d)
        { // for STL vector
            // mnote = d.mnote;
            // mrest = d.mrest;
            tie = d.tie;
            acciaccatura = d.acciaccatura;
            appoggiatura = d.appoggiatura;
            fermata = d.fermata;
            trill = d.trill;
            chord = d.chord;

            octave = d.octave;
            beam = d.beam;
            pitch = d.pitch;
            duration = d.duration;
            accidental = d.accidental;
            accidGes = d.accidGes;
            dots = d.dots;
            rest = d.rest;

            clef = d.clef;
            mensur = d.mensur;
            meter = d.meter;
            key = d.key;

            tuplet_notes = d.tuplet_notes;
            tuplet_note = d.tuplet_note;
            tuplet_val = d.tuplet_val;

            return *this;
        }

        // Note *mnote;
        // Rest *mrest; // this is not too nice

        // tuplet stuff
        int tuplet_notes; // quantity of notes in the tuplet
        int tuplet_note; // indicates this note is the nth in the tuplet
        int tuplet_val; // indicates the num in the tuplet (value after ;)

        bool acciaccatura;
        int appoggiatura;
        bool chord;
        bool fermata;
        bool tie;
        bool trill;

        char octave;
        unsigned char beam;
        data_PITCHNAME pitch;
        data_DURATION duration;
        data_ACCIDENTAL_WRITTEN accidental;
        bool accidGes;
        unsigned int dots;
        bool rest;

        Clef *clef;
        Mensur *mensur;
        MeterSig *meter;
        KeySig *key;
    };

    class Measure {

    public:
        Measure(const Measure &d)
        { // for STL vector
            clef = d.clef;
            meter = d.meter;
            mensur = d.mensur;
            notes = d.notes;

            key = d.key;

            durations = d.durations;
            dots = d.dots;
            durations_offset = d.durations_offset;
            barLine = d.barLine;
            abbreviation_offset = d.abbreviation_offset;
            wholerest = d.wholerest;
        }
        Measure() { clear(); }

        Measure &operator=(const Measure &d)
        { // for STL vector
            clef = d.clef;
            meter = d.meter;
            mensur = d.mensur;
            notes = d.notes;

            key = d.key;

            durations = d.durations;
            dots = d.dots;
            durations_offset = d.durations_offset;
            barLine = d.barLine;
            abbreviation_offset = d.abbreviation_offset;
            wholerest = d.wholerest;
            return *this;
        }

        void clear()
        {
            durations.clear();
            dots.clear();
            notes.clear();
            durations_offset = DURATION_long;
            reset();
        }
        void reset()
        {
            clef = NULL;
            meter = NULL;
            mensur = NULL;
            key = NULL;
            notes.clear();
            barLine = BARRENDITION_invis;
            wholerest = 0;
            abbreviation_offset = -1;
        }
        Clef *clef;
        Mensur *mensur;
        MeterSig *meter;
        KeySig *key;

        std::vector<pae::Note> notes;

        std::vector<data_DURATION> durations;
        std::vector<int> dots; // use the same offset as durations, they are used in parallel
        char durations_offset;
        data_BARRENDITION barLine;
        int abbreviation_offset;
        int wholerest; // number of whole rests to process
    };

} // namespace pae

//----------------------------------------------------------------------------
// PaeInput
//----------------------------------------------------------------------------

class PaeInput : public FileInputStream {
public:
    // constructors and destructors
    PaeInput(Doc *doc, std::string filename);
    virtual ~PaeInput();

    virtual bool ImportFile();
    virtual bool ImportString(const std::string &pae);

#ifndef NO_PAE_SUPPORT

private:
    // function declarations:

    void parsePlainAndEasy(std::istream &infile);

    // parsing functions
    int getKeyInfo(const char *incipit, KeySig *key, int index = 0);
    int getTimeInfo(const char *incipit, MeterSig *meter, Mensur *mensur, int index = 0);
    int getClefInfo(const char *incipit, Clef *mus_clef, int index = 0);
    int getBarLine(const char *incipit, data_BARRENDITION *output, int index);
    int getAccidental(const char *incipit, data_ACCIDENTAL_WRITTEN *accident, int index = 0);
    int getOctave(const char *incipit, char *octave, int index = 0);
    int getDurations(const char *incipit, pae::Measure *measure, int index = 0);
    int getDuration(const char *incipit, data_DURATION *duration, int *dot, int index);
    int getTupletFermata(const char *incipit, pae::Note *note, int index = 0);
    int getTupletFermataEnd(const char *incipit, pae::Note *note, int index = 0);
    int getGraceNote(const char *incipit, pae::Note *note, int index = 0);
    int getWholeRest(const char *incipit, int *wholerest, int index);
    int getAbbreviation(const char *incipit, pae::Measure *measure, int index = 0);
    int getNote(const char *incipit, pae::Note *note, pae::Measure *measure, int index = 0);

    data_PITCHNAME getPitch(char c_note);

    // output functions
    void addLayerElement(LayerElement *element);
    void parseNote(pae::Note *note);
    void popContainer();
    void convertMeasure(pae::Measure *measure);
    void pushContainer(LayerElement *container);

    // input functions
    void getAtRecordKeyValue(char *key, char *value, const char *input);

#endif // NO_PAE_SUPPORT

public:
    //
private:
    std::string m_filename;
    Staff *m_staff;
    Measure *m_measure;
    Layer *m_layer;
    Tie *m_tie;
    bool m_is_in_chord;
    bool m_is_mensural;

    MapOfPitchAccid m_currentAccids;
    KeySig *m_currentKeySig;
    std::pair<data_PITCHNAME, data_ACCIDENTAL_WRITTEN> m_tieAccid;

    std::vector<LayerElement *> m_nested_objects;
};

} // namespace vrv

#endif
