/////////////////////////////////////////////////////////////////////////////
// Name:        doc.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "doc.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>
#include <ranges>

//----------------------------------------------------------------------------

#include "alignfunctor.h"
#include "barline.h"
#include "beatrpt.h"
#include "castofffunctor.h"
#include "chord.h"
#include "comparison.h"
#include "convertfunctor.h"
#include "docselection.h"
#include "expansion.h"
#include "facsimilefunctor.h"
#include "featureextractor.h"
#include "functor.h"
#include "glyph.h"
#include "instrdef.h"
#include "iomei.h"
#include "keysig.h"
#include "label.h"
#include "layer.h"
#include "mdiv.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "midifunctor.h"
#include "miscfunctor.h"
#include "mnum.h"
#include "mrest.h"
#include "mrpt.h"
#include "mrpt2.h"
#include "multirest.h"
#include "multirpt.h"
#include "note.h"
#include "page.h"
#include "pages.h"
#include "pgfoot.h"
#include "pghead.h"
#include "preparedatafunctor.h"
#include "resetfunctor.h"
#include "runningelement.h"
#include "score.h"
#include "scoringupfunctor.h"
#include "setscoredeffunctor.h"
#include "slur.h"
#include "smufl.h"
#include "staff.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "surface.h"
#include "syl.h"
#include "syllable.h"
#include "system.h"
#include "tempo.h"
#include "text.h"
#include "timemap.h"
#include "timestamp.h"
#include "transposefunctor.h"
#include "transposition.h"
#include "verse.h"
#include "vrv.h"
#include "zone.h"

//----------------------------------------------------------------------------

#include "MidiEvent.h"
#include "MidiFile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Doc
//----------------------------------------------------------------------------

Doc::Doc() : Object(DOC)
{
    m_options = new Options();

    // owned pointers need to be set to NULL;
    m_selectionPreceding = NULL;
    m_selectionFollowing = NULL;
    m_focusRange = NULL;

    this->Reset();
}

Doc::~Doc()
{
    this->ClearSelectionPages();

    delete m_options;
    if (m_focusRange) delete m_focusRange;
}

void Doc::Reset()
{
    Object::Reset();
    this->ResetID();

    this->ClearSelectionPages();

    m_type = Raw;
    m_notationType = NOTATIONTYPE_NONE;
    m_pageHeight = -1;
    m_pageWidth = -1;
    m_pageMarginBottom = 0;
    m_pageMarginRight = 0;
    m_pageMarginLeft = 0;
    m_pageMarginTop = 0;

    m_drawingPageHeight = -1;
    m_drawingPageWidth = -1;
    m_drawingPageContentHeight = -1;
    m_drawingPageContentWidth = -1;
    m_drawingPageMarginBottom = 0;
    m_drawingPageMarginRight = 0;
    m_drawingPageMarginLeft = 0;
    m_drawingPageMarginTop = 0;

    m_drawingPage = NULL;
    m_currentScoreDefDone = false;
    m_dataPreparationDone = false;
    m_timemapTempo = 0.0;
    m_markup = MARKUP_DEFAULT;
    m_isMensuralMusicOnly = BOOLEAN_NONE;
    m_isNeumeLines = false;
    m_isCastOff = false;
    m_visibleScores.clear();
    m_focusStatus = FOCUS_UNSET;

    m_facsimile = NULL;

    m_drawingSmuflFontSize = 0;
    m_drawingLyricFontSize = 0;

    m_header.reset();
    m_front.reset();
    m_back.reset();
}

void Doc::ResetToLoading()
{
    if (m_currentScoreDefDone) {
        ScoreDefUnsetCurrentFunctor scoreDefUnsetCurrent;
        this->Process(scoreDefUnsetCurrent);
        m_currentScoreDefDone = false;
    }
}

void Doc::ClearSelectionPages()
{
    if (m_selectionPreceding) {
        delete m_selectionPreceding;
        m_selectionPreceding = NULL;
    }
    if (m_selectionFollowing) {
        delete m_selectionFollowing;
        m_selectionFollowing = NULL;
    }
    m_selectionStart = "";
    m_selectionEnd = "";
}

void Doc::SetType(DocType type)
{
    m_type = type;
}

bool Doc::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ MDIV, PAGES };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

bool Doc::GenerateDocumentScoreDef()
{
    Measure *measure = vrv_cast<Measure *>(this->FindDescendantByType(MEASURE));
    if (!measure) {
        LogError("No measure found for generating a scoreDef");
        return false;
    }

    ListOfObjects staves = measure->FindAllDescendantsByType(STAFF, false);

    if (staves.empty()) {
        LogError("No staff found for generating a scoreDef");
        return false;
    }

    ScoreDef *scoreDef = this->GetFirstScoreDef();
    scoreDef->Reset();
    StaffGrp *staffGrp = new StaffGrp();
    for (Object *object : staves) {
        Staff *staff = vrv_cast<Staff *>(object);
        assert(staff);
        StaffDef *staffDef = new StaffDef();
        staffDef->SetN(staff->GetN());
        staffDef->SetLines(5);
        if (!measure->IsMeasuredMusic()) staffDef->SetNotationtype(NOTATIONTYPE_mensural);
        staffGrp->AddChild(staffDef);
    }
    scoreDef->AddChild(staffGrp);

    LogInfo("ScoreDef generated");

    return true;
}

void Doc::GenerateFooter()
{
    for (Score *score : this->GetVisibleScores()) {
        ScoreDef *scoreDef = score->GetScoreDef();
        assert(scoreDef);
        if (scoreDef->FindDescendantByType(PGFOOT)) continue;

        PgFoot *pgFoot = new PgFoot();
        pgFoot->SetFunc(PGFUNC_first);
        // We mark it as generated for not having it written in the output
        pgFoot->IsGenerated(true);
        pgFoot->LoadFooter(this);
        pgFoot->SetType("autogenerated");
        scoreDef->AddChild(pgFoot);

        PgFoot *pgFoot2 = new PgFoot();
        pgFoot2->SetFunc(PGFUNC_all);
        pgFoot2->IsGenerated(true);
        pgFoot2->LoadFooter(this);
        pgFoot2->SetType("autogenerated");
        scoreDef->AddChild(pgFoot2);
    }
}

void Doc::GenerateHeader()
{
    for (Score *score : this->GetVisibleScores()) {
        ScoreDef *scoreDef = score->GetScoreDef();
        assert(scoreDef);
        if (scoreDef->FindDescendantByType(PGHEAD)) continue;

        PgHead *pgHead = new PgHead();
        pgHead->SetFunc(PGFUNC_first);
        // We mark it as generated for not having it written in the output
        pgHead->IsGenerated(true);
        pgHead->GenerateFromMEIHeader(m_header);
        pgHead->SetType("autogenerated");
        scoreDef->AddChild(pgHead);

        PgHead *pgHead2 = new PgHead();
        pgHead2->SetFunc(PGFUNC_all);
        pgHead2->IsGenerated(true);
        pgHead2->AddPageNum(HORIZONTALALIGNMENT_center, VERTICALALIGNMENT_top);
        pgHead2->SetType("autogenerated");
        scoreDef->AddChild(pgHead2);
    }
}

void Doc::PrepareMeasureIndices()
{
    ListOfObjects measures = this->FindAllDescendantsByType(MEASURE, false);

    int index = 0;
    for (Object *object : measures) {
        vrv_cast<Measure *>(object)->SetIndex(++index);
    }
}

bool Doc::GenerateMeasureNumbers()
{
    ListOfObjects measures = this->FindAllDescendantsByType(MEASURE, false);

    // run through all measures and generate missing mNum from attribute
    for (Object *object : measures) {
        Measure *measure = vrv_cast<Measure *>(object);
        // First remove previously generated elements
        ListOfObjects mNums = measure->FindAllDescendantsByType(MNUM);
        for (Object *child : mNums) {
            MNum *mNum = vrv_cast<MNum *>(child);
            assert(mNum);
            if (mNum->IsGenerated()) {
                measure->DeleteChild(mNum);
            }
        }
        if (measure->HasN() && !measure->FindDescendantByType(MNUM)) {
            MNum *mnum = new MNum;
            Text *text = new Text;
            text->SetText(UTF8to32(measure->GetN()));
            mnum->SetType("autogenerated");
            mnum->AddChild(text);
            mnum->IsGenerated(true);
            measure->AddChild(mnum);
        }
    }

    return true;
}

void Doc::GenerateMEIHeader()
{
    m_header.remove_children();
    pugi::xml_node meiHead = m_header.append_child("meiHead");
    pugi::xml_node fileDesc = meiHead.append_child("fileDesc");
    pugi::xml_node titleStmt = fileDesc.append_child("titleStmt");
    titleStmt.append_child("title");

    pugi::xml_node pubStmt = fileDesc.append_child("pubStmt");
    pugi::xml_node date = pubStmt.append_child("date");

    // date
    time_t t = time(0); // get time now
    struct tm *now = localtime(&t);
    std::string dateStr = StringFormat("%d-%02d-%02d-%02d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1,
        now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
    date.append_attribute("isodate") = dateStr.c_str();

    // encodingDesc
    pugi::xml_node encodingDesc = meiHead.append_child("encodingDesc");
    // appInfo/application/name
    pugi::xml_node appInfo = encodingDesc.append_child("appInfo");
    pugi::xml_node application = appInfo.append_child("application");
    application.append_attribute("xml:id") = "verovio";
    application.append_attribute("version") = GetVersion().c_str();
    pugi::xml_node name = application.append_child("name");
    name.text().set(StringFormat("Verovio (%s)", GetVersion().c_str()).c_str());
    // projectDesc
    pugi::xml_node projectDesc = encodingDesc.append_child("projectDesc");
    pugi::xml_node p1 = projectDesc.append_child("p");
    p1.text().set(StringFormat("MEI encoded with Verovio").c_str());
}

void Doc::ConvertHeaderToMEIBasic()
{
    pugi::xpath_node_set toRemove;

    // Keep only fileDesc
    toRemove = m_header.select_nodes("//meiHead/*[not(self::fileDesc)]");
    // Remove each of the selected nodes
    for (pugi::xpath_node node : toRemove) {
        node.node().parent().remove_child(node.node());
    }

    // Keep only  titleStmt, respStmt, composer, arranger and lyricist in fileDesc
    pugi::xml_node titleStmt = m_header.select_node("//meiHead/fileDesc/titleStmt").node();
    // Remove each of the selected nodes
    toRemove = titleStmt.select_nodes(
        "./*[not(self::title or self::respStmt or self::composer or self::arranger or self::lyricist)]");
    // Remove each of the selected nodes
    for (pugi::xpath_node node : toRemove) {
        node.node().parent().remove_child(node.node());
    }
}

bool Doc::HasTimemap() const
{
    return (m_timemapTempo == m_options->m_midiTempoAdjustment.GetValue());
}

void Doc::CalculateTimemap()
{
    // There is no data to calculate the timemap
    if (this->GetPageCount() == 0) {
        return;
    }

    m_timemapTempo = 0.0;

    // This happens if the document was never cast off (breaks none option in the toolkit)
    if (!m_drawingPage) {
        Page *page = this->SetDrawingPage(0);
        assert(page);
        this->ScoreDefSetCurrentDoc();
        page->LayOutHorizontally();
    }

    double tempo = MIDI_TEMPO;

    // Set tempo
    ScoreDef *scoreDef = this->GetFirstVisibleScore()->GetScoreDef();
    assert(scoreDef);
    if (scoreDef->HasMidiBpm()) {
        tempo = scoreDef->GetMidiBpm();
    }
    else if (scoreDef->HasMm()) {
        tempo = Tempo::CalcTempo(scoreDef);
    }

    // We first calculate the maximum duration of each measure
    InitMaxMeasureDurationFunctor initMaxMeasureDuration;
    initMaxMeasureDuration.SetCurrentTempo(tempo);
    initMaxMeasureDuration.SetTempoAdjustment(m_options->m_midiTempoAdjustment.GetValue());
    this->Process(initMaxMeasureDuration);

    // Then calculate the onset and offset times (w.r.t. the measure) for every note
    InitOnsetOffsetFunctor initOnsetOffset(this);
    this->Process(initOnsetOffset);

    // Adjust the duration of tied notes
    InitTimemapTiesFunctor initTimemapTies;
    initTimemapTies.SetDirection(BACKWARD);
    this->Process(initTimemapTies);

    // Adjust the duration of notes (grace notes and arpeggios)
    InitTimemapAdjustNotesFunctor initTimemapAdjustNotes;
    initTimemapAdjustNotes.SetNoCue(this->GetOptions()->m_midiNoCue.GetValue());
    this->Process(initTimemapAdjustNotes);

    m_timemapTempo = m_options->m_midiTempoAdjustment.GetValue();
}

void Doc::ExportMIDI(smf::MidiFile *midiFile)
{
    midiFile->absoluteTicks();

    if (!this->HasTimemap()) {
        // generate MIDI timemap before progressing
        this->CalculateTimemap();
    }
    if (!this->HasTimemap()) {
        LogWarning("Calculation of the timemap failed, MIDI cannot be exported.");
    }

    double tempo = MIDI_TEMPO;
    std::set<int> tempoEventTicks; // track the ticks of added tempo events

    // set MIDI tempo
    ScoreDef *scoreDef = this->GetFirstVisibleScore()->GetScoreDef();
    assert(scoreDef);
    if (scoreDef->HasMidiBpm()) {
        tempo = scoreDef->GetMidiBpm();
        tempoEventTicks.insert(0);
        midiFile->addTempo(0, 0, tempo);
    }
    else if (scoreDef->HasMm()) {
        tempo = Tempo::CalcTempo(scoreDef);
        tempoEventTicks.insert(0);
        midiFile->addTempo(0, 0, tempo);
    }

    // Capture information for MIDI generation, i.e. from control elements
    InitMIDIFunctor initMIDI;
    initMIDI.SetCurrentTempo(tempo);
    this->Process(initMIDI);

    // We need to populate processing lists for processing the document by Layer (by Verse will not be used)
    InitProcessingListsFunctor initProcessingLists;

    // We first fill a tree of int with [staff/layer] and [staff/layer/verse] numbers (@n) to be processed
    this->Process(initProcessingLists);
    const IntTree &layerTree = initProcessingLists.GetLayerTree();

    // The tree is used to process each staff/layer/verse separately
    // For this, we use a array of AttNIntegerComparison that looks for each object if it is of the type
    // and with @n specified

    // Process notes and chords, rests, spaces layer by layer
    // track 0 (included by default) is reserved for meta messages common to all tracks
    int midiChannel = 0;
    int midiTrack = 1;
    Filters filters;
    for (auto &staves : layerTree.child) {
        int transSemi = 0;
        if (StaffDef *staffDef = scoreDef->GetStaffDef(staves.first)) {
            // get the transposition (semi-tone) value for the staff
            if (staffDef->HasTransSemi()) transSemi = staffDef->GetTransSemi();
            midiTrack = staffDef->GetN();
            if (midiFile->getTrackCount() < (midiTrack + 1)) {
                midiFile->addTracks(midiTrack + 1 - midiFile->getTrackCount());
            }
            // set MIDI channel and instrument
            InstrDef *instrdef = vrv_cast<InstrDef *>(staffDef->FindDescendantByType(INSTRDEF, 1));
            if (!instrdef) {
                StaffGrp *staffGrp = vrv_cast<StaffGrp *>(staffDef->GetFirstAncestor(STAFFGRP));
                assert(staffGrp);
                instrdef = vrv_cast<InstrDef *>(staffGrp->FindDescendantByType(INSTRDEF, 1));
            }
            if (instrdef) {
                if (instrdef->HasMidiChannel()) midiChannel = instrdef->GetMidiChannel();
                if (instrdef->HasMidiTrack()) {
                    midiTrack = instrdef->GetMidiTrack();
                    if (midiFile->getTrackCount() < (midiTrack + 1)) {
                        midiFile->addTracks(midiTrack + 1 - midiFile->getTrackCount());
                    }
                    if (midiTrack > 255) {
                        LogWarning("A high MIDI track number was assigned to staff %d", staffDef->GetN());
                    }
                }
                if (instrdef->HasMidiInstrnum()) {
                    midiFile->addPatchChange(midiTrack, 0, midiChannel, instrdef->GetMidiInstrnum());
                }
            }
            // set MIDI track name
            Label *label = vrv_cast<Label *>(staffDef->FindDescendantByType(LABEL, 1));
            if (!label) {
                StaffGrp *staffGrp = vrv_cast<StaffGrp *>(staffDef->GetFirstAncestor(STAFFGRP));
                assert(staffGrp);
                label = vrv_cast<Label *>(staffGrp->FindDescendantByType(LABEL, 1));
            }
            if (label) {
                std::string trackName = UTF32to8(label->GetText()).c_str();
                if (!trackName.empty()) midiFile->addTrackName(midiTrack, 0, trackName);
            }
            // set MIDI key signature
            KeySig *keySig = vrv_cast<KeySig *>(staffDef->FindDescendantByType(KEYSIG));
            if (!keySig && (scoreDef->HasKeySigInfo())) {
                keySig = vrv_cast<KeySig *>(scoreDef->GetKeySig());
            }
            if (keySig && keySig->HasSig()) {
                midiFile->addKeySignature(midiTrack, 0, keySig->GetFifthsInt(), (keySig->GetMode() == MODE_minor));
            }
            // set MIDI time signature
            MeterSig *meterSig = vrv_cast<MeterSig *>(staffDef->FindDescendantByType(METERSIG));
            if (!meterSig && (scoreDef->HasMeterSigInfo())) {
                meterSig = vrv_cast<MeterSig *>(scoreDef->GetMeterSig());
            }
            if (meterSig && meterSig->HasCount() && meterSig->HasUnit()) {
                midiFile->addTimeSignature(midiTrack, 0, meterSig->GetTotalCount(), meterSig->GetUnit());
            }
            else if (meterSig && meterSig->HasSym()) {
                midiFile->addTimeSignature(midiTrack, 0, meterSig->GetTotalCount(), meterSig->GetSymImplicitUnit());
            }
        }

        // Set initial scoreDef values for tuning
        GenerateMIDIFunctor generateScoreDefMIDI(midiFile);
        generateScoreDefMIDI.SetChannel(midiChannel);
        generateScoreDefMIDI.SetTrack(midiTrack);

        scoreDef->Process(generateScoreDefMIDI);

        bool controlEvents = true;

        for (auto &layers : staves.second.child) {
            filters.Clear();
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves.first);
            AttNIntegerComparison matchLayer(LAYER, layers.first);
            filters.Add(&matchStaff);
            filters.Add(&matchLayer);

            GenerateMIDIFunctor generateMIDI(midiFile);
            generateMIDI.SetFilters(&filters);

            generateMIDI.SetChannel(midiChannel);
            generateMIDI.SetTrack(midiTrack);
            generateMIDI.SetStaffN(staves.first);
            generateMIDI.SetLayerN(layers.first);
            generateMIDI.SetTempoEventTicks(tempoEventTicks);
            generateMIDI.SetTransSemi(transSemi);
            generateMIDI.SetCurrentTempo(tempo);
            generateMIDI.SetOctaves(initMIDI.GetOctaves());
            generateMIDI.SetNoCue(this->GetOptions()->m_midiNoCue.GetValue());
            generateMIDI.SetControlEvents(controlEvents);

            // LogDebug("Exporting track %d ----------------", midiTrack);
            this->Process(generateMIDI);

            tempoEventTicks = generateMIDI.GetTempoEventTicks();
            // Process them only once per staff
            controlEvents = false;
        }
    }
    midiFile->sortTracksNoteOffsBeforeOns();
}

bool Doc::ExportTimemap(std::string &output, bool includeRests, bool includeMeasures, bool useFractions)
{
    if (!this->HasTimemap()) {
        // generate MIDI timemap before progressing
        this->CalculateTimemap();
    }
    if (!this->HasTimemap()) {
        LogWarning("Calculation of the timemap failed, the timemap cannot be exported.");
        output = "{}";
        return false;
    }
    Timemap timemap;
    GenerateTimemapFunctor generateTimemap(&timemap);
    generateTimemap.SetNoCue(this->GetOptions()->m_midiNoCue.GetValue());
    this->Process(generateTimemap);

    timemap.ToJson(output, includeRests, includeMeasures, useFractions);

    return true;
}

bool Doc::ExportExpansionMap(std::string &output)
{
    if (m_expansionMap.HasExpansionMap()) {
        m_expansionMap.ToJson(output);
        return true;
    }
    output = "{}";
    return false;
}

bool Doc::ExportFeatures(std::string &output, const std::string &options)
{
    if (!this->HasTimemap()) {
        // generate MIDI timemap before progressing
        this->CalculateTimemap();
    }
    if (!this->HasTimemap()) {
        LogWarning("Calculation of the timemap failed, the features cannot be exported.");
        output = "{}";
        return false;
    }
    FeatureExtractor extractor(options);
    GenerateFeaturesFunctor generateFeatures(&extractor);
    this->Process(generateFeatures);
    extractor.ToJson(output);

    return true;
}

void Doc::PrepareData()
{
    Object *root = this;
    if (m_focusStatus != FOCUS_UNSET) {
        m_focusStatus = FOCUS_USED;
        root = m_focusRange;
    }

    /************ Reset and initialization ************/

    if (m_dataPreparationDone) {
        // Reset the scoreDef for the entire doc
        this->ResetToLoading();
        ResetDataFunctor resetData;
        root->Process(resetData);
    }
    PrepareDataInitializationFunctor prepareDataInitialization(this);
    root->Process(prepareDataInitialization);

    /************ Generate measure indices ************/

    this->PrepareMeasureIndices();

    /************ Collect all visible scores ************/

    this->CollectVisibleScores();

    /************ Store default durations ************/

    PrepareDurationFunctor prepareDuration;
    root->Process(prepareDuration);

    /************ Resolve @startid / @endid ************/

    // Try to match all spanning elements (slur, tie, etc) by processing backwards
    PrepareTimeSpanningFunctor prepareTimeSpanning;
    root->Process(prepareTimeSpanning);
    prepareTimeSpanning.SetDataCollectionCompleted();

    // First we try a forward pass which should collect most of the spanning elements.
    // However, in some cases, one (or both) end points might appear a few measures
    // before the spanning element in the encoding. For these, the previous iteration will not have resolved the link
    // and the spanning elements will remain in the timeSpanningElements array. We try again forwards but this time
    // without filling the list (that is only resolving remaining elements).
    const ListOfSpanningInterOwnerPairs &interfaceOwnerPairs = prepareTimeSpanning.GetInterfaceOwnerPairs();
    if (!interfaceOwnerPairs.empty()) {
        root->Process(prepareTimeSpanning);
    }

    // Display warning if some elements were not matched
    const int unmatchedElements = (int)std::count_if(interfaceOwnerPairs.cbegin(), interfaceOwnerPairs.cend(),
        [](const ListOfSpanningInterOwnerPairs::value_type &entry) {
            return (entry.first->HasStartid() && entry.first->HasEndid());
        });
    if (unmatchedElements > 0) {
        LogWarning("%d time spanning element(s) with startid and endid could not be matched.", unmatchedElements);
    }

    /************ Resolve @startid (only) ************/

    // Resolve <reh> elements first, since they can be encoded without @startid or @tstamp, but we need one internally
    // for placement
    PrepareRehPositionFunctor prepareRehPosition;
    root->Process(prepareRehPosition);

    // Try to match all time pointing elements (tempo, fermata, etc) by processing backwards
    PrepareTimePointingFunctor prepareTimePointing;
    prepareTimePointing.SetDirection(BACKWARD);
    root->Process(prepareTimePointing);

    /************ Resolve @tstamp / tstamp2 ************/

    // Now try to match the @tstamp and @tstamp2 attributes.
    PrepareTimestampsFunctor prepareTimestamps;
    root->Process(prepareTimestamps);

    // If some are still there, then it is probably an issue in the encoding
    if (!prepareTimestamps.GetInterfaceIDPairs().empty()) {
        LogWarning("%d time spanning element(s) with timestamps could not be matched.",
            prepareTimestamps.GetInterfaceIDPairs().size());
    }

    /************ Resolve linking (@next) ************/

    // Try to match all pointing elements using @next, @sameas and @stem.sameas
    PrepareLinkingFunctor prepareLinking;
    root->Process(prepareLinking);
    prepareLinking.SetDataCollectionCompleted();

    // If we have some left process again backward
    if (!prepareLinking.GetSameasIDPairs().empty() || !prepareLinking.GetStemSameasIDPairs().empty()) {
        prepareLinking.SetDirection(BACKWARD);
        root->Process(prepareLinking);
    }

    // If some are still there, then it is probably an issue in the encoding
    if (!prepareLinking.GetNextIDPairs().empty()) {
        LogWarning("%d element(s) with a @next could not match the target", prepareLinking.GetNextIDPairs().size());
    }
    if (!prepareLinking.GetSameasIDPairs().empty()) {
        LogWarning("%d element(s) with a @sameas could not match the target", prepareLinking.GetSameasIDPairs().size());
    }
    if (!prepareLinking.GetStemSameasIDPairs().empty()) {
        LogWarning("%d element(s) with a @stem.sameas could not match the target",
            prepareLinking.GetStemSameasIDPairs().size());
    }

    /************ Resolve @plist ************/

    // Try to match all pointing elements using @plist
    PreparePlistFunctor preparePlist;
    root->Process(preparePlist);
    preparePlist.SetDataCollectionCompleted();

    // Process plist after all pairs have been collected
    if (!preparePlist.GetInterfaceIDPairs().empty()) {
        root->Process(preparePlist);
    }

    // If some are still there, then it is probably an issue in the encoding
    if (!preparePlist.GetInterfaceIDPairs().empty()) {
        LogWarning("%d element(s) with a @plist could not match the target", preparePlist.GetInterfaceIDPairs().size());
    }

    /************ Resolve cross staff ************/

    // Prepare the cross-staff pointers
    PrepareCrossStaffFunctor prepareCrossStaff;
    root->Process(prepareCrossStaff);

    /************ Resolve beamspan elements ***********/

    PrepareBeamSpanElementsFunctor prepareBeamSpanElements;
    root->Process(prepareBeamSpanElements);

    /************ Match pedal lines ***********/

    PreparePedalsFunctor preparePedals(this);
    root->Process(preparePedals);

    /************ Prepare processing by staff/layer/verse ************/

    // We need to populate processing lists for processing the document by Layer (for matching @tie) and
    // by Verse (for matching syllable connectors)
    InitProcessingListsFunctor initProcessingLists;

    // We first fill a tree of ints with [staff/layer] and [staff/layer/verse] numbers (@n) to be processed
    // LogElapsedTimeStart();
    root->Process(initProcessingLists);
    const IntTree &layerTree = initProcessingLists.GetLayerTree();
    const IntTree &verseTree = initProcessingLists.GetVerseTree();

    // The tree is used to process each staff/layer/verse separately
    // For this, we use an array of AttNIntegerComparison that looks for each object if it is of the type
    // and with @n specified

    /************ Resolve some pointers by layer ************/

    Filters filters;
    for (auto &staves : layerTree.child) {
        for (auto &layers : staves.second.child) {
            filters.Clear();
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves.first);
            AttNIntegerComparison matchLayer(LAYER, layers.first);
            filters.Add(&matchStaff);
            filters.Add(&matchLayer);

            PreparePointersByLayerFunctor preparePointersByLayer;
            preparePointersByLayer.SetFilters(&filters);
            root->Process(preparePointersByLayer);
        }
    }

    /************ Resolve delayed turns ************/

    PrepareDelayedTurnsFunctor prepareDelayedTurns;
    root->Process(prepareDelayedTurns);
    prepareDelayedTurns.SetDataCollectionCompleted();

    if (!prepareDelayedTurns.GetDelayedTurns().empty()) {
        for (auto &staves : layerTree.child) {
            for (auto &layers : staves.second.child) {
                filters.Clear();
                // Create ad comparison object for each type / @n
                AttNIntegerComparison matchStaff(STAFF, staves.first);
                AttNIntegerComparison matchLayer(LAYER, layers.first);
                filters.Add(&matchStaff);
                filters.Add(&matchLayer);

                prepareDelayedTurns.SetFilters(&filters);
                prepareDelayedTurns.ResetCurrent();
                root->Process(prepareDelayedTurns);
            }
        }
    }

    /************ Resolve lyric connectors ************/

    // Same for the lyrics, but Verse by Verse since Syl are TimeSpanningInterface elements for handling connectors
    for (auto &staves : verseTree.child) {
        for (auto &layers : staves.second.child) {
            for (auto &verses : layers.second.child) {
                // std::cout << staves->first << " => " << layers->first << " => " << verses->first << '\n';
                filters.Clear();
                // Create ad comparison object for each type / @n
                AttNIntegerComparison matchStaff(STAFF, staves.first);
                AttNIntegerComparison matchLayer(LAYER, layers.first);
                AttNIntegerComparison matchVerse(VERSE, verses.first);
                filters.Add(&matchStaff);
                filters.Add(&matchLayer);
                filters.Add(&matchVerse);

                // The first pass sets m_drawingFirstNote and m_drawingLastNote for each syl
                // m_drawingLastNote is set only if the syl has a forward connector
                PrepareLyricsFunctor prepareLyrics;
                prepareLyrics.SetFilters(&filters);
                root->Process(prepareLyrics);
            }
        }
    }

    /************ Fill control event spanning ************/

    // Once <slur>, <ties> and @ties are matched but also syl connectors, we need to set them as running
    // TimeSpanningInterface to each staff they are extended. This does not need to be done staff by staff because we
    // can just check the staff->GetN to see where we are (see PrepareStaffCurrentTimeSpanningFunctor::VisitStaff)
    PrepareStaffCurrentTimeSpanningFunctor prepareStaffCurrentTimeSpanning;
    root->Process(prepareStaffCurrentTimeSpanning);

    // Something must be wrong in the encoding because a TimeSpanningInterface was left open
    if (!prepareStaffCurrentTimeSpanning.GetTimeSpanningElements().empty()) {
        LogDebug("%d time spanning elements could not be set as running",
            prepareStaffCurrentTimeSpanning.GetTimeSpanningElements().size());
    }

    /************ Resolve mRpt ************/

    // Process by staff for matching mRpt elements and setting the drawing number
    for (auto &staves : layerTree.child) {
        for (auto &layers : staves.second.child) {
            filters.Clear();
            // Create ad comparison object for each type / @n
            AttNIntegerComparison matchStaff(STAFF, staves.first);
            AttNIntegerComparison matchLayer(LAYER, layers.first);
            filters.Add(&matchStaff);
            filters.Add(&matchLayer);

            // We set multiNumber to NONE for indicated we need to look at the staffDef when reaching the first staff
            PrepareRptFunctor prepareRpt(this);
            prepareRpt.SetFilters(&filters);
            root->Process(prepareRpt);
        }
    }

    /************ Resolve endings ************/

    // Prepare the endings (pointers to the measure after and before the boundaries)
    PrepareMilestonesFunctor prepareMilestones;
    root->Process(prepareMilestones);

    /************ Resolve floating groups for vertical alignment ************/

    // Prepare the floating drawing groups
    PrepareFloatingGrpsFunctor prepareFloatingGrps;
    root->Process(prepareFloatingGrps);

    /************ Resolve cue size ************/

    // Prepare the drawing cue size
    PrepareCueSizeFunctor prepareCueSize;
    root->Process(prepareCueSize);

    /************ Resolve @altsym ************/

    // Try to match all pointing elements using @next, @sameas and @stem.sameas
    PrepareAltSymFunctor prepareAltSym;
    root->Process(prepareAltSym);

    /************ Instanciate LayerElement parts (stem, flag, dots, etc) ************/

    PrepareLayerElementPartsFunctor prepareLayerElementParts;
    root->Process(prepareLayerElementParts);

    /************ Resolve @facs ************/
    if (this->IsFacs()) {
        // Associate zones with elements
        PrepareFacsimileFunctor prepareFacsimile(this->GetFacsimile());
        root->Process(prepareFacsimile);

        // Add default syl zone if one is not present.
        for (Object *object : prepareFacsimile.GetZonelessSyls()) {
            Syl *syl = vrv_cast<Syl *>(object);
            assert(syl);
            syl->CreateDefaultZone(this);
        }
    }

    for (Score *score : this->GetVisibleScores()) {
        ScoreDefSetGrpSymFunctor scoreDefSetGrpSym;
        assert(score->GetScoreDef());
        score->GetScoreDef()->Process(scoreDefSetGrpSym);
    }

    // LogElapsedTimeEnd ("Preparing drawing");

    m_dataPreparationDone = true;
}

void Doc::ScoreDefSetCurrentDoc(bool force)
{
    if (m_currentScoreDefDone && !force) {
        return;
    }

    if (m_currentScoreDefDone) {
        ScoreDefUnsetCurrentFunctor scoreDefUnsetCurrent;
        this->Process(scoreDefUnsetCurrent);
    }

    // First we need to set Page::m_score and Page::m_scoreEnd
    ScoreDefSetCurrentPageFunctor scoreDefSetCurrentPage(this);
    this->Process(scoreDefSetCurrentPage, 3);

    ScoreDefSetCurrentFunctor scoreDefSetCurrent(this);
    this->Process(scoreDefSetCurrent);

    this->ScoreDefSetGrpSymDoc();

    m_currentScoreDefDone = true;
}

void Doc::ScoreDefOptimizeDoc()
{
    ScoreDefOptimizeFunctor scoreDefOptimize(this);
    this->Process(scoreDefOptimize);

    this->ScoreDefSetGrpSymDoc();
}

void Doc::ScoreDefSetGrpSymDoc()
{
    // Group symbols need to be resolved using scoreDef, since there might be @startid/@endid attributes that determine
    // their positioning
    ScoreDefSetGrpSymFunctor scoreDefSetGrpSym;
    this->Process(scoreDefSetGrpSym);
}

void Doc::CastOffDoc()
{
    Doc::CastOffDocBase(false, false);
}

void Doc::CastOffLineDoc()
{
    Doc::CastOffDocBase(true, false);
}

void Doc::CastOffSmartDoc()
{
    Doc::CastOffDocBase(false, false, true);
}

void Doc::CastOffDocBase(bool useSb, bool usePb, bool smart)
{
    Pages *pages = this->GetPages();
    assert(pages);

    if (this->IsCastOff()) {
        LogDebug("Document is already cast off");
        return;
    }

    std::list<Score *> scores = this->GetVisibleScores();
    assert(!scores.empty());

    if (m_focusStatus == FOCUS_USED) {
        m_focusStatus = FOCUS_UNSET;
        this->PrepareData();
    }
    this->ScoreDefSetCurrentDoc();

    Page *unCastOffPage = this->SetDrawingPage(0);
    assert(unCastOffPage);

    // Check if the the horizontal layout is cached by looking at the first measure
    // The cache is not set the first time, or can be reset by Doc::UnCastOffDoc
    Measure *firstMeasure = vrv_cast<Measure *>(unCastOffPage->FindDescendantByType(MEASURE));
    if (!firstMeasure || !firstMeasure->HasCachedHorizontalLayout()) {
        // LogDebug("Performing the horizontal layout");
        unCastOffPage->LayOutHorizontally();
        unCastOffPage->LayOutHorizontallyWithCache();
    }
    else {
        unCastOffPage->LayOutHorizontallyWithCache(true);
    }

    Page *castOffSinglePage = new Page();

    System *leftoverSystem = NULL;
    if (useSb && !usePb && !smart) {
        CastOffEncodingFunctor castOffEncoding(this, castOffSinglePage, false);
        unCastOffPage->Process(castOffEncoding);
    }
    else {
        CastOffSystemsFunctor castOffSystems(castOffSinglePage, this, smart);
        castOffSystems.SetSystemWidth(m_drawingPageContentWidth);
        unCastOffPage->Process(castOffSystems);
        leftoverSystem = castOffSystems.GetLeftoverSystem();
    }
    // We can now detach and delete the old content page
    pages->DetachChild(0);
    assert(unCastOffPage && !unCastOffPage->GetParent());
    delete unCastOffPage;
    unCastOffPage = NULL;

    // Store the cast off system widths => these are used to adjust the horizontal spacing
    // for a given duration during page layout
    AlignMeasuresFunctor alignMeasures(this);
    alignMeasures.StoreCastOffSystemWidths(true);
    castOffSinglePage->Process(alignMeasures);

    // Replace it with the castOffSinglePage
    pages->AddChild(castOffSinglePage);
    this->ResetDataPage();
    this->SetDrawingPage(0);

    bool optimize = false;
    for (Score *score : scores) {
        if (score->ScoreDefNeedsOptimization(m_options->m_condense.GetValue())) {
            optimize = true;
            break;
        }
    }

    // Reset the scoreDef at the beginning of each system
    this->ScoreDefSetCurrentDoc(true);
    if (optimize) {
        this->ScoreDefOptimizeDoc();
    }

    // Here we redo the alignment because of the new scoreDefs
    // Because of the new scoreDef, we need to reset cached drawingX
    castOffSinglePage->ResetCachedDrawingX();
    castOffSinglePage->LayOutVertically();

    // Detach the contentPage to prepare for CastOffPages
    pages->DetachChild(0);
    assert(castOffSinglePage && !castOffSinglePage->GetParent());
    this->ResetDataPage();

    for (Score *score : scores) {
        score->CalcRunningElementHeight(this);
    }

    Page *castOffFirstPage = new Page();
    CastOffPagesFunctor castOffPages(castOffSinglePage, this, castOffFirstPage);
    castOffPages.SetPageHeight(m_drawingPageContentHeight);
    castOffPages.SetLeftoverSystem(leftoverSystem);

    pages->AddChild(castOffFirstPage);
    castOffSinglePage->Process(castOffPages);
    delete castOffSinglePage;

    this->ScoreDefSetCurrentDoc(true);
    if (optimize) {
        this->ScoreDefOptimizeDoc();
    }

    m_isCastOff = true;
}

void Doc::UnCastOffDoc(bool resetCache)
{
    if (!this->IsCastOff()) {
        LogDebug("Document is not cast off");
        return;
    }

    this->ResetFocus();

    Pages *pages = this->GetPages();
    assert(pages);

    Page *unCastOffPage = new Page();
    UnCastOffFunctor unCastOff(unCastOffPage);
    unCastOff.SetResetCache(resetCache);
    this->Process(unCastOff);

    pages->ClearChildren();

    pages->AddChild(unCastOffPage);

    // LogDebug("ContinuousLayout: %d pages", this->GetChildCount());

    // We need to reset the drawing page to NULL
    // because idx will still be 0 but contentPage is dead!
    this->ResetDataPage();
    this->ScoreDefSetCurrentDoc(true);

    m_isCastOff = false;
}

void Doc::CastOffEncodingDoc()
{
    if (this->IsCastOff()) {
        LogDebug("Document is already cast off");
        return;
    }

    this->ScoreDefSetCurrentDoc();

    Pages *pages = this->GetPages();
    assert(pages);

    Page *unCastOffPage = this->SetDrawingPage(0);
    assert(unCastOffPage);
    unCastOffPage->ResetAligners();

    // Detach the contentPage
    pages->DetachChild(0);
    assert(unCastOffPage && !unCastOffPage->GetParent());

    Page *castOffFirstPage = new Page();
    pages->AddChild(castOffFirstPage);

    CastOffEncodingFunctor castOffEncoding(this, castOffFirstPage);
    unCastOffPage->Process(castOffEncoding);
    delete unCastOffPage;

    // We need to reset the drawing page to NULL
    // because idx will still be 0 but contentPage is dead!
    this->ResetDataPage();
    this->ScoreDefSetCurrentDoc(true);

    // Optimize the doc if one of the score requires optimization
    for (Score *score : this->GetVisibleScores()) {
        if (score->ScoreDefNeedsOptimization(m_options->m_condense.GetValue())) {
            this->ScoreDefOptimizeDoc();
            break;
        }
    }

    m_isCastOff = true;
}

void Doc::InitSelectionDoc(DocSelection &selection, bool resetCache)
{
    // No new selection to apply;
    if (!selection.m_isPending) return;

    if (this->HasSelection()) {
        this->ResetSelectionDoc(resetCache);
    }

    selection.Set(this);

    if (!this->HasSelection()) return;

    assert(!m_selectionPreceding && !m_selectionFollowing);

    if (this->IsCastOff()) this->UnCastOffDoc();

    Pages *pages = this->GetPages();
    assert(pages);

    this->ScoreDefSetCurrentDoc();

    Page *unCastOffPage = this->SetDrawingPage(0);

    // Make sure we have global slurs curve dir
    unCastOffPage->ResetAligners();

    // We can now detach and delete the old content page
    pages->DetachChild(0);
    assert(unCastOffPage);

    Page *selectionFirstPage = new Page();
    pages->AddChild(selectionFirstPage);

    CastOffToSelectionFunctor castOffToSelection(selectionFirstPage, this, m_selectionStart, m_selectionEnd);
    unCastOffPage->Process(castOffToSelection);

    delete unCastOffPage;

    this->ResetDataPage();
    this->ScoreDefSetCurrentDoc(true);

    if (pages->GetChildCount() < 2) {
        LogWarning("Selection could not be made");
        m_selectionStart = "";
        m_selectionEnd = "";
        return;
    }
    else if (pages->GetChildCount() == 2) {
        LogWarning("Selection end '%s' could not be found", m_selectionEnd.c_str());
        // Add an empty page to make it work
        pages->AddChild(new Page());
    }

    this->ReactivateSelection(true);
}

void Doc::ResetSelectionDoc(bool resetCache)
{
    assert(m_selectionPreceding && m_selectionFollowing);

    m_selectionStart = "";
    m_selectionEnd = "";

    if (this->IsCastOff()) this->UnCastOffDoc();

    this->DeactiveateSelection();

    this->m_isCastOff = true;
    this->UnCastOffDoc(resetCache);
}

bool Doc::HasSelection() const
{
    return (!m_selectionStart.empty() && !m_selectionEnd.empty());
}

void Doc::DeactiveateSelection()
{
    Pages *pages = this->GetPages();
    assert(pages);

    Page *selectionPage = vrv_cast<Page *>(pages->GetChild(0));
    assert(selectionPage);
    // We need to delete the selection scoreDef
    Score *selectionScore = vrv_cast<Score *>(selectionPage->FindDescendantByType(SCORE));
    assert(selectionScore);
    if (selectionScore->GetLabel() != "[selectionScore]") LogError("Deleting wrong score element. Something is wrong");
    selectionPage->DeleteChild(selectionScore);

    pages->InsertChild(m_selectionPreceding, 0);
    pages->AddChild(m_selectionFollowing);

    m_selectionPreceding = NULL;
    m_selectionFollowing = NULL;
}

void Doc::ReactivateSelection(bool resetAligners)
{
    Pages *pages = this->GetPages();
    assert(pages);

    const int lastPage = pages->GetChildCount() - 1;
    assert(lastPage > 1);

    Page *selectionPage = vrv_cast<Page *>(pages->GetChild(1));
    System *system = vrv_cast<System *>(selectionPage->FindDescendantByType(SYSTEM));
    // Add a selection scoreDef based on the current drawing system scoreDef
    Score *selectionScore = new Score();
    selectionScore->GetScoreDef()->ReplaceWithCopyOf(system->GetDrawingScoreDef());
    selectionScore->SetLabel("[selectionScore]");
    // Use the drawing values as actual scoreDef
    selectionScore->GetScoreDef()->ResetFromDrawingValues();
    selectionPage->InsertChild(selectionScore, 0);

    m_selectionPreceding = vrv_cast<Page *>(pages->GetChild(0));
    // Reset the aligners because data will be accessed when rendering control events outside the selection
    if (resetAligners && m_selectionPreceding->FindDescendantByType(MEASURE)) {
        this->SetDrawingPage(0);
        m_selectionPreceding->ResetAligners();
    }

    m_selectionFollowing = vrv_cast<Page *>(pages->GetChild(lastPage));
    // Same for the following content
    if (resetAligners && m_selectionFollowing->FindDescendantByType(MEASURE)) {
        this->SetDrawingPage(2);
        m_selectionFollowing->ResetAligners();
    }

    // Detach the preceding and following page
    pages->DetachChild(lastPage);
    pages->DetachChild(0);
    // Make sure we do not point to page moved out of the selection
    this->m_drawingPage = NULL;
}

void Doc::ConvertToPageBasedDoc()
{
    Pages *pages = new Pages();
    Page *page = new Page();
    pages->AddChild(page);

    ConvertToPageBasedFunctor convertToPageBased(page);
    this->Process(convertToPageBased);

    this->ClearRelinquishedChildren();
    assert(this->GetChildCount() == 0);

    this->AddChild(pages);

    this->ResetDataPage();
}

void Doc::ConvertToCastOffMensuralDoc(bool castOff)
{
    if (!this->IsMensuralMusicOnly()) return;

    // Do not convert transcription files
    if (this->IsTranscription()) return;

    // Do not convert facs files
    if (this->IsFacs()) return;

    // Make sure the document is not cast-off
    if (this->IsCastOff()) this->UnCastOffDoc();

    this->ScoreDefSetCurrentDoc();

    Page *contentPage = this->SetDrawingPage(0);
    assert(contentPage);

    contentPage->LayOutHorizontally();

    ListOfObjects systems = contentPage->FindAllDescendantsByType(SYSTEM, false, 1);
    for (const auto item : systems) {
        System *system = vrv_cast<System *>(item);
        assert(system);
        if (castOff) {
            System *convertedSystem = new System();
            ConvertToCastOffMensuralFunctor convertToCastOffMensural(this, convertedSystem);
            // Convert the system and replace it
            system->Process(convertToCastOffMensural);
            contentPage->ReplaceChild(system, convertedSystem);
            delete system;
        }
        else {
            system->ConvertToUnCastOffMensuralSystem();
        }
    }

    this->PrepareData();

    // We need to reset the drawing page to NULL
    // because idx will still be 0 but contentPage is dead!
    this->ResetDataPage();
    this->ScoreDefSetCurrentDoc(true);
}

void Doc::ConvertToCmnDoc()
{
    if (!this->IsMensuralMusicOnly()) return;

    // Do not convert transcription files
    if (this->IsTranscription()) return;

    // Do not convert facs files
    if (this->IsFacs()) return;

    m_isMensuralMusicOnly = BOOLEAN_false;

    // Temporarily change the equivalence option to minima
    int previousEquivalence = m_options->m_durationEquivalence.GetValue();
    m_options->m_durationEquivalence.SetValue(DURATION_EQ_minima);

    // Make sure the document is not cast-off
    if (this->IsCastOff()) this->UnCastOffDoc();

    this->ScoreDefSetCurrentDoc();

    this->CalculateTimemap();

    Page *contentPage = this->SetDrawingPage(0);
    assert(contentPage);

    contentPage->LayOutHorizontally();

    ListOfObjects systems = contentPage->FindAllDescendantsByType(SYSTEM, false, 1);
    ListOfObjects scores = contentPage->FindAllDescendantsByType(SCORE, false, 1);
    assert(systems.size() == scores.size());

    ListOfObjects::iterator systemsIt = systems.begin();
    ListOfObjects::iterator scoresIt = scores.begin();
    for (; systemsIt != systems.end(); ++systemsIt, ++scoresIt) {
        System *system = vrv_cast<System *>(*systemsIt);
        assert(system);
        Score *score = vrv_cast<Score *>(*scoresIt);
        assert(score);
        System *convertedSystem = new System();
        ConvertToCmnFunctor convertToCmn(this, convertedSystem, score);
        // Convert the system and replace it
        system->Process(convertToCmn);
        contentPage->ReplaceChild(system, convertedSystem);
        delete system;
    }

    this->GenerateMeasureNumbers();

    this->PrepareData();

    // We need to reset the drawing page to NULL
    // because idx will still be 0 but contentPage is dead!
    this->ResetDataPage();
    this->ScoreDefSetCurrentDoc(true);

    // Reset the option
    m_options->m_durationEquivalence.SetValue(previousEquivalence);
}

void Doc::ConvertMarkupDoc(bool permanent)
{
    if (m_markup == MARKUP_DEFAULT) return;

    LogInfo("Converting markup...");

    if (m_markup & MARKUP_GRACE_ATTRIBUTE) {
    }

    if (m_markup & MARKUP_ARTIC_MULTIVAL) {
        LogInfo("Converting artic markup...");
        ConvertMarkupArticFunctor convertMarkupArtic;
        this->Process(convertMarkupArtic);
    }

    if ((m_markup & MARKUP_ANALYTICAL_FERMATA) || (m_markup & MARKUP_ANALYTICAL_TIE)) {
        LogInfo("Converting analytical markup...");
        /************ Prepare processing by staff/layer/verse ************/

        // We need to populate processing lists for processing the document by Layer (for matching @tie) and
        // by Verse (for matching syllable connectors)
        InitProcessingListsFunctor initProcessingLists;

        // We first fill a tree of ints with [staff/layer] and [staff/layer/verse] numbers (@n) to be processed
        this->Process(initProcessingLists);
        const IntTree &layerTree = initProcessingLists.GetLayerTree();

        /************ Resolve ties ************/

        // Process by layer for matching @tie attribute - we process notes and chords, looking at
        // GetTie values and pitch and oct for matching notes
        Filters filters;
        for (auto &staves : layerTree.child) {
            for (auto &layers : staves.second.child) {
                filters.Clear();
                // Create ad comparison object for each type / @n
                AttNIntegerComparison matchStaff(STAFF, staves.first);
                AttNIntegerComparison matchLayer(LAYER, layers.first);
                filters.Add(&matchStaff);
                filters.Add(&matchLayer);

                ConvertMarkupAnalyticalFunctor convertMarkupAnalytical(permanent);
                convertMarkupAnalytical.SetFilters(&filters);
                this->Process(convertMarkupAnalytical);

                // After having processed one layer, we check if we have open ties - if yes, we
                // must reset them and they will be ignored.
                for (Note *note : convertMarkupAnalytical.GetCurrentNotes()) {
                    LogWarning("Unable to match @tie of note '%s', skipping it", note->GetID().c_str());
                }
            }
        }
    }

    if (m_markup & MARKUP_SCOREDEF_DEFINITIONS) {
        LogInfo("Converting scoreDef markup...");
        ConvertMarkupScoreDefFunctor convertMarkupScoreDef(this);
        this->Process(convertMarkupScoreDef);
    }
}

void Doc::ScoringUpDoc()
{
    ScoringUpFunctor scoringUp;
    this->Process(scoringUp);
}

void Doc::ConvertToMensuralViewDoc()
{
    if (this->IsCastOff()) {
        LogDebug("Document is cast off");
        return;
    }

    ConvertToMensuralViewFunctor convertToMensuralView(this);
    this->Process(convertToMensuralView);
}

void Doc::ConvertMensuralToCmnDoc()
{
    if (this->IsCastOff()) {
        LogDebug("Document is cast off");
        return;
    }
}

void Doc::SyncFromFacsimileDoc()
{
    PrepareFacsimileFunctor prepareFacsimile(this->GetFacsimile());
    this->Process(prepareFacsimile);

    SyncFromFacsimileFunctor syncFromFacsimileFunctor(this);
    this->Process(syncFromFacsimileFunctor);
}

void Doc::SyncToFacsimileDoc()
{
    double ppuFactor = 1.0;
    // Create a new facsimile object if we do not have one already
    if (!this->HasFacsimile()) {
        Facsimile *facsimile = new Facsimile();
        this->SetFacsimile(facsimile);
        m_facsimile->SetType("transcription");
        // We use the scale option to determine the ppu and adjust the ppu factor accordingly
        // For example, with scale 50 and the default unit, the ppu will be 4.5 (instead of 9.0)
        ppuFactor = (double)m_options->m_scale.GetValue() / 100.0;
    }

    this->ScoreDefSetCurrentDoc();

    SyncToFacsimileFunctor syncToFacimileFunctor(this, ppuFactor);
    this->Process(syncToFacimileFunctor);
}

void Doc::TransposeDoc()
{
    Transposer transposer;
    transposer.SetBase600(); // Set extended chromatic alteration mode (allowing more than double sharps/flats)

    const bool selectedOnly = m_options->m_transposeSelectedOnly.GetValue();

    if (m_options->m_transpose.IsSet()) {
        // Transpose the entire document
        if (m_options->m_transposeMdiv.IsSet()) {
            LogWarning("\"%s\" is ignored when \"%s\" is set as well. Please use only one of the two options.",
                m_options->m_transposeMdiv.GetKey().c_str(), m_options->m_transpose.GetKey().c_str());
        }
        TransposeFunctor transpose(this, &transposer);
        transpose.SetVisibleOnly(selectedOnly);
        transpose.SetTransposition(m_options->m_transpose.GetValue());
        this->Process(transpose);
    }
    else if (m_options->m_transposeMdiv.IsSet()) {
        // Transpose mdivs individually
        std::set<std::string> ids = m_options->m_transposeMdiv.GetKeys();
        for (const std::string &id : ids) {
            TransposeSelectedMdivFunctor transposeSelectedMdiv(this, &transposer);
            transposeSelectedMdiv.SetVisibleOnly(selectedOnly);
            transposeSelectedMdiv.SetSelectedMdivID(id);
            transposeSelectedMdiv.SetTransposition(m_options->m_transposeMdiv.GetStrValue({ id }));
            this->Process(transposeSelectedMdiv);
        }
    }

    if (m_options->m_transposeToSoundingPitch.GetValue()) {
        // Transpose to sounding pitch
        TransposeToSoundingPitchFunctor transposeToSoundingPitch(this, &transposer);
        transposeToSoundingPitch.SetVisibleOnly(selectedOnly);
        this->Process(transposeToSoundingPitch);
    }
}

void Doc::ExpandExpansions()
{
    // Upon MEI import: use expansion ID, given by command line argument
    std::string expansionId = this->GetOptions()->m_expand.GetValue();
    if (expansionId.empty()) return;

    Expansion *start = dynamic_cast<Expansion *>(this->FindDescendantByID(expansionId));
    if (start == NULL) {
        LogWarning("Expansion ID '%s' not found. Nothing expanded.", expansionId.c_str());
        return;
    }

    xsdAnyURI_List expansionList = start->GetPlist();
    xsdAnyURI_List existingList;
    m_expansionMap.Expand(expansionList, existingList, start);

    // save original/notated expansion as element in expanded MEI
    // Expansion *originalExpansion = new Expansion();
    // char rnd[35];
    // snprintf(rnd, 35, "expansion-notated-%016d", std::rand());
    // originalExpansion->SetID(rnd);

    // for (std::string ref : existingList) {
    //    originalExpansion->GetPlistInterface()->AddRef("#" + ref);
    //}

    // start->GetParent()->InsertAfter(start, originalExpansion);

    // std::cout << "[expand] original expansion xml:id=\"" << originalExpansion->GetID().c_str()
    //          << "\" plist={";
    // for (std::string s : existingList) std::cout << s.c_str() << ((s != existingList.back()) ? " " : "}.\n");
}

bool Doc::HasPage(int pageIdx) const
{
    const Pages *pages = this->GetPages();
    assert(pages);
    return ((pageIdx >= 0) && (pageIdx < pages->GetChildCount()));
}

Pages *Doc::GetPages()
{
    return vrv_cast<Pages *>(this->FindDescendantByType(PAGES));
}

const Pages *Doc::GetPages() const
{
    return vrv_cast<const Pages *>(this->FindDescendantByType(PAGES));
}

int Doc::GetPageCount() const
{
    const Pages *pages = this->GetPages();
    return ((pages) ? pages->GetChildCount() : 0);
}

ScoreDef *Doc::GetFirstScoreDef()
{
    return const_cast<ScoreDef *>(std::as_const(*this).GetFirstScoreDef());
}

const ScoreDef *Doc::GetFirstScoreDef() const
{
    const Score *score = vrv_cast<const Score *>(this->FindDescendantByType(SCORE, 3));

    return score ? score->GetScoreDef() : NULL;
}

std::list<Score *> Doc::GetVisibleScores()
{
    if (m_visibleScores.empty()) {
        this->CollectVisibleScores();

        assert(!m_visibleScores.empty());
    }

    return m_visibleScores;
}

Score *Doc::GetFirstVisibleScore()
{
    if (m_visibleScores.empty()) {
        this->CollectVisibleScores();

        assert(!m_visibleScores.empty());
    }

    return m_visibleScores.front();
}

Score *Doc::GetCorrespondingScore(const Object *object)
{
    return const_cast<Score *>(std::as_const(*this).GetCorrespondingScore(object));
}

const Score *Doc::GetCorrespondingScore(const Object *object) const
{
    return this->GetCorrespondingScore(object, m_visibleScores);
}

Score *Doc::GetCorrespondingScore(const Object *object, const std::list<Score *> &scores)
{
    return const_cast<Score *>(std::as_const(*this).GetCorrespondingScore(object, scores));
}

const Score *Doc::GetCorrespondingScore(const Object *object, const std::list<Score *> &scores) const
{
    assert(!scores.empty());

    const Score *correspondingScore = scores.front();
    for (Score *score : scores) {
        if ((score == object) || Object::IsPreOrdered(score, object)) {
            correspondingScore = score;
        }
    }
    return correspondingScore;
}

void Doc::CollectVisibleScores()
{
    m_visibleScores.clear();
    ListOfObjects objects = this->FindAllDescendantsByType(SCORE, false, 3);
    for (Object *object : objects) {
        Score *score = vrv_cast<Score *>(object);
        assert(score);
        // Visible scores have milestone end
        if (score->GetMilestoneEnd()) {
            m_visibleScores.push_back(score);
        }
    }
}

void Doc::RefreshLayout()
{
    if (m_focusStatus != FOCUS_UNSET) {
        m_focusRange->LayOutAll();
    }
    else {
        this->GetPages()->LayOutAll();
    }
}

void Doc::SetFocus()
{
    // Focus has already been set
    if (m_focusStatus != FOCUS_UNSET) return;

    if (!m_focusRange) {
        m_focusRange = new PageRange(this);
    }
    m_focusRange->Reset();
    m_focusRange->SetAsFocus(m_drawingPage);
    m_focusStatus = FOCUS_SET;

    this->PrepareData();
    this->ScoreDefSetCurrentDoc(true);
    this->RefreshLayout();
}

void Doc::ResetFocus()
{
    if (m_focusStatus == FOCUS_UNSET) return;

    m_focusRange->ClearChildren();
    m_focusStatus = FOCUS_UNSET;
    this->PrepareData();
    this->ScoreDefSetCurrentDoc(true);
}

int Doc::GetGlyphHeight(char32_t code, int staffSize, bool graceSize) const
{
    int x, y, w, h;
    const Resources &resources = this->GetResources();
    const Glyph *glyph = resources.GetGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    h = h * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    if (graceSize) h = h * m_options->m_graceFactor.GetValue();
    h = h * staffSize / 100;
    return h;
}

int Doc::GetGlyphWidth(char32_t code, int staffSize, bool graceSize) const
{
    int x, y, w, h;
    const Resources &resources = this->GetResources();
    const Glyph *glyph = resources.GetGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    w = w * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    if (graceSize) w = w * m_options->m_graceFactor.GetValue();
    w = w * staffSize / 100;
    return w;
}

int Doc::GetGlyphAdvX(char32_t code, int staffSize, bool graceSize) const
{
    const Resources &resources = this->GetResources();
    const Glyph *glyph = resources.GetGlyph(code);
    assert(glyph);
    int advX = glyph->GetHorizAdvX();
    advX = advX * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    if (graceSize) advX = advX * m_options->m_graceFactor.GetValue();
    advX = advX * staffSize / 100;
    return advX;
}

Point Doc::ConvertFontPoint(const Glyph *glyph, const Point &fontPoint, int staffSize, bool graceSize) const
{
    assert(glyph);

    Point point;
    point.x = fontPoint.x * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    point.y = fontPoint.y * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    if (graceSize) {
        point.x = point.x * m_options->m_graceFactor.GetValue();
        point.y = point.y * m_options->m_graceFactor.GetValue();
    }
    if (staffSize != 100) {
        point.x = point.x * staffSize / 100;
        point.y = point.y * staffSize / 100;
    }
    return point;
}

int Doc::GetGlyphLeft(char32_t code, int staffSize, bool graceSize) const
{
    int x, y, w, h;
    const Resources &resources = this->GetResources();
    const Glyph *glyph = resources.GetGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    x = x * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    if (graceSize) x = x * m_options->m_graceFactor.GetValue();
    x = x * staffSize / 100;
    return x;
}

int Doc::GetGlyphRight(char32_t code, int staffSize, bool graceSize) const
{
    return this->GetGlyphLeft(code, staffSize, graceSize) + this->GetGlyphWidth(code, staffSize, graceSize);
}

int Doc::GetGlyphBottom(char32_t code, int staffSize, bool graceSize) const
{
    int x, y, w, h;
    const Resources &resources = this->GetResources();
    const Glyph *glyph = resources.GetGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    y = y * m_drawingSmuflFontSize / glyph->GetUnitsPerEm();
    if (graceSize) y = y * m_options->m_graceFactor.GetValue();
    y = y * staffSize / 100;
    return y;
}

int Doc::GetGlyphTop(char32_t code, int staffSize, bool graceSize) const
{
    return this->GetGlyphBottom(code, staffSize, graceSize) + this->GetGlyphHeight(code, staffSize, graceSize);
}

int Doc::GetTextGlyphHeight(char32_t code, const FontInfo *font, bool graceSize) const
{
    assert(font);

    int x, y, w, h;
    const Resources &resources = this->GetResources();
    const Glyph *glyph = resources.GetTextGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    h = h * font->GetPointSize() / glyph->GetUnitsPerEm();
    if (graceSize) h = h * m_options->m_graceFactor.GetValue();
    return h;
}

int Doc::GetTextGlyphWidth(char32_t code, const FontInfo *font, bool graceSize) const
{
    assert(font);

    int x, y, w, h;
    const Resources &resources = this->GetResources();
    const Glyph *glyph = resources.GetTextGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    w = w * font->GetPointSize() / glyph->GetUnitsPerEm();
    if (graceSize) w = w * m_options->m_graceFactor.GetValue();
    return w;
}

int Doc::GetTextGlyphAdvX(char32_t code, const FontInfo *font, bool graceSize) const
{
    assert(font);

    const Resources &resources = this->GetResources();
    const Glyph *glyph = resources.GetTextGlyph(code);
    assert(glyph);
    int advX = glyph->GetHorizAdvX();
    advX = advX * font->GetPointSize() / glyph->GetUnitsPerEm();
    if (graceSize) advX = advX * m_options->m_graceFactor.GetValue();
    return advX;
}

int Doc::GetTextGlyphDescender(char32_t code, const FontInfo *font, bool graceSize) const
{
    assert(font);

    int x, y, w, h;
    const Resources &resources = this->GetResources();
    const Glyph *glyph = resources.GetTextGlyph(code);
    assert(glyph);
    glyph->GetBoundingBox(x, y, w, h);
    y = y * font->GetPointSize() / glyph->GetUnitsPerEm();
    if (graceSize) y = y * m_options->m_graceFactor.GetValue();
    return y;
}

int Doc::GetTextLineHeight(const FontInfo *font, bool graceSize) const
{
    int descender = -this->GetTextGlyphDescender(L'q', font, graceSize);
    int height = this->GetTextGlyphHeight(L'I', font, graceSize);

    int lineHeight = ((descender + height) * 1.1);
    if (font->GetSupSubScript()) lineHeight /= SUPER_SCRIPT_FACTOR;

    return lineHeight;
}

int Doc::GetTextXHeight(const FontInfo *font, bool graceSize) const
{
    return this->GetTextGlyphHeight('x', font, graceSize);
}

int Doc::GetDrawingUnit(int staffSize) const
{
    return m_options->m_unit.GetValue() * staffSize / 100;
}

int Doc::GetDrawingDoubleUnit(int staffSize) const
{
    return m_options->m_unit.GetValue() * 2 * staffSize / 100;
}

int Doc::GetDrawingStaffSize(int staffSize) const
{
    return m_options->m_unit.GetValue() * 8 * staffSize / 100;
}

int Doc::GetDrawingOctaveSize(int staffSize) const
{
    return m_options->m_unit.GetValue() * 7 * staffSize / 100;
}

int Doc::GetDrawingBrevisWidth(int staffSize) const
{
    return m_drawingBrevisWidth * staffSize / 100;
}

int Doc::GetDrawingBarLineWidth(int staffSize) const
{
    return m_options->m_barLineWidth.GetValue() * this->GetDrawingUnit(staffSize);
}

int Doc::GetDrawingStaffLineWidth(int staffSize) const
{
    return m_options->m_staffLineWidth.GetValue() * this->GetDrawingUnit(staffSize);
}

int Doc::GetDrawingStemWidth(int staffSize) const
{
    return m_options->m_stemWidth.GetValue() * this->GetDrawingUnit(staffSize);
}

int Doc::GetDrawingDynamHeight(int staffSize, bool withMargin) const
{
    int height = this->GetGlyphHeight(SMUFL_E522_dynamicForte, staffSize, false);
    // This should be styled
    if (withMargin) height += this->GetDrawingUnit(staffSize);
    return height;
}

int Doc::GetDrawingHairpinSize(int staffSize, bool withMargin) const
{
    int size = m_options->m_hairpinSize.GetValue() * this->GetDrawingUnit(staffSize);
    // This should be styled
    if (withMargin) size += this->GetDrawingUnit(staffSize);
    return size;
}

int Doc::GetDrawingBeamWidth(int staffSize, bool graceSize) const
{
    int value = m_drawingBeamWidth * staffSize / 100;
    if (graceSize) value = value * m_options->m_graceFactor.GetValue();
    return value;
}

int Doc::GetDrawingBeamWhiteWidth(int staffSize, bool graceSize) const
{
    int value = m_drawingBeamWhiteWidth * staffSize / 100;
    if (graceSize) value = value * m_options->m_graceFactor.GetValue();
    return value;
}

int Doc::GetDrawingLedgerLineExtension(int staffSize, bool graceSize) const
{
    int value = m_options->m_ledgerLineExtension.GetValue() * this->GetDrawingUnit(staffSize);
    if (graceSize) value = this->GetCueSize(value);
    return value;
}

int Doc::GetDrawingMinimalLedgerLineExtension(int staffSize, bool graceSize) const
{
    int value = m_options->m_ledgerLineExtension.GetMin() * this->GetDrawingUnit(staffSize);
    if (graceSize) value = this->GetCueSize(value);
    return value;
}

int Doc::GetCueSize(int value) const
{
    return value * this->GetCueScaling();
}

double Doc::GetCueScaling() const
{
    return m_options->m_graceFactor.GetValue();
}

FontInfo *Doc::GetDrawingSmuflFont(int staffSize, bool graceSize)
{
    m_drawingSmuflFont.SetFaceName(this->GetResources().GetCurrentFont().c_str());
    int value = m_drawingSmuflFontSize * staffSize / 100;
    if (graceSize) value = value * m_options->m_graceFactor.GetValue();
    m_drawingSmuflFont.SetPointSize(value);
    return &m_drawingSmuflFont;
}

FontInfo *Doc::GetDrawingLyricFont(int staffSize)
{
    m_drawingLyricFont.SetPointSize(m_drawingLyricFontSize * staffSize / 100);
    return &m_drawingLyricFont;
}

FontInfo *Doc::GetFingeringFont(int staffSize)
{
    m_fingeringFont.SetPointSize(m_fingeringFontSize * staffSize / 100);
    return &m_fingeringFont;
}

double Doc::GetMusicToLyricFontSizeRatio() const
{
    return (m_drawingLyricFontSize == 0.0) ? 1.0 : (double)m_drawingSmuflFontSize / (double)m_drawingLyricFontSize;
}

double Doc::GetLeftMargin(const ClassId classId) const
{
    if (classId == ACCID) return m_options->m_leftMarginAccid.GetValue();
    if (classId == BARLINE) return m_options->m_leftMarginBarLine.GetValue();
    if (classId == BEATRPT) return m_options->m_leftMarginBeatRpt.GetValue();
    if (classId == CHORD) return m_options->m_leftMarginChord.GetValue();
    if (classId == CLEF) return m_options->m_leftMarginClef.GetValue();
    if (classId == KEYSIG) return m_options->m_leftMarginKeySig.GetValue();
    if (classId == MENSUR) return m_options->m_leftMarginMensur.GetValue();
    if (classId == METERSIG) return m_options->m_leftMarginMeterSig.GetValue();
    if (classId == MREST) return m_options->m_leftMarginMRest.GetValue();
    if (classId == MRPT2) return m_options->m_leftMarginMRpt2.GetValue();
    if (classId == MULTIREST) return m_options->m_leftMarginMultiRest.GetValue();
    if (classId == MULTIRPT) return m_options->m_leftMarginMultiRpt.GetValue();
    if (classId == NOTE) return m_options->m_leftMarginNote.GetValue();
    if (classId == STEM) return m_options->m_leftMarginNote.GetValue();
    if (classId == REST) return m_options->m_leftMarginRest.GetValue();
    if (classId == TABDURSYM) return m_options->m_leftMarginTabDurSym.GetValue();
    return m_options->m_defaultLeftMargin.GetValue();
}

double Doc::GetLeftMargin(const Object *object) const
{
    assert(object);
    const ClassId id = object->GetClassId();
    if (id == BARLINE) {
        const BarLine *barLine = vrv_cast<const BarLine *>(object);
        switch (barLine->GetPosition()) {
            case BarLinePosition::None: return m_options->m_leftMarginBarLine.GetValue();
            case BarLinePosition::Left: return m_options->m_leftMarginLeftBarLine.GetValue();
            case BarLinePosition::Right: return m_options->m_leftMarginRightBarLine.GetValue();
            default: break;
        }
    }
    return this->GetLeftMargin(id);
}

double Doc::GetRightMargin(const ClassId classId) const
{
    if (classId == ACCID) return m_options->m_rightMarginAccid.GetValue();
    if (classId == BARLINE) return m_options->m_rightMarginBarLine.GetValue();
    if (classId == BEATRPT) return m_options->m_rightMarginBeatRpt.GetValue();
    if (classId == CHORD) return m_options->m_rightMarginChord.GetValue();
    if (classId == CLEF) return m_options->m_rightMarginClef.GetValue();
    if (classId == KEYSIG) return m_options->m_rightMarginKeySig.GetValue();
    if (classId == MENSUR) return m_options->m_rightMarginMensur.GetValue();
    if (classId == METERSIG) return m_options->m_rightMarginMeterSig.GetValue();
    if (classId == MREST) return m_options->m_rightMarginMRest.GetValue();
    if (classId == MRPT2) return m_options->m_rightMarginMRpt2.GetValue();
    if (classId == MULTIREST) return m_options->m_rightMarginMultiRest.GetValue();
    if (classId == MULTIRPT) return m_options->m_rightMarginMultiRpt.GetValue();
    if (classId == NOTE) return m_options->m_rightMarginNote.GetValue();
    if (classId == STEM) return m_options->m_rightMarginNote.GetValue();
    if (classId == REST) return m_options->m_rightMarginRest.GetValue();
    if (classId == TABDURSYM) return m_options->m_rightMarginTabDurSym.GetValue();
    return m_options->m_defaultRightMargin.GetValue();
}

double Doc::GetRightMargin(const Object *object) const
{
    assert(object);
    const ClassId id = object->GetClassId();
    if (id == BARLINE) {
        const BarLine *barLine = vrv_cast<const BarLine *>(object);
        switch (barLine->GetPosition()) {
            case BarLinePosition::None: return m_options->m_rightMarginBarLine.GetValue();
            case BarLinePosition::Left: return m_options->m_rightMarginLeftBarLine.GetValue();
            case BarLinePosition::Right: return m_options->m_rightMarginRightBarLine.GetValue();
            default: break;
        }
    }
    return this->GetRightMargin(id);
}

double Doc::GetBottomMargin(const ClassId classId) const
{
    if (classId == ARTIC) return m_options->m_bottomMarginArtic.GetValue();
    if (classId == HARM) return m_options->m_bottomMarginHarm.GetValue();
    if (classId == OCTAVE) return m_options->m_bottomMarginOctave.GetValue();
    return m_options->m_defaultBottomMargin.GetValue();
}

double Doc::GetTopMargin(const ClassId classId) const
{
    if (classId == ARTIC) return m_options->m_topMarginArtic.GetValue();
    if (classId == HARM) return m_options->m_topMarginHarm.GetValue();
    return m_options->m_defaultTopMargin.GetValue();
}

data_MEASUREMENTSIGNED Doc::GetStaffDistance(const Object *object, int staffIndex, data_STAFFREL staffPosition) const
{
    const ScoreDef *scoreDef = this->GetCorrespondingScore(object)->GetScoreDef();

    data_MEASUREMENTSIGNED distance;
    if (staffPosition == STAFFREL_above || staffPosition == STAFFREL_below) {
        if (object->Is(DIR)) {
            // Inspect the scoreDef attribute
            if (scoreDef->HasDirDist()) {
                distance = scoreDef->GetDirDist();
            }

            // Inspect the staffDef attributes
            const StaffDef *staffDef = scoreDef->GetStaffDef(staffIndex);
            if (staffDef != NULL && staffDef->HasDirDist()) {
                distance = staffDef->GetDirDist();
            }
        }
        else if (object->Is(DYNAM)) {
            distance.SetVu(m_options->m_dynamDist.GetDefault());

            // Inspect the scoreDef attribute
            if (scoreDef->HasDynamDist()) {
                distance = scoreDef->GetDynamDist();
            }

            // Inspect the staffDef attributes
            const StaffDef *staffDef = scoreDef->GetStaffDef(staffIndex);
            if (staffDef != NULL && staffDef->HasDynamDist()) {
                distance = staffDef->GetDynamDist();
            }

            // Apply CLI option if set
            if (m_options->m_dynamDist.IsSet()) {
                distance.SetVu(m_options->m_dynamDist.GetValue());
            }
        }
        else if (object->Is(HARM)) {
            distance.SetVu(m_options->m_harmDist.GetDefault());

            // Inspect the scoreDef attribute
            if (scoreDef->HasHarmDist()) {
                distance = scoreDef->GetHarmDist();
            }

            // Inspect the staffDef attributes
            const StaffDef *staffDef = scoreDef->GetStaffDef(staffIndex);
            if (staffDef != NULL && staffDef->HasHarmDist()) {
                distance = staffDef->GetHarmDist();
            }

            // Apply CLI option if set
            if (m_options->m_harmDist.IsSet()) {
                distance.SetVu(m_options->m_harmDist.GetValue());
            }
        }
        else if (object->Is(TEMPO)) {
            // Inspect the scoreDef attribute
            if (scoreDef->HasTempoDist()) {
                distance = scoreDef->GetTempoDist();
            }

            // Inspect the staffDef attributes
            const StaffDef *staffDef = scoreDef->GetStaffDef(staffIndex);
            if (staffDef != NULL && staffDef->HasTempoDist()) {
                distance = staffDef->GetTempoDist();
            }
        }
    }
    return distance;
}

Page *Doc::SetDrawingPage(int pageIdx, bool withPageRange)
{
    // out of range
    if (!HasPage(pageIdx)) {
        return NULL;
    }
    // nothing to do
    if (m_drawingPage && m_drawingPage->GetIdx() == pageIdx) {
        return m_drawingPage;
    }
    Pages *pages = this->GetPages();
    assert(pages);
    m_drawingPage = vrv_cast<Page *>(pages->GetChild(pageIdx));
    assert(m_drawingPage);

    this->ResetFocus();

    this->UpdatePageDrawingSizes();

    // Layout pages in the corresponding range
    if (withPageRange) {
        PageRange pageRange(this);
        pageRange.SetAsFocus(m_drawingPage);
        pageRange.LayOutAll();
    }

    return m_drawingPage;
}

void Doc::UpdatePageDrawingSizes()
{
    assert(m_drawingPage);

    int glyph_size;

    // we use the page members only if set (!= -1)
    if (m_drawingPage->m_pageHeight != -1) {
        m_drawingPageHeight = m_drawingPage->m_pageHeight;
        m_drawingPageWidth = m_drawingPage->m_pageWidth;
        m_drawingPageMarginBottom = m_drawingPage->m_pageMarginBottom;
        m_drawingPageMarginLeft = m_drawingPage->m_pageMarginLeft;
        m_drawingPageMarginRight = m_drawingPage->m_pageMarginRight;
        m_drawingPageMarginTop = m_drawingPage->m_pageMarginTop;
    }
    else if (m_pageHeight != -1) {
        m_drawingPageHeight = m_pageHeight;
        m_drawingPageWidth = m_pageWidth;
        m_drawingPageMarginBottom = m_pageMarginBottom;
        m_drawingPageMarginLeft = m_pageMarginLeft;
        m_drawingPageMarginRight = m_pageMarginRight;
        m_drawingPageMarginTop = m_pageMarginTop;
    }
    else {
        m_drawingPageHeight = m_options->m_pageHeight.GetValue();
        m_drawingPageWidth = m_options->m_pageWidth.GetValue();
        m_drawingPageMarginBottom = m_options->m_pageMarginBottom.GetValue();
        m_drawingPageMarginLeft = m_options->m_pageMarginLeft.GetValue();
        m_drawingPageMarginRight = m_options->m_pageMarginRight.GetValue();
        m_drawingPageMarginTop = m_options->m_pageMarginTop.GetValue();

        if (m_options->m_scaleToPageSize.GetValue()) {
            m_drawingPageHeight = m_drawingPageHeight * 100 / m_options->m_scale.GetValue();
            m_drawingPageWidth = m_drawingPageWidth * 100 / m_options->m_scale.GetValue();
            // Margins do remain the same
        }
    }

    if (m_options->m_landscape.GetValue()) {
        int pageHeight = m_drawingPageWidth;
        m_drawingPageWidth = m_drawingPageHeight;
        m_drawingPageHeight = pageHeight;
        int pageMarginRight = m_drawingPageMarginLeft;
        m_drawingPageMarginLeft = m_drawingPageMarginRight;
        m_drawingPageMarginRight = pageMarginRight;
    }

    m_drawingPageContentHeight = m_drawingPageHeight - m_drawingPageMarginTop - m_drawingPageMarginBottom;
    m_drawingPageContentWidth = m_drawingPageWidth - m_drawingPageMarginLeft - m_drawingPageMarginRight;

    // From here we could check if values have changed
    // Since m_options->m_interlDefin stays the same, it's useless to do it
    // every time for now.

    m_drawingBeamMaxSlope = m_options->m_beamMaxSlope.GetValue();
    m_drawingBeamMaxSlope /= 100;

    // values for beams
    m_drawingBeamWidth = m_options->m_unit.GetValue();
    m_drawingBeamWhiteWidth = m_options->m_unit.GetValue() / 2;

    // values for fonts
    m_drawingSmuflFontSize = this->CalcMusicFontSize();
    m_drawingLyricFontSize = m_options->m_unit.GetValue() * m_options->m_lyricSize.GetValue();
    m_fingeringFontSize = m_drawingLyricFontSize * m_options->m_fingeringScale.GetValue();

    glyph_size = this->GetGlyphWidth(SMUFL_E0A2_noteheadWhole, 100, 0);

    m_drawingBrevisWidth = (int)((glyph_size * 0.8) / 2);
}

bool Doc::CheckPageSize(const Page *page) const
{
    assert(page);
    assert(m_drawingPage);

    if (page == m_drawingPage) return true;

    return ((page->m_pageHeight == -1) && (m_drawingPage->m_pageHeight == -1));
}

int Doc::CalcMusicFontSize()
{
    return m_options->m_unit.GetValue() * 8;
}

int Doc::GetAdjustedDrawingPageHeight() const
{
    assert(m_drawingPage);

    // Take into account the PPU when getting the page height in facsimile
    if (this->IsTranscription() || this->IsFacs()) {
        return m_drawingPage->m_pageHeight * m_drawingPage->GetPPUFactor() / DEFINITION_FACTOR;
    }

    int contentHeight = m_drawingPage->GetContentHeight();
    if (m_options->m_scaleToPageSize.GetValue()) {
        contentHeight = contentHeight * m_options->m_scale.GetValue() / 100;
    }
    return (contentHeight + m_drawingPageMarginTop + m_drawingPageMarginBottom) / DEFINITION_FACTOR;
}

int Doc::GetAdjustedDrawingPageWidth() const
{
    assert(m_drawingPage);

    // Take into account the PPU when getting the page width in facsimile
    if (this->IsTranscription() || this->IsFacs()) {
        return m_drawingPage->m_pageWidth * m_drawingPage->GetPPUFactor() / DEFINITION_FACTOR;
    }

    int contentWidth = m_drawingPage->GetContentWidth();
    if (m_options->m_scaleToPageSize.GetValue()) {
        contentWidth = contentWidth * m_options->m_scale.GetValue() / 100;
    }
    return (contentWidth + m_drawingPageMarginLeft + m_drawingPageMarginRight) / DEFINITION_FACTOR;
}

void Doc::SetMensuralMusicOnly(data_BOOLEAN isMensuralMusicOnly)
{
    // Already marked as non mensural only cannoy be set back
    if (m_isMensuralMusicOnly != BOOLEAN_false) {
        m_isMensuralMusicOnly = isMensuralMusicOnly;
    }
}

//----------------------------------------------------------------------------
// Doc functors methods
//----------------------------------------------------------------------------

FunctorCode Doc::Accept(Functor &functor)
{
    return functor.VisitDoc(this);
}

FunctorCode Doc::Accept(ConstFunctor &functor) const
{
    return functor.VisitDoc(this);
}

FunctorCode Doc::AcceptEnd(Functor &functor)
{
    return functor.VisitDocEnd(this);
}

FunctorCode Doc::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitDocEnd(this);
}

} // namespace vrv
