//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Feb 14 21:55:38 PST 2015
// Last Modified: Sat Feb 14 21:55:40 PST 2015
// Filename:      midifile/src-library/MidiEventList.cpp
// Website:       http://midifile.sapp.org
// Syntax:        C++11
// vim:           ts=3 expandtab
//
// Description:   A class which stores a MidiEvents for a MidiFile track.
//


#include "MidiEventList.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;


//////////////////////////////
//
// MidiEventList::MidiEventList -- Constructor.
//

MidiEventList::MidiEventList(void) {
   reserve(1000);
}



//////////////////////////////
//
// MidiEventList::MidiEventList(MidiEventList&) -- Copy constructor.
//

MidiEventList::MidiEventList(const MidiEventList& other) {
   list.reserve(other.list.size());
   auto it = other.list.begin();
   std::generate_n(std::back_inserter(list), other.list.size(), [&]() -> MidiEvent* {
      return new MidiEvent(**it++);
   });
}



//////////////////////////////
//
// MidiEventList::MidiEventList(MidiEventList&&) -- Move constructor.
//

MidiEventList::MidiEventList(MidiEventList&& other) {
    list = std::move(other.list);
    other.list.clear();
}



//////////////////////////////
//
// MidiEventList::~MidiEventList -- Deconstructor.  Deallocate all stored
//   data.
//

MidiEventList::~MidiEventList() {
   clear();
}



//////////////////////////////
//
// MidiEventList::operator[] --
//

MidiEvent&  MidiEventList::operator[](int index) {
   return *list[index];
}



//////////////////////////////
//
// MidiEventList::back -- Return the last element in the list.
//

MidiEvent& MidiEventList::back(void) {
   return *list.back();
}

// Alias for back:

MidiEvent& MidiEventList::last(void) {
   return back();
}



//////////////////////////////
//
// MidiEventList::getEvent -- The same thing as operator[], for 
//      internal use when operator[] would look more messy.
//   

MidiEvent& MidiEventList::getEvent(int index) {
     return *list[index];
}



//////////////////////////////
//
// MidiEventList::clear -- De-allocate any MidiEvents present in the list
//    and set the size of the list to 0.
//

void MidiEventList::clear(void) {
   for (int i=0; i<(int)list.size(); i++) {
      if (list[i] != NULL) {
         delete list[i];
         list[i] = NULL;
      }
   }
   list.resize(0);
}



//////////////////////////////
//
// MidiEventList::data --
//

MidiEvent** MidiEventList::data(void) {
   return list.data();
}



//////////////////////////////
//
// MidiEventList::reserve --  Pre-allocate space in the list for storing
//     elements.
//

void MidiEventList::reserve(int rsize) {
   if (rsize > (int)list.size()) {
      list.reserve(rsize);
   }
}


//////////////////////////////
//
// MidiEventList::getSize --
//

int MidiEventList::getSize(void) {
   return (int)list.size();
}


int MidiEventList::size(void) {
   return getSize();
}



//////////////////////////////
//
// MidiEventList::append -- add a MidiEvent at the end of the list.  Returns
//     the index of the appended event.
//

int MidiEventList::append(MidiEvent& event) { 
   MidiEvent* ptr = new MidiEvent(event);
   list.push_back(ptr);
   return (int)list.size()-1;
}


int MidiEventList::push(MidiEvent& event) { 
   return append(event);
}


int MidiEventList::push_back(MidiEvent& event) { 
   return append(event);
}


//////////////////////////////
//
// MidiEventList::linkNotePairs -- Match note-ones and note-offs together
//   There are two models that can be done if two notes are overlapping
//   on the same pitch: the first note-off affects the last note-on,
//   or the first note-off affects the first note-on.  Currently  the 
//   first note-off affects the last note-on, but both methods could
//   be implemented with user selectability.  The current state of the
//   track is assumed to be in time-sorted order.  Returns the number
//   of linked notes (note-on/note-off pairs).
//

int MidiEventList::linkNotePairs(void) {
   // dimension 1: MIDI channel (0-15)
   // dimension 2: MIDI key     (0-127)  (but 0 not used for note-ons)
   // dimension 3: List of active note-ons or note-offs.
   vector<vector<vector<MidiEvent*> > > noteons;
   noteons.resize(16);
   int i;
   for (i=0; i<(int)noteons.size(); i++) {
      noteons[i].resize(128);
   }

   // Now iterate through the MidiEventList keeping track of note
   // states and linking notes as needed.
   int channel;
   int key;
   int counter = 0;
   MidiEvent* mev;
   MidiEvent* noteon;
   for (i=0; i<getSize(); i++) {
      mev = &getEvent(i);
      mev->unlinkEvent();
      if (mev->isNoteOn()) {
         // store the note-on to pair later with a note-off message.
         key = mev->getKeyNumber();
         channel = mev->getChannel();
         noteons[channel][key].push_back(mev);
      } else if (mev->isNoteOff()) {
         key = mev->getKeyNumber();
         channel = mev->getChannel();
         if (noteons[channel][key].size() > 0) {
            noteon = noteons[channel][key].back();
            noteons[channel][key].pop_back();
            noteon->linkEvent(mev);
            counter++;
         }
      }
   }
   return counter;
}



//////////////////////////////
//
// MidiEventList::clearLinks -- remove all note-on/note-off links.
//

void MidiEventList::clearLinks(void) {
   for (int i=0; i<(int)getSize(); i++) {
      getEvent(i).unlinkEvent();
   }
}


///////////////////////////////////////////////////////////////////////////
//
// protected functions --
//


//////////////////////////////
//
// MidiEventList::detach -- De-allocate any MidiEvents present in the list
//    and set the size of the list to 0.
//


void MidiEventList::detach(void) {
   list.resize(0);
}



//////////////////////////////
//
// MidiEventList::push_back_no_copy -- add a MidiEvent at the end of 
//     the list.  The event is not copied, but memory from the 
//     remote location is used.  Returns the index of the appended event.
//

int MidiEventList::push_back_no_copy(MidiEvent* event) { 
   list.push_back(event);
   return (int)list.size()-1;
}



//////////////////////////////
//
// MidiEventList::operator=(MidiEventList) -- Assignment.
//

MidiEventList& MidiEventList::operator=(MidiEventList other) {
   list.swap(other.list);
   return *this;
}


