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
    std::string GetClassName() const override { return "symbolTable"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /**
     * Disable cloning of the symboleTable (for now?).
     * It does not make sense you carry copying the symbol table across the systems.
     */
    Object *Clone() const override { return NULL; }

private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif
