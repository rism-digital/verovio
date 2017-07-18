/////////////////////////////////////////////////////////////////////////////
// Name:        ioabc.h
// Author:      Klaus Rettinghaus
// Created:     2017
// Copyright (c) Klaus Rettinghaus. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOABC_H__
#define __VRV_IOABC_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "io.h"
#include "vrvdef.h"

namespace vrv {

class Beam;
class Clef;
class ControlElement;
class Layer;
class LayerElement;
class Measure;
class MeterSig;
class Note;
class Score;
class Section;
class Staff;
class Tie;
class Tuplet;
class KeySig;
class BarLine;

//----------------------------------------------------------------------------
// namespace for local ABC classes
//----------------------------------------------------------------------------

namespace abc {

    class Note {

    public:
        Note(const abc::Note &old)
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
            dots = old.dots;
            rest = old.rest;

            clef = old.clef;
            meter = old.meter;
            key = old.key;

            tuplet_notes = old.tuplet_notes;
            tuplet_note = old.tuplet_note;
        }
        Note(void) { clear(); }
        void clear(void)
        {
            appoggiatura = 0;
            acciaccatura = fermata = trill = chord = false;
            tie = 0;

            octave = 4;
            beam = 0;
            pitch = PITCHNAME_NONE;
            duration = DURATION_NONE;
            accidental = ACCIDENTAL_WRITTEN_NONE;
            dots = 0;
            rest = false;

            tuplet_notes = 0;
            tuplet_note = 0;

            clef = NULL;
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
            dots = d.dots;
            rest = d.rest;

            clef = d.clef;
            meter = d.meter;
            key = d.key;

            tuplet_notes = d.tuplet_notes;
            tuplet_note = d.tuplet_note;

            return *this;
        }

        // Note *mnote;
        // Rest *mrest; // this is not too nice

        // tuplet stuff
        int tuplet_notes; // quantity of notes in the tuplet
        int tuplet_note; // indicates this note is the nth in the tuplet

        int tie;
        bool acciaccatura;
        int appoggiatura;
        bool fermata;
        bool trill;
        bool chord;

        char octave;
        unsigned char beam;
        data_PITCHNAME pitch;
        data_DURATION duration;
        data_ACCIDENTAL_WRITTEN accidental;
        unsigned int dots;
        bool rest;

        Clef *clef;
        MeterSig *meter;
        KeySig *key;
    };

    class Measure {

    public:
        Measure(const Measure &d)
        { // for STL vector
            clef = d.clef;
            meter = d.meter;
            notes = d.notes;

            key = d.key;

            durations = d.durations;
            dots = d.dots;
            durations_offset = d.durations_offset;
            barLine = d.barLine;
            abbreviation_offset = d.abbreviation_offset;
            multirest = d.multirest;
        }
        Measure(void) { clear(); }

        Measure &operator=(const Measure &d)
        { // for STL vector
            clef = d.clef;
            meter = d.meter;
            notes = d.notes;

            key = d.key;

            durations = d.durations;
            dots = d.dots;
            durations_offset = d.durations_offset;
            barLine = d.barLine;
            abbreviation_offset = d.abbreviation_offset;
            multirest = d.multirest;
            return *this;
        }

        void clear(void)
        {
            durations.clear();
            dots.clear();
            notes.clear();
            durations_offset = DURATION_long;
            reset();
        }
        void reset(void)
        {
            clef = NULL;
            meter = NULL;
            key = NULL;
            notes.clear();
            barLine = BARRENDITION_invis;
            multirest = 0;
            abbreviation_offset = -1;
        }
        Clef *clef;
        MeterSig *meter;
        KeySig *key;

        std::vector<abc::Note> notes;

        std::vector<data_DURATION> durations;
        std::vector<int> dots; // use the same offset as durations, they are used in parallel
        char durations_offset;
        data_BARRENDITION barLine;
        int abbreviation_offset;
        int multirest; // number of multiple rests to process
    };

} // namespace abc

//----------------------------------------------------------------------------
// AbcInput
//----------------------------------------------------------------------------

class AbcInput : public FileInputStream {
public:
    // constructors and destructors
    AbcInput(Doc *doc, std::string filename);
    virtual ~AbcInput();

    virtual bool ImportFile();
    virtual bool ImportString(std::string const &abc);

#ifndef NO_ABC_SUPPORT

private:
    // function declarations:

    void parseABC(std::istream &infile);

    // parsing functions
    int getBarLine(const char *incipit, data_BARRENDITION *output, int index);
    int getAccidental(const char *incipit, data_ACCIDENTAL_WRITTEN *accident, int index = 0);
    int getOctave(const char *incipit, char *octave, int index = 0);
    int getMultiRest(const char *incipit, int *multirest, int index);
    int getAbbreviation(const char *incipit, abc::Measure *measure, int index = 0);
    int getNote(const char *incipit, abc::Note *note, abc::Measure *measure, int index = 0);

    // output functions
    void addLayerElement(LayerElement *element);
    void parseNote(abc::Note *note);
    void popContainer();
    void convertMeasure(abc::Measure *measure);
    void pushContainer(LayerElement *container);

    // parse information fields
    void parseInstruction(std::string keyString); // I:
    void parseKey(std::string keyString); // K:
    void parseUnitNoteLength(std::string unitNoteLength); // L:
    void parseMeter(std::string meterString); // M:
    void parseTempo(std::string tempoString); // Q:
    void parseReferenceNumber(std::string referenceNumberString); // X:

    // input functions
    void readInformationField(char dataKey, std::string dataValue, Score *score);
    void readMusicCode(const char *musicCode, Section *section);

#endif // NO_ABC_SUPPORT

public:
    //
private:
    std::string m_filename;
    Staff *m_staff;
    Measure *m_measure;
    Layer *m_layer;

    data_DURATION m_durDefault = DURATION_NONE;
    int m_unitDur;
    char m_decoration = '!';
    char m_linebreak = '$';
    int m_lineNum = 0;

    std::vector<ControlElement *> m_controlElements;

    Note *m_last_tied_note;
    bool m_is_in_chord;

    std::vector<LayerElement *> m_layerElements;
};

} // namespace vrv

#endif
