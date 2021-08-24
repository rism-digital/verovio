/////////////////////////////////////////////////////////////////////////////
// Name:        pageboundary.h
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PAGE_BOUNDARY_H__
#define __VRV_PAGE_BOUNDARY_H__

#include "pageelement.h"
#include "vrvdef.h"

namespace vrv {

class Measure;
class Object;

//----------------------------------------------------------------------------
// PageElementEnd
//----------------------------------------------------------------------------

/**
 * This class models an end milestone element and has no MEI equivalent.
 */
class PageElementEnd : public PageElement {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    PageElementEnd(Object *start);
    virtual ~PageElementEnd();
    virtual void Reset();
    virtual std::string GetClassName() const { return "PageElementEnd"; }
    ///@}

    // void SetMeasure(Measure *measure) { m_drawingMeasure = measure; }
    // Measure *GetMeasure() { return m_drawingMeasure; }

    /**
     * @name Get the corresponding boundary start
     */
    ///@{
    Object *GetStart() { return m_start; }
    std::string GetStartClassName() const { return m_startClassName; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CastOffSystems
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

    /**
     * See Object::CastOffPages
     */
    virtual int CastOffPages(FunctorParams *functorParams);

    /**
     * See Object::CastOffEncoding
     */
    virtual int CastOffEncoding(FunctorParams *functorParams);

    /**
     * See Object::UnCastOff
     */
    virtual int UnCastOff(FunctorParams *functorParams);

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
// PageElementStartInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for container elements that have to be turned to milestones in a page-base representation.
 */
class PageElementStartInterface {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    PageElementStartInterface();
    virtual ~PageElementStartInterface();
    virtual void Reset();
    ///@}

    /**
     * @name Set and get the first LayerElement
     * The setter asserts that no LayerElement was previously set.
     */
    ///@{
    void SetEnd(PageElementEnd *end);
    PageElementEnd *GetEnd() { return m_end; }
    bool IsPageBoundary() { return (m_end != NULL); }
    ///@}

    /**
     *
     */
    void ConvertToPageBasedBoundary(Object *object, Object *parent);

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
    PageElementEnd *m_end;

private:
};

} // namespace vrv

#endif
