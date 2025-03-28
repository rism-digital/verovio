/////////////////////////////////////////////////////////////////////////////
// Name:        lv.h
// Author:      Andriy Makarchuk
// Created:     1/07/2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_LV_H__
#define __VRV_LV_H__

#include "tie.h"

namespace vrv {

//----------------------------------------------------------------------------
// Lv
//----------------------------------------------------------------------------

/**
 * This class models the MEI <lv> element.
 */
class Lv : public Tie {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Lv();
    virtual ~Lv();
    Object *Clone() const override { return new Lv(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "lv"; }
    ///@}

    bool CalculatePosition(
        const Doc *doc, const Staff *staff, int x1, int x2, int spanningType, Point bezier[4]) override;

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
}; // lv

} // namespace vrv

#endif // __VRV_LV_H__
