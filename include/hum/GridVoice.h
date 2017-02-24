//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Oct 18 12:01:36 PDT 2016
// Last Modified: Tue Oct 18 12:01:41 PDT 2016
// Filename:      GridVoice.h
// URL:           https://github.com/craigsapp/hum2ly/blob/master/include/GridVoice.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   HumGrid is an intermediate container for converting from
//                MusicXML syntax into Humdrum syntax.  GridVoice is a class
//                which stores all information (notes, dynamics, lyrics, etc)
//                for a particular part (which may have more than one
//                staff.
//
//

#ifndef _GRIDVOICE_H
#define _GRIDVOICE_H

#include "humlib.h"

using namespace std;

class GridSlice;

namespace hum {


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

} // end namespace hum

#endif /* _GRIDVOICE_H */



