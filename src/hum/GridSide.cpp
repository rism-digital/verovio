//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Oct 16 16:08:05 PDT 2016
// Last Modified: Sun Oct 16 16:08:08 PDT 2016
// Filename:      GridSide.cpp
// URL:           https://github.com/craigsapp/hum2ly/blob/master/src/GridSide.cpp
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   GridSide is an intermediate container for converting from
//                MusicXML syntax into Humdrum syntax.
//
//

#include "HumGrid.h"

using namespace std;

namespace hum {


//////////////////////////////
//
// GridSide::GridSide -- Constructor.
//

GridSide::GridSide(void) {
	m_harmony = NULL;
}



//////////////////////////////
//
// GridSide::~GridSide -- Deconstructor.
//

GridSide::~GridSide(void) {

	for (int i=0; i<(int)m_verses.size(); i++) {
		if (m_verses[i]) {
			delete m_verses[i];
			m_verses[i] = NULL;
		}
	}
	m_verses.resize(0);

	for (int i=0; i<(int)m_dynamics.size(); i++) {
		if (m_dynamics[i]) {
			delete m_dynamics[i];
			m_dynamics[i] = NULL;
		}
	}
	m_dynamics.resize(0);

	if (m_harmony) {
		delete m_harmony;
		m_harmony = NULL;
	}
}



//////////////////////////////
//
// GridSide::setVerse --
//

void GridSide::setVerse(int index, HTp token) {
   if (index == (int)m_verses.size()) {
		m_verses.push_back(token);
		return;
	} else if (index < 0) {
		return;
	} else if (index < (int)m_verses.size()) {
		m_verses[index] = token;
	} else {
		int oldsize = (int)m_verses.size();
		int newsize = index + 1;
		m_verses.resize(newsize);
		for (int i=oldsize; i<newsize; i++) {
			m_verses[i] = NULL;
		}
		m_verses[index] = token;
	}
}



//////////////////////////////
//
// GridSide::getVerse --
//

HTp GridSide::getVerse(int index) {
	if (index < 0 || index >= getVerseCount()) {
		return NULL;
	}
	return m_verses[index];
}



//////////////////////////////
//
// GridSide::getVerseCount --
//

int GridSide::getVerseCount(void) {
 	return (int)m_verses.size();
}



//////////////////////////////
//
// GridSide::getHarmonyCount --
//

int GridSide::getHarmonyCount(void) { 
	if (m_harmony == NULL) {
		return 0;
	} else {
		return 1;
	}
}



//////////////////////////////
//
// GridSide::setHarmony --
//

void GridSide::setHarmony(HTp token) { 
	if (m_harmony) {
		delete m_harmony;
		m_harmony = NULL;
	}
	m_harmony = token;
}



///////////////////////////
//
// GridSide::detachHarmony --
//

void GridSide::detachHarmony(void) { 
	m_harmony = NULL;
}



//////////////////////////////
//
// GridSide::getHarmony --
//

HTp GridSide::getHarmony(void) { 
	return m_harmony;
}


} // end namespace hum



