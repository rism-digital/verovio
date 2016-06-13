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
#include "io.h"
#include "vrvdef.h"

#include "humlib.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// HumdrumInput
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
};

} // namespace vrv

#endif // __VRV_IOHUMDRUM_H__
