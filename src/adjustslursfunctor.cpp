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

} // namespace vrv
