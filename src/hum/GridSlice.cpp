//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Oct 16 16:08:05 PDT 2016
// Last Modified: Sun Oct 16 16:08:08 PDT 2016
// Filename:      GridSlice.cpp
// URL:           https://github.com/craigsapp/hum2ly/blob/master/src/GridSlice.cpp
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   GridSlice store a timestamp, a slice type and data for
//                all parts in the given slice.
//

#include "HumGrid.h"
#include "GridPart.h"

using namespace std;

namespace hum {


//////////////////////////////
//
// GridSlice::GridSlice -- Constructor.  If partcount is positive, then
//    allocate the desired number of parts (still have to allocate staves
//    in part before using).
// default value: partcount = 0
//

GridSlice::GridSlice(HumNum timestamp, SliceType type, int partcount) {
	m_timestamp = timestamp;
	m_type = type;
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

GridSlice::GridSlice(HumNum timestamp, SliceType type, const GridSlice& slice) {
	m_timestamp = timestamp;
	m_type = type;
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


GridSlice::GridSlice(HumNum timestamp, SliceType type, GridSlice* slice) {
	m_timestamp = timestamp;
	m_type = type;
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

	// try to fit to two dots
	// try to fit to three dots


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
// GridSlice::transferTokens -- Create a HumdrumLine and append it to
//    the data.
//

void GridSlice::transferTokens(HumdrumFile& outfile, bool recip) {

	HTp token;
	HumdrumLine* line = new HumdrumLine;

	if (recip) {
		if (isNoteSlice()) {
			token = createRecipTokenFromDuration(getDuration());
		} else if (isClefSlice()) {
			token = new HumdrumToken("*");
		} else if (isMeasureSlice()) {
			token = new HumdrumToken("=");
		} else if (isInterpretationSlice()) {
			token = new HumdrumToken("*");
		} else {
			token = new HumdrumToken("55");
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
				token = new HumdrumToken(".a");
				line->appendToken(token);
			} else {
				for (v=0; v<(int)staff.size(); v++) {
					if (staff.at(v)->getToken()) {
						line->appendToken(staff.at(v)->getToken());
						staff.at(v)->forgetToken();
					} else {
						token = new HumdrumToken(".b");
						line->appendToken(token);
					}
				}
			}
		}
	}

	outfile.appendLine(line);
}



//////////////////////////////
//
// GridSlice::initializePartStaves --
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



} // end namespace hum



