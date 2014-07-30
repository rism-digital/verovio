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

#ifndef __VRV_ATTS_EDITTRANS_H__
#define __VRV_ATTS_EDITTRANS_H__

#include "vrvdef.h"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttAgentident
//----------------------------------------------------------------------------

class AttAgentident 
{
public:
    AttAgentident();
    virtual ~AttAgentident();
    
    /** Reset the default values for the attribute class **/
    void ResetAgentident();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetAgent(std::string agent_) { m_agent = agent_; };
    std::string GetAgent() { return m_agent; };
    ///@}

protected:
    /**
     * Signifies the causative agent of damage, illegibility, or other loss of original
     * text.
     **/
    std::string m_agent;

/* include <attagent> */
};

//----------------------------------------------------------------------------
// AttEdit
//----------------------------------------------------------------------------

class AttEdit 
{
public:
    AttEdit();
    virtual ~AttEdit();
    
    /** Reset the default values for the attribute class **/
    void ResetEdit();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetCert(std::string cert_) { m_cert = cert_; };
    std::string GetCert() { return m_cert; };
    //
    void SetEvidence(std::string evidence_) { m_evidence = evidence_; };
    std::string GetEvidence() { return m_evidence; };
    ///@}

protected:
    /** Signifies the degree of certainty or precision associated with a feature. **/
    std::string m_cert;
    /**
     * Indicates the nature of the evidence supporting the reliability or accuracy of
     * the intervention or interpretation.
     * Suggested values include: 'internal', 'external', 'conjecture'.
     **/
    std::string m_evidence;

/* include <attevidence> */
};

//----------------------------------------------------------------------------
// AttExtent
//----------------------------------------------------------------------------

class AttExtent 
{
public:
    AttExtent();
    virtual ~AttExtent();
    
    /** Reset the default values for the attribute class **/
    void ResetExtent();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetExtent(std::string extent_) { m_extent = extent_; };
    std::string GetExtent() { return m_extent; };
    ///@}

protected:
    /** Indicates the extent of damage or omission. **/
    std::string m_extent;

/* include <attextent> */
};

//----------------------------------------------------------------------------
// AttReasonident
//----------------------------------------------------------------------------

class AttReasonident 
{
public:
    AttReasonident();
    virtual ~AttReasonident();
    
    /** Reset the default values for the attribute class **/
    void ResetReasonident();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetReason(std::string reason_) { m_reason = reason_; };
    std::string GetReason() { return m_reason; };
    ///@}

protected:
    /**
     * Holds a short phrase describing the reason for missing textual material (gap),
     * why material is supplied (supplied), or why transcription is difficult
     * (unclear).
     **/
    std::string m_reason;

/* include <attreason> */
};

} // vrv namespace

#endif  // __VRV_ATTS_EDITTRANS_H__

