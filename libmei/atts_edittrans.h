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

#include "att.h"
#include "att_classes.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttAgentident
//----------------------------------------------------------------------------

class AttAgentident: public Att
{
public:
    AttAgentident();
    virtual ~AttAgentident();
    
    /** Reset the default values for the attribute class **/
    void ResetAgentident();
    
    /** Read the values for the attribute class **/
    bool ReadAgentident( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteAgentident( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetAgent(std::string agent_) { m_agent = agent_; };
    std::string GetAgent() const { return m_agent; };    
    bool HasAgent( );
    
    ///@}

private:
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

class AttEdit: public Att
{
public:
    AttEdit();
    virtual ~AttEdit();
    
    /** Reset the default values for the attribute class **/
    void ResetEdit();
    
    /** Read the values for the attribute class **/
    bool ReadEdit( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteEdit( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetCert(std::string cert_) { m_cert = cert_; };
    std::string GetCert() const { return m_cert; };    
    bool HasCert( );
    
    //
    void SetEvidence(std::string evidence_) { m_evidence = evidence_; };
    std::string GetEvidence() const { return m_evidence; };    
    bool HasEvidence( );
    
    ///@}

private:
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

class AttExtent: public Att
{
public:
    AttExtent();
    virtual ~AttExtent();
    
    /** Reset the default values for the attribute class **/
    void ResetExtent();
    
    /** Read the values for the attribute class **/
    bool ReadExtent( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteExtent( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetExtent(std::string extent_) { m_extent = extent_; };
    std::string GetExtent() const { return m_extent; };    
    bool HasExtent( );
    
    ///@}

private:
    /** Indicates the extent of damage or omission. **/
    std::string m_extent;

/* include <attextent> */
};

//----------------------------------------------------------------------------
// AttReasonident
//----------------------------------------------------------------------------

class AttReasonident: public Att
{
public:
    AttReasonident();
    virtual ~AttReasonident();
    
    /** Reset the default values for the attribute class **/
    void ResetReasonident();
    
    /** Read the values for the attribute class **/
    bool ReadReasonident( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteReasonident( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetReason(std::string reason_) { m_reason = reason_; };
    std::string GetReason() const { return m_reason; };    
    bool HasReason( );
    
    ///@}

private:
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

