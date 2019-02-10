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

#ifndef __VRV_ATTS_USERSYMBOLS_H__
#define __VRV_ATTS_USERSYMBOLS_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttAltSym
//----------------------------------------------------------------------------

class AttAltSym : public Att {
public:
    AttAltSym();
    virtual ~AttAltSym();

    /** Reset the default values for the attribute class **/
    void ResetAltSym();

    /** Read the values for the attribute class **/
    bool ReadAltSym(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAltSym(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAltsym(std::string altsym_) { m_altsym = altsym_; }
    std::string GetAltsym() const { return m_altsym; }
    bool HasAltsym() const;
    ///@}

private:
    /**
     * Provides a way of pointing to a user-defined symbol.
     * It must contain a reference to an ID of a
     **/
    std::string m_altsym;

    /* include <attaltsym> */
};

//----------------------------------------------------------------------------
// AttAnchoredTextLog
//----------------------------------------------------------------------------

class AttAnchoredTextLog : public Att {
public:
    AttAnchoredTextLog();
    virtual ~AttAnchoredTextLog();

    /** Reset the default values for the attribute class **/
    void ResetAnchoredTextLog();

    /** Read the values for the attribute class **/
    bool ReadAnchoredTextLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAnchoredTextLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFunc(std::string func_) { m_func = func_; }
    std::string GetFunc() const { return m_func; }
    bool HasFunc() const;
    ///@}

private:
    /** Describes the function of the bracketed event sequence. **/
    std::string m_func;

    /* include <attfunc> */
};

//----------------------------------------------------------------------------
// AttCurveLog
//----------------------------------------------------------------------------

class AttCurveLog : public Att {
public:
    AttCurveLog();
    virtual ~AttCurveLog();

    /** Reset the default values for the attribute class **/
    void ResetCurveLog();

    /** Read the values for the attribute class **/
    bool ReadCurveLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteCurveLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFunc(std::string func_) { m_func = func_; }
    std::string GetFunc() const { return m_func; }
    bool HasFunc() const;
    ///@}

private:
    /** Describes the function of the bracketed event sequence. **/
    std::string m_func;

    /* include <attfunc> */
};

//----------------------------------------------------------------------------
// AttLineLog
//----------------------------------------------------------------------------

class AttLineLog : public Att {
public:
    AttLineLog();
    virtual ~AttLineLog();

    /** Reset the default values for the attribute class **/
    void ResetLineLog();

    /** Read the values for the attribute class **/
    bool ReadLineLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLineLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFunc(std::string func_) { m_func = func_; }
    std::string GetFunc() const { return m_func; }
    bool HasFunc() const;
    ///@}

private:
    /** Describes the function of the bracketed event sequence. **/
    std::string m_func;

    /* include <attfunc> */
};

} // vrv namespace

#endif // __VRV_ATTS_USERSYMBOLS_H__
