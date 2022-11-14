/////////////////////////////////////////////////////////////////////////////
// Name:        calcdotsfunctor.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCDOTSFUNCTOR_H__
#define __VRV_CALCDOTSFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcDotsFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the drawing dot positions, including for chords.
 */
class CalcDotsFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcDotsFunctor(Doc *doc);
    virtual ~CalcDotsFunctor() = default;
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
    FunctorCode VisitRest(Rest *rest) override;
    ///@}

protected:
    //
private:
    /**
     * Return whether dots are overlapping with flag. Take into account flag height, its position as well
     * as position of the note and position of the dots
     */
    bool IsDotOverlappingWithFlag(const Note *note, const int staffSize, int dotLocShift) const;

public:
    //
private:
    // The chord dots object when processing chord notes
    Dots *m_chordDots;
    // The chord drawing X position
    int m_chordDrawingX;
    // The chord stem direction
    data_STEMDIRECTION m_chordStemDir;
};

} // namespace vrv

#endif // __VRV_CALCDOTSFUNCTOR_H__
