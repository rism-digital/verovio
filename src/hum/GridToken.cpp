//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Oct 18 12:01:36 PDT 2016
// Last Modified: Tue Oct 18 12:01:41 PDT 2016
// Filename:      GridToken.h
// URL:           https://github.com/craigsapp/hum2ly/blob/master/include/GridToken.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   HumGrid is an intermediate container for converting from
//                MusicXML syntax into Humdrum syntax.  GridToken is a class
//                which stores all information (notes, dynamics, lyrics, etc)
//                for a particular part (which may have more than one
//                staff.
//
//


#include "GridToken.h"

using namespace std;


namespace hum {


//////////////////////////////
//
// GridToken::GridToken -- Constructor.
//

GridToken::GridToken(void) {
	m_token      = NULL;
	m_transfered = false;
}

GridToken::GridToken(HTp token, HumNum duration) {
	m_token      = token;
	m_nextdur    = duration;
	m_transfered = false;
}


GridToken::GridToken(const char* token, HumNum duration) {
	m_token      = new HumdrumToken(token);
	m_nextdur    = duration;
	m_transfered = false;
}


GridToken::GridToken(const string& token, HumNum duration) {
	m_token      = new HumdrumToken(token);
	m_nextdur    = duration;
	m_transfered = false;
}



//////////////////////////////
//
// GridToken::~GridToken -- Deconstructor: delete the token only if it
//     has not been transfered to a HumdrumFile object.
//

GridToken::~GridToken() {
	if (m_token && !m_transfered) {
		delete m_token;
	}
	m_token = NULL;
}



//////////////////////////////
//
// GridToken::isTransfered -- True if token was copied to a HumdrumFile
//      object.
//

bool GridToken::isTransfered(void) { 
	return m_transfered;
}



//////////////////////////////
//
// GridToken::setTransfered -- True if the object should not be
//    deleted with the object is destroyed.  False if the token
//    is not NULL and should be deleted when object is destroyed.
//

void GridToken::setTransfered(bool state) {
	m_transfered = state;
}



//////////////////////////////
//
// GridToken::getToken --
//

HTp GridToken::getToken(void) const {
	return m_token;
}



//////////////////////////////
//
// GridToken::setToken --
//

void GridToken::setToken(HTp token) {
	if (!m_transfered && m_token) {
		delete m_token;
	}
	m_token = token;
	m_transfered = false;
}


void GridToken::setToken(const string& token) {
	HTp realtoken = new HumdrumToken(token);
	setToken(realtoken);
}


void GridToken::setToken(const char* token) {
	HTp realtoken = new HumdrumToken(token);
	setToken(realtoken);
}



//////////////////////////////
//
// GridToken::isNull -- returns true if token is NULL or ".".
//

bool GridToken::isNull(void) const {
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
// GridToken::setDuration --
//

void GridToken::setDuration(HumNum duration) {
	m_nextdur = duration;
	m_prevdur = 0;
}



//////////////////////////////
//
// GridToken::setDurationToPrev --
//

void GridToken::setDurationToPrev(HumNum dur) {
	m_prevdur = dur;
}



//////////////////////////////
//
// GridToken::getDurationToNext --
//

HumNum GridToken::getDurationToNext(void) const {
	return m_nextdur;
}



//////////////////////////////
//
// GridToken::getDurationToPrev --
//

HumNum GridToken::getDurationToPrev(void) const {
	return m_nextdur;
}



//////////////////////////////
//
// GridToken::incrementDuration -- 
//

void GridToken::incrementDuration(HumNum duration) {
	m_nextdur -= duration;
	m_prevdur += duration;
}



//////////////////////////////
//
// GridToken::forgetToken -- The HumdrumToken was passed off
//      to some other object which is now responsible for 
//      deleting it.
//

void GridToken::forgetToken(void) {
	setTransfered(true);
	m_token = NULL;
}



//////////////////////////////
//
// GridToken::getDuration -- Return the total duration of the
//   durational item, the sum of the nextdur and prevdur.
//

HumNum GridToken::getDuration(void) const {
	return m_nextdur + m_prevdur;
}



} // end namespace hum



