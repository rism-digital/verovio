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
// AttFretlocation
//----------------------------------------------------------------------------

AttFretlocation::AttFretlocation(): Att()
{
    ResetFretlocation();
}

AttFretlocation::~AttFretlocation()
{
}

void AttFretlocation::ResetFretlocation()
{
    m_fret = FRET_NONE;
}

bool AttFretlocation::ReadFretlocation(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("fret")) {
        this->SetFret(StrToFret(element.attribute("fret").value()));
        element.remove_attribute("fret");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFretlocation::WriteFretlocation(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFret()) {
        element.append_attribute("fret") = FretToStr(this->GetFret()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFretlocation::HasFret()
{
    return (m_fret != FRET_NONE);
}


/* include <attfret> */

//----------------------------------------------------------------------------
// AttHarmLog
//----------------------------------------------------------------------------

AttHarmLog::AttHarmLog(): Att()
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

bool AttHarmLog::HasChordref()
{
    return (m_chordref != "");
}


/* include <attchordref> */

//----------------------------------------------------------------------------
// AttHarmVis
//----------------------------------------------------------------------------

AttHarmVis::AttHarmVis(): Att()
{
    ResetHarmVis();
}

AttHarmVis::~AttHarmVis()
{
}

void AttHarmVis::ResetHarmVis()
{
    m_rendgrid = harmVis_RENDGRID_NONE;
}

bool AttHarmVis::ReadHarmVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("rendgrid")) {
        this->SetRendgrid(StrToHarmVisRendgrid(element.attribute("rendgrid").value()));
        element.remove_attribute("rendgrid");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarmVis::WriteHarmVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasRendgrid()) {
        element.append_attribute("rendgrid") = HarmVisRendgridToStr(this->GetRendgrid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarmVis::HasRendgrid()
{
    return (m_rendgrid != harmVis_RENDGRID_NONE);
}


/* include <attrendgrid> */

bool Att::SetHarmony(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_FRETLOCATION)) {
        AttFretlocation *att = dynamic_cast<AttFretlocation*>(element);
        assert(att);
        if (attrType == "fret") {
            att->SetFret(att->StrToFret(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HARMLOG)) {
        AttHarmLog *att = dynamic_cast<AttHarmLog*>(element);
        assert(att);
        if (attrType == "chordref") {
            att->SetChordref(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HARMVIS)) {
        AttHarmVis *att = dynamic_cast<AttHarmVis*>(element);
        assert(att);
        if (attrType == "rendgrid") {
            att->SetRendgrid(att->StrToHarmVisRendgrid(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetHarmony(Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_FRETLOCATION)) {
        AttFretlocation *att = dynamic_cast<AttFretlocation*>(element);
        assert(att);
        if (att->HasFret()) {
            attributes->push_back(std::make_pair("fret", att->FretToStr(att->GetFret())));
        }
    }
    if (element->HasAttClass(ATT_HARMLOG)) {
        AttHarmLog *att = dynamic_cast<AttHarmLog*>(element);
        assert(att);
        if (att->HasChordref()) {
            attributes->push_back(std::make_pair("chordref", att->StrToStr(att->GetChordref())));
        }
    }
    if (element->HasAttClass(ATT_HARMVIS)) {
        AttHarmVis *att = dynamic_cast<AttHarmVis*>(element);
        assert(att);
        if (att->HasRendgrid()) {
            attributes->push_back(std::make_pair("rendgrid", att->HarmVisRendgridToStr(att->GetRendgrid())));
        }
    }

}

} // vrv namespace

