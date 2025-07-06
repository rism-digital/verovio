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

#include "atts_header.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttAdlibitum
//----------------------------------------------------------------------------

AttAdlibitum::AttAdlibitum() : Att()
{
    this->ResetAdlibitum();
}

void AttAdlibitum::ResetAdlibitum()
{
    m_adlib = BOOLEAN_NONE;
}

bool AttAdlibitum::ReadAdlibitum(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("adlib")) {
        this->SetAdlib(StrToBoolean(element.attribute("adlib").value()));
        if (removeAttr) element.remove_attribute("adlib");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAdlibitum::WriteAdlibitum(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAdlib()) {
        element.append_attribute("adlib") = BooleanToStr(this->GetAdlib()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAdlibitum::HasAdlib() const
{
    return (m_adlib != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttBifoliumSurfaces
//----------------------------------------------------------------------------

AttBifoliumSurfaces::AttBifoliumSurfaces() : Att()
{
    this->ResetBifoliumSurfaces();
}

void AttBifoliumSurfaces::ResetBifoliumSurfaces()
{
    m_outerRecto = "";
    m_innerVerso = "";
    m_innerRecto = "";
    m_outerVerso = "";
}

bool AttBifoliumSurfaces::ReadBifoliumSurfaces(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("outer.recto")) {
        this->SetOuterRecto(StrToStr(element.attribute("outer.recto").value()));
        if (removeAttr) element.remove_attribute("outer.recto");
        hasAttribute = true;
    }
    if (element.attribute("inner.verso")) {
        this->SetInnerVerso(StrToStr(element.attribute("inner.verso").value()));
        if (removeAttr) element.remove_attribute("inner.verso");
        hasAttribute = true;
    }
    if (element.attribute("inner.recto")) {
        this->SetInnerRecto(StrToStr(element.attribute("inner.recto").value()));
        if (removeAttr) element.remove_attribute("inner.recto");
        hasAttribute = true;
    }
    if (element.attribute("outer.verso")) {
        this->SetOuterVerso(StrToStr(element.attribute("outer.verso").value()));
        if (removeAttr) element.remove_attribute("outer.verso");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBifoliumSurfaces::WriteBifoliumSurfaces(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOuterRecto()) {
        element.append_attribute("outer.recto") = StrToStr(this->GetOuterRecto()).c_str();
        wroteAttribute = true;
    }
    if (this->HasInnerVerso()) {
        element.append_attribute("inner.verso") = StrToStr(this->GetInnerVerso()).c_str();
        wroteAttribute = true;
    }
    if (this->HasInnerRecto()) {
        element.append_attribute("inner.recto") = StrToStr(this->GetInnerRecto()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOuterVerso()) {
        element.append_attribute("outer.verso") = StrToStr(this->GetOuterVerso()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBifoliumSurfaces::HasOuterRecto() const
{
    return (m_outerRecto != "");
}

bool AttBifoliumSurfaces::HasInnerVerso() const
{
    return (m_innerVerso != "");
}

bool AttBifoliumSurfaces::HasInnerRecto() const
{
    return (m_innerRecto != "");
}

bool AttBifoliumSurfaces::HasOuterVerso() const
{
    return (m_outerVerso != "");
}

//----------------------------------------------------------------------------
// AttFoliumSurfaces
//----------------------------------------------------------------------------

AttFoliumSurfaces::AttFoliumSurfaces() : Att()
{
    this->ResetFoliumSurfaces();
}

void AttFoliumSurfaces::ResetFoliumSurfaces()
{
    m_recto = "";
    m_verso = "";
}

bool AttFoliumSurfaces::ReadFoliumSurfaces(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("recto")) {
        this->SetRecto(StrToStr(element.attribute("recto").value()));
        if (removeAttr) element.remove_attribute("recto");
        hasAttribute = true;
    }
    if (element.attribute("verso")) {
        this->SetVerso(StrToStr(element.attribute("verso").value()));
        if (removeAttr) element.remove_attribute("verso");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFoliumSurfaces::WriteFoliumSurfaces(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasRecto()) {
        element.append_attribute("recto") = StrToStr(this->GetRecto()).c_str();
        wroteAttribute = true;
    }
    if (this->HasVerso()) {
        element.append_attribute("verso") = StrToStr(this->GetVerso()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFoliumSurfaces::HasRecto() const
{
    return (m_recto != "");
}

bool AttFoliumSurfaces::HasVerso() const
{
    return (m_verso != "");
}

//----------------------------------------------------------------------------
// AttPerfRes
//----------------------------------------------------------------------------

AttPerfRes::AttPerfRes() : Att()
{
    this->ResetPerfRes();
}

void AttPerfRes::ResetPerfRes()
{
    m_solo = BOOLEAN_NONE;
}

bool AttPerfRes::ReadPerfRes(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("solo")) {
        this->SetSolo(StrToBoolean(element.attribute("solo").value()));
        if (removeAttr) element.remove_attribute("solo");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPerfRes::WritePerfRes(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSolo()) {
        element.append_attribute("solo") = BooleanToStr(this->GetSolo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPerfRes::HasSolo() const
{
    return (m_solo != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttPerfResBasic
//----------------------------------------------------------------------------

AttPerfResBasic::AttPerfResBasic() : Att()
{
    this->ResetPerfResBasic();
}

void AttPerfResBasic::ResetPerfResBasic()
{
    m_count = MEI_UNSET;
}

bool AttPerfResBasic::ReadPerfResBasic(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("count")) {
        this->SetCount(StrToInt(element.attribute("count").value()));
        if (removeAttr) element.remove_attribute("count");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPerfResBasic::WritePerfResBasic(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCount()) {
        element.append_attribute("count") = IntToStr(this->GetCount()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPerfResBasic::HasCount() const
{
    return (m_count != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttRecordType
//----------------------------------------------------------------------------

AttRecordType::AttRecordType() : Att()
{
    this->ResetRecordType();
}

void AttRecordType::ResetRecordType()
{
    m_recordtype = recordType_RECORDTYPE_NONE;
}

bool AttRecordType::ReadRecordType(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("recordtype")) {
        this->SetRecordtype(StrToRecordTypeRecordtype(element.attribute("recordtype").value()));
        if (removeAttr) element.remove_attribute("recordtype");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRecordType::WriteRecordType(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasRecordtype()) {
        element.append_attribute("recordtype") = RecordTypeRecordtypeToStr(this->GetRecordtype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRecordType::HasRecordtype() const
{
    return (m_recordtype != recordType_RECORDTYPE_NONE);
}

//----------------------------------------------------------------------------
// AttRegularMethod
//----------------------------------------------------------------------------

AttRegularMethod::AttRegularMethod() : Att()
{
    this->ResetRegularMethod();
}

void AttRegularMethod::ResetRegularMethod()
{
    m_method = regularMethod_METHOD_NONE;
}

bool AttRegularMethod::ReadRegularMethod(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("method")) {
        this->SetMethod(StrToRegularMethodMethod(element.attribute("method").value()));
        if (removeAttr) element.remove_attribute("method");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRegularMethod::WriteRegularMethod(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMethod()) {
        element.append_attribute("method") = RegularMethodMethodToStr(this->GetMethod()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRegularMethod::HasMethod() const
{
    return (m_method != regularMethod_METHOD_NONE);
}

} // namespace vrv
