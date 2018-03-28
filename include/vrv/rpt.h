/////////////////////////////////////////////////////////////////////////////
// Name:        rpt.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_RPT_H__
#define __VRV_RPT_H__

#include "atts_cmn.h"
#include "atts_shared.h"
#include "beam.h"
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeatRpt (beat repeat)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <beatRpt> element.
 */
class BeatRpt : public LayerElement, public AttColor, public AttBeatRptVis {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    BeatRpt();
    virtual ~BeatRpt();
    virtual void Reset();
    virtual std::string GetClassName() const { return "BeatRpt"; }
    virtual ClassId GetClassId() const { return BEATRPT; }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Returns the duration (in double) for the BeatRpt.
     */

    double GetBeatRptAlignmentDuration(int meterUnit) const;

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// MRpt (measure repeat)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <mRpt> element.
 */
class MRpt : public LayerElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MRpt();
    virtual ~MRpt();
    virtual void Reset();
    virtual std::string GetClassName() const { return "MRpt"; }
    virtual ClassId GetClassId() const { return MRPT; }
    ///@}

    //----------//
    // Functors //
    //----------//

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

//----------------------------------------------------------------------------
// MRpt2 (2-measure repeat)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <mRpt2> element.
 */
class MRpt2 : public LayerElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MRpt2();
    virtual ~MRpt2();
    virtual void Reset();
    virtual std::string GetClassName() const { return "MRpt2"; }
    virtual ClassId GetClassId() const { return MRPT2; }
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// MultiRpt (multiple repeat)
//----------------------------------------------------------------------------

/**
 * This class models the MEI <multiRpt> element.
 */
class MultiRpt : public LayerElement, public AttNumbered {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    MultiRpt();
    virtual ~MultiRpt();
    virtual void Reset();
    virtual std::string GetClassName() const { return "MultiRpt"; }
    virtual ClassId GetClassId() const { return MULTIRPT; }
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
