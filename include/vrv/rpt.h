/////////////////////////////////////////////////////////////////////////////
// Name:        rpt.h
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_RPT_H__
#define __VRV_RPT_H__

#include "atts_cmn.h"
#include "layerelement.h"

namespace vrv {
    
//----------------------------------------------------------------------------
// BeatRpt
//----------------------------------------------------------------------------

/**
 * This class models the MEI <beatRpt>
 */
class BeatRpt: public LayerElement,
    public AttBeatRptVis
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    BeatRpt( );
    virtual ~BeatRpt();
    virtual void Reset();
    virtual std::string GetClassName( ){ return "BeatRpt"; }; ;
    virtual ClassId Is() { return BEAT_RPT; };
    ///@}
    
private:
    
public:
    
private:
    
};
    
//----------------------------------------------------------------------------
// MRpt
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <mRpt>
 */
class MRpt: public LayerElement
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    MRpt( );
    virtual ~MRpt();
    virtual void Reset();
    virtual std::string GetClassName( ){ return "MRpt"; }; ;
    virtual ClassId Is() { return MRPT; };
    ///@}
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * Functor for setting mRpt drawing numbers (if required)
     * See implementation and Object::PrepareRpt
     */
    virtual int PrepareRpt( ArrayPtrVoid *params );

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
class MRpt2: public LayerElement
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    MRpt2( );
    virtual ~MRpt2();
    virtual void Reset();
    virtual std::string GetClassName( ){ return "MRpt2"; }; ;
    virtual ClassId Is() { return MRPT2; };
    ///@}
    
private:
    
public:
    
private:
    
};


//----------------------------------------------------------------------------
// MultiRpt
//----------------------------------------------------------------------------

/**
 * This class models the MEI <multiRpt>
 */
class MultiRpt: public LayerElement
{
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method reset all attribute classes
     */
    ///@{
    MultiRpt( );
    virtual ~MultiRpt();
    virtual void Reset();
    virtual std::string GetClassName( ){ return "MultiRpt"; }; ;
    virtual ClassId Is() { return MULTI_RPT; };
    ///@}
    
private:
    
public:
    
private:
    
};

} // namespace vrv    

#endif
