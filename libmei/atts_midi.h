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

#ifndef __VRV_ATTS_MIDI_H__
#define __VRV_ATTS_MIDI_H__

#include "att.h"
#include "att_classes.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {
    
//----------------------------------------------------------------------------
// AttChannelized
//----------------------------------------------------------------------------

class AttChannelized: public Att
{
public:
    AttChannelized();
    virtual ~AttChannelized();
    
    /** Reset the default values for the attribute class **/
    void ResetChannelized();
    
    /** Read the values for the attribute class **/
    bool ReadChannelized( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteChannelized( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMidiChannel(std::string midiChannel_) { m_midiChannel = midiChannel_; };
    std::string GetMidiChannel() const { return m_midiChannel; };    
    bool HasMidiChannel( );
    
    //
    void SetMidiDuty(std::string midiDuty_) { m_midiDuty = midiDuty_; };
    std::string GetMidiDuty() const { return m_midiDuty; };    
    bool HasMidiDuty( );
    
    //
    void SetMidiPort(std::string midiPort_) { m_midiPort = midiPort_; };
    std::string GetMidiPort() const { return m_midiPort; };    
    bool HasMidiPort( );
    
    //
    void SetMidiTrack(int midiTrackInt_) { m_midiTrackInt = midiTrackInt_; };
    int GetMidiTrack() const { return m_midiTrackInt; };    
    bool HasMidiTrack( );
    
    ///@}

private:
    /** Records a MIDI channel value. **/
    std::string m_midiChannel;
    /** Specifies the 'on' part of the duty cycle as a percentage of a note's duration. **/
    std::string m_midiDuty;
    /** Sets the MIDI port value. **/
    std::string m_midiPort;
    /** Sets the MIDI track. **/
    int m_midiTrackInt;

/* include <attmidi.track> */
};

//----------------------------------------------------------------------------
// AttMidiinstrument
//----------------------------------------------------------------------------

class AttMidiinstrument: public Att
{
public:
    AttMidiinstrument();
    virtual ~AttMidiinstrument();
    
    /** Reset the default values for the attribute class **/
    void ResetMidiinstrument();
    
    /** Read the values for the attribute class **/
    bool ReadMidiinstrument( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMidiinstrument( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMidiInstrnum(std::string midiInstrnum_) { m_midiInstrnum = midiInstrnum_; };
    std::string GetMidiInstrnum() const { return m_midiInstrnum; };    
    bool HasMidiInstrnum( );
    
    //
    void SetMidiInstrname(std::string midiInstrname_) { m_midiInstrname = midiInstrname_; };
    std::string GetMidiInstrname() const { return m_midiInstrname; };    
    bool HasMidiInstrname( );
    
    //
    void SetMidiPan(std::string midiPan_) { m_midiPan = midiPan_; };
    std::string GetMidiPan() const { return m_midiPan; };    
    bool HasMidiPan( );
    
    //
    void SetMidiVolume(std::string midiVolume_) { m_midiVolume = midiVolume_; };
    std::string GetMidiVolume() const { return m_midiVolume; };    
    bool HasMidiVolume( );
    
    ///@}

private:
    /** Sets the MIDI instrument number. **/
    std::string m_midiInstrnum;
    /** Provides a General MIDI label for the MIDI instrument. **/
    std::string m_midiInstrname;
    /**
     * Sets the instrument's position in a stereo field.
     * Values of 0 and 1 both pan left, 127 pans right, and 64 pans to the center.
     **/
    std::string m_midiPan;
    /** Sets the instrument's volume. **/
    std::string m_midiVolume;

/* include <attmidi.volume> */
};

//----------------------------------------------------------------------------
// AttMidinumber
//----------------------------------------------------------------------------

class AttMidinumber: public Att
{
public:
    AttMidinumber();
    virtual ~AttMidinumber();
    
    /** Reset the default values for the attribute class **/
    void ResetMidinumber();
    
    /** Read the values for the attribute class **/
    bool ReadMidinumber( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMidinumber( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetNum(int numInt_) { m_numInt = numInt_; };
    int GetNum() const { return m_numInt; };    
    bool HasNum( );
    
    ///@}

private:
    /**
     * Along with numbase, describes duration as a ratio.
     * num is the first value in the ratio, while numbase is the second.
     **/
    int m_numInt;

/* include <attnum> */
};

//----------------------------------------------------------------------------
// AttMiditempo
//----------------------------------------------------------------------------

class AttMiditempo: public Att
{
public:
    AttMiditempo();
    virtual ~AttMiditempo();
    
    /** Reset the default values for the attribute class **/
    void ResetMiditempo();
    
    /** Read the values for the attribute class **/
    bool ReadMiditempo( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMiditempo( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetMidiTempo(std::string midiTempo_) { m_midiTempo = midiTempo_; };
    std::string GetMidiTempo() const { return m_midiTempo; };    
    bool HasMidiTempo( );
    
    ///@}

private:
    /**
     * Contains a MIDI value, that is, the number of quarter notes per minute in the
     * range from 10 to 1000.
     **/
    std::string m_midiTempo;

/* include <attmidi.tempo> */
};

//----------------------------------------------------------------------------
// AttMidivalue
//----------------------------------------------------------------------------

class AttMidivalue: public Att
{
public:
    AttMidivalue();
    virtual ~AttMidivalue();
    
    /** Reset the default values for the attribute class **/
    void ResetMidivalue();
    
    /** Read the values for the attribute class **/
    bool ReadMidivalue( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMidivalue( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetVal(std::string val_) { m_val = val_; };
    std::string GetVal() const { return m_val; };    
    bool HasVal( );
    
    ///@}

private:
    /** MIDI number. **/
    std::string m_val;

/* include <attval> */
};

//----------------------------------------------------------------------------
// AttMidivalue2
//----------------------------------------------------------------------------

class AttMidivalue2: public Att
{
public:
    AttMidivalue2();
    virtual ~AttMidivalue2();
    
    /** Reset the default values for the attribute class **/
    void ResetMidivalue2();
    
    /** Read the values for the attribute class **/
    bool ReadMidivalue2( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMidivalue2( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetVal2(std::string val2_) { m_val2 = val2_; };
    std::string GetVal2() const { return m_val2; };    
    bool HasVal2( );
    
    ///@}

private:
    /** MIDI number. **/
    std::string m_val2;

/* include <attval2> */
};

//----------------------------------------------------------------------------
// AttMidivelocity
//----------------------------------------------------------------------------

class AttMidivelocity: public Att
{
public:
    AttMidivelocity();
    virtual ~AttMidivelocity();
    
    /** Reset the default values for the attribute class **/
    void ResetMidivelocity();
    
    /** Read the values for the attribute class **/
    bool ReadMidivelocity( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteMidivelocity( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetVel(std::string vel_) { m_vel = vel_; };
    std::string GetVel() const { return m_vel; };    
    bool HasVel( );
    
    ///@}

private:
    /** MIDI Note-on/off velocity. **/
    std::string m_vel;

/* include <attvel> */
};

//----------------------------------------------------------------------------
// AttTimebase
//----------------------------------------------------------------------------

class AttTimebase: public Att
{
public:
    AttTimebase();
    virtual ~AttTimebase();
    
    /** Reset the default values for the attribute class **/
    void ResetTimebase();
    
    /** Read the values for the attribute class **/
    bool ReadTimebase( pugi::xml_node element );
    
    /** Write the values for the attribute class **/
    bool WriteTimebase( pugi::xml_node element );
    
    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal 
     * to the default value)
     **/
    ///@{
    void SetPpq(int ppqInt_) { m_ppqInt = ppqInt_; };
    int GetPpq() const { return m_ppqInt; };    
    bool HasPpq( );
    
    ///@}

private:
    /**
     * Indicates the number of pulses (sometimes referred to as ticks or divisions) per
     * quarter note.
     * Unlike MIDI, MEI permits different values for a score and individual staves.
     **/
    int m_ppqInt;

/* include <attppq> */
};

} // vrv namespace

#endif  // __VRV_ATTS_MIDI_H__

