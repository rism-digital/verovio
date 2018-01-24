/////////////////////////////////////////////////////////////////////////////
// Name:        lb.h
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LB_H__
#define __VRV_LB_H__

#include "atts_shared.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Lb
//----------------------------------------------------------------------------

/**
 * This class models the MEI <lb>
 */
class Lb : public TextElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Lb();
    virtual ~Lb();
    virtual Object *Clone() const { return new Lb(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Lb"; }
    virtual ClassId GetClassId() const { return LB; }
    ///@}

    /**
     * Lb is an empty element
     */
    virtual void AddChild(Object *object){};

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
