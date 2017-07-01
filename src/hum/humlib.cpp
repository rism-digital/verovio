//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Aug  8 12:24:49 PDT 2015
// Last Modified: Sat Jul  1 02:23:07 CEST 2017
// Filename:      /include/humlib.cpp
// URL:           https://github.com/craigsapp/humlib/blob/master/src/humlib.cpp
// Syntax:        C++11
// vim:           ts=3
//
// Description:   Source file for humlib library.
//
/*
Copyright (c) 2015, 2016, 2017 Craig Stuart Sapp
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   and the following disclaimer in the documentation and/or other materials
   provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "humlib.h"

namespace hum {



//////////////////////////////
//
// Convert::isKernRest -- Returns true if the input string represents
//   a **kern rest.
//

bool Convert::isKernRest(const string& kerndata) {
	if (kerndata.find("r") != string::npos) {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// Convert::isKernNote -- Returns true if the input string represents
//   a **kern note (i.e., token with a pitch, not a null token or a rest).
//

bool Convert::isKernNote(const string& kerndata) {
	char ch;
	for (int i=0; i < (int)kerndata.size(); i++) {
		ch = std::tolower(kerndata[i]);
		if ((ch >= 'a') && (ch <= 'g')) {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// Convert::isKernSecondaryTiedNote -- Returns true if the input string
//   represents a **kern note (i.e., token with a pitch,
//   not a null token or a rest) and has a '_' or ']' character.
//

bool Convert::isKernSecondaryTiedNote(const string& kerndata) {
	char ch;
	if (!Convert::isKernNote(kerndata)) {
		return false;
	}
	for (int i=0; i < (int)kerndata.size(); i++) {
		ch = std::tolower(kerndata[i]);
		if ((ch == '_') || (ch == ']')) {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// Convert::isKernNoteAttack -- Returns true if the input string
//   represents a **kern note (not null or rest) and is not a
//   secondary tied note.
//

bool Convert::isKernNoteAttack(const string& kerndata) {
	char ch;
	if (!Convert::isKernNote(kerndata)) {
		return false;
	}
	for (int i=0; i < (int)kerndata.size(); i++) {
		ch = std::tolower(kerndata[i]);
		if ((ch == '_') || (ch == ']')) {
			return false;
		}
	}
	return true;
}



//////////////////////////////
//
// Convert::hasKernSlurStart -- Returns true if the input string
//   has a '('.
//

bool Convert::hasKernSlurStart(const string& kerndata) {
	for (int i=0; i < (int)kerndata.size(); i++) {
		char ch = kerndata[i];
		if (ch == '(') {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// Convert::hasKernSlurEnd -- Returns true if the input string
//   has a '('.
//

bool Convert::hasKernSlurEnd(const string& kerndata) {
	for (int i=0; i < (int)kerndata.size(); i++) {
		char ch = kerndata[i];
		if (ch == ')') {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// Convert::getKernSlurStartElisionLevel -- Returns the number of
//   '&' characters before the given '(' character in a kern token.
//   Returns -1 if no '(' character in string.
//

int Convert::getKernSlurStartElisionLevel(const string& kerndata, int index) {
	bool foundSlurStart = false;
	int output = 0;
	int count = 0;
	int target = index + 1;
	for (int i=0; i<(int)kerndata.size(); i++) {
		char ch = kerndata[i];
		if (ch == '(') {
			count++;
		}
		if (count == target) {
			foundSlurStart = true;
			for (int j=i-1; j>=0; j--) {
				ch = kerndata[j];
				if (ch == '&') {
					output++;
				} else {
					break;
				}
			}
			break;
		}
	}
	if (!foundSlurStart) {
		return -1;
	} else {
		return output;
	}
}



//////////////////////////////
//
// Convert::getKernSlurEndElisionLevel -- Returns the number of
//   '&' characters before the last ')' character in a kern token.
//   Returns -1 if no ')' character in string.
//

int Convert::getKernSlurEndElisionLevel(const string& kerndata, int index) {
	bool foundSlurEnd = false;
	int output = 0;
	int count = 0;
	int target = index + 1;
	for (int i=0; i<(int)kerndata.size(); i++) {
		char ch = kerndata[i];
		if (ch == ')') {
			count++;
		}
		if (count == target) {
			foundSlurEnd = true;
			for (int j=i-1; j>=0; j--) {
				ch = kerndata[j];
				if (ch == '&') {
					output++;
				} else {
					break;
				}
			}
			break;
		}
	}
	if (!foundSlurEnd) {
		return -1;
	} else {
		return output;
	}
}



//////////////////////////////
//
// Convert::getKernPitchAttributes --
//    pc         = pitch class
//    numacc     = numeric accidental (-1=flat, 0=natural, 1=sharp)
//    explicit   = force showing of accidental
//    oct        = octave number (middle C = 4)
//    base40     = base-40 enumeration of pitch (valid if abs(numacc) <= 2)
//

string Convert::getKernPitchAttributes(const string& kerndata) {
	int accid = kernToAccidentalCount(kerndata);
	string output = "";

	output += " dpc=\"";
	output += kernToDiatonicUC(kerndata);
	output += "\"";

	output += " numacc=\"";
	output += to_string(accid);
	output += "\"";

	if (kerndata.find('n') != string::npos) {
		output += " explicit =\"true\"";
	}

	output += " oct=\"";
	output += to_string(kernToOctaveNumber(kerndata));
	output += "\"";

	if (abs(accid) <= 2) {
		output += " base40=\"";
		output += to_string(kernToBase40(kerndata));
		output += "\"";
	}

	return output;
}





//////////////////////////////
//
// Convert::getLcm -- Return the Least Common Multiple of a list of numbers.
//

int Convert::getLcm(const vector<int>& numbers) {
	if (numbers.size() == 0) {
		return 1;
	}
	int output = numbers[0];
	for (int i=1; i<(int)numbers.size(); i++) {
		output = (output * numbers[i]) / getGcd(output, numbers[i]);
	}
	return output;
}



//////////////////////////////
//
// Convert::getGcd -- Return the Greatest Common Divisor of two numbers.
//

int Convert::getGcd(int a, int b) {
	if (b == 0) {
		return a;
	}
	int c = a % b;
	a = b;
	int output = getGcd(a, c);
	return output;
}



//////////////////////////////
//
// Convert::primeFactors -- Return a list of prime factors of a number.
//

void Convert::primeFactors(vector<int>& output, int n) {
	output.clear();
	while (n%2 == 0) {
		output.push_back(2);
		n = n >> 1;
	}
	for (int i=3; i <= sqrt(n); i += 2) {
		while (n%i == 0) {
			output.push_back(i);
			n = n/i;
		}
	}
	if (n > 2) {
		output.push_back(n);
	}
}



//////////////////////////////
//
// Convert::nearIntQuantize -- avoid small deviations from integer values.
//    devault value: delta = 0.00001
//

double Convert::nearIntQuantize(double value, double delta) {
	if ((value + delta) - int(value+delta)  < delta*2) {
		value = (int)(value+delta);
	}
	return value;
}



//////////////////////////////
//
// Convert::significantDigits --
//

double Convert::significantDigits(double value, int digits) {
	double scale = pow(10, digits);
	return (int(value * scale + 0.5))/scale;
}



//////////////////////////////
//
// Convert::isNaN -- needed due to compiler differences.
//

bool Convert::isNaN(double value) {
	union { uint64_t u; double f; } ieee754;
	ieee754.f = value;
	return ( (unsigned)(ieee754.u >> 32) & 0x7fffffff ) +
           ( (unsigned)ieee754.u != 0 ) > 0x7ff00000;
}



//////////////////////////////
//
// Tool_transpose::pearsonCorrelation --
//

double Convert::pearsonCorrelation(vector<double> x, vector<double> y) {
	double sumx  = 0.0;
	double sumy  = 0.0;
	double sumco = 0.0;
	double meanx = x[0];
	double meany = y[0];
	double sweep;
	double deltax;
	double deltay;

	int size = (int)x.size();
	if ((int)y.size() < size) {
		size = (int)y.size();
	}

	for (int i=2; i<=size; i++) {
		sweep = (i-1.0) / i;
		deltax = x[i-1] - meanx;
		deltay = y[i-1] - meany;
		sumx  += deltax * deltax * sweep;
		sumy  += deltay * deltay * sweep;
		sumco += deltax * deltay * sweep;
		meanx += deltax / i;
		meany += deltay / i;
	}

	double popsdx = sqrt(sumx / size);
	double popsdy = sqrt(sumy / size);
	double covxy  = sumco / size;

	return covxy / (popsdx * popsdy);
}





//////////////////////////////
//
// Convert::kernToScientificPitch -- Convert a **kern pitch to
//   ScientificPitch notation, which is the diatonic letter name,
//   followed by a possible accidental, then an optional separator
//   string, and finally the octave number.  A string representing a
//   chord can be given to this function, and the output will return
//   a list of the pitches in the chord, separated by a space.
// default value: flat      = "b"
// default value: sharp     = "#"
// default value: separator = ""
//

string Convert::kernToScientificPitch(const string& kerndata,
		string flat, string sharp, string separator) {
	vector<string> subtokens = Convert::splitString(kerndata);
	string output;
	char   diatonic;
	int    accidental;
	int    octave;

	for (int i=0; i<(int)subtokens.size(); i++) {
		diatonic   = Convert::kernToDiatonicUC(subtokens[i]);
		accidental = Convert::kernToAccidentalCount(subtokens[i]);
		octave     = Convert::kernToOctaveNumber(subtokens[i]);
		if ((i > 0) && (i < (int)subtokens.size()-1)) {
			output += " ";
		}
		output += diatonic;
		for (int j=0; j<abs(accidental); j++) {
			output += (accidental < 0 ? flat : sharp);
		}
		output += separator;
		output += to_string(octave);
	}

	return output;
}



//////////////////////////////
//
// Convert::kernToDiatonicPC -- Convert a kern token into a diatonic
//    note pitch-class where 0="C", 1="D", ..., 6="B".  -1000 is returned
//    if the note is rest, and -2000 if there is no pitch information in the
//    input string. Only the first subtoken in the string is considered.
//

int Convert::kernToDiatonicPC(const string& kerndata) {
	for (int i=0; i<(int)kerndata.size(); i++) {
		if (kerndata[i] == ' ') {
			break;
		}
		if (kerndata[i] == 'r') {
			return -1000;
		}
		switch (kerndata[i]) {
			case 'A': case 'a': return 5;
			case 'B': case 'b': return 6;
			case 'C': case 'c': return 0;
			case 'D': case 'd': return 1;
			case 'E': case 'e': return 2;
			case 'F': case 'f': return 3;
			case 'G': case 'g': return 4;
		}
	}
	return -2000;
}



//////////////////////////////
//
// Convert::kernToDiatonicUC -- Convert a kern token into a diatonic
//    note pitch-class.  "R" is returned if the note is rest, and
//    "X" is returned if there is no pitch name in the string.
//    Only the first subtoken in the string is considered.
//

char Convert::kernToDiatonicUC(const string& kerndata) {
	for (int i=0; i<(int)kerndata.size(); i++) {
		if (kerndata[i] == ' ') {
			break;
		}
		if (kerndata[i] == 'r') {
			return 'R';
		}
		if (('A' <= kerndata[i]) && (kerndata[i] <= 'G')) {
			return kerndata[i];
		}
		if (('a' <= kerndata[i]) && (kerndata[i] <= 'g')) {
			return toupper(kerndata[i]);
		}
	}
	return 'X';
}



//////////////////////////////
//
// Convert::kernToDiatonicLC -- Similar to kernToDiatonicUC, but
//    the returned pitch name is lower case.
//

char Convert::kernToDiatonicLC(const string& kerndata) {
	return tolower(Convert::kernToDiatonicUC(kerndata));
}



//////////////////////////////
//
// Convert::kernToAccidentalCount -- Convert a kern token into a count
//    of accidentals in the first subtoken.  Sharps are assigned to the
//    value +1 and flats to -1.  So a double sharp is +2 and a double
//    flat is -2.  Only the first subtoken in the string is considered.
//    Cases such as "#-" should not exist, but in this case the return
//    value will be 0.
//

int Convert::kernToAccidentalCount(const string& kerndata) {
	int output = 0;
	for (int i=0; i<(int)kerndata.size(); i++) {
		if (kerndata[i] == ' ') {
			break;
		}
		if (kerndata[i] == '-') {
			output--;
		}
		if (kerndata[i] == '#') {
			output++;
		}
	}
	return output;
}



//////////////////////////////
//
// Convert::kernToOctaveNumber -- Convert a kern token into an octave number.
//    Middle C is the start of the 4th octave. -1000 is returned if there
//    is not pitch in the string.  Only the first subtoken in the string is
//    considered.
//

int Convert::kernToOctaveNumber(const string& kerndata) {
	int uc = 0;
	int lc = 0;
	if (kerndata == ".") {
		return -1000;
	}
	for (int i=0; i<(int)kerndata.size(); i++) {
		if (kerndata[i] == ' ') {
			break;
		}
		if (kerndata[i] == 'r') {
			return -1000;
		}
		uc += ('A' <= kerndata[i]) && (kerndata[i] <= 'G') ? 1 : 0;
		lc += ('a' <= kerndata[i]) && (kerndata[i] <= 'g') ? 1 : 0;
	}
	if ((uc > 0) && (lc > 0)) {
		// invalid pitch description
		return -1000;
	}
	if (uc > 0) {
		return 4 - uc;
	} else if (lc > 0) {
		return 3 + lc;
	} else {
		return -1000;
	}
}



//////////////////////////////
//
// Convert::kernToBase40PC -- Convert **kern pitch to a base-40 pitch class.
//    Will ignore subsequent pitches in a chord.
//

int Convert::kernToBase40PC(const string& kerndata) {
	int diatonic = Convert::kernToDiatonicPC(kerndata);
	if (diatonic < 0) {
		return diatonic;
	}
	int accid  = Convert::kernToAccidentalCount(kerndata);
	int output = -1000;
	switch (diatonic) {
		case 0: output =  0; break;
		case 1: output =  6; break;
		case 2: output = 12; break;
		case 3: output = 17; break;
		case 4: output = 23; break;
		case 5: output = 29; break;
		case 6: output = 35; break;
	}
	output += accid;
	return output + 2;     // +2 to make c-flat-flat bottom of octave.
}



//////////////////////////////
//
// Convert::kernToBase40 -- Convert **kern pitch to a base-40 integer.
//    Will ignore subsequent pitches in a chord.
//

int Convert::kernToBase40(const string& kerndata) {
	int pc = Convert::kernToBase40PC(kerndata);
	if (pc < 0) {
		return pc;
	}
	int octave   = Convert::kernToOctaveNumber(kerndata);
	return pc + 40 * octave;
}



//////////////////////////////
//
// Convert::kernToBase12PC -- Convert **kern pitch to a base-12 pitch-class.
//   C=0, C#/D-flat=1, D=2, etc.
//

int Convert::kernToBase12PC(const string& kerndata) {
	int diatonic = Convert::kernToDiatonicPC(kerndata);
	if (diatonic < 0) {
		return diatonic;
	}
	int accid    = Convert::kernToAccidentalCount(kerndata);
	int output = -1000;
	switch (diatonic) {
		case 0: output =  0; break;
		case 1: output =  2; break;
		case 2: output =  4; break;
		case 3: output =  5; break;
		case 4: output =  7; break;
		case 5: output =  9; break;
		case 6: output = 11; break;
	}
	output += accid;
	return output;
}



//////////////////////////////
//
// Convert::kernToBase12 -- Convert **kern pitch to a base-12 integer.
//     (middle C = 48).
//

int Convert::kernToBase12(const string& kerndata) {
	int pc = Convert::kernToBase12PC(kerndata);
	if (pc < 0) {
		return pc;
	}
	int octave = Convert::kernToOctaveNumber(kerndata);
	return pc + 12 * octave;
}



//////////////////////////////
//
// Convert::base40ToKern -- Convert Base-40 integer pitches into
//   **kern pitch representation.
//

string Convert::base40ToKern(int b40) {
	int octave     = b40 / 40;
	int accidental = Convert::base40ToAccidental(b40);
	int diatonic   = Convert::base40ToDiatonic(b40) % 7;
	char base = 'a';
	switch (diatonic) {
		case 0: base = 'c'; break;
		case 1: base = 'd'; break;
		case 2: base = 'e'; break;
		case 3: base = 'f'; break;
		case 4: base = 'g'; break;
		case 5: base = 'a'; break;
		case 6: base = 'b'; break;
	}
	if (octave < 4) {
		base = std::toupper(base);
	}
	int repeat = 0;
	if (octave > 4) {
		repeat = octave - 4;
	} else if (octave < 3) {
		repeat = 3 - octave;
	}
	if (repeat > 12) {
		cerr << "Error: unreasonable octave value: " << octave << endl;
		exit(1);
	}
	string output;
	output += base;
	for (int i=0; i<repeat; i++) {
		output += base;
	}
	if (accidental == 0) {
		return output;
	}
	if (accidental > 0) {
		for (int i=0; i<accidental; i++) {
			output += '#';
		}
	} else if (accidental < 0) {
		for (int i=0; i<-accidental; i++) {
			output += '-';
		}
	}

	return output;
}



//////////////////////////////
//
// Convert::base40ToDiatonic -- find the diatonic pitch of the
//   given base-40 pitch.  Output pitch classes: 0=C, 1=D, 2=E,
//   3=F, 4=G, 5=A, 6=B.  To this the diatonic octave is added.
//   To get only the diatonic pitch class, mod by 7: (% 7).
//   Base-40 pitches are not allowed, and the algorithm will have
//   to be adjusted to allow them.  Currently any negative base-40
//   value is presumed to be a rest and not processed.
//

int Convert::base40ToDiatonic(int b40) {
	int chroma = b40 % 40;
	int octaveoffset = (b40 / 40) * 7;
	if (b40 < 0) {
		return -1;   // rest;
	}
	switch (chroma) {
		case 0: case 1: case 2: case 3: case 4:      // C-- to C##
			return 0 + octaveoffset;
		case 6: case 7: case 8: case 9: case 10:     // D-- to D##
			return 1 + octaveoffset;
		case 12: case 13: case 14: case 15: case 16: // E-- to E##
			return 2 + octaveoffset;
		case 17: case 18: case 19: case 20: case 21: // F-- to F##
			return 3 + octaveoffset;
		case 23: case 24: case 25: case 26: case 27: // G-- to G##
			return 4 + octaveoffset;
		case 29: case 30: case 31: case 32: case 33: // A-- to A##
			return 5 + octaveoffset;
		case 35: case 36: case 37: case 38: case 39: // B-- to B##
			return 6 + octaveoffset;
	}

	// found an empty slot, so return rest:
	return -1;
}



//////////////////////////////
//
// Convert::base40ToMidiNoteNumber --
//

int Convert::base40ToMidiNoteNumber(int b40) {
	// +1 since middle-C octave is 5 in MIDI:
	int octave     = b40 / 40 + 1;
	int accidental = Convert::base40ToAccidental(b40);
	int diatonicpc = Convert::base40ToDiatonic(b40) % 7;
	switch (diatonicpc) {
		case 0: return octave * 12 +  0 + accidental;
		case 1: return octave * 12 +  2 + accidental;
		case 2: return octave * 12 +  4 + accidental;
		case 3: return octave * 12 +  5 + accidental;
		case 4: return octave * 12 +  7 + accidental;
		case 5: return octave * 12 +  9 + accidental;
		case 6: return octave * 12 + 11 + accidental;
		default: return -1000; // can't deal with negative pitches
	}
}



//////////////////////////////
//
// Convert::base40ToAccidental -- +1 = 1 sharp, +2 = double sharp, 0 = natural
//	-1 = 1 flat, -2 = double flat
//

int Convert::base40ToAccidental(int b40) {
	if (b40 < 0) {
		// not considering low pitches.  If so then the mod operator
		// below whould need fixing.
		return 0;
	}

	switch (b40 % 40) {
		case 0:	return -2;      // C-double-flat
		case 1:	return -1;      // C-flat
		case 2:	return  0;      // C
		case 3:	return  1;      // C-sharp
		case 4:	return  2;      // C-double-sharp
		case 5:	return 1000;
		case 6:	return -2;
		case 7:	return -1;
		case 8:	return  0;      // D
		case 9:	return  1;
		case 10:	return  2;
		case 11:	return 1000;
		case 12:	return -2;
		case 13:	return -1;
		case 14:	return  0;      // E
		case 15:	return  1;
		case 16:	return  2;
		case 17:	return -2;
		case 18:	return -1;
		case 19:	return  0;      // F
		case 20:	return  1;
		case 21:	return  2;
		case 22:	return 1000;
		case 23:	return -2;
		case 24:	return -1;
		case 25:	return  0;      // G
		case 26:	return  1;
		case 27:	return  2;
		case 28:	return 1000;
		case 29:	return -2;
		case 30:	return -1;
		case 31:	return  0;      // A
		case 32:	return  1;
		case 33:	return  2;
		case 34:	return 1000;
		case 35:	return -2;
		case 36:	return -1;
		case 37:	return  0;      // B
		case 38:	return  1;
		case 39:	return  2;
	}

	return 0;
}



///////////////////////////////
//
// Convert::kernToMidiNoteNumber -- Convert **kern to MIDI note number
//    (middle C = 60).  Middle C is assigned to octave 5 rather than
//    octave 4 for the kernToBase12() function.
//

int Convert::kernToMidiNoteNumber(const string& kerndata) {
	int pc = Convert::kernToBase12PC(kerndata);
	if (pc < 0) {
		return pc;
	}
	int octave = Convert::kernToOctaveNumber(kerndata);
	return pc + 12 * (octave + 1);
}



//////////////////////////////
//
// Convert::kernToBase7 -- Convert **kern pitch to a base-7 integer.
//    This is a diatonic pitch class with C=0, D=1, ..., B=6.
//

int Convert::kernToBase7(const string& kerndata) {
	int diatonic = Convert::kernToDiatonicPC(kerndata);
	if (diatonic < 0) {
		return diatonic;
	}
	int octave = Convert::kernToOctaveNumber(kerndata);
	return diatonic + 7 * octave;;
}



//////////////////////////////
//
// Convert::pitchToWbh -- Convert a given diatonic pitch class and
//   accidental adjustment into an integer.  The diatonic pitch class
//   is C=0, D=1, E=2, F=3, G=4, A=5, B=6. "acc" is the accidental
//   count: -2=double flat, -1=double flat, 0 natural, +1=sharp, etc.
//   "octave" is the octave number, with middle-C being the start of
//   octave 4.  //   "maxacc" is the maximum accidental which defines
//    the base:
//    maxacc = 2 -> Base-40.
//    maxacc = n -> Base (n*2+1)*7 + 5.
//

int Convert::pitchToWbh(int dpc, int acc, int octave, int maxacc) {
	if (dpc > 6) {
		// allow for pitch-classes expressed as ASCII characters:
		dpc = std::tolower(dpc) - 'a' + 5;
		dpc = dpc % 7;
	}
	int output = -1000;
	switch (dpc) {
		case 0: output = maxacc;            break;
		case 1: output =  3  * maxacc + 2;  break;
		case 2: output =  5  * maxacc + 4;  break;
		case 3: output =  7  * maxacc + 5;  break;
		case 4: output =  9  * maxacc + 7;  break;
		case 5: output =  11 * maxacc + 9;  break;
		case 6: output =  13 * maxacc + 11; break;
	}
	if (output < 0) {
		return output;
	}
	return (output + acc) + (7 * (maxacc * 2 + 1) + 5) * octave;
}



//////////////////////////////
//
// Convert::wbhToPitch -- Convert an integer-based pitch into
//    a diatonic pitch class, accidental alteration and octave number
//   The output diatonic pitch classes are 0=C, 1=D, 2=E, 3=F, 4=G, 5=A, 6=B.
//   "acc" is the accidental count: -2=double flat, -1=double flat,
//   0 natural, +1=sharp, etc.
//   "octave" is the octave number, with middle-C being the start of
//   octave 4.
//   "maxacc" is the maximum accidental which defines
//    the base:
//    maxacc = 2 -> Base-40.
//    maxacc = n -> Base (n*2+1)*7 + 5.
//    This valus must match the the analogous value used in PitchToWbh().
//

void Convert::wbhToPitch(int& dpc, int& acc, int& octave, int maxacc,
		int wbh) {
	int cwidth = maxacc * 2 + 1;
	int base = 7 * cwidth + 5;
	octave = wbh / base;
	int pc = wbh % base;

	// test for C diatonic pitch:
	int pctest = cwidth;
	if (pc < pctest) {
		dpc = 0;
		acc = pc - pctest + maxacc + 1;
		return;
	}

	// test for D diatonic pitch
	pctest += 1 + cwidth;
	if (pc < pctest) {
		dpc = 1;
		acc = pc - pctest + maxacc + 1;
		return;
	}

	// test for E diatonic pitch
	pctest += 1 + cwidth;
	if (pc < pctest) {
		dpc = 2;
		acc = pc - pctest + maxacc + 1;
		return;
	}

	// test for F diatonic pitch
	pctest += cwidth;
	if (pc < pctest) {
		dpc = 3;
		acc = pc - pctest + maxacc + 1;
		return;
	}

	// test for G diatonic pitch
	pctest += 1 + cwidth;
	if (pc < pctest) {
		dpc = 4;
		acc = pc - pctest + maxacc + 1;
		return;
	}

	// test for A diatonic pitch
	pctest += 1 + cwidth;
	if (pc < pctest) {
		dpc = 5;
		acc = pc - pctest + maxacc + 1;
		return;
	}

	// test for B diatonic pitch
	pctest += 1 + cwidth;
	if (pc < pctest) {
		dpc = 6;
		acc = pc - pctest + maxacc + 1;
		return;
	}

	// if acc in any of the above tests is +3/-3, then there was an
	// accidental overflow (overflow of the accidental).
}



//////////////////////////////
//
// Convert::kernClefToBaseline -- returns the diatonic pitch
//    of the bottom line on the staff.
//

int Convert::kernClefToBaseline(const string& input) {
	string clefname;
	if (input.compare(0, 5, "*clef") == 0) {
		clefname = input.substr(5);
	} else if (input.compare(0, 4, "clef") == 0) {
		clefname = input.substr(4);
	} else {
		cerr << "Error in Convert::kernClefToBaseline: " << input << endl;
		return -1000;
	}

	if (clefname == "G2") {                        // treble clef
		return Convert::kernToBase7("e");
	} else if (clefname == "F4") {                 // bass clef
		return Convert::kernToBase7("GG");
	} else if (clefname == "C3") {                 // alto clef
		return Convert::kernToBase7("F");
	} else if (clefname == "C4") {                 // tenor clef
		return Convert::kernToBase7("D");
	} else if (clefname == "Gv2") {                // vocal tenor clef
		return Convert::kernToBase7("E");

	// rest of C clef possibilities:
	} else if (clefname == "C1") {                 // soprano clef
		return Convert::kernToBase7("c");
	} else if (clefname == "C2") {                 // mezzo-soprano clef
		return Convert::kernToBase7("A");
	} else if (clefname == "C5") {                 // baritone clef
		return Convert::kernToBase7("BB");

	// rest of G clef possibilities:
	} else if (clefname == "G1") {                 // French-violin clef
		return Convert::kernToBase7("g");
	} else if (clefname == "G3") {
		return Convert::kernToBase7("c");
	} else if (clefname == "G4") {
		return Convert::kernToBase7("A");
	} else if (clefname == "G5") {
		return Convert::kernToBase7("F");

	// rest of F clef possibilities:
	} else if (clefname == "F1") {
		return Convert::kernToBase7("F");
	} else if (clefname == "F2") {
		return Convert::kernToBase7("D");
	} else if (clefname == "F3") {
		return Convert::kernToBase7("BB");
	} else if (clefname == "F5") {
		return Convert::kernToBase7("EE");

	// rest of G clef down an octave possibilities:
	} else if (clefname == "Gv1") {
		return Convert::kernToBase7("G");
	} else if (clefname == "Gv3") {
		return Convert::kernToBase7("C");
	} else if (clefname == "Gv4") {
		return Convert::kernToBase7("AA");
	} else if (clefname == "Gv5") {
		return Convert::kernToBase7("FF");

	// F clef down an octave possibilities:
	} else if (clefname == "Fv1") {
		return Convert::kernToBase7("FF");
	} else if (clefname == "Fv2") {
		return Convert::kernToBase7("DD");
	} else if (clefname == "Fv3") {
		return Convert::kernToBase7("BBB");
	} else if (clefname == "Fv4") {
		return Convert::kernToBase7("GGG");
	} else if (clefname == "Fv5") {
		return Convert::kernToBase7("EEE");

	// C clef down an octave possibilities:
	} else if (clefname == "Cv1") {
		return Convert::kernToBase7("C");
	} else if (clefname == "Cv2") {
		return Convert::kernToBase7("AA");
	} else if (clefname == "Cv3") {
		return Convert::kernToBase7("FF");
	} else if (clefname == "Cv4") {
		return Convert::kernToBase7("DD");
	} else if (clefname == "Cv5") {
		return Convert::kernToBase7("BBB");

	// G clef up an octave possibilities:
	} else if (clefname == "G^1") {
		return Convert::kernToBase7("gg");
	} else if (clefname == "G^2") {
		return Convert::kernToBase7("ee");
	} else if (clefname == "G^3") {
		return Convert::kernToBase7("cc");
	} else if (clefname == "G^4") {
		return Convert::kernToBase7("a");
	} else if (clefname == "G^5") {
		return Convert::kernToBase7("f");

	// F clef up an octave possibilities:
	} else if (clefname == "F^1") {
		return Convert::kernToBase7("f");
	} else if (clefname == "F^2") {
		return Convert::kernToBase7("d");
	} else if (clefname == "F^3") {
		return Convert::kernToBase7("B");
	} else if (clefname == "F^4") {
		return Convert::kernToBase7("G");
	} else if (clefname == "F^5") {
		return Convert::kernToBase7("E");

	// C clef up an octave possibilities:
	} else if (clefname == "C^1") {
		return Convert::kernToBase7("cc");
	} else if (clefname == "C^2") {
		return Convert::kernToBase7("a");
	} else if (clefname == "C^3") {
		return Convert::kernToBase7("f");
	} else if (clefname == "C^4") {
		return Convert::kernToBase7("d");
	} else if (clefname == "C^5") {
		return Convert::kernToBase7("B");

	// there are also two octaves down (*clefGvv2) and two octaves up (*clefG^^2)
	} else {
		// but just use treble clef if don't know what the clef it by this point
		return Convert::kernToBase7("e");
	}
}



//////////////////////////////
//
// Convert::base40ToTrans -- convert a base-40 interval into
//    a trans program's diatonic/chromatic alteration marker
//

string Convert::base40ToTrans(int base40) {
	int sign = 1;
	int chroma;
	int octave;
	if (base40 < 0) {
		sign = -1;
		chroma = -base40 % 40;
		octave = -base40 / 40;
	} else {
		sign = +1;
		chroma = base40 % 40;
		octave = base40 / 40;
	}

	int cval = 0;
	int dval = 0;

	switch (chroma * sign) {
		case   0: dval=0;  cval=0;   break; // C -> C
		case   1: dval=0;  cval=1;   break; // C -> C#
		case   2: dval=0;  cval=2;   break; // C -> C##
		case   4: dval=1;  cval=0;   break; // C -> D--
		case   5: dval=1;  cval=1;   break; // C -> D-
		case   6: dval=1;  cval=2;   break; // C -> D
		case   7: dval=1;  cval=3;   break; // C -> D#
		case   8: dval=1;  cval=4;   break; // C -> D##
		case  10: dval=2;  cval=2;   break; // C -> E--
		case  11: dval=2;  cval=3;   break; // C -> E-
		case  12: dval=2;  cval=4;   break; // C -> E
		case  13: dval=2;  cval=5;   break; // C -> E#
		case  14: dval=2;  cval=6;   break; // C -> E##
		case  15: dval=3;  cval=3;   break; // C -> F--
		case  16: dval=3;  cval=4;   break; // C -> F-
		case  17: dval=3;  cval=5;   break; // C -> F
		case  18: dval=3;  cval=6;   break; // C -> F#
		case  19: dval=3;  cval=7;   break; // C -> F##
		case  21: dval=4;  cval=5;   break; // C -> G--
		case  22: dval=4;  cval=6;   break; // C -> G-
		case  23: dval=4;  cval=7;   break; // C -> G
		case  24: dval=4;  cval=8;   break; // C -> G#
		case  25: dval=4;  cval=9;   break; // C -> G##
		case  27: dval=5;  cval=7;   break; // C -> A--
		case  28: dval=5;  cval=8;   break; // C -> A-
		case  29: dval=5;  cval=9;   break; // C -> A
		case  30: dval=5;  cval=10;  break; // C -> A#
		case  31: dval=5;  cval=11;  break; // C -> A##
		case  33: dval=6;  cval=9;   break; // C -> B--
		case  34: dval=6;  cval=10;  break; // C -> B-
		case  35: dval=6;  cval=11;  break; // C -> B
		case  36: dval=6;  cval=12;  break; // C -> B#
		case  37: dval=6;  cval=13;  break; // C -> B##
		case  38: dval=7;  cval=10;  break; // C -> c--
		case  39: dval=7;  cval=11;  break; // C -> c-
		case  -1: dval=-0; cval=-1;  break; // c -> c-
		case  -2: dval=-0; cval=-2;  break; // c -> c--
		case  -3: dval=-1; cval=1;   break; // c -> B##
		case  -4: dval=-1; cval=-0;  break; // c -> B#
		case  -5: dval=-1; cval=-1;  break; // c -> B
		case  -6: dval=-1; cval=-2;  break; // c -> B-
		case  -7: dval=-1; cval=-3;  break; // c -> B--
		case  -9: dval=-2; cval=-1;  break; // c -> A##
		case -10: dval=-2; cval=-2;  break; // c -> A#
		case -11: dval=-2; cval=-3;  break; // c -> A
		case -12: dval=-2; cval=-4;  break; // c -> A-
		case -13: dval=-2; cval=-5;  break; // c -> A-
		case -15: dval=-3; cval=-3;  break; // c -> G##
		case -16: dval=-3; cval=-4;  break; // c -> G#
		case -17: dval=-3; cval=-5;  break; // c -> G
		case -18: dval=-3; cval=-6;  break; // c -> G-
		case -19: dval=-3; cval=-7;  break; // c -> G--
		case -21: dval=-4; cval=-5;  break; // c -> F##
		case -22: dval=-4; cval=-6;  break; // c -> F#
		case -23: dval=-4; cval=-7;  break; // c -> F
		case -24: dval=-4; cval=-8;  break; // c -> F-
		case -25: dval=-4; cval=-9;  break; // c -> F--
		case -26: dval=-5; cval=-6;  break; // c -> E##
		case -27: dval=-5; cval=-7;  break; // c -> E#
		case -28: dval=-5; cval=-8;  break; // c -> E
		case -29: dval=-5; cval=-9;  break; // c -> E-
		case -30: dval=-5; cval=-10; break; // c -> E--
		case -32: dval=-6; cval=-8;  break; // c -> D##
		case -33: dval=-6; cval=-9;  break; // c -> D#
		case -34: dval=-6; cval=-10; break; // c -> D
		case -35: dval=-6; cval=-11; break; // c -> D-
		case -36: dval=-6; cval=-12; break; // c -> D--
		case -38: dval=-7; cval=-10; break; // c -> C##
		case -39: dval=-7; cval=-11; break; // c -> C#
		default:
			dval=0; cval=0;
	}

	if (octave > 0) {
		dval = dval + sign * octave * 7;
		cval = cval + sign * octave * 12;
	}

	string output = "d";
	output += to_string(dval);
	output += "c";
	output += to_string(cval);

	return output;
}



//////////////////////////////
//
// Convert::base40ToIntervalAbbr --
//

string Convert::base40ToIntervalAbbr(int base40interval) {
	if (base40interval < -1000) {
		return "r";
	}

	string output;
	if (base40interval < 0) {
		output = "-";
		base40interval = -base40interval;
	}

	// Add chromatic prefix
	switch (base40interval % 40) {
		case  0: output += "p"   ; break;  // C
		case  1: output += "a"   ; break;  // C#
		case  2: output += "aa"  ; break;  // C##
		case  3: output += "X"   ; break;  // X
		case  4: output += "d"   ; break;  // D--
		case  5: output += "m"   ; break;  // D-
		case  6: output += "M"   ; break;  // D
		case  7: output += "a"   ; break;  // D#
		case  8: output += "aa"  ; break;  // D##
		case  9: output += "X"   ; break;  // X
		case 10: output += "d"   ; break;  // E--
		case 11: output += "m"   ; break;  // E-
		case 12: output += "M"   ; break;  // E
		case 13: output += "a"   ; break;  // E#
		case 14: output += "aa"  ; break;  // E##
		case 15: output += "dd"  ; break;  // F--
		case 16: output += "d"   ; break;  // F-
		case 17: output += "p"   ; break;  // F
		case 18: output += "a"   ; break;  // F#
		case 19: output += "aa"  ; break;  // F##
		case 20: output += "X"   ; break;  // X
		case 21: output += "dd"  ; break;  // G--
		case 22: output += "d"   ; break;  // G-
		case 23: output += "p"   ; break;  // G
		case 24: output += "a"   ; break;  // G#
		case 25: output += "aa"  ; break;  // G##
		case 26: output += "X"   ; break;  // X
		case 27: output += "d"   ; break;  // A--
		case 28: output += "m"   ; break;  // A-
		case 29: output += "M"   ; break;  // A
		case 30: output += "a"   ; break;  // A#
		case 31: output += "aa"  ; break;  // A##
		case 32: output += "X"   ; break;  // X
		case 33: output += "d"   ; break;  // B--
		case 34: output += "m"   ; break;  // B-
		case 35: output += "M"   ; break;  // B
		case 36: output += "a"   ; break;  // B#
		case 37: output += "aa"  ; break;  // B##
		case 38: output += "dd"  ; break;  // C--
		case 39: output += "d"   ; break;  // C-
	}

	// Add base-7 number
	char buffer2[32] = {0};
	int diatonic = Convert::base40IntervalToDiatonic(base40interval)+1;
	sprintf(buffer2, "%d", diatonic);
	output += buffer2;

	return output;
}



//////////////////////////////
//
// Convert::base40IntervalToDiatonic -- convert a base40 interval
//    into a diatonic interval (excluding the chromatic alteration)
//

int Convert::base40IntervalToDiatonic(int base40interval) {
   int sign = 1;
   if (base40interval < 0) {
      sign = -1;
      base40interval = -base40interval;
   }
   int octave = base40interval / 40;
   base40interval = base40interval % 40;

   int diatonic = 0;
   switch (base40interval) {
      case  0: diatonic = 0; break;  // C
      case  1: diatonic = 0; break;  // C#
      case  2: diatonic = 0; break;  // C##

      case  3: diatonic = 1000; break;  // blank

      case  4: diatonic = 1; break;  // D--
      case  5: diatonic = 1; break;  // D-
      case  6: diatonic = 1; break;  // D
      case  7: diatonic = 1; break;  // D#
      case  8: diatonic = 1; break;  // D##

      case  9: diatonic = 1000; break;  // blank

      case 10: diatonic = 2; break;  // E--
      case 11: diatonic = 2; break;  // E-
      case 12: diatonic = 2; break;  // E
      case 13: diatonic = 2; break;  // E#
      case 14: diatonic = 2; break;  // E##

      case 15: diatonic = 3; break;  // F--
      case 16: diatonic = 3; break;  // F-
      case 17: diatonic = 3; break;  // F
      case 18: diatonic = 3; break;  // F#
      case 19: diatonic = 3; break;  // F##

      case 20: diatonic = 1000; break;  // blank

      case 21: diatonic = 4; break;  // G--
      case 22: diatonic = 4; break;  // G-
      case 23: diatonic = 4; break;  // G
      case 24: diatonic = 4; break;  // G#
      case 25: diatonic = 4; break;  // G##

      case 26: diatonic = 1000; break;  // blank

      case 27: diatonic = 5; break;  // A--
      case 28: diatonic = 5; break;  // A-
      case 29: diatonic = 5; break;  // A
      case 30: diatonic = 5; break;  // A#
      case 31: diatonic = 5; break;  // A##

      case 32: diatonic = 1000; break;  // blank

      case 33: diatonic = 6; break;  // B--
      case 34: diatonic = 6; break;  // B-
      case 35: diatonic = 6; break;  // B
      case 36: diatonic = 6; break;  // B#
      case 37: diatonic = 6; break;  // B##

      case 38: diatonic = 0; break;  // C--
      case 39: diatonic = 0; break;  // C-
   }

   return sign * (diatonic + octave * 7);
}



//////////////////////////////
//
// Convert::transToBase40 -- convert the Humdrum Toolkit program
//     trans's binomial notation for intervals into base-40.
//  The input can be in three formats:
//     d1c2      == no prepended text on information
//     *Trd1c2   == Transposition interpretation marker prefixed
//     *ITrd1c2  == Instrumental transposition marker prefixed
//

int Convert::transToBase40(const string& input) {
	int dval = 0;
	int cval = 0;
	if (sscanf(input.c_str(), "d%dc%d", &dval, &cval) != 2) {
		if (sscanf(input.c_str(), "*Trd%dc%d", &dval, &cval) != 2) {
			if (sscanf(input.c_str(), "*ITrd%dc%d", &dval, &cval) != 2) {
			   // cerr << "Cannot find correct information" << endl;
			   return 0;
			}
		}
	}

	int dsign = 1;
	// int csign = 1;
	if (dval < 0) {
		dsign = -1;
	}
	// if (cval < 0) {
	//    csign = -1;
	// }

	int doctave = dsign * dval / 7;
	// int coctave = csign * cval / 12;

	int base = 0;

		  if ((dval==0)  && (cval==0))   { base =	 0; }
	else if ((dval==0)  && (cval==1))   { base =	 1; }
	else if ((dval==0)  && (cval==2))   { base =	 2; }
	else if ((dval==1)  && (cval==0))   { base =	 4; }
	else if ((dval==1)  && (cval==1))   { base =	 5; }
	else if ((dval==1)  && (cval==2))   { base =	 6; }
	else if ((dval==1)  && (cval==3))   { base =	 7; }
	else if ((dval==1)  && (cval==4))   { base =	 8; }
	else if ((dval==2)  && (cval==2))   { base =	 10; }
	else if ((dval==2)  && (cval==3))   { base =	 11; }
	else if ((dval==2)  && (cval==4))   { base =	 12; }
	else if ((dval==2)  && (cval==5))   { base =	 13; }
	else if ((dval==2)  && (cval==6))   { base =	 14; }
	else if ((dval==3)  && (cval==3))   { base =	 15; }
	else if ((dval==3)  && (cval==4))   { base =	 16; }
	else if ((dval==3)  && (cval==5))   { base =	 17; }
	else if ((dval==3)  && (cval==6))   { base =	 18; }
	else if ((dval==3)  && (cval==7))   { base =	 19; }
	else if ((dval==4)  && (cval==5))   { base =	 21; }
	else if ((dval==4)  && (cval==6))   { base =	 22; }
	else if ((dval==4)  && (cval==7))   { base =	 23; }
	else if ((dval==4)  && (cval==8))   { base =	 24; }
	else if ((dval==4)  && (cval==9))   { base =	 25; }
	else if ((dval==5)  && (cval==7))   { base =	 27; }
	else if ((dval==5)  && (cval==8))   { base =	 28; }
	else if ((dval==5)  && (cval==9))   { base =	 29; }
	else if ((dval==5)  && (cval==10))  { base =	 30; }
	else if ((dval==5)  && (cval==11))  { base =	 31; }
	else if ((dval==6)  && (cval==9))   { base =	 33; }
	else if ((dval==6)  && (cval==10))  { base =	 34; }
	else if ((dval==6)  && (cval==11))  { base =	 35; }
	else if ((dval==6)  && (cval==12))  { base =	 36; }
	else if ((dval==6)  && (cval==13))  { base =	 37; }
	else if ((dval==7)  && (cval==10))  { base =	 38; }
	else if ((dval==7)  && (cval==11))  { base =	 38; }
	else if ((dval==-0) && (cval==-0))  { base =	 -0; }
	else if ((dval==-0) && (cval==-1))  { base =	 -1; }
	else if ((dval==-0) && (cval==-2))  { base =	 -2; }
	else if ((dval==-1) && (cval==1))   { base =	 -3; }
	else if ((dval==-1) && (cval==-0))  { base =	 -4; }
	else if ((dval==-1) && (cval==-1))  { base =	 -5; }
	else if ((dval==-1) && (cval==-2))  { base =	 -6; }
	else if ((dval==-1) && (cval==-3))  { base =	 -7; }
	else if ((dval==-2) && (cval==-1))  { base =	 -9; }
	else if ((dval==-2) && (cval==-2))  { base =	-10; }
	else if ((dval==-2) && (cval==-3))  { base =	-11; }
	else if ((dval==-2) && (cval==-4))  { base =	-12; }
	else if ((dval==-2) && (cval==-5))  { base =	-13; }
	else if ((dval==-3) && (cval==-3))  { base =	-15; }
	else if ((dval==-3) && (cval==-4))  { base =	-16; }
	else if ((dval==-3) && (cval==-5))  { base =	-17; }
	else if ((dval==-3) && (cval==-6))  { base =	-18; }
	else if ((dval==-3) && (cval==-7))  { base =	-19; }
	else if ((dval==-4) && (cval==-5))  { base =	-21; }
	else if ((dval==-4) && (cval==-6))  { base =	-22; }
	else if ((dval==-4) && (cval==-7))  { base =	-23; }
	else if ((dval==-4) && (cval==-8))  { base =	-24; }
	else if ((dval==-4) && (cval==-9))  { base =	-25; }
	else if ((dval==-5) && (cval==-6))  { base =	-26; }
	else if ((dval==-5) && (cval==-7))  { base =	-27; }
	else if ((dval==-5) && (cval==-8))  { base =	-28; }
	else if ((dval==-5) && (cval==-9))  { base =	-29; }
	else if ((dval==-5) && (cval==-10)) { base =	-30; }
	else if ((dval==-6) && (cval==-8))  { base =	-32; }
	else if ((dval==-6) && (cval==-9))  { base =	-33; }
	else if ((dval==-6) && (cval==-10)) { base =	-34; }
	else if ((dval==-6) && (cval==-11)) { base =	-35; }
	else if ((dval==-6) && (cval==-12)) { base =	-36; }
	else if ((dval==-7) && (cval==-10)) { base =	-38; }
	else if ((dval==-7) && (cval==-11)) { base =	-39; }
	else { // some error occurred or accidentals out of range
		// cerr << "Problem occured in transToBase40()" << endl;
		base = 0;
	}

	base += 40 * doctave * dsign;

	return base;
}



//////////////////////////////
//
// Convert::base40IntervalToLineOfFifths -- 0 => 0 (unison), 
//    Perfect Fifth => 1, Major second => 2 (two fifths up), etc.
//

int Convert::base40IntervalToLineOfFifths(int base40interval) {
	base40interval += 4000;
	base40interval = base40interval % 40;

	switch (base40interval) {
		case 0:    return   0;     // C
		case 1:    return   7;     // C#
		case 2:    return  14;     // C##
		case 3:    return 100;     // X
		case 4:    return -12;     // D--
		case 5:    return  -5;     // D-
		case 6:    return   2;     // D
		case 7:    return   9;     // D#
		case 8:    return  16;     // D##
		case 9:    return 100;     // X
		case 10:   return -10;     // E--
		case 11:   return  -3;     // E-
		case 12:   return   4;     // E
		case 13:   return  11;     // E#
		case 14:   return  18;     // E##
		case 15:   return -15;     // F--
		case 16:   return  -8;     // F-
		case 17:   return  -1;     // F
		case 18:   return   6;     // F#
		case 19:   return  13;     // F##
		case 20:   return 100;     // X
		case 21:   return -13;     // G--
		case 22:   return  -6;     // G-
		case 23:   return   1;     // G
		case 24:   return   8;     // G#
		case 25:   return  15;     // G##
		case 26:   return 100;     // X
		case 27:   return -11;     // A--
		case 28:   return  -4;     // A-
		case 29:   return   3;     // A
		case 30:   return  10;     // A#
		case 31:   return  17;     // A##
		case 32:   return 100;     // X
		case 33:   return  -9;     // B--
		case 34:   return  -2;     // B-
		case 35:   return   5;     // B
		case 36:   return  12;     // B#
		case 37:   return  19;     // B##
		case 38:   return -14;     // C--
		case 39:   return  -7;     // C-
		default:   return 100;     // X
	}

	return 100;
}



//////////////////////////////
//
// Convert::keyNumberToKern -- reverse of kernKeyToNumber.
//

string Convert::keyNumberToKern(int number) {
	switch (number) {
		case -7: return "*k[b-e-a-d-g-c-f-]";
		case -6: return "*k[b-e-a-d-g-c-]";
		case -5: return "*k[b-e-a-d-g-]";
		case -4: return "*k[b-e-a-d-]";
		case -3: return "*k[b-e-a-]";
		case -2: return "*k[b-e-]";
		case -1: return "*k[b-]";
		case  0: return "*k[]";
		case +1: return "*k[f#]";
		case +2: return "*k[f#c#]";
		case +3: return "*k[f#c#g#]";
		case +4: return "*k[f#c#g#d#]";
		case +5: return "*k[f#c#g#d#a#]";
		case +6: return "*k[f#c#g#d#a#e#]";
		case +7: return "*k[f#c#g#d#a#e#b#]";
		default: return "*k[]";
	}
}



//////////////////////////////
//
// Convert::base7ToBase40 -- Convert a base7 value to a base-40 value
//   (without accidentals).  Negative values are not allowed, but not
//   checked for.
//

int Convert::base7ToBase40(int base7) {
	int octave = base7 / 7;
	int b7pc = base7 % 7;
	int b40pc = 0;
	switch (b7pc) {
		case 0: b40pc =  0; break; // C
		case 1: b40pc =  6; break; // D
		case 2: b40pc = 12; break; // E
		case 3: b40pc = 17; break; // F
		case 4: b40pc = 23; break; // G
		case 5: b40pc = 29; break; // A
		case 6: b40pc = 35; break; // B
	}
	return octave * 40 + 2 + b40pc;
}





//////////////////////////////
//
// Convert::recipToDuration -- Convert **recip rhythmic values into
//     rational number durations in terms of quarter notes.  For example "4"
//     will be converted to 1, "4." to 3/2 (1+1/2).  The second parameter
//     is a scaling factor which can change the rhythmic value's base duration.
//     Giving a scale of 1 will return the duration in whole note units, so
//     "4" will return a value of 1/4 (one quarter of a whole note).  Using
//     3/2 will give the duration in terms of dotted-quarter note units.
//     The third parameter is the sub-token separate.  For example if the input
//     string contains a space, anything after the first space will be ignored
//     when extracting the string.  **kern data which also includes the pitch
//     along with the rhythm can also be given and will be ignored.
// default value: scale = 4 (duration in terms of quarter notes)
// default value: separator = " " (sub-token separator)
//

HumNum Convert::recipToDuration(string* recip, HumNum scale,
		const string& separator) {
	return Convert::recipToDuration(*recip, scale, separator);
}


HumNum Convert::recipToDuration(const string& recip, HumNum scale,
		const string& separator) {
	size_t loc;
	loc = recip.find(separator);
	string subtok;
	if (loc != string::npos) {
		subtok = recip.substr(0, loc);
	} else {
		subtok = recip;
	}

	loc = recip.find('q');
	if (loc != string::npos) {
		// grace note, ignore printed rhythm
		HumNum zero(0);
		return zero;
	}

	int dotcount = 0;
	int i;
	int numi = -1;
	for (i=0; i<(int)subtok.size(); i++) {
		if (subtok[i] == '.') {
			dotcount++;
		}
		if ((numi < 0) && isdigit(subtok[i])) {
			numi = i;
		}
	}
	loc = subtok.find("%");
	int numerator = 1;
	int denominator = 1;
	HumNum output;
	if (loc != string::npos) {
		// reciprocal rhythm
		numerator = 1;
		denominator = subtok[numi++] - '0';
		while ((numi<(int)subtok.size()) && isdigit(subtok[numi])) {
			denominator = denominator * 10 + (subtok[numi++] - '0');
		}
		if ((loc + 1 < subtok.size()) && isdigit(subtok[loc+1])) {
			int xi = (int)loc + 1;
			numerator = subtok[xi++] - '0';
			while ((xi<(int)subtok.size()) && isdigit(subtok[xi])) {
				numerator = numerator * 10 + (subtok[xi++] - '0');
			}
		}
		output.setValue(numerator, denominator);
	} else if (numi < 0) {
		// no rhythm found
		HumNum zero(0);
		return zero;
	} else if (subtok[numi] == '0') {
		// 0-symbol
		int zerocount = 1;
		for (i=numi+1; i<(int)subtok.size(); i++) {
			if (subtok[i] == '0') {
				zerocount++;
			} else {
				break;
			}
		}
		numerator = (int)pow(2, zerocount);
		output.setValue(numerator, 1);
	} else {
		// plain rhythm
		denominator = subtok[numi++] - '0';
		while ((numi<(int)subtok.size()) && isdigit(subtok[numi])) {
			denominator = denominator * 10 + (subtok[numi++] - '0');
		}
		output.setValue(1, denominator);
	}

	if (dotcount <= 0) {
		return output * scale;
	}

	int bot = (int)pow(2.0, dotcount);
	int top = (int)pow(2.0, dotcount + 1) - 1;
	HumNum factor(top, bot);
	return output * factor * scale;
}



//////////////////////////////
//
// Convert::recipToDurationNoDots -- Same as recipToDuration(), but ignore
//   any augmentation dots.
//

HumNum Convert::recipToDurationNoDots(string* recip, HumNum scale,
		const string& separator) {
	return Convert::recipToDurationNoDots(*recip, scale, separator);
}


HumNum Convert::recipToDurationNoDots(const string& recip, HumNum scale,
		const string& separator) {
	string temp = recip;
	std::replace(temp.begin(), temp.end(), '.', 'Z');
	return Convert::recipToDuration(temp, scale, separator);
}


//////////////////////////////
//
// Convert::durationToRecip -- Duration input is in units of quarter notes,
//     since the default value for scale is 1/4.
//

string Convert::durationToRecip(HumNum duration, HumNum scale) {
	duration *= scale;
	if (duration.getNumerator() == 1) {
		// simple rhythm (integer divisions of the whole note)
		return to_string(duration.getDenominator());
	}
	if (duration.getNumerator() == 0) {
		// grace note
		return "q";
	}

	// now decide if the rhythm can be represented simply with one dot.
	HumNum test1dot = (duration * 2) / 3;
	if (test1dot.getNumerator() == 1) {
		// single dot works
		string output = to_string(test1dot.getDenominator() * 2);
		output += ".";
		return output;
	}

	// now decide if the rhythm can be represented simply with two dots.
	HumNum test2dot = (duration * 4) / 7;
	if (test2dot.getNumerator() == 1) {
		// double dot works
		string output = to_string(test2dot.getDenominator() * 4);
		output += "..";
		return output;
	}

	// now decide if the rhythm can be represented simply with three dots.
	HumNum test3dot = (duration * 8) / 15;
	if (test3dot.getNumerator() == 1) {
		// single dot works
		string output = to_string(test3dot.getDenominator() * 8);
		output += "...";
		return output;
	}

	// duration required more than three dots or is not simple,
	// so assume that it is not simple:
	string output = to_string(duration.getDenominator());
	output += "%";
	output = to_string(duration.getNumerator());
	return output;
}



//////////////////////////////
//
// Convert::durationFloatToRecip -- not allowed to have more than
//	three rhythmic dots
//	default value: timebase = 1;
//

string Convert::durationFloatToRecip(double input, HumNum timebase) {
	string output;

   double testinput = input;
   double basic = 4.0 / input * timebase.getFloat();
   double diff = basic - (int)basic;

   if (diff > 0.998) {
      diff = 1.0 - diff;
      basic += diff;
   }

	// do power of two checks instead
   if (input == 0.0625)  { output = "64"; return output; }
   if (input == 0.125)   { output = "32"; return output; }
   if (input == 0.25)    { output = "16"; return output; }
   if (input == 0.5)  { output = "8";    return output; }
   if (input == 1.0)  { output = "4";    return output; }
   if (input == 2.0)  { output = "2";    return output; }
   if (input == 4.0)  { output = "1";    return output; }
   if (input == 8.0)  { output = "0";    return output; }
   if (input == 12.0) { output = "0.";   return output; }
   if (input == 16.0) { output = "00";   return output; }
   if (input == 24.0) { output = "00.";  return output; }
   if (input == 32.0) { output = "000";  return output; }
   if (input == 48.0) { output = "000."; return output; }

   // special case for triplet whole notes:
   if (fabs(input - (4.0 * 2.0 / 3.0)) < 0.0001) {
		return "3%2";
   }

   // special case for triplet breve notes:
   if (fabs(input - (4.0 * 4.0 / 3.0)) < 0.0001) {
		return "3%4";
   }

   // special case for 9/8 full rests
   if (fabs(input - (4.0 * 9.0 / 8.0)) < 0.0001) {
		return "8%9";
   }

   // special case for 9/2 full-measure rest
   if (fabs(input - 18.0) < 0.0001) {
		return "2%9";
   } 

   // handle special rounding cases primarily for SCORE which
   // only stores 4 digits for a duration
   if (input == 0.0833) {
      // triplet 32nd note, which has a real duration of 0.0833333 etc.
		return "48";
   }
	    
   if (diff < 0.002) {
		output += to_string((int)basic);
   } else { 
      testinput = input / 3.0 * 2.0;
      basic = 4.0 / testinput;
      diff = basic - (int)basic;
      if (diff < 0.002) {
			output += to_string((int)basic);
			output += ".";
      } else {
         testinput = input / 7.0 * 4.0;
         basic = 4.0 / testinput;
         diff = basic - (int)basic;
         if (diff < 0.002) {
				output += to_string((int)basic);
            output += "..";
         } else {
            testinput = input / 15.0 * 4.0;
            basic = 2.0 / testinput;
            diff = basic - (int)basic;
            if (diff < 0.002) {
					output += to_string((int)basic);
               output += "...";
            } else {
					// Don't know what it could be so echo as a grace note.
					output += "q";
					output += to_string(input);
            }
         }
      }
   }

   return output;
}





//////////////////////////////
//
// Convert::replaceOccurrences -- Similar to s// regular expressions
//    operator.  This function replaces the search string in the source
//    string with the replace string.
//

void Convert::replaceOccurrences(string& source, const string& search,
		const string& replace) {
	for (int loc=0; ; loc += (int)replace.size()) {
		loc = (int)source.find(search, loc);
		if (loc == (int)string::npos) {
			break;
		}
		source.erase(loc, search.length());
		source.insert(loc, replace);
	}
}



//////////////////////////////
//
// Convert::splitString -- Splits a string into a list of strings
//   separated by the given character.  Empty strings will be generated
//   if the separator occurs at the start/end of the input string, and
//   if two or more separates are adjacent to each other.
// default value: separator = ' ';
//

vector<string> Convert::splitString(const string& data, char separator) {
	stringstream ss(data);
	string key;
	vector<string> output;
	while (getline(ss, key, separator)) {
		output.push_back(key);
	}
	if (output.size() == 0) {
		output.push_back(data);
	}
	return output;
}



//////////////////////////////
//
// Convert::repeatString -- Returns a string which repeats the given
//   pattern by the given count.
//

string Convert::repeatString(const string& pattern, int count) {
	string output;
	for (int i=0; i<count; i++) {
		output += pattern;
	}
	return output;
}


//////////////////////////////
//
// Convert::encodeXml -- Encode a string for XML printing.  Ampersands
//    get converted to &amp;, < to &lt; > to &gt;, " to &quot; and
//    ' to &apos;.
//

string Convert::encodeXml(const string& input) {
	string output;
	output.reserve(input.size()*2);
	for (int i=0; i<(int)input.size(); i++) {
		switch (input[i]) {
			case '&':  output += "&amp;";   break;
			case '<':  output += "&lt;";    break;
			case '>':  output += "&gt;";    break;
			case '"':  output += "&quot;";  break;
			case '\'': output += "&apos;";  break;
			default:   output += input[i];
		}
	}
	return output;
}



//////////////////////////////
//
// Convert::getHumNumAttributes -- Returns XML attributes for a HumNum
//   number.  First @float which gives the floating-point representation.
//   If the number has a fractional part, then also add @ratfrac with the
//   fractional representation of the non-integer portion number.
//

string Convert::getHumNumAttributes(const HumNum& num) {
	string output;
	if (num.isInteger()) {
		output += " float=\"" + to_string(num.getNumerator()) + "\"";
	} else {
		stringstream sstr;
		sstr << num.toFloat();
		output += " float=\"" + sstr.str() + "\"";
	}
	if (!num.isInteger()) {
		HumNum rem = num.getRemainder();
		output += " ratfrac=\"" + to_string(rem.getNumerator()) +
				+ "/" + to_string(rem.getDenominator()) + "\"";
	}
	return output;
}



//////////////////////////////
//
// Convert::trimWhiteSpace -- remove spaces, tabs and/or newlines
//     from the beginning and end of input string.
//

string Convert::trimWhiteSpace(const string& input) {
	string s = input;
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}



//////////////////////////////
//
// Convert::startsWith --
//

bool Convert::startsWith(const string& input, const string& searchstring) {
	return input.compare(0, searchstring.size(), searchstring) == 0;
}


/////////////////////////////
//
// Convert::contains -- Returns true if the character or string
//    is found in the string.
//

bool Convert::contains(const string& input, const string& pattern) {
	return input.find(pattern) != string::npos;
}

bool Convert::contains(const string& input, char pattern) {
	return input.find(pattern) != string::npos;
}

bool Convert::contains(string* input, const string& pattern) {
	return Convert::contains(*input, pattern);
}

bool Convert::contains(string* input, char pattern) {
	return Convert::contains(*input, pattern);
}


//////////////////////////////
//
// Convert::makeBooleanTrackList -- Given a string
//   such as "1,2,3" and a max track of 5, then
//   create a vector with contents:
//      0:false, 1:true, 2:true, 3:true, 4:false, 5:false.
//   The 0 track is not used, and the two tracks not specified
//   in the string are set to false.  Special abbreviations:
//     $ = maxtrack
//     $1 = maxtrack - 1
//     $2 = maxtrack - 2
//     etc.
//   Ranges can be given, such as 1-3 instead of 1,2,3
//

void Convert::makeBooleanTrackList(vector<bool>& spinelist,
		 const string& spinestring, int maxtrack) {
   spinelist.resize(maxtrack+1);

	if (spinestring.size() == 0) {
		fill(spinelist.begin()+1, spinelist.end(), true);
		return;
	}
	fill(spinelist.begin(), spinelist.end(), false);

   string buffer = spinestring;;
	vector<string> entries;
	string separator = "[^\\d\\$-]+";
   HumRegex hre;

	// create an initial list of values:
	hre.split(entries, buffer, separator);

	// Now process each token in the extracted list:
	int val = -1;
	int val2 = -1;
	bool range = false;
	string tbuff;
	for (int i=0; i<(int)entries.size(); i++) {

		if (hre.search(entries[i], "\\$(\\d*)")) {
			if (hre.getMatch(1).size() == 0) {
				tbuff = to_string(maxtrack);
			} else {
				val = hre.getMatchInt(1);
				tbuff = to_string(maxtrack - val);
			}
			hre.replaceDestructive(entries[i], tbuff, "\\$\\d+");
		}

		range = false;
		if (entries[i].find('-') != string::npos) {
			range = true;
			// check for second $ abbreviation at end of range:
			if (hre.search(entries[i], "\\$(\\d*)")) {
				if (hre.getMatch(1).size() == 0) {
					tbuff = to_string(maxtrack);
				} else {
					val = hre.getMatchInt(1);
					tbuff = to_string(maxtrack - val);
				}
				hre.replaceDestructive(entries[i], tbuff, "\\$\\d+");
			}
			if (entries[i].back() == '$') {
				entries[i].pop_back();
				entries[i] += to_string(maxtrack);
			}
			// extract second vlaue
			if (hre.search(entries[i], "-(\\d+)")) {
				val2 = hre.getMatchInt(1);
			} else {
				range = false;
			}
		}


		// get first value:
		if (hre.search(entries[i], "(\\d+)")) {
			val = stoi(hre.getMatch(1));
		}
		if (range) {
			int direction = 1;
			if (val > val2) {
				direction = -1;
			}
			for (int j=val; j != val2; j += direction) {
				if ((j > 0) && (j < maxtrack + 1)) {
					spinelist[j] = true;
				}
			}
			if ((val2 > 0) && (val2 < maxtrack + 1)) {
				spinelist[val2] = true;
			}
		} else {
			// not a range
			if ((val > 0) && (val < maxtrack+1)) {
				spinelist[val] = true;
			}
		}
	}
}



//////////////////////////////
//
// GridMeasure::GridMeasure -- Constructor.
//

GridMeasure::GridMeasure(HumGrid* owner) {
	m_owner = owner;
	m_style = MeasureStyle::Plain;
}



//////////////////////////////
//
// GridMeasure::~GridMeasure -- Deconstructor.
//

GridMeasure::~GridMeasure(void) {
	for (auto it = this->begin(); it != this->end(); it++) {
		if (*it) {
			delete *it;
			*it = NULL;
		}
	}
}



//////////////////////////////
//
// GridMeasure::transferTokens --
//

bool GridMeasure::transferTokens(HumdrumFile& outfile, bool recip,
		bool addbar) {

	// If the last data slice duration is zero, then calculate
	// the true duration from the duration of the measure.
	if (this->size() > 0) {
		GridSlice* slice = back();
		if (slice->isMeasureSlice() && (this->size() >= 2)) {
			auto ending = this->end();
			--ending;
			--ending;
			while ((ending != this->begin()) && (!(*ending)->isDataSlice())) {
				--ending;
			}
			slice = *ending;
		} else {
			slice = NULL;
		}
		if ((slice != NULL) && slice->isDataSlice() 
				&& (slice->getDuration() == 0)) {
			HumNum mts  = getTimestamp();
			HumNum mdur = getDuration();
			HumNum sts  = slice->getTimestamp();
			HumNum slicedur = (mts + mdur) - sts;
			slice->setDuration(slicedur);
		}
	}

	bool founddata = false;
	bool addedbar = false;

	for (auto it : *this) {
		if (it->isInvalidSlice()) {
			// ignore slices to be removed from output (used for 
			// removing redundant clef slices).
			continue;
		}
		if (it->isDataSlice()) {
			founddata = true;
		}
		if (it->isManipulatorSlice()) {
			// didn't acutally find data, but the barline should
			// be placed before any manipulator (a spine split), since
			// that is more a property of the data than of the header
			// interpretations.
			founddata = true;
		}
		if (founddata && addbar && !addedbar) {
			appendInitialBarline(outfile);
			addedbar = true;
		}
		it->transferTokens(outfile, recip);
	}
	return true;
}



//////////////////////////////
//
// GridMeasure::appendInitialBarline -- The barline will be
//    duplicated to all spines later.
//

void GridMeasure::appendInitialBarline(HumdrumFile& infile) {
	if (infile.getLineCount() == 0) {
		// strange case which should never happen.
		return;
	}
	int fieldcount = infile.back()->getFieldCount();
	HumdrumLine* line = new HumdrumLine;
	HTp token;
	for (int i=0; i<fieldcount; i++) {
		token = new HumdrumToken("=1-");
		line->appendToken(token);
	}
	infile.push_back(line);
}



//////////////////////////////
//
// GridMeasure::getOwner --
//

HumGrid* GridMeasure::getOwner(void) {
	return m_owner;
}



//////////////////////////////
//
// GridMeasure::setOwner --
//

void GridMeasure::setOwner(HumGrid* owner) {
	m_owner = owner;
}



//////////////////////////////
//
// GridMeasure::setDuration --
//

void GridMeasure::setDuration(HumNum duration) {
	m_duration = duration;
}



//////////////////////////////
//
// GridMeasure::getDuration --
//

HumNum GridMeasure::getDuration(void) {
	return m_duration;
}



//////////////////////////////
//
// GridMeasure::getTimestamp --
//

HumNum GridMeasure::getTimestamp(void) {
	return m_timestamp;
}



//////////////////////////////
//
// GridMeasure::setTimestamp --
//

void GridMeasure::setTimestamp(HumNum timestamp) {
	m_timestamp = timestamp;
}



//////////////////////////////
//
// GridMeasure::getTimeSigDur --
//

HumNum GridMeasure::getTimeSigDur(void) {
	return m_timesigdur;
}



//////////////////////////////
//
// GridMeasure::setTimeSigDur --
//

void GridMeasure::setTimeSigDur(HumNum duration) {
	m_timesigdur = duration;
}



//////////////////////////////
//
// GridPart::GridPart -- Constructor.
//

GridPart::GridPart(void) : GridSide() {
	// do nothing;
}

//////////////////////////////
//
// GridPart::~GridPart -- Deconstructor: delete any GridStaff items
//     being stored.
//

GridPart::~GridPart(void) {
	for (int i=0; i<(int)this->size(); i++) {
		if (this->at(i)) {
			delete this->at(i);
			this->at(i) = NULL;
		}
	}
}


//////////////////////////////
//
// operator<< -- print the contents of a GridPart data structure --
//

ostream& operator<<(ostream& output, GridPart* part) {
	if (part == NULL) {
		output << "{n}";
		return output;
	}
	for (int s=0; s<(int)part->size(); s++) {
		GridStaff* staff = part->at(s);
		output << "(s" << s << ":)";
		if (staff == NULL) {
			output << "{n}";
			continue;
		}
		for (int t=0; t<(int)staff->size(); t++) {
			GridVoice* gt = staff->at(t);
			output << "(v" << t << ":)";
			if (gt == NULL) {
				output << "{n}";
				continue;
			} else {
				HTp token = gt->getToken();
				if (token == NULL) {
					output << "{n}";
				} else {
					output << " \"" << *token << "\" ";
				}
			}
		}
	}
	return output;
}


ostream& operator<<(ostream& output, GridPart& part) {
	output << &part;
	return output;
}



//////////////////////////////
//
// GridSide::GridSide -- Constructor.
//

GridSide::GridSide(void) {
	m_harmony = NULL;
}



//////////////////////////////
//
// GridSide::~GridSide -- Deconstructor.
//

GridSide::~GridSide(void) {

	for (int i=0; i<(int)m_verses.size(); i++) {
		if (m_verses[i]) {
			delete m_verses[i];
			m_verses[i] = NULL;
		}
	}
	m_verses.resize(0);

	for (int i=0; i<(int)m_dynamics.size(); i++) {
		if (m_dynamics[i]) {
			delete m_dynamics[i];
			m_dynamics[i] = NULL;
		}
	}
	m_dynamics.resize(0);

	if (m_harmony) {
		delete m_harmony;
		m_harmony = NULL;
	}
}



//////////////////////////////
//
// GridSide::setVerse --
//

void GridSide::setVerse(int index, HTp token) {
   if (index == (int)m_verses.size()) {
		m_verses.push_back(token);
		return;
	} else if (index < 0) {
		return;
	} else if (index < (int)m_verses.size()) {
		m_verses[index] = token;
	} else {
		int oldsize = (int)m_verses.size();
		int newsize = index + 1;
		m_verses.resize(newsize);
		for (int i=oldsize; i<newsize; i++) {
			m_verses[i] = NULL;
		}
		m_verses[index] = token;
	}
}



//////////////////////////////
//
// GridSide::getVerse --
//

HTp GridSide::getVerse(int index) {
	if (index < 0 || index >= getVerseCount()) {
		return NULL;
	}
	return m_verses[index];
}



//////////////////////////////
//
// GridSide::getVerseCount --
//

int GridSide::getVerseCount(void) {
 	return (int)m_verses.size();
}



//////////////////////////////
//
// GridSide::getHarmonyCount --
//

int GridSide::getHarmonyCount(void) {
	if (m_harmony == NULL) {
		return 0;
	} else {
		return 1;
	}
}



//////////////////////////////
//
// GridSide::setHarmony --
//

void GridSide::setHarmony(HTp token) {
	if (m_harmony) {
		delete m_harmony;
		m_harmony = NULL;
	}
	m_harmony = token;
}



///////////////////////////
//
// GridSide::detachHarmony --
//

void GridSide::detachHarmony(void) {
	m_harmony = NULL;
}



//////////////////////////////
//
// GridSide::getHarmony --
//

HTp GridSide::getHarmony(void) {
	return m_harmony;
}



//////////////////////////////
//
// GridSlice::GridSlice -- Constructor.  If partcount is positive, then
//    allocate the desired number of parts (still have to allocate staves
//    in part before using).
// default value: partcount = 0
//

GridSlice::GridSlice(GridMeasure* measure, HumNum timestamp, SliceType type,
		int partcount) {
	m_timestamp = timestamp;
	m_type      = type;
	m_owner     = NULL;
	m_measure   = measure;
	if (m_measure) {
		m_owner = measure->getOwner();
		m_measure = measure;
	}
	if (partcount > 0) {
		this->resize(partcount);
		for (int p=0; p<partcount; p++) {
			this->at(p) = new GridPart;
		}
	}
}


//
// This constructor allocates the matching part and staff count of the
// input slice parameter.  There will be no GridVoices allocated inside the
// GridStaffs (they will be required to have at least one).
//

GridSlice::GridSlice(GridMeasure* measure, HumNum timestamp, SliceType type,
		const GridSlice& slice) {
	m_timestamp = timestamp;
	m_type = type;
	if (m_measure) {
		m_owner = measure->getOwner();
		m_measure = measure;
	} else {
		m_owner = NULL;
		m_measure = NULL;
	}
	int partcount = (int)slice.size();
	int staffcount;
	if (partcount > 0) {
		this->resize(partcount);
		for (int p=0; p<partcount; p++) {
			this->at(p) = new GridPart;
			GridPart* part = this->at(p);
			staffcount = (int)slice.at(p)->size();
			part->resize(staffcount);
			for (int s=0; s<staffcount; s++) {
				part->at(s) = new GridStaff;
			}
		}
	}
}


GridSlice::GridSlice(GridMeasure* measure, HumNum timestamp, SliceType type,
		GridSlice* slice) {
	m_timestamp = timestamp;
	m_type = type;
	if (m_measure) {
		m_owner = measure->getOwner();
		m_measure = measure;
	} else {
		m_owner = NULL;
		m_measure = NULL;
	}
	int partcount = (int)slice->size();
	int staffcount;
	if (partcount > 0) {
		this->resize(partcount);
		for (int p=0; p<partcount; p++) {
			this->at(p) = new GridPart;
			GridPart* part = this->at(p);
			staffcount = (int)slice->at(p)->size();
			part->resize(staffcount);
			for (int s=0; s<staffcount; s++) {
				part->at(s) = new GridStaff;
			}
		}
	}
}



//////////////////////////////
//
// GridSlice::~GridSlice -- Deconstructor.
//

GridSlice::~GridSlice(void) {
	for (int i=0; i<(int)this->size(); i++) {
		if (this->at(i)) {
			delete this->at(i);
			this->at(i) = NULL;
		}
	}
}



//////////////////////////////
//
// GridSlice::createRecipTokenFromDuration --  Will not be able to
//   distinguish between triplet notes and dotted normal equivalents,
//   this can be changed later by checking neighboring durations in the
//   list for the presence of triplets.
//

HTp GridSlice::createRecipTokenFromDuration(HumNum duration) {
	duration /= 4;  // convert to quarter note units.
	HTp token;
	string str;
	HumNum dotdur;
	if (duration.getNumerator() == 0) {
		// if the GridSlice is at the end of a measure, the
      // time between the starttime/endtime of the GridSlice should
		// be subtracted from the endtime of the current GridMeasure.
		token = new HumdrumToken("g");
		return token;
	} else if (duration.getNumerator() == 1) {
		token = new HumdrumToken(to_string(duration.getDenominator()));
		return token;
	} else if (duration.getNumerator() % 3 == 0) {
		dotdur = ((duration * 2) / 3);
		if (dotdur.getNumerator() == 1) {
			token = new HumdrumToken(to_string(dotdur.getDenominator()) + ".");
			return token;
		}
	}

	// try to fit to two dots here

	// try to fit to three dots here

	str = to_string(duration.getDenominator()) + "%" +
	         to_string(duration.getNumerator());
	token = new HumdrumToken(str);
	return token;
}



//////////////////////////////
//
// GridSlice::isInterpretationSlice --
//

bool GridSlice::isInterpretationSlice(void) {
	SliceType type = getType();
	if (type < SliceType::_Measure) {
		return false;
	}
	if (type > SliceType::_Interpretation) {
		return false;
	}
	return true;
}



//////////////////////////////
//
// GridSlice::isDataSlice --
//

bool GridSlice::isDataSlice(void) {
	SliceType type = getType();
	if (type <= SliceType::_Data) {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// GridSlice::transferTokens -- Create a HumdrumLine and append it to
//    the data.
//

void GridSlice::transferTokens(HumdrumFile& outfile, bool recip) {
	HTp token;
	HumdrumLine* line = new HumdrumLine;
	GridVoice* voice;
	string empty = ".";
	if (isMeasureSlice()) {
		if (this->at(0)->at(0)->size() > 0) {
			voice = this->at(0)->at(0)->at(0);
			empty = (string)*voice->getToken();
		} else {
			empty = "=";
		}
	} else if (isInterpretationSlice()) {
		empty = "*";
	}

	if (recip) {
		if (isNoteSlice()) {
			token = createRecipTokenFromDuration(getDuration());
		} else if (isClefSlice()) {
			token = new HumdrumToken("*");
			empty = "*";
		} else if (isMeasureSlice()) {
			if (this->at(0)->at(0)->size() > 0) {
				voice = this->at(0)->at(0)->at(0);
				token = new HumdrumToken((string)*voice->getToken());
			} else {
				token = new HumdrumToken("=X");
			}
			empty = (string)*token;
		} else if (isInterpretationSlice()) {
			token = new HumdrumToken("*");
			empty = "*";
		} else if (isGraceSlice()) {
			token = new HumdrumToken("q");
			empty = ".H";
		} else {
			token = new HumdrumToken("55");
			empty = "!z";
		}
		line->appendToken(token);
	}

	// extract the Tokens from each part/staff
	int p; // part index
	int s; // staff index
	int v; // voice index

	for (p=(int)size()-1; p>=0; p--) {

		GridPart& part = *this->at(p);
		for (s=(int)part.size()-1; s>=0; s--) {
			GridStaff& staff = *part.at(s);
			if (staff.size() == 0) {
				// fix this later.  For now if there are no notes
				// on the staff, add a null token.  Fix so that
				// all open voices are given null tokens.
				token = new HumdrumToken(empty);
				line->appendToken(token);
			} else {
				for (v=0; v<(int)staff.size(); v++) {
					if (staff.at(v) && staff.at(v)->getToken()) {
						line->appendToken(staff.at(v)->getToken());
						staff.at(v)->forgetToken();
					} else if (!staff.at(v)) {
						token = new HumdrumToken(".");
						line->appendToken(token);
					} else {
						token = new HumdrumToken(".");
						line->appendToken(token);
					}
				}

			}
			int maxvcount = getVerseCount(p, s);
			int maxhcount = getHarmonyCount(p, s);
			transferSides(*line, staff, empty, maxvcount, maxhcount);
		}
		int maxhcount = getHarmonyCount(p);
		int maxvcount = getVerseCount(p, -1);
		transferSides(*line, part, empty, maxvcount, maxhcount);
	}

	outfile.appendLine(line);
}



//////////////////////////////
//
// GridSlice::getMeasureDuration --
//

HumNum GridSlice::getMeasureDuration(void) {
	GridMeasure* measure = getMeasure();
	if (!measure) {
		return -1;
	} else {
		return measure->getDuration();
	}
}



//////////////////////////////
//
// GridSlice::getMeasureTimestamp -- Return the start time of the measure.
//

HumNum GridSlice::getMeasureTimestamp(void) {
	GridMeasure* measure = getMeasure();
	if (!measure) {
		return -1;
	} else {
		return measure->getTimestamp();
	}
}



//////////////////////////////
//
// GridSlice::getVerseCount --
//

int GridSlice::getVerseCount(int partindex, int staffindex) {
	HumGrid* grid = getOwner();
	if (!grid) {
		return 0;
	}
	return grid->getVerseCount(partindex, staffindex);
}



//////////////////////////////
//
// GridSlice::getHarmonyCount --
//    default value: staffindex = -1; (currently not looking for
//        harmony data attached directly to staff (only to part.)
//

int GridSlice::getHarmonyCount(int partindex, int staffindex) {
	HumGrid* grid = getOwner();
	if (!grid) {
		return 0;
	}
	if (staffindex >= 0) {
		// ignoring staff-level harmony
		return 0;
	} else {
		return grid->getHarmonyCount(partindex);
	}
}



//////////////////////////////
//
// GridSlice::transferSides --
//


// this version is used to transfer Sides from the Part
void GridSlice::transferSides(HumdrumLine& line, GridPart& sides,
		const string& empty, int maxvcount, int maxhcount) {

	int hcount = sides.getHarmonyCount();
	int vcount = sides.getVerseCount();
	HTp newtoken;

	for (int i=0; i<vcount; i++) {
		HTp verse = sides.getVerse(i);
		if (verse) {
			line.appendToken(verse);
			sides.detachHarmony();
		} else {
			newtoken = new HumdrumToken(empty);
			line.appendToken(newtoken);
		}
	}

	for (int i=vcount; i<maxvcount; i++) {
		newtoken = new HumdrumToken(empty);
		line.appendToken(newtoken);
	}

	for (int i=0; i<hcount; i++) {
		HTp harmony = sides.getHarmony();
		if (harmony) {
			line.appendToken(harmony);
			sides.detachHarmony();
		} else {
			newtoken = new HumdrumToken(empty);
			line.appendToken(newtoken);
		}
	}

	for (int i=hcount; i<maxhcount; i++) {
		newtoken = new HumdrumToken(empty);
		line.appendToken(newtoken);
	}
}


// this version is used to transfer Sides from the Staff
void GridSlice::transferSides(HumdrumLine& line, GridStaff& sides,
		const string& empty, int maxvcount, int maxhcount) {

	// existing verses:
	int vcount = sides.getVerseCount();

	// there should not be any harony attached to staves
	// (only to parts, so hcount should only be zero):
	int hcount = sides.getHarmonyCount();
	HTp newtoken;


	for (int i=0; i<vcount; i++) {
		HTp verse = sides.getVerse(i);
		if (verse) {
			line.appendToken(verse);
			sides.setVerse(i, NULL); // needed to avoid double delete
		} else {
			newtoken = new HumdrumToken(empty);
			line.appendToken(newtoken);
		}
	}

	if (vcount < maxvcount) {
		for (int i=vcount; i<maxvcount; i++) {
			newtoken = new HumdrumToken(empty);
			line.appendToken(newtoken);
		}
	}

	for (int i=0; i<hcount; i++) {
		HTp harmony = sides.getHarmony();
		if (harmony) {
			line.appendToken(harmony);
			sides.detachHarmony();
		} else {
			newtoken = new HumdrumToken(empty);
			line.appendToken(newtoken);
		}
	}

	if (hcount < maxhcount) {
		for (int i=hcount; i<maxhcount; i++) {
			newtoken = new HumdrumToken(empty);
			line.appendToken(newtoken);
		}
	}
}



//////////////////////////////
//
// GridSlice::initializePartStaves -- Also initialize sides
//

void GridSlice::initializePartStaves(vector<MxmlPart>& partdata) {
	int i, j;
	if (this->size() > 0) {
		// strange that this should happen, but presume the data
		// needs to be deleted.
		for (int i=0; i<(int)this->size(); i++) {
			if (this->at(i)) {
				delete this->at(i);
				this->at(i) = NULL;
			}
		}
	}
	this->resize(partdata.size());

	for (i=0; i<(int)partdata.size(); i++) {
		this->at(i) = new GridPart;
		this->at(i)->resize(partdata[i].getStaffCount());
		for (j=0; j<(int)partdata[i].getStaffCount(); j++) {
			this->at(i)->at(j) = new GridStaff;
		}
	}

}



//////////////////////////////
//
// GridSlice::getDuration -- Return the duration of the slice in
//      quarter notes.
//

HumNum GridSlice::getDuration(void) {
	return m_duration;
}



//////////////////////////////
//
// GridSlice::setDuration --
//

void GridSlice::setDuration(HumNum duration) {
	m_duration = duration;
}



//////////////////////////////
//
// GridSlice::getTimestamp --
//

HumNum GridSlice::getTimestamp(void) {
	return m_timestamp;
}



//////////////////////////////
//
// GridSlice::setTimestamp --
//

void GridSlice::setTimestamp(HumNum timestamp) {
	m_timestamp = timestamp;
}



//////////////////////////////
//
// GridSlice::setOwner --
//

void GridSlice::setOwner(HumGrid* owner) {
	m_owner = owner;
}



//////////////////////////////
//
// GridSlice::getOwner --
//

HumGrid* GridSlice::getOwner(void) {
	return m_owner;
}



//////////////////////////////
//
// GridSlice::getMeasure --
//

GridMeasure* GridSlice::getMeasure(void) {
	return m_measure;
}



//////////////////////////////
//
// operator<< -- print token content of a slice
//

ostream& operator<<(ostream& output, GridSlice* slice) {
	if (slice == NULL) {
		output << "{n}";
		return output;
	}
	for (int p=0; p<(int)slice->size(); p++) {
		GridPart* part = slice->at(p);
		output << "(p" << p << ":)";
		if (part == NULL) {
			output << "{n}";
			continue;
		}
		for (int s=0; s<(int)part->size(); s++) {
			GridStaff* staff = part->at(s);
			output << "(s" << s << ":)";
			if (staff == NULL) {
				output << "{n}";
				continue;
			}
			for (int t=0; t<(int)staff->size(); t++) {
				GridVoice* gt = staff->at(t);
				output << "(v" << t << ":)";
				if (gt == NULL) {
					output << "{n}";
					continue;
				} else {
					HTp token = gt->getToken();
					if (token == NULL) {
						output << "{n}";
					} else {
						output << " \"" << *token << "\" ";
					}
				}

			}
		}
	}
	return output;
}



//////////////////////////////
//
// GridSlice::invalidate -- Mark the slice as invalid, which means that
//    it should not be transferred to the output Humdrum file in HumGrid.
//    Tokens stored in the GridSlice will be deleted by GridSlice when it
//    is destroyed.
//

void GridSlice::invalidate(void) {
		m_type = SliceType::Invalid;
		// should only do with 0 duration slices, but force to 0 if not already.
		setDuration(0);
}



//////////////////////////////
//
// GridStaff::GridStaff -- Constructor.
//

GridStaff::GridStaff(void) : vector<GridVoice*>(0), GridSide() {
	// do nothing;
}



//////////////////////////////
//
// GridStaff::~GridStaff -- Deconstructor.
//

GridStaff::~GridStaff(void) {
	for (int i=0; i<(int)this->size(); i++) {
		if (this->at(i)) {
			delete this->at(i);
			this->at(i) = NULL;
		}
	}
}



//////////////////////////////
//
// GridStaff::setTokenLayer -- Insert a token at the given voice/layer index.
//    If there is another token already there, then delete it.  If there
//    is no slot for the given voice, then create one and fill in all of the
//    other new ones with NULLs.
//

GridVoice* GridStaff::setTokenLayer(int layerindex, HTp token, HumNum duration) {
	if (layerindex < 0) {
		cerr << "Error: layer index is " << layerindex
		     << " for " << token << endl;
		return NULL;
	}
	if (layerindex > (int)this->size()-1) {
		int oldsize = (int)this->size();
		this->resize(layerindex+1);
		for (int i=oldsize; i<(int)this->size(); i++) {
			this->at(i) = NULL;
		}
	}
	if (this->at(layerindex) != NULL) {
		delete this->at(layerindex);
	}
	GridVoice* gv = new GridVoice(token, duration);
	this->at(layerindex) = gv;
	return gv;
}



////////////////////////////
//
// GridStaff::setNullTokenLayer --
//

void GridStaff::setNullTokenLayer(int layerindex, SliceType type,
		HumNum nextdur) {

	string nulltoken;
	if (type < SliceType::_Data) {
		nulltoken = ".";
	} else if (type < SliceType::_Measure) {
		nulltoken = "=";
	} else if (type < SliceType::_Manipulator) {
		nulltoken = "*";
	} else if (type < SliceType::_Spined) {
		nulltoken = "!!";
	} else {
		cerr << "STRANGE ERROR" << endl;
	}

	if (layerindex < (int)this->size()) {
		if (at(layerindex) != NULL) {
			if ((string)*at(layerindex)->getToken() == nulltoken) {
				// there is already a null data token here, so don't 
				// replace it.
				return;
			}
			cerr << "Warning, replacing existing token: "
			     << *this->at(layerindex)->getToken()
			     << " with a null token"
			     << endl;
		}
	}
	HumdrumToken* token = new  HumdrumToken(nulltoken);
	setTokenLayer(layerindex, token, nextdur);

}



//////////////////////////////
//
// GridStaff::appendTokenLayer -- concatenate the string content
//   of a token onto the current token stored in the slot (or just
//   place this one in the slot if none there yet).  This is used for
//   chords normally.
//

void GridStaff::appendTokenLayer(int layerindex, HTp token, HumNum duration,
		const string& spacer) {

	GridVoice* gt;
	if (layerindex > (int)this->size()-1) {
		int oldsize = (int)this->size();
		this->resize(layerindex+1);
		for (int i=oldsize; i<(int)this->size(); i++) {
			this->at(i) = NULL;
		}
	}
	if (this->at(layerindex) != NULL) {
		string newtoken;
		newtoken = (string)*this->at(layerindex)->getToken();
		newtoken += spacer;
		newtoken += (string)*token;
		(string)*(this->at(layerindex)->getToken()) = newtoken;
	} else {
		gt = new GridVoice(token, duration);
		this->at(layerindex) = gt;
	}
}



//////////////////////////////
//
// GridStaff::getMaxVerseCount --
//

int GridStaff::getMaxVerseCount(void) {
	return 5;
// ggg
}



//////////////////////////////
//
// operator<< --
//

ostream& operator<<(ostream& output, GridStaff* staff) {
	if (staff == NULL) {
		output << "{n}";
		return output;
	}
	for (int t=0; t<(int)staff->size(); t++) {
		GridVoice* gt = staff->at(t);
		cout << "(v" << t << ":)";
		if (gt == NULL) {
			cout << "{gt:n}";
			continue;
		} else {
			HTp token = gt->getToken();
			if (token == NULL) {
				cout << "{n}";
			} else {
				cout << " \"" << *token << "\" ";
			}
		}
	}
	return output;
}



//////////////////////////////
//
// GridVoice::GridVoice -- Constructor.
//

GridVoice::GridVoice(void) {
	m_token      = NULL;
	m_transfered = false;
}

GridVoice::GridVoice(HTp token, HumNum duration) {
	m_token      = token;
	m_nextdur    = duration;
	m_transfered = false;
}


GridVoice::GridVoice(const char* token, HumNum duration) {
	m_token      = new HumdrumToken(token);
	m_nextdur    = duration;
	m_transfered = false;
}


GridVoice::GridVoice(const string& token, HumNum duration) {
	m_token      = new HumdrumToken(token);
	m_nextdur    = duration;
	m_transfered = false;
}



//////////////////////////////
//
// GridVoice::~GridVoice -- Deconstructor: delete the token only if it
//     has not been transfered to a HumdrumFile object.
//

GridVoice::~GridVoice() {
	if (m_token && !m_transfered) {
		delete m_token;
	}
	m_token = NULL;
}



//////////////////////////////
//
// GridVoice::isTransfered -- True if token was copied to a HumdrumFile
//      object.
//

bool GridVoice::isTransfered(void) {
	return m_transfered;
}



//////////////////////////////
//
// GridVoice::setTransfered -- True if the object should not be
//    deleted with the object is destroyed.  False if the token
//    is not NULL and should be deleted when object is destroyed.
//

void GridVoice::setTransfered(bool state) {
	m_transfered = state;
}



//////////////////////////////
//
// GridVoice::getToken --
//

HTp GridVoice::getToken(void) const {
	return m_token;
}



//////////////////////////////
//
// GridVoice::setToken --
//

void GridVoice::setToken(HTp token) {
	if (!m_transfered && m_token) {
		delete m_token;
	}
	m_token = token;
	m_transfered = false;
}


void GridVoice::setToken(const string& token) {
	HTp realtoken = new HumdrumToken(token);
	setToken(realtoken);
}


void GridVoice::setToken(const char* token) {
	HTp realtoken = new HumdrumToken(token);
	setToken(realtoken);
}



//////////////////////////////
//
// GridVoice::isNull -- returns true if token is NULL or ".".
//

bool GridVoice::isNull(void) const {
	if (getToken() == NULL) {
		return true;
	} else if (getToken()->isNull()) {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// GridVoice::setDuration --
//

void GridVoice::setDuration(HumNum duration) {
	m_nextdur = duration;
	m_prevdur = 0;
}



//////////////////////////////
//
// GridVoice::setDurationToPrev --
//

void GridVoice::setDurationToPrev(HumNum dur) {
	m_prevdur = dur;
}



//////////////////////////////
//
// GridVoice::getDurationToNext --
//

HumNum GridVoice::getDurationToNext(void) const {
	return m_nextdur;
}



//////////////////////////////
//
// GridVoice::getDurationToPrev --
//

HumNum GridVoice::getDurationToPrev(void) const {
	return m_nextdur;
}



//////////////////////////////
//
// GridVoice::incrementDuration --
//

void GridVoice::incrementDuration(HumNum duration) {
	m_nextdur -= duration;
	m_prevdur += duration;
}



//////////////////////////////
//
// GridVoice::forgetToken -- The HumdrumToken was passed off
//      to some other object which is now responsible for
//      deleting it.
//

void GridVoice::forgetToken(void) {
	setTransfered(true);
	m_token = NULL;
}



//////////////////////////////
//
// GridVoice::getDuration -- Return the total duration of the
//   durational item, the sum of the nextdur and prevdur.
//

HumNum GridVoice::getDuration(void) const {
	return m_nextdur + m_prevdur;
}


//////////////////////////////
//
// operator<< -- print token content of a voice
//

ostream& operator<<(ostream& output, GridVoice* voice) {
	if (voice == NULL) {
		output << "{n}";
		return output;
	}

	HTp token = voice->getToken();
	if (token == NULL) {
		cout << "{n}";
	} else {
		cout << " \"" << *token << "\" ";
	}
	return output;
}

ostream& operator<<(ostream& output, GridVoice& voice) {
	output << &voice;
	return output;
}




//////////////////////////////
//
// HumAddress::HumAddress -- HumAddress constructor.
//

HumAddress::HumAddress(void) {
	m_track         = -1;
	m_subtrack      = -1;
	m_subtrackcount = 0;
	m_fieldindex    = -1;
	m_owner         = NULL;
}


HumAddress::HumAddress(HumAddress& address) {
	m_fieldindex    = address.m_fieldindex;
	m_track         = address.m_track;
	m_subtrack      = address.m_subtrack;
	m_subtrackcount = address.m_subtrackcount;
	m_spining       = address.m_spining;
	m_owner         = address.m_owner;
}



//////////////////////////////
//
// HumAddress::~HumAddress -- HumAddress deconstructor.
//

HumAddress::~HumAddress() {
	m_track         = -1;
	m_subtrack      = -1;
	m_fieldindex    = -1;
	m_subtrackcount = 0;
	m_owner         = NULL;
}



//////////////////////////////
//
// HumAddress::operator= -- Copy humdrum address to another object.
//

HumAddress& HumAddress::operator=(const HumAddress& address) {
	m_fieldindex    = address.m_fieldindex;
	m_track         = address.m_track;
	m_subtrack      = address.m_subtrack;
	m_subtrackcount = address.m_subtrackcount;
	m_spining       = address.m_spining;
	m_owner         = address.m_owner;
	return *this;
}


//////////////////////////////
//
// HumAddress::getLineIndex -- Returns the line index in the owning HumdrumFile
//    for the token associated with the address.  Returns -1 if not owned by a
//    HumdrumLine (or line assignments have not been made for tokens in the
//    file).
//

int  HumAddress::getLineIndex(void) const {
	if (m_owner == NULL) {
		return -1;
	} else {
		return m_owner->getLineIndex();
	}
}



//////////////////////////////
//
// HumAddress::getLineNumber --  Similar to getLineIndex() but adds one.
//

int HumAddress::getLineNumber(void) const {
	return getLineIndex() + 1;
}



//////////////////////////////
//
// HumAddress::getFieldIndex -- Returns the field index on the line of the
//     token associated with the address.
//

int HumAddress::getFieldIndex(void) const {
	return m_fieldindex;
}



//////////////////////////////
//
// HumAddress::getDataType -- Return the exclusive interpretation string of the
//    token associated with the address.
//

const HumdrumToken& HumAddress::getDataType(void) const {
	static HumdrumToken null("");
	if (m_owner == NULL) {
		return null;
	}
	HumdrumToken* tok = m_owner->getTrackStart(getTrack());
	return *tok;
}



//////////////////////////////
//
// HumAddress::getSpineInfo -- Return the spine information for the token
//     associated with the address.  Examples: "1" the token is in the first
//     (left-most) spine, and there are no active sub-spines for the spine.
//     "(1)a"/"(1)b" are the spine descriptions of the two sub-spines after
//     a split manipulator (*^).  "((1)a)b" is the second sub-spines of the
//     first sub-spine for spine 1.
//
//

const string& HumAddress::getSpineInfo(void) const {
	return m_spining;
}



//////////////////////////////
//
// HumAddress::getTrack -- The track number of the given spine.  This is the
//   first number in the spine info string.  The track number is the same
//   as a spine number.
//

int HumAddress::getTrack(void) const {
	return m_track;
}



//////////////////////////////
//
// HumAddress::getSubtrack -- The subtrack number of the given spine.  This
//   functions in a similar manner to layer numbers in MEI data.  The first
//   sub-spine of a spine is always subtrack 1, regardless of whether or not
//   an exchange manipulator (*x) was used to switch the left-to-right ordering
//   of the spines in the file.  All sub-spines regardless of their splitting
//   origin are given sequential subtrack numbers.  For example if the spine
//   info is "(1)a"/"((1)b)a"/"((1)b)b" -- the spine is split, then the second
//   sub-spine only is split--then the sub-spines are labeled as sub-tracks "1",
//   "2", "3" respectively.  When a track has only one sub-spine (i.e., it has
//   been split), the subtrack value will be "0".
//

int HumAddress::getSubtrack(void) const {
	return m_subtrack;
}



//////////////////////////////
//
// HumAddress::getSubtrackCount -- The number of subtrack spines for a
//   given spine on the owning HumdurmLine.  Returns 0 if spine analysis
//   has not been done, or if the line does not have spines (i.e., reference
//   records, global comments and empty lines).
//

int HumAddress::getSubtrackCount(void) const {
	return m_subtrackcount;
}



//////////////////////////////
//
// HumAddress::getTrackString --  Return the track and subtrack as a string.
//      The returned string will have the track number if the sub-spine value
//      is zero.  The optional separator parameter is used to separate the
//      track number from the subtrack number.
// default value: separator = "."
//

string HumAddress::getTrackString(string separator) const {
	string output;
	int thetrack    = getTrack();
	int thesubtrack = getSubtrack();
	output += to_string(thetrack);
	if (thesubtrack > 0) {
		output += separator + to_string(thesubtrack);
	}
	return output;
}



//////////////////////////////
//
// HumAddress::setOwner -- Stores a pointer to the HumdrumLine on which
//   the token associated with this address belongs.  When not owned by
//   a HumdrumLine, the parameter's value should be NULL.
//

void HumAddress::setOwner(HumdrumLine* aLine) {
	m_owner = aLine;
}



//////////////////////////////
//
// HumAddress::getLine -- return the HumdrumLine which owns the token
//    associated with this address.  Returns NULL if it does not belong
//    to a HumdrumLine object.
//

HumdrumLine* HumAddress::getLine(void) const {
	return m_owner;
}



//////////////////////////////
//
// HumAddress::hasOwner -- Returns true if a HumdrumLine owns the token
//    associated with the address.
//

bool HumAddress::hasOwner(void) const {
	return m_owner == NULL ? 0 : 1;
}



//////////////////////////////
//
// HumAddress::setFieldIndex -- Set the field index of associated token
//   in the HumdrumLine owner.  If the token is now owned by a HumdrumLine,
//   then the input parameter should be -1.
//

void HumAddress::setFieldIndex(int index) {
	m_fieldindex = index;
}



//////////////////////////////
//
// HumAddress::setSpineInfo -- Set the spine description of the associated
//     token.  For example "2" for the second spine (from the left), or
//     "((2)a)b" for a sub-spine created as the left sub-spine of the main
//     spine and then as the right sub-spine of that sub-spine.  This function
//     is used by the HumdrumFileStructure class.
//

void HumAddress::setSpineInfo(const string& spineinfo) {
	m_spining = spineinfo;
}



//////////////////////////////
//
// HumAddress::setTrack -- Set the track number of the associated token.
//   This should always be the first number in the spine information string,
//   or -1 if the spine info is empty.  Tracks are limited to an arbitrary
//   count of 1000 (could be increased in the future if needed).  This function
//   is used by the HumdrumFileStructure class.
//

void HumAddress::setTrack(int aTrack, int aSubtrack) {
	setTrack(aTrack);
	setSubtrack(aSubtrack);
}


void HumAddress::setTrack(int aTrack) {
	if (aTrack < 0) {
		aTrack = -1;
	}
	if (aTrack > 1000) {
		aTrack = 1000;
	}
	m_track = aTrack;
}



//////////////////////////////
//
// HumAddress::setSubtrack -- Set the subtrack of the spine.
//   If the token is the only one active for a spine, the subtrack should
//   be set to zero.  If there are more than one sub-tracks for the spine, this
//   is the one-offset index of the spine (be careful if a sub-spine column
//   is exchanged with another spine other than the one from which it was
//   created.  In this case the subtrack number is not useful to calculate
//   the field index of other sub-tracks for the given track.
//   This function is used by the HumdrumFileStructure class.
//

void HumAddress::setSubtrack(int aSubtrack) {
	if (aSubtrack < 0) {
		aSubtrack = -1;
	}
	if (aSubtrack > 1000) {
		aSubtrack = 1000;
	}
	m_subtrack = aSubtrack;
}



//////////////////////////////
//
// HumAddress::setSubtrackCount --
//

void HumAddress::setSubtrackCount(int count) {
	m_subtrackcount = count;
}



//////////////////////////////
//
// HumGrid::HumGrid -- Constructor.
//

HumGrid::HumGrid(void) {
	// for now, limit to 100 parts:
	m_verseCount.resize(100);
	m_harmonyCount.resize(100);
	fill(m_harmonyCount.begin(), m_harmonyCount.end(), 0);

	// default options
	m_musicxmlbarlines = false;
	m_recip = false;
	m_pickup = false;
}



//////////////////////////////
//
// HumGrid::~HumGrid -- Deconstructor.
//

HumGrid::~HumGrid(void) {
	for (int i=0; i<(int)this->size(); i++) {
		if (this->at(i)) {
			delete this->at(i);
		}
	}
}


//////////////////////////////
//
// HumGrid::enableRecipSpine --
//

void HumGrid::enableRecipSpine(void) {
	m_recip = true;
}


//////////////////////////////
//
// HumGrid::getHarmonyCount --
//

int HumGrid::getHarmonyCount(int partindex) {
	if ((partindex < 0) || (partindex >= (int)m_harmonyCount.size())) {
		return 0;
	}
	return m_harmonyCount.at(partindex);
}



//////////////////////////////
//
// HumGrid::getVerseCount --
//

int HumGrid::getVerseCount(int partindex, int staffindex) {
	if ((partindex < 0) || (partindex >= (int)m_verseCount.size())) {
		return 0;
	}
	int staffnumber = staffindex + 1;
	if ((staffnumber < 1) ||
			(staffnumber >= (int)m_verseCount.at(partindex).size())) {
		return 0;
	}
	int value = m_verseCount.at(partindex).at(staffnumber);
	return value;
}



//////////////////////////////
//
// HumGrid::setHarmonyCount -- part size hardwired to 100 for now.
//

void HumGrid::setHarmonyCount(int partindex, int count) {
	if ((partindex < 0) || (partindex > (int)m_harmonyCount.size())) {
		return;
	}
	m_harmonyCount[partindex] = count;
}



//////////////////////////////
//
// HumGrid::setVerseCount --
//

void HumGrid::setVerseCount(int partindex, int staffindex, int count) {
	if ((partindex < 0) || (partindex > (int)m_verseCount.size())) {
		return;
	}
	int staffnumber = staffindex + 1;
	if (staffnumber < 0) {
		return;
	}
	if (staffnumber < (int)m_verseCount.at(partindex).size()) {
		m_verseCount.at(partindex).at(staffnumber) = count;
	} else {
		int oldsize = (int)m_verseCount.at(partindex).size();
		int newsize = staffnumber + 1;
		m_verseCount.at(partindex).resize(newsize);
		for (int i=oldsize; i<newsize; i++) {
			m_verseCount.at(partindex).at(i) = 0;
		}
		m_verseCount.at(partindex).at(staffnumber) = count;
	}
}



//////////////////////////////
//
// HumGrid::transferTokens --
//

bool HumGrid::transferTokens(HumdrumFile& outfile) {
	bool status = buildSingleList();
	if (!status) {
		return false;
	}
	calculateGridDurations();
	addNullTokens();
	addMeasureLines();
	buildSingleList();
	addLastMeasure();
	if (manipulatorCheck()) {
		cleanupManipulators();
	}

	insertStaffIndications(outfile);
	insertPartIndications(outfile);
	insertExclusiveInterpretationLine(outfile);
	bool addstartbar = (!hasPickup()) && (!m_musicxmlbarlines);
	for (int m=0; m<(int)this->size(); m++) {
		if (addstartbar && m == 0) {
			status &= at(m)->transferTokens(outfile, m_recip, addstartbar);
		} else {
			status &= at(m)->transferTokens(outfile, m_recip, false);
		}
		if (!status) {
			break;
		}
	}
	insertDataTerminationLine(outfile);
	return true;
}



//////////////////////////////
//
// HumGrid::cleanupManipulators --
//

void HumGrid::cleanupManipulators(void) {
	int m;
	vector<GridSlice*> newslices;
	for (m=0; m<(int)this->size(); m++) {
		for (auto it = this->at(m)->begin(); it != this->at(m)->end(); it++) {
			if ((*it)->getType() != SliceType::Manipulators) {
				continue;
			}
			// check to see if manipulator needs to be split into
			// multiple lines.
			newslices.resize(0);
			cleanManipulator(newslices, *it);
			if (newslices.size()) {
				for (int j=0; j<(int)newslices.size(); j++) {
					this->at(m)->insert(it, newslices.at(j));
				}
			}
		}
	}
}



//////////////////////////////
//
// HumGrid::cleanManipulator --
//

void HumGrid::cleanManipulator(vector<GridSlice*>& newslices, GridSlice* curr) {
	newslices.resize(0);
	GridSlice* output;

	// deal with *^ manipulators:
	while ((output = checkManipulatorExpand(curr))) {
		newslices.push_back(output);
	}

	// deal with *v manipulators:
	while ((output = checkManipulatorContract(curr))) {
		newslices.push_back(output);
	}
}



//////////////////////////////
//
// HumGrid::checkManipulatorExpand -- Check for cases where a spine expands
//    into sub-spines.
//

GridSlice* HumGrid::checkManipulatorExpand(GridSlice* curr) {
	GridStaff* staff     = NULL;
	GridPart*  part      = NULL;
	GridVoice* voice     = NULL;
	HTp        token     = NULL;
	bool       neednew   = false;

	int p, s, v;
	int partcount = (int)curr->size();
	int staffcount;

	for (p=0; p<partcount; p++) {
		part = curr->at(p);
		staffcount = (int)part->size();
		for (s=0; s<staffcount; s++) {
			staff = part->at(s);
			for (v=0; v<(int)staff->size(); v++) {
				voice = staff->at(v);
				token = voice->getToken();
				if (token->compare(0, 2, "*^") == 0) {
					if ((token->size() > 2) && isdigit((*token)[2])) {
						neednew = true;
						break;
					}
				}
			}
			if (neednew) {
				break;
			}
		}
		if (neednew) {
			break;
		}
	}

	if (neednew == false) {
		return NULL;
	}

	// need to split *^#'s into separate *^

	GridSlice* newmanip = new GridSlice(curr->getMeasure(), curr->getTimestamp(),
	curr->getType(), curr);

	for (p=0; p<partcount; p++) {
		part = curr->at(p);
		staffcount = (int)part->size();
		for (s=0; s<staffcount; s++) {
			staff = part->at(s);
			adjustExpansionsInStaff(newmanip, curr, p, s);
		}
	}
	return newmanip;
}



//////////////////////////////
//
// HumGrid::adjustExpansionsInStaff -- duplicate null
//   manipulators, and expand large-expansions, such as *^3 into
//   *^ and *^ on the next line, or *^4 into *^ and *^3 on the
//   next line.  The "newmanip" will be placed before curr, so
//

void HumGrid::adjustExpansionsInStaff(GridSlice* newmanip, GridSlice* curr, int p, int s) {
	HTp token = NULL;
	GridVoice* newvoice  = NULL;
	GridVoice* curvoice  = NULL;
	GridStaff* newstaff  = NULL;
	GridStaff* curstaff  = NULL;

	curstaff = curr->at(p)->at(s);
	newstaff = newmanip->at(p)->at(s);

	int originalsize = (int)curstaff->size();
	int cv = 0;

	for (int v=0; v<originalsize; v++) {
		curvoice = curstaff->at(cv);
		token = curvoice->getToken();

		if (token->compare(0, 2, "*^") == 0) {
			if ((token->size() > 2) && isdigit((*token)[2])) {
				// transfer *^ to newmanip and replace with * and *^(n-1) in curr
				// Convert *^3 to *^ and add ^* to next line, for example
				// Convert *^4 to *^ and add ^*3 to next line, for example
				int count = 0;
				if (!sscanf(token->c_str(), "*^%d", &count)) {
					cerr << "Error finding expansion number" << endl;
				}
				newstaff->push_back(curvoice);
				curvoice->getToken()->setText("*^");
				newvoice = new GridVoice("*", 0);
				curstaff->at(cv) = newvoice;
				if (count <= 3) {
					newvoice = new GridVoice("*^", 0);
				} else {
					newvoice = new GridVoice("*^" + to_string(count-1), 0);
				}
				curstaff->insert(curstaff->begin()+cv+1, newvoice);
				cv++;
				continue;
			} else {
				// transfer *^ to newmanip and replace with two * in curr
				newstaff->push_back(curvoice);
				newvoice = new GridVoice("*", 0);
				curstaff->at(cv) = newvoice;
				newvoice = new GridVoice("*", 0);
				curstaff->insert(curstaff->begin()+cv, newvoice);
				cv++;
				continue;
			}
		} else {
			// insert * in newmanip
			newvoice = new GridVoice("*", 0);
			newstaff->push_back(newvoice);
			cv++;
			continue;
		}

	}

// ggg
}



//////////////////////////////
//
// HumGrid::checkManipulatorContract -- Will only check for adjacent
//    *v records across adjacent staves, which should be good enough.
//    Will not check within a staff, but this should not occur within
//    MusicXML input data due to the way it is being processed.
//    The return value is a newly created GridSlice pointer which contains
//    a new manipulator to add to the file (and the current manipultor
//    slice will also be modified if the return value is not NULL).
//

GridSlice* HumGrid::checkManipulatorContract(GridSlice* curr) {
	GridVoice* lastvoice = NULL;
	GridVoice* voice     = NULL;
	GridStaff* staff     = NULL;
	GridPart*  part      = NULL;
	bool       neednew   = false;

	int p, s;
	int partcount = (int)curr->size();
	int staffcount;
	for (p=0; p<partcount; p++) {
		part  = curr->at(p);
		staffcount = (int)part->size();
		for (s=0; s<staffcount; s++) {
			staff = part->at(s);
			voice = staff->front();
			if ((p == 0) && (s == 0)) {
				lastvoice = staff->back();
				continue;
			}
			if (lastvoice != NULL) {
           	if ((*voice->getToken() == "*v") &&
						(*lastvoice->getToken() == "*v")) {
					neednew = true;
					break;
				}
			}
			lastvoice = staff->back();
		}
		if (neednew) {
			break;
		}
	}

	if (neednew == false) {
		return NULL;
	}

	// need to split *v's from different adjacent staves onto separate lines.

	GridSlice* newmanip = new GridSlice(curr->getMeasure(), curr->getTimestamp(),
		curr->getType(), curr);

	lastvoice = NULL;
	GridStaff* laststaff    = NULL;
	GridStaff* newstaff     = NULL;
	GridStaff* newlaststaff = NULL;
	bool foundnew = false;
	partcount = (int)curr->size();
	int lastp = 0;
	int lasts = 0;

	for (p=0; p<partcount; p++) {
		part  = curr->at(p);
		staffcount = (int)part->size();
		for (s=0; s<staffcount; s++) {
			staff = part->at(s);
			voice = staff->front();
			if (lastvoice != NULL) {
           	if ((*voice->getToken() == "*v") &&
						(*lastvoice->getToken() == "*v")) {
               // splitting the slices at this staff boundary
					newstaff     = newmanip->at(p)->at(s);
					newlaststaff = newmanip->at(lastp)->at(lasts);

					transferMerges(staff, laststaff, newstaff, newlaststaff);
					foundnew = true;
					break;
				}
			}
			laststaff = staff;
			lastvoice = staff->back();
			lastp = p;
			lasts = s;
		}
		if (foundnew) {
			break;
		}
	}

	return newmanip;
}



//////////////////////////////
//
// HumGrid::transferMerges -- Move *v spines from one staff to last staff,
//   and re-adjust staff "*v" tokens to a single "*" token.
// Example:
//                 laststaff      staff
// old:            *v   *v        *v   *v
// converts to:
// new:            *v   *v        *    *
// old:            *              *v   *v
//
//

void HumGrid::transferMerges(GridStaff* oldstaff, GridStaff* oldlaststaff,
		GridStaff* newstaff, GridStaff* newlaststaff) {

	if ((oldstaff == NULL) || (oldlaststaff == NULL)) {
		cerr << "Weird error in HumGrid::transferMerges()" << endl;
		return;
	}
	// New staves are presumed to be totally empty.

	GridVoice* gv;

	// First create "*" tokens for newstaff slice where there are
	// "*v" in old staff.  All other tokens should be set to "*".
	int tcount = (int)oldstaff->size();
	int t;
	for (t=0; t<tcount; t++) {
		if (*oldstaff->at(t)->getToken() == "*v") {
			gv = new GridVoice("*", 0);
			newstaff->push_back(gv);
		} else {
			gv = new GridVoice("*", 0);
			newstaff->push_back(gv);
		}
	}

	// Next, all "*v" tokens at end of old previous staff should be
	// transferred to the new previous staff and replaced with
	// a single "*" token.  Non "*v" tokens in the old last staff should
	// be converted to "*" tokens in the new last staff.
	//
	// It may be possible for *v tokens to not be only at the end of
	// the list of oldlaststaff tokens, but does not seem possible.

	tcount = (int)oldlaststaff->size();
	bool addednull = false;
	for (t=0; t<tcount; t++) {
		if (*oldlaststaff->at(t)->getToken() == "*v") {
			newlaststaff->push_back(oldlaststaff->at(t));
			if (addednull == false) {
				gv = new GridVoice("*", 0);
				oldlaststaff->at(t) = gv;
				addednull = true;
			} else {
				oldlaststaff->at(t) = NULL;
			}
		} else {
			gv = new GridVoice("*", 0);
			newlaststaff->push_back(gv);
		}
	}

	// Go back to the oldlaststaff and chop off all ending NULLs
	// * it should never get to zero (there should be at least one "*" left.
	// In theory intermediate NULLs should be checked for, and if they
	// exist, then something bad will happen.  But it does not seem
	// possible to have intermediate NULLs.
	tcount = (int)oldlaststaff->size();
	for (t=tcount-1; t>=0; t--) {
		if (oldlaststaff->at(t) == NULL) {
			int newsize = (int)oldlaststaff->size() - 1;
			oldlaststaff->resize(newsize);
		} else {
		}
	}
}



//////////////////////////////
//
// HumGrid::manipulatorCheck --
//

bool HumGrid::manipulatorCheck(void) {
	GridSlice* manipulator;
	int m;
	GridSlice* s1;
	GridSlice* s2;
	bool output = false;
	for (m=0; m<(int)this->size(); m++) {
		if (this->at(m)->size() == 0) {
			continue;
		}
		for (auto it = this->at(m)->begin(); it != this->at(m)->end(); it++) {
			s1 = *it;
			auto nextone = it;
			nextone++;
			if (nextone != this->at(m)->end()) {
				s2 = *nextone;
			} else if (m<(int)this->size()-1) {
				s2 = this->at(m+1)->front();
			} else {
				continue;
				// there is no next slice.  Presumably the terminal
				// barlines have already been added, so this will not
				// be a problem.
			}
			manipulator = manipulatorCheck(s1, s2);
			if (manipulator == NULL) {
				continue;
			}
			output = true;
			auto inserter = it;
			inserter++;
			this->at(m)->insert(inserter, manipulator);
			it++; // skip over the new manipulator line (expand it later)
		}
	}
	return output;
}


//
// HumGrid::manipulatorCheck -- Look for differences in voice/layer count
//   for each part/staff pairing between adjacent lines.  If they do not match,
//   then add spine manipulator line to Grid between the two lines.
//

GridSlice* HumGrid::manipulatorCheck(GridSlice* ice1, GridSlice* ice2) {
	int p1count;
	int p2count;
	int s1count;
	int s2count;
	int v1count;
	int v2count;
	int p;
	int s;
	int v;
	bool needmanip = false;

	if (ice1 == NULL) {
		return NULL;
	}
	if (ice2 == NULL) {
		return NULL;
	}
	p1count = (int)ice1->size();
	p2count = (int)ice2->size();
	if (p1count != p2count) {
		cerr << "Warning: Something weird happend here" << endl;
		cerr << "p1count = " << p1count << endl;
		cerr << "p2count = " << p2count << endl;
		cerr << "The above two values should be the same." << endl;
		return NULL;
	}
	for (p=0; p<p1count; p++) {
		s1count = (int)ice1->at(p)->size();
		s2count = (int)ice2->at(p)->size();
		if (s1count != s2count) {
			cerr << "Warning: Something weird happend here with staff" << endl;
			return NULL;
		}
		for (s=0; s<s1count; s++) {
			v1count = (int)ice1->at(p)->at(s)->size();
			// the voice count always must be at least 1.  This case
			// is related to inserting clefs in other parts.
			if (v1count < 1) {
				v1count = 1;
			}
			v2count = (int)ice2->at(p)->at(s)->size();
			if (v2count < 1) {
				v2count = 1;
			}
			if (v1count == v2count) {
				continue;
			}
			needmanip = true;
			break;
		}
		if (needmanip) {
			break;
		}
	}

	if (!needmanip) {
		return NULL;
	}

	// build manipulator line (which will be expanded further if adjacent
	// staves have *v manipulators.

	GridSlice* mslice;
	mslice = new GridSlice(ice1->getMeasure(), ice2->getTimestamp(),
			SliceType::Manipulators);

	int z;
	HTp token;
	GridVoice* gv;
	p1count = (int)ice1->size();
	mslice->resize(p1count);
	for (p=0; p<p1count; p++) {
		mslice->at(p) = new GridPart;
		s1count = (int)ice1->at(p)->size();
		mslice->at(p)->resize(s1count);
		for (s=0; s<s1count; s++) {
			mslice->at(p)->at(s) = new GridStaff;
			v1count = (int)ice1->at(p)->at(s)->size();
			v2count = (int)ice2->at(p)->at(s)->size();
			if (v2count < 1) {
				// empty spines will be filled in with at least one null token.
				v2count = 1;
			}
			if (v1count < 1) {
				// empty spines will be filled in with at least one null token.
				v1count = 1;
			}
			if ((v1count == 0) && (v2count == 1)) {
				// grace note at the start of the measure in another voice
				// no longer can get here due to v1count min being 1.
				token = new HumdrumToken("*G");
				gv = new GridVoice(token, 0);
				mslice->at(p)->at(s)->push_back(gv);
			} else if (v1count == v2count) {
				for (v=0; v<v1count; v++) {
					token = new HumdrumToken("*");
					gv = new GridVoice(token, 0);
					mslice->at(p)->at(s)->push_back(gv);
				}
			} else if (v1count < v2count) {
				// need to grow
				int grow = v2count - v1count;
				// if (grow == 2 * v1count) {
				if (v2count == 2 * v1count) {
					// all subspines split
					for (z=0; z<v1count; z++) {
						token = new HumdrumToken("*^");
						gv = new GridVoice(token, 0);
						mslice->at(p)->at(s)->push_back(gv);
					}
				} else if ((v1count > 0) && (grow > 2 * v1count)) {
					// too large to split all at the same time, deal with later
					for (z=0; z<v1count-1; z++) {
						token = new HumdrumToken("*^");
						gv = new GridVoice(token, 0);
						mslice->at(p)->at(s)->push_back(gv);
					}
					int extra = v2count - (v1count - 1) * 2;
					token = new HumdrumToken("*^" + to_string(extra));
					gv = new GridVoice(token, 0);
					mslice->at(p)->at(s)->push_back(gv);
				} else {
					// only split spines at end of list
					int doubled = v2count - v1count;
					int notdoubled = v1count - doubled;
					for (z=0; z<notdoubled; z++) {
						token = new HumdrumToken("*");
						gv = new GridVoice(token, 0);
						mslice->at(p)->at(s)->push_back(gv);
					}
					//for (z=0; z<doubled; z++) {
						token = new HumdrumToken("*^" + to_string(doubled+1));
						// token = new HumdrumToken("*^Z");
						gv = new GridVoice(token, 0);
						mslice->at(p)->at(s)->push_back(gv);
					//}
				}
			} else if (v1count > v2count) {
				// need to shrink
				int shrink = v1count - v2count + 1;
				int notshrink = v1count - shrink;
				for (z=0; z<notshrink; z++) {
					token = new HumdrumToken("*");
					gv = new GridVoice(token, 0);
					mslice->at(p)->at(s)->push_back(gv);
				}
				for (z=0; z<shrink; z++) {
					token = new HumdrumToken("*v");
					gv = new GridVoice(token, 0);
					mslice->at(p)->at(s)->push_back(gv);
				}
			}
		}
	}
	return mslice;
}



//////////////////////////////
//
// HumGrid::addMeasureLines --
//

void HumGrid::addMeasureLines(void) {
	HumNum timestamp;
	GridSlice* mslice;
	GridSlice* endslice;
	GridPart* part;
	GridStaff* staff;
	GridVoice* gv;
	string token;
	int staffcount, partcount, vcount, nextvcount, lcount;
	GridMeasure* measure = NULL;
	GridMeasure* nextmeasure = NULL;

	vector<int> barnums;
	if (!m_musicxmlbarlines) {
		getMetricBarNumbers(barnums);
	}

	for (int m=0; m<(int)this->size()-1; m++) {
		measure = this->at(m);
		nextmeasure = this->at(m+1);
		if (nextmeasure->size() == 0) {
			// next measure is empty for some reason so give up
			continue;
		}
		timestamp = nextmeasure->front()->getTimestamp();
		mslice = new GridSlice(measure, timestamp, SliceType::Measures);
		if (measure->size() == 0) {
			continue;
		}
		endslice = measure->back();
		measure->push_back(mslice);
		partcount = (int)nextmeasure->front()->size();
		mslice->resize(partcount);

		for (int p=0; p<partcount; p++) {
			part = new GridPart();
			mslice->at(p) = part;
			staffcount = (int)nextmeasure->front()->at(p)->size();
			mslice->at(p)->resize(staffcount);
			for (int s=0; s<(int)staffcount; s++) {
				staff = new GridStaff;
				mslice->at(p)->at(s) = staff;

				// insert the minimum number of barlines based on the
				// voices in the current and next measure.
				vcount = (int)endslice->at(p)->at(s)->size();
				nextvcount = (int)nextmeasure->front()->at(p)->at(s)->size();
				lcount = vcount;
				if (lcount > nextvcount) {
					lcount = nextvcount;
				}
				if (lcount == 0) {
					lcount = 1;
				}
				for (int v=0; v<lcount; v++) {
					token = createBarToken(m, barnums[m], measure);
					gv = new GridVoice(token, 0);
					mslice->at(p)->at(s)->push_back(gv);
				}
			}
		}
	}
}



//////////////////////////////
//
// HumGrid::createBarToken --
//

string HumGrid::createBarToken(int m, int barnum, GridMeasure* measure) {
	string token;
	string barstyle = getBarStyle(measure);
	string number = "";
	if (barnum > 0) {
		number = to_string(barnum);
	}
	if (m_musicxmlbarlines) {
		// m+1 because of the measure number
		// comes from the previous measure.
		if (barstyle == "=") {
			token = "==";
			token += to_string(m+1);
		} else {
			token = "=";
			token += to_string(m+1);
			token += barstyle;
		}
	} else {
		if (barnum > 0) {
			if (barstyle == "=") {
				token = "==";
				token += number;
			} else {
				token = "=";
				token += number;
				token += barstyle;
			}
		} else {
			if (barstyle == "=") {
				token = "==";
			} else {
				token = "=";
				token += barstyle;
			}
		}
	}
	return token;
}



//////////////////////////////
//
// HumGrid::addMetricBarNumbers --
//

void HumGrid::getMetricBarNumbers(vector<int>& barnums) {
	int mcount = (int)this->size();
	barnums.resize(mcount);

	if (mcount == 0) {
		return;
	}

	vector<HumNum> mdur(mcount);
	vector<HumNum> tsdur(mcount); // time signature duration

	for (int m=0; m<(int)this->size(); m++) {
		mdur[m]   = this->at(m)->getDuration();
		tsdur[m] = this->at(m)->getTimeSigDur();
		if (tsdur[m] <= 0) {
			tsdur[m] = mdur[m];
		}
	}

	int counter = 1;
	if (mdur[0] == tsdur[0]) {
		m_pickup = false;
		counter++;
		// add the initial barline later when creating HumdrumFile.
	} else {
		m_pickup = true;
	}

	for (int m=0; m<(int)this->size(); m++) {
		if ((m < mcount-1) && (tsdur[m] == tsdur[m+1])) {
			if (mdur[m] + mdur[m+1] == tsdur[m]) {
				barnums[m] = -1;
			} else {
				barnums[m] = counter++;
			}
		} else {
			barnums[m] = counter++;
		}
	}
}



//////////////////////////////
//
// HumGrid::getBarStyle --
//

string HumGrid::getBarStyle(GridMeasure* measure) {
	string output = "";
	if (measure->isDouble()) {
		output = "||";
	} else if (measure->isFinal()) {
		output = "=";
	} else if (measure->isRepeatBoth()) {
		output = ":|!|:";
	} else if (measure->isRepeatBackward()) {
		output = ":|!";
	} else if (measure->isRepeatForward()) {
		output = "!|:";
	}
	return output;
}



//////////////////////////////
//
// HumGrid::addLastMeasure --
//

void HumGrid::addLastMeasure(void) {
   // add the last measure, which will be only one voice
	// for each part/staff.
	GridSlice* model = this->back()->back();

	// probably not the correct timestamp, but probably not important
	// to get correct:
	HumNum timestamp = model->getTimestamp();

	if (this->empty()) {
		return;
	}
	GridMeasure* measure = this->back();

	string barstyle = getBarStyle(measure);

	GridSlice* mslice = new GridSlice(model->getMeasure(), timestamp,
			SliceType::Measures);
	this->back()->push_back(mslice);
	mslice->setTimestamp(timestamp);
	int partcount = (int)model->size();
	mslice->resize(partcount);
	for (int p=0; p<partcount; p++) {
		GridPart* part = new GridPart();
		mslice->at(p) = part;
		int staffcount = (int)model->at(p)->size();
		mslice->at(p)->resize(staffcount);
		for (int s=0; s<staffcount; s++) {
			GridStaff* staff = new GridStaff;
			mslice->at(p)->at(s) = staff;
			HTp token = new HumdrumToken("=" + barstyle);
			GridVoice* gv = new GridVoice(token, 0);
			mslice->at(p)->at(s)->push_back(gv);
		}
	}
}



//////////////////////////////
//
// HumGrid::buildSingleList --
//

bool HumGrid::buildSingleList(void) {
	m_allslices.resize(0);

	int gridcount = 0;
	for (auto it : (vector<GridMeasure*>)*this) {
		gridcount += (int)it->size();
	}
	m_allslices.reserve(gridcount + 100);
	for (int m=0; m<(int)this->size(); m++) {
		for (auto it : (list<GridSlice*>)*this->at(m)) {
			m_allslices.push_back(it);
		}
	}

	HumNum ts1;
	HumNum ts2;
	HumNum dur;
	for (int i=0; i<(int)m_allslices.size() - 1; i++) {
		ts1 = m_allslices[i]->getTimestamp();
		ts2 = m_allslices[i+1]->getTimestamp();
		dur = (ts2 - ts1); // whole-note units
		m_allslices[i]->setDuration(dur);
	}

	return !m_allslices.empty();
}



//////////////////////////////
//
// HumGrid::addNullTokensForGraceNotes -- Avoid grace notes at
//     starts of measures from contracting the subspine count.
//

void HumGrid::addNullTokensForGraceNotes(void) {
	// add null tokens for grace notes in other voices
	GridSlice *lastnote = NULL;
	GridSlice *nextnote = NULL;
	for (int i=0; i<(int)m_allslices.size(); i++) {
		if (!m_allslices[i]->isGraceSlice()) {
			continue;
		}
		// cerr << "PROCESSING " << m_allslices[i] << endl;
		lastnote = NULL;
		nextnote = NULL;

		for (int j=i+1; j<(int)m_allslices.size(); j++) {
			if (m_allslices[j]->isNoteSlice()) {
				nextnote = m_allslices[j];
				break;
			}
		}
		if (nextnote == NULL) {
			continue;
		}

		for (int j=i-1; j>=0; j--) {
			if (m_allslices[j]->isNoteSlice()) {
				lastnote = m_allslices[j];
				break;
			}
		}
		if (lastnote == NULL) {
			continue;
		}

		FillInNullTokensForGraceNotes(m_allslices[i], lastnote, nextnote);
	}

}



//////////////////////////////
//
// HumGrid::FillInNullTokensForGraceNotes --
//

void HumGrid::FillInNullTokensForGraceNotes(GridSlice* graceslice, GridSlice* lastnote,
		GridSlice* nextnote) {

	if (graceslice == NULL) {
		return;
	}
	if (lastnote == NULL) {
		return;
	}
	if (nextnote == NULL) {
		return;
	}

	// cerr << "CHECKING GRACE SLICE: " << endl;
	// cerr << "\tgrace\t" << graceslice << endl;
	// cerr << "\tlast\t" << lastnote << endl;
	// cerr << "\tnext\t" << nextnote << endl;

	int partcount = (int)graceslice->size();
	int staffcount;
	int vgcount;
	int v1count;
	int v2count;

	for (int p=0; p<partcount; p++) {
		staffcount = (int)lastnote->at(p)->size();
		for (int s=0; s<staffcount; s++) {
			v1count = (int)lastnote->at(p)->at(s)->size();
			v2count = (int)nextnote->at(p)->at(s)->size();
			vgcount = (int)graceslice->at(p)->at(s)->size();
			// if (vgcount < 1) {
			// 	vgcount = 1;
			// }
			if (v1count < 1) {
				v1count = 1;
			}
			if (v2count < 1) {
				v2count = 1;
			}
			// cerr << "p=" << p << "\ts=" << s << "\tv1count = " << v1count;
			// cerr << "\tv2count = " << v2count;
			// cerr << "\tvgcount = " << vgcount << endl;
			if (v1count != v2count) {
				// Note slices are expanding or contracting so do
				// not try to adjust grace slice between them.
				continue;
			}
			if (vgcount == v1count) {
				// Grace note slice does not need to be adjusted.
			}
			int diff = v1count - vgcount;
			// fill in a null for each empty slot in voice
			for (int i=0; i<diff; i++) {
				GridVoice* gv = new GridVoice(".", 0);
				graceslice->at(p)->at(s)->push_back(gv);
			}
		}
	}
}



//////////////////////////////
//
// HumGrid::addNullTokens --
//

void HumGrid::addNullTokens(void) {
	int i; // slice index
	int p; // part index
	int s; // staff index
	int v; // voice index

	if (0) {
		cerr << "SLICE TIMESTAMPS: " << endl;
		for (int x=0; x<(int)m_allslices.size(); x++) {
			cerr << "\tTIMESTAMP " << x << "= "
			     << m_allslices[x]->getTimestamp()
			     << "\tDUR=" << m_allslices[x]->getDuration()
			     << "\t"
			     << m_allslices[x]
			     << endl;
		}
	}


	for (i=0; i<(int)m_allslices.size(); i++) {
		GridSlice& slice = *m_allslices.at(i);
		if (!slice.isNoteSlice()) {
			// probably need to deal with grace note slices here
			continue;
		}
      for (p=0; p<(int)slice.size(); p++) {
			GridPart& part = *slice.at(p);
      	for (s=0; s<(int)part.size(); s++) {
				GridStaff& staff = *part.at(s);
      		for (v=0; v<(int)staff.size(); v++) {
					if (!staff.at(v)) {
						// in theory should not happen
						continue;
					}
					GridVoice& gv = *staff.at(v);
					if (gv.isNull()) {
						continue;
					}
					// found a note/rest which should have a non-zero
					// duration that needs to be extended to the next
					// duration in the
					extendDurationToken(i, p, s, v);
				}
			}
		}
	}

	addNullTokensForGraceNotes();
}



//////////////////////////////
//
// HumGrid::extendDurationToken --
//

void HumGrid::extendDurationToken(int slicei, int parti, int staffi,
		int voicei) {
	if ((slicei < 0) || (slicei >= ((int)m_allslices.size()) - 1)) {
		// nothing after this line, so can extend further.
		return;
	}

	GridVoice* gv = m_allslices.at(slicei)->at(parti)->at(staffi)->at(voicei);
 	HTp token = gv->getToken();
	if (!token) {
		cerr << "STRANGE: token should not be null" << endl;
		return;
	}
	if (*token == ".") {
		// null data token so ignore;
		// change this later to add a duration for the null token below.
		return;
	}
	
	HumNum tokendur = Convert::recipToDuration((string)*token);
	HumNum currts   = m_allslices.at(slicei)->getTimestamp();
	HumNum nextts   = m_allslices.at(slicei+1)->getTimestamp();
	HumNum slicedur = nextts - currts;
	HumNum timeleft = tokendur - slicedur;

	if (0) {
		cerr << "===================" << endl;
		cerr << "EXTENDING TOKEN    " << token      << endl;
		cerr << "\tTOKEN DUR:       " << tokendur   << endl;
		cerr << "\tTOKEN START:     " << currts     << endl;
		cerr << "\tSLICE DUR:       " << slicedur   << endl;
		cerr << "\tNEXT SLICE START:" << nextts     << endl;
		cerr << "\tTIME LEFT:       " << timeleft   << endl;
		cerr << "\t-----------------" << endl;
	}

	if (timeleft != 0) {
		// fill in null tokens for the required duration.
		if (timeleft < 0) {
			cerr << "ERROR: Negative duration" << endl;
			return;
		}

		SliceType type;
		GridStaff* gs;
		int s = slicei+1;

		while ((s < (int)m_allslices.size()) && (timeleft > 0)) {
			currts = nextts;
			if (s < (int)m_allslices.size() - 1) {
				nextts = m_allslices.at(s+1)->getTimestamp();
			} else {
				nextts = currts + m_allslices.at(s)->getDuration();
			}
			slicedur = nextts - currts;
			type = m_allslices[s]->getType();

			gs = m_allslices.at(s)->at(parti)->at(staffi);
			if (gs == NULL) {
				cerr << "Strange error2 in extendDurationToken()" << endl;
				return;
			}
			gs->setNullTokenLayer(voicei, type, slicedur);
			
			if (m_allslices.at(s)->isDataSlice()) {
				gs->setNullTokenLayer(voicei, type, slicedur);
				timeleft = timeleft - slicedur;
			} else {
				// store a null token for the non-data slice, but probably skip
				// if there is a token already there (such as a clef-change).
				gs->setNullTokenLayer(voicei, type, slicedur);
			}
			s++;
			if (s == (int)m_allslices.size() - 1) {
				m_allslices[s]->setDuration(timeleft);
			}
		}
	}
	// walk through zero-dur items and fill them in, but stop at
	// a token (likely a grace note which should not be erased).
// ggg

}



//////////////////////////////
//
// HumGrid::getGridVoice -- Check to see if GridVoice exists, returns
//    NULL otherwise. Requires HumGrid::buildSingleList() being run first.
//

GridVoice* HumGrid::getGridVoice(int slicei, int parti, int staffi,
		int voicei) {
	if (slicei >= (int)m_allslices.size()) {
		cerr << "Strange error 1a" << endl;
		return NULL;
	}
	GridSlice* gsl = m_allslices.at(slicei);
	if (gsl == NULL) {
		cerr << "Strange error 1b" << endl;
		return NULL;
	}

	if (parti >= (int)gsl->size()) {
		cerr << "Strange error 2a" << endl;
		return NULL;
	}
	GridPart* gp = gsl->at(parti);
	if (gp == NULL) {
		cerr << "Strange error 2" << endl;
		return NULL;
	}

	if (staffi >= (int)gp->size()) {
		cerr << "Strange error 3a" << endl;
		return NULL;
	}
	GridStaff* gst = gp->at(staffi);
	if (gst == NULL) {
		cerr << "Strange error 3b" << endl;
		return NULL;
	}

	if (voicei >= (int)gst->size()) {
		cerr << "Strange error 4a" << endl;
		return NULL;
	}
	GridVoice* gv = gst->at(voicei);
	if (gv == NULL) {
		cerr << "Strange error 4b" << endl;
		return NULL;
	}
	return gv;
}



//////////////////////////////
//
// HumGrid::calculateGridDurations --
//

void HumGrid::calculateGridDurations(void) {

	// the last line has to be calculated from the shortest or
   // longest duration on the line.  Acutally all durations
	// starting on this line must be the same, so just search for
	// the first duration.

	auto last = m_allslices.back();

	// set to zero in case not a duration type of line:
	last->setDuration(0);

	bool finished = false;
	if (last->isNoteSlice()) {
		for (auto part : *last) {
			for (auto staff : *part) {
				for (auto voice : *staff) {
					if (!voice) {
						continue;
					}
					if (voice->getDuration() > 0) {
						last->setDuration(voice->getDuration());
						finished = true;
						break;
					}
				}
				if (finished) {
					break;
				}
			}
			if (finished) {
				break;
			}
		}
	}
}



//////////////////////////////
//
// HumGrid::insertExclusiveInterpretationLine -- Currently presumes
//    that the first entry contains spines.  And the first measure
//    in the HumGrid object must contain a slice.
//

void HumGrid::insertExclusiveInterpretationLine(HumdrumFile& outfile) {
	if (this->size() == 0) {
		return;
	}
	if (this->at(0)->empty()) {
		return;
	}

	HumdrumLine* line = new HumdrumLine;
	HTp token;

	if (m_recip) {
		token = new HumdrumToken("**recip");
		line->appendToken(token);
	}

	GridSlice& slice = *this->at(0)->front();
	int p; // part index
	int s; // staff index
	for (p=(int)slice.size()-1; p>=0; p--) {
		GridPart& part = *slice[p];
		for (s=(int)part.size()-1; s>=0; s--) {
			token = new HumdrumToken("**kern");
			line->appendToken(token);
			insertExInterpSides(line, p, s); // insert staff sides
		}
		insertExInterpSides(line, p, -1);   // insert part sides
	}
	outfile.insertLine(0, line);
}



//////////////////////////////
//
// HumGrid::insertExInterpSides --
//

void HumGrid::insertExInterpSides(HumdrumLine* line, int part, int staff) {
	if (staff >= 0) {
		int versecount = getVerseCount(part, staff); // verses related to staff
		for (int i=0; i<versecount; i++) {
			HTp token = new HumdrumToken("**text");
			line->appendToken(token);
		}
	}

	if (staff < 0) {
		int harmonyCount = getHarmonyCount(part);
		for (int i=0; i<harmonyCount; i++) {
			HTp token = new HumdrumToken("**mxhm");
			line->appendToken(token);
		}
	}
}



//////////////////////////////
//
// HumGrid::insertPartIndications -- Currently presumes
//    that the first entry contains spines.  And the first measure
//    in the HumGrid object must contain a slice.  This is the
//    MusicXML Part number. (Some parts will contain more than one
//    staff).
//

void HumGrid::insertPartIndications(HumdrumFile& outfile) {
	if (this->size() == 0) {
		return;
	}
	if (this->at(0)->empty()) {
		return;
	}
	HumdrumLine* line = new HumdrumLine;
	HTp token;

	if (m_recip) {
		token = new HumdrumToken("*");
		line->appendToken(token);
	}

	string text;
	GridSlice& slice = *this->at(0)->front();
	int p; // part index
	int s; // staff index
	for (p=(int)slice.size()-1; p>=0; p--) {
		GridPart& part = *slice[p];
		for (s=(int)part.size()-1; s>=0; s--) {
			text = "*part" + to_string(p+1);
			token = new HumdrumToken(text);
			line->appendToken(token);
			insertSidePartInfo(line, p, s);
		}
		insertSidePartInfo(line, p, -1);   // insert part sides
	}
	outfile.insertLine(0, line);
}



//////////////////////////////
//
// HumGrid::insertSidePartInfo --
//

void HumGrid::insertSidePartInfo(HumdrumLine* line, int part, int staff) {
	HTp token;
	string text;

	if (staff < 0) {
		int harmcount = getHarmonyCount(part);
		for (int i=0; i<harmcount; i++) {
			text = "*part" + to_string(part+1);
			token = new HumdrumToken(text);
			line->appendToken(token);
		}
	} else {
		int versecount = getVerseCount(part, staff);
		for (int i=0; i<versecount; i++) {
			text = "*part" + to_string(part+1);
			token = new HumdrumToken(text);
			line->appendToken(token);
		}
	}
}



//////////////////////////////
//
// HumGrid::insertStaffIndications -- Currently presumes
//    that the first entry contains spines.  And the first measure
//    in the HumGrid object must contain a slice.  This is the
//    MusicXML Part number. (Some parts will contain more than one
//    staff).
//

void HumGrid::insertStaffIndications(HumdrumFile& outfile) {
	if (this->size() == 0) {
		return;
	}
	if (this->at(0)->empty()) {
		return;
	}

	HumdrumLine* line = new HumdrumLine;
	HTp token;

	if (m_recip) {
		token = new HumdrumToken("*");
		line->appendToken(token);
	}

	string text;
	GridSlice& slice = *this->at(0)->front();
	int p; // part index
	int s; // staff index

	int staffcount = 0;
	for (p=0; p<(int)slice.size(); p++) {
		GridPart& part = *slice[p];
		staffcount += (int)part.size();
	}

	for (p=(int)slice.size()-1; p>=0; p--) {
		GridPart& part = *slice[p];
		for (s=(int)part.size()-1; s>=0; s--) {
			text = "*staff" + to_string(staffcount--);
			token = new HumdrumToken(text);
			line->appendToken(token);
			insertSideStaffInfo(line, p, s, staffcount+1);
		}
		insertSideStaffInfo(line, p, -1, -1);  // insert part sides
	}
	outfile.insertLine(0, line);
}



//////////////////////////////
//
// HumGrid::insertSideStaffInfo --
//

void HumGrid::insertSideStaffInfo(HumdrumLine* line, int part, int staff,
		int staffnum) {
	HTp token;
	string text;

	// part-specific sides (no staff markers)
	if (staffnum < 0) {
		int harmcount = getHarmonyCount(part);
		for (int i=0; i<harmcount; i++) {
			token = new HumdrumToken("*");
			line->appendToken(token);
		}
		return;
	}

	int versecount = getVerseCount(part, staff);
	for (int i=0; i<versecount; i++) {
		if (staffnum > 0) {
			text = "*staff" + to_string(staffnum);
			token = new HumdrumToken(text);
		} else {
			token = new HumdrumToken("*");
		}
		line->appendToken(token);
	}


}



//////////////////////////////
//
// HumGrid::insertDataTerminationLine -- Currently presumes
//    that the last entry contains spines.  And the first
//    measure in the HumGrid object must contain a slice.
//    Also need to compensate for *v on previous line.
//

void HumGrid::insertDataTerminationLine(HumdrumFile& outfile) {
	if (this->size() == 0) {
		return;
	}
	if (this->at(0)->empty()) {
		return;
	}
	HumdrumLine* line = new HumdrumLine;
	HTp token;

	if (m_recip) {
		token = new HumdrumToken("*-");
		line->appendToken(token);
	}

	GridSlice& slice = *this->at(0)->back();
	int p; // part index
	int s; // staff index
	for (p=(int)slice.size()-1; p>=0; p--) {
		GridPart& part = *slice[p];
		for (s=(int)part.size()-1; s>=0; s--) {
			token = new HumdrumToken("*-");
			line->appendToken(token);
			insertSideTerminals(line, p, s);
		}
		insertSideTerminals(line, p, -1);   // insert part sides
	}
	outfile.appendLine(line);
}



//////////////////////////////
//
// HumGrid::insertSideTerminals --
//

void HumGrid::insertSideTerminals(HumdrumLine* line, int part, int staff) {
	HTp token;

	if (staff < 0) {
		int harmcount = getHarmonyCount(part);
		for (int i=0; i<harmcount; i++) {
			token = new HumdrumToken("*-");
			line->appendToken(token);
		}
	} else {
		int versecount = getVerseCount(part, staff);
		for (int i=0; i<versecount; i++) {
			token = new HumdrumToken("*-");
			line->appendToken(token);
		}
	}
}



//////////////////////////////
//
// HumGrid::removeRedundantClefChanges -- Will also have to consider
//		the meter signature.
//

void HumGrid::removeRedundantClefChanges(void) {
	// curclef is a list of the current staff on the part:staff.
	vector<vector<string> > curclef;

	bool hasduplicate = false;
	GridMeasure* measure;
	GridVoice* voice;
	HTp token;
	for (int m=0; m<(int)this->size(); m++) {
		measure = this->at(m);
		for (auto slice : *measure) {
			if (!slice->isClefSlice()) {
				continue;
			}
			bool allempty = true;
			for (int p=0; p<(int)slice->size(); p++) {
				for (int s=0; s<(int)slice->at(p)->size(); s++) {
					if (slice->at(p)->at(s)->size() < 1) {
						continue;
					}
					voice = slice->at(p)->at(s)->at(0);
					token = voice->getToken();
					if (!token) {
						continue;
					}
					if (string(*token) == "*") {
						continue;
					}
					if (token->find("clef") == string::npos) {
						// something (probably invalid) which is not a clef change
						allempty = false;
						continue;
					}
					if (p >= (int)curclef.size()) {
						curclef.resize(p+1);
					}
					if (s >= (int)curclef[p].size()) {
						// first clef on the staff, so can't be a duplicate
						curclef[p].resize(s+1);
						curclef[p][s] = *token;
						allempty = false;
						continue;
					} else {
						if (curclef[p][s] == (string)*token) {
							// clef is already active, so remove this one
							hasduplicate = true;
							voice->setToken("*");
						} else {
							// new clef change
							curclef[p][s] = *token;
							allempty = false;
						}
					}
				}
			}
			if (!hasduplicate) {
				continue;
			}
			// Check the slice to see if it empty, and delete if so.
			// This algorithm does not consider GridSide content.
			if (allempty) {
				slice->invalidate();
			}
			
		}
	}
}



//////////////////////////////
//
// HumGrid::hasPickup --
//

bool HumGrid::hasPickup(void) {
	return m_pickup;
}




////////////////////////////////
//
// HumParameter::HumParameter -- HumParameter constructor.
//

HumParameter::HumParameter(void) {
	origin = NULL;
}


HumParameter::HumParameter(const string& str) : string(str) {
	origin = NULL;
}



//////////////////////////////
//
// HumHash::HumHash -- HumHash constructor.  The data storage is empty
//    until the first parameter in the Hash is set.
//

HumHash::HumHash(void) {
	parameters = NULL;
}



//////////////////////////////
//
// HumHash::~HumHash -- The HumHash deconstructor, which removed any
//    allocated storage before the object dies.
//

HumHash::~HumHash() {
	if (parameters != NULL) {
		delete parameters;
		parameters = NULL;
	}
}



//////////////////////////////
//
// HumHash::getValue -- Returns the value specified by the given key.
//    If there is no colon in the key then return the value for the key
//    in the default namespaces (NS1="" and NS2="").  If there is one colon,
//    then the two pieces of the string as NS2 and the key, with NS1="".
//    If there are two colons, then that specified the complete namespaces/key
//    address of the value.  The namespaces and key can be specified as
//    separate parameters in a similar manner to the single-string version.
//    But in these cases colon concatenation of the namespaces and/or key
//    are not allowed.
//

string HumHash::getValue(const string& key) const {
	if (parameters == NULL) {
		return "";
	} else {
		vector<string> keys = getKeyList(key);
		if (keys.size() == 1) {
			return getValue("", "", keys[0]);
		} else if (keys.size() == 2) {
			return getValue("", keys[0], keys[1]);
		} else {
			return getValue(keys[0], keys[1], keys[2]);
		}
	}
}


string HumHash::getValue(const string& ns2, const string& key) const {
	if (parameters == NULL) {
		return "";
	} else {
		return getValue("", ns2, key);
	}
}


string HumHash::getValue(const string& ns1, const string& ns2,
		const string& key) const {
	if (parameters == NULL) {
		return "";
	}
	MapNNKV& p = *parameters;
	auto it1 = p.find(ns1);
	if (it1 == p.end()) {
		return "";
	}
	auto it2 = it1->second.find(ns2);
	if (it2 == it1->second.end()) {
		return "";
	}
	auto it3 = it2->second.find(key);
	if (it3 == it2->second.end()) {
		return "";
	}
	return it3->second;
}



//////////////////////////////
//
// HumHash::getValueHTp -- Return an address of a HumdrumToken.
//   Presumes 64-bit pointers (or at least not 128-bit pointers).
//

HTp HumHash::getValueHTp(const string& key) const {
	if (parameters == NULL) {
		return NULL;
	}
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		return getValueHTp("", "", keys[2]);
	} else if (keys.size() == 2) {
		return getValueHTp(keys[0], keys[1]);
	} else {
		return getValueHTp(keys[0], keys[1], keys[2]);
	}
}


HTp HumHash::getValueHTp(const string& ns2, const string& key) const {
	if (parameters == NULL) {
		return NULL;
	}
	return getValueHTp("", ns2, key);
}


HTp HumHash::getValueHTp(const string& ns1, const string& ns2,
		const string& key) const {
	if (parameters == NULL) {
		return NULL;
	}
	string value = getValue(ns1, ns2, key);
	if (value.find("HT_") != 0) {
		return NULL;
	} else {
		HTp pointer = NULL;
		try {
			pointer = (HTp)(stoll(value.substr(3)));
		} catch (invalid_argument& e) {
			pointer = NULL;
		}
		return pointer;
	}
}



//////////////////////////////
//
// HumHash::getValueInt -- Return the value as an integer.  The value must
//   start with a number and have no text before it; otherwise the
//   returned value will be "0".  The HumHash class is aware of fractional
//   values, so the integer form of the fraction will be returned.  For
//   example if the value is "12/7", then the return value will be "1"
//   since the integer part of 12/7 is 1 with a remainder of 5/7ths
//   which will be chopped off.
//

int HumHash::getValueInt(const string& key) const {
	if (parameters == NULL) {
		return 0;
	}
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		return getValueInt("", "", keys[2]);
	} else if (keys.size() == 2) {
		return getValueInt(keys[0], keys[1]);
	} else {
		return getValueInt(keys[0], keys[1], keys[2]);
	}
}


int HumHash::getValueInt(const string& ns2, const string& key) const {
	if (parameters == NULL) {
		return 0;
	}
	return getValueInt("", ns2, key);
}


int HumHash::getValueInt(const string& ns1, const string& ns2,
		const string& key) const {
	if (parameters == NULL) {
		return 0;
	}
	string value = getValue(ns1, ns2, key);
	if (value.find("/") != string::npos) {
		HumNum nvalue(value);
		return  nvalue.getInteger();
	} else {
		int intvalue;
		try {
			// problem with emscripten with stoi:
			// intvalue = stoi(value);
			stringstream converter(value);
			if (!(converter >> intvalue)) {
				intvalue = 0;
			}
		} catch (invalid_argument& e) {
			intvalue = 0;
		}
		return intvalue;
	}
}



//////////////////////////////
//
// HumHash::getValueFraction -- Return the value as a HumNum fraction.
//    If the string represents an integer, it will be preserved in the
//    HumNum return value.  For floating-point values, the fractional
//    part will be ignored.  For example "1.52" will be returned as "1".
//

HumNum HumHash::getValueFraction(const string& key) const {
	if (parameters == NULL) {
		return 0;
	}
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		return getValueFraction("", "", keys[0]);
	} else if (keys.size() == 2) {
		return getValueFraction(keys[0], keys[1]);
	} else {
		return getValueFraction(keys[0], keys[1], keys[2]);
	}
}


HumNum HumHash::getValueFraction(const string& ns2, const string& key) const {
	if (parameters == NULL) {
		return 0;
	}
	return getValueFraction("", ns2, key);
}


HumNum HumHash::getValueFraction(const string& ns1, const string& ns2,
		const string& key) const {
	if (!isDefined(ns1, ns2, key)) {
		return 0;
	}
	string value = getValue(ns1, ns2, key);
	HumNum fractionvalue(value);
	return fractionvalue;
}



//////////////////////////////
//
// HumHash::getValueFloat --  Return the floating-point interpretation
//   of the value string.  If the string can represent a HumNum fraction,
//   then convert the HumNum interpretation as a floating point number.
//   For example "1.25" and "5/4" will both return 1.25.  The value
//   cannot contain a slash unless it is part of the first fraction
//   on in the value string (this may be changed when regular expressions
//   are used to implement this function).
//

double HumHash::getValueFloat(const string& key) const {
	if (parameters == NULL) {
		return 0.0;
	}
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		return getValueFloat("", "", keys[2]);
	} else if (keys.size() == 2) {
		return getValueFloat(keys[0], keys[1]);
	} else {
		return getValueFloat(keys[0], keys[1], keys[2]);
	}
}


double HumHash::getValueFloat(const string& ns2, const string& key) const {
	if (parameters == NULL) {
		return 0.0;
	}
	return getValueInt("", ns2, key);
}


double HumHash::getValueFloat(const string& ns1, const string& ns2,
		const string& key) const {
	if (parameters == NULL) {
		return 0.0;
	}
	string value = getValue(ns1, ns2, key);
	if (value.find("/") != string::npos) {
		HumNum nvalue(value);
		return nvalue.getFloat();
	} else {
		double floatvalue;
		try {
			floatvalue = stod(value);
		} catch (invalid_argument& e) {
			floatvalue = 0;
		}
		return floatvalue;
	}
}



//////////////////////////////
//
// HumHash::getValueBool -- Return true or false based on the
//   value.  If the value is "0" or false, then the function
//   will return false.  If the value is anything else, then
//   true will be returned.  If the parameter is not defined
//   in the HumHash, then false will also be defined.
//   See also hasParameter() if you do not like this last
//   behavior.
//

bool HumHash::getValueBool(const string& key) const {
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		return getValueBool("", "", keys[2]);
	} else if (keys.size() == 2) {
		return getValueBool(keys[0], keys[1]);
	} else {
		return getValueBool(keys[0], keys[1], keys[2]);
	}
}


bool HumHash::getValueBool(const string& ns2, const string& key) const {
	return getValueBool("", ns2, key);
}


bool HumHash::getValueBool(const string& ns1, const string& ns2,
		const string& key) const {
	if (parameters == NULL) {
		return false;
	}
	if (!isDefined(ns1, ns2, key)) {
		return false;
	}
	if ((*parameters)[ns1][ns2][key] == "false") {
		return false;
	} else if ((*parameters)[ns1][ns2][key] == "0") {
		return false;
	} else {
		return true;
	}
}



//////////////////////////////
//
// HumHash::setValue -- Set the parameter to the given value,
//     over-writing any previous value for the parameter.  The
//     value is any arbitrary string, but preferably does not
//     include tabs or colons.  If a colon is needed, then specify
//     as "&colon;" without the quotes.  Values such as integers
//     fractions and floats can be specified, and these wil be converted
//     internally into strings (use getValueInt() or getValueFloat()
//     to recover the original type).
//

void HumHash::setValue(const string& key, const string& value) {
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		setValue("", "", keys[0], value);
	} else if (keys.size() == 2) {
		setValue("", keys[0], keys[1], value);
	} else {
		setValue(keys[0], keys[1], keys[2], value);
	}
}


void HumHash::setValue(const string& ns2, const string& key,
		const string& value) {
		setValue("", ns2, key, value);
}


void HumHash::setValue(const string& ns1, const string& ns2,
		const string& key, const string& value) {
	initializeParameters();
	(*parameters)[ns1][ns2][key] = value;
}


void HumHash::setValue(const string& key, const char* value) {
	setValue(key, (string)value);
}


void HumHash::setValue(const string& ns2, const string& key,
		const char* value) {
	setValue(ns2, key, (string)value);
}


void HumHash::setValue(const string& ns1, const string& ns2, const string& key,
		const char* value) {
	setValue(ns1, ns2, key, (string)value);
}


void HumHash::setValue(const string& key, int value) {
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		setValue("", "", keys[0], value);
	} else if (keys.size() == 2) {
		setValue("", keys[0], keys[1], value);
	} else {
		setValue(keys[0], keys[1], keys[2], value);
	}
}


void HumHash::setValue(const string& ns2, const string& key, int value) {
		setValue("", ns2, key, value);
}


void HumHash::setValue(const string& ns1, const string& ns2,
		const string& key, int value) {
	initializeParameters();
	stringstream ss;
	ss << value;
	(*parameters)[ns1][ns2][key] = ss.str();
}


void HumHash::setValue(const string& key, HTp value) {
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		setValue("", "", keys[0], value);
	} else if (keys.size() == 2) {
		setValue("", keys[0], keys[1], value);
	} else {
		setValue(keys[0], keys[1], keys[2], value);
	}
}


void HumHash::setValue(const string& ns2, const string& key, HTp value) {
		setValue("", ns2, key, value);
}


void HumHash::setValue(const string& ns1, const string& ns2,
		const string& key, HTp value) {
	initializeParameters();
	stringstream ss;
	ss << "HT_" << ((long long)value);
	(*parameters)[ns1][ns2][key] = ss.str();
}


void HumHash::setValue(const string& key, HumNum value) {
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		setValue("", "", keys[0], value);
	} else if (keys.size() == 2) {
		setValue("", keys[0], keys[1], value);
	} else {
		setValue(keys[0], keys[1], keys[2], value);
	}
}


void HumHash::setValue(const string& ns2, const string& key, HumNum value) {
		setValue("", ns2, key, value);
}


void HumHash::setValue(const string& ns1, const string& ns2,
		const string& key, HumNum value) {
	initializeParameters();
	stringstream ss;
	ss << value;
	(*parameters)[ns1][ns2][key] = ss.str();
}


void HumHash::setValue(const string& key, double value) {
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		setValue("", "", keys[0], value);
	} else if (keys.size() == 2) {
		setValue("", keys[0], keys[1], value);
	} else {
		setValue(keys[0], keys[1], keys[2], value);
	}
}


void HumHash::setValue(const string& ns2, const string& key, double value) {
		setValue("", ns2, key, value);
}


void HumHash::setValue(const string& ns1, const string& ns2,
		const string& key, double value) {
	initializeParameters();
	stringstream ss;
	ss << value;
	(*parameters)[ns1][ns2][key] = ss.str();
}



//////////////////////////////
//
// HumHash::getKeys -- Return a list of keys in a particular namespace
//     combination.  With no parameters, a complete list of all
//     namespaces/keys will be returned.  Giving one parameter will
//     produce a list will give all NS2:key values in the NS1 namespace.
//     If there is a colon in the single parameter version of the function,
//     then this will be interpreted as "NS1", "NS2" version of the parameters
//     described above.
//

vector<string> HumHash::getKeys(const string& ns1, const string& ns2) const {
	vector<string> output;
	if (parameters == NULL) {
		return output;
	}
	for (auto& it : (*parameters)[ns1][ns2]) {
		output.push_back(it.first);
	}
	return output;
}


vector<string> HumHash::getKeys(const string& ns) const {
	vector<string> output;
	if (parameters == NULL) {
		return output;
	}
	auto loc = ns.find(":");
	if (loc != string::npos) {
		string ns1 = ns.substr(0, loc);
		string ns2 = ns.substr(loc+1);
		return getKeys(ns1, ns2);
	}

	for (auto& it1 : (*parameters)[ns]) {
		for (auto& it2 : it1.second) {
			output.push_back(it1.first + ":" + it2.first);
		}
	}
	return output;
}


vector<string> HumHash::getKeys(void) const {
	vector<string> output;
	if (parameters == NULL) {
		return output;
	}
	for (auto& it1 : (*parameters)) {
		for (auto& it2 : it1.second) {
			for (auto it3 : it2.second) {
				output.push_back(it1.first + ":" + it2.first + ":" + it3.first);
			}
		}
	}
	return output;
}



//////////////////////////////
//
// HumHash::hasParameters -- Returns true if at least one parameter is defined
//     in the HumHash object (when no arguments are given to the function).
//     When two strings are given as arguments, the function checks to see if
//     the given namespace pair has any keys.  If only one string argument,
//     then check if the given NS1 has any parameters, unless there is a
//     colon in the string which means to check NS1:NS2.
//

bool HumHash::hasParameters(const string& ns1, const string& ns2) const {
	if (parameters == NULL) {
		return false;
	}
	if (parameters->size() == 0) {
		return false;
	}
	auto it1 = parameters->find(ns1);
	if (it1 == parameters->end()) {
		return false;
	}
	auto it2 = (*parameters)[ns1].find(ns2);
	if (it2 == (*parameters)[ns1].end()) {
		return false;
	} else {
		return true;
	}
}


bool HumHash::hasParameters(const string& ns) const {
	if (parameters == NULL) {
		return false;
	}
	auto loc = ns.find(":");
	if (loc != string::npos) {
		string ns1 = ns.substr(0, loc);
		string ns2 = ns.substr(loc+1);
		return hasParameters(ns1, ns2);
	}

	auto it = parameters->find(ns);
	if (it == parameters->end()) {
		return false;
	} else {
		return true;
	}
}


bool HumHash::hasParameters(void) const {
	if (parameters == NULL) {
		return false;
	}
	if (parameters->size() == 0) {
		return false;
	}
	for (auto& it1 : *parameters) {
		for (auto& it2 : it1.second) {
			if (it2.second.size() == 0) {
				continue;
			} else {
				return true;
			}
		}
	}
	return false;
}



//////////////////////////////
//
// HumHash::getParameterCount -- Return a count of the parameters which are
//     stored in the HumHash.  If no arguments, then count all value in
//     all namespaces.  If two arguments, then return the count for a
//     specific NS1:NS2 namespace.  If one argument, then return the
//     parameters in NS1, but if there is a colon in the string,
//     return the parameters in NS1:NS2.
//
//

int HumHash::getParameterCount(const string& ns1, const string& ns2) const {
	if (parameters == NULL) {
		return 0;
	}
	if (parameters->size() == 0) {
		return 0;
	}
	auto it1 = parameters->find(ns1);
	if (it1 == parameters->end()) {
		return 0;
	}
	auto it2 = it1->second.find(ns2);
	if (it2 == it1->second.end()) {
		return 0;
	}
	return (int)it2->second.size();
}


int HumHash::getParameterCount(const string& ns) const {
	if (parameters == NULL) {
		return false;
	}
	auto loc = ns.find(":");
	if (loc != string::npos) {
		string ns1 = ns.substr(0, loc);
		string ns2 = ns.substr(loc+1);
		return getParameterCount(ns1, ns2);
	}

	auto it1 = parameters->find(ns);
	if (it1 == parameters->end()) {
		return false;
	}
	int sum = 0;
	for (auto& it2 : it1->second) {
		sum += it2.second.size();
	}
	return sum;
}


int HumHash::getParameterCount(void) const {
	if (parameters == NULL) {
		return 0;
	}
	if (parameters->size() == 0) {
		return 0;
	}
	int sum = 0;
	for (auto& it1 : (*parameters)) {
		for (auto& it2 : it1.second) {
			sum += it2.second.size();
		}
	}
	return sum;
}



//////////////////////////////
//
// HumHash::isDefined -- Returns true if the given parameter exists in the
//    map.   Format of the input string:   NS1:NS2:key or "":NS2:key for the
//    two argument version of the function.  OR "":"":key if no colons in
//    single string argument version.
//

bool HumHash::isDefined(const string& key) const {
	if (parameters == NULL) {
		return false;
	}
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		return (*parameters)[""][""].count(keys[0]);
	} else if (keys.size() == 2) {
		return (*parameters)[""][keys[0]].count(keys[1]);
	} else {
		return (*parameters)[keys[0]][keys[1]].count(keys[2]);
	}
}


bool HumHash::isDefined(const string& ns2, const string& key) const {
	if (parameters == NULL) {
		return false;
	}
	return (*parameters)[""][ns2].count(key);
}


bool HumHash::isDefined(const string& ns1, const string& ns2,
		const string& key) const {
	if (parameters == NULL) {
		return false;
	}
	return (*parameters)[ns1][ns2].count(key);
}



//////////////////////////////
//
// HumHash::deleteValue -- Delete the given parameter key from the HumHash
//   object.  Three string version is N1,NS2,key; two string version is
//   "",NS2,key; and one argument version is "","",key.
//

void HumHash::deleteValue(const string& key) {
	if (parameters == NULL) {
		return;
	}
	vector<string> keys = getKeyList(key);
	if (keys.size() == 1) {
		deleteValue("", "", keys[0]);
	} else if (keys.size() == 2) {
		deleteValue("", keys[0], keys[1]);
	} else {
		deleteValue(keys[0], keys[1], keys[2]);
	}
}


void HumHash::deleteValue(const string& ns2, const string& key) {
	if (parameters == NULL) {
		return;
	}
	deleteValue("", ns2, key);
}


void HumHash::deleteValue(const string& ns1, const string& ns2,
		const string& key) {
	if (parameters == NULL) {
		return;
	}
	(*parameters)[ns1][ns2].erase(key);

	MapNNKV& p = *parameters;
	auto it1 = p.find(ns1);
	if (it1 == p.end()) {
		return;
	}
	auto it2 = it1->second.find(ns2);
	if (it2 == it1->second.end()) {
		return;
	}
	auto it3 = it2->second.find(key);
	if (it3 == it2->second.end()) {
		return;
	}
	it2->second.erase(key);
}



//////////////////////////////
//
// HumHash::initializeParameters -- Create the map structure if it does not
//     already exist.
//

void HumHash::initializeParameters(void) {
	if (parameters == NULL) {
		parameters = new MapNNKV;
	}
}



//////////////////////////////
//
// HumHash::getKeyList -- Return a list of colon separated values from
//      the string.
//

vector<string> HumHash::getKeyList(const string& keys) const {
	stringstream ss(keys);
	string key;
	vector<string> output;
	while (getline(ss, key, ':')) {
		output.push_back(key);
	}
	if (output.size() == 0) {
		output.push_back(keys);
	}
	return output;
}



//////////////////////////////
//
// HumHash::setPrefix -- initial string to print when using
//   operator<<.  This is used for including the "!" for local
//   comments or "!!" for global comments.   The prefix will
//   remain the same until it is changed.  The default prefix
//   of the object it the empty string.
//

void HumHash::setPrefix(const string& value) {
	prefix = value;
}



//////////////////////////////
//
// HumHash::getPrefix -- get the prefix.
//

string HumHash::getPrefix(void) const {
	return prefix;
}



//////////////////////////////
//
// HumHash::setOrigin -- Set the source token for the parameter.
//

void HumHash::setOrigin(const string& key, HumdrumToken* tok) {
	if (parameters == NULL) {
		return;
	} else {
		vector<string> keys = getKeyList(key);
		if (keys.size() == 1) {
			setOrigin("", "", keys[0], tok);
		} else if (keys.size() == 2) {
			setOrigin("", keys[0], keys[1], tok);
		} else {
			setOrigin(keys[0], keys[1], keys[2], tok);
		}
	}
}


void HumHash::setOrigin(const string& key, HumdrumToken& tok) {
	setOrigin(key, &tok);
}


void HumHash::setOrigin(const string& ns2, const string& key,
		HumdrumToken* tok) {
	if (parameters == NULL) {
		return;
	} else {
		setOrigin("", ns2, key, tok);
	}
}


void HumHash::setOrigin(const string& ns2, const string& key,
		HumdrumToken& tok) {
	setOrigin(ns2, key, &tok);
}


void HumHash::setOrigin(const string& ns1, const string& ns2,
		const string& key, HumdrumToken* tok) {
	if (parameters == NULL) {
		return;
	}
	MapNNKV& p = *parameters;
	auto it1 = p.find(ns1);
	if (it1 == p.end()) {
		return;
	}
	auto it2 = it1->second.find(ns2);
	if (it2 == it1->second.end()) {
		return;
	}
	auto it3 = it2->second.find(key);
	if (it3 == it2->second.end()) {
		return;
	}
	it3->second.origin = tok;
}


void HumHash::setOrigin(const string& ns1, const string& ns2,
		const string& key, HumdrumToken& tok) {
	setOrigin(ns1, ns2, key, &tok);
}



//////////////////////////////
//
// HumHash::getOrigin -- Get the source token for the parameter.
//    Returns NULL if there is no origin.
//

HumdrumToken* HumHash::getOrigin(const string& key) const {
	if (parameters == NULL) {
		return NULL;
	} else {
		vector<string> keys = getKeyList(key);
		if (keys.size() == 1) {
			return getOrigin("", "", keys[0]);
		} else if (keys.size() == 2) {
			return getOrigin("", keys[0], keys[1]);
		} else {
			return getOrigin(keys[0], keys[1], keys[2]);
		}
	}
}


HumdrumToken* HumHash::getOrigin(const string& ns2, const string& key) const {
	if (parameters == NULL) {
		return NULL;
	} else {
		return getOrigin("", ns2, key);
	}
}


HumdrumToken* HumHash::getOrigin(const string& ns1, const string& ns2,
		const string& key) const {
	if (parameters == NULL) {
		return NULL;
	}
	MapNNKV& p = *parameters;
	auto it1 = p.find(ns1);
	if (it1 == p.end()) {
		return NULL;
	}
	auto it2 = it1->second.find(ns2);
	if (it2 == it1->second.end()) {
		return NULL;
	}
	auto it3 = it2->second.find(key);
	if (it3 == it2->second.end()) {
		return NULL;
	}
	return it3->second.origin;
}



//////////////////////////////
//
// HumHash::printXml -- Print object as a <parameters> element for
//     in a HumdrumXML file.
//

ostream& HumHash::printXml(ostream& out, int level, const string& indent) {
	if (parameters == NULL) {
		return out;
	}
	if (parameters->size() == 0) {
		return out;
	}
	
	stringstream str;
	bool found = 0;

	HumdrumToken* ref = NULL;
	level++;
	for (auto& it1 : *(parameters)) {
		if (it1.second.size() == 0) {
			continue;
		}
		if (!found) {
			found = 1;
		}
		str << Convert::repeatString(indent, level++);
		str << "<namespace n=\"1\" name=\"" << it1.first << "\">\n";
		for (auto& it2 : it1.second) {
			if (it2.second.size() == 0) {
				continue;
			}

			str << Convert::repeatString(indent, level++);
			str << "<namespace n=\"2\" name=\"" << it2.first << "\">\n";

			for (auto& it3 : it2.second) {
				str << Convert::repeatString(indent, level);
				str << "<parameter key=\"" << it3.first << "\"";
				str << " value=\"";
				str << Convert::encodeXml(it3.second) << "\"";
				str << " idref=\"";
				ref = it3.second.origin;
				if (ref != NULL) {
					str << ref->getXmlId();
				}
				str << "\"";
				str << "/>\n";
			}
			str << Convert::repeatString(indent, --level) << "</namespace>\n";
		}
		str << Convert::repeatString(indent, --level) << "</namespace>\n";
	}
	if (found) {
		str << Convert::repeatString(indent, --level) << "</parameters>\n";
		out << Convert::repeatString(indent, level) << "<parameters>\n";
		out << str.str();
	}

	return out;
}



//////////////////////////////
//
// operator<< -- Print a list of the parameters in a HumHash object.
//

ostream& operator<<(ostream& out, const HumHash& hash) {
	if (hash.parameters == NULL) {
		return out;
	}
	if (hash.parameters->size() == 0) {
		return out;
	}

	string cleaned;

	for (auto& it1 : *(hash.parameters)) {
		if (it1.second.size() == 0) {
			continue;
		}
		for (auto& it2 : it1.second) {
			if (it2.second.size() == 0) {
				continue;
			}
			out << hash.prefix;
			out << it1.first << ":" << it2.first;
			for (auto& it3 : it2.second) {
				out << ":" << it3.first;
				if (it3.second != "true") {
					cleaned = it3.second;
					Convert::replaceOccurrences(cleaned, ":", "&colon;");
					out << "=" << cleaned;
				}
			}
			out << endl;
		}
	}

	return out;
}



typedef long TEMP64BITFIX;

// declare static variables
vector<_HumInstrument> HumInstrument::data;
int HumInstrument::classcount = 0;


//////////////////////////////
//
// HumInstrument::HumInstrument --
//

HumInstrument::HumInstrument(void) {
	if (classcount == 0) {
		initialize();
	}
	classcount++;
	index = -1;
}



//////////////////////////////
//
// HumInstrument::HumInstrument --
//

HumInstrument::HumInstrument(const string& Hname) {
	if (classcount == 0) {
		initialize();
	}

	index = find(Hname);
}



//////////////////////////////
//
// HumInstrument::~HumInstrument --
//

HumInstrument::~HumInstrument() {
	index = -1;
}



/////////////////////////////
//
// HumInstrument::getGM --
//

int HumInstrument::getGM(void) {
	if (index > 0) {
		return data[index].gm;
	} else {
		return -1;
	}
}



/////////////////////////////
//
// HumInstrument::getGM --
//

int HumInstrument::getGM(const string& Hname) {
	int tindex;
	if (Hname.compare(0, 2, "*I") == 0) {
		tindex = find(Hname.substr(2));
	} else {
		tindex = find(Hname);
	}

	if (tindex > 0) {
		return data[tindex].gm;
	} else {
		return -1;
	}
}



//////////////////////////////
//
// HumInstrument::getName --
//

string HumInstrument::getName(void) {
	if (index > 0) {
		return data[index].name;
	} else {
		return "";
	}
}



//////////////////////////////
//
// HumInstrument::getName --
//

string HumInstrument::getName(const string& Hname) {
	int tindex;
	if (Hname.compare(0, 2, "*I") == 0) {
		tindex = find(Hname.substr(2));
	} else{
		tindex = find(Hname);
	}
	if (tindex > 0) {
		return data[tindex].name;
	} else {
		return "";
	}
}



//////////////////////////////
//
// HumInstrument::getHumdrum --
//

string HumInstrument::getHumdrum(void) {
	if (index > 0) {
		return data[index].humdrum;
	} else {
		return "";
	}
}



//////////////////////////////
//
// HumInstrument::setGM --
//

int HumInstrument::setGM(const string& Hname, int aValue) {
	if (aValue < 0 || aValue > 127) {
		return 0;
	}
	int rindex = find(Hname);
	if (rindex > 0) {
		data[rindex].gm = aValue;
	} else {
		afi(Hname.c_str(), aValue, Hname.c_str());
		sortData();
	}
	return rindex;
}



//////////////////////////////
//
// HumInstrument::setHumdrum --
//

void HumInstrument::setHumdrum(const string& Hname) {
	if (Hname.compare(0, 2, "*I") == 0) {
		index = find(Hname.substr(2));
	} else {
		index = find(Hname);
	}
}



//////////////////////////////////////////////////////////////////////////
//
// private functions
//


//////////////////////////////
//
// HumInstrument::initialize --
//

void HumInstrument::initialize(void) {
	data.reserve(500);
	afi("accor",	GM_ACCORDION,	"accordion");
	afi("alto",		GM_RECORDER,	"alto");
	afi("archl",	GM_ACOUSTIC_GUITAR_NYLON,	"archlute");
	afi("armon",	GM_HARMONICA,	"harmonica");
	afi("arpa",		GM_ORCHESTRAL_HARP,	"harp");
	afi("bagpI",	GM_BAGPIPE,	"bagpipe (Irish)");
	afi("bagpS",	GM_BAGPIPE,	"bagpipe (Scottish)");
	afi("banjo",	GM_BANJO,	"banjo");
	afi("barit",	GM_CHOIR_AAHS,	"baritone");
	afi("baset",	GM_CLARINET,	"bassett horn");
	afi("bass",		GM_CHOIR_AAHS,	"bass");
	afi("bdrum",	GM_TAIKO_DRUM,	"bass drum (kit)");
	afi("bguit",	GM_ELECTRIC_BASS_FINGER,	"electric bass guitar");
	afi("biwa",		GM_FLUTE,	"biwa");
	afi("bscan",	GM_CHOIR_AAHS,	"basso cantante");
	afi("bspro",	GM_CHOIR_AAHS,	"basso profondo");
	afi("calam",	GM_OBOE,	"chalumeau");
	afi("calpe",	GM_LEAD_CALLIOPE,	"calliope");
	afi("calto",	GM_CHOIR_AAHS,	"contralto");
	afi("campn",	GM_TUBULAR_BELLS,	"bell");
	afi("cangl",	GM_ENGLISH_HORN,	"english horn");
	afi("caril",	GM_TUBULAR_BELLS,	"carillon");
	afi("castr",	GM_CHOIR_AAHS,	"castrato");
	afi("casts",	GM_WOODBLOCKS,	"castanets");
	afi("cbass",	GM_CONTRABASS,	"contrabass");
	afi("cello",	GM_CELLO,	"violoncello");
	afi("cemba",	GM_HARPSICHORD,	"harpsichord");
	afi("cetra",	GM_VIOLIN,	"cittern");
	afi("chime",	GM_TUBULAR_BELLS,	"chimes");
	afi("chlma",	GM_BASSOON,	"alto shawm");
	afi("chlms",	GM_BASSOON,	"soprano shawm");
	afi("chlmt",	GM_BASSOON,	"tenor shawm");
	afi("clara",	GM_CLARINET,	"alto clarinet (in E-flat)");
	afi("clarb",	GM_CLARINET,	"bass clarinet (in B-flat)");
	afi("clarp",	GM_CLARINET,	"piccolo clarinet");
	afi("clars",	GM_CLARINET,	"soprano clarinet");
	afi("clavi",	GM_CLAVI,	"clavichord");
	afi("clest",	GM_CELESTA,	"celesta");
	afi("colsp",	GM_FLUTE,	"coloratura soprano");
	afi("cor",		GM_FRENCH_HORN,	"horn");
	afi("cornm",	GM_BAGPIPE,	"French bagpipe");
	afi("corno",	GM_TRUMPET,	"cornett");
	afi("cornt",	GM_TRUMPET,	"cornet");
	afi("crshc",	GM_REVERSE_CYMBAL,	"crash cymbal (kit)");
	afi("ctenor",	GM_CHOIR_AAHS,	"counter-tenor");
	afi("ctina",	GM_ACCORDION,	"concertina");
	afi("drmsp",	GM_FLUTE,	"dramatic soprano");
	afi("dulc",		GM_DULCIMER,	"dulcimer");
	afi("eguit",	GM_ELECTRIC_GUITAR_CLEAN,	"electric guitar");
	afi("fag_c",	GM_BASSOON,	"contrabassoon");
	afi("fagot",	GM_BASSOON,	"bassoon");
	afi("false",	GM_RECORDER,	"falsetto");
	afi("feme",		GM_CHOIR_AAHS,	"female voice");
	afi("fife",		GM_BLOWN_BOTTLE,	"fife");
	afi("fingc",	GM_REVERSE_CYMBAL,	"finger cymbal");
	afi("flt",		GM_FLUTE,	"flute");
	afi("flt_a",	GM_FLUTE,	"alto flute");
	afi("flt_b",	GM_FLUTE,	"bass flute");
	afi("fltda",	GM_RECORDER,	"alto recorder");
	afi("fltdb",	GM_RECORDER,	"bass recorder");
	afi("fltdn",	GM_RECORDER,	"sopranino recorder");
	afi("fltds",	GM_RECORDER,	"soprano recorder");
	afi("fltdt",	GM_RECORDER,	"tenor recorder");
	afi("flugh",	GM_FRENCH_HORN,	"flugelhorn");
	afi("forte",	GM_HONKYTONK_PIANO,	"fortepiano");
	afi("glock",	GM_GLOCKENSPIEL,	"glockenspiel");
	afi("gong", 	GM_STEEL_DRUMS,	"gong");
	afi("guitr",	GM_ACOUSTIC_GUITAR_NYLON,	"guitar");
	afi("hammd",	GM_DRAWBAR_ORGAN,	"Hammond electronic organ");
	afi("heltn",	GM_CHOIR_AAHS,	"Heldentenor");
	afi("hichi",	GM_OBOE,	"hichiriki");
	afi("hurdy",	GM_LEAD_CALLIOPE,	"hurdy-gurdy");
	afi("kit",		GM_SYNTH_DRUM,	"drum kit");
	afi("kokyu",	GM_FIDDLE,	"kokyu (Japanese spike fiddle)");
	afi("komun",	GM_KOTO,	"komun'go (Korean long zither)");
	afi("koto",		GM_KOTO,	"koto (Japanese long zither)");
	afi("kruma",	GM_TRUMPET,	"alto crumhorn");
	afi("krumb",	GM_TRUMPET,	"bass crumhorn");
	afi("krums",	GM_TRUMPET,	"soprano crumhorn");
	afi("krumt",	GM_TRUMPET,	"tenor crumhorn");
	afi("liuto",	GM_ACOUSTIC_GUITAR_NYLON,	"lute");
	afi("lyrsp",	GM_FLUTE,	"lyric soprano");
	afi("lyrtn",	GM_FRENCH_HORN,	"lyric tenor");
	afi("male",		GM_CHOIR_AAHS,  	"male voice");
	afi("mando",	GM_ACOUSTIC_GUITAR_NYLON,	"mandolin");
	afi("marac",	GM_AGOGO,	"maracas");
	afi("marim",	GM_MARIMBA,	"marimba");
	afi("mezzo",	GM_CHOIR_AAHS,  	"mezzo soprano");
	afi("nfant",	GM_CHOIR_AAHS,  	"child's voice");
	afi("nokan",	GM_SHAKUHACHI,	"nokan (a Japanese flute)");
	afi("oboeD",	GM_ENGLISH_HORN,	"oboe d'amore");
	afi("oboe",		GM_OBOE,	"oboe");
	afi("ocari",	GM_OCARINA,	"ocarina");
	afi("organ",	GM_CHURCH_ORGAN,	"pipe organ");
	afi("panpi",	GM_PAN_FLUTE,	"panpipe");
	afi("piano",	GM_ACOUSTIC_GRAND_PIANO,	"pianoforte");
	afi("piatt",	GM_REVERSE_CYMBAL,	"cymbals");
	afi("picco",	GM_PICCOLO,	"piccolo");
	afi("pipa",		GM_ACOUSTIC_GUITAR_NYLON,	"Chinese lute");
	afi("porta",	GM_TANGO_ACCORDION,	"portative organ");
	afi("psalt",	GM_CLAVI,	"psaltery (box zither)");
	afi("qin",		GM_CLAVI,	"qin, ch'in (Chinese zither)");
	afi("quitr",	GM_ACOUSTIC_GUITAR_NYLON,	"gittern");
	afi("rackt",	GM_TRUMPET,	"racket");
	afi("rebec",	GM_ACOUSTIC_GUITAR_NYLON,	"rebec");
	afi("recit",	GM_CHOIR_AAHS,  	"recitativo");
	afi("reedo",	GM_REED_ORGAN,	"reed organ");
	afi("rhode",	GM_ELECTRIC_PIANO_1,	"Fender-Rhodes electric piano");
	afi("ridec",	GM_REVERSE_CYMBAL,	"ride cymbal (kit)");
	afi("sarod",	GM_SITAR,	"sarod");
	afi("sarus",	GM_TUBA,	"sarrusophone");
	afi("saxA",		GM_ALTO_SAX,	"E-flat alto saxophone");
	afi("saxB",		GM_BARITONE_SAX,	"B-flat bass saxophone");
	afi("saxC",		GM_BARITONE_SAX,	"E-flat contrabass saxophone");
	afi("saxN",		GM_SOPRANO_SAX,	"E-flat sopranino saxophone");
	afi("saxR",		GM_BARITONE_SAX,	"E-flat baritone saxophone");
	afi("saxS",		GM_SOPRANO_SAX,	"B-flat soprano saxophone");
	afi("saxT",		GM_TENOR_SAX,	"B-flat tenor saxophone");
	afi("sdrum",	GM_SYNTH_DRUM,	"snare drum (kit)");
	afi("shaku",	GM_SHAKUHACHI,	"shakuhachi");
	afi("shami",	GM_SHAMISEN,	"shamisen (Japanese fretless lute)");
	afi("sheng",	GM_SHANAI,	"mouth organ (Chinese)");
	afi("sho",		GM_SHANAI,	"mouth organ (Japanese)");
	afi("sitar",	GM_SITAR,	"sitar");
	afi("soprn",	GM_CHOIR_AAHS,  	"soprano");
	afi("spshc",	GM_REVERSE_CYMBAL,	"splash cymbal (kit)");
	afi("steel",	GM_STEEL_DRUMS,	"steel-drum");
	afi("sxhA",		GM_ALTO_SAX,	"E-flat alto saxhorn");
	afi("sxhB",		GM_BARITONE_SAX,	"B-flat bass saxhorn");
	afi("sxhC",		GM_BARITONE_SAX,	"E-flat contrabass saxhorn");
	afi("sxhR",		GM_BARITONE_SAX,	"E-flat baritone saxhorn");
	afi("sxhS",		GM_SOPRANO_SAX,	"B-flat soprano saxhorn");
	afi("sxhT",		GM_TENOR_SAX,	"B-flat tenor saxhorn");
	afi("synth",	GM_ELECTRIC_PIANO_2,	"keyboard synthesizer");
	afi("tabla",	GM_MELODIC_DRUM,	"tabla");
	afi("tambn",	GM_TINKLE_BELL,	"tambourine");
	afi("tambu",	GM_MELODIC_DRUM,	"tambura");
	afi("tanbr",	GM_MELODIC_DRUM,	"tanbur");
	afi("tenor",	GM_CHOIR_AAHS,	"tenor");
	afi("timpa",	GM_MELODIC_DRUM,	"timpani");
	afi("tiorb",	GM_ACOUSTIC_GUITAR_NYLON,	"theorbo");
	afi("tom",		GM_TAIKO_DRUM,	"tom-tom drum");
	afi("trngl",	GM_TINKLE_BELL,	"triangle");
	afi("tromb",	GM_TROMBONE,	"bass trombone");
	afi("tromp",	GM_TRUMPET,	"trumpet");
	afi("tromt",	GM_TROMBONE,	"tenor trombone");
	afi("tuba",		GM_TUBA,	"tuba");
	afi("ud",		GM_ACOUSTIC_GUITAR_NYLON,	"ud");
	afi("ukule",	GM_ACOUSTIC_GUITAR_NYLON,	"ukulele");
	afi("vibra",	GM_VIBRAPHONE,	"vibraphone");
	afi("vina",		GM_SITAR,	"vina");
	afi("viola",	GM_VIOLA,	"viola");
	afi("violb",	GM_CONTRABASS,	"bass viola da gamba");
	afi("viold",	GM_VIOLA,	"viola d'amore");
	afi("violn",	GM_VIOLIN,	"violin");
	afi("violp",	GM_VIOLIN,	"piccolo violin");
	afi("viols",	GM_VIOLIN,	"treble viola da gamba");
	afi("violt",	GM_CELLO,	"tenor viola da gamba");
	afi("vox",		GM_CHOIR_AAHS,  	"generic voice");
	afi("xylo",		GM_XYLOPHONE,	"xylophone");
	afi("zithr",	GM_CLAVI,	"zither");
	afi("zurna",	GM_ACOUSTIC_GUITAR_NYLON,	"zurna");
}



//////////////////////////////
//
// HumInstrument::afi --
//

void HumInstrument::afi(const char* humdrum_name, int midinum,
		const char* EN_name) {
	_HumInstrument x;
	x.name = EN_name;
	x.humdrum = humdrum_name;
	x.gm = midinum;

	data.push_back(x);
}



//////////////////////////////
//
// HumInstrument::find --
//

int HumInstrument::find(const string& Hname) {
	void* searchResult;
	_HumInstrument key;
	key.humdrum = Hname;
	key.name = "";
	key.gm = 0;

	searchResult = bsearch(&key, data.data(),
			data.size(), sizeof(_HumInstrument),
			&data_compare_by_humdrum_name);

	if (searchResult == NULL) {
		return -1;
	} else {
		return (int)(((TEMP64BITFIX)(searchResult)) - ((TEMP64BITFIX)(data.data())))/
			sizeof(_HumInstrument);
	}
}


//////////////////////////////
//
// HumInstrument::data_compare_by_humdrum_name --
//

int HumInstrument::data_compare_by_humdrum_name(const void* a,
		const void* b) {
	_HumInstrument& valuea = *((_HumInstrument*)a);
	_HumInstrument& valueb = *((_HumInstrument*)b);
	return strcmp(valuea.humdrum.c_str(), valueb.humdrum.c_str());
}



//////////////////////////////
//
// HumInstrument::sortData --
//

void HumInstrument::sortData(void) {
	qsort(data.data(), data.size(), sizeof(_HumInstrument),
		&HumInstrument::data_compare_by_humdrum_name);
}



//////////////////////////////
//
// HumNum::HumNum -- HumNum Constructor.  Set the default value
//   of the number to zero, or the given number if specified.
//

HumNum::HumNum(void){
	top = 0;
	bot = 1;
}


HumNum::HumNum(int value){
	top = value;
	bot = 1;
}


HumNum::HumNum(int numerator, int denominator){
	setValue(numerator, denominator);
}


HumNum::HumNum(const string& ratstring) {
	setValue(ratstring);
}


HumNum::HumNum(const char* ratstring) {
	setValue(ratstring);
}


HumNum::HumNum(const HumNum& rat) {
	top = rat.top;
	bot = rat.bot;
}



//////////////////////////////
//
// HumNum::~HumNum -- HumNum deconstructor.
//

HumNum::~HumNum() {
	// do nothing
}



//////////////////////////////
//
// HumNum::isNegative -- Returns true if value is negative.
//

bool HumNum::isNegative(void) const {
	return isFinite() && (top < 0);
}



//////////////////////////////
//
// HumNum::isPositive -- Returns true if value is positive.
//

bool HumNum::isPositive(void) const {
	return isFinite() && (top > 0);
}



//////////////////////////////
//
// HumNum::isZero -- Returns true if value is zero.
//

bool HumNum::isZero(void) const {
	return isFinite() && (top == 0);
}



//////////////////////////////
//
// HumNum::isNonZero -- Returns true if value is not zero.
//

bool HumNum::isNonZero(void) const {
	return isFinite() && (top != 0);
}



//////////////////////////////
//
// HumNum::isNonNegative -- Returns true if value is non-negative.
//

bool HumNum::isNonNegative(void) const {
	return isFinite() && (top >= 0);
}



//////////////////////////////
//
// HumNum::isNonPositive -- Returns true if value is non-positive.
//

bool HumNum::isNonPositive(void) const {
	return isFinite() && (top >= 0);
}



//////////////////////////////
//
// HumNum::getFloat -- Returns the floating-point equivalent of the
//     rational number.
//

double HumNum::getFloat(void) const {
	return (double)top/(double)bot;
}



//////////////////////////////
//
// HumNum::getInteger -- Returns the integral part of the fraction.
//    Default value: round = 0.0
//    Optional parameter is a rounding factor.
//    Examples:
//       8/5 | round=0.0 ==  1
//      -8/5 | round=0.0 == -1
//       8/5 | round=0.5 ==  1
//      -8/5 | round=0.5 == -1
//

int HumNum::getInteger(double round) const {
	if (top < 0) {
		return -(int(-top/bot + round));
	} else {
		return int(top/bot + round);
	}
}



//////////////////////////////
//
// HumNum::getNumerator -- Returns the top integer in the fraction.
//

int HumNum::getNumerator(void) const {
	return top;
}



//////////////////////////////
//
// HumNum::getDenominator -- Returns the bottom integer in the fraction.
//

int HumNum::getDenominator(void) const {
	return bot;
}



//////////////////////////////
//
// HumNum::getRemainder -- Returns the non-integer fractional part of the value.
//

HumNum HumNum::getRemainder(void) const {
	return (*this) - toInteger();
}



//////////////////////////////
//
// HumNum::setValue -- Set the number to the given integer.
//    For the two-parameter version, set the top and bottom
//    values for the number, reducing if necessary.  For the
//    string version, parse an integer or fraction from the
//    string and reduce if necessary.
//

void HumNum::setValue(int numerator) {
	top = numerator;
	bot = 1;
}


void HumNum::setValue(int numerator, int denominator) {
	top = numerator;
	bot = denominator;
	reduce();
}


void HumNum::setValue(const string& ratstring) {
	int buffer[2];
	buffer[0] = 0;
	buffer[1] = 0;
	int slash = 0;
	for (int i=0; i<(int)ratstring.size(); i++) {
		if (ratstring[i] == '/') {
			slash = 1;
			continue;
		}
		if (!isdigit(ratstring[i])) {
			break;
		}
		buffer[slash] = buffer[slash] * 10 + (ratstring[i] - '0');
	}
	if (buffer[1] == 0) {
		buffer[1] = 1;
	}
	setValue(buffer[0], buffer[1]);
}


void HumNum::setValue(const char* ratstring) {
	string realstring = ratstring;
	setValue(realstring);
}



//////////////////////////////
//
// HumNum::getAbs -- returns the absolute value of the rational number.
//

HumNum HumNum::getAbs(void) const {
	HumNum rat(top, bot);
	if (isNegative()) {
		rat.setValue(-top, bot);
	}
	return rat;
}



//////////////////////////////
//
// HumNum::makeAbs -- Make the rational number non-negative.
//

HumNum& HumNum::makeAbs(void) {
	if (!isNonNegative()) {
		top = -top;
	}
	return *this;
}



//////////////////////////////
//
// HumNum::reduce -- simplify the fraction.  For example, 4/24 will
//    reduce to 1/6 since a factor of 4 is common to the numerator
//    and denominator.
//

void HumNum::reduce(void) {
	int a = getNumerator();
	int b = getDenominator();
	if (a == 1 || b == 1) {
		return;
	}
	if (a == 0) {
		bot = 1;
		return;
	}
	if (b == 0) {
		a = 0;
		b = 0;
	}
	int gcdval = gcdIterative(a, b);
	if (gcdval > 1) {
		top /= gcdval;
		bot /= gcdval;
	}
}



//////////////////////////////
//
// HumNum::gcdIterative -- Returns the greatest common divisor of two
//      numbers using an iterative algorithm.
//

int HumNum::gcdIterative(int a, int b) {
	int c;
	while (b) {
		c = a;
		a = b;
		b = c % b;
	}
	return a < 0 ? -a : a;
}



//////////////////////////////
//
// HumNum::gcdRecursive -- Returns the greatest common divisor of two
//      numbers using a recursive algorithm.
//

int HumNum::gcdRecursive(int a, int b) {
	if (a < 0) {
		a = -a;
	}
	if (!b) {
		return a;
	} else {
		return gcdRecursive(b, a % b);
	}
}



//////////////////////////////
//
// HumNum::isInfinite -- Returns true if the denominator is zero.
//

bool HumNum::isInfinite(void) const {
	return (bot == 0) && (top != 0);
}



//////////////////////////////
//
// HumNum::isNaN -- Returns true if the numerator and denominator
//     are both zero.
//

bool HumNum::isNaN(void) const {
	return (bot == 0) && (top == 0);
}



//////////////////////////////
//
// HumNum::isFinite -- Returns true if the denominator is not zero.
//

bool HumNum::isFinite(void) const {
	return bot != 0;
}



//////////////////////////////
//
// HumNum::isInteger -- Returns true if number is an integer.
//

bool HumNum::isInteger(void) const {
	return isFinite() && (bot == 1);
}



//////////////////////////////
//
// HumNum::isPowerOfTwo -- Returns true if a power of two.
//

bool HumNum::isPowerOfTwo(void) const {
	if (top == 0) {
		return false;
	}
	int abstop = top > 0 ? top : -top;
	if (bot == 1) {
		return !(abstop & (abstop - 1));
	} else if (abstop == 1) {
		return !(bot & (bot - 1));
	}
	return false;
}



//////////////////////////////
//
// HumNum::operator+ -- Addition operator which adds HumNum
//    to another HumNum or with a integers.
//

HumNum HumNum::operator+(const HumNum& value) const {
	int a1  = getNumerator();
	int b1  = getDenominator();
	int a2  = value.getNumerator();
	int b2  = value.getDenominator();
	int ao = a1*b2 + a2 * b1;
	int bo = b1*b2;
	HumNum output(ao, bo);
	return output;
}


HumNum HumNum::operator+(int value) const {
	HumNum output(value * bot + top, bot);
	return output;
}



//////////////////////////////
//
// HumNum::operator- -- Subtraction operator to subtract
//     HumNums from each other and to subtrack integers from
//     HumNums.
//

HumNum HumNum::operator-(const HumNum& value) const {
	int a1  = getNumerator();
	int b1  = getDenominator();
	int a2  = value.getNumerator();
	int b2  = value.getDenominator();
	int ao = a1*b2 - a2*b1;
	int bo = b1*b2;
	HumNum output(ao, bo);
	return output;
}


HumNum HumNum::operator-(int value) const {
	HumNum output(top - value * bot, bot);
	return output;
}



//////////////////////////////
//
// HumNum::operator- -- Unary negation operator to generate
//   the negative version of a HumNum.
//

HumNum HumNum::operator-(void) const {
	HumNum output(-top, bot);
	return output;
}



//////////////////////////////
//
// HumNum::operator* -- Multiplication operator to multiply
//   two HumNums together or a HumNum and an integer.
//

HumNum HumNum::operator*(const HumNum& value) const {
	int a1  = getNumerator();
	int b1  = getDenominator();
	int a2  = value.getNumerator();
	int b2  = value.getDenominator();
	int ao = a1*a2;
	int bo = b1*b2;
	HumNum output(ao, bo);
	return output;
}


HumNum HumNum::operator*(int value) const {
	HumNum output(top * value, bot);
	return output;
}



//////////////////////////////
//
// HumNum::operator/ -- Division operator to divide two
//     HumNums together or divide a HumNum by an integer.
//

HumNum HumNum::operator/(const HumNum& value) const {
	int a1  = getNumerator();
	int b1  = getDenominator();
	int a2  = value.getNumerator();
	int b2  = value.getDenominator();
	int ao = a1*b2;
	int bo = b1*a2;
	HumNum output(ao, bo);
	return output;
}


HumNum HumNum::operator/(int value) const {
	int a  = getNumerator();
	int b  = getDenominator();
	if (value < 0) {
		a = -a;
		b *= -value;
	} else {
		b *= value;
	}
	HumNum output(a, b);
	return output;
}



//////////////////////////////
//
// HumNum::operator= -- Assign the contents of a HumNum
//    from another HumNum.
//

HumNum& HumNum::operator=(const HumNum& value) {
	if (this == &value) {
		return *this;
	}
	setValue(value.top, value.bot);
	return *this;
}

HumNum& HumNum::operator=(int  value) {
	setValue(value);
	return *this;
}



//////////////////////////////
//
// HumNum::operator+= -- Add a HumNum or integer to a HumNum.
//

HumNum& HumNum::operator+=(const HumNum& value) {
	*this = *this + value;
	return *this;
}


HumNum& HumNum::operator+=(int value) {
	*this = *this + value;
	return *this;
}



//////////////////////////////
//
// HumNum::operator-= -- Subtract a HumNum or an integer from
//    a HumNum.
//

HumNum& HumNum::operator-=(const HumNum& value) {
	*this = *this - value;
	return *this;
}


HumNum& HumNum::operator-=(int value) {
	*this = *this - value;
	return *this;
}



//////////////////////////////
//
// HumNum::operator*= -- Multiply a HumNum by a HumNum or integer.
//

HumNum& HumNum::operator*=(const HumNum& value) {
	*this = *this * value;
	return *this;
}


HumNum& HumNum::operator*=(int value) {
	*this = *this * value;
	return *this;
}



//////////////////////////////
//
// HumNum::operator/= -- Divide a HumNum by a HumNum or integer.
//

HumNum& HumNum::operator/=(const HumNum& value) {
	*this = *this / value;
	return *this;
}


HumNum& HumNum::operator/=(int value) {
	*this = *this / value;
	return *this;
}



//////////////////////////////
//
// HumNum::operator< -- Less-than equality for a HumNum and
//   a HumNum, integer, or float.
//

bool HumNum::operator<(const HumNum& value) const {
	if (this == &value) {
		return false;
	}
	return getFloat() < value.getFloat();
}


bool HumNum::operator<(int value) const {
	return getFloat() < value;
}


bool HumNum::operator<(double value) const {
	return getFloat() < value;
}



//////////////////////////////
//
// HumNum::operator<= -- Less-than-or-equal equality for a
//     HumNum with a HumNum, integer or float.
//

bool HumNum::operator<=(const HumNum& value) const {
	if (this == &value) {
		return true;
	}
	return getFloat() <= value.getFloat();
}


bool HumNum::operator<=(int value) const {
	return getFloat() <= value;
}


bool HumNum::operator<=(double value) const {
	return getFloat() <= value;
}



//////////////////////////////
//
// HumNum::operator> -- Greater-than equality for a HumNum
//     compared to a HumNum, integer, or float.
//

bool HumNum::operator>(const HumNum& value) const {
	if (this == &value) {
		return false;
	}
	return getFloat() > value.getFloat();
}


bool HumNum::operator>(int value) const {
	return getFloat() > value;
}


bool HumNum::operator>(double value) const {
	return getFloat() > value;
}



//////////////////////////////
//
// HumNum::operator>= -- Greater-than-or-equal equality
//    comparison for a HumNum to another HumNum, integer, or float.
//

bool HumNum::operator>=(const HumNum& value) const {
	if (this == &value) {
		return true;
	}
	return getFloat() >= value.getFloat();
}


bool HumNum::operator>=(int value) const {
	return getFloat() >= value;
}


bool HumNum::operator>=(double value) const {
	return getFloat() >= value;
}



//////////////////////////////
//
// HumNum::operator== -- Equality test for HumNums compared to
//   another HumNum, integer or float.
//

bool HumNum::operator==(const HumNum& value) const {
	if (this == &value) {
		return true;
	}
	return getFloat() == value.getFloat();
}


bool HumNum::operator==(int value) const {
	return getFloat() == value;
}


bool HumNum::operator==(double value) const {
	return getFloat() == value;
}



//////////////////////////////
//
// HumNum::operator!= -- Inequality test for HumNums compared
//   to other HumNums, integers or floats.
//

bool HumNum::operator!=(const HumNum& value) const {
	if (this == &value) {
		return false;
	}
	return getFloat() != value.getFloat();
}


bool HumNum::operator!=(int value) const {
	return getFloat() != value;
}


bool HumNum::operator!=(double value) const {
	return getFloat() != value;
}



//////////////////////////////
//
// HumNum::printFraction -- Print HumNum as a fraction,
//    such as 3/2.  If the HumNum is an integer, then do
//    not print the denominator.
//      default parameter: out = cout;
//

ostream& HumNum::printFraction(ostream& out) const {
	if (this->isInteger()) {
		out << getNumerator();
	} else {
		out << getNumerator() << '/' << getDenominator();
	}
	return out;
}



//////////////////////////////
//
// HumNum::printMixedFraction -- Print as an integer plus fractional
//     remainder.  If absolute value is less than one, will only
//     print the fraction.  The second parameter is the output stream
//     for printing, and the third parameter is a separation string
//     between the integer and remainder fraction.
//        default parameter: out = cout;
//        default parameter: separator = "_"
//

ostream& HumNum::printMixedFraction(ostream& out,
		string separator) const {
	if (this->isInteger()) {
		out << getNumerator();
	} else if (top > bot) {
		int intval = this->getInteger();
		int remainder = top - intval * bot;
		out << intval << separator << remainder << '/' << bot;
	} else {
		printFraction(out);
	}
	return out;
}



//////////////////////////////
//
// HumNum::printList -- Print as a list of two numbers, such as
//    "(1, 2)" for 1/2.
// default value: out = cout;
//

ostream& HumNum::printList(ostream& out) const {
	out << '(' << top << ", " << bot << ')';
	return out;
}



//////////////////////////////
//
// operator<< -- Default printing behavior for HumNums.
//

ostream& operator<<(ostream& out, const HumNum& number) {
	number.printFraction(out);
	return out;
}




//////////////////////////////
//
// HumRegex::HumRegex -- Constructor.
//

HumRegex::HumRegex(void) {
	// by default use ECMAScript regular expression syntax:
	m_regexflags  = std::regex_constants::ECMAScript;

	m_searchflags = std::regex_constants::format_first_only;
}


HumRegex::HumRegex(const string& exp, const string& options) {
	// initialize a regular expression for the object
	m_regexflags = (std::regex_constants::syntax_option_type)0;
	m_regexflags = getTemporaryRegexFlags(options);
	if (m_regexflags == 0) {
		// explicitly set the default syntax
		m_regexflags = std::regex_constants::ECMAScript;
	}
	m_regex = regex(exp, getTemporaryRegexFlags(options));
	m_searchflags = (std::regex_constants::match_flag_type)0;
	m_searchflags = getTemporarySearchFlags(options);
}



//////////////////////////////
//
// HumRegex::HumRegex -- Destructor.
//

HumRegex::~HumRegex() {
	// do nothing
}


///////////////////////////////////////////////////////////////////////////
//
// option setting
//

//////////////////////////////
//
// HumRegex::setIgnoreCase --
//

void HumRegex::setIgnoreCase(void) {
	m_regexflags |= std::regex_constants::icase;
}



//////////////////////////////
//
// HumRegex::getIgnoreCase --
//

bool HumRegex::getIgnoreCase(void) {
	return m_regexflags & std::regex_constants::icase;
}



//////////////////////////////
//
// HumRegex::unsetIgnoreCase --
//

void HumRegex::unsetIgnoreCase(void) {
	m_regexflags &= ~std::regex_constants::icase;
}



//////////////////////////////
//
// HumRegex::setGlobal --
//

void HumRegex::setGlobal(void) {
	m_searchflags &= ~std::regex_constants::format_first_only;
}



//////////////////////////////
//
// HumRegex::getGlobal --
//

bool HumRegex::getGlobal(void) {
	return !(m_searchflags & std::regex_constants::format_first_only);
}



//////////////////////////////
//
// HumRegex::unsetGlobal --
//

void HumRegex::unsetGlobal(void) {
	m_searchflags |= std::regex_constants::format_first_only;
}


///////////////////////////////////////////////////////////////////////////
//
// Searching functions
//

//////////////////////////////
//
// HumRegex::search -- Search for the regular expression in the
//    input string.  Returns true if any matches were found.  Search
//    results can be accessed with .getSubmatchCount() and .getSubmatch(index).
//

bool HumRegex::search(const string& input, const string& exp) {
	m_regex = regex(exp, m_regexflags);
	return regex_search(input, m_matches, m_regex, m_searchflags);
}


bool HumRegex::search(const string& input, int startindex,
		const string& exp) {
	m_regex = regex(exp, m_regexflags);
	auto startit = input.begin() + startindex;
	auto endit   = input.end();
	return regex_search(startit, endit, m_matches, m_regex, m_searchflags);
}


bool HumRegex::search(string* input, const string& exp) {
	return HumRegex::search(*input, exp);
}


bool HumRegex::search(string* input, int startindex, const string& exp) {
	return HumRegex::search(*input, startindex, exp);
}

//
// This version of HumRegex allows for setting the options temporarily.
//

bool HumRegex::search(const string& input, const string& exp,
		const string& options) {
	m_regex = regex(exp, getTemporaryRegexFlags(options));
	return regex_search(input, m_matches, m_regex, getTemporarySearchFlags(options));
}


bool HumRegex::search(const string& input, int startindex, const string& exp,
		const string& options) {
	m_regex = regex(exp, getTemporaryRegexFlags(options));
	auto startit = input.begin() + startindex;
	auto endit   = input.end();
	return regex_search(startit, endit, m_matches, m_regex, getTemporarySearchFlags(options));
}


bool HumRegex::search(string* input, const string& exp,
		const string& options) {
	return HumRegex::search(*input, exp, options);
}


bool HumRegex::search(string* input, int startindex, const string& exp,
		const string& options) {
	return HumRegex::search(*input, startindex, exp, options);
}


///////////////////////////////////////////////////////////////////////////
//
// match-related functions
//

/////////////////////////////
//
// HumRegex::getMatchCount -- Return the number of submatches that a
//   previous call to HumRegex::search generated.
//

int HumRegex::getMatchCount(void) {
	return (int)m_matches.size();
}



//////////////////////////////
//
// HumRegex::getMatch -- Returns the given match.  The first match
//   at "0" is the complete match.  The matches with a larger index
//   are the submatches.
//

string HumRegex::getMatch(int index) {
	return m_matches.str(index);
}



//////////////////////////////
//
// HumRegex::getMatchInt -- Get the match interpreted as a integer.
//

int HumRegex::getMatchInt(int index) {
	string value = m_matches.str(index);
	if (value.size() > 0) {
		return stoi(value);
	} else {
		return 0;
	}
}



//////////////////////////////
//
// HumRegex::getPrefix -- Return the input string text which
//    occurs before the match;
//

string HumRegex::getPrefix(void) {
	return m_matches.prefix().str();
}



//////////////////////////////
//
// HumRegex::getSuffix -- Return the input string text which
//    occurs after the match;
//

string HumRegex::getSuffix(void) {
	return m_matches.suffix().str();
}



//////////////////////////////
//
// HumRegex::getMatchStartIndex -- Get starting index of match in input
//     search string.
//

int HumRegex::getMatchStartIndex(int index) {
	return (int)m_matches.position(index);
}



//////////////////////////////
//
// HumRegex::getMatchEndIndex -- Get ending index of match in input
//     search string.  The index is one larger than the index of the
//     end of the matched position.
//

int HumRegex::getMatchEndIndex(int index) {
	return getMatchStartIndex(index) + getMatchLength(index);
}



//////////////////////////////
//
// HumRegex::getMatchLength -- Get starting character length of match.
//

int HumRegex::getMatchLength(int index) {
	return (int)m_matches.length(index);
}


///////////////////////////////////////////////////////////////////////////
//
// match functions (a "match" is a search that matches a regular 
//    expression to the entire string").
//

//////////////////////////////
//
// HumRegex::match --
//

bool HumRegex::match(const string& input, const string& exp) {
	m_regex = regex(exp, m_regexflags);
	return regex_match(input, m_regex, m_searchflags);
}


bool HumRegex::match(const string& input, const string& exp,
		const string& options) {
	m_regex = regex(exp, getTemporaryRegexFlags(options));
	return regex_match(input, m_regex, getTemporarySearchFlags(options));
}


bool HumRegex::match(const string* input, const string& exp) {
	return HumRegex::match(*input, exp);

}


bool HumRegex::match(const string* input, const string& exp,
		const string& options) {
	return HumRegex::match(*input, exp, options);
}



///////////////////////////////////////////////////////////////////////////
//
// search and replace functions.  Default behavior is to only match
// the first match.  use the "g" option or .setGlobal() to do global
// replacing.
//

//////////////////////////////
//
// HumRegex::replaceDestructive -- Replace in input string.
//

string& HumRegex::replaceDestructive(string& input, const string& replacement,
		const string& exp) {
	m_regex = regex(exp, m_regexflags);
	input = regex_replace(input, m_regex, replacement, m_searchflags);
	return input;
}


string& HumRegex::replaceDestructive(string* input, const string& replacement,
		const string& exp) {
	return HumRegex::replaceDestructive(*input, replacement, exp);
}

//
// This version allows for temporary match flag options.
//

string& HumRegex::replaceDestructive(string& input, const string& replacement,
		const string& exp, const string& options) {
	m_regex = regex(exp, getTemporaryRegexFlags(options));
	input = regex_replace(input, m_regex, replacement, getTemporarySearchFlags(options));
	return input;
}


string& HumRegex::replaceDestructive (string* input, const string& replacement,
		const string& exp, const string& options) {
	return HumRegex::replaceDestructive(*input, replacement, exp, options);
}



//////////////////////////////
//
// HumRegex::replaceCopy --  Keep input string the same, return replacement
//    string as output
//

string HumRegex::replaceCopy(const string& input, const string& replacement,
		const string& exp) {
	m_regex = regex(exp, m_regexflags);
	string output;
	regex_replace(std::back_inserter(output), input.begin(),
			input.end(), m_regex, replacement);
	return output;
}


string HumRegex::replaceCopy(string* input, const string& replacement,
		const string& exp) {
	return HumRegex::replaceCopy(*input, replacement, exp);
}

//
// This version allows for temporary match flag options.
//

string HumRegex::replaceCopy(const string& input, const string& exp,
		const string& replacement, const string& options) {
	m_regex = regex(exp, getTemporaryRegexFlags(options));
	string output;
	regex_replace(std::back_inserter(output), input.begin(),
			input.end(), m_regex, replacement, getTemporarySearchFlags(options));
	return output;
}


string HumRegex::replaceCopy(string* input, const string& exp,
		const string& replacement, const string& options) {
	return HumRegex::replaceCopy(*input, replacement, exp, options);
}



//////////////////////////////
//
// HumRegex::tr --
//

string& HumRegex::tr(string& input, const string& from, const string& to) {
	vector<char> trans;
	trans.resize(256);
	for (int i=0; i<(int)trans.size(); i++) {
		trans[i] = (char)i;
	}
	int minmax = from.size();
	if (to.size() < from.size()) {
		minmax = to.size();
	}
	
	for (int i=0; i<minmax; i++) {
		trans[from[i]] = to[i];
	}

	for (int i=0; i<(int)input.size(); i++) {
		input[i] = trans[input[i]];
	}

	return input;
}



//////////////////////////////
//
// HumRegex::split --
//

bool HumRegex::split(vector<string>& entries, const string& buffer,
		const string& separator) {
	entries.clear();
	string newsep = "(";
	newsep += separator;
	newsep += ")";
	int status = search(buffer, newsep);
	if (!status) {
		if (buffer.size() == 0) {
			return false;
		} else {
			entries.push_back(buffer);
			return true;
		}
	}
	int start = 0;
	while (status) {
		entries.push_back(getPrefix());
		start += getMatchEndIndex(1);
		status = search(buffer, start, newsep);
	}
	// add last token:
	entries.push_back(buffer.substr(start));
	return true;
}



//////////////////////////////
//
// HumRegex::getTemporaryRegexFlags --
//

std::regex_constants::syntax_option_type HumRegex::getTemporaryRegexFlags(
		const string& sflags) {
	if (sflags.empty()) {
		return m_regexflags;
	}
	std::regex_constants::syntax_option_type temp_flags = m_regexflags;
	for (auto it : sflags) {
		switch (it) {
			case 'i':
				temp_flags = (std::regex_constants::syntax_option_type)
						(temp_flags | std::regex_constants::icase);
				break;
			case 'I':
				temp_flags = (std::regex_constants::syntax_option_type)
						(temp_flags & ~std::regex_constants::icase);
				break;
		}
	}
	return temp_flags;
}



//////////////////////////////
//
// HumRegex::getTemporarySearchFlags --
//

std::regex_constants::match_flag_type HumRegex::getTemporarySearchFlags(
		const string& sflags) {
	if (sflags.empty()) {
		return m_searchflags;
	}
	std::regex_constants::match_flag_type temp_flags = m_searchflags;
	for (auto it : sflags) {
		switch (it) {
			case 'g':
				temp_flags = (std::regex_constants::match_flag_type)
						(temp_flags & ~std::regex_constants::format_first_only);
				break;
			case 'G':
				temp_flags = (std::regex_constants::match_flag_type)
						(temp_flags | std::regex_constants::format_first_only);
				break;
		}
	}
	return temp_flags;
}



//////////////////////////////
//
// HumTool::HumTool --
//

HumTool::HumTool(void) {
	// do nothing
}



//////////////////////////////
//
// HumTool::~HumTool --
//

HumTool::~HumTool() {
	// do nothing
}



//////////////////////////////
//
// HumTool::hasAnyText -- Returns true if the output contains
//    text content in Humdrum syntax.
//

bool HumTool::hasAnyText(void) {
	return (m_humdrum_text.rdbuf()->in_avail()
			|| m_free_text.rdbuf()->in_avail()
			|| m_json_text.rdbuf()->in_avail());
}



//////////////////////////////
//
// HumTool::getAllText -- Get the text content from any output
//     streams except warnings and errors.
//

string HumTool::getAllText(void) {
	return  m_humdrum_text.str()
	      + m_json_text.str()
	      + m_free_text.str();
}

//
// ostream version:
//

ostream& HumTool::getAllText(ostream& out) {
	out << m_humdrum_text.str();
	out << m_json_text.str();
	out << m_free_text.str();
	return out;
}



//////////////////////////////
//
// HumTool::hasHumdrumText -- Returns true if the output contains
//    text content in Humdrum syntax.
//

bool HumTool::hasHumdrumText(void) {
	return m_humdrum_text.rdbuf()->in_avail();
}



//////////////////////////////
//
// HumTool::getHumdrumText -- Get the text content which represents
//     Humdrum syntax.
//

string HumTool::getHumdrumText(void) {
	return m_humdrum_text.str();
}

//
// ostream version:
//

ostream& HumTool::getHumdrumText(ostream& out) {
	out << m_humdrum_text.str();
	return out;
}



//////////////////////////////
//
// HumTool::hasFreeText --
//

bool HumTool::hasFreeText(void) {
	return m_free_text.rdbuf()->in_avail();
}



//////////////////////////////
//
// HumTool::getFreeText -- Return any free-form text output from the 
//     tool.
//

string HumTool::getFreeText(void) {
	return m_free_text.str();
}

//
// ostream version:
//

ostream& HumTool::getFreeText(ostream& out) {
	out << m_free_text.str();
	return out;
}



//////////////////////////////
//
// HumTool::hasJsonText --
//

bool HumTool::hasJsonText(void) {
	return m_json_text.rdbuf()->in_avail();
}



//////////////////////////////
//
// HumTool::getFreeText -- Return any JSON text output from the 
//     tool.
//

string HumTool::getJsonText(void) {
	return m_json_text.str();
}

//
// ostream version:
//

ostream& HumTool::getJsonText(ostream& out) {
	out << m_json_text.str();
	return out;
}



//////////////////////////////
//
// HumTool::hasWarning --
//

bool HumTool::hasWarning(void) {
	return m_warning_text.rdbuf()->in_avail();
}



//////////////////////////////
//
// HumTool::getWarning -- Return any warning messages generated when
//     running the tool.
//

string HumTool::getWarning(void) {
	return m_warning_text.str();
}

//
// ostream version:
//

ostream& HumTool::getWarning(ostream& out) {
	out << m_warning_text.str();
	return out;
}



//////////////////////////////
//
// HumTool::hasError -- Return true if there is an error in processing
//    the options or input file(s).
//

bool HumTool::hasError(void) {
	if (hasParseError()) {
		return true;
	}
	return m_error_text.rdbuf()->in_avail();
}



//////////////////////////////
//
// HumTool::getError -- Return any error messages generated when
//     running the tool.   This includes option parsing errors as
//     well.
//

string HumTool::getError(void) {
	string output = getParseError();
	output += m_error_text.str();
	return output;
}

//
// ostream version:
//

ostream& HumTool::getError(ostream& out) {
	out << getParseError();
	out << m_error_text.str();
	return out;
}


//////////////////////////////
//
// HumTool::clearOutput -- clear write buffers to get ready to
//     process another file.
//

void HumTool::clearOutput(void) {
	m_humdrum_text.str("");
	m_json_text.str("");
	m_free_text.str("");
  	m_warning_text.str("");
  	m_error_text.str("");
}



//////////////////////////////
//
// HumdrumFile::HumdrumFile -- HumdrumFile constructor.
//

HumdrumFile::HumdrumFile(void) {
	// do nothing
}

HumdrumFile::HumdrumFile(const string& filename) :
		HUMDRUMFILE_PARENT() {
	read(filename);
}


HumdrumFile::HumdrumFile(istream& contents) :
		HUMDRUMFILE_PARENT() {
	read(contents);
}



//////////////////////////////
//
// HumdrumFile::~HumdrumFile -- HumdrumFile deconstructor.
//

HumdrumFile::~HumdrumFile() {
	// do nothing
}



//////////////////////////////
//
// HumdrumFile::printXml -- Print a HumdrumFile object in XML format.
// default value: level = 0
// default value: indent = tab character
//

ostream& HumdrumFile::printXml(ostream& out, int level,
		const string& indent) {
	out << Convert::repeatString(indent, level) << "<sequence>\n";
	level++;

	out << Convert::repeatString(indent, level) << "<sequenceInfo>\n";
	level++;
	
	out << Convert::repeatString(indent, level) << "<frameCount>";
	out << getLineCount() << "</frameCount>\n";
	

	out << Convert::repeatString(indent, level) << "<tpq>";
	out << tpq() << "</tpq>\n";

	// Starting at 0 by default (single segment only).  Eventually
	// add parameter to set the starting time of the sequence, which
	// would be the duration of all previous segments before this one.
	out << Convert::repeatString(indent, level) << "<sequenceStart";
	out << Convert::getHumNumAttributes(0);
	out << "/>\n";

	out << Convert::repeatString(indent, level) << "<sequenceDuration";
	out << Convert::getHumNumAttributes(getScoreDuration());
	out << "/>\n";

	out << Convert::repeatString(indent, level) << "<trackInfo>\n";
	level++;

	out << Convert::repeatString(indent, level) << "<trackCount>";
	out << getMaxTrack() << "</trackCount>\n";

	for (int i=1; i<=getMaxTrack(); i++) {
		out << Convert::repeatString(indent, level) << "<track";
		out << " n=\"" << i << "\"";
		HumdrumToken* trackstart = getTrackStart(i);
		if (trackstart != NULL) {
			out << " dataType=\"" <<  trackstart->getDataType().substr(2) << "\"";
			out << " startId=\"" <<  trackstart->getXmlId() << "\"";
		}
		HumdrumToken* trackend = getTrackEnd(i, 0);
		if (trackend != NULL) {
			out << " endId =\"" <<  trackend->getXmlId() << "\"";
		}
		out << "/>\n";
	}

	level--;
	out << Convert::repeatString(indent, level) << "<trackInfo>\n";

	printXmlParameterInfo(out, level, "\t");

	level--;
	out << Convert::repeatString(indent, level) << "</sequenceInfo>\n";

	out << Convert::repeatString(indent, level) << "<frames>\n";
	level++;
	for (int i=0; i<getLineCount(); i++) {
		m_lines[i]->printXml(out, level, indent);
	}
	level--;
	out << Convert::repeatString(indent, level) << "</frames>\n";

	level--;
	out << Convert::repeatString(indent, level) << "</sequence>\n";

	return out;
}



//////////////////////////////
//
// HumdrumFile::printXmlParameterInfo -- Print contents of HumHash for HumdrumFile.
// default value: out = cout
// default value: level = 0
// default value: indent = "\t"
//

ostream& HumdrumFile::printXmlParameterInfo(ostream& out, int level,
		const string& indent) {
	((HumHash*)this)->printXml(out, level, indent);
	return out;
}




//////////////////////////////
//
// HumdrumFileBase::getUriToUrlMapping --
//

string HumdrumFileBase::getUriToUrlMapping(const string& uri) {
	auto css = uri.find("://");
	if (css == string::npos) {
		// this is not a URI, so just return input:
		return string(uri);
	}

	if (Convert::startsWith(uri, "http://")) {
		// The URI is a URL, so just copy:
		return string(uri);
	}

	string tag  = uri.substr(0, css);
	string rest = uri.substr(css+3);
	if (rest.empty()) {
		rest = "/";
	}

	// getting a repertory:
	// http://kern.humdrum.org/data?l=osu/classical/bach/inventions
	// getting a single file:
	// http://kern.humdrum.org/data?s=http://kern.humdrum.org/data?s=osu/classical/bach/inventions&file=inven15.krn
	// (Should allow repertory from &s...)
	if ((tag == "humdrum") || (tag == "hum") || (tag == "h")) {
		string testlocation;
		string testfilename;
		int repertoryQ = false;
		auto slash = rest.rfind('/');
		if (slash != string::npos) {
			testlocation = rest.substr(0, slash);
			testfilename = rest.substr(slash+1);
			if (testfilename.find('.') == string::npos) {
				repertoryQ = true;
			}
		} if (slash == string::npos) {
			// no files in root directory, but no reperoties either
			repertoryQ = true;
		}
		string output = "http://";;
		output += "kern.ccarh.org";
		output += "/data?";
		if (repertoryQ) {
			output += "l=";
		} else {
			output += "s=";
		}
		output += rest;
		// probably not needed:
		//output += "&format=kern";
		return output;
	}

	if (tag == "jrp") {
		string output = "http://";
		output += "jrp.ccarh.org";
		output += "/cgi-bin/jrp?a=humdrum";
		output += "&f=";
		output += rest;
		return output;
	}

	// not familiar with the URI, just assume that it is a URL,
	// such as "https://".
	return uri;
}


#ifdef USING_URI

//////////////////////////////
//
// HumdrumFileBase::readFromHumdrumUri -- Read a Humdrum file from an
//      humdrum:// web address
//
// Example:
//    maps: humdrum://osu/classical/haydn/london/sym099a.krn
// into:
//    http://kern.ccarh.org/cgi-bin/ksdata?file=sym099a.krn&l=/osu/classical/haydn/london&format=kern
//

void HumdrumFileBase::readFromHumdrumUri(const string& humaddress) {
	string url = HumdrumFileBase::getUriToUrlMapping(humaddress);
	readFromHttpUri(url);
}



//////////////////////////////
//
// readFromJrpUri -- Read a Humdrum file from a jrp:// web-style address
//
// Example:
// maps:
//    jrp://Jos2721-La_Bernardina
// into:
//    http://jrp.ccarh.org/cgi-bin/jrp?a=humdrum&f=Jos2721-La_Bernardina
//

void HumdrumFileBase::readFromJrpUri(const string& jrpaddress) {
	string url = HumdrumFileBase::getUriToUrlMapping(jrpaddress);
	readFromHttpUri(url);
}


//////////////////////////////
//
// HumdrumFileBase::readFromHttpUri -- download content from the web.
//

void HumdrumFileBase::readFromHttpUri(const string& webaddress) {
	stringstream inputdata;
	readStringFromHttpUri(inputdata, webaddress);
	HumdrumFileBase::readString(inputdata.str());
}



//////////////////////////////
//
// readStringFromHttpUri -- Read a Humdrum file from an http:// web address
//

void HumdrumFileBase::readStringFromHttpUri(stringstream& inputdata,
		const string& webaddress) {
	auto css = webaddress.find("://");
	if (css == string::npos) {
		// give up since URI was not in correct format
	}
	string rest = webaddress.substr(css+3);
	string hostname;
	string location;
	css = rest.find("/");
	if (css != string::npos) {
		hostname = rest.substr(0, css);
		location = rest.substr(css);
	} else {
		hostname = rest;
		location = "/";
	}
	if (location.empty()) {
		location = "/";
	}

	string newline({0x0d, 0x0a});

	stringstream request;
	request << "GET "   << location << " HTTP/1.1" << newline;
	request << "Host: " << hostname << newline;
	request << "User-Agent: HumdrumFile Downloader 2.0 ("
		     << __DATE__ << ")" << newline;
	request << "Connection: close" << newline;  // this line is necessary
	request << newline;

	unsigned short int port = 80;
	int socket_id = open_network_socket(hostname, port);
	if (::write(socket_id, request.str().c_str(), request.str().size()) == -1) {
		exit(-1);
	}

	#define URI_BUFFER_SIZE (10000)
	char buffer[URI_BUFFER_SIZE];
	unsigned int message_len;
	stringstream header;
	int foundcontent   = 0;
	int newlinecounter = 0;
	int i;

	// read the response header:
	while ((message_len = ::read(socket_id, buffer, 1)) != 0) {
		header << buffer[0];
		if ((buffer[0] == 0x0a) || (buffer[0] == 0x0d)) {
					newlinecounter++;
		} else {
					newlinecounter = 0;
		}
		if (newlinecounter == 4) {
			foundcontent = 1;
			break;
		}
	}
	if (foundcontent == 0) {
		cerr << "Funny error trying to read server response" << endl;
		exit(1);
	}

	// now read the size of the rest of the data which is expected
	int datalength = -1;

	// also, check for chunked transfer encoding:

	int chunked = 0;

	header << ends; // necessary?
	while (header.getline(buffer, URI_BUFFER_SIZE)) {
		int len = (int)strlen(buffer);
		for (i=0; i<len; i++) {
			buffer[i] = std::tolower(buffer[i]);
		}
		if (strstr(buffer, "content-length") != NULL) {
			for (i=14; i<len; i++) {
				if (std::isdigit(buffer[i])) {
					sscanf(&buffer[i], "%d", &datalength);
					if (datalength == 0) {
						cerr << "Error: no data found for URI, probably invalid\n";
						cerr << "URL:   " << webaddress << endl;
						exit(1);
					}
					break;
				}
			}
		} else if ((strstr(buffer, "transfer-encoding") != NULL) &&
			(strstr(buffer, "chunked") != NULL)) {
			chunked = 1;
		}
	}

	// once the length of the remaining data is known (or not), read it:
	if (datalength > 0) {
		getFixedDataSize(socket_id, datalength, inputdata, buffer,
				URI_BUFFER_SIZE);

	} else if (chunked) {
		int chunksize;
		int totalsize = 0;
		do {
			chunksize = getChunk(socket_id, inputdata, buffer, URI_BUFFER_SIZE);
			totalsize += chunksize;
		} while (chunksize > 0);
		if (totalsize == 0) {
			cerr << "Error: no data found for URI (probably invalid)\n";
			exit(1);
		}
	} else {
		// if the size of the rest of the data cannot be found in the
		// header, then just keep reading until done (but this will
		// probably cause a 5 second delay at the last read).
		while ((message_len = ::read(socket_id, buffer, URI_BUFFER_SIZE)) != 0) {
			if (foundcontent) {
				inputdata.write(buffer, message_len);
			} else {
				for (i=0; i<(int)message_len; i++) {
					if (foundcontent) {
						inputdata << buffer[i];
					} else {
						header << buffer[i];
						if ((buffer[i] == 0x0a) || (buffer[i] == 0x0d)) {
							newlinecounter++;
						} else {
							newlinecounter = 0;
						}
						if (newlinecounter == 4) {
							foundcontent = 1;
							continue;
						}
					}

				}
			}
		}
	}

	close(socket_id);
}



//////////////////////////////
//
//  HumdrumFileBase::getChunk --
//
// http://en.wikipedia.org/wiki/Chunked_transfer_encoding
// http://tools.ietf.org/html/rfc2616
//
// Chunk Format
//
// If a Transfer-Encoding header with a value of chunked is specified in
// an HTTP message (either a request sent by a client or the response from
// the server), the body of the message is made of an unspecified number
// of chunks ending with a last, zero-sized, chunk.
//
// Each non-empty chunk starts with the number of octets of the data it
// embeds (size written in hexadecimal) followed by a CRLF (carriage
// return and linefeed), and the data itself. The chunk is then closed
// with a CRLF. In some implementations, white space chars (0x20) are
// padded between chunk-size and the CRLF.
//
// The last chunk is a single line, simply made of the chunk-size (0),
// some optional padding white spaces and the terminating CRLF. It is not
// followed by any data, but optional trailers can be sent using the same
// syntax as the message headers.
//
// The message is finally closed by a last CRLF combination.

int HumdrumFileBase::getChunk(int socket_id, stringstream& inputdata,
		char* buffer, int bufsize) {
	int chunksize = 0;
	unsigned int message_len;
	char digit[2] = {0};
	int founddigit = 0;

	// first read the chunk size:
	while ((message_len = ::read(socket_id, buffer, 1)) != 0) {
		if (isxdigit(buffer[0])) {
			digit[0] = buffer[0];
			chunksize = (chunksize << 4) | (int)strtol(digit, NULL, 16);
			founddigit = 1;
		} else if (founddigit) {
			break;
		} // else skipping whitespace before chunksize
	}
	if ((chunksize <= 0) || (message_len == 0)) {
		// next chunk is zero, so no more primary data0:w
		return 0;
	}

	// read the 0x0d and 0x0a characters which are expected (required)
	// after the size of chunk size:
	if (buffer[0] != 0x0d) {
		cerr << "Strange error occurred right after reading a chunk size" << endl;
		exit(1);
	}

	// now expect 0x0a:
	message_len = ::read(socket_id, buffer, 1);
	if ((message_len == 0) || (buffer[0] != 0x0a)) {
		cerr << "Strange error after reading newline at end of chunk size"<< endl;
		exit(1);
	}

	return getFixedDataSize(socket_id, chunksize, inputdata, buffer, bufsize);
}



//////////////////////////////
//
// getFixedDataSize -- read a know amount of data from a socket.
//

int HumdrumFileBase::getFixedDataSize(int socket_id, int datalength,
		stringstream& inputdata, char* buffer, int bufsize) {
	int readcount = 0;
	int readsize;
	int message_len;

	while (readcount < datalength) {
		readsize = bufsize;
		if (readcount + readsize > datalength) {
			readsize = datalength - readcount;
		}
		message_len = (int)::read(socket_id, buffer, readsize);
		if (message_len == 0) {
			// shouldn't happen, but who knows...
			break;
		}
		inputdata.write(buffer, message_len);
		readcount += message_len;
	}

	return readcount;
}



//////////////////////////////
//
// HumdrumFileBase::prepare_address -- Store a computer name, such as
//    www.google.com into a sockaddr_in structure for later use in
//    open_network_socket.
//

void HumdrumFileBase::prepare_address(struct sockaddr_in *address,
		const string& hostname, unsigned short int port) {

	memset(address, 0, sizeof(struct sockaddr_in));
	struct hostent *host_entry;
	host_entry = gethostbyname(hostname.c_str());

	if (host_entry == NULL) {
		cerr << "Could not find address for " << hostname << endl;
		exit(1);
	}

	// copy the address to the sockaddr_in struct.
	memcpy(&address->sin_addr.s_addr, host_entry->h_addr_list[0],
			host_entry->h_length);

	// set the family type (PF_INET)
	address->sin_family = host_entry->h_addrtype;
	address->sin_port = htons(port);
}



//////////////////////////////
//
// open_network_socket -- Open a connection to a computer on the internet.
//    Intended for downloading a Humdrum file from a website.
//

int HumdrumFileBase::open_network_socket(const string& hostname,
		unsigned short int port) {
	int inet_socket;                 // socket descriptor
	struct sockaddr_in servaddr;     // IP/port of the remote host

	prepare_address(&servaddr, hostname, port);

	// socket(domain, type, protocol)
	//    domain   = PF_INET(internet/IPv4 domain)
	//    type     = SOCK_STREAM(tcp) *
	//    protocol = 0 (only one SOCK_STREAM type in the PF_INET domain
	inet_socket = socket(PF_INET, SOCK_STREAM, 0);

	if (inet_socket < 0) {
		// socket returns -1 on error
		cerr << "Error opening socket to computer " << hostname << endl;
		exit(1);
	}
	if (connect(inet_socket, (struct sockaddr *)&servaddr,
			sizeof(struct sockaddr_in)) < 0) {
		// connect returns -1 on error
		cerr << "Error opening connection to coputer: " << hostname << endl;
		exit(1);
	}

	return inet_socket;
}

#endif




//////////////////////////////
//
// HumdrumFileBase::HumdrumFileBase -- HumdrumFileBase constructor.
//

HumdrumFileBase::HumdrumFileBase(void) : HumHash() {
	addToTrackStarts(NULL);
	m_ticksperquarternote = -1;
	m_quietParse = false;
	m_segmentlevel = 0;
}

HumdrumFileBase::HumdrumFileBase(const string& filename) : HumHash() {
	addToTrackStarts(NULL);
	m_ticksperquarternote = -1;
	m_quietParse = false;
	m_segmentlevel = 0;
	read(filename);
}

HumdrumFileBase::HumdrumFileBase(istream& contents) : HumHash() {
	addToTrackStarts(NULL);
	m_ticksperquarternote = -1;
	m_quietParse = false;
	m_segmentlevel = 0;
	read(contents);
}


//
// HumdrumFileStructure::analyzeStructure() needs to be called after
// using the following constructor:
//

HumdrumFileBase::HumdrumFileBase(HumdrumFileBase& infile) {

	m_filename = infile.m_filename;
	m_segmentlevel = infile.m_segmentlevel;
	m_trackstarts.clear();
	m_trackends.clear();
	m_barlines.clear();
	m_ticksperquarternote = infile.m_ticksperquarternote;
	m_idprefix = infile.m_idprefix;
	m_strand1d.clear();
	m_strand2d.clear();
	m_quietParse = infile.m_quietParse;
	m_parseError = infile.m_parseError;
	m_displayError = infile.m_displayError;

	m_lines.resize(infile.m_lines.size());
	for (int i=0; i<(int)m_lines.size(); i++) {
		m_lines[i] = new HumdrumLine(infile.m_lines[i]->getText());
		m_lines[i]->setOwner(this);
	}

	analyzeBaseFromLines();
}



//////////////////////////////
//
// HumdrumFileBase::operator = -- HumdrumFileStructure::analyzeStructure() 
// needs to be called after copying from another HumdrumFile.
//
//

HumdrumFileBase& HumdrumFileBase::operator=(HumdrumFileBase& infile) {
	if (this == &infile) {
		return *this;
	}

	m_filename = infile.m_filename;
	m_segmentlevel = infile.m_segmentlevel;
	m_trackstarts.clear();
	m_trackends.clear();
	m_barlines.clear();
	m_ticksperquarternote = infile.m_ticksperquarternote;
	m_idprefix = infile.m_idprefix;
	m_strand1d.clear();
	m_strand2d.clear();
	m_quietParse = infile.m_quietParse;
	m_parseError = infile.m_parseError;
	m_displayError = infile.m_displayError;

	m_lines.resize(infile.m_lines.size());
	for (int i=0; i<(int)m_lines.size(); i++) {
		m_lines[i] = new HumdrumLine(infile.m_lines[i]->getText());
		m_lines[i]->setOwner(this);
	}

	analyzeBaseFromLines();
	return *this;
}



//////////////////////////////
//
// HumdrumFileBase::~HumdrumFileBase -- HumdrumFileBase deconstructor.
//

HumdrumFileBase::~HumdrumFileBase() {
	clear();
}



//////////////////////////////
//
// HumdrumFileBase::clear -- Reset the contents of a file to be empty.
//

void HumdrumFileBase::clear(void) {
	// delete memory allocation:
	for (int i=0; i<(int)m_lines.size(); i++) {
		if (m_lines[i] != NULL) {
			delete m_lines[i];
			m_lines[i] = NULL;
		}
	}
	m_lines.clear();

	// clear state variables which are now invalid:
	m_trackstarts.clear();
	m_trackends.clear();
	m_barlines.clear();
	m_ticksperquarternote = -1;
	m_idprefix.clear();
	m_strand1d.clear();
	m_strand2d.clear();
	m_filename.clear();
	m_segmentlevel = 0;
}



//////////////////////////////
//
// HumdrumFileBase::setXmlIdPrefix -- Set the prefix for a HumdrumXML ID
//     atrribute.  The prefix should not start with a digit, nor have
//     spaces in it.
//

void HumdrumFileBase::setXmlIdPrefix(const string& value) {
	m_idprefix = value;
}



//////////////////////////////
//
// HumdrumFileBase::getXmlIdPrefix -- Return the HumdrumXML ID attribute prefix.
//

string HumdrumFileBase::getXmlIdPrefix(void) {
	return m_idprefix;
}



//////////////////////////////
//
// HumdrumFileBase::operator[] -- Access a Humdrum file line by and index.
//    Negative values reference the end of the list of lines.
//

HumdrumLine& HumdrumFileBase::operator[](int index) {
	if (index < 0) {
		index = (int)m_lines.size() - index;
	}
	if ((index < 0) || (index >= (int)m_lines.size())) {
		cerr << "Error: invalid index: " << index << endl;
		index = (int)m_lines.size()-1;
	}
	return *m_lines[index];
}



//////////////////////////////
//
// HumdrumFileBase::setParseError -- Set an error message from parsing
//     input data.  The size of the message will keep track of whether
//     or not an error was generated.  If no error message is generated
//     when reading data, then the parsing of the data is assumed to be
//     good.
//

bool HumdrumFileBase::setParseError(const string& err) {
	m_parseError = err;
	return !m_parseError.size();
}


bool HumdrumFileBase::setParseError(stringstream& err) {
	m_parseError = err.str();
	return !m_parseError.size();
}


bool HumdrumFileBase::setParseError(const char* format, ...) {
	char buffer[1024] = {0};
	va_list ap;
	va_start(ap, format);
	snprintf(buffer, 1024, format, ap);
	va_end(ap);
	m_parseError = buffer;
	return !m_parseError.size();
}



//////////////////////////////
//
// HumdrumFileBase::read -- Load file contents from an input stream or file.
//

bool HumdrumFileBase::read(const string& filename) {
	m_displayError = true;
	return HumdrumFileBase::read(filename.c_str());
}


bool HumdrumFileBase::read(const char* filename) {
	string fname = filename;
	m_displayError = true;

#ifdef USING_URI
	if (fname.find("://") != string::npos) {
		if (Convert::startsWith(fname, "http://")) {
			readFromHttpUri(fname);
			return isValid();
		}
		if (Convert::startsWith(fname, "jrp://")) {
			readFromJrpUri(fname);
			return isValid();
		}
		if (Convert::startsWith(fname, "h://") ||
			Convert::startsWith(fname, "hum://") ||
			Convert::startsWith(fname, "humdrum://")) {
			readFromHumdrumUri(fname);
			return isValid();
		}
	}
#endif

	ifstream infile;
	if (fname.empty() || (fname ==  "-")) {
		return HumdrumFileBase::read(cin);
	} else {
		infile.open(filename);
		if (!infile.is_open()) {
			return setParseError("Cannot open file %s for reading.", filename);
		}
	}
	HumdrumFileBase::read(infile);
	infile.close();
	return isValid();
}


bool HumdrumFileBase::read(istream& contents) {
	clear();
	m_displayError = true;
	char buffer[123123] = {0};
	HumdrumLine* s;
	while (contents.getline(buffer, sizeof(buffer), '\n')) {
		s = new HumdrumLine(buffer);
		s->setOwner(this);
		m_lines.push_back(s);
	}
	return analyzeBaseFromLines();
/*
	if (!analyzeTokens()) { return isValid(); }
	if (!analyzeLines() ) { return isValid(); }
	if (!analyzeSpines()) { return isValid(); }
	if (!analyzeLinks() ) { return isValid(); }
	if (!analyzeTracks()) { return isValid(); }
	return isValid();
*/
}



//////////////////////////////
//
// HumdrumFileBase::readCsv -- Read a Humdrum file in CSV format
//    (rather than TSV format).
// default value: separator = ","
//

bool HumdrumFileBase::readCsv(const string& filename, const string& separator) {
	return HumdrumFileBase::readCsv(filename.c_str());
}


bool HumdrumFileBase::readCsv(const char* filename, const string& separator) {
	ifstream infile;
	if ((strlen(filename) == 0) || (strcmp(filename, "-") == 0)) {
		return HumdrumFileBase::readCsv(cin, separator);
	} else {
		infile.open(filename);
		if (!infile.is_open()) {
			return setParseError("Cannot open file %s for reading.", filename);
		}
	}
	HumdrumFileBase::readCsv(infile, separator);
	infile.close();
	return isValid();
}


bool HumdrumFileBase::readCsv(istream& contents, const string& separator) {
	m_displayError = true;
	char buffer[123123] = {0};
	HumdrumLine* s;
	while (contents.getline(buffer, sizeof(buffer), '\n')) {
		s = new HumdrumLine;
		s->setLineFromCsv(buffer);
		s->setOwner(this);
		m_lines.push_back(s);
	}
	return analyzeBaseFromLines();
}



//////////////////////////////
//
// HumdrumFileBase::analyzeBaseFromLines --
//

bool HumdrumFileBase::analyzeBaseFromLines(void)  {
	if (!analyzeTokens()) { return isValid(); }
	if (!analyzeLines() ) { return isValid(); }
	if (!analyzeSpines()) { return isValid(); }
	if (!analyzeLinks() ) { return isValid(); }
	if (!analyzeTracks()) { return isValid(); }
	return isValid();
}



//////////////////////////////
//
// HumdrumFileBase::readString -- Read contents from a string rather than
//    an istream or filename.
//

bool HumdrumFileBase::readString(const string& contents) {
	stringstream infile;
	infile << contents;
	int status = read(infile);
	return status;
}


bool HumdrumFileBase::readString(const char* contents) {
	stringstream infile;
	infile << contents;
	return read(infile);
}



//////////////////////////////
//
// HumdrumFileBase::readStringCsv -- Reads Humdrum data in CSV format.
//

bool HumdrumFileBase::readStringCsv(const char* contents,
		const string& separator) {
	stringstream infile;
	infile << contents;
	return readCsv(infile, separator);
}


bool HumdrumFileBase::readStringCsv(const string& contents,
		const string& separator) {
	stringstream infile;
	infile << contents;
	return readCsv(infile, separator);
}



//////////////////////////////
//
// HumdrumFileBase::getParseError -- Return parse fail reason.
//

string HumdrumFileBase::getParseError(void) const {
	return m_parseError;
}



//////////////////////////////
//
// HumdrumFileBase::isValid -- Returns true if last read was
//     successful.
//

bool HumdrumFileBase::isValid(void) {
	if (m_displayError && (m_parseError.size() > 0)&& !isQuiet()) {
		cerr << m_parseError << endl;
		m_displayError = false;
	}
	return m_parseError.empty();
}



//////////////////////////////
//
// HumdrumFileBase::setQuietParsing -- Prevent error messages from
//   being displayed when reading data.
// @SEEALSO: setNoisyParsing
// @SEEALSO: isQuiet
//

void HumdrumFileBase::setQuietParsing(void) {
	m_quietParse = true;
}



//////////////////////////////
//
// HumdrumFileBase::setFilename --
//

void HumdrumFileBase::setFilename(const string& filename) {
	m_filename = filename;
}



//////////////////////////////
//
// HumdrumFileBase::getFilename --
//

string HumdrumFileBase::getFilename(void) {
	return m_filename;
}



//////////////////////////////
//
// HumdrumFileBase::printSegmentLabel --
//

ostream& HumdrumFileBase::printSegmentLabel(ostream& out) {
	out << "!!!!SEGMENT";
	string filename = getFilename();
	int segment = getSegmentLevel();
	if (segment != 0) {
		if (segment < 0) {
			out << segment;
		} else {
			out << "+" << segment;
		}
	}
	out << ": " << filename << endl;
	return out;
}



//////////////////////////////
//
// HumdrumFileBase::printNonemptySegmentLabel --
//

ostream& HumdrumFileBase::printNonemptySegmentLabel(ostream& out) {
	if (getFilename().size() > 0) {
		printSegmentLabel(out);
	}
	return out;
}



//////////////////////////////
//
// HumdrumFileBase::getSegmentLevel -- return the segment level
//

int HumdrumFileBase::getSegmentLevel(void) {
	return m_segmentlevel;
}



//////////////////////////////
//
// HumdrumFileBase::setSegmentLevel -- return the segment level
//

void HumdrumFileBase::setSegmentLevel(int level) {
	m_segmentlevel = level;
}

//////////////////////////////
//
// HumdrumFileBase::setNoisyParsing -- Display error messages
//   on console when reading data.
// @SEEALSO: setQuietParsing
// @SEEALSO: isQuiet
//

void HumdrumFileBase::setNoisyParsing(void) {
	m_quietParse = false;
}



//////////////////////////////
//
// HumdrmFileBase::isQuiet -- Returns true if parsing errors
//    messages should be suppressed. By default the parsing
//    is "noisy" and the error messages will be printed to
//    standard error.
// @SEEALSO: setQuietParsing
// @SEEALSO: setNoisyParsing
//

bool HumdrumFileBase::isQuiet(void) const{
	return m_quietParse;
}



//////////////////////////////
//
// HumdrumFileBase::printCsv -- print Humdrum file content in
//     CSV format.
// default value: out = std::cout
// default value: separator = ","
//

ostream& HumdrumFileBase::printCsv(ostream& out,
		const string& separator) {
	for (int i=0; i<getLineCount(); i++) {
		((*this)[i]).printCsv(out, separator);
	}
	return out;
}



//////////////////////////////
//
// HumdrumFileBase::printFieldNumber --
//

ostream& HumdrumFileBase::printFieldNumber(int fieldnum, ostream& out) {
	return printFieldIndex(fieldnum - 1, out);
}



//////////////////////////////
//
// HumdrumFileBase::printFieldIndex --
//

ostream& HumdrumFileBase::printFieldIndex(int fieldind, ostream& out) {
	if (fieldind < 0) {
		return out;
	}
	HumdrumFileBase& infile = *this;
	for (int i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].hasSpines()) {
			out << infile[i] << endl;
			continue;
		}
		cout << infile.token(i,fieldind) << endl;
	}
	return out;
}



//////////////////////////////
//
// HumdrumFileBase::getLine -- Return a pointer to the line at a
//     given index in the data storage.
//

HumdrumLine* HumdrumFileBase::getLine(int index) {
	if (index < 0) {
		return NULL;
	} else if (index >= (int)m_lines.size()) {
		return NULL;
	} else {
		return m_lines[index];
	}
}



//////////////////////////////
//
// HumdrumFileBase::analyzeTokens -- Generate token array from
//    current contents of the lines.  If either tokens or the line
//    is changed, then the other state becomes invalid.
//    See createLinesFromTokens for regeneration of lines from tokens.
//

bool HumdrumFileBase::analyzeTokens(void) {
	int i;
	for (i=0; i<(int)m_lines.size(); i++) {
		m_lines[i]->createTokensFromLine();
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileBase::createLinesFromTokens -- Generate Humdrum lines strings
//   from the stored list of tokens.
//

void HumdrumFileBase::createLinesFromTokens(void) {
	for (int i=0; i<(int)m_lines.size(); i++) {
		m_lines[i]->createLineFromTokens();
	}
}



////////////////////////////
//
// HumdrumFileBase::appendLine -- Add a line to the file's contents.  The file's
//    spine and rhythmic structure should be recalculated after an append.
//

void HumdrumFileBase::appendLine(const char* line) {
	HumdrumLine* s = new HumdrumLine(line);
	m_lines.push_back(s);
}


void HumdrumFileBase::appendLine(const string& line) {
	HumdrumLine* s = new HumdrumLine(line);
	m_lines.push_back(s);
}


void HumdrumFileBase::appendLine(HumdrumLine* line) {
	// deletion will be handled by class.
	m_lines.push_back(line);
}



////////////////////////////
//
// HumdrumFileBase::appendLine -- Add a line to the file's contents.  The file's
//    spine and rhythmic structure should be recalculated after an append.
//


void HumdrumFileBase::insertLine(int index, const char* line) {
	HumdrumLine* s = new HumdrumLine(line);
	m_lines.insert(m_lines.begin() + index, s);
}


void HumdrumFileBase::insertLine(int index, const string& line) {
	HumdrumLine* s = new HumdrumLine(line);
	m_lines.insert(m_lines.begin() + index, s);
}


void HumdrumFileBase::insertLine(int index, HumdrumLine* line) {
	// deletion will be handled by class.
	m_lines.insert(m_lines.begin() + index, line);
}



//////////////////////////////
//
// HumdrumFileBase::back --
//

HumdrumLine* HumdrumFileBase::back(void) {
	return m_lines.back();
}



//////////////////////////////
//
// HumdrumFileBase::getReferenceRecords --
//

vector<HumdrumLine*> HumdrumFileBase::getReferenceRecords(void) {
	vector<HumdrumLine*> hlps;
	hlps.reserve(32);
	HumdrumLine* hlp;
	auto& infile = *this;
	for (int i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isReference()) {
			hlp = &infile[i];
			hlps.push_back(hlp);
		}
	}
	return hlps;
}



////////////////////////////
//
// HumdrumFileBase::getLineCount -- Returns the number of lines.
//

int HumdrumFileBase::getLineCount(void) const {
	return (int)m_lines.size();
}



//////////////////////////////
//
// HumdrumFileBase::token -- Return the token at the given line/field index.
//

HTp HumdrumFileBase::token(int lineindex, int fieldindex) {
	if (lineindex < 0) {
		lineindex += getLineCount();
	}
	return m_lines[lineindex]->token(fieldindex);
}


//
// Special case that returns a subtoken string:
//   default value separator = " "
//

string HumdrumFileBase::token(int lineindex, int fieldindex,
		int subtokenindex, const string& separator) {
	return token(lineindex, fieldindex)->getSubtoken(subtokenindex, separator);
}



//////////////////////////////
//
// HumdrumFileBase::getMaxTrack -- Returns the number of primary
//     spines in the data.
//

int HumdrumFileBase::getMaxTrack(void) const {
	return (int)m_trackstarts.size() - 1;
}



//////////////////////////////
//
// HumdrumFileBase::printSpineInfo -- Print the spine information for all
//    lines/tokens in file (for debugging).
//

ostream& HumdrumFileBase::printSpineInfo(ostream& out) {
	for (int i=0; i<getLineCount(); i++) {
		m_lines[i]->printSpineInfo(out) << '\n';
	}
	return out;
}



//////////////////////////////
//
// HumdrumFileBase::printDataTypeInfo -- Print the data type for all
//     spines in the file (for debugging).
//

ostream& HumdrumFileBase::printDataTypeInfo(ostream& out) {
	for (int i=0; i<getLineCount(); i++) {
		m_lines[i]->printDataTypeInfo(out) << '\n';
	}
	return out;
}



//////////////////////////////
//
// HumdrumFileBase::printTrackInfo -- Print the track numbers for all
//     tokens in the file (for debugging).
//

ostream& HumdrumFileBase::printTrackInfo(ostream& out) {
	for (int i=0; i<getLineCount(); i++) {
		m_lines[i]->printTrackInfo(out) << '\n';
	}
	return out;
}



//////////////////////////////
//
// HumdrumFileBase::getSpineStartList -- Return a list of the exclustive
//     interpretations starting spines in the data.  The single parameter
//     version of the fuction returns all starting exclusive interpretations.
//     The two-parameter version will result all exclusive interpretations
//     of a given datatype, and the three-parameter version where the third
//     parameter is a vector of string, will selectively include all starting
//     tokens which match one of the data types in the input list.  The
//     trackstarts class variable contains an empty slot at index 0;
//     this is removed in the return vector.
//

void HumdrumFileBase::getSpineStartList(vector<HTp>& spinestarts) {
	spinestarts.reserve(m_trackstarts.size());
	spinestarts.resize(0);
	for (int i=1; i<(int)m_trackstarts.size(); i++) {
		spinestarts.push_back(m_trackstarts[i]);
	}
}


void HumdrumFileBase::getSpineStartList(vector<HTp>& spinestarts,
		const string& exinterp) {
	spinestarts.reserve(m_trackstarts.size());
	spinestarts.resize(0);
	for (int i=1; i<(int)m_trackstarts.size(); i++) {
		if (exinterp == *m_trackstarts[i]) {
			spinestarts.push_back(m_trackstarts[i]);
		}
	}
}


void HumdrumFileBase::getSpineStartList(vector<HTp>& spinestarts,
		const vector<string>& exinterps) {
	spinestarts.reserve(m_trackstarts.size());
	spinestarts.resize(0);
	for (int i=1; i<(int)m_trackstarts.size(); i++) {
		for (int j=0; j<(int)exinterps.size(); j++) {
			if (exinterps[j] == *m_trackstarts[i]) {
				spinestarts.push_back(m_trackstarts[i]);
			}
		}
	}
}


void HumdrumFileBase::getKernSpineStartList(vector<HTp>& spinestarts) {
	getSpineStartList(spinestarts, "**kern");
}

vector<HTp> HumdrumFileBase::getKernSpineStartList(void) {
	vector<HTp> starts;
	HumdrumFileBase::getKernSpineStartList(starts);
	return starts;
}



//////////////////////////////
//
// getPrimaryspineSequence -- Return a list of the HumdrumTokens in a spine,
//    but not any secondary spine content if the spine splits.
//


void HumdrumFileBase::getPrimarySpineSequence(vector<HTp>& sequence, int spine,
		int options) {
	getPrimaryTrackSequence(sequence, spine+1, options);
}



//////////////////////////////
//
// getPrimaryspineSequence -- Return a list of the HumdrumTokens in a spine,
//    but not any secondary spine content if the spine splits.
//


void HumdrumFileBase::getSpineSequence(vector<vector<HTp> >& sequence,
		HTp starttoken, int options) {
	getTrackSequence(sequence, starttoken, options);

}


void HumdrumFileBase::getSpineSequence(vector<vector<HTp> >& sequence,
		int spine, int options) {
	getTrackSequence(sequence, spine+1, options);
}



//////////////////////////////
//
// HumdrumFileBase::getPrimaryTrackSequence -- Return a list of the
//     given primary spine tokens for a given track (indexed starting at
//     one and going through getMaxTrack().
//

void HumdrumFileBase::getPrimaryTrackSequence(vector<HTp>& sequence, int track,
		int options) {
	vector<vector<HTp> > tempseq;
	getTrackSequence(tempseq, track, options | OPT_PRIMARY);
	sequence.resize(tempseq.size());
	for (int i=0; i<(int)tempseq.size(); i++) {
		sequence[i] = tempseq[i][0];
	}
}



/////////////////////////////
//
// HumdrumFileBase::getTrackSequence -- Extract a sequence of tokens
//    for the given spine.  All subspine tokens will be included.
//    See getPrimaryTrackSequence() if you only want the first subspine for
//    a track on all lines.
//
// The following options are used for the getPrimaryTrackTokens:
// * OPT_PRIMARY    => only extract primary subspine/subtrack.
// * OPT_NOEMPTY    => don't include null tokens in extracted list if all
//                        extracted subspines contains null tokens.
//                        Includes null interpretations and comments as well.
// * OPT_NONULL     => don't include any null tokens in extracted list.
// * OPT_NOINTERP   => don't include interprtation tokens.
// * OPT_NOMANIP    => don't include spine manipulators (*^, *v, *x, *+,
//                        but still keep ** and *0).
// * OPT_NOCOMMENT  => don't include comment tokens.
// * OPT_NOGLOBAL   => don't include global records (global comments, reference
//                        records, and empty lines). In other words, only return
//                        a list of tokens from lines which hasSpines() it true.
// * OPT_NOREST     => don't include **kern rests.
// * OPT_NOTIE      => don't include **kern secondary tied notes.
// Compound options:
// * OPT_DATA      (OPT_NOMANIP | OPT_NOCOMMENT | OPT_NOGLOBAL)
//     Only data tokens (including barlines)
// * OPT_ATTACKS   (OPT_DATA | OPT_NOREST | OPT_NOTIE | OPT_NONULL)
//     Only note-attack tokens (when etracting **kern data)
//

void HumdrumFileBase::getTrackSequence(vector<vector<HTp> >& sequence,
		HTp starttoken, int options) {
	int track = starttoken->getTrack();
	getTrackSequence(sequence, track, options);
}


void HumdrumFileBase::getTrackSequence(vector<vector<HTp> >& sequence,
		int track, int options) {
	bool primaryQ   = options & OPT_PRIMARY;
	bool nonullQ    = options & OPT_NONULL;
	bool noemptyQ   = options & OPT_NOEMPTY;
	bool nointerpQ  = options & OPT_NOINTERP;
	bool nomanipQ   = options & OPT_NOMANIP;
	bool nocommentQ = options & OPT_NOCOMMENT;
	bool noglobalQ  = options & OPT_NOGLOBAL;
	bool norestQ    = options & OPT_NOREST;
	bool notieQ     = options & OPT_NOTIE;

	vector<vector<HTp> >& output = sequence;
	output.reserve(getLineCount());
	output.resize(0);

	vector<HTp> tempout;
	auto& infile = *this;
	int i, j;
	bool allNull;
	HTp token;
	bool foundTrack;

	for (i=0; i<infile.getLineCount(); i++) {
		tempout.resize(0);
		if (!noglobalQ && (infile[i].isGlobal())) {
			tempout.push_back(infile[i].token(0));
			output.push_back(tempout);
			continue;
		}
		if (noemptyQ) {
			allNull = true;
			for (j=0; j<infile[i].getFieldCount(); j++) {
				if (infile[i].token(j)->getTrack() != track) {
					continue;
				}
				if (!infile[i].token(j)->isNull()) {
					allNull = false;
					break;
				}
			}
			if (allNull) {
				continue;
			}
		}

		foundTrack = false;
		for (j=0; j<infile[i].getFieldCount(); j++) {
			token = infile[i].token(j);
			if (token->getTrack() != track) {
				continue;
			}
			if (primaryQ && foundTrack) {
				continue;
			}
			foundTrack = true;
			if (nointerpQ && (infile[i].token(j)->isManipulator() ||
					infile[i].token(j)->isTerminator() ||
					infile[i].token(j)->isExclusive())) {
				continue;
			}
			if (nomanipQ && infile[i].token(j)->isManipulator()) {
				continue;
			}
			if (nonullQ && infile[i].token(j)->isNull()) {
				continue;
			}
			if (nocommentQ && infile[i].token(j)->isComment()) {
				continue;
			}
			if (norestQ && infile[i].token(j)->isRest()) {
				continue;
			}
			if (notieQ && infile[i].token(j)->isSecondaryTiedNote()) {
				continue;
			}

			tempout.push_back(infile[i].token(j));
		}
		if (tempout.size() > 0) {
			output.push_back(tempout);
		}
	}
}



//////////////////////////////
//
// HumdrumFileBase::getTrackStart -- Return the starting exclusive
//     interpretation for the given track.  Returns NULL if the track
//     number is out of range.
//

HTp HumdrumFileBase::getTrackStart(int track) const {
	if ((track > 0) && (track < (int)m_trackstarts.size())) {
		return m_trackstarts[track];
	} else {
		return NULL;
	}
}



//////////////////////////////
//
// HumdrumFileBase::getTrackEndCount -- Return the number of ending tokens
//    for the given track.  Spines must start as a single exclusive
//    interpretation token.  However, since spines may split and merge,
//    it is possible that there are more than one termination points for a
//    track.  This function returns the number of terminations which are
//    present in a file for any given spine/track.
//

int HumdrumFileBase::getTrackEndCount(int track) const {
	if (track < 0) {
		track += (int)m_trackends.size();
	}
	if (track < 0) {
		return 0;
	}
	if (track >= (int)m_trackends.size()) {
		return 0;
	}
	return (int)m_trackends[track].size();
}



//////////////////////////////
//
// HumdrumFileBase::getTrackEnd -- Returns a pointer to the terminal manipulator
//    token for the given track and subtrack.  Sub-tracks are indexed from 0 up
//    to but not including getTrackEndCount.
//

HTp HumdrumFileBase::getTrackEnd(int track, int subtrack) const {
	if (track < 0) {
		track += (int)m_trackends.size();
	}
	if (track < 0) {
		return NULL;
	}
	if (track >= (int)m_trackends.size()) {
		return NULL;
	}
	if (subtrack < 0) {
		subtrack += (int)m_trackends[track].size();
	}
	if (subtrack < 0) {
		return NULL;
	}
	if (subtrack >= (int)m_trackends[track].size()) {
		return NULL;
	}
	return m_trackends[track][subtrack];
}



//////////////////////////////
//
// HumdrumFileBase::analyzeLines -- Store a line's index number in the
//    HumdrumFile within the HumdrumLine object at that index.
//    Returns false if there was an error.
//

bool HumdrumFileBase::analyzeLines(void) {
	for (int i=0; i<(int)m_lines.size(); i++) {
		m_lines[i]->setLineIndex(i);
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileBase::analyzeTracks -- Analyze the track structure of the
//     data.  Returns false if there was a parse error.
//

bool HumdrumFileBase::analyzeTracks(void) {
	for (int i=0; i<(int)m_lines.size(); i++) {
		int status = m_lines[i]->analyzeTracks(m_parseError);
		if (!status) {
			return false;
		}
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileBase::analyzeLinks -- Generate forward and backwards spine links
//    for each token.
//

bool HumdrumFileBase::analyzeLinks(void) {
	HumdrumLine* next     = NULL;
	HumdrumLine* previous = NULL;

	for (int i=0; i<(int)m_lines.size(); i++) {
		if (!m_lines[i]->hasSpines()) {
			continue;
		}
		previous = next;
		next = m_lines[i];
		if (previous != NULL) {
			if (!stitchLinesTogether(*previous, *next)) {
				return isValid();
			}
		}
	}
	return isValid();;
}



//////////////////////////////
//
// HumdrumFileBase::stitchLinesTogether -- Make forward/backward links for
//    tokens on each line.
//

bool HumdrumFileBase::stitchLinesTogether(HumdrumLine& previous,
		HumdrumLine& next) {
	int i;

	// first handle simple cases where the spine assignments are one-to-one:
	if (!previous.isInterpretation() && !next.isInterpretation()) {
		if (previous.getTokenCount() != next.getTokenCount()) {
			stringstream err;
			err << "Error lines " << (previous.getLineNumber())
			    << " and " << (next.getLineNumber()) << " not same length\n";
			err << "Line " << (previous.getLineNumber()) << ": "
			    << previous << endl;
			err << "Line " << (next.getLineNumber()) << ": "
			    << next;
			return setParseError(err);
		}
		for (i=0; i<previous.getTokenCount(); i++) {
			if (next.token(i)) {
				previous.token(i)->makeForwardLink(*next.token(i));
			} else {
				cerr << "Strange error 1" << endl;
			}
		}
		return true;
	}
	int ii = 0;
	for (i=0; i<previous.getTokenCount(); i++) {
		if (!previous.token(i)->isManipulator()) {
			if (next.token(ii) != NULL) {
				previous.token(i)->makeForwardLink(*next.token(ii++));
			} else {
				cerr << "Strange error 2" << endl;
			}
		} else if (previous.token(i)->isSplitInterpretation()) {
			// connect the previous token to the next two tokens.
			if (next.token(ii) != NULL) {
				previous.token(i)->makeForwardLink(*next.token(ii++));
			} else {
				cerr << "Strange error 3" << endl;
			}
			if (next.token(ii) != NULL) {
				previous.token(i)->makeForwardLink(*next.token(ii++));
			} else {
				cerr << "Strange error 4" << endl;
			}
		} else if (previous.token(i)->isMergeInterpretation()) {
			// connect multiple previous tokens which are adjacent *v
			// spine manipulators to the current next token.
			while ((i<previous.getTokenCount()) &&
					previous.token(i)->isMergeInterpretation()) {
				if (next.token(ii) != NULL) {
					previous.token(i)->makeForwardLink(*next.token(ii));
				} else {
					cerr << "Strange error 5" << endl;
				}
				i++;
			}
			i--;
			ii++;
		} else if (previous.token(i)->isExchangeInterpretation()) {
			// swapping the order of two spines.
			if ((i<previous.getTokenCount()) &&
					previous.token(i+1)->isExchangeInterpretation()) {
				if (next.token(ii) != NULL) {
					previous.token(i+1)->makeForwardLink(*next.token(ii++));
				} else {
					cerr << "Strange error 6" << endl;
				}
				if (next.token(ii) != NULL) {
					previous.token(i)->makeForwardLink(*next.token(ii++));
				} else {
					cerr << "Strange error 7" << endl;
				}
			}
			i++;
		} else if (previous.token(i)->isTerminateInterpretation()) {
			// No link should be made.  There may be a problem if a
			// new segment is given (this should be handled by a
			// HumdrumSet class, not HumdrumFileBase.
		} else if (previous.token(i)->isAddInterpretation()) {
			// A new data stream is being added, the next linked token
			// should be an exclusive interpretation.
			if (!next.token(ii+1)->isExclusiveInterpretation()) {
				stringstream err;
				err << "Error: expecting exclusive interpretation on line "
				    << next.getLineNumber() << " at token " << i << " but got "
				    << next.token(i);
				return setParseError(err);
			}
			if (next.token(ii) != NULL) {
				previous.token(i)->makeForwardLink(*next.token(ii++));
			} else {
				cerr << "Strange error 8" << endl;
			}
			ii++;
		} else if (previous.token(i)->isExclusiveInterpretation()) {
			if (next.token(ii) != NULL) {
				if (previous.token(i) != NULL) {
					previous.token(i)->makeForwardLink(*next.token(ii++));
				} else {
					cerr << "Strange error 10" << endl;
				}
			} else {
				cerr << "Strange error 9" << endl;
			}
		} else {
			return setParseError("Error: should not get here");
		}
	}

	if ((i != previous.getTokenCount()) || (ii != next.getTokenCount())) {
		stringstream err;
		err << "Error: cannot stitch lines together due to alignment problem\n";
		err << "Line " << previous.getLineNumber() << ": "
		    << previous << endl;
		err << "Line " << next.getLineNumber() << ": "
		    << next << endl;
		err << "I = " <<i<< " token count " << previous.getTokenCount() << endl;
		err << "II = " <<ii<< " token count " << next.getTokenCount();
		return setParseError(err);
	}

	return isValid();
}



//////////////////////////////
//
// HumdrumFileBase::analyzeSpines -- Analyze the spine structure of the
//     data.  Returns false if there was a parse error.
//

bool HumdrumFileBase::analyzeSpines(void) {
	vector<string> datatype;
	vector<string> sinfo;
	vector<vector<HTp> > lastspine;
	m_trackstarts.resize(0);
	m_trackends.resize(0);
	addToTrackStarts(NULL);

	bool init = false;
	int i, j;
	for (i=0; i<getLineCount(); i++) {
		if (!m_lines[i]->hasSpines()) {
			m_lines[i]->token(0)->setFieldIndex(0);
			continue;
		}
		if ((init == false) && !m_lines[i]->isExclusive()) {
			stringstream err;
			err << "Error on line: " << (i+1) << ':' << endl;
			err << "   Data found before exclusive interpretation" << endl;
			err << "   LINE: " << *m_lines[i];
			return setParseError(err);
		}
		if ((init == false) && m_lines[i]->isExclusive()) {
			// first line of data in file.
			init = true;
			datatype.resize(m_lines[i]->getTokenCount());
			sinfo.resize(m_lines[i]->getTokenCount());
			lastspine.resize(m_lines[i]->getTokenCount());
			for (j=0; j<m_lines[i]->getTokenCount(); j++) {
				datatype[j] = m_lines[i]->getTokenString(j);
				addToTrackStarts(m_lines[i]->token(j));
				sinfo[j]    = to_string(j+1);
				m_lines[i]->token(j)->setSpineInfo(sinfo[j]);
				m_lines[i]->token(j)->setFieldIndex(j);
				lastspine[j].push_back(m_lines[i]->token(j));
			}
			continue;
		}
		if ((int)datatype.size() != m_lines[i]->getTokenCount()) {
			stringstream err;
			err << "Error on line " << (i+1) << ':' << endl;
			err << "   Expected " << datatype.size() << " fields,"
			     << " but found " << m_lines[i]->getTokenCount();
			return setParseError(err);
		}
		for (j=0; j<m_lines[i]->getTokenCount(); j++) {
			m_lines[i]->token(j)->setSpineInfo(sinfo[j]);
			m_lines[i]->token(j)->setFieldIndex(j);
		}
		if (!m_lines[i]->isManipulator()) {
			continue;
		}
		if (!adjustSpines(*m_lines[i], datatype, sinfo)) { return isValid(); }
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileBase::addToTrackStarts -- A starting exclusive interpretation was
//    found, so store in the list of track starts.  The first index position
//    in trackstarts is reserve for non-spine usage.
//

void HumdrumFileBase::addToTrackStarts(HTp token) {
	if (token == NULL) {
		m_trackstarts.push_back(NULL);
		m_trackends.resize(m_trackends.size()+1);
	} else if ((m_trackstarts.size() > 1) && (m_trackstarts.back() == NULL)) {
		m_trackstarts.back() = token;
	} else {
		m_trackstarts.push_back(token);
		m_trackends.resize(m_trackends.size()+1);
	}
}



//////////////////////////////
//
// HumdrumFileBase::adjustSpines -- adjust datatype and spineinfo values based
//   on manipulators found in the data.
//

bool HumdrumFileBase::adjustSpines(HumdrumLine& line, vector<string>& datatype,
		vector<string>& sinfo) {
	vector<string> newtype;
	vector<string> newinfo;
	int mergecount = 0;
	int i, j;
	for (i=0; i<line.getTokenCount(); i++) {
		if (line.token(i)->isSplitInterpretation()) {
			newtype.resize(newtype.size() + 1);
			newtype.back() = datatype[i];
			newtype.resize(newtype.size() + 1);
			newtype.back() = datatype[i];
			newinfo.resize(newinfo.size() + 2);
			newinfo[newinfo.size()-2] = '(' + sinfo[i] + ")a";
			newinfo[newinfo.size()-1] = '(' + sinfo[i] + ")b";
		} else if (line.token(i)->isMergeInterpretation()) {
			mergecount = 0;
			for (j=i+1; j<line.getTokenCount(); j++) {
				if (line.token(j)->isMergeInterpretation()) {
					mergecount++;
				} else {
					break;
				}
			}
			newinfo.resize(newtype.size() + 1);
			newinfo.back() = getMergedSpineInfo(sinfo, i, mergecount);
			newtype.resize(newtype.size() + 1);
			newtype.back() = datatype[i];
			i += mergecount;
		} else if (line.token(i)->isAddInterpretation()) {
			newtype.resize(newtype.size() + 1);
			newtype.back() = datatype[i];
			newtype.resize(newtype.size() + 1);
			newtype.back() = "";
			newinfo.resize(newinfo.size() + 1);
			newinfo.back() = sinfo[i];
			newinfo.resize(newinfo.size() + 1);
			addToTrackStarts(NULL);
			newinfo.back() = to_string(getMaxTrack());
		} else if (line.token(i)->isExchangeInterpretation()) {
			if (i < line.getTokenCount() - 1) {
				if (line.token(i)->isExchangeInterpretation()) {
					// exchange spine information
					newtype.resize(newtype.size() + 1);
					newtype.back() = datatype[i+1];
					newtype.resize(newtype.size() + 1);
					newtype.back() = datatype[i];
					newinfo.resize(newinfo.size() + 1);
					newinfo.back() = sinfo[i+1];
					newinfo.resize(newinfo.size() + 1);
					newinfo.back() = sinfo[i];
				} else {
					return setParseError("ERROR1 in *x calculation");
				}
				i++;
			} else {
				stringstream err;
				err << "ERROR2 in *x calculation" << endl;
				err << "Index " << i << " larger than allowed: "
				     << line.getTokenCount() - 1;
				return setParseError(err);
			}
		} else if (line.token(i)->isTerminateInterpretation()) {
			// store pointer to terminate token in trackends
			m_trackends[m_trackstarts.size()-1].push_back(line.token(i));
		} else if (((string*)line.token(i))->substr(0, 2) == "**") {
			newtype.resize(newtype.size() + 1);
			newtype.back() = line.getTokenString(i);
			newinfo.resize(newinfo.size() + 1);
			newinfo.back() = sinfo[i];
			if (!((m_trackstarts.size() > 1) && (m_trackstarts.back() == NULL))) {
				stringstream err;
				err << "Error: Exclusive interpretation with no preparation "
				     << "on line " << line.getLineIndex()
				     << " spine index " << i << endl;
				err << "Line: " << line;
				return setParseError(err);
			}
			if (m_trackstarts.back() == NULL) {
				addToTrackStarts(line.token(i));
			}
		} else {
			// should only be null interpretation, but doesn't matter
			newtype.resize(newtype.size() + 1);
			newtype.back() = datatype[i];
			newinfo.resize(newinfo.size() + 1);
			newinfo.back() = sinfo[i];
		}
	}

	datatype.resize(newtype.size());
	sinfo.resize(newinfo.size());
	for (i=0; i<(int)newtype.size(); i++) {
		datatype[i] = newtype[i];
		sinfo[i]    = newinfo[i];
	}

	return true;
}



//////////////////////////////
//
// HumdrumFileBase::getMergedSpineInfo -- Will only simplify a two-spine
//   merge.  Should be expanded to larger spine mergers in the future.
//   In other words, it is best to currently merge spines in the order
//   in which they were split, so that the original spine label can
//   be produced.
//

string HumdrumFileBase::getMergedSpineInfo(vector<string>& info, int starti,
		int extra) {
	string output;
	int len1;
	int len2;
	if (extra == 1) {
		len1 = (int)info[starti].size();
		len2 = (int)info[starti+1].size();
		if (len1 == len2) {
			if (info[starti].substr(0, len1-1) ==
					info[starti+1].substr(0,len2-1)) {
				output = info[starti].substr(1, len1-3);
				return output;
			}
		}
		output = info[starti] + " " + info[starti+1];
		return output;
	}
	output = info[starti];
	for (int i=0; i<extra; i++) {
		output += " " + info[starti+1+extra];
	}
	return output;
}



//////////////////////////////
//
// HumdrumFileBase::analyzeNonNullDataTokens -- For null data tokens, indicate
//    the previous non-null token which the null token refers to.  After
//    a spine merger, there may be multiple previous tokens, so you would
//		have to decide on the actual source token on based on subtrack or
//    sub-spine information.  The function also gives links to the previous/next
//    non-null tokens, skipping over intervening null data tokens.
//

bool HumdrumFileBase::analyzeNonNullDataTokens(void) {
	vector<HTp> ptokens;

	// analyze forward tokens:
	for (int i=1; i<=getMaxTrack(); i++) {
		if (!processNonNullDataTokensForTrackForward(getTrackStart(i),
				ptokens)) {
			return false;
		}
	}

	ptokens.resize(0);

	// analyze backward tokens:
	for (int i=1; i<=getMaxTrack(); i++) {
		for (int j=0; j<getTrackEndCount(i); j++) {
			if (!processNonNullDataTokensForTrackBackward(getTrackEnd(i, j),
					ptokens)) {
				return false;
			}
		}
	}

	// Eventually set the foward and backward non-null data token for
	// tokens in spines for all types of line types  For now specify
	// the next non-null data token for the exclusive interpretation token.
	// Also this implementation does not consider that the first
	// non-null data tokens may be from nultiple split tokens (fix later).
	vector<HTp> starts;
	vector<HTp> nexts;
	getSpineStartList(starts);
	nexts.resize(starts.size(), NULL);
	for (int i=0; i<(int)starts.size(); i++) {
		if (starts[i] == NULL) {
			continue;
		}
		HTp token = starts[i];
		token = token->getNextToken();
		while (token) {
			if (token->isData()) {
				if (!token->isNull()) {
					nexts[i] = token;
					break;
				}
			}
			token = token->getNextToken();
		}
	}
	for (int i=0; i<(int)nexts.size(); i++) {
		if (nexts[i] == NULL) {
			continue;
		}
		starts[i]->addNextNonNullToken(nexts[i]);
	}

	return true;
}



//////////////////////////////
//
// HumdurmFile::processNonNullDataTokensForTrackBackward -- Helper function
//    for analyzeNonNullDataTokens.  Given any token, this function tells
//    you what is the next non-null data token(s) in the spine after the given
//    token.
//

bool HumdrumFileBase::processNonNullDataTokensForTrackBackward(
		HTp endtoken, vector<HTp> ptokens) {

	HTp token = endtoken;
	int tcount = token->getPreviousTokenCount();

	while (tcount > 0) {
		for (int i=1; i<tcount; i++) {
			if (!processNonNullDataTokensForTrackBackward(
					token->getPreviousToken(i), ptokens)) {
				return false;
			}
		}
		HTp prevtoken = token->getPreviousToken();
		if (prevtoken->isSplitInterpretation()) {
			addUniqueTokens(prevtoken->m_nextNonNullTokens, ptokens);
			if (token != prevtoken->m_nextTokens[0]) {
				// terminate if not most primary subspine
				return true;
			}
		} else if (token->isData()) {
			addUniqueTokens(token->m_nextNonNullTokens, ptokens);
			if (!token->isNull()) {
				ptokens.resize(0);
				ptokens.push_back(token);
			}
		}

		// Follow previous data token 0 since 1 and higher are handled above.
		token = token->getPreviousToken(0);
		tcount = token->getPreviousTokenCount();
	}

	return true;
}



//////////////////////////////
//
// HumdurmFile::processNonNullDataTokensForTrackForward -- Helper function
//    for analyzeNonNullDataTokens.  Given any token, this function tells
//    you what are the previous non-null data token(s) in the spine before
//    the given token.
//

bool HumdrumFileBase::processNonNullDataTokensForTrackForward(HTp starttoken,
		vector<HTp> ptokens) {

	HTp token = starttoken;
	int tcount = token->getNextTokenCount();
	while (tcount > 0) {
		if (token->isSplitInterpretation()) {
			for (int i=1; i<tcount; i++) {
				if (!processNonNullDataTokensForTrackForward(
						token->getNextToken(i), ptokens)) {
					return false;
				}
			}
		} else if (token->isMergeInterpretation()) {
			HTp nexttoken = token->getNextToken();
			addUniqueTokens(nexttoken->m_previousNonNullTokens, ptokens);
			if (token != nexttoken->m_previousTokens[0]) {
				// terminate if not most primary subspine
				return true;
			}
		} else {
			addUniqueTokens(token->m_previousNonNullTokens, ptokens);
			if (token->isData() && !token->isNull()) {
				ptokens.resize(0);
				ptokens.push_back(token);

			}
		}
		// Data tokens can only be followed by up to one next token,
		// so no need to check for more than one next token.
		token = token->getNextToken(0);
		tcount = token->getNextTokenCount();
	}

	return true;
}



//////////////////////////////
//
// HumdrumFileBase::addUniqueTokens -- Used for non-null token analysis.  The
//    analysis is recursive like rhythmic analysis in the HumdrumFileStructure
//    class, but this algorithm does not terminate secondary traversals when
//    recursing.  Perhaps that should be fixed (utilizing the "rhycheck"
//    variable in HumdrumTokens)
//

void HumdrumFileBase::addUniqueTokens(vector<HTp>& target,
		vector<HTp>& source) {
	int i, j;
	bool found;
	for (i=0; i<(int)source.size(); i++) {
		found = false;
		for (j=0; j<(int)target.size(); j++) {
			if (source[i] == target[i]) {
				found = true;
			}
		}
		if (!found) {
			target.push_back(source[i]);
		}
	}
}



//////////////////////////////
//
// HumdrumFileBase::adjustMergeSpineLines -- fix *v lines to that adjacent
//     tracks do not merge at the same time.  In other words, split the line
//     into two or more merge lines.
//

/* still to be implemented

void HumdrumFileBase::adjustMergeSpineLines(void) {
	HumdrumFileBase& infile = *this;
	// going backwards to not have to deal with line number updates
	// at the moment...
	for (int i=infile.getLineCount()-1; i>= 0; i--) {
		if (!infile[i].isManipulator()) {
			continue;
		}
		bool hasbadmerge = false;
		int track1;
		int track2;
		for (int j=1; j<infile[i].getFieldCount(); j++) {
			if (!infile[i].token(j)->equalTo("*v")) {
				continue;
			}
			if (!infile[i].token(j-1)->equalTo("*v")) {
				continue;
			}
			track1 = infile.token(i, j-1)->getTrack();
			track2 = infile.token(i, j)->getTrack();
			if (track1 != track2) {
				hasbadmerge = true;
				break;
			}
		}
		if (hasbadmerge) {
			cerr << "!! BADMERGE on line " << i + 1 << endl;
			fixMerges(i);
		}
	}
}

*/



//////////////////////////////
//
// HumdrumFileBase::fixMerges -- Split a line with merges into two
//    lines.  The line is presumed to have a bad merge which
//    means that two adjacent tracks have adjacent *v tokens.
//    This algorithm will create a new lines where everything
//    after the bad merge is placed on the newline.   Example:
//
// track:    1    2    2    3    3    4    5    5 
//           *    *v   *v   *v   *v   *    *v   *v
//
// This is invalid because track 2 and track 3 have adjacent *v tokens.
// This function will create a new line and move everything after 
// the bad position to a new line:
//
// track:    1    2    2    3    3    4    5    5 
//           *    *v   *v   *v   *v   *    *v   *v
//           *    *    *v   *v   *    *    *
// track:    1    2    3    3    4    5    5 
//
// This algorithm only fixes one bad boundary.  The calling function
// will presumably fix any bad boundaries on the newly created line.
//

/* Still to be implemented...
void HumdrumFileBase::fixMerges(int linei) {
	HumdrumFileBase& infile = *this;

	vector<vector<HTp> > linetoks;
	HTp tok;

	// linetoks: collect tokens on the current line by track groups.
	int track1 = -1;
	int track2 = -1;
	for (int j=0; j<infile[linei].getFieldCount(); j++) {
		tok = infile[linei].token(j);
		track2 = tok->getTrack();
		if (track2 != track1) {
			linetoks.resize(linetoks.size()+1);
			linetoks.back().push_back(tok);
		}
		track1 = track2;
	}

	// ptoks: collect the tokens on the previous line for stiching tokens
	// together after adding new line.
	vector<vector<HTp> > ptoks;
	track1 = -1;
	track2 = -1;
	for (int j=0; j<infile[linei-1].getFieldCount(); j++) {
		tok = infile[linei-1].token(j);
		track2 = tok->getTrack();
		if (track2 != track1) {
			ptoks.resize(ptoks.size()+1);
			ptoks.back().push_back(tok);
		}
		track1 = track2;
	}

	// ntoks: collect the tokens on the next line for stiching tokens
	// together after adding new line.
	vector<vector<HTp> > ntoks;
	track1 = -1;
	track2 = -1;
	for (int j=0; j<infile[linei+1].getFieldCount(); j++) {
		tok = infile[linei+1].token(j);
		track2 = tok->getTrack();
		if (track2 != track1) {
			ntoks.resize(ntoks.size()+1);
			ntoks.back().push_back(tok);
		}
		track1 = track2;
	}

	int maxt = infile.getMaxTrack();
	vector<vector<HTp> > newtokbytrack(maxt+1);

// track:    1    2    2    3    3    4    5    5 
//           *    *v   *v   *v   *v   *    *v   *v
//
// o = new null tokens.
//
// original     *    *v   *v   o    o    o    o    o 
// new          o    o         *v   *v   *    *v   *v
// track:       1    2         3    3    4    5    5 

	HumdrumLine* newline = new HumdrumLine;
	newline->setOwner(this);
	bool foundboundary = false;
	HTp token;
	int findex;
	// int swaptrack = -1;
	int difference = 0;  // decrease in token count on new line
	for (int i=0; i<linetoks.size()-1; i++) {
		if (foundboundary) {
			// transfer the track tokens to the new line, and put
			// new null tokens in their place on the old line.
			for (int j=0; j<(int)linetoks[i].size(); j++) {
				track1 = linetoks[i][j]->getTrack();
				findex = linetoks[i][j]->getFieldIndex();
            // move the token to the next line:
				newline->m_tokens.push_back(linetoks[i][j]);
				// put it in the list for later processing:
				newtokbytrack[track1].push_back(linetoks[i][j]);
				// replace the moved token with a null token:
				token = new HumdrumToken("*");
				infile[linei].m_tokens[findex] = token;
				// probably need to update the HumAddress of both tokens.
			}
		} else if ((!foundboundary) && linetoks[i].back()->equalTo("*v") &&
				linetoks[i+1][0]->equalTo("*v")) {
			// This is the bad boundary.  Keep track fields in the
			// original line, and create one new null token in
			// the newline.
// original     *    *v   *v   o    o    o    o    o 
// new          o    o         *v   *v   *    *v   *v
// track:       1    2         3    3    4    5    5 
			difference = linetoks[i].size() - 1;

			track1 = linetoks[i][0]->getTrack();
			token = new HumdrumToken("*");
			token->setTrack(track1);
			token->setSubtrack(track1);
			newline->m_tokens.push_back(token);
			// put new token in list for later processing:
			newtokbytrack[track1].push_back(token);
			
			foundboundary = true;
		} else {
			// add null tokens to the new line, and keep the
			// tokens on the original line as they were
			for (int j=0; j<(int)linetoks[i].size(); j++) {
				track1 = linetoks[i][j]->getTrack();
				token = new HumdrumToken("*");
				token->setTrack(track1);
				token->setSubtrack(track1);
				newline->m_tokens.push_back(token);
				// put new token in list for later processing:
				newtokbytrack[track1].push_back(token);
			}
		}
	}

	// for now the links between the tokens on successive lines
	// will not be updated.  For the most part it will not be
	// important.  Probably more important is to update line numbers
	// for HumdrumLines occurring on new lines.  Maybe need to set
	// the line type for the new line.

	// add the new line to the file:
	m_lines.insert(m_lines.begin() + linei + 1, newline);

}

*/


//////////////////////////////
//
// operator<< -- Default method of printing HumdrumFiles.  This printing method
//    assumes that the HumdrumLine string is correct.  If a token is changed
//    in the file, the HumdrumFileBase::createLinesFromTokens() before printing
//    the contents of the line.
//

ostream& operator<<(ostream& out, HumdrumFileBase& infile) {
	for (int i=0; i<infile.getLineCount(); i++) {
		out << infile[i] << '\n';
	}
	return out;
}


//////////////////////////////
//
// sortTokenParisByLineIndex -- Sort two tokens so that the one
//    on the smaller line is first.  If both are on the same line then
//    sort the left-most token first.
//

bool sortTokenPairsByLineIndex(const TokenPair& a, const TokenPair& b) {
	if (a.first->getLineIndex() < b.first->getLineIndex()) {
		return true;
	}
	if (a.first->getLineIndex() == b.first->getLineIndex()) {
		if (a.first->getFieldIndex() < b.first->getFieldIndex()) {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// HumdrumFileBase::makeBooleanTrackList --
//

void HumdrumFileBase::makeBooleanTrackList(vector<bool>& spinelist,
		const string& spinestring) {
	Convert::makeBooleanTrackList(spinelist, spinestring, getMaxTrack());
}



//////////////////////////////
//
// HumdrumFileBase::getMeasureNumber -- If the current line is a
//      barline, then read the first integer found in the fields on the line.
//

int HumdrumFileBase::getMeasureNumber(int line) {
   HumdrumFileBase& infile = *this;
   int j;
   if (!infile[line].isBarline()) {
      // Return -1 if not a barline.  May be changed in the future
      // to return the measure number of the previous barline.
      return -1;
   }
   HumRegex hre;
   int measurenumber = -1;
   for (j=0; j<infile[line].getFieldCount(); j++) {
      if (hre.search(*infile.token(line, j), "^=[^\\d]*(\\d+)")) {
         measurenumber = hre.getMatchInt(1);
         return measurenumber;
      }
   }
	return -1;
}





//////////////////////////////
//
// HumdrumFileContent::analyzeKernAccidentals -- Identify accidentals that
//    should be printed (only in **kern spines) as well as cautionary
//    accidentals (accidentals which are forced to be displayed but otherwise
//    would not be printed.  Algorithm assumes that all secondary tied notes
//    will not display their accidental across a system break.  Consideration
//    about grace-note accidental display still needs to be done.
//

bool HumdrumFileContent::analyzeKernAccidentals(void) {
	HumdrumFileContent& infile = *this;
	int i, j, k;
	int kindex;
	int track;

	// ktracks == List of **kern spines in data.
	// rtracks == Reverse mapping from track to ktrack index (part/staff index).
	vector<HTp> ktracks = getKernSpineStartList();
	vector<int> rtracks(getMaxTrack()+1, -1);
	for (i=0; i<(int)ktracks.size(); i++) {
		track = ktracks[i]->getTrack();
		rtracks[track] = i;
	}
	int kcount = (int)ktracks.size();

	// keysigs == key signature spellings of diatonic pitch classes.  This array
	// is duplicated into dstates after each barline.
	vector<vector<int> > keysigs;
	keysigs.resize(kcount);
	for (i=0; i<kcount; i++) {
		keysigs[i].resize(7);
		std::fill(keysigs[i].begin(), keysigs[i].end(), 0);
	}

	// dstates == diatonic states for every pitch in a spine.
	// sub-spines are considered as a single unit, although there are
	// score conventions which would keep a separate voices on a staff
	// with different accidental states (i.e., two parts superimposed
	// on the same staff, but treated as if on separate staves).
	// Eventually this algorithm should be adjusted for dealing with
	// cross-staff notes, where the cross-staff notes should be following
	// the accidentals of a different spine...
	vector<vector<int> > dstates; // diatonic states
	dstates.resize(kcount);
	for (i=0; i<kcount; i++) {
		dstates[i].resize(70);     // 10 octave limit for analysis
			                        // may cause problems; fix later.
		std::fill(dstates[i].begin(), dstates[i].end(), 0);
	}

	// gdstates == grace note diatonic states for every pitch in a spine.
	vector<vector<int> > gdstates; // grace-note diatonic states
	gdstates.resize(kcount);
	for (i=0; i<kcount; i++) {
		gdstates[i].resize(70);
		std::fill(gdstates[i].begin(), gdstates[i].end(), 0);
	}


	// rhythmstart == keep track of first beat in measure.
	vector<int> firstinbar(kcount, 0);
	
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].hasSpines()) {
			continue;
		}
		if (infile[i].isInterpretation()) {
			for (j=0; j<infile[i].getFieldCount(); j++) {
				if (!infile[i].token(j)->isKern()) {
					continue;
				}
				if (infile[i].token(j)->compare(0, 3, "*k[") == 0) {
					track = infile[i].token(j)->getTrack();
					kindex = rtracks[track];
					fillKeySignature(keysigs[kindex], *infile[i].token(j));
					// resetting key states of current measure.  What to do if this
					// key signature is in the middle of a measure?
					resetDiatonicStatesWithKeySignature(dstates[kindex],
							keysigs[kindex]);
					resetDiatonicStatesWithKeySignature(gdstates[kindex],
							keysigs[kindex]);
				}
			}
		} else if (infile[i].isBarline()) {
			for (j=0; j<infile[i].getFieldCount(); j++) {
				if (!infile[i].token(j)->isKern()) {
					continue;
				}
				if (infile[i].token(j)->isInvisible()) {
					continue;
				}
				std::fill(firstinbar.begin(), firstinbar.end(), 1);
				track = infile[i].token(j)->getTrack();
				kindex = rtracks[track];
				// reset the accidental states in dstates to match keysigs.
				resetDiatonicStatesWithKeySignature(dstates[kindex],
						keysigs[kindex]);
				resetDiatonicStatesWithKeySignature(gdstates[kindex],
						keysigs[kindex]);
			}
		}

		if (!infile[i].isData()) {
			continue;
		}

		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile[i].token(j)->isKern()) {
				continue;
			}
			if (infile[i].token(j)->isNull()) {
				continue;
			}
			if (infile[i].token(j)->isRest()) {
				continue;
			}

			int subcount = infile[i].token(j)->getSubtokenCount();
			track = infile[i].token(j)->getTrack();
			int rindex = rtracks[track];
			for (k=0; k<subcount; k++) {
				string subtok = infile[i].token(j)->getSubtoken(k);
				int b40 = Convert::kernToBase40(subtok);
				int diatonic = Convert::kernToBase7(subtok);
				if (diatonic < 0) {
					// Deal with extra-low notes later.
					continue;
				}
				int graceQ = infile[i].token(j)->isGrace();
				int accid = Convert::kernToAccidentalCount(subtok);
				int hiddenQ = 0;
				if (subtok.find("yy") == string::npos) {
					if ((subtok.find("ny") != string::npos) ||
					    (subtok.find("#y") != string::npos) ||
					    (subtok.find("-y") != string::npos)) {
						hiddenQ = 1;
					}
				}

				if (((subtok.find("_") != string::npos) ||
						(subtok.find("]") != string::npos))) {
					// tied notes do not have slurs, so skip them
					if ((accid != keysigs[rindex][diatonic % 7]) &&
							firstinbar[rindex]) {
						// But first, prepare to force an accidental to be shown on
						// the note immediately following the end of a tied group
						// if the tied group crosses a barline.
						dstates[rindex][diatonic] = -1000 + accid;
						gdstates[rindex][diatonic] = -1000 + accid;
					}
					continue;
				}

				size_t loc;
				// check for accidentals on trills, mordents and turns.
				if (subtok.find("t") != string::npos) {
					// minor second trill
					int trillnote     = b40 + 5;
					int trilldiatonic = Convert::base40ToDiatonic(trillnote);
					int trillaccid    = Convert::base40ToAccidental(trillnote);
					if (dstates[rindex][trilldiatonic] != trillaccid) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"trillAccidental", to_string(trillaccid));
						dstates[rindex][trilldiatonic] = -1000 + trillaccid;
					}
				} else if (subtok.find("T") != string::npos) {
					// major second trill
					int trillnote     = b40 + 6;
					int trilldiatonic = Convert::base40ToDiatonic(trillnote);
					int trillaccid    = Convert::base40ToAccidental(trillnote);
					if (dstates[rindex][trilldiatonic] != trillaccid) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"trillAccidental", to_string(trillaccid));
						dstates[rindex][trilldiatonic] = -1000 + trillaccid;
					}
				} else if (subtok.find("M") != string::npos) {
					// major second upper mordent
					int auxnote     = b40 + 6;
					int auxdiatonic = Convert::base40ToDiatonic(auxnote);
					int auxaccid    = Convert::base40ToAccidental(auxnote);
					if (dstates[rindex][auxdiatonic] != auxaccid) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"mordentUpperAccidental", to_string(auxaccid));
						dstates[rindex][auxdiatonic] = -1000 + auxaccid;
					}
				} else if (subtok.find("m") != string::npos) {
					// minor second upper mordent
					int auxnote     = b40 + 5;
					int auxdiatonic = Convert::base40ToDiatonic(auxnote);
					int auxaccid    = Convert::base40ToAccidental(auxnote);
					if (dstates[rindex][auxdiatonic] != auxaccid) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"mordentUpperAccidental", to_string(auxaccid));
						dstates[rindex][auxdiatonic] = -1000 + auxaccid;
					}
				} else if (subtok.find("W") != string::npos) {
					// major second upper mordent
					int auxnote     = b40 - 6;
					int auxdiatonic = Convert::base40ToDiatonic(auxnote);
					int auxaccid    = Convert::base40ToAccidental(auxnote);
					if (dstates[rindex][auxdiatonic] != auxaccid) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"mordentLowerAccidental", to_string(auxaccid));
						dstates[rindex][auxdiatonic] = -1000 + auxaccid;
					}
				} else if (subtok.find("w") != string::npos) {
					// minor second upper mordent
					int auxnote     = b40 - 5;
					int auxdiatonic = Convert::base40ToDiatonic(auxnote);
					int auxaccid    = Convert::base40ToAccidental(auxnote);
					if (dstates[rindex][auxdiatonic] != auxaccid) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"mordentLowerAccidental", to_string(auxaccid));
						dstates[rindex][auxdiatonic] = -1000 + auxaccid;
					}

				} else if ((loc = subtok.find("$")) != string::npos) {

					int turndiatonic = Convert::base40ToDiatonic(b40);
					// int turnaccid = Convert::base40ToAccidental(b40);
					// inverted turn
					int lowerint = 0;
					int upperint = 0;
					if (loc < subtok.size()-1) {
						if (subtok[loc+1] == 's') {
							lowerint = -5;
						} else if (subtok[loc+1] == 'S') {
							lowerint = -6;
						}
					}
					if (loc < subtok.size()-2) {
						if (subtok[loc+2] == 's') {
							upperint = +5;
						} else if (subtok[loc+2] == 'S') {
							upperint = +6;
						}
					}
					int lowerdiatonic = turndiatonic - 1;
					// Maybe also need to check for forced accidental state...
					int loweraccid = dstates[rindex][lowerdiatonic];
					int lowerb40 = Convert::base7ToBase40(lowerdiatonic) + loweraccid;
					int upperdiatonic = turndiatonic + 1;
					// Maybe also need to check for forced accidental state...
					int upperaccid = dstates[rindex][upperdiatonic];
					int upperb40 = Convert::base7ToBase40(upperdiatonic) + upperaccid;
					if (lowerint == 0) {
						// need to calculate lower interval (but it will not appear
						// below the inverted turn, just calculating for performance
						// rendering.
						lowerint = lowerb40 - b40;
						lowerb40 = b40 + lowerint;
					}
					if (upperint == 0) {
						// need to calculate upper interval (but it will not appear
						// above the inverted turn, just calculating for performance
						// rendering.
						upperint = upperb40 - b40;
						upperb40 = b40 + upperint;
					}
					int uacc = Convert::base40ToAccidental(b40 + upperint);
					int bacc = Convert::base40ToAccidental(b40 + lowerint);
					if (uacc != upperaccid) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"turnUpperAccidental", to_string(uacc));
						dstates[rindex][upperdiatonic] = -1000 + uacc;
					}
					if (bacc != loweraccid) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"turnLowerAccidental", to_string(bacc));
						dstates[rindex][lowerdiatonic] = -1000 + bacc;
					}

				} else if ((loc = subtok.find("S")) != string::npos) {

					int turndiatonic = Convert::base40ToDiatonic(b40);
					// int turnaccid = Convert::base40ToAccidental(b40);
					// regular turn
					int lowerint = 0;
					int upperint = 0;
					if (loc < subtok.size()-1) {
						if (subtok[loc+1] == 's') {
							upperint = +5;
						} else if (subtok[loc+1] == 'S') {
							upperint = +6;
						}
					}
					if (loc < subtok.size()-2) {
						if (subtok[loc+2] == 's') {
							lowerint = -5;
						} else if (subtok[loc+2] == 'S') {
							lowerint = -6;
						}
					}
					int lowerdiatonic = turndiatonic - 1;
					// Maybe also need to check for forced accidental state...
					int loweraccid = dstates[rindex][lowerdiatonic];
					int lowerb40 = Convert::base7ToBase40(lowerdiatonic) + loweraccid;
					int upperdiatonic = turndiatonic + 1;
					// Maybe also need to check for forced accidental state...
					int upperaccid = dstates[rindex][upperdiatonic];
					int upperb40 = Convert::base7ToBase40(upperdiatonic) + upperaccid;
					if (lowerint == 0) {
						// need to calculate lower interval (but it will not appear
						// below the inverted turn, just calculating for performance
						// rendering.
						lowerint = lowerb40 - b40;
						lowerb40 = b40 + lowerint;
					}
					if (upperint == 0) {
						// need to calculate upper interval (but it will not appear
						// above the inverted turn, just calculating for performance
						// rendering.
						upperint = upperb40 - b40;
						upperb40 = b40 + upperint;
					}
					int uacc = Convert::base40ToAccidental(b40 + upperint);
					int bacc = Convert::base40ToAccidental(b40 + lowerint);
					if (uacc != upperaccid) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"turnUpperAccidental", to_string(uacc));
						dstates[rindex][upperdiatonic] = -1000 + uacc;
					}
					if (bacc != loweraccid) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"turnLowerAccidental", to_string(bacc));
						dstates[rindex][lowerdiatonic] = -1000 + bacc;
					}
				}

				if (graceQ && (accid != gdstates[rindex][diatonic])) {
					// accidental is different from the previous state so should be
					// printed
					if (!hiddenQ) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"visualAccidental", "true");
						if (gdstates[rindex][diatonic] < -900) {
							// this is an obligatory cautionary accidental
							// or at least half the time it is (figure that out later)
							infile[i].token(j)->setValue("auto", to_string(k),
									"obligatoryAccidental", "true");
							infile[i].token(j)->setValue("auto", to_string(k),
									"cautionaryAccidental", "true");
						}
					}
					gdstates[rindex][diatonic] = accid;
					// regular notes are not affected by grace notes accidental
					// changes, but should have an obligatory cautionary accidental,
					// displayed for clarification.
					dstates[rindex][diatonic] = -1000 + accid;

				} else if (!graceQ && (accid != dstates[rindex][diatonic])) {
					// accidental is different from the previous state so should be
					// printed, but only print if not supposed to be hidden.
					if (!hiddenQ) {
						infile[i].token(j)->setValue("auto", to_string(k),
								"visualAccidental", "true");
						if (dstates[rindex][diatonic] < -900) {
							// this is an obligatory cautionary accidental
							// or at least half the time it is (figure that out later)
							infile[i].token(j)->setValue("auto", to_string(k),
									"obligatoryAccidental", "true");
							infile[i].token(j)->setValue("auto", to_string(k),
									"cautionaryAccidental", "true");
						}
					}
					dstates[rindex][diatonic] = accid;
					gdstates[rindex][diatonic] = accid;

				} else if ((accid == 0) && (subtok.find("n") != string::npos) &&
							!hiddenQ) {
					infile[i].token(j)->setValue("auto", to_string(k),
							"cautionaryAccidental", "true");
					infile[i].token(j)->setValue("auto", to_string(k),
							"visualAccidental", "true");
				} else if (subtok.find("XX") == string::npos) {
					// The accidental is not necessary. See if there is a single "X"
					// immediately after the accidental which means to force it to
					// display.
					auto loc = subtok.find("X");
					if ((loc != string::npos) && (loc > 0)) {
						if (subtok[loc-1] == '#') {
							infile[i].token(j)->setValue("auto", to_string(k),
									"cautionaryAccidental", "true");
									infile[i].token(j)->setValue("auto", to_string(k),
											"visualAccidental", "true");
						} else if (subtok[loc-1] == '-') {
							infile[i].token(j)->setValue("auto", to_string(k),
									"cautionaryAccidental", "true");
									infile[i].token(j)->setValue("auto", to_string(k),
											"visualAccidental", "true");
						} else if (subtok[loc-1] == 'n') {
							infile[i].token(j)->setValue("auto", to_string(k),
									"cautionaryAccidental", "true");
							infile[i].token(j)->setValue("auto", to_string(k),
									"visualAccidental", "true");
						}
					}
				}
			}
		}
		std::fill(firstinbar.begin(), firstinbar.end(), 0);
	}

	// Indicate that the accidental analysis has been done:
	infile.setValue("auto", "accidentalAnalysis", "true");

	return true;
}



//////////////////////////////
//
// HumdrumFileContent::fillKeySignature -- Read key signature notes and
//    assign +1 to sharps, -1 to flats in the diatonic input array.  Used
//    only by HumdrumFileContent::analyzeKernAccidentals().
//

void HumdrumFileContent::fillKeySignature(vector<int>& states,
		const string& keysig) {
	std::fill(states.begin(), states.end(), 0);
	if (keysig.find("f#") != string::npos) { states[3] = +1; }
	if (keysig.find("c#") != string::npos) { states[0] = +1; }
	if (keysig.find("g#") != string::npos) { states[4] = +1; }
	if (keysig.find("d#") != string::npos) { states[1] = +1; }
	if (keysig.find("a#") != string::npos) { states[5] = +1; }
	if (keysig.find("e#") != string::npos) { states[2] = +1; }
	if (keysig.find("b#") != string::npos) { states[6] = +1; }
	if (keysig.find("b-") != string::npos) { states[6] = -1; }
	if (keysig.find("e-") != string::npos) { states[2] = -1; }
	if (keysig.find("a-") != string::npos) { states[5] = -1; }
	if (keysig.find("d-") != string::npos) { states[1] = -1; }
	if (keysig.find("g-") != string::npos) { states[4] = -1; }
	if (keysig.find("c-") != string::npos) { states[0] = -1; }
	if (keysig.find("f-") != string::npos) { states[3] = -1; }
}



//////////////////////////////
//
// HumdrumFileContent::resetDiatonicStatesWithKeySignature -- Only used in
//     HumdrumFileContent::analyzeKernAccidentals().  Resets the accidental
//     states for notes
//

void HumdrumFileContent::resetDiatonicStatesWithKeySignature(vector<int>&
		states, vector<int>& signature) {
	for (int i=0; i<(int)states.size(); i++) {
		states[i] = signature[i % 7];
	}
}



//////////////////////////////
//
// HumdrumFileStructure::getMetricLevels -- Each line in the output
//     vector matches to the line of the metric analysis data.
//     undefined is the value to represent undefined analysis data
//     (for non-data spines).
//
//     default value: track = 0: 0 means use the time signature
//         of the first **kern spines in the file; otherwise, use the
//         time signatures found in the given track (indexed from 1
//         for the first spine on a line).
//     default value: undefined = NAN: The value to use for un-analyzed lines.
//

void HumdrumFileContent::getMetricLevels(vector<double>& output,
		int track, double undefined) {

	HumdrumFileStructure& infile = *this;
	int lineCount = infile.getLineCount();
	output.resize(lineCount);
	fill(output.begin(), output.end(), undefined);
	vector<HTp> kernspines = infile.getKernSpineStartList();
	if ((track == 0) && (kernspines.size() > 0)) {
		track = kernspines[0]->getTrack();
	}
	if (track == 0) {
		track = 1;
	}

	int top = 1;                // top number of time signature (0 for no meter)
	int bot = 4;                // bottom number of time signature
	bool compoundQ = false;     // test for compound meters, such as 6/8
	HumNum beatdur(1 * 4, bot); // duration of a beat in the measure
	HumNum measurepos;          // quarter notes from the start of barline
	HumNum combeatdur;          // for adjusting beat level in compound meters
	HumNum commeasurepos;       // for adjusting beat level in compound meters

	for (int i=0; i<lineCount; i++) {
		if (infile[i].isInterpretation()) {
			// check for time signature:
			HumdrumLine& line = *infile.getLine(i);
			for (int j=0; j<line.getFieldCount(); j++) {
				int ttrack = line.token(j)->getTrack();
				if (ttrack != track) {
					continue;
				}
				if (sscanf(infile.token(i,j)->c_str(), "*M%d/%d", &top, &bot)) {
					beatdur.setValue(1*4, bot); // converted to quarter-note units
					if ((top % 3 == 0) && (top != 3)) {
						// if meter top is a multiple of 3 but not 3, then compound
						// such as 6/8, 9/8, 6/4, but not 3/8, 3/4.
						compoundQ = true;
						beatdur *= 3;
					} else {
						compoundQ = false;
					}
					break;
				}
			}
		}
		if (!infile[i].isData()) {
				continue;
		}

		measurepos = infile[i].getDurationFromBarline();
		// Might want to handle cases where the time signature changes in
		// the middle or a measure...
		measurepos /= beatdur;
		int denominator = measurepos.getDenominator();
		if (compoundQ) {
			output[i] = Convert::nearIntQuantize(log(denominator) / log(3.0));
			if ((output[i] != 0.0) && (output[i] != 1.0)) {
				// if not the beat or first level, then calculate
				// levels above level 1.  In 6/8 this means
				// to move the 8th note level to be the "beat"
				// and then use binary levels for rhythmic levels
				// smaller than a beat.
				combeatdur.setValue(4,bot);
				commeasurepos = infile[i].getDurationFromBarline() / combeatdur;
				denominator = commeasurepos.getDenominator();
				output[i] = 1.0 + log(denominator)/log(2.0);
			}
		} else {
			output[i] = Convert::nearIntQuantize(log(denominator) / log(2.0));
		}
	}
}





//////////////////////////////
//
// HumdrumFileContent::analyzeKernSlurs -- Link start and ends of
//    slurs to each other.
//

bool HumdrumFileContent::analyzeKernSlurs(void) {
	vector<HTp> kernspines;
	getSpineStartList(kernspines, "**kern");
	bool output = true;
	for (int i=0; i<(int)kernspines.size(); i++) {
		output = output && analyzeKernSlurs(kernspines[i]);
	}
	return output;
}


bool HumdrumFileContent::analyzeKernSlurs(HTp spinestart) {
	// tracktokens == the 2-D data list for the track,
	// arranged in layers with the second dimension.
	vector<vector<HTp> > tracktokens;
	this->getTrackSeq(tracktokens, spinestart, OPT_DATA | OPT_NOEMPTY);
	// printSequence(tracktokens);

	// sluropens == list of slur openings for each track and elision level
	// first dimension: elision level
	// second dimension: track number
	vector<vector<vector<HTp> > > sluropens;

	sluropens.resize(4); // maximum of 4 elision levels
	for (int i=0; i<(int)sluropens.size(); i++) {
		sluropens[i].resize(8);  // maximum of 8 layers
	}

	int opencount = 0;
	int closecount = 0;
	int elision = 0;
	HTp token;
	for (int row=0; row<(int)tracktokens.size(); row++) {
		for (int track=0; track<(int)tracktokens[row].size(); track++) {
			token = tracktokens[row][track];
			if (!token->isData()) {
				continue;
			}
			if (token->isNull()) {
				continue;
			}
			opencount = (int)count(token->begin(), token->end(), '(');
			closecount = (int)count(token->begin(), token->end(), ')');

			for (int i=0; i<closecount; i++) {
				elision = token->getSlurEndElisionLevel(i);
				if (elision < 0) {
					continue;
				}
				if (sluropens[elision][track].size() > 0) {
					linkSlurEndpoints(sluropens[elision][track].back(), token);
					// remove slur opening from buffer
					sluropens[elision][track].pop_back();
				} else {
					// No starting slur marker to match to this slur end in the
					// given track.
					// search for an open slur in another track:
					bool found = false;
					for (int itrack=0; itrack<(int)sluropens[elision].size(); itrack++) {
						if (sluropens[elision][itrack].size() > 0) {
							linkSlurEndpoints(sluropens[elision][itrack].back(), token);
							// remove slur opening from buffer
							sluropens[elision][itrack].pop_back();
							found = true;
							break;
						}
					}
					if (!found) {
						token->setValue("auto", "hangingSlur", "true");
						token->setValue("auto", "slurDration",
							token->getDurationToEnd());
					}
				}
			}

			for (int i=0; i<opencount; i++) {
				elision = token->getSlurStartElisionLevel(i);
				if (elision < 0) {
					continue;
				}
				sluropens[elision][track].push_back(token);
			}
		}
	}

	// Mark un-closed slur starts:
	for (int i=0; i<(int)sluropens.size(); i++) {
		for (int j=0; j<(int)sluropens[i].size(); j++) {
			for (int k=0; k<(int)sluropens[i][j].size(); j++) {
				sluropens[i][j][k]->setValue("", "auto", "hangingSlur", "true");
				sluropens[i][j][k]->setValue("", "auto", "slurDuration",
					sluropens[i][j][k]->getDurationFromStart());
			}
		}
	}

	return true;
}



//////////////////////////////
//
// HumdrumFileContent::linkSlurEndpoints --  Allow up to two slur starts/ends
//      on a note.
//

void HumdrumFileContent::linkSlurEndpoints(HTp slurstart, HTp slurend) {
	string durtag = "slurDuration";
	string endtag = "slurEnd";
	int slurEndCount = slurstart->getValueInt("auto", "slurEndCount");
	slurEndCount++;
	if (slurEndCount > 1) {
		endtag += to_string(slurEndCount);
		durtag += to_string(slurEndCount);
	}
	string starttag = "slurStart";
	int slurStartCount = slurend->getValueInt("auto", "slurStartCount");
	slurStartCount++;
	if (slurStartCount > 1) {
		starttag += to_string(slurStartCount);
	}

	slurstart->setValue("auto", endtag, slurend);
	slurstart->setValue("auto", "id", slurstart);
	slurend->setValue("auto", starttag, slurstart);
	slurend->setValue("auto", "id", slurend);
	HumNum duration = slurend->getDurationFromStart() 
			- slurstart->getDurationFromStart();
	slurstart->setValue("auto", durtag, duration);
	slurstart->setValue("auto", "slurEndCount", to_string(slurEndCount));
	slurend->setValue("auto", "slurStartCount", to_string(slurStartCount));
}





//////////////////////////////
//
// HumdrumFileContent::analyzeKernTies -- Link start and ends of
//    ties to each other.
//

bool HumdrumFileContent::analyzeKernTies(void) {
	vector<HTp> kernspines;
	getSpineStartList(kernspines, "**kern");
	bool output = true;
	for (int i=0; i<(int)kernspines.size(); i++) {
		output = output && analyzeKernTies(kernspines[i]);
	}
	return output;
}


bool HumdrumFileContent::analyzeKernTies(HTp spinestart) {
	vector<vector<HTp> > tracktokens;
	this->getTrackSeq(tracktokens, spinestart, OPT_DATA | OPT_NOEMPTY);


	return true;
}



//////////////////////////////
//
// HumdrumFileStructure::getTimeSigs -- Return the prevailing time signature
//     top and bottom for a particular spine for each line in the HumdrumFile.
//     This version does not handle mulimeters such as 2+3/4 or 3/4+6/8.
//     Only checks the primary strand of a spine/track for time signatures.
//
//     default value: track = 0: 0 means use the time signature
//         of the first **kern spine in the file; otherwise, use the
//         time signatures found in the given track (indexed from 1
//         for the first spine on a line).  A value of <0, 0> is used for
//         unassigned time signature lines.
//

void HumdrumFileContent::getTimeSigs(vector<pair<int, HumNum> >& output,
		int track) {
	HumdrumFileStructure& infile = *this;
	int lineCount = infile.getLineCount();
	output.resize(lineCount);
	pair<int, HumNum> current(0, 0);
	fill(output.begin(), output.end(), current);
	if (track == 0) {
		vector<HTp> kernspines = infile.getKernSpineStartList();
		if (kernspines.size() > 0) {
			track = kernspines[0]->getTrack();
		}
	}
	if (track == 0) {
		track = 1;
	}

	int top  = 0;   // top number of time signature (0 for no meter)
	int bot  = 0;   // bottom number of time signature
	int bot2 = 0;   // such as the 2 in 3%2.

	int firstsig  = -1;
	int firstdata = -1;

	HTp token = getTrackStart(track);
	while (token) {
		if (token->isData()) {
			if (firstdata < 0) {
				firstdata = token->getLineIndex();
			}
			token = token->getNextToken();
			continue;
		}
		if (!token->isInterpretation()) {
			token = token->getNextToken();
			continue;
		}
		// check for time signature:
		if (sscanf(token->c_str(), "*M%d/%d%%%d", &top, &bot, &bot2) == 3) {
			current.first = top;
			current.second.setValue(bot, bot2);
			if (firstsig < 0) {
				firstsig = token->getLineIndex();
			}
		} else if (sscanf(token->c_str(), "*M%d/%d", &top, &bot) == 2) {
			current.first = top;
			current.second = bot;
			if (firstsig < 0) {
				firstsig = token->getLineIndex();
			}
		}
		output[token->getLineIndex()] = current;
		token = token->getNextToken();
	}

	// Back-fill the list if the first time signature occurs before
	// the start of the data:
	if ((firstsig > 0) && (firstdata >= firstsig)) {
		current = output[firstsig];
		for (int i=0; i<firstsig; i++) {
			output[i] = current;
		}
	}

	// In-fill the list:
	int starti = firstsig;
	if (starti < 0) {
		starti = 0;
	}
	current = output[starti];
	for (int i=starti+1; i<(int)output.size(); i++) {
		if (output[i].first == 0) {
			output[i] = current;
		} else {
			current = output[i];
		}
	}
}



//////////////////////////////
//
// HumdrumFileContent::HumdrumFileContent --
//

HumdrumFileContent::HumdrumFileContent(void) {
	// do nothing
}


HumdrumFileContent::HumdrumFileContent(const string& filename) :
		HumdrumFileStructure() {
	read(filename);
}


HumdrumFileContent::HumdrumFileContent(istream& contents) :
		HumdrumFileStructure() {
	read(contents);
}



//////////////////////////////
//
// HumdrumFileContent::~HumdrumFileContent --
//

HumdrumFileContent::~HumdrumFileContent() {
	// do nothing
}




//////////////////////////////
//
// HumdrumFileStream::HumdrumFileStream --
//

HumdrumFileStream::HumdrumFileStream(void) {
	m_curfile = -1;
}

HumdrumFileStream::HumdrumFileStream(char** list) {
	m_curfile = -1;
	setFileList(list);
}

HumdrumFileStream::HumdrumFileStream(const vector<string>& list) {
	m_curfile = -1;
	setFileList(list);
}

HumdrumFileStream::HumdrumFileStream(Options& options) {
	m_curfile = -1;
	vector<string> list;
	options.getArgList(list);
	setFileList(list);
}



//////////////////////////////
//
// HumdrumFileStream::clear -- reset the contents of the class.
//

void HumdrumFileStream::clear(void) {
	m_curfile = 0;
	m_filelist.resize(0);
	m_universals.resize(0);
	m_newfilebuffer.resize(0);
}



//////////////////////////////
//
// HumdrumFileStream::setFileList --
//

int HumdrumFileStream::setFileList(char** list) {
	m_filelist.reserve(1000);
	m_filelist.resize(0);
	int i = 0;
	while (list[i] != NULL) {
		m_filelist.push_back(list[i]);
		i++;
	}
	return i;
}


int HumdrumFileStream::setFileList(const vector<string>& list) {
	m_filelist = list;
	return (int)list.size();
}



//////////////////////////////
//
// HumdrumFileStream::read -- alias for getFile.
//

int HumdrumFileStream::read(HumdrumFile& infile) {
	return getFile(infile);
}



//////////////////////////////
//
// HumdrumFileStream::eof -- returns true if there is no more segements
//     to read from the input source(s).
//

int HumdrumFileStream::eof(void) {
	istream* newinput = NULL;

	// Read HumdrumFile contents from:
	// (1) Current ifstream if open
	// (2) Next filename if ifstream is done
	// (3) cin if no ifstream open and no filenames

	// (1) Is an ifstream open?, then yes, there is more data to read.
	if (m_instream.is_open() && !m_instream.eof()) {
		return 0;
	}

	// (1b) Is the URL data buffer open?
	else if (m_urlbuffer.str() != "") {
		return 0;
	}

	// (2) If ifstream is closed but there is a file to be processed,
	// load it into the ifstream and start processing it immediately.
	else if ((m_filelist.size() > 0) && (m_curfile < (int)m_filelist.size()-1)) {
		return 0;
	} else {
		// no input fstream open and no list of files to process, so
		// start (or continue) reading from standard input.
		if (m_curfile < 0) {
			// but only read from cin if no files have previously been read
			newinput = &cin;
		}
		if ((newinput != NULL) && newinput->eof()) {
			return 1;
		}
	}

	return 1;
}



//////////////////////////////
//
// HumdrumFileStream::getFile -- fills a HumdrumFile class with content
//    from the input stream or next input file in the list.  Returns
//    true if content was extracted, fails if there is no more HumdrumFiles
//    in the input stream.
//

int HumdrumFileStream::getFile(HumdrumFile& infile) {
	infile.clear();
	istream* newinput = NULL;

restarting:

	newinput = NULL;

	if (m_urlbuffer.eof()) {
		// If the URL buffer is at its end, clear the buffer.
		m_urlbuffer.str("");
	}

	// Read HumdrumFile contents from:
	// (1) Current ifstream if open
	// (2) Next filename if ifstream is done
	// (3) cin if no ifstream open and no filenames

	// (1) Is an ifstream open?
	if (m_instream.is_open() && !m_instream.eof()) {
		newinput = &m_instream;
	}

	// (1b) Is the URL data buffer open?
	else if (m_urlbuffer.str() != "") {
		m_urlbuffer.clear();
		newinput = &m_urlbuffer;
	}

	// (2) If ifstream is closed but there is a file to be processed,
	// load it into the ifstream and start processing it immediately.
	else if (((int)m_filelist.size() > 0) &&
			(m_curfile < (int)m_filelist.size()-1)) {
		m_curfile++;
		if (m_instream.is_open()) {
			m_instream.close();
		}
		if (strstr(m_filelist[m_curfile].c_str(), "://") != NULL) {
			// The next file to read is a URL/URI, so buffer the
			// data from the internet and start reading that instead
			// of reading from a file on the hard disk.
			fillUrlBuffer(m_urlbuffer, m_filelist[m_curfile].c_str());
			infile.setFilename(m_filelist[m_curfile].c_str());
			goto restarting;
		}
		m_instream.open(m_filelist[m_curfile].c_str());
		infile.setFilename(m_filelist[m_curfile].c_str());
		if (!m_instream.is_open()) {
			// file does not exist or cannot be opened close
			// the file and try luck with next file in the list
			// (perhaps given an error or warning?).
			infile.setFilename("");
			m_instream.close();
			goto restarting;
		}
		newinput = &m_instream;
	} else {
		// no input fstream open and no list of files to process, so
		// start (or continue) reading from standard input.
		if (m_curfile < 0) {
			// but only read from cin if no files have previously been read
			newinput = &cin;
		}
	}

	// At this point the newinput istream is set to read from the given
	// file or from standard input, so start reading Humdrum content.
	// If there is "m_newfilebuffer" content, then set the filename of the
	// HumdrumFile to that value.

	if (m_newfilebuffer.size() > 0) {
		// store the filename for the current HumdrumFile being read:
		HumRegex hre;
		if (hre.search(m_newfilebuffer,
				R"(^!!!!SEGMENT\s*([+-]?\d+)?\s*:\s*(.*)\s*$)")) {
			if (hre.getMatchLength(1) > 0) {
				infile.setSegmentLevel(atoi(hre.getMatch(1).c_str()));
			} else {
				infile.setSegmentLevel(0);
			}
			infile.setFilename(hre.getMatch(2));
		} else if ((m_curfile >=0) && (m_curfile < (int)m_filelist.size())
				&& (m_filelist.size() > 0)) {
			infile.setFilename(m_filelist[m_curfile].c_str());
		} else {
			// reading from standard input, but no name.
		}
	}

	if (newinput == NULL) {
		// something strange happened, or no more files to read.
		return 0;
	}

	stringstream buffer;
	int foundUniversalQ = 0;

	// Start reading the input stream.  If !!!!SEGMENT: universal comment
	// is found, then store that line in m_newfilebuffer and return the
	// newly read HumdrumFile.  If other universal comments are found, then
	// overwrite the old universal comments here.

	int addedFilename = 0;
	//int searchName = 0;
	int dataFoundQ = 0;
	int starstarFoundQ = 0;
	int starminusFoundQ = 0;
	if (m_newfilebuffer.size() < 4) {
		//searchName = 1;
	}
	char templine[123123] = {0};

	if (newinput->eof()) {
		if (m_curfile < (int)m_filelist.size()-1) {
			m_curfile++;
			goto restarting;
		}
		// input stream is close and there is no more files to process.
		return 0;
	}

	istream& input = *newinput;

	// if the previous line from the last read starts with "**"
	// then treat it as part of the current file.
	if ((m_newfilebuffer.size() > 1) &&
		 (strncmp(m_newfilebuffer.c_str(), "**", 2)) == 0) {
		buffer << m_newfilebuffer << "\n";
		m_newfilebuffer = "";
		starstarFoundQ = 1;
	}

	while (!input.eof()) {
		input.getline(templine, 123123, '\n');
		if ((!dataFoundQ) &&
				(strncmp(templine, "!!!!SEGMENT", strlen("!!!!SEGMENT")) == 0)) {
			string tempstring;
			tempstring = templine;
			HumRegex hre;
			if (hre.search(tempstring,
					"^!!!!SEGMENT\\s*([+-]?\\d+)?\\s*:\\s*(.*)\\s*$")) {
				if (hre.getMatchLength(1) > 0) {
					infile.setSegmentLevel(atoi(hre.getMatch(1).c_str()));
				} else {
					infile.setSegmentLevel(0);
				}
				infile.setFilename(hre.getMatch(2));
			}
			continue;
		}

		if (strncmp(templine, "**", 2) == 0) {
			if (starstarFoundQ == 1) {
				m_newfilebuffer = templine;
				// already found a **, so this one is defined as a file
				// segment.  Exit from the loop and process the previous
				// content, waiting until the next read for to start with
				// this line.
				break;
			}
			starstarFoundQ = 1;
		}

		if (input.eof() && (strcmp(templine, "") == 0)) {
			// No more data coming from current stream, so this is
			// the end of the HumdrumFile.  Break from the while loop
			// and then store the read contents of the stream in the
			// HumdrumFile.
			break;
		}
		// (1) Does the line start with "!!!!SEGMENT"?  If so, then
		// this is either the name of the current or next file to process.
		// (1a) this is the name of the current file to process if no
		// data has yet been found,
		// (1b) or a name is being actively searched for.
		if (strncmp(templine, "!!!!SEGMENT", strlen("!!!!SEGMENT")) == 0) {
			m_newfilebuffer = templine;
			if (dataFoundQ) {
				// this new filename is for the next chunk to process in the
				// current file stream, not this one, so stop reading the
				// HumdrumFile content and send what has already been read back
				// out with new contents.
				break;
			}  else {
				// !!!!SEGMENT: came before any real data was read, so
				// it is most likely the name of the current file
				// (i.e., it comes at the start of the file stream and
				// is the name of the first HumdrumFile in the stream).
				HumRegex hre;
				if (hre.search(m_newfilebuffer,
						R"(^!!!!SEGMENT\s*([+-]?\d+)?\s:\s*(.*)\s*$)")) {
					if (hre.getMatchLength(1) > 0) {
						infile.setSegmentLevel(atoi(hre.getMatch(1).c_str()));
					} else {
						infile.setSegmentLevel(0);
					}
					infile.setFilename(hre.getMatch(2));
				}
				continue;
			}
		}
		int len = (int)strlen(templine);
		if ((len > 4) && (strncmp(templine, "!!!!", 4) == 0) &&
				(templine[4] != '!') && (dataFoundQ == 0)) {
			// This is a universal comment.  Should it be appended
			// to the list or should the currnet list be erased and
			// this record placed into the first entry?
			if (foundUniversalQ) {
				// already found a previous universal, so append.
				m_universals.push_back(templine);
			} else {
				// new universal comment, to delete all previous
				// universal comments and store this one.
				m_universals.reserve(1000);
				m_universals.resize(1);
				m_universals[0] = templine;
				foundUniversalQ = 1;
			}
			continue;
		}

		if (strncmp(templine, "*-", 2) == 0) {
			starminusFoundQ = 1;
		}

		// if before first ** in a data file or after *-, and the line
		// does not start with '!' or '*', then assume that it is a file
		// name which should be added to the file list to read.
		if (((starminusFoundQ == 1) || (starstarFoundQ == 0))
				&& (templine[0] != '*') && (templine[0] != '!')) {
			if ((templine[0] != '\0') && (templine[0] != ' ')) {
				// The file can only be added once in this manner
				// so that infinite loops are prevented.
				int found = 0;
				for (int mm=0; mm<(int)m_filelist.size(); mm++) {
					if (strcmp(m_filelist[mm].c_str(), templine) == 0) {
						found = 1;
					}
				}
				if (!found) {
					m_filelist.push_back(templine);
					addedFilename = 1;
				}
				continue;
			}
		}

		dataFoundQ = 1; // found something other than universal comments
		// should empty lines be treated somewhat as universal comments?

		// store the data line for later parsing into HumdrumFile record:
		buffer << templine << "\n";
	}

	if (dataFoundQ == 0) {
		// never found anything for some strange reason.
		if (addedFilename) {
			goto restarting;
		}
		return 0;
	}

	// Arriving here means that reading of the data stream is complete.
	// The string stream variable "buffer" contains the HumdrumFile
	// content, so send it to the HumdrumFile variable.  Also, prepend
	// Universal comments (demoted into Global comments) at the start
	// of the data stream (maybe allow for postpending Universal comments
	// in the future).
	stringstream contents;
	contents.str(""); // empty any contents in buffer
	contents.clear(); // reset error flags in buffer

	for (int i=0; i<(int)m_universals.size(); i++) {
		contents << &(m_universals[i][1]) << "\n";
	}
	contents << buffer.str();
	infile.read(contents);
	return 1;
}


//////////////////////////////
//
// HumdrumFileStream::fillUrlBuffer --
//


void HumdrumFileStream::fillUrlBuffer(stringstream& uribuffer,
		const string& uriname) {
	#ifdef USING_URI
		uribuffer.str(""); // empty any contents in buffer
		uribuffer.clear(); // reset error flags in buffer
		string webaddress = HumdrumFileBase::getUriToUrlMapping(uriname);
		HumdrumFileBase::readStringFromHttpUri(uribuffer, webaddress);
	#endif
}



//////////////////////////////
//
// HumdrumFileStructure::HumdrumFileStructure -- HumdrumFileStructure
//     constructor.
//

HumdrumFileStructure::HumdrumFileStructure(void) {
	// do nothing
}

HumdrumFileStructure::HumdrumFileStructure(const string& filename) :
		HumdrumFileBase() {
	read(filename);
}

HumdrumFileStructure::HumdrumFileStructure(istream& contents) :
		HumdrumFileBase() {
	read(contents);
}



//////////////////////////////
//
// HumdrumFileStructure::~HumdrumFileStructure -- HumdrumFileStructure
//     deconstructor.
//

HumdrumFileStructure::~HumdrumFileStructure() {
	// do nothing
}



//////////////////////////////
//
// HumdrumFileStructure::read --  Read the contents of a file from a file or
//   istream.  The file's structure is analyzed, and then the rhythmic structure
//   is calculated.
//


bool HumdrumFileStructure::read(istream& contents) {
	m_displayError = false;
	if (!readNoRhythm(contents)) {
		return isValid();
	}
	return analyzeStructure();
}


bool HumdrumFileStructure::read(const char* filename) {
	m_displayError = false;
	if (!readNoRhythm(filename)) {
		return isValid();
	}
	return analyzeStructure();
}


bool HumdrumFileStructure::read(const string& filename) {
	m_displayError = false;
	if (!readNoRhythm(filename)) {
		return isValid();
	}
	return analyzeStructure();
}



//////////////////////////////
//
// HumdrumFileStructure::readCsv --  Read the contents of a file from a file or
//   istream in CSV format.  The file's structure is analyzed, and then the
//   rhythmic structure is calculated.
// default value: separator = ","
//


bool HumdrumFileStructure::readCsv(istream& contents,
		const string& separator) {
	m_displayError = false;
	if (!readNoRhythmCsv(contents, separator)) {
		return isValid();
	}
	return analyzeStructure();
}


bool HumdrumFileStructure::readCsv(const char* filename,
		const string& separator) {
	m_displayError = false;
	if (!readNoRhythmCsv(filename, separator)) {
		return isValid();
	}
	return analyzeStructure();
}


bool HumdrumFileStructure::readCsv(const string& filename,
		const string& separator) {
	m_displayError = false;
	if (!readNoRhythmCsv(filename, separator)) {
		return isValid();
	}
	return analyzeStructure();
}



//////////////////////////////
//
// HumdrumFileStructure::readString -- Read the contents from a string.
//    Similar to HumdrumFileStructure::read, but for string data.
//

bool HumdrumFileStructure::readString(const char* contents) {
	m_displayError = false;
	if (!HumdrumFileBase::readString(contents)) {
		return isValid();
	}
	return analyzeStructure();
}


bool HumdrumFileStructure::readString(const string& contents) {
	m_displayError = false;
	if (!HumdrumFileBase::readString(contents)) {
		return isValid();
	}
	return analyzeStructure();
}



//////////////////////////////
//
// HumdrumFileStructure::readStringCsv -- Read the contents from a string.
//    Similar to HumdrumFileStructure::read, but for string data.
// default value: separator = ","
//

bool HumdrumFileStructure::readStringCsv(const char* contents,
		const string& separator) {
	m_displayError = false;
	if (!HumdrumFileBase::readStringCsv(contents, separator)) {
		return isValid();
	}
	return analyzeStructure();
}


bool HumdrumFileStructure::readStringCsv(const string& contents,
		const string& separator) {
	m_displayError = false;
	if (!HumdrumFileBase::readStringCsv(contents, separator)) {
		return isValid();
	}
	return analyzeStructure();
}



//////////////////////////////
//
// HumdrumFileStructure::analyzeStructure -- Analyze global/local
//    parameters and rhythmic structure.
//


bool HumdrumFileStructure::analyzeStructure(void) {
	if (!analyzeStrands()          ) { return isValid(); }
	if (!analyzeGlobalParameters() ) { return isValid(); }
	if (!analyzeLocalParameters()  ) { return isValid(); }
	if (!analyzeTokenDurations()   ) { return isValid(); }
	HTp firstspine = getSpineStart(0);
	if (firstspine && firstspine->isDataType("**recip")) {
		assignRhythmFromRecip(firstspine);
	} else {
		if (!analyzeRhythm()           ) { return isValid(); }
		if (!analyzeDurationsOfNonRhythmicSpines()) { return isValid(); }
	}
	return isValid();
}


//////////////////////////////
//
// HumdrumFileStructure::assignRhythmFromRecip --
//

bool HumdrumFileStructure::assignRhythmFromRecip(HTp spinestart) {
	HTp current = spinestart;
	
	HumNum duration;
	while (current) {
		if (!current->isData()) {
			current = current->getNextToken();
			continue;
		}
		if (current->isNull()) {
			// This should not occur in a well-formed **recip spine, but
			// treat as a zero duration.
			continue;
		}
		
		if (strchr(current->c_str(), 'q') != NULL) {
			duration = 0;
		} else {
			duration = Convert::recipToDuration((string)*current);
		}
		current->getLine()->setDuration(duration);
		current = current->getNextToken();
	}

	// now go back and set the absolute position from the start of
	// the file.
	HumNum sum = 0;
	HumdrumFileStructure& hfile = *this;
	for (int i=0; i<getLineCount(); i++) {
		hfile[i].setDurationFromStart(sum);
		if (hfile[i].getDuration() < 0) {
			hfile[i].setDuration(0);
		}
		sum += hfile[i].getDuration();
	}

	// Analyze durations to/from barlines:
	if (!analyzeMeter()) { return false; }
	if (!analyzeNonNullDataTokens()) { return false; }
	return true;
}



//////////////////////////////
//
// HumdrumFileStructure::readNoRhythm -- Similar to the read() functions, but
//    does not parse rhythm (or parameters).
//

bool HumdrumFileStructure::readNoRhythm(istream& infile) {
	return HumdrumFileBase::read(infile);
}


bool HumdrumFileStructure::readNoRhythm(const char* filename) {
	return HumdrumFileBase::read(filename);
}


bool HumdrumFileStructure::readNoRhythm(const string& filename) {
	return HumdrumFileBase::read(filename);
}



//////////////////////////////
//
// HumdrumFileStructure::readNoRhythmCsv -- Similar to the readCsv()
//    functions, but does not parse rhythm (or parameters).
// default value: separator = ","
//

bool HumdrumFileStructure::readNoRhythmCsv(istream& infile,
		const string& seperator) {
	return HumdrumFileBase::readCsv(infile);
}


bool HumdrumFileStructure::readNoRhythmCsv(const char* filename,
		const string& seperator) {
	return HumdrumFileBase::readCsv(filename);
}


bool HumdrumFileStructure::readNoRhythmCsv(const string& filename,
		const string& seperator) {
	return HumdrumFileBase::readCsv(filename);
}



//////////////////////////////
//
// HumdrumFileStructure::readStringNoRhythm -- Read a string, but
//   do not analyze the rhythm (or parameters) in the read data.
//


bool HumdrumFileStructure::readStringNoRhythm(const char* contents) {
	return HumdrumFileBase::readString(contents);
}


bool HumdrumFileStructure::readStringNoRhythm(const string& contents) {
	return HumdrumFileBase::readString(contents);
}



//////////////////////////////
//
// HumdrumFileStructure::readStringNoRhythmCsv -- Read a string, but
//   do not analyze the rhythm (or parameters) in the read data.
// default value: separator = ","
//


bool HumdrumFileStructure::readStringNoRhythmCsv(const char* contents,
		const string& separator) {
	return HumdrumFileBase::readStringCsv(contents);
}


bool HumdrumFileStructure::readStringNoRhythmCsv(const string& contents,
		const string& separator) {
	return HumdrumFileBase::readStringCsv(contents);
}



//////////////////////////////
//
// HumdrumFileStructure::getScoreDuration -- Return the total duration
//    of the score in quarter note units.  Returns zero if no lines in the
//    file, or -1 if there are lines, but no rhythmic analysis has been done.
//

HumNum HumdrumFileStructure::getScoreDuration(void) const {
	if (m_lines.size() == 0) {
		return 0;
	}
	return m_lines.back()->getDurationFromStart();
}



//////////////////////////////
//
// HumdrumFileStructure::tpq -- "Ticks per Quarter-note".  Returns the minimal
//    number of integral time units that divide a quarter note into equal
//    subdivisions.  This value is needed to convert Humdrum data into
//    MIDI file data, MuseData, and MusicXML data.  Also useful for timebase
//    type of operations on the data and describing the durations in terms
//    of integers rather than with fractions.  This function will also
//    consider the implicit durations of non-rhythmic spine data.
//

int HumdrumFileStructure::tpq(void) {
	if (m_ticksperquarternote > 0) {
		return m_ticksperquarternote;
	}
	set<HumNum> durlist = getPositiveLineDurations();
	vector<int> dems;
	for (auto& it : durlist) {
		if (it.getDenominator() > 1) {
			dems.push_back(it.getDenominator());
		}
	}
	int lcm = 1;
	if (dems.size() > 0) {
		lcm = Convert::getLcm(dems);
	}
	m_ticksperquarternote = lcm;
	return m_ticksperquarternote;
}



//////////////////////////////
//
// HumdrumFileStructure::getPositiveLineDurations -- Return a list of all
//    unique token durations in the file.  This function could be expanded
//    to limit the search to a range of lines or to a specific track.
//

set<HumNum> HumdrumFileStructure::getPositiveLineDurations(void) {
	set<HumNum> output;
	for (auto& line : m_lines) {
		if (line->getDuration().isPositive()) {
			output.insert(line->getDuration());
		}
	}
	return output;
}



//////////////////////////////
//
// HumdrumFileStructure::printDurationInfo -- Print the assigned duration
//    of each line in a file.  Useful for debugging.
//

ostream& HumdrumFileStructure::printDurationInfo(ostream& out) {
	for (int i=0; i<getLineCount(); i++) {
		m_lines[i]->printDurationInfo(out) << '\n';
	}
	return out;
}



//////////////////////////////
//
// HumdrumFileStructure::getBarline -- Return the given barline from the file
//   based on the index number.  Negative index accesses from the end of the
//   list.  If the first barline is a pickup measure, then the returned
//   HumdrumLine* will not be an actual barline line.
//

HumdrumLine* HumdrumFileStructure::getBarline(int index) const {
	if (index < 0) {
		index += m_barlines.size();
	}
	if (index < 0) {
		return NULL;
	}
	if (index >= (int)m_barlines.size()) {
		return NULL;
	}
	return m_barlines[index];
}



//////////////////////////////
//
// HumdrumFileStructure::getBarlineCount -- Return the number of barlines in
//   the file.  If there is a pickup beat, then the count includes an imaginary
//   barline before the first pickup (and the start of the file will be returned
//   for barline(0).
//

int HumdrumFileStructure::getBarlineCount(void) const {
	return (int)m_barlines.size();
}



///////////////////////////////
//
// HumdrumFileStructure::getBarlineDuration --  Return the duration from the
//    current barline to the next barline in the data.  For the last barline,
//    the duration will be calculated from the end of the data;  The final
//    will have a duration of 0 if there are not notes after the barline
//    in the data.
//

HumNum HumdrumFileStructure::getBarlineDuration(int index) const {
	if (index < 0) {
		index += m_barlines.size();
	}
	if (index < 0) {
		return 0;
	}
	if (index >= (int)m_barlines.size()) {
		return 0;
	}
	HumNum startdur = m_barlines[index]->getDurationFromStart();
	HumNum enddur;
	if (index + 1 < (int)m_barlines.size() - 1) {
		enddur = m_barlines[index+1]->getDurationFromStart();
	} else {
		enddur = getScoreDuration();
	}
	return enddur - startdur;
}



///////////////////////////////
//
// HumdrumFileStructure::getBarlineDurationFromStart -- Return the duration
//    between the start of the Humdrum file and the given barline.
//

HumNum HumdrumFileStructure::getBarlineDurationFromStart(int index) const {
	if (index < 0) {
		index += m_barlines.size();
	}
	if (index < 0) {
		return 0;
	}
	if (index >= (int)m_barlines.size()) {
		return getScoreDuration();
	}
	return m_barlines[index]->getDurationFromStart();
}



///////////////////////////////
//
// HumdrumFileStructure::getBarlineDurationToEnd -- Return the duration
//    between barline and the end of the HumdrumFileStructure.
//

HumNum HumdrumFileStructure::getBarlineDurationToEnd(int index) const {
	if (index < 0) {
		index += m_barlines.size();
	}
	if (index < 0) {
		return 0;
	}
	if (index >= (int)m_barlines.size()) {
		return getScoreDuration();
	}
	return m_barlines[index]->getDurationToEnd();
}



//////////////////////////////
//
// HumdrumFileStructure::analyzeRhythm -- Analyze the rhythmic structure of the
//     data.  Returns false if there was a parse error.
//

bool HumdrumFileStructure::analyzeRhythm(void) {
	if (getMaxTrack() == 0) {
		return true;
	}
	int startline = getTrackStart(1)->getLineIndex();
	int testline;
	HumNum zero(0);

	int i;
	for (int i=1; i<=getMaxTrack(); i++) {
		if (!getTrackStart(i)->hasRhythm()) {
			// Can't analyze rhythm of spines that do not have rhythm.
			continue;
		}
		testline = getTrackStart(i)->getLineIndex();
		if (testline == startline) {
			if (!assignDurationsToTrack(getTrackStart(i), zero)) {
				return false;
			}
		} else {
			// Spine does not start at beginning of data, so
			// the starting position of the spine has to be
			// determined before continuing.  Search for a token
			// which is on a line with assigned duration, then work
			// outwards from that position.
			continue;
		}
	}

	// Go back and analyze spines which do not start at the beginning
	// of the data stream.
	for (i=1; i<=getMaxTrack(); i++) {
		if (!getTrackStart(i)->hasRhythm()) {
			// Can't analyze rhythm of spines that do not have rhythm.
			continue;
		}
		testline = getTrackStart(i)->getLineIndex();
		if (testline > startline) {
			if (!analyzeRhythmOfFloatingSpine(getTrackStart(i))) { return false; }
		}
	}

	if (!analyzeNullLineRhythms()) { return false; }
	fillInNegativeStartTimes();
	assignLineDurations();
	if (!analyzeMeter()) { return false; }
	if (!analyzeNonNullDataTokens()) { return false; }

	return true;
}



//////////////////////////////
//
// HumdrumFileStructure::analyzeMeter -- Store the times from the last barline
//     to the current line, as well as the time to the next barline.
//     the sum of these two will be the duration of the barline, except
//     for barlines, where the getDurationToBarline() will store the
//     duration of the measure staring at that barline.  To get the
//     beat, you will have to figure out the current time signature.
//

bool HumdrumFileStructure::analyzeMeter(void) {

	m_barlines.resize(0);

	int i;
	HumNum sum = 0;
	bool foundbarline = false;
	for (i=0; i<getLineCount(); i++) {
		m_lines[i]->setDurationFromBarline(sum);
		sum += m_lines[i]->getDuration();
		if (m_lines[i]->isBarline()) {
			foundbarline = true;
			m_barlines.push_back(m_lines[i]);
			sum = 0;
		}
		if (m_lines[i]->isData() && !foundbarline) {
			// pickup measure, so set the first measure to the start of the file.
			m_barlines.push_back(m_lines[0]);
			foundbarline = 1;
		}
	}

	sum = 0;
	for (i=getLineCount()-1; i>=0; i--) {
		sum += m_lines[i]->getDuration();
		m_lines[i]->setDurationToBarline(sum);
		if (m_lines[i]->isBarline()) {
			sum = 0;
		}
	}

	return true;
}



//////////////////////////////
//
// HumdrumFileStructure::analyzeTokenDurations -- Calculate the duration of
//   all tokens in spines which posses duration in a file.
//

bool HumdrumFileStructure::analyzeTokenDurations (void) {
	for (int i=0; i<getLineCount(); i++) {
		if (!m_lines[i]->analyzeTokenDurations(m_parseError)) {
			return isValid();
		}
	}
	return isValid();
}



///////////////////////////////
//
// HumdrumFileStructure::analyzeGlobalParameters -- only allowing layout
//    parameters at the moment.  Global parameters affect the next
//    line which is either a barline, dataline or an interpretation
//    other than a spine manipulator.  Null lines are also not
//    considered.
//

bool HumdrumFileStructure::analyzeGlobalParameters(void) {
	HumdrumLine* spineline = NULL;
	for (int i=(int)m_lines.size()-1; i>=0; i--) {
		if (m_lines[i]->hasSpines()) {
			if (m_lines[i]->isAllNull())  {
				continue;
			}
			if (m_lines[i]->isManipulator()) {
				continue;
			}
			if (m_lines[i]->isCommentLocal()) {
				continue;
			}
			// should be a non-null data, barlines, or interpretation
			spineline = m_lines[i];
			continue;
		}
		if (spineline == NULL) {
			continue;
		}
		if (!m_lines[i]->isCommentGlobal()) {
			continue;
		}
		if (m_lines[i]->find("!!LO:") != 0) {
			continue;
		}
		spineline->setParameters(m_lines[i]);
	}

	return isValid();
}



///////////////////////////////
//
// HumdrumFileStructure::analyzeLocalParameters -- Parses any
//    local comments before a non-null token.
//

bool HumdrumFileStructure::analyzeLocalParameters(void) {
	// analyze backward tokens:
	for (int i=1; i<=getMaxTrack(); i++) {
		for (int j=0; j<getTrackEndCount(i); j++) {
			if (!processLocalParametersForTrack(getTrackEnd(i, j),
					getTrackEnd(i, j))) {
				return isValid();
			}
		}
	}

	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::analyzeDurationsOfNonRhythmicSpines -- Calculate the
//    duration of non-null data token in non-rhythmic spines.
//

bool HumdrumFileStructure::analyzeDurationsOfNonRhythmicSpines(void) {
	// analyze tokens backwards:
	for (int i=1; i<=getMaxTrack(); i++) {
		for (int j=0; j<getTrackEndCount(i); j++) {
			if (getTrackEnd(i, j)->hasRhythm()) {
				continue;
			}
			if (!assignDurationsToNonRhythmicTrack(getTrackEnd(i, j),
					getTrackEnd(i, j))) {
				return isValid();
			}
		}
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::getMinDur -- Return the smallest duration on the
//   line.  If all durations are zero, then return zero; otherwise,
//   return the smallest positive duration.
//

HumNum HumdrumFileStructure::getMinDur(vector<HumNum>& durs,
		vector<HumNum>& durstate) {
	HumNum mindur = 0;
	bool allzero = true;

	for (int i=0; i<(int)durs.size(); i++) {
		if (durs[i].isPositive()) {
			allzero = false;
			if (mindur.isZero()) {
				mindur = durs[i];
			} else if (mindur > durs[i]) {
				mindur = durs[i];
			}
		}
	}
	if (allzero) {
		return mindur;
	}

	for (int i=0; i<(int)durs.size(); i++) {
		if (durstate[i].isPositive() && mindur.isZero()) {
			if (durstate[i].isZero()) {
				// mindur = durstate[i];
			} else if (mindur > durstate[i]) {
				mindur = durstate[i];
			}
		}
	}
	return mindur;
}



//////////////////////////////
//
// HumdrumFileStructure::getTokenDurations -- Extract the duration of rhythmic
//    tokens on the line.
//

bool HumdrumFileStructure::getTokenDurations(vector<HumNum>& durs, int line) {
	durs.resize(0);
	for (int i=0; i<m_lines[line]->getTokenCount(); i++) {
		HumNum dur = m_lines[line]->token(i)->getDuration();
		durs.push_back(dur);
	}
	if (!cleanDurs(durs, line)) {
		return isValid();
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::cleanDurs -- Check if there are grace note and regular
//    notes on a line (not allowed).  Leaves negative durations which
//    indicate undefined durations (needed for keeping track of null
//    tokens in rhythmic spines.
//

bool HumdrumFileStructure::cleanDurs(vector<HumNum>& durs, int line) {
	bool zero     = false;
	bool positive = false;
	for (int i=0; i<(int)durs.size(); i++) {
		if      (durs[i].isPositive()) { positive = true; }
		else if (durs[i].isZero())     { zero     = true; }
	}
	if (zero && positive) {
		stringstream err;
		err << "Error on line " << (line+1) << " grace note and "
		    << " regular note cannot occur on same line." << endl;
		err << "Line: " << *m_lines[line] << endl;
		return setParseError(err);
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::decrementDurStates -- Subtract the line duration from
//   the current line of running durations.  If any duration is less
//   than zero, then a rhythm error exists in the data.
//

bool HumdrumFileStructure::decrementDurStates(vector<HumNum>& durs,
		HumNum linedur, int line) {
	if (linedur.isZero()) {
		return isValid();
	}
	for (int i=0; i<(int)durs.size(); i++) {
		if (!m_lines[line]->token(i)->hasRhythm()) {
			continue;
		}
		durs[i] -= linedur;
		if (durs[i].isNegative()) {
			stringstream err;
			err << "Error: rhythmic error on line " << (line+1)
			    << " field index " << i << endl;
			err << "Duration state is: " << durs[i] << endl;
			return setParseError(err);
		}
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::assignDurationsToTrack -- Assign duration from starts
//    for each rhythmic spine in the file.  Analysis is done recursively, one
//    sub-spine at a time.  Duplicate analyses are prevented by the state
//    variable in the HumdrumToken (currently called rhycheck because it is only
//    used in this function).  After the durationFromStarts have been assigned
//    for the rhythmic analysis of non-data tokens and non-rhythmic spines is
//    done elsewhere.
//

bool HumdrumFileStructure::assignDurationsToTrack(HumdrumToken* starttoken,
		HumNum startdur) {
	if (!starttoken->hasRhythm()) {
		return isValid();
	}
	int state = starttoken->getState();
	if (!prepareDurations(starttoken, state, startdur)) {
		return isValid();
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::prepareDurations -- Helper function for
//     HumdrumFileStructure::assignDurationsToTrack() which does all of the
//     work for assigning durationFromStart values.
//

bool HumdrumFileStructure::prepareDurations(HumdrumToken* token, int state,
		HumNum startdur) {
	if (state != token->getState()) {
		return isValid();
	}

	HumNum dursum = startdur;
	token->incrementState();

	if (!setLineDurationFromStart(token, dursum)) { return isValid(); }
	if (token->getDuration().isPositive()) {
		dursum += token->getDuration();
	}
	int tcount = token->getNextTokenCount();

	vector<HTp> reservoir;
	vector<HumNum> startdurs;

	// Assign line durationFromStarts for primary track first.
	while (tcount > 0) {
		for (int t=1; t<tcount; t++) {
			reservoir.push_back(token->getNextToken(t));
			startdurs.push_back(dursum);
		}
		token = token->getNextToken(0);
		if (state != token->getState()) {
			break;
		}
		token->incrementState();
		if (!setLineDurationFromStart(token, dursum)) { return isValid(); }
		if (token->getDuration().isPositive()) {
			dursum += token->getDuration();
		}
		tcount = token->getNextTokenCount();
	}

	if ((tcount == 0) && (token->isTerminateInterpretation())) {
		if (!setLineDurationFromStart(token, dursum)) { return isValid(); }
	}

	// Process secondary tracks next:
	int newstate = state;

	for (int i=(int)reservoir.size()-1; i>=0; i--) {
		prepareDurations(reservoir[i], newstate, startdurs[i]);
	}

	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::setLineDurationFromStart -- Set the duration of
//      a line based on the analysis of tokens in the spine.
//

bool HumdrumFileStructure::setLineDurationFromStart(HumdrumToken* token,
		HumNum dursum) {
	if ((!token->isTerminateInterpretation()) &&
			token->getDuration().isNegative()) {
		// undefined rhythm, so don't assign line duration information:
		return isValid();
	}
	HumdrumLine* line = token->getOwner();
	if (line->getDurationFromStart().isNegative()) {
		line->setDurationFromStart(dursum);
	} else if (line->getDurationFromStart() != dursum) {
		stringstream err;
		err << "Error: Inconsistent rhythm analysis occurring near line "
		    << token->getLineNumber() << endl;
		err << "Expected durationFromStart to be: " << dursum
		    << " but found it to be " << line->getDurationFromStart() << endl;
		err << "Line: " << *line << endl;
		return setParseError(err);
	}

	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::analyzeRhythmOfFloatingSpine --  This analysis
//    function is used to analyze the rhythm of spines which do not start at
//    the beginning of the data.  The function searches for the first line
//    which has an assigned durationFromStart value, and then uses that
//    as the basis for assigning the initial durationFromStart position
//    for the spine.
//

bool HumdrumFileStructure::analyzeRhythmOfFloatingSpine(
		HumdrumToken* spinestart) {
	HumNum dursum = 0;
	HumNum founddur = 0;
	HumdrumToken* token = spinestart;
	int tcount = token->getNextTokenCount();

	// Find a known durationFromStart for a line in the Humdrum file, then
	// use that to calculate the starting duration of the floating spine.
	if (token->getDurationFromStart().isNonNegative()) {
		founddur = token->getLine()->getDurationFromStart();
	} else {
		tcount = token->getNextTokenCount();
		while (tcount > 0) {
			if (token->getDurationFromStart().isNonNegative()) {
				founddur = token->getLine()->getDurationFromStart();
				break;
			}
			if (token->getDuration().isPositive()) {
				dursum += token->getDuration();
			}
			token = token->getNextToken(0);
		}
	}

	if (founddur.isZero()) {
		return setParseError("Error cannot link floating spine to score.");
	}

	if (!assignDurationsToTrack(spinestart, founddur - dursum)) {
		return isValid();
	}

	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::analyzeNullLineRhythms -- When a series of null-token
//    data line occur between two data lines possessing a start duration,
//    then split the duration between those two lines amongst the null-token
//    lines.  For example if a data line starts at time 15, and there is one
//    null-token line before another data line at time 16, then the null-token
//    line will be assigned to the position 15.5 in the score.
//

bool HumdrumFileStructure::analyzeNullLineRhythms(void) {
	vector<HumdrumLine*> nulllines;
	HumdrumLine* previous = NULL;
	HumdrumLine* next = NULL;
	HumNum dur;
	HumNum startdur;
	HumNum enddur;
	int i, j;
	for (i=0; i<(int)m_lines.size(); i++) {
		if (!m_lines[i]->hasSpines()) {
			continue;
		}
		if (m_lines[i]->isAllRhythmicNull()) {
			if (m_lines[i]->isData()) {
				nulllines.push_back(m_lines[i]);
			}
			continue;
		}
		dur = m_lines[i]->getDurationFromStart();
		if (dur.isNegative()) {
			if (m_lines[i]->isData()) {
				stringstream err;
				err << "Error: found an unexpected negative duration on line "
			       << m_lines[i]->getDurationFromStart()<< endl;
				err << "Line: " << *m_lines[i] << endl;
				return setParseError(err);
			} else {
				continue;
			}
		}
		next = m_lines[i];
		if (previous == NULL) {
			previous = next;
			nulllines.resize(0);
			continue;
		}
		startdur = previous->getDurationFromStart();
		enddur   = next ->getDurationFromStart();
		HumNum gapdur = enddur - startdur;
		HumNum nulldur = gapdur / ((int)nulllines.size() + 1);
		for (j=0; j<(int)nulllines.size(); j++) {
			nulllines[j]->setDurationFromStart(startdur + (nulldur * (j+1)));
		}
		previous = next;
		nulllines.resize(0);
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::fillInNegativeStartTimes -- Negative line durations
//    after the initial rhythmAnalysis mean that the lines are not data line.
//    Duplicate the duration of the next non-negative duration for all negative
//    durations.
//

void HumdrumFileStructure::fillInNegativeStartTimes(void) {
	int i;
	HumNum lastdur = -1;
	HumNum dur;
	for (i=(int)m_lines.size()-1; i>=0; i--) {
		dur = m_lines[i]->getDurationFromStart();
		if (dur.isNegative() && lastdur.isNonNegative()) {
			m_lines[i]->setDurationFromStart(lastdur);
		}
		if (dur.isNonNegative()) {
			lastdur = dur;
			continue;
		}
	}

	// fill in start times for ending comments
	for (i=0; i<(int)m_lines.size(); i++) {
		dur = m_lines[i]->getDurationFromStart();
		if (dur.isNonNegative()) {
			lastdur = dur;
		} else {
			m_lines[i]->setDurationFromStart(lastdur);
		}
	}
}



//////////////////////////////
//
// HumdrumFileStructure::assignLineDurations --  Calculate the duration of lines
//   based on the durationFromStart of the current line and the next line.
//

void HumdrumFileStructure::assignLineDurations(void) {
	HumNum startdur;
	HumNum enddur;
	HumNum dur;
	for (int i=0; i<(int)m_lines.size()-1; i++) {
		startdur = m_lines[i]->getDurationFromStart();
		enddur = m_lines[i+1]->getDurationFromStart();
		dur = enddur - startdur;
		m_lines[i]->setDuration(dur);
	}
	if (m_lines.size() > 0) {
		m_lines.back()->setDuration(0);
	}
}



//////////////////////////////
//
// HumdrumFileStructure::assignDurationsToNonRhythmicTrack --  After the basic
//   rhythmAnalysis has been done, go back and assign durations to non-rhythmic
//   spine tokens based on the lineFromStart values of the lines that they
//   occur on as well as the distance in the file to the next non-null token for
//   that spine.
//

bool HumdrumFileStructure::assignDurationsToNonRhythmicTrack(
		HumdrumToken* endtoken, HumdrumToken* current) {
	HumdrumToken* token = endtoken;
	int tcount = token->getPreviousTokenCount();
	while (tcount > 0) {
		for (int i=1; i<tcount; i++) {
			if (!assignDurationsToNonRhythmicTrack(token->getPreviousToken(i),
					current)) {
				return isValid();
			}
		}
		if (token->isData()) {
			if (!token->isNull()) {
				token->setDuration(current->getDurationFromStart() -
						token->getDurationFromStart());
				current = token;
			}
		}
		// Data tokens can only be followed by up to one previous token,
		// so no need to check for more than one next token.
		token = token->getPreviousToken(0);
		tcount = token->getPreviousTokenCount();
	}

	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::processLocalParametersForTrack --  Search for
//   local parameters backwards in each spine and fill in the HumHash
//   for the token to which the parameter is to be applied.
//

bool HumdrumFileStructure::processLocalParametersForTrack(
		HumdrumToken* starttok, HumdrumToken* current) {

	HumdrumToken* token = starttok;
	int tcount = token->getPreviousTokenCount();

	while (tcount > 0) {
		for (int i=1; i<tcount; i++) {
			if (!processLocalParametersForTrack(
					token->getPreviousToken(i), current)) {
				return isValid();
			}
		}
		HTp prevtoken = token->getPreviousToken();
		if (prevtoken->isSplitInterpretation()) {
			if (token != prevtoken->m_nextTokens[0]) {
				// terminate if not most primary subspine
				return true;
			}
		} else if (!(token->isNull() & token->isManipulator())) {
			if (token->isCommentLocal()) {
				checkForLocalParameters(token, current);
			} else {
				current = token;
			}
		}

		// Follow previous data token 0 since 1 and higher are handled above.
		token = token->getPreviousToken(0);
		tcount = token->getPreviousTokenCount();
	}

	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::checkForLocalParameters -- Helper function for
//     HumdrumFileStructure::processLocalParametersForTrack.  Only allowing
//     layout parameters currently.
//

void HumdrumFileStructure::checkForLocalParameters(HumdrumToken *token,
		HumdrumToken *current) {
	if (token->size() < 1) {
		return;
	}
	int loc1 = (int)token->find(":");
	if (loc1 == (int)string::npos) {
		return;
	}
	int loc2 = (int)token->substr(loc1).find(":");
	if (loc2 == (int)string::npos) {
		return;
	}
	loc2 += loc1 + 1;
	int sloc = (int)token->find(" ");
	if (sloc != (int)string::npos) {
		if ((sloc < loc1) || (sloc < loc2)) {
			return;
		}
	}
	sloc = (int)token->find("\t");
	if (sloc != (int)string::npos) {
		if ((sloc < loc1) || (sloc < loc2)) {
			return;
		}
	}
	// Looks like a parameter so parse the comment:
	current->setParameters(token);
}



//////////////////////////////
//
// HumdrumFileStructure::analyzeStrands -- Analyze spine strands.
//

bool HumdrumFileStructure::analyzeStrands(void) {
	int spines = getSpineCount();
	m_strand1d.resize(0);
	m_strand2d.resize(0);
	int i, j;
	for (i=0; i<spines; i++) {
		HumdrumToken* tok = getSpineStart(i);
		m_strand2d.resize(m_strand2d.size()+1);
		analyzeSpineStrands(m_strand2d.back(), tok);
	}

	for (i=0; i<(int)m_strand2d.size(); i++) {
		std::sort(m_strand2d[i].begin(), m_strand2d[i].end(),
				sortTokenPairsByLineIndex);
		for (j=0; j<(int)m_strand2d[i].size(); j++) {
			m_strand1d.push_back(m_strand2d[i][j]);
		}
	}

	assignStrandsToTokens();

	resolveNullTokens();
	
	return isValid();
}



///////////////////////////////
//
// HumdrumFileStructure::resolveNullTokens --
//

void HumdrumFileStructure::resolveNullTokens(void) {
	HTp token;
	HTp data = NULL;
	HTp strandend;
	for (int s=0; s<(int)m_strand1d.size(); s++) {
		token = getStrandStart(s);
		strandend = getStrandEnd(s);
		while (token != strandend) {
			if (!token->isData()) {
				token = token->getNextToken();
				continue;
			}
			if (data == NULL) {
				data = token;
				token->setNullResolution(data);
				token = token->getNextToken();
				continue;
			}
			if (token->isNull()) {
				token->setNullResolution(data);
			} else {
				data = token;
			}
			token = token->getNextToken();
		}
	}
}



//////////////////////////////
//
// HumdrumFileStructure::assignStrandsToTokens -- Store the 1D strand
//    index number for each token in the file.  Global tokens will have
//    strand index set to -1.
//

void HumdrumFileStructure::assignStrandsToTokens(void) {
	HTp tok;
	for (int i=0; i<(int)m_strand1d.size(); i++) {
		tok = m_strand1d[i].first;
		while (tok != NULL) {
			tok->setStrandIndex(i);
			tok = tok->getNextToken();
		}
	}
}



//////////////////////////////
//
// HumdrumFileStructure::analyzeSpineStrands -- Fill in the list of
//   strands in a single spine.
//

void HumdrumFileStructure::analyzeSpineStrands(vector<TokenPair>& ends,
		HumdrumToken* starttok) {

	ends.resize(ends.size()+1);
	int index = (int)ends.size()-1;
	ends[index].first = starttok;
	HumdrumToken* tok = starttok;
	while (tok != NULL) {
		if ((tok->getSubtrack() > 1) && (tok->isMerge())) {
			ends[index].last = tok;
			return;
		}
		if (tok->isTerminator()) {
			ends[index].last = tok;
			return;
		}
		if (tok->getNextTokenCount() > 1) {
			// should only be 2, but allow for generalizing in the future.
			for (int j=1; j<tok->getNextTokenCount(); j++) {
				analyzeSpineStrands(ends, tok->getNextToken(j));
			}
		}
		tok = tok->getNextToken();
	}

	cerr << "Should not get here in analyzeSpineStrands()\n";
}


//////////////////////////////
//
// HumdrumFileStructure::getStrandCount --
//

int HumdrumFileStructure::getStrandCount(void) const {
	return (int)m_strand1d.size();
}


int HumdrumFileStructure::getStrandCount(int spineindex) const {
	if (spineindex < 0) {
		return 0;
	}
	if (spineindex >= (int)m_strand2d.size()) {
		return 0;
	}
	return (int)m_strand2d[spineindex].size();
}



//////////////////////////////
//
// HumdrumFileStructure::getStrandStart -- Return the first token
//    in the a strand.
//

HumdrumToken* HumdrumFileStructure::getStrandStart(int index) const {
	return m_strand1d[index].first;
}


HumdrumToken* HumdrumFileStructure::getStrandEnd(int index) const {
	return m_strand1d[index].last;
}


HumdrumToken* HumdrumFileStructure::getStrandStart(int sindex,
		int index) const {
	return m_strand2d[sindex][index].first;
}


HumdrumToken* HumdrumFileStructure::getStrandEnd(int sindex, int index) const {
	return m_strand2d[sindex][index].last;
}



//////////////////////////////
//
// HumdrumFileStructure::hasFilters -- Returns true if has any
//    reference records starting with "!!!filter:".
//

bool HumdrumFileStructure::hasFilters(void) {
	HumdrumFileBase& infile = *this;
	vector<HumdrumLine*> refs  = infile.getReferenceRecords();
	for (int i=0; i<(int)refs.size(); i++) {
		if (refs[i]->getReferenceKey() == "filter") {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// HumdrumLine::HumdrumLine -- HumdrumLine constructor.
//

HumdrumLine::HumdrumLine(void) : string() {
	m_owner = NULL;
	m_duration = -1;
	m_durationFromStart = -1;
	setPrefix("!!");
}


HumdrumLine::HumdrumLine(const string& aString) : string(aString) {
	m_owner = NULL;
	if ((this->size() > 0) && (this->back() == 0x0d)) {
		this->resize(this->size() - 1);
	}
	m_duration = -1;
	m_durationFromStart = -1;
	setPrefix("!!");
	createTokensFromLine();
}


HumdrumLine::HumdrumLine(const char* aString) : string(aString) {
	m_owner = NULL;
	if ((this->size() > 0) && (this->back() == 0x0d)) {
		this->resize(this->size() - 1);
	}
	m_duration = -1;
	m_durationFromStart = -1;
	setPrefix("!!");
	createTokensFromLine();
}


HumdrumLine::HumdrumLine(HumdrumLine& line) {
	m_lineindex           = line.m_lineindex;
	m_duration            = line.m_duration;
	m_durationFromStart   = line.m_durationFromStart;
	m_durationFromBarline = line.m_durationFromBarline;
	m_durationToBarline   = line.m_durationToBarline;
	m_tokens.resize(line.m_tokens.size());
	for (int i=0; i<(int)m_tokens.size(); i++) {
		m_tokens[i] = new HumdrumToken(*line.m_tokens[i], this);
	}
	m_owner = NULL;
}


HumdrumLine::HumdrumLine(HumdrumLine& line, void* owner) {
	m_lineindex           = line.m_lineindex;
	m_duration            = line.m_duration;
	m_durationFromStart   = line.m_durationFromStart;
	m_durationFromBarline = line.m_durationFromBarline;
	m_durationToBarline   = line.m_durationToBarline;
	m_tokens.resize(line.m_tokens.size());
	for (int i=0; i<(int)m_tokens.size(); i++) {
		m_tokens[i] = new HumdrumToken(*line.m_tokens[i], this);
	}
	m_owner = owner;
}



//////////////////////////////
//
// HumdrumLine::operator= --
//

HumdrumLine& HumdrumLine::operator=(HumdrumLine& line) {
	m_lineindex           = line.m_lineindex;
	m_duration            = line.m_duration;
	m_durationFromStart   = line.m_durationFromStart;
	m_durationFromBarline = line.m_durationFromBarline;
	m_durationToBarline   = line.m_durationToBarline;
	m_tokens.resize(line.m_tokens.size());
	for (int i=0; i<(int)m_tokens.size(); i++) {
		m_tokens[i] = new HumdrumToken(*line.m_tokens[i], this);
	}
	m_owner = NULL;
	return *this;
}



//////////////////////////////
//
// HumdrumLine::~HumdrumLine -- HumdrumLine deconstructor.
//

HumdrumLine::~HumdrumLine() {
	// free stored HumdrumTokens:
	for (int i=0; i<(int)m_tokens.size(); i++) {
		if (m_tokens[i] != NULL) {
			delete m_tokens[i];
			m_tokens[i] = NULL;
		}
	}
}



//////////////////////////////
//
// HumdrumLine::setLineFromCsv -- Read a HumdrumLine from a CSV line.
// default value: separator = ","
//

void HumdrumLine::setLineFromCsv(const char* csv, const string& separator) {
	string temp = csv;
	setLineFromCsv(temp);
}



void HumdrumLine::setLineFromCsv(const string& csv, const string& separator) {
	if (csv.size() < 1) {
		return;
	}
	string newcsv = csv;
	if ((newcsv.size() > 0) && (newcsv.back() == 0x0d)) {
		newcsv.resize(newcsv.size() - 1);
	}
	// construct tab-delimited string
	string output;
	bool inquote = false;

	if ((newcsv.size() >= 2) && (newcsv[0] == '!') && (newcsv[1] == '!')) {
		// Global commands and reference records which do not start with a
		// quote are considered to be literal.
		this->setText(newcsv);
		return;
	}

	for (int i=0; i<(int)newcsv.size(); i++) {
		if ((newcsv[i] == '"') && !inquote) {
			inquote = true;
			continue;
		}
		if (inquote && (newcsv[i] == '"') && (newcsv[i+1] == '"')
				&& (i < (int)newcsv.length()-1)) {
			output += '"';
			i++;
			continue;
		}
		if (newcsv[i] == '"') {
			inquote = false;
			continue;
		}
		if ((!inquote) && (newcsv.substr(i, separator.size()) == separator)) {
			output += '\t';
			i += separator.size() - 1;
			continue;
		}
		output += newcsv[i];
	}
	string& value = *this;
	value = output;
}



//////////////////////////////
//
// HumdrumLine::setText -- Get the textual content of the line.  Note that
//    you may need to run HumdrumLine::createLineFromTokens() if the tokens
//    of the line have changed.
//

void HumdrumLine::setText(const string& text) {
	string::assign(text);
}



//////////////////////////////
//
// HumdrumLine::getText --
//

string HumdrumLine::getText(void) {
	return string(*this);
}



//////////////////////////////
//
// HumdrumLine::clear -- Remove stored tokens.
//

void HumdrumLine::clear(void) {
	for (int i=0; i<(int)m_tokens.size(); i++) {
		if (m_tokens[i] != NULL) {
			delete m_tokens[i];
			m_tokens[i] = NULL;
		}
	}
}



//////////////////////////////
//
// HumdrumLine::equalChar -- return true if the character at the given
//     index is the given char.
//

bool HumdrumLine::equalChar(int index, char ch) const {
	if ((int)size() <= index) {
		return false;
	}
	if (index < 0) {
		return false;
	}
	if (((string)(*this))[index] == ch) {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// HumdrumLine::isKernBoundaryStart -- Return true if the
//    line does not have any null tokens in **kern data which
//    refer to data tokens above the line.
//

bool HumdrumLine::isKernBoundaryStart(void) const {
	if (!isData()) {
		return false;
	}
	for (int i=0; i<getFieldCount(); i++) {
		if (!token(i)->isDataType("**kern")) {
			continue;
		}
		if (token(i)->isNull()) {
			return false;
		}
	}
	return true;
}



//////////////////////////////
//
// HumdrumLine::isKernBoundaryEnd -- Return true if the next
//    data line contains no null tokens in the **kern spines.
//    Assuming that a **kern spine split always starts with
//    a non-null token.
//

bool HumdrumLine::isKernBoundaryEnd(void) const {
	if (!isData()) {
		return false;
	}
	HTp ntok;
	for (int i=0; i<getFieldCount(); i++) {
		if (!token(i)->isDataType("**kern")) {
			continue;
		}
		ntok = token(i)->getNextToken();
		if (ntok == NULL) {
			continue;
		}
		while ((ntok != NULL) && !ntok->isData()) {
			ntok = ntok->getNextToken();
		}
		if (ntok == NULL) {
			continue;
		}
		if (ntok->isNull()) {
			return false;
		}
	}
	return true;
}



//////////////////////////////
//
// HumdrumLine::isComment -- Returns true if the first character
//   in the string is '!'. Could be local, global, or a reference record.
//

bool HumdrumLine::isComment(void) const {
	return equalChar(0, '!');
}



//////////////////////////////
//
// HumdrumLine::isCommentLocal -- Returns true if a local comment.
//

bool HumdrumLine::isCommentLocal(void) const {
	return equalChar(0, '!') && !equalChar(1, '!');
}



//////////////////////////////
//
// HumdrumLine::isCommentGlobal -- Returns true if a local comment.
//

bool HumdrumLine::isCommentGlobal(void) const {
	return equalChar(0, '!') && equalChar(1, '!');
}



//////////////////////////////
//
// HumdrumLine::isReference -- Returns true if a reference record.
//

bool HumdrumLine::isReference(void) const {

	if (this->size() < 5) {
		return false;
	}
	if (this->substr(0, 3) != "!!!") {
		return false;
	}
	if ((*this)[3] == '!') {
		return false;
	}
	int spaceloc = (int)this->find(" ");
	int tabloc = (int)this->find("\t");
	int colloc = (int)this->find(":");
	if (colloc == (int)string::npos) {
		return false;
	}
	if ((spaceloc != (int)string::npos) && (spaceloc < colloc)) {
		return false;
	}
	if ((tabloc != (int)string::npos) && (tabloc < colloc)) {
		return false;
	}
	return true;
}



//////////////////////////////
//
// HumdrumLine::getReferenceKey -- Return reference key if a reference
//     record.  Otherwise returns an empty string.
//

string HumdrumLine::getReferenceKey(void) const {
	if (this->size() < 5) {
		return "";
	}
	if (this->substr(0, 3) != "!!!") {
		return "";
	}
	if ((*this)[3] == '!') {
		return "";
	}
	int spaceloc = (int)this->find(" ");
	int tabloc = (int)this->find("\t");
	int colloc = (int)this->find(":");
	if (colloc == (int)string::npos) {
		return "";
	}
	if ((spaceloc != (int)string::npos) && (spaceloc < colloc)) {
		return "";
	}
	if ((tabloc != (int)string::npos) && (tabloc < colloc)) {
		return "";
	}
	return this->substr(3, colloc - 3);
}



//////////////////////////////
//
// HumdrumLine::getReferenceValue -- Return reference value if a reference
//     record.  Otherwise returns an empty string.
//

string HumdrumLine::getReferenceValue(void) const {
	if (this->size() < 5) {
		return "";
	}
	if (this->substr(0, 3) != "!!!") {
		return "";
	}
	if ((*this)[3] == '!') {
		return "";
	}
	int spaceloc = (int)this->find(" ");
	int tabloc = (int)this->find("\t");
	int colloc = (int)this->find(":");
	if (colloc == (int)string::npos) {
		return "";
	}
	if ((spaceloc != (int)string::npos) && (spaceloc < colloc)) {
		return "";
	}
	if ((tabloc != (int)string::npos) && (tabloc < colloc)) {
		return "";
	}
	return Convert::trimWhiteSpace(this->substr(colloc+1));
}



//////////////////////////////
//
// HumdrumLine::isExclusive -- Returns true if the first two characters
//     are "**".
//

bool HumdrumLine::isExclusive(void) const {
	return equalChar(1, '*') && equalChar(0, '*');
}



//////////////////////////////
//
// HumdrumLine::isTerminator -- Returns true if all tokens on the line
//    are terminators.
//

bool HumdrumLine::isTerminator(void) const {
	if (getTokenCount() == 0) {
		// if tokens have not been parsed, check line text
		return equalChar(1, '!') && equalChar(0, '*');
	}
	for (int i=0; i<getTokenCount(); i++) {
		if (!token(i)->isTerminator()) {
			return false;
		}
	}
	return true;
}



//////////////////////////////
//
// HumdrumLine::isInterp -- Returns true if starts with '*' character.
//

bool HumdrumLine::isInterp(void) const {
	return equalChar(0, '*');
}



//////////////////////////////
//
// HumdrumLine::isBarline -- Returns true if starts with '=' character.
//

bool HumdrumLine::isBarline(void) const {
	return equalChar(0, '=');
}



//////////////////////////////
//
// HumdrumLine::isData -- Returns true if data (but not measure).
//

bool HumdrumLine::isData(void) const {
	if (isComment() || isInterp() || isBarline() || isEmpty()) {
		return false;
	} else {
		return true;
	}
}



//////////////////////////////
//
// HumdrumLine::isAllNull -- Returns true if all tokens on the line
//    are null ("." if a data line, "*" if an interpretation line, "!"
//    if a local comment line).
//

bool HumdrumLine::isAllNull(void) const {
	if (!hasSpines()) {
		return false;
	}
	for (int i=0; i<getTokenCount(); i++) {
		if (!token(i)->isNull()) {
			return false;
		}
	}
	return true;
}



//////////////////////////////
//
// HumdrumLine::isAllRhythmicNull -- Returns true if all rhythmic
//    data-type tokens on the line are null ("." if a data line,
//    "*" if an interpretation line, "!" if a local comment line).
//

bool HumdrumLine::isAllRhythmicNull(void) const {
	if (!hasSpines()) {
		return false;
	}
	for (int i=0; i<getTokenCount(); i++) {
		if (!token(i)->hasRhythm()) {
			continue;
		}
		if (!token(i)->isNull()) {
			return false;
		}
	}
	return true;
}



//////////////////////////////
//
// HumdrumLine::setLineIndex -- Used by the HumdrumFileBase class to set the
//   index number of the line in the data storage for the file.
//

void HumdrumLine::setLineIndex(int index) {
	m_lineindex = index;
}



//////////////////////////////
//
// HumdrumLine::getLineIndex -- Returns the index number of the line in the
//    HumdrumFileBase storage for the lines.
//

int HumdrumLine::getLineIndex(void) const {
	return m_lineindex;
}



//////////////////////////////
//
// HumdrumLine::getLineNumber -- Returns the line index plus one.
//

int HumdrumLine::getLineNumber(void) const {
	return m_lineindex + 1;
}



//////////////////////////////
//
// HumdrumLine::getDuration -- Get the duration of the line.  The duration will
//    be negative one if rhythmic analysis in HumdrumFileStructure has not been
//    done on the owning HumdrumFile object.  Otherwise this is the duration of
//    the current line in the file.
//

HumNum HumdrumLine::getDuration(void) const {
	return m_duration;
}


HumNum HumdrumLine::getDuration(HumNum scale) const {
	return m_duration * scale;
}



//////////////////////////////
//
// HumdrumLine::getBarlineDuration -- Return the duration following a barline,
//    or the duration of the previous barline in the data.
//

HumNum HumdrumLine::getBarlineDuration(void) const {
	if (isBarline()) {
		return getDurationToBarline();
	} else {
		return getDurationFromBarline() + getDurationToBarline();
	}
}


HumNum HumdrumLine::getBarlineDuration(HumNum scale) const {
	if (isBarline()) {
		return getDurationToBarline(scale);
	} else {
		return getDurationFromBarline(scale) + getDurationToBarline(scale);
	}
}



//////////////////////////////
//
// HumdrumLine::setDurationFromStart -- Sets the duration from the start of the
//    file to the start of the current line.  This is used in rhythmic
//    analysis done in the HumdrumFileStructure class.
//

void HumdrumLine::setDurationFromStart(HumNum dur) {
	m_durationFromStart = dur;
}



//////////////////////////////
//
// HumdrumLine::getDurationFromStart -- Get the duration from the start of the
//    file to the start of the current line.  This will be -1 if rhythmic
//    analysis has not been done in the HumdrumFileStructure class.
//

HumNum HumdrumLine::getDurationFromStart(void) const {
	return m_durationFromStart;
}


HumNum HumdrumLine::getDurationFromStart(HumNum scale) const {
	return m_durationFromStart * scale;
}



//////////////////////////////
//
// HumdrumLine::getDurationToEnd -- Returns the duration from the start of the
//    line to the end of the HumdrumFile which owns this HumdrumLine.  The
//    rhythm of the HumdrumFile must be analyze before using this function;
//    otherwise a 0 will probably be returned.
//

HumNum HumdrumLine::getDurationToEnd(void) const {
	if (m_owner == NULL) {
		return 0;
	}
	return ((HumdrumFile*)m_owner)->getScoreDuration() -  m_durationFromStart;
}


HumNum HumdrumLine::getDurationToEnd(HumNum scale) const {
	if (m_owner == NULL) {
		return 0;
	}
	return scale * (((HumdrumFile*)m_owner)->getScoreDuration() -
		m_durationFromStart);
}



//////////////////////////////
//
// HumdrumLine::getDurationFromBarline -- Returns the duration from the start
//    of the given line to the first barline occurring before the given line.
//    Analysis of this data is found in HumdrumFileStructure::metricAnalysis.
//

HumNum HumdrumLine::getDurationFromBarline(void) const {
	return m_durationFromBarline;
}


HumNum HumdrumLine::getDurationFromBarline(HumNum scale) const {
	return m_durationFromBarline * scale;
}



//////////////////////////////
//
// HumdrumLine::getTrackStart --  Returns the starting exclusive interpretation
//    for the given spine/track.
//

HTp HumdrumLine::getTrackStart(int track) const {
	if (m_owner == NULL) {
		return NULL;
	} else {
		return ((HumdrumFile*)m_owner)->getTrackStart(track);
	}
}



//////////////////////////////
//
// HumdrumLine::setDurationFromBarline -- Time from the previous
//    barline to the current line.  This function is used in analyzeMeter in
//    the HumdrumFileStructure class.
//

void HumdrumLine::setDurationFromBarline(HumNum dur) {
	m_durationFromBarline = dur;
}



//////////////////////////////
//
// HumdrumLine::getDurationToBarline -- Time from the starting of the
//   current note to the next barline.
//

HumNum HumdrumLine::getDurationToBarline(void) const {
	return m_durationToBarline;
}


HumNum HumdrumLine::getDurationToBarline(HumNum scale) const {
	return m_durationToBarline * scale;
}



//////////////////////////////
//
// HumdrumLine::getBeat -- Returns the beat number for the data on the
//     current line given the input **recip representation for the duration
//     of a beat.  The beat in a measure is offset from 1 (first beat is
//     1 rather than 0).
//  Default value: beatrecip = "4".
//  Default value: beatdur   = 1.
//

HumNum HumdrumLine::getBeat(HumNum beatdur) const {
	if (beatdur.isZero()) {
		return beatdur;
	}
	HumNum beat = (getDurationFromBarline() / beatdur) + 1;
	return beat;
}


HumNum HumdrumLine::getBeat(string beatrecip) const {
	HumNum beatdur = Convert::recipToDuration(beatrecip);
	if (beatdur.isZero()) {
		return beatdur;
	}
	HumNum beat = (getDurationFromBarline() / beatdur) + 1;
	return beat;
}



//////////////////////////////
//
// HumdrumLine::setDurationToBarline -- Sets the duration from the current
//     line to the next barline in the score.  This function is used by
//     analyzeMeter in the HumdrumFileStructure class.
//

void HumdrumLine::setDurationToBarline(HumNum dur) {
	m_durationToBarline = dur;
}



//////////////////////////////
//
// HumdrumLine::setDuration -- Sets the duration of the line.  This is done
//   in the rhythmic analysis for the HumdurmFileStructure class.
//

void HumdrumLine::setDuration(HumNum aDur) {
	if (aDur.isNonNegative()) {
		m_duration = aDur;
	} else {
		m_duration = 0;
	}
}



//////////////////////////////
//
// HumdrumLine::hasSpines -- Returns true if the line contains spines.  This
//   means the the line is not empty or a global comment (which can include
//   reference records.
//

bool HumdrumLine::hasSpines(void) const {
	return (isEmpty() || isCommentGlobal()) ? false : true;
}



//////////////////////////////
//
// HumdrumLine::isGlobal -- Returns true if the line is a global record: either
//   and empty record, a global comment or a reference record.
//

bool HumdrumLine::isGlobal(void) const {
	return !hasSpines();
}



//////////////////////////////
//
// HumdrumLine::isManipulator -- Returns true if any tokens on the line are
//   manipulator interpretations.  Only null interpretations are allowed on
//   lines which contain manipulators, but the parser currently does not
//   enforce this rule.
//

bool HumdrumLine::isManipulator(void) const {
	for (int i=0; i<(int)m_tokens.size(); i++) {
		if (m_tokens[i]->isManipulator()) {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// HumdrumLine::isEmpty -- Returns true if no characters on line.  A blank line
//   is technically disallowed in the classic Humdrum Toolkit programs, but it
//   is usually tolerated.  In humlib (and HumdrumExtras) empty lines with
//   no content (not even space characters) are allowed and treated as a
//   special class of line.
//

bool HumdrumLine::isEmpty(void) const {
	return (size() == 0) ? true : false;
}



//////////////////////////////
//
// HumdrumLine::getTokenCount --  Returns the number of tokens on the line.
//     This value is set by HumdrumFileBase in analyzeTokens.
//

int HumdrumLine::getTokenCount(void) const {
	return (int)m_tokens.size();
}



//////////////////////////////
//
// HumdrumLine::token -- Returns a reference to the given token on the line.
//    An invalid token index would be bad to give to this function as it
//    returns a reference rather than a pointer (which could be set to
//    NULL if invalid).  Perhaps this function will eventually throw an
//    error if the index is out of bounds.
//

HTp HumdrumLine::token(int index) const {
	return m_tokens[index];
}



//////////////////////////////
//
// HumdrumLine::getTokenString -- Returns a copy of the string component of
//     a token.  This code will return a segmentation fault if index is out of
//     range...
//

string HumdrumLine::getTokenString(int index) const {
	return (string(*m_tokens[index]));
}


//////////////////////////////
//
// HumdrumLine::createTokensFromLine -- Chop up a HumdrumLine string into
//     individual tokens.
//

int HumdrumLine::createTokensFromLine(void) {
	// delete previous tokens (will need to re-analyze structure
	// of file after this).
	for (int i=0; i < (int)m_tokens.size(); i++) {
		delete m_tokens[i];
		m_tokens[i] = NULL;
	}
	m_tokens.resize(0);
	HTp token;
	char ch;
	string tstring;

	if (this->size() == 0) {
		token = new HumdrumToken();
		token->setOwner(this);
		m_tokens.push_back(token);
	} else if (this->compare(0, 2, "!!") == 0) {
		token = new HumdrumToken(this->c_str());
		token->setOwner(this);
		m_tokens.push_back(token);
	} else {
		for (int i=0; i<(int)size(); i++) {
			ch = getChar(i);
			if (ch == '\t') {
				token = new HumdrumToken(tstring);
				token->setOwner(this);
				m_tokens.push_back(token);
				tstring.clear();
			} else {
				tstring += ch;
			}
		}
	}
	if (tstring.size() > 0) {
		token = new HumdrumToken(tstring);
		token->setOwner(this);
		m_tokens.push_back(token);
		tstring.clear();
	}

	return (int)m_tokens.size();
}



//////////////////////////////
//
// HumdrumLine::createLineFromTokens --  Re-generate a HumdrumLine string from
//    individual tokens on the line.  This function will be necessary to
//    run before printing a HumdrumFile if you have changed any tokens on the
//    line.  Otherwise, changes in the tokens will not be passed on to the
///   printing of the line.
//

void HumdrumLine::createLineFromTokens(void) {
	string& iline = *this;
	iline.clear();
	for (int i=0; i<(int)m_tokens.size(); i++) {
		iline += (string)(*m_tokens[i]);
		if (i < (int)m_tokens.size() - 1) {
			iline += '\t';
		}
	}
}



//////////////////////////////
//
// HumdrumLine::getTokens -- Returns an array of tokens pointers for a
//   Humdrum line.  This function should not be called on global comments,
//   reference records (which are a sub-cateogry of global comments).  This
//   is because a line's type may contain tabs which are not representing
//   token separators.  Empty lines are ok to input: the output token
//   list will contain one empty string.
//

void HumdrumLine::getTokens(vector<HTp>& list) {
	if (m_tokens.size() == 0) {
		createTokensFromLine();
	}
	list = m_tokens;
}



//////////////////////////////
//
// HumdrumLine::getChar -- Returns character at given index in string, or
//    null if out of range.
//

char HumdrumLine::getChar(int index) const {
	if (index < 0) {
		return '\0';
	}
	if (index >= (int)size()) {
		return '\0';
	}
	return (((string)(*this))[index]);
}



//////////////////////////////
//
// HumdrumLine::printSpineInfo -- Print the spine state information of
//    each token in a file.  Useful for debugging.  The spine info
//    is the track number, such as "1".  When the track splits into
//    subtracks, then there will be two subtracks: "(1)a" and "(1)b".
//    If the second of those subtracks splits again, then its subtracks
//    will be "((1)b)a" and "((1)b)b". If two different tracks merge, such
//    as "1" and "(2)a", then the spine info will be "1 (2)a".
//
// default value: out = cout
//

ostream& HumdrumLine::printSpineInfo(ostream& out) {
	if (isManipulator()) {
		out << *this;
	} else {
		for (int i=0; i<(int)m_tokens.size(); i++) {
			out << m_tokens[i]->getSpineInfo();
			if (i < (int)m_tokens.size() - 1) {
				out << '\t';
			}
		}
	}
	return out;
}



//////////////////////////////
//
// HumdrumLine::printDataTypeInfo -- Print the datatype of each token in
//     the file.  Useful for debugging.  The datatype prefix "**" is removed;
//     otherwise, it is given when you call HumdrumToken::getDataType().
//
// default value: out = cout
//

ostream& HumdrumLine::printDataTypeInfo(ostream& out) {
	if (isManipulator()) {
		out << *this;
	} else {
		for (int i=0; i<(int)m_tokens.size(); i++) {
			out << m_tokens[i]->getDataType().substr(2, string::npos);
			if (i < (int)m_tokens.size() - 1) {
				out << '\t';
			}
		}
	}
	return out;
}



//////////////////////////////
//
// HumdrumLine::analyzeTokenDurations -- Calculate the duration of
//    all tokens on a line.
//

bool HumdrumLine::analyzeTokenDurations(string& err) {
	if (!hasSpines()) {
		return !err.size();
	}
	for (int i=0; i<(int)m_tokens.size(); i++) {
		if (!m_tokens[i]->analyzeDuration(err)) {
			return !err.size();
		}
	}
	return !err.size();
}



//////////////////////////////
//
// HumdrumLine::analyzeTracks -- Calculate the subtrack info for subspines.
//   Subtracks index subspines strictly from left to right on the line.
//   Subspines can be exchanged and be represented left to right out of
//   original order.
//

bool HumdrumLine::analyzeTracks(string& err) {
	if (!hasSpines()) {
		return !err.size();
	}

	string info;
	int track;
	int maxtrack = 0;
	int i, j, k;

	for (i=0; i<(int)m_tokens.size(); i++) {
		info = m_tokens[i]->getSpineInfo();
		track = 0;
		for (j=0; j<(int)info.size(); j++) {
			if (!isdigit(info[j])) {
				continue;
			}
			track = info[j] - '0';
			for (k=j+1; k<(int)info.size(); k++) {
				if (isdigit(info[k])) {
					track = track * 10 + (info[k] - '0');
				} else {
					break;
				}
			}
			break;
		}
		if (maxtrack < track) {
			maxtrack = track;
		}
		m_tokens[i]->setTrack(track);
	}

	int subtrack;
	vector<int> subtracks;
	vector<int> cursub;

	subtracks.resize(maxtrack+1);
	cursub.resize(maxtrack+1);
	fill(subtracks.begin(), subtracks.end(), 0);
	fill(cursub.begin(), cursub.end(), 0);

	for (i=0; i<(int)m_tokens.size(); i++) {
		subtracks[m_tokens[i]->getTrack()]++;
	}
	for (i=0; i<(int)m_tokens.size(); i++) {
		track = m_tokens[i]->getTrack();
		subtrack = subtracks[track];
		if (subtrack > 1) {
			m_tokens[i]->setSubtrack(++cursub[m_tokens[i]->getTrack()]);
		} else {
			m_tokens[i]->setSubtrack(0);
		}
		m_tokens[i]->setSubtrackCount(subtracks[track]);
	}
	return !err.size();
}



//////////////////////////////
//
// HumdrumLine::printDurationInfo -- Print the analyzed duration of each
//     token in a file (for debugging).  If a token has an undefined
//     duration, then its duration is -1.  If a token represents
//     a grace note, then its duration is 0 (regardless of whether it
//     includes a visual duration).
// default value: out = cout
//

ostream& HumdrumLine::printDurationInfo(ostream& out) {
	if (isManipulator()) {
		out << *this;
	} else {
		for (int i=0; i<(int)m_tokens.size(); i++) {
			m_tokens[i]->getDuration().printMixedFraction(out);
			if (i < (int)m_tokens.size() - 1) {
				out << '\t';
			}
		}
	}
	return out;
}


//////////////////////////////
//
// HumdrumLine::printCsv -- print the line as a CSV
//    (comma separate value) line.
// default value: out = std::cout;
// default value: separator = ","
//

ostream& HumdrumLine::printCsv(ostream& out, const string& separator) {
	for (int i=0; i<getFieldCount(); i++) {
		token(i)->printCsv(out);
		if (i<getFieldCount()-1) {
			out << separator;
		}
	}
	out << endl;
	return out;
}



//////////////////////////////
//
// HumdrumLine::printXml -- Print the HumdrumLine as a XML element.
//

ostream& HumdrumLine::printXml(ostream& out, int level, const string& indent) {

	if (hasSpines()) {
		out << Convert::repeatString(indent, level) << "<frame";
		out << " n=\"" << getLineIndex() << "\"";
		out << " xml:id=\"" << getXmlId() << "\"";
		out << ">\n";
		level++;

		out << Convert::repeatString(indent, level) << "<frameInfo>\n";
		level++;

		out << Convert::repeatString(indent, level) << "<fieldCount>";
		out << getTokenCount() << "</fieldCount>\n";

		out << Convert::repeatString(indent, level);
		out << "<frameStart";
		out << Convert::getHumNumAttributes(getDurationFromStart());
		out << "/>\n";

		out << Convert::repeatString(indent, level);
		out << "<frameDuration";
		out << Convert::getHumNumAttributes(getDuration());
		out << "/>\n";

		out << Convert::repeatString(indent, level) << "<frameType>";
		if (isData()) {
			out << "data";
		} else if (isBarline()) {
			out << "barline";
		} else if (isInterpretation()) {
			out << "interpretation";
		} else if (isLocalComment()) {
			out << "local-comment";
		}
		out << "</frameType>\n";

		if (isBarline()) {
			// print the duration to the next barline or to the end of the score
			// if there is no barline at the end of the score.
			out << Convert::repeatString(indent, level);
			out << "<barlineDuration";
			out << Convert::getHumNumAttributes(getBarlineDuration());
			out << "/>\n";
		}

		bool bstart = isKernBoundaryStart();
		bool bend   = isKernBoundaryEnd();
		if (bstart || bend) {
			out << Convert::repeatString(indent, level);
			cout << "<kernBoundary";
			cout << " start=\"";
			if (bstart) {
				cout << "true";
			} else {
				cout << "false";
			}
			cout << "\"";
			cout << " end=\"";
			if (bend) {
				cout << "true";
			} else {
				cout << "false";
			}
			cout << "\"";
			cout << "/>\n";
		}

		level--;
		out << Convert::repeatString(indent, level) << "</frameInfo>\n";

		out << Convert::repeatString(indent, level) << "<fields>\n";
		level++;
		for (int i=0; i<getFieldCount(); i++) {
			token(i)->printXml(out, level, indent);
		}
		level--;
		out << Convert::repeatString(indent, level) << "</fields>\n";

		level--;
		out << Convert::repeatString(indent, level) << "</frame>\n";

	} else {
		// global comments, reference records, or blank lines print here.
		out << Convert::repeatString(indent, level) << "<metaFrame";
		out << " n=\"" << getLineIndex() << "\"";
		out << " token=\"" << Convert::encodeXml(((string)(*this))) << "\"";
		out << " xml:id=\"" << getXmlId() << "\"";
		out << ">\n";
		level++;

		out << Convert::repeatString(indent, level) << "<frameInfo>\n";
		level++;

		out << Convert::repeatString(indent, level);
		out << "<startTime";
		out << Convert::getHumNumAttributes(getDurationFromStart());
		out << "/>\n";

		out << Convert::repeatString(indent, level) << "<frameType>";
		if (isReference()) {
			out << "reference";
		} else if (isBlank()) {
			out << "empty";
		} else {
			out << "global-comment";
		}
		out << "</frameType>\n";

		if (isReference()) {
			out << Convert::repeatString(indent, level);
			string key = getReferenceKey();
			string language;
			string primaryLanguage;
			auto loc = key.find("@@");
			if (loc != string::npos) {
				language = key.substr(loc+2);
				key = key.substr(0, loc);
				primaryLanguage = "true";
			} else {
				loc = key.find("@");
				if (loc != string::npos) {
					language = key.substr(loc+1);
					key = key.substr(0, loc);
				}
			}

			out << "<referenceKey";
			if (language.size() > 0) {
				out << " language=\"" << Convert::encodeXml(language) << "\"";
			}
			if (primaryLanguage.size() > 0) {
				out << " primary=\"" << Convert::encodeXml(primaryLanguage) << "\"";
			}
			out << ">" << Convert::encodeXml(key);
			out << "</referenceKey>\n";

			out << Convert::repeatString(indent, level);
			out << "<referenceValue>" << Convert::encodeXml(getReferenceValue());
			out << "</referenceValue>\n";
		}

		level--;
		out << Convert::repeatString(indent, level) << "</frameInfo>\n";


		level--;
		out << Convert::repeatString(indent, level) << "</metaFrame>\n";
	}

	return out;
}



//////////////////////////////
//
// HumdrumLine::getXmlId -- Return a unique ID for the current line.
//

string HumdrumLine::getXmlId(const string& prefix) const {
	string output;
	if (prefix.size() > 0) {
		output = prefix;
	} else {
		output = getXmlIdPrefix();
	}
	output += "loc" + to_string(getLineIndex());
	return output;
}



//////////////////////////////
//
// HumdrumLine::getXmlIdPrefix -- Return the pre-set XML ID attribute
//     prefix from the owning HumdrumFile object.
//

string HumdrumLine::getXmlIdPrefix(void) const {
	if (m_owner == NULL) {
		return "";
	}
	return ((HumdrumFileBase*)m_owner)->getXmlIdPrefix();
}



//////////////////////////////
//
// HumdrumLine::printTrackInfo -- Print the analyzed track information.
//     The first (left-most) spine in a Humdrum file is track 1, the
//     next is track 2, etc.  The track value is shared by all subspines,
//     so there may be duplicate track numbers on a line if the spine
//     has split.  When the spine splits, a subtrack number is given
//     after a "." character in the printed output from this function.
//     Subtrack==0 means that there is only one subtrack.
//     Examples:
//         "1"  == Track 1, subtrack 1 (and there are no more subtracks)
//	        "1.1" == Track 1, subtrack 1 (and there are more subtracks)
//	        "1.2" == Track 1, subtrack 2 (and there may be more subtracks)
//	        "1.10" == Track 1, subtrack 10 (and there may be subtracks)
//     Each starting exclusive interpretation is assigned to a unique
//     track number.  When a *+ manipulator is given, the new exclusive
//     interpretation on the next line is give the next higher track
//     number.
//
// default value: out = cout
//

ostream& HumdrumLine::printTrackInfo(ostream& out) {
	if (isManipulator()) {
		out << *this;
	} else {
		for (int i=0; i<(int)m_tokens.size(); i++) {
			out << m_tokens[i]->getTrackString();
			if (i < (int)m_tokens.size() - 1) {
				out << '\t';
			}
		}
	}
	return out;
}



//////////////////////////////
//
// HumdrumLine::setOwner -- store a pointer to the HumdrumFile which
//    manages (owns) this object.
//

void HumdrumLine::setOwner(void* hfile) {
	m_owner = hfile;
}



//////////////////////////////
//
// HumdrumLine::getOwner -- Return the HumdrumFile which manages
//   (owns) this line.
//

HumdrumFile* HumdrumLine::getOwner(void) {
	return (HumdrumFile*)m_owner;
}



//////////////////////////////
//
// HumdrumLine::setParameters -- Takes a global comment with
//     the structure:
//        !!NS1:NS2:key1=value1:key2=value2:key3=value3
//     and stores it in the HumHash parent class of the line.
//

void HumdrumLine::setParameters(HumdrumLine* pLine) {
	HumdrumLine& pl = *pLine;
	if (pl.size() <= 2) {
		return;
	}
	string pdata = pLine->substr(2, pl.size()-2);
	setParameters(pdata);
}


void HumdrumLine::setParameters(const string& pdata) {
	vector<string> pieces = Convert::splitString(pdata, ':');
	if (pieces.size() < 3) {
		return;
	}
	string ns1 = pieces[0];
	string ns2 = pieces[1];
	string key;
	string value;
	int loc;
	for (int i=2; i<(int)pieces.size(); i++) {
		Convert::replaceOccurrences(pieces[i], "&colon;", ":");
		loc = (int)pieces[i].find("=");
		if (loc != (int)string::npos) {
			key   = pieces[i].substr(0, loc);
			value = pieces[i].substr(loc+1, pieces[i].size());
		} else {
			key   = pieces[i];
			value = "true";
		}
		setValue(ns1, ns2, key, value);
	}
}



//////////////////////////////
//
// HumdrumLine::appendToken -- add a token at the end of the current
//      list of tokens in the line.
//

void HumdrumLine::appendToken(HTp token) {
	// deletion will be handled by class.
	m_tokens.push_back(token);
}


void HumdrumLine::appendToken(const HumdrumToken& token) {
	HTp newtok = new HumdrumToken(token);
	m_tokens.push_back(newtok);
}


void HumdrumLine::appendToken(const string& token) {
	HTp newtok = new HumdrumToken(token);
	m_tokens.push_back(newtok);
}


void HumdrumLine::appendToken(const char* token) {
	HTp newtok = new HumdrumToken(token);
	m_tokens.push_back(newtok);
}



//////////////////////////////
//
// HumdrumLine::getKernNoteAttacks -- Return the number of kern notes
//    that attack on a line.
//

int HumdrumLine::getKernNoteAttacks(void) {
	int output = 0;
	for (int i=0; i<getFieldCount(); i++) {
		if (!token(i)->isKern()) {
			continue;
		}
		if (token(i)->isNoteAttack()) {
			output++;
		}
	}
	return output;
}



//////////////////////////////
//
// HumdrumLine::insertToken -- Add a token before the given token position.
//

void HumdrumLine::insertToken(int index, HTp token) {
	// Warning: deletion will be handled by class.  Don't insert if it
	// already belongs to another HumdrumLine or HumdrumFile.
	m_tokens.insert(m_tokens.begin() + index, token);
}


void HumdrumLine::insertToken(int index, const HumdrumToken& token) {
	HTp newtok = new HumdrumToken(token);
	m_tokens.insert(m_tokens.begin() + index, newtok);
}


void HumdrumLine::insertToken(int index, const string& token) {
	HTp newtok = new HumdrumToken(token);
	m_tokens.insert(m_tokens.begin() + index, newtok);
}


void HumdrumLine::insertToken(int index, const char* token) {
	HTp newtok = new HumdrumToken(token);
	m_tokens.insert(m_tokens.begin() + index, newtok);
}



//////////////////////////////
//
// HumdrumLine::appendToken -- Add a token after the given token position.
//

void HumdrumLine::appendToken(int index, HTp token) { 
	HumdrumLine::insertToken(index+1, token);
}


void HumdrumLine::appendToken(int index, const HumdrumToken& token) {
	HumdrumLine::insertToken(index+1, token);
}


void HumdrumLine::appendToken(int index, const string& token) {
	HumdrumLine::insertToken(index+1, token);
}


void HumdrumLine::appendToken(int index, const char* token) {
	HumdrumLine::insertToken(index+1, token);
}



//////////////////////////////
//
// operator<< -- Print a HumdrumLine. Needed to avoid interaction with
//     HumHash parent class.
//

ostream& operator<<(ostream& out, HumdrumLine& line) {
	out << (string)line;
	return out;
}

ostream& operator<< (ostream& out, HumdrumLine* line) {
	out << (string)(*line);
	return out;
}



// spine mainipulators:
#define SPLIT_TOKEN       "*^"
#define MERGE_TOKEN       "*v"
#define EXCHANGE_TOKEN    "*x"
#define TERMINATE_TOKEN   "*-"
#define ADD_TOKEN         "*+"
// Also exclusive interpretations which start "**" followed by the data type.

// other special tokens:
#define NULL_DATA            "."
#define NULL_INTERPRETATION  "*"
#define NULL_COMMENT_LOCAL   "!"
#define NULL_COMMENT_GLOBAL  "!!"



//////////////////////////////
//
// HumdrumToken::HumdrumToken -- Constructor for HumdrumToken.
//

HumdrumToken::HumdrumToken(void) : string() {
	m_rhycheck = 0;
	setPrefix("!");
	m_strand = -1;
	m_nullresolve = NULL;
}


HumdrumToken::HumdrumToken(const string& aString) : string(aString) {
	m_rhycheck = 0;
	setPrefix("!");
	m_strand = -1;
	m_nullresolve = NULL;
}


HumdrumToken::HumdrumToken(const char* aString) : string(aString) {
	m_rhycheck = 0;
	setPrefix("!");
	m_strand = -1;
	m_nullresolve = NULL;
}


HumdrumToken::HumdrumToken(const HumdrumToken& token) :
		string((string)token), HumHash((HumHash)token) {
	m_address         = token.m_address;
	m_address.m_owner = NULL;
	m_duration        = token.m_duration;
	m_nextTokens      = token.m_nextTokens;
	m_previousTokens.clear();
	m_nextNonNullTokens.clear();
	m_previousNonNullTokens.clear();
	m_rhycheck        = token.m_rhycheck;
	m_strand          = -1;
	m_nullresolve     = NULL;
	setPrefix(token.getPrefix());
}


HumdrumToken::HumdrumToken(HumdrumToken* token) :
		string((string)(*token)), HumHash((HumHash)(*token)) {
	m_address         = token->m_address;
	m_address.m_owner = NULL;
	m_duration        = token->m_duration;
	m_nextTokens      = token->m_nextTokens;
	m_previousTokens.clear();
	m_nextNonNullTokens.clear();
	m_previousNonNullTokens.clear();
	m_rhycheck        = token->m_rhycheck;
	m_strand          = -1;
	m_nullresolve     = NULL;
	setPrefix(token->getPrefix());
}



HumdrumToken::HumdrumToken(const HumdrumToken& token, HumdrumLine* owner) :
		string((string)token), HumHash((HumHash)token) {
	m_address         = token.m_address;
	m_address.m_owner = owner;
	m_duration        = token.m_duration;
	m_nextTokens      = token.m_nextTokens;
	m_previousTokens.clear();
	m_nextNonNullTokens.clear();
	m_previousNonNullTokens.clear();
	m_rhycheck        = token.m_rhycheck;
	m_strand          = -1;
	m_nullresolve     = NULL;
	setPrefix(token.getPrefix());
}


HumdrumToken::HumdrumToken(HumdrumToken* token, HumdrumLine* owner) :
		string((string)(*token)), HumHash((HumHash)(*token)) {
	m_address         = token->m_address;
	m_address.m_owner = owner;
	m_duration        = token->m_duration;
	m_nextTokens      = token->m_nextTokens;
	m_previousTokens.clear();
	m_nextNonNullTokens.clear();
	m_previousNonNullTokens.clear();
	m_rhycheck        = token->m_rhycheck;
	m_strand          = -1;
	m_nullresolve     = NULL;
	setPrefix(token->getPrefix());
}



//////////////////////////////
//
// HumdrumToken::operator= -- Copy operator.
//

HumdrumToken& HumdrumToken::operator=(HumdrumToken& token) {
	if (this == &token) {
		return *this;
	}
	(string)(*this)   = (string)token;
	(HumHash)(*this)  = (HumHash)token;

	m_address         = token.m_address;
	m_address.m_owner = NULL;
	m_duration        = token.m_duration;
	m_nextTokens      = token.m_nextTokens;
	m_previousTokens.clear();
	m_nextNonNullTokens.clear();
	m_previousNonNullTokens.clear();
	m_rhycheck        = token.m_rhycheck;
	m_strand          = -1;
	m_nullresolve     = NULL;
	setPrefix(token.getPrefix());

	return *this;
}


HumdrumToken& HumdrumToken::operator=(const string& token) {
	(string)(*this) = token;

	m_address.m_owner = NULL;
	m_duration        = 0;
	m_nextTokens.clear();
	m_previousTokens.clear();
	m_nextNonNullTokens.clear();
	m_previousNonNullTokens.clear();
	m_rhycheck        = -1;
	m_strand          = -1;
	m_nullresolve     = NULL;
	setPrefix("!");

	return *this;
}


HumdrumToken& HumdrumToken::operator=(const char* token) {
	(string)(*this) = token;

	m_address.m_owner = NULL;
	m_duration        = 0;
	m_nextTokens.clear();
	m_previousTokens.clear();
	m_nextNonNullTokens.clear();
	m_previousNonNullTokens.clear();
	m_rhycheck        = -1;
	m_strand          = -1;
	m_nullresolve     = NULL;
	setPrefix("!");

	return *this;
}



//////////////////////////////
//
// HumdrumToken::~HumdrumToken -- Deconstructor for HumdrumToken.
//

HumdrumToken::~HumdrumToken() {
	// do nothing
}


//////////////////////////////
//
// HumdrumToken::equalChar -- Returns true if the character at the given
//     index is the given char.
//

bool HumdrumToken::equalChar(int index, char ch) const {
	if ((int)size() <= index) {
		return false;
	}
	if (index < 0) {
		return false;
	}
	if (((string)(*this))[index] == ch) {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// HumdrumToken::getPreviousNonNullDataTokenCount -- Returns the number of
//   previous tokens in the spine which is not a null token.  For null
//   tokens, this will be a count of the number of non-null tokens which
//   the null represents.
// @SEEALSO: getPreviousNonNullDataToken
//

int HumdrumToken::getPreviousNonNullDataTokenCount(void) {
	return (int)m_previousNonNullTokens.size();
}



//////////////////////////////
//
// HumdrumToken::getPreviousNonNullDataToken -- Returns the non-null
//    data token which occurs before this token in the data in the same
//    spine.  The default value is index 0, since mostly there will only
//    be one previous token.
//

HumdrumToken* HumdrumToken::getPreviousNonNullDataToken(int index) {
	if (index < 0) {
		index += (int)m_previousNonNullTokens.size();
	}
	if (index < 0) {
		return NULL;
	}
	if (index >= (int)m_previousNonNullTokens.size()) {
		return NULL;
	}
	return m_previousNonNullTokens[index];
}



//////////////////////////////
//
// HumdrumToken::getNextNonNullDataTokenCount -- Returns the number of non-null
//     data tokens which follow this token in the spine.
//

int HumdrumToken::getNextNonNullDataTokenCount(void) {
	return (int)m_nextNonNullTokens.size();
}



//////////////////////////////
//
// HumdrumToken::getNextNonNullDataToken -- Returns the given next non-null token
//    following this one in the spine.  The default value for index is 0 since
//    the next non-null data token count will typically be 1.
// default value: index = 0
//

HumdrumToken* HumdrumToken::getNextNonNullDataToken(int index) {
	if (index < 0) {
		index += m_nextNonNullTokens.size();
	}
	if (index < 0) {
		return NULL;
	}
	if (index >= (int)m_nextNonNullTokens.size()) {
		return NULL;
	}
	return m_nextNonNullTokens[index];
}



//////////////////////////////
//
// HumdrumToken::getSlurDuration -- If the note has a slur start, then
//    returns the duration until the endpoint; otherwise, returns 0;
//    Expand later to handle slur ends and elided slurs.  The function
//    HumdrumFileContent::analyzeSlurs() should be called before accessing
//    this function.  If the slur duruation was already calculated, return
//    thave value; otherwise, calculate from the location of a matching
//    slur end.
//

HumNum HumdrumToken::getSlurDuration(HumNum scale) {
	if (!isDataType("**kern")) {
		return 0;
	}
	if (isDefined("auto", "slurDuration")) {
		return getValueFraction("auto", "slurDuration");
	} else if (isDefined("auto", "slurEnd")) {
		HTp slurend = getValueHTp("auto", "slurEnd");
		return slurend->getDurationFromStart(scale) -
				getDurationFromStart(scale);
	} else {
		return 0;
	}
}



//////////////////////////////
//
// HumdrumToken::getDataType -- Get the exclusive interpretation type for
//     the token.
// @SEEALSO: isDataType
//

const string& HumdrumToken::getDataType(void) const {
	return m_address.getDataType();
}



//////////////////////////////
//
// HumdrumToken::isDataType -- Returns true if the data type of the token
//   matches the test data type.
// @SEEALSO: getDataType getKern
//

bool HumdrumToken::isDataType(string dtype) const {
	if (dtype.compare(0, 2, "**") == 0) {
		return dtype == getDataType();
	} else {
		return getDataType().compare(2, string::npos, dtype) == 0;
	}
}



//////////////////////////////
//
// HumdrumToken::isKern -- Returns true if the data type of the token
//    is **kern.
// @SEEALSO: isDataType
//

bool HumdrumToken::isKern(void) const {
	return isDataType("**kern");
}



//////////////////////////////
//
// HumdrumToken::setSpineInfo -- Sets the spine manipulation history string.
// @SEEALTO: getSpineInfo
//

void HumdrumToken::setSpineInfo(const string& spineinfo) {
	m_address.setSpineInfo(spineinfo);
}



//////////////////////////////
//
// HumdrumToken::getSpineInfo -- Returns the spine split/merge history
//    for the token.
// @SEEALTO: setSpineInfo
//

string HumdrumToken::getSpineInfo(void) const {
	return m_address.getSpineInfo();
}



//////////////////////////////
//
// HumdrumToken::getLineIndex -- Returns the line index of the owning
//    HumdrumLine for this token.
// @SEEALTO: getLineNumber
//

int HumdrumToken::getLineIndex(void) const {
	return m_address.getLineIndex();
}



//////////////////////////////
//
// HumdrumToken::getFieldIndex -- Returns the index of the token the line.
// @SEEALSO: getFieldIndex
//

int HumdrumToken::getFieldIndex(void) const {
	return m_address.getFieldIndex();
}



//////////////////////////////
//
// HumdrumToken::getFieldNumber -- Returns the index of the token the line.
// @SEEALSO: getFieldNumber
//

int HumdrumToken::getFieldNumber(void) const {
	return m_address.getFieldIndex() + 1;
}



//////////////////////////////
//
// HumdrumToken::getTokenIndex -- Returns the index of the token the line.
// @SEEALSO: getTokenIndex
//

int HumdrumToken::getTokenIndex(void) const {
	return m_address.getFieldIndex();
}



//////////////////////////////
//
// HumdrumToken::getTokenNumber -- Returns the index of the token the line.
// @SEEALSO: getFieldNumber
//

int HumdrumToken::getTokenNumber(void) const {
	return m_address.getFieldIndex() + 1;
}



//////////////////////////////
//
// HumdrumToken::getLineNumber -- Returns the line index plus 1.
// @SEEALTO: getLineIndex
//

int HumdrumToken::getLineNumber(void) const {
	return m_address.getLineNumber();
}



//////////////////////////////
//
// HumdrumToken::setFieldIndex -- Sets the field index of the token on the
//   owning HumdrumLine object.
// @SEEALSO: getFieldIndex
//

void HumdrumToken::setFieldIndex(int index) {
	m_address.setFieldIndex(index);
}



//////////////////////////////
//
// HumdrumToken::setTrack -- Sets the track number (similar to a staff in MEI).
//     The two-parameter version will set the track and sub-track at the same
//     time (subtrack is similar to a staff and layer in MEI).
//

void HumdrumToken::setTrack(int aTrack) {
	m_address.setTrack(aTrack);
}


void HumdrumToken::setTrack(int aTrack, int aSubtrack) {
	setTrack(aTrack);
	setSubtrack(aSubtrack);
}



//////////////////////////////
//
// HumdrumToken::getTrack -- Get the track (similar to a staff in MEI).
//

int HumdrumToken::getTrack(void) const {
	return m_address.getTrack();
}



//////////////////////////////
//
// HumdrumToken::setSubtrack -- Sets the subtrack (similar to a layer
//    in MEI).
//

void HumdrumToken::setSubtrack(int aSubtrack) {
	m_address.setSubtrack(aSubtrack);
}



//////////////////////////////
//
// HumdrumToken::setSubtrackCount -- Sets the subtrack count in the
//    HumdrumLine for all tokens in the same track as the current
//    token.
//

void HumdrumToken::setSubtrackCount(int count) {
	m_address.setSubtrackCount(count);
}



//////////////////////////////
//
// HumdrumToken::setPreviousToken --
//

void HumdrumToken::setPreviousToken(HumdrumToken* token) {
	m_previousTokens.resize(1);
	m_previousTokens[0] = token;
}



//////////////////////////////
//
// HumdrumToken::setNextToken --
//

void HumdrumToken::setNextToken(HumdrumToken* token) {
	m_nextTokens.resize(1);
	m_nextTokens[0] = token;
}



//////////////////////////////
//
// HumdrumToken::addNextNonNullToken --
//

void HumdrumToken::addNextNonNullToken(HTp token) {
	if (token == NULL) {
		return;
	}
	for (int i=0; i<(int)m_nextNonNullTokens.size(); i++) {
		if (token == m_nextNonNullTokens[i]) {
			return;
		}
	}
	m_nextNonNullTokens.push_back(token);
	// maybe should sort by track/subspine order...
}



//////////////////////////////
//
// HumdrumToken::getNextToken -- Returns the next token in the
//    spine.  Since the next token count is usually one, the default
//    index value is zero.  When there is no next token (when the current
//    token is a spine terminaor), then NULL will be returned.
// default value: index = 0
// @SEEALSO: getNextTokens, getPreviousToken
//

HTp HumdrumToken::getNextToken(int index) const {
	if ((index >= 0) && (index < (int)m_nextTokens.size())) {
		return m_nextTokens[index];
	} else {
		return NULL;
	}
}



//////////////////////////////
//
// HumdrumToken::getNextTokens -- Returns a list of the next
//   tokens in the spine after this token.
// @SEEALSO: getNextToken
//

vector<HumdrumToken*> HumdrumToken::getNextTokens(void) const {
	return m_nextTokens;
}



//////////////////////////////
//
// HumdrumToken::getPreviousTokens -- Returns a list of the previous
//    tokens in the spine before this token.
//

vector<HumdrumToken*> HumdrumToken::getPreviousTokens(void) const {
	return m_previousTokens;
}



//////////////////////////////
//
// HumdrumToken::getPreviousToken -- Returns the previous token in the
//    spine.  Since the previous token count is usually one, the default
//    index value is zero.
// default value: index = 0
//

HumdrumToken* HumdrumToken::getPreviousToken(int index) const {
	if ((index >= 0) && (index < (int)m_previousTokens.size())) {
		return m_previousTokens[index];
	} else {
		return NULL;
	}
}


//////////////////////////////
//
// HumdrumToken::getNextFieldToken --
//

HTp HumdrumToken::getNextFieldToken(void) const {
	HumdrumLine* line = getLine();
	if (!line) {
		return NULL;
	}
	int field = getFieldIndex();
	if (field >= line->getFieldCount()  - 1) {
		return NULL;
	}
	return line->token(field+1);
}



//////////////////////////////
//
// HumdrumToken::getPreviousFieldToken --
//

HTp HumdrumToken::getPreviousFieldToken(void) const {
	HumdrumLine* line = getLine();
	if (!line) {
		return NULL;
	}
	int field = getFieldIndex();
	if (field < 1) {
		return NULL;
	}
	return line->token(field-1);
}



//////////////////////////////
//
// HumdrumToken::analyzeDuration -- Currently reads the duration of
//   **kern and **recip data.  Add more data types here such as **koto.
//

bool HumdrumToken::analyzeDuration(string& err) {
	if ((*this) == NULL_DATA) {
		m_duration.setValue(-1);
		return true;
	}
	if (equalChar(0 ,'!')) {
		m_duration.setValue(-1);
		return true;
	}
	if (equalChar(0 ,'*')) {
		m_duration.setValue(-1);
		return true;
	}
	if (equalChar(0 ,'=')) {
		m_duration.setValue(-1);
		return true;
	}
	string dtype = getDataType();
	if (hasRhythm()) {
		if (isData()) {
			if (!isNull()) {
				if (strchr(this->c_str(), 'q') != NULL) {
					m_duration = 0;
				} else {
					m_duration = Convert::recipToDuration((string)(*this));
				}
			} else {
				m_duration.setValue(-1);
			}
		} else {
			m_duration.setValue(-1);
		}
	} else {
		m_duration.setValue(-1);
	}
	return true;
}



///////////////////////////////
//
// HumdrumToken::isManipulator -- Returns true if token is one of:
//    SPLIT_TOKEN     = "*^"  == spine splitter
//    MERGE_TOKEN     = "*v"  == spine merger
//    EXCHANGE_TOKEN  = "*x"  == spine exchanger
//    ADD_TOKEN       = "*+"  == spine adder
//    TERMINATE_TOKEN = "*-"  == spine terminator
//    **...  == exclusive interpretation
//

bool HumdrumToken::isManipulator(void) const {
	if (isSplitInterpretation())     { return true; }
	if (isMergeInterpretation())     { return true; }
	if (isExchangeInterpretation())  { return true; }
	if (isAddInterpretation())       { return true; }
	if (isTerminateInterpretation()) { return true; }
	if (isExclusiveInterpretation()) { return true; }
	return false;
}



//////////////////////////////
//
// HumdrumToken::getDuration -- Returns the duration of the token.  The token
//    does not necessarily need to have any explicit duration, as the returned
//    value will also include implicit duration calculated in analyzeRhythm
//    in the HumdrumFileStructure class.
//

HumNum HumdrumToken::getDuration(void) const {
	return m_duration;
}


HumNum HumdrumToken::getDuration(HumNum scale) const {
	return m_duration * scale;
}



//////////////////////////////
//
// HumdrumToken::getTiedDuration -- Returns the duration of the token and any
//    tied notes attached to it.  Does not work well which chords.
//

HumNum HumdrumToken::getTiedDuration(void) {
	HumNum output = m_duration;
	if ((*this).find("[") == string::npos) {
		return output;
	}
	// start of a tied group so add the durations of the other notes.
   int b40 = Convert::kernToBase40(*this);
	HumdrumToken *note = this;
	HumdrumToken *nnote = NULL;
	int tcount;
	while (note) {
		tcount = note->getNextNonNullDataTokenCount();
		if (tcount == 0) {
			break;
		}
		if (!note->getNextNNDT()->isData()) {
			note = note->getNextNNDT();
			continue;
		}
		for (int i=0; i<getNextNonNullDataTokenCount(); i++) {
			nnote = note->getNextNNDT();
			if (!nnote->isData())  {
				continue;
			}
			int pitch2 = Convert::kernToBase40(*nnote);
			if (pitch2 != b40) {
				continue;
			}
			if (nnote->find("_")  != string::npos) {
				output += nnote->getDuration();
			} else if (nnote->find("]") != string::npos) {
				output += nnote->getDuration();
				return output;
			}
		}
		note = getNextNNDT();
	}
	return output;
}


HumNum HumdrumToken::getTiedDuration(HumNum scale) {
	return getTiedDuration() * scale;
}



//////////////////////////////
//
// HumdrumToken::getDots -- Count the number of '.' characters in token string.
//

int HumdrumToken::getDots(void) const {
	int count = 0;
	for (int i=0; i<(int)this->size()-1; i++) {
		if (this->at(i) == '.') {
			count++;
		}
	}
	return count;
}



//////////////////////////////
//
// HumdrumToken::getDurationNoDots -- Return the duration of the
//   note excluding any dots.
//

HumNum HumdrumToken::getDurationNoDots(void) const {

	int dots = getDots();
	if (dots == 0) {
		return getDuration();
	}
	int bot = (int)pow(2.0, dots + 1) - 1;
	int top = (int)pow(2.0, dots);
	HumNum factor(top, bot);
	return getDuration() * factor;

}


HumNum HumdrumToken::getDurationNoDots(HumNum scale) const {
	int dots = getDots();
	if (dots == 0) {
		return getDuration(scale);
	}
	int top = (int)pow(2.0, dots + 1) - 1;
	int bot = (int)pow(2.0, dots);
	HumNum factor(top, bot);
	return getDuration(scale) * factor;
}



//////////////////////////////
//
// HumdrumToken::setDuration -- Sets the duration of the token.  This is done in
//    HumdrumFileStructure::analyzeTokenDurations().
//

void HumdrumToken::setDuration(const HumNum& dur) {
	m_duration = dur;
}



//////////////////////////////
//
// HumdrumToken::getDurationFromStart -- Returns the duration from the
//   start of the owning HumdrumFile to the starting time of the
//   owning HumdrumLine for the token.  The durationFromStart is
//   in reference to the start of the token, not the end of the token,
//   which may be on another HumdrumLine.
//

HumNum HumdrumToken::getDurationFromStart(void) const {
	return getLine()->getDurationFromStart();
}


HumNum HumdrumToken::getDurationFromStart(HumNum scale) const {
	return getLine()->getDurationFromStart() * scale;
}



//////////////////////////////
//
// HumdrumToken::getDurationToEnd -- Returns the duration from the
//   start of the current line to the start of the last line
//   (the duration of the last line is always zero, so the duration
//   to end is always the duration to the end of the last non-zero
//   duration line.
//

HumNum HumdrumToken::getDurationToEnd(void) const {
	return getLine()->getDurationToEnd();
}


HumNum HumdrumToken::getDurationToEnd(HumNum scale) const {
	return getLine()->getDurationToEnd() * scale;
}



//////////////////////////////
//
// HumdrumToken::getBarlineDuration -- Returns the duration between
//   the next and previous barline.  If the token is a barline token,
//   then return the duration to the next barline.  The barline duration data
//   is filled in automatically when reading a file with the
//   HumdrumFileStructure::analyzeMeter() function.  The duration
//   will always be non-positive if the file is read with HumdrumFileBase and
//   analyzeMeter() is not run to analyze the data.
//

HumNum HumdrumToken::getBarlineDuration(void) const {
	HumdrumLine* own = getOwner();
	if (own == NULL) {
		return 0;
	}
	return own->getBarlineDuration();
}


HumNum HumdrumToken::getBarlineDuration(HumNum scale) const {
	HumdrumLine* own = getOwner();
	if (own == NULL) {
		return 0;
	}
	return own->getBarlineDuration(scale);
}



//////////////////////////////
//
// HumdrumToken::getDurationToBarline -- Get duration from start of token to
//      the start of the next barline. Units are quarter notes, unless scale
//      is set to a value other than 1.
//

HumNum HumdrumToken::getDurationToBarline(void) const {
	HumdrumLine* own = getOwner();
	if (own == NULL) {
		return 0;
	}
	return own->getDurationToBarline();
}

HumNum HumdrumToken::getDurationToBarline(HumNum scale) const {
	HumdrumLine* own = getOwner();
	if (own == NULL) {
		return 0;
	}
	return own->getDurationToBarline(scale);
}



//////////////////////////////
//
// HumdrumToken::getDurationFromBarline -- Get duration from start of token to
//      the previous barline. Units are quarter notes, unless scale
//      is set to a value other than 1.
//

HumNum HumdrumToken::getDurationFromBarline(void) const {
	HumdrumLine* own = getOwner();
	if (own == NULL) {
		return 0;
	}
	return own->getDurationFromBarline();
}

HumNum HumdrumToken::getDurationFromBarline(HumNum scale) const {
	HumdrumLine* own = getOwner();
	if (own == NULL) {
		return 0;
	}
	return own->getDurationFromBarline(scale);
}



//////////////////////////////
//
// HumdrumToken::hasRhythm -- Returns true if the exclusive interpretation
//    contains rhythmic data which will be used for analyzing the
//    duration of a HumdrumFile, for example.
//

bool HumdrumToken::hasRhythm(void) const {
	string type = getDataType();
	if (type == "**kern") {
		return true;
	}
	if (type == "**recip") {
		return true;
	}
	return false;
}



//////////////////////////////
//
// HumdrumToken::hasBeam -- True if **kern has L, J, K, or k.
//

bool HumdrumToken::hasBeam(void) const {
	for (int i=0; i<(int)this->size(); i++) {
		switch (this->at(i)) {
			case 'L':
			case 'J':
			case 'k':
			case 'K':
				return true;
		}
	}
	return false;
}



//////////////////////////////
//
// HumdrumToken::equalTo --
//

bool HumdrumToken::equalTo(const string& pattern) {
	if ((string)(*this) == pattern) {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// HumdrumToken::isRest -- Returns true if the token is a (kern) rest.
//

bool HumdrumToken::isRest(void) {
	if (isDataType("**kern")) {
		if (isNull() && Convert::isKernRest((string)(*resolveNull()))) {
			return true;
		} else if (Convert::isKernRest((string)(*this))) {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// HumdrumToken::isNote -- Returns true if the token is a (kern) note
//     (possessing a pitch).
//

bool HumdrumToken::isNote(void) {
	if (isDataType("**kern")) {
		if (Convert::isKernNote((string)(*this))) {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// HumdrumToken::isSustainedNote -- Returns true if the token represents
//     a sounding note, but not the attack portion.  Should only be
//     applied to **kern data.
//

bool HumdrumToken::isSustainedNote(void) {
	HTp token = this;
	if (isNull()) {
		token = resolveNull();
	}
	return token->isSecondaryTiedNote();
}



//////////////////////////////
//
// HumdrumToken::isNoteAttack -- Returns true if the token represents
//     the attack of a note.  Should only be applied to **kern data.
//

bool HumdrumToken::isNoteAttack(void) {
	HTp token = this;
	if (isNull()) {
		token = resolveNull();
	}
	if (token->isRest()) {
		return false;
	}
	return !token->isSecondaryTiedNote();
}



//////////////////////////////
//
// HumdrumToken::isInvisible -- True if a barline and is invisible (contains
//     a "-" styling), or a note/rest contains the string "yy" which is
//     interpreted as meaning make it invisible.
//
//

bool HumdrumToken::isInvisible(void) {
	if (!isDataType("**kern")) {
			return false;
	}
	if (isBarline()) {
		if (find("-") != string::npos) {
			return true;
		}
	} else if (isData()) {
		if (find("yy") != string::npos) {
			return true;
		}
	}

	return false;
}



//////////////////////////////
//
// HumdrumToken::isGrace -- True if a **kern note has no duration.
//

bool HumdrumToken::isGrace(void) {
	if (!isDataType("**kern")) {
			return false;
	}
	if (!isData()) {
		return false;
	} else if (this->find("q") != string::npos) {
		return true;
	}

	return false;
}



//////////////////////////////
//
// HumdrumToken::isClef -- True if a **kern clef.
//

bool HumdrumToken::isClef(void) {
	if (!isDataType("**kern")) {
			return false;
	}
	if (!isInterpretation()) {
		return false;
	} else if (this->compare(0, 5, "*clef") == 0) {
		return true;
	}

	return false;
}



//////////////////////////////
//
// HumdrumToken::isKeySignature -- True if a **kern key signature.
//

bool HumdrumToken::isKeySignature(void) {
	if (this->compare(0, 3, "*k[") != 0) {
		return false;
	}
	if (this->back() != ']') {
		return false;
	}
	return true;
}



//////////////////////////////
//
// HumdrumToken::isKeyDesignation -- True if a **kern key designation.
//

bool HumdrumToken::isKeyDesignation(void) {
	if (this->size() < 3) {
		return false;
	}
	if (this->find(":") == string::npos) {
		return false;
	}
	char diatonic = (*this)[2];

	if ((diatonic >= 'A') && (diatonic <= 'G')) {
		return true;
	}
	if ((diatonic >= 'a') && (diatonic <= 'g')) {
		return true;
	}
	return false;
}



//////////////////////////////
//
// HumdrumToken::isTimeSignature -- True if a **kern time signature.
//

bool HumdrumToken::isTimeSignature(void) {
	if (this->size() < 3) {
		return false;
	}
	if (this->compare(0, 2, "*M") != 0) {
		return false;
	}
	if (!isdigit((*this)[2])) {
		return false;
	}
	if (this->find("/") == string::npos) {
		return false;
	}
	return true;
}



//////////////////////////////
//
// HumdrumToken::isMensurationSymbol -- True if a **kern mensuration Symbol.
//

bool HumdrumToken::isMensurationSymbol(void) {
	if (this->compare(0, 5, "*met(") != 0) {
		return false;
	}
	if ((*this)[this->size()-1] != ')') {
		return false;
	}
	return true;
}



//////////////////////////////
//
// HumdrumToken::hasSlurStart -- Returns true if the **kern token has
//     a '(' character.
//

bool HumdrumToken::hasSlurStart(void) {
	if (isDataType("**kern")) {
		if (Convert::hasKernSlurStart((string)(*this))) {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// HumdrumToken::hasSlurEnd -- Returns true if the **kern token has
//     a ')' character.
//

bool HumdrumToken::hasSlurEnd(void) {
	if (isDataType("**kern")) {
		if (Convert::hasKernSlurEnd((string)(*this))) {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// HumdrumToken::hasVisibleAccidental -- Returns true if the accidental
//    of a **kern note is viewable if rendered to graphical notation.
// 	return values:
//      0  = false;
//      1  = true;
//      -1 = undefined;
//

int HumdrumToken::hasVisibleAccidental(int subtokenIndex) const {
	HumdrumLine* humrec = getOwner();
	if (humrec == NULL) {
		return -1;
	}
	HumdrumFile* humfile = humrec->getOwner();
	if (humfile == NULL) {
		return -1;
	}
	if (!humfile->getValueBool("auto", "accidentalAnalysis")) {
		int status = humfile->analyzeKernAccidentals();
		if (!status) {
			return -1;
		}
	}
	return getValueBool("auto", to_string(subtokenIndex), "visualAccidental");
}



//////////////////////////////
//
// HumdrumToken::hasVisibleAccidental -- Returns true if the accidental
//    of a **kern note is viewable if rendered to graphical notation.
// 	return values:
//      0  = false;
//      1  = true;
//      -1 = undefined;
//

int HumdrumToken::hasCautionaryAccidental(int subtokenIndex) const {
	HumdrumLine* humrec = getOwner();
	if (humrec == NULL) {
		return -1;
	}
	HumdrumFile* humfile = humrec->getOwner();
	if (humfile == NULL) {
		return -1;
	}
	if (!humfile->getValueBool("auto", "accidentalAnalysis")) {
		int status = humfile->analyzeKernAccidentals();
		if (!status) {
			return -1;
		}
	}
	return getValueBool("auto", to_string(subtokenIndex), "cautionaryAccidental");
}



//////////////////////////////
//
// HumdrumToken::isSecondaryTiedNote -- Returns true if the token
//     is a (kern) note (possessing a pitch) and has '_' or ']' characters.
//

bool HumdrumToken::isSecondaryTiedNote(void) {
	if (isDataType("**kern")) {
		if (Convert::isKernSecondaryTiedNote((string)(*this))) {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// HumdrumToken::isBarline -- Returns true if the first character is an
//   equals sign.
//

bool HumdrumToken::isBarline(void) const {
	if (size() == 0) {
		return false;
	}
	if ((*this)[0] == '=') {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// HumdrumToken::isCommentLocal -- Returns true of the token start with "!",
//   but not "!!" which is for global comments.
//

bool HumdrumToken::isCommentLocal(void) const {
	if (size() == 0) {
		return false;
	}
	if ((*this)[0] == '!') {
		if (size() > 1) {
			if ((*this)[1] == '!') {
				// global comment
				return false;
			}
		}
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// HumdrumToken::isComment -- Returns true of the token start with "!".
//

bool HumdrumToken::isComment(void) const {
	if (size() == 0) {
		return false;
	}
	if ((*this)[0] == '!') {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// HumdrumToken::isData -- Returns true if not an interpretation, barline
//      or local comment.  This will not work on synthetic tokens generated
//      from an empty line.  So this function should be called only on tokens
//      in lines which pass the HumdrumLine::hasSpines() test.
//

bool HumdrumToken::isData(void) const {
	if (size() == 0) {
		return false;
	}
	int firstchar = (*this)[0];
	if ((firstchar == '*') || (firstchar == '!') || (firstchar == '=')) {
		return false;
	}
	return true;
}



//////////////////////////////
//
// HumdrumToken::isInterpretation -- Returns true if an interpretation.
//

bool HumdrumToken::isInterpretation(void) const {
	if (size() == 0) {
		return false;
	}
	int firstchar = (*this)[0];
	if (firstchar == '*') {
		return true;
	}
	return false;
}



//////////////////////////////
//
// HumdrumToken::isNonNullData -- Returns true if the token is a data token
//    that is not a null token.
//

bool HumdrumToken::isNonNullData(void) const {
	return isData() && !isNull();
}



//////////////////////////////
//
// HumdrumToken::isNullData -- Returns true if the token is a null
//     data token.
//

bool HumdrumToken::isNullData(void) const {
	return isData() && isNull();
}



//////////////////////////////
//
// HumdrumToken::isLabel -- Returns true if a thru label (such as *>A).
//

bool HumdrumToken::isLabel(void) const {
	if (string::compare(0, 2, "*>") != 0) {
		return false;
	}
	if (string::find("[") != string::npos) {
		return false;
	}
	return true;
}



/////////////////////////////
//
// HumdrumToken::isChord -- True if is a chord.  Presuming you know what
//     data type you are accessing.
//     Default value:
//          separate = " "   (**kern note separator)
//

bool HumdrumToken::isChord(const string& separator) {
	return (this->find(separator) != string::npos) ? true : false;
}



//////////////////////////////
//
// HumdrumToken::isExclusiveInterpretation -- Returns true if first two
//     characters are "**".
//

bool HumdrumToken::isExclusiveInterpretation(void) const {
	const string& tok = (string)(*this);
	return tok.substr(0, 2) == "**";
}



//////////////////////////////
//
// HumdrumToken::isSplitInterpretation -- True if the token is "*^".
//

bool HumdrumToken::isSplitInterpretation(void) const {
	return ((string)(*this)) == SPLIT_TOKEN;
}



//////////////////////////////
//
// HumdrumToken::isMergeInterpretation -- True if the token is "*v".
//

bool HumdrumToken::isMergeInterpretation(void) const {
	return ((string)(*this)) == MERGE_TOKEN;
}



//////////////////////////////
//
// HumdrumToken::isExchangeInterpretation -- True if the token is "*x".
//

bool HumdrumToken::isExchangeInterpretation(void) const {
	return ((string)(*this)) == EXCHANGE_TOKEN;
}



//////////////////////////////
//
// HumdrumToken::isTerminateInterpretation -- True if the token is "*-".
//

bool HumdrumToken::isTerminateInterpretation(void) const {
	return ((string)(*this)) == TERMINATE_TOKEN;
}



//////////////////////////////
//
// HumdrumToken::isAddInterpretation -- True if the token is "*+".
//

bool HumdrumToken::isAddInterpretation(void) const {
	return ((string)(*this)) == ADD_TOKEN;
}



//////////////////////////////
//
// HumdrumToken::isNull -- Returns true if the token is a null token,
//   either for data, comments, or interpretations.  Does not consider
//   null global comments since they are not part of the spine structure.
//

bool HumdrumToken::isNull(void) const {
	const string& tok = (string)(*this);
	if (tok == NULL_DATA)           { return true; }
	if (tok == NULL_INTERPRETATION) { return true; }
	if (tok == NULL_COMMENT_LOCAL)  { return true; }
	return false;
}



//////////////////////////////
//
// HumdrumToken::getSubtrack -- Get the subtrack (similar to a layer
//    in MEI).
//

int HumdrumToken::getSubtrack(void) const {
	return m_address.getSubtrack();
}



//////////////////////////////
//
// HumdrumToken::getTrackString -- Gets "track.subtrack" as a string.  The
//     track and subtrack are integers.  The getTrackString function will
//     return a string with the track and subtrack separated by an dot.  The
//     Dot is not a decimal point, but if the subtrack count does not exceed
//     9, then the returned string can be treated as a floating-point number
//     where the subtrack is the fractional part.
// @SEEALSO: getTrack, getSubtrack
//

string HumdrumToken::getTrackString(void) const {
	return m_address.getTrackString();
}



/////////////////////////////
//
// HumdrumToken::getSubtokenCount -- Returns the number of sub-tokens in
//     a token.  The input parameter is the sub-token separator.  If the
//     separator comes at the start or end of the token, then there will
//     be empty sub-token(s) included in the count.
// default value: separator = " "
// @SEEALSO: getSubtoken
//

int HumdrumToken::getSubtokenCount(const string& separator) const {
	int count = 0;
	string::size_type start = 0;
	while ((start = string::find(separator, start)) != string::npos) {
		count++;
		start += separator.size();
	}
	return count+1;
}



/////////////////////////////
//
// HumdrumToken::getSubtoken -- Extract the specified sub-token from the token.
//    Tokens usually are separated by spaces in Humdrum files, but this will
//    depened on the data type (so therefore, the tokens are not presplit into
//    sub-tokens when reading in the file).
// default value: separator = " "
// @SEEALSO: getSubtokenCount, getTrackString
//

string HumdrumToken::getSubtoken(int index, const string& separator) const {
	if (index < 0) {
		return "";
	}

	string output;
	const string& token = *this;
	if (separator.size() == 0) {
		output = token[index];
		return output;
	}

	int count = 0;
	for (int i=0; i<(int)size(); i++) {
		if (string::compare(i, separator.size(), separator) == 0) {
			count++;
			if (count > index) {
				break;
			}
			i += (int)separator.size() - 1;
		} else if (count == index) {
			output += token[i];
		}
	}
	return output;
}



//////////////////////////////
//
// HumdrumToken::setParameters -- Process a local comment with
//     the structure:
//        !NS1:NS2:key1=value1:key2=value2:key3=value3
//     and store the parameter in the HumHash parent class component of the
//     HumdrumToken object.
// default value for 2-parameter version: ptok = NULL
//

void HumdrumToken::setParameters(HumdrumToken* ptok) {
	HumdrumToken& pl = *ptok;
	if (pl.size() <= 1) {
		return;
	}
	string pdata = pl.substr(1, pl.size()-1);
	setParameters(pdata, ptok);
}


void HumdrumToken::setParameters(const string& pdata, HumdrumToken* ptok) {
	vector<string> pieces = Convert::splitString(pdata, ':');
	if (pieces.size() < 3) {
		return;
	}
	string ns1 = pieces[0];
	string ns2 = pieces[1];
	string key;
	string value;
	int loc;
	for (int i=2; i<(int)pieces.size(); i++) {
		Convert::replaceOccurrences(pieces[i], "&colon;", ":");
		loc = (int)pieces[i].find("=");
		if (loc != (int)string::npos) {
			key   = pieces[i].substr(0, loc);
			value = pieces[i].substr(loc+1, pieces[i].size());
		} else {
			key   = pieces[i];
			value = "true";
		}
		setValue(ns1, ns2, key, value);
		setOrigin(ns1, ns2, key, ptok);
	}
}



//////////////////////////////
//
// HumdrumToken::setText --
//

void HumdrumToken::setText(const string& text) {
	string::assign(text);
}



//////////////////////////////
//
// HumdrumToken::getText --
//

string HumdrumToken::getText(void) const {
	return string(*this);
}



//////////////////////////////
//
// HumdrumToken::makeForwardLink -- Line a following spine token to this one.
//    Used by the HumdrumFileBase::analyzeLinks function.
//

void HumdrumToken::makeForwardLink(HumdrumToken& nextToken) {
	m_nextTokens.push_back(&nextToken);
	nextToken.m_previousTokens.push_back(this);
}



//////////////////////////////
//
// HumdrumToken::makeBackwarddLink -- Link a previous spine token to this one.
//    Used by the HumdrumFileBase::analyzeLinks function.
//

void HumdrumToken::makeBackwardLink(HumdrumToken& previousToken) {
	m_previousTokens.push_back(&previousToken);
	previousToken.m_nextTokens.push_back(this);
}



//////////////////////////////
//
// HumdrumToken::setOwner -- Sets the HumdrumLine owner of this token.
//

void HumdrumToken::setOwner(HumdrumLine* aLine) {
	m_address.setOwner(aLine);
}



//////////////////////////////
//
// HumdrumToken::getOwner -- Returns a pointer to the HumdrumLine that
//    owns this token.
//

HumdrumLine* HumdrumToken::getOwner(void) const {
	return m_address.getOwner();
}



//////////////////////////////
//
// HumdrumToken::getState -- Returns the rhythm state variable.
//

int HumdrumToken::getState(void) const {
	return m_rhycheck;
}



//////////////////////////////
//
// HumdrumToken::getStrandIndex -- Returns the 1-D strand index
//    that the token belongs to in the owning HumdrumFile.
//    Returns -1 if there is no strand assignment.
//

int  HumdrumToken::getStrandIndex(void) const {
	return m_strand;
}



//////////////////////////////
//
// HumdrumToken::getSlurStartElisionLevel -- Returns the count of
//   elision marks ('&') preceding a slur start character '('.
//   Returns -1 if there is no slur start character.
//   Default value: index = 0
//

int HumdrumToken::getSlurStartElisionLevel(int index) const {
	if (isDataType("**kern")) {
		return Convert::getKernSlurStartElisionLevel((string)(*this), index);
	} else {
		return -1;
	}
}



//////////////////////////////
//
// HumdrumToken::getSlurEndElisionLevel -- Returns the count of
//   elision marks ('&') preceding a slur end character ')'.
//   Returns -1 if there is no slur end character.
//   Default value: index = 0
//

int HumdrumToken::getSlurEndElisionLevel(int index) const {
	if (isDataType("**kern")) {
		return Convert::getKernSlurEndElisionLevel((string)(*this), index);
	} else {
		return -1;
	}
}



//////////////////////////////
//
// HumdrumToken::setStrandIndex -- Sets the 1-D strand index
//    that the token belongs to in the owning HumdrumFile.
//    By default the strand index is set to -1 when a HumdrumToken
//    is created.
//

void  HumdrumToken::setStrandIndex(int index) {
	m_strand = index;
}



//////////////////////////////
//
// HumdrumToken::incrementState -- update the rhythm analysis state variable.
//    This will prevent redundant recursive analysis in analyzeRhythm of
//    the HumdrumFileStructure class.
//

void HumdrumToken::incrementState(void) {
	m_rhycheck++;
}



//////////////////////////////
//
// HumdrumToken::getNextTokenCount -- Returns the number of tokens in the
//   spine/sub spine which follow this token.  Typically this will be 1,
//   but will be zero for a terminator interpretation (*-), and will be
//   2 for a split interpretation (*^).
//

int HumdrumToken::getNextTokenCount(void) const {
	return (int)m_nextTokens.size();
}



//////////////////////////////
//
// HumdrumToken::getPreviousTokenCount -- Returns the number of tokens
//   in the spine/sub-spine which precede this token.  Typically this will
//   be 1, but will be zero for an exclusive interpretation (starting with
//   "**"), and will be greater than one for a token which follows a
//   spine merger (using *v interpretations).
//

int HumdrumToken::getPreviousTokenCount(void) const {
	return (int)m_previousTokens.size();
}



//////////////////////////////
//
// HumdrumToken::printCsv -- print token in CSV format.
// default value: out = std::cout
//

ostream& HumdrumToken::printCsv(ostream& out) {
	string& value = *this;
	int loc = (int)this->find(",");
	if (loc == (int)string::npos) {
		out << value;
	} else {
		out << '"';
		for (int i=0; i<(int)value.size(); i++) {
		   if (value[i] == '"') {
				out << '"' << '"';
			} else {
				out << value[i];
			}
		}
		out << '"';
	}
	return out;
}



//////////////////////////////
//
// HumdrumToken::printXml -- Print a HumdrumToken in XML format.
// default value: out = cout
// default value: level = 0
// default value: indent = "\t"
//
//

ostream& HumdrumToken::printXml(ostream& out, int level, const string& indent) {
	out << Convert::repeatString(indent, level);
	out << "<field";
	out << " n=\"" << getTokenIndex() << "\"";

	out << " track=\"" << getTrack() << "\"";
	if (getSubtrack() > 0) {
		out << " subtrack=\"" << getSubtrack() << "\"";
	}
	out << " token=\"" << Convert::encodeXml(((string)(*this))) << "\"";
	out << " xml:id=\"" << getXmlId() << "\"";
	out << ">\n";

	printXmlBaseInfo(out, level+1, indent);
	printXmlStructureInfo(out, level+1, indent);

	if (isData()) {
		if (isNote()) {
			out << Convert::repeatString(indent, level+1) << "<pitch";
			out << Convert::getKernPitchAttributes(((string)(*this)));
			out << "/>\n";
		}
	}

	printXmlContentInfo(out, level+1, indent);
	printXmlParameterInfo(out, level+1, indent);
	out << Convert::repeatString(indent, level) << "</field>\n";
	return out;
}



//////////////////////////////
//
// HumdrumToken::printXmlBaseInfo -- print data type and spine info.
// default value: out = cout
// default value: level = 0
// default value: indent = "\t"
//

ostream& HumdrumToken::printXmlBaseInfo(ostream& out, int level,
		const string& indent) {

	// <dataType> redundant with
	// sequence/sequenceInfo/trackInfo/track@dataType
	out << Convert::repeatString(indent, level);
	out << "<dataType>" << getDataType().substr(2) << "</dataType>\n";

	out << Convert::repeatString(indent, level) << "<tokenType>";
	if (isNull()) {
		out << "null";
	} else if (isManipulator()) {
		out << "manipulator";
	} else if (isCommentLocal()) {
		out << "local-comment";
	} else if (isBarline()) {
		out << "barline";
	} else if (isData()) {
		out << "data";
	} else {
		out << "interpretation";
	}
	out << "</tokenType>\n";

	// <tokenFunction>
	if (isDataType("**kern")) {
		if (isNote()) {
			out << Convert::repeatString(indent, level) << "<tokenFunction>";
			out << "note" << "</tokenFunction>\n";
		} else if (isRest()) {
			out << Convert::repeatString(indent, level) << "<tokenFunction>";
			out << "note" << "</tokenFunction>\n";
		}
	}

	if (isNull()) {
		HumdrumToken* previous = getPreviousNonNullDataToken(0);
		if (previous != NULL) {
			out << Convert::repeatString(indent, level) << "<nullResolve";
			out << " text=\"";
			out << Convert::encodeXml(((string)(*previous))) << "\"";
			out << " idref=\"";
			out << previous->getXmlId();
			out << "\"/>\n";
		}
	}

	return out;
}



//////////////////////////////
//
// HumdrumToken::printXmlStructureInfo -- Prints structural information
//    other than spine analysis.
// default value: out = cout
// default value: level = 0
// default value: indent = "\t"
//

ostream& HumdrumToken::printXmlStructureInfo(ostream& out, int level,
		const string& indent) {

	if (getDuration().isNonNegative()) {
		out << Convert::repeatString(indent, level);
		out << "<duration" << Convert::getHumNumAttributes(getDuration());
		out << "/>\n";
	}

	return out;
}



//////////////////////////////
//
// HumdrumToken::printXmlContentInfo -- Print content analysis information.
// default value: out = cout
// default value: level = 0
// default value: indent = "\t"
//

ostream& HumdrumToken::printXmlContentInfo(ostream& out, int level,
		const string& indent) {
	if (hasSlurStart()) {
		out << Convert::repeatString(indent, level) << "<slur";
		if (isDefined("auto", "hangingSlur")) {
			out << " hanging=\"" << getValue("auto", "hangingSlur") << "\"";
		}
		out << ">" << endl;
		out << Convert::repeatString(indent, level+1);
		out << "<duration" << Convert::getHumNumAttributes(getSlurDuration());
		out << "/>\n";
		out << Convert::repeatString(indent, level) << "</slur>" << endl;
	}
	return out;
}



//////////////////////////////
//
// HumdrumToken::printXmlParameterInfo -- Print contents of HumHash for token.
// default value: out = cout
// default value: level = 0
// default value: indent = "\t"
//

ostream& HumdrumToken::printXmlParameterInfo(ostream& out, int level,
		const string& indent) {
	((HumHash*)this)->printXml(out, level, indent);
	return out;
}



//////////////////////////////
//
// HumdrumToken::getXmlId -- Returns an XML id attribute based on the line
//     and field index for the location of the token in the HumdrumFile.
//     An optional parameter for a prefix can be given.  If this parameter
//     is an empty string, then the prefix set in the owning HumdrumFile
//     will instead be used.  The prefix cannot start with a digit, and
//     should not include a space charcter.
//

string HumdrumToken::getXmlId(const string& prefix) const {
	string output;
	if (prefix.size() > 0) {
		output = prefix;
	} else {
		output = getXmlIdPrefix();
	}
	output += "loc" + to_string(getLineIndex()) + "_";
	output += to_string(getFieldIndex());
	// subtoken IDS could be added here.
	return output;
}



//////////////////////////////
//
// HumdrumToken::getXmlIdPrefix -- Returns the XML ID prefix from the HumdrumFile
//   structure via the HumdrumLine on which the token resides.
//

string HumdrumToken::getXmlIdPrefix(void) const {
	auto own = getOwner();
	if (own == NULL) {
		return "";
	}
	return own->getXmlIdPrefix();
}



//////////////////////////////
//
// operator<< -- Needed to avoid interaction with the HumHash parent class.
//

ostream& operator<<(ostream& out, const HumdrumToken& token) {
	out << token.c_str();
	return out;
}


ostream& operator<<(ostream& out, HumdrumToken* token) {
	if (token) {
		out << token->c_str();
	} else {
		out << "{NULL}";
	}
	return out;
}



//////////////////////////////
//
// printSequence --
//    default value: out = cout;
//

ostream& printSequence(vector<vector<HTp> >& sequence, ostream& out) {
	for (int i=0; i<(int)sequence.size(); i++) {
		for (int j=0; j<(int)sequence[i].size(); j++) {
			out << sequence[i][j];
			if (j < (int)sequence[i].size() - 1) {
				out << '\t';
			}
		}
		out << endl;
	}
	return out;
}


ostream& printSequence(vector<HTp>& sequence, ostream& out) {
	for (int i=0; i<(int)sequence.size(); i++) {
		out << sequence[i] << endl;
	}
	return out;
}



//////////////////////////////
//
// HumdrumToken::getSlurStartToken -- Return a pointer to the token
//     which starts the given slur.  Returns NULL if no start.  Assumes that
//     HumdrumFileContent::analyzeKernSlurs() has already been run.
//				<parameter key="slurEnd" value="HT_140366146702320" idref=""/>
//

HTp HumdrumToken::getSlurStartToken(int number) {
	string tag = "slurStart";
	if (number > 1) {
		tag += to_string(number);
	}
	return getValueHTp("auto", tag);
}



//////////////////////////////
//
// HumdrumToken::getSlurEndToken -- Return a pointer to the token
//     which ends the given slur.  Returns NULL if no end.  Assumes that
//     HumdrumFileContent::analyzeKernSlurs() has already been run.
//				<parameter key="slurStart" value="HT_140366146702320" idref=""/>
//

HTp HumdrumToken::getSlurEndToken(int number) {
	string tag = "slurEnd";
	if (number > 1) {
		tag += to_string(number);
	}
	return getValueHTp("auto", tag);
}



//////////////////////////////
//
// HumdrumToken::resolveNull --
//

HTp HumdrumToken::resolveNull(void) {
	if (m_nullresolve == NULL) {
		return this;
	} else {
		return m_nullresolve;
	}
}



//////////////////////////////
//
// HumdrumToken::setNullResolution --
//

void HumdrumToken::setNullResolution(HTp resolution) {
	m_nullresolve = resolution;
}




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
	m_stems = false;
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
// MxmlEvent::enableStems --
//

void MxmlEvent::enableStems(void) {
	m_stems = true;
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

HumNum MxmlEvent::getDuration(void) const {
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
//

void MxmlEvent::reportMeasureStyleToOwner (MeasureStyle style) {
	if (!m_owner) {
		return;
	}
	m_owner->receiveMeasureStyleFromChild(style);
}



//////////////////////////////
//
// MxmlEvent::reportEditorialAccidentalToOwner --
//

void MxmlEvent::reportEditorialAccidentalToOwner(void) {
	if (!m_owner) {
		return;
	}
	m_owner->receiveEditorialAccidentalFromChild();
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

	HumNum val = (int)value;
	val /= (int)ticks;

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
	bool editorialQ   = false;
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
				xml_attribute paren = child.attribute("parentheses");
				if (paren) {
					if (strcmp(paren.value(), "yes") == 0) {
						editorialQ = 1;
						reportEditorialAccidentalToOwner();
					}
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

	if (editorialQ) {
		output += "i";
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
			if (m_stems || (getDuration() == 0)) {
				const char* stemdir = child.child_value();
				if (strcmp(stemdir, "up") == 0) {
					stem = 1;
				} else if (strcmp(stemdir, "down") == 0) {
					stem = -1;
				}
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
	bool umordent       = false;
	bool lmordent       = false;
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

				// umordent
          	// <ornaments>
          	//   <inverted-mordent default-x="-4" default-y="-65" placement="below"/>
          	// </ornaments>
				if (strcmp(grandchild.name(), "inverted-mordent") == 0) {
					umordent = true;
				}
				if (strcmp(grandchild.name(), "mordent") == 0) {
					lmordent = true;
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
	if (umordent)     { ss << "m";  }  // figure out whole-tone mordents later
	if (lmordent)     { ss << "w";  }  // figure out whole-tone mordents later

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
// MxmlMeasure::enableStems --
//

void MxmlMeasure::enableStems(void) {
	m_stems = true;
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
		if (m_stems) {
			event->enableStems();
		}
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
// MxmlMeasure::reportEditorialAccidentalToOwner --
//

void MxmlMeasure::reportEditorialAccidentalToOwner(void) {
	if (!m_owner) {
		return;
	}
	m_owner->receiveEditorialAccidental();
}



//////////////////////////////
//
// MxmlMeasure::receiveEditorialAccidentalFromChild --
//

void  MxmlMeasure::receiveEditorialAccidentalFromChild(void) {
	if (m_owner != NULL) {
		m_owner->receiveEditorialAccidental();
	}
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



class MxmlMeasure;
class MxmlPart;


//////////////////////////////
//
// MxmlPart::MxmlPart -- Constructor.
//

MxmlPart::MxmlPart(void) {
	clear();
}



//////////////////////////////
//
// MxmlPart::~MxmlPart -- Deconstructor.
//

MxmlPart::~MxmlPart(void) {
	clear();
}



//////////////////////////////
//
// MxmlPart::clear -- Clear all internal variables of object.
//

void MxmlPart::clear(void) {
	for (int i=0; i<(int)m_measures.size(); i++) {
		delete m_measures[i];
		m_measures[i] = NULL;
	}
	m_measures.clear();
	m_partnum = 0;
	m_maxstaff = 0;
	m_verseCount.resize(0);
	m_harmonyCount = 0;
	m_editorialAccidental = false;
}



//////////////////////////////
//
// MxmlPart::enableStems --
//


void MxmlPart::enableStems(void) {
	m_stems = true;
}


//////////////////////////////
//
// MxmlPart::getQTicks -- Return the current divisions element value,
//    which are the number of integer ticks representing a quarter-note
//    duration.
//

long MxmlPart::getQTicks(void) const {
	if (m_qtick.size() > 0) {
		return m_qtick.back();
	} else {
		return 0;
	}
}



//////////////////////////////
//
// MxmlPart::setQTicks -- Set the current attribute/divisions value,
//     which is the number of integer ticks representing a quarter-note
//     duration.
//

int MxmlPart::setQTicks(long value) {
	if (value < 0) {
		return (int)m_qtick.size();
	}
	if (m_qtick.size() > 0) {
		if (m_qtick.back() == value) {
			return (int)m_qtick.size();
		}
	}
	m_qtick.push_back(value);
	return (int)m_qtick.size();
}



//////////////////////////////
//
// MxmlPart::addMeasure -- Append a new measure to the list of measure element.
//

bool MxmlPart::addMeasure(xpath_node mel) {
	return addMeasure(mel.node());
}


bool MxmlPart::addMeasure(xml_node mel) {
	MxmlMeasure* meas = new MxmlMeasure(this);
	if (m_stems) {
		meas->enableStems();
	}
	if (m_measures.size() > 0) {
		meas->setPreviousMeasure(m_measures.back());
		m_measures.back()->setNextMeasure(meas);
	}
	m_measures.push_back(meas);
	int status = meas->parseMeasure(mel);

	return status;
}



//////////////////////////////
//
// MxmlPart::getMeasureCount -- Return the number of stored measures.
//

int MxmlPart::getMeasureCount(void) const {
	return (int)m_measures.size();
}



//////////////////////////////
//
// MxmlPart::getMeasure -- Get the measure number at the given index.
//

MxmlMeasure* MxmlPart::getMeasure(int index) const {
	if ((index < 0) || (index >= (int)m_measures.size())) {
		return NULL;
	}
	return m_measures[index];
}



//////////////////////////////
//
// MxmlPart::getPreviousMeasure -- Given a measure, return the
//    previous measure occuring before it.
//

MxmlMeasure* MxmlPart::getPreviousMeasure(MxmlMeasure* measure) const {
	if (!measure) {
		return NULL;
	}
	if (measure == *m_measures.begin()) {
		return NULL;
	}
	if (m_measures.size() == 0) {
		return NULL;
	}

	return measure->getPreviousMeasure();
}



//////////////////////////////
//
// MxmlPart::getDuration --  Return the duration of the part in units
//     of quarter notes.  This is a sum of the duration of all measures in
//     the part.
//

HumNum MxmlPart::getDuration(void) const {
	if (m_measures.size() == 0) {
		return 0;
	}
	return m_measures.back()->getStartTime() + m_measures.back()->getDuration();
}



//////////////////////////////
//
// MxmlPart::setPartNumber -- Set the part number for the part.  Typically
//   starts at "1" for the top part in a system.
//

void MxmlPart::setPartNumber(int number) {
	m_partnum = number;
}



//////////////////////////////
//
// MxmlPart::getPartNumber -- Return the part number for the part.  Typically
//     starts at "1" for the top part in a system.
//

int MxmlPart::getPartNumber(void) const {
	return m_partnum;
}



//////////////////////////////
//
// MxmlPart::getPartIndex -- Return the part number for the part.  Typically
//     starts at "0" for the top part in a system.
//

int MxmlPart::getPartIndex(void) const {
	return m_partnum - 1;
}



//////////////////////////////
//
// MxmlPart::getStaffCount -- Return the number of staves which the part
//   contains, such as 2 for piano parts.
//

int MxmlPart::getStaffCount(void) const {
	if (!m_maxstaff) {
		return 1;
	} else {
		return m_maxstaff;
	}
}



//////////////////////////////
//
// MxmlPart::getHarmonyCount -- Return the number of verses in the part.
//

int MxmlPart::getHarmonyCount(void) const {
	return m_harmonyCount;
}



//////////////////////////////
//
// MxmlPart::hasEditorialAccidental -- Return true if part contains an editorial
//    accidental (represented as parentheses around the accidental in MusicXML.
//

bool MxmlPart::hasEditorialAccidental(void) const {
	return m_editorialAccidental;
}



//////////////////////////////
//
// MxmlPart::getVerseCount -- Return the number of verses in the part.
//

int MxmlPart::getVerseCount(void) const {
	if (m_verseCount.size() == 0) {
		return 0;
	} else {
		return m_verseCount[0];
	}
}


int MxmlPart::getVerseCount(int staffindex) const {
	int staffnum = staffindex + 1;
	if (staffnum < (int)m_verseCount.size()) {
		return m_verseCount[staffnum];
	} else {
		return 0;
	}
}



//////////////////////////////
//
// MxmlPart::receiveHarmonyCount --
//

void MxmlPart::receiveHarmonyCount(int count) {
	m_harmonyCount = count;
}



//////////////////////////////
//
// MxmlPart::receiveEditorialAccidental --
//

void MxmlPart::receiveEditorialAccidental(void) {
	m_editorialAccidental = true;
}



//////////////////////////////
//
// MxmlPart::receiveVerseCount --
//

void MxmlPart::receiveVerseCount(int count) {
if (count > 0)  {
}
	receiveVerseCount(0, count);
}


void MxmlPart::receiveVerseCount(int staffindex, int count) {
	int staffnum = staffindex + 1;
	if (staffnum < 0) {
		return;
	}
	if (staffnum < (int)m_verseCount.size()) {
		if (count > m_verseCount[staffnum]) {
			m_verseCount[staffnum] = count;
		}
	} else {
		int oldsize = (int)m_verseCount.size();
		int newsize = staffnum + 1;
		m_verseCount.resize(newsize);
		for (int i=oldsize; i<newsize; i++) {
			m_verseCount[i] = 0;
		}
		m_verseCount[staffnum] = count;
	}
}



///////////////////////////////////////////////////////////////////////////
//
// private fuctions --
//

//////////////////////////////
//
// MxmlMeasure::receiveStaffNumberFromChild -- Receive a staff number
//    placement for a note or rest and pass it along to the part class
//    so that it can keep track of the maximum staff number used in
//    the part.
//

void MxmlPart::receiveStaffNumberFromChild(int staffnum, int voicenum) {
	if (m_maxstaff < staffnum) {
		m_maxstaff = staffnum;
	}
	trackStaffVoices(staffnum, voicenum);
}



//////////////////////////////
//
// MxmlPart::trackStaffVoices -- Keep track of which staff voices
//     occur on.  This will be used later to assign voices to
//     spines, and to make notes in the voice which are not on
//     the home staff (cross-staff beaming, etc).
//

void MxmlPart::trackStaffVoices(int staffnum, int voicenum) {
	vector<vector<int> >& sv = m_staffvoicehist;
	if (staffnum < 0) {
		return;
	}
	if (voicenum < 0) {
		return;
	}
	if (staffnum >= (int)sv.size()) {
		sv.resize(staffnum+1);
	}
	if (voicenum >= (int)sv[staffnum].size()) {
		int oldsize = (int)sv[staffnum].size();
		int newsize = voicenum + 1;
		sv[staffnum].resize(newsize);
		for (int i=oldsize; i<newsize; i++) {
			sv[staffnum][i] = 0;		
		}
	}
	sv[staffnum][voicenum]++;
}


//////////////////////////////
//
// MxmlPart::prepareVoiceIndex -- Takes the histogram of staff/voice
//    pairings and create a list of new voice indexes for each
//    staff.  In Finale & Sibelius, four voices are hardwired to each
//    staff: staff1 {1, 2, 3, 4}, staff2 {5, 6, 7, 8}.  But some
//    software will not use this, instead: staff1 {1}, staff2 {2}.
//    The m_voicemapping variable will re-index voice numbers independently
//    for each staff:
//       staff1 {1, 2, 3, 4}, staff2 {5, 6, 7, 8}
//       staff1 {0, 1, 2, 3}, staff2 {0, 1, 2, 3}
//    and:
//       staff1 {1}, staff2 {2}
//       staff1 {0}, staff2 {0}
//    strange cases such as this should also work:
//       staff1 {1, 3, 5, 7}, staff2 {2, 4, 6, 8}
//       staff1 {0, 1, 2, 3}, staff2 {0, 1, 2, 3}
//    A voice is assigned to the most common staff on which its note/rests
//    occur.
//
//    voicenum in MusicXML is mapped to a (staffindex, voiceindex) pair
//       vector<pair<int, int> > m_voicemapping;
//
// Example mapping process:
// First, start with a histogram of staff/voice numbers in MusicXML file:
// 	STAFF 0:	55
// 	STAFF 1:	0	98
// 	STAFF 2:	39	0	41
// In this case staff1 has a single voice numbered "1" (with 98 counts)
// And staff2 has a single voice, numbered "2".  The final mapping
// in m_voicemapping is:
// 	0 (voice number 1) => staffindex 0, voiceindex 0
// 	1 (voice number 2) => staffindex 1, voiceindex 0
// staff0 and voice0 assignments are ignored, since there are not
// for notes (usually measures which are on staff0/voice0, and
// non-notes such as harmony which will be attached to a staff with
// but voice0, but ignored at least for now.
//

void MxmlPart::prepareVoiceMapping(void) {
	vector<vector<int> >& sv = m_staffvoicehist;
	int staffcount = (int)sv.size() - 1;
	if (staffcount < 1) {
		return;
	}
	int i, j;
	int maxvoicenum = 0;
	// a staff without any voices will probably cause problems,
	// so maybe check for such a case. 0th position in sv is
	// not used, so maxvoicenum is an index for sv.

	for (i=1; i<(int)sv.size(); i++) {
		if ((int)sv[i].size() - 1 > maxvoicenum) {
			maxvoicenum = (int)sv[i].size() - 1;
		}
	}

	// reindex voice numbers to voice indexes on staves:
	// m_voicemapping[homevoicenum] => {homestaffindex, newvoiceindex}
	pair<int, int> empty;
	empty.first = -1;
	empty.second = -1;
	int homestaffnum;
	int homevoicenum;
	int newvoiceindex;
	int count;
	int maxcount;

	// for each voice number in the MusicXML data, assign
	// a voiceindex for it on each staff.
	for (j=1; j<=maxvoicenum; j++) {
		maxcount = -1;
		homestaffnum = -1;
		homevoicenum = -1;
		for (i=1; i<(int)sv.size(); i++) {
			if (j >= (int)sv[i].size()) {
				continue;
			}
			count = sv[i][j];
			if ((count > 0) && (maxcount < count)) {
				maxcount = count;
				homestaffnum = i;
				homevoicenum = j;
			}
		}
		if (homestaffnum < 1) {
			continue;
		}
		if (homevoicenum < 1) {
			continue;
		}

		// find highest newvoiceindex for the current staff
		newvoiceindex = -1;
		for (int n=1; n<(int)m_voicemapping.size(); n++) {
			if (m_voicemapping[n].first == homestaffnum - 1) {
				newvoiceindex++;
			}
		}
		// assign to next highest newvoiceindex for staff:
		newvoiceindex++;

		// add the new mapping for homevoicenum to (staffindex, newvoiceindex)
		if (homevoicenum >= (int)m_voicemapping.size()) {
			int oldsize = (int)m_voicemapping.size();
			int newsize = homevoicenum + 1;
			m_voicemapping.resize(newsize);
			for (int m=oldsize; m<newsize; m++) {
				m_voicemapping[m] = empty;
			}
			m_voicemapping[homevoicenum].first = homestaffnum - 1;
			m_voicemapping[homevoicenum].second = newvoiceindex;
		} else {
			m_voicemapping[homevoicenum].first = homestaffnum - 1;
			m_voicemapping[homevoicenum].second = newvoiceindex;
		}
	}
}



//////////////////////////////
//
// MxmlPart::getVoiceIndex -- Convert a MusicXML voice number to
//    a voice index on a particular staff.
//

int MxmlPart::getVoiceIndex(int voicenum) {
	if (voicenum < 1) {
		return -1;
	}
	if (voicenum >= (int)m_voicemapping.size()) {
		return -1;
	}
	return m_voicemapping[voicenum].second;
}



//////////////////////////////
//
// MxmlPart::getStaffIndex -- Convert a MusicXML voice number to
//    a voice index on a particular staff.
//

int MxmlPart::getStaffIndex(int voicenum) {
	if (voicenum < 1) {
		return -1;
	}
	if (voicenum >= (int)m_voicemapping.size()) {
		return -1;
	}
	return m_voicemapping[voicenum].first;
}



//////////////////////////////
//
// MxmlPart::printStaffVoiceInfo --
//

void MxmlPart::printStaffVoiceInfo(void) {
	vector<vector<int> >& sv = m_staffvoicehist;
	int i, j;
	cout << "\n!!STAFF-VOICE MAPPING:\n";
	for (i=0; i<(int)sv.size(); i++) {
		cout << "!!\tSTAFF " << i << ":";
		for (j=0; j<(int)sv[i].size(); j++) {
			cout << "\t" << sv[i][j];
		}
		cout << endl;
	}
	cout << "!!REMAPPING:\n";
	for (i=1; i<(int)m_voicemapping.size(); i++) {
		cout << "!!\tvoicenum " << i << ":\t(";
		cout << m_voicemapping[i].first << ", ";
		cout << m_voicemapping[i].second << ")\n";
	}
	cout << endl;
}



//////////////////////////////
//
// NoteCell::NoteCell -- Constructor.
//

NoteCell::NoteCell(NoteGrid* owner, HTp token) {
	clear();
	m_owner = owner;
	m_token = token;
	calculateNumericPitches();
}



//////////////////////////////
//
// NoteCell::clear -- Clear the contents of the object.
//

void NoteCell::clear(void) {
	m_owner = NULL;
	m_token = NULL;
	m_b7  = 0;
	m_b12 = 0;
	m_b40 = 0;
	m_accidental = 0;
	m_nextAttackIndex = -1;
	m_prevAttackIndex = -1;
	m_currAttackIndex = -1;
	m_timeslice = -1;
	m_voice = -1;
}



//////////////////////////////
//
// NoteCell::calculateNumericPitches -- Fills in b7, b12, and b40 variables.
//    0 = rest, negative values for sustained notes.
//

void NoteCell::calculateNumericPitches(void) {
	if (!m_token) {
		m_b40        = NAN;
		m_b12        = NAN;
		m_b7         = NAN;
		m_accidental = NAN;
		return;
	}

	bool sustain = m_token->isNull() || m_token->isSecondaryTiedNote();
	if (m_token->isRest()) {
		m_b40 = NAN;
	} else {
		m_b40 = Convert::kernToBase40(m_token->resolveNull());
		m_b40 = (sustain ? -m_b40 : m_b40);
	}

	// convert to base-7 (diatonic pitch numbers)
	if (m_b40 > 0) {
		m_b7         = Convert::base40ToDiatonic(m_b40);
		m_b12        = Convert::base40ToMidiNoteNumber(m_b40);
		m_accidental = Convert::base40ToAccidental(m_b40);
	} else if (m_b40 < 0) {
		m_b7         = -Convert::base40ToDiatonic(-m_b40);
		m_b12        = -Convert::base40ToMidiNoteNumber(-m_b40);
		m_accidental = -Convert::base40ToAccidental(-m_b40);
	} else {
		m_b7         = NAN;
		m_b12        = NAN;
		m_accidental = NAN;
	}
}



//////////////////////////////
//
// NoteCell::getSgnKernPitch -- Return the **kern representation of the pitch.
//   Parentheses are placed around the pitch name if the NoteCell is a
//   sustain.
//

string NoteCell::getSgnKernPitch(void) {
	if (isRest()) {
		return "r";
	}
	string pitch;
	pitch = Convert::base40ToKern(getAbsBase40Pitch());
	if (isSustained()) {
		pitch.insert(0, "(");
		pitch += ")";
	}
	return pitch;
}



//////////////////////////////
//
// NoteCell::getAbsKernPitch -- Return the **kern representation of the pitch.
//

string NoteCell::getAbsKernPitch(void) {
	if (isRest()) {
		return "r";
	}
	return Convert::base40ToKern(getAbsBase40Pitch());
}



//////////////////////////////
//
// NoteCell::isSustained --
//

bool NoteCell::isSustained(void) {
	if (m_b40 < 0) {
		return true;
	} else if (m_b40 > 0) {
		return false;
	}
	// calculate if rest is a "sustain" or an "attack"
	if (m_currAttackIndex == m_timeslice) {
		return false;
	} else {
		return true;
	}
}



//////////////////////////////
//
// NoteCell::operator- -- Calculate the diatonic interval between
//   two notes.  Maybe layter allow subtraction operator to deal
//   with base-12 and base-40 represnetation.
//

double NoteCell::operator-(NoteCell& B) {
	NoteCell& A = *this;
	return A.getAbsDiatonicPitch() - B.getAbsDiatonicPitch();
}


double NoteCell::operator-(int B) {
	NoteCell& A = *this;
	return A.getAbsDiatonicPitch() - B;
}



//////////////////////////////
//
// NoteCell::getLineIndex -- Returns the line index of the note in
//    the original file.
//

int NoteCell::getLineIndex(void) {
	if (!m_token) {
		return -1;
	}
	return m_token->getLineIndex();
}



//////////////////////////////
//
// NoteCell:printNoteInfo --
//

ostream& NoteCell::printNoteInfo(ostream& out) {
	out << getSliceIndex()       << "\t";
	out << getSgnKernPitch()     << "\t";
	out << getPrevAttackIndex()  << "\t";
	out << getCurrAttackIndex()  << "\t";
	out << getNextAttackIndex()  << "\t";
	out << getSgnDiatonicPitch() << "\t";
	out << getSgnMidiPitch()     << "\t";
	out << getSgnBase40Pitch();
	out << endl;
	return out;
}



//////////////////////////////
//
// NoteCell::getDiatonicIntervalFromPreviousAttack --
//

double NoteCell::getDiatonicIntervalFromPreviousAttack(void) {
	int previ = getPrevAttackIndex();
	if (previ < 0) {
		return NAN;
	}
	if (!m_owner) {
		return NAN;
	}
	return getAbsDiatonicPitch()
			- m_owner->cell(m_voice,previ)->getAbsDiatonicPitch();
}



//////////////////////////////
//
// NoteCell::getDiatonicIntervalFromNextAttack --
//

double NoteCell::getDiatonicIntervalToNextAttack(void) {
	int nexti = getNextAttackIndex();
	if (nexti < 0) {
		return NAN;
	}
	if (!m_owner) {
		return NAN;
	}
	return m_owner->cell(m_voice,nexti)->getAbsDiatonicPitch()
			- getAbsDiatonicPitch();
}



//////////////////////////////
//
// NoteCell::isRest --
//

bool NoteCell::isRest(void) {
	// bug in GCC requires :: prefix to resolve two different isnan() defs.
	return Convert::isNaN(m_b40);
}



//////////////////////////////
//
// NoteCell::getMetricLevel --
//

double NoteCell::getMetricLevel(void) {
	if (!m_owner) {
		return NAN;
	}
	return m_owner->getMetricLevel(getLineIndex());
}



//////////////////////////////
//
// NoteCell::getDurationFromStart --
//

HumNum NoteCell::getDurationFromStart(void) {
	if (m_token) {
		return m_token->getDurationFromStart();
	} else {
		return -1;
	}
}


//////////////////////////////
//
// NoteCell::getDuration -- Return the duration to the next note attack
//     in the grid in the same voice.
//

HumNum NoteCell::getDuration(void) {
	if (!m_owner) {
		return Convert::recipToDuration(m_token);
	}
	return m_owner->getNoteDuration(getVoiceIndex(), getSliceIndex());
}



//////////////////////////////
//
// NoteCell::setMeter --
//

void NoteCell::setMeter(int topval, HumNum botval) {
	m_metertop = topval;
	m_meterbot = botval;
}



//////////////////////////////
//
// NoteCell::getMeterTop --
//

int NoteCell::getMeterTop(void) {
	return m_metertop;
}



//////////////////////////////
//
// NoteCell::getMeterBottom --
//

HumNum NoteCell::getMeterBottom(void) {
	return m_meterbot;
}




//////////////////////////////
//
// NoteGrid::NoteGrid -- Constructor.
//

NoteGrid::NoteGrid(HumdrumFile& infile) {
	m_infile = NULL;
	load(infile);
}



//////////////////////////////
//
// NoteGrid::~NoteGrid -- Deconstructor.
//

NoteGrid::~NoteGrid() {
	clear();
}



//////////////////////////////
//
// NoteGrid::clear -- Deallocate storage and make the object empty.
//

void NoteGrid::clear(void) {
	m_infile = NULL;
	m_kernspines.clear();

	vector<vector<NoteCell* > >& grid = m_grid;
	for (int i=0; i<(int)grid.size(); i++) {
		for (int j=0; j<(int)grid[i].size(); j++) {
			if (grid[i][j]) {
				grid[i][j]->clear();
				delete grid[i][j];
				grid[i][j] = NULL;
			}
		}
		grid[i].clear();
	}
	grid.clear();
}



//////////////////////////////
//
// NoteGrid::getVoiceCount -- Return the number of voices/parts in the grid.
//

int NoteGrid::getVoiceCount(void) {
	return (int)m_grid.size();
}



//////////////////////////////
//
// NoteGrid::getSliceCount -- Return the number of time slices in the grid.
//     the grid.
//

int NoteGrid::getSliceCount(void) {
	if (m_grid.size() == 0) {
		return 0;
	} else {
		return (int)m_grid[0].size();
	}
}



//////////////////////////////
//
// NoteGrid::load -- Generate a two-dimensional list of notes
//     in a score.  Each row has at least one note attack, or an
//     empty data line in the Humdrum file will be skipped.
//

bool NoteGrid::load(HumdrumFile& infile) {
	// remove any previous contents:
	clear();

	m_infile = &infile;

	m_kernspines = infile.getKernSpineStartList();
	vector<HTp>& kernspines = m_kernspines;

	vector<int> metertops(infile.getMaxTrack() + 1, 0);
	vector<HumNum> meterbots(infile.getMaxTrack() + 1, 0);

	if (kernspines.size() == 0) {
		cerr << "Warning: no **kern spines in file" << endl;
		return false;
	}

	vector<vector<NoteCell* > >& grid = m_grid;
	grid.resize(kernspines.size());
	for (int i=0; i<(int)grid.size(); i++) {
		grid[i].reserve(infile.getLineCount());
	}

	int attack = 0;
	int track, lasttrack;
	vector<HTp> current;
	HumRegex hre;
	for (int i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isInterpretation()) {
			for (int j=0; j<infile[i].getFieldCount(); j++) {
				if (!infile[i].token(j)->isKern()) {
					continue;
				}
				track = infile.token(i, j)->getTrack();
				if (hre.search(*infile.token(i, j), "\\*M(\\d+)/(\\d+)%(\\d+)")) {
					metertops[track] = hre.getMatchInt(1);
					meterbots[track] = hre.getMatchInt(2);
					meterbots[track] /= hre.getMatchInt(3);
				} else if (hre.search(*infile.token(i, j), "\\*M(\\d+)/(\\d+)")) {
					metertops[track] = hre.getMatchInt(1);
					meterbots[track] = hre.getMatchInt(2);
				} else {
					continue;
				}

			}
		}
		if (!infile[i].isData()) {
			continue;
		}
		track = 0;
		attack = 0;
		current.clear();
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			lasttrack = track;
			track = infile.token(i, j)->getTrack();
			if (!infile[i].token(j)->isDataType("**kern")) {
				continue;
			}
			if (track == lasttrack) {
				// secondary voice: ignore
				continue;
			}
			current.push_back(infile.token(i, j));
			if (!(current.back()->isRest()
					|| current.back()->isSecondaryTiedNote())) {
				attack++;
			}
		}
		if (current.size() != kernspines.size()) {
			cerr << "Error: Unequal vector sizes " << current.size()
			     << " compared to " << kernspines.size() << endl;
			return false;
		}
		for (int j=0; j<(int)current.size(); j++) {
			NoteCell* cell = new NoteCell(this, current[j]);
			track = current[j]->getTrack();
			cell->setVoiceIndex(j);
			cell->setSliceIndex((int)grid[j].size());
			cell->setMeter(metertops[track], meterbots[track]);
			grid[j].push_back(cell);
		}
	}

	buildAttackIndexes();

	return true;
}



//////////////////////////////
//
// NoteGrid::cell -- Return the given cell in the grid.
//

NoteCell* NoteGrid::cell(int voiceindex, int sliceindex) {
	return m_grid.at(voiceindex).at(sliceindex);
}




//////////////////////////////
//
// NoteGrid::printDiatonicGrid --
//

void NoteGrid::printDiatonicGrid(ostream& output) {
	for (int j=0; j<getSliceCount(); j++) {
		for (int i=0; i<(int)getVoiceCount(); i++) {
			output << cell(i, j)->getSgnDiatonicPitch();
			if (i < getVoiceCount() - 1) {
				output << "\t";
			}
		}
		output << endl;
	}
}



//////////////////////////////
//
// NoteGrid::printMidiGrid --
//

void NoteGrid::printMidiGrid(ostream& output) {
	for (int j=0; j<getSliceCount(); j++) {
		for (int i=0; i<(int)getVoiceCount(); i++) {
			output << cell(i, j)->getSgnMidiPitch();
			if (i < getVoiceCount() - 1) {
				output << "\t";
			}
		}
		output << endl;
	}
}



//////////////////////////////
//
// NoteGrid::printBase40Grid --
//

void NoteGrid::printBase40Grid(ostream& output) {
	for (int j=0; j<getSliceCount(); j++) {
		for (int i=0; i<(int)getVoiceCount(); i++) {
			output << cell(i, j)->getSgnBase40Pitch();
			if (i < getVoiceCount() - 1) {
				output << "\t";
			}
		}
		output << endl;
	}
}



//////////////////////////////
//
// NoteGrid::printRawGrid --
//

void NoteGrid::printRawGrid(ostream& output) {
	for (int j=0; j<getSliceCount(); j++) {
		for (int i=0; i<(int)getVoiceCount(); i++) {
			output << cell(i, j)->getToken();
			if (i < getVoiceCount() - 1) {
				output << "\t";
			}
		}
		output << endl;
	}
}



//////////////////////////////
//
// NoteGrid::printKernGrid --
//

void NoteGrid::printKernGrid(ostream& output) {
	for (int j=0; j<getSliceCount(); j++) {
		for (int i=0; i<(int)getVoiceCount(); i++) {
			output << cell(i, j)->getSgnKernPitch();
			if (i < getVoiceCount() - 1) {
				output << "\t";
			}
		}
		output << endl;
	}
}



//////////////////////////////
//
// NoteGrid::buildAttackIndexes -- create forward and backward
//     note attack indexes for each cell.
//

void NoteGrid::buildAttackIndexes(void) {
	for (int i=0; i<(int)m_grid.size(); i++) {
		buildAttackIndex(i);
	}
}



//////////////////////////////
//
// NoteGrid::buildAttackIndex -- create forward and backward
//     note attack indexes for each cell in a single voice.
//

void NoteGrid::buildAttackIndex(int vindex) {
	vector<NoteCell*>& part = m_grid[vindex];

	// Set the slice index for the attack of the current note.  This
	// will be the same as the current slice if the NoteCell is an attack.
	// Otherwise if the note is a sustain, thie index will be set
	// to the slice of the attack correspinding to this NoteCell.
	// For rests, the first rest in a continuous sequence of rests
	// will be marked as the "attack" of the rest.
	for (int i=0; i<(int)part.size(); i++) {
		if (i == 0) {
			part[0]->setCurrAttackIndex(0);
			continue;
		}
		if (part[i]->isRest()) {
			// This is a rest, so check for a rest sustain or start
			// of a rest sequence.
			if (part[i-1]->isRest()) {
				// rest "sustain"
				part[i]->setCurrAttackIndex(part[i-1]->getCurrAttackIndex());
			} else {
				// rest "attack";
				part[i]->setCurrAttackIndex(i);
			}
		} else if (part[i]->isAttack()) {
			part[i]->setCurrAttackIndex(i);
		} else {
			// This is a sustain, so get the attack index of the
			// note from the previous slice index.
			part[i]->setCurrAttackIndex(part[i-1]->getCurrAttackIndex());
		}
	}

	// start with note attacks marked in the previous and next note slots:
	for (int i=0; i<(int)part.size(); i++) {
		if (part[i]->isAttack()) {
			part[i]->setNextAttackIndex(i);
			part[i]->setPrevAttackIndex(i);
		} else if (part[i]->isRest()) {
			if (part[i]->getCurrAttackIndex() == i) {
				part[i]->setNextAttackIndex(i);
				part[i]->setPrevAttackIndex(i);
			}
		}
	}

	// Go back and adjust the next note attack index:
	int value = -1;
	int temp  = -1;
	for (int i=(int)part.size()-1; i>=0; i--) {
		if (!part[i]->isSustained()) {
			temp = part[i]->getNextAttackIndex();
			part[i]->setNextAttackIndex(value);
			value = temp;
		} else {
			part[i]->setNextAttackIndex(value);
		}
	}

	// Go back and adjust the previous note attack index:
	value = -1;
	temp  = -1;
	for (int i=0; i<(int)part.size(); i++) {
		if (!part[i]->isSustained()) {
			temp = part[i]->getPrevAttackIndex();
			part[i]->setPrevAttackIndex(value);
			value = temp;
		} else {
			if (i != 0) {
				part[i]->setPrevAttackIndex(part[i-1]->getPrevAttackIndex());
			}
		}
	}

}



//////////////////////////////
//
// NoteGrid::getAbsDiatonicPitch -- Return the diatonic pitch number for
//     the given cell.
//

double NoteGrid::getAbsDiatonicPitch(int vindex, int sindex) {
	return m_grid.at(vindex).at(sindex)->getAbsDiatonicPitch();
}



//////////////////////////////
//
// NoteGrid::getSgnDiatonicPitch -- Return the diatonic pitch number for
//     the given cell.
//

double NoteGrid::getSgnDiatonicPitch(int vindex, int sindex) {
	return m_grid.at(vindex).at(sindex)->getSgnDiatonicPitch();
}



//////////////////////////////
//
// NoteGrid::getAbsMidiPitch -- Return the MIDI pitch number for
//     the given cell.
//

double NoteGrid::getAbsMidiPitch(int vindex, int sindex) {
	return m_grid.at(vindex).at(sindex)->getAbsMidiPitch();
}



//////////////////////////////
//
// NoteGrid::getSgnMidiPitch -- Return the MIDI pitch number for
//     the given cell.
//

double NoteGrid::getSgnMidiPitch(int vindex, int sindex) {
	return m_grid.at(vindex).at(sindex)->getSgnMidiPitch();
}



//////////////////////////////
//
// NoteGrid::getAbsBase40Pitch -- Return the base-40 pitch number for
//     the given cell.
//

double NoteGrid::getAbsBase40Pitch(int vindex, int sindex) {
	return m_grid.at(vindex).at(sindex)->getAbsBase40Pitch();
}



//////////////////////////////
//
// NoteGrid::getSgnBase40Pitch -- Return the base-40 pitch number for
//     the given cell.
//

double NoteGrid::getSgnBase40Pitch(int vindex, int sindex) {
	return m_grid.at(vindex).at(sindex)->getSgnBase40Pitch();
}



//////////////////////////////
//
// NoteGrid::getAbsKernPitch -- Return the **kern pitch name for
//     the given cell.  Sustained notes are enclosed in parentheses.
//

string NoteGrid::getAbsKernPitch(int vindex, int sindex) {
	return m_grid.at(vindex).at(sindex)->getAbsKernPitch();
}



//////////////////////////////
//
// NoteGrid::getSgnKernPitch -- Return the **kern pitch name for
//     the given cell.  Sustained notes are enclosed in parentheses.
//

string NoteGrid::getSgnKernPitch(int vindex, int sindex) {
	return m_grid.at(vindex).at(sindex)->getSgnKernPitch();
}



//////////////////////////////
//
// NoteGrid::getToken -- Return the HumdrumToken pointer for
//     the given cell.
//

HTp NoteGrid::getToken(int vindex, int sindex) {
	return m_grid.at(vindex).at(sindex)->getToken();
}



//////////////////////////////
//
// NoteGrid::getPrevAttackDiatonic -- Return the Diatonic note number
//     of the previous attacked note for the given cell.  Return 0 if
//     the previous note attack is a rest or there is no previous note
//     in the voice.
//

int NoteGrid::getPrevAttackDiatonic(int vindex, int sindex) {
	NoteCell*& cell = m_grid.at(vindex).at(sindex);
	int index = cell->getPrevAttackIndex();
	if (index < 0) {
		return 0;
	} else {
		return this->cell(vindex, index)->getAbsDiatonicPitch();
	}
}



//////////////////////////////
//
// NoteGrid::getNextAttackDiatonic -- Return the Diatonic note number
//     of the next attacked note for the given cell.  Return 0 if
//     the next note attack is a rest or there is no next note
//     in the voice.
//

int NoteGrid::getNextAttackDiatonic(int vindex, int sindex) {
	NoteCell*& cell = m_grid.at(vindex).at(sindex);
	int index = cell->getNextAttackIndex();
	if (index < 0) {
		return 0;
	} else {
		return this->cell(vindex, index)->getAbsDiatonicPitch();
	}
}



//////////////////////////////
//
// NoteGrid::getLineIndex -- return the line index in the original
//    Humdrum data for the given slice index.
//

int NoteGrid::getLineIndex(int sindex) {
	if (m_grid.size() == 0) {
		return -1;
	}
	return m_grid.at(0).at(sindex)->getToken()->getLineIndex();
}



//////////////////////////////
//
// NoteGrid::getNoteAndRestAttacks -- Return the note attacks,
//    and the first rest slice ("rest attack") for a particular voice.
//

void NoteGrid::getNoteAndRestAttacks(vector<NoteCell*>& attacks,
		int vindex) {
	attacks.resize(0);
	int max = getSliceCount();
	if (max == 0) {
		return;
	}
	attacks.reserve(max);
	NoteCell* note = cell(vindex, 0);
	attacks.push_back(note);
	while (attacks.back()->getNextAttackIndex() > 0) {
		note = cell(vindex, attacks.back()->getNextAttackIndex());
		if (note == attacks.back()) {
			cerr << "Strange duplicate: ";
			note->printNoteInfo(cerr);
			break;
		}
		attacks.push_back(note);
	}
}



//////////////////////////////
//
// NoteGrid::getMetricLevel --
//

double NoteGrid::getMetricLevel(int sindex) {
	if (!m_infile) {
		return NAN;
	}
	if ((getSliceCount() == 0) || (getVoiceCount() == 0)) {
		return NAN;
	}
	if (m_metriclevels.empty()) {
		int track = 0;
		if ((getVoiceCount() > 0) && (getSliceCount() > 0)) {
			track = cell(0, 0)->getToken()->getTrack();
		}
		m_infile->getMetricLevels(m_metriclevels, track, NAN);
	}
	return m_metriclevels[sindex];
}



//////////////////////////////
//
// NoteGrid::getNoteDuration --
//

HumNum NoteGrid::getNoteDuration(int vindex, int sindex) {
	NoteCell* curnote = cell(vindex, sindex);
	int attacki = curnote->getCurrAttackIndex();
	int nexti   = curnote->getNextAttackIndex();
	HumNum starttime = 0;
	if (attacki >= 0) {
		starttime = cell(vindex, attacki)->getDurationFromStart();
	}
	HumNum endtime = m_infile->getScoreDuration();;
	if (nexti >= 0) {
		endtime = cell(vindex, nexti)->getDurationFromStart();
	}
	return endtime - starttime;
}



//////////////////////////////
//
// NoteGrid::printGridInfo -- for debugging.
//

void NoteGrid::printGridInfo(ostream& out) {
	for (int i=0; i<getVoiceCount(); i++) {
		printVoiceInfo(out, i);
		out << endl;
	}

}



//////////////////////////////
//
// NoteGrid::printVoiceInfo -- for debugging.
//

void NoteGrid::printVoiceInfo(ostream& out, int vindex) {
	out << "============================================================";
	out << endl;
	out << "i\tnote\tprevi\tcurri\tnexti\tb7\tmidi\tb40\n";
	for (int i=0; i<getSliceCount(); i++) {
		this->cell(vindex, i)->printNoteInfo(out);
	}
}




///////////////////////////////////////////////////////////////////////////
//
// Option_register class function definitions.
//


//////////////////////////////
//
// Option_register::Option_register -- Constructor.
//

Option_register::Option_register(void) {
	m_modifiedQ = 0;
	setType('s');
}


Option_register::Option_register(const string& aDefinition, char aType,
		const string& aDefaultOption) {
	m_modifiedQ = 0;
	setType(aType);
	setDefinition(aDefinition);
	setDefault(aDefaultOption);
}


Option_register::Option_register(const string& aDefinition, char aType,
		const string& aDefaultOption, const string& aModifiedOption) {
	m_modifiedQ = 0;
	setType(aType);
	setDefinition(aDefinition);
	setDefault(aDefaultOption);
	setModified(aModifiedOption);
}


Option_register::Option_register(const Option_register& reg) {
	m_definition = reg.m_definition;
	m_description = reg.m_description;
	m_defaultOption = reg.m_defaultOption;
	m_modifiedOption = reg.m_modifiedOption;
	m_modifiedQ = reg.m_modifiedQ;
	m_type = reg.m_type;
}



//////////////////////////////
//
// Option_register::operator= --
//

Option_register& Option_register::operator=(const Option_register& reg) {
	if (this == &reg) {
		return *this;
	}
	m_definition = reg.m_definition;
	m_description = reg.m_description;
	m_defaultOption = reg.m_defaultOption;
	m_modifiedOption = reg.m_modifiedOption;
	m_modifiedQ = reg.m_modifiedQ;
	m_type = reg.m_type;
	return *this;
}



//////////////////////////////
//
// Option_register::~Option_register -- Destructor.
//

Option_register::~Option_register() {
	// do nothing
}



//////////////////////////////
//
// Option_register::clearModified -- Clear any changes in the option value.
//

void Option_register::clearModified(void) {
	m_modifiedOption.clear();
	m_modifiedQ = 0;
}



//////////////////////////////
//
// Option_register::getDefinition -- Returns the initial definition.
//	string used to define this entry.
//

string Option_register::getDefinition(void) {
	return m_definition;
}



//////////////////////////////
//
// Option_register::getDescription -- Return the textual description
//      of the entry.
//

string Option_register::getDescription(void) {
	return m_description;
}



//////////////////////////////
//
// Option_register::getDefault --  Return the default value string.
//

string Option_register::getDefault(void) {
	return m_defaultOption;
}



//////////////////////////////
//
// Option_register::getModified -- Return the modified option string.
//

string Option_register::getModified(void) {
	return m_modifiedOption;
}



//////////////////////////////
//
// Option_register::isModified -- Return true if option has been
//    set on the command-line.
//

int Option_register::isModified(void) {
	return m_modifiedQ;
}



//////////////////////////////
//
// Option_register::getType -- Return the data type of the option.
//

char Option_register::getType(void) {
	return m_type;
}



//////////////////////////////
//
// Option_register::getOption -- return the modified option
//  	or the default option if no modified option.
//

string Option_register::getOption(void) {
	if (isModified()) {
		return getModified();
	} else {
		return getDefault();
	}
}



//////////////////////////////
//
// Option_register::reset -- clear contents of register entry.
//

void Option_register::reset(void) {
	m_definition.clear();
	m_description.clear();
	m_defaultOption.clear();
	m_modifiedOption.clear();
	m_modifiedQ = false;
	m_type = 's';
}



//////////////////////////////
//
// Option_register::setDefault -- Set the default value.
//

void Option_register::setDefault(const string& aString) {
	m_defaultOption = aString;
}



//////////////////////////////
//
// Option_register::setDefinition -- Set the option definition.
//

void Option_register::setDefinition(const string& aString) {
	m_definition = aString;
}



//////////////////////////////
//
// Option_register::setDescription -- Set the textual description.
//

void Option_register::setDescription(const string& aString) {
	m_description = aString;
}



//////////////////////////////
//
// Option_register::setModified -- Set the modified value.
//

void Option_register::setModified(const string& aString) {
	m_modifiedOption = aString;
	m_modifiedQ = 1;
}



//////////////////////////////
//
// Option_register::setType -- Set the option type.
//

void Option_register::setType(char aType) {
	m_type = aType;
}



//////////////////////////////
//
// Option_register::print -- Print the state of the option registery entry.
//     Useul for debugging.
//

ostream& Option_register::print(ostream& out) {
	out << "definition:\t"     << m_definition     << endl;
	out << "description:\t"    << m_description    << endl;
	out << "defaultOption:\t"  << m_defaultOption  << endl;
	out << "modifiedOption:\t" << m_modifiedOption << endl;
	out << "modifiedQ:\t\t"    << m_modifiedQ      << endl;
	out << "type:\t\t"         << m_type           << endl;
	return out;
};




///////////////////////////////////////////////////////////////////////////
//
// Options class function definitions.
//

//////////////////////////////
//
// Options::Options -- Constructor.
//

Options::Options(void) {
	// do nothing
}


Options::Options(int argc, char** argv) {
	setOptions(argc, argv);
}


Options::Options(const Options& options) {
	m_argv = options.m_argv;
	m_arguments = options.m_arguments;
	m_optionFlag = options.m_optionFlag;
	m_optionList = options.m_optionList;
	m_options_error_checkQ = options.m_options_error_checkQ;
	m_processedQ = options.m_processedQ;
	m_suppressQ = options.m_suppressQ;
	m_optionsArgQ = options.m_optionsArgQ;
	for (int i=0; i<(int)options.m_optionRegister.size(); i++) {
		Option_register* orr = new Option_register(*options.m_optionRegister[i]);
		m_optionRegister.push_back(orr);
	}

}



//////////////////////////////
//
// Options::~Options -- Destructor.
//

Options::~Options() {
	reset();
}



//////////////////////////////
//
// Options::operator= --
//

Options& Options::operator=(const Options& options) {
	if (this == &options) {
		return *this;
	}
	m_argv = options.m_argv;
	m_arguments = options.m_arguments;
	m_optionFlag = options.m_optionFlag;
	m_optionList = options.m_optionList;
	m_options_error_checkQ = options.m_options_error_checkQ;
	m_processedQ = options.m_processedQ;
	m_suppressQ = options.m_suppressQ;
	m_optionsArgQ = options.m_optionsArgQ;

	for (int i=0; i<(int)m_optionRegister.size(); i++) {
		delete m_optionRegister[i];
		m_optionRegister[i] = NULL;
	}
	m_optionRegister.clear();
	
	for (int i=0; i<(int)options.m_optionRegister.size(); i++) {
		Option_register* orr = new Option_register(*options.m_optionRegister[i]);
		m_optionRegister.push_back(orr);
	}

	m_error.str("");
	return *this;
}



//////////////////////////////
//
// Options::argc -- returns the argument count as input from main().
//

int Options::argc(void) const {
	return (int)m_argv.size();
}



//////////////////////////////
//
// Options::argv -- returns the arguments strings as input from main().
//

const vector<string>& Options::argv(void) const {
	return m_argv;
}



//////////////////////////////
//
// Options::define -- store an option definition in the registry.  Option
//     definitions have this sructure:
//        option-name|alias-name1|alias-name2=option-type:option-default
// option-name :: name of the option (one or more character, not including
//      spaces or equal signs.
// alias-name  :: equivalent name(s) of the option.
// option-type :: single charater indicating the option data type.
// option-default :: default value for option if no given on the command-line.
//

int Options::define(const string& aDefinition) {
	Option_register* definitionEntry = NULL;

	// Error if definition string doesn't contain an equals sign
	auto location = aDefinition.find("=");
	if (location == string::npos) {
		m_error << "Error: no \"=\" in option definition: " << aDefinition << endl;
		return -1;
	}

	string aliases = aDefinition.substr(0, location);
	string rest    = aDefinition.substr(location+1);
	string otype   = rest;
	string ovalue  = "";

	location = rest.find(":");
	if (location != string::npos) {
		otype  = rest.substr(0, location);
		ovalue = rest.substr(location+1);
	}

	// Remove anyspaces in the option type field
	otype.erase(remove_if(otype.begin(), otype.end(), ::isspace), otype.end());

	// Option types are only a single charater (b, i, d, c or s)
	if (otype.size() != 1) {
		m_error << "Error: option type is invalid: " << otype
			  << " in option definition: " << aDefinition << endl;
		return -1;
	}

	// Check to make sure that the type is known
	if (otype[0] != OPTION_STRING_TYPE  &&
		 otype[0] != OPTION_INT_TYPE     &&
		 otype[0] != OPTION_FLOAT_TYPE   &&
		 otype[0] != OPTION_DOUBLE_TYPE  &&
		 otype[0] != OPTION_BOOLEAN_TYPE &&
		 otype[0] != OPTION_CHAR_TYPE ) {
		m_error << "Error: unknown option type \'" << otype[0]
			  << "\' in defintion: " << aDefinition << endl;
		return -1;
	}

	// Set up space for a option entry in the registry
	definitionEntry = new Option_register(aDefinition, otype[0], ovalue);

	int definitionIndex = (int)m_optionRegister.size();

	// Store option aliases
	string optionName;
	aliases += '|';
	for (int i=0; i<(int)aliases.size(); i++) {
		if (::isspace(aliases[i])) {
			continue;
		} else if (aliases[i] == '|') {
			if (isDefined(optionName)) {
				m_error << "Option \"" << optionName << "\" from definition:" << endl;
				m_error << "\t" << aDefinition << endl;
				m_error << "is already defined in: " << endl;
				m_error << "\t" << getDefinition(optionName) << endl;
				return -1;
			}
			if (optionName.size() > 0) {
				m_optionList[optionName] = definitionIndex;
			}
			optionName.clear();
		} else {
			optionName += aliases[i];
		}
	}

	// Store definition in registry and return its indexed location.
	// This location will be used to link option aliases to the main
	// command name.
	m_optionRegister.push_back(definitionEntry);
	return definitionIndex;
}


int Options::define(const string& aDefinition, const string& aDescription) {
	int index = define(aDefinition);
	m_optionRegister[index]->setDescription(aDescription);
	return index;
}



//////////////////////////////
//
// Options::isDefined -- Return true if option is present in registry.
//

int Options::isDefined(const string& name) {
	if (m_optionList.find(name) == m_optionList.end()) {
		return 0;
	} else {
		return 1;
	}
}



//////////////////////////////
//
// Options::getArg -- returns the specified argument.
//	argurment 0 is the command name.
//

string Options::getArg(int index) {
	if (index == 0) {
		if (m_argv.empty()) {
			return "";
		} else {
			return m_argv[0];
		}
	}
	if (index < 1 || index > (int)m_arguments.size()) {
		m_error << "Error: argument " << index << " does not exist." << endl;
		return "";
	}
	return m_arguments[index - 1];
}

// Alias:

string Options::getArgument(int index) {
	return getArg(index);
}



//////////////////////////////
//
// Options::getArgCount --  number of arguments on command line.
//	does not count the options or the command name.
//

int Options::getArgCount(void) {
	return (int)m_arguments.size();
}

// Alias:

int Options::getArgumentCount(void) {
	return getArgCount();
}



//////////////////////////////
//
// Options::getArgList -- return a string vector of the arguments
//     after the options have been parsed out of it.  This list
//     excludes the command name (uses Options::getCommand() for that).
//

vector<string>& Options::getArgList(vector<string>& output) {
	output = m_arguments;
	return output;
}

// Alias:

vector<string>& Options::getArgumentList(vector<string>& output) {
	return getArgList(output);
}



//////////////////////////////
//
// Options::getBoolean --  returns true if the option was
//	used on the command line.
//

int Options::getBoolean(const string& optionName) {
	int index = getRegIndex(optionName);
	if (index < 0) {
		return 0;
	}
	return m_optionRegister[index]->isModified();
}



//////////////////////////////
//
// Options::getCommand -- returns argv[0] (the first string
//     in the original argv list.
//

string Options::getCommand(void) {
	if (m_argv.empty()) {
		return "";
	} else {
		return m_argv[0];
	}
}



//////////////////////////////
//
// Options::getCommandLine -- returns a string which contains the
//     command-line call to the program, including any appended
//     options.  This command only works after .process() is run.
//

string Options::getCommandLine(void) {
	string output;
	for (int i=0; i<(int)m_argv.size(); i++) {
		// check for how " and ' are dealt with in m_arguments...
		output += m_argv[i];
		if ((int)output.size() < (int)m_argv.size() - 1) {
			output += ' ';
		}
	}
	return output;
}



//////////////////////////////
//
// Options::getDefinition -- returns the definition for the specified
//      option name.  Returns empty string if there is no entry for
//      the option name.  spaces count in the input option name.
//

string Options::getDefinition(const string& optionName) {
	auto it = m_optionList.find(optionName);
	if (it == m_optionList.end()) {
		return "";
	} else {
		return m_optionRegister[it->second]->getDefinition();
	}
}



//////////////////////////////
//
// Options::getDouble -- returns the double float associated
//	with the given option.  Returns 0 if there is no
//	number associated with the option.
//

double Options::getDouble(const string& optionName) {
	return strtod(getString(optionName).c_str(), (char**)NULL);
}



//////////////////////////////
//
// Options::getChar -- Return the first character in the option string;
//      If the length is zero, then return '\0'.
//

char Options::getChar(const string& optionName) {
	return getString(optionName).c_str()[0];
}



//////////////////////////////
//
// Options::getFloat -- Return the floating point number
//	associated with the given option.
//

float Options::getFloat(const string& optionName) {
	return (float)getDouble(optionName);
}



//////////////////////////////
//
// Options::getInt -- Return the integer argument.  Can handle
//	hexadecimal, decimal, and octal written in standard
//	C syntax.
//

int Options::getInt(const string& optionName) {
	return (int)strtol(getString(optionName).c_str(), (char**)NULL, 0);
}

int Options::getInteger(const string& optionName) {
	return getInt(optionName);
}



//////////////////////////////
//
// Options::getString -- Return the option argument string.
//

string Options::getString(const string& optionName) {
	int index = getRegIndex(optionName);
	if (index < 0) {
		return "UNKNOWN OPTION";
	} else {
		return m_optionRegister[index]->getOption();
	}
}



//////////////////////////////
//
// Options::optionsArg -- Return true if --options is present
//    on the command line, otherwise returns false.
//

int Options::optionsArg(void) {
	return m_optionsArgQ;
}



//////////////////////////////
//
// Options::print -- Print a list of the defined options.
//

ostream& Options::print(ostream& out) {
	for (unsigned int i=0; i<m_optionRegister.size(); i++) {
		out << m_optionRegister[i]->getDefinition() << "\t"
			  << m_optionRegister[i]->getDescription() << endl;
	}
	return out;
}



//////////////////////////////
//
// Options::reset -- Clear all defined options.
//

void Options::reset(void) {
	m_argv.clear();
	m_arguments.clear();

	for (int i=0; i<(int)m_optionRegister.size(); i++) {
		delete m_optionRegister[i];
		m_optionRegister[i] = NULL;
	}
	m_optionRegister.clear();
}



//////////////////////////////
//
// Options::getFlag -- Set the character which is usually set to a dash.
//

char Options::getFlag(void) {
	return m_optionFlag;
}



//////////////////////////////
//
// Options::setFlag -- Set the character used to indicate an
//	option.  For unix this is usually '-', in MS-DOS,
//	this is usually '/';  But the syntax of the Options
//	class is for Unix-style options.
//

void Options::setFlag(char aFlag) {
	m_optionFlag = aFlag;
}



//////////////////////////////
//
// Options::setModified --
//

void Options::setModified(const string& optionName, const string& aString) {
	int index = getRegIndex(optionName);
	if (index < 0) {
		return;
	}

	m_optionRegister[getRegIndex(optionName)]->setModified(aString);
}



//////////////////////////////
//
// Options::setOptions --  Store the input list of options.
//

void Options::setOptions(int argc, char** argv) {
	m_processedQ = 0;
	m_argv.resize(argc);
	for (int i=0; i<argc; i++) {
		m_argv[i] = argv[i];
	}
}


void Options::setOptions(const vector<string>& argv) {
	m_processedQ = 0;
	m_argv = argv;
}


void Options::setOptions(string& args) {
	m_processedQ = 0;
   m_argv = tokenizeCommandLine(args);
}



//////////////////////////////
//
// Options::appendOptions -- Add argc and argv data to the current
//      list residing inside the Options class variable.
//

void Options::appendOptions(int argc, char** argv) {
	m_processedQ = 0;
	for (int i=0; i<argc; i++) {
		m_argv.push_back(argv[i]);
	}
}


void Options::appendOptions(vector<string>& argv) {
	m_processedQ = 0;
	for (int i=0; i<(int)argv.size(); i++) {
		m_argv.push_back(argv[i]);
	}
}



//////////////////////////////
//
// Options::appendOptions -- parse the string like command-line arguments.
//   Either double or single quotes can be used to encapsulate
//   a command-line token.  If double quotes are used to encapsulate,
//   then you will not have to back quote single quotes inside the
//   token string, but you will have to backslash double quotes:
//      "-T \"\"" but "-T ''"
//   Likewise for single quotes in reverse with double quotes:
//      '-T \'\'' is equal to: '-T ""'
//

void Options::appendOptions(string& args) {
	vector<string> arglist = tokenizeCommandLine(args);
	appendOptions(arglist);
}



//////////////////////////////
//
// Options::tokenizeCommandLine -- Parse a string for individual
//    command-line strings.
//

vector<string> Options::tokenizeCommandLine(string& arguments) {
	char ch;
	int doublequote = 0;
	int singlequote = 0;

	vector<string> tokens;
	vector<string> tempargv;
	string tempvalue;

	tokens.reserve(100);
	tempargv.reserve(100);
	tempvalue.reserve(1000);

	for (int i=0; i<(int)arguments.size(); i++) {
		if (!singlequote && (arguments[i] == '"')) {
			if ((i>0) && (arguments[i-1] != '\\')) {
				doublequote = !doublequote;
				if (doublequote == 0) {
					// finished a doublequoted section of data, so store
					// even if it is the empty string
					ch = '\0';
					tempvalue += (ch);
					tokens.push_back(tempvalue);
					tempvalue.clear();
					continue;
				} else {
					// don't store the leading ":
					continue;
				}
			}
		} else if (!doublequote && (arguments[i] == '\'')) {
			if ((i>0) && (arguments[i-1] != '\\')) {
				singlequote = !singlequote;
				if (singlequote == 0) {
					// finished a singlequote section of data, so store
					// even if it is the empty string
					ch = '\0';
					tempvalue += ch;
					tokens.push_back(tempvalue);
					tempvalue.clear();
					continue;
				} else {
					// don't store the leading ":
					continue;
				}
			}
		}

		if ((!doublequote && !singlequote) && std::isspace(arguments[i])) {
			if (tempvalue.size() > 0) {
				// tempvalue += ch;
				tokens.push_back(tempvalue);
				tempvalue.clear();
			}
		} else {
			ch = arguments[i];
			tempvalue += ch;
		}
	}
	if (tempvalue.size() > 0) {
		tokens.push_back(tempvalue);
		tempvalue.clear();
	}

	return tokens;

}



//////////////////////////////
//
// Options:getType -- Return the type of the option.
//

char Options::getType(const string& optionName) {
	int index = getRegIndex(optionName);
	if (index < 0) {
		return -1;
	} else {
		return m_optionRegister[getRegIndex(optionName)]->getType();
	}
}



//////////////////////////////
//
// Options::process -- Same as xverify.
//   	default values: error_check = 1, suppress = 0;
//

bool Options::process(int argc, char** argv, int error_check, int suppress) {
	setOptions(argc, argv);
	xverify(error_check, suppress);
	return !hasParseError();
}


bool Options::process(const vector<string>& argv, int error_check, int suppress) {
	setOptions(argv);
	xverify(error_check, suppress);
	return !hasParseError();
}


bool Options::process(string& argv, int error_check, int suppress) {
	setOptions(argv);
	xverify(error_check, suppress);
	return !hasParseError();
}


bool Options::process(int error_check, int suppress) {
	xverify(error_check, suppress);
	return !hasParseError();
}



//////////////////////////////
//
// Options::xverify --
//	default value: error_check = 1, suppress = 0;
//

void Options::xverify(int argc, char** argv, int error_check, int suppress) {
	setOptions(argc, argv);
	xverify(error_check, suppress);
}


void Options::xverify(int error_check, int suppress) {
	m_options_error_checkQ = error_check;
	m_suppressQ = suppress ? true : false;


	// if calling xverify again, must remove previous argument list.
	if (m_arguments.size() != 0) {
		m_arguments.clear();
	}

	int position   = 0;
	int running    = 0;
	bool optionend = false;
	int i          = 1;
	int oldi;
	int terminate = 1000; // for malformed options (missing arguments)
	int tcount = 0;

	while ((i < (int)m_argv.size()) && !optionend) {
		tcount++;
		if (tcount > terminate) {
			m_error << "Error: missing option argument" << endl;
			break;
		}
		if (isOption(m_argv[i], i)) {
			oldi = i;
			i = storeOption(i, position, running);
			if (i != oldi) {
				running = 0;
				position = 0;
			}
		} else {
			if (m_argv[i].size() == 2 && m_argv[i][0] == getFlag() &&
				m_argv[i][2] == getFlag() ) {
					optionend = 1;
				i++;
				break;
			} else {                          // this is an argument
				m_arguments.push_back(m_argv[i]);
				i++;
			}
		}
		if (hasParseError()) {
			break;
		}
	}
}





///////////////////////////////////////////////////////////////////////////
//
// private functions
//


//////////////////////////////
//
// Options::getRegIndex -- returns the index of the option associated
//	with this name.
//

int Options::getRegIndex(const string& optionName) {
	if (m_suppressQ && (optionName == "options")) {
			return -1;
	}

	if (optionName == "options") {
		print(cout);
		return -1;
	}

	auto it = m_optionList.find(optionName);
	if (it == m_optionList.end()) {
		if (m_options_error_checkQ) {
			m_error << "Error: unknown option \"" << optionName << "\"." << endl;
			print(cout);
			return -1;
		} else {
			return -1;
		}
	} else {
		return it->second;
	}
}



//////////////////////////////
//
// Options::isOption --  returns true if the string is an option.
//	"--" is not an option, also '-' is not an option.
//	aString is assumed to not be NULL.
//

bool Options::isOption(const string& aString, int& argp) {
	if (aString[0] == getFlag()) {
		if (aString[1] == '\0') {
			argp++;
			return false;
		} else if (aString[1] == getFlag()) {
			if (aString[2] == '\0') {
				argp++;
				return false;
			} else {
				return true;
			}
		} else {
			return true;
		}
	} else {
		return false;
	}
}



//////////////////////////////
//
// Options::storeOption --
//

#define OPTION_FORM_SHORT     0
#define OPTION_FORM_LONG      1
#define OPTION_FORM_CONTINUE  2

int Options::storeOption(int index, int& position, int& running) {
	int optionForm;
	char tempname[1024];
	char optionType = '\0';

	if (running) {
		optionForm = OPTION_FORM_CONTINUE;
	} else if (m_argv[index][1] == getFlag()) {
		optionForm = OPTION_FORM_LONG;
	} else {
		optionForm = OPTION_FORM_SHORT;
	}

	switch (optionForm) {
		case OPTION_FORM_CONTINUE:
			position++;
			tempname[0] = m_argv[index][position];
			tempname[1] = '\0';
			optionType = getType(tempname);
			if (optionType != OPTION_BOOLEAN_TYPE) {
				running = 0;
				position++;
			}
			break;
		case OPTION_FORM_SHORT:
			position = 1;
			tempname[0] = m_argv[index][position];
			tempname[1] = '\0';
			optionType = getType(tempname);
			if (optionType != OPTION_BOOLEAN_TYPE) {
				position++;
			}
			break;
		case OPTION_FORM_LONG:
			position = 2;
			while (m_argv[index][position] != '=' &&
					m_argv[index][position] != '\0') {
				tempname[position-2] = m_argv[index][position];
				position++;
			}
			tempname[position-2] = '\0';
			optionType = getType(tempname);
			if (optionType == -1) {         // suppressed --options option
				m_optionsArgQ = 1;
				break;
			}
			if (m_argv[index][position] == '=') {
				if (optionType == OPTION_BOOLEAN_TYPE) {
					m_error << "Error: boolean variable cannot have any options: "
						  << tempname << endl;
					return -1;
				}
				position++;
			}
			break;
	}

	if (optionType == -1) {              // suppressed --options option
		m_optionsArgQ = 1;
		index++;
		position = 0;
		return index;
	}

	if (m_argv[index][position] == '\0' &&
			optionType != OPTION_BOOLEAN_TYPE) {
		index++;
		position = 0;
	}

	if (optionForm != OPTION_FORM_LONG && optionType == OPTION_BOOLEAN_TYPE &&
			m_argv[index][position+1] != '\0') {
		running = 1;
	} else if (optionType == OPTION_BOOLEAN_TYPE &&
			m_argv[index][position+1] == '\0') {
		running = 0;
	}

	if (index >= (int)m_argv.size()) {
		m_error << "Error: last option requires a parameter" << endl;
		return -1;
	}
	setModified(tempname, &m_argv[index][position]);

	if (!running) {
		index++;
	}
	return index;
}



//////////////////////////////
//
// Options::printOptionList --
//

ostream& Options::printOptionList(ostream& out) {
	for (auto it = m_optionList.begin(); it != m_optionList.end(); it++) {
		out << it->first << "\t" << it->second << endl;
	}
	return out;
}



//////////////////////////////
//
// Options::printOptionBooleanState --
//

ostream& Options::printOptionListBooleanState(ostream& out) {
	for (auto it = m_optionList.begin(); it != m_optionList.end(); it++) {
		out << it->first << "\t"
			 << m_optionRegister[it->second]->isModified() << endl;
	}
	return out;
}



//////////////////////////////
//
// Options::printRegister --
//

ostream& Options::printRegister(ostream& out) {
	for (auto it = m_optionRegister.begin(); it != m_optionRegister.end(); it++) {
		(*it)->print(out);
	}
	return out;
}



/////////////////////////////
//
// Options::hasParseError -- Returns true if there was an error parsing
//     the arguments.
//

bool Options::hasParseError(void) {
	return !m_error.str().empty();
}



//////////////////////////////
//
// Options::getParseError --
//

string Options::getParseError(void) {
	return m_error.str();
}


ostream& Options::getParseError(ostream& out) {
	out << m_error.str();
	return m_error;
}




/////////////////////////////////
//
// Tool_autobeam::Tool_autobeam -- Set the recognized options for the tool.
//

Tool_autobeam::Tool_autobeam(void) {
	define("k|kern=i:0",    "process specific kern spine number");
	define("t|track=i:0",   "process specific track number");
	define("r|remove=b",    "remove all beams");
	define("o|overwrite=b", "over-write existing stems");
}



/////////////////////////////////
//
// Tool_autobeam::run -- Primary interfaces to the tool.
//

bool Tool_autobeam::run(const string& indata, ostream& out) {
	HumdrumFile infile(indata);
	bool status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_autobeam::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}

//
// In-place processing of file:
//

bool Tool_autobeam::run(HumdrumFile& infile) {
	initialize(infile);
	if (getBoolean("remove")) {
		removeBeams(infile);
	} else {
		addBeams(infile);
	}
	// Re-load the text for each line from their tokens.
	infile.createLinesFromTokens();
	return true;
}



//////////////////////////////
//
// Tool_autobeam::removeBeams --
//

void Tool_autobeam::removeBeams(HumdrumFile& infile) {
	int strands = infile.getStrandCount();
	HTp endtok;
	HTp starttok;
	HTp token;
	int track;
	bool bfound = false;
	string newstr;
	for (int i=0; i<strands; i++) {
		if (m_track > 0) {
			track = infile.getStrandStart(i)->getTrack();
			if (track != m_track) {
				continue;
			}
		}
		starttok = infile.getStrandStart(i);

		if (!starttok->isKern()) {
			continue;
		}
		endtok   = infile.getStrandEnd(i);
		token    = starttok;

		while (token && (token != endtok)) {
			if (!token->isData()) {
				token = token->getNextToken();
				continue;
			}
			if (token->isNull()) {
				token = token->getNextToken();
				continue;
			}

			bfound = false;
			newstr.clear();
			for (int i=0; i<(int)((string)(*token)).size(); i++) {
				switch (((string)(*token))[i]) {
					case 'L':
					case 'J':
					case 'K':
					case 'k':
						bfound = true;
						break;
					default:
						newstr += ((string)(*token))[i];
				}
			}
			if (bfound) {
				((string)(*token))[0] = 'X';
				token->swap(newstr);
			}
			token = token->getNextToken();
		}
	}
}



//////////////////////////////
//
// Tool_autobeam::addBeams --
//

void Tool_autobeam::addBeams(HumdrumFile& infile) {
	int strands = infile.getStrandCount();
	int track;
	for (int i=0; i<strands; i++) {
		if (m_track > 0) {
			track = infile.getStrandStart(i)->getTrack();
			if (track != m_track) {
				continue;
			}
		}
		HTp starttok = infile.getStrandStart(i);
		if (!starttok->isKern()) {
			continue;
		}
		processStrand(infile.getStrandStart(i), infile.getStrandEnd(i));
	}
}



//////////////////////////////
//
// Tool_autobeam::initialize -- extract time signature lines for
//    each **kern spine in file.
//

void Tool_autobeam::initialize(HumdrumFile& infile) {
	m_kernspines = infile.getKernSpineStartList();
	vector<HTp>& ks = m_kernspines;
	m_timesigs.resize(infile.getTrackCount() + 1);
	for (int i=0; i<(int)ks.size(); i++) {
		infile.getTimeSigs(m_timesigs[ks[i]->getTrack()], ks[i]->getTrack());
	}
	m_overwriteQ = getBoolean("overwrite");
	m_track = getInteger("track");
	if ((m_track == 0) && getBoolean("kern")) {
		int ks = getInteger("kern") - 1;
		vector<HTp> kernspines = infile.getKernSpineStartList();
		if ((ks >= 0) && (ks <(int)kernspines.size())) {
			m_track = kernspines[ks]->getTrack();
		}
	}
}



//////////////////////////////
//
// Tool_autobeam::processStrand -- Add beams to a single strand.
//

void Tool_autobeam::processStrand(HTp strandstart, HTp strandend) {
	HTp token = strandstart;
	vector<HTp> measure;
	while (token && (token != strandend)) {
		if (token->isBarline()) {
			processMeasure(measure);
			measure.clear();
			token = token->getNextToken();
			continue;
		}
		if (!token->isData()) {
			token = token->getNextToken();
			continue;
		}
		if (token->isNull()) {
			token = token->getNextToken();
			continue;
		}
		measure.push_back(token);
		token = token->getNextToken();
	}

}



//////////////////////////////
//
// Tool_autobeam::processMeasure -- Need to deal with rests starting or ending
//    a beamed group.
//

void Tool_autobeam::processMeasure(vector<HTp>& measure) {
	if (measure.empty()) {
		return;
	}

	vector<HumNum> beatsize;
	vector<HumNum> beatpos;
	vector<HumNum> notedurnodots;

	// default beat duration is a quarter note.
	pair<int, HumNum> current;
	current.first = 1;
	current.second = 4;
	HumNum beatdur(1);

	// First, get the beat positions of all notes in the measure:
	vector<pair<int, HumNum> >& timesig = m_timesigs[measure[0]->getTrack()];
	for (int i=0; i<(int)measure.size(); i++) {
		int line = measure[i]->getLineIndex();
		if ((current.first != timesig[line].first) ||
		    (current.second != timesig[line].second)) {
			current = timesig[line];
			beatdur = 1;
			beatdur /= current.second;
			beatdur *= 4; // convert to quarter-notes units from whole-notes.
			if ((current.first % 3 == 0) && (current.first != 3)) {
				// compound meter, so shift the beat to 3x the demoniator
				beatdur *= 3;
			} else if (current.first == 3 && (current.second > 4)) {
				// time signatures such as 3/8 and 3/16 which should
				// beam together at the measure level (3/4 not included).
				beatdur *= 3;
			}
		}
		beatsize.push_back(beatdur);
		notedurnodots.push_back(measure[i]->getDurationNoDots());
		beatpos.push_back(measure[i]->getDurationFromBarline() / beatdur);
	}

	// Now identify notes which should be beamed together
	// (using lazy beaming for now).
	HumNum eighthnote(1, 2);
	int beat1;
	int beat2;
	#define INVALID -1000000
	int beamstart = INVALID;
	for (int i=0; i<(int)measure.size(); i++) {
		if (notedurnodots[i] > eighthnote) {
			// note does not need a beam, but first check if an open
			// beam should be closed:
			if ((beamstart >= 0) && (i - beamstart > 1)) {
				addBeam(measure[beamstart], measure[i-1]);
				beamstart = INVALID;
				continue;
			}
			beamstart = INVALID;
			continue;
		}

		if (beamstart == INVALID) {
			// possible beam start
			beamstart = i;
			beat1 = (int)beatpos[i].getFloat();
			continue;
		}

		beat2 = (int)beatpos[i].getFloat();
		if (beat1 == beat2) {
			// note should be added to current beam, but could
			// be beams to notes after it, so continue on to next note.
			continue;
		} else {
			// check if previous beam should be ended.
			if ((beamstart >= 0) && (i - beamstart > 1)) {
				addBeam(measure[beamstart], measure[i-1]);
				beamstart = i;
				beat1 = (int)beatpos[i].getFloat();
				continue;
			}
			beamstart = i;
			beat1 = (int)beatpos[i].getFloat();
		}

		// Note should not be attached to previous beam.

		// The current note should not be beamed to current note,
		// but perhaps will be beams to the following notes.
		beamstart = i;
		beat1 = (int)beatpos[i].getFloat();
	}

	// close the last beam
	if ((beamstart >= 0) && ((int)measure.size() - beamstart > 1)) {
		addBeam(measure[beamstart], measure[measure.size()-1]);
		beamstart = INVALID;
	}
}



//////////////////////////////
//
// Tool_autobeam::addBeam -- Lazy beaming for now.
//

void Tool_autobeam::addBeam(HTp startnote, HTp endnote) {
	if (!m_overwriteQ) {
		HTp token = startnote;
		while (token && (token != endnote)) {
			if (token->hasBeam()) {
				return;
			}
			token = token->getNextToken();
		}
	}
	startnote->push_back('L');
	endnote->push_back('J');
}




/////////////////////////////////
//
// Tool_autostem::Tool_autostem -- Set the recognized options for the tool.
//

Tool_autostem::Tool_autostem(void) {
	define("d|debug=b",       "Debugging information");
	define("r|remove=b",      "Remove stems");
	define("R|removeall=b",   "Remove all stems including explicit beams");
	define("o|overwrite|replace=b","Overwrite non-explicit stems in input");
	define("O|overwriteall|replaceall=b",  "Overwrite all stems in input");
	define("L|no-long|not-long|not-longs=b",
	       "Do not put stems one whole notes or breves");
	define("u|up=b",          "Middle note on staff has stem up");
	define("p|pos=b",         "Display only note vertical positions on staves");
	define("v|voice=b",       "Display only voice/layer information");
	define("author=b",        "Program author");
	define("version=b",       "Program version");
	define("example=b",       "Program examples");
	define("h|help=b",        "Short description");
}



/////////////////////////////////
//
// Tool_autostem::run -- Primary interfaces to the tool.
//

bool Tool_autostem::run(const string& indata, ostream& out) {
	HumdrumFile infile(indata);
	int status = run(infile, out);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_autostem::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}

//
// In-place processing of file:
//

bool Tool_autostem::run(HumdrumFile& infile) {
	initialize(infile);
	if (m_quit) {
		return true;
	}
	if (removeQ || overwriteQ) {
		removeStems(infile);
		if (removeQ) {
			infile.createLinesFromTokens();
			return true;
		}
	}
	autostem(infile);
	// Re-load the text for each line from their tokens.
	infile.createLinesFromTokens();
	return true;
}



//////////////////////////////
//
// Tool_autostem::initialize --
//

void Tool_autostem::initialize(HumdrumFile& infile) {
	// handle basic options:

	if (getBoolean("author")) {
		m_free_text << "Written by Craig Stuart Sapp, "
			  << "craig@ccrma.stanford.edu, December 2010" << endl;
		m_quit = true;
	} else if (getBoolean("version")) {
		m_free_text << getCommand() << ", version: 26 December 2010" << endl;
		m_free_text << "compiled: " << __DATE__ << endl;
		m_quit = true;
	} else if (getBoolean("help")) {
		usage();
		m_quit = true;
	} else if (getBoolean("example")) {
		example();
		m_quit = true;
	}

	debugQ        = getBoolean("debug");
	removeQ       = getBoolean("remove");
	removeallQ    = getBoolean("removeall");
	noteposQ      = getBoolean("pos");
	voiceQ        = getBoolean("voice");
	overwriteQ    = getBoolean("overwrite");
	overwriteallQ = getBoolean("overwriteall");
	notlongQ      = getBoolean("no-long");

	if (getBoolean("up")) {
		Middle = 4;
		Borderline = 1;
	}
	removeallQ = getBoolean("removeall");
	if (removeallQ) {
		removeQ = 1;
	}
	if (overwriteallQ) {
		overwriteQ = 1;
	}
}



//////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// Tool_autostem::removeStems --
//

void Tool_autostem::removeStems(HumdrumFile& infile) {
	for (int i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			HumRegex hre;
			// string& buffer = static_cast<string&>(*infile.token(i, j));
			if (removeallQ || overwriteallQ) {
				hre.replaceDestructive(infile.token(i, j), "", "[\\\\/]x(?!x)", "g");
				hre.replaceDestructive(infile.token(i, j), "", "[\\\\/](?!x)", "g");
			} else {
				hre.replaceDestructive(infile.token(i, j), "", "[\\\\/](?!x)", "g");
			}
		}
	}
}



//////////////////////////////
//
// Tool_autostem::autostem -- add an up/down stem on notes in **kern data
//     that do not already have stem information.
//

void Tool_autostem::autostem(HumdrumFile& infile) {
	vector<vector<int> > baseline;
	getClefInfo(baseline, infile);

	// get staff-line position of all notes:
	vector<vector<vector<int> > > notepos;
	getNotePositions(notepos, baseline, infile);
	if (noteposQ) {
		printNotePositions(infile, notepos);
		return;
	}

	// get voice/layer number in track:
	vector<vector<int> > voice;
	getVoiceInfo(voice, infile);
	if (voiceQ) {
		printVoiceInfo(infile, voice);
		return;
	}

	// get stem directions:
	vector<vector<int> > stemdir;
	assignStemDirections(stemdir, voice, notepos, infile);
	insertStems(infile, stemdir);
}



//////////////////////////////
//
// Tool_autostem::insertStems -- put stem directions into the data.
//

void Tool_autostem::insertStems(HumdrumFile& infile,
		vector<vector<int> >& stemdir) {
	for (int i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			setStemDirection(infile, i, j, stemdir[i][j]);
		}
	}
}



//////////////////////////////
//
// Tool_autostem::setStemDirection -- don't change stem direction if
//    there is already a stem on the note.
//

void Tool_autostem::setStemDirection(HumdrumFile& infile, int row, int col,
			int direction) {
	int& i = row;
	int& j = col;

	if (infile.token(i, j)->isNull()) {
		return;
	}
	if (infile.token(i, j)->isRest()) {
		return;
	}

	if (notlongQ) {
		// Don't print stems on whole notes and breves.
		// Duration units are in quarter notes.
		HumNum value = Convert::recipToDuration(*infile.token(row,col));
		double duration = value.getFloat();
		if ((duration >= 4.0) && (duration < 16.0)) {
			return;
		}
	}

	string buffer;
	string output;
	int tokencount = infile.token(i, j)->getSubtokenCount();
	for (int k=0; k<tokencount; k++) {
		buffer = infile.token(i, j, k);
		if ((!Convert::contains(buffer, '/')) &&
		    (!Convert::contains(buffer, '\\'))) {
			if (direction > 0) {
				addStem(buffer, "/");
			} else if (direction < 0) {
				addStem(buffer, "\\");
			}
		}
		output += buffer;
		output += ' ';
	}
	if (output.back() == ' ') {
		output.pop_back();
	}
	infile.token(i, j)->setText(output);
}



//////////////////////////////
//
// Tool_autostem::assignStemDirections --
//

void Tool_autostem::assignStemDirections(vector<vector<int> >& stemdir,
		vector<vector<int> > & voice,
		vector<vector<vector<int> > >& notepos, HumdrumFile& infile) {

	// the dimensions are not correct:
	stemdir.resize(infile.getLineCount());
	for (int i=0; i<(int)stemdir.size(); i++) {
		stemdir[i].resize(infile[i].getFieldCount());
		fill(stemdir[i].begin(), stemdir[i].end(), 0);
	}

	vector<int> maxlayer;
	getMaxLayers(maxlayer, voice, infile);

	assignBasicStemDirections(stemdir, voice, notepos, infile);

	vector<vector<string > > beamstates;
	getBeamState(beamstates, infile);

	vector<vector<Coord> > beamednotes;
	getBeamSegments(beamednotes, beamstates, infile, maxlayer);

	// print notes which are beamed together for debugging:

	if (debugQ) {
		for (int i=0; i<(int)beamednotes.size(); i++) {
			m_humdrum_text << "!! ";
			for (int j=0; j<(int)beamednotes[i].size(); j++) {
				m_humdrum_text << infile[beamednotes[i][j].i][beamednotes[i][j].j] << "\t";
			}
			m_humdrum_text << "\n";
		}
	}

	int direction;
	for (int i=0; i<(int)beamednotes.size(); i++) {
		direction = getBeamDirection(beamednotes[i], voice, notepos);
		setBeamDirection(stemdir, beamednotes[i], direction);
	}

	if (debugQ) {
		cerr << "STEM DIRECTION ASSIGNMENTS ==================" << endl;
		for (int i=0; i<(int)stemdir.size(); i++) {
			for (int j=0; j<(int)stemdir[i].size(); i++) {
				cerr << stemdir[i][j] << "\t";
			}
			cerr << endl;
		}
	}
}



//////////////////////////////
//
// Tool_autostem::setBeamDirection --
//

void Tool_autostem::setBeamDirection(vector<vector<int> >& stemdir,
		vector<Coord>& bnote, int direction) {
	int x;
	int i, j;
	for (x=0; x<(int)bnote.size(); x++) {
		i = bnote[x].i;
		j = bnote[x].j;
		stemdir[i][j] = direction;
	}
}




//////////////////////////////
//
// Tool_autostem::getBeamDirection -- return a consensus stem direction
//     for beamed notes.
//

int Tool_autostem::getBeamDirection(vector<Coord>& coords,
		vector<vector<int> >& voice, vector<vector<vector<int> > >& notepos) {

	// voice values are presumed to be 0 at the moment.

	int minn = 1000;
	int maxx = -1000;

	int x;
	int i, j, k;
	for (x=0; x<(int)coords.size(); x++) {
		i = coords[x].i;
		j = coords[x].j;
		if (voice[i][j] == 1) {
			return +1;
		}
		if (voice[i][j] == 2) {
			return -1;
		}
		for (k=0; k<(int)notepos[i][j].size(); k++) {
			if (minn > notepos[i][j][k]) {
				minn = notepos[i][j][k];
			}
			if (maxx < notepos[i][j][k]) {
				maxx = notepos[i][j][k];
			}
		}
	}

	if (maxx < 0 + Borderline) {
		// both minn and maxx are less than zero, so place stems up
		return +1;
	}
	if (minn > 0) {
		// both minn and maxx are greater than zero, so place stems down
		return -1;
	}

	if (abs(maxx) > abs(minn)) {
		// highest note is higher than lower note is lower, so place
		// stems down
		return -1;
	}
	if (abs(maxx) > abs(minn)) {
		// highest note is lower than lower note is lower, so place
		// stems up
		return +1;
	}

	// its a draw, so place stem up.
	return +1;
}



//////////////////////////////
//
// Tool_autostem::getBeamSegments -- arrange the beamed notes into
//     a long list with each entry being a list of notes containing one beam.
//     Each beamed note set should have their beams all pointing in the same
//     direction.
//

void Tool_autostem::getBeamSegments(vector<vector<Coord> >& beamednotes,
		vector<vector<string > >& beamstates, HumdrumFile& infile,
		vector<int> maxlayer) {
	beamednotes.clear();
	beamednotes.reserve(10000);
	vector<vector<vector<Coord> > > beambuffer;
	beambuffer.resize(infile.getMaxTrack() + 1);
	int i, j;
	for (i=0; i<(int)beambuffer.size(); i++) {
		beambuffer[i].resize(10); // layer  max 10, all more later if needed
		for (j=0; j<(int)beambuffer[i].size(); j++) {
			beambuffer[i][j].reserve(1000);
		}
	}

	Coord tcoord;
	char beamchar;
	int track, oldtrack, layer;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		oldtrack = 0;
		layer = 0;
		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			track = infile.token(i, j)->getTrack();
			if (track == oldtrack) {
				layer++;
			} else {
				layer = 0;
			}
			oldtrack = track;
			if (infile.token(i, j)->isNull()) {
				continue;
			}
			if (infile.token(i, j)->isRest()) {
				continue;
			}
			beamchar = beamstates[i][j][0];
			if (beamchar == '\0') {
				beambuffer[track][layer].resize(0);  // possible unter. beam
				continue;
			}
			if ((beamchar == '[') || (beamchar == '=')) {
				// add a beam to the buffer and wait for more
				tcoord.i = i;
				tcoord.j = j;
				beambuffer[track][layer].push_back(tcoord);
				continue;
			}
			if (beamchar == ']') {
				// ending of a beam so store in permanent storage
				tcoord.i = i;
				tcoord.j = j;
				beambuffer[track][layer].push_back(tcoord);
				beamednotes.push_back(beambuffer[track][layer]);
				beambuffer[track][layer].resize(0);
			}
		}
	}
}



//////////////////////////////
//
// Tool_autostem::getMaxLayers --
//

void Tool_autostem::getMaxLayers(vector<int>& maxlayer,
		vector<vector<int> >& voice, HumdrumFile& infile) {

	int track;
	maxlayer.resize(infile.getMaxTrack() + 1);
	fill(maxlayer.begin(), maxlayer.end(), 0);
	int i, j;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			if (infile.token(i, j)->isNull()) {
				continue;
			}
			if (infile.token(i, j)->isRest()) {
				continue;
			}
			track = infile.token(i, j)->getTrack();
			if (voice[i][j] + 1 > maxlayer[track]) {
				maxlayer[track] = voice[i][j] + 1;
			}
		}
	}
}



//////////////////////////////
//
// Tool_autostem::getVoiceInfo -- 0 = only voice in track, 1 = layer 1,
//     2 = layer 2, etc.
//
// 0 voices will be stemmed up or down based on vertical positions of notes
// 1 voices will be stemmed up always
// 2 voices will be stemmed down always.
// 3 and higher are still to be determined.
//
// Future enhancement of this algorithm: if one voice contains an invisible
// rest, then it will be ignored in the voice calculation.
//

void Tool_autostem::getVoiceInfo(vector<vector<int> >& voice,
		HumdrumFile& infile) {

	voice.resize(infile.getLineCount());

	int i, j, v;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		voice[i].resize(infile[i].getFieldCount());
		fill(voice[i].begin(), voice[i].end(), -1);
		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			if (infile.token(i, j)->isNull()) {
				continue;
			}
			v = getVoice(infile, i, j);
			voice[i][j] = v;
		}
	}
}



//////////////////////////////
//
// Tool_autostem::printVoiceInfo --
//

void Tool_autostem::printVoiceInfo(HumdrumFile& infile,
		vector<vector<int> >& voice) {
	vector<string> voiceinfo(infile.getLineCount());
	for (int i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			if (infile.token(i, j)->isNull()) {
				continue;
			}
			if (infile.token(i, j)->isRest()) {
				continue;
			}
			voiceinfo[i] += voice[i][j];
		}
		if (voiceinfo[i].back() == ' ') {
			voiceinfo[i].pop_back();
		}
	}
	infile.appendDataSpine(voiceinfo, "", "**voice");
}




//////////////////////////////
//
// Tool_autostem::printNotePositions -- prints the vertical position of notes on the
//    staves.  Mostly for debugging purposes.  A spine at the end of the
//    data will be added containing all positions for notes on the line
//    in the sequence in which the notes occur from left to right.
//
//    The middle line of a 5-line staff is the zero position, and
//    position values are diatonic steps above or below that level:
//
//    ===== +4
//          +3
//    ===== +2
//          +1
//    =====  0
//          -1
//    ===== -2
//          -3
//    ===== -4
//

void Tool_autostem::printNotePositions(HumdrumFile& infile,
		vector<vector<vector<int> > >& notepos) {
	vector<string> posinfo(infile.getLineCount());
	for (int i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			if (infile.token(i, j)->isNull()) {
				// ignore null tokens
				continue;
			}
			if (infile.token(i, j)->isRest()) {
				// ignore rests
				continue;
			}
			for (int k=0; k<(int)notepos[i][j].size(); k++) {
				posinfo[i] += notepos[i][j][k];
			}
			if (posinfo[i].back() == ' ') {
				posinfo[i].pop_back();
			}
		}
	}
	infile.appendDataSpine(posinfo, "", "**vpos");
}



//////////////////////////////
//
// Tool_autostem::getNotePositions -- Extract the vertical position of the notes
// on the staves, with the centerline of the staff being the 0 position
// and each diatonic step equal to 1, so that lines of 5-lined staff are
// at positions from bottom to top: -4, -2, 0, +2, +4.
//

void Tool_autostem::getNotePositions(vector<vector<vector<int> > >& notepos,
		vector<vector<int> >& baseline, HumdrumFile& infile) {

	notepos.resize(infile.getLineCount());

	int location;
	string buffer;
	int i, j, k;
	int tokencount;

	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		notepos[i].resize(infile[i].getFieldCount());
		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			if (infile.token(i, j)->isNull()) {
				// ignore null-tokens
				continue;
			}
			if (infile.token(i, j)->isRest()) {
				// ignore rests
				continue;
			}

			tokencount = infile.token(i, j)->getSubtokenCount();
			notepos[i][j].resize(tokencount);
			for (k=0; k<tokencount; k++) {
				buffer = infile.token(i, j)->getSubtoken(k);
				location = Convert::kernToBase7(buffer) -
						baseline[i][j] - 4;
				notepos[i][j][k] = location;
			}
		}
	}
}



//////////////////////////////
//
// Tool_autostem::processKernTokenStems --
//

void Tool_autostem::processKernTokenStems(HumdrumFile& infile,
		vector<vector<int> >& baseline, int row, int col) {
	exit(1);
}



//////////////////////////////
//
// Tool_autostem::assignBasicStemDirections -- don't take beams into
//     consideration.
//

void Tool_autostem::assignBasicStemDirections(vector<vector<int> >& stemdir,
		vector<vector<int> >& voice, vector<vector<vector<int> > >& notepos,
		HumdrumFile& infile) {

	int i, j;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			if (infile.token(i, j)->isNull()) {
				continue;
			}
			if (infile.token(i, j)->isRest()) {
				continue;
			}
			if (removeQ) {
				removeStem2(infile, i, j);
			}

			if (Convert::contains(infile.token(i, j), '/')) {
				stemdir[i][j] = +1;
			} else if (Convert::contains(infile.token(i, j), '\\')) {
				stemdir[i][j] = -1;
			} else {
				stemdir[i][j] = determineChordStem(voice, notepos, infile, i, j);
			}
		}
	}
}



//////////////////////////////
//
// Tool_autostem::determineChordStem --
//

int Tool_autostem::determineChordStem(vector<vector<int> >& voice,
		vector<vector<vector<int> > >& notepos, HumdrumFile& infile, int row,
		int col) {

	if (notepos[row][col].size() == 0) {
		return 0;
	}

	if (voice[row][col] == 1) {
		return +1;
	}
	if (voice[row][col] == 2) {
		return -1;
	}
	if (voice[row][col] == 3) {
		return +1;
	}
	// voice == 0 means determine by vertical position

	if (notepos[row][col].size() == 1) {
		int location = notepos[row][col][0];
		if (location >= 0 + Borderline) {
			return -1;
		} else {
			return +1;
		}
	}

	// chord with more than one note so choose the extreme note as the
	// one which decides the direction

	int i;
	int minn = notepos[row][col][0];
	int maxx = notepos[row][col][0];
	for (i=1; i<(int)notepos[row][col].size(); i++) {
		if (minn > notepos[row][col][i]) {
			minn = notepos[row][col][i];
		}
		if (maxx < notepos[row][col][i]) {
			maxx = notepos[row][col][i];
		}
	}

	if (maxx < 0 + Borderline) {
		// all stems want to point upwards:
		return +1;
	}
	if (minn > 0) {
		// all stems want to point downwards:
		return -1;
	}

	if (abs(maxx) > abs(minn)) {
		return -1;
	}
	if (abs(minn) > abs(maxx)) {
		return +1;
	}

	return +1;
}



//////////////////////////////
//
// Tool_autostem::processKernTokenStemsSimpleModel --
//

void Tool_autostem::processKernTokenStemsSimpleModel(HumdrumFile& infile,
		vector<vector<int> >& baseline, int row, int col) {
	int& i = row;
	int& j = col;
	int tokencount = infile.token(i, j)->getSubtokenCount();

	HumNum duration;
	if (tokencount == 1) {
		duration = Convert::recipToDuration(*infile.token(i, j));
		if (duration >= 4) {
			// whole note or larger for note/chord, to not append a stem
			return;
		}
		if (Convert::contains(infile.token(i, j), '/')) {
			if (removeallQ || overwriteallQ) {
				if (Convert::contains(infile.token(i, j), "/x")) {
					if (Convert::contains(infile.token(i, j), "/xx")) {
						return;
					}
				} else if (Convert::contains(infile.token(i, j), "\\x")) {
					if (Convert::contains(infile.token(i, j), "\\xx")) {
						return;
					}
				}
			} else if (removeallQ || overwriteallQ) {
				removeStem2(infile, i, j);
			} else {
				// nothing to do
				return;
			}
		}
		if (infile.token(i, j)->isRest()) {
			// rest which does not have a stem
			return;
		}
	}

	if (removeQ) {
		removeStem2(infile, i, j);
	}

	int voice = getVoice(infile, row, col);
	int location;
	string buffer;
	string output;
	for (int k=0; k<tokencount; k++) {
		buffer = infile.token(i, j, k);
		if (i == 0) {
			duration = Convert::recipToDuration(buffer);
			// if (duration >= 4) {
			//    // whole note or larger for note/chord, do not append a stem
			//    return;
			// }
		}
		if (!(Convert::contains(infile.token(i, j), '/') ||
				Convert::contains(infile.token(i, j), '\\'))) {
			location = Convert::kernToBase7(buffer) -
					baseline[row][col] - Middle;
		  if (voice == 1) {
			  addStem(buffer, "/");
			} else if (voice == 2) {
				addStem(buffer, "\\");
			} else {
				addStem(buffer, location > 0 ? "\\" : "/");
			}
			output += buffer;
			output += ' ';
		} else {
			output += buffer;
			output += ' ';
		}
	}
	if (output.back() == ' ') {
		output.pop_back();
	}
	infile.token(i, j)->setText(output);
}



//////////////////////////////
//
// Tool_autostem::getVoice -- return 0 if the only spine in primary track, otherwise, return
// the nth column offset from 1 in the primary track.
//

int Tool_autostem::getVoice(HumdrumFile& infile, int row, int col) {
	int output = 0;
	int tcount = 0;
	int track = infile.token(row, col)->getTrack();
	int j;
	int testtrack;
	for (j=0; j<infile[row].getFieldCount(); j++) {
		testtrack = infile.token(row, j)->getTrack();
		if (testtrack == track) {
			tcount++;
		}
		if (col == j) {
			output = tcount;
		}
	}
	if (tcount == 1) {
		output = 0;
	}
	return output;
}



//////////////////////////////
//
// Tool_autostem::removeStem2 -- remove stem and any single x after the stem.
//

void Tool_autostem::removeStem2(HumdrumFile& infile, int row, int col) {
	HumRegex hre;
	hre.replaceDestructive(infile.token(row, col), "", "[\\\\/]x(?!x)", "g");
	hre.replaceDestructive(infile.token(row, col), "", "[\\\\/](?!x)", "g");
}



//////////////////////////////
//
// Tool_autostem::addStem --
//

void Tool_autostem::addStem(string& input, const string& piece) {
	string output;
	HumRegex hre;
	if (hre.search(input, "(.*[ABCDEFG][n#-]*)(.*)$", "i")) {
		output = hre.getMatch(1);
		output += piece;
		output += hre.getMatch(2);
	} else {
		output = input;
		output += piece;
	}
	input = output;
}



///////////////////////////////
//
// Tool_autostem::getClefInfo -- Identify the clef of each note in the score.
//     Does not consider the case where a primary track contains more
//     than one clef at a time (but that should not reasonable happen
//     in more scores).
//

void Tool_autostem::getClefInfo(vector<vector<int> >& baseline,
		HumdrumFile& infile) {
	vector<int> states(infile.getMaxTrack()+1,
			Convert::kernClefToBaseline("*clefG2"));
	baseline.resize(infile.getLineCount());

	int track;
	for (int i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isInterpretation()) {
			for (int j=0; j<infile[i].getFieldCount(); j++) {
				if (!infile.token(i, j)->isKern()) {
					continue;
				}
				if (infile.token(i, j)->compare(0, 5, "*clef") == 0) {
					track = infile.token(i, j)->getTrack();
					states[track] = Convert::kernClefToBaseline(*infile.token(i, j));
				}
			}
		}
		if (!infile[i].isData()) {
			continue;
		}
		baseline[i].resize(infile[i].getFieldCount());
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			track = infile.token(i, j)->getTrack();
			baseline[i][j] = states[track];
		}
	}
}



//////////////////////////////
//
// Tool_autostem::example -- example function calls to the program.
//

void Tool_autostem::example(void) {
	m_error_text << getCommand() << " file.krn" << endl;
}



//////////////////////////////
//
// Tool_autostem::usage -- command-line usage description and brief summary
//

void Tool_autostem::usage(void) {
	m_error_text << "Usage: " << getCommand() << " [file(s)] " << endl;
}



//////////////////////////////
//
// Tool_autostem::getBeamState -- Analyze structure of beams and store note layout
//      directives at the same time.
//
// Type          Humdrum     MuseData
// start          L           [
// continue                   =
// end            J           ]
// forward hook   K           /
// backward hook  k           \  x
//

void Tool_autostem::getBeamState(vector<vector<string > >& beams,
		HumdrumFile& infile) {
	int len;
	int contin;
	int start;
	int stop;
	int flagr;
	int flagl;
	int track;
	HumNum rn;

	vector<vector<int> > beamstate;   // state of beams in tracks/layers
	vector<vector<int> > gracestate;  // independents state for grace notes

	string gbinfo;

	beamstate.resize(infile.getMaxTrack() + 1);
	gracestate.resize(infile.getMaxTrack() + 1);
	for (int i=0; i<(int)beamstate.size(); i++) {
		beamstate[i].resize(100);     // maximum of 100 layers in each track...
		gracestate[i].resize(100);    // maximum of 100 layers in each track...
		fill(beamstate[i].begin(), beamstate[i].end(), 0);
		fill(gracestate[i].begin(), gracestate[i].end(), 0);
	}

	beams.resize(infile.getLineCount());
	vector<int> curlayer;
	curlayer.resize(infile.getMaxTrack() + 1);
	vector<int> laycounter;

	for (int i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isBarline()) {
			// don't allow beams across barlines.  Mostly for
			// preventing buggy beams from propagating...
			for (int t=1; t<=infile.getMaxTrack(); t++) {
				fill(beamstate[t].begin(), beamstate[t].end(), 0);
				fill(gracestate[t].begin(), gracestate[t].end(), 0);
			}
		}

		if (!infile[i].isData() && !infile[i].isBarline()) {
			continue;
		}

		if (!infile[i].isData()) {
			continue;
		}

		beams[i].resize(infile[i].getFieldCount());
		for (int j=0; j<(int)beams[i].size(); j++) {
			beams[i][j].resize(1);
			beams[i][j][0] = '\0';
		}

		fill(curlayer.begin(), curlayer.end(), 0);
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			track = infile.token(i, j)->getTrack();
			curlayer[track]++;
			if (infile.token(i, j)->isNull()) {
				continue;
			}
			if (infile.token(i, j)->isRest()) {
				// ignore rests.  Might be useful to not ignore
				// rests if beams extend over rests...
				continue;
			}
			rn = Convert::recipToDuration(*infile.token(i, j));
			if (rn >= 1) {
				beamstate[track][curlayer[track]] = 0;
				continue;
			}
			if (rn == 0) {
				// grace notes;
				countBeamStuff(infile.token(i, j)->c_str(), start, stop, flagr, flagl);
				if ((start != 0) && (stop != 0)) {
					cerr << "Funny error in grace note beam calculation" << endl;
					exit(1);
				}
				if (start > 7) {
					cerr << "Too many beam starts" << endl;
				}
				if (stop > 7) {
					cerr << "Too many beam ends" << endl;
				}
				if (flagr > 7) {
					cerr << "Too many beam flagright" << endl;
				}
				if (flagl > 7) {
					cerr << "Too many beam flagleft" << endl;
				}
				contin = gracestate[track][curlayer[track]];
				contin -= stop;
				gbinfo.clear();
				gbinfo.resize(contin);
				for (int ii=0; ii<contin; ii++) {
					gbinfo[ii] = '=';
				}
				if (start > 0) {
					for (int ii=0; ii<start; ii++) {
						gbinfo += "[";
					}
				} else if (stop > 0) {
					for (int ii=0; ii<stop; ii++) {
						gbinfo += "]";
					}
				}
				for (int ii=0; ii<flagr; ii++) {
					gbinfo += "/";
				}
				for (int ii=0; ii<flagl; ii++) {
					gbinfo += "\\";
				}
				len = (int)gbinfo.size();
				if (len > 6) {
					cerr << "Error too many grace note beams" << endl;
					exit(1);
				}
				beams[i][j] = gbinfo;
				gracestate[track][curlayer[track]] = contin;
				gracestate[track][curlayer[track]] += start;

			} else {
				// regular notes which are shorter than a quarter note
				// (including tuplet quarter notes which should be removed):

				countBeamStuff(infile.token(i, j)->c_str(), start, stop, flagr, flagl);
				if ((start != 0) && (stop != 0)) {
					cerr << "Funny error in note beam calculation" << endl;
					exit(1);
				}
				if (start > 7) {
					cerr << "Too many beam starts" << endl;
				}
				if (stop > 7) {
					cerr << "Too many beam ends" << endl;
				}
				if (flagr > 7) {
					cerr << "Too many beam flagright" << endl;
				}
				if (flagl > 7) {
					cerr << "Too many beam flagleft" << endl;
				}
				contin = beamstate[track][curlayer[track]];
				contin -= stop;
				gbinfo.resize(contin);
				for (int ii=0; ii<contin; ii++) {
					gbinfo[ii] = '=';
				}
				if (start > 0) {
					for (int ii=0; ii<start; ii++) {
						gbinfo += "[";
					}
				} else if (stop > 0) {
					for (int ii=0; ii<stop; ii++) {
						gbinfo += "[";
					}
				}
				for (int ii=0; ii<flagr; ii++) {
					gbinfo += "/";
				}
				for (int ii=0; ii<flagl; ii++) {
					gbinfo += "\\";
				}
				len = (int)gbinfo.size();
				if (len > 6) {
					cerr << "Error too many grace note beams" << endl;
					exit(1);
				}
				beams[i][j] = gbinfo;
				beamstate[track][curlayer[track]] = contin;
				beamstate[track][curlayer[track]] += start;
			}
		}
	}
}



//////////////////////////////
//
// Tool_autostem::countBeamStuff --
//

void Tool_autostem::countBeamStuff(const string& token, int& start, int& stop,
		int& flagr, int& flagl) {
	start = stop = flagr = flagl = 0;
	for (int i=0; i<(int)token.size(); i++) {
		switch (token[i]) {
			case 'L': start++;  break;
			case 'J': stop++;   break;
			case 'K': flagr++;  break;
			case 'k': flagl++;  break;
		}
	}
}



#define EMPTY_ID ""
#define REST 0
#define RESTINT -1000000
#define RESTSTRING "R"
#define INTERVAL_HARMONIC 1
#define INTERVAL_MELODIC  2
#define MARKNOTES  1


/////////////////////////////////
//
// Tool_cint::Tool_cint -- Set the recognized options for the tool.
//

Tool_cint::Tool_cint(void) {
	define("base-40|base40|b40|40=b", "display pitches/intervals in base-40");
	define("base-12|base12|b12|12=b", "display pitches/intervals in base-12");
	define("base-7|base7|b7|7|diatonic=b", "display pitches/intervals in base-7");
	define("g|grid|pitch|pitches=b", "display pitch grid used to calculate modules");
	define("r|rhythm=b", "display rhythmic positions of notes");
	define("f|filename=b", "display filenames with --count");
	define("raw=b", "display only modules without formatting");
	define("raw2=b", "display only modules formatted for Vishesh");
	define("c|uncross=b", "uncross crossed voices when creating modules");
	define("k|koption=s:", "Select only two spines to analyze");
	define("C|comma=b", "separate intervals by comma rather than space");
	define("retro|retrospective=b", "Retrospective module display in the score");
	define("suspension|suspensions=b", "mark suspensions");
	define("rows|row=b", "display lattices in row form");
	define("dur|duration=b", "display durations appended to harmonic interval note attacks");
	define("id=b", "ids are echoed in module data");
	define("L|interleaved-lattice=b", "display interleaved lattices");
	define("q|harmonic-parentheses=b", "put square brackets around harmonic intervals");
	define("h|harmonic-marker=b", "put h character after harmonic intervals");
	define("m|melodic-marker=b", "put m character after melodic intervals");
	define("y|melodic-parentheses=b", "put curly braces around melodic intervals");
	define("p|parentheses=b", "put parentheses around modules intervals");
	define("l|lattice=b", "calculate lattice");
	define("loc|location=b", "displayLocation");
	define("s|sustain=b", "display sustain/attack states of notes");
	define("o|octave=b", "reduce compound intervals to within an octave");
	define("H|no-harmonic=b", "don't display harmonic intervals");
	define("M|no-melodic=b", "don't display melodic intervals");
	define("t|top=b", "display top melodic interval of modules");
	define("T|top-only=b", "display only top melodic interval of modules");
	define("U|no-melodic-unisons=b", "no melodic perfect unisons");
	define("attacks|attack=b", "start/stop module chains on pairs of note attacks");
	define("z|zero=b", "display diatonic intervals with 0 offset");
	define("N|note-marker=s:@", "pass-through note marking character");
	define("x|xoption=b", "display attack/sustain information on harmonic intervals only");
	define("n|chain=i:1", "number of sequential modules");
	define("R|no-rest|no-rests|norest|norests=b", "number of sequential modules");
	define("O|octave-all=b", "transpose all harmonic intervals to within an octave");
	define("chromatic=b", "display intervals as diatonic intervals with chromatic alterations");
	define("search=s:", "search string");
	define("mark=b", "mark matches notes from searches in data");
	define("count=b", "count matched modules from search query");
	define("debug=b");              // determine bad input line num
	define("author=b");             // author of program
	define("version=b");            // compilation info
	define("example=b");            // example usages
	define("help=b");               // short description
}



/////////////////////////////////
//
// Tool_cint::run -- Primary interfaces to the tool.
//

bool Tool_cint::run(const string& indata, ostream& out) {
	HumdrumFile infile(indata);
	bool status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_cint::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);

	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}

	return status;
}

//
// In-place processing of file:
//

bool Tool_cint::run(HumdrumFile& infile) {
	processFile(infile);


	if (hasAnyText()) {
		// getAllText(cout);
	} else {
		// Re-load the text for each line from their tokens.
		cout << infile;
	}

	return true;
}


///////////////////////////////////////////////////////////////////////////
//
// NoteNode class functions:
//

NoteNode::NoteNode(const NoteNode& anode) {
	b40        = anode.b40; 
	line       = anode.line; 
	spine      = anode.spine; 
	measure    = anode.measure; 
	serial     = anode.serial; 
	mark       = anode.mark; 
	notemarker = anode.notemarker; 
	beatsize   = anode.beatsize; 
	duration   = 0;
	protected_id = anode.protected_id;
}


NoteNode& NoteNode::operator=(NoteNode& anode) {
	if (this == &anode) {
		return *this;
	}
	b40        = anode.b40; 
	line       = anode.line; 
	spine      = anode.spine; 
	measure    = anode.measure; 
	serial     = anode.serial; 
	mark       = anode.mark; 
	notemarker = anode.notemarker; 
	beatsize   = anode.beatsize; 
	duration   = anode.duration;
	protected_id = anode.protected_id;
	return *this;
}


void NoteNode::setId(const string& anid) {
	protected_id = anid;
}


NoteNode::~NoteNode(void) {
	// do nothing
}


void NoteNode::clear(void) { 
	mark = measure = beatsize = serial = b40 = 0; 
	notemarker = 0; 
	line = spine = -1; 
	protected_id = "";
}


string NoteNode::getId(void) {
	return protected_id;
}


///////////////////////////////////////////////////////////////////////////
//
// Tool_cint functions:
//


//////////////////////////////
//
// Tool_cint::processFile -- Do requested analysis on a given file.
//

int Tool_cint::processFile(HumdrumFile& infile) {

   initialize();

	vector<vector<NoteNode> > notes;
	vector<string> names;
	vector<int>    ktracks;
	vector<HTp>    kstarts;
	vector<int>    reverselookup;

	infile.getSpineStartList(kstarts, "**kern");
	ktracks.resize(kstarts.size());
	for (int i=0; i<(int)kstarts.size(); i++) {
		ktracks[i] = kstarts[i]->getTrack();
	}

	if (koptionQ) {
		adjustKTracks(ktracks, getString("koption"));
	}
	notes.resize(ktracks.size());
	reverselookup.resize(infile.getTrackCount()+1);
	fill(reverselookup.begin(), reverselookup.end(), -1);

	vector<vector<string> > retrospective;
	if (retroQ) {
		initializeRetrospective(retrospective, infile, ktracks);
	}

//	if (locationQ || rhythmQ || durationQ) {
//		infile.analyzeRhythm();
//	}

	int i;
	for (i=0; i<(int)ktracks.size(); i++) {
		reverselookup[ktracks[i]] = i;
		// notes[i].reserve(infile.getLineCount());
		notes[i].resize(0);
	}

	getNames(names, reverselookup, infile);
	HumRegex pre;
	extractNoteArray(notes, infile, ktracks, reverselookup);

	if (pitchesQ) {
		printPitchGrid(notes, infile); 
		exit(0);
	}

	int count = 0;
	if (latticeQ) {
		printLattice(notes, infile, ktracks, reverselookup, Chaincount);
	} else if (interleavedQ) {
		printLatticeInterleaved(notes, infile, ktracks, reverselookup, 
			Chaincount);
	} else if (suspensionsQ) {
		count = printCombinationsSuspensions(notes, infile, ktracks, 
				reverselookup, Chaincount, retrospective);
	} else {
		count = printCombinations(notes, infile, ktracks, reverselookup, 
				Chaincount, retrospective, SearchString);
	}


	// handle search results here
	if (markQ) {
		if (count > 0) {
			addMarksToInputData(infile, notes, ktracks, reverselookup);
		}
		infile.createLinesFromTokens();
		m_humdrum_text << infile;
		m_humdrum_text << "!!!RDF**kern: @ = matched note, color=\"#ff0000\"\n";
	} 

	if (debugQ) { 
		int j;
		for (i=0; i<(int)retrospective[0].size(); i++) {
			for (j=0; j<(int)retrospective.size(); j++) {
				m_humdrum_text << retrospective[j][i];
				if (j < (int)retrospective.size() - 1) {
					m_humdrum_text << "\t";
				}
			}
			m_humdrum_text << "\n";
		}
	}

	return count;
}



//////////////////////////////
//
// Tool_cint::adjustKTracks -- Select only two spines to do analysis on.
//

void Tool_cint::adjustKTracks(vector<int>& ktracks, const string& koption) {
	HumRegex pre;
	if (!pre.search(koption, "(\\$|\\$?\\d*)[^\\$\\d]+(\\$|\\$?\\d*)")) {
		return;
	}
	int number1 = 0;
	int number2 = 0;
	HumRegex pre2;

	if (pre2.search(pre.getMatch(1), "\\d+")) { 
		number1 = pre.getMatchInt(1);
		if (pre.getMatch(1).find('$') != string::npos) {
			number1 = (int)ktracks.size() - number1;
		}
	} else {
		number1 = (int)ktracks.size();
	}

	if (pre2.search(pre.getMatch(2), "\\d+")) { 
		number2 = pre.getMatchInt(2);
		if (pre.getMatch(2).find('$') != string::npos) {
			number2 = (int)ktracks.size() - number2;
		}
	} else {
		number2 = (int)ktracks.size();
	}

	number1--;
	number2--;

	int track1 = ktracks[number1];
	int track2 = ktracks[number2];

	ktracks.resize(2);
	ktracks[0] = track1;
	ktracks[1] = track2;
}



//////////////////////////////
//
// Tool_cint::initializeRetrospective --
//

void Tool_cint::initializeRetrospective(vector<vector<string> >& retrospective, 
		HumdrumFile& infile, vector<int>& ktracks) {

	int columns = (int)ktracks.size();
	columns = columns * (columns + 1) / 2; // triangle number of analysis cols.

	retrospective.resize(columns);
	int i, j;

	for (i=0; i<(int)retrospective.size(); i++) {
		retrospective[i].resize(infile.getLineCount());
	}

	string token;
	for (i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isLocalComment()) {
			token = "!";
		} else if (infile[i].isGlobalComment()) {
			token = "!";
		} else if (infile[i].isReference()) {
			token = "!!";
		} else if (infile[i].isBarline()) {
			token = *infile.token(i, 0);
		} else if (infile[i].isData()) {
			token = ".";
		} else if (infile[i].isInterpretation()) {
			token = "*";
			if (infile[i].isExclusiveInterpretation()) {
				token = "**cint";
			}
		}

		for (j=0; j<(int)retrospective.size(); j++) {
			retrospective[j][i] = token;
		}
	}

	if (debugQ) {
		for (i=0; i<(int)retrospective[0].size(); i++) {
			for (j=0; j<(int)retrospective.size(); j++) {
				m_humdrum_text << retrospective[j][i];
				if (j < (int)retrospective.size() - 1) {
					m_humdrum_text << "\t";
				}
			}
			m_humdrum_text << "\n";
		}
	}
}



//////////////////////////////
//
// Tool_cint::printCombinationsSuspensions --
//
// have to do something with sbuffer
//

int  Tool_cint::printCombinationsSuspensions(vector<vector<NoteNode> >& notes, 
		HumdrumFile& infile, vector<int>& ktracks, vector<int>& reverselookup, 
		int n, vector<vector<string> >& retrospective) {

	string sbuffer;

	int oldcountQ = countQ;
	countQ = 1;             // mostly used to suppress intermediate output

	int countsum = 0;

	searchQ    = 1;               // turn on searching

	// Suspensions with length-2 modules
	n = 2;                        // -n 2
	xoptionQ   = 1;               // -x
	sbuffer = "";

	sbuffer += "^7xs 1 6sx -2 8xx$";
	sbuffer += "|^2sx -2 3xs 2 1xx$";
	sbuffer += "|^7xs 1 6sx 2 6xx$";
	sbuffer += "|^11xs 1 10sx -5 15xx$";
	sbuffer += "|^4xs 1 3sx -5 8xx$";
	sbuffer += "|^2sx -2 3xs 2 3xx$";

	// "9xs 1 8sx -2 10xx" archetype: Jos1405 m10 A&B
	sbuffer += "|^9xs 1 8sx -2 10xx$";
	// "4xs 1 3sx 5xx" archetype: Jos1713 m87-88 A&B
	sbuffer += "|^4xs 1 3sx -2 5xx$";
	// "11xs 1 10sx 4 8xx" archetype: Jos1402 m23-24 S&B
	sbuffer += "|^11xs 1 10sx 4 8xx$";

	countsum += printCombinations(notes, infile, ktracks, reverselookup, n,
			retrospective, sbuffer);

	// Suspensions with length-3 modules /////////////////////////////////
	n = 3;                        // -n 3
	xoptionQ   = 1;               // -x
	sbuffer = "";

	// "7xs 1 6sx 1 5sx 1 6sx" archetype: Jos2721 m27-78 S&T
	sbuffer += "^7xs 1 6sx 1 5sx 1 6sx$";
	// "7xs 1 6sx 1 6sx -2 8xx" archetype: Rue2018 m38-88 S&T
	sbuffer += "|^7xs 1 6sx 1 6sx -2 8xx$";
	// "11xs 1 10sx 1 10sx -5 15xx" archetype: Rue2018 m38-88 S&B
	sbuffer += "|^11xs 1 10sx 1 10sx -5 15xx$";

	countsum += printCombinations(notes, infile, ktracks, reverselookup, n,
							retrospective, sbuffer);

	// Suspensions with length-5 modules /////////////////////////////////
	n = 5;                        // -n 2
	xoptionQ   = 1;               // -x
	sbuffer = "";
	// "8xs 1 7sx 1 7sx 1 6sx 1 6sx 1 5sx -1 8xx" archetype: Duf3015a m94 S&T
	sbuffer += "^8xs 1 7sx 1 7sx 1 6sx 1 5sx -2 8xx$";
 
	countsum += printCombinations(notes, infile, ktracks, reverselookup, n,
							retrospective, sbuffer);

	// Suspensions with rests modules

	// done with multiple searches.  Mark the notes in the score if required.

	countQ = oldcountQ;

	return countsum;
}



//////////////////////////////
//
// Tool_cint::printCombinations --
//

int  Tool_cint::printCombinations(vector<vector<NoteNode> >& notes, 
		HumdrumFile& infile, vector<int>& ktracks, vector<int>& reverselookup, 
		int n, vector<vector<string> >& retrospective, const string& searchstring) {
	int i;
	int currentindex = 0;
	int matchcount   = 0;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].hasSpines()) {
			// print all lines here which do not contain spine 
			// information.
			if (!(raw2Q || rawQ || markQ || retroQ || countQ)) {
				m_humdrum_text << infile[i] << "\n";
			}
			continue;
		}

		// At this point there are only four types of lines:
		//    (1) data lines
		//    (2) interpretation lines (lines starting with *)
		//    (3) local comment lines (lines starting with single !)
		//    (4) barlines

		if (infile[i].isInterpretation()) {
			string pattern = "*";
			if (infile.token(i, 0)->compare(0, 2, "**") == 0) {
				pattern = "**cint";
			} else if (*infile.token(i, 0) == "*-") {
				pattern = "*-";
			} else if (infile.token(i, 0)->compare(0, 2, "*>") == 0) {
				pattern = *infile.token(i, 0);
			}
			printAsCombination(infile, i, ktracks, reverselookup, pattern);
		} else if (infile[i].isLocalComment()) {
			printAsCombination(infile, i, ktracks, reverselookup, "!");
		} else if (infile[i].isBarline()) {
			printAsCombination(infile, i, ktracks, reverselookup, *infile.token(i, 0));
		} else {
			// print combination data
			currentindex = printModuleCombinations(infile, i, ktracks, 
				reverselookup, n, currentindex, notes, matchcount, retrospective, searchstring);
		}
		if (!(raw2Q || rawQ || markQ || retroQ || countQ)) {
				m_humdrum_text << "\n";
		}
	}

	return matchcount;
}



//////////////////////////////
//
// Tool_cint::printModuleCombinations --
//

int Tool_cint::printModuleCombinations(HumdrumFile& infile, int line, vector<int>& ktracks,
		vector<int>& reverselookup, int n, int currentindex, 
		vector<vector<NoteNode> >& notes, int& matchcount, 
		vector<vector<string> >& retrospective, const string& searchstring) {

	int fileline = line;
	string filename = infile.getFilename();

	while ((currentindex < (int)notes[0].size()) 
			&& (fileline > notes[0][currentindex].line)) {
		currentindex++;
	}
	if (currentindex >= (int)notes[0].size()) {
		if (!(raw2Q || rawQ || markQ || retroQ || countQ)) {
			m_humdrum_text << ".";
			printAsCombination(infile, line, ktracks, reverselookup, ".");
		}
		return currentindex;
	}
	if (notes[0][currentindex].line != fileline) {
		// This section occurs when two voices are both sustaining
		// at the start of the module.  Print a "." to indicate that
		// the counterpoint module is continuing from a previous line.
		printAsCombination(infile, line, ktracks, reverselookup, ".");
		return currentindex;
	}

	// found the index into notes which matches to the current fileline.
	if (currentindex + n >= (int)notes[0].size()) {
		// asking for chain longer than rest of available data.
		printAsCombination(infile, line, ktracks, reverselookup, ".");
		return currentindex;
	}

	// printAsCombination(infile, line, ktracks, reverselookup, ".");
	// return currentindex;

	int tracknext;
	int track;
	int j, jj;
	int count = 0;
	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			if (!(raw2Q || rawQ || markQ || retroQ || countQ)) {
				m_humdrum_text << infile.token(line, j);
				if (j < infile[line].getFieldCount() - 1) {
					m_humdrum_text << "\t";
				}
			}
			continue;
		}
		track = infile.token(line, j)->getTrack();
		if (j < infile[line].getFieldCount() - 1) {
			tracknext = infile.token(line, j+1)->getTrack();
		} else {
			tracknext = -23525;
		}
		if (track == tracknext) {
			if (!(raw2Q || rawQ || markQ || retroQ || countQ)) {
				m_humdrum_text << infile.token(line, j);
				if (j < infile[line].getFieldCount() - 1) {
					m_humdrum_text << "\t";
				}
			}
			continue;
		}

		// print the **kern spine, then check to see if there
		// is some **cint data to print
		if (!(raw2Q || rawQ || markQ || retroQ || countQ)) {
				m_humdrum_text << infile.token(line, j);
		}
		if ((track != ktracks.back()) && (reverselookup[track] >= 0)) {
			count = (int)ktracks.size() - reverselookup[track] - 1;
			for (jj = 0; jj<count; jj++) {
				if (!(raw2Q || rawQ || markQ || retroQ || countQ)) {
					m_humdrum_text << "\t";
				}
				int part1 = reverselookup[track];
				int part2 = part1+1+jj;
				// m_humdrum_text << part1 << "," << part2;
				matchcount += printCombinationModulePrepare(m_humdrum_text, filename, 
						notes, n, currentindex, part1, part2, retrospective, infile,
						searchstring);
			}
		}

		if (!(raw2Q || rawQ || markQ || retroQ || countQ)) {
			if (j < infile[line].getFieldCount() - 1) {
				m_humdrum_text << "\t";
			}
		}
	}

	return currentindex;
}



//////////////////////////////
//
// Tool_cint::printCombinationModulePrepare --
//

int Tool_cint::printCombinationModulePrepare(ostream& out, const string& filename,
		 vector<vector<NoteNode> >& notes, int n, int startline, int part1, 
		 int part2, vector<vector<string> >& retrospective, 
		HumdrumFile& infile, const string& searchstring) {
	int count = 0;
	HumRegex hre;
	stringstream tempstream;
	int match;
	char notemarker = '\0';
// ggg
	int status = printCombinationModule(tempstream, filename, notes, 
			n, startline, part1, part2, retrospective, notemarker);
	if (status) { 
		if (raw2Q || rawQ) {
			tempstream << "\n";
		}
		if (NoteMarker && (notemarker == NoteMarker)) {
			out << (char)NoteMarker;
		}
		if (searchQ) {
			// Check to see if the extracted module matches to the
			// search query.
			match = hre.search(tempstream.str(), searchstring);
			if (match) {
				count++;
				if (locationQ) {
					int line = notes[0][startline].line;
					double loc = infile[line].getDurationFromStart().getFloat() / 
							infile[infile.getLineCount()-1].getDurationFromStart().getFloat();
					loc = int(100.0 * loc + 0.5)/100.0;
					m_humdrum_text << "!!LOCATION:" 
							<< "\t"  << loc 
							<< "\tm" << getMeasure(infile, line)
							<< "\tv" << ((int)notes.size() - part2)
							<< ":v"  << ((int)notes.size() - part1)
							<< "\t"  << infile.getFilename() 
							<< endl;
				}
				if (raw2Q || rawQ) {
					out << tempstream.str();
					// newline already added somewhere previously.
					// m_humdrum_text << "\n";
				} else {
					// mark notes of the matched module(s) in the note array 
					// for later marking in input score.
					status = printCombinationModule(tempstream, filename, 
						 notes, n, startline, part1, part2, retrospective, 
						 notemarker, MARKNOTES);
					if (status && (raw2Q || rawQ)) {
						tempstream << "\n";
					}
				}

			}
		} else {
			if (retroQ) {
				int column = getTriangleIndex((int)notes.size(), part1, part2);
				retrospective[column][status] = tempstream.str();
			} else {
				out << tempstream.str();
			}
		}
	} else {
		if (!(raw2Q || rawQ || markQ || retroQ || countQ || searchQ)) {
			out << ".";
		}
	}

	return count;
}



//////////////////////////////
//
// Tool_cint::getMeasure -- return the last measure number of the given line index.
//

int Tool_cint::getMeasure(HumdrumFile& infile, int line) {
	int measure = 0;
	HumRegex hre;
	
	for (int i=line; i>=0; i--) {
		if (!infile[i].isBarline()) {
			continue;
		}
		if (hre.search(*infile.token(i, 0), "=(\\d+)")) {
			measure = hre.getMatchInt(1);
			return measure;
		}
	}
	return 0;
}



//////////////////////////////
//
// Tool_cint::getTriangleIndex --
//

int Tool_cint::getTriangleIndex(int number, int num1, int num2) {
	// int triangle = number * (number + 1) / 2;
	// intermediate code, not active yet
	return 0;
}



//////////////////////////////
//
// Tool_cint::addMarksToInputData -- mark notes in the score which matched
//     to the search query.
//

void Tool_cint::addMarksToInputData(HumdrumFile& infile, 
		vector<vector<NoteNode> >& notes, vector<int>& ktracks,
		vector<int>& reverselookup) {

	// first carry all marks from sustained portions of notes onto their
	// note attacks.
	int i, j;

	int mark = 0;
	int track = 0;
	int markpitch = -1;
 
	for (i=0; i<(int)notes.size(); i++) {
		mark = 0;
		for (j=(int)notes[i].size()-1; j>=0; j--) {
			if (mark && (-markpitch == notes[i][j].b40)) {
				// In the sustain region between a note
				// attack and the marked sustain. Mark the 
				// sustained region as well (don't know
				// if this behavior might change in the
				// future.
				notes[i][j].mark = mark; 
				continue;
			}
			if (mark && (markpitch == notes[i][j].b40)) {
				// At the start of a notes which was marked.
				// Mark the attack since only note attacks
				// will be marked in the score
				notes[i][j].mark = mark; 
				mark = 0;
				continue;
			}
			if (mark && (markpitch != notes[i][j].b40)) {
				// something strange happened.  Probably
				// an open tie which was not started
				// properly, so just clear mark.
				mark = 0;
			}
			if (notes[i][j].mark) {
				mark = 1;
				markpitch = abs(notes[i][j].b40);
			} else {
				mark = 0;
			}
			
		}
	}

	// a forward loop here into notes array to continue
	// marks to end of sutained region of marked notes
	for (i=0; i<(int)notes.size(); i++)  {
		for (j=0; j<(int)notes[i].size(); j++) {
			if (notes[i][j].mark) {
				markpitch = -abs(notes[i][j].b40);
				continue;
			} else if (notes[i][j].b40 == markpitch) {
				notes[i][j].mark = 1;
				continue;
			} else {
				markpitch = -1;
			}
		}
	}

	// print mark information:
	// for (j=0; j<(int)notes[0].size(); j++) {
	//    for (i=0; i<(int)notes.size(); i++) {
	//       m_humdrum_text << notes[i][j].b40;
	//       if (notes[i][j].mark) {
	//          m_humdrum_text << "m";
	//       }
	//       m_humdrum_text << " ";
	//    }
	//    m_humdrum_text << "\n";
	// }


	// now go through the input score placing user-markers onto notes
	// which were marked in the note array.
	int currentindex = 0;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		while ((currentindex < (int)notes[0].size()) 
				&& (i > notes[0][currentindex].line)) {
			currentindex++;
		}
		if (currentindex >= (int)notes[0].size()) {
			continue;
		}
		if (notes[0][currentindex].line != i) {
			continue;
		}

		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			if (*infile.token(i, j) == ".") {
				// Don't mark null tokens.
				continue;
			}
			track = infile.token(i, j)->getTrack();
			if (reverselookup[track] < 0) {
				continue;
			}
			if (notes[reverselookup[track]][currentindex].mark != 0) {
				markNote(infile, i, j);
			}
		}
	}
}



//////////////////////////////
//
// Tool_cint::markNote --
//

void Tool_cint::markNote(HumdrumFile& infile, int line, int col) {
	// string text = *infile.token(line, col);
	// text += "@";
	// infile.token(line, col)->setText(text);
	*infile.token(line, col) += "@";
}



//////////////////////////////
//
// Tool_cint::getOctaveAdjustForCombinationModule -- Find the minim harmonic interval in 
//      the module chain.  If it is greater than an octave, then move it down 
//      below an octave.  If the minimum is an octave, then don't do anything.
//      Not considering crossed voices.
//

int Tool_cint::getOctaveAdjustForCombinationModule(vector<vector<NoteNode> >& notes, int n, 
		int startline, int part1, int part2) {

	// if the current two notes are both sustains, then skip
	if ((notes[part1][startline].b40 <= 0) && 
		 (notes[part2][startline].b40 <= 0)) {
		return 0;
	}

	if (norestsQ) {
		if (notes[part1][startline].b40 == 0) {
			return 0;
		}
		if (notes[part2][startline].b40 == 0) {
			return 0;
		}
	}

	int i;
	int count = 0;
	int attackcount = 0;
	int hint;

	vector<int> hintlist;
	hintlist.reserve(1000);

	for (i=startline; i<(int)notes[0].size(); i++) {
		if ((notes[part1][i].b40 <= 0) && (notes[part2][i].b40 <= 0)) {
			// skip notes if both are sustained
			continue;
		}
  
		if (attackQ && ((notes[part1][i].b40 <= 0) || 
							 (notes[part2][i].b40 <= 0))) {
			if (attackcount == 0) {
				// not at the start of a pair of attacks.
				return 0;
			}
		}

		// consider  harmonic interval
		if ((notes[part2][i].b40 != 0) && (notes[part1][i].b40 != 0)) {
			hint = abs(notes[part2][i].b40) - abs(notes[part1][i].b40);
			if (uncrossQ && (hint < 0)) {
				hint = -hint;
			}
			hintlist.push_back(hint);
		}

		// if count matches n, then exit loop
		if ((count == n) && !attackQ) {
			break;
		} 
		count++;

		if ((notes[part1][i].b40 > 0) && (notes[part2][i].b40 > 0)) {
			// keep track of double attacks
			if (attackcount >= n) {
				break;
			} else {
				attackcount++;
			}
		}

	}

	int minimum = 100000;

	for (i=0; i<(int)hintlist.size(); i++) {
		if (hintlist[i] < minimum) {
			minimum = hintlist[i];
		}
	}

	if (minimum > 1000) {
	  // no intervals found to consider
	  return 0;
	}

	if ((minimum >= 0) && (minimum <= 40)) {
		// nothing to do
		return 0;
	}

	if (minimum > 40) {
		return -(minimum/40);
	} else if (minimum < 0) {
		// don't go positive, this will invert the interval.
		return (-minimum)/40;
	}

	//int octaveadjust = -(minimum / 40);

	//if (attackQ && (attackcount == n)) {
	//   return octaveadjust;
	//} else if (count == n) {
	//   return octaveadjust;
	//} else {
	//   // did not find the required number of modules.
	//   return 0;
	//}

	return 0;
}



//////////////////////////////
//
// Tool_cint::printCombinationModule -- Similar to printLatticeModule, but harmonic 
//      intervals will not be triggered by a pair of sustained notes.  
//      Print a counterpoint module or module chain given the start notes 
//      and pair of parts to calculate the module (chains) from.  Will not 
//      print anything if the chain length is longer than the note array.  
//      The n parameter will be ignored if --attacks option is used 
//      (--attacks will gnereate a variable length module chain).
//

int Tool_cint::printCombinationModule(ostream& out, const string& filename, 
		vector<vector<NoteNode> >& notes, int n, int startline, int part1, 
		int part2, vector<vector<string> >& retrospective, char& notemarker, 
		int markstate) {

	notemarker = '\0';

	if (norestsQ) {
		if (notes[part1][startline].b40 == 0) {
			return 0;
		}
		if (notes[part2][startline].b40 == 0) {
			return 0;
		}
	}

	stringstream idstream;

	// int crossing =  0;
	//int oldcrossing =  0;

	int octaveadjust = 0;   // used for -o option
	if (octaveQ) {
		octaveadjust = getOctaveAdjustForCombinationModule(notes, n, startline, 
				part1, part2);
	}

	ostream *outp = &out;
	// if (rawQ && !searchQ) {
	//    outp = &m_humdrum_text;
	// }

	if (n + startline >= (int)notes[0].size()) { // [20150202]
		// definitely nothing to do
		return 0;
	}

	if ((int)notes.size() == 0) {
		// nothing to do
		return 0;
	}

	// if the current two notes are both sustains, then skip
	if ((notes[part1][startline].b40 <= 0) && 
		 (notes[part2][startline].b40 <= 0)) {
		return 0;
	}

	if (raw2Q) { 
		// print pitch of first bottom note
		if (filenameQ) {
			(*outp) << "file_" << filename;
			(*outp) << " ";
		}
	  
		(*outp) << "v_" << part1 << " v_" << part2 << " ";
		
		if (base12Q) {
			(*outp) << "base12_";
			(*outp) << Convert::base40ToMidiNoteNumber(abs(notes[part1][startline].b40));
		} else if (base40Q) {
			(*outp) << "base40_";
			(*outp) << abs(notes[part1][startline].b40);
		} else {
			(*outp) << "base7_";
			(*outp) << Convert::base40ToDiatonic(abs(notes[part1][startline].b40));
		}
		(*outp) << " ";
	}

	if (parenQ) {
		(*outp) << "(";
	}

	int i;
	int count = 0;
	int countm = 0;
	int attackcount = 0;
	int idstart = 0;

	int lastindex = -1;
	int retroline = 0;

	for (i=startline; i<(int)notes[0].size(); i++) {
		if ((notes[part1][i].b40 <= 0) && (notes[part2][i].b40 <= 0)) {
			// skip notes if both are sustained
			continue;
		}

		if (norestsQ) {
			if (notes[part1][i].b40 == 0) {
				return 0;
			}
			if (notes[part2][i].b40 == 0) {
				return 0;
			}
		}
  
		if (attackQ && ((notes[part1][i].b40 <= 0) || 
							 (notes[part2][i].b40 <= 0))) {
			if (attackcount == 0) {
				// not at the start of a pair of attacks.
				return 0;
			}
		}

		// print the melodic intervals (if not the first item in chain)
		if ((count > 0) && !nomelodicQ) {
			if (mparenQ) {
				(*outp) << "{";
			}

			if (nounisonsQ) {
				// suppress modules which contain melodic perfect unisons:
				if ((notes[part1][i].b40 != 0) && 
					(abs(notes[part1][i].b40) == abs(notes[part1][lastindex].b40))) {
					return 0;
				}
				if ((notes[part2][i].b40 != 0) && 
					(abs(notes[part2][i].b40) == abs(notes[part2][lastindex].b40))) {
					return 0;
				}
			}
			// bottom melodic interval:
			if (!toponlyQ) {
				printInterval((*outp), notes[part1][lastindex], 
						notes[part1][i], INTERVAL_MELODIC);
				if (mmarkerQ) {
					(*outp) << "m";
				}
			}
	 
			// print top melodic interval here if requested
			if (topQ || toponlyQ) {
				if (!toponlyQ) {
					printSpacer((*outp));
				}
				// top melodic interval:
				printInterval((*outp), notes[part2][lastindex], 
						notes[part2][i], INTERVAL_MELODIC);
				if (mmarkerQ) {
					(*outp) << "m";
				}
			}
		
			if (mparenQ) {
				(*outp) << "}";
			}
			printSpacer((*outp));
		}

		countm++;

		// print harmonic interval
		if (!noharmonicQ) {
			if (hparenQ) {
			  (*outp) << "[";
			}
			if (markstate) {
				notes[part1][i].mark = 1;
				notes[part2][i].mark = 1;
			} else {
				// oldcrossing = crossing;
				//crossing = printInterval((*outp), notes[part1][i], 
				//      notes[part2][i], INTERVAL_HARMONIC, octaveadjust);
				printInterval((*outp), notes[part1][i], 
						notes[part2][i], INTERVAL_HARMONIC, octaveadjust);
			}

			if (durationQ) {
				if (notes[part1][i].isAttack()) {
					(*outp) << "D" << notes[part1][i].duration;
				}
				if (notes[part2][i].isAttack()) {
					(*outp) << "d" << notes[part1][i].duration;
				}
			}
		
			if (hmarkerQ) {
				(*outp) << "h";
			}
			if (hparenQ) {
			  (*outp) << "]";
			}
		}

		// prepare the ids string if requested
		if (idQ) {
		//   if (count == 0) {
				// insert both first two notes, even if sustain.
				if (idstart != 0) { idstream << ':'; }
				idstart++;
				idstream << notes[part1][i].getId() << ':' 
							<< notes[part2][i].getId();
		//   } else {
		//      // only insert IDs if an attack
		//      if (notes[part1][i].b40 > 0) {
		//         if (idstart != 0) { idstream << ':'; }
		//         idstart++;
		//         idstream << notes[part1][i].getId();
		//      }
		//      if (notes[part2][i].b40 > 0) {
		//         if (idstart != 0) { idstream << ':'; }
		//         idstart++;
		//         idstream << notes[part2][i].getId();
		//      }
		//   }
		}

		// keep track of notemarker state
		if (notes[part1][i].notemarker == NoteMarker) {
			notemarker = NoteMarker;
		}
		if (notes[part2][i].notemarker == NoteMarker) {
			notemarker = NoteMarker;
		}

		// if count matches n, then exit loop
		if ((count == n) && !attackQ) {
			retroline = i;
			break;
		} else {
			if (!noharmonicQ) {
				printSpacer((*outp));
			}
		}
		lastindex = i;
		count++;

		if ((notes[part1][i].b40 > 0) && (notes[part2][i].b40 > 0)) {
			// keep track of double attacks
			if (attackcount >= n) {
				retroline = i;
				break;
			} else {
				attackcount++;
			}
		}

	}

	if (parenQ) {
		(*outp) << ")";
	}

	if (idQ && idstart) {
		idstream << ends;
		(*outp) << " ID:" << idstream.str();
	}

	if (attackQ && (attackcount == n)) {
		return retroline;
	} else if ((countm>1) && (count == n)) {
		return retroline;
	} else if (n == 0) {
		return retroline;
	} else {
		// did not print the required number of modules.
		return 0;
	}

	return 0;
}



//////////////////////////////
//
// Tool_cint::printAsCombination --
//

void Tool_cint::printAsCombination(HumdrumFile& infile, int line, vector<int>& ktracks, 
	 vector<int>& reverselookup, const string& interstring) {

	if (raw2Q || rawQ || markQ || retroQ || countQ) {
		return;
	}

	vector<int> done(ktracks.size(), 0);
	int track;
	int tracknext;
	int count;

	int j, jj;
	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			m_humdrum_text << infile.token(line, j);
			if (j < infile[line].getFieldCount() - 1) {
				m_humdrum_text << "\t";
			}
			continue;
		}
		track = infile.token(line, j)->getTrack();
		if (j < infile[line].getFieldCount() - 1) {
			tracknext = infile.token(line, j+1)->getTrack();
		} else {
			tracknext = -23525;
		}
		if (track == tracknext) {
			m_humdrum_text << infile.token(line, j);
			if (j < infile[line].getFieldCount() - 1) {
				m_humdrum_text << "\t";
			}
			continue;
		}

		// print the **kern spine, then check to see if there
		// is some **cint data to print
		// ggg
		m_humdrum_text << infile.token(line, j);

		if (reverselookup[track] >= 0) {
			count = (int)ktracks.size() - reverselookup[track] - 1;
			for (jj=0; jj<count; jj++) {
				m_humdrum_text << "\t" << interstring;
			}
		}

		if (j < infile[line].getFieldCount() - 1) {
			m_humdrum_text << "\t";
		}
	}
}



//////////////////////////////
//
// Tool_cint::printLatticeInterleaved --
//

void Tool_cint::printLatticeInterleaved(vector<vector<NoteNode> >& notes, 
		HumdrumFile& infile, vector<int>& ktracks, vector<int>& reverselookup, 
		int n) {
	int currentindex = 0;
	int i;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].hasSpines()) {
			// print all lines here which do not contain spine 
			// information.
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << infile[i] << "\n";
			}
			continue;
		}

		// At this point there are only four types of lines:
		//    (1) data lines
		//    (2) interpretation lines (lines starting with *)
		//    (3) local comment lines (lines starting with single !)
		//    (4) barlines

		if (infile[i].isInterpretation()) {
			string pattern = "*";
			if (infile.token(i, 0)->compare(0, 2, "**") == 0) {
				pattern = "**cint";
			} else if (infile.token(i, 0)->compare("*-") == 0) {
				pattern = "*-";
			} else if (infile.token(i, 0)->compare(0, 2, "*>") == 0) {
				pattern = *infile.token(i, 0);
			}
			printInterleaved(infile, i, ktracks, reverselookup, pattern);
		} else if (infile[i].isLocalComment()) {
			printInterleaved(infile, i, ktracks, reverselookup, "!");
		} else if (infile[i].isBarline()) {
			printInterleaved(infile, i, ktracks, reverselookup, *infile.token(i, 0));
		} else {
			// print interleaved data
			currentindex = printInterleavedLattice(infile, i, ktracks, 
				reverselookup, n, currentindex, notes);
		}
		if (!(rawQ || raw2Q)) {
			m_humdrum_text << "\n";
		}
	}
}



//////////////////////////////
//
// Tool_cint::printInterleavedLattice --
//

int Tool_cint::printInterleavedLattice(HumdrumFile& infile, int line, vector<int>& ktracks,
		vector<int>& reverselookup, int n, int currentindex, 
		vector<vector<NoteNode> >& notes) {

	int fileline = line;

	while ((currentindex < (int)notes[0].size()) 
			&& (fileline > notes[0][currentindex].line)) {
		currentindex++;
	}
	if (currentindex >= (int)notes[0].size()) {
		if (!(rawQ || raw2Q)) {
			m_humdrum_text << ".";
			printInterleaved(infile, line, ktracks, reverselookup, ".");
		}
		return currentindex;
	}
	if (notes[0][currentindex].line != fileline) {
		// should never get here.
		printInterleaved(infile, line, ktracks, reverselookup, "?");
		return currentindex;
	}

	// found the index into notes which matches to the current fileline.
	if (currentindex + n >= (int)notes[0].size()) {
		// asking for chain longer than rest of available data.
		printInterleaved(infile, line, ktracks, reverselookup, ".");
		return currentindex;
	}

	int tracknext;
	int track;
	int j;
	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << infile.token(line, j);
				if (j < infile[line].getFieldCount() - 1) {
					m_humdrum_text << "\t";
				}
			}
			continue;
		}
		track = infile.token(line, j)->getTrack();
		if (j < infile[line].getFieldCount() - 1) {
			tracknext = infile.token(line, j+1)->getTrack();
		} else {
			tracknext = -23525;
		}
		if (track == tracknext) {
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << infile.token(line, j);
				if (j < infile[line].getFieldCount() - 1) {
					m_humdrum_text << "\t";
				}
			}
			continue;
		}

		// print the **kern spine, then check to see if there
		// is some **cint data to print
		if (!(rawQ || raw2Q)) {
			m_humdrum_text << infile.token(line, j);
		}
		if ((track != ktracks.back()) && (reverselookup[track] >= 0)) {
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << "\t";
			}
			int part1 = reverselookup[track];
			int part2 = part1+1;
			// m_humdrum_text << part1 << "," << part2;
			printLatticeModule(m_humdrum_text, notes, n, currentindex, part1, part2);
		}

		if (!(rawQ || raw2Q)) {
			if (j < infile[line].getFieldCount() - 1) {
				m_humdrum_text << "\t";
			}
		}
	}

	return currentindex;
}



//////////////////////////////
//
// Tool_cint::printInterleaved --
//

void Tool_cint::printInterleaved(HumdrumFile& infile, int line, vector<int>& ktracks, 
	 vector<int>& reverselookup, const string& interstring) {

	vector<int> done(ktracks.size(), 0);
	int track;
	int tracknext;

	int j;
	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << infile.token(line, j);
				if (j < infile[line].getFieldCount() - 1) {
					m_humdrum_text << "\t";
				}
			}
			continue;
		}
		track = infile.token(line, j)->getTrack();
		if (j < infile[line].getFieldCount() - 1) {
			tracknext = infile.token(line, j+1)->getTrack();
		} else {
			tracknext = -23525;
		}
		if (track == tracknext) {
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << infile.token(line, j);
				if (j < infile[line].getFieldCount() - 1) {
					m_humdrum_text << "\t";
				}
			}
			continue;
		}

		// print the **kern spine, then check to see if there
		// is some **cint data to print
		if (!(rawQ || raw2Q)) {
			m_humdrum_text << infile.token(line, j);

			if ((track != ktracks.back()) && (reverselookup[track] >= 0)) {
				m_humdrum_text << "\t" << interstring;
			}

			if (j < infile[line].getFieldCount() - 1) {
				m_humdrum_text << "\t";
			}
		}
	}
}



//////////////////////////////
//
// Tool_cint::printLattice --
//

void Tool_cint::printLattice(vector<vector<NoteNode> >& notes, HumdrumFile& infile, 
		vector<int>& ktracks, vector<int>& reverselookup, int n) {

	int i;
	int ii = 0;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!(rawQ || raw2Q)) {
			m_humdrum_text << infile[i];
		}
		if (infile.token(i, 0)->compare(0, 2, "**") == 0) {
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << "\t**cint\n";
			}
			continue;
		}
		if (infile[i].isData()) {
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << "\t";
			}
			if (rowsQ) {
				ii = printLatticeItemRows(notes, n, ii, i);
			} else {
				ii = printLatticeItem(notes, n, ii, i);
			}
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << "\n";
			}
			continue;
		}
		if (infile[i].isBarline()) {
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << "\t" << infile.token(i, 0) << "\n";
			}
			continue;
		}
		if (infile[i].isInterpretation()) {
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << "\t*\n";
			}
			continue;
		}
		if (infile[i].isLocalComment()) {
			if (!(rawQ || raw2Q)) {
				m_humdrum_text << "\t!\n";
			}
			continue;
		}
	}

}


//////////////////////////////
//
// Tool_cint::printLatticeModule -- print a counterpoint module or module chain given
//      the start notes and pair of parts to calculate the module
//      (chains) from.  Will not print anything if the chain length
//      is longer than the note array.
//

int Tool_cint::printLatticeModule(ostream& out, vector<vector<NoteNode> >& notes, int n, 
		int startline, int part1, int part2) {

	if (n + startline >= (int)notes[0].size()) {
		return 0;
	}

	if (parenQ) {
		out << "(";
	}

	int i;
	for (i=0; i<n; i++) {
		// print harmonic interval
		if (hparenQ) {
			out << "[";
		}
		printInterval(out, notes[part1][startline+i], 
			notes[part2][startline+i], INTERVAL_HARMONIC);
		if (hmarkerQ) {
			out << "h";
		}
		if (hparenQ) {
			out << "]";
		}
		printSpacer(out);

		// print melodic interal(s)
		if (mparenQ) {
			out << "{";
		}
		// bottom melodic interval:
		if (!toponlyQ) {
			printInterval(out, notes[part1][startline+i], 
							  notes[part1][startline+i+1], INTERVAL_MELODIC);
		}
 
		// print top melodic interval here if requested
		if (topQ || toponlyQ) {
			if (!toponlyQ) {
				printSpacer(out);
			}
			// top melodic interval:
			printInterval(out, notes[part2][startline+i], 
							  notes[part2][startline+i+1], INTERVAL_MELODIC);
			if (mmarkerQ) {
				out << "m";
			}
		}

		if (mparenQ) {
			out << "}";
		}
		printSpacer(out);
	}

	// print last harmonic interval
	if (hparenQ) {
	  out << "[";
	}
	printInterval(out, notes[part1][startline+n], 
			notes[part2][startline+n], INTERVAL_HARMONIC);
	if (hmarkerQ) {
		out << "h";
	}
	if (hparenQ) {
	  out << "]";
	}

	if (parenQ) {
		out << ")";
	}

	return 1;
}



//////////////////////////////
//
// Tool_cint::printLatticeItemRows -- Row form of the lattice.
//

int Tool_cint::printLatticeItemRows(vector<vector<NoteNode> >& notes, int n, 
		int currentindex, int fileline) {

	while ((currentindex < (int)notes[0].size()) 
			&& (fileline > notes[0][currentindex].line)) {
		currentindex++;
	}
	if (currentindex >= (int)notes[0].size()) {
		if (!(rawQ || raw2Q)) {
			m_humdrum_text << ".";
		}
		return currentindex;
	}
	if (notes[0][currentindex].line != fileline) {
		// should never get here.
		if (!(rawQ || raw2Q)) {
			m_humdrum_text << "?";
		}
		return currentindex;
	}

	// found the index into notes which matches to the current fileline.
	if (currentindex + n >= (int)notes[0].size()) {
		// asking for chain longer than rest of available data.
		if (!(rawQ || raw2Q)) {
			m_humdrum_text << ".";
		}
		return currentindex;
	}

	stringstream tempstream;
	int j;
	int counter = 0;

	for (j=0; j<(int)notes.size()-1; j++) {
		// iterate through each part, printing the module
		// for adjacent parts.
		counter += printLatticeModule(tempstream, notes, n, currentindex, j, j+1);
		if (j < (int)notes.size()-2) {
			printSpacer(tempstream);
		}
	}

	if (!(rawQ || raw2Q)) {
		if (counter == 0) {
			m_humdrum_text << ".";
		} else {
			m_humdrum_text << tempstream.str();
		}
	}

	return currentindex;
}



//////////////////////////////
//
// Tool_cint::printLatticeItem --
//

int Tool_cint::printLatticeItem(vector<vector<NoteNode> >& notes, int n, int currentindex, 
		int fileline) {
	while ((currentindex < (int)notes[0].size()) 
			&& (fileline > notes[0][currentindex].line)) {
		currentindex++;
	}
	if (currentindex >= (int)notes[0].size()) {
		if (!(rawQ || raw2Q)) {
			m_humdrum_text << ".";
		}
		return currentindex;
	}
	if (notes[0][currentindex].line != fileline) {
		// should never get here.
		if (!(rawQ || raw2Q)) {
			m_humdrum_text << "??";
		}
		return currentindex;
	}

	// found the index into notes which matches to the current fileline.
	if (currentindex + n >= (int)notes[0].size()) {
		// asking for chain longer than rest of available data.
		if (!(rawQ || raw2Q)) {
			m_humdrum_text << ".";
		}
		return currentindex;
	}

	int count;
	int melcount;
	int j;
	if (parenQ) {
		m_humdrum_text << "(";
	}
	for (count = 0; count < n; count++) {
		// print harmonic intervals
		if (hparenQ) {
			m_humdrum_text << "[";
		}
		for (j=0; j<(int)notes.size()-1; j++) {
			printInterval(m_humdrum_text, notes[j][currentindex+count], 
					notes[j+1][currentindex+count], INTERVAL_HARMONIC);
			if (j < (int)notes.size()-2) {
				printSpacer(m_humdrum_text);
			}
		}
		if (hparenQ) {
			m_humdrum_text << "]";
		}
		printSpacer(m_humdrum_text);

		// print melodic intervals
		if (mparenQ) {
			m_humdrum_text << "{";
		}
		melcount = (int)notes.size()-1;
		if (topQ) {
			melcount++;
		}
		for (j=0; j<melcount; j++) {
			printInterval(m_humdrum_text, notes[j][currentindex+count], 
					notes[j][currentindex+count+1], INTERVAL_MELODIC);
			if (j < melcount-1) {
				printSpacer(m_humdrum_text);
			}
		}
		if (mparenQ) {
			m_humdrum_text << "}";
		}
		printSpacer(m_humdrum_text);

	}
	// print last sequence of harmonic intervals
	if (hparenQ) {
		m_humdrum_text << "[";
	}
	for (j=0; j<(int)notes.size()-1; j++) {
		printInterval(m_humdrum_text, notes[j][currentindex+n], 
				notes[j+1][currentindex+n], INTERVAL_HARMONIC);
		if (j < (int)notes.size()-2) {
			printSpacer(m_humdrum_text);
		}
	}
	if (hparenQ) {
		m_humdrum_text << "]";
	}
	if (parenQ) {
		m_humdrum_text << ")";
	}

	if ((rawQ || raw2Q)) {
		m_humdrum_text << "\n";
	}

	return currentindex;
}



//////////////////////////////
//
// Tool_cint::printInterval --
//

int Tool_cint::printInterval(ostream& out, NoteNode& note1, NoteNode& note2,
		int type, int octaveadjust) {
	if ((note1.b40 == REST) || (note2.b40 == REST)) {
		out << RESTSTRING;
		return 0;
	}
	int cross = 0;
	int pitch1 = abs(note1.b40);
	int pitch2 = abs(note2.b40);
	int interval = pitch2 - pitch1;

	if ((type == INTERVAL_HARMONIC) && (interval < 0)) {
		cross = 1;
		if (uncrossQ) {
			interval = -interval;
		}
	} else {
		interval = interval + octaveadjust  * 40;
	}

	if ((type == INTERVAL_HARMONIC) && (octaveallQ)) {
		if (interval <= -40) {
			interval = interval + 4000;
		}
		if (interval > 40) {
			if (interval % 40 == 0) {
				interval = 40;
			} else {
				interval = interval % 40;
			}
		} else if (interval < 0) {
			interval = interval + 40;
		}
	}
	if (base12Q && !chromaticQ) {
		interval = Convert::base40ToMidiNoteNumber(interval + 40*4 + 2) - 12*5;
		if ((type == INTERVAL_HARMONIC) && (octaveallQ)) {
			if (interval <= -12) {
				interval = interval + 1200;
			}
			if (interval > 12) {
				if (interval % 12 == 0) {
					interval = 12;
				} else {
					interval = interval % 12;
				}
			} else if (interval < 0) {
				interval = interval + 12;
			}
		}
		interval = interval + octaveadjust  * 12;
	} else if (base7Q && !chromaticQ) {
		interval = Convert::base40ToDiatonic(interval + 40*4 + 2) - 7*4;
		if ((type == INTERVAL_HARMONIC) && (octaveallQ)) {
			if (interval <= -7) {
				interval = interval + 700;
			}
			if (interval > 7) {
				if (interval % 7 == 0) {
					interval = 7;
				} else {
					interval = interval % 7;
				}
			} else if (interval < 0) {
				interval = interval + 7;
			}
		}
		interval = interval + octaveadjust  * 7;
	}


	if (chromaticQ) {
		out << Convert::base40ToIntervalAbbr(interval);
	} else {
		int negative = 1;
		if (interval < 0) {
			negative = -1; 
			interval = -interval;
		}
		if (base7Q && !zeroQ) {
			out << negative * (interval+1);
		} else {
			out << negative * interval;
		}
	}

	if (sustainQ || ((type == INTERVAL_HARMONIC) && xoptionQ)) {
		// print sustain/attack information of intervals.
		if (note1.b40 < 0) {
			out << "s";
		} else {
			out << "x";
		}
		if (note2.b40 < 0) {
			out << "s";
		} else {
			out << "x";
		}
	}

	return cross;
}



//////////////////////////////
//
// Tool_cint::printSpacer -- space or comma...
//

void Tool_cint::printSpacer(ostream& out) {
	out << Spacer;
}



//////////////////////////////
//
// Tool_cint::printPitchGrid -- print the pitch grid from which all counterpoint
//      modules are calculated.
//

void Tool_cint::printPitchGrid(vector<vector<NoteNode> >& notes, HumdrumFile& infile) {
	int i = 0;
	int j = 0;
	int pitch;
	int abspitch;
	int newpitch;
	int partcount;
	int line;
	double beat;

	if (base40Q) {
		partcount = (int)notes.size();

		if (rhythmQ) {
			m_humdrum_text << "**absq\t";
			m_humdrum_text << "**bar\t";
			m_humdrum_text << "**beat\t";
		}
		for (i=0; i<partcount; i++) {
			m_humdrum_text << "**b40";
			if (i < partcount - 1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << endl;
		for (i=0; i<(int)notes[0].size(); i++) {
			if (rhythmQ) {
				line = notes[0][i].line;
				beat = infile[line].getDurationFromBarline().getFloat() * notes[0][i].beatsize + 1; 
				m_humdrum_text << infile[line].getDurationFromStart().getFloat() << "\t";
				m_humdrum_text << notes[0][i].measure << "\t";
				m_humdrum_text << beat << "\t";
			}
			for (j=0; j<(int)notes.size(); j++) {
				if (notes[j][i].notemarker) {
					m_humdrum_text << (char)notes[j][i].notemarker;
				}
				m_humdrum_text << notes[j][i].b40;
				if (j < (int)notes.size()-1) {
					m_humdrum_text << "\t";
				}
			}
			m_humdrum_text << endl;
		}
		if (rhythmQ) {
			m_humdrum_text << "*-\t";
			m_humdrum_text << "*-\t";
			m_humdrum_text << "*-\t";
		}
		for (i=0; i<partcount; i++) {
			m_humdrum_text << "*-";
			if (i < partcount - 1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << endl;
	} else if (base7Q) {
		partcount = (int)notes.size();

		if (rhythmQ) {
			m_humdrum_text << "**absq\t";
			m_humdrum_text << "**bar\t";
			m_humdrum_text << "**beat\t";
		}
		for (i=0; i<partcount; i++) {
			m_humdrum_text << "**b7";
			if (i < partcount - 1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << endl;

		for (i=0; i<(int)notes[0].size(); i++) {
			if (rhythmQ) {
				line = notes[0][i].line;
				beat = infile[line].getDurationFromBarline().getFloat() * notes[0][i].beatsize + 1; 
				m_humdrum_text << infile[line].getDurationFromStart().getFloat() << "\t";
				m_humdrum_text << notes[0][i].measure << "\t";
				m_humdrum_text << beat << "\t";
			}
			for (j=0; j<(int)notes.size(); j++) {
				if (notes[j][i].notemarker) {
					m_humdrum_text << (char)notes[j][i].notemarker;
				}
				pitch = notes[j][i].b40;
				abspitch = abs(pitch);
				if (pitch == 0) {
					// print rest
					m_humdrum_text << 0;
				} else {
					newpitch = Convert::base40ToDiatonic(abspitch);
					if (pitch < 0) {
						newpitch = -newpitch;
					}
					m_humdrum_text << newpitch;
				}
				if (j < (int)notes.size()-1) {
					m_humdrum_text << "\t";
				}
			}
			m_humdrum_text << endl;
		}
		if (rhythmQ) {
			m_humdrum_text << "*-\t";
			m_humdrum_text << "*-\t";
			m_humdrum_text << "*-\t";
		}
		for (i=0; i<partcount; i++) {
			m_humdrum_text << "*-";
			if (i < partcount - 1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << endl;
	} else if (base12Q) {
		partcount = (int)notes.size();

		if (rhythmQ) {
			m_humdrum_text << "**absq\t";
			m_humdrum_text << "**bar\t";
			m_humdrum_text << "**beat\t";
		}
		for (i=0; i<partcount; i++) {
			m_humdrum_text << "**b12";
			if (i < partcount - 1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << endl;

		for (i=0; i<(int)notes[0].size(); i++) {
			if (rhythmQ) {
				line = notes[0][i].line;
				beat = infile[line].getDurationFromBarline().getFloat() * notes[0][i].beatsize + 1; 
				if (notes[j][i].notemarker) {
					m_humdrum_text << (char)notes[j][i].notemarker;
				}
				m_humdrum_text << infile[line].getDurationFromStart() << "\t";
				m_humdrum_text << notes[0][i].measure << "\t";
				m_humdrum_text << beat << "\t";
			}
			for (j=0; j<(int)notes.size(); j++) {
				if (notes[j][i].notemarker) {
					m_humdrum_text << (char)notes[j][i].notemarker;
				}
				pitch = notes[j][i].b40;
				if (pitch == 0) {
					// print rest
					m_humdrum_text << 0;
				} else {
					abspitch = abs(pitch);
					newpitch = Convert::base40ToMidiNoteNumber(abspitch);
					if (pitch < 0) {
						newpitch = -newpitch;
					}
					m_humdrum_text << newpitch;
				}
				if (j < (int)notes.size()-1) {
					m_humdrum_text << "\t";
				}
			}
			m_humdrum_text << endl;
		}
		if (rhythmQ) {
			m_humdrum_text << "*-\t";
			m_humdrum_text << "*-\t";
			m_humdrum_text << "*-\t";
		}
		for (i=0; i<partcount; i++) {
			m_humdrum_text << "*-";
			if (i < partcount - 1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << endl;
	} else {
		// print as Humdrum **kern data
		partcount = (int)notes.size();

		if (rhythmQ) {
			m_humdrum_text << "**absq\t";
			m_humdrum_text << "**bar\t";
			m_humdrum_text << "**beat\t";
		}
		for (i=0; i<partcount; i++) {
			m_humdrum_text << "**kern";
			if (i < partcount - 1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << endl;

		for (i=0; i<(int)notes[0].size(); i++) {
			if (rhythmQ) {
				line = notes[0][i].line;
				beat = infile[line].getDurationFromBarline().getFloat() * notes[0][i].beatsize + 1; 
				if (notes[j][i].notemarker) {
					m_humdrum_text << (char)notes[j][i].notemarker;
				}
				m_humdrum_text << infile[line].getDurationFromStart() << "\t";
				m_humdrum_text << notes[0][i].measure << "\t";
				m_humdrum_text << beat << "\t";
			}
			for (j=0; j<(int)notes.size(); j++) {
				if (notes[j][i].notemarker) {
					m_humdrum_text << (char)notes[j][i].notemarker;
				}
				pitch = notes[j][i].b40;
				abspitch = abs(pitch);
				if (pitch == 0) {
					m_humdrum_text << "r";
				} else {
					if ((pitch > 0) && (i<(int)notes[j].size()-1) && 
						 (notes[j][i+1].b40 == -abspitch)) {
						// start of a note which continues into next 
						// sonority.
						m_humdrum_text << "[";
					}
					m_humdrum_text << Convert::base40ToKern(abspitch);
					// print tie continue/termination as necessary.
					if (pitch < 0) {
						if ((i < (int)notes[j].size() - 1) && 
							 (notes[j][i+1].b40 == notes[j][i].b40)) {
						  // note sustains further
						  m_humdrum_text << "_";
						} else {
						  // note does not sustain any further.
						  m_humdrum_text << "]";
						}
					}
				}
				if (j < (int)notes.size()-1) {
					m_humdrum_text << "\t";
				}
			}
			m_humdrum_text << endl;
		}

		if (rhythmQ) {
			m_humdrum_text << "*-\t";
			m_humdrum_text << "*-\t";
			m_humdrum_text << "*-\t";
		}
		for (i=0; i<partcount; i++) {
			m_humdrum_text << "*-";
			if (i < partcount - 1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << endl;
	}
}



//////////////////////////////
//
// Tool_cint::extractNoteArray --
//

void Tool_cint::extractNoteArray(vector<vector<NoteNode> >& notes, HumdrumFile& infile,
		vector<int>& ktracks, vector<int>& reverselookup) {

	HumRegex hre;

	Ids.resize(infile.getTrackCount()+1);
	int i, j, ii, jj;
	for (i=0; i<(int)Ids.size(); i++) {
		Ids[i] = EMPTY_ID;
	}

	vector<NoteNode> current(ktracks.size());
	vector<double> beatsizes(infile.getTrackCount()+1, 1);

	int sign;
	int track = 0;
	int index;

	int snum = 0;
	int measurenumber = 0;
	int tempmeasurenum = 0;
	double beatsize = 1.0;
	int topnum, botnum;
	
	for (i=0; i<infile.getLineCount(); i++) {
		if (debugQ) {
			m_humdrum_text << "PROCESSING LINE: " << i << "\t" << infile[i] << endl;
		}
		if (infile[i].isBarline()) {
			tempmeasurenum = infile.getMeasureNumber(i);
			if (tempmeasurenum >= 0) {
				measurenumber = tempmeasurenum;
			}
		}
		for (j=0; j<(int)current.size(); j++) {
			current[j].clear();
			current[j].measure = measurenumber;
			current[j].line = i;
		}

		if (infile[i].isBarline() && (infile.token(i, 0)->find("||") != string::npos)) {
			// double barline (terminal for Josquin project), so add a row
			// of rests to prevent cint melodic interval identification between
			// adjacent notes in different sections.
			for (j=0; j<(int)notes.size(); j++) {
				notes[j].push_back(current[j]);
			}
		} else if (infile[i].isInterpretation()) {
			// search for time signatures from which to extract beat information.
			for (j=0; j<infile[i].getFieldCount(); j++) {
				track = infile.token(i, j)->getTrack();
				if (hre.search(*infile.token(i, j), "^\\*M(\\d+)/(\\d+)")) {
					// deal with 3%2 in denominator later...
					topnum = hre.getMatchInt(1);
					botnum = hre.getMatchInt(2);
					beatsize = botnum;
					if (((topnum % 3) == 0) && (topnum > 3) && (botnum > 1)) {
						// compound meter
						// fix later
						beatsize = botnum / 3;
					}
					beatsizes[track] = beatsize / 4.0;
				} else if (*infile.token(i, j) == "*met(C|)") {
					// MenCutC, use 2 as the "beat"
					beatsizes[track] = 2.0 / 4.0;
				}
			}
		} else if (idQ && infile[i].isLocalComment()) {
			for (j=0; j<infile[i].getFieldCount(); j++) {
				if (hre.search(*infile.token(i, j), "^!ID:\\s*([^\\s]*)")) {
					int track = infile.token(i, j)->getTrack();
					Ids[track] = hre.getMatch(1);
				}
			}
		}
		 
		if (!infile[i].isData()) {
			continue;
		}

		for (j=0; j<infile[i].getFieldCount(); j++) {
			sign = 1;
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			track = infile.token(i, j)->getTrack();
			index = reverselookup[track];
			if (index < 0) {
				continue;
			}
			if (idQ) {
				current[index].getId() = Ids[track];
				Ids[track] = "";  // don't assign to next item;
			}
			current[index].line  = i;
			current[index].spine = j;
			current[index].beatsize = beatsizes[track];
			if (infile.token(i, j)->isNull()) {
				sign = -1;
				HTp nullx = infile.token(i, j)->resolveNull();
				if (nullx == NULL) {
					ii = jj = -1;
				} else {
					ii = nullx->getLineIndex();
					jj = nullx->getFieldIndex();
				}
			} else {
				ii = i;
				jj = j;
			}
			if (infile.token(ii, jj)->find(NoteMarker) != string::npos) {
				current[index].notemarker = NoteMarker;
			}
			if (infile.token(ii, jj)->find('r') != string::npos) {
				current[index].b40 = 0;
				current[index].serial = ++snum;
				continue;
			}
			if (*infile.token(ii, jj) == ".") {
				current[index].b40 = 0;
				current[index].serial = snum;
			}
			current[index].b40 = Convert::kernToBase40(*infile.token(ii, jj));
			if (infile.token(ii, jj)->find('_') != string::npos) {
				sign = -1;
				current[index].serial = snum;
			}
			if (infile.token(ii, jj)->find(']') != string::npos) {
				sign = -1;
				current[index].serial = snum;
			}
			current[index].b40 *= sign;
			if (sign > 0) {
				current[index].serial = ++snum;
				if (durationQ) { 
					current[index].duration = infile.token(ii, jj)->getTiedDuration();
				}
			}
		}
		if (onlyRests(current) && onlyRests(notes.back())) {
			// don't store more than one row of rests in the data array.
			continue;
		}
		if (allSustained(current)) {
			// don't store sonorities which are purely sutained
			// (may need to be updated with a --sustain option implementation)
			continue;
		}
		for (j=0; j<(int)notes.size(); j++) {
			notes[j].push_back(current[j]);
		}
	}

	// attach ID tag to all sustain sections of notes
	if (idQ) {
		for (j=0; j<(int)notes.size(); j++) {
			for (i=1; i<(int)notes[j].size(); i++) {
				if (notes[j][i].isAttack()) {
					continue;
				}
				if ((int)notes[j][i].getId().size() > 0) {
					// allow for Ids on sustained notes which probably means
					// that there is a written tied note in the music.
					continue;
				}
				if (notes[j][i].getB40() == notes[j][i-1].getB40()) {
					notes[j][i].getId() = notes[j][i-1].getId();
				}
			}
		}
	}

}



//////////////////////////////
//
// Tool_cint::onlyRests -- returns true if all NoteNodes are for rests
//

int Tool_cint::onlyRests(vector<NoteNode>& data) {
	int i;
	for (i=0; i<(int)data.size(); i++) {
		if (!data[i].isRest()) {
			return 0;
		}
	}
	return 1;
}



//////////////////////////////
//
// Tool_cint::hasAttack -- returns true if all NoteNodes are for rests
//

int Tool_cint::hasAttack(vector<NoteNode>& data) {
	int i;
	for (i=0; i<(int)data.size(); i++) {
		if (data[i].isAttack()) {
			return 1;
		}
	}
	return 0;
}



//////////////////////////////
//
// Tool_cint::allSustained -- returns true if all NoteNodes are sustains
//    or rests (but not all rests).
//

int Tool_cint::allSustained(vector<NoteNode>& data) {
	int i;
	int hasnote = 0;
	for (i=0; i<(int)data.size(); i++) {
		if (data[i].b40 != 0) {
			hasnote = 1;
		}
		if (data[i].isAttack()) {
			return 0;
		}
	}
	if (hasnote == 0) {
		return 0;
	} 
	return 1;
}



//////////////////////////////
//
// Tool_cint::getAbbreviations --
//

void Tool_cint::getAbbreviations(vector<string>& abbreviations, 
		vector<string>& names) {
	abbreviations.resize(names.size());
	for (int i=0; i<(int)names.size(); i++) {
		getAbbreviation(abbreviations[i], names[i]);     
	}
}



//////////////////////////////
//
// Tool_cint::getAbbreviation --
//

void Tool_cint::getAbbreviation(string& abbr, string& name) {
	HumRegex hre;
	hre.replaceDestructive(abbr, "(?<=[a-zA-Z])[a-zA-Z]*", "");
	hre.tr(abbr, "123456789", "abcdefghi");
}



//////////////////////////////
//
// Tool_cint::getKernTracks -- return a list of track number for **kern spines.
//

void Tool_cint::getKernTracks(vector<int>& ktracks, HumdrumFile& infile) {
	int i, j;
	ktracks.reserve(infile.getTrackCount()+1);
	ktracks.resize(0);
	int track;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isInterpretation()) {
			continue;
		}
		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (infile.token(i, j)->isKern()) {
				track = infile.token(i, j)->getTrack();
				ktracks.push_back(track);
			}
		}
		break;
	}
}



//////////////////////////////
//
// Tool_cint::getNames -- get the names of each column if they have one.
//

void Tool_cint::getNames(vector<string>& names, vector<int>& reverselookup, 
		HumdrumFile& infile) {

	names.resize((int)reverselookup.size()-1);
	char buffer[1024] = {0};
	int value;
	HumRegex pre;
	int i;
	int j;
	int track;
 
	for (i=0; i<(int)names.size(); i++) {
		value = (int)reverselookup.size() - i;
		sprintf(buffer, "%d", value);
		names[i] = buffer;
	}

	for (i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isData()) {
			// stop looking for instrument name after the first data line
			break;
		}
		if (!infile[i].isInterpretation()) {
			continue;
		}
		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (reverselookup[infile.token(i, j)->getTrack()] < 0) {
				continue;
			}
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			if (pre.search(*infile.token(i, j), "^\\*I\"(.*)")) {
				track = infile.token(i, j)->getTrack();
				names[reverselookup[track]] = pre.getMatch(1);
			}
		}
	}

	if (debugQ) {
		for (i=0; i<(int)names.size(); i++) {
			m_humdrum_text << i << ":\t" <<  names[i] << endl;
		}
	}

}



//////////////////////////////
//
// Tool_cint::initialize -- validate and process command-line options.
//

void Tool_cint::initialize(void) {

	// handle basic options:
	if (getBoolean("author")) {
		m_humdrum_text << "Written by Craig Stuart Sapp, "
			  << "craig@ccrma.stanford.edu, September 2013" << endl;
		exit(0);
	} else if (getBoolean("version")) {
		m_humdrum_text << getCommand() << ", version: 31 May 2017" << endl;
		m_humdrum_text << "compiled: " << __DATE__ << endl;
		exit(0);
	} else if (getBoolean("help")) {
		usage(getCommand());
		exit(0);
	} else if (getBoolean("example")) {
		example();
		exit(0);
	}
	
	koptionQ = getBoolean("koption");

	if (getBoolean("comma")) {
		Spacer = ",";
	} else {
		Spacer = " ";
	}

	// dispay as base-7 by default
	base7Q = 1;

	base40Q    = getBoolean("base-40");
	base12Q    = getBoolean("base-12");
	chromaticQ = getBoolean("chromatic");
	zeroQ      = getBoolean("zero");

	if (base40Q) {
		base12Q = 0;
		base7Q = 0;
		zeroQ = 0;
	}

	if (base12Q) {
		base40Q = 0;
		base7Q = 0;
		zeroQ = 0;
	}

	pitchesQ     = getBoolean("pitches");
	debugQ       = getBoolean("debug");
	rhythmQ      = getBoolean("rhythm");
	durationQ    = getBoolean("duration");
	latticeQ     = getBoolean("lattice");
	sustainQ     = getBoolean("sustain");
	topQ         = getBoolean("top");
	toponlyQ     = getBoolean("top-only");
	hparenQ      = getBoolean("harmonic-parentheses");
	mparenQ      = getBoolean("melodic-parentheses");
	parenQ       = getBoolean("parentheses");
	rowsQ        = getBoolean("rows");
	hmarkerQ     = getBoolean("harmonic-marker");
	interleavedQ = getBoolean("interleaved-lattice");
	mmarkerQ     = getBoolean("melodic-marker");
	attackQ      = getBoolean("attacks");
	rawQ         = getBoolean("raw");
	raw2Q        = getBoolean("raw2");
	xoptionQ     = getBoolean("x");
	octaveallQ   = getBoolean("octave-all");
	octaveQ      = getBoolean("octave");
	noharmonicQ  = getBoolean("no-harmonic");
	nomelodicQ   = getBoolean("no-melodic");
	norestsQ     = getBoolean("no-rests");
	nounisonsQ   = getBoolean("no-melodic-unisons");
	Chaincount   = getInteger("n");
	searchQ      = getBoolean("search");
	markQ        = getBoolean("mark");
	idQ          = getBoolean("id");
	countQ       = getBoolean("count");
	filenameQ    = getBoolean("filename");
	suspensionsQ = getBoolean("suspensions");
	uncrossQ     = getBoolean("uncross");
	locationQ    = getBoolean("location");
	retroQ       = getBoolean("retrospective");
	NoteMarker   = 0;
	if (getBoolean("note-marker")) {
		NoteMarker = getString("note-marker").c_str()[0];
	}
	if (Chaincount < 0) {
		Chaincount = 0;
	}
	
	if (searchQ) {
		// Automatically assume marking of --search is used
		// (may change in the future).
		markQ = 1;
	} 
	if (countQ) {
		searchQ = 1;
		markQ   = 0;
	}

	if (raw2Q) {
		norestsQ = 1;
	}

	if (searchQ) {
		SearchString = getString("search");
	}

}



//////////////////////////////
//
// Tool_cint::example -- example usage of the quality program
//

void Tool_cint::example(void) {
	m_humdrum_text <<
	"                                                                         \n"
	<< endl;
}



//////////////////////////////
//
// Tool_cint::usage -- gives the usage statement for the meter program
//

void Tool_cint::usage(const string& command) {
	m_humdrum_text <<
	"                                                                         \n"
	<< endl;
}




/////////////////////////////////
//
// Tool_dissonant::Tool_dissonant -- Set the recognized options for the tool.
//

Tool_dissonant::Tool_dissonant(void) {
	define("r|raw=b",             "print raw grid");
	define("p|percent=b",         "print counts as percentages");
	define("s|suppress=b",        "suppress dissonant notes");
	define("d|diatonic=b",        "print diatonic grid");
	define("D|no-dissonant=b",    "don't do dissonance anaysis");
	define("m|midi-pitch=b",      "print midi-pitch grid");
	define("b|base-40=b",         "print base-40 grid");
	define("l|metric-levels=b",   "use metric levels in analysis");
	define("k|kern=b",            "print kern pitch grid");
	define("debug=b",             "print grid cell information");
	define("u|undirected=b",      "use undirected dissonance labels");
	define("c|count=b",           "count dissonances by category");
	define("e|exinterp=s:**cdata","specify exinterp for **cdata spine");
	define("color|colorize=b",    "color dissonant notes by beat level");
	define("color2|colorize2=b",  "color dissonant notes by dissonant interval");
}



/////////////////////////////////
//
// Tool_dissonant::run -- Do the main work of the tool.
//

bool Tool_dissonant::run(const string& indata, ostream& out) {

	if (getBoolean("undirected")) {
		fillLabels2();
	} else {
		fillLabels();
	}

	HumdrumFile infile(indata);
	bool status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_dissonant::run(HumdrumFile& infile, ostream& out) {

	if (getBoolean("undirected")) {
		fillLabels2();
	} else {
		fillLabels();
	}

	int status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_dissonant::run(HumdrumFile& infile) {

	if (getBoolean("undirected")) {
		fillLabels2();
	} else {
		fillLabels();
	}

	NoteGrid grid(infile);

	if (getBoolean("debug")) {
		grid.printGridInfo(cerr);
		// return 1;
	} else if (getBoolean("raw")) {
		grid.printRawGrid(m_free_text);
		return 1;
	} else if (getBoolean("diatonic")) {
		grid.printDiatonicGrid(m_free_text);
		return 1;
	} else if (getBoolean("midi-pitch")) {
		grid.printMidiGrid(m_free_text);
		return 1;
	} else if (getBoolean("base-40")) {
		grid.printBase40Grid(m_free_text);
		return 1;
	} else if (getBoolean("kern")) {
		grid.printKernGrid(m_free_text);
		return 1;
	}

	diss2Q = false;
	diss7Q = false;
	diss4Q = false;

	dissL0Q = false;
	dissL1Q = false;
	dissL2Q = false;

	suppressQ = getBoolean("suppress");

	vector<vector<string> > results;
	vector<vector<string> > results2;
	vector<vector<NoteCell*> > attacks;
	vector<vector<NoteCell*> > attacks2;

	attacks.resize(grid.getVoiceCount());
	results.resize(grid.getVoiceCount());
	for (int i=0; i<(int)results.size(); i++) {
		results[i].resize(infile.getLineCount());
	}
	doAnalysis(results, grid, attacks, getBoolean("debug"));

	if (suppressQ) {
		suppressDissonances(infile, grid, attacks, results);

		NoteGrid grid2(infile);
		results2.resize(grid2.getVoiceCount());
		for (int i=0; i<(int)results2.size(); i++) {
			results2[i].clear();
			results2[i].resize(infile.getLineCount());
		}
		vector<vector<NoteCell*> > attacks2;
		doAnalysis(results2, grid2, attacks2, getBoolean("debug"));

	}

	if (suppressQ) {
		if (getBoolean("count")) {
			printCountAnalysis(results2);
			return false;
		} else {
			string exinterp = getString("exinterp");
			vector<HTp> kernspines = infile.getKernSpineStartList();
			infile.appendDataSpine(results2.back(), "", exinterp);
			for (int i = (int)results2.size()-1; i>0; i--) {
				int track = kernspines[i]->getTrack();
				infile.insertDataSpineBefore(track, results2[i-1], "", exinterp);
			}
			printColorLegend(infile);
			infile.createLinesFromTokens();
			return true;
		}
	} else {
		if (getBoolean("count")) {
			printCountAnalysis(results);
			return false;
		} else {
			string exinterp = getString("exinterp");
			vector<HTp> kernspines = infile.getKernSpineStartList();
			infile.appendDataSpine(results.back(), "", exinterp);
			for (int i = (int)results.size()-1; i>0; i--) {
				int track = kernspines[i]->getTrack();
				infile.insertDataSpineBefore(track, results[i-1], "", exinterp);
			}
			printColorLegend(infile);
			infile.createLinesFromTokens();
			return true;
		}
	}

}



/////////////////////////////
//
// Tool_dissonant::suppressDissonances -- remove dissonances.
//

void Tool_dissonant::suppressDissonances(HumdrumFile& infile, NoteGrid& grid,
		vector<vector<NoteCell*> >& attacks, vector<vector<string> >& results) {

	for (int i=0; i<(int)attacks.size(); i++) {
		suppressDissonancesInVoice(infile, grid, i, attacks[i], results[i]);
	}
}



//////////////////////////////
//
// Tool_dissonant::suppressDissonancesInVoice --
//

void Tool_dissonant::suppressDissonancesInVoice(HumdrumFile& infile, 
		NoteGrid& grid, int vindex, vector<NoteCell*>& attacks,
		vector<string>& results) {

	for (int i=0; i<(int)attacks.size(); i++) {
		int lineindex = attacks[i]->getLineIndex();
		if (results[lineindex] == "") {
			continue;
		} else if (results[lineindex] == ".") {
			continue;
		} else if (results[lineindex] == m_labels[PASSING_UP]) {
			mergeWithPreviousNote(infile, attacks, i);
		} else if (results[lineindex] == m_labels[PASSING_DOWN]) {
			mergeWithPreviousNote(infile, attacks, i);
		} else if (results[lineindex] == m_labels[NEIGHBOR_UP]) {
			mergeWithPreviousNote(infile, attacks, i);
		} else if (results[lineindex] == m_labels[NEIGHBOR_DOWN]) {
			mergeWithPreviousNote(infile, attacks, i);
		}
	}
}



//////////////////////////////
//
// Tool_dissonant::mergeWithPreviousNote --  will not
//  handle chords correctly.
//

void Tool_dissonant::mergeWithPreviousNote(HumdrumFile& infile,
		vector<NoteCell*>& attacks, int index) {

	if (index < 1) {
		return;
	}

	HTp note1 = attacks[index-1]->getToken();
	HTp note2 = attacks[index]->getToken();

	int line1 = note1->getLineIndex();
	int line2 = note2->getLineIndex();

	// bool barlineQ = false;
	for (int i=line1+1; i<line2; i++) {
		if (infile[i].isBarline()) {
			// barlineQ = true;
			break;
		}
	}

	HumNum dur1 = note1->getDuration();
	HumNum dur2 = note2->getDuration();

	HumNum sumdur = dur1 + dur2;

	/*
	cerr << "Notes" << note1;
	cerr << "\tto\t" << note2;
	cerr << "\tline\t" << note1->getLineIndex();
	cerr << "\tnewdur=" << sumdur;
	cerr << endl;
	*/

	bool tied1 = note1->find("[") != string::npos ? true : false;
	bool tied2 = note2->find("[") != string::npos ? true : false;

	if (tied1 || tied2) {
		// don't deal with tied notes for now
		return;
	}


	// for now, replace the pitch of the second note with
	// that of the first note.  Later tied them together or
	// merge into a single note depending on the notational
	// context.

	changePitch(note2, note1);

}



//////////////////////////////
//
// Tool_dissonant::changePitch -- will not handle chords correctly.
//

void Tool_dissonant::changePitch(HTp note2, HTp note1) {
	int b40 = Convert::kernToBase40(note1);
	string pitch = Convert::base40ToKern(b40);
	HumRegex hre;
	string n2 = *note2;
	hre.replaceDestructive(n2, pitch, "[A-Ga-gr#-]+");
	note2->setText(n2);
}




//////////////////////////////
//
// Tool_dissonant::printColorLegend --
//

void Tool_dissonant::printColorLegend(HumdrumFile& infile) {
	if (getBoolean("colorize")) {
		if (dissL0Q) {
			infile.appendLine("!!!RDF**kern: N = strong dissonant marked note, color=\"#bb3300\"");
		}
		if (dissL1Q) {
			infile.appendLine("!!!RDF**kern: @ = weak 1 dissonant marked note, color=\"#33bb00\"");
		}
		if (dissL2Q) {
			infile.appendLine("!!!RDF**kern: + = weak 2 dissonant marked note, color=\"#0099ff\"");
		}
	} else if (getBoolean("colorize2")) {
		if (diss2Q) {
			infile.appendLine("!!!RDF**kern: @ = dissonant 2nd, marked note, color=\"#33bb00\"");
		}
		if (diss7Q) {
			infile.appendLine("!!!RDF**kern: + = dissonant 7th, marked note, color=\"#0099ff\"");
		}
		if (diss4Q) {
			infile.appendLine("!!!RDF**kern: N = dissonant 4th marked note, color=\"#bb3300\"");
		}
	}
}



//////////////////////////////
//
// Tool_dissonant::doAnalysis -- do a basic melodic analysis of all parts.
//

void Tool_dissonant::doAnalysis(vector<vector<string> >& results,
		NoteGrid& grid, vector<vector<NoteCell*> >& attacks, bool debug) {
	attacks.resize(grid.getVoiceCount());

	for (int i=0; i<grid.getVoiceCount(); i++) {
		attacks[i].clear();
		doAnalysisForVoice(results, grid, attacks[i], i, debug);
	}

	for (int i=0; i<grid.getVoiceCount(); i++) {
		findFakeSuspensions(results, grid, attacks[i], i);
	}

	for (int i=0; i<grid.getVoiceCount(); i++) {
		findLs(results, grid, attacks[i], i);
	}
	
	for (int i=0; i<grid.getVoiceCount(); i++) {
		findYs(results, grid, attacks[i], i);
	}

}



//////////////////////////////
//
// Tool_dissonant::doAnalysisForVoice -- do analysis for a single voice by
//     subtracting NoteCells to calculate the diatonic intervals.
//

void Tool_dissonant::doAnalysisForVoice(vector<vector<string> >& results,
		NoteGrid& grid, vector<NoteCell*>& attacks, int vindex, bool debug) {
	attacks.clear();
	grid.getNoteAndRestAttacks(attacks, vindex);

	if (debug) {
		cerr << "=======================================================";
		cerr << endl;
		cerr << "Note attacks for voice number "
		     << grid.getVoiceCount()-vindex << ":" << endl;
		for (int i=0; i<(int)attacks.size(); i++) {
			attacks[i]->printNoteInfo(cerr);
		}
	}
	bool nodissonanceQ = getBoolean("no-dissonant");
	bool colorizeQ = getBoolean("colorize");
	bool colorize2Q = getBoolean("colorize2");

	HumNum durpp = -1; // duration of previous previous note
	HumNum durp;       // duration of previous melodic note
	HumNum dur;        // duration of current note
	HumNum durn;       // duration of next melodic note
	HumNum odur = -1; // duration of current note in other voice which may have started earlier
	HumNum odurn = -1; // duration of next note in other voice
	double intp;       // diatonic interval from previous melodic note
	double intpp = -99;// diatonic interval to previous melodic note
	double intn;       // diatonic interval to next melodic note
	double levp;       // metric level of the previous melodic note
	double lev;        // metric level of the current note
	double levn;       // metric level of the next melodic note
	int lineindex;     // line in original Humdrum file content that contains note
	int lineindexpp = -1;// line in original Humdrum file content that contains the previous previous note
	// int lineindexn; // next line in original Humdrum file content that contains note
	int attackindexn;  // slice in NoteGrid content that contains next note attack
	int sliceindex;    // current timepoint in NoteGrid.
	int oattackindexn = -1; // next note attack index of the other voice involved in the diss.
	vector<double> harmint(grid.getVoiceCount());  // harmonic intervals
	bool dissonant;    // true if  note is dissonant with other sounding notes.
	char marking = '\0';
	int ovoiceindex = -1;
	string unexp_label; // default dissonance label if none of the diss types apply
	int refMeterNum;    // the numerator of the reference voice's notated time signature
	HumNum refMeterDen; // the denominator of the reference voice's notated time signature
	int othMeterNum;    // the numerator of the other voice's notated time signature
	HumNum othMeterDen; // the denominator of the other voice's notated time signature
	bool ternAgent = false;  // true if the ref voice would be a valid agent of a ternary susp. But if true, the diss is not necessarily a susp.

	for (int i=1; i<(int)attacks.size() - 1; i++) {
		sliceindex = attacks[i]->getSliceIndex();
		lineindex = attacks[i]->getLineIndex();
		// lineindexn = attacks[i+1]->getLineIndex();
		attackindexn = attacks[i]->getNextAttackIndex();

		marking = '\0';

		// calculate harmonic intervals:
		int lowestnote = 1000;
		double tpitch;
		// int lowestnotei = -1;
		for (int j=0; j<(int)harmint.size(); j++) {
			tpitch = grid.cell(j, sliceindex)->getAbsDiatonicPitch();
			if (!Convert::isNaN(tpitch)) {
				if (tpitch <= lowestnote) {
					lowestnote = tpitch;
					// lowestnotei = j;
				}
			}
			if (j == vindex) {
				harmint[j] = 0;
			}

			harmint[j] = *grid.cell(j, sliceindex) -
					*grid.cell(vindex, sliceindex);

/*
			if (j < vindex) {
				harmint[j] = *grid.cell(vindex, sliceindex) -
						*grid.cell(j, sliceindex);
			} else {
				harmint[j] = *grid.cell(j, sliceindex) -
						*grid.cell(vindex, sliceindex);
			}
*/
		}

		// check if current note is dissonant to another sounding note:
		dissonant = false;

		int nextj = 0;
		int j = 0;

RECONSIDER:

		int value = 0;
		for (j=nextj; j<(int)harmint.size(); j++) {
			if (j == vindex) {
				// don't compare to self
				continue;
			}
			if (Convert::isNaN(harmint[j])) {
				// rest, so ignore
				continue;
			}

			value = (int)harmint[j] % 7; // remove octaves from interval, can return negative ints

			int vpitch = (int)grid.cell(vindex, sliceindex)->getAbsDiatonicPitch();
			int otherpitch = (int)grid.cell(j, sliceindex)->getAbsDiatonicPitch();

			if ((value == 1) || (value == -1)) {
				// forms a second with another sounding note
				dissonant = true;
				diss2Q = true;
				marking = '@';
				unexp_label = m_labels[UNLABELED_Z2];
				ovoiceindex = j;
				oattackindexn = getNextPitchAttackIndex(grid, ovoiceindex, sliceindex);
				break;
			} else if ((value == 6) || (value == -6)) {
				// forms a seventh with another sounding note
				dissonant = true;
				diss7Q = true;
				marking = '+';
				unexp_label = m_labels[UNLABELED_Z7];
				ovoiceindex = j;
				oattackindexn = getNextPitchAttackIndex(grid, ovoiceindex, sliceindex);
				break;
			} else if (
					((value == 3) && not ((((vpitch-lowestnote) % 7) == 2) ||
					                     (((vpitch-lowestnote) % 7) == 4))) ||
					((value == -3) && not ((((otherpitch-lowestnote) % 7) == 2) ||
					                      (((otherpitch-lowestnote) % 7) == 4)))
					) {
				// If the harmonic interval between two notes is a fourth and 
				// the lower pitch in the interval is not a a third or a fifth
				// above the lowest note.
				dissonant = true;
				diss4Q = true;
				marking = 'N';
				unexp_label = m_labels[UNLABELED_Z4];
				// ovoiceindex = lowestnotei;
				ovoiceindex = j;
				// oattackindexn = grid.cell(ovoiceindex, sliceindex)->getNextAttackIndex();
				oattackindexn = getNextPitchAttackIndex(grid, ovoiceindex, sliceindex);
				break;
			}
		}
		nextj = j+1;


/*
		double vpitch = grid.cell(vindex, sliceindex)->getAbsDiatonicPitch();
		if (vpitch - lowestnote > 0) {
			if (int(vpitch - lowestnote) % 7 == 3) {
				diss4Q = true;
				dissonant = true;
				marking = 'N';
				ovoiceindex = lowestnotei;
				oattackindexn = grid.cell(ovoiceindex, sliceindex)->getNextAttackIndex();
				unexp_label = m_labels[UNLABELED_Z4];
			}
		}
*/


		// Don't label current note if not dissonant with other sounding notes.
		if (!dissonant) {
			if (!nodissonanceQ) {
				continue;
			}
		}

		if (colorizeQ) {
			int metriclevel = attacks[i]->getMetricLevel();
			if (metriclevel <= 0) {
				dissL0Q = true;
				marking = 'N';
			} else if (metriclevel < 2) {
				dissL1Q = true;
				marking = '@';
			} else {
				dissL2Q = true;
				marking = '+';
			}
		}

		if ((colorizeQ || colorize2Q) && marking) {
			// mark note
			string text = *attacks[i]->getToken();
			if (text.find(marking) == string::npos) {
				text += marking;
				attacks[i]->getToken()->setText(text);
			}
		}

		// variables for dissonant voice
		durp = attacks[i-1]->getDuration();
		dur  = attacks[i]->getDuration();
		durn = attacks[i+1]->getDuration();
		intp = *attacks[i] - *attacks[i-1];
		intn = *attacks[i+1] - *attacks[i];
		levp = attacks[i-1]->getMetricLevel();
		lev  = attacks[i]->getMetricLevel();
		levn = attacks[i+1]->getMetricLevel();
		if (i >= 2) {
			intpp = *attacks[i-1] - *attacks[i-2];
			durpp = attacks[i-2]->getDuration();
			lineindexpp = attacks[i-2]->getLineIndex();
		}

		// Non-suspension test cases ////////////////////////////////////////////

		// valid_acc_exit determines if the other (accompaniment) voice conforms to the
		// standards of all dissonant types except suspensions.

		// The reference (dissonant) voice moves out of the dissonance to a different
		// pitch at the same time or before the other (accompaniment) voice moves to a
		// different pitch class or a rest:
		bool valid_acc_exit = oattackindexn < attackindexn ? false : true;
		if (oattackindexn < 0) {
			valid_acc_exit = true;
		}

		// Suspension test cases ////////////////////////////////////////////////

		// Condition 2: The other (dissonant) voice stayed in place or repeated the
		//    same pitch at the onset of this dissonant interval.
		bool condition2 = true;
		bool condition2b = false;
		double opitch = grid.cell(ovoiceindex, sliceindex)->getSgnMidiPitch();
		double opitchDia = grid.cell(ovoiceindex, sliceindex)->getAbsDiatonicPitch();
		int lastonoteindex = grid.cell(ovoiceindex, sliceindex)->getPrevAttackIndex();
		double lopitch = NAN;
		if (lastonoteindex >= 0) {
			lopitch = grid.cell(ovoiceindex, lastonoteindex)->getAbsMidiPitch();
			double lopitchDia = grid.cell(ovoiceindex, lastonoteindex)->getAbsDiatonicPitch();
			if (abs(int(opitchDia - lopitchDia)) == 7) {
				condition2b = true;
			}
		} else {
			condition2 = false;
		}
		if (opitch < 0) {
			condition2 = true;
		} else if (opitch != lopitch) {
			condition2 = false;
		}

		int oattackindexp = grid.cell(ovoiceindex, sliceindex)->getPrevAttackIndex();
		int oattackindexc = grid.cell(ovoiceindex, sliceindex)->getCurrAttackIndex();
		odur = grid.cell(ovoiceindex, oattackindexc)->getDuration();
		int olineindexc = grid.cell(ovoiceindex, oattackindexc)->getLineIndex();
		double opitchp = NAN;
		if (oattackindexp >= 0) {
			opitchp = grid.cell(ovoiceindex, oattackindexp)->getAbsDiatonicPitch();
		}

		opitch = grid.cell(ovoiceindex, sliceindex)->getAbsDiatonicPitch();
		int oattackindexn = grid.cell(ovoiceindex, sliceindex)->getNextAttackIndex();

		int olineindexn = -1;
		if (oattackindexn >= 0) {
			olineindexn = grid.cell(ovoiceindex, oattackindexn)->getLineIndex();
		}
		double opitchn = NAN;
		if (oattackindexn >= 0) {
			opitchn = grid.cell(ovoiceindex, oattackindexn)->getAbsDiatonicPitch();
			odurn = grid.cell(ovoiceindex, oattackindexn)->getDuration();
		}
		int oattackindexnn = -1;
		if (oattackindexn >= 0) {
			oattackindexnn = grid.cell(ovoiceindex, oattackindexn)->getNextAttackIndex();
		}
		double opitchnn = NAN;
		if (oattackindexnn >= 0) {
			opitchnn = grid.cell(ovoiceindex, oattackindexnn)->getAbsDiatonicPitch();
		}

		// Condition 3: The other (dissonant) voice leaves its note before
		//    or at the same time as the accompaniment (reference) voice leaves
		//    its pitch class.  [The voices can leave their pitch classes for 
		//    another note or for a rest.]
		bool condition3a = oattackindexn <= attackindexn ? true : false;

		// For ornamented suspensions.
		bool condition3b = oattackindexnn <= attackindexn ? true : false;

		// valid_sus_acc: determines if the reference voice conforms to the
		// standards of the accompaniment voice for suspensions.
		bool valid_sus_acc = condition2 && condition3a;
		bool valid_ornam_sus_acc = condition2 && condition3b;

		double ointp = opitch - opitchp;
		double ointn = opitchn - opitch;
		double ointnn = opitchnn - opitchn;

		// To distinguish between binary and ternary suspensions and agents
		int    getMeterTop          (void);
		HumNum getMeterBottom       (void);

		// Assign time signature ints here:
		refMeterNum = attacks[i]->getMeterTop();
		refMeterDen = attacks[i]->getMeterBottom();
		othMeterNum = grid.cell(ovoiceindex, sliceindex)->getMeterTop();
		othMeterDen = grid.cell(ovoiceindex, sliceindex)->getMeterBottom();
		HumNum threehalves(3, 2);
		HumNum sixteenthirds(16, 3);
		if (othMeterDen == 0) {
			othMeterDen = 8;
		} else if (othMeterDen == 1) {
			othMeterDen = 4;
		} else if (othMeterDen == 4) {
			othMeterDen = 1;
		}

		ternAgent = false;
		if (((othMeterNum % 3 == 0) && (odur >= othMeterDen)) && // the durational value of the meter's denominator groups in threes and the sus lasts at least as long as the denominator
				((dur == othMeterDen*2) || // the ref note lasts 2 times as long as the meter's denominator
				 ((dur == othMeterDen*threehalves) && ((intn == 0) || (intn == -1))) || // ref note lasts 1.5 times the meter's denominator and next note is a tenorizans ornament
				 ((dur == sixteenthirds) && (refMeterNum == 3) && (refMeterDen == threehalves)) || // special case for 3/3 time signature
				 ((odur == othMeterDen*threehalves) && (ointn == -1) && (odurn == 2) && (ointnn == 0)) || // change of agent suspension with ant of resolution
				 ((dur == othMeterDen) && (odur == othMeterDen*2))) && // unornamented change of agent suspension
				(results[ovoiceindex][lineindex] != m_labels[SUS_BIN])) { // the other voice hasn't already been labeled as a binary suspension
			ternAgent = true;
		}


		if (((lev >= levn) || ((lev == 2) && (dur == .5))) && (lev >= levp) && 
			(dur <= 2) && (dur <= durp) && (condition2 || condition2b) && valid_acc_exit) { // weak dissonances
			if (intp == -1) { // descending dissonances
				if (intn == -1) {
					results[vindex][lineindex] = m_labels[PASSING_DOWN]; // downward passing tone
				} else if (intn == 1) {
					results[vindex][lineindex] = m_labels[NEIGHBOR_DOWN]; // lower neighbor
				} else if (intn == 0) {
					results[vindex][lineindex] = m_labels[ANT_DOWN]; // descending anticipation
				} else if (intn > 1) {
					results[vindex][lineindex] = m_labels[ECHAPPE_DOWN]; // lower échappée
				} else if (intn == -2) {
					results[vindex][lineindex] = m_labels[CAMBIATA_DOWN_S]; // descending short nota cambiata
				// } else if (intn < -2) {
				// 	results[vindex][lineindex] = m_labels[IPOSTLOW_NEIGHBOR]; // incomplete posterior lower neighbor
				}
			} else if (intp == 1) { // ascending dissonances
				if (intn == 1) {
					results[vindex][lineindex] = m_labels[PASSING_UP]; // rising passing tone
				} else if (intn == -1) {
					results[vindex][lineindex] = m_labels[NEIGHBOR_UP]; // upper neighbor
				} else if (intn < -1) {
					results[vindex][lineindex] = m_labels[ECHAPPE_UP]; // upper échappée
				} else if (intn == 0) {
					results[vindex][lineindex] = m_labels[ANT_UP]; // rising anticipation
				} else if (intn == 2) {
					results[vindex][lineindex] = m_labels[CAMBIATA_UP_S]; // ascending short nota cambiata
				// } else if (intn > 2) {
				// 	results[vindex][lineindex] = m_labels[IPOSTHI_NEIGHBOR]; // incomplete posterior upper neighbor
				}
			// } else if ((intp < -2) && (intn == 1)) {
			// 	results[vindex][lineindex] = m_labels[IANTLOW_NEIGHBOR]; // incomplete anterior lower neighbor
			// } else if ((intp > 2) && (intn == -1)) {
			// 	results[vindex][lineindex] = m_labels[IANTHI_NEIGHBOR]; // incomplete anterior upper neighbor
			}
		} else if ((durp >= 2) && (dur == 1) && (lev < levn) && valid_acc_exit &&
					 (condition2 || condition2b) && (lev == 1)) {
			if (intp == -1) {
				if (intn == -1) {
					results[vindex][lineindex] = m_labels[THIRD_Q_PASS_DOWN]; // dissonant third quarter descending passing tone
				} else if (intn == 1) {
					results[vindex][lineindex] = m_labels[THIRD_Q_LOWER_NEI]; // dissonant third quarter lower neighbor
				}
			} else if (intp == 1) {
				if (intn == 1) {
					results[vindex][lineindex] = m_labels[THIRD_Q_PASS_UP]; // dissonant third quarter ascending passing tone
				} else if (intn == -1) {
					results[vindex][lineindex] = m_labels[THIRD_Q_UPPER_NEI]; // dissonant third quarter upper neighbor
				}
			}
		} else if (((lev > levp) || (durp+durp+durp+durp == dur)) && 
				   (lev == levn) && condition2 && (intn == -1) && 
				   (dur == (durn+durn)) && ((dur+dur) <= odur)) {
			if ((intp == 1) || ((intp == 0) && (intpp == 1))) {
				results[vindex][lineindex] = m_labels[SUS_NO_AGENT_UP];
			} else if ((intp == -1) || ((intp == 0) && (intpp == -1))) {
				results[vindex][lineindex] = m_labels[SUS_NO_AGENT_DOWN];
			}
		}

		/////////////////////////////
		////
		//// Code to apply binary or ternary suspension and agent labels
		////

		else if (valid_sus_acc && (ointn == -1)) {
			if ((durpp == 1) && (durp == 1) && (intpp == -1) && (intp = 1) &&
				((results[vindex][lineindexpp] == m_labels[UNLABELED_Z7]) ||
				 (results[vindex][lineindexpp] == m_labels[UNLABELED_Z4]))) {
				results[vindex][lineindexpp] = m_labels[CHANSON_IDIOM];
			}
			if (ternAgent) {
				results[vindex][lineindex] = m_labels[AGENT_TERN]; // ternary agent
				results[ovoiceindex][lineindex] = m_labels[SUS_TERN]; // ternary suspension
			} else {
				results[vindex][lineindex] = m_labels[AGENT_BIN]; // binary agent
				results[ovoiceindex][lineindex] = m_labels[SUS_BIN]; // binary suspension
			}
		} else if (valid_ornam_sus_acc && ((ointn == 0) && (ointnn == -1))) {
			if ((durpp == 1) && (durp == 1) && (intpp == -1) && (intp = 1) &&
				((results[vindex][lineindexpp] == m_labels[UNLABELED_Z7]) ||
				 (results[vindex][lineindexpp] == m_labels[UNLABELED_Z4]))) {
				results[vindex][lineindexpp] = m_labels[CHANSON_IDIOM];
			}
			if (ternAgent) {
				results[vindex][lineindex] = m_labels[AGENT_TERN]; // ternary agent
				results[ovoiceindex][lineindex] = m_labels[SUS_TERN]; // ternary suspension
			} else {
				results[vindex][lineindex] = m_labels[AGENT_BIN]; // binary agent
				results[ovoiceindex][lineindex] = m_labels[SUS_BIN]; // binary suspension
			}
			results[ovoiceindex][olineindexn] = m_labels[SUSPENSION_REP]; // repeated-note of suspension
		} else if (valid_ornam_sus_acc && ((ointn == -2) && (ointnn == 1))) {
			if ((durpp == 1) && (durp == 1) && (intpp == -1) && (intp = 1) &&
				((results[vindex][lineindexpp] == m_labels[THIRD_Q_PASS_DOWN] ||
				 (results[vindex][lineindexpp] == m_labels[UNLABELED_Z7]) ||
				 (results[vindex][lineindexpp] == m_labels[UNLABELED_Z4])))) {
				results[vindex][lineindexpp] = m_labels[CHANSON_IDIOM];
			}
			if (ternAgent) {
				results[vindex][lineindex] = m_labels[AGENT_TERN]; // ternary agent
				results[ovoiceindex][lineindex] = m_labels[SUS_TERN]; // ternary suspension
			} else {
				results[vindex][lineindex] = m_labels[AGENT_BIN]; // binary agent
				results[ovoiceindex][lineindex] = m_labels[SUS_BIN]; // binary suspension
			}
			results[ovoiceindex][olineindexn] = m_labels[SUSPENSION_ORNAM]; // suspension ornament
		} else if (valid_ornam_sus_acc && ((ointn == 1) && (ointnn == -2))) {
			if ((durpp == 1) && (durp == 1) && (intpp == -1) && (intp = 1) &&
				((results[vindex][lineindexpp] == m_labels[UNLABELED_Z7]) ||
				 (results[vindex][lineindexpp] == m_labels[UNLABELED_Z4]))) {
				results[vindex][lineindexpp] = m_labels[CHANSON_IDIOM];
			}
			if (ternAgent) {
				results[vindex][lineindex] = m_labels[AGENT_TERN]; // ternary agent
				results[ovoiceindex][lineindex] = m_labels[SUS_TERN]; // ternary suspension
			} else {
				results[vindex][lineindex] = m_labels[AGENT_BIN]; // binary agent
				results[ovoiceindex][lineindex] = m_labels[SUS_BIN]; // binary suspension
			} // NB: in this case the ornament is consonant against the agent so no ornament label.
		}

/////////////////////////////

		if (i < ((int)attacks.size() - 2)) { // expand the analysis window

			double intnn = *attacks[i+2] - *attacks[i+1];
			HumNum durnn = attacks[i+2]->getDuration();	// dur of note after next
			// double levnn = attacks[i+2]->getMetricLevel(); // lev of note after next

			if ((dur <= durp) && (lev >= levp) && (lev >= levn) &&
					(intp == -1) && (intn == -2) && (intnn == 1)) {
				results[vindex][lineindex] = m_labels[CAMBIATA_DOWN_L]; // long-form descending cambiata
			} else if ((dur <= durp) && (lev >= levp) && (lev >= levn) &&
					(intp == 1) && (intn == 2) && (intnn == -1)) {
				results[vindex][lineindex] = m_labels[CAMBIATA_UP_L]; // long-form ascending nota cambiata
			}
		}

		// Decide whether to give an unexplained dissonance label to the ref.
		// voice if none of the dissonant conditions above apply.
		bool refLeaptTo = fabs(intp) > 1 ? true : false;
		bool othLeaptTo = fabs(ointp) > 1 ? true : false;
		bool refLeaptFrom = fabs(intn) > 1 ? true : false;
		bool othLeaptFrom = fabs(ointn) > 1 ? true : false;

		if ((results[vindex][lineindex] == "") && // this voice doesn't already have a dissonance label
			((olineindexc < lineindex) || // other voice does not attack at this point
				((olineindexc == lineindex) && (dur < odur)) || // both voices attack together, but ref voice leaves dissonance first
				(((olineindexc == lineindex) && (dur == odur)) && // both voices enter and leave dissonance simultaneously
				 ((!refLeaptFrom && othLeaptFrom) || // ref voice leaves diss by step or rep and other voice leaves by leap
				  (refLeaptTo && refLeaptFrom && othLeaptTo && othLeaptFrom) || // both voices enter and leave diss by leap
				  ((fabs(intp) == 1) && (fabs(intn) == 1) && !othLeaptTo && !othLeaptFrom) || // ref voice enters and leaves by step, other voice by step or rep
				  ((fabs(intp) == 1) && (intn == 0) && !othLeaptTo && (ointn == 0)) || // ref enters by step and leaves by rep, other v enters by step or rep and leaves by rep
				  (!refLeaptTo && refLeaptFrom && othLeaptFrom))))) { // ref voice enters diss by step and both voices leave by leap
			results[vindex][lineindex] = unexp_label;
		}


		// If the note was labeled as an unknown dissonance, then go back and check
		// against another note with which it might have a known dissonant function.
		if ((results[vindex][lineindex] == m_labels[UNLABELED_Z4]) || 
				(results[vindex][lineindex] == m_labels[UNLABELED_Z7]) ||
				(results[vindex][lineindex] == m_labels[UNLABELED_Z7])) {
			if (nextj < (int)harmint.size()) {
				goto RECONSIDER;
			}
		}
	}

}



//////////////////////////////
//
// Tool_dissonant::findFakeSuspensions --
//

void Tool_dissonant::findFakeSuspensions(vector<vector<string> >& results, NoteGrid& grid,
		vector<NoteCell*>& attacks, int vindex) {
	double intp;        // diatonic interval from previous melodic note
	int lineindexn;     // line index of the next note in the voice
	bool sfound;        // boolean for if a suspension is found after a Z dissonance

	for (int i=1; i<(int)attacks.size()-1; i++) {
		int lineindex = attacks[i]->getLineIndex();
		if ((results[vindex][lineindex].find("Z") == string::npos) &&
			(results[vindex][lineindex].find("z") == string::npos) &&
			(results[vindex][lineindex].find("M") == string::npos) &&
			(results[vindex][lineindex].find("m") == string::npos)) {
			continue;
		}
		intp = *attacks[i] - *attacks[i-1];
		lineindexn = attacks[i+1]->getLineIndex();
		sfound = false;
		for (int j=lineindex + 1; j<=lineindexn; j++) {
			if ((results[vindex][j].compare(0, 1, "s") == 0) ||
			    (results[vindex][j].compare(0, 1, "S") == 0)) {
				sfound = true;
				break;
			}
		}
		if (!sfound) {
			continue;
		}
		// Also may need to check for the existance of another voice attacked before Z 
		// and sustained through to the beginning of the resolution.

		// Apply labels for normal fake suspensions.
		if (intp == 1) {
			results[vindex][lineindex] = m_labels[FAKE_SUSPENSION_UP];
		} else if (intp == -1) {
			results[vindex][lineindex] = m_labels[FAKE_SUSPENSION_DOWN];
		} else if (i > 1) { // as long as i > 1 intpp will be in range.
			// The next two fake suspension types are preceded by an anticipation.
			double intpp = *attacks[i-1] - *attacks[i-2];
			if ((intp == 0) && (intpp == 1)) {
				results[vindex][lineindex] = m_labels[FAKE_SUSPENSION_UP];
			} else if ((intp == 0) && (intpp == -1)) {
				results[vindex][lineindex] = m_labels[FAKE_SUSPENSION_DOWN];
			}
		}
	}
}

//////////////////////////////
//
// Tool_dissonant::findLs --
//
void Tool_dissonant::findLs(vector<vector<string> >& results, NoteGrid& grid,
		vector<NoteCell*>& attacks, int vindex) {
	HumNum dur;        // duration of current note;
	HumNum odur;       // duration of current note in other voice which may have started earlier;
	double intp;       // diatonic interval from previous melodic note
	double intn;       // diatonic interval to next melodic note
	double ointp;      // diatonic interval from previous melodic note in other voice
	double ointn;      // diatonic interval to next melodic note in other voice
	int lineindex;     // line in original Humdrum file content that contains note
	int olineindex;   // line in original Humdrum file content that contains other voice note
	int sliceindex;    // current timepoint in NoteGrid.
	int oattackindexp; // line index of other voice's previous note
	int oattackindexc; // line index of other voice's current note
	int oattackindexn; // line index of other voice's next note
	double opitchp;    // previous pitch in other voice
	double opitch;     // current pitch in other voice
	double opitchn;    // next pitch in other voice

	for (int i=1; i<(int)attacks.size()-1; i++) {
		lineindex = attacks[i]->getLineIndex();
		if ((results[vindex][lineindex].find("Z") == string::npos) &&
			(results[vindex][lineindex].find("z") == string::npos)) {
			continue;
		}
		dur  = attacks[i]->getDuration();
		intp = *attacks[i] - *attacks[i-1];
		intn = *attacks[i+1] - *attacks[i];
		sliceindex = attacks[i]->getSliceIndex();

		for (int j=0; j<(int)grid.getVoiceCount(); j++) { // j is the voice index of the other voice
			if (vindex == j) { // only compare different voices
				continue;
			}
			if ((results[j][lineindex] == m_labels[AGENT_BIN]) ||
				(results[j][lineindex] == m_labels[AGENT_TERN]) ||
				(results[j][lineindex] == m_labels[UNLABELED_Z7]) ||
				(results[j][lineindex] == m_labels[UNLABELED_Z4]) ||
				(results[j][lineindex] == "")) {
				continue; // skip if other voice is an agent, unexplainable, or empty.
			}
			oattackindexc = grid.cell(j, sliceindex)->getCurrAttackIndex();
			olineindex = grid.cell(j, oattackindexc)->getLineIndex();
			if (olineindex != lineindex) { // if olineindex == lineindex then oattackindexp is in range
				continue; // skip if other voice doesn't attack at the same time
			}
			oattackindexp = grid.cell(j, sliceindex)->getPrevAttackIndex();
			odur = grid.cell(j, oattackindexc)->getDuration();
			if (dur != odur) { // if dur == odur then the oattackindexn will be in range
				continue;
			}
			opitchp = grid.cell(j, oattackindexp)->getAbsDiatonicPitch();
			opitch = grid.cell(j, sliceindex)->getAbsDiatonicPitch();
			oattackindexn = grid.cell(j, sliceindex)->getNextAttackIndex();
			opitchn = grid.cell(j, oattackindexn)->getAbsDiatonicPitch();
			ointp = opitch - opitchp;
			ointn = opitchn - opitch;
			if ((intp == ointp) && (intn == ointn)) { // this note moves in parallel with an identifiable dissonance
				if (intp > 0) {
					results[vindex][lineindex] = m_labels[PARALLEL_UP];
					break;
				} else if (intp < 0) {
					results[vindex][lineindex] = m_labels[PARALLEL_DOWN];
					break;			
				}
			}
		}
	}
}

//////////////////////////////
//
// Tool_dissonant::findYs --
//
void Tool_dissonant::findYs(vector<vector<string> >& results, NoteGrid& grid,
		vector<NoteCell*>& attacks, int vindex) { 
	double intp;       // diatonic interval from previous melodic note
	int lineindex;     // line in original Humdrum file content that contains note
	int olineindex;    // line in original Humdrum file content that contains other voice note
	int sliceindex;    // current timepoint in NoteGrid
	int oattackindexc; // line index of other voice current note
	double pitch;      // current pitch in this voice
	double opitch;     // current pitch in other voice
	bool onlyWithValids; // note is only dissonant with identifiable dissonances

	for (int i=1; i<(int)attacks.size()-1; i++) {
		lineindex = attacks[i]->getLineIndex();
		if ((results[vindex][lineindex].find("Z") == string::npos) &&
			(results[vindex][lineindex].find("z") == string::npos)) {
			continue;
		}
		intp = *attacks[i] - *attacks[i-1];
		sliceindex = attacks[i]->getSliceIndex();

		int lowestnote = 1000; // lowest sounding diatonic note in any voice at this sliceindex
		double tpitch;
		for (int v=0; v<(int)grid.getVoiceCount(); v++) {
			tpitch = grid.cell(v, sliceindex)->getAbsDiatonicPitch();
			if (!Convert::isNaN(tpitch)) {
				if (tpitch <= lowestnote) {
					lowestnote = tpitch;
				}
			}
		}

		onlyWithValids = true; 
		for (int j=0; j<(int)grid.getVoiceCount(); j++) { // j is the voice index of the other voice
			if (vindex == j) { // only compare different voices
				continue;
			}
			oattackindexc = grid.cell(j, sliceindex)->getCurrAttackIndex();
			pitch = attacks[i]->getAbsDiatonicPitch();
			opitch = grid.cell(j, sliceindex)->getAbsDiatonicPitch();
			olineindex = grid.cell(j, oattackindexc)->getLineIndex();
			int thisInt = opitch - pitch; // diatonic interval in this pair
			int thisMod7 = thisInt % 7; // simplify octaves out of thisInt


			if (((abs(thisMod7) == 1) || (abs(thisMod7) == 6)  ||
				 ((thisInt > 0) && (thisMod7 == 3) && 
				  not (((int(pitch-lowestnote) % 7) == 2) ||
                 	   ((int(pitch-lowestnote) % 7) == 4))) ||
				 ((thisInt < 0) && (thisMod7 == -3) && // a fourth by inversion is -3 and -3%7 = -3.
				  not (((int(opitch-lowestnote) % 7) == 2) ||
                 	   ((int(opitch-lowestnote) % 7) == 4)))) &&
				((results[j][olineindex] == m_labels[AGENT_BIN]) ||
				 (results[j][olineindex] == m_labels[AGENT_TERN]) ||
				 (results[j][olineindex] == m_labels[UNLABELED_Z7]) ||
				 (results[j][olineindex] == m_labels[UNLABELED_Z4]) ||
				 ((results[j][olineindex] == "") &&
				  ((results[j][lineindex] != m_labels[SUS_BIN]) &&
				   (results[j][lineindex] != m_labels[SUS_TERN]))))) {
				
				onlyWithValids = false;
			}
		}

		if (onlyWithValids && ((results[vindex][lineindex] == m_labels[UNLABELED_Z7]) ||
							   (results[vindex][lineindex] == m_labels[UNLABELED_Z4]))) {
			if (intp > 0) {
				results[vindex][lineindex] = m_labels[ONLY_WITH_VALID_UP];
			} else if (intp <= 0) {
				results[vindex][lineindex] = m_labels[ONLY_WITH_VALID_DOWN];
			}
		}
	}
}

///////////////////////////////
//
// printCountAnalysis --
//

void Tool_dissonant::printCountAnalysis(vector<vector<string> >& data) {

	map<string, bool> reduced;
	bool brief = getBoolean("u");
	bool percentQ = getBoolean("percent");

	vector<map<string, int> > analysis;
	analysis.resize(data.size());
	int i;
	int j;
	for (i=0; i<(int)data.size(); i++) {
		for (j=0; j<(int)data[i].size(); j++) {
			if (analysis[i].find(data[i][j]) != analysis[i].end()) {
				analysis[i][data[i][j]]++;
			} else {
				analysis[i][data[i][j]] = 1;
			}
		}
	}

	m_humdrum_text << "**dis";
	if (brief) {
		m_humdrum_text << "u";
	}
	m_humdrum_text << "\t**sum";
	for (j=0; j<(int)analysis.size(); j++) {
		m_humdrum_text << "\t" << "**v" << j + 1;
	}
	m_humdrum_text << endl;

	int sumsum = 0;
	int sum;
	string item;
	for (i=0; i<(int)LABELS_SIZE; i++) {

		item = m_labels[i];

		if (brief && (reduced.find(item) != reduced.end())) {
			continue;
		}
		reduced[item] = 1;

		sum = 0;
		for (j=0; j<(int)analysis.size(); j++) {
			if (analysis[j].find(item) != analysis[j].end()) {
				sum += analysis[j][item];
				sumsum += analysis[j][item];
			}
		}

		if (sum == 0) {
			continue;
		}

		m_humdrum_text << item;
		m_humdrum_text << "\t" << sum;

		for (int j=0; j<(int)analysis.size(); j++) {
			m_humdrum_text << "\t";
			if (analysis[j].find(item) != analysis[j].end()) {
				if (percentQ) {
					m_humdrum_text << int(analysis[j][item] * 1.0 / sum * 1000.0 + 0.5) / 10.0;
				} else {
					m_humdrum_text << analysis[j][item];
				}
			} else {
				m_humdrum_text << 0;
			}
		}
		m_humdrum_text << endl;
	}

	m_humdrum_text << "*-\t*-";
	for (j=0; j<(int)analysis.size(); j++) {
		m_humdrum_text << "\t" << "*-";
	}
	m_humdrum_text << endl;

	m_humdrum_text << "!!total_dissonances:\t" << sumsum << endl;

}



//////////////////////////////
//
// Tool_dissonant::getNextPitchAttackIndex -- Get the [line] index of the next
//     note attack, excluding any repeated pitch note attacks.
//

int Tool_dissonant::getNextPitchAttackIndex(NoteGrid& grid, int voicei, int sliceindex) {
	double pitch = NAN;
	int endslice = -1;
	if (sliceindex >= 0) {
		pitch = grid.cell(voicei, sliceindex)->getAbsMidiPitch();
		endslice = grid.cell(voicei, sliceindex)->getNextAttackIndex();
	}

	double pitch2 = NAN;
	if (endslice >= 0) {
		pitch2 = grid.cell(voicei, endslice)->getAbsMidiPitch();
	}

	if (Convert::isNaN(pitch)) {
		return endslice;
	}

	while (pitch == pitch2) {
		endslice = grid.cell(voicei, endslice)->getNextAttackIndex();
		pitch2 = NAN;
		if (endslice >= 0) {
			pitch2 = grid.cell(voicei, endslice)->getAbsMidiPitch();
		} else {
			break;
		}
	}

	return endslice;
}



//////////////////////////////
//
// Tool_dissonant::fillLabels -- Assign the labels for non-harmonic tone analysis.
//

void Tool_dissonant::fillLabels(void) {
	m_labels.resize(LABELS_SIZE);
	m_labels[PASSING_UP          ] = "P"; // rising passing tone
	m_labels[PASSING_DOWN        ] = "p"; // downward passing tone
	m_labels[NEIGHBOR_UP         ] = "N"; // upper neighbor
	m_labels[NEIGHBOR_DOWN       ] = "n"; // lower neighbor
	m_labels[ECHAPPE_UP          ] = "E"; // upper échappée
	m_labels[ECHAPPE_DOWN        ] = "e"; // lower échappée
	m_labels[CAMBIATA_UP_S       ] = "C"; // ascending short nota cambiata
	m_labels[CAMBIATA_DOWN_S     ] = "c"; // descending short nota cambiata
	m_labels[CAMBIATA_UP_L       ] = "K"; // ascending long nota cambiata
	m_labels[CAMBIATA_DOWN_L     ] = "k"; // descending long nota cambiata
	// m_labels[IPOSTHI_NEIGHBOR    ] = "J"; // incomplete posterior upper neighbor
	// m_labels[IPOSTLOW_NEIGHBOR   ] = "j"; // incomplete posterior lower neighbor
	// m_labels[IANTHI_NEIGHBOR     ] = "I"; // incomplete anterior upper neighbor
	// m_labels[IANTLOW_NEIGHBOR    ] = "i"; // incomplete anterior lower neighbor
	m_labels[ANT_UP              ] = "A"; // rising anticipation
	m_labels[ANT_DOWN            ] = "a"; // descending anticipation
	m_labels[THIRD_Q_PASS_UP     ] = "Q"; // dissonant third quarter ascending passing tone
	m_labels[THIRD_Q_PASS_DOWN   ] = "q"; // dissonant third quarter descending passing tone
	m_labels[THIRD_Q_UPPER_NEI   ] = "B"; // dissonant third quarter upper neighbor
	m_labels[THIRD_Q_LOWER_NEI   ] = "b"; // dissonant third quarter lower neighbor
	m_labels[SUS_BIN             ] = "s"; // binary suspension
	m_labels[SUS_TERN            ] = "S"; // ternary suspension
	m_labels[AGENT_BIN           ] = "g"; // binary agent
	m_labels[AGENT_TERN          ] = "G"; // ternary agent
	m_labels[SUSPENSION_ORNAM    ] = "o"; // suspension ornament
	m_labels[SUSPENSION_REP      ] = "r"; // suspension repeated note
	m_labels[FAKE_SUSPENSION_UP  ] = "F"; // fake suspension approached by step up
	m_labels[FAKE_SUSPENSION_DOWN] = "f"; // fake suspension approached by step down
	m_labels[SUS_NO_AGENT_UP     ] = "M"; // suspension missing a normal agent approached by step up
	m_labels[SUS_NO_AGENT_DOWN   ] = "m"; // suspension missing a normal agent approached by step down
	m_labels[CHANSON_IDIOM       ] = "h"; // chanson idiom
	m_labels[PARALLEL_UP         ] = "L"; // moves up in parallel with identifiable dissonance
	m_labels[PARALLEL_DOWN       ] = "l"; // moves down in parallel with identifiable dissonance
	m_labels[ONLY_WITH_VALID_UP  ] = "Y"; // only dissonant against identifiable dissonances, approached from below
	m_labels[ONLY_WITH_VALID_DOWN] = "y"; // only dissonant against identifiable dissonances, approached from above
	m_labels[UNKNOWN_DISSONANCE  ] = "Z"; // unknown dissonance
	m_labels[UNLABELED_Z2        ] = "Z"; // unknown dissonance, 2nd interval
	m_labels[UNLABELED_Z7        ] = "Z"; // unknown dissonance, 7th interval
	m_labels[UNLABELED_Z4        ] = "z"; // unknown dissonance, 4th interval
}



//////////////////////////////
//
// Tool_dissonant::fillLabels2 -- Assign the labels for non-harmonic tone analysis.
//     This version without direction separation.
//

void Tool_dissonant::fillLabels2(void) {
	m_labels.resize(LABELS_SIZE);
	m_labels[PASSING_UP          ] = "P"; // rising passing tone
	m_labels[PASSING_DOWN        ] = "P"; // downward passing tone
	m_labels[NEIGHBOR_UP         ] = "N"; // upper neighbor
	m_labels[NEIGHBOR_DOWN       ] = "N"; // lower neighbor
	m_labels[ECHAPPE_UP          ] = "E"; // upper échappée
	m_labels[ECHAPPE_DOWN        ] = "E"; // lower échappée
	m_labels[CAMBIATA_UP_S       ] = "C"; // ascending short nota cambiata
	m_labels[CAMBIATA_DOWN_S     ] = "C"; // descending short nota cambiata
	m_labels[CAMBIATA_UP_L       ] = "K"; // ascending long nota cambiata
	m_labels[CAMBIATA_DOWN_L     ] = "K"; // descending long nota cambiata
	// m_labels[IPOSTHI_NEIGHBOR    ] = "J"; // incomplete posterior upper neighbor
	// m_labels[IPOSTLOW_NEIGHBOR   ] = "J"; // incomplete posterior lower neighbor
	// m_labels[IANTHI_NEIGHBOR     ] = "I"; // incomplete anterior upper neighbor
	// m_labels[IANTLOW_NEIGHBOR    ] = "I"; // incomplete anterior lower neighbor
	m_labels[ANT_UP              ] = "A"; // rising anticipation
	m_labels[ANT_DOWN            ] = "A"; // descending anticipation
	m_labels[THIRD_Q_PASS_UP     ] = "Q"; // dissonant third quarter ascending passing tone
	m_labels[THIRD_Q_PASS_DOWN   ] = "Q"; // dissonant third quarter descending passing tone
	m_labels[THIRD_Q_UPPER_NEI   ] = "B"; // dissonant third quarter upper neighbor
	m_labels[THIRD_Q_LOWER_NEI   ] = "B"; // dissonant third quarter lower neighbor
	m_labels[SUS_BIN             ] = "S"; // binary suspension
	m_labels[SUS_TERN            ] = "S"; // ternary suspension
	m_labels[AGENT_BIN           ] = "G"; // binary agent
	m_labels[AGENT_TERN          ] = "G"; // ternary agent
	m_labels[SUSPENSION_ORNAM    ] = "O"; // suspension ornament
	m_labels[SUSPENSION_REP      ] = "R"; // suspension repeated note
	m_labels[FAKE_SUSPENSION_UP  ] = "F"; // fake suspension approached by step up
	m_labels[FAKE_SUSPENSION_DOWN] = "F"; // fake suspension approached by step down
	m_labels[SUS_NO_AGENT_UP     ] = "M"; // suspension missing a normal agent approached by step up
	m_labels[SUS_NO_AGENT_DOWN   ] = "M"; // suspension missing a normal agent approached by step down
	m_labels[CHANSON_IDIOM       ] = "H"; // chanson idiom
	m_labels[PARALLEL_UP         ] = "L"; // moves up in parallel with identifiable dissonance
	m_labels[PARALLEL_DOWN       ] = "L"; // moves down in parallel with identifiable dissonance
	m_labels[ONLY_WITH_VALID_UP  ] = "Y"; // only dissonant against identifiable dissonances, approached from below
	m_labels[ONLY_WITH_VALID_DOWN] = "Y"; // only dissonant against identifiable dissonances, approached from above
	m_labels[UNKNOWN_DISSONANCE  ] = "Z"; // unknown dissonance
	m_labels[UNLABELED_Z2        ] = "Z"; // unknown dissonance, 2nd interval
	m_labels[UNLABELED_Z7        ] = "Z"; // unknown dissonance, 7th interval
	m_labels[UNLABELED_Z4        ] = "Z"; // unknown dissonance, 4th interval
}




/////////////////////////////////
//
// Tool_esac2hum::Tool_esac2hum -- Set the recognized options for the tool.
//

Tool_esac2hum::Tool_esac2hum(void) {
	define("debug=b",            "print debug information");
	define("v|verbose=b",        "verbose output");
	define("h|header=s:",        "Header filename for placement in output");
	define("t|trailer=s:",       "Trailer filename for placement in output");
	define("s|split=s:file",     "Split song info into separate files");
	define("x|extension=s:.krn", "Split filename extension");
	define("f|first=i:1",        "Number of first split filename");
	define("author=b",           "author of program");
	define("version=b",          "compilation info");
	define("example=b",          "example usages");
	define("help=b",             "short description");
}



//////////////////////////////
//
// Tool_esac2hum::convert -- Convert a MusicXML file into
//     Humdrum content.
//

bool Tool_esac2hum::convertFile(ostream& out, const string& filename) {
	ifstream file(filename);
	stringstream s;
	if (file) {
		s << file.rdbuf();
		file.close();
	}
	return convert(out, s.str());
}


bool Tool_esac2hum::convert(ostream& out, istream& input) {
	convertEsacToHumdrum(out, input);
	return true;
}


bool Tool_esac2hum::convert(ostream& out, const string& input) {
	stringstream ss;
	ss << input;
	convertEsacToHumdrum(out, ss);
	return true;
}




//////////////////////////////
//
// Tool_esac2hum::initialize --
//

bool Tool_esac2hum::initialize(void) {
	// handle basic options:
	if (getBoolean("author")) {
		cerr << "Written by Craig Stuart Sapp, "
			  << "craig@ccrma.stanford.edu, March 2002" << endl;
		return false;
	} else if (getBoolean("version")) {
		cerr << getCommand() << ", version: 6 June 2017" << endl;
		cerr << "compiled: " << __DATE__ << endl;
		return false;
	} else if (getBoolean("help")) {
		usage(getCommand());
		return false;
	} else if (getBoolean("example")) {
		example();
		return false;
	}

	debugQ   = getBoolean("debug");
	verboseQ = getBoolean("verbose");

	if (getBoolean("header")) {
		if (!getFileContents(header, getString("header"))) {
			return false;
		}
	} else {
		header.resize(0);
	}
	if (getBoolean("trailer")) {
		if (!getFileContents(trailer, getString("trailer"))) {
			return false;
		}
	} else {
		trailer.resize(0);
	}

	if (getBoolean("split")) {
		splitQ = 1;
	}
	namebase = getString("split");
	fileextension = getString("extension");
	firstfilenum = getInteger("first");
	return true;
}



//////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// Tool_esac2hum::convertEsacToHumdrum --
//

void Tool_esac2hum::convertEsacToHumdrum(ostream& output, istream& infile) {
	initialize();
	vector<string> song;
	song.reserve(400);
	int init = 0;
	// int filecounter = firstfilenum;
	string outfilename;
	string numberstring;
	// ofstream outfile;
	while (!infile.eof()) {
		if (debugQ) {
			cerr << "Getting a song..." << endl;
		}
		getSong(song, infile, init);
		if (debugQ) {
			cerr << "Got a song ..." << endl;
		}
		init = 1;
/*
		if (splitQ) {
			outfilename = namebase);
			outfilename += to_string(filecounter);
			if (filecounter < 1000) {
				outfilename += "0";
			}
			if (filecounter < 100) {
				outfilename += "0";
			}
			if (filecounter < 10) {
				outfilename += "0";
			}
			outfilename += numberstring;
			outfilename += fileextension;
			filecounter++;

			outfile.open(outfilename);

			if (!outfile.is_open()) {
				cerr << "Error: cannot write to file: " << outfilename << endl;
			}
			convertSong(song, outfile);
			outfile.close();
		} else {
*/
			convertSong(song, output);
/*
		}
*/
	}
}



//////////////////////////////
//
// Tool_esac2hum::getSong -- get a song from the ESac file
//

bool Tool_esac2hum::getSong(vector<string>& song, istream& infile, int init) {
	static char holdbuffer[10000] = {0};

	song.resize(0);
	if (init) {
		// do nothing holdbuffer has the CUT[] information
	} else {
		strcpy(holdbuffer, "");
		while (!infile.eof() && strncmp(holdbuffer, "CUT[", 4) != 0) {
			infile.getline(holdbuffer, 256, '\n');
			if (verboseQ) {
				cerr << "Contents: " << holdbuffer << endl;
			}
			if (strncmp(holdbuffer, "!!", 2) == 0) {
				song.push_back(holdbuffer);
			}
		}
		if (infile.eof()) {
			return false;
		}
	}

	if (!infile.eof()) {
		song.push_back(holdbuffer);
	} else {
		return false;
	}

	infile.getline(holdbuffer, 256, '\n');
	chopExtraInfo(holdbuffer);
	inputline++;
	if (verboseQ) {
		cerr << "READ LINE: " << holdbuffer << endl;
	}
	while (!infile.eof() && strncmp(holdbuffer, "CUT[", 4) != 0) {
		song.push_back(holdbuffer);
		infile.getline(holdbuffer, 256, '\n');
		chopExtraInfo(holdbuffer);
		inputline++;
		if (verboseQ) {
			cerr << "READ ANOTHER LINE: " << holdbuffer << endl;
		}
	}

	return true;
}



//////////////////////////////
//
// Tool_esac2hum::chopExtraInfo -- remove phrase number information from Luxembourg data.
//

void Tool_esac2hum::chopExtraInfo(char* holdbuffer) {
	int length = strlen(holdbuffer);
	int i;
	int spacecount = 0;
	for (i=length-2; i>=0; i--) {
		if (holdbuffer[i] == ' ') {
			spacecount++;
			if (spacecount > 10) {
				holdbuffer[i] = '\0';
				break;
			}
		} else {
			spacecount = 0;
		}
	}
}



//////////////////////////////
//
// Tool_esac2hum::printHumdrumHeaderInfo --
//

void Tool_esac2hum::printHumdrumHeaderInfo(ostream& out, vector<string>& song) {
	for (int i=0; i<(int)song.size(); i++) {
		if (song[i].size() == 0) {
			continue;
		}
		if (song[i].compare(0, 2, "!!") == 0) {
			out << song[i] << "\n";
			continue;
		}
		if ((song[i][0] == ' ') || (song[i][0] == '\t')) {
			continue;
		}
		break;
	}
}



//////////////////////////////
//
// Tool_esac2hum::printHumdrumFooterInfo --
//

void Tool_esac2hum::printHumdrumFooterInfo(ostream& out, vector<string>& song) {
	int i = 0;
	for (i=0; i<(int)song.size(); i++) {
		if (song[i].size() == 0) {
			continue;
		}
		if (song[i].compare(0, 2, "!!") == 0) {
			continue;
		}
		if ((song[i][0] == ' ') || (song[i][0] == '\t')) {
			continue;
		}
		break;
	}
	int j = i;
	for (j=i; j<(int)song.size(); j++) {
		if (song[j].compare(0, 2, "!!") == 0) {
			out << song[j] << "\n";
		}
	}
}



//////////////////////////////
//
// Tool_esac2hum::convertSong --
//

void Tool_esac2hum::convertSong(vector<string>& song, ostream& out) {

	int i;
	if (verboseQ) {
		for (i=0; i<(int)song.size(); i++) {
			out << song[i] << "\n";
		}
	}

	printHumdrumHeaderInfo(out, song);

	string key;
	double mindur = 1.0;
	string meter;
	int tonic;
	getKeyInfo(song, key, mindur, tonic, meter, out);

	vector<NoteData> songdata;
	songdata.resize(0);
	songdata.reserve(1000);
	getNoteList(song, songdata, mindur, tonic);
	placeLyrics(song, songdata);

	vector<int> numerator;
	vector<int> denominator;
	getMeterInfo(meter, numerator, denominator);

	postProcessSongData(songdata, numerator, denominator);

	printTitleInfo(song, out);
	out << "!!!id: "    << key  << "\n";

	// check for presence of lyrics
	int textQ = 0;
	for (i=0; i<(int)songdata.size(); i++) {
		if (songdata[i].text !=  "") {
			textQ = 1;
			break;
		}
	}

	for (i=0; i<(int)header.size(); i++) {
		out << header[i] << "\n";
	}

	out << "**kern";
	if (textQ) {
		out << "\t**text";
	}
	out << "\n";

	printKeyInfo(songdata, tonic, textQ, out);
	for (i=0; i<(int)songdata.size(); i++) {
		printNoteData(songdata[i], textQ, out);
	}
	out << "*-";
	if (textQ) {
		out << "\t*-";
	}
	out << "\n";

	out << "!!!minrhy: ";
	out << Convert::durationFloatToRecip(mindur)<<"\n";
	out << "!!!meter";
	if (numerator.size() > 1) {
		out << "s";
	}
	out << ": "  << meter;
	if ((meter == "frei") || (meter == "Frei")) {
		out << " [unmetered]";
	} else if (meter.find('/') == string::npos) {
		out << " interpreted as [";
		for (i=0; i<(int)numerator.size(); i++) {
			out << numerator[i] << "/" << denominator[i];
			if (i < (int)numerator.size()-1) {
				out << ", ";
			}
		}
		out << "]";
	}
	out << "\n";

	printBibInfo(song, out);
	printSpecialChars(out);

	for (i=0; i<(int)songdata.size(); i++) {
		if (songdata[i].lyricerr) {
			out << "!!!RWG: Lyric placement mismatch "
				  << "in phrase (too many syllables) " << songdata[i].phnum << " ["
				  << key << "]\n";
			break;
		}
	}

	for (i=0; i<(int)trailer.size(); i++) {
		out << trailer[i] << "\n";
	}

	printHumdrumFooterInfo(out, song);

/*
	if (!splitQ) {
		out << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	}
*/
}



//////////////////////////////
//
// Tool_esac2hum::placeLyrics -- extract lyrics (if any) and place on correct notes
//

bool Tool_esac2hum::placeLyrics(vector<string>& song, vector<NoteData>& songdata) {
	int start = -1;
	int stop = -1;
	getLineRange(song, "TXT", start, stop);
	if (start < 0) {
		// no TXT[] field, so don't do anything
		return true;
	}
	int line = 0;
	vector<string> lyrics;
	string buffer;
	for (line=0; line<=stop-start; line++) {
		if (song[line+start].size() <= 4) {
			cerr << "Error: lyric line is too short!: "
				  << song[line+start] << endl;
			return false;
		}
		buffer = song[line+start].substr(4);
		if (line == stop - start) {
			auto loc = buffer.rfind(']');
			if (loc != string::npos) {
				buffer.resize(loc);
			}
		}
		if (buffer == "") {
			continue;
		}
		getLyrics(lyrics, buffer);
		cleanupLyrics(lyrics);
		placeLyricPhrase(songdata, lyrics, line);
	}

	return true;
}



//////////////////////////////
//
// Tool_esac2hum::cleanupLyrics -- add preceeding dashes, avoid starting *'s if any,
//    and convert _'s to spaces.
//

void Tool_esac2hum::cleanupLyrics(vector<string>& lyrics) {
	int length;
	int length2;
	int i, j, m;
	int lastsyl = 0;
	for (i=0; i<(int)lyrics.size(); i++) {
		length = lyrics[i].size();
		for (j=0; j<length; j++) {
			if (lyrics[i][j] == '_') {
				lyrics[i][j] = ' ';
			}
		}

		if (i > 0) {
			if ((lyrics[i] != ".") &&
				 (lyrics[i] != "")  &&
				 (lyrics[i] != "%") &&
				 (lyrics[i] != "^") &&
				 (lyrics[i] != "|") &&
				 (lyrics[i] != " ")) {
				lastsyl = -1;
				for (m=i-1; m>=0; m--) {
					if ((lyrics[m] != ".") &&
						 (lyrics[m] != "")  &&
						 (lyrics[m] != "%") &&
						 (lyrics[i] != "^") &&
						 (lyrics[m] != "|") &&
						 (lyrics[m] != " ")) {
						lastsyl = m;
						break;
					}
				}
				if (lastsyl >= 0) {
					length2 = lyrics[lastsyl].size();
					if (lyrics[lastsyl][length2-1] == '-') {
						for (j=0; j<=length; j++) {
							lyrics[i][length - j + 1] = lyrics[i][length - j];
						}
						lyrics[i][0] = '-';
					}
				}
			}
		}

		// avoid *'s on the start of lyrics by placing a space before
		// them if they exist.
		if (lyrics[i][0] == '*') {
			length = lyrics[i].size();
			for (j=0; j<=length; j++) {
				lyrics[i][length - j + 1] = lyrics[i][length - j];
			}
			lyrics[i][0] = ' ';
		}

		// avoid !'s on the start of lyrics by placing a space before
		// them if they exist.
		if (lyrics[i][0] == '!') {
			length = lyrics[i].size();
			for (j=0; j<=length; j++) {
				lyrics[i][length - j + 1] = lyrics[i][length - j];
			}
			lyrics[i][0] = ' ';
		}

	}

}



///////////////////////////////
//
// Tool_esac2hum::getLyrics -- extract the lyrics from the text string.
//

void Tool_esac2hum::getLyrics(vector<string>& lyrics, const string& buffer) {
	lyrics.resize(0);
	int zero1 = 0;
	string current;
	int zero2 = 0;
	zero2 = zero1 + zero2;

	int length = buffer.size();
	int i;

	i = 0;
	while (i<length) {
		current = "";
		if (buffer[i] == ' ') {
			current = ".";
			lyrics.push_back(current);
			i++;
			continue;
		}

		while (i < length && buffer[i] != ' ') {
			current += buffer[i++];
		}
		lyrics.push_back(current);
		i++;
	}

}



//////////////////////////////
//
// Tool_esac2hum::placeLyricPhrase -- match lyrics from a phrase to the songdata.
//

bool Tool_esac2hum::placeLyricPhrase(vector<NoteData>& songdata, vector<string>& lyrics, int line) {
	int i = 0;
	int start = 0;
	int found = 0;

	if (lyrics.size() == 0) {
		return true;
	}

	// find the phrase to which the lyrics belongs
	for (i=0; i<(int)songdata.size(); i++) {
		if (songdata[i].phnum == line) {
			found = 1;
			break;
		}
	}
	start = i;

	if (!found) {
		cerr << "Error: cannot find music for lyrics line " << line << endl;
		cerr << "Error near input data line: " << inputline << endl;
		return false;
	}

	for (i=0; i<(int)lyrics.size() && i+start < (int)songdata.size(); i++) {
		if ((lyrics[i] == " ") || (lyrics[i] == ".") || (lyrics[i] == "")) {
			if (songdata[i+start].pitch < 0) {
				lyrics[i] = "%";
			} else {
				lyrics[i] = "|";
			}
			// lyrics[i] = ".";
		}
		songdata[i+start].text = lyrics[i];
		songdata[i+start].lyricnum = line;
		if (line != songdata[i+start].phnum) {
			songdata[i+start].lyricerr = 1;   // lyric does not line up with music
		}
	}

	return true;
}



//////////////////////////////
//
// Tool_esac2hum::printSpecialChars -- print high ASCII character table
//

void Tool_esac2hum::printSpecialChars(ostream& out) {
	int i;
	for (i=0; i<(int)chartable.size(); i++) {
		if (chartable[i]) {
		switch (i) {
			case 129:   out << "!!!RNB" << ": symbol: &uuml;  = u umlaut (UTF-8: "
							     << (char)0xc3 << (char)0xb3 << ")\n";    break;
			case 130:   out << "!!!RNB" << ": symbol: &eacute;= e acute  (UTF-8: "
							     << (char)0xc3 << (char)0xa9 << ")\n";    break;
			case 132:   out << "!!!RNB" << ": symbol: &auml;  = a umlaut (UTF-8: "
							     << (char)0xc3 << (char)0xa4 << ")\n";    break;
			case 134:   out << "!!!RNB" << ": symbol: $c      = c acute  (UTF-8: "
							     << (char)0xc4 << (char)0x87 << ")\n";    break;
			case 136:   out << "!!!RNB" << ": symbol: $l      = l slash  (UTF-8: "
							     << (char)0xc5 << (char)0x82 << ")\n";    break;
			case 140:   out << "!!!RNB" << ": symbol: &icirc; = i circumflex (UTF-8: "
							     << (char)0xc3 << (char)0xaf << ")\n";    break;
			case 141:   out << "!!!RNB" << ": symbol: $X      = Z acute  (UTF-8: "
							     << (char)0xc5 << (char)0xb9 << ")\n";    break;
			case 142:   out << "!!!RNB" << ": symbol: &auml;  = a umlaut (UTF-8: "
							     << (char)0xc3 << (char)0xa4 << ")\n";    break;
			case 143:   out << "!!!RNB" << ": symbol: $C      = C acute  (UTF-8: "
							     << (char)0xc4 << (char)0x86 << ")\n";    break;
			case 148:   out << "!!!RNB" << ": symbol: &ouml;  = o umlaut (UTF-8: "
							     << (char)0xc3 << (char)0xb6 << ")\n";    break;
			case 151:   out << "!!!RNB" << ": symbol: $S      = S acute  (UTF-8: "
							     << (char)0xc5 << (char)0x9a << ")\n";    break;
			case 152:   out << "!!!RNB" << ": symbol: $s      = s acute  (UTF-8: "
							     << (char)0xc5 << (char)0x9b << ")\n";    break;
			case 156:   out << "!!!RNB" << ": symbol: $s      = s acute  (UTF-8: "
							     << (char)0xc5 << (char)0x9b << ")\n";    break;
			case 157:   out << "!!!RNB" << ": symbol: $L      = L slash  (UTF-8: "
							     << (char)0xc5 << (char)0x81 << ")\n";    break;
			case 159:   out << "!!!RNB" << ": symbol: $vc     = c hachek (UTF-8: "
							     << (char)0xc4 << (char)0x8d << ")\n";    break;
			case 162:   out << "!!!RNB" << ": symbol: &oacute;= o acute  (UTF-8: "
							     << (char)0xc3 << (char)0xb3 << ")\n";    break;
			case 163:   out << "!!!RNB" << ": symbol: &uacute;= u acute  (UTF-8: "
							     << (char)0xc3 << (char)0xba << ")\n";    break;
			case 165:   out << "!!!RNB" << ": symbol: $a      = a hook   (UTF-8: "
							     << (char)0xc4 << (char)0x85 << ")\n";    break;
			case 169:   out << "!!!RNB" << ": symbol: $e      = e hook   (UTF-8: "
							     << (char)0xc4 << (char)0x99 << ")\n";    break;
			case 171:   out << "!!!RNB" << ": symbol: $y      = z acute  (UTF-8: "
							     << (char)0xc5 << (char)0xba << ")\n";    break;
			case 175:   out << "!!!RNB" << ": symbol: $Z      = Z dot    (UTF-8: "
							     << (char)0xc5 << (char)0xbb << ")\n";    break;
			case 179:   out << "!!!RNB" << ": symbol: $l      = l slash  (UTF-8: "
							     << (char)0xc5 << (char)0x82 << ")\n";    break;
			case 185:   out << "!!!RNB" << ": symbol: $a      = a hook   (UTF-8: "
							     << (char)0xc4 << (char)0x85 << ")\n";    break;
			case 189:   out << "!!!RNB" << ": symbol: $Z      = Z dot    (UTF-8: "
							     << (char)0xc5 << (char)0xbb << ")\n";    break;
			case 190:   out << "!!!RNB" << ": symbol: $z      = z dot    (UTF-8: "
							     << (char)0xc5 << (char)0xbc << ")\n";    break;
			case 191:   out << "!!!RNB" << ": symbol: $z      = z dot    (UTF-8: "
							     << (char)0xc5 << (char)0xbc << ")\n";    break;
			case 224:   out << "!!!RNB" << ": symbol: &Oacute;= O acute  (UTF-8: "
							     << (char)0xc3 << (char)0x93 << ")\n";    break;
			case 225:   out << "!!!RNB" << ": symbol: &szlig; = sz ligature (UTF-8: "
							     << (char)0xc3 << (char)0x9f << ")\n";    break;
			case 0xdf:  out << "!!!RNB" << ": symbol: &szlig; = sz ligature (UTF-8: "
							     << (char)0xc3 << (char)0x9f << ")\n";    break;
// Polish version:
//         case 228:   out << "!!!RNB" << ": symbol: $n      = n acute  (UTF-8: "
//                          << (char)0xc5 << (char)0x84 << ")\n";    break;
// Luxembourg version for some reason...:
			case 228:   out << "!!!RNB" << ": symbol: &auml;      = a umlaut  (UTF-8: "
							     << (char)0xc5 << (char)0x84 << ")\n";    break;
			case 230:   out << "!!!RNB" << ": symbol: c       = c\n";           break;
			case 231:   out << "!!!RNB" << ": symbol: $vs     = s hachek (UTF-8: "
							     << (char)0xc5 << (char)0xa1 << ")\n";    break;
			case 234:   out << "!!!RNB" << ": symbol: $e      = e hook   (UTF-8: "
							     << (char)0xc4 << (char)0x99 << ")\n";    break;
			case 241:   out << "!!!RNB" << ": symbol: $n      = n acute  (UTF-8: "
							     << (char)0xc5 << (char)0x84 << ")\n";    break;
			case 243:   out << "!!!RNB" << ": symbol: &oacute;= o acute  (UTF-8: "
							     << (char)0xc3 << (char)0xb3 << ")\n";    break;
			case 252:   out << "!!!RNB" << ": symbol: &uuml;  = u umlaut (UTF-8: "
							     << (char)0xc3 << (char)0xbc << ")\n";    break;
//         default:
		}
		}
		chartable[i] = 0;
	}
}



//////////////////////////////
//
// Tool_esac2hum::printTitleInfo -- print the first line of the CUT[] field.
//

bool Tool_esac2hum::printTitleInfo(vector<string>& song, ostream& out) {
	int start = -1;
	int stop = -1;
	getLineRange(song, "CUT", start, stop);
	if (start == -1) {
		cerr << "Error: cannot find CUT[] field in song: " << song[0] << endl;
		return false;
	}

	string buffer;
	buffer = song[start].substr(4);
	if (buffer.back() == ']') {
		buffer.resize(buffer.size() - 1);
	}

	out << "!!!OTL: ";
	for (int i=0; i<(int)buffer.size(); i++) {
		printChar(buffer[i], out);
	}
	out << "\n";

	return true;
}



//////////////////////////////
//
// Tool_esac2hum::printChar -- print text characters, translating high-bit data
//    if required.
//

void Tool_esac2hum::printChar(unsigned char c, ostream& out) {
	if (c < 128) {
		out << c;
	} else {
		chartable[c]++;
		switch (c) {
			case 129:   out << "&uuml;";    break;
			case 130:   out << "&eacute;";  break;
			case 132:   out << "&auml;";    break;
			case 134:   out << "$c";        break;
			case 136:   out << "$l";        break;
			case 140:   out << "&icirc;";   break;
			case 141:   out << "$X";        break;   // Z acute
			case 142:   out << "&auml;";    break;   // ?
			case 143:   out << "$C";        break;
			case 148:   out << "&ouml;";    break;
			case 151:   out << "$S";        break;
			case 152:   out << "$s";        break;
			case 156:   out << "$s";        break;  // 1250 encoding
			case 157:   out << "$L";        break;
			case 159:   out << "$vc";       break;  // Cech c with v accent
			case 162:   out << "&oacute;";  break;
			case 163:   out << "&uacute;";  break;
			case 165:   out << "$a";        break;
			case 169:   out << "$e";        break;
			case 171:   out << "$y";        break;
			case 175:   out << "$Z";        break;  // 1250 encoding
			case 179:   out << "$l";        break;  // 1250 encoding
			case 185:   out << "$a";        break;  // 1250 encoding
			case 189:   out << "$Z";        break;  // Z dot
			case 190:   out << "$z";        break;  // z dot
			case 191:   out << "$z";        break;  // 1250 encoding
			case 224:   out << "&Oacute;";  break;
			case 225:   out << "&szlig;";   break;
			case 0xdf:  out << "&szlig;";   break;
			// Polish version:
			// case 228:   out << "$n";        break;
			// Luxembourg version (for some reason...)
			case 228:   out << "&auml;";        break;
			case 230:   out << "c";         break;  // ?
			case 231:   out << "$vs";       break;  // Cech s with v accent
			case 234:   out << "$e";        break;  // 1250 encoding
			case 241:   out << "$n";        break;  // 1250 encoding
			case 243:   out << "&oacute;";  break;  // 1250 encoding
			case 252:   out << "&uuml;";    break;
			default:    out << c;
		}
	}
}



//////////////////////////////
//
// Tool_esac2hum::printKeyInfo --
//

void Tool_esac2hum::printKeyInfo(vector<NoteData>& songdata, int tonic, int textQ,
		ostream& out) {
	vector<int> pitches(40, 0);
	int pitchsum = 0;
	int pitchcount = 0;
	int i;
	for (i=0; i<(int)songdata.size(); i++) {
		if (songdata[i].pitch >= 0) {
			pitches[songdata[i].pitch % 40]++;
			pitchsum += Convert::base40ToMidiNoteNumber(songdata[i].pitch);
			pitchcount++;
		}
	}

	// generate a clef, choosing either treble or bass clef depending
	// on the average pitch.
	double averagepitch = pitchsum * 1.0 / pitchcount;
	if (averagepitch > 60.0) {
		out << "*clefG2";
		if (textQ) {
			out << "\t*clefG2";
		}
		out << "\n";
	} else {
		out << "*clefF4";
		if (textQ) {
			out << "\t*clefF4";
		}
		out << "\n";
	}

	// generate a key signature
	vector<int> diatonic(7, 0);
	diatonic[0] = getAccidentalMax(pitches[1], pitches[2], pitches[3]);
	diatonic[1] = getAccidentalMax(pitches[7], pitches[8], pitches[9]);
	diatonic[2] = getAccidentalMax(pitches[13], pitches[14], pitches[15]);
	diatonic[3] = getAccidentalMax(pitches[18], pitches[19], pitches[20]);
	diatonic[4] = getAccidentalMax(pitches[24], pitches[25], pitches[26]);
	diatonic[5] = getAccidentalMax(pitches[30], pitches[31], pitches[32]);
	diatonic[6] = getAccidentalMax(pitches[36], pitches[37], pitches[38]);

	int flatcount = 0;
	int sharpcount = 0;
	int naturalcount = 0;
	for (i=0; i<7; i++) {
		switch (diatonic[i]) {
			case -1:   flatcount++;      break;
			case  0:   naturalcount++;   break;
			case +1:   sharpcount++;     break;
		}
	}

	char kbuf[32] = {0};
	if (naturalcount == 7) {
		// do nothing
	} else if (flatcount > sharpcount) {
		// print a flat key signature
		if (diatonic[6] == -1) strcat(kbuf, "b-"); else goto keysigend;
		if (diatonic[2] == -1) strcat(kbuf, "e-"); else goto keysigend;
		if (diatonic[5] == -1) strcat(kbuf, "a-"); else goto keysigend;
		if (diatonic[1] == -1) strcat(kbuf, "d-"); else goto keysigend;
		if (diatonic[4] == -1) strcat(kbuf, "g-"); else goto keysigend;
		if (diatonic[0] == -1) strcat(kbuf, "c-"); else goto keysigend;
		if (diatonic[3] == -1) strcat(kbuf, "f-"); else goto keysigend;
	} else {
		// print a sharp key signature
		if (diatonic[3] == +1) strcat(kbuf, "f#"); else goto keysigend;
		if (diatonic[0] == +1) strcat(kbuf, "c#"); else goto keysigend;
		if (diatonic[4] == +1) strcat(kbuf, "g#"); else goto keysigend;
		if (diatonic[1] == +1) strcat(kbuf, "d#"); else goto keysigend;
		if (diatonic[5] == +1) strcat(kbuf, "a#"); else goto keysigend;
		if (diatonic[2] == +1) strcat(kbuf, "e#"); else goto keysigend;
		if (diatonic[6] == +1) strcat(kbuf, "b#"); else goto keysigend;
	}

keysigend:
	out << "*k[" << kbuf << "]";
	if (textQ) {
		out << "\t*k[" << kbuf << "]";
	}
	out << "\n";

	// look at the third scale degree above the tonic pitch
	int minor = pitches[(tonic + 40 + 11) % 40];
	int major = pitches[(tonic + 40 + 12) % 40];

	if (minor > major) {
		// minor key (or related mode)
		out  << "*" << Convert::base40ToKern(40 * 4 + tonic) << ":";
		if (textQ) {
			out  << "\t*" << Convert::base40ToKern(40 * 4 + tonic) << ":";
		}
		out << "\n";
	} else {
		// major key (or related mode)
		out  << "*" << Convert::base40ToKern(40 * 3 + tonic) << ":";
		if (textQ) {
			out  << "\t*" << Convert::base40ToKern(40 * 3 + tonic) << ":";
		}
		out << "\n";
	}

}


//////////////////////////////
//
// Tool_esac2hum::getAccidentalMax --
//

int Tool_esac2hum::getAccidentalMax(int a, int b, int c) {
	if (a > b && a > c) {
		return -1;
	} else if (c > a && c > b) {
		return +1;
	} else {
		return 0;
	}
}


//////////////////////////////
//
// Tool_esac2hum::postProcessSongData -- clean up data and do some interpreting.
//

void Tool_esac2hum::postProcessSongData(vector<NoteData>& songdata, vector<int>& numerator,
		vector<int>& denominator) {
	int i, j;
	// move phrase start markers off of rests and onto the
	// first note that it finds
	for (i=0; i<(int)songdata.size()-1; i++) {
		if (songdata[i].pitch < 0 && songdata[i].phstart) {
			songdata[i+1].phstart = songdata[i].phstart;
			songdata[i].phstart = 0;
		}
	}

	// move phrase ending markers off of rests and onto the
	// previous note that it finds
	for (i=(int)songdata.size()-1; i>0; i--) {
		if (songdata[i].pitch < 0 && songdata[i].phend) {
			songdata[i-1].phend = songdata[i].phend;
			songdata[i].phend = 0;
		}
	}

	// examine barline information
	double dur = 0.0;
	for (i=(int)songdata.size()-1; i>=0; i--) {
		if (songdata[i].bar == 1) {
			songdata[i].bardur = dur;
			dur = songdata[i].duration;
		} else {
			dur += songdata[i].duration;
		}
	}

	int barnum = 0;
	double firstdur = 0.0;
	if (numerator.size() == 1 && numerator[0] > 0) {
		// handle single non-frei meter
		songdata[0].num = numerator[0];
		songdata[0].denom = denominator[0];
		dur = 0;
		double meterdur = 4.0 / denominator[0] * numerator[0];
		for (i=0; i<(int)songdata.size(); i++) {
			if (songdata[i].bar) {
				dur = 0.0;
			} else {
				dur += songdata[i].duration;
				if (fabs(dur - meterdur) < 0.001) {
					songdata[i].bar = 1;
					songdata[i].barinterp = 1;
					dur = 0.0;
				}
			}
		}

		// readjust measure beat counts
		dur = 0.0;
		for (i=(int)songdata.size()-1; i>=0; i--) {
			if (songdata[i].bar == 1) {
				songdata[i].bardur = dur;
				dur = songdata[i].duration;
			} else {
				dur += songdata[i].duration;
			}
		}
		firstdur = dur;

		// number the barlines
		barnum = 0;
		if (fabs(firstdur - meterdur) < 0.001) {
			// music for first bar, next bar will be bar 2
			barnum = 2;
		} else {
			barnum = 1;
			// pickup-measure
		}
		for (i=0; i<(int)songdata.size(); i++) {
			if (songdata[i].bar == 1) {
				songdata[i].barnum = barnum++;
			}
		}

	} else if (numerator.size() == 1 && numerator[0] == -1) {
		// handle free meter

		// number the barline
		firstdur = dur;
		barnum = 1;
		for (i=0; i<(int)songdata.size(); i++) {
			if (songdata[i].bar == 1) {
				songdata[i].barnum = barnum++;
			}
		}

	} else {
		// handle multiple time signatures

		// get the duration of each type of meter:
		vector<double> meterdurs;
		meterdurs.resize(numerator.size());
		for (i=0; i<(int)meterdurs.size(); i++) {
			meterdurs[i] = 4.0 / denominator[i] * numerator[i];
		}

		// measure beat counts:
		dur = 0.0;
		for (i=(int)songdata.size()-1; i>=0; i--) {
			if (songdata[i].bar == 1) {
				songdata[i].bardur = dur;
				dur = songdata[i].duration;
			} else {
				dur += songdata[i].duration;
			}
		}
		firstdur = dur;

		// interpret missing barlines
		int currentmeter = 0;
		// find first meter
		for (i=0; i<(int)numerator.size(); i++) {
			if (fabs(firstdur - meterdurs[i]) < 0.001) {
				songdata[0].num = numerator[i];
				songdata[0].denom = denominator[i];
				currentmeter = i;
			}
		}
		// now handle the meters in the rest of the music...
		int fnd = 0;
		dur = 0;
		for (i=0; i<(int)songdata.size()-1; i++) {
			if (songdata[i].bar) {
				if (songdata[i].bardur != meterdurs[currentmeter]) {
					// try to find the correct new meter

					fnd = 0;
					for (j=0; j<(int)numerator.size(); j++) {
						if (j == currentmeter) {
							continue;
						}
						if (fabs(songdata[i].bardur - meterdurs[j]) < 0.001) {
							songdata[i+1].num = numerator[j];
							songdata[i+1].denom = denominator[j];
							currentmeter = j;
							fnd = 1;
						}
					}
					if (!fnd) {
						for (j=0; j<(int)numerator.size(); j++) {
							if (j == currentmeter) {
							   continue;
							}
							if (fabs(songdata[i].bardur/2.0 - meterdurs[j]) < 0.001) {
							   songdata[i+1].num = numerator[j];
							   songdata[i+1].denom = denominator[j];
							   currentmeter = j;
							   fnd = 1;
							}
						}
					}
				}
				dur = 0.0;
			} else {
				dur += songdata[i].duration;
				if (fabs(dur - meterdurs[currentmeter]) < 0.001) {
					songdata[i].bar = 1;
					songdata[i].barinterp = 1;
					dur = 0.0;
				}
			}
		}

		// perhaps sum duration of measures again and search for error here?

		// finally, number the barlines:
		barnum = 1;
		for (i=0; i<(int)numerator.size(); i++) {
			if (fabs(firstdur - meterdurs[i]) < 0.001) {
				barnum = 2;
				break;
			}
		}
		for (i=0; i<(int)songdata.size(); i++) {
			if (songdata[i].bar == 1) {
				songdata[i].barnum = barnum++;
			}
		}


	}

}



//////////////////////////////
//
// Tool_esac2hum::getMeterInfo --
//

void Tool_esac2hum::getMeterInfo(string& meter, vector<int>& numerator,
		vector<int>& denominator) {
	char buffer[256] = {0};
	strcpy(buffer, meter.c_str());
	numerator.resize(0);
	denominator.resize(0);
	int num = -1;
	int denom = -1;
	char* ptr;
	ptr = strtok(buffer, " \t\n");
	while (ptr != NULL) {
		if (strcmp(ptr, "frei") == 0 || strcmp(ptr, "Frei") == 0) {
			num = -1;
			denom = -1;
			numerator.push_back(num);
			denominator.push_back(denom);
		} else {
			if (strchr(ptr, '/') != NULL) {
				num = -1;
				denom = 4;
				sscanf(ptr, "%d/%d", &num, &denom);
				numerator.push_back(num);
				denominator.push_back(denom);
			} else {
				num = atoi(ptr);
				denom = 4;
				numerator.push_back(num);
				denominator.push_back(denom);
			}
		}
		ptr = strtok(NULL, " \t\n");
	}

}



//////////////////////////////
//
// Tool_esac2hum::getLineRange -- get the staring line and ending line of a data
//     field.  Returns -1 if the data field was not found.
//

void Tool_esac2hum::getLineRange(vector<string>& song, const string& field,
		int& start, int& stop) {
	string searchstring = field;;
	searchstring += "[";
	start = stop = -1;
	for (int i=0; i<(int)song.size(); i++) {
		auto loc = song[i].find(']');
		if (song[i].compare(0, searchstring.size(), searchstring) == 0) {
			start = i;
			if (loc != string::npos) {
				stop = i;
				break;
			}
		} else if ((start >= 0) && (loc != string::npos)) {
			stop = i;
			break;
		}
	}
}



//////////////////////////////
//
// Tool_esac2hum::getNoteList -- get a list of the notes and rests and barlines in
//    the MEL field.
//

bool Tool_esac2hum::getNoteList(vector<string>& song, vector<NoteData>& songdata, double mindur,
		int tonic) {
	songdata.resize(0);
	NoteData tempnote;
	int melstart = -1;
	int melstop  = -1;
	int i, j;
	int octave      = 0;
	int degree      = 0;
	int accidental  = 0;
	double duration = mindur;
	int bar    = 0;
	// int tuplet = 0;
	int major[8] = {-1, 0, 6, 12, 17, 23, 29, 35};
	// int oldstate  = -1;
	int state     = -1;
	int nextstate = -1;
	int phend = 0;
	int phnum = 0;
	int phstart = 0;
	int slend = 0;
	int slstart = 0;
	int tie = 0;

	getLineRange(song, "MEL", melstart, melstop);

	for (i=melstart; i<=melstop; i++) {
		if (song[i].size() < 4) {
			cerr << "Error: invalid line in MEL[]: " << song[i] << endl;
			return false;
		}
		j = 4;
		phstart = 1;
		phend = 0;
		// Note Format: (+|-)*[0..7]_*\.*(  )?
		// ONADB
		// Order of data: Octave, Note, Accidental, Duration, Barline

		#define STATE_SLSTART -1
		#define STATE_OCTAVE   0
		#define STATE_NOTE     1
		#define STATE_ACC      2
		#define STATE_DUR      3
		#define STATE_BAR      4
		#define STATE_SLEND    5

		while (j < 200 && (j < (int)song[i].size())) {
			// oldstate = state;
			switch (song[i][j]) {
				// Octave information:
				case '-': octave--; state = STATE_OCTAVE; break;
				case '+': octave++; state = STATE_OCTAVE; break;

				// Duration information:
				case '_': duration *= 2.0; state = STATE_DUR; break;
				case '.': duration *= 1.5; state = STATE_DUR; break;

				// Accidental information:
				case 'b': accidental--; state = STATE_ACC;  break;
				case '#': accidental++; state = STATE_ACC;  break;

				// Note information:
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7':
					degree =  major[song[i][j] - '0'];
					state = STATE_NOTE;
					break;
				case 'O':
					degree =  major[0];
					state = STATE_NOTE;
					break;

				// Barline information:
				case ' ':
					state = STATE_BAR;
					if (song[i][j+1] == ' ') {
						bar = 1;
					}
					break;

				// Other information:
				case '{': slstart = 1;  state = STATE_SLSTART;  break;
				case '}': slend   = 1;  state = STATE_SLEND;    break;
				// case '(': tuplet  = 1;        break;
				// case ')': tuplet  = 0;        break;
				case '/':                     break;
				case ']':                     break;
//            case '>':                     break;   // unknown marker
//            case '<':                     break;   //
				case '^': tie = 1; state = STATE_NOTE; break;
				default : cerr << "Error: unknown character " << song[i][j]
							      << " on the line: " << song[i] << endl;
							 return false;
			}
			j++;
			switch (song[i][j]) {
				case '-': case '+': nextstate = STATE_OCTAVE; break;
				case 'O':
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': nextstate = STATE_NOTE; break;
				case 'b': case '#': nextstate = STATE_ACC;    break;
				case '_': case '.': nextstate = STATE_DUR; break;
				case '{': nextstate = STATE_SLSTART; break;
				case '}': nextstate = STATE_SLEND; break;
				case '^': nextstate = STATE_NOTE; break;
				case ' ':
					 if (song[i][j+1] == ' ') nextstate = STATE_BAR;
					 else if (song[i][j+1] == '/') nextstate = -2;
					 break;
				case '\0':
					phend = 1;
				default: nextstate = -1;
			}

			if (nextstate < state ||
					((nextstate == STATE_NOTE) && (state == nextstate))) {
				 tempnote.clear();
				 if (degree < 0) { // rest
					 tempnote.pitch = -999;
				 } else {
					 tempnote.pitch = degree + 40*(octave + 4) + accidental + tonic;
				 }
				 if (tie) {
					 tempnote.pitch = songdata[(int)songdata.size()-1].pitch;
					 if (songdata[(int)songdata.size()-1].tieend) {
						 songdata[(int)songdata.size()-1].tiecont = 1;
						 songdata[(int)songdata.size()-1].tieend = 0;
					 } else {
						 songdata[(int)songdata.size()-1].tiestart = 1;
					 }
					 tempnote.tieend = 1;
				 }
				 tempnote.duration = duration;
				 tempnote.phend = phend;
				 tempnote.bar = bar;
				 tempnote.phstart = phstart;
				 tempnote.slstart = slstart;
				 tempnote.slend = slend;
				 if (nextstate == -2) {
					 tempnote.bar = 2;
					 tempnote.phend = 1;
				 }
				 tempnote.phnum = phnum;

				 songdata.push_back(tempnote);
				 duration = mindur;
				 degree = 0;
				 bar = 0;
				 tie = 0;
				 phend = 0;
				 phstart = 0;
				 slend = 0;
				 slstart = 0;
				 octave = 0;
				 accidental = 0;
				 if (nextstate == -2) {
					 return true;
				 }
			}
		}
		phnum++;
	}

	return true;
}



//////////////////////////////
//
// Tool_esac2hum::printNoteData --
//

void Tool_esac2hum::printNoteData(NoteData& data, int textQ, ostream& out) {

	if (data.num > 0) {
		out << "*M" << data.num << "/" << data.denom;
		if (textQ) {
			out << "\t*M" << data.num << "/" << data.denom;
		}
		out << "\n";
	}
	if (data.phstart == 1) {
		out << "{";
	}
	if (data.slstart == 1) {
		out << "(";
	}
	if (data.tiestart == 1) {
		out << "[";
	}
	out << Convert::durationFloatToRecip(data.duration);
	if (data.pitch < 0) {
		out << "r";
	} else {
		out << Convert::base40ToKern(data.pitch);
	}
	if (data.tiecont == 1) {
		out << "_";
	}
	if (data.tieend == 1) {
		out << "]";
	}
	if (data.slend == 1) {
		out << ")";
	}
	if (data.phend == 1) {
		out << "}";
	}

	if (textQ) {
		out << "\t";
		if (data.phstart == 1) {
			out << "{";
		}
		if (data.text == "") {
			if (data.pitch < 0) {
				data.text = "%";
			} else {
				data.text = "|";
			}
		}
		if (data.pitch < 0 && (data.text.find('%') == string::npos)) {
			out << "%";
		}
		if (data.text == " *") {
			if (data.pitch < 0) {
				data.text = "%*";
			} else {
				data.text = "|*";
			}
		}
		if (data.text == "^") {
			data.text = "|^";
		}
		printString(data.text, out);
		if (data.phend == 1) {
			out << "}";
		}
	}

	out << "\n";

	// print barline information
	if (data.bar == 1) {

		out << "=";
		if (data.barnum > 0) {
			out << data.barnum;
		}
		if (data.barinterp) {
			// out << "yy";
		}
		if (debugQ) {
			if (data.bardur > 0.0) {
				out << "[" << data.bardur << "]";
			}
		}
		if (textQ) {
			out << "\t";
			out << "=";
			if (data.barnum > 0) {
				out << data.barnum;
			}
			if (data.barinterp) {
				// out << "yy";
			}
			if (debugQ) {
				if (data.bardur > 0.0) {
					out << "[" << data.bardur << "]";
				}
			}
		}

		out << "\n";
	} else if (data.bar == 2) {
		out << "==";
		if (textQ) {
			out << "\t==";
		}
		out << "\n";
	}
}



//////////////////////////////
//
// Tool_esac2hum::getKeyInfo -- look for a KEY[] entry and extract the data.
//
// ggg fix this function
//

bool Tool_esac2hum::getKeyInfo(vector<string>& song, string& key, double& mindur,
		int& tonic, string& meter, ostream& out) {
	int i;
	for (i=0; i<(int)song.size(); i++) {
		if (song[i].compare(0, 4, "KEY[") == 0) {
			key = song[i][4]; // letter
			key += song[i][5]; // number
			key += song[i][6]; // number
			key += song[i][7]; // number
			key += song[i][8]; // number
			if (!isspace(song[i][9])) {
				key += song[i][9];  // optional letter (sometimes ' or ")
			}
			if (!isspace(song[i][10])) {
				key += song[i][10];  // illegal but possible extra letter
			}
			if (song[i][10] != ' ') {
				out << "!! Warning key field is not complete" << endl;
				out << "!!Key field: " << song[i] << endl;
			}

			mindur = (song[i][11] - '0') * 10 + (song[i][12] - '0');
			mindur = 4.0 / mindur;

			string tonicstr;
			if (song[i][14] != ' ') {
				tonicstr[0] = song[i][14];
				if (tolower(song[i][15]) == 'b') {
					tonicstr[1] = '-';
				} else {
					tonicstr[1] = song[i][15];
				}
				tonicstr[2] = '\0';
			} else {
				tonicstr = song[i][15];
			}

			// convert German notation to English for note names
			// Hopefully all references to B will mean English B-flat.
			if (tonicstr == "B") {
				tonicstr = "B-";
			}
			if (tonicstr == "H") {
				tonicstr = "B";
			}

			tonic = Convert::kernToBase40(tonicstr);
			if (tonic <= 0) {
				cerr << "Error: invalid tonic on line: " << song[i] << endl;
				return false;
			}
			tonic = tonic % 40;
			meter = song[i].substr(17);
			if (meter.back() != ']') {
				cerr << "Error with meter on line: " << song[i] << endl;
				cerr << "Meter area: " << meter << endl;
				cerr << "Expected ] as last character but found " << meter.back() << endl;
				return false;
			} else {
				meter.resize(meter.size() - 1);
			}
			return true;
		}
	}
	cerr << "Error: did not find a KEY field" << endl;
	return false;
}



///////////////////////////////
//
// Tool_esac2hum::getFileContents -- read a file into the array.
//

bool Tool_esac2hum::getFileContents(vector<string>& array, const string& filename) {
	ifstream infile(filename.c_str());
	array.reserve(100);
	array.resize(0);

	if (!infile.is_open()) {
		cerr << "Error: cannot open file: " << filename << endl;
		return false;
	}

	char holdbuffer[1024] = {0};

	infile.getline(holdbuffer, 256, '\n');
	while (!infile.eof()) {
		array.push_back(holdbuffer);
		infile.getline(holdbuffer, 256, '\n');
	}

	infile.close();
	return true;
}



//////////////////////////////
//
// Tool_esac2hum::example --
//

void Tool_esac2hum::example(void) {


}



//////////////////////////////
//
// Tool_esac2hum::usage --
//

void Tool_esac2hum::usage(const string& command) {

}



//////////////////////////////
//
// Tool_esac2hum::printBibInfo --
//

void Tool_esac2hum::printBibInfo(vector<string>& song, ostream& out) {
	int i, j;
	char buffer[32] = {0};
	int start = -1;
	int stop  = -1;
	int count = 0;
	string templine;

	for (i=0; i<(int)song.size(); i++) {
		if (song[i] == "") {
			continue;
		}
		if (song[i][0] != ' ') {
			if (song[i].size() < 4 || song[i][3] != '[') {
				if (song[i].compare(0, 2, "!!") != 0) {
					out << "!! " << song[i] << "\n";
				}
				continue;
			}
			strncpy(buffer, song[i].c_str(), 3);
			buffer[3] = '\0';
			if (strcmp(buffer, "MEL") == 0) continue;
			if (strcmp(buffer, "TXT") == 0) continue;
			// if (strcmp(buffer, "KEY") == 0) continue;
			getLineRange(song, buffer, start, stop);

			// don't print CUT field if only one line.  !!!OTL: will contain CUT[]
			// if (strcmp(buffer, "CUT") == 0 && start == stop) continue;

			buffer[0] = tolower(buffer[0]);
			buffer[1] = tolower(buffer[1]);
			buffer[2] = tolower(buffer[2]);

			count = 1;
			templine = "";
			for (j=start; j<=stop; j++) {
				if (song[j].size() < 4) {
					continue;
				}
				if (stop - start == 0) {
					templine = song[j].substr(4);
					auto loc = templine.find(']');
					if (loc != string::npos) {
						templine.resize(loc);
					}
					if (templine != "") {
						out << "!!!" << buffer << ": ";
						printString(templine, out);
						out << "\n";
					}

				} else if (j==start) {
					out << "!!!" << buffer << count++ << ": ";
					printString(song[j].substr(4), out);
					out << "\n";
				} else if (j==stop) {
					templine = song[j].substr(4);
					auto loc = templine.find(']');
					if (loc != string::npos) {
						templine.resize(loc);
					}
					if (templine != "") {
						out << "!!!" << buffer << count++ << ": ";
						printString(templine, out);
						out << "\n";
					}
				} else {
					out << "!!!" << buffer << count++ << ": ";
					printString(&(song[j][4]), out);
					out << "\n";
				}
			}
		}
	}
}



//////////////////////////////
//
// Tool_esac2hum::printString -- print characters in string.
//

void Tool_esac2hum::printString(const string& string, ostream& out) {
	for (int i=0; i<(int)string.size(); i++) {
		printChar(string[i], out);
	}
}





/////////////////////////////////
//
// Tool_extract::Tool_extract -- Set the recognized options for the tool.
//

Tool_extract::Tool_extract(void) {
	define("P|F|S|x|exclude=s:", "Remove listed spines from output");
	define("i=s:", "Exclusive interpretation list to extract from input");
	define("I=s:", "Exclusive interpretation exclusion list");
	define("f|p|s|field|path|spine=s:",
			 "for extraction of particular spines");
	define("C|count=b", "print a count of the number of spines in file");
	define("c|cointerp=s:**kern", "Exclusive interpretation for cospines");
	define("g|grep=s:", "Extract spines which match a given regex.");
	define("r|reverse=b", "reverse order of spines by **kern group");
	define("R=s:**kern", "reverse order of spine by exinterp group");
	define("t|trace=s:", "use a trace file to extract data");
	define("e|expand=b", "expand spines with subspines");
	define("k|kern=s", "Extract by kern spine group");
	define("E|expand-interp=s:", "expand subspines limited to exinterp");
	define("m|model|method=s:d", "method for extracting secondary spines");
	define("M|cospine-model=s:d", "method for extracting cospines");
	define("Y|no-editoral-rests=b",
			"do not display yy marks on interpreted rests");

	define("debug=b", "print debugging information");
	define("author=b");              // author of program
	define("version=b");             // compilation info
	define("example=b");             // example usages
	define("h|help=b");              // short description
}



/////////////////////////////////
//
// Tool_extract::run -- Primary interfaces to the tool.
//

bool Tool_extract::run(const string& indata, ostream& out) {
	HumdrumFile infile(indata);
	bool status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_extract::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}

//
// In-place processing of file:
//

bool Tool_extract::run(HumdrumFile& infile) {
	initialize(infile);
	processFile(infile);
	// Re-load the text for each line from their tokens.
	// infile.createLinesFromTokens();
	return true;
}



//////////////////////////////
//
// Tool_extract::processFile --
//

void Tool_extract::processFile(HumdrumFile& infile) {
	if (countQ) {
		m_free_text << infile.getMaxTrack() << endl;
		return;
	}
	if (expandQ) {
		expandSpines(field, subfield, model, infile, expandInterp);
	} else if (interpQ) {
		getInterpretationFields(field, subfield, model, infile, interps,
				interpstate);
	} else if (reverseQ) {
		reverseSpines(field, subfield, model, infile, reverseInterp);
	} else if (fieldQ || excludeQ) {
		fillFieldData(field, subfield, model, fieldstring, infile);
	} else if (grepQ) {
		fillFieldDataByGrep(field, subfield, model, grepString, infile,
			interpstate);
	}

	if (debugQ && !traceQ) {
		m_free_text << "!! Field Expansion List:";
		for (int j=0; j<(int)field.size(); j++) {
			m_free_text << " " << field[j];
			if (subfield[j]) {
				m_free_text << (char)subfield[j];
			}
			if (model[j]) {
				m_free_text << (char)model[j];
			}
		}
		m_free_text << endl;
	}

	// preserve SEGMENT filename if present (now printed in main())
	// infile.printNonemptySegmentLabel(m_humdrum_text);

	// analyze the input file according to command-line options
	if (fieldQ || grepQ) {
		extractFields(infile, field, subfield, model);
	} else if (excludeQ) {
		excludeFields(infile, field, subfield, model);
	} else if (traceQ) {
		extractTrace(infile, tracefile);
	} else {
		m_humdrum_text << infile;
	}
}



//////////////////////////////
//
// Tool_extract::fillFieldDataByGrep --
//

void Tool_extract::fillFieldDataByGrep(vector<int>& field, vector<int>& subfield,
		vector<int>& model, const string& searchstring, HumdrumFile& infile,
		int state) {

	field.reserve(infile.getMaxTrack()+1);
	subfield.reserve(infile.getMaxTrack()+1);
	model.reserve(infile.getMaxTrack()+1);
	field.resize(0);
	subfield.resize(0);
	model.resize(0);

	vector<int> tracks;
	tracks.resize(infile.getMaxTrack()+1);
	fill(tracks.begin(), tracks.end(), 0);
	HumRegex hre;
	int track;

	int i, j;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isManipulator()) {
			continue;
		}
		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (hre.search(infile.token(i, j), searchstring.c_str(), "")) {
				track = infile[i].token(j)->getTrack();
				tracks[track] = 1;
			}
		}
	}

	int zero = 0;
	for (i=1; i<(int)tracks.size(); i++) {
		if (state != 0) {
			tracks[i] = !tracks[i];
		}
		if (tracks[i]) {
			field.push_back(i);
			subfield.push_back(zero);
			model.push_back(zero);
		}
	}
}



//////////////////////////////
//
// Tool_extract::getInterpretationFields --
//

void Tool_extract::getInterpretationFields(vector<int>& field, vector<int>& subfield,
		vector<int>& model, HumdrumFile& infile, string& interps, int state) {
	vector<string> sstrings; // search strings
	sstrings.reserve(100);
	sstrings.resize(0);

	int i, j, k;
	string buffer;
	buffer = interps;

	HumRegex hre;
	hre.replaceDestructive(buffer, "", "\\s+", "g");

	int start = 0;
	while (hre.search(buffer, start, "^([^,]+)")) {
		sstrings.push_back(hre.getMatch(1));
		start = hre.getMatchEndIndex(1);
	}

	if (debugQ) {
		m_humdrum_text << "!! Interpretation strings to search for: " << endl;
		for (i=0; i<(int)sstrings.size(); i++) {
			m_humdrum_text << "!!\t" << sstrings[i] << endl;
		}
	}

	vector<int> tracks;
	tracks.resize(infile.getMaxTrack()+1);
	fill(tracks.begin(), tracks.end(), 0);

	// Algorithm below could be made more efficient by
	// not searching the entire file...
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isInterpretation()) {
			continue;
		}
		for (j=0; j<infile[i].getFieldCount(); j++) {
			for (k=0; k<(int)sstrings.size(); k++) {
				if (sstrings[k] == *infile.token(i, j)) {
					tracks[infile[i].token(j)->getTrack()] = 1;
				}
			}
		}
	}

	field.reserve(tracks.size());
	subfield.reserve(tracks.size());
	model.reserve(tracks.size());

	field.resize(0);
	subfield.resize(0);
	model.resize(0);

	int zero = 0;
	for (i=1; i<(int)tracks.size(); i++) {
		if (state == 0) {
			tracks[i] = !tracks[i];
		}
		if (tracks[i]) {
			field.push_back(i);
			subfield.push_back(zero);
			model.push_back(zero);
		}
	}

}



//////////////////////////////
//
// Tool_extract::expandSpines --
//

void Tool_extract::expandSpines(vector<int>& field, vector<int>& subfield, vector<int>& model,
		HumdrumFile& infile, string& interp) {

	vector<int> splits;
	splits.resize(infile.getMaxTrack()+1);
	fill(splits.begin(), splits.end(), 0);

	int i, j;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isManipulator()) {
			continue;
		}

		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (strchr(infile.token(i, j)->getSpineInfo().c_str(), '(') != NULL) {
				splits[infile[i].token(j)->getTrack()] = 1;
			}
		}
	}

	field.reserve(infile.getMaxTrack()*2);
	field.resize(0);

	subfield.reserve(infile.getMaxTrack()*2);
	subfield.resize(0);

	model.reserve(infile.getMaxTrack()*2);
	model.resize(0);

	int allQ = 0;
	if (interp.size() == 0) {
		allQ = 1;
	}

	// ggg
	vector<int> dummyfield;
	vector<int> dummysubfield;
	vector<int> dummymodel;
	getInterpretationFields(dummyfield, dummysubfield, model, infile, interp, 1);

	vector<int> interptracks;

	interptracks.resize(infile.getMaxTrack()+1);
	fill(interptracks.begin(), interptracks.end(), 0);

	for (i=0; i<(int)dummyfield.size(); i++) {
		interptracks[dummyfield[i]] = 1;
	}

	int aval = 'a';
	int bval = 'b';
	int zero = 0;
	for (i=1; i<(int)splits.size(); i++) {
		if (splits[i] && (allQ || interptracks[i])) {
			field.push_back(i);
			subfield.push_back(aval);
			model.push_back(zero);
			field.push_back(i);
			subfield.push_back(bval);
			model.push_back(zero);
		} else {
			field.push_back(i);
			subfield.push_back(zero);
			model.push_back(zero);
		}
	}

	if (debugQ) {
		m_humdrum_text << "!!expand: ";
		for (i=0; i<(int)field.size(); i++) {
			m_humdrum_text << field[i];
			if (subfield[i]) {
				m_humdrum_text << (char)subfield[i];
			}
			if (i < (int)field.size()-1) {
				m_humdrum_text << ",";
			}
		}
		m_humdrum_text << endl;
	}
}



//////////////////////////////
//
// Tool_extract::reverseSpines -- reverse the order of spines, grouped by the
//   given exclusive interpretation.
//

void Tool_extract::reverseSpines(vector<int>& field, vector<int>& subfield, vector<int>& model,
		HumdrumFile& infile, const string& exinterp) {

	vector<int> target;
	target.resize(infile.getMaxTrack()+1);
	fill(target.begin(), target.end(), 0);

	vector<HTp> trackstarts;
	infile.getSpineStartList(trackstarts);

	for (int t=1; t<=(int)trackstarts.size(); t++) {
		if (trackstarts[t]->isDataType(exinterp)) {
			target[t] = 1;
		}
	}

	field.reserve(infile.getMaxTrack()*2);
	field.resize(0);

	int i, j;
	int lasti = (int)target.size();
	for (i=(int)target.size()-1; i>0; i--) {
		if (target[i]) {
			lasti = i;
			field.push_back(i);
			for (j=i+1; j<(int)target.size(); j++) {
				if (!target[j]) {
					field.push_back(j);
				} else {
					break;
				}
			}
		}
	}

	// if the grouping spine is not first, then preserve the
	// locations of the pre-spines.
	int extras = 0;
	if (lasti != 1) {
		extras = lasti - 1;
		field.resize(field.size()+extras);
		for (i=0; i<(int)field.size()-extras; i++) {
			field[(int)field.size()-1-i] = field[(int)field.size()-1-extras-i];
		}
		for (i=0; i<extras; i++) {
			field[i] = i+1;
		}
	}

	if (debugQ) {
		m_humdrum_text << "!!reverse: ";
		for (i=0; i<(int)field.size(); i++) {
			m_humdrum_text << field[i] << " ";
		}
		m_humdrum_text << endl;
	}

	subfield.resize(field.size());
	fill(subfield.begin(), subfield.end(), 0);

	model.resize(field.size());
	fill(model.begin(), model.end(), 0);

}



//////////////////////////////
//
// Tool_extract::fillFieldData --
//

void Tool_extract::fillFieldData(vector<int>& field, vector<int>& subfield,
		vector<int>& model, string& fieldstring, HumdrumFile& infile) {

	int maxtrack = infile.getMaxTrack();

	field.reserve(maxtrack);
	field.resize(0);

	subfield.reserve(maxtrack);
	subfield.resize(0);

	model.reserve(maxtrack);
	model.resize(0);

	HumRegex hre;
	string buffer = fieldstring;
	hre.replaceDestructive(buffer, "", "\\s", "gs");
	int start = 0;
	string tempstr;
	while (hre.search(buffer,  start, "^([^,]+,?)")) {
		processFieldEntry(field, subfield, model, hre.getMatch(1), infile);
		start += hre.getMatchEndIndex(1);
	}

}



//////////////////////////////
//
// Tool_extract::processFieldEntry --
//   3-6 expands to 3 4 5 6
//   $   expands to maximum spine track
//   $-1 expands to maximum spine track minus 1, etc.
//

void Tool_extract::processFieldEntry(vector<int>& field,
		vector<int>& subfield, vector<int>& model, const string& astring,
		HumdrumFile& infile) {

	int maxtrack = infile.getMaxTrack();

	vector<HTp> ktracks;
	infile.getKernSpineStartList(ktracks);
	if (kernQ) {
		maxtrack = (int)ktracks.size();
	}

	int modletter;
	int subletter;

	HumRegex hre;
	string buffer = astring;

	// remove any comma left at end of input astring (or anywhere else)
	hre.replaceDestructive(buffer, "", ",", "g");

	// first remove $ symbols and replace with the correct values
	removeDollarsFromString(buffer, maxtrack);

	int zero = 0;
	if (hre.search(buffer, "^(\\d+)-(\\d+)$")) {
		int firstone = hre.getMatchInt(1);
		int lastone  = hre.getMatchInt(2);

		if ((firstone < 1) && (firstone != 0)) {
			m_error_text << "Error: range token: \"" << astring << "\""
				  << " contains too small a number at start: " << firstone << endl;
			m_error_text << "Minimum number allowed is " << 1 << endl;
			return;
		}
		if ((lastone < 1) && (lastone != 0)) {
			m_error_text << "Error: range token: \"" << astring << "\""
				  << " contains too small a number at end: " << lastone << endl;
			m_error_text << "Minimum number allowed is " << 1 << endl;
			return;
		}
		if (firstone > maxtrack) {
			m_error_text << "Error: range token: \"" << astring << "\""
				  << " contains number too large at start: " << firstone << endl;
			m_error_text << "Maximum number allowed is " << maxtrack << endl;
			return;
		}
		if (lastone > maxtrack) {
			m_error_text << "Error: range token: \"" << astring << "\""
				  << " contains number too large at end: " << lastone << endl;
			m_error_text << "Maximum number allowed is " << maxtrack << endl;
			return;
		}

		if (firstone > lastone) {
			for (int i=firstone; i>=lastone; i--) {
				field.push_back(i);
				subfield.push_back(zero);
				model.push_back(zero);
			}
		} else {
			for (int i=firstone; i<=lastone; i++) {
				field.push_back(i);
				subfield.push_back(zero);
				model.push_back(zero);
			}
		}
	} else if (hre.search(buffer, "^(\\d+)([a-z]*)")) {
		int value = hre.getMatchInt(1);
		modletter = 0;
		subletter = 0;
		if (hre.getMatch(2) ==  "a") {
			subletter = 'a';
		}
		if (hre.getMatch(2) ==  "b") {
			subletter = 'b';
		}
		if (hre.getMatch(2) ==  "c") {
			subletter = 'c';
		}
		if (hre.getMatch(2) ==  "d") {
			modletter = 'd';
		}
		if (hre.getMatch(2) ==  "n") {
			modletter = 'n';
		}
		if (hre.getMatch(2) ==  "r") {
			modletter = 'r';
		}

		if ((value < 1) && (value != 0)) {
			m_error_text << "Error: range token: \"" << astring << "\""
				  << " contains too small a number at end: " << value << endl;
			m_error_text << "Minimum number allowed is " << 1 << endl;
			return;
		}
		if (value > maxtrack) {
			m_error_text << "Error: range token: \"" << astring << "\""
				  << " contains number too large at start: " << value << endl;
			m_error_text << "Maximum number allowed is " << maxtrack << endl;
			return;
		}
		field.push_back(value);
		if (value == 0) {
			subfield.push_back(zero);
			model.push_back(zero);
		} else {
			subfield.push_back(subletter);
			model.push_back(modletter);
		}
	}

	if (!kernQ) {
		return;
	}

	maxtrack = infile.getMaxTrack();

	// Fields to next kern track.
	// Needs some more bug fixing
	// Most just needs a fix to preserve old field data
	// For secondary passes into thi function.
	vector<int> newfield;
	vector<int> newsubfield;
	vector<int> newmodel;
	int i, j;

	for (i=0; i<(int)field.size(); i++) {
		if (field[i] != 0) {
		   field[i] = ktracks[field[i] - 1]->getTrack();
		}
	}

	vector<HTp> trackstarts;
	infile.getTrackStartList(trackstarts);

	int start, stop;
	for (i=0; i<(int)field.size(); i++) {
		newfield.push_back(field[i]);
		newsubfield.push_back(subfield[i]);
		newmodel.push_back(model[i]);
		start = field[i] + 1;
		if (i < (int)field.size()-1) {
		   stop = field[i+1];
		} else {
			stop = maxtrack+1;
		}
		for (j=start; j<stop; j++) {
			if (trackstarts[j-1]->isKern()) {
				break;
			}
			newfield.push_back(j);
			newsubfield.push_back(zero);
			newmodel.push_back(zero);
		}
	}

	field    = newfield;
	subfield = newsubfield;
	model    = newmodel;
}



//////////////////////////////
//
// Tool_extract::removeDollarsFromString -- substitute $ sign for maximum track count.
//

void Tool_extract::removeDollarsFromString(string& buffer, int maxtrack) {
	HumRegex hre;
	char buf2[128] = {0};
	int value2;

	if (hre.search(buffer, "\\$$")) {
		sprintf(buf2, "%d", maxtrack);
		hre.replaceDestructive(buffer, buf2, "\\$$");
	}

	if (hre.search(buffer, "\\$(?![\\d-])")) {
		// don't know how this case could happen, however...
		sprintf(buf2, "%d", maxtrack);
		hre.replaceDestructive(buffer, buf2, "\\$(?![\\d-])", "g");
	}

	if (hre.search(buffer, "\\$0")) {
		// replace $0 with maxtrack (used for reverse orderings)
		sprintf(buf2, "%d", maxtrack);
		hre.replaceDestructive(buffer, buf2, "\\$0", "g");
	}

	while (hre.search(buffer, "\\$(-?\\d+)")) {
		value2 = maxtrack - abs(hre.getMatchInt(1));
		sprintf(buf2, "%d", value2);
		hre.replaceDestructive(buffer, buf2, "\\$-?\\d+");
	}
}



//////////////////////////////
//
// Tool_extract::excludeFields -- print all spines except the ones in the list of fields.
//

void Tool_extract::excludeFields(HumdrumFile& infile, vector<int>& field,
		vector<int>& subfield, vector<int>& model) {
	int start = 0;
	for (int i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].hasSpines()) {
			m_humdrum_text << infile[i] << '\n';
			continue;
		} else {
			start = 0;
			for (int j=0; j<infile[i].getFieldCount(); j++) {
				if (isInList(infile[i].token(j)->getTrack(), field)) {
					continue;
				}
				if (start != 0) {
					m_humdrum_text << '\t';
				}
				start = 1;
				m_humdrum_text << infile.token(i, j);
			}
			if (start != 0) {
				m_humdrum_text << endl;
			}
		}
	}
}



//////////////////////////////
//
// Tool_extract::extractFields -- print all spines in the list of fields.
//

void Tool_extract::extractFields(HumdrumFile& infile, vector<int>& field,
		vector<int>& subfield, vector<int>& model) {

	HumRegex hre;
	int start = 0;
	int target;
	int subtarget;
	int modeltarget;
	string spat;

	for (int i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].hasSpines()) {
			m_humdrum_text << infile[i] << '\n';
			continue;
		}

		if (infile[i].isManipulator()) {
			dealWithSpineManipulators(infile, i, field, subfield, model);
			continue;
		}

		start = 0;
		for (int t=0; t<(int)field.size(); t++) {
			target = field[t];
			subtarget = subfield[t];
			modeltarget = model[t];
			if (modeltarget == 0) {
				switch (subtarget) {
					 case 'a':
					 case 'b':
					    modeltarget = submodel;
					    break;
					 case 'c':
					    modeltarget = comodel;
				}
			}
			if (target == 0) {
				if (start != 0) {
					 m_humdrum_text << '\t';
				}
				start = 1;
				if (!infile[i].isManipulator()) {
					if (infile[i].isLocalComment()) {
						m_humdrum_text << "!";
					} else if (infile[i].isBarline()) {
						m_humdrum_text << infile[i][0];
					} else if (infile[i].isData()) {
				       m_humdrum_text << ".";
					    // interpretations handled in dealWithSpineManipulators()
					    // [obviously not, so adding a blank one here
					} else if (infile[i].isInterpretation()) {
					    m_humdrum_text << "*";
					 }
				}
			} else {
				for (int j=0; j<infile[i].getFieldCount(); j++) {
					 if (infile[i].token(j)->getTrack() != target) {
					    continue;
					 }
					 switch (subtarget) {
					 case 'a':
					    getSearchPat(spat, target, "a");
					    if (hre.search(infile.token(i,j)->getSpineInfo(), spat) ||
					          !hre.search(infile.token(i, j)->getSpineInfo(), "\\(")) {
					       if (start != 0) {
					          m_humdrum_text << '\t';
					       }
					       start = 1;
					       m_humdrum_text << infile.token(i, j);
					    }
					    break;
					 case 'b':
					    getSearchPat(spat, target, "b");
					    if (hre.search(infile.token(i, j)->getSpineInfo(), spat)) {
					       if (start != 0) {
					          m_humdrum_text << '\t';
					       }
					       start = 1;
					       m_humdrum_text << infile.token(i, j);
					    } else if (!hre.search(infile.token(i, j)->getSpineInfo(),
					          "\\(")) {
					       if (start != 0) {
					          m_humdrum_text << '\t';
					       }
					       start = 1;
					       dealWithSecondarySubspine(field, subfield, model, t,
					             infile, i, j, modeltarget);
					    }
					    break;
					 case 'c':
					    if (start != 0) {
					       m_humdrum_text << '\t';
					    }
					    start = 1;
					    dealWithCospine(field, subfield, model, t, infile, i, j,
					       modeltarget, modeltarget, cointerp);
					    break;
					 default:
					    if (start != 0) {
					       m_humdrum_text << '\t';
					    }
					    start = 1;
					    m_humdrum_text << infile.token(i, j);
					 }
				}
			}
		}
		if (start != 0) {
			m_humdrum_text << endl;
		}
	}
}



//////////////////////////////
//
// Tool_extract::dealWithCospine -- extract the required token(s) from a co-spine.
//

void Tool_extract::dealWithCospine(vector<int>& field, vector<int>& subfield, vector<int>& model,
		int targetindex, HumdrumFile& infile, int line, int cospine,
		int comodel, int submodel, const string& cointerp) {

	vector<string> cotokens;
	cotokens.reserve(50);

	string buffer;
	int i, j, k;
	int index;

	if (infile[line].isInterpretation()) {
		m_humdrum_text << infile.token(line, cospine);
		return;
	}

	if (infile[line].isBarline()) {
		m_humdrum_text << infile.token(line, cospine);
		return;
	}

	if (infile[line].isLocalComment()) {
		m_humdrum_text << infile.token(line, cospine);
		return;
	}

	int count = infile[line].token(cospine)->getSubtokenCount();
	for (k=0; k<count; k++) {
		buffer = infile.token(line, cospine)->getSubtoken(k);
		cotokens.resize(cotokens.size()+1);
		index = (int)cotokens.size()-1;
		cotokens[index] = buffer;
	}

	vector<int> spineindex;
	vector<int> subspineindex;

	spineindex.reserve(infile.getMaxTrack()*2);
	spineindex.resize(0);

	subspineindex.reserve(infile.getMaxTrack()*2);
	subspineindex.resize(0);

	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (infile.token(line, j)->isDataType(cointerp)) {
			continue;
		}
		if (*infile.token(line, j) == ".") {
			continue;
		}
		count = infile[line].token(j)->getSubtokenCount();
		for (k=0; k<count; k++) {
			buffer = infile[line].token(j)->getSubtoken(k);
			if (comodel == 'r') {
				if (buffer == "r") {
					continue;
				}
			}
			spineindex.push_back(j);
			subspineindex.push_back(k);
		}
	}

	if (debugQ) {
		m_humdrum_text << "\n!!codata:\n";
		for (i=0; i<(int)cotokens.size(); i++) {
			m_humdrum_text << "!!\t" << i << "\t" << cotokens[i];
			if (i < (int)spineindex.size()) {
				m_humdrum_text << "\tspine=" << spineindex[i];
				m_humdrum_text << "\tsubspine=" << subspineindex[i];
			} else {
				m_humdrum_text << "\tspine=.";
				m_humdrum_text << "\tsubspine=.";
			}
			m_humdrum_text << endl;
		}
	}

	string buff;

	int start = 0;
	for (i=0; i<(int)field.size(); i++) {
		if (infile.token(line, field[i])->isDataType(cointerp)) {
			continue;
		}

		for (j=0; j<infile[line].getFieldCount(); j++) {
			if (infile[line].token(j)->getTrack() != field[i]) {
				continue;
			}
			if (subfield[i] == 'a') {
				getSearchPat(buff, field[i], "a");
				if ((strchr(infile.token(line, j)->getSpineInfo().c_str(), '(') == NULL) ||
					(infile.token(line, j)->getSpineInfo().find(buff) != string::npos)) {
					printCotokenInfo(start, infile, line, j, cotokens, spineindex,
							 subspineindex);
				}
			} else if (subfield[i] == 'b') {
				// this section may need more work...
				getSearchPat(buff, field[i], "b");
				if ((strchr(infile.token(line, j)->getSpineInfo().c_str(), '(') == NULL) ||
					(strstr(infile.token(line, j)->getSpineInfo().c_str(), buff.c_str()) != NULL)) {
					printCotokenInfo(start, infile, line, j, cotokens, spineindex,
							 subspineindex);
				}
			} else {
				printCotokenInfo(start, infile, line, j, cotokens, spineindex,
					subspineindex);
			}
		}
	}
}



//////////////////////////////
//
// Tool_extract::printCotokenInfo --
//

void Tool_extract::printCotokenInfo(int& start, HumdrumFile& infile, int line, int spine,
		vector<string>& cotokens, vector<int>& spineindex,
		vector<int>& subspineindex) {
	int i;
	int found = 0;
	for (i=0; i<(int)spineindex.size(); i++) {
		if (spineindex[i] == spine) {
			if (start == 0) {
				start++;
			} else {
				m_humdrum_text << subtokenseparator;
			}
			if (i<(int)cotokens.size()) {
				m_humdrum_text << cotokens[i];
			} else {
				m_humdrum_text << ".";
			}
		found = 1;
		}
	}
	if (!found) {
		if (start == 0) {
			start++;
		} else {
			m_humdrum_text << subtokenseparator;
		}
		m_humdrum_text << ".";
	}
}



//////////////////////////////
//
// Tool_extract::dealWithSecondarySubspine -- what to print if a secondary spine
//     does not exist on a line.
//

void Tool_extract::dealWithSecondarySubspine(vector<int>& field, vector<int>& subfield,
		vector<int>& model, int targetindex, HumdrumFile& infile, int line,
		int spine, int submodel) {

	int& i = line;
	int& j = spine;

	HumRegex hre;
	string buffer;
	if (infile[line].isLocalComment()) {
		if ((submodel == 'n') || (submodel == 'r')) {
			m_humdrum_text << "!";
		} else {
			m_humdrum_text << infile.token(i, j);
		}
	} else if (infile[line].isBarline()) {
		m_humdrum_text << infile.token(i, j);
	} else if (infile[line].isInterpretation()) {
		if ((submodel == 'n') || (submodel == 'r')) {
			m_humdrum_text << "*";
		} else {
			m_humdrum_text << infile.token(i, j);
		}
	} else if (infile[line].isData()) {
		if (submodel == 'n') {
			m_humdrum_text << ".";
		} else if (submodel == 'r') {
			if (*infile.token(i, j) == ".") {
				m_humdrum_text << ".";
			} else if (infile.token(i, j)->find('q') != string::npos) {
				m_humdrum_text << ".";
			} else if (infile.token(i, j)->find('Q') != string::npos) {
				m_humdrum_text << ".";
			} else {
				buffer = *infile.token(i, j);
				if (hre.search(buffer, "{")) {
					m_humdrum_text << "{";
				}
				// remove secondary chord notes:
				hre.replaceDestructive(buffer, "", " .*");
				// remove unnecessary characters (such as stem direction):
				hre.replaceDestructive(buffer, "", 
						"[^}pPqQA-Ga-g0-9.;%#nr-]", "g");
				// change pitch to rest:
				hre.replaceDestructive(buffer, "[A-Ga-g#n-]+", "r");
				// add editorial marking unless -Y option is given:
				if (editorialInterpretation != "") {
					if (hre.search(buffer, "rr")) {
						 hre.replaceDestructive(buffer, editorialInterpretation, "(?<=rr)");
						 hre.replaceDestructive(buffer, "r", "rr");
					} else {
						 hre.replaceDestructive(buffer, editorialInterpretation, "(?<=r)");
					}
				}
				m_humdrum_text << buffer;
			}
		} else {
			m_humdrum_text << infile.token(i, j);
		}
	} else {
		m_error_text << "Should not get to this line of code" << endl;
		return;
	}
}




//////////////////////////////
//
// Tool_extract::getSearchPat --
//

void Tool_extract::getSearchPat(string& spat, int target, const string& modifier) {
	if (modifier.size() > 20) {
		m_error_text << "Error in GetSearchPat" << endl;
		return;
	}
	spat.reserve(16);
	spat = "\\(";
	spat += to_string(target);
	spat += "\\)";
	spat += modifier;
}



//////////////////////////////
//
// Tool_extract::dealWithSpineManipulators -- check for proper Humdrum syntax of
//     spine manipulators (**, *-, *x, *v, *^) when creating the output.
//

void Tool_extract::dealWithSpineManipulators(HumdrumFile& infile, int line,
		vector<int>& field, vector<int>& subfield, vector<int>& model) {

	vector<int> vmanip;  // counter for *v records on line
	vmanip.resize(infile[line].getFieldCount());
	fill(vmanip.begin(), vmanip.end(), 0);

	vector<int> xmanip; // counter for *x record on line
	xmanip.resize(infile[line].getFieldCount());
	fill(xmanip.begin(), xmanip.end(), 0);

	int i = 0;
	int j;
	for (j=0; j<(int)vmanip.size(); j++) {
		if (*infile.token(line, j) == "*v") {
			vmanip[j] = 1;
		}
		if (*infile.token(line, j) == "*x") {
			xmanip[j] = 1;
		}
	}

	int counter = 1;
	for (i=1; i<(int)xmanip.size(); i++) {
		if ((xmanip[i] == 1) && (xmanip[i-1] == 1)) {
			xmanip[i] = counter;
			xmanip[i-1] = counter;
			counter++;
		}
	}

	counter = 1;
	i = 0;
	while (i < (int)vmanip.size()) {
		if (vmanip[i] == 1) {
			while ((i < (int)vmanip.size()) && (vmanip[i] == 1)) {
				vmanip[i] = counter;
				i++;
			}
			counter++;
		}
		i++;
	}

	vector<int> fieldoccur;  // nth occurance of an input spine in the output
	fieldoccur.resize(field.size());
	fill(fieldoccur.begin(), fieldoccur.end(), 0);

	vector<int> trackcounter; // counter of input spines occurances in output
	trackcounter.resize(infile.getMaxTrack()+1);
	fill(trackcounter.begin(), trackcounter.end(), 0);

	for (i=0; i<(int)field.size(); i++) {
		if (field[i] != 0) {
			trackcounter[field[i]]++;
			fieldoccur[i] = trackcounter[field[i]];
		}
	}

	vector<string> tempout;
	vector<int> vserial;
	vector<int> xserial;
	vector<int> fpos;     // input column of output spine

	tempout.reserve(1000);
	tempout.resize(0);

	vserial.reserve(1000);
	vserial.resize(0);

	xserial.reserve(1000);
	xserial.resize(0);

	fpos.reserve(1000);
	fpos.resize(0);

	string spat;
	string spinepat;
	HumRegex hre;
	int subtarget;
	int modeltarget;
	int xdebug = 0;
	int vdebug = 0;
	int suppress = 0;
	int target;
	int tval;
	for (int t=0; t<(int)field.size(); t++) {
		target = field[t];
		subtarget = subfield[t];
		modeltarget = model[t];
		if (modeltarget == 0) {
			switch (subtarget) {
				case 'a':
				case 'b':
					modeltarget = submodel;
					break;
				case 'c':
					modeltarget = comodel;
			}
		}
		suppress = 0;
		if (target == 0) {
			if (infile.token(line, 0)->compare(0, 2, "**") == 0) {
				storeToken(tempout, "**blank");
				tval = 0;
				vserial.push_back(tval);
				xserial.push_back(tval);
				fpos.push_back(tval);
			} else if (*infile.token(line, 0) == "*-") {
				storeToken(tempout, "*-");
				tval = 0;
				vserial.push_back(tval);
				xserial.push_back(tval);
				fpos.push_back(tval);
			} else {
				storeToken(tempout, "*");
				tval = 0;
				vserial.push_back(tval);
				xserial.push_back(tval);
				fpos.push_back(tval);
			}
		} else {
			for (j=0; j<infile[line].getFieldCount(); j++) {
				if (infile[line].token(j)->getTrack() != target) {
					continue;
				}
		 // filter by subfield
		 if (subtarget == 'a') {
					getSearchPat(spat, target, "b");
			 if (hre.search(infile.token(line, j)->getSpineInfo(), spat)) {
						continue;
			 }
		 } else if (subtarget == 'b') {
					getSearchPat(spat, target, "a");
			 if (hre.search(infile.token(line, j)->getSpineInfo(), spat)) {
						continue;
					}
				}

				switch (subtarget) {
				case 'a':

					if (!hre.search(infile.token(line, j)->getSpineInfo(), "\\(")) {
						if (*infile.token(line, j)  == "*^") {
							 storeToken(tempout, "*");
						} else {
							 storeToken(tempout, *infile.token(line, j));
						}
					} else {
						getSearchPat(spat, target, "a");
						spinepat =  infile.token(line, j)->getSpineInfo();
						hre.replaceDestructive(spinepat, "\\(", "\\(", "g");
						hre.replaceDestructive(spinepat, "\\)", "\\)", "g");

						if ((*infile.token(line, j) == "*v") &&
							    (spinepat == spat)) {
							 storeToken(tempout, "*");
						} else {
							 getSearchPat(spat, target, "b");
							 if ((spinepat == spat) &&
							       (*infile.token(line, j) ==  "*v")) {
							    // do nothing
							    suppress = 1;
							 } else {
							    storeToken(tempout, *infile.token(line, j));
							 }
						}
					}

					break;
				case 'b':

					if (!hre.search(infile.token(line, j)->getSpineInfo(), "\\(")) {
						if (*infile.token(line, j) == "*^") {
							 storeToken(tempout, "*");
						} else {
							 storeToken(tempout, *infile.token(line, j));
						}
					} else {
						getSearchPat(spat, target, "b");
						spinepat = infile.token(line, j)->getSpineInfo();
						hre.replaceDestructive(spinepat, "\\(", "\\(", "g");
						hre.replaceDestructive(spinepat, "\\)", "\\)", "g");

						if ((*infile.token(line, j) ==  "*v") &&
							    (spinepat == spat)) {
							 storeToken(tempout, "*");
						} else {
							 getSearchPat(spat, target, "a");
							 if ((spinepat == spat) &&
							       (*infile.token(line, j) == "*v")) {
							    // do nothing
							    suppress = 1;
							 } else {
							    storeToken(tempout, *infile.token(line, j));
							 }
						}
					}

					break;
				case 'c':
					// work on later
					storeToken(tempout, *infile.token(line, j));
					break;
				default:
					storeToken(tempout, *infile.token(line, j));
				}

				if (suppress) {
					continue;
				}

				if (tempout[(int)tempout.size()-1] == "*x") {
					tval = fieldoccur[t] * 1000 + xmanip[j];
					xserial.push_back(tval);
					xdebug = 1;
				} else {
					tval = 0;
					xserial.push_back(tval);
				}

				if (tempout[(int)tempout.size()-1] == "*v") {
					tval = fieldoccur[t] * 1000 + vmanip[j];
					vserial.push_back(tval);
					vdebug = 1;
				} else {
					tval = 0;
					vserial.push_back(tval);
				}

				fpos.push_back(j);

			}
		}
	}

	if (debugQ && xdebug) {
		m_humdrum_text << "!! *x serials = ";
		for (int ii=0; ii<(int)xserial.size(); ii++) {
			m_humdrum_text << xserial[ii] << " ";
		}
		m_humdrum_text << "\n";
	}

	if (debugQ && vdebug) {
		m_humdrum_text << "!!LINE: " << infile[line] << endl;
		m_humdrum_text << "!! *v serials = ";
		for (int ii=0; ii<(int)vserial.size(); ii++) {
			m_humdrum_text << vserial[ii] << " ";
		}
		m_humdrum_text << "\n";
	}

	// check for proper *x syntax /////////////////////////////////
	for (i=0; i<(int)xserial.size()-1; i++) {
		if (!xserial[i]) {
			continue;
		}
		if (xserial[i] != xserial[i+1]) {
			if (tempout[i] == "*x") {
				xserial[i] = 0;
				tempout[i] = "*";
			}
		} else {
			i++;
		}
	}

	if ((tempout.size() == 1) || (xserial.size() == 1)) {
		// get rid of *x if there is only one spine in output
		if (xserial[0]) {
			xserial[0] = 0;
			tempout[0] = "*";
		}
	} else if ((int)xserial.size() > 1) {
		// check the last item in the list
		int index = (int)xserial.size()-1;
		if (tempout[index] == "*x") {
			if (xserial[index] != xserial[index-1]) {
				xserial[index] = 0;
				tempout[index] = "*";
			}
		}
	}

	// check for proper *v syntax /////////////////////////////////
	vector<int> vsplit;
	vsplit.resize((int)vserial.size());
	fill(vsplit.begin(), vsplit.end(), 0);

	// identify necessary line splits
	for (i=0; i<(int)vserial.size()-1; i++) {
		if (!vserial[i]) {
			continue;
		}
		while ((i<(int)vserial.size()-1) && (vserial[i]==vserial[i+1])) {
			i++;
		}
		if ((i<(int)vserial.size()-1) && vserial[i]) {
			if (vserial.size() > 1) {
				if (vserial[i+1]) {
					vsplit[i+1] = 1;
				}
			}
		}
	}

	// remove single *v spines:

	for (i=0; i<(int)vsplit.size()-1; i++) {
		if (vsplit[i] && vsplit[i+1]) {
			if (tempout[i] == "*v") {
				tempout[i] = "*";
				vsplit[i] = 0;
			}
		}
	}

	if (debugQ) {
		m_humdrum_text << "!!vsplit array: ";
		for (i=0; i<(int)vsplit.size(); i++) {
			m_humdrum_text << " " << vsplit[i];
		}
		m_humdrum_text << endl;
	}

	if (vsplit.size() > 0) {
		if (vsplit[(int)vsplit.size()-1]) {
			if (tempout[(int)tempout.size()-1] == "*v") {
				tempout[(int)tempout.size()-1] = "*";
				vsplit[(int)vsplit.size()-1] = 0;
			}
		}
	}

	int vcount = 0;
	for (i=0; i<(int)vsplit.size(); i++) {
		vcount += vsplit[i];
	}

	if (vcount) {
		printMultiLines(vsplit, vserial, tempout);
	}

	int start = 0;
	for (i=0; i<(int)tempout.size(); i++) {
		if (tempout[i] != "") {
			if (start != 0) {
				m_humdrum_text << "\t";
			}
			m_humdrum_text << tempout[i];
			start++;
		}
	}
	if (start) {
		m_humdrum_text << '\n';
	}
}



//////////////////////////////
//
// Tool_extract::printMultiLines -- print separate *v lines.
//

void Tool_extract::printMultiLines(vector<int>& vsplit, vector<int>& vserial,
		vector<string>& tempout) {
	int i;

	int splitpoint = -1;
	for (i=0; i<(int)vsplit.size(); i++) {
		if (vsplit[i]) {
			splitpoint = i;
			break;
		}
	}

	if (debugQ) {
		m_humdrum_text << "!!tempout: ";
		for (i=0; i<(int)tempout.size(); i++) {
			m_humdrum_text << tempout[i] << " ";
		}
		m_humdrum_text << endl;
	}

	if (splitpoint == -1) {
		return;
	}

	int start = 0;
	int printv = 0;
	for (i=0; i<splitpoint; i++) {
		if (tempout[i] != "") {
			if (start) {
				m_humdrum_text << "\t";
			}
			m_humdrum_text << tempout[i];
			start = 1;
			if (tempout[i] == "*v") {
				if (printv) {
					tempout[i] = "";
				} else {
					tempout[i] = "*";
					printv = 1;
				}
			} else {
				tempout[i] = "*";
			}
		}
	}

	for (i=splitpoint; i<(int)vsplit.size(); i++) {
		if (tempout[i] != "") {
			if (start) {
				m_humdrum_text << "\t";
			}
			m_humdrum_text << "*";
		}
	}

	if (start) {
		m_humdrum_text << "\n";
	}

	vsplit[splitpoint] = 0;

	printMultiLines(vsplit, vserial, tempout);
}



//////////////////////////////
//
// Tool_extract::storeToken --
//

void Tool_extract::storeToken(vector<string>& storage, const string& string) {
	storage.push_back(string);
}

void storeToken(vector<string>& storage, int index, const string& string) {
	storage[index] = string;
}



//////////////////////////////
//
// Tool_extract::isInList -- returns true if first number found in list of numbers.
//     returns the matching index plus one.
//

int Tool_extract::isInList(int number, vector<int>& listofnum) {
	int i;
	for (i=0; i<(int)listofnum.size(); i++) {
		if (listofnum[i] == number) {
			return i+1;
		}
	}
	return 0;

}



//////////////////////////////
//
// Tool_extract::getTraceData --
//

void Tool_extract::getTraceData(vector<int>& startline, vector<vector<int> >& fields,
		const string& tracefile, HumdrumFile& infile) {
	char buffer[1024] = {0};
	HumRegex hre;
	int linenum;
	startline.reserve(10000);
	startline.resize(0);
	fields.reserve(10000);
	fields.resize(0);

	ifstream input;
	input.open(tracefile.c_str());
	if (!input.is_open()) {
		m_error_text << "Error: cannot open file for reading: " << tracefile << endl;
		return;
	}

	string temps;
	vector<int> field;
	vector<int> subfield;
	vector<int> model;

	input.getline(buffer, 1024);
	while (!input.eof()) {
		if (hre.search(buffer, "^\\s*$")) {
			continue;
		}
		if (!hre.search(buffer, "(\\d+)")) {
			continue;
		}
		linenum = hre.getMatchInt(1);
		linenum--;  // adjust so that line 0 is the first line in the file
		temps = buffer;
		hre.replaceDestructive(temps, "", "\\d+");
		hre.replaceDestructive(temps, "", "[^,\\s\\d\\$\\-].*");  // remove any possible comments
		hre.replaceDestructive(temps, "", "\\s", "g");
		if (hre.search(temps, "^\\s*$")) {
			// no field data to process online
			continue;
		}
		startline.push_back(linenum);
		string ttemp = temps;
		fillFieldData(field, subfield, model, ttemp, infile);
		fields.push_back(field);
		input.getline(buffer, 1024);
	}

}



//////////////////////////////
//
// Tool_extract::extractTrace --
//

void Tool_extract::extractTrace(HumdrumFile& infile, const string& tracefile) {
	vector<int> startline;
	vector<vector<int> > fields;
	getTraceData(startline, fields, tracefile, infile);
	int i, j;

	if (debugQ) {
		for (i=0; i<(int)startline.size(); i++) {
			m_humdrum_text << "!!TRACE " << startline[i]+1 << ":\t";
			for (j=0; j<(int)fields[i].size(); j++) {
				m_humdrum_text << fields[i][j] << " ";
			}
			m_humdrum_text << "\n";
		}
	}


	if (startline.size() == 0) {
		for (i=0; i<infile.getLineCount(); i++) {
			if (!infile[i].hasSpines()) {
				m_humdrum_text << infile[i] << '\n';
			}
		}
		return;
	}

	for (i=0; i<startline[0]; i++) {
		if (!infile[i].hasSpines()) {
			m_humdrum_text << infile[i] << '\n';
		}
	}

	int endline;
	for (j=0; j<(int)startline.size(); j++) {
		if (j == (int)startline.size()-1) {
			endline = infile.getLineCount()-1;
		} else {
			endline = startline[j+1]-1;
		}
		for (i=startline[j]; i<endline; i++) {
			if (!infile[i].hasSpines()) {
				m_humdrum_text << infile[i] << '\n';
			} else {
				printTraceLine(infile, i, fields[j]);
			}
		}
	}
}



//////////////////////////////
//
// Tool_extract::printTraceLine --
//

void Tool_extract::printTraceLine(HumdrumFile& infile, int line, vector<int>& field) {
	int j;
	int t;
	int start = 0;
	int target;

	start = 0;
	for (t=0; t<(int)field.size(); t++) {
		target = field[t];
		for (j=0; j<infile[line].getFieldCount(); j++) {
			if (infile[line].token(j)->getTrack() != target) {
				continue;
			}
			if (start != 0) {
				m_humdrum_text << '\t';
			}
			start = 1;
			m_humdrum_text << infile.token(line, j);
		}
	}
	if (start != 0) {
		m_humdrum_text << endl;
	}
}



//////////////////////////////
//
// Tool_extract::example -- example usage of the sonority program
//

void Tool_extract::example(void) {
	m_free_text <<
	"					                                                          \n"
	<< endl;
}



//////////////////////////////
//
// Tool_extract::usage -- gives the usage statement for the sonority program
//

void Tool_extract::usage(const string& command) {
	m_free_text <<
	"					                                                          \n"
	<< endl;
}



//////////////////////////////
//
// Tool_extract::initialize --
//

void Tool_extract::initialize(HumdrumFile& infile) {
	// handle basic options:
	if (getBoolean("author")) {
		m_free_text << "Written by Craig Stuart Sapp, "
			  << "craig@ccrma.stanford.edu, Feb 2008" << endl;
		return;
	} else if (getBoolean("version")) {
		m_free_text << getArg(0) << ", version: Feb 2008" << endl;
		m_free_text << "compiled: " << __DATE__ << endl;
		return;
	} else if (getBoolean("help")) {
		usage(getCommand().c_str());
		return;
	} else if (getBoolean("example")) {
		example();
		return;
	}

	excludeQ    = getBoolean("x");
	interpQ     = getBoolean("i");
	interps     = getString("i");
	kernQ       = getBoolean("k");

	interpstate = 1;
	if (!interpQ) {
		interpQ = getBoolean("I");
		interpstate = 0;
		interps = getString("I");
	}
	if (interps.size() > 0) {
		if (interps[0] != '*') {
			// Automatically add ** if not given on exclusive interpretation
			string tstring = "**";
			interps = tstring + interps;
		}
	}

	fieldQ      = getBoolean("f");
	debugQ      = getBoolean("debug");
	countQ      = getBoolean("count");
	traceQ      = getBoolean("trace");
	tracefile   = getString("trace");
	reverseQ    = getBoolean("reverse");
	expandQ     = getBoolean("expand") || getBoolean("E");
	submodel    = getString("model").c_str()[0];
	cointerp    = getString("cointerp");
	comodel     = getString("cospine-model").c_str()[0];

	if (getBoolean("no-editoral-rests")) {
		editorialInterpretation = "";
	}

	if (interpQ) {
		fieldQ = 1;
	}

	if (expandQ) {
		fieldQ = 1;
		expandInterp = getString("expand-interp");
	}

	if (!reverseQ) {
		reverseQ = getBoolean("R");
		if (reverseQ) {
			reverseInterp = getString("R");
		}
	}

	if (reverseQ) {
		fieldQ = 1;
	}

	if (excludeQ) {
		fieldstring = getString("x");
	} else if (fieldQ) {
		fieldstring = getString("f");
	} else if (kernQ) {
		fieldstring = getString("k");
		fieldQ = 1;
	}

	grepQ = getBoolean("grep");
	grepString = getString("grep");

}




#define RUNTOOL(NAME, INFILE, COMMAND, STATUS)     \
	Tool_##NAME *tool = new Tool_##NAME;            \
	tool->process(COMMAND);                         \
	tool->run(INFILE);                              \
	if (tool->hasError()) {                         \
		status = false;                              \
		tool->getError(cerr);                        \
		delete tool;                                 \
		break;                                       \
	} else if (tool->hasHumdrumText()) {            \
		INFILE.readString(tool->getHumdrumText());   \
	}                                               \
	delete tool;


////////////////////////////////
//
// Tool_filter::Tool_filter -- Set the recognized options for the tool.
//

Tool_filter::Tool_filter(void) {
	define("debug=b", "print debug statement");
}



/////////////////////////////////
//
// Tool_filter::run -- Primary interfaces to the tool.
//

bool Tool_filter::run(const string& indata, ostream& out) {
	HumdrumFile infile(indata);
	bool status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_filter::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}

//
// In-place processing of file:
//

bool Tool_filter::run(HumdrumFile& infile) {
	initialize(infile);

	bool status = true;
	vector<pair<string, string> > commands;
	getCommandList(commands, infile);
	for (int i=0; i<(int)commands.size(); i++) {
		if (commands[i].first == "autobeam") {
			RUNTOOL(autobeam, infile, commands[i].second, status);
		} else if (commands[i].first == "autostem") {
			RUNTOOL(autostem, infile, commands[i].second, status);
		} else if (commands[i].first == "cint") {
			RUNTOOL(cint, infile, commands[i].second, status);
		} else if (commands[i].first == "dissonant") {
			RUNTOOL(dissonant, infile, commands[i].second, status);
		} else if (commands[i].first == "imitation") {
			RUNTOOL(imitation, infile, commands[i].second, status);
		} else if (commands[i].first == "extract") {
			RUNTOOL(extract, infile, commands[i].second, status);
		} else if (commands[i].first == "metlev") {
			RUNTOOL(metlev, infile, commands[i].second, status);
		} else if (commands[i].first == "satb2gs") {
			RUNTOOL(satb2gs, infile, commands[i].second, status);
		} else if (commands[i].first == "recip") {
			RUNTOOL(recip, infile, commands[i].second, status);
		} else if (commands[i].first == "transpose") {
			RUNTOOL(transpose, infile, commands[i].second, status);
		} else if (commands[i].first == "myank") {
			RUNTOOL(myank, infile, commands[i].second, status);
		}
	}

	removeFilterLines(infile);

	// Re-load the text for each line from their tokens in case any
	// updates are needed from token changes.
	infile.createLinesFromTokens();
	return status;
}



//////////////////////////////
//
// Tool_filter::removeFilterLines --
//

void Tool_filter::removeFilterLines(HumdrumFile& infile) {
	HumRegex hre;
	string text;
	for (int i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isReference()) {
			continue;
		}
		if (infile.token(i, 0)->compare(0, 10, "!!!filter:") == 0) { 
			text = infile.token(i, 0)->getText();
			hre.replaceDestructive(text, "!!!Xfilter:", "^!!!filter:");
			infile.token(i, 0)->setText(text);
		}
	}
}



//////////////////////////////
//
// Tool_filter::getCommandList --
//

void Tool_filter::getCommandList(vector<pair<string, string> >& commands,
		HumdrumFile& infile) {

	vector<HumdrumLine*> refs = infile.getReferenceRecords();
	pair<string, string> entry;
	string tag = "filter";
	vector<string> clist;
	HumRegex hre;
   if (m_variant.size() > 0) {
		tag += "-";
		tag += m_variant;
	}
	for (int i=0; i<(int)refs.size(); i++) {
		if (refs[i]->getReferenceKey() != tag) {
			continue;
		}
		string command = refs[i]->getReferenceValue();
		hre.split(clist, command, "\\s*\\|\\s*");
		for (int j=0; j<(int)clist.size(); j++) {
			if (hre.search(clist[j], "^\\s*([^\\s]+)")) {
				entry.first  = hre.getMatch(1);
				entry.second = clist[j];
				commands.push_back(entry);
			}
		}
	}
}



//////////////////////////////
//
// Tool_filter::initialize -- extract time signature lines for
//    each **kern spine in file.
//

void Tool_filter::initialize(HumdrumFile& infile) {
	m_debugQ = getBoolean("debug");
}





int Tool_imitation::Enumerator = 0;


/////////////////////////////////
//
// Tool_imitation::Tool_imitation -- Set the recognized options for the tool.
//

Tool_imitation::Tool_imitation(void) {
	define("debug=b",             "print grid cell information");
	define("e|exinterp=s:**vvdata","specify exinterp for **vvdata spine");
	define("n|threshold=i:7",     "minimum number of notes to match");
	define("D|no-duration=b",     "do not consider duration when matching");
	define("r|rest=b",            "require match trigger to follow a rest");
	define("R|rest2=b",           "require match target to also follow a rest");
	define("M|no-mark=b",         "do not mark matched sequences");
}



/////////////////////////////////
//
// Tool_imitation::run -- Do the main work of the tool.
//

bool Tool_imitation::run(const string& indata, ostream& out) {

	HumdrumFile infile(indata);
	bool status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_imitation::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_imitation::run(HumdrumFile& infile) {
	Enumerator = 0;

	NoteGrid grid(infile);

	if (getBoolean("debug")) {
		grid.printGridInfo(cerr);
		// return 1;
	} 

	m_threshold = getInteger("threshold") + 1;
	if (m_threshold < 3) {
		m_threshold = 3;
	}

	m_duration = !getBoolean("no-duration");
	m_mark     = !getBoolean("no-mark");
	m_rest     = getBoolean("rest");
	m_rest2    = getBoolean("rest2");

	vector<vector<string>>    results;
	vector<vector<NoteCell*>> attacks;
	vector<vector<double>>    intervals;

	doAnalysis(results, grid, attacks, intervals, infile, getBoolean("debug"));

	string exinterp = getString("exinterp");
	vector<HTp> kernspines = infile.getKernSpineStartList();
	infile.appendDataSpine(results.back(), "", exinterp);
	for (int i = (int)results.size()-1; i>0; i--) {
		int track = kernspines[i]->getTrack();
		infile.insertDataSpineBefore(track, results[i-1], "", exinterp);
	}
	infile.createLinesFromTokens();
	if (m_mark && Enumerator) {
		string rdfline = "!!!RDF**kern: ";
		rdfline += m_marker;
		rdfline += " = marked note (color=\"chocolate\")";
		infile.appendLine(rdfline);
	}
	return true;
}



//////////////////////////////
//
// Tool_imitation::doAnalysis -- do a basic melodic analysis of all parts.
//

void Tool_imitation::doAnalysis(vector<vector<string> >& results,
		NoteGrid& grid, vector<vector<NoteCell*> >& attacks,
		vector<vector<double>>& intervals, HumdrumFile& infile,
		bool debug) {

	results.resize(grid.getVoiceCount());
	for (int i=0; i<(int)results.size(); i++) {
		results[i].resize(infile.getLineCount());
	}

	attacks.resize(grid.getVoiceCount());
	for (int i=0; i<(int)attacks.size(); i++) {
		grid.getNoteAndRestAttacks(attacks[i], i);
	}

	intervals.resize(grid.getVoiceCount());
	for (int i=0; i<(int)intervals.size(); i++) {
		intervals[i].resize(attacks[i].size());
		getIntervals(intervals[i], attacks[i]);
	}


	for (int i=0; i<(int)attacks.size(); i++) {
		for (int j=i+1; j<(int)attacks.size(); j++) {
			analyzeImitation(results, attacks, intervals, i, j);
		}
	}
}



///////////////////////////////
//
// Tool_imitation::getIntervals --
//

void Tool_imitation::getIntervals(vector<double>& intervals,
		vector<NoteCell*>& attacks) {
	for (int i=0; i<(int)attacks.size() - 1; i++) {
		intervals[i] = *attacks[i+1] - *attacks[i];
	}
	intervals.back() = NAN;

	if (getBoolean("debug")) {
		cout << endl;
		for (int i=0; i<(int)intervals.size(); i++) {
			cout << "INTERVAL " << i << "\t=\t" << intervals[i] << "\tATK " << attacks[i]->getSgnDiatonicPitch() << "\t" << attacks[i]->getToken() << endl;
		}
	}

}



//////////////////////////////
//
// Tool_imitation::analyzeImitation -- do imitation analysis between two voices.
//

void Tool_imitation::analyzeImitation(vector<vector<string>>& results,
		vector<vector<NoteCell*>>& attacks, vector<vector<double>>& intervals,
		int v1, int v2) {

	vector<NoteCell*>& v1a = attacks[v1];
	vector<NoteCell*>& v2a = attacks[v2];
	vector<double>& v1i = intervals[v1];
	vector<double>& v2i = intervals[v2];

	int min = m_threshold - 1;
	int count;

	vector<int> enum1(v1a.size(), 0);
	vector<int> enum2(v1a.size(), 0);

	for (int i=0; i<(int)v1i.size() - 1; i++) {
		for (int j=0; j<(int)v2i.size() - 1; j++) {
			if (m_rest || m_rest2) {
				if ((i > 0) && (!Convert::isNaN(attacks[v1][i-1]->getSgnDiatonicPitch()))) {
					// match initiator must be preceded by a rest (or start of music)
					continue;
				}
			}
			if (m_rest2) {
				if ((j > 0) && (!Convert::isNaN(attacks[v2][j-1]->getSgnDiatonicPitch()))) {
					// match target must be preceded by a rest (or start of music)
					continue;
				}
			}
			if ((enum1[i] != 0) && (enum1[i] == enum2[j])) {
				// avoid re-matching an existing match as a submatch
				continue;
			}
			count = compareSequences(v1a, v1i, i, v2a, v2i, j);
			if (count >= min) {
				Enumerator++;
				for (int k=0; k<count; k++) {
					enum1[i+k] = Enumerator;
					enum2[j+k] = Enumerator;
				}
				// cout << "Match length count " << count << endl;
				HTp token1 = attacks[v1][i]->getToken();
				HTp token2 = attacks[v2][j]->getToken();
				HumNum time1 = token1->getDurationFromStart();
				HumNum time2 = token2->getDurationFromStart();
				HumNum distance1 = time2 - time1;
				HumNum distance2 = time1 - time2;

				int interval = *attacks[v2][j] - *attacks[v1][i];
				int line1 = attacks[v1][i]->getLineIndex();
				int line2 = attacks[v2][j]->getLineIndex();
				if (!results[v1][line1].empty()) {
					results[v1][line1] += " ";
				}
				results[v1][line1] += "n";
				results[v1][line1] += to_string(Enumerator);
				results[v1][line1] += ":c";
				results[v1][line1] += to_string(count);
				results[v1][line1] += ":d";
				results[v1][line1] += to_string(distance1.getNumerator());
				if (distance1.getDenominator() != 1) {
					results[v1][line1] += '/';
					results[v1][line1] += to_string(distance1.getNumerator());
				}
				results[v1][line1] += ":i";
				results[v1][line1] += to_string(interval + 1);

				if (!results[v2][line2].empty()) {
					results[v2][line2] += " ";
				}
				results[v2][line2] += "n";
				results[v2][line2] += to_string(Enumerator);
				results[v2][line2] += ":c";
				results[v2][line2] += to_string(count);
				results[v2][line2] += ":d";
				results[v2][line2] += to_string(distance2.getNumerator());
				if (distance2.getDenominator() != 1) {
					results[v2][line2] += '/';
					results[v2][line2] += to_string(distance2.getNumerator());
				}
				results[v2][line2] += ":i";
				results[v2][line2] += to_string(interval + 1);

				if (m_mark) {
					for (int z=0; z<count; z++) {
						token1 = attacks[v1][i+z]->getToken();
						token2 = attacks[v2][j+z]->getToken();
						token1->setText(*token1 + m_marker);
						token2->setText(*token2 + m_marker);
					}
				}

			}
			// skip over match (need to do in i as well somehow)
			j += count;
		} // j loop
	} // i loop
}



///////////////////////////////
//
// Tool_imitation::compareSequences --
//

int Tool_imitation::compareSequences(vector<NoteCell*>& attack1,
		vector<double>& seq1, int i1, vector<NoteCell*>& attack2,
		vector<double>& seq2, int i2) {
	int count = 0;
	// sequences cannot start with rests
	if (Convert::isNaN(seq1[i1]) || Convert::isNaN(seq2[i2])) {
		return count;
	}

	HumNum dur1;
	HumNum dur2;

	while ((i1+count < (int)seq1.size()) && (i2+count < (int)seq2.size())) {

		if (m_duration) {
			dur1 = attack1[i1+count]->getDuration();
			dur2 = attack2[i2+count]->getDuration();
			if (dur1 != dur2) {
				break;
			}
		}
		
		if (Convert::isNaN(seq1[i1+count])) {
			if (Convert::isNaN(seq2[i2+count])) {
				count++;
				continue;
			} else {
				break;
			}
		} else if (Convert::isNaN(seq2[i2+count])) {
			break;
		} else if (seq1[i1+count] == seq2[i2+count]) {
			count++;
			continue;
		} else {
			break;
		}
	}

	return count + 1;
}




/////////////////////////////////
//
// Tool_gridtest::Tool_metlev -- Set the recognized options for the tool.
//

Tool_metlev::Tool_metlev(void) {
	define("a|append=b",          "append data analysis to input file");
	define("p|prepend=b",         "prepend data analysis to input file");
	define("c|composite=b",       "generate composite rhythm");
	define("i|integer=b",         "quantize metric levels to int values");
	define("x|attacks-only=b",    "only mark lines with note attacks");
	define("G|no-grace-notes=b",  "do not mark grace note lines");
	define("k|kern-spine=i:1",    "analyze only given kern spine");
	define("e|exinterp=s:blev",   "exclusive interpretation type for output");
}



///////////////////////////////
//
// Tool_metlev::run -- Primary interfaces to the tool.
//

bool Tool_metlev::run(const string& indata, ostream& out) {
	HumdrumFile infile(indata);
	return run(infile, out);
}


bool Tool_metlev::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);
	out << infile;
	return status;
}


bool Tool_metlev::run(HumdrumFile& infile) {
	int lineCount = infile.getLineCount();
	if (lineCount == 0) {
		m_error_text << "No input data";
		return false;
	}

	string exinterp = getString("exinterp");
	if (exinterp.empty()) {
		exinterp = "**blev";
	} else if (exinterp[0] != '*') {
		exinterp.insert(0, "*");
	}
	if (exinterp[1] != '*') {
		exinterp.insert(0, "*");
	}

	m_kernspines = infile.getKernSpineStartList();

	vector<double> beatlev(lineCount, NAN);
	int track = 0;
	if (m_kernspines.size() > 0) {
		track = m_kernspines[0]->getTrack();
	} else {
		m_error_text << "No **kern spines in input file" << endl;
		return false;
	}
	infile.getMetricLevels(beatlev, track, NAN);

	for (int i=0; i<lineCount; i++) {
		if (!infile[i].isData()) {
				continue;
		}
		if (getBoolean("no-grace-notes") && (infile[i].getDuration() == 0)) {
			beatlev[i] = NAN;
			continue;
		}
		if (getBoolean("attacks-only")) {
			if (!infile[i].getKernNoteAttacks()) {
				beatlev[i] = NAN;
				continue;
			}
		}
		if (beatlev[i] - (int)beatlev[i] != 0.0) {
			if (getBoolean("integer")) {
					beatlev[i] = floor(beatlev[i]);
			} else {
				beatlev[i] = Convert::significantDigits(beatlev[i], 2);
			}
		}
	}

	if (getBoolean("kern-spine")) {
		int kspine = getInteger("kern-spine") - 1;
		if ((kspine >= 0) && (kspine < (int)m_kernspines.size())) {
			vector<vector<double> > results;
			fillVoiceResults(results, infile, beatlev);
			if (kspine == (int)m_kernspines.size() - 1) {
				infile.appendDataSpine(results.back(), "nan", exinterp);
			} else {
				int track = m_kernspines[kspine+1]->getTrack();
				infile.insertDataSpineBefore(track, results[kspine],
						"nan", exinterp);
			}
			infile.createLinesFromTokens();
			return true;
		}
	} else if (getBoolean("append")) {
		infile.appendDataSpine(beatlev, "nan", exinterp);
		infile.createLinesFromTokens();
		return true;
	} else if (getBoolean("prepend")) {
		infile.prependDataSpine(beatlev, "nan", exinterp);
		infile.createLinesFromTokens();
		return true;
	} else if (getBoolean("composite")) {
		infile.prependDataSpine(beatlev, "nan", exinterp);
		infile.printFieldIndex(0, m_humdrum_text);
		infile.clear();
		infile.readString(m_humdrum_text.str());
	} else {
		vector<vector<double> > results;
		fillVoiceResults(results, infile, beatlev);
		infile.appendDataSpine(results.back(), "nan", exinterp);
		for (int i = (int)results.size()-1; i>0; i--) {
			int track = m_kernspines[i]->getTrack();
			infile.insertDataSpineBefore(track, results[i-1], "nan", exinterp);
		}
		infile.createLinesFromTokens();
		return true;
	}

	return false;
}



//////////////////////////////
//
// Tool_metlev::fillVoiceResults -- Split the metric level analysis into values
//     for each voice.
//

void Tool_metlev::fillVoiceResults(vector<vector<double> >& results,
		HumdrumFile& infile, vector<double>& beatlev) {

	results.resize(m_kernspines.size());
	for (int i=0; i<(int)results.size(); i++) {
		results[i].resize(beatlev.size());
		fill(results[i].begin(), results[i].end(), NAN);
	}
	int track;
	vector<int> rtracks(infile.getTrackCount() + 1, -1);
	for (int i=0; i<(int)m_kernspines.size(); i++) {
		int track = m_kernspines[i]->getTrack();
		rtracks[track] = i;
	}

	bool attacksQ = getBoolean("attacks-only");
	vector<int> nonnullcount(m_kernspines.size(), 0);
	vector<int> attackcount(m_kernspines.size(), 0);
	HTp token;
	int voice;
	int i, j;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		for (j=0; j<infile[i].getFieldCount(); j++) {
			token = infile.token(i, j);
			if (!token->isKern()) {
				continue;
			}
			if (token->isNull()) {
				continue;
			}
			track = token->getTrack();
			voice = rtracks[track];
			nonnullcount[voice]++;
			if (token->isNoteAttack()) {
				attackcount[voice]++;
			}
		}
		for (int v=0; v<(int)m_kernspines.size(); v++) {
			if (attacksQ) {
				if (attackcount[v]) {
					results[v][i] = beatlev[i];
					attackcount[v] = 0;
				}
			} else {
				if (nonnullcount[v]) {
					results[v][i] = beatlev[i];
				}
				nonnullcount[v] = 0;
			}
		}
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::Tool_musicxml2hum --
//

Tool_musicxml2hum::Tool_musicxml2hum(void) {
	// Options& options = m_options;
	// options.define("k|kern=b","display corresponding **kern data");

	define("r|recip=b", "output **recip spine");
	define("s|stems=b", "include stems in output");

	VoiceDebugQ = false;
	DebugQ = false;

}



//////////////////////////////
//
// Tool_musicxml2hum::convert -- Convert a MusicXML file into
//     Humdrum content.
//

bool Tool_musicxml2hum::convertFile(ostream& out, const char* filename) {
	xml_document doc;
	auto result = doc.load_file(filename);
	if (!result) {
		cerr << "\nXML file [" << filename << "] has syntax errors\n";
		cerr << "Error description:\t" << result.description() << "\n";
		cerr << "Error offset:\t" << result.offset << "\n\n";
		exit(1);
	}

	return convert(out, doc);
}


bool Tool_musicxml2hum::convert(ostream& out, istream& input) {
	string s(istreambuf_iterator<char>(input), {});
	return convert(out, s.c_str());
}


bool Tool_musicxml2hum::convert(ostream& out, const char* input) {
	xml_document doc;
	auto result = doc.load(input);
	if (!result) {
		cout << "\nXML content has syntax errors\n";
		cout << "Error description:\t" << result.description() << "\n";
		cout << "Error offset:\t" << result.offset << "\n\n";
		exit(1);
	}

	return convert(out, doc);
}



bool Tool_musicxml2hum::convert(ostream& out, xml_document& doc) {

	initialize();

	bool status = true; // for keeping track of problems in conversion process.

	vector<string> partids;            // list of part IDs
	map<string, xml_node> partinfo;    // mapping if IDs to score-part elements
	map<string, xml_node> partcontent; // mapping of IDs to part elements

	getPartInfo(partinfo, partids, doc);
	getPartContent(partcontent, partids, doc);
	vector<MxmlPart> partdata;
	partdata.resize(partids.size());
	fillPartData(partdata, partids, partinfo, partcontent);

	// for debugging:
	//printPartInfo(partids, partinfo, partcontent, partdata);

	// check the voice info
	for (int i=0; i<(int)partdata.size(); i++) {
		partdata[i].prepareVoiceMapping();
		// for debugging:
		if (VoiceDebugQ) {
			partdata[i].printStaffVoiceInfo();
		}
	}

	// re-index voices to disallow empty intermediate voices.
	reindexVoices(partdata);

	HumGrid outdata;
	if (m_recipQ) {
		outdata.enableRecipSpine();
	}
	status &= stitchParts(outdata, partids, partinfo, partcontent, partdata);

	outdata.removeRedundantClefChanges();

	// tranfer verse counts from parts/staves to HumGrid:
	// should also do part verse counts here (-1 staffindex).
	int versecount;
	for (int p=0; p<(int)partdata.size(); p++) {
		for (int s=0; s<partdata[p].getStaffCount(); s++) {
			versecount = partdata[p].getVerseCount(s);
			outdata.setVerseCount(p, s, versecount);
		}
	}

	// transfer harmony counts from parts to HumGrid:
	for (int p=0; p<(int)partdata.size(); p++) {
		int harmonyCount = partdata[p].getHarmonyCount();
		outdata.setHarmonyCount(p, harmonyCount);
	}

	// set the duration of the last slice

	HumdrumFile outfile;
	outdata.transferTokens(outfile);

	for (int i=0; i<outfile.getLineCount(); i++) {
		outfile[i].createLineFromTokens();
	}
	out << outfile;

	// add RDFs
	if (m_slurabove) {
		out << "!!!RDF**kern: > = slur above" << endl;
	}
	if (m_slurbelow) {
		out << "!!!RDF**kern: > = slur below" << endl;
	}

	for (int i=0; i<(int)partdata.size(); i++) {
		if (partdata[i].hasEditorialAccidental()) {
			out << "!!!RDF**kern: i = editorial accidental" << endl;
			break;
		}
	}

	return status;
}



//////////////////////////////
//
// initialize --
//

void Tool_musicxml2hum::initialize(void) {
	m_recipQ = getBoolean("recip");
	m_stemsQ = getBoolean("stems");
}



//////////////////////////////
//
// Tool_musicxml2hum::reindexVoices --
//

void Tool_musicxml2hum::reindexVoices(vector<MxmlPart>& partdata) {
	for (int p=0; p<(int)partdata.size(); p++) {
		for (int m=0; m<(int)partdata[p].getMeasureCount(); m++) {
			MxmlMeasure* measure = partdata[p].getMeasure(m);
			if (!measure) {
				continue;
			}
			reindexMeasure(measure);
		}
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::reindexMeasure --
//

void Tool_musicxml2hum::reindexMeasure(MxmlMeasure* measure) {
	if (!measure) {
		return;
	}

	vector<vector<int> > staffVoiceCounts;
	vector<MxmlEvent*>& elist = measure->getEventList();

	for (int i=0; i<(int)elist.size(); i++) {
		int staff = elist[i]->getStaffIndex();
		int voice = elist[i]->getVoiceIndex();

		if ((voice >= 0) && (staff >= 0)) {
			if (staff >= (int)staffVoiceCounts.size()) {
				int newsize = staff + 1;
				staffVoiceCounts.resize(newsize);
			}
			if (voice >= (int)staffVoiceCounts[staff].size()) {
				int oldsize = (int)staffVoiceCounts[staff].size();
				int newsize = voice + 1;
				staffVoiceCounts[staff].resize(newsize);
				for (int i=oldsize; i<newsize; i++) {
					staffVoiceCounts[staff][voice] = 0;
				}
			}
			staffVoiceCounts[staff][voice]++;
		}
	}

	bool needreindexing = false;

	for (int i=0; i<(int)staffVoiceCounts.size(); i++) {
		if (staffVoiceCounts[i].size() < 2) {
			continue;
		}
		for (int j=1; j<(int)staffVoiceCounts[i].size(); j++) {
			if (staffVoiceCounts[i][j] == 0) {
				needreindexing = true;
				break;
			}
		}
		if (needreindexing) {
			break;
		}
	}

	if (!needreindexing) {
		return;
	}

	vector<vector<int> > remapping;
	remapping.resize(staffVoiceCounts.size());
	int reindex;
	for (int i=0; i<(int)staffVoiceCounts.size(); i++) {
		remapping[i].resize(staffVoiceCounts[i].size());
		reindex = 0;
		for (int j=0; j<(int)remapping[i].size(); j++) {
			if (remapping[i].size() == 1) {
				remapping[i][j] = 0;
				continue;
			}
			if (staffVoiceCounts[i][j]) {
				remapping[i][j] = reindex++;
			} else {
				remapping[i][j] = -1;  // invalidate voice
			}
		}
	}

	// Go back and remap the voice indexes of elements.
	// Presuming that the staff does not need to be reindex.
	for (int i=0; i<(int)elist.size(); i++) {
		int oldvoice = elist[i]->getVoiceIndex();
		int staff = elist[i]->getStaffIndex();
		if (oldvoice < 0) {
			continue;
		}
		int newvoice = remapping[staff][oldvoice];
		if (newvoice == oldvoice) {
			continue;
		}
		elist[i]->setVoiceIndex(newvoice);
	}

}



//////////////////////////////
//
// Tool_musicxml2hum::setOptions --
//

void Tool_musicxml2hum::setOptions(int argc, char** argv) {
	m_options.process(argc, argv);
}


void Tool_musicxml2hum::setOptions(const vector<string>& argvlist) {
    m_options.process(argvlist);
}



//////////////////////////////
//
// Tool_musicxml2hum::getOptionDefinitions -- Used to avoid
//     duplicating the definitions in the test main() function.
//

Options Tool_musicxml2hum::getOptionDefinitions(void) {
	return m_options;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// Tool_musicxml2hum::fillPartData --
//

bool Tool_musicxml2hum::fillPartData(vector<MxmlPart>& partdata,
		const vector<string>& partids, map<string, xml_node>& partinfo,
		map<string, xml_node>& partcontent) {

	bool output = true;
	for (int i=0; i<(int)partinfo.size(); i++) {
		partdata[i].setPartNumber(i+1);
		output &= fillPartData(partdata[i], partids[i], partinfo[partids[i]],
				partcontent[partids[i]]);
	}
	return output;
}


bool Tool_musicxml2hum::fillPartData(MxmlPart& partdata,
		const string& id, xml_node partdeclaration, xml_node partcontent) {
	if (m_stemsQ) {
		partdata.enableStems();
	}
	int count;
	auto measures = partcontent.select_nodes("./measure");
	for (int i=0; i<(int)measures.size(); i++) {
		partdata.addMeasure(measures[i].node());
		count = partdata.getMeasureCount();
		if (count > 1) {
			HumNum dur = partdata.getMeasure(count-1)->getTimeSigDur();
			if (dur == 0) {
				HumNum dur = partdata.getMeasure(count-2)
						->getTimeSigDur();
				if (dur > 0) {
					partdata.getMeasure(count - 1)->setTimeSigDur(dur);
				}
			}
		}

	}
	return true;
}



//////////////////////////////
//
// Tool_musicxml2hum::printPartInfo -- Debug information.
//

void Tool_musicxml2hum::printPartInfo(vector<string>& partids,
		map<string, xml_node>& partinfo, map<string, xml_node>& partcontent,
		vector<MxmlPart>& partdata) {
	cout << "\nPart information in the file:" << endl;
	int maxmeasure = 0;
	for (int i=0; i<(int)partids.size(); i++) {
		cout << "\tPART " << i+1 << " id = " << partids[i] << endl;
		cout << "\tMAXSTAFF " << partdata[i].getStaffCount() << endl;
		cout << "\t\tpart name:\t"
		     << getChildElementText(partinfo[partids[i]], "part-name") << endl;
		cout << "\t\tpart abbr:\t"
		     << getChildElementText(partinfo[partids[i]], "part-abbreviation")
		     << endl;
		auto node = partcontent[partids[i]];
		auto measures = node.select_nodes("./measure");
		cout << "\t\tMeasure count:\t" << measures.size() << endl;
		if (maxmeasure < (int)measures.size()) {
			maxmeasure = (int)measures.size();
		}
		cout << "\t\tTotal duration:\t" << partdata[i].getDuration() << endl;
	}

	MxmlMeasure* measure;
	for (int i=0; i<maxmeasure; i++) {
		cout << "m" << i+1 << "\t";
		for (int j=0; j<(int)partdata.size(); j++) {
			measure = partdata[j].getMeasure(i);
			if (measure) {
				cout << measure->getDuration();
			}
			if (j < (int)partdata.size() - 1) {
				cout << "\t";
			}
		}
		cout << endl;
	}
}



//////////////////////////////
//
// stitchParts -- Merge individual parts into a single score sequence.
//

bool Tool_musicxml2hum::stitchParts(HumGrid& outdata,
		vector<string>& partids, map<string, xml_node>& partinfo,
		map<string, xml_node>& partcontent, vector<MxmlPart>& partdata) {
	if (partdata.size() == 0) {
		return false;
	}

	int i;
	int measurecount = partdata[0].getMeasureCount();
	// i used to start at 1 for some strange reason.
	for (i=0; i<(int)partdata.size(); i++) {
		if (measurecount != partdata[i].getMeasureCount()) {
			cerr << "ERROR: cannot handle parts with different measure\n";
			cerr << "counts yet. Compare MM" << measurecount << " to MM";
			cerr << partdata[i].getMeasureCount() << endl;
			exit(1);
		}
	}

	vector<int> partstaves(partdata.size(), 0);
	for (i=0; i<(int)partstaves.size(); i++) {
		partstaves[i] = partdata[i].getStaffCount();
	}

	bool status = true;
	int m;
	for (m=0; m<partdata[0].getMeasureCount(); m++) {
		status &= insertMeasure(outdata, m, partdata, partstaves);
		// a hack for now:
		// insertSingleMeasure(outfile);
		// measures.push_back(&outfile[outfile.getLineCount()-1]);
	}

	return status;
}



//////////////////////////////
//
// Tool_musicxml2hum::cleanupMeasures --
//     Also add barlines here (keeping track of the
//     duration of each measure).
//

void Tool_musicxml2hum::cleanupMeasures(HumdrumFile& outfile,
		vector<HumdrumLine*> measures) {

   HumdrumToken* token;
	for (int i=0; i<outfile.getLineCount(); i++) {
		if (!outfile[i].isBarline()) {
			continue;
		}
		if (!outfile[i+1].isInterpretation()) {
			int fieldcount = outfile[i+1].getFieldCount();
			for (int j=1; j<fieldcount; j++) {
				token = new HumdrumToken("=");
				outfile[i].appendToken(token);
			}
		}
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::insertSingleMeasure --
//

void Tool_musicxml2hum::insertSingleMeasure(HumdrumFile& outfile) {
	HumdrumLine* line = new HumdrumLine;
	HumdrumToken* token;
	token = new HumdrumToken("=");
	line->appendToken(token);
	line->createLineFromTokens();
	outfile.appendLine(line);
}



//////////////////////////////
//
// Tool_musicxml2hum::insertAllToken --
//

void Tool_musicxml2hum::insertAllToken(HumdrumFile& outfile,
		vector<MxmlPart>& partdata, const string& common) {

	HumdrumLine* line = new HumdrumLine;
	HumdrumToken* token;

	int i, j;
	for (i=0; i<(int)partdata.size(); i++) {
		for (j=0; j<(int)partdata[i].getStaffCount(); j++) {
			token = new HumdrumToken(common);
			line->appendToken(token);
		}
		for (j=0; j<(int)partdata[i].getVerseCount(); j++) {
			token = new HumdrumToken(common);
			line->appendToken(token);
		}
	}
	outfile.appendLine(line);
}



//////////////////////////////
//
// Tool_musicxml2hum::insertMeasure --
//

bool Tool_musicxml2hum::insertMeasure(HumGrid& outdata, int mnum,
		vector<MxmlPart>& partdata, vector<int> partstaves) {

	GridMeasure* gm = new GridMeasure(&outdata);
	outdata.push_back(gm);

	MxmlMeasure* xmeasure;
	vector<MxmlMeasure*> measuredata;
	vector<vector<SimultaneousEvents>* > sevents;
	int i;

	for (i=0; i<(int)partdata.size(); i++) {
		xmeasure = partdata[i].getMeasure(mnum);
		measuredata.push_back(xmeasure);
		if (i==0) {
			gm->setDuration(partdata[i].getMeasure(mnum)->getDuration());
			gm->setTimestamp(partdata[i].getMeasure(mnum)->getTimestamp());
			gm->setTimeSigDur(partdata[i].getMeasure(mnum)->getTimeSigDur());
		}
		checkForDummyRests(xmeasure);
		sevents.push_back(xmeasure->getSortedEvents());
		if (i == 0) {
			// only checking measure style of first barline
			gm->setBarStyle(xmeasure->getBarStyle());
		}
	}

	vector<HumNum> curtime(partdata.size());
	vector<HumNum> measuredurs(partdata.size());
	vector<int> curindex(partdata.size(), 0); // assuming data in a measure...
	HumNum nexttime = -1;

	HumNum tsdur;
	for (i=0; i<(int)curtime.size(); i++) {
		tsdur = measuredata[i]->getTimeSigDur();
		if ((tsdur == 0) && (i > 0)) {
			tsdur = measuredata[i-1]->getTimeSigDur();
			measuredata[i]->setTimeSigDur(tsdur);
		}
		if (VoiceDebugQ) {
			vector<MxmlEvent*>& events = measuredata[i]->getEventList();
			for (int j=0; j<(int)events.size(); j++) {
				cerr << "!!ELEMENT: ";
				cerr << "\tSTi:   " << events[j]->getStaffIndex();
				cerr << "\tVi:    " << events[j]->getVoiceIndex();
				cerr << "\tTS:    " << events[j]->getStartTime();
				cerr << "\tDUR:   " << events[j]->getDuration();
				cerr << "\tPITCH: " << events[j]->getKernPitch();
				cerr << "\tNAME:  " << events[j]->getElementName();
				cerr << endl;
			}
		}
		if (!(*sevents[i]).empty()) {
			curtime[i] = (*sevents[i])[curindex[i]].starttime;
		} else {
			curtime[i] = tsdur;
		}
		if (nexttime < 0) {
			nexttime = curtime[i];
		} else if (curtime[i] < nexttime) {
			nexttime = curtime[i];
		}
		measuredurs[i] = measuredata[i]->getDuration();
	}

	bool allend = false;
	vector<SimultaneousEvents*> nowevents;
	vector<int> nowparts;
	bool status = true;
	while (!allend) {
		nowevents.resize(0);
		nowparts.resize(0);
		allend = true;
		HumNum processtime = nexttime;
		nexttime = -1;
		for (i = (int)partdata.size()-1; i >= 0; i--) {
			if (curindex[i] >= (int)(*sevents[i]).size()) {
				continue;
			}

			if ((*sevents[i])[curindex[i]].starttime == processtime) {
				auto thing = &(*sevents[i])[curindex[i]];
				nowevents.push_back(thing);
				nowparts.push_back(i);
				curindex[i]++;
			}

			if (curindex[i] < (int)(*sevents[i]).size()) {
				allend = false;
				if ((nexttime < 0) ||
						((*sevents[i])[curindex[i]].starttime < nexttime)) {
					nexttime = (*sevents[i])[curindex[i]].starttime;
				}
			}
		}
		status &= convertNowEvents(outdata.back(),
				nowevents, nowparts, processtime, partdata, partstaves);
	}

	return status;
}



//////////////////////////////
//
// Tool_musicxml2hum::checkForDummyRests --
//

void Tool_musicxml2hum::checkForDummyRests(MxmlMeasure* measure) {
	vector<MxmlEvent*>& events = measure->getEventList();

	MxmlPart* owner = measure->getOwner();
	int maxstaff = owner->getStaffCount();
	vector<vector<int> > itemcounts(maxstaff);
	for (int i=0; i<(int)itemcounts.size(); i++) {
		itemcounts[i].resize(1);
		itemcounts[i][0] = 0;
	}

	for (int i=0; i<(int)events.size(); i++) {
		if (!nodeType(events[i]->getNode(), "note")) {
			// only counting notes/(rests) for now.  <forward> may
			// need to be counted.
			continue;
		}
     	int voiceindex = events[i]->getVoiceIndex();
		int staffindex = events[i]->getStaffIndex();

		if (voiceindex < 0) {
			continue;
		}
		if (staffindex < 0) {
			continue;
		}

		if (staffindex >= (int)itemcounts.size()) {
			itemcounts.resize(staffindex+1);
		}

		if (voiceindex >= (int)itemcounts[staffindex].size()) {
			int oldsize = (int)itemcounts[staffindex].size();
			int newsize = voiceindex + 1;
			itemcounts[staffindex].resize(newsize);
			for (int j=oldsize; j<newsize; j++) {
					  itemcounts[staffindex][j] = 0;
			}
		}
		itemcounts[staffindex][voiceindex]++;
  	}

	bool dummy = false;
	for (int i=0; i<(int)itemcounts.size(); i++) {
		for (int j=0; j<(int)itemcounts[i].size(); j++) {
			if (itemcounts[i][j]) {
				continue;
			}
			HumNum mdur = measure->getDuration();
			HumNum starttime = measure->getStartTime();
      	measure->addDummyRest(starttime, mdur, i, j);
			measure->forceLastInvisible();
			dummy = true;
		}
	}

	if (dummy) {
		measure->sortEvents();
	}

}



//////////////////////////////
//
// Tool_musicxml2hum::convertNowEvents --
//

bool Tool_musicxml2hum::convertNowEvents(GridMeasure* outdata,
		vector<SimultaneousEvents*>& nowevents, vector<int>& nowparts,
		HumNum nowtime, vector<MxmlPart>& partdata, vector<int>& partstaves) {

	if (nowevents.size() == 0) {
		// cout << "NOW EVENTS ARE EMPTY" << endl;
		return true;
	}

	//if (0 && VoiceDebugQ) {
	//	for (int j=0; j<(int)nowevents.size(); j++) {
	//		vector<MxmlEvent*> nz = nowevents[j]->nonzerodur;
	//		for (int i=0; i<(int)nz.size(); i++) {
	//			cerr << "NOWEVENT NZ NAME: " << nz[i]->getElementName()
	//			     << "<\t" << nz[i]->getKernPitch() << endl;
	//		}
	//	}
	//}

	appendZeroEvents(outdata, nowevents, nowtime, partdata);

	if (nowevents[0]->nonzerodur.size() == 0) {
		// no duration events (should be a terminal barline)
		// ignore and deal with in calling function.
		return true;
	}

	appendNonZeroEvents(outdata, nowevents, nowtime, partdata);

	return true;
}



/////////////////////////////
//
// Tool_musicxml2hum::appendNonZeroEvents --
//

void Tool_musicxml2hum::appendNonZeroEvents(GridMeasure* outdata,
		vector<SimultaneousEvents*>& nowevents, HumNum nowtime,
		vector<MxmlPart>& partdata) {

	GridSlice* slice = new GridSlice(outdata, nowtime,
			SliceType::Notes);
	outdata->push_back(slice);
	slice->initializePartStaves(partdata);

	for (int i=0; i<(int)nowevents.size(); i++) {
		vector<MxmlEvent*>& events = nowevents[i]->nonzerodur;
		for (int j=0; j<(int)events.size(); j++) {
			addEvent(*slice, events[j]);
		}
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::addEvent -- Add a note or rest.
//

void Tool_musicxml2hum::addEvent(GridSlice& slice,
		MxmlEvent* event) {

	int partindex;  // which part the event occurs in
	int staffindex; // which staff the event occurs in (need to fix)
	int voiceindex; // which voice the event occurs in (use for staff)

	partindex  = event->getPartIndex();
	staffindex = event->getStaffIndex();
	voiceindex = event->getVoiceIndex();

	string recip;
	string pitch;
	string prefix;
	string postfix;
	bool grace = false;
	bool invisible = false;
	bool primarynote = true;
	bool slurstart = false;
	bool slurstop = false;
	int slurdir = 0;

	if (!event->isFloating()) {
		recip     = event->getRecip();
		pitch     = event->getKernPitch();
		prefix    = event->getPrefixNoteInfo();
		postfix   = event->getPostfixNoteInfo(primarynote);
		grace     = event->isGrace();
		slurstart = event->hasSlurStart(slurdir);
		slurstop  = event->hasSlurStop();

		if (slurstart) {
			prefix.insert(0, "(");
			if (slurdir) {
				if (slurdir > 0) {
					prefix.insert(1, ">");
					m_slurabove++;
				} else if (slurdir < 0) {
					prefix.insert(1, "<");
					m_slurbelow++;
				}
			}
		}
		if (slurstop) {
			postfix.push_back(')');
		}

		invisible = isInvisible(event);
		if (event->isInvisible()) {
			invisible = true;
		}

		if (grace) {
			HumNum dur = event->getEmbeddedDuration(event->getNode()) / 4;
			if (dur.getNumerator() == 1) {
				recip = to_string(dur.getDenominator()) + "q";
			} else {
				recip = "q";
			}
		}
	}

	stringstream ss;
	if (event->isFloating()) {
		ss << ".";
		HTp token = new HumdrumToken(ss.str());
		slice.at(partindex)->at(staffindex)->setTokenLayer(voiceindex, token,
			event->getDuration());
	} else {
		ss << prefix << recip << pitch << postfix;
		if (invisible) {
			ss << "yy";
		}

		// check for chord notes.
		HTp token;
		if (event->isChord()) {
			addSecondaryChordNotes(ss, event, recip);
			token = new HumdrumToken(ss.str());
			slice.at(partindex)->at(staffindex)->setTokenLayer(voiceindex, token,
				event->getDuration());
		} else {
			token = new HumdrumToken(ss.str());
			slice.at(partindex)->at(staffindex)->setTokenLayer(voiceindex, token,
				event->getDuration());
		}
	}

	if (DebugQ) {
		cerr << "!!TOKEN: " << ss.str();
		cerr << "\tTS: "    << event->getStartTime();
		cerr << "\tDUR: "   << event->getDuration();
		cerr << "\tSTi: "   << event->getStaffNumber();
		cerr << "\tVn: "    << event->getVoiceNumber();
		cerr << "\tSTi: "   << event->getStaffIndex();
		cerr << "\tVi: "    << event->getVoiceIndex();
		cerr << "\teNAME: " << event->getElementName();
		cerr << endl;
	}

	int vcount = addLyrics(slice.at(partindex)->at(staffindex), event);

	if (vcount > 0) {
		event->reportVerseCountToOwner(staffindex, vcount);
	}

	int hcount = addHarmony(slice.at(partindex), event);
	if (hcount > 0) {
		event->reportHarmonyCountToOwner(hcount);
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::addHarmony --
//

int Tool_musicxml2hum::addHarmony(GridPart* part, MxmlEvent* event) {
	xml_node hnode = event->getHNode();
	if (!hnode) {
		return 0;
	}

	// fill in X with the harmony values from the <harmony> node
	string hstring = getHarmonyString(hnode);
	HTp htok = new HumdrumToken(hstring);
	part->setHarmony(htok);

	return 1;
}



//////////////////////////////
//
// Tool_musicxml2hum::getHarmonyString --
//   <harmony default-y="40">
//       <root>
//           <root-step>C</root-step>
//       </root>
//       <kind>major-ninth</kind>
//       <bass>
//           <bass-step>E</bass-step>
//       </bass>
//   </harmony>
//

string Tool_musicxml2hum::getHarmonyString(xml_node hnode) {
	if (!hnode) {
		return "";
	}
	xml_node child = hnode.first_child();
	if (!child) {
		return "";
	}
	string root;
	string kind;
	string bass;
	int rootalter = 0;
	int bassalter = 0;
	xml_node grandchild;
	while (child) {
		if (nodeType(child, "root")) {
			grandchild = child.first_child();
			while (grandchild) {
				if (nodeType(grandchild, "root-step")) {
					root = grandchild.child_value();
				} if (nodeType(grandchild, "root-alter")) {
					rootalter = atoi(grandchild.child_value());
				}
				grandchild = grandchild.next_sibling();
			}
		} else if (nodeType(child, "kind")) {
			kind = child.child_value();
			if (kind == "") {
				kind = child.attribute("text").value();
				transform(kind.begin(), kind.end(), kind.begin(), ::tolower);
			}
		} else if (nodeType(child, "bass")) {
			grandchild = child.first_child();
			while (grandchild) {
				if (nodeType(grandchild, "bass-step")) {
					bass = grandchild.child_value();
				} if (nodeType(grandchild, "bass-alter")) {
					bassalter = atoi(grandchild.child_value());
				}
				grandchild = grandchild.next_sibling();
			}
		}
		child = child.next_sibling();
	}
	stringstream ss;
	ss << root;

	if (rootalter > 0) {
		for (int i=0; i<rootalter; i++) {
			ss << "#";
		}
	} else if (rootalter < 0) {
		for (int i=0; i<-rootalter; i++) {
			ss << "-";
		}
	}

	if (root.size() && kind.size()) {
		ss << " ";
	}
	ss << kind;
	if (bass.size()) {
		ss << "/";
	}
	ss << bass;

	if (bassalter > 0) {
		for (int i=0; i<bassalter; i++) {
			ss << "#";
		}
	} else if (bassalter < 0) {
		for (int i=0; i<-bassalter; i++) {
			ss << "-";
		}
	}

	string output = cleanSpaces(ss.str());
	return output;
}



//////////////////////////////
//
// Tool_musicxml2hum::addLyrics --
//

int Tool_musicxml2hum::addLyrics(GridStaff* staff, MxmlEvent* event) {
	xml_node node = event->getNode();
	if (!node) {
		return 0;
	}
	xml_node child = node.first_child();
	xml_node grandchild;
	// int max;
	int number;
	vector<xml_node> verses;
	string syllabic;
	string text;
	while (child) {
		if (!nodeType(child, "lyric")) {
			child = child.next_sibling();
			continue;
		}
		number = atoi(child.attribute("number").value());
		if (number == (int)verses.size() + 1) {
			verses.push_back(child);
		} else if ((number > 0) && (number < (int)verses.size())) {
			// replace a verse for some reason.
			verses[number-1] = child;
		} else if (number > 0) {
			int oldsize = (int)verses.size();
			int newsize = number;
			verses.resize(newsize);
			for (int i=oldsize; i<newsize; i++) {
				verses[i] = xml_node(NULL);
			}
			verses[number-1] = child;
		}
		child = child.next_sibling();
	}

	string finaltext;
	HTp token;
	for (int i=0; i<(int)verses.size(); i++) {
		if (!verses[i]) {
			// no verse so doing an empty slot.
		} else {
			child = verses[i].first_child();
			finaltext = "";
			while (child) {
				if (nodeType(child, "syllabic")) {
					syllabic = child.child_value();
					child = child.next_sibling();
					continue;
				} else if (nodeType(child, "text")) {
					text = cleanSpaces(child.child_value());
				} else if (nodeType(child, "elision")) {
					finaltext += " ";
					child = child.next_sibling();
					continue;
				} else {
					// such as <extend>
					child = child.next_sibling();
					continue;
				}
				// escape text which would otherwise be reinterpreated
				// as Humdrum syntax.
				if (!text.empty()) {
					if (text[0] == '!') {
						text.insert(0, 1, '\\');
					} else if (text[0] == '*') {
						text.insert(0, 1, '\\');
					}
				}
				child = child.next_sibling();
				if (syllabic == "middle" ) {
					finaltext += "-";
					finaltext += text;
					finaltext += "-";
				} else if (syllabic == "end") {
					finaltext += "-";
					finaltext += text;
				} else if (syllabic == "begin") {
					finaltext += text;
					finaltext += "-";
				} else {
					finaltext += text;
				}
			}
		}

		if (finaltext.empty()) {
			continue;
		}

		if (verses[i]) {
			token = new HumdrumToken(finaltext);
			staff->setVerse(i,token);
		} else {
			token = new HumdrumToken(".");
			staff->setVerse(i,token);
		}
	}

	return (int)staff->getVerseCount();
}



//////////////////////////////
//
// cleanSpaces -- remove trailing and leading spaces from text.
//    Also removed doubled spaces, and converts tabs and newlines
//    into spaces.
//

string Tool_musicxml2hum::cleanSpaces(const string& input) {
	int endi = (int)input.size() - 1;
	while (endi >= 0) {
		if (isspace(input[endi])) {
			endi--;
			continue;
		}
		break;
	}
	int starti = 0;
	while (starti <= endi) {
		if (isspace(input[starti])) {
			starti++;
			continue;
		}
		break;

	}
	string output;
   for (int i=starti; i<=endi; i++) {
		if (!isspace(input[i])) {
			output += input[i];
			continue;
		}
		output += " ";
		i++;
		while ((i < endi) && isspace(input[i])) {
			i++;
		}
		i--;
	}
	return output;
}



//////////////////////////////
//
// Tool_musicxml2hum::isInvisible --
//

bool Tool_musicxml2hum::isInvisible(MxmlEvent* event) {
	xml_node node = event->getNode();
	if (!node) {
		return false;
	}
	if (strcmp(node.attribute("print-object").value(), "no") == 0) {
		return true;
	}

	return false;
}



//////////////////////////////
//
// Tool_musicxml2hum::addSecondaryChordNotes --
//

void Tool_musicxml2hum::addSecondaryChordNotes(ostream& output,
		MxmlEvent* head, const string& recip) {
	vector<MxmlEvent*> links = head->getLinkedNotes();
	MxmlEvent* note;
	string pitch;
	string prefix;
	string postfix;
	bool slurstart = false;
	bool slurstop  = false;
	int  slurdir = 0;

	bool primarynote = false;
	for (int i=0; i<(int)links.size(); i++) {
		note = links.at(i);
		pitch   = note->getKernPitch();
		prefix  = note->getPrefixNoteInfo();
		postfix = note->getPostfixNoteInfo(primarynote);
		slurstart = note->hasSlurStart(slurdir);
		slurstop  = note->hasSlurStop();

		if (slurstart) {
			prefix.insert(0, "(");
			if (slurdir) {
				if (slurdir > 0) {
					prefix.insert(1, ">");
					m_slurabove++;
				} else if (slurdir < 0) {
					prefix.insert(1, "<");
					m_slurbelow++;
				}
			}
		}
		if (slurstop) {
			postfix.push_back(')');
		}

		output << " " << prefix << recip << pitch << postfix;
	}
}



/////////////////////////////
//
// Tool_musicxml2hum::appendZeroEvents --
//

void Tool_musicxml2hum::appendZeroEvents(GridMeasure* outdata,
		vector<SimultaneousEvents*>& nowevents, HumNum nowtime,
		vector<MxmlPart>& partdata) {

	bool hasclef    = false;
	bool haskeysig  = false;
	bool hastimesig = false;

	vector<vector<xml_node> > clefs(partdata.size());
	vector<vector<xml_node> > keysigs(partdata.size());
	vector<vector<xml_node> > timesigs(partdata.size());

	vector<vector<vector<vector<MxmlEvent*> > > > gracebefore(partdata.size());
	vector<vector<vector<vector<MxmlEvent*> > > > graceafter(partdata.size());
	bool foundnongrace = false;

	int pindex = 0;
	xml_node child;

	for (int i=0; i<(int)nowevents.size(); i++) {
		for (int j=0; j<(int)nowevents[i]->zerodur.size(); j++) {
			xml_node element = nowevents[i]->zerodur[j]->getNode();

			if (nodeType(element, "attributes")) {
				child = element.first_child();
				while (child) {
					pindex = nowevents[i]->zerodur[j]->getPartIndex();
					if (nodeType(child, "clef")) {
						clefs[pindex].push_back(child);
						hasclef = true;
						foundnongrace = true;
					}

					if (nodeType(child, "key")) {
						keysigs[pindex].push_back(child);
						haskeysig = true;
						foundnongrace = true;
					}

					if (nodeType(child, "time")) {
						timesigs[pindex].push_back(child);
						hastimesig = true;
						foundnongrace = true;
					}

					child = child.next_sibling();
				}
			} else if (nodeType(element, "note")) {
				if (foundnongrace) {
					addEventToList(graceafter, nowevents[i]->zerodur[j]);
				} else {
					addEventToList(gracebefore, nowevents[i]->zerodur[j]);
				}
			}
		}
	}

	addGraceLines(outdata, gracebefore, partdata, nowtime);

	if (hasclef) {
		addClefLine(outdata, clefs, partdata, nowtime);
	}

	if (haskeysig) {
		addKeySigLine(outdata, keysigs, partdata, nowtime);
	}

	if (hastimesig) {
		addTimeSigLine(outdata, timesigs, partdata, nowtime);
	}

	addGraceLines(outdata, graceafter, partdata, nowtime);
}



///////////////////////////////
//
// Tool_musicxml2hum::addEventToList --
//

void Tool_musicxml2hum::addEventToList(vector<vector<vector<vector<MxmlEvent*> > > >& list, 
		MxmlEvent* event) {
	int pindex = event->getPartIndex();
	int staffindex = event->getStaffIndex();
	int voiceindex = event->getVoiceIndex();
	if (pindex >= (int)list.size()) {
		list.resize(pindex+1);
	}
	if (staffindex >= (int)list[pindex].size()) {
		list[pindex].resize(staffindex+1);
	}
	if (voiceindex >= (int)list[pindex][staffindex].size()) {
		list[pindex][staffindex].resize(voiceindex+1);
	}
	list[pindex][staffindex][voiceindex].push_back(event);
}



///////////////////////////////
//
// Tool_musicxml2hum::addGraceLines -- Add grace note lines.  The number of 
//     lines is equal to the maximum number of successive grace notes in
//     any part.  Grace notes are filled in reverse sequence.
//

void Tool_musicxml2hum::addGraceLines(GridMeasure* outdata,
		vector<vector<vector<vector<MxmlEvent*> > > >& notes,
		vector<MxmlPart>& partdata, HumNum nowtime) {

	int maxcount = 0;

	for (int i=0; i<(int)notes.size(); i++) {
		for (int j=0; j<(int)notes.at(i).size(); j++) {
			for (int k=0; k<(int)notes.at(i).at(j).size(); k++) {
				if (maxcount < (int)notes.at(i).at(j).at(k).size()) {
					maxcount = (int)notes.at(i).at(j).at(k).size();
				}
			}
		}
	}

	if (maxcount == 0) {
		return;
	}

	vector<GridSlice*> slices(maxcount);
	for (int i=0; i<(int)slices.size(); i++) {
		slices[i] = new GridSlice(outdata, nowtime, SliceType::GraceNotes);
		outdata->push_back(slices[i]);
		slices[i]->initializePartStaves(partdata);
	}

	for (int i=0; i<(int)notes.size(); i++) {
		for (int j=0; j<(int)notes[i].size(); j++) {
			for (int k=0; k<(int)notes[i][j].size(); k++) {
				int startm = maxcount - (int)notes[i][j][k].size();
				for (int m=0; m<(int)notes[i][j][k].size(); m++) {
					addEvent(*slices.at(startm+m), notes[i][j][k][m]);
				}
			}
		}
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::addClefLine --
//

void Tool_musicxml2hum::addClefLine(GridMeasure* outdata,
		vector<vector<xml_node> >& clefs, vector<MxmlPart>& partdata,
		HumNum nowtime) {

	GridSlice* slice = new GridSlice(outdata, nowtime,
		SliceType::Clefs);
	outdata->push_back(slice);
	slice->initializePartStaves(partdata);

	for (int i=0; i<(int)partdata.size(); i++) {
		for (int j=0; j<(int)clefs[i].size(); j++) {
			if (clefs[i][j]) {
				insertPartClefs(clefs[i][j], *slice->at(i));
			}
		}
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::addTimeSigLine --
//

void Tool_musicxml2hum::addTimeSigLine(GridMeasure* outdata,
		vector<vector<xml_node> >& timesigs, vector<MxmlPart>& partdata,
		HumNum nowtime) {

	GridSlice* slice = new GridSlice(outdata, nowtime, SliceType::TimeSigs);
	outdata->push_back(slice);
	slice->initializePartStaves(partdata);

	bool status = false;

	for (int i=0; i<(int)partdata.size(); i++) {
		for (int j=0; j<(int)timesigs[i].size(); j++) {
			if (timesigs[i][j]) {
				status |= insertPartTimeSigs(timesigs[i][j], *slice->at(i));
			}
		}
	}

	if (!status) {
		return;
	}

	// Add mensurations related to time signatures

	slice = new GridSlice(outdata, nowtime, SliceType::MeterSigs);
	outdata->push_back(slice);
	slice->initializePartStaves(partdata);

	// now add mensuration symbols associated with time signatures
	for (int i=0; i<(int)partdata.size(); i++) {
		for (int j=0; j<(int)timesigs[i].size(); j++) {
			if (timesigs[i][j]) {
				insertPartMensurations(timesigs[i][j], *slice->at(i));
			}
		}
	}


}



//////////////////////////////
//
// Tool_musicxml2hum::addKeySigLine -- Only adding one key signature
//   for each part for now.
//

void Tool_musicxml2hum::addKeySigLine(GridMeasure* outdata,
		vector<vector<xml_node> >& keysigs,
		vector<MxmlPart>& partdata, HumNum nowtime) {

	GridSlice* slice = new GridSlice(outdata, nowtime,
		SliceType::KeySigs);
	outdata->push_back(slice);
	slice->initializePartStaves(partdata);

	for (int i=0; i<(int)partdata.size(); i++) {
		for (int j=0; j<(int)keysigs[i].size(); j++) {
			if (keysigs[i][j]) {
				insertPartKeySigs(keysigs[i][j], *slice->at(i));
			}
		}
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::insertPartClefs --
//

void Tool_musicxml2hum::insertPartClefs(xml_node clef, GridPart& part) {
	if (!clef) {
		// no clef for some reason.
		return;
	}

	HTp token;
	int staffnum = 0;
	while (clef) {
		clef = convertClefToHumdrum(clef, token, staffnum);
		part[staffnum]->setTokenLayer(0, token, 0);
	}

	// go back and fill in all NULL pointers with null interpretations
	fillEmpties(&part, "*");
}



//////////////////////////////
//
// Tool_musicxml2hum::fillEmpties --
//

void Tool_musicxml2hum::fillEmpties(GridPart* part, const char* string) {
	int staffcount = (int)part->size();
	GridVoice* gv;
	int vcount;

 	for (int s=0; s<staffcount; s++) {
		GridStaff* staff = part->at(s);
		if (staff == NULL) {
			cerr << "Strange error here" << endl;
			continue;
		}
		vcount = (int)staff->size();
		if (vcount == 0) {
			gv = new GridVoice(string, 0);
			staff->push_back(gv);
		} else {
			for (int v=0; v<vcount; v++) {
				gv = staff->at(v);
				if (gv == NULL) {
					gv = new GridVoice(string, 0);
					staff->at(v) = gv;
				}
			}
		}
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::insertPartKeySigs --
//

void Tool_musicxml2hum::insertPartKeySigs(xml_node keysig, GridPart& part) {
	if (!keysig) {
		return;
	}

	HTp token;
	int staffnum = 0;
	while (keysig) {
		keysig = convertKeySigToHumdrum(keysig, token, staffnum);
		if (staffnum < 0) {
			// key signature applies to all staves in part (most common case)
			for (int s=0; s<(int)part.size(); s++) {
				if (s==0) {
					part[s]->setTokenLayer(0, token, 0);
				} else {
					HTp token2 = new HumdrumToken(*token);
					part[s]->setTokenLayer(0, token2, 0);
				}
			}
		} else {
			part[staffnum]->setTokenLayer(0, token, 0);
		}
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::insertPartTimeSigs -- Only allowing one
//		time signature per part for now.
//

bool Tool_musicxml2hum::insertPartTimeSigs(xml_node timesig, GridPart& part) {
	if (!timesig) {
		// no timesig
		return false;
	}

	bool hasmensuration = false;
	HTp token;
	int staffnum = 0;
	
	while (timesig) {
		hasmensuration |= checkForMensuration(timesig);
		timesig = convertTimeSigToHumdrum(timesig, token, staffnum);
		if (staffnum < 0) {
			// time signature applies to all staves in part (most common case)
			for (int s=0; s<(int)part.size(); s++) {
				if (s==0) {
					part[s]->setTokenLayer(0, token, 0);
				} else {
					HTp token2 = new HumdrumToken(*token);
					part[s]->setTokenLayer(0, token2, 0);
				}
			}
		} else {
			part[staffnum]->setTokenLayer(0, token, 0);
		}
	}

	return hasmensuration;
}



//////////////////////////////
//
// Tool_musicxml2hum::insertPartMensurations -- 
//

void Tool_musicxml2hum::insertPartMensurations(xml_node timesig,
		GridPart& part) {
	if (!timesig) {
		// no timesig
		return;
	}

	HTp token;
	int staffnum = 0;

	while (timesig) {
		timesig = convertMensurationToHumdrum(timesig, token, staffnum);
		if (staffnum < 0) {
			// time signature applies to all staves in part (most common case)
			for (int s=0; s<(int)part.size(); s++) {
				if (s==0) {
					part[s]->setTokenLayer(0, token, 0);
				} else {
					HTp token2 = new HumdrumToken(*token);
					part[s]->setTokenLayer(0, token2, 0);
				}
			}
		} else {
			part[staffnum]->setTokenLayer(0, token, 0);
		}
	}

}


//////////////////////////////
//
// Tool_musicxml::checkForMensuration --
//    Examples:
//        <time symbol="common">
//        <time symbol="cut">
//

bool Tool_musicxml2hum::checkForMensuration(xml_node timesig) {
	if (!timesig) {
		return false;
	}

	xml_attribute mens = timesig.attribute("symbol");
	if (mens) {
		return true;
	} else {
		return false;
	}
}


//////////////////////////////
//
//	Tool_musicxml2hum::convertKeySigToHumdrum --
//
//  <key>
//     <fifths>4</fifths>
//

xml_node Tool_musicxml2hum::convertKeySigToHumdrum(xml_node keysig,
		HTp& token, int& staffindex) {

	if (!keysig) {
		return keysig;
	}

	staffindex = -1;
	xml_attribute sn = keysig.attribute("number");
	if (sn) {
		staffindex = atoi(sn.value()) - 1;
	}

	int fifths = 0;

	xml_node child = keysig.first_child();
	while (child) {
		if (nodeType(child, "fifths")) {
			fifths = atoi(child.child_value());
		}
		child = child.next_sibling();
	}

	stringstream ss;
	ss << "*k[";
	if (fifths > 0) {
		if (fifths > 0) { ss << "f#"; }
		if (fifths > 1) { ss << "c#"; }
		if (fifths > 2) { ss << "g#"; }
		if (fifths > 3) { ss << "d#"; }
		if (fifths > 4) { ss << "a#"; }
		if (fifths > 5) { ss << "e#"; }
		if (fifths > 6) { ss << "b#"; }
	} else if (fifths < 0) {
		if (fifths < 0)  { ss << "b-"; }
		if (fifths < -1) { ss << "e-"; }
		if (fifths < -2) { ss << "a-"; }
		if (fifths < -3) { ss << "d-"; }
		if (fifths < -4) { ss << "g-"; }
		if (fifths < -5) { ss << "c-"; }
		if (fifths < -6) { ss << "f-"; }
	}
	ss << "]";

	token = new HumdrumToken(ss.str());

	keysig = keysig.next_sibling();
	if (!keysig) {
		return keysig;
	}
	if (nodeType(keysig, "key")) {
		return keysig;
	} else {
		return xml_node(NULL);
	}
}



//////////////////////////////
//
//	Tool_musicxml2hum::convertTimeSigToHumdrum --
//
//  <time symbol="common">
//     <beats>4</beats>
//     <beat-type>4</beat-type>
//
// also:
//  <time symbol="common">
//

xml_node Tool_musicxml2hum::convertTimeSigToHumdrum(xml_node timesig,
		HTp& token, int& staffindex) {

	if (!timesig) {
		return timesig;
	}

	staffindex = -1;
	xml_attribute sn = timesig.attribute("number");
	if (sn) {
		staffindex = atoi(sn.value()) - 1;
	}

	int beats = -1;
	int beattype = -1;

	xml_node child = timesig.first_child();
	while (child) {
		if (nodeType(child, "beats")) {
			beats = atoi(child.child_value());
		} else if (nodeType(child, "beat-type")) {
			beattype = atoi(child.child_value());
		}
		child = child.next_sibling();
	}

	stringstream ss;
	ss << "*M" << beats<< "/" << beattype;
	token = new HumdrumToken(ss.str());

	timesig = timesig.next_sibling();
	if (!timesig) {
		return timesig;
	}
	if (nodeType(timesig, "time")) {
		return timesig;
	} else {
		return xml_node(NULL);
	}
}



//////////////////////////////
//
//	Tool_musicxml2hum::convertMensurationToHumdrum --
//
//  <time symbol="common">
//     <beats>4</beats>
//     <beat-type>4</beat-type>
//
// also:
//  <time symbol="common">
//

xml_node Tool_musicxml2hum::convertMensurationToHumdrum(xml_node timesig,
		HTp& token, int& staffindex) {

	if (!timesig) {
		return timesig;
	}

	staffindex = -1;
	xml_attribute mens = timesig.attribute("symbol");
	if (!mens) {
		token = new HumdrumToken("*");
	} else {
		string text = mens.value();
		if (text == "cut") {
			token = new HumdrumToken("*met(c|)");
		} else if (text == "common") {
			token = new HumdrumToken("*met(c)");
		} else {
			token = new HumdrumToken("*");
		}
	}

	timesig = timesig.next_sibling();
	if (!timesig) {
		return timesig;
	}
	if (nodeType(timesig, "time")) {
		return timesig;
	} else {
		return xml_node(NULL);
	}
}



//////////////////////////////
//
//	Tool_musicxml2hum::convertClefToHumdrum --
//

xml_node Tool_musicxml2hum::convertClefToHumdrum(xml_node clef,
		HTp& token, int& staffindex) {

	if (!clef) {
		// no clef for some reason.
		return clef;
	}

	staffindex = 0;
	xml_attribute sn = clef.attribute("number");
	if (sn) {
		staffindex = atoi(sn.value()) - 1;
	}

	string sign;
	int line = 0;
	int octadjust = 0;

	xml_node child = clef.first_child();
	while (child) {
		if (nodeType(child, "sign")) {
			sign = child.child_value();
		} else if (nodeType(child, "line")) {
			line = atoi(child.child_value());
		} else if (nodeType(child, "clef-octave-change")) {
			octadjust = atoi(child.child_value());
		}
		child = child.next_sibling();
	}

	// Check for percussion clefs, etc., here.
	stringstream ss;
	ss << "*clef" << sign;
	if (octadjust < 0) {
		for (int i=0; i < -octadjust; i++) {
			ss << "v";
		}
	} else if (octadjust > 0) {
		for (int i=0; i<octadjust; i++) {
			ss << "^";
		}
	}
	ss << line;
	token = new HumdrumToken(ss.str());

	clef = clef.next_sibling();
	if (!clef) {
		return clef;
	}
	if (nodeType(clef, "clef")) {
		return clef;
	} else {
		return xml_node(NULL);
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::nodeType -- return true if node type matches
//     string.
//

bool Tool_musicxml2hum::nodeType(xml_node node, const char* testname) {
	if (strcmp(node.name(), testname) == 0) {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::appendNullTokens --
//

void Tool_musicxml2hum::appendNullTokens(HumdrumLine* line,
		MxmlPart& part) {
	int i;
	int staffcount = part.getStaffCount();
	int versecount = part.getVerseCount();
	for (i=staffcount-1; i>=0; i--) {
		line->appendToken(".");
	}
	for (i=0; i<versecount; i++) {
		line->appendToken(".");
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::getPartContent -- Extract the part elements in
//     the file indexed by part ID.
//

bool Tool_musicxml2hum::getPartContent(
		map<string, xml_node>& partcontent,
		vector<string>& partids, xml_document& doc) {

	auto parts = doc.select_nodes("/score-partwise/part");
	int count = (int)parts.size();
	if (count != (int)partids.size()) {
		cerr << "Warning: part element count does not match part IDs count: "
		     << parts.size() << " compared to " << partids.size() << endl;
	}

	string partid;
	for (int i=0; i<(int)parts.size(); i++) {
		partid = getAttributeValue(parts[i], "id");
		if (partid.size() == 0) {
			cerr << "Warning: Part " << i << " has no ID" << endl;
		}
		auto status = partcontent.insert(make_pair(partid, parts[i].node()));
		if (status.second == false) {
			cerr << "Error: ID " << partids.back()
			     << " is duplicated and secondary part will be ignored" << endl;
		}
		if (find(partids.begin(), partids.end(), partid) == partids.end()) {
			cerr << "Error: Part ID " << partid
			     << " is not present in part-list element list" << endl;
			continue;
		}
	}

	if (partcontent.size() != partids.size()) {
		cerr << "Error: part-list count does not match part count "
		     << partcontent.size() << " compared to " << partids.size() << endl;
		return false;
	} else {
		return true;
	}
}



//////////////////////////////
//
// Tool_musicxml2hum::getPartInfo -- Extract a list of the part ids,
//    and a reverse mapping to the <score-part> element to which is refers.
//
//	   part-list structure:
//        <part-list>
//          <score-part id="P1"/>
//          <score-part id="P2"/>
//          etc.
//        </part-list>
//

bool Tool_musicxml2hum::getPartInfo(map<string, xml_node>& partinfo,
		vector<string>& partids, xml_document& doc) {
	auto scoreparts = doc.select_nodes("/score-partwise/part-list/score-part");
	partids.reserve(scoreparts.size());
	bool output = true;
	for (auto el : scoreparts) {
		partids.emplace_back(getAttributeValue(el.node(), "id"));
		auto status = partinfo.insert(make_pair(partids.back(), el.node()));
		if (status.second == false) {
			cerr << "Error: ID " << partids.back()
			     << " is duplicated and secondary part will be ignored" << endl;
		}
		output &= status.second;
		partinfo[partids.back()] = el.node();
	}
	return output;
}



//////////////////////////////
//
// Tool_musicxml2hum::getChildElementText -- Return the (first)
//    matching child element's text content.
//

string Tool_musicxml2hum::getChildElementText(xml_node root,
		const char* xpath) {
	return root.select_single_node(xpath).node().child_value();
}

string Tool_musicxml2hum::getChildElementText(xpath_node root,
		const char* xpath) {
	return root.node().select_single_node(xpath).node().child_value();
}



//////////////////////////////
//
// Tool_musicxml2hum::getAttributeValue -- For an xml_node, return
//     the value for the given attribute name.
//

string Tool_musicxml2hum::getAttributeValue(xml_node xnode,
		const string& target) {
	for (auto at = xnode.first_attribute(); at; at = at.next_attribute()) {
		if (target == at.name()) {
			return at.value();
		}
	}
	return "";
}


string Tool_musicxml2hum::getAttributeValue(xpath_node xnode,
		const string& target) {
	auto node = xnode.node();
	for (auto at = node.first_attribute(); at; at = at.next_attribute()) {
		if (target == at.name()) {
			return at.value();
		}
	}
	return "";
}



//////////////////////////////
//
// Tool_musicxml2hum::printAttributes -- Print list of all attributes
//     for an xml_node.
//

void Tool_musicxml2hum::printAttributes(xml_node node) {
	int counter = 1;
	for (auto at = node.first_attribute(); at; at = at.next_attribute()) {
		cout << "\tattribute " << counter++
		     << "\tname  = " << at.name()
		     << "\tvalue = " << at.value()
		     << endl;
	}
}




/////////////////////////////////
//
// Tool_myank::Tool_myank -- Set the recognized options for the tool.
//

Tool_myank::Tool_myank(void) {
	define("v|verbose=b",    "Verbose output of data");
	define("debug=b",        "Debugging information");
	define("inlist=b",       "Show input measure list");
	define("outlist=b",      "Show output measure list");
	define("mark|marks=b",   "Yank measure with marked notes");
	define("T|M|bar-number-text=b", "print barnum with LO text above system ");
	define("d|double|dm|md|mdsep|mdseparator=b", "Put double barline between non-consecutive measure segments");
	define("m|b|measures|bars|measure|bar=s", "Measures to yank");
	define("I|i|instrument=b", "Include instrument codes from start of data");
	define("visible|not-invisible=b", "Do not make initial measure invisible");
	define("B|noendbar=b", "Do not print barline at end of data");
	define("max=b",  "print maximum measure number");
	define("min=b",  "print minimum measure number");
	define("section-count=b", "count the number of sections, JRP style");
	define("section=i:0", "extract given section number (indexed from 1");
	define("author=b",        "Program author");
	define("version=b",       "Program version");
	define("example=b",       "Program examples");
	define("h|help=b",        "Short description");
}



/////////////////////////////////
//
// Tool_myank::run -- Primary interfaces to the tool.
//

bool Tool_myank::run(const string& indata, ostream& out) {
	HumdrumFile infile(indata);
	bool status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_myank::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}

//
// In-place processing of file:
//

bool Tool_myank::run(HumdrumFile& infile) {
	initialize(infile);
	processFile(infile);
	// Re-load the text for each line from their tokens.
	infile.createLinesFromTokens();
	return true;
}


///////////////////////////////////////////////////////////////////////////

ostream& operator<<(ostream& out, MyCoord& value) {
	out << "(" << value.x << "," << value.y << ")";
	return out;
}


ostream& operator<<(ostream& out, MeasureInfo& info) {
	if (info.file == NULL) {
		return out;
	}
	HumdrumFile& infile = *(info.file);
	out << "================================== " << endl;
	out << "NUMBER         = " << info.num << endl;
	out << "SEGMENT        = " << info.seg << endl;
	out << "START          = " << info.start << endl;
	out << "STOP           = " << info.stop << endl;

	for (int i=1; i<(int)info.sclef.size(); i++) {
		out << "TRACK " << i << ":" << endl;
		if (info.sclef[i].isValid()) {
			out << "   START CLEF    = " << infile.token(info.sclef[i].x, info.sclef[i].y)       << endl;
		}
		if (info.skeysig[i].isValid()) {
			out << "   START KEYSIG  = " << infile.token(info.skeysig[i].x, info.skeysig[i].y)   << endl;
		}
		if (info.skey[i].isValid()) {
			out << "   START KEY     = " << infile.token(info.skey[i].x, info.skey[i].y)         << endl;
		}
		if (info.stimesig[i].isValid()) {
			out << "   START TIMESIG = " << infile.token(info.stimesig[i].x, info.stimesig[i].y) << endl;
		}
		if (info.smet[i].isValid()) {
			out << "   START MET     = " << infile.token(info.smet[i].x, info.smet[i].y)         << endl;
		}
		if (info.stempo[i].isValid()) {
			out << "   START TEMPO   = " << infile.token(info.stempo[i].x, info.stempo[i].y)     << endl;
		}
	
		if (info.eclef[i].isValid()) {
			out << "   END CLEF    = " << infile.token(info.eclef[i].x, info.eclef[i].y)       << endl;
		}
		if (info.ekeysig[i].isValid()) {
			out << "   END KEYSIG  = " << infile.token(info.ekeysig[i].x, info.ekeysig[i].y)   << endl;
		}
		if (info.ekey[i].isValid()) {
			out << "   END KEY     = " << infile.token(info.ekey[i].x, info.ekey[i].y)         << endl;
		}
		if (info.etimesig[i].isValid()) {
			out << "   END TIMESIG = " << infile.token(info.etimesig[i].x, info.etimesig[i].y) << endl;
		}
		if (info.emet[i].isValid()) {
			out << "   END MET     = " << infile.token(info.emet[i].x, info.emet[i].y)         << endl;
		}
		if (info.etempo[i].isValid()) {
			out << "   END TEMPO   = " << infile.token(info.etempo[i].x, info.etempo[i].y)     << endl;
		}
	}

	return out;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// Tool_myank::initialize -- extract time signature lines for
//    each **kern spine in file.
//

void Tool_myank::initialize(HumdrumFile& infile) {
	// handle basic options:
	if (getBoolean("author")) {
		m_free_text << "Written by Craig Stuart Sapp, "
			  << "craig@ccrma.stanford.edu, December 2010" << endl;
		return;
	} else if (getBoolean("version")) {
		m_free_text << getCommand() << ", version: 26 December 2010" << endl;
		m_free_text << "compiled: " << __DATE__ << endl;
		return;
	} else if (getBoolean("help")) {
		usage(getCommand());
		return;
	} else if (getBoolean("example")) {
		example();
		return;
	}

	debugQ        = getBoolean("debug");
	inlistQ       = getBoolean("inlist");
	outlistQ      = getBoolean("outlist");
	verboseQ      = getBoolean("verbose");
	maxQ          = getBoolean("max");
	minQ          = getBoolean("min");

	invisibleQ    = !getBoolean("not-invisible");
	instrumentQ   =  getBoolean("instrument");
	nolastbarQ    =  getBoolean("noendbar");
	markQ         =  getBoolean("mark");
	doubleQ       =  getBoolean("mdsep");
	barnumtextQ   =  getBoolean("bar-number-text");
	sectionCountQ =  getBoolean("section-count");
	Section       =  getInteger("section");

	if (!Section) {
		if (!(getBoolean("measures") || markQ)) {
			// if -m option is not given, then --mark option presumed
			markQ = 1;
			// cerr << "Error: the -m option is required" << endl;
			// exit(1);
		}
	}

}



////////////////////////
//
// Tool_myank::processFile --
//

void Tool_myank::processFile(HumdrumFile& infile) {
	if (sectionCountQ) {
		int sections = getSectionCount(infile);
		m_humdrum_text << sections << endl;
		return;
	}

	getMetStates(metstates, infile);
	getMeasureStartStop(MeasureInList, infile);

	string measurestring = getString("measure");
	if (markQ) {
		stringstream mstring;
		getMarkString(mstring, infile); 
		measurestring = mstring.str();
		if (debugQ) {
			m_free_text << "MARK STRING: " << mstring.str() << endl;
		}
	} else if (Section) {
		string sstring;
		getSectionString(sstring, infile, Section);
		measurestring = sstring;
	}
	if (debugQ) {
		m_free_text << "MARK MEASURES: " << measurestring << endl;
	}
	
	// expand to multiple measures later.
	expandMeasureOutList(MeasureOutList, MeasureInList, infile, 
			measurestring);

	if (inlistQ) {
		m_free_text << "INPUT MEASURE MAP: " << endl;
		for (int i=0; i<(int)MeasureInList.size(); i++) {
			m_free_text << MeasureInList[i];
		}
	}
	if (outlistQ) {
		m_free_text << "OUTPUT MEASURE MAP: " << endl;
		for (int i=0; i<(int)MeasureOutList.size(); i++) {
			m_free_text << MeasureOutList[i];
		}
	}

	if (MeasureOutList.size() == 0) {
		// disallow processing files with no barlines
		return;
	}

	myank(infile, MeasureOutList);
}



////////////////////////
//
// Tool_myank::getMetStates --  Store the current *met for every token
// in the score, keeping track of meter without metric symbols.
//

void Tool_myank::getMetStates(vector<vector<MyCoord> >& metstates, 
		HumdrumFile& infile) {
	vector<MyCoord> current;
	current.resize(infile.getMaxTrack()+1);
	metstates.resize(infile.getLineCount());
	HumRegex hre;

	int track;
	for (int i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isInterpretation()) {
			for (int j=0; j<infile[i].getFieldCount(); j++) {
				track = infile.token(i, j)->getTrack();
				if (hre.search(infile.token(i, j), R"(^\*met\([^\)]+\))")) {
					current[track].x = i;
					current[track].y = j;
				} else if (hre.search(infile.token(i, j), R"(^\*M\d+\d+)")) {
					current[track] = getLocalMetInfo(infile, i, track);   
				}
			}
		}

		// metstates[i].resize(infile[i].getFieldCount());
		// for (j=0; j<infile[i].getFieldCount(); j++) {
		//    track = infile.token(i, j)->getTrack();
		//    metstates[i][j] = current[track];
		// }
		metstates[i].resize(infile.getMaxTrack()+1);
		for (int j=1; j<=infile.getMaxTrack(); j++) {
			metstates[i][j] = current[j];
		}
	}

	if (debugQ) {
		for (int i=0; i<infile.getLineCount(); i++) {
			for (int j=1; j<(int)metstates[i].size(); j++) {
				if (metstates[i][j].x < 0) {
					m_humdrum_text << ".";
				} else {
					m_humdrum_text << infile.token(metstates[i][j].x, metstates[i][j].y);
				}
				m_humdrum_text << "\t";
			}
			m_humdrum_text << infile[i] << endl;
		}

	}
}



//////////////////////////////
//
// Tool_myank::getLocalMetInfo -- search in the non-data region indicated by the 
// input row for a *met entry in the input track.  Return empty 
// value if none found.
//

MyCoord Tool_myank::getLocalMetInfo(HumdrumFile& infile, int row, int track) {
	MyCoord output;
	int startline = -1;
	int stopline = -1;
	int i = row;
	int j;
	int xtrac;
	HumRegex hre;

	while (i>=0) {
		if (infile[i].isData()) {
			startline = i+1;
			break;
		}
		i--;
	}
	if (startline < 0) {
		startline = 0;
	}
	i = row; 
	while (i<infile.getLineCount()){ 
		if (infile[i].isData()) {
			stopline = i-1;
			break;
		}
		i++;
	}
	if (stopline >= infile.getLineCount()) {
		stopline = infile.getLineCount()-1;
	}
	for (i=startline; i<=stopline; i++) {
		if (!infile[i].isInterpretation()) {
			continue;
		}
		for (j=0; j<infile[i].getFieldCount(); j++) {
			xtrac = infile.token(i, j)->getTrack();
			if (track != xtrac) {
				continue;
			}
			if (hre.search(infile.token(i, j), R"(^\*met\([^\)]+\))")) {
				output.x = i;
				output.x = j;
			}
		}
	}
	return output;
}



//////////////////////////////
//
// Tool_myank::getMarkString -- return a list of measures which contain marked
//    notes (primarily from search matches).
// This function scans for reference records in this form:
// !!!RDF**kern: @= matched note
// or
// !!!RDF**kern: i= marked note
// If it finds any lines like that, it will extract the character before
// the equals sign, and scan for it in the **kern data in the file.
// any measure which contains such a mark will be stored in the output
// string.
//

void Tool_myank::getMarkString(ostream& out, HumdrumFile& infile)  {
	string mchar; // list of characters which are marks
	char target;
	HumRegex hre;
	for (int i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isReference()) {
			continue;
		}
		if (hre.search(infile.token(i, 0), 
				R"(!!!RDF\*\*kern\s*:\s*([^=])\s*=\s*match)", "i")) {
			target = hre.getMatch(1)[0];
			mchar.push_back(target);
		} else if (hre.search(infile.token(i, 0), 
				R"(!!!RDF\*\*kern\s*:\s*([^=])\s*=\s*mark)", "i")) {
			target = hre.getMatch(1)[0];
			mchar.push_back(target);
		}
	}

	if (debugQ) {
		for (int i=0; i<(int)mchar.size(); i++) {
			m_free_text << "\tMARK CHARCTER: " << mchar[i] << endl;
		}
	}

	if (mchar.size() == 0) {
		return;
	}

	// now search for measures which contains any of those character
	// in **kern data:
	int curmeasure = 0;
	int inserted = 0;
	int hasmark = 0;
	string str;
	for (int i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isBarline()) {
			if (hre.search(infile.token(i, 0), "^=.*?(\\d+)", "")) {
				curmeasure = stoi(hre.getMatch(1));
				hasmark = 0;
			}
		}
		if (hasmark) {
			continue;
		}
		if (!infile[i].isData()) {
			continue;
		}
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			if (infile.token(i, j)->isKern()) {
				int k=0;
				str = *infile.token(i, j);
				while (str[k] != '\0') {
					for (int m=0; m<(int)mchar.size(); m++) {
						if (str[k] == mchar[m]) {
							if (inserted) {
							   out << ',';
							} else {
							   inserted++;
							}
							out << curmeasure;
							hasmark = 1;
							goto outerforloop;
						}
					}
					k++;
				}
			}
		}
outerforloop: ;
	}
}



//////////////////////////////
//
// Tool_myank::myank -- yank the specified measures.
//

void Tool_myank::myank(HumdrumFile& infile, vector<MeasureInfo>& outmeasures) {
	if (outmeasures.size() > 0) {
		printStarting(infile);
	}

	int lastline = -1;
	int h, i, j;
	int counter;
	int printed = 0;
	int mcount = 0;
	int measurestart = 1;
	int datastart = 0;
	int bartextcount = 0;
	for (h=0; h<(int)outmeasures.size(); h++) {
		measurestart = 1;
		printed = 0;
		counter = 0;
		if (debugQ) {
			m_humdrum_text << "!! =====================================\n";
			m_humdrum_text << "!! processing " << outmeasures[h].num << endl;
		}
		if (h > 0) {
			reconcileSpineBoundary(infile, outmeasures[h-1].stop,
				outmeasures[h].start);
		} else {
			reconcileStartingPosition(infile, outmeasures[0].start);
		}
		for (i=outmeasures[h].start; i<outmeasures[h].stop; i++) {
			counter++;
			if ((!printed) && ((mcount == 0) || (counter == 2))) {
				if ((datastart == 0) && outmeasures[h].num == 0) {
					// not ideal setup...
					datastart = 1;
				} else{ 
					adjustGlobalInterpretations(infile, i, outmeasures, h);
					printed = 1;
				}
			}
			if (infile[i].isData() && (mcount == 0)) {
				mcount++;
			}
			if (infile[i].isBarline()) {
				mcount++;
			}
			if ((mcount == 1) && invisibleQ && infile[i].isBarline()) {
				printInvisibleMeasure(infile, i);
				measurestart = 0;
				if ((bartextcount++ == 0) && infile[i].isBarline()) {
					int barline = 0;
					sscanf(infile.token(i, 0)->c_str(), "=%d", &barline);
					if (barnumtextQ && (barline > 0)) {
						m_humdrum_text << "!!LO:TX:Z=20:X=-90:t=" << barline << endl;
					}
				}
			} else if (doubleQ && measurestart) {
				printDoubleBarline(infile, i);
				measurestart = 0;
			} else {
				m_humdrum_text << infile[i] << "\n";
				if (barnumtextQ && (bartextcount++ == 0) && infile[i].isBarline()) {
					int barline = 0;
					sscanf(infile.token(i, 0)->c_str(), "=%d", &barline);
					if (barline > 0) {
						m_humdrum_text << "!!LO:TX:Z=20:X=-25:t=" << barline << endl;
					}
				}
			}
			lastline = i;
		}
	}

	HumRegex hre;
	string token;
	int lasti;
	if (outmeasures.size() > 0) {
		lasti = outmeasures.back().stop;
	} else {
		lasti = -1;
	}
	if ((!nolastbarQ) &&  (lasti >= 0) && infile[lasti].isBarline()) {
		for (j=0; j<infile[lasti].getFieldCount(); j++) {
			token = *infile.token(lasti, j);
			hre.replaceDestructive(token, "", "\\d+");
			if (doubleQ) {
				if (hre.search(token, "=(.+)")) {
					// don't add double barline, there is already
					// some style on the barline
				} else {
					// add a double barline
					hre.replaceDestructive(token, "||", "$");
				}
			}
			m_humdrum_text << token;
			if (j < infile[lasti].getFieldCount() - 1) {
				m_humdrum_text << '\t';
			}
		}
		m_humdrum_text << '\n';
	}

	if (debugQ) {
		m_free_text << "PROCESSING ENDING" << endl;
	}

	if (lastline >= 0) {
		//printEnding(infile, lastline);
		printEnding(infile, outmeasures.back().stop, lasti);
	}

}



//////////////////////////////
//
// Tool_myank::adjustGlobalInterpretations --
//

void Tool_myank::adjustGlobalInterpretations(HumdrumFile& infile, int ii,
		vector<MeasureInfo>& outmeasures, int index) {

	if (index <= 0) {
		adjustGlobalInterpretationsStart(infile, ii, outmeasures, index);
		return;
	}

	// the following lines will not work when non-contiguous measures are
	// elided.
	//   if (!infile[ii].isInterpretation()) { 
	//      return;
	//   }

	int clefQ    = 0;
	int keysigQ  = 0;
	int keyQ     = 0;
	int timesigQ = 0;
	int metQ     = 0;
	int tempoQ   = 0;

	int x, y;
	int xo, yo;

	int tracks = infile.getMaxTrack();

	// these lines may cause bugs, but they get rid of zeroth measure
	// problem.
// ggg
//   if ((outmeasures.size() > 1) && (outmeasures[index-1].num == 0)) {
//      return;
//   }
//   if ((outmeasures.size() > 0) && (outmeasures[index].num == 0)) {
//      return;
//   }

	for (int i=1; i<=tracks; i++) {
		if (!clefQ && (outmeasures[index].sclef.size() > 0)) {
			x  = outmeasures[index].sclef[i].x;
			y  = outmeasures[index].sclef[i].y;
			xo = outmeasures[index-1].eclef[i].x;
			yo = outmeasures[index-1].eclef[i].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					clefQ = 1;
				}
			}
		}

		if (!keysigQ && (outmeasures[index].skeysig.size() > 0)) {
			x  = outmeasures[index].skeysig[i].x;
			y  = outmeasures[index].skeysig[i].y;
			xo = outmeasures[index-1].ekeysig[i].x;
			yo = outmeasures[index-1].ekeysig[i].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					keysigQ = 1;
				}
			}
		}

		if (!keyQ && (outmeasures[index].skey.size() > 0)) {
			x  = outmeasures[index].skey[i].x;
			y  = outmeasures[index].skey[i].y;
			xo = outmeasures[index-1].ekey[i].x;
			yo = outmeasures[index-1].ekey[i].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					keyQ = 1;
				}
			}
		}

		if (!timesigQ && (outmeasures[index].stimesig.size() > 0)) {
			x  = outmeasures[index].stimesig[i].x;
			y  = outmeasures[index].stimesig[i].y;
			xo = outmeasures[index-1].etimesig[i].x;
			yo = outmeasures[index-1].etimesig[i].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					timesigQ = 1;
				}
			}
		}

		if (!metQ && (outmeasures[index].smet.size() > 0)) {
			x  = outmeasures[index].smet[i].x;
			y  = outmeasures[index].smet[i].y;
			xo = outmeasures[index-1].emet[i].x;
			yo = outmeasures[index-1].emet[i].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					metQ = 1;
				}
			}
		}

		if (!tempoQ && (outmeasures[index].stempo.size() > 0)) {
			x  = outmeasures[index].stempo[i].x;
			y  = outmeasures[index].stempo[i].y;
			xo = outmeasures[index-1].etempo[i].x;
			yo = outmeasures[index-1].etempo[i].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					tempoQ = 1;
				}
			}
		}
	}

	int track;

	if (clefQ) {
		for (int i=0; i<infile[ii].getFieldCount(); i++) {
			track = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].sclef[track].x;
			y  = outmeasures[index].sclef[track].y;
			xo = outmeasures[index-1].eclef[track].x;
			yo = outmeasures[index-1].eclef[track].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					m_humdrum_text << infile.token(x, y);
				} else {
					m_humdrum_text << "*";
				}
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}

	if (keysigQ) {
		for (int i=0; i<infile[ii].getFieldCount(); i++) {
			track = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].skeysig[track].x;
			y  = outmeasures[index].skeysig[track].y;
			xo = outmeasures[index-1].ekeysig[track].x;
			yo = outmeasures[index-1].ekeysig[track].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					m_humdrum_text << infile.token(x, y);
				} else {
					m_humdrum_text << "*";
				}
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}

	if (keyQ) {
		for (int i=0; i<infile[ii].getFieldCount(); i++) {
			track = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].skey[track].x;
			y  = outmeasures[index].skey[track].y;
			xo = outmeasures[index-1].ekey[track].x;
			yo = outmeasures[index-1].ekey[track].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					m_humdrum_text << infile.token(x, y);
				} else {
					m_humdrum_text << "*";
				}
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}

	if (timesigQ) {
		for (int i=0; i<infile[ii].getFieldCount(); i++) {
			track = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].stimesig[track].x;
			y  = outmeasures[index].stimesig[track].y;
			xo = outmeasures[index-1].etimesig[track].x;
			yo = outmeasures[index-1].etimesig[track].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					m_humdrum_text << infile.token(x, y);
				} else {
					m_humdrum_text << "*";
				}
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}

	if (metQ) {
		for (int i=0; i<infile[ii].getFieldCount(); i++) {
			track = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].smet[track].x;
			y  = outmeasures[index].smet[track].y;
			xo = outmeasures[index-1].emet[track].x;
			yo = outmeasures[index-1].emet[track].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					m_humdrum_text << infile.token(x, y);
				} else {
					m_humdrum_text << "*";
				}
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}

	if (tempoQ) {
		for (int i=0; i<infile[ii].getFieldCount(); i++) {
			track = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].stempo[track].x;
			y  = outmeasures[index].stempo[track].y;
			xo = outmeasures[index-1].etempo[track].x;
			yo = outmeasures[index-1].etempo[track].y;
			if ((x>=0)&&(y>=0)&&(xo>=0)&&(yo>=0)) {
				if (*infile.token(x, y) != *infile.token(xo, yo)) {
					m_humdrum_text << infile.token(x, y);
				} else {
					m_humdrum_text << "*";
				}
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}

}



//////////////////////////////
//
// Tool_myank::adjustGlobalInterpretationsStart --
//

void Tool_myank::adjustGlobalInterpretationsStart(HumdrumFile& infile, int ii,
		vector<MeasureInfo>& outmeasures, int index) {
	if (index != 0) {
		cerr << "Error in adjustGlobalInterpetationsStart" << endl;
		exit(1);
	}

	int i;

	int clefQ    = 0;
	int keysigQ  = 0;
	int keyQ     = 0;
	int timesigQ = 0;
	int metQ     = 0;
	int tempoQ   = 0;

	int x, y;

	// ignore the zeroth measure
	// (may not be proper).
// ggg
	if (outmeasures[index].num == 0) {
		return;
	}

	int tracks = infile.getMaxTrack();

	for (i=1; i<=tracks; i++) {

		if (!clefQ) {
			x  = outmeasures[index].sclef[i].x;
			y  = outmeasures[index].sclef[i].y;

			if ((x>=0)&&(y>=0)) {
				clefQ = 1;
			}
		}

		if (!keysigQ) {
			x  = outmeasures[index].skeysig[i].x;
			y  = outmeasures[index].skeysig[i].y;
			if ((x>=0)&&(y>=0)) {
				keysigQ = 1;
			}
		}

		if (!keyQ) {
			x  = outmeasures[index].skey[i].x;
			y  = outmeasures[index].skey[i].y;
			if ((x>=0)&&(y>=0)) {
				keyQ = 1;
			}
		}

		if (!timesigQ) {
			x  = outmeasures[index].stimesig[i].x;
			y  = outmeasures[index].stimesig[i].y;
			if ((x>=0)&&(y>=0)) {
				timesigQ = 1;
			}
		}

		if (!metQ) {
			x  = outmeasures[index].smet[i].x;
			y  = outmeasures[index].smet[i].y;
			if ((x>=0)&&(y>=0)) {
				metQ = 1;
			}
		}

		if (!tempoQ) {
			x  = outmeasures[index].stempo[i].x;
			y  = outmeasures[index].stempo[i].y;
			if ((x>=0)&&(y>=0)) {
				tempoQ = 1;
			}
		}
	}

	int ptrack;

	if (clefQ) {
		for (i=0; i<infile[ii].getFieldCount(); i++) {
			ptrack = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].sclef[ptrack].x;
			y  = outmeasures[index].sclef[ptrack].y;
			if ((x>=0)&&(y>=0)) {
				m_humdrum_text << infile.token(x, y);
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}

	if (keysigQ) {
		for (i=0; i<infile[ii].getFieldCount(); i++) {
			ptrack = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].skeysig[ptrack].x;
			y  = outmeasures[index].skeysig[ptrack].y;
			if ((x>=0)&&(y>=0)) {
				m_humdrum_text << infile.token(x, y);
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}

	if (keyQ) {
		for (i=0; i<infile[ii].getFieldCount(); i++) {
			ptrack = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].skey[ptrack].x;
			y  = outmeasures[index].skey[ptrack].y;
			if ((x>=0)&&(y>=0)) {
				m_humdrum_text << infile.token(x, y);
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}

	if (timesigQ) {
		for (i=0; i<infile[ii].getFieldCount(); i++) {
			ptrack = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].stimesig[ptrack].x;
			y  = outmeasures[index].stimesig[ptrack].y;
			if ((x>=0)&&(y>=0)) {
				m_humdrum_text << infile.token(x, y);
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}
	if (metQ) {
		for (i=0; i<infile[ii].getFieldCount(); i++) {
			ptrack = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].smet[ptrack].x;
			y  = outmeasures[index].smet[ptrack].y;
			if ((x>=0)&&(y>=0)) {
				m_humdrum_text << infile.token(x, y);
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}

	if (tempoQ) {
		for (i=0; i<infile[ii].getFieldCount(); i++) {
			ptrack = infile.token(ii, i)->getTrack();
			x  = outmeasures[index].stempo[ptrack].x;
			y  = outmeasures[index].stempo[ptrack].y;
			if ((x>=0)&&(y>=0)) {
				m_humdrum_text << infile.token(x, y);
			} else {
				m_humdrum_text << "*";
			}
			if (i < infile[ii].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
		}
		m_humdrum_text << "\n";
	}
}



//////////////////////////////
//
// Tool_myank::printDoubleBarline --
//

void Tool_myank::printDoubleBarline(HumdrumFile& infile, int line) {


	if (!infile[line].isBarline()) {
		m_humdrum_text << infile[line] << "\n";
		return;
	}

	HumRegex hre;
	int j;
	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (hre.search(infile.token(line, j), "(=\\d*)(.*)", "")) {
			m_humdrum_text << hre.getMatch(1);
			m_humdrum_text << "||";
		} else {
			m_humdrum_text << "=||";
		}
		if (j < infile[line].getFieldCount()-1) {
			m_humdrum_text << "\t";
		}
	}
	m_humdrum_text << "\n";

	if (barnumtextQ) {
		int barline = 0;
		sscanf(infile.token(line, 0)->c_str(), "=%d", &barline);
		if (barline > 0) {
			m_humdrum_text << "!!LO:TX:Z=20:X=-25:t=" << barline << endl;
		}
	}

}



//////////////////////////////
//
// Tool_myank::printInvisibleMeasure --
//

void Tool_myank::printInvisibleMeasure(HumdrumFile& infile, int line) {
	if (!infile[line].isBarline()) {
		m_humdrum_text << infile[line] << "\n";
		return;
	}

	HumRegex hre;
	int j;
	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (infile.token(line, j)->find('-') != string::npos) {
			m_humdrum_text << infile.token(line, j);
			if (j < infile[line].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
			continue;
		}
		if (hre.search(infile.token(line, j), "(=\\d*)(.*)", "")) {
			m_humdrum_text << hre.getMatch(1);
			m_humdrum_text << "-";
			m_humdrum_text << hre.getMatch(2);
		} else {
			m_humdrum_text << infile.token(line, j); 
		}
		if (j < infile[line].getFieldCount()-1) {
			m_humdrum_text << "\t";
		}
	}
	m_humdrum_text << "\n";
}



//////////////////////////////
//
// Tool_myank::reconcileSpineBoundary -- merge spines correctly between segments.
//    will not be able to handle all permutations of spine manipulators.
//    So don't expect exotic manipulators to work...
//

void Tool_myank::reconcileSpineBoundary(HumdrumFile& infile, int index1, int index2) {

	if (debugQ) {
		m_humdrum_text << "RECONCILING LINES " << index1+1 << " and " << index2+1 << endl;
		m_humdrum_text << "FIELD COUNT OF " << index1+1 << " is " 
			  << infile[index1].getFieldCount() << endl;
		m_humdrum_text << "FIELD COUNT OF " << index2+1 << " is " 
			  << infile[index2].getFieldCount() << endl;
	}

	// check to see if any changes need reconciling; otherwise, exit function
	int i, j; 
	if (infile[index1].getFieldCount() == infile[index2].getFieldCount()) {
		int same = 1;
		for (i=0; i<infile[index1].getFieldCount(); i++) {
			if (infile.token(index1,i)->getSpineInfo() != infile.token(index2, i)->getSpineInfo()) {
				same = 0;
			}
		}
		if (same != 0) {
			return;
		}
	}

	// handle splits all at once
	string buff1;
	string buff2;

	vector<int> splits(infile[index1].getFieldCount());
	fill(splits.begin(), splits.end(), 0);

	int hassplit = 0;
	for (i=0; i<infile[index1].getFieldCount(); i++) {
		buff1 = "(";
		buff1 += infile.token(index1, i)->getSpineInfo();
		buff1 += ")";
		buff2 = buff1;
		buff1 += "a";
		buff2 += "b";
		for (j=0; j<infile[index2].getFieldCount()-1; j++) {
			if ((buff1 == infile.token(index2, j)->getSpineInfo()
					&& (buff2 == infile.token(index2,j+1)->getSpineInfo()))) {
				splits[i] = 1;
				hassplit++;
			}
		}
	}

	if (hassplit) {
		for (i=0; i<(int)splits.size(); i++) {
			if (splits[i]) {
				m_humdrum_text << "*^";
			} else {
				m_humdrum_text << '*';
			}
			if (i < (int)splits.size()-1) {
				m_humdrum_text << '\t';
			}
		}
		m_humdrum_text << '\n';
	}

	// make splits cumulative;
	//for (i=1; i<(int)splits.size(); i++) {
	//   splits[i] += splits[i-1];
	//}
  
	HumRegex hre1;
	HumRegex hre2;
	// handle joins one at a time, only look for binary joins at the moment.
	// assuming that no *x has been used to mix the voices up.
	for (i=0; i<infile[index1].getFieldCount()-1; i++) {
		if (!hre1.search(infile.token(index1, i)->getSpineInfo(), "\\((.*)\\)a")) {
			continue;
		}
		if (!hre2.search(infile.token(index1, i+1)->getSpineInfo(), "\\((.*)\\)b")) {
			continue;
		}
		if (hre1.getMatch(1) != hre2.getMatch(1)) {
			// spines are not split from same source
			continue;
		}

		// found an "a" and "b" portion of a spine split, now search
		// through the target line for a joint of those two sub-spines
		for (j=0; j<infile[index2].getFieldCount(); j++) {
			if (infile.token(index2, j)->getSpineInfo() != hre1.getMatch(1)) {
				continue;
			}
			// found a simple binary spine join: emmit a spine manipulator line
			printJoinLine(splits, i, 2);
		}
	}
	
	// handle *x switches, not perfect since ordering might need to be
	// handled between manipulators...
	
}



//////////////////////////////
//
// Tool_myank::printJoinLine -- count is currently ignored, but may in the future
//    allow for more than two spines to join at the same time.
//

void Tool_myank::printJoinLine(vector<int>& splits, int index, int count) {
	int i;
	for (i=0; i<(int)splits.size(); i++) {
		if (i == index) {
			m_humdrum_text << "*v\t*v";
			i+=count-1;
		} else {
			m_humdrum_text << "*";
		}
		if (i<(int)splits.size()-1) {
			m_humdrum_text << "\t";
		}
	}
	m_humdrum_text << "\n";

	// merge splits by one element
	for (i=index+1; i<(int)splits.size()-1; i++) {
		splits[i] = splits[i+1];
	}
	splits.resize(splits.size()-1);
}



//////////////////////////////
//
// Tool_myank::reconcileStartingPosition -- merge spines from start of data and 
//    first measure in output.
//

void Tool_myank::reconcileStartingPosition(HumdrumFile& infile, int index2) {
	int i;
	for (i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isInterpretation()) {
			reconcileSpineBoundary(infile, i, index2);
			break;
		}
	}
}

  

//////////////////////////////
//
// Tool_myank::printStarting -- print header information before start of data.
//

void Tool_myank::printStarting(HumdrumFile& infile) {
	int i, j;
	int exi = -1;
	for (i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isInterpretation()) {
			// the first interpretation is the exclusive one
			m_humdrum_text << infile[i] << "\n";
			exi = i;
			break;
		}
		m_humdrum_text << infile[i] << "\n";
	}

	int hasI = 0;

	if (instrumentQ) {
		// print any tandem interpretations which start with *I found
		// at the start of the data before measures, notes, or any
		// spine manipulator lines
		for (i=exi+1; i<infile.getLineCount(); i++) {
			if (infile[i].isData()) {
				break;
			}
			if (infile[i].isBarline()) {
				break;
			}
			if (!infile[i].isInterpretation()) {
				continue;
			}
			if (infile[i].isManipulator()) {
				break;
			}
			hasI = 0;
			for (j=0; j<infile[i].getFieldCount(); j++) {
				if (infile.token(i, j)->compare(0, 2, "*I") == 0) {
					hasI = 1;
					break;
				}
			}
			if (hasI) {
				for (j=0; j<infile[i].getFieldCount(); j++) {
					if (infile.token(i, j)->compare(0, 2, "*I") == 0) {
						m_humdrum_text << infile.token(i, j);
					} else {
						m_humdrum_text << "*";
					}
					if (j < infile[i].getFieldCount() - 1) {
						m_humdrum_text << "\t";
					}
				}
				m_humdrum_text << "\n";
			}
		}
	}

}



//////////////////////////////
//
// Tool_myank::printEnding -- print the measure
//

void Tool_myank::printEnding(HumdrumFile& infile, int lastline, int adjlin) {
	if (debugQ) {
		m_humdrum_text << "IN printEnding" << endl;
	}
	int ending = -1;
	int marker = -1;
	int i;
	for (i=infile.getLineCount()-1; i>=0; i--) {
		if (infile[i].isInterpretation() && (ending <0) 
				&& (*infile.token(i, 0) == "*-")) {
			ending = i;
		}
		if (infile[i].isData()) {
			marker = i+1;
			break;
		}
		if (infile[i].isBarline()) {
			marker = i+1;
			break;
		}
	}

	if (ending >= 0) {
		reconcileSpineBoundary(infile, adjlin, ending);
	}
	
	int startline  = ending;
	if (marker >= 0) {
		// capture any comment which occur after the last measure
		// line in the data.
		startline = marker;
	}

	// reconcileSpineBoundary(infile, lastline, startline);

	if (startline >= 0) {
		for (i=startline; i<infile.getLineCount(); i++) {
			m_humdrum_text << infile[i] << "\n";
		}
	}

}



//////////////////////////////
//
// Tool_myank::getMeasureStartStop --  Get a list of the (numbered) measures in the
//    input file, and store the start/stop lines for those measures.
//    All data before the first numbered measure is in measure 0.  
//    although, if the first measure is not labeled, then ...
//

void Tool_myank::getMeasureStartStop(vector<MeasureInfo>& measurelist, HumdrumFile& infile) {
	measurelist.reserve(infile.getLineCount());
	measurelist.resize(0);

	MeasureInfo current;
	int i, ii;
	int lastend = -1;
	int dataend = -1;
	int barnum1 = -1;
	int barnum2 = -1;
	HumRegex hre;

	insertZerothMeasure(measurelist, infile);

	for (i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isInterpretation()) {
			if (*infile.token(i, 0) == "*-") {
				dataend = i;
				break;
			}
		}
		if (!infile[i].isBarline()) {
			continue;
		}
		//if (!hre.search(infile.token(i, 0), "^=.*(\\d+)")) {
		//   continue;
		//}
		//barnum1 = stoi(hre.getMatch(1));
		if (!sscanf(infile.token(i, 0)->c_str(), "=%d", &barnum1)) {
			continue;
		}
		current.clear();
		current.start = i;
		current.num   = barnum1;
		for (ii=i+1; ii<infile.getLineCount(); ii++) {
			if (!infile[ii].isBarline()) {
				continue;
			}
			//if (hre.search(infile.token(ii, 0), "^=.*(\\d+)")) {
			//   barnum2 = stoi(hre.getMatch(1));
			//   current.stop = ii;
			//   lastend = ii;
			//   i = ii - 1;
			//   measurelist.push_back(current);
			//   break;
			//}
			if (hre.search(infile.token(ii, 0), "=[^\\d]*(\\d+)")) {
			// if (sscanf(infile.token(ii, 0), "=%d", &barnum2)) {
				barnum2 = stoi(hre.getMatch(1));
				current.stop = ii;
				lastend = ii;
				i = ii - 1;
				current.file = &infile;
				measurelist.push_back(current);
				break;
			} else {
				if (atEndOfFile(infile, ii)) {
					break;
				}
			}
		}
	}

	int lastdata    = -1;   // last line in file with data
	int lastmeasure = -1;   // last line in file with measure

	for (i=infile.getLineCount()-1; i>=0; i--) {
		if ((lastdata < 0) && infile[i].isData()) {
			lastdata = i; 
		}
		if ((lastmeasure < 0) && infile[i].isBarline()) {
			lastmeasure = i; 
		}
		if ((lastmeasure >= 0) && (lastdata >= 0)) {
			break;
		}
	}

	if (lastmeasure < lastdata) {
		// no final barline, so set to ignore
		lastmeasure = -1;
		lastdata    = -1;
	}
 
	if ((barnum2 >= 0) && (lastend >= 0) && (dataend >= 0)) {
		current.clear();
		current.num = barnum2;
		current.start = lastend;
		current.stop = dataend;
		if (lastmeasure > lastdata) {
			current.stop = lastmeasure;
		}
		current.file = &infile;
		measurelist.push_back(current);
	}


}



//////////////////////////////
//
// Tool_myank::getSectionCount -- Count the number of sections in a file according to
//     JRP rules: sections are defined by double barlines. There may be some
//     corner cases to consider.
//

int Tool_myank::getSectionCount(HumdrumFile& infile) {
	int i;
	int count = 0;
	int dataQ = 0;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!dataQ && infile[i].isData()) {
			dataQ = 1;
			count++;
			continue;
		}
		if (infile[i].isBarline()) {
			if (infile.token(i, 0)->find("||") != string::npos) {
				dataQ = 0;
			}
		}
	}
	return count;
}



//////////////////////////////
//
// Tool_myank::getSectionString -- return the measure range of a section.
//

void Tool_myank::getSectionString(string& sstring, HumdrumFile& infile, int sec) {
	int i;
	int first = -1;
	int second = -1;
	int barnum = 0;
	int count = 0;
	int dataQ = 0;
	HumRegex hre;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!dataQ && infile[i].isData()) {
			dataQ = 1;
			count++;
			if (count == sec) {
				first = barnum;
			} else if (count == sec+1) {
				second = barnum - 1;
			}
			continue;
		}
		if (infile[i].isBarline()) {
			if (infile.token(i, 0)->find("||") != string::npos) {
				dataQ = 0;
			}
			if (hre.search(infile.token(i, 0), "(\\d+)")) {
				barnum = hre.getMatchInt(1);
			} 
		}
	}
	if (second < 0) {
		second = barnum;
	}
	sstring = to_string(first);
	sstring += "-";
	sstring += to_string(second);
}



//////////////////////////////
//
// Tool_myank::atEndOfFile --
//

int Tool_myank::atEndOfFile(HumdrumFile& infile, int line) {
	int i;
	for (i=line+1; i<infile.getLineCount(); i++) {
		if (infile[i].isData()) {
			return 1;
		}
	}

	return 0;
}



//////////////////////////////
//
// Tool_myank::insertZerothMeasure --
//

void Tool_myank::insertZerothMeasure(vector<MeasureInfo>& measurelist, 
		HumdrumFile& infile) {

	HumRegex hre;
	int exinterpline = -1;
	int startline = -1;
	int stopline = -1;
	int i;
	for (i=9; i<infile.getLineCount(); i++) {
		if ((exinterpline < 0) && infile[i].isInterpretation()) {
			exinterpline = i;
		}
		if ((startline < 0) && (infile[i].isData())) {
			startline = i;
		}
		if (infile[i].isBarline() && hre.search(infile.token(i, 0), "^=.*\\d+", "")) {
			stopline = i;
			break;
		}
	}

	if (exinterpline < 0) {
		// somethind weird happend, just return
		return;
	}
	if (startline < 0) {
		// no zeroth measure;
		return;
	}
	if (stopline < 0) {
		// strange situation, no measure numbers
		// consider what to do later...
		return;
	}

	MeasureInfo current;
	current.clear();
	current.num = 0;
	// current.start = startline;
	current.start = exinterpline+1;
	current.stop = stopline;
	measurelist.push_back(current);
}



//////////////////////////////
//
// Tool_myank::expandMeasureOutList -- read the measure list for the sequence of measures
//     to extract.
//

void Tool_myank::expandMeasureOutList(vector<MeasureInfo>& measureout, 
		vector<MeasureInfo>& measurein, HumdrumFile& infile, 
		const string& optionstring) {

	HumRegex hre;
	// find the largest measure number in the score
	int maxmeasure = -1;
	int minmeasure = -1;
	int i;
	for (i=0; i<(int)measurein.size(); i++) {
		if (maxmeasure < measurein[i].num) {
			maxmeasure = measurein[i].num;
		}
		if ((minmeasure == -1) || (minmeasure > measurein[i].num)) {
			minmeasure = measurein[i].num;
		}
	}
	if (maxmeasure <= 0) {
		cerr << "Error: There are no measure numbers present in the data" << endl;
		exit(1);
	}
	if (maxmeasure > 1123123) {
		cerr << "Error: ridiculusly large measure number: " << maxmeasure << endl;
		exit(1);
	}
	if (maxQ) {
		if (measurein.size() == 0) {
			m_humdrum_text << 0 << endl;
		} else {
			m_humdrum_text << maxmeasure << endl;
		}
		exit(0);
	} else if (minQ) {
		int ii;
		for (ii=0; ii<infile.getLineCount(); ii++) {
			if (infile[ii].isBarline()) {
				if (hre.search(infile.token(ii, 0), "=\\d", "")) {
					break;
				} else {
					m_humdrum_text << 0 << endl;
					exit(0);
				}
			}
			if (infile[ii].isData()) {
				m_humdrum_text << 0 << endl;
				exit(0);
			}
		}
		if (measurein.size() == 0) {
			m_humdrum_text << 0 << endl;
		} else {
			m_humdrum_text << minmeasure << endl;
		}
		exit(0);
	}

	// create reverse-lookup list
	vector<int> inmap(maxmeasure+1);
	fill(inmap.begin(), inmap.end(), -1);
	for (i=0; i<(int)measurein.size(); i++) {
		inmap[measurein[i].num] = i;
	}

	fillGlobalDefaults(infile, measurein, inmap);
	string ostring = optionstring;
	removeDollarsFromString(ostring, maxmeasure);

	if (debugQ) {
		m_free_text << "Option string expanded: " << ostring << endl;
	}

	hre.replaceDestructive(ostring, "", "\\s+", "g");  // remove any spaces between items.
	hre.replaceDestructive(ostring, "-", "--+", "g");  // remove extra dashes
	int value = 0;
	int start = 0;
	vector<MeasureInfo>& range = measureout;
	range.reserve(10000);
	value = hre.search(ostring, "^([^,]+,?)");
	while (value != 0) {
		start += value - 1;
		start += hre.getMatch(1).size();
		processFieldEntry(range, hre.getMatch(1), infile, maxmeasure,
			 measurein, inmap);
		value = hre.search(ostring.c_str() + start, "^([^,]+,?)");
	}
}



//////////////////////////////
//
// Tool_myank::fillGlobalDefaults -- keep track of the clef, key signature, key, etc.
//

void Tool_myank::fillGlobalDefaults(HumdrumFile& infile, vector<MeasureInfo>& measurein, 
		vector<int>& inmap) {
	int i, j;
	HumRegex hre;

	int tracks = infile.getMaxTrack();

	vector<MyCoord> currclef(tracks+1);
	vector<MyCoord> currkeysig(tracks+1);
	vector<MyCoord> currkey(tracks+1);
	vector<MyCoord> currtimesig(tracks+1);
	vector<MyCoord> currmet(tracks+1);
	vector<MyCoord> currtempo(tracks+1);

	MyCoord undefMyCoord;
	undefMyCoord.clear();

	fill(currclef.begin(), currclef.end(), undefMyCoord);
	fill(currkeysig.begin(), currkeysig.end(), undefMyCoord);
	fill(currkey.begin(), currkey.end(), undefMyCoord);
	fill(currtimesig.begin(), currtimesig.end(), undefMyCoord);
	fill(currmet.begin(), currmet.end(), undefMyCoord);
	fill(currtempo.begin(), currtempo.end(), undefMyCoord);

	int currmeasure = -1;
	int lastmeasure = -1;
	int datafound   = 0;
	int track;
	int thingy = 0;

	for (i=0; i<infile.getLineCount(); i++) {
		if ((currmeasure == -1) && (thingy == 0) && infile[i].isData()) {
			currmeasure = 0;
		}
		if (infile[i].isBarline()) {
			if (!hre.search(infile.token(i, 0), "(\\d+)", "")) {
				continue;
			}
			thingy = 1;

			// store state of global music values at end of measure
			if (currmeasure >= 0) {
				measurein[inmap[currmeasure]].eclef    = currclef;
				measurein[inmap[currmeasure]].ekeysig  = currkeysig;
				measurein[inmap[currmeasure]].ekey     = currkey;
				measurein[inmap[currmeasure]].etimesig = currtimesig;
				measurein[inmap[currmeasure]].emet     = currmet;
				measurein[inmap[currmeasure]].etempo   = currtempo;
			}

			lastmeasure = currmeasure;
			currmeasure = hre.getMatchInt(1);

			if (currmeasure < (int)inmap.size()) {
				// [20120818] Had to compensate for last measure being single
				// and un-numbered.
				if (inmap[currmeasure] < 0) {
					// [20111008] Had to compensate for "==85" barline
					datafound = 0;
					break;
				}
				measurein[inmap[currmeasure]].sclef    = currclef;
				measurein[inmap[currmeasure]].skeysig  = currkeysig;
				measurein[inmap[currmeasure]].skey     = currkey;
				measurein[inmap[currmeasure]].stimesig = currtimesig;
				// measurein[inmap[currmeasure]].smet     = metstates[i];
				measurein[inmap[currmeasure]].smet     = currmet;
				measurein[inmap[currmeasure]].stempo   = currtempo;
			}

			datafound   = 0;
			continue;
		}
		if (infile[i].isInterpretation()) {
			for (j=0; j<infile[i].getFieldCount(); j++) {
				if (!infile.token(i, j)->isKern()) {
					continue;
				}
				track = infile.token(i, j)->getTrack();

				if ((datafound == 0) && (lastmeasure >= 0)) {
					if (infile.token(i, j)->compare(0, 5, "*clef") == 0) {
						measurein[inmap[currmeasure]].sclef[track].x = -1;
						measurein[inmap[currmeasure]].sclef[track].y = -1;
					} else if (hre.search(infile.token(i, j), "^\\*k\\[.*\\]", "")) {
						measurein[inmap[currmeasure]].skeysig[track].x = -1;
						measurein[inmap[currmeasure]].skeysig[track].y = -1;
					} else if (hre.search(infile.token(i, j), "^\\*[A-G][#-]?:", "i")) {
						measurein[inmap[currmeasure]].skey[track].x = -1;
						measurein[inmap[currmeasure]].skey[track].y = -1;
					} else if (hre.search(infile.token(i, j), R"(^\*M\d+/\d+)")) {
						measurein[inmap[currmeasure]].stimesig[track].x = -1;
						measurein[inmap[currmeasure]].stimesig[track].y = -1;
					} else if (hre.search(infile.token(i, j), R"(^\*met\(.*\))")) {
						measurein[inmap[currmeasure]].smet[track].x = -1;
						measurein[inmap[currmeasure]].smet[track].y = -1;
					} else if (hre.search(infile.token(i, j), "^\\*MM\\d+", "i")) {
						measurein[inmap[currmeasure]].stempo[track].x = -1;
						measurein[inmap[currmeasure]].stempo[track].y = -1;
					}
				} 

				if (infile.token(i, j)->compare(0, 5, "*clef") == 0) {
					currclef[track].x = i;
					currclef[track].y = j;
					continue;
				}
				if (hre.search(infile.token(i, j), R"(^\*k\[.*\])")) {
					currkeysig[track].x = i;
					currkeysig[track].y = j;
					continue;
				}
				if (hre.search(infile.token(i, j), "^\\*[A-G][#-]?:", "i")) {
					currkey[track].x = i;
					currkey[track].y = j;
					continue;
				}
				if (hre.search(infile.token(i, j), R"(^\*M\d+/\d+)")) {
					currtimesig[track].x = i;
					currtimesig[track].y = j;
					continue;
				}
				if (hre.search(infile.token(i, j), R"(^\*met\(.*\))")) {
					currmet[track].x = i;
					currmet[track].y = j;
					continue;
				}
				if (hre.search(infile.token(i, j), R"(^\*MM[\d.]+)")) {
					currtempo[track].x = i;
					currtempo[track].y = j;
					continue;
				}

			}
		}
		if (infile[i].isData()) {
			datafound = 1;
		}
	}

	// store state of global music values at end of music
	if ((currmeasure >= 0) && (currmeasure < (int)inmap.size()) 
			&& (inmap[currmeasure] >= 0)) {
		measurein[inmap[currmeasure]].eclef    = currclef;
		measurein[inmap[currmeasure]].ekeysig  = currkeysig;
		measurein[inmap[currmeasure]].ekey     = currkey;
		measurein[inmap[currmeasure]].etimesig = currtimesig;
		measurein[inmap[currmeasure]].emet     = currmet;
		measurein[inmap[currmeasure]].etempo   = currtempo;
	}

	// go through the measure list and clean up start/end states
	for (i=0; i<(int)measurein.size()-2; i++) {

		if (measurein[i].sclef.size() == 0) {
			measurein[i].sclef.resize(tracks+1);
			fill(measurein[i].sclef.begin(), measurein[i].sclef.end(), undefMyCoord);
		}
		if (measurein[i].eclef.size() == 0) {
			measurein[i].eclef.resize(tracks+1);
			fill(measurein[i].eclef.begin(), measurein[i].eclef.end(), undefMyCoord);
		}
		if (measurein[i+1].sclef.size() == 0) {
			measurein[i+1].sclef.resize(tracks+1);
			fill(measurein[i+1].sclef.begin(), measurein[i+1].sclef.end(), undefMyCoord);
		}
		if (measurein[i+1].eclef.size() == 0) {
			measurein[i+1].eclef.resize(tracks+1);
			fill(measurein[i+1].eclef.begin(), measurein[i+1].eclef.end(), undefMyCoord);
		}
		for (j=1; j<(int)measurein[i].sclef.size(); j++) {
			if (!measurein[i].eclef[j].isValid()) {
				if (measurein[i].sclef[j].isValid()) {
					measurein[i].eclef[j] = measurein[i].sclef[j];
				}
			}
			if (!measurein[i+1].sclef[j].isValid()) {
				if (measurein[i].eclef[j].isValid()) {
					measurein[i+1].sclef[j] = measurein[i].eclef[j];
				}
			}
		}

		if (measurein[i].skeysig.size() == 0) {
			measurein[i].skeysig.resize(tracks+1);
			fill(measurein[i].skeysig.begin(), measurein[i].skeysig.end(), undefMyCoord);
		}
		if (measurein[i].ekeysig.size() == 0) {
			measurein[i].ekeysig.resize(tracks+1);
			fill(measurein[i].ekeysig.begin(), measurein[i].ekeysig.end(), undefMyCoord);
		}
		if (measurein[i+1].skeysig.size() == 0) {
			measurein[i+1].skeysig.resize(tracks+1);
			fill(measurein[i+1].skeysig.begin(), measurein[i+1].skeysig.end(), undefMyCoord);
		}
		if (measurein[i+1].ekeysig.size() == 0) {
			measurein[i+1].ekeysig.resize(tracks+1);
			fill(measurein[i+1].ekeysig.begin(), measurein[i+1].ekeysig.end(), undefMyCoord);
		}
		for (j=1; j<(int)measurein[i].skeysig.size(); j++) {
			if (!measurein[i].ekeysig[j].isValid()) {
				if (measurein[i].skeysig[j].isValid()) {
					measurein[i].ekeysig[j] = measurein[i].skeysig[j];
				}
			}
			if (!measurein[i+1].skeysig[j].isValid()) {
				if (measurein[i].ekeysig[j].isValid()) {
					measurein[i+1].skeysig[j] = measurein[i].ekeysig[j];
				}
			}
		}

		if (measurein[i].skey.size() == 0) {
			measurein[i].skey.resize(tracks+1);
			fill(measurein[i].skey.begin(), measurein[i].skey.end(), undefMyCoord);
		}
		if (measurein[i].ekey.size() == 0) {
			measurein[i].ekey.resize(tracks+1);
			fill(measurein[i].ekey.begin(), measurein[i].ekey.end(), undefMyCoord);
		}
		if (measurein[i+1].skey.size() == 0) {
			measurein[i+1].skey.resize(tracks+1);
			fill(measurein[i+1].skey.begin(), measurein[i+1].skey.end(), undefMyCoord);
		}
		if (measurein[i+1].ekey.size() == 0) {
			measurein[i+1].ekey.resize(tracks+1);
			fill(measurein[i+1].ekey.begin(), measurein[i+1].ekey.end(), undefMyCoord);
		}
		for (j=1; j<(int)measurein[i].skey.size(); j++) {
			if (!measurein[i].ekey[j].isValid()) {
				if (measurein[i].skey[j].isValid()) {
					measurein[i].ekey[j] = measurein[i].skey[j];
				}
			}
			if (!measurein[i+1].skey[j].isValid()) {
				if (measurein[i].ekey[j].isValid()) {
					measurein[i+1].skey[j] = measurein[i].ekey[j];
				}
			}
		}

		if (measurein[i].stimesig.size() == 0) {
			measurein[i].stimesig.resize(tracks+1);
			fill(measurein[i].stimesig.begin(), measurein[i].stimesig.end(), undefMyCoord);
		}
		if (measurein[i].etimesig.size() == 0) {
			measurein[i].etimesig.resize(tracks+1);
			fill(measurein[i].etimesig.begin(), measurein[i].etimesig.end(), undefMyCoord);
		}
		if (measurein[i+1].stimesig.size() == 0) {
			measurein[i+1].stimesig.resize(tracks+1);
			fill(measurein[i+1].stimesig.begin(), measurein[i+1].stimesig.end(), undefMyCoord);
		}
		if (measurein[i+1].etimesig.size() == 0) {
			measurein[i+1].etimesig.resize(tracks+1);
			fill(measurein[i+1].etimesig.begin(), measurein[i+1].etimesig.end(), undefMyCoord);
		}
		for (j=1; j<(int)measurein[i].stimesig.size(); j++) {
			if (!measurein[i].etimesig[j].isValid()) {
				if (measurein[i].stimesig[j].isValid()) {
					measurein[i].etimesig[j] = measurein[i].stimesig[j];
				}
			}
			if (!measurein[i+1].stimesig[j].isValid()) {
				if (measurein[i].etimesig[j].isValid()) {
					measurein[i+1].stimesig[j] = measurein[i].etimesig[j];
				}
			}
		}

		if (measurein[i].smet.size() == 0) {
			measurein[i].smet.resize(tracks+1);
			fill(measurein[i].smet.begin(), measurein[i].smet.end(), undefMyCoord);
		}
		if (measurein[i].emet.size() == 0) {
			measurein[i].emet.resize(tracks+1);
			fill(measurein[i].emet.begin(), measurein[i].emet.end(), undefMyCoord);
		}
		if (measurein[i+1].smet.size() == 0) {
			measurein[i+1].smet.resize(tracks+1);
			fill(measurein[i+1].smet.begin(), measurein[i+1].smet.end(), undefMyCoord);
		}
		if (measurein[i+1].emet.size() == 0) {
			measurein[i+1].emet.resize(tracks+1);
			fill(measurein[i+1].emet.begin(), measurein[i+1].emet.end(), undefMyCoord);
		}
		for (j=1; j<(int)measurein[i].smet.size(); j++) {
			if (!measurein[i].emet[j].isValid()) {
				if (measurein[i].smet[j].isValid()) {
					measurein[i].emet[j] = measurein[i].smet[j];
				}
			}
			if (!measurein[i+1].smet[j].isValid()) {
				if (measurein[i].emet[j].isValid()) {
					measurein[i+1].smet[j] = measurein[i].emet[j];
				}
			}
		}

		if (measurein[i].stempo.size() == 0) {
			measurein[i].stempo.resize(tracks+1);
			fill(measurein[i].stempo.begin(), measurein[i].stempo.end(), undefMyCoord);
		}
		if (measurein[i].etempo.size() == 0) {
			measurein[i].etempo.resize(tracks+1);
			fill(measurein[i].etempo.begin(), measurein[i].etempo.end(), undefMyCoord);
		}
		if (measurein[i+1].stempo.size() == 0) {
			measurein[i+1].stempo.resize(tracks+1);
			fill(measurein[i+1].stempo.begin(), measurein[i+1].stempo.end(), undefMyCoord);
		}
		if (measurein[i+1].etempo.size() == 0) {
			measurein[i+1].etempo.resize(tracks+1);
			fill(measurein[i+1].etempo.begin(), measurein[i+1].etempo.end(), undefMyCoord);
		}
		for (j=1; j<(int)measurein[i].stempo.size(); j++) {
			if (!measurein[i].etempo[j].isValid()) {
				if (measurein[i].stempo[j].isValid()) {
					measurein[i].etempo[j] = measurein[i].stempo[j];
				}
			}
			if (!measurein[i+1].stempo[j].isValid()) {
				if (measurein[i].etempo[j].isValid()) {
					measurein[i+1].stempo[j] = measurein[i].etempo[j];
				}
			}
		}
	}
}



//////////////////////////////
//
// Tool_myank::processFieldEntry -- 
//   3-6 expands to 3 4 5 6
//   $   expands to maximum spine track
//   $0  expands to maximum spine track
//   $1  expands to maximum spine track minus 1, etc.
//   2-$1 expands to 2 through the maximum minus one.
//   6-3 expands to 6 5 4 3
//   $2-5 expands to the maximum minus 2 down through 5.
//   Ignore negative values and values which exceed the maximum value.
//

void Tool_myank::processFieldEntry(vector<MeasureInfo>& field,
		const string& str, HumdrumFile& infile, int maxmeasure,
		vector<MeasureInfo>& inmeasures, vector<int>& inmap) {

	MeasureInfo current;

	HumRegex hre;
	string buffer = str;

	// remove any comma left at end of input string (or anywhere else)
	hre.replaceDestructive(buffer, "", ",", "g");

	if (hre.search(buffer, "^(\\d+)[a-z]?-(\\d+)[a-z]?$")) {
		int firstone = hre.getMatchInt(1);
		int lastone  = hre.getMatchInt(2);

		// limit the range to 0 to maxmeasure
		if (firstone > maxmeasure) { firstone = maxmeasure; }
		if (lastone  > maxmeasure) { lastone  = maxmeasure; }
		if (firstone < 0         ) { firstone = 0         ; }
		if (lastone  < 0         ) { lastone  = 0         ; }

		if ((firstone < 1) && (firstone != 0)) {
			cerr << "Error: range token: \"" << str << "\"" 
				  << " contains too small a number at start: " << firstone << endl;
			cerr << "Minimum number allowed is " << 1 << endl;
			exit(1);
		}
		if ((lastone < 1) && (lastone != 0)) {
			cerr << "Error: range token: \"" << str << "\"" 
				  << " contains too small a number at end: " << lastone << endl;
			cerr << "Minimum number allowed is " << 1 << endl;
			exit(1);
		}

		if (firstone > lastone) {
			for (int i=firstone; i>=lastone; i--) {
				if (inmap[i] >= 0) {
					if ((field.size() > 0) && 
							(field.back().stop == inmeasures[inmap[i]].start)) {
						field.back().stop = inmeasures[inmap[i]].stop;
					} else {
						current.clear();
						current.file = &infile;
						current.num = i;
						current.start = inmeasures[inmap[i]].start;
						current.stop = inmeasures[inmap[i]].stop;

						current.sclef    = inmeasures[inmap[i]].sclef;
						current.skeysig  = inmeasures[inmap[i]].skeysig;
						current.skey     = inmeasures[inmap[i]].skey;
						current.stimesig = inmeasures[inmap[i]].stimesig;
						current.smet     = inmeasures[inmap[i]].smet;
						current.stempo   = inmeasures[inmap[i]].stempo;

						current.eclef    = inmeasures[inmap[i]].eclef;
						current.ekeysig  = inmeasures[inmap[i]].ekeysig;
						current.ekey     = inmeasures[inmap[i]].ekey;
						current.etimesig = inmeasures[inmap[i]].etimesig;
						current.emet     = inmeasures[inmap[i]].emet;
						current.etempo   = inmeasures[inmap[i]].etempo;

						field.push_back(current);
					}
				}
			}
		} else {
			for (int i=firstone; i<=lastone; i++) {
				if (inmap[i] >= 0) {
					if ((field.size() > 0) && 
							(field.back().stop == inmeasures[inmap[i]].start)) {
						field.back().stop = inmeasures[inmap[i]].stop;
					} else {
						current.clear();
						current.file = &infile;
						current.num = i;
						current.start = inmeasures[inmap[i]].start;
						current.stop = inmeasures[inmap[i]].stop;

						current.sclef    = inmeasures[inmap[i]].sclef;
						current.skeysig  = inmeasures[inmap[i]].skeysig;
						current.skey     = inmeasures[inmap[i]].skey;
						current.stimesig = inmeasures[inmap[i]].stimesig;
						current.smet     = inmeasures[inmap[i]].smet;
						current.stempo   = inmeasures[inmap[i]].stempo;

						current.eclef    = inmeasures[inmap[i]].eclef;
						current.ekeysig  = inmeasures[inmap[i]].ekeysig;
						current.ekey     = inmeasures[inmap[i]].ekey;
						current.etimesig = inmeasures[inmap[i]].etimesig;
						current.emet     = inmeasures[inmap[i]].emet;
						current.etempo   = inmeasures[inmap[i]].etempo;

						field.push_back(current);
					}
				}
			}
		}
	} else if (hre.search(buffer, "^(\\d+)([a-z]*)")) {
		int value = hre.getMatchInt(1);
		// do something with letter later...

		if ((value < 1) && (value != 0)) {
			cerr << "Error: range token: \"" << str << "\"" 
				  << " contains too small a number at end: " << value << endl;
			cerr << "Minimum number allowed is " << 1 << endl;
			exit(1);
		}
		if (inmap[value] >= 0) {
			if ((field.size() > 0) && 
					(field.back().stop == inmeasures[inmap[value]].start)) {
				field.back().stop = inmeasures[inmap[value]].stop;
			} else {
				current.clear();
				current.file = &infile;
				current.num = value;
				current.start = inmeasures[inmap[value]].start;
				current.stop = inmeasures[inmap[value]].stop;

				current.sclef    = inmeasures[inmap[value]].sclef;
				current.skeysig  = inmeasures[inmap[value]].skeysig;
				current.skey     = inmeasures[inmap[value]].skey;
				current.stimesig = inmeasures[inmap[value]].stimesig;
				current.smet     = inmeasures[inmap[value]].smet;
				current.stempo   = inmeasures[inmap[value]].stempo;

				current.eclef    = inmeasures[inmap[value]].eclef;
				current.ekeysig  = inmeasures[inmap[value]].ekeysig;
				current.ekey     = inmeasures[inmap[value]].ekey;
				current.etimesig = inmeasures[inmap[value]].etimesig;
				current.emet     = inmeasures[inmap[value]].emet;
				current.etempo   = inmeasures[inmap[value]].etempo;

				field.push_back(current);
			}
		}
	}
}



//////////////////////////////
//
// Tool_myank::removeDollarsFromString -- substitute $ sign for maximum track count.
//

void Tool_myank::removeDollarsFromString(string& buffer, int maxx) {
	HumRegex hre;
	HumRegex hre2;
	string tbuf;
	string obuf;
	int outval;
	int value;

	if (debugQ) {
		m_free_text << "MEASURE STRING BEFORE DOLLAR REMOVAL: " << buffer << endl;
	}

	while (hre.search(buffer, "(\\$\\d*)", "")) {
		tbuf = hre.getMatch(1);
		if (hre2.search(tbuf, "(\\$\\d+)")) {
		  sscanf(hre2.getMatch(1).c_str(), "$%d", &value);
		  outval = maxx - value;
		} else {
			outval = maxx;
		}

		if (outval < 0) {
			outval = 0;
		}
		
		tbuf = to_string(outval);
		obuf = "\\";
		obuf += hre.getMatch(1);
		hre.replaceDestructive(buffer, tbuf, obuf);
	}
	if (debugQ) {
		m_free_text << "DOLLAR EXPAND: " << buffer << endl;
	}
}






//////////////////////////////
//
// Tool_myank::example -- example function calls to the program.
//

void Tool_myank::example(void) {


}



//////////////////////////////
//
// Tool_myank::usage -- command-line usage description and brief summary
//

void Tool_myank::usage(const string& ommand) {

}




/////////////////////////////////
//
// Tool_gridtest::Tool_recip -- Set the recognized options for the tool.
//

Tool_recip::Tool_recip(void) {
	define("c|composite=b",          "do composite rhythm analysis");
	define("a|append=b",             "append composite analysis to input");
	define("p|prepend=b",            "prepend composite analysis to input");
	define("r|replace=b",            "replace **kern data with **recip data");
	define("x|attacks-only=b",       "only mark lines with note attacks");
	define("G|ignore-grace-notes=b", "ignore grace notes");
	define("k|kern-spine=i:1",       "analyze only given kern spine");
	define("K|all-spines=b",         "analyze each kern spine separately");
	define("e|exinterp=s:**recip",   "analyze each kern spine separately");
}



///////////////////////////////
//
// Tool_recip::run -- Primary interfaces to the tool.
//

bool Tool_recip::run(const string& indata, ostream& out) {
	HumdrumFile infile(indata);
	return run(infile, out);
}


bool Tool_recip::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);
	out << infile;
	return status;
}


bool Tool_recip::run(HumdrumFile& infile) {
   initialize(infile);

	int lineCount = infile.getLineCount();
	if (lineCount == 0) {
		m_error_text << "No input data";
		return false;
	}

	if (getBoolean("composite") || getBoolean("append") || getBoolean("prepend")) {
		doCompositeAnalysis(infile);
		infile.createLinesFromTokens();
		return true;
	} else if (getBoolean("replace")) {
		replaceKernWithRecip(infile);
		infile.createLinesFromTokens();
		return true;
	}
	HumdrumFile cfile = infile;
	cfile.analyzeStructure();
	replaceKernWithRecip(cfile);
	cfile.createLinesFromTokens();
	insertAnalysisSpines(infile, cfile);
	// infile.adjustMergeSpineLines();
	infile.createLinesFromTokens();
	return true;
}



//////////////////////////////
//
// Tool_recip::insertAnalysisSpines -- Could be more efficient than the 
//     k-index loop...
//

void Tool_recip::insertAnalysisSpines(HumdrumFile& infile, HumdrumFile& cfile) {
	for (int i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].hasSpines()) {
			continue;
		}
		for (int k=(int)m_kernspines.size()-1; k>=0; k--) {
			int fcount = infile[i].getFieldCount();
			int ktrack = m_kernspines[k]->getTrack();
			int insertj = -1;
			for (int j=fcount-1; j>=0; j--) {
				if (!infile.token(i, j)->isKern()) {
					continue;
				}
				int track = infile.token(i, j)->getTrack();
				if (track != ktrack) {
					continue;
				}
				if (insertj < 0) {
					insertj = j;
				}
				infile[i].appendToken(insertj, cfile.token(i, j)->getText());
				// infile.token(i, insertj+1)->setTrack(remapping[k]);
			}
		}
	}
}



//////////////////////////////
//
// Tool_recip::doCompositeAnalysis --
//

void Tool_recip::doCompositeAnalysis(HumdrumFile& infile) {

	// Calculate composite rhythm **recip spine:

	vector<HumNum> composite(infile.getLineCount());
	for (int i=0; i<(int)composite.size(); i++) {
		composite[i] = infile[i].getDuration();
	}
	
	// convert durations to **recip strings
	vector<string> recips(composite.size());
	for (int i=0; i<(int)recips.size(); i++) {
		if ((!m_graceQ) && (composite[i] == 0)) {
			continue;
		}
		recips[i] = Convert::durationToRecip(composite[i]);
	}

	if (getBoolean("append")) {
		infile.appendDataSpine(recips, "", m_exinterp);
		return;
	} else if (getBoolean("prepend")) {
		infile.prependDataSpine(recips, "", m_exinterp);
		return;
	} else {
		infile.prependDataSpine(recips, "", m_exinterp);
		infile.printFieldIndex(0, m_humdrum_text);
		infile.clear();
		infile.readString(m_humdrum_text.str());
	}
}



//////////////////////////////
//
// Tool_recip::replaceKernWithRecip --
//

void Tool_recip::replaceKernWithRecip(HumdrumFile& infile) {
	vector<HTp> kspines = infile.getKernSpineStartList();
	HumRegex hre;
	string expression = "[^q\\d.%\\]\\[]+";
	for (int i=0; i<infile.getStrandCount(); i++) {
		HTp stok = infile.getStrandStart(i);
		if (!stok->isKern()) {
			continue;
		}
		HTp etok = infile.getStrandEnd(i);
		HTp tok = stok;
		while (tok && (tok != etok)) {
			if (!tok->isData()) {
				tok = tok->getNextToken();
				continue;
			}
			if (tok->isNull()) {
				tok = tok->getNextToken();
				continue;
			}
			if (tok->find('q') != string::npos) {
				if (m_graceQ) {
					tok->setText("q");
				} else {
					tok->setText(".");
				}
			} else {
				hre.replaceDestructive(*tok, "", expression, "g");
			}
			tok = tok->getNextToken();
		}
	}

	for (int i=0; i<(int)kspines.size(); i++) {
		kspines[i]->setText(m_exinterp);
	}

}




//////////////////////////////
//
// Tool_recip::initialize --
//

void Tool_recip::initialize(HumdrumFile& infile) {
	m_kernspines = infile.getKernSpineStartList();
	m_graceQ = !getBoolean("ignore-grace-notes");

	m_exinterp = getString("exinterp");
	if (m_exinterp.empty()) {
		m_exinterp = "**recip";
	} else if (m_exinterp[0] != '*') {
		m_exinterp.insert(0, "*");
	}
	if (m_exinterp[1] != '*') {
		m_exinterp.insert(0, "*");
	}
}





/////////////////////////////////
//
// Tool_satb2gs::Tool_satb2gs -- Set the recognized options for the tool.
//

Tool_satb2gs::Tool_satb2gs(void) {
   define("d|debug=b",    "Debugging information");
   define("author=b",     "Program author");
   define("version=b",    "Program version");
   define("example=b",    "Program examples");
   define("h|help=b",     "Short description");
}



/////////////////////////////////
//
// Tool_satb2gs::run -- Primary interfaces to the tool.
//

bool Tool_satb2gs::run(const string& indata, ostream& out) {
	HumdrumFile infile(indata);
	bool status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_satb2gs::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}

//
// In-place processing of file:
//

bool Tool_satb2gs::run(HumdrumFile& infile) {
	initialize(infile);
	processFile(infile);
	infile.createLinesFromTokens();
	return true;
}



//////////////////////////////
//
// Tool_satb2gs::initialize -- extract time signature lines for
//    each **kern spine in file.
//

void Tool_satb2gs::initialize(HumdrumFile& infile) {
   // handle basic options:
   if (getBoolean("author")) {
      m_free_text << "Written by Craig Stuart Sapp, "
           << "craig@ccrma.stanford.edu, Feb 2011" << endl;
      exit(0);
   } else if (getBoolean("version")) {
      m_free_text << getCommand() << ", version: 16 Dec 2016" << endl;
      m_free_text << "compiled: " << __DATE__ << endl;
      exit(0);
   } else if (getBoolean("help")) {
      usage(getCommand());
      exit(0);
   } else if (getBoolean("example")) {
      example();
      exit(0);
   }

   debugQ     =  getBoolean("debug");
}



//////////////////////////////
//
// Tool_satb2gs::processFile -- data is assumed to be in the order from
// bass, tenor, alto, soprano, with non-**kern data found 
// in any order.  Only the first four **kern spines in the file
// will be considered.
//

void Tool_satb2gs::processFile(HumdrumFile& infile) {
	vector<int> satbtracks;
	satbtracks.resize(4);
	int exinterpline = getSatbTracks(satbtracks, infile);
	int lastline = -1;
	for (int i=0; i<exinterpline; i++) {
		m_humdrum_text << infile[i] << endl;
	}

	printExInterp(infile, exinterpline, satbtracks);

	for (int i=exinterpline+1; i<infile.getLineCount(); i++) {
		if (infile[i].getFieldCount() == 1) {
			m_humdrum_text << infile[i] << endl;
			continue;
		}
		if (*infile.token(i, 0) == "*-") {
			lastline = i;
			break;
		}
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			printSpine(infile, i, j, satbtracks);
			if (j < infile[i].getFieldCount() - 1) {
				m_humdrum_text << '\t';
			}
		}
		m_humdrum_text << '\n';
	}

	if (lastline < 0) {
		return;
	}
	printLastLine(infile, lastline, satbtracks);

	for (int i=lastline+1; i<infile.getLineCount(); i++) {
		m_humdrum_text << infile[i] << endl;
	}
}



//////////////////////////////
//
// Tool_satb2gs::printLastLine --
//

void Tool_satb2gs::printLastLine(HumdrumFile& infile, int line, vector<int>& tracks) {
	int track;

	stringstream output;
	for (int j=0; j<infile[line].getFieldCount() - 1; j++) {
		track = infile.token(line, j)->getTrack();
		if ((track == tracks[1]) || (track == tracks[3])) {
			continue;
		}
		if (track == tracks[0])  {
			output << "*v\t*v";
		} else if (track == tracks[2])  {
			output << "*\t*";
		} else {
			output << "*";
		}
		output << "\t";
	}

	string strang = output.str();
	HumRegex hre;
	hre.replaceDestructive(strang, "", "\t+$");
	m_humdrum_text << strang;
	m_humdrum_text << endl;

	stringstream output2;
	for (int j=0; j<infile[line].getFieldCount() - 1; j++) {
		track = infile.token(line, j)->getTrack();
		if ((track == tracks[1]) || (track == tracks[3])) {
			continue;
		}
		if (track == tracks[2])  {
			output2 << "*v\t*v";
		} else if (track == tracks[0])  {
			output2 << "*";
		} else {
			output2 << "*";
		}
		output2 << "\t";
	}

	output2 << ends;
	strang = output2.str();
	hre.replaceDestructive(strang, "", "\t+$");
	m_humdrum_text << strang;
	m_humdrum_text << endl;

	for (int j=0; j<infile[line].getFieldCount()-2; j++) {
		m_humdrum_text << infile.token(line, j);
		if (j < infile[line].getFieldCount() - 3) {
			m_humdrum_text << "\t";
		}
	}
	m_humdrum_text << "\n";

}



//////////////////////////////
//
// Tool_satb2gs::printExInterp -- print only tenor and soprano tracks
//

void Tool_satb2gs::printExInterp(HumdrumFile& infile, int line,
		vector<int>& tracks) {
	stringstream output;
	int j;
	int track;

	// first print exclusive interpretations
	for (j=0; j<infile[line].getFieldCount(); j++) {
		track = infile.token(line, j)->getTrack();
		if ((track == tracks[1]) || (track == tracks[3])) {
			continue;
		}
		output << infile.token(line, j) << "\t";
	}
	string strang = output.str();
	HumRegex hre;
	hre.replaceDestructive(strang, "", "\t+$");
	m_humdrum_text << strang;
	m_humdrum_text << endl;

	stringstream output2;
	stringstream output3;
	for (j=0; j<infile[line].getFieldCount(); j++) {
		track = infile.token(line, j)->getTrack();
		if ((track == tracks[1]) || (track == tracks[3])) {
			continue;
		}
		if (track == tracks[0]) {
			output3 << "*clefF4";
			output2 << "*^";
		} else if (track == tracks[2]) {
			output3 << "*clefG2";
			output2 << "*^";
		} else {
			output3 << "*";
			output2 << "*";
		}
		output3 << "\t";
		output2 << "\t";
	}

	strang = output3.str();
	hre.replaceDestructive(strang, "", "\t+$");
	m_humdrum_text << strang;
	m_humdrum_text << endl;

	strang = output2.str();
	hre.replaceDestructive(strang, "", "\t+$");
	m_humdrum_text << strang;
	m_humdrum_text << endl;

}



///////////////////////
//
// Tool_satb2gs::printSpine --
//

void Tool_satb2gs::printSpine(HumdrumFile& infile, int row, int col,
		vector<int>& satbtracks) {
	int track = infile.token(row, col)->getTrack();
	int target = -1;
	for (int k=0; k<(int)satbtracks.size(); k++) {
		if (track == satbtracks[k]) {
			if (k % 2 == 0) {
				target = satbtracks[k+1];
			} else {
				target = satbtracks[k-1];
			}
			break;
		}
	}

	if (target < 0) {
		// does not need to be switched
		m_humdrum_text << infile.token(row, col);
		return;
	}

	// print the SAT or B token(s) (there should be only one token for each)
	//
	// If a tenor has a fermata and a bass has a fermata and both are
	// the same duration, then hide the tenor's fermata.
	//
	// If an alto has a fermata and a soprano has a fermata and both are
	// the same duration, then hide the alto's fermata.
	//


	// first identify the column for each voice, considering only the first
	// layer, if there are multiple layers.
	vector<int> cols(4);
	fill(cols.begin(), cols.end(), -1);
	for (int j=0; j<infile[row].getFieldCount(); j++) {
		track = infile.token(row, j)->getTrack();
		for (int k=0; k<(int)satbtracks.size(); k++) {
			if (cols[k] >= 0) {
				continue;
			}
			if (track == satbtracks[k]) {
				cols[k] = j;
			}
		}
	}


	HumRegex hre;
	string strang;
	int count = 0;
	bool foundnames = false;
	bool foundabbreviations = false;
	for (int j=0; j<infile[row].getFieldCount(); j++) {
		track = infile.token(row, j)->getTrack();
		if (track == target) {
			if (count > 0) {
				m_humdrum_text << '\t';
			}
			strang = *infile.token(row,j);
			hre.replaceDestructive(strang, "!*clef", "^\\*clef");
			if ((!foundnames) && hre.search(strang, R"(^\*I")")) {
				foundnames = true;
				hre.replaceDestructive(strang, R"(!*I"Soprano")", R"(^\*I"Soprano)");
				hre.replaceDestructive(strang, R"(!*I"Alto")"   , R"(^\*I"Alto)");
				hre.replaceDestructive(strang, R"(!*I"Tenor")"  , R"(^\*I"Tenor)");
				hre.replaceDestructive(strang, R"(!*I\"Bass")"  , R"(^\*I"Bass)");
			}
			if ((!foundabbreviations) && hre.search(strang, R"(^\*I')")) {
				foundabbreviations = true;
				hre.replaceDestructive(strang, R"(!*I'S")", R"(^\*I'S)");
				hre.replaceDestructive(strang, R"(!*I'A")", R"(^\*I'A)");
				hre.replaceDestructive(strang, R"(!*I'T")", R"(^\*I'T)");
				hre.replaceDestructive(strang, R"(!*I'B")", R"(^\*I'B)");
			}

			if (infile[row].isData()) {
				if ((cols[0] == col) && 
							(infile.token(row, col)->find(';') != string::npos)) {
					HumNum tenordur;
					HumNum bassdur;
					tenordur = Convert::recipToDuration(infile.token(row, cols[0])).getFloat();
					bassdur  = Convert::recipToDuration(infile.token(row, cols[1])).getFloat();
					if (tenordur == bassdur) {
						hre.replaceDestructive(strang, ";y", ";", "g"); // hide fermata
						// hre.replaceDestructive(strang, ";y", ";", "g"); // hide fermata
					}
				}

				if ((cols[3] == col) && (infile.token(row, col)->find(';') != string::npos)) {
					HumNum altodur;
					HumNum sopranodur;
					altodur = Convert::recipToDuration(infile.token(row, cols[3]));
					sopranodur  = Convert::recipToDuration(infile.token(row, cols[2]));
					if (altodur == sopranodur) {
						hre.replaceDestructive(strang, ";y", ";", "g"); // hide fermata
					}
				}

			}

			m_humdrum_text << strang;
			count++;
		}
	}
}



///////////////////////////////
//
// Tool_satb2gs::getSatbTracks -- return the primary track numbers of 
//     the satb spines.
//

int Tool_satb2gs::getSatbTracks(vector<int>& tracks, HumdrumFile& infile) {
	tracks.clear();
	int output = -1;
	int track;
	for (int i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isInterpretation()) {
			continue;
		}
		output = i;
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			track = infile.token(i, j)->getTrack();
			tracks.push_back(track);
			if (tracks.size() == 4) {
				return output;
			}
		}
		break;
	}

	if (tracks.size() != 4) {
		m_error_text << "Error: there are " << tracks.size() << " **kern spines"
			  << " in input data (needs to be 4)" << endl;
		exit(1);
	}

	return output;
}




//////////////////////////////
//
// Tool_satb2gs::example -- example function calls to the program.
//

void Tool_satb2gs::example(void) {


}



//////////////////////////////
//
// Tool_satb2gs::usage -- command-line usage description and brief summary
//

void Tool_satb2gs::usage(const string& command) {

}





#define STYLE_CONCERT 0
#define STYLE_WRITTEN 1

/////////////////////////////////
//
// Tool_transpose::Tool_transpose -- Set the recognized options for the tool.
//

Tool_transpose::Tool_transpose(void) {
	define("b|base40=i:0",   "the base-40 transposition value");
	define("d|diatonic=i:0", "the diatonic transposition value");
	define("c|chromatic=i:0","the chromatic transposition value");
	define("o|octave=i:0",    "the octave addition to tranpose value");
	define("t|transpose=s",   "musical interval transposition value");
	define("k|setkey=s",      "transpose to the given key");
	define("auto=b",         "auto. trans. inst. parts to concert pitch");
	define("debug=b",        "print debugging statements");
	define("s|spines=s:", "transpose only specified spines");
	define("q|quiet=b",      "suppress *Tr interpretations in output");
	define("I|instrument=b", "insert instrument code (*ITr) as well");
	define("C|concert=b",    "transpose written score to concert pitch");
	define("W|written=b",    "trans. concert pitch score to written score");
	define("rotation=b",     "display transposition in half-steps");

	define("author=b",  "author of program");
	define("version=b", "compilation info");
	define("example=b", "example usages");
	define("help=b",  "short description");
}



/////////////////////////////////
//
// Tool_transpose::run -- Do the main work of the tool.
//

bool Tool_transpose::run(const string& indata, ostream& out) {
	HumdrumFile infile(indata);
	bool status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}


bool Tool_transpose::run(HumdrumFile& infile, ostream& out) {
	int status = run(infile);
	if (hasAnyText()) {
		getAllText(out);
	} else {
		out << infile;
	}
	return status;
}

//
// In-place processing of file:
//

bool Tool_transpose::run(HumdrumFile& infile) {
	initialize(infile);

	if (ssetkeyQ) {
		transval = calculateTranspositionFromKey(ssetkey, infile);
		transval = transval + octave * 40;
		if (debugQ) {
			m_humdrum_text << "!!Key TRANSVAL = " << transval;
		}
	}

	if (getBoolean("rotation")) {
		// returns the base-12 pitch transposition for use in conjunction
		// with the mkeyscape --rotate option
		int value = 60 - Convert::base40ToMidiNoteNumber(162 - transval);
		m_free_text << value << endl;
		return false;
	}

	if (concertQ) {
		convertScore(infile, STYLE_CONCERT);
	} else if (writtenQ) {
		convertScore(infile, STYLE_WRITTEN);
	} else if (autoQ) {
		doAutoTransposeAnalysis(infile);
	} else {
		vector<bool> spineprocess;
		infile.makeBooleanTrackList(spineprocess, spinestring);
		// filter out non-kern spines so they are not analyzed.
		for (int t=1; t<=infile.getMaxTrack(); t++) {
			if (!infile.getTrackStart(t)->isKern()) {
				spineprocess[t] = false;
			}
		}
		processFile(infile, spineprocess);
	}

	return true;
}



//////////////////////////////
//
// Tool_transpose::convertScore -- create a concert pitch score from
//     a written pitch score.  The function will search for *Tr
//     interpretations in spines, and convert them to *ITr interpretations
//     as well as transposing notes, and transposing key signatures and
//     key interpretations.  Or create a written score from a
//     concert pitch score based on the style parameter.
//

void Tool_transpose::convertScore(HumdrumFile& infile, int style) {
	vector<int> tvals;  // transposition values for each spine
	tvals.reserve(infile.getMaxTrack() + 1);

	int ptrack;
	int i, j;
	for (i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isInterpretation()) {
				// scan the line for transposition codes
				// as well as key signatures and key markers
				processInterpretationLine(infile, i, tvals, style);
				break;

		} else if (infile[i].isData()) {
			// transpose notes according to tvals data
			for (j=0; j<infile[i].getFieldCount(); j++) {
				if (!infile.token(i, j)->isKern()) {
					m_humdrum_text << infile.token(i, j);
					if (j < infile[i].getFieldCount() - 1) {
					 		m_humdrum_text << "\t";
					}
					continue;
				}
				ptrack = infile.token(i, j)->getTrack();
				if (tvals[ptrack] == 0) {
					  m_humdrum_text << infile.token(i, j);
				} else {
					  printTransposedToken(infile, i, j, tvals[ptrack]);
				}
				if (j < infile[i].getFieldCount() - 1) {
					  m_humdrum_text << "\t";
				}
			}
			m_humdrum_text << "\n";
			break;

		} else {
			m_humdrum_text << infile[i] << "\n";
		}
	}
}



//////////////////////////////
//
// Tool_transpose::processInterpretationLine --  Used in converting between
//   concert pitch and written pitch scores.
//

void Tool_transpose::processInterpretationLine(HumdrumFile& infile, int line,
	  vector<int>& tvals, int style) {
	if (hasTrMarkers(infile, line)) {
		switch (style) {
			case STYLE_CONCERT:
				convertToConcertPitches(infile, line, tvals);
				break;
			case STYLE_WRITTEN:
				convertToWrittenPitches(infile, line, tvals);
				break;
			default: m_humdrum_text << infile[line];
		}
		m_humdrum_text << "\n";
		return;
	}

	for (int j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			m_humdrum_text << infile.token(line, j);
			if (j<infile[line].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
			continue;
		}
		int ptrack = infile.token(line, j)->getTrack();

		// check for *ITr or *Tr markers
		// ignore *ITr markers when creating a Concert-pitch score
		// ignore *Tr  markers when creating a Written-pitch score
		HumRegex hre;
		if (hre.search(infile.token(line, j), "^\\*k\\[([a-gA-G\\#-]*)\\]", "")) {
			// transpose *k[] markers if necessary
			if (tvals[ptrack] != 0) {
				printNewKeySignature(hre.getMatch(1), tvals[ptrack]);
			} else {
				m_humdrum_text << infile.token(line, j);
			}

		} else if (isKeyMarker(*infile.token(line, j))) {
			// transpose *C: markers and like if necessary
			if (tvals[ptrack] != 0) {
				printNewKeyInterpretation(infile[line], j, tvals[ptrack]);
			} else {
				m_humdrum_text << infile.token(line, j);
			}

		} else {
			// other interpretations just echoed to output:
			m_humdrum_text << infile.token(line, j);
		}
		if (j<infile[line].getFieldCount()-1) {
			m_humdrum_text << "\t";
		}
	}
	m_humdrum_text << "\n";

}



//////////////////////////////
//
// Tool_transpose::convertToWrittenPitches --
//

void Tool_transpose::convertToWrittenPitches(HumdrumFile& infile, int line,
		vector<int>& tvals) {
	HumRegex hre;
	int j;
	int base;
	int ptrack;
	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			m_humdrum_text << infile.token(line, j);
			if (j < infile[line].getFieldCount() - 1) {
				m_humdrum_text << "\t";
			}
			continue;
		}
		if (hre.search(infile.token(line, j),
		"^\\*ITrd[+-]?\\d+c[+-]?\\d+$", "")) {
			base = Convert::transToBase40(*infile.token(line, j));

			string output = "*Tr";
			output += Convert::base40ToTrans(base);
			m_humdrum_text << output;
			ptrack = infile.token(line, j)->getTrack();
			tvals[ptrack] = base;
		} else {
			m_humdrum_text << infile.token(line, j);
		}
		if (j < infile[line].getFieldCount() - 1) {
			m_humdrum_text << "\t";
		}
	}
}



//////////////////////////////
//
// Tool_transpose::convertToConcertPitches --
//

void Tool_transpose::convertToConcertPitches(HumdrumFile& infile, int line, vector<int>& tvals) {
	HumRegex hre;
	int j;
	int base;
	int ptrack;
	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			m_humdrum_text << infile.token(line, j);
			if (j < infile[line].getFieldCount() - 1) {
				m_humdrum_text << "\t";
			}
			continue;
		}
		if (hre.search(infile.token(line, j),
				"^\\*Trd[+-]?\\d+c[+-]?\\d+$", "")) {
			base = Convert::transToBase40(*infile.token(line, j));
			string output = "*ITr";
			output += Convert::base40ToTrans(base);
			m_humdrum_text << output;
			ptrack = infile.token(line, j)->getTrack();
			tvals[ptrack] = -base;
		} else {
			m_humdrum_text << infile.token(line, j);
		}
		if (j < infile[line].getFieldCount() - 1) {
			m_humdrum_text << "\t";
		}
	}
}



//////////////////////////////
//
// Tool_transpose::hasTrMarkers -- returns true if there are any tokens
//    which start with *ITr or *Tr and contains c and d
//    with numbers after each of them.
//

int Tool_transpose::hasTrMarkers(HumdrumFile& infile, int line) {
	HumRegex hre;
	int j;
	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			continue;
		}
		if (hre.search(infile.token(line, j),
				"^\\*I?Trd[+-]?\\d+c[+-]?\\d+$", "")) {
			return 1;
		}
	}

	return 0;
}



//////////////////////////////
//
// Tool_transpose::isKeyMarker -- returns true if the interpretation is
//    a key description, such as *C: for C major, or *a:.
//

int Tool_transpose::isKeyMarker(const string& str) {
	HumRegex hre;
	return hre.search(str, "^\\*[a-g]?[\\#-]?:", "i");
}



//////////////////////////////
//
// Tool_transpose::printTransposedToken -- print a Humdrum token with the given
//    base-40 transposition value applied.  Only **kern data is
//    know now to transpose, other data types are currently not
//    allowed to be transposed (but could be added here later).
//

void Tool_transpose::printTransposedToken(HumdrumFile& infile, int row, int col, int transval) {
	if (infile.token(row, col)->isKern()) {
		// don't know how to transpose this type of data, so leave it as is
		m_humdrum_text << infile.token(row, col);
		return;
	}

	printHumdrumKernToken(infile[row], col, transval);
}



//////////////////////////////
//
// Tool_transpose::calculateTranspositionFromKey --
//

int Tool_transpose::calculateTranspositionFromKey(int targetkey, 
		HumdrumFile& infile) {
	HumRegex hre;
	int base40 = 0;
	int currentkey = 0;
	int mode = 0;
	int found = 0;

	for (int i=0; i<infile.getLineCount(); i++) {
		if (infile[i].isData()) {
			// no initial key label was found, so don't transpose.
			// in the future, maybe allow an automatic key analysis
			// to be performed on the data if there is not explicit
			// key designation.
			return 0;
		}
		if (!infile[i].isInterpretation()) {
			continue;
		}
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			if (!hre.search(infile.token(i, j), "^\\*([A-G][#-]?):", "i")) {
				continue;
			}

			mode = 0;  // major key
			if (std::islower(infile.token(i, j)->at(1))) {
				mode = 1;  // minor key
			}
			base40 = Convert::kernToBase40(infile.token(i, j));
			// base40 = base40 + transval;
			base40 = base40 + 4000;
			base40 = base40 % 40;
			base40 = base40 + (3 + mode) * 40;
			currentkey = base40;
	 		found = 1;
			break;
		}
		if (found) {
			break;
		}
	}

	int trans = (targetkey%40 - currentkey%40);
	// base40 = targetkey + (3 + mode) * 40;
	if (trans > 40) {
		trans -= 40;
	}
	if (trans > 20) {
		trans = 40 - trans;
		trans = -trans;
	}
	if (trans < -40) {
		trans += 40;
	}
	if (trans < -20) {
		trans = -40 - trans;
		trans = -trans;
	}

	return trans;
}



//////////////////////////////
//
// Tool_transpose::printTransposeInformation -- collect and print *Tr interpretations
//      at the start of the spine.  Looks for *Tr markers at the start
//      of the file before any data.
//

void Tool_transpose::printTransposeInformation(HumdrumFile& infile,
		vector<bool>& spineprocess, int line, int transval) {
	int j;
	int ptrack;

	vector<int> startvalues(infile.getMaxTrack()+1);
	vector<int> finalvalues(infile.getMaxTrack()+1);

	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			continue;
		}
		ptrack = infile.token(line, j)->getTrack();
		startvalues[ptrack] = getTransposeInfo(infile, line, j);
		// m_humdrum_text << "Found transpose value " << startvalues[ptrack] << endl;
	}

	int entry = 0;
	// check if any spine will be transposed after final processing
	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			continue;
		}
		ptrack = infile.token(line, j)->getTrack();
		if (spineprocess[ptrack]) {
		finalvalues[ptrack] = transval;
		if (!instrumentQ) {
			finalvalues[ptrack] += startvalues[ptrack];
		}
		if (finalvalues[ptrack] != 0) {
			entry = 1;
		}
	} else {
			finalvalues[ptrack] = startvalues[ptrack];
			if (finalvalues[ptrack] != 0) {
				entry = 1;
			}
		}
	}

	if (!entry) {
		return;
	}

	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			m_humdrum_text << "*";
			if (j < infile[line].getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
			continue;
		}
		ptrack = infile.token(line, j)->getTrack();
		if (finalvalues[ptrack] == 0) {
			m_humdrum_text << "*";
		} else {
			if (instrumentQ) {
				m_humdrum_text << "*ITr";
				m_humdrum_text << Convert::base40ToTrans(-finalvalues[ptrack]);
			} else {
				m_humdrum_text << "*Tr";
				m_humdrum_text << Convert::base40ToTrans(finalvalues[ptrack]);
			}
		}
		if (j < infile[line].getFieldCount()-1) {
			m_humdrum_text << "\t";
		}

	}
	m_humdrum_text << "\n";
}



//////////////////////////////
//
// getTransposeInfo -- returns the Transpose information found in
//    the specified spine starting at the current line, and searching
//    until data is found (or a *- record is found). Return value is a
//    base-40 number.
//

int Tool_transpose::getTransposeInfo(HumdrumFile& infile, int row, int col) {
	int track = infile.token(row, col)->getTrack();
	int ptrack;
	HumRegex hre;
	int base;
	int output = 0;

	for (int i=row; i<infile.getLineCount(); i++) {
		if (infile[i].isData()) {
			break;
		}
		if (!infile[i].isInterpretation()) {
			continue;
		}
		for (int j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			ptrack = infile.token(i, j)->getTrack();
			if (ptrack != track) {
				continue;
			}
			if (hre.search(infile.token(i, j),
					"^\\*Trd[+-]?\\d+c[+-]?\\d+$", "")) {
				base = Convert::transToBase40(*infile.token(i, j));
				output += base;
				// erase the *Tr value because it will be printed elsewhere
				infile.token(i, j)->setText("*deletedTr");
			}
		}
	}

	return output;
}



//////////////////////////////
//
// Tool_transpose::checkForDeletedLine -- check to see if a "*deletedTr
//

int Tool_transpose::checkForDeletedLine(HumdrumFile& infile, int line) {
	int j;
	if (!infile[line].isInterpretation()) {
		return 0;
	}

	int present = 0;
	int composite = 0;
	for (j=0; j<infile[line].getFieldCount(); j++) {
		if (!infile.token(line, j)->isKern()) {
			continue;
		}
		if (infile.token(line, j)->find("deletedTr") != string::npos) {
			present = 1;
		} else if (infile.token(line, j)->isNull()) {
			// do nothing: not composite
		} else {
			// not a *deletedTr token or a * token, so have to print line later
			composite = 1;
		}
	}

	if (present == 0) {
		// no *deletedTr records found on the currnet line, so process normally
		return 0;
	}

	if (composite == 0) {
		// *deletedTr found, but no other important data found on line.
		return 1;
	}

	// print non-deleted elements in line.
	for (j=0; j<infile[line].getFieldCount(); j++) {;
		if ((string)(*infile.token(line, j)) == "deletedTr") {
			m_humdrum_text << "*";
		} else {
			m_humdrum_text << infile.token(line, j);
		}
		if (j < infile[line].getFieldCount() - 1) {
			m_humdrum_text << "\t";
		}
	}
	m_humdrum_text << "\n";

	return 1;
}



//////////////////////////////
//
// Tool_transpose::processFile --
//

void Tool_transpose::processFile(HumdrumFile& infile,
		vector<bool>& spineprocess) {
	int i;
	int diatonic;
	int j;
	HumRegex hre;
	int interpstart = 0;

	for (i=0; i<infile.getLineCount(); i++) {
		if (!quietQ && (interpstart == 1)) {
			interpstart = 2;
			printTransposeInformation(infile, spineprocess, i, transval);
		}
		if (checkForDeletedLine(infile, i)) {
			continue;
		}

		if (infile[i].isData()) {
			printHumdrumDataRecord(infile[i], spineprocess);
			m_humdrum_text << "\n";
		} else if (infile[i].isInterpretation()) {
			for (j=0; j<infile[i].getFieldCount(); j++) {
				if (!infile.token(i, j)->isKern()) {
					m_humdrum_text << infile.token(i, j);
					if (j<infile[i].getFieldCount()-1) {
						m_humdrum_text << "\t";
					}
					continue;
				}
				if (infile.token(i, j)->compare(0, 2, "**") == 0) {
						interpstart = 1;
				}

				// check for key signature in a spine which is being
				// transposed, and adjust it.
				if (spineprocess[infile.token(i, j)->getTrack()] &&
						hre.search(infile.token(i, j),
							"^\\*k\\[([a-gA-G#-]*)\\]", "i")) {
						printNewKeySignature(hre.getMatch(1), transval);
						if (j<infile[i].getFieldCount()-1) {
						m_humdrum_text << "\t";
						}
						continue;
				}

				// check for key tandem interpretation and tranpose
				// if the spine data is being transposed.

				if (hre.search(infile.token(i, j), "^\\*([A-G])[#-]?:", "i")) {
					diatonic = tolower(hre.getMatch(1)[0]) - 'a';
					if (diatonic >= 0 && diatonic <= 6) {
					  	printNewKeyInterpretation(infile[i], j, transval);
					  	if (j<infile[i].getFieldCount()-1) {
							m_humdrum_text << "\t";
					  	}
					  	continue;
					}
				}
				m_humdrum_text << infile.token(i, j);
				if (j<infile[i].getFieldCount()-1) {
					m_humdrum_text << "\t";
				}
			}
			m_humdrum_text << "\n";

		} else {
			m_humdrum_text << infile[i] << "\n";
		}
	}
}



//////////////////////////////
//
// Tool_transpose::printNewKeySignature --
//

void Tool_transpose::printNewKeySignature(const string& keysig, int trans) {
	int counter = 0;
	int len = (int)keysig.size();
	for (int i=0; i<len; i++) {
		switch(keysig[i]) {
			case '-': counter--; break;
			case '#': counter++; break;
		}
	}

	int xxx = Convert::base40IntervalToLineOfFifths(trans);
	int newkey = xxx + counter;
	m_humdrum_text << Convert::keyNumberToKern(newkey);
}



//////////////////////////////
//
// Tool_transpose::printNewKeyInterpretation --
//

void Tool_transpose::printNewKeyInterpretation(HumdrumLine& aRecord,
		int index, int transval) {

	int mode = 0;
	if (std::islower(aRecord.token(index)->at(1))) {
		mode = 1;
	}
	int base40 = Convert::kernToBase40(*aRecord.token(index));
	currentkey = base40;
	base40 = base40 + transval;
	base40 = base40 + 4000;
	base40 = base40 % 40;
	base40 = base40 + (3 + mode) * 40;

	m_humdrum_text << "*" << Convert::base40ToKern(base40) << ":";

	HumRegex hre;
	if (hre.search((string)*aRecord.token(index), ":(.+)$", "")) {
		m_humdrum_text << hre.getMatch(1);
	}
}



//////////////////////////////
//
// Tool_transpose::printHumdrumDataRecord --
//

void Tool_transpose::printHumdrumDataRecord(HumdrumLine& record,
		vector<bool>& spineprocess) {
	int i;
	for (i=0; i<record.getFieldCount(); i++) {
		if (!record.token(i)->isKern()) {
			// don't try to transpose non-kern spines
			m_humdrum_text << record.token(i);
			if (i<record.getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
			continue;
		}
		if (!spineprocess[record.token(i)->getTrack()]) {
			// don't try to transpose spines which were not indicated.
			m_humdrum_text << record.token(i);
			if (i<record.getFieldCount()-1) {
				m_humdrum_text << "\t";
			}
			continue;
		}

		printHumdrumKernToken(record, i, transval);

		if (i<record.getFieldCount()-1) {
			m_humdrum_text << "\t";
		}
		continue;
	}

}



//////////////////////////////
//
// Tool_transpose::printHumdrumKernToken --
//

void Tool_transpose::printHumdrumKernToken(HumdrumLine& record, int index,
		int transval) {
	if (record.token(index)->isNull()) {
		// null record element (no pitch).
		m_humdrum_text << record.token(index);
		return;
	}
	if (!record.token(index)->isKern()) {
		m_humdrum_text << record.token(index);
		return;
	}
	string buffer;
	int tokencount = record.token(index)->getSubtokenCount();
	for (int k=0; k<tokencount; k++) {
		buffer = record.token(index)->getSubtoken(k);
		printNewKernString(buffer, transval);
		if (k<tokencount-1) {
			m_humdrum_text << " ";
		}
	}
}




//////////////////////////////
//
// Tool_transpose::printNewKernString --
//

void Tool_transpose::printNewKernString(const string& input, int transval) {
	if (input.rfind('r') != string::npos) {
		// don't transpose rests...
		m_humdrum_text << input;
		return;
	}
	if (input == ".") {
		// don't transpose null tokens...
		m_humdrum_text << input;
		return;
	}

	int base40 = Convert::kernToBase40(input);
	string newpitch = Convert::base40ToKern(base40 + transval);

	// consider interaction of #X -X n interaction vs. nX.
	HumRegex hre;
	string output;
	if (hre.search(input, "([A-Ga-g#n-]+)")) {
		string oldpitch = hre.getMatch(1);
		output = hre.replaceCopy(input, newpitch, oldpitch);
	}
	m_humdrum_text << output;
}



//////////////////////////////
//
// Tool_transpose::getBase40ValueFromInterval -- note: only ninth interval range allowed
//

int Tool_transpose::getBase40ValueFromInterval(const string& string) {
	int sign = 1;
	if (string.find('-') != string::npos) {
		sign = -1;
	}

	int length = (int)string.size();
	char* buffer = new char[length+1];
	strcpy(buffer, string.c_str());
	int i;
	for (i=0; i<length; i++) {
		if (buffer[i] == 'p') { buffer[i] = 'P'; }
		if (buffer[i] == 'a') { buffer[i] = 'A'; }
		if (buffer[i] == 'D') { buffer[i] = 'd'; }
	}

	int output = 0;

	if (strstr(buffer, "dd1") != NULL)      { output = -2; }
	else if (strstr(buffer, "d1") != NULL)  { output = -1; }
	else if (strstr(buffer, "P1") != NULL)  { output =  0; }
	else if (strstr(buffer, "AA1") != NULL) { output =  2; }
	else if (strstr(buffer, "A1") != NULL)  { output =  1; }

	else if (strstr(buffer, "dd2") != NULL) { output =  3; }
	else if (strstr(buffer, "d2") != NULL)  { output =  4; }
	else if (strstr(buffer, "m2") != NULL)  { output =  5; }
	else if (strstr(buffer, "M2") != NULL)  { output =  6; }
	else if (strstr(buffer, "AA2") != NULL) { output =  8; }
	else if (strstr(buffer, "A2") != NULL)  { output =  7; }

	else if (strstr(buffer, "dd3") != NULL) { output =  9; }
	else if (strstr(buffer, "d3") != NULL)  { output = 10; }
	else if (strstr(buffer, "m3") != NULL)  { output = 11; }
	else if (strstr(buffer, "M3") != NULL)  { output = 12; }
	else if (strstr(buffer, "AA3") != NULL) { output = 14; }
	else if (strstr(buffer, "A3") != NULL)  { output = 13; }

	else if (strstr(buffer, "dd4") != NULL) { output = 15; }
	else if (strstr(buffer, "d4") != NULL)  { output = 16; }
	else if (strstr(buffer, "P4") != NULL)  { output = 17; }
	else if (strstr(buffer, "AA4") != NULL) { output = 19; }
	else if (strstr(buffer, "A4") != NULL)  { output = 18; }

	else if (strstr(buffer, "dd5") != NULL) { output = 21; }
	else if (strstr(buffer, "d5") != NULL)  { output = 22; }
	else if (strstr(buffer, "P5") != NULL)  { output = 23; }
	else if (strstr(buffer, "AA5") != NULL) { output = 25; }
	else if (strstr(buffer, "A5") != NULL)  { output = 24; }

	else if (strstr(buffer, "dd6") != NULL) { output = 26; }
	else if (strstr(buffer, "d6") != NULL)  { output = 27; }
	else if (strstr(buffer, "m6") != NULL)  { output = 28; }
	else if (strstr(buffer, "M6") != NULL)  { output = 29; }
	else if (strstr(buffer, "AA6") != NULL) { output = 31; }
	else if (strstr(buffer, "A6") != NULL)  { output = 30; }

	else if (strstr(buffer, "dd7") != NULL) { output = 32; }
	else if (strstr(buffer, "d7") != NULL)  { output = 33; }
	else if (strstr(buffer, "m7") != NULL)  { output = 34; }
	else if (strstr(buffer, "M7") != NULL)  { output = 35; }
	else if (strstr(buffer, "AA7") != NULL) { output = 37; }
	else if (strstr(buffer, "A7") != NULL)  { output = 36; }

	else if (strstr(buffer, "dd8") != NULL) { output = 38; }
	else if (strstr(buffer, "d8") != NULL)  { output = 39; }
	else if (strstr(buffer, "P8") != NULL)  { output = 40; }
	else if (strstr(buffer, "AA8") != NULL) { output = 42; }
	else if (strstr(buffer, "A8") != NULL)  { output = 41; }

	else if (strstr(buffer, "dd9") != NULL) { output = 43; }
	else if (strstr(buffer, "d9") != NULL)  { output = 44; }
	else if (strstr(buffer, "m9") != NULL)  { output = 45; }
	else if (strstr(buffer, "M9") != NULL)  { output = 46; }
	else if (strstr(buffer, "AA9") != NULL) { output = 48; }
	else if (strstr(buffer, "A9") != NULL)  { output = 47; }

	return output * sign;
}



//////////////////////////////
//
// Tool_transpose::example --
//

void Tool_transpose::example(void) {


}



//////////////////////////////
//
// Tool_transpose::usage --
//

void Tool_transpose::usage(const string& command) {

}



///////////////////////////////////////////////////////////////////////////
//
// Automatic transposition functions
//


//////////////////////////////
//
// Tool_transpose::doAutoTransposeAnalysis --
//

void Tool_transpose::doAutoTransposeAnalysis(HumdrumFile& infile) {
	vector<int> ktracks(infile.getMaxTrack()+1, 0);

	vector<HTp> tracks;
	infile.getTrackStartList(tracks);
	int i;
	for (i=0; i<(int)tracks.size(); i++) {
		if (tracks[i]->isKern()) {
			ktracks[i] = tracks[i]->getTrack();
		} else {
			ktracks[i] = 0;
		}
	}

	int segments = int(infile.getScoreDuration().getFloat()+0.5);
	if (segments < 1) {
		segments = 1;
	}

	vector<vector<vector<double> > > trackhist;
	trackhist.resize(ktracks.size());

	for (i=1; i<(int)trackhist.size(); i++) {
		if (ktracks[i]) {
			storeHistogramForTrack(trackhist[i], infile, i, segments);
		}
	}

	if (debugQ) {
		m_free_text << "Segment pitch histograms: " << endl;
		printHistograms(segments, ktracks, trackhist);
	}

	int level = 16;
	int hop   = 8;
	int count = segments / hop;

	if (segments < count * level / (double)hop) {
		level = level / 2;
		hop   = hop / 2;
	}
	if (segments < count * level / (double)hop) {
		count = count / 2;
	}

	if (segments < count * level / (double)hop) {
		level = level / 2;
		hop   = hop / 2;
	}
	if (segments < count * level / (double)hop) {
		count = count / 2;
	}

	vector<vector<vector<double> > > analysis;

	doAutoKeyAnalysis(analysis, level, hop, count, segments, ktracks, trackhist);

	// print analyses raw results

	m_free_text << "Raw key analysis by track:" << endl;
	printRawTrackAnalysis(analysis, ktracks);

	doTranspositionAnalysis(analysis);
}



//////////////////////////////
//
// Tool_transpose::doTranspositionAnalysis --
//

void Tool_transpose::doTranspositionAnalysis(vector<vector<vector<double> > >& analysis) {
	int i, j, k;
	int value1;
	int value2;
	int value;

	for (i=0; i<1; i++) {
		for (j=2; j<3; j++) {
			for (k=0; k<(int)analysis[i].size(); k++) {
				if (analysis[i][k][24] >= 0 && analysis[j][k][24] >= 0) {
					value1 = (int)analysis[i][k][25];
	 				if (value1 >= 12) {
						  value1 = value1 - 12;
					}
					value2 = (int)analysis[j][k][25];
	 				if (value2 >= 12) {
						  value2 = value2 - 12;
					}
					value = value1 - value2;
					if (value < 0) {
						  value = value + 12;
					}
					if (value > 6) {
						  value = 12 - value;
					}
					m_free_text << value << endl;
				}
			}
		}
	}
}



//////////////////////////////
//
// Tool_transpose::printRawTrackAnalysis --
//

void Tool_transpose::printRawTrackAnalysis(vector<vector<vector<double> > >& analysis,
		vector<int>& ktracks) {

	int i, j;
	int value;
	int value2;

	for (i=0; i<(int)analysis[0].size(); i++) {
		m_free_text << "Frame\t" << i << ":";
		for (j=0; j<(int)analysis.size(); j++) {
			m_free_text << "\t";
	 		value = (int)analysis[j][i][24];
	 		if (value >= 12) {
				value = value - 12;
			}
	 		value2 = (int)analysis[j][i][25];
	 		if (value2 >= 12) {
				value2 = value2 - 12;
			}
			m_free_text << value;
	 		// if (value != value2) {
	 		//    m_free_text << "," << value2;
			// }
		}
		m_free_text << "\n";
	}
}



//////////////////////////////
//
// doAutoKeyAnalysis --
//

void Tool_transpose::doAutoKeyAnalysis(vector<vector<vector<double> > >& analysis, int level,
		int hop, int count, int segments, vector<int>& ktracks,
		vector<vector<vector<double> > >& trackhist) {

	vector<double> majorweights;
	vector<double> minorweights;
	fillWeightsWithKostkaPayne(majorweights, minorweights);

	int size = 0;
	int i;
	for (i=1; i<(int)ktracks.size(); i++) {
		if (ktracks[i]) {
			size++;
		}
	}

	analysis.resize(size);
	for (i=0; i<(int)analysis.size(); i++) {
		analysis[i].reserve(count);
	}

	int aindex = 0;
	for (i=1; i<(int)ktracks.size(); i++) {
		if (!ktracks[i]) {
			continue;
		}
		doTrackKeyAnalysis(analysis[aindex++], level, hop, count,
				trackhist[i], majorweights, minorweights);
	}
}



//////////////////////////////
//
// Tool_transpose::doTrackKeyAnalysis -- Do individual key analyses of sections of the
//   given track.
//

void Tool_transpose::doTrackKeyAnalysis(vector<vector<double> >& analysis, int level, int hop,
		int count, vector<vector<double> >& trackhist,
		vector<double>& majorweights, vector<double>& minorweights) {

	int i;
	for (i=0; i<count; i++) {
		if (i * hop + level > (int)trackhist.size()) {
			break;
		}
		analysis.resize(i+1);
		doSingleAnalysis(analysis[analysis.size()-1], i*hop+level, level,
				trackhist, majorweights, minorweights);
	}
}



//////////////////////////////
//
// Tool_transpose::doSingleAnalysis --
//

void Tool_transpose::doSingleAnalysis(vector<double>& analysis, int startindex, int length,
		vector<vector<double> >& trackhist, vector<double>& majorweights,
		vector<double>& minorweights) {
	vector<double> histsum(12, 0);

	for (int i=0; (i<length) && (startindex+i+length<(int)trackhist.size()); i++) {
		for (int k=0; k<12; k++) {
			histsum[k] += trackhist[i+startindex][k];
		}
	}

	identifyKey(analysis, histsum, majorweights, minorweights);
}



///////////////////////////////
//
// Tool_transpose::fillWeightsWithKostkaPayne --
//

void Tool_transpose::fillWeightsWithKostkaPayne(vector<double>& maj, vector<double>& min) {
	maj.resize(12);
	min.resize(12);

	// found in David Temperley: Music and Probability 2006
	maj[0]  = 0.748;	// C major weights
	maj[1]  = 0.060;	// C#
	maj[2]  = 0.488;	// D
	maj[3]  = 0.082;	// D#
	maj[4]  = 0.670;	// E
	maj[5]  = 0.460;	// F
	maj[6]  = 0.096;	// F#
	maj[7]  = 0.715;	// G
	maj[8]  = 0.104;	// G#
	maj[9]  = 0.366;	// A
	maj[10] = 0.057;	// A#
	maj[11] = 0.400;	// B
	min[0]  = 0.712;	// c minor weights
	min[1]  = 0.084;	// c#
	min[2]  = 0.474;	// d
	min[3]  = 0.618;	// d#
	min[4]  = 0.049;	// e
	min[5]  = 0.460;	// f
	min[6]  = 0.105;	// f#
	min[7]  = 0.747;	// g
	min[8]  = 0.404;	// g#
	min[9]  = 0.067;	// a
	min[10] = 0.133;	// a#
	min[11] = 0.330;	// b
}



////////////////////////////////////////
//
// identifyKey -- correls contains the 12 major key correlation
//      values, then the 12 minor key correlation values, then two
//      more values: index=24 is the best key, and index=25 is the
//      second best key.  If [24] or [25] is -1, then that means that
//      all entries in the original histogram were zero (all rests).
//

void Tool_transpose::identifyKey(vector<double>& correls,
		vector<double>& histogram, vector<double>& majorweights,
		vector<double>& minorweights) {

	correls.clear();
	correls.reserve(26);

	double testsum = 0.0;
	for (int i=0; i<12; i++) {
		testsum += histogram[i];
	}
	if (testsum == 0.0) {
		correls.resize(26);
		fill(correls.begin(), correls.end(), -1);
		correls[24] = -1;
		correls[25] = -1;
		return;
	}

	vector<double> majorcorrels;
	vector<double> minorcorrels;
	for (int i=0; i<12; i++) {
		majorcorrels[i] = Convert::pearsonCorrelation(majorweights, histogram);
		minorcorrels[i] = Convert::pearsonCorrelation(minorweights, histogram);
	}

	// find max value
	int besti;
	int majorbesti = 0;
	int minorbesti = 0;
	for (int i=1; i<12; i++) {
		if (majorcorrels[i] > majorcorrels[majorbesti]) {
			majorbesti = i;
		}
		if (minorcorrels[i] > minorcorrels[minorbesti]) {
			minorbesti = i;
		}
	}
	besti = majorbesti;
	if (majorcorrels[majorbesti] < minorcorrels[minorbesti]) {
		besti = minorbesti + 12;
	}

	// find second best major key
	int majorsecondbesti = 0;
	if (majorbesti == 0) {
		majorsecondbesti = 1;
	}
	for (int i=1; i<12; i++) {
		if (i == majorbesti) {
			continue;
		}
		if (majorcorrels[i] > majorcorrels[majorsecondbesti]) {
			majorsecondbesti = i;
		}
	}

	// find second best minor key
	int minorsecondbesti = 0;
	if (minorbesti == 0) {
		minorsecondbesti = 1;
	}
	for (int i=1; i<12; i++) {
		if (i == minorbesti) {
			continue;
		}
		if (minorcorrels[i] > minorcorrels[minorsecondbesti]) {
			minorsecondbesti = i;
		}
	}
	
	int secondbesti = majorsecondbesti;
	if (majorcorrels[majorsecondbesti] < minorcorrels[minorsecondbesti]) {
		secondbesti = minorsecondbesti;
	}
	secondbesti += 12;

	correls = majorcorrels;
	correls.insert(correls.end(), minorcorrels.begin(), minorcorrels.end());
	correls.push_back(besti);
	correls.push_back(secondbesti);
}



//////////////////////////////
//
// Tool_transpose::printHistograms --
//

void Tool_transpose::printHistograms(int segments, vector<int> ktracks,
vector<vector<vector<double> > >& trackhist) {
	int i, j, k;
	int start;

	for (i=0; i<segments; i++) {
//m_free_text << "i=" << i << endl;
		m_free_text << "segment " << i
				<< " ==========================================\n";
		for (j=0; j<12; j++) {
			start = 0;
//m_free_text << "j=" << i << endl;
			for (k=1; k<(int)ktracks.size(); k++) {
//m_free_text << "k=" << i << endl;
				if (!ktracks[k]) {
					continue;
				}
				if (!start) {
					m_free_text << j;
					start = 1;
				}
				m_free_text << "\t";
				m_free_text << trackhist[k][i][j];
			}
	 if (start) {
				m_free_text << "\n";
			}
		}
	}
	m_free_text << "==========================================\n";
}



//////////////////////////////
//
// Tool_transpose::storeHistogramForTrack --
//

double Tool_transpose::storeHistogramForTrack(vector<vector<double> >& histogram,
		HumdrumFile& infile, int track, int segments) {

	histogram.clear();
	histogram.reserve(segments);

	int i;
	int j;
	int k;

	for (i=0; i<(int)histogram.size(); i++) {
		histogram[i].resize(12);
		fill(histogram[i].begin(), histogram[i].end(), 0);
	}

	double totalduration = infile.getScoreDuration().getFloat();

	double duration;
	string buffer;
	int pitch;
	double start;
	int tokencount;
	for (i=0; i<infile.getLineCount(); i++) {
		if (!infile[i].isData()) {
			continue;
		}
		start = infile[i].getDurationFromStart().getFloat();
		for (j=0; j<infile[i].getFieldCount(); j++) {
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
	 		if (infile.token(i, j)->getTrack() != track) {
				continue;
			}
			if (!infile.token(i, j)->isKern()) {
				continue;
			}
			if (!infile.token(i, j)->isNull()) {
				continue;
			}
			tokencount = infile.token(i, j)->getSubtokenCount();
			for (k=0; k<tokencount; k++) {
				buffer = *infile.token(j, k);
				if (buffer == ".") {
					continue;  // ignore illegal inline null tokens
				}
				pitch = Convert::kernToMidiNoteNumber(buffer);
				if (pitch < 0) {
					continue;  // ignore rests or strange objects
				}
				pitch = pitch % 12;  // convert to chromatic pitch-class
				duration = Convert::recipToDuration(buffer).getFloat();
				if (duration <= 0.0) {
					continue;   // ignore grace notes and strange objects
				}
				addToHistogramDouble(histogram, pitch,
						  start, duration, totalduration, segments);
			}
		}
	}

	return totalduration;
}



//////////////////////////////
//
// Tool_transpose::addToHistogramDouble -- fill the pitch histogram in the right spots.
//

void Tool_transpose::addToHistogramDouble(vector<vector<double> >& histogram, int pc,
		double start, double dur, double tdur, int segments) {

	pc = (pc + 12) % 12;

	double startseg = start / tdur * segments;
	double startfrac = startseg - (int)startseg;

	double segdur = dur / tdur * segments;

	if (segdur <= 1.0 - startfrac) {
		histogram[(int)startseg][pc] += segdur;
		return;
	} else if (1.0 - startfrac > 0.0) {
		histogram[(int)startseg][pc] += (1.0 - startfrac);
		segdur -= (1.0 - startfrac);
	}

	int i = (int)(startseg + 1);
	while (segdur > 0.0 && i < (int)histogram.size()) {
		if (segdur < 1.0) {
			histogram[i][pc] += segdur;
			segdur = 0.0;
		} else {
			histogram[i][pc] += 1.0;
			segdur -= 1.0;
		}
		i++;
	}
}



//////////////////////////////
//
// Tool_transpose::initialize --
//

void Tool_transpose::initialize(HumdrumFile& infile) {

	// handle basic options:
	if (getBoolean("author")) {
		m_free_text << "Written by Craig Stuart Sapp, "
			  << "craig@ccrma.stanford.edu, 12 Apr 2004" << endl;
		exit(0);
	} else if (getBoolean("version")) {
		m_free_text << getArg(0) << ", version: 10 Dec 2016" << endl;
		m_free_text << "compiled: " << __DATE__ << endl;
		exit(0);
	} else if (getBoolean("help")) {
		usage(getArg(0));
		exit(0);
	} else if (getBoolean("example")) {
		example();
		exit(0);
	}

	transval     = getInteger("base40");
	ssetkeyQ     = getBoolean("setkey");
	ssetkey      = Convert::kernToBase40(getString("setkey").data());
	autoQ        = getBoolean("auto");
	debugQ       = getBoolean("debug");
	spineQ       = getBoolean("spines");
	spinestring  = getString("spines");
	octave       = getInteger("octave");
	concertQ     = getBoolean("concert");
	writtenQ     = getBoolean("written");
	quietQ       = getBoolean("quiet");
	instrumentQ  = getBoolean("instrument");

	switch (getBoolean("diatonic") + getBoolean("chromatic")) {
		case 1:
			cerr << "Error: both -d and -c options must be specified" << endl;
			exit(1);
			break;
		case 2:
			{
				char buffer[128] = {0};
				sprintf(buffer, "d%dc%d", getInt("d"), getInt("c"));
				transval = Convert::transToBase40(buffer);
			}
			break;
	}

	ssetkey = ssetkey % 40;

	if (getBoolean("transpose")) {
		transval = getBase40ValueFromInterval(getString("transpose").data());
	}

	transval += 40 * octave;
}



} // end namespace hum
