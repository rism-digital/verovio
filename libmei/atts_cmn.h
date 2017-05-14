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
    /** Records the appearance and usually the function of the bar line. **/
    bTremLog_FORM m_form;

    /* include <attform> */
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
    void SetBeatdef(data_DURATION beatdef_) { m_beatdef = beatdef_; }
    data_DURATION GetBeatdef() const { return m_beatdef; }
    bool HasBeatdef() const;
    ///@}

private:
    /** Indicates the performed duration represented by the beatRpt symbol. **/
    data_DURATION m_beatdef;

    /* include <attbeatdef> */
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
    /** Records the appearance and usually the function of the bar line. **/
    fTremLog_FORM m_form;

    /* include <attform> */
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
    /** Records the appearance and usually the function of the bar line. **/
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
    /** Records the function of an accidental. **/
    meterSigGrpLog_FUNC m_func;

    /* include <attfunc> */
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
    /**
     * Along with numbase, describes duration as a ratio.
     * num is the first value in the ratio, while numbase is the second.
     **/
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
    /** Records the function of an accidental. **/
    std::string m_func;

    /* include <attfunc> */
};

} // vrv namespace

#endif // __VRV_ATTS_CMN_H__
