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
#include "layerelement.h"

namespace vrv {

//----------------------------------------------------------------------------
// BeatRpt
//----------------------------------------------------------------------------

/**
 * This class models the MEI <beatRpt>
 */
class BeatRpt : public LayerElement, public AttBeatRptVis {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    BeatRpt();
    virtual ~BeatRpt();
    virtual void Reset();
    virtual std::string GetClassName() const { return "BeatRpt"; };
    virtual ClassId Is() const { return BEATRPT; };
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; };

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
// BTrem
//----------------------------------------------------------------------------

/**
 * This class models the MEI <bTrem>
 */
class BTrem : public LayerElement {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    BTrem();
    virtual ~BTrem();
    virtual void Reset();
    virtual std::string GetClassName() const { return "BTrem"; };
    virtual ClassId Is() const { return BTREM; }
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    virtual void AddChild(Object *object);

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// FTrem
//----------------------------------------------------------------------------

/**
 * This class models the MEI <fTrem>
 */
class FTrem : public LayerElement, public ObjectListInterface, public AttSlashcount {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    FTrem();
    virtual ~FTrem();
    virtual void Reset();
    virtual std::string GetClassName() const { return "FTrem"; };
    virtual ClassId Is() const { return FTREM; };
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    virtual void AddChild(Object *object);

private:
    //
protected:
    /**
     * Filter the list for a specific fTrem;
     */
    virtual void FilterList(ListOfObjects *childList);

public:
    //
private:
};

//----------------------------------------------------------------------------
// MRpt
//----------------------------------------------------------------------------

/**
 * This class models the MEI <mRpt>
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
    virtual std::string GetClassName() const { return "MRpt"; };
    virtual ClassId Is() const { return MRPT; };
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Functor for setting mRpt drawing numbers (if required)
     * See implementation and Object::PrepareRpt
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
// MRpt2
//----------------------------------------------------------------------------

/**
 * This class models the MEI <mRpt2>
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
    virtual std::string GetClassName() const { return "MRpt2"; };
    virtual ClassId Is() const { return MRPT2; };
    ///@}

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// MultiRpt
//----------------------------------------------------------------------------

/**
 * This class models the MEI <multiRpt>
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
    virtual std::string GetClassName() const { return "MultiRpt"; };
    virtual ClassId Is() const { return MULTIRPT; };
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
