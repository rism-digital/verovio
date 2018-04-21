//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov 26 14:12:01 PST 1999
// Last Modified: Sat Apr 21 10:52:19 PDT 2018 Removed using namespace std;
// Filename:      midifile/include/MidiFile.h
// Website:       http://midifile.sapp.org
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   A class that can read/write Standard MIDI files.
//                MIDI data is stored by track in an array.
//

#ifndef _MIDIFILE_H_INCLUDED
#define _MIDIFILE_H_INCLUDED

#include "MidiEventList.h"

#include <vector>
#include <string>
#include <istream>
#include <fstream>

#define TIME_STATE_DELTA       0
#define TIME_STATE_ABSOLUTE    1

#define TRACK_STATE_SPLIT      0
#define TRACK_STATE_JOINED     1

namespace smf {

class _TickTime {
	public:
		int    tick;
		double seconds;
};


class MidiFile {
	public:
		               MidiFile                    (void);
		               MidiFile                    (const char* aFile);
		               MidiFile                    (const std::string& aFile);
		               MidiFile                    (std::istream& input);
		               MidiFile                    (const MidiFile& other);
		               MidiFile                    (MidiFile&& other);
		              ~MidiFile                    ();

		MidiFile&      operator=                   (MidiFile other);

		// reading/writing functions:
		int            read                        (const char* aFile);
		int            read                        (const std::string& aFile);
		int            read                        (std::istream& instream);
		int            write                       (const char* aFile);
		int            write                       (const std::string& aFile);
		int            write                       (std::ostream& out);
		int            writeHex                    (const char* aFile,
		                                            int width = 25);
		int            writeHex                    (const std::string& aFile,
		                                            int width = 25);
		int            writeHex                    (std::ostream& out,
		                                            int width = 25);
		int            writeBinasc                 (const char* aFile);
		int            writeBinasc                 (const std::string& aFile);
		int            writeBinasc                 (std::ostream& out);
		int            writeBinascWithComments     (const char* aFile);
		int            writeBinascWithComments     (const std::string& aFile);
		int            writeBinascWithComments     (std::ostream& out);
		int            status                      (void);

		// track-related functions:
		const MidiEventList& operator[]            (int aTrack) const;
		MidiEventList&   operator[]                (int aTrack);
		int              getTrackCount             (void) const;
		int              getNumTracks              (void) const;
		int              size                      (void) const;
		void             removeEmpties             (void);

		// tick-related functions:
		void             deltaTicks                (void);
		void             absoluteTicks             (void);
		int              getTickState              (void);
		int              isDeltaTicks              (void);
		int              isAbsoluteTicks           (void);
		int              getMaxTick                (void);

		// join/split track functionality:
		void             joinTracks                (void);
		void             splitTracks               (void);
		void             splitTracksByChannel      (void);
		int              getTrackState             (void);
		int              hasJoinedTracks           (void);
		int              hasSplitTracks            (void);
		int              getSplitTrack             (int track, int index);
		int              getSplitTrack             (int index);

		// track sorting funcionality:
		void             sortTrack                 (int track);
		void             sortTracks                (void);
		void             markSequence              (void);
		void             markSequence              (int track, int sequence = 1);
		void             clearSequence             (void);
		void             clearSequence             (int track);

		// track manipulation functionality:
		int              addTrack                  (void);
		int              addTrack                  (int count);
		int              addTracks                 (int count);
		void             deleteTrack               (int aTrack);
		void             mergeTracks               (int aTrack1, int aTrack2);
		int              getTrackCountAsType1      (void);

		// ticks-per-quarter related functions:
		void             setMillisecondTicks       (void);
		int              getTicksPerQuarterNote    (void);
		int              getTPQ                    (void);
		void             setTicksPerQuarterNote    (int ticks);
		void             setTPQ                    (int ticks);

		// physical-time analysis functions:
		void             doTimeAnalysis            (void);
		double           getTimeInSeconds          (int aTrack, int anIndex);
		double           getTimeInSeconds          (int tickvalue);
		double           getAbsoluteTickTime       (double starttime);
		double           getTotalTimeInSeconds     (void);
		int              getTotalTimeInTicks       (void);
		double           getTotalTimeInQuarters    (void);

		// note-analysis functions:
		int              linkNotePairs             (void);
		int              linkEventPairs            (void);
		void             clearLinks                (void);

		// filename functions:
		void             setFilename               (const char* aname);
		void             setFilename               (const std::string& aname);
		const char*      getFilename               (void);

		// event functionality:
		MidiEvent*       addEvent                  (int aTrack, int aTick,
		                                            std::vector<uchar>& midiData);
		MidiEvent*       addEvent                  (MidiEvent& mfevent);
		MidiEvent&       getEvent                  (int aTrack, int anIndex);
		int              getEventCount             (int aTrack);
		int              getNumEvents              (int aTrack);
		void             allocateEvents            (int track, int aSize);
		void             erase                     (void);
		void             clear                     (void);
		void             clear_no_deallocate       (void);

		// MIDI message adding convenience functions:
		MidiEvent*        addNoteOn               (int aTrack, int aTick,
		                                           int aChannel, int key,
		                                           int vel);
		MidiEvent*        addNoteOff              (int aTrack, int aTick,
		                                           int aChannel, int key,
		                                           int vel);
		MidiEvent*        addNoteOff              (int aTrack, int aTick,
		                                           int aChannel, int key);
		MidiEvent*        addController           (int aTrack, int aTick,
		                                           int aChannel, int num,
		                                           int value);
		MidiEvent*        addPatchChange          (int aTrack, int aTick,
		                                           int aChannel, int patchnum);
		MidiEvent*        addTimbre               (int aTrack, int aTick,
		                                           int aChannel, int patchnum);
		MidiEvent*        addPitchBend            (int aTrack, int aTick,
		                                           int aChannel, double amount);

		// Controller message adding convenience functions:
		MidiEvent*        addSustain              (int aTrack, int aTick,
		                                           int aChannel, int value);
		MidiEvent*        addSustainPedal         (int aTrack, int aTick,
		                                           int aChannel, int value);
		MidiEvent*        addSustainOn            (int aTrack, int aTick,
		                                           int aChannel);
		MidiEvent*        addSustainPedalOn       (int aTrack, int aTick,
		                                           int aChannel);
		MidiEvent*        addSustainOff           (int aTrack, int aTick,
		                                           int aChannel);
		MidiEvent*        addSustainPedalOff      (int aTrack, int aTick,
		                                           int aChannel);

		// Meta-event adding convenience functions:
		MidiEvent*      addMetaEvent              (int aTrack, int aTick,
		                                           int aType,
		                                           std::vector<uchar>& metaData);
		MidiEvent*      addMetaEvent              (int aTrack, int aTick,
		                                           int aType,
		                                           const std::string& metaData);
		MidiEvent*      addText                   (int aTrack, int aTick,
		                                           const std::string& text);
		MidiEvent*      addCopyright              (int aTrack, int aTick,
		                                           const std::string& text);
		MidiEvent*      addTrackName              (int aTrack, int aTick,
		                                           const std::string& name);
		MidiEvent*      addInstrumentName         (int aTrack, int aTick,
		                                           const std::string& name);
		MidiEvent*      addLyric                  (int aTrack, int aTick,
		                                           const std::string& text);
		MidiEvent*      addMarker                 (int aTrack, int aTick,
		                                           const std::string& text);
		MidiEvent*      addCue                    (int aTrack, int aTick,
		                                           const std::string& text);
		MidiEvent*      addTempo                  (int aTrack, int aTick,
		                                           double aTempo);
		MidiEvent*      addTimeSignature          (int aTrack, int aTick,
		                                           int top, int bottom,
		                                           int clocksPerClick = 24,
		                                           int num32dsPerQuarter = 8);
		MidiEvent*      addCompoundTimeSignature  (int aTrack, int aTick,
		                                           int top, int bottom,
		                                           int clocksPerClick = 36,
		                                           int num32dsPerQuarter = 8);

		uchar           readByte                  (std::istream& input);

		// static functions:
		static ushort        readLittleEndian2Bytes  (std::istream& input);
		static ulong         readLittleEndian4Bytes  (std::istream& input);
		static std::ostream& writeLittleEndianUShort (std::ostream& out,
		                                              ushort value);
		static std::ostream& writeBigEndianUShort    (std::ostream& out,
		                                              ushort value);
		static std::ostream& writeLittleEndianShort  (std::ostream& out,
		                                              short value);
		static std::ostream& writeBigEndianShort     (std::ostream& out,
		                                              short value);
		static std::ostream& writeLittleEndianULong  (std::ostream& out,
		                                              ulong value);
		static std::ostream& writeBigEndianULong     (std::ostream& out,
		                                              ulong value);
		static std::ostream& writeLittleEndianLong   (std::ostream& out,
		                                              long value);
		static std::ostream& writeBigEndianLong      (std::ostream& out,
		                                              long value);
		static std::ostream& writeLittleEndianFloat  (std::ostream& out,
		                                              float value);
		static std::ostream& writeBigEndianFloat     (std::ostream& out,
		                                              float value);
		static std::ostream& writeLittleEndianDouble (std::ostream& out,
		                                              double value);
		static std::ostream& writeBigEndianDouble    (std::ostream& out,
		                                              double value);

	protected:
		std::vector<MidiEventList*> m_events;         // MIDI file events
		int                         m_ticksPerQuarterNote; // time base of file
		int                         m_trackCount;     // # of tracks in file
		int                         m_theTrackState;  // joined or split
		int                         m_theTimeState;   // absolute or delta
		std::string                 m_readFileName;   // read file name
		int                         m_timemapvalid;
		std::vector<_TickTime>      m_timemap;
		int                         m_rwstatus;       // read/write success flag

	private:
		int        extractMidiData                 (std::istream& inputfile,
		                                            std::vector<uchar>& array,
		                                            uchar& runningCommand);
		ulong      readVLValue                     (std::istream& inputfile);
		ulong      unpackVLV                       (uchar a = 0, uchar b = 0,
		                                            uchar c = 0, uchar d = 0,
		                                            uchar e = 0);
		void       writeVLValue                    (long aValue,
		                                            std::vector<uchar>& data);
		int        makeVLV                         (uchar *buffer, int number);
		static int ticksearch                      (const void* A, const void* B);
		static int secondsearch                    (const void* A, const void* B);
		void       buildTimeMap                    (void);
		double     linearTickInterpolationAtSecond (double seconds);
		double     linearSecondInterpolationAtTick (int ticktime);
};

} // end of namespace smf

std::ostream& operator<<(std::ostream& out, smf::MidiFile& aMidiFile);

#endif /* _MIDIFILE_H_INCLUDED */



