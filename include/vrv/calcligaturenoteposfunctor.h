/////////////////////////////////////////////////////////////////////////////
// Name:        calcligaturenoteposfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCLIGATURENOTEPOSFUNCTOR_H__
#define __VRV_CALCLIGATURENOTEPOSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcLigatureNotePosFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the note position for each note in ligature.
 */
class CalcLigatureNotePosFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcLigatureNotePosFunctor(Doc *doc);
    virtual ~CalcLigatureNotePosFunctor() = default;
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

#endif // __VRV_CALCLIGATURENOTEPOSFUNCTOR_H__
