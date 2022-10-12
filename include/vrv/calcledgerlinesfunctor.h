/////////////////////////////////////////////////////////////////////////////
// Name:        calcledgerlines.h
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_CALCLEDGERLINES_H__
#define __VRV_CALCLEDGERLINES_H__

#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// CalcLedgerLines
//----------------------------------------------------------------------------

/**
 * This class calculates the ledger lines.
 */
class CalcLedgerLines : public DocFunctor {
public:
    /**
     * @name Constructors, destructors
     */
    ///@{
    CalcLedgerLines(Doc *doc);
    virtual ~CalcLedgerLines() = default;
    ///@}

    /*
     * Abstract base implementation
     */
    bool ImplementsEndInterface() override { return true; }

    /*
     * Functor interface
     */
    ///@{
    FunctorCode VisitNote(Note *note) override;
    FunctorCode VisitStaffEnd(Staff *staff) override;
    ///@}

protected:
    //
private:
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

#endif // __VRV_CALCLEDGERLINES_H__
