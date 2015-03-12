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

#include "object.h"

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
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLigatureLog::WriteLigatureLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLigatureLog::HasForm( )
{
    return (m_form != "");
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
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("orient")) {
        this->SetOrient(StrToOrientation(element.attribute("orient").value()));
        element.remove_attribute("orient");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensurVis::WriteMensurVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOrient()) {
        element.append_attribute("orient") = OrientationToStr(this->GetOrient()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensurVis::HasForm( )
{
    return (m_form != "");
}

bool AttMensurVis::HasOrient( )
{
    return (m_orient != ORIENTATION_NONE);
}


/* include <attorient> */

//----------------------------------------------------------------------------
// AttMensuralLog
//----------------------------------------------------------------------------

AttMensuralLog::AttMensuralLog(): Att() {
    ResetMensuralLog();
}

AttMensuralLog::~AttMensuralLog() {

}

void AttMensuralLog::ResetMensuralLog() {
    m_mensurDot = BOOLEAN_NONE;
    m_mensurSign = MENSURATIONSIGN_NONE;
    m_mensurSlash = 0;
    m_proportNum = -1;
    m_proportNumbase = -1;
}

bool AttMensuralLog::ReadMensuralLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("mensur.dot")) {
        this->SetMensurDot(StrToBool(element.attribute("mensur.dot").value()));
        element.remove_attribute("mensur.dot");
        hasAttribute = true;
    }
    if (element.attribute("mensur.sign")) {
        this->SetMensurSign(StrToMensurationSign(element.attribute("mensur.sign").value()));
        element.remove_attribute("mensur.sign");
        hasAttribute = true;
    }
    if (element.attribute("mensur.slash")) {
        this->SetMensurSlash(StrToInt(element.attribute("mensur.slash").value()));
        element.remove_attribute("mensur.slash");
        hasAttribute = true;
    }
    if (element.attribute("proport.num")) {
        this->SetProportNum(StrToInt(element.attribute("proport.num").value()));
        element.remove_attribute("proport.num");
        hasAttribute = true;
    }
    if (element.attribute("proport.numbase")) {
        this->SetProportNumbase(StrToInt(element.attribute("proport.numbase").value()));
        element.remove_attribute("proport.numbase");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensuralLog::WriteMensuralLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMensurDot()) {
        element.append_attribute("mensur.dot") = BoolToStr(this->GetMensurDot()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurSign()) {
        element.append_attribute("mensur.sign") = MensurationSignToStr(this->GetMensurSign()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurSlash()) {
        element.append_attribute("mensur.slash") = IntToStr(this->GetMensurSlash()).c_str();
        wroteAttribute = true;
    }
    if (this->HasProportNum()) {
        element.append_attribute("proport.num") = IntToStr(this->GetProportNum()).c_str();
        wroteAttribute = true;
    }
    if (this->HasProportNumbase()) {
        element.append_attribute("proport.numbase") = IntToStr(this->GetProportNumbase()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensuralLog::HasMensurDot( )
{
    return (m_mensurDot != BOOLEAN_NONE);
}

bool AttMensuralLog::HasMensurSign( )
{
    return (m_mensurSign != MENSURATIONSIGN_NONE);
}

bool AttMensuralLog::HasMensurSlash( )
{
    return (m_mensurSlash != 0);
}

bool AttMensuralLog::HasProportNum( )
{
    return (m_proportNum != -1);
}

bool AttMensuralLog::HasProportNumbase( )
{
    return (m_proportNumbase != -1);
}


/* include <attproport.numbase> */

//----------------------------------------------------------------------------
// AttMensuralShared
//----------------------------------------------------------------------------

AttMensuralShared::AttMensuralShared(): Att() {
    ResetMensuralShared();
}

AttMensuralShared::~AttMensuralShared() {

}

void AttMensuralShared::ResetMensuralShared() {
    m_modusmaior = MODUSMAIOR_NONE;
    m_modusminor = MODUSMINOR_NONE;
    m_prolatio = PROLATIO_NONE;
    m_tempus = TEMPUS_NONE;
}

bool AttMensuralShared::ReadMensuralShared(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("modusmaior")) {
        this->SetModusmaior(StrToModusmaior(element.attribute("modusmaior").value()));
        element.remove_attribute("modusmaior");
        hasAttribute = true;
    }
    if (element.attribute("modusminor")) {
        this->SetModusminor(StrToModusminor(element.attribute("modusminor").value()));
        element.remove_attribute("modusminor");
        hasAttribute = true;
    }
    if (element.attribute("prolatio")) {
        this->SetProlatio(StrToProlatio(element.attribute("prolatio").value()));
        element.remove_attribute("prolatio");
        hasAttribute = true;
    }
    if (element.attribute("tempus")) {
        this->SetTempus(StrToTempus(element.attribute("tempus").value()));
        element.remove_attribute("tempus");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensuralShared::WriteMensuralShared(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasModusmaior()) {
        element.append_attribute("modusmaior") = ModusmaiorToStr(this->GetModusmaior()).c_str();
        wroteAttribute = true;
    }
    if (this->HasModusminor()) {
        element.append_attribute("modusminor") = ModusminorToStr(this->GetModusminor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasProlatio()) {
        element.append_attribute("prolatio") = ProlatioToStr(this->GetProlatio()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTempus()) {
        element.append_attribute("tempus") = TempusToStr(this->GetTempus()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensuralShared::HasModusmaior( )
{
    return (m_modusmaior != MODUSMAIOR_NONE);
}

bool AttMensuralShared::HasModusminor( )
{
    return (m_modusminor != MODUSMINOR_NONE);
}

bool AttMensuralShared::HasProlatio( )
{
    return (m_prolatio != PROLATIO_NONE);
}

bool AttMensuralShared::HasTempus( )
{
    return (m_tempus != TEMPUS_NONE);
}


/* include <atttempus> */

//----------------------------------------------------------------------------
// AttMensuralVis
//----------------------------------------------------------------------------

AttMensuralVis::AttMensuralVis(): Att() {
    ResetMensuralVis();
}

AttMensuralVis::~AttMensuralVis() {

}

void AttMensuralVis::ResetMensuralVis() {
    m_mensurColor = "";
    m_mensurForm = "";
    m_mensurLoc = "";
    m_mensurOrient = "";
    m_mensurSize = "";
}

bool AttMensuralVis::ReadMensuralVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("mensur.color")) {
        this->SetMensurColor(StrToStr(element.attribute("mensur.color").value()));
        element.remove_attribute("mensur.color");
        hasAttribute = true;
    }
    if (element.attribute("mensur.form")) {
        this->SetMensurForm(StrToStr(element.attribute("mensur.form").value()));
        element.remove_attribute("mensur.form");
        hasAttribute = true;
    }
    if (element.attribute("mensur.loc")) {
        this->SetMensurLoc(StrToStr(element.attribute("mensur.loc").value()));
        element.remove_attribute("mensur.loc");
        hasAttribute = true;
    }
    if (element.attribute("mensur.orient")) {
        this->SetMensurOrient(StrToStr(element.attribute("mensur.orient").value()));
        element.remove_attribute("mensur.orient");
        hasAttribute = true;
    }
    if (element.attribute("mensur.size")) {
        this->SetMensurSize(StrToStr(element.attribute("mensur.size").value()));
        element.remove_attribute("mensur.size");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensuralVis::WriteMensuralVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMensurColor()) {
        element.append_attribute("mensur.color") = StrToStr(this->GetMensurColor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurForm()) {
        element.append_attribute("mensur.form") = StrToStr(this->GetMensurForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurLoc()) {
        element.append_attribute("mensur.loc") = StrToStr(this->GetMensurLoc()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurOrient()) {
        element.append_attribute("mensur.orient") = StrToStr(this->GetMensurOrient()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurSize()) {
        element.append_attribute("mensur.size") = StrToStr(this->GetMensurSize()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensuralVis::HasMensurColor( )
{
    return (m_mensurColor != "");
}

bool AttMensuralVis::HasMensurForm( )
{
    return (m_mensurForm != "");
}

bool AttMensuralVis::HasMensurLoc( )
{
    return (m_mensurLoc != "");
}

bool AttMensuralVis::HasMensurOrient( )
{
    return (m_mensurOrient != "");
}

bool AttMensuralVis::HasMensurSize( )
{
    return (m_mensurSize != "");
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
    m_lig = LIGATURE_NONE;
}

bool AttNoteLogMensural::ReadNoteLogMensural(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("lig")) {
        this->SetLig(StrToLigature(element.attribute("lig").value()));
        element.remove_attribute("lig");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteLogMensural::WriteNoteLogMensural(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasLig()) {
        element.append_attribute("lig") = LigatureToStr(this->GetLig()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNoteLogMensural::HasLig( )
{
    return (m_lig != LIGATURE_NONE);
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
        this->SetSpaces(StrToInt(element.attribute("spaces").value()));
        element.remove_attribute("spaces");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRestVisMensural::WriteRestVisMensural(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasSpaces()) {
        element.append_attribute("spaces") = IntToStr(this->GetSpaces()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRestVisMensural::HasSpaces( )
{
    return (m_spacesInt != 0);
}


/* include <attspaces> */

bool Att::SetMensural( Object *element, std::string attrType, std::string attrValue ) {
    if (dynamic_cast<AttLigatureLog*>(element) ) {
        AttLigatureLog *att = dynamic_cast<AttLigatureLog*>(element);
        if (attrType == "form") {
            att->SetForm(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMensurVis*>(element) ) {
        AttMensurVis *att = dynamic_cast<AttMensurVis*>(element);
        if (attrType == "form") {
            att->SetForm(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "orient") {
            att->SetOrient(att->StrToOrientation(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMensuralLog*>(element) ) {
        AttMensuralLog *att = dynamic_cast<AttMensuralLog*>(element);
        if (attrType == "mensurDot") {
            att->SetMensurDot(att->StrToBool(attrValue));
            return true;
        }
        if (attrType == "mensurSign") {
            att->SetMensurSign(att->StrToMensurationSign(attrValue));
            return true;
        }
        if (attrType == "mensurSlash") {
            att->SetMensurSlash(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "proportNum") {
            att->SetProportNum(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "proportNumbase") {
            att->SetProportNumbase(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMensuralShared*>(element) ) {
        AttMensuralShared *att = dynamic_cast<AttMensuralShared*>(element);
        if (attrType == "modusmaior") {
            att->SetModusmaior(att->StrToModusmaior(attrValue));
            return true;
        }
        if (attrType == "modusminor") {
            att->SetModusminor(att->StrToModusminor(attrValue));
            return true;
        }
        if (attrType == "prolatio") {
            att->SetProlatio(att->StrToProlatio(attrValue));
            return true;
        }
        if (attrType == "tempus") {
            att->SetTempus(att->StrToTempus(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMensuralVis*>(element) ) {
        AttMensuralVis *att = dynamic_cast<AttMensuralVis*>(element);
        if (attrType == "mensurColor") {
            att->SetMensurColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "mensurForm") {
            att->SetMensurForm(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "mensurLoc") {
            att->SetMensurLoc(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "mensurOrient") {
            att->SetMensurOrient(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "mensurSize") {
            att->SetMensurSize(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttNoteLogMensural*>(element) ) {
        AttNoteLogMensural *att = dynamic_cast<AttNoteLogMensural*>(element);
        if (attrType == "lig") {
            att->SetLig(att->StrToLigature(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttRestVisMensural*>(element) ) {
        AttRestVisMensural *att = dynamic_cast<AttRestVisMensural*>(element);
        if (attrType == "spacesInt") {
            att->SetSpaces(att->StrToInt(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetMensural( Object *element, ArrayOfStrAttr *attributes ) {
    if (dynamic_cast<AttLigatureLog*>(element) ) {
        AttLigatureLog *att = dynamic_cast<AttLigatureLog*>(element);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->StrToStr(att->GetForm())));
        }
    }
    if (dynamic_cast<AttMensurVis*>(element) ) {
        AttMensurVis *att = dynamic_cast<AttMensurVis*>(element);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->StrToStr(att->GetForm())));
        }
        if (att->HasOrient()) {
            attributes->push_back(std::make_pair("orient", att->OrientationToStr(att->GetOrient())));
        }
    }
    if (dynamic_cast<AttMensuralLog*>(element) ) {
        AttMensuralLog *att = dynamic_cast<AttMensuralLog*>(element);
        if (att->HasMensurDot()) {
            attributes->push_back(std::make_pair("mensurDot", att->BoolToStr(att->GetMensurDot())));
        }
        if (att->HasMensurSign()) {
            attributes->push_back(std::make_pair("mensurSign", att->MensurationSignToStr(att->GetMensurSign())));
        }
        if (att->HasMensurSlash()) {
            attributes->push_back(std::make_pair("mensurSlash", att->IntToStr(att->GetMensurSlash())));
        }
        if (att->HasProportNum()) {
            attributes->push_back(std::make_pair("proportNum", att->IntToStr(att->GetProportNum())));
        }
        if (att->HasProportNumbase()) {
            attributes->push_back(std::make_pair("proportNumbase", att->IntToStr(att->GetProportNumbase())));
        }
    }
    if (dynamic_cast<AttMensuralShared*>(element) ) {
        AttMensuralShared *att = dynamic_cast<AttMensuralShared*>(element);
        if (att->HasModusmaior()) {
            attributes->push_back(std::make_pair("modusmaior", att->ModusmaiorToStr(att->GetModusmaior())));
        }
        if (att->HasModusminor()) {
            attributes->push_back(std::make_pair("modusminor", att->ModusminorToStr(att->GetModusminor())));
        }
        if (att->HasProlatio()) {
            attributes->push_back(std::make_pair("prolatio", att->ProlatioToStr(att->GetProlatio())));
        }
        if (att->HasTempus()) {
            attributes->push_back(std::make_pair("tempus", att->TempusToStr(att->GetTempus())));
        }
    }
    if (dynamic_cast<AttMensuralVis*>(element) ) {
        AttMensuralVis *att = dynamic_cast<AttMensuralVis*>(element);
        if (att->HasMensurColor()) {
            attributes->push_back(std::make_pair("mensurColor", att->StrToStr(att->GetMensurColor())));
        }
        if (att->HasMensurForm()) {
            attributes->push_back(std::make_pair("mensurForm", att->StrToStr(att->GetMensurForm())));
        }
        if (att->HasMensurLoc()) {
            attributes->push_back(std::make_pair("mensurLoc", att->StrToStr(att->GetMensurLoc())));
        }
        if (att->HasMensurOrient()) {
            attributes->push_back(std::make_pair("mensurOrient", att->StrToStr(att->GetMensurOrient())));
        }
        if (att->HasMensurSize()) {
            attributes->push_back(std::make_pair("mensurSize", att->StrToStr(att->GetMensurSize())));
        }
    }
    if (dynamic_cast<AttNoteLogMensural*>(element) ) {
        AttNoteLogMensural *att = dynamic_cast<AttNoteLogMensural*>(element);
        if (att->HasLig()) {
            attributes->push_back(std::make_pair("lig", att->LigatureToStr(att->GetLig())));
        }
    }
    if (dynamic_cast<AttRestVisMensural*>(element) ) {
        AttRestVisMensural *att = dynamic_cast<AttRestVisMensural*>(element);
        if (att->HasSpaces()) {
            attributes->push_back(std::make_pair("spacesInt", att->IntToStr(att->GetSpaces())));
        }
    }

}
    
} // vrv namespace
    
