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
// CPInequality
//----------------------------------------------------------------------------
struct CPInequality {
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

    bool AdjustSlur(Doc *doc, FloatingCurvePositioner *curve, Staff *staff);

    void AdjustControlPointHeight(Doc *doc, BezierCurve &bezierCurve, float angle, int staffSize);

    /**
     * Adjust slur position based on overlapping objects within its spanning elements
     */
    void AdjustSlurPosition(Doc *doc, FloatingCurvePositioner *curve, BezierCurve &bezierCurve);

    std::pair<int, int> CalcEndShift(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);
    std::tuple<bool, int, int> CalcCPOffset(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve);
    std::pair<int, int> CalcCPVerticalShift(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve, int margin);
    std::pair<int, int> SolveCPConstraints(const std::list<CPInequality> &constraints);

    float GetAdjustedSlurAngle(Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir, bool withPoints);
    void GetControlPoints(BezierCurve &curve, curvature_CURVEDIR curveDir, bool ignoreAngle = false);

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
    //
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
