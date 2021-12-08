/////////////////////////////////////////////////////////////////////////////
// Name:        pagems.h
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PAGE_MS_H__
#define __VRV_PAGE_MS_H__

#include "pageelement.h"
#include "vrvdef.h"

namespace vrv {

class Measure;
class Object;

//----------------------------------------------------------------------------
// PageMsEnd
//----------------------------------------------------------------------------

/**
 * This class models an end milestone element and has no MEI equivalent.
 */
class PageMsEnd : public PageElement {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    PageMsEnd(Object *start);
    virtual ~PageMsEnd();
    void Reset() override;
    std::string GetClassName() const override { return "PageMsEnd"; }
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
     * See Object::UnCastOff
     */
    int UnCastOff(FunctorParams *functorParams) override;

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
    void SetEnd(PageMsEnd *end);
    PageMsEnd *GetEnd() { return m_end; }
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
    PageMsEnd *m_end;

private:
};

} // namespace vrv

#endif
