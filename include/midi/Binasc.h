//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Feb 16 12:26:32 PST 2015 Adapted from binasc program.
// Last Modified: Wed Feb 18 14:48:21 PST 2015
// Filename:      midifile/include/Binasc.cpp
// Syntax:        C++11
// vim:           ts=3 expandtab
//
// description:   Interface to convert bytes between binary and ASCII forms.
//

#ifndef _BINASC_H_INCLUDED
#define _BINASC_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

class Binasc {
   public:
               Binasc         (void);
              ~Binasc         ();

      // functions for setting options:
      int      setLineLength  (int length);
      int      getLineLength  (void);
      int      setLineBytes   (int length);
      int      getLineBytes   (void);
      void     setComments    (int state);
      void     setCommentsOn  (void);
      void     setCommentsOff (void);
      int      getComments    (void);
      void     setBytes       (int state);
      void     setBytesOn     (void);
      void     setBytesOff    (void);
      int      getBytes       (void);
      void     setMidi        (int state);
      void     setMidiOn      (void);
      void     setMidiOff     (void);
      int      getMidi        (void);

      // functions for converting into a binary file:
      int      writeToBinary  (const string& outfile, const string& infile);
      int      writeToBinary  (const string& outfile, istream& input);
      int      writeToBinary  (ostream& out, const string& infile);
      int      writeToBinary  (ostream& out, istream& input);

      // functions for converting into an ASCII file with hex bytes:
      int      readFromBinary (const string& outfile, const string& infile);
      int      readFromBinary (const string& outfile, istream& input);
      int      readFromBinary (ostream& out, const string& infile);
      int      readFromBinary (ostream& out, istream& input);

      // static functions for writing ordered bytes:
      static ostream& writeLittleEndianUShort (ostream& out, ushort value);
      static ostream& writeBigEndianUShort    (ostream& out, ushort value);
      static ostream& writeLittleEndianShort  (ostream& out, short  value);
      static ostream& writeBigEndianShort     (ostream& out, short  value);
      static ostream& writeLittleEndianULong  (ostream& out, ulong  value);
      static ostream& writeBigEndianULong     (ostream& out, ulong  value);
      static ostream& writeLittleEndianLong   (ostream& out, long   value);
      static ostream& writeBigEndianLong      (ostream& out, long   value);
      static ostream& writeLittleEndianFloat  (ostream& out, float  value);
      static ostream& writeBigEndianFloat     (ostream& out, float  value);
      static ostream& writeLittleEndianDouble (ostream& out, double value);
      static ostream& writeBigEndianDouble    (ostream& out, double value);

      static string   keyToPitchName          (int key);

   protected:
      // helper functions for reading ASCII content to conver to binary:
      int      processLine        (ostream& out, const string& input,
                                   int lineNum);
      int      processAsciiWord   (ostream& out, const string& input,
                                   int lineNum);
      int      processStringWord  (ostream& out, const string& input,
                                   int lineNum);
      int      processBinaryWord  (ostream& out, const string& input,
                                   int lineNum);
      int      processDecimalWord (ostream& out, const string& input,
                                   int lineNum);
      int      processHexWord     (ostream& out, const string& input,
                                   int lineNum);
      int      processVlvWord     (ostream& out, const string& input,
                                   int lineNum);
      int      processMidiPitchBendWord(ostream& out, const string& input,
                                   int lineNum);
      int      processMidiTempoWord(ostream& out, const string& input,
                                   int lineNum);

      // helper functions for reading binary content to convert to ASCII:
      int      outputStyleAscii   (ostream& out, istream& input);
      int      outputStyleBinary  (ostream& out, istream& input);
      int      outputStyleBoth    (ostream& out, istream& input);
      int      outputStyleMidi    (ostream& out, istream& input);

      // MIDI parsing helper functions:
      int      readMidiEvent  (ostream& out, istream& infile, int& trackbytes,
                               int& command);
      int      getVLV         (istream& infile, int& trackbytes);
      int      getWord        (string& word, const string& input,
                               const string& terminators, int index);


   private:
      int bytesQ;        // option for printing hex bytes in ASCII output.
      int commentsQ;     // option for printing comments in ASCII output.
      int midiQ;         // output ASCII data as parsed MIDI file.
      int maxLineLength; // number of character in ASCII output on a line.
      int maxLineBytes;  // number of hex bytes in ASCII output on a line.
};


#endif /* _BINASC_H_INCLUDED */



