/////////////////////////////////////////////////////////////////////////////
// Name:        ending.h
// Author:      Laurent Pugin
// Created:     14/07/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ENDING_H__
#define __VRV_ENDING_H__

#include "atts_shared.h"
#include "floatingelement.h"

namespace vrv {

class Measure;

//----------------------------------------------------------------------------
// EndingBoundary
//----------------------------------------------------------------------------

class EndingBoundary : public FloatingElement, public AttCommon {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    EndingBoundary(EndingBoundary *startBoundary = NULL);
    virtual ~EndingBoundary();
    virtual void Reset();
    virtual std::string GetClassName() const { return "EndingBoundary"; };
    virtual ClassId Is() const { return ENDING_BOUNDARY; };
    ///@}

    void SetMeasure(Measure *measure) { m_drawingMeasure = measure; }
    Measure *GetMeasure() { return m_drawingMeasure; }

    void SetEndBoundary(EndingBoundary *endBoundary) { m_endBoundary = endBoundary; }
    EndingBoundary *GetEndBoundary() { return m_endBoundary; }
    EndingBoundary *GetStartBoundary() { return m_startBoundary; }

    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    bool IsStartBoundary() { return (m_startBoundary == NULL); }
    bool IsEndBoundary() { return (m_startBoundary != NULL); }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareEndings.
     */
    virtual int PrepareEndings(FunctorParams *functorParams);

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * Fill a page by adding systems with the appropriate length.
     * For Endings, this means only moving them since their width is not taken into
     * account
     */
    virtual int CastOffSystems(FunctorParams *functorParams);

private:
    //
public:
    //
private:
    /** The measure (first or last) child of the ending */
    Measure *m_drawingMeasure;
    /**
     * Pointer for starting and ending boundary.
     */
    EndingBoundary *m_startBoundary;
    EndingBoundary *m_endBoundary;
};

} // namespace vrv

#endif
