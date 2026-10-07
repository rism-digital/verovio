/////////////////////////////////////////////////////////////////////////////
// Name:        refrain.h
// Author:      Simon Waloschek
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_REFRAIN_H__
#define __VRV_REFRAIN_H__

#include "lyricelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Refrain
//----------------------------------------------------------------------------

/** Recurring note-attached lyrics, such as a chorus. */
class Refrain : public LyricElement, public AttNNumberLike {
public:
    Refrain();
    virtual ~Refrain();
    Object *Clone() const override { return new Refrain(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "refrain"; }

    FunctorCode Accept(Functor &functor) override;
    FunctorCode Accept(ConstFunctor &functor) const override;
    FunctorCode AcceptEnd(Functor &functor) override;
    FunctorCode AcceptEnd(ConstFunctor &functor) const override;
};

} // namespace vrv

#endif
