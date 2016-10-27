//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Aug  6 10:53:40 CEST 2016
// Last Modified: Sun Sep 18 14:16:18 PDT 2016
// Filename:      MxmlPart.cpp
// URL:           https://github.com/craigsapp/hum2ly/blob/master/include/MxmlPart.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   MusicXML parsing abstraction for part elements which
//                contain a list of measures.
//
// part element documentation:
//    http://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-part.htm
//

#ifndef _MXMLPART_H
#define _MXMLPART_H

#include "humlib.h"

#include "MxmlMeasure.h"

#include "pugiconfig.hpp"
#include "pugixml.hpp"

#include <vector>

using namespace pugi;
using namespace std;


namespace hum {

class MxmlMeasure;
class MxmlEvent;

class MxmlPart {
	public:
		              MxmlPart             (void);
		             ~MxmlPart             ();
		void          clear                (void);
		bool          readPart             (const string& id, xml_node partdef, 
		                                    xml_node part);
		bool          addMeasure           (xml_node mel);
		bool          addMeasure           (xpath_node mel);
		int           getMeasureCount      (void) const;
		MxmlMeasure*  getMeasure           (int index) const;
		long          getQTicks            (void) const;
		int           setQTicks            (long value);
	   MxmlMeasure*  getPreviousMeasure   (MxmlMeasure* measure) const;
		HumNum        getDuration          (void) const;
		void          allocateSortedEvents (void);
		void          setPartNumber        (int number);
		int           getPartNumber        (void) const;
		int           getPartIndex         (void) const;
		int           getStaffCount        (void) const;
		int           getVerseCount        (void) const;

	private:
		void          receiveStaffNumberFromChild (int staffnum);

	protected:
		vector<MxmlMeasure*> m_measures;
		vector<long>         m_qtick;
		int                  m_partnum;
		int                  m_maxstaff;
		
	friend MxmlMeasure;
	friend MxmlEvent;

};


} // end namespace hum

#endif /* _MXMLPART_H */



