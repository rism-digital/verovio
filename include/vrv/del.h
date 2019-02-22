/////////////////////////////////////////////////////////////////////////////
// Name:        del.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DEL_H__
#define __VRV_DEL_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Del
//----------------------------------------------------------------------------

class Del : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Del();
    virtual ~Del();
    virtual Object *Clone() const { return new Del(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Del"; }
    virtual ClassId GetClassId() const { return DEL; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
