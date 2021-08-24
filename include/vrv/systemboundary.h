/////////////////////////////////////////////////////////////////////////////
// Name:        systemboundary.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYSTEM_BOUNDARY_H__
#define __VRV_SYSTEM_BOUNDARY_H__

#include "systemelement.h"
#include "vrvdef.h"

namespace vrv {

class Measure;
class Object;

//----------------------------------------------------------------------------
// SystemElementEnd
//----------------------------------------------------------------------------

/**
 * This class models an end milestone element and has no MEI equivalent.
 */
class SystemElementEnd : public SystemElement {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    SystemElementEnd(Object *start);
    virtual ~SystemElementEnd();
    virtual void Reset();
    virtual std::string GetClassName() const { return "systemElementEnd"; }
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
// SystemElementStartInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for container elements that have to be turned to milestones in a page-base representation.
 */
class SystemElementStartInterface {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    SystemElementStartInterface();
    virtual ~SystemElementStartInterface();
    virtual void Reset();
    ///@}

    void SetMeasure(Measure *measure) { m_drawingMeasure = measure; }
    Measure *GetMeasure() { return m_drawingMeasure; }

    /**
     * @name Set and get the first LayerElement
     * The setter asserts that no LayerElement was previously set.
     */
    ///@{
    void SetEnd(SystemElementEnd *end);
    SystemElementEnd *GetEnd() { return m_end; }
    bool IsSystemBoundary() { return (m_end != NULL); }
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
    SystemElementEnd *m_end;

private:
    /** The first measure child of the element */
    Measure *m_drawingMeasure;
};

} // namespace vrv

#endif
