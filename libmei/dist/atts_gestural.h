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

#ifndef __LIBMEI_ATTS_GESTURAL_H__
#define __LIBMEI_ATTS_GESTURAL_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttAccidentalGes
//----------------------------------------------------------------------------

class AttAccidentalGes : public Att {
protected:
    AttAccidentalGes();
    ~AttAccidentalGes() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetAccidentalGes();

    /** Read the values for the attribute class **/
    bool ReadAccidentalGes(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteAccidentalGes(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAccidGes(data_ACCIDENTAL_GESTURAL accidGes_) { m_accidGes = accidGes_; }
    data_ACCIDENTAL_GESTURAL GetAccidGes() const { return m_accidGes; }
    bool HasAccidGes() const;
    ///@}

private:
    /** Records the performed pitch inflection. **/
    data_ACCIDENTAL_GESTURAL m_accidGes;
};

//----------------------------------------------------------------------------
// InstAccidentalGes
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttAccidentalGes
 */

class InstAccidentalGes : public AttAccidentalGes {
public:
    InstAccidentalGes() = default;
    virtual ~InstAccidentalGes() = default;
};

//----------------------------------------------------------------------------
// AttArticulationGes
//----------------------------------------------------------------------------

class AttArticulationGes : public Att {
protected:
    AttArticulationGes();
    ~AttArticulationGes() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetArticulationGes();

    /** Read the values for the attribute class **/
    bool ReadArticulationGes(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteArticulationGes(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetArticGes(data_ARTICULATION_List articGes_) { m_articGes = articGes_; }
    data_ARTICULATION_List GetArticGes() const { return m_articGes; }
    bool HasArticGes() const;
    ///@}

private:
    /** Records performed articulation that differs from the written value. **/
    data_ARTICULATION_List m_articGes;
};

//----------------------------------------------------------------------------
// InstArticulationGes
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttArticulationGes
 */

class InstArticulationGes : public AttArticulationGes {
public:
    InstArticulationGes() = default;
    virtual ~InstArticulationGes() = default;
};

//----------------------------------------------------------------------------
// AttAttacking
//----------------------------------------------------------------------------

class AttAttacking : public Att {
protected:
    AttAttacking();
    ~AttAttacking() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetAttacking();

    /** Read the values for the attribute class **/
    bool ReadAttacking(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteAttacking(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAttacca(data_BOOLEAN attacca_) { m_attacca = attacca_; }
    data_BOOLEAN GetAttacca() const { return m_attacca; }
    bool HasAttacca() const;
    ///@}

private:
    /**
     * Indicates that the performance of the next musical division should begin
     * immediately following this one.
     **/
    data_BOOLEAN m_attacca;
};

//----------------------------------------------------------------------------
// InstAttacking
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttAttacking
 */

class InstAttacking : public AttAttacking {
public:
    InstAttacking() = default;
    virtual ~InstAttacking() = default;
};

//----------------------------------------------------------------------------
// AttBendGes
//----------------------------------------------------------------------------

class AttBendGes : public Att {
protected:
    AttBendGes();
    ~AttBendGes() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetBendGes();

    /** Read the values for the attribute class **/
    bool ReadBendGes(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteBendGes(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAmount(double amount_) { m_amount = amount_; }
    double GetAmount() const { return m_amount; }
    bool HasAmount() const;
    ///@}

private:
    /**
     * Records the amount of detuning.
     * The decimal values should be rendered as a fraction (or an integer plus a
     * fraction) along with the bend symbol.
     **/
    double m_amount;
};

//----------------------------------------------------------------------------
// InstBendGes
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttBendGes
 */

class InstBendGes : public AttBendGes {
public:
    InstBendGes() = default;
    virtual ~InstBendGes() = default;
};

//----------------------------------------------------------------------------
// AttDurationGes
//----------------------------------------------------------------------------

class AttDurationGes : public Att {
protected:
    AttDurationGes();
    ~AttDurationGes() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetDurationGes();

    /** Read the values for the attribute class **/
    bool ReadDurationGes(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteDurationGes(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDurGes(data_DURATION durGes_) { m_durGes = durGes_; }
    data_DURATION GetDurGes() const { return m_durGes; }
    bool HasDurGes() const;
    //
    void SetDotsGes(int dotsGes_) { m_dotsGes = dotsGes_; }
    int GetDotsGes() const { return m_dotsGes; }
    bool HasDotsGes() const;
    //
    void SetDurMetrical(double durMetrical_) { m_durMetrical = durMetrical_; }
    double GetDurMetrical() const { return m_durMetrical; }
    bool HasDurMetrical() const;
    //
    void SetDurPpq(int durPpq_) { m_durPpq = durPpq_; }
    int GetDurPpq() const { return m_durPpq; }
    bool HasDurPpq() const;
    //
    void SetDurReal(double durReal_) { m_durReal = durReal_; }
    double GetDurReal() const { return m_durReal; }
    bool HasDurReal() const;
    //
    void SetDurRecip(std::string durRecip_) { m_durRecip = durRecip_; }
    std::string GetDurRecip() const { return m_durRecip; }
    bool HasDurRecip() const;
    ///@}

private:
    /** Records performed duration information that differs from the written duration. **/
    data_DURATION m_durGes;
    /**
     * Number of dots required for a gestural duration when different from that of the
     * written duration.
     **/
    int m_dotsGes;
    /** Duration as a count of units provided in the time signature denominator. **/
    double m_durMetrical;
    /**
     * Duration recorded as pulses-per-quarter note, e.g., MIDI clicks or MusicXML
     * divisions.
     **/
    int m_durPpq;
    /** Duration in seconds, e.g., 1.732. **/
    double m_durReal;
    /** Duration as an optionally dotted Humdrum **recip value. **/
    std::string m_durRecip;
};

//----------------------------------------------------------------------------
// InstDurationGes
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttDurationGes
 */

class InstDurationGes : public AttDurationGes {
public:
    InstDurationGes() = default;
    virtual ~InstDurationGes() = default;
};

//----------------------------------------------------------------------------
// AttNoteGes
//----------------------------------------------------------------------------

class AttNoteGes : public Att {
protected:
    AttNoteGes();
    ~AttNoteGes() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetNoteGes();

    /** Read the values for the attribute class **/
    bool ReadNoteGes(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteNoteGes(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetExtremis(noteGes_EXTREMIS extremis_) { m_extremis = extremis_; }
    noteGes_EXTREMIS GetExtremis() const { return m_extremis; }
    bool HasExtremis() const;
    ///@}

private:
    /** Indicates an extreme, indefinite performed pitch. **/
    noteGes_EXTREMIS m_extremis;
};

//----------------------------------------------------------------------------
// InstNoteGes
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttNoteGes
 */

class InstNoteGes : public AttNoteGes {
public:
    InstNoteGes() = default;
    virtual ~InstNoteGes() = default;
};

//----------------------------------------------------------------------------
// AttOrnamentAccidGes
//----------------------------------------------------------------------------

class AttOrnamentAccidGes : public Att {
protected:
    AttOrnamentAccidGes();
    ~AttOrnamentAccidGes() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetOrnamentAccidGes();

    /** Read the values for the attribute class **/
    bool ReadOrnamentAccidGes(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteOrnamentAccidGes(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAccidupperGes(data_ACCIDENTAL_GESTURAL accidupperGes_) { m_accidupperGes = accidupperGes_; }
    data_ACCIDENTAL_GESTURAL GetAccidupperGes() const { return m_accidupperGes; }
    bool HasAccidupperGes() const;
    //
    void SetAccidlowerGes(data_ACCIDENTAL_GESTURAL accidlowerGes_) { m_accidlowerGes = accidlowerGes_; }
    data_ACCIDENTAL_GESTURAL GetAccidlowerGes() const { return m_accidlowerGes; }
    bool HasAccidlowerGes() const;
    ///@}

private:
    /** Records the sounding accidental associated with an upper neighboring note. **/
    data_ACCIDENTAL_GESTURAL m_accidupperGes;
    /** Records the sounding accidental associated with a lower neighboring note. **/
    data_ACCIDENTAL_GESTURAL m_accidlowerGes;
};

//----------------------------------------------------------------------------
// InstOrnamentAccidGes
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttOrnamentAccidGes
 */

class InstOrnamentAccidGes : public AttOrnamentAccidGes {
public:
    InstOrnamentAccidGes() = default;
    virtual ~InstOrnamentAccidGes() = default;
};

//----------------------------------------------------------------------------
// AttPitchGes
//----------------------------------------------------------------------------

class AttPitchGes : public Att {
protected:
    AttPitchGes();
    ~AttPitchGes() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetPitchGes();

    /** Read the values for the attribute class **/
    bool ReadPitchGes(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WritePitchGes(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOctGes(data_OCTAVE octGes_) { m_octGes = octGes_; }
    data_OCTAVE GetOctGes() const { return m_octGes; }
    bool HasOctGes() const;
    //
    void SetPnameGes(data_PITCHNAME pnameGes_) { m_pnameGes = pnameGes_; }
    data_PITCHNAME GetPnameGes() const { return m_pnameGes; }
    bool HasPnameGes() const;
    //
    void SetPnum(int pnum_) { m_pnum = pnum_; }
    int GetPnum() const { return m_pnum; }
    bool HasPnum() const;
    ///@}

private:
    /** Records performed octave information that differs from the written value. **/
    data_OCTAVE m_octGes;
    /** Contains a performed pitch name that differs from the written value. **/
    data_PITCHNAME m_pnameGes;
    /** Holds a pitch-to-number mapping, a base-40 or MIDI note number, for example. **/
    int m_pnum;
};

//----------------------------------------------------------------------------
// InstPitchGes
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttPitchGes
 */

class InstPitchGes : public AttPitchGes {
public:
    InstPitchGes() = default;
    virtual ~InstPitchGes() = default;
};

//----------------------------------------------------------------------------
// AttSoundLocation
//----------------------------------------------------------------------------

class AttSoundLocation : public Att {
protected:
    AttSoundLocation();
    ~AttSoundLocation() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetSoundLocation();

    /** Read the values for the attribute class **/
    bool ReadSoundLocation(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteSoundLocation(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAzimuth(data_DEGREES azimuth_) { m_azimuth = azimuth_; }
    data_DEGREES GetAzimuth() const { return m_azimuth; }
    bool HasAzimuth() const;
    //
    void SetElevation(data_DEGREES elevation_) { m_elevation = elevation_; }
    data_DEGREES GetElevation() const { return m_elevation; }
    bool HasElevation() const;
    ///@}

private:
    /** The lateral or left-to-right plane. **/
    data_DEGREES m_azimuth;
    /** The above-to-below axis. **/
    data_DEGREES m_elevation;
};

//----------------------------------------------------------------------------
// InstSoundLocation
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttSoundLocation
 */

class InstSoundLocation : public AttSoundLocation {
public:
    InstSoundLocation() = default;
    virtual ~InstSoundLocation() = default;
};

//----------------------------------------------------------------------------
// AttTimestampGes
//----------------------------------------------------------------------------

class AttTimestampGes : public Att {
protected:
    AttTimestampGes();
    ~AttTimestampGes() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetTimestampGes();

    /** Read the values for the attribute class **/
    bool ReadTimestampGes(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteTimestampGes(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTstampGes(double tstampGes_) { m_tstampGes = tstampGes_; }
    double GetTstampGes() const { return m_tstampGes; }
    bool HasTstampGes() const;
    //
    void SetTstampReal(std::string tstampReal_) { m_tstampReal = tstampReal_; }
    std::string GetTstampReal() const { return m_tstampReal; }
    bool HasTstampReal() const;
    ///@}

private:
    /**
     * Encodes the onset time in terms of musical time, i.e., beats[.fractional beat
     * part], as expressed in the written time signature.
     **/
    double m_tstampGes;
    /** Records the onset time in terms of ISO time. **/
    std::string m_tstampReal;
};

//----------------------------------------------------------------------------
// InstTimestampGes
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttTimestampGes
 */

class InstTimestampGes : public AttTimestampGes {
public:
    InstTimestampGes() = default;
    virtual ~InstTimestampGes() = default;
};

//----------------------------------------------------------------------------
// AttTimestamp2Ges
//----------------------------------------------------------------------------

class AttTimestamp2Ges : public Att {
protected:
    AttTimestamp2Ges();
    ~AttTimestamp2Ges() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetTimestamp2Ges();

    /** Read the values for the attribute class **/
    bool ReadTimestamp2Ges(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteTimestamp2Ges(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTstamp2Ges(data_MEASUREBEAT tstamp2Ges_) { m_tstamp2Ges = tstamp2Ges_; }
    data_MEASUREBEAT GetTstamp2Ges() const { return m_tstamp2Ges; }
    bool HasTstamp2Ges() const;
    //
    void SetTstamp2Real(std::string tstamp2Real_) { m_tstamp2Real = tstamp2Real_; }
    std::string GetTstamp2Real() const { return m_tstamp2Real; }
    bool HasTstamp2Real() const;
    ///@}

private:
    /**
     * Encodes the ending point of an event, i.e., a count of measures plus a beat
     * location in the ending measure.
     **/
    data_MEASUREBEAT m_tstamp2Ges;
    /** Records the ending point of an event in terms of ISO time. **/
    std::string m_tstamp2Real;
};

//----------------------------------------------------------------------------
// InstTimestamp2Ges
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttTimestamp2Ges
 */

class InstTimestamp2Ges : public AttTimestamp2Ges {
public:
    InstTimestamp2Ges() = default;
    virtual ~InstTimestamp2Ges() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_GESTURAL_H__
