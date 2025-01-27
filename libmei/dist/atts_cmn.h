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

#ifndef __LIBMEI_ATTS_CMN_H__
#define __LIBMEI_ATTS_CMN_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttArpegLog
//----------------------------------------------------------------------------

class AttArpegLog : public Att {
protected:
    AttArpegLog();
    ~AttArpegLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetArpegLog();

    /** Read the values for the attribute class **/
    bool ReadArpegLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteArpegLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetOrder(arpegLog_ORDER order_) { m_order = order_; }
    arpegLog_ORDER GetOrder() const { return m_order; }
    bool HasOrder() const;
    ///@}

private:
    /** Describes the direction in which an arpeggio is to be performed. **/
    arpegLog_ORDER m_order;
};

//----------------------------------------------------------------------------
// InstArpegLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttArpegLog
 */

class InstArpegLog : public AttArpegLog {
public:
    InstArpegLog() = default;
    virtual ~InstArpegLog() = default;
};

//----------------------------------------------------------------------------
// AttBeamPresent
//----------------------------------------------------------------------------

class AttBeamPresent : public Att {
protected:
    AttBeamPresent();
    ~AttBeamPresent() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetBeamPresent();

    /** Read the values for the attribute class **/
    bool ReadBeamPresent(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteBeamPresent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBeam(std::string beam_) { m_beam = beam_; }
    std::string GetBeam() const { return m_beam; }
    bool HasBeam() const;
    ///@}

private:
    /** Indicates that this event is "under a beam". **/
    std::string m_beam;
};

//----------------------------------------------------------------------------
// InstBeamPresent
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttBeamPresent
 */

class InstBeamPresent : public AttBeamPresent {
public:
    InstBeamPresent() = default;
    virtual ~InstBeamPresent() = default;
};

//----------------------------------------------------------------------------
// AttBeamRend
//----------------------------------------------------------------------------

class AttBeamRend : public Att {
protected:
    AttBeamRend();
    ~AttBeamRend() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetBeamRend();

    /** Read the values for the attribute class **/
    bool ReadBeamRend(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteBeamRend(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(beamRend_FORM form_) { m_form = form_; }
    beamRend_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    //
    void SetPlace(data_BEAMPLACE place_) { m_place = place_; }
    data_BEAMPLACE GetPlace() const { return m_place; }
    bool HasPlace() const;
    //
    void SetSlash(data_BOOLEAN slash_) { m_slash = slash_; }
    data_BOOLEAN GetSlash() const { return m_slash; }
    bool HasSlash() const;
    //
    void SetSlope(double slope_) { m_slope = slope_; }
    double GetSlope() const { return m_slope; }
    bool HasSlope() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    beamRend_FORM m_form;
    /** Records the placement of the beam relative to the events it affects. **/
    data_BEAMPLACE m_place;
    /** Indicates presence of slash through the beam. **/
    data_BOOLEAN m_slash;
    /** Records the slope of the beam. **/
    double m_slope;
};

//----------------------------------------------------------------------------
// InstBeamRend
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttBeamRend
 */

class InstBeamRend : public AttBeamRend {
public:
    InstBeamRend() = default;
    virtual ~InstBeamRend() = default;
};

//----------------------------------------------------------------------------
// AttBeamSecondary
//----------------------------------------------------------------------------

class AttBeamSecondary : public Att {
protected:
    AttBeamSecondary();
    ~AttBeamSecondary() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetBeamSecondary();

    /** Read the values for the attribute class **/
    bool ReadBeamSecondary(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteBeamSecondary(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBreaksec(int breaksec_) { m_breaksec = breaksec_; }
    int GetBreaksec() const { return m_breaksec; }
    bool HasBreaksec() const;
    ///@}

private:
    /**
     * Presence of this attribute indicates that the secondary beam should be broken
     * following this note/chord.
     * The value of the attribute records the number of beams which should remain
     * unbroken.
     **/
    int m_breaksec;
};

//----------------------------------------------------------------------------
// InstBeamSecondary
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttBeamSecondary
 */

class InstBeamSecondary : public AttBeamSecondary {
public:
    InstBeamSecondary() = default;
    virtual ~InstBeamSecondary() = default;
};

//----------------------------------------------------------------------------
// AttBeamedWith
//----------------------------------------------------------------------------

class AttBeamedWith : public Att {
protected:
    AttBeamedWith();
    ~AttBeamedWith() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetBeamedWith();

    /** Read the values for the attribute class **/
    bool ReadBeamedWith(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteBeamedWith(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBeamWith(data_NEIGHBORINGLAYER beamWith_) { m_beamWith = beamWith_; }
    data_NEIGHBORINGLAYER GetBeamWith() const { return m_beamWith; }
    bool HasBeamWith() const;
    ///@}

private:
    /**
     * In the case of cross-staff beams, the beam.with attribute is used to indicate
     * which staff the beam is connected to; that is, the staff above or the staff
     * below.
     **/
    data_NEIGHBORINGLAYER m_beamWith;
};

//----------------------------------------------------------------------------
// InstBeamedWith
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttBeamedWith
 */

class InstBeamedWith : public AttBeamedWith {
public:
    InstBeamedWith() = default;
    virtual ~InstBeamedWith() = default;
};

//----------------------------------------------------------------------------
// AttBeamingLog
//----------------------------------------------------------------------------

class AttBeamingLog : public Att {
protected:
    AttBeamingLog();
    ~AttBeamingLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetBeamingLog();

    /** Read the values for the attribute class **/
    bool ReadBeamingLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteBeamingLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBeamGroup(std::string beamGroup_) { m_beamGroup = beamGroup_; }
    std::string GetBeamGroup() const { return m_beamGroup; }
    bool HasBeamGroup() const;
    //
    void SetBeamRests(data_BOOLEAN beamRests_) { m_beamRests = beamRests_; }
    data_BOOLEAN GetBeamRests() const { return m_beamRests; }
    bool HasBeamRests() const;
    ///@}

private:
    /**
     * Provides an example of how automated beaming (including secondary beams) is to
     * be performed.
     **/
    std::string m_beamGroup;
    /**
     * Indicates whether automatically-drawn beams should include rests shorter than a
     * quarter note duration.
     **/
    data_BOOLEAN m_beamRests;
};

//----------------------------------------------------------------------------
// InstBeamingLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttBeamingLog
 */

class InstBeamingLog : public AttBeamingLog {
public:
    InstBeamingLog() = default;
    virtual ~InstBeamingLog() = default;
};

//----------------------------------------------------------------------------
// AttBeatRptLog
//----------------------------------------------------------------------------

class AttBeatRptLog : public Att {
protected:
    AttBeatRptLog();
    ~AttBeatRptLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetBeatRptLog();

    /** Read the values for the attribute class **/
    bool ReadBeatRptLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteBeatRptLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBeatdef(double beatdef_) { m_beatdef = beatdef_; }
    double GetBeatdef() const { return m_beatdef; }
    bool HasBeatdef() const;
    ///@}

private:
    /**
     * Indicates the performed duration represented by the beatRpt symbol; expressed in
     * time signature denominator units.
     **/
    double m_beatdef;
};

//----------------------------------------------------------------------------
// InstBeatRptLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttBeatRptLog
 */

class InstBeatRptLog : public AttBeatRptLog {
public:
    InstBeatRptLog() = default;
    virtual ~InstBeatRptLog() = default;
};

//----------------------------------------------------------------------------
// AttBracketSpanLog
//----------------------------------------------------------------------------

class AttBracketSpanLog : public Att {
protected:
    AttBracketSpanLog();
    ~AttBracketSpanLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetBracketSpanLog();

    /** Read the values for the attribute class **/
    bool ReadBracketSpanLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteBracketSpanLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFunc(bracketSpanLog_FUNC func_) { m_func = func_; }
    bracketSpanLog_FUNC GetFunc() const { return m_func; }
    bool HasFunc() const;
    ///@}

private:
    /** Describes the function of the bracketed event sequence. **/
    bracketSpanLog_FUNC m_func;
};

//----------------------------------------------------------------------------
// InstBracketSpanLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttBracketSpanLog
 */

class InstBracketSpanLog : public AttBracketSpanLog {
public:
    InstBracketSpanLog() = default;
    virtual ~InstBracketSpanLog() = default;
};

//----------------------------------------------------------------------------
// AttCutout
//----------------------------------------------------------------------------

class AttCutout : public Att {
protected:
    AttCutout();
    ~AttCutout() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetCutout();

    /** Read the values for the attribute class **/
    bool ReadCutout(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteCutout(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetCutout(cutout_CUTOUT cutout_) { m_cutout = cutout_; }
    cutout_CUTOUT GetCutout() const { return m_cutout; }
    bool HasCutout() const;
    ///@}

private:
    /** "Cut-out" style. **/
    cutout_CUTOUT m_cutout;
};

//----------------------------------------------------------------------------
// InstCutout
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttCutout
 */

class InstCutout : public AttCutout {
public:
    InstCutout() = default;
    virtual ~InstCutout() = default;
};

//----------------------------------------------------------------------------
// AttExpandable
//----------------------------------------------------------------------------

class AttExpandable : public Att {
protected:
    AttExpandable();
    ~AttExpandable() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetExpandable();

    /** Read the values for the attribute class **/
    bool ReadExpandable(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteExpandable(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetExpand(data_BOOLEAN expand_) { m_expand = expand_; }
    data_BOOLEAN GetExpand() const { return m_expand; }
    bool HasExpand() const;
    ///@}

private:
    /**
     * Indicates whether to render a repeat symbol or the source material to which it
     * refers.
     * A value of 'true' renders the source material, while 'false' displays the repeat
     * symbol.
     **/
    data_BOOLEAN m_expand;
};

//----------------------------------------------------------------------------
// InstExpandable
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttExpandable
 */

class InstExpandable : public AttExpandable {
public:
    InstExpandable() = default;
    virtual ~InstExpandable() = default;
};

//----------------------------------------------------------------------------
// AttGlissPresent
//----------------------------------------------------------------------------

class AttGlissPresent : public Att {
protected:
    AttGlissPresent();
    ~AttGlissPresent() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetGlissPresent();

    /** Read the values for the attribute class **/
    bool ReadGlissPresent(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteGlissPresent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetGliss(data_GLISSANDO gliss_) { m_gliss = gliss_; }
    data_GLISSANDO GetGliss() const { return m_gliss; }
    bool HasGliss() const;
    ///@}

private:
    /**
     * Indicates that this element participates in a glissando.
     * If visual information about the glissando needs to be recorded, then a gliss
     * element should be employed instead.
     **/
    data_GLISSANDO m_gliss;
};

//----------------------------------------------------------------------------
// InstGlissPresent
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttGlissPresent
 */

class InstGlissPresent : public AttGlissPresent {
public:
    InstGlissPresent() = default;
    virtual ~InstGlissPresent() = default;
};

//----------------------------------------------------------------------------
// AttGraceGrpLog
//----------------------------------------------------------------------------

class AttGraceGrpLog : public Att {
protected:
    AttGraceGrpLog();
    ~AttGraceGrpLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetGraceGrpLog();

    /** Read the values for the attribute class **/
    bool ReadGraceGrpLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteGraceGrpLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetAttach(graceGrpLog_ATTACH attach_) { m_attach = attach_; }
    graceGrpLog_ATTACH GetAttach() const { return m_attach; }
    bool HasAttach() const;
    ///@}

private:
    /**
     * Records whether the grace note group is attached to the following event or to
     * the preceding one.
     * The usual name for the latter is "Nachschlag".
     **/
    graceGrpLog_ATTACH m_attach;
};

//----------------------------------------------------------------------------
// InstGraceGrpLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttGraceGrpLog
 */

class InstGraceGrpLog : public AttGraceGrpLog {
public:
    InstGraceGrpLog() = default;
    virtual ~InstGraceGrpLog() = default;
};

//----------------------------------------------------------------------------
// AttGraced
//----------------------------------------------------------------------------

class AttGraced : public Att {
protected:
    AttGraced();
    ~AttGraced() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetGraced();

    /** Read the values for the attribute class **/
    bool ReadGraced(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteGraced(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetGrace(data_GRACE grace_) { m_grace = grace_; }
    data_GRACE GetGrace() const { return m_grace; }
    bool HasGrace() const;
    //
    void SetGraceTime(data_PERCENT graceTime_) { m_graceTime = graceTime_; }
    data_PERCENT GetGraceTime() const { return m_graceTime; }
    bool HasGraceTime() const;
    ///@}

private:
    /**
     * Marks a note or chord as a "grace" (without a definite performed duration) and
     * records from which other note/chord it should "steal" time.
     **/
    data_GRACE m_grace;
    /** Records the amount of time to be "stolen" from a non-grace note/chord. **/
    data_PERCENT m_graceTime;
};

//----------------------------------------------------------------------------
// InstGraced
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttGraced
 */

class InstGraced : public AttGraced {
public:
    InstGraced() = default;
    virtual ~InstGraced() = default;
};

//----------------------------------------------------------------------------
// AttHairpinLog
//----------------------------------------------------------------------------

class AttHairpinLog : public Att {
protected:
    AttHairpinLog();
    ~AttHairpinLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetHairpinLog();

    /** Read the values for the attribute class **/
    bool ReadHairpinLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteHairpinLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(hairpinLog_FORM form_) { m_form = form_; }
    hairpinLog_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    //
    void SetNiente(data_BOOLEAN niente_) { m_niente = niente_; }
    data_BOOLEAN GetNiente() const { return m_niente; }
    bool HasNiente() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    hairpinLog_FORM m_form;
    /**
     * Indicates that the hairpin starts from or ends in silence.
     * Often rendered as a small circle attached to the closed end of the hairpin. See
     * Gould, p. 108.
     **/
    data_BOOLEAN m_niente;
};

//----------------------------------------------------------------------------
// InstHairpinLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttHairpinLog
 */

class InstHairpinLog : public AttHairpinLog {
public:
    InstHairpinLog() = default;
    virtual ~InstHairpinLog() = default;
};

//----------------------------------------------------------------------------
// AttHarpPedalLog
//----------------------------------------------------------------------------

class AttHarpPedalLog : public Att {
protected:
    AttHarpPedalLog();
    ~AttHarpPedalLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetHarpPedalLog();

    /** Read the values for the attribute class **/
    bool ReadHarpPedalLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteHarpPedalLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetC(data_HARPPEDALPOSITION c_) { m_c = c_; }
    data_HARPPEDALPOSITION GetC() const { return m_c; }
    bool HasC() const;
    //
    void SetD(data_HARPPEDALPOSITION d_) { m_d = d_; }
    data_HARPPEDALPOSITION GetD() const { return m_d; }
    bool HasD() const;
    //
    void SetE(data_HARPPEDALPOSITION e_) { m_e = e_; }
    data_HARPPEDALPOSITION GetE() const { return m_e; }
    bool HasE() const;
    //
    void SetF(data_HARPPEDALPOSITION f_) { m_f = f_; }
    data_HARPPEDALPOSITION GetF() const { return m_f; }
    bool HasF() const;
    //
    void SetG(data_HARPPEDALPOSITION g_) { m_g = g_; }
    data_HARPPEDALPOSITION GetG() const { return m_g; }
    bool HasG() const;
    //
    void SetA(data_HARPPEDALPOSITION a_) { m_a = a_; }
    data_HARPPEDALPOSITION GetA() const { return m_a; }
    bool HasA() const;
    //
    void SetB(data_HARPPEDALPOSITION b_) { m_b = b_; }
    data_HARPPEDALPOSITION GetB() const { return m_b; }
    bool HasB() const;
    ///@}

private:
    /** Indicates the pedal setting for the harp’s C strings. **/
    data_HARPPEDALPOSITION m_c;
    /** Indicates the pedal setting for the harp’s D strings. **/
    data_HARPPEDALPOSITION m_d;
    /** Indicates the pedal setting for the harp’s E strings. **/
    data_HARPPEDALPOSITION m_e;
    /** Indicates the pedal setting for the harp’s F strings. **/
    data_HARPPEDALPOSITION m_f;
    /** Indicates the pedal setting for the harp’s G strings. **/
    data_HARPPEDALPOSITION m_g;
    /** Indicates the pedal setting for the harp’s A strings. **/
    data_HARPPEDALPOSITION m_a;
    /** Indicates the pedal setting for the harp’s B strings. **/
    data_HARPPEDALPOSITION m_b;
};

//----------------------------------------------------------------------------
// InstHarpPedalLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttHarpPedalLog
 */

class InstHarpPedalLog : public AttHarpPedalLog {
public:
    InstHarpPedalLog() = default;
    virtual ~InstHarpPedalLog() = default;
};

//----------------------------------------------------------------------------
// AttLvPresent
//----------------------------------------------------------------------------

class AttLvPresent : public Att {
protected:
    AttLvPresent();
    ~AttLvPresent() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetLvPresent();

    /** Read the values for the attribute class **/
    bool ReadLvPresent(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteLvPresent(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLv(data_BOOLEAN lv_) { m_lv = lv_; }
    data_BOOLEAN GetLv() const { return m_lv; }
    bool HasLv() const;
    ///@}

private:
    /**
     * Indicates the attachment of an l.v.
     * (laissez vibrer) sign to this element.
     **/
    data_BOOLEAN m_lv;
};

//----------------------------------------------------------------------------
// InstLvPresent
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttLvPresent
 */

class InstLvPresent : public AttLvPresent {
public:
    InstLvPresent() = default;
    virtual ~InstLvPresent() = default;
};

//----------------------------------------------------------------------------
// AttMeasureLog
//----------------------------------------------------------------------------

class AttMeasureLog : public Att {
protected:
    AttMeasureLog();
    ~AttMeasureLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetMeasureLog();

    /** Read the values for the attribute class **/
    bool ReadMeasureLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteMeasureLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLeft(data_BARRENDITION left_) { m_left = left_; }
    data_BARRENDITION GetLeft() const { return m_left; }
    bool HasLeft() const;
    //
    void SetRight(data_BARRENDITION right_) { m_right = right_; }
    data_BARRENDITION GetRight() const { return m_right; }
    bool HasRight() const;
    ///@}

private:
    /**
     * Indicates the visual rendition of the left bar line.
     * It is present here only for facilitation of translation from legacy encodings
     * which use it. Usually, it can be safely ignored.
     **/
    data_BARRENDITION m_left;
    /** Indicates the function of the right bar line and is structurally important. **/
    data_BARRENDITION m_right;
};

//----------------------------------------------------------------------------
// InstMeasureLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttMeasureLog
 */

class InstMeasureLog : public AttMeasureLog {
public:
    InstMeasureLog() = default;
    virtual ~InstMeasureLog() = default;
};

//----------------------------------------------------------------------------
// AttMeterSigGrpLog
//----------------------------------------------------------------------------

class AttMeterSigGrpLog : public Att {
protected:
    AttMeterSigGrpLog();
    ~AttMeterSigGrpLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetMeterSigGrpLog();

    /** Read the values for the attribute class **/
    bool ReadMeterSigGrpLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteMeterSigGrpLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFunc(meterSigGrpLog_FUNC func_) { m_func = func_; }
    meterSigGrpLog_FUNC GetFunc() const { return m_func; }
    bool HasFunc() const;
    ///@}

private:
    /** Describes the function of the bracketed event sequence. **/
    meterSigGrpLog_FUNC m_func;
};

//----------------------------------------------------------------------------
// InstMeterSigGrpLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttMeterSigGrpLog
 */

class InstMeterSigGrpLog : public AttMeterSigGrpLog {
public:
    InstMeterSigGrpLog() = default;
    virtual ~InstMeterSigGrpLog() = default;
};

//----------------------------------------------------------------------------
// AttNumberPlacement
//----------------------------------------------------------------------------

class AttNumberPlacement : public Att {
protected:
    AttNumberPlacement();
    ~AttNumberPlacement() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetNumberPlacement();

    /** Read the values for the attribute class **/
    bool ReadNumberPlacement(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteNumberPlacement(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetNumPlace(data_STAFFREL_basic numPlace_) { m_numPlace = numPlace_; }
    data_STAFFREL_basic GetNumPlace() const { return m_numPlace; }
    bool HasNumPlace() const;
    //
    void SetNumVisible(data_BOOLEAN numVisible_) { m_numVisible = numVisible_; }
    data_BOOLEAN GetNumVisible() const { return m_numVisible; }
    bool HasNumVisible() const;
    ///@}

private:
    /** States where the tuplet number will be placed in relation to the note heads. **/
    data_STAFFREL_basic m_numPlace;
    /** Determines if the tuplet number is visible. **/
    data_BOOLEAN m_numVisible;
};

//----------------------------------------------------------------------------
// InstNumberPlacement
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttNumberPlacement
 */

class InstNumberPlacement : public AttNumberPlacement {
public:
    InstNumberPlacement() = default;
    virtual ~InstNumberPlacement() = default;
};

//----------------------------------------------------------------------------
// AttNumbered
//----------------------------------------------------------------------------

class AttNumbered : public Att {
protected:
    AttNumbered();
    ~AttNumbered() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetNumbered();

    /** Read the values for the attribute class **/
    bool ReadNumbered(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteNumbered(pugi::xml_node element);

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
    /** Records a number or count accompanying a notational feature. **/
    int m_num;
};

//----------------------------------------------------------------------------
// InstNumbered
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttNumbered
 */

class InstNumbered : public AttNumbered {
public:
    InstNumbered() = default;
    virtual ~InstNumbered() = default;
};

//----------------------------------------------------------------------------
// AttOctaveLog
//----------------------------------------------------------------------------

class AttOctaveLog : public Att {
protected:
    AttOctaveLog();
    ~AttOctaveLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetOctaveLog();

    /** Read the values for the attribute class **/
    bool ReadOctaveLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteOctaveLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetColl(octaveLog_COLL coll_) { m_coll = coll_; }
    octaveLog_COLL GetColl() const { return m_coll; }
    bool HasColl() const;
    ///@}

private:
    /**
     * Indicates whether the octave displacement should be performed simultaneously
     * with the written notes, i.e., "coll' ottava".
     * Unlike other octave signs which are indicated by broken lines, coll' ottava
     * typically uses an unbroken line or a series of longer broken lines, ending with
     * a short vertical stroke. See Read, p. 47-48.
     **/
    octaveLog_COLL m_coll;
};

//----------------------------------------------------------------------------
// InstOctaveLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttOctaveLog
 */

class InstOctaveLog : public AttOctaveLog {
public:
    InstOctaveLog() = default;
    virtual ~InstOctaveLog() = default;
};

//----------------------------------------------------------------------------
// AttPedalLog
//----------------------------------------------------------------------------

class AttPedalLog : public Att {
protected:
    AttPedalLog();
    ~AttPedalLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetPedalLog();

    /** Read the values for the attribute class **/
    bool ReadPedalLog(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WritePedalLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDir(pedalLog_DIR dir_) { m_dir = dir_; }
    pedalLog_DIR GetDir() const { return m_dir; }
    bool HasDir() const;
    //
    void SetFunc(std::string func_) { m_func = func_; }
    std::string GetFunc() const { return m_func; }
    bool HasFunc() const;
    ///@}

private:
    /** Records the position of the piano damper pedal. **/
    pedalLog_DIR m_dir;
    /** Describes the function of the bracketed event sequence. **/
    std::string m_func;
};

//----------------------------------------------------------------------------
// InstPedalLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttPedalLog
 */

class InstPedalLog : public AttPedalLog {
public:
    InstPedalLog() = default;
    virtual ~InstPedalLog() = default;
};

//----------------------------------------------------------------------------
// AttPianoPedals
//----------------------------------------------------------------------------

class AttPianoPedals : public Att {
protected:
    AttPianoPedals();
    ~AttPianoPedals() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetPianoPedals();

    /** Read the values for the attribute class **/
    bool ReadPianoPedals(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WritePianoPedals(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPedalStyle(data_PEDALSTYLE pedalStyle_) { m_pedalStyle = pedalStyle_; }
    data_PEDALSTYLE GetPedalStyle() const { return m_pedalStyle; }
    bool HasPedalStyle() const;
    ///@}

private:
    /** Determines whether piano pedal marks should be rendered as lines or as terms. **/
    data_PEDALSTYLE m_pedalStyle;
};

//----------------------------------------------------------------------------
// InstPianoPedals
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttPianoPedals
 */

class InstPianoPedals : public AttPianoPedals {
public:
    InstPianoPedals() = default;
    virtual ~InstPianoPedals() = default;
};

//----------------------------------------------------------------------------
// AttRehearsal
//----------------------------------------------------------------------------

class AttRehearsal : public Att {
protected:
    AttRehearsal();
    ~AttRehearsal() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetRehearsal();

    /** Read the values for the attribute class **/
    bool ReadRehearsal(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteRehearsal(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetRehEnclose(rehearsal_REHENCLOSE rehEnclose_) { m_rehEnclose = rehEnclose_; }
    rehearsal_REHENCLOSE GetRehEnclose() const { return m_rehEnclose; }
    bool HasRehEnclose() const;
    ///@}

private:
    /** Describes the enclosing shape for rehearsal marks. **/
    rehearsal_REHENCLOSE m_rehEnclose;
};

//----------------------------------------------------------------------------
// InstRehearsal
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttRehearsal
 */

class InstRehearsal : public AttRehearsal {
public:
    InstRehearsal() = default;
    virtual ~InstRehearsal() = default;
};

//----------------------------------------------------------------------------
// AttSlurRend
//----------------------------------------------------------------------------

class AttSlurRend : public Att {
protected:
    AttSlurRend();
    ~AttSlurRend() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetSlurRend();

    /** Read the values for the attribute class **/
    bool ReadSlurRend(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteSlurRend(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSlurLform(data_LINEFORM slurLform_) { m_slurLform = slurLform_; }
    data_LINEFORM GetSlurLform() const { return m_slurLform; }
    bool HasSlurLform() const;
    //
    void SetSlurLwidth(data_LINEWIDTH slurLwidth_) { m_slurLwidth = slurLwidth_; }
    data_LINEWIDTH GetSlurLwidth() const { return m_slurLwidth; }
    bool HasSlurLwidth() const;
    /** Getter for reference (for alternate type only) */
    data_LINEWIDTH *GetSlurLwidthAlternate() { return &m_slurLwidth; }
    ///@}

private:
    /** --- **/
    data_LINEFORM m_slurLform;
    /** --- **/
    data_LINEWIDTH m_slurLwidth;
};

//----------------------------------------------------------------------------
// InstSlurRend
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttSlurRend
 */

class InstSlurRend : public AttSlurRend {
public:
    InstSlurRend() = default;
    virtual ~InstSlurRend() = default;
};

//----------------------------------------------------------------------------
// AttStemsCmn
//----------------------------------------------------------------------------

class AttStemsCmn : public Att {
protected:
    AttStemsCmn();
    ~AttStemsCmn() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetStemsCmn();

    /** Read the values for the attribute class **/
    bool ReadStemsCmn(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteStemsCmn(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetStemWith(data_NEIGHBORINGLAYER stemWith_) { m_stemWith = stemWith_; }
    data_NEIGHBORINGLAYER GetStemWith() const { return m_stemWith; }
    bool HasStemWith() const;
    ///@}

private:
    /**
     * Contains an indication of which staff a note or chord that logically belongs to
     * the current staff should be visually placed on; that is, the one above or the
     * one below.
     **/
    data_NEIGHBORINGLAYER m_stemWith;
};

//----------------------------------------------------------------------------
// InstStemsCmn
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttStemsCmn
 */

class InstStemsCmn : public AttStemsCmn {
public:
    InstStemsCmn() = default;
    virtual ~InstStemsCmn() = default;
};

//----------------------------------------------------------------------------
// AttTieRend
//----------------------------------------------------------------------------

class AttTieRend : public Att {
protected:
    AttTieRend();
    ~AttTieRend() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetTieRend();

    /** Read the values for the attribute class **/
    bool ReadTieRend(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteTieRend(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetTieLform(data_LINEFORM tieLform_) { m_tieLform = tieLform_; }
    data_LINEFORM GetTieLform() const { return m_tieLform; }
    bool HasTieLform() const;
    //
    void SetTieLwidth(data_LINEWIDTH tieLwidth_) { m_tieLwidth = tieLwidth_; }
    data_LINEWIDTH GetTieLwidth() const { return m_tieLwidth; }
    bool HasTieLwidth() const;
    /** Getter for reference (for alternate type only) */
    data_LINEWIDTH *GetTieLwidthAlternate() { return &m_tieLwidth; }
    ///@}

private:
    /** --- **/
    data_LINEFORM m_tieLform;
    /** --- **/
    data_LINEWIDTH m_tieLwidth;
};

//----------------------------------------------------------------------------
// InstTieRend
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttTieRend
 */

class InstTieRend : public AttTieRend {
public:
    InstTieRend() = default;
    virtual ~InstTieRend() = default;
};

//----------------------------------------------------------------------------
// AttTremForm
//----------------------------------------------------------------------------

class AttTremForm : public Att {
protected:
    AttTremForm();
    ~AttTremForm() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetTremForm();

    /** Read the values for the attribute class **/
    bool ReadTremForm(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteTremForm(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(tremForm_FORM form_) { m_form = form_; }
    tremForm_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    tremForm_FORM m_form;
};

//----------------------------------------------------------------------------
// InstTremForm
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttTremForm
 */

class InstTremForm : public AttTremForm {
public:
    InstTremForm() = default;
    virtual ~InstTremForm() = default;
};

//----------------------------------------------------------------------------
// AttTremMeasured
//----------------------------------------------------------------------------

class AttTremMeasured : public Att {
protected:
    AttTremMeasured();
    ~AttTremMeasured() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetTremMeasured();

    /** Read the values for the attribute class **/
    bool ReadTremMeasured(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteTremMeasured(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetUnitdur(data_DURATION unitdur_) { m_unitdur = unitdur_; }
    data_DURATION GetUnitdur() const { return m_unitdur; }
    bool HasUnitdur() const;
    ///@}

private:
    /** The performed duration of an individual note in a measured tremolo. **/
    data_DURATION m_unitdur;
};

//----------------------------------------------------------------------------
// InstTremMeasured
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttTremMeasured
 */

class InstTremMeasured : public AttTremMeasured {
public:
    InstTremMeasured() = default;
    virtual ~InstTremMeasured() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_CMN_H__
