/////////////////////////////////////////////////////////////////////////////
// Name:        attributes.h
// Author:      Laurent Pugin
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

#ifndef __VRV_ANALYSISMIXIN_H__
#define __VRV_ANALYSISMIXIN_H__

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttCommonAnl
//----------------------------------------------------------------------------

class AttCommonAnl 
{
public:
    AttCommonAnl();
    virtual ~AttCommonAnl();
    
    /** Reset the default values for the attribute class **/
    void ResetCommonAnl();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetCopyof(std::string copyof_) { m_copyof = copyof_; };
    std::string GetCopyof() { return m_copyof; };
    //
    void SetCorresp(std::string corresp_) { m_corresp = corresp_; };
    std::string GetCorresp() { return m_corresp; };
    //
    void SetNext(std::string next_) { m_next = next_; };
    std::string GetNext() { return m_next; };
    //
    void SetPrev(std::string prev_) { m_prev = prev_; };
    std::string GetPrev() { return m_prev; };
    //
    void SetSameas(std::string sameas_) { m_sameas = sameas_; };
    std::string GetSameas() { return m_sameas; };
    //
    void SetSynch(std::string synch_) { m_synch = synch_; };
    std::string GetSynch() { return m_synch; };
    ///@}

protected:
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

class AttHarmonicfunction 
{
public:
    AttHarmonicfunction();
    virtual ~AttHarmonicfunction();
    
    /** Reset the default values for the attribute class **/
    void ResetHarmonicfunction();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetHfunc(std::string hfunc_) { m_hfunc = hfunc_; };
    std::string GetHfunc() { return m_hfunc; };
    ///@}

protected:
    /** Describes harmonic function in any convenient typology. **/
    std::string m_hfunc;

/* include <atthfunc> */
};

//----------------------------------------------------------------------------
// AttIntervalharmonic
//----------------------------------------------------------------------------

class AttIntervalharmonic 
{
public:
    AttIntervalharmonic();
    virtual ~AttIntervalharmonic();
    
    /** Reset the default values for the attribute class **/
    void ResetIntervalharmonic();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetInth(std::string inth_) { m_inth = inth_; };
    std::string GetInth() { return m_inth; };
    ///@}

protected:
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

class AttIntervallicdesc 
{
public:
    AttIntervallicdesc();
    virtual ~AttIntervallicdesc();
    
    /** Reset the default values for the attribute class **/
    void ResetIntervallicdesc();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetIntm(std::string intm_) { m_intm = intm_; };
    std::string GetIntm() { return m_intm; };
    ///@}

protected:
    /**
     * Encodes the melodic interval from the previous pitch.
     * The value may be a general directional indication (u, d, s) or a precise numeric
     * value in half steps.
     **/
    std::string m_intm;

/* include <attintm> */
};

//----------------------------------------------------------------------------
// AttMelodicfunction
//----------------------------------------------------------------------------

class AttMelodicfunction 
{
public:
    AttMelodicfunction();
    virtual ~AttMelodicfunction();
    
    /** Reset the default values for the attribute class **/
    void ResetMelodicfunction();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMfunc(std::string mfunc_) { m_mfunc = mfunc_; };
    std::string GetMfunc() { return m_mfunc; };
    ///@}

protected:
    /** Describes melodic function in any convenient typology. **/
    std::string m_mfunc;

/* include <attmfunc> */
};

//----------------------------------------------------------------------------
// AttPitchclass
//----------------------------------------------------------------------------

class AttPitchclass 
{
public:
    AttPitchclass();
    virtual ~AttPitchclass();
    
    /** Reset the default values for the attribute class **/
    void ResetPitchclass();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetPclass(std::string pclass_) { m_pclass = pclass_; };
    std::string GetPclass() { return m_pclass; };
    ///@}

protected:
    /** Holds pitch class information. **/
    std::string m_pclass;

/* include <attpclass> */
};

//----------------------------------------------------------------------------
// AttSolfa
//----------------------------------------------------------------------------

class AttSolfa 
{
public:
    AttSolfa();
    virtual ~AttSolfa();
    
    /** Reset the default values for the attribute class **/
    void ResetSolfa();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetPsolfa(std::string psolfa_) { m_psolfa = psolfa_; };
    std::string GetPsolfa() { return m_psolfa; };
    ///@}

protected:
    /**
     * Contains sol-fa designation, e.g., do, re, mi, etc., in either a fixed or
     * movable Do system.
     **/
    std::string m_psolfa;

/* include <attpsolfa> */
};

} // vrv namespace

#endif  // __VRV_ANALYSISMIXIN_H__

