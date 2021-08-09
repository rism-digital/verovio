/////////////////////////////////////////////////////////////////////////////
// Name:        pageboundary.h
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PAGE_BOUNDARY_H__
#define __VRV_PAGE_BOUNDARY_H__

#include "systemelement.h"
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
class PageElementEnd : public SystemElement {
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
    virtual ClassId GetClassId() const { return SYSTEM_ELEMENT_END; }
    ///@}

    void SetMeasure(Measure *measure) { m_drawingMeasure = measure; }
    Measure *GetMeasure() { return m_drawingMeasure; }

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
     * See Object::PrepareBoundaries
     */
    virtual int PrepareBoundaries(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::CastOffSystems
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

    /**
     * See Object::PrepareFloatingGrps
     */
    virtual int PrepareFloatingGrps(FunctorParams *functoParams);

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

    void SetMeasure(Measure *measure) { m_drawingMeasure = measure; }
    Measure *GetMeasure() { return m_drawingMeasure; }

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

    /**
     * See Object::PrepareBoundaries
     * Called from Ending::PrepareBoundaries and EditorialElement::PrepareBoundaries
     */
    virtual int InterfacePrepareBoundaries(FunctorParams *functorParams);

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     * Called from Ending::ResetDrawing and EditorialElement::ResetDrawing
     */
    virtual int InterfaceResetDrawing(FunctorParams *functorParams);

protected:
    //
private:
    //
public:
    //
protected:
    PageElementEnd *m_end;

private:
    /** The first measure child of the element */
    Measure *m_drawingMeasure;
};

} // namespace vrv

#endif
