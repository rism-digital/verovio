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
class ReplaceDrawingValuesInStaffDefFunctor : public MutableFunctor {
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
    FunctorCode VisitObject(Object *object) override;
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

} // namespace vrv

#endif // __VRV_SETSCOREDEFFUNCTOR_H__
