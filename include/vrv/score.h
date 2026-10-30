/////////////////////////////////////////////////////////////////////////////
// Name:        score.h
// Author:      Laurent Pugin
// Created:     2026
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SCORE_H__
#define __VRV_SCORE_H__

#include "scoreorpartelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// Score
//----------------------------------------------------------------------------

/**
 * This class represent a <score> MEI.
 */
class Score : public ScoreOrPartElement {

public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Score(bool createScoreDef);
    Score();
    virtual ~Score();
    Object *Clone() const override { return new Score(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "score"; }
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
