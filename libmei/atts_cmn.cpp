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
    m_order = arpegLog_ORDER_NONE;
}

bool AttArpegLog::ReadArpegLog( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("order")) {
        this->SetOrder(StrToArpegLogOrder(element.attribute("order").value()));
        element.remove_attribute("order");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArpegLog::WriteArpegLog( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasOrder()) {
        element.append_attribute("order") = ArpegLogOrderToStr(this->GetOrder()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttArpegLog::HasOrder( )
{
    return (m_order != arpegLog_ORDER_NONE);
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
    m_arrow = BOOLEAN_NONE;
}

bool AttArpegVis::ReadArpegVis( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("arrow")) {
        this->SetArrow(StrToBoolean(element.attribute("arrow").value()));
        element.remove_attribute("arrow");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArpegVis::WriteArpegVis( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasArrow()) {
        element.append_attribute("arrow") = BooleanToStr(this->GetArrow()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttArpegVis::HasArrow( )
{
    return (m_arrow != BOOLEAN_NONE);
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
    m_form = bTremLog_FORM_NONE;
}

bool AttBTremLog::ReadBTremLog( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToBTremLogForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBTremLog::WriteBTremLog( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = BTremLogFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBTremLog::HasForm( )
{
    return (m_form != bTremLog_FORM_NONE);
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

bool AttBeamed::ReadBeamed( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("beam")) {
        this->SetBeam(StrToStr(element.attribute("beam").value()));
        element.remove_attribute("beam");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamed::WriteBeamed( pugi::xml_node element ) {
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
    m_beamWith = OTHERSTAFF_NONE;
}

bool AttBeamedwith::ReadBeamedwith( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("beam.with")) {
        this->SetBeamWith(StrToOtherstaff(element.attribute("beam.with").value()));
        element.remove_attribute("beam.with");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamedwith::WriteBeamedwith( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBeamWith()) {
        element.append_attribute("beam.with") = OtherstaffToStr(this->GetBeamWith()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamedwith::HasBeamWith( )
{
    return (m_beamWith != OTHERSTAFF_NONE);
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
    m_beamRests = BOOLEAN_NONE;
}

bool AttBeamingLog::ReadBeamingLog( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("beam.group")) {
        this->SetBeamGroup(StrToStr(element.attribute("beam.group").value()));
        element.remove_attribute("beam.group");
        hasAttribute = true;
    }
    if (element.attribute("beam.rests")) {
        this->SetBeamRests(StrToBoolean(element.attribute("beam.rests").value()));
        element.remove_attribute("beam.rests");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamingLog::WriteBeamingLog( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBeamGroup()) {
        element.append_attribute("beam.group") = StrToStr(this->GetBeamGroup()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBeamRests()) {
        element.append_attribute("beam.rests") = BooleanToStr(this->GetBeamRests()).c_str();
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
    return (m_beamRests != BOOLEAN_NONE);
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
    m_form = beamrend_FORM_NONE;
    m_slope = 0.0;
}

bool AttBeamrend::ReadBeamrend( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToBeamrendForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("slope")) {
        this->SetSlope(StrToDbl(element.attribute("slope").value()));
        element.remove_attribute("slope");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamrend::WriteBeamrend( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = BeamrendFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSlope()) {
        element.append_attribute("slope") = DblToStr(this->GetSlope()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamrend::HasForm( )
{
    return (m_form != beamrend_FORM_NONE);
}

bool AttBeamrend::HasSlope( )
{
    return (m_slope != 0.0);
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
    m_breaksec = 0;
}

bool AttBeamsecondary::ReadBeamsecondary( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("breaksec")) {
        this->SetBreaksec(StrToInt(element.attribute("breaksec").value()));
        element.remove_attribute("breaksec");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamsecondary::WriteBeamsecondary( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBreaksec()) {
        element.append_attribute("breaksec") = IntToStr(this->GetBreaksec()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamsecondary::HasBreaksec( )
{
    return (m_breaksec != 0);
}


/* include <attbreaksec> */

//----------------------------------------------------------------------------
// AttBeatRptLog
//----------------------------------------------------------------------------

AttBeatRptLog::AttBeatRptLog(): Att() {
    ResetBeatRptLog();
}

AttBeatRptLog::~AttBeatRptLog() {

}

void AttBeatRptLog::ResetBeatRptLog() {
    m_beatDef = DURATION_NONE;
}

bool AttBeatRptLog::ReadBeatRptLog( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("beatDef")) {
        this->SetBeatDef(StrToDuration(element.attribute("beatDef").value()));
        element.remove_attribute("beatDef");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeatRptLog::WriteBeatRptLog( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBeatDef()) {
        element.append_attribute("beatDef") = DurationToStr(this->GetBeatDef()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeatRptLog::HasBeatDef( )
{
    return (m_beatDef != DURATION_NONE);
}


/* include <attbeatDef> */

//----------------------------------------------------------------------------
// AttBeatRptVis
//----------------------------------------------------------------------------

AttBeatRptVis::AttBeatRptVis(): Att() {
    ResetBeatRptVis();
}

AttBeatRptVis::~AttBeatRptVis() {

}

void AttBeatRptVis::ResetBeatRptVis() {
    m_form = BEATRPT_REND_NONE;
}

bool AttBeatRptVis::ReadBeatRptVis( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToBeatrptRend(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeatRptVis::WriteBeatRptVis( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = BeatrptRendToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeatRptVis::HasForm( )
{
    return (m_form != BEATRPT_REND_NONE);
}


/* include <attform> */

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

bool AttBendGes::ReadBendGes( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("amount")) {
        this->SetAmount(StrToStr(element.attribute("amount").value()));
        element.remove_attribute("amount");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBendGes::WriteBendGes( pugi::xml_node element ) {
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
    m_cutout = cutout_CUTOUT_NONE;
}

bool AttCutout::ReadCutout( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("cutout")) {
        this->SetCutout(StrToCutoutCutout(element.attribute("cutout").value()));
        element.remove_attribute("cutout");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCutout::WriteCutout( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasCutout()) {
        element.append_attribute("cutout") = CutoutCutoutToStr(this->GetCutout()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCutout::HasCutout( )
{
    return (m_cutout != cutout_CUTOUT_NONE);
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
    m_expand = BOOLEAN_NONE;
}

bool AttExpandable::ReadExpandable( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("expand")) {
        this->SetExpand(StrToBoolean(element.attribute("expand").value()));
        element.remove_attribute("expand");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttExpandable::WriteExpandable( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasExpand()) {
        element.append_attribute("expand") = BooleanToStr(this->GetExpand()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttExpandable::HasExpand( )
{
    return (m_expand != BOOLEAN_NONE);
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
    m_form = fTremLog_FORM_NONE;
}

bool AttFTremLog::ReadFTremLog( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToFTremLogForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFTremLog::WriteFTremLog( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = FTremLogFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFTremLog::HasForm( )
{
    return (m_form != fTremLog_FORM_NONE);
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
    m_form = fermataVis_FORM_NONE;
    m_shape = fermataVis_SHAPE_NONE;
}

bool AttFermataVis::ReadFermataVis( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToFermataVisForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("shape")) {
        this->SetShape(StrToFermataVisShape(element.attribute("shape").value()));
        element.remove_attribute("shape");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFermataVis::WriteFermataVis( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = FermataVisFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasShape()) {
        element.append_attribute("shape") = FermataVisShapeToStr(this->GetShape()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFermataVis::HasForm( )
{
    return (m_form != fermataVis_FORM_NONE);
}

bool AttFermataVis::HasShape( )
{
    return (m_shape != fermataVis_SHAPE_NONE);
}


/* include <attshape> */

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
    m_graceTime = 0;
}

bool AttGraced::ReadGraced( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("grace")) {
        this->SetGrace(StrToGrace(element.attribute("grace").value()));
        element.remove_attribute("grace");
        hasAttribute = true;
    }
    if (element.attribute("grace.time")) {
        this->SetGraceTime(StrToInt(element.attribute("grace.time").value()));
        element.remove_attribute("grace.time");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttGraced::WriteGraced( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasGrace()) {
        element.append_attribute("grace") = GraceToStr(this->GetGrace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasGraceTime()) {
        element.append_attribute("grace.time") = IntToStr(this->GetGraceTime()).c_str();
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
    return (m_graceTime != 0);
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
    m_form = hairpinLog_FORM_NONE;
    m_niente = BOOLEAN_NONE;
}

bool AttHairpinLog::ReadHairpinLog( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToHairpinLogForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("niente")) {
        this->SetNiente(StrToBoolean(element.attribute("niente").value()));
        element.remove_attribute("niente");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHairpinLog::WriteHairpinLog( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = HairpinLogFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNiente()) {
        element.append_attribute("niente") = BooleanToStr(this->GetNiente()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHairpinLog::HasForm( )
{
    return (m_form != hairpinLog_FORM_NONE);
}

bool AttHairpinLog::HasNiente( )
{
    return (m_niente != BOOLEAN_NONE);
}


/* include <attniente> */

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

bool AttHairpinVis::ReadHairpinVis( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("opening")) {
        this->SetOpening(StrToStr(element.attribute("opening").value()));
        element.remove_attribute("opening");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHairpinVis::WriteHairpinVis( pugi::xml_node element ) {
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
    m_c = harpPedalLog_C_NONE;
    m_d = harpPedalLog_D_NONE;
    m_e = harpPedalLog_E_NONE;
    m_f = harpPedalLog_F_NONE;
    m_g = harpPedalLog_G_NONE;
    m_a = harpPedalLog_A_NONE;
    m_b = harpPedalLog_B_NONE;
}

bool AttHarpPedalLog::ReadHarpPedalLog( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("c")) {
        this->SetC(StrToHarpPedalLogC(element.attribute("c").value()));
        element.remove_attribute("c");
        hasAttribute = true;
    }
    if (element.attribute("d")) {
        this->SetD(StrToHarpPedalLogD(element.attribute("d").value()));
        element.remove_attribute("d");
        hasAttribute = true;
    }
    if (element.attribute("e")) {
        this->SetE(StrToHarpPedalLogE(element.attribute("e").value()));
        element.remove_attribute("e");
        hasAttribute = true;
    }
    if (element.attribute("f")) {
        this->SetF(StrToHarpPedalLogF(element.attribute("f").value()));
        element.remove_attribute("f");
        hasAttribute = true;
    }
    if (element.attribute("g")) {
        this->SetG(StrToHarpPedalLogG(element.attribute("g").value()));
        element.remove_attribute("g");
        hasAttribute = true;
    }
    if (element.attribute("a")) {
        this->SetA(StrToHarpPedalLogA(element.attribute("a").value()));
        element.remove_attribute("a");
        hasAttribute = true;
    }
    if (element.attribute("b")) {
        this->SetB(StrToHarpPedalLogB(element.attribute("b").value()));
        element.remove_attribute("b");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarpPedalLog::WriteHarpPedalLog( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasC()) {
        element.append_attribute("c") = HarpPedalLogCToStr(this->GetC()).c_str();
        wroteAttribute = true;
    }
    if (this->HasD()) {
        element.append_attribute("d") = HarpPedalLogDToStr(this->GetD()).c_str();
        wroteAttribute = true;
    }
    if (this->HasE()) {
        element.append_attribute("e") = HarpPedalLogEToStr(this->GetE()).c_str();
        wroteAttribute = true;
    }
    if (this->HasF()) {
        element.append_attribute("f") = HarpPedalLogFToStr(this->GetF()).c_str();
        wroteAttribute = true;
    }
    if (this->HasG()) {
        element.append_attribute("g") = HarpPedalLogGToStr(this->GetG()).c_str();
        wroteAttribute = true;
    }
    if (this->HasA()) {
        element.append_attribute("a") = HarpPedalLogAToStr(this->GetA()).c_str();
        wroteAttribute = true;
    }
    if (this->HasB()) {
        element.append_attribute("b") = HarpPedalLogBToStr(this->GetB()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarpPedalLog::HasC( )
{
    return (m_c != harpPedalLog_C_NONE);
}

bool AttHarpPedalLog::HasD( )
{
    return (m_d != harpPedalLog_D_NONE);
}

bool AttHarpPedalLog::HasE( )
{
    return (m_e != harpPedalLog_E_NONE);
}

bool AttHarpPedalLog::HasF( )
{
    return (m_f != harpPedalLog_F_NONE);
}

bool AttHarpPedalLog::HasG( )
{
    return (m_g != harpPedalLog_G_NONE);
}

bool AttHarpPedalLog::HasA( )
{
    return (m_a != harpPedalLog_A_NONE);
}

bool AttHarpPedalLog::HasB( )
{
    return (m_b != harpPedalLog_B_NONE);
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
    m_lv = BOOLEAN_NONE;
}

bool AttLvpresent::ReadLvpresent( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("lv")) {
        this->SetLv(StrToBoolean(element.attribute("lv").value()));
        element.remove_attribute("lv");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLvpresent::WriteLvpresent( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasLv()) {
        element.append_attribute("lv") = BooleanToStr(this->GetLv()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLvpresent::HasLv( )
{
    return (m_lv != BOOLEAN_NONE);
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
    m_block = BOOLEAN_NONE;
}

bool AttMultiRestVis::ReadMultiRestVis( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("block")) {
        this->SetBlock(StrToBoolean(element.attribute("block").value()));
        element.remove_attribute("block");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMultiRestVis::WriteMultiRestVis( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBlock()) {
        element.append_attribute("block") = BooleanToStr(this->GetBlock()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMultiRestVis::HasBlock( )
{
    return (m_block != BOOLEAN_NONE);
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
    m_gliss = GLISSANDO_NONE;
}

bool AttNoteGesCmn::ReadNoteGesCmn( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("gliss")) {
        this->SetGliss(StrToGlissando(element.attribute("gliss").value()));
        element.remove_attribute("gliss");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteGesCmn::WriteNoteGesCmn( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasGliss()) {
        element.append_attribute("gliss") = GlissandoToStr(this->GetGliss()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNoteGesCmn::HasGliss( )
{
    return (m_gliss != GLISSANDO_NONE);
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
    m_num = 0;
}

bool AttNumbered::ReadNumbered( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNum(StrToInt(element.attribute("num").value()));
        element.remove_attribute("num");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNumbered::WriteNumbered( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasNum()) {
        element.append_attribute("num") = IntToStr(this->GetNum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNumbered::HasNum( )
{
    return (m_num != 0);
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

bool AttNumberplacement::ReadNumberplacement( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("num.place")) {
        this->SetNumPlace(StrToPlace(element.attribute("num.place").value()));
        element.remove_attribute("num.place");
        hasAttribute = true;
    }
    if (element.attribute("num.visible")) {
        this->SetNumVisible(StrToBoolean(element.attribute("num.visible").value()));
        element.remove_attribute("num.visible");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNumberplacement::WriteNumberplacement( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasNumPlace()) {
        element.append_attribute("num.place") = PlaceToStr(this->GetNumPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNumVisible()) {
        element.append_attribute("num.visible") = BooleanToStr(this->GetNumVisible()).c_str();
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
    m_coll = octaveLog_COLL_NONE;
}

bool AttOctaveLog::ReadOctaveLog( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("coll")) {
        this->SetColl(StrToOctaveLogColl(element.attribute("coll").value()));
        element.remove_attribute("coll");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctaveLog::WriteOctaveLog( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasColl()) {
        element.append_attribute("coll") = OctaveLogCollToStr(this->GetColl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOctaveLog::HasColl( )
{
    return (m_coll != octaveLog_COLL_NONE);
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
    m_dir = pedalLog_DIR_NONE;
}

bool AttPedalLog::ReadPedalLog( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dir")) {
        this->SetDir(StrToPedalLogDir(element.attribute("dir").value()));
        element.remove_attribute("dir");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPedalLog::WritePedalLog( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDir()) {
        element.append_attribute("dir") = PedalLogDirToStr(this->GetDir()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPedalLog::HasDir( )
{
    return (m_dir != pedalLog_DIR_NONE);
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
    m_form = pedalVis_FORM_NONE;
}

bool AttPedalVis::ReadPedalVis( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToPedalVisForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPedalVis::WritePedalVis( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = PedalVisFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPedalVis::HasForm( )
{
    return (m_form != pedalVis_FORM_NONE);
}


/* include <attform> */

//----------------------------------------------------------------------------
// AttPianopedals
//----------------------------------------------------------------------------

AttPianopedals::AttPianopedals(): Att() {
    ResetPianopedals();
}

AttPianopedals::~AttPianopedals() {

}

void AttPianopedals::ResetPianopedals() {
    m_pedalStyle = pianopedals_PEDALSTYLE_NONE;
}

bool AttPianopedals::ReadPianopedals( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("pedal.style")) {
        this->SetPedalStyle(StrToPianopedalsPedalstyle(element.attribute("pedal.style").value()));
        element.remove_attribute("pedal.style");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPianopedals::WritePianopedals( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasPedalStyle()) {
        element.append_attribute("pedal.style") = PianopedalsPedalstyleToStr(this->GetPedalStyle()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPianopedals::HasPedalStyle( )
{
    return (m_pedalStyle != pianopedals_PEDALSTYLE_NONE);
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
    m_rehEnclose = rehearsal_REHENCLOSE_NONE;
}

bool AttRehearsal::ReadRehearsal( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("reh.enclose")) {
        this->SetRehEnclose(StrToRehearsalRehenclose(element.attribute("reh.enclose").value()));
        element.remove_attribute("reh.enclose");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRehearsal::WriteRehearsal( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRehEnclose()) {
        element.append_attribute("reh.enclose") = RehearsalRehencloseToStr(this->GetRehEnclose()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRehearsal::HasRehEnclose( )
{
    return (m_rehEnclose != rehearsal_REHENCLOSE_NONE);
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
    m_gridShow = BOOLEAN_NONE;
}

bool AttScoreDefVisCmn::ReadScoreDefVisCmn( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("grid.show")) {
        this->SetGridShow(StrToBoolean(element.attribute("grid.show").value()));
        element.remove_attribute("grid.show");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScoreDefVisCmn::WriteScoreDefVisCmn( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasGridShow()) {
        element.append_attribute("grid.show") = BooleanToStr(this->GetGridShow()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttScoreDefVisCmn::HasGridShow( )
{
    return (m_gridShow != BOOLEAN_NONE);
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
    m_slurRend = CURVERENDITION_NONE;
}

bool AttSlurrend::ReadSlurrend( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("slur.rend")) {
        this->SetSlurRend(StrToCurverendition(element.attribute("slur.rend").value()));
        element.remove_attribute("slur.rend");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSlurrend::WriteSlurrend( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasSlurRend()) {
        element.append_attribute("slur.rend") = CurverenditionToStr(this->GetSlurRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSlurrend::HasSlurRend( )
{
    return (m_slurRend != CURVERENDITION_NONE);
}


/* include <attslur.rend> */

//----------------------------------------------------------------------------
// AttStemsCmn
//----------------------------------------------------------------------------

AttStemsCmn::AttStemsCmn(): Att() {
    ResetStemsCmn();
}

AttStemsCmn::~AttStemsCmn() {

}

void AttStemsCmn::ResetStemsCmn() {
    m_stemWith = OTHERSTAFF_NONE;
}

bool AttStemsCmn::ReadStemsCmn( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("stem.with")) {
        this->SetStemWith(StrToOtherstaff(element.attribute("stem.with").value()));
        element.remove_attribute("stem.with");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStemsCmn::WriteStemsCmn( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasStemWith()) {
        element.append_attribute("stem.with") = OtherstaffToStr(this->GetStemWith()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStemsCmn::HasStemWith( )
{
    return (m_stemWith != OTHERSTAFF_NONE);
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
    m_tieRend = CURVERENDITION_NONE;
}

bool AttTierend::ReadTierend( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("tie.rend")) {
        this->SetTieRend(StrToCurverendition(element.attribute("tie.rend").value()));
        element.remove_attribute("tie.rend");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTierend::WriteTierend( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTieRend()) {
        element.append_attribute("tie.rend") = CurverenditionToStr(this->GetTieRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTierend::HasTieRend( )
{
    return (m_tieRend != CURVERENDITION_NONE);
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
    m_measperf = DURATION_NONE;
}

bool AttTremmeasured::ReadTremmeasured( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("measperf")) {
        this->SetMeasperf(StrToDuration(element.attribute("measperf").value()));
        element.remove_attribute("measperf");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTremmeasured::WriteTremmeasured( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMeasperf()) {
        element.append_attribute("measperf") = DurationToStr(this->GetMeasperf()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTremmeasured::HasMeasperf( )
{
    return (m_measperf != DURATION_NONE);
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
    m_numFormat = tupletVis_NUMFORMAT_NONE;
}

bool AttTupletVis::ReadTupletVis( pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("bracket.place")) {
        this->SetBracketPlace(StrToPlace(element.attribute("bracket.place").value()));
        element.remove_attribute("bracket.place");
        hasAttribute = true;
    }
    if (element.attribute("bracket.visible")) {
        this->SetBracketVisible(StrToBoolean(element.attribute("bracket.visible").value()));
        element.remove_attribute("bracket.visible");
        hasAttribute = true;
    }
    if (element.attribute("dur.visible")) {
        this->SetDurVisible(StrToBoolean(element.attribute("dur.visible").value()));
        element.remove_attribute("dur.visible");
        hasAttribute = true;
    }
    if (element.attribute("num.format")) {
        this->SetNumFormat(StrToTupletVisNumformat(element.attribute("num.format").value()));
        element.remove_attribute("num.format");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTupletVis::WriteTupletVis( pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBracketPlace()) {
        element.append_attribute("bracket.place") = PlaceToStr(this->GetBracketPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBracketVisible()) {
        element.append_attribute("bracket.visible") = BooleanToStr(this->GetBracketVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDurVisible()) {
        element.append_attribute("dur.visible") = BooleanToStr(this->GetDurVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNumFormat()) {
        element.append_attribute("num.format") = TupletVisNumformatToStr(this->GetNumFormat()).c_str();
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
    return (m_numFormat != tupletVis_NUMFORMAT_NONE);
}


/* include <attnum.format> */

bool Att::SetCmn( Object *element, std::string attrType, std::string attrValue ) {
    if (element->HasAttClass( ATT_ARPEGLOG ) ) {
        AttArpegLog *att = dynamic_cast<AttArpegLog*>(element);
        assert( att );
        if (attrType == "order") {
            att->SetOrder(att->StrToArpegLogOrder(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_ARPEGVIS ) ) {
        AttArpegVis *att = dynamic_cast<AttArpegVis*>(element);
        assert( att );
        if (attrType == "arrow") {
            att->SetArrow(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BTREMLOG ) ) {
        AttBTremLog *att = dynamic_cast<AttBTremLog*>(element);
        assert( att );
        if (attrType == "form") {
            att->SetForm(att->StrToBTremLogForm(attrValue));
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
        if (attrType == "beam.with") {
            att->SetBeamWith(att->StrToOtherstaff(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BEAMINGLOG ) ) {
        AttBeamingLog *att = dynamic_cast<AttBeamingLog*>(element);
        assert( att );
        if (attrType == "beam.group") {
            att->SetBeamGroup(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "beam.rests") {
            att->SetBeamRests(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BEAMREND ) ) {
        AttBeamrend *att = dynamic_cast<AttBeamrend*>(element);
        assert( att );
        if (attrType == "form") {
            att->SetForm(att->StrToBeamrendForm(attrValue));
            return true;
        }
        if (attrType == "slope") {
            att->SetSlope(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BEAMSECONDARY ) ) {
        AttBeamsecondary *att = dynamic_cast<AttBeamsecondary*>(element);
        assert( att );
        if (attrType == "breaksec") {
            att->SetBreaksec(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BEATRPTLOG ) ) {
        AttBeatRptLog *att = dynamic_cast<AttBeatRptLog*>(element);
        assert( att );
        if (attrType == "beatDef") {
            att->SetBeatDef(att->StrToDuration(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_BEATRPTVIS ) ) {
        AttBeatRptVis *att = dynamic_cast<AttBeatRptVis*>(element);
        assert( att );
        if (attrType == "form") {
            att->SetForm(att->StrToBeatrptRend(attrValue));
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
            att->SetCutout(att->StrToCutoutCutout(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_EXPANDABLE ) ) {
        AttExpandable *att = dynamic_cast<AttExpandable*>(element);
        assert( att );
        if (attrType == "expand") {
            att->SetExpand(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_FTREMLOG ) ) {
        AttFTremLog *att = dynamic_cast<AttFTremLog*>(element);
        assert( att );
        if (attrType == "form") {
            att->SetForm(att->StrToFTremLogForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_FERMATAVIS ) ) {
        AttFermataVis *att = dynamic_cast<AttFermataVis*>(element);
        assert( att );
        if (attrType == "form") {
            att->SetForm(att->StrToFermataVisForm(attrValue));
            return true;
        }
        if (attrType == "shape") {
            att->SetShape(att->StrToFermataVisShape(attrValue));
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
        if (attrType == "grace.time") {
            att->SetGraceTime(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_HAIRPINLOG ) ) {
        AttHairpinLog *att = dynamic_cast<AttHairpinLog*>(element);
        assert( att );
        if (attrType == "form") {
            att->SetForm(att->StrToHairpinLogForm(attrValue));
            return true;
        }
        if (attrType == "niente") {
            att->SetNiente(att->StrToBoolean(attrValue));
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
            att->SetC(att->StrToHarpPedalLogC(attrValue));
            return true;
        }
        if (attrType == "d") {
            att->SetD(att->StrToHarpPedalLogD(attrValue));
            return true;
        }
        if (attrType == "e") {
            att->SetE(att->StrToHarpPedalLogE(attrValue));
            return true;
        }
        if (attrType == "f") {
            att->SetF(att->StrToHarpPedalLogF(attrValue));
            return true;
        }
        if (attrType == "g") {
            att->SetG(att->StrToHarpPedalLogG(attrValue));
            return true;
        }
        if (attrType == "a") {
            att->SetA(att->StrToHarpPedalLogA(attrValue));
            return true;
        }
        if (attrType == "b") {
            att->SetB(att->StrToHarpPedalLogB(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_LVPRESENT ) ) {
        AttLvpresent *att = dynamic_cast<AttLvpresent*>(element);
        assert( att );
        if (attrType == "lv") {
            att->SetLv(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_MULTIRESTVIS ) ) {
        AttMultiRestVis *att = dynamic_cast<AttMultiRestVis*>(element);
        assert( att );
        if (attrType == "block") {
            att->SetBlock(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_NOTEGESCMN ) ) {
        AttNoteGesCmn *att = dynamic_cast<AttNoteGesCmn*>(element);
        assert( att );
        if (attrType == "gliss") {
            att->SetGliss(att->StrToGlissando(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_NUMBERED ) ) {
        AttNumbered *att = dynamic_cast<AttNumbered*>(element);
        assert( att );
        if (attrType == "num") {
            att->SetNum(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_NUMBERPLACEMENT ) ) {
        AttNumberplacement *att = dynamic_cast<AttNumberplacement*>(element);
        assert( att );
        if (attrType == "num.place") {
            att->SetNumPlace(att->StrToPlace(attrValue));
            return true;
        }
        if (attrType == "num.visible") {
            att->SetNumVisible(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_OCTAVELOG ) ) {
        AttOctaveLog *att = dynamic_cast<AttOctaveLog*>(element);
        assert( att );
        if (attrType == "coll") {
            att->SetColl(att->StrToOctaveLogColl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_PEDALLOG ) ) {
        AttPedalLog *att = dynamic_cast<AttPedalLog*>(element);
        assert( att );
        if (attrType == "dir") {
            att->SetDir(att->StrToPedalLogDir(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_PEDALVIS ) ) {
        AttPedalVis *att = dynamic_cast<AttPedalVis*>(element);
        assert( att );
        if (attrType == "form") {
            att->SetForm(att->StrToPedalVisForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_PIANOPEDALS ) ) {
        AttPianopedals *att = dynamic_cast<AttPianopedals*>(element);
        assert( att );
        if (attrType == "pedal.style") {
            att->SetPedalStyle(att->StrToPianopedalsPedalstyle(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_REHEARSAL ) ) {
        AttRehearsal *att = dynamic_cast<AttRehearsal*>(element);
        assert( att );
        if (attrType == "reh.enclose") {
            att->SetRehEnclose(att->StrToRehearsalRehenclose(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_SCOREDEFVISCMN ) ) {
        AttScoreDefVisCmn *att = dynamic_cast<AttScoreDefVisCmn*>(element);
        assert( att );
        if (attrType == "grid.show") {
            att->SetGridShow(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_SLURREND ) ) {
        AttSlurrend *att = dynamic_cast<AttSlurrend*>(element);
        assert( att );
        if (attrType == "slur.rend") {
            att->SetSlurRend(att->StrToCurverendition(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_STEMSCMN ) ) {
        AttStemsCmn *att = dynamic_cast<AttStemsCmn*>(element);
        assert( att );
        if (attrType == "stem.with") {
            att->SetStemWith(att->StrToOtherstaff(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_TIEREND ) ) {
        AttTierend *att = dynamic_cast<AttTierend*>(element);
        assert( att );
        if (attrType == "tie.rend") {
            att->SetTieRend(att->StrToCurverendition(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_TREMMEASURED ) ) {
        AttTremmeasured *att = dynamic_cast<AttTremmeasured*>(element);
        assert( att );
        if (attrType == "measperf") {
            att->SetMeasperf(att->StrToDuration(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_TUPLETVIS ) ) {
        AttTupletVis *att = dynamic_cast<AttTupletVis*>(element);
        assert( att );
        if (attrType == "bracket.place") {
            att->SetBracketPlace(att->StrToPlace(attrValue));
            return true;
        }
        if (attrType == "bracket.visible") {
            att->SetBracketVisible(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "dur.visible") {
            att->SetDurVisible(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "num.format") {
            att->SetNumFormat(att->StrToTupletVisNumformat(attrValue));
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
            attributes->push_back(std::make_pair("order", att->ArpegLogOrderToStr(att->GetOrder())));
        }
    }
    if (element->HasAttClass( ATT_ARPEGVIS ) ) {
        AttArpegVis *att = dynamic_cast<AttArpegVis*>(element);
        assert( att );
        if (att->HasArrow()) {
            attributes->push_back(std::make_pair("arrow", att->BooleanToStr(att->GetArrow())));
        }
    }
    if (element->HasAttClass( ATT_BTREMLOG ) ) {
        AttBTremLog *att = dynamic_cast<AttBTremLog*>(element);
        assert( att );
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->BTremLogFormToStr(att->GetForm())));
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
            attributes->push_back(std::make_pair("beam.with", att->OtherstaffToStr(att->GetBeamWith())));
        }
    }
    if (element->HasAttClass( ATT_BEAMINGLOG ) ) {
        AttBeamingLog *att = dynamic_cast<AttBeamingLog*>(element);
        assert( att );
        if (att->HasBeamGroup()) {
            attributes->push_back(std::make_pair("beam.group", att->StrToStr(att->GetBeamGroup())));
        }
        if (att->HasBeamRests()) {
            attributes->push_back(std::make_pair("beam.rests", att->BooleanToStr(att->GetBeamRests())));
        }
    }
    if (element->HasAttClass( ATT_BEAMREND ) ) {
        AttBeamrend *att = dynamic_cast<AttBeamrend*>(element);
        assert( att );
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->BeamrendFormToStr(att->GetForm())));
        }
        if (att->HasSlope()) {
            attributes->push_back(std::make_pair("slope", att->DblToStr(att->GetSlope())));
        }
    }
    if (element->HasAttClass( ATT_BEAMSECONDARY ) ) {
        AttBeamsecondary *att = dynamic_cast<AttBeamsecondary*>(element);
        assert( att );
        if (att->HasBreaksec()) {
            attributes->push_back(std::make_pair("breaksec", att->IntToStr(att->GetBreaksec())));
        }
    }
    if (element->HasAttClass( ATT_BEATRPTLOG ) ) {
        AttBeatRptLog *att = dynamic_cast<AttBeatRptLog*>(element);
        assert( att );
        if (att->HasBeatDef()) {
            attributes->push_back(std::make_pair("beatDef", att->DurationToStr(att->GetBeatDef())));
        }
    }
    if (element->HasAttClass( ATT_BEATRPTVIS ) ) {
        AttBeatRptVis *att = dynamic_cast<AttBeatRptVis*>(element);
        assert( att );
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->BeatrptRendToStr(att->GetForm())));
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
            attributes->push_back(std::make_pair("cutout", att->CutoutCutoutToStr(att->GetCutout())));
        }
    }
    if (element->HasAttClass( ATT_EXPANDABLE ) ) {
        AttExpandable *att = dynamic_cast<AttExpandable*>(element);
        assert( att );
        if (att->HasExpand()) {
            attributes->push_back(std::make_pair("expand", att->BooleanToStr(att->GetExpand())));
        }
    }
    if (element->HasAttClass( ATT_FTREMLOG ) ) {
        AttFTremLog *att = dynamic_cast<AttFTremLog*>(element);
        assert( att );
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->FTremLogFormToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass( ATT_FERMATAVIS ) ) {
        AttFermataVis *att = dynamic_cast<AttFermataVis*>(element);
        assert( att );
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->FermataVisFormToStr(att->GetForm())));
        }
        if (att->HasShape()) {
            attributes->push_back(std::make_pair("shape", att->FermataVisShapeToStr(att->GetShape())));
        }
    }
    if (element->HasAttClass( ATT_GRACED ) ) {
        AttGraced *att = dynamic_cast<AttGraced*>(element);
        assert( att );
        if (att->HasGrace()) {
            attributes->push_back(std::make_pair("grace", att->GraceToStr(att->GetGrace())));
        }
        if (att->HasGraceTime()) {
            attributes->push_back(std::make_pair("grace.time", att->IntToStr(att->GetGraceTime())));
        }
    }
    if (element->HasAttClass( ATT_HAIRPINLOG ) ) {
        AttHairpinLog *att = dynamic_cast<AttHairpinLog*>(element);
        assert( att );
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->HairpinLogFormToStr(att->GetForm())));
        }
        if (att->HasNiente()) {
            attributes->push_back(std::make_pair("niente", att->BooleanToStr(att->GetNiente())));
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
            attributes->push_back(std::make_pair("c", att->HarpPedalLogCToStr(att->GetC())));
        }
        if (att->HasD()) {
            attributes->push_back(std::make_pair("d", att->HarpPedalLogDToStr(att->GetD())));
        }
        if (att->HasE()) {
            attributes->push_back(std::make_pair("e", att->HarpPedalLogEToStr(att->GetE())));
        }
        if (att->HasF()) {
            attributes->push_back(std::make_pair("f", att->HarpPedalLogFToStr(att->GetF())));
        }
        if (att->HasG()) {
            attributes->push_back(std::make_pair("g", att->HarpPedalLogGToStr(att->GetG())));
        }
        if (att->HasA()) {
            attributes->push_back(std::make_pair("a", att->HarpPedalLogAToStr(att->GetA())));
        }
        if (att->HasB()) {
            attributes->push_back(std::make_pair("b", att->HarpPedalLogBToStr(att->GetB())));
        }
    }
    if (element->HasAttClass( ATT_LVPRESENT ) ) {
        AttLvpresent *att = dynamic_cast<AttLvpresent*>(element);
        assert( att );
        if (att->HasLv()) {
            attributes->push_back(std::make_pair("lv", att->BooleanToStr(att->GetLv())));
        }
    }
    if (element->HasAttClass( ATT_MULTIRESTVIS ) ) {
        AttMultiRestVis *att = dynamic_cast<AttMultiRestVis*>(element);
        assert( att );
        if (att->HasBlock()) {
            attributes->push_back(std::make_pair("block", att->BooleanToStr(att->GetBlock())));
        }
    }
    if (element->HasAttClass( ATT_NOTEGESCMN ) ) {
        AttNoteGesCmn *att = dynamic_cast<AttNoteGesCmn*>(element);
        assert( att );
        if (att->HasGliss()) {
            attributes->push_back(std::make_pair("gliss", att->GlissandoToStr(att->GetGliss())));
        }
    }
    if (element->HasAttClass( ATT_NUMBERED ) ) {
        AttNumbered *att = dynamic_cast<AttNumbered*>(element);
        assert( att );
        if (att->HasNum()) {
            attributes->push_back(std::make_pair("num", att->IntToStr(att->GetNum())));
        }
    }
    if (element->HasAttClass( ATT_NUMBERPLACEMENT ) ) {
        AttNumberplacement *att = dynamic_cast<AttNumberplacement*>(element);
        assert( att );
        if (att->HasNumPlace()) {
            attributes->push_back(std::make_pair("num.place", att->PlaceToStr(att->GetNumPlace())));
        }
        if (att->HasNumVisible()) {
            attributes->push_back(std::make_pair("num.visible", att->BooleanToStr(att->GetNumVisible())));
        }
    }
    if (element->HasAttClass( ATT_OCTAVELOG ) ) {
        AttOctaveLog *att = dynamic_cast<AttOctaveLog*>(element);
        assert( att );
        if (att->HasColl()) {
            attributes->push_back(std::make_pair("coll", att->OctaveLogCollToStr(att->GetColl())));
        }
    }
    if (element->HasAttClass( ATT_PEDALLOG ) ) {
        AttPedalLog *att = dynamic_cast<AttPedalLog*>(element);
        assert( att );
        if (att->HasDir()) {
            attributes->push_back(std::make_pair("dir", att->PedalLogDirToStr(att->GetDir())));
        }
    }
    if (element->HasAttClass( ATT_PEDALVIS ) ) {
        AttPedalVis *att = dynamic_cast<AttPedalVis*>(element);
        assert( att );
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->PedalVisFormToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass( ATT_PIANOPEDALS ) ) {
        AttPianopedals *att = dynamic_cast<AttPianopedals*>(element);
        assert( att );
        if (att->HasPedalStyle()) {
            attributes->push_back(std::make_pair("pedal.style", att->PianopedalsPedalstyleToStr(att->GetPedalStyle())));
        }
    }
    if (element->HasAttClass( ATT_REHEARSAL ) ) {
        AttRehearsal *att = dynamic_cast<AttRehearsal*>(element);
        assert( att );
        if (att->HasRehEnclose()) {
            attributes->push_back(std::make_pair("reh.enclose", att->RehearsalRehencloseToStr(att->GetRehEnclose())));
        }
    }
    if (element->HasAttClass( ATT_SCOREDEFVISCMN ) ) {
        AttScoreDefVisCmn *att = dynamic_cast<AttScoreDefVisCmn*>(element);
        assert( att );
        if (att->HasGridShow()) {
            attributes->push_back(std::make_pair("grid.show", att->BooleanToStr(att->GetGridShow())));
        }
    }
    if (element->HasAttClass( ATT_SLURREND ) ) {
        AttSlurrend *att = dynamic_cast<AttSlurrend*>(element);
        assert( att );
        if (att->HasSlurRend()) {
            attributes->push_back(std::make_pair("slur.rend", att->CurverenditionToStr(att->GetSlurRend())));
        }
    }
    if (element->HasAttClass( ATT_STEMSCMN ) ) {
        AttStemsCmn *att = dynamic_cast<AttStemsCmn*>(element);
        assert( att );
        if (att->HasStemWith()) {
            attributes->push_back(std::make_pair("stem.with", att->OtherstaffToStr(att->GetStemWith())));
        }
    }
    if (element->HasAttClass( ATT_TIEREND ) ) {
        AttTierend *att = dynamic_cast<AttTierend*>(element);
        assert( att );
        if (att->HasTieRend()) {
            attributes->push_back(std::make_pair("tie.rend", att->CurverenditionToStr(att->GetTieRend())));
        }
    }
    if (element->HasAttClass( ATT_TREMMEASURED ) ) {
        AttTremmeasured *att = dynamic_cast<AttTremmeasured*>(element);
        assert( att );
        if (att->HasMeasperf()) {
            attributes->push_back(std::make_pair("measperf", att->DurationToStr(att->GetMeasperf())));
        }
    }
    if (element->HasAttClass( ATT_TUPLETVIS ) ) {
        AttTupletVis *att = dynamic_cast<AttTupletVis*>(element);
        assert( att );
        if (att->HasBracketPlace()) {
            attributes->push_back(std::make_pair("bracket.place", att->PlaceToStr(att->GetBracketPlace())));
        }
        if (att->HasBracketVisible()) {
            attributes->push_back(std::make_pair("bracket.visible", att->BooleanToStr(att->GetBracketVisible())));
        }
        if (att->HasDurVisible()) {
            attributes->push_back(std::make_pair("dur.visible", att->BooleanToStr(att->GetDurVisible())));
        }
        if (att->HasNumFormat()) {
            attributes->push_back(std::make_pair("num.format", att->TupletVisNumformatToStr(att->GetNumFormat())));
        }
    }

}
    
} // vrv namespace
    
