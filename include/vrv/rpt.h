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
     * Reset method reset all attribute classes
     */
    ///@{
    BeatRpt();
    virtual ~BeatRpt();
    virtual void Reset();
    virtual std::string GetClassName() { return "BeatRpt"; };
    ;
    virtual ClassId Is() { return BEATRPT; };
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() { return true; };

    /**
     * Returns the duration (in double) for the element.
     * Careful: this method is not overriding LayerElement::GetAlignmentDuration since
     * LayerElement and DurationInterface have no inheritance link.
     */
    virtual double GetAlignmentDuration(int meterUnit);

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
     * Reset method reset all attribute classes
     */
    ///@{
    BTrem();
    virtual ~BTrem();
    virtual void Reset();
    virtual std::string GetClassName() { return "BTrem"; };
    ;
    virtual ClassId Is() { return BTREM; };
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    void AddLayerElement(LayerElement *element);

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
     * Reset method reset all attribute classes
     */
    ///@{
    FTrem();
    virtual ~FTrem();
    virtual void Reset();
    virtual std::string GetClassName() { return "FTrem"; };
    ;
    virtual ClassId Is() { return FTREM; };
    ///@}

    /**
     * Add an element (a note or a chord) to a fTrem.
     * Only Note or Chord elements will be actually added to the fTrem.
     */
    void AddLayerElement(LayerElement *element);

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
     * Reset method reset all attribute classes
     */
    ///@{
    MRpt();
    virtual ~MRpt();
    virtual void Reset();
    virtual std::string GetClassName() { return "MRpt"; };
    ;
    virtual ClassId Is() { return MRPT; };
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * Functor for setting mRpt drawing numbers (if required)
     * See implementation and Object::PrepareRpt
     */
    virtual int PrepareRpt(ArrayPtrVoid *params);

private:
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
     * Reset method reset all attribute classes
     */
    ///@{
    MRpt2();
    virtual ~MRpt2();
    virtual void Reset();
    virtual std::string GetClassName() { return "MRpt2"; };
    ;
    virtual ClassId Is() { return MRPT2; };
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
     * Reset method reset all attribute classes
     */
    ///@{
    MultiRpt();
    virtual ~MultiRpt();
    virtual void Reset();
    virtual std::string GetClassName() { return "MultiRpt"; };
    ;
    virtual ClassId Is() { return MULTIRPT; };
    ///@}

private:
    //
public:
    //
private:
};

} // namespace vrv

#endif
