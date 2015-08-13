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

#ifndef __VRV_ATTS_ANALYSIS_H__
#define __VRV_ATTS_ANALYSIS_H__

#include "att.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttCommonAnl
//----------------------------------------------------------------------------

#define ATT_COMMONANL 1

class AttCommonAnl: public Att
{
public:
    AttCommonAnl();
    virtual ~AttCommonAnl();
    
    /** Reset the default values for the attribute class **/
    void ResetCommonAnl();
    
    /** Read the values for the attribute class **/
    bool ReadCommonAnl( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteCommonAnl( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetCopyof(std::string copyof_) { m_copyof = copyof_; };
    std::string GetCopyof() const { return m_copyof; };    
    bool HasCopyof( );
    
    //
    void SetCorresp(std::string corresp_) { m_corresp = corresp_; };
    std::string GetCorresp() const { return m_corresp; };    
    bool HasCorresp( );
    
    //
    void SetNext(std::string next_) { m_next = next_; };
    std::string GetNext() const { return m_next; };    
    bool HasNext( );
    
    //
    void SetPrev(std::string prev_) { m_prev = prev_; };
    std::string GetPrev() const { return m_prev; };    
    bool HasPrev( );
    
    //
    void SetSameas(std::string sameas_) { m_sameas = sameas_; };
    std::string GetSameas() const { return m_sameas; };    
    bool HasSameas( );
    
    //
    void SetSynch(std::string synch_) { m_synch = synch_; };
    std::string GetSynch() const { return m_synch; };    
    bool HasSynch( );
    
    ///@}

private:
    /** Points to an element of which the current element is a copy. **/
    std::string m_copyof;
    /**
     * Used to point to other elements that correspond to this one in a generic
     * fashion.
     **/
    std::string m_corresp;
    /** Used to point to the next event(s) in a user-defined collection. **/
    std::string m_next;
    /** Points to the previous event(s) in a user-defined collection. **/
    std::string m_prev;
    /**
     * Points to an element that is the same as the current element but is not a
     * literal copy of the current element.
     **/
    std::string m_sameas;
    /** Points to elements that are synchronous with the current element. **/
    std::string m_synch;

/* include <attsynch> */
};

//----------------------------------------------------------------------------
// AttHarmonicfunction
//----------------------------------------------------------------------------

#define ATT_HARMONICFUNCTION 2

class AttHarmonicfunction: public Att
{
public:
    AttHarmonicfunction();
    virtual ~AttHarmonicfunction();
    
    /** Reset the default values for the attribute class **/
    void ResetHarmonicfunction();
    
    /** Read the values for the attribute class **/
    bool ReadHarmonicfunction( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteHarmonicfunction( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetDeg(std::string deg_) { m_deg = deg_; };
    std::string GetDeg() const { return m_deg; };    
    bool HasDeg( );
    
    ///@}

private:
    /**
     * Captures relative scale degree information using Humdrum **deg syntax -- an
     * optional indicator of melodic approach (^ = ascending approach, v = descending
     * approach), a scale degree value (1 = tonic ...
     * 7 = leading tone), and an optional indication of chromatic alteration. The
     * amount of chromatic alternation is not indicated.
     **/
    std::string m_deg;

/* include <attdeg> */
};

//----------------------------------------------------------------------------
// AttIntervalharmonic
//----------------------------------------------------------------------------

#define ATT_INTERVALHARMONIC 3

class AttIntervalharmonic: public Att
{
public:
    AttIntervalharmonic();
    virtual ~AttIntervalharmonic();
    
    /** Reset the default values for the attribute class **/
    void ResetIntervalharmonic();
    
    /** Read the values for the attribute class **/
    bool ReadIntervalharmonic( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteIntervalharmonic( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetInth(std::string inth_) { m_inth = inth_; };
    std::string GetInth() const { return m_inth; };    
    bool HasInth( );
    
    ///@}

private:
    /**
     * Encodes the harmonic interval between this note and other pitches occurring at
     * the same time.
     **/
    std::string m_inth;

/* include <attinth> */
};

//----------------------------------------------------------------------------
// AttIntervallicdesc
//----------------------------------------------------------------------------

#define ATT_INTERVALLICDESC 4

class AttIntervallicdesc: public Att
{
public:
    AttIntervallicdesc();
    virtual ~AttIntervallicdesc();
    
    /** Reset the default values for the attribute class **/
    void ResetIntervallicdesc();
    
    /** Read the values for the attribute class **/
    bool ReadIntervallicdesc( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteIntervallicdesc( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetIntm(std::string intm_) { m_intm = intm_; };
    std::string GetIntm() const { return m_intm; };    
    bool HasIntm( );
    
    ///@}

private:
    /**
     * Encodes the melodic interval from the previous pitch.
     * The value may be a general directional indication (u, d, s), an indication of
     * diatonic interval direction, quality, and size, or a precise numeric value in
     * half steps.
     **/
    std::string m_intm;

/* include <attintm> */
};

//----------------------------------------------------------------------------
// AttMelodicfunction
//----------------------------------------------------------------------------

#define ATT_MELODICFUNCTION 5

class AttMelodicfunction: public Att
{
public:
    AttMelodicfunction();
    virtual ~AttMelodicfunction();
    
    /** Reset the default values for the attribute class **/
    void ResetMelodicfunction();
    
    /** Read the values for the attribute class **/
    bool ReadMelodicfunction( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMelodicfunction( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMfunc(std::string mfunc_) { m_mfunc = mfunc_; };
    std::string GetMfunc() const { return m_mfunc; };    
    bool HasMfunc( );
    
    ///@}

private:
    /** Describes melodic function using Humdrum **embel syntax. **/
    std::string m_mfunc;

/* include <attmfunc> */
};

//----------------------------------------------------------------------------
// AttPitchclass
//----------------------------------------------------------------------------

#define ATT_PITCHCLASS 6

class AttPitchclass: public Att
{
public:
    AttPitchclass();
    virtual ~AttPitchclass();
    
    /** Reset the default values for the attribute class **/
    void ResetPitchclass();
    
    /** Read the values for the attribute class **/
    bool ReadPitchclass( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WritePitchclass( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetPclass(std::string pclass_) { m_pclass = pclass_; };
    std::string GetPclass() const { return m_pclass; };    
    bool HasPclass( );
    
    ///@}

private:
    /** Holds pitch class information. **/
    std::string m_pclass;

/* include <attpclass> */
};

//----------------------------------------------------------------------------
// AttSolfa
//----------------------------------------------------------------------------

#define ATT_SOLFA 7

class AttSolfa: public Att
{
public:
    AttSolfa();
    virtual ~AttSolfa();
    
    /** Reset the default values for the attribute class **/
    void ResetSolfa();
    
    /** Read the values for the attribute class **/
    bool ReadSolfa( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteSolfa( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetPsolfa(std::string psolfa_) { m_psolfa = psolfa_; };
    std::string GetPsolfa() const { return m_psolfa; };    
    bool HasPsolfa( );
    
    ///@}

private:
    /**
     * Contains sol-fa designation, e.g., do, re, mi, etc., in either a fixed or
     * movable Do system.
     **/
    std::string m_psolfa;

/* include <attpsolfa> */
};

} // vrv namespace

#endif  // __VRV_ATTS_ANALYSIS_H__

