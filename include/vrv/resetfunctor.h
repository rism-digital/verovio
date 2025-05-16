/////////////////////////////////////////////////////////////////////////////
// Name:        resetfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RESETFUNCTOR_H__
#define __VRV_RESETFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// ResetDataFunctor
//----------------------------------------------------------------------------

/**
 * This class resets the drawing values before calling PrepareData after changes.
 */
class ResetDataFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ResetDataFunctor();
    virtual ~ResetDataFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAccid(Accid *accid) override;
    FunctorCode VisitArpeg(Arpeg *arpeg) override;
    FunctorCode VisitArtic(Artic *artic) override;
    FunctorCode VisitBeam(Beam *beam) override;
    FunctorCode VisitBeamSpan(BeamSpan *beamSpan) override;
    FunctorCode VisitChord(Chord *chord) override;
    FunctorCode VisitControlElement(ControlElement *controlElement) override;
    FunctorCode VisitCustos(Custos *custos) override;
    FunctorCode VisitDiv(Div *div) override;
    FunctorCode VisitDot(Dot *dot) override;
    FunctorCode VisitDots(Dots *dots) override;
    FunctorCode VisitEditorialElement(EditorialElement *editorialElement) override;
    FunctorCode VisitEnding(Ending *ending) override;
    FunctorCode VisitF(F *f) override;
    FunctorCode VisitFlag(Flag *flag) override;
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitFTrem(FTrem *fTrem) override;
    FunctorCode VisitHairpin(Hairpin *hairpin) override;
    FunctorCode VisitKeySig(KeySig *keySig) override;
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitLigature(Ligature *ligature) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMRest(MRest *mRest) override;
    FunctorCode VisitNote(Note *note) override;
    FunctorCode VisitNc(Nc *nc) override;
    FunctorCode VisitObject(Object *object) override;
    FunctorCode VisitPage(Page *page) override;
    FunctorCode VisitRepeatMark(RepeatMark *repeatMark) override;
    FunctorCode VisitRest(Rest *rest) override;
    FunctorCode VisitSection(Section *section) override;
    FunctorCode VisitSlur(Slur *slur) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitStaffDef(StaffDef *staffDef) override;
    FunctorCode VisitStem(Stem *stem) override;
    FunctorCode VisitSyl(Syl *syl) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitSystemMilestone(SystemMilestoneEnd *systemMilestoneEnd) override;
    FunctorCode VisitTabDurSym(TabDurSym *tabDurSym) override;
    FunctorCode VisitTempo(Tempo *tempo) override;
    FunctorCode VisitTuplet(Tuplet *tuplet) override;
    FunctorCode VisitTurn(Turn *turn) override;
    FunctorCode VisitVerse(Verse *verse) override;
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

//----------------------------------------------------------------------------
// ResetHorizontalAlignmentFunctor
//----------------------------------------------------------------------------

/**
 * This class resets the horizontal alignment for various types.
 */
class ResetHorizontalAlignmentFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ResetHorizontalAlignmentFunctor();
    virtual ~ResetHorizontalAlignmentFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAccid(Accid *accid) override;
    FunctorCode VisitArpeg(Arpeg *arpeg) override;
    FunctorCode VisitBeam(Beam *beam) override;
    FunctorCode VisitBeamSpan(BeamSpan *beamSpan) override;
    FunctorCode VisitCustos(Custos *custos) override;
    FunctorCode VisitDiv(Div *div) override;
    FunctorCode VisitDot(Dot *dot) override;
    FunctorCode VisitDots(Dots *dots) override;
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMRest(MRest *mRest) override;
    FunctorCode VisitNote(Note *note) override;
    FunctorCode VisitProport(Proport *proport) override;
    FunctorCode VisitRest(Rest *rest) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitTuplet(Tuplet *tuplet) override;
    FunctorCode VisitTupletBracket(TupletBracket *tupletBracket) override;
    FunctorCode VisitTupletNum(TupletNum *tupletNum) override;
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

//----------------------------------------------------------------------------
// ResetVerticalAlignmentFunctor
//----------------------------------------------------------------------------

/**
 * This class resets the vertical alignment for various types.
 */
class ResetVerticalAlignmentFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ResetVerticalAlignmentFunctor();
    virtual ~ResetVerticalAlignmentFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitArtic(Artic *artic) override;
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitOctave(Octave *octave) override;
    FunctorCode VisitPage(Page *page) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitTextElement(TextElement *textElement) override;
    FunctorCode VisitTuplet(Tuplet *tuplet) override;
    FunctorCode VisitTupletBracket(TupletBracket *tupletBracket) override;
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

#endif // __VRV_RESETFUNCTOR_H__
