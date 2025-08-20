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

#include "atts_analytical.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttHarmAnl
//----------------------------------------------------------------------------

AttHarmAnl::AttHarmAnl() : Att()
{
    this->ResetHarmAnl();
}

void AttHarmAnl::ResetHarmAnl()
{
    m_form = harmAnl_FORM_NONE;
}

bool AttHarmAnl::ReadHarmAnl(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToHarmAnlForm(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarmAnl::WriteHarmAnl(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = HarmAnlFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarmAnl::HasForm() const
{
    return (m_form != harmAnl_FORM_NONE);
}

//----------------------------------------------------------------------------
// AttHarmonicFunction
//----------------------------------------------------------------------------

AttHarmonicFunction::AttHarmonicFunction() : Att()
{
    this->ResetHarmonicFunction();
}

void AttHarmonicFunction::ResetHarmonicFunction()
{
    m_deg = "";
}

bool AttHarmonicFunction::ReadHarmonicFunction(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("deg")) {
        this->SetDeg(StrToStr(element.attribute("deg").value()));
        if (removeAttr) element.remove_attribute("deg");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarmonicFunction::WriteHarmonicFunction(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDeg()) {
        element.append_attribute("deg") = StrToStr(this->GetDeg()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarmonicFunction::HasDeg() const
{
    return (m_deg != "");
}

//----------------------------------------------------------------------------
// AttIntervalHarmonic
//----------------------------------------------------------------------------

AttIntervalHarmonic::AttIntervalHarmonic() : Att()
{
    this->ResetIntervalHarmonic();
}

void AttIntervalHarmonic::ResetIntervalHarmonic()
{
    m_inth = "";
}

bool AttIntervalHarmonic::ReadIntervalHarmonic(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("inth")) {
        this->SetInth(StrToStr(element.attribute("inth").value()));
        if (removeAttr) element.remove_attribute("inth");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttIntervalHarmonic::WriteIntervalHarmonic(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasInth()) {
        element.append_attribute("inth") = StrToStr(this->GetInth()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttIntervalHarmonic::HasInth() const
{
    return (m_inth != "");
}

//----------------------------------------------------------------------------
// AttIntervalMelodic
//----------------------------------------------------------------------------

AttIntervalMelodic::AttIntervalMelodic() : Att()
{
    this->ResetIntervalMelodic();
}

void AttIntervalMelodic::ResetIntervalMelodic()
{
    m_intm = "";
}

bool AttIntervalMelodic::ReadIntervalMelodic(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("intm")) {
        this->SetIntm(StrToStr(element.attribute("intm").value()));
        if (removeAttr) element.remove_attribute("intm");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttIntervalMelodic::WriteIntervalMelodic(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasIntm()) {
        element.append_attribute("intm") = StrToStr(this->GetIntm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttIntervalMelodic::HasIntm() const
{
    return (m_intm != "");
}

//----------------------------------------------------------------------------
// AttKeySigDefaultAnl
//----------------------------------------------------------------------------

AttKeySigDefaultAnl::AttKeySigDefaultAnl() : Att()
{
    this->ResetKeySigDefaultAnl();
}

void AttKeySigDefaultAnl::ResetKeySigDefaultAnl()
{
    m_keyAccid = ACCIDENTAL_GESTURAL_NONE;
    m_keyMode = MODE_NONE;
    m_keyPname = PITCHNAME_NONE;
}

bool AttKeySigDefaultAnl::ReadKeySigDefaultAnl(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("key.accid")) {
        this->SetKeyAccid(StrToAccidentalGestural(element.attribute("key.accid").value()));
        if (removeAttr) element.remove_attribute("key.accid");
        hasAttribute = true;
    }
    if (element.attribute("key.mode")) {
        this->SetKeyMode(StrToMode(element.attribute("key.mode").value()));
        if (removeAttr) element.remove_attribute("key.mode");
        hasAttribute = true;
    }
    if (element.attribute("key.pname")) {
        this->SetKeyPname(StrToPitchname(element.attribute("key.pname").value()));
        if (removeAttr) element.remove_attribute("key.pname");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigDefaultAnl::WriteKeySigDefaultAnl(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasKeyAccid()) {
        element.append_attribute("key.accid") = AccidentalGesturalToStr(this->GetKeyAccid()).c_str();
        wroteAttribute = true;
    }
    if (this->HasKeyMode()) {
        element.append_attribute("key.mode") = ModeToStr(this->GetKeyMode()).c_str();
        wroteAttribute = true;
    }
    if (this->HasKeyPname()) {
        element.append_attribute("key.pname") = PitchnameToStr(this->GetKeyPname()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigDefaultAnl::HasKeyAccid() const
{
    return (m_keyAccid != ACCIDENTAL_GESTURAL_NONE);
}

bool AttKeySigDefaultAnl::HasKeyMode() const
{
    return (m_keyMode != MODE_NONE);
}

bool AttKeySigDefaultAnl::HasKeyPname() const
{
    return (m_keyPname != PITCHNAME_NONE);
}

//----------------------------------------------------------------------------
// AttMelodicFunction
//----------------------------------------------------------------------------

AttMelodicFunction::AttMelodicFunction() : Att()
{
    this->ResetMelodicFunction();
}

void AttMelodicFunction::ResetMelodicFunction()
{
    m_mfunc = MELODICFUNCTION_NONE;
}

bool AttMelodicFunction::ReadMelodicFunction(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("mfunc")) {
        this->SetMfunc(StrToMelodicfunction(element.attribute("mfunc").value()));
        if (removeAttr) element.remove_attribute("mfunc");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMelodicFunction::WriteMelodicFunction(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMfunc()) {
        element.append_attribute("mfunc") = MelodicfunctionToStr(this->GetMfunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMelodicFunction::HasMfunc() const
{
    return (m_mfunc != MELODICFUNCTION_NONE);
}

//----------------------------------------------------------------------------
// AttPitchClass
//----------------------------------------------------------------------------

AttPitchClass::AttPitchClass() : Att()
{
    this->ResetPitchClass();
}

void AttPitchClass::ResetPitchClass()
{
    m_pclass = MEI_UNSET;
}

bool AttPitchClass::ReadPitchClass(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("pclass")) {
        this->SetPclass(StrToInt(element.attribute("pclass").value()));
        if (removeAttr) element.remove_attribute("pclass");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPitchClass::WritePitchClass(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPclass()) {
        element.append_attribute("pclass") = IntToStr(this->GetPclass()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPitchClass::HasPclass() const
{
    return (m_pclass != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttSolfa
//----------------------------------------------------------------------------

AttSolfa::AttSolfa() : Att()
{
    this->ResetSolfa();
}

void AttSolfa::ResetSolfa()
{
    m_psolfa = "";
}

bool AttSolfa::ReadSolfa(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("psolfa")) {
        this->SetPsolfa(StrToStr(element.attribute("psolfa").value()));
        if (removeAttr) element.remove_attribute("psolfa");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSolfa::WriteSolfa(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPsolfa()) {
        element.append_attribute("psolfa") = StrToStr(this->GetPsolfa()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSolfa::HasPsolfa() const
{
    return (m_psolfa != "");
}

} // namespace vrv
