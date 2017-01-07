//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Oct 16 16:08:05 PDT 2016
// Last Modified: Sun Oct 16 16:08:08 PDT 2016
// Filename:      GridMeasure.h
// URL:           https://github.com/craigsapp/hum2ly/blob/master/include/GridMeasure.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   HumGrid is an intermediate container for converting from
//                MusicXML syntax into Humdrum syntax. HumGrid is composed
//                of a vector of GridMeasures which contain the data for
//                all parts in particular MusicXML <measure>.
//
//

#ifndef _GRIDMEASURE_H
#define _GRIDMEASURE_H

#include "humlib.h"
#include "grid.h"

#include <list>

using namespace std;


namespace hum {

class GridSlice;
class HumGrid;

class GridMeasure : public list<GridSlice*> {
	public:
		GridMeasure(HumGrid* owner);
		~GridMeasure();

		bool         transferTokens (HumdrumFile& outfile, bool recip,
		                             bool addbar);
		HumGrid*     getOwner       (void);
		void         setOwner       (HumGrid* owner);
		HumNum       getDuration    (void);
		void         setDuration    (HumNum duration);
		HumNum       getTimestamp   (void);
		void         setTimestamp   (HumNum timestamp);
		HumNum       getTimeSigDur  (void);
		void         setTimeSigDur  (HumNum duration);
		MeasureStyle getStyle       (void) { return m_style; }
		MeasureStyle getBarStyle    (void) { return getStyle(); }
		void         setStyle       (MeasureStyle style) { m_style = style; }
		void         setBarStyle    (MeasureStyle style) { setStyle(style); }

		bool         isDouble(void) 
		                  {return m_style == MeasureStyle::Double;}
		bool         isFinal(void) 
		                  {return m_style == MeasureStyle::Final;}
		bool         isRepeatBackward(void) 
		                  { return m_style == MeasureStyle::RepeatBackward; }
		bool         isRepeatForward(void) 
		                  { return m_style == MeasureStyle::RepeatForward; }
		bool         isRepeatBoth(void) 
		                  { return m_style == MeasureStyle::RepeatBoth; }

	protected:
		void         appendInitialBarline(HumdrumFile& infile);

	private:
		HumGrid*     m_owner;
		HumNum       m_duration;
		HumNum       m_timestamp;
		HumNum       m_timesigdur;
		MeasureStyle m_style;
};


} // end namespace hum

#endif /* _GRIDMEASURE_H */



