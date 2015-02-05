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
        hasAttribute = true;
    }
    if (element.attribute("midi.duty")) {
        this->SetMidiDuty(StrToStr(element.attribute("midi.duty").value()));
        hasAttribute = true;
    }
    if (element.attribute("midi.port")) {
        this->SetMidiPort(StrToStr(element.attribute("midi.port").value()));
        hasAttribute = true;
    }
    if (element.attribute("midi.track")) {
        this->SetMidiTrack(StrToInt(element.attribute("midi.track").value()));
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
        hasAttribute = true;
    }
    if (element.attribute("midi.instrname")) {
        this->SetMidiInstrname(StrToStr(element.attribute("midi.instrname").value()));
        hasAttribute = true;
    }
    if (element.attribute("midi.pan")) {
        this->SetMidiPan(StrToStr(element.attribute("midi.pan").value()));
        hasAttribute = true;
    }
    if (element.attribute("midi.volume")) {
        this->SetMidiVolume(StrToStr(element.attribute("midi.volume").value()));
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

bool Att::SetMidi( Object *element, std::string attrType, std::string attrValue )
{
    if ( (attrType == "midiChannel") && dynamic_cast<AttChannelized*>(element) ) {
        AttChannelized *att = dynamic_cast<AttChannelized*>(element);
        att->SetMidiChannel(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "midiDuty") && dynamic_cast<AttChannelized*>(element) ) {
        AttChannelized *att = dynamic_cast<AttChannelized*>(element);
        att->SetMidiDuty(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "midiPort") && dynamic_cast<AttChannelized*>(element) ) {
        AttChannelized *att = dynamic_cast<AttChannelized*>(element);
        att->SetMidiPort(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "midiTrackInt") && dynamic_cast<AttChannelized*>(element) ) {
        AttChannelized *att = dynamic_cast<AttChannelized*>(element);
        att->SetMidiTrack(att->StrToInt(attrValue));
    return true;
    }
    if ( (attrType == "midiInstrnum") && dynamic_cast<AttMidiinstrument*>(element) ) {
        AttMidiinstrument *att = dynamic_cast<AttMidiinstrument*>(element);
        att->SetMidiInstrnum(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "midiInstrname") && dynamic_cast<AttMidiinstrument*>(element) ) {
        AttMidiinstrument *att = dynamic_cast<AttMidiinstrument*>(element);
        att->SetMidiInstrname(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "midiPan") && dynamic_cast<AttMidiinstrument*>(element) ) {
        AttMidiinstrument *att = dynamic_cast<AttMidiinstrument*>(element);
        att->SetMidiPan(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "midiVolume") && dynamic_cast<AttMidiinstrument*>(element) ) {
        AttMidiinstrument *att = dynamic_cast<AttMidiinstrument*>(element);
        att->SetMidiVolume(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "numInt") && dynamic_cast<AttMidinumber*>(element) ) {
        AttMidinumber *att = dynamic_cast<AttMidinumber*>(element);
        att->SetNum(att->StrToInt(attrValue));
    return true;
    }
    if ( (attrType == "midiTempo") && dynamic_cast<AttMiditempo*>(element) ) {
        AttMiditempo *att = dynamic_cast<AttMiditempo*>(element);
        att->SetMidiTempo(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "val") && dynamic_cast<AttMidivalue*>(element) ) {
        AttMidivalue *att = dynamic_cast<AttMidivalue*>(element);
        att->SetVal(att->StrToStr(attrValue));
    return true;
    }
    if ( (attrType == "ppqInt") && dynamic_cast<AttTimebase*>(element) ) {
        AttTimebase *att = dynamic_cast<AttTimebase*>(element);
        att->SetPpq(att->StrToInt(attrValue));
    return true;
    }

    return false;
}


} // vrv namespace
    