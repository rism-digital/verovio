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

#ifndef __VRV_ATTS_LAYOUT_H__
#define __VRV_ATTS_LAYOUT_H__

#include "vrvdef.h"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttBarline
//----------------------------------------------------------------------------

class AttBarline 
{
public:
    AttBarline();
    virtual ~AttBarline();
    
    /** Reset the default values for the attribute class **/
    void ResetBarline();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetBarline(std::string barline_) { m_barline = barline_; };
    std::string GetBarline() { return m_barline; };
    ///@}

protected:
    /**
     * Indicates if the element describes the beginning or end of a measure.
     * If a //mb/@barline="left" is pointing to a measure without a @left, there is no
     * barline drawn by default. If a //mb/@barline="right" is pointing to a measure
     * without a @right, there is a regular barline drawn by default.
     **/
    std::string m_barline;

/* include <attbarline> */
};

//----------------------------------------------------------------------------
// AttIgnore
//----------------------------------------------------------------------------

class AttIgnore 
{
public:
    AttIgnore();
    virtual ~AttIgnore();
    
    /** Reset the default values for the attribute class **/
    void ResetIgnore();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetIgnored(std::string ignored_) { m_ignored = ignored_; };
    std::string GetIgnored() { return m_ignored; };
    ///@}

protected:
    /**
     * Indicates whether the referenced element should be included or omitted from the
     * rendering.
     **/
    std::string m_ignored;

/* include <attignored> */
};

//----------------------------------------------------------------------------
// AttLayout
//----------------------------------------------------------------------------

class AttLayout 
{
public:
    AttLayout();
    virtual ~AttLayout();
    
    /** Reset the default values for the attribute class **/
    void ResetLayout();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetIgnored(std::string ignored_) { m_ignored = ignored_; };
    std::string GetIgnored() { return m_ignored; };
    ///@}

protected:
    /**
     * Indicates whether the referenced element should be included or omitted from the
     * rendering.
     **/
    std::string m_ignored;

/* include <attignored> */
};

//----------------------------------------------------------------------------
// AttMeasureRef
//----------------------------------------------------------------------------

class AttMeasureRef 
{
public:
    AttMeasureRef();
    virtual ~AttMeasureRef();
    
    /** Reset the default values for the attribute class **/
    void ResetMeasureRef();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetMeasureref(std::string measureref_) { m_measureref = measureref_; };
    std::string GetMeasureref() { return m_measureref; };
    ///@}

protected:
    /** Contains a reference to a measure. **/
    std::string m_measureref;

/* include <attmeasureref> */
};

//----------------------------------------------------------------------------
// AttPageRef
//----------------------------------------------------------------------------

class AttPageRef 
{
public:
    AttPageRef();
    virtual ~AttPageRef();
    
    /** Reset the default values for the attribute class **/
    void ResetPageRef();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetPbrefs(std::string pbrefs_) { m_pbrefs = pbrefs_; };
    std::string GetPbrefs() { return m_pbrefs; };
    ///@}

protected:
    /** Contains a list of pbs that are referenced. **/
    std::string m_pbrefs;

/* include <attpbrefs> */
};

//----------------------------------------------------------------------------
// AttSurface
//----------------------------------------------------------------------------

class AttSurface 
{
public:
    AttSurface();
    virtual ~AttSurface();
    
    /** Reset the default values for the attribute class **/
    void ResetSurface();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetSurface(std::string surface_) { m_surface = surface_; };
    std::string GetSurface() { return m_surface; };
    ///@}

protected:
    /** Contains a reference to a surface element **/
    std::string m_surface;

/* include <attsurface> */
};

//----------------------------------------------------------------------------
// AttSystemRef
//----------------------------------------------------------------------------

class AttSystemRef 
{
public:
    AttSystemRef();
    virtual ~AttSystemRef();
    
    /** Reset the default values for the attribute class **/
    void ResetSystemRef();
    
    /**
     * @name Setters and getters for class members
     */
    ///@{
    void SetSbrefs(std::string sbrefs_) { m_sbrefs = sbrefs_; };
    std::string GetSbrefs() { return m_sbrefs; };
    ///@}

protected:
    /** Contains a list of sbs that are referenced. **/
    std::string m_sbrefs;

/* include <attsbrefs> */
};

} // vrv namespace

#endif  // __VRV_ATTS_LAYOUT_H__

