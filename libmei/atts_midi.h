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
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttChannelized
//----------------------------------------------------------------------------

class AttChannelized : public Att {
public:
    AttChannelized();
    virtual ~AttChannelized();

    /** Reset the default values for the attribute class **/
    void ResetChannelized();

    /** Read the values for the attribute class **/
    bool ReadChannelized(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteChannelized(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMidiChannel(data_MIDICHANNEL midiChannel_) { m_midiChannel = midiChannel_; }
    data_MIDICHANNEL GetMidiChannel() const { return m_midiChannel; }
    bool HasMidiChannel() const;
    //
    void SetMidiDuty(data_PERCENT midiDuty_) { m_midiDuty = midiDuty_; }
    data_PERCENT GetMidiDuty() const { return m_midiDuty; }
    bool HasMidiDuty() const;
    //
    void SetMidiPort(data_MIDIVALUE midiPort_) { m_midiPort = midiPort_; }
    data_MIDIVALUE GetMidiPort() const { return m_midiPort; }
    bool HasMidiPort() const;
    //
    void SetMidiTrack(int midiTrack_) { m_midiTrack = midiTrack_; }
    int GetMidiTrack() const { return m_midiTrack; }
    bool HasMidiTrack() const;
    ///@}

private:
    /** Records a MIDI channel value. **/
    data_MIDICHANNEL m_midiChannel;
    /** Specifies the 'on' part of the duty cycle as a percentage of a note's duration. **/
    data_PERCENT m_midiDuty;
    /** Sets the MIDI port value. **/
    data_MIDIVALUE m_midiPort;
    /** Sets the MIDI track. **/
    int m_midiTrack;

    /* include <attmidi.track> */
};

//----------------------------------------------------------------------------
// AttMidiinstrument
//----------------------------------------------------------------------------

class AttMidiinstrument : public Att {
public:
    AttMidiinstrument();
    virtual ~AttMidiinstrument();

    /** Reset the default values for the attribute class **/
    void ResetMidiinstrument();

    /** Read the values for the attribute class **/
    bool ReadMidiinstrument(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMidiinstrument(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMidiInstrnum(data_MIDIVALUE midiInstrnum_) { m_midiInstrnum = midiInstrnum_; }
    data_MIDIVALUE GetMidiInstrnum() const { return m_midiInstrnum; }
    bool HasMidiInstrnum() const;
    //
    void SetMidiInstrname(data_MIDINAMES midiInstrname_) { m_midiInstrname = midiInstrname_; }
    data_MIDINAMES GetMidiInstrname() const { return m_midiInstrname; }
    bool HasMidiInstrname() const;
    //
    void SetMidiPan(data_MIDIVALUE midiPan_) { m_midiPan = midiPan_; }
    data_MIDIVALUE GetMidiPan() const { return m_midiPan; }
    bool HasMidiPan() const;
    //
    void SetMidiVolume(data_MIDIVALUE midiVolume_) { m_midiVolume = midiVolume_; }
    data_MIDIVALUE GetMidiVolume() const { return m_midiVolume; }
    bool HasMidiVolume() const;
    ///@}

private:
    /** Sets the MIDI instrument number. **/
    data_MIDIVALUE m_midiInstrnum;
    /** Provides a General MIDI label for the MIDI instrument. **/
    data_MIDINAMES m_midiInstrname;
    /**
     * Sets the instrument's position in a stereo field.
     * Values of 0 and 1 both pan left, 127 pans right, and 64 pans to the center.
     **/
    data_MIDIVALUE m_midiPan;
    /** Sets the instrument's volume. **/
    data_MIDIVALUE m_midiVolume;

    /* include <attmidi.volume> */
};

//----------------------------------------------------------------------------
// AttMidinumber
//----------------------------------------------------------------------------

class AttMidinumber : public Att {
public:
    AttMidinumber();
    virtual ~AttMidinumber();

    /** Reset the default values for the attribute class **/
    void ResetMidinumber();

    /** Read the values for the attribute class **/
    bool ReadMidinumber(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMidinumber(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetNum(int num_) { m_num = num_; }
    int GetNum() const { return m_num; }
    bool HasNum() const;
    ///@}

private:
    /**
     * Along with numbase, describes duration as a ratio.
     * num is the first value in the ratio, while numbase is the second.
     **/
    int m_num;

    /* include <attnum> */
};

//----------------------------------------------------------------------------
// AttMiditempo
//----------------------------------------------------------------------------

class AttMiditempo : public Att {
public:
    AttMiditempo();
    virtual ~AttMiditempo();

    /** Reset the default values for the attribute class **/
    void ResetMiditempo();

    /** Read the values for the attribute class **/
    bool ReadMiditempo(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMiditempo(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMidiBpm(data_MIDIBPM midiBpm_) { m_midiBpm = midiBpm_; }
    data_MIDIBPM GetMidiBpm() const { return m_midiBpm; }
    bool HasMidiBpm() const;
    //
    void SetMidiMspb(data_MIDIMSPB midiMspb_) { m_midiMspb = midiMspb_; }
    data_MIDIMSPB GetMidiMspb() const { return m_midiMspb; }
    bool HasMidiMspb() const;
    ///@}

private:
    /**
     * Captures the number of *quarter notes* per minute.
     * In MIDI, a beat is always defined as a quarter note, *not the numerator of the
     * time signature or the metronomic indication*.
     **/
    data_MIDIBPM m_midiBpm;
    /**
     * Records the number of microseconds per *quarter note*.
     * In MIDI, a beat is always defined as a quarter note, *not the numerator of the
     * time signature or the metronomic indication*. At 120 quarter notes per minute,
     * each quarter note will last 500,000 microseconds.
     **/
    data_MIDIMSPB m_midiMspb;

    /* include <attmidi.mspb> */
};

//----------------------------------------------------------------------------
// AttMidivalue
//----------------------------------------------------------------------------

class AttMidivalue : public Att {
public:
    AttMidivalue();
    virtual ~AttMidivalue();

    /** Reset the default values for the attribute class **/
    void ResetMidivalue();

    /** Read the values for the attribute class **/
    bool ReadMidivalue(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMidivalue(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetVal(data_MIDIVALUE val_) { m_val = val_; }
    data_MIDIVALUE GetVal() const { return m_val; }
    bool HasVal() const;
    ///@}

private:
    /** MIDI number. **/
    data_MIDIVALUE m_val;

    /* include <attval> */
};

//----------------------------------------------------------------------------
// AttMidivalue2
//----------------------------------------------------------------------------

class AttMidivalue2 : public Att {
public:
    AttMidivalue2();
    virtual ~AttMidivalue2();

    /** Reset the default values for the attribute class **/
    void ResetMidivalue2();

    /** Read the values for the attribute class **/
    bool ReadMidivalue2(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMidivalue2(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetVal2(data_MIDIVALUE val2_) { m_val2 = val2_; }
    data_MIDIVALUE GetVal2() const { return m_val2; }
    bool HasVal2() const;
    ///@}

private:
    /** MIDI number. **/
    data_MIDIVALUE m_val2;

    /* include <attval2> */
};

//----------------------------------------------------------------------------
// AttMidivelocity
//----------------------------------------------------------------------------

class AttMidivelocity : public Att {
public:
    AttMidivelocity();
    virtual ~AttMidivelocity();

    /** Reset the default values for the attribute class **/
    void ResetMidivelocity();

    /** Read the values for the attribute class **/
    bool ReadMidivelocity(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMidivelocity(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetVel(data_MIDIVALUE vel_) { m_vel = vel_; }
    data_MIDIVALUE GetVel() const { return m_vel; }
    bool HasVel() const;
    ///@}

private:
    /** MIDI Note-on/off velocity. **/
    data_MIDIVALUE m_vel;

    /* include <attvel> */
};

//----------------------------------------------------------------------------
// AttTimebase
//----------------------------------------------------------------------------

class AttTimebase : public Att {
public:
    AttTimebase();
    virtual ~AttTimebase();

    /** Reset the default values for the attribute class **/
    void ResetTimebase();

    /** Read the values for the attribute class **/
    bool ReadTimebase(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTimebase(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPpq(int ppq_) { m_ppq = ppq_; }
    int GetPpq() const { return m_ppq; }
    bool HasPpq() const;
    ///@}

private:
    /**
     * Indicates the number of pulses (sometimes referred to as ticks or divisions) per
     * quarter note.
     * Unlike MIDI, MEI permits different values for a score and individual staves.
     **/
    int m_ppq;

    /* include <attppq> */
};

} // vrv namespace

#endif // __VRV_ATTS_MIDI_H__
