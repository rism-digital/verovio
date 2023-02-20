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

#ifndef __LIBMEI_ATTS_EDITTRANS_H__
#define __LIBMEI_ATTS_EDITTRANS_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttAgentIdent
//----------------------------------------------------------------------------

class AttAgentIdent : public Att {
protected:
    AttAgentIdent();
    ~AttAgentIdent() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetAgentIdent();

    /** Read the values for the attribute class **/
    bool ReadAgentIdent(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteAgentIdent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAgent(std::string agent_) { m_agent = agent_; }
    std::string GetAgent() const { return m_agent; }
    bool HasAgent() const;
    ///@}

private:
    /**
     * Signifies the causative agent of damage, illegibility, or other loss of original
     * text.
     **/
    std::string m_agent;
};

//----------------------------------------------------------------------------
// InstAgentIdent
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttAgentIdent
 */

class InstAgentIdent : public AttAgentIdent {
public:
    InstAgentIdent() = default;
    virtual ~InstAgentIdent() = default;
};

//----------------------------------------------------------------------------
// AttReasonIdent
//----------------------------------------------------------------------------

class AttReasonIdent : public Att {
protected:
    AttReasonIdent();
    ~AttReasonIdent() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetReasonIdent();

    /** Read the values for the attribute class **/
    bool ReadReasonIdent(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteReasonIdent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetReason(std::string reason_) { m_reason = reason_; }
    std::string GetReason() const { return m_reason; }
    bool HasReason() const;
    ///@}

private:
    /**
     * Holds a short phrase describing the reason for missing textual material (gap),
     * why material is supplied (supplied), or why transcription is difficult
     * (unclear).
     **/
    std::string m_reason;
};

//----------------------------------------------------------------------------
// InstReasonIdent
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttReasonIdent
 */

class InstReasonIdent : public AttReasonIdent {
public:
    InstReasonIdent() = default;
    virtual ~InstReasonIdent() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_EDITTRANS_H__
