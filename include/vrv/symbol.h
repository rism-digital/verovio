/////////////////////////////////////////////////////////////////////////////
// Name:        symbol.h
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYMBOL_H__
#define __VRV_SYMBOL_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Symbol
//----------------------------------------------------------------------------

/**
 * This class models the MEI <symbol> element.
 */
class Symbol : public Object {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    Symbol();
    virtual ~Symbol();
    Object *Clone() const override { return new Symbol(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Symbol"; }
    ///@}

    /**
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(Object *object) override;

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
