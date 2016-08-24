/////////////////////////////////////////////////////////////////////////////
// Name:        section.h
// Author:      Laurent Pugin
// Created:     14/07/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SECTION_H__
#define __VRV_SECTION_H__

#include "atts_shared.h"
#include "boundary.h"
#include "object.h"

namespace vrv {

class Section;

//----------------------------------------------------------------------------
// Ending
//----------------------------------------------------------------------------

class Section : public Object, public BoundaryStartInterface, public AttCommon, public AttCommonPart {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Section();
    virtual ~Section();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Section"; };
    virtual ClassId Is() const { return SECTION; };
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
    //
};

} // namespace vrv

#endif
