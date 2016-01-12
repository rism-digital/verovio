/////////////////////////////////////////////////////////////////////////////
// Name:        timestamp.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_TIMESTAMP_H__
#define __VRV_TIMESTAMP_H__

#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// TimestampAttr
//----------------------------------------------------------------------------

class TimestampAttr : public LayerElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    TimestampAttr();
    virtual ~TimestampAttr();
    virtual void Reset();
    virtual std::string GetClassName() { return "TimestampAttr"; };
    virtual ClassId Is() { return TIMESTAMP_ATTR; };
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
