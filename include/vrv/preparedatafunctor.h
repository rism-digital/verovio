/////////////////////////////////////////////////////////////////////////////
// Name:        preparedatafunctor.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_PREPAREDATAFUNCTOR_H__
#define __VRV_PREPAREDATAFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// PrepareDataInitializationFunctor
//----------------------------------------------------------------------------

/**
 * This class is responsible for one time member initialization at the very begin.
 */
class PrepareDataInitializationFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareDataInitializationFunctor(Doc *doc);
    virtual ~PrepareDataInitializationFunctor() = default;
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
    FunctorCode VisitFloatingObject(FloatingObject *floatingObject) override;
    FunctorCode VisitKeySig(KeySig *keySig) override;
    FunctorCode VisitRunningElement(RunningElement *runningElement) override;
    FunctorCode VisitScore(Score *score) override;
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
// PrepareCueSizeFunctor
//----------------------------------------------------------------------------

/**
 * This class sets the drawing cue size for all layer elements.
 */
class PrepareCueSizeFunctor : public MutableFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    PrepareCueSizeFunctor();
    virtual ~PrepareCueSizeFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
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

#endif // __VRV_PREPAREDATAFUNCTOR_H__
