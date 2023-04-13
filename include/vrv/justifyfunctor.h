/////////////////////////////////////////////////////////////////////////////
// Name:        justifyfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_JUSTIFYFUNCTOR_H__
#define __VRV_JUSTIFYFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// JustifyXFunctor
//----------------------------------------------------------------------------

/**
 * This class justifies the X positions.
 */
class JustifyXFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    JustifyXFunctor(Doc *doc);
    virtual ~JustifyXFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Set the full system width
     */
    void SetSystemFullWidth(int width) { m_systemFullWidth = width; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAlignment(Alignment *alignment) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMeasureAligner(MeasureAligner *measureAligner) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitSection(Section *section) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The relative X position of the next measure
    int m_measureXRel;
    // The justification ratio
    double m_justifiableRatio;
    // The left/right barline X position
    int m_leftBarLineX;
    int m_rightBarLineX;
    // The system full width (without system margins)
    int m_systemFullWidth;
    // Indicates shift of next measure due to section restart
    bool m_applySectionRestartShift;
};

//----------------------------------------------------------------------------
// JustifyYFunctor
//----------------------------------------------------------------------------

using ShiftMap = std::map<const StaffAlignment *, int>;

/**
 * This class justifies the Y positions.
 */
class JustifyYFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    JustifyYFunctor(Doc *doc);
    virtual ~JustifyYFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Setter and getter for various properties
     */
    ///@{
    void SetJustificationSum(double justificationSum) { m_justificationSum = justificationSum; }
    void SetSpaceToDistribute(int space) { m_spaceToDistribute = space; }
    const ShiftMap &GetShiftForStaff() const { return m_shiftForStaff; }
    ///@}

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
    // The cumulated shift
    int m_cumulatedShift;
    // The relative shift of the staff w.r.t. the system
    int m_relativeShift;
    // The amount of space for distribution
    int m_spaceToDistribute;
    // The sum of justification factors per page
    double m_justificationSum;
    // A map of calculated shifts per StaffAlignment
    // => this is transferred to the JustifyYAdjustCrossStaffFunctor
    ShiftMap m_shiftForStaff;
};

//----------------------------------------------------------------------------
// JustifyYAdjustCrossStaffFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the cross staff content after vertical justification.
 */
class JustifyYAdjustCrossStaffFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    JustifyYAdjustCrossStaffFunctor(Doc *doc);
    virtual ~JustifyYAdjustCrossStaffFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Transfer the shift map
     */
    void SetShiftForStaff(const ShiftMap &shiftMap) { m_shiftForStaff = shiftMap; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitChord(Chord *chord) override;
    ///@}

protected:
    //
private:
    /*
     * Calculate the shift due to vertical justification
     */
    int GetShift(const Staff *staff) const;

public:
    //
private:
    // A map of calculated shifts per StaffAlignment
    // => this is transferred from the JustifyYFunctor
    ShiftMap m_shiftForStaff;
};

} // namespace vrv

#endif // __VRV_JUSTIFYFUNCTOR_H__
