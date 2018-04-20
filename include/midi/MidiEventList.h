//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Feb 14 21:55:38 PST 2015
// Last Modified: Sat Feb 14 21:55:40 PST 2015
// Filename:      midifile/include/MidiEventList.h
// Website:       http://midifile.sapp.org
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   A class which stores a MidiEvents for a MidiFile track.
//

#ifndef _MIDIEVENTLIST_H_INCLUDED
#define _MIDIEVENTLIST_H_INCLUDED

#include "MidiEvent.h"
#include <vector>

using namespace std;

class MidiEventList {
	public:
		                 MidiEventList    (void);
		                 MidiEventList    (const MidiEventList& other);
		                 MidiEventList    (MidiEventList&& other);

		                ~MidiEventList    ();

		MidiEvent&       operator[]       (int index);
		const MidiEvent& operator[]       (int index) const;
		MidiEvent&       back             (void);
		MidiEvent&       last             (void);
		MidiEvent&       getEvent         (int index);
		void             clear            (void);
		void             reserve          (int rsize);
		int              getSize          (void) const;
		int              getEventCount    (void) const;
		int              size             (void) const;
		void             removeEmpties    (void);
		int              linkNotePairs    (void);
		int              linkEventPairs   (void);
		void             clearLinks       (void);
		MidiEvent**      data             (void);
		void             clearSequence    (void);
		int              markSequence     (int sequence = 1);

		int              push             (MidiEvent& event);
		int              push_back        (MidiEvent& event);
		int              append           (MidiEvent& event);

		// careful when using these, intended for internal use in MidiFile class:
		void             detach              (void);
		int              push_back_no_copy   (MidiEvent* event);

		MidiEventList& operator=(MidiEventList other);

	protected:
		vector<MidiEvent*> list;

	private:
		void             sort                 (void);
		void             sortKeepSequence     (void);

	friend class MidiFile;

};


int eventcompare(const void* a, const void* b);

#endif /* _MIDIEVENTLIST_H_INCLUDED */



