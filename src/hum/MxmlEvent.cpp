//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Aug  6 10:53:40 CEST 2016
// Last Modified: Sun Sep 18 14:16:18 PDT 2016
// Filename:      musicxml2hum.cpp
// URL:           https://github.com/craigsapp/hum2ly/blob/master/src/MxmlEvent.cpp
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   MusicXML parsing abstraction for elements which are children
//                of the measure element.
//

#include "humlib.h"

#include "MxmlEvent.h"
#include "MxmlMeasure.h"

#include "pugiconfig.hpp"
#include "pugixml.hpp"

#include <stdlib.h>

#include <sstream>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>

using namespace pugi;
using namespace std;

namespace hum {

class MxmlMeasure;
class MxmlPart;

int MxmlEvent::m_counter = 0;

////////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// MxmlEvent::MxmlEvent -- Constructor.
//

MxmlEvent::MxmlEvent(MxmlMeasure* measure) {
	clear();
	m_owner = measure;
	m_sequence = m_counter++;
}



//////////////////////////////
//
// MxmlEvent::~MxmlEvent -- Destructor.
//

MxmlEvent::~MxmlEvent() {
	clear();
}



//////////////////////////////
//
// MxmlEvent::clear -- Clear any previous contents of the object.
//

void MxmlEvent::clear(void) {
	m_starttime = m_duration = 0;
	m_eventtype = mevent_unknown;
	m_owner = NULL;
	m_linked = false;
	m_voice = -1;
	m_staff = 0;
	m_invisible = false;
	m_voiceindex = -1;
	m_sequence = -1;
	for (int i=0; i<(int)m_links.size(); i++) {
		delete m_links[i];
		m_links[i] = NULL;
	}
	m_links.resize(0);
}



///////////////////////////////
//
// MxmlEvent::makeDummyRest --
//   default values:
//     staffindex = 0;
//     voiceindex = 0;
//

void MxmlEvent::makeDummyRest(MxmlMeasure* owner, HumNum starttime,
		HumNum duration, int staffindex, int voiceindex) {
	m_starttime = starttime;
	m_duration = duration;
	m_eventtype = mevent_forward;  // not a real rest (will be invisible)
	// m_node remains null
	// m_links remains empty
	m_linked = false;
	m_sequence = -m_counter;
	m_counter++;
	m_voice = 1;  // don't know what the original voice number is
	m_voiceindex = voiceindex;
	m_staff = staffindex + 1;
	m_maxstaff = m_staff;  // how is this used/set?
	//	m_hnode remains null
}



//////////////////////////////
//
// MxmlEvent::setStartTime -- Set the starting timestamp of the event
//    in terms of quater notes since the start of the music.
//

void MxmlEvent::setStartTime(HumNum value) {
	m_starttime = value;
}



//////////////////////////////
//
// MxmlEvent::setDuration -- Set the duration of the event in terms
//   of quarter note durations.
//

void MxmlEvent::setDuration(HumNum value) {
	m_duration = value;
}



//////////////////////////////
//
// MxmlEvent::getStartTime -- Return the start time of the event in terms
//      of quarter notes since the start of the music.
//

HumNum MxmlEvent::getStartTime(void) const {
	return m_starttime;
}



//////////////////////////////
//
// MxmlEvent::getDuration -- Return the duration of the event in terms
//      of quarter note durations.
//

HumNum MxmlEvent::getDuration(void) {
	return m_duration;
}



//////////////////////////////
//
// MxmlEvent::setOwner -- Indicate which measure the event belongs to.
//

void MxmlEvent::setOwner(MxmlMeasure* measure) {
	m_owner = measure;
}



//////////////////////////////
//
// MxmlEvent::getOwner -- Return the measure object that contains this
//     event.  If there is no owner, then returns NULL.
//

MxmlMeasure* MxmlEvent::getOwner(void) const {
	return m_owner;
}



//////////////////////////////
//
// MxmlEvent::reportVerseCountToOwner --
//

void MxmlEvent::reportVerseCountToOwner(int count) {
	if (!m_owner) {
		return;
	}
	m_owner->reportVerseCountToOwner(count);
}


void MxmlEvent::reportVerseCountToOwner(int staffindex, int count) {
	if (!m_owner) {
		return;
	}
	m_owner->reportVerseCountToOwner(staffindex, count);
}



//////////////////////////////
//
// MxmlEvent::reportHarmonyCountToOwner --
//

void MxmlEvent::reportHarmonyCountToOwner(int count) {
	if (!m_owner) {
		return;
	}
	m_owner->reportHarmonyCountToOwner(count);
}


//////////////////////////////
//
// MxmlEvent::reportMeasureStyleToOwner --

void MxmlEvent::reportMeasureStyleToOwner (MeasureStyle style) {
	if (!m_owner) {
		return;
	}
	m_owner->receiveMeasureStyleFromChild(style);
}



//////////////////////////////
//
// MxmlEvent::getPartNumber --
//

int MxmlEvent::getPartNumber(void) const {
	if (!m_owner) {
		return 0;
	}
	return m_owner->getPartNumber();
}



//////////////////////////////
//
// MxmlEvent::getPartIndex --
//

int MxmlEvent::getPartIndex(void) const {
	if (!m_owner) {
		return 0;
	}
	return m_owner->getPartIndex();
}



//////////////////////////////
//
// MxmlEvent::getName --
//

const char* MxmlEvent::getName(void) const {
	return m_node.name();
}



//////////////////////////////
//
// MxmlEvent::setQTicks -- Set the number of ticks per quarter note.
//     Returns the number of times that the ticks has been set.
//     Returns 0 if the tick count is invalid.
//

int MxmlEvent::setQTicks(long value) {
	if (value <= 0) {
		return 0;
	}
	if (m_owner) {
		return m_owner->setQTicks(value);
	} else {
		return 0;
	}
}



//////////////////////////////
//
// MxmlEvent::getQTicks -- Get the number of ticks per quarter note.
//

long MxmlEvent::getQTicks(void) const {
	if (m_owner) {
		return m_owner->getQTicks();
	} else {
		return 0;
	}
}



//////////////////////////////
//
// MxmlEvent::getIntValue -- Convenience function for an XPath query,
//    where the child text of the element should be interpreted as
//    an integer.
//

long MxmlEvent::getIntValue(const char* query) const {
	const char* val = m_node.select_node(query).node().child_value();
	if (strcmp(val, "") == 0) {
		return 0;
	} else {
		return atoi(val);
	}
}



//////////////////////////////
//
// Mxmlvent::setDurationByTicks -- Given a <duration> element tick
//    count, set the duration by dividing by the current quarter-note
//    duration tick count (from a prevailing attribute setting for
//    <divisions>).
//

void MxmlEvent::setDurationByTicks(long value, xml_node el) {
	long ticks = getQTicks();
	if (ticks == 0) {
		setDuration(0);
		return;
	}

	if (isGrace()) {
		setDuration(0);
		return;
	}

	HumNum val = value;
	val /= ticks;

	if (el) {
		HumNum checkval = getEmbeddedDuration(el);
		if ((checkval == 0) && isRest()) {
			// This is a whole rest.
			// val = val
		} else if (checkval != val) {
			// cerr << "WARNING: True duration " << checkval << " does not match";
			// cerr << " tick duration (buggy data: " << val << ")" << endl;
			double difference = fabs(checkval.getFloat() - val.getFloat());
			if (difference < 0.1) {
				// only correct if the duration is small, since some programs
				// will mark rests such as half notes as whole notes (since they
				// are displayed as centered whole notes)
				val = checkval;
			}
		}
	}
	setDuration(val);
}



//////////////////////////////
//
// MxmlEvent::hasChild -- True if the given XPath query resulting
//      element has a child node.
//

bool MxmlEvent::hasChild(const char* query) const {
	xpath_node result = m_node.select_single_node(query);
	return !result.node().empty();
}



//////////////////////////////
//
// MxmlEvent::attachToLast --
//

void MxmlEvent::attachToLastEvent(void) {
	if (!m_owner) {
		return;
	}
	m_owner->attachLastEventToPrevious();
}



//////////////////////////////
//
// MxmlEvent::link --  This function is used to link secondary
//   elements to a primary one.  Currently only used for chord notes.
//   The first note of a chord will be stored in event lists, and
//   secondary notes will be suppressed from the list and instead
//   accessed through the m_links structure.
//

void MxmlEvent::link(MxmlEvent* event) {
	m_links.push_back(event);
	event->setLinked();
}



//////////////////////////////
//
// MxmlEvent::setLinked -- Indicate that a note is a secondary
//     chord note.
//

void MxmlEvent::setLinked(void) {
	m_linked = true;
}



//////////////////////////////
//
// MxmlEvent::isLinked -- Returns true if the note is a secondary
//     chord note.
//

bool MxmlEvent::isLinked(void) const {
	return m_linked;
}



//////////////////////////////
//
// MxmlEvent::isRest --
//

bool MxmlEvent::isRest(void) {
	if (!m_node) {
		return false;
	}
	xml_node child = m_node.first_child();
	while (child) {
		if (nodeType(child, "rest")) {
			return true;
		}
		child = child.next_sibling();
	}
	return false;
}



//////////////////////////////
//
// MxmlEvent::isChord -- Returns true if the event is the primary note
//    in a chord.
//

bool MxmlEvent::isChord(void) const {
	if ((m_links.size() > 0) && nodeType(m_node, "note")) {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// MxmlEvent::isGrace -- Returns true if the event is the primary note
//    in a chord.
//

bool MxmlEvent::isGrace(void) {
	xml_node child = this->getNode();
	if (!nodeType(child, "note")) {
		return false;
	}
	child = child.first_child();
	while (child) {
		if (nodeType(child, "grace")) {
			return true;
		} else if (nodeType(child, "pitch")) {
			// grace element has to come before pitch
			return false;
		}
		child = child.next_sibling();
	}
	return false;
}



//////////////////////////////
//
// MxmlEvent::hasSlurStart -- 
//   direction: 0=unspecified, 1=positive curvature, -1=negative curvature.
//
//  <note>
//     <notations>
//         <slur type="start" orientation="under" number="1">
//         <slur type="start" orientation="over" number="1">
//
//

bool MxmlEvent::hasSlurStart(int& direction) {
	direction = 0;
	bool output = false;
	xml_node child = this->getNode();
	if (!nodeType(child, "note")) {
		return output;
	}
	child = child.first_child();
	while (child) {
		if (nodeType(child, "notations")) {
			xml_node grandchild = child.first_child();
			while (grandchild) {
				if (nodeType(grandchild, "slur")) {
					xml_attribute slurtype = grandchild.attribute("type");
					if (slurtype) {
						if (strcmp(slurtype.value(), "start") == 0) {
							output = true;
						}
					}
					xml_attribute orientation = grandchild.attribute("orientation");
					if (orientation) {
						if (strcmp(orientation.value(), "over") == 0) {
							direction = 1;
						} else if (strcmp(orientation.value(), "under") == 0) {
							direction = -1;
						}
					}
					return output;
				}
				grandchild = grandchild.next_sibling();
			}
		}
		child = child.next_sibling();
	}
	return output;
}



//////////////////////////////
//
// MxmlEvent::hasSlurStop --
//
//  <note>
//     <notations>
//         <slur type="start" orientation="under" number="1">
//

bool MxmlEvent::hasSlurStop(void) {
	xml_node child = this->getNode();
	if (!nodeType(child, "note")) {
		return false;
	}
	child = child.first_child();
	while (child) {
		if (nodeType(child, "notations")) {
			xml_node grandchild = child.first_child();
			while (grandchild) {
				if (nodeType(grandchild, "slur")) {
					xml_attribute slurtype = grandchild.attribute("type");
					if (slurtype) {
						if (strcmp(slurtype.value(), "stop") == 0) {
							return true;
						}
					}
				}
				grandchild = grandchild.next_sibling();
			}
		}
		child = child.next_sibling();
	}
	return false;
}



//////////////////////////////
//
// MxmlEvent::isFloating -- For a harmony or basso continuo item
//     which is not attached to a note onset.
//

bool MxmlEvent::isFloating(void) {
	xml_node empty = xml_node(NULL);
	if (m_node == empty && (m_hnode != empty)) {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// MxmlEvent::getLinkedNotes --
//

vector<MxmlEvent*> MxmlEvent::getLinkedNotes(void) {
	return m_links;
}



//////////////////////////////
//
// MxmlEvent::printEvent -- Useful for debugging.
//

void MxmlEvent::printEvent(void) {
	cout << getStartTime() << "\t" << getDuration() << "\t" << m_node.name();
	if (isChord()) {
		cout << "\tCHORD";
	}
	cout << endl;
}



//////////////////////////////
//
// MxmlEvent::getSequenceNumber -- Return the sequence number of the
//   event in the input data file.  Useful for sorting items which
//   occur at the same time.
//

int MxmlEvent::getSequenceNumber(void) const {
	return m_sequence;
}



//////////////////////////////
//
// MxmlEvent::getVoiceNumber -- Return the voice number of the event.
//

int MxmlEvent::getVoiceNumber(void) const {
	if (m_voice) {
		return m_voice;
	} else {
		return 1;
	}
}



//////////////////////////////
//
// MxmlEvent::setVoiceIndex --
//

void MxmlEvent::setVoiceIndex(int index) {
	m_voiceindex = index;
}



//////////////////////////////
//
// MxmlEvent::getVoiceIndex -- Return the voice number of the event.
//    But mod 4 which presumably sets the voice number on a staff.
//    This is not always true: "PrintMusic 2010 for Windows" may
//    use voice 2 for staff 2. In this case the voice index should
//    be calculated by %2 rather than %4.
//    default value: maxvoice = 4.
//
//    This function will replace with a query to MxmlPart
//    as to what the voice on a staff should be.
//

int MxmlEvent::getVoiceIndex(int maxvoice) const {
	if (m_voiceindex >= 0) {
		return m_voiceindex;
	}

	if (m_owner) {
		int voiceindex = m_owner->getVoiceIndex(m_voice);
		if (voiceindex >= 0) {
			return voiceindex;
		}
	}

	// the following case handles notes/rests which do not contain
	// a voice number.  Assume that this item should be placed
	// in the first voice.
	if (m_voiceindex < 0) {
		if (nodeType(m_node, "note")) {
			return 0;
		}
	}


	// don't know what the voice mapping is, so make one up:
	if (maxvoice < 1) {
		maxvoice = 4;
	}
	if (m_voice) {
		return (m_voice - 1) % maxvoice;
	} else {
		return 0;
	}
}



//////////////////////////////
//
// MxmlEvent::forceInvisible --
//

void MxmlEvent::forceInvisible(void) {
	m_invisible = true;
}



//////////////////////////////
//
// MxmlEvent::isInvisible --
//

bool MxmlEvent::isInvisible(void) {
	return m_invisible;
}



//////////////////////////////
//
// MxmlEvent::getStaffIndex --
//

int MxmlEvent::getStaffIndex(void) const {
	if (m_staff > 0) {
		return m_staff - 1;
	}
	if (m_owner) {
		int staffindex = m_owner->getStaffIndex(m_voice);
		if (staffindex >= 0) {
			return staffindex;
		}
	}

	// don't know what the modified staff is, so give the original staff index:
	if (!m_staff) {
		return 0;
	} else {
		return m_staff - 1;
	}
}



//////////////////////////////
//
// MxmlEvent::setVoiceNumber --
//

void MxmlEvent::setVoiceNumber(int value) {
	m_voice = (short)value;
}



//////////////////////////////
//
// MxmlEvent::setStaffNumber --
//

void MxmlEvent::setStaffNumber(int value) {
	m_staff = (short)value;
}



//////////////////////////////
//
// MxmlEvent::getStaffNumber --
//

int MxmlEvent::getStaffNumber(void) const {
	if (!m_staff) {
		return 1;
	} else {
		return m_staff;
	}
}



//////////////////////////////
//
// MxmlEvent::getType --
//

measure_event_type MxmlEvent::getType(void) const {
	return m_eventtype;
}



//////////////////////////////
//
// MxmlEvent::parseEvent --
//

bool MxmlEvent::parseEvent(xpath_node el, HumNum starttime) {
	return parseEvent(el.node(), xml_node(NULL), starttime);
}


bool MxmlEvent::parseEvent(xml_node el, xml_node nextel, HumNum starttime) {
	m_node = el;

	bool floatingharmony = false;
	if (nodeType(m_node, "attributes")) {
		m_eventtype = mevent_attributes;
	} else if (nodeType(m_node, "backup")) {
		m_eventtype = mevent_backup;
	} else if (nodeType(m_node, "barline")) {
		m_eventtype = mevent_barline;
		setBarlineStyle(m_node);
	} else if (nodeType(m_node, "bookmark")) {
		m_eventtype = mevent_bookmark;
	} else if (nodeType(m_node, "direction")) {
		m_eventtype = mevent_direction;
	} else if (nodeType(m_node, "figured-bass")) {
		m_eventtype = mevent_figured_bass;
	} else if (nodeType(m_node, "forward")) {
		m_eventtype = mevent_forward;
		m_staff = -1; // set default staff if not supplied
		m_voice = -1; // set default staff if not supplied
	} else if (nodeType(m_node, "grouping")) {
		m_eventtype = mevent_grouping;
	} else if (nodeType(m_node, "harmony")) {
		m_eventtype = mevent_harmony;
		if (!nodeType(nextel, "note")) {
			// harmony is not attached to a note
			floatingharmony = true;
			m_staff = -1;
			m_voice = -1;
		}
	} else if (nodeType(m_node, "link")) {
		m_eventtype = mevent_link;
	} else if (nodeType(m_node, "note")) {
		m_eventtype = mevent_note;
		m_staff = 1; // set default staff if not supplied
		m_voice = -1; // set default staff if not supplied
	} else if (nodeType(m_node, "print")) {
		m_eventtype = mevent_print;
	} else if (nodeType(m_node, "sound")) {
		m_eventtype = mevent_sound;
	} else {
		m_eventtype = mevent_unknown;
	}

	int tempstaff    = 1;
	int tempvoice    = -1;
	int tempduration = 0;
	for (auto el = m_node.first_child(); el; el = el.next_sibling()) {
		if (nodeType(el, "staff")) {
			tempstaff = atoi(el.child_value());
		} else if (nodeType(el, "voice")) {
			tempvoice = atoi(el.child_value());
		} else if (nodeType(el, "duration")) {
			tempduration = atoi(el.child_value());
		}
	}

	bool emptyvoice = false;
	if (!floatingharmony) {
		if (tempvoice < 0) {
			emptyvoice = true;
			if (nodeType(el, "note")) {
				this->setVoiceIndex(0);
			}
		}
	}

	if (m_eventtype == mevent_forward) {
		xml_node pel = el.previous_sibling();
		if (nodeType(pel, "harmony")) {
			// This is a spacer forward which is not in any voice/layer,
			// so invalidate is staff/voice to prevent it from being
			// converted to a rest.
			m_voice = -1;
			tempvoice = -1;
			m_staff = -1;
			tempstaff = -1;
		}
	}

	if (tempvoice >= 0) {
		m_voice = (short)tempvoice;
	}
	if (tempstaff > 0) {
		m_staff = (short)tempstaff;
	}
	if (!emptyvoice) {
   	reportStaffNumberToOwner(m_staff, m_voice);
	} else {
		// no voice child element, or not a note or rest.
	}
	HumNum timesigdur;
	HumNum difference;
	HumNum dur;
	MxmlMeasure* measure = getOwner();
	HumNum mst;
	if (measure) {
		mst = measure->getStartTime();
	}

	setStartTime(starttime);

	switch (m_eventtype) {
		case mevent_note:
			setDuration(0);
			if (hasChild("./chord")) {
				setDuration(0);
				attachToLastEvent();
			} else {
				setDurationByTicks(tempduration, el);
			}
			break;

		case mevent_forward:
			setDurationByTicks(tempduration);
			break;

		case mevent_backup:
			setDurationByTicks(-tempduration);
			dur = getDuration();
			difference = starttime - mst + dur;
			if (difference < 0) {
				// cerr << "Warning: backup before start of measure " << endl;
				setDuration(dur - difference);
			}
			break;

		case mevent_attributes:
			setQTicks(getIntValue("./divisions"));
			timesigdur = getTimeSigDur();
			if (timesigdur > 0) {
				reportTimeSigDurToOwner(timesigdur);
			}
			break;

		case mevent_harmony:
		case mevent_barline:
		case mevent_bookmark:
		case mevent_direction:
		case mevent_figured_bass:
		case mevent_grouping:
		case mevent_link:
		case mevent_print:
		case mevent_sound:
		case mevent_unknown:
			setDuration(tempduration);
			break;
		case mevent_float:
			// assigned later for floating harmony
			break;
	}

	if (floatingharmony) {
		m_hnode = el;
		m_eventtype = mevent_float;
		m_duration = 0;
		m_node = xml_node(NULL);
		m_voice = 1;
		m_voiceindex = 0;
	} else {
		// if the previous sibling was a <harmony>, then store
		// for later parsing.  May have to check even further back
		// until another note or barline was found.
		xml_node lastsib = el.previous_sibling();
		if (!lastsib) {
			return true;
		}
		if (nodeType(lastsib, "harmony")) {
			m_hnode = lastsib;
		}
	}

	return true;
}



//////////////////////////////
//
// MxmlEvent::getTimeSigDur -- extract the time signature duration
//     from an attributes element.  If there is no time signature
//     in the attributes list, then return 0.
//                <time>
//                    <beats>4</beats>
//                    <beat-type>4</beat-type>
//                </time>
//     Output duration is in units of quarter notes.
//

HumNum MxmlEvent::getTimeSigDur(void) {
	if (!nodeType(m_node, "attributes")) {
		return 0;
	}
	int beats = 0;
	int beattype = 4;
	xml_node child = m_node.first_child();
	while (child) {
		if (!nodeType(child, "time")) {
			child = child.next_sibling();
			continue;
		}
		xml_node grandchild = child.first_child();
		while (grandchild) {
			if (nodeType(grandchild, "beats")) {
				beats = atoi(grandchild.child_value());
			} else if (nodeType(grandchild, "beat-type")) {
				beattype = atoi(grandchild.child_value());
			}
			grandchild = grandchild.next_sibling();
		}
		break;
	}
	HumNum output = beats;
	output /= beattype;
	output *= 4; // convert to quarter note duration
	return output;
}



//////////////////////////////
//
// MxmlEvent::setBarlineStyle --
// "==" -> Final
//    <barline location="right">
//       <bar-style>light-heavy</bar-style>
//    </barline>
//
// ":|!" -> RepeatBackward
//    <barline location="right">
//       <bar-style>light-heavy</bar-style>
//       <repeat direction="backward"/>
//    </barline>
//
//  "!|:" -> RepeatForward
//    <barline location="left">
//        <repeat direction="forward"/>
//    </barline>
//

void MxmlEvent::setBarlineStyle(xml_node node) {
	xml_node child = node.first_child();
	int repeat = 0;
	string barstyle;
	while (child) {
		if (nodeType(child, "bar-style")) {
			barstyle = child.child_value();
		} else if (nodeType(child, "repeat")) {
			if (strcmp(child.attribute("direction").value(), "backward") == 0) {
				repeat = -1;
			} else if (strcmp(child.attribute("direction").value(),
					"forward") == 0) {
				repeat = +1;
			}
		}
		child = child.next_sibling();
	}

	if ((repeat == 0) && (barstyle == "light-light")) {
		reportMeasureStyleToOwner(MeasureStyle::Double);
	} else if ((repeat == 0) && (barstyle == "light-heavy")) {
		reportMeasureStyleToOwner(MeasureStyle::Final);
	} else if ((repeat == -1) && (barstyle == "light-heavy")) {
		reportMeasureStyleToOwner(MeasureStyle::RepeatBackward);
	} else if (repeat == +1) {
		reportMeasureStyleToOwner(MeasureStyle::RepeatForward);
	}
}



//////////////////////////////
//
// MxmlEvent::getRecip -- return **recip value for note/rest.
//   Units are whole notes.
//

string MxmlEvent::getRecip(void) const {
	HumNum dur = m_duration;
	dur /= 4;  // convert to whole-note units;
	int n = getDotCount();
	if (n > 0) {
		dur = dur * (1 << n) / ((1 << (n+1)) - 1);
	} else if (n < 0) {
		// calculate a dot count and adjust duration as needed
		if (dur.getNumerator() == 1) {
			// do nothing since it won't need dots
		} else {
			// otherwise check to three augmentation dots
			HumNum onedotdur = dur * (1 << 1) / ((1 << 2) - 1);
			if (onedotdur.getNumerator() == 1) {
				dur = onedotdur;
				n = 1;
			} else {
				HumNum twodotdur = dur * (1 << 2) / ((1 << 3) - 1);
				if (twodotdur.getNumerator() == 1) {
					dur = twodotdur;
					n = 2;
				} else {
					HumNum threedotdur = dur * (1 << 3) / ((1 << 4) - 1);
					if (threedotdur.getNumerator() == 1) {
						dur = threedotdur;
						n = 3;
					}
				}
			}
		}
	}
	stringstream ss;
	ss << dur.getDenominator();
	if (dur.getNumerator() != 1) {
		ss << "%" << dur.getNumerator();
	}
	for (int i=0; i<n; i++) {
		ss << ".";
	}
	return ss.str();
}



//////////////////////////////
//
// MxmlEvent::getKernPitch -- return **kern pitch of note/rest.
//

string MxmlEvent::getKernPitch(void) {
	bool rest = false;

	if (!m_node) {
		// this is for an interpreted whole-measure rest.  Needed
		// for multi-measure rests as generated by Sibelius.
		return "r";
	}

	xml_node child = m_node.first_child();

	string step;
	int alter  = 0;
	int octave = 4;
	bool explicitQ    = false;
	bool naturalQ     = false;
	// bool sharpQ       = false;
	// bool flatQ        = false;
	// bool doubleflatQ  = false;
	// bool doublesharpQ = false;

	if (nodeType(m_node, "forward")) {
		rest = true;
		forceInvisible();
	} else {
		while (child) {
			if (nodeType(child, "rest")) {
				rest = true;
				break;
			}
			if (nodeType(child, "pitch")) {
				xml_node grandchild = child.first_child();
				while (grandchild) {
					if (nodeType(grandchild, "step")) {
						step = grandchild.child_value();
					} else if (nodeType(grandchild, "alter")) {
						alter = atoi(grandchild.child_value());
					} else if (nodeType(grandchild, "octave")) {
						octave = atoi(grandchild.child_value());
					}
					grandchild = grandchild.next_sibling();
				}
			} else if (nodeType(child, "accidental")) {
				if (strcmp(child.child_value(), "natural") == 0) {
					naturalQ = true;
					explicitQ = true;
				} else if (strcmp(child.child_value(), "sharp") == 0) {
					// sharpQ = true;
					explicitQ = true;
				} else if (strcmp(child.child_value(), "flat") == 0) {
					// flatQ = true;
					explicitQ = true;
				} else if (strcmp(child.child_value(), "double-flat") == 0) {
					// doubleflatQ = true;
					explicitQ = true;
				} else if (strcmp(child.child_value(), "double-sharp") == 0) {
					// doublesharpQ = true;
					explicitQ = true;
				}
			}
			child = child.next_sibling();
		}
	}

	if (rest) {
		return "r";
	}

	int count = 1;
	char pc = 'X';
	if (step.size() > 0) {
		pc = step[0];
	}
	if (octave > 3) {
		pc = tolower(pc);
		count = octave - 3;
	} else {
		pc = toupper(pc);
		count = 4 - octave;
	}
	string output;
	for (int i=0; i<count; i++) {
		output += pc;
	}
	if (alter > 0) {  // sharps
		for (int i=0; i<alter; i++) {
			output += '#';
		}
	} else if (alter < 0) { // flats
		for (int i=0; i>alter; i--) {
			output += '-';
		}
	}
	if (naturalQ) {
		output += 'n';
	} else if (explicitQ) {
		output += 'X';
	}

	return output;
}



//////////////////////////////
//
// MxmlEvent::getPrefixNoteInfo --
//

string MxmlEvent::getPrefixNoteInfo(void) const {
	int tiestart = 0;
	int tiestop  = 0;
	// bool rest    = false;

	xml_node child = m_node.first_child();

	while (child) {
		if (nodeType(child, "rest")) {
			// rest = true;
		} else if (nodeType(child, "tie")) {
			xml_attribute tietype = child.attribute("type");
			if (tietype) {
				if (strcmp(tietype.value(), "start") == 0) {
					tiestart = 1;
				} else if (strcmp(tietype.value(), "stop") == 0) {
					tiestop = 1;
				}
			}
		}
		child = child.next_sibling();
	}

	stringstream ss;

	if (tiestart && !tiestop) {
		ss << "[";
	}

	return ss.str();
}



//////////////////////////////
//
// MxmlEvent::getPostfixNoteInfo --
//

string MxmlEvent::getPostfixNoteInfo(bool primarynote) const {
	int beamstarts   = 0;
	int beamends     = 0;
	int beamconts    = 0;
	int hookbacks    = 0;
	int hookforwards = 0;
	int stem         = 0;
	int tiestart     = 0;
	int tiestop      = 0;

	// bool rest = false;
	xml_node child = m_node.first_child();
	xml_node notations;

	while (child) {
		if (nodeType(child, "rest")) {
			// rest = true;
		} else if (strcmp(child.name(), "beam") == 0) {
			const char* beaminfo = child.child_value();
			if (strcmp(beaminfo, "begin") == 0) {
				beamstarts++;
			} else if (strcmp(beaminfo, "end") == 0) {
				beamends++;
			} else if (strcmp(beaminfo, "continue") == 0) {
				beamconts++;
			} else if (strcmp(beaminfo, "forward hook") == 0) {
				hookforwards++;
			} else if (strcmp(beaminfo, "backward hook") == 0) {
				hookbacks++;
			}
		} else if (nodeType(child, "stem")) {
			const char* stemdir = child.child_value();
			if (strcmp(stemdir, "up") == 0) {
				stem = 1;
			} else if (strcmp(stemdir, "down") == 0) {
				stem = -1;
			}
		} else if (nodeType(child, "notations")) {
			notations = child;
		} else if (nodeType(child, "tie")) {
			xml_attribute tietype = child.attribute("type");
			if (tietype) {
				if (strcmp(tietype.value(), "start") == 0) {
					tiestart = 1;
				} else if (strcmp(tietype.value(), "stop") == 0) {
					tiestop = 1;
				}
			}
		}
		child = child.next_sibling();
	}

	stringstream ss;

	addNotations(ss, notations);

	if (primarynote) {
		// only add these signifiers if this is the first
		// note in a chord.  This is mostly important for
		// beam descriptions, as there can be only one beam
		// for each chord in a **kern token.  stems are not
		// given since they are not needed for secondary
		// chord notes (but nothing bad will happen if they
		// are included on secondary notes.
		switch (stem) {
			case  1:	ss << '/'; break;
			case -1:	ss << '\\'; break;
		}
		int i;
		for (i=0; i<beamends; i++)     { ss << "J"; }
		for (i=0; i<hookbacks; i++)    { ss << "k"; }
		for (i=0; i<hookforwards; i++) { ss << "K"; }
		for (i=0; i<beamstarts; i++)   { ss << "L"; }
	}

	if (tiestart && tiestop) {
		ss << "_";
	} else if (tiestop) {
		ss << "]";
	}

	return ss.str();
}



//////////////////////////////
//
// MxmlEvent::addNotations --
// see: http://www.music-cog.ohio-state.edu/Humdrum/representations/kern.html
//
// Others to add:
//   Mordent
//   Inverted mordent
//   Turn
//   Inverted turn (Wagnerian turn)
//   TrillTurn (TR or tR).
//

void MxmlEvent::addNotations(stringstream& ss, xml_node notations) const {
	if (!notations) {
		return;
	}

	xml_node child = notations.first_child();
	xml_node grandchild;

	bool staccato       = false;
	bool staccatissimo  = false;
	bool accent         = false;
	bool tenuto         = false;
	bool strongaccent   = false;
	bool fermata        = false;
	bool trill          = false;
	bool upbow          = false;
	bool downbow        = false;
	bool harmonic       = false;

	while (child) {
		if (strcmp(child.name(), "articulations") == 0) {
			grandchild = child.first_child();
			while (grandchild) {
				if (strcmp(grandchild.name(), "staccato") == 0) {
					staccato = true;
				} else if (strcmp(grandchild.name(), "staccatissimo") == 0) {
					staccatissimo = true;
				} else if (strcmp(grandchild.name(), "spiccato") == 0) {
					staccatissimo = true;
				} else if (strcmp(grandchild.name(), "accent") == 0) {
					accent = true;
				} else if (strcmp(grandchild.name(), "tenuto") == 0) {
					tenuto = true;
				} else if (strcmp(grandchild.name(), "strong-accent") == 0) {
					strongaccent = true;
				} else if (strcmp(grandchild.name(), "detached-legato") == 0) {
					tenuto = true;
					staccato = true;
				}
				grandchild = grandchild.next_sibling();
			}
		} else if (strcmp(child.name(), "technical") == 0) {
			// usermanuals.musicxml.com/MusicXML/Content/CT-MusicXML-technical.htm
			grandchild = child.first_child();
			while (grandchild) {
				if (strcmp(grandchild.name(), "up-bow") == 0) {
					upbow = true;
				} else if (strcmp(grandchild.name(), "down-bow") == 0) {
					downbow = true;
				}
				grandchild = grandchild.next_sibling();
			}
		} else if (strcmp(child.name(), "ornaments") == 0) {
			grandchild = child.first_child();
			while (grandchild) {
				if (strcmp(grandchild.name(), "trill-mark") == 0) {
					trill = true;
				}
				grandchild = grandchild.next_sibling();
			}
		} else if (strcmp(child.name(), "fermata") == 0) {
			fermata = true;
		}

		child = child.next_sibling();
	}

	if (staccato)     { ss << "'";  }
	if (staccatissimo){ ss << "`";  }
	if (tenuto)       { ss << "~";  }
	if (accent)       { ss << "^";  }
	if (strongaccent) { ss << "^^"; }  // might be something else
	if (harmonic)     { ss << "o";  }
	if (trill)        { ss << "t";  }  // figure out whole-tone trills later
	if (fermata)      { ss << ";";  }
	if (upbow)        { ss << "v";  }
	if (downbow)      { ss << "u";  }

}



//////////////////////////////
//
// MxmlEvent::getNode --
//

xml_node MxmlEvent::getNode(void) {
	return m_node;
}



//////////////////////////////
//
// MxmlEvent::getElementName --
//

string MxmlEvent::getElementName(void) {
	if (m_node) {
		string name = m_node.name();
		return name;
	} else {
		return "NULL";
	}
}



//////////////////////////////
//
// MxmlEvent::getHNode -- Return <harmony> element.
//

xml_node MxmlEvent::getHNode(void) {
	return m_hnode;
}



///////////////////////////////////////////////////////////////////////////
//
// private functions --
//

//////////////////////////////
//
// MxmlEvent::reportStaffNumberToOwner --
//

void MxmlEvent::reportStaffNumberToOwner(int staffnum, int voicenum) {
	if (m_owner != NULL) {
		m_owner->receiveStaffNumberFromChild(staffnum, voicenum);
	}
}



//////////////////////////////
//
// MxmlEvent::reportTimeSigDurToOwner --
//

void MxmlEvent::reportTimeSigDurToOwner(HumNum duration) {
	if (m_owner != NULL) {
		m_owner->receiveTimeSigDurFromChild(duration);
	}
}



//////////////////////////////
//
//  MxmlEvent::getDotCount -- return the number of augmentation dots
//     which are children of the given event element.  Returns -1
//     if the dot count should be calculated for a duration (such as whole
//     measure rests).
//

int MxmlEvent::getDotCount(void) const {
	xml_node child = m_node.first_child();
	int output = 0;
	bool foundType = false;
	while (child) {
		if (nodeType(child, "type")) {
			foundType = true;
		}
		if (output && !nodeType(child, "dot")) {
			return output;
		}
		if (strcmp(child.name(), "dot") == 0) {
			output++;
		}
		child = child.next_sibling();
	}
	if (foundType) {
		return output;
	} else {
		return -1;
	}
}



///////////////////////////////////////////////////////////////////////////
//
// static functions --
//

//////////////////////////////
//
// MxmlEvent::getEmbeddedDuration -- Given a <note>, return the
//   expeceded duration of the note, not from the <duration>, but
//   from a combination of <type> <dot>s and <time-modification>.
//   This value should match <duration>, but Sibelius has a buggy
//   <divisions> value so there can be round-off errors in the
//   duration of notes in MusicXML output from Sibelius.
//

HumNum MxmlEvent::getEmbeddedDuration(xml_node el) {
	if (!el) {
		return 0;
	}
	xml_node child = el.first_child();
   int dots          = 0;  // count of <dot /> elements
   HumNum type       = 0;  // powoftwo note type (as duration)
   bool tuplet       = false;  // is a tuplet
   int actualnotes   = 1;      // numerator of tuplet factor
   int normalnotes   = 1;      // denominator of tuplet factor
   HumNum normaltype = 0;      // poweroftwo duration of tuplet
   int tupdots       = 0;      // dots of "normal type" duration
	HumNum tfactor    = 1;

	while (child) {
		if (strcmp(child.name(), "dot") == 0) {
			dots++;
		} else if (strcmp(child.name(), "type") == 0) {
			type = getQuarterDurationFromType(child.child_value());
		} else if (strcmp(child.name(), "time-modification") == 0) {
			xml_node grandchild = child.first_child();
			normaltype = type;
			tuplet = true;
			while (grandchild) {
				if (strcmp(grandchild.name(), "actual-notes") == 0) {
					actualnotes = atoi(grandchild.child_value());
				} else if (strcmp(grandchild.name(), "normal-notes") == 0) {
					normalnotes = atoi(grandchild.child_value());
				}
				grandchild = grandchild.next_sibling();
			}
         // no duration information after <time-modification> so exit
			// outer loop now.
			break;
		} else if (strcmp(child.name(), "normal-dot") == 0) {
			tupdots++;
		}
		child = child.next_sibling();
	}

	HumNum duration = type;
	if (dots) {
		HumNum newdur = duration;
		for (int i=0; i<dots; i++) {
			newdur += duration / (1 << (i+1));
		}
		duration = newdur;
	}
	if (tuplet) {
		HumNum modification(actualnotes, normalnotes);
		duration /= modification;
      if (normaltype != type) {
			cerr << "Warning: cannot handle this tuplet type yet" << endl;
		}
      if (tupdots != 0) {
			cerr << "Warning: cannot handle this tuplet dots yet" << endl;
		}
	}

	return duration;
}



////////////////////////////////////////
//
// MxmlEvent::getQuarterDurationFromType --
//

HumNum MxmlEvent::getQuarterDurationFromType(const char* type) {
	if      (strcmp(type, "quarter") == 0) { return 1;              }
	else if (strcmp(type, "eighth") == 0)  { return HumNum(1, 2);   }
	else if (strcmp(type, "half") == 0)    { return 2;              }
	else if (strcmp(type, "16th") == 0)    { return HumNum(1, 4);   }
	else if (strcmp(type, "whole") == 0)   { return 4;              }
	else if (strcmp(type, "32nd") == 0)    { return HumNum(1, 8);   }
	else if (strcmp(type, "64th") == 0)    { return HumNum(1, 16);  }
	else if (strcmp(type, "128th") == 0)   { return HumNum(1, 32);  }
	else if (strcmp(type, "256th") == 0)   { return HumNum(1, 64);  }
	else if (strcmp(type, "512th") == 0)   { return HumNum(1, 128); }
	else if (strcmp(type, "1024th") == 0)  { return HumNum(1, 256); }
	else if (strcmp(type, "breve") == 0)   { return 8;              }
	else if (strcmp(type, "long") == 0)    { return 16;             }
	else if (strcmp(type, "maxima") == 0)  { return 32;             }
	else {
		cerr << "Error: Unknown note type: " << type << endl;
		return 0;
	}
}


//////////////////////////////
//
// MxmlEvent::nodeType -- return true if node type matches string.
//

bool MxmlEvent::nodeType(xml_node node, const char* testname) {
	if (strcmp(node.name(), testname) == 0) {
		return true;
	} else {
		return false;
	}
}


} // end namespace hum



