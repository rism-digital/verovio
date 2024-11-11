/////////////////////////////////////////////////////////////////////////////
// Name:        calcledgerlinesfunctor.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCLEDGERLINESFUNCTOR_H__
#define __VRV_CALCLEDGERLINESFUNCTOR_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcLedgerLinesFunctor
//----------------------------------------------------------------------------

/**
 * This class calculates the ledger lines.
 */
class CalcLedgerLinesFunctor : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcLedgerLinesFunctor(Doc *doc);
    virtual ~CalcLedgerLinesFunctor() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() const override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitAccid(Accid *accid) override;
    FunctorCode VisitNote(Note *note) override;
    FunctorCode VisitStaffEnd(Staff *staff) override;
    ///@}

protected:
    //
private:
    /**
     *
     */
    void CalcForLayerElement(LayerElement *layerElement, int width, data_HORIZONTALALIGNMENT alignment);
    /**
     * Shorten ledger lines which overlap with neighbors
     */
    void AdjustLedgerLines(
        ArrayOfLedgerLines &lines, ArrayOfLedgerLines &cueLines, double cueScaling, int extension, int minExtension);

public:
    //
private:
    //
};

} // namespace vrv

#endif // __VRV_CALCLEDGERLINESFUNCTOR_H__
