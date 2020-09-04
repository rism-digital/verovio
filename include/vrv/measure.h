/////////////////////////////////////////////////////////////////////////////
// Name:        measure.h
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MEASURE_H__
#define __VRV_MEASURE_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "barline.h"
#include "horizontalaligner.h"
#include "object.h"

namespace vrv {

class Ending;
class ControlElement;
class ScoreDef;
class System;
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
class Measure : public Object,
                public AttBarring,
                public AttMeasureLog,
                public AttMeterConformanceBar,
                public AttNNumberLike,
                public AttPointing,
                public AttTyped {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Measure(bool measuredMusic = true, int logMeasureNb = -1);
    virtual ~Measure();
    virtual Object *Clone() const { return new Measure(*this); };
    virtual void Reset();
    virtual std::string GetClassName() const { return "Measure"; }
    virtual ClassId GetClassId() const { return MEASURE; }
    ///@}

    /**
     * Overriding CloneReset() method to be called after copy / assignment calls.
     */
    virtual void CloneReset();

    /**
     * Return true if measured music (otherwise we have fake measures)
     */
    bool IsMeasuredMusic() const { return m_measuredMusic; }

    /**
     * Methods for adding allowed content
     */
    virtual bool IsSupportedChild(Object *object);

    /**
     * Specific method for measures
     */
    void AddChildBack(Object *object);

    /**
     * Add a TimestampAttr to the measure.
     * The TimestampAttr it not added as child of the measure but to the Measure::m_timestamps array.
     */
    void AddTimestamp(TimestampAttr *timestampAttr);

    /**
     * Get the X drawing position
     */
    virtual int GetDrawingX() const;

    /**
     * Reset the cached values of the drawingX values.
     */
    virtual void ResetCachedDrawingX() const;

    /**
     * @name Get and set the X drawing relative positions
     */
    ///@{
    int GetDrawingXRel() const { return m_drawingXRel; }
    void SetDrawingXRel(int drawingXRel);
    ///@}

    /**
     * Return the index position of the measure in its system parent
     */
    int GetMeasureIdx() const { return Object::GetIdx(); }

    /**
     * @name Set and get the left and right barline types
     * This somehow conflicts with AttMeasureLog, which is transfered from and to the
     * Barline object when reading and writing MEI. See MEIInput::ReadMeasure and
     * MEIOutput::WriteMeasure
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
    BarLine *GetLeftBarLine() { return &m_leftBarLine; }
    BarLine *GetRightBarLine() { return &m_rightBarLine; }
    ///@}

    /**
     * Return the non-justifiable left margin for the measure
     */
    int GetNonJustifiableLeftMargin() const { return m_measureAligner.GetNonJustifiableMargin(); }

    /**
     * @name Return the X1 and X2 rel position of the right and left barLine
     */
    ///@{
    int GetLeftBarLineXRel() const;
    int GetLeftBarLineLeft() const;
    int GetLeftBarLineRight() const;
    int GetRightBarLineXRel() const;
    int GetRightBarLineLeft() const;
    int GetRightBarLineRight() const;
    ///@}

    /**
     * Return the width of the measure, including the barLine width
     */
    int GetWidth() const;

    /**
     * Return the inner width of the measure
     */
    int GetInnerWidth() const;

    /**
     * Return the center x of the inner of the measure
     */
    int GetInnerCenterX() const;

    /**
     * Return the right overlow of the control events in the measure.
     * Takes into account Dir, Dynam, and Tempo.
     */
    int GetDrawingOverflow();

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

    /**
     * Return the top (first) visible staff in the measure (if any).
     * Takes into account system optimization
     */
    Staff *GetTopVisibleStaff();

    /**
     * Return the botoom (last) visible staff in the measure (if any).
     * Takes into account system optimization
     */
    Staff *GetBottomVisibleStaff();

    /**
     * Check if the measure encloses the given time (in millisecond)
     * Return the playing repeat time (1-based), 0 otherwise
     */
    int EnclosesTime(int time) const;

    /**
     * Return the real time offset in millisecond for the repeat (1-based).
     */
    double GetRealTimeOffsetMilliseconds(int repeat) const;

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertMarkupAnalytical
     */
    virtual int ConvertMarkupAnalyticalEnd(FunctorParams *functorParams);

    /**
     * See Object::ConvertToPageBased
     */
    virtual int ConvertToPageBased(FunctorParams *functorParams);

    /**
     * See Object::ConvertToCastOffMensural
     */
    virtual int ConvertToCastOffMensural(FunctorParams *params);

    /**
     * See Object::ConvertToUnCastOffMensural
     */
    virtual int ConvertToUnCastOffMensural(FunctorParams *params);

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
     * See Object::OptimizeScoreDef
     */
    virtual int OptimizeScoreDef(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::ApplyPPUFactor
     */
    virtual int ApplyPPUFactor(FunctorParams *functorParams);

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
     * See Object::SetAlignmentXPos
     */
    virtual int SetAlignmentXPos(FunctorParams *functorParams);

    /**
     * See Object::AdjustArpeg
     */
    virtual int AdjustArpegEnd(FunctorParams *functorParams);

    /**
     * See Object::AdjustLayers
     */
    virtual int AdjustLayers(FunctorParams *functorParams);

    /**
     * See Object::AjustAccidX
     */
    virtual int AdjustAccidX(FunctorParams *functorParams);

    /**
     * See Object::AdjustGraceXPos
     */
    virtual int AdjustGraceXPos(FunctorParams *functorParams);

    /**
     * See Object::AdjustXOverflow
     */
    virtual int AdjustXOverflow(FunctorParams *functorParams);

    /**
     * See Object::AdjustXPos
     */
    virtual int AdjustXPos(FunctorParams *functorParams);

    /**
     * See Object::AdjustHarmGrpsSpacing
     */
    virtual int AdjustHarmGrpsSpacingEnd(FunctorParams *functorParams);

    /**
     * See Object::AdjustSylSpacing
     */
    virtual int AdjustSylSpacingEnd(FunctorParams *functorParams);

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
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::FillStaffCurrentTimeSpanningEnd
     */
    virtual int FillStaffCurrentTimeSpanningEnd(FunctorParams *functorParams);

    /**
     * See Object::PrepareCrossStaff
     */
    virtual int PrepareCrossStaff(FunctorParams *functorParams);

    /**
     * @name See Object::PrepareFloatingGrps
     */
    ///@{
    virtual int PrepareFloatingGrps(FunctorParams *functoParams);
    virtual int PrepareFloatingGrpsEnd(FunctorParams *functoParams);
    ///@}

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
    ///@}

    /**
     * @name See Object::GenerateTimemap
     */
    ///@{
    virtual int GenerateTimemap(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::CalcMaxMeasureDuration
     */
    virtual int CalcMaxMeasureDuration(FunctorParams *functorParams);

    /**
     * See Object::CalcOnsetOffset
     */
    ///@{
    virtual int CalcOnsetOffset(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::PrepareTimestamps
     */
    virtual int PrepareTimestampsEnd(FunctorParams *functorParams);

public:
    /**
     * The X absolute position of the measure for facsimile (transcription) encodings.
     * This is the left and right position of the measure.
     */
    ///@{
    int m_xAbs;
    int m_xAbs2;
    ///@}

    /**
     * The measure aligner that holds the x positions of the content of the measure
     */
    mutable MeasureAligner m_measureAligner;

    TimestampAligner m_timestampAligner;

protected:
    /**
     * The X relative position of the measure.
     * It is used internally when calculating the layout and it is not stored in the file.
     */
    int m_drawingXRel;

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

    /**
     * A flag indicating if the measure has AlignmentReference with multiple layers
     */
    bool m_hasAlignmentRefWithMultipleLayers;

    /**
     * Start time state variables.
     */
    std::vector<double> m_scoreTimeOffset;
    std::vector<double> m_realTimeOffsetMilliseconds;
    int m_currentTempo;
};

} // namespace vrv

#endif
