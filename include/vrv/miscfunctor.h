/////////////////////////////////////////////////////////////////////////////
// Name:        miscfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MISCFUNCTOR_H__
#define __VRV_MISCFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// ApplyPPUFactorFunctor
//----------------------------------------------------------------------------

/**
 * This class applies the Pixel Per Unit factor of the page to its elements.
 */
class ApplyPPUFactorFunctor : public MutableFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ApplyPPUFactorFunctor();
    virtual ~ApplyPPUFactorFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitPage(Page *page) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current page
    Page *m_page;
};

//----------------------------------------------------------------------------
// GetAlignmentLeftRightFunctor
//----------------------------------------------------------------------------

/**
 * This class retrieves the minimum left and maximum right for an alignment.
 */
class GetAlignmentLeftRightFunctor : public ConstFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    GetAlignmentLeftRightFunctor();
    virtual ~GetAlignmentLeftRightFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Set the classes to exclude
     */
    void ExcludeClasses(const std::vector<ClassId> &excludeClasses) { m_excludeClasses = excludeClasses; }

    /*
     * Get the minimum left and maximum right
     */
    ///@{
    int GetMinLeft() const { return m_minLeft; }
    int GetMaxRight() const { return m_maxRight; }
    ///@}

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitObject(const Object *object) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The min left and right
    int m_minLeft;
    int m_maxRight;
    // The classes which are ignored
    std::vector<ClassId> m_excludeClasses;
};

} // namespace vrv

#endif // __VRV_MISCFUNCTOR_H__
