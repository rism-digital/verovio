/////////////////////////////////////////////////////////////////////////////
// Name:        tabrhythm.h
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TABRHYTHM_H__
#define __VRV_TABRHYTHM_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// TabRhythm
//----------------------------------------------------------------------------

/**
 * This class models the MEI <tabRhythm> element.
 */
class TabRhythm : public LayerElement, public AttNNumberLike {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    TabRhythm();
    virtual ~TabRhythm();
    virtual void Reset();
    virtual std::string GetClassName() const { return "TabRhythm"; };
    virtual ClassId GetClassId() const { return TABRHYTHM; };
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Add an element to a element.
     */
    virtual bool IsSupportedChild(Object *object);

    //----------//
    // Functors //
    //----------//

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
