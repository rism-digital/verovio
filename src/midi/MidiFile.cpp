//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri Nov 26 14:12:01 PST 1999
// Last Modified: Fri Dec  2 13:26:29 PST 1999
// Last Modified: Wed Dec 13 10:33:30 PST 2000 Modified sorting routine
// Last Modified: Tue Jan 22 23:23:37 PST 2002 Allowed reading of meta events
// Last Modified: Tue Nov  4 23:09:19 PST 2003 Adjust noteoff in eventcompare
// Last Modified: Tue Jun 29 09:43:10 PDT 2004 Fixed end-of-track problem
// Last Modified: Sat Dec 17 23:11:57 PST 2005 Added millisecond ticks
// Last Modified: Thu Sep 14 20:07:45 PDT 2006 Added SMPTE ASCII printing
// Last Modified: Tue Apr  7 09:23:48 PDT 2009 Added addMetaEvent.
// Last Modified: Fri Jun 12 22:58:34 PDT 2009 Renamed SigCollection class.
// Last Modified: Mon Jul 26 13:38:23 PDT 2010 Added timing in seconds.
// Last Modified: Tue Feb 22 13:26:40 PST 2011 Added write(ostream).
// Last Modified: Mon Nov 18 13:10:37 PST 2013 Added .printHex function.
// Last Modified: Mon Feb  9 12:22:18 PST 2015 Remove dep. on FileIO class.
// Last Modified: Sat Feb 14 23:40:17 PST 2015 Split out subclasses.
// Last Modified: Wed Feb 18 20:06:39 PST 2015 Added binasc MIDI read/write.
// Last Modified: Thu Mar 19 13:09:00 PDT 2015 Improve Sysex read/write.
// Last Modified: Fri Feb 19 00:32:39 PST 2016 Switch to Binasc stdout.
// Filename:      midifile/src/MidiFile.cpp
// Website:       http://midifile.sapp.org
// Syntax:        C++11
// vim:           ts=3 expandtab hlsearch
//
// Description:   A class which can read/write Standard MIDI files.
//                MIDI data is stored by track in an array.  This
//                class is used for example in the MidiPerform class.
//

#include "MidiFile.h"
#include "Binasc.h"

#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;


//////////////////////////////
//
// MidiFile::MidiFile -- Constuctor.
//

MidiFile::MidiFile(void) {
   ticksPerQuarterNote = 120;            // TPQ time base of file
   trackCount = 1;                       // # of tracks in file
   theTrackState = TRACK_STATE_SPLIT;    // joined or split
   theTimeState = TIME_STATE_ABSOLUTE;   // absolute or delta
   events.resize(1);
   events[0] = new MidiEventList;
   readFileName.resize(1);
   readFileName[0] = '\0';
   timemap.clear();
   timemapvalid = 0;
   rwstatus = 1;
}


MidiFile::MidiFile(const char* filename) {
   ticksPerQuarterNote = 120;            // TPQ time base of file
   trackCount = 1;                       // # of tracks in file
   theTrackState = TRACK_STATE_SPLIT;    // joined or split
   theTimeState = TIME_STATE_ABSOLUTE;   // absolute or delta
   events.resize(1);
   events[0] = new MidiEventList;
   readFileName.resize(1);
   readFileName[0] = '\0';
   read(filename);
   timemap.clear();
   timemapvalid = 0;
   rwstatus = 1;
}


MidiFile::MidiFile(const string& filename) {
   ticksPerQuarterNote = 120;            // TQP time base of file
   trackCount = 1;                       // # of tracks in file
   theTrackState = TRACK_STATE_SPLIT;    // joined or split
   theTimeState = TIME_STATE_ABSOLUTE;   // absolute or delta
   events.resize(1);
   events[0] = new MidiEventList;
   readFileName.resize(1);
   readFileName[0] = '\0';
   read(filename);
   timemap.clear();
   timemapvalid = 0;
   rwstatus = 1;
}


MidiFile::MidiFile(istream& input) {
   ticksPerQuarterNote = 120;            // TQP time base of file
   trackCount = 1;                       // # of tracks in file
   theTrackState = TRACK_STATE_SPLIT;    // joined or split
   theTimeState = TIME_STATE_ABSOLUTE;   // absolute or delta
   events.resize(1);
   events[0] = new MidiEventList;
   readFileName.resize(1);
   readFileName[0] = '\0';
   read(input);
   timemap.clear();
   timemapvalid = 0;
   rwstatus = 1;
}



//////////////////////////////
//
// MidiFile::MidiFile(MidiFile&) -- Copy constructor.
//

MidiFile::MidiFile(const MidiFile& other) {
   events.reserve(other.events.size());
   auto it = other.events.begin();
   std::generate_n(std::back_inserter(events), other.events.size(),
         [&]() -> MidiEventList* {
      return new MidiEventList(**it++);
   });

   ticksPerQuarterNote = other.ticksPerQuarterNote;
   trackCount = other.trackCount;
   theTrackState = other.theTrackState;
   theTimeState = other.theTimeState;
   readFileName = other.readFileName;

   timemapvalid = other.timemapvalid;
   timemap = other.timemap;
   rwstatus = other.rwstatus;
}



//////////////////////////////
//
// MidiFile::MidiFile(MidiFile&&) -- Move constructor.
//

MidiFile::MidiFile(MidiFile&& other) {
    events = std::move(other.events);
    other.events.clear();
    other.events.push_back(new MidiEventList);

   ticksPerQuarterNote = other.ticksPerQuarterNote;
   trackCount = other.trackCount;
   theTrackState = other.theTrackState;
   theTimeState = other.theTimeState;
   readFileName = other.readFileName;

   timemapvalid = other.timemapvalid;
   timemap = other.timemap;
   rwstatus = other.rwstatus;
}



//////////////////////////////
//
// MidiFile::~MidiFile -- Deconstructor.
//

MidiFile::~MidiFile() {
   readFileName.resize(1);
   readFileName[0] = '\0';
   clear();
   if (events[0] != NULL) {
      delete events[0];
      events[0] = NULL;
   }
   events.resize(0);
   rwstatus = 0;
   timemap.clear();
   timemapvalid = 0;
}


///////////////////////////////////////////////////////////////////////////
//
// reading/writing functions --
//

//////////////////////////////
//
// MidiFile::read -- Parse a Standard MIDI File and store its contents
//      in the object.
//

int MidiFile::read(const char* filename) {
   rwstatus = 1;
   timemapvalid = 0;
   if (filename != NULL) {
      setFilename(filename);
   }

   fstream input;
   input.open(filename, ios::binary | ios::in);

   if (!input.is_open()) {
      return 0;
   }

   rwstatus = MidiFile::read(input);
   return rwstatus;
}


//
// string version of read().
//


int MidiFile::read(const string& filename) {
   timemapvalid = 0;
   setFilename(filename);
   rwstatus = 1;

   fstream input;
   input.open(filename.data(), ios::binary | ios::in);

   if (!input.is_open()) {
      return 0;
   }

   rwstatus = MidiFile::read(input);
   return rwstatus;
}


//
// istream version of read().
//

int MidiFile::read(istream& input) {
   rwstatus = 1;
   if (input.peek() != 'M') {
      // If the first byte in the input stream is not 'M', then presume that
      // the MIDI file is in the binasc format which is an ASCII representation
      // of the MIDI file.  Convert the binasc content into binary content and
      // then continue reading with this function.
      stringstream binarydata;
      Binasc binasc;
      binasc.writeToBinary(binarydata, input);
      binarydata.seekg(0, ios_base::beg);
      if (binarydata.peek() != 'M') {
         cerr << "Bad MIDI data input" << endl;
         rwstatus = 0;
         return rwstatus;
      } else {
         rwstatus = read(binarydata);
         return rwstatus;
      }
   }

   const char* filename = getFilename();

   int    character;
   // uchar  buffer[123456] = {0};
   ulong  longdata;
   ushort shortdata;


   // Read the MIDI header (4 bytes of ID, 4 byte data size,
   // anticipated 6 bytes of data.

   character = input.get();
   if (character == EOF) {
      cerr << "In file " << filename << ": unexpected end of file." << endl;
      cerr << "Expecting 'M' at first byte, but found nothing." << endl;
      rwstatus = 0; return rwstatus;
   } else if (character != 'M') {
      cerr << "File " << filename << " is not a MIDI file" << endl;
      cerr << "Expecting 'M' at first byte but got '"
           << character << "'" << endl;
      rwstatus = 0; return rwstatus;
   }

   character = input.get();
   if (character == EOF) {
      cerr << "In file " << filename << ": unexpected end of file." << endl;
      cerr << "Expecting 'T' at first byte, but found nothing." << endl;
      rwstatus = 0; return rwstatus;
   } else if (character != 'T') {
      cerr << "File " << filename << " is not a MIDI file" << endl;
      cerr << "Expecting 'T' at first byte but got '"
           << character << "'" << endl;
      rwstatus = 0; return rwstatus;
   }

   character = input.get();
   if (character == EOF) {
      cerr << "In file " << filename << ": unexpected end of file." << endl;
      cerr << "Expecting 'h' at first byte, but found nothing." << endl;
      rwstatus = 0; return rwstatus;
   } else if (character != 'h') {
      cerr << "File " << filename << " is not a MIDI file" << endl;
      cerr << "Expecting 'h' at first byte but got '"
           << character << "'" << endl;
      rwstatus = 0; return rwstatus;
   }

   character = input.get();
   if (character == EOF) {
      cerr << "In file " << filename << ": unexpected end of file." << endl;
      cerr << "Expecting 'd' at first byte, but found nothing." << endl;
      rwstatus = 0; return rwstatus;
   } else if (character != 'd') {
      cerr << "File " << filename << " is not a MIDI file" << endl;
      cerr << "Expecting 'd' at first byte but got '"
           << character << "'" << endl;
      rwstatus = 0; return rwstatus;
   }

   // read header size (allow larger header size?)
   longdata = MidiFile::readLittleEndian4Bytes(input);
   if (longdata != 6) {
      cerr << "File " << filename
           << " is not a MIDI 1.0 Standard MIDI file." << endl;
      cerr << "The header size is " << longdata << " bytes." << endl;
      rwstatus = 0; return rwstatus;
   }

   // Header parameter #1: format type
   int type;
   shortdata = MidiFile::readLittleEndian2Bytes(input);
   switch (shortdata) {
      case 0:
         type = 0;
         break;
      case 1:
         type = 1;
         break;
      case 2:    // Type-2 MIDI files should probably be allowed as well.
      default:
         cerr << "Error: cannot handle a type-" << shortdata
              << " MIDI file" << endl;
         rwstatus = 0; return rwstatus;
   }

   // Header parameter #2: track count
   int tracks;
   shortdata = MidiFile::readLittleEndian2Bytes(input);
   if (type == 0 && shortdata != 1) {
      cerr << "Error: Type 0 MIDI file can only contain one track" << endl;
      cerr << "Instead track count is: " << shortdata << endl;
      rwstatus = 0; return rwstatus;
   } else {
      tracks = shortdata;
   }
   clear();
   if (events[0] != NULL) {
      delete events[0];
   }
   events.resize(tracks);
   for (int z=0; z<tracks; z++) {
      events[z] = new MidiEventList;
      events[z]->reserve(10000);   // Initialize with 10,000 event storage.
      events[z]->clear();
   }

   // Header parameter #3: Ticks per quarter note
   shortdata = MidiFile::readLittleEndian2Bytes(input);
   if (shortdata >= 0x8000) {
      int framespersecond = ((!(shortdata >> 8))+1) & 0x00ff;
      int resolution      = shortdata & 0x00ff;
      switch (framespersecond) {
         case 232:  framespersecond = 24; break;
         case 231:  framespersecond = 25; break;
         case 227:  framespersecond = 29; break;
         case 226:  framespersecond = 30; break;
         default:
               cerr << "Warning: unknown FPS: " << framespersecond << endl;
               framespersecond = 255 - framespersecond + 1;
               cerr << "Setting FPS to " << framespersecond << endl;
      }
      // actually ticks per second (except for frame=29 (drop frame)):
      ticksPerQuarterNote = shortdata;

      cerr << "SMPTE ticks: " << ticksPerQuarterNote << " ticks/sec" << endl;
      cerr << "SMPTE frames per second: " << framespersecond << endl;
      cerr << "SMPTE frame resolution per frame: " << resolution << endl;
   }  else {
      ticksPerQuarterNote = shortdata;
   }


   //////////////////////////////////////////////////
   //
   // now read individual tracks:
   //

   uchar runningCommand;
   MidiEvent event;
   vector<uchar> bytes;
   int absticks;
   int xstatus;
   // int barline;

   for (int i=0; i<tracks; i++) {
      runningCommand = 0;

      // cout << "\nReading Track: " << i + 1 << flush;

      // read track header...

      character = input.get();
      if (character == EOF) {
         cerr << "In file " << filename << ": unexpected end of file." << endl;
         cerr << "Expecting 'M' at first byte in track, but found nothing."
              << endl;
         rwstatus = 0; return rwstatus;
      } else if (character != 'M') {
         cerr << "File " << filename << " is not a MIDI file" << endl;
         cerr << "Expecting 'M' at first byte in track but got '"
              << character << "'" << endl;
         rwstatus = 0; return rwstatus;
      }

      character = input.get();
      if (character == EOF) {
         cerr << "In file " << filename << ": unexpected end of file." << endl;
         cerr << "Expecting 'T' at first byte in track, but found nothing."
              << endl;
         rwstatus = 0; return rwstatus;
      } else if (character != 'T') {
         cerr << "File " << filename << " is not a MIDI file" << endl;
         cerr << "Expecting 'T' at first byte in track but got '"
              << character << "'" << endl;
         rwstatus = 0; return rwstatus;
      }

      character = input.get();
      if (character == EOF) {
         cerr << "In file " << filename << ": unexpected end of file." << endl;
         cerr << "Expecting 'r' at first byte in track, but found nothing."
              << endl;
         rwstatus = 0; return rwstatus;
      } else if (character != 'r') {
         cerr << "File " << filename << " is not a MIDI file" << endl;
         cerr << "Expecting 'r' at first byte in track but got '"
              << character << "'" << endl;
         rwstatus = 0; return rwstatus;
      }

      character = input.get();
      if (character == EOF) {
         cerr << "In file " << filename << ": unexpected end of file." << endl;
         cerr << "Expecting 'k' at first byte in track, but found nothing."
              << endl;
         rwstatus = 0; return rwstatus;
      } else if (character != 'k') {
         cerr << "File " << filename << " is not a MIDI file" << endl;
         cerr << "Expecting 'k' at first byte in track but got '"
              << character << "'" << endl;
         rwstatus = 0; return rwstatus;
      }

      // Now read track chunk size and throw it away because it is
      // not really necessary since the track MUST end with an
      // end of track meta event, and many MIDI files found in the wild
      // do not correctly give the track size.
      longdata = MidiFile::readLittleEndian4Bytes(input);

      // set the size of the track allocation so that it might
      // approximately fit the data.
      events[i]->reserve((int)longdata/2);
      events[i]->clear();

      // process the track
      absticks = 0;
      // barline = 1;
      while (!input.eof()) {
         longdata = readVLValue(input);
         //cout << "ticks = " << longdata << endl;
         absticks += longdata;
         xstatus = extractMidiData(input, bytes, runningCommand);
         if (xstatus == 0) {
            rwstatus = 0;  return rwstatus;
         }
         event.setMessage(bytes);
         //cout << "command = " << hex << (int)event.data[0] << dec << endl;
         if (bytes[0] == 0xff && (bytes[1] == 1 ||
             bytes[1] == 2 || bytes[1] == 3 || bytes[1] == 4)) {
           // mididata.push_back('\0');
           // cout << '\t';
           // for (int m=0; m<event.data[2]; m++) {
           //    cout << event.data[m+3];
           // }
           // cout.flush();
         } else if (bytes[0] == 0xff && bytes[1] == 0x2f) {
            // end of track message
            // uncomment out the following three lines if you don't want
            // to see the end of track message (which is always required,
            // and added automatically when a MIDI is written.
            event.tick = absticks;
            event.track = i;
            events[i]->push_back(event);

            break;
         }

         if (bytes[0] != 0xff && bytes[0] != 0xf0) {
            event.tick = absticks;
            event.track = i;
            events[i]->push_back(event);
         } else {
            event.tick = absticks;
            event.track = i;
            events[i]->push_back(event);
         }

      }

   }

   theTimeState = TIME_STATE_ABSOLUTE;
   markSequence();
   return 1;
}



//////////////////////////////
//
// MidiFile::write -- write a standard MIDI file to a file or an output
//    stream.
//

int MidiFile::write(const char* filename) {
   fstream output(filename, ios::binary | ios::out);

   if (!output.is_open()) {
      cerr << "Error: could not write: " << filename << endl;
      return 0;
   }
   rwstatus = write(output);
   output.close();
   return rwstatus;
}


int MidiFile::write(const string& filename) {
   return MidiFile::write(filename.data());
}


int MidiFile::write(ostream& out) {
   int oldTimeState = getTickState();
   if (oldTimeState == TIME_STATE_ABSOLUTE) {
      deltaTicks();
   }

   // write the header of the Standard MIDI File

   char ch;

   // 1. The characters "MThd"
   ch = 'M'; out << ch;
   ch = 'T'; out << ch;
   ch = 'h'; out << ch;
   ch = 'd'; out << ch;

   // 2. write the size of the header (always a "6" stored in unsigned long
   //    (4 bytes).
   ulong longdata = 6;
   writeBigEndianULong(out, longdata);

   // 3. MIDI file format, type 0, 1, or 2
   ushort shortdata;
   shortdata = (getNumTracks() == 1) ? 0 : 1;
   writeBigEndianUShort(out,shortdata);

   // 4. write out the number of tracks.
   shortdata = getNumTracks();
   writeBigEndianUShort(out, shortdata);

   // 5. write out the number of ticks per quarternote. (avoiding SMTPE for now)
   shortdata = getTicksPerQuarterNote();
   writeBigEndianUShort(out, shortdata);

   // now write each track.
   vector<uchar> trackdata;
   uchar endoftrack[4] = {0, 0xff, 0x2f, 0x00};
   int i, j, k;
   int size;
   for (i=0; i<getNumTracks(); i++) {
      trackdata.reserve(123456);   // make the track data larger than
                                   // expected data input
      trackdata.clear();
      for (j=0; j<(int)events[i]->size(); j++) {
         if ((*events[i])[j].isEndOfTrack()) {
            // suppress end-of-track meta messages (one will be added
            // automatically after all track data has been written).
            continue;
         }
         writeVLValue((*events[i])[j].tick, trackdata);
         if (((*events[i])[j].getCommandByte() == 0xf0) ||
             ((*events[i])[j].getCommandByte() == 0xf7)) {
            // 0xf0 == Complete sysex message (0xf0 is part of the raw MIDI).
            // 0xf7 == Raw byte message (0xf7 not part of the raw MIDI).
            // Print the first byte of the message (0xf0 or 0xf7), then
            // print a VLV length for the rest of the bytes in the message.
            // In other words, when creating a 0xf0 or 0xf7 MIDI message,
            // do not insert the VLV byte length yourself, as this code will
            // do it for you automatically.
            trackdata.push_back((*events[i])[j][0]); // 0xf0 or 0xf7;
            writeVLValue((*events[i])[j].size()-1, trackdata);
            for (k=1; k<(int)(*events[i])[j].size(); k++) {
               trackdata.push_back((*events[i])[j][k]);
            }
         } else {
            // non-sysex type of message, so just output the
            // bytes of the message:
            for (k=0; k<(int)(*events[i])[j].size(); k++) {
               trackdata.push_back((*events[i])[j][k]);
            }
         }
      }
      size = (int)trackdata.size();
      if ((size < 3) || !((trackdata[size-3] == 0xff)
            && (trackdata[size-2] == 0x2f))) {
         trackdata.push_back(endoftrack[0]);
         trackdata.push_back(endoftrack[1]);
         trackdata.push_back(endoftrack[2]);
         trackdata.push_back(endoftrack[3]);
      }

      // now ready to write to MIDI file.

      // first write the track ID marker "MTrk":
      ch = 'M'; out << ch;
      ch = 'T'; out << ch;
      ch = 'r'; out << ch;
      ch = 'k'; out << ch;

      // A. write the size of the MIDI data to follow:
      longdata = trackdata.size();
      writeBigEndianULong(out, longdata);

      // B. write the actual data
      out.write((char*)trackdata.data(), trackdata.size());
   }

   if (oldTimeState == TIME_STATE_ABSOLUTE) {
      absoluteTicks();
   }

   return 1;
}



//////////////////////////////
//
// MidiFile::writeHex -- print the Standard MIDI file as a list of
//    ASCII Hex bytes, formatted 25 to a line by default, and
//    two digits for each hex byte code.  If the input width is 0,
//    then don't wrap lines.
//
//  default value: width=25
//

int MidiFile::writeHex(const char* aFile, int width) {
   fstream output(aFile, ios::out);
   if (!output.is_open()) {
      cerr << "Error: could not write: " << aFile << endl;
      return 0;
   }
   rwstatus = writeHex(output, width);
   output.close();
   return rwstatus;
}


//
// string version of writeHex().
//

int MidiFile::writeHex(const string& aFile, int width) {
   return MidiFile::writeHex(aFile.data(), width);
}


//
// ostream version of writeHex().
//

int MidiFile::writeHex(ostream& out, int width) {
   stringstream tempstream;
   MidiFile::write(tempstream);
   int value = 0;
   int len = (int)tempstream.str().length();
   int wordcount = 1;
   int linewidth = width >= 0 ? width : 25;
   for (int i=0; i<len; i++) {
      value = (unsigned char)tempstream.str()[i];
      printf("%02x", value);
      if (linewidth) {
         if (i < len - 1) {
            out << (wordcount % linewidth ? ' ' : '\n');
         }
         wordcount++;
      } else {
         // print with no line breaks
         if (i < len - 1) {
            out << ' ';
         }
      }
   }
   if (linewidth) {
      out << '\n';
   }
   return 1;
}



//////////////////////////////
//
// MidiFile::writeBinasc -- write a standard MIDI file from data into
//    the binasc format (ASCII version of the MIDI file).
//

int MidiFile::writeBinasc(const char* aFile) {
   fstream output(aFile, ios::out);

   if (!output.is_open()) {
      cerr << "Error: could not write: " << aFile << endl;
      return 0;
   }
   rwstatus = writeBinasc(output);
   output.close();
   return rwstatus;
}


int MidiFile::writeBinascWithComments(const char* aFile) {
   fstream output(aFile, ios::out);

   if (!output.is_open()) {
      cerr << "Error: could not write: " << aFile << endl;
      return 0;
   }
   rwstatus = writeBinascWithComments(output);
   output.close();
   return rwstatus;
}


int MidiFile::writeBinasc(const string& aFile) {
   return writeBinasc(aFile.data());
}


int MidiFile::writeBinascWithComments(const string& aFile) {
   return writeBinascWithComments(aFile.data());
}


int MidiFile::writeBinasc(ostream& output) {
   stringstream binarydata;
   rwstatus = write(binarydata);
   if (rwstatus == 0) {
      return 0;
   }

   Binasc binasc;
   binasc.setMidiOn();
   binarydata.seekg(0, ios_base::beg);
   binasc.readFromBinary(output, binarydata);
   return 1;
}


int MidiFile::writeBinascWithComments(ostream& output) {
   stringstream binarydata;
   rwstatus = write(binarydata);
   if (rwstatus == 0) {
      return 0;
   }

   Binasc binasc;
   binasc.setMidiOn();
   binasc.setCommentsOn();
   binarydata.seekg(0, ios_base::beg);
   binasc.readFromBinary(output, binarydata);
   return 1;
}



//////////////////////////////
//
// MidiFile::status -- return the success flag from the last read or
//    write (writeHex, writeBinasc).
//

int MidiFile::status(void) {
   return rwstatus;
}


///////////////////////////////////////////////////////////////////////////
//
// track-related functions --
//

//////////////////////////////
//
// MidiFile::operator[] -- return the event list for the specified track.
//

MidiEventList& MidiFile::operator[](int aTrack) {
   return *events[aTrack];
}

const MidiEventList& MidiFile::operator[](int aTrack) const {
   return *events[aTrack];
}


//////////////////////////////
//
// MidiFile::getTrackCount -- return the number of tracks in
//   the Midi File.
//

int MidiFile::getTrackCount(void) const {
   return (int)events.size();
}

//
// Alias for getTrackCount()
//

int MidiFile::getNumTracks(void) const {
   return getTrackCount();
}

//
// Alias for getTrackCount()
//

int MidiFile::size(void) const {
   return getTrackCount();
}



//////////////////////////////
//
// MidiFile::markSequence -- Assign a sequence serial number to
//   every MidiEvent in every track in the MIDI file.  This is
//   useful if you want to preseve the order of MIDI messages in
//   a track when they occur at the same tick time.  Particularly
//   for use with joinTracks() or sortTracks().  markSequence will
//   be done automatically when a MIDI file is read, in case the
//   ordering of events occuring at the same time is important.
//   Use clearSequence() to use the default sorting behavior of
//   sortTracks().
//

void MidiFile::markSequence(void) {
   int sequence = 1;
   for (int i=0; i<size(); i++) {
      for (int j=0; j<events[i]->size(); j++) {
         (*events[i])[j].seq = sequence++;
      }
   }
}



//////////////////////////////
//
// MidiFile::clearSequence -- Remove any seqence serial numbers from
//   MidiEvents in the MidiFile.  This will cause the default ordering by
//   sortTracks() to be used, in which case the ordering of MidiEvents
//   occurding at the same tick may switch their ordering.
//

void MidiFile::clearSequence(void) {
   for (int i=0; i<size(); i++) {
      for (int j=0; j<events[i]->size(); j++) {
         (*events[i])[j].seq = 0;
      }
   }
}




//////////////////////////////
//
// MidiFile::joinTracks -- Interleave the data from all tracks,
//   but keeping the identity of the tracks unique so that
//   the function splitTracks can be called to split the
//   tracks into separate units again.  The style of the
//   MidiFile when read from a file is with tracks split.
//   The original track index is stored in the MidiEvent::track
//   variable.
//

void MidiFile::joinTracks(void) {
   if (getTrackState() == TRACK_STATE_JOINED) {
      return;
   }
   if (getNumTracks() == 1) {
      return;
   }

   MidiEventList* joinedTrack;
   joinedTrack = new MidiEventList;

   int messagesum = 0;
   int length = getNumTracks();
   int i, j;
   for (i=0; i<length; i++) {
      messagesum += (*events[i]).size();
   }
   joinedTrack->reserve((int)(messagesum + 32 + messagesum * 0.1));

   int oldTimeState = getTickState();
   if (oldTimeState == TIME_STATE_DELTA) {
      absoluteTicks();
   }
   for (i=0; i<length; i++) {
      for (j=0; j<(int)events[i]->size(); j++) {
         joinedTrack->push_back_no_copy(&(*events[i])[j]);
      }
   }

   clear_no_deallocate();

   delete events[0];
   events.resize(0);
   events.push_back(joinedTrack);
   sortTracks();
   if (oldTimeState == TIME_STATE_DELTA) {
      deltaTicks();
   }

   theTrackState = TRACK_STATE_JOINED;
}



//////////////////////////////
//
// MidiFile::splitTracks -- Take the joined tracks and split them
//   back into their separate track identities.
//

void MidiFile::splitTracks(void) {
   if (getTrackState() == TRACK_STATE_SPLIT) {
      return;
   }
   int oldTimeState = getTickState();
   if (oldTimeState == TIME_STATE_DELTA) {
      absoluteTicks();
   }

   int maxTrack = 0;
   int i;
   int length = events[0]->size();
   for (i=0; i<length; i++) {
      if ((*events[0])[i].track > maxTrack) {
          maxTrack = (*events[0])[i].track;
      }
   }
   int trackCount = maxTrack + 1;

   if (trackCount <= 1) {
      return;
   }

   MidiEventList* olddata = events[0];
   events[0] = NULL;
   events.resize(trackCount);
   for (i=0; i<trackCount; i++) {
      events[i] = new MidiEventList;
   }

   int trackValue = 0;
   for (i=0; i<length; i++) {
      trackValue = (*olddata)[i].track;
      events[trackValue]->push_back_no_copy(&(*olddata)[i]);
   }

   olddata->detach();
   delete olddata;

   if (oldTimeState == TIME_STATE_DELTA) {
      deltaTicks();
   }

   theTrackState = TRACK_STATE_SPLIT;
}



//////////////////////////////
//
// MidiFile::splitTracksByChannel -- Take the joined tracks and split them
//   back into their separate track identities.
//

void MidiFile::splitTracksByChannel(void) {
   joinTracks();
   if (getTrackState() == TRACK_STATE_SPLIT) {
      return;
   }

   int oldTimeState = getTickState();
   if (oldTimeState == TIME_STATE_DELTA) {
      absoluteTicks();
   }

   int maxTrack = 0;
   int i;
   MidiEventList& eventlist = *events[0];
   MidiEventList* olddata = &eventlist;
   int length = eventlist.size();
   for (i=0; i<length; i++) {
      if (eventlist[i].size() == 0) {
         continue;
      }
      if ((eventlist[i][0] & 0xf0) == 0xf0) {
         // ignore system and meta messages.
         continue;
      }
      if (maxTrack < (eventlist[i][0] & 0x0f)) {
         maxTrack = eventlist[i][0] & 0x0f;
      }
   }
   int trackCount = maxTrack + 2; // + 1 for expression track

   if (trackCount <= 1) {
      // only one channel, so don't do anything (leave as Type-0 file).
      return;
   }

   events[0] = NULL;
   events.resize(trackCount);
   for (i=0; i<trackCount; i++) {
      events[i] = new MidiEventList;
   }

   int trackValue = 0;
   for (i=0; i<length; i++) {
      trackValue = 0;
      if ((eventlist[i][0] & 0xf0) == 0xf0) {
         trackValue = 0;
      } else if (eventlist[i].size() > 0) {
         trackValue = (eventlist[i][0] & 0x0f) + 1;
      }
      events[trackValue]->push_back_no_copy(&eventlist[i]);
   }

   olddata->detach();
   delete olddata;

   if (oldTimeState == TIME_STATE_DELTA) {
      deltaTicks();
   }

   theTrackState = TRACK_STATE_SPLIT;
}



//////////////////////////////
//
// MidiFile::getTrackState -- returns what type of track method
//     is being used: either TRACK_STATE_JOINED or TRACK_STATE_SPLIT.
//

int MidiFile::getTrackState(void) {
   return theTrackState;
}



//////////////////////////////
//
// MidiFile::hasJoinedTracks -- Returns true if the MidiFile tracks
//    are in a joined state.
//

int MidiFile::hasJoinedTracks(void) {
   return theTrackState == TRACK_STATE_JOINED;
}



//////////////////////////////
//
// MidiFile::hasSplitTracks -- Returns true if the MidiFile tracks
//     are in a split state.
//

int MidiFile::hasSplitTracks(void) {
   return theTrackState == TRACK_STATE_SPLIT;
}



//////////////////////////////
//
// MidiFile::getSplitTrack --  Return the track index when the MidiFile
//   is in the split state.  This function returns the original track
//   when the MidiFile is in the joined state.  The MidiEvent::track
//   variable is used to store the original track index when the
//   MidiFile is converted to the joined-track state.
//

int MidiFile::getSplitTrack(int track, int index) {
   if (hasSplitTracks()) {
      return track;
   } else {
      return getEvent(track, index).track;
   }
}

//
// When the parameter is void, assume track 0:
//

int MidiFile::getSplitTrack(int index) {
   if (hasSplitTracks()) {
      return 0;
   } else {
      return getEvent(0, index).track;
   }
}



///////////////////////////////////////////////////////////////////////////
//
// tick-related functions --
//

//////////////////////////////
//
// MidiFile::deltaTicks -- convert the time data to
//     delta time, which means that the time field
//     in the MidiEvent struct represents the time
//     since the last event was played. When a MIDI file
//     is read from a file, this is the default setting.
//

void MidiFile::deltaTicks(void) {
   if (getTickState() == TIME_STATE_DELTA) {
      return;
   }
   int i, j;
   int temp;
   int length = getNumTracks();
   int *timedata = new int[length];
   for (i=0; i<length; i++) {
      timedata[i] = 0;
      if (events[i]->size() > 0) {
         timedata[i] = (*events[i])[0].tick;
      } else {
         continue;
      }
      for (j=1; j<(int)events[i]->size(); j++) {
         temp = (*events[i])[j].tick;
         int deltatick = temp - timedata[i];
         if (deltatick < 0) {
            cerr << "Error: negative delta tick value: " << deltatick << endl
                 << "Timestamps must be sorted first"
                 << " (use MidiFile::sortTracks() before writing)." << endl;
         }
         (*events[i])[j].tick = deltatick;
         timedata[i] = temp;
      }
   }
   theTimeState = TIME_STATE_DELTA;
   delete [] timedata;
}



//////////////////////////////
//
// MidiFile::absoluteTicks -- convert the time data to
//    absolute time, which means that the time field
//    in the MidiEvent struct represents the exact tick
//    time to play the event rather than the time since
//    the last event to wait untill playing the current
//    event.
//

void MidiFile::absoluteTicks(void) {
   if (getTickState() == TIME_STATE_ABSOLUTE) {
      return;
   }
   int i, j;
   int length = getNumTracks();
   int* timedata = new int[length];
   for (i=0; i<length; i++) {
      timedata[i] = 0;
      if (events[i]->size() > 0) {
         timedata[i] = (*events[i])[0].tick;
      } else {
         continue;
      }
      for (j=1; j<(int)events[i]->size(); j++) {
         timedata[i] += (*events[i])[j].tick;
         (*events[i])[j].tick = timedata[i];
      }
   }
   theTimeState = TIME_STATE_ABSOLUTE;
   delete [] timedata;
}



//////////////////////////////
//
// MidiFile::getTickState -- returns what type of time method is
//   being used: either TIME_STATE_ABSOLUTE or TIME_STATE_DELTA.
//

int MidiFile::getTickState(void) {
   return theTimeState;
}



//////////////////////////////
//
// MidiFile::isDeltaTicks -- Returns true if MidiEvent .tick
//    variables are in delta time mode.
//

int MidiFile::isDeltaTicks(void) {
   return theTimeState == TIME_STATE_DELTA ? 1 : 0;
}



//////////////////////////////
//
// MidiFile::isAbsoluteTicks -- Returns true if MidiEvent .tick
//    variables are in absolute time mode.
//

int MidiFile::isAbsoluteTicks(void) {
   return theTimeState == TIME_STATE_ABSOLUTE ? 1 : 0;
}



///////////////////////////////////////////////////////////////////////////
//
// physical-time analysis functions --
//



///////////////////////////////////////////////////////////////////////////
//
// note-analysis functions --
//



///////////////////////////////////////////////////////////////////////////
//
// filename functions --
//

//////////////////////////////
//
// MidiFile::setFilename -- sets the filename of the MIDI file.
//      Currently removed any directory path.
//

void MidiFile::setFilename(const char* aname) {
   const char* ptr = strrchr(aname, '/');
   int len;
   if (ptr != NULL) {
     len = (int)strlen(ptr+1);
     readFileName.resize(len+1);
     strncpy(readFileName.data(), ptr+1, len);
   } else {
      len = (int)strlen(aname);
      readFileName.resize(len+1);
      strncpy(readFileName.data(), aname, len);
   }
}


void MidiFile::setFilename(const string& aname) {
   MidiFile::setFilename(aname.data());
}



//////////////////////////////
//
// MidiFile::getFilename -- returns the name of the file read into the
//    structure (if the data was read from a file).
//

const char* MidiFile::getFilename(void) {
   return readFileName.data();
}




















//////////////////////////////
//
// MidiFile::addEvent --
//

int MidiFile::addEvent(int aTrack, int aTick, vector<uchar>& midiData) {
   timemapvalid = 0;
   MidiEvent anEvent;
   anEvent.tick = aTick;
   anEvent.track = aTrack;
   anEvent.setMessage(midiData);

   events[aTrack]->push_back(anEvent);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addEvent -- Some bug here when joinedTracks(), but track==1...
//

int MidiFile::addEvent(MidiEvent& mfevent) {
   if (getTrackState() == TRACK_STATE_JOINED) {
      events[0]->push_back(mfevent);
      return events[0]->size()-1;
   } else {
      events[mfevent.track]->push_back(mfevent);
      return events[mfevent.track]->size()-1;
   }
}



///////////////////////////////
//
// MidiFile::addMetaEvent --
//

int MidiFile::addMetaEvent(int aTrack, int aTick, int aType,
      vector<uchar>& metaData) {
   timemapvalid = 0;
   int i;
   int length = (int)metaData.size();
   vector<uchar> fulldata;
   uchar size[23] = {0};
   int lengthsize = makeVLV(size, length);

   fulldata.resize(2+lengthsize+length);
   fulldata[0] = 0xff;
   fulldata[1] = aType & 0x7F;
   for (i=0; i<lengthsize; i++) {
      fulldata[2+i] = size[i];
   }
   for (i=0; i<length; i++) {
      fulldata[2+lengthsize+i] = metaData[i];
   }

   return addEvent(aTrack, aTick, fulldata);
}


int MidiFile::addMetaEvent(int aTrack, int aTick, int aType,
      const char* metaData) {

   int length = (int)strlen(metaData);
   vector<uchar> buffer;
   buffer.resize(length);
   int i;
   for (i=0; i<length; i++) {
      buffer[i] = (uchar)metaData[i];
   }
   return addMetaEvent(aTrack, aTick, aType, buffer);
}



//////////////////////////////
//
// MidiFile::addCopyright --  Add a copyright notice meta-message (#2).
//

int MidiFile::addCopyright(int aTrack, int aTick, const string& text) {
   MidiEvent* me = new MidiEvent;
   me->makeCopyright(text);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addTrackName --  Add an track name meta-message (#3).
//

int MidiFile::addTrackName(int aTrack, int aTick, const string& name) {
   MidiEvent* me = new MidiEvent;
   me->makeTrackName(name);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addInstrumentName --  Add an instrument name meta-message (#4).
//

int MidiFile::addInstrumentName(int aTrack, int aTick, const string& name) {
   MidiEvent* me = new MidiEvent;
   me->makeInstrumentName(name);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addLyric -- Add a lyric meta-message (meta #5).
//

int MidiFile::addLyric(int aTrack, int aTick, const string& text) {
   MidiEvent* me = new MidiEvent;
   me->makeLyric(text);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addMarker -- Add a marker meta-message (meta #6).
//

int MidiFile::addMarker(int aTrack, int aTick, const string& text) {
   MidiEvent* me = new MidiEvent;
   me->makeMarker(text);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addCue -- Add a cue-point meta-message (meta #7).
//

int MidiFile::addCue(int aTrack, int aTick, const string& text) {
   MidiEvent* me = new MidiEvent;
   me->makeCue(text);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addTempo -- Add a tempo meta message (meta #0x51).
//

int MidiFile::addTempo(int aTrack, int aTick, double aTempo) {
   MidiEvent* me = new MidiEvent;
   me->makeTempo(aTempo);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addTimeSignature -- Add a time signature meta message
//      (meta #0x58).  The "bottom" parameter must be a power of two;
//      otherwise, it will be set to the next highest power of two.
//
// Default values:
//     clocksPerClick     == 24 (quarter note)
//     num32ndsPerQuarter ==  8 (8 32nds per quarter note)
//
// Time signature of 4/4 would be:
//    top    = 4
//    bottom = 4 (converted to 2 in the MIDI file for 2nd power of 2).
//    clocksPerClick = 24 (2 eighth notes based on num32ndsPerQuarter)
//    num32ndsPerQuarter = 8
//
// Time signature of 6/8 would be:
//    top    = 6
//    bottom = 8 (converted to 3 in the MIDI file for 3rd power of 2).
//    clocksPerClick = 36 (3 eighth notes based on num32ndsPerQuarter)
//    num32ndsPerQuarter = 8
//

int MidiFile::addTimeSignature(int aTrack, int aTick, int top, int bottom,
      int clocksPerClick, int num32ndsPerQuarter) {
   MidiEvent* me = new MidiEvent;
   me->makeTimeSignature(top, bottom, clocksPerClick, num32ndsPerQuarter);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addCompoundTimeSignature -- Add a time signature meta message
//      (meta #0x58), where the clocksPerClick parameter is set to three
//      eighth notes for compount meters such as 6/8 which represents
//      two beats per measure.
//
// Default values:
//     clocksPerClick     == 36 (quarter note)
//     num32ndsPerQuarter ==  8 (8 32nds per quarter note)
//

int MidiFile::addCompoundTimeSignature(int aTrack, int aTick, int top,
      int bottom, int clocksPerClick, int num32ndsPerQuarter) {
   return addTimeSignature(aTrack, aTick, top, bottom, clocksPerClick,
      num32ndsPerQuarter);
}



//////////////////////////////
//
// MidiFile::makeVLV --  This function is used to create
//   size byte(s) for meta-messages.  If the size of the data
//   in the meta-message is greater than 127, then the size
//   should (?) be specified as a VLV.
//

int MidiFile::makeVLV(uchar *buffer, int number) {

   unsigned long value = (unsigned long)number;

   if (value >= (1 << 28)) {
      cerr << "Error: Meta-message size too large to handle" << endl;
      buffer[0] = 0;
      buffer[1] = 0;
      buffer[2] = 0;
      buffer[3] = 0;
      return 1;
   }

   buffer[0] = (value >> 21) & 0x7f;
   buffer[1] = (value >> 14) & 0x7f;
   buffer[2] = (value >>  7) & 0x7f;
   buffer[3] = (value >>  0) & 0x7f;

   int i;
   int flag = 0;
   int length = -1;
   for (i=0; i<3; i++) {
      if (buffer[i] != 0) {
         flag = 1;
      }
      if (flag) {
         buffer[i] |= 0x80;
      }
      if (length == -1 && buffer[i] >= 0x80) {
         length = 4-i;
      }
   }

   if (length == -1) {
      length = 1;
   }

   if (length < 4) {
      for (i=0; i<length; i++) {
         buffer[i] = buffer[4-length+i];
      }
   }

   return length;
}



//////////////////////////////
//
// MidiFile::addNoteOn -- Add a note-on message to the given track at the
//    given time in the given channel.
//

int MidiFile::addNoteOn(int aTrack, int aTick, int aChannel, int key, int vel) {
   MidiEvent* me = new MidiEvent;
   me->makeNoteOn(aChannel, key, vel);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addNoteOff -- Add a note-off message (using 0x80 messages).
//

int MidiFile::addNoteOff(int aTrack, int aTick, int aChannel, int key,
      int vel) {
   MidiEvent* me = new MidiEvent;
   me->makeNoteOff(aChannel, key, vel);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addNoteOff -- Add a note-off message (using 0x90 messages with
//   zero attack velocity).
//

int MidiFile::addNoteOff(int aTrack, int aTick, int aChannel, int key) {
   MidiEvent* me = new MidiEvent;
   me->makeNoteOff(aChannel, key);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addController -- Add a controller message in the given
//    track at the given tick time in the given channel.
//

int MidiFile::addController(int aTrack, int aTick, int aChannel,
      int num, int value) {
   MidiEvent* me = new MidiEvent;
   me->makeController(aChannel, num, value);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addPatchChange -- Add a patch-change message in the given
//    track at the given tick time in the given channel.
//

int MidiFile::addPatchChange(int aTrack, int aTick, int aChannel,
      int patchnum) {
   MidiEvent* me = new MidiEvent;
   me->makePatchChange(aChannel, patchnum);
   me->tick = aTick;
   events[aTrack]->push_back_no_copy(me);
   return events[aTrack]->size() - 1;
}



//////////////////////////////
//
// MidiFile::addTimbre -- Add a patch-change message in the given
//    track at the given tick time in the given channel.  Alias for
//    MidiFile::addPatchChange().
//

int MidiFile::addTimbre(int aTrack, int aTick, int aChannel, int patchnum) {
   return addPatchChange(aTrack, aTick, aChannel, patchnum);
}



//////////////////////////////
//
// MidiFile::addPitchBend -- convert  number in the range from -1 to +1
//     into two 7-bit numbers (smallest piece first)
//
//   -1.0 maps to 0 (0x0000)
//    0.0 maps to 8192 (0x2000 --> 0x40 0x00)
//   +1.0 maps to 16383 (0x3FFF --> 0x7F 0x7F)
//

int MidiFile::addPitchBend(int aTrack, int aTick, int aChannel, double amount) {
   timemapvalid = 0;
   amount += 1.0;
   int value = int(amount * 8192 + 0.5);

   // prevent any wrap-around in case of round-off errors
   if (value > 0x3fff) {
      value = 0x3fff;
   }
   if (value < 0) {
      value = 0;
   }

   int lsbint = 0x7f & value;
   int msbint = 0x7f & (value  >> 7);

   vector<uchar> mididata;
   mididata.resize(3);
   if (aChannel < 0) {
      aChannel = 0;
   } else if (aChannel > 15) {
      aChannel = 15;
   }
   mididata[0] = uchar(0xe0 | aChannel);
   mididata[1] = uchar(lsbint);
   mididata[2] = uchar(msbint);

   return addEvent(aTrack, aTick, mididata);
}



//////////////////////////////
//
// MidiFile::addTrack -- adds a blank track at end of the
//    track list.  Returns the track number of the added
//    track.
//

int MidiFile::addTrack(void) {
   int length = getNumTracks();
   events.resize(length+1);
   events[length] = new MidiEventList;
   events[length]->reserve(10000);
   events[length]->clear();
   return length;
}

int MidiFile::addTrack(int count) {
   int length = getNumTracks();
   events.resize(length+count);
   int i;
   for (i=0; i<count; i++) {
      events[length + i] = new MidiEventList;
      events[length + i]->reserve(10000);
      events[length + i]->clear();
   }
   return length + count - 1;
}



//////////////////////////////
//
// MidiFile::allocateEvents --
//

void MidiFile::allocateEvents(int track, int aSize) {
   int oldsize = events[track]->size();
   if (oldsize < aSize) {
      events[track]->reserve(aSize);
   }
}



//////////////////////////////
//
// MidiFile::deleteTrack -- remove a track from the MidiFile.
//   Tracks are numbered starting at track 0.
//

void MidiFile::deleteTrack(int aTrack) {
   int length = getNumTracks();
   if (aTrack < 0 || aTrack >= length) {
      return;
   }
   if (length == 1) {
      return;
   }
   delete events[aTrack];
   for (int i=aTrack; i<length-1; i++) {
      events[i] = events[i+1];
   }

   events[length] = NULL;
   events.resize(length-1);
}



//////////////////////////////
//
// MidiFile::clear -- make the MIDI file empty with one
//     track with no data in it.
//

void MidiFile::clear(void) {
   int length = getNumTracks();
   for (int i=0; i<length; i++) {
      delete events[i];
      events[i] = NULL;
   }
   events.resize(1);
   events[0] = new MidiEventList;
   timemapvalid=0;
   timemap.clear();
   theTrackState = TRACK_STATE_SPLIT;
   theTimeState = TIME_STATE_ABSOLUTE;
}


void MidiFile::erase(void) {
   clear();
}



//////////////////////////////
//
// MidiFile::getEvent -- return the event at the given index in the
//    specified track.
//

MidiEvent& MidiFile::getEvent(int aTrack, int anIndex) {
   return (*events[aTrack])[anIndex];
}



//////////////////////////////
//
// MidiFile::getTicksPerQuarterNote -- returns the number of
//   time units that are supposed to occur during a quarternote.
//

int MidiFile::getTicksPerQuarterNote(void) {
   if (ticksPerQuarterNote == 0xE728) {
      // this is a special case which is the SMPTE time code
      // setting for 25 frames a second with 40 subframes
      // which means one tick per millisecond.  When SMPTE is
      // being used, there is no real concept of the quarter note,
      // so presume 60 bpm as a simiplification here.
      // return 1000;
   }
   return ticksPerQuarterNote;
}

//
// Alias for getTicksPerQuarterNote:
//

int MidiFile::getTPQ(void) {
   return getTicksPerQuarterNote();
}



//////////////////////////////
//
// MidiFile::getEventCount -- returns the number of events
//   in a given track.
//

int MidiFile::getEventCount(int aTrack) {
   return events[aTrack]->size();
}


int MidiFile::getNumEvents(int aTrack) {
   return events[aTrack]->size();
}



//////////////////////////////
//
// MidiFile::mergeTracks -- combine the data from two
//   tracks into one.  Placing the data in the first
//   track location listed, and Moving the other tracks
//   in the file around to fill in the spot where Track2
//   used to be.  The results of this function call cannot
//   be reversed.
//

void MidiFile::mergeTracks(int aTrack1, int aTrack2) {
   MidiEventList* mergedTrack;
   mergedTrack = new MidiEventList;
   int oldTimeState = getTickState();
   if (oldTimeState == TIME_STATE_DELTA) {
      absoluteTicks();
   }
   int i, j;
   int length = getNumTracks();
   for (i=0; i<(int)events[aTrack1]->size(); i++) {
      mergedTrack->push_back((*events[aTrack1])[i]);
   }
   for (j=0; j<(int)events[aTrack2]->size(); i++) {
      (*events[aTrack2])[i].track = aTrack1;
      mergedTrack->push_back((*events[aTrack2])[i]);
   }

   sortTrack(*mergedTrack);

   delete events[aTrack1];

   events[aTrack1] = mergedTrack;

   for (i=aTrack2; i<length-1; i++) {
      events[i] = events[i+1];
   }

   events[length] = NULL;
   events.resize(length-1);

   if (oldTimeState == TIME_STATE_DELTA) {
      deltaTicks();
   }
}



//////////////////////////////
//
// MidiFile::setTicksPerQuarterNote --
//

void MidiFile::setTicksPerQuarterNote(int ticks) {
   ticksPerQuarterNote = ticks;
}

//
// Alias for setTicksPerQuarterNote:
//

void MidiFile::setTPQ(int ticks) {
   setTicksPerQuarterNote(ticks);
}


//////////////////////////////
//
// MidiFile::setMillisecondTicks -- set the ticks per quarter note
//   value to milliseconds.  The format for this specification is
//   highest 8-bits: SMPTE Frame rate (as a negative 2's compliment value).
//   lowest 8-bits: divisions per frame (as a positive number).
//   for millisecond resolution, the SMPTE value is -25, and the
//   frame rate is 40 frame per division.  In hexadecimal, these
//   values are: -25 = 1110,0111 = 0xE7 and 40 = 0010,1000 = 0x28
//   So setting the ticks per quarter note value to 0xE728 will cause
//   delta times in the MIDI file to represent milliseconds.
//

void MidiFile::setMillisecondTicks(void) {
   ticksPerQuarterNote = 0xE728;
}



//////////////////////////////
//
// MidiFile::sortTrack --
//

void MidiFile::sortTrack(MidiEventList& trackData) {
   if (theTimeState == TIME_STATE_ABSOLUTE) {
      qsort(trackData.data(), trackData.size(), sizeof(MidiEvent*), eventcompare);
   }
}



//////////////////////////////
//
// MidiFile::sortTracks -- sort all tracks in the MidiFile.
//

void MidiFile::sortTracks(void) {
   if (theTimeState == TIME_STATE_ABSOLUTE) {
      for (int i=0; i<getTrackCount(); i++) {
         sortTrack(*events[i]);
      }
   }
}



//////////////////////////////
//
// MidiFile::getTrackCountAsType1 --  Return the number of tracks in the
//    MIDI file.  Returns the size of the events if not in joined state.
//    If in joined state, reads track 0 to find the maximum track
//    value from the original unjoined tracks.
//

int MidiFile::getTrackCountAsType1(void) {
   if (getTrackState() == TRACK_STATE_JOINED) {
      int output = 0;
      int i;
      for (i=0; i<(int)events[0]->size(); i++) {
         if (getEvent(0,i).track > output) {
            output = getEvent(0,i).track;
         }
      }
      return output+1;  // I think the track values are 0 offset...
   } else {
      return (int)events.size();
   }
}



//////////////////////////////
//
// MidiFile::getTimeInSeconds -- return the time in seconds for
//     the current message.
//

double MidiFile::getTimeInSeconds(int aTrack, int anIndex) {
   return getTimeInSeconds(getEvent(aTrack, anIndex).tick);
}


double MidiFile::getTimeInSeconds(int tickvalue) {
   if (timemapvalid == 0) {
      buildTimeMap();
      if (timemapvalid == 0) {
         return -1.0;    // something went wrong
      }
   }

   _TickTime key;
   key.tick    = tickvalue;
   key.seconds = -1;

   void* ptr = bsearch(&key, timemap.data(), timemap.size(),
         sizeof(_TickTime), ticksearch);

   if (ptr == NULL) {
      // The specific tick value was not found, so do a linear
      // search for the two tick values which occur before and
      // after the tick value, and do a linear interpolation of
      // the time in seconds values to figure out the final
      // time in seconds.
      // Since the code is not yet written, kill the program at this point:
      return linearSecondInterpolationAtTick(tickvalue);
   } else {
      return ((_TickTime*)ptr)->seconds;
   }
}



//////////////////////////////
//
// MidiFile::getAbsoluteTickTime -- return the tick value represented
//    by the input time in seconds.  If there is not tick entry at
//    the given time in seconds, then interpolate between two values.
//

int MidiFile::getAbsoluteTickTime(double starttime) {
   if (timemapvalid == 0) {
      buildTimeMap();
      if (timemapvalid == 0) {
         if (timemapvalid == 0) {
            return -1.0;    // something went wrong
         }
      }
   }

   _TickTime key;
   key.tick    = -1;
   key.seconds = starttime;

   void* ptr = bsearch(&key, timemap.data(), timemap.size(),
         sizeof(_TickTime), secondsearch);

   if (ptr == NULL) {
      // The specific seconds value was not found, so do a linear
      // search for the two time values which occur before and
      // after the given time value, and do a linear interpolation of
      // the time in tick values to figure out the final time in ticks.
      return linearTickInterpolationAtSecond(starttime);
   } else {
      return ((_TickTime*)ptr)->tick;
   }

}



//////////////////////////////
//
// MidiFile::getTotalTimeInSeconds -- Returns the duration of the MidiFile
//    event list in seconds.  If doTimeAnalysis() is not called before this
//    function is called, it will be called automatically.
//

double MidiFile::getTotalTimeInSeconds(void) {
   if (timemapvalid == 0) {
      buildTimeMap();
      if (timemapvalid == 0) {
         return -1.0;    // something went wrong
      }
   }
   double output = 0.0;
   for (int i=0; i<(int)events.size(); i++) {
      if (events[i]->last().seconds > output) {
         output = events[i]->last().seconds;
      }
   }
   return output;
}



///////////////////////////////
//
// MidiFile::getTotalTimeInTicks -- Returns the absolute tick value for the
//    latest event in any track.  If the MidiFile is in TIME_STATE_DELTA,
//    then temporarily got into TIME_STATE_ABSOLUTE to do the calculations.
//    Note that this is expensive, so you should normally call this function
//    while in aboslute tick mode.
//

int MidiFile::getTotalTimeInTicks(void) {
   int oldTimeState = getTickState();
   if (oldTimeState == TIME_STATE_DELTA) {
      absoluteTicks();
   }
   if (oldTimeState == TIME_STATE_DELTA) {
      deltaTicks();
   }
   int output = 0.0;
   for (int i=0; i<(int)events.size(); i++) {
      if (events[i]->last().tick > output) {
         output = events[i]->last().tick;
      }
   }
   return output;
}



///////////////////////////////
//
// MidiFile::getTotalTimeInQuarters -- Returns the Duration of the MidiFile
//    in units of quarter notes.  If the MidiFile is in TIME_STATE_DELTA,
//    then temporarily got into TIME_STATE_ABSOLUTE to do the calculations.
//    Note that this is expensive, so you should normally call this function
//    while in aboslute tick mode.
//

double MidiFile::getTotalTimeInQuarters(void) {
   double totalTicks = getTotalTimeInTicks();
   return totalTicks / getTicksPerQuarterNote();
}



//////////////////////////////
//
// MidiFile::doTimeAnalysis -- Identify the real-time position of
//    all events by monitoring the tempo in relations to the tick
//    times in the file.
//

void MidiFile::doTimeAnalysis(void) {
    buildTimeMap();
}



//////////////////////////////
//
// MidiFile::linkNotePairs --  Link note-ons to note-offs separately
//     for each track.  Returns the total number of note message pairs
//     that were linked.
//

int MidiFile::linkNotePairs(void) {
   int i;
   int sum = 0;
   for (i=0; i<getTrackCount(); i++) {
      if (events[i] == NULL) {
         continue;
      }
      sum += events[i]->linkNotePairs();
   }
   return sum;
}


int MidiFile::linkEventPairs(void) {
    return linkNotePairs();
}


//////////////////////////////
//
// MidiFile::clearLinks --
//

void MidiFile::clearLinks(void) {
   for (int i=0; i<getTrackCount(); i++) {
      if (events[i] == NULL) {
         continue;
      }
      events[i]->clearLinks();
   }
}



///////////////////////////////////////////////////////////////////////////
//
// private functions
//

//////////////////////////////
//
// MidiFile::linearTickInterpolationAtSecond -- return the tick value at the
//    given input time.
//

int MidiFile::linearTickInterpolationAtSecond(double seconds) {
   if (timemapvalid == 0) {
      buildTimeMap();
      if (timemapvalid == 0) {
         return -1.0;    // something went wrong
      }
   }

   int i;
   double lasttime = timemap[timemap.size()-1].seconds;
   // give an error value of -1 if time is out of range of data.
   if (seconds < 0.0) {
      return -1;
   }
   if (seconds > timemap[timemap.size()-1].seconds) {
      return -1;
   }

   // Guess which side of the list is closest to target:
   // Could do a more efficient algorithm since time values are sorted,
   // but good enough for now...
   int startindex = -1;
   if (seconds < lasttime / 2) {
      for (i=0; i<(int)timemap.size(); i++) {
         if (timemap[i].seconds > seconds) {
            startindex = i-1;
            break;
         } else if (timemap[i].seconds == seconds) {
            startindex = i;
            break;
         }
      }
   } else {
      for (i=(int)timemap.size()-1; i>0; i--) {
         if (timemap[i].seconds < seconds) {
            startindex = i+1;
            break;
         } else if (timemap[i].seconds == seconds) {
            startindex = i;
            break;
         }
      }
   }

   if (startindex < 0) {
      return -1;
   }
   if (startindex >= (int)timemap.size()-1) {
      return -1;
   }

   double x1 = timemap[startindex].seconds;
   double x2 = timemap[startindex+1].seconds;
   double y1 = timemap[startindex].tick;
   double y2 = timemap[startindex+1].tick;
   double xi = seconds;

   return (xi-x1) * ((y2-y1)/(x2-x1)) + y1;
}



//////////////////////////////
//
// MidiFile::linearSecondInterpolationAtTick -- return the time in seconds
//    value at the given input tick time. (Ticks input could be made double).
//

double MidiFile::linearSecondInterpolationAtTick(int ticktime) {
   if (timemapvalid == 0) {
      buildTimeMap();
      if (timemapvalid == 0) {
         return -1.0;    // something went wrong
      }
   }

   int i;
   double lasttick = timemap[timemap.size()-1].tick;
   // give an error value of -1 if time is out of range of data.
   if (ticktime < 0.0) {
      return -1;
   }
   if (ticktime > timemap.back().tick) {
      return -1;  // don't try to extrapolate
   }

   // Guess which side of the list is closest to target:
   // Could do a more efficient algorithm since time values are sorted,
   // but good enough for now...
   int startindex = -1;
   if (ticktime < lasttick / 2) {
      for (i=0; i<(int)timemap.size(); i++) {
         if (timemap[i].tick > ticktime) {
            startindex = i-1;
            break;
         } else if (timemap[i].tick == ticktime) {
            startindex = i;
            break;
         }
      }
   } else {
      for (i=(int)timemap.size()-1; i>0; i--) {
         if (timemap[i].tick < ticktime) {
            startindex = i;
            break;
         } else if (timemap[i].tick == ticktime) {
            startindex = i;
            break;
         }
      }
   }

   if (startindex < 0) {
      return -1;
   }
   if (startindex >= (int)timemap.size()-1) {
      return -1;
   }

   if (timemap[startindex].tick == ticktime) {
      return timemap[startindex].seconds;
   }

   double x1 = timemap[startindex].tick;
   double x2 = timemap[startindex+1].tick;
   double y1 = timemap[startindex].seconds;
   double y2 = timemap[startindex+1].seconds;
   double xi = ticktime;

   return (xi-x1) * ((y2-y1)/(x2-x1)) + y1;
}



//////////////////////////////
//
// MidiFile::buildTimeMap -- build an index of the absolute tick values
//      found in a MIDI file, and their corresponding time values in
//      seconds, taking into consideration tempo change messages.  If no
//      tempo messages are given (or until they are given, then the
//      tempo is set to 120 beats per minute).  If SMPTE time code is
//      used, then ticks are actually time values.  So don't build
//      a time map for SMPTE ticks, and just calculate the time in
//      seconds from the tick value (1000 ticks per second SMPTE
//      is the only mode tested (25 frames per second and 40 subframes
//      per frame).
//

void MidiFile::buildTimeMap(void) {

   // convert the MIDI file to absolute time representation
   // in single track mode (and undo if the MIDI file was not
   // in that state when this function was called.
   //
   int trackstate = getTrackState();
   int timestate  = getTickState();

   absoluteTicks();
   joinTracks();

   int allocsize = getNumEvents(0);
   timemap.reserve(allocsize+10);
   timemap.clear();

   _TickTime value;

   int lasttick = 0;
   int curtick;
   int tickinit = 0;

   int i;
   int tpq = getTicksPerQuarterNote();
   double defaultTempo = 120.0;
   double secondsPerTick = 60.0 / (defaultTempo * tpq);

   double lastsec = 0.0;
   double cursec = 0.0;

   for (i=0; i<getNumEvents(0); i++) {
      curtick = getEvent(0, i).tick;
      getEvent(0, i).seconds = cursec;
      if ((curtick > lasttick) || !tickinit) {
         tickinit = 1;

         // calculate the current time in seconds:
         cursec = lastsec + (curtick - lasttick) * secondsPerTick;
         getEvent(0, i).seconds = cursec;

         // store the new tick to second mapping
         value.tick = curtick;
         value.seconds = cursec;
         timemap.push_back(value);
         lasttick   = curtick;
         lastsec    = cursec;
      }

      // update the tempo if needed:
      if (getEvent(0,i).isTempo()) {
         secondsPerTick = getEvent(0,i).getTempoSPT(getTicksPerQuarterNote());
      }
   }

   // reset the states of the tracks or time values if necessary here:
   if (timestate == TIME_STATE_DELTA) {
      deltaTicks();
   }
   if (trackstate == TRACK_STATE_SPLIT) {
      splitTracks();
   }

   timemapvalid = 1;

}



//////////////////////////////
//
// MidiFile::extractMidiData -- Extract MIDI data from input
//    stream.  Return value is 0 if failure; otherwise, returns 1.
//

int MidiFile::extractMidiData(istream& input, vector<uchar>& array,
   uchar& runningCommand) {

   int character;
   uchar byte;
   array.clear();
   int runningQ;

   character = input.get();
   if (character == EOF) {
      cerr << "Error: unexpected end of file." << endl;
      return 0;
   } else {
      byte = (uchar)character;
   }

   if (byte < 0x80) {
      runningQ = 1;
      if (runningCommand == 0) {
         cerr << "Error: running command with no previous command" << endl;
         return 0;
      }
      if (runningCommand >= 0xf0) {
         cerr << "Error: running status not permitted with meta and sysex"
              << " event." << endl;
         return 0;
      }
   } else {
      runningCommand = byte;
      runningQ = 0;
   }

   array.push_back(runningCommand);
   if (runningQ) {
      array.push_back(byte);
   }

   int i;
   uchar metai;
   switch (runningCommand & 0xf0) {
      case 0x80:        // note off (2 more bytes)
      case 0x90:        // note on (2 more bytes)
      case 0xA0:        // aftertouch (2 more bytes)
      case 0xB0:        // cont. controller (2 more bytes)
      case 0xE0:        // pitch wheel (2 more bytes)
         byte = MidiFile::readByte(input);
         array.push_back(byte);
         if (!runningQ) {
            byte = MidiFile::readByte(input);
            array.push_back(byte);
         }
         break;
      case 0xC0:        // patch change (1 more byte)
      case 0xD0:        // channel pressure (1 more byte)
         if (!runningQ) {
            byte = MidiFile::readByte(input);
            array.push_back(byte);
         }
         break;
      case 0xF0:
         switch (runningCommand) {
            case 0xff:                 // meta event
               {
               if (!runningQ) {
                  byte = MidiFile::readByte(input); // meta type
               array.push_back(byte);
               }
               metai = MidiFile::readByte(input); // meta type
               array.push_back(metai);
               for (uchar j=0; j<metai; j++) {
                  byte = MidiFile::readByte(input); // meta type
                  array.push_back(byte);
               }
               }
               break;
            // The 0xf0 and 0xf7 meta commands deal with system-exclusive
            // messages. 0xf0 is used to either start a message or to store
            // a complete message.  The 0xf0 is part of the outgoing MIDI
            // bytes.  The 0xf7 message is used to send arbitrary bytes,
            // typically the middle or ends of system exclusive messages.  The
            // 0xf7 byte at the start of the message is not part of the
            // outgoing raw MIDI bytes, but is kept in the MidiFile message
            // to indicate a raw MIDI byte message (typically a partial
            // system exclusive message).
            case 0xf7:                // Raw bytes. 0xf7 is not part of the raw
                                      // bytes, but are included to indicate
                                      // that this is a raw byte message.
            case 0xf0:                // System Exclusive message
               {                      // (complete, or start of message).
               int length = (int)readVLValue(input);
               for (i=0; i<length; i++) {
                  byte = MidiFile::readByte(input);
                  array.push_back(byte);
               }
               }
               break;
             // other "F" MIDI commands are not expected, but can be
             // handled here if they exist.
         }
         break;
      default:
         cout << "Error reading midifile" << endl;
         cout << "Command byte was " << (int)runningCommand << endl;
         return 0;
   }
   return 1;
}



//////////////////////////////
//
// MidiFile::readVLValue -- The VLV value is expected to be unpacked into
//   a 4-byte integer no greater than 0x0fffFFFF, so a VLV value up to
//   4-bytes in size (FF FF FF 7F) will only be considered.  Longer
//   VLV values are not allowed in standard MIDI files, so the extract
//   delta time would be truncated and the extra byte(s) will be parsed
//   incorrectly as a MIDI command.
//

ulong MidiFile::readVLValue(istream& input) {
   uchar b[4] = {0};

   for (int i=0; i<4; i++) {
      b[i] = MidiFile::readByte(input);
      if (b[i] < 0x80) {
         break;
      }
   }

   return unpackVLV(b[0], b[1], b[2], b[3]);
}



//////////////////////////////
//
// MidiFile::unpackVLV -- converts a VLV value to an unsigned long value.
//     The bytes a, b, c, d are in big-endian order (the order they would
//     be read out of the MIDI file).
// default values: a = b = c = d = 0;
//

ulong MidiFile::unpackVLV(uchar a, uchar b, uchar c, uchar d) {
   if (d > 0x7f) {
      cerr << "Error: VLV value was too long" << endl;
      return 0;
   }

   uchar bytes[4] = {a, b, c, d};
   int count = 0;
   while (bytes[count] > 0x7f && count < 4) {
      count++;
   }
   count++;

   ulong output = 0;
   for (int i=0; i<count; i++) {
      output = output << 7;
      output = output | (bytes[i] & 0x7f);
   }

   return output;
}



//////////////////////////////
//
// MidiFile::writeVLValue -- write a number to the midifile
//    as a variable length value which segments a file into 7-bit
//    values and adds a contination bit to each.  Maximum size of input 
//    aValue is 0x0FFFffff.
//

void MidiFile::writeVLValue(long aValue, vector<uchar>& outdata) {
   uchar bytes[4] = {0};

   if ((unsigned long)aValue >= (1 << 28)) {
      cerr << "Error: number too large to convert to VLV" << endl;
      aValue = 0x0FFFffff;
   }

   bytes[0] = (uchar)(((ulong)aValue >> 21) & 0x7f);  // most significant 7 bits
   bytes[1] = (uchar)(((ulong)aValue >> 14) & 0x7f);
   bytes[2] = (uchar)(((ulong)aValue >> 7)  & 0x7f);
   bytes[3] = (uchar)(((ulong)aValue)       & 0x7f);  // least significant 7 bits

   int start = 0;
   while ((start<4) && (bytes[start] == 0))  start++;

   for (int i=start; i<3; i++) {
      bytes[i] = bytes[i] | 0x80;
      outdata.push_back(bytes[i]);
   }
   outdata.push_back(bytes[3]);
}



//////////////////////////////
//
// MidiFile::clear_no_deallocate -- Similar to clear() but does not
//   delete the Events in the lists.  This is primarily used internally
//   to the MidiFile class, so don't use unless you really know what you
//   are doing (otherwise you will end up with memory leaks or
//   segmentation faults).
//

void MidiFile::clear_no_deallocate(void) {
   for (int i=0; i<getTrackCount(); i++) {
      events[i]->detach();
      delete events[i];
      events[i] = NULL;
   }
   events.resize(1);
   events[0] = new MidiEventList;
   timemapvalid=0;
   timemap.clear();
   // events.resize(0);   // causes a memory leak [20150205 Jorden Thatcher]
}



///////////////////////////////////////////////////////////////////////////
//
// external functions
//



//////////////////////////////
//
// eventcompare -- Event comparison function for sorting tracks.
//
// Sorting rules:
//    (1) sort by (absolute) tick value; otherwise, if tick values are the same:
//    (2) end-of-track meta message is always last.
//    (3) other meta-messages come before regular MIDI messages.
//    (4) note-offs come after all other regular MIDI messages except note-ons.
//    (5) note-ons come after all other regular MIDI messages.
//

int eventcompare(const void* a, const void* b) {
   MidiEvent& aevent = **((MidiEvent**)a);
   MidiEvent& bevent = **((MidiEvent**)b);

   if (aevent.tick > bevent.tick) {
      // aevent occurs after bevent
      return +1;
   } else if (aevent.tick < bevent.tick) {
      // aevent occurs before bevent
      return -1;
   } else if (aevent.seq > bevent.seq) {
      // aevent sequencing state occurs after bevent
      // see MidiFile::markSequence()
      return +1;
   } else if (aevent.seq < bevent.seq) {
      // aevent sequencing state occurs before bevent
      // see MidiFile::markSequence()
      return -1;
   } else if (aevent[0] == 0xff && aevent[1] == 0x2f) {
      // end-of-track meta-message should always be last (but won't really
      // matter since the writing function ignores all end-of-track messages
      // and writes its own.
      return +1;
   } else if (bevent[0] == 0xff && bevent[1] == 0x2f) {
      // end-of-track meta-message should always be last (but won't really
      // matter since the writing function ignores all end-of-track messages
      // and writes its own.
      return -1;
   } else if (aevent[0] == 0xff && bevent[0] != 0xff) {
      // other meta-messages are placed before real MIDI messages
      return -1;
   } else if (aevent[0] != 0xff && bevent[0] == 0xff) {
      // other meta-messages are placed before real MIDI messages
      return +1;
   } else if (((aevent[0] & 0xf0) == 0x90) && (aevent[2] != 0)) {
      // note-ons come after all other types of MIDI messages
      return +1;
   } else if (((bevent[0] & 0xf0) == 0x90) && (bevent[2] != 0)) {
      // note-ons come after all other types of MIDI messages
      return -1;
   } else if (((aevent[0] & 0xf0) == 0x90) || ((aevent[0] & 0xf0) == 0x80)) {
      // note-offs come after all other MIDI messages (except note-ons)
      return +1;
   } else if (((bevent[0] & 0xf0) == 0x90) || ((bevent[0] & 0xf0) == 0x80)) {
      // note-offs come after all other MIDI messages (except note-ons)
      return -1;
   } else {
      return 0;
   }
}



//////////////////////////////
//
// operator<< -- for printing an ASCII version of the MIDI file
//

ostream& operator<<(ostream& out, MidiFile& aMidiFile) {
   aMidiFile.writeBinascWithComments(out);
   return out;
}



//////////////////////////////
//
// MidiFile::ticksearch -- for finding a tick entry in the time map.
//

int MidiFile::ticksearch(const void* A, const void* B) {
   _TickTime& a = *((_TickTime*)A);
   _TickTime& b = *((_TickTime*)B);

   if (a.tick < b.tick) {
      return -1;
   } else if (a.tick > b.tick) {
      return 1;
   }
   return 0;
}



//////////////////////////////
//
// MidiFile::secondsearch -- for finding a second entry in the time map.
//

int MidiFile::secondsearch(const void* A, const void* B) {
   _TickTime& a = *((_TickTime*)A);
   _TickTime& b = *((_TickTime*)B);

   if (a.seconds < b.seconds) {
      return -1;
   } else if (a.seconds > b.seconds) {
      return 1;
   }
   return 0;
}


///////////////////////////////////////////////////////////////////////////
//
// Static functions:
//


//////////////////////////////
//
// MidiFile::readLittleEndian4Bytes -- Read four bytes which are in
//      little-endian order (smallest byte is first).  Then flip
//      the order of the bytes to create the return value.
//

ulong MidiFile::readLittleEndian4Bytes(istream& input) {
   uchar buffer[4] = {0};
   input.read((char*)buffer, 4);
   if (input.eof()) {
      cerr << "Error: unexpected end of file." << endl;
      return 0;
   }
   return buffer[3] | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);
}



//////////////////////////////
//
// MidiFile::readLittleEndian2Bytes -- Read two bytes which are in
//       little-endian order (smallest byte is first).  Then flip
//       the order of the bytes to create the return value.
//

ushort MidiFile::readLittleEndian2Bytes(istream& input) {
   uchar buffer[2] = {0};
   input.read((char*)buffer, 2);
   if (input.eof()) {
      cerr << "Error: unexpected end of file." << endl;
      return 0;
   }
   return buffer[1] | (buffer[0] << 8);
}



//////////////////////////////
//
// MidiFile::readByte -- Read one byte from input stream.  Exit if there
//     was an error.
//

uchar MidiFile::readByte(istream& input) {
   uchar buffer[1] = {0};
   input.read((char*)buffer, 1);
   if (input.eof()) {
      cerr << "Error: unexpected end of file." << endl;
      return 0;
   }
   return buffer[0];
}



//////////////////////////////
//
// MidiFile::writeLittleEndianUShort --
//

ostream& MidiFile::writeLittleEndianUShort(ostream& out, ushort value) {
   union { char bytes[2]; ushort us; } data;
   data.us = value;
   out << data.bytes[0];
   out << data.bytes[1];
   return out;
}



//////////////////////////////
//
// MidiFile::writeBigEndianUShort --
//

ostream& MidiFile::writeBigEndianUShort(ostream& out, ushort value) {
   union { char bytes[2]; ushort us; } data;
   data.us = value;
   out << data.bytes[1];
   out << data.bytes[0];
   return out;
}



//////////////////////////////
//
// MidiFile::writeLittleEndianShort --
//

ostream& MidiFile::writeLittleEndianShort(ostream& out, short value) {
   union { char bytes[2]; short s; } data;
   data.s = value;
   out << data.bytes[0];
   out << data.bytes[1];
   return out;
}



//////////////////////////////
//
// MidiFile::writeBigEndianShort --
//

ostream& MidiFile::writeBigEndianShort(ostream& out, short value) {
   union { char bytes[2]; short s; } data;
   data.s = value;
   out << data.bytes[1];
   out << data.bytes[0];
   return out;
}



//////////////////////////////
//
// MidiFile::writeLittleEndianULong --
//

ostream& MidiFile::writeLittleEndianULong(ostream& out, ulong value) {
   union { char bytes[4]; ulong ul; } data;
   data.ul = value;
   out << data.bytes[0];
   out << data.bytes[1];
   out << data.bytes[2];
   out << data.bytes[3];
   return out;
}



//////////////////////////////
//
// MidiFile::writeBigEndianULong --
//

ostream& MidiFile::writeBigEndianULong(ostream& out, ulong value) {
   union { char bytes[4]; long ul; } data;
   data.ul = value;
   out << data.bytes[3];
   out << data.bytes[2];
   out << data.bytes[1];
   out << data.bytes[0];
   return out;
}



//////////////////////////////
//
// MidiFile::writeLittleEndianLong --
//

ostream& MidiFile::writeLittleEndianLong(ostream& out, long value) {
   union { char bytes[4]; long l; } data;
   data.l = value;
   out << data.bytes[0];
   out << data.bytes[1];
   out << data.bytes[2];
   out << data.bytes[3];
   return out;
}



//////////////////////////////
//
// MidiFile::writeBigEndianLong --
//

ostream& MidiFile::writeBigEndianLong(ostream& out, long value) {
   union { char bytes[4]; long l; } data;
   data.l = value;
   out << data.bytes[3];
   out << data.bytes[2];
   out << data.bytes[1];
   out << data.bytes[0];
   return out;

}



//////////////////////////////
//
// MidiFile::writeBigEndianFloat --
//

ostream& MidiFile::writeBigEndianFloat(ostream& out, float value) {
   union { char bytes[4]; float f; } data;
   data.f = value;
   out << data.bytes[3];
   out << data.bytes[2];
   out << data.bytes[1];
   out << data.bytes[0];
   return out;
}



//////////////////////////////
//
// MidiFile::writeLittleEndianFloat --
//

ostream& MidiFile::writeLittleEndianFloat(ostream& out, float value) {
   union { char bytes[4]; float f; } data;
   data.f = value;
   out << data.bytes[0];
   out << data.bytes[1];
   out << data.bytes[2];
   out << data.bytes[3];
   return out;
}



//////////////////////////////
//
// MidiFile::writeBigEndianDouble --
//

ostream& MidiFile::writeBigEndianDouble(ostream& out, double value) {
   union { char bytes[8]; double d; } data;
   data.d = value;
   out << data.bytes[7];
   out << data.bytes[6];
   out << data.bytes[5];
   out << data.bytes[4];
   out << data.bytes[3];
   out << data.bytes[2];
   out << data.bytes[1];
   out << data.bytes[0];
   return out;
}



//////////////////////////////
//
// MidiFile::writeLittleEndianDouble --
//

ostream& MidiFile::writeLittleEndianDouble(ostream& out, double value) {
   union { char bytes[8]; double d; } data;
   data.d = value;
   out << data.bytes[0];
   out << data.bytes[1];
   out << data.bytes[2];
   out << data.bytes[3];
   out << data.bytes[4];
   out << data.bytes[5];
   out << data.bytes[6];
   out << data.bytes[7];
   return out;
}



//////////////////////////////
//
// MidiFile::operator=(MidiFile) -- Assignment.
//

MidiFile& MidiFile::operator=(MidiFile other) {
   events.swap(other.events);
   return *this;
}

