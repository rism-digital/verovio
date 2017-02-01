/////////////////////////////////////////////////////////////////////////////
// Name:        measure.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MEASURE_H__
#define __VRV_MEASURE_H__

#include "aligner.h"
#include "atts_shared.h"
#include "barline.h"
#include "object.h"

namespace vrv {

class Ending;
class ControlElement;
class ScoreDef;
class TimestampAttr;

//----------------------------------------------------------------------------
// Measure
//----------------------------------------------------------------------------

/**
 * This class represents a measure in a page-based score (Doc).
 * A Measure is contained in a Staff.
 * It contains Layer objects.
 * For internally simplication of processing, unmeasured music is contained in one single measure object
 */
class Measure : public Object, public AttCommon, public AttMeasureLog, public AttPointing {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Measure(bool measuredMusic = true, int logMeasureNb = -1);
    virtual ~Measure();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Measure"; }
    virtual ClassId GetClassId() const { return MEASURE; }
    ///@}

    /**
     * Return true if measured music (otherwise we have fake measures)
     */
    bool IsMeasuredMusic() const { return m_measuredMusic; }

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    virtual void AddChild(Object *object);
    ///@}

    /**
     * Add a TimestampAttr to the measure.
     * The TimestampAttr it not added as child of the measure but to the Measure::m_timestamps array.
     */
    void AddTimestamp(TimestampAttr *timestampAttr);

    /**
     * Return the index position of the measure in its system parent
     */
    int GetMeasureIdx() const { return Object::GetIdx(); }

    /**
     * @name Set and get the left and right barline types
     * This somehow conflicts with AttMeasureLog, which is transfered from and to the
     * Barline object when reading and writing MEI. See MeiInput::ReadMeiMeasure and
     * MeiOutput::ReadMeiMeasure
     * Alternatively, we could keep them in sync here:
     * data_BARRENDITION GetDrawingLeftBarLine() { m_leftBarLine.SetRend(GetRight()); return m_leftBarLine.GetRend(); }
     * void SetLeftBarLineType(data_BARRENDITION type) { m_leftBarLine.SetRend(type); SetLeft(type); }
     */
    ///@{
    data_BARRENDITION GetDrawingLeftBarLine() const { return m_leftBarLine.GetForm(); }
    void SetDrawingLeftBarLine(data_BARRENDITION type) { m_leftBarLine.SetForm(type); }
    data_BARRENDITION GetDrawingRightBarLine() const { return m_rightBarLine.GetForm(); }
    void SetDrawingRightBarLine(data_BARRENDITION type) { m_rightBarLine.SetForm(type); }
    ///@}

    /**
     * Set the drawing barlines for the measure.
     * Also adjust the right barline of the previous measure and the left one if necessary.
     */
    void SetDrawingBarLines(Measure *previous, bool systemBreak, bool scoreDefInsert);

    /**
     * @name Set and get the barlines.
     * Careful - the barlines are owned by the measure and will be destroyed by it.
     * This method should be used only for acessing them (e.g., when drawing) and
     * not for creating other measure objects.
     */
    ///@{
    BarLine *const GetLeftBarLine() { return &m_leftBarLine; }
    BarLine *const GetRightBarLine() { return &m_rightBarLine; }
    ///@}

    // int GetXRel() const;

    /**
     * Return the non-justifiable left margin for the measure
     */
    int GetNonJustifiableLeftMargin() const { return m_measureAligner.GetNonJustifiableMargin(); }

    /**
     * @name Return the X1 and X2 rel position of the right and left barLine
     */
    ///@{
    int GetLeftBarLineXRel() const;
    int GetLeftBarLineX1Rel() const;
    int GetLeftBarLineX2Rel() const;
    int GetRightBarLineXRel() const;
    int GetRightBarLineX1Rel() const;
    int GetRightBarLineX2Rel() const;
    ///@}

    /**
     * Return the width of the measure, including the barLine width
     */
    int GetWidth() const;

    /**
     * @name Setter and getter of the drawing scoreDef
     */
    ///@{
    ScoreDef *GetDrawingScoreDef() const { return m_drawingScoreDef; }
    void SetDrawingScoreDef(ScoreDef *drawingScoreDef);
    ///@}

    /**
     * @name Setter and getter of the drawing ending
     */
    ///@{
    Ending *GetDrawingEnding() const { return m_drawingEnding; }
    void SetDrawingEnding(Ending *ending) { m_drawingEnding = ending; }
    ///@}

    /*
     * Return the first staff of each staffGrp according to the scoreDef
     */
    std::vector<Staff *> GetFirstStaffGrpStaves(ScoreDef *scoreDef);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertToPageBased
     */
    virtual int ConvertToPageBased(FunctorParams *functorParams);

    /**
     * See Object::Save
     */
    ///@{
    virtual int Save(FunctorParams *functorParams);
    virtual int SaveEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::UnsetCurrentScoreDef
     */
    virtual int UnsetCurrentScoreDef(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::AlignHorizontally
     */
    virtual int AlignHorizontally(FunctorParams *functorParams);
    virtual int AlignHorizontallyEnd(FunctorParams *functorParams);

    /**
     * See Object::AlignVertically
     */
    virtual int AlignVertically(FunctorParams *functorParams);

    /**
     * See Object::IntegrateBoundingBoxGraceXShift
     */
    virtual int IntegrateBoundingBoxGraceXShift(FunctorParams *functorParams);

    /**
     * See Object::IntegrateBoundingBoxXShift
     */
    virtual int IntegrateBoundingBoxXShift(FunctorParams *functorParams);

    /**
     * See Object::SetAlignmentXPos
     */
    virtual int SetAlignmentXPos(FunctorParams *functorParams);

    /**
     * See Object::SetBoundingBoxXShift
     */
    ///@{
    virtual int SetBoundingBoxXShift(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::AlignMeasures
     */
    virtual int AlignMeasures(FunctorParams *functorParams);

    /**
     * See Object::JustifyX
     */
    virtual int JustifyX(FunctorParams *functorParams);

    /**
     * See Object::CastOffSystems
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

    /**
     * See Object::CastOffEncoding
     */
    virtual int CastOffEncoding(FunctorParams *functorParams);

    /**
     * See Object::SetDrawingXY
     */
    virtual int SetDrawingXY(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::FillStaffCurrentTimeSpanningEnd
     */
    virtual int FillStaffCurrentTimeSpanningEnd(FunctorParams *functorParams);

    /**
     * See Object::PrepareFloatingGrps
     */
    virtual int PrepareFloatingGrps(FunctorParams *functoParams);

    /**
     * See Object::PrepareTimePointingEnd
     */
    virtual int PrepareTimePointingEnd(FunctorParams *functorParams);

    /**
     * See Object::PrepareTimeSpanningEnd
     */
    virtual int PrepareTimeSpanningEnd(FunctorParams *functorParams);

    /**
     * See Object::PrepareBoundaries
     */
    virtual int PrepareBoundaries(FunctorParams *functorParams);

    /**
     * @name See Object::GenerateMIDI
     */
    ///@{
    virtual int GenerateMIDI(FunctorParams *functorParams);
    virtual int GenerateMIDIEnd(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::CalcMaxMeasureDuration
     */
    virtual int CalcMaxMeasureDuration(FunctorParams *functorParams);

    /**
     * See Object::PrepareTimestamps
     */
    virtual int PrepareTimestampsEnd(FunctorParams *functorParams);

public:
    /**
     * The X absolute position of the measure for facsimile (transcription) encodings.
     * This is the top left position of the measure.
     */
    int m_xAbs;
    /**
     * The X relative position of the measure.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingXRel;

    /**
     * The measure aligner that holds the x positions of the content of the measure
     */
    MeasureAligner m_measureAligner;

    TimestampAligner m_timestampAligner;

private:
    bool m_measuredMusic;

    /**
     * @name The measure barlines (left and right) used when drawing
     */
    ///@{
    BarLineAttr m_leftBarLine;
    BarLineAttr m_rightBarLine;
    ///@}

    /**
     * A pointer to the drawing ScoreDef instance. It is owned by the measure and added to a measure when a scoreDef
     * change before or requires it. This include scoreDef elements before it but also clef changes within the previous
     * measure.
     */
    ScoreDef *m_drawingScoreDef;

    /**
     * A pointer to the ending to which the measure belongs. Set by PrepareBoundaries and passed to the System drawing
     * list
     * in DrawMeasure
     */
    Ending *m_drawingEnding;
};

} // namespace vrv

#endif
