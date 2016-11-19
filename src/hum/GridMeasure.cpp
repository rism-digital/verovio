//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Oct 16 16:08:05 PDT 2016
// Last Modified: Mon Oct 17 19:18:43 PDT 2016
// Filename:      GridMeasure.cpp
// URL:           https://github.com/craigsapp/hum2ly/blob/master/src/GridMeasure.cpp
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   GridMeasure stores the data for each measure in a HumGrid
//                object.
//

#include "HumGrid.h"

using namespace std;

namespace hum {


//////////////////////////////
//
// GridMeasure::GridMeasure -- Constructor.
//

GridMeasure::GridMeasure(HumGrid* owner) {
	m_owner = owner;
}



//////////////////////////////
//
// GridMeasure::~GridMeasure -- Deconstructor.
//

GridMeasure::~GridMeasure(void) {
	for (auto it = this->begin(); it != this->end(); it++) {
		if (*it) {
			delete *it;
			*it = NULL;
		}
	}
}



//////////////////////////////
//
// GridMeasure::transferTokens --
//

bool GridMeasure::transferTokens(HumdrumFile& outfile, bool recip) {

	// If the last data slice duration is zero, then calculate
	// the true duration from the duration of the measure.
	if (this->size() > 0) {
		GridSlice* slice = back();
		if (slice->isMeasureSlice() && (this->size() >= 2)) {
			auto ending = this->end();
			--ending;
			--ending;
			while ((ending != this->begin()) && (!(*ending)->isDataSlice())) {
				--ending;
			}
			slice = *ending;
		} else {
			slice = NULL;
		}
		if ((slice != NULL) && slice->isDataSlice() 
				&& (slice->getDuration() == 0)) {
			HumNum mts  = getTimestamp();
			HumNum mdur = getDuration();
			HumNum sts  = slice->getTimestamp();
			HumNum slicedur = (mts + mdur) - sts;
			slice->setDuration(slicedur);
		}
	}

	for (auto it : *this) {
		if (it->isInvalidSlice()) {
			// ignore slices to be removed from output (used for 
			// removing redundant clef slices).
			continue;
		}
		it->transferTokens(outfile, recip);
	}
	return true;
}



//////////////////////////////
//
// GridMeasure::getOwner --
//

HumGrid* GridMeasure::getOwner(void) {
	return m_owner;
}



//////////////////////////////
//
// GridMeasure::setOwner --
//

void GridMeasure::setOwner(HumGrid* owner) {
	m_owner = owner;
}



//////////////////////////////
//
// GridMeasure::setDuration --
//

void GridMeasure::setDuration(HumNum duration) {
	m_duration = duration;
}



//////////////////////////////
//
// GridMeasure::getDuration --
//

HumNum GridMeasure::getDuration(void) {
	return m_duration;
}



//////////////////////////////
//
// GridMeasure::getTimestamp --
//

HumNum GridMeasure::getTimestamp(void) {
	return m_timestamp;
}



//////////////////////////////
//
// GridMeasure::setTimestamp --
//

void GridMeasure::setTimestamp(HumNum timestamp) {
	m_timestamp = timestamp;
}




} // end namespace hum



