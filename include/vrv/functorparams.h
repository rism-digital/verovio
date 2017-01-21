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
class View;

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
// AdjustArticParams
//----------------------------------------------------------------------------

/**
 * member 0: the Doc
 **/

class AdjustArticParams : public FunctorParams {
public:
    AdjustArticParams(Doc *doc) { m_doc = doc; }
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AdjustArticWithSlursParams
//----------------------------------------------------------------------------

/**
 * member 0: the Doc
 **/

class AdjustArticWithSlursParams : public FunctorParams {
public:
    AdjustArticWithSlursParams(Doc *doc) { m_doc = doc; }
    Doc *m_doc;
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
    }
    MeasureAligner *m_measureAligner;
    double m_time;
    Mensur *m_currentMensur;
    MeterSig *m_currentMeterSig;
    Functor *m_functor;
    ElementScoreDefRole m_scoreDefRole;
    bool m_isFirstMeasure;
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
 * member 3: the doc
**/

class AlignVerticallyParams : public FunctorParams {
public:
    AlignVerticallyParams(Doc *doc)
    {
        m_systemAligner = NULL;
        m_staffIdx = 0;
        m_staffN = 0;
        m_doc = doc;
    }
    SystemAligner *m_systemAligner;
    int m_staffIdx;
    int m_staffN;
    Doc *m_doc;
};

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
        m_currentValue = 0.0;
        m_currentBpm = 120;
    }
    std::vector<double> m_maxValues;
    double m_currentValue;
    int m_currentBpm;
};

//----------------------------------------------------------------------------
// CalcStaffOverlapParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the previous staff alignment
 * member 1: a pointer to the functor for passing it to the system aligner
**/

class CalcStaffOverlapParams : public FunctorParams {
public:
    CalcStaffOverlapParams(Functor *functor)
    {
        m_previous = NULL;
        m_functor = functor;
    }
    StaffAlignment *m_previous;
    Functor *m_functor;
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
 * member 1: int*: the midi track number
 * member 2: int*: the current time in the measure (incremented by each element)
 * member 3: int*: the current total measure time (incremented by each measure
 * member 4: std::vector<double>: a stack of maximum duration filled by the functor
 * member 5: int* the semi tone transposition for the current track
 * member 6: int with the current bpm
**/

class GenerateMIDIParams : public FunctorParams {
public:
    GenerateMIDIParams(MidiFile *midiFile)
    {
        m_midiFile = midiFile;
        m_midiTrack = 1;
        m_currentMeasureTime = 0.0;
        m_totalTime = 0.0;
        m_transSemi = 0;
        m_currentBpm = 120;
    }
    MidiFile *m_midiFile;
    int m_midiTrack;
    double m_currentMeasureTime;
    double m_totalTime;
    std::vector<double> m_maxValues;
    int m_transSemi;
    int m_currentBpm;
};

//----------------------------------------------------------------------------
// IntegrateBoundingBoxGraceXShiftParams
//----------------------------------------------------------------------------

/**
 * member 0: the functor to be redirected to Aligner
**/

class IntegrateBoundingBoxGraceXShiftParams : public FunctorParams {
public:
    IntegrateBoundingBoxGraceXShiftParams(Functor *functor) { m_functor = functor; }
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// IntegrateBoundingBoxXShiftParams
//----------------------------------------------------------------------------

/**
 * member 0: the cumulated shift
 * member 3: the doc for accessing drawing parameters
 * member 4: the functor to be redirected to Aligner
**/

class IntegrateBoundingBoxXShiftParams : public FunctorParams {
public:
    IntegrateBoundingBoxXShiftParams(Doc *doc, Functor *functor)
    {
        m_shift = 0;
        m_doc = doc;
        m_functor = functor;
    }
    int m_shift;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// IntegrateBoundingBoxYShiftParams
//----------------------------------------------------------------------------

/**
 * member 0: the cumulated shift
 * member 1: the functor to be redirected to SystemAligner
**/

class IntegrateBoundingBoxYShiftParams : public FunctorParams {
public:
    IntegrateBoundingBoxYShiftParams(Functor *functor)
    {
        m_shift = 0;
        m_functor = functor;
    }
    int m_shift;
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
// SetAligmentYPosParams
//----------------------------------------------------------------------------

/**
 * member 0: the previous staff height
 * member 1: the extra staff height
 * member 2  the previous verse count
 * member 3: the doc
 * member 4: the functor to be redirected to SystemAligner
**/

class SetAligmentYPosParams : public FunctorParams {
public:
    SetAligmentYPosParams(Doc *doc, Functor *functor)
    {
        m_previousStaffHeight = 0;
        m_previousOverflowBelow = 0;
        m_previousVerseCount = 0;
        m_doc = doc;
        m_functor = functor;
    }
    int m_previousStaffHeight;
    int m_previousOverflowBelow;
    int m_previousVerseCount;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// SetBoundingBoxGraceXShiftParams
//----------------------------------------------------------------------------

/**
 * member 0: the minimum position (i.e., the width of the previous element)
 * member 1: the Doc
**/

class SetBoundingBoxGraceXShiftParams : public FunctorParams {
public:
    SetBoundingBoxGraceXShiftParams(Doc *doc)
    {
        m_graceMinPos = 0;
        m_doc = doc;
    }

    int m_graceMinPos;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// SetBoundingBoxXShiftParams
//----------------------------------------------------------------------------

/**
 * member 0: the minimum position (i.e., the width of the previous element)
 * member 1: the maximum width in the current measure
 * member 2: the Doc
**/

class SetBoundingBoxXShiftParams : public FunctorParams {
public:
    SetBoundingBoxXShiftParams(Doc *doc, Functor *functor, Functor *functorEnd)
    {
        m_minPos = 0;
        m_layerMinPos = 0;
        m_measureWidth = 0;
        m_doc = doc;
        m_functor = functor;
        m_functorEnd = functorEnd;
    }
    int m_minPos;
    int m_layerMinPos;
    int m_measureWidth;
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
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
**/

class SetCurrentScoreDefParams : public FunctorParams {
public:
    SetCurrentScoreDefParams(ScoreDef *upcomingScoreDef)
    {
        m_currentScoreDef = NULL;
        m_currentStaffDef = NULL;
        m_upcomingScoreDef = upcomingScoreDef;
        m_previousMeasure = NULL;
        m_currentSystem = NULL;
        m_drawLabels = false;
    }
    ScoreDef *m_currentScoreDef;
    StaffDef *m_currentStaffDef;
    ScoreDef *m_upcomingScoreDef;
    Measure *m_previousMeasure;
    System *m_currentSystem;
    bool m_drawLabels;
};

//----------------------------------------------------------------------------
// SetDrawingXYParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer doc
 * member 1: a pointer to the current system
 * member 2: a pointer to the current measure
 * member 3: a pointer to the current staff
 * member 4: a pointer to the current layer
 * member 5: a pointer to the view
 * member 6: a bool indicating if we are processing layer elements or not
 * member 7: a pointer to the functor for passing it to the timestamps
**/

class SetDrawingXYParams : public FunctorParams {
public:
    SetDrawingXYParams(Doc *doc, View *view, Functor *functor)
    {
        m_doc = doc;
        m_currentSystem = NULL;
        m_currentMeasure = NULL;
        m_currentStaff = NULL;
        m_currentLayer = NULL;
        m_view = view;
        m_processLayerElements = false;
        m_functor = functor;
    }
    Doc *m_doc;
    System *m_currentSystem;
    Measure *m_currentMeasure;
    Staff *m_currentStaff;
    Layer *m_currentLayer;
    View *m_view;
    bool m_processLayerElements;
    Functor *m_functor;
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
