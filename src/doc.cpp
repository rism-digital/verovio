/////////////////////////////////////////////////////////////////////////////
// Name:        doc.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "doc.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "attcomparison.h"
#include "barline.h"
#include "chord.h"
#include "functorparams.h"
#include "glyph.h"
#include "keysig.h"
#include "label.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "page.h"
#include "rpt.h"
#include "score.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "syl.h"
#include "system.h"
#include "verse.h"
#include "vrv.h"

//----------------------------------------------------------------------------

#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Doc
//----------------------------------------------------------------------------

Doc::Doc() : Object("doc-")
{
    m_style = new Style();

    // owned pointers need to be set to NULL;
    m_scoreBuffer = NULL;
    Reset();
}

Doc::~Doc()
{
    delete m_style;
    if (m_scoreBuffer) {
        delete m_scoreBuffer;
    }
}

void Doc::Reset()
{
    Object::Reset();

    m_type = Raw;
    m_pageWidth = -1;
    m_pageHeight = -1;
    m_pageRightMar = 0;
    m_pageLeftMar = 0;
    m_pageTopMar = 0;

    m_drawingSpacingLinear = DEFAULT_SPACING_LINEAR;
    m_drawingSpacingNonLinear = DEFAULT_SPACING_NON_LINEAR;

    m_spacingStaff = m_style->m_spacingStaff;
    m_spacingSystem = m_style->m_spacingSystem;

    m_drawingPage = NULL;
    m_drawingJustifyX = true;
    m_drawingEvenSpacing = false;
    m_currentScoreDefDone = false;
    m_drawingPreparationDone = false;
    m_hasMidiTimemap = false;

    m_scoreDef.Reset();
    if (m_scoreBuffer) {
        delete m_scoreBuffer;
        m_scoreBuffer = NULL;
    }

    m_drawingSmuflFontSize = 0;
    m_drawingLyricFontSize = 0;
}

void Doc::SetType(DocType type)
{
    Reset();
    m_type = type;
}

void Doc::AddChild(Object *child)
{
    assert(!m_scoreBuffer); // Children cannot be added if a score buffer was created;

    if (child->Is(PAGE)) {
        assert(dynamic_cast<Page *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

Score *Doc::CreateScoreBuffer()
{
    assert(!m_scoreBuffer); // Should not be called twice - Call Doc::Reset() to Reset it if necessary

    ClearChildren();
    m_scoreDef.Reset();

    m_scoreBuffer = new Score();
    return m_scoreBuffer;
}

void Doc::Refresh()
{
    RefreshViews();
}

bool Doc::GenerateDocumentScoreDef()
{
    Measure *measure = dynamic_cast<Measure *>(this->FindChildByType(MEASURE));
    if (!measure) {
        LogError("No measure found for generating a scoreDef");
        return false;
    }

    ArrayOfObjects staves;
    AttComparison matchType(STAFF);
    measure->FindAllChildByAttComparison(&staves, &matchType);

    if (staves.empty()) {
        LogError("No staff found for generating a scoreDef");
        return false;
    }

    m_scoreDef.Reset();
    StaffGrp *staffGrp = new StaffGrp();
    ArrayOfObjects::iterator iter;
    for (iter = staves.begin(); iter != staves.end(); iter++) {
        Staff *staff = dynamic_cast<Staff *>(*iter);
        assert(staff);
        StaffDef *staffDef = new StaffDef();
        staffDef->SetN(staff->GetN());
        staffDef->SetLines(5);
        if (!measure->IsMeasuredMusic()) staffDef->SetNotationtype(NOTATIONTYPE_mensural);
        staffGrp->AddChild(staffDef);
    }
    m_scoreDef.AddChild(staffGrp);

    LogMessage("ScoreDef generated");

    return true;
}

bool Doc::HasMidiTimemap()
{
    return m_hasMidiTimemap;
}

void Doc::CalculateMidiTimemap()
{
    m_hasMidiTimemap = false;

    // This happens if the document was never cast off (no-layout option in the toolkit)
    if (!m_drawingPage && GetChildCount() == 1) {
        Page *page = this->SetDrawingPage(0);
        if (!page) {
            return;
        }
        this->CollectScoreDefs();
        page->LayOutHorizontally();
    }

    int tempo = 120;

    // Set tempo
    if (m_scoreDef.HasMidiBpm()) {
        tempo = m_scoreDef.GetMidiBpm();
    }

    // We first calculate the maximum duration of each measure
    CalcMaxMeasureDurationParams calcMaxMeasureDurationParams;
    calcMaxMeasureDurationParams.m_currentTempo = tempo;
    Functor calcMaxMeasureDuration(&Object::CalcMaxMeasureDuration);
    this->Process(&calcMaxMeasureDuration, &calcMaxMeasureDurationParams);

    // Then calculate the onset and offset times (w.r.t. the measure) for every note
    CalcOnsetOffsetParams calcOnsetOffsetParams;
    Functor calcOnsetOffset(&Object::CalcOnsetOffset);
    Functor calcOnsetOffsetEnd(&Object::CalcOnsetOffsetEnd);
    this->Process(&calcOnsetOffset, &calcOnsetOffsetParams, &calcOnsetOffsetEnd);

    // Adjust the duration of tied notes
    Functor resolveMIDITies(&Object::ResolveMIDITies);
    this->Process(&resolveMIDITies, NULL, NULL, NULL, UNLIMITED_DEPTH, BACKWARD);

    m_hasMidiTimemap = true;
}

void Doc::ExportMIDI(MidiFile *midiFile)
{

    if (!Doc::HasMidiTimemap()) {
        // generate MIDI timemap before progressing
        CalculateMidiTimemap();
    }
    if (!Doc::HasMidiTimemap()) {
        LogWarning("Calculation of MIDI timemap failed, not exporting MidiFile.");
    }

    int tempo = 120;

    // Set tempo
    if (m_scoreDef.HasMidiBpm()) {
        tempo = m_scoreDef.GetMidiBpm();
    }
    midiFile->addTempo(0, 0, tempo);

    // We need to populate processing lists for processing the document by Layer (by Verse will not be used)
    PrepareProcessingListsParams prepareProcessingListsParams;
    // Alternate solution with StaffN_LayerN_VerseN_t (see also Verse::PrepareDrawing)
    // StaffN_LayerN_VerseN_t staffLayerVerseTree;
    // params.push_back(&staffLayerVerseTree);

    // We first fill a tree of int with [staff/layer] and [staff/layer/verse] numbers (@n) to be process
    Functor prepareProcessingLists(&Object::PrepareProcessingLists);
    this->Process(&prepareProcessingLists, &prepareProcessingListsParams);

    // The tree is used to process each staff/layer/verse separatly
    // For this, we use a array of AttNIntegerComparison that looks for each object if it is of the type
    // and with @n specified

    IntTree_t::iterator staves;
    IntTree_t::iterator layers;

    // Process notes and chords, rests, spaces layer by layer
    // track 0 (included by default) is reserved for meta messages common to all tracks
    int midiTrack = 1;
    std::vector<AttComparison *> filters;
    for (staves = prepareProcessingListsParams.m_layerTree.child.begin();
         staves != prepareProcessingListsParams.m_layerTree.child.end(); ++staves) {

        int transSemi = 0;
        // Get the transposition (semi-tone) value for the staff
        if (StaffDef *staffDef = this->m_scoreDef.GetStaffDef(staves->first)) {
            if (staffDef->HasTransSemi()) transSemi = staffDef->GetTransSemi();
            midiTrack = staffDef->GetN();
            midiFile->addTrack();
            Label *label = dynamic_cast<Label *>(staffDef->FindChildByType(LABEL, 1));
            if (!label) {
                StaffGrp *staffGrp = dynamic_cast<StaffGrp *>(staffDef->GetFirstParent(STAFFGRP));
                assert(staffGrp);
                label = dynamic_cast<Label *>(staffGrp->FindChildByType(LABEL, 1));
            }
            if (label) {
                std::string trackName = UTF16to8(label->GetText(label)).c_str();
                if (!trackName.empty()) midiFile->addTrackName(midiTrack, 0, trackName);
            }
        }

        for (layers = staves->second.child.begin(); layers != staves->second.child.end(); ++layers) {
            filters.clear();
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves->first);
            AttNIntegerComparison matchLayer(LAYER, layers->first);
            filters.push_back(&matchStaff);
            filters.push_back(&matchLayer);

            GenerateMIDIParams generateMIDIParams(midiFile);
            generateMIDIParams.m_midiTrack = midiTrack;
            generateMIDIParams.m_transSemi = transSemi;
            generateMIDIParams.m_currentTempo = tempo;
            Functor generateMIDI(&Object::GenerateMIDI);

            // LogDebug("Exporting track %d ----------------", midiTrack);
            this->Process(&generateMIDI, &generateMIDIParams, NULL, &filters);
        }
    }
}

bool Doc::ExportTimemap(string &output)
{
    if (!Doc::HasMidiTimemap()) {
        // generate MIDI timemap before progressing
        CalculateMidiTimemap();
    }
    if (!Doc::HasMidiTimemap()) {
        LogWarning("Calculation of MIDI timemap failed, not exporting MidiFile.");
        output = "";
        return false;
    }
    GenerateTimemapParams generateTimemapParams;
    Functor generateTimemap(&Object::GenerateTimemap);
    this->Process(&generateTimemap, &generateTimemapParams);

    PrepareJsonTimemap(output, generateTimemapParams.realTimeToScoreTime, generateTimemapParams.realTimeToOnElements,
        generateTimemapParams.realTimeToOffElements, generateTimemapParams.realTimeToTempo);

    return true;
}

void Doc::PrepareJsonTimemap(std::string &output, std::map<int, double> &realTimeToScoreTime,
    std::map<int, vector<string> > &realTimeToOnElements, std::map<int, vector<string> > &realTimeToOffElements,
    std::map<int, int> &realTimeToTempo)
{

    int currentTempo = -1000;
    int newTempo;
    int mapsize = (int)realTimeToScoreTime.size();
    output = "";
    output.reserve(mapsize * 100); // Estimate 100 characters for each entry.
    output += "[\n";
    auto lastit = realTimeToScoreTime.end();
    lastit--;
    for (auto it = realTimeToScoreTime.begin(); it != realTimeToScoreTime.end(); it++) {
        output += "\t{\n";
        output += "\t\t\"tstamp\":\t";
        output += to_string(it->first);
        output += ",\n";
        output += "\t\t\"qstamp\":\t";
        output += to_string(it->second);

        auto ittempo = realTimeToTempo.find(it->first);
        if (ittempo != realTimeToTempo.end()) {
            newTempo = ittempo->second;
            if (newTempo != currentTempo) {
                currentTempo = newTempo;
                output += ",\n\t\t\"tempo\":\t";
                output += to_string(currentTempo);
            }
        }

        auto iton = realTimeToOnElements.find(it->first);
        if (iton != realTimeToOnElements.end()) {
            output += ",\n\t\t\"on\":\t[";
            for (int ion = 0; ion < (int)iton->second.size(); ion++) {
                output += "\"";
                output += iton->second[ion];
                output += "\"";
                if (ion < (int)iton->second.size() - 1) {
                    output += ", ";
                }
            }
            output += "]";
        }

        auto itoff = realTimeToOffElements.find(it->first);
        if (itoff != realTimeToOffElements.end()) {
            output += ",\n\t\t\"off\":\t[";
            for (int ioff = 0; ioff < (int)itoff->second.size(); ioff++) {
                output += "\"";
                output += itoff->second[ioff];
                output += "\"";
                if (ioff < (int)itoff->second.size() - 1) {
                    output += ", ";
                }
            }
            output += "]";
        }

        output += "\n\t}";
        if (it == lastit) {
            output += "\n";
        }
        else {
            output += ",\n";
        }
    }
    output += "]\n";
}

void Doc::PrepareDrawing()
{
    if (m_drawingPreparationDone) {
        Functor resetDrawing(&Object::ResetDrawing);
        this->Process(&resetDrawing, NULL);
    }

    // Try to match all spanning elements (slur, tie, etc) by processing backwards
    PrepareTimeSpanningParams prepareTimeSpanningParams;
    Functor prepareTimeSpanning(&Object::PrepareTimeSpanning);
    Functor prepareTimeSpanningEnd(&Object::PrepareTimeSpanningEnd);
    this->Process(
        &prepareTimeSpanning, &prepareTimeSpanningParams, &prepareTimeSpanningEnd, NULL, UNLIMITED_DEPTH, BACKWARD);

    // First we try backwards because normally the spanning elements are at the end of
    // the measure. However, in some case, one (or both) end points will appear afterwards
    // in the encoding. For these, the previous iteration will not have resolved the link and
    // the spanning elements will remain in the timeSpanningElements array. We try again forwards
    // but this time without filling the list (that is only will the remaining elements)
    if (!prepareTimeSpanningParams.m_timeSpanningInterfaces.empty()) {
        prepareTimeSpanningParams.m_fillList = false;
        this->Process(&prepareTimeSpanning, &prepareTimeSpanningParams);
    }

    // Try to match all time pointing elements (tempo, fermata, etc) by processing backwards
    PrepareTimePointingParams prepareTimePointingParams;
    Functor prepareTimePointing(&Object::PrepareTimePointing);
    Functor prepareTimePointingEnd(&Object::PrepareTimePointingEnd);
    this->Process(
        &prepareTimePointing, &prepareTimePointingParams, &prepareTimePointingEnd, NULL, UNLIMITED_DEPTH, BACKWARD);

    // Now try to match the @tstamp and @tstamp2 attributes.
    PrepareTimestampsParams prepareTimestampsParams;
    prepareTimestampsParams.m_timeSpanningInterfaces = prepareTimeSpanningParams.m_timeSpanningInterfaces;
    Functor prepareTimestamps(&Object::PrepareTimestamps);
    Functor prepareTimestampsEnd(&Object::PrepareTimestampsEnd);
    this->Process(&prepareTimestamps, &prepareTimestampsParams, &prepareTimestampsEnd);

    // If some are still there, then it is probably an issue in the encoding
    if (!prepareTimestampsParams.m_timeSpanningInterfaces.empty()) {
        LogWarning("%d time spanning element(s) could not be matched",
            prepareTimestampsParams.m_timeSpanningInterfaces.size());
    }

    // Prepare the cross-staff pointers
    PrepareCrossStaffParams prepareCrossStaffParams;
    Functor prepareCrossStaff(&Object::PrepareCrossStaff);
    Functor prepareCrossStaffEnd(&Object::PrepareCrossStaffEnd);
    this->Process(&prepareCrossStaff, &prepareCrossStaffParams, &prepareCrossStaffEnd);

    // We need to populate processing lists for processing the document by Layer (for matching @tie) and
    // by Verse (for matching syllable connectors)
    PrepareProcessingListsParams prepareProcessingListsParams;
    // Alternate solution with StaffN_LayerN_VerseN_t (see also Verse::PrepareDrawing)
    // StaffN_LayerN_VerseN_t staffLayerVerseTree;
    // params.push_back(&staffLayerVerseTree);

    // We first fill a tree of ints with [staff/layer] and [staff/layer/verse] numbers (@n) to be processed
    // LogElapsedTimeStart();
    Functor prepareProcessingLists(&Object::PrepareProcessingLists);
    this->Process(&prepareProcessingLists, &prepareProcessingListsParams);

    // The tree is used to process each staff/layer/verse separately
    // For this, we use an array of AttNIntegerComparison that looks for each object if it is of the type
    // and with @n specified

    IntTree_t::iterator staves;
    IntTree_t::iterator layers;
    IntTree_t::iterator verses;

    // Process by layer for matching @tie attribute - we process notes and chords, looking at
    // GetTie values and pitch and oct for matching notes
    std::vector<AttComparison *> filters;
    for (staves = prepareProcessingListsParams.m_layerTree.child.begin();
         staves != prepareProcessingListsParams.m_layerTree.child.end(); ++staves) {
        for (layers = staves->second.child.begin(); layers != staves->second.child.end(); ++layers) {
            filters.clear();
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves->first);
            AttNIntegerComparison matchLayer(LAYER, layers->first);
            filters.push_back(&matchStaff);
            filters.push_back(&matchLayer);

            PrepareTieAttrParams prepareTieAttrParams;
            Functor prepareTieAttr(&Object::PrepareTieAttr);
            Functor prepareTieAttrEnd(&Object::PrepareTieAttrEnd);
            this->Process(&prepareTieAttr, &prepareTieAttrParams, &prepareTieAttrEnd, &filters);

            // After having processed one layer, we check if we have open ties - if yes, we
            // must reset them and they will be ignored.
            if (!prepareTieAttrParams.m_currentNotes.empty()) {
                std::vector<Note *>::iterator iter;
                for (iter = prepareTieAttrParams.m_currentNotes.begin();
                     iter != prepareTieAttrParams.m_currentNotes.end(); iter++) {
                    LogWarning("Unable to match @tie of note '%s', skipping it", (*iter)->GetUuid().c_str());
                    (*iter)->ResetDrawingTieAttr();
                }
            }
        }
    }

    for (staves = prepareProcessingListsParams.m_layerTree.child.begin();
         staves != prepareProcessingListsParams.m_layerTree.child.end(); ++staves) {
        for (layers = staves->second.child.begin(); layers != staves->second.child.end(); ++layers) {
            filters.clear();
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves->first);
            AttNIntegerComparison matchLayer(LAYER, layers->first);
            filters.push_back(&matchStaff);
            filters.push_back(&matchLayer);

            PreparePointersByLayerParams preparePointersByLayerParams;
            Functor preparePointersByLayer(&Object::PreparePointersByLayer);
            this->Process(&preparePointersByLayer, &preparePointersByLayerParams, NULL, &filters);
        }
    }

    // Same for the lyrics, but Verse by Verse since Syl are TimeSpanningInterface elements for handling connectors
    for (staves = prepareProcessingListsParams.m_verseTree.child.begin();
         staves != prepareProcessingListsParams.m_verseTree.child.end(); ++staves) {
        for (layers = staves->second.child.begin(); layers != staves->second.child.end(); ++layers) {
            for (verses = layers->second.child.begin(); verses != layers->second.child.end(); ++verses) {
                // std::cout << staves->first << " => " << layers->first << " => " << verses->first << '\n';
                filters.clear();
                // Create ad comparison object for each type / @n
                AttNIntegerComparison matchStaff(STAFF, staves->first);
                AttNIntegerComparison matchLayer(LAYER, layers->first);
                AttNIntegerComparison matchVerse(VERSE, verses->first);
                filters.push_back(&matchStaff);
                filters.push_back(&matchLayer);
                filters.push_back(&matchVerse);

                // The first pass sets m_drawingFirstNote and m_drawingLastNote for each syl
                // m_drawingLastNote is set only if the syl has a forward connector
                PrepareLyricsParams prepareLyricsParams;
                Functor prepareLyrics(&Object::PrepareLyrics);
                Functor prepareLyricsEnd(&Object::PrepareLyricsEnd);
                this->Process(&prepareLyrics, &prepareLyricsParams, &prepareLyricsEnd, &filters);
            }
        }
    }

    // Once <slur>, <ties> and @ties are matched but also syl connectors, we need to set them as running
    // TimeSpanningInterface to each staff they are extended. This does not need to be done staff by staff because we
    // can just check the staff->GetN to see where we are (see Staff::FillStaffCurrentTimeSpanning)
    FillStaffCurrentTimeSpanningParams fillStaffCurrentTimeSpanningParams;
    Functor fillStaffCurrentTimeSpanning(&Object::FillStaffCurrentTimeSpanning);
    Functor fillStaffCurrentTimeSpanningEnd(&Object::FillStaffCurrentTimeSpanningEnd);
    this->Process(&fillStaffCurrentTimeSpanning, &fillStaffCurrentTimeSpanningParams, &fillStaffCurrentTimeSpanningEnd);

    // Something must be wrong in the encoding because a TimeSpanningInterface was left open
    if (!fillStaffCurrentTimeSpanningParams.m_timeSpanningElements.empty()) {
        LogDebug("%d time spanning elements could not be set as running",
            fillStaffCurrentTimeSpanningParams.m_timeSpanningElements.size());
    }

    // Process by staff for matching mRpt elements and setting the drawing number
    for (staves = prepareProcessingListsParams.m_layerTree.child.begin();
         staves != prepareProcessingListsParams.m_layerTree.child.end(); ++staves) {
        for (layers = staves->second.child.begin(); layers != staves->second.child.end(); ++layers) {
            filters.clear();
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves->first);
            AttNIntegerComparison matchLayer(LAYER, layers->first);
            filters.push_back(&matchStaff);
            filters.push_back(&matchLayer);

            // We set multiNumber to NONE for indicated we need to look at the staffDef when reaching the first staff
            PrepareRptParams prepareRptParams(&m_scoreDef);
            Functor prepareRpt(&Object::PrepareRpt);
            this->Process(&prepareRpt, &prepareRptParams, NULL, &filters);
        }
    }

    // Prepare the endings (pointers to the measure after and before the boundaries
    PrepareBoundariesParams prepareEndingsParams;
    Functor prepareEndings(&Object::PrepareBoundaries);
    this->Process(&prepareEndings, &prepareEndingsParams);

    // Prepare the floating drawing groups
    PrepareFloatingGrpsParams prepareFloatingGrpsParams;
    Functor prepareFloatingGrps(&Object::PrepareFloatingGrps);
    this->Process(&prepareFloatingGrps, &prepareFloatingGrpsParams);

    Functor prepareLayerElementParts(&Object::PrepareLayerElementParts);
    this->Process(&prepareLayerElementParts, NULL);

    // Prepare the drawing cue size
    Functor prepareDrawingCueSize(&Object::PrepareDrawingCueSize);
    this->Process(&prepareDrawingCueSize, NULL);

    /*
    // Alternate solution with StaffN_LayerN_VerseN_t
    StaffN_LayerN_VerseN_t::iterator staves;
    LayerN_VerserN_t::iterator layers;
    VerseN_t::iterator verses;
    std::vector<AttComparison*> filters;
    for (staves = staffLayerVerseTree.begin(); staves != staffLayerVerseTree.end(); ++staves) {
        for (layers = staves->second.begin(); layers != staves->second.end(); ++layers) {
            for (verses= layers->second.begin(); verses != layers->second.end(); ++verses) {
                std::cout << staves->first << " => " << layers->first << " => " << verses->first << '\n';
                filters.clear();
                AttNIntegerComparison matchStaff(&typeid(Staff), staves->first);
                AttNIntegerComparison matchLayer(&typeid(Layer), layers->first);
                AttNIntegerComparison matchVerse(&typeid(Verse), verses->first);
                filters.push_back(&matchStaff);
                filters.push_back(&matchLayer);
                filters.push_back(&matchVerse);

                FunctorParams paramsLyrics;
                Functor prepareLyrics(&Object::PrepareLyrics);
                this->Process(&prepareLyrics, paramsLyrics, NULL, &filters);
            }
        }
    }
    */

    // LogElapsedTimeEnd ("Preparing drawing");

    m_drawingPreparationDone = true;
}

void Doc::CollectScoreDefs(bool force)
{
    if (m_currentScoreDefDone && !force) {
        return;
    }

    if (m_currentScoreDefDone) {
        Functor unsetCurrentScoreDef(&Object::UnsetCurrentScoreDef);
        this->Process(&unsetCurrentScoreDef, NULL);
    }

    ScoreDef upcomingScoreDef = m_scoreDef;
    SetCurrentScoreDefParams setCurrentScoreDefParams(this, &upcomingScoreDef);
    Functor setCurrentScoreDef(&Object::SetCurrentScoreDef);

    // First process the current scoreDef in order to fill the staffDef with
    // the appropriate drawing values
    upcomingScoreDef.Process(&setCurrentScoreDef, &setCurrentScoreDefParams);

    // LogElapsedTimeStart();
    this->Process(&setCurrentScoreDef, &setCurrentScoreDefParams);
    // LogElapsedTimeEnd ("Setting scoreDefs");

    m_currentScoreDefDone = true;
}

void Doc::CastOffDoc()
{
    this->CollectScoreDefs();

    Page *contentPage = this->SetDrawingPage(0);
    assert(contentPage);
    contentPage->LayOutHorizontally();

    System *contentSystem = dynamic_cast<System *>(contentPage->DetachChild(0));
    assert(contentSystem);

    System *currentSystem = new System();
    contentPage->AddChild(currentSystem);
    CastOffSystemsParams castOffSystemsParams(contentSystem, contentPage, currentSystem);
    castOffSystemsParams.m_systemWidth = this->m_drawingPageWidth - this->m_drawingPageLeftMar
        - this->m_drawingPageRightMar - currentSystem->m_systemLeftMar - currentSystem->m_systemRightMar;
    castOffSystemsParams.m_shift = -contentSystem->GetDrawingLabelsWidth();
    castOffSystemsParams.m_currentScoreDefWidth
        = contentPage->m_drawingScoreDef.GetDrawingWidth() + contentSystem->GetDrawingAbbrLabelsWidth();

    Functor castOffSystems(&Object::CastOffSystems);
    Functor castOffSystemsEnd(&Object::CastOffSystemsEnd);
    contentSystem->Process(&castOffSystems, &castOffSystemsParams, &castOffSystemsEnd);
    delete contentSystem;

    // Reset the scoreDef at the beginning of each system
    this->CollectScoreDefs(true);

    // Here we redo the alignment because of the new scoreDefs
    // We can actually optimise this and have a custom version that does not redo all the calculation
    // contentPage->LayOutHorizontally();

    contentPage->LayOutVertically();

    // Detach the contentPage
    this->DetachChild(0);
    assert(contentPage && !contentPage->GetParent());

    Page *currentPage = new Page();
    this->AddChild(currentPage);
    CastOffPagesParams castOffPagesParams(contentPage, this, currentPage);
    castOffPagesParams.m_pageHeight
        = this->m_drawingPageHeight - this->m_drawingPageTopMar; // obviously we need a bottom margin
    Functor castOffPages(&Object::CastOffPages);
    contentPage->Process(&castOffPages, &castOffPagesParams);
    delete contentPage;

    // LogDebug("Layout: %d pages", this->GetChildCount());

    // We need to reset the drawing page to NULL
    // because idx will still be 0 but contentPage is dead!
    this->ResetDrawingPage();
    this->CollectScoreDefs(true);
}

void Doc::UnCastOffDoc()
{
    Page *contentPage = new Page();
    System *contentSystem = new System();
    contentPage->AddChild(contentSystem);

    UnCastOffParams unCastOffParams(contentSystem);

    Functor unCastOff(&Object::UnCastOff);
    this->Process(&unCastOff, &unCastOffParams);

    this->ClearChildren();

    this->AddChild(contentPage);

    // LogDebug("ContinousLayout: %d pages", this->GetChildCount());

    // We need to reset the drawing page to NULL
    // because idx will still be 0 but contentPage is dead!
    this->ResetDrawingPage();
    this->CollectScoreDefs(true);
}

void Doc::CastOffEncodingDoc()
{
    this->CollectScoreDefs();

    Page *contentPage = this->SetDrawingPage(0);
    assert(contentPage);

    System *contentSystem = dynamic_cast<System *>(contentPage->FindChildByType(SYSTEM));
    assert(contentSystem);

    // Detach the contentPage
    this->DetachChild(0);
    assert(contentPage && !contentPage->GetParent());

    Page *page = new Page();
    this->AddChild(page);
    System *system = new System();
    page->AddChild(system);

    CastOffEncodingParams castOffEncodingParams(this, page, system, contentSystem);

    Functor castOffEncoding(&Object::CastOffEncoding);
    contentSystem->Process(&castOffEncoding, &castOffEncodingParams);
    delete contentPage;

    // We need to reset the drawing page to NULL
    // because idx will still be 0 but contentPage is dead!
    this->ResetDrawingPage();
    this->CollectScoreDefs(true);
}

void Doc::ConvertToPageBasedDoc()
{
    assert(m_scoreBuffer); // Doc::CreateScoreBuffer needs to be called first;

    Page *page = new Page();
    System *system = new System();
    page->AddChild(system);

    ConvertToPageBasedParams convertToPageBasedParams(system);
    Functor convertToPageBased(&Object::ConvertToPageBased);
    Functor convertToPageBasedEnd(&Object::ConvertToPageBasedEnd);
    m_scoreBuffer->Process(&convertToPageBased, &convertToPageBasedParams, &convertToPageBasedEnd);

    m_scoreBuffer->ClearRelinquishedChildren();
    assert(m_scoreBuffer->GetChildCount() == 0);

    delete m_scoreBuffer;
    m_scoreBuffer = NULL;

    this->AddChild(page);

    this->ResetDrawingPage();
}

bool Doc::HasPage(int pageIdx) const
{
    return ((pageIdx >= 0) && (pageIdx < GetChildCount()));
}

int Doc::GetPageCount() const
{
    return GetChildCount();
}

int Doc::GetGlyphHeight(wchar_t code, int staffSize, bool graceSize) const
{
    int x, y, w, h;
    Glyph *glyph = Resources::GetGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    h = h * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    if (graceSize) h = h * this->m_style->m_graceNum / this->m_style->m_graceDen;
    h = h * staffSize / 100;
    return h;
}

int Doc::GetGlyphWidth(wchar_t code, int staffSize, bool graceSize) const
{
    int x, y, w, h;
    Glyph *glyph = Resources::GetGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    w = w * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    if (graceSize) w = w * this->m_style->m_graceNum / this->m_style->m_graceDen;
    w = w * staffSize / 100;
    return w;
}

Point Doc::ConvertFontPoint(const Glyph *glyph, const Point &fontPoint, int staffSize, bool graceSize) const
{
    assert(glyph);

    Point point;
    point.x = fontPoint.x * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    point.y = fontPoint.y * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    if (graceSize) {
        point.x = point.x * this->m_style->m_graceNum / this->m_style->m_graceDen;
        point.y = point.y * this->m_style->m_graceNum / this->m_style->m_graceDen;
    }
    if (staffSize != 100) {
        point.x = point.x * staffSize / 100;
        point.y = point.y * staffSize / 100;
    }
    return point;
}

int Doc::GetGlyphDescender(wchar_t code, int staffSize, bool graceSize) const
{
    int x, y, w, h;
    Glyph *glyph = Resources::GetGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    y = y * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    if (graceSize) y = y * this->m_style->m_graceNum / this->m_style->m_graceDen;
    y = y * staffSize / 100;
    return y;
}

int Doc::GetTextGlyphHeight(wchar_t code, FontInfo *font, bool graceSize) const
{
    assert(font);

    int x, y, w, h;
    Glyph *glyph = Resources::GetTextGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    h = h * font->GetPointSize() / glyph->GetUnitsPerEm();
    if (graceSize) h = h * this->m_style->m_graceNum / this->m_style->m_graceDen;
    return h;
}

int Doc::GetTextGlyphWidth(wchar_t code, FontInfo *font, bool graceSize) const
{
    assert(font);

    int x, y, w, h;
    Glyph *glyph = Resources::GetTextGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    w = w * font->GetPointSize() / glyph->GetUnitsPerEm();
    if (graceSize) w = w * this->m_style->m_graceNum / this->m_style->m_graceDen;
    return w;
}

int Doc::GetTextGlyphDescender(wchar_t code, FontInfo *font, bool graceSize) const
{
    assert(font);

    int x, y, w, h;
    Glyph *glyph = Resources::GetTextGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    y = y * font->GetPointSize() / glyph->GetUnitsPerEm();
    if (graceSize) y = y * this->m_style->m_graceNum / this->m_style->m_graceDen;
    return y;
}

int Doc::GetDrawingUnit(int staffSize) const
{
    return m_drawingUnit * staffSize / 100;
}

int Doc::GetDrawingDoubleUnit(int staffSize) const
{
    return m_drawingDoubleUnit * staffSize / 100;
}

int Doc::GetDrawingStaffSize(int staffSize) const
{
    return m_drawingStaffSize * staffSize / 100;
}

int Doc::GetDrawingOctaveSize(int staffSize) const
{
    return m_drawingOctaveSize * staffSize / 100;
}

int Doc::GetDrawingBrevisWidth(int staffSize) const
{
    return m_drawingBrevisWidth * staffSize / 100;
}

int Doc::GetDrawingBarLineWidth(int staffSize) const
{
    return m_style->m_barLineWidth * staffSize / 100;
}

int Doc::GetDrawingStaffLineWidth(int staffSize) const
{
    return m_style->m_staffLineWidth * staffSize / 100;
}

int Doc::GetDrawingStemWidth(int staffSize) const
{
    return m_style->m_stemWidth * staffSize / 100;
}

int Doc::GetDrawingDynamHeight(int staffSize, bool withMargin) const
{
    int height = GetGlyphHeight(SMUFL_E522_dynamicForte, staffSize, false);
    // This should be styled
    if (withMargin) height += GetDrawingUnit(staffSize);
    return height;
}

int Doc::GetDrawingHairpinSize(int staffSize, bool withMargin) const
{
    int size = m_style->m_hairpinSize * GetDrawingUnit(staffSize) / PARAM_DENOMINATOR;
    // This should be styled
    if (withMargin) size += GetDrawingUnit(staffSize);
    return size;
}

int Doc::GetDrawingBeamWidth(int staffSize, bool graceSize) const
{
    int value = m_drawingBeamWidth * staffSize / 100;
    if (graceSize) value = value * this->m_style->m_graceNum / this->m_style->m_graceDen;
    return value;
}

int Doc::GetDrawingBeamWhiteWidth(int staffSize, bool graceSize) const
{
    int value = m_drawingBeamWhiteWidth * staffSize / 100;
    if (graceSize) value = value * this->m_style->m_graceNum / this->m_style->m_graceDen;
    return value;
}

int Doc::GetDrawingLedgerLineLength(int staffSize, bool graceSize) const
{
    int value = m_drawingLedgerLine * staffSize / 100;
    if (graceSize) value = value * this->m_style->m_graceNum / this->m_style->m_graceDen;
    return value;
}

int Doc::GetCueSize(int value) const
{
    return value * this->m_style->m_graceNum / this->m_style->m_graceDen;
}

FontInfo *Doc::GetDrawingSmuflFont(int staffSize, bool graceSize)
{
    int value = m_drawingSmuflFontSize * staffSize / 100;
    if (graceSize) value = value * this->m_style->m_graceNum / this->m_style->m_graceDen;
    m_drawingSmuflFont.SetPointSize(value);
    return &m_drawingSmuflFont;
}

FontInfo *Doc::GetDrawingLyricFont(int staffSize)
{
    m_drawingLyricFont.SetPointSize(m_drawingLyricFontSize * staffSize / 100);
    return &m_drawingLyricFont;
}

char Doc::GetLeftMargin(const ClassId classId) const
{
    if (classId == ACCID) return m_style->m_leftMarginAccid;
    if (classId == BARLINE) return m_style->m_leftMarginBarLine;
    if (classId == BARLINE_ATTR_LEFT) return m_style->m_leftMarginBarLineAttrLeft;
    if (classId == BARLINE_ATTR_RIGHT) return m_style->m_leftMarginBarLineAttrRight;
    if (classId == BEATRPT) return m_style->m_leftMarginBeatRpt;
    if (classId == CHORD) return m_style->m_leftMarginChord;
    if (classId == CLEF) return m_style->m_leftMarginClef;
    if (classId == KEYSIG) return m_style->m_leftMarginKeySig;
    if (classId == MENSUR) return m_style->m_leftMarginMensur;
    if (classId == METERSIG) return m_style->m_leftMarginMeterSig;
    if (classId == MREST) return m_style->m_leftMarginMRest;
    if (classId == MRPT2) return m_style->m_leftMarginMRpt2;
    if (classId == MULTIREST) return m_style->m_leftMarginMultiRest;
    if (classId == MULTIRPT) return m_style->m_leftMarginMultiRpt;
    if (classId == NOTE) return m_style->m_leftMarginNote;
    if (classId == REST) return m_style->m_leftMarginRest;
    return m_style->m_leftMarginDefault;
}

char Doc::GetRightMargin(const ClassId classId) const
{
    if (classId == ACCID) return m_style->m_rightMarginAccid;
    if (classId == BARLINE) return m_style->m_rightMarginBarLine;
    if (classId == BARLINE_ATTR_LEFT) return m_style->m_rightMarginBarLineAttrLeft;
    if (classId == BARLINE_ATTR_RIGHT) return m_style->m_rightMarginBarLineAttrRight;
    if (classId == BEATRPT) return m_style->m_rightMarginBeatRpt;
    if (classId == CHORD) return m_style->m_rightMarginChord;
    if (classId == CLEF) return m_style->m_rightMarginClef;
    if (classId == KEYSIG) return m_style->m_rightMarginKeySig;
    if (classId == MENSUR) return m_style->m_rightMarginMensur;
    if (classId == METERSIG) return m_style->m_rightMarginMeterSig;
    if (classId == MREST) return m_style->m_rightMarginMRest;
    if (classId == MRPT2) return m_style->m_rightMarginMRpt2;
    if (classId == MULTIREST) return m_style->m_rightMarginMultiRest;
    if (classId == MULTIRPT) return m_style->m_rightMarginMultiRpt;
    if (classId == NOTE) return m_style->m_rightMarginNote;
    if (classId == REST) return m_style->m_rightMarginRest;
    return m_style->m_rightMarginDefault;
}

char Doc::GetBottomMargin(const ClassId classId) const
{
    return m_style->m_bottomMarginDefault;
}

char Doc::GetTopMargin(const ClassId classId) const
{
    return m_style->m_topMarginDefault;
}

char Doc::GetLeftPosition() const
{
    return m_style->m_leftPosition;
}

void Doc::SetPageHeight(int pageHeight)
{
    m_pageHeight = pageHeight * DEFINITION_FACTOR;
};

void Doc::SetPageWidth(int pageWidth)
{
    m_pageWidth = pageWidth * DEFINITION_FACTOR;
};

void Doc::SetPageLeftMar(short pageLeftMar)
{
    m_pageLeftMar = pageLeftMar * DEFINITION_FACTOR;
};

void Doc::SetPageRightMar(short pageRightMar)
{
    m_pageRightMar = pageRightMar * DEFINITION_FACTOR;
};

void Doc::SetPageTopMar(short pageTopMar)
{
    m_pageTopMar = pageTopMar * DEFINITION_FACTOR;
};

void Doc::SetSpacingStaff(short spacingStaff)
{
    m_spacingStaff = spacingStaff;
};

void Doc::SetSpacingSystem(short spacingSystem)
{
    m_spacingSystem = spacingSystem;
};

Page *Doc::SetDrawingPage(int pageIdx)
{
    // out of range
    if (!HasPage(pageIdx)) {
        return NULL;
    }
    // nothing to do
    if (m_drawingPage && m_drawingPage->GetIdx() == pageIdx) {
        return m_drawingPage;
    }
    m_drawingPage = dynamic_cast<Page *>(this->GetChild(pageIdx));
    assert(m_drawingPage);

    int glyph_size;

    // we use the page members only if set (!= -1)
    if (m_drawingPage->m_pageHeight != -1) {
        m_drawingPageHeight = m_drawingPage->m_pageHeight;
        m_drawingPageWidth = m_drawingPage->m_pageWidth;
        m_drawingPageLeftMar = m_drawingPage->m_pageLeftMar;
        m_drawingPageRightMar = m_drawingPage->m_pageRightMar;
        m_drawingPageTopMar = m_drawingPage->m_pageTopMar;
    }
    else if (this->m_pageHeight != -1) {
        m_drawingPageHeight = this->m_pageHeight;
        m_drawingPageWidth = this->m_pageWidth;
        m_drawingPageLeftMar = this->m_pageLeftMar;
        m_drawingPageRightMar = this->m_pageRightMar;
        m_drawingPageTopMar = this->m_pageTopMar;
    }
    else {
        m_drawingPageHeight = m_style->m_pageHeight;
        m_drawingPageWidth = m_style->m_pageWidth;
        m_drawingPageLeftMar = m_style->m_pageLeftMar;
        m_drawingPageRightMar = m_style->m_pageRightMar;
        m_drawingPageTopMar = m_style->m_pageTopMar;
    }

    if (this->m_style->m_landscape) {
        int pageHeight = m_drawingPageWidth;
        m_drawingPageWidth = m_drawingPageHeight;
        m_drawingPageHeight = pageHeight;
        int pageRightMar = m_drawingPageLeftMar;
        m_drawingPageLeftMar = m_drawingPageRightMar;
        m_drawingPageRightMar = pageRightMar;
    }

    // From here we could check if values have changed
    // Since  m_style->m_interlDefin stays the same, it's useless to do it
    // every time for now.

    m_drawingBeamMaxSlope = this->m_style->m_beamMaxSlope;
    m_drawingBeamMinSlope = this->m_style->m_beamMinSlope;
    m_drawingBeamMaxSlope /= 100;
    m_drawingBeamMinSlope /= 100;

    // half of the space between two lines
    m_drawingUnit = m_style->m_unit;
    // space between two lines
    m_drawingDoubleUnit = m_drawingUnit * 2;
    // staff (with five lines)
    m_drawingStaffSize = m_drawingDoubleUnit * 4;
    // octave height
    m_drawingOctaveSize = m_drawingUnit * 7;
    // measure minimal width
    m_drawingMinMeasureWidth = m_drawingUnit * m_style->m_minMeasureWidth / PARAM_DENOMINATOR;

    // values for beams
    m_drawingBeamWidth = this->m_style->m_unit;
    m_drawingBeamWhiteWidth = this->m_style->m_unit / 2;

    // values for fonts
    m_drawingSmuflFontSize = CalcMusicFontSize();
    m_drawingLyricFontSize = m_drawingUnit * m_style->m_lyricSize / PARAM_DENOMINATOR;

    glyph_size = GetGlyphWidth(SMUFL_E0A3_noteheadHalf, 100, 0);
    m_drawingLedgerLine = glyph_size * 72 / 100;

    glyph_size = GetGlyphWidth(SMUFL_E0A2_noteheadWhole, 100, 0);

    m_drawingBrevisWidth = (int)((glyph_size * 0.8) / 2);

    return m_drawingPage;
}

int Doc::CalcMusicFontSize()
{
    return m_style->m_unit * 8;
}

int Doc::GetAdjustedDrawingPageHeight() const
{
    assert(m_drawingPage);

    if (this->GetType() == Transcription) return m_drawingPage->m_pageHeight / DEFINITION_FACTOR;

    int contentHeight = m_drawingPage->GetContentHeight();
    return (contentHeight + m_drawingPageTopMar * 2) / DEFINITION_FACTOR;
}

int Doc::GetAdjustedDrawingPageWidth() const
{
    assert(m_drawingPage);

    if (this->GetType() == Transcription) return m_drawingPage->m_pageWidth / DEFINITION_FACTOR;

    int contentWidth = m_drawingPage->GetContentWidth();
    return (contentWidth + m_drawingPageLeftMar + m_drawingPageRightMar) / DEFINITION_FACTOR;
}

//----------------------------------------------------------------------------
// Doc functors methods
//----------------------------------------------------------------------------

int Doc::PrepareLyricsEnd(FunctorParams *functorParams)
{
    PrepareLyricsParams *params = dynamic_cast<PrepareLyricsParams *>(functorParams);
    assert(params);

    if ((params->m_currentSyl && params->m_lastNote) && (params->m_currentSyl->GetStart() != params->m_lastNote)) {
        params->m_currentSyl->SetEnd(params->m_lastNote);
    }

    return FUNCTOR_STOP;
}

} // namespace vrv
