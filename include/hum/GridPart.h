//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Oct 16 16:08:05 PDT 2016
// Last Modified: Sun Oct 16 16:08:08 PDT 2016
// Filename:      GridPart.h
// URL:           https://github.com/craigsapp/hum2ly/blob/master/include/GridPart.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   HumGrid is an intermediate container for converting from
//                MusicXML syntax into Humdrum syntax.  GridPart is a class
//                which stores all information (notes, dynamics, lyrics, etc)
//                for a particular part (which may have more than one
//                staff.
//
//

#ifndef _GRIDPART_H
#define _GRIDPART_H

#include "humlib.h"
#include "GridStaff.h"

#include <vector>

using namespace std;


namespace hum {


class GridPart : public vector<GridStaff*>, public GridSide {
	public:
		GridPart(void);
		~GridPart();
};

ostream& operator<<(ostream& output, GridPart* part);
ostream& operator<<(ostream& output, GridPart& part);


} // end namespace hum

#endif /* _GRIDPART_H */



