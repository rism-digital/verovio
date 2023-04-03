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
// AdjustArticParams
//----------------------------------------------------------------------------

/**
 * member 0: list of above articulations
 * member 1: list of below articulations
 * member 2: the parent element to which the articulations refer
 * member 3: the doc
 **/

class AdjustArticParams : public FunctorParams {
public:
    AdjustArticParams(Doc *doc)
    {
        m_parent = NULL;
        m_doc = doc;
    }
    std::list<Artic *> m_articAbove;
    std::list<Artic *> m_articBelow;
    LayerElement *m_parent;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AdjustBeamParams
//----------------------------------------------------------------------------

/**
 * member 0: the beam that should be adjusted
 * member 1: y coordinate of the beam left side
 * member 2: y coordinate of the beam right side
 * member 3: x coordinate of the beam left side (starting point)
 * member 4: x coordinate of the beam right side (end point)
 * member 5: slope of the beam
 * member 6: overlap margin that beam needs to be displaced by
 * member 7: the Doc
 * member 8: the flag indicating whether element from different layer is being processed
 **/

class AdjustBeamParams : public FunctorParams {
public:
    AdjustBeamParams(Doc *doc)
    {
        m_beam = NULL;
        m_y1 = 0;
        m_y2 = 0;
        m_x1 = 0;
        m_x2 = 0;
        m_beamSlope = 0.0;
        m_directionBias = 0;
        m_overlapMargin = 0;
        m_doc = doc;
        m_isOtherLayer = false;
    }

    Object *m_beam;
    int m_y1;
    int m_y2;
    int m_x1;
    int m_x2;
    double m_beamSlope;
    int m_directionBias;
    int m_overlapMargin;
    Doc *m_doc;
    bool m_isOtherLayer;
};

//----------------------------------------------------------------------------
// AdjustFloatingPositionersParams
//----------------------------------------------------------------------------

/**
 * member 0: the classId
 * member 1: a flag indicating we are processing floating object to be put in between
 * member 2: the doc
 * member 3: a pointer to the functor for passing it to the system aligner
 **/

class AdjustFloatingPositionersParams : public FunctorParams {
public:
    AdjustFloatingPositionersParams(Doc *doc, Functor *functor)
    {
        m_classId = OBJECT;
        m_inBetween = false;
        m_doc = doc;
        m_functor = functor;
    }
    ClassId m_classId;
    bool m_inBetween;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// AdjustFloatingPositionersBetweenParams
//----------------------------------------------------------------------------

/**
 * member 2: the doc
 * member 3: a pointer to the functor for passing it to the system aligner
 **/

class AdjustFloatingPositionersBetweenParams : public FunctorParams {
public:
    AdjustFloatingPositionersBetweenParams(Doc *doc, Functor *functor)
    {
        m_previousStaffPositioners = NULL;
        m_previousStaffAlignment = NULL;
        m_doc = doc;
        m_functor = functor;
    }
    const ArrayOfFloatingPositioners *m_previousStaffPositioners;
    StaffAlignment *m_previousStaffAlignment;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// AdjustFloatingPositionerGrpsParams
//----------------------------------------------------------------------------

/**
 * member 0: a vector of the classId to group
 * member 1: the doc
 **/

class AdjustFloatingPositionerGrpsParams : public FunctorParams {
public:
    AdjustFloatingPositionerGrpsParams(Doc *doc)
    {
        m_doc = doc;
        m_place = STAFFREL_above;
    }
    std::vector<ClassId> m_classIds;
    data_STAFFREL m_place;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AdjustSlursParams
//----------------------------------------------------------------------------

/**
 * member 0: a flag indicating that there is at least one cross-staff slur
 * member 1: the doc
 * member 2: a pointer to the functor for passing it to the system aligner
 **/

class AdjustSlursParams : public FunctorParams {
public:
    AdjustSlursParams(Doc *doc, Functor *functor)
    {
        m_crossStaffSlurs = false;
        m_doc = doc;
        m_functor = functor;
    }
    bool m_crossStaffSlurs;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// AdjustStaffOverlapParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the previous staff alignment
 * member 1: the doc
 * member 2: a pointer to the functor for passing it to the system aligner
 **/

class AdjustStaffOverlapParams : public FunctorParams {
public:
    AdjustStaffOverlapParams(Doc *doc, Functor *functor)
    {
        m_previous = NULL;
        m_doc = doc;
        m_functor = functor;
    }
    StaffAlignment *m_previous;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// AdjustTupletNumOverlapParams
//----------------------------------------------------------------------------

/**
 * member 0: tupletNum relative position for which is being calculatied
 * member 1: drawing position of tupletNum
 * member 2: margin for tupletNum vertical overlap
 * member 3: staff relevant to positioning of tuplet
 * member 4: resulting relative Y for the tupletNum
 **/
class AdjustTupletNumOverlapParams : public FunctorParams {
public:
    AdjustTupletNumOverlapParams(const TupletNum *tupletNum, const Staff *staff)
    {
        m_tupletNum = tupletNum;
        m_drawingNumPos = STAFFREL_basic_NONE;
        m_horizontalMargin = 0;
        m_verticalMargin = 0;
        m_staff = staff;
        m_yRel = 0;
    }

    const TupletNum *m_tupletNum;
    data_STAFFREL_basic m_drawingNumPos;
    int m_horizontalMargin;
    int m_verticalMargin;
    const Staff *m_staff;
    int m_yRel;
};

//----------------------------------------------------------------------------
// AdjustYPosParams
//----------------------------------------------------------------------------

/**
 * member 0: the cumulated shift
 * member 1: the doc
 * member 2: the functor to be redirected to SystemAligner
 **/

class AdjustYPosParams : public FunctorParams {
public:
    AdjustYPosParams(Doc *doc, Functor *functor)
    {
        m_cumulatedShift = 0;
        m_doc = doc;
        m_functor = functor;
    }
    int m_cumulatedShift;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// AlignSystemsParams
//----------------------------------------------------------------------------

/**
 * member 0: the cumulated shift
 * member 1: the system margin
 * member 2: the overflow below of previous system
 * member 3: the sum of justification factors per page
 * member 4: the doc
 **/

class AlignSystemsParams : public FunctorParams {
public:
    AlignSystemsParams(Doc *doc)
    {
        m_shift = 0;
        m_systemSpacing = 0;
        m_prevBottomOverflow = 0;
        m_prevBottomClefOverflow = 0;
        m_justificationSum = 0.;
        m_doc = doc;
    }
    int m_shift;
    int m_systemSpacing;
    int m_prevBottomOverflow;
    int m_prevBottomClefOverflow;
    double m_justificationSum;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AlignVerticallyParams
//----------------------------------------------------------------------------

/**
 * member 0: the systemAligner
 * member 1: the staffIdx
 * member 2: the staffN
 * member 3: the cumulated shift for the default alignment
 * member 4: the sum of justification factors per page
 * member 5: the page width
 * member 6: the functor (for redirecting from page running elements)
 * member 7: the end functor (for redirecting from measure)
 * member 8: the doc
 **/

class AlignVerticallyParams : public FunctorParams {
public:
    AlignVerticallyParams(Doc *doc, Functor *functor, Functor *functorEnd)
    {
        m_systemAligner = NULL;
        m_staffIdx = 0;
        m_staffN = 0;
        m_cumulatedShift = 0;
        m_justificationSum = 0.;
        m_pageWidth = 0;
        m_functor = functor;
        m_functorEnd = functorEnd;
        m_doc = doc;
    }
    SystemAligner *m_systemAligner;
    int m_staffIdx;
    int m_staffN;
    int m_cumulatedShift;
    int m_justificationSum;
    int m_pageWidth;
    Functor *m_functor;
    Functor *m_functorEnd;
    Doc *m_doc;
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
// CalcArticParams
//----------------------------------------------------------------------------

/**
 * member 0: the chord dots object when processing chord notes
 * member 7: the doc
 **/

class CalcArticParams : public FunctorParams {
public:
    CalcArticParams(Doc *doc)
    {
        m_parent = NULL;
        m_doc = doc;
        m_staffAbove = NULL;
        m_staffBelow = NULL;
        m_layerAbove = NULL;
        m_layerBelow = NULL;
        m_crossStaffAbove = false;
        m_crossStaffBelow = false;
    }
    LayerElement *m_parent;
    data_STEMDIRECTION m_stemDir;
    Staff *m_staffAbove;
    Staff *m_staffBelow;
    Layer *m_layerAbove;
    Layer *m_layerBelow;
    bool m_crossStaffAbove;
    bool m_crossStaffBelow;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// CalcBBoxOverflowsParams
//----------------------------------------------------------------------------

/**
 * member 0: the current staffAlignment
 * member 1: the doc
 **/

class CalcBBoxOverflowsParams : public FunctorParams {
public:
    CalcBBoxOverflowsParams(Doc *doc)
    {
        m_staffAlignment = NULL;
        m_doc = doc;
    }
    StaffAlignment *m_staffAlignment;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// CastOffEncodingParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer the document we are adding pages to
 * member 1: a pointer to the current page
 * member 2: a pointer to the current system
 * member 3: a pointer to the system we are taking the content from
 * member 4: a flag if we want to use the pageBreaks from the document
 **/

class CastOffEncodingParams : public FunctorParams {
public:
    CastOffEncodingParams(Doc *doc, Page *currentPage, bool usePages = true)
    {
        m_doc = doc;
        m_currentPage = currentPage;
        m_currentSystem = NULL;
        m_contentSystem = NULL;
        m_usePages = usePages;
    }
    Doc *m_doc;
    Page *m_currentPage;
    System *m_contentSystem;
    System *m_currentSystem;
    bool m_usePages;
};

//----------------------------------------------------------------------------
// CastOffPagesParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the page we are taking the content from
 * member 1: a pointer the document we are adding pages to
 * member 2: a pointer to the current page
 * member 3: the cummulated shift (m_drawingYRel of the first system of the current page)
 * members 4-8: the page heights
 * member 9: a pointer to the leftover system (last system with only one measure)
 * member 10: the current pending elements (Mdiv, Score) to be place at the beginning of a page
 **/

class CastOffPagesParams : public FunctorParams {
public:
    CastOffPagesParams(Page *contentPage, Doc *doc, Page *currentPage)
    {
        m_contentPage = contentPage;
        m_doc = doc;
        m_currentPage = currentPage;
        m_shift = 0;
        m_pageHeight = 0;
        m_pgHeadHeight = 0;
        m_pgFootHeight = 0;
        m_pgHead2Height = 0;
        m_pgFoot2Height = 0;
        m_leftoverSystem = NULL;
    }
    Page *m_contentPage;
    Doc *m_doc;
    Page *m_currentPage;
    int m_shift;
    int m_pageHeight;
    int m_pgHeadHeight;
    int m_pgFootHeight;
    int m_pgHead2Height;
    int m_pgFoot2Height;
    System *m_leftoverSystem;
    ArrayOfObjects m_pendingPageElements;
};

//----------------------------------------------------------------------------
// CastOffSystemsParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the system we are taking the content from
 * member 1: a pointer the page we are adding system to
 * member 2: a pointer to the current system
 * member 3: the cummulated shift (m_drawingXRel of the first measure of the current system)
 * member 4: the system width
 * member 5: the current scoreDef width
 * member 6: the current pending elements (ScoreDef, Endings, etc.) to be place at the beginning of a system
 * member 7: the doc
 * member 8: whether to smartly use encoded system breaks
 * member 9: a pointer to the leftover system (last system with only one measure)
 **/

class CastOffSystemsParams : public FunctorParams {
public:
    CastOffSystemsParams(Page *page, Doc *doc, bool smart)
    {
        m_page = page;
        m_contentSystem = NULL;
        m_currentSystem = NULL;
        m_shift = 0;
        m_systemWidth = 0;
        m_currentScoreDefWidth = 0;
        m_doc = doc;
        m_smart = smart;
        m_leftoverSystem = NULL;
    }
    System *m_contentSystem;
    Page *m_page;
    System *m_currentSystem;
    int m_shift;
    int m_systemWidth;
    int m_currentScoreDefWidth;
    ArrayOfObjects m_pendingElements;
    Doc *m_doc;
    bool m_smart;
    System *m_leftoverSystem;
};

//----------------------------------------------------------------------------
// CastOffToSelectionParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the system we are taking the content from
 * member 1: a pointer the page we are adding system to
 * member 2: a pointer to the current system
 * member 3: a pointer to the doc
 **/

class CastOffToSelectionParams : public FunctorParams {
public:
    CastOffToSelectionParams(Page *page, Doc *doc, const std::string &start, const std::string &end)
    {
        m_page = page;
        m_contentSystem = NULL;
        m_currentSystem = NULL;
        m_start = start;
        m_end = end;
        m_isSelection = false;
        m_doc = doc;
    }
    System *m_contentSystem;
    Page *m_page;
    System *m_currentSystem;
    std::string m_start;
    std::string m_end;
    bool m_isSelection;
    Doc *m_doc;
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
// JustifyXParams
//----------------------------------------------------------------------------

/**
 * member 0: the relative X position of the next measure
 * member 1: the justification ratio
 * member 2: the left barline X position
 * member 3: the right barline X position
 * member 4: the system full width (without system margins)
 * member 5: shift next measure due to section restart
 * member 6: the functor to be redirected to the MeasureAligner
 * member 7: the doc
 **/

class JustifyXParams : public FunctorParams {
public:
    JustifyXParams(Functor *functor, Doc *doc)
    {
        m_measureXRel = 0;
        m_justifiableRatio = 1.0;
        m_leftBarLineX = 0;
        m_rightBarLineX = 0;
        m_systemFullWidth = 0;
        m_applySectionRestartShift = false;
        m_functor = functor;
        m_doc = doc;
    }
    int m_measureXRel;
    double m_justifiableRatio;
    int m_leftBarLineX;
    int m_rightBarLineX;
    int m_systemFullWidth;
    bool m_applySectionRestartShift;
    Functor *m_functor;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// JustifyYParams
//----------------------------------------------------------------------------

/**
 * member 0: the cumulated shift
 * member 1: the relative shift of the staff w.r.t. the system
 * member 2: the amount of space for distribution
 * member 3: the sum of justification factors per page
 * member 4: a map of calculated shifts per StaffAlignment
 *  => this is transferred to JustifyYAdjustCrossStaff
 * member 5: the functor to be redirected to the MeasureAligner
 * member 6: the doc
 **/

class JustifyYParams : public FunctorParams {
public:
    JustifyYParams(Functor *functor, Doc *doc)
    {
        m_cumulatedShift = 0;
        m_relativeShift = 0;
        m_spaceToDistribute = 0;
        m_justificationSum = 0.;
        m_functor = functor;
        m_doc = doc;
    }

    int m_cumulatedShift;
    int m_relativeShift;
    int m_spaceToDistribute;
    double m_justificationSum;
    std::map<StaffAlignment *, int> m_shiftForStaff;
    Functor *m_functor;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// JustifyYAdjustCrossStaffParams
//----------------------------------------------------------------------------

/**
 * member 0: a map of calculated shifts per StaffAlignment
 *  => this is transferred from JustifyY
 * member 1: the doc
 **/

class JustifyYAdjustCrossStaffParams : public FunctorParams {
public:
    JustifyYAdjustCrossStaffParams(Doc *doc) { m_doc = doc; }

    std::map<StaffAlignment *, int> m_shiftForStaff;
    Doc *m_doc;
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

//----------------------------------------------------------------------------
// UnCastOffParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the page we are adding system to
 * member 1: a pointer to the system we are adding content to
 * member 2: a flag indicating if we need to reset the horizontal layout cache
 **/

class UnCastOffParams : public FunctorParams {
public:
    UnCastOffParams(Page *page)
    {
        m_page = page;
        m_currentSystem = NULL;
        m_resetCache = true;
    }
    Page *m_page;
    System *m_currentSystem;
    bool m_resetCache;
};

} // namespace vrv

#endif
