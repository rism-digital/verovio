/////////////////////////////////////////////////////////////////////////////
// Name:        adjusttupletsyfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTTUPLETSYFUNCTOR_H__
#define __VRV_ADJUSTTUPLETSYFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustTupletsYFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the Y position of tuplet brackets and num.
 */
class AdjustTupletsYFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustTupletsYFunctor(Doc *doc);
    virtual ~AdjustTupletsYFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitTuplet(Tuplet *tuplet) override;
    ///@}

protected:
    //
private:
    /**
     * Adjust tuplet relative positioning based on possible overlaps
     */
    ///@{
    void AdjustTupletBracketY(Tuplet *tuplet, const Staff *staff) const;
    void AdjustTupletNumY(Tuplet *tuplet, const Staff *staff) const;
    ///@}

    /**
     * Adjust tuplet relative positioning based on beams
     */
    void AdjustTupletBracketBeamY(Tuplet *tuplet, TupletBracket *bracket, const Beam *beam, const Staff *staff) const;

public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_ADJUSTTUPLETSYFUNCTOR_H__
