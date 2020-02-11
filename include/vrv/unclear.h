/////////////////////////////////////////////////////////////////////////////
// Name:        unclear.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_UNCLEAR_H__
#define __VRV_UNCLEAR_H__

#include "atts_shared.h"
#include "editorial.h"

namespace vrv {

//----------------------------------------------------------------------------
// Unclear
//----------------------------------------------------------------------------

class Unclear : public EditorialElement, public AttSource {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Unclear();
    virtual ~Unclear();
    virtual Object *Clone() const { return new Unclear(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Unclear"; }
    virtual ClassId GetClassId() const { return UNCLEAR; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
