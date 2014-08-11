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
    if (this->GetOrder() == "") {
        element.append_attribute("order") = StrToStr(this->GetOrder()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetArrow() == "") {
        element.append_attribute("arrow") = StrToStr(this->GetArrow()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetForm() == "") {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetBeam() == "") {
        element.append_attribute("beam") = StrToStr(this->GetBeam()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetBeamWith() == "") {
        element.append_attribute("beam.with") = StrToStr(this->GetBeamWith()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetBeamGroup() == "") {
        element.append_attribute("beam.group") = StrToStr(this->GetBeamGroup()).c_str();
        wroteAttribute = true;
    }
    if (this->GetBeamRests() == "") {
        element.append_attribute("beam.rests") = StrToStr(this->GetBeamRests()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
        this->SetSlopeDbl(StrToDbl(element.attribute("slope").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamrend::WriteBeamrend(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetRend() == "") {
        element.append_attribute("rend") = StrToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSlopeDbl() == 0.0) {
        element.append_attribute("slope") = DblToStr(this->GetSlopeDbl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
        this->SetBreaksecInt(StrToInt(element.attribute("breaksec").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamsecondary::WriteBeamsecondary(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetBreaksecInt() == 0) {
        element.append_attribute("breaksec") = IntToStr(this->GetBreaksecInt()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetRend() == "") {
        element.append_attribute("rend") = StrToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetAmount() == "") {
        element.append_attribute("amount") = StrToStr(this->GetAmount()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetCutout() == "") {
        element.append_attribute("cutout") = StrToStr(this->GetCutout()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetExpand() == "") {
        element.append_attribute("expand") = StrToStr(this->GetExpand()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetForm() == "") {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetForm() == "") {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->GetShape() == "") {
        element.append_attribute("shape") = StrToStr(this->GetShape()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetText() == "") {
        element.append_attribute("text") = StrToStr(this->GetText()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetGrace() == "") {
        element.append_attribute("grace") = StrToStr(this->GetGrace()).c_str();
        wroteAttribute = true;
    }
    if (this->GetGraceTime() == "") {
        element.append_attribute("grace.time") = StrToStr(this->GetGraceTime()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetForm() == "") {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetOpening() == "") {
        element.append_attribute("opening") = StrToStr(this->GetOpening()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetC() == "") {
        element.append_attribute("c") = StrToStr(this->GetC()).c_str();
        wroteAttribute = true;
    }
    if (this->GetD() == "") {
        element.append_attribute("d") = StrToStr(this->GetD()).c_str();
        wroteAttribute = true;
    }
    if (this->GetE() == "") {
        element.append_attribute("e") = StrToStr(this->GetE()).c_str();
        wroteAttribute = true;
    }
    if (this->GetF() == "") {
        element.append_attribute("f") = StrToStr(this->GetF()).c_str();
        wroteAttribute = true;
    }
    if (this->GetG() == "") {
        element.append_attribute("g") = StrToStr(this->GetG()).c_str();
        wroteAttribute = true;
    }
    if (this->GetA() == "") {
        element.append_attribute("a") = StrToStr(this->GetA()).c_str();
        wroteAttribute = true;
    }
    if (this->GetB() == "") {
        element.append_attribute("b") = StrToStr(this->GetB()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetLv() == "") {
        element.append_attribute("lv") = StrToStr(this->GetLv()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetBlock() == "") {
        element.append_attribute("block") = StrToStr(this->GetBlock()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetGliss() == "") {
        element.append_attribute("gliss") = StrToStr(this->GetGliss()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
        this->SetNumInt(StrToInt(element.attribute("num").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNumbered::WriteNumbered(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetNumInt() == 0) {
        element.append_attribute("num") = IntToStr(this->GetNumInt()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetNumPlace() == "") {
        element.append_attribute("num.place") = StrToStr(this->GetNumPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->GetNumVisible() == "") {
        element.append_attribute("num.visible") = StrToStr(this->GetNumVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetColl() == "") {
        element.append_attribute("coll") = StrToStr(this->GetColl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetDir() == "") {
        element.append_attribute("dir") = StrToStr(this->GetDir()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetStyle() == "") {
        element.append_attribute("style") = StrToStr(this->GetStyle()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetPedalStyle() == "") {
        element.append_attribute("pedal.style") = StrToStr(this->GetPedalStyle()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetRehEnclose() == "") {
        element.append_attribute("reh.enclose") = StrToStr(this->GetRehEnclose()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetGridShow() == "") {
        element.append_attribute("grid.show") = StrToStr(this->GetGridShow()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetSlurRend() == "") {
        element.append_attribute("slur.rend") = StrToStr(this->GetSlurRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetStemMod() == "") {
        element.append_attribute("stem.mod") = StrToStr(this->GetStemMod()).c_str();
        wroteAttribute = true;
    }
    if (this->GetStemWith() == "") {
        element.append_attribute("stem.with") = StrToStr(this->GetStemWith()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetTieRend() == "") {
        element.append_attribute("tie.rend") = StrToStr(this->GetTieRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
    if (this->GetMeasperf() == "") {
        element.append_attribute("measperf") = StrToStr(this->GetMeasperf()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmeasperf> */

//----------------------------------------------------------------------------
// AttTupletLog
//----------------------------------------------------------------------------

AttTupletLog::AttTupletLog(): Att() {
    ResetTupletLog();
}

AttTupletLog::~AttTupletLog() {

}

void AttTupletLog::ResetTupletLog() {
    m_dur = "";
}

bool AttTupletLog::ReadTupletLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dur")) {
        this->SetDur(StrToStr(element.attribute("dur").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTupletLog::WriteTupletLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDur() == "") {
        element.append_attribute("dur") = StrToStr(this->GetDur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdur> */

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
    if (this->GetBracketPlace() == "") {
        element.append_attribute("bracket.place") = StrToStr(this->GetBracketPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->GetBracketVisible() == "") {
        element.append_attribute("bracket.visible") = StrToStr(this->GetBracketVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->GetDurVisible() == "") {
        element.append_attribute("dur.visible") = StrToStr(this->GetDurVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->GetNumFormat() == "") {
        element.append_attribute("num.format") = StrToStr(this->GetNumFormat()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attnum.format> */

//----------------------------------------------------------------------------
// AttTupletSpanLog
//----------------------------------------------------------------------------

AttTupletSpanLog::AttTupletSpanLog(): Att() {
    ResetTupletSpanLog();
}

AttTupletSpanLog::~AttTupletSpanLog() {

}

void AttTupletSpanLog::ResetTupletSpanLog() {
    m_dur = "";
}

bool AttTupletSpanLog::ReadTupletSpanLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dur")) {
        this->SetDur(StrToStr(element.attribute("dur").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTupletSpanLog::WriteTupletSpanLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDur() == "") {
        element.append_attribute("dur") = StrToStr(this->GetDur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdur> */

} // vrv namespace

