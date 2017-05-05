//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Oct 16 16:08:05 PDT 2016
// Last Modified: Sun Oct 16 16:08:08 PDT 2016
// Filename:      HumGrid.h
// URL:           https://github.com/craigsapp/hum2ly/blob/master/include/HumGrid.h
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   HumGrid is an intermediate container for converting from
//                MusicXML syntax into Humdrum syntax.
//

#ifndef _HUMGRID_H
#define _HUMGRID_H

#include "humlib.h"
#include "GridMeasure.h"
#include "GridSlice.h"

#include <vector>

using namespace std;


namespace hum {


class HumGrid : public vector<GridMeasure*> {
	public:
		HumGrid(void);
		~HumGrid();
		bool transferTokens   (HumdrumFile& outfile);
		int  getHarmonyCount  (int partindex);
		int  getVerseCount    (int partindex, int staffindex);
		void setVerseCount    (int partindex, int staffindex, int count);
		void setHarmonyCount  (int partindex, int count);
		void removeRedundantClefChanges(void);
		bool hasPickup         (void);

	protected:
		void calculateGridDurations        (void);
		void insertExclusiveInterpretationLine (HumdrumFile& outfile);
		void insertDataTerminationLine     (HumdrumFile& outfile);
		void appendMeasureLine             (HumdrumFile& outfile,
		                                    GridSlice& slice);
		void insertPartIndications         (HumdrumFile& outfile);
		void insertStaffIndications        (HumdrumFile& outfile);
		void addNullTokens                 (void);
		bool buildSingleList               (void);
		void extendDurationToken           (int slicei, int parti,
		                                    int staffi, int voicei);
		GridVoice* getGridVoice(int slicei, int parti, int staffi, int voicei);
		void addMeasureLines                (void);
		void addLastMeasure                 (void);
		bool manipulatorCheck               (void);
		GridSlice* manipulatorCheck         (GridSlice* ice1, GridSlice* ice2);
		void cleanupManipulators            (void);
		void cleanManipulator               (vector<GridSlice*>& newslices, 
		                                     GridSlice* curr);
		GridSlice* checkManipulatorContract (GridSlice* curr);
		void transferMerges                 (GridStaff* oldstaff,
		                                     GridStaff* oldlaststaff,
		                                     GridStaff* newstaff,
		                                     GridStaff* newlaststaff);
		void insertExInterpSides            (HumdrumLine* line, int part,
		                                     int staff);
		void insertSideTerminals            (HumdrumLine* line, int part, 
		                                     int staff);
		void insertSidePartInfo             (HumdrumLine* line, int part,
		                                     int staff);
		void insertSideStaffInfo            (HumdrumLine* line, int part,
		                                     int staff, int staffnum);
		void getMetricBarNumbers            (vector<int>& barnums);
		string  createBarToken              (int m, int barnum,
		                                     GridMeasure* measure);
		string getBarStyle                  (GridMeasure* measure);

	private:
		vector<GridSlice*>   m_allslices;
		vector<vector<int> > m_verseCount;
		vector<int>          m_harmonyCount;
		bool                 m_pickup;

		// options:
		bool m_recip;               // include **recip spine in output
		bool m_musicxmlbarlines;    // use measure numbers from <measure> element

};


} // end namespace hum

#endif /* _HUMGRID_H */



