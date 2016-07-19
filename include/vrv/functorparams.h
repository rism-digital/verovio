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
class Chord;
class Clef;
class EndingBoundary;
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

// param 0: the ListOfObjects

class AddLayerElementToFlatListParams : public FunctorParams {
public:
    AddLayerElementToFlatListParams() { m_flatList = NULL; }
    ListOfObjects *m_flatList;
};

class AdjustFloatingPostionersParams : public FunctorParams {
public:
    AdjustFloatingPostionersParams()
    {
        m_classId = OBJECT;
        m_doc = NULL;
        m_functor = NULL;
    }
    ClassId m_classId;
    Doc *m_doc;
    Functor *m_functor;
};

// param 0: the measureAligner (unused)
// param 1: the time
// param 2: the current Mensur
// param 3: the current MeterSig
// param 4: the functor for passing it to the TimeStampAligner (unused)

class AlignHorizontallyParams : public FunctorParams {
public:
    AlignHorizontallyParams()
    {
        m_measureAligner = NULL;
        m_time = 0.0;
        m_currentMensur = NULL;
        m_currentMeterSig = NULL;
        m_functor = NULL;
    }
    MeasureAligner *m_measureAligner;
    double m_time;
    Mensur *m_currentMensur;
    MeterSig *m_currentMeterSig;
    Functor *m_functor;
};

// param 0: the cumulated shift
// param 1: the cumulated justifiable width

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

// param 0: the cumulated shift
// param 1: the system margin

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

// param 0: the systemAligner (unused)
// param 1: the staffIdx
// param 2: the staffN (unused)
// param 3: the doc (unused)

class AlignVerticallyParams : public FunctorParams {
public:
    AlignVerticallyParams()
    {
        m_systemAligner = NULL;
        m_staffIdx = 0;
        m_staffN = 0;
        m_doc = NULL;
    }
    SystemAligner *m_systemAligner;
    int m_staffIdx;
    int m_staffN;
    Doc *m_doc;
};

class CalcMaxMeasureDurationParams : public FunctorParams {
public:
    CalcMaxMeasureDurationParams() { m_currentValue = 0.0; }
    std::vector<double> m_maxValues;
    double m_currentValue;
};

class CalcStaffOverlapParams : public FunctorParams {
public:
    CalcStaffOverlapParams()
    {
        m_previous = NULL;
        m_functor = NULL;
    }
    StaffAlignment *m_previous;
    Functor *m_functor;
};

// param 0: a pointer to the page we are taking the content from
// param 1: a pointer the document we are adding pages to
// param 2: a pointer to the current page
// param 3: the cummulated shift (m_drawingYRel of the first system of the current page)
// param 4: the page height

class CastOffPagesParams : public FunctorParams {
public:
    CastOffPagesParams()
    {
        m_contentPage = NULL;
        m_doc = NULL;
        m_currentPage = NULL;
        m_shift = 0;
        m_pageHeight = 0;
    }
    Page *m_contentPage;
    Doc *m_doc;
    Page *m_currentPage;
    int m_shift;
    int m_pageHeight;
};

// param 0: a pointer to the system we are taking the content from
// param 1: a pointer the page we are adding system to
// param 2: a pointer to the current system
// param 3: the cummulated shift (m_drawingXRel of the first measure of the current system)
// param 4: the system width
// param 5: the current scoreDef width

class CastOffSystemsParams : public FunctorParams {
public:
    CastOffSystemsParams()
    {
        m_contentSystem = NULL;
        m_page = NULL;
        m_currentSystem = NULL;
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
};

class FillStaffCurrentTimeSpanningParams : public FunctorParams {
public:
    FillStaffCurrentTimeSpanningParams() {}
    std::vector<Object *> m_timeSpanningElements;
};

// param 0: the attComparision text
// param 1: an array of all matching objects

class FindAllByAttComparisonParams : public FunctorParams {
public:
    FindAllByAttComparisonParams()
    {
        m_attComparison = NULL;
        m_elements = NULL;
    }
    AttComparison *m_attComparison;
    ArrayOfObjects *m_elements;
};

// param 0: the attComparision text
// param 1: the pointer to pointer to the Object

class FindByAttComparisonParams : public FunctorParams {
public:
    FindByAttComparisonParams()
    {
        m_attComparison = NULL;
        m_element = NULL;
    }
    AttComparison *m_attComparison;
    Object *m_element;
};

// param 0: the uuid we are looking for
// param 1: the pointer to pointer to the Object

class FindByUuidParams : public FunctorParams {
public:
    FindByUuidParams() { m_element = NULL; }
    std::string m_uuid;
    Object *m_element;
};

// param 0: the attComparision text
// param 1: the pointer to pointer to the Object

class FindExtremeByAttComparisonParams : public FunctorParams {
public:
    FindExtremeByAttComparisonParams()
    {
        m_attComparison = NULL;
        m_element = NULL;
    }
    AttComparison *m_attComparison;
    Object *m_element;
};

// param 0: MidiFile*: the MidiFile we are writing to
// param 1: int*: the midi track number
// param 2: int*: the current time in the measure (incremented by each element)
// param 3: int*: the current total measure time (incremented by each measure
// param 4: std::vector<double>: a stack of maximum duration filled by the functor (unused)
// param 5: int* the semi tone transposition for the current track

class GenerateMIDIParams : public FunctorParams {
public:
    GenerateMIDIParams()
    {
        m_midiFile = NULL;
        m_midiTrack = 0;
        m_currentMeasureTime = 0.0;
        m_totalTime = 0.0;
        m_transSemi = 0;
    }
    MidiFile *m_midiFile;
    int m_midiTrack;
    double m_currentMeasureTime;
    double m_totalTime;
    std::vector<double> m_maxValues;
    int m_transSemi;
};

// param 0: the functor to be redirected to Aligner (unused)

class IntegrateBoundingBoxGraceXShiftParams : public FunctorParams {
public:
    IntegrateBoundingBoxGraceXShiftParams() { m_functor = NULL; }
    Functor *m_functor;
};

class IntegrateBoundingBoxXShiftParams : public FunctorParams {
public:
    IntegrateBoundingBoxXShiftParams()
    {
        m_shift = 0;
        m_doc = NULL;
        m_functor = NULL;
    }
    int m_shift;
    Doc *m_doc;
    Functor *m_functor;
};

class IntegrateBoundingBoxYShiftParams : public FunctorParams {
public:
    IntegrateBoundingBoxYShiftParams()
    {
        m_shift = 0;
        m_functor = NULL;
    }
    int m_shift;
    Functor *m_functor;
};

class JustifyXParams : public FunctorParams {
public:
    JustifyXParams()
    {
        m_measureXRel = 0;
        m_justifiableRatio = 1.0;
        m_leftBarLineX = 0;
        m_rightBarLineX = 0;
        m_systemFullWidth = 0;
        m_functor = NULL;
    }
    int m_measureXRel;
    double m_justifiableRatio;
    int m_leftBarLineX;
    int m_rightBarLineX;
    int m_systemFullWidth;
    Functor *m_functor;
};

class PrepareEndingsParams : public FunctorParams {
public:
    PrepareEndingsParams()
    {
        m_lastMeasure = NULL;
        m_currentEnding = NULL;
    }
    Measure *m_lastMeasure;
    EndingBoundary *m_currentEnding;
};

// param 0: the current Syl
// param 1: the last Note

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

class PrepareRptParams : public FunctorParams {
public:
    PrepareRptParams()
    {
        m_currentMRpt = NULL;
        m_multiNumber = BOOLEAN_NONE;
        m_currentScoreDef = NULL;
    }
    MRpt *m_currentMRpt;
    data_BOOLEAN m_multiNumber;
    ScoreDef *m_currentScoreDef;
};

class PreparePointersByLayerParams : public FunctorParams {
public:
    PreparePointersByLayerParams() { m_currentNote = NULL; }
    Note *m_currentNote;
};

class PrepareProcessingListsParams : public FunctorParams {
public:
    PrepareProcessingListsParams() {}
    IntTree m_verseTree;
    IntTree m_layerTree;
};

// param 0: std::vector<Note*>* that holds the current notes with open ties
// param 1: Chord** currentChord for the current chord if in a chord

class PrepareTieAttrParams : public FunctorParams {
public:
    PrepareTieAttrParams() { m_currentChord = NULL; }

    std::vector<Note *> m_currentNotes;
    Chord *m_currentChord;
};

class PrepareTimeSpanningParams : public FunctorParams {
public:
    PrepareTimeSpanningParams() { m_fillList = true; }
    ArrayOfInterfaceClassIdPairs m_timeSpanningInterfaces;
    bool m_fillList;
};

class PrepareTimestampsParams : public FunctorParams {
public:
    PrepareTimestampsParams() {}
    ArrayOfInterfaceClassIdPairs m_timeSpanningInterfaces;
    ArrayOfObjectBeatPairs m_tstamps;
};

// param 0: Clef pointer (NULL if none)
// param 1: KeySig pointer (NULL if none)
// param 2: Mensur pointer (NULL if none)
// param 3: MeterSig pointer (NULL if none)

class ReplaceDrawingValuesInStaffDefParams : public FunctorParams {
public:
    ReplaceDrawingValuesInStaffDefParams()
    {
        m_clef = NULL;
        m_keySig = NULL;
        m_mensur = NULL;
        m_meterSig = NULL;
    }
    Clef *m_clef;
    KeySig *m_keySig;
    Mensur *m_mensur;
    MeterSig *m_meterSig;
};

// param 0: output stream

class SaveParams : public FunctorParams {
public:
    SaveParams() { m_output = NULL; }
    FileOutputStream *m_output;
};

class SetAlignmentXPosParams : public FunctorParams {
public:
    SetAlignmentXPosParams()
    {
        m_previousTime = 0.0;
        m_previousXRel = 0;
        m_longestActualDur = 0;
        m_doc = NULL;
        m_functor = NULL;
    }
    double m_previousTime;
    int m_previousXRel;
    int m_longestActualDur;
    Doc *m_doc;
    Functor *m_functor;
};

class SetAligmentYPosParams : public FunctorParams {
public:
    SetAligmentYPosParams()
    {
        m_previousStaffHeight = 0;
        m_previousOverflowBelow = 0;
        m_previousVerseCount = 0;
        m_doc = NULL;
        m_functor = NULL;
    }
    int m_previousStaffHeight;
    int m_previousOverflowBelow;
    int m_previousVerseCount;
    Doc *m_doc;
    Functor *m_functor;
};

// param 0: the minimum position (i.e., the width of the previous element)
// param 1: the Doc

class SetBoundingBoxGraceXShiftParams : public FunctorParams {
public:
    SetBoundingBoxGraceXShiftParams()
    {
        m_graceMinPos = 0;
        m_doc = NULL;
    }

    int m_graceMinPos;
    Doc *m_doc;
};

class SetBoundingBoxXShiftParams : public FunctorParams {
public:
    SetBoundingBoxXShiftParams()
    {
        m_minPos = 0;
        m_layerMinPos = 0;
        m_measureWidth = 0;
        m_doc = NULL;
        m_functor = NULL;
        m_functorEnd = NULL;
    }
    int m_minPos;
    int m_layerMinPos;
    int m_measureWidth;
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
};

class SetCurrentScoreDefParams : public FunctorParams {
public:
    SetCurrentScoreDefParams()
    {
        m_currentScoreDef = NULL;
        m_currentStaffDef = NULL;
        m_upcomingScoreDef = NULL;
    }
    ScoreDef *m_currentScoreDef;
    StaffDef *m_currentStaffDef;
    ScoreDef *m_upcomingScoreDef;
};

// param 0: a pointer doc (unused)
// param 1: a pointer to the current system (unused)
// param 2: a pointer to the current measure
// param 3: a pointer to the current staff (unused)
// param 4: a pointer to the current layer
// param 5: a pointer to the view (unused)
// param 6: a bool indicating if we are processing layer elements or not
// param 7: a pointer to the functor for passing it to the timestamps (unused)

class SetDrawingXYParams : public FunctorParams {
public:
    SetDrawingXYParams()
    {
        m_doc = NULL;
        m_currentSystem = NULL;
        m_currentMeasure = NULL;
        m_currentStaff = NULL;
        m_currentLayer = NULL;
        m_view = NULL;
        m_processLayerElements = false;
        m_functor = NULL;
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

// param 0: the current staffAlignment
// param 1: the doc

class SetOverflowBBoxesParams : public FunctorParams {
public:
    SetOverflowBBoxesParams()
    {
        m_staffAlignment = NULL;
        m_doc = NULL;
    }
    StaffAlignment *m_staffAlignment;
    Doc *m_doc;
};

// param 0: bool clef flag
// param 1: bool keysig flag
// param 2: bool mensur flag
// param 3: bool meterSig flag
// param 4: bool keySig cancellation flag
// param 5: bool the flag for indicating if apply to all or not

class SetStaffDefRedrawFlagsParams : public FunctorParams {
public:
    SetStaffDefRedrawFlagsParams()
    {
        m_clef = false;
        m_keySig = false;
        m_mensur = false;
        m_meterSig = false;
        m_keySigCancellation = false;
        m_applyToAll = false;
    }
    bool m_clef;
    bool m_keySig;
    bool m_mensur;
    bool m_meterSig;
    bool m_keySigCancellation;
    bool m_applyToAll;
};

class TimeSpanningLayerElementsParams : public FunctorParams {
public:
    TimeSpanningLayerElementsParams()
    {
        m_minPos = 0;
        m_maxPos = 0;
    }
    std::vector<LayerElement *> m_spanningContent;
    int m_minPos;
    int m_maxPos;
};

// param 0: a pointer to the system we are adding system to

class UnCastOffParams : public FunctorParams {
public:
    UnCastOffParams() { m_currentSystem = NULL; }
    System *m_currentSystem;
};

} // namespace vrv

#endif
