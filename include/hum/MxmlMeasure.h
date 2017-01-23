//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Aug  6 10:53:40 CEST 2016
// Last Modified: Sun Sep 18 14:16:18 PDT 2016
// Filename:      MxmlMeasure.cpp
// URL:           https://github.com/craigsapp/musicxml2hum/blob/master/include/MxmlMeasure.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   MusicXML parsing abstraction for measure elements which
//                are children of the part element.
//
// measure element documentation:
//    http://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-measure.htm
//

#ifndef _MXMLMEASURE_H
#define _MXMLMEASURE_H

#include "humlib.h"
#include "grid.h"

#include "pugiconfig.hpp"
#include "pugixml.hpp"

#include <vector>

using namespace pugi;
using namespace std;


namespace hum {


class MxmlEvent;
class MxmlPart;


class SimultaneousEvents {
	public:
		SimultaneousEvents(void) { }
		~SimultaneousEvents() { }
		HumNum starttime;              // start time of events
		HumNum duration;               // duration to next non-zero duration
		vector<MxmlEvent*> zerodur;    // zero-duration elements at this time
		vector<MxmlEvent*> nonzerodur; // non-zero dur elements at this time
};


class MxmlMeasure {
	public:
		              MxmlMeasure        (MxmlPart* part);
		             ~MxmlMeasure        (void);
		void          clear              (void);
		bool          parseMeasure       (xml_node mel);
		bool          parseMeasure       (xpath_node mel);
		void          setStartTimeOfMeasure (HumNum value);
		void          setStartTimeOfMeasure (void);
		void          setDuration        (HumNum value);
		HumNum        getStartTime       (void) const;
		HumNum        getTimestamp       (void) const { return getStartTime(); }
		HumNum        getDuration        (void) const;
		void          setOwner           (MxmlPart* part);
		MxmlPart*     getOwner           (void) const;
		int           getPartNumber      (void) const;
		int           getPartIndex       (void) const;
		int           setQTicks          (long value);
		long          getQTicks          (void) const;
		void          attachLastEventToPrevious  (void);
		void          calculateDuration  (void);
		int           getEventCount      (void) const;
		vector<SimultaneousEvents>* getSortedEvents(void);
		MxmlEvent*    getEvent           (int index) const;

		void          setNextMeasure     (MxmlMeasure* event);
		MxmlMeasure*  getNextMeasure     (void) const;
		MxmlMeasure*  getPreviousMeasure (void) const;
		void          setPreviousMeasure (MxmlMeasure* event);

		int           getVoiceIndex      (int voicenum);
		int           getStaffIndex      (int voicenum);
		void          setTimeSigDur      (HumNum duration);
		HumNum        getTimeSigDur      (void);
		void          addDummyRest       (void);
		void          addDummyRest       (HumNum starttime, HumNum duration, 
		                                  int staffindex, int voiceindex);
		vector<MxmlEvent*>& getEventList (void);
		void  sortEvents                 (void);
		void  forceLastInvisible         (void);
		MeasureStyle  getStyle           (void);
		MeasureStyle  getBarStyle        (void);
		void  setStyle                   (MeasureStyle style);
		void  setBarStyle                (MeasureStyle style);
		void  makeFinalBarline(void)   { m_style = MeasureStyle::Final; }
		bool  isFinal(void)            { return m_style == MeasureStyle::Final; }
		bool  isDouble(void)           { return m_style == MeasureStyle::Double; }
		bool  isRepeatBackward(void)   { return m_style == MeasureStyle::RepeatBackward; }
		bool  isRepeatForward(void)    { return m_style == MeasureStyle::RepeatForward; }
		bool  isRepeatBoth(void)       { return m_style == MeasureStyle::RepeatBoth; }

	private:
		void  receiveStaffNumberFromChild (int staffnum, int voicenum);
		void  receiveTimeSigDurFromChild  (HumNum duration);
		void  receiveMeasureStyleFromChild(MeasureStyle style);
   	void  reportStaffNumberToOwner    (int staffnum, int voicenum);
		void  reportVerseCountToOwner     (int count);
		void  reportVerseCountToOwner     (int staffindex, int count);
		void  reportHarmonyCountToOwner   (int count);

	protected:
		HumNum             m_starttime; // start time of measure in quarter notes
		HumNum             m_duration;  // duration of measure in quarter notes
		HumNum             m_timesigdur; // duration of measure according to 
													// prevailing time signature.
		MxmlPart*          m_owner;     // part which contains measure
		MxmlMeasure*       m_previous;  // previous measure in part or null
		MxmlMeasure*       m_following; // following measure in part or null
		vector<MxmlEvent*> m_events;    // list of semi-ordered events in measure
		vector<SimultaneousEvents> m_sortedevents; // list of time-sorted events
		MeasureStyle       m_style;     // measure style type

	friend MxmlEvent;
	friend MxmlPart;

};


} // end namespace hum

#endif /* _MXMLMEASURE_H */



