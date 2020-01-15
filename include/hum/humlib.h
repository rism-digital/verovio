//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Aug  8 12:24:49 PDT 2015
// Last Modified: Thu 05 Dec 2019 10:35:04 AM PST
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
#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
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


class HumParameter : public std::string {
	public:
		HumParameter(void);
		HumParameter(const std::string& str);
		HumdrumToken* origin;
};

typedef std::map<std::string, std::map<std::string, std::map<std::string, HumParameter> > > MapNNKV;
typedef std::map<std::string, std::map<std::string, HumParameter> > MapNKV;
typedef std::map<std::string, HumParameter> MapKV;

class HumHash {
	public:
		               HumHash             (void);
		              ~HumHash             ();

		std::string    getValue            (const std::string& key) const;
		std::string    getValue            (const std::string& ns2,
		                                    const std::string& key) const;
		std::string    getValue            (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key) const;
		HTp            getValueHTp         (const std::string& key) const;
		HTp            getValueHTp         (const std::string& ns2,
		                                    const std::string& key) const;
		HTp            getValueHTp         (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key) const;
		int            getValueInt         (const std::string& key) const;
		int            getValueInt         (const std::string& ns2,
		                                    const std::string& key) const;
		int            getValueInt         (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key) const;
		HumNum         getValueFraction    (const std::string& key) const;
		HumNum         getValueFraction    (const std::string& ns2,
		                                    const std::string& key) const;
		HumNum         getValueFraction    (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key)const ;
		double         getValueFloat       (const std::string& key)const ;
		double         getValueFloat       (const std::string& ns2,
		                                    const std::string& key) const;
		double         getValueFloat       (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key) const;
		bool           getValueBool        (const std::string& key) const;
		bool           getValueBool        (const std::string& ns2,
		                                    const std::string& key) const;
		bool           getValueBool        (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key) const;

		void           setValue            (const std::string& key,
		                                    const std::string& value);
		void           setValue            (const std::string& ns2,
		                                    const std::string& key,
		                                    const std::string& value);
		void           setValue            (const std::string& ns1,
		                                    const std::string& ns2,
		                                    const std::string& key,
		                                    const std::string& value);
		void           setValue            (const std::string& key,
		                                    const char* value);
		void           setValue            (const std::string& ns2,
		                                    const std::string& key,
		                                    const char* value);
		void           setValue            (const std::string& ns1,
		                                    const std::string& ns2,
		                                    const std::string& key,
		                                    const char* value);
		void           setValue            (const std::string& key, int value);
		void           setValue            (const std::string& ns2, const std::string& key,
		                                    int value);
		void           setValue            (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key, int value);
		void           setValue            (const std::string& key, HTp value);
		void           setValue            (const std::string& ns2, const std::string& key,
		                                    HTp value);
		void           setValue            (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key, HTp value);
		void           setValue            (const std::string& key, HumNum value);
		void           setValue            (const std::string& ns2, const std::string& key,
		                                    HumNum value);
		void           setValue            (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key, HumNum value);
		void           setValue            (const std::string& key, double value);
		void           setValue            (const std::string& ns2, const std::string& key,
		                                    double value);
		void           setValue            (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key, double value);
		bool           isDefined           (const std::string& key) const;
		bool           isDefined           (const std::string& ns2,
		                                    const std::string& key) const;
		bool           isDefined           (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key) const;
		void           deleteValue         (const std::string& key);
		void           deleteValue         (const std::string& ns2, const std::string& key);
		void           deleteValue         (const std::string& ns1, const std::string& ns2,
		                                    const std::string& key);
		std::vector<std::string> getKeys             (void) const;
		std::vector<std::string> getKeys             (const std::string& ns) const;
		std::vector<std::string> getKeys             (const std::string& ns1,
		                                    const std::string& ns2) const;
		bool           hasParameters       (void) const;
		bool           hasParameters       (const std::string& ns) const;
		bool           hasParameters       (const std::string& ns1,
		                                    const std::string& ns2) const;
		int            getParameterCount   (void) const;
		int            getParameterCount   (const std::string& ns) const;
		int            getParameterCount   (const std::string& ns1,
		                                    const std::string& ns2) const;
		void           setPrefix           (const std::string& value);
		std::string    getPrefix           (void) const;
		std::ostream&       printXml            (std::ostream& out = std::cout, int level = 0,
		                                    const std::string& indent = "\t");
		std::ostream&       printXmlAsGlobal    (std::ostream& out = std::cout, int level = 0,
		                                    const std::string& indent = "\t");

		void           setOrigin           (const std::string& key,
		                                    HumdrumToken* tok);
		void           setOrigin           (const std::string& key,
		                                    HumdrumToken& tok);
		void           setOrigin           (const std::string& ns2, const std::string& key,
		                                    HumdrumToken* tok);
		void           setOrigin           (const std::string& ns2, const std::string& key,
		                                    HumdrumToken& tok);
		void           setOrigin           (const std::string& ns1, const std::string& ns2,
		                                    const std::string& parameter,
		                                    HumdrumToken* tok);
		void           setOrigin           (const std::string& ns1, const std::string& ns2,
		                                    const std::string& parameter,
		                                    HumdrumToken& tok);

		HumdrumToken*  getOrigin           (const std::string& key) const;
		HumdrumToken*  getOrigin           (const std::string& ns2,
		                                    const std::string& key) const;
		HumdrumToken*  getOrigin           (const std::string& ns1,
		                                    const std::string& ns2,
		                                    const std::string& parameter) const;

	protected:
		void                     initializeParameters  (void);
		std::vector<std::string> getKeyList            (const std::string& keys) const;

	private:
		MapNNKV*    parameters;
		std::string prefix;

	friend std::ostream& operator<<(std::ostream& out, const HumHash& hash);
};



class HumNum {
	public:
		         HumNum             (void);
		         HumNum             (int value);
		         HumNum             (int numerator, int denominator);
		         HumNum             (const HumNum& rat);
		         HumNum             (const std::string& ratstring);
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
		void     setValue           (const std::string& ratstring);
		void     setValue           (const char* ratstring);
		void     invert             (void);
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
		std::ostream& printFraction      (std::ostream& = std::cout) const;
		std::ostream& printMixedFraction (std::ostream& out = std::cout,
		                             std::string separator = "_") const;
		std::ostream& printList          (std::ostream& out) const;
		std::ostream& printTwoPart  (std::ostream& out, const std::string& spacer = "+") const;

	protected:
		void     reduce             (void);
		int      gcdIterative       (int a, int b);
		int      gcdRecursive       (int a, int b);

	private:
		int top;
		int bot;
};


std::ostream& operator<<(std::ostream& out, const HumNum& number);

template <typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<A>& v);



class HumRegex {
	public:
		            HumRegex           (void);
		            HumRegex           (const std::string& exp,
		                                const std::string& options = "");
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
		std::string&     replaceDestructive (std::string& input, const std::string& replacement,
		                                const std::string& exp);
		std::string&     replaceDestructive (std::string& input, const std::string& replacement,
		                                const std::string& exp,
		                                const std::string& options);
		std::string      replaceCopy        (const std::string& input,
		                                const std::string& replacement,
		                                const std::string& exp);
		std::string      replaceCopy        (const std::string& input,
		                                const std::string& replacement,
		                                const std::string& exp,
		                                const std::string& options);

		std::string&     replaceDestructive (std::string* input, const std::string& replacement,
		                                const std::string& exp);
		std::string&     replaceDestructive (std::string* input, const std::string& replacement,
		                                const std::string& exp,
		                                const std::string& options);
		std::string      replaceCopy        (std::string* input, const std::string& replacement,
		                                const std::string& exp);
		std::string      replaceCopy        (std::string* input, const std::string& replacement,
		                                const std::string& exp,
		                                const std::string& options);
		std::string&      tr                 (std::string& input, const std::string& from,
		                                const std::string& to);

		// matching (full-string match)
		bool        match              (const std::string& input, const std::string& exp);
		bool        match              (const std::string& input, const std::string& exp,
		                                const std::string& options);
		bool        match              (const std::string* input, const std::string& exp);
		bool        match              (const std::string* input, const std::string& exp,
		                                const std::string& options);


		// searching
		// http://www.cplusplus.com/reference/regex/regex_search
		int         search             (const std::string& input, const std::string& exp);
		int         search             (const std::string& input, const std::string& exp,
		                                const std::string& options);
		int         search             (const std::string& input, int startindex,
		                                const std::string& exp);
		int         search             (const std::string& input, int startindex,
		                                const std::string& exp,
		                                const std::string& options);

		int         search             (std::string* input, const std::string& exp);
		int         search             (std::string* input, const std::string& exp,
		                                const std::string& options);
		int         search             (std::string* input, int startindex,
		                                const std::string& exp);
		int         search             (std::string* input, int startindex,
		                                const std::string& exp,
		                                const std::string& options);

		int         getMatchCount      (void);
		std::string getMatch           (int index);
		int         getMatchInt        (int index);
		double      getMatchDouble     (int index);
		std::string getPrefix          (void);
		std::string getSuffix          (void);
		int         getMatchStartIndex (int index = 0);
		int         getMatchEndIndex   (int index = 0);
		int         getMatchLength     (int index = 0);

		// token lists:
		bool        split              (std::vector<std::string>& entries,
		                                const std::string& buffer,
		                                const std::string& separator);

	protected:
		std::regex_constants::syntax_option_type
				getTemporaryRegexFlags(const std::string& sflags);
		std::regex_constants::match_flag_type
				getTemporarySearchFlags(const std::string& sflags);


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



enum signifier_type {
	signifier_unknown,
	signifier_link,
	signifier_above,
	signifier_below
};

class HumSignifier {

	public:
		            HumSignifier     (void);
		            HumSignifier     (const std::string& rdfline);
		           ~HumSignifier     ();
		bool        parseSignifier   (const std::string& rdfline);
		void        clear            (void);
		std::string getSignifier     (void);
		std::string getDefinition    (void);
		std::string getParameter     (const std::string& key);
		bool        isKernLink       (void);
		bool        isKernAbove      (void);
		bool        isKernBelow      (void);

	private:
		std::string m_exinterp;
		std::string m_signifier;
		std::string m_definition;
		int         m_sigtype = signifier_type::signifier_unknown;
		std::map<std::string, std::string> m_parameters;
};



class HumSignifiers {
	public:
		              HumSignifiers    (void);
		             ~HumSignifiers    ();

		void          clear            (void);
		bool          addSignifier     (const std::string& rdfline);
		bool          hasKernLinkSignifier (void);
		std::string   getKernLinkSignifier (void);
		bool          hasKernAboveSignifier (void);
		std::string   getKernAboveSignifier (void);
		bool          hasKernBelowSignifier (void);
		std::string   getKernBelowSignifier (void);
		int           getSignifierCount(void);
		HumSignifier* getSignifier(int index);

	private:
		std::vector<HumSignifier*> m_signifiers;
		int  m_kernLinkIndex = -1;
		int  m_kernAboveIndex = -1;
		int  m_kernBelowIndex = -1;

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
		const std::string&  getSpineInfo      (void) const;
		int                 getTrack          (void) const;
		int                 getSubtrack       (void) const;
		int                 getSubtrackCount  (void) const;
		std::string         getTrackString    (std::string separator = ".") const;
		HumdrumLine*        getLine           (void) const;
		HumdrumLine*        getOwner          (void) const { return getLine(); }
		bool                hasOwner          (void) const;

	protected:
		void                setOwner          (HumdrumLine* aLine);
		void                setFieldIndex     (int fieldlindex);
		void                setSpineInfo      (const std::string& spineinfo);
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
		std::string m_spining;

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
		              HumParamSet        (const std::string& token);
		              HumParamSet        (HTp token);
		             ~HumParamSet        ();

		const std::string& getNamespace1      (void);
		const std::string& getNamespace2      (void);
		std::string   getNamespace       (void);
		void          setNamespace1      (const std::string& name);
		void          setNamespace2      (const std::string& name);
		void          setNamespace       (const std::string& name);
		void          setNamespace       (const std::string& name1, const std::string& name2);

		void          clear              (void);
		int           getCount           (void);
		const std::string& getParameterName   (int index);
		const std::string& getParameterValue  (int index);
		int           addParameter       (const std::string& name, const std::string& value);
		int           setParameter       (const std::string& name, const std::string& value);
		void          readString         (const std::string& text);
		std::ostream& printXml           (std::ostream& out = std::cout, int level = 0,
		                                  const std::string& indent = "\t");

	private:
		std::string m_ns1;
		std::string m_ns2;
		std::vector<std::pair<std::string, std::string>> m_parameters;

};


std::ostream& operator<<(std::ostream& out, HumParamSet* hps);
std::ostream& operator<<(std::ostream& out, HumParamSet& hps);



class _HumInstrument {
	public:
		_HumInstrument    (void) { humdrum = ""; name = ""; gm = 0; }
	  ~_HumInstrument    ()     { humdrum = ""; name = ""; gm = 0; }

		std::string humdrum;
		std::string name;
		int         gm;
};


class HumInstrument {
	public:
		            HumInstrument       (void);
		            HumInstrument       (const std::string& Hname);
		           ~HumInstrument       ();

		std::string getName             (void);
		std::string getName             (const std::string& Hname);
		std::string getHumdrum          (void);
		int         getGM               (void);
		int         getGM               (const std::string& Hname);
		void        setHumdrum          (const std::string& Hname);
		int         setGM               (const std::string& Hname, int aValue);

	private:
		int                            index;
		static std::vector<_HumInstrument>  data;
		static int                     classcount;

	protected:
		void       initialize          (void);
		void       afi                 (const char* humdrum_name, int midinum,
		                                const char* EN_name);
		int        find                (const std::string& Hname);
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



typedef HumdrumLine* HLp;

class HumdrumLine : public std::string, public HumHash {
	public:
		            HumdrumLine            (void);
		            HumdrumLine            (const std::string& aString);
		            HumdrumLine            (const char* aString);
		            HumdrumLine            (HumdrumLine& line);
		            HumdrumLine            (HumdrumLine& line, void* owner);
		           ~HumdrumLine            ();

		HumdrumLine& operator=             (HumdrumLine& line);
		bool        isComment              (void) const;
		bool        isCommentLocal         (void) const;
		bool        isLocalComment         (void) const { return isCommentLocal(); }
		bool        isCommentGlobal        (void) const;
		bool        isCommentUniversal     (void) const;
		bool        isReference            (void) const;
		bool        isGlobalReference      (void) const;
		bool        isUniversalReference   (void) const;
		bool        isSignifier            (void) const;
		std::string getReferenceKey        (void) const;
		std::string getReferenceValue      (void) const;
		std::string getGlobalReferenceKey      (void) const;
		std::string getGlobalReferenceValue    (void) const;
		std::string getUniversalReferenceKey   (void) const;
		std::string getUniversalReferenceValue (void) const;
		bool        isUniversalComment     (void) const { return isCommentUniversal(); }
		bool        isGlobalComment        (void) const { return isCommentGlobal(); }
		bool        isExclusive            (void) const;
		bool        isExclusiveInterpretation (void) const { return isExclusive(); }
		bool        isTerminator           (void) const;
		bool        isInterp               (void) const;
		bool        isInterpretation       (void) const { return isInterp(); }
		bool        isBarline              (void) const;
		bool        isData                 (void) const;
		bool        isAllNull              (void) const;
		bool        isAllRhythmicNull      (void) const;
		bool        isEmpty                (void) const;
		bool        isBlank                (void) const { return isEmpty(); }
		bool        isManipulator          (void) const;
		bool        hasSpines              (void) const;
		bool        isGlobal               (void) const;
		HTp         token                  (int index) const;
		void        getTokens              (std::vector<HTp>& list);
		int         getTokenCount          (void) const;
		int         getFieldCount          (void) const { return getTokenCount(); }
		std::string getTokenString         (int index) const;
		bool        equalChar              (int index, char ch) const;
		char        getChar                (int index) const;
		bool        isKernBoundaryStart    (void) const;
		bool        isKernBoundaryEnd      (void) const;
		std::ostream& printSpineInfo       (std::ostream& out = std::cout);
		std::ostream& printTrackInfo       (std::ostream& out = std::cout);
		std::ostream& printDataTypeInfo    (std::ostream& out = std::cout);
		std::ostream& printDurationInfo    (std::ostream& out = std::cout);
		std::ostream& printCsv             (std::ostream& out = std::cout,
		                                    const std::string& separator = ",");
		std::ostream& printXml             (std::ostream& out = std::cout, int level = 0,
		                                    const std::string& indent = "\t");
		std::ostream& printXmlParameterInfo(std::ostream& out, int level,
		                                    const std::string& indent);
		std::ostream& printGlobalXmlParameterInfo(std::ostream& out, int level,
		                                    const std::string& indent);
		std::string   getXmlId             (const std::string& prefix = "") const;
		std::string   getXmlIdPrefix       (void) const;
		void          createLineFromTokens (void);
		void          removeExtraTabs      (void);
		void          addExtraTabs         (std::vector<int>& trackWidths);
		int           getLineIndex         (void) const;
		int           getLineNumber        (void) const;
		HumdrumFile*  getOwner             (void);
		void          setText              (const std::string& text);
		std::string   getText              (void);
		int           getBarNumber         (void);
		int           getMeasureNumber     (void) { return getBarNumber(); }

		HumNum      getDuration            (void);
		HumNum      getDurationFromStart   (void);
		HumNum      getDurationToEnd       (void);
		HumNum      getDurationFromBarline (void);
		HumNum      getDurationToBarline   (void);
		HumNum      getBarlineDuration     (void);

		HumNum      getDuration            (HumNum scale);
		HumNum      getDurationFromStart   (HumNum scale);
		HumNum      getDurationToEnd       (HumNum scale);
		HumNum      getDurationFromBarline (HumNum scale);
		HumNum      getDurationToBarline   (HumNum scale);
		HumNum      getBarlineDuration     (HumNum scale);
		int         getKernNoteAttacks     (void);
		int         addLinkedParameter     (HTp token);

		HumNum   getBeat                (HumNum beatdur = 1);
		HumNum   getBeatStr             (std::string beatrecip = "4");
		HTp      getTrackStart          (int track) const;
		void     setLineFromCsv         (const char* csv,
		                                 const std::string& separator = ",");
		void     setLineFromCsv         (const std::string& csv,
		                                 const std::string& separator = ",");

		// low-level editing functions (need to re-analyze structure after using)
		void     appendToken            (HTp token, int tabcount = 1);
		void     appendToken            (const HumdrumToken& token, int tabcount = 1);
		void     appendToken            (const std::string& token, int tabcount = 1);
		void     appendToken            (const char* token, int tabcount = 1);

		void     appendToken            (int index, HTp token, int tabcount = 1);
		void     appendToken            (int index, const HumdrumToken& token, int tabcount = 1);
		void     appendToken            (int index, const std::string& token, int tabcount = 1);
		void     appendToken            (int index, const char* token, int tabcount = 1);

		void     insertToken            (int index, HTp token, int tabcount = 1);
		void     insertToken            (int index, const HumdrumToken& token, int tabcount = 1);
		void     insertToken            (int index, const std::string& token, int tabcount = 1);
		void     insertToken            (int index, const char* token, int tabcount = 1);

		void     setDuration            (HumNum aDur);
		void     setDurationFromStart   (HumNum dur);
		void     setDurationFromBarline (HumNum dur);
		void     setDurationToBarline   (HumNum dur);

	protected:
		bool     analyzeTracks          (std::string& err);
		bool     analyzeTokenDurations  (std::string& err);
		void     setLineIndex           (int index);
		void     clear                  (void);
		void     setOwner               (void* hfile);
		int      createTokensFromLine   (void);
		void     setLayoutParameters    (void);
		void     setParameters          (const std::string& pdata);
		void     storeGlobalLinkedParameters(void);
		std::ostream&	printXmlGlobalLinkedParameterInfo(std::ostream& out = std::cout, int level = 0,
		                                 const std::string& indent = "\t");
		std::ostream& printXmlGlobalLinkedParameters(std::ostream& out = std::cout, int level = 0,
		                                 const std::string& indent = "\t");

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
		std::vector<HumdrumToken*> m_tokens;

		// m_tabs: Used to store a count of the number of tabs between
		// each token on a line.  This is the number of tabs after the
		// token at the given index (so no tabs before the first token).
		std::vector<int> m_tabs;

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
		std::vector<HTp> m_linkedParameters;

		// m_rhythm_analyzed: True if duration information from HumdrumFile
		// has been added to line.
		bool m_rhythm_analyzed = false;

		// owner: This is the HumdrumFile which manages the given line.
		void* m_owner;

	friend class HumdrumFileBase;
	friend class HumdrumFileStructure;
	friend class HumdrumFileContent;
	friend class HumdrumFile;
};

std::ostream& operator<< (std::ostream& out, HumdrumLine& line);
std::ostream& operator<< (std::ostream& out, HumdrumLine* line);




typedef HumdrumToken* HTp;

class HumdrumToken : public std::string, public HumHash {
	public:
		         HumdrumToken              (void);
		         HumdrumToken              (const HumdrumToken& token);
		         HumdrumToken              (HumdrumToken* token);
		         HumdrumToken              (const HumdrumToken& token,
		                                    HumdrumLine* owner);
		         HumdrumToken              (HumdrumToken* token,
		                                    HumdrumLine* owner);
		         HumdrumToken              (const char* token);
		         HumdrumToken              (const std::string& token);
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
		bool     isChord                   (const std::string& separator = " ");
		bool     isLabel                   (void) const;
		bool     hasRhythm                 (void) const;
		bool     hasBeam                   (void) const;
		bool     equalTo                   (const std::string& pattern);

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
		bool     isTempo                   (void);
		bool     isMensurationSymbol       (void);

		bool     hasSlurStart              (void);
		bool     hasSlurEnd                (void);
		int      hasVisibleAccidental      (int subtokenIndex) const;
		int      hasCautionaryAccidental   (int subtokenIndex) const;
		bool     hasLigatureBegin          (void);
		bool     hasRectaLigatureBegin     (void);
		bool     hasObliquaLigatureBegin   (void);
		bool     hasLigatureEnd            (void);
		bool     hasRectaLigatureEnd       (void);
		bool     hasObliquaLigatureEnd     (void);
		char     hasStemDirection          (void);

		HumNum   getDuration               (void);
		HumNum   getDuration               (HumNum scale);
		HumNum   getTiedDuration           (void);
		HumNum   getTiedDuration           (HumNum scale);
		HumNum   getDurationNoDots         (void);
		HumNum   getDurationNoDots         (HumNum scale);
		int      getDots                   (char separator = ' ') const;

		HumNum   getDurationFromStart      (void);
		HumNum   getDurationFromStart      (HumNum scale);

		HumNum   getDurationToEnd          (void);
		HumNum   getDurationToEnd          (HumNum scale);

		HumNum   getDurationFromBarline    (void);
		HumNum   getDurationFromBarline    (HumNum scale);

		HumNum   getDurationToBarline      (void);
		HumNum   getDurationToBarline      (HumNum scale);

		HumNum   getBarlineDuration        (void);
		HumNum   getBarlineDuration        (HumNum scale);

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
		const std::string& getDataType     (void) const;
		bool     isDataType                (const std::string& dtype) const;
		bool     isKern                    (void) const;
		bool     isMens                    (void) const;
		std::string   getSpineInfo         (void) const;
		int      getTrack                  (void) const;
		int      getSubtrack               (void) const;
		bool     noteInLowerSubtrack       (void);
		std::string   getTrackString       (void) const;
		int      getSubtokenCount          (const std::string& separator = " ") const;
		std::string   getSubtoken          (int index,
		                                    const std::string& separator = " ") const;
		std::vector<std::string> getSubtokens (const std::string& separator = " ") const;
		void     replaceSubtoken           (int index, const std::string& newsubtok,
		                                    const std::string& separator = " ");
		void     setParameters             (HTp ptok);
		void     setParameters             (const std::string& pdata, HTp ptok = NULL);
		int      getStrandIndex            (void) const;
		int      getSlurStartElisionLevel  (int index = 0) const;
		int      getSlurEndElisionLevel    (int index = 0) const;
		HTp      getSlurStartToken         (int number = 0);
		HTp      getSlurEndToken           (int number = 0);
		void     storeLinkedParameters     (void);
		bool     linkedParameterIsGlobal   (int index);
		std::ostream& printCsv             (std::ostream& out = std::cout);
		std::ostream& printXml             (std::ostream& out = std::cout, int level = 0,
		                                    const std::string& indent = "\t");
		std::ostream& printGlobalXmlParameterInfo(std::ostream& out = std::cout, int level = 0,
		                                   const std::string& indent = "\t");
		std::string   getXmlId             (const std::string& prefix = "") const;
		std::string   getXmlIdPrefix       (void) const;
		void     setText                   (const std::string& text);
		std::string   getText              (void) const;
		int      addLinkedParameter        (HTp token);
		int      getLinkedParameterCount   (void);
		HumParamSet* getLinkedParameter    (int index);
		HumParamSet* getLinkedParameter    (void);
		std::string getSlurLayoutParameter (const std::string& keyname, int subtokenindex = -1);
		std::string getLayoutParameter     (const std::string& category, const std::string& keyname,
		                                    int subtokenindex = -1);
		std::string getLayoutParameterChord(const std::string& category,
		                                    const std::string& keyname);
		std::string getLayoutParameterNote (const std::string& category,
		                                    const std::string& keyname, int subtokenindex);
		std::ostream& printXmlLinkedParameterInfo(std::ostream& out, int level, const std::string& indent);

		// layout parameter accessors
		std::string   getVisualDuration    (int subtokenindex = -1);
		std::string   getVisualDurationChord(void);
		std::string   getVisualDurationNote(int subtokenindex = -1);

		HumdrumToken& operator=            (HumdrumToken& aToken);
		HumdrumToken& operator=            (const std::string& aToken);
		HumdrumToken& operator=            (const char* aToken);

		// next/previous token functions:
		int         getNextTokenCount      (void) const;
		int         getPreviousTokenCount  (void) const;
		HTp         getNextToken           (int index = 0) const;
		HTp         getPreviousToken       (int index = 0) const;
		std::vector<HTp> getNextTokens     (void) const;
		std::vector<HTp> getPreviousTokens (void) const;

		// next/previous token on line:
		HTp      getNextFieldToken         (void) const;
		HTp      getPreviousFieldToken     (void) const;

		int      getPreviousNonNullDataTokenCount(void);
		int      getPreviousNNDTCount      (void)
		                           { return getPreviousNonNullDataTokenCount(); }
		HTp      getPreviousNonNullDataToken(int index = 0);
		HTp      getPreviousNNDT           (int index = 0)
		                           { return getPreviousNonNullDataToken(index); }
		int      getNextNonNullDataTokenCount(void);
		int      getNextNNDTCount           (void)
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
		void     setSpineInfo              (const std::string& spineinfo);
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

		bool     analyzeDuration           (void);
		std::ostream& printXmlBaseInfo     (std::ostream& out = std::cout, int level = 0,
		                                    const std::string& indent = "\t");
		std::ostream& printXmlContentInfo  (std::ostream& out = std::cout, int level = 0,
		                                    const std::string& indent = "\t");
		std::ostream& printXmlStructureInfo(std::ostream& out = std::cout, int level = 0,
		                                    const std::string& indent = "\t");
		std::ostream& printXmlParameterInfo(std::ostream& out = std::cout, int level = 0,
		                                    const std::string& indent = "\t");
		std::ostream&	printXmlLinkedParameters (std::ostream& out = std::cout, int level = 0,
		                                    const std::string& indent = "\t");

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
		std::vector<HTp> m_nextTokens;     // link to next token(s) in spine

		// previousTokens: Simiar to nextTokens, but for the immediately
		// follow token(s) in the data.  Typically there will be one
		// preceding token, but there can be multiple tokens when the previous
		// line has *v merge tokens for the spine.  Exclusive interpretations
		// have no tokens preceding them.
		std::vector<HTp> m_previousTokens; // link to last token(s) in spine

		// nextNonNullTokens: This is a list of non-tokens in the spine
		// that follow this one.
		std::vector<HTp> m_nextNonNullTokens;

		// previousNonNullTokens: This is a list of non-tokens in the spine
		// that preced this one.
		std::vector<HTp> m_previousNonNullTokens;

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
		std::vector<HTp> m_linkedParameters;

		// m_linkedParameter: A single parameter encoded in the text of the
		// token.
		HumParamSet* m_linkedParameter = NULL;

		// m_rhythm_analyzed: Set to true when HumdrumFile assigned duration
		bool m_rhythm_analyzed = false;

	friend class HumdrumLine;
	friend class HumdrumFileBase;
	friend class HumdrumFileStructure;
	friend class HumdrumFileContent;
	friend class HumdrumFile;
};


std::ostream& operator<<(std::ostream& out, const HumdrumToken& token);
std::ostream& operator<<(std::ostream& out, HTp token);

std::ostream& printSequence(std::vector<std::vector<HTp> >& sequence, std::ostream& out=std::cout);
std::ostream& printSequence(std::vector<HTp>& sequence, std::ostream& out = std::cout);



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
		              HumdrumFileBase          (const std::string& contents);
		              HumdrumFileBase          (std::istream& contents);
		             ~HumdrumFileBase          ();

		HumdrumFileBase& operator=             (HumdrumFileBase& infile);
		bool          read                     (std::istream& contents);
		bool          read                     (const char* filename);
		bool          read                     (const std::string& filename);
		bool          readCsv                  (std::istream& contents,
		                                        const std::string& separator=",");
		bool          readCsv                  (const char* contents,
		                                        const std::string& separator=",");
		bool          readCsv                  (const std::string& contents,
		                                        const std::string& separator=",");

		bool          readString               (const char* contents);
		bool          readString               (const std::string& contents);
		bool          readStringCsv            (const char* contents,
		                                        const std::string& separator=",");
		bool          readStringCsv            (const std::string& contents,
		                                        const std::string& separator=",");
		bool          isValid                  (void);
		std::string   getParseError            (void) const;
		bool          isQuiet                  (void) const;
		void          setQuietParsing          (void);
		void          setNoisyParsing          (void);
		void          clear                    (void);
		bool          isStructureAnalyzed      (void);
		bool          isRhythmAnalyzed         (void);
		bool          areStrandsAnalyzed       (void);

    	template <class TYPE>
		   void       initializeArray          (std::vector<std::vector<TYPE>>& array, TYPE value);

		bool          parse                    (std::istream& contents)
		                                    { return read(contents); }
		bool          parse                    (const char* contents)
		                                    { return readString(contents); }
		bool          parse                    (const std::string& contents)
		                                    { return readString(contents); }
		bool          parseCsv                 (std::istream& contents,
		                                        const std::string& separator = ",")
		                                    { return readCsv(contents); }
		bool          parseCsv                 (const char* contents,
		                                        const std::string& separator = ",")
		                                    { return readStringCsv(contents); }
		bool          parseCsv                 (const std::string& contents,
		                                        const std::string& separator = ",")
		                                    { return readStringCsv(contents); }

		void          setXmlIdPrefix           (const std::string& value);
		std::string   getXmlIdPrefix           (void);
		void          setFilename              (const std::string& filename);
		std::string   getFilename              (void);

		void          setSegmentLevel          (int level = 0);
		int           getSegmentLevel          (void);
		std::ostream& printSegmentLabel        (std::ostream& out);
		std::ostream& printNonemptySegmentLabel(std::ostream& out);

		HumdrumLine&  operator[]               (int index);
		HumdrumLine*  getLine                  (int index);
		int           getLineCount             (void) const;
		HTp           token                    (int lineindex, int fieldindex);
		std::string   token                    (int lineindex, int fieldindex,
		                                        int subtokenindex,
		                                        const std::string& separator = " ");
		int           getMaxTrack              (void) const;
		int           getTrackCount            (void) const
		                                                { return getMaxTrack(); }
		int           getSpineCount            (void) const
		                                                { return getMaxTrack(); }
		std::vector<int> getMeasureNumbers     (void);
		int           getMeasureNumber         (int line);
		std::ostream& printSpineInfo           (std::ostream& out = std::cout);
		std::ostream& printDataTypeInfo        (std::ostream& out = std::cout);
		std::ostream& printTrackInfo           (std::ostream& out = std::cout);
		std::ostream& printCsv                 (std::ostream& out = std::cout,
		                                        const std::string& separator = ",");
		std::ostream& printFieldNumber         (int fieldnum, std::ostream& out);
		std::ostream& printFieldIndex          (int fieldind, std::ostream& out);
		void          usage                    (const std::string& command);
		void          example                  (void);

		bool          analyzeNonNullDataTokens (void);
		HTp           getTrackStart            (int track) const;
		void          getSpineStopList         (std::vector<HTp>& spinestops);
		HTp           getSpineStart            (int spine) const
		                                       { return getTrackStart(spine+1); }
		void          getSpineStartList        (std::vector<HTp>& spinestarts);
		void          getSpineStartList        (std::vector<HTp>& spinestarts,
		                                        const std::string& exinterp);
		void          getKernSpineStartList    (std::vector<HTp>& spinestarts);
		std::vector<HTp> getKernSpineStartList (void);
		int           getExinterpCount         (const std::string& exinterp);
		void          getSpineStartList        (std::vector<HTp>& spinestarts,
		                                        const std::vector<std::string>& exinterps);
		void          getTrackStartList        (std::vector<HTp>& spinestarts)
		                               { return getSpineStartList(spinestarts); }
		void          getTrackStartList        (std::vector<HTp>& spinestarts,
		                                        const std::string& exinterp)
		                     { return getSpineStartList(spinestarts, exinterp); }
		void          getTrackStartList        (std::vector<HTp>& spinestarts,
		                                        const std::vector<std::string>& exinterps)
		                    { return getSpineStartList(spinestarts, exinterps); }

		int           getTrackEndCount         (int track) const;
		HTp           getTrackEnd              (int track, int subtrack) const;
		void          createLinesFromTokens    (void);
		void          removeExtraTabs          (void);
		void          addExtraTabs             (void);
		std::vector<int> getTrackWidths        (void);
		void          appendLine               (const char* line);
		void          appendLine               (const std::string& line);
		void          appendLine               (HumdrumLine* line);
		void          push_back                (const char* line)
		                                                    { appendLine(line); }
		void          push_back                (const std::string& line)
		                                                    { appendLine(line); }
		void          push_back                (HumdrumLine* line)
		                                                    { appendLine(line); }

		void          insertLine               (int index, const char* line);
		void          insertLine               (int index, const std::string& line);
		void          insertLine               (int index, HumdrumLine* line);

		void          deleteLine               (int index);
//		void          adjustMergeSpineLines    (void);

		HumdrumLine*  back                     (void);
		void          makeBooleanTrackList     (std::vector<bool>& spinelist,
		                                        const std::string& spinestring);
		bool          analyzeBaseFromLines     (void);
		bool          analyzeBaseFromTokens    (void);


		std::vector<HumdrumLine*> getReferenceRecords(void);
		std::vector<HumdrumLine*> getGlobalReferenceRecords(void);
		std::vector<HumdrumLine*> getUniversalReferenceRecords(void);
		std::string getReferenceRecord(const std::string& key);

		// spine analysis functionality:
		void          getTrackSequence         (std::vector<std::vector<HTp> >& sequence,
		                                        HTp starttoken, int options);
		void          getTrackSequence         (std::vector<std::vector<HTp> >& sequence,
		                                        int track, int options);
		void          getPrimaryTrackSequence  (std::vector<HTp>& sequence,
		                                        int track, int options);

		void          getSpineSequence         (std::vector<std::vector<HTp> >& sequence,
		                                        HTp starttoken, int options);
		void          getSpineSequence         (std::vector<std::vector<HTp> >& sequence,
		                                        int spine, int options);
		void          getPrimarySpineSequence  (std::vector<HTp>& sequence,
		                                        int spine, int options);

		void          getTrackSeq              (std::vector<std::vector<HTp> >& sequence,
		                                        HTp starttoken, int options)
		                     { getTrackSequence(sequence, starttoken, options); }
		void          getTrackSeq              (std::vector<std::vector<HTp> >& sequence,
		                                        int track, int options)
		                          { getTrackSequence(sequence, track, options); }
		void          getPrimaryTrackSeq       (std::vector<HTp>& sequence,
		                                        int track, int options)
		                    {getPrimaryTrackSequence(sequence, track, options); }

		// functions defined in HumdrumFileBase-net.cpp:
		static std::string getUriToUrlMapping        (const std::string& uri);
		void          readFromHumdrumUri        (const std::string& humaddress);
		void          readFromJrpUri            (const std::string& jrpaddress);
		void          readFromHttpUri           (const std::string& webaddress);
		static void   readStringFromHttpUri     (std::stringstream& inputdata,
		                                         const std::string& webaddress);

	protected:
		static int    getChunk                  (int socket_id,
		                                         std::stringstream& inputdata,
		                                         char* buffer, int bufsize);
		static int    getFixedDataSize          (int socket_id,
		                                         int datalength,
		                                         std::stringstream& inputdata,
		                                         char* buffer, int bufsize);
		static void   prepare_address           (struct sockaddr_in *address,
		                                         const std::string& hostname,
		                                         unsigned short int port);
		static int    open_network_socket       (const std::string& hostname,
		                                         unsigned short int port);

	protected:
		bool          analyzeTokens             (void);
		bool          analyzeSpines             (void);
		bool          analyzeLinks              (void);
		bool          analyzeTracks             (void);
		bool          analyzeLines              (void);
		bool          adjustSpines              (HumdrumLine& line,
		                                         std::vector<std::string>& datatype,
		                                         std::vector<std::string>& sinfo);
		std::string   getMergedSpineInfo        (std::vector<std::string>& info,
		                                         int starti, int extra);
		bool          stitchLinesTogether       (HumdrumLine& previous,
		                                         HumdrumLine& next);
		void          addToTrackStarts          (HTp token);
		void          addUniqueTokens           (std::vector<HTp>& target,
		                                         std::vector<HTp>& source);
		bool          processNonNullDataTokensForTrackForward(HTp starttoken,
		                                         std::vector<HTp> ptokens);
		bool          processNonNullDataTokensForTrackBackward(HTp starttoken,
		                                         std::vector<HTp> ptokens);
		bool          setParseError             (std::stringstream& err);
		bool          setParseError             (const std::string& err);
		bool          setParseError             (const char* format, ...);
//		void          fixMerges                 (int linei);

	protected:

		// m_lines: an array representing lines from the input file.
		// The contents of lines must be deallocated when deconstructing object.
		std::vector<HumdrumLine*> m_lines;

		// m_filename: name of the file which was loaded.
		std::string m_filename;

		// m_segementlevel: segment level (e.g., work/movement)
		int m_segmentlevel;

		// m_trackstarts: list of addresses of the exclusive interpreations
		// in the file.  The first element in the list is reserved, so the
		// number of tracks (primary spines) is equal to one less than the
		// size of this list.
		std::vector<HTp> m_trackstarts;

		// m_trackends: list of the addresses of the spine terminators in the
		// file. It is possible that spines can split and their subspines do not
		// merge before termination; therefore, the ends are stored in
		// a 2d array. The first dimension is the track number, and the second
		// dimension is the list of terminators.
		std::vector<std::vector<HTp> > m_trackends;

		// m_barlines: list of barlines in the data.  If the first measures is
		// a pickup measure, then the first entry will not point to the first
		// starting exclusive interpretation line rather than to a barline.
		std::vector<HumdrumLine*> m_barlines;
		// Maybe also add "measures" which are complete metrical cycles.

		// m_ticksperquarternote: this is the number of tick
		int m_ticksperquarternote;

		// m_idprefix: an XML id prefix used to avoid id collisions when
		// includeing multiple HumdrumFile XML in a single group.
		std::string m_idprefix;

		// m_strands1d: one-dimensional list of spine strands.
		std::vector<TokenPair> m_strand1d;

		// m_strands2d: two-dimensional list of spine strands.
		std::vector<std::vector<TokenPair> > m_strand2d;

		// m_quietParse: Set to true if error messages should not be
		// printed to the console when reading.
		bool m_quietParse;

		// m_parseError: Set to true if a read is successful.
		std::string m_parseError;

		// m_displayError: Used to print error message only once.
		bool m_displayError;

		// m_signifiers: Used to keep track of !!!RDF records.
		HumSignifiers m_signifiers;

		// m_structure_analyzed: Used to keep track of whether or not
		// file structure has been analyzed.
		bool m_structure_analyzed = false;

		// m_rhythm_analyzed: Used to keep track of whether or not
		// rhythm structure has been analyzed.
		bool m_rhythm_analyzed = false;

		// m_strands_analyzed: Used to keep track of whether or not
		// file strands have been analyzed.
		bool m_strands_analyzed = false;

		// m_slurs_analyzed: Used to keep track of whether or not
		// slur endpoints have been linked or not.
		bool m_slurs_analyzed = false;

		// m_nulls_analyzed: Used to keep track of wheter or not
		// null tokens have been analyzed yet.
		bool m_nulls_analyzed = false;

	public:
		// Dummy functions to allow the HumdrumFile class's inheritance
		// to be shifted between HumdrumFileContent (the top-level default),
		// HumdrumFileStructure (mid-level interface), or HumdrumFileBase
		// (low-level interface).

		//
		// HumdrumFileStructure public functions:
		//
		bool readNoRhythm      (std::istream& infile) { return read(infile); };
		bool readNoRhythm      (const char*   filename) {return read(filename);};
		bool readNoRhythm      (const std::string& filename) {return read(filename);};
		bool readStringNoRhythm(const char*   contents) {return read(contents);};
		bool readStringNoRhythm(const std::string& contents) {return read(contents);};
		HumNum       getScoreDuration           (void) const { return 0; };
		std::ostream&     printDurationInfo          (std::ostream& out=std::cout) {return out;};
		int          tpq                        (void) { return 0; }
		int          getBarlineCount            (void) const { return 0; }
		HumdrumLine* getBarline                 (int index) const { return NULL;};
		HumNum       getBarlineDuration         (int index) const { return 0; };
		HumNum       getBarlineDurationFromStart(int index) const { return 0; };
		HumNum       getBarlineDurationToEnd    (int index) const { return 0; };

		// HumdrumFileContent public functions:
		// to be added later

};

std::ostream& operator<<(std::ostream& out, HumdrumFileBase& infile);



class HumdrumFileStructure : public HumdrumFileBase {
	public:
		              HumdrumFileStructure         (void);
		              HumdrumFileStructure         (const std::string& filename);
		              HumdrumFileStructure         (std::istream& contents);
		             ~HumdrumFileStructure         ();
		bool          hasFilters                   (void);
		bool          hasGlobalFilters             (void);
		bool          hasUniversalFilters          (void);

		// TSV reading functions:
		bool          read                         (std::istream& contents);
		bool          read                         (const char* filename);
		bool          read                         (const std::string& filename);
		bool          readString                   (const char* contents);
		bool          readString                   (const std::string& contents);
		bool parse(std::istream& contents)      { return read(contents); }
		bool parse(const char* contents)   { return readString(contents); }
		bool parse(const std::string& contents) { return readString(contents); }
		bool          readNoRhythm                 (std::istream& contents);
		bool          readNoRhythm                 (const char* filename);
		bool          readNoRhythm                 (const std::string& filename);
		bool          readStringNoRhythm           (const char* contents);
		bool          readStringNoRhythm           (const std::string& contents);

		// CSV reading functions:
		bool          readCsv                      (std::istream& contents,
		                                            const std::string& separator=",");
		bool          readCsv                      (const char* filename,
		                                            const std::string& separator=",");
		bool          readCsv                      (const std::string& filename,
		                                            const std::string& separator=",");
		bool          readStringCsv                (const char* contents,
		                                            const std::string& separator=",");
		bool          readStringCsv                (const std::string& contents,
		                                            const std::string& separator=",");
		bool parseCsv(std::istream& contents, const std::string& separator = ",")
		                                 { return readCsv(contents, separator); }
		bool parseCsv(const char* contents, const std::string& separator = ",")
		                           { return readStringCsv(contents, separator); }
		bool parseCsv(const std::string& contents, const std::string& separator = ",")
		                           { return readStringCsv(contents, separator); }
		bool          readNoRhythmCsv              (std::istream& contents,
		                                            const std::string& separator = ",");
		bool          readNoRhythmCsv              (const char* filename,
		                                            const std::string& separator = ",");
		bool          readNoRhythmCsv              (const std::string& filename,
		                                            const std::string& separator = ",");
		bool          readStringNoRhythmCsv        (const char* contents,
		                                            const std::string& separator = ",");
		bool          readStringNoRhythmCsv        (const std::string& contents,
		                                            const std::string& separator = ",");

		// rhythmic analysis related functionality:
		HumNum        getScoreDuration             (void) const;
		std::ostream&      printDurationInfo       (std::ostream& out = std::cout);
		int           tpq                          (void);

		// strand functionality:
		HTp           getStrandStart               (int index);
		HTp           getStrandEnd                 (int index);
		HTp           getStrandStart               (int sindex, int index);
		HTp           getStrandEnd                 (int sindex, int index);
		int           getStrandCount               (void);
		int           getStrandCount               (int spineindex);
		void          resolveNullTokens            (void);

		HTp           getStrand                    (int index)
		                                        { return getStrandStart(index); }
		HTp           getStrand                    (int sindex, int index)
		                                { return getStrandStart(sindex, index); }

		// barline/measure functionality:
		int           getBarlineCount              (void) const;
		HumdrumLine*  getBarline                   (int index) const;
		HumNum        getBarlineDuration           (int index) const;
		HumNum        getBarlineDurationFromStart  (int index) const;
		HumNum        getBarlineDurationToEnd      (int index) const;

		bool          analyzeStructure             (void);
		bool          analyzeStructureNoRhythm     (void);
		bool          analyzeRhythmStructure       (void);
		bool          analyzeStrands               (void);

		// signifier access
		std::string   getKernLinkSignifier         (void);
		std::string   getKernAboveSignifier        (void);
		std::string   getKernBelowSignifier        (void);

	protected:
		bool          analyzeRhythm                (void);
		bool          assignRhythmFromRecip        (HTp spinestart);
		bool          analyzeMeter                 (void);
		bool          analyzeTokenDurations        (void);
		bool          analyzeGlobalParameters      (void);
		bool          analyzeLocalParameters       (void);
		bool          analyzeDurationsOfNonRhythmicSpines(void);
		HumNum        getMinDur                    (std::vector<HumNum>& durs,
		                                            std::vector<HumNum>& durstate);
		bool          getTokenDurations            (std::vector<HumNum>& durs,
		                                            int line);
		bool          cleanDurs                    (std::vector<HumNum>& durs,
		                                            int line);
		bool          decrementDurStates           (std::vector<HumNum>& durs,
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
		std::set<HumNum>   getNonZeroLineDurations      (void);
		std::set<HumNum>   getPositiveLineDurations     (void);
		void          processLocalParametersForStrand(int index);
		bool          processLocalParametersForTrack (HTp starttok, HTp current);
		void          checkForLocalParameters      (HTp token, HTp current);
		bool          assignDurationsToNonRhythmicTrack(HTp endtoken, HTp ptoken);
		void          analyzeSpineStrands          (std::vector<TokenPair>& ends,
		                                            HTp starttok);
		void          analyzeSignifiers            (void);
		void          setLineRhythmAnalyzed        (void);
};



class HumdrumFileContent : public HumdrumFileStructure {
	public:
		       HumdrumFileContent         (void);
		       HumdrumFileContent         (const std::string& filename);
		       HumdrumFileContent         (std::istream& contents);
		      ~HumdrumFileContent         ();

		bool   analyzeSlurs               (void);
	private:
		bool   analyzeMensSlurs           (void);
		bool   analyzeKernSlurs           (void);
	public:
		bool   analyzeKernTies            (void);
		bool   analyzeKernAccidentals     (void);

		bool   analyzeRScale              (void);

		// in HumdrumFileContent-rest.cpp
		void  analyzeRestPositions                  (void);
		void  assignImplicitVerticalRestPositions   (HTp kernstart);
		void  checkForExplicitVerticalRestPositions (void);

		// in HumdrumFileContent-stem.cpp
		bool analyzeKernStemLengths       (void);

		// in HumdrumFileContent-metlev.cpp
		void  getMetricLevels             (std::vector<double>& output, int track = 0,
		                                   double undefined = NAN);
		// in HumdrumFileContent-timesig.cpp
		void  getTimeSigs                 (std::vector<std::pair<int, HumNum> >& output,
		                                   int track = 0);

		template <class DATATYPE>
		bool   prependDataSpine           (std::vector<DATATYPE> data,
		                                   const std::string& null = ".",
		                                   const std::string& exinterp = "**data",
		                                   bool recalcLine = true);

		template <class DATATYPE>
		bool   appendDataSpine            (std::vector<DATATYPE> data,
		                                   const std::string& null = ".",
		                                   const std::string& exinterp = "**data",
		                                   bool recalcLine = true);

		template <class DATATYPE>
		bool   insertDataSpineBefore      (int nexttrack,
		                                   std::vector<DATATYPE> data,
		                                   const std::string& null = ".",
		                                   const std::string& exinterp = "**data",
		                                   bool recalcLine = true);

		template <class DATATYPE>
		bool   insertDataSpineAfter       (int prevtrack,
		                                   std::vector<DATATYPE> data,
		                                   const std::string& null = ".",
		                                   const std::string& exinterp = "**data",
		                                   bool recalcLine = true);

		// in HumdrumFileContent-ottava.cpp
		void   analyzeOttavas             (void);

		// in HumdrumFileContent-note.cpp
		void   analyzeCrossStaffStemDirections (void);
		void   analyzeCrossStaffStemDirections (HTp kernstart);

		int    hasPickup                  (void);

	protected:
		bool   analyzeKernSlurs           (HTp spinestart, std::vector<HTp>& slurstarts,
		                                   std::vector<HTp>& slurends,
		                                   std::vector<std::pair<HTp, HTp>>& labels,
		                                   std::vector<int>& endings,
		                                   const std::string& linksig = "");
		bool   analyzeKernTies            (std::vector<std::pair<HTp, int>>& linkedtiestarts,
		                                   std::vector<std::pair<HTp, int>>& linkedtieends,
		                                   std::string& linkSignifier);
		void   fillKeySignature           (std::vector<int>& states,
		                                   const std::string& keysig);
		void   resetDiatonicStatesWithKeySignature(std::vector<int>& states,
				                             std::vector<int>& signature);
		void    linkSlurEndpoints         (HTp slurstart, HTp slurend);
		void    linkTieEndpoints          (HTp tiestart, int startindex,
		                                   HTp tieend, int endindex);
		bool    isLinkedSlurBegin         (HTp token, int index, const std::string& pattern);
		bool    isLinkedSlurEnd           (HTp token, int index, const std::string& pattern);
		void    createLinkedSlurs         (std::vector<HTp>& linkstarts, std::vector<HTp>& linkends);
		void    assignVerticalRestPosition(HTp first, HTp second, int baseline);
		int     getRestPositionAboveNotes (HTp rest, std::vector<int>& vpos);
		int     getRestPositionBelowNotes (HTp rest, std::vector<int>& vpos);
		void    setRestOnCenterStaffLine  (HTp rest, int baseline);
		bool    checkRestForVerticalPositioning(HTp rest, int baseline);
		bool    analyzeKernStemLengths    (HTp stok, HTp etok, std::vector<std::vector<int>>& centerlines);
		void    getBaselines              (std::vector<std::vector<int>>& centerlines);
		void    createLinkedTies          (std::vector<std::pair<HTp, int>>& starts,
		                                   std::vector<std::pair<HTp, int>>& ends);
		void    checkCrossStaffStems      (HTp token, std::string& above, std::string& below);
		void    checkDataForCrossStaffStems(HTp token, std::string& above, std::string& below);
		void    prepareStaffAboveNoteStems (HTp token);
		void    prepareStaffBelowNoteStems (HTp token);
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
bool HumdrumFileContent::prependDataSpine(std::vector<DATATYPE> data,
		const std::string& null, const std::string& exinterp, bool recalcLine) {

	if ((int)data.size() != getLineCount()) {
		return false;
	}

	std::string ex;
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

	std::stringstream ss;
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
			line->insertToken(0, (std::string)*infile.token(i, 0));
		} else if (line->isData()) {
			ss.str(std::string());
			ss << data[i];
			if (ss.str() == null) {
				line->insertToken(0, ".");
			} else if (ss.str() == "") {
				line->insertToken(0, ".");
			} else {
				line->insertToken(0, ss.str());
			}
		} else{
			std::cerr << "!!strange error for line " << i+1 << ":\t" << line << std::endl;
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
bool HumdrumFileContent::appendDataSpine(std::vector<DATATYPE> data,
		const std::string& null, const std::string& exinterp, bool recalcLine) {

	if ((int)data.size() != getLineCount()) {
		std::cerr << "DATA SIZE DOES NOT MATCH GETLINECOUNT " << std::endl;
		std::cerr << "DATA SIZE " << data.size() << "\tLINECOUNT ";
		std::cerr  << getLineCount() << std::endl;
		return false;
	}

	std::string ex;
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

	std::stringstream ss;
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
			line->appendToken((std::string)*infile.token(i, 0));
		} else if (line->isData()) {
			ss.str(std::string());
			ss << data[i];
			if (ss.str() == null) {
				line->appendToken(".");
			} else if (ss.str() == "") {
				line->appendToken(".");
			} else {
				line->appendToken(ss.str());
			}
		} else{
			std::cerr << "!!strange error for line " << i+1 << ":\t" << line << std::endl;
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
		std::vector<DATATYPE> data, const std::string& null, const std::string& exinterp,
		bool recalcLine) {

	if ((int)data.size() != getLineCount()) {
		std::cerr << "DATA SIZE DOES NOT MATCH GETLINECOUNT " << std::endl;
		std::cerr << "DATA SIZE " << data.size() << "\tLINECOUNT ";
		std::cerr  << getLineCount() << std::endl;
		return false;
	}

	std::string ex;
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

	std::stringstream ss;
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
			line->insertToken(insertionField, (std::string)*infile.token(i, 0));
		} else if (line->isData()) {
			ss.str(std::string());
			ss << data[i];
			if (ss.str() == null) {
				line->insertToken(insertionField, ".");
			} else if (ss.str() == "") {
				line->insertToken(insertionField, ".");
			} else {
				line->insertToken(insertionField, ss.str());
			}
		} else{
			std::cerr << "!!strange error for line " << i+1 << ":\t" << line << std::endl;
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
		std::vector<DATATYPE> data, const std::string& null, const std::string& exinterp,
		bool recalcLine) {

	if ((int)data.size() != getLineCount()) {
		std::cerr << "DATA SIZE DOES NOT MATCH GETLINECOUNT " << std::endl;
		std::cerr << "DATA SIZE " << data.size() << "\tLINECOUNT ";
		std::cerr  << getLineCount() << std::endl;
		return false;
	}

	std::string ex;
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

	std::stringstream ss;
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
			line->insertToken(insertionField, (std::string)*infile.token(i, 0));
		} else if (line->isData()) {
			ss.str(std::string());
			ss << data[i];
			if (ss.str() == null) {
				line->insertToken(insertionField, ".");
			} else if (ss.str() == "") {
				line->insertToken(insertionField, ".");
			} else {
				line->insertToken(insertionField, ss.str());
			}
		} else{
			std::cerr << "!!strange error for line " << i+1 << ":\t" << line << std::endl;
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
		              HumdrumFile          (const std::string& filename);
		              HumdrumFile          (std::istream& filename);
		             ~HumdrumFile          ();

		std::ostream& printXml             (std::ostream& out = std::cout, int level = 0,
		                                    const std::string& indent = "\t");
		std::ostream& printXmlParameterInfo(std::ostream& out, int level,
		                                    const std::string& indent);
};



// Reference:     Beyond Midi, page 410.
#define E_muserec_note_regular       'N'
	//                                'A' --> use type E_muserec_note_regular
	//                                'B' --> use type E_muserec_note_regular
	//                                'C' --> use type E_muserec_note_regular
	//                                'D' --> use type E_muserec_note_regular
	//                                'E' --> use type E_muserec_note_regular
	//                                'F' --> use type E_muserec_note_regular
	//                                'G' --> use type E_muserec_note_regular
#define E_muserec_note_chord         'C'
#define E_muserec_note_cue           'c'
#define E_muserec_note_grace         'g'
#define E_muserec_note_grace_chord   'G'
#define E_muserec_print_suggestion   'P'
#define E_muserec_sound_directives   'S'
#define E_muserec_end                '/'
#define E_muserec_endtext            'T'
#define E_muserec_append             'a'
#define E_muserec_backspace          'b'
#define E_muserec_back               'b'
#define E_muserec_backward           'b'
#define E_muserec_figured_harmony    'f'
#define E_muserec_rest_invisible     'i'
#define E_muserec_forward            'i'
#define E_muserec_measure            'm'
#define E_muserec_rest               'r'
#define E_muserec_musical_attributes '$'
#define E_muserec_comment_toggle     '&'
#define E_muserec_comment_line       '@'
#define E_muserec_musical_directions '*'
#define E_muserec_copyright          '1'  // reserved for copyright notice
#define E_muserec_header_1           '1'  // reserved for copyright notice
#define E_muserec_header_2           '2'  // reserved for identification
#define E_muserec_id                 '2'  // reserved for identification
#define E_muserec_header_3           '3'  // reserved
#define E_muserec_header_4           '4'  // <date> <name of encoder>
#define E_muserec_encoder            '4'  // <date> <name of encoder>
#define E_muserec_header_5           '5'  // WK#:<work number> MV#:<mvmt num>
#define E_muserec_work_info          '5'  // WK#:<work number> MV#:<mvmt num>
#define E_muserec_header_6           '6'  // <source>
#define E_muserec_source             '6'  // <source>
#define E_muserec_header_7           '7'  // <work title>
#define E_muserec_work_title         '7'  // <work title>
#define E_muserec_header_8           '8'  // <movement title>
#define E_muserec_movement_title     '8'  // <movement title>
#define E_muserec_header_9           '9'  // <name of part>
#define E_muserec_header_part_name   '9'  // <name of part>
#define E_muserec_header_10          '0'  // misc designations
#define E_muserec_header_11          'A'  // group memberships
#define E_muserec_group_memberships  'A'  // group memberships
// multiple musered_head_12 lines can occur:
#define E_muserec_header_12          'B'  // <name1>: part <x> of <num in group>
#define E_muserec_group              'B'  // <name1>: part <x> of <num in group>
#define E_muserec_unknown            'U'  // unknown record type
#define E_muserec_empty              'E'  // nothing on line and not header
	                                       // or multi-line comment
#define E_muserec_deleted            'D'  // deleted line
// non-standard record types for MuseDataSet
#define E_muserec_filemarker         '+'
#define E_muserec_filename           'F'
#define E_musrec_header               1000
#define E_musrec_footer               2000


class MuseRecordBasic {
	public:
		                  MuseRecordBasic    (void);
		                  MuseRecordBasic    (const std::string& aLine, int index = -1);
		                  MuseRecordBasic    (MuseRecordBasic& aRecord);
		                 ~MuseRecordBasic    ();

		void              clear              (void);
		int               isEmpty            (void);
		void              cleanLineEnding    (void);
		std::string       extract            (int start, int stop);
		char&             getColumn          (int index);
		std::string       getColumns         (int startcol, int endcol);
		void              setColumns         (std::string& data, int startcol,
		                                      int endcol);
		int               getLength          (void) const;
		std::string       getLine            (void);
		int               getLineIndex       (void) { return m_lineindex; }
		void              setLineIndex       (int index);
		int               getLineNumber      (void) { return m_lineindex+1; }
		int               getType            (void) const;
		void              setTypeGraceNote   (void);
		void              setTypeGraceChordNote(void);
		void              setHeaderState     (int state);

		// Humdrum conversion variables
		void              setToken           (HTp token);
		HTp               getToken           (void);
		void              setVoice           (GridVoice* voice);
		GridVoice*        getVoice           (void);

		MuseRecordBasic&  operator=          (MuseRecordBasic& aRecord);
		MuseRecordBasic&  operator=          (MuseRecordBasic* aRecord);
		MuseRecordBasic&  operator=          (const std::string& aRecord);
		char&             operator[]         (int index);
		void              setLine            (const std::string& aString);
		void              setType            (int aType);
		void              shrink             (void);
		void              insertString       (int column, const std::string& strang);
		void              insertStringRight  (int column, const std::string& strang);
		void              setString          (std::string& strang);
		void              appendString       (const std::string& strang);
		void              appendInteger      (int value);
		void              appendRational     (HumNum& value);
		void              append             (const char* format, ...);

		// mark-up accessor functions:

		void              setAbsBeat         (HumNum value);
		void              setAbsBeat         (int topval, int botval = 1);
		HumNum            getAbsBeat         (void);

		void              setLineDuration    (HumNum value);
		void              setLineDuration    (int topval, int botval = 1);
		HumNum            getLineDuration    (void);

		void              setNoteDuration    (HumNum value);
		void              setNoteDuration    (int topval, int botval = 1);
		HumNum            getNoteDuration    (void);
		void              setRoundedBreve    (void);

		void              setMarkupPitch     (int aPitch);
		int               getMarkupPitch     (void);

		void              setLayer           (int layer);
		int               getLayer           (void);

		// tied note functions:
		int               isTied                  (void);
		int               getLastTiedNoteLineIndex(void);
		int               getNextTiedNoteLineIndex(void);
		void              setLastTiedNoteLineIndex(int index);
		void              setNextTiedNoteLineIndex(int index);

		std::string       getLayoutVis       (void);

		// boolean type fuctions:
		bool              isAnyNote          (void);
		bool              isAnyNoteOrRest    (void);
		bool              isAttributes       (void);
		bool              isBackup           (void);
		bool              isBarline          (void);
		bool              isBodyRecord       (void);
		bool              isChordGraceNote   (void);
		bool              isChordNote        (void);
		bool              isDirection        (void);
		bool              isAnyComment       (void);
		bool              isLineComment      (void);
		bool              isBlockComment     (void);
		bool              isCopyright        (void);
		bool              isCueNote          (void);
		bool              isEncoder          (void);
		bool              isFiguredHarmony   (void);
		bool              isGraceNote        (void);
		bool              isGroup            (void);
		bool              isGroupMembership  (void);
		bool              isHeaderRecord     (void);
		bool              isId               (void);
		bool              isMovementTitle    (void);
		bool              isPartName         (void);
		bool              isRegularNote      (void);
		bool              isAnyRest          (void);
		bool              isRegularRest      (void);
		bool              isInvisibleRest    (void);
		bool              isSource           (void);
		bool              isWorkInfo         (void);
		bool              isWorkTitle        (void);
		bool              hasTpq             (void);
		int               getTpq             (void);
		void              setTpq             (int value);
		static std::string musedataToUtf8    (std::string& input);

	protected:
		std::string       m_recordString;    // actual characters on line

		// mark-up data for the line:
		int               m_lineindex;       // index into original file
		int               m_type;            // category of MuseRecordBasic record
		HumNum            m_absbeat;         // dur in quarter notes from start
		HumNum            m_lineduration;    // duration of line
		HumNum            m_noteduration;    // duration of note

		int               m_b40pitch;        // base 40 pitch
		int               m_nexttiednote;    // line number of next note tied to
		                                     // this one (-1 if no tied note)
		int               m_lasttiednote;    // line number of previous note tied
		                                     // to this one (-1 if no tied note)
		int               m_roundBreve;
		int               m_header = -1;     // -1 = undefined, 0 = no, 1 = yes
		int               m_layer = 0;       // voice/layer (track info but may be analyzed)
		int               m_tpq = 0;         // ticks-per-quarter for durations
		std::string       m_graphicrecip;    // graphical duration of note/rest
		GridVoice*			m_voice = NULL;    // conversion structure that token is stored in.

	public:
		static std::string       trimSpaces         (std::string input);
};


std::ostream& operator<<(std::ostream& out, MuseRecordBasic& aRecord);
std::ostream& operator<<(std::ostream& out, MuseRecordBasic* aRecord);





class MuseRecord : public MuseRecordBasic {
	public:
		                  MuseRecord                  (void);
		                  MuseRecord                  (const std::string& aLine);
		                  MuseRecord                  (MuseRecord& aRecord);
		                 ~MuseRecord                  ();

		MuseRecord& operator=(MuseRecord& aRecord);

	//////////////////////////////
	// functions which work with regular note, cue note, and grace note records
	// (A..G, c, g)

		// columns 1 -- 5: pitch field information
		std::string      getNoteField                 (void);
		int              getOctave                    (void);
		std::string      getOctaveString              (void);
		int              getPitch                     (void);
		std::string      getPitchString               (void);
		int              getPitchClass                (void);
		std::string      getPitchClassString          (void);
		int              getAccidental                (void);
		std::string      getAccidentalString          (void);
		int              getBase40                    (void);
		void             setPitch                     (int base40, int chordnote = 0,
		                                               int gracenote = 0);
		void             setPitch                     (const std::string& pitchname);
		void             setPitchAtIndex              (int index,
		                                               const std::string& pitchname);
		void             setChordPitch                (const std::string& pitchname);
		void             setGracePitch                (const std::string& pitchname);
		void             setGraceChordPitch           (const std::string& pitchname);
		void             setCuePitch                  (const std::string& pitchname);
		void             setStemDown                  (void);
		void             setStemUp                    (void);

		// columns 6 -- 9: duration field information
		std::string      getTickDurationField         (void);
		std::string      getTickDurationString        (void);
		int              getTickDuration              (void);
		int              getLineTickDuration          (void);
		int              getNoteTickDuration          (void);
		std::string      getTieString                 (void);
		int              getTie                       (void);
		int              setTie                       (int hidden = 0);
		int              tieQ                         (void);
		int              getTicks                     (void);
		void             setTicks                     (int value);
		void             setBack                      (int value);
		void             setDots                      (int value);
		int              getDotCount                  (void);
		void             setNoteheadShape             (HumNum duration);
		void             setNoteheadShapeMensural     (HumNum duration);
		void             setNoteheadMaxima            (void);
		void             setNoteheadLong              (void);
		void             setNoteheadBreve             (void);
		void             setNoteheadBreveSquare       (void);
		void             setNoteheadBreveRound        (void);

		void             setNoteheadWhole             (void);
		void             setNoteheadHalf              (void);
		void             setNoteheadQuarter           (void);
		void             setNotehead8th               (void);
		void             setNotehead16th              (void);
		void             setNotehead32nd              (void);
		void             setNotehead64th              (void);
		void             setNotehead128th             (void);
		void             setNotehead256th             (void);

		void             setNoteheadBreveMensural     (void);
		void             setNoteheadWholeMensural     (void);
		void             setNoteheadHalfMensural      (void);
		void             setNoteheadQuarterMensural   (void);
		void             setNotehead8thMensural       (void);
		void             setNotehead16thMensural      (void);
		void             setNotehead32ndMensural      (void);
		void             setNotehead64thMensural      (void);
		void             setNotehead128thMensural     (void);
		void             setNotehead256thMensural     (void);

		// columns 10 -- 12 ---> blank

		// columns 13 -- 80: graphical and interpretive information

		// column 13: footnote flag
		std::string      getFootnoteFlagField         (void);
		std::string      getFootnoteFlagString        (void);
		int              getFootnoteFlag              (void);
		int              footnoteFlagQ                (void);

		// column 14: level number
		std::string      getLevelField                (void);
		std::string      getLevelString               (void);
		int              getLevel                     (void);
		int              levelQ                       (void);

		// column 15: track number
		std::string      getTrackField                (void);
		std::string      getTrackString               (void);
		int              getTrack                     (void);
		int              trackQ                       (void);

		// column 16 ---> blank

		// column 17: graphic note type
		std::string      getGraphicNoteTypeField      (void);
		std::string      getGraphicNoteTypeString     (void);
		int              getGraphicNoteType           (void);
		int              getGraphicNoteTypeSize       (void);
		int              graphicNoteTypeQ             (void);
		std::string      getGraphicRecip              (void);

		// column 18: dots of prolongation
		std::string      getProlongationField         (void);
		std::string      getProlongationString        (void);
		int              getProlongation              (void);
		std::string      getStringProlongation        (void);
		int              prolongationQ                (void);

		// column 19: actual notated accidentals
		std::string      getNotatedAccidentalField    (void);
		std::string      getNotatedAccidentalString   (void);
		int              getNotatedAccidental         (void);
		int              notatedAccidentalQ           (void);

		// columns 20 -- 22: time modification
		std::string      getTimeModificationField     (void);
		std::string      getTimeModification          (void);
		std::string      getTimeModificationLeftField (void);
		std::string      getTimeModificationLeftString(void);
		int              getTimeModificationLeft      (void);
		std::string      getTimeModificationRightField(void);
		std::string      getTimeModificationRightString(void);
		int              getTimeModificationRight     (void);
		int              timeModificationQ            (void);
		int              timeModificationLeftQ        (void);
		int              timeModificationRightQ       (void);

		// column 23
		std::string      getStemDirectionField        (void);
		std::string      getStemDirectionString       (void);
		int              getStemDirection             (void);
		int              stemDirectionQ               (void);

		// column 24
		std::string      getStaffField                (void);
		std::string      getStaffString               (void);
		int              getStaff                     (void);
		int              staffQ                       (void);
		
		// column 25 ---> blank

		// columns 26 - 31: beam codes
		std::string      getBeamField                 (void);
		int              beamQ                        (void);
		char             getBeam8                     (void);
		char             getBeam16                    (void);
		char             getBeam32                    (void);
		char             getBeam64                    (void);
		char             getBeam128                   (void);
		char             getBeam256                   (void);
		int              beam8Q                       (void);
		int              beam16Q                      (void);
		int              beam32Q                      (void);
		int              beam64Q                      (void);
		int              beam128Q                     (void);
		int              beam256Q                     (void);
		std::string      getKernBeamStyle             (void);
		void             setBeamInfo                  (std::string& strang);

		// columns 32 -- 43: additional notation
		std::string      getAdditionalNotationsField  (void);
		int              additionalNotationsQ         (void);
		int              getAddCount                  (void);
		std::string      getAddItem                   (int elementIndex);
		int              addAdditionalNotation        (char symbol);
		int              addAdditionalNotation        (const std::string& symbol);
		int              getAddItemLevel              (int elementIndex);
		std::string      getEditorialLevels           (void);
		int              addEditorialLevelQ           (void);
		//  protected:   getAddElementIndex
		int              findField                    (const std::string& key);
		int              findField                    (char key, int mincol,
		                                               int maxcol);
		// int           getNotationLevel
		int              getSlurStartColumn           (void);
		std::string      getSlurParameterRegion       (void);
		void             getSlurInfo                  (std::string& slurstarts,
		                                               std::string& slurends);

		// columns 44 -- 80: text underlay
		std::string      getTextUnderlayField         (void);
		int              textUnderlayQ                (void);
		int              getVerseCount                (void);
		std::string      getVerse                     (int index);
		std::string      getVerseUtf8                 (int index);

		// general functions for note records:
		std::string      getKernNoteStyle             (int beams = 0, int stems = 0);
		std::string      getKernNoteAccents           (void);


	//////////////////////////////
	// functions which work with basso continuo figuration records ('f'):

		// column 2: number of figure fields
		std::string      getFigureCountField          (void);
		std::string      getFigureCountString         (void);
		int              getFigureCount               (void);

		// columns 3 -- 5 ---> blank
		
		// columns 6 -- 8: figure division pointer advancement (duration)
		std::string      getFigurePointerField        (void);
		int              figurePointerQ               (void);
		// same as note records: getDuration

		// columns 9 -- 12 ---> blank

		// columns 13 -- 15: footnote and level information
		// column 13 --> footnote: uses same functions as note records in col 13.
		// column 14 --> level: uses same functions as note records on column 14.
		// column 15 ---> blank

		// columns 17 -- 80: figure fields
		std::string      getFigureFields              (void);
		std::string      getFigureString              (void);
		int              figureFieldsQ                (void);
		std::string      getFigure                    (int index = 0);


	//////////////////////////////
	// functions which work with combined records ('b', 'i'):


	//////////////////////////////
	// functions which work with measure records ('m'):

		// columns 1 -- 7: measure style information
		std::string      getMeasureTypeField          (void);

		// columns 9 -- 12: measure number (left justified)
		std::string      getMeasureNumberField        (void);
		std::string      getMeasureNumberString       (void);
		int              getMeasureNumber             (void);
		int              measureNumberQ               (void);

		// columns 17 -- 80: measure flags
		std::string      getMeasureFlagsString        (void);
		int              measureFermataQ              (void);
		int              measureFlagQ                 (const std::string& key);
		void             addMeasureFlag               (const std::string& strang);

		// general functions for measure records:
		std::string      getKernMeasureStyle          (void);


	//////////////////////////////
	// functions which work with musical attributes records ('$'):

		std::string      getAttributes                (void);
		void             getAttributeMap              (std::map<std::string, std::string>& amap);
		int              attributeQ                   (const std::string& attribute);
		int              getAttributeInt              (char attribute);
		int              getAttributeField            (std::string& output, const std::string& attribute);

	//////////////////////////////
	// functions which work with musical direction records ('$'):

		// columns 17-18: type of direction
		std::string      getDirectionTypeField        (void);
		std::string      getDirectionTypeString       (void);
		bool             isTextDirection              (void);
		bool             isHairpin                    (void);
		bool             isHairpinStart               (void);
		bool             isHairpinStop                (void);
		bool             isDashStart                  (void);
		bool             isDashStop                   (void);
		bool             isPedalStart                 (void);
		bool             isPedalEnd                   (void);
		bool             isRehearsal                  (void);
		bool             isOctaveUpStart              (void);
		bool             isOctaveDownStart            (void);
		bool             isOctaveStop                 (void);

		std::string      getDirectionText             (void);
		std::string      getTextDirection             (void) { return getDirectionText(); }

	//
	//////////////////////////////

		std::string      getKernRestStyle             (void);

	protected:
		void             allowNotesOnly               (const std::string& functionName);
		void             allowNotesAndRestsOnly       (const std::string& functionName);
		void             allowMeasuresOnly            (const std::string& functioName);
		void             allowFigurationOnly          (const std::string& functioName);
		void             allowFigurationAndNotesOnly  (const std::string& functioName);
		void             allowDirectionsOnly          (const std::string& functioName);
		int              getAddElementIndex           (int& index, std::string& output,
		                                               const std::string& input);
		void             zerase                       (std::string& inout, int num);
};




// A MuseEventSet is a timestamp and then a list of pointers to all
// lines in the original file that occur at that time.
// The MuseData class contains a variable called "sequence" which is
// a list of MuseEventSet object pointers which are sorted by time.

class MuseEventSet {
	public:
		                   MuseEventSet       (void);
		                   MuseEventSet       (const MuseEventSet& aSet);
		                   MuseEventSet       (HumNum atime);
		                  ~MuseEventSet       ()     { clear(); }

		void               clear              (void);
		void               setTime            (HumNum abstime);
		HumNum             getTime            (void);
		void               appendRecord       (MuseRecord* arecord);
		MuseRecord&        operator[]         (int index);
		MuseEventSet       operator=          (MuseEventSet& anevent);
		int                getEventCount      (void);

	protected:
		HumNum     absbeat;              // starting time of events
		std::vector<MuseRecord*> events; // list of events on absbeat
};



class MuseData {
	public:
		                  MuseData            (void);
		                  MuseData            (MuseData& input);
		                 ~MuseData            ();

		void              setFilename         (const std::string& filename);
		std::string       getFilename         (void);
		std::string       getPartName         (void);
		int               isMember            (const std::string& mstring);
		int               getMembershipPartNumber(const std::string& mstring);
		void              selectMembership    (const std::string& selectstring);
		MuseData&         operator=           (MuseData& input);
		int               getLineCount        (void);
		int               getNumLines         (void) { return getLineCount(); }
		MuseRecord&       last                (void);
		int               isEmpty             (void);
		int               append              (MuseRecord& arecord);
		int               append              (MuseData& musedata);
		int               append              (std::string& charstring);
		void              insert              (int index, MuseRecord& arecord);
		void              clear               (void);
		int               getInitialTpq       (void);

		int               read                (std::istream& input);
		int               readString          (const std::string& filename);
		int               readFile            (const std::string& filename);
		void              analyzeLayers       (void);
		int               analyzeLayersInMeasure(int startindex);

		// aliases for access to MuseRecord objects based on line indexes:
		std::string       getLine             (int index);

		bool              isCopyright         (int index);
		bool              isEncoder           (int index);
		bool              isId                (int index);
		bool              isMovementTitle     (int index);
		bool              isAnyNote           (int index);
		bool              isRegularNote       (int index);
		bool              isPartName          (int index);
		bool              isSource            (int index);
		bool              isWorkInfo          (int index);
		bool              isWorkTitle         (int index);
		bool              isHeaderRecord      (int index);
		bool              isBodyRecord        (int index);

		// header information
		std::string       getComposer         (void);
		std::string       getComposerDate     (void);
		std::string       getCopyright        (void);
		std::string       getEncoder          (void);
		std::string       getEncoderDate      (void);
		std::string       getEncoderName      (void);
		std::string       getId               (void);
		std::string       getMovementTitle    (void);
		std::string       getSource           (void);
		std::string       getWorkInfo         (void);
		std::string       getWorkTitle        (void);
		std::string       getOpus             (void);
		std::string       getNumber           (void);
		std::string       getMovementNumber   (void);

		// additional mark-up analysis functions for post-processing:
		void              doAnalyses          (void);
		void              analyzeType         (void);
		void              analyzeRhythm       (void);
		void              analyzeTies         (void);
		void              analyzePitch        (void);
		void              analyzeTpq          (void);

		// line-based (file-order indexing) accessor functions:
		MuseRecord&       operator[]          (int lindex);
		MuseRecord&       getRecord           (int lindex);
		HumNum            getTiedDuration     (int lindex);

		HumNum            getAbsBeat         (int lindex);
		HumNum            getFileDuration    (void);

		int               getLineTickDuration (int lindex);

		// event-based (time-order indexing) accessor functions:
		MuseEventSet&     getEvent            (int eindex);
		int               getEventCount       (void);
		HumNum            getEventTime        (int eindex);
		MuseRecord&       getRecord           (int eindex, int erecord);
		int               getLineIndex        (int eindex, int erecord);
		HumNum            getLineDuration     (int eindex, int erecord);
		HumNum            getNoteDuration     (int eindex, int erecord);
		int               getLastTiedNoteLineIndex(int eindex, int erecord);
		int               getNextTiedNoteLineIndex(int eindex, int erecord);
		HumNum            getTiedDuration     (int eindex, int erecord);
		int               getType             (int eindex, int erecord);
		void              cleanLineEndings    (void);
		std::string       getError            (void);
		bool              hasError            (void);


	private:
		std::vector<MuseRecord*>    m_data;
		std::vector<MuseEventSet*>  m_sequence;
		std::string                 m_name;
		std::string                 m_error;

	protected:
		void              clearError          (void);
		void              setError            (const std::string& error);
		void              processTie          (int eventindex, int recordindex,
		                                       int lastindex);
		int               searchForPitch      (int eventindex, int b40,
		                                       int track);
		int               getNextEventIndex   (int startindex,
		                                       HumNum target);
		void              constructTimeSequence(void);
		void              insertEventBackwards (HumNum atime,
		                                        MuseRecord* arecord);
		int               getPartNameIndex    (void);
		std::string       getPartName         (int index);
		std::string       trimSpaces          (std::string);
		void              assignHeaderBodyState(void);
};


std::ostream& operator<<(std::ostream& out, MuseData& musedata);




class MuseDataSet {
	public:
		                  MuseDataSet         (void);
		                  MuseDataSet         (MuseDataSet& input);
		                 ~MuseDataSet         () { clear(); }

		void              clear               (void);
		int               readPartFile        (const std::string& filename);
		int               readPartString      (const std::string& data);
		int               readPart            (std::istream& input);
		int               readFile            (const std::string& filename);
		int               readString          (const std::string& data);
		int               read                (std::istream& input);
		MuseData&         operator[]          (int index);
		int               getPartCount        (void);
		void              deletePart          (int index);
		void              cleanLineEndings    (void);

		std::string       getError            (void);
		bool              hasError            (void);
		void              clearError          (void);

	private:
		std::vector<MuseData*>  m_part;
		std::string             m_error;

	protected:
		int               appendPart          (MuseData* musedata);
		void              analyzeSetType      (std::vector<int>& types,
		                                       std::vector<std::string>& lines);
		void              analyzePartSegments (std::vector<int>& startindex,
		                                       std::vector<int>& stopindex,
		                                       std::vector<std::string>& lines);
		void              setError            (const std::string& error);

};


std::ostream& operator<<(std::ostream& out, MuseDataSet& musedata);



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

		bool   isAttack             (void);
		bool   isRest               (void);
		bool   isSustained          (void);

		std::string getAbsKernPitch (void);
		std::string getSgnKernPitch (void);

		double operator-            (NoteCell& B);
		double operator-            (int B);

		int    getLineIndex         (void);
		int    getFieldIndex        (void);
		std::ostream& printNoteInfo (std::ostream& out);
		double getDiatonicIntervalToNextAttack      (void);
		double getDiatonicIntervalFromPreviousAttack(void);
		double getMetricLevel       (void);
		HumNum getDurationFromStart (void);
		HumNum getDuration          (void);
		void   setMeter             (int topval, HumNum botval);
		int    getMeterTop          (void);
		HumNum getMeterBottom       (void);

		std::vector<HTp> m_tiedtokens;  // list of tied notes/rests after note attack

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
		int        getFieldIndex         (int vindex);

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
		static HumNum  recipToDuration      (const std::string& recip,
		                                     HumNum scale = 4,
		                                     const std::string& separator = " ");
		static HumNum  recipToDurationNoDots(const std::string& recip,
		                                     HumNum scale = 4,
		                                     const std::string& separator = " ");
		static HumNum  recipToDuration      (std::string* recip,
		                                     HumNum scale = 4,
		                                     const std::string& separator = " ");
		static HumNum  recipToDurationNoDots(std::string* recip,
		                                     HumNum scale = 4,
		                                     const std::string& separator = " ");
		static std::string  durationToRecip      (HumNum duration,
		                                     HumNum scale = HumNum(1,4));
		static std::string  durationFloatToRecip (double duration,
		                                     HumNum scale = HumNum(1,4));
		static HumNum timeSigToDurationInQuarter(HTp token);

		// Pitch processing, defined in Convert-pitch.cpp
		static std::string  base40ToKern    (int b40);
		static int     base40ToAccidental   (int b40);
		static int     base40ToDiatonic     (int b40);
		static int     base40ToMidiNoteNumber(int b40);
		static std::string  base40ToIntervalAbbr (int b40);
		static int     kernToOctaveNumber   (const std::string& kerndata);
		static int     kernToOctaveNumber   (HTp token)
				{ return kernToOctaveNumber((std::string)*token); }
		static int     kernToAccidentalCount(const std::string& kerndata);
		static int     kernToAccidentalCount(HTp token)
				{ return kernToAccidentalCount((std::string)*token); }
		static int     kernToDiatonicPC     (const std::string& kerndata);
		static int     kernToDiatonicPC     (HTp token)
				{ return kernToDiatonicPC     ((std::string)*token); }
		static char    kernToDiatonicUC     (const std::string& kerndata);
		static int     kernToDiatonicUC     (HTp token)
				{ return kernToDiatonicUC     ((std::string)*token); }
		static char    kernToDiatonicLC     (const std::string& kerndata);
		static int     kernToDiatonicLC     (HTp token)
				{ return kernToDiatonicLC     ((std::string)*token); }
		static int     kernToBase40PC       (const std::string& kerndata);
		static int     kernToBase40PC       (HTp token)
				{ return kernToBase40PC       ((std::string)*token); }
		static int     kernToBase12PC       (const std::string& kerndata);
		static int     kernToBase12PC       (HTp token)
				{ return kernToBase12PC       ((std::string)*token); }
		static int     kernToBase7PC        (const std::string& kerndata) {
		                                     return kernToDiatonicPC(kerndata); }
		static int     kernToBase7PC        (HTp token)
				{ return kernToBase7PC        ((std::string)*token); }
		static int     kernToBase40         (const std::string& kerndata);
		static int     kernToBase40         (HTp token)
				{ return kernToBase40         ((std::string)*token); }
		static int     kernToBase12         (const std::string& kerndata);
		static int     kernToBase12         (HTp token)
				{ return kernToBase12         ((std::string)*token); }
		static int     kernToBase7          (const std::string& kerndata);
		static int     kernToBase7          (HTp token)
				{ return kernToBase7          ((std::string)*token); }
		static int     kernToMidiNoteNumber (const std::string& kerndata);
		static int     kernToMidiNoteNumber(HTp token)
				{ return kernToMidiNoteNumber((std::string)*token); }
		static std::string  kernToScientificPitch(const std::string& kerndata,
		                                     std::string flat = "b",
		                                     std::string sharp = "#",
		                                     std::string separator = "");
		static std::string  kernToSciPitch  (const std::string& kerndata,
		      										 std::string flat = "b",
		                                     std::string sharp = "#",
		                                     std::string separator = "")
	       { return kernToScientificPitch(kerndata, flat, sharp, separator); }
		static std::string  kernToSP        (const std::string& kerndata,
		                                     std::string flat = "b",
		                                     std::string sharp = "#",
		                                     std::string separator = "")
		      { return kernToScientificPitch(kerndata, flat, sharp, separator); }
		static int     pitchToWbh           (int dpc, int acc, int octave,
		                                     int maxacc);
		static void    wbhToPitch           (int& dpc, int& acc, int& octave,
		                                     int maxacc, int wbh);
		static int     kernClefToBaseline   (const std::string& input);
		static int     kernClefToBaseline   (HTp input);
		static std::string  base40ToTrans   (int base40);
		static int     transToBase40        (const std::string& input);
		static int     base40IntervalToLineOfFifths(int trans);
		static std::string  keyNumberToKern (int number);
		static int     base7ToBase40        (int base7);
		static int     base40IntervalToDiatonic(int base40interval);


		// **mens, white mensual notation, defiend in Convert-mens.cpp
		static bool    isMensRest           (const std::string& mensdata);
		static bool    isMensNote           (const std::string& mensdata);
		static bool    hasLigatureBegin     (const std::string& mensdata);
		static bool    hasRectaLigatureBegin(const std::string& mensdata);
		static bool    hasObliquaLigatureBegin(const std::string& mensdata);
		static bool    hasLigatureEnd       (const std::string& mensdata);
		static bool    hasRectaLigatureEnd  (const std::string& mensdata);
		static bool    hasObliquaLigatureEnd(const std::string& mensdata);
		static bool    getMensStemDirection (const std::string& mensdata);
		static HumNum  mensToDuration       (const std::string& mensdata,
		                                     HumNum scale = 4,
		                                     const std::string& separator = " ");
		static std::string  mensToRecip          (const std::string& mensdata,
		                                     HumNum scale = 4,
		                                     const std::string& separator = " ");
		static HumNum  mensToDurationNoDots(const std::string& mensdata,
		                                     HumNum scale = 4,
		                                     const std::string& separator = " ");

		// MuseData conversions in Convert-musedata.cpp
      static int       museToBase40        (const std::string& pitchString);
      static std::string musePitchToKernPitch(const std::string& museInput);
		static std::string museClefToKernClef(const std::string& mclef);
		static std::string museKeySigToKernKeySig(const std::string& mkeysig);
		static std::string museTimeSigToKernTimeSig(const std::string& mtimesig);
		static std::string museMeterSigToKernMeterSig(const std::string& mtimesig);
		static std::string museFiguredBassToKernFiguredBass(const std::string& mfb);

		// Harmony processing, defined in Convert-harmony.cpp
		static std::vector<int> minorHScaleBase40(void);
		static std::vector<int> majorScaleBase40 (void);
		static int         keyToInversion   (const std::string& harm);
		static int         keyToBase40      (const std::string& key);
		static std::vector<int> harmToBase40     (HTp harm, const std::string& key) {
		                                        return harmToBase40(*harm, key); }
		static std::vector<int> harmToBase40     (HTp harm, HTp key) {
		                                        return harmToBase40(*harm, *key); }
		static std::vector<int> harmToBase40     (const std::string& harm, const std::string& key);
		static std::vector<int> harmToBase40     (const std::string& harm, int keyroot, int keymode);
		static void        makeAdjustedKeyRootAndMode(const std::string& secondary,
		                                     int& keyroot, int& keymode);
		static int         chromaticAlteration(const std::string& content);

		// data-type specific (other than pitch/rhythm),
		// defined in Convert-kern.cpp
		static bool isKernRest              (const std::string& kerndata);
		static bool isKernNote              (const std::string& kerndata);
		static bool isKernNoteAttack        (const std::string& kerndata);
		static bool hasKernSlurStart        (const std::string& kerndata);
		static bool hasKernSlurEnd          (const std::string& kerndata);
		static int  getKernSlurStartElisionLevel(const std::string& kerndata, int index);
		static int  getKernSlurEndElisionLevel  (const std::string& kerndata, int index);
		static char hasKernStemDirection    (const std::string& kerndata);

		static bool isKernSecondaryTiedNote (const std::string& kerndata);
		static std::string getKernPitchAttributes(const std::string& kerndata);

		// String processing, defined in Convert-string.cpp
		static std::vector<std::string> splitString   (const std::string& data,
		                                     char separator = ' ');
		static void    replaceOccurrences   (std::string& source,
		                                     const std::string& search,
		                                     const std::string& replace);
		static std::string  repeatString         (const std::string& pattern, int count);
		static std::string  encodeXml            (const std::string& input);
		static std::string  getHumNumAttributes  (const HumNum& num);
		static std::string  trimWhiteSpace       (const std::string& input);
		static bool    startsWith           (const std::string& input,
		                                     const std::string& searchstring);
		static bool    contains(const std::string& input, const std::string& pattern);
		static bool    contains(const std::string& input, char pattern);
		static bool    contains(std::string* input, const std::string& pattern);
		static bool    contains(std::string* input, char pattern);
		static void    makeBooleanTrackList(std::vector<bool>& spinelist,
		                                     const std::string& spinestring,
		                                     int maxtrack);
		static std::vector<int> extractIntegerList(const std::string& input, int maximum);
		// private functions for extractIntegerList:
		static void processSegmentEntry(std::vector<int>& field, const std::string& astring, int maximum);
		static void removeDollarsFromString(std::string& buffer, int maximum);

		// Mathematical processing, defined in Convert-math.cpp
		static int     getLcm               (const std::vector<int>& numbers);
		static int     getGcd               (int a, int b);
		static void    primeFactors         (std::vector<int>& output, int n);
		static double  nearIntQuantize      (double value,
		                                    double delta = 0.00001);
		static double  significantDigits    (double value, int digits);
		static bool    isNaN                (double value);
		static double  pearsonCorrelation   (const std::vector<double> &x, const std::vector<double> &y);
		static double  standardDeviation    (const std::vector<double>& x);
		static double  standardDeviationSample(const std::vector<double>& x);
		static double  mean                 (const std::vector<double>& x);
		static int     romanNumeralToInteger(const std::string& roman);
		static double  coefficientOfVariationSample(const std::vector<double>& x);
		static double  coefficientOfVariationPopulation(const std::vector<double>& x);
		static double  nPvi                 (const std::vector<double>& x);
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
				KeyDesignations,
				KeySigs,
				TimeSigs,
				MeterSigs,
				Tempos,
				Labels,
				LabelAbbrs,
				Ottavas,
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
		string        getCaesura           (void) const;
		int           getHarmonyCount      (void) const;
		void          trackStaffVoices     (int staffnum, int voicenum);
		void          printStaffVoiceInfo  (void);
		void          prepareVoiceMapping  (void);
		int           getVoiceIndex        (int voicenum);
		int           getStaffIndex        (int voicenum);
		bool          hasEditorialAccidental(void) const;
		bool          hasDynamics          (void) const;
		bool          hasFiguredBass       (void) const;
		void          parsePartInfo        (xml_node partdeclaration);
		string        getPartName          (void) const;
		string        getPartAbbr          (void) const;
		string        cleanSpaces          (const string& input);
		bool          hasOrnaments         (void) const;


	private:
		void          receiveStaffNumberFromChild (int staffnum, int voicenum);
		void          receiveVerseCount           (int count);
		void          receiveVerseCount           (int staffnum, int count);
		void          receiveHarmonyCount         (int count);
		void          receiveEditorialAccidental  (void);
		void          receiveDynamic              (void);
		void          receiveFiguredBass          (void);
		void          receiveCaesura              (const string& letter);
		void          receiveOrnament             (void);

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
		bool                 m_has_figured_bass = false;
		string               m_partname;
		string               m_partabbr;
		string               m_caesura;
		bool                 m_hasOrnaments = false;

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
		void  setVerse          (int index, const std::string& token);

		int   getHarmonyCount   (void);
		void  setHarmony        (HTp token);
		void  setHarmony        (const std::string& token);
		void  detachHarmony     (void);
		HTp   getHarmony        (void);

		int   getDynamicsCount  (void);
		void  setDynamics       (HTp token);
		void  setDynamics       (const std::string& token);
		void  detachDynamics    (void);
		HTp   getDynamics       (void);

		int   getFiguredBassCount (void);
		void  setFiguredBass      (HTp token);
		void  setFiguredBass      (const std::string& token);
		void  detachFiguredBass   (void);
		HTp   getFiguredBass      (void);

	private:
		std::vector<HumdrumToken*> m_verses;
		HumdrumToken* m_dynamics = NULL;
		HumdrumToken* m_figured_bass = NULL;
		HumdrumToken* m_harmony = NULL;
};

std::ostream& operator<<(std::ostream& output, GridSide* side);


class GridStaff : public std::vector<GridVoice*>, public GridSide {
	public:
		GridStaff(void);
		~GridStaff();
		GridVoice* setTokenLayer (int layerindex, HTp token, HumNum duration);
		void setNullTokenLayer   (int layerindex, SliceType type, HumNum nextdur);
		void appendTokenLayer    (int layerindex, HTp token, HumNum duration,
		                          const std::string& spacer = " ");
		int getMaxVerseCount     (void);
		string getString         (void);
};

std::ostream& operator<<(std::ostream& output, GridStaff* staff);



class GridPart : public std::vector<GridStaff*>, public GridSide {
	public:
		GridPart(void);
		~GridPart();

	private:
		std::string m_partName;


};

std::ostream& operator<<(std::ostream& output, GridPart* part);
std::ostream& operator<<(std::ostream& output, GridPart& part);



class GridSlice;
class HumGrid;

class GridMeasure : public std::list<GridSlice*> {
	public:
		GridMeasure(HumGrid* owner);
		~GridMeasure();

		GridSlice*   addTempoToken  (const std::string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addTimeSigToken(const std::string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addMeterSigToken(const std::string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addKeySigToken (const std::string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addClefToken   (const std::string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addTransposeToken(const std::string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addLabelToken  (const std::string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxpart,
		                             int maxstaff);
		GridSlice*   addLabelAbbrToken(const std::string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxpart,
		                             int maxstaff);
		GridSlice*   addDataToken   (const std::string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff);
		GridSlice*   addDataSubtoken(const std::string& tok, HumNum timestamp,
		                             int part, int staff, int voice);
		GridSlice*   addGraceToken  (const std::string& tok, HumNum timestamp,
		                             int part, int staff, int voice, int maxstaff,
		                             int gracenumber);
		GridSlice*   addGlobalLayout(const std::string& tok, HumNum timestamp);
		GridSlice*   addGlobalComment(const std::string& tok, HumNum timestamp);
		GridSlice*   appendGlobalLayout(const std::string& tok, HumNum timestamp);
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
		void         setMeasureNumber(int value);
		int          getMeasureNumber(void);

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
		void         addLayoutParameter(GridSlice* slice, int partindex, const std::string& locomment);
		void         addLayoutParameter(HumNum timestamp, int partindex, int staffindex, const std::string& locomment);
		void         addDynamicsLayoutParameters(GridSlice* slice, int partindex, const std::string& locomment);
		void         addFiguredBassLayoutParameters(GridSlice* slice, int partindex, const std::string& locomment);
		GridSlice*   addFiguredBass(HTp token, HumNum timestamp, int part, int maxstaff);
		GridSlice*   addFiguredBass(const std::string& tok, HumNum timestamp, int part, int maxstaff);
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
		int          m_barnum = -1;
};

std::ostream& operator<<(std::ostream& output, GridMeasure& measure);
std::ostream& operator<<(std::ostream& output, GridMeasure* measure);


class HumGrid;


class GridSlice : public std::vector<GridPart*> {
	public:
		GridSlice(GridMeasure* measure, HumNum timestamp, SliceType type,
		          int partcount = 0);
		GridSlice(GridMeasure* measure, HumNum timestamp, SliceType type,
		          const GridSlice& slice);
		GridSlice(GridMeasure* measure, HumNum timestamp, SliceType type,
		          GridSlice* slice);
		~GridSlice();

		bool isNoteSlice(void)          { return m_type == SliceType::Notes;            }
		bool isGraceSlice(void)         { return m_type == SliceType::GraceNotes;       }
		bool isMeasureSlice(void)       { return m_type == SliceType::Measures;         }
		bool isClefSlice(void)          { return m_type == SliceType::Clefs;            }
		bool isLabelSlice(void)         { return m_type == SliceType::Labels;           }
		bool isLabelAbbrSlice(void)     { return m_type == SliceType::LabelAbbrs;       }
		bool isTransposeSlice(void)     { return m_type == SliceType::Transpositions;   }
		bool isKeySigSlice(void)        { return m_type == SliceType::KeySigs;          }
		bool isKeyDesignationSlice(void){ return m_type == SliceType::KeyDesignations;  }
		bool isTimeSigSlice(void)       { return m_type == SliceType::TimeSigs;         }
		bool isTempoSlice(void)         { return m_type == SliceType::Tempos;           }
		bool isMeterSigSlice(void)      { return m_type == SliceType::MeterSigs;        }
		bool isManipulatorSlice(void)   { return m_type==SliceType::Manipulators;       }
		bool isLayoutSlice(void)        { return m_type ==  SliceType::Layouts;         }
		bool isLocalLayoutSlice(void)   { return m_type ==  SliceType::Layouts;         }
		bool isInvalidSlice(void)       { return m_type == SliceType::Invalid;          }
		bool isGlobalComment(void)      { return m_type == SliceType::GlobalComments;   }
		bool isGlobalLayout(void)       { return m_type == SliceType::GlobalLayouts;    }
		bool isReferenceRecord(void)    { return m_type == SliceType::ReferenceRecords; }
		bool isOttavaRecord(void)       { return m_type == SliceType::Ottavas;          }
		bool isInterpretationSlice(void);
		bool isDataSlice(void);
		bool hasSpines(void);
		SliceType getType(void)    { return m_type; }

		void transferTokens        (HumdrumFile& outfile, bool recip);
		void initializePartStaves  (std::vector<MxmlPart>& partdata);
		void initializeBySlice     (GridSlice* slice);
		void initializeByStaffCount(int staffcount);
		void reportVerseCount      (int partindex, int staffindex, int count);

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
		                           const std::string& empty, int maxvcount,
		                           int maxhcount, int maxfcount);
		void transferSides        (HumdrumLine& line, GridPart& sides,
		                           int partindex, const std::string& empty,
		                           int maxvcount, int maxhcount,
		                           int maxdcount, int maxfcount);
		int getVerseCount         (int partindex, int staffindex);
		int getHarmonyCount       (int partindex, int staffindex = -1);
		int getDynamicsCount      (int partindex, int staffindex = -1);
		int getFiguredBassCount   (int partindex, int staffindex = -1);
		void addToken             (const std::string& tok, int parti, int staffi, int voicei);

	protected:
		HTp  createRecipTokenFromDuration  (HumNum duration);

	private:
		HumGrid*     m_owner;
		GridMeasure* m_measure;
		HumNum       m_timestamp;
		HumNum       m_duration;
		SliceType    m_type;

};


std::ostream& operator<<(std::ostream& output, GridSlice* slice);
std::ostream& operator<<(std::ostream& output, GridSlice& slice);



class GridVoice {
	public:
		GridVoice(void);
		GridVoice(HTp token, HumNum duration);
		GridVoice(const char* token, HumNum duration);
		GridVoice(const std::string& token, HumNum duration);
		~GridVoice();

		bool   isTransfered       (void);

		HTp    getToken           (void) const;
		void   setToken           (HTp token);
		void   setToken           (const std::string& token);
		void   setToken           (const char* token);
		bool   isNull             (void) const;

		void   setDuration        (HumNum duration);
		HumNum getDuration        (void) const;
		HumNum getDurationToNext  (void) const;
		HumNum getDurationToPrev  (void) const;
		void   setDurationToPrev  (HumNum dur);
		void   incrementDuration  (HumNum duration);
		void   forgetToken        (void);
		std::string getString          (void);

	protected:
		void   setTransfered      (bool state);

	private:
		HTp    m_token;
		HumNum m_nextdur;
		HumNum m_prevdur;
		bool   m_transfered;

	friend class GridSlice;
};

std::ostream& operator<<(std::ostream& output, GridVoice* voice);
std::ostream& operator<<(std::ostream& output, GridVoice& voice);



class HumGrid : public std::vector<GridMeasure*> {
	public:
		HumGrid(void);
		~HumGrid();
		void enableRecipSpine           (void);
		bool transferTokens             (HumdrumFile& outfile, int startbarnum = 0);
		int  getHarmonyCount            (int partindex);
		int  getDynamicsCount           (int partindex);
		int  getFiguredBassCount        (int partindex);
		int  getVerseCount              (int partindex, int staffindex);
		bool hasDynamics                (int partindex);
		bool hasFiguredBass             (int partindex);
		void setDynamicsPresent         (int partindex);
		void setFiguredBassPresent      (int partindex);
		void setHarmonyPresent          (int partindex);
		void setVerseCount              (int partindex, int staffindex, int count);
		void reportVerseCount           (int partindex, int staffindex, int count);
		void setHarmonyCount            (int partindex, int count);
		void removeRedundantClefChanges (void);
		void removeSibeliusIncipit      (void);
		bool hasPickup                  (void);
		GridMeasure*  addMeasureToBack  (void);
		int  getPartCount               (void);
		int  getStaffCount              (int partindex);
		void deleteMeasure              (int index);
		void setPartName                (int index, const string& name);
		std::string getPartName         (int index);
		void addInvisibleRestsInFirstTrack(void);
		void setPartStaffDimensions     (std::vector<std::vector<GridSlice*>>& nextevent,
		                                 GridSlice* startslice);
		void addInvisibleRest           (std::vector<std::vector<GridSlice*>>& nextevent,
		                                 int index, int p, int s);

	protected:
		void calculateGridDurations        (void);
		void insertExclusiveInterpretationLine (HumdrumFile& outfile);
		void insertDataTerminationLine     (HumdrumFile& outfile);
		void appendMeasureLine             (HumdrumFile& outfile,
		                                    GridSlice& slice);
		void insertPartIndications         (HumdrumFile& outfile);
		void insertStaffIndications        (HumdrumFile& outfile);
		void insertPartNames               (HumdrumFile& outfile);
		void addNullTokens                 (void);
		void addNullTokensForGraceNotes    (void);
		void addNullTokensForClefChanges   (void);
		void addNullTokensForLayoutComments(void);

		void fillInNullTokensForGraceNotes(GridSlice* graceslice, GridSlice* lastnote,
		                                   GridSlice* nextnote);
		void fillInNullTokensForLayoutComments(GridSlice* layoutslice, GridSlice* lastnote,
		                                   GridSlice* nextnote);
		void fillInNullTokensForClefChanges (GridSlice* clefslice,
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
		void cleanManipulator              (std::vector<GridSlice*>& newslices,
		                                    GridSlice* curr);
		GridSlice* checkManipulatorExpand  (GridSlice* curr);
		GridSlice* checkManipulatorContract(GridSlice* curr);
		void transferMerges                (GridStaff* oldstaff,
		                                    GridStaff* oldlaststaff,
		                                    GridStaff* newstaff,
		                                    GridStaff* newlaststaff, int pindex,
		                                    int sindex);
		void transferOtherParts            (GridSlice* oldline, GridSlice* newline, int maxpart);
		void insertExInterpSides           (HumdrumLine* line, int part,
		                                    int staff);
		void insertSideTerminals           (HumdrumLine* line, int part,
		                                    int staff);
		void insertSidePartInfo            (HumdrumLine* line, int part,
		                                    int staff);
		void insertSideStaffInfo           (HumdrumLine* line, int part,
		                                    int staff, int staffnum);
		void insertSideNullInterpretations (HumdrumLine* line,
		                                    int part, int staff);
		void getMetricBarNumbers           (std::vector<int>& barnums);
		string  createBarToken             (int m, int barnum,
		                                    GridMeasure* measure);
		string getBarStyle                 (GridMeasure* measure);
		void adjustExpansionsInStaff       (GridSlice* newmanip, GridSlice* curr,
		                                    int p, int s);
		void transferNonDataSlices         (GridMeasure* output, GridMeasure* input);
		string extractMelody               (GridMeasure* measure);
		void insertMelodyString            (GridMeasure* measure, const string& melody);
		GridVoice* createVoice             (const string& tok, const string& post, HumNum duration, int pindex, int sindex);
		HTp createHumdrumToken             (const string& tok, int pindex, int sindex);
		GridSlice* getNextSpinedLine       (const GridMeasure::iterator& it, int measureindex);
		void matchVoices                   (GridSlice* current, GridSlice* last);
		void adjustVoices                  (GridSlice* curr, GridSlice* newmanip, int partsplit);
		void createMatchedVoiceCount       (GridStaff* snew, GridStaff* sold, int p, int s);

	private:
		std::vector<GridSlice*>       m_allslices;
		std::vector<std::vector<int>> m_verseCount;
		std::vector<int>              m_harmonyCount;
		bool                          m_pickup;
		std::vector<bool>             m_dynamics;
		std::vector<bool>             m_figured_bass;
		std::vector<bool>             m_harmony;

		std::vector<std::string>      m_partnames;

		// options:
		bool m_recip;               // include **recip spine in output
		bool m_musicxmlbarlines;    // use measure numbers from <measure> element

};

ostream& operator<<(ostream& out, HumGrid& grid);



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
		bool               hasGraceSlash      (void);
		bool               isFloating         (void);
		bool               hasSlurStart       (int& direction);
		bool               hasSlurStop        (void);
		void               setLinked          (void);
		std::vector<MxmlEvent*> getLinkedNotes     (void);
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
		std::string        getRecip           (void) const;
		std::string        getKernPitch       (void);
		std::string        getPrefixNoteInfo  (void) const;
		std::string        getPostfixNoteInfo (bool primarynote) const;
		xml_node           getNode            (void);
		xml_node           getHNode           (void);
		HumNum             getTimeSigDur      (void);
		std::string        getElementName     (void);
		void               addNotations       (std::stringstream& ss,
		                                       xml_node notations) const;
		void               reportVerseCountToOwner    (int count);
		void               reportVerseCountToOwner    (int staffnum, int count);
		void               reportHarmonyCountToOwner  (int count);
		void               reportMeasureStyleToOwner  (MeasureStyle style);
		void               reportEditorialAccidentalToOwner(void);
		void               reportDynamicToOwner       (void);
		void               reportFiguredBassToOwner   (void);
		void               reportCaesuraToOwner       (const std::string& letter = "Z") const;
		void               reportOrnamentToOwner      (void) const;
      void               makeDummyRest      (MxmlMeasure* owner,
		                                       HumNum startime,
		                                       HumNum duration,
		                                       int staffindex = 0,
		                                       int voiceindex = 0);
		void               setVoiceIndex      (int voiceindex);
		void               forceInvisible     (void);
		bool               isInvisible        (void);
		void               setBarlineStyle    (xml_node node);
		void               setTexts           (std::vector<std::pair<int, xml_node>>& nodes);
		std::vector<std::pair<int, xml_node>>&  getTexts           (void);
		void               setDynamics        (xml_node node);
		void               setHairpinEnding   (xml_node node);
		void               setFiguredBass     (xml_node node);
		xml_node           getDynamics        (void);
		xml_node           getHairpinEnding   (void);
		xml_node           getFiguredBass     (void);
		std::string        getRestPitch       (void) const;

	protected:
		HumNum             m_starttime;  // start time in quarter notes of event
		HumNum             m_duration;   // duration in quarter notes of event
		measure_event_type m_eventtype;  // enumeration type of event
		xml_node           m_node;       // pointer to event in XML structure
		MxmlMeasure*       m_owner;      // measure that contains this event
		std::vector<MxmlEvent*> m_links; // list of secondary chord notes
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
		xml_node          m_hairpin_ending; // hairpin <direction> starting just after note and before new measure
		xml_node          m_figured_bass;// fb starting just before note
		std::vector<std::pair<int, xml_node>>  m_text;   // text <direction> starting just before note

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


std::ostream& operator<<(std::ostream& output, xml_node element);



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
		void  receiveOrnamentFromChild            (void);
   	void  reportStaffNumberToOwner            (int staffnum, int voicenum);
		void  reportVerseCountToOwner             (int count);
		void  reportVerseCountToOwner             (int staffindex, int count);
		void  reportHarmonyCountToOwner           (int count);
		void  reportEditorialAccidentalToOwner    (void);
		void  reportDynamicToOwner                (void);
		void  reportFiguredBassToOwner            (void);
		void  reportCaesuraToOwner                (const string& letter);
		void  reportOrnamentToOwner               (void);

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
		bool     isModified         (void);
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
		bool         m_modifiedQ;
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
		bool            getBoolean        (const string& optionName);
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

		void          clearOutput     (void);

		bool          hasAnyText      (void);
		std::string   getAllText      (void);
		ostream&      getAllText      (ostream& out);

		bool          hasHumdrumText  (void);
		std::string   getHumdrumText  (void);
		ostream&      getHumdrumText  (ostream& out);
		void          suppressHumdrumFileOutput(void);

		bool          hasJsonText     (void);
		std::string   getJsonText     (void);
		ostream&      getJsonText     (ostream& out);

		bool          hasFreeText     (void);
		std::string   getFreeText     (void);
		ostream&      getFreeText     (ostream& out);

		bool          hasWarning      (void);
		std::string   getWarning      (void);
		ostream&      getWarning      (ostream& out);

		bool          hasError        (void);
		std::string   getError        (void);
		ostream&      getError        (ostream& out);
		void          setError        (const string& message);

	protected:
		std::stringstream m_humdrum_text;  // output text in Humdrum syntax.
		std::stringstream m_json_text;     // output text in JSON syntax.
		std::stringstream m_free_text;     // output for plain text content.
	  	std::stringstream m_warning_text;  // output for warning messages;
	  	std::stringstream m_error_text;    // output for error messages;

		bool m_suppress = false;

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

#define BASIC_INTERFACE(CLASS)                         \
using namespace std;                                   \
using namespace hum;                                   \
int main(int argc, char** argv) {                      \
	CLASS interface;                                    \
	if (!interface.process(argc, argv)) {               \
		interface.getError(cerr);                        \
		return -1;                                       \
	}                                                   \
	HumdrumFile infile;                                 \
	if (interface.getArgCount() > 0) {                  \
		infile.readNoRhythm(interface.getArgument(1));   \
	} else {                                            \
		infile.readNoRhythm(cin);                        \
	}                                                   \
	int status = interface.run(infile, cout);           \
	if (interface.hasWarning()) {                       \
		interface.getWarning(cerr);                      \
		return 0;                                        \
	}                                                   \
	if (interface.hasError()) {                         \
		interface.getError(cerr);                        \
		return -1;                                       \
	}                                                   \
	return !status;                                     \
}



//////////////////////////////
//
// STREAM_INTERFACE -- Use HumdrumFileStream (low-memory
//    usage implementation).
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
	HumdrumFileStream instream(static_cast<Options&>(interface)); \
	HumdrumFileSet infiles;                                       \
	bool status = true;                                           \
	while (instream.readSingleSegment(infiles)) {                 \
		status &= interface.run(infiles);                          \
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
			for (int i=0; i<infiles.getCount(); i++) {              \
				cout << infiles[i];                                  \
			}                                                       \
		}                                                          \
		interface.clearOutput();                                   \
	}                                                             \
	return !status;                                               \
}



//////////////////////////////
//
// RAW_STREAM_INTERFACE -- Use HumdrumFileStream but send the
//    HumdrumFileStream object to the filter rather than individual
//    Humdrum files.
//

#define RAW_STREAM_INTERFACE(CLASS)                              \
using namespace std;                                             \
using namespace hum;                                             \
int main(int argc, char** argv) {                                \
	CLASS interface;                                              \
	if (!interface.process(argc, argv)) {                         \
		interface.getError(cerr);                                  \
		return -1;                                                 \
	}                                                             \
	HumdrumFileStream instream(static_cast<Options&>(interface)); \
	bool status = interface.run(instream);                        \
	if (interface.hasWarning()) {                                 \
		interface.getWarning(cerr);                                \
	}                                                             \
	if (interface.hasAnyText()) {                                 \
	   interface.getAllText(cout);                                \
	}                                                             \
	if (interface.hasError()) {                                   \
		interface.getError(cerr);                                  \
        return -1;                                               \
	}                                                             \
	interface.clearOutput();                                      \
	return !status;                                               \
}



//////////////////////////////
//
// SET_INTERFACE -- Use HumdrumFileSet (multiple file high-memory
//    usage implementation).
//

#define SET_INTERFACE(CLASS)                                     \
using namespace std;                                             \
using namespace hum;                                             \
int main(int argc, char** argv) {                                \
	CLASS interface;                                              \
	if (!interface.process(argc, argv)) {                         \
		interface.getError(cerr);                                  \
		return -1;                                                 \
	}                                                             \
	HumdrumFileStream instream(static_cast<Options&>(interface)); \
	HumdrumFileSet infiles;                                       \
	instream.read(infiles);                                       \
	bool status = interface.run(infiles);                         \
	if (interface.hasWarning()) {                                 \
		interface.getWarning(cerr);                                \
	}                                                             \
	if (interface.hasAnyText()) {                                 \
	   interface.getAllText(cout);                                \
	}                                                             \
	if (interface.hasError()) {                                   \
		interface.getError(cerr);                                  \
        return -1;                                               \
	}                                                             \
	if (!interface.hasAnyText()) {                                \
		for (int i=0; i<infiles.getCount(); i++) {                 \
			cout << infiles[i];                                     \
		}                                                          \
	}                                                             \
	interface.clearOutput();                                      \
	return !status;                                               \
}



class HumdrumFileSet;

class HumdrumFileStream {
	public:
		                HumdrumFileStream  (void);
		                HumdrumFileStream  (char** list);
		                HumdrumFileStream  (const std::vector<std::string>& list);
		                HumdrumFileStream  (Options& options);
		                HumdrumFileStream  (const string& datastream);

		void            loadString         (const string& data);

		int             setFileList        (char** list);
		int             setFileList        (const std::vector<std::string>& list);

		void            clear              (void);
		int             eof                (void);

		int             getFile            (HumdrumFile& infile);
		int             read               (HumdrumFile& infile);
		int             read               (HumdrumFileSet& infiles);
		int             readSingleSegment  (HumdrumFileSet& infiles);

	protected:
		std::stringstream m_stringbuffer;   // used to read files from a string
		std::ifstream     m_instream;       // used to read from list of files
		std::stringstream m_urlbuffer;      // used to read data over internet
		std::string       m_newfilebuffer;  // used to keep track of !!!!segment:
		                                    // records.

		std::vector<std::string>  m_filelist;       // used when not using cin
		int                       m_curfile;        // index into filelist

		std::vector<std::string>  m_universals;     // storage for universal comments

		// Automatic URL downloading of data from internet in read():
		void     fillUrlBuffer            (std::stringstream& uribuffer,
		                                   const std::string& uriname);

};



///////////////////////////////////////////////////////////////////////////

class HumdrumFileSet {
   public:
                            HumdrumFileSet   (void);
                            HumdrumFileSet   (Options& options);
                            HumdrumFileSet   (const std::string& contents);
                           ~HumdrumFileSet   ();

      void                  clear            (void);
      void                  clearNoFree      (void);
      int                   getSize          (void);
      int                   getCount         (void) { return getSize(); }
      HumdrumFile&          operator[]       (int index);
		bool                  swap             (int index1, int index2);
		bool                  hasFilters       (void);
		bool                  hasGlobalFilters    (void);
		bool                  hasUniversalFilters (void);
		std::vector<HumdrumLine*> getUniversalReferenceRecords(void);

      int                   readFile         (const std::string& filename);
      int                   readString       (const std::string& contents);
      int                   readStringCsv    (const std::string& contents);
      int                   read             (std::istream& inStream);
      int                   read             (Options& options);
      int                   read             (HumdrumFileStream& instream);

      int                   readAppendFile   (const std::string& filename);
      int                   readAppendString (const std::string& contents);
      int                   readAppendStringCsv (const std::string& contents);
      int                   readAppend       (std::istream& inStream);
      int                   readAppend       (Options& options);
      int                   readAppend       (HumdrumFileStream& instream);
      int                   readAppendHumdrum(HumdrumFile& infile);
		int                   appendHumdrumPointer(HumdrumFile* infile);

   protected:
      vector<HumdrumFile*>  m_data;

      void                  appendHumdrumFileContent(const std::string& filename,
                                               std::stringstream& inbuffer);
};




class Tool_autobeam : public HumTool {
	public:
		         Tool_autobeam   (void);
		        ~Tool_autobeam   () {};

		bool     run             (HumdrumFile& infile);
		bool     run             (HumdrumFileSet& infiles);
		bool     run             (const string& indata, ostream& out);
		bool     run             (HumdrumFile& infile, ostream& out);

	protected:
		void     initialize      (HumdrumFile& infile);
		void     processStrand   (HTp strandstart, HTp strandend);
		void     processMeasure  (vector<HTp>& measure);
		void     addBeam         (HTp startnote, HTp endnote);
		void     addBeams        (HumdrumFile& infile);
		void     removeBeams     (HumdrumFile& infile);
		void     removeEdgeRests (HTp& startnote, HTp& endnote);
		void     breakBeamsByLyrics(HumdrumFile& infile);
		void     processStrandForLyrics(HTp stok, HTp etok);
		bool     hasSyllable     (HTp token);
		void     splitBeam       (HTp tok, HTp stok, HTp etok);
		void     splitBeam2      (vector<HTp>& group, HTp tok);
		void     getBeamedNotes(vector<HTp>& toks, HTp tok, HTp stok, HTp etok);
		bool     isLazy          (vector<HTp>& group);
		void     splitBeamLazy   (vector<HTp>& group, HTp tok);
		void     splitBeamNotLazy(vector<HTp>& group, HTp tok);
		void     removeBeamCharacters(HTp token);

	private:
		std::vector<std::vector<pair<int, HumNum> > > m_timesigs;
		std::vector<HTp> m_kernspines;
		bool        m_overwriteQ;
		int         m_track;
		bool        m_includerests = false;
		int         m_splitcount = 0;

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

		bool     run                   (HumdrumFileSet& infiles);
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

		bool     run               (HumdrumFileSet& infiles);
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


class Tool_chooser : public HumTool {
	public:
		       	   Tool_chooser       (void);
		       	  ~Tool_chooser       () {};

		bool        run                (HumdrumFileSet& infiles);
		bool        run                (const string& indata);
		bool        run                (HumdrumFileStream& instream);

	protected:
		void        processFiles       (HumdrumFileSet& infiles);
		void        initialize         (void);

		void        expandSegmentList  (vector<int>& field, string& fieldstring,
		                                int maximum);
		void        processSegmentEntry(vector<int>& field,
		                                const string& astring, int maximum);
		void        removeDollarsFromString(string& buffer, int maximum);

	private:

};


class Tool_chord : public HumTool {
	public:
		         Tool_chord      (void);
		        ~Tool_chord      () {};

		bool     run               (HumdrumFileSet& infiles);
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

		bool     run                    (HumdrumFileSet& infiles);
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


class Tool_composite : public HumTool {
	public:
		       	   Tool_composite      (void);
		       	  ~Tool_composite      () {};

		bool        run                (HumdrumFileSet& infiles);
		bool        run                (HumdrumFile& infile);
		bool        run                (const string& indata, ostream& out);
		bool        run                (HumdrumFile& infile, ostream& out);

	protected:
		void        processFile        (HumdrumFile& infile);
		void        initialize         (void);
		HumNum      getLineDuration    (HumdrumFile& infile, int index, vector<bool>& isNull);

	private:
		string      m_pitch = "e";

};


class Tool_dissonant : public HumTool {
	public:
		         Tool_dissonant    (void);
		        ~Tool_dissonant    () {};

		bool     run               (HumdrumFileSet& infiles);
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
		void    findCadentialVoiceFunctions(vector<vector<string> >& results,
		                            NoteGrid& grid, vector<NoteCell*>& attacks,
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
		void    simpleNextMerge    (HTp cnote, HTp nnote);
		void    changePitchOfTieGroupFollowing(HTp note, const string& pitch);
		void    mergeWithPreviousNoteViaTies(HTp pnote, HTp cnote);
		void    mergeWithPreviousNote(HumdrumFile& infile, NoteCell* cell);
		void    mergeWithNextNote    (HumdrumFile& infile, NoteCell* cell);
		void    adjustColorization   (HumdrumFile& infile);
		void    adjustColorForVoice  (HTp spinestart, vector<string>& labels);
		void    removeAgentColor     (HTp disslabel, const string& marker, const string& query);
		void    addLabelToSuspensions(HTp disslabel, const string& marker);
		bool    isSuspension         (HTp token);
		void    addSuspensionMarkToNote(HTp start, const string& marks);
		void    adjustSuspensionColors(HTp speinstart);

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
		const int ACC_PASSING_UP       = 22; // appoggiatura
		const int ACC_PASSING_DOWN     = 23; // appoggiatura
		const int ACC_UP_NEI           = 24; // appoggiatura
		const int ACC_LO_NEI           = 25; // appoggiatura
		const int APP_UPPER            = 26; // appoggiatura
		const int APP_LOWER            = 27; // appoggiatura
		const int SUS_BIN              = 28; // binary suspension
		const int SUS_TERN             = 29; // ternary suspension
		const int AGENT_BIN            = 30; // binary agent
		const int AGENT_TERN           = 31; // ternary agent
		const int SUSPENSION_REP       = 32; // suspension repeated note
		const int FAKE_SUSPENSION_LEAP = 33; // fake suspension approached by leap
		const int FAKE_SUSPENSION_STEP = 34; // fake suspension approached by step or anticipation
		const int SUS_NO_AGENT_LEAP    = 35; // suspension missing a normal agent approached by leap
		const int SUS_NO_AGENT_STEP    = 36; // suspension missing a normal agent approached by step or anticipation
		const int CHANSON_IDIOM        = 37; // chanson idiom
		const int ORNAMENTAL_SUS       = 38; // purely ornamental suspension

		// unknown dissonances:
		const int PARALLEL_UP          = 39; // moves in parallel with known dissonant, approached from below
		const int PARALLEL_DOWN        = 40; // moves in parallel with known dissonant, approached from above
		const int RES_PITCH            = 41; // note of resolution of a suspension against suspension dissonance

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
		         Tool_extract           (void);
		        ~Tool_extract           () {};

		bool     run                    (HumdrumFileSet& infiles);
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
		vector<int> getNullDataTracks(HumdrumFile& infile);
		void fillFieldDataByEmpty       (vector<int>& field, vector<int>& subfield,
				                           vector<int>& model, HumdrumFile& infile, int negate);
		void fillFieldDataByNoEmpty     (vector<int>& field, vector<int>& subfield,
				                           vector<int>& model, HumdrumFile& infile, int negate);
		void fillFieldDataByNoRest      (vector<int>& field, vector<int>& subfield,
		                                 vector<int>& model, const string& searchstring,
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
		string      blankName   = "**blank"; // used with -n option
		int         noEmptyQ    = 0;       // used with --no-empty option
		int         emptyQ      = 0;       // used with --empty option
		int         spineListQ  = 0;       // used with --spine option
		int         removerestQ = 0;       // used with --no-rest option

};



class Tool_filter : public HumTool {
	public:
		         Tool_filter        (void);
		        ~Tool_filter        () {};

		bool     run                (HumdrumFileSet& infiles);
		bool     run                (HumdrumFile& infile);
		bool     run                (const string& indata);

		bool     runUniversal       (HumdrumFileSet& infiles);

	protected:
		void     getCommandList     (vector<pair<string, string> >& commands,
		                             HumdrumFile& infile);
		void     getUniversalCommandList(std::vector<std::pair<std::string, std::string> >& commands,
		                             HumdrumFileSet& infiles);
		void     initialize         (HumdrumFile& infile);
		void     removeGlobalFilterLines    (HumdrumFile& infile);
		void     removeUniversalFilterLines (HumdrumFileSet& infiles);

	private:
		string   m_variant;        // used with -v option.
		bool     m_debugQ = false; // used with --debug option

};


class HPNote {
	public:
		int track = -1;
		int line = -1;
		int field = -1;
		int subfield = -1;
		HTp token = NULL;
		HumNum duration = 0;
		std::string text;
		bool attack = false;
		bool nullQ = false;
};

class Tool_homophonic : public HumTool {
	public:
		            Tool_homophonic    (void);
		           ~Tool_homophonic    () {};

		bool        run                (HumdrumFileSet& infiles);
		bool        run                (HumdrumFile& infile);
		bool        run                (const string& indata, ostream& out);
		bool        run                (HumdrumFile& infile, ostream& out);

	protected:
		void        processFile        (HumdrumFile& infile);
		void        analyzeLine        (HumdrumFile& infile, int line);
		void        initialize         (void);
		void        markHomophonicNotes(void);
		int         getExtantVoiceCount(HumdrumFile& infile);
		int         getOriginalVoiceCount(HumdrumFile& infile);
		void        addRawAnalysis     (HumdrumFile& infile, vector<double>& raw);
		void        addAccumulatedScores(HumdrumFile& infile, vector<double>& score);
		void        addAttacks         (HumdrumFile& infile, vector<int>& attacks);
		void        addFractionAnalysis(HumdrumFile& infile, std::vector<double>& score);

	private:
		std::vector<std::string> m_homophonic;
		std::vector<int> m_notecount;
		std::vector<int> m_attacks;
		std::vector<std::vector<HPNote>> m_notes;
		double m_threshold = 4.0;
		double m_score = 1.0;
		double m_intermediate_score = 0.5;
		int m_voice_count = 0;
		bool m_letterQ = false;
};


class Tool_homophonic2 : public HumTool {
	public:
		            Tool_homophonic2    (void);
		           ~Tool_homophonic2    () {};

		bool        run                (HumdrumFileSet& infiles);
		bool        run                (HumdrumFile& infile);
		bool        run                (const string& indata, ostream& out);
		bool        run                (HumdrumFile& infile, ostream& out);

	protected:
		void        processFile        (HumdrumFile& infile);
		void        initialize         (void);

	private:
		double      m_threshold = 0.6;
		double      m_threshold2 = 0.4;
		vector<double> m_score;
};


class Tool_hproof : public HumTool {
	public:
		      Tool_hproof      (void);
		     ~Tool_hproof      () {};

		bool  run              (HumdrumFileSet& infiles);
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



// A TimePoint records the event times in a file.  These are positions of note attacks
// in the file.  The "index" variable keeps track of the line in the original file
// (for the first position in index), and other positions in index keep track of the
// equivalent line position of the timepoint in other file(s) that are being compared.
class TimePoint {
	public:
		// file: pointers to the file in which the index refers to
		vector<HumdrumFile*> file;

		// index :: A list of indexes for the lines at which the given timestamp
		// occurs in each file.  The first index is for the reference work.
		vector<int> index;

		// timestamp :: The duration from the start of the score to given time in score.
		HumNum timestamp = -1;

		// measure :: The measure number in which the timestamp occurs.
		int measure = -1;

		void clear(void) {
			file.clear();
			index.clear();
			timestamp = -1;
			measure = -1;
		}
};


// NotePoint is a note from a score that will be matches hopefully to an
// equivalent note in another score.
class NotePoint {
	public:
		HTp         token          = NULL;   // Humdrum token that contains note
		string      subtoken;                // string that represents not (token may be chord)
		int         subindex       = -1;     // subtoken index of note (in chord)
		int         measure        = -1;     // measure number that note is found
		HumNum      measurequarter = -1;     // distance from start of measure to note
		int         track          = -1;     // track that note is from
		int         layer          = -1;     // layer that note is in
		HumNum      duration       = -1;     // duration (tied) of note
		int         b40            = -1;     // b40 pitch of note
		int         processed      = 0;      // has note been processed/matched
		int         sourceindex    = -1;     // source file index for note
		int         tpindex        = -1;     // timepoint index of note in source
		vector<int> matched;       // indexes to the location of the note in TimePoint list.
		                           // the index indicate which score the match is related to,
		                           // and a value of -1 means there is no equivalent timepoint.
		void clear(void) {
			token = NULL;
			subtoken = "";
			subindex = -1;
			measure = -1;
			measurequarter = -1;
			track = -1;
			layer = -1;
			duration = -1;
			b40 = -1;
			processed = 0;
			sourceindex = -1;
			tpindex = -1;
			matched.clear();
		}
};


// Function declarations:

class Tool_humdiff : public HumTool {
	public:
		         Tool_humdiff       (void);

		bool     run                (HumdrumFileSet& infiles);

	protected:
		void     compareFiles       (HumdrumFile& reference, HumdrumFile& alternate);

		void     compareTimePoints  (vector<vector<TimePoint>>& timepoints, HumdrumFile& reference, HumdrumFile& alternate);
		void     extractTimePoints  (vector<TimePoint>& points, HumdrumFile& infile);
		void     printTimePoints    (vector<TimePoint>& timepoints);
		void     compareLines       (HumNum minval, vector<int>& indexes, vector<vector<TimePoint>>& timepoints, vector<HumdrumFile*> infiles);
		void     getNoteList        (vector<NotePoint>& notelist, HumdrumFile& infile, int line, int measure, int sourceindex, int tpindex);
		int      findNoteInList     (NotePoint& np, vector<NotePoint>& nps);
		void     printNotePoints    (vector<NotePoint>& notelist);
		void     markNote           (NotePoint& np);

	private:
		int m_marked = 0;


};

ostream& operator<<(ostream& out, TimePoint& tp);
ostream& operator<<(ostream& out, NotePoint& np);



class Tool_humsort : public HumTool {
	public:
		         Tool_humsort      (void);
		        ~Tool_humsort      () {};

		bool     run               (HumdrumFileSet& infiles);
		bool     run               (HumdrumFile& infile);
		bool     run               (const string& indata, ostream& out);
		bool     run               (HumdrumFile& infile, ostream& out);

	protected:
		void    processFile        (HumdrumFile& infile);

};


class Tool_imitation : public HumTool {
	public:
		         Tool_imitation    (void);
		        ~Tool_imitation    () {};

		bool     run               (HumdrumFileSet& infiles);
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
		bool m_single = false;
		static int Enumerator;
		bool m_first = false;
		bool m_nozero = false;
		bool m_onlyzero = false;
		bool m_measure = false;
		bool m_beat    = false;
		bool m_length  = false;

		bool m_noInfo = false;

		bool m_noN    = false;
		bool m_noC    = false;
		bool m_noD    = false;
		bool m_noI    = false;

		bool m_noNN   = false;
		bool m_noCC   = false;
		bool m_noDD   = false;
		bool m_noII   = false;

		bool m_addsearches  = false;
		bool m_inversion  = false;
		bool m_retrograde = false;

		vector<int> m_barlines;
};


class Tool_kern2mens : public HumTool {
	public:
		         Tool_kern2mens           (void);
		        ~Tool_kern2mens           () {};

		bool     run                      (HumdrumFileSet& infiles);
		bool     run                      (HumdrumFile& infile);
		bool     run                      (const string& indata, ostream& out);
		bool     run                      (HumdrumFile& infile, ostream& out);

	protected:
		void     convertToMens            (HumdrumFile& infile);
		string   convertKernTokenToMens   (HTp token);
		void     printBarline             (HumdrumFile& infile, int line);

	private:
		bool     m_numbersQ   = true;      // used with -N option
		bool     m_measuresQ  = true;      // used with -M option
		bool     m_invisibleQ = true;      // used with -I option
		bool     m_doublebarQ = true;      // used with -D option
		string   m_clef;                   // used with -c option

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
		void   parseHarm            (xml_node harm, HumNum starttime);
		void   parseTempo           (xml_node tempo, HumNum starttime);
		void   parseDir             (xml_node dir, HumNum starttime);
		HumNum getDuration          (xml_node element);
		string getHumdrumPitch      (xml_node note, vector<xml_node>& children);
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
		string getChildAccidGes      (vector<xml_node>& children);
		string getChildAccidVis      (vector<xml_node>& children);

		// static functions
		static string accidToKern(const string& accid);

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
		vector<bool>   m_hasHarm;
		const int      m_maxstaff = 1000;

		bool           m_fermata = false;     // set priority of note/fermata over note@fermata
		vector<grace_info> m_gracenotes;      // buffer for storing grace notes
		HumNum			m_gracetime = 0;       // performance time of buffered grace notes

		vector<hairpin_info> m_hairpins;

		map<string, vector<xml_node>> m_startlinks;
		map<string, vector<xml_node>> m_stoplinks;

};



class WordInfo {
	public:
		string word;                 // text of word
		int notes = 0;               // number of notes in word
		HumNum starttime;            // start time of word
		HumNum endtime;              // end time of word
		int bar = 0;                 // starting barline number for word
	  	vector<int> bars;            // starting barline number for each syllable
		vector<string> syllables;    // list of syllables in word with melisma
		vector<int> notecounts;      // list of note counts for each syllable in word
		vector<HumNum> starttimes;   // list of start times for each syllable
		vector<HumNum> endtimes;     // list of end times for each syllable
		HumNum duration(void) { return endtime - starttime; }
		string name;
		string abbreviation;
		int partnum = 0;
		void clear(void) {
			starttime = 0;
			endtime   = 0;
			partnum   = 0;
			notes     = 0;
			bar       = 0;
			abbreviation.clear();
			notecounts.clear();
			starttimes.clear();
			syllables.clear();
			endtimes.clear();
			word.clear();
			name.clear();
			bars.clear();
		}
};


class Tool_melisma : public HumTool {
	public:
		      Tool_melisma             (void);
		     ~Tool_melisma             () {};

		bool  run                      (HumdrumFileSet& infiles);
		bool  run                      (HumdrumFile& infile);
		bool  run                      (const string& indata, ostream& out);
		bool  run                      (HumdrumFile& infile, ostream& out);

	protected:
		void   initialize              (HumdrumFile& infile);
		void   processFile             (HumdrumFile& infile);
		void   getNoteCounts           (HumdrumFile& infile, vector<vector<int>>& counts);
		void   getNoteCountsForLyric   (vector<vector<int>>& counts, HTp lyricStart);
		int    getCountForSyllable     (HTp token);
		void   replaceLyrics           (HumdrumFile& infile, vector<vector<int>>& counts);
		void   markMelismas            (HumdrumFile& infile, vector<vector<int>>& counts);
		void   markMelismaNotes        (HTp text, int count);
		void   extractWordlist         (vector<WordInfo>& wordinfo, map<string, int>& wordlist,
		                                HumdrumFile& infile, vector<vector<int>>& notecount);
		string extractWord             (WordInfo& winfo, HTp token, vector<vector<int>>& counts);
		HumNum getEndtime              (HTp text);
		void   printWordlist           (HumdrumFile& infile, vector<WordInfo>& wordinfo,
		                                map<string, int>);
		void   initializePartInfo      (HumdrumFile& infile);
		void   getMelismaNoteCounts    (vector<int>& ncounts, vector<int>& mcounts,
		                                HumdrumFile& infile);
		double getScoreDuration        (HumdrumFile& infile);
		void   initBarlines            (HumdrumFile& infile);

	private:
		vector<vector<HumNum>> m_endtimes;      // end time of syllables indexed by line/field
		vector<string>         m_names;         // name of parts indexed by track
		vector<string>         m_abbreviations; // abbreviation of parts indexed by track
		vector<int>            m_partnums;      // part number index by track
		vector<int>            m_measures;      // current measure number

};



class Tool_metlev : public HumTool {
	public:
		      Tool_metlev      (void);
		     ~Tool_metlev      () {};

		bool  run              (HumdrumFileSet& infiles);
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

		bool     run               (HumdrumFileSet& infiles);
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


class Tool_musedata2hum : public HumTool {
	public:
		        Tool_musedata2hum    (void);
		       ~Tool_musedata2hum    () {}

		bool    convertFile          (ostream& out, const string& filename);
		bool    convertString        (ostream& out, const string& input);
		bool    convert              (ostream& out, MuseDataSet& mds);
		bool    convert              (ostream& out, istream& input);

		void    setOptions           (int argc, char** argv);
		void    setOptions           (const std::vector<std::string>& argvlist);
		Options getOptionDefinitions (void);
		void    setInitialOmd        (const string& omd);

	protected:
		void    initialize           (void);
		void    convertLine          (GridMeasure* gm, MuseRecord& mr);
		bool    convertPart          (HumGrid& outdata, MuseDataSet& mds, int index);
		int     convertMeasure       (HumGrid& outdata, MuseData& part, int partindex, int startindex);
		GridMeasure* getMeasure      (HumGrid& outdata, HumNum starttime);
		void    setTimeSigDurInfo    (const std::string& mtimesig);
		void    setMeasureStyle      (GridMeasure* gm, MuseRecord& mr);
		void    setMeasureNumber     (GridMeasure* gm, MuseRecord& mr);
		void    storePartName        (HumGrid& outdata, MuseData& part, int index);
		void    addNoteDynamics      (GridSlice* slice, int part, 
		                              MuseRecord& mr);
		void    addLyrics            (GridSlice* slice, int part, int staff, MuseRecord& mr);
		void    addFiguredHarmony    (MuseRecord& mr, GridMeasure* gm,
		                              HumNum timestamp, int part, int maxstaff);
		std::string trimSpaces       (std::string input);
		void    addTextDirection     (GridMeasure* gm, int part, int staff,
		                              MuseRecord& mr, HumNum timestamp);

	private:
		// options:
		Options m_options;
		bool    m_stemsQ = false;    // used with -s option
		bool    m_recipQ = false;    // used with -r option
		std::string m_omd = "";      // initial tempo designation (store for later output)

		// state variables:
		int m_part     = 0;          // staff index currently being processed
		int m_maxstaff = 0;          // total number of staves (parts)
		HumNum m_timesigdur = 4;     // duration of current time signature in quarter notes
		HTp m_lastfigure = NULL;     // last figured bass token
		int m_lastbarnum = -1;       // barnumber carried over from previous bar
		HTp m_lastnote = NULL;       // for dealing with chords.

};



class MusicXmlHarmonyInfo {
	public:
		HTp    token;
		HumNum timestamp;
		int    partindex;
};


class Tool_musicxml2hum : public HumTool {
	public:
		        Tool_musicxml2hum    (void);
		       ~Tool_musicxml2hum    () {}

		bool    convertFile          (ostream& out, const char* filename);
		bool    convert              (ostream& out, pugi::xml_document& infile);
		bool    convert              (ostream& out, const char* input);
		bool    convert              (ostream& out, istream& input);

		void    setOptions           (int argc, char** argv);
		void    setOptions           (const std::vector<std::string>& argvlist);
		Options getOptionDefinitions (void);

	protected:
		void   initialize           (void);
		std::string getChildElementText  (pugi::xml_node root, const char* xpath);
		std::string getChildElementText  (pugi::xpath_node root, const char* xpath);
		std::string getAttributeValue    (pugi::xml_node xnode, const std::string& target);
		std::string getAttributeValue    (xpath_node xnode, const std::string& target);
		void   printAttributes      (pugi::xml_node node);
		bool   getPartInfo          (map<std::string, pugi::xml_node>& partinfo,
		                             std::vector<std::string>& partids, pugi::xml_document& doc);
		bool   stitchParts          (HumGrid& outdata,
		                             std::vector<std::string>& partids,
		                             map<std::string, pugi::xml_node>& partinfo,
		                             map<std::string, pugi::xml_node>& partcontent,
		                             std::vector<MxmlPart>& partdata);
		bool   getPartContent       (map<std::string, pugi::xml_node>& partcontent,
		                             std::vector<std::string>& partids, pugi::xml_document& doc);
		void   printPartInfo        (std::vector<std::string>& partids,
		                             map<std::string, pugi::xml_node>& partinfo,
		                             map<std::string, pugi::xml_node>& partcontent,
		                             std::vector<MxmlPart>& partdata);
		bool   fillPartData         (std::vector<MxmlPart>& partdata,
		                             const std::vector<std::string>& partids,
		                             map<std::string, pugi::xml_node>& partinfo,
		                             map<std::string, pugi::xml_node>& partcontent);
		bool   fillPartData         (MxmlPart& partdata, const std::string& id,
		                             pugi::xml_node partdeclaration,
		                             pugi::xml_node partcontent);
		void   appendZeroEvents     (GridMeasure* outfile,
		                             std::vector<SimultaneousEvents*>& nowevents,
		                             HumNum nowtime,
		                             std::vector<MxmlPart>& partdata);
		void   appendNonZeroEvents   (GridMeasure* outdata,
		                              std::vector<SimultaneousEvents*>& nowevents,
		                              HumNum nowtime,
		                              std::vector<MxmlPart>& partdata);
		void   addGraceLines         (GridMeasure* outdata,
		                              std::vector<std::vector<std::vector<std::vector<MxmlEvent*>>>>& notes,
		                              std::vector<MxmlPart>& partdata, HumNum nowtime);
		void   addEventToList        (std::vector<std::vector<std::vector<std::vector<MxmlEvent*>>>>& list,
		                              MxmlEvent* event);
		void   addHeaderRecords      (HumdrumFile& outfile, pugi::xml_document& doc);
		void   addFooterRecords      (HumdrumFile& outfile, pugi::xml_document& doc);
		std::string& cleanSpaces     (std::string& input);
		std::string cleanSpacesAndColons(const std::string& input);
		void setEditorialAccidental  (int accidental, GridSlice* slice,
		                              int partindex, int staffindex, int voiceindex);

		bool convert          (ostream& out);
		bool convertPart      (ostream& out, const std::string& partname,
		                       int partindex);
		bool insertMeasure    (HumGrid& outdata, int mnum,
		                       std::vector<MxmlPart>& partdata,
		                       std::vector<int> partstaves);
		bool convertNowEvents (GridMeasure* outdata,
		                       std::vector<SimultaneousEvents*>& nowevents,
		                       std::vector<int>& nowparts,
		                       HumNum nowtime,
		                       std::vector<MxmlPart>& partdata,
		                       std::vector<int>& partstaves);
		void appendNullTokens (HumdrumLine* line, MxmlPart& part);
		void appendEvent      (HumdrumLine* line, MxmlEvent* event);
		void insertExclusiveInterpretationLine(HumdrumFile& outfile,
		                       std::vector<MxmlPart>& partdata);
		void insertAllToken   (HumdrumFile& outfile, std::vector<MxmlPart>& partdata,
		                       const std::string& common);
		void insertSingleMeasure(HumdrumFile& outfile);
		void cleanupMeasures   (HumdrumFile& outfile,
		                        std::vector<HumdrumLine*> measures);
		void processPrintElement(GridMeasure* outdata, pugi::xml_node element, HumNum timestamp);
		void insertOffsetHarmonyIntoMeasure(GridMeasure* gm);

		void addClefLine       (GridMeasure* outdata, std::vector<std::vector<pugi::xml_node>>& clefs,
		                        std::vector<MxmlPart>& partdata, HumNum nowtime);
		void addOttavaLine     (GridMeasure* outdata, std::vector<std::vector<pugi::xml_node>>& ottavas,
		                        std::vector<MxmlPart>& partdata, HumNum nowtime);
		void insertPartClefs   (pugi::xml_node clef, GridPart& part);
		void insertPartOttavas (pugi::xml_node ottava, GridPart& part, int partindex, int partstaffindex, int staffcount);
		pugi::xml_node convertClefToHumdrum(pugi::xml_node clef, HTp& token, int& staffindex);
		pugi::xml_node convertOttavaToHumdrum(pugi::xml_node ottava, HTp& token, int& staffindex,
		                        int partindex, int partstaffindex, int staffcount);

		void addTranspositionLine(GridMeasure* outdata, std::vector<std::vector<pugi::xml_node>>& transpositions,
		                       std::vector<MxmlPart>& partdata, HumNum nowtime);
		void addKeySigLine    (GridMeasure* outdata, std::vector<std::vector<pugi::xml_node>>& keysigs,
		                        std::vector<MxmlPart>& partdata, HumNum nowtime);
		void addKeyDesignationLine(GridMeasure* outdata, vector<vector<xml_node>>& keydesigs,
		                        vector<MxmlPart>& partdata, HumNum nowtime);
		void insertPartKeySigs (pugi::xml_node keysig, GridPart& part);
		void insertPartKeyDesignations(xml_node keydeg, GridPart& part);
		pugi::xml_node convertKeySigToHumdrum(pugi::xml_node keysig,
		                        HTp& token, int& staffindex);
		pugi::xml_node convertKeySigToHumdrumKeyDesignation(xml_node keysig,
		                        HTp& token, int& staffindex);

		void addTimeSigLine    (GridMeasure* outdata, std::vector<std::vector<pugi::xml_node>>& timesigs,
		                        std::vector<MxmlPart>& partdata, HumNum nowtime);
		bool insertPartTimeSigs (pugi::xml_node timesig, GridPart& part);
		void insertPartMensurations(pugi::xml_node timesig, GridPart& part);
		void insertPartNames    (HumGrid& outdata, std::vector<MxmlPart>& partdata);
		bool checkForMensuration(pugi::xml_node timesig);
		pugi::xml_node convertTimeSigToHumdrum(pugi::xml_node timesig,
		                        HTp& token, int& staffindex);
		pugi::xml_node convertMensurationToHumdrum(pugi::xml_node timesig,
		                        HTp& token, int& staffindex);

		void addEvent          (GridSlice* slice, GridMeasure* outdata, MxmlEvent* event, HumNum nowtime);
		void fillEmpties       (GridPart* part, const char* string);
		void addSecondaryChordNotes (ostream& output, MxmlEvent* head, const std::string& recip);
		bool isInvisible       (MxmlEvent* event);
		int  addLyrics         (GridStaff* staff, MxmlEvent* event);
		int  addHarmony        (GridPart* oart, MxmlEvent* event, HumNum nowtime, int partindex);
		void addDynamic        (GridPart* part, MxmlEvent* event, int partindex);
		void addHairpinEnding  (GridPart* part, MxmlEvent* event, int partindex);
		void addFiguredBass    (GridPart* part, MxmlEvent* event);
		void addTexts          (GridSlice* slice, GridMeasure* measure, int partindex,
		                        int staffindex, int voiceindex, MxmlEvent* event);
		void addText           (GridSlice* slice, GridMeasure* measure, int partindex,
		                        int staffindex, int voiceindex, pugi::xml_node node);
		int         getHarmonyOffset(pugi::xml_node hnode);
		std::string getHarmonyString(pugi::xml_node hnode);
		std::string getDynamicString(pugi::xml_node element);
		std::string getDynamicsParameters(pugi::xml_node element);
		std::string getFiguredBassString(pugi::xml_node element);
		std::string getFiguredBassParameters(pugi::xml_node element);
		std::string convertFiguredBassNumber(const xml_node& figure);
		std::string getHairpinString(pugi::xml_node element, int partindex);
		std::string cleanSpaces     (const std::string& input);
		void checkForDummyRests(MxmlMeasure* measure);
		void reindexVoices     (std::vector<MxmlPart>& partdata);
		void reindexMeasure    (MxmlMeasure* measure);
		void setSoftwareInfo   (pugi::xml_document& doc);
		std::string getSystemDecoration(pugi::xml_document& doc, HumGrid& grid, std::vector<std::string>& partids);
		void getChildrenVector (std::vector<pugi::xml_node>& children, pugi::xml_node parent);
		void insertPartTranspositions(pugi::xml_node transposition, GridPart& part);
		pugi::xml_node convertTranspositionToHumdrum(pugi::xml_node transpose, HTp& token, int& staffindex);
		void prepareRdfs       (std::vector<MxmlPart>& partdata);
		void printRdfs         (ostream& out);
		void printResult       (ostream& out, HumdrumFile& outfile);

	public:

	static bool nodeType      (pugi::xml_node node, const char* testname);

	private:
		Options m_options;
		bool DebugQ;
		bool VoiceDebugQ;
		bool m_recipQ       = false;
		bool m_stemsQ       = false;
		int  m_slurabove    = 0;
		int  m_slurbelow    = 0;
		char m_hasEditorial = '\0';
		bool m_hasOrnamentsQ = false;
		std::vector<std::vector<std::string>> m_last_ottava_direction;
		std::vector<MusicXmlHarmonyInfo> offsetHarmony;
		std::vector<string> m_stop_char;

		// RDF indications in **kern data:
		std::string  m_caesura_rdf;

		std::string m_software;
		std::string m_systemDecoration;

		std::vector<std::vector<pugi::xml_node>> m_current_dynamic;
		pugi::xml_node m_current_figured_bass = pugi::xml_node(NULL);
		std::vector<std::pair<int, pugi::xml_node>> m_current_text;

		bool m_hasTransposition = false;

		// m_forceRecipQ is used to force the display of the **recip spint
		// when a data line contains no notes or rests.  This is used for
		// harmony/dynamics side spines.
		bool m_forceRecipQ = false;

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

		bool     run                   (HumdrumFileSet& infiles);
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



class Tool_pccount : public HumTool {
	public:
		      Tool_pccount              (void);
		     ~Tool_pccount              () {};

		bool  run                       (HumdrumFileSet& infiles);
		bool  run                       (HumdrumFile& infile);
		bool  run                       (const string& indata, ostream& out);
		bool  run                       (HumdrumFile& infile, ostream& out);
 
	protected:
		void   initialize               (HumdrumFile& infile);
		void   processFile              (HumdrumFile& infile);
		void   initializePartInfo       (HumdrumFile& infile);
		void   addCounts                (HTp sstart, HTp send);
		void   countPitches             (HumdrumFile& infile);
		void   printHumdrumTable        (void);
		void   printPitchClassList      (void);
		void   printVegaLiteJsonTemplate(const std::string& datavariable, HumdrumFile& infile);
		void   printVegaLiteJsonData    (void);
		void   printVoiceList           (void);
		void   printReverseVoiceList    (void);
		void   printColorList           (void);
		std::string getPitchClassString (int b40);
		void   printVegaLiteScript      (const string& jsonvar,
		                                 const string& target,
		                                 const string& datavar,
		                                 HumdrumFile& infile);
		void   printVegaLiteHtml        (const string& jsonvar,
		                                 const string& target,
		                                 const string& datavar,
		                                 HumdrumFile& infile);
		void   printVegaLitePage        (const string& jsonvar,
		                                 const string& target,
		                                 const string& datavar,
		                                 HumdrumFile& infile);
		std::string getFinal            (HumdrumFile& infile);
		double  getPercent              (const string& pitchclass);
		int     getCount                (const string& pitchclass);
		void    setFactorMaximum        (void);
		void    setFactorNormalize      (void);

	private:
		std::vector<int>               m_rkern;
		std::vector<int>               m_parttracks;
		std::vector<std::string>       m_names;
		std::vector<std::string>       m_abbreviations;
		std::vector<std::vector<double>> m_counts;
		bool m_attack       = false;
		bool m_full         = false;
		bool m_doublefull   = false;
		bool m_normalize    = false;
		bool m_maximum      = false;
		bool m_template     = false;
		bool m_data         = false;
		bool m_script       = false;
		bool m_html         = false;
		bool m_page         = false;
		int  m_width        = 500;
		double m_ratio      = 0.67;
		bool m_key          = true;
		double m_factor     = 1.0;
		int m_maxpc         = 0;
		std::string m_title = "";
		std::string m_id    = "id";
		std::map<std::string, std::string> m_vcolor;

};





class Tool_periodicity : public HumTool {
	public:
		         Tool_periodicity   (void);
		        ~Tool_periodicity   () {};

		bool     run                (HumdrumFileSet& infiles);
		bool     run                (HumdrumFile& infile);
		bool     run                (const string& indata, ostream& out);
		bool     run                (HumdrumFile& infile, ostream& out);

	protected:
		void     initialize         (HumdrumFile& infile);
		void     processFile        (HumdrumFile& infile);
		void     fillAttackGrids    (HumdrumFile& infile, vector<vector<double>>& grids, HumNum minrhy);
		void     printAttackGrid    (ostream& out, HumdrumFile& infile, vector<vector<double>>& grids, HumNum minrhy);
		void     doAnalysis         (vector<vector<double>>& analysis, int level, vector<double>& grid);
		void     doPeriodicityAnalysis(vector<vector<double>> & analysis, vector<double>& grid, HumNum minrhy);
		void     printPeriodicityAnalysis(ostream& out, vector<vector<double>>& analysis);
		void     printSvgAnalysis(ostream& out, vector<vector<double>>& analysis, HumNum minrhy);
		void     getColorMapping(double input, double& hue, double& saturation, double& lightness);

	private:

};


class Tool_phrase : public HumTool {
	public:
		     Tool_phrase          (void);
		    ~Tool_phrase          () {};

		bool  run                 (HumdrumFileSet& infiles);
		bool  run                 (HumdrumFile& infile);
		bool  run                 (const string& indata, ostream& out);
		bool  run                 (HumdrumFile& infile, ostream& out);

	protected:
		void  analyzeSpineByRests (int index);
		void  analyzeSpineByPhrase(int index);
		void  initialize          (HumdrumFile& infile);
		void  prepareAnalysis     (HumdrumFile& infile);
		void  addAverageLines     (HumdrumFile& infile);
		bool  hasPhraseMarks      (HTp start);
		void  removePhraseMarks   (HTp start);

	private:
		vector<vector<string>>    m_results;
		vector<HTp>               m_starts;
		HumdrumFile               m_infile;
		vector<int>               m_pcount;
		vector<HumNum>            m_psum;
		bool                      m_markQ;
		bool                      m_removeQ;
		bool                      m_remove2Q;
		bool                      m_averageQ;
		string                    m_color;

};



class Tool_pnum : public HumTool {
	public:
		      Tool_pnum               (void);
		     ~Tool_pnum               () {};

		bool  run                     (HumdrumFileSet& infiles);
		bool  run                     (HumdrumFile& infile);
		bool  run                     (const string& indata, ostream& out);
		bool  run                     (HumdrumFile& infile, ostream& out);

	protected:
		void  initialize              (HumdrumFile& infile);
		void  processFile             (HumdrumFile& infile);
		std::string convertSubtokenToBase(const std::string& text);
		void  convertTokenToBase      (HTp token);

	private:
		int  m_base = 12;
		int  m_midiQ;
		bool m_durationQ;
		bool m_classQ;
		bool m_octaveQ;
		bool m_attacksQ;
		std::string m_rest;
		bool m_restQ;

};



class Tool_recip : public HumTool {
	public:
		      Tool_recip               (void);
		     ~Tool_recip               () {};

		bool  run                      (HumdrumFileSet& infiles);
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
		string      m_kernpitch = "e";

};



class Tool_restfill : public HumTool {
	public:
		         Tool_restfill         (void);
		        ~Tool_restfill         () {};

		bool        run                (HumdrumFileSet& infiles);
		bool        run                (HumdrumFile& infile);
		bool        run                (const string& indata, ostream& out);
		bool        run                (HumdrumFile& infile, ostream& out);

	protected:
		void        processFile        (HumdrumFile& infile);
		void        initialize         (void);
		bool        hasBlankMeasure    (HTp start);
		void        fillInRests        (HTp start);
		void        addRest            (HTp cell, HumNum duration);
		HumNum      getNextTime        (HTp token);

	private:
		bool        m_hiddenQ  = false;
		std::string m_exinterp = "**kern";

};


class Tool_ruthfix : public HumTool {
	public:
		         Tool_ruthfix      (void);
		        ~Tool_ruthfix      () {};

		bool     run               (HumdrumFileSet& infiles);
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

		bool     run             (HumdrumFileSet& infiles);
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



class Tool_shed : public HumTool {
	public:
		         Tool_shed       (void);
		        ~Tool_shed       () {};

		bool     run               (HumdrumFileSet& infiles);
		bool     run               (HumdrumFile& infile);
		bool     run               (const string& indata, ostream& out);
		bool     run               (HumdrumFile& infile, ostream& out);

	protected:
		void    processFile        (HumdrumFile& infile);
		void    searchAndReplaceInterpretation(HumdrumFile& infile);
		void    searchAndReplaceExinterp(HumdrumFile& infile);
		void    searchAndReplaceData(HumdrumFile& infile);
		void    searchAndReplaceBarline(HumdrumFile& infile);
		void    searchAndReplaceLocalComment(HumdrumFile& infile);
		void    initialize         (void);
		void    initializeSegment  (HumdrumFile& infile);
		bool    isValid            (HTp token);
		bool    isValidDataType    (HTp token);
		bool    isValidSpine       (HTp token);
		std::vector<std::string> addToExInterpList(void);
		void    parseExpression    (const string& value);
		void    prepareSearch      (int index);
		std::string getExInterp    (const string& value);

	private:
		std::vector<std::string> m_searches;  // search strings
		std::vector<std::string> m_replaces;  // replace strings
		std::vector<std::string> m_options;   // search options

		std::string m_search;
		std::string m_replace;
		std::string m_option;

		bool m_data           = true;  // process data
		bool m_barline        = false; // process barlines
		bool m_exinterp       = false; // process exclusive interpretations
		bool m_interpretation = false; // process interpretations
		bool m_localcomment   = false; // process local comments
		std::string m_xInterp; // used with -x option
		std::string m_yInterp; // used with -y option
		std::string m_zInterp; // used with -z option

		bool m_modified       = false;

		// list of exclusive interpretations to process
		std::vector<std::string> m_exinterps; 

		std::vector<bool> m_spines; // usar with -s option
		std::string m_grepoptions;

};


class MeasureData {
	public:
		            MeasureData               (void);
		            MeasureData               (HumdrumFile& infile,
		                                       int startline,int stopline);
		            MeasureData               (HumdrumFile* infile,
		                                      int startline,int stopline);
		           ~MeasureData               ();
		void        setOwner                  (HumdrumFile* infile);
		void        setOwner                  (HumdrumFile& infile);
		void        setStartLine              (int startline);
		void        setStopLine               (int stopline);
		int         getStartLine              (void);
		int         getStopLine               (void);
		void        clear                     (void);
		std::vector<double>& getHistogram7pc (void);
		void        generateNoteHistogram     (void);
		double      getSum7pc                 (void);
		double      getStartTime              (void);
		double      getStopTime               (void);
		double      getDuration               (void);
		int         getMeasure                (void);
		std::string getQoff                   (void);
		std::string getQon                    (void);
		double      getScoreDuration          (void);

	private:
		HumdrumFile*        m_owner       = NULL;
		int                 m_startline   = -1;
		int                 m_stopline    = -1;
		std::vector<double> m_hist7pc;
		double              m_sum7pc      = 0.0;
};



class MeasureDataSet {
	public:
		             MeasureDataSet   (void);
		             MeasureDataSet   (HumdrumFile& infile);
		            ~MeasureDataSet   ();

		void         clear            (void);
		int          parse            (HumdrumFile& infile);
		MeasureData& operator[]       (int index);
		int          size             (void) { return (int)m_data.size(); }
		double       getScoreDuration (void);

	private:
		std::vector<MeasureData*> m_data;
};



class MeasureComparison {
	public:
		MeasureComparison();
		MeasureComparison(MeasureData& data1, MeasureData& data2);
		MeasureComparison(MeasureData* data1, MeasureData* data2);
		~MeasureComparison();

		void clear(void);
		void compare(MeasureData& data1, MeasureData& data2);
		void compare(MeasureData* data1, MeasureData* data2);

		double getCorrelation7pc(void);

	protected:
		double correlation7pc = 0.0;
};



class MeasureComparisonGrid {
	public:
		             MeasureComparisonGrid     (void);
		             MeasureComparisonGrid     (MeasureDataSet& set1, MeasureDataSet& set2);
		             MeasureComparisonGrid     (MeasureDataSet* set1, MeasureDataSet* set2);
		            ~MeasureComparisonGrid     ();

		void         clear                     (void);
		void         analyze                   (MeasureDataSet& set1, MeasureDataSet& set2);
		void         analyze                   (MeasureDataSet* set1, MeasureDataSet* set2);

		double       getStartTime1             (int index);
		double       getStopTime1              (int index);
		double       getDuration1              (int index);
		int          getMeasure1               (int index);
		std::string  getQon1                   (int index);
		std::string  getQoff1                  (int index);
		double       getScoreDuration1         (void);
		double       getStartTime2             (int index);
		double       getStopTime2              (int index);
		double       getDuration2              (int index);
		int          getMeasure2               (int index);
		std::string  getQon2                   (int index);
		std::string  getQoff2                  (int index);
		double       getScoreDuration2         (void);

		ostream&     printCorrelationGrid      (ostream& out = std::cout);
		ostream&     printCorrelationDiagonal  (ostream& out = std::cout);
		ostream&     printSvgGrid              (ostream& out = std::cout);
		void         getColorMapping           (double input, double& hue, double& saturation,
				 double& lightness);

	private:
		std::vector<std::vector<MeasureComparison>> m_grid;
		MeasureDataSet* m_set1 = NULL;
		MeasureDataSet* m_set2 = NULL;
};



class Tool_simat : public HumTool {
	public:
		         Tool_simat         (void);
		        ~Tool_simat         () {};

		bool     run                (HumdrumFileSet& infiles);
		bool     run                (HumdrumFile& infile1, HumdrumFile& infile2);
		bool     run                (const string& indata1, const string& indata2, ostream& out);
		bool     run                (HumdrumFile& infile1, HumdrumFile& infile2, ostream& out);

	protected:
		void     initialize         (HumdrumFile& infile1, HumdrumFile& infile2);
		void     processFile        (HumdrumFile& infile1, HumdrumFile& infile2);

	private:
		MeasureDataSet        m_data1;
		MeasureDataSet        m_data2;
		MeasureComparisonGrid m_grid;

};


class Tool_slurcheck : public HumTool {
	public:
		         Tool_slurcheck    (void);
		        ~Tool_slurcheck    () {};

		bool     run               (HumdrumFileSet& infiles);
		bool     run               (HumdrumFile& infile);
		bool     run               (const string& indata, ostream& out);
		bool     run               (HumdrumFile& infile, ostream& out);

	protected:
		void     processFile       (HumdrumFile& infile);
		void     initialize        (void);

	private:

};


class Tool_spinetrace : public HumTool {
	public:
		      Tool_spinetrace          (void);
		     ~Tool_spinetrace          () {};

		bool  run                      (HumdrumFileSet& infiles);
		bool  run                      (HumdrumFile& infile);
		bool  run                      (const string& indata, ostream& out);
		bool  run                      (HumdrumFile& infile, ostream& out);

	protected:
		void  initialize               (HumdrumFile& infile);
		void  processFile              (HumdrumFile& infile);

	private:

};



class Tool_tabber : public HumTool {
	public:
		      Tool_tabber              (void);
		     ~Tool_tabber              () {};

		bool  run                      (HumdrumFileSet& infiles);
		bool  run                      (HumdrumFile& infile);
		bool  run                      (const string& indata, ostream& out);
		bool  run                      (HumdrumFile& infile, ostream& out);

	protected:
		void  initialize               (HumdrumFile& infile);
		void  processFile              (HumdrumFile& infile);

	private:

};



class Tool_tassoize : public HumTool {
	public:
		         Tool_tassoize   (void);
		        ~Tool_tassoize   () {};

		bool     run                (HumdrumFileSet& infiles);
		bool     run                (HumdrumFile& infile);
		bool     run                (const string& indata, ostream& out);
		bool     run                (HumdrumFile& infile, ostream& out);

	protected:
		void     initialize         (HumdrumFile& infile);
		void     processFile        (HumdrumFile& infile);
		void     updateKeySignatures(HumdrumFile& infile, int lineindex);
		void     checkDataLine      (HumdrumFile& infile, int lineindex);
		void     clearStates        (void);
		void     addBibliographicRecords(HumdrumFile& infile);
		void     deleteBreaks       (HumdrumFile& infile);
		void     fixEditorialAccidentals(HumdrumFile& infile);
		void     fixInstrumentAbbreviations(HumdrumFile& infile);
		void     addTerminalLongs   (HumdrumFile& infile);
		void     deleteDummyTranspositions(HumdrumFile& infile);
		string   getDate            (void);

	private:
		vector<vector<int>> m_pstates;
		vector<vector<int>> m_kstates;
		vector<vector<bool>> m_estates;

};


class Tool_transpose : public HumTool {
	public:
		         Tool_transpose  (void);
		        ~Tool_transpose  () {};

		bool     run             (HumdrumFileSet& infiles);
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
		void     printHumdrumMxhmToken(HumdrumLine& record, int index,
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
		int      ssettonicQ   = 0;   // used with -k option
		int      ssettonic    = 0;   // used with -k option
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



class Tool_trillspell : public HumTool {
	public:
		      Tool_trillspell     (void);
		     ~Tool_trillspell     () {};

		bool  run                 (HumdrumFileSet& infiles);
		bool  run                 (HumdrumFile& infile);
		bool  run                 (const string& indata, ostream& out);
		bool  run                 (HumdrumFile& infile, ostream& out);

	protected:
		void  processFile         (HumdrumFile& infile);
		bool  analyzeOrnamentAccidentals(HumdrumFile& infile);
		void  resetDiatonicStatesWithKeySignature(vector<int>& states,
		                                          vector<int>& signature);
		void  fillKeySignature    (vector<int>& states, const string& keysig);
		int   getBase40           (int diatonic, int accidental);

	private:
		bool m_xmark = false;

};



} // end of namespace hum


#endif /* _HUMLIB_H_INCLUDED */



