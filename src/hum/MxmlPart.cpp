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

void MxmlPart::receiveStaffNumberFromChild(int staffnum) {
	if (m_maxstaff < staffnum) {
		m_maxstaff = staffnum;
	}
}


} // end namespace hum



