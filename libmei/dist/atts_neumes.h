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

#ifndef __LIBMEI_ATTS_NEUMES_H__
#define __LIBMEI_ATTS_NEUMES_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttDivLineLog
//----------------------------------------------------------------------------

class AttDivLineLog : public Att {
protected:
    AttDivLineLog();
    ~AttDivLineLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetDivLineLog();

    /** Read the values for the attribute class **/
    bool ReadDivLineLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteDivLineLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(divLineLog_FORM form_) { m_form = form_; }
    divLineLog_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    divLineLog_FORM m_form;
};

//----------------------------------------------------------------------------
// InstDivLineLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttDivLineLog
 */

class InstDivLineLog : public AttDivLineLog {
public:
    InstDivLineLog() = default;
    virtual ~InstDivLineLog() = default;
};

//----------------------------------------------------------------------------
// AttNcLog
//----------------------------------------------------------------------------

class AttNcLog : public Att {
protected:
    AttNcLog();
    ~AttNcLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetNcLog();

    /** Read the values for the attribute class **/
    bool ReadNcLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteNcLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOct(std::string oct_) { m_oct = oct_; }
    std::string GetOct() const { return m_oct; }
    bool HasOct() const;
    //
    void SetPname(std::string pname_) { m_pname = pname_; }
    std::string GetPname() const { return m_pname; }
    bool HasPname() const;
    ///@}

private:
    /** Captures written octave information. **/
    std::string m_oct;
    /** Contains a written pitch name. **/
    std::string m_pname;
};

//----------------------------------------------------------------------------
// InstNcLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttNcLog
 */

class InstNcLog : public AttNcLog {
public:
    InstNcLog() = default;
    virtual ~InstNcLog() = default;
};

//----------------------------------------------------------------------------
// AttNcForm
//----------------------------------------------------------------------------

class AttNcForm : public Att {
protected:
    AttNcForm();
    ~AttNcForm() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetNcForm();

    /** Read the values for the attribute class **/
    bool ReadNcForm(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteNcForm(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAngled(data_BOOLEAN angled_) { m_angled = angled_; }
    data_BOOLEAN GetAngled() const { return m_angled; }
    bool HasAngled() const;
    //
    void SetCon(ncForm_CON con_) { m_con = con_; }
    ncForm_CON GetCon() const { return m_con; }
    bool HasCon() const;
    //
    void SetHooked(data_BOOLEAN hooked_) { m_hooked = hooked_; }
    data_BOOLEAN GetHooked() const { return m_hooked; }
    bool HasHooked() const;
    //
    void SetLigated(data_BOOLEAN ligated_) { m_ligated = ligated_; }
    data_BOOLEAN GetLigated() const { return m_ligated; }
    bool HasLigated() const;
    //
    void SetRellen(ncForm_RELLEN rellen_) { m_rellen = rellen_; }
    ncForm_RELLEN GetRellen() const { return m_rellen; }
    bool HasRellen() const;
    //
    void SetSShape(std::string sShape_) { m_sShape = sShape_; }
    std::string GetSShape() const { return m_sShape; }
    bool HasSShape() const;
    //
    void SetTilt(data_COMPASSDIRECTION tilt_) { m_tilt = tilt_; }
    data_COMPASSDIRECTION GetTilt() const { return m_tilt; }
    bool HasTilt() const;
    ///@}

private:
    /** --- **/
    data_BOOLEAN m_angled;
    /**
     * Connection to the previous component within the same neume; this attribute
     * should not be used for the first component of a neume.
     **/
    ncForm_CON m_con;
    /** Pen stroke has an extension; specific to Hispanic notation. **/
    data_BOOLEAN m_hooked;
    /** Indicates participation in a ligature. **/
    data_BOOLEAN m_ligated;
    /**
     * Length of the pen stroke relative to the previous component within the same
     * neume; this attribute should not be used for the first component of a neume.
     **/
    ncForm_RELLEN m_rellen;
    /** --- **/
    std::string m_sShape;
    /** Direction of the pen stroke. **/
    data_COMPASSDIRECTION m_tilt;
};

//----------------------------------------------------------------------------
// InstNcForm
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttNcForm
 */

class InstNcForm : public AttNcForm {
public:
    InstNcForm() = default;
    virtual ~InstNcForm() = default;
};

//----------------------------------------------------------------------------
// AttNeumeType
//----------------------------------------------------------------------------

class AttNeumeType : public Att {
protected:
    AttNeumeType();
    ~AttNeumeType() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetNeumeType();

    /** Read the values for the attribute class **/
    bool ReadNeumeType(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteNeumeType(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetType(std::string type_) { m_type = type_; }
    std::string GetType() const { return m_type; }
    bool HasType() const;
    ///@}

private:
    /**
     * Provides a description of the relationship between the current and the target
     * categories.
     **/
    std::string m_type;
};

//----------------------------------------------------------------------------
// InstNeumeType
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttNeumeType
 */

class InstNeumeType : public AttNeumeType {
public:
    InstNeumeType() = default;
    virtual ~InstNeumeType() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_NEUMES_H__
