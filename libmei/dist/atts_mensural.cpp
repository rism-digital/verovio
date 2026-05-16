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

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttDurationQuality
//----------------------------------------------------------------------------

AttDurationQuality::AttDurationQuality() : Att()
{
    this->ResetDurationQuality();
}

void AttDurationQuality::ResetDurationQuality()
{
    m_durQuality = DURQUALITY_mensural_NONE;
}

bool AttDurationQuality::ReadDurationQuality(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dur.quality")) {
        this->SetDurQuality(StrToDurqualityMensural(element.attribute("dur.quality").value()));
        if (removeAttr) element.remove_attribute("dur.quality");
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

//----------------------------------------------------------------------------
// AttMensuralLog
//----------------------------------------------------------------------------

AttMensuralLog::AttMensuralLog() : Att()
{
    this->ResetMensuralLog();
}

void AttMensuralLog::ResetMensuralLog()
{
    m_proportNum = MEI_UNSET;
    m_proportNumbase = MEI_UNSET;
}

bool AttMensuralLog::ReadMensuralLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("proport.num")) {
        this->SetProportNum(StrToInt(element.attribute("proport.num").value()));
        if (removeAttr) element.remove_attribute("proport.num");
        hasAttribute = true;
    }
    if (element.attribute("proport.numbase")) {
        this->SetProportNumbase(StrToInt(element.attribute("proport.numbase").value()));
        if (removeAttr) element.remove_attribute("proport.numbase");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensuralLog::WriteMensuralLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
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

bool AttMensuralLog::HasProportNum() const
{
    return (m_proportNum != MEI_UNSET);
}

bool AttMensuralLog::HasProportNumbase() const
{
    return (m_proportNumbase != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttMensuralShared
//----------------------------------------------------------------------------

AttMensuralShared::AttMensuralShared() : Att()
{
    this->ResetMensuralShared();
}

void AttMensuralShared::ResetMensuralShared()
{
    m_modusmaior = MODUSMAIOR_NONE;
    m_modusminor = MODUSMINOR_NONE;
    m_prolatio = PROLATIO_NONE;
    m_tempus = TEMPUS_NONE;
    m_divisio = DIVISIO_NONE;
}

bool AttMensuralShared::ReadMensuralShared(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("modusmaior")) {
        this->SetModusmaior(StrToModusmaior(element.attribute("modusmaior").value()));
        if (removeAttr) element.remove_attribute("modusmaior");
        hasAttribute = true;
    }
    if (element.attribute("modusminor")) {
        this->SetModusminor(StrToModusminor(element.attribute("modusminor").value()));
        if (removeAttr) element.remove_attribute("modusminor");
        hasAttribute = true;
    }
    if (element.attribute("prolatio")) {
        this->SetProlatio(StrToProlatio(element.attribute("prolatio").value()));
        if (removeAttr) element.remove_attribute("prolatio");
        hasAttribute = true;
    }
    if (element.attribute("tempus")) {
        this->SetTempus(StrToTempus(element.attribute("tempus").value()));
        if (removeAttr) element.remove_attribute("tempus");
        hasAttribute = true;
    }
    if (element.attribute("divisio")) {
        this->SetDivisio(StrToDivisio(element.attribute("divisio").value()));
        if (removeAttr) element.remove_attribute("divisio");
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

//----------------------------------------------------------------------------
// AttNoteVisMensural
//----------------------------------------------------------------------------

AttNoteVisMensural::AttNoteVisMensural() : Att()
{
    this->ResetNoteVisMensural();
}

void AttNoteVisMensural::ResetNoteVisMensural()
{
    m_lig = LIGATUREFORM_NONE;
}

bool AttNoteVisMensural::ReadNoteVisMensural(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("lig")) {
        this->SetLig(StrToLigatureform(element.attribute("lig").value()));
        if (removeAttr) element.remove_attribute("lig");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteVisMensural::WriteNoteVisMensural(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLig()) {
        element.append_attribute("lig") = LigatureformToStr(this->GetLig()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNoteVisMensural::HasLig() const
{
    return (m_lig != LIGATUREFORM_NONE);
}

//----------------------------------------------------------------------------
// AttRestVisMensural
//----------------------------------------------------------------------------

AttRestVisMensural::AttRestVisMensural() : Att()
{
    this->ResetRestVisMensural();
}

void AttRestVisMensural::ResetRestVisMensural()
{
    m_spaces = MEI_UNSET;
}

bool AttRestVisMensural::ReadRestVisMensural(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("spaces")) {
        this->SetSpaces(StrToInt(element.attribute("spaces").value()));
        if (removeAttr) element.remove_attribute("spaces");
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
    return (m_spaces != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttStemsMensural
//----------------------------------------------------------------------------

AttStemsMensural::AttStemsMensural() : Att()
{
    this->ResetStemsMensural();
}

void AttStemsMensural::ResetStemsMensural()
{
    m_stemForm = STEMFORM_mensural_NONE;
}

bool AttStemsMensural::ReadStemsMensural(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("stem.form")) {
        this->SetStemForm(StrToStemformMensural(element.attribute("stem.form").value()));
        if (removeAttr) element.remove_attribute("stem.form");
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

} // namespace vrv
