//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Aug  8 12:24:49 PDT 2015
// Last Modified: Thu Mar  8 12:21:07 PST 2018
// Filename:      humlib.h
// URL:           https://github.com/craigsapp/humlib/blob/master/include/humlib.h
// Syntax:        C++11
// vim:           ts=3
//
// Description:   Include file for humlib library.
//
/*
Copyright (c) 2015, 2016, 2017, 2018 Craig Stuart Sapp
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

#ifndef _HUMLIB_H_INCLUDED
#define _HUMLIB_H_INCLUDED

#include <stdarg.h>
#include <string.h>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::ends;
using std::ifstream;
using std::invalid_argument;
using std::istream;
using std::istreambuf_iterator;
using std::list;
using std::map;
using std::ostream;
using std::pair;
using std::regex;
using std::set;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

#ifdef USING_URI
	#include <sys/types.h>   /* socket, connect */
	#include <sys/socket.h>  /* socket, connect */
	#include <netinet/in.h>  /* htons           */
	#include <netdb.h>       /* gethostbyname   */
	#include <unistd.h>      /* read, write     */
	#include <string.h>      /* memcpy          */
   #include <sstream>
#endif

#include "pugiconfig.hpp"
#include "pugixml.hpp"

using pugi::xml_node;
using pugi::xml_attribute;
using pugi::xml_document;
using pugi::xpath_node;

namespace hum {

class Convert;
class HumNum;
class HumAddress;
class HumdrumToken;
typedef HumdrumToken* HTp;
class HumdrumLine;
class HumdrumFileBase;
class HumdrumFileStructure;
class HumdrumFileContent;
class HumdrumFile;
class GridVoice;


class HumParameter : public string {
	public:
		HumParameter(void);
		HumParameter(const string& str);
		HumdrumToken* origin;
};

typedef map<string, map<string, map<string, HumParameter> > > MapNNKV;
typedef map<string, map<string, HumParameter> > MapNKV;
typedef map<string, HumParameter> MapKV;

class HumHash {
	public:
		               HumHash             (void);
		              ~HumHash             ();

		string         getValue            (const string& key) const;
		string         getValue            (const string& ns2,
		                                    const string& key) const;
		string         getValue            (const string& ns1, const string& ns2,
		                                    const string& key) const;
		HTp            getValueHTp         (const string& key) const;
		HTp            getValueHTp         (const string& ns2,
		                                    const string& key) const;
		HTp            getValueHTp         (const string& ns1, const string& ns2,
		                                    const string& key) const;
		int            getValueInt         (const string& key) const;
		int            getValueInt         (const string& ns2,
		                                    const string& key) const;
		int            getValueInt         (const string& ns1, const string& ns2,
		                                    const string& key) const;
		HumNum         getValueFraction    (const string& key) const;
		HumNum         getValueFraction    (const string& ns2,
		                                    const string& key) const;
		HumNum         getValueFraction    (const string& ns1, const string& ns2,
		                                    const string& key)const ;
		double         getValueFloat       (const string& key)const ;
		double         getValueFloat       (const string& ns2,
		                                    const string& key) const;
		double         getValueFloat       (const string& ns1, const string& ns2,
		                                    const string& key) const;
		bool           getValueBool        (const string& key) const;
		bool           getValueBool        (const string& ns2,
		                                    const string& key) const;
		bool           getValueBool        (const string& ns1, const string& ns2,
		                                    const string& key) const;

		void           setValue            (const string& key,
		                                    const string& value);
		void           setValue            (const string& ns2,
		                                    const string& key,
		                                    const string& value);
		void           setValue            (const string& ns1,
		                                    const string& ns2,
		                                    const string& key,
		                                    const string& value);
		void           setValue            (const string& key,
		                                    const char* value);
		void           setValue            (const string& ns2,
		                                    const string& key,
		                                    const char* value);
		void           setValue            (const string& ns1,
		                                    const string& ns2,
		                                    const string& key,
		                                    const char* value);
		void           setValue            (const string& key, int value);
		void           setValue            (const string& ns2, const string& key,
		                                    int value);
		void           setValue            (const string& ns1, const string& ns2,
		                                    const string& key, int value);
		void           setValue            (const string& key, HTp value);
		void           setValue            (const string& ns2, const string& key,
		                                    HTp value);
		void           setValue            (const string& ns1, const string& ns2,
		                                    const string& key, HTp value);
		void           setValue            (const string& key, HumNum value);
		void           setValue            (const string& ns2, const string& key,
		                                    HumNum value);
		void           setValue            (const string& ns1, const string& ns2,
		                                    const string& key, HumNum value);
		void           setValue            (const string& key, double value);
		void           setValue            (const string& ns2, const string& key,
		                                    double value);
		void           setValue            (const string& ns1, const string& ns2,
		                                    const string& key, double value);
		bool           isDefined           (const string& key) const;
		bool           isDefined           (const string& ns2,
		                                    const string& key) const;
		bool           isDefined           (const string& ns1, const string& ns2,
		                                    const string& key) const;
		void           deleteValue         (const string& key);
		void           deleteValue         (const string& ns2, const string& key);
		void           deleteValue         (const string& ns1, const string& ns2,
		                                    const string& key);
		vector<string> getKeys             (void) const;
		vector<string> getKeys             (const string& ns) const;
		vector<string> getKeys             (const string& ns1,
		                                    const string& ns2) const;
		bool           hasParameters       (void) const;
		bool           hasParameters       (const string& ns) const;
		bool           hasParameters       (const string& ns1,
		                                    const string& ns2) const;
		int            getParameterCount   (void) const;
		int            getParameterCount   (const string& ns) const;
		int            getParameterCount   (const string& ns1,
		                                    const string& ns2) const;
		void           setPrefix           (const string& value);
		string         getPrefix           (void) const;
		ostream&       printXml            (ostream& out = cout, int level = 0,
		                                    const string& indent = "\t");
		ostream&       printXmlAsGlobal    (ostream& out = cout, int level = 0,
		                                    const string& indent = "\t");

		void           setOrigin           (const string& key,
		                                    HumdrumToken* tok);
		void           setOrigin           (const string& key,
		                                    HumdrumToken& tok);
		void           setOrigin           (const string& ns2, const string& key,
		                                    HumdrumToken* tok);
		void           setOrigin           (const string& ns2, const string& key,
		                                    HumdrumToken& tok);
		void           setOrigin           (const string& ns1, const string& ns2,
		                                    const string& parameter,
		                                    HumdrumToken* tok);
		void           setOrigin           (const string& ns1, const string& ns2,
		                                    const string& parameter,
		                                    HumdrumToken& tok);

		HumdrumToken*  getOrigin           (const string& key) const;
		HumdrumToken*  getOrigin           (const string& ns2,
		                                    const string& key) const;
		HumdrumToken*  getOrigin           (const string& ns1,
		                                    const string& ns2,
		                                    const string& parameter) const;

	protected:
		void           initializeParameters(void);
		vector<string> getKeyList          (const string& keys) const;

	private:
		MapNNKV* parameters;
		string   prefix;

	friend ostream& operator<<(ostream& out, const HumHash& hash);
};



class HumNum {
	public:
		         HumNum             (void);
		         HumNum             (int value);
		         HumNum             (int numerator, int denominator);
		         HumNum             (const HumNum& rat);
		         HumNum             (const string& ratstring);
		         HumNum             (const char* ratstring);
		        ~HumNum             ();

		bool     isNegative         (void) const;
		bool     isPositive         (void) const;
		bool     isZero             (void) const;
		bool     isNonZero          (void) const;
		bool     isNonNegative      (void) const;
		bool     isNonPositive      (void) const;
		bool     isInfinite         (void) const;
		bool     isFinite           (void) const;
		bool     isNaN              (void) const;
		bool     isInteger          (void) const;
		bool     isPowerOfTwo       (void) const;
		double   getFloat           (void) const;
		double   toFloat  (void) const { return getFloat(); }
		int      getInteger         (double round = 0.0) const;
		int      toInteger (double round = 0.0) const {
		                                            return getInteger(round); }
		int      getNumerator       (void) const;
		int      getDenominator     (void) const;
		HumNum   getRemainder       (void) const;
		void     setValue           (int numerator);
		void     setValue           (int numerator, int denominator);
		void     setValue           (const string& ratstring);
		void     setValue           (const char* ratstring);
		HumNum   getAbs             (void) const;
		HumNum&  makeAbs            (void);
		HumNum&  operator=          (const HumNum& value);
		HumNum&  operator=          (int value);
		HumNum&  operator+=         (const HumNum& value);
		HumNum&  operator+=         (int value);
		HumNum&  operator-=         (const HumNum& value);
		HumNum&  operator-=         (int value);
		HumNum&  operator*=         (const HumNum& value);
		HumNum&  operator*=         (int value);
		HumNum&  operator/=         (const HumNum& value);
		HumNum&  operator/=         (int value);
		HumNum   operator-          (void) const;
		HumNum   operator+          (const HumNum& value) const;
		HumNum   operator+          (int value) const;
		HumNum   operator-          (const HumNum& value) const;
		HumNum   operator-          (int value) const;
		HumNum   operator*          (const HumNum& value) const;
		HumNum   operator*          (int value) const;
		HumNum   operator/          (const HumNum& value) const;
		HumNum   operator/          (int value) const;
		bool     operator==         (const HumNum& value) const;
		bool     operator==         (double value) const;
		bool     operator==         (int value) const;
		bool     operator!=         (const HumNum& value) const;
		bool     operator!=         (double value) const;
		bool     operator!=         (int value) const;
		bool     operator<          (const HumNum& value) const;
		bool     operator<          (double value) const;
		bool     operator<          (int value) const;
		bool     operator<=         (const HumNum& value) const;
		bool     operator<=         (double value) const;
		bool     operator<=         (int value) const;
		bool     operator>          (const HumNum& value) const;
		bool     operator>          (double value) const;
		bool     operator>          (int value) const;
		bool     operator>=         (const HumNum& value) const;
		bool     operator>=         (double value) const;
		bool     operator>=         (int value) const;
		ostream& printFraction      (ostream& = cout) const;
		ostream& printMixedFraction (ostream& out = cout,
		                             string separator = "_") const;
		ostream& printList          (ostream& out) const;

	protected:
		void     reduce             (void);
		int      gcdIterative       (int a, int b);
		int      gcdRecursive       (int a, int b);

	private:
		int top;
		int bot;
};


ostream& operator<<(ostream& out, const HumNum& number);

template <typename A>
ostream& operator<<(ostream& out, const vector<A>& v);



class HumRegex {
	public:
		            HumRegex           (void);
		            HumRegex           (const string& exp,
		                                const string& options = "");
		           ~HumRegex           ();

		// setting persistent options for regular expression contruction
		void        setIgnoreCase      (void);
		bool        getIgnoreCase      (void);
		void        unsetIgnoreCase    (void);

		// setting persistent search/match options
		void        setGlobal          (void);
		bool        getGlobal          (void);
		void        unsetGlobal        (void);

		// replacing
		string&     replaceDestructive (string& input, const string& replacement,
		                                const string& exp);
		string&     replaceDestructive (string& input, const string& replacement,
		                                const string& exp,
		                                const string& options);
		string      replaceCopy        (const string& input,
		                                const string& replacement,
		                                const string& exp);
		string      replaceCopy        (const string& input,
		                                const string& replacement,
		                                const string& exp,
		                                const string& options);

		string&     replaceDestructive (string* input, const string& replacement,
		                                const string& exp);
		string&     replaceDestructive (string* input, const string& replacement,
		                                const string& exp,
		                                const string& options);
		string      replaceCopy        (string* input, const string& replacement,
		                                const string& exp);
		string      replaceCopy        (string* input, const string& replacement,
		                                const string& exp,
		                                const string& options);
		string&      tr                 (string& input, const string& from, 
		                                const string& to);

		// matching (full-string match)
		bool        match              (const string& input, const string& exp);
		bool        match              (const string& input, const string& exp,
		                                const string& options);
		bool        match              (const string* input, const string& exp);
		bool        match              (const string* input, const string& exp,
		                                const string& options);


		// searching
		// http://www.cplusplus.com/reference/regex/regex_search
		bool        search             (const string& input, const string& exp);
		bool        search             (const string& input, const string& exp,
		                                const string& options);
		bool        search             (const string& input, int startindex,
		                                const string& exp);
		bool        search             (const string& input, int startindex,
		                                const string& exp,
		                                const string& options);

		bool        search             (string* input, const string& exp);
		bool        search             (string* input, const string& exp,
		                                const string& options);
		bool        search             (string* input, int startindex,
		                                const string& exp);
		bool        search             (string* input, int startindex,
		                                const string& exp,
		                                const string& options);

		int         getMatchCount      (void);
		string      getMatch           (int index);
		int         getMatchInt        (int index);
		double      getMatchDouble     (int index);
		string      getPrefix          (void);
		string      getSuffix          (void);
		int         getMatchStartIndex (int index = 0);
		int         getMatchEndIndex   (int index = 0);
		int         getMatchLength     (int index = 0);

		// token lists:
		bool        split              (vector<string>& entries,
		                                const string& buffer,
		                                const string& separator);

	protected:
		std::regex_constants::syntax_option_type
				getTemporaryRegexFlags(const string& sflags);
		std::regex_constants::match_flag_type
				getTemporarySearchFlags(const string& sflags);


	private:

		// m_regex: stores the regular expression to use as a default.
		//
		// http://en.cppreference.com/w/cpp/regex/basic_regex
		// .assign(string) == set the regular expression.
		// operator=       == set the regular expression.
		// .flags()        == return syntax_option_type used to construct.
		std::regex m_regex;

		// m_matches: stores the matches from a search:
		//
		// http://en.cppreference.com/w/cpp/regex/match_results
		// .empty()     == check if match was successful.
		// .size()      == number of matches.
		// .length(i)   == return length of a submatch.
		// .position(i) == return start index of submatch in search string.
		// .str(i)      == return string of submatch.
		// operator[i]  == return submatch.
		// .prefix
		// .suffix
		// .begin()     == start of submatch list.
		// .end()       == end of submatch list.
		std::smatch m_matches;

		// m_regexflags: store default settings for regex processing
		// http://en.cppreference.com/w/cpp/regex/syntax_option_type
		// http://en.cppreference.com/w/cpp/regex/basic_regex
		// /usr/local/Cellar/gcc49/4.9.3/include/c++/4.9.3/bits/regex_constants.h 
		//
		// Options (in the namespace std::regex_constants):
		//    icase      == Ignore case.
		//    nosubs     == Don't collect submatches.
		//    optimize   == Make matching faster, but construction slower.
		//    collate    == locale character ranges.
		//    multiline  == C++17 only.
		//
		// Only one of the following can be given.  EMCAScript will be
		// used by default if none specified.
		//    EMCAScript == Use EMCAScript regex syntax.
		//    basic      == Use basic POSIX syntax.
		//    extended   == Use extended POSIX syntax.
		//    awk        == Use awk POSIX syntax.
		//    grep       == Use grep POSIX syntax.
		//    egrep      == Use egrep POSIX syntax.
		std::regex_constants::syntax_option_type m_regexflags;

		// m_flags: store default settings for regex processing
		// http://www.cplusplus.com/reference/regex/regex_search
		//    match_default     == clear all options.
		//    match_not_bol     == not beginning of line.
		//    match_not_eol     == not end of line.
		//    match_not_bow     == not beginning of word for \b.
		//    match_not_eow     == not end of word for \b.
		//    match_any         == any match acceptable if more than 1 possible..
		//    match_not_null    == empty sequence does note match.
		//    match_continuous  ==
		//    match_prev_avail  ==
		//    format_default    == same as match_default.
		std::regex_constants::match_flag_type m_searchflags;

};



class HumdrumLine;
class HumdrumToken;

class HumAddress {
	public:
		                    HumAddress        (void);
		                    HumAddress        (HumAddress& address);
		                   ~HumAddress        ();

		HumAddress&         operator=         (const HumAddress& address);
		int                 getLineIndex      (void) const;
		int                 getLineNumber     (void) const;
		int                 getFieldIndex     (void) const;
		const HumdrumToken& getDataType       (void) const;
		const string&       getSpineInfo      (void) const;
		int                 getTrack          (void) const;
		int                 getSubtrack       (void) const;
		int                 getSubtrackCount  (void) const;
		string              getTrackString    (string separator = ".") const;
		HumdrumLine*        getLine           (void) const;
		HumdrumLine*        getOwner          (void) const { return getLine(); }
		bool                hasOwner          (void) const;

	protected:
		void                setOwner          (HumdrumLine* aLine);
		void                setFieldIndex     (int fieldlindex);
		void                setSpineInfo      (const string& spineinfo);
		void                setTrack          (int aTrack, int aSubtrack);
		void                setTrack          (int aTrack);
		void                setSubtrack       (int aSubtrack);
		void                setSubtrackCount  (int aSubtrack);

	private:

		// fieldindex: This is the index of the token in the HumdrumLine
		// which owns this token.
		int m_fieldindex;

		// spining: This is the spine position of the token. A simple spine
		// position is an integer, starting with "1" for the first spine
		// of the file (left-most spine).  When the spine splits, "(#)a"
		// is wrapped around the left-subspine's spine info, and "(#)b"
		// around the right-subspine's info. Merged spines will add a space
		// between the two or more merged spines information, such as
		// "(#)a (#)b" for two sub-spines merged into a single spine again.
		// But in this case there is a spine info simplification which will
		// convert "(#)a (#)b" into "#" where # is the original spine number.
		// Other more complicated mergers may be simplified in the future.
		string m_spining;

		// track: This is the track number of the spine.  It is the first
		// number found in the spineinfo string.
		int m_track;

		// subtrack: This is the subtrack number for the spine.  When a spine
		// is not split, it will be 0, if the spine has been split with *^,
		// then the left-subspine will be in subtrack 1 and the right-spine
		// will be subtrack 2.  If subspines are exchanged with *x, then their
		// subtrack assignments will also change.
		int m_subtrack;

		// subtrackcount: The number of currently active subtracks tokens
		// on the owning HumdrumLine (in the same track).  The subtrack range
		// is from 1 (if there is only a primary spine), to a larger number.
		// if subtrackcount is 0, then the variable is not set, or there are
		// no tokens in the track (such as for global comments).
		int m_subtrackcount;

		// owner: This is the line which manages the given token.
		HumdrumLine* m_owner;

	friend class HumdrumToken;
	friend class HumdrumLine;
	friend class HumdrumFile;
};



class HumParamSet {

	public:
		              HumParamSet        (void);
		              HumParamSet        (const string& token);
		              HumParamSet        (HTp token);
		             ~HumParamSet        ();

		const string& getNamespace1      (void);
		const string& getNamespace2      (void);
		string        getNamespace       (void);
		void          setNamespace1      (const string& name);
		void          setNamespace2      (const string& name);
		void          setNamespace       (const string& name);
		void          setNamespace       (const string& name1, const string& name2);

		void          clear              (void);
		int           getCount           (void);
		const string& getParameterName   (int index);
		const string& getParameterValue  (int index);
		int           addParameter       (const string& name, const string& value);
		int           setParameter       (const string& name, const string& value);
		void          readString         (const string& text);
		ostream&      printXml           (ostream& out = cout, int level = 0,
		                                  const string& indent = "\t");

	private:
		string m_ns1;
		string m_ns2;
		vector<pair<string, string>> m_parameters;

};


ostream& operator<<(ostream& out, HumParamSet* hps);
ostream& operator<<(ostream& out, HumParamSet& hps);



class _HumInstrument {
	public:
		_HumInstrument    (void) { humdrum = ""; name = ""; gm = 0; }
	  ~_HumInstrument    ()     { humdrum = ""; name = ""; gm = 0; }

		string humdrum;
		string name;
		int    gm;
};


class HumInstrument {
	public:
		           HumInstrument       (void);
		           HumInstrument       (const string& Hname);
		          ~HumInstrument       ();

		string     getName             (void);
		string     getName             (const string& Hname);
		string     getHumdrum          (void);
		int        getGM               (void);
		int        getGM               (const string& Hname);
		void       setHumdrum          (const string& Hname);
		int        setGM               (const string& Hname, int aValue);

	private:
		int                            index;
		static vector<_HumInstrument>  data;
		static int                     classcount;

	protected:
		void       initialize          (void);
		void       afi                 (const char* humdrum_name, int midinum,
		                                const char* EN_name);
		int        find                (const string& Hname);
		void       sortData            (void);
		static int data_compare_by_humdrum_name(const void* a, const void* b);
};


///////////////////////////////////////////////////////////////////////////
//
// General MIDI instrument definitions
//

#define  CH_1                             0
#define  CH_2                             1
#define  CH_3                             2
#define  CH_4                             3
#define  CH_5                             4
#define  CH_6                             5
#define  CH_7                             6
#define  CH_8                             7
#define  CH_9                             8
#define  CH_10                            9
#define  CH_11                            10
#define  CH_12                            11
#define  CH_13                            12
#define  CH_14                            13
#define  CH_15                            14
#define  CH_16                            15

#define  GM_PIANO(X)                      (0+(X))
#define  GM_ACOUSTIC_GRAND_PIANO          (0)
#define  GM_BRIGHT_ACOUSTIC_PIANO         (1)
#define  GM_ELECTRIC_GRAND_PIANO          (1)
#define  GM_HONKYTONK_PIANO               (2)
#define  GM_HONKY_TONK_PIANO              (3)
#define  GM_ELECTRIC_PIANO_1              (4)
#define  GM_ELECTRIC_PIANO_2              (5)
#define  GM_HARPSICHORD                   (6)
#define  GM_CLAVI                         (7)

#define  GM_CHROMATIC(X)                  (8+(X))
#define  GM_CELESTA                       (8)
#define  GM_GLOCKENSPIEL                  (9)
#define  GM_MUSIC_BOX                     (10)
#define  GM_VIBRAPHONE                    (11)
#define  GM_MARIMBA                       (12)
#define  GM_XYLOPHONE                     (13)
#define  GM_TUBULAR_BELLS                 (14)
#define  GM_DULCIMER                      (15)

#define  GM_ORGAN(X)                      (16+(X))
#define  GM_DRAWBAR_ORGAN                 (16)
#define  GM_PERCUSSIVE_ORGAN              (17)
#define  GM_ROCK_ORGAN                    (18)
#define  GM_CHURCH_ORGAN                  (19)
#define  GM_REED_ORGAN                    (20)
#define  GM_ACCORDION                     (21)
#define  GM_HARMONICA                     (22)
#define  GM_TANGO_ACCORDION               (23)

#define  GM_GUITAR(X)                     (24+(X))
#define  GM_ACOUSTIC_GUITAR_NYLON         (24)
#define  GM_ACOUSTIC_GUITAR_STEEL         (25)
#define  GM_ELECTRIC_GUITAR_JAZZ          (26)
#define  GM_ELECTRIC_GUITAR_CLEAN         (27)
#define  GM_ELECTRIC_GUITAR_MUTED         (28)
#define  GM_OVERDRIVEN_GUITAR             (29)
#define  GM_DISTORTION_GUITAR             (30)
#define  GM_GUITAR_HARMONICS              (31)

#define  GM_BASS(X)                       (32+(X))
#define  GM_ACOUSTIC_BASS                 (32)
#define  GM_ELECTRIC_BASS_FINGER          (33)
#define  GM_ELECTRIC_BASS_PICK            (34)
#define  GM_FRETLESS_BASS                 (35)
#define  GM_SLAP_BASS_1                   (36)
#define  GM_SLAP_BASS_2                   (37)
#define  GM_SYNTH_BASS_1                  (38)
#define  GM_SYNTH_BASS_2                  (39)

#define  GM_STRINGS(X)                    (40+(X))
#define  GM_VIOLIN                        (40)
#define  GM_VIOLA                         (41)
#define  GM_CELLO                         (42)
#define  GM_CONTRABASS                    (43)
#define  GM_TREMOLO_STRINGS               (44)
#define  GM_PIZZACATO_STRINGS             (45)
#define  GM_ORCHESTRAL_HARP               (46)
#define  GM_TIMPANI                       (47)

#define  GM_ENSEMBLE(X)                   (48+(X))
#define  GM_STRING_ENSEMBLE_1             (48)
#define  GM_STRING_ENSEMBLE_2             (49)
#define  GM_SYNTHSTRINGS_1                (50)
#define  GM_SYNTHSTRINGS_2                (51)
#define  GM_CHOIR_AAHS                    (52)
#define  GM_VOICE_OOHS                    (53)
#define  GM_SYNTH_VOICE                   (54)
#define  GM_ORCHESTRA_HIT                 (55)

#define  GM_BRASS(X)                      (56+(X))
#define  GM_TRUMPET                       (56)
#define  GM_TROMBONE                      (57)
#define  GM_TUBA                          (58)
#define  GM_MUTED_TRUMPED                 (59)
#define  GM_FRENCH_HORN                   (60)
#define  GM_BRASS_SECTION                 (61)
#define  GM_SYNTHBRASS_1                  (62)
#define  GM_SYNTHBRASS_2                  (63)

#define  GM_REED(X)                       (64+(X))
#define  GM_SOPRANO_SAX                   (64)
#define  GM_ALTO_SAX                      (65)
#define  GM_TENOR_SAX                     (66)
#define  GM_BARITONE_SAX                  (67)
#define  GM_OBOE                          (68)
#define  GM_ENGLISH_HORN                  (69)
#define  GM_BASSOON                       (70)
#define  GM_CLARINET                      (71)

#define  GM_PIPE(X)                       (72+(X))
#define  GM_PICCOLO                       (72)
#define  GM_FLUTE                         (73)
#define  GM_RECORDER                      (74)
#define  GM_PAN_FLUTE                     (75)
#define  GM_BLOWN_BOTTLE                  (76)
#define  GM_SHAKUHACHI                    (77)
#define  GM_WHISTLE                       (78)
#define  GM_OCARINA                       (79)

#define  GM_LEAD(X)                       (80+(X))
#define  GM_LEAD_SQUARE                   (80)
#define  GM_LEAD_SAWTOOTH                 (81)
#define  GM_LEAD_CALLIOPE                 (82)
#define  GM_LEAD_CHIFF                    (83)
#define  GM_LEAD_CHARANG                  (84)
#define  GM_LEAD_VOICE                    (85)
#define  GM_LEAD_FIFTHS                   (86)
#define  GM_LEAD_BASS                     (87)

#define  GM_PAD(X)                        (88+(X))
#define  GM_PAD_NEW_AGE                   (88)
#define  GM_PAD_WARM                      (89)
#define  GM_PAD_POLYSYNTH                 (90)
#define  GM_PAD_CHOIR                     (91)
#define  GM_PAD_BOWED                     (92)
#define  GM_PAD_METALLIC                  (93)
#define  GM_PAD_HALO                      (94)
#define  GM_PAD_SWEEP                     (95)

#define  GM_FX(X)                         (96+(X))
#define  GM_FX_TRAIN                      (96)
#define  GM_FX_SOUNDTRACK                 (97)
#define  GM_FX_CRYSTAL                    (98)
#define  GM_FX_ATMOSPHERE                 (99)
#define  GM_FX_BRIGHTNESS                 (100)
#define  GM_FX_GOBLINS                    (101)
#define  GM_FX_ECHOES                     (102)
#define  GM_FX_SCI_FI                     (103)

#define  GM_ETHNIC(X)                     (104+(X))
#define  GM_SITAR                         (104)
#define  GM_BANJO                         (105)
#define  GM_SHAMISEN                      (106)
#define  GM_KOTO                          (107)
#define  GM_KALIMBA                       (108)
#define  GM_BAGPIPE                       (109)
#define  GM_FIDDLE                        (110)
#define  GM_SHANAI                        (111)

#define  GM_PERCUSSION(X)                 (112+(X))
#define  GM_TINKLE_BELL                   (112)
#define  GM_AGOGO                         (113)
#define  GM_STEEL_DRUMS                   (114)
#define  GM_WOODBLOCKS                    (115)
#define  GM_TAIKO_DRUM                    (116)
#define  GM_MELODIC_DRUM                  (117)
#define  GM_SYNTH_DRUM                    (118)
#define  GM_REVERSE_CYMBAL                (119)

#define  GM_SOUNDEFFECT(X)                (120+(X))
#define  GM_GUITAR_FRET_NOISE             (120)
#define  GM_BREATH_NOISE                  (121)
#define  GM_SEASHORE                      (122)
#define  GM_BIRD_TWEET                    (123)
#define  GM_TELEPHONE_RING                (124)
#define  GM_HELICOPTER                    (125)
#define  GM_APPLAUSE                      (126)
#define  GM_GUNSHOT                       (127)

//
// Percussion instruments on channel 10
//

#define  GM_ACOUSTIC_BASS_DRUM            (35)
#define  GM_BASS_DRUM_1                   (36)
#define  GM_SIDE_STICK                    (37)
#define  GM_ACOUSTIC_SNARE                (38)
#define  GM_HAND_CLAP                     (39)
#define  GM_ELECTRIC_SNARE                (40)
#define  GM_LOW_FLOOR_TOM                 (41)
#define  GM_CLOSED_HI_HAT                 (42)
#define  GM_HIGH_FLOOR_TOM                (43)
#define  GM_PEDAL_HI_HAT                  (44)
#define  GM_LOW_TOM                       (45)
#define  GM_OPEN_HI_HAT                   (46)
#define  GM_LOW_MID_TOM                   (47)
#define  GM_HIGH_MID_TOM                  (48)
#define  GM_CRASH_CYMBAL_1                (49)
#define  GM_HIGH_TOM                      (50)
#define  GM_RIDE_CYMBAL_1                 (51)
#define  GM_CHINESE_CYMBAL                (52)
#define  GM_RIDE_BELL                     (53)
#define  GM_TAMBOURINE                    (54)
#define  GM_SPLASH_CYMBAL                 (55)
#define  GM_COWBELL                       (56)
#define  GM_CRASH_CYMBAL_2                (57)
#define  GM_VIBRASLAP                     (58)
#define  GM_RIDE_CYMBAL_2                 (59)
#define  GM_HI_BONGO                      (60)
#define  GM_LOW_BONGO                     (61)
#define  GM_MUTE_HI_CONGA                 (62)
#define  GM_OPEN_HI_CONGA                 (63)
#define  GM_LOW_CONGA                     (64)
#define  GM_HIGH_TIMBALE                  (65)
#define  GM_LOW_TIMBALE                   (66)
#define  GM_HIGH_AGOGO                    (67)
#define  GM_LOW_AGOGO                     (68)
#define  GM_CABASA                        (69)
#define  GM_MARACAS                       (70)
#define  GM_SHORT_WHISTLE                 (71)
#define  GM_LONG_WHISTLE                  (72)
#define  GM_SHORT_GUIRO                   (73)
#define  GM_LONG_GUIRO                    (74)
#define  GM_CLAVES                        (75)
#define  GM_HI_WOOD_BLOCK                 (76)
#define  GM_LOW_WOOD_BLOCK                (77)
#define  GM_MUTE_CUICA                    (78)
#define  GM_OPEN_CUICA                    (79)
#define  GM_MUTE_TRIANGLE                 (80)
#define  GM_OPEN_TRIANGLE                 (81)



class HumdrumLine : public string, public HumHash {
	public:
		         HumdrumLine            (void);
		         HumdrumLine            (const string& aString);
		         HumdrumLine            (const char* aString);
		         HumdrumLine            (HumdrumLine& line);
		         HumdrumLine            (HumdrumLine& line, void* owner);
		        ~HumdrumLine            ();

		HumdrumLine& operator=          (HumdrumLine& line);
		bool     isComment              (void) const;
		bool     isCommentLocal         (void) const;
		bool     isLocalComment         (void) const { return isCommentLocal(); }
		bool     isCommentGlobal        (void) const;
		bool     isReference            (void) const;
		string   getReferenceKey        (void) const;
		string   getReferenceValue      (void) const;
		bool     isGlobalComment         (void) const { return isCommentGlobal(); }
		bool     isExclusive            (void) const;
		bool     isExclusiveInterpretation (void) const { return isExclusive(); }
		bool     isTerminator           (void) const;
		bool     isInterp               (void) const;
		bool     isInterpretation       (void) const { return isInterp(); }
		bool     isBarline              (void) const;
		bool     isData                 (void) const;
		bool     isAllNull              (void) const;
		bool     isAllRhythmicNull      (void) const;
		bool     isEmpty                (void) const;
		bool     isBlank                (void) const { return isEmpty(); }
		bool     isManipulator          (void) const;
		bool     hasSpines              (void) const;
		bool     isGlobal               (void) const;
		HTp      token                  (int index) const;
		void     getTokens              (vector<HTp>& list);
		int      getTokenCount          (void) const;
		int      getFieldCount          (void) const { return getTokenCount(); }
		string   getTokenString         (int index) const;
		bool     equalChar              (int index, char ch) const;
		char     getChar                (int index) const;
		bool     isKernBoundaryStart    (void) const;
		bool     isKernBoundaryEnd      (void) const;
		ostream& printSpineInfo         (ostream& out = cout);
		ostream& printTrackInfo         (ostream& out = cout);
		ostream& printDataTypeInfo      (ostream& out = cout);
		ostream& printDurationInfo      (ostream& out = cout);
		ostream& printCsv               (ostream& out = cout,
		                                 const string& separator = ",");
		ostream& printXml               (ostream& out = cout, int level = 0,
		                                 const string& indent = "\t");
		ostream& printXmlParameterInfo  (ostream& out, int level,
		                                 const string& indent);
		ostream& printGlobalXmlParameterInfo(ostream& out, int level,
		                                 const string& indent);
		string   getXmlId               (const string& prefix = "") const;
		string   getXmlIdPrefix         (void) const;
		void     createLineFromTokens   (void);
		int      getLineIndex           (void) const;
		int      getLineNumber          (void) const;
		HumdrumFile* getOwner           (void);
		void     setText                (const string& text);
		string   getText                (void);

		HumNum   getDuration            (void) const;
		HumNum   getDurationFromStart   (void) const;
		HumNum   getDurationToEnd       (void) const;
		HumNum   getDurationFromBarline (void) const;
		HumNum   getDurationToBarline   (void) const;
		HumNum   getBarlineDuration     (void) const;

		HumNum   getDuration            (HumNum scale) const;
		HumNum   getDurationFromStart   (HumNum scale) const;
		HumNum   getDurationToEnd       (HumNum scale) const;
		HumNum   getDurationFromBarline (HumNum scale) const;
		HumNum   getDurationToBarline   (HumNum scale) const;
		HumNum   getBarlineDuration     (HumNum scale) const;
		int      getKernNoteAttacks     (void);
		int      addLinkedParameter     (HTp token);

		HumNum   getBeat                (HumNum beatdur = "1") const;
		HumNum   getBeat                (string beatrecip = "4") const;
		HTp      getTrackStart          (int track) const;
		void     setLineFromCsv         (const char* csv,
		                                 const string& separator = ",");
		void     setLineFromCsv         (const string& csv,
		                                 const string& separator = ",");

		// low-level editing functions (need to re-analyze structure after using)
		void     appendToken            (HTp token);
		void     appendToken            (const HumdrumToken& token);
		void     appendToken            (const string& token);
		void     appendToken            (const char* token);

		void     appendToken            (int index, HTp token);
		void     appendToken            (int index, const HumdrumToken& token);
		void     appendToken            (int index, const string& token);
		void     appendToken            (int index, const char* token);

		void     insertToken            (int index, HTp token);
		void     insertToken            (int index, const HumdrumToken& token);
		void     insertToken            (int index, const string& token);
		void     insertToken            (int index, const char* token);

		void     setDuration            (HumNum aDur);
		void     setDurationFromStart   (HumNum dur);
		void     setDurationFromBarline (HumNum dur);
		void     setDurationToBarline   (HumNum dur);

	protected:
		bool     analyzeTracks          (string& err);
		bool     analyzeTokenDurations  (string& err);
		void     setLineIndex           (int index);
		void     clear                  (void);
		void     setOwner               (void* hfile);
		int      createTokensFromLine   (void);
		void     setLayoutParameters    (void);
		void     setParameters          (const string& pdata);
		void     storeGlobalLinkedParameters(void);
		ostream&	printXmlGlobalLinkedParameterInfo(ostream& out = cout, int level = 0,
		                                 const string& indent = "\t");
		ostream& printXmlGlobalLinkedParameters(ostream& out = cout, int level = 0,
		                                 const string& indent = "\t");

	private:

		//
		// State variables managed by the HumdrumLine class:
		//

		// m_lineindex: Used to store the index number of the HumdrumLine in
		// the owning HumdrumFile object.
		// This variable is filled by HumdrumFileStructure::analyzeLines().
		int m_lineindex;

		// m_tokens: Used to store the individual tab-separated token fields
		// on a line.  These are prepared automatically after reading in
		// a full line of text (which is accessed throught the string parent
		// class).  If the full line is changed, the tokens are not updated
		// automatically -- use createTokensFromLine().  Likewise the full
		// text line is not updated if any tokens are changed -- use
		// createLineFromTokens() in that case.  The second case is more
		// useful: you can read in a HumdrumFile, tweak the tokens, then
		// reconstruct the full line and print out again.
		// This variable is filled by HumdrumFile::read().
		// The contents of this vector should be deleted when deconstructing
		// a HumdrumLine object.
		vector<HumdrumToken*> m_tokens;

		// m_duration: This is the "duration" of a line.  The duration is
		// equal to the minimum time unit of all durational tokens on the
		// line.  This also includes null tokens when the duration of a
		// previous note in a previous spine is ending on the line, so it is
		// not just the minimum duration on the line.
		// This variable is filled by HumdrumFileStructure::analyzeRhythm().
		HumNum m_duration;

		// m_durationFromStart: This is the cumulative duration of all lines
		// prior to this one in the owning HumdrumFile object.  For example,
		// the first notes in a score start at time 0, If the duration of the
		// first data line is 1 quarter note, then the durationFromStart for
		// the second line will be 1 quarter note.
		// This variable is filled by HumdrumFileStructure::analyzeRhythm().
		HumNum m_durationFromStart;

		// m_durationFromBarline: This is the cumulative duration from the
		// last barline to the current data line.
		// This variable is filled by HumdrumFileStructure::analyzeMeter().
		HumNum m_durationFromBarline;

		// m_durationToBarline: This is the duration from the start of the
		// current line to the next barline in the owning HumdrumFile object.
		// This variable is filled by HumdrumFileStructure::analyzeMeter().
		HumNum m_durationToBarline;

		// m_linkedParameters: List of Humdrum tokens which are parameters
		// (mostly only layout parameters at the moment)
		vector<HTp> m_linkedParameters;

		// owner: This is the HumdrumFile which manages the given line.
		void* m_owner;

	friend class HumdrumFileBase;
	friend class HumdrumFileStructure;
	friend class HumdrumFileContent;
	friend class HumdrumFile;
};

ostream& operator<< (ostream& out, HumdrumLine& line);
ostream& operator<< (ostream& out, HumdrumLine* line);




typedef HumdrumToken* HTp;

class HumdrumToken : public string, public HumHash {
	public:
		         HumdrumToken              (void);
		         HumdrumToken              (const HumdrumToken& token);
		         HumdrumToken              (HumdrumToken* token);
		         HumdrumToken              (const HumdrumToken& token,
		                                    HumdrumLine* owner);
		         HumdrumToken              (HumdrumToken* token,
		                                    HumdrumLine* owner);
		         HumdrumToken              (const char* token);
		         HumdrumToken              (const string& token);
		        ~HumdrumToken              ();

		bool     isNull                    (void) const;
		bool     isManipulator             (void) const;

		bool     isExclusiveInterpretation (void) const;
		bool     isSplitInterpretation     (void) const;
		bool     isMergeInterpretation     (void) const;
		bool     isExchangeInterpretation  (void) const;
		bool     isTerminateInterpretation (void) const;
		bool     isAddInterpretation       (void) const;

		// alises for the above
		bool     isExclusive               (void) const
		                                  { return isExclusiveInterpretation(); }
		bool     isExInterp                (void) const
		                                  { return isExclusiveInterpretation(); }
		bool     isSplit                   (void) const
		                                      { return isSplitInterpretation(); }
		bool     isMerge                   (void) const
		                                      { return isMergeInterpretation(); }
		bool     isExchange                (void) const
		                                   { return isExchangeInterpretation(); }
		bool     isTerminate               (void) const
		                                  { return isTerminateInterpretation(); }
		bool     isTerminator              (void) const
		                                  { return isTerminateInterpretation(); }
		bool     isAdd                     (void) const
		                                      { return isSplitInterpretation(); }

		bool     isBarline                 (void) const;
		bool     isCommentLocal            (void) const;
		bool     isCommentGlobal           (void) const;
		bool     isComment                 (void) const;
		bool     isData                    (void) const;
		bool     isInterpretation          (void) const;
		bool     isNonNullData             (void) const;
		bool     isNullData                (void) const;
		bool     isChord                   (const string& separator = " ");
		bool     isLabel                   (void) const;
		bool     hasRhythm                 (void) const;
		bool     hasBeam                   (void) const;
		bool     equalTo                   (const string& pattern);

		// kern-specific functions:
		bool     isRest                    (void);
		bool     isNote                    (void);
		bool     isSecondaryTiedNote       (void);
		bool     isSustainedNote           (void);
		bool     isNoteAttack              (void);
		bool     isInvisible               (void);
		bool     isGrace                   (void);
		bool     isClef                    (void);
		bool     isKeySignature            (void);
		bool     isKeyDesignation          (void);
		bool     isTimeSignature           (void);
		bool     isMensurationSymbol       (void);

		bool     hasSlurStart              (void);
		bool     hasSlurEnd                (void);
		int      hasVisibleAccidental      (int subtokenIndex) const;
		int      hasCautionaryAccidental   (int subtokenIndex) const;

		HumNum   getDuration               (void) const;
		HumNum   getDuration               (HumNum scale) const;
		HumNum   getTiedDuration           (void);
		HumNum   getTiedDuration           (HumNum scale);
		HumNum   getDurationNoDots         (void) const;
		HumNum   getDurationNoDots         (HumNum scale) const;
		int      getDots                   (char separator = ' ') const;

		HumNum   getDurationFromStart      (void) const;
		HumNum   getDurationFromStart      (HumNum scale) const;

		HumNum   getDurationToEnd          (void) const;
		HumNum   getDurationToEnd          (HumNum scale) const;

		HumNum   getDurationFromBarline    (void) const;
		HumNum   getDurationFromBarline    (HumNum scale) const;

		HumNum   getDurationToBarline      (void) const;
		HumNum   getDurationToBarline      (HumNum scale) const;

		HumNum   getBarlineDuration        (void) const;
		HumNum   getBarlineDuration        (HumNum scale) const;

		HumdrumLine* getOwner              (void) const;
		HumdrumLine* getLine               (void) const { return getOwner(); }
		bool     equalChar                 (int index, char ch) const;

		HTp      resolveNull               (void);
		void     setNullResolution         (HTp resolution);
		int      getLineIndex              (void) const;
		int      getLineNumber             (void) const;
		int      getFieldIndex             (void) const;
		int      getFieldNumber            (void) const;
		int      getTokenIndex             (void) const;
		int      getTokenNumber            (void) const;
		const string& getDataType          (void) const;
		bool     isDataType                (string dtype) const;
		bool     isKern                    (void) const;
		string   getSpineInfo              (void) const;
		int      getTrack                  (void) const;
		int      getSubtrack               (void) const;
		bool     noteInLowerSubtrack       (void);
		string   getTrackString            (void) const;
		int      getSubtokenCount          (const string& separator = " ") const;
		string   getSubtoken               (int index,
		                                    const string& separator = " ") const;
		void     setParameters             (HTp ptok);
		void     setParameters             (const string& pdata, HTp ptok = NULL);
		int      getStrandIndex            (void) const;
		int      getSlurStartElisionLevel  (int index = 0) const;
		int      getSlurEndElisionLevel    (int index = 0) const;
		HTp      getSlurStartToken         (int number = 0);
		HTp      getSlurEndToken           (int number = 0);
		void     storeLinkedParameters     (void);
		bool     linkedParameterIsGlobal   (int index);
		ostream& printCsv                  (ostream& out = cout);
		ostream& printXml                  (ostream& out = cout, int level = 0,
		                                    const string& indent = "\t");
		ostream& printGlobalXmlParameterInfo(ostream& out = cout, int level = 0,
		                                   const string& indent = "\t");
		string   getXmlId                  (const string& prefix = "") const;
		string   getXmlIdPrefix            (void) const;
		void     setText                   (const string& text);
		string   getText                   (void) const;
		int      addLinkedParameter        (HTp token);
		int      getLinkedParameterCount   (void);
		HumParamSet* getLinkedParameter    (int index);
		HumParamSet* getLinkedParameter    (void);
		ostream& printXmlLinkedParameterInfo(ostream& out, int level, const string& indent);

		// layout parameter accessors
		string   getVisualDuration         (void);

		HumdrumToken& operator=            (HumdrumToken& aToken);
		HumdrumToken& operator=            (const string& aToken);
		HumdrumToken& operator=            (const char* aToken);

		// next/previous token functions:
		int         getNextTokenCount      (void) const;
		int         getPreviousTokenCount  (void) const;
		HTp         getNextToken           (int index = 0) const;
		HTp         getPreviousToken       (int index = 0) const;
		vector<HTp> getNextTokens          (void) const;
		vector<HTp> getPreviousTokens      (void) const;

		// next/previous token on line:
		HTp      getNextFieldToken           (void) const;
		HTp      getPreviousFieldToken       (void) const;

		int      getPreviousNonNullDataTokenCount(void);
		int      getPreviousNNDTCount      (void)
		                           { return getPreviousNonNullDataTokenCount(); }
		HTp      getPreviousNonNullDataToken(int index = 0);
		HTp      getPreviousNNDT           (int index = 0)
		                           { return getPreviousNonNullDataToken(index); }
		int      getNextNonNullDataTokenCount(void);
		int      getNextNNDTCount          (void)
		                               { return getNextNonNullDataTokenCount(); }
		HTp      getNextNonNullDataToken   (int index = 0);
		HTp      getNextNNDT               (int index = 0)
		                               { return getNextNonNullDataToken(index); }

		// slur-analysis based functions:
		HumNum   getSlurDuration           (HumNum scale = 1);

		void     setTrack                  (int aTrack, int aSubtrack);
		void     setTrack                  (int aTrack);

	protected:
		void     setLineIndex              (int lineindex);
		void     setFieldIndex             (int fieldlindex);
		void     setSpineInfo              (const string& spineinfo);
		void     setSubtrack               (int aSubtrack);
		void     setSubtrackCount          (int count);
		void     setPreviousToken          (HTp aToken);
		void     setNextToken              (HTp aToken);
		void     addNextNonNullToken       (HTp token);
		void     makeForwardLink           (HumdrumToken& nextToken);
		void     makeBackwardLink          (HumdrumToken& previousToken);
		void     setOwner                  (HumdrumLine* aLine);
		int      getState                  (void) const;
		void     incrementState            (void);
		void     setDuration               (const HumNum& dur);
		void     setStrandIndex            (int index);

		bool     analyzeDuration          (string& err);
		ostream& printXmlBaseInfo         (ostream& out = cout, int level = 0,
		                                   const string& indent = "\t");
		ostream& printXmlContentInfo      (ostream& out = cout, int level = 0,
		                                   const string& indent = "\t");
		ostream& printXmlStructureInfo    (ostream& out = cout, int level = 0,
		                                   const string& indent = "\t");
		ostream& printXmlParameterInfo    (ostream& out = cout, int level = 0,
		                                   const string& indent = "\t");
		ostream&	printXmlLinkedParameters (ostream& out = cout, int level = 0,
		                                   const string& indent = "\t");

	private:
		// address: The address contains information about the location of
		// the token on a HumdrumLine and in a HumdrumFile.
		HumAddress m_address;

		// duration: The duration of the token.  Non-rhythmic data types
		// will have a negative duration (which should be interpreted
		// as a zero duration--See HumdrumToken::hasRhythm()).
		// Grace note will have a zero duration, even if they have a duration
		// list in the token for a graphical display duration.
		HumNum m_duration;

		// nextTokens: This is a list of all previous tokens in the spine which
		// immediately precede this token. Typically there will be one
		// following token, but there can be two tokens if the current
		// token is *^, and there will be zero following tokens after a
		// spine terminating token (*-).
		vector<HTp> m_nextTokens;     // link to next token(s) in spine

		// previousTokens: Simiar to nextTokens, but for the immediately
		// follow token(s) in the data.  Typically there will be one
		// preceding token, but there can be multiple tokens when the previous
		// line has *v merge tokens for the spine.  Exclusive interpretations
		// have no tokens preceding them.
		vector<HTp> m_previousTokens; // link to last token(s) in spine

		// nextNonNullTokens: This is a list of non-tokens in the spine
		// that follow this one.
		vector<HTp> m_nextNonNullTokens;

		// previousNonNullTokens: This is a list of non-tokens in the spine
		// that preced this one.
		vector<HTp> m_previousNonNullTokens;

		// rhycheck: Used to perfrom HumdrumFileStructure::analyzeRhythm
		// recursively.
		int m_rhycheck;

		// strand: Used to keep track of contiguous voice connections between
		// secondary spines/tracks.  This is the 1-D strand index number
		// (not the 2-d one).
		int m_strand;

		// m_nullresolve: used to point to the token that a null token
		// refers to.
		HTp m_nullresolve;

		// m_linkedParameters: List of Humdrum tokens which are parameters
		// (mostly only layout parameters at the moment).
		vector<HTp> m_linkedParameters;

		// m_linkedParameter: A single parameter encoded in the text of the
		// token.
		HumParamSet* m_linkedParameter = NULL;

	friend class HumdrumLine;
	friend class HumdrumFileBase;
	friend class HumdrumFileStructure;
	friend class HumdrumFileContent;
	friend class HumdrumFile;
};


ostream& operator<<(ostream& out, const HumdrumToken& token);
ostream& operator<<(ostream& out, HTp token);

ostream& printSequence(vector<vector<HTp> >& sequence, ostream& out=std::cout);
ostream& printSequence(vector<HTp>& sequence, ostream& out = std::cout);



// The following options are used for get[Primary]TrackTokens:
// * OPT_PRIMARY    => only extract primary subspine/subtrack.
// * OPT_NOEMPTY    => don't include null tokens in extracted list if all
//                        extracted subspines contains null tokens.
//                        Includes null interpretations and comments as well.
// * OPT_NONULL     => don't include any null tokens in extracted list.
// * OPT_NOINTERP   => don't include interprtation tokens.
// * OPT_NOMANIP    => don't include spine manipulators (*^, *v, *x, *+,
//                        but still keep ** and *0).
// * OPT_NOCOMMENT  => don't include comment tokens.
// * OPT_NOGLOBALS  => don't include global records (global comments, reference
//                        records, and empty lines). In other words, only return
//                        a list of tokens from lines which hasSpines() it true.
// * OPT_NOREST     => don't include **kern rests.
// * OPT_NOTIE      => don't include **kern secondary tied notes.
//
// Compound options:
// * OPT_DATA      (OPT_NOMANIP | OPT_NOCOMMENT | OPT_NOGLOBAL)
//     Only data tokens (including barlines)
// * OPT_ATTACKS   (OPT_DATA | OPT_NOREST | OPT_NOTIE | OPT_NONULL)
//     Only note-attack tokens (when etracting **kern data)
//
#define OPT_PRIMARY   0x001
#define OPT_NOEMPTY   0x002
#define OPT_NONULL    0x004
#define OPT_NOINTERP  0x008
#define OPT_NOMANIP   0x010
#define OPT_NOCOMMENT 0x020
#define OPT_NOGLOBAL  0x040
#define OPT_NOREST    0x080
#define OPT_NOTIE     0x100
#define OPT_DATA      (OPT_NOMANIP | OPT_NOCOMMENT | OPT_NOGLOBAL)
#define OPT_ATTACKS   (OPT_DATA | OPT_NOREST | OPT_NOTIE | OPT_NONULL)


class TokenPair {
	public:
		TokenPair(void) { clear(); }
		~TokenPair() { clear(); }
		void clear(void) {
			first = NULL;
			last  = NULL;
		}
		HTp first;
		HTp last;
};


bool sortTokenPairsByLineIndex(const TokenPair& a, const TokenPair& b);


class HumdrumFileBase : public HumHash {
	public:
		              HumdrumFileBase          (void);
		              HumdrumFileBase          (HumdrumFileBase& infile);
		              HumdrumFileBase          (const string& contents);
		              HumdrumFileBase          (istream& contents);
		             ~HumdrumFileBase          ();

		HumdrumFileBase& operator=             (HumdrumFileBase& infile);
		bool          read                     (istream& contents);
		bool          read                     (const char* filename);
		bool          read                     (const string& filename);
		bool          readCsv                  (istream& contents,
		                                        const string& separator=",");
		bool          readCsv                  (const char* contents,
		                                        const string& separator=",");
		bool          readCsv                  (const string& contents,
		                                        const string& separator=",");

		bool          readString               (const char* contents);
		bool          readString               (const string& contents);
		bool          readStringCsv            (const char* contents,
		                                        const string& separator=",");
		bool          readStringCsv            (const string& contents,
		                                        const string& separator=",");
		bool          isValid                  (void);
		string        getParseError            (void) const;
		bool          isQuiet                  (void) const;
		void          setQuietParsing          (void);
		void          setNoisyParsing          (void);
		void          clear                    (void);

		bool          parse                    (istream& contents)
		                                    { return read(contents); }
		bool          parse                    (const char* contents)
		                                    { return readString(contents); }
		bool          parse                    (const string& contents)
		                                    { return readString(contents); }
		bool          parseCsv                 (istream& contents,
		                                        const string& separator = ",")
		                                    { return readCsv(contents); }
		bool          parseCsv                 (const char* contents,
		                                        const string& separator = ",")
		                                    { return readStringCsv(contents); }
		bool          parseCsv                 (const string& contents,
		                                        const string& separator = ",")
		                                    { return readStringCsv(contents); }

		void          setXmlIdPrefix           (const string& value);
		string        getXmlIdPrefix           (void);
		void          setFilename              (const string& filename);
		string        getFilename              (void);

		void          setSegmentLevel          (int level = 0);
		int           getSegmentLevel          (void);
		ostream&      printSegmentLabel        (ostream& out);
		ostream&      printNonemptySegmentLabel(ostream& out);

		HumdrumLine&  operator[]               (int index);
		HumdrumLine*  getLine                  (int index);
		int           getLineCount             (void) const;
		HTp           token                    (int lineindex, int fieldindex);
		string        token                    (int lineindex, int fieldindex,
		                                        int subtokenindex,
		                                        const string& separator = " ");
		int           getMaxTrack              (void) const;
		int           getTrackCount            (void) const
		                                                { return getMaxTrack(); }
		int           getSpineCount            (void) const
		                                                { return getMaxTrack(); }
		int           getMeasureNumber         (int line);
		ostream&      printSpineInfo           (ostream& out = cout);
		ostream&      printDataTypeInfo        (ostream& out = cout);
		ostream&      printTrackInfo           (ostream& out = cout);
		ostream&      printCsv                 (ostream& out = cout,
		                                        const string& separator = ",");
		ostream&      printFieldNumber         (int fieldnum, ostream& out);
		ostream&      printFieldIndex          (int fieldind, ostream& out);
		void          usage                    (const string& command);
		void          example                  (void);

		HTp           getTrackStart            (int track) const;
		HTp           getSpineStart            (int spine) const
		                                       { return getTrackStart(spine+1); }
		void          getSpineStartList        (vector<HTp>& spinestarts);
		void          getSpineStartList        (vector<HTp>& spinestarts,
		                                        const string& exinterp);
		void          getKernSpineStartList    (vector<HTp>& spinestarts);
		vector<HTp>   getKernSpineStartList    ();
		void          getSpineStartList        (vector<HTp>& spinestarts,
		                                        const vector<string>& exinterps);
		void          getTrackStartList        (vector<HTp>& spinestarts)
		                               { return getSpineStartList(spinestarts); }
		void          getTrackStartList        (vector<HTp>& spinestarts,
		                                        const string& exinterp)
		                     { return getSpineStartList(spinestarts, exinterp); }
		void          getTrackStartList        (vector<HTp>& spinestarts,
		                                        const vector<string>& exinterps)
		                    { return getSpineStartList(spinestarts, exinterps); }

		int           getTrackEndCount         (int track) const;
		HTp           getTrackEnd              (int track, int subtrack) const;
		void          createLinesFromTokens    (void);

		void          appendLine               (const char* line);
		void          appendLine               (const string& line);
		void          appendLine               (HumdrumLine* line);
		void          push_back                (const char* line)
		                                                    { appendLine(line); }
		void          push_back                (const string& line)
		                                                    { appendLine(line); }
		void          push_back                (HumdrumLine* line)
		                                                    { appendLine(line); }

		void          insertLine               (int index, const char* line);
		void          insertLine               (int index, const string& line);
		void          insertLine               (int index, HumdrumLine* line);
//		void          adjustMergeSpineLines    (void);

		HumdrumLine*  back                     (void);
		void          makeBooleanTrackList     (vector<bool>& spinelist,
		                                        const string& spinestring);
		bool          analyzeBaseFromLines     (void);
		bool          analyzeBaseFromTokens    (void);


		vector<HumdrumLine*> getReferenceRecords(void);

		// spine analysis functionality:
		void          getTrackSequence         (vector<vector<HTp> >& sequence,
		                                        HTp starttoken, int options);
		void          getTrackSequence         (vector<vector<HTp> >& sequence,
		                                        int track, int options);
		void          getPrimaryTrackSequence  (vector<HTp>& sequence,
		                                        int track, int options);

		void          getSpineSequence         (vector<vector<HTp> >& sequence,
		                                        HTp starttoken, int options);
		void          getSpineSequence         (vector<vector<HTp> >& sequence,
		                                        int spine, int options);
		void          getPrimarySpineSequence  (vector<HTp>& sequence,
		                                        int spine, int options);

		void          getTrackSeq              (vector<vector<HTp> >& sequence,
		                                        HTp starttoken, int options)
		                     { getTrackSequence(sequence, starttoken, options); }
		void          getTrackSeq              (vector<vector<HTp> >& sequence,
		                                        int track, int options)
		                          { getTrackSequence(sequence, track, options); }
		void          getPrimaryTrackSeq       (vector<HTp>& sequence,
		                                        int track, int options)
		                    {getPrimaryTrackSequence(sequence, track, options); }

		// functions defined in HumdrumFileBase-net.cpp:
		static string getUriToUrlMapping        (const string& uri);
		void          readFromHumdrumUri        (const string& humaddress);
		void          readFromJrpUri            (const string& jrpaddress);
		void          readFromHttpUri           (const string& webaddress);
		static void   readStringFromHttpUri     (stringstream& inputdata,
		                                         const string& webaddress);

	protected:
		static int    getChunk                  (int socket_id,
		                                         stringstream& inputdata,
		                                         char* buffer, int bufsize);
		static int    getFixedDataSize          (int socket_id,
		                                         int datalength,
		                                         stringstream& inputdata,
		                                         char* buffer, int bufsize);
		static void   prepare_address           (struct sockaddr_in *address,
		                                         const string& hostname,
		                                         unsigned short int port);
		static int    open_network_socket       (const string& hostname,
		                                         unsigned short int port);

	protected:
		bool          analyzeTokens             (void);
		bool          analyzeSpines             (void);
		bool          analyzeLinks              (void);
		bool          analyzeTracks             (void);
		bool          analyzeLines              (void);
		bool          adjustSpines              (HumdrumLine& line,
		                                         vector<string>& datatype,
		                                         vector<string>& sinfo);
		string        getMergedSpineInfo        (vector<string>& info,
		                                         int starti, int extra);
		bool          stitchLinesTogether       (HumdrumLine& previous,
		                                         HumdrumLine& next);
		void          addToTrackStarts          (HTp token);
		bool          analyzeNonNullDataTokens  (void);
		void          addUniqueTokens           (vector<HTp>& target,
		                                         vector<HTp>& source);
		bool          processNonNullDataTokensForTrackForward(HTp starttoken,
		                                         vector<HTp> ptokens);
		bool          processNonNullDataTokensForTrackBackward(HTp starttoken,
		                                         vector<HTp> ptokens);
		bool          setParseError             (stringstream& err);
		bool          setParseError             (const string& err);
		bool          setParseError             (const char* format, ...);
//		void          fixMerges                 (int linei);

	protected:

		// m_lines: an array representing lines from the input file.
		// The contents of lines must be deallocated when deconstructing object.
		vector<HumdrumLine*> m_lines;

		// m_filename: name of the file which was loaded.
		string m_filename;

		// m_segementlevel: segment level (e.g., work/movement)
		int m_segmentlevel;

		// m_trackstarts: list of addresses of the exclusive interpreations
		// in the file.  The first element in the list is reserved, so the
		// number of tracks (primary spines) is equal to one less than the
		// size of this list.
		vector<HTp> m_trackstarts;

		// m_trackends: list of the addresses of the spine terminators in the
		// file. It is possible that spines can split and their subspines do not
		// merge before termination; therefore, the ends are stored in
		// a 2d array. The first dimension is the track number, and the second
		// dimension is the list of terminators.
		vector<vector<HTp> > m_trackends;

		// m_barlines: list of barlines in the data.  If the first measures is
		// a pickup measure, then the first entry will not point to the first
		// starting exclusive interpretation line rather than to a barline.
		vector<HumdrumLine*> m_barlines;
		// Maybe also add "measures" which are complete metrical cycles.

		// m_ticksperquarternote: this is the number of tick
		int m_ticksperquarternote;

		// m_idprefix: an XML id prefix used to avoid id collisions when
		// includeing multiple HumdrumFile XML in a single group.
		string m_idprefix;

		// m_strands1d: one-dimensional list of spine strands.
		vector<TokenPair> m_strand1d;

		// m_strands2d: two-dimensional list of spine strands.
		vector<vector<TokenPair> > m_strand2d;

		// m_quietParse: Set to true if error messages should not be
		// printed to the console when reading.
		bool m_quietParse;

		// m_parseError: Set to true if a read is successful.
		string m_parseError;

		// m_displayError: Used to print error message only once.
		bool m_displayError;

	public:
		// Dummy functions to allow the HumdrumFile class's inheritance
		// to be shifted between HumdrumFileContent (the top-level default),
		// HumdrumFileStructure (mid-level interface), or HumdrumFileBase
		// (low-level interface).

		//
		// HumdrumFileStructure public functions:
		//
		bool readNoRhythm      (istream& infile) { return read(infile); };
		bool readNoRhythm      (const char*   filename) {return read(filename);};
		bool readNoRhythm      (const string& filename) {return read(filename);};
		bool readStringNoRhythm(const char*   contents) {return read(contents);};
		bool readStringNoRhythm(const string& contents) {return read(contents);};
		HumNum       getScoreDuration           (void) const { return 0; };
		ostream&     printDurationInfo          (ostream& out=cout) {return out;};
		int          tpq                        (void) { return 0; }
		int          getBarlineCount            (void) const { return 0; }
		HumdrumLine* getBarline                 (int index) const { return NULL;};
		HumNum       getBarlineDuration         (int index) const { return 0; };
		HumNum       getBarlineDurationFromStart(int index) const { return 0; };
		HumNum       getBarlineDurationToEnd    (int index) const { return 0; };

		// HumdrumFileContent public functions:
		// to be added later

};

ostream& operator<<(ostream& out, HumdrumFileBase& infile);



class HumdrumFileStructure : public HumdrumFileBase {
	public:
		              HumdrumFileStructure         (void);
		              HumdrumFileStructure         (const string& filename);
		              HumdrumFileStructure         (istream& contents);
		             ~HumdrumFileStructure         ();
		bool          hasFilters                   (void);

		// TSV reading functions:
		bool          read                         (istream& contents);
		bool          read                         (const char*   filename);
		bool          read                         (const string& filename);
		bool          readString                   (const char*   contents);
		bool          readString                   (const string& contents);
		bool parse(istream& contents)      { return read(contents); }
		bool parse(const char* contents)   { return readString(contents); }
		bool parse(const string& contents) { return readString(contents); }
		bool          readNoRhythm                 (istream& contents);
		bool          readNoRhythm                 (const char*   filename);
		bool          readNoRhythm                 (const string& filename);
		bool          readStringNoRhythm           (const char*   contents);
		bool          readStringNoRhythm           (const string& contents);

		// CSV reading functions:
		bool          readCsv                      (istream& contents,
		                                            const string& separator=",");
		bool          readCsv                      (const char*   filename,
		                                            const string& separator=",");
		bool          readCsv                      (const string& filename,
		                                            const string& separator=",");
		bool          readStringCsv                (const char*   contents,
		                                            const string& separator=",");
		bool          readStringCsv                (const string& contents,
		                                            const string& separator=",");
		bool parseCsv(istream& contents, const string& separator = ",")
		                                 { return readCsv(contents, separator); }
		bool parseCsv(const char* contents, const string& separator = ",")
		                           { return readStringCsv(contents, separator); }
		bool parseCsv(const string& contents, const string& separator = ",")
		                           { return readStringCsv(contents, separator); }
		bool          readNoRhythmCsv              (istream& contents,
		                                            const string& separator = ",");
		bool          readNoRhythmCsv              (const char*   filename,
		                                            const string& separator = ",");
		bool          readNoRhythmCsv              (const string& filename,
		                                            const string& separator = ",");
		bool          readStringNoRhythmCsv        (const char*   contents,
		                                            const string& separator = ",");
		bool          readStringNoRhythmCsv        (const string& contents,
		                                            const string& separator = ",");

		// rhythmic analysis related functionality:
		HumNum        getScoreDuration             (void) const;
		ostream&      printDurationInfo            (ostream& out = cout);
		int           tpq                          (void);

		// strand functionality:
		HTp           getStrandStart               (int index) const;
		HTp           getStrandEnd                 (int index) const;
		HTp           getStrandStart               (int sindex, int index) const;
		HTp           getStrandEnd                 (int sindex, int index) const;
		int           getStrandCount               (void) const;
		int           getStrandCount               (int spineindex) const;
		void          resolveNullTokens            (void);

		HTp           getStrand                    (int index) const
		                                        { return getStrandStart(index); }
		HTp           getStrand                    (int sindex, int index) const
		                                { return getStrandStart(sindex, index); }

		// barline/measure functionality:
		int           getBarlineCount              (void) const;
		HumdrumLine*  getBarline                   (int index) const;
		HumNum        getBarlineDuration           (int index) const;
		HumNum        getBarlineDurationFromStart  (int index) const;
		HumNum        getBarlineDurationToEnd      (int index) const;

		bool          analyzeStructure             (void);
		bool          analyzeStrands               (void);

	protected:

		bool          analyzeRhythm                (void);
		bool          assignRhythmFromRecip        (HTp spinestart);
		bool          analyzeMeter                 (void);
		bool          analyzeTokenDurations        (void);
		bool          analyzeGlobalParameters      (void);
		bool          analyzeLocalParameters       (void);
		bool          analyzeDurationsOfNonRhythmicSpines(void);
		HumNum        getMinDur                    (vector<HumNum>& durs,
		                                            vector<HumNum>& durstate);
		bool          getTokenDurations            (vector<HumNum>& durs,
		                                            int line);
		bool          cleanDurs                    (vector<HumNum>& durs,
		                                            int line);
		bool          decrementDurStates           (vector<HumNum>& durs,
		                                            HumNum linedur, int line);
		bool          assignDurationsToTrack       (HTp starttoken,
		                                            HumNum startdur);
		bool          prepareDurations             (HTp token, int state,
		                                            HumNum startdur);
		bool          setLineDurationFromStart     (HTp token, HumNum dursum);
		bool          analyzeRhythmOfFloatingSpine (HTp spinestart);
		bool          analyzeNullLineRhythms       (void);
		void          fillInNegativeStartTimes     (void);
		void          assignLineDurations          (void);
		void          assignStrandsToTokens        (void);
		set<HumNum>   getNonZeroLineDurations      (void);
		set<HumNum>   getPositiveLineDurations     (void);
		void          processLocalParametersForStrand(int index);
		bool          processLocalParametersForTrack (HTp starttok, HTp current);
		void          checkForLocalParameters      (HTp token, HTp current);
		bool          assignDurationsToNonRhythmicTrack(HTp endtoken, HTp ptoken);
		void          analyzeSpineStrands          (vector<TokenPair>& ends,
		                                            HTp starttok);
};



class HumdrumFileContent : public HumdrumFileStructure {
	public:
		       HumdrumFileContent         (void);
		       HumdrumFileContent         (const string& filename);
		       HumdrumFileContent         (istream& contents);
		      ~HumdrumFileContent         ();

		bool   analyzeKernSlurs           (void);
		bool   analyzeKernTies            (void);
		bool   analyzeKernAccidentals     (void);

		bool   analyzeRScale              (void);

		// in HumdrumFileContent-metlev.cpp
		void  getMetricLevels             (vector<double>& output, int track = 0,
		                                   double undefined = NAN);
		// in HumdrumFileContent-timesig.cpp
		void  getTimeSigs                 (vector<pair<int, HumNum> >& output,
		                                   int track = 0);

		template <class DATATYPE>
		bool   prependDataSpine           (vector<DATATYPE> data,
		                                   const string& null = ".",
		                                   const string& exinterp = "**data",
		                                   bool recalcLine = true);

		template <class DATATYPE>
		bool   appendDataSpine            (vector<DATATYPE> data,
		                                   const string& null = ".",
		                                   const string& exinterp = "**data",
		                                   bool recalcLine = true);

		template <class DATATYPE>
		bool   insertDataSpineBefore      (int nexttrack,
		                                   vector<DATATYPE> data,
		                                   const string& null = ".",
		                                   const string& exinterp = "**data",
		                                   bool recalcLine = true);

		template <class DATATYPE>
		bool   insertDataSpineAfter       (int prevtrack,
		                                   vector<DATATYPE> data,
		                                   const string& null = ".",
		                                   const string& exinterp = "**data",
		                                   bool recalcLine = true);

	protected:
		bool   analyzeKernSlurs           (HumdrumToken* spinestart);
		bool   analyzeKernTies            (HumdrumToken* spinestart);
		void   fillKeySignature           (vector<int>& states,
		                                   const string& keysig);
		void   resetDiatonicStatesWithKeySignature(vector<int>& states,
				                             vector<int>& signature);
		void    linkSlurEndpoints         (HTp slurstart, HTp slurend);
};


//
// Templates:
//


//////////////////////////////
//
// HumdrumFileContent::prependDataSpine -- prepend a data spine
//     to the file.  Returns true if successful; false otherwise.
//
//     data == numeric or string data to print
//     null == if the data is converted to a string is equal to this
//             string then set the data spine content to a null token, ".".
//             default is ".".
//     exinterp == the exterp string to use.  Default is "**data".
//     recalcLine == boolean for whether or not to recalculate line string.
//                   Default is true;
//

template <class DATATYPE>
bool HumdrumFileContent::prependDataSpine(vector<DATATYPE> data,
		const string& null, const string& exinterp, bool recalcLine) {

	if ((int)data.size() != getLineCount()) {
		return false;
	}

	string ex;
	if (exinterp.find("**") == 0) {
		ex = exinterp;
	} else if (exinterp.find("*") == 0) {
		ex = "*" + exinterp;
	} else {
		ex = "**" + exinterp;
	}
	if (ex.size() <= 2) {
		ex += "data";
	}

	stringstream ss;
	HumdrumFileContent& infile = *this;
	HumdrumLine* line;
	for (int i=0; i<infile.getLineCount(); i++) {
		line = infile.getLine(i);
		if (!line->hasSpines()) {
			continue;
		}
		if (line->isExclusive()) {
			line->insertToken(0, ex);
		} else if (line->isTerminator()) {
			line->insertToken(0, "*-");
		} else if (line->isInterpretation()) {
			line->insertToken(0, "*");
		} else if (line->isLocalComment()) {
			line->insertToken(0, "!");
		} else if (line->isBarline()) {
			line->insertToken(0, (string)*infile.token(i, 0));
		} else if (line->isData()) {
			ss.str(string());
			ss << data[i];
			if (ss.str() == null) {
				line->insertToken(0, ".");
			} else if (ss.str() == "") {
				line->insertToken(0, ".");
			} else {
				line->insertToken(0, ss.str());
			}
		} else{
			cerr << "!!strange error for line " << i+1 << ":\t" << line << endl;
		}
		if (recalcLine) {
			line->createLineFromTokens();
		}
	}
	return true;
}



//////////////////////////////
//
// HumdrumFileContent::appendDataSpine -- prepend a data spine
//     to the file.  Returns true if successful; false otherwise.
//
//     data == numeric or string data to print
//     null == if the data is converted to a string is equal to this
//             string then set the data spine content to a null token, ".".
//             default is ".".
//     exinterp == the exterp string to use.  Default is "**data".
//     recalcLine == boolean for whether or not to recalculate line string.
//                   Default is true;
//

template <class DATATYPE>
bool HumdrumFileContent::appendDataSpine(vector<DATATYPE> data,
		const string& null, const string& exinterp, bool recalcLine) {

	if ((int)data.size() != getLineCount()) {
		cerr << "DATA SIZE DOES NOT MATCH GETLINECOUNT " << endl;
		cerr << "DATA SIZE " << data.size() << "\tLINECOUNT ";
		cerr  << getLineCount() << endl;
		return false;
	}

	string ex;
	if (exinterp.find("**") == 0) {
		ex = exinterp;
	} else if (exinterp.find("*") == 0) {
		ex = "*" + exinterp;
	} else {
		ex = "**" + exinterp;
	}
	if (ex.size() <= 2) {
		ex += "data";
	}

	stringstream ss;
	HumdrumFileContent& infile = *this;
	HumdrumLine* line;
	for (int i=0; i<infile.getLineCount(); i++) {
		line = infile.getLine(i);
		if (!line->hasSpines()) {
			continue;
		}
		if (line->isExclusive()) {
			line->appendToken(ex);
		} else if (line->isTerminator()) {
			line->appendToken("*-");
		} else if (line->isInterpretation()) {
			line->appendToken("*");
		} else if (line->isLocalComment()) {
			line->appendToken("!");
		} else if (line->isBarline()) {
			line->appendToken((string)*infile.token(i, 0));
		} else if (line->isData()) {
			ss.str(string());
			ss << data[i];
			if (ss.str() == null) {
				line->appendToken(".");
			} else if (ss.str() == "") {
				line->appendToken(".");
			} else {
				line->appendToken(ss.str());
			}
		} else{
			cerr << "!!strange error for line " << i+1 << ":\t" << line << endl;
		}
		if (recalcLine) {
			line->createLineFromTokens();
		}
	}
	return true;
}



//////////////////////////////
//
// HumdrumFileContent::insertDataSpineBefore -- prepend a data spine
//     to the file before the given spine.  Returns true if successful;
//     false otherwise.
//
//     nexttrack == track number to insert before.
//     data == numeric or string data to print
//     null == if the data is converted to a string is equal to this
//             string then set the data spine content to a null token, ".".
//             default is ".".
//     exinterp == the exterp string to use.  Default is "**data".
//     recalcLine == boolean for whether or not to recalculate line string.
//                   Default is true;
//

template <class DATATYPE>
bool HumdrumFileContent::insertDataSpineBefore(int nexttrack,
		vector<DATATYPE> data, const string& null, const string& exinterp,
		bool recalcLine) {

	if ((int)data.size() != getLineCount()) {
		cerr << "DATA SIZE DOES NOT MATCH GETLINECOUNT " << endl;
		cerr << "DATA SIZE " << data.size() << "\tLINECOUNT ";
		cerr  << getLineCount() << endl;
		return false;
	}

	string ex;
	if (exinterp.find("**") == 0) {
		ex = exinterp;
	} else if (exinterp.find("*") == 0) {
		ex = "*" + exinterp;
	} else {
		ex = "**" + exinterp;
	}
	if (ex.size() <= 2) {
		ex += "data";
	}

	stringstream ss;
	HumdrumFileContent& infile = *this;
	HumdrumLine* line;
	int insertionField = -1;
	int track;
	for (int i=0; i<infile.getLineCount(); i++) {
		line = infile.getLine(i);
		if (!line->hasSpines()) {
			continue;
		}
		insertionField = -1;
		for (int j=0; j<line->getFieldCount(); j++) {
			track = line->token(j)->getTrack();
			if (track != nexttrack) {
				continue;
			}
			insertionField = j;
			break;
		}
		if (insertionField < 0) {
			return false;
		}

		if (line->isExclusive()) {
			line->insertToken(insertionField, ex);
		} else if (line->isTerminator()) {
			line->insertToken(insertionField, "*-");
		} else if (line->isInterpretation()) {
			line->insertToken(insertionField, "*");
		} else if (line->isLocalComment()) {
			line->insertToken(insertionField, "!");
		} else if (line->isBarline()) {
			line->insertToken(insertionField, (string)*infile.token(i, 0));
		} else if (line->isData()) {
			ss.str(string());
			ss << data[i];
			if (ss.str() == null) {
				line->insertToken(insertionField, ".");
			} else if (ss.str() == "") {
				line->insertToken(insertionField, ".");
			} else {
				line->insertToken(insertionField, ss.str());
			}
		} else{
			cerr << "!!strange error for line " << i+1 << ":\t" << line << endl;
		}
		if (recalcLine) {
			line->createLineFromTokens();
		}
	}
	return true;
}



//////////////////////////////
//
// HumdrumFileContent::insertDataSpineAfter -- appen a data spine
//     to the file after the given spine.  Returns true if successful;
//     false otherwise.
//
//     prevtrack == track number to insert after.
//     data == numeric or string data to print
//     null == if the data is converted to a string is equal to this
//             string then set the data spine content to a null token, ".".
//             default is ".".
//     exinterp == the exterp string to use.  Default is "**data".
//     recalcLine == boolean for whether or not to recalculate line string.
//                   Default is true;
//

template <class DATATYPE>
bool HumdrumFileContent::insertDataSpineAfter(int prevtrack,
		vector<DATATYPE> data, const string& null, const string& exinterp,
		bool recalcLine) {

	if ((int)data.size() != getLineCount()) {
		cerr << "DATA SIZE DOES NOT MATCH GETLINECOUNT " << endl;
		cerr << "DATA SIZE " << data.size() << "\tLINECOUNT ";
		cerr  << getLineCount() << endl;
		return false;
	}

	string ex;
	if (exinterp.find("**") == 0) {
		ex = exinterp;
	} else if (exinterp.find("*") == 0) {
		ex = "*" + exinterp;
	} else {
		ex = "**" + exinterp;
	}
	if (ex.size() <= 2) {
		ex += "data";
	}

	stringstream ss;
	HumdrumFileContent& infile = *this;
	HumdrumLine* line;
	int insertionField = -1;
	int track;
	for (int i=0; i<infile.getLineCount(); i++) {
		line = infile.getLine(i);
		if (!line->hasSpines()) {
			continue;
		}
		insertionField = -1;
		for (int j = line->getFieldCount() - 1; j >= 0; j--) {
			track = line->token(j)->getTrack();
			if (track != prevtrack) {
				continue;
			}
			insertionField = j;
			break;
		}
		insertionField++;
		if (insertionField < 0) {
			return false;
		}

		if (line->isExclusive()) {
			line->insertToken(insertionField, ex);
		} else if (line->isTerminator()) {
			line->insertToken(insertionField, "*-");
		} else if (line->isInterpretation()) {
			line->insertToken(insertionField, "*");
		} else if (line->isLocalComment()) {
			line->insertToken(insertionField, "!");
		} else if (line->isBarline()) {
			line->insertToken(insertionField, (string)*infile.token(i, 0));
		} else if (line->isData()) {
			ss.str(string());
			ss << data[i];
			if (ss.str() == null) {
				line->insertToken(insertionField, ".");
			} else if (ss.str() == "") {
				line->insertToken(insertionField, ".");
			} else {
				line->insertToken(insertionField, ss.str());
			}
		} else{
			cerr << "!!strange error for line " << i+1 << ":\t" << line << endl;
		}
		if (recalcLine) {
			line->createLineFromTokens();
		}
	}
	return true;
}



#ifndef HUMDRUMFILE_PARENT
	#define HUMDRUMFILE_PARENT HumdrumFileContent
#endif

class HumdrumFile : public HUMDRUMFILE_PARENT {
	public:
		              HumdrumFile          (void);
		              HumdrumFile          (const string& filename);
		              HumdrumFile          (istream& filename);
		             ~HumdrumFile          ();

		ostream&      printXml             (ostream& out = cout, int level = 0,
		                                    const string& indent = "\t");
		ostream&      printXmlParameterInfo(ostream& out, int level,
		                                    const string& indent);
};



#define GRIDREST NAN

class NoteGrid;


class NoteCell {
	public:
		       NoteCell             (NoteGrid* owner, HTp token);
		      ~NoteCell             (void) { clear();                    }

		double getSgnDiatonicPitch  (void) { return m_b7;                }
		double getSgnMidiPitch      (void) { return m_b12;               }
		double getSgnBase40Pitch    (void) { return m_b40;               }
		double getSgnAccidental     (void) { return m_accidental;        }

		double getSgnDiatonicPitchClass(void);
		double getAbsDiatonicPitchClass(void);

		double getSgnBase40PitchClass(void);
		double getAbsBase40PitchClass(void);

		double getAbsDiatonicPitch  (void) { return fabs(m_b7);          }
		double getAbsMidiPitch      (void) { return fabs(m_b12);         }
		double getAbsBase40Pitch    (void) { return fabs(m_b40);         }
		double getAbsAccidental     (void) { return fabs(m_accidental);  }

		HTp    getToken             (void) { return m_token;             }
		int    getNextAttackIndex   (void) { return m_nextAttackIndex;   }
		int    getPrevAttackIndex   (void) { return m_prevAttackIndex;   }
		int    getCurrAttackIndex   (void) { return m_currAttackIndex;   }
		int    getSliceIndex        (void) { return m_timeslice;         }
		int    getVoiceIndex        (void) { return m_voice;             }

		bool   isAttack             (void) { return m_b40>0? true:false; }
		bool   isRest               (void);
		bool   isSustained          (void);

		string getAbsKernPitch      (void);
		string getSgnKernPitch      (void);

		double operator-            (NoteCell& B);
		double operator-            (int B);

		int    getLineIndex         (void);
		int    getFieldIndex        (void);
		ostream& printNoteInfo      (ostream& out);
		double getDiatonicIntervalToNextAttack      (void);
		double getDiatonicIntervalFromPreviousAttack(void);
		double getMetricLevel       (void);
		HumNum getDurationFromStart (void);
		HumNum getDuration          (void);
		void   setMeter             (int topval, HumNum botval);
		int    getMeterTop          (void);
		HumNum getMeterBottom       (void);

		vector<HTp> m_tiedtokens;	// list of tied notes/rests after note attack

	protected:
		void clear                  (void);
		void calculateNumericPitches(void);
		void setVoiceIndex          (int index) { m_voice = index;           }
		void setSliceIndex          (int index) { m_timeslice = index;       }
		void setNextAttackIndex     (int index) { m_nextAttackIndex = index; }
		void setPrevAttackIndex     (int index) { m_prevAttackIndex = index; }
		void setCurrAttackIndex     (int index) { m_currAttackIndex = index; }

	private:
		NoteGrid* m_owner; // the NoteGrid to which this cell belongs.
		HTp m_token;       // pointer to the note in the origina Humdrum file.
		int m_voice;       // index of the voice in the score the note belongs
		                   // 0=bottom voice (HumdrumFile ordering of parts)
		                   // column in NoteGrid.
		int m_timeslice;   // index for the row in NoteGrid.

		double m_b7;         // diatonic note number; NaN=rest; negative=sustain.
		double m_b12;        // MIDI note number; NaN=rest; negative=sustain.
		double m_b40;        // base-40 note number; NaN=rest; negative=sustain.
		double m_accidental; // chromatic alteration of a diatonic pitch.
		                     // NaN=no accidental.
		int m_nextAttackIndex; // index to next note attack (or rest),
		                       // -1 for undefined (interpred as rest).
		int m_prevAttackIndex; // index to previous note attack.
		int m_currAttackIndex; // index to current note attack (useful for
		                       // finding the start of a sustained note.
		int m_metertop = 0;    // top number of prevailing meter signature
		HumNum m_meterbot = 0; // bottom number of prevailing meter signature

	friend NoteGrid;
};



class NoteGrid {
	public:
		           NoteGrid              (void) { }
		           NoteGrid              (HumdrumFile& infile);
		          ~NoteGrid              ();

		void       clear                 (void);

		bool       load                  (HumdrumFile& infile);
		NoteCell*  cell                  (int voiceindex, int sliceindex);
		int        getVoiceCount         (void);
		int        getSliceCount         (void);
		int        getLineIndex          (int sindex);
		int        getFieldIndex         (int sindex);

		void       printDiatonicGrid     (ostream& out);
		void       printMidiGrid         (ostream& out);
		void       printBase40Grid       (ostream& out);
		void       printRawGrid          (ostream& out);
		void       printKernGrid         (ostream& out);

		double     getSgnDiatonicPitch   (int vindex, int sindex);
		double     getSgnMidiPitch       (int vindex, int sindex);
		double     getSgnBase40Pitch     (int vindex, int sindex);
		string     getSgnKernPitch       (int vindex, int sindex);

		double     getAbsDiatonicPitch   (int vindex, int sindex);
		double     getAbsMidiPitch       (int vindex, int sindex);
		double     getAbsBase40Pitch     (int vindex, int sindex);
		string     getAbsKernPitch       (int vindex, int sindex);

		bool       isRest                (int vindex, int sindex);
		bool       isSustained           (int vindex, int sindex);
		bool       isAttack              (int vindex, int sindex);

		HTp        getToken              (int vindex, int sindex);

		int        getPrevAttackDiatonic (int vindex, int sindex);
		int        getNextAttackDiatonic (int vindex, int sindex);

		void       printGridInfo         (ostream& out);
		void       printVoiceInfo        (ostream& out, int vindex);

		void       getNoteAndRestAttacks (vector<NoteCell*>& attacks, int vindex);
		double     getMetricLevel        (int sindex);
		HumNum     getNoteDuration       (int vindex, int sindex);

	protected:
		void       buildAttackIndexes    (void);
		void       buildAttackIndex      (int vindex);

	private:
		vector<vector<NoteCell*> > m_grid;
		vector<HTp>                m_kernspines;
		vector<double>             m_metriclevels;
		HumdrumFile*               m_infile;
};



class Convert {
	public:

		// Rhythm processing, defined in Convert-rhythm.cpp
		static HumNum  recipToDuration      (const string& recip,
		                                     HumNum scale = 4,
		                                     const string& separator = " ");
		static HumNum  recipToDurationNoDots(const string& recip,
		                                     HumNum scale = 4,
		                                     const string& separator = " ");
		static HumNum  recipToDuration      (string* recip,
		                                     HumNum scale = 4,
		                                     const string& separator = " ");
		static HumNum  recipToDurationNoDots(string* recip,
		                                     HumNum scale = 4,
		                                     const string& separator = " ");
		static string  durationToRecip      (HumNum duration,
		                                     HumNum scale = HumNum(1,4));
		static string  durationFloatToRecip (double duration,
		                                     HumNum scale = HumNum(1,4));

		// Pitch processing, defined in Convert-pitch.cpp
		static string  base40ToKern         (int b40);
		static int     base40ToAccidental   (int b40);
		static int     base40ToDiatonic     (int b40);
		static int     base40ToMidiNoteNumber(int b40);
		static string  base40ToIntervalAbbr (int b40);
		static int     kernToOctaveNumber   (const string& kerndata);
		static int     kernToOctaveNumber   (HTp token)
				{ return kernToOctaveNumber((string)*token); }
		static int     kernToAccidentalCount(const string& kerndata);
		static int     kernToAccidentalCount(HTp token)
				{ return kernToAccidentalCount((string)*token); }
		static int     kernToDiatonicPC     (const string& kerndata);
		static int     kernToDiatonicPC     (HTp token)
				{ return kernToDiatonicPC     ((string)*token); }
		static char    kernToDiatonicUC     (const string& kerndata);
		static int     kernToDiatonicUC     (HTp token)
				{ return kernToDiatonicUC     ((string)*token); }
		static char    kernToDiatonicLC     (const string& kerndata);
		static int     kernToDiatonicLC     (HTp token)
				{ return kernToDiatonicLC     ((string)*token); }
		static int     kernToBase40PC       (const string& kerndata);
		static int     kernToBase40PC       (HTp token)
				{ return kernToBase40PC       ((string)*token); }
		static int     kernToBase12PC       (const string& kerndata);
		static int     kernToBase12PC       (HTp token)
				{ return kernToBase12PC       ((string)*token); }
		static int     kernToBase7PC        (const string& kerndata) {
		                                     return kernToDiatonicPC(kerndata); }
		static int     kernToBase7PC        (HTp token)
				{ return kernToBase7PC        ((string)*token); }
		static int     kernToBase40         (const string& kerndata);
		static int     kernToBase40         (HTp token)
				{ return kernToBase40         ((string)*token); }
		static int     kernToBase12         (const string& kerndata);
		static int     kernToBase12         (HTp token)
				{ return kernToBase12         ((string)*token); }
		static int     kernToBase7          (const string& kerndata);
		static int     kernToBase7          (HTp token)
				{ return kernToBase7          ((string)*token); }
		static int     kernToMidiNoteNumber (const string& kerndata);
		static int     kernToMidiNoteNumber(HTp token)
				{ return kernToMidiNoteNumber((string)*token); }
		static string  kernToScientificPitch(const string& kerndata,
		                                     string flat = "b",
		                                     string sharp = "#",
		                                     string separator = "");
		static string  kernToSciPitch       (const string& kerndata,
		      										 string flat = "b",
		                                     string sharp = "#",
		                                     string separator = "")
	       { return kernToScientificPitch(kerndata, flat, sharp, separator); }
		static string  kernToSP             (const string& kerndata,
		                                     string flat = "b",
		                                     string sharp = "#",
		                                     string separator = "")
		      { return kernToScientificPitch(kerndata, flat, sharp, separator); }
		static int     pitchToWbh           (int dpc, int acc, int octave,
		                                     int maxacc);
		static void    wbhToPitch           (int& dpc, int& acc, int& octave,
		                                     int maxacc, int wbh);
		static int     kernClefToBaseline   (const string& input);
		static string  base40ToTrans        (int base40);
		static int     transToBase40        (const string& input);
		static int     base40IntervalToLineOfFifths(int trans);
		static string  keyNumberToKern      (int number);
		static int     base7ToBase40        (int base7);
		static int     base40IntervalToDiatonic(int base40interval);

		// Harmony processing, defined in Convert-harmony.cpp
		static vector<int> minorHScaleBase40(void);
		static vector<int> majorScaleBase40 (void);
		static int         keyToInversion   (const string& harm);
		static int         keyToBase40      (const string& key);
		static vector<int> harmToBase40     (HTp harm, const string& key) {
		                                        return harmToBase40(*harm, key); }
		static vector<int> harmToBase40     (HTp harm, HTp key) {
		                                        return harmToBase40(*harm, *key); }
		static vector<int> harmToBase40     (const string& harm, const string& key);
		static vector<int> harmToBase40     (const string& harm, int keyroot, int keymode);
		static void        makeAdjustedKeyRootAndMode(const string& secondary,
		                                     int& keyroot, int& keymode);
		static int         chromaticAlteration(const string& content);

		// data-type specific (other than pitch/rhythm),
		// defined in Convert-kern.cpp
		static bool isKernRest              (const string& kerndata);
		static bool isKernNote              (const string& kerndata);
		static bool isKernNoteAttack        (const string& kerndata);
		static bool hasKernSlurStart        (const string& kerndata);
		static bool hasKernSlurEnd          (const string& kerndata);
		static int  getKernSlurStartElisionLevel(const string& kerndata, int index);
		static int  getKernSlurEndElisionLevel  (const string& kerndata, int index);

		static bool isKernSecondaryTiedNote (const string& kerndata);
		static string getKernPitchAttributes(const string& kerndata);

		// String processing, defined in Convert-string.cpp
		static vector<string> splitString   (const string& data,
		                                     char separator = ' ');
		static void    replaceOccurrences   (string& source,
		                                     const string& search,
		                                     const string& replace);
		static string  repeatString         (const string& pattern, int count);
		static string  encodeXml            (const string& input);
		static string  getHumNumAttributes  (const HumNum& num);
		static string  trimWhiteSpace       (const string& input);
		static bool    startsWith           (const string& input,
		                                     const string& searchstring);
		static bool    contains(const string& input, const string& pattern);
		static bool    contains(const string& input, char pattern);
		static bool    contains(string* input, const string& pattern);
		static bool    contains(string* input, char pattern);
		static void    makeBooleanTrackList(vector<bool>& spinelist,
		                                     const string& spinestring,
		                                     int maxtrack);

		// Mathematical processing, defined in Convert-math.cpp
		static int     getLcm               (const vector<int>& numbers);
		static int     getGcd               (int a, int b);
		static void    primeFactors         (vector<int>& output, int n);
		static double  nearIntQuantize      (double value,
		                                    double delta = 0.00001);
		static double  significantDigits    (double value, int digits);
		static bool    isNaN                (double value);
		static double  pearsonCorrelation   (vector<double> x, vector<double> y);
		static int     romanNumeralToInteger(const string& roman);

};



// SliceType is a list of various Humdrum line types.  Groupings are 
// segmented by categories which are prefixed with an underscore.
// For example Notes are in the _Duration group, since they have 
// non-zero durations.  Notes and Gracenotes are in the _Data group.
// The indentation shows the various types of groups.
// 

enum class SliceType {
				Notes = 1,
			_Duration,
				GraceNotes,
		_Data,
			Measures,
		_Measure,
				Clefs,
				Transpositions,
				KeySigs,
				TimeSigs,
				MeterSigs,
				Tempos,
				Labels,
				LabelAbbrs,
			_RegularInterpretation,
				Exclusives,
				Terminators,
				Manipulators,
			_Manipulator,
		_Interpretation,
			Layouts,
			LocalComments,
	_Spined,
		GlobalComments,
		GlobalLayouts,
		ReferenceRecords,
	_Other,
		Invalid
};


// MeasureType is a list of the style types for a measure (ending type for now)

enum class MeasureStyle {
	Plain,
	RepeatBackward,
	RepeatForward,
	RepeatBoth,
	Double,
	Final
};



class MxmlMeasure;
class MxmlEvent;

class MxmlPart {
	public:
		              MxmlPart             (void);
		             ~MxmlPart             ();
		void          clear                (void);
		void          enableStems          (void);
		bool          readPart             (const string& id, xml_node partdef, 
		                                    xml_node part);
		bool          addMeasure           (xml_node mel);
		bool          addMeasure           (xpath_node mel);
		int           getMeasureCount      (void) const;
		MxmlMeasure*  getMeasure           (int index) const;
		long          getQTicks            (void) const;
		int           setQTicks            (long value);
	   MxmlMeasure*  getPreviousMeasure   (MxmlMeasure* measure) const;
		HumNum        getDuration          (void) const;
		void          allocateSortedEvents (void);
		void          setPartNumber        (int number);
		int           getPartNumber        (void) const;
		int           getPartIndex         (void) const;
		int           getStaffCount        (void) const;
		int           getVerseCount        (void) const;
		int           getVerseCount        (int staffindex) const;
		int           getHarmonyCount      (void) const;
		void          trackStaffVoices     (int staffnum, int voicenum);
		void          printStaffVoiceInfo  (void);
		void          prepareVoiceMapping  (void);
		int           getVoiceIndex        (int voicenum);
		int           getStaffIndex        (int voicenum);
		bool          hasEditorialAccidental(void) const;
		bool          hasDynamics          (void) const;
		void          parsePartInfo        (xml_node partdeclaration);
		string        getPartName          (void) const;
		string        getPartAbbr          (void) const;
		string        cleanSpaces          (const string& input);


	private:
		void          receiveStaffNumberFromChild (int staffnum, int voicenum);
		void          receiveVerseCount           (int count);
		void          receiveVerseCount           (int staffnum, int count);
		void          receiveHarmonyCount         (int count);
		void          receiveEditorialAccidental  (void);
		void          receiveDynamic              (void);

	protected:
		vector<MxmlMeasure*> m_measures;
		vector<long>         m_qtick;
		int                  m_partnum;
		int                  m_maxstaff;
		vector<int>          m_verseCount;
		int                  m_harmonyCount;
		bool                 m_editorialAccidental;
		bool                 m_stems = false;
		bool                 m_has_dynamics = false;
		string               m_partname;
		string               m_partabbr;

		// m_staffvoicehist: counts of staff and voice numbers.  
		// staff=0 is used for items such as measures.
		// voice=0 is used for nonduration items such as harmony.
		vector<vector<int> > m_staffvoicehist;
	 	vector<pair<int, int> > m_voicemapping; // voicenum -> (staff, voiceindex)
		
	friend MxmlMeasure;
	friend MxmlEvent;

};



class GridSide {
	public:
		GridSide(void);
		~GridSide();

		int   getVerseCount     (void);
		HTp   getVerse          (int index);
		void  setVerse          (int index, HTp token);
		void  setVerse          (int index, const string& token);

		int   getHarmonyCount   (void);
		void  setHarmony        (HTp token);
		void  detachHarmony     (void);
		HTp   getHarmony        (void);

		int   getDynamicsCount  (void);
		void  setDynamics       (HTp token);
		void  setDynamics       (const string& token);
		void  detachDynamics    (void);
		HTp   getDynamics       (void);

	private:
		vector<HumdrumToken*> m_verses;
		HumdrumToken* m_dynamics = NULL;
		HumdrumToken* m_harmony = NULL;
};

ostream& operator<<(ostream& output, GridSide* side);


class GridStaff : public vector<GridVoice*>, public GridSide {
	public:
		GridStaff(void);
		~GridStaff();
		GridVoice* setTokenLayer (int layerindex, HTp token, HumNum duration);
		void setNullTokenLayer   (int layerindex, SliceType type, HumNum nextdur);
		void appendTokenLayer    (int layerindex, HTp token, HumNum duration,
		                          const string& spacer = " ");
		int getMaxVerseCount     (void);
};

ostream& operator<<(ostream& output, GridStaff* staff);



class GridPart : public vector<GridStaff*>, public GridSide {
	public:
		GridPart(void);
		~GridPart();
};

ostream& operator<<(ostream& output, GridPart* part);
ostream& operator<<(ostream& output, GridPart& part);



class GridSlice;
class HumGrid;

class GridMeasure : public list<GridSlice*> {
	public:
		GridMeasure(HumGrid* owner);
		~GridMeasure();

		GridSlice*   addTempoToken  (const string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addTimeSigToken(const string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addKeySigToken (const string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addClefToken   (const string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addTransposeToken(const string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addLabelToken  (const string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxpart,
		                             int maxstaff);
		GridSlice*   addLabelAbbrToken(const string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxpart,
		                             int maxstaff);
		GridSlice*   addDataToken   (const string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addGraceToken  (const string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff,
		                             int gracenumber);
		GridSlice*   addGlobalLayout(const string& tok, HumNum timestamp);
		GridSlice*   addGlobalComment(const string& tok, HumNum timestamp);
		GridSlice*   appendGlobalLayout(const string& tok, HumNum timestamp);
		bool         transferTokens (HumdrumFile& outfile, bool recip,
		                             bool addbar, int startbarnum = 0);
		HumGrid*     getOwner       (void);
		void         setOwner       (HumGrid* owner);
		HumNum       getDuration    (void);
		void         setDuration    (HumNum duration);
		HumNum       getTimestamp   (void);
		void         setTimestamp   (HumNum timestamp);
		HumNum       getTimeSigDur  (void);
		void         setTimeSigDur  (HumNum duration);
		MeasureStyle getStyle       (void) { return m_style; }
		MeasureStyle getBarStyle    (void) { return getStyle(); }
		void         setStyle       (MeasureStyle style) { m_style = style; }
		void         setBarStyle    (MeasureStyle style) { setStyle(style); }
		void         setFinalBarlineStyle(void) { setStyle(MeasureStyle::Final); }
		void         setRepeatEndStyle(void) { setStyle(MeasureStyle::RepeatBackward); }
		void         setRepeatBackwardStyle(void) { setStyle(MeasureStyle::RepeatBackward); }

		bool         isDouble(void) 
		                  {return m_style == MeasureStyle::Double;}
		bool         isFinal(void) 
		                  {return m_style == MeasureStyle::Final;}
		bool         isRepeatBackward(void) 
		                  { return m_style == MeasureStyle::RepeatBackward; }
		bool         isRepeatForward(void) 
		                  { return m_style == MeasureStyle::RepeatForward; }
		bool         isRepeatBoth(void) 
		                  { return m_style == MeasureStyle::RepeatBoth; }
		void         addLayoutParameter(GridSlice* slice, int partindex, const string& locomment);
		void         addDynamicsLayoutParameters(GridSlice* slice, int partindex, const string& locomment);
		bool         isInvisible(void);
		bool         isSingleChordMeasure(void);
		bool         isMonophonicMeasure(void);
		GridSlice*   getLastSpinedSlice(void);
		GridSlice*   getFirstSpinedSlice(void);
	
	protected:
		void         appendInitialBarline(HumdrumFile& infile, int startbarnum = 0);

	private:
		HumGrid*     m_owner;
		HumNum       m_duration;
		HumNum       m_timestamp;
		HumNum       m_timesigdur;
		MeasureStyle m_style;
};

ostream& operator<<(ostream& output, GridMeasure& measure);
ostream& operator<<(ostream& output, GridMeasure* measure);


class HumGrid;


class GridSlice : public vector<GridPart*> {
	public:
		GridSlice(GridMeasure* measure, HumNum timestamp, SliceType type,
		          int partcount = 0);
		GridSlice(GridMeasure* measure, HumNum timestamp, SliceType type,
		          const GridSlice& slice);
		GridSlice(GridMeasure* measure, HumNum timestamp, SliceType type,
		          GridSlice* slice);
		~GridSlice();

		bool isNoteSlice(void)        { return m_type == SliceType::Notes;            }
		bool isGraceSlice(void)       { return m_type == SliceType::GraceNotes;       }
		bool isMeasureSlice(void)     { return m_type == SliceType::Measures;         }
		bool isClefSlice(void)        { return m_type == SliceType::Clefs;            }
		bool isLabelSlice(void)       { return m_type == SliceType::Labels;           }
		bool isLabelAbbrSlice(void)   { return m_type == SliceType::LabelAbbrs;       }
		bool isTransposeSlice(void)   { return m_type == SliceType::Transpositions;   }
		bool isKeySigSlice(void)      { return m_type == SliceType::KeySigs;          }
		bool isTimeSigSlice(void)     { return m_type == SliceType::TimeSigs;         }
		bool isTempoSlice(void)       { return m_type == SliceType::Tempos;           }
		bool isMeterSigSlice(void)    { return m_type == SliceType::MeterSigs;        }
		bool isManipulatorSlice(void) { return m_type==SliceType::Manipulators;       }
		bool isLayoutSlice(void)      { return m_type ==  SliceType::Layouts;         }
		bool isLocalLayoutSlice(void) { return m_type ==  SliceType::Layouts;         }
		bool isInvalidSlice(void)     { return m_type == SliceType::Invalid;          }
		bool isGlobalComment(void)    { return m_type == SliceType::GlobalComments;   }
		bool isGlobalLayout(void)     { return m_type == SliceType::GlobalLayouts;    }
		bool isReferenceRecord(void)  { return m_type == SliceType::ReferenceRecords; }
		bool isInterpretationSlice(void);
		bool isDataSlice(void);
		bool hasSpines(void);
		SliceType getType(void)    { return m_type; }

		void transferTokens        (HumdrumFile& outfile, bool recip);
		void initializePartStaves  (vector<MxmlPart>& partdata);
		void initializeBySlice     (GridSlice* slice);
		void initializeByStaffCount(int staffcount);

		HumNum       getDuration        (void);
		void         setDuration        (HumNum duration);
		HumNum       getTimestamp       (void);
		void         setTimestamp       (HumNum timestamp);
		void         setOwner           (HumGrid* owner);
		HumGrid*     getOwner           (void);
		HumNum       getMeasureDuration (void);
		HumNum       getMeasureTimestamp(void);
		GridMeasure* getMeasure         (void);
		void         invalidate         (void);

		void transferSides        (HumdrumLine& line, GridStaff& sides, 
		                           const string& empty, int maxvcount,
		                           int maxhcount);
		void transferSides        (HumdrumLine& line, GridPart& sides, 
		                           int partindex, const string& empty,
		                           int maxvcount, int maxhcount,
		                           int maxdcount);
		int getVerseCount         (int partindex, int staffindex);
		int getHarmonyCount       (int partindex, int staffindex = -1);
		int getDynamicsCount      (int partindex, int staffindex = -1);
		void addToken             (const string& tok, int parti, int staffi, int voicei);

	protected:
		HTp  createRecipTokenFromDuration  (HumNum duration);

	private:
		HumGrid*     m_owner;
		GridMeasure* m_measure;
		HumNum       m_timestamp;
		HumNum       m_duration;
		SliceType    m_type;

};


ostream& operator<<(ostream& output, GridSlice* slice);



class GridVoice {
	public:
		GridVoice(void);
		GridVoice(HTp token, HumNum duration);
		GridVoice(const char* token, HumNum duration);
		GridVoice(const string& token, HumNum duration);
		~GridVoice();

		bool   isTransfered       (void);

		HTp    getToken           (void) const;
		void   setToken           (HTp token);
		void   setToken           (const string& token);
		void   setToken           (const char* token);
		bool   isNull             (void) const;

		void   setDuration        (HumNum duration);
		HumNum getDuration        (void) const;
		HumNum getDurationToNext  (void) const;
		HumNum getDurationToPrev  (void) const;
		void   setDurationToPrev  (HumNum dur);
		void   incrementDuration  (HumNum duration);
		void   forgetToken        (void);

	protected:
		void   setTransfered      (bool state);

	private:
		HTp    m_token;
		HumNum m_nextdur;
		HumNum m_prevdur;
		bool   m_transfered;

	friend class GridSlice;
};

ostream& operator<<(ostream& output, GridVoice* voice);
ostream& operator<<(ostream& output, GridVoice& voice);



class HumGrid : public vector<GridMeasure*> {
	public:
		HumGrid(void);
		~HumGrid();
		void enableRecipSpine           (void);
		bool transferTokens             (HumdrumFile& outfile, int startbarnum = 0);
		int  getHarmonyCount            (int partindex);
		int  getDynamicsCount           (int partindex);
		int  getVerseCount              (int partindex, int staffindex);
		bool hasDynamics                (int partindex);
		void setDynamicsPresent         (int partindex);
		void setVerseCount              (int partindex, int staffindex, int count);
		void setHarmonyCount            (int partindex, int count);
		void removeRedundantClefChanges (void);
		void removeSibeliusIncipit      (void);
		bool hasPickup                  (void);
		GridMeasure*  addMeasureToBack  (void);
		int  getPartCount               (void);
		int  getStaffCount              (int partindex);
		void deleteMeasure              (int index);

	protected:
		void calculateGridDurations        (void);
		void insertExclusiveInterpretationLine (HumdrumFile& outfile);
		void insertDataTerminationLine     (HumdrumFile& outfile);
		void appendMeasureLine             (HumdrumFile& outfile,
		                                    GridSlice& slice);
		void insertPartIndications         (HumdrumFile& outfile);
		void insertStaffIndications        (HumdrumFile& outfile);
		void addNullTokens                 (void);
		void addNullTokensForGraceNotes    (void);
		void addNullTokensForClefChanges   (void);
		void addNullTokensForLayoutComments(void);

		void FillInNullTokensForGraceNotes(GridSlice* graceslice, GridSlice* lastnote,
		                                   GridSlice* nextnote);
		void FillInNullTokensForLayoutComments(GridSlice* layoutslice, GridSlice* lastnote,
		                                   GridSlice* nextnote);
		void FillInNullTokensForClefChanges (GridSlice* clefslice,
		                                    GridSlice* lastnote, GridSlice* nextnote);
		void adjustClefChanges             (void);
		bool buildSingleList               (void);
		void extendDurationToken           (int slicei, int parti,
		                                    int staffi, int voicei);
		GridVoice* getGridVoice(int slicei, int parti, int staffi, int voicei);
		void addMeasureLines               (void);
		void addLastMeasure                (void);
		bool manipulatorCheck              (void);
		GridSlice* manipulatorCheck        (GridSlice* ice1, GridSlice* ice2);
		void cleanupManipulators           (void);
		void cleanManipulator              (vector<GridSlice*>& newslices,
		                                    GridSlice* curr);
		GridSlice* checkManipulatorExpand  (GridSlice* curr);
		GridSlice* checkManipulatorContract(GridSlice* curr);
		void transferMerges                (GridStaff* oldstaff,
		                                    GridStaff* oldlaststaff,
		                                    GridStaff* newstaff,
		                                    GridStaff* newlaststaff);
		void insertExInterpSides           (HumdrumLine* line, int part,
		                                    int staff);
		void insertSideTerminals           (HumdrumLine* line, int part,
		                                    int staff);
		void insertSidePartInfo            (HumdrumLine* line, int part,
		                                    int staff);
		void insertSideStaffInfo           (HumdrumLine* line, int part,
		                                    int staff, int staffnum);
		void getMetricBarNumbers           (vector<int>& barnums);
		string  createBarToken             (int m, int barnum,
		                                    GridMeasure* measure);
		string getBarStyle                 (GridMeasure* measure);
		void adjustExpansionsInStaff       (GridSlice* newmanip, GridSlice* curr,
		                                    int p, int s);
		void transferNonDataSlices         (GridMeasure* output, GridMeasure* input);
		string extractMelody               (GridMeasure* measure);
		void insertMelodyString            (GridMeasure* measure, const string& melody);

		GridSlice* getNextSpinedLine       (const GridMeasure::iterator& it, int measureindex);

	private:
		vector<GridSlice*>   m_allslices;
		vector<vector<int> > m_verseCount;
		vector<int>          m_harmonyCount;
		bool                 m_pickup;
		vector<bool>         m_dynamics;

		// options:
		bool m_recip;               // include **recip spine in output
		bool m_musicxmlbarlines;    // use measure numbers from <measure> element

};



class MxmlMeasure;
class MxmlPart;

// Event types: These are all of the XML elements which can be children of
// the measure element in MusicXML.

enum measure_event_type {
	mevent_unknown,
	mevent_attributes,
	mevent_backup,
	mevent_barline,
	mevent_bookmark,
	mevent_direction,
	mevent_figured_bass,
	mevent_forward,
	mevent_grouping,
	mevent_harmony,
	mevent_link,
	mevent_note,
	mevent_print,
	mevent_sound,
	mevent_float       // category for GridSides not attached to note onsets
};


class MxmlEvent {
	public:
		                   MxmlEvent          (MxmlMeasure* measure);
		                  ~MxmlEvent          ();
		void               clear              (void);
		void               enableStems        (void);
		bool               parseEvent         (xml_node el, xml_node nextel,
		                                       HumNum starttime);
		bool               parseEvent         (xpath_node el, HumNum starttime);
		void               setTickStart       (long value, long ticks);
		void               setTickDur         (long value, long ticks);
		void               setStartTime       (HumNum value);
		void               setDuration        (HumNum value);
		void               setDurationByTicks (long value,
		                                       xml_node el = xml_node(NULL));
		HumNum             getStartTime       (void) const;
		HumNum             getDuration        (void) const;
		void               setOwner           (MxmlMeasure* measure);
		MxmlMeasure*       getOwner           (void) const;
		const char*        getName            (void) const;
		int                setQTicks          (long value);
		long               getQTicks          (void) const;
		long               getIntValue        (const char* query) const;
		bool               hasChild           (const char* query) const;
		void               link               (MxmlEvent* event);
		bool               isLinked           (void) const;
		bool               isRest             (void);
		bool               isGrace            (void);
		bool               isFloating         (void);
		bool               hasSlurStart       (int& direction);
		bool               hasSlurStop        (void);
		void               setLinked          (void);
		vector<MxmlEvent*> getLinkedNotes     (void);
		void               attachToLastEvent  (void);
		bool               isChord            (void) const;
		void               printEvent         (void);
		int                getSequenceNumber  (void) const;
		int                getVoiceNumber     (void) const;
		void               setVoiceNumber     (int value);
		int                getStaffNumber     (void) const;
		int                getStaffIndex      (void) const;
		int                getVoiceIndex      (int maxvoice = 4) const;
		void               setStaffNumber     (int value);
		measure_event_type getType            (void) const;
		int                getPartNumber      (void) const;
		int                getPartIndex       (void) const;
		string             getRecip           (void) const;
		string             getKernPitch       (void);
		string             getPrefixNoteInfo  (void) const;
		string             getPostfixNoteInfo (bool primarynote) const;
		xml_node           getNode            (void);
		xml_node           getHNode           (void);
		HumNum             getTimeSigDur      (void);
		string             getElementName     (void);
		void               addNotations       (stringstream& ss, 
		                                       xml_node notations) const;
		void               reportVerseCountToOwner         (int count);
		void               reportVerseCountToOwner         (int staffnum, int count);
		void               reportHarmonyCountToOwner       (int count);
		void               reportMeasureStyleToOwner       (MeasureStyle style);
		void               reportEditorialAccidentalToOwner(void);
		void               reportDynamicToOwner            (void);
      void               makeDummyRest      (MxmlMeasure* owner, 
		                                       HumNum startime,
		                                       HumNum duration,
		                                       int staffindex = 0,
		                                       int voiceindex = 0);
		void               setVoiceIndex      (int voiceindex);
		void               forceInvisible     (void);
		bool               isInvisible        (void);
		void               setBarlineStyle    (xml_node node);
		void               setTexts           (vector<xml_node>& nodes);
		vector<xml_node>&  getTexts           (void);
		void               setDynamics        (xml_node node);
		xml_node           getDynamics        (void);

	protected:
		HumNum             m_starttime;  // start time in quarter notes of event
		HumNum             m_duration;   // duration in quarter notes of event
		measure_event_type m_eventtype;  // enumeration type of event
		xml_node           m_node;       // pointer to event in XML structure
		MxmlMeasure*       m_owner;      // measure that contains this event
		vector<MxmlEvent*> m_links;      // list of secondary chord notes
		bool               m_linked;     // true if a secondary chord note
		int                m_sequence;   // ordering of event in XML file
		static int         m_counter;    // counter for sequence variable
		short              m_staff;      // staff number in part for event
		short              m_voice;      // voice number in part for event
		int                m_voiceindex; // voice index of item (remapping)
      int                m_maxstaff;   // maximum staff number for measure
		xml_node           m_hnode;      // harmony label starting at note event
		bool               m_invisible;  // for forceInvisible();
		bool               m_stems;      // for preserving stems

		xml_node          m_dynamics;    // dynamics <direction> starting just before note
		vector<xml_node>  m_text;        // text <direction> starting just before note

	private:
   	void   reportStaffNumberToOwner  (int staffnum, int voicenum);
		void   reportTimeSigDurToOwner   (HumNum duration);
		int    getDotCount               (void) const;

	public:
		static HumNum getEmbeddedDuration  (xml_node el = xml_node(NULL));
		static HumNum getQuarterDurationFromType (const char* type);
		static bool   nodeType             (xml_node node, const char* testname);


	friend MxmlMeasure;
	friend MxmlPart;
};



class MxmlEvent;
class MxmlPart;


class SimultaneousEvents {
	public:
		SimultaneousEvents(void) { }
		~SimultaneousEvents() { }
		HumNum starttime;              // start time of events
		HumNum duration;               // duration to next non-zero duration
		vector<MxmlEvent*> zerodur;    // zero-duration elements at this time
		vector<MxmlEvent*> nonzerodur; // non-zero dur elements at this time
};


class MxmlMeasure {
	public:
		              MxmlMeasure        (MxmlPart* part);
		             ~MxmlMeasure        (void);
		void          clear              (void);
		void          enableStems        (void);
		bool          parseMeasure       (xml_node mel);
		bool          parseMeasure       (xpath_node mel);
		void          setStartTimeOfMeasure (HumNum value);
		void          setStartTimeOfMeasure (void);
		void          setDuration        (HumNum value);
		HumNum        getStartTime       (void) const;
		HumNum        getTimestamp       (void) const { return getStartTime(); }
		HumNum        getDuration        (void) const;
		void          setOwner           (MxmlPart* part);
		MxmlPart*     getOwner           (void) const;
		int           getPartNumber      (void) const;
		int           getPartIndex       (void) const;
		int           setQTicks          (long value);
		long          getQTicks          (void) const;
		void          attachLastEventToPrevious  (void);
		void          calculateDuration  (void);
		int           getEventCount      (void) const;
		vector<SimultaneousEvents>* getSortedEvents(void);
		MxmlEvent*    getEvent           (int index) const;

		void          setNextMeasure     (MxmlMeasure* event);
		MxmlMeasure*  getNextMeasure     (void) const;
		MxmlMeasure*  getPreviousMeasure (void) const;
		void          setPreviousMeasure (MxmlMeasure* event);

		int           getVoiceIndex      (int voicenum);
		int           getStaffIndex      (int voicenum);
		void          setTimeSigDur      (HumNum duration);
		HumNum        getTimeSigDur      (void);
		void          addDummyRest       (void);
		void          addDummyRest       (HumNum starttime, HumNum duration, 
		                                  int staffindex, int voiceindex);
		vector<MxmlEvent*>& getEventList (void);
		void  sortEvents                 (void);
		void  forceLastInvisible         (void);
		MeasureStyle  getStyle           (void);
		MeasureStyle  getBarStyle        (void);
		void  setStyle                   (MeasureStyle style);
		void  setBarStyle                (MeasureStyle style);
		void  makeFinalBarline(void)   { m_style = MeasureStyle::Final; }
		bool  isFinal(void)            { return m_style == MeasureStyle::Final; }
		bool  isDouble(void)           { return m_style == MeasureStyle::Double; }
		bool  isRepeatBackward(void)   { return m_style == MeasureStyle::RepeatBackward; }
		bool  isRepeatForward(void)    { return m_style == MeasureStyle::RepeatForward; }
		bool  isRepeatBoth(void)       { return m_style == MeasureStyle::RepeatBoth; }

	private:
		void  receiveStaffNumberFromChild         (int staffnum, int voicenum);
		void  receiveTimeSigDurFromChild          (HumNum duration);
		void  receiveMeasureStyleFromChild        (MeasureStyle style);
		void  receiveEditorialAccidentalFromChild (void);
   	void  reportStaffNumberToOwner            (int staffnum, int voicenum);
		void  reportVerseCountToOwner             (int count);
		void  reportVerseCountToOwner             (int staffindex, int count);
		void  reportHarmonyCountToOwner           (int count);
		void  reportEditorialAccidentalToOwner    (void);
		void  reportDynamicToOwner                (void);

	protected:
		HumNum             m_starttime; // start time of measure in quarter notes
		HumNum             m_duration;  // duration of measure in quarter notes
		HumNum             m_timesigdur; // duration of measure according to 
													// prevailing time signature.
		MxmlPart*          m_owner;     // part which contains measure
		MxmlMeasure*       m_previous;  // previous measure in part or null
		MxmlMeasure*       m_following; // following measure in part or null
		vector<MxmlEvent*> m_events;    // list of semi-ordered events in measure
		vector<SimultaneousEvents> m_sortedevents; // list of time-sorted events
		MeasureStyle       m_style;     // measure style type
		bool               m_stems = false;

	friend MxmlEvent;
	friend MxmlPart;
};



class Option_register {
	public:
		         Option_register     (void);
		         Option_register     (const string& aDefinition, char aType,
		                                  const string& aDefaultOption);
		         Option_register     (const string& aDefinition, char aType,
		                                  const string& aDefaultOption,
		                                  const string& aModifiedOption);
		         Option_register     (const Option_register& reg);
		        ~Option_register     ();

		Option_register& operator=(const Option_register& reg);
		void     clearModified      (void);
		string   getDefinition      (void);
		string   getDefault         (void);
		string   getOption          (void);
		string   getModified        (void);
		string   getDescription     (void);
		int      isModified         (void);
		char     getType            (void);
		void     reset              (void);
		void     setDefault         (const string& aString);
		void     setDefinition      (const string& aString);
		void     setDescription     (const string& aString);
		void     setModified        (const string& aString);
		void     setType            (char aType);
		ostream& print              (ostream& out);

	protected:
		string       m_definition;
		string       m_description;
		string       m_defaultOption;
		string       m_modifiedOption;
		int          m_modifiedQ;
		char         m_type;
};


class Options {
	public:
		                Options           (void);
		                Options           (int argc, char** argv);
		                Options           (const Options& options);
		               ~Options           ();

		Options&        operator=         (const Options& options);
		int             argc              (void) const;
		const vector<string>& argv        (void) const;
		int             define            (const string& aDefinition);
		int             define            (const string& aDefinition,
		                                   const string& description);
		string          getArg            (int index);
		string          getArgument       (int index);
		int             getArgCount       (void);
		int             getArgumentCount  (void);
		vector<string>& getArgList        (vector<string>& output);
		vector<string>& getArgumentList   (vector<string>& output);
		int             getBoolean        (const string& optionName);
		string          getCommand        (void);
		string          getCommandLine    (void);
		string          getDefinition     (const string& optionName);
		double          getDouble         (const string& optionName);
		char            getFlag           (void);
		char            getChar           (const string& optionName);
		float           getFloat          (const string& optionName);
		int             getInt            (const string& optionName);
		int             getInteger        (const string& optionName);
		string          getString         (const string& optionName);
		char            getType           (const string& optionName);
		int             optionsArg        (void);
		ostream&        print             (ostream& out);
		ostream&        printOptionList   (ostream& out);
		ostream&        printOptionListBooleanState(ostream& out);
		bool            process           (int error_check = 1, int suppress = 0);
		bool            process           (int argc, char** argv,
		                                      int error_check = 1,
		                                      int suppress = 0);
		bool            process           (const vector<string>& argv,
		                                      int error_check = 1,
		                                      int suppress = 0);
		bool            process           (const string& argv, int error_check = 1,
		                                      int suppress = 0);
		void            reset             (void);
		void            xverify           (int argc, char** argv,
		                                      int error_check = 1,
		                                      int suppress = 0);
		void            xverify           (int error_check = 1,
		                                      int suppress = 0);
		void            setFlag           (char aFlag);
		void            setModified       (const string& optionName,
		                                   const string& optionValue);
		void            setOptions        (int argc, char** argv);
		void            setOptions        (const vector<string>& argv);
		void            setOptions        (const string& args);
		void            appendOptions     (int argc, char** argv);
		void            appendOptions     (string& args);
		void            appendOptions     (vector<string>& argv);
		ostream&        printRegister     (ostream& out);
		int             isDefined         (const string& name);
		static vector<string> tokenizeCommandLine(const string& args);
		bool            hasParseError     (void);
		string          getParseError     (void);
		ostream&        getParseError     (ostream& out);

	protected:
		// m_argv: the list of raw command line strings including
		// a mix of options and non-option argument.
		vector<string> m_argv;

		// m_arguments: list of parsed command-line arguments which
		// are not options, or the command (argv[0]);
		vector<string> m_arguments;

		// m_optionRegister: store for the states/values of each option.
		vector<Option_register*> m_optionRegister;

		// m_optionFlag: the character which indicates an option.
		// Generally a dash, but could be made a slash for Windows environments.
		char m_optionFlag = '-';

		// m_optionList:
		map<string, int> m_optionList;

		//
		// boolern options for object:
		//

		// m_options_error_check: for .verify() function.
		bool m_options_error_checkQ = true;

		// m_processedQ: true if process() was run.  This will parse
		// the command-line arguments into a list of options, and also
		// enable boolean versions of the options.
		bool m_processedQ = false;

		// m_suppressQ: true means to suppress automatic --options option
		// listing.
		bool m_suppressQ = false;

		// m_optionsArgument: indicate that --options was used.
		bool m_optionsArgQ = false;

		// m_error: used to store errors in parsing command-line options.
		stringstream m_error;

	private:
		int     getRegIndex    (const string& optionName);
		bool    isOption       (const string& aString, int& argp);
		int     storeOption    (int gargp, int& position, int& running);
};

#define OPTION_BOOLEAN_TYPE   'b'
#define OPTION_CHAR_TYPE      'c'
#define OPTION_DOUBLE_TYPE    'd'
#define OPTION_FLOAT_TYPE     'f'
#define OPTION_INT_TYPE       'i'
#define OPTION_STRING_TYPE    's'
#define OPTION_UNKNOWN_TYPE   'x'



class HumTool : public Options {
	public:
		         HumTool         (void);
		        ~HumTool         ();

		void     clearOutput     (void);

		bool     hasAnyText      (void);
		string   getAllText      (void);
		ostream& getAllText      (ostream& out);

		bool     hasHumdrumText  (void);
		string   getHumdrumText  (void);
		ostream& getHumdrumText  (ostream& out);

		bool     hasJsonText     (void);
		string   getJsonText     (void);
		ostream& getJsonText     (ostream& out);

		bool     hasFreeText     (void);
		string   getFreeText     (void);
		ostream& getFreeText     (ostream& out);

		bool     hasWarning      (void);
		string   getWarning      (void);
		ostream& getWarning      (ostream& out);

		bool     hasError        (void);
		string   getError        (void);
		ostream& getError        (ostream& out);

	protected:
		stringstream m_humdrum_text;  // output text in Humdrum syntax.
		stringstream m_json_text;     // output text in JSON syntax.
		stringstream m_free_text;     // output for plain text content.
	  	stringstream m_warning_text;  // output for warning messages;
	  	stringstream m_error_text;    // output for error messages;

};


///////////////////////////////////////////////////////////////////////////
//
// common command-line Interfaces
//

//////////////////////////////
//
// BASIC_INTERFACE -- Expects one Humdurm file, either from the
//    first command-line argument (left over after options have been
//    parsed out), or from standard input.
//
// function call that the interface must implement:
//  .run(HumdrumFile& infile, ostream& out)
//
//

#define BASIC_INTERFACE(CLASS)                 \
using namespace std;                           \
using namespace hum;                           \
int main(int argc, char** argv) {              \
	CLASS interface;                            \
	if (!interface.process(argc, argv)) {       \
		interface.getError(cerr);                \
		return -1;                               \
	}                                           \
	HumdrumFile infile;                         \
	if (interface.getArgCount() > 0) {          \
		infile.read(interface.getArgument(1));   \
	} else {                                    \
		infile.read(cin);                        \
	}                                           \
	int status = interface.run(infile, cout);   \
	if (interface.hasWarning()) {               \
		interface.getWarning(cerr);              \
		return 0;                                \
	}                                           \
	if (interface.hasError()) {                 \
		interface.getError(cerr);                \
		return -1;                               \
	}                                           \
	return !status;                             \
}



//////////////////////////////
//
// STREAM_INTERFACE -- Expects one Humdurm file, either from the
//    first command-line argument (left over after options have been
//    parsed out), or from standard input.
//
// function call that the interface must implement:
//  .run(HumdrumFile& infile, ostream& out)
//
//

#define STREAM_INTERFACE(CLASS)                                  \
using namespace std;                                             \
using namespace hum;                                             \
int main(int argc, char** argv) {                                \
	CLASS interface;                                              \
	if (!interface.process(argc, argv)) {                         \
		interface.getError(cerr);                                  \
		return -1;                                                 \
	}                                                             \
	HumdrumFileStream streamer(static_cast<Options&>(interface)); \
	HumdrumFile infile;                                           \
	bool status = true;                                           \
	while (streamer.read(infile)) {                               \
		status &= interface.run(infile);                           \
		if (interface.hasWarning()) {                              \
			interface.getWarning(cerr);                             \
		}                                                          \
		if (interface.hasAnyText()) {                              \
		   interface.getAllText(cout);                             \
		}                                                          \
		if (interface.hasError()) {                                \
			interface.getError(cerr);                               \
         return -1;                                              \
		}                                                          \
		if (!interface.hasAnyText()) {                             \
			cout << infile;                                         \
		}                                                          \
		interface.clearOutput();                                   \
	}                                                             \
	return !status;                                               \
}



class HumdrumFileStream {
	public:
		                HumdrumFileStream  (void);
		                HumdrumFileStream  (char** list);
		                HumdrumFileStream  (const vector<string>& list);
		                HumdrumFileStream  (Options& options);

		int             setFileList        (char** list);
		int             setFileList        (const vector<string>& list);

		void            clear              (void);
		int             eof                (void);

		int             getFile            (HumdrumFile& infile);
		int             read               (HumdrumFile& infile);

	protected:
		ifstream        m_instream;       // used to read from list of files.
		stringstream    m_urlbuffer;      // used to read data over internet.
		string          m_newfilebuffer;  // used to keep track of !!!!segment:
		                                  // records.

		vector<string>  m_filelist;       // used when not using cin
		int             m_curfile;        // index into filelist

		vector<string>  m_universals;     // storage for universal comments

		// Automatic URL downloading of data from internet in read():
		void     fillUrlBuffer            (stringstream& uribuffer,
		                                   const string& uriname);

};



class Tool_autobeam : public HumTool {
	public:
		         Tool_autobeam   (void);
		        ~Tool_autobeam   () {};

		bool     run             (HumdrumFile& infile);
		bool     run             (const string& indata, ostream& out);
		bool     run             (HumdrumFile& infile, ostream& out);

	protected:
		void     initialize      (HumdrumFile& infile);
		void     processStrand   (HTp strandstart, HTp strandend);
		void     processMeasure  (vector<HTp>& measure);
		void     addBeam         (HTp startnote, HTp endnote);
		void     addBeams        (HumdrumFile& infile);
		void     removeBeams     (HumdrumFile& infile);

	private:
		vector<vector<pair<int, HumNum> > > m_timesigs;
		vector<HTp> m_kernspines;
		bool        m_overwriteQ;
		int         m_track;

};



class Coord {
   public:
           Coord(void) { clear(); }
      void clear(void) { i = j = -1; }
      int i;
      int j;
};


class Tool_autostem : public HumTool {
	public:
		         Tool_autostem         (void);
		        ~Tool_autostem         () {};

		bool     run                   (HumdrumFile& infile);
		bool     run                   (const string& indata, ostream& out);
		bool     run                   (HumdrumFile& infile, ostream& out);

	protected:
		void     initialize            (HumdrumFile& infile);
		void      example              (void);
		void      usage                (void);
		void      autostem             (HumdrumFile& infile);
		void      getClefInfo          (vector<vector<int> >& baseline,
		                                HumdrumFile& infile);
		void      addStem              (string& input, const string& piece);
		void      processKernTokenStemsSimpleModel(HumdrumFile& infile,
		                                vector<vector<int> >& baseline,
		                                int row, int col);
		void      removeStems          (HumdrumFile& infile);
		void      removeStem2          (HumdrumFile& infile, int row, int col);
		int       getVoice             (HumdrumFile& infile, int row, int col);
		void      getNotePositions     (vector<vector<vector<int> > >& notepos,
		                                vector<vector<int> >& baseline,
		                                HumdrumFile& infile);
		void      printNotePositions   (HumdrumFile& infile,
		                                vector<vector<vector<int> > >& notepos);
		void      getVoiceInfo         (vector<vector<int> >& voice, HumdrumFile& infile);
		void      printVoiceInfo       (HumdrumFile& infile, vector<vector<int> >& voice);
		void      processKernTokenStems(HumdrumFile& infile,
		                                vector<vector<int> >& baseline, int row, int col);
		void      getMaxLayers         (vector<int>& maxlayer, vector<vector<int> >& voice,
		                                HumdrumFile& infile);
		void      assignStemDirections (vector<vector<int> >& stemdir,
		                                vector<vector<int> > & voice,
		                                vector<vector<vector<int> > >& notepos,
		                                HumdrumFile& infile);
		void      assignBasicStemDirections(vector<vector<int> >& stemdir,
		                                vector<vector<int> >& voice,
		                                vector<vector<vector<int> > >& notepos,
		                                HumdrumFile& infile);
		int       determineChordStem   (vector<vector<int> >& voice,
		                                vector<vector<vector<int> > >& notepos,
		                                HumdrumFile& infile, int row, int col);
		void      insertStems          (HumdrumFile& infile,
		                                vector<vector<int> >& stemdir);
		void      setStemDirection     (HumdrumFile& infile, int row, int col,
		                                int direction);
		void      getBeamState         (vector<vector<string > >& beams,
		                                HumdrumFile& infile);
		void      countBeamStuff       (const string& token, int& start, int& stop,
		                                int& flagr, int& flagl);
		void      getBeamSegments      (vector<vector<Coord> >& beamednotes,
		                                vector<vector<string > >& beamstates,
		                                HumdrumFile& infile, vector<int> maxlayer);
		int       getBeamDirection     (vector<Coord>& coords,
		                                vector<vector<int> >& voice,
		                                vector<vector<vector<int> > >& notepos);
		void      setBeamDirection     (vector<vector<int> >& stemdir,
		                                vector<Coord>& bnote, int direction);

	private:
		int    debugQ        = 0;       // used with --debug option
		int    removeQ       = 0;       // used with -r option
		int    noteposQ      = 0;       // used with -p option
		int    voiceQ        = 0;       // used with --voice option
		int    removeallQ    = 0;       // used with -R option
		int    overwriteQ    = 0;       // used with -o option
		int    overwriteallQ = 0;       // used with -O option
		int    Middle        = 4;       // used with -u option
		int    Borderline    = 0;       // really used with -u option
		int    notlongQ      = 0;       // used with -L option
		bool   m_quit        = false;

};


class Tool_binroll : public HumTool {
	public:
		         Tool_binroll      (void);
		        ~Tool_binroll      () {};

		bool     run               (HumdrumFile& infile);
		bool     run               (const string& indata, ostream& out);
		bool     run               (HumdrumFile& infile, ostream& out);

	protected:
		void     processFile       (HumdrumFile& infile);
		void     processStrand     (vector<vector<char>>& roll, HTp starting,
		                            HTp ending);
		void     printAnalysis     (HumdrumFile& infile,
		                            vector<vector<char>>& roll);

	private:
		HumNum    m_duration;

};


class Tool_chord : public HumTool {
	public:
		         Tool_chord      (void);
		        ~Tool_chord      () {};

		bool     run               (HumdrumFile& infile);
		bool     run               (const string& indata, ostream& out);
		bool     run               (HumdrumFile& infile, ostream& out);

	protected:
		void     processFile       (HumdrumFile& infile, int direction);
		void     processChord      (HTp tok, int direction);
		void     initialize        (void);
		void     minimizeChordPitches(vector<string>& notes, vector<pair<int,int>>& pitches);
		void     maximizeChordPitches(vector<string>& notes, vector<pair<int,int>>& pitches);

	private:
		int       m_direction = 0;
		int       m_spine     = -1;
		int       m_primary   = 0;

};


class NoteNode {
   public:
		int b40;         // base-40 pitch number or 0 if a rest, negative if tied
		int line;        // line number in original score of note
		int spine;       // spine number in original score of note
		int measure;     // measure number of note
		int serial;      // serial number 
		int mark;        // for marking search matches
		int notemarker;  // for pass-through of marks
		double beatsize; // time signature bottom value which or
		                 // 3 times the bottom if compound meter
		HumNum   duration;  // duration

		         NoteNode             (void) { clear(); }
		         NoteNode             (const NoteNode& anode);
		         NoteNode& operator=  (NoteNode& anode);
		        ~NoteNode             (void);
		void     clear                (void);
		int      isRest               (void) { return b40 == 0 ? 1 : 0; }
		int      isSustain            (void) { return b40 < 0 ? 1 : 0; }
		int      isAttack             (void) { return b40 > 0 ? 1 : 0; }
		int      getB40               (void) { return abs(b40); }
		void     setId                (const string& anid);
		string   getIdString          (void);
		string   getId                (void);

   protected:
		string  protected_id; // id number provided by data
};



class Tool_cint : public HumTool {
	public:
		         Tool_cint    (void);
		        ~Tool_cint    () {};

		bool     run                    (HumdrumFile& infile);
		bool     run                    (const string& indata, ostream& out);
		bool     run                    (HumdrumFile& infile, ostream& out);

	protected:

		void      initialize           (void);
		void      example              (void);
		void      usage                (const string& command);
		int       processFile          (HumdrumFile& infile);
		void      getKernTracks        (vector<int>& ktracks, HumdrumFile& infile);
		int       validateInterval     (vector<vector<NoteNode> >& notes, 
		                                int i, int j, int k);
		void      printIntervalInfo    (HumdrumFile& infile, int line, 
		                                int spine, vector<vector<NoteNode> >& notes, 
		                                int noteline, int noteindex, 
		                                vector<string >& abbr);
		void      getAbbreviations     (vector<string >& abbreviations, 
		                                vector<string >& names);
		void      getAbbreviation      (string& abbr, string& name);
		void      extractNoteArray     (vector<vector<NoteNode> >& notes, 
		                                HumdrumFile& infile, vector<int>& ktracks, 
		                                vector<int>& reverselookup);
		int       onlyRests            (vector<NoteNode>& data);
		int       hasAttack            (vector<NoteNode>& data);
		int       allSustained         (vector<NoteNode>& data);
		void      printPitchGrid       (vector<vector<NoteNode> >& notes, 
		                                HumdrumFile& infile);
		void      getNames             (vector<string >& names, 
		                                vector<int>& reverselookup, HumdrumFile& infile);
		void      printLattice         (vector<vector<NoteNode> >& notes, 
		                                HumdrumFile& infile, vector<int>& ktracks, 
		                                vector<int>& reverselookup, int n);
		void      printSpacer          (ostream& out);
		int       printInterval        (ostream& out, NoteNode& note1, NoteNode& note2,
		                                int type, int octaveadjust = 0);
		int       printLatticeItem     (vector<vector<NoteNode> >& notes, int n, 
		                                int currentindex, int fileline);
		int       printLatticeItemRows (vector<vector<NoteNode> >& notes, int n, 
		                                int currentindex, int fileline);
		int       printLatticeModule   (ostream& out, vector<vector<NoteNode> >& notes, 
		                                int n, int startline, int part1, int part2);
		void      printInterleaved     (HumdrumFile& infile, int line, 
		                                vector<int>& ktracks, vector<int>& reverselookup, 
		                                const string& interstring);
		void      printLatticeInterleaved(vector<vector<NoteNode> >& notes, 
		                                HumdrumFile& infile, vector<int>& ktracks, 
		                                vector<int>& reverselookup, int n);
		int       printInterleavedLattice(HumdrumFile& infile, int line, 
		                                vector<int>& ktracks, vector<int>& reverselookup,
		                                int n, int currentindex,
		                                vector<vector<NoteNode> >& notes);
		int       printCombinations    (vector<vector<NoteNode> >& notes, 
		                                HumdrumFile& infile, vector<int>& ktracks, 
		                                vector<int>& reverselookup, int n,
		                                vector<vector<string> >& retrospective,
		                                const string& searchstring);
		void      printAsCombination   (HumdrumFile& infile, int line, 
		                                vector<int>& ktracks, vector<int>& reverselookup,
		                                const string& interstring);
		int       printModuleCombinations(HumdrumFile& infile, int line, 
		                                vector<int>& ktracks, vector<int>& reverselookup,
		                                int n, int currentindex, 
		                                vector<vector<NoteNode> >& notes, 
		                                int& matchcount, 
		                                vector<vector<string> >& retrospective,
		                                const string& searchstring);
		int       printCombinationsSuspensions(vector<vector<NoteNode> >& notes, 
		                                HumdrumFile& infile, vector<int>& ktracks, 
		                                vector<int>& reverselookup, int n,
		                                vector<vector<string> >& retrospective);
		int       printCombinationModule(ostream& out, const string& filename, 
		                                vector<vector<NoteNode> >& notes, 
		                                int n, int startline, int part1, int part2,
		                                vector<vector<string> >& retrospective,
		                                char& notemarker, int markstate = 0);
		int       printCombinationModulePrepare(ostream& out, const string& filename,
		                                vector<vector<NoteNode> >& notes, int n, 
		                                int startline, int part1, int part2,
		                                vector<vector<string> >& retrospective,
		                                HumdrumFile& infile, const string& searchstring);
		int       getOctaveAdjustForCombinationModule(vector<vector<NoteNode> >& notes, 
		                                int n, int startline, int part1, int part2);
		void      addMarksToInputData  (HumdrumFile& infile, 
		                                vector<vector<NoteNode> >& notes,
		                                vector<int>& ktracks,
		                                vector<int>& reverselookup);
		void      markNote              (HumdrumFile& infile, int line, int col);
		void      initializeRetrospective(vector<vector<string> >& retrospective, 
		                                HumdrumFile& infile, vector<int>& ktracks);
		int       getTriangleIndex(int number, int num1, int num2);
		void      adjustKTracks        (vector<int>& ktracks, const string& koption);
		int       getMeasure           (HumdrumFile& infile, int line);

	private:

		int       debugQ       = 0;      // used with --debug option
		int       base40Q      = 0;      // used with --40 option
		int       base12Q      = 0;      // used with --12 option
		int       base7Q       = 0;      // used with -7 option
		int       pitchesQ     = 0;      // used with --pitches option
		int       rhythmQ      = 0;      // used with -r option and others
		int       durationQ    = 0;      // used with --dur option
		int       latticeQ     = 0;      // used with -l option
		int       interleavedQ = 0;      // used with -L option
		int       Chaincount   = 1;      // used with -n option
		int       chromaticQ   = 0;      // used with --chromatic option
		int       sustainQ     = 0;      // used with -s option
		int       zeroQ        = 0;      // used with -z option
		int       topQ         = 0;      // used with -t option
		int       toponlyQ     = 0;      // used with -T option
		int       hparenQ      = 0;      // used with -h option
		int       mparenQ      = 0;      // used with -y option
		int       locationQ    = 0;      // used with --location option
		int       koptionQ     = 0;      // used with -k option
		int       parenQ       = 0;      // used with -p option
		int       rowsQ        = 0;      // used with --rows option
		int       hmarkerQ     = 0;      // used with -h option
		int       mmarkerQ     = 0;      // used with -m option
		int       attackQ      = 0;      // used with --attacks option
		int       rawQ         = 0;      // used with --raw option
		int       raw2Q        = 0;      // used with --raw2 option
		int       xoptionQ     = 0;      // used with -x option
		int       octaveallQ   = 0;      // used with -O option
		int       octaveQ      = 0;      // used with -o option
		int       noharmonicQ  = 0;      // used with -H option
		int       nomelodicQ   = 0;      // used with -M option
		int       norestsQ     = 0;      // used with -R option
		int       nounisonsQ   = 0;      // used with -U option
		int       filenameQ    = 0;      // used with -f option
		int       searchQ      = 0;      // used with --search option
		int       markQ        = 0;      // used with --mark option
		int       countQ       = 0;      // used with --count option
		int       suspensionsQ = 0;      // used with --suspensions option
		int       uncrossQ     = 0;      // used with -c option
		int       retroQ       = 0;      // used with --retro option
		int       idQ          = 0;      // used with --id option
		vector<string> Ids;              // used with --id option
		char      NoteMarker   = '\0';   // used with -N option
		string    SearchString;
		string Spacer;

};


class Tool_dissonant : public HumTool {
	public:
		         Tool_dissonant    (void);
		        ~Tool_dissonant    () {};

		bool     run               (HumdrumFile& infile);
		bool     run               (const string& indata, ostream& out);
		bool     run               (HumdrumFile& infile, ostream& out);

	protected:
		void    doAnalysis         (vector<vector<string> >& results,
		                            NoteGrid& grid,
		                            vector<vector<NoteCell*> >& attacks,
		                            bool debug);
		void    doAnalysisForVoice (vector<vector<string> >& results,
		                            NoteGrid& grid,
		                            vector<NoteCell*>& attacks,
		                            int vindex, bool debug);
		void    findFakeSuspensions(vector<vector<string> >& results, 
		                            NoteGrid& grid,
		                            vector<NoteCell*>& attacks, int vindex);
		void    findAppoggiaturas  (vector<vector<string> >& results, 
		                            NoteGrid& grid,
		                            vector<NoteCell*>& attacks, int vindex);
		void    findLs             (vector<vector<string> >& results, 
		                            NoteGrid& grid,
		                            vector<NoteCell*>& attacks, int vindex);
		void    findYs             (vector<vector<string> >& results, 
		                            NoteGrid& grid,
		                            vector<NoteCell*>& attacks, int vindex);
		void	findCadentialVoiceFunctions(vector<vector<string> >& results,
									NoteGrid& grid,	vector<NoteCell*>& attacks,
									vector<vector<string> >& voiceFuncs,
									int vindex);

		void    printColorLegend   (HumdrumFile& infile);
		int     getNextPitchAttackIndex(NoteGrid& grid, int voicei,
		                            int sliceindex);
		void    fillLabels         (void);
		void    fillLabels2        (void);
		void    printCountAnalysis (vector<vector<string> >& data);
		void    suppressDissonances(HumdrumFile& infile, NoteGrid& grid,
		                            vector<vector<NoteCell* > >& attacks,
		                            vector<vector<string> >& results);
		void    suppressDissonancesInVoice(HumdrumFile& infile, 
		                            NoteGrid& grid, int vindex,
		                            vector<NoteCell*>& attacks,
		                            vector<string>& results);
		void    suppressSusOrnamentsInVoice(HumdrumFile& infile, 
		                            NoteGrid& grid, int vindex,
		                            vector<NoteCell*>& attacks,
		                            vector<string>& results);
		void    mergeWithPreviousNote(HumdrumFile& infile, int line, int field);
		void    mergeWithNextNote(HumdrumFile& infile, int line, int field);
		void    changeDurationOfNote(HTp note, HumNum dur);
		void    changePitch        (HTp note2, HTp note1);
		void    simplePreviousMerge(HTp pnote, HTp cnote);
		void    simpleNextMerge	   (HTp cnote, HTp nnote);
		void    changePitchOfTieGroupFollowing(HTp note, const string& pitch);
		void    mergeWithPreviousNoteViaTies(HTp pnote, HTp cnote);
		void    mergeWithPreviousNote(HumdrumFile& infile, NoteCell* cell);
		void    mergeWithNextNote(HumdrumFile& infile, NoteCell* cell);

	private:
	 	vector<HTp> m_kernspines;
		bool diss2Q = false;
		bool diss7Q = false;
		bool diss4Q = false;
		bool dissL0Q = false;
		bool dissL1Q = false;
		bool dissL2Q = false;
		bool suppressQ = false;
		bool voiceFuncsQ = false;
		bool m_voicenumQ = false;
		bool m_selfnumQ = false;

		vector<string> m_labels;

		// unaccdented non-harmonic tones:
		const int PASSING_UP           =  0; // rising passing tone
		const int PASSING_DOWN         =  1; // downward passing tone
		const int NEIGHBOR_UP          =  2; // upper neighbor
		const int NEIGHBOR_DOWN        =  3; // lower neighbor
		const int ECHAPPEE_UP          =  4; // upper échappée
		const int ECHAPPEE_DOWN        =  5; // lower échappée
		const int CAMBIATA_UP_S        =  6; // ascending short nota cambiata
		const int CAMBIATA_DOWN_S      =  7; // descending short nota cambiata
		const int CAMBIATA_UP_L        =  8; // ascending long nota cambiata
		const int CAMBIATA_DOWN_L      =  9; // descending long nota cambiata
		const int REV_CAMBIATA_UP      = 10; // incomplete anterior upper neighbor
		const int REV_CAMBIATA_DOWN    = 11; // incomplete anterior lower neighbor
		const int REV_ECHAPPEE_UP      = 12; // incomplete posterior upper neighbor
		const int REV_ECHAPPEE_DOWN    = 13; // incomplete posterior lower neighbor
		const int ANT_UP               = 14; // rising anticipation
		const int ANT_DOWN             = 15; // descending anticipation
		const int DBL_NEIGHBOR_UP      = 16; // double neighbor beginning with upper neighbor
		const int DBL_NEIGHBOR_DOWN    = 17; // double neighbor beginning with lower neighbor

		// accented non-harmonic tones:
		const int THIRD_Q_PASS_UP      = 18; // dissonant third quarter
		const int THIRD_Q_PASS_DOWN    = 19; // dissonant third quarter
		const int THIRD_Q_UPPER_NEI    = 20; // dissonant third quarter
		const int THIRD_Q_LOWER_NEI    = 21; // dissonant third quarter
		const int ACC_PASSING_UP	   = 22; // appoggiatura
		const int ACC_PASSING_DOWN	   = 23; // appoggiatura
		const int ACC_UP_NEI		   = 24; // appoggiatura
		const int ACC_LO_NEI		   = 25; // appoggiatura
		const int APP_UPPER            = 26; // appoggiatura
		const int APP_LOWER            = 27; // appoggiatura
		const int SUS_BIN  	           = 28; // binary suspension
		const int SUS_TERN  	       = 29; // ternary suspension
		const int AGENT_BIN		       = 30; // binary agent
		const int AGENT_TERN		   = 31; // ternary agent
		const int SUSPENSION_REP       = 32; // suspension repeated note
		const int FAKE_SUSPENSION_LEAP = 33; // fake suspension approached by leap
		const int FAKE_SUSPENSION_STEP = 34; // fake suspension approached by step or anticipation
		const int SUS_NO_AGENT_LEAP    = 35; // suspension missing a normal agent approached by leap
		const int SUS_NO_AGENT_STEP    = 36; // suspension missing a normal agent approached by step or anticipation
		const int CHANSON_IDIOM        = 37; // chanson idiom
		const int ORNAMENTAL_SUS	   = 38; // purely ornamental suspension

		// unknown dissonances:
		const int PARALLEL_UP          = 39; // moves in parallel with known dissonant, approached from below
		const int PARALLEL_DOWN        = 40; // moves in parallel with known dissonant, approached from above
		const int RES_PITCH			   = 41; // note of resolution of a suspension against suspension dissonance

		const int ONLY_WITH_VALID_UP   = 42; // only dissonant with identifiable dissonances, approached from below
		const int ONLY_WITH_VALID_DOWN = 43; // only dissonant with identifiable dissonances, approached from above
		const int UNKNOWN_DISSONANCE   = 44; // unknown dissonance type
		const int UNLABELED_Z2         = 45; // unknown dissonance type, 2nd interval
		const int UNLABELED_Z7         = 46; // unknown dissonance type, 7th interval
		const int UNLABELED_Z4         = 47; // unknown dissonance type, 4th interval

		const int LABELS_SIZE          = 48; // one more than last index
};



#define ND_NOTE 0  /* notes or rests + text and phrase markings */
#define ND_BAR  1  /* explicit barlines */


class NoteData {
	public:
		NoteData(void) { clear(); }
		void clear(void) { bar = pitch = phstart = phend = 0;
							  phnum = -1;
							  lyricerr = lyricnum = 0;
							  tiestart = tiecont = tieend = 0;
							  slstart = slend = 0;
							  num = denom = barnum = 0;
							  barinterp = 0; bardur = 0.0;
							  duration = 0.0; text = ""; }
		double duration;
		int    bar;       int    num;
		int    denom;     int    barnum;
		double bardur;    int    barinterp;
		int    pitch;     int    lyricerr;
		int    phstart;   int    phend;    int phnum;
		int    slstart;   int    slend;    int lyricnum;
		int    tiestart;  int    tiecont;  int tieend;
		string text;
};

		

class Tool_esac2hum : public HumTool {
	public:
		         Tool_esac2hum         (void);
		        ~Tool_esac2hum         () {};

		bool    convertFile          (ostream& out, const string& filename);
		bool    convert              (ostream& out, const string& input);
		bool    convert              (ostream& out, istream& input);

	protected:
		bool      initialize            (void);
		void      checkOptions          (Options& opts, int argc, char** argv);
		void      example               (void);
		void      usage                 (const string& command);
		void      convertEsacToHumdrum  (ostream& out, istream& input);
		bool      getSong               (vector<string>& song, istream& infile, 
		                                int init);
		void      convertSong           (vector<string>& song, ostream& out);
		bool      getKeyInfo            (vector<string>& song, string& key, 
		                                 double& mindur, int& tonic, string& meter,
		                                 ostream& out);
		void      printNoteData         (NoteData& data, int textQ, ostream& out);
		bool      getNoteList           (vector<string>& song, 
		                                 vector<NoteData>& songdata, double mindur,
		                                 int tonic);
		void      getMeterInfo          (string& meter, vector<int>& numerator, 
		                                 vector<int>& denominator);
		void      postProcessSongData   (vector<NoteData>& songdata,
		                                 vector<int>& numerator,vector<int>& denominator);
		void      printKeyInfo          (vector<NoteData>& songdata, int tonic, 
		                                 int textQ, ostream& out);
		int       getAccidentalMax      (int a, int b, int c);
		bool      printTitleInfo        (vector<string>& song, ostream& out);
		void      getLineRange          (vector<string>& song, const string& field, 
		                                 int& start, int& stop);
		void      printChar             (unsigned char c, ostream& out);
		void      printBibInfo          (vector<string>& song, ostream& out);
		void      printString           (const string& string, ostream& out);
		void      printSpecialChars     (ostream& out);
		bool      placeLyrics           (vector<string>& song,
		                                 vector<NoteData>& songdata);
		bool      placeLyricPhrase      (vector<NoteData>& songdata, 
		                                 vector<string>& lyrics, int line);
		void      getLyrics             (vector<string>& lyrics, const string& buffer);
		void      cleanupLyrics         (vector<string>& lyrics);
		bool      getFileContents       (vector<string>& array, const string& filename);
		void      chopExtraInfo         (char* holdbuffer);
		void      printHumdrumHeaderInfo(ostream& out, vector<string>& song);
		void      printHumdrumFooterInfo(ostream& out, vector<string>& song);
		
	private:
		int            debugQ = 0;        // used with --debug option
		int            verboseQ = 0;      // used with -v option
		int            splitQ = 0;        // used with -s option
		int            firstfilenum = 1;  // used with -f option
		vector<string> header;            // used with -h option
		vector<string> trailer;           // used with -t option
		string         fileextension;     // used with -x option
		string         namebase;          // used with -s option

		vector<int>    chartable;  // used printChars() & printSpecialChars()
		int inputline = 0;

};


class Tool_extract : public HumTool {
	public:
		         Tool_extract  (void);
		        ~Tool_extract  () {};

		bool     run                    (HumdrumFile& infile);
		bool     run                    (const string& indata, ostream& out);
		bool     run                    (HumdrumFile& infile, ostream& out);

	protected:

		// auto transpose functions:
		void     initialize             (HumdrumFile& infile);

		// function declarations
		void    processFile             (HumdrumFile& infile);
		void    excludeFields           (HumdrumFile& infile, vector<int>& field,
		                                 vector<int>& subfield, vector<int>& model);
		void    extractFields           (HumdrumFile& infile, vector<int>& field,
		                                 vector<int>& subfield, vector<int>& model);
		void    extractTrace            (HumdrumFile& infile, const string& tracefile);
		void    getInterpretationFields (vector<int>& field, vector<int>& subfield,
		                                 vector<int>& model, HumdrumFile& infile,
		                                 string& interps, int state);
		//void    extractInterpretations  (HumdrumFile& infile, string& interps);
		void    example                 (void);
		void    usage                   (const string& command);
		void    fillFieldData           (vector<int>& field, vector<int>& subfield,
		                                 vector<int>& model, string& fieldstring,
		                                 HumdrumFile& infile);
		void    processFieldEntry       (vector<int>& field, vector<int>& subfield,
		                                 vector<int>& model, const string& astring,
		                                 HumdrumFile& infile);
		void    removeDollarsFromString (string& buffer, int maxtrack);
		int     isInList                (int number, vector<int>& listofnum);
		void    getTraceData            (vector<int>& startline,
		                                 vector<vector<int> >& fields,
		                                 const string& tracefile, HumdrumFile& infile);
		void    printTraceLine          (HumdrumFile& infile, int line,
		                                 vector<int>& field);
		void    dealWithSpineManipulators(HumdrumFile& infile, int line,
		                                 vector<int>& field, vector<int>& subfield,
		                                 vector<int>& model);
		void    storeToken              (vector<string>& storage,
		                                 const string& string);
		void    storeToken              (vector<string>& storage, int index,
		                                 const string& string);
		void    printMultiLines         (vector<int>& vsplit, vector<int>& vserial,
		                                 vector<string>& tempout);
		void    reverseSpines           (vector<int>& field, vector<int>& subfield,
		                                 vector<int>& model, HumdrumFile& infile,
		                                 const string& exinterp);
		void    getSearchPat            (string& spat, int target,
		                                 const string& modifier);
		void    expandSpines            (vector<int>& field, vector<int>& subfield,
		                                 vector<int>& model, HumdrumFile& infile,
		                                 string& interp);
		void    dealWithSecondarySubspine(vector<int>& field, vector<int>& subfield,
		                                 vector<int>& model, int targetindex,
		                                 HumdrumFile& infile, int line, int spine,
		                                 int submodel);
		void    dealWithCospine         (vector<int>& field, vector<int>& subfield,
		                                 vector<int>& model, int targetindex,
		                                 HumdrumFile& infile, int line, int cospine,
		                                 int comodel, int submodel,
		                                 const string& cointerp);
		void    printCotokenInfo        (int& start, HumdrumFile& infile, int line,
		                                 int spine, vector<string>& cotokens,
		                                 vector<int>& spineindex,
		                                 vector<int>& subspineindex);
		void    fillFieldDataByGrep     (vector<int>& field, vector<int>& subfield,
		                                 vector<int>& model, const string& grepString,
		                                 HumdrumFile& infile, int state);

	private:

		// global variables
		int          excludeQ = 0;        // used with -x option
		int          expandQ  = 0;        // used with -e option
		string       expandInterp = "";   // used with -E option
		int          interpQ  = 0;        // used with -i option
		string       interps  = "";       // used with -i option
		int          debugQ   = 0;        // used with --debug option
		int          kernQ    = 0;        // used with -k option
		int          fieldQ   = 0;        // used with -f or -p option
		string       fieldstring = "";    // used with -f or -p option
		vector<int>  field;               // used with -f or -p option
		vector<int>  subfield;            // used with -f or -p option
		vector<int>  model;               // used with -p, or -e options and similar
		int          countQ   = 0;        // used with -C option
		int          traceQ   = 0;        // used with -t option
		string       tracefile = "";      // used with -t option
		int          reverseQ = 0;        // used with -r option
		string       reverseInterp = "**kern"; // used with -r and -R options.
		// sub-spine "b" expansion model: how to generate data for a secondary
		// spine if the primary spine is not divided.  Models are:
		//    'd': duplicate primary spine (or "a" subspine) data (default)
		//    'n': null = use a null token
		//    'r': rest = use a rest instead of a primary spine note (in **kern)
		//         data.  'n' will be used for non-kern spines when 'r' is used.
		int          submodel = 'd';       // used with -m option
		string editorialInterpretation = "yy";
		string      cointerp = "**kern";   // used with -c option
		int         comodel  = 0;          // used with -M option
		string subtokenseparator = " "; // used with a future option
		int         interpstate = 0;       // used -I or with -i
		int         grepQ       = 0;       // used with -g option
		string      grepString  = "";      // used with -g option

};



class Tool_filter : public HumTool {
	public:
		         Tool_filter        (void);
		        ~Tool_filter        () {};

		bool     run                (HumdrumFile& infile);
		bool     run                (const string& indata, ostream& out);
		bool     run                (HumdrumFile& infile, ostream& out);

	protected:
		void     getCommandList     (vector<pair<string, string> >& commands,
		                             HumdrumFile& infile);
		void     initialize         (HumdrumFile& infile);
		void     removeFilterLines  (HumdrumFile& infile);

	private:
		string   m_variant;        // used with -v option.
		bool     m_debugQ = false; // used with --debug option

};


class Tool_hproof : public HumTool {
	public:
		      Tool_hproof      (void);
		     ~Tool_hproof      () {};

		bool  run              (HumdrumFile& infile);
		bool  run              (const string& indata, ostream& out);
		bool  run              (HumdrumFile& infile, ostream& out);

	protected:
		void  markNonChordTones(HumdrumFile& infile);
		void  processHarmSpine (HumdrumFile& infile, HTp hstart);
		void  markNotesInRange (HumdrumFile& infile, HTp ctoken, HTp ntoken, const string& key);
		void  markHarmonicTones(HTp tok, vector<int>& cts);
		void  getNewKey        (HTp token, HTp ntoken, string& key);

	private:
		vector<HTp> m_kernspines;

};



class Tool_imitation : public HumTool {
	public:
		         Tool_imitation    (void);
		        ~Tool_imitation    () {};

		bool     run               (HumdrumFile& infile);
		bool     run               (const string& indata, ostream& out);
		bool     run               (HumdrumFile& infile, ostream& out);

	protected:
		void    doAnalysis         (vector<vector<string>>& results, NoteGrid& grid,
		                            vector<vector<NoteCell*>>& attacks,
		                            vector<vector<double>>& intervals,
		                            HumdrumFile& infile, bool debug);
		void    analyzeImitation  (vector<vector<string>>& results,
		                            vector<vector<NoteCell*>>& attacks,
		                            vector<vector<double>>& intervals,
		                            int v1, int v2);
		void    getIntervals       (vector<double>& intervals,
		                            vector<NoteCell*>& attacks);
		int     compareSequences   (vector<NoteCell*>& attack1, vector<double>& seq1,
		                            int i1, vector<NoteCell*>& attack2,
		                            vector<double>& seq2, int i2);
		int     checkForIntervalSequence(vector<int>& m_intervals,
		                            vector<double>& v1i, int starti, int count);
		void    markedTiedNotes    (vector<HTp>& tokens);

	private:
	 	vector<HTp> m_kernspines;
		int m_threshold;
		bool m_duration;
		bool m_rest;
		bool m_rest2;
		double m_maxdistance;
		bool m_maxdistanceQ;
		vector<int> m_intervals;
		bool m_mark;
		char m_marker = '@';
		static int Enumerator;
};


class mei_staffDef {
	public:
		HumNum timestamp;
		string clef;       // such as *clefG2
		string timesig;    // such as *M4/4
		string keysig;     // such as *k[f#]
		string midibpm;    // such as *MM120
		string transpose;  // such as *Trd-1c-2
		int base40 = 0;    // used for transposing to C score
		string label;      // such as *I"violin 1
		string labelabbr;  // such as *I'v1

		void clear(void) {
			clef.clear();
			timesig.clear();
			keysig.clear();
			midibpm.clear();
			transpose.clear();
			base40 = 0;
			label.clear();
			labelabbr.clear();
		}
		mei_staffDef& operator=(mei_staffDef& staffDef) {
			if (this == &staffDef) {
				return *this;
			}
			clef       = staffDef.clef;
			timesig    = staffDef.timesig;
			keysig     = staffDef.keysig;
			midibpm    = staffDef.midibpm;
			transpose  = staffDef.transpose;
			base40     = staffDef.base40;
			label      = staffDef.label;
			labelabbr  = staffDef.labelabbr;
			return *this;
		}
		mei_staffDef(void) {
			// do nothing
		}
		mei_staffDef(const mei_staffDef& staffDef) {
			clef       = staffDef.clef;
			timesig    = staffDef.timesig;
			keysig     = staffDef.keysig;
			midibpm    = staffDef.midibpm;
			transpose  = staffDef.transpose;
			base40     = staffDef.base40;
			label      = staffDef.label;
			labelabbr  = staffDef.labelabbr;
		}
};


class mei_scoreDef {
	public:
		mei_staffDef global;
		vector<mei_staffDef> staves;
		void clear(void) {
			global.clear();
			staves.clear(); // or clear the contents of each staff...
		}
		void minresize(int count) {
			if (count < 1) {
				return;
			} else if (count < (int)staves.size()) {
				return;
			} else {
				staves.resize(count);
			}
		}
};


class hairpin_info {
	public:
		xml_node hairpin;
		GridMeasure *gm = NULL;
		int mindex = 0;
};


class grace_info {
	public:
		xml_node node; // note or chord
		string beamprefix;
		string beampostfix;
};


class Tool_mei2hum : public HumTool {
	public:
		        Tool_mei2hum    (void);
		       ~Tool_mei2hum    () {}

		bool    convertFile          (ostream& out, const char* filename);
		bool    convert              (ostream& out, xml_document& infile);
		bool    convert              (ostream& out, const char* input);
		bool    convert              (ostream& out, istream& input);

		void    setOptions           (int argc, char** argv);
		void    setOptions           (const vector<string>& argvlist);
		Options getOptionDefinitions (void);

	protected:
		void   initialize           (void);
		HumNum parseScore           (xml_node score, HumNum starttime);
		void   getChildrenVector    (vector<xml_node>& children, xml_node parent);
		void   parseScoreDef        (xml_node scoreDef, HumNum starttime);
		void   parseSectionScoreDef (xml_node scoreDef, HumNum starttime);
		void   processPgHead        (xml_node pgHead, HumNum starttime);
		void   processPgFoot        (xml_node pgFoot, HumNum starttime);
		HumNum parseSection         (xml_node section, HumNum starttime);
		HumNum parseApp             (xml_node app, HumNum starttime);
		HumNum parseLem             (xml_node lem, HumNum starttime);
		HumNum parseRdg             (xml_node rdg, HumNum starttime);
		void   parseStaffGrp        (xml_node staffGrp, HumNum starttime);
		void   parseStaffDef        (xml_node staffDef, HumNum starttime);
		void   fillWithStaffDefAttributes(mei_staffDef& staffinfo, xml_node element);
		HumNum parseMeasure         (xml_node measure, HumNum starttime);
		HumNum parseStaff           (xml_node staff, HumNum starttime);
		void   parseReh             (xml_node reh, HumNum starttime);
		HumNum parseLayer           (xml_node layer, HumNum starttime, vector<bool>& layerPresent);
		int    extractStaffCount    (xml_node element);
		HumNum parseRest            (xml_node chord, HumNum starttime);
		HumNum parseMRest           (xml_node mrest, HumNum starttime);
		HumNum parseChord           (xml_node chord, HumNum starttime, int gracenumber);
		HumNum parseNote            (xml_node note, xml_node chord, string& output, HumNum starttime, int gracenumber);
		HumNum parseBeam            (xml_node note, HumNum starttime);
		HumNum parseTuplet          (xml_node note, HumNum starttime);
		void   parseClef            (xml_node clef, HumNum starttime);
		void   parseDynam           (xml_node dynam, HumNum starttime);
		void   parseTempo           (xml_node tempo, HumNum starttime);
		void   parseDir             (xml_node dir, HumNum starttime);
		HumNum getDuration          (xml_node element);
		string getHumdrumPitch      (xml_node note);
		string getHumdrumRecip      (HumNum duration, int dotcount);
		void   buildIdLinkMap       (xml_document& doc);
		void   processNodeStartLinks(string& output, xml_node node,
		                             vector<xml_node>& nodelist);
		void   processNodeStopLinks(string& output, xml_node node,
		                             vector<xml_node>& nodelist);
		void   processPreliminaryLinkedNodes(xml_node node);
		void   processNodeStartLinks2(xml_node node, vector<xml_node>& nodelist);
		void   parseFermata         (string& output, xml_node node, xml_node fermata);
		void   parseSlurStart       (string& output, xml_node node, xml_node slur);
		void   parseSlurStop        (string& output, xml_node node, xml_node slur);
		void   parseTieStart        (string& output, xml_node node, xml_node tie);
		void   parseTieStop         (string& output, xml_node node, xml_node tie);
		void   parseArpeg           (string& output, xml_node node, xml_node arpeg);
		void   parseTrill           (string& output, xml_node node, xml_node trill);
		void   parseTupletSpanStart (xml_node node, xml_node tupletSpan);
		void   parseTupletSpanStop  (string& output, xml_node node, xml_node tupletSpan);
		void   parseSb              (xml_node sb, HumNum starttime);
		void   parsePb              (xml_node pb, HumNum starttime);
		void   processLinkedNodes   (string& output, xml_node node);
		int    getDotCount          (xml_node node);
		void   processFermataAttribute(string& output, xml_node node);
		string getNoteArticulations (xml_node note, xml_node chord);
		string getHumdrumArticulation(const string& tag, const string& humdrum,
		                              const string& attribute_artic,
		                              vector<xml_node>& element_artic,
		                              const string& chord_attribute_artic,
		                              vector<xml_node>& chord_element_artic);
		string setPlacement          (const string& placement);
		void   addFooterRecords      (HumdrumFile& outfile, xml_document& doc);
		void   addExtMetaRecords     (HumdrumFile& outfile, xml_document& doc);
		void   addHeaderRecords      (HumdrumFile& outfile, xml_document& doc);
		void   parseVerse            (xml_node verse, GridStaff* staff);
		string parseSyl              (xml_node syl);
		void   parseSylAttribute     (const string& attsyl, GridStaff* staff);
		void   reportVerseNumber     (int pmax, int staffindex);
		string getEditorialAccidental(vector<xml_node>& children);
		string getCautionaryAccidental(vector<xml_node>& children);
		string makeHumdrumClef       (const string& shape, 
		                              const string& line,
		                              const string& clefdis,
		                              const string& clefdisplace);
		string cleanDirText          (const string& input);
		string cleanWhiteSpace       (const string& input);
		string cleanReferenceRecordText(const string& input);
		string cleanVerseText        (const string& input);
		bool   beamIsValid           (vector<xml_node>& beamlist);
		bool   beamIsGrace           (vector<xml_node>& beamlist);
		void   parseHairpin          (xml_node hairpin, HumNum starttime);
		void   processHairpins       (void);
		void   processHairpin        (hairpin_info& info);
		void   processGraceNotes     (HumNum timestamp);
		string prepareSystemDecoration(xml_node scoreDef);
		void   getRecursiveSDString  (string& output, xml_node current);
		void   parseBareSyl          (xml_node syl, GridStaff* staff);

	private:
		Options        m_options;
		bool           m_stemsQ = false;
		bool           m_recipQ = false;
		bool           m_placeQ = false;

		mei_scoreDef   m_scoreDef;    // for keeping track of key/meter/clef etc.
		int            m_staffcount;  // number of staves in score.
		HumNum         m_tupletfactor = 1;
		HumGrid        m_outdata;
		int            m_currentLayer = 0;
		int            m_currentStaff = 0;
		int            m_maxStaffInFile = 0; // valid after parsing staves in first measure
		int            m_currentMeasure = -1;
		vector<int>    m_currentMeterUnit;
		string         m_beamPrefix;
		string         m_beamPostfix;
		bool           m_aboveQ = false;
		bool           m_belowQ = false;
		bool           m_editorialAccidentalQ = false;
		string         m_appLabel;
		string         m_systemDecoration;

		vector<int>    m_maxverse;
		vector<HumNum> m_measureDuration;
		vector<bool>   m_hasDynamics;
		const int      m_maxstaff = 1000;

		bool           m_fermata = false;     // set priority of note/fermata over note@fermata
		vector<grace_info> m_gracenotes;      // buffer for storing grace notes
		HumNum			m_gracetime = 0;       // performance time of buffered grace notes

		vector<hairpin_info> m_hairpins;

		map<string, vector<xml_node>> m_startlinks;
		map<string, vector<xml_node>> m_stoplinks;

};



class Tool_metlev : public HumTool {
	public:
		      Tool_metlev      (void);
		     ~Tool_metlev      () {};

		bool  run              (HumdrumFile& infile);
		bool  run              (const string& indata, ostream& out);
		bool  run              (HumdrumFile& infile, ostream& out);

	protected:
		void  fillVoiceResults (vector<vector<double> >& results,
		                        HumdrumFile& infile,
		                        vector<double>& beatlev);

	private:
		vector<HTp> m_kernspines;

};



class MSearchQueryToken {
	public:
		MSearchQueryToken(void) {
			clear();
		}
		MSearchQueryToken(const MSearchQueryToken& token) {
			pc        = token.pc;
			base      = token.base;
			direction = token.direction;
			duration  = token.duration;
			rhythm    = token.rhythm;
			anything  = token.anything;
		}
		MSearchQueryToken& operator=(const MSearchQueryToken& token) {
			if (this == &token) {
				return *this;
			}
			pc        = token.pc;
			base      = token.base;
			direction = token.direction;
			duration  = token.duration;
			rhythm    = token.rhythm;
			anything  = token.anything;
			return *this;
		}
		void clear(void) {
			pc        = NAN;
			base      = 0;
			direction = 0;
			duration  = -1;
			rhythm    = "";
			anything  = false;
		}
		double pc;           // NAN = rest
		int    base;
		int    direction; 
		HumNum duration;
		string rhythm;
		bool   anything;
};



class MSearchTextQuery {
	public:
		MSearchTextQuery(void) {
			clear();
		}
		MSearchTextQuery(const MSearchTextQuery& token) {
			word = token.word;
			link = token.link;
		}
		MSearchTextQuery& operator=(const MSearchTextQuery& token) {
			if (this == &token) {
				return *this;
			}
			word = token.word;
			link = token.link;
			return *this;
		}
		void clear(void) {
			word.clear();
			link = false;
		}
		string word;
		bool link = false;
};


class TextInfo {
	public:
		TextInfo(void) {
			clear();
		}
		TextInfo(const TextInfo& info) {
			fullword = info.fullword;
			starttoken = info.starttoken;
			nexttoken = info.nexttoken;
		}
		TextInfo& operator=(const TextInfo& info) {
			if (this == &info) {
				return *this;
			}
			fullword = info.fullword;
			starttoken = info.starttoken;
			nexttoken = info.nexttoken;
			return *this;
		}
		void clear(void) {
			fullword.clear();
			starttoken = NULL;
			nexttoken = NULL;
		}
		string fullword;
		HTp starttoken;
		HTp nexttoken;
};


class Tool_msearch : public HumTool {
	public:
		         Tool_msearch      (void);
		        ~Tool_msearch      () {};

		bool     run               (HumdrumFile& infile);
		bool     run               (const string& indata, ostream& out);
		bool     run               (HumdrumFile& infile, ostream& out);

	protected:
		void    initialize         (void);
		void    doMusicSearch      (HumdrumFile& infile, NoteGrid& grid,
		                            vector<MSearchQueryToken>& query);
		void    doTextSearch       (HumdrumFile& infile, NoteGrid& grid,
		                            vector<MSearchTextQuery>& query);
		void    fillMusicQuery     (vector<MSearchQueryToken>& query,
		                            const string& input);
		void    fillTextQuery      (vector<MSearchTextQuery>& query,
		                            const string& input);
		bool    checkForMatchDiatonicPC(vector<NoteCell*>& notes, int index, 
		                            vector<MSearchQueryToken>& dpcQuery,
		                            vector<NoteCell*>& match);
		void    markMatch          (HumdrumFile& infile,
		                            vector<NoteCell*>& match);
		void    markTextMatch      (HumdrumFile& infile, TextInfo& word);
		void    fillWords          (HumdrumFile& infile,
		                            vector<TextInfo*>& words);
		void    fillWordsForTrack  (vector<TextInfo*>& words,
		                            HTp starttoken);

	private:
	 	vector<HTp> m_kernspines;
		string      m_text;
		string      m_marker;
};


class Tool_musicxml2hum : public HumTool {
	public:
		        Tool_musicxml2hum    (void);
		       ~Tool_musicxml2hum    () {}

		bool    convertFile          (ostream& out, const char* filename);
		bool    convert              (ostream& out, xml_document& infile);
		bool    convert              (ostream& out, const char* input);
		bool    convert              (ostream& out, istream& input);

		void    setOptions           (int argc, char** argv);
		void    setOptions           (const vector<string>& argvlist);
		Options getOptionDefinitions (void);

	protected:
		void   initialize           (void);
		string getChildElementText  (xml_node root, const char* xpath);
		string getChildElementText  (xpath_node root, const char* xpath);
		string getAttributeValue    (xml_node xnode, const string& target);
		string getAttributeValue    (xpath_node xnode, const string& target);
		void   printAttributes      (xml_node node);
		bool   getPartInfo          (map<string, xml_node>& partinfo,
		                             vector<string>& partids, xml_document& doc);
		bool   stitchParts          (HumGrid& outdata,
		                             vector<string>& partids,
		                             map<string, xml_node>& partinfo,
		                             map<string, xml_node>& partcontent,
		                             vector<MxmlPart>& partdata);
		bool   getPartContent       (map<string, xml_node>& partcontent,
		                             vector<string>& partids, xml_document& doc);
		void   printPartInfo        (vector<string>& partids,
		                             map<string, xml_node>& partinfo,
		                             map<string, xml_node>& partcontent,
		                             vector<MxmlPart>& partdata);
		bool   fillPartData         (vector<MxmlPart>& partdata,
		                             const vector<string>& partids,
		                             map<string, xml_node>& partinfo,
		                             map<string, xml_node>& partcontent);
		bool   fillPartData         (MxmlPart& partdata, const string& id,
		                             xml_node partdeclaration,
		                             xml_node partcontent);
		void   appendZeroEvents     (GridMeasure* outfile,
		                             vector<SimultaneousEvents*>& nowevents,
		                             HumNum nowtime,
		                             vector<MxmlPart>& partdata);
		void   appendNonZeroEvents   (GridMeasure* outdata,
		                              vector<SimultaneousEvents*>& nowevents,
		                              HumNum nowtime,
		                              vector<MxmlPart>& partdata);
		void   addGraceLines         (GridMeasure* outdata,
		                              vector<vector<vector<vector<MxmlEvent*> > > >& notes,
		                              vector<MxmlPart>& partdata, HumNum nowtime);
		void   addEventToList        (vector<vector<vector<vector<MxmlEvent*> > > >& list, 
		                              MxmlEvent* event);
		void   addHeaderRecords      (HumdrumFile& outfile, xml_document& doc);
		void   addFooterRecords      (HumdrumFile& outfile, xml_document& doc);
		string cleanSpaces           (string& input);
		void setEditorialAccidental  (int accidental, GridSlice* slice, 
		                              int partindex, int staffindex, int voiceindex);

		bool convert          (ostream& out);
		bool convertPart      (ostream& out, const string& partname,
		                       int partindex);
		bool insertMeasure    (HumGrid& outdata, int mnum,
		                       vector<MxmlPart>& partdata,
		                       vector<int> partstaves);
		bool convertNowEvents (GridMeasure* outdata, 
		                       vector<SimultaneousEvents*>& nowevents,
		                       vector<int>& nowparts, 
		                       HumNum nowtime,
		                       vector<MxmlPart>& partdata, 
		                       vector<int>& partstaves);
		void appendNullTokens (HumdrumLine* line, MxmlPart& part);
		void appendEvent      (HumdrumLine* line, MxmlEvent* event);
		void insertExclusiveInterpretationLine(HumdrumFile& outfile,
		                       vector<MxmlPart>& partdata);
		void insertAllToken   (HumdrumFile& outfile, vector<MxmlPart>& partdata,
		                       const string& common);
		void insertSingleMeasure(HumdrumFile& outfile);
		void cleanupMeasures   (HumdrumFile& outfile,
		                        vector<HumdrumLine*> measures);
		void processPrintElement(GridMeasure* outdata, xml_node element, HumNum timestamp);

		void addClefLine       (GridMeasure* outdata, vector<vector<xml_node> >& clefs,
		                        vector<MxmlPart>& partdata, HumNum nowtime);
		void insertPartClefs   (xml_node clef, GridPart& part);
		xml_node convertClefToHumdrum(xml_node clef, HTp& token, int& staffindex);

		void addTranspositionLine(GridMeasure* outdata, vector<vector<xml_node> >& transpositions,
		                       vector<MxmlPart>& partdata, HumNum nowtime);
		void addKeySigLine    (GridMeasure* outdata, vector<vector<xml_node> >& keysigs,
		                        vector<MxmlPart>& partdata, HumNum nowtime);
		void insertPartKeySigs (xml_node keysig, GridPart& part);
		xml_node convertKeySigToHumdrum(xml_node keysig, 
		                        HTp& token, int& staffindex);

		void addTimeSigLine    (GridMeasure* outdata, vector<vector<xml_node> >& timesigs,
		                        vector<MxmlPart>& partdata, HumNum nowtime);
		bool insertPartTimeSigs (xml_node timesig, GridPart& part);
		void insertPartMensurations(xml_node timesig, GridPart& part);
		void insertPartNames    (HumGrid& outdata, vector<MxmlPart>& partdata);
		bool checkForMensuration(xml_node timesig);
		xml_node convertTimeSigToHumdrum(xml_node timesig, 
		                        HTp& token, int& staffindex);
		xml_node convertMensurationToHumdrum(xml_node timesig,
		                        HTp& token, int& staffindex);

		void addEvent          (GridSlice* slice, GridMeasure* outdata, MxmlEvent* event);
		void fillEmpties       (GridPart* part, const char* string);
		void addSecondaryChordNotes (ostream& output, MxmlEvent* head, const string& recip);
		bool isInvisible       (MxmlEvent* event);
		int  addLyrics         (GridStaff* staff, MxmlEvent* event);
		int  addHarmony        (GridPart* oart, MxmlEvent* event);
		void addDynamic        (GridPart* part, MxmlEvent* event);
		void addTexts          (GridSlice* slice, GridMeasure* measure, int partindex,
		                        int staffindex, int voiceindex, MxmlEvent* event);
		void addText           (GridSlice* slice, GridMeasure* measure, int partindex, 
		                        int staffindex, int voiceindex, xml_node node);
		string getHarmonyString(xml_node hnode);
		string getDynamicString(xml_node element);
		string getDynamicsParameters(xml_node element);
		string getHairpinString(xml_node element);
		string cleanSpaces     (const string& input);
		void checkForDummyRests(MxmlMeasure* measure);
		void reindexVoices     (vector<MxmlPart>& partdata);
		void reindexMeasure    (MxmlMeasure* measure);
		void setSoftwareInfo   (xml_document& doc);
		string getSystemDecoration(xml_document& doc, HumGrid& grid, vector<string>& partids);
		void getChildrenVector (vector<xml_node>& children, xml_node parent);
		void insertPartTranspositions(xml_node transposition, GridPart& part);
		xml_node convertTranspositionToHumdrum(xml_node transpose, HTp& token, int& staffindex);

	public:

	static bool nodeType      (xml_node node, const char* testname);

	private:
		Options m_options;
		bool DebugQ;
		bool VoiceDebugQ;
		bool m_recipQ = false;
		bool m_stemsQ = false;
		int m_slurabove = 0;
		int m_slurbelow = 0;
		char m_hasEditorial = '\0';

		string m_software;
		string m_systemDecoration;

		xml_node m_current_dynamic = xml_node(NULL);
		vector<xml_node> m_current_text;
		bool m_hasTransposition = false;

};



class MyCoord {
	public:
		     MyCoord   (void) { clear(); }
		void clear   (void) { x = -1; y = -1; }
		bool isValid (void) { return ((x < 0) || (y < 0)) ? false : true; }
		int  x;
		int  y;
};

class MeasureInfo {
	public:
		MeasureInfo(void) { clear(); }
		void clear(void)  { num = seg = start = stop = -1; 
			sclef.resize(0); skeysig.resize(0); skey.resize(0);
			stimesig.resize(0); smet.resize(0); stempo.resize(0);
			eclef.resize(0); ekeysig.resize(0); ekey.resize(0);
			etimesig.resize(0); emet.resize(0); etempo.resize(0);
			file = NULL;
		}
		void setTrackCount(int tcount) {
			sclef.resize(tcount+1);
			skeysig.resize(tcount+1);
			skey.resize(tcount+1);
			stimesig.resize(tcount+1);
			smet.resize(tcount+1);
			stempo.resize(tcount+1);
			eclef.resize(tcount+1);
			ekeysig.resize(tcount+1);
			ekey.resize(tcount+1);
			etimesig.resize(tcount+1);
			emet.resize(tcount+1);
			etempo.resize(tcount+1);
			int i;
			for (i=0; i<tcount+1; i++) {
				sclef[i].clear();
				skeysig[i].clear();
				skey[i].clear();
				stimesig[i].clear();
				smet[i].clear();
				stempo[i].clear();
				eclef[i].clear();
				ekeysig[i].clear();
				ekey[i].clear();
				etimesig[i].clear();
				emet[i].clear();
				etempo[i].clear();
			}
			tracks = tcount;
		}
		int num;          // measure number
		int seg;          // measure segment
		int start;        // starting line of segment
		int stop;         // ending line of segment
		int tracks;       // number of primary tracks in file.
		HumdrumFile* file;
	 
		// musical settings at start of measure
		vector<MyCoord> sclef;     // starting clef of segment
		vector<MyCoord> skeysig;   // starting keysig of segment
		vector<MyCoord> skey;      // starting key of segment
		vector<MyCoord> stimesig;  // starting timesig of segment
		vector<MyCoord> smet;      // starting met of segment
		vector<MyCoord> stempo;    // starting tempo of segment

		// musical settings at start of measure
		vector<MyCoord> eclef;     // ending clef    of segment
		vector<MyCoord> ekeysig;   // ending keysig  of segment
		vector<MyCoord> ekey;      // ending key     of segment
		vector<MyCoord> etimesig;  // ending timesig of segment
		vector<MyCoord> emet;      // ending met     of segment
		vector<MyCoord> etempo;    // ending tempo   of segment
};



class Tool_myank : public HumTool {
	public:
		         Tool_myank            (void);
		        ~Tool_myank            () {};

		bool     run                   (HumdrumFile& infile);
		bool     run                   (const string& indata, ostream& out);
		bool     run                   (HumdrumFile& infile, ostream& out);

	protected:
		void      initialize            (HumdrumFile& infile);
		void      example              (void);
		void      usage                (const string& command);
		void      myank                (HumdrumFile& infile, 
		                                vector<MeasureInfo>& outmeasure);
		void      removeDollarsFromString(string& buffer, int maxx);
		void      processFieldEntry    (vector<MeasureInfo>& field, 
		                                const string& str, 
		                                HumdrumFile& infile, int maxmeasure, 
		                                vector<MeasureInfo>& inmeasures, 
		                                vector<int>& inmap);
		void      expandMeasureOutList (vector<MeasureInfo>& measureout, 
		                                vector<MeasureInfo>& measurein, 
		                                HumdrumFile& infile, const string& optionstring);
		void      getMeasureStartStop  (vector<MeasureInfo>& measurelist, 
		                                HumdrumFile& infile);
		void      printEnding          (HumdrumFile& infile, int lastline, int adjlin);
		void      printStarting        (HumdrumFile& infile);
		void      reconcileSpineBoundary(HumdrumFile& infile, int index1, int index2);
		void      reconcileStartingPosition(HumdrumFile& infile, int index2);
		void      printJoinLine        (vector<int>& splits, int index, int count);
		void      printInvisibleMeasure(HumdrumFile& infile, int line);
		void      fillGlobalDefaults   (HumdrumFile& infile, 
		                                vector<MeasureInfo>& measurein, 
		                                vector<int>& inmap);
		void      adjustGlobalInterpretations(HumdrumFile& infile, int ii,
		                                vector<MeasureInfo>& outmeasures,
		                                int index);
		void      adjustGlobalInterpretationsStart(HumdrumFile& infile, int ii,
		                                vector<MeasureInfo>& outmeasures, 
		                                int index);
		void      getMarkString        (ostream& out, HumdrumFile& infile);
		void      printDoubleBarline   (HumdrumFile& infile, int line);
		void      insertZerothMeasure  (vector<MeasureInfo>& measurelist, 
		                                HumdrumFile& infile);
		void      getMetStates         (vector<vector<MyCoord> >& metstates, 
		                                HumdrumFile& infile);
		MyCoord   getLocalMetInfo      (HumdrumFile& infile, int row, int track);
		int       atEndOfFile          (HumdrumFile& infile, int line);
		void      processFile          (HumdrumFile& infile);
		int       getSectionCount      (HumdrumFile& infile);
		void      getSectionString     (string& sstring, HumdrumFile& infile,
		                                int sec);

	private:
		int    debugQ      = 0;             // used with --debug option
		// int    inputlist   = 0;             // used with --inlist option
		int    inlistQ     = 0;             // used with --inlist option
		int    outlistQ    = 0;             // used with --outlist option
		int    verboseQ    = 0;             // used with -v option
		int    invisibleQ  = 1;             // used with --visible option
		int    maxQ        = 0;             // used with --max option
		int    minQ        = 0;             // used with --min option
		int    instrumentQ = 0;             // used with -I option
		int    nolastbarQ  = 0;             // used with -B option
		int    markQ       = 0;             // used with --mark option
		int    doubleQ     = 0;             // used with --mdsep option
		int    barnumtextQ = 0;             // used with -T option
		int    Section     = 0;             // used with --section option
		int    sectionCountQ = 0;           // used with --section-count option
		vector<MeasureInfo> MeasureOutList; // used with -m option
		vector<MeasureInfo> MeasureInList;  // used with -m option
		vector<vector<MyCoord> > metstates;

};


class Tool_recip : public HumTool {
	public:
		      Tool_recip               (void);
		     ~Tool_recip               () {};

		bool  run                      (HumdrumFile& infile);
		bool  run                      (const string& indata, ostream& out);
		bool  run                      (HumdrumFile& infile, ostream& out);

	protected:
		void  initialize               (HumdrumFile& infile);
		void  replaceKernWithRecip     (HumdrumFile& infile);
		void  doCompositeAnalysis      (HumdrumFile& infile);
		void  insertAnalysisSpines     (HumdrumFile& infile, HumdrumFile& cfile);

	private:
		vector<HTp> m_kernspines;
		bool        m_graceQ = true;
		string      m_exinterp = "**recip";

};



class Tool_ruthfix : public HumTool {
	public:
		         Tool_ruthfix      (void);
		        ~Tool_ruthfix      () {};

		bool     run               (HumdrumFile& infile);
		bool     run               (const string& indata, ostream& out);
		bool     run               (HumdrumFile& infile, ostream& out);

	protected:
		void    insertCrossBarTies (HumdrumFile& infile);
		void    insertCrossBarTies (HumdrumFile& infile, int strand);
		void    createTiedNote     (HTp left, HTp right);

};


class Tool_satb2gs : public HumTool {
	public:
		         Tool_satb2gs    (void);
		        ~Tool_satb2gs    () {};

		bool     run             (HumdrumFile& infile);
		bool     run             (const string& indata, ostream& out);
		bool     run             (HumdrumFile& infile, ostream& out);

	protected:
		void     initialize      (HumdrumFile& infile);
		void     processFile     (HumdrumFile& infile);
		void     example         (void);
		void     usage           (const string& command);
		void     convertData     (HumdrumFile& infile);
		int      getSatbTracks   (vector<int>& tracks, HumdrumFile& infile);
		void     printSpine      (HumdrumFile& infile, int row, int col, 
		                          vector<int>& satbtracks);
		void     printExInterp   (HumdrumFile& infile, int line, 
		                          vector<int>& tracks);
		void     printLastLine   (HumdrumFile& infile, int line, 
		                          vector<int>& tracks);
	private:
		int    debugQ    = 0;             // used with --debug option
};



class Tool_tassoize : public HumTool {
	public:
		         Tool_tassoize   (void);
		        ~Tool_tassoize   () {};

		bool     run                (HumdrumFile& infile);
		bool     run                (const string& indata, ostream& out);
		bool     run                (HumdrumFile& infile, ostream& out);

	protected:
		void     initialize         (HumdrumFile& infile);
		void     processFile        (HumdrumFile& infile);
		void     updateKeySignatures(HumdrumFile& infile, int lineindex);
		void     checkDataLine      (HumdrumFile& infile, int lineindex);
		void     clearStates        (void);

	private:
		vector<vector<int>> m_pstates;
		vector<vector<bool>> m_estates;

};


class Tool_transpose : public HumTool {
	public:
		         Tool_transpose  (void);
		        ~Tool_transpose  () {};

		bool     run             (HumdrumFile& infile);
		bool     run             (const string& indata, ostream& out);
		bool     run             (HumdrumFile& infile, ostream& out);

	protected:

		// auto transpose functions:
		void     initialize             (HumdrumFile& infile);
		void     convertScore           (HumdrumFile& infile, int style);
		void     processFile            (HumdrumFile& infile,
		                                 vector<bool>& spineprocess);
		void     convertToConcertPitches(HumdrumFile& infile, int line,
		                                 vector<int>& tvals);
		void     convertToWrittenPitches(HumdrumFile& infile, int line,
		                                 vector<int>& tvals);
		void     printNewKeySignature   (const string& keysig, int trans);
		void     processInterpretationLine(HumdrumFile& infile, int line,
		                                 vector<int>& tvals, int style);
		int      isKeyMarker            (const string& str);
		void     printNewKeyInterpretation(HumdrumLine& aRecord,
		                                 int index, int transval);
		int      hasTrMarkers           (HumdrumFile& infile, int line);
		void     printHumdrumKernToken  (HumdrumLine& record, int index,
		                                 int transval);
		int      checkForDeletedLine    (HumdrumFile& infile, int line);
		int      getBase40ValueFromInterval(const string& string);
		void     example                (void);
		void     usage                  (const string& command);
		void     printHumdrumDataRecord (HumdrumLine& record,
		                                 vector<bool>& spineprocess);

		double   pearsonCorrelation     (int size, double* x, double* y);
		void     doAutoTransposeAnalysis(HumdrumFile& infile);
		void     addToHistogramDouble   (vector<vector<double> >& histogram,
		                                 int pc, double start, double dur,
		                                 double tdur, int segments);
		double   storeHistogramForTrack (vector<vector<double> >& histogram, 
		                                 HumdrumFile& infile, int track,
		                                 int segments);
		void     printHistograms        (int segments, vector<int> ktracks, 
		                                vector<vector<vector<double> > >&
		                                 trackhist);
		void     doAutoKeyAnalysis      (vector<vector<vector<double> > >&
		                                 analysis, int level, int hop, int count,
		                                 int segments, vector<int>& ktracks, 
		                                 vector<vector<vector<double> > >&
		                                 trackhist);
		void     doTrackKeyAnalysis     (vector<vector<double> >& analysis,
		                                 int level, int hop, int count, 
		                                 vector<vector<double> >& trackhist,
		                                 vector<double>& majorweights,
		                                 vector<double>& minorweights);
		void     identifyKeyDouble      (vector<double>& correls, 
		                                 vector<double>& histogram, 
		                                 vector<double>& majorweights, 
		                                 vector<double>& minorweights);
		void     fillWeightsWithKostkaPayne(vector<double>& maj,
		                                 vector<double>& min);
		void     printRawTrackAnalysis  (vector<vector<vector<double> > >&
		                                 analysis, vector<int>& ktracks);
		void     doSingleAnalysis       (vector<double>& analysis,
		                                 int startindex, int length,
		                                 vector<vector<double> >& trackhist, 
		                                 vector<double>& majorweights, 
		                                 vector<double>& minorweights);
		void     identifyKey            (vector<double>& correls, 
		                                 vector<double>& histogram,
		                                 vector<double>& majorweights, 
		                                 vector<double>& minorweights);
		void     doTranspositionAnalysis(vector<vector<vector<double> > >&
		                                 analysis);
		int      calculateTranspositionFromKey(int targetkey,
		                                 HumdrumFile& infile);
		void     printTransposedToken   (HumdrumFile& infile, int row, int col,
		                                 int transval);
		void     printTransposeInformation(HumdrumFile& infile,
		                                 vector<bool>& spineprocess,
		                                 int line, int transval);
		int      getTransposeInfo       (HumdrumFile& infile, int row, int col);
		void     printNewKernString     (const string& string, int transval);

	private:
		int      transval     = 0;   // used with -b option
		int      ssetkeyQ     = 0;   // used with -k option
		int      ssetkey      = 0;   // used with -k option
		int      currentkey   = 0;
		int      autoQ        = 0;   // used with --auto option
		int      debugQ       = 0;   // used with --debug option
		int      spineQ       = 0;   // used with -s option
		string   spinestring  = "";  // used with -s option
		int      octave       = 0;   // used with -o option
		int      concertQ     = 0;   // used with -C option
		int      writtenQ     = 0;   // used with -W option
		int      quietQ       = 0;   // used with -q option
		int      instrumentQ  = 0;   // used with -I option
};



} // end of namespace hum


#endif /* _HUMLIB_H_INCLUDED */



