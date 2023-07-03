/////////////////////////////////////////////////////////////////////////////
// Name:        calcchordnoteheadsfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCCHORDNOTEHEADSFUNCTOR_H__
#define __VRV_CALCCHORDNOTEHEADSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcChordNoteHeadsFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the note head flipped positions.
 */
class CalcChordNoteHeadsFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcChordNoteHeadsFunctor(Doc *doc);
    virtual ~CalcChordNoteHeadsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitNote(Note *note) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The diameter of the anchoring note of the chord
    int m_diameter;
    // The alignment type
    int m_alignmentType;
};

} // namespace vrv

#endif // __VRV_CALCCHORDNOTEHEADSFUNCTOR_H__
