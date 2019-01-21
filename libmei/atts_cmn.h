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

#ifndef __VRV_ATTS_CMN_H__
#define __VRV_ATTS_CMN_H__

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
public:
    AttArpegLog();
    virtual ~AttArpegLog();

    /** Reset the default values for the attribute class **/
    void ResetArpegLog();

    /** Read the values for the attribute class **/
    bool ReadArpegLog(pugi::xml_node element);

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

    /* include <attorder> */
};

//----------------------------------------------------------------------------
// AttBTremLog
//----------------------------------------------------------------------------

class AttBTremLog : public Att {
public:
    AttBTremLog();
    virtual ~AttBTremLog();

    /** Reset the default values for the attribute class **/
    void ResetBTremLog();

    /** Read the values for the attribute class **/
    bool ReadBTremLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteBTremLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(bTremLog_FORM form_) { m_form = form_; }
    bTremLog_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    bTremLog_FORM m_form;

    /* include <attform> */
};

//----------------------------------------------------------------------------
// AttBeamPresent
//----------------------------------------------------------------------------

class AttBeamPresent : public Att {
public:
    AttBeamPresent();
    virtual ~AttBeamPresent();

    /** Reset the default values for the attribute class **/
    void ResetBeamPresent();

    /** Read the values for the attribute class **/
    bool ReadBeamPresent(pugi::xml_node element);

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

    /* include <attbeam> */
};

//----------------------------------------------------------------------------
// AttBeamRend
//----------------------------------------------------------------------------

class AttBeamRend : public Att {
public:
    AttBeamRend();
    virtual ~AttBeamRend();

    /** Reset the default values for the attribute class **/
    void ResetBeamRend();

    /** Read the values for the attribute class **/
    bool ReadBeamRend(pugi::xml_node element);

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

    /* include <attslope> */
};

//----------------------------------------------------------------------------
// AttBeamSecondary
//----------------------------------------------------------------------------

class AttBeamSecondary : public Att {
public:
    AttBeamSecondary();
    virtual ~AttBeamSecondary();

    /** Reset the default values for the attribute class **/
    void ResetBeamSecondary();

    /** Read the values for the attribute class **/
    bool ReadBeamSecondary(pugi::xml_node element);

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

    /* include <attbreaksec> */
};

//----------------------------------------------------------------------------
// AttBeamedWith
//----------------------------------------------------------------------------

class AttBeamedWith : public Att {
public:
    AttBeamedWith();
    virtual ~AttBeamedWith();

    /** Reset the default values for the attribute class **/
    void ResetBeamedWith();

    /** Read the values for the attribute class **/
    bool ReadBeamedWith(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteBeamedWith(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetBeamWith(data_OTHERSTAFF beamWith_) { m_beamWith = beamWith_; }
    data_OTHERSTAFF GetBeamWith() const { return m_beamWith; }
    bool HasBeamWith() const;
    ///@}

private:
    /**
     * In the case of cross-staff beams, the beam.with attribute is used to indicate
     * which staff the beam is connected to; that is, the staff above or the staff
     * below.
     **/
    data_OTHERSTAFF m_beamWith;

    /* include <attbeam.with> */
};

//----------------------------------------------------------------------------
// AttBeamingLog
//----------------------------------------------------------------------------

class AttBeamingLog : public Att {
public:
    AttBeamingLog();
    virtual ~AttBeamingLog();

    /** Reset the default values for the attribute class **/
    void ResetBeamingLog();

    /** Read the values for the attribute class **/
    bool ReadBeamingLog(pugi::xml_node element);

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

    /* include <attbeam.rests> */
};

//----------------------------------------------------------------------------
// AttBeatRptLog
//----------------------------------------------------------------------------

class AttBeatRptLog : public Att {
public:
    AttBeatRptLog();
    virtual ~AttBeatRptLog();

    /** Reset the default values for the attribute class **/
    void ResetBeatRptLog();

    /** Read the values for the attribute class **/
    bool ReadBeatRptLog(pugi::xml_node element);

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

    /* include <attbeatdef> */
};

//----------------------------------------------------------------------------
// AttBracketSpanLog
//----------------------------------------------------------------------------

class AttBracketSpanLog : public Att {
public:
    AttBracketSpanLog();
    virtual ~AttBracketSpanLog();

    /** Reset the default values for the attribute class **/
    void ResetBracketSpanLog();

    /** Read the values for the attribute class **/
    bool ReadBracketSpanLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteBracketSpanLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetFunc(std::string func_) { m_func = func_; }
    std::string GetFunc() const { return m_func; }
    bool HasFunc() const;
    ///@}

private:
    /** Describes the function of the bracketed event sequence. **/
    std::string m_func;

    /* include <attfunc> */
};

//----------------------------------------------------------------------------
// AttCutout
//----------------------------------------------------------------------------

class AttCutout : public Att {
public:
    AttCutout();
    virtual ~AttCutout();

    /** Reset the default values for the attribute class **/
    void ResetCutout();

    /** Read the values for the attribute class **/
    bool ReadCutout(pugi::xml_node element);

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

    /* include <attcutout> */
};

//----------------------------------------------------------------------------
// AttExpandable
//----------------------------------------------------------------------------

class AttExpandable : public Att {
public:
    AttExpandable();
    virtual ~AttExpandable();

    /** Reset the default values for the attribute class **/
    void ResetExpandable();

    /** Read the values for the attribute class **/
    bool ReadExpandable(pugi::xml_node element);

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

    /* include <attexpand> */
};

//----------------------------------------------------------------------------
// AttFTremLog
//----------------------------------------------------------------------------

class AttFTremLog : public Att {
public:
    AttFTremLog();
    virtual ~AttFTremLog();

    /** Reset the default values for the attribute class **/
    void ResetFTremLog();

    /** Read the values for the attribute class **/
    bool ReadFTremLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteFTremLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(fTremLog_FORM form_) { m_form = form_; }
    fTremLog_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    fTremLog_FORM m_form;

    /* include <attform> */
};

//----------------------------------------------------------------------------
// AttGlissPresent
//----------------------------------------------------------------------------

class AttGlissPresent : public Att {
public:
    AttGlissPresent();
    virtual ~AttGlissPresent();

    /** Reset the default values for the attribute class **/
    void ResetGlissPresent();

    /** Read the values for the attribute class **/
    bool ReadGlissPresent(pugi::xml_node element);

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
     * If visual information about the glissando needs to be recorded, then a
     **/
    data_GLISSANDO m_gliss;

    /* include <attgliss> */
};

//----------------------------------------------------------------------------
// AttGraceGrpLog
//----------------------------------------------------------------------------

class AttGraceGrpLog : public Att {
public:
    AttGraceGrpLog();
    virtual ~AttGraceGrpLog();

    /** Reset the default values for the attribute class **/
    void ResetGraceGrpLog();

    /** Read the values for the attribute class **/
    bool ReadGraceGrpLog(pugi::xml_node element);

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

    /* include <attattach> */
};

//----------------------------------------------------------------------------
// AttGraced
//----------------------------------------------------------------------------

class AttGraced : public Att {
public:
    AttGraced();
    virtual ~AttGraced();

    /** Reset the default values for the attribute class **/
    void ResetGraced();

    /** Read the values for the attribute class **/
    bool ReadGraced(pugi::xml_node element);

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

    /* include <attgrace.time> */
};

//----------------------------------------------------------------------------
// AttHairpinLog
//----------------------------------------------------------------------------

class AttHairpinLog : public Att {
public:
    AttHairpinLog();
    virtual ~AttHairpinLog();

    /** Reset the default values for the attribute class **/
    void ResetHairpinLog();

    /** Read the values for the attribute class **/
    bool ReadHairpinLog(pugi::xml_node element);

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

    /* include <attniente> */
};

//----------------------------------------------------------------------------
// AttHarpPedalLog
//----------------------------------------------------------------------------

class AttHarpPedalLog : public Att {
public:
    AttHarpPedalLog();
    virtual ~AttHarpPedalLog();

    /** Reset the default values for the attribute class **/
    void ResetHarpPedalLog();

    /** Read the values for the attribute class **/
    bool ReadHarpPedalLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteHarpPedalLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetC(harpPedalLog_C c_) { m_c = c_; }
    harpPedalLog_C GetC() const { return m_c; }
    bool HasC() const;
    //
    void SetD(harpPedalLog_D d_) { m_d = d_; }
    harpPedalLog_D GetD() const { return m_d; }
    bool HasD() const;
    //
    void SetE(harpPedalLog_E e_) { m_e = e_; }
    harpPedalLog_E GetE() const { return m_e; }
    bool HasE() const;
    //
    void SetF(harpPedalLog_F f_) { m_f = f_; }
    harpPedalLog_F GetF() const { return m_f; }
    bool HasF() const;
    //
    void SetG(harpPedalLog_G g_) { m_g = g_; }
    harpPedalLog_G GetG() const { return m_g; }
    bool HasG() const;
    //
    void SetA(harpPedalLog_A a_) { m_a = a_; }
    harpPedalLog_A GetA() const { return m_a; }
    bool HasA() const;
    //
    void SetB(harpPedalLog_B b_) { m_b = b_; }
    harpPedalLog_B GetB() const { return m_b; }
    bool HasB() const;
    ///@}

private:
    /** Indicates the pedal setting for the harp's C strings. **/
    harpPedalLog_C m_c;
    /** Indicates the pedal setting for the harp's D strings. **/
    harpPedalLog_D m_d;
    /** Indicates the pedal setting for the harp's E strings. **/
    harpPedalLog_E m_e;
    /** Indicates the pedal setting for the harp's F strings. **/
    harpPedalLog_F m_f;
    /** Indicates the pedal setting for the harp's G strings. **/
    harpPedalLog_G m_g;
    /** Indicates the pedal setting for the harp's A strings. **/
    harpPedalLog_A m_a;
    /** Indicates the pedal setting for the harp's B strings. **/
    harpPedalLog_B m_b;

    /* include <attb> */
};

//----------------------------------------------------------------------------
// AttLvPresent
//----------------------------------------------------------------------------

class AttLvPresent : public Att {
public:
    AttLvPresent();
    virtual ~AttLvPresent();

    /** Reset the default values for the attribute class **/
    void ResetLvPresent();

    /** Read the values for the attribute class **/
    bool ReadLvPresent(pugi::xml_node element);

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

    /* include <attlv> */
};

//----------------------------------------------------------------------------
// AttMeasureLog
//----------------------------------------------------------------------------

class AttMeasureLog : public Att {
public:
    AttMeasureLog();
    virtual ~AttMeasureLog();

    /** Reset the default values for the attribute class **/
    void ResetMeasureLog();

    /** Read the values for the attribute class **/
    bool ReadMeasureLog(pugi::xml_node element);

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

    /* include <attright> */
};

//----------------------------------------------------------------------------
// AttMeterSigGrpLog
//----------------------------------------------------------------------------

class AttMeterSigGrpLog : public Att {
public:
    AttMeterSigGrpLog();
    virtual ~AttMeterSigGrpLog();

    /** Reset the default values for the attribute class **/
    void ResetMeterSigGrpLog();

    /** Read the values for the attribute class **/
    bool ReadMeterSigGrpLog(pugi::xml_node element);

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

    /* include <attfunc> */
};

//----------------------------------------------------------------------------
// AttNumberPlacement
//----------------------------------------------------------------------------

class AttNumberPlacement : public Att {
public:
    AttNumberPlacement();
    virtual ~AttNumberPlacement();

    /** Reset the default values for the attribute class **/
    void ResetNumberPlacement();

    /** Read the values for the attribute class **/
    bool ReadNumberPlacement(pugi::xml_node element);

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

    /* include <attnum.visible> */
};

//----------------------------------------------------------------------------
// AttNumbered
//----------------------------------------------------------------------------

class AttNumbered : public Att {
public:
    AttNumbered();
    virtual ~AttNumbered();

    /** Reset the default values for the attribute class **/
    void ResetNumbered();

    /** Read the values for the attribute class **/
    bool ReadNumbered(pugi::xml_node element);

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

    /* include <attnum> */
};

//----------------------------------------------------------------------------
// AttOctaveLog
//----------------------------------------------------------------------------

class AttOctaveLog : public Att {
public:
    AttOctaveLog();
    virtual ~AttOctaveLog();

    /** Reset the default values for the attribute class **/
    void ResetOctaveLog();

    /** Read the values for the attribute class **/
    bool ReadOctaveLog(pugi::xml_node element);

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

    /* include <attcoll> */
};

//----------------------------------------------------------------------------
// AttPedalLog
//----------------------------------------------------------------------------

class AttPedalLog : public Att {
public:
    AttPedalLog();
    virtual ~AttPedalLog();

    /** Reset the default values for the attribute class **/
    void ResetPedalLog();

    /** Read the values for the attribute class **/
    bool ReadPedalLog(pugi::xml_node element);

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

    /* include <attfunc> */
};

//----------------------------------------------------------------------------
// AttPianoPedals
//----------------------------------------------------------------------------

class AttPianoPedals : public Att {
public:
    AttPianoPedals();
    virtual ~AttPianoPedals();

    /** Reset the default values for the attribute class **/
    void ResetPianoPedals();

    /** Read the values for the attribute class **/
    bool ReadPianoPedals(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePianoPedals(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPedalStyle(pianoPedals_PEDALSTYLE pedalStyle_) { m_pedalStyle = pedalStyle_; }
    pianoPedals_PEDALSTYLE GetPedalStyle() const { return m_pedalStyle; }
    bool HasPedalStyle() const;
    ///@}

private:
    /** Determines whether piano pedal marks should be rendered as lines or as terms. **/
    pianoPedals_PEDALSTYLE m_pedalStyle;

    /* include <attpedal.style> */
};

//----------------------------------------------------------------------------
// AttRehearsal
//----------------------------------------------------------------------------

class AttRehearsal : public Att {
public:
    AttRehearsal();
    virtual ~AttRehearsal();

    /** Reset the default values for the attribute class **/
    void ResetRehearsal();

    /** Read the values for the attribute class **/
    bool ReadRehearsal(pugi::xml_node element);

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

    /* include <attreh.enclose> */
};

//----------------------------------------------------------------------------
// AttScoreDefVisCmn
//----------------------------------------------------------------------------

class AttScoreDefVisCmn : public Att {
public:
    AttScoreDefVisCmn();
    virtual ~AttScoreDefVisCmn();

    /** Reset the default values for the attribute class **/
    void ResetScoreDefVisCmn();

    /** Read the values for the attribute class **/
    bool ReadScoreDefVisCmn(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteScoreDefVisCmn(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetGridShow(data_BOOLEAN gridShow_) { m_gridShow = gridShow_; }
    data_BOOLEAN GetGridShow() const { return m_gridShow; }
    bool HasGridShow() const;
    ///@}

private:
    /** Determines whether to display guitar chord grids. **/
    data_BOOLEAN m_gridShow;

    /* include <attgrid.show> */
};

//----------------------------------------------------------------------------
// AttSlurRend
//----------------------------------------------------------------------------

class AttSlurRend : public Att {
public:
    AttSlurRend();
    virtual ~AttSlurRend();

    /** Reset the default values for the attribute class **/
    void ResetSlurRend();

    /** Read the values for the attribute class **/
    bool ReadSlurRend(pugi::xml_node element);

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

    /* include <attslur.lwidth> */
};

//----------------------------------------------------------------------------
// AttStemsCmn
//----------------------------------------------------------------------------

class AttStemsCmn : public Att {
public:
    AttStemsCmn();
    virtual ~AttStemsCmn();

    /** Reset the default values for the attribute class **/
    void ResetStemsCmn();

    /** Read the values for the attribute class **/
    bool ReadStemsCmn(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStemsCmn(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetStemWith(data_OTHERSTAFF stemWith_) { m_stemWith = stemWith_; }
    data_OTHERSTAFF GetStemWith() const { return m_stemWith; }
    bool HasStemWith() const;
    ///@}

private:
    /**
     * Contains an indication of which staff a note or chord that logically belongs to
     * the current staff should be visually placed on; that is, the one above or the
     * one below.
     **/
    data_OTHERSTAFF m_stemWith;

    /* include <attstem.with> */
};

//----------------------------------------------------------------------------
// AttTieRend
//----------------------------------------------------------------------------

class AttTieRend : public Att {
public:
    AttTieRend();
    virtual ~AttTieRend();

    /** Reset the default values for the attribute class **/
    void ResetTieRend();

    /** Read the values for the attribute class **/
    bool ReadTieRend(pugi::xml_node element);

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

    /* include <atttie.lwidth> */
};

//----------------------------------------------------------------------------
// AttTremMeasured
//----------------------------------------------------------------------------

class AttTremMeasured : public Att {
public:
    AttTremMeasured();
    virtual ~AttTremMeasured();

    /** Reset the default values for the attribute class **/
    void ResetTremMeasured();

    /** Read the values for the attribute class **/
    bool ReadTremMeasured(pugi::xml_node element);

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

    /* include <attunitdur> */
};

} // vrv namespace

#endif // __VRV_ATTS_CMN_H__
