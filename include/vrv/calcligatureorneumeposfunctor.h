/////////////////////////////////////////////////////////////////////////////
// Name:        calcligatureorneumeposfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCLIGATUREORNEUMEPOSFUNCTOR_H__
#define __VRV_CALCLIGATUREORNEUMEPOSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcLigatureOrNeumePosFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the note position for each note in ligature.
 */
class CalcLigatureOrNeumePosFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcLigatureOrNeumePosFunctor(Doc *doc);
    virtual ~CalcLigatureOrNeumePosFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLigature(Ligature *ligature) override;
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

#endif // __VRV_CALCLIGATUREORNEUMEPOSFUNCTOR_H__
