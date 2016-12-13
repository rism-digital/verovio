//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Feb 14 20:36:32 PST 2015
// Last Modified: Sun Feb 15 20:32:19 PST 2015
// Filename:      midifile/include/MidiMessage.h
// Website:       http://midifile.sapp.org
// Syntax:        C++11
// vim:           ts=3 expandtab
//
// Description:   Storage for bytes of a MIDI message for use in MidiFile
//                class.
//

#ifndef _MIDIMESSAGE_H_INCLUDED
#define _MIDIMESSAGE_H_INCLUDED

#include <vector>
#include <string>

using namespace std;

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned long  ulong;

class MidiMessage : public vector<uchar> {
	public:
		               MidiMessage          (void);
		               MidiMessage          (int command);
		               MidiMessage          (int command, int p1);
		               MidiMessage          (int command, int p1, int p2);
                     MidiMessage          (const MidiMessage& message);
                     MidiMessage          (const vector<uchar>& message);
                     MidiMessage          (const vector<char>& message);
                     MidiMessage          (const vector<int>& message);

                    ~MidiMessage         ();

      MidiMessage&   operator=            (const MidiMessage& message);
      MidiMessage&   operator=            (const vector<uchar>& bytes);
      MidiMessage&   operator=            (const vector<char>& bytes);
      MidiMessage&   operator=            (const vector<int>& bytes);
      void           setSize              (int asize);
      int            getSize              (void) const;
      int            setSizeToCommand     (void);
      int            resizeToCommand      (void);

      int            getTempoMicro        (void) const;
      int            getTempoMicroseconds (void) const;
      double         getTempoSeconds      (void) const;
      double         getTempoBPM          (void) const;
      double         getTempoTPS          (int tpq) const;
      double         getTempoSPT          (int tpq) const;

      int            isMetaMessage        (void) const;
      int            isMeta               (void) const;
      int            isNoteOff            (void) const;
      int            isNoteOn             (void) const;
      int            isNote               (void) const;
      int            isAftertouch         (void) const;
      int            isController         (void) const;
      int            isTimbre             (void) const;
      int            isPatchChange        (void) const;
      int            isPressure           (void) const;
      int            isPitchbend          (void) const;

      int            getP0                (void) const;
      int            getP1                (void) const;
      int            getP2                (void) const;
      int            getP3                (void) const;
      int            getKeyNumber         (void) const;
      int            getVelocity          (void) const;

      void           setP0                (int value);
      void           setP1                (int value);
      void           setP2                (int value);
      void           setP3                (int value);
      void           setKeyNumber         (int value);
      void           setVelocity          (int value);

      int            getCommandNibble     (void) const;
      int            getCommandByte       (void) const;
      int            getChannelNibble     (void) const;
      int            getChannel           (void) const;

      void           setCommandByte       (int value);
      void           setCommand           (int value);
      void           setCommand           (int value, int p1);
      void           setCommand           (int value, int p1, int p2);
      void           setCommandNibble     (int value);
      void           setChannelNibble     (int value);
      void           setChannel           (int value);
      void           setParameters        (int p1, int p2);
      void           setParameters        (int p1);
      void           setMessage           (const vector<uchar>& message);
      void           setMessage           (const vector<char>& message);
      void           setMessage           (const vector<int>& message);

      void           setSpelling          (int base7, int accidental);
      void           getSpelling          (int& base7, int& accidental);

      // helper functions to create various MidiMessages
      void           makeNoteOn           (int channel, int key, int velocity);
      void           makeNoteOff          (int channel, int key, int velocity);
      void           makeNoteOff          (int channel, int key);
      void           makeNoteOff          (void);
      void           makeController       (int channel, int num, int value);
      void           makePatchChange      (int channel, int patchnum);
      void           makeTimbre           (int channel, int patchnum);

      // meta-message creation helper functions:
      void           makeMetaMessage      (int mnum, const string& data);
      void           makeTrackName        (const string& name);
      void           makeInstrumentName   (const string& name);
      void           makeLyric            (const string& text);
      void           makeMarker           (const string& text);
      void           makeCue              (const string& text);
      void           makeCopyright        (const string& text);
      void           makeTempo            (double tempo) { setTempo(tempo); }
      void           makeTimeSignature    (int top, int bottom,
	                                        int clocksPerClick = 24,
                                           int num32dsPerQuarter = 8);

      // meta-message related functions:
      int            getMetaType          (void) const;
      int            isTempo              (void) const;
      void           setTempo             (double tempo);
      void           setTempoMicroseconds (int microseconds);
      void           setMetaTempo         (double tempo);
      int            isEndOfTrack         (void) const;

};


#endif /* _MIDIMESSAGE_H_INCLUDED */



