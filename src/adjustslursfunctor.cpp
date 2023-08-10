/////////////////////////////////////////////////////////////////////////////
// Name:        adjustslursfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjustslursfunctor.h"

//----------------------------------------------------------------------------

#include "doc.h"
#include "floatingobject.h"
#include "slur.h"
#include "staff.h"
#include "system.h"
#include "verticalaligner.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustSlursFunctor
//----------------------------------------------------------------------------

AdjustSlursFunctor::AdjustSlursFunctor(Doc *doc) : DocFunctor(doc)
{
    m_crossStaffSlurs = false;
}

FunctorCode AdjustSlursFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    Staff *staff = staffAlignment->GetStaff();
    if (!staff) return FUNCTOR_CONTINUE;
    const int unit = m_doc->GetDrawingUnit(staff->m_drawingStaffSize);

    // Adjust each slur such that spanned elements are avoided
    ArrayOfFloatingCurvePositioners positioners;
    for (FloatingPositioner *positioner : staffAlignment->GetFloatingPositioners()) {
        assert(positioner->GetObject());
        if (!positioner->GetObject()->Is({ PHRASE, SLUR })) continue;
        Slur *slur = vrv_cast<Slur *>(positioner->GetObject());
        assert(slur);

        assert(positioner->Is(FLOATING_CURVE_POSITIONER));
        FloatingCurvePositioner *curve = vrv_cast<FloatingCurvePositioner *>(positioner);
        assert(curve);

        // Skip if no content bounding box is available
        if (!curve->HasContentBB()) continue;
        positioners.push_back(curve);

        slur->AdjustSlur(m_doc, curve, unit);

        if (curve->IsCrossStaff()) {
            m_crossStaffSlurs = true;
        }
    }

    // Detection of inner slurs
    std::map<FloatingCurvePositioner *, ArrayOfFloatingCurvePositioners> innerCurveMap;
    for (int i = 0; i < (int)positioners.size(); ++i) {
        Slur *firstSlur = vrv_cast<Slur *>(positioners[i]->GetObject());
        ArrayOfFloatingCurvePositioners innerCurves;
        for (int j = 0; j < (int)positioners.size(); ++j) {
            if (i == j) continue;
            Slur *secondSlur = vrv_cast<Slur *>(positioners[j]->GetObject());
            // Check if second slur is inner slur of first
            if (positioners[j]->GetSpanningType() == SPANNING_START_END) {
                if (firstSlur->HasInnerSlur(secondSlur)) {
                    innerCurves.push_back(positioners[j]);
                    continue;
                }
            }
            // Adjust positioning of slurs with common start/end
            Point points1[4], points2[4];
            positioners[i]->GetPoints(points1);
            positioners[j]->GetPoints(points2);
            if ((firstSlur->GetEnd() == secondSlur->GetStart())
                && BoundingBox::ArePointsClose(points1[3], points2[0], unit)) {
                positioners[i]->MoveBackHorizontal(-unit / 2);
                positioners[j]->MoveFrontHorizontal(unit / 2);
            }
            if ((firstSlur->GetStart() == secondSlur->GetStart())
                && BoundingBox::ArePointsClose(points1[0], points2[0], unit) && (points1[3].x > points2[3].x)) {
                int diff = points2[0].y - points1[0].y;
                diff += ((positioners[i]->GetDir() == curvature_CURVEDIR_below) ? -unit : unit);
                positioners[i]->MoveFrontVertical(diff);
            }
            if ((firstSlur->GetEnd() == secondSlur->GetEnd())
                && BoundingBox::ArePointsClose(points1[3], points2[3], unit) && (points1[0].x < points2[0].x)) {
                int diff = points2[3].y - points1[3].y;
                diff += ((positioners[i]->GetDir() == curvature_CURVEDIR_below) ? -unit : unit);
                positioners[i]->MoveBackVertical(diff);
            }
        }
        if (!innerCurves.empty()) {
            innerCurveMap[positioners[i]] = innerCurves;
        }
    }

    // Adjust outer slurs w.r.t. inner slurs
    for (const auto &mapEntry : innerCurveMap) {
        Slur *slur = vrv_cast<Slur *>(mapEntry.first->GetObject());
        assert(slur);
        slur->AdjustOuterSlur(m_doc, mapEntry.first, mapEntry.second, unit);
    }

    return FUNCTOR_SIBLINGS;
}

FunctorCode AdjustSlursFunctor::VisitSystem(System *system)
{
    system->m_systemAligner.Process(*this);

    return FUNCTOR_SIBLINGS;
}

void AdjustSlursFunctor::ShiftEndPoints(const Slur *slur, int &shiftLeft, int &shiftRight, double ratio,
    int intersection, double flexibility, bool isBelow, char spanningType) const
{
    // Filter collisions near the endpoints
    // Collisions with ratio beyond the partialShiftRadius do not contribute to shifts
    // They are compensated later by shifting the control points
    double fullShiftRadius = 0.0;
    double partialShiftRadius = 0.0;
    std::tie(fullShiftRadius, partialShiftRadius) = this->CalcShiftRadii(true, flexibility, spanningType);

    if ((ratio < partialShiftRadius) && (slur->HasEndpointAboveStart() == isBelow)) {
        if (ratio > fullShiftRadius) {
            // Collisions here only partially contribute to shifts
            // We multiply with a function that interpolates between 1 and 0
            intersection *= this->CalcQuadraticInterpolation(partialShiftRadius, fullShiftRadius, ratio);
        }
        shiftLeft = std::max(shiftLeft, intersection);
    }

    std::tie(fullShiftRadius, partialShiftRadius) = this->CalcShiftRadii(false, flexibility, spanningType);

    if ((ratio > 1.0 - partialShiftRadius) && (slur->HasEndpointAboveEnd() == isBelow)) {
        if (ratio < 1.0 - fullShiftRadius) {
            // Collisions here only partially contribute to shifts
            // We multiply with a function that interpolates between 0 and 1
            intersection *= this->CalcQuadraticInterpolation(1.0 - partialShiftRadius, 1.0 - fullShiftRadius, ratio);
        }
        shiftRight = std::max(shiftRight, intersection);
    }
}

std::pair<double, double> AdjustSlursFunctor::CalcShiftRadii(
    bool forShiftLeft, double flexibility, char spanningType) const
{
    // Use full flexibility for broken slur endpoints
    if (forShiftLeft) {
        if ((spanningType == SPANNING_MIDDLE) || (spanningType == SPANNING_END)) {
            flexibility = 1.0;
        }
    }
    else {
        if ((spanningType == SPANNING_START) || (spanningType == SPANNING_MIDDLE)) {
            flexibility = 1.0;
        }
    }

    const double fullShiftRadius = 0.05 + flexibility * 0.15;
    const double partialShiftRadius = fullShiftRadius * 3.0;

    return { fullShiftRadius, partialShiftRadius };
}

double AdjustSlursFunctor::CalcQuadraticInterpolation(double zeroAt, double oneAt, double arg) const
{
    assert(zeroAt != oneAt);
    const double a = 1.0 / (oneAt - zeroAt);
    const double b = zeroAt / (zeroAt - oneAt);
    return pow(a * arg + b, 2.0);
}

double AdjustSlursFunctor::RotateSlope(double slope, double degrees, double doublingBound, bool upwards) const
{
    assert(degrees >= 0.0);
    assert(doublingBound >= 0.0);

    if (upwards && (slope >= doublingBound)) return slope * 2.0;
    if (!upwards && (slope <= -doublingBound)) return slope * 2.0;
    const int sign = upwards ? 1 : -1;
    return tan(atan(slope) + sign * M_PI * degrees / 180.0);
}

float AdjustSlursFunctor::GetMinControlPointAngle(const BezierCurve &bezierCurve, float angle, int unit) const
{
    angle = abs(angle);
    const double distance = double(bezierCurve.p2.x - bezierCurve.p1.x) / unit;

    // Increase min angle for short and angled slurs
    double angleIncrement = std::min(angle / 4.0, 15.0); // values in [0.0, 15.0]
    double factor = 1.0 - (distance - 8.0) / 8.0;
    factor = std::min(factor, 1.0);
    factor = std::max(factor, 0.0); // values in [0.0, 1.0]

    // not if control points are horizontally in a degenerated position
    if ((bezierCurve.c1.x < bezierCurve.p1.x) || (2.0 * bezierCurve.c1.x > bezierCurve.p1.x + bezierCurve.p2.x)) {
        angleIncrement = 0.0;
    }
    if ((bezierCurve.c2.x > bezierCurve.p2.x) || (2.0 * bezierCurve.c2.x < bezierCurve.p1.x + bezierCurve.p2.x)) {
        angleIncrement = 0.0;
    }

    return 30.0 + angleIncrement * factor;
}

} // namespace vrv
