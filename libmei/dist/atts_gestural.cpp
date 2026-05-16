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

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttAccidentalGes
//----------------------------------------------------------------------------

AttAccidentalGes::AttAccidentalGes() : Att()
{
    this->ResetAccidentalGes();
}

void AttAccidentalGes::ResetAccidentalGes()
{
    m_accidGes = ACCIDENTAL_GESTURAL_NONE;
}

bool AttAccidentalGes::ReadAccidentalGes(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("accid.ges")) {
        this->SetAccidGes(StrToAccidentalGestural(element.attribute("accid.ges").value()));
        if (removeAttr) element.remove_attribute("accid.ges");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAccidentalGes::WriteAccidentalGes(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAccidGes()) {
        element.append_attribute("accid.ges") = AccidentalGesturalToStr(this->GetAccidGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAccidentalGes::HasAccidGes() const
{
    return (m_accidGes != ACCIDENTAL_GESTURAL_NONE);
}

//----------------------------------------------------------------------------
// AttArticulationGes
//----------------------------------------------------------------------------

AttArticulationGes::AttArticulationGes() : Att()
{
    this->ResetArticulationGes();
}

void AttArticulationGes::ResetArticulationGes()
{
    m_articGes = std::vector<data_ARTICULATION>();
}

bool AttArticulationGes::ReadArticulationGes(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("artic.ges")) {
        this->SetArticGes(StrToArticulationList(element.attribute("artic.ges").value()));
        if (removeAttr) element.remove_attribute("artic.ges");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArticulationGes::WriteArticulationGes(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasArticGes()) {
        element.append_attribute("artic.ges") = ArticulationListToStr(this->GetArticGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttArticulationGes::HasArticGes() const
{
    return (m_articGes != std::vector<data_ARTICULATION>());
}

//----------------------------------------------------------------------------
// AttAttacking
//----------------------------------------------------------------------------

AttAttacking::AttAttacking() : Att()
{
    this->ResetAttacking();
}

void AttAttacking::ResetAttacking()
{
    m_attacca = BOOLEAN_NONE;
}

bool AttAttacking::ReadAttacking(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("attacca")) {
        this->SetAttacca(StrToBoolean(element.attribute("attacca").value()));
        if (removeAttr) element.remove_attribute("attacca");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAttacking::WriteAttacking(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAttacca()) {
        element.append_attribute("attacca") = BooleanToStr(this->GetAttacca()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAttacking::HasAttacca() const
{
    return (m_attacca != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttBendGes
//----------------------------------------------------------------------------

AttBendGes::AttBendGes() : Att()
{
    this->ResetBendGes();
}

void AttBendGes::ResetBendGes()
{
    m_amount = 0.0;
}

bool AttBendGes::ReadBendGes(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("amount")) {
        this->SetAmount(StrToDbl(element.attribute("amount").value()));
        if (removeAttr) element.remove_attribute("amount");
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

//----------------------------------------------------------------------------
// AttDurationGes
//----------------------------------------------------------------------------

AttDurationGes::AttDurationGes() : Att()
{
    this->ResetDurationGes();
}

void AttDurationGes::ResetDurationGes()
{
    m_durGes = DURATION_NONE;
    m_dotsGes = MEI_UNSET;
    m_durMetrical = 0.0;
    m_durPpq = MEI_UNSET;
    m_durReal = 0.0;
    m_durRecip = "";
}

bool AttDurationGes::ReadDurationGes(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dur.ges")) {
        this->SetDurGes(StrToDuration(element.attribute("dur.ges").value()));
        if (removeAttr) element.remove_attribute("dur.ges");
        hasAttribute = true;
    }
    if (element.attribute("dots.ges")) {
        this->SetDotsGes(StrToInt(element.attribute("dots.ges").value()));
        if (removeAttr) element.remove_attribute("dots.ges");
        hasAttribute = true;
    }
    if (element.attribute("dur.metrical")) {
        this->SetDurMetrical(StrToDbl(element.attribute("dur.metrical").value()));
        if (removeAttr) element.remove_attribute("dur.metrical");
        hasAttribute = true;
    }
    if (element.attribute("dur.ppq")) {
        this->SetDurPpq(StrToInt(element.attribute("dur.ppq").value()));
        if (removeAttr) element.remove_attribute("dur.ppq");
        hasAttribute = true;
    }
    if (element.attribute("dur.real")) {
        this->SetDurReal(StrToDbl(element.attribute("dur.real").value()));
        if (removeAttr) element.remove_attribute("dur.real");
        hasAttribute = true;
    }
    if (element.attribute("dur.recip")) {
        this->SetDurRecip(StrToStr(element.attribute("dur.recip").value()));
        if (removeAttr) element.remove_attribute("dur.recip");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationGes::WriteDurationGes(pugi::xml_node element)
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

bool AttDurationGes::HasDurGes() const
{
    return (m_durGes != DURATION_NONE);
}

bool AttDurationGes::HasDotsGes() const
{
    return (m_dotsGes != MEI_UNSET);
}

bool AttDurationGes::HasDurMetrical() const
{
    return (m_durMetrical != 0.0);
}

bool AttDurationGes::HasDurPpq() const
{
    return (m_durPpq != MEI_UNSET);
}

bool AttDurationGes::HasDurReal() const
{
    return (m_durReal != 0.0);
}

bool AttDurationGes::HasDurRecip() const
{
    return (m_durRecip != "");
}

//----------------------------------------------------------------------------
// AttNoteGes
//----------------------------------------------------------------------------

AttNoteGes::AttNoteGes() : Att()
{
    this->ResetNoteGes();
}

void AttNoteGes::ResetNoteGes()
{
    m_extremis = noteGes_EXTREMIS_NONE;
}

bool AttNoteGes::ReadNoteGes(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("extremis")) {
        this->SetExtremis(StrToNoteGesExtremis(element.attribute("extremis").value()));
        if (removeAttr) element.remove_attribute("extremis");
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
    return wroteAttribute;
}

bool AttNoteGes::HasExtremis() const
{
    return (m_extremis != noteGes_EXTREMIS_NONE);
}

//----------------------------------------------------------------------------
// AttOrnamentAccidGes
//----------------------------------------------------------------------------

AttOrnamentAccidGes::AttOrnamentAccidGes() : Att()
{
    this->ResetOrnamentAccidGes();
}

void AttOrnamentAccidGes::ResetOrnamentAccidGes()
{
    m_accidupperGes = ACCIDENTAL_GESTURAL_NONE;
    m_accidlowerGes = ACCIDENTAL_GESTURAL_NONE;
}

bool AttOrnamentAccidGes::ReadOrnamentAccidGes(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("accidupper.ges")) {
        this->SetAccidupperGes(StrToAccidentalGestural(element.attribute("accidupper.ges").value()));
        if (removeAttr) element.remove_attribute("accidupper.ges");
        hasAttribute = true;
    }
    if (element.attribute("accidlower.ges")) {
        this->SetAccidlowerGes(StrToAccidentalGestural(element.attribute("accidlower.ges").value()));
        if (removeAttr) element.remove_attribute("accidlower.ges");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOrnamentAccidGes::WriteOrnamentAccidGes(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAccidupperGes()) {
        element.append_attribute("accidupper.ges") = AccidentalGesturalToStr(this->GetAccidupperGes()).c_str();
        wroteAttribute = true;
    }
    if (this->HasAccidlowerGes()) {
        element.append_attribute("accidlower.ges") = AccidentalGesturalToStr(this->GetAccidlowerGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOrnamentAccidGes::HasAccidupperGes() const
{
    return (m_accidupperGes != ACCIDENTAL_GESTURAL_NONE);
}

bool AttOrnamentAccidGes::HasAccidlowerGes() const
{
    return (m_accidlowerGes != ACCIDENTAL_GESTURAL_NONE);
}

//----------------------------------------------------------------------------
// AttPitchGes
//----------------------------------------------------------------------------

AttPitchGes::AttPitchGes() : Att()
{
    this->ResetPitchGes();
}

void AttPitchGes::ResetPitchGes()
{
    m_octGes = MEI_UNSET_OCT;
    m_pnameGes = PITCHNAME_NONE;
    m_pnum = MEI_UNSET;
}

bool AttPitchGes::ReadPitchGes(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("oct.ges")) {
        this->SetOctGes(StrToOctave(element.attribute("oct.ges").value()));
        if (removeAttr) element.remove_attribute("oct.ges");
        hasAttribute = true;
    }
    if (element.attribute("pname.ges")) {
        this->SetPnameGes(StrToPitchname(element.attribute("pname.ges").value()));
        if (removeAttr) element.remove_attribute("pname.ges");
        hasAttribute = true;
    }
    if (element.attribute("pnum")) {
        this->SetPnum(StrToInt(element.attribute("pnum").value()));
        if (removeAttr) element.remove_attribute("pnum");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPitchGes::WritePitchGes(pugi::xml_node element)
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

bool AttPitchGes::HasOctGes() const
{
    return (m_octGes != MEI_UNSET_OCT);
}

bool AttPitchGes::HasPnameGes() const
{
    return (m_pnameGes != PITCHNAME_NONE);
}

bool AttPitchGes::HasPnum() const
{
    return (m_pnum != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttSoundLocation
//----------------------------------------------------------------------------

AttSoundLocation::AttSoundLocation() : Att()
{
    this->ResetSoundLocation();
}

void AttSoundLocation::ResetSoundLocation()
{
    m_azimuth = MEI_UNSET;
    m_elevation = MEI_UNSET;
}

bool AttSoundLocation::ReadSoundLocation(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("azimuth")) {
        this->SetAzimuth(StrToDegrees(element.attribute("azimuth").value()));
        if (removeAttr) element.remove_attribute("azimuth");
        hasAttribute = true;
    }
    if (element.attribute("elevation")) {
        this->SetElevation(StrToDegrees(element.attribute("elevation").value()));
        if (removeAttr) element.remove_attribute("elevation");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSoundLocation::WriteSoundLocation(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAzimuth()) {
        element.append_attribute("azimuth") = DegreesToStr(this->GetAzimuth()).c_str();
        wroteAttribute = true;
    }
    if (this->HasElevation()) {
        element.append_attribute("elevation") = DegreesToStr(this->GetElevation()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSoundLocation::HasAzimuth() const
{
    return (m_azimuth != MEI_UNSET);
}

bool AttSoundLocation::HasElevation() const
{
    return (m_elevation != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttTimestampGes
//----------------------------------------------------------------------------

AttTimestampGes::AttTimestampGes() : Att()
{
    this->ResetTimestampGes();
}

void AttTimestampGes::ResetTimestampGes()
{
    m_tstampGes = 0.0;
    m_tstampReal = "";
}

bool AttTimestampGes::ReadTimestampGes(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("tstamp.ges")) {
        this->SetTstampGes(StrToDbl(element.attribute("tstamp.ges").value()));
        if (removeAttr) element.remove_attribute("tstamp.ges");
        hasAttribute = true;
    }
    if (element.attribute("tstamp.real")) {
        this->SetTstampReal(StrToStr(element.attribute("tstamp.real").value()));
        if (removeAttr) element.remove_attribute("tstamp.real");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestampGes::WriteTimestampGes(pugi::xml_node element)
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

bool AttTimestampGes::HasTstampGes() const
{
    return (m_tstampGes != 0.0);
}

bool AttTimestampGes::HasTstampReal() const
{
    return (m_tstampReal != "");
}

//----------------------------------------------------------------------------
// AttTimestamp2Ges
//----------------------------------------------------------------------------

AttTimestamp2Ges::AttTimestamp2Ges() : Att()
{
    this->ResetTimestamp2Ges();
}

void AttTimestamp2Ges::ResetTimestamp2Ges()
{
    m_tstamp2Ges = std::make_pair(-1, -1.0);
    m_tstamp2Real = "";
}

bool AttTimestamp2Ges::ReadTimestamp2Ges(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("tstamp2.ges")) {
        this->SetTstamp2Ges(StrToMeasurebeat(element.attribute("tstamp2.ges").value()));
        if (removeAttr) element.remove_attribute("tstamp2.ges");
        hasAttribute = true;
    }
    if (element.attribute("tstamp2.real")) {
        this->SetTstamp2Real(StrToStr(element.attribute("tstamp2.real").value()));
        if (removeAttr) element.remove_attribute("tstamp2.real");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestamp2Ges::WriteTimestamp2Ges(pugi::xml_node element)
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

bool AttTimestamp2Ges::HasTstamp2Ges() const
{
    return (m_tstamp2Ges != std::make_pair(-1, -1.0));
}

bool AttTimestamp2Ges::HasTstamp2Real() const
{
    return (m_tstamp2Real != "");
}

} // namespace vrv
