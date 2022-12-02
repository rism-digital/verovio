/////////////////////////////////////////////////////////////////////////////
// Name:        symboltable.h
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYMBOLTABLE_H__
#define __VRV_SYMBOLTABLE_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// SymbolTable
//----------------------------------------------------------------------------

class SymbolTable : public Object {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    SymbolTable();
    virtual ~SymbolTable();
    void Reset() override;
    std::string GetClassName() const override { return "SymbolTable"; }
    ///@}

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
