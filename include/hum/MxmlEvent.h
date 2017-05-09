//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Aug  6 10:53:40 CEST 2016
// Last Modified: Sun Sep 18 14:16:18 PDT 2016
// Filename:      MxmlEvent.cpp
// URL:           https://github.com/craigsapp/musicxml2hum/blob/master/include/MxmlEvent.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   MusicXML parsing abstraction for elements which are children
//                of the measure element.
//

#ifndef _MXMLEVENT_H
#define _MXMLEVENT_H

#include "humlib.h"
#include "grid.h"

#include "pugiconfig.hpp"
#include "pugixml.hpp"

#include <sstream>

using namespace pugi;
using namespace std;

namespace hum {

class MxmlMeasure;
class MxmlPart;

// Event types: These are all of the XML elements which can be children of
// the measure element in MusicXML.

enum measure_event_type {
	mevent_unknown,
	mevent_attributes,
	mevent_backup,
	mevent_barline,
	mevent_bookmark,
	mevent_direction,
	mevent_figured_bass,
	mevent_forward,
	mevent_grouping,
	mevent_harmony,
	mevent_link,
	mevent_note,
	mevent_print,
	mevent_sound,
	mevent_float       // category for GridSides not attached to note onsets
};


class MxmlEvent {
	public:
		                   MxmlEvent          (MxmlMeasure* measure);
		                  ~MxmlEvent          ();
		void               clear              (void);
		bool               parseEvent         (xml_node el, xml_node nextel,
		                                       HumNum starttime);
		bool               parseEvent         (xpath_node el, HumNum starttime);
		void               setTickStart       (long value, long ticks);
		void               setTickDur         (long value, long ticks);
		void               setStartTime       (HumNum value);
		void               setDuration        (HumNum value);
		void               setDurationByTicks (long value,
		                                       xml_node el = xml_node(NULL));
		HumNum             getStartTime       (void) const;
		HumNum             getDuration        (void);
		void               setOwner           (MxmlMeasure* measure);
		MxmlMeasure*       getOwner           (void) const;
		const char*        getName            (void) const;
		int                setQTicks          (long value);
		long               getQTicks          (void) const;
		long               getIntValue        (const char* query) const;
		bool               hasChild           (const char* query) const;
		void               link               (MxmlEvent* event);
		bool               isLinked           (void) const;
		bool               isRest             (void);
		bool               isGrace            (void);
		bool               isFloating         (void);
		bool               hasSlurStart       (int& direction);
		bool               hasSlurStop        (void);
		void               setLinked          (void);
		vector<MxmlEvent*> getLinkedNotes     (void);
		void               attachToLastEvent  (void);
		bool               isChord            (void) const;
		void               printEvent         (void);
		int                getSequenceNumber  (void) const;
		int                getVoiceNumber     (void) const;
		void               setVoiceNumber     (int value);
		int                getStaffNumber     (void) const;
		int                getStaffIndex      (void) const;
		int                getVoiceIndex      (int maxvoice = 4) const;
		void               setStaffNumber     (int value);
		measure_event_type getType            (void) const;
		int                getPartNumber      (void) const;
		int                getPartIndex       (void) const;
		string             getRecip           (void) const;
		string             getKernPitch       (void);
		string             getPrefixNoteInfo  (void) const;
		string             getPostfixNoteInfo (bool primarynote) const;
		xml_node           getNode            (void);
		xml_node           getHNode           (void);
		HumNum             getTimeSigDur      (void);
		string             getElementName     (void);
		void               addNotations       (stringstream& ss, 
		                                       xml_node notations) const;
		void               reportVerseCountToOwner (int count);
		void               reportVerseCountToOwner (int staffnum, int count);
		void               reportHarmonyCountToOwner (int count);
		void               reportMeasureStyleToOwner (MeasureStyle style);
      void               makeDummyRest      (MxmlMeasure* owner, 
		                                       HumNum startime,
		                                       HumNum duration,
		                                       int staffindex = 0,
		                                       int voiceindex = 0);
		void               setVoiceIndex      (int voiceindex);
		void               forceInvisible     (void);
		bool               isInvisible        (void);
		void               setBarlineStyle    (xml_node node);

	protected:
		HumNum             m_starttime;  // start time in quarter notes of event
		HumNum             m_duration;   // duration in quarter notes of event
		measure_event_type m_eventtype;  // enumeration type of event
		xml_node           m_node;       // pointer to event in XML structure
		MxmlMeasure*       m_owner;      // measure that contains this event
		vector<MxmlEvent*> m_links;      // list of secondary chord notes
		bool               m_linked;     // true if a secondary chord note
		int                m_sequence;   // ordering of event in XML file
		static int         m_counter;    // counter for sequence variable
		short              m_staff;      // staff number in part for event
		short              m_voice;      // voice number in part for event
		int                m_voiceindex; // voice index of item (remapping)
      int                m_maxstaff;   // maximum staff number for measure
		xml_node           m_hnode;      // harmony label starting at note event
		bool               m_invisible;  // for forceInvisible();


	private:
   	void   reportStaffNumberToOwner  (int staffnum, int voicenum);
		void   reportTimeSigDurToOwner   (HumNum duration);
		int    getDotCount               (void) const;

	public:
		static HumNum getEmbeddedDuration  (xml_node el = xml_node(NULL));
		static HumNum getQuarterDurationFromType (const char* type);
		static bool   nodeType             (xml_node node, const char* testname);

	friend MxmlMeasure;
	friend MxmlPart;
};



} // end namespace hum

#endif /* _MXMLEVENT_H */



