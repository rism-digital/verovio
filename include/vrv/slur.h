/////////////////////////////////////////////////////////////////////////////
// Name:        slur.h
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SLUR_H__
#define __VRV_SLUR_H__

#include "controlelement.h"
#include "timeinterface.h"

namespace vrv {

class Chord;
class Doc;
class Layer;
class Note;
class Staff;

//----------------------------------------------------------------------------
// SpannedElements
//----------------------------------------------------------------------------
/**
 * Contains the layer elements used for collision detection
 */
struct SpannedElements {
    std::vector<const LayerElement *> elements;
    std::set<int> layersN;
};

//----------------------------------------------------------------------------
// NearEndCollision
//----------------------------------------------------------------------------
/**
 * Measure collisions near the end points
 */
struct NearEndCollision {
    double metricAtStart;
    double metricAtEnd;
    bool endPointsAdjusted;
};

//----------------------------------------------------------------------------
// ControlPointConstraint
//----------------------------------------------------------------------------
/**
 * This represents a constraint ax + by >= c where x and y are
 * vertical control point adjustments
 */
struct ControlPointConstraint {
    double a;
    double b;
    double c;
};

//----------------------------------------------------------------------------
// ControlPointAdjustment
//----------------------------------------------------------------------------
/**
 * A vertical adjustment of bezier control points
 */
struct ControlPointAdjustment {
    int leftShift;
    int rightShift;
    bool moveUpwards;
    int requestedStaffSpace;
};

// Helper enum classes
enum class SlurCurveDirection { None, Above, Below, AboveBelow, BelowAbove };
enum class PortatoSlurType { None, StemSide, Centered };

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

class Slur : public ControlElement,
             public TimeSpanningInterface,
             public AttColor,
             public AttCurvature,
             public AttCurveRend,
             public AttLayerIdent {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Slur();
    Slur(ClassId classId);
    Slur(ClassId classId, const std::string &classIdStr);
    virtual ~Slur();
    Object *Clone() const override { return new Slur(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Slur"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    TimePointInterface *GetTimePointInterface() override { return vrv_cast<TimePointInterface *>(this); }
    const TimePointInterface *GetTimePointInterface() const override
    {
        return vrv_cast<const TimePointInterface *>(this);
    }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return vrv_cast<TimeSpanningInterface *>(this); }
    const TimeSpanningInterface *GetTimeSpanningInterface() const override
    {
        return vrv_cast<const TimeSpanningInterface *>(this);
    }
    ///@}

    /**
     * @name Getter, setter and checker for the drawing curve direction
     */
    ///@{
    SlurCurveDirection GetDrawingCurveDir() const { return m_drawingCurveDir; }
    void SetDrawingCurveDir(SlurCurveDirection curveDir) { m_drawingCurveDir = curveDir; }
    bool HasDrawingCurveDir() const { return (m_drawingCurveDir != SlurCurveDirection::None); }
    curvature_CURVEDIR CalcDrawingCurveDir(char spanningType) const;
    ///@}

    /**
     * @name Additional checks based on the drawing curve direction
     */
    ///@{
    bool HasMixedCurveDir() const
    {
        return (m_drawingCurveDir == SlurCurveDirection::AboveBelow)
            || (m_drawingCurveDir == SlurCurveDirection::BelowAbove);
    }
    bool HasEndpointAboveStart() const
    {
        return (m_drawingCurveDir == SlurCurveDirection::Above)
            || (m_drawingCurveDir == SlurCurveDirection::AboveBelow);
    }
    bool HasEndpointBelowStart() const
    {
        return (m_drawingCurveDir == SlurCurveDirection::Below)
            || (m_drawingCurveDir == SlurCurveDirection::BelowAbove);
    }
    bool HasEndpointAboveEnd() const
    {
        return (m_drawingCurveDir == SlurCurveDirection::Above)
            || (m_drawingCurveDir == SlurCurveDirection::BelowAbove);
    }
    bool HasEndpointBelowEnd() const
    {
        return (m_drawingCurveDir == SlurCurveDirection::Below)
            || (m_drawingCurveDir == SlurCurveDirection::AboveBelow);
    }
    ///@}

    /**
     * @name Detection of inner slurs
     */
    bool HasInnerSlur(const Slur *innerSlur) const;

    /**
     * Calculate the initial slur bezier curve and store it in the curve positioner
     */
    void CalcInitialCurve(const Doc *doc, FloatingCurvePositioner *curve, NearEndCollision *nearEndCollision = NULL);

    /**
     * Recalculate the spanned elements of the curve positioner
     */
    void CalcSpannedElements(FloatingCurvePositioner *curve);

    /**
     * Add curve positioner to articulations
     */
    void AddPositionerToArticulations(FloatingCurvePositioner *curve);

    /**
     * Calculate the staff where the slur's floating curve positioner lives
     */
    ///@{
    Staff *CalculateExtremalStaff(const Staff *staff, int xMin, int xMax);
    const Staff *CalculateExtremalStaff(const Staff *staff, int xMin, int xMax) const;
    ///@}

    /**
     * Set the bezier control sides depending on the curve direction
     */
    void InitBezierControlSides(BezierCurve &bezier, curvature_CURVEDIR curveDir) const;

    /**
     * Slur adjustment
     */
    ///@{
    void AdjustSlur(const Doc *doc, FloatingCurvePositioner *curve, int unit);

    void AdjustOuterSlur(
        const Doc *doc, FloatingCurvePositioner *curve, const ArrayOfFloatingCurvePositioners &innerCurves, int unit);

    float GetAdjustedSlurAngle(const Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir) const;
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetData
     */
    int ResetData(FunctorParams *functorParams) override;

    /**
     * See Object::CalcSlurDirection
     */
    int CalcSlurDirection(FunctorParams *functorParams) override;

private:
    /**
     * Helper for calculating the slur direction
     */
    ///@{
    // Get layer by only considering the slur boundary
    std::pair<const Layer *, const LayerElement *> GetBoundaryLayer() const;
    // Get cross staff by only considering the slur boundary
    const Staff *GetBoundaryCrossStaff() const;
    // Determine curve direction for the slurs that start at grace note
    curvature_CURVEDIR GetGraceCurveDirection() const;
    // Get preferred curve direction based on various conditions
    curvature_CURVEDIR GetPreferredCurveDirection(
        data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter, bool isGraceToNoteSlur) const;
    ///@}

    /**
     * Helper for calculating spanned elements
     */
    ///@{
    // Determine layer elements spanned by the slur
    SpannedElements CollectSpannedElements(const Staff *staff, int xMin, int xMax) const;
    // Filter and add layer elements spanned by the slur to the positioner
    void AddSpannedElements(
        FloatingCurvePositioner *curve, const SpannedElements &elements, Staff *staff, int xMin, int xMax);
    // Determine whether a layer element should lie above or below the slur
    bool IsElementBelow(const LayerElement *element, const Staff *startStaff, const Staff *endStaff) const;
    bool IsElementBelow(const FloatingPositioner *positioner, const Staff *startStaff, const Staff *endStaff) const;
    ///@}

    /**
     * Helper for calculating the initial slur
     */
    ///@{
    // Calculate the endpoint coordinates depending on the curve direction and spanning type of the slur
    std::pair<Point, Point> CalcEndPoints(const Doc *doc, const Staff *staff, NearEndCollision *nearEndCollision,
        int x1, int x2, curvature_CURVEDIR drawingCurveDir, char spanningType) const;
    // Retrieve the start and end note locations of the slur
    std::pair<int, int> GetStartEndLocs(
        const Note *startNote, const Chord *startChord, const Note *endNote, const Chord *endChord) const;
    // Calculate the break location at system start/end and the pitch difference
    std::pair<int, int> CalcBrokenLoc(const Staff *staff, int startLoc, int endLoc) const;
    // Check if the slur resembles portato
    PortatoSlurType IsPortatoSlur(const Doc *doc, const Note *startNote, const Chord *startChord) const;
    // Check if the slur starts or ends on a beam
    bool StartsOnBeam() const { return this->HasBoundaryOnBeam(true); }
    bool EndsOnBeam() const { return this->HasBoundaryOnBeam(false); }
    bool HasBoundaryOnBeam(bool isStart) const;
    ///@}

    /**
     * Adjust slur position based on overlapping objects within its spanning elements
     */
    ///@{
    // Discard certain spanned elements
    void FilterSpannedElements(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);

    // Detect collisions near the endpoints
    NearEndCollision DetectCollisionsNearEnd(
        FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);

    // Calculate the vertical shift of the slur end points
    std::pair<int, int> CalcEndPointShift(
        FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, double flexibility, int margin);

    // Apply the vertical shift of the slur end points
    void ApplyEndPointShift(
        FloatingCurvePositioner *curve, BezierCurve &bezierCurve, int endPointShiftLeft, int endPointShiftRight);

    // Calculate slur from bulge values
    void AdjustSlurFromBulge(FloatingCurvePositioner *curve, BezierCurve &bezierCurve, int unit);

    // Check whether control points should be adjusted horizontally
    bool AllowControlOffsetAdjustment(const BezierCurve &bezierCurve, double symmetry, int unit) const;

    // Calculate the horizontal control point offset
    std::tuple<bool, int, int> CalcControlPointOffset(
        FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);

    // Calculate the vertical control point shift
    ControlPointAdjustment CalcControlPointVerticalShift(
        FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, double symmetry, int margin);

    // Solve the constraints for vertical control point adjustment
    std::pair<int, int> SolveControlPointConstraints(
        const std::list<ControlPointConstraint> &constraints, double symmetry = 0.0) const;

    // Improve the slur shape by adjusting the control point heights
    void AdjustSlurShape(BezierCurve &bezierCurve, curvature_CURVEDIR dir, int unit);
    ///@}

    /**
     * Adjust slur position based on inner slurs
     */
    ///@{
    // Calculate the vertical control point shift
    ControlPointAdjustment CalcControlPointShift(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve,
        const ArrayOfFloatingCurvePositioners &innerCurves, double symmetry, int margin);

    // Calculate the vertical shift of the slur end points
    std::pair<int, int> CalcEndPointShift(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve,
        const ArrayOfFloatingCurvePositioners &innerCurves, double flexibility, int margin);
    ///@}

    /**
     * Low level helper functions for slur adjustment
     */
    ///@{
    // Shift end points for collisions nearby
    void ShiftEndPoints(
        int &shiftLeft, int &shiftRight, double ratio, int intersection, double flexibility, bool isBelow) const;

    // Determine a quadratic interpolation function between zero and one and evaluate it
    double CalcQuadraticInterpolation(double zeroAt, double oneAt, double arg) const;

    // Rotate the slope by a given number of degrees, but choose smaller angles if already close to the vertical axis
    // Choose doublingBound as the positive slope value where doubling has the same effect as rotating:
    // tan(atan(doublingBound) + degrees * PI / 180.0) ≈ 2.0 * doublingBound
    double RotateSlope(double slope, double degrees, double doublingBound, bool upwards) const;

    // Calculate the minimal angle <)C1P1P2 or <)P1P2C2
    float GetMinControlPointAngle(const BezierCurve &bezierCurve, float angle, int unit) const;
    ///@}

public:
    //
private:
    /**
     * The drawing curve direction
     * This is calculated in the CalcSlurDirection functor and contains an additional distinction
     * for s-shaped slurs / mixed direction
     */
    SlurCurveDirection m_drawingCurveDir;
};

} // namespace vrv

#endif
