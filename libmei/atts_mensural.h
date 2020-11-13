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

#ifndef __VRV_ATTS_MENSURAL_H__
#define __VRV_ATTS_MENSURAL_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttDurationQuality
//----------------------------------------------------------------------------

class AttDurationQuality : public Att {
public:
    AttDurationQuality();
    virtual ~AttDurationQuality();

    /** Reset the default values for the attribute class **/
    void ResetDurationQuality();

    /** Read the values for the attribute class **/
    bool ReadDurationQuality(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteDurationQuality(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDurQuality(data_DURQUALITY_mensural durQuality_) { m_durQuality = durQuality_; }
    data_DURQUALITY_mensural GetDurQuality() const { return m_durQuality; }
    bool HasDurQuality() const;
    ///@}

private:
    /**
     * Encodes the durational quality of a mensural note using the values provided by
     * the data.DURQUALITY.mensural datatype (i.e., the perfect / imperfect / altered /
     * major / minor / duplex quality of a note).
     **/
    data_DURQUALITY_mensural m_durQuality;

    /* include <attdur.quality> */
};

//----------------------------------------------------------------------------
// AttMensuralLog
//----------------------------------------------------------------------------

class AttMensuralLog : public Att {
public:
    AttMensuralLog();
    virtual ~AttMensuralLog();

    /** Reset the default values for the attribute class **/
    void ResetMensuralLog();

    /** Read the values for the attribute class **/
    bool ReadMensuralLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMensuralLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetProportNum(int proportNum_) { m_proportNum = proportNum_; }
    int GetProportNum() const { return m_proportNum; }
    bool HasProportNum() const;
    //
    void SetProportNumbase(int proportNumbase_) { m_proportNumbase = proportNumbase_; }
    int GetProportNumbase() const { return m_proportNumbase; }
    bool HasProportNumbase() const;
    ///@}

private:
    /**
     * Together, proport.num and proport.numbase specify a proportional change as a
     * ratio, e.g., 1:3.
     * Proport.num is for the first value in the ratio.
     **/
    int m_proportNum;
    /**
     * Together, proport.num and proport.numbase specify a proportional change as a
     * ratio, e.g., 1:3.
     * Proport.numbase is for the second value in the ratio.
     **/
    int m_proportNumbase;

    /* include <attproport.numbase> */
};

//----------------------------------------------------------------------------
// AttMensuralShared
//----------------------------------------------------------------------------

class AttMensuralShared : public Att {
public:
    AttMensuralShared();
    virtual ~AttMensuralShared();

    /** Reset the default values for the attribute class **/
    void ResetMensuralShared();

    /** Read the values for the attribute class **/
    bool ReadMensuralShared(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMensuralShared(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetModusmaior(data_MODUSMAIOR modusmaior_) { m_modusmaior = modusmaior_; }
    data_MODUSMAIOR GetModusmaior() const { return m_modusmaior; }
    bool HasModusmaior() const;
    //
    void SetModusminor(data_MODUSMINOR modusminor_) { m_modusminor = modusminor_; }
    data_MODUSMINOR GetModusminor() const { return m_modusminor; }
    bool HasModusminor() const;
    //
    void SetProlatio(data_PROLATIO prolatio_) { m_prolatio = prolatio_; }
    data_PROLATIO GetProlatio() const { return m_prolatio; }
    bool HasProlatio() const;
    //
    void SetTempus(data_TEMPUS tempus_) { m_tempus = tempus_; }
    data_TEMPUS GetTempus() const { return m_tempus; }
    bool HasTempus() const;
    //
    void SetDivisio(data_DIVISIO divisio_) { m_divisio = divisio_; }
    data_DIVISIO GetDivisio() const { return m_divisio; }
    bool HasDivisio() const;
    ///@}

private:
    /** Describes the maxima-long relationship. **/
    data_MODUSMAIOR m_modusmaior;
    /** Describes the long-breve relationship. **/
    data_MODUSMINOR m_modusminor;
    /** Describes the semibreve-minim relationship. **/
    data_PROLATIO m_prolatio;
    /** Describes the breve-semibreve relationship. **/
    data_TEMPUS m_tempus;
    /** Describes the divisions of the breve in use in 14th-century Italy. **/
    data_DIVISIO m_divisio;

    /* include <attdivisio> */
};

//----------------------------------------------------------------------------
// AttNoteVisMensural
//----------------------------------------------------------------------------

class AttNoteVisMensural : public Att {
public:
    AttNoteVisMensural();
    virtual ~AttNoteVisMensural();

    /** Reset the default values for the attribute class **/
    void ResetNoteVisMensural();

    /** Read the values for the attribute class **/
    bool ReadNoteVisMensural(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNoteVisMensural(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLig(data_LIGATUREFORM lig_) { m_lig = lig_; }
    data_LIGATUREFORM GetLig() const { return m_lig; }
    bool HasLig() const;
    ///@}

private:
    /** Indicates this element's participation in a ligature. **/
    data_LIGATUREFORM m_lig;

    /* include <attlig> */
};

//----------------------------------------------------------------------------
// AttPlicaVis
//----------------------------------------------------------------------------

class AttPlicaVis : public Att {
public:
    AttPlicaVis();
    virtual ~AttPlicaVis();

    /** Reset the default values for the attribute class **/
    void ResetPlicaVis();

    /** Read the values for the attribute class **/
    bool ReadPlicaVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WritePlicaVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDir(data_STEMDIRECTION_basic dir_) { m_dir = dir_; }
    data_STEMDIRECTION_basic GetDir() const { return m_dir; }
    bool HasDir() const;
    //
    void SetLen(double len_) { m_len = len_; }
    double GetLen() const { return m_len; }
    bool HasLen() const;
    ///@}

private:
    /** Records the position of the piano damper pedal. **/
    data_STEMDIRECTION_basic m_dir;
    /** Encodes the stem length. **/
    double m_len;

    /* include <attlen> */
};

//----------------------------------------------------------------------------
// AttRestVisMensural
//----------------------------------------------------------------------------

class AttRestVisMensural : public Att {
public:
    AttRestVisMensural();
    virtual ~AttRestVisMensural();

    /** Reset the default values for the attribute class **/
    void ResetRestVisMensural();

    /** Read the values for the attribute class **/
    bool ReadRestVisMensural(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteRestVisMensural(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetSpaces(int spaces_) { m_spaces = spaces_; }
    int GetSpaces() const { return m_spaces; }
    bool HasSpaces() const;
    ///@}

private:
    /** States how many spaces are covered by the rest. **/
    int m_spaces;

    /* include <attspaces> */
};

//----------------------------------------------------------------------------
// AttStemVis
//----------------------------------------------------------------------------

class AttStemVis : public Att {
public:
    AttStemVis();
    virtual ~AttStemVis();

    /** Reset the default values for the attribute class **/
    void ResetStemVis();

    /** Read the values for the attribute class **/
    bool ReadStemVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStemVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPos(data_STEMPOSITION pos_) { m_pos = pos_; }
    data_STEMPOSITION GetPos() const { return m_pos; }
    bool HasPos() const;
    //
    void SetLen(double len_) { m_len = len_; }
    double GetLen() const { return m_len; }
    bool HasLen() const;
    //
    void SetForm(data_STEMFORM_mensural form_) { m_form = form_; }
    data_STEMFORM_mensural GetForm() const { return m_form; }
    bool HasForm() const;
    //
    void SetDir(data_STEMDIRECTION dir_) { m_dir = dir_; }
    data_STEMDIRECTION GetDir() const { return m_dir; }
    bool HasDir() const;
    //
    void SetFlagPos(data_FLAGPOS_mensural flagPos_) { m_flagPos = flagPos_; }
    data_FLAGPOS_mensural GetFlagPos() const { return m_flagPos; }
    bool HasFlagPos() const;
    //
    void SetFlagForm(data_FLAGFORM_mensural flagForm_) { m_flagForm = flagForm_; }
    data_FLAGFORM_mensural GetFlagForm() const { return m_flagForm; }
    bool HasFlagForm() const;
    ///@}

private:
    /** Records the position of the stem in relation to the note head(s). **/
    data_STEMPOSITION m_pos;
    /** Encodes the stem length. **/
    double m_len;
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    data_STEMFORM_mensural m_form;
    /** Records the position of the piano damper pedal. **/
    data_STEMDIRECTION m_dir;
    /**
     * Records the position of the flag using the values provided by the
     * data.FLAGPOS.mensural datatype.
     **/
    data_FLAGPOS_mensural m_flagPos;
    /**
     * Encodes the form of the flag using the values provided by the
     * data.FLAGFORM.mensural datatype.
     **/
    data_FLAGFORM_mensural m_flagForm;

    /* include <attflag.form> */
};

//----------------------------------------------------------------------------
// AttStemsMensural
//----------------------------------------------------------------------------

class AttStemsMensural : public Att {
public:
    AttStemsMensural();
    virtual ~AttStemsMensural();

    /** Reset the default values for the attribute class **/
    void ResetStemsMensural();

    /** Read the values for the attribute class **/
    bool ReadStemsMensural(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteStemsMensural(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetStemForm(data_STEMFORM_mensural stemForm_) { m_stemForm = stemForm_; }
    data_STEMFORM_mensural GetStemForm() const { return m_stemForm; }
    bool HasStemForm() const;
    ///@}

private:
    /** Records the form of the stem. **/
    data_STEMFORM_mensural m_stemForm;

    /* include <attstem.form> */
};

} // vrv namespace

#endif // __VRV_ATTS_MENSURAL_H__
