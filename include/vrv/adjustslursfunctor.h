/////////////////////////////////////////////////////////////////////////////
// Name:        adjustslursfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTSLURSFUNCTOR_H__
#define __VRV_ADJUSTSLURSFUNCTOR_H__

#include "functor.h"

namespace vrv {

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
     * Low level helper functions
     */
    ///@{
    // Shift end points for collisions nearby
    void ShiftEndPoints(const Slur *slur, int &shiftLeft, int &shiftRight, double ratio, int intersection,
        double flexibility, bool isBelow, char spanningType) const;

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
};

} // namespace vrv

#endif // __VRV_ADJUSTSLURSFUNCTOR_H__
