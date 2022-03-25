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
    Object *Clone() const override { return new Space(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Space"; }
    ///@}

    /**
     * @name Getter to interfaces
     */
    ///@{
    DurationInterface *GetDurationInterface() override { return vrv_cast<DurationInterface *>(this); }
    const DurationInterface *GetDurationInterface() const override { return vrv_cast<const DurationInterface *>(this); }
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
