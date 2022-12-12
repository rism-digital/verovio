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
    ///

    /**
     * @name Set or reset a temporary parent for the SymbolDef.
     * This should be used only in very specific temporary operations because the ownership is not changed.
     * It means that the parent must be replaced back.
     */
    ///@{
    void SetTemporaryParent(Object *parent);
    void ResetTemporaryParent();
    ///@}

private:
    //
public:
    //
private:
    /** The original parent for when a temporary parent is set */
    Object *m_originalParent;
};

} // namespace vrv

#endif
