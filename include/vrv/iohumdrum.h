/////////////////////////////////////////////////////////////////////////////
// Name:        iohumdrum.h
// Author:      Craig Stuart Sapp
// Created:     06/06/2015
// Copyright (c) Authors and others. All rights reserved.
// vim:         ts=4
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOHUMDRUM_H__
#define __VRV_IOHUMDRUM_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "attdef.h"
#include "clef.h"
#include "ending.h"
#include "io.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#ifndef NO_HUMDRUM_SUPPORT
#include "humlib.h"
#include "pugixml.hpp"
#include "tool-musicxml2hum.h"
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
class Rest;
class Score;
class Section;
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
        HumdrumTie(void);
        HumdrumTie(const HumdrumTie &anothertie);
        ~HumdrumTie();
        HumdrumTie &operator=(const HumdrumTie &anothertie);
        void insertTieIntoDom(void);
        void setStart(const string &id, Measure *starting, int layer, const string &token, int pitch,
            hum::HumNum starttime, hum::HumNum endtime);
        void setEnd(const string &id, Measure *ending, const string &token);
        void setEndAndInsert(const string &id, Measure *ending, const string &token);
        hum::HumNum getEndTime(void);
        hum::HumNum getStartTime(void);
        hum::HumNum getDuration(void);
        string getStartToken(void);
        string getEndToken(void);
        int getPitch(void);
        int getLayer(void);
        bool isInserted(void);
        void clear();

    private:
        string m_starttoken;
        string m_endtoken;
        hum::HumNum m_starttime;
        hum::HumNum m_endtime;
        int m_pitch;
        int m_layer;
        bool m_inserted;
        string m_startid;
        string m_endid;
        Measure *m_startmeasure;
        Measure *m_endmeasure;
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
        HumdrumBeamAndTuplet(void) { clear(); }
        ~HumdrumBeamAndTuplet(void) { clear(); }
        void clear(void)
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
        StaffStateVariables(void);
        ~StaffStateVariables(void);
        void clear(void);

        // verse == keeps track of whether or not staff contains associated
        // **text spines which will be converted into lyrics.
        bool verse;

        // suppress_beam_tuplet == keeps track of whether or not beams should
        // display beam tuplet numbers.
        bool suppress_beam_tuplet;

        // suppress_bracket_tuplet == keeps track of whether or not tuplet
        // brackets should be displayed.
        bool suppress_bracket_tuplet;

        // ottavanote == keep track of ottava marks: stores the starting note of
        // an ottava line which will be turned off later.  ottavameasure == the
        // starting measure of the ottava mark.
        Note *ottavanotestart;
        Note *ottavanoteend;
        Measure *ottavameasure;

        // meter_bottom == Used to keep track of bottom value of time signature.
        // This is needed to calculate tstamps.
        hum::HumNum meter_bottom;

        // ties == keep track of ties for each staff/layer/pitch
        // and allow for cross-layer ties (no cross staff ties, but that
        // could be easy to implement.
        // dimensions:
        // 1: staff
        // 2: all open ties for the staff
        std::list<humaux::HumdrumTie> ties;
    };
}

class HumdrumSignifiers {
public:
    bool empty = true;

    // boolean switches:
    char nostem = '\0'; // !!!RDF**kern: i = no stem
    char editacc = '\0'; // !!!RDF**kern: i = editorial accidental
	char slurbelow = '\0'; // !!!RDF**kern: i = slur below
	char slurabove = '\0'; // !!!RDF**kern: i = slur above

    // coloring of notes (add rests later):
    // !!!RDF**kern: i = marked note, color="#553325"
    // !!!RDF**kern: i = matched note, color=red
    // !!!RDF**kern: i = color="blue"
    // default is red if no color given:
    // !!!RDF**kern: i = matched note
    // !!!RDF**kern: i = marked note
    vector<char> mark;
    vector<string> mcolor;
};

#endif /* NO_HUMDRUM_SUPPORT */

//----------------------------------------------------------------------------
// HumdrumInput
//----------------------------------------------------------------------------

class HumdrumInput : public vrv::FileInputStream {
public:
    // constructors and destructors
    HumdrumInput(vrv::Doc *doc, std::string filename);
    virtual ~HumdrumInput();

    virtual bool ImportFile();
    virtual bool ImportString(std::string const &humdrum);

#ifndef NO_HUMDRUM_SUPPORT

    string GetHumdrumString(void);
    string GetMeiString(void);

protected:
    void clear(void);
    bool convertHumdrum(void);
    void setupMeiDocument(void);
    int getMeasureEndLine(int startline);
    bool convertSystemMeasure(int &line);
    bool convertMeasureStaves(int startline, int endline);
    bool convertMeasureStaff(int track, int startline, int endline, int n, int layercount);
    void setupSystemMeasure(int startline, int endline);
    bool convertStaffLayer(int track, int startline, int endline, int layerindex);
    int getMeasureNumber(int startline, int endline);
    bool fillContentsOfLayer(int track, int startline, int endline, int layerindex);
    void calculateLayout(void);
    void setSystemMeasureStyle(int startline, int endline);
    vector<int> getStaffLayerCounts(void);
    void prepareStaffGroup(void);
    void setClef(StaffDef *part, const string &clef);
    void setTimeSig(StaffDef *part, const string &timesig);
    void setMeterSymbol(StaffDef *part, const string &metersig);
    void fillPartInfo(hum::HTp partstart, int partnumber);
    void storeStaffLayerTokensForMeasure(int startline, int endline);
    void calculateReverseKernIndex(void);
    void prepareTimeSigDur(void);
    void printMeasureTokens(void);
    int characterCount(const string &text, char symbol);
    int characterCount(hum::HTp token, char symbol);
    int characterCountInSubtoken(hum::HTp token, char symbol);
    int characterCountInSubtoken(const string &text, char symbol);
    bool emptyMeasures(void);
    bool hasFullMeasureRest(vector<hum::HTp> &layerdata, hum::HumNum timesigdur, hum::HumNum measuredur);
    void convertNote(vrv::Note *note, hum::HTp token, int staffindex, int subtoken = -1);
    void addCautionaryAccidental(Note *note, hum::HTp token, int acount);
    void convertRest(vrv::Rest *rest, hum::HTp token, int subtoken = -1);
    void processTieStart(Note *note, hum::HTp token, const string &tstring);
    void processTieEnd(Note *note, hum::HTp token, const string &tstring);
    void addFermata(hum::HTp token);
    void getTimingInformation(vector<hum::HumNum> &prespace, vector<hum::HTp> &layerdata, hum::HumNum layerstarttime,
        hum::HumNum layerendtime);
    void convertChord(Chord *chord, hum::HTp token, int staffindex);
    void prepareVerses(void);
    void convertVerses(Note *note, hum::HTp token, int subtoken);
    void checkForOmd(int startline, int endline);
    void handleOttavaMark(hum::HTp token, Note *note);
    void handlePedalMark(hum::HTp token, Note *note);
    void prepareBeamAndTupletGroups(const vector<hum::HTp> &layerdata, vector<humaux::HumdrumBeamAndTuplet> &hg);
    void printGroupInfo(vector<humaux::HumdrumBeamAndTuplet> &tg, const vector<hum::HTp> &layerdata);
    void insertTuplet(vector<string> &elements, vector<void *> &pointers, const humaux::HumdrumBeamAndTuplet &tg,
        hum::HTp token, bool suppress);
    void insertBeam(vector<string> &elements, vector<void *> &pointers, const humaux::HumdrumBeamAndTuplet &tg);
    void insertGBeam(vector<string> &elements, vector<void *> &pointers, const humaux::HumdrumBeamAndTuplet &tg);
    void analyzeLayerBeams(vector<int> &beamnum, vector<int> &gbeamnum, const vector<hum::HTp> &layerdata);
    void handleGroupStarts(
        const humaux::HumdrumBeamAndTuplet &tg, vector<string> &elements, vector<void *> &pointers, hum::HTp token);
    void handleGroupEnds(const humaux::HumdrumBeamAndTuplet &tg, vector<string> &elements, vector<void *> &pointers);
    void handleStaffStateVariables(hum::HTp token);
    void removeTuplet(vector<string> &elements, vector<void *> &pointers);
    void removeGBeam(vector<string> &elements, vector<void *> &pointers);
    void removeBeam(vector<string> &elements, vector<void *> &pointers);
    vrv::Clef *insertClefElement(vector<string> &elements, vector<void *> &pointers, hum::HTp clef);
    void insertMeterSigElement(
        vector<string> &elements, vector<void *> &pointers, vector<hum::HTp> &layeritems, int index);
    void processSlur(hum::HTp token);
    void addHarmFloatsForMeasure(int startine, int endline);
    void processDynamics(hum::HTp token, int staffindex);
    void processDirection(hum::HTp token, int staffindex);
    hum::HumNum getMeasureTstamp(hum::HTp token, int staffindex);
    hum::HTp getPreviousDataToken(hum::HTp token);
    hum::HTp getHairpinEnd(hum::HTp token, const string &endchar);
    hum::HTp getDecrescendoEnd(hum::HTp token);
    hum::HTp getCrescendoEnd(hum::HTp token);
    int getMeasureDifference(hum::HTp starttok, hum::HTp endtok);
    void storeOriginalClefApp(void);
    void addSpace(vector<string> &elements, vector<void *> &pointers, hum::HumNum duration);
    void setLocationId(vrv::Object *object, hum::HTp token, int subtoken);
    void setLocationId(vrv::Object *object, int lineindex, int fieldindex, int subtokenindex);
    void addMidiTempo(vrv::ScoreDef &m_scoreDef, hum::HTp kernpart);
    void addInstrumentDefinition(vrv::StaffDef *staffdef, hum::HTp partstart);
    void addOrnamentMarkers(hum::HTp token);
    void setNextLeftBarStyle(data_BARRENDITION style);
    void parseSignifiers(hum::HumdrumFile &infile);
    string getAutoClef(hum::HTp partstart, int partnumber);
    void colorNote(vrv::Note *note, hum::HTp token);
    vector<int> analyzeMultiRest(hum::HumdrumFile &infile);
    void addSystemKeyTimeChange(int startline, int endline);
    void prepareEndings(void);

    // header related functions: ///////////////////////////////////////////
    void createHeader(void);
    void insertTitle(pugi::xml_node &titleStmt, const vector<hum::HumdrumLine *> &references);
    void insertExtMeta(vector<hum::HumdrumLine *> &references);
    void addPerson(vector<vector<string> > &respPeople, vector<hum::HumdrumLine *> &references, const string &key,
        const string &role);
    void getRespPeople(vector<vector<string> > &respPeople, vector<hum::HumdrumLine *> &references);
    void insertRespStmt(pugi::xml_node &titleStmt, vector<vector<string> > &respPeople);

    /// Templates ///////////////////////////////////////////////////////////
    template <class ELEMENT> void setKeySig(ELEMENT element, const string &keysig);
    template <class PARENT, class CHILD> void appendElement(PARENT parent, CHILD child);

    template <class ELEMENT> void addArticulations(ELEMENT element, hum::HTp token);

    template <class ELEMENT> hum::HumNum convertRhythm(ELEMENT element, hum::HTp token, int subtoken = -1);

    template <class ELEMENT> hum::HumNum setDuration(ELEMENT element, hum::HumNum duration);

    template <class ELEMENT> void setStaff(ELEMENT element, int staffnum);

    template <class CHILD> void appendElement(const vector<string> &name, const vector<void *> &pointers, CHILD child);

    template <class ELEMENT> void addTextElement(ELEMENT *element, const string &content);

    /// Static functions ////////////////////////////////////////////////////
    static string unescapeHtmlEntities(const string &input);
    static void WriteUTF8(std::ostream &Out, unsigned int Ch);
    static void UnquoteHTML(std::istream &In, std::ostream &Out);
    static hum::HumNum removeFactorsOfTwo(hum::HumNum value, int &tcount, int &bcount);
    static int getDotPowerOfTwo(hum::HumNum value);
    static int nextLowerPowerOfTwo(int x);
    static string getDateString(void);
    static string getReferenceValue(const string &key, vector<hum::HumdrumLine *> &references);
    static bool replace(string &str, const string &oldStr, const string &newStr);
    string cleanHarmString(const string &content);

private:
    std::string m_filename; // Filename to read/was read.

    // m_debug == mostly for printing MEI data to standard input.
    int m_debug = 0;

    // m_comment == Display **kern data embedded in comments for each
    // staff layer.
    int m_comment = 1;

    // m_doc is inherited root document object.

    // m_score stores the music
    Score *m_score = NULL;

    // m_sections stores segments of the music
    vector<Section *> m_sections;

    //////////////////////////////
    //
    // State variables for doing the conversion without having to pass
    // these variables as parameters:
    //

    // m_staffgroup == information about parts
    vrv::StaffGrp *m_staffgroup = NULL;

    // m_staffdef == information about a staff.
    vector<vrv::StaffDef *> m_staffdef;

    vector<vrv::Measure *> m_measures;

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
    vector<vector<vector<hum::HTp> > > m_layertokens;

    // m_kernspines == list of tracks in Humdrum file being parsed which
    // contain **kern data.
    vector<hum::HTp> m_kernstarts;

    // m_rkern == reverse mapping of Humdrum track to staff number..
    vector<int> m_rkern;

    // m_infile == Humdrum file used for conversion.
    hum::HumdrumFile m_infile;

    // m_timesigdurs == Prevailing time signature duration of measure
    vector<hum::HumNum> m_timesigdurs;

    // m_tupletscaling == tuplet-scaling factor for the current note.
    hum::HumNum m_tupletscaling = 1;

    // m_omd == temporary variable for printing tempo designation.
    bool m_omd = false;

    // m_oclef == temporary variable for printing "original-clef" <app>
    vector<std::pair<int, hum::HTp> > m_oclef;

    // m_staffstates == state variables for each staff.
    vector<humaux::StaffStateVariables> m_staffstates;

    // m_measureIndex == state variable for keeping track of the
    // current measure number being converted.
    int m_measureIndex;

    // m_harm == state variable for keeping track of whether or not
    // the file to convert contains **mxhm spines that should be
    // converted into <harm> element in the MEI conversion.
    bool m_harm;

    // m_leftbarstyle is a barline left-hand style to store in the next measure.
    // When processing a measure, this variable should be checked and used
    // in @left="style" for the measure.
    data_BARRENDITION m_leftbarstyle;

    // m_signifiers == a list of user defined signfiers in **kern spine data
    // which indicate non-standard **kern parametesr that can be converted
    // into notation.
    HumdrumSignifiers m_signifiers;

    // m_multirest == boolean states to keep track of muti-rest cases.
    vector<int> m_multirest;

    // m_ending == keep track of 1st/second endings.
    vector<int> m_ending;

    // m_currentending == keep track of current ending.
    int m_endingnum = 0;

    // m_currentending == keep track of current ending.
    vrv::Ending *m_currentending = NULL;

#endif /* NO_HUMDRUM_SUPPORT */
};

} // namespace vrv

#endif // __VRV_IOHUMDRUM_H__
