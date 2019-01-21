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
// AttLigatureLog
//----------------------------------------------------------------------------

class AttLigatureLog : public Att {
public:
    AttLigatureLog();
    virtual ~AttLigatureLog();

    /** Reset the default values for the attribute class **/
    void ResetLigatureLog();

    /** Read the values for the attribute class **/
    bool ReadLigatureLog(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteLigatureLog(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(data_LIGATUREFORM form_) { m_form = form_; }
    data_LIGATUREFORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    data_LIGATUREFORM m_form;

    /* include <attform> */
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
    void SetMensurDot(data_BOOLEAN mensurDot_) { m_mensurDot = mensurDot_; }
    data_BOOLEAN GetMensurDot() const { return m_mensurDot; }
    bool HasMensurDot() const;
    //
    void SetMensurSign(data_MENSURATIONSIGN mensurSign_) { m_mensurSign = mensurSign_; }
    data_MENSURATIONSIGN GetMensurSign() const { return m_mensurSign; }
    bool HasMensurSign() const;
    //
    void SetMensurSlash(int mensurSlash_) { m_mensurSlash = mensurSlash_; }
    int GetMensurSlash() const { return m_mensurSlash; }
    bool HasMensurSlash() const;
    //
    void SetProportNum(int proportNum_) { m_proportNum = proportNum_; }
    int GetProportNum() const { return m_proportNum; }
    bool HasProportNum() const;
    //
    void SetProportNumbase(int proportNumbase_) { m_proportNumbase = proportNumbase_; }
    int GetProportNumbase() const { return m_proportNumbase; }
    bool HasProportNumbase() const;
    ///@}

private:
    /** Determines if a dot is to be added to the base symbol. **/
    data_BOOLEAN m_mensurDot;
    /** The base symbol in the mensuration sign/time signature of mensural notation. **/
    data_MENSURATIONSIGN m_mensurSign;
    /**
     * Indicates the number lines added to the mensuration sign.
     * For example, one slash is added for what we now call 'alla breve'.
     **/
    int m_mensurSlash;
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

    /* include <atttempus> */
};

//----------------------------------------------------------------------------
// AttNoteAnlMensural
//----------------------------------------------------------------------------

class AttNoteAnlMensural : public Att {
public:
    AttNoteAnlMensural();
    virtual ~AttNoteAnlMensural();

    /** Reset the default values for the attribute class **/
    void ResetNoteAnlMensural();

    /** Read the values for the attribute class **/
    bool ReadNoteAnlMensural(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNoteAnlMensural(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLig(noteAnlMensural_LIG lig_) { m_lig = lig_; }
    noteAnlMensural_LIG GetLig() const { return m_lig; }
    bool HasLig() const;
    ///@}

private:
    /** Indicates this element's participation in a ligature. **/
    noteAnlMensural_LIG m_lig;

    /* include <attlig> */
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

} // vrv namespace

#endif // __VRV_ATTS_MENSURAL_H__
