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
    /** Records the appearance and usually the function of the bar line. **/
    data_LIGATUREFORM m_form;

    /* include <attform> */
};

//----------------------------------------------------------------------------
// AttMensurVis
//----------------------------------------------------------------------------

class AttMensurVis : public Att {
public:
    AttMensurVis();
    virtual ~AttMensurVis();

    /** Reset the default values for the attribute class **/
    void ResetMensurVis();

    /** Read the values for the attribute class **/
    bool ReadMensurVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMensurVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(mensurVis_FORM form_) { m_form = form_; }
    mensurVis_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    //
    void SetOrient(data_ORIENTATION orient_) { m_orient = orient_; }
    data_ORIENTATION GetOrient() const { return m_orient; }
    bool HasOrient() const;
    ///@}

private:
    /** Records the appearance and usually the function of the bar line. **/
    mensurVis_FORM m_form;
    /** Describes the rotation or reflection of the base symbol. **/
    data_ORIENTATION m_orient;

    /* include <attorient> */
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
// AttMensuralVis
//----------------------------------------------------------------------------

class AttMensuralVis : public Att {
public:
    AttMensuralVis();
    virtual ~AttMensuralVis();

    /** Reset the default values for the attribute class **/
    void ResetMensuralVis();

    /** Read the values for the attribute class **/
    bool ReadMensuralVis(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteMensuralVis(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMensurColor(std::string mensurColor_) { m_mensurColor = mensurColor_; }
    std::string GetMensurColor() const { return m_mensurColor; }
    bool HasMensurColor() const;
    //
    void SetMensurForm(mensuralVis_MENSURFORM mensurForm_) { m_mensurForm = mensurForm_; }
    mensuralVis_MENSURFORM GetMensurForm() const { return m_mensurForm; }
    bool HasMensurForm() const;
    //
    void SetMensurLoc(int mensurLoc_) { m_mensurLoc = mensurLoc_; }
    int GetMensurLoc() const { return m_mensurLoc; }
    bool HasMensurLoc() const;
    //
    void SetMensurOrient(data_ORIENTATION mensurOrient_) { m_mensurOrient = mensurOrient_; }
    data_ORIENTATION GetMensurOrient() const { return m_mensurOrient; }
    bool HasMensurOrient() const;
    //
    void SetMensurSize(data_SIZE mensurSize_) { m_mensurSize = mensurSize_; }
    data_SIZE GetMensurSize() const { return m_mensurSize; }
    bool HasMensurSize() const;
    ///@}

private:
    /**
     * Records the color of the mensuration sign.
     * Do not confuse this with the musical term 'color' as used in pre-CMN notation.
     **/
    std::string m_mensurColor;
    /** Indicates whether the base symbol is written vertically or horizontally. **/
    mensuralVis_MENSURFORM m_mensurForm;
    /** Holds the staff location of the mensuration sign. **/
    int m_mensurLoc;
    /** Describes the rotation or reflection of the base symbol. **/
    data_ORIENTATION m_mensurOrient;
    /** Describes the relative size of the mensuration sign. **/
    data_SIZE m_mensurSize;

    /* include <attmensur.size> */
};

//----------------------------------------------------------------------------
// AttNoteLogMensural
//----------------------------------------------------------------------------

class AttNoteLogMensural : public Att {
public:
    AttNoteLogMensural();
    virtual ~AttNoteLogMensural();

    /** Reset the default values for the attribute class **/
    void ResetNoteLogMensural();

    /** Read the values for the attribute class **/
    bool ReadNoteLogMensural(pugi::xml_node element);

    /** Write the values for the attribute class **/
    bool WriteNoteLogMensural(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetLig(noteLogMensural_LIG lig_) { m_lig = lig_; }
    noteLogMensural_LIG GetLig() const { return m_lig; }
    bool HasLig() const;
    ///@}

private:
    /** Indicates this element's participation in a ligature. **/
    noteLogMensural_LIG m_lig;

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
