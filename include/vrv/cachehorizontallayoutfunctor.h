/////////////////////////////////////////////////////////////////////////////
// Name:        cachehorizontallayoutfunctor.h
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CACHEHORIZONTALLAYOUTFUNCTOR_H__
#define __VRV_CACHEHORIZONTALLAYOUTFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CacheHorizontalLayoutFunctor
//----------------------------------------------------------------------------

/**
 * This class caches or restores cached horizontal layout for faster layout redoing.
 */
class CacheHorizontalLayoutFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CacheHorizontalLayoutFunctor(Doc *doc);
    virtual ~CacheHorizontalLayoutFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return false; }

    /*
     * Set the restore flag
     */
    void SetRestore(bool restore) { m_restore = restore; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitArpeg(Arpeg *arpeg) override;
    FunctorCode VisitLayerElement(LayerElement *layerElement) override;
    FunctorCode VisitMeasure(Measure *measure) override;
    ///@}

protected:
    //
private:
    //
public:
    //
private:
    // Indicates if the cache should be stored (default) or restored
    bool m_restore;
};

} // namespace vrv

#endif // __VRV_CACHEHORIZONTALLAYOUTFUNCTOR_H__
