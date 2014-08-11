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

#include "atts_mensural.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttLigatureLog
//----------------------------------------------------------------------------

AttLigatureLog::AttLigatureLog(): Att() {
    ResetLigatureLog();
}

AttLigatureLog::~AttLigatureLog() {

}

void AttLigatureLog::ResetLigatureLog() {
    m_form = "";
}

bool AttLigatureLog::ReadLigatureLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToStr(element.attribute("form").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLigatureLog::WriteLigatureLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetForm() == "") {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttMensurVis
//----------------------------------------------------------------------------

AttMensurVis::AttMensurVis(): Att() {
    ResetMensurVis();
}

AttMensurVis::~AttMensurVis() {

}

void AttMensurVis::ResetMensurVis() {
    m_form = "";
    m_orient = ORIENTATION_NONE;
}

bool AttMensurVis::ReadMensurVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToStr(element.attribute("form").value()));
        hasAttribute = true;
    }
    if (element.attribute("orient")) {
        this->SetOrient(StrToOrientation(element.attribute("orient").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensurVis::WriteMensurVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetForm() == "") {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->GetOrient() == ORIENTATION_NONE) {
        element.append_attribute("orient") = OrientationToStr(this->GetOrient()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attorient> */

//----------------------------------------------------------------------------
// AttMensurDefaultLog
//----------------------------------------------------------------------------

AttMensurDefaultLog::AttMensurDefaultLog(): Att() {
    ResetMensurDefaultLog();
}

AttMensurDefaultLog::~AttMensurDefaultLog() {

}

void AttMensurDefaultLog::ResetMensurDefaultLog() {
    m_mensurDot = BOOLEAN_NONE;
    m_mensurLocInt = 0;
    m_mensurSign = MENSURATIONSIGN_NONE;
    m_mensurSlash = 0;
}

bool AttMensurDefaultLog::ReadMensurDefaultLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("mensur.dot")) {
        this->SetMensurDot(StrToBool(element.attribute("mensur.dot").value()));
        hasAttribute = true;
    }
    if (element.attribute("mensur.loc")) {
        this->SetMensurLocInt(StrToInt(element.attribute("mensur.loc").value()));
        hasAttribute = true;
    }
    if (element.attribute("mensur.sign")) {
        this->SetMensurSign(StrToMensurationSign(element.attribute("mensur.sign").value()));
        hasAttribute = true;
    }
    if (element.attribute("mensur.slash")) {
        this->SetMensurSlash(StrToInt(element.attribute("mensur.slash").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensurDefaultLog::WriteMensurDefaultLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMensurDot() == BOOLEAN_NONE) {
        element.append_attribute("mensur.dot") = BoolToStr(this->GetMensurDot()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMensurLocInt() == 0) {
        element.append_attribute("mensur.loc") = IntToStr(this->GetMensurLocInt()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMensurSign() == MENSURATIONSIGN_NONE) {
        element.append_attribute("mensur.sign") = MensurationSignToStr(this->GetMensurSign()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMensurSlash() == 0) {
        element.append_attribute("mensur.slash") = IntToStr(this->GetMensurSlash()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmensur.slash> */

//----------------------------------------------------------------------------
// AttMensurDefaultVis
//----------------------------------------------------------------------------

AttMensurDefaultVis::AttMensurDefaultVis(): Att() {
    ResetMensurDefaultVis();
}

AttMensurDefaultVis::~AttMensurDefaultVis() {

}

void AttMensurDefaultVis::ResetMensurDefaultVis() {
    m_mensurColor = "";
    m_mensurForm = "";
    m_mensurOrient = "";
    m_mensurSize = "";
}

bool AttMensurDefaultVis::ReadMensurDefaultVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("mensur.color")) {
        this->SetMensurColor(StrToStr(element.attribute("mensur.color").value()));
        hasAttribute = true;
    }
    if (element.attribute("mensur.form")) {
        this->SetMensurForm(StrToStr(element.attribute("mensur.form").value()));
        hasAttribute = true;
    }
    if (element.attribute("mensur.orient")) {
        this->SetMensurOrient(StrToStr(element.attribute("mensur.orient").value()));
        hasAttribute = true;
    }
    if (element.attribute("mensur.size")) {
        this->SetMensurSize(StrToStr(element.attribute("mensur.size").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensurDefaultVis::WriteMensurDefaultVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMensurColor() == "") {
        element.append_attribute("mensur.color") = StrToStr(this->GetMensurColor()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMensurForm() == "") {
        element.append_attribute("mensur.form") = StrToStr(this->GetMensurForm()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMensurOrient() == "") {
        element.append_attribute("mensur.orient") = StrToStr(this->GetMensurOrient()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMensurSize() == "") {
        element.append_attribute("mensur.size") = StrToStr(this->GetMensurSize()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmensur.size> */

//----------------------------------------------------------------------------
// AttNoteLogMensural
//----------------------------------------------------------------------------

AttNoteLogMensural::AttNoteLogMensural(): Att() {
    ResetNoteLogMensural();
}

AttNoteLogMensural::~AttNoteLogMensural() {

}

void AttNoteLogMensural::ResetNoteLogMensural() {
    m_lig = "";
}

bool AttNoteLogMensural::ReadNoteLogMensural(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("lig")) {
        this->SetLig(StrToStr(element.attribute("lig").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteLogMensural::WriteNoteLogMensural(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetLig() == "") {
        element.append_attribute("lig") = StrToStr(this->GetLig()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attlig> */

//----------------------------------------------------------------------------
// AttRestVisMensural
//----------------------------------------------------------------------------

AttRestVisMensural::AttRestVisMensural(): Att() {
    ResetRestVisMensural();
}

AttRestVisMensural::~AttRestVisMensural() {

}

void AttRestVisMensural::ResetRestVisMensural() {
    m_spacesInt = 0;
}

bool AttRestVisMensural::ReadRestVisMensural(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("spaces")) {
        this->SetSpacesInt(StrToInt(element.attribute("spaces").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRestVisMensural::WriteRestVisMensural(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetSpacesInt() == 0) {
        element.append_attribute("spaces") = IntToStr(this->GetSpacesInt()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attspaces> */

//----------------------------------------------------------------------------
// AttStaffDefLogMensural
//----------------------------------------------------------------------------

AttStaffDefLogMensural::AttStaffDefLogMensural(): Att() {
    ResetStaffDefLogMensural();
}

AttStaffDefLogMensural::~AttStaffDefLogMensural() {

}

void AttStaffDefLogMensural::ResetStaffDefLogMensural() {
    m_proportNumInt = 0;
    m_proportNumbaseInt = 0;
}

bool AttStaffDefLogMensural::ReadStaffDefLogMensural(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("proport.num")) {
        this->SetProportNumInt(StrToInt(element.attribute("proport.num").value()));
        hasAttribute = true;
    }
    if (element.attribute("proport.numbase")) {
        this->SetProportNumbaseInt(StrToInt(element.attribute("proport.numbase").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffDefLogMensural::WriteStaffDefLogMensural(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetProportNumInt() == 0) {
        element.append_attribute("proport.num") = IntToStr(this->GetProportNumInt()).c_str();
        wroteAttribute = true;
    }
    if (this->GetProportNumbaseInt() == 0) {
        element.append_attribute("proport.numbase") = IntToStr(this->GetProportNumbaseInt()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attproport.numbase> */

//----------------------------------------------------------------------------
// AttStafflocPitched
//----------------------------------------------------------------------------

AttStafflocPitched::AttStafflocPitched(): Att() {
    ResetStafflocPitched();
}

AttStafflocPitched::~AttStafflocPitched() {

}

void AttStafflocPitched::ResetStafflocPitched() {
    m_ploc = "";
    m_oloc = "";
}

bool AttStafflocPitched::ReadStafflocPitched(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("ploc")) {
        this->SetPloc(StrToStr(element.attribute("ploc").value()));
        hasAttribute = true;
    }
    if (element.attribute("oloc")) {
        this->SetOloc(StrToStr(element.attribute("oloc").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStafflocPitched::WriteStafflocPitched(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetPloc() == "") {
        element.append_attribute("ploc") = StrToStr(this->GetPloc()).c_str();
        wroteAttribute = true;
    }
    if (this->GetOloc() == "") {
        element.append_attribute("oloc") = StrToStr(this->GetOloc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attoloc> */

} // vrv namespace

