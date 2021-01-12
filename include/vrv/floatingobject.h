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
    FloatingObject(const std::string &classid);
    virtual ~FloatingObject();
    virtual void Reset();
    virtual ClassId GetClassId() const { return FLOATING_OBJECT; }
    ///@}

    virtual void UpdateContentBBoxX(int x1, int x2);
    virtual void UpdateContentBBoxY(int y1, int y2);
    virtual void UpdateSelfBBoxX(int x1, int x2);
    virtual void UpdateSelfBBoxY(int y1, int y2);

    /**
     * @name Get and set the X and Y drawing position
     */
    ///@{
    virtual int GetDrawingX() const;
    virtual int GetDrawingY() const;
    ///@}

    void SetCurrentFloatingPositioner(FloatingPositioner *boundingBox);
    FloatingPositioner *GetCurrentFloatingPositioner() const { return m_currentPositioner; }

    /**
     * Look for the FloatingPositioner corresponding to the current one but for another object.
     * Return NULL if no current positioner or nothing found.
     */
    FloatingPositioner *GetCorrespFloatingPositioner(FloatingObject *object);

    /**
     * @name Get and set the drawing group for linking floating element horizontally.
     * When setting it with an object, the corresponding id is returned (found or created)
     */
    ///@{
    int GetDrawingGrpId() const { return m_drawingGrpId; }
    void SetDrawingGrpId(int drawingGrpId) { m_drawingGrpId = drawingGrpId; }
    int SetDrawingGrpObject(void *drawingGrpObject);
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    /**
     * See Object::ResetVerticalAlignment
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);

    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning(FunctorParams *functorParams);

    /**
     * See Object::PrepareTimePointing
     */
    virtual int PrepareTimePointing(FunctorParams *functorParams);

    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int PrepareTimeSpanning(FunctorParams *functorParams);

    /**
     * See Object::PrepareTimestamps
     */
    virtual int PrepareTimestamps(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::UnCastOff
     */
    virtual int UnCastOff(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    FloatingPositioner *m_currentPositioner;

    /* Drawing Id to group floating elements horizontally */
    int m_drawingGrpId;

    /**
     * A vector for storing object / ids mapping.
     * When a group is created based on an object address, it is stack on the vector.
     * The ids of the group is then the position in the vector + GRPS_BASE_ID.
     * Groups coded in MEI have negative ids (-@vgrp value)
     */
    static std::vector<void *> s_drawingObjectIds;
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
    virtual ~FloatingPositioner(){};
    virtual ClassId GetClassId() const { return FLOATING_POSITIONER; }

    virtual void ResetPositioner();

    /**
     * @name Get the X and Y drawing position
     */
    ///@{
    virtual int GetDrawingX() const;
    virtual int GetDrawingY() const;
    ///@}

    virtual void ResetCachedDrawingX() const;
    virtual void ResetCachedDrawingY() const;

    /**
     * @name Setter and getters for the objectX and Y
     */
    ///@{
    void SetObjectXY(Object *objectX, Object *objectY);
    Object *GetObjectX() const { return m_objectX; }
    Object *GetObjectY() const { return m_objectY; }
    ///@}

    /**
     * Getter for the FloatingObject (asserted, cannot be NULL)
     */
    FloatingObject *GetObject() const { return m_object; }

    /**
     * Getter for the StaffAlignment (asserted, cannot be NULL)
     */
    StaffAlignment *GetAlignment() const { return m_alignment; }

    /**
     * Getter for the spanning type
     */
    char GetSpanningType() { return m_spanningType; }

    bool CalcDrawingYRel(Doc *doc, StaffAlignment *staffAlignment, BoundingBox *horizOverlapingBBox);

    int GetSpaceBelow(Doc *doc, StaffAlignment *staffAlignment, BoundingBox *horizOverlapingBBox);

    data_STAFFREL GetDrawingPlace() const { return m_place; }

    /**
     * @name Get and set the X and Y drawing relative position.
     * (X is currently used only for Arpeg)
     */
    ///@{
    int GetDrawingYRel() const { return m_drawingYRel; }
    virtual void SetDrawingYRel(int drawingYRel);
    int GetDrawingXRel() const { return m_drawingXRel; }
    virtual void SetDrawingXRel(int drawingXRel);
    ///@}

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
     * The spanning type of the positionner for spanning control elements
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
    virtual ClassId GetClassId() const { return FLOATING_CURVE_POSITIONER; }

    virtual void ResetPositioner();

    /**
     * Reset the curve parameters in FloatingCurvePositioner::FloatingCurvePositioner and in
     * FloatingCurvePositioner::ResetPositioner
     */
    void ResetCurveParams();

    /**
     * Update the curve parameters.
     * Stored points are made relative to the curve darwingY.
     */
    void UpdateCurveParams(const Point points[4], float angle, int thickness, curvature_CURVEDIR curveDir);

    /**
     * Moves bounding points vertically by a specified distance downward
     */
    void MoveFrontVertical(int distance);
    void MoveBackVertical(int distance);

    /**
     * Calculate the min or max Y for a set of points
     */
    int CalcMinMaxY(const Point points[4]);

    /**
     * Calculate the adjustment needed for an element for the curve not to overlap with it.
     * Discard will be true if the element already fits.
     */
    int CalcAdjustment(BoundingBox *boundingBox, bool &discard, int margin = 0);

    /**
     * @name Getters for the current parameters
     */
    ///@{
    void GetPoints(Point points[4]) const;
    float GetAngle() { return m_angle; }
    int GetThickness() { return m_thickness; }
    curvature_CURVEDIR GetDir() { return m_dir; }
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
     * Return a cont pointer to the spanned elements
     */
    const ArrayOfCurveSpannedElements *GetSpannedElements() { return &m_spannedElements; }

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
    float m_angle;
    int m_thickness;
    curvature_CURVEDIR m_dir;
    ///@}

    ArrayOfCurveSpannedElements m_spannedElements;

    /** The cached min or max value (depending on the curvature) */
    int m_cachedMinMaxY;
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
    }
    virtual ~CurveSpannedElement(){};

    Point m_rotatedPoints[4];
    BoundingBox *m_boundingBox;
    bool m_discarded;
};

} // namespace vrv

#endif
