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

#include "atts_neumes.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttDivLineLog
//----------------------------------------------------------------------------

AttDivLineLog::AttDivLineLog() : Att()
{
    this->ResetDivLineLog();
}

void AttDivLineLog::ResetDivLineLog()
{
    m_form = divLineLog_FORM_NONE;
}

bool AttDivLineLog::ReadDivLineLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToDivLineLogForm(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDivLineLog::WriteDivLineLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = DivLineLogFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDivLineLog::HasForm() const
{
    return (m_form != divLineLog_FORM_NONE);
}

//----------------------------------------------------------------------------
// AttNcLog
//----------------------------------------------------------------------------

AttNcLog::AttNcLog() : Att()
{
    this->ResetNcLog();
}

void AttNcLog::ResetNcLog()
{
    m_oct = "";
    m_pname = "";
}

bool AttNcLog::ReadNcLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("oct")) {
        this->SetOct(StrToStr(element.attribute("oct").value()));
        if (removeAttr) element.remove_attribute("oct");
        hasAttribute = true;
    }
    if (element.attribute("pname")) {
        this->SetPname(StrToStr(element.attribute("pname").value()));
        if (removeAttr) element.remove_attribute("pname");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNcLog::WriteNcLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOct()) {
        element.append_attribute("oct") = StrToStr(this->GetOct()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPname()) {
        element.append_attribute("pname") = StrToStr(this->GetPname()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNcLog::HasOct() const
{
    return (m_oct != "");
}

bool AttNcLog::HasPname() const
{
    return (m_pname != "");
}

//----------------------------------------------------------------------------
// AttNcForm
//----------------------------------------------------------------------------

AttNcForm::AttNcForm() : Att()
{
    this->ResetNcForm();
}

void AttNcForm::ResetNcForm()
{
    m_angled = BOOLEAN_NONE;
    m_con = ncForm_CON_NONE;
    m_hooked = BOOLEAN_NONE;
    m_ligated = BOOLEAN_NONE;
    m_rellen = ncForm_RELLEN_NONE;
    m_sShape = "";
    m_tilt = data_COMPASSDIRECTION();
}

bool AttNcForm::ReadNcForm(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("angled")) {
        this->SetAngled(StrToBoolean(element.attribute("angled").value()));
        if (removeAttr) element.remove_attribute("angled");
        hasAttribute = true;
    }
    if (element.attribute("con")) {
        this->SetCon(StrToNcFormCon(element.attribute("con").value()));
        if (removeAttr) element.remove_attribute("con");
        hasAttribute = true;
    }
    if (element.attribute("hooked")) {
        this->SetHooked(StrToBoolean(element.attribute("hooked").value()));
        if (removeAttr) element.remove_attribute("hooked");
        hasAttribute = true;
    }
    if (element.attribute("ligated")) {
        this->SetLigated(StrToBoolean(element.attribute("ligated").value()));
        if (removeAttr) element.remove_attribute("ligated");
        hasAttribute = true;
    }
    if (element.attribute("rellen")) {
        this->SetRellen(StrToNcFormRellen(element.attribute("rellen").value()));
        if (removeAttr) element.remove_attribute("rellen");
        hasAttribute = true;
    }
    if (element.attribute("sShape")) {
        this->SetSShape(StrToStr(element.attribute("sShape").value()));
        if (removeAttr) element.remove_attribute("sShape");
        hasAttribute = true;
    }
    if (element.attribute("tilt")) {
        this->SetTilt(StrToCompassdirection(element.attribute("tilt").value()));
        if (removeAttr) element.remove_attribute("tilt");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNcForm::WriteNcForm(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAngled()) {
        element.append_attribute("angled") = BooleanToStr(this->GetAngled()).c_str();
        wroteAttribute = true;
    }
    if (this->HasCon()) {
        element.append_attribute("con") = NcFormConToStr(this->GetCon()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHooked()) {
        element.append_attribute("hooked") = BooleanToStr(this->GetHooked()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLigated()) {
        element.append_attribute("ligated") = BooleanToStr(this->GetLigated()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRellen()) {
        element.append_attribute("rellen") = NcFormRellenToStr(this->GetRellen()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSShape()) {
        element.append_attribute("sShape") = StrToStr(this->GetSShape()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTilt()) {
        element.append_attribute("tilt") = CompassdirectionToStr(this->GetTilt()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNcForm::HasAngled() const
{
    return (m_angled != BOOLEAN_NONE);
}

bool AttNcForm::HasCon() const
{
    return (m_con != ncForm_CON_NONE);
}

bool AttNcForm::HasHooked() const
{
    return (m_hooked != BOOLEAN_NONE);
}

bool AttNcForm::HasLigated() const
{
    return (m_ligated != BOOLEAN_NONE);
}

bool AttNcForm::HasRellen() const
{
    return (m_rellen != ncForm_RELLEN_NONE);
}

bool AttNcForm::HasSShape() const
{
    return (m_sShape != "");
}

bool AttNcForm::HasTilt() const
{
    return (m_tilt != data_COMPASSDIRECTION());
}

//----------------------------------------------------------------------------
// AttNeumeType
//----------------------------------------------------------------------------

AttNeumeType::AttNeumeType() : Att()
{
    this->ResetNeumeType();
}

void AttNeumeType::ResetNeumeType()
{
    m_type = "";
}

bool AttNeumeType::ReadNeumeType(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("type")) {
        this->SetType(StrToStr(element.attribute("type").value()));
        if (removeAttr) element.remove_attribute("type");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNeumeType::WriteNeumeType(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasType()) {
        element.append_attribute("type") = StrToStr(this->GetType()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNeumeType::HasType() const
{
    return (m_type != "");
}

} // namespace vrv
