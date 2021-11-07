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

#ifndef __VRV_ATTS_GESTURAL_H__
#define __VRV_ATTS_GESTURAL_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttAccidentalGestural
//----------------------------------------------------------------------------

class AttAccidentalGestural : public Att {
public:
    AttAccidentalGestural();
    virtual ~AttAccidentalGestural();

    /** Reset the default values for the attribute class **/
    void ResetAccidentalGestural();

    /** Read the values for the attribute class **/
    bool ReadAccidentalGestural(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteAccidentalGestural(pugi::xml_node element);

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

    /* include <attaccid.ges> */
};

//----------------------------------------------------------------------------
// AttArticulationGestural
//----------------------------------------------------------------------------

class AttArticulationGestural : public Att {
public:
    AttArticulationGestural();
    virtual ~AttArticulationGestural();

    /** Reset the default values for the attribute class **/
    void ResetArticulationGestural();

    /** Read the values for the attribute class **/
    bool ReadArticulationGestural(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteArticulationGestural(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetArticGes(data_ARTICULATION articGes_) { m_articGes = articGes_; }
    data_ARTICULATION GetArticGes() const { return m_articGes; }
    bool HasArticGes() const;
    ///@}

private:
    /** Records performed articulation that differs from the written value. **/
    data_ARTICULATION m_articGes;

    /* include <attartic.ges> */
};

//----------------------------------------------------------------------------
// AttBendGes
//----------------------------------------------------------------------------

class AttBendGes : public Att {
public:
    AttBendGes();
    virtual ~AttBendGes();

    /** Reset the default values for the attribute class **/
    void ResetBendGes();

    /** Read the values for the attribute class **/
    bool ReadBendGes(pugi::xml_node element);

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

    /* include <attamount> */
};

//----------------------------------------------------------------------------
// AttDurationGestural
//----------------------------------------------------------------------------

class AttDurationGestural : public Att {
public:
    AttDurationGestural();
    virtual ~AttDurationGestural();

    /** Reset the default values for the attribute class **/
    void ResetDurationGestural();

    /** Read the values for the attribute class **/
    bool ReadDurationGestural(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDurationGestural(pugi::xml_node element);

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
     * Duration recorded as pulses-per-quarter note, e.g.
     * MIDI clicks or MusicXML divisions.
     **/
    int m_durPpq;
    /**
     * Duration in seconds, e.g.
     * '1.732'.
     **/
    double m_durReal;
    /** Duration as an optionally dotted Humdrum *recip value. **/
    std::string m_durRecip;

    /* include <attdur.recip> */
};

//----------------------------------------------------------------------------
// AttNcGes
//----------------------------------------------------------------------------

class AttNcGes : public Att {
public:
    AttNcGes();
    virtual ~AttNcGes();

    /** Reset the default values for the attribute class **/
    void ResetNcGes();

    /** Read the values for the attribute class **/
    bool ReadNcGes(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNcGes(pugi::xml_node element);

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

    /* include <attpnum> */
};

//----------------------------------------------------------------------------
// AttNoteGes
//----------------------------------------------------------------------------

class AttNoteGes : public Att {
public:
    AttNoteGes();
    virtual ~AttNoteGes();

    /** Reset the default values for the attribute class **/
    void ResetNoteGes();

    /** Read the values for the attribute class **/
    bool ReadNoteGes(pugi::xml_node element);

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
    //
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
    /** Indicates an extreme, indefinite performed pitch. **/
    noteGes_EXTREMIS m_extremis;
    /** Records performed octave information that differs from the written value. **/
    data_OCTAVE m_octGes;
    /** Contains a performed pitch name that differs from the written value. **/
    data_PITCHNAME m_pnameGes;
    /** Holds a pitch-to-number mapping, a base-40 or MIDI note number, for example. **/
    int m_pnum;

    /* include <attpnum> */
};

//----------------------------------------------------------------------------
// AttScoreDefGes
//----------------------------------------------------------------------------

class AttScoreDefGes : public Att {
public:
    AttScoreDefGes();
    virtual ~AttScoreDefGes();

    /** Reset the default values for the attribute class **/
    void ResetScoreDefGes();

    /** Read the values for the attribute class **/
    bool ReadScoreDefGes(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteScoreDefGes(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTunePname(data_PITCHNAME tunePname_) { m_tunePname = tunePname_; }
    data_PITCHNAME GetTunePname() const { return m_tunePname; }
    bool HasTunePname() const;
    //
    void SetTuneHz(double tuneHz_) { m_tuneHz = tuneHz_; }
    double GetTuneHz() const { return m_tuneHz; }
    bool HasTuneHz() const;
    //
    void SetTuneTemper(data_TEMPERAMENT tuneTemper_) { m_tuneTemper = tuneTemper_; }
    data_TEMPERAMENT GetTuneTemper() const { return m_tuneTemper; }
    bool HasTuneTemper() const;
    ///@}

private:
    /** Holds the pitch name of a tuning reference pitch. **/
    data_PITCHNAME m_tunePname;
    /** Holds a value for cycles per second, i.e., Hertz, for a tuning reference pitch. **/
    double m_tuneHz;
    /** Provides an indication of the tuning system, 'just', for example. **/
    data_TEMPERAMENT m_tuneTemper;

    /* include <atttune.temper> */
};

//----------------------------------------------------------------------------
// AttSectionGes
//----------------------------------------------------------------------------

class AttSectionGes : public Att {
public:
    AttSectionGes();
    virtual ~AttSectionGes();

    /** Reset the default values for the attribute class **/
    void ResetSectionGes();

    /** Read the values for the attribute class **/
    bool ReadSectionGes(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSectionGes(pugi::xml_node element);

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
     * Indicates that the performance of the next section should begin immediately
     * following this one.
     **/
    data_BOOLEAN m_attacca;

    /* include <attattacca> */
};

//----------------------------------------------------------------------------
// AttSoundLocation
//----------------------------------------------------------------------------

class AttSoundLocation : public Att {
public:
    AttSoundLocation();
    virtual ~AttSoundLocation();

    /** Reset the default values for the attribute class **/
    void ResetSoundLocation();

    /** Read the values for the attribute class **/
    bool ReadSoundLocation(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteSoundLocation(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAzimuth(double azimuth_) { m_azimuth = azimuth_; }
    double GetAzimuth() const { return m_azimuth; }
    bool HasAzimuth() const;
    //
    void SetElevation(double elevation_) { m_elevation = elevation_; }
    double GetElevation() const { return m_elevation; }
    bool HasElevation() const;
    ///@}

private:
    /** The lateral or left-to-right plane. **/
    double m_azimuth;
    /** The above-to-below axis. **/
    double m_elevation;

    /* include <attelevation> */
};

//----------------------------------------------------------------------------
// AttTimestampGestural
//----------------------------------------------------------------------------

class AttTimestampGestural : public Att {
public:
    AttTimestampGestural();
    virtual ~AttTimestampGestural();

    /** Reset the default values for the attribute class **/
    void ResetTimestampGestural();

    /** Read the values for the attribute class **/
    bool ReadTimestampGestural(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTimestampGestural(pugi::xml_node element);

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

    /* include <atttstamp.real> */
};

//----------------------------------------------------------------------------
// AttTimestamp2Gestural
//----------------------------------------------------------------------------

class AttTimestamp2Gestural : public Att {
public:
    AttTimestamp2Gestural();
    virtual ~AttTimestamp2Gestural();

    /** Reset the default values for the attribute class **/
    void ResetTimestamp2Gestural();

    /** Read the values for the attribute class **/
    bool ReadTimestamp2Gestural(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteTimestamp2Gestural(pugi::xml_node element);

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

    /* include <atttstamp2.real> */
};

} // vrv namespace

#endif // __VRV_ATTS_GESTURAL_H__
