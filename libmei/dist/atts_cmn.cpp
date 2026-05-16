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

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttArpegLog
//----------------------------------------------------------------------------

AttArpegLog::AttArpegLog() : Att()
{
    this->ResetArpegLog();
}

void AttArpegLog::ResetArpegLog()
{
    m_order = arpegLog_ORDER_NONE;
}

bool AttArpegLog::ReadArpegLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("order")) {
        this->SetOrder(StrToArpegLogOrder(element.attribute("order").value()));
        if (removeAttr) element.remove_attribute("order");
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

//----------------------------------------------------------------------------
// AttBeamPresent
//----------------------------------------------------------------------------

AttBeamPresent::AttBeamPresent() : Att()
{
    this->ResetBeamPresent();
}

void AttBeamPresent::ResetBeamPresent()
{
    m_beam = "";
}

bool AttBeamPresent::ReadBeamPresent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("beam")) {
        this->SetBeam(StrToStr(element.attribute("beam").value()));
        if (removeAttr) element.remove_attribute("beam");
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

//----------------------------------------------------------------------------
// AttBeamRend
//----------------------------------------------------------------------------

AttBeamRend::AttBeamRend() : Att()
{
    this->ResetBeamRend();
}

void AttBeamRend::ResetBeamRend()
{
    m_form = beamRend_FORM_NONE;
    m_place = BEAMPLACE_NONE;
    m_slash = BOOLEAN_NONE;
    m_slope = 0.0;
}

bool AttBeamRend::ReadBeamRend(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToBeamRendForm(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("place")) {
        this->SetPlace(StrToBeamplace(element.attribute("place").value()));
        if (removeAttr) element.remove_attribute("place");
        hasAttribute = true;
    }
    if (element.attribute("slash")) {
        this->SetSlash(StrToBoolean(element.attribute("slash").value()));
        if (removeAttr) element.remove_attribute("slash");
        hasAttribute = true;
    }
    if (element.attribute("slope")) {
        this->SetSlope(StrToDbl(element.attribute("slope").value()));
        if (removeAttr) element.remove_attribute("slope");
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

//----------------------------------------------------------------------------
// AttBeamSecondary
//----------------------------------------------------------------------------

AttBeamSecondary::AttBeamSecondary() : Att()
{
    this->ResetBeamSecondary();
}

void AttBeamSecondary::ResetBeamSecondary()
{
    m_breaksec = MEI_UNSET;
}

bool AttBeamSecondary::ReadBeamSecondary(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("breaksec")) {
        this->SetBreaksec(StrToInt(element.attribute("breaksec").value()));
        if (removeAttr) element.remove_attribute("breaksec");
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
    return (m_breaksec != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttBeamedWith
//----------------------------------------------------------------------------

AttBeamedWith::AttBeamedWith() : Att()
{
    this->ResetBeamedWith();
}

void AttBeamedWith::ResetBeamedWith()
{
    m_beamWith = NEIGHBORINGLAYER_NONE;
}

bool AttBeamedWith::ReadBeamedWith(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("beam.with")) {
        this->SetBeamWith(StrToNeighboringlayer(element.attribute("beam.with").value()));
        if (removeAttr) element.remove_attribute("beam.with");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamedWith::WriteBeamedWith(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBeamWith()) {
        element.append_attribute("beam.with") = NeighboringlayerToStr(this->GetBeamWith()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamedWith::HasBeamWith() const
{
    return (m_beamWith != NEIGHBORINGLAYER_NONE);
}

//----------------------------------------------------------------------------
// AttBeamingLog
//----------------------------------------------------------------------------

AttBeamingLog::AttBeamingLog() : Att()
{
    this->ResetBeamingLog();
}

void AttBeamingLog::ResetBeamingLog()
{
    m_beamGroup = "";
    m_beamRests = BOOLEAN_NONE;
}

bool AttBeamingLog::ReadBeamingLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("beam.group")) {
        this->SetBeamGroup(StrToStr(element.attribute("beam.group").value()));
        if (removeAttr) element.remove_attribute("beam.group");
        hasAttribute = true;
    }
    if (element.attribute("beam.rests")) {
        this->SetBeamRests(StrToBoolean(element.attribute("beam.rests").value()));
        if (removeAttr) element.remove_attribute("beam.rests");
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

//----------------------------------------------------------------------------
// AttBeatRptLog
//----------------------------------------------------------------------------

AttBeatRptLog::AttBeatRptLog() : Att()
{
    this->ResetBeatRptLog();
}

void AttBeatRptLog::ResetBeatRptLog()
{
    m_beatdef = 0.0;
}

bool AttBeatRptLog::ReadBeatRptLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("beatdef")) {
        this->SetBeatdef(StrToDbl(element.attribute("beatdef").value()));
        if (removeAttr) element.remove_attribute("beatdef");
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

//----------------------------------------------------------------------------
// AttBracketSpanLog
//----------------------------------------------------------------------------

AttBracketSpanLog::AttBracketSpanLog() : Att()
{
    this->ResetBracketSpanLog();
}

void AttBracketSpanLog::ResetBracketSpanLog()
{
    m_func = bracketSpanLog_FUNC_NONE;
}

bool AttBracketSpanLog::ReadBracketSpanLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToBracketSpanLogFunc(element.attribute("func").value()));
        if (removeAttr) element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBracketSpanLog::WriteBracketSpanLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = BracketSpanLogFuncToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBracketSpanLog::HasFunc() const
{
    return (m_func != bracketSpanLog_FUNC_NONE);
}

//----------------------------------------------------------------------------
// AttCutout
//----------------------------------------------------------------------------

AttCutout::AttCutout() : Att()
{
    this->ResetCutout();
}

void AttCutout::ResetCutout()
{
    m_cutout = cutout_CUTOUT_NONE;
}

bool AttCutout::ReadCutout(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("cutout")) {
        this->SetCutout(StrToCutoutCutout(element.attribute("cutout").value()));
        if (removeAttr) element.remove_attribute("cutout");
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

//----------------------------------------------------------------------------
// AttExpandable
//----------------------------------------------------------------------------

AttExpandable::AttExpandable() : Att()
{
    this->ResetExpandable();
}

void AttExpandable::ResetExpandable()
{
    m_expand = BOOLEAN_NONE;
}

bool AttExpandable::ReadExpandable(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("expand")) {
        this->SetExpand(StrToBoolean(element.attribute("expand").value()));
        if (removeAttr) element.remove_attribute("expand");
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

//----------------------------------------------------------------------------
// AttGlissPresent
//----------------------------------------------------------------------------

AttGlissPresent::AttGlissPresent() : Att()
{
    this->ResetGlissPresent();
}

void AttGlissPresent::ResetGlissPresent()
{
    m_gliss = GLISSANDO_NONE;
}

bool AttGlissPresent::ReadGlissPresent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("gliss")) {
        this->SetGliss(StrToGlissando(element.attribute("gliss").value()));
        if (removeAttr) element.remove_attribute("gliss");
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

//----------------------------------------------------------------------------
// AttGraceGrpLog
//----------------------------------------------------------------------------

AttGraceGrpLog::AttGraceGrpLog() : Att()
{
    this->ResetGraceGrpLog();
}

void AttGraceGrpLog::ResetGraceGrpLog()
{
    m_attach = graceGrpLog_ATTACH_NONE;
}

bool AttGraceGrpLog::ReadGraceGrpLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("attach")) {
        this->SetAttach(StrToGraceGrpLogAttach(element.attribute("attach").value()));
        if (removeAttr) element.remove_attribute("attach");
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

//----------------------------------------------------------------------------
// AttGraced
//----------------------------------------------------------------------------

AttGraced::AttGraced() : Att()
{
    this->ResetGraced();
}

void AttGraced::ResetGraced()
{
    m_grace = GRACE_NONE;
    m_graceTime = -1.0;
}

bool AttGraced::ReadGraced(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("grace")) {
        this->SetGrace(StrToGrace(element.attribute("grace").value()));
        if (removeAttr) element.remove_attribute("grace");
        hasAttribute = true;
    }
    if (element.attribute("grace.time")) {
        this->SetGraceTime(StrToPercent(element.attribute("grace.time").value()));
        if (removeAttr) element.remove_attribute("grace.time");
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

//----------------------------------------------------------------------------
// AttHairpinLog
//----------------------------------------------------------------------------

AttHairpinLog::AttHairpinLog() : Att()
{
    this->ResetHairpinLog();
}

void AttHairpinLog::ResetHairpinLog()
{
    m_form = hairpinLog_FORM_NONE;
    m_niente = BOOLEAN_NONE;
}

bool AttHairpinLog::ReadHairpinLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToHairpinLogForm(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("niente")) {
        this->SetNiente(StrToBoolean(element.attribute("niente").value()));
        if (removeAttr) element.remove_attribute("niente");
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

//----------------------------------------------------------------------------
// AttHarpPedalLog
//----------------------------------------------------------------------------

AttHarpPedalLog::AttHarpPedalLog() : Att()
{
    this->ResetHarpPedalLog();
}

void AttHarpPedalLog::ResetHarpPedalLog()
{
    m_c = HARPPEDALPOSITION_NONE;
    m_d = HARPPEDALPOSITION_NONE;
    m_e = HARPPEDALPOSITION_NONE;
    m_f = HARPPEDALPOSITION_NONE;
    m_g = HARPPEDALPOSITION_NONE;
    m_a = HARPPEDALPOSITION_NONE;
    m_b = HARPPEDALPOSITION_NONE;
}

bool AttHarpPedalLog::ReadHarpPedalLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("c")) {
        this->SetC(StrToHarppedalposition(element.attribute("c").value()));
        if (removeAttr) element.remove_attribute("c");
        hasAttribute = true;
    }
    if (element.attribute("d")) {
        this->SetD(StrToHarppedalposition(element.attribute("d").value()));
        if (removeAttr) element.remove_attribute("d");
        hasAttribute = true;
    }
    if (element.attribute("e")) {
        this->SetE(StrToHarppedalposition(element.attribute("e").value()));
        if (removeAttr) element.remove_attribute("e");
        hasAttribute = true;
    }
    if (element.attribute("f")) {
        this->SetF(StrToHarppedalposition(element.attribute("f").value()));
        if (removeAttr) element.remove_attribute("f");
        hasAttribute = true;
    }
    if (element.attribute("g")) {
        this->SetG(StrToHarppedalposition(element.attribute("g").value()));
        if (removeAttr) element.remove_attribute("g");
        hasAttribute = true;
    }
    if (element.attribute("a")) {
        this->SetA(StrToHarppedalposition(element.attribute("a").value()));
        if (removeAttr) element.remove_attribute("a");
        hasAttribute = true;
    }
    if (element.attribute("b")) {
        this->SetB(StrToHarppedalposition(element.attribute("b").value()));
        if (removeAttr) element.remove_attribute("b");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarpPedalLog::WriteHarpPedalLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasC()) {
        element.append_attribute("c") = HarppedalpositionToStr(this->GetC()).c_str();
        wroteAttribute = true;
    }
    if (this->HasD()) {
        element.append_attribute("d") = HarppedalpositionToStr(this->GetD()).c_str();
        wroteAttribute = true;
    }
    if (this->HasE()) {
        element.append_attribute("e") = HarppedalpositionToStr(this->GetE()).c_str();
        wroteAttribute = true;
    }
    if (this->HasF()) {
        element.append_attribute("f") = HarppedalpositionToStr(this->GetF()).c_str();
        wroteAttribute = true;
    }
    if (this->HasG()) {
        element.append_attribute("g") = HarppedalpositionToStr(this->GetG()).c_str();
        wroteAttribute = true;
    }
    if (this->HasA()) {
        element.append_attribute("a") = HarppedalpositionToStr(this->GetA()).c_str();
        wroteAttribute = true;
    }
    if (this->HasB()) {
        element.append_attribute("b") = HarppedalpositionToStr(this->GetB()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarpPedalLog::HasC() const
{
    return (m_c != HARPPEDALPOSITION_NONE);
}

bool AttHarpPedalLog::HasD() const
{
    return (m_d != HARPPEDALPOSITION_NONE);
}

bool AttHarpPedalLog::HasE() const
{
    return (m_e != HARPPEDALPOSITION_NONE);
}

bool AttHarpPedalLog::HasF() const
{
    return (m_f != HARPPEDALPOSITION_NONE);
}

bool AttHarpPedalLog::HasG() const
{
    return (m_g != HARPPEDALPOSITION_NONE);
}

bool AttHarpPedalLog::HasA() const
{
    return (m_a != HARPPEDALPOSITION_NONE);
}

bool AttHarpPedalLog::HasB() const
{
    return (m_b != HARPPEDALPOSITION_NONE);
}

//----------------------------------------------------------------------------
// AttLvPresent
//----------------------------------------------------------------------------

AttLvPresent::AttLvPresent() : Att()
{
    this->ResetLvPresent();
}

void AttLvPresent::ResetLvPresent()
{
    m_lv = BOOLEAN_NONE;
}

bool AttLvPresent::ReadLvPresent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("lv")) {
        this->SetLv(StrToBoolean(element.attribute("lv").value()));
        if (removeAttr) element.remove_attribute("lv");
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

//----------------------------------------------------------------------------
// AttMeasureLog
//----------------------------------------------------------------------------

AttMeasureLog::AttMeasureLog() : Att()
{
    this->ResetMeasureLog();
}

void AttMeasureLog::ResetMeasureLog()
{
    m_left = BARRENDITION_NONE;
    m_right = BARRENDITION_NONE;
}

bool AttMeasureLog::ReadMeasureLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("left")) {
        this->SetLeft(StrToBarrendition(element.attribute("left").value()));
        if (removeAttr) element.remove_attribute("left");
        hasAttribute = true;
    }
    if (element.attribute("right")) {
        this->SetRight(StrToBarrendition(element.attribute("right").value()));
        if (removeAttr) element.remove_attribute("right");
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

//----------------------------------------------------------------------------
// AttMeterSigGrpLog
//----------------------------------------------------------------------------

AttMeterSigGrpLog::AttMeterSigGrpLog() : Att()
{
    this->ResetMeterSigGrpLog();
}

void AttMeterSigGrpLog::ResetMeterSigGrpLog()
{
    m_func = meterSigGrpLog_FUNC_NONE;
}

bool AttMeterSigGrpLog::ReadMeterSigGrpLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToMeterSigGrpLogFunc(element.attribute("func").value()));
        if (removeAttr) element.remove_attribute("func");
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

//----------------------------------------------------------------------------
// AttNumberPlacement
//----------------------------------------------------------------------------

AttNumberPlacement::AttNumberPlacement() : Att()
{
    this->ResetNumberPlacement();
}

void AttNumberPlacement::ResetNumberPlacement()
{
    m_numPlace = STAFFREL_basic_NONE;
    m_numVisible = BOOLEAN_NONE;
}

bool AttNumberPlacement::ReadNumberPlacement(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("num.place")) {
        this->SetNumPlace(StrToStaffrelBasic(element.attribute("num.place").value()));
        if (removeAttr) element.remove_attribute("num.place");
        hasAttribute = true;
    }
    if (element.attribute("num.visible")) {
        this->SetNumVisible(StrToBoolean(element.attribute("num.visible").value()));
        if (removeAttr) element.remove_attribute("num.visible");
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

//----------------------------------------------------------------------------
// AttNumbered
//----------------------------------------------------------------------------

AttNumbered::AttNumbered() : Att()
{
    this->ResetNumbered();
}

void AttNumbered::ResetNumbered()
{
    m_num = MEI_UNSET;
}

bool AttNumbered::ReadNumbered(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNum(StrToInt(element.attribute("num").value()));
        if (removeAttr) element.remove_attribute("num");
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
    return (m_num != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttOctaveLog
//----------------------------------------------------------------------------

AttOctaveLog::AttOctaveLog() : Att()
{
    this->ResetOctaveLog();
}

void AttOctaveLog::ResetOctaveLog()
{
    m_coll = octaveLog_COLL_NONE;
}

bool AttOctaveLog::ReadOctaveLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("coll")) {
        this->SetColl(StrToOctaveLogColl(element.attribute("coll").value()));
        if (removeAttr) element.remove_attribute("coll");
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

//----------------------------------------------------------------------------
// AttPedalLog
//----------------------------------------------------------------------------

AttPedalLog::AttPedalLog() : Att()
{
    this->ResetPedalLog();
}

void AttPedalLog::ResetPedalLog()
{
    m_dir = pedalLog_DIR_NONE;
    m_func = "";
}

bool AttPedalLog::ReadPedalLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dir")) {
        this->SetDir(StrToPedalLogDir(element.attribute("dir").value()));
        if (removeAttr) element.remove_attribute("dir");
        hasAttribute = true;
    }
    if (element.attribute("func")) {
        this->SetFunc(StrToStr(element.attribute("func").value()));
        if (removeAttr) element.remove_attribute("func");
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

//----------------------------------------------------------------------------
// AttPianoPedals
//----------------------------------------------------------------------------

AttPianoPedals::AttPianoPedals() : Att()
{
    this->ResetPianoPedals();
}

void AttPianoPedals::ResetPianoPedals()
{
    m_pedalStyle = PEDALSTYLE_NONE;
}

bool AttPianoPedals::ReadPianoPedals(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("pedal.style")) {
        this->SetPedalStyle(StrToPedalstyle(element.attribute("pedal.style").value()));
        if (removeAttr) element.remove_attribute("pedal.style");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPianoPedals::WritePianoPedals(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPedalStyle()) {
        element.append_attribute("pedal.style") = PedalstyleToStr(this->GetPedalStyle()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPianoPedals::HasPedalStyle() const
{
    return (m_pedalStyle != PEDALSTYLE_NONE);
}

//----------------------------------------------------------------------------
// AttRehearsal
//----------------------------------------------------------------------------

AttRehearsal::AttRehearsal() : Att()
{
    this->ResetRehearsal();
}

void AttRehearsal::ResetRehearsal()
{
    m_rehEnclose = rehearsal_REHENCLOSE_NONE;
}

bool AttRehearsal::ReadRehearsal(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("reh.enclose")) {
        this->SetRehEnclose(StrToRehearsalRehenclose(element.attribute("reh.enclose").value()));
        if (removeAttr) element.remove_attribute("reh.enclose");
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

//----------------------------------------------------------------------------
// AttSlurRend
//----------------------------------------------------------------------------

AttSlurRend::AttSlurRend() : Att()
{
    this->ResetSlurRend();
}

void AttSlurRend::ResetSlurRend()
{
    m_slurLform = LINEFORM_NONE;
    m_slurLwidth = data_LINEWIDTH();
}

bool AttSlurRend::ReadSlurRend(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("slur.lform")) {
        this->SetSlurLform(StrToLineform(element.attribute("slur.lform").value()));
        if (removeAttr) element.remove_attribute("slur.lform");
        hasAttribute = true;
    }
    if (element.attribute("slur.lwidth")) {
        this->SetSlurLwidth(StrToLinewidth(element.attribute("slur.lwidth").value()));
        if (removeAttr) element.remove_attribute("slur.lwidth");
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

//----------------------------------------------------------------------------
// AttStemsCmn
//----------------------------------------------------------------------------

AttStemsCmn::AttStemsCmn() : Att()
{
    this->ResetStemsCmn();
}

void AttStemsCmn::ResetStemsCmn()
{
    m_stemWith = NEIGHBORINGLAYER_NONE;
}

bool AttStemsCmn::ReadStemsCmn(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("stem.with")) {
        this->SetStemWith(StrToNeighboringlayer(element.attribute("stem.with").value()));
        if (removeAttr) element.remove_attribute("stem.with");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStemsCmn::WriteStemsCmn(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasStemWith()) {
        element.append_attribute("stem.with") = NeighboringlayerToStr(this->GetStemWith()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStemsCmn::HasStemWith() const
{
    return (m_stemWith != NEIGHBORINGLAYER_NONE);
}

//----------------------------------------------------------------------------
// AttTieRend
//----------------------------------------------------------------------------

AttTieRend::AttTieRend() : Att()
{
    this->ResetTieRend();
}

void AttTieRend::ResetTieRend()
{
    m_tieLform = LINEFORM_NONE;
    m_tieLwidth = data_LINEWIDTH();
}

bool AttTieRend::ReadTieRend(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("tie.lform")) {
        this->SetTieLform(StrToLineform(element.attribute("tie.lform").value()));
        if (removeAttr) element.remove_attribute("tie.lform");
        hasAttribute = true;
    }
    if (element.attribute("tie.lwidth")) {
        this->SetTieLwidth(StrToLinewidth(element.attribute("tie.lwidth").value()));
        if (removeAttr) element.remove_attribute("tie.lwidth");
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

//----------------------------------------------------------------------------
// AttTremForm
//----------------------------------------------------------------------------

AttTremForm::AttTremForm() : Att()
{
    this->ResetTremForm();
}

void AttTremForm::ResetTremForm()
{
    m_form = tremForm_FORM_NONE;
}

bool AttTremForm::ReadTremForm(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToTremFormForm(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTremForm::WriteTremForm(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = TremFormFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTremForm::HasForm() const
{
    return (m_form != tremForm_FORM_NONE);
}

//----------------------------------------------------------------------------
// AttTremMeasured
//----------------------------------------------------------------------------

AttTremMeasured::AttTremMeasured() : Att()
{
    this->ResetTremMeasured();
}

void AttTremMeasured::ResetTremMeasured()
{
    m_unitdur = DURATION_NONE;
}

bool AttTremMeasured::ReadTremMeasured(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("unitdur")) {
        this->SetUnitdur(StrToDuration(element.attribute("unitdur").value()));
        if (removeAttr) element.remove_attribute("unitdur");
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

} // namespace vrv
