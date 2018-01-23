/////////////////////////////////////////////////////////////////////////////
// Name:        num.h
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_NUM_H__
#define __VRV_NUM_H__

#include "atts_shared.h"
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Num
//----------------------------------------------------------------------------

/**
 * This class models the MEI <num>
 */
class Num : public TextElement, public AttLabelled {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Num();
    virtual ~Num();
    virtual Object *Clone() const { return new Num(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "Num"; }
    virtual ClassId GetClassId() const { return NUM; }
    ///@}

    /**
     * Add an element (text) to a num.
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
