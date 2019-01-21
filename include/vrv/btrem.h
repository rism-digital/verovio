/////////////////////////////////////////////////////////////////////////////
// Name:        btrem.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_BTREM_H__
#define __VRV_BTREM_H__

#include "atts_cmn.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// BTrem (bowed tremolo)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <bTrem> element.
 */
class BTrem : public LayerElement, public AttTremMeasured {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    BTrem();
    virtual ~BTrem();
    virtual void Reset();
    virtual std::string GetClassName() const { return "BTrem"; }
    virtual ClassId GetClassId() const { return BTREM; }
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    virtual void AddChild(Object *object);

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
