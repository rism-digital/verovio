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
// BeatRpt
//----------------------------------------------------------------------------

/**
 * This class models the MEI <beatRpt>
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
// BTrem
//----------------------------------------------------------------------------

/**
 * This class models the MEI <bTrem>
 */
class BTrem : public LayerElement, public AttTremmeasured {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    BTrem();
    virtual ~BTrem();
    virtual void Reset();
    virtual std::string GetClassName() const { return "BTrem"; }
    virtual ClassId GetClassId() const { return BTREM; }
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
class FTrem : public LayerElement, public ObjectListInterface, public AttSlashcount, public AttTremmeasured {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    FTrem();
    virtual ~FTrem();
    virtual void Reset();
    virtual std::string GetClassName() const { return "FTrem"; }
    virtual ClassId GetClassId() const { return FTREM; }
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    virtual void AddChild(Object *object);

    /**
     * Initializes the m_beamElementCoords vector objects.
     * This is called by FTrem::FilterList
     */
    void InitCoords(ListOfObjects *childList);

    /**
     * Clear the m_beamElementCoords vector and delete all the objects.
     */
    void ClearCoords();

    /**
     *
     */
    const ArrayOfBeamElementCoords *GetElementCoords() const { return &m_beamElementCoords; }

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::CalcStem
     */
    virtual int CalcStem(FunctorParams *functorParams);

private:
    //
protected:
    /**
     * Filter the list for a specific fTrem;
     */
    virtual void FilterList(ListOfObjects *childList);

public:
    /** */
    BeamDrawingParams m_drawingParams;

private:
    /**
     * An array of coordinates for each element
     **/
    mutable ArrayOfBeamElementCoords m_beamElementCoords;
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
