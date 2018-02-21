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

bool Att::SetHeader(Object *element, std::string attrType, std::string attrValue)
{
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
