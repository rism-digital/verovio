/////////////////////////////////////////////////////////////////////////////
// Name:        systemmilestone.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYSTEM_MILESTONE_H__
#define __VRV_SYSTEM_MILESTONE_H__

#include "systemelement.h"
#include "vrvdef.h"

namespace vrv {

class Measure;
class Object;
class PrepareMilestonesFunctor;
class ResetDataFunctor;

//----------------------------------------------------------------------------
// SystemMilestoneEnd
//----------------------------------------------------------------------------

/**
 * This class models an end milestone element at the system level.
 */
class SystemMilestoneEnd : public SystemElement {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    SystemMilestoneEnd(Object *start);
    virtual ~SystemMilestoneEnd();
    void Reset() override;
    std::string GetClassName() const override { return "systemMilestoneEnd"; }
    ///@}

    void SetMeasure(Measure *measure) { m_drawingMeasure = measure; }
    Measure *GetMeasure() { return m_drawingMeasure; }
    const Measure *GetMeasure() const { return m_drawingMeasure; }

    /**
     * @name Get the corresponding milestone start
     */
    ///@{
    Object *GetStart() { return m_start; }
    const Object *GetStart() const { return m_start; }
    std::string GetStartClassName() const { return m_startClassName; }
    ///@}

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

protected:
    //
private:
    //
public:
    //
protected:
    Object *m_start;

private:
    /** The class name of the corresponding start */
    std::string m_startClassName;
    /** The last measure child of the element */
    Measure *m_drawingMeasure;
};

//----------------------------------------------------------------------------
// SystemMilestoneInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for container elements that have to be turned to milestones in a page-base representation.
 */
class SystemMilestoneInterface {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    SystemMilestoneInterface();
    virtual ~SystemMilestoneInterface();
    virtual void Reset();
    ///@}

    void SetMeasure(Measure *measure) { m_drawingMeasure = measure; }
    Measure *GetMeasure() { return m_drawingMeasure; }
    const Measure *GetMeasure() const { return m_drawingMeasure; }

    /**
     * @name Set and get the first LayerElement
     * The setter asserts that no LayerElement was previously set.
     */
    ///@{
    void SetEnd(SystemMilestoneEnd *end);
    SystemMilestoneEnd *GetEnd() { return m_end; }
    const SystemMilestoneEnd *GetEnd() const { return m_end; }
    bool IsSystemMilestone() const { return (m_end != NULL); }
    ///@}

    /**
     *
     */
    void ConvertToPageBasedMilestone(Object *object, Object *parent);

    //-----------------//
    // Pseudo functors //
    //-----------------//

    /**
     * We have functor code in the interface for avoiding code duplication in each implementation class.
     */
    ///@{
    FunctorCode InterfacePrepareMilestones(PrepareMilestonesFunctor &functor);
    FunctorCode InterfaceResetData(ResetDataFunctor &functor);
    ///@}

protected:
    //
private:
    //
public:
    //
protected:
    SystemMilestoneEnd *m_end;

private:
    /** The first measure child of the element */
    Measure *m_drawingMeasure;
};

} // namespace vrv

#endif
