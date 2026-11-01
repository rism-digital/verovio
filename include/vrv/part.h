/////////////////////////////////////////////////////////////////////////////
// Name:        part.h
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PART_H__
#define __VRV_PART_H__

#include "scoreorpartelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Part
//----------------------------------------------------------------------------

/**
 * This class represent a <part> MEI.
 */
class Part : public ScoreOrPartElement {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Part(bool createScoreDef);
    Part();
    virtual ~Part();
    Object *Clone() const override { return new Part(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "part"; }
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
