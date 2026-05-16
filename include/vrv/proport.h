/////////////////////////////////////////////////////////////////////////////
// Name:        proport.h
// Author:      Donald Byrd
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PROPORT_H__
#define __VRV_PROPORT_H__

#include "atts_shared.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Proport
//----------------------------------------------------------------------------

/**
 * This class models the MEI <proport> element.
 */
class Proport : public LayerElement, public AttDurationRatio {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes.
     */
    ///@{
    Proport();
    virtual ~Proport();
    Object *Clone() const override { return new Proport(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "proport"; }
    ///@}

    int GetCumulatedNum() const;
    int GetCumulatedNumbase() const;

    void Cumulate(const Proport *proport);
    void ResetCumulate();

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

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
    /** the cumulated num and numbase */
    int m_cumulatedNum;
    int m_cumulatedNumbase;
};

} // namespace vrv

#endif
