/////////////////////////////////////////////////////////////////////////////
// Name:        div.h
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_DIV_H__
#define __VRV_DIV_H__

#include "textlayoutelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Div
//----------------------------------------------------------------------------

/**
 * This class represents an MEI Div.
 */
class Div : public TextLayoutElement {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Div();
    virtual ~Div();
    void Reset() override;
    std::string GetClassName() const override { return "Div"; }
    ///@}

    /**
     * Overriden to get the appropriate margin
     */
    int GetTotalHeight(const Doc *doc) const override;

    int GetTotalWidth(const Doc *doc) const;

    //----------//
    // Functors //
    //----------//

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
    //
private:
    //
};

} // namespace vrv

#endif
