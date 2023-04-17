/////////////////////////////////////////////////////////////////////////////
// Name:        functorparams.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FUNCTOR_PARAMS_H__
#define __VRV_FUNCTOR_PARAMS_H__

//----------------------------------------------------------------------------

#include "doc.h"
#include "vrvdef.h"

namespace smf {
class MidiFile;
}

namespace vrv {

class Artic;
class SystemMilestoneInterface;
class Chord;
class ClassIdComparison;
class Clef;
class Doc;
class Dot;
class Dots;
class Dynam;
class Ending;
class Output;
class Facsimile;
class FeatureExtractor;
class Functor;
class Hairpin;
class Harm;
class KeySig;
class LabelAbbr;
class Layer;
class LayerElement;
class Measure;
class MeasureAligner;
class Mensur;
class MeterSig;
class MeterSigGrp;
class MRpt;
class Object;
class Page;
class Pedal;
class ScoreDef;
class ScoreDefElement;
class Slur;
class Staff;
class StaffAlignment;
class StaffDef;
class StemmedDrawingInterface;
class Syl;
class SymbolTable;
class System;
class SystemAligner;
class Timemap;
class Transposer;
class TupletNum;
class Turn;
class Verse;

//----------------------------------------------------------------------------
// FunctorParams
//----------------------------------------------------------------------------

/**
 * This is the base class for all MEI att classes.
 * It is not an abstract class but it should not be instanciated directly.
 * The att classes are generated with the libmei parser for Verovio.
 */

class FunctorParams {
public:
    FunctorParams() {}
    virtual ~FunctorParams() {}
};

//----------------------------------------------------------------------------
// Child classes of FunctorParams
//----------------------------------------------------------------------------

/**
 * This is a basic FunctorParams with only the doc pointer for cases where
 * it is the only parameter needed.
 * member 0: the Doc
 **/

class FunctorDocParams : public FunctorParams {
public:
    FunctorDocParams(Doc *doc) { m_doc = doc; }
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AddLayerElementToFlatListParams
//----------------------------------------------------------------------------

/**
 * member 0: the ArrayOfObjects
 **/

class AddLayerElementToFlatListParams : public FunctorParams {
public:
    AddLayerElementToFlatListParams(ListOfConstObjects *flatList) { m_flatList = flatList; }
    ListOfConstObjects *m_flatList;
};

//----------------------------------------------------------------------------
// ApplyPPUFactorParams
//----------------------------------------------------------------------------

/**
 * member 0: the current page
 */

class ApplyPPUFactorParams : public FunctorParams {
public:
    ApplyPPUFactorParams() { m_page = NULL; }
    Page *m_page;
};

//----------------------------------------------------------------------------
// ConvertMarkupAnalyticalParams
//----------------------------------------------------------------------------

/**
 * member 0: std::vector<Note*>* that holds the current notes with open ties
 * member 1: Chord** currentChord for the current chord if in a chord
 * member 2: an array of control events to be added to the measure (at its end)
 * member 3: a flag indicating whereas the conversion is permanent of not
 **/

class ConvertMarkupAnalyticalParams : public FunctorParams {
public:
    ConvertMarkupAnalyticalParams(bool permanent)
    {
        m_currentChord = NULL;
        m_permanent = permanent;
    }
    std::vector<Note *> m_currentNotes;
    Chord *m_currentChord;
    ArrayOfObjects m_controlEvents;
    bool m_permanent;
};

//----------------------------------------------------------------------------
// ConvertMarkupArticParams
//----------------------------------------------------------------------------

/**
 * member 0: std::vector<Artic *>* that needs to be converted
 **/

class ConvertMarkupArticParams : public FunctorParams {
public:
    ConvertMarkupArticParams() {}
    std::vector<std::pair<Object *, Artic *>> m_articPairsToConvert;
};

//----------------------------------------------------------------------------
// ConvertMarkupScoreDefParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the scoreDef we are moving the content from
 * member 1: the doc
 * member 2: the functor
 * member 3: the end functor
 **/

class ConvertMarkupScoreDefParams : public FunctorParams {
public:
    ConvertMarkupScoreDefParams(Doc *doc, Functor *functor, Functor *functorEnd)
    {
        m_currentScoreDef = NULL;
        m_doc = doc;
        m_functor = functor;
        m_functorEnd = functorEnd;
    }
    ScoreDefElement *m_currentScoreDef;
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
};

//----------------------------------------------------------------------------
// ConvertToCastOffMensuralParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer the document we are adding pages to
 * member 1: a vector of all the staff @n for finding spliting bar lines
 * member 2: a pointer to the content Layer from which we are copying the elements
 * member 3: a pointer to the target destination System
 * member 4: a pointer to a sub-system (e.g., section) to add measure segments
 * member 4: a pointer to the target destination System
 * member 5: a pointer to the target destination Measure
 * member 6: a pointer to the target destination Staff
 * member 7: a pointer to the target destination Layer
 * member 8: a counter for segments in the sub-system (section)
 * member 9  a counter for the total number of segments (previous sections)
 * member 10: a IntTree for precessing by Layer
 **/

class ConvertToCastOffMensuralParams : public FunctorParams {
public:
    ConvertToCastOffMensuralParams(Doc *doc, System *targetSystem, IntTree *layerTree)
    {
        m_doc = doc;
        m_contentLayer = NULL;
        m_targetSystem = targetSystem;
        m_targetSubSystem = NULL;
        m_targetMeasure = NULL;
        m_targetStaff = NULL;
        m_targetLayer = NULL;
        m_segmentIdx = 0;
        m_segmentTotal = 0;
        m_layerTree = layerTree;
    }
    Doc *m_doc;
    std::vector<int> m_staffNs;
    Layer *m_contentLayer;
    System *m_targetSystem;
    System *m_targetSubSystem;
    Measure *m_targetMeasure;
    Staff *m_targetStaff;
    Layer *m_targetLayer;
    int m_segmentIdx;
    int m_segmentTotal;
    IntTree *m_layerTree;
};

//----------------------------------------------------------------------------
// ConvertToPageBasedParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the system we are moving the content to
 **/

class ConvertToPageBasedParams : public FunctorParams {
public:
    ConvertToPageBasedParams(Page *page)
    {
        m_currentSystem = NULL;
        m_page = page;
    }
    System *m_currentSystem;
    Page *m_page;
};

//----------------------------------------------------------------------------
// ConvertToUnCastOffMensuralParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the content / target Measure (NULL at the beginning of a section)
 * member 1: a pointer to the content / target Layer (NULL at the beginning of a section)
 * member 2: a flag indicating if we keep a reference of the measure segments to delete at the end
 * member 3: a list of measure segments to delete at the end (fill in the first pass only)
 **/

class ConvertToUnCastOffMensuralParams : public FunctorParams {
public:
    ConvertToUnCastOffMensuralParams()
    {
        m_contentMeasure = NULL;
        m_contentLayer = NULL;
        m_addSegmentsToDelete = true;
    }
    Measure *m_contentMeasure;
    Layer *m_contentLayer;
    bool m_addSegmentsToDelete;
    ArrayOfObjects m_segmentsToDelete;
};

//----------------------------------------------------------------------------
// FindLayerIDWithinStaffDefParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the element inside Layer StaffDef
 * member 1: ID of element to be found
 **/

class FindLayerIDWithinStaffDefParams : public FunctorParams {
public:
    explicit FindLayerIDWithinStaffDefParams(const std::string &xmlId)
    {
        m_id = xmlId;
        m_object = NULL;
    }

    const Object *m_object;
    std::string m_id;
};

//----------------------------------------------------------------------------
// GenerateFeaturesParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the Doc
 * member 1: a pointer to the FeatureExtractor to which extraction is delegated
 **/

class GenerateFeaturesParams : public FunctorParams {
public:
    GenerateFeaturesParams(Doc *doc, FeatureExtractor *extractor)
    {
        m_doc = doc;
        m_extractor = extractor;
    }
    Doc *m_doc;
    FeatureExtractor *m_extractor;
};

//----------------------------------------------------------------------------
// GenerateMIDIParams
//----------------------------------------------------------------------------

/**
 * Helper struct to store note sequences which replace notes in MIDI output due to expanded ornaments and tremolandi
 */
struct MIDINote {
    int pitch;
    double duration;
};

using MIDINoteSequence = std::list<MIDINote>;

/**
 * Helper struct for held notes in tablature
 */
struct MIDIHeldNote {
    int m_pitch = 0;
    double m_stopTime = 0;
};

/**
 * Helper struct to store chord sequences in MIDI output due to grace notes
 */
struct MIDIChord {
    std::set<int> pitches;
    double duration;
};

using MIDIChordSequence = std::list<MIDIChord>;

/**
 * member 0: MidiFile*: the MidiFile we are writing to
 * member 1: int: the midi track number
 * member 2: int: the midi channel number
 * member 3: double: the score time from the start of the music to the start of the current measure
 * member 4: the current staff number
 * member 5: the semi tone transposition for the current track
 * member 6: double with the current tempo
 * member 7: the last (non grace) note that was performed
 * member 8: expanded notes due to ornaments and tremolandi
 * member 9: deferred notes which start slightly later
 * member 10: grace note sequence
 * member 11: flag indicating whether the last grace note/chord was accented
 * member 12: flag indicating whether cue notes should be included
 * member 13: the functor
 * member 14: Tablature held notes indexed by (course - 1)
 **/

class GenerateMIDIParams : public FunctorParams {
public:
    GenerateMIDIParams(smf::MidiFile *midiFile, Functor *functor)
    {
        m_midiFile = midiFile;
        m_midiTrack = 1;
        m_midiChannel = 0;
        m_totalTime = 0.0;
        m_staffN = 0;
        m_transSemi = 0;
        m_currentTempo = MIDI_TEMPO;
        m_lastNote = NULL;
        m_accentedGraceNote = false;
        m_cueExclusion = false;
        m_functor = functor;
    }
    smf::MidiFile *m_midiFile;
    int m_midiTrack;
    int m_midiChannel;
    double m_totalTime;
    int m_staffN;
    int m_transSemi;
    double m_currentTempo;
    Note *m_lastNote;
    std::map<Note *, MIDINoteSequence> m_expandedNotes;
    std::map<Note *, double> m_deferredNotes;
    MIDIChordSequence m_graceNotes;
    bool m_accentedGraceNote;
    bool m_cueExclusion;
    Functor *m_functor;
    std::vector<MIDIHeldNote> m_heldNotes;
};

//----------------------------------------------------------------------------
// GenerateTimemapParams
//----------------------------------------------------------------------------

/**
 * member 0: Score time from the start of the piece to previous barline in quarter notes
 * member 1: Real time from the start of the piece to previous barline in ms
 * member 2: Currently active tempo
 * member 3: flag indicating whether cue notes should be included
 * member 4: a pointer to the Timemap
 * member 5: the functor for redirection
 **/

class GenerateTimemapParams : public FunctorParams {
public:
    GenerateTimemapParams(Timemap *timemap, Functor *functor)
    {
        m_scoreTimeOffset = 0.0;
        m_realTimeOffsetMilliseconds = 0;
        m_currentTempo = MIDI_TEMPO;
        m_cueExclusion = false;
        m_timemap = timemap;
        m_functor = functor;
    }
    double m_scoreTimeOffset;
    double m_realTimeOffsetMilliseconds;
    double m_currentTempo;
    bool m_cueExclusion;
    Timemap *m_timemap;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// GetAlignmentLeftRightParams
//----------------------------------------------------------------------------

/**
 * member 0: the min left
 * member 1: the max right
 **/

class GetAlignmentLeftRightParams : public FunctorParams {
public:
    GetAlignmentLeftRightParams(Functor *functor)
    {
        m_minLeft = -VRV_UNSET;
        m_maxRight = VRV_UNSET;
        m_functor = functor;
    }
    int m_minLeft;
    int m_maxRight;
    std::vector<ClassId> m_excludeClasses;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// InitMaxMeasureDurationParams
//----------------------------------------------------------------------------

/**
 * member 0: the current score time
 * member 1: the current time in seconds
 * member 2: the current tempo
 * member 3: the tempo adjustment
 * member 4: factor for multibar rests
 **/

class InitMaxMeasureDurationParams : public FunctorParams {
public:
    InitMaxMeasureDurationParams()
    {
        m_currentScoreTime = 0.0;
        m_currentRealTimeSeconds = 0.0;
        m_currentTempo = MIDI_TEMPO;
        m_tempoAdjustment = 1.0;
        m_multiRestFactor = 1;
    }
    double m_currentScoreTime;
    double m_currentRealTimeSeconds;
    double m_currentTempo;
    double m_tempoAdjustment;
    int m_multiRestFactor;
};

//----------------------------------------------------------------------------
// InitOnsetOffset
//----------------------------------------------------------------------------

/**
 * member 0: double: the current score time in the measure (incremented by each element)
 * member 1: double: the current real time in seconds in the measure (incremented by each element)
 * member 2: the current Mensur
 * member 3: the current MeterSig
 * member 4: the current notation type
 * member 5: the current tempo
 **/

class InitOnsetOffsetParams : public FunctorParams {
public:
    InitOnsetOffsetParams()
    {
        m_currentScoreTime = 0.0;
        m_currentRealTimeSeconds = 0.0;
        m_currentMensur = NULL;
        m_currentMeterSig = NULL;
        m_notationType = NOTATIONTYPE_cmn;
        m_currentTempo = MIDI_TEMPO;
    }
    double m_currentScoreTime;
    double m_currentRealTimeSeconds;
    Mensur *m_currentMensur;
    MeterSig *m_currentMeterSig;
    data_NOTATIONTYPE m_notationType;
    double m_currentTempo;
};

//----------------------------------------------------------------------------
// InitProcessingListsParams
//----------------------------------------------------------------------------

/**
 * member 0: the IntTree* for staff/layer/verse
 * member 1: the IntTree* for staff/layer
 **/

class InitProcessingListsParams : public FunctorParams {
public:
    InitProcessingListsParams() {}
    IntTree m_verseTree;
    IntTree m_layerTree;
};

//----------------------------------------------------------------------------
// InitMIDIParams
//----------------------------------------------------------------------------

/**
 * member 0: the current tempo
 * member 1: deferred notes which start slightly later
 **/

class InitMIDIParams : public FunctorParams {
public:
    InitMIDIParams() { m_currentTempo = MIDI_TEMPO; }
    double m_currentTempo;
    std::map<Note *, double> m_deferredNotes;
};

//----------------------------------------------------------------------------
// ReorderByXPosParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the current object whose children we (may) reorder
 **/
class ReorderByXPosParams : public FunctorParams {
public:
    int modifications = 0;
};

//----------------------------------------------------------------------------
// SaveParams
//----------------------------------------------------------------------------

/**
 * member 0: output stream
 * member 1: flag for MEI basic output for filtering out editorial markup
 **/

class SaveParams : public FunctorParams {
public:
    SaveParams(Output *output, bool basic)
    {
        m_output = output;
        m_basic = basic;
    }
    Output *m_output;
    bool m_basic;
};

//----------------------------------------------------------------------------
// TransposeParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the document
 * member 1: the functor for redirection
 * member 2: the end functor for redirection
 * member 3: a pointer to the transposer
 * member 4: the transposition to be applied
 * member 5: the mdiv selected for transposition
 * member 6: the list of current (nested) mdivs
 * member 7: transpose to sounding pitch by evaluating @trans.semi
 * member 8: current KeySig for staff (ScoreDef key signatures are mapped to -1)
 * member 9: transposition interval for staff
 **/

class TransposeParams : public FunctorParams {
public:
    TransposeParams(Doc *doc, Functor *functor, Functor *functorEnd, Transposer *transposer)
    {
        m_doc = doc;
        m_functor = functor;
        m_functorEnd = functorEnd;
        m_transposer = transposer;
        m_transposeToSoundingPitch = false;
    }
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
    Transposer *m_transposer;
    std::string m_transposition;
    std::string m_selectedMdivID;
    std::list<std::string> m_currentMdivIDs;
    bool m_transposeToSoundingPitch;
    std::map<int, const KeySig *> m_keySigForStaffN;
    std::map<int, int> m_transposeIntervalForStaffN;
};

} // namespace vrv

#endif
