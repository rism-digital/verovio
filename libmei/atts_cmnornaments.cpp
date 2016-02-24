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

#include "atts_cmnornaments.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttMordentLog
//----------------------------------------------------------------------------

AttMordentLog::AttMordentLog() : Att()
{
    ResetMordentLog();
}

AttMordentLog::~AttMordentLog()
{
}

void AttMordentLog::ResetMordentLog()
{
    m_form = mordentLog_FORM_NONE;
    m_long = BOOLEAN_NONE;
}

bool AttMordentLog::ReadMordentLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToMordentLogForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("long")) {
        this->SetLong(StrToBoolean(element.attribute("long").value()));
        element.remove_attribute("long");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMordentLog::WriteMordentLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = MordentLogFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLong()) {
        element.append_attribute("long") = BooleanToStr(this->GetLong()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMordentLog::HasForm()
{
    return (m_form != mordentLog_FORM_NONE);
}

bool AttMordentLog::HasLong()
{
    return (m_long != BOOLEAN_NONE);
}

/* include <attlong> */

//----------------------------------------------------------------------------
// AttOrnam
//----------------------------------------------------------------------------

AttOrnam::AttOrnam() : Att()
{
    ResetOrnam();
}

AttOrnam::~AttOrnam()
{
}

void AttOrnam::ResetOrnam()
{
    m_ornam = ORNAM_cmn_NONE;
}

bool AttOrnam::ReadOrnam(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("ornam")) {
        this->SetOrnam(StrToOrnamCmn(element.attribute("ornam").value()));
        element.remove_attribute("ornam");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOrnam::WriteOrnam(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOrnam()) {
        element.append_attribute("ornam") = OrnamCmnToStr(this->GetOrnam()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOrnam::HasOrnam()
{
    return (m_ornam != ORNAM_cmn_NONE);
}

/* include <attornam> */

//----------------------------------------------------------------------------
// AttOrnamentaccid
//----------------------------------------------------------------------------

AttOrnamentaccid::AttOrnamentaccid() : Att()
{
    ResetOrnamentaccid();
}

AttOrnamentaccid::~AttOrnamentaccid()
{
}

void AttOrnamentaccid::ResetOrnamentaccid()
{
    m_accidupper = ACCIDENTAL_EXPLICIT_NONE;
    m_accidlower = ACCIDENTAL_EXPLICIT_NONE;
}

bool AttOrnamentaccid::ReadOrnamentaccid(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("accidupper")) {
        this->SetAccidupper(StrToAccidentalExplicit(element.attribute("accidupper").value()));
        element.remove_attribute("accidupper");
        hasAttribute = true;
    }
    if (element.attribute("accidlower")) {
        this->SetAccidlower(StrToAccidentalExplicit(element.attribute("accidlower").value()));
        element.remove_attribute("accidlower");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOrnamentaccid::WriteOrnamentaccid(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAccidupper()) {
        element.append_attribute("accidupper") = AccidentalExplicitToStr(this->GetAccidupper()).c_str();
        wroteAttribute = true;
    }
    if (this->HasAccidlower()) {
        element.append_attribute("accidlower") = AccidentalExplicitToStr(this->GetAccidlower()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOrnamentaccid::HasAccidupper()
{
    return (m_accidupper != ACCIDENTAL_EXPLICIT_NONE);
}

bool AttOrnamentaccid::HasAccidlower()
{
    return (m_accidlower != ACCIDENTAL_EXPLICIT_NONE);
}

/* include <attaccidlower> */

//----------------------------------------------------------------------------
// AttTurnLog
//----------------------------------------------------------------------------

AttTurnLog::AttTurnLog() : Att()
{
    ResetTurnLog();
}

AttTurnLog::~AttTurnLog()
{
}

void AttTurnLog::ResetTurnLog()
{
    m_delayed = BOOLEAN_NONE;
    m_form = turnLog_FORM_NONE;
}

bool AttTurnLog::ReadTurnLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("delayed")) {
        this->SetDelayed(StrToBoolean(element.attribute("delayed").value()));
        element.remove_attribute("delayed");
        hasAttribute = true;
    }
    if (element.attribute("form")) {
        this->SetForm(StrToTurnLogForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTurnLog::WriteTurnLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDelayed()) {
        element.append_attribute("delayed") = BooleanToStr(this->GetDelayed()).c_str();
        wroteAttribute = true;
    }
    if (this->HasForm()) {
        element.append_attribute("form") = TurnLogFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTurnLog::HasDelayed()
{
    return (m_delayed != BOOLEAN_NONE);
}

bool AttTurnLog::HasForm()
{
    return (m_form != turnLog_FORM_NONE);
}

/* include <attform> */

bool Att::SetCmnornaments(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_MORDENTLOG)) {
        AttMordentLog *att = vrv_cast(AttMordentLog *)(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToMordentLogForm(attrValue));
            return true;
        }
        if (attrType == "long") {
            att->SetLong(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ORNAM)) {
        AttOrnam *att = vrv_cast(AttOrnam *)(element);
        assert(att);
        if (attrType == "ornam") {
            att->SetOrnam(att->StrToOrnamCmn(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ORNAMENTACCID)) {
        AttOrnamentaccid *att = vrv_cast(AttOrnamentaccid *)(element);
        assert(att);
        if (attrType == "accidupper") {
            att->SetAccidupper(att->StrToAccidentalExplicit(attrValue));
            return true;
        }
        if (attrType == "accidlower") {
            att->SetAccidlower(att->StrToAccidentalExplicit(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TURNLOG)) {
        AttTurnLog *att = vrv_cast(AttTurnLog *)(element);
        assert(att);
        if (attrType == "delayed") {
            att->SetDelayed(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "form") {
            att->SetForm(att->StrToTurnLogForm(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetCmnornaments(Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_MORDENTLOG)) {
        AttMordentLog *att = vrv_cast(AttMordentLog *)(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->MordentLogFormToStr(att->GetForm())));
        }
        if (att->HasLong()) {
            attributes->push_back(std::make_pair("long", att->BooleanToStr(att->GetLong())));
        }
    }
    if (element->HasAttClass(ATT_ORNAM)) {
        AttOrnam *att = vrv_cast(AttOrnam *)(element);
        assert(att);
        if (att->HasOrnam()) {
            attributes->push_back(std::make_pair("ornam", att->OrnamCmnToStr(att->GetOrnam())));
        }
    }
    if (element->HasAttClass(ATT_ORNAMENTACCID)) {
        AttOrnamentaccid *att = vrv_cast(AttOrnamentaccid *)(element);
        assert(att);
        if (att->HasAccidupper()) {
            attributes->push_back(std::make_pair("accidupper", att->AccidentalExplicitToStr(att->GetAccidupper())));
        }
        if (att->HasAccidlower()) {
            attributes->push_back(std::make_pair("accidlower", att->AccidentalExplicitToStr(att->GetAccidlower())));
        }
    }
    if (element->HasAttClass(ATT_TURNLOG)) {
        AttTurnLog *att = vrv_cast(AttTurnLog *)(element);
        assert(att);
        if (att->HasDelayed()) {
            attributes->push_back(std::make_pair("delayed", att->BooleanToStr(att->GetDelayed())));
        }
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->TurnLogFormToStr(att->GetForm())));
        }
    }
}

} // vrv namespace
