/////////////////////////////////////////////////////////////////////////////
// Name:        abbr.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ABBR_H__
#define __VRV_ABBR_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Abbr
//----------------------------------------------------------------------------

class Abbr : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Abbr();
    virtual ~Abbr();
    virtual Object *Clone() const { return new Abbr(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Abbr"; }
    virtual ClassId GetClassId() const { return ABBR; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
