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

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttBifoliumSurfaces
//----------------------------------------------------------------------------

AttBifoliumSurfaces::AttBifoliumSurfaces() : Att()
{
    ResetBifoliumSurfaces();
}

AttBifoliumSurfaces::~AttBifoliumSurfaces()
{
}

void AttBifoliumSurfaces::ResetBifoliumSurfaces()
{
    m_outerRecto = "";
    m_innerVerso = "";
    m_innerRecto = "";
    m_outerVerso = "";
}

bool AttBifoliumSurfaces::ReadBifoliumSurfaces(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("outer.recto")) {
        this->SetOuterRecto(StrToStr(element.attribute("outer.recto").value()));
        element.remove_attribute("outer.recto");
        hasAttribute = true;
    }
    if (element.attribute("inner.verso")) {
        this->SetInnerVerso(StrToStr(element.attribute("inner.verso").value()));
        element.remove_attribute("inner.verso");
        hasAttribute = true;
    }
    if (element.attribute("inner.recto")) {
        this->SetInnerRecto(StrToStr(element.attribute("inner.recto").value()));
        element.remove_attribute("inner.recto");
        hasAttribute = true;
    }
    if (element.attribute("outer.verso")) {
        this->SetOuterVerso(StrToStr(element.attribute("outer.verso").value()));
        element.remove_attribute("outer.verso");
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

/* include <attouter.verso> */

//----------------------------------------------------------------------------
// AttFoliumSurfaces
//----------------------------------------------------------------------------

AttFoliumSurfaces::AttFoliumSurfaces() : Att()
{
    ResetFoliumSurfaces();
}

AttFoliumSurfaces::~AttFoliumSurfaces()
{
}

void AttFoliumSurfaces::ResetFoliumSurfaces()
{
    m_recto = "";
    m_verso = "";
}

bool AttFoliumSurfaces::ReadFoliumSurfaces(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("recto")) {
        this->SetRecto(StrToStr(element.attribute("recto").value()));
        element.remove_attribute("recto");
        hasAttribute = true;
    }
    if (element.attribute("verso")) {
        this->SetVerso(StrToStr(element.attribute("verso").value()));
        element.remove_attribute("verso");
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

/* include <attverso> */

//----------------------------------------------------------------------------
// AttRecordType
//----------------------------------------------------------------------------

AttRecordType::AttRecordType() : Att()
{
    ResetRecordType();
}

AttRecordType::~AttRecordType()
{
}

void AttRecordType::ResetRecordType()
{
    m_recordtype = recordType_RECORDTYPE_NONE;
}

bool AttRecordType::ReadRecordType(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("recordtype")) {
        this->SetRecordtype(StrToRecordTypeRecordtype(element.attribute("recordtype").value()));
        element.remove_attribute("recordtype");
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

/* include <attrecordtype> */

//----------------------------------------------------------------------------
// AttRegularMethod
//----------------------------------------------------------------------------

AttRegularMethod::AttRegularMethod() : Att()
{
    ResetRegularMethod();
}

AttRegularMethod::~AttRegularMethod()
{
}

void AttRegularMethod::ResetRegularMethod()
{
    m_method = regularMethod_METHOD_NONE;
}

bool AttRegularMethod::ReadRegularMethod(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("method")) {
        this->SetMethod(StrToRegularMethodMethod(element.attribute("method").value()));
        element.remove_attribute("method");
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

/* include <attmethod> */

bool Att::SetHeader(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_BIFOLIUMSURFACES)) {
        AttBifoliumSurfaces *att = dynamic_cast<AttBifoliumSurfaces *>(element);
        assert(att);
        if (attrType == "outer.recto") {
            att->SetOuterRecto(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "inner.verso") {
            att->SetInnerVerso(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "inner.recto") {
            att->SetInnerRecto(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "outer.verso") {
            att->SetOuterVerso(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FOLIUMSURFACES)) {
        AttFoliumSurfaces *att = dynamic_cast<AttFoliumSurfaces *>(element);
        assert(att);
        if (attrType == "recto") {
            att->SetRecto(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "verso") {
            att->SetVerso(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_RECORDTYPE)) {
        AttRecordType *att = dynamic_cast<AttRecordType *>(element);
        assert(att);
        if (attrType == "recordtype") {
            att->SetRecordtype(att->StrToRecordTypeRecordtype(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_REGULARMETHOD)) {
        AttRegularMethod *att = dynamic_cast<AttRegularMethod *>(element);
        assert(att);
        if (attrType == "method") {
            att->SetMethod(att->StrToRegularMethodMethod(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetHeader(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_BIFOLIUMSURFACES)) {
        const AttBifoliumSurfaces *att = dynamic_cast<const AttBifoliumSurfaces *>(element);
        assert(att);
        if (att->HasOuterRecto()) {
            attributes->push_back(std::make_pair("outer.recto", att->StrToStr(att->GetOuterRecto())));
        }
        if (att->HasInnerVerso()) {
            attributes->push_back(std::make_pair("inner.verso", att->StrToStr(att->GetInnerVerso())));
        }
        if (att->HasInnerRecto()) {
            attributes->push_back(std::make_pair("inner.recto", att->StrToStr(att->GetInnerRecto())));
        }
        if (att->HasOuterVerso()) {
            attributes->push_back(std::make_pair("outer.verso", att->StrToStr(att->GetOuterVerso())));
        }
    }
    if (element->HasAttClass(ATT_FOLIUMSURFACES)) {
        const AttFoliumSurfaces *att = dynamic_cast<const AttFoliumSurfaces *>(element);
        assert(att);
        if (att->HasRecto()) {
            attributes->push_back(std::make_pair("recto", att->StrToStr(att->GetRecto())));
        }
        if (att->HasVerso()) {
            attributes->push_back(std::make_pair("verso", att->StrToStr(att->GetVerso())));
        }
    }
    if (element->HasAttClass(ATT_RECORDTYPE)) {
        const AttRecordType *att = dynamic_cast<const AttRecordType *>(element);
        assert(att);
        if (att->HasRecordtype()) {
            attributes->push_back(std::make_pair("recordtype", att->RecordTypeRecordtypeToStr(att->GetRecordtype())));
        }
    }
    if (element->HasAttClass(ATT_REGULARMETHOD)) {
        const AttRegularMethod *att = dynamic_cast<const AttRegularMethod *>(element);
        assert(att);
        if (att->HasMethod()) {
            attributes->push_back(std::make_pair("method", att->RegularMethodMethodToStr(att->GetMethod())));
        }
    }
}

} // vrv namespace
