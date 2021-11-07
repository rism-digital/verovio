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

#include "atts_midi.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttChannelized
//----------------------------------------------------------------------------

AttChannelized::AttChannelized() : Att()
{
    ResetChannelized();
}

AttChannelized::~AttChannelized()
{
}

void AttChannelized::ResetChannelized()
{
    m_midiChannel = -1;
    m_midiDuty = -1.0;
    m_midiPort = data_MIDIVALUE_NAME();
    m_midiTrack = 0;
}

bool AttChannelized::ReadChannelized(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("midi.channel")) {
        this->SetMidiChannel(StrToMidichannel(element.attribute("midi.channel").value()));
        element.remove_attribute("midi.channel");
        hasAttribute = true;
    }
    if (element.attribute("midi.duty")) {
        this->SetMidiDuty(StrToPercentLimited(element.attribute("midi.duty").value()));
        element.remove_attribute("midi.duty");
        hasAttribute = true;
    }
    if (element.attribute("midi.port")) {
        this->SetMidiPort(StrToMidivalueName(element.attribute("midi.port").value()));
        element.remove_attribute("midi.port");
        hasAttribute = true;
    }
    if (element.attribute("midi.track")) {
        this->SetMidiTrack(StrToInt(element.attribute("midi.track").value()));
        element.remove_attribute("midi.track");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttChannelized::WriteChannelized(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMidiChannel()) {
        element.append_attribute("midi.channel") = MidichannelToStr(this->GetMidiChannel()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiDuty()) {
        element.append_attribute("midi.duty") = PercentLimitedToStr(this->GetMidiDuty()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiPort()) {
        element.append_attribute("midi.port") = MidivalueNameToStr(this->GetMidiPort()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiTrack()) {
        element.append_attribute("midi.track") = IntToStr(this->GetMidiTrack()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttChannelized::HasMidiChannel() const
{
    return (m_midiChannel != -1);
}

bool AttChannelized::HasMidiDuty() const
{
    return (m_midiDuty != -1.0);
}

bool AttChannelized::HasMidiPort() const
{
    return (m_midiPort != data_MIDIVALUE_NAME());
}

bool AttChannelized::HasMidiTrack() const
{
    return (m_midiTrack != 0);
}

/* include <attmidi.track> */

//----------------------------------------------------------------------------
// AttInstrumentIdent
//----------------------------------------------------------------------------

AttInstrumentIdent::AttInstrumentIdent() : Att()
{
    ResetInstrumentIdent();
}

AttInstrumentIdent::~AttInstrumentIdent()
{
}

void AttInstrumentIdent::ResetInstrumentIdent()
{
    m_instr = "";
}

bool AttInstrumentIdent::ReadInstrumentIdent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("instr")) {
        this->SetInstr(StrToStr(element.attribute("instr").value()));
        element.remove_attribute("instr");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttInstrumentIdent::WriteInstrumentIdent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasInstr()) {
        element.append_attribute("instr") = StrToStr(this->GetInstr()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttInstrumentIdent::HasInstr() const
{
    return (m_instr != "");
}

/* include <attinstr> */

//----------------------------------------------------------------------------
// AttMidiInstrument
//----------------------------------------------------------------------------

AttMidiInstrument::AttMidiInstrument() : Att()
{
    ResetMidiInstrument();
}

AttMidiInstrument::~AttMidiInstrument()
{
}

void AttMidiInstrument::ResetMidiInstrument()
{
    m_midiInstrnum = -1;
    m_midiInstrname = MIDINAMES_NONE;
    m_midiPan = data_MIDIVALUE_PAN();
    m_midiPatchname = "";
    m_midiPatchnum = -1;
    m_midiVolume = -1.0;
}

bool AttMidiInstrument::ReadMidiInstrument(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("midi.instrnum")) {
        this->SetMidiInstrnum(StrToMidivalue(element.attribute("midi.instrnum").value()));
        element.remove_attribute("midi.instrnum");
        hasAttribute = true;
    }
    if (element.attribute("midi.instrname")) {
        this->SetMidiInstrname(StrToMidinames(element.attribute("midi.instrname").value()));
        element.remove_attribute("midi.instrname");
        hasAttribute = true;
    }
    if (element.attribute("midi.pan")) {
        this->SetMidiPan(StrToMidivaluePan(element.attribute("midi.pan").value()));
        element.remove_attribute("midi.pan");
        hasAttribute = true;
    }
    if (element.attribute("midi.patchname")) {
        this->SetMidiPatchname(StrToStr(element.attribute("midi.patchname").value()));
        element.remove_attribute("midi.patchname");
        hasAttribute = true;
    }
    if (element.attribute("midi.patchnum")) {
        this->SetMidiPatchnum(StrToMidivalue(element.attribute("midi.patchnum").value()));
        element.remove_attribute("midi.patchnum");
        hasAttribute = true;
    }
    if (element.attribute("midi.volume")) {
        this->SetMidiVolume(StrToPercent(element.attribute("midi.volume").value()));
        element.remove_attribute("midi.volume");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidiInstrument::WriteMidiInstrument(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMidiInstrnum()) {
        element.append_attribute("midi.instrnum") = MidivalueToStr(this->GetMidiInstrnum()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiInstrname()) {
        element.append_attribute("midi.instrname") = MidinamesToStr(this->GetMidiInstrname()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiPan()) {
        element.append_attribute("midi.pan") = MidivaluePanToStr(this->GetMidiPan()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiPatchname()) {
        element.append_attribute("midi.patchname") = StrToStr(this->GetMidiPatchname()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiPatchnum()) {
        element.append_attribute("midi.patchnum") = MidivalueToStr(this->GetMidiPatchnum()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiVolume()) {
        element.append_attribute("midi.volume") = PercentToStr(this->GetMidiVolume()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidiInstrument::HasMidiInstrnum() const
{
    return (m_midiInstrnum != -1);
}

bool AttMidiInstrument::HasMidiInstrname() const
{
    return (m_midiInstrname != MIDINAMES_NONE);
}

bool AttMidiInstrument::HasMidiPan() const
{
    return (m_midiPan != data_MIDIVALUE_PAN());
}

bool AttMidiInstrument::HasMidiPatchname() const
{
    return (m_midiPatchname != "");
}

bool AttMidiInstrument::HasMidiPatchnum() const
{
    return (m_midiPatchnum != -1);
}

bool AttMidiInstrument::HasMidiVolume() const
{
    return (m_midiVolume != -1.0);
}

/* include <attmidi.volume> */

//----------------------------------------------------------------------------
// AttMidiNumber
//----------------------------------------------------------------------------

AttMidiNumber::AttMidiNumber() : Att()
{
    ResetMidiNumber();
}

AttMidiNumber::~AttMidiNumber()
{
}

void AttMidiNumber::ResetMidiNumber()
{
    m_num = 0;
}

bool AttMidiNumber::ReadMidiNumber(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNum(StrToInt(element.attribute("num").value()));
        element.remove_attribute("num");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidiNumber::WriteMidiNumber(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasNum()) {
        element.append_attribute("num") = IntToStr(this->GetNum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidiNumber::HasNum() const
{
    return (m_num != 0);
}

/* include <attnum> */

//----------------------------------------------------------------------------
// AttMidiTempo
//----------------------------------------------------------------------------

AttMidiTempo::AttMidiTempo() : Att()
{
    ResetMidiTempo();
}

AttMidiTempo::~AttMidiTempo()
{
}

void AttMidiTempo::ResetMidiTempo()
{
    m_midiBpm = -1;
    m_midiMspb = -1;
}

bool AttMidiTempo::ReadMidiTempo(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("midi.bpm")) {
        this->SetMidiBpm(StrToMidibpm(element.attribute("midi.bpm").value()));
        element.remove_attribute("midi.bpm");
        hasAttribute = true;
    }
    if (element.attribute("midi.mspb")) {
        this->SetMidiMspb(StrToMidimspb(element.attribute("midi.mspb").value()));
        element.remove_attribute("midi.mspb");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidiTempo::WriteMidiTempo(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMidiBpm()) {
        element.append_attribute("midi.bpm") = MidibpmToStr(this->GetMidiBpm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiMspb()) {
        element.append_attribute("midi.mspb") = MidimspbToStr(this->GetMidiMspb()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidiTempo::HasMidiBpm() const
{
    return (m_midiBpm != -1);
}

bool AttMidiTempo::HasMidiMspb() const
{
    return (m_midiMspb != -1);
}

/* include <attmidi.mspb> */

//----------------------------------------------------------------------------
// AttMidiValue
//----------------------------------------------------------------------------

AttMidiValue::AttMidiValue() : Att()
{
    ResetMidiValue();
}

AttMidiValue::~AttMidiValue()
{
}

void AttMidiValue::ResetMidiValue()
{
    m_val = -1;
}

bool AttMidiValue::ReadMidiValue(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("val")) {
        this->SetVal(StrToMidivalue(element.attribute("val").value()));
        element.remove_attribute("val");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidiValue::WriteMidiValue(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVal()) {
        element.append_attribute("val") = MidivalueToStr(this->GetVal()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidiValue::HasVal() const
{
    return (m_val != -1);
}

/* include <attval> */

//----------------------------------------------------------------------------
// AttMidiValue2
//----------------------------------------------------------------------------

AttMidiValue2::AttMidiValue2() : Att()
{
    ResetMidiValue2();
}

AttMidiValue2::~AttMidiValue2()
{
}

void AttMidiValue2::ResetMidiValue2()
{
    m_val2 = -1;
}

bool AttMidiValue2::ReadMidiValue2(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("val2")) {
        this->SetVal2(StrToMidivalue(element.attribute("val2").value()));
        element.remove_attribute("val2");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidiValue2::WriteMidiValue2(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVal2()) {
        element.append_attribute("val2") = MidivalueToStr(this->GetVal2()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidiValue2::HasVal2() const
{
    return (m_val2 != -1);
}

/* include <attval2> */

//----------------------------------------------------------------------------
// AttMidiVelocity
//----------------------------------------------------------------------------

AttMidiVelocity::AttMidiVelocity() : Att()
{
    ResetMidiVelocity();
}

AttMidiVelocity::~AttMidiVelocity()
{
}

void AttMidiVelocity::ResetMidiVelocity()
{
    m_vel = -1;
}

bool AttMidiVelocity::ReadMidiVelocity(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("vel")) {
        this->SetVel(StrToMidivalue(element.attribute("vel").value()));
        element.remove_attribute("vel");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidiVelocity::WriteMidiVelocity(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVel()) {
        element.append_attribute("vel") = MidivalueToStr(this->GetVel()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidiVelocity::HasVel() const
{
    return (m_vel != -1);
}

/* include <attvel> */

//----------------------------------------------------------------------------
// AttTimeBase
//----------------------------------------------------------------------------

AttTimeBase::AttTimeBase() : Att()
{
    ResetTimeBase();
}

AttTimeBase::~AttTimeBase()
{
}

void AttTimeBase::ResetTimeBase()
{
    m_ppq = 0;
}

bool AttTimeBase::ReadTimeBase(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("ppq")) {
        this->SetPpq(StrToInt(element.attribute("ppq").value()));
        element.remove_attribute("ppq");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimeBase::WriteTimeBase(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPpq()) {
        element.append_attribute("ppq") = IntToStr(this->GetPpq()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimeBase::HasPpq() const
{
    return (m_ppq != 0);
}

/* include <attppq> */

bool Att::SetMidi(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_CHANNELIZED)) {
        AttChannelized *att = dynamic_cast<AttChannelized *>(element);
        assert(att);
        if (attrType == "midi.channel") {
            att->SetMidiChannel(att->StrToMidichannel(attrValue));
            return true;
        }
        if (attrType == "midi.duty") {
            att->SetMidiDuty(att->StrToPercentLimited(attrValue));
            return true;
        }
        if (attrType == "midi.port") {
            att->SetMidiPort(att->StrToMidivalueName(attrValue));
            return true;
        }
        if (attrType == "midi.track") {
            att->SetMidiTrack(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_INSTRUMENTIDENT)) {
        AttInstrumentIdent *att = dynamic_cast<AttInstrumentIdent *>(element);
        assert(att);
        if (attrType == "instr") {
            att->SetInstr(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDIINSTRUMENT)) {
        AttMidiInstrument *att = dynamic_cast<AttMidiInstrument *>(element);
        assert(att);
        if (attrType == "midi.instrnum") {
            att->SetMidiInstrnum(att->StrToMidivalue(attrValue));
            return true;
        }
        if (attrType == "midi.instrname") {
            att->SetMidiInstrname(att->StrToMidinames(attrValue));
            return true;
        }
        if (attrType == "midi.pan") {
            att->SetMidiPan(att->StrToMidivaluePan(attrValue));
            return true;
        }
        if (attrType == "midi.patchname") {
            att->SetMidiPatchname(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "midi.patchnum") {
            att->SetMidiPatchnum(att->StrToMidivalue(attrValue));
            return true;
        }
        if (attrType == "midi.volume") {
            att->SetMidiVolume(att->StrToPercent(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDINUMBER)) {
        AttMidiNumber *att = dynamic_cast<AttMidiNumber *>(element);
        assert(att);
        if (attrType == "num") {
            att->SetNum(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDITEMPO)) {
        AttMidiTempo *att = dynamic_cast<AttMidiTempo *>(element);
        assert(att);
        if (attrType == "midi.bpm") {
            att->SetMidiBpm(att->StrToMidibpm(attrValue));
            return true;
        }
        if (attrType == "midi.mspb") {
            att->SetMidiMspb(att->StrToMidimspb(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE)) {
        AttMidiValue *att = dynamic_cast<AttMidiValue *>(element);
        assert(att);
        if (attrType == "val") {
            att->SetVal(att->StrToMidivalue(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE2)) {
        AttMidiValue2 *att = dynamic_cast<AttMidiValue2 *>(element);
        assert(att);
        if (attrType == "val2") {
            att->SetVal2(att->StrToMidivalue(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDIVELOCITY)) {
        AttMidiVelocity *att = dynamic_cast<AttMidiVelocity *>(element);
        assert(att);
        if (attrType == "vel") {
            att->SetVel(att->StrToMidivalue(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIMEBASE)) {
        AttTimeBase *att = dynamic_cast<AttTimeBase *>(element);
        assert(att);
        if (attrType == "ppq") {
            att->SetPpq(att->StrToInt(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetMidi(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_CHANNELIZED)) {
        const AttChannelized *att = dynamic_cast<const AttChannelized *>(element);
        assert(att);
        if (att->HasMidiChannel()) {
            attributes->push_back(std::make_pair("midi.channel", att->MidichannelToStr(att->GetMidiChannel())));
        }
        if (att->HasMidiDuty()) {
            attributes->push_back(std::make_pair("midi.duty", att->PercentLimitedToStr(att->GetMidiDuty())));
        }
        if (att->HasMidiPort()) {
            attributes->push_back(std::make_pair("midi.port", att->MidivalueNameToStr(att->GetMidiPort())));
        }
        if (att->HasMidiTrack()) {
            attributes->push_back(std::make_pair("midi.track", att->IntToStr(att->GetMidiTrack())));
        }
    }
    if (element->HasAttClass(ATT_INSTRUMENTIDENT)) {
        const AttInstrumentIdent *att = dynamic_cast<const AttInstrumentIdent *>(element);
        assert(att);
        if (att->HasInstr()) {
            attributes->push_back(std::make_pair("instr", att->StrToStr(att->GetInstr())));
        }
    }
    if (element->HasAttClass(ATT_MIDIINSTRUMENT)) {
        const AttMidiInstrument *att = dynamic_cast<const AttMidiInstrument *>(element);
        assert(att);
        if (att->HasMidiInstrnum()) {
            attributes->push_back(std::make_pair("midi.instrnum", att->MidivalueToStr(att->GetMidiInstrnum())));
        }
        if (att->HasMidiInstrname()) {
            attributes->push_back(std::make_pair("midi.instrname", att->MidinamesToStr(att->GetMidiInstrname())));
        }
        if (att->HasMidiPan()) {
            attributes->push_back(std::make_pair("midi.pan", att->MidivaluePanToStr(att->GetMidiPan())));
        }
        if (att->HasMidiPatchname()) {
            attributes->push_back(std::make_pair("midi.patchname", att->StrToStr(att->GetMidiPatchname())));
        }
        if (att->HasMidiPatchnum()) {
            attributes->push_back(std::make_pair("midi.patchnum", att->MidivalueToStr(att->GetMidiPatchnum())));
        }
        if (att->HasMidiVolume()) {
            attributes->push_back(std::make_pair("midi.volume", att->PercentToStr(att->GetMidiVolume())));
        }
    }
    if (element->HasAttClass(ATT_MIDINUMBER)) {
        const AttMidiNumber *att = dynamic_cast<const AttMidiNumber *>(element);
        assert(att);
        if (att->HasNum()) {
            attributes->push_back(std::make_pair("num", att->IntToStr(att->GetNum())));
        }
    }
    if (element->HasAttClass(ATT_MIDITEMPO)) {
        const AttMidiTempo *att = dynamic_cast<const AttMidiTempo *>(element);
        assert(att);
        if (att->HasMidiBpm()) {
            attributes->push_back(std::make_pair("midi.bpm", att->MidibpmToStr(att->GetMidiBpm())));
        }
        if (att->HasMidiMspb()) {
            attributes->push_back(std::make_pair("midi.mspb", att->MidimspbToStr(att->GetMidiMspb())));
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE)) {
        const AttMidiValue *att = dynamic_cast<const AttMidiValue *>(element);
        assert(att);
        if (att->HasVal()) {
            attributes->push_back(std::make_pair("val", att->MidivalueToStr(att->GetVal())));
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE2)) {
        const AttMidiValue2 *att = dynamic_cast<const AttMidiValue2 *>(element);
        assert(att);
        if (att->HasVal2()) {
            attributes->push_back(std::make_pair("val2", att->MidivalueToStr(att->GetVal2())));
        }
    }
    if (element->HasAttClass(ATT_MIDIVELOCITY)) {
        const AttMidiVelocity *att = dynamic_cast<const AttMidiVelocity *>(element);
        assert(att);
        if (att->HasVel()) {
            attributes->push_back(std::make_pair("vel", att->MidivalueToStr(att->GetVel())));
        }
    }
    if (element->HasAttClass(ATT_TIMEBASE)) {
        const AttTimeBase *att = dynamic_cast<const AttTimeBase *>(element);
        assert(att);
        if (att->HasPpq()) {
            attributes->push_back(std::make_pair("ppq", att->IntToStr(att->GetPpq())));
        }
    }
}

} // vrv namespace
