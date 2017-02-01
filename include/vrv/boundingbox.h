/////////////////////////////////////////////////////////////////////////////
// Name:        boundingbox.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BOUNDING_BOX_H__
#define __VRV_BOUNDING_BOX_H__

//----------------------------------------------------------------------------

#include "vrvdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// BoundingBox
//----------------------------------------------------------------------------

/**
 * This class represents a basic object in the layout domain
 */
class BoundingBox {
public:
    // constructors and destructors
    BoundingBox();
    virtual ~BoundingBox(){};
    virtual ClassId GetClassId() const;
    bool Is(ClassId classId) const { return (this->GetClassId() == classId); }

    virtual void UpdateContentBBoxX(int x1, int x2);
    virtual void UpdateContentBBoxY(int y1, int y2);
    virtual void UpdateSelfBBoxX(int x1, int x2);
    virtual void UpdateSelfBBoxY(int y1, int y2);
    bool HasContentBB();
    bool HasSelfBB();
    void SetEmptyBB();
    bool HasEmptyBB();

    /**
     * Reset the bounding box values
     */
    virtual void ResetBoundingBox();

    /**
     * @name Get and set the X and Y drawing position
     */
    ///@{
    virtual int GetDrawingX() const { return m_drawingX; }
    virtual int GetDrawingY() const { return m_drawingY; }
    void SetDrawingX(int drawingX) { m_drawingX = drawingX; }
    void SetDrawingY(int drawingY) { m_drawingY = drawingY; }
    ///@}

    /**
     * @name Get positions for self and content
     */
    ///@{
    int GetSelfBottom() const { return (this->GetDrawingY() + m_selfBB_y1); }
    int GetSelfTop() const { return (this->GetDrawingY() + m_selfBB_y2); }
    int GetSelfLeft() const { return (this->GetDrawingX() + m_selfBB_x1); }
    int GetSelfRight() const { return (this->GetDrawingX() + m_selfBB_x2); }
    int GetContentBottom() const { return (this->GetDrawingY() + m_contentBB_y1); }
    int GetContentTop() const { return (this->GetDrawingY() + m_contentBB_y2); }
    int GetContentLeft() const { return (this->GetDrawingX() + m_contentBB_x1); }
    int GetContentRight() const { return (this->GetDrawingX() + m_contentBB_x2); }
    
    int GetSelfX1() const { return m_selfBB_x1; }
    int GetSelfX2() const { return m_selfBB_x2; }
    int GetSelfY1() const { return m_selfBB_y1; }
    int GetSelfY2() const { return m_selfBB_y2; }
    int GetContentX1() const { return m_contentBB_x1; }
    int GetContentX2() const { return m_contentBB_x2; }
    int GetContentY1() const { return m_contentBB_y1; }
    int GetContentY2() const { return m_contentBB_y2; }
    ///@}

    /**
     * Is true if the bounding box (self or content) has been updated at least once.
     * We need this to avoid not updating bounding boxes to screw up the layout with their initial values.
     */
    bool HasUpdatedBB() const { return (m_updatedBBoxX && m_updatedBBoxY); }

    /**
     * Returns true if the bounding box has a horizontal overlap with the other one.
     */
    bool HorizontalOverlap(const BoundingBox *other) const;

    bool Encloses(const Point point) const;

    int Intersects(FloatingPositioner *curve, int margin = 0) const;

    /**
     * Swap the points passed as reference.
     * This is useful for example when calculating bezier positions.
     */
    static void SwapPoints(Point *x1, Point *x2);

    /**
     * Calculate the position of a point after a rotation of rot_alpha around the center
     */
    static Point CalcPositionAfterRotation(Point point, float rot_alpha, Point center);

    /**
     * Calculate the position of a point after a rotation of rot_alpha around the center
     */
    static int CalcBezierAtPosition(const Point bezier[4], int x);

    /**
     * Calculate the position of the bezier above and below for a thick bezier
     */
    static void CalcThickBezier(
        const Point bezier[4], int thickness, float angle, Point *topBezier, Point *bottomBezier);

    /**
     * Swap values passed as reference.
     * This is useful for example when switching to the device context world.
     */
    static void SwapY(int *y1, int *y2)
    {
        int tmp = *y1;
        *y1 = *y2;
        *y2 = tmp;
    }

    static void ApproximateBezierBoundingBox(
        const Point bezier[4], Point &pos, int &width, int &height, int &minYPos, int &maxYPos);

private:
    bool m_updatedBBoxX;
    bool m_updatedBBoxY;
    /** buffer for De-Casteljau algorithm */
    static int s_deCasteljau[4][4];
    
    int m_contentBB_x1, m_contentBB_y1, m_contentBB_x2, m_contentBB_y2;
    int m_selfBB_x1, m_selfBB_y1, m_selfBB_x2, m_selfBB_y2;

protected:
    /**
     * The Y drawing position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingY;
    /**
     * The X drawing position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingX;
};

} // namespace vrv

#endif
