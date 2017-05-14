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
    m_midiDuty = 0;
    m_midiPort = -1;
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
        this->SetMidiDuty(StrToPercent(element.attribute("midi.duty").value()));
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
        element.append_attribute("midi.duty") = PercentToStr(this->GetMidiDuty()).c_str();
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
    return (m_midiChannel != -1);
}

bool AttChannelized::HasMidiDuty() const
{
    return (m_midiDuty != 0);
}

bool AttChannelized::HasMidiPort() const
{
    return (m_midiPort != -1);
}

bool AttChannelized::HasMidiTrack() const
{
    return (m_midiTrack != 0);
}

/* include <attmidi.track> */

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
    m_midiPan = -1;
    m_midiPatchname = "";
    m_midiPatchnum = -1;
    m_midiVolume = -1;
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
        this->SetMidiPan(StrToMidivalue(element.attribute("midi.pan").value()));
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
        this->SetMidiVolume(StrToMidivalue(element.attribute("midi.volume").value()));
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
        element.append_attribute("midi.pan") = MidivalueToStr(this->GetMidiPan()).c_str();
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
        element.append_attribute("midi.volume") = MidivalueToStr(this->GetMidiVolume()).c_str();
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
    return (m_midiPan != -1);
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
    return (m_midiVolume != -1);
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
            att->SetMidiDuty(att->StrToPercent(attrValue));
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
            att->SetMidiPan(att->StrToMidivalue(attrValue));
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
            att->SetMidiVolume(att->StrToMidivalue(attrValue));
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
    if (element->HasAttClass(ATT_MIDIVALUE)) {
        AttMidiValue *att = dynamic_cast<AttMidiValue *>(element);
        assert(att);
        if (attrType == "val") {
            att->SetVal(att->StrToMidivalue(attrValue));
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
            attributes->push_back(std::make_pair("midi.duty", att->PercentToStr(att->GetMidiDuty())));
        }
        if (att->HasMidiPort()) {
            attributes->push_back(std::make_pair("midi.port", att->MidivalueToStr(att->GetMidiPort())));
        }
        if (att->HasMidiTrack()) {
            attributes->push_back(std::make_pair("midi.track", att->IntToStr(att->GetMidiTrack())));
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
            attributes->push_back(std::make_pair("midi.pan", att->MidivalueToStr(att->GetMidiPan())));
        }
        if (att->HasMidiPatchname()) {
            attributes->push_back(std::make_pair("midi.patchname", att->StrToStr(att->GetMidiPatchname())));
        }
        if (att->HasMidiPatchnum()) {
            attributes->push_back(std::make_pair("midi.patchnum", att->MidivalueToStr(att->GetMidiPatchnum())));
        }
        if (att->HasMidiVolume()) {
            attributes->push_back(std::make_pair("midi.volume", att->MidivalueToStr(att->GetMidiVolume())));
        }
    }
    if (element->HasAttClass(ATT_MIDINUMBER)) {
        const AttMidiNumber *att = dynamic_cast<const AttMidiNumber *>(element);
        assert(att);
        if (att->HasNum()) {
            attributes->push_back(std::make_pair("num", att->IntToStr(att->GetNum())));
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE)) {
        const AttMidiValue *att = dynamic_cast<const AttMidiValue *>(element);
        assert(att);
        if (att->HasVal()) {
            attributes->push_back(std::make_pair("val", att->MidivalueToStr(att->GetVal())));
        }
    }
}

} // vrv namespace
