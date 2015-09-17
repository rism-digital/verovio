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

AttChannelized::AttChannelized(): Att() {
    ResetChannelized();
}

AttChannelized::~AttChannelized() {

}

void AttChannelized::ResetChannelized() {
    m_midiChannel = "";
    m_midiDuty = "";
    m_midiPort = "";
    m_midiTrackInt = 0;
}

bool AttChannelized::ReadChannelized(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("midi.channel")) {
        this->SetMidiChannel(StrToStr(element.attribute("midi.channel").value()));
        element.remove_attribute("midi.channel");
        hasAttribute = true;
    }
    if (element.attribute("midi.duty")) {
        this->SetMidiDuty(StrToStr(element.attribute("midi.duty").value()));
        element.remove_attribute("midi.duty");
        hasAttribute = true;
    }
    if (element.attribute("midi.port")) {
        this->SetMidiPort(StrToStr(element.attribute("midi.port").value()));
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

bool AttChannelized::WriteChannelized(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMidiChannel()) {
        element.append_attribute("midi.channel") = StrToStr(this->GetMidiChannel()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiDuty()) {
        element.append_attribute("midi.duty") = StrToStr(this->GetMidiDuty()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiPort()) {
        element.append_attribute("midi.port") = StrToStr(this->GetMidiPort()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiTrack()) {
        element.append_attribute("midi.track") = IntToStr(this->GetMidiTrack()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttChannelized::HasMidiChannel( )
{
    return (m_midiChannel != "");
}

bool AttChannelized::HasMidiDuty( )
{
    return (m_midiDuty != "");
}

bool AttChannelized::HasMidiPort( )
{
    return (m_midiPort != "");
}

bool AttChannelized::HasMidiTrack( )
{
    return (m_midiTrackInt != 0);
}


/* include <attmidi.track> */

//----------------------------------------------------------------------------
// AttMidiinstrument
//----------------------------------------------------------------------------

AttMidiinstrument::AttMidiinstrument(): Att() {
    ResetMidiinstrument();
}

AttMidiinstrument::~AttMidiinstrument() {

}

void AttMidiinstrument::ResetMidiinstrument() {
    m_midiInstrnum = "";
    m_midiInstrname = "";
    m_midiPan = "";
    m_midiVolume = "";
}

bool AttMidiinstrument::ReadMidiinstrument(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("midi.instrnum")) {
        this->SetMidiInstrnum(StrToStr(element.attribute("midi.instrnum").value()));
        element.remove_attribute("midi.instrnum");
        hasAttribute = true;
    }
    if (element.attribute("midi.instrname")) {
        this->SetMidiInstrname(StrToStr(element.attribute("midi.instrname").value()));
        element.remove_attribute("midi.instrname");
        hasAttribute = true;
    }
    if (element.attribute("midi.pan")) {
        this->SetMidiPan(StrToStr(element.attribute("midi.pan").value()));
        element.remove_attribute("midi.pan");
        hasAttribute = true;
    }
    if (element.attribute("midi.volume")) {
        this->SetMidiVolume(StrToStr(element.attribute("midi.volume").value()));
        element.remove_attribute("midi.volume");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidiinstrument::WriteMidiinstrument(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMidiInstrnum()) {
        element.append_attribute("midi.instrnum") = StrToStr(this->GetMidiInstrnum()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiInstrname()) {
        element.append_attribute("midi.instrname") = StrToStr(this->GetMidiInstrname()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiPan()) {
        element.append_attribute("midi.pan") = StrToStr(this->GetMidiPan()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMidiVolume()) {
        element.append_attribute("midi.volume") = StrToStr(this->GetMidiVolume()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidiinstrument::HasMidiInstrnum( )
{
    return (m_midiInstrnum != "");
}

bool AttMidiinstrument::HasMidiInstrname( )
{
    return (m_midiInstrname != "");
}

bool AttMidiinstrument::HasMidiPan( )
{
    return (m_midiPan != "");
}

bool AttMidiinstrument::HasMidiVolume( )
{
    return (m_midiVolume != "");
}


/* include <attmidi.volume> */

//----------------------------------------------------------------------------
// AttMidinumber
//----------------------------------------------------------------------------

AttMidinumber::AttMidinumber(): Att() {
    ResetMidinumber();
}

AttMidinumber::~AttMidinumber() {

}

void AttMidinumber::ResetMidinumber() {
    m_numInt = 0;
}

bool AttMidinumber::ReadMidinumber(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNum(StrToInt(element.attribute("num").value()));
        element.remove_attribute("num");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidinumber::WriteMidinumber(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasNum()) {
        element.append_attribute("num") = IntToStr(this->GetNum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidinumber::HasNum( )
{
    return (m_numInt != 0);
}


/* include <attnum> */

//----------------------------------------------------------------------------
// AttMiditempo
//----------------------------------------------------------------------------

AttMiditempo::AttMiditempo(): Att() {
    ResetMiditempo();
}

AttMiditempo::~AttMiditempo() {

}

void AttMiditempo::ResetMiditempo() {
    m_midiTempo = "";
}

bool AttMiditempo::ReadMiditempo(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("midi.tempo")) {
        this->SetMidiTempo(StrToStr(element.attribute("midi.tempo").value()));
        element.remove_attribute("midi.tempo");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMiditempo::WriteMiditempo(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMidiTempo()) {
        element.append_attribute("midi.tempo") = StrToStr(this->GetMidiTempo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMiditempo::HasMidiTempo( )
{
    return (m_midiTempo != "");
}


/* include <attmidi.tempo> */

//----------------------------------------------------------------------------
// AttMidivalue
//----------------------------------------------------------------------------

AttMidivalue::AttMidivalue(): Att() {
    ResetMidivalue();
}

AttMidivalue::~AttMidivalue() {

}

void AttMidivalue::ResetMidivalue() {
    m_val = "";
}

bool AttMidivalue::ReadMidivalue(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("val")) {
        this->SetVal(StrToStr(element.attribute("val").value()));
        element.remove_attribute("val");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMidivalue::WriteMidivalue(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasVal()) {
        element.append_attribute("val") = StrToStr(this->GetVal()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMidivalue::HasVal( )
{
    return (m_val != "");
}


/* include <attval> */

//----------------------------------------------------------------------------
// AttTimebase
//----------------------------------------------------------------------------

AttTimebase::AttTimebase(): Att() {
    ResetTimebase();
}

AttTimebase::~AttTimebase() {

}

void AttTimebase::ResetTimebase() {
    m_ppqInt = 0;
}

bool AttTimebase::ReadTimebase(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("ppq")) {
        this->SetPpq(StrToInt(element.attribute("ppq").value()));
        element.remove_attribute("ppq");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimebase::WriteTimebase(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasPpq()) {
        element.append_attribute("ppq") = IntToStr(this->GetPpq()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimebase::HasPpq( )
{
    return (m_ppqInt != 0);
}


/* include <attppq> */

bool Att::SetMidi( Object *element, std::string attrType, std::string attrValue ) {
    if (element->HasAttClass( ATT_CHANNELIZED ) ) {
        AttChannelized *att = dynamic_cast<AttChannelized*>(element);
        assert( att );
        if (attrType == "midiChannel") {
            att->SetMidiChannel(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "midiDuty") {
            att->SetMidiDuty(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "midiPort") {
            att->SetMidiPort(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "midiTrackInt") {
            att->SetMidiTrack(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_MIDIINSTRUMENT ) ) {
        AttMidiinstrument *att = dynamic_cast<AttMidiinstrument*>(element);
        assert( att );
        if (attrType == "midiInstrnum") {
            att->SetMidiInstrnum(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "midiInstrname") {
            att->SetMidiInstrname(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "midiPan") {
            att->SetMidiPan(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "midiVolume") {
            att->SetMidiVolume(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_MIDINUMBER ) ) {
        AttMidinumber *att = dynamic_cast<AttMidinumber*>(element);
        assert( att );
        if (attrType == "numInt") {
            att->SetNum(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_MIDITEMPO ) ) {
        AttMiditempo *att = dynamic_cast<AttMiditempo*>(element);
        assert( att );
        if (attrType == "midiTempo") {
            att->SetMidiTempo(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_MIDIVALUE ) ) {
        AttMidivalue *att = dynamic_cast<AttMidivalue*>(element);
        assert( att );
        if (attrType == "val") {
            att->SetVal(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass( ATT_TIMEBASE ) ) {
        AttTimebase *att = dynamic_cast<AttTimebase*>(element);
        assert( att );
        if (attrType == "ppqInt") {
            att->SetPpq(att->StrToInt(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetMidi( Object *element, ArrayOfStrAttr *attributes ) {
    if (element->HasAttClass( ATT_CHANNELIZED ) ) {
        AttChannelized *att = dynamic_cast<AttChannelized*>(element);
        assert( att );
        if (att->HasMidiChannel()) {
            attributes->push_back(std::make_pair("midiChannel", att->StrToStr(att->GetMidiChannel())));
        }
        if (att->HasMidiDuty()) {
            attributes->push_back(std::make_pair("midiDuty", att->StrToStr(att->GetMidiDuty())));
        }
        if (att->HasMidiPort()) {
            attributes->push_back(std::make_pair("midiPort", att->StrToStr(att->GetMidiPort())));
        }
        if (att->HasMidiTrack()) {
            attributes->push_back(std::make_pair("midiTrackInt", att->IntToStr(att->GetMidiTrack())));
        }
    }
    if (element->HasAttClass( ATT_MIDIINSTRUMENT ) ) {
        AttMidiinstrument *att = dynamic_cast<AttMidiinstrument*>(element);
        assert( att );
        if (att->HasMidiInstrnum()) {
            attributes->push_back(std::make_pair("midiInstrnum", att->StrToStr(att->GetMidiInstrnum())));
        }
        if (att->HasMidiInstrname()) {
            attributes->push_back(std::make_pair("midiInstrname", att->StrToStr(att->GetMidiInstrname())));
        }
        if (att->HasMidiPan()) {
            attributes->push_back(std::make_pair("midiPan", att->StrToStr(att->GetMidiPan())));
        }
        if (att->HasMidiVolume()) {
            attributes->push_back(std::make_pair("midiVolume", att->StrToStr(att->GetMidiVolume())));
        }
    }
    if (element->HasAttClass( ATT_MIDINUMBER ) ) {
        AttMidinumber *att = dynamic_cast<AttMidinumber*>(element);
        assert( att );
        if (att->HasNum()) {
            attributes->push_back(std::make_pair("numInt", att->IntToStr(att->GetNum())));
        }
    }
    if (element->HasAttClass( ATT_MIDITEMPO ) ) {
        AttMiditempo *att = dynamic_cast<AttMiditempo*>(element);
        assert( att );
        if (att->HasMidiTempo()) {
            attributes->push_back(std::make_pair("midiTempo", att->StrToStr(att->GetMidiTempo())));
        }
    }
    if (element->HasAttClass( ATT_MIDIVALUE ) ) {
        AttMidivalue *att = dynamic_cast<AttMidivalue*>(element);
        assert( att );
        if (att->HasVal()) {
            attributes->push_back(std::make_pair("val", att->StrToStr(att->GetVal())));
        }
    }
    if (element->HasAttClass( ATT_TIMEBASE ) ) {
        AttTimebase *att = dynamic_cast<AttTimebase*>(element);
        assert( att );
        if (att->HasPpq()) {
            attributes->push_back(std::make_pair("ppqInt", att->IntToStr(att->GetPpq())));
        }
    }

}
    
} // vrv namespace
    
