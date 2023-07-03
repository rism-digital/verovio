/////////////////////////////////////////////////////////////////////////////
// Name:        adjustfloatingpositionerfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTFLOATINGPOSITIONERFUNCTOR_H__
#define __VRV_ADJUSTFLOATINGPOSITIONERFUNCTOR_H__

#include "functor.h"

namespace vrv {

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
    //
public:
    //
private:
    // The class ID
    ClassId m_classId;
    // Indicates if we are processing floating objects to be put in between the staff
    bool m_inBetween;
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
