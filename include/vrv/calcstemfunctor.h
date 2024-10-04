/////////////////////////////////////////////////////////////////////////////
// Name:        calcstemfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCSTEMFUNCTOR_H__
#define __VRV_CALCSTEMFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcStemFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the drawing stem positions, including for beams.
 */
class CalcStemFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcStemFunctor(Doc *doc);
    virtual ~CalcStemFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitBeam(Beam *beam) override;
    FunctorCode VisitBeamSpan(BeamSpan *beamSpan) override;
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitFTrem(FTrem *fTrem) override;
    FunctorCode VisitNote(Note *note) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitStem(Stem *stem) override;
    FunctorCode VisitTabDurSym(TabDurSym *tabDurSym) override;
    FunctorCode VisitTabGrp(TabGrp *tabGrp) override;
    ///@}

protected:
    //
private:
    /**
     * Calculate stem direction based on the position of the notes in chord. Notes are compared in pairs starting from
     * the top-/bottom most and moving inward towards the center of the chord
     */
    data_STEMDIRECTION CalcStemDirection(const Chord *chord, int verticalCenter) const;

    /**
     * Adjusts the flag placement and stem length if they are crossing notehead or ledger lines
     */
    void AdjustFlagPlacement(
        const Doc *doc, Stem *stem, Flag *flag, int staffSize, int verticalCenter, data_DURATION duration) const;

public:
    //
private:
    // The additional length for a chord stem (between the top and bottom note)
    int m_chordStemLength;
    // The vertical center of the staff
    int m_verticalCenter;
    // The actual duration of the chord / note
    data_DURATION m_dur;
    // The flag for grace notes (stem is not extended)
    bool m_isGraceNote;
    // The flag for stem.sameas notes
    bool m_isStemSameasSecondary;
    // The flag indicating that we have no note in tabGrp
    bool m_tabGrpWithNoNote;
    // The current staff (to avoid additional lookup)
    Staff *m_staff;
    // The current layer (ditto)
    Layer *m_layer;
    // The chord or note to which the stem belongs
    StemmedDrawingInterface *m_interface;
};

} // namespace vrv

#endif // __VRV_CALCSTEMFUNCTOR_H__
