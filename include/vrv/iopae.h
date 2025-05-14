/////////////////////////////////////////////////////////////////////////////
// Name:        musiopae.h
// Author:      Rodolfo Zitellini
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOPAE_H__
#define __VRV_IOPAE_H__

/*
 * There are two implementation of the Plaine & Easie parser.
 * The new one was introduced in Verovio 3.7.
 * In order to build with the old parser, the following define needs to be uncommented
 */
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// #define USE_PAE_OLD_PARSER
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#include <sstream>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "atts_cmn.h"
#include "clef.h"
#include "iobase.h"
#include "keysig.h"
#include "mensur.h"
#include "metersig.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include "jsonxx.h"

namespace vrv {

class Beam;
class Chord;
class DurationInterface;
class GraceGrp;
class KeyAccid;
class Layer;
class LayerElement;
class Mdiv;
class Measure;
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
    std::string Export() override;

    /**
     * The main method for write objects.
     */
    bool WriteObject(Object *object) override;

    /**
     * Writing object method that must be overridden in the child class.
     */
    bool WriteObjectEnd(Object *object) override;

    /**
     * Helper method to return a string representation of the PAE duration.
     */
    static std::string GetPaeDur(data_DURATION dur, int ndots);

private:
    /**
     * @name Methods for writing containers (measures, staff, etc) scoreDef and related.
     */
    ///@{
    void WriteMdiv(Mdiv *mDiv);
    void WriteMdivEnd(Mdiv *mDiv);
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
    bool HasFermata(Object *object);
    ///@}

public:
    //
private:
    std::ostringstream m_streamStringOutput;
    bool m_docScoreDef; // Indicates that we are writing the document scoreDef
    bool m_mensural; // Indicates that the incipit is mensural (initial staffDef)
    bool m_skip; // Processing a staff or a layer to skip
    int m_layerN; // The \@n of the first layer within the first staff
    int m_staffN; // The \@n of the first staff (initial staffDef)
    int m_currentOct; // The current octave
    int m_currentDur; // The current duration
    int m_currentDots;
    bool m_grace;
    Measure *m_currentMeasure;
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#ifdef USE_PAE_OLD_PARSER
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// namespace for local Plaine & Easie classes
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
// PAEInput
//----------------------------------------------------------------------------

class PAEInput : public Input {
public:
    // constructors and destructors
    PAEInput(Doc *doc);
    virtual ~PAEInput();

    // dummy validation log
    jsonxx::Object GetValidationLog();

    // dummy setter
    void SetScoreBased(bool scoreBased) {}

#ifndef NO_PAE_SUPPORT
    bool Import(const std::string &pae) override;

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

    MapOfOctavedPitchAccid m_currentAccids;
    KeySig *m_currentKeySig;
    std::pair<int, data_ACCIDENTAL_WRITTEN> m_tieAccid;

    std::vector<LayerElement *> m_nested_objects;
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#else // USE_PAE_OLD_PARSER
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// PAEInput
//----------------------------------------------------------------------------

namespace pae {

    class Token {
    public:
        Token(char c, int position, Object *object = NULL);
        virtual ~Token();

        /** Return true if the token has an Object and its classId is ClassId */
        bool Is(ClassId);
        /** Return true if the token if the end of a container (e.g., Beam) */
        bool IsContainerEnd();
        /** Return true if the token is that end sentinel of the list */
        bool IsEnd();
        /** Return true if the input char is '_' */
        bool IsSpace();
        /** Return true is the token has to be ignore during parsing */
        bool IsVoid();
        /** Set the object as being inserted in the MEI tree */
        void SetInTree();

        /* Helper to the a lowercase version of the Object classname (if any) */
        std::string GetName();

        /** The character to process - 0 once done */
        char m_char;
        /** The Object to be added to the tree */
        Object *m_object;
        /** The object added to the tree */
        Object *m_treeObject;
        /** the input char preserved for debugging purposes */
        char m_inputChar;
        /** the position in the original input string for debuggin purposes */
        int m_position;
        /** a flag indicating that an error occured at this position */
        bool m_isError;
    };

} // namespace pae

class PAEInput : public Input {
public:
    // constructors and destructors
    PAEInput(Doc *doc);
    virtual ~PAEInput();

    /**
     * Return a JSON object with the validation log
     * It is a single object when an input error is encountered.
     * Otherwise, validation log errors/warnings are listed in their respective JSON input keys
     */
    jsonxx::Object GetValidationLog();

    /**
     * Setter for scoreBased flag (false by default).
     */
    void SetScoreBased(bool scoreBased) { m_scoreBased = scoreBased; }

#ifndef NO_PAE_SUPPORT
    bool Import(const std::string &input) override;

private:
    /**
     * Convert the old-style \@clef:... \@keysig:... \@data:... to a JSON input
     */
    jsonxx::Object InputKeysToJson(const std::string &inputKeys);

    /**
     * Convert single-line incipits with $clef@timesig%keysig data to JSON input
     */
    jsonxx::Object SingleLineToJson(const std::string &singleLine);

    /**
     * Helper to add a token to the list.
     * Performs re-expansion of internal characters (e.g., Q back to qq in the inputChar)
     * Re-expansion of internal characters inserts void tokens ignored during parsing.
     * The are added only for debug purposes.
     */
    void AddToken(char c, int &position);

    /**
     * Main method that calls the converting methods.
     * A the end of the converting process, the token list is essentially a list of Objects.
     * The method then performs a check of the hierachy.
     * Once this is done, it builds the MEI tree.
     * The method then performs some additional checks of the content (to be implemented)
     */
    bool Parse();

    /**
     * Parse the metadata in the \@header json object
     */
    void ParseHeader(jsonxx::Object &header);

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
    bool ConvertTuplet();
    bool ConvertDuration();
    bool ConvertTie();
    bool ConvertLigature();
    bool ConvertAccidGes();
    ///@}

    /**
     * @name Helpers to check what a token is or was.
     *
     */
    ///@{
    bool Is(pae::Token &token, const std::string &map);
    bool Was(pae::Token &token, const std::string &map);
    bool HasInput(char inputChar);
    ///@}

    /**
     * @name Methods that parse sub string instantiate corresponding objects
     */
    ///@{
    bool ParseKeySig(KeySig *keySig, const std::string &paeStr, pae::Token &token);
    bool ParseClef(Clef *clef, const std::string &paeStr, pae::Token &token, bool *mensuralScoreDef = NULL);
    bool ParseMeterSig(MeterSig *meterSig, const std::string &paeStr, pae::Token &token);
    bool ParseMensur(Mensur *mensur, const std::string &paeStr, pae::Token &token);
    bool ParseMeasure(Measure *measure, const std::string &paeStr, pae::Token &token);
    bool ParseDuration(
        std::list<std::pair<data_DURATION, int>> &durations, const std::string &paeStr, pae::Token &token);
    ///@}

    /**
     * @name Check if the input string contains any invalid characters.
     * Invalid characters are put in invalidChars.
     * Return false if invalid characters are found.
     */
    ///@{
    bool CheckPAEChars(const std::string &input, std::string &invalidChars, const std::string &validChars = "");
    ///@}

    /**
     * When repeated content is inserted we set the position of all token
     * to the one of the repetition marker (f or i).
     * We also need to clone all objects in the tokens
     */
    void PrepareInsertion(int position, std::list<pae::Token> &insertion);

    /**
     * Check that the token list is a valid opening / closing tag successing.
     * Also check that every element is supported by is containing element.
     * Remove invalid opening / closing successions or invalid elements in non pedantic mode.
     */
    bool CheckHierarchy();

    /**
     * Some additional checked to be performed before the MEI tree has been build.
     * Unimplemented
     */
    bool CheckContentPreBuild();

    /**
     * Some additional checked to be performed one the MEI tree has been build.
     */
    bool CheckContentPostBuild();

    /**
     * A helper to remove a token when checking the hierarchy and it is not valid
     */
    void RemoveContainerToken(Object *object);

    /**
     * Return the token corresponding to an object in the tree.
     * Return NULL if not found.
     */
    pae::Token *GetTokenForTreeObject(Object *object);

    /**
     * @name Some logging methods specific to the PAE parser
     */
    ///@{
    void LogPAE(int errCode, pae::Token &token, std::string value = "");
    void LogDebugTokens(bool vertical = false);
    ///@}

#endif // NO_PAE_SUPPORT

    /**
     * Verify that no object remains in the token list before clearing the list.
     * Remaining objects are deleted.
     */
    void ClearTokenObjects();

public:
    static const std::map<int, std::string> s_errCodes;

private:
    /**
     * The list of tokens representing the incipit
     * Each token is a characther that needs to be processed.
     * When processing the tokens, the token can be assigned an Object.
     * When processed, the token::m_char is set to 0.
     * The orignal input value remains in token::m_inputChar
     * Whenever necessary, some tokens can be added. For example when:
     * - repeated figures or measures are used
     * - closing tags are missing (in non-pendantic modes)
     * - opening tags are not part of the PAE syntax (e.g., a chord)
     * Each token also stores the original position in the PAE string.
     */
    std::list<pae::Token> m_pae;

    /**
     * A flag indicating the incipit is mensural.
     * Based on the \@clef of the input.
     */
    bool m_isMensural;

    /**
     * A flag that makes parsing fails when an error is encountered.
     * Parsing will stop there in pedantic mode.
     * Currently hard-coded and will require an option to be added.
     */
    bool m_pedanticMode;

    /**
     * A flag indicating we had errors when parsing the incipit in non pedantic mode.
     */
    bool m_hasErrors;

    /**
     * A flag indicating the document should be kept as score-based.
     */
    bool m_scoreBased;

    /**
     * @name The scoreDef clef, keysig and timesig.
     * Mensur is used with mensural incipits (i.e., with clefs with a + second sign.
     */
    ///@{
    Clef m_clef;
    KeySig m_keySig;
    Mensur m_mensur;
    MeterSig m_meterSig;
    bool m_hasClef;
    bool m_hasKeySig;
    bool m_hasMeterSig;
    bool m_hasMensur;
    ///@}

    /**
     * @name The jsonxx members to store the validation logs
     * ScoreDef and input members are single objects that can log one entry
     * Data is an array that can store multiple entries
     */
    ///@{
    jsonxx::Object m_clefLog;
    jsonxx::Object m_keysigLog;
    jsonxx::Object m_timesigLog;
    jsonxx::Object m_inputLog;
    jsonxx::Array m_dataLog;
    ///@}
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#endif // USE_PAE_OLD_PARSER
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

} // namespace vrv

#endif
