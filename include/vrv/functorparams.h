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
class Slur;
class Staff;
class StaffAlignment;
class StaffDef;
class StemmedDrawingInterface;
class Syl;
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
 * member 0: the array of Alignment, Arpeg, StaffN, bool tuples
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
    ArrayOfAlignmentArpegTuples m_alignmentArpegTuples;
    MeasureAligner *m_measureAligner;
    Functor *m_functor;
    Doc *m_doc;
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
 * member 4: slope of the beam
 * member 5: overlap margin that beam needs to be displaced by
 * member 6: the Doc
 * member 7: the flag indicating whether element from different layer is being processed
 **/

class AdjustBeamParams : public FunctorParams {
public:
    AdjustBeamParams(Doc *doc)
    {
        m_beam = NULL;
        m_y1 = 0;
        m_y2 = 0;
        m_x1 = 0;
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
    double m_beamSlope;
    int m_directionBias;
    int m_overlapMargin;
    Doc *m_doc;
    bool m_isOtherLayer;
};

//----------------------------------------------------------------------------
// AdjustClefsParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the measureAligner
 * member 1: the Doc
 **/

class AdjustClefsParams : public FunctorParams {
public:
    AdjustClefsParams(Doc *doc)
    {
        m_aligner = NULL;
        m_doc = doc;
    }
    MeasureAligner *m_aligner;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AdjustCrossStaffContentParams
//----------------------------------------------------------------------------

/**
 * member 0: a map of calculated shifts per StaffAlignment
 *  => this is transferred from JustifyY
 * member 1: the doc
 **/

class AdjustCrossStaffContentParams : public FunctorParams {
public:
    AdjustCrossStaffContentParams(Doc *doc) { m_doc = doc; }

    std::map<StaffAlignment *, int> m_shiftForStaff;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// AdjustDotsParams
//----------------------------------------------------------------------------

/**
 * member 0: the list of staffN in the top-level scoreDef
 * member 1: the list of all elements (except for dots) in for the alignment in the staff
 * member 2: the list of all dots in for the alignment in the staff
 * member 3: the Doc
 * member 4: the Functor for redirection to the MeasureAligner
 * member 5: the end Functor for redirection
 **/

class AdjustDotsParams : public FunctorParams {
public:
    AdjustDotsParams(Doc *doc, Functor *functor, Functor *functorEnd, const std::vector<int> &staffNs)
    {
        m_doc = doc;
        m_functor = functor;
        m_functorEnd = functorEnd;
        m_staffNs = staffNs;
    }
    std::vector<int> m_staffNs;
    std::vector<LayerElement *> m_elements;
    std::vector<Dots *> m_dots;
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
};

//----------------------------------------------------------------------------
// AdjustArticWithSlursParams
//----------------------------------------------------------------------------

// Use FunctorDocParams

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
        m_staffNs = staffNs;
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
 * member 4: the doc
 * member 5: a pointer to the functor for passing it to the system aligner
 * member 6: a pointer to the end functor for passing it to the system aligner
 * member 7: flag whether the element is in unison
 * member 8: flag whether the element (note) has as stem.sameas note
 * member 9: the total shift of the current note or chord
 **/

class AdjustLayersParams : public FunctorParams {
public:
    AdjustLayersParams(Doc *doc, Functor *functor, Functor *functorEnd, const std::vector<int> &staffNs)
    {
        m_currentLayerN = VRV_UNSET;
        m_doc = doc;
        m_functor = functor;
        m_functorEnd = functorEnd;
        m_staffNs = staffNs;
        m_unison = false;
        m_ignoreDots = true;
        m_stemSameas = false;
        m_accumulatedShift = 0;
    }
    std::vector<int> m_staffNs;
    int m_currentLayerN;
    std::vector<LayerElement *> m_previous;
    std::vector<LayerElement *> m_current;
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
    bool m_unison;
    bool m_ignoreDots;
    bool m_stemSameas;
    int m_accumulatedShift;
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
// AdjustTempoParams
//----------------------------------------------------------------------------

/**
 * member 0: the systemAligner
 * member 1: the doc
 **/

class AdjustTempoParams : public FunctorParams {
public:
    AdjustTempoParams(Doc *doc)
    {
        m_systemAligner = NULL;
        m_doc = doc;
    }

    SystemAligner *m_systemAligner;
    Doc *m_doc;
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
    AdjustTupletNumOverlapParams(TupletNum *tupletNum, Staff *staff)
    {
        m_tupletNum = tupletNum;
        m_drawingNumPos = STAFFREL_basic_NONE;
        m_horizontalMargin = 0;
        m_verticalMargin = 0;
        m_staff = staff;
        m_yRel = 0;
    }

    TupletNum *m_tupletNum;
    data_STAFFREL_basic m_drawingNumPos;
    int m_horizontalMargin;
    int m_verticalMargin;
    Staff *m_staff;
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
// AdjustXPosAlignmentOffset
//----------------------------------------------------------------------------
// Class to hold information about alignment, possible offset and overlapping bounding box. This is a helper class to be
// used solely with AdjustXPosParams to store information regarding current/previous alignment that are being processed
class AdjustXPosAlignmentOffset {
public:
    AdjustXPosAlignmentOffset() : m_offset(0), m_overlappingBB(NULL){};

    void Reset()
    {
        m_alignment = NULL;
        m_offset = 0;
        m_overlappingBB = NULL;
    }

public:
    // data members
    Alignment *m_alignment;
    int m_offset;
    BoundingBox *m_overlappingBB;
};

//----------------------------------------------------------------------------
// AdjustXPosParams
//----------------------------------------------------------------------------

/**
 * member 0: the minimum position (i.e., the width of the previous element)
 * member 1: the upcoming minimum position (i.e., the min pos for the next element)
 * member 2: the cumulated shift on the previous aligners
 * member 3: the @n of the staff currently processed (used for grace note alignment)
 * member 4: the size of the staff being processed
 * member 5: the list of staffN in the top-level scoreDef
 * member 6: the bounding box in the previous aligner
 * member 7: the upcoming bounding boxes (to be used in the next aligner)
 * member 8: list of types to include
 * member 9: list of types to exclude
 * member 10: flag to indicate whether only right bar line positions should be considered
 * member 11: list of tie endpoints for the current measure
 * member 12: the Doc
 * member 13: the Functor for redirection to the MeasureAligner
 * member 14: the end Functor for redirection
 * member 15: current aligner that is being processed
 * member 16: preceeding aligner that was handled before
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
        m_staffSize = 100;
        m_rightBarLinesOnly = false;
        m_doc = doc;
        m_functor = functor;
        m_functorEnd = functorEnd;
        m_currentAlignment.Reset();
        m_previousAlignment.Reset();
        m_measureTieEndpoints.clear();
    }
    int m_minPos;
    int m_upcomingMinPos;
    int m_cumulatedXShift;
    int m_staffN;
    int m_staffSize;
    std::vector<int> m_staffNs;
    std::vector<BoundingBox *> m_boundingBoxes;
    std::vector<BoundingBox *> m_upcomingBoundingBoxes;
    std::vector<ClassId> m_includes;
    std::vector<ClassId> m_excludes;
    bool m_rightBarLinesOnly;
    std::vector<std::pair<LayerElement *, LayerElement *>> m_measureTieEndpoints;
    Doc *m_doc;
    Functor *m_functor;
    Functor *m_functorEnd;
    AdjustXPosAlignmentOffset m_currentAlignment;
    AdjustXPosAlignmentOffset m_previousAlignment;
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
 * member 2: shift next measure due to section restart
 * member 3: store castoff system widths if true
 * member 4: the doc
 **/

class AlignMeasuresParams : public FunctorParams {
public:
    AlignMeasuresParams(Doc *doc)
    {
        m_shift = 0;
        m_justifiableWidth = 0;
        m_applySectionRestartShift = false;
        m_storeCastOffSystemWidths = false;
        m_doc = doc;
    }

    int m_shift;
    int m_justifiableWidth;
    bool m_applySectionRestartShift;
    bool m_storeCastOffSystemWidths;
    Doc *m_doc;
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
 * member 0: the current score time
 * member 1: the current time in seconds
 * member 2: the current tempo
 * member 3: the tempo adjustment
 * member 4: factor for multibar rests
 **/

class CalcMaxMeasureDurationParams : public FunctorParams {
public:
    CalcMaxMeasureDurationParams()
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
// CalcSlursParams
//----------------------------------------------------------------------------

/**
 * member 0: the doc
 **/

class CalcSlursParams : public FunctorParams {
public:
    CalcSlursParams(Doc *doc) { m_doc = doc; }
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// CalcStemParams
//----------------------------------------------------------------------------

/**
 * member 0: the additional length for a chord stem (between the top and bottom note)
 * member 1: the vertical center of the staff
 * member 2: the actual duration of the chord / note
 * member 3: the flag for grace notes (stem is not extended)
 * member 4: the flag for stem.sameas notes
 * member 5: the flag indicating that we have no note in tabGrp
 * member 6: the current staff (to avoid additional lookup)
 * member 7: the current layer (ditto)
 * member 8: the chord or note to which the stem belongs
 * member 9: the doc
 **/

class CalcStemParams : public FunctorParams {
public:
    CalcStemParams(Doc *doc)
    {
        m_chordStemLength = 0;
        m_verticalCenter = 0;
        m_dur = DUR_1;
        m_isGraceNote = false;
        m_isStemSameasSecondary = false;
        m_tabGrpWithNoNote = false;
        m_staff = NULL;
        m_layer = NULL;
        m_interface = NULL;
        m_doc = doc;
    }
    int m_chordStemLength;
    int m_verticalCenter;
    int m_dur;
    bool m_isGraceNote;
    bool m_isStemSameasSecondary;
    bool m_tabGrpWithNoNote;
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
 * member 0: the comparison object
 * member 1: an array of all matching objects
 * member 2: the start object range
 * member 3: the end object range
 **/

class FindAllBetweenParams : public FunctorParams {
public:
    FindAllBetweenParams(Comparison *comparison, ListOfObjects *elements, const Object *start, const Object *end)
    {
        m_comparison = comparison;
        m_elements = elements;
        m_start = start;
        m_end = end;
    }
    Comparison *m_comparison;
    ListOfObjects *m_elements;
    const Object *m_start;
    const Object *m_end;
};

//----------------------------------------------------------------------------
// FindAllConstBetweenParams
//----------------------------------------------------------------------------

/**
 * member 0: the comparison object
 * member 1: an array of all matching objects
 * member 2: the start object range
 * member 3: the end object range
 **/

class FindAllConstBetweenParams : public FunctorParams {
public:
    FindAllConstBetweenParams(
        Comparison *comparison, ListOfConstObjects *elements, const Object *start, const Object *end)
    {
        m_comparison = comparison;
        m_elements = elements;
        m_start = start;
        m_end = end;
    }
    Comparison *m_comparison;
    ListOfConstObjects *m_elements;
    const Object *m_start;
    const Object *m_end;
};

//----------------------------------------------------------------------------
// FindAllReferencedObjectsParams
//----------------------------------------------------------------------------

/**
 * member 0: an array of all matching objects
 * member 1: a flag indicating if milestone references should be included as well
 **/

class FindAllReferencedObjectsParams : public FunctorParams {
public:
    FindAllReferencedObjectsParams(ListOfObjects *elements)
    {
        m_elements = elements;
        m_milestoneReferences = false;
    }
    ListOfObjects *m_elements;
    bool m_milestoneReferences;
};

//----------------------------------------------------------------------------
// FindAllByComparisonParams
//----------------------------------------------------------------------------

/**
 * member 0: the comparison object
 * member 1: an array of all matching objects
 * member 2: flag indicating whether descendants of matches should be searched as well
 **/

class FindAllByComparisonParams : public FunctorParams {
public:
    FindAllByComparisonParams(Comparison *comparison, ListOfObjects *elements)
    {
        m_comparison = comparison;
        m_elements = elements;
        m_continueDepthSearchForMatches = true;
    }
    Comparison *m_comparison;
    ListOfObjects *m_elements;
    bool m_continueDepthSearchForMatches;
};

//----------------------------------------------------------------------------
// FindAllConstByComparisonParams
//----------------------------------------------------------------------------

/**
 * member 0: the comparison object
 * member 1: an array of all matching objects
 * member 2: flag indicating whether descendants of matches should be searched as well
 **/

class FindAllConstByComparisonParams : public FunctorParams {
public:
    FindAllConstByComparisonParams(Comparison *comparison, ListOfConstObjects *elements)
    {
        m_comparison = comparison;
        m_elements = elements;
        m_continueDepthSearchForMatches = true;
    }
    Comparison *m_comparison;
    ListOfConstObjects *m_elements;
    bool m_continueDepthSearchForMatches;
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
    const Object *m_element;
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
    const Object *m_element;
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
    const Object *m_element;
};

//----------------------------------------------------------------------------
// FindSpannedLayerElementsParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the vector of LayerElement pointer to fill
 * member 1: the minimum position
 * member 2: the maximum position
 * member 3: the staff numbers to consider, any staff if empty
 * member 4: the minimal layerN to consider, unbounded below if zero
 * member 5: the maximal layerN to consider, unbounded above if zero
 * member 6: true if within measure range of timespanning interface, only this is searched
 * member 7: the timespanning interface
 * member 8: the class ids to keep
 **/

class FindSpannedLayerElementsParams : public FunctorParams {
public:
    FindSpannedLayerElementsParams(TimeSpanningInterface *interface)
    {
        m_interface = interface;
        m_minPos = 0;
        m_maxPos = 0;
        m_minLayerN = 0;
        m_maxLayerN = 0;
        m_inMeasureRange = false;
    }
    std::vector<LayerElement *> m_elements;
    int m_minPos;
    int m_maxPos;
    std::set<int> m_staffNs;
    int m_minLayerN;
    int m_maxLayerN;
    bool m_inMeasureRange;
    TimeSpanningInterface *m_interface;
    std::vector<ClassId> m_classIds;
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
// HorizontalLayoutCacheParams
//----------------------------------------------------------------------------

/**
 * member 0: a flag indicating if the cache should be stored (default) or restored
 * member 1: a pointer to the Doc
 **/

class HorizontalLayoutCacheParams : public FunctorParams {
public:
    HorizontalLayoutCacheParams(Doc *doc)
    {
        m_restore = false;
        m_doc = doc;
    }
    bool m_restore;
    Doc *m_doc;
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
 * member 4: int: the semi tone transposition for the current track
 * member 5: double with the current tempo
 * member 6: the last (non grace) note that was performed
 * member 7: expanded notes due to ornaments and tremolandi
 * member 8: deferred notes which start slightly later
 * member 9: grace note sequence
 * member 10: flag indicating whether the last grace note/chord was accented
 * member 11: the functor
 * member 12: Tablature held notes indexed by (course - 1)
 **/

class GenerateMIDIParams : public FunctorParams {
public:
    GenerateMIDIParams(Doc *doc, smf::MidiFile *midiFile, Functor *functor)
    {
        m_midiFile = midiFile;
        m_midiChannel = 0;
        m_midiTrack = 1;
        m_totalTime = 0.0;
        m_transSemi = 0;
        m_currentTempo = MIDI_TEMPO;
        m_lastNote = NULL;
        m_accentedGraceNote = false;
        m_functor = functor;
        m_doc = doc;
    }
    smf::MidiFile *m_midiFile;
    int m_midiChannel;
    int m_midiTrack;
    double m_totalTime;
    int m_transSemi;
    double m_currentTempo;
    Note *m_lastNote;
    std::map<Note *, MIDINoteSequence> m_expandedNotes;
    std::map<Note *, double> m_deferredNotes;
    MIDIChordSequence m_graceNotes;
    bool m_accentedGraceNote;
    Functor *m_functor;
    std::vector<MIDIHeldNote> m_heldNotes;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// GenerateTimemapParams
//----------------------------------------------------------------------------

/**
 * member 0: Score time from the start of the piece to previous barline in quarter notes
 * member 1: Real time from the start of the piece to previous barline in ms
 * member 2: Currently active tempo
 * member 3: A pointer to the Timemap
 * member 4: The functor for redirection
 **/

class GenerateTimemapParams : public FunctorParams {
public:
    GenerateTimemapParams(Timemap *timemap, Functor *functor)
    {
        m_scoreTimeOffset = 0.0;
        m_realTimeOffsetMilliseconds = 0;
        m_currentTempo = MIDI_TEMPO;
        m_timemap = timemap;
        m_functor = functor;
    }
    double m_scoreTimeOffset;
    double m_realTimeOffsetMilliseconds;
    double m_currentTempo;
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
// InitSelectionParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the system we are taking the content from
 * member 1: a pointer the page we are adding system to
 * member 2: a pointer to the current system
 * member 3: a pointer to the doc
 **/

class InitSelectionParams : public FunctorParams {
public:
    InitSelectionParams(Page *page, Doc *doc, const std::string &start, const std::string &end)
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
 *  => this is transferred to AdjustCrossStaffContent
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
    std::set<int> m_layers;
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
        m_allLayersButCurrent = false;
        m_meterSig = meterSig;
        m_mensur = mensur;
        m_layer = layer;
    }
    double m_time;
    double m_duration;
    bool m_allLayersButCurrent;
    ListOfObjects m_elements;
    MeterSig *m_meterSig;
    Mensur *m_mensur;
    Layer *m_layer;
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
// PrepareDelayedTurnsParams
//----------------------------------------------------------------------------

/**
 * member 0: a flag indicating that we are running a first pass to fill the map
 * member 1: a pointer to the element to which a turn is pointing to
 * member 2: a pointer to the turn to which we want to set a m_drawingEndElement
 * member 3: a map of the delayed turns and the layer element they point to
 **/

class PrepareDelayedTurnsParams : public FunctorParams {
public:
    PrepareDelayedTurnsParams()
    {
        m_initMap = true;
        m_previousElement = NULL;
        m_currentTurn = NULL;
    }
    bool m_initMap;
    LayerElement *m_previousElement;
    Turn *m_currentTurn;
    std::map<LayerElement *, Turn *> m_delayedTurns;
};

//----------------------------------------------------------------------------
// PrepareDurationParams
//----------------------------------------------------------------------------

/**
 * member 0: the current scoredef default duration
 * member 1: the current staffdef default durations
 * member 2: the functor for redirection
 **/

class PrepareDurationParams : public FunctorParams {
public:
    PrepareDurationParams(Functor *functor)
    {
        m_durDefault = DURATION_NONE;
        m_functor = functor;
    }
    data_DURATION m_durDefault;
    std::map<int, data_DURATION> m_durDefaultForStaffN;
    Functor *m_functor;
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
 * member 5: a pointer to the doc scoreDef
 **/

class PrepareFloatingGrpsParams : public FunctorParams {
public:
    PrepareFloatingGrpsParams(Doc *doc)
    {
        m_previousEnding = NULL;
        m_doc = doc;
    }
    Ending *m_previousEnding;
    std::list<Pedal *> m_pedalLines;
    std::vector<Dynam *> m_dynams;
    std::vector<Hairpin *> m_hairpins;
    std::map<std::string, Harm *> m_harms;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// PrepareLyricsParams
//----------------------------------------------------------------------------

/**
 * member 0: the current Syl
 * member 1: the last Note or Chord
 * member 2: the penultimate Note or Chord
 **/

class PrepareLyricsParams : public FunctorParams {
public:
    PrepareLyricsParams()
    {
        m_currentSyl = NULL;
        m_lastNoteOrChord = NULL;
        m_penultimateNoteOrChord = NULL;
    }
    Syl *m_currentSyl;
    LayerElement *m_lastNoteOrChord;
    LayerElement *m_penultimateNoteOrChord;
};

//----------------------------------------------------------------------------
// PrepareLinkingParams
//----------------------------------------------------------------------------

/**
 * member 0: MapOfLinkingInterfaceUuidPairs holds the interface / uuid pairs to match for links
 * member 1: MapOfLinkingInterfaceUuidPairs holds the interface / uuid pairs to match for sameas
 * member 2: MapOfNoteUuidPairs holds the note / uuid pairs to match for stem.sameas
 * member 3: bool* fillList for indicating whether the pairs have to be stacked or not
 *
 **/

class PrepareLinkingParams : public FunctorParams {
public:
    PrepareLinkingParams() { m_fillList = true; }
    MapOfLinkingInterfaceUuidPairs m_nextUuidPairs;
    MapOfLinkingInterfaceUuidPairs m_sameasUuidPairs;
    MapOfNoteUuidPairs m_stemSameasUuidPairs;
    bool m_fillList;
};

//----------------------------------------------------------------------------
// PrepareMIDIParams
//----------------------------------------------------------------------------

/**
 * member 0: the current tempo
 * member 1: deferred notes which start slightly later
 **/

class PrepareMIDIParams : public FunctorParams {
public:
    PrepareMIDIParams() { m_currentTempo = MIDI_TEMPO; }
    double m_currentTempo;
    std::map<Note *, double> m_deferredNotes;
};

//----------------------------------------------------------------------------
// PrepareMilestonesParams
//----------------------------------------------------------------------------

/**
 * member 0: the last measure
 * member 1: the current milestone
 **/

class PrepareMilestonesParams : public FunctorParams {
public:
    PrepareMilestonesParams()
    {
        m_lastMeasure = NULL;
        m_currentEnding = NULL;
    }
    Measure *m_lastMeasure;
    Ending *m_currentEnding;
    std::vector<SystemMilestoneInterface *> m_startMilestones;
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
    ArrayOfPlistInterfaceUuidTuples m_interfaceUuidTuples;
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
        m_currentElement = NULL;
        m_lastDot = NULL;
    }
    LayerElement *m_currentElement;
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
    PrepareRptParams(Doc *doc)
    {
        m_currentMRpt = NULL;
        m_multiNumber = BOOLEAN_NONE;
        m_doc = doc;
    }
    MRpt *m_currentMRpt;
    data_BOOLEAN m_multiNumber;
    Doc *m_doc;
};

//----------------------------------------------------------------------------
// PrepareTimePointingParams
//----------------------------------------------------------------------------

/**
 * member 0: interface list that holds the current elements to match
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
 * member 0: interface list that holds the current elements to match
 * member 1: bool fillList for indicating whether the elements have to be stacked or not
 **/

class PrepareTimeSpanningParams : public FunctorParams {
public:
    PrepareTimeSpanningParams() { m_fillList = true; }
    ListOfSpanningInterOwnerPairs m_timeSpanningInterfaces;
    bool m_fillList;
};

//----------------------------------------------------------------------------
// PrepareTimestampsParams
//----------------------------------------------------------------------------

/**
 * member 0: interface list that holds the current elements to match
 * member 1: ListOfObjectBeatPairs that holds the tstamp2 elements for attach to the end measure
 **/

class PrepareTimestampsParams : public FunctorParams {
public:
    PrepareTimestampsParams() {}
    ListOfSpanningInterClassIdPairs m_timeSpanningInterfaces;
    ListOfObjectBeatPairs m_tstamps;
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
    ReplaceDrawingValuesInStaffDefParams(Clef const *clef, KeySig const *keySig, Mensur const *mensur,
        MeterSig const *meterSig, MeterSigGrp const *meterSigGrp)
    {
        m_clef = clef;
        m_keySig = keySig;
        m_mensur = mensur;
        m_meterSig = meterSig;
        m_meterSigGrp = meterSigGrp;
    }
    Clef const *m_clef;
    KeySig const *m_keySig;
    Mensur const *m_mensur;
    MeterSig const *m_meterSig;
    MeterSigGrp const *m_meterSigGrp;
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
 * member 3: the estimated justification ratio of the system
 * member 4: the last alignment that was not timestamp-only
 * member 5: the list of timestamp-only alignment that needs to be adjusted
 * member 6: the MeasureAligner
 * member 7: the Doc
 * member 8: the functor to be redirected to Aligner
 **/

class SetAlignmentXPosParams : public FunctorParams {
public:
    SetAlignmentXPosParams(Doc *doc, Functor *functor)
    {
        m_previousTime = 0.0;
        m_previousXRel = 0;
        m_longestActualDur = 0;
        m_estimatedJustificationRatio = 1.0;
        m_lastNonTimestamp = NULL;
        m_measureAligner = NULL;
        m_doc = doc;
        m_functor = functor;
    }
    double m_previousTime;
    int m_previousXRel;
    int m_longestActualDur;
    double m_estimatedJustificationRatio;
    Alignment *m_lastNonTimestamp;
    std::list<Alignment *> m_timestamps;
    MeasureAligner *m_measureAligner;
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
// ScoreDefOptimizeParams
//----------------------------------------------------------------------------

/**
 * member 0: the current scoreDef
 * member 1: the current staffDef
 * member 2: the flag indicating if we are optimizing encoded layout
 * member 3: the doc
 **/

class ScoreDefOptimizeParams : public FunctorParams {
public:
    ScoreDefOptimizeParams(Doc *doc, Functor *functor, Functor *functorEnd)
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
// ScoreDefSetCurrentParams
//----------------------------------------------------------------------------

/**
 * member 0: the current score
 * member 1: the current scoreDef
 * member 2: the current staffDef
 * member 3: the upcoming scoreDef
 * member 4: the previous measure (for setting cautionary scoreDef)
 * member 5: the current system (for setting the system scoreDef)
 * member 6: the flag indicating whereas full labels have to be drawn
 * member 7: the flag indicating that the scoreDef restarts (draw brace and label)
 * member 8: the flag indicating is we already have a measure in the system
 * member 9: the doc
 * member 10: the functor to be redirected from Score
 **/

class ScoreDefSetCurrentParams : public FunctorParams {
public:
    ScoreDefSetCurrentParams(Doc *doc, Functor *functor)
    {
        m_currentScore = NULL;
        m_currentScoreDef = NULL;
        m_currentStaffDef = NULL;
        m_upcomingScoreDef.Reset();
        m_previousMeasure = NULL;
        m_currentSystem = NULL;
        m_drawLabels = false;
        m_restart = false;
        m_hasMeasure = false;
        m_doc = doc;
        m_functor = functor;
    }
    Score *m_currentScore;
    ScoreDef *m_currentScoreDef;
    StaffDef *m_currentStaffDef;
    ScoreDef m_upcomingScoreDef;
    Measure *m_previousMeasure;
    System *m_currentSystem;
    bool m_drawLabels;
    bool m_restart;
    bool m_hasMeasure;
    Doc *m_doc;
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// ScoreDefSetGrpSymParams
//----------------------------------------------------------------------------

/**
 * member 0: the functor to be redirected to the System::m_currentScoreDef
 **/

class ScoreDefSetGrpSymParams : public FunctorParams {
public:
    ScoreDefSetGrpSymParams(Functor *functor) { m_functor = functor; }
    Functor *m_functor;
};

//----------------------------------------------------------------------------
// ScoreDefUnsetCurrentParams
//----------------------------------------------------------------------------

/**
 * member 0: the functor to be redirected to Aligner
 **/

class ScoreDefUnsetCurrentParams : public FunctorParams {
public:
    ScoreDefUnsetCurrentParams(Functor *functor) { m_functor = functor; }
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

enum StaffDefRedrawFlags {
    REDRAW_CLEF = 0x1,
    REDRAW_KEYSIG = 0x2,
    REDRAW_MENSUR = 0x4,
    REDRAW_METERSIG = 0x8,
    REDRAW_METERSIGGRP = 0x10,
    // all flags
    REDRAW_ALL = REDRAW_CLEF | REDRAW_KEYSIG | REDRAW_MENSUR | REDRAW_METERSIG | REDRAW_METERSIGGRP,
    //
    FORCE_REDRAW = 0x100
};

class SetStaffDefRedrawFlagsParams : public FunctorParams {
public:
    SetStaffDefRedrawFlagsParams() { m_redrawFlags = 0; }
    int m_redrawFlags;
};

//----------------------------------------------------------------------------
// TransposeParams
//----------------------------------------------------------------------------

/**
 * member 0: a pointer to the document
 * member 1: the functor for redirection
 * member 2: a pointer to the transposer
 * member 3: the transposition to be applied
 * member 4: the mdiv selected for transposition
 * member 5: the list of current (nested) mdivs
 **/

class TransposeParams : public FunctorParams {
public:
    TransposeParams(Doc *doc, Functor *functor, Transposer *transposer)
    {
        m_doc = doc;
        m_functor = functor;
        m_transposer = transposer;
    }
    Doc *m_doc;
    Functor *m_functor;
    Transposer *m_transposer;
    std::string m_transposition;
    std::string m_selectedMdivUuid;
    std::list<std::string> m_currentMdivUuids;
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
