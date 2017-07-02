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

#include "atts_gestural.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttAccidentalGestural
//----------------------------------------------------------------------------

AttAccidentalGestural::AttAccidentalGestural() : Att()
{
    ResetAccidentalGestural();
}

AttAccidentalGestural::~AttAccidentalGestural()
{
}

void AttAccidentalGestural::ResetAccidentalGestural()
{
    m_accidGes = ACCIDENTAL_GESTURAL_NONE;
}

bool AttAccidentalGestural::ReadAccidentalGestural(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("accid.ges")) {
        this->SetAccidGes(StrToAccidentalGestural(element.attribute("accid.ges").value()));
        element.remove_attribute("accid.ges");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAccidentalGestural::WriteAccidentalGestural(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAccidGes()) {
        element.append_attribute("accid.ges") = AccidentalGesturalToStr(this->GetAccidGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAccidentalGestural::HasAccidGes() const
{
    return (m_accidGes != ACCIDENTAL_GESTURAL_NONE);
}

/* include <attaccid.ges> */

//----------------------------------------------------------------------------
// AttArticulationGestural
//----------------------------------------------------------------------------

AttArticulationGestural::AttArticulationGestural() : Att()
{
    ResetArticulationGestural();
}

AttArticulationGestural::~AttArticulationGestural()
{
}

void AttArticulationGestural::ResetArticulationGestural()
{
    m_articGes = ARTICULATION_NONE;
}

bool AttArticulationGestural::ReadArticulationGestural(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("artic.ges")) {
        this->SetArticGes(StrToArticulation(element.attribute("artic.ges").value()));
        element.remove_attribute("artic.ges");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArticulationGestural::WriteArticulationGestural(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasArticGes()) {
        element.append_attribute("artic.ges") = ArticulationToStr(this->GetArticGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttArticulationGestural::HasArticGes() const
{
    return (m_articGes != ARTICULATION_NONE);
}

/* include <attartic.ges> */

//----------------------------------------------------------------------------
// AttBendGes
//----------------------------------------------------------------------------

AttBendGes::AttBendGes() : Att()
{
    ResetBendGes();
}

AttBendGes::~AttBendGes()
{
}

void AttBendGes::ResetBendGes()
{
    m_amount = 0.0;
}

bool AttBendGes::ReadBendGes(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("amount")) {
        this->SetAmount(StrToDbl(element.attribute("amount").value()));
        element.remove_attribute("amount");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBendGes::WriteBendGes(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAmount()) {
        element.append_attribute("amount") = DblToStr(this->GetAmount()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBendGes::HasAmount() const
{
    return (m_amount != 0.0);
}

/* include <attamount> */

//----------------------------------------------------------------------------
// AttDurationGestural
//----------------------------------------------------------------------------

AttDurationGestural::AttDurationGestural() : Att()
{
    ResetDurationGestural();
}

AttDurationGestural::~AttDurationGestural()
{
}

void AttDurationGestural::ResetDurationGestural()
{
    m_durGes = DURATION_NONE;
}

bool AttDurationGestural::ReadDurationGestural(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dur.ges")) {
        this->SetDurGes(StrToDuration(element.attribute("dur.ges").value()));
        element.remove_attribute("dur.ges");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationGestural::WriteDurationGestural(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDurGes()) {
        element.append_attribute("dur.ges") = DurationToStr(this->GetDurGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDurationGestural::HasDurGes() const
{
    return (m_durGes != DURATION_NONE);
}

/* include <attdur.ges> */

//----------------------------------------------------------------------------
// AttNoteGes
//----------------------------------------------------------------------------

AttNoteGes::AttNoteGes() : Att()
{
    ResetNoteGes();
}

AttNoteGes::~AttNoteGes()
{
}

void AttNoteGes::ResetNoteGes()
{
    m_extremis = noteGes_EXTREMIS_NONE;
    m_octGes = 0;
    m_pnameGes = PITCHNAME_NONE;
    m_pnum = 0;
}

bool AttNoteGes::ReadNoteGes(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("extremis")) {
        this->SetExtremis(StrToNoteGesExtremis(element.attribute("extremis").value()));
        element.remove_attribute("extremis");
        hasAttribute = true;
    }
    if (element.attribute("oct.ges")) {
        this->SetOctGes(StrToInt(element.attribute("oct.ges").value()));
        element.remove_attribute("oct.ges");
        hasAttribute = true;
    }
    if (element.attribute("pname.ges")) {
        this->SetPnameGes(StrToPitchname(element.attribute("pname.ges").value()));
        element.remove_attribute("pname.ges");
        hasAttribute = true;
    }
    if (element.attribute("pnum")) {
        this->SetPnum(StrToInt(element.attribute("pnum").value()));
        element.remove_attribute("pnum");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteGes::WriteNoteGes(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasExtremis()) {
        element.append_attribute("extremis") = NoteGesExtremisToStr(this->GetExtremis()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOctGes()) {
        element.append_attribute("oct.ges") = IntToStr(this->GetOctGes()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPnameGes()) {
        element.append_attribute("pname.ges") = PitchnameToStr(this->GetPnameGes()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPnum()) {
        element.append_attribute("pnum") = IntToStr(this->GetPnum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNoteGes::HasExtremis() const
{
    return (m_extremis != noteGes_EXTREMIS_NONE);
}

bool AttNoteGes::HasOctGes() const
{
    return (m_octGes != 0);
}

bool AttNoteGes::HasPnameGes() const
{
    return (m_pnameGes != PITCHNAME_NONE);
}

bool AttNoteGes::HasPnum() const
{
    return (m_pnum != 0);
}

/* include <attpnum> */

//----------------------------------------------------------------------------
// AttScoreDefGes
//----------------------------------------------------------------------------

AttScoreDefGes::AttScoreDefGes() : Att()
{
    ResetScoreDefGes();
}

AttScoreDefGes::~AttScoreDefGes()
{
}

void AttScoreDefGes::ResetScoreDefGes()
{
    m_tunePname = PITCHNAME_NONE;
    m_tuneHz = 0.0;
    m_tuneTemper = TEMPERAMENT_NONE;
}

bool AttScoreDefGes::ReadScoreDefGes(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tune.pname")) {
        this->SetTunePname(StrToPitchname(element.attribute("tune.pname").value()));
        element.remove_attribute("tune.pname");
        hasAttribute = true;
    }
    if (element.attribute("tune.Hz")) {
        this->SetTuneHz(StrToDbl(element.attribute("tune.Hz").value()));
        element.remove_attribute("tune.Hz");
        hasAttribute = true;
    }
    if (element.attribute("tune.temper")) {
        this->SetTuneTemper(StrToTemperament(element.attribute("tune.temper").value()));
        element.remove_attribute("tune.temper");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScoreDefGes::WriteScoreDefGes(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTunePname()) {
        element.append_attribute("tune.pname") = PitchnameToStr(this->GetTunePname()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTuneHz()) {
        element.append_attribute("tune.Hz") = DblToStr(this->GetTuneHz()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTuneTemper()) {
        element.append_attribute("tune.temper") = TemperamentToStr(this->GetTuneTemper()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttScoreDefGes::HasTunePname() const
{
    return (m_tunePname != PITCHNAME_NONE);
}

bool AttScoreDefGes::HasTuneHz() const
{
    return (m_tuneHz != 0.0);
}

bool AttScoreDefGes::HasTuneTemper() const
{
    return (m_tuneTemper != TEMPERAMENT_NONE);
}

/* include <atttune.temper> */

//----------------------------------------------------------------------------
// AttTimestampGestural
//----------------------------------------------------------------------------

AttTimestampGestural::AttTimestampGestural() : Att()
{
    ResetTimestampGestural();
}

AttTimestampGestural::~AttTimestampGestural()
{
}

void AttTimestampGestural::ResetTimestampGestural()
{
    m_tstampGes = DURATION_NONE;
    m_tstampReal = "";
}

bool AttTimestampGestural::ReadTimestampGestural(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tstamp.ges")) {
        this->SetTstampGes(StrToDuration(element.attribute("tstamp.ges").value()));
        element.remove_attribute("tstamp.ges");
        hasAttribute = true;
    }
    if (element.attribute("tstamp.real")) {
        this->SetTstampReal(StrToStr(element.attribute("tstamp.real").value()));
        element.remove_attribute("tstamp.real");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestampGestural::WriteTimestampGestural(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTstampGes()) {
        element.append_attribute("tstamp.ges") = DurationToStr(this->GetTstampGes()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTstampReal()) {
        element.append_attribute("tstamp.real") = StrToStr(this->GetTstampReal()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimestampGestural::HasTstampGes() const
{
    return (m_tstampGes != DURATION_NONE);
}

bool AttTimestampGestural::HasTstampReal() const
{
    return (m_tstampReal != "");
}

/* include <atttstamp.real> */

bool Att::SetGestural(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_ACCIDENTALGESTURAL)) {
        AttAccidentalGestural *att = dynamic_cast<AttAccidentalGestural *>(element);
        assert(att);
        if (attrType == "accid.ges") {
            att->SetAccidGes(att->StrToAccidentalGestural(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ARTICULATIONGESTURAL)) {
        AttArticulationGestural *att = dynamic_cast<AttArticulationGestural *>(element);
        assert(att);
        if (attrType == "artic.ges") {
            att->SetArticGes(att->StrToArticulation(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BENDGES)) {
        AttBendGes *att = dynamic_cast<AttBendGes *>(element);
        assert(att);
        if (attrType == "amount") {
            att->SetAmount(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_DURATIONGESTURAL)) {
        AttDurationGestural *att = dynamic_cast<AttDurationGestural *>(element);
        assert(att);
        if (attrType == "dur.ges") {
            att->SetDurGes(att->StrToDuration(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NOTEGES)) {
        AttNoteGes *att = dynamic_cast<AttNoteGes *>(element);
        assert(att);
        if (attrType == "extremis") {
            att->SetExtremis(att->StrToNoteGesExtremis(attrValue));
            return true;
        }
        if (attrType == "oct.ges") {
            att->SetOctGes(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "pname.ges") {
            att->SetPnameGes(att->StrToPitchname(attrValue));
            return true;
        }
        if (attrType == "pnum") {
            att->SetPnum(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SCOREDEFGES)) {
        AttScoreDefGes *att = dynamic_cast<AttScoreDefGes *>(element);
        assert(att);
        if (attrType == "tune.pname") {
            att->SetTunePname(att->StrToPitchname(attrValue));
            return true;
        }
        if (attrType == "tune.Hz") {
            att->SetTuneHz(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "tune.temper") {
            att->SetTuneTemper(att->StrToTemperament(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMPGESTURAL)) {
        AttTimestampGestural *att = dynamic_cast<AttTimestampGestural *>(element);
        assert(att);
        if (attrType == "tstamp.ges") {
            att->SetTstampGes(att->StrToDuration(attrValue));
            return true;
        }
        if (attrType == "tstamp.real") {
            att->SetTstampReal(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetGestural(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ACCIDENTALGESTURAL)) {
        const AttAccidentalGestural *att = dynamic_cast<const AttAccidentalGestural *>(element);
        assert(att);
        if (att->HasAccidGes()) {
            attributes->push_back(std::make_pair("accid.ges", att->AccidentalGesturalToStr(att->GetAccidGes())));
        }
    }
    if (element->HasAttClass(ATT_ARTICULATIONGESTURAL)) {
        const AttArticulationGestural *att = dynamic_cast<const AttArticulationGestural *>(element);
        assert(att);
        if (att->HasArticGes()) {
            attributes->push_back(std::make_pair("artic.ges", att->ArticulationToStr(att->GetArticGes())));
        }
    }
    if (element->HasAttClass(ATT_BENDGES)) {
        const AttBendGes *att = dynamic_cast<const AttBendGes *>(element);
        assert(att);
        if (att->HasAmount()) {
            attributes->push_back(std::make_pair("amount", att->DblToStr(att->GetAmount())));
        }
    }
    if (element->HasAttClass(ATT_DURATIONGESTURAL)) {
        const AttDurationGestural *att = dynamic_cast<const AttDurationGestural *>(element);
        assert(att);
        if (att->HasDurGes()) {
            attributes->push_back(std::make_pair("dur.ges", att->DurationToStr(att->GetDurGes())));
        }
    }
    if (element->HasAttClass(ATT_NOTEGES)) {
        const AttNoteGes *att = dynamic_cast<const AttNoteGes *>(element);
        assert(att);
        if (att->HasExtremis()) {
            attributes->push_back(std::make_pair("extremis", att->NoteGesExtremisToStr(att->GetExtremis())));
        }
        if (att->HasOctGes()) {
            attributes->push_back(std::make_pair("oct.ges", att->IntToStr(att->GetOctGes())));
        }
        if (att->HasPnameGes()) {
            attributes->push_back(std::make_pair("pname.ges", att->PitchnameToStr(att->GetPnameGes())));
        }
        if (att->HasPnum()) {
            attributes->push_back(std::make_pair("pnum", att->IntToStr(att->GetPnum())));
        }
    }
    if (element->HasAttClass(ATT_SCOREDEFGES)) {
        const AttScoreDefGes *att = dynamic_cast<const AttScoreDefGes *>(element);
        assert(att);
        if (att->HasTunePname()) {
            attributes->push_back(std::make_pair("tune.pname", att->PitchnameToStr(att->GetTunePname())));
        }
        if (att->HasTuneHz()) {
            attributes->push_back(std::make_pair("tune.Hz", att->DblToStr(att->GetTuneHz())));
        }
        if (att->HasTuneTemper()) {
            attributes->push_back(std::make_pair("tune.temper", att->TemperamentToStr(att->GetTuneTemper())));
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMPGESTURAL)) {
        const AttTimestampGestural *att = dynamic_cast<const AttTimestampGestural *>(element);
        assert(att);
        if (att->HasTstampGes()) {
            attributes->push_back(std::make_pair("tstamp.ges", att->DurationToStr(att->GetTstampGes())));
        }
        if (att->HasTstampReal()) {
            attributes->push_back(std::make_pair("tstamp.real", att->StrToStr(att->GetTstampReal())));
        }
    }
}

} // vrv namespace
