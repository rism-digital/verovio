//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Oct 16 16:08:05 PDT 2016
// Last Modified: Sun Oct 16 16:08:08 PDT 2016
// Filename:      GridPart.cpp
// URL:           https://github.com/craigsapp/hum2ly/blob/master/src/GridPart.cpp
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   GridPart is an intermediate container for converting from
//                MusicXML syntax into Humdrum syntax.
//
//

#include "GridPart.h"

using namespace std;

namespace hum {


//////////////////////////////
//
// GridPart::GridPart -- Constructor.
//

GridPart::GridPart(void) {
	// do nothing;
}

//////////////////////////////
//
// GridPart::~GridPart -- Deconstructor: delete any GridStaff items
//     being stored.
//

GridPart::~GridPart(void) {
	for (int i=0; i<(int)this->size(); i++) {
		if (this->at(i)) {
			delete this->at(i);
			this->at(i) = NULL;
		}
	}
}


} // end namespace hum



