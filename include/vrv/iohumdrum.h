/////////////////////////////////////////////////////////////////////////////
// Name:        iohumdrum.h
// Author:      Craig Stuart Sapp
// Created:     06/06/2015
// Copyright (c) Authors and others. All rights reserved.
// vim:         ts=4
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOHUMDRUM_H__
#define __VRV_IOHUMDRUM_H__

#include <map>
#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "accid.h"
#include "attdef.h"
#include "clef.h"
#include "dir.h"
#include "ending.h"
#include "io.h"
#include "keysig.h"
#include "metersig.h"
#include "options.h"
#include "pedal.h"
#include "runningelement.h"
#include "section.h"
#include "verse.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#ifndef NO_HUMDRUM_SUPPORT
#include "humlib.h"
#include "pugixml.hpp"
// #include "tool-musicxml2hum.h"
#endif /* NO_HUMDRUM_SUPPORT */

//----------------------------------------------------------------------------

namespace vrv {

#ifndef NO_HUMDRUM_SUPPORT

class Beam;
class Chord;
class Layer;
class Measure;
class ScoreDef;
class Note;
class Hairpin;
class MRest;
class Rest;
class Score;
class Section;
class Space;
class Staff;
class StaffDef;
class StaffGrp;
class Tie;

//----------------------------------------------------------------------------
// namespace for local IoHumdrum classes
//----------------------------------------------------------------------------

namespace humaux {

    class HumdrumTie {
    public:
        HumdrumTie();
        HumdrumTie(const HumdrumTie &anothertie);
        ~HumdrumTie();
        HumdrumTie &operator=(const HumdrumTie &anothertie);
        vrv::Tie *insertTieIntoDom();
        void setStart(const std::string &id, Measure *starting, int layer, const std::string &token, int pitch,
            hum::HumNum starttime, hum::HumNum endtime, int subindex, hum::HTp starttok);
        void setEnd(const std::string &id, Measure *ending, const std::string &token);
        vrv::Tie *setEndAndInsert(const std::string &id, Measure *ending, const std::string &token);
        hum::HumNum getEndTime();
        hum::HumNum getStartTime();
        hum::HumNum getDuration();
        std::string getStartToken();
        hum::HTp getStartTokenPointer();
        std::string getEndToken();
        int getStartSubindex();
        int getPitch();
        int getLayer();
        bool isInserted();
        void clear();
        void setTieAbove();
        void setTieBelow();

    private:
        std::string m_starttoken;
        std::string m_endtoken;
        hum::HumNum m_starttime;
        hum::HumNum m_endtime;
        int m_pitch;
        int m_layer;
        bool m_inserted;
        bool m_above;
        bool m_below;
        std::string m_startid;
        std::string m_endid;
        Measure *m_startmeasure;
        Measure *m_endmeasure;
        hum::HTp m_starttokenpointer = NULL;
        int m_subindex; // the subtoken index for the start of the slur
    };

    class HumdrumBeamAndTuplet {
    public:
        int group; // tuplet group for layer
        int bracket; // -1=undefined, 0=none, 1=show
        int num; // @num
        int numbase; // @numbase
        int numscale; // for converting triplets into sextuplets for example
        int tupletstart;
        int tupletend;
        int beamstart;
        int beamend;
        int gbeamstart;
        int gbeamend;
        char priority;
        HumdrumBeamAndTuplet() { clear(); }
        ~HumdrumBeamAndTuplet() { clear(); }
        void clear()
        {
            group = bracket = 0;
            numbase = num = 1;
            numscale = 1;
            tupletstart = tupletend = 0;
            beamstart = beamend = 0;
            gbeamstart = gbeamend = 0;
            priority = ' ';
        }
    };

    // StaffStateVariables is a data structure used in the HumdrumInput
    // class to store state variables for processing staves.  This structure
    // is used to store all variables which are vectors
    class StaffStateVariables {
    public:
        StaffStateVariables();
        ~StaffStateVariables();
        void clear();

        // verse == keeps track of whether or not staff contains associated
        // **text spines which will be converted into lyrics.
        bool verse;

        // suppress_beam_tuplet == keeps track of whether or not beams should
        // display beam tuplet numbers.
        bool suppress_beam_tuplet;

        // suppress_bracket_tuplet == keeps track of whether or not tuplet
        // brackets should be displayed.
        bool suppress_bracket_tuplet;

        // Used for tremolo compression
        bool tremolo;

        // cue_size == keeps track of whether or not the notes in the current
        // staff/layer should be cue sized.  Index 0 is used to control all
        // layers.
        vector<bool> cue_size;

        // stem_type == keeps track of what type of stem to automatically
        // add to a note/chord.  The states are:
        // '\' == down stem
        // '/' == up stem
        // 'x' == no stem
        // 'X' == no automatic assignments (assignment will be done automatically by verovio).
        vector<char> stem_type;

        // ligature_recta == true if in a recta ligature
        bool ligature_recta = false;
        // ligature_obliqua == true if in a obliqua ligature
        bool ligature_obliqua = false;

        // last_clef == keep track of last clef on staff to avoid duplications.
        std::string last_clef;

        // acclev == In **mens data, controls the accidental level conversion
        // from gestural to editorial.
        // *acclev:0 == no editorial acccidentals visible (all converted to gestural)
        // *acclev:1 == accidentals marked with YY are displayed as editorial accidentals (above notes)
        // *acclev:2 == accidentals marked with Y are displayed as editorial accidentals (above notes)
        // *acclev:3 == accidentals marked with yy are displayed as editorial accidentals (above notes)
        // *acclev:4 == accidentals marked with y are displayed as editorial accidentals (above notes)
        // meaning of the accidental marks:
        // y = algorithmic interpretation of an implicit accidental, primarily used for notes
        //     with accidentals not shown but assigned by the key signature.
        // yy = a cautionary type of accidental, such as returning to the accidental within the
        //     key signature (such as a b-flat coming after a b-natural in a 1-flat key signature).
        // Y = a performance interpretation accidental (musica ficta) that is not indicated or
        //     directly implied in the source, but is needed due to performance practice.
        // YY = an accidental that should be added due to what the editor thinks is an error.
        //
        // Equivalences to numbers:
        // *Xacclev            == *acclev:0  all levels are mapped to @accid.ges
        // *acclev             == *acclev:0  all levels are mapped to @accid.ges
        // *acclev:            == *acclev:0  all levels are mapped to @accid.ges
        // *acclev:YY          == *acclev:1  levels 2-4 are mapped to @accid.ges and level 1    to @accd+@edit
        // *acclev:Y           == *acclev:2  levels 3-4 are mapped to @accid.ges and levels 1-2 to @accd+@edit
        // *acclev:yy          == *acclev:3  level    4 is  mapped to @accid.ges and levels 1-3 to @accd+@edit
        // *acclev:y           == *acclev:4  all levels are mapped to @accid+@edit
        //
        // The default level is *acclev:1 (YY will show as editorial accidental, y, yy, and Y will be @accid.ges)
        int acclev = 1;

        // righthalfstem == true means to place half-note stems always on right side
        // of noteheads.  False is standard modern style.
        bool righthalfstem;

        // ottavanote == keep track of ottava marks: stores the starting note of
        // an ottava line which will be turned off later.  ottavameasure == the
        // starting measure of the ottava mark.
        Note *ottavanotestart;
        Note *ottavanoteend;
        hum::HumNum ottavaendtimestamp;
        Measure *ottavameasure;

        // ottavadownnote == keep track of ottava down marks: stores the starting note of
        // an ottava down line which will be turned off later.  ottavadownmeasure == the
        // starting measure of the ottava down mark.
        Note *ottavadownnotestart;
        Note *ottavadownnoteend;
        hum::HumNum ottavadownendtimestamp;
        Measure *ottavadownmeasure;

        // ottava2note == keep track of ottava2 marks: stores the starting note of
        // an ottava2 line which will be turned off later.  ottava2measure == the
        // starting measure of the ottava2 mark.
        Note *ottava2notestart;
        Note *ottava2noteend;
        hum::HumNum ottava2endtimestamp;
        Measure *ottava2measure;

        // ottava2downnote == keep track of ottava2 down marks: stores the starting note of
        // an ottava2 down line which will be turned off later.  ottava2downmeasure == the
        // starting measure of the ottava2 down mark.
        Note *ottava2downnotestart;
        Note *ottava2downnoteend;
        hum::HumNum ottava2downendtimestamp;
        Measure *ottava2downmeasure;

        // meter_bottom == Used to keep track of bottom value of time signature.
        // This is needed to calculate tstamps.
        hum::HumNum meter_bottom;
        int meter_top;

        // ties == keep track of ties for each staff/layer/pitch
        // and allow for cross-layer ties (no cross staff ties, but that
        // could be easy to implement.
        // dimensions:
        // 1: staff
        // 2: all open ties for the staff
        std::list<humaux::HumdrumTie> ties;

        // m_dynampos == dynamic position relativ to the staff:
        // +1 = above, -1=below, 0=undefined (deal center between staves later)
        int m_dynampos = 0;
    };
} // namespace humaux

class HumdrumSignifiers {
public:
    bool empty = true;

    // boolean switches:
    char nostem = '\0'; // !!!RDF**kern: N = no stem
    char cuesize = '\0'; // !!!RDF**kern: @ = cue size
    char terminallong = '\0'; // !!!RDF**kern: l = terminal long
    vector<char> editacc; // !!!RDF**kern: i = editorial accidental
    vector<string> edittype; // !!!RDF**kern: i = editoral accidental, brack[ets]/paren[theses]

    // for **dynam:
    string cresctext; // !!!RDF**kern: > = "cresc."
    string crescfontstyle; // !!!RDF**kern: < = "cresc." fontstyle="normal|italic|bold|bold-italic"
    string decresctext; // !!!RDF**kern: > = "decresc."
    string decrescfontstyle; // !!!RDF**kern: < = "decresc." fontstyle="normal|italic|bold|bold-italic"

    char below = '\0'; // !!!RDF**kern: < = below (previous signifier is "below")
    char above = '\0'; // !!!RDF**kern: > = above (previous signifier is "above")

    std::string space_color; // !!!RDF**kern: show spaces color=hotpink
    std::string ispace_color; // !!!RDF**kern: show invisible rests color=chartreuse
    std::string irest_color; // !!!RDF**kern: show implicit spaces color=blueviolet
    std::string rspace_color; // !!!RDF**kern: show recip spaces color=royalblue

    // coloring of notes
    // !!!RDF**kern: i = marked note, color="#553325"
    // !!!RDF**kern: i = matched note, color=red
    // !!!RDF**kern: i = color="blue"
    // default is red if no color given:
    // !!!RDF**kern: i = matched note
    // !!!RDF**kern: i = marked note
    std::vector<char> mark;
    std::vector<std::string> mcolor;
    std::vector<std::string> markdir;

    std::vector<char> textmark;
    std::vector<std::string> textcolor;
};

#endif /* NO_HUMDRUM_SUPPORT */

//----------------------------------------------------------------------------
// HumdrumInput
//----------------------------------------------------------------------------

class HumdrumInput : public vrv::Input {
public:
    // constructors and destructors
    HumdrumInput(vrv::Doc *doc);
    virtual ~HumdrumInput();

    virtual bool Import(const std::string &humdrum);

    void parseEmbeddedOptions(vrv::Doc &doc);

#ifndef NO_HUMDRUM_SUPPORT

    std::string GetHumdrumString();
    std::string GetMeiString();

protected:
    void clear();
    bool convertHumdrum();
    void setupMeiDocument();
    int getMeasureEndLine(int startline);
    bool convertSystemMeasure(int &line);
    bool convertMeasureStaves(int startline, int endline);
    bool convertMeasureStaff(int track, int startline, int endline, int n, int layercount);
    void setupSystemMeasure(int startline, int endline);
    bool convertStaffLayer(int track, int startline, int endline, int layerindex);
    int getMeasureNumber(int startline, int endline);
    bool fillContentsOfLayer(int track, int startline, int endline, int layerindex);
    void calculateLayout();
    void setSystemMeasureStyle(int startline, int endline);
    std::vector<int> getStaffLayerCounts();
    void prepareStaffGroups();
    void setClef(StaffDef *part, const std::string &clef, hum::HTp cleftok = NULL);
    void setTransposition(StaffDef *part, const std::string &transpose);
    void setDynamicTransposition(int partindex, StaffDef *part, const std::string &itranspose);
    void setTimeSig(StaffDef *part, const std::string &timesig, const std::string &metersig = "",
        hum::HTp partstart = NULL, hum::HTp timetok = NULL);
    void fillPartInfo(hum::HTp partstart, int partnumber, int partcount);
    void storeStaffLayerTokensForMeasure(int startline, int endline);
    void calculateReverseKernIndex();
    void prepareTimeSigDur();
    void printMeasureTokens();
    int characterCount(const std::string &text, char symbol);
    int characterCount(hum::HTp token, char symbol);
    int characterCountInSubtoken(hum::HTp token, char symbol);
    int characterCountInSubtoken(const std::string &text, char symbol);
    bool emptyMeasures();
    bool hasFullMeasureRest(std::vector<hum::HTp> &layerdata, hum::HumNum timesigdur, hum::HumNum measuredur);
    void convertNote(vrv::Note *note, hum::HTp token, int staffadj, int staffindex, int subtoken = -1);
    void addCautionaryAccidental(Accid *accid, hum::HTp token, int acount);
    void convertRest(vrv::Rest *rest, hum::HTp token, int subtoken = -1);
    void convertMRest(MRest *rest, hum::HTp token, int subtoken, int staffindex);
    void processTieStart(Note *note, hum::HTp token, const std::string &tstring, int subindex);
    void processTieEnd(Note *note, hum::HTp token, const std::string &tstring, int subindex);
    void addFermata(hum::HTp token, vrv::Object *parent = NULL);
    void addBreath(hum::HTp token, vrv::Object *parent = NULL);
    void addTrill(hum::HTp token);
    void addTurn(vrv::Object *linked, hum::HTp token);
    void addMordent(vrv::Object *linked, hum::HTp token);
    void addOrnaments(vrv::Object *object, hum::HTp token);
    void addArpeggio(vrv::Object *object, hum::HTp token);
    void getTimingInformation(std::vector<hum::HumNum> &prespace, std::vector<hum::HTp> &layerdata,
        hum::HumNum layerstarttime, hum::HumNum layerendtime);
    void convertChord(Chord *chord, hum::HTp token, int staffindex);
    void prepareVerses();
    void convertVerses(Note *note, hum::HTp token, int subtoken);
    void checkForOmd(int startline, int endline);
    void handleOttavaMark(hum::HTp token, Note *note);
    void handlePedalMark(hum::HTp token);
    void handleLigature(hum::HTp token);
    void handleColoration(hum::HTp token);
    void prepareBeamAndTupletGroups(
        const std::vector<hum::HTp> &layerdata, std::vector<humaux::HumdrumBeamAndTuplet> &hg);
    void printGroupInfo(std::vector<humaux::HumdrumBeamAndTuplet> &tg, const std::vector<hum::HTp> &layerdata);
    void insertTuplet(std::vector<std::string> &elements, std::vector<void *> &pointers,
        const std::vector<humaux::HumdrumBeamAndTuplet> &tgs, std::vector<hum::HTp> layerdata, int layerindex,
        bool suppress);
    vrv::Beam *insertBeam(
        std::vector<std::string> &elements, std::vector<void *> &pointers, const humaux::HumdrumBeamAndTuplet &tg);
    vrv::Beam *insertGBeam(
        std::vector<std::string> &elements, std::vector<void *> &pointers, const humaux::HumdrumBeamAndTuplet &tg);
    void analyzeLayerBeams(
        std::vector<int> &beamnum, std::vector<int> &gbeamnum, const std::vector<hum::HTp> &layerdata);
    void storeBreaksec(std::vector<int> &beamstate, std::vector<int> &gbeamstate,
        const std::vector<hum::HTp> &layerdata, bool grace = false);
    void setBeamDirection(int direction, const std::vector<humaux::HumdrumBeamAndTuplet> &tgs,
        std::vector<hum::HTp> &layerdata, int layerindex, bool grace);
    void handleGroupStarts(const std::vector<humaux::HumdrumBeamAndTuplet> &tgs, std::vector<std::string> &elements,
        std::vector<void *> &pointers, std::vector<hum::HTp> &layerdata, int layerindex);
    bool checkForTremolo(
        std::vector<hum::HTp> &layerdata, const std::vector<humaux::HumdrumBeamAndTuplet> &tgs, int startindex);
    void handleGroupEnds(
        const humaux::HumdrumBeamAndTuplet &tg, std::vector<std::string> &elements, std::vector<void *> &pointers);
    void handleStaffStateVariables(hum::HTp token);
    void handleStaffDynamStateVariables(hum::HTp token);
    void removeTuplet(std::vector<std::string> &elements, std::vector<void *> &pointers);
    void removeGBeam(std::vector<std::string> &elements, std::vector<void *> &pointers);
    void removeBeam(std::vector<std::string> &elements, std::vector<void *> &pointers);
    vrv::Clef *insertClefElement(std::vector<std::string> &elements, std::vector<void *> &pointers, hum::HTp clef);
    void insertMeterSigElement(std::vector<std::string> &elements, std::vector<void *> &pointers,
        std::vector<hum::HTp> &layeritems, int index);
    void processSlurs(hum::HTp token);
    int getSlurEndIndex(hum::HTp token, std::string targetid, std::vector<bool> &indexused);
    void addHarmFloatsForMeasure(int startine, int endline);
    void addFingeringsForMeasure(int startline, int endline);
    void insertFingerNumberInMeasure(
        const std::string &text, int staffindex, hum::HTp token, int maxstaff, bool aboveQ);
    void addStringNumbersForMeasure(int startline, int endline);
    void addFiguredBassForMeasure(int startline, int endline);
    void processDynamics(hum::HTp token, int staffindex);
    void processDirections(hum::HTp token, int staffindex);
    void processLinkedDirection(int index, hum::HTp token, int staffindex);
    void processGlobalDirections(hum::HTp token, int staffindex);
    void processChordSignifiers(Chord *chord, hum::HTp token, int staffindex);
    hum::HumNum getMeasureTstamp(hum::HTp token, int staffindex, hum::HumNum frac = 0);
    hum::HumNum getMeasureTstampPlusDur(hum::HTp token, int staffindex, hum::HumNum fract = 0);
    hum::HumNum getMeasureEndTstamp(int staffindex);
    hum::HumNum getMeasureFactor(int staffindex);
    hum::HTp getPreviousDataToken(hum::HTp token);
    hum::HTp getHairpinEnd(hum::HTp token, const std::string &endchar);
    hum::HTp getDecrescendoEnd(hum::HTp token);
    hum::HTp getCrescendoEnd(hum::HTp token);
    int getMeasureDifference(hum::HTp starttok, hum::HTp endtok);
    void storeOriginalClefMensurationApp();
    void addSpace(std::vector<std::string> &elements, std::vector<void *> &pointers, hum::HumNum duration);
    void setLocationId(vrv::Object *object, hum::HTp token, int subtoken = -1);
    void setLocationId(vrv::Object *object, int lineindex, int fieldindex, int subtokenindex);
    std::string getLocationId(vrv::Object *object, hum::HTp token, int subtoken = -1);
    std::string getLocationId(Object *object, int lineindex, int fieldindex, int subtokenindex);
    std::string getLocationId(const std::string &prefix, hum::HTp token, int subtoken = -1);
    std::string getLocationId(const std::string &prefix, int lineindex, int fieldindex, int subtokenindex);
    void setLocationIdNSuffix(vrv::Object *object, hum::HTp token, int number);
    void setSlurLocationId(vrv::Object *object, hum::HTp slurstart, hum::HTp slurend, int eindex);
    void setTieLocationId(vrv::Object *object, hum::HTp tiestart, int sindex, hum::HTp tieend, int eindex);
    void setBeamLocationId(vrv::Object *object, const std::vector<humaux::HumdrumBeamAndTuplet> &tgs,
        std::vector<hum::HTp> &layerdata, int startindex);
    void setTupletLocationId(vrv::Object *object, const std::vector<humaux::HumdrumBeamAndTuplet> &tgs,
        std::vector<hum::HTp> &layerdata, int startindex);
    void addMidiTempo(vrv::ScoreDef &m_scoreDef, hum::HTp kernpart);
    void addInstrumentDefinition(vrv::StaffDef *staffdef, hum::HTp partstart);
    void addOrnamentMarkers(hum::HTp token);
    void setNextLeftBarStyle(data_BARRENDITION style);
    void parseSignifiers(hum::HumdrumFile &infile);
    std::string getAutoClef(hum::HTp partstart, int partnumber);
    void colorNote(vrv::Note *note, hum::HTp token, const std::string &subtoken, int line, int field);
    void colorRest(vrv::Rest *rest, const std::string &token, int line, int field);
    void colorVerse(Verse *verse, std::string &token);
    std::string getSpineColor(int line, int field);
    void checkForColorSpine(hum::HumdrumFile &infile);
    std::vector<int> analyzeMultiRest(hum::HumdrumFile &infile);
    bool analyzeBreaks(hum::HumdrumFile &infile);
    void addSystemKeyTimeChange(int startline, int endline);
    void prepareSections();
    int getDirection(const std::string &token, const std::string &target);
    void resolveTupletBeamTie(std::vector<humaux::HumdrumBeamAndTuplet> &tg);
    void resolveTupletBeamStartTie(std::vector<humaux::HumdrumBeamAndTuplet> &tg, int index);
    void resolveTupletBeamEndTie(std::vector<humaux::HumdrumBeamAndTuplet> &tg, int index);
    void mergeTupletsCuttingBeam(std::vector<humaux::HumdrumBeamAndTuplet> &tg);
    void embedQstampInClass(vrv::Note *note, hum::HTp token, const std::string &tstring);
    void embedQstampInClass(vrv::Rest *note, hum::HTp token, const std::string &tstring);
    void embedQstampInClass(vrv::MRest *mrest, hum::HTp token, const std::string &tstring);
    void embedQstampInClass(vrv::Space *irest, hum::HTp token, const std::string &tstring);
    void embedPitchInformationInClass(vrv::Note *note, const std::string &token);
    void embedTieInformation(Note *note, const std::string &token);
    void splitSyllableBySpaces(vector<string> &vtext, char spacer = ' ');
    void addDefaultTempo(ScoreDef &m_scoreDef);
    int getChordNoteCount(hum::HTp token);
    bool leftmostSystemArpeggio(hum::HTp token);
    bool leftmostStaffArpeggio(hum::HTp token);
    hum::HTp getRightmostSystemArpeggio(hum::HTp token);
    hum::HTp getRightmostStaffArpeggio(hum::HTp token);
    void addDirection(const std::string &text, const std::string &placement, bool bold, bool italic, hum::HTp token,
        int staffindex, int justification = 0, const std::string &color = "");
    void processTerminalLong(hum::HTp token);
    void removeCharacter(hum::HTp token, char removechar);
    std::string getSystemDecoration(const std::string &tag);
    void processStaffDecoration(const std::string &decoration);
    int getStaffNumberLabel(hum::HTp spinestart);
    bool isFirstTokenOnStaff(hum::HTp token);
    bool hasAboveParameter(hum::HTp token, const string &category);
    bool hasBelowParameter(hum::HTp token, const string &category);
    bool hasBelowParameter(hum::HTp token, const string &category, int &output);
    void prepareHeaderFooter();
    bool prepareHeader(std::vector<std::pair<string, string> > &biblist, std::map<std::string, std::string> &refmap);
    bool prepareFooter(std::vector<std::pair<string, string> > &biblist, std::map<std::string, std::string> &refmap);
    std::string processReferenceTemplate(const std::string &input, std::vector<std::pair<string, string> > &biblist,
        std::map<std::string, std::string> &refmap);
    std::string processTemplateOperator(const std::string &value, const std::string &op);
    std::string automaticHeaderLeft(
        std::vector<std::pair<string, string> > &biblist, std::map<std::string, std::string> &refmap, int linecount);
    std::string automaticHeaderCenter(
        std::vector<std::pair<string, string> > &biblist, std::map<std::string, std::string> &refmap);
    std::string automaticHeaderRight(
        std::vector<std::pair<string, string> > &biblist, std::map<std::string, std::string> &refmap, int &linecount);
    void convertMensuralToken(
        std::vector<string> &elements, std::vector<void *> &pointers, hum::HTp token, int staffindex);
    void initializeSpineColor(hum::HumdrumFile &infile);
    void setStemLength(Note *note, hum::HTp token);
    void storeExpansionLists(Section *section, hum::HTp starting);
    void storeExpansionList(Section *section, hum::HTp etok);
    std::string replaceMusicShapes(const std::string input);
    int getStaffAdjustment(hum::HTp token);
    void calculateNoteIdForSlur(std::string &idstring, std::vector<pair<int, int> > &sortednotes, int index);
    void promoteInstrumentNamesToGroup();
    void promoteInstrumentsForStaffGroup(StaffGrp *group);
    void promoteInstrumentAbbreviationsToGroup();
    void promoteInstrumentAbbreviationsForStaffGroup(StaffGrp *group);
    std::string getInstrumentName(StaffDef *sd);
    std::string getInstrumentAbbreviation(StaffDef *sd);
    void removeInstrumentName(StaffDef *sd);
    void removeInstrumentAbbreviation(StaffDef *sd);
    std::string getEndIdForOttava(hum::HTp token);
    void prepareInitialOttavas(hum::HTp measure);
    void linkFingeringToNote(Dir *dir, hum::HTp token, int xstaffindex);
    bool checkForTupletForcedBreak(const std::vector<hum::HTp> &duritems, int index);
    void extractSlurNoteAttachmentInformation(std::vector<std::pair<int, bool> > &data, hum::HTp token, char slurtype);
    bool getNoteState(hum::HTp token, int slurnumber);
    void assignVerticalGroup(Pedal *ped, hum::HTp token);
    void storeAcclev(const std::string value, int staffindex);
    void storeStemInterpretation(const std::string &value, int staffindex, int layernumber);
    bool getBooleanParameter(hum::HTp token, const string &category, const string &key);
    std::string getStringParameter(hum::HTp token, const string &category, const string &key);
    bool shouldHideBeamBracket(
        const std::vector<humaux::HumdrumBeamAndTuplet> &tgs, std::vector<hum::HTp> &layerdata, int layerindex);
    void checkBeamWith(Beam *beam, const std::vector<humaux::HumdrumBeamAndTuplet> &tgs,
        std::vector<hum::HTp> &layerdata, int startindex);
    std::string getTrackText(hum::HTp token);
    void checkForLayoutBreak(int line);
    std::string removeCommas(const std::string &input);
    void extractNullInformation(vector<bool> &nulls, hum::HumdrumFile &infile);
    void initializeIgnoreVector(hum::HumdrumFile &infile);
    bool hasIndent(hum::HTp tok);
    void prepareNonStandardKeySignature(KeySig *vrvkeysig, const std::string &ks, hum::HTp keytok);
    void fixLargeTuplets(std::vector<humaux::HumdrumBeamAndTuplet> &tg);

    // header related functions: ///////////////////////////////////////////
    void createHeader();
    void insertTitle(pugi::xml_node &titleStmt, const std::vector<hum::HumdrumLine *> &references);
    void insertExtMeta(std::vector<hum::HumdrumLine *> &references);
    void addPerson(std::vector<std::vector<std::string> > &respPeople, std::vector<hum::HumdrumLine *> &references,
        const std::string &key, const std::string &role);
    void getRespPeople(std::vector<std::vector<std::string> > &respPeople, std::vector<hum::HumdrumLine *> &references);
    void insertRespStmt(pugi::xml_node &titleStmt, std::vector<std::vector<std::string> > &respPeople);
    void insertPeople(pugi::xml_node &work, std::vector<std::vector<string> > &respPeople);

    /// Templates ///////////////////////////////////////////////////////////
    template <class ELEMENT> void verticalRest(ELEMENT rest, const std::string &token);
    template <class ELEMENT>
    void setKeySig(int partindex, ELEMENT element, const std::string &keysig, hum::HTp keytok, bool secondary);
    template <class PARENT, class CHILD> void appendElement(PARENT parent, CHILD child);
    template <class ELEMENT> void addArticulations(ELEMENT element, hum::HTp token);
    template <class ELEMENT> hum::HumNum convertRhythm(ELEMENT element, hum::HTp token, int subtoken = -1);
    template <class ELEMENT> void setRhythmFromDuration(ELEMENT element, hum::HumNum dur);
    template <class ELEMENT> hum::HumNum convertMensuralRhythm(ELEMENT element, hum::HTp token, int subtoken = -1);
    template <class ELEMENT> hum::HumNum setDuration(ELEMENT element, hum::HumNum duration);
    template <class ELEMENT> void setStaff(ELEMENT element, int staffnum);
    template <class ELEMENT> void setN(ELEMENT element, int nvalue, hum::HTp tok = NULL);
    template <class ELEMENT> void assignAutomaticStem(ELEMENT element, hum::HTp tok, int staffindex);
    template <class ELEMENT> KeySig *getKeySig(ELEMENT element);
    template <class ELEMENT> MeterSig *getMeterSig(ELEMENT element);
    template <class ELEMENT> Clef *getClef(ELEMENT element);
    template <class ELEMENT> Mensur *getMensur(ELEMENT element);

    template <class CHILD>
    void appendElement(const std::vector<std::string> &name, const std::vector<void *> &pointers, CHILD child);
    void popElementStack(std::vector<string> &elements, std::vector<void *> &pointers);

    template <class ELEMENT>
    void addTextElement(ELEMENT *element, const std::string &content, const std::string &fontstyle = "");
    template <class ELEMENT> void checkForAutoStem(ELEMENT element, hum::HTp token);
    template <class ELEMENT> void appendTypeTag(ELEMENT *element, const std::string &tag);
    template <class ELEMENT> void setPlace(ELEMENT *element, const std::string &place);
    template <class ELEMENT>
    void setMeterSymbol(
        ELEMENT *element, const std::string &metersig, hum::HTp partstart = NULL, hum::HTp metertok = NULL);
    template <class ELEMENT>
    void setMensurationSymbol(ELEMENT *element, const std::string &metersig, hum::HTp mensurtok = NULL);
    template <class ELEMENT>
    void setInstrumentName(ELEMENT *staffdef, const std::string &name, hum::HTp labeltok = NULL);
    template <class ELEMENT>
    void setInstrumentAbbreviation(ELEMENT *staffdef, const std::string &name, hum::HTp abbrtok);
    template <class ELEMENT>
    void addLineFormStyle(ELEMENT element, hum::HTp token, const string &layout, int index = 0);

    /// Static functions ////////////////////////////////////////////////////
    static std::string unescapeHtmlEntities(const std::string &input);
    static void WriteUTF8(std::ostream &Out, unsigned int Ch);
    static void UnquoteHTML(std::istream &In, std::ostream &Out);
    static hum::HumNum removeFactorsOfTwo(hum::HumNum value, int &tcount, int &bcount);
    static int getDotPowerOfTwo(hum::HumNum value);
    static int nextLowerPowerOfTwo(int x);
    static hum::HumNum nextHigherPowerOfTwo(hum::HumNum x);
    static std::string getDateString();
    static std::string getReferenceValue(const std::string &key, std::vector<hum::HumdrumLine *> &references);
    static bool replace(std::string &str, const std::string &oldStr, const std::string &newStr);
    static bool replace(std::wstring &str, const std::wstring &oldStr, const std::wstring &newStr);
    std::wstring cleanHarmString(const std::string &content);
    std::wstring cleanHarmString2(const std::string &content);
    std::wstring cleanHarmString3(const std::string &content);
    std::wstring cleanStringString(const std::string &content);
    std::vector<std::wstring> cleanFBString(std::vector<std::string> &pieces, hum::HTp token);
    std::wstring cleanFBString2(std::vector<std::string> &pieces, hum::HTp token);
    std::vector<std::string> splitFBString(const std::string &content, const std::string &separator = " ");
    std::wstring getVisualFBAccidental(int accidental);
    std::wstring convertFBNumber(const string &input, hum::HTp token);
    void checkForLineContinuations(hum::HTp token);
    std::wstring convertNumberToWstring(int number);

private:
    // m_filename == Filename to read/was read.
    std::string m_filename;

    // m_debug == mostly for printing MEI data to standard input.
    int m_debug = 0;

    // m_comment == Display **kern data embedded in comments for each
    // staff layer.
    int m_comment = 1;

    // m_doc is inherited root document object.

    // m_score stores the music
    Score *m_score = NULL;

    // m_sections stores segments of the music
    std::vector<Section *> m_sections;

    // m_lastsection == The section label of the previous measure
    string m_lastsection;

    //////////////////////////////
    //
    // State variables for doing the conversion without having to pass
    // these variables as parameters:
    //

    // m_staffgroup == information about parts
    // no longer used
    // vrv::StaffGrp *m_staffgroup = NULL;

    // m_staffdef == information about a staff.
    std::vector<vrv::StaffDef *> m_staffdef;

    std::vector<vrv::Measure *> m_measures;

    // m_measure == current measure, or NULL.
    vrv::Measure *m_measure = NULL;

    // m_staff == current staff, or NULL.
    vrv::Staff *m_staff = NULL;

    // m_currentstaff == The current staff being parsed.
    int m_currentstaff = -1;

    // m_layer == current layer, or NULL.
    vrv::Layer *m_layer = NULL;

    int m_currentlayer;

    // m_layertokens == Humdrum **kern tokens for each staff/layer to be
    // converted.
    std::vector<std::vector<std::vector<hum::HTp> > > m_layertokens;

    // m_staffstarts == list of tracks in Humdrum file being parsed which
    // contain **kern, **mens data or whatever other data types
    // will be converted into a staff in the conversion to MEI.
    std::vector<hum::HTp> m_staffstarts;

    // m_rkern == reverse mapping of Humdrum track to staff number..
    std::vector<int> m_rkern;

    // m_infiles == Humdrum file used for conversion.
    hum::HumdrumFileSet m_infiles;

    // m_timesigdurs == Prevailing time signature duration of measure
    std::vector<hum::HumNum> m_timesigdurs;

    // m_tupletscaling == tuplet-scaling factor for the current note.
    hum::HumNum m_tupletscaling = 1;

    // m_omd == temporary variable for printing tempo designation.
    hum::HumNum m_omd = -1;

    // m_oclef == temporary variable for printing "original-clef" <app>
    std::vector<std::pair<int, hum::HTp> > m_oclef;

    // m_omet == temporary variable for printing "original-mensuration" <app>
    std::vector<std::pair<int, hum::HTp> > m_omet;

    // m_staffstates == state variables for each staff.
    std::vector<humaux::StaffStateVariables> m_staffstates;

    // m_measureIndex == state variable for keeping track of the
    // current measure number being converted.
    int m_measureIndex;

    // m_harm == state variable for keeping track of whether or not
    // the file to convert contains **mxhm spines that should be
    // converted into <harm> element in the MEI conversion.
    bool m_harm = false;

    // m_fing == state variable for keeping track of whether or not
    // the file to convert contains **fing spines that should be
    // converted into <fing> elements in the MEI conversion.
    bool m_fing = false;

    // m_string == state variable for keeping track of whether or not
    // the file to convert contains **string spines that should be
    // converted into <string> elements in the MEI conversion.
    bool m_string = false;

    // m_mens == state variable for keeping track of whether or not
    // the file to convert contains **mens spines that should be
    // converted into mensuration notation staves.
    bool m_mens = false;

    // m_fb == state variable for keeping track of whether or not
    // the file to convert contains **Bnum spines that should be
    // converted into <harm> element in the MEI conversion.
    bool m_fb = false;

    // m_fbstaff == state variable for keeping track of whether or not
    // a particular staff (index) has figured bass.  This is used by
    // the tuplet@place parameter to avoid collision between tuplet numbers
    // and figured bass.
    std::vector<bool> m_fbstaff;

    // m_fbstates == position of the figured bass in relation to
    // each staff index.  0 = no figured bass, -1 = figured bass below staff,
    // +1 = figured bass above staff.
    std::vector<int> m_fbstates;

    // m_setrightstem == used for setting right-side stem of half notes
    bool m_setrightstem = false;

    // m_leftbarstyle is a barline left-hand style to store in the next measure.
    // When processing a measure, this variable should be checked and used
    // in @left="style" for the measure.
    data_BARRENDITION m_leftbarstyle;

    // m_signifiers == a list of user defined signfiers in **kern spine data
    // which indicate non-standard **kern parametesr that can be converted
    // into notation.
    HumdrumSignifiers m_signifiers;

    // m_multirest == boolean states to keep track of muti-rest cases.
    std::vector<int> m_multirest;

    // m_sections == keep track of thru sections and 1st/second endings.
    std::vector<hum::HTp> m_sectionlabels;

    // m_endingnum == keep track of current ending.
    int m_endingnum = 0;

    // m_currentending == keep track of current ending.
    vrv::Ending *m_currentending = NULL;

    // m_currentsection == keep track of current section.
    vrv::Section *m_currentsection = NULL;

    // m_has_color_spine == true if a color spine is present.
    bool m_has_color_spine = false;

    // m_spine_color == list of colors to apply to spine data
    std::vector<string> m_spine_color;

    // m_traspose == transposition to go from sounding to written pitch.
    vector<int> m_transpose;

    //    *kcancel     = display cancellation key signatures
    //    *Xkcancel    = do not display cancellation key signatures (default)
    bool m_show_cautionary_keysig = false;

    // m_hasTremolo == true if there is a *tremolo found in input data.
    bool m_hasTremolo = false;

    // m_placement == placement above/below state for a particular spine of information
    // currently used for **fb, but expand to other spine types as needed (such as **harm).
    // +1 = *above marker encountered in spine.
    // -1 = *below marker encountered in spine.
    // 0 = *auto neither above or below explicitly given (leave up to renderer).
    // Up to 1000 spines can be processed (see constructor).
    // Indexed by track number of the spine it revers to.
    std::vector<int> m_placement;

    // m_reverse == placement reversed or not reversed.  currently used for **fb,
    // might be useful for other types of spines in the fugure.
    // +1 = *reverse marker encountered in spine.
    // 0 = *Xreverse marker encountered in spine. (or default)
    // Up to 1000 spines can be processed (see constructor).
    std::vector<int> m_reverse;

    // m_absolute == use relative or absolute accidentals in **fb.
    // +1 = *absolute marker encountered in spine.
    // 0 = *Xabsolute marker encountered in spine. (default)
    // Up to 1000 spines can be processed (see constructor).
    std::vector<int> m_absolute;

    // m_slash == display accidentals as slashes or as accidentals in **fb.
    // +1 = *slash marker encountered in spine. (default)
    // 0 = *Xslash marker encountered in spine.
    // Up to 1000 spines can be processed (see constructor).
    std::vector<int> m_slash;

    // m_breaks == true if the music contains encoded page/system breaks
    bool m_breaks = false;

    // m_nulls == true if the line only contains null data.
    std::vector<bool> m_nulls;

    // m_duradj == duration adjustments due to the presence of a full
    // null data line.  Add this value when calculating the prespace
    // variable in fillContentsOfLayer().
    std::vector<hum::HumNum> m_duradj;

    // m_ignore == limit conversion range of data (for speeding up editing of
    // larger files).
    std::vector<bool> m_ignore;

#endif /* NO_HUMDRUM_SUPPORT */
};

} // namespace vrv

#endif // __VRV_IOHUMDRUM_H__
