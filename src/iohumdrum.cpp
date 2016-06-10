/////////////////////////////////////////////////////////////////////////////
// Name:        iohumdrum.cpp
// Author:      Craig Stuart Sapp
// Created:     06/06/2015
// vim:         ts=3
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef NO_HUMDRUM_SUPPORT

#include "iohumdrum.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <sstream>
#include <algorithm>
#include <vector>

//----------------------------------------------------------------------------

#include "doc.h"
#include "rest.h"
#include "vrv.h"
#include "iomei.h"

//#include "attcomparison.h"
//#include "beam.h"
//#include "chord.h"
//#include "mrest.h"
//#include "note.h"
//#include "slur.h"
//#include "syl.h"
//#include "text.h"
//#include "tie.h"
//#include "tuplet.h"
//#include "verse.h"
//#include "pugixml.hpp"


using namespace hum;   // humlib namespace
using namespace std;


namespace vrv {


//////////////////////////////
//
// HumdrumInput::HumdrumInput -- Constructor.
//

HumdrumInput::HumdrumInput(Doc *doc, std::string filename)
		: FileInputStream(doc) {
	m_filename = filename;
	m_page     = NULL;
	m_system   = NULL;
	m_measure  = NULL;
	m_staff    = NULL;
	m_layer    = NULL;
	m_debug    = 1;
}



//////////////////////////////
//
// HumdrumInput::~HumdrumInput -- Deconstructor.
//

HumdrumInput::~HumdrumInput() {
	clear();
}



//////////////////////////////
//
// HumdrumInput::ImportFile -- Read a Humdrum file from a file.
//

bool HumdrumInput::ImportFile() {
	try {
		m_doc->Reset(Raw);
		HumdrumFile infile;
		bool result = infile.read(m_filename);
		if (!result) {
			return false;
		}
		return convertHumdrum(infile);
	}
	catch (char *str) {
		LogError("%s", str);
		return false;
	}
}



//////////////////////////////
//
// HumdrumInput::ImportString -- Read a Humdrum file from a text string.
//

bool HumdrumInput::ImportString(const std::string content) {
	try {
		m_doc->Reset(Raw);
		HumdrumFile infile;
		bool result = infile.readString(content);
		if (!result) {
			return false;
		}
		return convertHumdrum(infile);
	}
	catch (char *str) {
		LogError("%s", str);
		return false;
	}
}


///////////////////////////////////////////////////////////////////////////
//
// Protected functions.
//

//////////////////////////////
//
// HumdrumInput::convertHumdrum -- Top level method called from ImportFile or
//     ImportString.  Convert a HumdrumFile structure into an MEI
//     structure.  Returns false if there was an error in the conversion
//     process.
//
// Reference:
//     http://music-encoding.org/documentation/2.1.1/cmn
//

bool HumdrumInput::convertHumdrum(HumdrumFile& infile) {
	bool status = true;  // for keeping track of problems in conversion process.

	setupMeiDocument();

	// Create a list of the parts and which spine represents them.
	vector<HTp> kernstarts = infile.getKernSpineStartList();

	if (kernstarts.size() == 0) {
		// no parts in file, give up.  Perhaps return an error.
		return status;
	}

	// Reverse the order, since top part is last spine.
	reverse(kernstarts.begin(), kernstarts.end());

	int line = kernstarts[0]->getLineIndex();
	while (line < infile.getLineCount() - 1 && (line >= 0)) {
		status &= convertSystemMeasure(infile, kernstarts, line);
	}

	// calculateLayout();

	if (m_debug) {
		MeiOutput meioutput(m_doc, "");
		meioutput.SetScoreBasedMEI(true);
		string mei = meioutput.GetOutput();
		cout << mei;
	}

	return status;
}



//////////////////////////////
//
// HumdrumInput::convertSystemMeasure -- Convert a measure of Humdrum data into
//    a measure of MEI data.
//

bool HumdrumInput::convertSystemMeasure(HumdrumFile& infile,
		const vector<HTp>& kernstarts, int& line) {

	int startline = line;
	int endline   = getMeasureEndLine(infile, startline);
	if (endline < 0) {
		// empty measure, skip it.  This can happen at the start of
		// a score if there is an invisible measure before the start of the
		// data, or if there is an ending bar before the ending of the data.
		line = -endline;
		return true;
	} else {
		line = endline;
	}

	setupSystemMeasure(infile, kernstarts, startline, endline);

	return convertMeasureStaves(infile, kernstarts, startline, endline);
}



//////////////////////////////
//
// HumdrumInput::convertMeasureStaves -- fill in a measure with the
//    individual staff elements for each part.
//

bool HumdrumInput::convertMeasureStaves(HumdrumFile& infile,
		const vector<HTp>& kernstarts, int startline, int endline) {

	vector<int> layers = getStaffLayerCounts(infile, kernstarts, startline,
			endline);

	bool status = true;
	for (int i=0; i<(int)kernstarts.size(); i++) {
		status &= convertMeasureStaff(infile, kernstarts[i]->getTrack(),
				startline, endline, i+1, layers[i]);
		if (!status) {
			break;
		}
	}

	return status;
}



//////////////////////////////
//
// HumdrumInput::convertMeasureStaff -- print a particular staff in a
//     particular measure.
//

bool HumdrumInput::convertMeasureStaff(HumdrumFile& infile, int track,
		int startline, int endline, int n, int layercount) {

	m_staff = new Staff();
	m_measure->AddStaff(m_staff);
	m_staff->SetN(n);

	bool status = true;
	for (int i=0; i<layercount; i++) {
		status &= convertStaffLayer(infile, track, startline, endline, i);
		if (!status) {
			break;
		}
	}

	return status;
}



//////////////////////////////
//
// HumdrumInput::convertStaffLayer -- Prepare a layer element in the current
//   staff and then fill it with data.
//

bool HumdrumInput::convertStaffLayer(HumdrumFile& infile, int track,
		int startline, int endline, int layerindex) {
	m_layer = new Layer();
	m_layer->SetN(layerindex + 1);
	m_staff->AddLayer(m_layer);

	return fillContentsOfLayer(infile, track, startline, endline, layerindex);
}



//////////////////////////////
//
// HumdrumInput::convertStaffLayer -- Fill the layer with musical data.
//

bool HumdrumInput::fillContentsOfLayer(HumdrumFile& infile, int track,
		int startline, int endline, int layerindex) {

	Rest* rest = new Rest();
	rest->SetDur(DURATION_8);
	m_layer->AddLayerElement(rest);

	return true;
}



//////////////////////////////
//
// HumdrumInput::getStaffLayerCounts -- Return the maximum layer count for each
//    part within the measure.
//

vector<int> HumdrumInput::getStaffLayerCounts(HumdrumFile& infile,
		const vector<HTp>& kernstarts, int startline, int endline) {
	vector<int> maxvals(kernstarts.size(), 0);
	vector<int> curvals(kernstarts.size(), 0);
	vector<int> rkern(infile.getSpineCount() + 1, -1);
	int i, j, track;

	for (i=0; i<(int)kernstarts.size(); i++) {
		rkern[kernstarts[i]->getTrack()] = i;
	}

	for (i=startline; i<=endline; i++) {
		if (!infile[i].isData()) {
			continue;
		}
		std::fill(curvals.begin(), curvals.end(), 0);
		for (j=0; j<infile[i].getFieldCount(); j++) {
			track = infile[i].token(j)->getTrack();
			if (rkern[track] < 0) {
				continue;
			}
			curvals[rkern[track]]++;
		}
		for (j=0; j<(int)curvals.size(); j++) {
			if (curvals[j] > maxvals[j]) {
				maxvals[j] = curvals[j];
			}
		}
	}

	return maxvals;
}



//////////////////////////////
//
// HumdrumInput::setupSystemMeasure -- prepare a new system measure.
//

void HumdrumInput::setupSystemMeasure(HumdrumFile& infile,
		const vector<HTp>& kernstarts, int startline, int endline) {
	m_measure = new Measure();
	m_system->AddMeasure(m_measure);

	int measurenumber = getMeasureNumber(infile, startline, endline);
	if (measurenumber >= 0) {
		m_measure->SetN(measurenumber);
	}

	setSystemMeasureStyle(infile, kernstarts, startline, endline);
}



//////////////////////////////
//
// HumdrumInput::setSystemMeasureStyle -- Set the style of the left and/or
//    right barline for the measure.
//
// Bar types listed in ../libmei/atttypes.h
//     BARRENDITION_NONE = 0,
//     BARRENDITION_dashed,
//     BARRENDITION_dotted,
//     BARRENDITION_dbl,
//     BARRENDITION_dbldashed,
//     BARRENDITION_dbldotted,
//     BARRENDITION_end,
//     BARRENDITION_invis,
//     BARRENDITION_rptstart,
//     BARRENDITION_rptboth,
//     BARRENDITION_rptend,
//     BARRENDITION_single,
// See also:
//      http://music-encoding.org/documentation/2.1.1/data.BARRENDITION
//      http://music-encoding.org/guidelines/3.0.0/Images/ExampleImages/barline-20100510.png
//

void HumdrumInput::setSystemMeasureStyle(HumdrumFile& infile,
		const vector<HTp>& kernstarts, int startline, int endline) {

	string endbar   = infile[endline].getTokenString(0);
	string startbar = infile[startline].getTokenString(0);
	if (endbar.compare(0, 2, "==") == 0) {
		m_measure->SetRight(BARRENDITION_end);
	} else if (endbar.find(":|!|:") != string::npos) {
		m_measure->SetRight(BARRENDITION_rptend);
	} else if (endbar.find(":!!:") != string::npos) {
		m_measure->SetRight(BARRENDITION_rptend);
	} else if (endbar.find(":||:") != string::npos) {
		m_measure->SetRight(BARRENDITION_rptend);
	} else if (endbar.find(":!:") != string::npos) {
		m_measure->SetRight(BARRENDITION_rptend);
	} else if (endbar.find(":|:") != string::npos) {
		m_measure->SetRight(BARRENDITION_rptend);
	} else if (endbar.find(":|") != string::npos) {
		m_measure->SetRight(BARRENDITION_rptend);
	} else if (endbar.find(":!") != string::npos) {
		m_measure->SetRight(BARRENDITION_rptend);
	} else if (endbar.find("-") != string::npos) {
		m_measure->SetRight(BARRENDITION_invis);
	}

	if (startbar.find(":|!|:") != string::npos) {
		m_measure->SetLeft(BARRENDITION_rptstart);
	} else if (startbar.find(":!!:") != string::npos) {
		m_measure->SetLeft(BARRENDITION_rptstart);
	} else if (startbar.find(":||:") != string::npos) {
		m_measure->SetLeft(BARRENDITION_rptstart);
	} else if (startbar.find(":!:") != string::npos) {
		m_measure->SetLeft(BARRENDITION_rptstart);
	} else if (startbar.find(":|:") != string::npos) {
		m_measure->SetLeft(BARRENDITION_rptstart);
	} else if (startbar.find("|:") != string::npos) {
		m_measure->SetLeft(BARRENDITION_rptend);
	} else if (startbar.find("!:") != string::npos) {
		m_measure->SetLeft(BARRENDITION_rptend);
	} else if (startbar.find("-") != string::npos) {
		m_measure->SetLeft(BARRENDITION_invis);
	}
}



//////////////////////////////
//
// HumdrumInput::getMeasureEndLine -- Return the line index of the
//   ending of a given measure.  This is usually a barline, but can be
//   the end of a file if there is no terminal barline in the **kern
//   data.  Returns a negative line number if there is no data in the
//   measure.
//

int HumdrumInput::getMeasureEndLine(HumdrumFile& infile, int startline) {
	int endline = infile.getLineCount() - 1;
	bool foundDataQ = false;
	int i = startline+1;
	while (i < infile.getLineCount()) {
		if (infile[i].isData()) {
			foundDataQ = true;
		} else if (infile[i].isBarline()) {
			endline = i;
			break;
		}
		endline = i;
		i++;
	}

	if (foundDataQ) {
		return endline;
	} else {
		return -endline;
	}
}



//////////////////////////////
//
// HumdumInput::setupMeiDocument -- Add a page and a system on the page to
//     get things started.
//

void HumdrumInput::setupMeiDocument(void) {
	m_page = new Page();
	m_doc->AddPage(m_page);
	m_system = new System();
	m_page->AddSystem(m_system);
}



//////////////////////////////
//
// HumdrumInput::clear -- clear contents of object (m_doc handled by
//    parent class).
//

void HumdrumInput::clear(void) {
	m_filename = "";
	m_page     = NULL;
	m_system   = NULL;
}



//////////////////////////////
//
// HumdrumInput::getMeasureNumber -- Return the current barline's measure
//     number, or return -1 if no measure number.  Returns 0 if a
//     pickup measure.
//

int HumdrumInput::getMeasureNumber(HumdrumFile& infile, int startline,
		int endline) {
	if (infile[startline].getTokenString(0).compare(0, 2, "**") == 0) {
		// create HumdrumFile.hasPickup() function and uncomment out below:
		// if (infile.hasPickup()) {
		//    return 1;
		// } else {
			return 0;
		// }
	}

	int number;
	if (sscanf(infile[startline].getTokenString(0).c_str(), "=%d", &number)) {
		return number;
	} else {
		return -1;
	}
}



//////////////////////////////
//
// HumdrumInput::calculateLayout -- Have verovio figure out the
//     line break (and page break) locations in the data.  Do this
//     after the Humdrum data has finished loading.
//

void HumdrumInput::calculateLayout(void) {
	m_doc->CastOff();
}



} // namespace vrv


/*

	Layer *layer         = NULL;
	Note *note           = NULL;
	Beam *beam           = NULL;
	Tuplet *tuplet       = NULL;
	Tie *tie             = NULL;
	Chord *chord         = NULL;
	Slur *slur           = NULL;
	ScoreDef *scoreDef   = NULL;

	// staffGrp and staffDef
	StaffGrp *staffGrp = new StaffGrp();
	StaffDef *staffDef = new StaffDef();
	staffDef->SetN(1);
	staffDef->SetLines(5);

	// clef
	staffDef->SetClefLine(2);
	staffDef->SetClefShape(CLEFSHAPE_G);

	m_doc->m_scoreDef.SetKeySig(KEYSIGNATURE_2s);
	m_doc->m_scoreDef.SetMeterSym(METERSIGN_common);

	staffGrp->AddStaffDef(staffDef);
	m_doc->m_scoreDef.AddStaffGrp(staffGrp);


}


*/



#endif  /* NO_HUMDRUM_SUPPORT */



