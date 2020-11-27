/////////////////////////////////////////////////////////////////////////////
// Name:        grpsym.h
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_GRPSYM_H__
#define __VRV_GRPSYM_H__

#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// GrpSym
//----------------------------------------------------------------------------

/**
 * This class represents a MEI grpSym.
 */
class GrpSym : public Object {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    GrpSym();
    virtual ~GrpSym();
    virtual Object *Clone() const { return new GrpSym(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "GrpSym"; }
    //virtual ClassId GetClassId() const { return GRPSYM; }
    ///@}

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_GRPSYM_H__