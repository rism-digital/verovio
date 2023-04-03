/////////////////////////////////////////////////////////////////////////////
// Name:        adjusttempofunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ADJUSTTEMPOFUNCTOR_H__
#define __VRV_ADJUSTTEMPOFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// AdjustTempoFunctor
//----------------------------------------------------------------------------

/**
 * This class adjusts the X position of tempi.
 */
class AdjustTempoFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    AdjustTempoFunctor(Doc *doc);
    virtual ~AdjustTempoFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitSystem(System *system) override;
    FunctorCode VisitTempo(Tempo *tempo) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // The systemAligner
    SystemAligner *m_systemAligner;
};

} // namespace vrv

#endif // __VRV_ADJUSTTEMPOFUNCTOR_H__
