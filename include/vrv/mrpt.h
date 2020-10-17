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
class MRpt : public LayerElement, public AttColor {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MRpt();
    virtual ~MRpt();
    virtual Object *Clone() const { return new MRpt(*this); }
    virtual void Reset();
    virtual std::string GetClassName() const { return "MRpt"; }
    virtual ClassId GetClassId() const { return MRPT; }
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * @name See Object::GenerateMIDI
     */
    ///@{
    virtual int GenerateMIDI(FunctorParams *functorParams);
    ///@}

    /**
     * See Object::PrepareRpt
     */
    virtual int PrepareRpt(FunctorParams *functorParams);

private:
    //
public:
    /** measure count */
    int m_drawingMeasureCount;

private:
};

} // namespace vrv

#endif
