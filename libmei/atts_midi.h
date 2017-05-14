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
// AttMidiInstrument
//----------------------------------------------------------------------------

class AttMidiInstrument : public Att {
public:
    AttMidiInstrument();
    virtual ~AttMidiInstrument();

    /** Reset the default values for the attribute class **/
    void ResetMidiInstrument();

    /** Read the values for the attribute class **/
    bool ReadMidiInstrument(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMidiInstrument(pugi::xml_node element);

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
    void SetMidiPatchname(std::string midiPatchname_) { m_midiPatchname = midiPatchname_; }
    std::string GetMidiPatchname() const { return m_midiPatchname; }
    bool HasMidiPatchname() const;
    //
    void SetMidiPatchnum(data_MIDIVALUE midiPatchnum_) { m_midiPatchnum = midiPatchnum_; }
    data_MIDIVALUE GetMidiPatchnum() const { return m_midiPatchnum; }
    bool HasMidiPatchnum() const;
    //
    void SetMidiVolume(data_MIDIVALUE midiVolume_) { m_midiVolume = midiVolume_; }
    data_MIDIVALUE GetMidiVolume() const { return m_midiVolume; }
    bool HasMidiVolume() const;
    ///@}

private:
    /** Captures the General MIDI instrument number. **/
    data_MIDIVALUE m_midiInstrnum;
    /** Provides a General MIDI label for the MIDI instrument. **/
    data_MIDINAMES m_midiInstrname;
    /**
     * Sets the instrument's position in a stereo field.
     * Values of 0 and 1 both pan left, 127 pans right, and 64 pans to the center.
     **/
    data_MIDIVALUE m_midiPan;
    /** Records a non-General MIDI patch/instrument name. **/
    std::string m_midiPatchname;
    /** Records a non-General MIDI patch/instrument number. **/
    data_MIDIVALUE m_midiPatchnum;
    /** Sets the instrument's volume. **/
    data_MIDIVALUE m_midiVolume;

    /* include <attmidi.volume> */
};

//----------------------------------------------------------------------------
// AttMidiNumber
//----------------------------------------------------------------------------

class AttMidiNumber : public Att {
public:
    AttMidiNumber();
    virtual ~AttMidiNumber();

    /** Reset the default values for the attribute class **/
    void ResetMidiNumber();

    /** Read the values for the attribute class **/
    bool ReadMidiNumber(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMidiNumber(pugi::xml_node element);

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
// AttMidiValue
//----------------------------------------------------------------------------

class AttMidiValue : public Att {
public:
    AttMidiValue();
    virtual ~AttMidiValue();

    /** Reset the default values for the attribute class **/
    void ResetMidiValue();

    /** Read the values for the attribute class **/
    bool ReadMidiValue(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMidiValue(pugi::xml_node element);

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

} // vrv namespace

#endif // __VRV_ATTS_MIDI_H__
