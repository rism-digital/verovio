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

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttNcLog
//----------------------------------------------------------------------------

AttNcLog::AttNcLog() : Att()
{
    ResetNcLog();
}

AttNcLog::~AttNcLog()
{
}

void AttNcLog::ResetNcLog()
{
    m_oct = "";
    m_pname = "";
}

bool AttNcLog::ReadNcLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("oct")) {
        this->SetOct(StrToStr(element.attribute("oct").value()));
        element.remove_attribute("oct");
        hasAttribute = true;
    }
    if (element.attribute("pname")) {
        this->SetPname(StrToStr(element.attribute("pname").value()));
        element.remove_attribute("pname");
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

/* include <attpname> */

//----------------------------------------------------------------------------
// AttNcForm
//----------------------------------------------------------------------------

AttNcForm::AttNcForm() : Att()
{
    ResetNcForm();
}

AttNcForm::~AttNcForm()
{
}

void AttNcForm::ResetNcForm()
{
    m_angled = BOOLEAN_NONE;
    m_con = ncForm_CON_NONE;
    m_curve = ncForm_CURVE_NONE;
    m_hooked = BOOLEAN_NONE;
    m_ligated = BOOLEAN_NONE;
    m_rellen = ncForm_RELLEN_NONE;
    m_sShape = "";
    m_tilt = data_COMPASSDIRECTION();
}

bool AttNcForm::ReadNcForm(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("angled")) {
        this->SetAngled(StrToBoolean(element.attribute("angled").value()));
        element.remove_attribute("angled");
        hasAttribute = true;
    }
    if (element.attribute("con")) {
        this->SetCon(StrToNcFormCon(element.attribute("con").value()));
        element.remove_attribute("con");
        hasAttribute = true;
    }
    if (element.attribute("curve")) {
        this->SetCurve(StrToNcFormCurve(element.attribute("curve").value()));
        element.remove_attribute("curve");
        hasAttribute = true;
    }
    if (element.attribute("hooked")) {
        this->SetHooked(StrToBoolean(element.attribute("hooked").value()));
        element.remove_attribute("hooked");
        hasAttribute = true;
    }
    if (element.attribute("ligated")) {
        this->SetLigated(StrToBoolean(element.attribute("ligated").value()));
        element.remove_attribute("ligated");
        hasAttribute = true;
    }
    if (element.attribute("rellen")) {
        this->SetRellen(StrToNcFormRellen(element.attribute("rellen").value()));
        element.remove_attribute("rellen");
        hasAttribute = true;
    }
    if (element.attribute("sShape")) {
        this->SetSShape(StrToStr(element.attribute("sShape").value()));
        element.remove_attribute("sShape");
        hasAttribute = true;
    }
    if (element.attribute("tilt")) {
        this->SetTilt(StrToCompassdirection(element.attribute("tilt").value()));
        element.remove_attribute("tilt");
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
    if (this->HasCurve()) {
        element.append_attribute("curve") = NcFormCurveToStr(this->GetCurve()).c_str();
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

bool AttNcForm::HasCurve() const
{
    return (m_curve != ncForm_CURVE_NONE);
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

/* include <atttilt> */

bool Att::SetNeumes(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_NCLOG)) {
        AttNcLog *att = dynamic_cast<AttNcLog *>(element);
        assert(att);
        if (attrType == "oct") {
            att->SetOct(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pname") {
            att->SetPname(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NCFORM)) {
        AttNcForm *att = dynamic_cast<AttNcForm *>(element);
        assert(att);
        if (attrType == "angled") {
            att->SetAngled(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "con") {
            att->SetCon(att->StrToNcFormCon(attrValue));
            return true;
        }
        if (attrType == "curve") {
            att->SetCurve(att->StrToNcFormCurve(attrValue));
            return true;
        }
        if (attrType == "hooked") {
            att->SetHooked(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "ligated") {
            att->SetLigated(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "rellen") {
            att->SetRellen(att->StrToNcFormRellen(attrValue));
            return true;
        }
        if (attrType == "sShape") {
            att->SetSShape(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "tilt") {
            att->SetTilt(att->StrToCompassdirection(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetNeumes(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_NCLOG)) {
        const AttNcLog *att = dynamic_cast<const AttNcLog *>(element);
        assert(att);
        if (att->HasOct()) {
            attributes->push_back(std::make_pair("oct", att->StrToStr(att->GetOct())));
        }
        if (att->HasPname()) {
            attributes->push_back(std::make_pair("pname", att->StrToStr(att->GetPname())));
        }
    }
    if (element->HasAttClass(ATT_NCFORM)) {
        const AttNcForm *att = dynamic_cast<const AttNcForm *>(element);
        assert(att);
        if (att->HasAngled()) {
            attributes->push_back(std::make_pair("angled", att->BooleanToStr(att->GetAngled())));
        }
        if (att->HasCon()) {
            attributes->push_back(std::make_pair("con", att->NcFormConToStr(att->GetCon())));
        }
        if (att->HasCurve()) {
            attributes->push_back(std::make_pair("curve", att->NcFormCurveToStr(att->GetCurve())));
        }
        if (att->HasHooked()) {
            attributes->push_back(std::make_pair("hooked", att->BooleanToStr(att->GetHooked())));
        }
        if (att->HasLigated()) {
            attributes->push_back(std::make_pair("ligated", att->BooleanToStr(att->GetLigated())));
        }
        if (att->HasRellen()) {
            attributes->push_back(std::make_pair("rellen", att->NcFormRellenToStr(att->GetRellen())));
        }
        if (att->HasSShape()) {
            attributes->push_back(std::make_pair("sShape", att->StrToStr(att->GetSShape())));
        }
        if (att->HasTilt()) {
            attributes->push_back(std::make_pair("tilt", att->CompassdirectionToStr(att->GetTilt())));
        }
    }
}

} // vrv namespace
