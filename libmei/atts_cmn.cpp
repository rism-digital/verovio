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

#include <assert.h>

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
        element.remove_attribute("order");
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
        element.remove_attribute("arrow");
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
        element.remove_attribute("form");
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
        element.remove_attribute("beam");
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
        element.remove_attribute("beam.with");
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
        element.remove_attribute("beam.group");
        hasAttribute = true;
    }
    if (element.attribute("beam.rests")) {
        this->SetBeamRests(StrToStr(element.attribute("beam.rests").value()));
        element.remove_attribute("beam.rests");
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
    m_rend = BEATRPT_REND_NONE;
    m_slopeDbl = 0.0;
}

bool AttBeamrend::ReadBeamrend(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBeatrptRend(element.attribute("rend").value()));
        element.remove_attribute("rend");
        hasAttribute = true;
    }
    if (element.attribute("slope")) {
        this->SetSlope(StrToDbl(element.attribute("slope").value()));
        element.remove_attribute("slope");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamrend::WriteBeamrend(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRend()) {
        element.append_attribute("rend") = BeatrptRendToStr(this->GetRend()).c_str();
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
    return (m_rend != BEATRPT_REND_NONE);
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
        element.remove_attribute("breaksec");
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
    m_rend = BEATRPT_REND_NONE;
}

bool AttBeatRptVis::ReadBeatRptVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBeatrptRend(element.attribute("rend").value()));
        element.remove_attribute("rend");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeatRptVis::WriteBeatRptVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRend()) {
        element.append_attribute("rend") = BeatrptRendToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeatRptVis::HasRend( )
{
    return (m_rend != BEATRPT_REND_NONE);
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
        element.remove_attribute("amount");
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
        element.remove_attribute("cutout");
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
        element.remove_attribute("expand");
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
        element.remove_attribute("form");
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
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("shape")) {
        this->SetShape(StrToStr(element.attribute("shape").value()));
        element.remove_attribute("shape");
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
        element.remove_attribute("text");
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
    m_grace = GRACE_NONE;
    m_graceTime = "";
}

bool AttGraced::ReadGraced(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("grace")) {
        this->SetGrace(StrToGrace(element.attribute("grace").value()));
        element.remove_attribute("grace");
        hasAttribute = true;
    }
    if (element.attribute("grace.time")) {
        this->SetGraceTime(StrToStr(element.attribute("grace.time").value()));
        element.remove_attribute("grace.time");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttGraced::WriteGraced(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasGrace()) {
        element.append_attribute("grace") = GraceToStr(this->GetGrace()).c_str();
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
    return (m_grace != GRACE_NONE);
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
        element.remove_attribute("form");
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
        element.remove_attribute("opening");
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
        element.remove_attribute("c");
        hasAttribute = true;
    }
    if (element.attribute("d")) {
        this->SetD(StrToStr(element.attribute("d").value()));
        element.remove_attribute("d");
        hasAttribute = true;
    }
    if (element.attribute("e")) {
        this->SetE(StrToStr(element.attribute("e").value()));
        element.remove_attribute("e");
        hasAttribute = true;
    }
    if (element.attribute("f")) {
        this->SetF(StrToStr(element.attribute("f").value()));
        element.remove_attribute("f");
        hasAttribute = true;
    }
    if (element.attribute("g")) {
        this->SetG(StrToStr(element.attribute("g").value()));
        element.remove_attribute("g");
        hasAttribute = true;
    }
    if (element.attribute("a")) {
        this->SetA(StrToStr(element.attribute("a").value()));
        element.remove_attribute("a");
        hasAttribute = true;
    }
    if (element.attribute("b")) {
        this->SetB(StrToStr(element.attribute("b").value()));
        element.remove_attribute("b");
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
        element.remove_attribute("lv");
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
        element.remove_attribute("block");
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
        element.remove_attribute("gliss");
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
        element.remove_attribute("num");
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
    m_numPlace = PLACE_NONE;
    m_numVisible = BOOLEAN_NONE;
}

bool AttNumberplacement::ReadNumberplacement(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("num.place")) {
        this->SetNumPlace(StrToPlace(element.attribute("num.place").value()));
        element.remove_attribute("num.place");
        hasAttribute = true;
    }
    if (element.attribute("num.visible")) {
        this->SetNumVisible(StrToBool(element.attribute("num.visible").value()));
        element.remove_attribute("num.visible");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNumberplacement::WriteNumberplacement(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasNumPlace()) {
        element.append_attribute("num.place") = PlaceToStr(this->GetNumPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNumVisible()) {
        element.append_attribute("num.visible") = BoolToStr(this->GetNumVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNumberplacement::HasNumPlace( )
{
    return (m_numPlace != PLACE_NONE);
}

bool AttNumberplacement::HasNumVisible( )
{
    return (m_numVisible != BOOLEAN_NONE);
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
        element.remove_attribute("coll");
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
        element.remove_attribute("dir");
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
        element.remove_attribute("style");
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
        element.remove_attribute("pedal.style");
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
        element.remove_attribute("reh.enclose");
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
        element.remove_attribute("grid.show");
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
        element.remove_attribute("slur.rend");
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
        element.remove_attribute("stem.mod");
        hasAttribute = true;
    }
    if (element.attribute("stem.with")) {
        this->SetStemWith(StrToStr(element.attribute("stem.with").value()));
        element.remove_attribute("stem.with");
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
        element.remove_attribute("tie.rend");
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
        element.remove_attribute("measperf");
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
    m_bracketPlace = PLACE_NONE;
    m_bracketVisible = BOOLEAN_NONE;
    m_durVisible = BOOLEAN_NONE;
    m_numFormat = NUMFORMAT_NONE;
}

bool AttTupletVis::ReadTupletVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("bracket.place")) {
        this->SetBracketPlace(StrToPlace(element.attribute("bracket.place").value()));
        element.remove_attribute("bracket.place");
        hasAttribute = true;
    }
    if (element.attribute("bracket.visible")) {
        this->SetBracketVisible(StrToBool(element.attribute("bracket.visible").value()));
        element.remove_attribute("bracket.visible");
        hasAttribute = true;
    }
    if (element.attribute("dur.visible")) {
        this->SetDurVisible(StrToBool(element.attribute("dur.visible").value()));
        element.remove_attribute("dur.visible");
        hasAttribute = true;
    }
    if (element.attribute("num.format")) {
        this->SetNumFormat(StrToNumformat(element.attribute("num.format").value()));
        element.remove_attribute("num.format");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTupletVis::WriteTupletVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBracketPlace()) {
        element.append_attribute("bracket.place") = PlaceToStr(this->GetBracketPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBracketVisible()) {
        element.append_attribute("bracket.visible") = BoolToStr(this->GetBracketVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDurVisible()) {
        element.append_attribute("dur.visible") = BoolToStr(this->GetDurVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNumFormat()) {
        element.append_attribute("num.format") = NumformatToStr(this->GetNumFormat()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTupletVis::HasBracketPlace( )
{
    return (m_bracketPlace != PLACE_NONE);
}

bool AttTupletVis::HasBracketVisible( )
{
    return (m_bracketVisible != BOOLEAN_NONE);
}

bool AttTupletVis::HasDurVisible( )
{
    return (m_durVisible != BOOLEAN_NONE);
}

bool AttTupletVis::HasNumFormat( )
{
    return (m_numFormat != NUMFORMAT_NONE);
}


/* include <attnum.format> */

bool Att::SetCmn( Object *element, std::string attrType, std::string attrValue ) {
    if (element->HasAttClass( ATT_ARPEGLOG ) ) {
        AttArpegLog *att = dynamic_cast<AttArpegLog*>(element);
        assert( att );
        if (attrType == "order") {
            att->SetOrder(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_ARPEGVIS ) ) {
        AttArpegVis *att = dynamic_cast<AttArpegVis*>(element);
        assert( att );
        if (attrType == "arrow") {
            att->SetArrow(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BTREMLOG ) ) {
        AttBTremLog *att = dynamic_cast<AttBTremLog*>(element);
        assert( att );
        if (attrType == "form") {
            att->SetForm(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BEAMED ) ) {
        AttBeamed *att = dynamic_cast<AttBeamed*>(element);
        assert( att );
        if (attrType == "beam") {
            att->SetBeam(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BEAMEDWITH ) ) {
        AttBeamedwith *att = dynamic_cast<AttBeamedwith*>(element);
        assert( att );
        if (attrType == "beamWith") {
            att->SetBeamWith(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BEAMINGLOG ) ) {
        AttBeamingLog *att = dynamic_cast<AttBeamingLog*>(element);
        assert( att );
        if (attrType == "beamGroup") {
            att->SetBeamGroup(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "beamRests") {
            att->SetBeamRests(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BEAMREND ) ) {
        AttBeamrend *att = dynamic_cast<AttBeamrend*>(element);
        assert( att );
        if (attrType == "rend") {
            att->SetRend(att->StrToBeatrptRend(attrValue));
            return true;
        }
        if (attrType == "slopeDbl") {
            att->SetSlope(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BEAMSECONDARY ) ) {
        AttBeamsecondary *att = dynamic_cast<AttBeamsecondary*>(element);
        assert( att );
        if (attrType == "breaksecInt") {
            att->SetBreaksec(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BEATRPTVIS ) ) {
        AttBeatRptVis *att = dynamic_cast<AttBeatRptVis*>(element);
        assert( att );
        if (attrType == "rend") {
            att->SetRend(att->StrToBeatrptRend(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BENDGES ) ) {
        AttBendGes *att = dynamic_cast<AttBendGes*>(element);
        assert( att );
        if (attrType == "amount") {
            att->SetAmount(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_CUTOUT ) ) {
        AttCutout *att = dynamic_cast<AttCutout*>(element);
        assert( att );
        if (attrType == "cutout") {
            att->SetCutout(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_EXPANDABLE ) ) {
        AttExpandable *att = dynamic_cast<AttExpandable*>(element);
        assert( att );
        if (attrType == "expand") {
            att->SetExpand(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_FTREMLOG ) ) {
        AttFTremLog *att = dynamic_cast<AttFTremLog*>(element);
        assert( att );
        if (attrType == "form") {
            att->SetForm(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_FERMATAVIS ) ) {
        AttFermataVis *att = dynamic_cast<AttFermataVis*>(element);
        assert( att );
        if (attrType == "form") {
            att->SetForm(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "shape") {
            att->SetShape(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_GLISSVIS ) ) {
        AttGlissVis *att = dynamic_cast<AttGlissVis*>(element);
        assert( att );
        if (attrType == "text") {
            att->SetText(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_GRACED ) ) {
        AttGraced *att = dynamic_cast<AttGraced*>(element);
        assert( att );
        if (attrType == "grace") {
            att->SetGrace(att->StrToGrace(attrValue));
            return true;
        }
        if (attrType == "graceTime") {
            att->SetGraceTime(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_HAIRPINLOG ) ) {
        AttHairpinLog *att = dynamic_cast<AttHairpinLog*>(element);
        assert( att );
        if (attrType == "form") {
            att->SetForm(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_HAIRPINVIS ) ) {
        AttHairpinVis *att = dynamic_cast<AttHairpinVis*>(element);
        assert( att );
        if (attrType == "opening") {
            att->SetOpening(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_HARPPEDALLOG ) ) {
        AttHarpPedalLog *att = dynamic_cast<AttHarpPedalLog*>(element);
        assert( att );
        if (attrType == "c") {
            att->SetC(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "d") {
            att->SetD(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "e") {
            att->SetE(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "f") {
            att->SetF(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "g") {
            att->SetG(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "a") {
            att->SetA(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "b") {
            att->SetB(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_LVPRESENT ) ) {
        AttLvpresent *att = dynamic_cast<AttLvpresent*>(element);
        assert( att );
        if (attrType == "lv") {
            att->SetLv(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_MULTIRESTVIS ) ) {
        AttMultiRestVis *att = dynamic_cast<AttMultiRestVis*>(element);
        assert( att );
        if (attrType == "block") {
            att->SetBlock(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_NOTEGESCMN ) ) {
        AttNoteGesCmn *att = dynamic_cast<AttNoteGesCmn*>(element);
        assert( att );
        if (attrType == "gliss") {
            att->SetGliss(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_NUMBERED ) ) {
        AttNumbered *att = dynamic_cast<AttNumbered*>(element);
        assert( att );
        if (attrType == "numInt") {
            att->SetNum(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_NUMBERPLACEMENT ) ) {
        AttNumberplacement *att = dynamic_cast<AttNumberplacement*>(element);
        assert( att );
        if (attrType == "numPlace") {
            att->SetNumPlace(att->StrToPlace(attrValue));
            return true;
        }
        if (attrType == "numVisible") {
            att->SetNumVisible(att->StrToBool(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_OCTAVELOG ) ) {
        AttOctaveLog *att = dynamic_cast<AttOctaveLog*>(element);
        assert( att );
        if (attrType == "coll") {
            att->SetColl(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_PEDALLOG ) ) {
        AttPedalLog *att = dynamic_cast<AttPedalLog*>(element);
        assert( att );
        if (attrType == "dir") {
            att->SetDir(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_PEDALVIS ) ) {
        AttPedalVis *att = dynamic_cast<AttPedalVis*>(element);
        assert( att );
        if (attrType == "style") {
            att->SetStyle(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_PIANOPEDALS ) ) {
        AttPianopedals *att = dynamic_cast<AttPianopedals*>(element);
        assert( att );
        if (attrType == "pedalStyle") {
            att->SetPedalStyle(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_REHEARSAL ) ) {
        AttRehearsal *att = dynamic_cast<AttRehearsal*>(element);
        assert( att );
        if (attrType == "rehEnclose") {
            att->SetRehEnclose(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_SCOREDEFVISCMN ) ) {
        AttScoreDefVisCmn *att = dynamic_cast<AttScoreDefVisCmn*>(element);
        assert( att );
        if (attrType == "gridShow") {
            att->SetGridShow(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_SLURREND ) ) {
        AttSlurrend *att = dynamic_cast<AttSlurrend*>(element);
        assert( att );
        if (attrType == "slurRend") {
            att->SetSlurRend(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_STEMMEDCMN ) ) {
        AttStemmedCmn *att = dynamic_cast<AttStemmedCmn*>(element);
        assert( att );
        if (attrType == "stemMod") {
            att->SetStemMod(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "stemWith") {
            att->SetStemWith(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_TIEREND ) ) {
        AttTierend *att = dynamic_cast<AttTierend*>(element);
        assert( att );
        if (attrType == "tieRend") {
            att->SetTieRend(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_TREMMEASURED ) ) {
        AttTremmeasured *att = dynamic_cast<AttTremmeasured*>(element);
        assert( att );
        if (attrType == "measperf") {
            att->SetMeasperf(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_TUPLETVIS ) ) {
        AttTupletVis *att = dynamic_cast<AttTupletVis*>(element);
        assert( att );
        if (attrType == "bracketPlace") {
            att->SetBracketPlace(att->StrToPlace(attrValue));
            return true;
        }
        if (attrType == "bracketVisible") {
            att->SetBracketVisible(att->StrToBool(attrValue));
            return true;
        }
        if (attrType == "durVisible") {
            att->SetDurVisible(att->StrToBool(attrValue));
            return true;
        }
        if (attrType == "numFormat") {
            att->SetNumFormat(att->StrToNumformat(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetCmn( Object *element, ArrayOfStrAttr *attributes ) {
    if (element->HasAttClass( ATT_ARPEGLOG ) ) {
        AttArpegLog *att = dynamic_cast<AttArpegLog*>(element);
        assert( att );
        if (att->HasOrder()) {
            attributes->push_back(std::make_pair("order", att->StrToStr(att->GetOrder())));
        }
    }
    if (element->HasAttClass( ATT_ARPEGVIS ) ) {
        AttArpegVis *att = dynamic_cast<AttArpegVis*>(element);
        assert( att );
        if (att->HasArrow()) {
            attributes->push_back(std::make_pair("arrow", att->StrToStr(att->GetArrow())));
        }
    }
    if (element->HasAttClass( ATT_BTREMLOG ) ) {
        AttBTremLog *att = dynamic_cast<AttBTremLog*>(element);
        assert( att );
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->StrToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass( ATT_BEAMED ) ) {
        AttBeamed *att = dynamic_cast<AttBeamed*>(element);
        assert( att );
        if (att->HasBeam()) {
            attributes->push_back(std::make_pair("beam", att->StrToStr(att->GetBeam())));
        }
    }
    if (element->HasAttClass( ATT_BEAMEDWITH ) ) {
        AttBeamedwith *att = dynamic_cast<AttBeamedwith*>(element);
        assert( att );
        if (att->HasBeamWith()) {
            attributes->push_back(std::make_pair("beamWith", att->StrToStr(att->GetBeamWith())));
        }
    }
    if (element->HasAttClass( ATT_BEAMINGLOG ) ) {
        AttBeamingLog *att = dynamic_cast<AttBeamingLog*>(element);
        assert( att );
        if (att->HasBeamGroup()) {
            attributes->push_back(std::make_pair("beamGroup", att->StrToStr(att->GetBeamGroup())));
        }
        if (att->HasBeamRests()) {
            attributes->push_back(std::make_pair("beamRests", att->StrToStr(att->GetBeamRests())));
        }
    }
    if (element->HasAttClass( ATT_BEAMREND ) ) {
        AttBeamrend *att = dynamic_cast<AttBeamrend*>(element);
        assert( att );
        if (att->HasRend()) {
            attributes->push_back(std::make_pair("rend", att->BeatrptRendToStr(att->GetRend())));
        }
        if (att->HasSlope()) {
            attributes->push_back(std::make_pair("slopeDbl", att->DblToStr(att->GetSlope())));
        }
    }
    if (element->HasAttClass( ATT_BEAMSECONDARY ) ) {
        AttBeamsecondary *att = dynamic_cast<AttBeamsecondary*>(element);
        assert( att );
        if (att->HasBreaksec()) {
            attributes->push_back(std::make_pair("breaksecInt", att->IntToStr(att->GetBreaksec())));
        }
    }
    if (element->HasAttClass( ATT_BEATRPTVIS ) ) {
        AttBeatRptVis *att = dynamic_cast<AttBeatRptVis*>(element);
        assert( att );
        if (att->HasRend()) {
            attributes->push_back(std::make_pair("rend", att->BeatrptRendToStr(att->GetRend())));
        }
    }
    if (element->HasAttClass( ATT_BENDGES ) ) {
        AttBendGes *att = dynamic_cast<AttBendGes*>(element);
        assert( att );
        if (att->HasAmount()) {
            attributes->push_back(std::make_pair("amount", att->StrToStr(att->GetAmount())));
        }
    }
    if (element->HasAttClass( ATT_CUTOUT ) ) {
        AttCutout *att = dynamic_cast<AttCutout*>(element);
        assert( att );
        if (att->HasCutout()) {
            attributes->push_back(std::make_pair("cutout", att->StrToStr(att->GetCutout())));
        }
    }
    if (element->HasAttClass( ATT_EXPANDABLE ) ) {
        AttExpandable *att = dynamic_cast<AttExpandable*>(element);
        assert( att );
        if (att->HasExpand()) {
            attributes->push_back(std::make_pair("expand", att->StrToStr(att->GetExpand())));
        }
    }
    if (element->HasAttClass( ATT_FTREMLOG ) ) {
        AttFTremLog *att = dynamic_cast<AttFTremLog*>(element);
        assert( att );
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->StrToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass( ATT_FERMATAVIS ) ) {
        AttFermataVis *att = dynamic_cast<AttFermataVis*>(element);
        assert( att );
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->StrToStr(att->GetForm())));
        }
        if (att->HasShape()) {
            attributes->push_back(std::make_pair("shape", att->StrToStr(att->GetShape())));
        }
    }
    if (element->HasAttClass( ATT_GLISSVIS ) ) {
        AttGlissVis *att = dynamic_cast<AttGlissVis*>(element);
        assert( att );
        if (att->HasText()) {
            attributes->push_back(std::make_pair("text", att->StrToStr(att->GetText())));
        }
    }
    if (element->HasAttClass( ATT_GRACED ) ) {
        AttGraced *att = dynamic_cast<AttGraced*>(element);
        assert( att );
        if (att->HasGrace()) {
            attributes->push_back(std::make_pair("grace", att->GraceToStr(att->GetGrace())));
        }
        if (att->HasGraceTime()) {
            attributes->push_back(std::make_pair("graceTime", att->StrToStr(att->GetGraceTime())));
        }
    }
    if (element->HasAttClass( ATT_HAIRPINLOG ) ) {
        AttHairpinLog *att = dynamic_cast<AttHairpinLog*>(element);
        assert( att );
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->StrToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass( ATT_HAIRPINVIS ) ) {
        AttHairpinVis *att = dynamic_cast<AttHairpinVis*>(element);
        assert( att );
        if (att->HasOpening()) {
            attributes->push_back(std::make_pair("opening", att->StrToStr(att->GetOpening())));
        }
    }
    if (element->HasAttClass( ATT_HARPPEDALLOG ) ) {
        AttHarpPedalLog *att = dynamic_cast<AttHarpPedalLog*>(element);
        assert( att );
        if (att->HasC()) {
            attributes->push_back(std::make_pair("c", att->StrToStr(att->GetC())));
        }
        if (att->HasD()) {
            attributes->push_back(std::make_pair("d", att->StrToStr(att->GetD())));
        }
        if (att->HasE()) {
            attributes->push_back(std::make_pair("e", att->StrToStr(att->GetE())));
        }
        if (att->HasF()) {
            attributes->push_back(std::make_pair("f", att->StrToStr(att->GetF())));
        }
        if (att->HasG()) {
            attributes->push_back(std::make_pair("g", att->StrToStr(att->GetG())));
        }
        if (att->HasA()) {
            attributes->push_back(std::make_pair("a", att->StrToStr(att->GetA())));
        }
        if (att->HasB()) {
            attributes->push_back(std::make_pair("b", att->StrToStr(att->GetB())));
        }
    }
    if (element->HasAttClass( ATT_LVPRESENT ) ) {
        AttLvpresent *att = dynamic_cast<AttLvpresent*>(element);
        assert( att );
        if (att->HasLv()) {
            attributes->push_back(std::make_pair("lv", att->StrToStr(att->GetLv())));
        }
    }
    if (element->HasAttClass( ATT_MULTIRESTVIS ) ) {
        AttMultiRestVis *att = dynamic_cast<AttMultiRestVis*>(element);
        assert( att );
        if (att->HasBlock()) {
            attributes->push_back(std::make_pair("block", att->StrToStr(att->GetBlock())));
        }
    }
    if (element->HasAttClass( ATT_NOTEGESCMN ) ) {
        AttNoteGesCmn *att = dynamic_cast<AttNoteGesCmn*>(element);
        assert( att );
        if (att->HasGliss()) {
            attributes->push_back(std::make_pair("gliss", att->StrToStr(att->GetGliss())));
        }
    }
    if (element->HasAttClass( ATT_NUMBERED ) ) {
        AttNumbered *att = dynamic_cast<AttNumbered*>(element);
        assert( att );
        if (att->HasNum()) {
            attributes->push_back(std::make_pair("numInt", att->IntToStr(att->GetNum())));
        }
    }
    if (element->HasAttClass( ATT_NUMBERPLACEMENT ) ) {
        AttNumberplacement *att = dynamic_cast<AttNumberplacement*>(element);
        assert( att );
        if (att->HasNumPlace()) {
            attributes->push_back(std::make_pair("numPlace", att->PlaceToStr(att->GetNumPlace())));
        }
        if (att->HasNumVisible()) {
            attributes->push_back(std::make_pair("numVisible", att->BoolToStr(att->GetNumVisible())));
        }
    }
    if (element->HasAttClass( ATT_OCTAVELOG ) ) {
        AttOctaveLog *att = dynamic_cast<AttOctaveLog*>(element);
        assert( att );
        if (att->HasColl()) {
            attributes->push_back(std::make_pair("coll", att->StrToStr(att->GetColl())));
        }
    }
    if (element->HasAttClass( ATT_PEDALLOG ) ) {
        AttPedalLog *att = dynamic_cast<AttPedalLog*>(element);
        assert( att );
        if (att->HasDir()) {
            attributes->push_back(std::make_pair("dir", att->StrToStr(att->GetDir())));
        }
    }
    if (element->HasAttClass( ATT_PEDALVIS ) ) {
        AttPedalVis *att = dynamic_cast<AttPedalVis*>(element);
        assert( att );
        if (att->HasStyle()) {
            attributes->push_back(std::make_pair("style", att->StrToStr(att->GetStyle())));
        }
    }
    if (element->HasAttClass( ATT_PIANOPEDALS ) ) {
        AttPianopedals *att = dynamic_cast<AttPianopedals*>(element);
        assert( att );
        if (att->HasPedalStyle()) {
            attributes->push_back(std::make_pair("pedalStyle", att->StrToStr(att->GetPedalStyle())));
        }
    }
    if (element->HasAttClass( ATT_REHEARSAL ) ) {
        AttRehearsal *att = dynamic_cast<AttRehearsal*>(element);
        assert( att );
        if (att->HasRehEnclose()) {
            attributes->push_back(std::make_pair("rehEnclose", att->StrToStr(att->GetRehEnclose())));
        }
    }
    if (element->HasAttClass( ATT_SCOREDEFVISCMN ) ) {
        AttScoreDefVisCmn *att = dynamic_cast<AttScoreDefVisCmn*>(element);
        assert( att );
        if (att->HasGridShow()) {
            attributes->push_back(std::make_pair("gridShow", att->StrToStr(att->GetGridShow())));
        }
    }
    if (element->HasAttClass( ATT_SLURREND ) ) {
        AttSlurrend *att = dynamic_cast<AttSlurrend*>(element);
        assert( att );
        if (att->HasSlurRend()) {
            attributes->push_back(std::make_pair("slurRend", att->StrToStr(att->GetSlurRend())));
        }
    }
    if (element->HasAttClass( ATT_STEMMEDCMN ) ) {
        AttStemmedCmn *att = dynamic_cast<AttStemmedCmn*>(element);
        assert( att );
        if (att->HasStemMod()) {
            attributes->push_back(std::make_pair("stemMod", att->StrToStr(att->GetStemMod())));
        }
        if (att->HasStemWith()) {
            attributes->push_back(std::make_pair("stemWith", att->StrToStr(att->GetStemWith())));
        }
    }
    if (element->HasAttClass( ATT_TIEREND ) ) {
        AttTierend *att = dynamic_cast<AttTierend*>(element);
        assert( att );
        if (att->HasTieRend()) {
            attributes->push_back(std::make_pair("tieRend", att->StrToStr(att->GetTieRend())));
        }
    }
    if (element->HasAttClass( ATT_TREMMEASURED ) ) {
        AttTremmeasured *att = dynamic_cast<AttTremmeasured*>(element);
        assert( att );
        if (att->HasMeasperf()) {
            attributes->push_back(std::make_pair("measperf", att->StrToStr(att->GetMeasperf())));
        }
    }
    if (element->HasAttClass( ATT_TUPLETVIS ) ) {
        AttTupletVis *att = dynamic_cast<AttTupletVis*>(element);
        assert( att );
        if (att->HasBracketPlace()) {
            attributes->push_back(std::make_pair("bracketPlace", att->PlaceToStr(att->GetBracketPlace())));
        }
        if (att->HasBracketVisible()) {
            attributes->push_back(std::make_pair("bracketVisible", att->BoolToStr(att->GetBracketVisible())));
        }
        if (att->HasDurVisible()) {
            attributes->push_back(std::make_pair("durVisible", att->BoolToStr(att->GetDurVisible())));
        }
        if (att->HasNumFormat()) {
            attributes->push_back(std::make_pair("numFormat", att->NumformatToStr(att->GetNumFormat())));
        }
    }

}
    
} // vrv namespace
    
