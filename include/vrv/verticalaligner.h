/////////////////////////////////////////////////////////////////////////////
// Name:        verticalaligner.h
// Author:      Laurent Pugin
// Created:     2013
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_VERTICAL_ALIGNER_H__
#define __VRV_VERTICAL_ALIGNER_H__

#include "object.h"

namespace vrv {

class AttSpacing;
class FloatingObject;
class ScoreDef;
class StaffAlignment;
class StaffDef;
class System;
class SystemAligner;

//----------------------------------------------------------------------------
// SystemAligner
//----------------------------------------------------------------------------

/**
 * This class aligns the content of a system
 * It contains a vector of StaffAlignment
 */
class SystemAligner : public Object {
public:
    /**
     * Declares different spacing types between staves
     */
    enum class SpacingType { System, Staff, Brace, Bracket, None };

    // constructors and destructors
    SystemAligner();
    virtual ~SystemAligner();

    /**
     * Override the method of adding AlignmentReference children
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Do not copy children for HorizontalAligner
     */
    bool CopyChildren() const override { return false; }

    /**
     * Reset the aligner (clear the content) and creates the end (bottom) alignement
     */
    void Reset() override;

    /**
     * Get bottom StaffAlignment for the system.
     * For each SystemAligner, we keep a StaffAlignment for the bottom position.
     */
    ///@{
    StaffAlignment *GetBottomAlignment() { return m_bottomAlignment; }
    const StaffAlignment *GetBottomAlignment() const { return m_bottomAlignment; }
    ///@}

    /**
     * Get the StaffAlignment at index idx.
     * Creates the StaffAlignment if not there yet.
     * Checks the they are created incrementally (without gap).
     * If a staff is passed, it will be used for initializing m_staffN and m_staffSize of the aligner.
     * (no const since the bottom alignment is temporarily removed)
     */
    StaffAlignment *GetStaffAlignment(int idx, Staff *staff, const Doc *doc);

    /**
     * Reorder the staff alignment as given in the staffNs.
     * Reordering will fail if the number of staffAlignment does not correspond, or
     * if some staffAlignment pointers are missing for a corresponding staffN
     */
    void ReorderBy(const std::vector<int> &staffNs);

    /**
     * Get the StaffAlignment for the staffN.
     * Return NULL if not found.
     */
    ///@{
    StaffAlignment *GetStaffAlignmentForStaffN(int staffN);
    const StaffAlignment *GetStaffAlignmentForStaffN(int staffN) const;
    ///@}

    /**
     * Get pointer to the parent system.
     * Return NULL if parent is not set.
     */
    System *GetSystem();

    /**
     * Find all the positioners pointing to an object;
     */
    void FindAllPositionerPointingTo(ArrayOfFloatingPositioners *positioners, const FloatingObject *object);

    /**
     * Find all the intersection points with a vertical line (top to bottom)
     */
    void FindAllIntersectionPoints(
        SegmentedLine &line, const BoundingBox &boundingBox, const std::vector<ClassId> &classIds, int margin) const;
    /**
     * Get System Overflows
     */
    int GetOverflowAbove(const Doc *doc, bool scoreDefClef = false) const;
    int GetOverflowBelow(const Doc *doc, bool scoreDefClef = false) const;

    /**
     * Get justification sum
     */
    double GetJustificationSum(const Doc *doc) const;

    /**
     * Calculates and sets spacing for specified ScoreDef
     */
    void SetSpacing(const ScoreDef *scoreDef);

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
    /**
     * Return above spacing type for passed staff.
     * Calculates spacings if required.
     */
    SpacingType GetAboveSpacingType(const Staff *staff);
    /**
     * Calculates above spacing type for staffDef
     */
    SpacingType CalculateSpacingAbove(const StaffDef *staffDef) const;

public:
    //
private:
    /**
     * A pointer to the left StaffAlignment object kept for the system bottom position
     */
    StaffAlignment *m_bottomAlignment;
    /**
     * Stores the above spacing type of staves (based on visibility)
     */
    std::map<int, SpacingType> m_spacingTypes;
    /**
     * A pointer to the parent system
     */
    System *m_system;
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
    ///@}

    /**
     * @name Setter and getter for y
     */
    void SetYRel(int yRel);
    int GetYRel() const { return m_yRel; }

    /**
     * @name Methods for managing verse count with / without the collapse option
     * When setting a value of 0, then 1 is assumed. This occurs
     * Typically with one single verse and no \@n in <verse>
     * Without the collapse option, the count is the greatest @n
     * With the collapse option, the count is number of verses.
     * The position is calculated from the bottom.
     */
    ///@{
    void AddVerseN(int verseN, data_STAFFREL place);
    int GetVerseCount(bool collapse) const;
    int GetVerseCountAbove(bool collapse) const;
    int GetVerseCountBelow(bool collapse) const;
    int GetVersePositionAbove(int verseN, bool collapse) const;
    int GetVersePositionBelow(int verseN, bool collapse) const;
    ///@}

    /**
     * Retrieves or creates the FloatingPositioner for the FloatingObject on this staff.
     */
    void SetCurrentFloatingPositioner(FloatingObject *object, Object *objectX, Object *objectY, char spanningType);

    /**
     * Retrieve all FloatingPositioner.
     */
    const ArrayOfFloatingPositioners &GetFloatingPositioners() { return m_floatingPositioners; }

    /**
     * Look for the first FloatingPositioner corresponding to the FloatingObject of the ClassId.
     * Return NULL if not found and does not create anything.
     */
    ///@{
    FloatingPositioner *FindFirstFloatingPositioner(ClassId classId);
    const FloatingPositioner *FindFirstFloatingPositioner(ClassId classId) const;
    ///@}

    /**
     * Find all FloatingPositioner corresponding to a FloatingObject with given ClassId.
     */
    ArrayOfFloatingPositioners FindAllFloatingPositioners(ClassId classId);

    /**
     * Look for the FloatingPositioner corresponding to the FloatingObject.
     * Return NULL if not found and does not create anything.
     */
    ///@{
    FloatingPositioner *GetCorrespFloatingPositioner(const FloatingObject *object);
    const FloatingPositioner *GetCorrespFloatingPositioner(const FloatingObject *object) const;
    ///@}

    /**
     * @name Setter and getter of the staff from which the alignment is created alignment.
     * Used for accessing the staff \@n, the size, etc.
     */
    ///@{
    Staff *GetStaff() { return m_staff; }
    const Staff *GetStaff() const { return m_staff; }
    void SetStaff(Staff *staff, const Doc *doc, SystemAligner::SpacingType spacingType);
    ///@}

    /**
     * @name Setter and getter of the system pointer to which the Alignment is belong to.
     */
    ///@{
    System *GetParentSystem() { return m_system; }
    const System *GetParentSystem() const { return m_system; }
    void SetParentSystem(System *system);
    ///@}

    /**
     * Returns the staff size (100 if no staff object is refered to)
     */
    int GetStaffSize() const;

    /**
     * Returns the spacing attribute object of correspond ScoreDef
     */
    const AttSpacing *GetAttSpacing() const;

    /**
     * @name Calculates the overflow (above or below) for the bounding box.
     * Looks if the bounding box is a FloatingPositioner or not, in which case it we take into account its m_drawingYRel
     * value.
     */
    ///@{
    int CalcOverflowAbove(const BoundingBox *box) const;
    int CalcOverflowBelow(const BoundingBox *box) const;
    ///@}

    /**
     * @name Getter for spacing
     */
    ///@{
    int GetMinimumSpacing(const Doc *doc) const;
    int CalcMinimumRequiredSpacing(const Doc *doc) const;
    SystemAligner::SpacingType GetSpacingType() const { return m_spacingType; }
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
    void SetRequestedSpaceAbove(int space);
    int GetRequestedSpaceAbove() const { return m_requestedSpaceAbove; }
    void SetRequestedSpaceBelow(int space);
    int GetRequestedSpaceBelow() const { return m_requestedSpaceBelow; }
    void SetRequestedSpacing(int spacing) { m_requestedSpacing = spacing; }
    int GetRequestedSpacing() const { return m_requestedSpacing; }
    int GetStaffHeight() const { return m_staffHeight; }
    void SetScoreDefClefOverflowAbove(int overflowAbove) { m_scoreDefClefOverflowAbove = overflowAbove; }
    int GetScoreDefClefOverflowAbove() const { return m_scoreDefClefOverflowAbove; }
    void SetScoreDefClefOverflowBelow(int overflowBelow) { m_scoreDefClefOverflowBelow = overflowBelow; }
    int GetScoreDefClefOverflowBelow() const { return m_scoreDefClefOverflowBelow; }
    ///@}

    /**
     * @name Returns justification factor based on staff type
     */
    ///@{
    double GetJustificationFactor(const Doc *doc) const;
    ///@}

    /**
     * @name Modify/Get the array of overflowing objects above or below
     */
    ///@{
    void AddBBoxAbove(BoundingBox *box) { m_overflowAboveBBoxes.push_back(box); }
    void AddBBoxBelow(BoundingBox *box) { m_overflowBelowBBoxes.push_back(box); }
    void ClearBBoxesAbove() { m_overflowAboveBBoxes.clear(); }
    void ClearBBoxesBelow() { m_overflowBelowBBoxes.clear(); }
    ArrayOfBoundingBoxes &GetBBoxesAboveForModification() { return m_overflowAboveBBoxes; }
    ArrayOfBoundingBoxes &GetBBoxesBelowForModification() { return m_overflowBelowBBoxes; }
    const ArrayOfBoundingBoxes &GetBBoxesAbove() { return m_overflowAboveBBoxes; }
    const ArrayOfBoundingBoxes &GetBBoxesBelow() { return m_overflowBelowBBoxes; }
    ///@}

    /**
     * Deletes all the FloatingPositioner objects.
     */
    void ClearPositioners();

    /**
     * Sort the FloatingPositioner objects.
     */
    void SortPositioners();

    /**
     * Find all the intersection points with a vertical line (top to bottom)
     */
    void FindAllIntersectionPoints(
        SegmentedLine &line, const BoundingBox &boundingBox, const std::vector<ClassId> &classIds, int margin) const;

    /**
     * Find overflow for the alignments taking bracket group elements into account
     */
    void AdjustBracketGroupSpacing(const Doc *doc, const StaffAlignment *previous, int spacing);

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
    /**
     * Returns minimus preset spacing
     */
    int GetMinimumStaffSpacing(const Doc *doc, const AttSpacing *attSpacing) const;

    /**
     * Return whether current staff alignment is at the start/end of a bracket group
     */
    bool IsInBracketGroup(bool isFirst) const;

public:
    //
private:
    /**
     * Defines spacing type between current staff and previous one
     */
    SystemAligner::SpacingType m_spacingType = SystemAligner::SpacingType::None;

    /**
     * The list of FloatingPositioner for the staff.
     */
    ArrayOfFloatingPositioners m_floatingPositioners;
    /**
     * Flag indicating whether the list of FloatingPositioner is sorted
     */
    bool m_floatingPositionersSorted;
    /**
     * Stores a pointer to the staff from which the aligner was created.
     * This is necessary since we don't always have all the staves.
     */
    Staff *m_staff;
    /**
     * Stores a pointer to the system to which the Alignment is belong to
     * This is necessary to reduce amount of dynamic_casts.
     */
    System *m_system;
    /**
     * Stores the position relative to the system.
     */
    int m_yRel;
    /**
     * Stores the verse@n of the staves attached to the aligner (above and below)
     */
    std::set<int> m_verseAboveNs;
    std::set<int> m_verseBelowNs;

    /**
     * @name values for storing the overflow and overlap
     */
    ///@{
    int m_overflowAbove;
    int m_overflowBelow;
    int m_overlap;
    int m_requestedSpaceAbove;
    int m_requestedSpaceBelow;
    int m_requestedSpacing;
    int m_staffHeight;
    int m_scoreDefClefOverflowAbove;
    int m_scoreDefClefOverflowBelow;
    ///@}

    /**
     * The list of overflowing bounding boxes (e.g., LayerElement or FloatingPositioner)
     */
    ArrayOfBoundingBoxes m_overflowAboveBBoxes;
    ArrayOfBoundingBoxes m_overflowBelowBBoxes;
};

} // namespace vrv

#endif
