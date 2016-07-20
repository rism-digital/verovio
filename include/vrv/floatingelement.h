/////////////////////////////////////////////////////////////////////////////
// Name:        floatingelement.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FLOATING_ELEMENT_H__
#define __VRV_FLOATING_ELEMENT_H__

#include "devicecontextbase.h"
#include "object.h"

namespace vrv {

class FloatingPositioner;
class StaffAlignment;

//----------------------------------------------------------------------------
// FloatingElement
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements appearing within measure.
 * It is not an abstract class but should not be instanciated directly.
 */
class FloatingElement : public Object {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    FloatingElement();
    FloatingElement(std::string classid);
    virtual ~FloatingElement();
    virtual void Reset();
    virtual ClassId Is() const { return FLOATING_ELEMENT; };
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
    FloatingPositioner *GetCurrentFloatingPositioner() { return m_currentPositioner; };

    //----------//
    // Functors //
    //----------//

    /**
     * @name Reset the horizontal alignment
     */
    ///@{
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);
    ///@}

    /**
     * @name Reset the vertical alignment
     */
    ///@{
    virtual int ResetVerticalAlignment(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning(FunctorParams *functorParams);

    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int PrepareTimeSpanning(FunctorParams *functorParams);

    /**
     * See Object::PrepareTimestamps
     */
    virtual int PrepareTimestamps(FunctorParams *functorParams);

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * Undo the cast of the system.
     * This is used by Doc::ContinuousLayout
     */
    virtual int UnCastOff(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    FloatingPositioner *m_currentPositioner;
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
    FloatingPositioner(FloatingElement *element);
    virtual ~FloatingPositioner(){};
    virtual ClassId Is() const { return FLOATING_POSITIONER; };

    virtual void ResetPositioner();

    FloatingElement *GetElement() const { return m_element; };

    bool CalcDrawingYRel(Doc *doc, StaffAlignment *staffAlignment, BoundingBox *horizOverlapingBBox);

    data_STAFFREL GetDrawingPlace() const { return m_place; };

    void UpdateSlurPosition(const Point points[4], float angle, int thickness, curvature_CURVEDIR curveDir);

    /**
     * @name Get and set the Y drawing relative position
     */
    ///@{
    int GetDrawingYRel() const { return m_drawingYRel; };
    void SetDrawingYRel(int drawingYRel);
    ///@}

protected:
    /**
     * The Y drawing relative position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingYRel;

    FloatingElement *m_element;

    data_STAFFREL m_place;

    Point m_slurPoints[4];
    float m_slurAngle;
    int m_slurThickness;
    curvature_CURVEDIR m_slurDir;
};

} // namespace vrv

#endif
