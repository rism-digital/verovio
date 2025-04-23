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

// Helper enum classes
enum class SlurCurveDirection { None, Above, Below, AboveBelow, BelowAbove };
enum class PortatoSlurType { None, StemSide, Centered };

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

class Slur : public ControlElement,
             public TimeSpanningInterface,
             public AttCurvature,
             public AttLayerIdent,
             public AttLineRendBase {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Slur();
    Slur(ClassId classId);
    virtual ~Slur();
    Object *Clone() const override { return new Slur(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "slur"; }
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
    Staff *CalculatePrincipalStaff(const Staff *staff, int xMin, int xMax);
    const Staff *CalculatePrincipalStaff(const Staff *staff, int xMin, int xMax) const;
    ///@}

    /**
     * Determine the slur's layer/cross staff by only considering the boundary
     */
    ///@{
    std::pair<const Layer *, const LayerElement *> GetBoundaryLayer() const;
    const Staff *GetBoundaryCrossStaff() const;
    ///@}

    /**
     * Set the bezier control sides depending on the curve direction
     */
    void InitBezierControlSides(BezierCurve &bezier, curvature_CURVEDIR curveDir) const;

    //----------//
    // Functors //
    //----------//

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
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
    // Discard tuplets that don't have to be considered for slur adjustment
    void DiscardTupletElements(FloatingCurvePositioner *curve, int xMin, int xMax);
    ///@}

    /**
     * Helper for calculating the initial slur
     */
    ///@{
    // Calculate the endpoint coordinates depending on the curve direction and spanning type of the slur
    std::pair<Point, Point> CalcEndPoints(const Doc *doc, const Staff *staff, NearEndCollision *nearEndCollision,
        int x1, int x2, curvature_CURVEDIR drawingCurveDir, char spanningType) const;
    // Consider the melodic direction for the break location?
    bool ConsiderMelodicDirection() const;
    // Retrieve the start and end note locations of the slur
    std::pair<int, int> GetStartEndLocs(
        const Note *startNote, const Chord *startChord, const Note *endNote, const Chord *endChord) const;
    // Calculate the pitch difference used to adjust for melodic direction
    int CalcPitchDifference(const Staff *staff, int startLoc, int endLoc) const;
    // Check if the slur resembles portato
    PortatoSlurType IsPortatoSlur(const Doc *doc, const Note *startNote, const Chord *startChord) const;
    // Check if the slur starts or ends on a beam
    bool StartsOnBeam() const { return this->HasBoundaryOnBeam(true); }
    bool EndsOnBeam() const { return this->HasBoundaryOnBeam(false); }
    bool HasBoundaryOnBeam(bool isStart) const;
    // Angle adjustment
    float GetAdjustedSlurAngle(const Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir) const;
    ///@}

public:
    //
private:
    /**
     * The drawing curve direction
     * This is calculated in the CalcSlurDirectionFunctor and contains an additional distinction
     * for s-shaped slurs / mixed direction
     */
    SlurCurveDirection m_drawingCurveDir;
};

} // namespace vrv

#endif
