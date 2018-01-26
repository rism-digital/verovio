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

#include "atts_harmony.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttChordDefLog
//----------------------------------------------------------------------------

AttChordDefLog::AttChordDefLog() : Att()
{
    ResetChordDefLog();
}

AttChordDefLog::~AttChordDefLog()
{
}

void AttChordDefLog::ResetChordDefLog()
{
    m_pos = 0;
}

bool AttChordDefLog::ReadChordDefLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("pos")) {
        this->SetPos(StrToInt(element.attribute("pos").value()));
        element.remove_attribute("pos");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttChordDefLog::WriteChordDefLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPos()) {
        element.append_attribute("pos") = IntToStr(this->GetPos()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttChordDefLog::HasPos() const
{
    return (m_pos != 0);
}

/* include <attpos> */

//----------------------------------------------------------------------------
// AttChordMemberLog
//----------------------------------------------------------------------------

AttChordMemberLog::AttChordMemberLog() : Att()
{
    ResetChordMemberLog();
}

AttChordMemberLog::~AttChordMemberLog()
{
}

void AttChordMemberLog::ResetChordMemberLog()
{
    m_string = 0;
    m_fret = 0;
    m_fing = FINGER_FRET_NONE;
}

bool AttChordMemberLog::ReadChordMemberLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("string")) {
        this->SetString(StrToInt(element.attribute("string").value()));
        element.remove_attribute("string");
        hasAttribute = true;
    }
    if (element.attribute("fret")) {
        this->SetFret(StrToInt(element.attribute("fret").value()));
        element.remove_attribute("fret");
        hasAttribute = true;
    }
    if (element.attribute("fing")) {
        this->SetFing(StrToFingerFret(element.attribute("fing").value()));
        element.remove_attribute("fing");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttChordMemberLog::WriteChordMemberLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasString()) {
        element.append_attribute("string") = IntToStr(this->GetString()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFret()) {
        element.append_attribute("fret") = IntToStr(this->GetFret()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFing()) {
        element.append_attribute("fing") = FingerFretToStr(this->GetFing()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttChordMemberLog::HasString() const
{
    return (m_string != 0);
}

bool AttChordMemberLog::HasFret() const
{
    return (m_fret != 0);
}

bool AttChordMemberLog::HasFing() const
{
    return (m_fing != FINGER_FRET_NONE);
}

/* include <attfing> */

//----------------------------------------------------------------------------
// AttHarmLog
//----------------------------------------------------------------------------

AttHarmLog::AttHarmLog() : Att()
{
    ResetHarmLog();
}

AttHarmLog::~AttHarmLog()
{
}

void AttHarmLog::ResetHarmLog()
{
    m_chordref = "";
}

bool AttHarmLog::ReadHarmLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("chordref")) {
        this->SetChordref(StrToStr(element.attribute("chordref").value()));
        element.remove_attribute("chordref");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarmLog::WriteHarmLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasChordref()) {
        element.append_attribute("chordref") = StrToStr(this->GetChordref()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarmLog::HasChordref() const
{
    return (m_chordref != "");
}

/* include <attchordref> */

bool Att::SetHarmony(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_CHORDDEFLOG)) {
        AttChordDefLog *att = dynamic_cast<AttChordDefLog *>(element);
        assert(att);
        if (attrType == "pos") {
            att->SetPos(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CHORDMEMBERLOG)) {
        AttChordMemberLog *att = dynamic_cast<AttChordMemberLog *>(element);
        assert(att);
        if (attrType == "string") {
            att->SetString(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "fret") {
            att->SetFret(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "fing") {
            att->SetFing(att->StrToFingerFret(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HARMLOG)) {
        AttHarmLog *att = dynamic_cast<AttHarmLog *>(element);
        assert(att);
        if (attrType == "chordref") {
            att->SetChordref(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetHarmony(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_CHORDDEFLOG)) {
        const AttChordDefLog *att = dynamic_cast<const AttChordDefLog *>(element);
        assert(att);
        if (att->HasPos()) {
            attributes->push_back(std::make_pair("pos", att->IntToStr(att->GetPos())));
        }
    }
    if (element->HasAttClass(ATT_CHORDMEMBERLOG)) {
        const AttChordMemberLog *att = dynamic_cast<const AttChordMemberLog *>(element);
        assert(att);
        if (att->HasString()) {
            attributes->push_back(std::make_pair("string", att->IntToStr(att->GetString())));
        }
        if (att->HasFret()) {
            attributes->push_back(std::make_pair("fret", att->IntToStr(att->GetFret())));
        }
        if (att->HasFing()) {
            attributes->push_back(std::make_pair("fing", att->FingerFretToStr(att->GetFing())));
        }
    }
    if (element->HasAttClass(ATT_HARMLOG)) {
        const AttHarmLog *att = dynamic_cast<const AttHarmLog *>(element);
        assert(att);
        if (att->HasChordref()) {
            attributes->push_back(std::make_pair("chordref", att->StrToStr(att->GetChordref())));
        }
    }
}

} // vrv namespace
