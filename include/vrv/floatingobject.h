/////////////////////////////////////////////////////////////////////////////
// Name:        floatingobject.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FLOATING_OBJECT_H__
#define __VRV_FLOATING_OBJECT_H__

#include "devicecontextbase.h"
#include "object.h"

namespace vrv {

class Alignment;
class FloatingPositioner;
class StaffAlignment;

//----------------------------------------------------------------------------
// FloatingObject
//----------------------------------------------------------------------------

/**
 * This class represents elements appearing within a measure.
 * It is not an abstract class but should not be instanciated directly.
 */
class FloatingObject : public Object {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    FloatingObject();
    FloatingObject(ClassId classId);
    virtual ~FloatingObject();
    void Reset() override;
    void ResetDrawing();
    ///@}

    void UpdateContentBBoxX(int x1, int x2) override;
    void UpdateContentBBoxY(int y1, int y2) override;
    void UpdateSelfBBoxX(int x1, int x2) override;
    void UpdateSelfBBoxY(int y1, int y2) override;

    /**
     * @name Get and set the X and Y drawing position
     */
    ///@{
    int GetDrawingX() const override;
    int GetDrawingY() const override;
    ///@}

    /**
     * @name Get and set the current positioner
     */
    ///@{
    void SetCurrentFloatingPositioner(FloatingPositioner *boundingBox);
    FloatingPositioner *GetCurrentFloatingPositioner() { return m_currentPositioner; }
    const FloatingPositioner *GetCurrentFloatingPositioner() const { return m_currentPositioner; }
    ///@}

    /**
     * Look for the FloatingPositioner corresponding to the current one but for another object.
     * Return NULL if no current positioner or nothing found.
     */
    ///@{
    FloatingPositioner *GetCorrespFloatingPositioner(const FloatingObject *object);
    const FloatingPositioner *GetCorrespFloatingPositioner(const FloatingObject *object) const;
    ///@}

    /**
     * @name Get and set the drawing group for linking floating element horizontally.
     * When setting it with an object, the corresponding id is returned (found or created)
     */
    ///@{
    int GetDrawingGrpId() const { return m_drawingGrpId; }
    void SetDrawingGrpId(int drawingGrpId) { m_drawingGrpId = drawingGrpId; }
    int SetDrawingGrpObject(void *drawingGrpObject);
    ///@}

    /**
     * @name Get and set maximum drawing yRel that is persistent for the floating object across all its floating
     * positioners, which allows for persistent vertical positioning for some elements
     */
    ///@{
    void ResetMaxDrawingYRel() { m_maxDrawingYRel = VRV_UNSET; }
    void SetMaxDrawingYRel(int maxDrawingYRel, data_STAFFREL place);
    int GetMaxDrawingYRel() const { return m_maxDrawingYRel; };
    ///@}

    /**
     * @name Get and set the drawing object IDs
     */
    ///@{
    void ResetDrawingObjectIDs();
    ///@}

    /**
     * Check whether current object represents initial element or extender lines
     */
    virtual bool IsExtenderElement() const { return false; }

    /**
     * Check whether the current object must be positioned closer to the staff than the other
     */
    virtual bool IsCloserToStaffThan(const FloatingObject *, data_STAFFREL) const { return false; }

    /**
     * Determine the vertical content boundary.
     * For refined layout this can take the overlapping bbox into account.
     * Returns a pair consisting of the boundary (relative to the object position)
     * and a flag indicating whether refined layout was used.
     */
    virtual std::pair<int, bool> GetVerticalContentBoundaryRel(const Doc *doc, const FloatingPositioner *positioner,
        const BoundingBox *horizOverlappingBBox, bool contentTop) const;

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
    //
public:
    //
private:
    FloatingPositioner *m_currentPositioner;

    /* Drawing Id to group floating elements horizontally */
    int m_drawingGrpId;

    int m_maxDrawingYRel;

    //----------------//
    // Static members //
    //----------------//

    /**
     * A vector for storing object / ids mapping.
     * When a group is created based on an object address, it is stack on the vector.
     * The ids of the group is then the position in the vector + GRPS_BASE_ID.
     * Groups coded in MEI have negative ids (-@vgrp value)
     */
    static thread_local std::vector<void *> s_drawingObjectIds;
};

//----------------------------------------------------------------------------
// FloatingPositioner
//----------------------------------------------------------------------------

/**
 * This class represents a basic object in the layout domain
 */
class FloatingPositioner : public BoundingBox {
public:
    // constructors and destructors
    FloatingPositioner(FloatingObject *object, StaffAlignment *alignment, char spanningType);
    virtual ~FloatingPositioner() {}
    ClassId GetClassId() const override { return FLOATING_POSITIONER; }

    virtual void ResetPositioner();

    /**
     * @name Get the X and Y drawing position
     */
    ///@{
    int GetDrawingX() const override;
    int GetDrawingY() const override;
    ///@}

    void ResetCachedDrawingX() const override;
    void ResetCachedDrawingY() const override;

    /**
     * @name Setter and getters for the objectX and Y
     */
    ///@{
    void SetObjectXY(Object *objectX, Object *objectY);
    Object *GetObjectX() { return m_objectX; }
    const Object *GetObjectX() const { return m_objectX; }
    Object *GetObjectY() { return m_objectY; }
    const Object *GetObjectY() const { return m_objectY; }
    ///@}

    /**
     * Getter for the FloatingObject (asserted, cannot be NULL)
     */
    ///@{
    FloatingObject *GetObject() { return m_object; }
    const FloatingObject *GetObject() const { return m_object; }
    ///@}

    /**
     * Getter for the StaffAlignment (asserted, cannot be NULL)
     */
    ///@{
    StaffAlignment *GetAlignment() { return m_alignment; }
    const StaffAlignment *GetAlignment() const { return m_alignment; }
    ///@}

    /**
     * Getter for the spanning type
     */
    char GetSpanningType() const { return m_spanningType; }

    data_STAFFREL GetDrawingPlace() const { return m_place; }

    /**
     * @name Get and set the X and Y drawing relative position.
     * (X is currently used only for Arpeg)
     */
    ///@{
    int GetDrawingYRel() const { return m_drawingYRel; }
    virtual void SetDrawingYRel(int drawingYRel, bool force = false);
    int GetDrawingXRel() const { return m_drawingXRel; }
    virtual void SetDrawingXRel(int drawingXRel);
    ///@}

    /**
     * @name Get and set the drawing extender width.
     * Should be nonzero only if the extender line is not included in the bounding box.
     */
    ///@{
    int GetDrawingExtenderWidth() const { return m_drawingExtenderWidth; }
    void SetDrawingExtenderWidth(int extenderWidth) { m_drawingExtenderWidth = extenderWidth; }
    ///@}

    /**
     * Check for horizontal overlap with special consideration for extender lines
     */
    bool HasHorizontalOverlapWith(const BoundingBox *bbox, int unit) const;

    /**
     * Return the horizontal margin for overlap with another element
     * This can be negative, if elements are allowed to slightly overlap
     */
    int GetAdmissibleHorizOverlapMargin(const BoundingBox *bbox, int unit) const;

    /**
     * Update the Y drawing relative position based on collision detection with the overlapping bounding box
     */
    void CalcDrawingYRel(const Doc *doc, const StaffAlignment *staffAlignment, const BoundingBox *horizOverlappingBBox);

    /**
     * Align extender elements across systems
     */
    void AdjustExtenders();

    /**
     * Calculate the vertical space below the element and above the bounding box
     */
    int GetSpaceBelow(
        const Doc *doc, const StaffAlignment *staffAlignment, const BoundingBox *horizOverlappingBBox) const;

    /**
     * Determine the vertical content boundary.
     * For refined layout this can take the overlapping bbox into account.
     */
    ///@{
    std::pair<int, bool> GetVerticalContentBoundaryRel(
        const Doc *doc, const BoundingBox *horizOverlappingBBox, bool contentTop) const;
    int GetVerticalContentBoundary(const Doc *doc, const BoundingBox *horizOverlappingBBox, bool contentTop) const;
    ///@}

    /**
     * Version of Boundary::VerticalContentOverlap which takes refined boundaries into account
     */
    bool HasVerticalContentOverlap(const Doc *doc, const BoundingBox *horizOverlappingBBox, int margin) const;

private:
    Object *m_objectX;
    Object *m_objectY;

protected:
    /**
     * The X drawing relative position of the object.
     * Arpeg are positionned according to the top note through the FloatingPositioner
     * (See View::DrawArpeg that sets the FloatingPositioner)
     */
    int m_drawingXRel;
    /**
     * The Y drawing relative position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingYRel;

    /**
     * The horizontal width of the extender line whenever it is not included in the bounding box.
     */
    int m_drawingExtenderWidth;

    /**
     * A pointer to the FloatingObject it represents.
     */
    FloatingObject *m_object;

    /**
     * A pointer to the StaffAlignment that owns it.
     */
    StaffAlignment *m_alignment;

    data_STAFFREL m_place;

public:
    /**
     * The spanning type of the positioner for spanning control elements
     */
    char m_spanningType;
};

//----------------------------------------------------------------------------
// FloatingCurvePositioner
//----------------------------------------------------------------------------

/**
 * This class represents a basic object for a curve (slur, tie) in the layout domain
 */
class FloatingCurvePositioner : public FloatingPositioner {
public:
    // constructors and destructors
    FloatingCurvePositioner(FloatingObject *object, StaffAlignment *alignment, char spanningType);
    virtual ~FloatingCurvePositioner();
    ClassId GetClassId() const override { return FLOATING_CURVE_POSITIONER; }

    void ResetPositioner() override;

    /**
     * Reset the curve parameters in FloatingCurvePositioner::FloatingCurvePositioner and in
     * FloatingCurvePositioner::ResetPositioner
     */
    void ResetCurveParams();

    /**
     * Update the curve parameters.
     * Stored points are made relative to the curve drawingY.
     */
    ///@{
    void UpdateCurveParams(const Point points[4], int thickness, curvature_CURVEDIR curveDir);
    void UpdatePoints(const BezierCurve &bezier);
    ///@}

    /**
     * Moves bounding points horizontally or vertically by a specified distance
     */
    ///@{
    void MoveFrontHorizontal(int distance);
    void MoveBackHorizontal(int distance);
    void MoveFrontVertical(int distance);
    void MoveBackVertical(int distance);
    ///@}

    /**
     * Calculate the min or max Y for a set of points
     */
    int CalcMinMaxY(const Point points[4]) const;

    /**
     * Calculate the adjustment needed for an element for the curve not to overlap with it.
     * Discard will be true if the element already fits.
     */
    ///@{
    int CalcAdjustment(
        const BoundingBox *boundingBox, bool &discard, int margin = 0, bool horizontalOverlap = true) const;
    int CalcDirectionalAdjustment(const BoundingBox *boundingBox, bool isCurveAbove, bool &discard, int margin = 0,
        bool horizontalOverlap = true) const;
    // Refined version that returns the adjustments on the left and right hand side of the bounding box
    std::pair<int, int> CalcLeftRightAdjustment(
        const BoundingBox *boundingBox, bool &discard, int margin = 0, bool horizontalOverlap = true) const;
    std::pair<int, int> CalcDirectionalLeftRightAdjustment(const BoundingBox *boundingBox, bool isCurveAbove,
        bool &discard, int margin = 0, bool horizontalOverlap = true) const;
    ///@}

    /**
     * @name Getters for the current parameters
     */
    ///@{
    void GetPoints(Point points[4]) const;
    int GetThickness() const { return m_thickness; }
    curvature_CURVEDIR GetDir() const { return m_dir; }
    ///@}

    /**
     * @name Getter and setter for cached x1 and x2
     */
    ///@{
    bool HasCachedX12() const;
    std::pair<int, int> GetCachedX12() const { return m_cachedX12; }
    void SetCachedX12(const std::pair<int, int> &cachedX12) { m_cachedX12 = cachedX12; }
    ///@}

    /**
     * Deletes all the CurveSpannedElement objects.
     */
    void ClearSpannedElements();

    /**
     * Add a CurveSpannedElement to the FloatingCurvePositioner.
     * The element is owned by the positioner.
     */
    void AddSpannedElement(CurveSpannedElement *spannedElement) { m_spannedElements.push_back(spannedElement); }

    /**
     * Return a const pointer to the spanned elements
     */
    const ArrayOfCurveSpannedElements *GetSpannedElements() { return &m_spannedElements; }

    /**
     * @name Getter, setter and checker for the crossStaff
     */
    ///@{
    void SetCrossStaff(Staff *crossStaff) { m_crossStaff = crossStaff; }
    Staff *GetCrossStaff() { return m_crossStaff; }
    const Staff *GetCrossStaff() const { return m_crossStaff; }
    bool IsCrossStaff() const { return m_crossStaff != NULL; }
    ///@}

    /**
     * @name Getter, setter for the requested staff space
     */
    ///@{
    void SetRequestedStaffSpace(int space) { m_requestedStaffSpace = space; }
    int GetRequestedStaffSpace() const { return m_requestedStaffSpace; }
    ///@}

    /**
     * Calculate the requested staff space above and below
     */
    std::pair<int, int> CalcRequestedStaffSpace(const StaffAlignment *alignment) const;

private:
    //
public:
    //
private:
    /**
     * @name Current parameters.
     * Points are relative to the curve current drawingY.
     */
    ///@{
    Point m_points[4];
    int m_thickness;
    curvature_CURVEDIR m_dir;
    Staff *m_crossStaff;
    ///@}

    ArrayOfCurveSpannedElements m_spannedElements;

    /** The cached min or max value (depending on the curvature) */
    mutable int m_cachedMinMaxY;

    /** The cached values for x1 and x2 */
    std::pair<int, int> m_cachedX12;

    /**
     * Some curves (S-shaped slurs) can request staff space to prevent collisions from two sides
     */
    int m_requestedStaffSpace;
};

//----------------------------------------------------------------------------
// CurveSpannedElement
//----------------------------------------------------------------------------

class CurveSpannedElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     */
    ///@{
    CurveSpannedElement()
    {
        m_boundingBox = NULL;
        m_discarded = false;
        m_isBelow = true;
    }
    virtual ~CurveSpannedElement() {}

    Point m_rotatedPoints[4];
    const BoundingBox *m_boundingBox;
    bool m_discarded;
    bool m_isBelow;
};

} // namespace vrv

#endif
