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

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttHarmAnl
//----------------------------------------------------------------------------

AttHarmAnl::AttHarmAnl() : Att()
{
    ResetHarmAnl();
}

AttHarmAnl::~AttHarmAnl()
{
}

void AttHarmAnl::ResetHarmAnl()
{
    m_form = harmAnl_FORM_NONE;
}

bool AttHarmAnl::ReadHarmAnl(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToHarmAnlForm(element.attribute("form").value()));
        element.remove_attribute("form");
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

/* include <attform> */

//----------------------------------------------------------------------------
// AttHarmonicFunction
//----------------------------------------------------------------------------

AttHarmonicFunction::AttHarmonicFunction() : Att()
{
    ResetHarmonicFunction();
}

AttHarmonicFunction::~AttHarmonicFunction()
{
}

void AttHarmonicFunction::ResetHarmonicFunction()
{
    m_deg = "";
}

bool AttHarmonicFunction::ReadHarmonicFunction(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("deg")) {
        this->SetDeg(StrToStr(element.attribute("deg").value()));
        element.remove_attribute("deg");
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

/* include <attdeg> */

//----------------------------------------------------------------------------
// AttIntervalHarmonic
//----------------------------------------------------------------------------

AttIntervalHarmonic::AttIntervalHarmonic() : Att()
{
    ResetIntervalHarmonic();
}

AttIntervalHarmonic::~AttIntervalHarmonic()
{
}

void AttIntervalHarmonic::ResetIntervalHarmonic()
{
    m_inth = "";
}

bool AttIntervalHarmonic::ReadIntervalHarmonic(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("inth")) {
        this->SetInth(StrToStr(element.attribute("inth").value()));
        element.remove_attribute("inth");
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

/* include <attinth> */

//----------------------------------------------------------------------------
// AttIntervalMelodic
//----------------------------------------------------------------------------

AttIntervalMelodic::AttIntervalMelodic() : Att()
{
    ResetIntervalMelodic();
}

AttIntervalMelodic::~AttIntervalMelodic()
{
}

void AttIntervalMelodic::ResetIntervalMelodic()
{
    m_intm = "";
}

bool AttIntervalMelodic::ReadIntervalMelodic(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("intm")) {
        this->SetIntm(StrToStr(element.attribute("intm").value()));
        element.remove_attribute("intm");
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

/* include <attintm> */

//----------------------------------------------------------------------------
// AttKeySigAnl
//----------------------------------------------------------------------------

AttKeySigAnl::AttKeySigAnl() : Att()
{
    ResetKeySigAnl();
}

AttKeySigAnl::~AttKeySigAnl()
{
}

void AttKeySigAnl::ResetKeySigAnl()
{
    m_mode = MODE_NONE;
}

bool AttKeySigAnl::ReadKeySigAnl(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("mode")) {
        this->SetMode(StrToMode(element.attribute("mode").value()));
        element.remove_attribute("mode");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigAnl::WriteKeySigAnl(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMode()) {
        element.append_attribute("mode") = ModeToStr(this->GetMode()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigAnl::HasMode() const
{
    return (m_mode != MODE_NONE);
}

/* include <attmode> */

//----------------------------------------------------------------------------
// AttKeySigDefaultAnl
//----------------------------------------------------------------------------

AttKeySigDefaultAnl::AttKeySigDefaultAnl() : Att()
{
    ResetKeySigDefaultAnl();
}

AttKeySigDefaultAnl::~AttKeySigDefaultAnl()
{
}

void AttKeySigDefaultAnl::ResetKeySigDefaultAnl()
{
    m_keyAccid = ACCIDENTAL_GESTURAL_NONE;
    m_keyMode = MODE_NONE;
    m_keyPname = PITCHNAME_NONE;
}

bool AttKeySigDefaultAnl::ReadKeySigDefaultAnl(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("key.accid")) {
        this->SetKeyAccid(StrToAccidentalGestural(element.attribute("key.accid").value()));
        element.remove_attribute("key.accid");
        hasAttribute = true;
    }
    if (element.attribute("key.mode")) {
        this->SetKeyMode(StrToMode(element.attribute("key.mode").value()));
        element.remove_attribute("key.mode");
        hasAttribute = true;
    }
    if (element.attribute("key.pname")) {
        this->SetKeyPname(StrToPitchname(element.attribute("key.pname").value()));
        element.remove_attribute("key.pname");
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

/* include <attkey.pname> */

//----------------------------------------------------------------------------
// AttMelodicFunction
//----------------------------------------------------------------------------

AttMelodicFunction::AttMelodicFunction() : Att()
{
    ResetMelodicFunction();
}

AttMelodicFunction::~AttMelodicFunction()
{
}

void AttMelodicFunction::ResetMelodicFunction()
{
    m_mfunc = MELODICFUNCTION_NONE;
}

bool AttMelodicFunction::ReadMelodicFunction(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("mfunc")) {
        this->SetMfunc(StrToMelodicfunction(element.attribute("mfunc").value()));
        element.remove_attribute("mfunc");
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

/* include <attmfunc> */

//----------------------------------------------------------------------------
// AttPitchClass
//----------------------------------------------------------------------------

AttPitchClass::AttPitchClass() : Att()
{
    ResetPitchClass();
}

AttPitchClass::~AttPitchClass()
{
}

void AttPitchClass::ResetPitchClass()
{
    m_pclass = 0;
}

bool AttPitchClass::ReadPitchClass(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("pclass")) {
        this->SetPclass(StrToInt(element.attribute("pclass").value()));
        element.remove_attribute("pclass");
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
    return (m_pclass != 0);
}

/* include <attpclass> */

//----------------------------------------------------------------------------
// AttSolfa
//----------------------------------------------------------------------------

AttSolfa::AttSolfa() : Att()
{
    ResetSolfa();
}

AttSolfa::~AttSolfa()
{
}

void AttSolfa::ResetSolfa()
{
    m_psolfa = "";
}

bool AttSolfa::ReadSolfa(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("psolfa")) {
        this->SetPsolfa(StrToStr(element.attribute("psolfa").value()));
        element.remove_attribute("psolfa");
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

/* include <attpsolfa> */

bool Att::SetAnalytical(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_HARMANL)) {
        AttHarmAnl *att = dynamic_cast<AttHarmAnl *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToHarmAnlForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HARMONICFUNCTION)) {
        AttHarmonicFunction *att = dynamic_cast<AttHarmonicFunction *>(element);
        assert(att);
        if (attrType == "deg") {
            att->SetDeg(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_INTERVALHARMONIC)) {
        AttIntervalHarmonic *att = dynamic_cast<AttIntervalHarmonic *>(element);
        assert(att);
        if (attrType == "inth") {
            att->SetInth(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_INTERVALMELODIC)) {
        AttIntervalMelodic *att = dynamic_cast<AttIntervalMelodic *>(element);
        assert(att);
        if (attrType == "intm") {
            att->SetIntm(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_KEYSIGANL)) {
        AttKeySigAnl *att = dynamic_cast<AttKeySigAnl *>(element);
        assert(att);
        if (attrType == "mode") {
            att->SetMode(att->StrToMode(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_KEYSIGDEFAULTANL)) {
        AttKeySigDefaultAnl *att = dynamic_cast<AttKeySigDefaultAnl *>(element);
        assert(att);
        if (attrType == "key.accid") {
            att->SetKeyAccid(att->StrToAccidentalGestural(attrValue));
            return true;
        }
        if (attrType == "key.mode") {
            att->SetKeyMode(att->StrToMode(attrValue));
            return true;
        }
        if (attrType == "key.pname") {
            att->SetKeyPname(att->StrToPitchname(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MELODICFUNCTION)) {
        AttMelodicFunction *att = dynamic_cast<AttMelodicFunction *>(element);
        assert(att);
        if (attrType == "mfunc") {
            att->SetMfunc(att->StrToMelodicfunction(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PITCHCLASS)) {
        AttPitchClass *att = dynamic_cast<AttPitchClass *>(element);
        assert(att);
        if (attrType == "pclass") {
            att->SetPclass(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SOLFA)) {
        AttSolfa *att = dynamic_cast<AttSolfa *>(element);
        assert(att);
        if (attrType == "psolfa") {
            att->SetPsolfa(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetAnalytical(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_HARMANL)) {
        const AttHarmAnl *att = dynamic_cast<const AttHarmAnl *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->HarmAnlFormToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass(ATT_HARMONICFUNCTION)) {
        const AttHarmonicFunction *att = dynamic_cast<const AttHarmonicFunction *>(element);
        assert(att);
        if (att->HasDeg()) {
            attributes->push_back(std::make_pair("deg", att->StrToStr(att->GetDeg())));
        }
    }
    if (element->HasAttClass(ATT_INTERVALHARMONIC)) {
        const AttIntervalHarmonic *att = dynamic_cast<const AttIntervalHarmonic *>(element);
        assert(att);
        if (att->HasInth()) {
            attributes->push_back(std::make_pair("inth", att->StrToStr(att->GetInth())));
        }
    }
    if (element->HasAttClass(ATT_INTERVALMELODIC)) {
        const AttIntervalMelodic *att = dynamic_cast<const AttIntervalMelodic *>(element);
        assert(att);
        if (att->HasIntm()) {
            attributes->push_back(std::make_pair("intm", att->StrToStr(att->GetIntm())));
        }
    }
    if (element->HasAttClass(ATT_KEYSIGANL)) {
        const AttKeySigAnl *att = dynamic_cast<const AttKeySigAnl *>(element);
        assert(att);
        if (att->HasMode()) {
            attributes->push_back(std::make_pair("mode", att->ModeToStr(att->GetMode())));
        }
    }
    if (element->HasAttClass(ATT_KEYSIGDEFAULTANL)) {
        const AttKeySigDefaultAnl *att = dynamic_cast<const AttKeySigDefaultAnl *>(element);
        assert(att);
        if (att->HasKeyAccid()) {
            attributes->push_back(std::make_pair("key.accid", att->AccidentalGesturalToStr(att->GetKeyAccid())));
        }
        if (att->HasKeyMode()) {
            attributes->push_back(std::make_pair("key.mode", att->ModeToStr(att->GetKeyMode())));
        }
        if (att->HasKeyPname()) {
            attributes->push_back(std::make_pair("key.pname", att->PitchnameToStr(att->GetKeyPname())));
        }
    }
    if (element->HasAttClass(ATT_MELODICFUNCTION)) {
        const AttMelodicFunction *att = dynamic_cast<const AttMelodicFunction *>(element);
        assert(att);
        if (att->HasMfunc()) {
            attributes->push_back(std::make_pair("mfunc", att->MelodicfunctionToStr(att->GetMfunc())));
        }
    }
    if (element->HasAttClass(ATT_PITCHCLASS)) {
        const AttPitchClass *att = dynamic_cast<const AttPitchClass *>(element);
        assert(att);
        if (att->HasPclass()) {
            attributes->push_back(std::make_pair("pclass", att->IntToStr(att->GetPclass())));
        }
    }
    if (element->HasAttClass(ATT_SOLFA)) {
        const AttSolfa *att = dynamic_cast<const AttSolfa *>(element);
        assert(att);
        if (att->HasPsolfa()) {
            attributes->push_back(std::make_pair("psolfa", att->StrToStr(att->GetPsolfa())));
        }
    }
}

} // vrv namespace
