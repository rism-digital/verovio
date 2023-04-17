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

class ControlElement;
class Ending;
class LayerElement;
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
                public AttCoordX1,
                public AttCoordX2,
                public AttMeasureLog,
                public AttMeterConformanceBar,
                public AttNNumberLike,
                public AttPointing,
                public AttTyped {
private:
    using BarlineRenditionPair = std::pair<data_BARRENDITION, data_BARRENDITION>;

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Measure(bool measuredMusic = true, int logMeasureNb = -1);
    virtual ~Measure();
    Object *Clone() const override { return new Measure(*this); };
    void Reset() override;
    std::string GetClassName() const override { return "Measure"; }
    ///@}

    /**
     * Overriding CloneReset() method to be called after copy / assignment calls.
     */
    void CloneReset() override;

    /**
     * Return true if measured music (otherwise we have fake measures)
     */
    bool IsMeasuredMusic() const { return m_measuredMusic; }

    /**
     * Get and set the measure index
     */
    ///@{
    int GetIndex() const { return m_index; }
    void SetIndex(int index) { m_index = index; }
    ///@}

    /**
     * Methods for adding allowed content
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * Specific method for measures
     */
    void AddChildBack(Object *object);

    /**
     * Return true if the Measure has cached values for the horizontal layout
     */
    bool HasCachedHorizontalLayout() const { return (m_cachedWidth != VRV_UNSET); }

    /**
     * Get the X drawing position
     */
    int GetDrawingX() const override;

    /**
     * Reset the cached values of the drawingX values.
     */
    void ResetCachedDrawingX() const override;

    /**
     * @name Get and set the X drawing relative positions
     */
    ///@{
    int GetDrawingXRel() const { return m_drawingXRel; }
    void SetDrawingXRel(int drawingXRel);
    void CacheXRel(bool restore = false);
    int GetCachedXRel() const { return m_cachedXRel; }
    void ResetCachedXRel() { m_cachedXRel = VRV_UNSET; }
    ///@}

    /**
     * @name Check if the measure is the first or last in the system
     */
    ///@{
    bool IsFirstInSystem() const;
    bool IsLastInSystem() const;
    ///@}

    /**
     * Return the index position of the measure in its system parent
     */
    int GetMeasureIdx() const { return Object::GetIdx(); }

    /**
     * @name Set and get the left and right barline types
     * This somehow conflicts with AttMeasureLog, which is transferred from and to the
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
    data_BARRENDITION GetDrawingLeftBarLineByStaffN(int staffN) const;
    data_BARRENDITION GetDrawingRightBarLineByStaffN(int staffN) const;
    ///@}

    /**
     * Return whether there is mapping of barline values to invisible staves present in measure
     */
    bool HasInvisibleStaffBarlines() const { return !m_invisibleStaffBarlines.empty(); }

    /**
     * Select drawing barlines based on the previous right and current left barlines (to avoid duplicated doubles or
     * singles). In certain cases drawn barlines would be simplified if they can be overlapped, e.g. single with dbl
     */
    BarlineRenditionPair SelectDrawingBarLines(const Measure *previous) const;

    /**
     * Set the drawing barlines for the measure.
     * Also adjust the right barline of the previous measure and the left one if necessary.
     */
    void SetDrawingBarLines(Measure *previous, int barlineDrawingFlags);

    /**
     * Create mapping of original barline values to staves in the measure that are neighbored by invisible staves. This
     * will allow to draw proper barline when invisible staff hides overlapping barline
     */
    void SetInvisibleStaffBarlines(
        Measure *previous, ListOfObjects &currentInvisible, ListOfObjects &previousInvisible, int barlineDrawingFlags);

    /**
     * @name Set and get the barlines.
     * Careful - the barlines are owned by the measure and will be destroyed by it.
     * This method should be used only for acessing them (e.g., when drawing) and
     * not for creating other measure objects.
     */
    ///@{
    BarLine *GetLeftBarLine() { return &m_leftBarLine; }
    const BarLine *GetLeftBarLine() const { return &m_leftBarLine; }
    BarLine *GetRightBarLine() { return &m_rightBarLine; }
    const BarLine *GetRightBarLine() const { return &m_rightBarLine; }
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
     * Return the width of the right barline based on the barline form
     */
    int CalculateRightBarLineWidth(const Doc *doc, int staffSize) const;

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
     * Return and reset the cached width / overflow
     */
    ///@{
    int GetCachedWidth() const { return m_cachedWidth; }
    int GetCachedOverflow() const { return m_cachedOverflow; }
    void ResetCachedWidth() { m_cachedWidth = VRV_UNSET; }
    void ResetCachedOverflow() { m_cachedOverflow = VRV_UNSET; }
    ///@}

    /**
     * Return the right overflow of the control events in the measure.
     * Takes into account Dir, Dynam, and Tempo.
     */
    int GetDrawingOverflow();

    /**
     * Calculates the section restart shift
     */
    int GetSectionRestartShift(const Doc *doc) const;

    /**
     * @name Setter and getter of the drawing scoreDef
     */
    ///@{
    ScoreDef *GetDrawingScoreDef() { return m_drawingScoreDef; }
    const ScoreDef *GetDrawingScoreDef() const { return m_drawingScoreDef; }
    void SetDrawingScoreDef(ScoreDef *drawingScoreDef);
    void ResetDrawingScoreDef();
    ///@}

    /**
     * @name Setter and getter of the drawing ending
     */
    ///@{
    Ending *GetDrawingEnding() { return m_drawingEnding; }
    const Ending *GetDrawingEnding() const { return m_drawingEnding; }
    void SetDrawingEnding(Ending *ending) { m_drawingEnding = ending; }
    ///@}

    /**
     * @name Setter and getter for the flag indicating if there is an AlignmentReference
     * with multiple layers
     */
    ///@{
    bool HasAlignmentRefWithMultipleLayers() const { return m_hasAlignmentRefWithMultipleLayers; }
    void HasAlignmentRefWithMultipleLayers(bool hasRef) { m_hasAlignmentRefWithMultipleLayers = hasRef; }
    ///@}

    /*
     * Return the first staff of each staffGrp according to the scoreDef
     */
    std::vector<Staff *> GetFirstStaffGrpStaves(ScoreDef *scoreDef);

    /**
     * Return the top (first) visible staff in the measure (if any).
     * Takes into account system optimization
     */
    ///@{
    Staff *GetTopVisibleStaff();
    const Staff *GetTopVisibleStaff() const;
    ///@}

    /**
     * Return the bottom (last) visible staff in the measure (if any).
     * Takes into account system optimization
     */
    ///@{
    Staff *GetBottomVisibleStaff();
    const Staff *GetBottomVisibleStaff() const;
    ///@}

    /**
     * Check if the measure encloses the given time (in millisecond)
     * Return the playing repeat time (1-based), 0 otherwise
     */
    int EnclosesTime(int time) const;

    /**
     * Return the real time offset in millisecond for the repeat (1-based).
     */
    double GetRealTimeOffsetMilliseconds(int repeat) const;

    /**
     * Return vector with tie endpoints for ties that start and end in current measure
     */
    std::vector<std::pair<LayerElement *, LayerElement *>> GetInternalTieEndpoints();

    /**
     * Read only access to m_scoreTimeOffset
     */
    double GetLastTimeOffset() const { return m_scoreTimeOffset.back(); }

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(MutableFunctor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(MutableFunctor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

    /**
     * See Object::ConvertMarkupAnalytical
     */
    int ConvertMarkupAnalyticalEnd(FunctorParams *functorParams) override;

    /**
     * See Object::ConvertToPageBased
     */
    int ConvertToPageBased(FunctorParams *functorParams) override;

    /**
     * See Object::ConvertToCastOffMensural
     */
    int ConvertToCastOffMensural(FunctorParams *functorParams) override;

    /**
     * See Object::ConvertToUnCastOffMensural
     */
    int ConvertToUnCastOffMensural(FunctorParams *functorParams) override;

    /**
     * See Object::Save
     */
    ///@{
    int Save(FunctorParams *functorParams) override;
    int SaveEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::ApplyPPUFactor
     */
    int ApplyPPUFactor(FunctorParams *functorParams) override;

    /**
     * See Object::InitMIDI
     */
    int InitMIDI(FunctorParams *functorParams) override;

    /**
     * See Object::GenerateMIDI
     */
    int GenerateMIDI(FunctorParams *functorParams) override;

    /**
     * See Object::GenerateTimemap
     */
    int GenerateTimemap(FunctorParams *functorParams) override;

    /**
     * See Object::CalcMaxMeasureDuration
     */
    ///@{
    int InitMaxMeasureDuration(FunctorParams *functorParams) override;
    int InitMaxMeasureDurationEnd(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::InitOnsetOffset
     */
    int InitOnsetOffset(FunctorParams *functorParams) override;

public:
    // flags for drawing measure barline based on visibility or other conditions
    enum BarlineDrawingFlags {
        SYSTEM_BREAK = 0x1,
        SCORE_DEF_INSERT = 0x2,
        INVISIBLE_MEASURE_CURRENT = 0x4,
        INVISIBLE_MEASURE_PREVIOUS = 0x8
    };
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

    /**
     * The cached value for m_drawingXRel for caching horizontal layout
     */
    int m_cachedXRel;

    /**
     * @name Cached values of overflow and width for caching the horizontal layout
     */
    ///@{
    int m_cachedOverflow;
    int m_cachedWidth;
    ///@}

private:
    /**
     * Indicates measured music (otherwise we have fake measures)
     */
    bool m_measuredMusic;

    /**
     * The unique measure index
     */
    int m_index;

    /**
     * @name The measure barlines (left and right) used when drawing
     */
    ///@{
    BarLine m_leftBarLine;
    BarLine m_rightBarLine;
    ///@}

    /**
     * A pointer to the drawing ScoreDef instance. It is owned by the measure and added to a measure when a scoreDef
     * change before or requires it. This include scoreDef elements before it but also clef changes within the previous
     * measure.
     */
    ScoreDef *m_drawingScoreDef;

    /**
     * A pointer to the ending to which the measure belongs. Set by PrepareMilestonesFunctor
     * and passed to the System drawing list in DrawMeasure
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
    double m_currentTempo;

    std::map<int, BarlineRenditionPair> m_invisibleStaffBarlines;
};

} // namespace vrv

#endif
