/////////////////////////////////////////////////////////////////////////////
// Authors:     Laurent Pugin and Rodolfo Zitellini
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
//
// Code generated using a modified version of libmei 
// by Andrew Hankinson, Alastair Porter, and Others
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////// 
// NOTE: this file was generated with the Verovio libmei version and 
// should not be edited because changes will be lost.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_ATTS_CRITAPP_H__
#define __VRV_ATTS_CRITAPP_H__

#include "vrvdef.h"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttCrit
//----------------------------------------------------------------------------

class AttCrit 
{
public:
    AttCrit();
    virtual ~AttCrit();
    
    /** Reset the default values for the attribute class **/
    void ResetCrit();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetCause(std::string cause_) { m_cause = cause_; };
    std::string GetCause() { return m_cause; };
    ///@}

protected:
    /**
     * Classifies the cause for the variant reading, according to any appropriate
     * typology of possible origins.
     **/
    std::string m_cause;

/* include <attcause> */
};

//----------------------------------------------------------------------------
// AttSource
//----------------------------------------------------------------------------

class AttSource 
{
public:
    AttSource();
    virtual ~AttSource();
    
    /** Reset the default values for the attribute class **/
    void ResetSource();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetSource(std::string source_) { m_source = source_; };
    std::string GetSource() { return m_source; };
    ///@}

protected:
    /**
     * Contains a list of one or more pointers indicating the sources which attest to a
     * given reading.
     * Each value should correspond to the ID of a <source> element located in the
     * document header.
     **/
    std::string m_source;

/* include <attsource> */
};

} // vrv namespace

#endif  // __VRV_ATTS_CRITAPP_H__

