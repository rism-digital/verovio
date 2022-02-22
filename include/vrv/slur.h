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

// Helper enum classes
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
    TimePointInterface *GetTimePointInterface() override { return dynamic_cast<TimePointInterface *>(this); }
    TimeSpanningInterface *GetTimeSpanningInterface() override { return dynamic_cast<TimeSpanningInterface *>(this); }
    ///@}

    /**
     * @name Getter, setter and checker for the drawing curve direction and cross-staff flag
     */
    ///@{
    curvature_CURVEDIR GetDrawingCurvedir() const { return m_drawingCurvedir; }
    void SetDrawingCurvedir(curvature_CURVEDIR curvedir) { m_drawingCurvedir = curvedir; }
    bool HasDrawingCurvedir() const { return (m_drawingCurvedir != curvature_CURVEDIR_NONE); }
    ///@}

    /**
     * Adjust starting coordinates for the slurs depending on the curve direction and spanning type of the slur
     */
    std::pair<Point, Point> AdjustCoordinates(
        Doc *doc, Staff *staff, std::pair<Point, Point> points, int spanningType, curvature_CURVEDIR drawingCurveDir);

    /**
     * Determine layer elements spanned by the slur
     */
    std::vector<LayerElement *> CollectSpannedElements(Staff *staff, int xMin, int xMax, char spanningType);

    /**
     * Calculate the staff where the slur's floating curve positioner lives
     */
    Staff *CalculateExtremalStaff(Staff *staff, int xMin, int xMax, char spanningType);

    /**
     * Slur adjustment
     */
    ///@{
    void AdjustSlur(Doc *doc, FloatingCurvePositioner *curve, Staff *staff);

    float GetAdjustedSlurAngle(Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetDrawing
     */
    int ResetDrawing(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareSlurs
     */
    int PrepareSlurs(FunctorParams *functorParams) override;

private:
    /**
     * Helper for calculating the slur direction
     */
    ///@{
    // Get layer by only considering the slur boundary
    std::pair<Layer *, LayerElement *> GetBoundaryLayer();
    // Get cross staff by only considering the slur boundary
    Staff *GetBoundaryCrossStaff();
    // Get preferred curve direction based on various conditions
    curvature_CURVEDIR GetPreferredCurveDirection(Doc *doc, data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter);
    ///@}

    /**
     * Helper for calculating the initial slur start and end points
     */
    ///@{
    // Retrieve the start and end note locations of the slur
    std::pair<int, int> GetStartEndLocs(
        Note *startNote, Chord *startChord, Note *endNote, Chord *endChord, curvature_CURVEDIR dir) const;
    // Calculate the break location at system start/end and the pitch difference
    std::pair<int, int> CalcBrokenLoc(Staff *staff, int startLoc, int endLoc, curvature_CURVEDIR dir) const;
    // Check if the slur resembles portato
    PortatoSlurType IsPortatoSlur(Doc *doc, Note *startNote, Chord *startChord, curvature_CURVEDIR curveDir) const;
    ///@}

    /**
     * Adjust slur position based on overlapping objects within its spanning elements
     */
    ///@{
    // Discard certain spanned elements
    void FilterSpannedElements(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);

    // Calculate the vertical shift of the slur end points
    std::pair<int, int> CalcEndPointShift(
        FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin, int unit);

    // Calculate the horizontal control point offset
    std::tuple<bool, int, int> CalcControlPointOffset(
        FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);

    // Calculate the vertical control point shift
    std::pair<int, int> CalcControlPointVerticalShift(
        FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);

    // Helper function to determine curve direction for the slurs that start at grace note
    curvature_CURVEDIR GetGraceCurveDirection(Doc *doc);

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

    // Rebalance shifts to avoid awkward tilting of short slurs
    void RebalanceShifts(int &shiftLeft, int &shiftRight, double distance, int unit) const;

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
