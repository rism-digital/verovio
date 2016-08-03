/////////////////////////////////////////////////////////////////////////////
// Name:        ending.h
// Author:      Laurent Pugin
// Created:     14/07/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ENDING_H__
#define __VRV_ENDING_H__

#include "atts_shared.h"
#include "boundary.h"
#include "floatingelement.h"

namespace vrv {

class Measure;

//----------------------------------------------------------------------------
// Ending
//----------------------------------------------------------------------------

class Ending : public FloatingElement, public BoundaryStartInterface, public AttCommon {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Ending();
    virtual ~Ending();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Ending"; };
    virtual ClassId Is() const { return ENDING; };
    ///@}

    // void SetEndBoundary(Ending *endBoundary) { m_endBoundary = endBoundary; }
    // Ending *GetEndBoundary() { return m_endBoundary; }
    // Ending *GetStartBoundary() { return m_startBoundary; }

    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    // bool IsStartBoundary() { return (m_startBoundary == NULL); }
    // bool IsEndBoundary() { return (m_startBoundary != NULL); }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::PrepareBoundaries.
     */
    virtual int PrepareBoundaries(FunctorParams *functorParams);

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
    /**
     * Pointer for starting and ending boundary.
     */
    // Ending *m_startBoundary;
    // Ending *m_endBoundary;
};

} // namespace vrv

#endif
