//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Oct 18 12:01:36 PDT 2016
// Last Modified: Tue Oct 18 12:01:41 PDT 2016
// Filename:      GridToken.h
// URL:           https://github.com/craigsapp/hum2ly/blob/master/include/GridToken.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   HumGrid is an intermediate container for converting from
//                MusicXML syntax into Humdrum syntax.  GridToken is a class
//                which stores all information (notes, dynamics, lyrics, etc)
//                for a particular part (which may have more than one
//                staff.
//
//

#ifndef _GRIDTOKEN_H
#define _GRIDTOKEN_H

#include "humlib.h"

using namespace std;

class GridSlice;

namespace hum {


class GridToken {
	public:
		GridToken(void);
		GridToken(HTp token, HumNum duration);
		GridToken(const char* token, HumNum duration);
		GridToken(const string& token, HumNum duration);
		~GridToken();

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

	protected:
		void   setTransfered      (bool state);

	private:
		HTp    m_token;
		HumNum m_nextdur;
		HumNum m_prevdur;
		bool   m_transfered;

	friend class GridSlice;
};


} // end namespace hum

#endif /* _GRIDTOKEN_H */



