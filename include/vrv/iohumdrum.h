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
#include "io.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

#include "humlib.h"

//----------------------------------------------------------------------------

namespace vrv {

class Beam;
class Chord;
class Layer;
class Measure;
class Note;
class Page;
class Rest;
class Staff;
class StaffDef;
class StaffGrp;
class System;
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
}

//----------------------------------------------------------------------------
// HumdrumInput
//----------------------------------------------------------------------------

class HumdrumInput : public vrv::FileInputStream {
public:
    // constructors and destructors
    HumdrumInput(vrv::Doc *doc, std::string filename);
    virtual ~HumdrumInput();

    virtual bool ImportFile();
    virtual bool ImportString(std::string humdrum);

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
    void setKeySig(StaffDef *part, const string &keysig);
    void setTimeSig(StaffDef *part, const string &timesig);
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
    void convertNote(vrv::Note *note, hum::HTp token, int subtoken = -1);
    void convertRest(vrv::Rest *rest, hum::HTp token, int subtoken = -1);
    void processTieStart(Note *note, hum::HTp token, const string &tstring);
    void processTieEnd(Note *note, hum::HTp token, const string &tstring);
    void printNoteArticulations(Note *note, hum::HTp token, const string &tstring);
    void getTimingInformation(vector<hum::HumNum> &prespace, vector<hum::HTp> &layerdata, hum::HumNum layerstarttime,
        hum::HumNum layerendtime);
    void convertChord(Chord *chord, hum::HTp token);
    void prepareVerses(void);
    void convertVerses(Note *note, hum::HTp token, int subtoken);
    void checkForOmd(int startline, int endline);

    /// Templates ///////////////////////////////////////////////////////////
    template <class PARENT, class CHILD> void appendElement(PARENT parent, CHILD child);

    template <class ELEMENT> void convertRhythm(ELEMENT element, hum::HTp token, int subtoken = -1);

    template <class ELEMENT> void setDuration(ELEMENT rest, hum::HumNum duration);

    /// Static functions ////////////////////////////////////////////////////
    static string unescapeHtmlEntities(const string &input);
    static void WriteUTF8(std::ostream &Out, unsigned int Ch);
    static void UnquoteHTML(std::istream &In, std::ostream &Out);

private:
    std::string m_filename; // Filename to read/was read.

    // m_debug == temporary variable to print MEI convertsion to standard
    // output.
    int m_debug; // printing MEI data to standard input

    // m_doc is inherited root document object.

    //////////////////////////////
    //
    // State variables for doing the conversion without having to pass
    // these variables as parameters:
    //

    vrv::StaffGrp *m_staffgroup; // information about parts
    vector<vrv::StaffDef *> m_staffdef; // information about a staff

    vrv::Page *m_page; // current page, or NULL
    vrv::System *m_system; // current system, or NULL
    vrv::Measure *m_measure; // current measure, or NULL
    vrv::Staff *m_staff; // current staff, or NULL
    vrv::Layer *m_layer; // current layer, or NULL
    int m_currentlayer;

    // m_layertokens == Humdrum **kern tokens for each staff/layer to be
    // converted.
    vector<vector<vector<hum::HTp> > > m_layertokens;

    // m_kernspines == list of tracks in Humdrum file being parsed which
    // contain **kern data.
    vector<hum::HTp> m_kernstarts;

    // m_rkern == reverse listing of staff to Humdrum file track.
    vector<int> m_rkern;

    // m_infile == Humdrum file used for conversion.
    hum::HumdrumFile m_infile;

    // m_timesigdurs == Prevailing time signature duration of measure
    vector<hum::HumNum> m_timesigdurs;

    // m_tiestates == keep track of ties for each staff/layer/pitch
    // and allow for cross-layer ties (no cross staff ties, but that
    // could be easy to implement.
    // dimensions:
    // 1: staff
    // 2: all open ties for the staff
    vector<std::list<humaux::HumdrumTie> > m_ties;

    // m_verses == keeps track of whether or not each staff contains associated
    // **text spines which will be converted into lyrics.
    vector<bool> m_verses;
};

} // namespace vrv

#endif // __VRV_IOHUMDRUM_H__
