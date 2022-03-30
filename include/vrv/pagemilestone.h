/////////////////////////////////////////////////////////////////////////////
// Name:        pagemilestone.h
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PAGE_MILESTONE_H__
#define __VRV_PAGE_MILESTONE_H__

#include "pageelement.h"
#include "vrvdef.h"

namespace vrv {

class Measure;
class Object;

//----------------------------------------------------------------------------
// PageMilestoneEnd
//----------------------------------------------------------------------------

/**
 * This class models an end milestone element and has no MEI equivalent.
 */
class PageMilestoneEnd : public PageElement {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    PageMilestoneEnd(Object *start);
    virtual ~PageMilestoneEnd();
    void Reset() override;
    std::string GetClassName() const override { return "PageMilestoneEnd"; }
    ///@}

    // void SetMeasure(Measure *measure) { m_drawingMeasure = measure; }
    // Measure *GetMeasure() { return m_drawingMeasure; }

    /**
     * @name Get the corresponding milestone start
     */
    ///@{
    Object *GetStart() const { return m_start; }
    std::string GetStartClassName() const { return m_startClassName; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CastOffSystems
     */
    int CastOffSystems(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffPages
     */
    int CastOffPages(FunctorParams *functorParams) override;

    /**
     * See Object::CastOffEncoding
     */
    int CastOffEncoding(FunctorParams *functorParams) override;

    /**
     * See Object::InitSelection
     */
    int InitSelection(FunctorParams *) override;

    /**
     * See Object::UnCastOff
     */
    int UnCastOff(FunctorParams *functorParams) override;

    /**
     * See Object::Transpose
     */
    int Transpose(FunctorParams *functorParams) override;

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
};

//----------------------------------------------------------------------------
// PageMilestoneInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for container elements that have to be turned to milestones in a page-base representation.
 */
class PageMilestoneInterface {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    PageMilestoneInterface();
    virtual ~PageMilestoneInterface();
    virtual void Reset();
    ///@}

    /**
     * @name Set and get the first LayerElement
     * The setter asserts that no LayerElement was previously set.
     */
    ///@{
    void SetEnd(PageMilestoneEnd *end);
    PageMilestoneEnd *GetEnd() { return m_end; }
    bool IsPageMilestone() { return (m_end != NULL); }
    ///@}

    /**
     *
     */
    void ConvertToPageBasedMilestone(Object *object, Object *parent);

    //-----------------//
    // Pseudo functors //
    //-----------------//

protected:
    //
private:
    //
public:
    //
protected:
    /**
     * The corresponding end element
     */
    PageMilestoneEnd *m_end;

private:
};

} // namespace vrv

#endif
