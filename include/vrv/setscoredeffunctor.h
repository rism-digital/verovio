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

} // namespace vrv

#endif // __VRV_SETSCOREDEFFUNCTOR_H__
