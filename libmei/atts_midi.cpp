/////////////////////////////////////////////////////////////////////////////
// Name:        attributes.h
// Author:      Laurent Pugin
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

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttChannelized
//----------------------------------------------------------------------------

AttChannelized::AttChannelized() {
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

/* include <attmidi.track> */

//----------------------------------------------------------------------------
// AttMidiinstrument
//----------------------------------------------------------------------------

AttMidiinstrument::AttMidiinstrument() {
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

/* include <attmidi.volume> */

//----------------------------------------------------------------------------
// AttMidinumber
//----------------------------------------------------------------------------

AttMidinumber::AttMidinumber() {
    ResetMidinumber();
}

AttMidinumber::~AttMidinumber() {

}

void AttMidinumber::ResetMidinumber() {
    m_numInt = 0;
}

/* include <attnum> */

//----------------------------------------------------------------------------
// AttMiditempo
//----------------------------------------------------------------------------

AttMiditempo::AttMiditempo() {
    ResetMiditempo();
}

AttMiditempo::~AttMiditempo() {

}

void AttMiditempo::ResetMiditempo() {
    m_midiTempo = "";
}

/* include <attmidi.tempo> */

//----------------------------------------------------------------------------
// AttMidivalue
//----------------------------------------------------------------------------

AttMidivalue::AttMidivalue() {
    ResetMidivalue();
}

AttMidivalue::~AttMidivalue() {

}

void AttMidivalue::ResetMidivalue() {
    m_val = "";
}

/* include <attval> */

//----------------------------------------------------------------------------
// AttTimebase
//----------------------------------------------------------------------------

AttTimebase::AttTimebase() {
    ResetTimebase();
}

AttTimebase::~AttTimebase() {

}

void AttTimebase::ResetTimebase() {
    m_ppqInt = 0;
}

/* include <attppq> */

} // vrv namespace

