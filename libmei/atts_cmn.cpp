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
    m_beatdef = DURATION_NONE;
}

bool AttBeatRptLog::ReadBeatRptLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("beatdef")) {
        this->SetBeatdef(StrToDuration(element.attribute("beatdef").value()));
        element.remove_attribute("beatdef");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeatRptLog::WriteBeatRptLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBeatdef()) {
        element.append_attribute("beatdef") = DurationToStr(this->GetBeatdef()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeatRptLog::HasBeatdef() const
{
    return (m_beatdef != DURATION_NONE);
}

/* include <attbeatdef> */

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
    m_graceTime = 0;
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
    return (m_graceTime != 0);
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

bool Att::SetCmn(Object *element, std::string attrType, std::string attrValue)
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
            att->SetBeatdef(att->StrToDuration(attrValue));
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
    if (element->HasAttClass(ATT_METERSIGGRPLOG)) {
        AttMeterSigGrpLog *att = dynamic_cast<AttMeterSigGrpLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToMeterSigGrpLogFunc(attrValue));
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
            attributes->push_back(std::make_pair("beatdef", att->DurationToStr(att->GetBeatdef())));
        }
    }
    if (element->HasAttClass(ATT_FTREMLOG)) {
        const AttFTremLog *att = dynamic_cast<const AttFTremLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->FTremLogFormToStr(att->GetForm())));
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
    if (element->HasAttClass(ATT_METERSIGGRPLOG)) {
        const AttMeterSigGrpLog *att = dynamic_cast<const AttMeterSigGrpLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back(std::make_pair("func", att->MeterSigGrpLogFuncToStr(att->GetFunc())));
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
}

} // vrv namespace
