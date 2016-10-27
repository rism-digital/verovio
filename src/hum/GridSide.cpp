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
	// do nothing;
}

//////////////////////////////
//
// GridSide::~GridSide -- Deconstructor.
//

GridSide::~GridSide(void) {
	// do nothing: for the moment;
	// all HumdrumTokens* stored in this
	// structure should be transferred into
	// a HumdrumFile structure.
}


} // end namespace hum



