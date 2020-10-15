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
    m_dotsGes = -1;
    m_durMetrical = 0.0;
    m_durPpq = 0;
    m_durReal = 0.0;
    m_durRecip = "";
}

bool AttDurationGestural::ReadDurationGestural(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dur.ges")) {
        this->SetDurGes(StrToDuration(element.attribute("dur.ges").value()));
        element.remove_attribute("dur.ges");
        hasAttribute = true;
    }
    if (element.attribute("dots.ges")) {
        this->SetDotsGes(StrToInt(element.attribute("dots.ges").value()));
        element.remove_attribute("dots.ges");
        hasAttribute = true;
    }
    if (element.attribute("dur.metrical")) {
        this->SetDurMetrical(StrToDbl(element.attribute("dur.metrical").value()));
        element.remove_attribute("dur.metrical");
        hasAttribute = true;
    }
    if (element.attribute("dur.ppq")) {
        this->SetDurPpq(StrToInt(element.attribute("dur.ppq").value()));
        element.remove_attribute("dur.ppq");
        hasAttribute = true;
    }
    if (element.attribute("dur.real")) {
        this->SetDurReal(StrToDbl(element.attribute("dur.real").value()));
        element.remove_attribute("dur.real");
        hasAttribute = true;
    }
    if (element.attribute("dur.recip")) {
        this->SetDurRecip(StrToStr(element.attribute("dur.recip").value()));
        element.remove_attribute("dur.recip");
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
    if (this->HasDotsGes()) {
        element.append_attribute("dots.ges") = IntToStr(this->GetDotsGes()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDurMetrical()) {
        element.append_attribute("dur.metrical") = DblToStr(this->GetDurMetrical()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDurPpq()) {
        element.append_attribute("dur.ppq") = IntToStr(this->GetDurPpq()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDurReal()) {
        element.append_attribute("dur.real") = DblToStr(this->GetDurReal()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDurRecip()) {
        element.append_attribute("dur.recip") = StrToStr(this->GetDurRecip()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDurationGestural::HasDurGes() const
{
    return (m_durGes != DURATION_NONE);
}

bool AttDurationGestural::HasDotsGes() const
{
    return (m_dotsGes != -1);
}

bool AttDurationGestural::HasDurMetrical() const
{
    return (m_durMetrical != 0.0);
}

bool AttDurationGestural::HasDurPpq() const
{
    return (m_durPpq != 0);
}

bool AttDurationGestural::HasDurReal() const
{
    return (m_durReal != 0.0);
}

bool AttDurationGestural::HasDurRecip() const
{
    return (m_durRecip != "");
}

/* include <attdur.recip> */

//----------------------------------------------------------------------------
// AttNcGes
//----------------------------------------------------------------------------

AttNcGes::AttNcGes() : Att()
{
    ResetNcGes();
}

AttNcGes::~AttNcGes()
{
}

void AttNcGes::ResetNcGes()
{
    m_octGes = -127;
    m_pnameGes = PITCHNAME_NONE;
    m_pnum = 0;
}

bool AttNcGes::ReadNcGes(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("oct.ges")) {
        this->SetOctGes(StrToOctave(element.attribute("oct.ges").value()));
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

bool AttNcGes::WriteNcGes(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOctGes()) {
        element.append_attribute("oct.ges") = OctaveToStr(this->GetOctGes()).c_str();
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

bool AttNcGes::HasOctGes() const
{
    return (m_octGes != -127);
}

bool AttNcGes::HasPnameGes() const
{
    return (m_pnameGes != PITCHNAME_NONE);
}

bool AttNcGes::HasPnum() const
{
    return (m_pnum != 0);
}

/* include <attpnum> */

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
    m_octGes = -127;
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
        this->SetOctGes(StrToOctave(element.attribute("oct.ges").value()));
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
        element.append_attribute("oct.ges") = OctaveToStr(this->GetOctGes()).c_str();
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
    return (m_octGes != -127);
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
// AttSectionGes
//----------------------------------------------------------------------------

AttSectionGes::AttSectionGes() : Att()
{
    ResetSectionGes();
}

AttSectionGes::~AttSectionGes()
{
}

void AttSectionGes::ResetSectionGes()
{
    m_attacca = BOOLEAN_NONE;
}

bool AttSectionGes::ReadSectionGes(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("attacca")) {
        this->SetAttacca(StrToBoolean(element.attribute("attacca").value()));
        element.remove_attribute("attacca");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSectionGes::WriteSectionGes(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAttacca()) {
        element.append_attribute("attacca") = BooleanToStr(this->GetAttacca()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSectionGes::HasAttacca() const
{
    return (m_attacca != BOOLEAN_NONE);
}

/* include <attattacca> */

//----------------------------------------------------------------------------
// AttSoundLocation
//----------------------------------------------------------------------------

AttSoundLocation::AttSoundLocation() : Att()
{
    ResetSoundLocation();
}

AttSoundLocation::~AttSoundLocation()
{
}

void AttSoundLocation::ResetSoundLocation()
{
    m_azimuth = 0.0;
    m_elevation = 0.0;
}

bool AttSoundLocation::ReadSoundLocation(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("azimuth")) {
        this->SetAzimuth(StrToDbl(element.attribute("azimuth").value()));
        element.remove_attribute("azimuth");
        hasAttribute = true;
    }
    if (element.attribute("elevation")) {
        this->SetElevation(StrToDbl(element.attribute("elevation").value()));
        element.remove_attribute("elevation");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSoundLocation::WriteSoundLocation(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAzimuth()) {
        element.append_attribute("azimuth") = DblToStr(this->GetAzimuth()).c_str();
        wroteAttribute = true;
    }
    if (this->HasElevation()) {
        element.append_attribute("elevation") = DblToStr(this->GetElevation()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSoundLocation::HasAzimuth() const
{
    return (m_azimuth != 0.0);
}

bool AttSoundLocation::HasElevation() const
{
    return (m_elevation != 0.0);
}

/* include <attelevation> */

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
    m_tstampGes = 0.0;
    m_tstampReal = "";
}

bool AttTimestampGestural::ReadTimestampGestural(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tstamp.ges")) {
        this->SetTstampGes(StrToDbl(element.attribute("tstamp.ges").value()));
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
        element.append_attribute("tstamp.ges") = DblToStr(this->GetTstampGes()).c_str();
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
    return (m_tstampGes != 0.0);
}

bool AttTimestampGestural::HasTstampReal() const
{
    return (m_tstampReal != "");
}

/* include <atttstamp.real> */

//----------------------------------------------------------------------------
// AttTimestamp2Gestural
//----------------------------------------------------------------------------

AttTimestamp2Gestural::AttTimestamp2Gestural() : Att()
{
    ResetTimestamp2Gestural();
}

AttTimestamp2Gestural::~AttTimestamp2Gestural()
{
}

void AttTimestamp2Gestural::ResetTimestamp2Gestural()
{
    m_tstamp2Ges = std::make_pair(-1, -1.0);
    m_tstamp2Real = "";
}

bool AttTimestamp2Gestural::ReadTimestamp2Gestural(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tstamp2.ges")) {
        this->SetTstamp2Ges(StrToMeasurebeat(element.attribute("tstamp2.ges").value()));
        element.remove_attribute("tstamp2.ges");
        hasAttribute = true;
    }
    if (element.attribute("tstamp2.real")) {
        this->SetTstamp2Real(StrToStr(element.attribute("tstamp2.real").value()));
        element.remove_attribute("tstamp2.real");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestamp2Gestural::WriteTimestamp2Gestural(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTstamp2Ges()) {
        element.append_attribute("tstamp2.ges") = MeasurebeatToStr(this->GetTstamp2Ges()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTstamp2Real()) {
        element.append_attribute("tstamp2.real") = StrToStr(this->GetTstamp2Real()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimestamp2Gestural::HasTstamp2Ges() const
{
    return (m_tstamp2Ges != std::make_pair(-1, -1.0));
}

bool AttTimestamp2Gestural::HasTstamp2Real() const
{
    return (m_tstamp2Real != "");
}

/* include <atttstamp2.real> */

bool Att::SetGestural(Object *element, const std::string &attrType, const std::string &attrValue)
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
        if (attrType == "dots.ges") {
            att->SetDotsGes(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "dur.metrical") {
            att->SetDurMetrical(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "dur.ppq") {
            att->SetDurPpq(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "dur.real") {
            att->SetDurReal(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "dur.recip") {
            att->SetDurRecip(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NCGES)) {
        AttNcGes *att = dynamic_cast<AttNcGes *>(element);
        assert(att);
        if (attrType == "oct.ges") {
            att->SetOctGes(att->StrToOctave(attrValue));
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
    if (element->HasAttClass(ATT_NOTEGES)) {
        AttNoteGes *att = dynamic_cast<AttNoteGes *>(element);
        assert(att);
        if (attrType == "extremis") {
            att->SetExtremis(att->StrToNoteGesExtremis(attrValue));
            return true;
        }
        if (attrType == "oct.ges") {
            att->SetOctGes(att->StrToOctave(attrValue));
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
    if (element->HasAttClass(ATT_SECTIONGES)) {
        AttSectionGes *att = dynamic_cast<AttSectionGes *>(element);
        assert(att);
        if (attrType == "attacca") {
            att->SetAttacca(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SOUNDLOCATION)) {
        AttSoundLocation *att = dynamic_cast<AttSoundLocation *>(element);
        assert(att);
        if (attrType == "azimuth") {
            att->SetAzimuth(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "elevation") {
            att->SetElevation(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMPGESTURAL)) {
        AttTimestampGestural *att = dynamic_cast<AttTimestampGestural *>(element);
        assert(att);
        if (attrType == "tstamp.ges") {
            att->SetTstampGes(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "tstamp.real") {
            att->SetTstampReal(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMP2GESTURAL)) {
        AttTimestamp2Gestural *att = dynamic_cast<AttTimestamp2Gestural *>(element);
        assert(att);
        if (attrType == "tstamp2.ges") {
            att->SetTstamp2Ges(att->StrToMeasurebeat(attrValue));
            return true;
        }
        if (attrType == "tstamp2.real") {
            att->SetTstamp2Real(att->StrToStr(attrValue));
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
        if (att->HasDotsGes()) {
            attributes->push_back(std::make_pair("dots.ges", att->IntToStr(att->GetDotsGes())));
        }
        if (att->HasDurMetrical()) {
            attributes->push_back(std::make_pair("dur.metrical", att->DblToStr(att->GetDurMetrical())));
        }
        if (att->HasDurPpq()) {
            attributes->push_back(std::make_pair("dur.ppq", att->IntToStr(att->GetDurPpq())));
        }
        if (att->HasDurReal()) {
            attributes->push_back(std::make_pair("dur.real", att->DblToStr(att->GetDurReal())));
        }
        if (att->HasDurRecip()) {
            attributes->push_back(std::make_pair("dur.recip", att->StrToStr(att->GetDurRecip())));
        }
    }
    if (element->HasAttClass(ATT_NCGES)) {
        const AttNcGes *att = dynamic_cast<const AttNcGes *>(element);
        assert(att);
        if (att->HasOctGes()) {
            attributes->push_back(std::make_pair("oct.ges", att->OctaveToStr(att->GetOctGes())));
        }
        if (att->HasPnameGes()) {
            attributes->push_back(std::make_pair("pname.ges", att->PitchnameToStr(att->GetPnameGes())));
        }
        if (att->HasPnum()) {
            attributes->push_back(std::make_pair("pnum", att->IntToStr(att->GetPnum())));
        }
    }
    if (element->HasAttClass(ATT_NOTEGES)) {
        const AttNoteGes *att = dynamic_cast<const AttNoteGes *>(element);
        assert(att);
        if (att->HasExtremis()) {
            attributes->push_back(std::make_pair("extremis", att->NoteGesExtremisToStr(att->GetExtremis())));
        }
        if (att->HasOctGes()) {
            attributes->push_back(std::make_pair("oct.ges", att->OctaveToStr(att->GetOctGes())));
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
    if (element->HasAttClass(ATT_SECTIONGES)) {
        const AttSectionGes *att = dynamic_cast<const AttSectionGes *>(element);
        assert(att);
        if (att->HasAttacca()) {
            attributes->push_back(std::make_pair("attacca", att->BooleanToStr(att->GetAttacca())));
        }
    }
    if (element->HasAttClass(ATT_SOUNDLOCATION)) {
        const AttSoundLocation *att = dynamic_cast<const AttSoundLocation *>(element);
        assert(att);
        if (att->HasAzimuth()) {
            attributes->push_back(std::make_pair("azimuth", att->DblToStr(att->GetAzimuth())));
        }
        if (att->HasElevation()) {
            attributes->push_back(std::make_pair("elevation", att->DblToStr(att->GetElevation())));
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMPGESTURAL)) {
        const AttTimestampGestural *att = dynamic_cast<const AttTimestampGestural *>(element);
        assert(att);
        if (att->HasTstampGes()) {
            attributes->push_back(std::make_pair("tstamp.ges", att->DblToStr(att->GetTstampGes())));
        }
        if (att->HasTstampReal()) {
            attributes->push_back(std::make_pair("tstamp.real", att->StrToStr(att->GetTstampReal())));
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMP2GESTURAL)) {
        const AttTimestamp2Gestural *att = dynamic_cast<const AttTimestamp2Gestural *>(element);
        assert(att);
        if (att->HasTstamp2Ges()) {
            attributes->push_back(std::make_pair("tstamp2.ges", att->MeasurebeatToStr(att->GetTstamp2Ges())));
        }
        if (att->HasTstamp2Real()) {
            attributes->push_back(std::make_pair("tstamp2.real", att->StrToStr(att->GetTstamp2Real())));
        }
    }
}

} // vrv namespace
