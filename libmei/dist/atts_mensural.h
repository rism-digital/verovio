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

#ifndef __LIBMEI_ATTS_MENSURAL_H__
#define __LIBMEI_ATTS_MENSURAL_H__

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
protected:
    AttDurationQuality();
    ~AttDurationQuality() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetDurationQuality();

    /** Read the values for the attribute class **/
    bool ReadDurationQuality(pugi::xml_node element, bool removeAttr = true);

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
};

//----------------------------------------------------------------------------
// InstDurationQuality
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttDurationQuality
 */

class InstDurationQuality : public AttDurationQuality {
public:
    InstDurationQuality() = default;
    virtual ~InstDurationQuality() = default;
};

//----------------------------------------------------------------------------
// AttMensuralLog
//----------------------------------------------------------------------------

class AttMensuralLog : public Att {
protected:
    AttMensuralLog();
    ~AttMensuralLog() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetMensuralLog();

    /** Read the values for the attribute class **/
    bool ReadMensuralLog(pugi::xml_node element, bool removeAttr = true);

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
};

//----------------------------------------------------------------------------
// InstMensuralLog
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttMensuralLog
 */

class InstMensuralLog : public AttMensuralLog {
public:
    InstMensuralLog() = default;
    virtual ~InstMensuralLog() = default;
};

//----------------------------------------------------------------------------
// AttMensuralShared
//----------------------------------------------------------------------------

class AttMensuralShared : public Att {
protected:
    AttMensuralShared();
    ~AttMensuralShared() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetMensuralShared();

    /** Read the values for the attribute class **/
    bool ReadMensuralShared(pugi::xml_node element, bool removeAttr = true);

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
};

//----------------------------------------------------------------------------
// InstMensuralShared
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttMensuralShared
 */

class InstMensuralShared : public AttMensuralShared {
public:
    InstMensuralShared() = default;
    virtual ~InstMensuralShared() = default;
};

//----------------------------------------------------------------------------
// AttNoteVisMensural
//----------------------------------------------------------------------------

class AttNoteVisMensural : public Att {
protected:
    AttNoteVisMensural();
    ~AttNoteVisMensural() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetNoteVisMensural();

    /** Read the values for the attribute class **/
    bool ReadNoteVisMensural(pugi::xml_node element, bool removeAttr = true);

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
    /** Indicates this element’s participation in a ligature. **/
    data_LIGATUREFORM m_lig;
};

//----------------------------------------------------------------------------
// InstNoteVisMensural
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttNoteVisMensural
 */

class InstNoteVisMensural : public AttNoteVisMensural {
public:
    InstNoteVisMensural() = default;
    virtual ~InstNoteVisMensural() = default;
};

//----------------------------------------------------------------------------
// AttRestVisMensural
//----------------------------------------------------------------------------

class AttRestVisMensural : public Att {
protected:
    AttRestVisMensural();
    ~AttRestVisMensural() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetRestVisMensural();

    /** Read the values for the attribute class **/
    bool ReadRestVisMensural(pugi::xml_node element, bool removeAttr = true);

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
};

//----------------------------------------------------------------------------
// InstRestVisMensural
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttRestVisMensural
 */

class InstRestVisMensural : public AttRestVisMensural {
public:
    InstRestVisMensural() = default;
    virtual ~InstRestVisMensural() = default;
};

//----------------------------------------------------------------------------
// AttStemsMensural
//----------------------------------------------------------------------------

class AttStemsMensural : public Att {
protected:
    AttStemsMensural();
    ~AttStemsMensural() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetStemsMensural();

    /** Read the values for the attribute class **/
    bool ReadStemsMensural(pugi::xml_node element, bool removeAttr = true);

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
};

//----------------------------------------------------------------------------
// InstStemsMensural
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttStemsMensural
 */

class InstStemsMensural : public AttStemsMensural {
public:
    InstStemsMensural() = default;
    virtual ~InstStemsMensural() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_MENSURAL_H__
