//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Aug  6 10:53:40 CEST 2016
// Last Modified: Sun Sep 18 14:16:18 PDT 2016
// Filename:      musicxml2hum.cpp
// URL:           https://github.com/craigsapp/hum2ly/blob/master/src/musicxml2hum.cpp
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   Convert a MusicXML file into a Humdrum file.
//

#include "musicxml2hum.h"
#include "HumGrid.h"

#include <string.h>
#include <stdlib.h>

#include <algorithm>

using namespace std;
using namespace pugi;

namespace hum {


//////////////////////////////
//
// musicxml2hum_interface::musicxml2hum_interface --
//

musicxml2hum_interface::musicxml2hum_interface(void){
	// Options& options = m_options;
	// options.define("k|kern=b","display corresponding **kern data");
	VoiceDebugQ = false;
	DebugQ = false;
}



//////////////////////////////
//
// musicxml2hum_interface::convert -- Convert a MusicXML file into
//     Humdrum content.
//

bool musicxml2hum_interface::convertFile(ostream& out,
		const char* filename) {
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


bool musicxml2hum_interface::convert(ostream& out, istream& input) {
	string s(istreambuf_iterator<char>(input), {});
	return convert(out, s.c_str());
}


bool musicxml2hum_interface::convert(ostream& out, const char* input) {
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


bool musicxml2hum_interface::convert(ostream& out, xml_document& doc) {
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
	status &= stitchParts(outdata, partids, partinfo, partcontent, partdata);

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

	return status;
}



//////////////////////////////
//
// musicxml2hum_interface::reindexVoices --
//

void musicxml2hum_interface::reindexVoices(vector<MxmlPart>& partdata) {
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
// musicxml2hum_interface::reindexMeasure --
//

void musicxml2hum_interface::reindexMeasure(MxmlMeasure* measure) {
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
// musicxml2hum_interface::setOptions --
//

void musicxml2hum_interface::setOptions(int argc, char** argv) {
	m_options.process(argc, argv);
}


void musicxml2hum_interface::setOptions(const vector<string>& argvlist) {
	int tempargc = (int)argvlist.size();
	char* tempargv[tempargc+1];
	tempargv[tempargc] = NULL;

	int i;
	for (i=0; i<tempargc; i++) {
		tempargv[i] = new char[argvlist[i].size() + 1];
		strcpy(tempargv[i], argvlist[i].c_str());
	}

	setOptions(tempargc, tempargv);

	for (i=0; i<tempargc; i++) {
		if (tempargv[i] != NULL) {
			delete [] tempargv[i];
		}
	}
}



//////////////////////////////
//
// musicxml2hum_interface::getOptionDefinitions -- Used to avoid
//     duplicating the definitions in the test main() function.
//

Options musicxml2hum_interface::getOptionDefinitions(void) {
	return m_options;
}


///////////////////////////////////////////////////////////////////////////


//////////////////////////////
//
// musicxml2hum_interface::fillPartData --
//

bool musicxml2hum_interface::fillPartData(vector<MxmlPart>& partdata,
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


bool musicxml2hum_interface::fillPartData(MxmlPart& partdata,
		const string& id, xml_node partdeclaration, xml_node partcontent) {
	int count;
	auto measures = partcontent.select_nodes("./measure");
	for (int i=0; i<(int)measures.size(); i++) {
		partdata.addMeasure(measures[i].node());
		count = partdata.getMeasureCount();
		if (count > 1) {
			HumNum dur = partdata.getMeasure(count-1)->getTimeSignatureDuration();
			if (dur == 0) {
				HumNum dur = partdata.getMeasure(count-2)
						->getTimeSignatureDuration();
				if (dur > 0) {
					partdata.getMeasure(count - 1)->setTimeSignatureDuration(dur);
				}
			}
		}

	}
	return true;
}



//////////////////////////////
//
// musicxml2hum_interface::printPartInfo -- Debug information.
//

void musicxml2hum_interface::printPartInfo(vector<string>& partids,
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

bool musicxml2hum_interface::stitchParts(HumGrid& outdata,
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
// musicxml2hum_interface::cleanupMeasures --
//     Also add barlines here (keeping track of the
//     duration of each measure).
//

void musicxml2hum_interface::cleanupMeasures(HumdrumFile& outfile,
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
// musicxml2hum_interface::insertSingleMeasure --
//

void musicxml2hum_interface::insertSingleMeasure(HumdrumFile& outfile) {
	HumdrumLine* line = new HumdrumLine;
	HumdrumToken* token;
	token = new HumdrumToken("=");
	line->appendToken(token);
	line->createLineFromTokens();
	outfile.appendLine(line);
}



//////////////////////////////
//
// musicxml2hum_interface::insertAllToken --
//

void musicxml2hum_interface::insertAllToken(HumdrumFile& outfile,
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
// musicxml2hum_interface::insertMeasure --
//

bool musicxml2hum_interface::insertMeasure(HumGrid& outdata, int mnum,
		vector<MxmlPart>& partdata, vector<int> partstaves) {

	GridMeasure* gm = new GridMeasure(&outdata);
	outdata.push_back(gm);

	vector<MxmlMeasure*> measuredata;
	vector<vector<SimultaneousEvents>* > sevents;
	int i;

	for (i=0; i<(int)partdata.size(); i++) {
		measuredata.push_back(partdata[i].getMeasure(mnum));
		if (i==0) {
			gm->setDuration(partdata[i].getMeasure(mnum)->getDuration());
			gm->setTimestamp(partdata[i].getMeasure(mnum)->getTimestamp());
		}
		checkForDummyRests(partdata[i].getMeasure(mnum));
		sevents.push_back(measuredata.back()->getSortedEvents());
	}

	vector<HumNum> curtime(partdata.size());
	vector<HumNum> measuredurs(partdata.size());
	vector<int> curindex(partdata.size(), 0); // assuming data in a measure...
	HumNum nexttime = -1;

	HumNum tsdur;
	for (i=0; i<(int)curtime.size(); i++) {
		tsdur = measuredata[i]->getTimeSignatureDuration();
		if ((tsdur == 0) && (i > 0)) {
			tsdur = measuredata[i-1]->getTimeSignatureDuration();
			measuredata[i]->setTimeSignatureDuration(tsdur);
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
// musicxml2hum_interface::checkForDummyRests --
//

void musicxml2hum_interface::checkForDummyRests(MxmlMeasure* measure) {
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
// musicxml2hum_interface::convertNowEvents --
//

bool musicxml2hum_interface::convertNowEvents(GridMeasure* outdata,
		vector<SimultaneousEvents*>& nowevents, vector<int>& nowparts,
		HumNum nowtime, vector<MxmlPart>& partdata, vector<int>& partstaves) {

	if (nowevents.size() == 0) {
		// cout << "NOW EVENTS ARE EMPTY" << endl;
		return true;
	}

	if (0 && VoiceDebugQ) {
		for (int j=0; j<(int)nowevents.size(); j++) {
			vector<MxmlEvent*> nz = nowevents[j]->nonzerodur;
			for (int i=0; i<(int)nz.size(); i++) {
				cerr << "NOWEVENT NZ NAME: " << nz[i]->getElementName()
				     << "<\t" << nz[i]->getKernPitch() << endl;
			}
		}
	}

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
// musicxml2hum_interface::appendNonZeroEvents --
//

void musicxml2hum_interface::appendNonZeroEvents(GridMeasure* outdata,
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
// musicxml2hum_interface::addEvent -- Add a note or rest.
//

void musicxml2hum_interface::addEvent(GridSlice& slice,
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

	if (!event->isFloating()) {
		recip   = event->getRecip();
		pitch   = event->getKernPitch();
		prefix  = event->getPrefixNoteInfo();
		postfix = event->getPostfixNoteInfo();
		grace   = event->isGrace();

		invisible = isInvisible(event);
		if (event->isInvisible()) {
			invisible = true;
		}

		if (grace) {
			cerr << "!! NOTE IS GRACE" << endl;
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
// musicxml2hum_interface::addHarmony --
//

int musicxml2hum_interface::addHarmony(GridPart* part, MxmlEvent* event) {
	xml_node hnode = event->getHNode();
	if (!hnode) {
		return 0;
	}

	// ggg fill in X with the harmony values from the <harmony> node
	string hstring = getHarmonyString(hnode);
	HTp htok = new HumdrumToken(hstring);
	part->setHarmony(htok);

	return 1;
}



//////////////////////////////
//
// musicxml2hum_interface::getHarmonyString --
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

string musicxml2hum_interface::getHarmonyString(xml_node hnode) {
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
// musicxml2hum_interface::addLyrics --
//

int musicxml2hum_interface::addLyrics(GridStaff* staff, MxmlEvent* event) {
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
				} else if (nodeType(child, "text")) {
					text = cleanSpaces(child.child_value());
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
			}
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

string musicxml2hum_interface::cleanSpaces(const string& input) {
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
// musicxml2hum_interface::isInvisible --
//

bool musicxml2hum_interface::isInvisible(MxmlEvent* event) {
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
// musicxml2hum_interface::addSecondaryChordNotes --
//

void musicxml2hum_interface::addSecondaryChordNotes(ostream& output,
		MxmlEvent* head, const string& recip) {
	vector<MxmlEvent*> links = head->getLinkedNotes();
	MxmlEvent* note;
	string pitch;
	string prefix;
	string postfix;
	for (int i=0; i<(int)links.size(); i++) {
		note = links.at(i);
		pitch   = note->getKernPitch();
		prefix  = note->getPrefixNoteInfo();
		postfix = note->getPostfixNoteInfo();
		output << " " << prefix << recip << pitch << postfix;
	}
}



/////////////////////////////
//
// musicxml2hum_interface::appendZeroEvents --
//

void musicxml2hum_interface::appendZeroEvents(GridMeasure* outdata,
		vector<SimultaneousEvents*>& nowevents, HumNum nowtime,
		vector<MxmlPart>& partdata) {

	bool hasclef    = false;
	bool haskeysig  = false;
	bool hastimesig = false;

	vector<xml_node> clefs(partdata.size(), xml_node(NULL));
	vector<xml_node> keysigs(partdata.size(), xml_node(NULL));
	vector<xml_node> timesigs(partdata.size(), xml_node(NULL));
	int pindex = 0;
	xml_node child;

	for (int i=0; i<(int)nowevents.size(); i++) {
		for (int j=0; j<(int)nowevents[i]->zerodur.size(); j++) {
			xml_node element = nowevents[i]->zerodur[j]->getNode();
			if (nodeType(element, "attributes")) {
				child = element.first_child();
				while (child) {
					pindex = nowevents[i]->zerodur[j]->getPartIndex();

					if (nodeType(child, "clef") && !clefs[pindex]) {
						clefs[pindex] = child;
						hasclef = true;
					}

					if (nodeType(child, "key") && !keysigs[pindex]) {
						keysigs[pindex] = child;
						haskeysig = true;
					}

					if (nodeType(child, "time") && !timesigs[pindex]) {
						timesigs[pindex] = child;
						hastimesig = true;
					}

					child = child.next_sibling();
				}
			}
		}
	}

	if (hasclef) {
		addClefLine(outdata, clefs, partdata, nowtime);
	}

	if (haskeysig) {
		addKeySigLine(outdata, keysigs, partdata, nowtime);
	}

	if (hastimesig) {
		addTimeSigLine(outdata, timesigs, partdata, nowtime);
	}

}



//////////////////////////////
//
// musicxml2hum_interface::addClefLine --
//

void musicxml2hum_interface::addClefLine(GridMeasure* outdata,
		vector<xml_node>& clefs, vector<MxmlPart>& partdata, HumNum nowtime) {

	GridSlice* slice = new GridSlice(outdata, nowtime,
		SliceType::Clefs);
	outdata->push_back(slice);
	slice->initializePartStaves(partdata);

	for (int i=0; i<(int)partdata.size(); i++) {
		if (clefs[i]) {
			insertPartClefs(clefs[i], *slice->at(i));
		}
	}
}



//////////////////////////////
//
// musicxml2hum_interface::addTimeSigLine --
//

void musicxml2hum_interface::addTimeSigLine(GridMeasure* outdata,
		vector<xml_node>& timesigs, vector<MxmlPart>& partdata, HumNum nowtime) {

	GridSlice* slice = new GridSlice(outdata, nowtime,
		SliceType::TimeSigs);
	outdata->push_back(slice);
	slice->initializePartStaves(partdata);

	for (int i=0; i<(int)partdata.size(); i++) {
		if (timesigs[i]) {
			insertPartTimeSigs(timesigs[i], *slice->at(i));
		}
	}
}



//////////////////////////////
//
// musicxml2hum_interface::addKeySigLine --
//

void musicxml2hum_interface::addKeySigLine(GridMeasure* outdata,
		vector<xml_node>& keysigs, vector<MxmlPart>& partdata, HumNum nowtime) {

	GridSlice* slice = new GridSlice(outdata, nowtime,
		SliceType::KeySigs);
	outdata->push_back(slice);
	slice->initializePartStaves(partdata);

	for (int i=0; i<(int)partdata.size(); i++) {
		if (keysigs[i]) {
			insertPartKeySigs(keysigs[i], *slice->at(i));
		}
	}
}



//////////////////////////////
//
// musicxml2hum_interface::insertPartClefs --
//

void musicxml2hum_interface::insertPartClefs(xml_node clef, GridPart& part) {
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
// musicxml2hum_interface::fillEmpties --
//

void musicxml2hum_interface::fillEmpties(GridPart* part, const char* string) {
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
// musicxml2hum_interface::insertPartKeySigs --
//

void musicxml2hum_interface::insertPartKeySigs(xml_node keysig, GridPart& part) {
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
// musicxml2hum_interface::insertPartTimeSigs --
//

void musicxml2hum_interface::insertPartTimeSigs(xml_node timesig, GridPart& part) {
	if (!timesig) {
		// no timesig
		return;
	}

	HTp token;
	int staffnum = 0;
	while (timesig) {
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
}



//////////////////////////////
//
//	musicxml2hum_interface::convertKeySigToHumdrum --
//
//  <key>
//     <fifths>4</fifths>
//

xml_node musicxml2hum_interface::convertKeySigToHumdrum(xml_node keysig,
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
		switch (fifths) {
			case 7: ss << "b#";
			case 6: ss << "e#";
			case 5: ss << "a#";
			case 4: ss << "d#";
			case 3: ss << "g#";
			case 2: ss << "c#";
			case 1: ss << "f#";
		}
	} else if (fifths < 0) {
		switch (fifths) {
			case 7: ss << "f-";
			case 6: ss << "g-";
			case 5: ss << "c-";
			case 4: ss << "d-";
			case 3: ss << "a-";
			case 2: ss << "e-";
			case 1: ss << "b-";
		}
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
//	musicxml2hum_interface::convertTimeSigToHumdrum --
//
//  <time symbol="common">
//     <beats>4</beats>
//     <beat-type>4</beat-type>
//

xml_node musicxml2hum_interface::convertTimeSigToHumdrum(xml_node timesig,
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
//	musicxml2hum_interface::convertClefToHumdrum --
//

xml_node musicxml2hum_interface::convertClefToHumdrum(xml_node clef,
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
// musicxml2hum_interface::nodeType -- return true if node type matches
//     string.
//

bool musicxml2hum_interface::nodeType(xml_node node, const char* testname) {
	if (strcmp(node.name(), testname) == 0) {
		return true;
	} else {
		return false;
	}
}



//////////////////////////////
//
// musicxml2hum_interface::appendNullTokens --
//

void musicxml2hum_interface::appendNullTokens(HumdrumLine* line,
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
// musicxml2hum_interface::getPartContent -- Extract the part elements in
//     the file indexed by part ID.
//

bool musicxml2hum_interface::getPartContent(
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
// musicxml2hum_interface::getPartInfo -- Extract a list of the part ids,
//    and a reverse mapping to the <score-part> element to which is refers.
//
//	   part-list structure:
//        <part-list>
//          <score-part id="P1"/>
//          <score-part id="P2"/>
//          etc.
//        </part-list>
//

bool musicxml2hum_interface::getPartInfo(map<string, xml_node>& partinfo,
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
// musicxml2hum_interface::getChildElementText -- Return the (first)
//    matching child element's text content.
//

string musicxml2hum_interface::getChildElementText(xml_node root,
		const char* xpath) {
	return root.select_single_node(xpath).node().child_value();
}

string musicxml2hum_interface::getChildElementText(xpath_node root,
		const char* xpath) {
	return root.node().select_single_node(xpath).node().child_value();
}



//////////////////////////////
//
// musicxml2hum_interface::getAttributeValue -- For an xml_node, return
//     the value for the given attribute name.
//

string musicxml2hum_interface::getAttributeValue(xml_node xnode,
		const string& target) {
	for (auto at = xnode.first_attribute(); at; at = at.next_attribute()) {
		if (target == at.name()) {
			return at.value();
		}
	}
	return "";
}


string musicxml2hum_interface::getAttributeValue(xpath_node xnode,
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
// musicxml2hum_interface::printAttributes -- Print list of all attributes
//     for an xml_node.
//

void musicxml2hum_interface::printAttributes(xml_node node) {
	int counter = 1;
	for (auto at = node.first_attribute(); at; at = at.next_attribute()) {
		cout << "\tattribute " << counter++
		     << "\tname  = " << at.name()
		     << "\tvalue = " << at.value()
		     << endl;
	}
}

} // end namespace hum


