//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Oct 18 12:01:36 PDT 2016
// Last Modified: Tue Oct 18 12:01:41 PDT 2016
// Filename:      GridVoice.h
// URL:           https://github.com/craigsapp/hum2ly/blob/master/include/GridVoice.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   HumGrid is an intermediate container for converting from
//                MusicXML syntax into Humdrum syntax.  GridVoice is a class
//                which stores all information (notes, dynamics, lyrics, etc)
//                for a particular part (which may have more than one
//                staff.
//
//


#include "GridVoice.h"

using namespace std;


namespace hum {


//////////////////////////////
//
// GridVoice::GridVoice -- Constructor.
//

GridVoice::GridVoice(void) {
	m_token      = NULL;
	m_transfered = false;
}

GridVoice::GridVoice(HTp token, HumNum duration) {
	m_token      = token;
	m_nextdur    = duration;
	m_transfered = false;
}


GridVoice::GridVoice(const char* token, HumNum duration) {
	m_token      = new HumdrumToken(token);
	m_nextdur    = duration;
	m_transfered = false;
}


GridVoice::GridVoice(const string& token, HumNum duration) {
	m_token      = new HumdrumToken(token);
	m_nextdur    = duration;
	m_transfered = false;
}



//////////////////////////////
//
// GridVoice::~GridVoice -- Deconstructor: delete the token only if it
//     has not been transfered to a HumdrumFile object.
//

GridVoice::~GridVoice() {
	if (m_token && !m_transfered) {
		delete m_token;
	}
	m_token = NULL;
}



//////////////////////////////
//
// GridVoice::isTransfered -- True if token was copied to a HumdrumFile
//      object.
//

bool GridVoice::isTransfered(void) {
	return m_transfered;
}



//////////////////////////////
//
// GridVoice::setTransfered -- True if the object should not be
//    deleted with the object is destroyed.  False if the token
//    is not NULL and should be deleted when object is destroyed.
//

void GridVoice::setTransfered(bool state) {
	m_transfered = state;
}



//////////////////////////////
//
// GridVoice::getToken --
//

HTp GridVoice::getToken(void) const {
	return m_token;
}



//////////////////////////////
//
// GridVoice::setToken --
//

void GridVoice::setToken(HTp token) {
	if (!m_transfered && m_token) {
		delete m_token;
	}
	m_token = token;
	m_transfered = false;
}


void GridVoice::setToken(const string& token) {
	HTp realtoken = new HumdrumToken(token);
	setToken(realtoken);
}


void GridVoice::setToken(const char* token) {
	HTp realtoken = new HumdrumToken(token);
	setToken(realtoken);
}



//////////////////////////////
//
// GridVoice::isNull -- returns true if token is NULL or ".".
//

bool GridVoice::isNull(void) const {
	if (getToken() == NULL) {
		return true;
	} else if (getToken()->isNull()) {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// GridVoice::setDuration --
//

void GridVoice::setDuration(HumNum duration) {
	m_nextdur = duration;
	m_prevdur = 0;
}



//////////////////////////////
//
// GridVoice::setDurationToPrev --
//

void GridVoice::setDurationToPrev(HumNum dur) {
	m_prevdur = dur;
}



//////////////////////////////
//
// GridVoice::getDurationToNext --
//

HumNum GridVoice::getDurationToNext(void) const {
	return m_nextdur;
}



//////////////////////////////
//
// GridVoice::getDurationToPrev --
//

HumNum GridVoice::getDurationToPrev(void) const {
	return m_nextdur;
}



//////////////////////////////
//
// GridVoice::incrementDuration --
//

void GridVoice::incrementDuration(HumNum duration) {
	m_nextdur -= duration;
	m_prevdur += duration;
}



//////////////////////////////
//
// GridVoice::forgetToken -- The HumdrumToken was passed off
//      to some other object which is now responsible for
//      deleting it.
//

void GridVoice::forgetToken(void) {
	setTransfered(true);
	m_token = NULL;
}



//////////////////////////////
//
// GridVoice::getDuration -- Return the total duration of the
//   durational item, the sum of the nextdur and prevdur.
//

HumNum GridVoice::getDuration(void) const {
	return m_nextdur + m_prevdur;
}


//////////////////////////////
//
// operator<< -- print token content of a voice
//

ostream& operator<<(ostream& output, GridVoice* voice) {
	if (voice == NULL) {
		output << "{n}";
		return output;
	}

	HTp token = voice->getToken();
	if (token == NULL) {
		cout << "{n}";
	} else {
		cout << " \"" << *token << "\" ";
	}
	return output;
}

ostream& operator<<(ostream& output, GridVoice& voice) {
	output << &voice;
	return output;
}



} // end namespace hum



