/////////////////////////////////////////////////////////////////////////////
// Name:        phrase.h
// Author:      Laurent Pugin
// Created:     15/06/2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PHRASE_H__
#define __VRV_PHRASE_H__

#include "slur.h"

namespace vrv {

//----------------------------------------------------------------------------
// Phrase
//----------------------------------------------------------------------------

class Phrase : public Slur {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Phrase();
    virtual ~Phrase();
    Object *Clone() const override { return new Phrase(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "phrase"; }
    ///@}

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
