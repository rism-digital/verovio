/////////////////////////////////////////////////////////////////////////////
// Name:        setscoredeffunctor.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_SETSCOREDEFFUNCTOR_H__
#define __VRV_SETSCOREDEFFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// ReplaceDrawingValuesInStaffDefFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the current drawing clef, key signature, etc. to the StaffDef
 */
class ReplaceDrawingValuesInStaffDefFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ReplaceDrawingValuesInStaffDefFunctor(const Clef *clef, const KeySig *keySig, const Mensur *mensur,
        const MeterSig *meterSig, const MeterSigGrp *meterSigGrp);
    virtual ~ReplaceDrawingValuesInStaffDefFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitStaffDef(StaffDef *staffDef) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The clef (NULL if none)
    const Clef *m_clef;
    // The key signature (NULL if none)
    const KeySig *m_keySig;
    // The mensur (NULL if none)
    const Mensur *m_mensur;
    // The meter signature (NULL if none)
    const MeterSig *m_meterSig;
    // The meter signature group (NULL if none)
    const MeterSigGrp *m_meterSigGrp;
};

//----------------------------------------------------------------------------
// ScoreDefSetCurrentPageFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the Page::m_score and Page::m_scoreEnd pointers
 */
class ScoreDefSetCurrentPageFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ScoreDefSetCurrentPageFunctor(Doc *doc);
    virtual ~ScoreDefSetCurrentPageFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitPageEnd(Page *page) override;
    FunctorCode VisitScore(Score *score) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The list of all scores
    std::list<Score *> m_scores;
};

//----------------------------------------------------------------------------
// ScoreDefSetCurrentFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the current scoreDef wherever needed.
 * This includes a scoreDef for each system. It also includes a scoreDef for each measure where a change occured before.
 * A change can be either a scoreDef before or a clef, meterSig, etc. within the previous measure.
 */
class ScoreDefSetCurrentFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ScoreDefSetCurrentFunctor(Doc *doc);
    virtual ~ScoreDefSetCurrentFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitClef(Clef *clef) override;
    FunctorCode VisitKeySig(KeySig *keySig) override;
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitMensur(Mensur *mensur) override;
    FunctorCode VisitPage(Page *page) override;
    FunctorCode VisitProport(Proport *proport) override;
    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitScoreDef(ScoreDef *scoreDef) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitStaffDef(StaffDef *staffDef) override;
    FunctorCode VisitStaffGrp(StaffGrp *staffGrp) override;
    FunctorCode VisitSystem(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current score
    Score *m_currentScore;
    // The current scoreDef
    ScoreDef *m_currentScoreDef;
    // The current staffDef
    StaffDef *m_currentStaffDef;
    // The upcoming scoreDef
    ScoreDef m_upcomingScoreDef;
    // The previous measure (for setting cautionary scoreDef)
    Measure *m_previousMeasure;
    // The current system (for setting the system scoreDef)
    System *m_currentSystem;
    // Flag indicating whether full labels have to be drawn
    bool m_drawLabels;
    // Flag indicating that the scoreDef restarts (draw brace and label)
    bool m_restart;
    // Flag indicating if we already have a measure in the system
    bool m_hasMeasure;
};

//----------------------------------------------------------------------------
// ScoreDefOptimizeFunctor
//----------------------------------------------------------------------------

/**
 * This class optimizes the scoreDef for each system.
 * For automatic breaks, it looks for staves with only mRests.
 */
class ScoreDefOptimizeFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ScoreDefOptimizeFunctor(Doc *doc);
    virtual ~ScoreDefOptimizeFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitScore(Score *score) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitStaffGrpEnd(StaffGrp *staffGrp) override;
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitSystemEnd(System *system) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current scoreDef
    ScoreDef *m_currentScoreDef;
    // Flag indicating if we are optimizing encoded layout
    bool m_encoded;
    // Flag indicating if we consider the first scoreDef
    bool m_firstScoreDef;
    // Flag indicating if a Fermata element is present
    bool m_hasFermata;
    // Flag indicating if a Tempo element is present
    bool m_hasTempo;
};

//----------------------------------------------------------------------------
// SetCautionaryScoreDefFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the cautionary scoreDef wherever needed.
 */
class SetCautionaryScoreDefFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    SetCautionaryScoreDefFunctor(ScoreDef *currentScoreDef);
    virtual ~SetCautionaryScoreDefFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitStaff(Staff *staff) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The current scoreDef
    ScoreDef *m_currentScoreDef;
    // The current staffDef
    StaffDef *m_currentStaffDef;
};

//----------------------------------------------------------------------------
// ScoreDefSetGrpSymFunctor
//----------------------------------------------------------------------------

/**
 * This class prepares the group symbol starting and ending staffDefs for drawing.
 */
class ScoreDefSetGrpSymFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ScoreDefSetGrpSymFunctor();
    virtual ~ScoreDefSetGrpSymFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitGrpSym(GrpSym *grpSym) override;
    FunctorCode VisitSystem(System *system) override;
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
// ScoreDefUnsetCurrentFunctor
//----------------------------------------------------------------------------

/**
 * This class unsets the initial scoreDef for each system and measure.
 */
class ScoreDefUnsetCurrentFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    ScoreDefUnsetCurrentFunctor();
    virtual ~ScoreDefUnsetCurrentFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAlignmentReference(AlignmentReference *alignmentReference) override;
    FunctorCode VisitKeySig(KeySig *keySig) override;
    FunctorCode VisitLayer(Layer *layer) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    FunctorCode VisitPage(Page *page) override;
    FunctorCode VisitStaff(Staff *staff) override;
    FunctorCode VisitSystem(System *system) override;
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
// SetStaffDefRedrawFlagsFunctor
//----------------------------------------------------------------------------

enum StaffDefRedrawFlags {
    REDRAW_CLEF = 0x1,
    REDRAW_KEYSIG = 0x2,
    REDRAW_MENSUR = 0x4,
    REDRAW_METERSIG = 0x8,
    REDRAW_METERSIGGRP = 0x10,
    // all flags
    REDRAW_ALL = REDRAW_CLEF | REDRAW_KEYSIG | REDRAW_MENSUR | REDRAW_METERSIG | REDRAW_METERSIGGRP,
    //
    FORCE_REDRAW = 0x100
};

/**
 * This class sets drawing flags for the StaffDef for indicating whether clefs, keysigs, etc. need
 * to be redrawn. This typically occurs when a new System or a new ScoreDef is encountered.
 */
class SetStaffDefRedrawFlagsFunctor : public Functor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    SetStaffDefRedrawFlagsFunctor(int redrawFlags);
    virtual ~SetStaffDefRedrawFlagsFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitStaffDef(StaffDef *staffDef) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The staffDef redraw flags
    int m_redrawFlags;
};

} // namespace vrv

#endif // __VRV_SETSCOREDEFFUNCTOR_H__
