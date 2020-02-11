/////////////////////////////////////////////////////////////////////////////
// Name:        verticalaligner.h
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_VERTICAL_ALIGNER_H__
#define __VRV_VERTICAL_ALIGNER_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

class AdjustFloatingPositionerGrpsParams;
class FloatingObject;
class StaffAlignment;
class SystemAligner;
class TimestampAttr;

//----------------------------------------------------------------------------
// SystemAligner
//----------------------------------------------------------------------------

/**
 * This class aligns the content of a system
 * It contains a vector of StaffAlignment
 */
class SystemAligner : public Object {
public:
    // constructors and destructors
    SystemAligner();
    virtual ~SystemAligner();
    virtual ClassId GetClassId() const { return SYSTEM_ALIGNER; }

    /**
     * Do not copy children for HorizontalAligner
     */
    virtual bool CopyChildren() const { return false; }

    /**
     * Reset the aligner (clear the content) and creates the end (bottom) alignement
     */
    virtual void Reset();

    /**
     * Get bottom StaffAlignment for the system.
     * For each SystemAligner, we keep a StaffAlignment for the bottom position.
     */
    StaffAlignment *GetBottomAlignment() const { return m_bottomAlignment; }

    /**
     * Get the StaffAlignment at index idx.
     * Creates the StaffAlignment if not there yet.
     * Checks the they are created incrementally (without gap).
     * If a staff is passed, it will be used for initializing m_staffN and m_staffSize of the aligner.
     * (no const since the bottom alignment is temporarily removed)
     */
    StaffAlignment *GetStaffAlignment(int idx, Staff *staff, Doc *doc);

    /**
     * Get the StaffAlignment for the staffN.
     * Return NULL if not found.
     */
    StaffAlignment *GetStaffAlignmentForStaffN(int staffN) const;

    /**
     * Find all the positioners pointing to an object;
     */
    void FindAllPositionerPointingTo(ArrayOfFloatingPositioners *positioners, FloatingObject *object);

    /**
     * Find all the intersection points with a vertical line (top to bottom)
     */
    void FindAllIntersectionPoints(
        SegmentedLine &line, BoundingBox &boundingBox, const std::vector<ClassId> &classIds, int margin);

private:
    //
public:
    //
private:
    /**
     * A pointer to the left StaffAlignment object kept for the system bottom position
     */
    StaffAlignment *m_bottomAlignment;
};

//----------------------------------------------------------------------------
// StaffAlignment
//----------------------------------------------------------------------------

/**
 * This class stores an alignement position staves will point to
 */
class StaffAlignment : public Object {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    StaffAlignment();
    virtual ~StaffAlignment();
    virtual ClassId GetClassId() const { return STAFF_ALIGNMENT; }
    ///@}

    /**
     * @name Setter and getter for y
     */
    void SetYRel(int yRel);
    int GetYRel() const { return m_yRel; }

    /**
     * @name Set and get verse count.
     * When setting a value of 0, then 1 is assumed. This occurs
     * Typically with one single verse and no @n in <verse>
     */
    ///@{
    void SetVerseCount(int verse_count);
    int GetVerseCount() const { return m_verseCount; }

    /**
     * Retrieves or creates the FloatingPositioner for the FloatingObject on this staff.
     */
    void SetCurrentFloatingPositioner(FloatingObject *object, Object *objectX, Object *objectY, char spanningType);

    /**
     * Look for the first FloatingPositioner corresponding to the FloatingObject of the ClassId.
     * Return NULL if not found and does not create anything.
     */
    FloatingPositioner *FindFirstFloatingPositioner(ClassId classId);

    /**
     * Look for the FloatingPositioner corresponding to the FloatingObject.
     * Return NULL if not found and does not create anything.
     */
    FloatingPositioner *GetCorrespFloatingPositioner(FloatingObject *object);

    /**
     * @name Setter and getter of the staff from which the alignment is created alignment.
     * Used for accessing the staff @n, the size, etc.
     */
    ///@{
    Staff *GetStaff() const { return m_staff; }
    void SetStaff(Staff *staff, Doc *doc);
    ///@}

    /**
     * Returns the staff size (100 if no staff object is refered to)
     */
    int GetStaffSize() const;

    /**
     * @name Calculates the overlow (above or below for the bounding box.
     * Looks if the bounding box is a FloatingPositioner or not, in which case it we take into account its m_drawingYRel
     * value.
     */
    ///@{
    int CalcOverflowAbove(BoundingBox *box);
    int CalcOverflowBelow(BoundingBox *box);
    ///@}

    /**
     * @name Setter and getter for overflow and overlap values
     */
    ///@{
    void SetOverflowAbove(int overflowAbove);
    int GetOverflowAbove() const { return m_overflowAbove; }
    void SetOverflowBelow(int overflowBottom);
    int GetOverflowBelow() const { return m_overflowBelow; }
    void SetOverlap(int overlap);
    int GetOverlap() const { return m_overlap; }
    int GetStaffHeight() const { return m_staffHeight; }
    ///@}

    /**
     * @name Adds a bounding box to the array of overflowing objects above or below
     */
    ///@{
    void AddBBoxAbove(BoundingBox *box) { m_overflowAboveBBoxes.push_back(box); }
    void AddBBoxBelow(BoundingBox *box) { m_overflowBelowBBoxes.push_back(box); }
    ///@}

    /**
     * Deletes all the FloatingPositioner objects.
     */
    void ClearPositioners();

    /**
     * Find all the intersection points with a vertical line (top to bottom)
     */
    void FindAllIntersectionPoints(
        SegmentedLine &line, BoundingBox &boundingBox, const std::vector<ClassId> &classIds, int margin);

    void ReAdjustFloatingPositionersGrps(AdjustFloatingPositionerGrpsParams *params,
        const ArrayOfFloatingPositioners &positioners, ArrayOfIntPairs &grpIdYRel);

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AlignVertically
     */
    virtual int AlignVerticallyEnd(FunctorParams *functorParams);

    /**
     * See Object::AdjustYPos
     */
    virtual int AdjustYPos(FunctorParams *functorParams);

    /**
     * See Object::AdjustStaffOverlap
     */
    virtual int AdjustStaffOverlap(FunctorParams *functorParams);

    /**
     * See Object::AdjustFloatingPositioners
     */
    virtual int AdjustFloatingPositioners(FunctorParams *functorParams);

    /**
     * See Object::AdjustFloatingPositionerGrps
     */
    virtual int AdjustFloatingPositionerGrps(FunctorParams *functorParams);

    /**
     * See Object::AdjustSlurs
     */
    virtual int AdjustSlurs(FunctorParams *functorParams);

    /**
     * See Object::JustifyY
     */
    virtual int JustifyY(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    /**
     * The list of FloatingPositioner for the staff.
     */
    ArrayOfFloatingPositioners m_floatingPositioners;
    /**
     * Stores a pointer to the staff from which the aligner was created.
     * This is necessary since we don't always have all the staves.
     */
    Staff *m_staff;
    /**
     * Stores the position relative to the system.
     */
    int m_yRel;
    // int m_yShift;
    /**
     * Stores the number of verse of the staves attached to the aligner
     */
    int m_verseCount;

    /**
     * @name values for storing the overlow and overlap
     */
    ///@{
    int m_overflowAbove;
    int m_overflowBelow;
    int m_overlap;
    int m_staffHeight;
    ///@}

    /**
     * The list of overflowing bounding boxes (e.g, LayerElement or FloatingPositioner)
     */
    std::vector<BoundingBox *> m_overflowAboveBBoxes;
    std::vector<BoundingBox *> m_overflowBelowBBoxes;
};

} // namespace vrv

#endif
