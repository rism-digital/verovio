/////////////////////////////////////////////////////////////////////////////
// Name:        symbol.h
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SYMBOL_H__
#define __VRV_SYMBOL_H__

#include "atts_externalsymbols.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "object.h"

namespace vrv {

//----------------------------------------------------------------------------
// Symbol
//----------------------------------------------------------------------------

/**
 * This class models the MEI <symbol> element.
 */
class Symbol : public Object, public AttColor, public AttExtSym {
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
    /**
     * Holds the visibility (hidden or visible) for an symbol element.
     * By default, a symbo elements is visible but it is make invisible when with mixed content
     * The reason is that we cannot render SVG <g> within <text>.
     * Symbol with text is not supported by Verovio.
     */
    VisibilityType m_visibility;

private:
};

} // namespace vrv

#endif
