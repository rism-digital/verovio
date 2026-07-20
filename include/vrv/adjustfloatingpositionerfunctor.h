/////////////////////////////////////////////////////////////////////////////
// Name:        adjustfloatingpositionerfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTFLOATINGPOSITIONERFUNCTOR_H__
#define __VRV_ADJUSTFLOATINGPOSITIONERFUNCTOR_H__

#include "functor.h"

#include <functional>
#include <map>

namespace vrv {

class Measure;

//----------------------------------------------------------------------------
// AdjustFloatingPositionersFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of all floating positioners, staff by staff.
 */
class AdjustFloatingPositionersFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustFloatingPositionersFunctor(Doc *doc);
    virtual ~AdjustFloatingPositionersFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitStaffAlignment(StaffAlignment *staffAlignment) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    FunctorCode AdjustCurrentPositioners(StaffAlignment *staffAlignment);
    FunctorCode AdjustStaffItemOrder(StaffAlignment *staffAlignment);
    void AdjustLegacyPlace(StaffAlignment *staffAlignment, data_STAFFREL place, bool includeWithin = false);
    void AdjustOrderedPlace(StaffAlignment *staffAlignment, data_STAFFREL place, bool includeWithin = false);
    void AdjustPositionerGroups(StaffAlignment *staffAlignment, data_STAFFREL place);
    void BuildPositionerGroups(StaffAlignment *staffAlignment, data_STAFFREL place, bool includeWithin);
    void StabilizePositionerGroups(StaffAlignment *staffAlignment, data_STAFFREL place,
        const ArrayOfBoundingBoxes &baseBoxes, const std::function<void()> &processPositioners);
    bool HasExplicitPositionerGroup(StaffAlignment *staffAlignment, data_STAFFREL place) const;
    bool HasStaffItemOrder(StaffAlignment *staffAlignment, data_STAFFREL place) const;
    void ProcessClass(StaffAlignment *staffAlignment, ClassId classId, data_STAFFREL place,
        data_STAFFITEM staffItem = STAFFITEM_NONE);
    void ProcessPositioners(StaffAlignment *staffAlignment, const ArrayOfFloatingPositioners &positioners,
        ClassId classId, data_STAFFREL place, data_STAFFITEM staffItem, bool keepGroupPosition);

public:
    //
private:
    // The class ID
    ClassId m_classId;
    // Restrict a shared Verovio class to the exact MEI staff-item category
    data_STAFFITEM m_staffItem;
    // Indicates if we are processing floating objects to be put in between the staff
    bool m_inBetween;
    // Restrict processing to one placement (NONE keeps the legacy behavior)
    data_STAFFREL m_place;
    // Include positioners within the staff in the current below-staff pass
    bool m_includeWithin;
    // Restrict processing to a prepared set of positioners
    const ArrayOfFloatingPositioners *m_positioners;
    // Keep grouped positioners on their common baseline during collision stabilization
    bool m_keepGroupPosition;
    // Allow a stabilization pass to move complete groups in response to collisions
    bool m_movePositionerGroups;
    // Positioners indexed by their explicit or automatic drawing group
    std::map<long long, ArrayOfFloatingPositioners> m_positionerGroups;
    // Detect explicit vertical groups that require the ordered grouping path
    bool m_detectExplicitGroups;
    bool m_hasExplicitGroups;
    // Process each staff using its effective scoreDef / staffDef order
    bool m_useStaffItemOrder;
};

//----------------------------------------------------------------------------
// AdjustFloatingPositionerGrpsFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of all floating positioners that are grouped, staff by staff.
 */
class AdjustFloatingPositionerGrpsFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustFloatingPositionerGrpsFunctor(Doc *doc);
    virtual ~AdjustFloatingPositionerGrpsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Various setter
     */
    ///@{
    void SetClassIDs(const std::vector<ClassId> &classIds) { m_classIds = classIds; }
    void SetPlace(data_STAFFREL place) { m_place = place; }
    void SetGroupType(int groupType) { m_groupType = groupType; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitStaffAlignment(StaffAlignment *staffAlignment) override;
    ///@}

protected:
    //
private:
    // Adjust the position of groups to ensure that any group is positioned further away from the staff
    // than preceding groups
    void AdjustGroupsMonotone(const StaffAlignment *staffAlignment, const ArrayOfFloatingPositioners &positioners,
        ArrayOfIntPairs &grpIdYRel) const;

public:
    //
private:
    // The class IDs to group
    std::vector<ClassId> m_classIds;
    // The place w.r.t. the staff
    data_STAFFREL m_place;
    // Negative for explicit groups, positive for automatic groups, zero for both
    int m_groupType;
};

//----------------------------------------------------------------------------
// AdjustFloatingPositionersBetweenFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the position of floating positioners placed between staves.
 */
class AdjustFloatingPositionersBetweenFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustFloatingPositionersBetweenFunctor(Doc *doc);
    virtual ~AdjustFloatingPositionersBetweenFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitStaffAlignment(StaffAlignment *staffAlignment) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The previous staff alignment
    StaffAlignment *m_previousStaffAlignment;
};

} // namespace vrv

#endif // __VRV_ADJUSTFLOATINGPOSITIONERFUNCTOR_H__
