//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Oct 16 16:08:05 PDT 2016
// Last Modified: Sun Oct 16 16:08:08 PDT 2016
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

using namespace std;

namespace hum {


//////////////////////////////
//
// HumGrid::HumGrid -- Constructor.
//

HumGrid::HumGrid(void) {
	// do nothing;
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
// HumGrid::transferTokens --
//

void HumGrid::transferTokens(HumdrumFile& outfile) {
	buildSingleList();
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
	int m; // measure index
	for (m=0; m<(int)this->size(); m++) {
		at(m)->transferTokens(outfile, m_recip);
	}
	insertDataTerminationLine(outfile);
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
	GridToken* lastvoice = NULL;
	GridToken* voice     = NULL;
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
			staff = part->at(p);
			voice = staff->front();
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

	GridSlice* newmanip = new GridSlice(curr->getTimestamp(), curr->getType(),
		curr);

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

	GridToken* gt;

	// First create "*" tokens for newstaff slice where there are
	// "*v" in old staff.  All other tokens should be set to "*".
	int tcount = (int)oldstaff->size();
	int t;
	for (t=0; t<tcount; t++) {
		if (*oldstaff->at(t)->getToken() == "*v") {
			gt = new GridToken("*", 0);
			newstaff->push_back(gt);
		} else {
			gt = new GridToken("*", 0);
			newstaff->push_back(gt);
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
				gt = new GridToken("*", 0);
				oldlaststaff->at(t) = gt;
				addednull = true;
			} else {
				oldlaststaff->at(t) = NULL;
			}
		} else {
			gt = new GridToken("*", 0);
			newlaststaff->push_back(gt);
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

	p1count = (int)ice1->size();
	p2count = (int)ice2->size();
	if (p1count != p2count) {
		cerr << "Warning: Something weird happend here" << endl;
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
			v2count = (int)ice2->at(p)->at(s)->size();
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
	mslice = new GridSlice(ice2->getTimestamp(), SliceType::Manipulators);

	int z;
	HTp token;
	GridToken* gt;
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
					gt = new GridToken(token, 0);
					mslice->at(p)->at(s)->push_back(gt);
				}
			} else if (v1count < v2count) {
				// need to grow
				int grow = v2count - v1count;
				if (grow == 2 * v1count) {
					// all subspines split
					for (z=0; z<v1count; z++) {
						token = new HumdrumToken("*^");
						gt = new GridToken(token, 0);
						mslice->at(p)->at(s)->push_back(gt);
					}
				} else if (grow > 2 * v1count) {
					// too large to split all at the same time, deal with later
					for (z=0; z<v1count-1; z++) {
						token = new HumdrumToken("*^");
						gt = new GridToken(token, 0);
						mslice->at(p)->at(s)->push_back(gt);
					}
					int extra = v2count - (v1count - 1) * 2;
					token = new HumdrumToken("*^" + to_string(extra));
					gt = new GridToken(token, 0);
					mslice->at(p)->at(s)->push_back(gt);
				} else {
					// only split spines at end of list
					int doubled = v2count - v1count;
					int notdoubled = v1count - doubled;
					for (z=0; z<notdoubled; z++) {
						token = new HumdrumToken("*");
						gt = new GridToken(token, 0);
						mslice->at(p)->at(s)->push_back(gt);
					}
					for (z=0; z<doubled; z++) {
						token = new HumdrumToken("*^");
						gt = new GridToken(token, 0);
						mslice->at(p)->at(s)->push_back(gt);
					}
				}
			} else if (v1count > v2count) {
				// need to shrink
				int shrink = v1count - v2count + 1;
				int notshrink = v1count - shrink;
				for (z=0; z<notshrink; z++) {
					token = new HumdrumToken("*");
					gt = new GridToken(token, 0);
					mslice->at(p)->at(s)->push_back(gt);
				}
				for (z=0; z<shrink; z++) {
					token = new HumdrumToken("*v");
					gt = new GridToken(token, 0);
					mslice->at(p)->at(s)->push_back(gt);
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
	GridPart* part;
	GridStaff* staff;
	GridToken* gt;
	int staffcount;
	int partcount;
	int vcount;
	int nextvcount;
	int lcount;
	HTp token;
	GridSlice* endslice;
	int m;
	int p;
	int s;
	for (m=0; m<(int)this->size()-1; m++) {
		GridMeasure* measure = this->at(m);
		GridMeasure* nextmeasure = this->at(m+1);
		timestamp = nextmeasure->front()->getTimestamp();
		mslice = new GridSlice(timestamp, SliceType::Measures);
		endslice = measure->back();
		measure->push_back(mslice);
		partcount = (int)nextmeasure->front()->size();
		mslice->resize(partcount);
		for (p=0; p<partcount; p++) {
			part = new GridPart();
			mslice->at(p) = part;
			staffcount = (int)nextmeasure->front()->at(p)->size();
			mslice->at(p)->resize(staffcount);
			for (s=0; s<(int)staffcount; s++) {
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
					token = new HumdrumToken("=");
					gt = new GridToken(token, 0);
					mslice->at(p)->at(s)->push_back(gt);
				}

			}
		}
	}

   // add last measure later
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

	GridSlice* mslice = new GridSlice(timestamp, SliceType::Measures);
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
			HTp token = new HumdrumToken("=");
			GridToken* gt = new GridToken(token, 0);
			mslice->at(p)->at(s)->push_back(gt);
		}
	}
}



//////////////////////////////
//
// HumGrid::buildSingleList --
//

void HumGrid::buildSingleList(void) {
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
					GridToken& gt = *staff.at(v);
					if (gt.isNull()) {
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

	GridToken* gt = m_allslices.at(slicei)->at(parti)->at(staffi)->at(voicei);
	if (gt == NULL) {
		cerr << "Strange error: null starting token" << endl;
		return;
	}

	int lasts = slicei;
	int s = slicei+1;
	HumNum lastdur = gt->getDurationToNext();
 	HTp token = gt->getToken();
	if (*token == ".") {
		// null data token so ignore;
		return;
	}

// cerr << ">> processing " << gt->getToken() << " DUR = " << lastdur  << endl;

	if (lastdur == 0) {
		// nothing to do
		return;
	}
	GridToken* lastgt = gt;
	HumNum nextdur;
	HumNum prevdur;
	HumNum slicedur;
	while (s < (int)m_allslices.size()) {
		lastgt = getGridToken(lasts, parti, staffi, voicei);
		if (lastgt == NULL) {
			cerr << "Strange error in extendDurationToken()" << endl;
			return;
		}
		GridStaff* gs = m_allslices.at(s)->at(parti)->at(staffi);
		SliceType type = m_allslices.at(s)->getType();
		slicedur = m_allslices.at(lasts)->getDuration();
		nextdur = lastgt->getDurationToNext() - slicedur;
		if (nextdur == 0) {
			// found end of note
			break;
		}
		prevdur = lastgt->getDurationToPrev() + slicedur;
		gs->setNullTokenLayer(voicei, type, nextdur, prevdur);
		lasts = s++;
	}
}



//////////////////////////////
//
// HumGrid::getGridToken -- Check to see if GridToken exists, returns
//    NULL otherwise. Requires HumGrid::buildSingleList() being run first.
//

GridToken* HumGrid::getGridToken(int slicei, int parti, int staffi, int voicei) {
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
	GridToken* gt = gst->at(voicei);
	if (gt == NULL) {
		cerr << "Strange error 4b" << endl;
		return NULL;
	}
	return gt;
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
		}
	}
	outfile.insertLine(0, line);
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
		}
	}
	outfile.insertLine(0, line);
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
		}
	}
	outfile.insertLine(0, line);
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
		}
	}
	outfile.appendLine(line);
}


} // end namespace hum



