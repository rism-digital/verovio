/////////////////////////////////////////////////////////////////////////////
// Name:        symboldef.h
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYMBOLDEF_H__
#define __VRV_SYMBOLDEF_H__

#include "atts_shared.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// SymbolDef
//----------------------------------------------------------------------------

class SymbolDef : public Object {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    SymbolDef();
    virtual ~SymbolDef();
    void Reset() override;
    std::string GetClassName() const override { return "SymbolDef"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(Object *object) override;
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
