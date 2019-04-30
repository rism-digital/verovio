/////////////////////////////////////////////////////////////////////////////
// Name:        corr.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CORR_H__
#define __VRV_CORR_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Corr
//----------------------------------------------------------------------------

class Corr : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Corr();
    virtual ~Corr();
    virtual Object *Clone() const { return new Corr(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Corr"; }
    virtual ClassId GetClassId() const { return CORR; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
