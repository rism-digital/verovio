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

#ifndef __VRV_ATTS_GENETIC_H__
#define __VRV_ATTS_GENETIC_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttGeneticState
//----------------------------------------------------------------------------

class AttGeneticState : public Att {
public:
    AttGeneticState();
    virtual ~AttGeneticState();

    /** Reset the default values for the attribute class **/
    void ResetGeneticState();

    /** Read the values for the attribute class **/
    bool ReadGeneticState(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteGeneticState(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetInstant(std::string instant_) { m_instant = instant_; }
    std::string GetInstant() const { return m_instant; }
    bool HasInstant() const;
    //
    void SetState(std::string state_) { m_state = state_; }
    std::string GetState() const { return m_state; }
    bool HasState() const;
    ///@}

private:
    /**
     * The @instant attribute is syntactic sugar for classifying a scribal intervention
     * as an ad-hoc modification; that is, one which does not interrupt the writing
     * process.
     **/
    std::string m_instant;
    /** Points to the genetic state that results from this modification. **/
    std::string m_state;

    /* include <attstate> */
};

} // vrv namespace

#endif // __VRV_ATTS_GENETIC_H__
