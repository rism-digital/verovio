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

#ifndef __VRV_ATTS_HARMONY_H__
#define __VRV_ATTS_HARMONY_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttHarmLog
//----------------------------------------------------------------------------

class AttHarmLog : public Att {
public:
    AttHarmLog();
    virtual ~AttHarmLog();

    /** Reset the default values for the attribute class **/
    void ResetHarmLog();

    /** Read the values for the attribute class **/
    bool ReadHarmLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHarmLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetChordref(std::string chordref_) { m_chordref = chordref_; }
    std::string GetChordref() const { return m_chordref; }
    bool HasChordref() const;
    ///@}

private:
    /** Contains a reference to a **/
    std::string m_chordref;

    /* include <attchordref> */
};

} // vrv namespace

#endif // __VRV_ATTS_HARMONY_H__
