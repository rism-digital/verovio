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

AttArpegLog::AttArpegLog() : Att()
{
    ResetArpegLog();
}

AttArpegLog::~AttArpegLog()
{
}

void AttArpegLog::ResetArpegLog()
{
    m_order = arpegLog_ORDER_NONE;
}

bool AttArpegLog::ReadArpegLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("order")) {
        this->SetOrder(StrToArpegLogOrder(element.attribute("order").value()));
        element.remove_attribute("order");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArpegLog::WriteArpegLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOrder()) {
        element.append_attribute("order") = ArpegLogOrderToStr(this->GetOrder()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttArpegLog::HasOrder() const
{
    return (m_order != arpegLog_ORDER_NONE);
}

/* include <attorder> */

//----------------------------------------------------------------------------
// AttBTremLog
//----------------------------------------------------------------------------

AttBTremLog::AttBTremLog() : Att()
{
    ResetBTremLog();
}

AttBTremLog::~AttBTremLog()
{
}

void AttBTremLog::ResetBTremLog()
{
    m_form = bTremLog_FORM_NONE;
}

bool AttBTremLog::ReadBTremLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToBTremLogForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBTremLog::WriteBTremLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = BTremLogFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBTremLog::HasForm() const
{
    return (m_form != bTremLog_FORM_NONE);
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttBeamPresent
//----------------------------------------------------------------------------

AttBeamPresent::AttBeamPresent() : Att()
{
    ResetBeamPresent();
}

AttBeamPresent::~AttBeamPresent()
{
}

void AttBeamPresent::ResetBeamPresent()
{
    m_beam = "";
}

bool AttBeamPresent::ReadBeamPresent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("beam")) {
        this->SetBeam(StrToStr(element.attribute("beam").value()));
        element.remove_attribute("beam");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamPresent::WriteBeamPresent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBeam()) {
        element.append_attribute("beam") = StrToStr(this->GetBeam()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamPresent::HasBeam() const
{
    return (m_beam != "");
}

/* include <attbeam> */

//----------------------------------------------------------------------------
// AttBeamRend
//----------------------------------------------------------------------------

AttBeamRend::AttBeamRend() : Att()
{
    ResetBeamRend();
}

AttBeamRend::~AttBeamRend()
{
}

void AttBeamRend::ResetBeamRend()
{
    m_form = beamRend_FORM_NONE;
    m_place = BEAMPLACE_NONE;
    m_slash = BOOLEAN_NONE;
    m_slope = 0.0;
}

bool AttBeamRend::ReadBeamRend(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToBeamRendForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("place")) {
        this->SetPlace(StrToBeamplace(element.attribute("place").value()));
        element.remove_attribute("place");
        hasAttribute = true;
    }
    if (element.attribute("slash")) {
        this->SetSlash(StrToBoolean(element.attribute("slash").value()));
        element.remove_attribute("slash");
        hasAttribute = true;
    }
    if (element.attribute("slope")) {
        this->SetSlope(StrToDbl(element.attribute("slope").value()));
        element.remove_attribute("slope");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamRend::WriteBeamRend(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = BeamRendFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPlace()) {
        element.append_attribute("place") = BeamplaceToStr(this->GetPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSlash()) {
        element.append_attribute("slash") = BooleanToStr(this->GetSlash()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSlope()) {
        element.append_attribute("slope") = DblToStr(this->GetSlope()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamRend::HasForm() const
{
    return (m_form != beamRend_FORM_NONE);
}

bool AttBeamRend::HasPlace() const
{
    return (m_place != BEAMPLACE_NONE);
}

bool AttBeamRend::HasSlash() const
{
    return (m_slash != BOOLEAN_NONE);
}

bool AttBeamRend::HasSlope() const
{
    return (m_slope != 0.0);
}

/* include <attslope> */

//----------------------------------------------------------------------------
// AttBeamSecondary
//----------------------------------------------------------------------------

AttBeamSecondary::AttBeamSecondary() : Att()
{
    ResetBeamSecondary();
}

AttBeamSecondary::~AttBeamSecondary()
{
}

void AttBeamSecondary::ResetBeamSecondary()
{
    m_breaksec = 0;
}

bool AttBeamSecondary::ReadBeamSecondary(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("breaksec")) {
        this->SetBreaksec(StrToInt(element.attribute("breaksec").value()));
        element.remove_attribute("breaksec");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamSecondary::WriteBeamSecondary(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBreaksec()) {
        element.append_attribute("breaksec") = IntToStr(this->GetBreaksec()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamSecondary::HasBreaksec() const
{
    return (m_breaksec != 0);
}

/* include <attbreaksec> */

//----------------------------------------------------------------------------
// AttBeamedWith
//----------------------------------------------------------------------------

AttBeamedWith::AttBeamedWith() : Att()
{
    ResetBeamedWith();
}

AttBeamedWith::~AttBeamedWith()
{
}

void AttBeamedWith::ResetBeamedWith()
{
    m_beamWith = OTHERSTAFF_NONE;
}

bool AttBeamedWith::ReadBeamedWith(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("beam.with")) {
        this->SetBeamWith(StrToOtherstaff(element.attribute("beam.with").value()));
        element.remove_attribute("beam.with");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamedWith::WriteBeamedWith(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBeamWith()) {
        element.append_attribute("beam.with") = OtherstaffToStr(this->GetBeamWith()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamedWith::HasBeamWith() const
{
    return (m_beamWith != OTHERSTAFF_NONE);
}

/* include <attbeam.with> */

//----------------------------------------------------------------------------
// AttBeamingLog
//----------------------------------------------------------------------------

AttBeamingLog::AttBeamingLog() : Att()
{
    ResetBeamingLog();
}

AttBeamingLog::~AttBeamingLog()
{
}

void AttBeamingLog::ResetBeamingLog()
{
    m_beamGroup = "";
    m_beamRests = BOOLEAN_NONE;
}

bool AttBeamingLog::ReadBeamingLog(pugi::xml_node element)
{
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

bool AttBeamingLog::WriteBeamingLog(pugi::xml_node element)
{
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

bool AttBeamingLog::HasBeamGroup() const
{
    return (m_beamGroup != "");
}

bool AttBeamingLog::HasBeamRests() const
{
    return (m_beamRests != BOOLEAN_NONE);
}

/* include <attbeam.rests> */

//----------------------------------------------------------------------------
// AttBeatRptLog
//----------------------------------------------------------------------------

AttBeatRptLog::AttBeatRptLog() : Att()
{
    ResetBeatRptLog();
}

AttBeatRptLog::~AttBeatRptLog()
{
}

void AttBeatRptLog::ResetBeatRptLog()
{
    m_beatdef = 0.0;
}

bool AttBeatRptLog::ReadBeatRptLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("beatdef")) {
        this->SetBeatdef(StrToDbl(element.attribute("beatdef").value()));
        element.remove_attribute("beatdef");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeatRptLog::WriteBeatRptLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBeatdef()) {
        element.append_attribute("beatdef") = DblToStr(this->GetBeatdef()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeatRptLog::HasBeatdef() const
{
    return (m_beatdef != 0.0);
}

/* include <attbeatdef> */

//----------------------------------------------------------------------------
// AttBracketSpanLog
//----------------------------------------------------------------------------

AttBracketSpanLog::AttBracketSpanLog() : Att()
{
    ResetBracketSpanLog();
}

AttBracketSpanLog::~AttBracketSpanLog()
{
}

void AttBracketSpanLog::ResetBracketSpanLog()
{
    m_func = "";
}

bool AttBracketSpanLog::ReadBracketSpanLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToStr(element.attribute("func").value()));
        element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBracketSpanLog::WriteBracketSpanLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = StrToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBracketSpanLog::HasFunc() const
{
    return (m_func != "");
}

/* include <attfunc> */

//----------------------------------------------------------------------------
// AttCutout
//----------------------------------------------------------------------------

AttCutout::AttCutout() : Att()
{
    ResetCutout();
}

AttCutout::~AttCutout()
{
}

void AttCutout::ResetCutout()
{
    m_cutout = cutout_CUTOUT_NONE;
}

bool AttCutout::ReadCutout(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("cutout")) {
        this->SetCutout(StrToCutoutCutout(element.attribute("cutout").value()));
        element.remove_attribute("cutout");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCutout::WriteCutout(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCutout()) {
        element.append_attribute("cutout") = CutoutCutoutToStr(this->GetCutout()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCutout::HasCutout() const
{
    return (m_cutout != cutout_CUTOUT_NONE);
}

/* include <attcutout> */

//----------------------------------------------------------------------------
// AttExpandable
//----------------------------------------------------------------------------

AttExpandable::AttExpandable() : Att()
{
    ResetExpandable();
}

AttExpandable::~AttExpandable()
{
}

void AttExpandable::ResetExpandable()
{
    m_expand = BOOLEAN_NONE;
}

bool AttExpandable::ReadExpandable(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("expand")) {
        this->SetExpand(StrToBoolean(element.attribute("expand").value()));
        element.remove_attribute("expand");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttExpandable::WriteExpandable(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasExpand()) {
        element.append_attribute("expand") = BooleanToStr(this->GetExpand()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttExpandable::HasExpand() const
{
    return (m_expand != BOOLEAN_NONE);
}

/* include <attexpand> */

//----------------------------------------------------------------------------
// AttFTremLog
//----------------------------------------------------------------------------

AttFTremLog::AttFTremLog() : Att()
{
    ResetFTremLog();
}

AttFTremLog::~AttFTremLog()
{
}

void AttFTremLog::ResetFTremLog()
{
    m_form = fTremLog_FORM_NONE;
}

bool AttFTremLog::ReadFTremLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToFTremLogForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFTremLog::WriteFTremLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = FTremLogFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFTremLog::HasForm() const
{
    return (m_form != fTremLog_FORM_NONE);
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttGlissPresent
//----------------------------------------------------------------------------

AttGlissPresent::AttGlissPresent() : Att()
{
    ResetGlissPresent();
}

AttGlissPresent::~AttGlissPresent()
{
}

void AttGlissPresent::ResetGlissPresent()
{
    m_gliss = GLISSANDO_NONE;
}

bool AttGlissPresent::ReadGlissPresent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("gliss")) {
        this->SetGliss(StrToGlissando(element.attribute("gliss").value()));
        element.remove_attribute("gliss");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttGlissPresent::WriteGlissPresent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasGliss()) {
        element.append_attribute("gliss") = GlissandoToStr(this->GetGliss()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttGlissPresent::HasGliss() const
{
    return (m_gliss != GLISSANDO_NONE);
}

/* include <attgliss> */

//----------------------------------------------------------------------------
// AttGraceGrpLog
//----------------------------------------------------------------------------

AttGraceGrpLog::AttGraceGrpLog() : Att()
{
    ResetGraceGrpLog();
}

AttGraceGrpLog::~AttGraceGrpLog()
{
}

void AttGraceGrpLog::ResetGraceGrpLog()
{
    m_attach = graceGrpLog_ATTACH_NONE;
}

bool AttGraceGrpLog::ReadGraceGrpLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("attach")) {
        this->SetAttach(StrToGraceGrpLogAttach(element.attribute("attach").value()));
        element.remove_attribute("attach");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttGraceGrpLog::WriteGraceGrpLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAttach()) {
        element.append_attribute("attach") = GraceGrpLogAttachToStr(this->GetAttach()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttGraceGrpLog::HasAttach() const
{
    return (m_attach != graceGrpLog_ATTACH_NONE);
}

/* include <attattach> */

//----------------------------------------------------------------------------
// AttGraced
//----------------------------------------------------------------------------

AttGraced::AttGraced() : Att()
{
    ResetGraced();
}

AttGraced::~AttGraced()
{
}

void AttGraced::ResetGraced()
{
    m_grace = GRACE_NONE;
    m_graceTime = -1.0;
}

bool AttGraced::ReadGraced(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("grace")) {
        this->SetGrace(StrToGrace(element.attribute("grace").value()));
        element.remove_attribute("grace");
        hasAttribute = true;
    }
    if (element.attribute("grace.time")) {
        this->SetGraceTime(StrToPercent(element.attribute("grace.time").value()));
        element.remove_attribute("grace.time");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttGraced::WriteGraced(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasGrace()) {
        element.append_attribute("grace") = GraceToStr(this->GetGrace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasGraceTime()) {
        element.append_attribute("grace.time") = PercentToStr(this->GetGraceTime()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttGraced::HasGrace() const
{
    return (m_grace != GRACE_NONE);
}

bool AttGraced::HasGraceTime() const
{
    return (m_graceTime != -1.0);
}

/* include <attgrace.time> */

//----------------------------------------------------------------------------
// AttHairpinLog
//----------------------------------------------------------------------------

AttHairpinLog::AttHairpinLog() : Att()
{
    ResetHairpinLog();
}

AttHairpinLog::~AttHairpinLog()
{
}

void AttHairpinLog::ResetHairpinLog()
{
    m_form = hairpinLog_FORM_NONE;
    m_niente = BOOLEAN_NONE;
}

bool AttHairpinLog::ReadHairpinLog(pugi::xml_node element)
{
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

bool AttHairpinLog::WriteHairpinLog(pugi::xml_node element)
{
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

bool AttHairpinLog::HasForm() const
{
    return (m_form != hairpinLog_FORM_NONE);
}

bool AttHairpinLog::HasNiente() const
{
    return (m_niente != BOOLEAN_NONE);
}

/* include <attniente> */

//----------------------------------------------------------------------------
// AttHarpPedalLog
//----------------------------------------------------------------------------

AttHarpPedalLog::AttHarpPedalLog() : Att()
{
    ResetHarpPedalLog();
}

AttHarpPedalLog::~AttHarpPedalLog()
{
}

void AttHarpPedalLog::ResetHarpPedalLog()
{
    m_c = harpPedalLog_C_NONE;
    m_d = harpPedalLog_D_NONE;
    m_e = harpPedalLog_E_NONE;
    m_f = harpPedalLog_F_NONE;
    m_g = harpPedalLog_G_NONE;
    m_a = harpPedalLog_A_NONE;
    m_b = harpPedalLog_B_NONE;
}

bool AttHarpPedalLog::ReadHarpPedalLog(pugi::xml_node element)
{
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

bool AttHarpPedalLog::WriteHarpPedalLog(pugi::xml_node element)
{
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

bool AttHarpPedalLog::HasC() const
{
    return (m_c != harpPedalLog_C_NONE);
}

bool AttHarpPedalLog::HasD() const
{
    return (m_d != harpPedalLog_D_NONE);
}

bool AttHarpPedalLog::HasE() const
{
    return (m_e != harpPedalLog_E_NONE);
}

bool AttHarpPedalLog::HasF() const
{
    return (m_f != harpPedalLog_F_NONE);
}

bool AttHarpPedalLog::HasG() const
{
    return (m_g != harpPedalLog_G_NONE);
}

bool AttHarpPedalLog::HasA() const
{
    return (m_a != harpPedalLog_A_NONE);
}

bool AttHarpPedalLog::HasB() const
{
    return (m_b != harpPedalLog_B_NONE);
}

/* include <attb> */

//----------------------------------------------------------------------------
// AttLvPresent
//----------------------------------------------------------------------------

AttLvPresent::AttLvPresent() : Att()
{
    ResetLvPresent();
}

AttLvPresent::~AttLvPresent()
{
}

void AttLvPresent::ResetLvPresent()
{
    m_lv = BOOLEAN_NONE;
}

bool AttLvPresent::ReadLvPresent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("lv")) {
        this->SetLv(StrToBoolean(element.attribute("lv").value()));
        element.remove_attribute("lv");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLvPresent::WriteLvPresent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLv()) {
        element.append_attribute("lv") = BooleanToStr(this->GetLv()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLvPresent::HasLv() const
{
    return (m_lv != BOOLEAN_NONE);
}

/* include <attlv> */

//----------------------------------------------------------------------------
// AttMeasureLog
//----------------------------------------------------------------------------

AttMeasureLog::AttMeasureLog() : Att()
{
    ResetMeasureLog();
}

AttMeasureLog::~AttMeasureLog()
{
}

void AttMeasureLog::ResetMeasureLog()
{
    m_left = BARRENDITION_NONE;
    m_right = BARRENDITION_NONE;
}

bool AttMeasureLog::ReadMeasureLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("left")) {
        this->SetLeft(StrToBarrendition(element.attribute("left").value()));
        element.remove_attribute("left");
        hasAttribute = true;
    }
    if (element.attribute("right")) {
        this->SetRight(StrToBarrendition(element.attribute("right").value()));
        element.remove_attribute("right");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeasureLog::WriteMeasureLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLeft()) {
        element.append_attribute("left") = BarrenditionToStr(this->GetLeft()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRight()) {
        element.append_attribute("right") = BarrenditionToStr(this->GetRight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeasureLog::HasLeft() const
{
    return (m_left != BARRENDITION_NONE);
}

bool AttMeasureLog::HasRight() const
{
    return (m_right != BARRENDITION_NONE);
}

/* include <attright> */

//----------------------------------------------------------------------------
// AttMeterSigGrpLog
//----------------------------------------------------------------------------

AttMeterSigGrpLog::AttMeterSigGrpLog() : Att()
{
    ResetMeterSigGrpLog();
}

AttMeterSigGrpLog::~AttMeterSigGrpLog()
{
}

void AttMeterSigGrpLog::ResetMeterSigGrpLog()
{
    m_func = meterSigGrpLog_FUNC_NONE;
}

bool AttMeterSigGrpLog::ReadMeterSigGrpLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToMeterSigGrpLogFunc(element.attribute("func").value()));
        element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigGrpLog::WriteMeterSigGrpLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = MeterSigGrpLogFuncToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterSigGrpLog::HasFunc() const
{
    return (m_func != meterSigGrpLog_FUNC_NONE);
}

/* include <attfunc> */

//----------------------------------------------------------------------------
// AttNumberPlacement
//----------------------------------------------------------------------------

AttNumberPlacement::AttNumberPlacement() : Att()
{
    ResetNumberPlacement();
}

AttNumberPlacement::~AttNumberPlacement()
{
}

void AttNumberPlacement::ResetNumberPlacement()
{
    m_numPlace = STAFFREL_basic_NONE;
    m_numVisible = BOOLEAN_NONE;
}

bool AttNumberPlacement::ReadNumberPlacement(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("num.place")) {
        this->SetNumPlace(StrToStaffrelBasic(element.attribute("num.place").value()));
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

bool AttNumberPlacement::WriteNumberPlacement(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasNumPlace()) {
        element.append_attribute("num.place") = StaffrelBasicToStr(this->GetNumPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNumVisible()) {
        element.append_attribute("num.visible") = BooleanToStr(this->GetNumVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNumberPlacement::HasNumPlace() const
{
    return (m_numPlace != STAFFREL_basic_NONE);
}

bool AttNumberPlacement::HasNumVisible() const
{
    return (m_numVisible != BOOLEAN_NONE);
}

/* include <attnum.visible> */

//----------------------------------------------------------------------------
// AttNumbered
//----------------------------------------------------------------------------

AttNumbered::AttNumbered() : Att()
{
    ResetNumbered();
}

AttNumbered::~AttNumbered()
{
}

void AttNumbered::ResetNumbered()
{
    m_num = 0;
}

bool AttNumbered::ReadNumbered(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNum(StrToInt(element.attribute("num").value()));
        element.remove_attribute("num");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNumbered::WriteNumbered(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasNum()) {
        element.append_attribute("num") = IntToStr(this->GetNum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNumbered::HasNum() const
{
    return (m_num != 0);
}

/* include <attnum> */

//----------------------------------------------------------------------------
// AttOctaveLog
//----------------------------------------------------------------------------

AttOctaveLog::AttOctaveLog() : Att()
{
    ResetOctaveLog();
}

AttOctaveLog::~AttOctaveLog()
{
}

void AttOctaveLog::ResetOctaveLog()
{
    m_coll = octaveLog_COLL_NONE;
}

bool AttOctaveLog::ReadOctaveLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("coll")) {
        this->SetColl(StrToOctaveLogColl(element.attribute("coll").value()));
        element.remove_attribute("coll");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctaveLog::WriteOctaveLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasColl()) {
        element.append_attribute("coll") = OctaveLogCollToStr(this->GetColl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOctaveLog::HasColl() const
{
    return (m_coll != octaveLog_COLL_NONE);
}

/* include <attcoll> */

//----------------------------------------------------------------------------
// AttPedalLog
//----------------------------------------------------------------------------

AttPedalLog::AttPedalLog() : Att()
{
    ResetPedalLog();
}

AttPedalLog::~AttPedalLog()
{
}

void AttPedalLog::ResetPedalLog()
{
    m_dir = pedalLog_DIR_NONE;
    m_func = "";
}

bool AttPedalLog::ReadPedalLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dir")) {
        this->SetDir(StrToPedalLogDir(element.attribute("dir").value()));
        element.remove_attribute("dir");
        hasAttribute = true;
    }
    if (element.attribute("func")) {
        this->SetFunc(StrToStr(element.attribute("func").value()));
        element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPedalLog::WritePedalLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDir()) {
        element.append_attribute("dir") = PedalLogDirToStr(this->GetDir()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFunc()) {
        element.append_attribute("func") = StrToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPedalLog::HasDir() const
{
    return (m_dir != pedalLog_DIR_NONE);
}

bool AttPedalLog::HasFunc() const
{
    return (m_func != "");
}

/* include <attfunc> */

//----------------------------------------------------------------------------
// AttPianoPedals
//----------------------------------------------------------------------------

AttPianoPedals::AttPianoPedals() : Att()
{
    ResetPianoPedals();
}

AttPianoPedals::~AttPianoPedals()
{
}

void AttPianoPedals::ResetPianoPedals()
{
    m_pedalStyle = pianoPedals_PEDALSTYLE_NONE;
}

bool AttPianoPedals::ReadPianoPedals(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("pedal.style")) {
        this->SetPedalStyle(StrToPianoPedalsPedalstyle(element.attribute("pedal.style").value()));
        element.remove_attribute("pedal.style");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPianoPedals::WritePianoPedals(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPedalStyle()) {
        element.append_attribute("pedal.style") = PianoPedalsPedalstyleToStr(this->GetPedalStyle()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPianoPedals::HasPedalStyle() const
{
    return (m_pedalStyle != pianoPedals_PEDALSTYLE_NONE);
}

/* include <attpedal.style> */

//----------------------------------------------------------------------------
// AttRehearsal
//----------------------------------------------------------------------------

AttRehearsal::AttRehearsal() : Att()
{
    ResetRehearsal();
}

AttRehearsal::~AttRehearsal()
{
}

void AttRehearsal::ResetRehearsal()
{
    m_rehEnclose = rehearsal_REHENCLOSE_NONE;
}

bool AttRehearsal::ReadRehearsal(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("reh.enclose")) {
        this->SetRehEnclose(StrToRehearsalRehenclose(element.attribute("reh.enclose").value()));
        element.remove_attribute("reh.enclose");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRehearsal::WriteRehearsal(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasRehEnclose()) {
        element.append_attribute("reh.enclose") = RehearsalRehencloseToStr(this->GetRehEnclose()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRehearsal::HasRehEnclose() const
{
    return (m_rehEnclose != rehearsal_REHENCLOSE_NONE);
}

/* include <attreh.enclose> */

//----------------------------------------------------------------------------
// AttScoreDefVisCmn
//----------------------------------------------------------------------------

AttScoreDefVisCmn::AttScoreDefVisCmn() : Att()
{
    ResetScoreDefVisCmn();
}

AttScoreDefVisCmn::~AttScoreDefVisCmn()
{
}

void AttScoreDefVisCmn::ResetScoreDefVisCmn()
{
    m_gridShow = BOOLEAN_NONE;
}

bool AttScoreDefVisCmn::ReadScoreDefVisCmn(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("grid.show")) {
        this->SetGridShow(StrToBoolean(element.attribute("grid.show").value()));
        element.remove_attribute("grid.show");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScoreDefVisCmn::WriteScoreDefVisCmn(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasGridShow()) {
        element.append_attribute("grid.show") = BooleanToStr(this->GetGridShow()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttScoreDefVisCmn::HasGridShow() const
{
    return (m_gridShow != BOOLEAN_NONE);
}

/* include <attgrid.show> */

//----------------------------------------------------------------------------
// AttSlurRend
//----------------------------------------------------------------------------

AttSlurRend::AttSlurRend() : Att()
{
    ResetSlurRend();
}

AttSlurRend::~AttSlurRend()
{
}

void AttSlurRend::ResetSlurRend()
{
    m_slurLform = LINEFORM_NONE;
    m_slurLwidth = data_LINEWIDTH();
}

bool AttSlurRend::ReadSlurRend(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("slur.lform")) {
        this->SetSlurLform(StrToLineform(element.attribute("slur.lform").value()));
        element.remove_attribute("slur.lform");
        hasAttribute = true;
    }
    if (element.attribute("slur.lwidth")) {
        this->SetSlurLwidth(StrToLinewidth(element.attribute("slur.lwidth").value()));
        element.remove_attribute("slur.lwidth");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSlurRend::WriteSlurRend(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSlurLform()) {
        element.append_attribute("slur.lform") = LineformToStr(this->GetSlurLform()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSlurLwidth()) {
        element.append_attribute("slur.lwidth") = LinewidthToStr(this->GetSlurLwidth()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSlurRend::HasSlurLform() const
{
    return (m_slurLform != LINEFORM_NONE);
}

bool AttSlurRend::HasSlurLwidth() const
{
    return (m_slurLwidth.HasValue());
}

/* include <attslur.lwidth> */

//----------------------------------------------------------------------------
// AttStemsCmn
//----------------------------------------------------------------------------

AttStemsCmn::AttStemsCmn() : Att()
{
    ResetStemsCmn();
}

AttStemsCmn::~AttStemsCmn()
{
}

void AttStemsCmn::ResetStemsCmn()
{
    m_stemWith = OTHERSTAFF_NONE;
}

bool AttStemsCmn::ReadStemsCmn(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("stem.with")) {
        this->SetStemWith(StrToOtherstaff(element.attribute("stem.with").value()));
        element.remove_attribute("stem.with");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStemsCmn::WriteStemsCmn(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasStemWith()) {
        element.append_attribute("stem.with") = OtherstaffToStr(this->GetStemWith()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStemsCmn::HasStemWith() const
{
    return (m_stemWith != OTHERSTAFF_NONE);
}

/* include <attstem.with> */

//----------------------------------------------------------------------------
// AttTieRend
//----------------------------------------------------------------------------

AttTieRend::AttTieRend() : Att()
{
    ResetTieRend();
}

AttTieRend::~AttTieRend()
{
}

void AttTieRend::ResetTieRend()
{
    m_tieLform = LINEFORM_NONE;
    m_tieLwidth = data_LINEWIDTH();
}

bool AttTieRend::ReadTieRend(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tie.lform")) {
        this->SetTieLform(StrToLineform(element.attribute("tie.lform").value()));
        element.remove_attribute("tie.lform");
        hasAttribute = true;
    }
    if (element.attribute("tie.lwidth")) {
        this->SetTieLwidth(StrToLinewidth(element.attribute("tie.lwidth").value()));
        element.remove_attribute("tie.lwidth");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTieRend::WriteTieRend(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTieLform()) {
        element.append_attribute("tie.lform") = LineformToStr(this->GetTieLform()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTieLwidth()) {
        element.append_attribute("tie.lwidth") = LinewidthToStr(this->GetTieLwidth()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTieRend::HasTieLform() const
{
    return (m_tieLform != LINEFORM_NONE);
}

bool AttTieRend::HasTieLwidth() const
{
    return (m_tieLwidth.HasValue());
}

/* include <atttie.lwidth> */

//----------------------------------------------------------------------------
// AttTremMeasured
//----------------------------------------------------------------------------

AttTremMeasured::AttTremMeasured() : Att()
{
    ResetTremMeasured();
}

AttTremMeasured::~AttTremMeasured()
{
}

void AttTremMeasured::ResetTremMeasured()
{
    m_unitdur = DURATION_NONE;
}

bool AttTremMeasured::ReadTremMeasured(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("unitdur")) {
        this->SetUnitdur(StrToDuration(element.attribute("unitdur").value()));
        element.remove_attribute("unitdur");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTremMeasured::WriteTremMeasured(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasUnitdur()) {
        element.append_attribute("unitdur") = DurationToStr(this->GetUnitdur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTremMeasured::HasUnitdur() const
{
    return (m_unitdur != DURATION_NONE);
}

/* include <attunitdur> */

bool Att::SetCmn(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_ARPEGLOG)) {
        AttArpegLog *att = dynamic_cast<AttArpegLog *>(element);
        assert(att);
        if (attrType == "order") {
            att->SetOrder(att->StrToArpegLogOrder(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BTREMLOG)) {
        AttBTremLog *att = dynamic_cast<AttBTremLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToBTremLogForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMPRESENT)) {
        AttBeamPresent *att = dynamic_cast<AttBeamPresent *>(element);
        assert(att);
        if (attrType == "beam") {
            att->SetBeam(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMREND)) {
        AttBeamRend *att = dynamic_cast<AttBeamRend *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToBeamRendForm(attrValue));
            return true;
        }
        if (attrType == "place") {
            att->SetPlace(att->StrToBeamplace(attrValue));
            return true;
        }
        if (attrType == "slash") {
            att->SetSlash(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "slope") {
            att->SetSlope(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMSECONDARY)) {
        AttBeamSecondary *att = dynamic_cast<AttBeamSecondary *>(element);
        assert(att);
        if (attrType == "breaksec") {
            att->SetBreaksec(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMEDWITH)) {
        AttBeamedWith *att = dynamic_cast<AttBeamedWith *>(element);
        assert(att);
        if (attrType == "beam.with") {
            att->SetBeamWith(att->StrToOtherstaff(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMINGLOG)) {
        AttBeamingLog *att = dynamic_cast<AttBeamingLog *>(element);
        assert(att);
        if (attrType == "beam.group") {
            att->SetBeamGroup(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "beam.rests") {
            att->SetBeamRests(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEATRPTLOG)) {
        AttBeatRptLog *att = dynamic_cast<AttBeatRptLog *>(element);
        assert(att);
        if (attrType == "beatdef") {
            att->SetBeatdef(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BRACKETSPANLOG)) {
        AttBracketSpanLog *att = dynamic_cast<AttBracketSpanLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CUTOUT)) {
        AttCutout *att = dynamic_cast<AttCutout *>(element);
        assert(att);
        if (attrType == "cutout") {
            att->SetCutout(att->StrToCutoutCutout(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_EXPANDABLE)) {
        AttExpandable *att = dynamic_cast<AttExpandable *>(element);
        assert(att);
        if (attrType == "expand") {
            att->SetExpand(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FTREMLOG)) {
        AttFTremLog *att = dynamic_cast<AttFTremLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToFTremLogForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_GLISSPRESENT)) {
        AttGlissPresent *att = dynamic_cast<AttGlissPresent *>(element);
        assert(att);
        if (attrType == "gliss") {
            att->SetGliss(att->StrToGlissando(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_GRACEGRPLOG)) {
        AttGraceGrpLog *att = dynamic_cast<AttGraceGrpLog *>(element);
        assert(att);
        if (attrType == "attach") {
            att->SetAttach(att->StrToGraceGrpLogAttach(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_GRACED)) {
        AttGraced *att = dynamic_cast<AttGraced *>(element);
        assert(att);
        if (attrType == "grace") {
            att->SetGrace(att->StrToGrace(attrValue));
            return true;
        }
        if (attrType == "grace.time") {
            att->SetGraceTime(att->StrToPercent(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HAIRPINLOG)) {
        AttHairpinLog *att = dynamic_cast<AttHairpinLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToHairpinLogForm(attrValue));
            return true;
        }
        if (attrType == "niente") {
            att->SetNiente(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HARPPEDALLOG)) {
        AttHarpPedalLog *att = dynamic_cast<AttHarpPedalLog *>(element);
        assert(att);
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
    if (element->HasAttClass(ATT_LVPRESENT)) {
        AttLvPresent *att = dynamic_cast<AttLvPresent *>(element);
        assert(att);
        if (attrType == "lv") {
            att->SetLv(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MEASURELOG)) {
        AttMeasureLog *att = dynamic_cast<AttMeasureLog *>(element);
        assert(att);
        if (attrType == "left") {
            att->SetLeft(att->StrToBarrendition(attrValue));
            return true;
        }
        if (attrType == "right") {
            att->SetRight(att->StrToBarrendition(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERSIGGRPLOG)) {
        AttMeterSigGrpLog *att = dynamic_cast<AttMeterSigGrpLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToMeterSigGrpLogFunc(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NUMBERPLACEMENT)) {
        AttNumberPlacement *att = dynamic_cast<AttNumberPlacement *>(element);
        assert(att);
        if (attrType == "num.place") {
            att->SetNumPlace(att->StrToStaffrelBasic(attrValue));
            return true;
        }
        if (attrType == "num.visible") {
            att->SetNumVisible(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NUMBERED)) {
        AttNumbered *att = dynamic_cast<AttNumbered *>(element);
        assert(att);
        if (attrType == "num") {
            att->SetNum(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_OCTAVELOG)) {
        AttOctaveLog *att = dynamic_cast<AttOctaveLog *>(element);
        assert(att);
        if (attrType == "coll") {
            att->SetColl(att->StrToOctaveLogColl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PEDALLOG)) {
        AttPedalLog *att = dynamic_cast<AttPedalLog *>(element);
        assert(att);
        if (attrType == "dir") {
            att->SetDir(att->StrToPedalLogDir(attrValue));
            return true;
        }
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PIANOPEDALS)) {
        AttPianoPedals *att = dynamic_cast<AttPianoPedals *>(element);
        assert(att);
        if (attrType == "pedal.style") {
            att->SetPedalStyle(att->StrToPianoPedalsPedalstyle(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_REHEARSAL)) {
        AttRehearsal *att = dynamic_cast<AttRehearsal *>(element);
        assert(att);
        if (attrType == "reh.enclose") {
            att->SetRehEnclose(att->StrToRehearsalRehenclose(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SCOREDEFVISCMN)) {
        AttScoreDefVisCmn *att = dynamic_cast<AttScoreDefVisCmn *>(element);
        assert(att);
        if (attrType == "grid.show") {
            att->SetGridShow(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SLURREND)) {
        AttSlurRend *att = dynamic_cast<AttSlurRend *>(element);
        assert(att);
        if (attrType == "slur.lform") {
            att->SetSlurLform(att->StrToLineform(attrValue));
            return true;
        }
        if (attrType == "slur.lwidth") {
            att->SetSlurLwidth(att->StrToLinewidth(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STEMSCMN)) {
        AttStemsCmn *att = dynamic_cast<AttStemsCmn *>(element);
        assert(att);
        if (attrType == "stem.with") {
            att->SetStemWith(att->StrToOtherstaff(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIEREND)) {
        AttTieRend *att = dynamic_cast<AttTieRend *>(element);
        assert(att);
        if (attrType == "tie.lform") {
            att->SetTieLform(att->StrToLineform(attrValue));
            return true;
        }
        if (attrType == "tie.lwidth") {
            att->SetTieLwidth(att->StrToLinewidth(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TREMMEASURED)) {
        AttTremMeasured *att = dynamic_cast<AttTremMeasured *>(element);
        assert(att);
        if (attrType == "unitdur") {
            att->SetUnitdur(att->StrToDuration(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetCmn(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ARPEGLOG)) {
        const AttArpegLog *att = dynamic_cast<const AttArpegLog *>(element);
        assert(att);
        if (att->HasOrder()) {
            attributes->push_back(std::make_pair("order", att->ArpegLogOrderToStr(att->GetOrder())));
        }
    }
    if (element->HasAttClass(ATT_BTREMLOG)) {
        const AttBTremLog *att = dynamic_cast<const AttBTremLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->BTremLogFormToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass(ATT_BEAMPRESENT)) {
        const AttBeamPresent *att = dynamic_cast<const AttBeamPresent *>(element);
        assert(att);
        if (att->HasBeam()) {
            attributes->push_back(std::make_pair("beam", att->StrToStr(att->GetBeam())));
        }
    }
    if (element->HasAttClass(ATT_BEAMREND)) {
        const AttBeamRend *att = dynamic_cast<const AttBeamRend *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->BeamRendFormToStr(att->GetForm())));
        }
        if (att->HasPlace()) {
            attributes->push_back(std::make_pair("place", att->BeamplaceToStr(att->GetPlace())));
        }
        if (att->HasSlash()) {
            attributes->push_back(std::make_pair("slash", att->BooleanToStr(att->GetSlash())));
        }
        if (att->HasSlope()) {
            attributes->push_back(std::make_pair("slope", att->DblToStr(att->GetSlope())));
        }
    }
    if (element->HasAttClass(ATT_BEAMSECONDARY)) {
        const AttBeamSecondary *att = dynamic_cast<const AttBeamSecondary *>(element);
        assert(att);
        if (att->HasBreaksec()) {
            attributes->push_back(std::make_pair("breaksec", att->IntToStr(att->GetBreaksec())));
        }
    }
    if (element->HasAttClass(ATT_BEAMEDWITH)) {
        const AttBeamedWith *att = dynamic_cast<const AttBeamedWith *>(element);
        assert(att);
        if (att->HasBeamWith()) {
            attributes->push_back(std::make_pair("beam.with", att->OtherstaffToStr(att->GetBeamWith())));
        }
    }
    if (element->HasAttClass(ATT_BEAMINGLOG)) {
        const AttBeamingLog *att = dynamic_cast<const AttBeamingLog *>(element);
        assert(att);
        if (att->HasBeamGroup()) {
            attributes->push_back(std::make_pair("beam.group", att->StrToStr(att->GetBeamGroup())));
        }
        if (att->HasBeamRests()) {
            attributes->push_back(std::make_pair("beam.rests", att->BooleanToStr(att->GetBeamRests())));
        }
    }
    if (element->HasAttClass(ATT_BEATRPTLOG)) {
        const AttBeatRptLog *att = dynamic_cast<const AttBeatRptLog *>(element);
        assert(att);
        if (att->HasBeatdef()) {
            attributes->push_back(std::make_pair("beatdef", att->DblToStr(att->GetBeatdef())));
        }
    }
    if (element->HasAttClass(ATT_BRACKETSPANLOG)) {
        const AttBracketSpanLog *att = dynamic_cast<const AttBracketSpanLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back(std::make_pair("func", att->StrToStr(att->GetFunc())));
        }
    }
    if (element->HasAttClass(ATT_CUTOUT)) {
        const AttCutout *att = dynamic_cast<const AttCutout *>(element);
        assert(att);
        if (att->HasCutout()) {
            attributes->push_back(std::make_pair("cutout", att->CutoutCutoutToStr(att->GetCutout())));
        }
    }
    if (element->HasAttClass(ATT_EXPANDABLE)) {
        const AttExpandable *att = dynamic_cast<const AttExpandable *>(element);
        assert(att);
        if (att->HasExpand()) {
            attributes->push_back(std::make_pair("expand", att->BooleanToStr(att->GetExpand())));
        }
    }
    if (element->HasAttClass(ATT_FTREMLOG)) {
        const AttFTremLog *att = dynamic_cast<const AttFTremLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->FTremLogFormToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass(ATT_GLISSPRESENT)) {
        const AttGlissPresent *att = dynamic_cast<const AttGlissPresent *>(element);
        assert(att);
        if (att->HasGliss()) {
            attributes->push_back(std::make_pair("gliss", att->GlissandoToStr(att->GetGliss())));
        }
    }
    if (element->HasAttClass(ATT_GRACEGRPLOG)) {
        const AttGraceGrpLog *att = dynamic_cast<const AttGraceGrpLog *>(element);
        assert(att);
        if (att->HasAttach()) {
            attributes->push_back(std::make_pair("attach", att->GraceGrpLogAttachToStr(att->GetAttach())));
        }
    }
    if (element->HasAttClass(ATT_GRACED)) {
        const AttGraced *att = dynamic_cast<const AttGraced *>(element);
        assert(att);
        if (att->HasGrace()) {
            attributes->push_back(std::make_pair("grace", att->GraceToStr(att->GetGrace())));
        }
        if (att->HasGraceTime()) {
            attributes->push_back(std::make_pair("grace.time", att->PercentToStr(att->GetGraceTime())));
        }
    }
    if (element->HasAttClass(ATT_HAIRPINLOG)) {
        const AttHairpinLog *att = dynamic_cast<const AttHairpinLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->HairpinLogFormToStr(att->GetForm())));
        }
        if (att->HasNiente()) {
            attributes->push_back(std::make_pair("niente", att->BooleanToStr(att->GetNiente())));
        }
    }
    if (element->HasAttClass(ATT_HARPPEDALLOG)) {
        const AttHarpPedalLog *att = dynamic_cast<const AttHarpPedalLog *>(element);
        assert(att);
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
    if (element->HasAttClass(ATT_LVPRESENT)) {
        const AttLvPresent *att = dynamic_cast<const AttLvPresent *>(element);
        assert(att);
        if (att->HasLv()) {
            attributes->push_back(std::make_pair("lv", att->BooleanToStr(att->GetLv())));
        }
    }
    if (element->HasAttClass(ATT_MEASURELOG)) {
        const AttMeasureLog *att = dynamic_cast<const AttMeasureLog *>(element);
        assert(att);
        if (att->HasLeft()) {
            attributes->push_back(std::make_pair("left", att->BarrenditionToStr(att->GetLeft())));
        }
        if (att->HasRight()) {
            attributes->push_back(std::make_pair("right", att->BarrenditionToStr(att->GetRight())));
        }
    }
    if (element->HasAttClass(ATT_METERSIGGRPLOG)) {
        const AttMeterSigGrpLog *att = dynamic_cast<const AttMeterSigGrpLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back(std::make_pair("func", att->MeterSigGrpLogFuncToStr(att->GetFunc())));
        }
    }
    if (element->HasAttClass(ATT_NUMBERPLACEMENT)) {
        const AttNumberPlacement *att = dynamic_cast<const AttNumberPlacement *>(element);
        assert(att);
        if (att->HasNumPlace()) {
            attributes->push_back(std::make_pair("num.place", att->StaffrelBasicToStr(att->GetNumPlace())));
        }
        if (att->HasNumVisible()) {
            attributes->push_back(std::make_pair("num.visible", att->BooleanToStr(att->GetNumVisible())));
        }
    }
    if (element->HasAttClass(ATT_NUMBERED)) {
        const AttNumbered *att = dynamic_cast<const AttNumbered *>(element);
        assert(att);
        if (att->HasNum()) {
            attributes->push_back(std::make_pair("num", att->IntToStr(att->GetNum())));
        }
    }
    if (element->HasAttClass(ATT_OCTAVELOG)) {
        const AttOctaveLog *att = dynamic_cast<const AttOctaveLog *>(element);
        assert(att);
        if (att->HasColl()) {
            attributes->push_back(std::make_pair("coll", att->OctaveLogCollToStr(att->GetColl())));
        }
    }
    if (element->HasAttClass(ATT_PEDALLOG)) {
        const AttPedalLog *att = dynamic_cast<const AttPedalLog *>(element);
        assert(att);
        if (att->HasDir()) {
            attributes->push_back(std::make_pair("dir", att->PedalLogDirToStr(att->GetDir())));
        }
        if (att->HasFunc()) {
            attributes->push_back(std::make_pair("func", att->StrToStr(att->GetFunc())));
        }
    }
    if (element->HasAttClass(ATT_PIANOPEDALS)) {
        const AttPianoPedals *att = dynamic_cast<const AttPianoPedals *>(element);
        assert(att);
        if (att->HasPedalStyle()) {
            attributes->push_back(std::make_pair("pedal.style", att->PianoPedalsPedalstyleToStr(att->GetPedalStyle())));
        }
    }
    if (element->HasAttClass(ATT_REHEARSAL)) {
        const AttRehearsal *att = dynamic_cast<const AttRehearsal *>(element);
        assert(att);
        if (att->HasRehEnclose()) {
            attributes->push_back(std::make_pair("reh.enclose", att->RehearsalRehencloseToStr(att->GetRehEnclose())));
        }
    }
    if (element->HasAttClass(ATT_SCOREDEFVISCMN)) {
        const AttScoreDefVisCmn *att = dynamic_cast<const AttScoreDefVisCmn *>(element);
        assert(att);
        if (att->HasGridShow()) {
            attributes->push_back(std::make_pair("grid.show", att->BooleanToStr(att->GetGridShow())));
        }
    }
    if (element->HasAttClass(ATT_SLURREND)) {
        const AttSlurRend *att = dynamic_cast<const AttSlurRend *>(element);
        assert(att);
        if (att->HasSlurLform()) {
            attributes->push_back(std::make_pair("slur.lform", att->LineformToStr(att->GetSlurLform())));
        }
        if (att->HasSlurLwidth()) {
            attributes->push_back(std::make_pair("slur.lwidth", att->LinewidthToStr(att->GetSlurLwidth())));
        }
    }
    if (element->HasAttClass(ATT_STEMSCMN)) {
        const AttStemsCmn *att = dynamic_cast<const AttStemsCmn *>(element);
        assert(att);
        if (att->HasStemWith()) {
            attributes->push_back(std::make_pair("stem.with", att->OtherstaffToStr(att->GetStemWith())));
        }
    }
    if (element->HasAttClass(ATT_TIEREND)) {
        const AttTieRend *att = dynamic_cast<const AttTieRend *>(element);
        assert(att);
        if (att->HasTieLform()) {
            attributes->push_back(std::make_pair("tie.lform", att->LineformToStr(att->GetTieLform())));
        }
        if (att->HasTieLwidth()) {
            attributes->push_back(std::make_pair("tie.lwidth", att->LinewidthToStr(att->GetTieLwidth())));
        }
    }
    if (element->HasAttClass(ATT_TREMMEASURED)) {
        const AttTremMeasured *att = dynamic_cast<const AttTremMeasured *>(element);
        assert(att);
        if (att->HasUnitdur()) {
            attributes->push_back(std::make_pair("unitdur", att->DurationToStr(att->GetUnitdur())));
        }
    }
}

} // vrv namespace
