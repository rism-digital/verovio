/////////////////////////////////////////////////////////////////////////////
// Name:        adjustslursfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTSLURSFUNCTOR_H__
#define __VRV_ADJUSTSLURSFUNCTOR_H__

#include "functor.h"
#include "slur.h"

namespace vrv {

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

//----------------------------------------------------------------------------
// AdjustSlursFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of slurs.
 */
class AdjustSlursFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustSlursFunctor(Doc *doc);
    virtual ~AdjustSlursFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Check existence of cross-staff slurs
     */
    bool HasCrossStaffSlurs() const { return m_crossStaffSlurs; }

    /*
     * Reset the current slur and curve
     */
    void ResetCurrent();

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitStaffAlignment(StaffAlignment *staffAlignment) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    /**
     * High level helper functions
     */
    ///@{
    void AdjustSlur(int unit) const;

    void AdjustOuterSlur(const ArrayOfFloatingCurvePositioners &innerCurves, int unit) const;
    ///@}

    /**
     * Intermediate level helper functions
     */
    ///@{
    // Discard certain spanned elements
    void FilterSpannedElements(const BezierCurve &bezierCurve, int margin) const;

    // Detect collisions near the endpoints
    NearEndCollision DetectCollisionsNearEnd(const BezierCurve &bezierCurve, int margin) const;

    // Calculate the vertical shift of the slur end points
    std::pair<int, int> CalcEndPointShift(const BezierCurve &bezierCurve, double flexibility, int margin) const;

    // Apply the vertical shift of the slur end points
    void ApplyEndPointShift(BezierCurve &bezierCurve, int endPointShiftLeft, int endPointShiftRight) const;

    // Calculate slur from bulge values
    void AdjustSlurFromBulge(BezierCurve &bezierCurve, int unit) const;

    // Check whether control points should be adjusted horizontally
    bool AllowControlOffsetAdjustment(const BezierCurve &bezierCurve, double symmetry, int unit) const;

    // Calculate the horizontal control point offset
    std::tuple<bool, int, int> CalcControlPointOffset(const BezierCurve &bezierCurve, int margin) const;

    // Calculate the vertical control point shift
    ControlPointAdjustment CalcControlPointVerticalShift(
        const BezierCurve &bezierCurve, double symmetry, int margin) const;

    // Solve the constraints for vertical control point adjustment
    std::pair<int, int> SolveControlPointConstraints(
        const std::list<ControlPointConstraint> &constraints, double symmetry = 0.0) const;

    // Improve the slur shape by adjusting the control point heights
    void AdjustSlurShape(BezierCurve &bezierCurve, curvature_CURVEDIR dir, int unit) const;
    ///@}

    /**
     * Intermediate level helper functions for adjustment w.r.t. inner slurs
     */
    ///@{
    // Calculate the vertical control point shift
    ControlPointAdjustment CalcControlPointShift(const BezierCurve &bezierCurve,
        const ArrayOfFloatingCurvePositioners &innerCurves, double symmetry, int margin) const;

    // Calculate the vertical shift of the slur end points
    std::pair<int, int> CalcEndPointShift(const BezierCurve &bezierCurve,
        const ArrayOfFloatingCurvePositioners &innerCurves, double flexibility, int margin) const;
    ///@}

    /**
     * Low level helper functions
     */
    ///@{
    // Shift end points for collisions nearby
    void ShiftEndPoints(int &shiftLeft, int &shiftRight, double ratio, int intersection, double flexibility,
        bool isBelow, char spanningType) const;

    // Calculate the full and partial shift radii
    std::pair<double, double> CalcShiftRadii(bool forShiftLeft, double flexibility, char spanningType) const;

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
    // Indicates that there is at least one cross-staff slur
    bool m_crossStaffSlurs;
    // The current slur
    Slur *m_currentSlur;
    // The curve positioner
    FloatingCurvePositioner *m_currentCurve;
};

} // namespace vrv

#endif // __VRV_ADJUSTSLURSFUNCTOR_H__
