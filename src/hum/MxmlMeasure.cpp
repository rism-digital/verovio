// vim: ts=3

// measure element:
// http://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-measure.htm

#include "humlib.h"

#include "MxmlEvent.h"
#include "MxmlMeasure.h"
#include "MxmlPart.h"

#include "pugiconfig.hpp"
#include "pugixml.hpp"

#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace pugi;
using namespace std;

namespace hum {

class MxmlPart;


//////////////////////////////
//
// MxmlMeasure::MxmlMeasure --
//

MxmlMeasure::MxmlMeasure(MxmlPart* part) {
	clear();
	setOwner(part);
}



//////////////////////////////
//
// MxmlMeasure::~MxmlMeasure --
//

MxmlMeasure::~MxmlMeasure() {
	clear();
}



//////////////////////////////
//
// MxmlMeasure::clear --
//

void MxmlMeasure::clear(void) {
	m_starttime = m_duration = 0;
	for (int i=0; i<(int)m_events.size(); i++) {
		delete m_events[i];
		m_events[i] = NULL;
	}
	m_events.clear();
	m_owner = NULL;
	m_timesigdur = -1;
	m_previous = m_following = NULL;
	m_style = MeasureStyle::Plain;
}



//////////////////////////////
//
// MxmlMeasure::parseMeasure -- Reads XML data for one part's measure.
//

bool MxmlMeasure::parseMeasure(xpath_node mel) {
	return parseMeasure(mel.node());
}


bool MxmlMeasure::parseMeasure(xml_node mel) {
	bool output = true;
	vector<vector<int> > staffVoiceCounts;
	setStartTimeOfMeasure();

	HumNum starttime = getStartTime();
	HumNum st   = starttime;
	HumNum maxst = starttime;

	xml_node nextel;
	for (auto el = mel.first_child(); el; el = el.next_sibling()) {
		MxmlEvent* event = new MxmlEvent(this);
		m_events.push_back(event);
		nextel = el.next_sibling();
		output &= event->parseEvent(el, nextel, starttime);
		starttime += event->getDuration();
		if (starttime > maxst) {
			maxst = starttime;
		}
	}
	setDuration(maxst - st);

	// Should no longer be needed:
	// calculateDuration();

   bool needdummy = false;

   MxmlMeasure* pmeasure = getPreviousMeasure();
   if (getTimeSigDur() <= 0) {
      if (pmeasure) {
         setTimeSigDur(pmeasure->getTimeSigDur());
      }
   }

   if (getDuration() == 0) {
      if (pmeasure) {
         setDuration(pmeasure->getTimeSigDur());
      } else {
         setTimeSigDur(getTimeSigDur());
      }
      needdummy = true;
   }

	// Maybe check for overfull measures around here

   if (needdummy || getEventCount() == 0) {
      // if the duration of the measure is zero, then set the duration
      // of the measure to the duration of the time signature
      // This is needed for certain cases of multi-measure rests, where no
      // full-measure rest is given in the measure (Sibelius does this).
      setDuration(getTimeSigDur());
		addDummyRest();
   }

   // Neeed to check for empty voice/layers occuring lower in the
   // voice index list than layers which contain notes.  For example
   // if voice/layer 2 contains notes, but voice/layer 1 does not, then
   // a dummy full-measure rest should fill voice/layer 1.  The voice
   // layer 1 should be filled with the duration of the measure according
   // to the other voice/layers in the measure.  This is done later
   // after a voice analysis has been done in
   // musicxml2hum_interface::insertMeasure(), specifically:
	// musicxml2hum_interface::checkForDummyRests().

	sortEvents();

	return output;
}



//////////////////////////////
//
// MxmlMeasure::forceLastInvisible --
//

void MxmlMeasure::forceLastInvisible(void) {
   if (!m_events.empty()) {
      m_events.back()->forceInvisible();
   }
}



//////////////////////////////
//
// MxmlMeasure::getEventList --
//

vector<MxmlEvent*>& MxmlMeasure::getEventList(void) {
   return m_events;
}



//////////////////////////////
//
// MxmlMeasure::addDummyRest --
//

void MxmlMeasure::addDummyRest(void) {
   HumNum measuredur = getTimeSigDur();
   HumNum starttime = getStartTime();
   MxmlEvent* event = new MxmlEvent(this);
   m_events.push_back(event);
   MxmlMeasure* measure = this;
   event->makeDummyRest(measure, starttime, measuredur);
}


void MxmlMeasure::addDummyRest(HumNum starttime, HumNum duration,
		int staffindex, int voiceindex) {
	MxmlEvent* event = new MxmlEvent(this);
	m_events.push_back(event);
   MxmlMeasure* measure = this;
   event->makeDummyRest(measure, starttime, duration, staffindex, voiceindex);
}



//////////////////////////////
//
// MxmlMeasure::setStartTimeOfMeasure --
//

void MxmlMeasure::setStartTimeOfMeasure(void) {
	if (!m_owner) {
		setStartTimeOfMeasure(0);
		return;
	}
	MxmlMeasure* previous = m_owner->getPreviousMeasure(this);
	if (!previous) {
		setStartTimeOfMeasure(0);
		return;
	}
	setStartTimeOfMeasure(previous->getStartTime() + previous->getDuration());
}


void MxmlMeasure::setStartTimeOfMeasure(HumNum value) {
	m_starttime = value;
}



//////////////////////////////
//
// MxmlMeasure::calculateDuration --
//

void MxmlMeasure::calculateDuration(void) {
	HumNum maxdur   = 0;
	HumNum sum      = 0;
	for (int i=0; i<(int)m_events.size(); i++) {
		m_events[i]->setStartTime(sum + getStartTime());
		sum += m_events[i]->getDuration();
		if (maxdur < sum) {
			maxdur = sum;
		}
	}
	setDuration(maxdur);
}



//////////////////////////////
//
// MxmlMeasure::setDuration --
//

void MxmlMeasure::setDuration(HumNum value) {
	m_duration = value;
}



//////////////////////////////
//
// MxmlMeasure::getStartTime --
//

HumNum MxmlMeasure::getStartTime(void) const {
	return m_starttime;
}



//////////////////////////////
//
// MxmlMeasure::getDuration --
//

HumNum MxmlMeasure::getDuration(void) const {
	return m_duration;
}



//////////////////////////////
//
// MxmlMeasure::setOwner --
//

void MxmlMeasure::setOwner(MxmlPart* part) {
	m_owner = part;
}



//////////////////////////////
//
// MxmlMeasure::setOwner --
//

MxmlPart* MxmlMeasure::getOwner(void) const {
	return m_owner;
}



//////////////////////////////
//
// MxmlMeasure::reportVerseCountToOwner --
//

void MxmlMeasure::reportVerseCountToOwner(int count) {
	if (!m_owner) {
		return;
	}
	m_owner->receiveVerseCount(count);
}


void MxmlMeasure::reportVerseCountToOwner(int staffindex, int count) {
	if (!m_owner) {
		return;
	}
	m_owner->receiveVerseCount(staffindex, count);
}



//////////////////////////////
//
// MxmlMeasure::reportHarmonyCountToOwner --
//

void MxmlMeasure::reportHarmonyCountToOwner(int count) {
	if (!m_owner) {
		return;
	}
	m_owner->receiveHarmonyCount(count);
}



//////////////////////////////
//
// MxmlMeasure::getPartNumber --
//

int MxmlMeasure::getPartNumber(void) const {
	if (!m_owner) {
		return 0;
	}
	return m_owner->getPartNumber();
}



//////////////////////////////
//
// MxmlMeasure::getPartIndex --
//

int MxmlMeasure::getPartIndex(void) const {
	if (!m_owner) {
		return -1;
	}
	return m_owner->getPartIndex();
}



//////////////////////////////
//
// MxmlMeasure::setQTicks -- Set the number of ticks per quarter note.
//     Returns the number of times that the ticks has been set.
//

int MxmlMeasure::setQTicks(long value) {
	if (m_owner) {
		return m_owner->setQTicks(value);
	} else {
		return 0;
	}
}



//////////////////////////////
//
// MxmlMeasure::getQTicks -- Get the number of ticks per quarter note.
//

long MxmlMeasure::getQTicks(void) const {
	if (m_owner) {
		return m_owner->getQTicks();
	} else {
		return 0;
	}
}



//////////////////////////////
//
// MxmlMeasure::attachLastEventToPrevious --
//

void MxmlMeasure::attachLastEventToPrevious(void) {
 	if (m_events.size() < 2) {
 		return;
 	}
	MxmlEvent* event = m_events.back();
	m_events.resize(m_events.size() - 1);
	m_events.back()->link(event);
}



//////////////////////////////
//
// MxmlMeasure::getEventCount --
//

int MxmlMeasure::getEventCount(void) const {
	return (int)m_events.size();
}



//////////////////////////////
//
// MxmlMeasure::getSortedEvents --
//

vector<SimultaneousEvents>* MxmlMeasure::getSortedEvents(void) {
	return &m_sortedevents;
}



//////////////////////////////
//
// MxmlMeasure::getEvent --
//

MxmlEvent* MxmlMeasure::getEvent(int index) const {
	if (index < 0) {
		return NULL;
	}
	if (index >= (int)m_events.size()) {
		return NULL;
	}
	return m_events[index];
}



//////////////////////////////
//
// MxmlMeasure::setPreviousMeasure --
//

void MxmlMeasure::setPreviousMeasure(MxmlMeasure* event) {
	m_previous = event;
}



//////////////////////////////
//
// MxmlMeasure::setNextMeasure --
//

void MxmlMeasure::setNextMeasure(MxmlMeasure* event) {
	m_following = event;
}



//////////////////////////////
//
// MxmlMeasure::getPreviousMeasure --
//

MxmlMeasure* MxmlMeasure::getPreviousMeasure(void) const {
	return m_previous;
}



//////////////////////////////
//
// MxmlMeasure::getNextMeasure --
//

MxmlMeasure* MxmlMeasure::getNextMeasure(void) const {
	return m_following;
}



//////////////////////////////
//
// MxmlMeasure::getVoiceIndex --
//

int MxmlMeasure::getVoiceIndex(int voicenum) {
   if (m_owner) {
      return m_owner->getVoiceIndex(voicenum);
   } else {
      return -1;
   }
}



//////////////////////////////
//
// MxmlMeasure::getStaffIndex --
//

int MxmlMeasure::getStaffIndex(int voicenum) {
   if (m_owner) {
      return m_owner->getStaffIndex(voicenum);
   } else {
      return -1;
   }
}



///////////////////////////////////////////////////////////////////////////
//
// private functions --
//

//////////////////////////////
//
// MxmlMeasure::sortEvents -- Sorts events for the measure into
//   time order.  They are split into zero-duration evnets and
//   non-zero events.  mevent_floating type are placed into the 
//   non-zero events eventhough they have zero duration (this is
//   for harmony not attached to a note attack, and will be
//   eventually including basso continuo figuration having the
//   same situation).
//

void MxmlMeasure::sortEvents(void) {
	int i;
	set<HumNum> times;

	for (i=0; i<(int)m_events.size(); i++) {
		times.insert(m_events[i]->getStartTime());
	}

	m_sortedevents.resize(times.size());
	int counter = 0;

	for (HumNum val : times) {
		m_sortedevents[counter++].starttime = val;
	}

	// setup sorted access:
	map<HumNum, SimultaneousEvents*> mapping;
	for (i=0; i<(int)m_sortedevents.size(); i++) {
		mapping[m_sortedevents[i].starttime] = &m_sortedevents[i];
	}

	HumNum duration;
	HumNum starttime;
	for (i=0; i<(int)m_events.size(); i++) {

		// skip storing certain types of events:
		switch (m_events[i]->getType()) {
			case mevent_backup:
				continue;
			case mevent_forward:
            if (m_events[i]->getDuration() == this->getDuration()) {
                 // forward elements are encoded as whole-measure rests
                 // if they fill the duration of a measure
            } else if (m_events[i]->getVoiceIndex() < 0) {
               // Skip forward elements which are not invisible rests
               continue;
            }
            break;
			default:
				break;
		}

		starttime = m_events[i]->getStartTime();
		duration  = m_events[i]->getDuration();
		if (m_events[i]->isFloating()) {
			mapping[starttime]->nonzerodur.push_back(m_events[i]);
		} else if (duration == 0) {
			mapping[starttime]->zerodur.push_back(m_events[i]);
		} else {
			mapping[starttime]->nonzerodur.push_back(m_events[i]);
		}
	}

	/* debugging information:

	int j;
	vector<SimultaneousEvents>& se = m_sortedevents;

	cout << "QTIME SORTED EVENTS:" << endl;
	for (i=0; i<(int)se.size(); i++) {
		if (se[i].zerodur.size() > 0) {
			cout << se[i].starttime << "z\t";
			for (j=0; j<(int)se[i].zerodur.size(); j++) {
				cout << " " << se[i].zerodur[j]->getName();
				cout << "(";
				cout << se[i].zerodur[j]->getPartNumber();
				cout << ",";
				cout << se[i].zerodur[j]->getStaffNumber();
				cout << ",";
				cout << se[i].zerodur[j]->getVoiceNumber();
				cout << ")";
			}
			cout << endl;
		}
		if (se[i].nonzerodur.size() > 0) {
			cout << se[i].starttime << "\t";
			for (j=0; j<(int)se[i].nonzerodur.size(); j++) {
				cout << " " << se[i].nonzerodur[j]->getName();
				cout << "(";
				cout << se[i].nonzerodur[j]->getPartNumber();
				cout << ",";
				cout << se[i].nonzerodur[j]->getStaffNumber();
				cout << ",";
				cout << se[i].nonzerodur[j]->getVoiceNumber();
				cout << ")";
			}
			cout << endl;
		}
	}
	*/

}



//////////////////////////////
//
// MxmlMeasure::receiveStaffNumberFromChild -- Receive a staff number
//    placement for a note or rest and pass it along to the part class
//    so that it can keep track of the maximum staff number used in
//    the part.
//

void MxmlMeasure::receiveStaffNumberFromChild(int staffnum, int voicenum) {
	reportStaffNumberToOwner(staffnum, voicenum);
}



//////////////////////////////
//
// MxmlMeasure::receiveTimeSigDurFromChild --
//

void MxmlMeasure::receiveTimeSigDurFromChild(HumNum duration) {
   setTimeSigDur(duration);
}



//////////////////////////////
//
// MxmlMeasure::setTimeSigDur --
//

void MxmlMeasure::setTimeSigDur(HumNum duration) {
   m_timesigdur = duration;
}



//////////////////////////////
//
// MxmlMeasure::getTimeSigDur --
//

HumNum MxmlMeasure::getTimeSigDur(void) {
   return m_timesigdur;
}



//////////////////////////////
//
// MxmlMeasure::reportStaffNumberToOwner -- Send a staff number
//    placement for a note or rest and pass it along to the part class
//    so that it can keep track of the maximum staff number used in
//    the part.
//

void MxmlMeasure::reportStaffNumberToOwner(int staffnum, int voicenum) {
	if (m_owner != NULL) {
		m_owner->receiveStaffNumberFromChild(staffnum, voicenum);
	}
}



//////////////////////////////
//
// MxmlMeasure::receiveMeasureStyleFromChild --
//

void  MxmlMeasure::receiveMeasureStyleFromChild(MeasureStyle style) {
	if (style == MeasureStyle::RepeatForward) {
		MxmlMeasure* previous = getPreviousMeasure();
		if (previous) {
			previous->setStyle(style);
		}
	} else {
		setStyle(style);
	}
}



//////////////////////////////
//
// MxmlMeasure::getStyle --
//

MeasureStyle MxmlMeasure::getStyle(void) {
	return m_style;
}



//////////////////////////////
//
// MxmlMeasure::getBarStyle --
//

MeasureStyle MxmlMeasure::getBarStyle(void) { 
	return getStyle();
}



//////////////////////////////
//
// MxmlMeasure::setStyle --
//

void MxmlMeasure::setStyle(MeasureStyle style) {
	if (m_style == MeasureStyle::Plain) {
		m_style = style;
	} else if ((m_style == MeasureStyle::RepeatBackward) && 
			(style == MeasureStyle::RepeatForward)) {
		m_style = MeasureStyle::RepeatBoth;
	} else if ((m_style == MeasureStyle::RepeatForward) && 
			(style == MeasureStyle::RepeatBackward)) {
		m_style = MeasureStyle::RepeatBoth;
	} else {
		// some sort of problem to deal with later
		m_style = style;
	}
}



//////////////////////////////
//
// MxmlMeasure::setBarStyle --
//

void MxmlMeasure::setBarStyle(MeasureStyle style) { 
	m_style = style;
}




} // end namespace hum



