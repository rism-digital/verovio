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
// Slur
//----------------------------------------------------------------------------

class Slur : public ControlElement,
             public TimeSpanningInterface,
             public AttColor,
             public AttCurvature,
             public AttCurveRend {
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
    virtual Object *Clone() const { return new Slur(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Slur"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual TimePointInterface *GetTimePointInterface() { return dynamic_cast<TimePointInterface *>(this); }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return dynamic_cast<TimeSpanningInterface *>(this); }
    ///@}

    /**
     * @name Getter, setter and checker for the drawing curve direction and cross-staff flag
     */
    ///@{
    curvature_CURVEDIR GetDrawingCurvedir() const { return m_drawingCurvedir; }
    void SetDrawingCurvedir(curvature_CURVEDIR curvedir) { m_drawingCurvedir = curvedir; }
    bool HasDrawingCurvedir() const { return (m_drawingCurvedir != curvature_CURVEDIR_NONE); }
    ///@}

    void AdjustSlur(Doc *doc, FloatingCurvePositioner *curve, Staff *staff);

    float GetAdjustedSlurAngle(Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::AdjustCrossStaffContent
     */
    virtual int AdjustCrossStaffContent(FunctorParams *functorParams);

private:
    /**
     * Adjust slur position based on overlapping objects within its spanning elements
     */
    ///@{
    // Discard certain spanned elements
    void FilterSpannedElements(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);

    // Calculate the vertical shift of the slur end points
    std::pair<int, int> CalcEndPointShift(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);

    // Calculate the horizontal control point offset
    std::tuple<bool, int, int> CalcControlPointOffset(
        FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);

    // Calculate the vertical control point shift
    std::pair<int, int> CalcControlPointVerticalShift(
        FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);

    // Solve the constraints for vertical control point adjustment
    std::pair<int, int> SolveControlPointConstraints(const std::list<ControlPointConstraint> &constraints);

    // Improve the slur shape by adjusting the control point heights
    void AdjustSlurShape(BezierCurve &bezierCurve, curvature_CURVEDIR dir, int unit);
    ///@}

    /**
     * Low level helper functions for slur adjustment
     */
    ///@{
    // Shift end points for collisions nearby
    void ShiftEndPoints(int &shiftLeft, int &shiftRight, double ratio, int intersection) const;

    // Rotate the slope by a given number of degrees, but choose smaller angles if already close to the vertical axis
    // Choose doublingBound as the positive slope value where doubling has the same effect as rotating:
    // tan(atan(doublingBound) + degrees * PI / 180.0) ≈ 2.0 * doublingBound
    double RotateSlope(double slope, double degrees, double doublingBound, bool upwards) const;
    ///@}

public:
    //
private:
    /**
     * The drawing curve direction.
     * This is calculated only when start - end points are on the same system. Otherwise
     * it is left unset. This also means that it is reset only in ResetDrawing and not when
     * the alignment is reset. The reason is because we want to preserve the value when the
     * document is cast-off.
     */
    curvature_CURVEDIR m_drawingCurvedir;
};

} // namespace vrv

#endif
