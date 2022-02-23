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
enum class SlurCurveDirection { None, Above, Below, MixedDownwards, MixedUpwards };
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
        return (m_drawingCurveDir == SlurCurveDirection::MixedDownwards)
            || (m_drawingCurveDir == SlurCurveDirection::MixedUpwards);
    }
    bool HasEndpointAboveStart() const
    {
        return (m_drawingCurveDir == SlurCurveDirection::Above)
            || (m_drawingCurveDir == SlurCurveDirection::MixedUpwards);
    }
    bool HasEndpointBelowStart() const
    {
        return (m_drawingCurveDir == SlurCurveDirection::Below)
            || (m_drawingCurveDir == SlurCurveDirection::MixedDownwards);
    }
    bool HasEndpointAboveEnd() const
    {
        return (m_drawingCurveDir == SlurCurveDirection::Above)
            || (m_drawingCurveDir == SlurCurveDirection::MixedDownwards);
    }
    bool HasEndpointBelowEnd() const
    {
        return (m_drawingCurveDir == SlurCurveDirection::Below)
            || (m_drawingCurveDir == SlurCurveDirection::MixedUpwards);
    }
    ///@}

    /**
     * Adjust starting coordinates for the slurs depending on the curve direction and spanning type of the slur
     */
    std::pair<Point, Point> AdjustCoordinates(
        Doc *doc, Staff *staff, std::pair<Point, Point> points, char spanningType);

    /**
     * Determine layer elements spanned by the slur
     */
    std::vector<LayerElement *> CollectSpannedElements(Staff *staff, int xMin, int xMax, char spanningType);

    /**
     * Calculate the staff where the slur's floating curve positioner lives
     */
    Staff *CalculateExtremalStaff(Staff *staff, int xMin, int xMax, char spanningType);

    /**
     * Determine whether a layer element should lie above or below the slur
     */
    ///@{
    bool IsElementBelow(LayerElement *element, Staff *startStaff, Staff *endStaff) const;
    bool IsElementBelow(FloatingPositioner *positioner, Staff *startStaff, Staff *endStaff) const;
    ///@}

    /**
     * Set the bezier control sides depending on the curve direction
     */
    void InitBezierControlSides(BezierCurve &bezier, curvature_CURVEDIR curveDir) const;

    /**
     * Slur adjustment
     */
    ///@{
    void AdjustSlur(Doc *doc, FloatingCurvePositioner *curve, Staff *staff);

    float GetAdjustedSlurAngle(Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir);
    ///@}

    /**
     * Calculate the staff space above and below requested by s-shaped slurs
     */
    std::pair<int, int> CalcRequestedStaffSpace(StaffAlignment *alignment);

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
    // Determine curve direction for the slurs that start at grace note
    curvature_CURVEDIR GetGraceCurveDirection(Doc *doc);
    // Get preferred curve direction based on various conditions
    curvature_CURVEDIR GetPreferredCurveDirection(Doc *doc, data_STEMDIRECTION noteStemDir, bool isAboveStaffCenter);
    ///@}

    /**
     * Helper for calculating the initial slur start and end points
     */
    ///@{
    // Retrieve the start and end note locations of the slur
    std::pair<int, int> GetStartEndLocs(Note *startNote, Chord *startChord, Note *endNote, Chord *endChord) const;
    // Calculate the break location at system start/end and the pitch difference
    std::pair<int, int> CalcBrokenLoc(Staff *staff, int startLoc, int endLoc) const;
    // Check if the slur resembles portato
    PortatoSlurType IsPortatoSlur(Doc *doc, Note *startNote, Chord *startChord) const;
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
    ControlPointAdjustment CalcControlPointVerticalShift(
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
    void ShiftEndPoints(int &shiftLeft, int &shiftRight, double ratio, int intersection, bool isBelow) const;

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
     * The drawing curve direction
     * This is calculated in the PrepareSlurs functor and contains an additional distinction
     * for s-shaped slurs / mixed direction: whether the slur goes upwards or downwards
     */
    SlurCurveDirection m_drawingCurveDir;

    /**
     * The requested staff space
     * S-shaped slurs can request staff space to prevent collisions from two sides
     */
    int m_requestedStaffSpace;
};

} // namespace vrv

#endif
