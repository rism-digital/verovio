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

#include "atts_mensural.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttSTEMPROPERTIESMensural
//----------------------------------------------------------------------------

AttSTEMPROPERTIESMensural::AttSTEMPROPERTIESMensural() : Att()
{
    ResetSTEMPROPERTIESMensural();
}

AttSTEMPROPERTIESMensural::~AttSTEMPROPERTIESMensural()
{
}

void AttSTEMPROPERTIESMensural::ResetSTEMPROPERTIESMensural()
{
    m_pos = STEMPOSITION_NONE;
    m_length = VRV_UNSET;
    m_form = STEMFORM_mensural_NONE;
    m_dir = STEMDIRECTION_NONE;
    m_flagPos = FLAGPOS_mensural_NONE;
    m_flagForm = FLAGFORM_mensural_NONE;
}

bool AttSTEMPROPERTIESMensural::ReadSTEMPROPERTIESMensural(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("pos")) {
        this->SetPos(StrToStemposition(element.attribute("pos").value()));
        element.remove_attribute("pos");
        hasAttribute = true;
    }
    if (element.attribute("length")) {
        this->SetLength(StrToMeasurementabs(element.attribute("length").value()));
        element.remove_attribute("length");
        hasAttribute = true;
    }
    if (element.attribute("form")) {
        this->SetForm(StrToStemformMensural(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("dir")) {
        this->SetDir(StrToStemdirection(element.attribute("dir").value()));
        element.remove_attribute("dir");
        hasAttribute = true;
    }
    if (element.attribute("flag.pos")) {
        this->SetFlagPos(StrToFlagposMensural(element.attribute("flag.pos").value()));
        element.remove_attribute("flag.pos");
        hasAttribute = true;
    }
    if (element.attribute("flag.form")) {
        this->SetFlagForm(StrToFlagformMensural(element.attribute("flag.form").value()));
        element.remove_attribute("flag.form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSTEMPROPERTIESMensural::WriteSTEMPROPERTIESMensural(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPos()) {
        element.append_attribute("pos") = StempositionToStr(this->GetPos()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLength()) {
        element.append_attribute("length") = MeasurementabsToStr(this->GetLength()).c_str();
        wroteAttribute = true;
    }
    if (this->HasForm()) {
        element.append_attribute("form") = StemformMensuralToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDir()) {
        element.append_attribute("dir") = StemdirectionToStr(this->GetDir()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFlagPos()) {
        element.append_attribute("flag.pos") = FlagposMensuralToStr(this->GetFlagPos()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFlagForm()) {
        element.append_attribute("flag.form") = FlagformMensuralToStr(this->GetFlagForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSTEMPROPERTIESMensural::HasPos() const
{
    return (m_pos != STEMPOSITION_NONE);
}

bool AttSTEMPROPERTIESMensural::HasLength() const
{
    return (m_length != VRV_UNSET);
}

bool AttSTEMPROPERTIESMensural::HasForm() const
{
    return (m_form != STEMFORM_mensural_NONE);
}

bool AttSTEMPROPERTIESMensural::HasDir() const
{
    return (m_dir != STEMDIRECTION_NONE);
}

bool AttSTEMPROPERTIESMensural::HasFlagPos() const
{
    return (m_flagPos != FLAGPOS_mensural_NONE);
}

bool AttSTEMPROPERTIESMensural::HasFlagForm() const
{
    return (m_flagForm != FLAGFORM_mensural_NONE);
}

/* include <attflag.form> */

//----------------------------------------------------------------------------
// AttDurationQuality
//----------------------------------------------------------------------------

AttDurationQuality::AttDurationQuality() : Att()
{
    ResetDurationQuality();
}

AttDurationQuality::~AttDurationQuality()
{
}

void AttDurationQuality::ResetDurationQuality()
{
    m_durQuality = DURQUALITY_mensural_NONE;
}

bool AttDurationQuality::ReadDurationQuality(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dur.quality")) {
        this->SetDurQuality(StrToDurqualityMensural(element.attribute("dur.quality").value()));
        element.remove_attribute("dur.quality");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationQuality::WriteDurationQuality(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDurQuality()) {
        element.append_attribute("dur.quality") = DurqualityMensuralToStr(this->GetDurQuality()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDurationQuality::HasDurQuality() const
{
    return (m_durQuality != DURQUALITY_mensural_NONE);
}

/* include <attdur.quality> */

//----------------------------------------------------------------------------
// AttLigatureLog
//----------------------------------------------------------------------------

AttLigatureLog::AttLigatureLog() : Att()
{
    ResetLigatureLog();
}

AttLigatureLog::~AttLigatureLog()
{
}

void AttLigatureLog::ResetLigatureLog()
{
    m_form = LIGATUREFORM_NONE;
}

bool AttLigatureLog::ReadLigatureLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToLigatureform(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLigatureLog::WriteLigatureLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = LigatureformToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLigatureLog::HasForm() const
{
    return (m_form != LIGATUREFORM_NONE);
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttMensuralLog
//----------------------------------------------------------------------------

AttMensuralLog::AttMensuralLog() : Att()
{
    ResetMensuralLog();
}

AttMensuralLog::~AttMensuralLog()
{
}

void AttMensuralLog::ResetMensuralLog()
{
    m_mensurDot = BOOLEAN_NONE;
    m_mensurSign = MENSURATIONSIGN_NONE;
    m_mensurSlash = 0;
    m_proportNum = -1;
    m_proportNumbase = -1;
}

bool AttMensuralLog::ReadMensuralLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("mensur.dot")) {
        this->SetMensurDot(StrToBoolean(element.attribute("mensur.dot").value()));
        element.remove_attribute("mensur.dot");
        hasAttribute = true;
    }
    if (element.attribute("mensur.sign")) {
        this->SetMensurSign(StrToMensurationsign(element.attribute("mensur.sign").value()));
        element.remove_attribute("mensur.sign");
        hasAttribute = true;
    }
    if (element.attribute("mensur.slash")) {
        this->SetMensurSlash(StrToInt(element.attribute("mensur.slash").value()));
        element.remove_attribute("mensur.slash");
        hasAttribute = true;
    }
    if (element.attribute("proport.num")) {
        this->SetProportNum(StrToInt(element.attribute("proport.num").value()));
        element.remove_attribute("proport.num");
        hasAttribute = true;
    }
    if (element.attribute("proport.numbase")) {
        this->SetProportNumbase(StrToInt(element.attribute("proport.numbase").value()));
        element.remove_attribute("proport.numbase");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensuralLog::WriteMensuralLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMensurDot()) {
        element.append_attribute("mensur.dot") = BooleanToStr(this->GetMensurDot()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurSign()) {
        element.append_attribute("mensur.sign") = MensurationsignToStr(this->GetMensurSign()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurSlash()) {
        element.append_attribute("mensur.slash") = IntToStr(this->GetMensurSlash()).c_str();
        wroteAttribute = true;
    }
    if (this->HasProportNum()) {
        element.append_attribute("proport.num") = IntToStr(this->GetProportNum()).c_str();
        wroteAttribute = true;
    }
    if (this->HasProportNumbase()) {
        element.append_attribute("proport.numbase") = IntToStr(this->GetProportNumbase()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensuralLog::HasMensurDot() const
{
    return (m_mensurDot != BOOLEAN_NONE);
}

bool AttMensuralLog::HasMensurSign() const
{
    return (m_mensurSign != MENSURATIONSIGN_NONE);
}

bool AttMensuralLog::HasMensurSlash() const
{
    return (m_mensurSlash != 0);
}

bool AttMensuralLog::HasProportNum() const
{
    return (m_proportNum != -1);
}

bool AttMensuralLog::HasProportNumbase() const
{
    return (m_proportNumbase != -1);
}

/* include <attproport.numbase> */

//----------------------------------------------------------------------------
// AttMensuralShared
//----------------------------------------------------------------------------

AttMensuralShared::AttMensuralShared() : Att()
{
    ResetMensuralShared();
}

AttMensuralShared::~AttMensuralShared()
{
}

void AttMensuralShared::ResetMensuralShared()
{
    m_modusmaior = MODUSMAIOR_NONE;
    m_modusminor = MODUSMINOR_NONE;
    m_prolatio = PROLATIO_NONE;
    m_tempus = TEMPUS_NONE;
    m_divisio = DIVISIO_NONE;
}

bool AttMensuralShared::ReadMensuralShared(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("modusmaior")) {
        this->SetModusmaior(StrToModusmaior(element.attribute("modusmaior").value()));
        element.remove_attribute("modusmaior");
        hasAttribute = true;
    }
    if (element.attribute("modusminor")) {
        this->SetModusminor(StrToModusminor(element.attribute("modusminor").value()));
        element.remove_attribute("modusminor");
        hasAttribute = true;
    }
    if (element.attribute("prolatio")) {
        this->SetProlatio(StrToProlatio(element.attribute("prolatio").value()));
        element.remove_attribute("prolatio");
        hasAttribute = true;
    }
    if (element.attribute("tempus")) {
        this->SetTempus(StrToTempus(element.attribute("tempus").value()));
        element.remove_attribute("tempus");
        hasAttribute = true;
    }
    if (element.attribute("divisio")) {
        this->SetDivisio(StrToDivisio(element.attribute("divisio").value()));
        element.remove_attribute("divisio");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensuralShared::WriteMensuralShared(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasModusmaior()) {
        element.append_attribute("modusmaior") = ModusmaiorToStr(this->GetModusmaior()).c_str();
        wroteAttribute = true;
    }
    if (this->HasModusminor()) {
        element.append_attribute("modusminor") = ModusminorToStr(this->GetModusminor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasProlatio()) {
        element.append_attribute("prolatio") = ProlatioToStr(this->GetProlatio()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTempus()) {
        element.append_attribute("tempus") = TempusToStr(this->GetTempus()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDivisio()) {
        element.append_attribute("divisio") = DivisioToStr(this->GetDivisio()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensuralShared::HasModusmaior() const
{
    return (m_modusmaior != MODUSMAIOR_NONE);
}

bool AttMensuralShared::HasModusminor() const
{
    return (m_modusminor != MODUSMINOR_NONE);
}

bool AttMensuralShared::HasProlatio() const
{
    return (m_prolatio != PROLATIO_NONE);
}

bool AttMensuralShared::HasTempus() const
{
    return (m_tempus != TEMPUS_NONE);
}

bool AttMensuralShared::HasDivisio() const
{
    return (m_divisio != DIVISIO_NONE);
}

/* include <attdivisio> */

//----------------------------------------------------------------------------
// AttNoteAnlMensural
//----------------------------------------------------------------------------

AttNoteAnlMensural::AttNoteAnlMensural() : Att()
{
    ResetNoteAnlMensural();
}

AttNoteAnlMensural::~AttNoteAnlMensural()
{
}

void AttNoteAnlMensural::ResetNoteAnlMensural()
{
    m_lig = noteAnlMensural_LIG_NONE;
}

bool AttNoteAnlMensural::ReadNoteAnlMensural(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("lig")) {
        this->SetLig(StrToNoteAnlMensuralLig(element.attribute("lig").value()));
        element.remove_attribute("lig");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteAnlMensural::WriteNoteAnlMensural(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLig()) {
        element.append_attribute("lig") = NoteAnlMensuralLigToStr(this->GetLig()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNoteAnlMensural::HasLig() const
{
    return (m_lig != noteAnlMensural_LIG_NONE);
}

/* include <attlig> */

//----------------------------------------------------------------------------
// AttRestVisMensural
//----------------------------------------------------------------------------

AttRestVisMensural::AttRestVisMensural() : Att()
{
    ResetRestVisMensural();
}

AttRestVisMensural::~AttRestVisMensural()
{
}

void AttRestVisMensural::ResetRestVisMensural()
{
    m_spaces = 0;
}

bool AttRestVisMensural::ReadRestVisMensural(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("spaces")) {
        this->SetSpaces(StrToInt(element.attribute("spaces").value()));
        element.remove_attribute("spaces");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRestVisMensural::WriteRestVisMensural(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSpaces()) {
        element.append_attribute("spaces") = IntToStr(this->GetSpaces()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRestVisMensural::HasSpaces() const
{
    return (m_spaces != 0);
}

/* include <attspaces> */

//----------------------------------------------------------------------------
// AttStemsMensural
//----------------------------------------------------------------------------

AttStemsMensural::AttStemsMensural() : Att()
{
    ResetStemsMensural();
}

AttStemsMensural::~AttStemsMensural()
{
}

void AttStemsMensural::ResetStemsMensural()
{
    m_stemForm = STEMFORM_mensural_NONE;
}

bool AttStemsMensural::ReadStemsMensural(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("stem.form")) {
        this->SetStemForm(StrToStemformMensural(element.attribute("stem.form").value()));
        element.remove_attribute("stem.form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStemsMensural::WriteStemsMensural(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasStemForm()) {
        element.append_attribute("stem.form") = StemformMensuralToStr(this->GetStemForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStemsMensural::HasStemForm() const
{
    return (m_stemForm != STEMFORM_mensural_NONE);
}

/* include <attstem.form> */

bool Att::SetMensural(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_STEMPROPERTIESMENSURAL)) {
        AttSTEMPROPERTIESMensural *att = dynamic_cast<AttSTEMPROPERTIESMensural *>(element);
        assert(att);
        if (attrType == "pos") {
            att->SetPos(att->StrToStemposition(attrValue));
            return true;
        }
        if (attrType == "length") {
            att->SetLength(att->StrToMeasurementabs(attrValue));
            return true;
        }
        if (attrType == "form") {
            att->SetForm(att->StrToStemformMensural(attrValue));
            return true;
        }
        if (attrType == "dir") {
            att->SetDir(att->StrToStemdirection(attrValue));
            return true;
        }
        if (attrType == "flag.pos") {
            att->SetFlagPos(att->StrToFlagposMensural(attrValue));
            return true;
        }
        if (attrType == "flag.form") {
            att->SetFlagForm(att->StrToFlagformMensural(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_DURATIONQUALITY)) {
        AttDurationQuality *att = dynamic_cast<AttDurationQuality *>(element);
        assert(att);
        if (attrType == "dur.quality") {
            att->SetDurQuality(att->StrToDurqualityMensural(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LIGATURELOG)) {
        AttLigatureLog *att = dynamic_cast<AttLigatureLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToLigatureform(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MENSURALLOG)) {
        AttMensuralLog *att = dynamic_cast<AttMensuralLog *>(element);
        assert(att);
        if (attrType == "mensur.dot") {
            att->SetMensurDot(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "mensur.sign") {
            att->SetMensurSign(att->StrToMensurationsign(attrValue));
            return true;
        }
        if (attrType == "mensur.slash") {
            att->SetMensurSlash(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "proport.num") {
            att->SetProportNum(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "proport.numbase") {
            att->SetProportNumbase(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MENSURALSHARED)) {
        AttMensuralShared *att = dynamic_cast<AttMensuralShared *>(element);
        assert(att);
        if (attrType == "modusmaior") {
            att->SetModusmaior(att->StrToModusmaior(attrValue));
            return true;
        }
        if (attrType == "modusminor") {
            att->SetModusminor(att->StrToModusminor(attrValue));
            return true;
        }
        if (attrType == "prolatio") {
            att->SetProlatio(att->StrToProlatio(attrValue));
            return true;
        }
        if (attrType == "tempus") {
            att->SetTempus(att->StrToTempus(attrValue));
            return true;
        }
        if (attrType == "divisio") {
            att->SetDivisio(att->StrToDivisio(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NOTEANLMENSURAL)) {
        AttNoteAnlMensural *att = dynamic_cast<AttNoteAnlMensural *>(element);
        assert(att);
        if (attrType == "lig") {
            att->SetLig(att->StrToNoteAnlMensuralLig(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_RESTVISMENSURAL)) {
        AttRestVisMensural *att = dynamic_cast<AttRestVisMensural *>(element);
        assert(att);
        if (attrType == "spaces") {
            att->SetSpaces(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STEMSMENSURAL)) {
        AttStemsMensural *att = dynamic_cast<AttStemsMensural *>(element);
        assert(att);
        if (attrType == "stem.form") {
            att->SetStemForm(att->StrToStemformMensural(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetMensural(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_STEMPROPERTIESMENSURAL)) {
        const AttSTEMPROPERTIESMensural *att = dynamic_cast<const AttSTEMPROPERTIESMensural *>(element);
        assert(att);
        if (att->HasPos()) {
            attributes->push_back(std::make_pair("pos", att->StempositionToStr(att->GetPos())));
        }
        if (att->HasLength()) {
            attributes->push_back(std::make_pair("length", att->MeasurementabsToStr(att->GetLength())));
        }
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->StemformMensuralToStr(att->GetForm())));
        }
        if (att->HasDir()) {
            attributes->push_back(std::make_pair("dir", att->StemdirectionToStr(att->GetDir())));
        }
        if (att->HasFlagPos()) {
            attributes->push_back(std::make_pair("flag.pos", att->FlagposMensuralToStr(att->GetFlagPos())));
        }
        if (att->HasFlagForm()) {
            attributes->push_back(std::make_pair("flag.form", att->FlagformMensuralToStr(att->GetFlagForm())));
        }
    }
    if (element->HasAttClass(ATT_DURATIONQUALITY)) {
        const AttDurationQuality *att = dynamic_cast<const AttDurationQuality *>(element);
        assert(att);
        if (att->HasDurQuality()) {
            attributes->push_back(std::make_pair("dur.quality", att->DurqualityMensuralToStr(att->GetDurQuality())));
        }
    }
    if (element->HasAttClass(ATT_LIGATURELOG)) {
        const AttLigatureLog *att = dynamic_cast<const AttLigatureLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->LigatureformToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass(ATT_MENSURALLOG)) {
        const AttMensuralLog *att = dynamic_cast<const AttMensuralLog *>(element);
        assert(att);
        if (att->HasMensurDot()) {
            attributes->push_back(std::make_pair("mensur.dot", att->BooleanToStr(att->GetMensurDot())));
        }
        if (att->HasMensurSign()) {
            attributes->push_back(std::make_pair("mensur.sign", att->MensurationsignToStr(att->GetMensurSign())));
        }
        if (att->HasMensurSlash()) {
            attributes->push_back(std::make_pair("mensur.slash", att->IntToStr(att->GetMensurSlash())));
        }
        if (att->HasProportNum()) {
            attributes->push_back(std::make_pair("proport.num", att->IntToStr(att->GetProportNum())));
        }
        if (att->HasProportNumbase()) {
            attributes->push_back(std::make_pair("proport.numbase", att->IntToStr(att->GetProportNumbase())));
        }
    }
    if (element->HasAttClass(ATT_MENSURALSHARED)) {
        const AttMensuralShared *att = dynamic_cast<const AttMensuralShared *>(element);
        assert(att);
        if (att->HasModusmaior()) {
            attributes->push_back(std::make_pair("modusmaior", att->ModusmaiorToStr(att->GetModusmaior())));
        }
        if (att->HasModusminor()) {
            attributes->push_back(std::make_pair("modusminor", att->ModusminorToStr(att->GetModusminor())));
        }
        if (att->HasProlatio()) {
            attributes->push_back(std::make_pair("prolatio", att->ProlatioToStr(att->GetProlatio())));
        }
        if (att->HasTempus()) {
            attributes->push_back(std::make_pair("tempus", att->TempusToStr(att->GetTempus())));
        }
        if (att->HasDivisio()) {
            attributes->push_back(std::make_pair("divisio", att->DivisioToStr(att->GetDivisio())));
        }
    }
    if (element->HasAttClass(ATT_NOTEANLMENSURAL)) {
        const AttNoteAnlMensural *att = dynamic_cast<const AttNoteAnlMensural *>(element);
        assert(att);
        if (att->HasLig()) {
            attributes->push_back(std::make_pair("lig", att->NoteAnlMensuralLigToStr(att->GetLig())));
        }
    }
    if (element->HasAttClass(ATT_RESTVISMENSURAL)) {
        const AttRestVisMensural *att = dynamic_cast<const AttRestVisMensural *>(element);
        assert(att);
        if (att->HasSpaces()) {
            attributes->push_back(std::make_pair("spaces", att->IntToStr(att->GetSpaces())));
        }
    }
    if (element->HasAttClass(ATT_STEMSMENSURAL)) {
        const AttStemsMensural *att = dynamic_cast<const AttStemsMensural *>(element);
        assert(att);
        if (att->HasStemForm()) {
            attributes->push_back(std::make_pair("stem.form", att->StemformMensuralToStr(att->GetStemForm())));
        }
    }
}

} // vrv namespace
