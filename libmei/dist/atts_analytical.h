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

#ifndef __LIBMEI_ATTS_ANALYTICAL_H__
#define __LIBMEI_ATTS_ANALYTICAL_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

#include <string>

namespace vrv {

//----------------------------------------------------------------------------
// AttHarmAnl
//----------------------------------------------------------------------------

class AttHarmAnl : public Att {
protected:
    AttHarmAnl();
    ~AttHarmAnl() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetHarmAnl();

    /** Read the values for the attribute class **/
    bool ReadHarmAnl(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteHarmAnl(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetForm(harmAnl_FORM form_) { m_form = form_; }
    harmAnl_FORM GetForm() const { return m_form; }
    bool HasForm() const;
    ///@}

private:
    /** Indicates to what degree the harmonic label is supported by the notation. **/
    harmAnl_FORM m_form;
};

//----------------------------------------------------------------------------
// InstHarmAnl
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttHarmAnl
 */

class InstHarmAnl : public AttHarmAnl {
public:
    InstHarmAnl() = default;
    virtual ~InstHarmAnl() = default;
};

//----------------------------------------------------------------------------
// AttHarmonicFunction
//----------------------------------------------------------------------------

class AttHarmonicFunction : public Att {
protected:
    AttHarmonicFunction();
    ~AttHarmonicFunction() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetHarmonicFunction();

    /** Read the values for the attribute class **/
    bool ReadHarmonicFunction(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteHarmonicFunction(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetDeg(std::string deg_) { m_deg = deg_; }
    std::string GetDeg() const { return m_deg; }
    bool HasDeg() const;
    ///@}

private:
    /**
     * Captures scale degree information using Humdrum **deg syntax -- an optional
     * indicator of melodic approach (^ = ascending approach, v = descending approach),
     * a scale degree value (1 = tonic ...
     * 7 = leading tone), and an optional indication of chromatic alteration, 1, v7,
     * ^1, or v5+, for example. The amount of chromatic alternation is not indicated.
     **/
    std::string m_deg;
};

//----------------------------------------------------------------------------
// InstHarmonicFunction
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttHarmonicFunction
 */

class InstHarmonicFunction : public AttHarmonicFunction {
public:
    InstHarmonicFunction() = default;
    virtual ~InstHarmonicFunction() = default;
};

//----------------------------------------------------------------------------
// AttIntervalHarmonic
//----------------------------------------------------------------------------

class AttIntervalHarmonic : public Att {
protected:
    AttIntervalHarmonic();
    ~AttIntervalHarmonic() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetIntervalHarmonic();

    /** Read the values for the attribute class **/
    bool ReadIntervalHarmonic(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteIntervalHarmonic(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetInth(std::string inth_) { m_inth = inth_; }
    std::string GetInth() const { return m_inth; }
    bool HasInth() const;
    ///@}

private:
    /** Encodes the harmonic interval between pitches occurring at the same time. **/
    std::string m_inth;
};

//----------------------------------------------------------------------------
// InstIntervalHarmonic
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttIntervalHarmonic
 */

class InstIntervalHarmonic : public AttIntervalHarmonic {
public:
    InstIntervalHarmonic() = default;
    virtual ~InstIntervalHarmonic() = default;
};

//----------------------------------------------------------------------------
// AttIntervalMelodic
//----------------------------------------------------------------------------

class AttIntervalMelodic : public Att {
protected:
    AttIntervalMelodic();
    ~AttIntervalMelodic() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetIntervalMelodic();

    /** Read the values for the attribute class **/
    bool ReadIntervalMelodic(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteIntervalMelodic(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetIntm(std::string intm_) { m_intm = intm_; }
    std::string GetIntm() const { return m_intm; }
    bool HasIntm() const;
    ///@}

private:
    /**
     * Encodes the melodic interval from the previous pitch.
     * The value may be a general directional indication (u, d, s, etc.), an indication
     * of diatonic interval direction, quality, and size, or a precise numeric value in
     * half steps.
     **/
    std::string m_intm;
};

//----------------------------------------------------------------------------
// InstIntervalMelodic
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttIntervalMelodic
 */

class InstIntervalMelodic : public AttIntervalMelodic {
public:
    InstIntervalMelodic() = default;
    virtual ~InstIntervalMelodic() = default;
};

//----------------------------------------------------------------------------
// AttKeySigDefaultAnl
//----------------------------------------------------------------------------

class AttKeySigDefaultAnl : public Att {
protected:
    AttKeySigDefaultAnl();
    ~AttKeySigDefaultAnl() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetKeySigDefaultAnl();

    /** Read the values for the attribute class **/
    bool ReadKeySigDefaultAnl(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteKeySigDefaultAnl(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetKeyAccid(data_ACCIDENTAL_GESTURAL keyAccid_) { m_keyAccid = keyAccid_; }
    data_ACCIDENTAL_GESTURAL GetKeyAccid() const { return m_keyAccid; }
    bool HasKeyAccid() const;
    //
    void SetKeyMode(data_MODE keyMode_) { m_keyMode = keyMode_; }
    data_MODE GetKeyMode() const { return m_keyMode; }
    bool HasKeyMode() const;
    //
    void SetKeyPname(data_PITCHNAME keyPname_) { m_keyPname = keyPname_; }
    data_PITCHNAME GetKeyPname() const { return m_keyPname; }
    bool HasKeyPname() const;
    ///@}

private:
    /**
     * Contains an accidental for the tonic key, if one is required, e.g., if key.pname
     * equals c and key.accid equals s, then a tonic of C# is indicated.
     **/
    data_ACCIDENTAL_GESTURAL m_keyAccid;
    /** Indicates major, minor, or other tonality. **/
    data_MODE m_keyMode;
    /** Holds the pitch name of the tonic key, e.g., c for the key of C. **/
    data_PITCHNAME m_keyPname;
};

//----------------------------------------------------------------------------
// InstKeySigDefaultAnl
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttKeySigDefaultAnl
 */

class InstKeySigDefaultAnl : public AttKeySigDefaultAnl {
public:
    InstKeySigDefaultAnl() = default;
    virtual ~InstKeySigDefaultAnl() = default;
};

//----------------------------------------------------------------------------
// AttMelodicFunction
//----------------------------------------------------------------------------

class AttMelodicFunction : public Att {
protected:
    AttMelodicFunction();
    ~AttMelodicFunction() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetMelodicFunction();

    /** Read the values for the attribute class **/
    bool ReadMelodicFunction(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteMelodicFunction(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetMfunc(data_MELODICFUNCTION mfunc_) { m_mfunc = mfunc_; }
    data_MELODICFUNCTION GetMfunc() const { return m_mfunc; }
    bool HasMfunc() const;
    ///@}

private:
    /** Describes melodic function using Humdrum **embel syntax. **/
    data_MELODICFUNCTION m_mfunc;
};

//----------------------------------------------------------------------------
// InstMelodicFunction
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttMelodicFunction
 */

class InstMelodicFunction : public AttMelodicFunction {
public:
    InstMelodicFunction() = default;
    virtual ~InstMelodicFunction() = default;
};

//----------------------------------------------------------------------------
// AttPitchClass
//----------------------------------------------------------------------------

class AttPitchClass : public Att {
protected:
    AttPitchClass();
    ~AttPitchClass() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetPitchClass();

    /** Read the values for the attribute class **/
    bool ReadPitchClass(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WritePitchClass(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPclass(int pclass_) { m_pclass = pclass_; }
    int GetPclass() const { return m_pclass; }
    bool HasPclass() const;
    ///@}

private:
    /** Holds pitch class information. **/
    int m_pclass;
};

//----------------------------------------------------------------------------
// InstPitchClass
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttPitchClass
 */

class InstPitchClass : public AttPitchClass {
public:
    InstPitchClass() = default;
    virtual ~InstPitchClass() = default;
};

//----------------------------------------------------------------------------
// AttSolfa
//----------------------------------------------------------------------------

class AttSolfa : public Att {
protected:
    AttSolfa();
    ~AttSolfa() = default;

public:
    /** Reset the default values for the attribute class **/
    void ResetSolfa();

    /** Read the values for the attribute class **/
    bool ReadSolfa(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool WriteSolfa(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{
    void SetPsolfa(std::string psolfa_) { m_psolfa = psolfa_; }
    std::string GetPsolfa() const { return m_psolfa; }
    bool HasPsolfa() const;
    ///@}

private:
    /**
     * Contains sol-fa designation, e.g., do, re, mi, etc., in either a fixed or
     * movable Do system.
     **/
    std::string m_psolfa;
};

//----------------------------------------------------------------------------
// InstSolfa
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttSolfa
 */

class InstSolfa : public AttSolfa {
public:
    InstSolfa() = default;
    virtual ~InstSolfa() = default;
};

} // namespace vrv

#endif // __LIBMEI_ATTS_ANALYTICAL_H__
