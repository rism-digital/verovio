/////////////////////////////////////////////////////////////////////////////
// Name:        sic.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SIC_H__
#define __VRV_SIC_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Sic
//----------------------------------------------------------------------------

class Sic : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Sic();
    virtual ~Sic();
    virtual Object *Clone() const { return new Sic(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Sic"; }
    virtual ClassId GetClassId() const { return SIC; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
