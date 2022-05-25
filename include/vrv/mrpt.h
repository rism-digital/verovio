/////////////////////////////////////////////////////////////////////////////
// Name:        mrpt.h
// Author:      Klaus Rettinghaus
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_MRPT_H__
#define __VRV_MRPT_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "atts_visual.h"
#include "beam.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// MRpt (measure repeat)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <mRpt> element.
 */
class MRpt : public LayerElement, public AttColor, public AttNumbered, public AttNumberPlacement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MRpt();
    virtual ~MRpt();
    Object *Clone() const override { return new MRpt(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "MRpt"; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * @name See Object::GenerateMIDI
     */
    ///@{
    int GenerateMIDI(FunctorParams *functorParams) override;
    ///@}

    /**
     * See Object::PrepareRpt
     */
    int PrepareRpt(FunctorParams *functorParams) override;

private:
    //
public:
    /** measure count */
    int m_drawingMeasureCount;

private:
};

} // namespace vrv

#endif
