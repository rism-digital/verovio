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

class FloatingElement;
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
    virtual std::string GetClassName() const { return "Measure"; };
    virtual ClassId Is() const { return MEASURE; };
    ///@}

    /**
     * Return true if measured music (otherwise we have fake measures)
     */
    bool IsMeasuredMusic() const { return m_measuredMusic; };

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    void AddStaff(Staff *staff);
    void AddFloatingElement(FloatingElement *element);
    ///@}

    /**
     * Add a TimestampAttr to the measure.
     * The TimestampAttr it not added as child of the measure but to the Measure::m_timestamps array.
     */
    void AddTimestamp(TimestampAttr *timestampAttr);

    /**
     * Return the index position of the measure in its system parent
     */
    int GetMeasureIdx() const { return Object::GetIdx(); };

    /**
     * @name Set and get the left and right barline types
     * This somehow conflicts with AttMeasureLog, which is transfered from and to the
     * Barline object when reading and writing MEI. See MeiInput::ReadMeiMeasure and
     * MeiOutput::ReadMeiMeasure
     * Alternatively, we could keep them in sync here:
     * data_BARRENDITION GetLeftBarLineType() { m_leftBarLine.SetRend(GetRight()); return m_leftBarLine.GetRend(); };
     * void SetLeftBarLineType(data_BARRENDITION type) { m_leftBarLine.SetRend(type); SetLeft(type); };
     */
    ///@{
    data_BARRENDITION GetLeftBarLineType() const { return m_leftBarLine.GetForm(); };
    void SetLeftBarLineType(data_BARRENDITION type) { m_leftBarLine.SetForm(type); };
    data_BARRENDITION GetRightBarLineType() const { return m_rightBarLine.GetForm(); };
    void SetRightBarLineType(data_BARRENDITION type) { m_rightBarLine.SetForm(type); };
    ///@}

    /**
     * @name Set and get the barlines.
     * Careful - the barlines are owned by the measure and will be destroyed by it.
     * This method should be used only for acessing them (e.g., when drawing) and
     * not for creating other measure objects.
     */
    ///@{
    BarLine *const GetLeftBarLine() { return &m_leftBarLine; };
    BarLine *const GetRightBarLine() { return &m_rightBarLine; };
    ///@}

    int GetXRel() const;

    /**
     * Return the non-justifiable left margin for the measure
     */
    int GetNonJustifiableLeftMargin() const { return m_measureAligner.GetNonJustifiableMargin(); }

    /**
     * @name Return the X rel position of the right and left barLine (without their width)
     */
    ///@{
    int GetLeftBarLineX() const;
    int GetRightBarLineX() const;
    ///@}

    /**
     * Return the width of the measure, including the barLine width
     */
    int GetWidth() const;

    //----------//
    // Functors //
    //----------//

    /**
     * @name Reset the horizontal alignment
     */
    ///@{
    virtual int ResetHorizontalAlignment(ArrayPtrVoid *params);
    ///@}

    /**
     * AlignHorizontally the content of a measure.
     */
    virtual int AlignHorizontally(ArrayPtrVoid *params);

    /**
     * Align horizontally the content of a layer.
     */
    virtual int AlignHorizontallyEnd(ArrayPtrVoid *params);

    /**
     * AlignVertically the content of a measure.
     */
    virtual int AlignVertically(ArrayPtrVoid *params);

    /**
     * Correct the X alignment of grace notes once the content of a system has been aligned and laid out.
     * Special case that redirects the functor to the GraceAligner.
     */
    virtual int IntegrateBoundingBoxGraceXShift(ArrayPtrVoid *params);

    /**
     * Correct the X alignment once the content of a system has been aligned and laid out.
     * Special case that redirects the functor to the MeasureAligner.
     */
    virtual int IntegrateBoundingBoxXShift(ArrayPtrVoid *params);

    /**
     * Set the position of the Alignment.
     * Special case that redirects the functor to the MeasureAligner.
     */
    virtual int SetAlignmentXPos(ArrayPtrVoid *params);

    /**
     * Align the measures by adjusting the m_drawingXRel position looking at the MeasureAligner.
     * This method also moves the end position of the measure according to the barLine width.
     */
    virtual int AlignMeasures(ArrayPtrVoid *params);

    /**
     * Justify the X positions
     * Special case that redirects the functor to the MeasureAligner.
     */
    virtual int JustifyX(ArrayPtrVoid *params);

    /**
     * Fill a page by adding systems with the appropriate length
     *
     */
    virtual int CastOffSystems(ArrayPtrVoid *params);

    /**
     * Set the drawing position (m_drawingX and m_drawingY) values for objects
     */
    virtual int SetDrawingXY(ArrayPtrVoid *params);

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(ArrayPtrVoid *params);

    /**
     * See Object::FillStaffCurrentTimeSpanningEnd
     */
    virtual int FillStaffCurrentTimeSpanningEnd(ArrayPtrVoid *params);

    /**
     * See Object::PrepareTimeSpanning.
     */
    virtual int PrepareTimeSpanningEnd(ArrayPtrVoid *params);

    /**
     * See Object::GenerateMIDI
     */
    virtual int GenerateMIDI(ArrayPtrVoid *params);

    /**
     * See Object::GenerateMIDI
     */
    virtual int GenerateMIDIEnd(ArrayPtrVoid *params);

    /**
     * See Object::CalcMaxMeasureDuration
     */
    virtual int CalcMaxMeasureDuration(ArrayPtrVoid *params);

    /**
     * See Object::PrepareTimestamps.
     */
    virtual int PrepareTimestampsEnd(ArrayPtrVoid *params);

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
};

} // namespace vrv

#endif
