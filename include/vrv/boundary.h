/////////////////////////////////////////////////////////////////////////////
// Name:        boundary.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BOUNDARY_H__
#define __VRV_BOUNDARY_H__

#include "object.h"
#include "vrvdef.h"

namespace vrv {

class Object;

//----------------------------------------------------------------------------
// BoundaryEnd
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements having a single time point, such as tempo, reh, etc..
 * It is not an abstract class but should not be instantiated directly.
 */
class BoundaryEnd : public Object {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    BoundaryEnd(Object *start);
    virtual ~BoundaryEnd();
    virtual void Reset();
    virtual std::string GetClassName() const;
    virtual ClassId Is() const { return MEASURE; };
    ///@}

    /**
     * @name Get the corresponding boundary start
     */
    ///@{
    Object *GetStart() { return m_start; };
    ///@}

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
    Object *m_start;

private:
    std::string m_startClassName;
};

//----------------------------------------------------------------------------
// BoundaryStartInterface
//----------------------------------------------------------------------------

/**
 * This class is an interface for elements having a single time point, such as tempo, reh, etc..
 * It is not an abstract class but should not be instantiated directly.
 */
class BoundaryStartInterface {
public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method reset all attribute classes
     */
    ///@{
    BoundaryStartInterface();
    virtual ~BoundaryStartInterface();
    virtual void Reset();
    ///@}

    /**
     * @name Set and get the first LayerElement
     * The setter asserts that no LayerElement was previously set.
     */
    ///@{
    void SetEnd(BoundaryEnd *end);
    BoundaryEnd *GetEnd() { return m_end; }
    bool IsBoundary() { return (m_end != NULL); }
    ///@}

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
    BoundaryEnd *m_end;

private:
};

} // namespace vrv

#endif
