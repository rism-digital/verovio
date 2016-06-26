//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Aug  8 12:24:49 PDT 2015
// Last Modified: Sat Jun 25 20:55:48 PDT 2016
// Filename:      /include/humlib.cpp
// URL:           https://github.com/craigsapp/humlib/blob/master/src/humlib.cpp
// Syntax:        C++11
// vim:           ts=3
//
// Description:   Source file for humlib library.
//
/*
Copyright (c) 2015 Craig Stuart Sapp
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
// HumAddress::HumAddress -- HumAddress constructor.
//

HumAddress::HumAddress(void) {
	track         = -1;
	subtrack      = -1;
	subtrackcount = 0;
	fieldindex    = -1;
	owner         = NULL;
}



//////////////////////////////
//
// HumAddress::~HumAddress -- HumAddress deconstructor.
//

HumAddress::~HumAddress() {
	track         = -1;
	subtrack      = -1;
	fieldindex    = -1;
	subtrackcount = 0;
	owner         = NULL;
}



//////////////////////////////
//
// HumAddress::getLineIndex -- Returns the line index in the owning HumdrumFile
//    for the token associated with the address.  Returns -1 if not owned by a
//    HumdrumLine (or line assignments have not been made for tokens in the
//    file).
//

int  HumAddress::getLineIndex(void) const {
	if (owner == NULL) {
		return -1;
	} else {
		return owner->getLineIndex();
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
	return fieldindex;
}



//////////////////////////////
//
// HumAddress::getDataType -- Return the exclusive interpretation string of the
//    token associated with the address.
//

const HumdrumToken& HumAddress::getDataType(void) const {
	static HumdrumToken null("");
	if (owner == NULL) {
		return null;
	}
	HumdrumToken* tok = owner->getTrackStart(getTrack());
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
	return spining;
}



//////////////////////////////
//
// HumAddress::getTrack -- The track number of the given spine.  This is the
//   first number in the spine info string.  The track number is the same
//   as a spine number.
//

int HumAddress::getTrack(void) const {
	return track;
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
	return subtrack;
}



//////////////////////////////
//
// HumAddress::getSubtrackCount -- The number of subtrack spines for a
//   given spine on the owning HumdurmLine.  Returns 0 if spine analysis
//   has not been done, or if the line does not have spines (i.e., reference
//   records, global comments and empty lines).
//

int HumAddress::getSubtrackCount(void) const {
	return subtrackcount;
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
	owner = aLine;
}



//////////////////////////////
//
// HumAddress::getLine -- return the HumdrumLine which owns the token
//    associated with this address.  Returns NULL if it does not belong
//    to a HumdrumLine object.
//

HumdrumLine* HumAddress::getLine(void) const {
	return owner;
}



//////////////////////////////
//
// HumAddress::hasOwner -- Returns true if a HumdrumLine owns the token
//    associated with the address.
//

bool HumAddress::hasOwner(void) const {
	return owner == NULL ? 0 : 1;
}



//////////////////////////////
//
// HumAddress::setFieldIndex -- Set the field index of associated token
//   in the HumdrumLine owner.  If the token is now owned by a HumdrumLine,
//   then the input parameter should be -1.
//

void HumAddress::setFieldIndex(int index) {
	fieldindex = index;
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
	spining = spineinfo;
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
	setSubtrack(aTrack);
}


void HumAddress::setTrack(int aTrack) {
	if (aTrack < 0) {
		aTrack = -1;
	}
	if (aTrack > 1000) {
		aTrack = 1000;
	}
	track = aTrack;
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
	subtrack = aSubtrack;
}



//////////////////////////////
//
// HumAddress::setSubtrackCount --
//

void HumAddress::setSubtrackCount(int count) {
	subtrackcount = count;
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
			intvalue = stoi(value);
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
	stringstream ss(value);
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
	int loc = (int)ns.find(":");
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
	int loc = (int)ns.find(":");
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
	int loc = (int)ns.find(":");
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




//////////////////////////////
//
// HumNum::HumNum -- HumNum Constructor.  Set the default value
//   of the number to zero, or the given number if specified.
//

HumNum::HumNum(void){
	setValue(0);
}


HumNum::HumNum(int value){
	setValue(value);
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
	*this = rat;
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
	for (int i=0; i<ratstring.size(); i++) {
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
		lines[i]->printXml(out, level, indent);
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
// HumdrumFileBase::HumdrumFileBase -- HumdrumFileBase constructor.
//

HumdrumFileBase::HumdrumFileBase(void) : HumHash() {
	addToTrackStarts(NULL);
	ticksperquarternote = -1;
	quietParse = false;
}

HumdrumFileBase::HumdrumFileBase(const string& filename) : HumHash() {
	addToTrackStarts(NULL);
	ticksperquarternote = -1;
	quietParse = false;
	read(filename);
}

HumdrumFileBase::HumdrumFileBase(istream& contents) : HumHash() {
	addToTrackStarts(NULL);
	ticksperquarternote = -1;
	quietParse = false;
	read(contents);
}



//////////////////////////////
//
// HumdrumFileBase::~HumdrumFileBase -- HumdrumFileBase deconstructor.
//

HumdrumFileBase::~HumdrumFileBase() { 
	// do nothing
}



//////////////////////////////
//
// HumdrumFileBase::setXmlIdPrefix -- Set the prefix for a HumdrumXML ID
//     atrribute.  The prefix should not start with a digit, nor have
//     spaces in it.
//

void HumdrumFileBase::setXmlIdPrefix(const string& value) {
	idprefix = value;
}



//////////////////////////////
//
// HumdrumFileBase::getXmlIdPrefix -- Return the HumdrumXML ID attribute prefix.
//

string HumdrumFileBase::getXmlIdPrefix(void) {
	return idprefix;
}



//////////////////////////////
//
// HumdrumFileBase::operator[] -- Access a Humdrum file line by and index.
//    Negative values reference the end of the list of lines.
//

HumdrumLine& HumdrumFileBase::operator[](int index) {
	if (index < 0) {
		index = (int)lines.size() - index;
	}
	if ((index < 0) || (index >= lines.size())) {
		cerr << "Error: invalid index: " << index << endl;
		index = (int)lines.size()-1;
	}
	return *lines[index];
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
	parseError = err;
	return !parseError.size();
}


bool HumdrumFileBase::setParseError(stringstream& err) {
	parseError = err.str();
	return !parseError.size();
}


bool HumdrumFileBase::setParseError(const char* format, ...) {
	char buffer[1024] = {0};	
	va_list ap;
	va_start(ap, format);
	snprintf(buffer, 1024, format, ap);
	va_end(ap);
	parseError = buffer;
	return !parseError.size();
}



//////////////////////////////
//
// HumdrumFileBase::read -- Load file contents from an input stream or file.
//

bool HumdrumFileBase::read(const string& filename) {
	displayError = true;
	return HumdrumFileBase::read(filename.c_str());
}


bool HumdrumFileBase::read(const char* filename) {
	displayError = true;
	ifstream infile;
	if ((strlen(filename) == 0) || (strcmp(filename, "-") == 0)) {
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
	displayError = true;
	char buffer[123123] = {0};
	HumdrumLine* s;
	while (contents.getline(buffer, sizeof(buffer), '\n')) {
		s = new HumdrumLine(buffer);
		s->setOwner(this);
		lines.push_back(s);
	}
	if (!analyzeTokens()         ) { return isValid(); }
	if (!analyzeLines()          ) { return isValid(); }
	if (!analyzeSpines()         ) { return isValid(); }
	if (!analyzeLinks()          ) { return isValid(); }
	if (!analyzeTracks()         ) { return isValid(); }
	return isValid();
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
	displayError = true;
	char buffer[123123] = {0};
	HumdrumLine* s;
	while (contents.getline(buffer, sizeof(buffer), '\n')) {
		s = new HumdrumLine;
		s->setLineFromCsv(buffer);
		s->setOwner(this);
		lines.push_back(s);
	}
	if (!analyzeTokens()         ) { return isValid(); }
	if (!analyzeLines()          ) { return isValid(); }
	if (!analyzeSpines()         ) { return isValid(); }
	if (!analyzeLinks()          ) { return isValid(); }
	if (!analyzeTracks()         ) { return isValid(); }
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
	return read(infile);
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
	return parseError;
}



//////////////////////////////
//
// HumdrumFileBase::isValid -- Returns true if last read was
//     successful.
//

bool HumdrumFileBase::isValid(void) {
	if (displayError && (parseError.size() > 0)&& !isQuiet()) {
		cerr << parseError << endl;
		displayError = false;
	}
   return !parseError.size();
}



//////////////////////////////
//
// HumdrumFileBase::setQuietParsing -- Prevent error messages from
//   being displayed when reading data.
// @SEEALSO: setNoisyParsing
// @SEEALSO: isQuiet
//

void HumdrumFileBase::setQuietParsing(void) {
	quietParse = true;
}



//////////////////////////////
//
// HumdrumFileBase::setNoisyParsing -- Display error messages
//   on console when reading data.
// @SEEALSO: setQuietParsing
// @SEEALSO: isQuiet
//

void HumdrumFileBase::setNoisyParsing(void) {
	quietParse = false;
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
	return quietParse;
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
// HumdrumFileBase::analyzeTokens -- Generate token array from
//    current contents of the lines.  If either tokens or the line
//    is changed, then the other state becomes invalid.
//    See createLinesFromTokens for regeneration of lines from tokens.
//

bool HumdrumFileBase::analyzeTokens(void) {
	int i;
	for (i=0; i<lines.size(); i++) {
		lines[i]->createTokensFromLine();
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileBase::createLinesFromTokens -- Generate Humdrum lines strings
//   from the stored list of tokens.
//

void HumdrumFileBase::createLinesFromTokens(void) {
	for (int i=0; i<lines.size(); i++) {
		lines[i]->createLineFromTokens();
	}
}



////////////////////////////
//
// HumdrumFileBase::append -- Add a line to the file's contents.  The file's
//    spine and rhythmic structure should be recalculated after an append.
//

void HumdrumFileBase::append(const char* line) {
	HumdrumLine* s = new HumdrumLine(line);
	lines.push_back(s);
}


void HumdrumFileBase::append(const string& line) {
	HumdrumLine* s = new HumdrumLine(line);
	lines.push_back(s);
}



////////////////////////////
//
// HumdrumFileBase::getLineCount -- Returns the number of lines.
//

int HumdrumFileBase::getLineCount(void) const {
	return (int)lines.size();
}



//////////////////////////////
//
// HumdrumFileBase::token -- Return the token at the given line/field index.
//

HTp HumdrumFileBase::token(int lineindex, int fieldindex) {
	if (lineindex < 0) {
		lineindex += getLineCount();
	}
	return lines[lineindex]->token(fieldindex);
}



//////////////////////////////
//
// HumdrumFileBase::getMaxTrack -- Returns the number of primary
//     spines in the data.
//

int HumdrumFileBase::getMaxTrack(void) const {
	return (int)trackstarts.size() - 1;
}



//////////////////////////////
//
// HumdrumFileBase::printSpineInfo -- Print the spine information for all
//    lines/tokens in file (for debugging).
//

ostream& HumdrumFileBase::printSpineInfo(ostream& out) {
	for (int i=0; i<getLineCount(); i++) {
		lines[i]->printSpineInfo(out) << '\n';
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
		lines[i]->printDataTypeInfo(out) << '\n';
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
		lines[i]->printTrackInfo(out) << '\n';
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
	spinestarts.reserve(trackstarts.size());
	spinestarts.resize(0);
	for (int i=1; i<trackstarts.size(); i++) {
		spinestarts.push_back(trackstarts[i]);
	}
}


void HumdrumFileBase::getSpineStartList(vector<HTp>& spinestarts,
		const string& exinterp) {
	spinestarts.reserve(trackstarts.size());
	spinestarts.resize(0);
	for (int i=1; i<trackstarts.size(); i++) {
		if (exinterp == *trackstarts[i]) {
			spinestarts.push_back(trackstarts[i]);
		}
	}
}


void HumdrumFileBase::getSpineStartList(vector<HTp>& spinestarts,
		const vector<string>& exinterps) {
	spinestarts.reserve(trackstarts.size());
	spinestarts.resize(0);
	for (int i=1; i<trackstarts.size(); i++) {
		for (int j=0; j<exinterps.size(); j++) {
			if (exinterps[j] == *trackstarts[i]) {
				spinestarts.push_back(trackstarts[i]);
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


void HumdrumFileBase::getSpineSequence(vector<vector<HTp> >& sequence, int spine,
		int options) {
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
	for (int i=0; i<tempseq.size(); i++) {
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
	if ((track > 0) && (track < trackstarts.size())) {
		return trackstarts[track];
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
		track += (int)trackends.size();
	}
	if (track < 0) {
		return 0;
	}
	if (track >= (int)trackends.size()) {
		return 0;
	}
	return (int)trackends[track].size();
}



//////////////////////////////
//
// HumdrumFileBase::getTrackEnd -- Returns a pointer to the terminal manipulator
//    token for the given track and subtrack.  Sub-tracks are indexed from 0 up
//    to but not including getTrackEndCount.
//

HTp HumdrumFileBase::getTrackEnd(int track, int subtrack) const {
	if (track < 0) {
		track += (int)trackends.size();
	}
	if (track < 0) {
		return NULL;
	}
	if (track >= (int)trackends.size()) {
		return NULL;
	}
	if (subtrack < 0) {
		subtrack += (int)trackends[track].size();
	}
	if (subtrack < 0) {
		return NULL;
	}
	if (subtrack >= (int)trackends[track].size()) {
		return NULL;
	}
	return trackends[track][subtrack];
}



//////////////////////////////
//
// HumdrumFileBase::analyzeLines -- Store a line's index number in the
//    HumdrumFile within the HumdrumLine object at that index.
//    Returns false if there was an error.
//

bool HumdrumFileBase::analyzeLines(void) {
	for (int i=0; i<lines.size(); i++) {
		lines[i]->setLineIndex(i);
	}
	return isValid();
}



//////////////////////////////
//
// HumdrumFileBase::analyzeTracks -- Analyze the track structure of the
//     data.  Returns false if there was a parse error.
//

bool HumdrumFileBase::analyzeTracks(void) {
	for (int i=0; i<lines.size(); i++) {
		int status = lines[i]->analyzeTracks(parseError);
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

	for (int i=0; i<lines.size(); i++) {
		if (!lines[i]->hasSpines()) {
			continue;
		}
		previous = next;
		next = lines[i];
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
	trackstarts.resize(0);
	trackends.resize(0);
	addToTrackStarts(NULL);

	bool init = false;
	int i, j;
	for (i=0; i<getLineCount(); i++) {
		if (!lines[i]->hasSpines()) {
			lines[i]->token(0)->setFieldIndex(0);
			continue;
		}
		if ((init == false) && !lines[i]->isExclusive()) {
			stringstream err;
			err << "Error on line: " << (i+1) << ':' << endl;
			err << "   Data found before exclusive interpretation" << endl;
			err << "   LINE: " << *lines[i];
			return setParseError(err);
		}
		if ((init == false) && lines[i]->isExclusive()) {
			// first line of data in file.
			init = true;
			datatype.resize(lines[i]->getTokenCount());
			sinfo.resize(lines[i]->getTokenCount());
			lastspine.resize(lines[i]->getTokenCount());
			for (j=0; j<lines[i]->getTokenCount(); j++) {
				datatype[j] = lines[i]->getTokenString(j);
				addToTrackStarts(lines[i]->token(j));
				sinfo[j]    = to_string(j+1);
				lines[i]->token(j)->setSpineInfo(sinfo[j]);
				lines[i]->token(j)->setFieldIndex(j);
				lastspine[j].push_back(lines[i]->token(j));
			}
			continue;
		}
		if (datatype.size() != lines[i]->getTokenCount()) {
			stringstream err;
			err << "Error on line " << (i+1) << ':' << endl;
			err << "   Expected " << datatype.size() << " fields,"
			     << " but found " << lines[i]->getTokenCount();
			return setParseError(err);
		}
		for (j=0; j<lines[i]->getTokenCount(); j++) {
			lines[i]->token(j)->setSpineInfo(sinfo[j]);
			lines[i]->token(j)->setFieldIndex(j);
		}
		if (!lines[i]->isManipulator()) {
			continue;
		}
		if (!adjustSpines(*lines[i], datatype, sinfo)) { return isValid(); }
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
		trackstarts.push_back(NULL);
		trackends.resize(trackends.size()+1);
	} else if ((trackstarts.size() > 1) && (trackstarts.back() == NULL)) {
		trackstarts.back() = token;
	} else {
		trackstarts.push_back(token);
		trackends.resize(trackends.size()+1);
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
			trackends[trackstarts.size()-1].push_back(line.token(i));
		} else if (((string*)line.token(i))->substr(0, 2) == "**") {
			newtype.resize(newtype.size() + 1);
			newtype.back() = line.getTokenString(i);
			newinfo.resize(newinfo.size() + 1);
			newinfo.back() = sinfo[i];
			if (!((trackstarts.size() > 1) && (trackstarts.back() == NULL))) {
				stringstream err;
				err << "Error: Exclusive interpretation with no preparation "
				     << "on line " << line.getLineIndex()
				     << " spine index " << i << endl;
				err << "Line: " << line;
				return setParseError(err);
			}
			if (trackstarts.back() == NULL) {
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
	for (i=0; i<newtype.size(); i++) {
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

	// analyze backward tokens:
	for (int i=1; i<=getMaxTrack(); i++) {
		for (int j=0; j<getTrackEndCount(i); j++) {
			if (!processNonNullDataTokensForTrackBackward(getTrackEnd(i, j),
					ptokens)) {
				return false;
			}
		}
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
		if (token->isData()) {
			addUniqueTokens(token->nextNonNullTokens, ptokens);
			if (!token->isNull()) {
				ptokens.resize(0);
				ptokens.push_back(token);
			}
		}
		// Data tokens can only be followed by up to one previous token,
		// so no need to check for more than one next token.
		token = token->getPreviousToken(0);
		tcount = token->getPreviousTokenCount();
	}

	return true;
}



//////////////////////////////
//
// HumdurmFile::processNonNullDataTokensForTrackForward -- Helper function
//    for analyzeNonNullDataTokens.  Given any token, this function tells
//    you what is the previous non-null data token(s) in the spine after
//    the given token.
//

bool HumdrumFileBase::processNonNullDataTokensForTrackForward(
		HTp starttoken, vector<HTp> ptokens) {
	HTp token = starttoken;
	int tcount = token->getNextTokenCount();
	while (tcount > 0) {
		if (!token->isData()) {
			for (int i=1; i<tcount; i++) {
				if (!processNonNullDataTokensForTrackForward(
						token->getNextToken(i), ptokens)) {
					return false;
				}
			}
		} else {
			addUniqueTokens(token->previousNonNullTokens, ptokens);
			if (!token->isNull()) {
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
	for (i=0; i<source.size(); i++) {
		found = false;
		for (j=0; j<target.size(); j++) {
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
	for (i=0; i<ktracks.size(); i++) {
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

	// dstates == diatonic states for every pitch in a spine
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


	// rhythmstart == keep track of first non-grace note in measure.
	vector<int> foundrhythm(kcount, 0);
	
	int loc;
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
				std::fill(foundrhythm.begin(), foundrhythm.end(), 0);
				track = infile[i].token(j)->getTrack();
				kindex = rtracks[track];
				// reset the accidental states in dstates to match keysigs.
				resetDiatonicStatesWithKeySignature(dstates[kindex],
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
			if (infile[i].token(j)->find("q") != string::npos) {
				// deal with grace notes later...
				continue;
			}
			int subcount = infile[i].token(j)->getSubtokenCount();
			track = infile[i].token(j)->getTrack();
			int rindex = rtracks[track];
			for (k=0; k<subcount; k++) {
				string subtok = infile[i].token(j)->getSubtoken(k);
				int diatonic = Convert::kernToBase7(subtok);
				if (diatonic < 0) {
					// Deal with extra-low notes later.
					continue;
				}
				int accid = Convert::kernToAccidentalCount(subtok);
				if ((subtok.find("_") != string::npos) ||
						(subtok.find("]") != string::npos)) {
					// tied notes do not have slurs, so skip them
					if ((accid != keysigs[rindex][diatonic % 7]) &&
							(foundrhythm[rindex] == 0)) {
						// But first, prepare to force an accidental to be shown on
						// the note immediately following the end of a tied group
						// if the tied group crosses a barline.
						dstates[rindex][diatonic] = -1000 + accid;
					}
					continue;
				}
				if (accid != dstates[rindex][diatonic]) {
					// accidental is different from the previous state so should be
					// printed
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
					dstates[rindex][diatonic] = accid;
				} else if ((accid == 0) && (subtok.find("n") != string::npos)) {
					infile[i].token(j)->setValue("auto", to_string(k),
							"cautionaryAccidental", "true");
					infile[i].token(j)->setValue("auto", to_string(k),
							"visualAccidental", "true");
				} else if (subtok.find("XX") == string::npos) {
					// The accidental is not necessary. See if there is a single "X"
					// immediately after the accidental which means to force it to
					// display.
					loc = (int)subtok.find("X");
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
// HumdrumFileContent::analyzeKernSlurs -- Link start and ends of
//    slurs to each other.
//

bool HumdrumFileContent::analyzeKernSlurs(void) {
	vector<HTp> kernspines;
	getSpineStartList(kernspines, "**kern");
	bool output = true;
	for (int i=0; i<kernspines.size(); i++) {
		output = output && analyzeKernSlurs(kernspines[i]);
	}
	return output;
}


bool HumdrumFileContent::analyzeKernSlurs(HTp spinestart) {
	vector<vector<HTp> > tracktokens;
	this->getTrackSeq(tracktokens, spinestart, OPT_DATA | OPT_NOEMPTY);
	// printSequence(tracktokens);

	vector<vector<HTp> > sluropens;
	sluropens.resize(32);

	int elisionlevel;
	int i, j;
	for (i=0; i<tracktokens.size(); i++) {
		for (j=0; j<tracktokens[i].size(); j++) {
			if (tracktokens[i][j]->hasSlurStart()) {
				elisionlevel = tracktokens[i][j]->getSlurStartElisionLevel();
				if (elisionlevel >= 0) {
					sluropens[elisionlevel].push_back(tracktokens[i][j]);
				}
			}
			if (tracktokens[i][j]->hasSlurEnd()) {
				elisionlevel = tracktokens[i][j]->getSlurEndElisionLevel();
				if (elisionlevel >= 0) {
					if (sluropens[elisionlevel].size() > 0) {
						sluropens[elisionlevel].back()->setValue("auto",
								"slurEnd", tracktokens[i][j]);
						sluropens[elisionlevel].back()->setValue("auto",
								"id", sluropens[elisionlevel].back());
						tracktokens[i][j]->setValue("auto", "slurStart",
								sluropens[elisionlevel].back());
						tracktokens[i][j]->setValue("auto", "id",
								tracktokens[i][j]);
						sluropens[elisionlevel].back()->setValue("auto", "slurDuration",
							tracktokens[i][j]->getDurationFromStart() - 
							sluropens[elisionlevel].back()->getDurationFromStart());
						sluropens[elisionlevel].pop_back();
					} else {
						// no starting slur marker to match to this slur end.
						tracktokens[i][j]->setValue("auto", "hangingSlur", "true");
						tracktokens[i][j]->setValue("auto", "slurDration", 
							tracktokens[i][j]->getDurationToEnd());
					}
				}
			}
		}
	}

	// Mark un-closed slur starts:
	for (i=0; i<sluropens.size(); i++) {
		for (j=0; j<sluropens[i].size(); j++) {
			sluropens[i][j]->setValue("", "auto", "hangingSlur", "true");
			sluropens[i][j]->setValue("", "auto", "slurDuration", 
				sluropens[i][j]->getDurationFromStart());
		}
	}

	return true;
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
	for (int i=0; i<kernspines.size(); i++) {
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
	displayError = false;
	if (!readNoRhythm(contents)) {
		return isValid();
	} 
	return analyzeStructure();
}


bool HumdrumFileStructure::read(const char* filename) {
	displayError = false;
	if (!readNoRhythm(filename)) {
		return isValid();
	}
	return analyzeStructure();
}
 

bool HumdrumFileStructure::read(const string& filename) {
	displayError = false;
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
	displayError = false;
	if (!readNoRhythmCsv(contents, separator)) {
		return isValid();
	}
	return analyzeStructure();
}


bool HumdrumFileStructure::readCsv(const char* filename,
		const string& separator) {
	displayError = false;
	if (!readNoRhythmCsv(filename, separator)) {
		return isValid();
	}
	return analyzeStructure();
}


bool HumdrumFileStructure::readCsv(const string& filename,
		const string& separator) {
	displayError = false;
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
	displayError = false;
	if (!HumdrumFileBase::readString(contents)) {
		return isValid();
	}
	return analyzeStructure();
}


bool HumdrumFileStructure::readString(const string& contents) {
	displayError = false;
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
	displayError = false;
	if (!HumdrumFileBase::readStringCsv(contents, separator)) {
		return isValid();
	}
	return analyzeStructure();
}


bool HumdrumFileStructure::readStringCsv(const string& contents,
		const string& separator) {
	displayError = false;
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
	if (!analyzeRhythm()           ) { return isValid(); }
	if (!analyzeDurationsOfNonRhythmicSpines()) { return isValid(); }
	return isValid();
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
	if (lines.size() == 0) {
		return 0;
	}
	return lines.back()->getDurationFromStart();
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
	if (ticksperquarternote > 0) {
		return ticksperquarternote;
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
	ticksperquarternote = lcm;
	return ticksperquarternote;
}



//////////////////////////////
//
// HumdrumFileStructure::getPositiveLineDurations -- Return a list of all
//    unique token durations in the file.  This function could be expanded
//    to limit the search to a range of lines or to a specific track.
//

set<HumNum> HumdrumFileStructure::getPositiveLineDurations(void) {
	set<HumNum> output;
	for (auto& line : lines) {
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
		lines[i]->printDurationInfo(out) << '\n';
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
		index += barlines.size();
	}
	if (index < 0) {
		return NULL;
	}
	if (index >= barlines.size()) {
		return NULL;
	}
	return barlines[index];
}



//////////////////////////////
//
// HumdrumFileStructure::getBarlineCount -- Return the number of barlines in
//   the file.  If there is a pickup beat, then the count includes an imaginary
//   barline before the first pickup (and the start of the file will be returned
//   for barline(0).
//

int HumdrumFileStructure::getBarlineCount(void) const {
	return (int)barlines.size();
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
		index += barlines.size();
	}
	if (index < 0) {
		return 0;
	}
	if (index >= barlines.size()) {
		return 0;
	}
	HumNum startdur = barlines[index]->getDurationFromStart();
	HumNum enddur;
	if (index + 1 < barlines.size() - 1) {
		enddur = barlines[index+1]->getDurationFromStart();
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
		index += barlines.size();
	}
	if (index < 0) {
		return 0;
	}
	if (index >= barlines.size()) {
		return getScoreDuration();
	}
	return barlines[index]->getDurationFromStart();
}



///////////////////////////////
//
// HumdrumFileStructure::getBarlineDurationToEnd -- Return the duration
//    between barline and the end of the HumdrumFileStructure.
//

HumNum HumdrumFileStructure::getBarlineDurationToEnd(int index) const {
	if (index < 0) {
		index += barlines.size();
	}
	if (index < 0) {
		return 0;
	}
	if (index >= barlines.size()) {
		return getScoreDuration();
	}
	return barlines[index]->getDurationToEnd();
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

	barlines.resize(0);

	int i;
	HumNum sum = 0;
	bool foundbarline = false;
	for (i=0; i<getLineCount(); i++) {
		lines[i]->setDurationFromBarline(sum);
		sum += lines[i]->getDuration();
		if (lines[i]->isBarline()) {
			foundbarline = true;
			barlines.push_back(lines[i]);
			sum = 0;
		}
		if (lines[i]->isData() && !foundbarline) {
			// pickup measure, so set the first measure to the start of the file.
			barlines.push_back(lines[0]);
			foundbarline = 1;
		}
	}

	sum = 0;
	for (i=getLineCount()-1; i>=0; i--) {
		sum += lines[i]->getDuration();
		lines[i]->setDurationToBarline(sum);
		if (lines[i]->isBarline()) {
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
		if (!lines[i]->analyzeTokenDurations(parseError)) {
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
	for (int i=(int)lines.size()-1; i>=0; i--) {
		if (lines[i]->hasSpines()) {
			if (lines[i]->isAllNull())  {
				continue;
			}
			if (lines[i]->isManipulator()) {
				continue;
			}
			if (lines[i]->isCommentLocal()) {
				continue;
			}
			// should be a non-null data, barlines, or interpretation
			spineline = lines[i];
			continue;
		}
		if (spineline == NULL) {
			continue;
		}
		if (!lines[i]->isCommentGlobal()) {
			continue;
		}
		if (lines[i]->find("!!LO:") != 0) {
			continue;
		}
		spineline->setParameters(lines[i]);
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
	for (int i=0; i<lines[line]->getTokenCount(); i++) {
		HumNum dur = lines[line]->token(i)->getDuration();
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
		err << "Line: " << *lines[line] << endl;
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
		if (!lines[line]->token(i)->hasRhythm()) {
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

	HumdrumToken* initial = token;
	HumNum dursum = startdur;
	token->incrementState();

	if (!setLineDurationFromStart(token, dursum)) { return isValid(); }
	if (token->getDuration().isPositive()) {
		dursum += token->getDuration();
	}
	int tcount = token->getNextTokenCount();

	// Assign line durationFromStarts for primary track first.
	while (tcount > 0) {
		token = token->getNextToken(0);
		if (state != token->getState()) {
			return isValid();
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
	int newstate = state + 1;

	token = initial;
	dursum = startdur;
	if (token->getDuration().isPositive()) {
		dursum += token->getDuration();
	}
	tcount = token->getNextTokenCount();
	while (tcount > 0) {
		if (tcount > 1) {
			for (int i=1; i<tcount; i++) {
				if (!prepareDurations(token->getNextToken(i), state, dursum)) {
					return isValid();
				}
			}
		}
		token = token->getNextToken(0);
		if (newstate != token->getState()) {
			return isValid();
		}
		if (token->getDuration().isPositive()) {
			dursum += token->getDuration();
		}
		tcount = token->getNextTokenCount();
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
	for (i=0; i<(int)lines.size(); i++) {
		if (!lines[i]->hasSpines()) {
			continue;
		}
		if (lines[i]->isAllRhythmicNull()) {
			if (lines[i]->isData()) {
				nulllines.push_back(lines[i]);
			}
			continue;
		}
		dur = lines[i]->getDurationFromStart();
		if (dur.isNegative()) {
			if (lines[i]->isData()) {
				stringstream err;
				err << "Error: found an unexpected negative duration on line "
			       << lines[i]->getDurationFromStart()<< endl;
				err << "Line: " << *lines[i] << endl;
				return setParseError(err);
			} else {
				continue;
			}
		}
		next = lines[i];
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
	for (i=(int)lines.size()-1; i>=0; i--) {
		dur = lines[i]->getDurationFromStart();
		if (dur.isNegative() && lastdur.isNonNegative()) {
			lines[i]->setDurationFromStart(lastdur);
		}
		if (dur.isNonNegative()) {
			lastdur = dur;
			continue;
		}
	}

	// fill in start times for ending comments
	for (i=0; i<lines.size(); i++) {
		dur = lines[i]->getDurationFromStart();
		if (dur.isNonNegative()) {
			lastdur = dur;
		} else {
			lines[i]->setDurationFromStart(lastdur);
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
	for (int i=0; i<lines.size()-1; i++) {
		startdur = lines[i]->getDurationFromStart();
		enddur = lines[i+1]->getDurationFromStart();
		dur = enddur - startdur;
		lines[i]->setDuration(dur);
	}
	if (lines.size() > 0) {
		lines.back()->setDuration(0);
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
// HumdrumFileStructure::processLocalParametersForTrack --  Search for local
//   parameters in each spine and fill in the HumHash for the token to which the
//   parameter is to be applied.
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
		if (!(token->isNull() && token->isManipulator())) {
			if (token->isCommentLocal()) {
				checkForLocalParameters(token, current);
			} else {
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
	strand1d.resize(0);
	strand2d.resize(0);
	int i, j;
	for (i=0; i<spines; i++) {
		HumdrumToken* tok = getSpineStart(i);
		strand2d.resize(strand2d.size()+1);
		analyzeSpineStrands(strand2d.back(), tok);
	}

	for (i=0; i<strand2d.size(); i++) {
		std::sort(strand2d[i].begin(), strand2d[i].end(), 
				sortTokenPairsByLineIndex);
		for (j=0; j<strand2d[i].size(); j++) {
			strand1d.push_back(strand2d[i][j]);
		}
	}

	assignStrandsToTokens();
	
	return isValid();
}



//////////////////////////////
//
// HumdrumFileStructure::assignStrandsToTokens -- Store the 1D strand
//    index number for each token in the file.  Global tokens will have
//    strand index set to -1.
//

void HumdrumFileStructure::assignStrandsToTokens(void) {
	HTp tok;
	for (int i=0; i<strand1d.size(); i++) {
		tok = strand1d[i].first;
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
// HumdrumFileStructure::getStrandStart -- Return the first token
//    in the a strand.
//

HumdrumToken* HumdrumFileStructure::getStrandStart(int index) const {
	return strand1d[index].first;
}


HumdrumToken* HumdrumFileStructure::getStrandEnd(int index) const {
	return strand1d[index].last;
}


HumdrumToken* HumdrumFileStructure::getStrandStart(int sindex, 
		int index) const {
	return strand2d[sindex][index].first;
}


HumdrumToken* HumdrumFileStructure::getStrandEnd(int sindex, int index) const {
	return strand2d[sindex][index].last;
}




//////////////////////////////
//
// HumdrumLine::HumdrumLine -- HumdrumLine constructor.
//

HumdrumLine::HumdrumLine(void) : string() {
	owner = NULL;
	duration = -1;
	durationFromStart = -1;
	setPrefix("!!");
}

HumdrumLine::HumdrumLine(const string& aString) : string(aString) {
	owner = NULL;
	duration = -1;
	durationFromStart = -1;
	setPrefix("!!");
}

HumdrumLine::HumdrumLine(const char* aString) : string(aString) {
	owner = NULL;
	duration = -1;
	durationFromStart = -1;
	setPrefix("!!");
}



//////////////////////////////
//
// HumdrumLine::~HumdrumLine -- HumdrumLine deconstructor.
//

HumdrumLine::~HumdrumLine() {
	// free stored HumdrumTokens:
	clear();
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
	// construct tab-delimited string
	string output;
	bool inquote = false;
	for (int i=0; i<csv.size(); i++) {
		if ((csv[i] == '"') && !inquote) {
			inquote = true;
			continue;
		}
		if (inquote && (csv[i] == '"') && (i < csv.length()-1)) {
			output += '"';
			i++;
			continue;
		}
		if (csv[i] == '"') {
			inquote = false;
			continue;
		}
		if ((!inquote) && (csv.substr(i, separator.size()) == separator)) {
			output += '\t';
			i += separator.size() - 1;
			continue;
		}
		output += csv[i];
	}
	string& value = *this;
	value = output;
}



//////////////////////////////
//
// HumdrumLine::clear -- Remove stored tokens.
//

void HumdrumLine::clear(void) {
	for (int i=0; i<tokens.size(); i++) {
		delete tokens[i];
		tokens[i] = NULL;
	}
}



//////////////////////////////
//
// HumdrumLine::equalChar -- return true if the character at the given
//     index is the given char.
//

bool HumdrumLine::equalChar(int index, char ch) const {
	if (size() <= index) {
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
	HumdrumToken* ntok;
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
	lineindex = index;
}



//////////////////////////////
//
// HumdrumLine::getLineIndex -- Returns the index number of the line in the
//    HumdrumFileBase storage for the lines.
//

int HumdrumLine::getLineIndex(void) const {
	return lineindex;
}



//////////////////////////////
//
// HumdrumLine::getLineNumber -- Returns the line index plus one.
//

int HumdrumLine::getLineNumber(void) const {
	return lineindex + 1;
}



//////////////////////////////
//
// HumdrumLine::getDuration -- Get the duration of the line.  The duration will
//    be negative one if rhythmic analysis in HumdrumFileStructure has not been
//    done on the owning HumdrumFile object.  Otherwise this is the duration of
//    the current line in the file.
//

HumNum HumdrumLine::getDuration(void) const {
	return duration;
}


HumNum HumdrumLine::getDuration(HumNum scale) const {
	return duration * scale;
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
	durationFromStart = dur;
}



//////////////////////////////
//
// HumdrumLine::getDurationFromStart -- Get the duration from the start of the
//    file to the start of the current line.  This will be -1 if rhythmic
//    analysis has not been done in the HumdrumFileStructure class.
//

HumNum HumdrumLine::getDurationFromStart(void) const {
	return durationFromStart;
}


HumNum HumdrumLine::getDurationFromStart(HumNum scale) const {
	return durationFromStart * scale;
}



//////////////////////////////
//
// HumdrumLine::getDurationToEnd -- Returns the duration from the start of the
//    line to the end of the HumdrumFile which owns this HumdrumLine.  The
//    rhythm of the HumdrumFile must be analyze before using this function;
//    otherwise a 0 will probably be returned.
//

HumNum HumdrumLine::getDurationToEnd(void) const {
	if (owner == NULL) {
		return 0;
	}
	return ((HumdrumFile*)owner)->getScoreDuration() -  durationFromStart;
}


HumNum HumdrumLine::getDurationToEnd(HumNum scale) const {
	if (owner == NULL) {
		return 0;
	}
	return scale * (((HumdrumFile*)owner)->getScoreDuration() -
		durationFromStart);
}



//////////////////////////////
//
// HumdrumLine::getDurationFromBarline -- Returns the duration from the start
//    of the given line to the first barline occurring before the given line.
//    Analysis of this data is found in HumdrumFileStructure::metricAnalysis.
//

HumNum HumdrumLine::getDurationFromBarline(void) const {
	return durationFromBarline;
}


HumNum HumdrumLine::getDurationFromBarline(HumNum scale) const {
	return durationFromBarline * scale;
}



//////////////////////////////
//
// HumdrumLine::getTrackStart --  Returns the starting exclusive interpretation
//    for the given spine/track.
//

HumdrumToken* HumdrumLine::getTrackStart(int track) const {
	if (owner == NULL) {
		return NULL;
	} else {
		return ((HumdrumFile*)owner)->getTrackStart(track);
	}
}



//////////////////////////////
//
// HumdrumLine::setDurationFromBarline -- Time from the previous
//    barline to the current line.  This function is used in analyzeMeter in
//    the HumdrumFileStructure class.
//

void HumdrumLine::setDurationFromBarline(HumNum dur) {
	durationFromBarline = dur;
}



//////////////////////////////
//
// HumdrumLine::getDurationToBarline -- Time from the starting of the
//   current note to the next barline.
//

HumNum HumdrumLine::getDurationToBarline(void) const {
	return durationToBarline;
}


HumNum HumdrumLine::getDurationToBarline(HumNum scale) const {
	return durationToBarline * scale;
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
	durationToBarline = dur;
}



//////////////////////////////
//
// HumdrumLine::setDuration -- Sets the duration of the line.  This is done
//   in the rhythmic analysis for the HumdurmFileStructure class.
//

void HumdrumLine::setDuration(HumNum aDur) {
	if (aDur.isNonNegative()) {
		duration = aDur;
	} else {
		duration = 0;
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
	for (int i=0; i<tokens.size(); i++) {
		if (tokens[i]->isManipulator()) {
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
	return (int)tokens.size();
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
	return tokens[index];
}



//////////////////////////////
//
// HumdrumLine::getTokenString -- Returns a copy of the string component of
//     a token.  This code will return a segmentation fault if index is out of
//     range...
//

string HumdrumLine::getTokenString(int index) const {
	return (string(*tokens[index]));
}


//////////////////////////////
//
// HumdrumLine::createTokensFromLine -- Chop up a HumdrumLine string into
//     individual tokens.
//

int HumdrumLine::createTokensFromLine(void) {
	tokens.resize(0);
	HumdrumToken* token = new HumdrumToken();
	token->setOwner(this);
	char ch;
	for (int i=0; i<size(); i++) {
		ch = getChar(i);
		if (ch == '\t') {
			tokens.push_back(token);
			token = new HumdrumToken();
			token->setOwner(this);
		} else {
			*token += ch;
		}
	}
	tokens.push_back(token);
	return (int)tokens.size();
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
	iline.resize(0);
	for (int i=0; i<tokens.size(); i++) {
		iline += (string)(*tokens[i]);
		if (i < tokens.size() - 1) {
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

void HumdrumLine::getTokens(vector<HumdrumToken*>& list) {
	if (tokens.size() == 0) {
		createTokensFromLine();
	}
	list = tokens;
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
	if (index >= size()) {
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
		for (int i=0; i<tokens.size(); i++) {
			out << tokens[i]->getSpineInfo();
			if (i < tokens.size() - 1) {
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
		for (int i=0; i<tokens.size(); i++) {
			out << tokens[i]->getDataType().substr(2, string::npos);
			if (i < tokens.size() - 1) {
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
	for (int i=0; i<tokens.size(); i++) {
		if (!tokens[i]->analyzeDuration(err)) {
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

	for (i=0; i<tokens.size(); i++) {
		info = tokens[i]->getSpineInfo();
		track = 0;
		for (j=0; j<info.size(); j++) {
			if (!isdigit(info[j])) {
				continue;
			}
			track = info[j] - '0';
			for (k=j+1; k<info.size(); k++) {
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
		tokens[i]->setTrack(track);
	}

	int subtrack;
	vector<int> subtracks;
	vector<int> cursub;

	subtracks.resize(maxtrack+1);
	cursub.resize(maxtrack+1);
	fill(subtracks.begin(), subtracks.end(), 0);
	fill(cursub.begin(), cursub.end(), 0);

	for (i=0; i<tokens.size(); i++) {
		subtracks[tokens[i]->getTrack()]++;
	}
	for (i=0; i<tokens.size(); i++) {
		track = tokens[i]->getTrack();
		subtrack = subtracks[track];
		if (subtrack > 1) {
			tokens[i]->setSubtrack(++cursub[tokens[i]->getTrack()]);
		} else {
			tokens[i]->setSubtrack(0);
		}
		tokens[i]->setSubtrackCount(subtracks[track]);
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
		for (int i=0; i<tokens.size(); i++) {
			tokens[i]->getDuration().printMixedFraction(out);
			if (i < tokens.size() - 1) {
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
		out << "/>\n";
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
			out << "<referenceKey>" << Convert::encodeXml(getReferenceKey());
			out << "</referenceKey>\n";

			out << Convert::repeatString(indent, level);
			out << "<referenceValue>" << Convert::encodeXml(getReferenceValue());
			out << "</referenceValue>\n";
		}

		level--;
		out << Convert::repeatString(indent, level) << "<frameInfo>\n";


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
	if (owner == NULL) {
		return "";
	}
	return ((HumdrumFileBase*)owner)->getXmlIdPrefix();
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
		for (int i=0; i<tokens.size(); i++) {
			out << tokens[i]->getTrackString();
			if (i < tokens.size() - 1) {
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
	owner = hfile;
}



//////////////////////////////
//
// HumdrumLine::getOwner -- Return the HumdrumFile which manages
//   (owns) this line.
//

HumdrumFile* HumdrumLine::getOwner(void) {
	return (HumdrumFile*)owner;
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
	for (int i=2; i<pieces.size(); i++) {
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
// operator<< -- Print a HumdrumLine. Needed to avoid interaction with
//     HumHash parent class.
//

ostream& operator<<(ostream& out, HumdrumLine& line) {
	out << (string)line;
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
	rhycheck = 0;
	setPrefix("!");
	strand = -1;
}

HumdrumToken::HumdrumToken(const string& aString) : string(aString) {
	rhycheck = 0;
	setPrefix("!");
	strand = -1;
}

HumdrumToken::HumdrumToken(const char* aString) : string(aString) {
	rhycheck = 0;
	setPrefix("!");
	strand = -1;
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
	if (size() <= index) {
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
	return (int)previousNonNullTokens.size();
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
		index += (int)previousNonNullTokens.size();
	}
	if (index < 0) {
		return NULL;
	}
	if (index >= (int)previousNonNullTokens.size()) {
		return NULL;
	}
	return previousNonNullTokens[index];
}



//////////////////////////////
//
// HumdrumToken::getNextNonNullDataTokenCount -- Returns the number of non-null
//     data tokens which follow this token in the spine.
//

int HumdrumToken::getNextNonNullDataTokenCount(void) {
	return (int)nextNonNullTokens.size();
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
		index += nextNonNullTokens.size();
	}
	if (index < 0) {
		return NULL;
	}
	if (index >= nextNonNullTokens.size()) {
		return NULL;
	}
	return nextNonNullTokens[index];
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
	return address.getDataType();
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
	address.setSpineInfo(spineinfo);
}



//////////////////////////////
//
// HumdrumToken::getSpineInfo -- Returns the spine split/merge history
//    for the token.
// @SEEALTO: setSpineInfo
//

string HumdrumToken::getSpineInfo(void) const {
	return address.getSpineInfo();
}



//////////////////////////////
//
// HumdrumToken::getLineIndex -- Returns the line index of the owning
//    HumdrumLine for this token.
// @SEEALTO: getLineNumber
//

int HumdrumToken::getLineIndex(void) const {
	return address.getLineIndex();
}



//////////////////////////////
//
// HumdrumToken::getFieldIndex -- Returns the index of the token the line.
// @SEEALSO: getFieldIndex
//

int HumdrumToken::getFieldIndex(void) const {
	return address.getFieldIndex();
}



//////////////////////////////
//
// HumdrumToken::getLineNumber -- Returns the line index plus 1.
// @SEEALTO: getLineIndex
//

int HumdrumToken::getLineNumber(void) const {
	return address.getLineNumber();
}



//////////////////////////////
//
// HumdrumToken::setFieldIndex -- Sets the field index of the token on the
//   owning HumdrumLine object.
// @SEEALSO: getFieldIndex
//

void HumdrumToken::setFieldIndex(int index) {
	address.setFieldIndex(index);
}



//////////////////////////////
//
// HumdrumToken::setTrack -- Sets the track number (similar to a staff in MEI).
//     The two-parameter version will set the track and sub-track at the same
//     time (subtrack is similar to a staff and layer in MEI).
//

void HumdrumToken::setTrack(int aTrack) {
	address.setTrack(aTrack);
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
	return address.getTrack();
}



//////////////////////////////
//
// HumdrumToken::setSubtrack -- Sets the subtrack (similar to a layer
//    in MEI).
//

void HumdrumToken::setSubtrack(int aSubtrack) {
	address.setSubtrack(aSubtrack);
}



//////////////////////////////
//
// HumdrumToken::setSubtrackCount -- Sets the subtrack count in the
//    HumdrumLine for all tokens in the same track as the current
//    token.
//

void HumdrumToken::setSubtrackCount(int count) {
	address.setSubtrackCount(count);
}



//////////////////////////////
//
// HumdrumToken::setPreviousToken --
//

void HumdrumToken::setPreviousToken(HumdrumToken* aToken) {
	previousTokens.resize(1);
	previousTokens[0] = aToken;
}



//////////////////////////////
//
// HumdrumToken::setNextToken --
//

void HumdrumToken::setNextToken(HumdrumToken* aToken) {
	nextTokens.resize(1);
	nextTokens[0] = aToken;
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

HumdrumToken* HumdrumToken::getNextToken(int index) const {
	if ((index >= 0) && (index < (int)nextTokens.size())) {
		return nextTokens[index];
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
	return nextTokens;
}



//////////////////////////////
//
// HumdrumToken::getPreviousTokens -- Returns a list of the previous
//    tokens in the spine before this token.
//

vector<HumdrumToken*> HumdrumToken::getPreviousTokens(void) const {
	return previousTokens;
}



//////////////////////////////
//
// HumdrumToken::getPreviousToken -- Returns the previous token in the
//    spine.  Since the previous token count is usually one, the default
//    index value is zero.
// default value: index = 0
//

HumdrumToken* HumdrumToken::getPreviousToken(int index) const {
	if ((index >= 0) && (index < (int)previousTokens.size())) {
		return previousTokens[index];
	} else {
		return NULL;
	}
}



//////////////////////////////
//
// HumdrumToken::analyzeDuration -- Currently reads the duration of
//   **kern and **recip data.  Add more data types here such as **koto.
//

bool HumdrumToken::analyzeDuration(string& err) {
	if ((*this) == NULL_DATA) {
		duration.setValue(-1);
		return true;
	}
	if (equalChar(0 ,'!')) {
		duration.setValue(-1);
		return true;
	}
	if (equalChar(0 ,'*')) {
		duration.setValue(-1);
		return true;
	}
	if (equalChar(0 ,'=')) {
		duration.setValue(-1);
		return true;
	}
	string dtype = getDataType();
	if (hasRhythm()) {
		if (isData()) {
			if (!isNull()) {
				duration = Convert::recipToDuration((string)(*this));
			} else {
				duration.setValue(-1);
			}
		} else {
			duration.setValue(-1);
		}
	} else {
		duration.setValue(-1);
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
	return duration;
}


HumNum HumdrumToken::getDuration(HumNum scale) const {
	return duration * scale;
}


//////////////////////////////
//
// HumdrumToken::setDuration -- Sets the duration of the token.  This is done in
//    HumdrumFileStructure::analyzeTokenDurations().
//

void HumdrumToken::setDuration(const HumNum& dur) {
	duration = dur;
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
// HumdrumToken::isRest -- Returns true if the token is a (kern) rest.
//

bool HumdrumToken::isRest(void) const {
	if (isDataType("**kern")) {
		if (Convert::isKernRest((string)(*this))) {
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

bool HumdrumToken::isNote(void) const {
	if (isDataType("**kern")) {
		if (Convert::isKernNote((string)(*this))) {
			return true;
		}
	}
	return false;
}



//////////////////////////////
//
// HumdrumToken::isInvisible -- True if a barline and is invisible (contains
//     a "-" styling), or a note/rest contains the string "yy" which is
//     interpreted as meaning make it invisible.
// 
// 

bool HumdrumToken::isInvisible(void) const {
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

bool HumdrumToken::isGrace(void) const {
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

bool HumdrumToken::isClef(void) const {
	if (!isDataType("**kern")) {
			return false;
	}
	if (!isInterpretation()) {
		return false;
	} else if (this->compare(0, 5, "*clef") != string::npos) {
		return true;
	}

	return false;
}



//////////////////////////////
//
// HumdrumToken::hasSlurStart -- Returns true if the **kern token has 
//     a '(' character.
//

bool HumdrumToken::hasSlurStart(void) const {
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

bool HumdrumToken::hasSlurEnd(void) const {
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

bool HumdrumToken::isSecondaryTiedNote(void) const {
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
	return address.getSubtrack();
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
	return address.getTrackString();
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
	for (int i=2; i<pieces.size(); i++) {
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
// HumdrumToken::makeForwardLink -- Line a following spine token to this one.
//    Used by the HumdrumFileBase::analyzeLinks function.
//

void HumdrumToken::makeForwardLink(HumdrumToken& nextToken) {
	nextTokens.push_back(&nextToken);
	nextToken.previousTokens.push_back(this);
}



//////////////////////////////
//
// HumdrumToken::makeBackwarddLink -- Link a previous spine token to this one.
//    Used by the HumdrumFileBase::analyzeLinks function.
//

void HumdrumToken::makeBackwardLink(HumdrumToken& previousToken) {
	previousTokens.push_back(&previousToken);
	previousToken.nextTokens.push_back(this);
}



//////////////////////////////
//
// HumdrumToken::setOwner -- Sets the HumdrumLine owner of this token.
//

void HumdrumToken::setOwner(HumdrumLine* aLine) {
	address.setOwner(aLine);
}



//////////////////////////////
//
// HumdrumToken::getOwner -- Returns a pointer to the HumdrumLine that
//    owns this token.
//

HumdrumLine* HumdrumToken::getOwner(void) const {
	return address.getOwner();
}



//////////////////////////////
//
// HumdrumToken::getState -- Returns the rhythm state variable.
//

int HumdrumToken::getState(void) const {
	return rhycheck;
}



//////////////////////////////
//
// HumdrumToken::getStrandIndex -- Returns the 1-D strand index
//    that the token belongs to in the owning HumdrumFile.
//    Returns -1 if there is no strand assignment.
//

int  HumdrumToken::getStrandIndex(void) const {
	return strand;
}



//////////////////////////////
//
// HumdrumToken::getSlurStartElisionLevel -- Returns the count of
//   elision marks ('&') preceding a slur start character '('.
//   Returns -1 if there is no slur start character.
//

int HumdrumToken::getSlurStartElisionLevel(void) const { 
	if (isDataType("**kern")) {
		return Convert::getKernSlurStartElisionLevel((string)(*this));
	} else {
		return -1;
	}
}



//////////////////////////////
//
// HumdrumToken::getSlurEndElisionLevel -- Returns the count of
//   elision marks ('&') preceding a slur end character ')'.
//   Returns -1 if there is no slur end character.
//

int HumdrumToken::getSlurEndElisionLevel(void) const { 
	if (isDataType("**kern")) {
		return Convert::getKernSlurEndElisionLevel((string)(*this));
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
	strand = index;
}



//////////////////////////////
//
// HumdrumToken::incrementState -- update the rhythm analysis state variable.
//    This will prevent redundant recursive analysis in analyzeRhythm of
//    the HumdrumFileStructure class.
//

void HumdrumToken::incrementState(void) {
	rhycheck++;
}



//////////////////////////////
//
// HumdrumToken::getNextTokenCount -- Returns the number of tokens in the
//   spine/sub spine which follow this token.  Typically this will be 1,
//   but will be zero for a terminator interpretation (*-), and will be
//   2 for a split interpretation (*^).
//

int HumdrumToken::getNextTokenCount(void) const {
	return (int)nextTokens.size();
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
	return (int)previousTokens.size();
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
		for (int i=0; i<value.size(); i++) {
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
	out << token->c_str();
	return out;
}



//////////////////////////////
//
// printSequence --
//    default value: out = cout;
//

ostream& printSequence(vector<vector<HTp> >& sequence, ostream& out) {
	for (int i=0; i<sequence.size(); i++) {
		for (int j=0; j<sequence[i].size(); j++) {
			out << sequence[i][j];
			if (j < sequence[i].size() - 1) {
				out << '\t';
			}
		}
		out << endl;
	}
	return out;
}


ostream& printSequence(vector<HTp>& sequence, ostream& out) {
	for (int i=0; i<sequence.size(); i++) {
		out << sequence[i] << endl;
	}
	return out;
}





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
	for (int i=0; i < kerndata.size(); i++) {
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
	for (int i=0; i < kerndata.size(); i++) {
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
	for (int i=0; i < kerndata.size(); i++) {
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
	for (int i=0; i < kerndata.size(); i++) {
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
	for (int i=0; i < kerndata.size(); i++) {
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
//   '&' characters before the last '(' character in a kern token.
//   Returns -1 if no '(' character in string.
//

int Convert::getKernSlurStartElisionLevel(const string& kerndata) { 
	bool foundSlurStart = false;
	int output = 0;
	for (int i=(int)kerndata.size()-1; i >=0; i--) {
		char ch = kerndata[i];
		if (ch == '(') {
			foundSlurStart = true;
			continue;
		}
		if (!foundSlurStart) {
			continue;
		}
		if (ch == '&') {
			output++;
		} else {
			return output;
		}
	}
	if (foundSlurStart) {
		return output;
	} else {
		return -1;
	}
}



//////////////////////////////
//
// Convert::getKernSlurEndElisionLevel -- Returns the number of 
//   '&' characters before the last ')' character in a kern token.
//   Returns -1 if no ')' character in string.
//

int Convert::getKernSlurEndElisionLevel(const string& kerndata) { 
	bool foundSlurEnd = false;
	int output = 0;
	for (int i=(int)kerndata.size()-1; i >=0; i--) {
		char ch = kerndata[i];
		if (ch == ')') {
			foundSlurEnd = true;
			continue;
		}
		if (!foundSlurEnd) {
			continue;
		}
		if (ch == '&') {
			output++;
		} else {
			return output;
		}
	}
	if (foundSlurEnd) {
		return output;
	} else {
		return -1;
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
	for (int i=1; i<numbers.size(); i++) {
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

	for (int i=0; i<subtokens.size(); i++) {
		diatonic   = Convert::kernToDiatonicUC(subtokens[i]);
		accidental = Convert::kernToAccidentalCount(subtokens[i]);
		octave     = Convert::kernToOctaveNumber(subtokens[i]);
		if ((i > 0) && (i < subtokens.size()-1)) {
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
	for (int i=0; i<kerndata.size(); i++) {
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
	for (int i=0; i<kerndata.size(); i++) {
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
	for (int i=0; i<kerndata.size(); i++) {
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
	for (int i=0; i<kerndata.size(); i++) {
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
//   accidental adjustment to an integer.  The diatonic pitch class
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
		// allow for pitch-classes as ASCII characters:
		dpc = std::tolower(dpc) - 'a' + 5;
		dpc = dpc % 7;
	}
	int output = -1000;
	switch (dpc) {
		case 0: output = 0;
		case 1: output =  2 * maxacc + 2;
		case 2: output =  4 * maxacc + 4;
		case 3: output =  6 * maxacc + 4;
		case 4: output =  8 * maxacc + 6;
		case 5: output = 10 * maxacc + 8;
		case 6: output = 12 * maxacc + 8;
	}
	if (output < 0) {
		return output;
	}
	return (output + acc) * octave + maxacc;
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

HumNum Convert::recipToDuration(const string& recip, HumNum scale,
		string separator) {
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
	for (i=0; i<subtok.size(); i++) {
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
		while ((numi < subtok.size()) && isdigit(subtok[numi])) {
			denominator = denominator * 10 + (subtok[numi++] - '0');
		}
		if ((loc + 1 < subtok.size()) && isdigit(subtok[loc+1])) {
			int xi = (int)loc + 1;
			numerator = subtok[xi++] - '0';
			while ((xi < subtok.size()) && isdigit(subtok[xi])) {
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
		for (i=numi+1; i<subtok.size(); i++) {
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
		while ((numi < subtok.size()) && isdigit(subtok[numi])) {
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

HumNum Convert::recipToDurationNoDots(const string& recip, HumNum scale,
		string separator) {
	string temp = recip;
	std::replace(temp.begin(), temp.end(), '.', 'Z');
	return Convert::recipToDuration(temp, scale, separator);
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
	for (int i=0; i<input.size(); i++) {
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




} // end namespace hum
