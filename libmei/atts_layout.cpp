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

#include "atts_layout.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttBarline
//----------------------------------------------------------------------------

AttBarline::AttBarline(): Att() {
    ResetBarline();
}

AttBarline::~AttBarline() {

}

void AttBarline::ResetBarline() {
    m_barline = "";
}

bool AttBarline::ReadBarline(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("barline")) {
        this->SetBarline(StrToStr(element.attribute("barline").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBarline::WriteBarline(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetBarline() == "") {
        element.append_attribute("barline") = StrToStr(this->GetBarline()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attbarline> */

//----------------------------------------------------------------------------
// AttIgnore
//----------------------------------------------------------------------------

AttIgnore::AttIgnore(): Att() {
    ResetIgnore();
}

AttIgnore::~AttIgnore() {

}

void AttIgnore::ResetIgnore() {
    m_ignored = "";
}

bool AttIgnore::ReadIgnore(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("ignored")) {
        this->SetIgnored(StrToStr(element.attribute("ignored").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttIgnore::WriteIgnore(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetIgnored() == "") {
        element.append_attribute("ignored") = StrToStr(this->GetIgnored()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attignored> */

//----------------------------------------------------------------------------
// AttLayout
//----------------------------------------------------------------------------

AttLayout::AttLayout(): Att() {
    ResetLayout();
}

AttLayout::~AttLayout() {

}

void AttLayout::ResetLayout() {
    m_ignored = "";
}

bool AttLayout::ReadLayout(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("ignored")) {
        this->SetIgnored(StrToStr(element.attribute("ignored").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLayout::WriteLayout(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetIgnored() == "") {
        element.append_attribute("ignored") = StrToStr(this->GetIgnored()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attignored> */

//----------------------------------------------------------------------------
// AttMeasureRef
//----------------------------------------------------------------------------

AttMeasureRef::AttMeasureRef(): Att() {
    ResetMeasureRef();
}

AttMeasureRef::~AttMeasureRef() {

}

void AttMeasureRef::ResetMeasureRef() {
    m_measureref = "";
}

bool AttMeasureRef::ReadMeasureRef(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("measureref")) {
        this->SetMeasureref(StrToStr(element.attribute("measureref").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeasureRef::WriteMeasureRef(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMeasureref() == "") {
        element.append_attribute("measureref") = StrToStr(this->GetMeasureref()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmeasureref> */

//----------------------------------------------------------------------------
// AttPageRef
//----------------------------------------------------------------------------

AttPageRef::AttPageRef(): Att() {
    ResetPageRef();
}

AttPageRef::~AttPageRef() {

}

void AttPageRef::ResetPageRef() {
    m_pbrefs = "";
}

bool AttPageRef::ReadPageRef(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("pbrefs")) {
        this->SetPbrefs(StrToStr(element.attribute("pbrefs").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPageRef::WritePageRef(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetPbrefs() == "") {
        element.append_attribute("pbrefs") = StrToStr(this->GetPbrefs()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attpbrefs> */

//----------------------------------------------------------------------------
// AttSurface
//----------------------------------------------------------------------------

AttSurface::AttSurface(): Att() {
    ResetSurface();
}

AttSurface::~AttSurface() {

}

void AttSurface::ResetSurface() {
    m_surface = "";
}

bool AttSurface::ReadSurface(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("surface")) {
        this->SetSurface(StrToStr(element.attribute("surface").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSurface::WriteSurface(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetSurface() == "") {
        element.append_attribute("surface") = StrToStr(this->GetSurface()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attsurface> */

//----------------------------------------------------------------------------
// AttSystemRef
//----------------------------------------------------------------------------

AttSystemRef::AttSystemRef(): Att() {
    ResetSystemRef();
}

AttSystemRef::~AttSystemRef() {

}

void AttSystemRef::ResetSystemRef() {
    m_sbrefs = "";
}

bool AttSystemRef::ReadSystemRef(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("sbrefs")) {
        this->SetSbrefs(StrToStr(element.attribute("sbrefs").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSystemRef::WriteSystemRef(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetSbrefs() == "") {
        element.append_attribute("sbrefs") = StrToStr(this->GetSbrefs()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attsbrefs> */

} // vrv namespace

