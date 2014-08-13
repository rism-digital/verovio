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

#include "atts_analysis.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttCommonAnl
//----------------------------------------------------------------------------

AttCommonAnl::AttCommonAnl(): Att() {
    ResetCommonAnl();
}

AttCommonAnl::~AttCommonAnl() {

}

void AttCommonAnl::ResetCommonAnl() {
    m_copyof = "";
    m_corresp = "";
    m_next = "";
    m_prev = "";
    m_sameas = "";
    m_synch = "";
}

bool AttCommonAnl::ReadCommonAnl(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("copyof")) {
        this->SetCopyof(StrToStr(element.attribute("copyof").value()));
        hasAttribute = true;
    }
    if (element.attribute("corresp")) {
        this->SetCorresp(StrToStr(element.attribute("corresp").value()));
        hasAttribute = true;
    }
    if (element.attribute("next")) {
        this->SetNext(StrToStr(element.attribute("next").value()));
        hasAttribute = true;
    }
    if (element.attribute("prev")) {
        this->SetPrev(StrToStr(element.attribute("prev").value()));
        hasAttribute = true;
    }
    if (element.attribute("sameas")) {
        this->SetSameas(StrToStr(element.attribute("sameas").value()));
        hasAttribute = true;
    }
    if (element.attribute("synch")) {
        this->SetSynch(StrToStr(element.attribute("synch").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCommonAnl::WriteCommonAnl(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasCopyof()) {
        element.append_attribute("copyof") = StrToStr(this->GetCopyof()).c_str();
        wroteAttribute = true;
    }
    if (this->HasCorresp()) {
        element.append_attribute("corresp") = StrToStr(this->GetCorresp()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNext()) {
        element.append_attribute("next") = StrToStr(this->GetNext()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPrev()) {
        element.append_attribute("prev") = StrToStr(this->GetPrev()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSameas()) {
        element.append_attribute("sameas") = StrToStr(this->GetSameas()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSynch()) {
        element.append_attribute("synch") = StrToStr(this->GetSynch()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCommonAnl::HasCopyof( )
{
    return (m_copyof != "");
}

bool AttCommonAnl::HasCorresp( )
{
    return (m_corresp != "");
}

bool AttCommonAnl::HasNext( )
{
    return (m_next != "");
}

bool AttCommonAnl::HasPrev( )
{
    return (m_prev != "");
}

bool AttCommonAnl::HasSameas( )
{
    return (m_sameas != "");
}

bool AttCommonAnl::HasSynch( )
{
    return (m_synch != "");
}


/* include <attsynch> */

//----------------------------------------------------------------------------
// AttHarmonicfunction
//----------------------------------------------------------------------------

AttHarmonicfunction::AttHarmonicfunction(): Att() {
    ResetHarmonicfunction();
}

AttHarmonicfunction::~AttHarmonicfunction() {

}

void AttHarmonicfunction::ResetHarmonicfunction() {
    m_hfunc = "";
}

bool AttHarmonicfunction::ReadHarmonicfunction(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("hfunc")) {
        this->SetHfunc(StrToStr(element.attribute("hfunc").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarmonicfunction::WriteHarmonicfunction(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasHfunc()) {
        element.append_attribute("hfunc") = StrToStr(this->GetHfunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarmonicfunction::HasHfunc( )
{
    return (m_hfunc != "");
}


/* include <atthfunc> */

//----------------------------------------------------------------------------
// AttIntervalharmonic
//----------------------------------------------------------------------------

AttIntervalharmonic::AttIntervalharmonic(): Att() {
    ResetIntervalharmonic();
}

AttIntervalharmonic::~AttIntervalharmonic() {

}

void AttIntervalharmonic::ResetIntervalharmonic() {
    m_inth = "";
}

bool AttIntervalharmonic::ReadIntervalharmonic(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("inth")) {
        this->SetInth(StrToStr(element.attribute("inth").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttIntervalharmonic::WriteIntervalharmonic(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasInth()) {
        element.append_attribute("inth") = StrToStr(this->GetInth()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttIntervalharmonic::HasInth( )
{
    return (m_inth != "");
}


/* include <attinth> */

//----------------------------------------------------------------------------
// AttIntervallicdesc
//----------------------------------------------------------------------------

AttIntervallicdesc::AttIntervallicdesc(): Att() {
    ResetIntervallicdesc();
}

AttIntervallicdesc::~AttIntervallicdesc() {

}

void AttIntervallicdesc::ResetIntervallicdesc() {
    m_intm = "";
}

bool AttIntervallicdesc::ReadIntervallicdesc(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("intm")) {
        this->SetIntm(StrToStr(element.attribute("intm").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttIntervallicdesc::WriteIntervallicdesc(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasIntm()) {
        element.append_attribute("intm") = StrToStr(this->GetIntm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttIntervallicdesc::HasIntm( )
{
    return (m_intm != "");
}


/* include <attintm> */

//----------------------------------------------------------------------------
// AttMelodicfunction
//----------------------------------------------------------------------------

AttMelodicfunction::AttMelodicfunction(): Att() {
    ResetMelodicfunction();
}

AttMelodicfunction::~AttMelodicfunction() {

}

void AttMelodicfunction::ResetMelodicfunction() {
    m_mfunc = "";
}

bool AttMelodicfunction::ReadMelodicfunction(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("mfunc")) {
        this->SetMfunc(StrToStr(element.attribute("mfunc").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMelodicfunction::WriteMelodicfunction(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMfunc()) {
        element.append_attribute("mfunc") = StrToStr(this->GetMfunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMelodicfunction::HasMfunc( )
{
    return (m_mfunc != "");
}


/* include <attmfunc> */

//----------------------------------------------------------------------------
// AttPitchclass
//----------------------------------------------------------------------------

AttPitchclass::AttPitchclass(): Att() {
    ResetPitchclass();
}

AttPitchclass::~AttPitchclass() {

}

void AttPitchclass::ResetPitchclass() {
    m_pclass = "";
}

bool AttPitchclass::ReadPitchclass(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("pclass")) {
        this->SetPclass(StrToStr(element.attribute("pclass").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPitchclass::WritePitchclass(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasPclass()) {
        element.append_attribute("pclass") = StrToStr(this->GetPclass()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPitchclass::HasPclass( )
{
    return (m_pclass != "");
}


/* include <attpclass> */

//----------------------------------------------------------------------------
// AttSolfa
//----------------------------------------------------------------------------

AttSolfa::AttSolfa(): Att() {
    ResetSolfa();
}

AttSolfa::~AttSolfa() {

}

void AttSolfa::ResetSolfa() {
    m_psolfa = "";
}

bool AttSolfa::ReadSolfa(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("psolfa")) {
        this->SetPsolfa(StrToStr(element.attribute("psolfa").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSolfa::WriteSolfa(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasPsolfa()) {
        element.append_attribute("psolfa") = StrToStr(this->GetPsolfa()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSolfa::HasPsolfa( )
{
    return (m_psolfa != "");
}


/* include <attpsolfa> */

} // vrv namespace

