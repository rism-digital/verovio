/////////////////////////////////////////////////////////////////////////////
// Name:        iohumdrum.h
// Author:      Craig Stuart Sapp
// Created:     06/06/2015
// Copyright (c) Authors and others. All rights reserved.
// vim:         ts=3
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_IOHUMDRUM_H__
#define __VRV_IOHUMDRUM_H__

#include <string>
#include <vector>

//----------------------------------------------------------------------------

#include "attdef.h"
#include "humlib.h"
#include "io.h"
#include "vrvdef.h"

//----------------------------------------------------------------------------

namespace vrv {

class Layer;
class Measure;
class Page;
class Staff;
class StaffDef;
class StaffGrp;
class System;
class Rest;
class Note;
class Beam;
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
    void setDuration(Rest *rest, hum::HumNum duration);
    void printMeasureTokens(void);
    int characterCount(const string &text, char symbol);
    int characterCount(hum::HTp token, char symbol);
    bool emptyMeasures(void);
    // void insertChord(vrv::Layer *element, hum::HTp token);
    void insertNoteInLayer(vrv::Layer *element, hum::HTp token);
    void insertNoteInBeam(vrv::Beam *element, hum::HTp token);
    void insertRestInLayer(vrv::Layer *element, hum::HTp token);
    void insertRestInBeam(vrv::Beam *element, hum::HTp token);
    void convertNote(vrv::Note *note, hum::HTp token, int subtoken = -1);
    void processTieStart(Note *note, hum::HTp token, const string &tstring);
    void processTieEnd(Note *note, hum::HTp token, const string &tstring);

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
    vector<vector<humaux::HumdrumTie> > m_ties;
};

} // namespace vrv

#endif // __VRV_IOHUMDRUM_H__
