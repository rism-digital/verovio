/////////////////////////////////////////////////////////////////////////////
// Name:        floatingelement.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_FLOATING_ELEMENT_H__
#define __VRV_FLOATING_ELEMENT_H__

#include "object.h"

namespace vrv {

class FloatingBoundingBox;

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
    virtual ClassId Is() { return FLOATING_ELEMENT; };
    ///@}

    virtual void UpdateContentBBoxX(int x1, int x2){};
    virtual void UpdateContentBBoxY(int y1, int y2){};
    virtual void UpdateSelfBBoxX(int x1, int x2);
    virtual void UpdateSelfBBoxY(int y1, int y2);

    void SetCurrentBoundingBox(FloatingBoundingBox *boundingBox);
    FloatingBoundingBox *GetCurrentBoundingBox() { return m_currentBoundingBox; };

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::FillStaffCurrentTimeSpanning
     */
    virtual int FillStaffCurrentTimeSpanning(ArrayPtrVoid *params);

    /**
     * See Object::PrepareTimeSpanning
     */
    virtual int PrepareTimeSpanning(ArrayPtrVoid *params);

    /**
     * See Object::PrepareTimestamps
     */
    virtual int PrepareTimestamps(ArrayPtrVoid *params);

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(ArrayPtrVoid *params);
    ///@}

private:
    //
public:
    //
private:
    FloatingBoundingBox *m_currentBoundingBox;
};

//----------------------------------------------------------------------------
// FloatingBoundingBox
//----------------------------------------------------------------------------

/**
 * This class represents a basic object in the layout domain
 */
class FloatingBoundingBox : public BoundingBox {
public:
    // constructors and destructors
    FloatingBoundingBox();
    virtual ~FloatingBoundingBox(){};

    virtual void ResetBB();

    /**
     * @name Get and set the Y drawing relative position
     */
    ///@{
    int GetDrawingYRel() { return m_drawingYRel; };
    void SetDrawingYRel(int drawingYRel) { m_drawingYRel = drawingYRel; };
    ///@}

protected:
    /**
     * The Y drawing relative position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingYRel;
};

} // namespace vrv

#endif
