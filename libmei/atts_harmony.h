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
// AttFretlocation
//----------------------------------------------------------------------------

class AttFretlocation : public Att {
public:
    AttFretlocation();
    virtual ~AttFretlocation();

    /** Reset the default values for the attribute class **/
    void ResetFretlocation();

    /** Read the values for the attribute class **/
    bool ReadFretlocation(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteFretlocation(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFret(data_FRET fret_) { m_fret = fret_; };
    data_FRET GetFret() const { return m_fret; };
    bool HasFret();
    ///@}

private:
    /** Records the location at which a string should be stopped against a fret. **/
    data_FRET m_fret;

    /* include <attfret> */
};

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
    void SetChordref(std::string chordref_) { m_chordref = chordref_; };
    std::string GetChordref() const { return m_chordref; };
    bool HasChordref();
    ///@}

private:
    /** Contains a reference to a <chordDef> element elsewhere in the document. **/
    std::string m_chordref;

    /* include <attchordref> */
};

//----------------------------------------------------------------------------
// AttHarmVis
//----------------------------------------------------------------------------

class AttHarmVis : public Att {
public:
    AttHarmVis();
    virtual ~AttHarmVis();

    /** Reset the default values for the attribute class **/
    void ResetHarmVis();

    /** Read the values for the attribute class **/
    bool ReadHarmVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHarmVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetRendgrid(harmVis_RENDGRID rendgrid_) { m_rendgrid = rendgrid_; };
    harmVis_RENDGRID GetRendgrid() const { return m_rendgrid; };
    bool HasRendgrid();
    ///@}

private:
    /** Describes how the harmonic indication should be rendered. **/
    harmVis_RENDGRID m_rendgrid;

    /* include <attrendgrid> */
};

} // vrv namespace

#endif // __VRV_ATTS_HARMONY_H__
