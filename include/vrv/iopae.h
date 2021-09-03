/////////////////////////////////////////////////////////////////////////////
// Name:        musiopae.h
// Author:      Rodolfo Zitellini
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOPAE_H__
#define __VRV_IOPAE_H__

#include <sstream>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "atts_cmn.h"
#include "io.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include "jsonxx.h"

namespace vrv {

class Beam;
class Chord;
class Clef;
class DurationInterface;
class GraceGrp;
class KeyAccid;
class Layer;
class LayerElement;
class Mdiv;
class Measure;
class MeterSig;
class Mensur;
class MRest;
class MultiRest;
class Note;
class Rest;
class ScoreDef;
class Space;
class Staff;
class StaffDef;
class Tie;
class Tuplet;
class KeySig;
class BarLine;

//----------------------------------------------------------------------------
// namespace for local Plain and Easy classes
//----------------------------------------------------------------------------

namespace pae {

    // static char keySigFlats[7];

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
// PAEOutput
//----------------------------------------------------------------------------

/**
 * This class is a file output stream for writing PAE files.
 */
class PAEOutput : public Output {
public:
    /** @name Constructors and destructor */
    ///@{
    PAEOutput(Doc *doc);
    virtual ~PAEOutput();
    ///@}

    /**
     * The main method for exporting to PAE.
     */
    bool Export(std::string &output);

    /**
     * The main method for write objects.
     */
    virtual bool WriteObject(Object *object);

    /**
     * Writing object method that must be overridden in the child class.
     */
    virtual bool WriteObjectEnd(Object *object);

private:
    bool WriteDoc(Doc *doc);

    /**
     * @name Methods for writing containers (measures, staff, etc) scoreDef and related.
     */
    ///@{
    void WriteMdiv(Mdiv *mDiv);
    void WriteScoreDef(ScoreDef *scoreDef);
    void WriteStaffDef(StaffDef *staffDef);
    void WriteMeasure(Measure *measure);
    void WriteMeasureEnd(Measure *measure);
    void WriteStaff(Staff *staff);
    void WriteLayer(Layer *layer);
    ///@}

    /**
     * @name Methods for writing LayerElement children.
     * Called from WriteLayerElement.
     */
    ///@{
    void WriteBarLine(BarLine *barLine);
    void WriteBeam(Beam *beam);
    void WriteBeamEnd(Beam *beam);
    void WriteChord(Chord *chord);
    void WriteClef(Clef *clef);
    void WriteGraceGrp(GraceGrp *graceGrp);
    void WriteKeyAccid(KeyAccid *keyAccid);
    void WriteKeySig(KeySig *keySig);
    void WriteMensur(Mensur *mensur);
    void WriteMeterSig(MeterSig *meterSig);
    void WriteMRest(MRest *mRest);
    void WriteMultiRest(MultiRest *multiRest);
    void WriteNote(Note *note);
    void WriteRest(Rest *rest);
    void WriteSpace(Space *space);
    void WriteTuplet(Tuplet *tuplet);
    void WriteTupletEnd(Tuplet *tuplet);
    ///@}

    /**
     * @name Methods for writing ControlElement
     */
    ///@{
    ///@}

    /**
     * @name Methods for writing text and figure elements
     */
    ///@{
    ///@}

    /**
     * @name Methods for writing editorial markup
     */
    ///@{
    ///@}

    /**
     * @name Methods for writing other mei elements
     */
    ///@{
    ///@}

    /**
     * @name Methods for writing LayerElement, EditorialElement and interfaces.
     * Call WriteDurationInferface from WriteNote, for example.
     */
    ///@{
    ///@}

    /**
     * @name Other private methods
     */
    ///@{
    void WriteDur(DurationInterface *interface);
    void WriteGrace(AttGraced *attGraced);
    ///@}

public:
    //
private:
    std::ostringstream m_streamStringOutput;
    bool m_docScoreDef; // Indicates that we are writing the document scoreDef
    bool m_mensural; // Indicates that the incipit is mensural (initial staffDef)
    bool m_skip; // Processing a staff or a layer to skip
    int m_layerN; // The @n of the first layer within the first staff
    int m_staffN; // The @n of the first staff (initial staffDef)
    int m_currentOct; // The current octave
    int m_currentDur; // The current duration
    int m_currentDots;
    bool m_grace;
    Measure *m_currentMeasure;
};

//----------------------------------------------------------------------------
// PAEInput
//----------------------------------------------------------------------------

class PAEInput : public Input {
public:
    // constructors and destructors
    PAEInput(Doc *doc);
    virtual ~PAEInput();

#ifndef NO_PAE_SUPPORT
    virtual bool Import(const std::string &pae);

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

//----------------------------------------------------------------------------
// PAEInput2
//----------------------------------------------------------------------------

namespace pae {

    class Token {
    public:
        Token(char c, int position, Object *object = NULL);
        virtual ~Token();
        bool Is(ClassId);
        bool IsContainerEnd();
        bool IsEnd();
        bool IsSpace();
        bool IsVoid();

        std::string GetName();

        char m_char;
        Object *m_object;
        /** the input char preserved for debugging purposes */
        char m_inputChar;
        /** the position in the original input string for debuggin purposes */
        int m_position;
        /** a flag indicating the we and error at this position */
        bool m_isError;
    };

}; // namespace pae

class PAEInput2 : public Input {
public:
    // constructors and destructors
    PAEInput2(Doc *doc);
    virtual ~PAEInput2();

    virtual bool Import(const std::string &input);

private:
    // function declarations:
    jsonxx::Object InputKeysToJson(const std::string &inputKeys);

    void AddToken(char c, int &position);

    bool Parse();

    /**
     * @name Methods that convert pae::Token::m_char to pae::Token::m_objects
     *
     * The order in which they are called is important.
     * For example, KeySig have to be converted before Pitch, because pitch charachters used
     * in KeySig needs to be consumed first
     */
    ///@{
    bool ConvertKeySig();
    bool ConvertClef();
    bool ConvertMeterSigOrMensur();
    bool ConvertMeasure();
    bool ConvertRepeatedFigure();
    bool ConvertRepeatedMeasure();
    bool ConvertMRestOrMultiRest();
    bool ConvertPitch();
    bool ConvertOctave();
    bool ConvertTrill();
    bool ConvertFermata();
    bool ConvertAccidental();
    bool ConvertRest();
    bool ConvertChord();
    bool ConvertBeam();
    bool ConvertGrace();
    bool ConvertGraceGrp();
    ///@}

    bool Is(pae::Token &token, const std::string &map);
    bool Was(pae::Token &token, const std::string &map);
    bool HasInput(char inputChar);

    /**
     * @name Methods that parse sub string instantiate corresponding objects
     */
    ///@{
    bool ParseKeySig(KeySig *keySig, const std::string &paeStr, pae::Token &token);
    bool ParseClef(Clef *clef, const std::string &paeStr, pae::Token &token);
    bool ParseMeterSig(MeterSig *meterSig, const std::string &paeStr, pae::Token &token);
    bool ParseMensur(Mensur *mensur, const std::string &paeStr, pae::Token &token);
    bool ParseMeasure(Measure *measure, const std::string &paeStr, pae::Token &token);
    ///@}

    /**
     * When repeated content is inserted we set the position of all token
     * to the one of the repetition marker (f or i).
     * We also need to clone all objects in the tokens
     */
    void PrepareInsertion(int position, std::list<pae::Token> &insertion);

    bool CheckHierarchy();

    bool CheckContent();

    void ClearTokenObjects();

    void RemoveContainerToken(Object *);

    void LogPAE(std::string msg, pae::Token &token);

    void LogDebugTokens(bool vertical = false);

public:
    //
private:
    std::list<pae::Token> m_pae;

    bool m_isMensural;

    bool m_pedanticMode;

    bool m_hasErrors;
};

} // namespace vrv

#endif
