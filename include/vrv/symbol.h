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
#include "textelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Symbol
//----------------------------------------------------------------------------

/**
 * This class models the MEI <symbol> element.
 */
class Symbol : public TextElement, public AttColor, public AttExtSymAuth, public AttExtSymNames, public AttTypography {
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
    std::string GetClassName() const override { return "symbol"; }
    ///@}

    /**
     * Only supported elements will be actually added to the child list.
     */
    bool IsSupportedChild(ClassId classId) override;

    /**
     * Get the SMuFL glyph for the symbol based on glyph.num or glyph.name
     */
    char32_t GetSymbolGlyph() const;

    /**
     * Interface for class functor visitation
     */
    ///@{
    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
    ///@}

private:
    //
public:
private:
};

} // namespace vrv

#endif
