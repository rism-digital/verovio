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

namespace smf {
class MidiFile;
}

namespace vrv {

class ClassIdComparison;
class BoundaryStartInterface;
class Chord;
class Clef;
class Doc;
class Dot;
class Dots;
class Dynam;
class Ending;
class Output;
class Facsimile;
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
class MRpt;
class Object;
class Page;
class Pedal;
class ScoreDef;
class Slur;
class Staff;
class StaffAlignment;
class StaffDef;
class StemmedDrawingInterface;
class Syl;
class System;
class SystemAligner;
class Transposer;
class TupletNum;
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
    AddLayerElementToFlatListParams(ArrayOfObjects *flatList) { m_flatList = flatList; }
    ArrayOfObjects *m_flatList;
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
 * member 0: the array of Alignement, Arpeg, StaffN, bool tuples
 * member 1: the MeasureAligner
 * member 2: the Functor to be redirected to MeasureAligner
 * member 3: the Doc
 **/

class AdjustArpegParams : public FunctorParams {
public:
    AdjustArpegParams(Doc *doc, Functor *functor)
    {
        m_measureAligner = NULL;
        m_doc = doc;
        m_functor = functor;
    }
    ArrayOfAligmentArpegTuples m_alignmentArpegTuples;
    MeasureAligner *m_measureAligner;
    Functor *m_functor;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AdjustBeamParams
//----------------------------------------------------------------------------

/**
 * member 0: the beam that should be adjusted
 * member 1: y coordinate of the beam left side
 * member 2: y coordinate of the beam right side
 * member 3: overlap margin that beam needs to be displaced by
 * member 4: the Doc
 **/

class AdjustBeamParams : public FunctorParams {
public:
    AdjustBeamParams(Doc *doc)
    {
        m_beam = NULL;
        m_y1 = 0;
        m_y2 = 0;
        m_directionBias = 0;
        m_overlapMargin = 0;
        m_doc = doc;
    }

    Object *m_beam;
    int m_y1;
    int m_y2;
    int m_directionBias;
    int m_overlapMargin;
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
    ArrayOfFloatingPositioners *m_previousStaffPositioners;
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
// AdjustHarmGrpsSpacingParams
//----------------------------------------------------------------------------

/**
 * member 0: a vector of the grpIds of harms in the system
 * member 1: the current grp id (0 for the first pass)
 * member 2: a vector adjustment tuples (Aligment start, Aligment end, distance)
 * member 3: a pointer to the previous harm positioner (if any)
 * member 4: a pointer to the previous harm start (if any)
 * member 5: a pointer to the previous measure (if any)
 * member 6: a pointer to the current system
 * member 7: the doc
 * member 8: a pointer to the functor for passing it to the system aligner
 * member 9: a pointer to the functor end for passing it to the system aligner
 **/

class AdjustHarmGrpsSpacingParams : public FunctorParams {
public:
    AdjustHarmGrpsSpacingParams(Doc *doc, Functor *functor, Functor *functorEnd)
    {
        m_doc = doc;
        m_functor = functor;
        m_functorEnd = functorEnd;
        m_currentGrp = 0;
        m_previousHarmPositioner = NULL;
        m_previousHarmStart = NULL;
        m_previousMeasure = NULL;
        m_currentSystem = NULL;
    }
    std::vector<int> m_grpIds;
    int m_currentGrp;
    ArrayOfAdjustmentTuples m_overlapingHarm;
    FloatingPositioner *m_previousHarmPositioner;
    LayerElement *m_previousHarmStart;
    Measure *m_previousMeasure;
    System *m_currentSystem;
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
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
 * member 8: flag whether element is in unison
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
        m_unison = false;
    }
    std::vector<int> m_staffNs;
    int m_currentLayerN;
    std::vector<LayerElement *> m_previous;
    std::vector<LayerElement *> m_current;
    Note *m_currentNote;
    Chord *m_currentChord;
    Doc *m_doc;
    Functor *m_functor;
    bool m_unison;
};

//----------------------------------------------------------------------------
// AdjustSlursParams
//----------------------------------------------------------------------------

/**
 * member 1: the doc
 * member 2: a pointer to the functor for passing it to the system aligner
 **/

class AdjustSlursParams : public FunctorParams {
public:
    AdjustSlursParams(Doc *doc, Functor *functor)
    {
        m_adjusted = false;
        m_doc = doc;
        m_functor = functor;
    }
    bool m_adjusted;
    Doc *m_doc;
    Functor *m_functor;
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
        m_previousVerse = NULL;
        m_lastSyl = NULL;
        m_previousMeasure = NULL;
        m_currentLabelAbbr = NULL;
        m_freeSpace = 0;
        m_staffSize = 100;
        m_doc = doc;
    }
    ArrayOfAdjustmentTuples m_overlapingSyl;
    Verse *m_previousVerse;
    Syl *m_lastSyl;
    Measure *m_previousMeasure;
    LabelAbbr *m_currentLabelAbbr;
    int m_freeSpace;
    int m_staffSize;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AdjustTupletNumOverlapParams
//----------------------------------------------------------------------------

/**
 * member 0: tupletNum relative position for which is being calculatied
 * member 1: drawing position of tupletNum
 * member 2: margin for tupletNum vertical overlap
 * member 3: flag to indicate whether cross-staff elements should be considered
 * member 4: resulting relative Y for the tupletNum
 **/
class AdjustTupletNumOverlapParams : public FunctorParams {
public:
    AdjustTupletNumOverlapParams(TupletNum *tupletNum)
    {
        m_tupletNum = tupletNum;
        m_drawingNumPos = STAFFREL_basic_NONE;
        m_verticalMargin = 0;
        m_ignoreCrossStaff = false;
        m_yRel = 0;
    }

    TupletNum *m_tupletNum;
    data_STAFFREL_basic m_drawingNumPos;
    int m_verticalMargin;
    bool m_ignoreCrossStaff;
    int m_yRel;
};

//----------------------------------------------------------------------------
// AdjustXOverflowParams
//----------------------------------------------------------------------------

/**
 * member 0: the current system
 * member 1: the last measure;
 * member 2: the current widest control event
 * member 3: the margin
 **/

class AdjustXOverflowParams : public FunctorParams {
public:
    AdjustXOverflowParams(int margin)
    {
        m_currentSystem = NULL;
        m_lastMeasure = NULL;
        m_currentWidest = NULL;
        m_margin = margin;
    }
    System *m_currentSystem;
    Measure *m_lastMeasure;
    FloatingPositioner *m_currentWidest;
    int m_margin;
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
// AlignHorizontallyParams
//----------------------------------------------------------------------------

/**
 * member 0: the measureAligner
 * member 1: the time
 * member 2: the current Mensur
 * member 3: the current MeterSig
 * member 4: the current notation type
 * member 5: the functor for passing it to the TimeStampAligner
 * member 6: a flag indicating whereas we are processing the caution scoreDef
 * member 7: a flag indicating is we are in the first measure (for the scoreDef role)
 * member 8: a flag indicating if we had mutliple layer alignment reference in the measure
 * member 9: the doc
 **/

class AlignHorizontallyParams : public FunctorParams {
public:
    AlignHorizontallyParams(Functor *functor, Doc *doc)
    {
        m_measureAligner = NULL;
        m_time = 0.0;
        m_currentMensur = NULL;
        m_currentMeterSig = NULL;
        m_notationType = NOTATIONTYPE_cmn;
        m_functor = functor;
        m_scoreDefRole = SCOREDEF_NONE;
        m_isFirstMeasure = false;
        m_hasMultipleLayer = false;
        m_doc = doc;
    }
    MeasureAligner *m_measureAligner;
    double m_time;
    Mensur *m_currentMensur;
    MeterSig *m_currentMeterSig;
    data_NOTATIONTYPE m_notationType;
    Functor *m_functor;
    ElementScoreDefRole m_scoreDefRole;
    bool m_isFirstMeasure;
    bool m_hasMultipleLayer;
    Doc *m_doc;
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
 * member 2: the overflow below of previous system
 * member 3: the sum of justification factors per page
 * member 4: the doc
 **/

class AlignSystemsParams : public FunctorParams {
public:
    AlignSystemsParams(Doc *doc)
    {
        m_shift = 0;
        m_systemMargin = 0;
        m_prevBottomOverflow = 0;
        m_justificationSum = 0.;
        m_doc = doc;
    }
    int m_shift;
    int m_systemMargin;
    int m_prevBottomOverflow;
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
// CalcLigatureNotePosParams
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
        m_tempoAdjustment = 1.0;
    }
    double m_currentScoreTime;
    double m_currentRealTimeSeconds;
    double m_maxCurrentScoreTime;
    double m_maxCurrentRealTimeSeconds;
    int m_currentTempo;
    double m_tempoAdjustment;
};

//----------------------------------------------------------------------------
// CalcOnsetOffset
//----------------------------------------------------------------------------

/**
 * member 0: double: the current score time in the measure (incremented by each element)
 * member 1: double: the current real time in seconds in the measure (incremented by each element)
 * member 2: the current Mensur
 * member 3: the current MeterSig
 * member 4: the current notation type
 * member 5: the current tempo
 **/

class CalcOnsetOffsetParams : public FunctorParams {
public:
    CalcOnsetOffsetParams()
    {
        m_currentScoreTime = 0.0;
        m_currentRealTimeSeconds = 0.0;
        m_currentMensur = NULL;
        m_currentMeterSig = NULL;
        m_notationType = NOTATIONTYPE_cmn;
        m_currentTempo = 120;
    }
    double m_currentScoreTime;
    double m_currentRealTimeSeconds;
    Mensur *m_currentMensur;
    MeterSig *m_currentMeterSig;
    data_NOTATIONTYPE m_notationType;
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
 * member 1: a pointer to the current page
 * member 2: a pointer to the current system
 * member 3: a pointer to the system we are taking the content from
 * member 4: a flag if we want to use the pageBreaks from the document
 **/

class CastOffEncodingParams : public FunctorParams {
public:
    CastOffEncodingParams(
        Doc *doc, Page *currentPage, System *currentSystem, System *contentSystem, bool usePages = true)
    {
        m_doc = doc;
        m_currentPage = currentPage;
        m_currentSystem = currentSystem;
        m_contentSystem = contentSystem;
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
        m_pgHeadHeight = 0;
        m_pgFootHeight = 0;
        m_pgHead2Height = 0;
        m_pgFoot2Height = 0;
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
 * member 7: the doc
 **/

class CastOffSystemsParams : public FunctorParams {
public:
    CastOffSystemsParams(System *contentSystem, Page *page, System *currentSystem, Doc *doc)
    {
        m_contentSystem = contentSystem;
        m_page = page;
        m_currentSystem = currentSystem;
        m_shift = 0;
        m_systemWidth = 0;
        m_currentScoreDefWidth = 0;
        m_doc = doc;
    }
    System *m_contentSystem;
    Page *m_page;
    System *m_currentSystem;
    int m_shift;
    int m_systemWidth;
    int m_currentScoreDefWidth;
    ArrayOfObjects m_pendingObjects;
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
// ConvertScoreDefMarkupParams
//----------------------------------------------------------------------------

/**
 * member 0: a flag indicating whereas the conversion is permanent of not
 **/

class ConvertScoreDefMarkupParams : public FunctorParams {
public:
    ConvertScoreDefMarkupParams(bool permanent) { m_permanent = permanent; }
    bool m_permanent;
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
    ConvertToPageBasedParams(System *pageBasedSystem) { m_pageBasedSystem = pageBasedSystem; }
    System *m_pageBasedSystem;
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
    FindAllBetweenParams(Comparison *comparison, ListOfObjects *elements, Object *start, Object *end)
    {
        m_comparison = comparison;
        m_elements = elements;
        m_start = start;
        m_end = end;
    }
    Comparison *m_comparison;
    ListOfObjects *m_elements;
    Object *m_start;
    Object *m_end;
};

//----------------------------------------------------------------------------
// FindAllReferencedObjectsParams
//----------------------------------------------------------------------------

/**
 * member 0: an array of all matching objects
 **/

class FindAllReferencedObjectsParams : public FunctorParams {
public:
    FindAllReferencedObjectsParams(ListOfObjects *elements) { m_elements = elements; }
    ListOfObjects *m_elements;
};

//----------------------------------------------------------------------------
// FindAllByComparisonParams
//----------------------------------------------------------------------------

/**
 * member 0: the attComparision text
 * member 1: an array of all matching objects
 **/

class FindAllByComparisonParams : public FunctorParams {
public:
    FindAllByComparisonParams(Comparison *comparison, ListOfObjects *elements)
    {
        m_comparison = comparison;
        m_elements = elements;
    }
    Comparison *m_comparison;
    ListOfObjects *m_elements;
};

//----------------------------------------------------------------------------
// FindByComparisonParams
//----------------------------------------------------------------------------

/**
 * member 0: the attComparision text
 * member 1: the pointer to pointer to the Object
 **/

class FindByComparisonParams : public FunctorParams {
public:
    FindByComparisonParams(Comparison *comparison)
    {
        m_comparison = comparison;
        m_element = NULL;
    }
    Comparison *m_comparison;
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
// FindChildByComparisonParams
//----------------------------------------------------------------------------

/**
 * member 0: the comparison
 * member 1: the start object
 */

class FindChildByComparisonParams : public FunctorParams {
public:
    FindChildByComparisonParams(Comparison *comparison, Object *start)
    {
        m_comparison = comparison;
        m_element = NULL;
        m_start = start;
    }
    Comparison *m_comparison;
    Object *m_element;
    Object *m_start;
};

//----------------------------------------------------------------------------
// FindExtremeByComparisonParams
//----------------------------------------------------------------------------

/**
 * member 0: the attComparision text
 * member 1: the pointer to pointer to the Object
 **/

class FindExtremeByComparisonParams : public FunctorParams {
public:
    FindExtremeByComparisonParams(Comparison *comparison)
    {
        m_comparison = comparison;
        m_element = NULL;
    }
    Comparison *m_comparison;
    Object *m_element;
};

//----------------------------------------------------------------------------
// FindSpannedLayerElementsParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the vector of LayerElement pointer to fill
 * member 1: the minimum position
 * member 2: the maximum position
 * member 3: the timespanning interface
 * member 4: the class Ids to keep
 * member 5: the slur for finding ties (too specific, to be refactored)
 * member 6: the ties we need to consider (too specific, to be refactored)
 **/

class FindSpannedLayerElementsParams : public FunctorParams {
public:
    FindSpannedLayerElementsParams(TimeSpanningInterface *interface, Slur *slur)
    {
        m_interface = interface;
        m_minPos = 0;
        m_maxPos = 0;
        m_slur = slur;
    }
    std::vector<LayerElement *> m_elements;
    int m_minPos;
    int m_maxPos;
    TimeSpanningInterface *m_interface;
    std::vector<ClassId> m_classIds;
    Slur *m_slur;
    std::vector<FloatingPositioner *> m_ties;
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
    GenerateMIDIParams(smf::MidiFile *midiFile, Functor *functor)
    {
        m_midiFile = midiFile;
        m_midiChannel = 0;
        m_midiTrack = 1;
        m_totalTime = 0.0;
        m_transSemi = 0;
        m_currentTempo = 120;
        m_functor = functor;
    }
    smf::MidiFile *m_midiFile;
    int m_midiChannel;
    int m_midiTrack;
    double m_totalTime;
    int m_transSemi;
    int m_currentTempo;
    Functor *m_functor;
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
    GenerateTimemapParams(Functor *functor)
    {
        m_scoreTimeOffset = 0.0;
        m_realTimeOffsetMilliseconds = 0;
        m_currentTempo = 120;
        m_functor = functor;
    }
    std::map<double, double> realTimeToScoreTime;
    std::map<double, std::vector<std::string> > realTimeToOnElements;
    std::map<double, std::vector<std::string> > realTimeToOffElements;
    std::map<double, int> realTimeToTempo;
    double m_scoreTimeOffset;
    double m_realTimeOffsetMilliseconds;
    int m_currentTempo;
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
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// GetRelativeLayerElementParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the next/previous relevant layer element
 * member 1: Id of the layer element that is being compared to (starting point)
 * member 2: direction of search - BACKWARD is for previous element, FORWARD - next
 * member 3: flag to indicate whether search is done in the same layer as element Id, or in neighboring one
 **/

class GetRelativeLayerElementParams : public FunctorParams {
public:
    GetRelativeLayerElementParams(const int elementId, bool searchDirection, bool anotherLayer)
    {
        m_relativeElement = NULL;
        m_initialElementId = elementId;
        m_searchDirection = searchDirection;
        m_isInNeighboringLayer = anotherLayer;
    }
    Object *m_relativeElement;
    int m_initialElementId;
    bool m_searchDirection;
    bool m_isInNeighboringLayer;
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
 * member 5: the doc
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
        m_functor = functor;
        m_doc = doc;
    }
    int m_measureXRel;
    double m_justifiableRatio;
    int m_leftBarLineX;
    int m_rightBarLineX;
    int m_systemFullWidth;
    Functor *m_functor;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// JustifyYParams
//----------------------------------------------------------------------------

/**
 * member 0: the cumulated shift
 * member 1: the amount of space for distribution
 * member 2: the sum of justification factors per page
 * member 3: the functor to be redirected to the MeasureAligner
 * member 4: the doc
 **/

class JustifyYParams : public FunctorParams {
public:
    JustifyYParams(Functor *functor, Doc *doc)
    {
        m_cumulatedShift = 0;
        m_spaceToDistribute = 0;
        m_justificationSum = 0.;
        m_functor = functor;
        m_doc = doc;
    }

    int m_cumulatedShift;
    int m_spaceToDistribute;
    double m_justificationSum;
    Functor *m_functor;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// LayerCountInTimeSpanParams
//----------------------------------------------------------------------------

/**
 * member 0: the time of the event
 * member 1: the duration of the event
 * member 2: the list of layerN found
 * member 3: the current meter signature
 * member 4: the current mensur
 * member 5: the functor for redirection
 **/

class LayerCountInTimeSpanParams : public FunctorParams {
public:
    LayerCountInTimeSpanParams(MeterSig *meterSig, Mensur *mensur, Functor *functor)
    {
        m_time = 0.0;
        m_duration = 0.0;
        m_meterSig = meterSig;
        m_mensur = mensur;
        m_functor = functor;
    }
    double m_time;
    double m_duration;
    std::vector<int> m_layers;
    MeterSig *m_meterSig;
    Mensur *m_mensur;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// LayerElementsInTimeSpanParams
//----------------------------------------------------------------------------

/**
 * member 0: the time of the event
 * member 1: the duration of the event
 * member 2: the list of layer elements found
 * member 3: the current meter signature
 * member 4: the current mensur
 * member 5: layer to process elements on
 **/

class LayerElementsInTimeSpanParams : public FunctorParams {
public:
    LayerElementsInTimeSpanParams(MeterSig *meterSig, Mensur *mensur, Layer *layer)
    {
        m_time = 0.0;
        m_duration = 0.0;
        m_meterSig = meterSig;
        m_mensur = mensur;
        m_layer = layer;
    }
    double m_time;
    double m_duration;
    ListOfObjects m_elements;
    MeterSig *m_meterSig;
    Mensur *m_mensur;
    Layer *m_layer;
};

//----------------------------------------------------------------------------
// OptimizeScoreDefParams
//----------------------------------------------------------------------------

/**
 * member 0: the current scoreDef
 * member 1: the current staffDef
 * member 2: the flag indicating if we are optimizing encoded layout
 * member 3: the doc
 **/

class OptimizeScoreDefParams : public FunctorParams {
public:
    OptimizeScoreDefParams(Doc *doc, Functor *functor, Functor *functorEnd)
    {
        m_currentScoreDef = NULL;
        m_encoded = false;
        m_firstScoreDef = true;
        m_hasFermata = false;
        m_hasTempo = false;
        m_doc = doc;
        m_functor = functor;
        m_functorEnd = functorEnd;
    }
    ScoreDef *m_currentScoreDef;
    bool m_encoded;
    bool m_firstScoreDef;
    bool m_hasFermata;
    bool m_hasTempo;
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
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
// PrepareFacsimileParams
//----------------------------------------------------------------------------

/**
 * member 0: the doc
 */

class PrepareFacsimileParams : public FunctorParams {
public:
    PrepareFacsimileParams(Facsimile *facsimile) { m_facsimile = facsimile; }
    Facsimile *m_facsimile;
    ListOfObjects m_zonelessSyls;
};

//----------------------------------------------------------------------------
// PrepareFloatingGrpsParams
//----------------------------------------------------------------------------

/**
 * member 0: the previous ending
 * member 1: the current grpId
 * member 2: the dynam in the current measure
 * member 3: the current hairpins to be linked / grouped
 * member 4: the map of existing harms (based on @n)
 **/

class PrepareFloatingGrpsParams : public FunctorParams {
public:
    PrepareFloatingGrpsParams()
    {
        m_previousEnding = NULL;
        m_pedalLine = NULL;
    }
    Ending *m_previousEnding;
    Pedal *m_pedalLine;
    std::vector<Dynam *> m_dynams;
    std::vector<Hairpin *> m_hairpins;
    std::map<std::string, Harm *> m_harms;
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
// PrepareLinkingParams
//----------------------------------------------------------------------------

/**
 * member 0: ArrayOfInterfaceUuidPairs holds the interface / uuid pairs to match
 * member 1: bool* fillList for indicating whether the pairs have to be stacked or not
 **/

class PrepareLinkingParams : public FunctorParams {
public:
    PrepareLinkingParams() { m_fillList = true; }
    MapOfLinkingInterfaceUuidPairs m_nextUuidPairs;
    MapOfLinkingInterfaceUuidPairs m_sameasUuidPairs;
    bool m_fillList;
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
    ArrayOfPlistInterfaceUuidPairs m_interfaceUuidPairs;
    bool m_fillList;
};

//----------------------------------------------------------------------------
// PreparePointersByLayerParams
//----------------------------------------------------------------------------

/**
 * member 0: the current Note
 * member 1: the last Dot
 **/

class PreparePointersByLayerParams : public FunctorParams {
public:
    PreparePointersByLayerParams()
    {
        m_currentNote = NULL;
        m_lastDot = NULL;
    }
    Note *m_currentNote;
    Dot *m_lastDot;
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
// PrepareTimePointingParams
//----------------------------------------------------------------------------

/**
 * member 0: std::vector< Object*>* that holds the current elements to match
 **/

class PrepareTimePointingParams : public FunctorParams {
public:
    PrepareTimePointingParams() {}
    ListOfPointingInterClassIdPairs m_timePointingInterfaces;
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
    ListOfSpanningInterClassIdPairs m_timeSpanningInterfaces;
    bool m_fillList;
};

//----------------------------------------------------------------------------
// PrepareTimestampsParams
//----------------------------------------------------------------------------

/**
 * member 0: std::vector< Object*>* that holds the current elements to match
 * member 1:  ListOfObjectBeatPairs* that holds the tstamp2 elements for attach to the end measure
 **/

class PrepareTimestampsParams : public FunctorParams {
public:
    PrepareTimestampsParams() {}
    ListOfSpanningInterClassIdPairs m_timeSpanningInterfaces;
    ListOfObjectBeatPairs m_tstamps;
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
    ReplaceDrawingValuesInStaffDefParams(
        Clef const *clef, KeySig const *keySig, Mensur const *mensur, MeterSig const *meterSig)
    {
        m_clef = clef;
        m_keySig = keySig;
        m_mensur = mensur;
        m_meterSig = meterSig;
    }
    Clef const *m_clef;
    KeySig const *m_keySig;
    Mensur const *m_mensur;
    MeterSig const *m_meterSig;
};

//----------------------------------------------------------------------------
// SaveParams
//----------------------------------------------------------------------------

/**
 * member 0: output stream
 **/

class SaveParams : public FunctorParams {
public:
    SaveParams(Output *output) { m_output = output; }
    Output *m_output;
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
// TransposeParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the transposer
 * member 1: a pointer to document
 **/

class TransposeParams : public FunctorParams {
public:
    TransposeParams(Doc *doc, Transposer *transposer)
    {
        m_transposer = transposer;
        m_doc = doc;
    }
    Transposer *m_transposer;
    Doc *m_doc;
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
// UnsetCurrentScoreDefParams
//----------------------------------------------------------------------------

/**
 * member 0: the functor to be redirected to Aligner
 **/

class UnsetCurrentScoreDefParams : public FunctorParams {
public:
    UnsetCurrentScoreDefParams(Functor *functor) { m_functor = functor; }
    Functor *m_functor;
};

} // namespace vrv

#endif
