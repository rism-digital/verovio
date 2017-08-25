/////////////////////////////////////////////////////////////////////////////
// Name:        functorparams.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FUNCTOR_PARAMS_H__
#define __VRV_FUNCTOR_PARAMS_H__

//----------------------------------------------------------------------------

#include "vrvdef.h"

class MidiFile;

namespace vrv {

class AttComparison;
class BoundaryStartInterface;
class Chord;
class Clef;
class Dots;
class Ending;
class FileOutputStream;
class KeySig;
class Layer;
class LayerElement;
class Measure;
class MeasureAligner;
class Mensur;
class MeterSig;
class MRpt;
class Object;
class Page;
class ScoreDef;
class Staff;
class StaffAlignment;
class StaffDef;
class Syl;
class System;
class SystemAligner;

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
 * member 0: the ListOfObjects
 **/

class AddLayerElementToFlatListParams : public FunctorParams {
public:
    AddLayerElementToFlatListParams(ListOfObjects *flatList) { m_flatList = flatList; }
    ListOfObjects *m_flatList;
};
    
//----------------------------------------------------------------------------
// AdjustAccidXParams
//----------------------------------------------------------------------------

/**
 * member 0: the Functor to be redirected to MeasureAligner and GraceAligner
 * member 1: the Doc
 **/

class AdjustAccidXParams : public FunctorParams {
public:
    AdjustAccidXParams(Doc *doc, Functor *functor)
    {
        m_currentMeasure = NULL;
        m_functor = functor;
        m_doc = doc;
    }
    Measure *m_currentMeasure;
    Functor *m_functor;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AdjustArpegParams
//----------------------------------------------------------------------------

/**
 **/

class AdjustArpegParams : public FunctorParams {
public:
    AdjustArpegParams(Doc *doc) {
        m_doc = doc;
    }
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AdjustArticWithSlursParams
//----------------------------------------------------------------------------
    
// Use FunctorDocParams

//----------------------------------------------------------------------------
// AdjustGraceXPosParams
//----------------------------------------------------------------------------

/**
 * member 0: the maximum position
 * member 1: the upcoming maximum position (i.e., the min pos for the next element)
 * member 2: the cumulated shift on the previous aligners
 * member 3: the list of staffN in the top-level scoreDef
 * member 4: the flag indicating whereas the alignment is in a Measure or in a Grace
 * member 5: the pointer to the right ALIGNMENT_DEFAULT (if any)
 * member 6: the Doc
 * member 7: the Functor to be redirected to MeasureAligner and GraceAligner
 * member 8: the end Functor for redirection
 **/

class AdjustGraceXPosParams : public FunctorParams {
public:
    AdjustGraceXPosParams(Doc *doc, Functor *functor, Functor *functorEnd, std::vector<int> staffNs)
    {
        m_graceMaxPos = 0;
        m_graceUpcomingMaxPos = -VRV_UNSET;
        m_graceCumulatedXShift = 0;
        m_staffNs = staffNs;
        m_isGraceAlignment = false;
        m_rightDefaultAlignment = NULL;
        m_doc = doc;
        m_functor = functor;
        m_functorEnd = functorEnd;
    }

    int m_graceMaxPos;
    int m_graceUpcomingMaxPos;
    int m_graceCumulatedXShift;
    std::vector<int> m_staffNs;
    bool m_isGraceAlignment;
    Alignment *m_rightDefaultAlignment;
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
};

//----------------------------------------------------------------------------
// AdjustFloatingPostionersParams
//----------------------------------------------------------------------------

/**
 * member 0: the classId
 * member 1: the doc
 * member 2: a pointer to the functor for passing it to the system aligner
 **/

class AdjustFloatingPostionersParams : public FunctorParams {
public:
    AdjustFloatingPostionersParams(Doc *doc, Functor *functor)
    {
        m_classId = OBJECT;
        m_doc = doc;
        m_functor = functor;
    }
    ClassId m_classId;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// AdjustLayersParams
//----------------------------------------------------------------------------

/**
 * member 0: the list of staffN in the top-level scoreDef
 * member 1: the current layerN set in the AlignmentRef (negative values for cross-staff)
 * member 2: the elements for the previous layer(s)
 * member 3: the elements of the current layer
 * member 4: the current note
 * member 5: the current chord (if any)
 * member 6: the doc
 * member 7: a pointer to the functor for passing it to the system aligner
 **/

class AdjustLayersParams : public FunctorParams {
public:
    AdjustLayersParams(Doc *doc, Functor *functor, const std::vector<int> &staffNs)
    {
        m_currentLayerN = VRV_UNSET;
        m_currentNote = NULL;
        m_currentChord = NULL;
        m_doc = doc;
        m_functor = functor;
        m_staffNs = staffNs;
    }
    std::vector<int> m_staffNs;
    int m_currentLayerN;
    std::vector<LayerElement *> m_previous;
    std::vector<LayerElement *> m_current;
    Note *m_currentNote;
    Chord *m_currentChord;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// AdjustFloatingPostionerGrpsParams
//----------------------------------------------------------------------------

/**
 * member 0: a vector of the classId to group
 * member 1: the doc
 **/

class AdjustFloatingPostionerGrpsParams : public FunctorParams {
public:
    AdjustFloatingPostionerGrpsParams(Doc *doc) { m_doc = doc; }
    std::vector<ClassId> m_classIds;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AdjustStaffOverlapParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the previous staff alignment
 * member 1: a pointer to the functor for passing it to the system aligner
 **/

class AdjustStaffOverlapParams : public FunctorParams {
public:
    AdjustStaffOverlapParams(Functor *functor)
    {
        m_previous = NULL;
        m_functor = functor;
    }
    StaffAlignment *m_previous;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// AdjustSylSpacingParams
//----------------------------------------------------------------------------

/**
 * member 0: a vector adjustment tuples (Aligment start, Aligment end, distance)
 * member 1: a pointer to the previous Syl
 * member 2: the doc
 **/

class AdjustSylSpacingParams : public FunctorParams {
public:
    AdjustSylSpacingParams(Doc *doc)
    {
        m_previousSyl = NULL;
        m_doc = doc;
    }
    ArrayOfAdjustmentTuples m_overlapingSyl;
    Syl *m_previousSyl;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AdjustXPosParams
//----------------------------------------------------------------------------

/**
 * member 0: the minimum position (i.e., the width of the previous element)
 * member 1: the upcoming minimum position (i.e., the min pos for the next element)
 * member 2: the cumulated shift on the previous aligners
 * member 3: the @n of the staff currently processed (used for grace note alignment)
 * member 4: the list of staffN in the top-level scoreDef
 * member 5: the bounding box in the previous aligner
 * member 6: the upcoming bounding boxes (to be used in the next aligner)
 * member 7: the Doc
 * member 8: the Functor for redirection to the MeasureAligner
 * member 9: the end Functor for redirection
 **/

class AdjustXPosParams : public FunctorParams {
public:
    AdjustXPosParams(Doc *doc, Functor *functor, Functor *functorEnd, const std::vector<int> &staffNs)
    {
        m_minPos = 0;
        m_upcomingMinPos = VRV_UNSET;
        m_cumulatedXShift = 0;
        m_staffN = 0;
        m_staffNs = staffNs;
        m_doc = doc;
        m_functor = functor;
        m_functorEnd = functorEnd;
    }
    int m_minPos;
    int m_upcomingMinPos;
    int m_cumulatedXShift;
    int m_staffN;
    std::vector<int> m_staffNs;
    std::vector<BoundingBox *> m_boundingBoxes;
    std::vector<BoundingBox *> m_upcomingBoundingBoxes;
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
};

//----------------------------------------------------------------------------
// AdjustYPosParams
//----------------------------------------------------------------------------

/**
 * member 0: the previous staff height
 * member 1: the extra staff height
 * member 2  the previous verse count
 * member 3: the cumulated shift
 * member 4: the doc
 * member 5: the functor to be redirected to SystemAligner
 **/

class AdjustYPosParams : public FunctorParams {
public:
    AdjustYPosParams(Doc *doc, Functor *functor)
    {
        m_previousOverflowBelow = 0;
        m_previousVerseCount = 0;
        m_cumulatedShift = 0;
        m_doc = doc;
        m_functor = functor;
    }
    int m_previousOverflowBelow;
    int m_previousVerseCount;
    int m_cumulatedShift;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// AlignHorizontallyParams
//----------------------------------------------------------------------------

/**
 * member 0: the measureAligner
 * member 1: the time
 * member 2: the current Mensur
 * member 3: the current MeterSig
 * member 4: the functor for passing it to the TimeStampAligner
 * member 5: a flag indicating whereas we are processing the caution scoreDef
 * member 6: a flag indicating is we are in the first measure (for the scoreDef role)
 * member 7: a flag indicating if we had mutliple layer alignment reference in the measure
 **/

class AlignHorizontallyParams : public FunctorParams {
public:
    AlignHorizontallyParams(Functor *functor)
    {
        m_measureAligner = NULL;
        m_time = 0.0;
        m_currentMensur = NULL;
        m_currentMeterSig = NULL;
        m_functor = functor;
        m_scoreDefRole = NONE;
        m_isFirstMeasure = false;
        m_hasMultipleLayer = false;
    }
    MeasureAligner *m_measureAligner;
    double m_time;
    Mensur *m_currentMensur;
    MeterSig *m_currentMeterSig;
    Functor *m_functor;
    ElementScoreDefRole m_scoreDefRole;
    bool m_isFirstMeasure;
    bool m_hasMultipleLayer;
};

//----------------------------------------------------------------------------
// AlignMeasuresParams
//----------------------------------------------------------------------------

/**
 * member 0: the cumulated shift
 * member 1: the cumulated justifiable width
 **/

class AlignMeasuresParams : public FunctorParams {
public:
    AlignMeasuresParams()
    {
        m_shift = 0;
        m_justifiableWidth = 0;
    }

    int m_shift;
    int m_justifiableWidth;
};

//----------------------------------------------------------------------------
// AlignSystemsParams
//----------------------------------------------------------------------------

/**
 * member 0: the cumulated shift
 * member 1: the system margin
 **/

class AlignSystemsParams : public FunctorParams {
public:
    AlignSystemsParams()
    {
        m_shift = 0;
        m_systemMargin = 0;
    }
    int m_shift;
    int m_systemMargin;
};

//----------------------------------------------------------------------------
// AlignVerticallyParams
//----------------------------------------------------------------------------

/**
 * member 0: the systemAligner
 * member 1: the staffIdx
 * member 2: the staffN
 * member 3: the cumulated shift for the default alignment
 * member 4: the end functor (for redirecting from measure)
 * member 5: the doc
 **/

class AlignVerticallyParams : public FunctorParams {
public:
    AlignVerticallyParams(Doc *doc, Functor *functorEnd)
    {
        m_systemAligner = NULL;
        m_staffIdx = 0;
        m_staffN = 0;
        m_cumulatedShift = 0;
        m_functorEnd = functorEnd;
        m_doc = doc;
    }
    SystemAligner *m_systemAligner;
    int m_staffIdx;
    int m_staffN;
    int m_cumulatedShift;
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

// Use FunctorDocParams

//----------------------------------------------------------------------------
// CalcChordNoteHeads
//----------------------------------------------------------------------------

// Use FunctorDocParams

//----------------------------------------------------------------------------
// CalcDotsParams
//----------------------------------------------------------------------------

/**
 * member 0: the chord dots object when processing chord notes
 * member 7: the doc
 **/

class CalcDotsParams : public FunctorParams {
public:
    CalcDotsParams(Doc *doc)
    {
        m_chordDots = NULL;
        m_chordDrawingX = 0;
        m_chordStemDir = STEMDIRECTION_NONE;
        m_doc = doc;
    }
    Dots *m_chordDots;
    int m_chordDrawingX;
    data_STEMDIRECTION m_chordStemDir;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// CalcLedgerLine
//----------------------------------------------------------------------------

// Use FunctorDocParams

//----------------------------------------------------------------------------
// CalcMaxMeasureDurationParams
//----------------------------------------------------------------------------

/**
 * member 0: std::vector<double>: a stack of maximum duration filled by the functor
 * member 1: double: the duration of the current measure
 * member 2: the current bpm
 **/

class CalcMaxMeasureDurationParams : public FunctorParams {
public:
    CalcMaxMeasureDurationParams()
    {
        m_currentScoreTime = 0.0;
        m_currentRealTimeSeconds = 0.0;
        m_maxCurrentScoreTime = 0.0;
        m_maxCurrentRealTimeSeconds = 0.0;
        m_currentTempo = 120;
    }
    double m_currentScoreTime;
    double m_currentRealTimeSeconds;
    double m_maxCurrentScoreTime;
    double m_maxCurrentRealTimeSeconds;
    int m_currentTempo;
};

//----------------------------------------------------------------------------
// CalcOnsetOffset
//----------------------------------------------------------------------------

/**
 * member 0: double: the current score time in the measure (incremented by each element)
 * member 1: double: the current real time in seconds in the measure (incremented by each element)
 **/

class CalcOnsetOffsetParams : public FunctorParams {
public:
    CalcOnsetOffsetParams()
    {
        m_currentScoreTime = 0.0;
        m_currentRealTimeSeconds = 0.0;
        m_currentTempo = 120;
    }
    double m_currentScoreTime;
    double m_currentRealTimeSeconds;
    int m_currentTempo;
};

//----------------------------------------------------------------------------
// CalcStemParams
//----------------------------------------------------------------------------

/**
 * member 0: the additional length for a chord stem (between the top and bottom note)
 * member 1: the vertical center of the staff
 * member 2: the actual duration of the chord / note
 * member 3: the flag for grace notes (stem is not extended)
 * member 4: the current staff (to avoid additional lookup)
 * member 5: the current layer (ditto)
 * member 6: the chord or note to which the stem belongs
 * member 7: the doc
 **/

class CalcStemParams : public FunctorParams {
public:
    CalcStemParams(Doc *doc)
    {
        m_chordStemLength = 0;
        m_verticalCenter = 0;
        m_dur = DUR_1;
        m_isGraceNote = false;
        m_staff = NULL;
        m_layer = NULL;
        m_interface = NULL;
        m_doc = doc;
    }
    int m_chordStemLength;
    int m_verticalCenter;
    int m_dur;
    bool m_isGraceNote;
    Staff *m_staff;
    Layer *m_layer;
    StemmedDrawingInterface *m_interface;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// CastOffEncodingParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer the document we are adding pages to
 * member 2: a pointer to the current page
 * member 4: a pointer to the current system
 * member 3: a pointer to the system we are taking the content from
 * member 5: a flag indicating if we have processed the first pb
 **/

class CastOffEncodingParams : public FunctorParams {
public:
    CastOffEncodingParams(Doc *doc, Page *currentPage, System *currentSystem, System *contentSystem)
    {
        m_doc = doc;
        m_currentPage = currentPage;
        m_currentSystem = currentSystem;
        m_contentSystem = contentSystem;
        m_firstPbProcessed = false;
    }
    Doc *m_doc;
    Page *m_currentPage;
    System *m_contentSystem;
    System *m_currentSystem;
    bool m_firstPbProcessed;
};

//----------------------------------------------------------------------------
// CastOffPagesParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the page we are taking the content from
 * member 1: a pointer the document we are adding pages to
 * member 2: a pointer to the current page
 * member 3: the cummulated shift (m_drawingYRel of the first system of the current page)
 * member 4: the page height
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
    }
    Page *m_contentPage;
    Doc *m_doc;
    Page *m_currentPage;
    int m_shift;
    int m_pageHeight;
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
 * member 6: the current pending objects (ScoreDef, Endings, etc.) to be place at the beginning of a system
 **/

class CastOffSystemsParams : public FunctorParams {
public:
    CastOffSystemsParams(System *contentSystem, Page *page, System *currentSystem)
    {
        m_contentSystem = contentSystem;
        m_page = page;
        m_currentSystem = currentSystem;
        m_shift = 0;
        m_systemWidth = 0;
        m_currentScoreDefWidth = 0;
    }
    System *m_contentSystem;
    Page *m_page;
    System *m_currentSystem;
    int m_shift;
    int m_systemWidth;
    int m_currentScoreDefWidth;
    ArrayOfObjects m_pendingObjects;
};

//----------------------------------------------------------------------------
// ConvertToPageBasedParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the system we are moving the content to
 **/

class ConvertToPageBasedParams : public FunctorParams {
public:
    ConvertToPageBasedParams(System *pageBasedSystem) { m_pageBasedSystem = pageBasedSystem; }
    System *m_pageBasedSystem;
};

//----------------------------------------------------------------------------
// FillStaffCurrentTimeSpanningParams
//----------------------------------------------------------------------------

/**
 * member 0: std::vector< Object * >* of the current running TimeSpanningInterface elements
 **/

class FillStaffCurrentTimeSpanningParams : public FunctorParams {
public:
    FillStaffCurrentTimeSpanningParams() {}
    std::vector<Object *> m_timeSpanningElements;
};

//----------------------------------------------------------------------------
// FindAllBetweenParams
//----------------------------------------------------------------------------

/**
 * member 0: the attComparision text
 * member 1: an array of all matching objects
 * member 2: the start object range
 * member 3: the end object range
 **/

class FindAllBetweenParams : public FunctorParams {
public:
    FindAllBetweenParams(AttComparison *attComparison, ArrayOfObjects *elements, Object *start, Object *end)
    {
        m_attComparison = attComparison;
        m_elements = elements;
        m_start = start;
        m_end = end;
    }
    AttComparison *m_attComparison;
    ArrayOfObjects *m_elements;
    Object *m_start;
    Object *m_end;
};

//----------------------------------------------------------------------------
// FindAllByAttComparisonParams
//----------------------------------------------------------------------------

/**
 * member 0: the attComparision text
 * member 1: an array of all matching objects
 **/

class FindAllByAttComparisonParams : public FunctorParams {
public:
    FindAllByAttComparisonParams(AttComparison *attComparison, ArrayOfObjects *elements)
    {
        m_attComparison = attComparison;
        m_elements = elements;
    }
    AttComparison *m_attComparison;
    ArrayOfObjects *m_elements;
};

//----------------------------------------------------------------------------
// FindByAttComparisonParams
//----------------------------------------------------------------------------

/**
 * member 0: the attComparision text
 * member 1: the pointer to pointer to the Object
 **/

class FindByAttComparisonParams : public FunctorParams {
public:
    FindByAttComparisonParams(AttComparison *attComparison)
    {
        m_attComparison = attComparison;
        m_element = NULL;
    }
    AttComparison *m_attComparison;
    Object *m_element;
};

//----------------------------------------------------------------------------
// FindByUuidParams
//----------------------------------------------------------------------------

/**
 * member 0: the uuid we are looking for
 * member 1: the pointer to pointer to the Object
 **/

class FindByUuidParams : public FunctorParams {
public:
    FindByUuidParams() { m_element = NULL; }
    std::string m_uuid;
    Object *m_element;
};

//----------------------------------------------------------------------------
// FindExtremeByAttComparisonParams
//----------------------------------------------------------------------------

/**
 * member 0: the attComparision text
 * member 1: the pointer to pointer to the Object
 **/

class FindExtremeByAttComparisonParams : public FunctorParams {
public:
    FindExtremeByAttComparisonParams(AttComparison *attComparison)
    {
        m_attComparison = attComparison;
        m_element = NULL;
    }
    AttComparison *m_attComparison;
    Object *m_element;
};

//----------------------------------------------------------------------------
// FindSpaceInAlignmentParams
//----------------------------------------------------------------------------

/**
 * member 0: the time of the event
 * member 1: the duration of the event
 * member 2: the layer count at that position
 * member 3: the flag indicating whereas the event is aligned with a space
 * member 4: the current meter signature
 * member 5: the current mensur
 * member 6: the functor for redirection
 **/

class FindSpaceInAlignmentParams : public FunctorParams {
public:
    FindSpaceInAlignmentParams(MeterSig *meterSig, Mensur *mensur, Functor *functor)
    {
        m_time = 0.0;
        m_duration = 0.0;
        m_layerCount = 1;
        m_success = false;
        m_meterSig = meterSig;
        m_mensur = mensur;
        m_functor = functor;
    }
    double m_time;
    double m_duration;
    int m_layerCount;
    bool m_success;
    MeterSig *m_meterSig;
    Mensur *m_mensur;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// FindTimeSpanningLayerElementsParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the vector of LayerElement pointer to fill
 * member 1: the minimum position
 * member 2: the maximum position
 **/

class FindTimeSpanningLayerElementsParams : public FunctorParams {
public:
    FindTimeSpanningLayerElementsParams()
    {
        m_minPos = 0;
        m_maxPos = 0;
    }
    std::vector<LayerElement *> m_spanningContent;
    int m_minPos;
    int m_maxPos;
};

//----------------------------------------------------------------------------
// GenerateMIDIParams
//----------------------------------------------------------------------------

/**
 * member 0: MidiFile*: the MidiFile we are writing to
 * member 1: int: the midi track number
 * member 3: double: the score time from the start of the music to the start of the current measure
 * member 4: int: the semi tone transposition for the current track
 * member 5: int with the current tempo
 **/

class GenerateMIDIParams : public FunctorParams {
public:
    GenerateMIDIParams(MidiFile *midiFile)
    {
        m_midiFile = midiFile;
        m_midiTrack = 1;
        m_totalTime = 0.0;
        m_transSemi = 0;
        m_currentTempo = 120;
    }
    MidiFile *m_midiFile;
    int m_midiTrack;
    double m_totalTime;
    int m_transSemi;
    int m_currentTempo;
};

//----------------------------------------------------------------------------
// GenerateTimemapParams
//----------------------------------------------------------------------------

/**
 * member 0: mapping of real times to score times
 * member 1: mapping of real times to elements which should be highlighted at time
 * member 2: mapping of real times to elements which should be unhighlighted at time
 * member 3: mapping of real times to tempos
 * member 4: Score time from the start of the piece to previous barline in quarter notes
 * member 5: Real time from the start of the piece to previous barline in ms
 * member 6: Currently active tempo
 **/

class GenerateTimemapParams : public FunctorParams {
public:
    GenerateTimemapParams()
    {
        m_scoreTimeOffset = 0.0;
        m_realTimeOffsetMilliseconds = 0;
        m_currentTempo = 120;
    }
    std::map<int, double> realTimeToScoreTime;
    std::map<int, std::vector<std::string> > realTimeToOnElements;
    std::map<int, std::vector<std::string> > realTimeToOffElements;
    std::map<int, int> realTimeToTempo;
    double m_scoreTimeOffset;
    int m_realTimeOffsetMilliseconds;
    int m_currentTempo;
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
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// JustifyXParams
//----------------------------------------------------------------------------

/**
 * member 0: the justification ratio
 * member 1: the justification ratio for the measure (depends on the margin)
 * member 2: the non justifiable margin
 * member 3: the system full width (without system margins)
 * member 4: the functor to be redirected to the MeasureAligner
 **/

class JustifyXParams : public FunctorParams {
public:
    JustifyXParams(Functor *functor)
    {
        m_measureXRel = 0;
        m_justifiableRatio = 1.0;
        m_leftBarLineX = 0;
        m_rightBarLineX = 0;
        m_systemFullWidth = 0;
        m_functor = functor;
    }
    int m_measureXRel;
    double m_justifiableRatio;
    int m_leftBarLineX;
    int m_rightBarLineX;
    int m_systemFullWidth;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// PrepareBoundariesParams
//----------------------------------------------------------------------------

/**
 * member 0: the last measure
 * member 1: the current boundary
 **/

class PrepareBoundariesParams : public FunctorParams {
public:
    PrepareBoundariesParams()
    {
        m_lastMeasure = NULL;
        m_currentEnding = NULL;
    }
    Measure *m_lastMeasure;
    Ending *m_currentEnding;
    std::vector<BoundaryStartInterface *> m_startBoundaries;
};

//----------------------------------------------------------------------------
// PrepareFloatingGrpsParams
//----------------------------------------------------------------------------

/**
 * member 0: the previous ending
 * member 1: the current grpId
 **/

class PrepareFloatingGrpsParams : public FunctorParams {
public:
    PrepareFloatingGrpsParams()
    {
        m_previousEnding = NULL;
        m_drawingGrpId = DRAWING_GRP_OTHER;
    }
    Ending *m_previousEnding;
    int m_drawingGrpId;
};

//----------------------------------------------------------------------------
// PrepareCrossStaffParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the current measure
 **/

class PrepareCrossStaffParams : public FunctorParams {
public:
    PrepareCrossStaffParams()
    {
        m_currentMeasure = NULL;
        m_currentCrossStaff = NULL;
        m_currentCrossLayer = NULL;
    }
    Measure *m_currentMeasure;
    Staff *m_currentCrossStaff;
    Layer *m_currentCrossLayer;
};

//----------------------------------------------------------------------------
// PrepareLyricsParams
//----------------------------------------------------------------------------

/**
 * member 0: the current Syl
 * member 1: the last Note
 **/

class PrepareLyricsParams : public FunctorParams {
public:
    PrepareLyricsParams()
    {
        m_currentSyl = NULL;
        m_lastNote = NULL;
        m_lastButOneNote = NULL;
    }
    Syl *m_currentSyl;
    Note *m_lastNote;
    Note *m_lastButOneNote;
};

//----------------------------------------------------------------------------
// PreparePlistParams
//----------------------------------------------------------------------------

/**
 * member 0: ArrayOfInterfaceUuidPairs holds the interface / uuid pairs to match
 * member 1: bool* fillList for indicating whether the pairs have to be stacked or not
 **/

class PreparePlistParams : public FunctorParams {
public:
    PreparePlistParams() { m_fillList = true; }
    ArrayOfInterfaceUuidPairs m_interfaceUuidPairs;
    bool m_fillList;
};

//----------------------------------------------------------------------------
// PreparePointersByLayerParams
//----------------------------------------------------------------------------

/**
 * member 0: the current Note
 **/

class PreparePointersByLayerParams : public FunctorParams {
public:
    PreparePointersByLayerParams() { m_currentNote = NULL; }
    Note *m_currentNote;
};

//----------------------------------------------------------------------------
// PrepareProcessingListsParams
//----------------------------------------------------------------------------

/**
 * member 0: the IntTree* for staff/layer/verse
 * member 1: the IntTree* for staff/layer
 **/

class PrepareProcessingListsParams : public FunctorParams {
public:
    PrepareProcessingListsParams() {}
    IntTree m_verseTree;
    IntTree m_layerTree;
};

//----------------------------------------------------------------------------
// PrepareRptParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the current MRpt pointer
 * member 1: a pointer to the data_BOOLEAN indicating if multiNumber
 * member 2: a pointer to the doc scoreDef
 **/

class PrepareRptParams : public FunctorParams {
public:
    PrepareRptParams(ScoreDef *currentScoreDef)
    {
        m_currentMRpt = NULL;
        m_multiNumber = BOOLEAN_NONE;
        m_currentScoreDef = currentScoreDef;
    }
    MRpt *m_currentMRpt;
    data_BOOLEAN m_multiNumber;
    ScoreDef *m_currentScoreDef;
};

//----------------------------------------------------------------------------
// PrepareTieAttrParams
//----------------------------------------------------------------------------

/**
 * member 0: std::vector<Note*>* that holds the current notes with open ties
 * member 1: Chord** currentChord for the current chord if in a chord
 **/

class PrepareTieAttrParams : public FunctorParams {
public:
    PrepareTieAttrParams() { m_currentChord = NULL; }

    std::vector<Note *> m_currentNotes;
    Chord *m_currentChord;
};

//----------------------------------------------------------------------------
// PrepareTimePointingParams
//----------------------------------------------------------------------------

/**
 * member 0: std::vector< Object*>* that holds the current elements to match
 **/

class PrepareTimePointingParams : public FunctorParams {
public:
    PrepareTimePointingParams() {}
    ArrayOfPointingInterClassIdPairs m_timePointingInterfaces;
};

//----------------------------------------------------------------------------
// PrepareTimeSpanningParams
//----------------------------------------------------------------------------

/**
 * member 0: std::vector< Object*>* that holds the current elements to match
 * member 1: bool* fillList for indicating whether the elements have to be stacked or not
 **/

class PrepareTimeSpanningParams : public FunctorParams {
public:
    PrepareTimeSpanningParams() { m_fillList = true; }
    ArrayOfSpanningInterClassIdPairs m_timeSpanningInterfaces;
    bool m_fillList;
};

//----------------------------------------------------------------------------
// PrepareTimestampsParams
//----------------------------------------------------------------------------

/**
 * member 0: std::vector< Object*>* that holds the current elements to match
 * member 1:  ArrayOfObjectBeatPairs* that holds the tstamp2 elements for attach to the end measure
 **/

class PrepareTimestampsParams : public FunctorParams {
public:
    PrepareTimestampsParams() {}
    ArrayOfSpanningInterClassIdPairs m_timeSpanningInterfaces;
    ArrayOfObjectBeatPairs m_tstamps;
};

//----------------------------------------------------------------------------
// ReplaceDrawingValuesInStaffDefParams
//----------------------------------------------------------------------------

/**
 * member 0: Clef pointer (NULL if none)
 * member 1: KeySig pointer (NULL if none)
 * member 2: Mensur pointer (NULL if none)
 * member 3: MeterSig pointer (NULL if none)
 **/

class ReplaceDrawingValuesInStaffDefParams : public FunctorParams {
public:
    ReplaceDrawingValuesInStaffDefParams(Clef *clef, KeySig *keySig, Mensur *mensur, MeterSig *meterSig)
    {
        m_clef = clef;
        m_keySig = keySig;
        m_mensur = mensur;
        m_meterSig = meterSig;
    }
    Clef *m_clef;
    KeySig *m_keySig;
    Mensur *m_mensur;
    MeterSig *m_meterSig;
};

//----------------------------------------------------------------------------
// SaveParams
//----------------------------------------------------------------------------

/**
 * member 0: output stream
 **/

class SaveParams : public FunctorParams {
public:
    SaveParams(FileOutputStream *output) { m_output = output; }
    FileOutputStream *m_output;
};

//----------------------------------------------------------------------------
// SetAlignmentPitchPosParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer doc
 **/

class SetAlignmentPitchPosParams : public FunctorParams {
public:
    SetAlignmentPitchPosParams(Doc *doc) { m_doc = doc; }
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// SetAlignmentXPosParams
//----------------------------------------------------------------------------

/**
 * member 0: the previous time position
 * member 1: the previous x rel position
 * member 2: duration of the longest note
 * member 3: the doc
 * member 4: the functor to be redirected to Aligner
 **/

class SetAlignmentXPosParams : public FunctorParams {
public:
    SetAlignmentXPosParams(Doc *doc, Functor *functor)
    {
        m_previousTime = 0.0;
        m_previousXRel = 0;
        m_longestActualDur = 0;
        m_doc = doc;
        m_functor = functor;
    }
    double m_previousTime;
    int m_previousXRel;
    int m_longestActualDur;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// SetCautionaryScoreDefParams
//----------------------------------------------------------------------------

/**
 * member 0: the current scoreDef
 * member 1: the current staffDef
 **/

class SetCautionaryScoreDefParams : public FunctorParams {
public:
    SetCautionaryScoreDefParams(ScoreDef *currentScoreDef)
    {
        m_currentScoreDef = currentScoreDef;
        m_currentStaffDef = NULL;
    }
    ScoreDef *m_currentScoreDef;
    StaffDef *m_currentStaffDef;
};

//----------------------------------------------------------------------------
// SetCurrentScoreDefParams
//----------------------------------------------------------------------------

/**
 * member 0: the current scoreDef
 * member 1: the current staffDef
 * member 2: the upcoming scoreDef
 * member 3: the previous measure (for setting cautionary scoreDef)
 * member 4: the current system (for setting the system scoreDef)
 * member 5: the flag indicating whereas full labels have to be drawn
 * member 6: the doc
 **/

class SetCurrentScoreDefParams : public FunctorParams {
public:
    SetCurrentScoreDefParams(Doc *doc, ScoreDef *upcomingScoreDef)
    {
        m_currentScoreDef = NULL;
        m_currentStaffDef = NULL;
        m_upcomingScoreDef = upcomingScoreDef;
        m_previousMeasure = NULL;
        m_currentSystem = NULL;
        m_drawLabels = false;
        m_doc = doc;
    }
    ScoreDef *m_currentScoreDef;
    StaffDef *m_currentStaffDef;
    ScoreDef *m_upcomingScoreDef;
    Measure *m_previousMeasure;
    System *m_currentSystem;
    bool m_drawLabels;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// SetOverflowBBoxesParams
//----------------------------------------------------------------------------

/**
 * member 0: the current staffAlignment
 * member 1: the doc
 **/

class SetOverflowBBoxesParams : public FunctorParams {
public:
    SetOverflowBBoxesParams(Doc *doc)
    {
        m_staffAlignment = NULL;
        m_doc = doc;
    }
    StaffAlignment *m_staffAlignment;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// SetStaffDefRedrawFlagsParams
//----------------------------------------------------------------------------

/**
 * member 0: bool clef flag
 * member 1: bool keysig flag
 * member 2: bool mensur flag
 * member 3: bool meterSig flag
 * member 4: bool the flag for indicating if apply to all or not
 **/

class SetStaffDefRedrawFlagsParams : public FunctorParams {
public:
    SetStaffDefRedrawFlagsParams()
    {
        m_clef = false;
        m_keySig = false;
        m_mensur = false;
        m_meterSig = false;
        m_applyToAll = false;
    }
    bool m_clef;
    bool m_keySig;
    bool m_mensur;
    bool m_meterSig;
    bool m_applyToAll;
};

//----------------------------------------------------------------------------
// UnCastOffParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the system we are adding system to
 **/

class UnCastOffParams : public FunctorParams {
public:
    UnCastOffParams(System *currentSystem) { m_currentSystem = currentSystem; }
    System *m_currentSystem;
};

} // namespace vrv

#endif
