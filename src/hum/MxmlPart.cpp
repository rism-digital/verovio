//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Aug  6 10:53:40 CEST 2016
// Last Modified: Sun Sep 18 14:16:18 PDT 2016
// Filename:      MxmlPart.cpp
// URL:           https://github.com/craigsapp/musicxml2hum/blob/master/src/MxmlPart.cpp
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   MusicXML parsing abstraction for part elements which
//                contain a list of measures.
//
// part element documentation:
//    http://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-part.htm
//

#include "humlib.h"

#include "MxmlMeasure.h"
#include "MxmlPart.h"

#include "pugiconfig.hpp"
#include "pugixml.hpp"

#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include <map>

using namespace pugi;
using namespace std;


namespace hum {

class MxmlMeasure;
class MxmlPart;


//////////////////////////////
//
// MxmlPart::MxmlPart -- Constructor.
//

MxmlPart::MxmlPart(void) {
	clear();
}



//////////////////////////////
//
// MxmlPart::~MxmlPart -- Deconstructor.
//

MxmlPart::~MxmlPart(void) {
	clear();
}



//////////////////////////////
//
// MxmlPart::clear -- Clear all internal variables of object.
//

void MxmlPart::clear(void) {
	for (int i=0; i<(int)m_measures.size(); i++) {
		delete m_measures[i];
		m_measures[i] = NULL;
	}
	m_measures.clear();
	m_partnum = 0;
	m_maxstaff = 0;
	m_verseCount.resize(0);
	m_harmonyCount = 0;
}



//////////////////////////////
//
// MxmlPart::getQTicks -- Return the current divisions element value,
//    which are the number of integer ticks representing a quarter-note
//    duration.
//

long MxmlPart::getQTicks(void) const {
	if (m_qtick.size() > 0) {
		return m_qtick.back();
	} else {
		return 0;
	}
}



//////////////////////////////
//
// MxmlPart::setQTicks -- Set the current attribute/divisions value,
//     which is the number of integer ticks representing a quarter-note
//     duration.
//

int MxmlPart::setQTicks(long value) {
	if (value < 0) {
		return (int)m_qtick.size();
	}
	if (m_qtick.size() > 0) {
		if (m_qtick.back() == value) {
			return (int)m_qtick.size();
		}
	}
	m_qtick.push_back(value);
	return (int)m_qtick.size();
}



//////////////////////////////
//
// MxmlPart::addMeasure -- Append a new measure to the list of measure element.
//

bool MxmlPart::addMeasure(xpath_node mel) {
	return addMeasure(mel.node());
}


bool MxmlPart::addMeasure(xml_node mel) {
	MxmlMeasure* meas = new MxmlMeasure(this);
	if (m_measures.size() > 0) {
		meas->setPreviousMeasure(m_measures.back());
		m_measures.back()->setNextMeasure(meas);
	}
	m_measures.push_back(meas);
	int status = meas->parseMeasure(mel);

	return status;
}



//////////////////////////////
//
// MxmlPart::getMeasureCount -- Return the number of stored measures.
//

int MxmlPart::getMeasureCount(void) const {
	return (int)m_measures.size();
}



//////////////////////////////
//
// MxmlPart::getMeasure -- Get the measure number at the given index.
//

MxmlMeasure* MxmlPart::getMeasure(int index) const {
	if ((index < 0) || (index >= (int)m_measures.size())) {
		return NULL;
	}
	return m_measures[index];
}



//////////////////////////////
//
// MxmlPart::getPreviousMeasure -- Given a measure, return the
//    previous measure occuring before it.
//

MxmlMeasure* MxmlPart::getPreviousMeasure(MxmlMeasure* measure) const {
	if (!measure) {
		return NULL;
	}
	if (measure == *m_measures.begin()) {
		return NULL;
	}
	if (m_measures.size() == 0) {
		return NULL;
	}

	return measure->getPreviousMeasure();
}



//////////////////////////////
//
// MxmlPart::getDuration --  Return the duration of the part in units
//     of quarter notes.  This is a sum of the duration of all measures in
//     the part.
//

HumNum MxmlPart::getDuration(void) const {
	if (m_measures.size() == 0) {
		return 0;
	}
	return m_measures.back()->getStartTime() + m_measures.back()->getDuration();
}



//////////////////////////////
//
// MxmlPart::setPartNumber -- Set the part number for the part.  Typically
//   starts at "1" for the top part in a system.
//

void MxmlPart::setPartNumber(int number) {
	m_partnum = number;
}



//////////////////////////////
//
// MxmlPart::getPartNumber -- Return the part number for the part.  Typically
//     starts at "1" for the top part in a system.
//

int MxmlPart::getPartNumber(void) const {
	return m_partnum;
}



//////////////////////////////
//
// MxmlPart::getPartIndex -- Return the part number for the part.  Typically
//     starts at "0" for the top part in a system.
//

int MxmlPart::getPartIndex(void) const {
	return m_partnum - 1;
}



//////////////////////////////
//
// MxmlPart::getStaffCount -- Return the number of staves which the part
//   contains, such as 2 for piano parts.
//

int MxmlPart::getStaffCount(void) const {
	if (!m_maxstaff) {
		return 1;
	} else {
		return m_maxstaff;
	}
}



//////////////////////////////
//
// MxmlPart::getHarmonyCount -- Return the number of verses in the part.
//

int MxmlPart::getHarmonyCount(void) const {
	return m_harmonyCount;
}



//////////////////////////////
//
// MxmlPart::getVerseCount -- Return the number of verses in the part.
//

int MxmlPart::getVerseCount(void) const {
	if (m_verseCount.size() == 0) {
		return 0;
	} else {
		return m_verseCount[0];
	}
}


int MxmlPart::getVerseCount(int staffindex) const {
	int staffnum = staffindex + 1;
	if (staffnum < (int)m_verseCount.size()) {
		return m_verseCount[staffnum];
	} else {
		return 0;
	}
}



//////////////////////////////
//
// MxmlPart::receiveHarmonyCount --
//

void MxmlPart::receiveHarmonyCount(int count) {
	m_harmonyCount = count;
}



//////////////////////////////
//
// MxmlPart::receiveVerseCount --
//

void MxmlPart::receiveVerseCount(int count) {
	receiveVerseCount(0, count);
}


void MxmlPart::receiveVerseCount(int staffindex, int count) {
	int staffnum = staffindex + 1;
	if (staffnum < 0) {
		return;
	}
	if (staffnum < (int)m_verseCount.size()) {
		if (count > m_verseCount[staffnum]) {
			m_verseCount[staffnum] = count;
		}
	} else {
		int oldsize = (int)m_verseCount.size();
		int newsize = staffnum + 1;
		m_verseCount.resize(newsize);
		for (int i=oldsize; i<newsize; i++) {
			m_verseCount[i] = 0;
		}
		m_verseCount[staffnum] = count;
	}
}



///////////////////////////////////////////////////////////////////////////
//
// private fuctions --
//

//////////////////////////////
//
// MxmlMeasure::receiveStaffNumberFromChild -- Receive a staff number
//    placement for a note or rest and pass it along to the part class
//    so that it can keep track of the maximum staff number used in
//    the part.
//

void MxmlPart::receiveStaffNumberFromChild(int staffnum, int voicenum) {
	if (m_maxstaff < staffnum) {
		m_maxstaff = staffnum;
	}
	trackStaffVoices(staffnum, voicenum);
}



//////////////////////////////
//
// MxmlPart::trackStaffVoices -- Keep track of which staff voices
//     occur on.  This will be used later to assign voices to
//     spines, and to make notes in the voice which are not on
//     the home staff (cross-staff beaming, etc).
//

void MxmlPart::trackStaffVoices(int staffnum, int voicenum) {
	vector<vector<int> >& sv = m_staffvoicehist;
	if (staffnum < 0) {
		return;
	}
	if (voicenum < 0) {
		return;
	}
	if (staffnum >= (int)sv.size()) {
		sv.resize(staffnum+1);
	}
	if (voicenum >= (int)sv[staffnum].size()) {
		int oldsize = (int)sv[staffnum].size();
		int newsize = voicenum + 1;
		sv[staffnum].resize(newsize);
		for (int i=oldsize; i<newsize; i++) {
			sv[staffnum][i] = 0;		
		}
	}
	sv[staffnum][voicenum]++;
}


//////////////////////////////
//
// MxmlPart::prepareVoiceIndex -- Takes the histogram of staff/voice
//    pairings and create a list of new voice indexes for each
//    staff.  In Finale & Sibelius, four voices are hardwired to each
//    staff: staff1 {1, 2, 3, 4}, staff2 {5, 6, 7, 8}.  But some
//    software will not use this, instead: staff1 {1}, staff2 {2}.
//    The m_voicemapping variable will re-index voice numbers independently
//    for each staff:
//       staff1 {1, 2, 3, 4}, staff2 {5, 6, 7, 8}
//       staff1 {0, 1, 2, 3}, staff2 {0, 1, 2, 3}
//    and:
//       staff1 {1}, staff2 {2}
//       staff1 {0}, staff2 {0}
//    strange cases such as this should also work:
//       staff1 {1, 3, 5, 7}, staff2 {2, 4, 6, 8}
//       staff1 {0, 1, 2, 3}, staff2 {0, 1, 2, 3}
//    A voice is assigned to the most common staff on which its note/rests
//    occur.
//
//    voicenum in MusicXML is mapped to a (staffindex, voiceindex) pair
//       vector<pair<int, int> > m_voicemapping;
//
// Example mapping process:
// First, start with a histogram of staff/voice numbers in MusicXML file:
// 	STAFF 0:	55
// 	STAFF 1:	0	98
// 	STAFF 2:	39	0	41
// In this case staff1 has a single voice numbered "1" (with 98 counts)
// And staff2 has a single voice, numbered "2".  The final mapping
// in m_voicemapping is:
// 	0 (voice number 1) => staffindex 0, voiceindex 0
// 	1 (voice number 2) => staffindex 1, voiceindex 0
// staff0 and voice0 assignments are ignored, since there are not
// for notes (usually measures which are on staff0/voice0, and
// non-notes such as harmony which will be attached to a staff with
// but voice0, but ignored at least for now.
//

void MxmlPart::prepareVoiceMapping(void) {
	vector<vector<int> >& sv = m_staffvoicehist;
	int staffcount = (int)sv.size() - 1;
	if (staffcount < 1) {
		return;
	}
	int i, j;
	int maxvoicenum = 0;
	// a staff without any voices will probably cause problems,
	// so maybe check for such a case. 0th position in sv is
	// not used, so maxvoicenum is an index for sv.

	for (i=1; i<(int)sv.size(); i++) {
		if ((int)sv[i].size() - 1 > maxvoicenum) {
			maxvoicenum = sv[i].size() - 1;
		}
	}

	// reindex voice numbers to voice indexes on staves:
	// m_voicemapping[homevoicenum] => {homestaffindex, newvoiceindex}
	pair<int, int> empty;
	empty.first = -1;
	empty.second = -1;
	int homestaffnum;
	int homevoicenum;
	int newvoiceindex;
	int count;
	int maxcount;

	// for each voice number in the MusicXML data, assign
	// a voiceindex for it on each staff.
	for (j=1; j<=maxvoicenum; j++) {
		maxcount = -1;
		homestaffnum = -1;
		for (i=1; i<(int)sv.size(); i++) {
			if (j >= (int)sv[i].size()) {
				continue;
			}
			count = sv[i][j];
			if ((count > 0) && (maxcount < count)) {
				maxcount = count;
				homestaffnum = i;
				homevoicenum = j;
			}
		}
		if (homestaffnum < 1) {
			continue;
		}
		if (homevoicenum < 1) {
			continue;
		}

		// find highest newvoiceindex for the current staff
		newvoiceindex = -1;
		for (int n=1; n<(int)m_voicemapping.size(); n++) {
			if (m_voicemapping[n].first == homestaffnum - 1) {
				newvoiceindex++;
			}
		}
		// assign to next highest newvoiceindex for staff:
		newvoiceindex++;

		// add the new mapping for homevoicenum to (staffindex, newvoiceindex)
		if (homevoicenum >= (int)m_voicemapping.size()) {
			int oldsize = (int)m_voicemapping.size();
			int newsize = homevoicenum + 1;
			m_voicemapping.resize(newsize);
			for (int m=oldsize; m<newsize; m++) {
				m_voicemapping[m] = empty;
			}
			m_voicemapping[homevoicenum].first = homestaffnum - 1;
			m_voicemapping[homevoicenum].second = newvoiceindex;
		} else {
			m_voicemapping[homevoicenum].first = homestaffnum - 1;
			m_voicemapping[homevoicenum].second = newvoiceindex;
		}
	}
}



//////////////////////////////
//
// MxmlPart::getVoiceIndex -- Convert a MusicXML voice number to
//    a voice index on a particular staff.
//

int MxmlPart::getVoiceIndex(int voicenum) {
	if (voicenum < 1) {
		return -1;
	}
	if (voicenum >= (int)m_voicemapping.size()) {
		return -1;
	}
	return m_voicemapping[voicenum].second;
}



//////////////////////////////
//
// MxmlPart::getStaffIndex -- Convert a MusicXML voice number to
//    a voice index on a particular staff.
//

int MxmlPart::getStaffIndex(int voicenum) {
	if (voicenum < 1) {
		return -1;
	}
	if (voicenum >= (int)m_voicemapping.size()) {
		return -1;
	}
	return m_voicemapping[voicenum].first;
}



//////////////////////////////
//
// MxmlPart::printStaffVoiceInfo --
//

void MxmlPart::printStaffVoiceInfo(void) {
	vector<vector<int> >& sv = m_staffvoicehist;
	int i, j;
	cout << "\n!!STAFF-VOICE MAPPING:\n";
	for (i=0; i<(int)sv.size(); i++) {
		cout << "!!\tSTAFF " << i << ":";
		for (j=0; j<(int)sv[i].size(); j++) {
			cout << "\t" << sv[i][j];
		}
		cout << endl;
	}
	cout << "!!REMAPPING:\n";
	for (i=1; i<(int)m_voicemapping.size(); i++) {
		cout << "!!\tvoicenum " << i << ":\t(";
		cout << m_voicemapping[i].first << ", ";
		cout << m_voicemapping[i].second << ")\n";
	}
	cout << endl;
}


} // end namespace hum

