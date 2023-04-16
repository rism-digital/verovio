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

#include "atts_stringtab.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttStringtab
//----------------------------------------------------------------------------

AttStringtab::AttStringtab() : Att()
{
    ResetStringtab();
}

AttStringtab::~AttStringtab()
{
}

void AttStringtab::ResetStringtab()
{
    m_tabFing = FINGER_FRET_NONE;
    m_tabFret = FRETNUMBER_NONE;
    m_tabString = STRINGNUMBER_NONE;
}

bool AttStringtab::ReadStringtab(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tab.fing")) {
        this->SetTabFing(StrToFingerFret(element.attribute("tab.fing").value()));
        element.remove_attribute("tab.fing");
        hasAttribute = true;
    }
    if (element.attribute("tab.fret")) {
        this->SetTabFret(StrToFretnumber(element.attribute("tab.fret").value()));
        element.remove_attribute("tab.fret");
        hasAttribute = true;
    }
    if (element.attribute("tab.string")) {
        this->SetTabString(StrToStringnumber(element.attribute("tab.string").value()));
        element.remove_attribute("tab.string");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStringtab::WriteStringtab(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTabFing()) {
        element.append_attribute("tab.fing") = FingerFretToStr(this->GetTabFing()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTabFret()) {
        element.append_attribute("tab.fret") = FretnumberToStr(this->GetTabFret()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTabString()) {
        element.append_attribute("tab.string") = StringnumberToStr(this->GetTabString()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStringtab::HasTabFing() const
{
    return (m_tabFing != FINGER_FRET_NONE);
}

bool AttStringtab::HasTabFret() const
{
    return (m_tabFret != FRETNUMBER_NONE);
}

bool AttStringtab::HasTabString() const
{
    return (m_tabString != STRINGNUMBER_NONE);
}

/* include <atttab.string> */

//----------------------------------------------------------------------------
// AttStringtabPosition
//----------------------------------------------------------------------------

AttStringtabPosition::AttStringtabPosition() : Att()
{
    ResetStringtabPosition();
}

AttStringtabPosition::~AttStringtabPosition()
{
}

void AttStringtabPosition::ResetStringtabPosition()
{
    m_tabPos = 0;
}

bool AttStringtabPosition::ReadStringtabPosition(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tab.pos")) {
        this->SetTabPos(StrToInt(element.attribute("tab.pos").value()));
        element.remove_attribute("tab.pos");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStringtabPosition::WriteStringtabPosition(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTabPos()) {
        element.append_attribute("tab.pos") = IntToStr(this->GetTabPos()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStringtabPosition::HasTabPos() const
{
    return (m_tabPos != 0);
}

/* include <atttab.pos> */

//----------------------------------------------------------------------------
// AttStringtabTuning
//----------------------------------------------------------------------------

AttStringtabTuning::AttStringtabTuning() : Att()
{
    ResetStringtabTuning();
}

AttStringtabTuning::~AttStringtabTuning()
{
}

void AttStringtabTuning::ResetStringtabTuning()
{
    m_tabStrings = "";
}

bool AttStringtabTuning::ReadStringtabTuning(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tab.strings")) {
        this->SetTabStrings(StrToStr(element.attribute("tab.strings").value()));
        element.remove_attribute("tab.strings");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStringtabTuning::WriteStringtabTuning(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTabStrings()) {
        element.append_attribute("tab.strings") = StrToStr(this->GetTabStrings()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStringtabTuning::HasTabStrings() const
{
    return (m_tabStrings != "");
}

/* include <atttab.strings> */

bool Att::SetStringtab(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_STRINGTAB)) {
        AttStringtab *att = dynamic_cast<AttStringtab *>(element);
        assert(att);
        if (attrType == "tab.fing") {
            att->SetTabFing(att->StrToFingerFret(attrValue));
            return true;
        }
        if (attrType == "tab.fret") {
            att->SetTabFret(att->StrToFretnumber(attrValue));
            return true;
        }
        if (attrType == "tab.string") {
            att->SetTabString(att->StrToStringnumber(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STRINGTABPOSITION)) {
        AttStringtabPosition *att = dynamic_cast<AttStringtabPosition *>(element);
        assert(att);
        if (attrType == "tab.pos") {
            att->SetTabPos(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STRINGTABTUNING)) {
        AttStringtabTuning *att = dynamic_cast<AttStringtabTuning *>(element);
        assert(att);
        if (attrType == "tab.strings") {
            att->SetTabStrings(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetStringtab(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_STRINGTAB)) {
        const AttStringtab *att = dynamic_cast<const AttStringtab *>(element);
        assert(att);
        if (att->HasTabFing()) {
            attributes->push_back({ "tab.fing", att->FingerFretToStr(att->GetTabFing()) });
        }
        if (att->HasTabFret()) {
            attributes->push_back({ "tab.fret", att->FretnumberToStr(att->GetTabFret()) });
        }
        if (att->HasTabString()) {
            attributes->push_back({ "tab.string", att->StringnumberToStr(att->GetTabString()) });
        }
    }
    if (element->HasAttClass(ATT_STRINGTABPOSITION)) {
        const AttStringtabPosition *att = dynamic_cast<const AttStringtabPosition *>(element);
        assert(att);
        if (att->HasTabPos()) {
            attributes->push_back({ "tab.pos", att->IntToStr(att->GetTabPos()) });
        }
    }
    if (element->HasAttClass(ATT_STRINGTABTUNING)) {
        const AttStringtabTuning *att = dynamic_cast<const AttStringtabTuning *>(element);
        assert(att);
        if (att->HasTabStrings()) {
            attributes->push_back({ "tab.strings", att->StrToStr(att->GetTabStrings()) });
        }
    }
}

} // vrv namespace
