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

    int AdjustSlurCurve(Doc *doc, const ArrayOfCurveSpannedElements *spannedElements, BezierCurve &bezierCurve,
        curvature_CURVEDIR curveDir, float angle, int staffSize, bool posRatio = true);

    /**
     * Adjust slur position based on overlapping objects within its spanning elements
     */
    bool AdjustSlurPosition(
        Doc *doc, FloatingCurvePositioner *curve, BezierCurve &bezierCurve, float &angle, bool forceBothSides);
    /**
     * Calculate slur left/right maximum shifts required for slur not to overlap with other objects
     */
    std::pair<int, int> CalculateAdjustedSlurShift(FloatingCurvePositioner *curve, const BezierCurve &bezierCurve,
        int margin, bool forceBothSides, bool &isNotAdjustable);

    float GetAdjustedSlurAngle(Doc *doc, Point &p1, Point &p2, curvature_CURVEDIR curveDir, bool withPoints);
    void GetControlPoints(BezierCurve &curve, curvature_CURVEDIR curveDir, bool ignoreAngle = false);
    void GetSpannedPointPositions(Doc *doc, const ArrayOfCurveSpannedElements *spannedElements, Point p1, float angle,
        curvature_CURVEDIR curveDir, int staffSize);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

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
