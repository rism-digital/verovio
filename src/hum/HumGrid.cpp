//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Oct 16 16:08:05 PDT 2016
// Last Modified: Tue Nov  8 19:59:10 PST 2016
// Filename:      HumGrid.cpp
// URL:           https://github.com/craigsapp/hum2ly/blob/master/src/HumGrid.cpp
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   HumGrid is an intermediate container for converting from
//                MusicXML syntax into Humdrum syntax.
//
//

#include "HumGrid.h"
#include <string.h>
#include <iomanip>

using namespace std;

namespace hum {


//////////////////////////////
//
// HumGrid::HumGrid -- Constructor.
//

HumGrid::HumGrid(void) {
	// for now, limit to 100 parts:
	m_verseCount.resize(100);
	m_harmonyCount.resize(100);
	fill(m_harmonyCount.begin(), m_harmonyCount.end(), 0);

	// default options
	m_musicxmlbarlines = false;
	m_recip = true;
	m_pickup = false;
}



//////////////////////////////
//
// HumGrid::~HumGrid -- Deconstructor.
//

HumGrid::~HumGrid(void) {
	for (int i=0; i<(int)this->size(); i++) {
		if (this->at(i)) {
			delete this->at(i);
		}
	}
}


//////////////////////////////
//
// HumGrid::getHarmonyCount --
//

int HumGrid::getHarmonyCount(int partindex) {
	if ((partindex < 0) || (partindex >= (int)m_harmonyCount.size())) {
		return 0;
	}
	return m_harmonyCount.at(partindex);
}



//////////////////////////////
//
// HumGrid::getVerseCount --
//

int HumGrid::getVerseCount(int partindex, int staffindex) {
	if ((partindex < 0) || (partindex >= (int)m_verseCount.size())) {
		return 0;
	}
	int staffnumber = staffindex + 1;
	if ((staffnumber < 1) ||
			(staffnumber >= (int)m_verseCount.at(partindex).size())) {
		return 0;
	}
	int value = m_verseCount.at(partindex).at(staffnumber);
	return value;
}



//////////////////////////////
//
// HumGrid::setHarmonyCount -- part size hardwired to 100 for now.
//

void HumGrid::setHarmonyCount(int partindex, int count) {
	if ((partindex < 0) || (partindex > (int)m_harmonyCount.size())) {
		return;
	}
	m_harmonyCount[partindex] = count;
}



//////////////////////////////
//
// HumGrid::setVerseCount --
//

void HumGrid::setVerseCount(int partindex, int staffindex, int count) {
	if ((partindex < 0) || (partindex > (int)m_verseCount.size())) {
		return;
	}
	int staffnumber = staffindex + 1;
	if (staffnumber < 0) {
		return;
	}
	if (staffnumber < (int)m_verseCount.at(partindex).size()) {
		m_verseCount.at(partindex).at(staffnumber) = count;
	} else {
		int oldsize = (int)m_verseCount.at(partindex).size();
		int newsize = staffnumber + 1;
		m_verseCount.at(partindex).resize(newsize);
		for (int i=oldsize; i<newsize; i++) {
			m_verseCount.at(partindex).at(i) = 0;
		}
		m_verseCount.at(partindex).at(staffnumber) = count;
	}
}



//////////////////////////////
//
// HumGrid::transferTokens --
//

bool HumGrid::transferTokens(HumdrumFile& outfile) {
	bool status = buildSingleList();
	if (!status) {
		return false;
	}
	calculateGridDurations();
	addNullTokens();
	addMeasureLines();
	buildSingleList();
	addLastMeasure();
	if (manipulatorCheck()) {
		cleanupManipulators();
	}

	insertStaffIndications(outfile);
	insertPartIndications(outfile);
	insertExclusiveInterpretationLine(outfile);
	bool addstartbar = (!hasPickup()) && (!m_musicxmlbarlines);
	for (int m=0; m<(int)this->size(); m++) {
		if (addstartbar && m == 0) {
			status &= at(m)->transferTokens(outfile, m_recip, addstartbar);
		} else {
			status &= at(m)->transferTokens(outfile, m_recip, false);
		}
		if (!status) {
			break;
		}
	}
	insertDataTerminationLine(outfile);
	return true;
}



//////////////////////////////
//
// HumGrid::cleanupManipulators --
//

void HumGrid::cleanupManipulators(void) {
	int m;
	vector<GridSlice*> newslices;
	for (m=0; m<(int)this->size(); m++) {
		for (auto it = this->at(m)->begin(); it != this->at(m)->end(); it++) {
			if ((*it)->getType() != SliceType::Manipulators) {
				continue;
			}
			// check to see if manipulator needs to be split into
			// multiple lines.
			newslices.resize(0);
			cleanManipulator(newslices, *it);
			if (newslices.size()) {
				for (int j=0; j<(int)newslices.size(); j++) {
					this->at(m)->insert(it, newslices.at(j));
				}
			}
		}
	}
}



//////////////////////////////
//
// HumGrid::cleanManipulator --
//

void HumGrid::cleanManipulator(vector<GridSlice*>& newslices, GridSlice* curr) {
	newslices.resize(0);
	GridSlice* output;

	// deal with *^ manipulators:

// ggg implement later:
//	while (output = checkManipulatorExpand(curr)) {
//		newslices.push_back(output);
//	}

	// deal with *v manipulators:
	while ((output = checkManipulatorContract(curr))) {
		newslices.push_back(output);
	}
}



//////////////////////////////
//
// HumGrid::checkManipulatorContract -- Will only check for adjacent
//    *v records across adjacent staves, which should be good enough.
//    Will not check within a staff, but this should not occur within
//    MusicXML input data due to the way it is being processed.
//    The return value is a newly created GridSlice pointer which contains
//    a new manipulator to add to the file (and the current manipultor
//    slice will also be modified if the return value is not NULL).
//

GridSlice* HumGrid::checkManipulatorContract(GridSlice* curr) {
	GridVoice* lastvoice = NULL;
	GridVoice* voice     = NULL;
	GridStaff* staff     = NULL;
	GridPart*  part      = NULL;
	bool       neednew   = false;

	int p, s;
	int partcount = (int)curr->size();
	int staffcount;
	for (p=0; p<partcount; p++) {
		part  = curr->at(p);
		staffcount = (int)part->size();
		for (s=0; s<staffcount; s++) {
			staff = part->at(s);
			voice = staff->front();
			if ((p == 0) && (s == 0)) {
				lastvoice = staff->back();
				continue;
			}
			if (lastvoice != NULL) {
           	if ((*voice->getToken() == "*v") &&
						(*lastvoice->getToken() == "*v")) {
					neednew = true;
					break;
				}
			}
			lastvoice = staff->back();
		}
		if (neednew) {
			break;
		}
	}

	if (neednew == false) {
		return NULL;
	}

	// need to split *v's from different adjacent staves onto separate lines.

	GridSlice* newmanip = new GridSlice(curr->getMeasure(), curr->getTimestamp(),
		curr->getType(), curr);

	lastvoice = NULL;
	GridStaff* laststaff    = NULL;
	GridStaff* newstaff     = NULL;
	GridStaff* newlaststaff = NULL;
	bool foundnew = false;
	partcount = (int)curr->size();
	int lastp = 0;
	int lasts = 0;
	for (p=0; p<partcount; p++) {
		part  = curr->at(p);
		staffcount = (int)part->size();
		for (s=0; s<staffcount; s++) {
			staff = part->at(s);
			voice = staff->front();
			if (lastvoice != NULL) {
           	if ((*voice->getToken() == "*v") &&
						(*lastvoice->getToken() == "*v")) {
               // splitting the slices at this staff boundary
					newstaff     = newmanip->at(p)->at(s);
					newlaststaff = newmanip->at(lastp)->at(lasts);

					transferMerges(staff, laststaff, newstaff, newlaststaff);
					foundnew = true;
					break;
				}
			}
			laststaff = staff;
			lastvoice = staff->back();
			lastp = p;
			lasts = s;
		}
		if (foundnew) {
			break;
		}
	}

	return newmanip;
}



//////////////////////////////
//
// HumGrid::transferMerges -- Move *v spines from one staff to last staff,
//   and re-adjust staff "*v" tokens to a single "*" token.
// Example:
//                 laststaff      staff
// old:            *v   *v        *v   *v
// converts to:
// new:            *v   *v        *    *
// old:            *              *v   *v
//
//

void HumGrid::transferMerges(GridStaff* oldstaff, GridStaff* oldlaststaff,
		GridStaff* newstaff, GridStaff* newlaststaff) {

	if ((oldstaff == NULL) || (oldlaststaff == NULL)) {
		cerr << "Weird error in HumGrid::transferMerges()" << endl;
		return;
	}
	// New staves are presumed to be totally empty.

	GridVoice* gv;

	// First create "*" tokens for newstaff slice where there are
	// "*v" in old staff.  All other tokens should be set to "*".
	int tcount = (int)oldstaff->size();
	int t;
	for (t=0; t<tcount; t++) {
		if (*oldstaff->at(t)->getToken() == "*v") {
			gv = new GridVoice("*", 0);
			newstaff->push_back(gv);
		} else {
			gv = new GridVoice("*", 0);
			newstaff->push_back(gv);
		}
	}

	// Next, all "*v" tokens at end of old previous staff should be
	// transferred to the new previous staff and replaced with
	// a single "*" token.  Non "*v" tokens in the old last staff should
	// be converted to "*" tokens in the new last staff.
	//
	// It may be possible for *v tokens to not be only at the end of
	// the list of oldlaststaff tokens, but does not seem possible.

	tcount = (int)oldlaststaff->size();
	bool addednull = false;
	for (t=0; t<tcount; t++) {
		if (*oldlaststaff->at(t)->getToken() == "*v") {
			newlaststaff->push_back(oldlaststaff->at(t));
			if (addednull == false) {
				gv = new GridVoice("*", 0);
				oldlaststaff->at(t) = gv;
				addednull = true;
			} else {
				oldlaststaff->at(t) = NULL;
			}
		} else {
			gv = new GridVoice("*", 0);
			newlaststaff->push_back(gv);
		}
	}

	// Go back to the oldlaststaff and chop off all ending NULLs
	// * t should never get to zero (there should be at least one "*" left.
	// In theory intermediate NULLs should be checked for, and if they
	// exist, then something bad will happen.  But it does not seem
	// possible to have intermediate NULLs.
	tcount = (int)oldlaststaff->size();
	for (t=tcount-1; t>=0; t--) {
		if (oldlaststaff->at(t) == NULL) {
			int newsize = (int)oldlaststaff->size() - 1;
			oldlaststaff->resize(newsize);
		} else {
		}
	}
}



//////////////////////////////
//
// HumGrid::manipulatorCheck --
//

bool HumGrid::manipulatorCheck(void) {
	GridSlice* manipulator;
	int m;
	GridSlice* s1;
	GridSlice* s2;
	bool output = false;
	for (m=0; m<(int)this->size(); m++) {
		if (this->at(m)->size() == 0) {
			continue;
		}
		for (auto it = this->at(m)->begin(); it != this->at(m)->end(); it++) {
			s1 = *it;
			auto nextone = it;
			nextone++;
			if (nextone != this->at(m)->end()) {
				s2 = *nextone;
			} else if (m<(int)this->size()-1) {
				s2 = this->at(m+1)->front();
			} else {
				continue;
				// there is no next slice.  Presumably the terminal
				// barlines have already been added, so this will not
				// be a problem.
			}
			manipulator = manipulatorCheck(s1, s2);
			if (manipulator == NULL) {
				continue;
			}
			output = true;
			auto inserter = it;
			inserter++;
			this->at(m)->insert(inserter, manipulator);
			it++; // skip over the new manipulator line (expand it later)
		}
	}
	return output;
}


//
// HumGrid::manipulatorCheck -- Look for differences in voice/layer count
//   for each part/staff pairing between adjacent lines.  If they do not match,
//   then add spine manipulator line to Grid between the two lines.
//

GridSlice* HumGrid::manipulatorCheck(GridSlice* ice1, GridSlice* ice2) {
	int p1count;
	int p2count;
	int s1count;
	int s2count;
	int v1count;
	int v2count;
	int p;
	int s;
	int v;
	bool needmanip = false;

	if (ice1 == NULL) {
		return NULL;
	}
	if (ice2 == NULL) {
		return NULL;
	}
	p1count = (int)ice1->size();
	p2count = (int)ice2->size();
	if (p1count != p2count) {
		cerr << "Warning: Something weird happend here" << endl;
		cerr << "p1count = " << p1count << endl;
		cerr << "p2count = " << p2count << endl;
		cerr << "The above two values should be the same." << endl;
		return NULL;
	}
	for (p=0; p<p1count; p++) {
		s1count = (int)ice1->at(p)->size();
		s2count = (int)ice2->at(p)->size();
		if (s1count != s2count) {
			cerr << "Warning: Something weird happend here with staff" << endl;
			return NULL;
		}
		for (s=0; s<s1count; s++) {
			v1count = (int)ice1->at(p)->at(s)->size();
			// the voice count always must be at least 1.  This case
			// is related to inserting clefs in other parts.
			if (v1count < 1) {
				v1count = 1;
			}
			v2count = (int)ice2->at(p)->at(s)->size();
			if (v2count < 1) {
				v2count = 1;
			}
			if (v1count == v2count) {
				continue;
			}
			needmanip = true;
			break;
		}
		if (needmanip) {
			break;
		}
	}

	if (!needmanip) {
		return NULL;
	}

	// build manipulator line (which will be expanded further if adjacent
	// staves have *v manipulators.

	GridSlice* mslice;
	mslice = new GridSlice(ice1->getMeasure(), ice2->getTimestamp(),
			SliceType::Manipulators);

	int z;
	HTp token;
	GridVoice* gv;
	p1count = (int)ice1->size();
	mslice->resize(p1count);
	for (p=0; p<p1count; p++) {
		mslice->at(p) = new GridPart;
		s1count = (int)ice1->at(p)->size();
		mslice->at(p)->resize(s1count);
		for (s=0; s<s1count; s++) {
			mslice->at(p)->at(s) = new GridStaff;
			v1count = (int)ice1->at(p)->at(s)->size();
			v2count = (int)ice2->at(p)->at(s)->size();
			if (v1count == v2count) {
				for (v=0; v<v1count; v++) {
					token = new HumdrumToken("*");
					gv = new GridVoice(token, 0);
					mslice->at(p)->at(s)->push_back(gv);
				}
			} else if (v1count < v2count) {
				// need to grow
				int grow = v2count - v1count;
				if (grow == 2 * v1count) {
					// all subspines split
					for (z=0; z<v1count; z++) {
						token = new HumdrumToken("*^");
						gv = new GridVoice(token, 0);
						mslice->at(p)->at(s)->push_back(gv);
					}
				} else if (grow > 2 * v1count) {
					// too large to split all at the same time, deal with later
					for (z=0; z<v1count-1; z++) {
						token = new HumdrumToken("*^");
						gv = new GridVoice(token, 0);
						mslice->at(p)->at(s)->push_back(gv);
					}
					int extra = v2count - (v1count - 1) * 2;
					token = new HumdrumToken("*^" + to_string(extra));
					gv = new GridVoice(token, 0);
					mslice->at(p)->at(s)->push_back(gv);
				} else {
					// only split spines at end of list
					int doubled = v2count - v1count;
					int notdoubled = v1count - doubled;
					for (z=0; z<notdoubled; z++) {
						token = new HumdrumToken("*");
						gv = new GridVoice(token, 0);
						mslice->at(p)->at(s)->push_back(gv);
					}
					for (z=0; z<doubled; z++) {
						token = new HumdrumToken("*^");
						gv = new GridVoice(token, 0);
						mslice->at(p)->at(s)->push_back(gv);
					}
				}
			} else if (v1count > v2count) {
				// need to shrink
				int shrink = v1count - v2count + 1;
				int notshrink = v1count - shrink;
				for (z=0; z<notshrink; z++) {
					token = new HumdrumToken("*");
					gv = new GridVoice(token, 0);
					mslice->at(p)->at(s)->push_back(gv);
				}
				for (z=0; z<shrink; z++) {
					token = new HumdrumToken("*v");
					gv = new GridVoice(token, 0);
					mslice->at(p)->at(s)->push_back(gv);
				}
			}
		}
	}
	return mslice;
}



//////////////////////////////
//
// HumGrid::addMeasureLines --
//

void HumGrid::addMeasureLines(void) {
	HumNum timestamp;
	GridSlice* mslice;
	GridSlice* endslice;
	GridPart* part;
	GridStaff* staff;
	GridVoice* gv;
	string token;
	int staffcount, partcount, vcount, nextvcount, lcount;
	GridMeasure* measure = NULL;
	GridMeasure* nextmeasure = NULL;

	vector<int> barnums;
	if (!m_musicxmlbarlines) {
		getMetricBarNumbers(barnums);
	}

	for (int m=0; m<(int)this->size()-1; m++) {
		measure = this->at(m);
		nextmeasure = this->at(m+1);
		if (nextmeasure->size() == 0) {
			// next measure is empty for some reason so give up
			continue;
		}
		timestamp = nextmeasure->front()->getTimestamp();
		mslice = new GridSlice(measure, timestamp, SliceType::Measures);
		if (measure->size() == 0) {
			continue;
		}
		endslice = measure->back();
		measure->push_back(mslice);
		partcount = (int)nextmeasure->front()->size();
		mslice->resize(partcount);

		for (int p=0; p<partcount; p++) {
			part = new GridPart();
			mslice->at(p) = part;
			staffcount = (int)nextmeasure->front()->at(p)->size();
			mslice->at(p)->resize(staffcount);
			for (int s=0; s<(int)staffcount; s++) {
				staff = new GridStaff;
				mslice->at(p)->at(s) = staff;

				// insert the minimum number of barlines based on the
				// voices in the current and next measure.
				vcount = endslice->at(p)->at(s)->size();
				nextvcount = nextmeasure->front()->at(p)->at(s)->size();
				lcount = vcount;
				if (lcount > nextvcount) {
					lcount = nextvcount;
				}
				for (int v=0; v<lcount; v++) {
					token = createBarToken(m, barnums[m], measure);
					gv = new GridVoice(token, 0);
					mslice->at(p)->at(s)->push_back(gv);
				}
			}
		}
	}
}



//////////////////////////////
//
// HumGrid::createBarToken --
//

string HumGrid::createBarToken(int m, int barnum, GridMeasure* measure) {
	string token;
	string barstyle = getBarStyle(measure);
	string number = "";
	if (barnum > 0) {
		number = to_string(barnum);
	}
	if (m_musicxmlbarlines) {
		// m+1 because of the measure number
		// comes from the previous measure.
		if (barstyle == "=") {
			token = "==";
			token += to_string(m+1);
		} else {
			token = "=";
			token += to_string(m+1);
			token += barstyle;
		}
	} else {
		if (barnum > 0) {
			if (barstyle == "=") {
				token = "==";
				token += number;
			} else {
				token = "=";
				token += number;
				token += barstyle;
			}
		} else {
			if (barstyle == "=") {
				token = "==";
			} else {
				token = "=";
				token += barstyle;
			}
		}
	}
	return token;
}



//////////////////////////////
//
// HumGrid::addMetricBarNumbers --
//

void HumGrid::getMetricBarNumbers(vector<int>& barnums) {
	int mcount = (int)this->size();
	barnums.resize(mcount);

	if (mcount == 0) {
		return;
	}

	vector<HumNum> mdur(mcount);
	vector<HumNum> tsdur(mcount); // time signature duration

	for (int m=0; m<(int)this->size(); m++) {
		mdur[m]   = this->at(m)->getDuration();
		tsdur[m] = this->at(m)->getTimeSigDur();
		if (tsdur[m] <= 0) {
			tsdur[m] = mdur[m];
		}
	}

	int counter = 1;
	if (mdur[0] == tsdur[0]) {
		m_pickup = false;
		counter++;
		// add the initial barline later when creating HumdrumFile.
	} else {
		m_pickup = true;
	}

	for (int m=0; m<(int)this->size(); m++) {
		if ((m < mcount-1) && (tsdur[m] == tsdur[m+1])) {
			if (mdur[m] + mdur[m+1] == tsdur[m]) {
				barnums[m] = -1;
			} else {
				barnums[m] = counter++;
			}
		} else {
			barnums[m] = counter++;
		}
	}
}



//////////////////////////////
//
// HumGrid::getBarStyle --
//

string HumGrid::getBarStyle(GridMeasure* measure) {
	string output = "";
	if (measure->isDouble()) {
		output = "||";
	} else if (measure->isFinal()) {
		output = "=";
	} else if (measure->isRepeatBoth()) {
		output = ":|!|:";
	} else if (measure->isRepeatBackward()) {
		output = ":|!";
	} else if (measure->isRepeatForward()) {
		output = "!|:";
	}
	return output;
}



//////////////////////////////
//
// HumGrid::addLastMeasure --
//

void HumGrid::addLastMeasure(void) {
   // add the last measure, which will be only one voice
	// for each part/staff.
	GridSlice* model = this->back()->back();

	// probably not the correct timestamp, but probably not important
	// to get correct:
	HumNum timestamp = model->getTimestamp();

	if (this->empty()) {
		return;
	}
	GridMeasure* measure = this->back();

	string barstyle = getBarStyle(measure);

	GridSlice* mslice = new GridSlice(model->getMeasure(), timestamp,
			SliceType::Measures);
	this->back()->push_back(mslice);
	mslice->setTimestamp(timestamp);
	int partcount = model->size();
	mslice->resize(partcount);
	for (int p=0; p<partcount; p++) {
		GridPart* part = new GridPart();
		mslice->at(p) = part;
		int staffcount = (int)model->at(p)->size();
		mslice->at(p)->resize(staffcount);
		for (int s=0; s<staffcount; s++) {
			GridStaff* staff = new GridStaff;
			mslice->at(p)->at(s) = staff;
			HTp token = new HumdrumToken("=" + barstyle);
			GridVoice* gv = new GridVoice(token, 0);
			mslice->at(p)->at(s)->push_back(gv);
		}
	}
}



//////////////////////////////
//
// HumGrid::buildSingleList --
//

bool HumGrid::buildSingleList(void) {
	m_allslices.resize(0);

	int gridcount = 0;
	for (auto it : (vector<GridMeasure*>)*this) {
		gridcount += (int)it->size();
	}
	m_allslices.reserve(gridcount + 100);
	for (int m=0; m<(int)this->size(); m++) {
		for (auto it : (list<GridSlice*>)*this->at(m)) {
			m_allslices.push_back(it);
		}
	}

	HumNum ts1;
	HumNum ts2;
	HumNum dur;
	for (int i=0; i<(int)m_allslices.size() - 1; i++) {
		ts1 = m_allslices[i]->getTimestamp();
		ts2 = m_allslices[i+1]->getTimestamp();
		dur = (ts2 - ts1); // whole-note units
		m_allslices[i]->setDuration(dur);
	}

	return !m_allslices.empty();
}



//////////////////////////////
//
// HumGrid::addNullTokens --
//

void HumGrid::addNullTokens(void) {
	int i; // slice index
	int p; // part index
	int s; // staff index
	int v; // voice index

	if (0) {
		cerr << "SLICE TIMESTAMPS: " << endl;
		for (int x=0; x<(int)m_allslices.size(); x++) {
			cerr << "\tTIMESTAMP " << x << "= " 
			     << m_allslices[x]->getTimestamp()
			     << "\tDUR=" << m_allslices[x]->getDuration()
			     << "\t"
			     << m_allslices[x]
			     << endl;
		}
	}

	for (i=0; i<(int)m_allslices.size(); i++) {
		GridSlice& slice = *m_allslices.at(i);
		if (!slice.isNoteSlice()) {
			// probably need to deal with grace note slices here
			continue;
		}
      for (p=0; p<(int)slice.size(); p++) {
			GridPart& part = *slice.at(p);
      	for (s=0; s<(int)part.size(); s++) {
				GridStaff& staff = *part.at(s);
      		for (v=0; v<(int)staff.size(); v++) {
					if (!staff.at(v)) {
						// in theory should not happen
						continue;
					}
					GridVoice& gv = *staff.at(v);
					if (gv.isNull()) {
						continue;
					}
					// found a note/rest which should have a non-zero
					// duration that needs to be extended to the next
					// duration in the
					extendDurationToken(i, p, s, v);
				}
			}
		}
	}
}



//////////////////////////////
//
// HumGrid::extendDurationToken --
//

void HumGrid::extendDurationToken(int slicei, int parti, int staffi,
		int voicei) {
	if ((slicei < 0) || (slicei >= ((int)m_allslices.size()) - 1)) {
		// nothing after this line, so can extend further.
		return;
	}

	GridVoice* gv = m_allslices.at(slicei)->at(parti)->at(staffi)->at(voicei);
 	HTp token = gv->getToken();
	if (!token) {
		cerr << "STRANGE: token should not be null" << endl;
		return;
	}
	if (*token == ".") {
		// null data token so ignore;
		// change this later to add a duration for the null token below.
		return;
	}
	
	HumNum tokendur = Convert::recipToDuration((string)*token);
	HumNum currts   = m_allslices.at(slicei)->getTimestamp();
	HumNum nextts   = m_allslices.at(slicei+1)->getTimestamp();
	HumNum slicedur = nextts - currts;
	HumNum timeleft = tokendur - slicedur;

	if (0) {
		cerr << "===================" << endl;
		cerr << "EXTENDING TOKEN    " << token      << endl;
		cerr << "\tTOKEN DUR:       " << tokendur   << endl;
		cerr << "\tTOKEN START:     " << currts     << endl;
		cerr << "\tSLICE DUR:       " << slicedur   << endl;
		cerr << "\tNEXT SLICE START:" << nextts     << endl;
		cerr << "\tTIME LEFT:       " << timeleft   << endl;
		cerr << "\t-----------------" << endl;
	}

	if (timeleft != 0) {
		// fill in null tokens for the required duration.
		if (timeleft < 0) {
			cerr << "ERROR: Negative duration" << endl;
			return;
		}

		SliceType type;
		GridStaff* gs;
		int s = slicei+1;

		while ((s < (int)m_allslices.size()) && (timeleft > 0)) {
			currts = nextts;
			if (s < (int)m_allslices.size() - 1) {
				nextts = m_allslices.at(s+1)->getTimestamp();
			} else {
				nextts = currts + m_allslices.at(s)->getDuration();
			}
			slicedur = nextts - currts;
			type = m_allslices[s]->getType();

			gs = m_allslices.at(s)->at(parti)->at(staffi);
			if (gs == NULL) {
				cerr << "Strange error2 in extendDurationToken()" << endl;
				return;
			}
			gs->setNullTokenLayer(voicei, type, slicedur);
			
			if (m_allslices.at(s)->isDataSlice()) {
				gs->setNullTokenLayer(voicei, type, slicedur);
				timeleft = timeleft - slicedur;
			} else {
				// store a null token for the non-data slice, but probably skip
				// if there is a token already there (such as a clef-change).
				gs->setNullTokenLayer(voicei, type, slicedur);
			}
			s++;
			if (s == (int)m_allslices.size() - 1) {
				m_allslices[s]->setDuration(timeleft);
			}
		}
	}
	// walk through zero-dur items and fill them in, but stop at
	// a token (likely a grace note which should not be erased).
// ggg

}



//////////////////////////////
//
// HumGrid::getGridVoice -- Check to see if GridVoice exists, returns
//    NULL otherwise. Requires HumGrid::buildSingleList() being run first.
//

GridVoice* HumGrid::getGridVoice(int slicei, int parti, int staffi,
		int voicei) {
	if (slicei >= (int)m_allslices.size()) {
		cerr << "Strange error 1a" << endl;
		return NULL;
	}
	GridSlice* gsl = m_allslices.at(slicei);
	if (gsl == NULL) {
		cerr << "Strange error 1b" << endl;
		return NULL;
	}

	if (parti >= (int)gsl->size()) {
		cerr << "Strange error 2a" << endl;
		return NULL;
	}
	GridPart* gp = gsl->at(parti);
	if (gp == NULL) {
		cerr << "Strange error 2" << endl;
		return NULL;
	}

	if (staffi >= (int)gp->size()) {
		cerr << "Strange error 3a" << endl;
		return NULL;
	}
	GridStaff* gst = gp->at(staffi);
	if (gst == NULL) {
		cerr << "Strange error 3b" << endl;
		return NULL;
	}

	if (voicei >= (int)gst->size()) {
		cerr << "Strange error 4a" << endl;
		return NULL;
	}
	GridVoice* gv = gst->at(voicei);
	if (gv == NULL) {
		cerr << "Strange error 4b" << endl;
		return NULL;
	}
	return gv;
}



//////////////////////////////
//
// HumGrid::calculateGridDurations --
//

void HumGrid::calculateGridDurations(void) {

	// the last line has to be calculated from the shortest or
   // longest duration on the line.  Acutally all durations
	// starting on this line must be the same, so just search for
	// the first duration.

	auto last = m_allslices.back();

	// set to zero in case not a duration type of line:
	last->setDuration(0);

	bool finished = false;
	if (last->isNoteSlice()) {
		for (auto part : *last) {
			for (auto staff : *part) {
				for (auto voice : *staff) {
					if (!voice) {
						continue;
					}
					if (voice->getDuration() > 0) {
						last->setDuration(voice->getDuration());
						finished = true;
						break;
					}
				}
				if (finished) {
					break;
				}
			}
			if (finished) {
				break;
			}
		}
	}
}



//////////////////////////////
//
// HumGrid::insertExclusiveInterpretationLine -- Currently presumes
//    that the first entry contains spines.  And the first measure
//    in the HumGrid object must contain a slice.
//

void HumGrid::insertExclusiveInterpretationLine(HumdrumFile& outfile) {
	if (this->size() == 0) {
		return;
	}
	if (this->at(0)->empty()) {
		return;
	}

	HumdrumLine* line = new HumdrumLine;
	HTp token;

	if (m_recip) {
		token = new HumdrumToken("**recip");
		line->appendToken(token);
	}

	GridSlice& slice = *this->at(0)->front();
	int p; // part index
	int s; // staff index
	for (p=(int)slice.size()-1; p>=0; p--) {
		GridPart& part = *slice[p];
		for (s=(int)part.size()-1; s>=0; s--) {
			token = new HumdrumToken("**kern");
			line->appendToken(token);
			insertExInterpSides(line, p, s); // insert staff sides
		}
		insertExInterpSides(line, p, -1);   // insert part sides
	}
	outfile.insertLine(0, line);
}



//////////////////////////////
//
// HumGrid::insertExInterpSides --
//

void HumGrid::insertExInterpSides(HumdrumLine* line, int part, int staff) {

	if (staff >= 0) {
		int versecount = getVerseCount(part, staff); // verses related to staff
		for (int i=0; i<versecount; i++) {
			HTp token = new HumdrumToken("**text");
			line->appendToken(token);
		}
	}

	if (staff < 0) {
		int harmonyCount = getHarmonyCount(part);
		for (int i=0; i<harmonyCount; i++) {
			HTp token = new HumdrumToken("**mxhm");
			line->appendToken(token);
		}
	}

}



//////////////////////////////
//
// HumGrid::insertPartIndications -- Currently presumes
//    that the first entry contains spines.  And the first measure
//    in the HumGrid object must contain a slice.  This is the
//    MusicXML Part number. (Some parts will contain more than one
//    staff).
//

void HumGrid::insertPartIndications(HumdrumFile& outfile) {
	if (this->size() == 0) {
		return;
	}
	if (this->at(0)->empty()) {
		return;
	}
	HumdrumLine* line = new HumdrumLine;
	HTp token;

	if (m_recip) {
		token = new HumdrumToken("*");
		line->appendToken(token);
	}

	string text;
	GridSlice& slice = *this->at(0)->front();
	int p; // part index
	int s; // staff index
	for (p=(int)slice.size()-1; p>=0; p--) {
		GridPart& part = *slice[p];
		for (s=(int)part.size()-1; s>=0; s--) {
			text = "*part" + to_string(p+1);
			token = new HumdrumToken(text);
			line->appendToken(token);
			insertSidePartInfo(line, p, s);
		}
		insertSidePartInfo(line, p, -1);   // insert part sides
	}
	outfile.insertLine(0, line);
}



//////////////////////////////
//
// HumGrid::insertSidePartInfo --
//

void HumGrid::insertSidePartInfo(HumdrumLine* line, int part, int staff) {
	HTp token;
	string text;

	if (staff < 0) {
		int harmcount = getHarmonyCount(part);
		for (int i=0; i<harmcount; i++) {
			text = "*part" + to_string(part+1);
			token = new HumdrumToken(text);
			line->appendToken(token);
		}
	} else {
		int versecount = getVerseCount(part, staff);
		for (int i=0; i<versecount; i++) {
			text = "*part" + to_string(part+1);
			token = new HumdrumToken(text);
			line->appendToken(token);
		}
	}
}



//////////////////////////////
//
// HumGrid::insertStaffIndications -- Currently presumes
//    that the first entry contains spines.  And the first measure
//    in the HumGrid object must contain a slice.  This is the
//    MusicXML Part number. (Some parts will contain more than one
//    staff).
//

void HumGrid::insertStaffIndications(HumdrumFile& outfile) {
	if (this->size() == 0) {
		return;
	}
	if (this->at(0)->empty()) {
		return;
	}

	HumdrumLine* line = new HumdrumLine;
	HTp token;

	if (m_recip) {
		token = new HumdrumToken("*");
		line->appendToken(token);
	}

	string text;
	GridSlice& slice = *this->at(0)->front();
	int p; // part index
	int s; // staff index

	int staffcount = 0;
	for (p=0; p<(int)slice.size(); p++) {
		GridPart& part = *slice[p];
		staffcount += (int)part.size();
	}

	for (p=(int)slice.size()-1; p>=0; p--) {
		GridPart& part = *slice[p];
		for (s=(int)part.size()-1; s>=0; s--) {
			text = "*staff" + to_string(staffcount--);
			token = new HumdrumToken(text);
			line->appendToken(token);
			insertSideStaffInfo(line, p, s, staffcount+1);
		}
		insertSideStaffInfo(line, p, -1, -1);  // insert part sides
	}
	outfile.insertLine(0, line);
}



//////////////////////////////
//
// HumGrid::insertSideStaffInfo --
//

void HumGrid::insertSideStaffInfo(HumdrumLine* line, int part, int staff,
		int staffnum) {
	HTp token;
	string text;

	// part-specific sides (no staff markers)
	if (staffnum < 0) {
		int harmcount = getHarmonyCount(part);
		for (int i=0; i<harmcount; i++) {
			token = new HumdrumToken("*");
			line->appendToken(token);
		}
		return;
	}

	int versecount = getVerseCount(part, staff);
	for (int i=0; i<versecount; i++) {
		if (staffnum > 0) {
			text = "*staff" + to_string(staffnum);
			token = new HumdrumToken(text);
		} else {
			token = new HumdrumToken("*");
		}
		line->appendToken(token);
	}


}



//////////////////////////////
//
// HumGrid::insertDataTerminationLine -- Currently presumes
//    that the last entry contains spines.  And the first
//    measure in the HumGrid object must contain a slice.
//    Also need to compensate for *v on previous line.
//

void HumGrid::insertDataTerminationLine(HumdrumFile& outfile) {
	if (this->size() == 0) {
		return;
	}
	if (this->at(0)->empty()) {
		return;
	}
	HumdrumLine* line = new HumdrumLine;
	HTp token;

	if (m_recip) {
		token = new HumdrumToken("*-");
		line->appendToken(token);
	}

	GridSlice& slice = *this->at(0)->back();
	int p; // part index
	int s; // staff index
	for (p=(int)slice.size()-1; p>=0; p--) {
		GridPart& part = *slice[p];
		for (s=(int)part.size()-1; s>=0; s--) {
			token = new HumdrumToken("*-");
			line->appendToken(token);
			insertSideTerminals(line, p, s);
		}
		insertSideTerminals(line, p, -1);   // insert part sides
	}
	outfile.appendLine(line);
}



//////////////////////////////
//
// HumGrid::insertSideTerminals --
//

void HumGrid::insertSideTerminals(HumdrumLine* line, int part, int staff) {
	HTp token;

	if (staff < 0) {
		int harmcount = getHarmonyCount(part);
		for (int i=0; i<harmcount; i++) {
			token = new HumdrumToken("*-");
			line->appendToken(token);
		}
	} else {
		int versecount = getVerseCount(part, staff);
		for (int i=0; i<versecount; i++) {
			token = new HumdrumToken("*-");
			line->appendToken(token);
		}
	}
}



//////////////////////////////
//
// HumGrid::removeRedundantClefChanges -- Will also have to consider
//		the meter signature.
//

void HumGrid::removeRedundantClefChanges(void) {
	// curclef is a list of the current staff on the part:staff.
	vector<vector<string> > curclef;

	bool hasduplicate = false;
	GridMeasure* measure;
	GridVoice* voice;
	HTp token;
	for (int m=0; m<(int)this->size(); m++) {
		measure = this->at(m);
		for (auto slice : *measure) {
			if (!slice->isClefSlice()) {
				continue;
			}
			bool allempty = true;
			for (int p=0; p<(int)slice->size(); p++) {
				for (int s=0; s<(int)slice->at(p)->size(); s++) {
					if (slice->at(p)->at(s)->size() < 1) {
						continue;
					}
					voice = slice->at(p)->at(s)->at(0);
					token = voice->getToken();
					if (!token) {
						continue;
					}
					if (string(*token) == "*") {
						continue;
					}
					if (token->find("clef") == string::npos) {
						// something (probably invalid) which is not a clef change
						allempty = false;
						continue;
					}
					if (p >= (int)curclef.size()) {
						curclef.resize(p+1);
					}
					if (s >= (int)curclef[p].size()) {
						// first clef on the staff, so can't be a duplicate
						curclef[p].resize(s+1);
						curclef[p][s] = *token;
						allempty = false;
						continue;
					} else {
						if (curclef[p][s] == (string)*token) {
							// clef is already active, so remove this one
							hasduplicate = true;
							voice->setToken("*");
						} else {
							// new clef change
							curclef[p][s] = *token;
							allempty = false;
						}
					}
				}
			}
			if (!hasduplicate) {
				continue;
			}
			// Check the slice to see if it empty, and delete if so.
			// This algorithm does not consider GridSide content.
			if (allempty) {
				slice->invalidate();
			}
			
		}
	}
}



//////////////////////////////
//
// HumGrid::hasPickup --
//

bool HumGrid::hasPickup(void) {
	return m_pickup;
}



} // end namespace hum



