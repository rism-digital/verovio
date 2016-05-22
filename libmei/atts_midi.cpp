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
    m_midiChannel = MIDICHANNEL_NONE;
    m_midiDuty = 0;
    m_midiPort = MIDIVALUE_NONE;
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
        this->SetMidiDuty(StrToInt(element.attribute("midi.duty").value()));
        element.remove_attribute("midi.duty");
        hasAttribute = true;
    }
    if (element.attribute("midi.port")) {
        this->SetMidiPort(StrToMidivalue(element.attribute("midi.port").value()));
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
        element.append_attribute("midi.duty") = IntToStr(this->GetMidiDuty()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiPort()) {
        element.append_attribute("midi.port") = MidivalueToStr(this->GetMidiPort()).c_str();
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
    return (m_midiChannel != MIDICHANNEL_NONE);
}

bool AttChannelized::HasMidiDuty() const
{
    return (m_midiDuty != 0);
}

bool AttChannelized::HasMidiPort() const
{
    return (m_midiPort != MIDIVALUE_NONE);
}

bool AttChannelized::HasMidiTrack() const
{
    return (m_midiTrack != 0);
}

/* include <attmidi.track> */

//----------------------------------------------------------------------------
// AttMidiinstrument
//----------------------------------------------------------------------------

AttMidiinstrument::AttMidiinstrument() : Att()
{
    ResetMidiinstrument();
}

AttMidiinstrument::~AttMidiinstrument()
{
}

void AttMidiinstrument::ResetMidiinstrument()
{
    m_midiInstrnum = MIDIVALUE_NONE;
    m_midiInstrname = MIDINAMES_NONE;
    m_midiPan = MIDIVALUE_NONE;
    m_midiVolume = MIDIVALUE_NONE;
}

bool AttMidiinstrument::ReadMidiinstrument(pugi::xml_node element)
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
        this->SetMidiPan(StrToMidivalue(element.attribute("midi.pan").value()));
        element.remove_attribute("midi.pan");
        hasAttribute = true;
    }
    if (element.attribute("midi.volume")) {
        this->SetMidiVolume(StrToMidivalue(element.attribute("midi.volume").value()));
        element.remove_attribute("midi.volume");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidiinstrument::WriteMidiinstrument(pugi::xml_node element)
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
        element.append_attribute("midi.pan") = MidivalueToStr(this->GetMidiPan()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiVolume()) {
        element.append_attribute("midi.volume") = MidivalueToStr(this->GetMidiVolume()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidiinstrument::HasMidiInstrnum() const
{
    return (m_midiInstrnum != MIDIVALUE_NONE);
}

bool AttMidiinstrument::HasMidiInstrname() const
{
    return (m_midiInstrname != MIDINAMES_NONE);
}

bool AttMidiinstrument::HasMidiPan() const
{
    return (m_midiPan != MIDIVALUE_NONE);
}

bool AttMidiinstrument::HasMidiVolume() const
{
    return (m_midiVolume != MIDIVALUE_NONE);
}

/* include <attmidi.volume> */

//----------------------------------------------------------------------------
// AttMidinumber
//----------------------------------------------------------------------------

AttMidinumber::AttMidinumber() : Att()
{
    ResetMidinumber();
}

AttMidinumber::~AttMidinumber()
{
}

void AttMidinumber::ResetMidinumber()
{
    m_num = 0;
}

bool AttMidinumber::ReadMidinumber(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNum(StrToInt(element.attribute("num").value()));
        element.remove_attribute("num");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidinumber::WriteMidinumber(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasNum()) {
        element.append_attribute("num") = IntToStr(this->GetNum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidinumber::HasNum() const
{
    return (m_num != 0);
}

/* include <attnum> */

//----------------------------------------------------------------------------
// AttMiditempo
//----------------------------------------------------------------------------

AttMiditempo::AttMiditempo() : Att()
{
    ResetMiditempo();
}

AttMiditempo::~AttMiditempo()
{
}

void AttMiditempo::ResetMiditempo()
{
    m_midiBpm = MIDIBPM_NONE;
    m_midiMspb = MIDIMSPB_NONE;
}

bool AttMiditempo::ReadMiditempo(pugi::xml_node element)
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

bool AttMiditempo::WriteMiditempo(pugi::xml_node element)
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

bool AttMiditempo::HasMidiBpm() const
{
    return (m_midiBpm != MIDIBPM_NONE);
}

bool AttMiditempo::HasMidiMspb() const
{
    return (m_midiMspb != MIDIMSPB_NONE);
}

/* include <attmidi.mspb> */

//----------------------------------------------------------------------------
// AttMidivalue
//----------------------------------------------------------------------------

AttMidivalue::AttMidivalue() : Att()
{
    ResetMidivalue();
}

AttMidivalue::~AttMidivalue()
{
}

void AttMidivalue::ResetMidivalue()
{
    m_val = MIDIVALUE_NONE;
}

bool AttMidivalue::ReadMidivalue(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("val")) {
        this->SetVal(StrToMidivalue(element.attribute("val").value()));
        element.remove_attribute("val");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidivalue::WriteMidivalue(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVal()) {
        element.append_attribute("val") = MidivalueToStr(this->GetVal()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidivalue::HasVal() const
{
    return (m_val != MIDIVALUE_NONE);
}

/* include <attval> */

//----------------------------------------------------------------------------
// AttMidivalue2
//----------------------------------------------------------------------------

AttMidivalue2::AttMidivalue2() : Att()
{
    ResetMidivalue2();
}

AttMidivalue2::~AttMidivalue2()
{
}

void AttMidivalue2::ResetMidivalue2()
{
    m_val2 = MIDIVALUE_NONE;
}

bool AttMidivalue2::ReadMidivalue2(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("val2")) {
        this->SetVal2(StrToMidivalue(element.attribute("val2").value()));
        element.remove_attribute("val2");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidivalue2::WriteMidivalue2(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVal2()) {
        element.append_attribute("val2") = MidivalueToStr(this->GetVal2()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidivalue2::HasVal2() const
{
    return (m_val2 != MIDIVALUE_NONE);
}

/* include <attval2> */

//----------------------------------------------------------------------------
// AttMidivelocity
//----------------------------------------------------------------------------

AttMidivelocity::AttMidivelocity() : Att()
{
    ResetMidivelocity();
}

AttMidivelocity::~AttMidivelocity()
{
}

void AttMidivelocity::ResetMidivelocity()
{
    m_vel = MIDIVALUE_NONE;
}

bool AttMidivelocity::ReadMidivelocity(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("vel")) {
        this->SetVel(StrToMidivalue(element.attribute("vel").value()));
        element.remove_attribute("vel");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidivelocity::WriteMidivelocity(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVel()) {
        element.append_attribute("vel") = MidivalueToStr(this->GetVel()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidivelocity::HasVel() const
{
    return (m_vel != MIDIVALUE_NONE);
}

/* include <attvel> */

//----------------------------------------------------------------------------
// AttTimebase
//----------------------------------------------------------------------------

AttTimebase::AttTimebase() : Att()
{
    ResetTimebase();
}

AttTimebase::~AttTimebase()
{
}

void AttTimebase::ResetTimebase()
{
    m_ppq = 0;
}

bool AttTimebase::ReadTimebase(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("ppq")) {
        this->SetPpq(StrToInt(element.attribute("ppq").value()));
        element.remove_attribute("ppq");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimebase::WriteTimebase(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPpq()) {
        element.append_attribute("ppq") = IntToStr(this->GetPpq()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimebase::HasPpq() const
{
    return (m_ppq != 0);
}

/* include <attppq> */

bool Att::SetMidi(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_CHANNELIZED)) {
        AttChannelized *att = dynamic_cast<AttChannelized *>(element);
        assert(att);
        if (attrType == "midi.channel") {
            att->SetMidiChannel(att->StrToMidichannel(attrValue));
            return true;
        }
        if (attrType == "midi.duty") {
            att->SetMidiDuty(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "midi.port") {
            att->SetMidiPort(att->StrToMidivalue(attrValue));
            return true;
        }
        if (attrType == "midi.track") {
            att->SetMidiTrack(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDIINSTRUMENT)) {
        AttMidiinstrument *att = dynamic_cast<AttMidiinstrument *>(element);
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
            att->SetMidiPan(att->StrToMidivalue(attrValue));
            return true;
        }
        if (attrType == "midi.volume") {
            att->SetMidiVolume(att->StrToMidivalue(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDINUMBER)) {
        AttMidinumber *att = dynamic_cast<AttMidinumber *>(element);
        assert(att);
        if (attrType == "num") {
            att->SetNum(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDITEMPO)) {
        AttMiditempo *att = dynamic_cast<AttMiditempo *>(element);
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
        AttMidivalue *att = dynamic_cast<AttMidivalue *>(element);
        assert(att);
        if (attrType == "val") {
            att->SetVal(att->StrToMidivalue(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE2)) {
        AttMidivalue2 *att = dynamic_cast<AttMidivalue2 *>(element);
        assert(att);
        if (attrType == "val2") {
            att->SetVal2(att->StrToMidivalue(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDIVELOCITY)) {
        AttMidivelocity *att = dynamic_cast<AttMidivelocity *>(element);
        assert(att);
        if (attrType == "vel") {
            att->SetVel(att->StrToMidivalue(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIMEBASE)) {
        AttTimebase *att = dynamic_cast<AttTimebase *>(element);
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
            attributes->push_back(std::make_pair("midi.duty", att->IntToStr(att->GetMidiDuty())));
        }
        if (att->HasMidiPort()) {
            attributes->push_back(std::make_pair("midi.port", att->MidivalueToStr(att->GetMidiPort())));
        }
        if (att->HasMidiTrack()) {
            attributes->push_back(std::make_pair("midi.track", att->IntToStr(att->GetMidiTrack())));
        }
    }
    if (element->HasAttClass(ATT_MIDIINSTRUMENT)) {
        const AttMidiinstrument *att = dynamic_cast<const AttMidiinstrument *>(element);
        assert(att);
        if (att->HasMidiInstrnum()) {
            attributes->push_back(std::make_pair("midi.instrnum", att->MidivalueToStr(att->GetMidiInstrnum())));
        }
        if (att->HasMidiInstrname()) {
            attributes->push_back(std::make_pair("midi.instrname", att->MidinamesToStr(att->GetMidiInstrname())));
        }
        if (att->HasMidiPan()) {
            attributes->push_back(std::make_pair("midi.pan", att->MidivalueToStr(att->GetMidiPan())));
        }
        if (att->HasMidiVolume()) {
            attributes->push_back(std::make_pair("midi.volume", att->MidivalueToStr(att->GetMidiVolume())));
        }
    }
    if (element->HasAttClass(ATT_MIDINUMBER)) {
        const AttMidinumber *att = dynamic_cast<const AttMidinumber *>(element);
        assert(att);
        if (att->HasNum()) {
            attributes->push_back(std::make_pair("num", att->IntToStr(att->GetNum())));
        }
    }
    if (element->HasAttClass(ATT_MIDITEMPO)) {
        const AttMiditempo *att = dynamic_cast<const AttMiditempo *>(element);
        assert(att);
        if (att->HasMidiBpm()) {
            attributes->push_back(std::make_pair("midi.bpm", att->MidibpmToStr(att->GetMidiBpm())));
        }
        if (att->HasMidiMspb()) {
            attributes->push_back(std::make_pair("midi.mspb", att->MidimspbToStr(att->GetMidiMspb())));
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE)) {
        const AttMidivalue *att = dynamic_cast<const AttMidivalue *>(element);
        assert(att);
        if (att->HasVal()) {
            attributes->push_back(std::make_pair("val", att->MidivalueToStr(att->GetVal())));
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE2)) {
        const AttMidivalue2 *att = dynamic_cast<const AttMidivalue2 *>(element);
        assert(att);
        if (att->HasVal2()) {
            attributes->push_back(std::make_pair("val2", att->MidivalueToStr(att->GetVal2())));
        }
    }
    if (element->HasAttClass(ATT_MIDIVELOCITY)) {
        const AttMidivelocity *att = dynamic_cast<const AttMidivelocity *>(element);
        assert(att);
        if (att->HasVel()) {
            attributes->push_back(std::make_pair("vel", att->MidivalueToStr(att->GetVel())));
        }
    }
    if (element->HasAttClass(ATT_TIMEBASE)) {
        const AttTimebase *att = dynamic_cast<const AttTimebase *>(element);
        assert(att);
        if (att->HasPpq()) {
            attributes->push_back(std::make_pair("ppq", att->IntToStr(att->GetPpq())));
        }
    }
}

} // vrv namespace
