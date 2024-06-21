/////////////////////////////////////////////////////////////////////////////
// Name:        adjustneumexfunctor.h
// Author:      Laurent Pugin
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTNEUMEXFUNCTOR_H__
#define __VRV_ADJUSTNEUMEXFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustNeumeXFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the X position of accidentals.
 */
class AdjustNeumeXFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustNeumeXFunctor(Doc *doc);
    virtual ~AdjustNeumeXFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitNeume(Neume *neume) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_ADJUSTNEUMEXFUNCTOR_H__
