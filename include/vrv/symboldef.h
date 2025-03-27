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
    std::string GetClassName() const override { return "symbolDef"; }
    ///@}

    /**
     * @name Methods for adding allowed content
     */
    ///@{
    bool IsSupportedChild(ClassId classId) override;
    ///@}

    /**
     * @name Getters for the drawing width and height of the symbolDef content.
     */
    ///@{
    int GetSymbolWidth(Doc *doc, int staffSize, bool dimin) const;
    int GetSymbolHeight(Doc *doc, int staffSize, bool dimin) const;
    ///@}

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
    /** Calculate the size of the symbolDef content by iterating through the children */
    std::pair<int, int> GetSymbolSize(Doc *doc, int staffSize, bool dimin) const;

public:
    //
private:
    /** The original parent for when a temporary parent is set */
    Object *m_originalParent;
};

} // namespace vrv

#endif
