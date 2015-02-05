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

#include "atts_cmn.h"

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttArpegLog
//----------------------------------------------------------------------------

AttArpegLog::AttArpegLog(): Att() {
    ResetArpegLog();
}

AttArpegLog::~AttArpegLog() {

}

void AttArpegLog::ResetArpegLog() {
    m_order = "";
}

bool AttArpegLog::ReadArpegLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("order")) {
        this->SetOrder(StrToStr(element.attribute("order").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArpegLog::WriteArpegLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasOrder()) {
        element.append_attribute("order") = StrToStr(this->GetOrder()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttArpegLog::HasOrder( )
{
    return (m_order != "");
}


/* include <attorder> */

//----------------------------------------------------------------------------
// AttArpegVis
//----------------------------------------------------------------------------

AttArpegVis::AttArpegVis(): Att() {
    ResetArpegVis();
}

AttArpegVis::~AttArpegVis() {

}

void AttArpegVis::ResetArpegVis() {
    m_arrow = "";
}

bool AttArpegVis::ReadArpegVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("arrow")) {
        this->SetArrow(StrToStr(element.attribute("arrow").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArpegVis::WriteArpegVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasArrow()) {
        element.append_attribute("arrow") = StrToStr(this->GetArrow()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttArpegVis::HasArrow( )
{
    return (m_arrow != "");
}


/* include <attarrow> */

//----------------------------------------------------------------------------
// AttBTremLog
//----------------------------------------------------------------------------

AttBTremLog::AttBTremLog(): Att() {
    ResetBTremLog();
}

AttBTremLog::~AttBTremLog() {

}

void AttBTremLog::ResetBTremLog() {
    m_form = "";
}

bool AttBTremLog::ReadBTremLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToStr(element.attribute("form").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBTremLog::WriteBTremLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBTremLog::HasForm( )
{
    return (m_form != "");
}


/* include <attform> */

//----------------------------------------------------------------------------
// AttBeamed
//----------------------------------------------------------------------------

AttBeamed::AttBeamed(): Att() {
    ResetBeamed();
}

AttBeamed::~AttBeamed() {

}

void AttBeamed::ResetBeamed() {
    m_beam = "";
}

bool AttBeamed::ReadBeamed(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("beam")) {
        this->SetBeam(StrToStr(element.attribute("beam").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamed::WriteBeamed(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBeam()) {
        element.append_attribute("beam") = StrToStr(this->GetBeam()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamed::HasBeam( )
{
    return (m_beam != "");
}


/* include <attbeam> */

//----------------------------------------------------------------------------
// AttBeamedwith
//----------------------------------------------------------------------------

AttBeamedwith::AttBeamedwith(): Att() {
    ResetBeamedwith();
}

AttBeamedwith::~AttBeamedwith() {

}

void AttBeamedwith::ResetBeamedwith() {
    m_beamWith = "";
}

bool AttBeamedwith::ReadBeamedwith(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("beam.with")) {
        this->SetBeamWith(StrToStr(element.attribute("beam.with").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamedwith::WriteBeamedwith(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBeamWith()) {
        element.append_attribute("beam.with") = StrToStr(this->GetBeamWith()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamedwith::HasBeamWith( )
{
    return (m_beamWith != "");
}


/* include <attbeam.with> */

//----------------------------------------------------------------------------
// AttBeamingLog
//----------------------------------------------------------------------------

AttBeamingLog::AttBeamingLog(): Att() {
    ResetBeamingLog();
}

AttBeamingLog::~AttBeamingLog() {

}

void AttBeamingLog::ResetBeamingLog() {
    m_beamGroup = "";
    m_beamRests = "";
}

bool AttBeamingLog::ReadBeamingLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("beam.group")) {
        this->SetBeamGroup(StrToStr(element.attribute("beam.group").value()));
        hasAttribute = true;
    }
    if (element.attribute("beam.rests")) {
        this->SetBeamRests(StrToStr(element.attribute("beam.rests").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamingLog::WriteBeamingLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBeamGroup()) {
        element.append_attribute("beam.group") = StrToStr(this->GetBeamGroup()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBeamRests()) {
        element.append_attribute("beam.rests") = StrToStr(this->GetBeamRests()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamingLog::HasBeamGroup( )
{
    return (m_beamGroup != "");
}

bool AttBeamingLog::HasBeamRests( )
{
    return (m_beamRests != "");
}


/* include <attbeam.rests> */

//----------------------------------------------------------------------------
// AttBeamrend
//----------------------------------------------------------------------------

AttBeamrend::AttBeamrend(): Att() {
    ResetBeamrend();
}

AttBeamrend::~AttBeamrend() {

}

void AttBeamrend::ResetBeamrend() {
    m_rend = "";
    m_slopeDbl = 0.0;
}

bool AttBeamrend::ReadBeamrend(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToStr(element.attribute("rend").value()));
        hasAttribute = true;
    }
    if (element.attribute("slope")) {
        this->SetSlope(StrToDbl(element.attribute("slope").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamrend::WriteBeamrend(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRend()) {
        element.append_attribute("rend") = StrToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSlope()) {
        element.append_attribute("slope") = DblToStr(this->GetSlope()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamrend::HasRend( )
{
    return (m_rend != "");
}

bool AttBeamrend::HasSlope( )
{
    return (m_slopeDbl != 0.0);
}


/* include <attslope> */

//----------------------------------------------------------------------------
// AttBeamsecondary
//----------------------------------------------------------------------------

AttBeamsecondary::AttBeamsecondary(): Att() {
    ResetBeamsecondary();
}

AttBeamsecondary::~AttBeamsecondary() {

}

void AttBeamsecondary::ResetBeamsecondary() {
    m_breaksecInt = 0;
}

bool AttBeamsecondary::ReadBeamsecondary(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("breaksec")) {
        this->SetBreaksec(StrToInt(element.attribute("breaksec").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamsecondary::WriteBeamsecondary(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBreaksec()) {
        element.append_attribute("breaksec") = IntToStr(this->GetBreaksec()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamsecondary::HasBreaksec( )
{
    return (m_breaksecInt != 0);
}


/* include <attbreaksec> */

//----------------------------------------------------------------------------
// AttBeatRptVis
//----------------------------------------------------------------------------

AttBeatRptVis::AttBeatRptVis(): Att() {
    ResetBeatRptVis();
}

AttBeatRptVis::~AttBeatRptVis() {

}

void AttBeatRptVis::ResetBeatRptVis() {
    m_rend = "";
}

bool AttBeatRptVis::ReadBeatRptVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToStr(element.attribute("rend").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeatRptVis::WriteBeatRptVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRend()) {
        element.append_attribute("rend") = StrToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeatRptVis::HasRend( )
{
    return (m_rend != "");
}


/* include <attrend> */

//----------------------------------------------------------------------------
// AttBendGes
//----------------------------------------------------------------------------

AttBendGes::AttBendGes(): Att() {
    ResetBendGes();
}

AttBendGes::~AttBendGes() {

}

void AttBendGes::ResetBendGes() {
    m_amount = "";
}

bool AttBendGes::ReadBendGes(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("amount")) {
        this->SetAmount(StrToStr(element.attribute("amount").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBendGes::WriteBendGes(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasAmount()) {
        element.append_attribute("amount") = StrToStr(this->GetAmount()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBendGes::HasAmount( )
{
    return (m_amount != "");
}


/* include <attamount> */

//----------------------------------------------------------------------------
// AttCutout
//----------------------------------------------------------------------------

AttCutout::AttCutout(): Att() {
    ResetCutout();
}

AttCutout::~AttCutout() {

}

void AttCutout::ResetCutout() {
    m_cutout = "";
}

bool AttCutout::ReadCutout(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("cutout")) {
        this->SetCutout(StrToStr(element.attribute("cutout").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCutout::WriteCutout(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasCutout()) {
        element.append_attribute("cutout") = StrToStr(this->GetCutout()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCutout::HasCutout( )
{
    return (m_cutout != "");
}


/* include <attcutout> */

//----------------------------------------------------------------------------
// AttExpandable
//----------------------------------------------------------------------------

AttExpandable::AttExpandable(): Att() {
    ResetExpandable();
}

AttExpandable::~AttExpandable() {

}

void AttExpandable::ResetExpandable() {
    m_expand = "";
}

bool AttExpandable::ReadExpandable(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("expand")) {
        this->SetExpand(StrToStr(element.attribute("expand").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttExpandable::WriteExpandable(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasExpand()) {
        element.append_attribute("expand") = StrToStr(this->GetExpand()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttExpandable::HasExpand( )
{
    return (m_expand != "");
}


/* include <attexpand> */

//----------------------------------------------------------------------------
// AttFTremLog
//----------------------------------------------------------------------------

AttFTremLog::AttFTremLog(): Att() {
    ResetFTremLog();
}

AttFTremLog::~AttFTremLog() {

}

void AttFTremLog::ResetFTremLog() {
    m_form = "";
}

bool AttFTremLog::ReadFTremLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToStr(element.attribute("form").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFTremLog::WriteFTremLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFTremLog::HasForm( )
{
    return (m_form != "");
}


/* include <attform> */

//----------------------------------------------------------------------------
// AttFermataVis
//----------------------------------------------------------------------------

AttFermataVis::AttFermataVis(): Att() {
    ResetFermataVis();
}

AttFermataVis::~AttFermataVis() {

}

void AttFermataVis::ResetFermataVis() {
    m_form = "";
    m_shape = "";
}

bool AttFermataVis::ReadFermataVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToStr(element.attribute("form").value()));
        hasAttribute = true;
    }
    if (element.attribute("shape")) {
        this->SetShape(StrToStr(element.attribute("shape").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFermataVis::WriteFermataVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasShape()) {
        element.append_attribute("shape") = StrToStr(this->GetShape()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFermataVis::HasForm( )
{
    return (m_form != "");
}

bool AttFermataVis::HasShape( )
{
    return (m_shape != "");
}


/* include <attshape> */

//----------------------------------------------------------------------------
// AttGlissVis
//----------------------------------------------------------------------------

AttGlissVis::AttGlissVis(): Att() {
    ResetGlissVis();
}

AttGlissVis::~AttGlissVis() {

}

void AttGlissVis::ResetGlissVis() {
    m_text = "";
}

bool AttGlissVis::ReadGlissVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("text")) {
        this->SetText(StrToStr(element.attribute("text").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttGlissVis::WriteGlissVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasText()) {
        element.append_attribute("text") = StrToStr(this->GetText()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttGlissVis::HasText( )
{
    return (m_text != "");
}


/* include <atttext> */

//----------------------------------------------------------------------------
// AttGraced
//----------------------------------------------------------------------------

AttGraced::AttGraced(): Att() {
    ResetGraced();
}

AttGraced::~AttGraced() {

}

void AttGraced::ResetGraced() {
    m_grace = "";
    m_graceTime = "";
}

bool AttGraced::ReadGraced(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("grace")) {
        this->SetGrace(StrToStr(element.attribute("grace").value()));
        hasAttribute = true;
    }
    if (element.attribute("grace.time")) {
        this->SetGraceTime(StrToStr(element.attribute("grace.time").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttGraced::WriteGraced(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasGrace()) {
        element.append_attribute("grace") = StrToStr(this->GetGrace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasGraceTime()) {
        element.append_attribute("grace.time") = StrToStr(this->GetGraceTime()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttGraced::HasGrace( )
{
    return (m_grace != "");
}

bool AttGraced::HasGraceTime( )
{
    return (m_graceTime != "");
}


/* include <attgrace.time> */

//----------------------------------------------------------------------------
// AttHairpinLog
//----------------------------------------------------------------------------

AttHairpinLog::AttHairpinLog(): Att() {
    ResetHairpinLog();
}

AttHairpinLog::~AttHairpinLog() {

}

void AttHairpinLog::ResetHairpinLog() {
    m_form = "";
}

bool AttHairpinLog::ReadHairpinLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToStr(element.attribute("form").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHairpinLog::WriteHairpinLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHairpinLog::HasForm( )
{
    return (m_form != "");
}


/* include <attform> */

//----------------------------------------------------------------------------
// AttHairpinVis
//----------------------------------------------------------------------------

AttHairpinVis::AttHairpinVis(): Att() {
    ResetHairpinVis();
}

AttHairpinVis::~AttHairpinVis() {

}

void AttHairpinVis::ResetHairpinVis() {
    m_opening = "";
}

bool AttHairpinVis::ReadHairpinVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("opening")) {
        this->SetOpening(StrToStr(element.attribute("opening").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHairpinVis::WriteHairpinVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasOpening()) {
        element.append_attribute("opening") = StrToStr(this->GetOpening()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHairpinVis::HasOpening( )
{
    return (m_opening != "");
}


/* include <attopening> */

//----------------------------------------------------------------------------
// AttHarpPedalLog
//----------------------------------------------------------------------------

AttHarpPedalLog::AttHarpPedalLog(): Att() {
    ResetHarpPedalLog();
}

AttHarpPedalLog::~AttHarpPedalLog() {

}

void AttHarpPedalLog::ResetHarpPedalLog() {
    m_c = "";
    m_d = "";
    m_e = "";
    m_f = "";
    m_g = "";
    m_a = "";
    m_b = "";
}

bool AttHarpPedalLog::ReadHarpPedalLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("c")) {
        this->SetC(StrToStr(element.attribute("c").value()));
        hasAttribute = true;
    }
    if (element.attribute("d")) {
        this->SetD(StrToStr(element.attribute("d").value()));
        hasAttribute = true;
    }
    if (element.attribute("e")) {
        this->SetE(StrToStr(element.attribute("e").value()));
        hasAttribute = true;
    }
    if (element.attribute("f")) {
        this->SetF(StrToStr(element.attribute("f").value()));
        hasAttribute = true;
    }
    if (element.attribute("g")) {
        this->SetG(StrToStr(element.attribute("g").value()));
        hasAttribute = true;
    }
    if (element.attribute("a")) {
        this->SetA(StrToStr(element.attribute("a").value()));
        hasAttribute = true;
    }
    if (element.attribute("b")) {
        this->SetB(StrToStr(element.attribute("b").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarpPedalLog::WriteHarpPedalLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasC()) {
        element.append_attribute("c") = StrToStr(this->GetC()).c_str();
        wroteAttribute = true;
    }
    if (this->HasD()) {
        element.append_attribute("d") = StrToStr(this->GetD()).c_str();
        wroteAttribute = true;
    }
    if (this->HasE()) {
        element.append_attribute("e") = StrToStr(this->GetE()).c_str();
        wroteAttribute = true;
    }
    if (this->HasF()) {
        element.append_attribute("f") = StrToStr(this->GetF()).c_str();
        wroteAttribute = true;
    }
    if (this->HasG()) {
        element.append_attribute("g") = StrToStr(this->GetG()).c_str();
        wroteAttribute = true;
    }
    if (this->HasA()) {
        element.append_attribute("a") = StrToStr(this->GetA()).c_str();
        wroteAttribute = true;
    }
    if (this->HasB()) {
        element.append_attribute("b") = StrToStr(this->GetB()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarpPedalLog::HasC( )
{
    return (m_c != "");
}

bool AttHarpPedalLog::HasD( )
{
    return (m_d != "");
}

bool AttHarpPedalLog::HasE( )
{
    return (m_e != "");
}

bool AttHarpPedalLog::HasF( )
{
    return (m_f != "");
}

bool AttHarpPedalLog::HasG( )
{
    return (m_g != "");
}

bool AttHarpPedalLog::HasA( )
{
    return (m_a != "");
}

bool AttHarpPedalLog::HasB( )
{
    return (m_b != "");
}


/* include <attb> */

//----------------------------------------------------------------------------
// AttLvpresent
//----------------------------------------------------------------------------

AttLvpresent::AttLvpresent(): Att() {
    ResetLvpresent();
}

AttLvpresent::~AttLvpresent() {

}

void AttLvpresent::ResetLvpresent() {
    m_lv = "";
}

bool AttLvpresent::ReadLvpresent(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("lv")) {
        this->SetLv(StrToStr(element.attribute("lv").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLvpresent::WriteLvpresent(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasLv()) {
        element.append_attribute("lv") = StrToStr(this->GetLv()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLvpresent::HasLv( )
{
    return (m_lv != "");
}


/* include <attlv> */

//----------------------------------------------------------------------------
// AttMultiRestVis
//----------------------------------------------------------------------------

AttMultiRestVis::AttMultiRestVis(): Att() {
    ResetMultiRestVis();
}

AttMultiRestVis::~AttMultiRestVis() {

}

void AttMultiRestVis::ResetMultiRestVis() {
    m_block = "";
}

bool AttMultiRestVis::ReadMultiRestVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("block")) {
        this->SetBlock(StrToStr(element.attribute("block").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMultiRestVis::WriteMultiRestVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBlock()) {
        element.append_attribute("block") = StrToStr(this->GetBlock()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMultiRestVis::HasBlock( )
{
    return (m_block != "");
}


/* include <attblock> */

//----------------------------------------------------------------------------
// AttNoteGesCmn
//----------------------------------------------------------------------------

AttNoteGesCmn::AttNoteGesCmn(): Att() {
    ResetNoteGesCmn();
}

AttNoteGesCmn::~AttNoteGesCmn() {

}

void AttNoteGesCmn::ResetNoteGesCmn() {
    m_gliss = "";
}

bool AttNoteGesCmn::ReadNoteGesCmn(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("gliss")) {
        this->SetGliss(StrToStr(element.attribute("gliss").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteGesCmn::WriteNoteGesCmn(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasGliss()) {
        element.append_attribute("gliss") = StrToStr(this->GetGliss()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNoteGesCmn::HasGliss( )
{
    return (m_gliss != "");
}


/* include <attgliss> */

//----------------------------------------------------------------------------
// AttNumbered
//----------------------------------------------------------------------------

AttNumbered::AttNumbered(): Att() {
    ResetNumbered();
}

AttNumbered::~AttNumbered() {

}

void AttNumbered::ResetNumbered() {
    m_numInt = 0;
}

bool AttNumbered::ReadNumbered(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNum(StrToInt(element.attribute("num").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNumbered::WriteNumbered(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasNum()) {
        element.append_attribute("num") = IntToStr(this->GetNum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNumbered::HasNum( )
{
    return (m_numInt != 0);
}


/* include <attnum> */

//----------------------------------------------------------------------------
// AttNumberplacement
//----------------------------------------------------------------------------

AttNumberplacement::AttNumberplacement(): Att() {
    ResetNumberplacement();
}

AttNumberplacement::~AttNumberplacement() {

}

void AttNumberplacement::ResetNumberplacement() {
    m_numPlace = "";
    m_numVisible = "";
}

bool AttNumberplacement::ReadNumberplacement(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("num.place")) {
        this->SetNumPlace(StrToStr(element.attribute("num.place").value()));
        hasAttribute = true;
    }
    if (element.attribute("num.visible")) {
        this->SetNumVisible(StrToStr(element.attribute("num.visible").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNumberplacement::WriteNumberplacement(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasNumPlace()) {
        element.append_attribute("num.place") = StrToStr(this->GetNumPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNumVisible()) {
        element.append_attribute("num.visible") = StrToStr(this->GetNumVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNumberplacement::HasNumPlace( )
{
    return (m_numPlace != "");
}

bool AttNumberplacement::HasNumVisible( )
{
    return (m_numVisible != "");
}


/* include <attnum.visible> */

//----------------------------------------------------------------------------
// AttOctaveLog
//----------------------------------------------------------------------------

AttOctaveLog::AttOctaveLog(): Att() {
    ResetOctaveLog();
}

AttOctaveLog::~AttOctaveLog() {

}

void AttOctaveLog::ResetOctaveLog() {
    m_coll = "";
}

bool AttOctaveLog::ReadOctaveLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("coll")) {
        this->SetColl(StrToStr(element.attribute("coll").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctaveLog::WriteOctaveLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasColl()) {
        element.append_attribute("coll") = StrToStr(this->GetColl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOctaveLog::HasColl( )
{
    return (m_coll != "");
}


/* include <attcoll> */

//----------------------------------------------------------------------------
// AttPedalLog
//----------------------------------------------------------------------------

AttPedalLog::AttPedalLog(): Att() {
    ResetPedalLog();
}

AttPedalLog::~AttPedalLog() {

}

void AttPedalLog::ResetPedalLog() {
    m_dir = "";
}

bool AttPedalLog::ReadPedalLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dir")) {
        this->SetDir(StrToStr(element.attribute("dir").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPedalLog::WritePedalLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDir()) {
        element.append_attribute("dir") = StrToStr(this->GetDir()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPedalLog::HasDir( )
{
    return (m_dir != "");
}


/* include <attdir> */

//----------------------------------------------------------------------------
// AttPedalVis
//----------------------------------------------------------------------------

AttPedalVis::AttPedalVis(): Att() {
    ResetPedalVis();
}

AttPedalVis::~AttPedalVis() {

}

void AttPedalVis::ResetPedalVis() {
    m_style = "";
}

bool AttPedalVis::ReadPedalVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("style")) {
        this->SetStyle(StrToStr(element.attribute("style").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPedalVis::WritePedalVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasStyle()) {
        element.append_attribute("style") = StrToStr(this->GetStyle()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPedalVis::HasStyle( )
{
    return (m_style != "");
}


/* include <attstyle> */

//----------------------------------------------------------------------------
// AttPianopedals
//----------------------------------------------------------------------------

AttPianopedals::AttPianopedals(): Att() {
    ResetPianopedals();
}

AttPianopedals::~AttPianopedals() {

}

void AttPianopedals::ResetPianopedals() {
    m_pedalStyle = "";
}

bool AttPianopedals::ReadPianopedals(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("pedal.style")) {
        this->SetPedalStyle(StrToStr(element.attribute("pedal.style").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPianopedals::WritePianopedals(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasPedalStyle()) {
        element.append_attribute("pedal.style") = StrToStr(this->GetPedalStyle()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPianopedals::HasPedalStyle( )
{
    return (m_pedalStyle != "");
}


/* include <attpedal.style> */

//----------------------------------------------------------------------------
// AttRehearsal
//----------------------------------------------------------------------------

AttRehearsal::AttRehearsal(): Att() {
    ResetRehearsal();
}

AttRehearsal::~AttRehearsal() {

}

void AttRehearsal::ResetRehearsal() {
    m_rehEnclose = "";
}

bool AttRehearsal::ReadRehearsal(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("reh.enclose")) {
        this->SetRehEnclose(StrToStr(element.attribute("reh.enclose").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRehearsal::WriteRehearsal(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRehEnclose()) {
        element.append_attribute("reh.enclose") = StrToStr(this->GetRehEnclose()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRehearsal::HasRehEnclose( )
{
    return (m_rehEnclose != "");
}


/* include <attreh.enclose> */

//----------------------------------------------------------------------------
// AttScoreDefVisCmn
//----------------------------------------------------------------------------

AttScoreDefVisCmn::AttScoreDefVisCmn(): Att() {
    ResetScoreDefVisCmn();
}

AttScoreDefVisCmn::~AttScoreDefVisCmn() {

}

void AttScoreDefVisCmn::ResetScoreDefVisCmn() {
    m_gridShow = "";
}

bool AttScoreDefVisCmn::ReadScoreDefVisCmn(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("grid.show")) {
        this->SetGridShow(StrToStr(element.attribute("grid.show").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScoreDefVisCmn::WriteScoreDefVisCmn(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasGridShow()) {
        element.append_attribute("grid.show") = StrToStr(this->GetGridShow()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttScoreDefVisCmn::HasGridShow( )
{
    return (m_gridShow != "");
}


/* include <attgrid.show> */

//----------------------------------------------------------------------------
// AttSlurrend
//----------------------------------------------------------------------------

AttSlurrend::AttSlurrend(): Att() {
    ResetSlurrend();
}

AttSlurrend::~AttSlurrend() {

}

void AttSlurrend::ResetSlurrend() {
    m_slurRend = "";
}

bool AttSlurrend::ReadSlurrend(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("slur.rend")) {
        this->SetSlurRend(StrToStr(element.attribute("slur.rend").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSlurrend::WriteSlurrend(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasSlurRend()) {
        element.append_attribute("slur.rend") = StrToStr(this->GetSlurRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSlurrend::HasSlurRend( )
{
    return (m_slurRend != "");
}


/* include <attslur.rend> */

//----------------------------------------------------------------------------
// AttStemmedCmn
//----------------------------------------------------------------------------

AttStemmedCmn::AttStemmedCmn(): Att() {
    ResetStemmedCmn();
}

AttStemmedCmn::~AttStemmedCmn() {

}

void AttStemmedCmn::ResetStemmedCmn() {
    m_stemMod = "";
    m_stemWith = "";
}

bool AttStemmedCmn::ReadStemmedCmn(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("stem.mod")) {
        this->SetStemMod(StrToStr(element.attribute("stem.mod").value()));
        hasAttribute = true;
    }
    if (element.attribute("stem.with")) {
        this->SetStemWith(StrToStr(element.attribute("stem.with").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStemmedCmn::WriteStemmedCmn(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasStemMod()) {
        element.append_attribute("stem.mod") = StrToStr(this->GetStemMod()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStemWith()) {
        element.append_attribute("stem.with") = StrToStr(this->GetStemWith()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStemmedCmn::HasStemMod( )
{
    return (m_stemMod != "");
}

bool AttStemmedCmn::HasStemWith( )
{
    return (m_stemWith != "");
}


/* include <attstem.with> */

//----------------------------------------------------------------------------
// AttTierend
//----------------------------------------------------------------------------

AttTierend::AttTierend(): Att() {
    ResetTierend();
}

AttTierend::~AttTierend() {

}

void AttTierend::ResetTierend() {
    m_tieRend = "";
}

bool AttTierend::ReadTierend(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("tie.rend")) {
        this->SetTieRend(StrToStr(element.attribute("tie.rend").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTierend::WriteTierend(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTieRend()) {
        element.append_attribute("tie.rend") = StrToStr(this->GetTieRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTierend::HasTieRend( )
{
    return (m_tieRend != "");
}


/* include <atttie.rend> */

//----------------------------------------------------------------------------
// AttTremmeasured
//----------------------------------------------------------------------------

AttTremmeasured::AttTremmeasured(): Att() {
    ResetTremmeasured();
}

AttTremmeasured::~AttTremmeasured() {

}

void AttTremmeasured::ResetTremmeasured() {
    m_measperf = "";
}

bool AttTremmeasured::ReadTremmeasured(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("measperf")) {
        this->SetMeasperf(StrToStr(element.attribute("measperf").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTremmeasured::WriteTremmeasured(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMeasperf()) {
        element.append_attribute("measperf") = StrToStr(this->GetMeasperf()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTremmeasured::HasMeasperf( )
{
    return (m_measperf != "");
}


/* include <attmeasperf> */

//----------------------------------------------------------------------------
// AttTupletVis
//----------------------------------------------------------------------------

AttTupletVis::AttTupletVis(): Att() {
    ResetTupletVis();
}

AttTupletVis::~AttTupletVis() {

}

void AttTupletVis::ResetTupletVis() {
    m_bracketPlace = "";
    m_bracketVisible = "";
    m_durVisible = "";
    m_numFormat = "";
}

bool AttTupletVis::ReadTupletVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("bracket.place")) {
        this->SetBracketPlace(StrToStr(element.attribute("bracket.place").value()));
        hasAttribute = true;
    }
    if (element.attribute("bracket.visible")) {
        this->SetBracketVisible(StrToStr(element.attribute("bracket.visible").value()));
        hasAttribute = true;
    }
    if (element.attribute("dur.visible")) {
        this->SetDurVisible(StrToStr(element.attribute("dur.visible").value()));
        hasAttribute = true;
    }
    if (element.attribute("num.format")) {
        this->SetNumFormat(StrToStr(element.attribute("num.format").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTupletVis::WriteTupletVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBracketPlace()) {
        element.append_attribute("bracket.place") = StrToStr(this->GetBracketPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBracketVisible()) {
        element.append_attribute("bracket.visible") = StrToStr(this->GetBracketVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDurVisible()) {
        element.append_attribute("dur.visible") = StrToStr(this->GetDurVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNumFormat()) {
        element.append_attribute("num.format") = StrToStr(this->GetNumFormat()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTupletVis::HasBracketPlace( )
{
    return (m_bracketPlace != "");
}

bool AttTupletVis::HasBracketVisible( )
{
    return (m_bracketVisible != "");
}

bool AttTupletVis::HasDurVisible( )
{
    return (m_durVisible != "");
}

bool AttTupletVis::HasNumFormat( )
{
    return (m_numFormat != "");
}


/* include <attnum.format> */

bool Att::SetCmn( Object *element, std::string attrType, std::string attrValue )
{
    if ( (attrType == "order") && dynamic_cast<AttArpegLog*>(element) ) {
        AttArpegLog *att = dynamic_cast<AttArpegLog*>(element);
        att->SetOrder(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "arrow") && dynamic_cast<AttArpegVis*>(element) ) {
        AttArpegVis *att = dynamic_cast<AttArpegVis*>(element);
        att->SetArrow(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "form") && dynamic_cast<AttBTremLog*>(element) ) {
        AttBTremLog *att = dynamic_cast<AttBTremLog*>(element);
        att->SetForm(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "beam") && dynamic_cast<AttBeamed*>(element) ) {
        AttBeamed *att = dynamic_cast<AttBeamed*>(element);
        att->SetBeam(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "beamWith") && dynamic_cast<AttBeamedwith*>(element) ) {
        AttBeamedwith *att = dynamic_cast<AttBeamedwith*>(element);
        att->SetBeamWith(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "beamGroup") && dynamic_cast<AttBeamingLog*>(element) ) {
        AttBeamingLog *att = dynamic_cast<AttBeamingLog*>(element);
        att->SetBeamGroup(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "beamRests") && dynamic_cast<AttBeamingLog*>(element) ) {
        AttBeamingLog *att = dynamic_cast<AttBeamingLog*>(element);
        att->SetBeamRests(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "rend") && dynamic_cast<AttBeamrend*>(element) ) {
        AttBeamrend *att = dynamic_cast<AttBeamrend*>(element);
        att->SetRend(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "slopeDbl") && dynamic_cast<AttBeamrend*>(element) ) {
        AttBeamrend *att = dynamic_cast<AttBeamrend*>(element);
        att->SetSlope(att->StrToDbl(attrValue));
    return true;
    }
    if ( (attrType == "breaksecInt") && dynamic_cast<AttBeamsecondary*>(element) ) {
        AttBeamsecondary *att = dynamic_cast<AttBeamsecondary*>(element);
        att->SetBreaksec(att->StrToInt(attrValue));
    return true;
    }
    if ( (attrType == "rend") && dynamic_cast<AttBeatRptVis*>(element) ) {
        AttBeatRptVis *att = dynamic_cast<AttBeatRptVis*>(element);
        att->SetRend(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "amount") && dynamic_cast<AttBendGes*>(element) ) {
        AttBendGes *att = dynamic_cast<AttBendGes*>(element);
        att->SetAmount(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "cutout") && dynamic_cast<AttCutout*>(element) ) {
        AttCutout *att = dynamic_cast<AttCutout*>(element);
        att->SetCutout(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "expand") && dynamic_cast<AttExpandable*>(element) ) {
        AttExpandable *att = dynamic_cast<AttExpandable*>(element);
        att->SetExpand(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "form") && dynamic_cast<AttFTremLog*>(element) ) {
        AttFTremLog *att = dynamic_cast<AttFTremLog*>(element);
        att->SetForm(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "form") && dynamic_cast<AttFermataVis*>(element) ) {
        AttFermataVis *att = dynamic_cast<AttFermataVis*>(element);
        att->SetForm(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "shape") && dynamic_cast<AttFermataVis*>(element) ) {
        AttFermataVis *att = dynamic_cast<AttFermataVis*>(element);
        att->SetShape(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "text") && dynamic_cast<AttGlissVis*>(element) ) {
        AttGlissVis *att = dynamic_cast<AttGlissVis*>(element);
        att->SetText(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "grace") && dynamic_cast<AttGraced*>(element) ) {
        AttGraced *att = dynamic_cast<AttGraced*>(element);
        att->SetGrace(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "graceTime") && dynamic_cast<AttGraced*>(element) ) {
        AttGraced *att = dynamic_cast<AttGraced*>(element);
        att->SetGraceTime(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "form") && dynamic_cast<AttHairpinLog*>(element) ) {
        AttHairpinLog *att = dynamic_cast<AttHairpinLog*>(element);
        att->SetForm(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "opening") && dynamic_cast<AttHairpinVis*>(element) ) {
        AttHairpinVis *att = dynamic_cast<AttHairpinVis*>(element);
        att->SetOpening(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "c") && dynamic_cast<AttHarpPedalLog*>(element) ) {
        AttHarpPedalLog *att = dynamic_cast<AttHarpPedalLog*>(element);
        att->SetC(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "d") && dynamic_cast<AttHarpPedalLog*>(element) ) {
        AttHarpPedalLog *att = dynamic_cast<AttHarpPedalLog*>(element);
        att->SetD(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "e") && dynamic_cast<AttHarpPedalLog*>(element) ) {
        AttHarpPedalLog *att = dynamic_cast<AttHarpPedalLog*>(element);
        att->SetE(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "f") && dynamic_cast<AttHarpPedalLog*>(element) ) {
        AttHarpPedalLog *att = dynamic_cast<AttHarpPedalLog*>(element);
        att->SetF(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "g") && dynamic_cast<AttHarpPedalLog*>(element) ) {
        AttHarpPedalLog *att = dynamic_cast<AttHarpPedalLog*>(element);
        att->SetG(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "a") && dynamic_cast<AttHarpPedalLog*>(element) ) {
        AttHarpPedalLog *att = dynamic_cast<AttHarpPedalLog*>(element);
        att->SetA(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "b") && dynamic_cast<AttHarpPedalLog*>(element) ) {
        AttHarpPedalLog *att = dynamic_cast<AttHarpPedalLog*>(element);
        att->SetB(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "lv") && dynamic_cast<AttLvpresent*>(element) ) {
        AttLvpresent *att = dynamic_cast<AttLvpresent*>(element);
        att->SetLv(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "block") && dynamic_cast<AttMultiRestVis*>(element) ) {
        AttMultiRestVis *att = dynamic_cast<AttMultiRestVis*>(element);
        att->SetBlock(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "gliss") && dynamic_cast<AttNoteGesCmn*>(element) ) {
        AttNoteGesCmn *att = dynamic_cast<AttNoteGesCmn*>(element);
        att->SetGliss(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "numInt") && dynamic_cast<AttNumbered*>(element) ) {
        AttNumbered *att = dynamic_cast<AttNumbered*>(element);
        att->SetNum(att->StrToInt(attrValue));
    return true;
    }
    if ( (attrType == "numPlace") && dynamic_cast<AttNumberplacement*>(element) ) {
        AttNumberplacement *att = dynamic_cast<AttNumberplacement*>(element);
        att->SetNumPlace(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "numVisible") && dynamic_cast<AttNumberplacement*>(element) ) {
        AttNumberplacement *att = dynamic_cast<AttNumberplacement*>(element);
        att->SetNumVisible(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "coll") && dynamic_cast<AttOctaveLog*>(element) ) {
        AttOctaveLog *att = dynamic_cast<AttOctaveLog*>(element);
        att->SetColl(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "dir") && dynamic_cast<AttPedalLog*>(element) ) {
        AttPedalLog *att = dynamic_cast<AttPedalLog*>(element);
        att->SetDir(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "style") && dynamic_cast<AttPedalVis*>(element) ) {
        AttPedalVis *att = dynamic_cast<AttPedalVis*>(element);
        att->SetStyle(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "pedalStyle") && dynamic_cast<AttPianopedals*>(element) ) {
        AttPianopedals *att = dynamic_cast<AttPianopedals*>(element);
        att->SetPedalStyle(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "rehEnclose") && dynamic_cast<AttRehearsal*>(element) ) {
        AttRehearsal *att = dynamic_cast<AttRehearsal*>(element);
        att->SetRehEnclose(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "gridShow") && dynamic_cast<AttScoreDefVisCmn*>(element) ) {
        AttScoreDefVisCmn *att = dynamic_cast<AttScoreDefVisCmn*>(element);
        att->SetGridShow(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "slurRend") && dynamic_cast<AttSlurrend*>(element) ) {
        AttSlurrend *att = dynamic_cast<AttSlurrend*>(element);
        att->SetSlurRend(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "stemMod") && dynamic_cast<AttStemmedCmn*>(element) ) {
        AttStemmedCmn *att = dynamic_cast<AttStemmedCmn*>(element);
        att->SetStemMod(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "stemWith") && dynamic_cast<AttStemmedCmn*>(element) ) {
        AttStemmedCmn *att = dynamic_cast<AttStemmedCmn*>(element);
        att->SetStemWith(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "tieRend") && dynamic_cast<AttTierend*>(element) ) {
        AttTierend *att = dynamic_cast<AttTierend*>(element);
        att->SetTieRend(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "measperf") && dynamic_cast<AttTremmeasured*>(element) ) {
        AttTremmeasured *att = dynamic_cast<AttTremmeasured*>(element);
        att->SetMeasperf(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "bracketPlace") && dynamic_cast<AttTupletVis*>(element) ) {
        AttTupletVis *att = dynamic_cast<AttTupletVis*>(element);
        att->SetBracketPlace(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "bracketVisible") && dynamic_cast<AttTupletVis*>(element) ) {
        AttTupletVis *att = dynamic_cast<AttTupletVis*>(element);
        att->SetBracketVisible(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "durVisible") && dynamic_cast<AttTupletVis*>(element) ) {
        AttTupletVis *att = dynamic_cast<AttTupletVis*>(element);
        att->SetDurVisible(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "numFormat") && dynamic_cast<AttTupletVis*>(element) ) {
        AttTupletVis *att = dynamic_cast<AttTupletVis*>(element);
        att->SetNumFormat(att->StrToStr(attrValue));
    return true;
    }

    return false;
}


} // vrv namespace
    