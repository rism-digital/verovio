/////////////////////////////////////////////////////////////////////////////
// Name:        space.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SPACE_H__
#define __VRV_SPACE_H__

#include "durationinterface.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Space
//----------------------------------------------------------------------------

/**
 * This class models the MEI <space> element.
 */
class Space : public LayerElement, public DurationInterface {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Space();
    virtual ~Space();
    virtual Object *Clone() const { return new Space(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Space"; }
    virtual ClassId GetClassId() const { return SPACE; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual DurationInterface *GetDurationInterface() { return dynamic_cast<DurationInterface *>(this); }
    ///@}

    //----------//
    // Functors //
    //----------//

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
