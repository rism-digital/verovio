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

#include "attmodule.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "object.h"

#include "atts_mei.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mei
//----------------------------------------------------------------------------

bool AttModule::SetMei(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_NOTATIONTYPE)) {
        AttNotationType *att = dynamic_cast<AttNotationType *>(element);
        assert(att);
        if (attrType == "notationtype") {
            att->SetNotationtype(att->StrToNotationtype(attrValue));
            return true;
        }
        if (attrType == "notationsubtype") {
            att->SetNotationsubtype(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetMei(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_NOTATIONTYPE)) {
        const AttNotationType *att = dynamic_cast<const AttNotationType *>(element);
        assert(att);
        if (att->HasNotationtype()) {
            attributes->push_back({ "notationtype", att->NotationtypeToStr(att->GetNotationtype()) });
        }
        if (att->HasNotationsubtype()) {
            attributes->push_back({ "notationsubtype", att->StrToStr(att->GetNotationsubtype()) });
        }
    }
}

} // namespace vrv

#include "atts_analytical.h"

namespace vrv {

//----------------------------------------------------------------------------
// Analytical
//----------------------------------------------------------------------------

bool AttModule::SetAnalytical(Object *element, const std::string &attrType, const std::string &attrValue)
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

void AttModule::GetAnalytical(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_HARMANL)) {
        const AttHarmAnl *att = dynamic_cast<const AttHarmAnl *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->HarmAnlFormToStr(att->GetForm()) });
        }
    }
    if (element->HasAttClass(ATT_HARMONICFUNCTION)) {
        const AttHarmonicFunction *att = dynamic_cast<const AttHarmonicFunction *>(element);
        assert(att);
        if (att->HasDeg()) {
            attributes->push_back({ "deg", att->StrToStr(att->GetDeg()) });
        }
    }
    if (element->HasAttClass(ATT_INTERVALHARMONIC)) {
        const AttIntervalHarmonic *att = dynamic_cast<const AttIntervalHarmonic *>(element);
        assert(att);
        if (att->HasInth()) {
            attributes->push_back({ "inth", att->StrToStr(att->GetInth()) });
        }
    }
    if (element->HasAttClass(ATT_INTERVALMELODIC)) {
        const AttIntervalMelodic *att = dynamic_cast<const AttIntervalMelodic *>(element);
        assert(att);
        if (att->HasIntm()) {
            attributes->push_back({ "intm", att->StrToStr(att->GetIntm()) });
        }
    }
    if (element->HasAttClass(ATT_KEYSIGANL)) {
        const AttKeySigAnl *att = dynamic_cast<const AttKeySigAnl *>(element);
        assert(att);
        if (att->HasMode()) {
            attributes->push_back({ "mode", att->ModeToStr(att->GetMode()) });
        }
    }
    if (element->HasAttClass(ATT_KEYSIGDEFAULTANL)) {
        const AttKeySigDefaultAnl *att = dynamic_cast<const AttKeySigDefaultAnl *>(element);
        assert(att);
        if (att->HasKeyAccid()) {
            attributes->push_back({ "key.accid", att->AccidentalGesturalToStr(att->GetKeyAccid()) });
        }
        if (att->HasKeyMode()) {
            attributes->push_back({ "key.mode", att->ModeToStr(att->GetKeyMode()) });
        }
        if (att->HasKeyPname()) {
            attributes->push_back({ "key.pname", att->PitchnameToStr(att->GetKeyPname()) });
        }
    }
    if (element->HasAttClass(ATT_MELODICFUNCTION)) {
        const AttMelodicFunction *att = dynamic_cast<const AttMelodicFunction *>(element);
        assert(att);
        if (att->HasMfunc()) {
            attributes->push_back({ "mfunc", att->MelodicfunctionToStr(att->GetMfunc()) });
        }
    }
    if (element->HasAttClass(ATT_PITCHCLASS)) {
        const AttPitchClass *att = dynamic_cast<const AttPitchClass *>(element);
        assert(att);
        if (att->HasPclass()) {
            attributes->push_back({ "pclass", att->IntToStr(att->GetPclass()) });
        }
    }
    if (element->HasAttClass(ATT_SOLFA)) {
        const AttSolfa *att = dynamic_cast<const AttSolfa *>(element);
        assert(att);
        if (att->HasPsolfa()) {
            attributes->push_back({ "psolfa", att->StrToStr(att->GetPsolfa()) });
        }
    }
}

} // namespace vrv

#include "atts_cmn.h"

namespace vrv {

//----------------------------------------------------------------------------
// Cmn
//----------------------------------------------------------------------------

bool AttModule::SetCmn(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_ARPEGLOG)) {
        AttArpegLog *att = dynamic_cast<AttArpegLog *>(element);
        assert(att);
        if (attrType == "order") {
            att->SetOrder(att->StrToArpegLogOrder(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BTREMLOG)) {
        AttBTremLog *att = dynamic_cast<AttBTremLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToBTremLogForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMPRESENT)) {
        AttBeamPresent *att = dynamic_cast<AttBeamPresent *>(element);
        assert(att);
        if (attrType == "beam") {
            att->SetBeam(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMREND)) {
        AttBeamRend *att = dynamic_cast<AttBeamRend *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToBeamRendForm(attrValue));
            return true;
        }
        if (attrType == "place") {
            att->SetPlace(att->StrToBeamplace(attrValue));
            return true;
        }
        if (attrType == "slash") {
            att->SetSlash(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "slope") {
            att->SetSlope(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMSECONDARY)) {
        AttBeamSecondary *att = dynamic_cast<AttBeamSecondary *>(element);
        assert(att);
        if (attrType == "breaksec") {
            att->SetBreaksec(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMEDWITH)) {
        AttBeamedWith *att = dynamic_cast<AttBeamedWith *>(element);
        assert(att);
        if (attrType == "beam.with") {
            att->SetBeamWith(att->StrToNeighboringlayer(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMINGLOG)) {
        AttBeamingLog *att = dynamic_cast<AttBeamingLog *>(element);
        assert(att);
        if (attrType == "beam.group") {
            att->SetBeamGroup(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "beam.rests") {
            att->SetBeamRests(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEATRPTLOG)) {
        AttBeatRptLog *att = dynamic_cast<AttBeatRptLog *>(element);
        assert(att);
        if (attrType == "beatdef") {
            att->SetBeatdef(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BRACKETSPANLOG)) {
        AttBracketSpanLog *att = dynamic_cast<AttBracketSpanLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CUTOUT)) {
        AttCutout *att = dynamic_cast<AttCutout *>(element);
        assert(att);
        if (attrType == "cutout") {
            att->SetCutout(att->StrToCutoutCutout(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_EXPANDABLE)) {
        AttExpandable *att = dynamic_cast<AttExpandable *>(element);
        assert(att);
        if (attrType == "expand") {
            att->SetExpand(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FTREMLOG)) {
        AttFTremLog *att = dynamic_cast<AttFTremLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToFTremLogForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_GLISSPRESENT)) {
        AttGlissPresent *att = dynamic_cast<AttGlissPresent *>(element);
        assert(att);
        if (attrType == "gliss") {
            att->SetGliss(att->StrToGlissando(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_GRACEGRPLOG)) {
        AttGraceGrpLog *att = dynamic_cast<AttGraceGrpLog *>(element);
        assert(att);
        if (attrType == "attach") {
            att->SetAttach(att->StrToGraceGrpLogAttach(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_GRACED)) {
        AttGraced *att = dynamic_cast<AttGraced *>(element);
        assert(att);
        if (attrType == "grace") {
            att->SetGrace(att->StrToGrace(attrValue));
            return true;
        }
        if (attrType == "grace.time") {
            att->SetGraceTime(att->StrToPercent(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HAIRPINLOG)) {
        AttHairpinLog *att = dynamic_cast<AttHairpinLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToHairpinLogForm(attrValue));
            return true;
        }
        if (attrType == "niente") {
            att->SetNiente(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HARPPEDALLOG)) {
        AttHarpPedalLog *att = dynamic_cast<AttHarpPedalLog *>(element);
        assert(att);
        if (attrType == "c") {
            att->SetC(att->StrToHarpPedalLogC(attrValue));
            return true;
        }
        if (attrType == "d") {
            att->SetD(att->StrToHarpPedalLogD(attrValue));
            return true;
        }
        if (attrType == "e") {
            att->SetE(att->StrToHarpPedalLogE(attrValue));
            return true;
        }
        if (attrType == "f") {
            att->SetF(att->StrToHarpPedalLogF(attrValue));
            return true;
        }
        if (attrType == "g") {
            att->SetG(att->StrToHarpPedalLogG(attrValue));
            return true;
        }
        if (attrType == "a") {
            att->SetA(att->StrToHarpPedalLogA(attrValue));
            return true;
        }
        if (attrType == "b") {
            att->SetB(att->StrToHarpPedalLogB(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LVPRESENT)) {
        AttLvPresent *att = dynamic_cast<AttLvPresent *>(element);
        assert(att);
        if (attrType == "lv") {
            att->SetLv(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MEASURELOG)) {
        AttMeasureLog *att = dynamic_cast<AttMeasureLog *>(element);
        assert(att);
        if (attrType == "left") {
            att->SetLeft(att->StrToBarrendition(attrValue));
            return true;
        }
        if (attrType == "right") {
            att->SetRight(att->StrToBarrendition(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERSIGGRPLOG)) {
        AttMeterSigGrpLog *att = dynamic_cast<AttMeterSigGrpLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToMeterSigGrpLogFunc(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NUMBERPLACEMENT)) {
        AttNumberPlacement *att = dynamic_cast<AttNumberPlacement *>(element);
        assert(att);
        if (attrType == "num.place") {
            att->SetNumPlace(att->StrToStaffrelBasic(attrValue));
            return true;
        }
        if (attrType == "num.visible") {
            att->SetNumVisible(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NUMBERED)) {
        AttNumbered *att = dynamic_cast<AttNumbered *>(element);
        assert(att);
        if (attrType == "num") {
            att->SetNum(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_OCTAVELOG)) {
        AttOctaveLog *att = dynamic_cast<AttOctaveLog *>(element);
        assert(att);
        if (attrType == "coll") {
            att->SetColl(att->StrToOctaveLogColl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PEDALLOG)) {
        AttPedalLog *att = dynamic_cast<AttPedalLog *>(element);
        assert(att);
        if (attrType == "dir") {
            att->SetDir(att->StrToPedalLogDir(attrValue));
            return true;
        }
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PIANOPEDALS)) {
        AttPianoPedals *att = dynamic_cast<AttPianoPedals *>(element);
        assert(att);
        if (attrType == "pedal.style") {
            att->SetPedalStyle(att->StrToPedalstyle(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_REHEARSAL)) {
        AttRehearsal *att = dynamic_cast<AttRehearsal *>(element);
        assert(att);
        if (attrType == "reh.enclose") {
            att->SetRehEnclose(att->StrToRehearsalRehenclose(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SCOREDEFVISCMN)) {
        AttScoreDefVisCmn *att = dynamic_cast<AttScoreDefVisCmn *>(element);
        assert(att);
        if (attrType == "grid.show") {
            att->SetGridShow(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SLURREND)) {
        AttSlurRend *att = dynamic_cast<AttSlurRend *>(element);
        assert(att);
        if (attrType == "slur.lform") {
            att->SetSlurLform(att->StrToLineform(attrValue));
            return true;
        }
        if (attrType == "slur.lwidth") {
            att->SetSlurLwidth(att->StrToLinewidth(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STEMSCMN)) {
        AttStemsCmn *att = dynamic_cast<AttStemsCmn *>(element);
        assert(att);
        if (attrType == "stem.with") {
            att->SetStemWith(att->StrToNeighboringlayer(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIEREND)) {
        AttTieRend *att = dynamic_cast<AttTieRend *>(element);
        assert(att);
        if (attrType == "tie.lform") {
            att->SetTieLform(att->StrToLineform(attrValue));
            return true;
        }
        if (attrType == "tie.lwidth") {
            att->SetTieLwidth(att->StrToLinewidth(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TREMMEASURED)) {
        AttTremMeasured *att = dynamic_cast<AttTremMeasured *>(element);
        assert(att);
        if (attrType == "unitdur") {
            att->SetUnitdur(att->StrToDuration(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetCmn(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ARPEGLOG)) {
        const AttArpegLog *att = dynamic_cast<const AttArpegLog *>(element);
        assert(att);
        if (att->HasOrder()) {
            attributes->push_back({ "order", att->ArpegLogOrderToStr(att->GetOrder()) });
        }
    }
    if (element->HasAttClass(ATT_BTREMLOG)) {
        const AttBTremLog *att = dynamic_cast<const AttBTremLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->BTremLogFormToStr(att->GetForm()) });
        }
    }
    if (element->HasAttClass(ATT_BEAMPRESENT)) {
        const AttBeamPresent *att = dynamic_cast<const AttBeamPresent *>(element);
        assert(att);
        if (att->HasBeam()) {
            attributes->push_back({ "beam", att->StrToStr(att->GetBeam()) });
        }
    }
    if (element->HasAttClass(ATT_BEAMREND)) {
        const AttBeamRend *att = dynamic_cast<const AttBeamRend *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->BeamRendFormToStr(att->GetForm()) });
        }
        if (att->HasPlace()) {
            attributes->push_back({ "place", att->BeamplaceToStr(att->GetPlace()) });
        }
        if (att->HasSlash()) {
            attributes->push_back({ "slash", att->BooleanToStr(att->GetSlash()) });
        }
        if (att->HasSlope()) {
            attributes->push_back({ "slope", att->DblToStr(att->GetSlope()) });
        }
    }
    if (element->HasAttClass(ATT_BEAMSECONDARY)) {
        const AttBeamSecondary *att = dynamic_cast<const AttBeamSecondary *>(element);
        assert(att);
        if (att->HasBreaksec()) {
            attributes->push_back({ "breaksec", att->IntToStr(att->GetBreaksec()) });
        }
    }
    if (element->HasAttClass(ATT_BEAMEDWITH)) {
        const AttBeamedWith *att = dynamic_cast<const AttBeamedWith *>(element);
        assert(att);
        if (att->HasBeamWith()) {
            attributes->push_back({ "beam.with", att->NeighboringlayerToStr(att->GetBeamWith()) });
        }
    }
    if (element->HasAttClass(ATT_BEAMINGLOG)) {
        const AttBeamingLog *att = dynamic_cast<const AttBeamingLog *>(element);
        assert(att);
        if (att->HasBeamGroup()) {
            attributes->push_back({ "beam.group", att->StrToStr(att->GetBeamGroup()) });
        }
        if (att->HasBeamRests()) {
            attributes->push_back({ "beam.rests", att->BooleanToStr(att->GetBeamRests()) });
        }
    }
    if (element->HasAttClass(ATT_BEATRPTLOG)) {
        const AttBeatRptLog *att = dynamic_cast<const AttBeatRptLog *>(element);
        assert(att);
        if (att->HasBeatdef()) {
            attributes->push_back({ "beatdef", att->DblToStr(att->GetBeatdef()) });
        }
    }
    if (element->HasAttClass(ATT_BRACKETSPANLOG)) {
        const AttBracketSpanLog *att = dynamic_cast<const AttBracketSpanLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back({ "func", att->StrToStr(att->GetFunc()) });
        }
    }
    if (element->HasAttClass(ATT_CUTOUT)) {
        const AttCutout *att = dynamic_cast<const AttCutout *>(element);
        assert(att);
        if (att->HasCutout()) {
            attributes->push_back({ "cutout", att->CutoutCutoutToStr(att->GetCutout()) });
        }
    }
    if (element->HasAttClass(ATT_EXPANDABLE)) {
        const AttExpandable *att = dynamic_cast<const AttExpandable *>(element);
        assert(att);
        if (att->HasExpand()) {
            attributes->push_back({ "expand", att->BooleanToStr(att->GetExpand()) });
        }
    }
    if (element->HasAttClass(ATT_FTREMLOG)) {
        const AttFTremLog *att = dynamic_cast<const AttFTremLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->FTremLogFormToStr(att->GetForm()) });
        }
    }
    if (element->HasAttClass(ATT_GLISSPRESENT)) {
        const AttGlissPresent *att = dynamic_cast<const AttGlissPresent *>(element);
        assert(att);
        if (att->HasGliss()) {
            attributes->push_back({ "gliss", att->GlissandoToStr(att->GetGliss()) });
        }
    }
    if (element->HasAttClass(ATT_GRACEGRPLOG)) {
        const AttGraceGrpLog *att = dynamic_cast<const AttGraceGrpLog *>(element);
        assert(att);
        if (att->HasAttach()) {
            attributes->push_back({ "attach", att->GraceGrpLogAttachToStr(att->GetAttach()) });
        }
    }
    if (element->HasAttClass(ATT_GRACED)) {
        const AttGraced *att = dynamic_cast<const AttGraced *>(element);
        assert(att);
        if (att->HasGrace()) {
            attributes->push_back({ "grace", att->GraceToStr(att->GetGrace()) });
        }
        if (att->HasGraceTime()) {
            attributes->push_back({ "grace.time", att->PercentToStr(att->GetGraceTime()) });
        }
    }
    if (element->HasAttClass(ATT_HAIRPINLOG)) {
        const AttHairpinLog *att = dynamic_cast<const AttHairpinLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->HairpinLogFormToStr(att->GetForm()) });
        }
        if (att->HasNiente()) {
            attributes->push_back({ "niente", att->BooleanToStr(att->GetNiente()) });
        }
    }
    if (element->HasAttClass(ATT_HARPPEDALLOG)) {
        const AttHarpPedalLog *att = dynamic_cast<const AttHarpPedalLog *>(element);
        assert(att);
        if (att->HasC()) {
            attributes->push_back({ "c", att->HarpPedalLogCToStr(att->GetC()) });
        }
        if (att->HasD()) {
            attributes->push_back({ "d", att->HarpPedalLogDToStr(att->GetD()) });
        }
        if (att->HasE()) {
            attributes->push_back({ "e", att->HarpPedalLogEToStr(att->GetE()) });
        }
        if (att->HasF()) {
            attributes->push_back({ "f", att->HarpPedalLogFToStr(att->GetF()) });
        }
        if (att->HasG()) {
            attributes->push_back({ "g", att->HarpPedalLogGToStr(att->GetG()) });
        }
        if (att->HasA()) {
            attributes->push_back({ "a", att->HarpPedalLogAToStr(att->GetA()) });
        }
        if (att->HasB()) {
            attributes->push_back({ "b", att->HarpPedalLogBToStr(att->GetB()) });
        }
    }
    if (element->HasAttClass(ATT_LVPRESENT)) {
        const AttLvPresent *att = dynamic_cast<const AttLvPresent *>(element);
        assert(att);
        if (att->HasLv()) {
            attributes->push_back({ "lv", att->BooleanToStr(att->GetLv()) });
        }
    }
    if (element->HasAttClass(ATT_MEASURELOG)) {
        const AttMeasureLog *att = dynamic_cast<const AttMeasureLog *>(element);
        assert(att);
        if (att->HasLeft()) {
            attributes->push_back({ "left", att->BarrenditionToStr(att->GetLeft()) });
        }
        if (att->HasRight()) {
            attributes->push_back({ "right", att->BarrenditionToStr(att->GetRight()) });
        }
    }
    if (element->HasAttClass(ATT_METERSIGGRPLOG)) {
        const AttMeterSigGrpLog *att = dynamic_cast<const AttMeterSigGrpLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back({ "func", att->MeterSigGrpLogFuncToStr(att->GetFunc()) });
        }
    }
    if (element->HasAttClass(ATT_NUMBERPLACEMENT)) {
        const AttNumberPlacement *att = dynamic_cast<const AttNumberPlacement *>(element);
        assert(att);
        if (att->HasNumPlace()) {
            attributes->push_back({ "num.place", att->StaffrelBasicToStr(att->GetNumPlace()) });
        }
        if (att->HasNumVisible()) {
            attributes->push_back({ "num.visible", att->BooleanToStr(att->GetNumVisible()) });
        }
    }
    if (element->HasAttClass(ATT_NUMBERED)) {
        const AttNumbered *att = dynamic_cast<const AttNumbered *>(element);
        assert(att);
        if (att->HasNum()) {
            attributes->push_back({ "num", att->IntToStr(att->GetNum()) });
        }
    }
    if (element->HasAttClass(ATT_OCTAVELOG)) {
        const AttOctaveLog *att = dynamic_cast<const AttOctaveLog *>(element);
        assert(att);
        if (att->HasColl()) {
            attributes->push_back({ "coll", att->OctaveLogCollToStr(att->GetColl()) });
        }
    }
    if (element->HasAttClass(ATT_PEDALLOG)) {
        const AttPedalLog *att = dynamic_cast<const AttPedalLog *>(element);
        assert(att);
        if (att->HasDir()) {
            attributes->push_back({ "dir", att->PedalLogDirToStr(att->GetDir()) });
        }
        if (att->HasFunc()) {
            attributes->push_back({ "func", att->StrToStr(att->GetFunc()) });
        }
    }
    if (element->HasAttClass(ATT_PIANOPEDALS)) {
        const AttPianoPedals *att = dynamic_cast<const AttPianoPedals *>(element);
        assert(att);
        if (att->HasPedalStyle()) {
            attributes->push_back({ "pedal.style", att->PedalstyleToStr(att->GetPedalStyle()) });
        }
    }
    if (element->HasAttClass(ATT_REHEARSAL)) {
        const AttRehearsal *att = dynamic_cast<const AttRehearsal *>(element);
        assert(att);
        if (att->HasRehEnclose()) {
            attributes->push_back({ "reh.enclose", att->RehearsalRehencloseToStr(att->GetRehEnclose()) });
        }
    }
    if (element->HasAttClass(ATT_SCOREDEFVISCMN)) {
        const AttScoreDefVisCmn *att = dynamic_cast<const AttScoreDefVisCmn *>(element);
        assert(att);
        if (att->HasGridShow()) {
            attributes->push_back({ "grid.show", att->BooleanToStr(att->GetGridShow()) });
        }
    }
    if (element->HasAttClass(ATT_SLURREND)) {
        const AttSlurRend *att = dynamic_cast<const AttSlurRend *>(element);
        assert(att);
        if (att->HasSlurLform()) {
            attributes->push_back({ "slur.lform", att->LineformToStr(att->GetSlurLform()) });
        }
        if (att->HasSlurLwidth()) {
            attributes->push_back({ "slur.lwidth", att->LinewidthToStr(att->GetSlurLwidth()) });
        }
    }
    if (element->HasAttClass(ATT_STEMSCMN)) {
        const AttStemsCmn *att = dynamic_cast<const AttStemsCmn *>(element);
        assert(att);
        if (att->HasStemWith()) {
            attributes->push_back({ "stem.with", att->NeighboringlayerToStr(att->GetStemWith()) });
        }
    }
    if (element->HasAttClass(ATT_TIEREND)) {
        const AttTieRend *att = dynamic_cast<const AttTieRend *>(element);
        assert(att);
        if (att->HasTieLform()) {
            attributes->push_back({ "tie.lform", att->LineformToStr(att->GetTieLform()) });
        }
        if (att->HasTieLwidth()) {
            attributes->push_back({ "tie.lwidth", att->LinewidthToStr(att->GetTieLwidth()) });
        }
    }
    if (element->HasAttClass(ATT_TREMMEASURED)) {
        const AttTremMeasured *att = dynamic_cast<const AttTremMeasured *>(element);
        assert(att);
        if (att->HasUnitdur()) {
            attributes->push_back({ "unitdur", att->DurationToStr(att->GetUnitdur()) });
        }
    }
}

} // namespace vrv

#include "atts_cmnornaments.h"

namespace vrv {

//----------------------------------------------------------------------------
// Cmnornaments
//----------------------------------------------------------------------------

bool AttModule::SetCmnornaments(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_MORDENTLOG)) {
        AttMordentLog *att = dynamic_cast<AttMordentLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToMordentLogForm(attrValue));
            return true;
        }
        if (attrType == "long") {
            att->SetLong(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ORNAMPRESENT)) {
        AttOrnamPresent *att = dynamic_cast<AttOrnamPresent *>(element);
        assert(att);
        if (attrType == "ornam") {
            att->SetOrnam(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ORNAMENTACCID)) {
        AttOrnamentAccid *att = dynamic_cast<AttOrnamentAccid *>(element);
        assert(att);
        if (attrType == "accidupper") {
            att->SetAccidupper(att->StrToAccidentalWritten(attrValue));
            return true;
        }
        if (attrType == "accidlower") {
            att->SetAccidlower(att->StrToAccidentalWritten(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TURNLOG)) {
        AttTurnLog *att = dynamic_cast<AttTurnLog *>(element);
        assert(att);
        if (attrType == "delayed") {
            att->SetDelayed(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "form") {
            att->SetForm(att->StrToTurnLogForm(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetCmnornaments(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_MORDENTLOG)) {
        const AttMordentLog *att = dynamic_cast<const AttMordentLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->MordentLogFormToStr(att->GetForm()) });
        }
        if (att->HasLong()) {
            attributes->push_back({ "long", att->BooleanToStr(att->GetLong()) });
        }
    }
    if (element->HasAttClass(ATT_ORNAMPRESENT)) {
        const AttOrnamPresent *att = dynamic_cast<const AttOrnamPresent *>(element);
        assert(att);
        if (att->HasOrnam()) {
            attributes->push_back({ "ornam", att->StrToStr(att->GetOrnam()) });
        }
    }
    if (element->HasAttClass(ATT_ORNAMENTACCID)) {
        const AttOrnamentAccid *att = dynamic_cast<const AttOrnamentAccid *>(element);
        assert(att);
        if (att->HasAccidupper()) {
            attributes->push_back({ "accidupper", att->AccidentalWrittenToStr(att->GetAccidupper()) });
        }
        if (att->HasAccidlower()) {
            attributes->push_back({ "accidlower", att->AccidentalWrittenToStr(att->GetAccidlower()) });
        }
    }
    if (element->HasAttClass(ATT_TURNLOG)) {
        const AttTurnLog *att = dynamic_cast<const AttTurnLog *>(element);
        assert(att);
        if (att->HasDelayed()) {
            attributes->push_back({ "delayed", att->BooleanToStr(att->GetDelayed()) });
        }
        if (att->HasForm()) {
            attributes->push_back({ "form", att->TurnLogFormToStr(att->GetForm()) });
        }
    }
}

} // namespace vrv

#include "atts_critapp.h"

namespace vrv {

//----------------------------------------------------------------------------
// Critapp
//----------------------------------------------------------------------------

bool AttModule::SetCritapp(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_CRIT)) {
        AttCrit *att = dynamic_cast<AttCrit *>(element);
        assert(att);
        if (attrType == "cause") {
            att->SetCause(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetCritapp(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_CRIT)) {
        const AttCrit *att = dynamic_cast<const AttCrit *>(element);
        assert(att);
        if (att->HasCause()) {
            attributes->push_back({ "cause", att->StrToStr(att->GetCause()) });
        }
    }
}

} // namespace vrv

#include "atts_edittrans.h"

namespace vrv {

//----------------------------------------------------------------------------
// Edittrans
//----------------------------------------------------------------------------

bool AttModule::SetEdittrans(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_AGENTIDENT)) {
        AttAgentIdent *att = dynamic_cast<AttAgentIdent *>(element);
        assert(att);
        if (attrType == "agent") {
            att->SetAgent(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_REASONIDENT)) {
        AttReasonIdent *att = dynamic_cast<AttReasonIdent *>(element);
        assert(att);
        if (attrType == "reason") {
            att->SetReason(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetEdittrans(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_AGENTIDENT)) {
        const AttAgentIdent *att = dynamic_cast<const AttAgentIdent *>(element);
        assert(att);
        if (att->HasAgent()) {
            attributes->push_back({ "agent", att->StrToStr(att->GetAgent()) });
        }
    }
    if (element->HasAttClass(ATT_REASONIDENT)) {
        const AttReasonIdent *att = dynamic_cast<const AttReasonIdent *>(element);
        assert(att);
        if (att->HasReason()) {
            attributes->push_back({ "reason", att->StrToStr(att->GetReason()) });
        }
    }
}

} // namespace vrv

#include "atts_externalsymbols.h"

namespace vrv {

//----------------------------------------------------------------------------
// Externalsymbols
//----------------------------------------------------------------------------

bool AttModule::SetExternalsymbols(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_EXTSYM)) {
        AttExtSym *att = dynamic_cast<AttExtSym *>(element);
        assert(att);
        if (attrType == "glyph.auth") {
            att->SetGlyphAuth(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "glyph.name") {
            att->SetGlyphName(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "glyph.num") {
            att->SetGlyphNum(att->StrToHexnum(attrValue));
            return true;
        }
        if (attrType == "glyph.uri") {
            att->SetGlyphUri(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetExternalsymbols(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_EXTSYM)) {
        const AttExtSym *att = dynamic_cast<const AttExtSym *>(element);
        assert(att);
        if (att->HasGlyphAuth()) {
            attributes->push_back({ "glyph.auth", att->StrToStr(att->GetGlyphAuth()) });
        }
        if (att->HasGlyphName()) {
            attributes->push_back({ "glyph.name", att->StrToStr(att->GetGlyphName()) });
        }
        if (att->HasGlyphNum()) {
            attributes->push_back({ "glyph.num", att->HexnumToStr(att->GetGlyphNum()) });
        }
        if (att->HasGlyphUri()) {
            attributes->push_back({ "glyph.uri", att->StrToStr(att->GetGlyphUri()) });
        }
    }
}

} // namespace vrv

#include "atts_facsimile.h"

namespace vrv {

//----------------------------------------------------------------------------
// Facsimile
//----------------------------------------------------------------------------

bool AttModule::SetFacsimile(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_FACSIMILE)) {
        AttFacsimile *att = dynamic_cast<AttFacsimile *>(element);
        assert(att);
        if (attrType == "facs") {
            att->SetFacs(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetFacsimile(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_FACSIMILE)) {
        const AttFacsimile *att = dynamic_cast<const AttFacsimile *>(element);
        assert(att);
        if (att->HasFacs()) {
            attributes->push_back({ "facs", att->StrToStr(att->GetFacs()) });
        }
    }
}

} // namespace vrv

#include "atts_figtable.h"

namespace vrv {

//----------------------------------------------------------------------------
// Figtable
//----------------------------------------------------------------------------

bool AttModule::SetFigtable(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_TABULAR)) {
        AttTabular *att = dynamic_cast<AttTabular *>(element);
        assert(att);
        if (attrType == "colspan") {
            att->SetColspan(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "rowspan") {
            att->SetRowspan(att->StrToInt(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetFigtable(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_TABULAR)) {
        const AttTabular *att = dynamic_cast<const AttTabular *>(element);
        assert(att);
        if (att->HasColspan()) {
            attributes->push_back({ "colspan", att->IntToStr(att->GetColspan()) });
        }
        if (att->HasRowspan()) {
            attributes->push_back({ "rowspan", att->IntToStr(att->GetRowspan()) });
        }
    }
}

} // namespace vrv

#include "atts_fingering.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fingering
//----------------------------------------------------------------------------

bool AttModule::SetFingering(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_FINGGRPLOG)) {
        AttFingGrpLog *att = dynamic_cast<AttFingGrpLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToFingGrpLogForm(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetFingering(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_FINGGRPLOG)) {
        const AttFingGrpLog *att = dynamic_cast<const AttFingGrpLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->FingGrpLogFormToStr(att->GetForm()) });
        }
    }
}

} // namespace vrv

#include "atts_frettab.h"

namespace vrv {

//----------------------------------------------------------------------------
// Frettab
//----------------------------------------------------------------------------

bool AttModule::SetFrettab(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_COURSELOG)) {
        AttCourseLog *att = dynamic_cast<AttCourseLog *>(element);
        assert(att);
        if (attrType == "tuning.standard") {
            att->SetTuningStandard(att->StrToCoursetuning(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NOTEGESTAB)) {
        AttNoteGesTab *att = dynamic_cast<AttNoteGesTab *>(element);
        assert(att);
        if (attrType == "tab.course") {
            att->SetTabCourse(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "tab.fret") {
            att->SetTabFret(att->StrToInt(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetFrettab(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_COURSELOG)) {
        const AttCourseLog *att = dynamic_cast<const AttCourseLog *>(element);
        assert(att);
        if (att->HasTuningStandard()) {
            attributes->push_back({ "tuning.standard", att->CoursetuningToStr(att->GetTuningStandard()) });
        }
    }
    if (element->HasAttClass(ATT_NOTEGESTAB)) {
        const AttNoteGesTab *att = dynamic_cast<const AttNoteGesTab *>(element);
        assert(att);
        if (att->HasTabCourse()) {
            attributes->push_back({ "tab.course", att->IntToStr(att->GetTabCourse()) });
        }
        if (att->HasTabFret()) {
            attributes->push_back({ "tab.fret", att->IntToStr(att->GetTabFret()) });
        }
    }
}

} // namespace vrv

#include "atts_gestural.h"

namespace vrv {

//----------------------------------------------------------------------------
// Gestural
//----------------------------------------------------------------------------

bool AttModule::SetGestural(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_ACCIDENTALGES)) {
        AttAccidentalGes *att = dynamic_cast<AttAccidentalGes *>(element);
        assert(att);
        if (attrType == "accid.ges") {
            att->SetAccidGes(att->StrToAccidentalGestural(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ARTICULATIONGES)) {
        AttArticulationGes *att = dynamic_cast<AttArticulationGes *>(element);
        assert(att);
        if (attrType == "artic.ges") {
            att->SetArticGes(att->StrToArticulationList(attrValue));
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
    if (element->HasAttClass(ATT_DURATIONGES)) {
        AttDurationGes *att = dynamic_cast<AttDurationGes *>(element);
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
    if (element->HasAttClass(ATT_MDIVGES)) {
        AttMdivGes *att = dynamic_cast<AttMdivGes *>(element);
        assert(att);
        if (attrType == "attacca") {
            att->SetAttacca(att->StrToBoolean(attrValue));
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
    if (element->HasAttClass(ATT_ORNAMENTACCIDGES)) {
        AttOrnamentAccidGes *att = dynamic_cast<AttOrnamentAccidGes *>(element);
        assert(att);
        if (attrType == "accidupper.ges") {
            att->SetAccidupperGes(att->StrToAccidentalGestural(attrValue));
            return true;
        }
        if (attrType == "accidlower.ges") {
            att->SetAccidlowerGes(att->StrToAccidentalGestural(attrValue));
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
    if (element->HasAttClass(ATT_TIMESTAMPGES)) {
        AttTimestampGes *att = dynamic_cast<AttTimestampGes *>(element);
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
    if (element->HasAttClass(ATT_TIMESTAMP2GES)) {
        AttTimestamp2Ges *att = dynamic_cast<AttTimestamp2Ges *>(element);
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

void AttModule::GetGestural(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ACCIDENTALGES)) {
        const AttAccidentalGes *att = dynamic_cast<const AttAccidentalGes *>(element);
        assert(att);
        if (att->HasAccidGes()) {
            attributes->push_back({ "accid.ges", att->AccidentalGesturalToStr(att->GetAccidGes()) });
        }
    }
    if (element->HasAttClass(ATT_ARTICULATIONGES)) {
        const AttArticulationGes *att = dynamic_cast<const AttArticulationGes *>(element);
        assert(att);
        if (att->HasArticGes()) {
            attributes->push_back({ "artic.ges", att->ArticulationListToStr(att->GetArticGes()) });
        }
    }
    if (element->HasAttClass(ATT_BENDGES)) {
        const AttBendGes *att = dynamic_cast<const AttBendGes *>(element);
        assert(att);
        if (att->HasAmount()) {
            attributes->push_back({ "amount", att->DblToStr(att->GetAmount()) });
        }
    }
    if (element->HasAttClass(ATT_DURATIONGES)) {
        const AttDurationGes *att = dynamic_cast<const AttDurationGes *>(element);
        assert(att);
        if (att->HasDurGes()) {
            attributes->push_back({ "dur.ges", att->DurationToStr(att->GetDurGes()) });
        }
        if (att->HasDotsGes()) {
            attributes->push_back({ "dots.ges", att->IntToStr(att->GetDotsGes()) });
        }
        if (att->HasDurMetrical()) {
            attributes->push_back({ "dur.metrical", att->DblToStr(att->GetDurMetrical()) });
        }
        if (att->HasDurPpq()) {
            attributes->push_back({ "dur.ppq", att->IntToStr(att->GetDurPpq()) });
        }
        if (att->HasDurReal()) {
            attributes->push_back({ "dur.real", att->DblToStr(att->GetDurReal()) });
        }
        if (att->HasDurRecip()) {
            attributes->push_back({ "dur.recip", att->StrToStr(att->GetDurRecip()) });
        }
    }
    if (element->HasAttClass(ATT_MDIVGES)) {
        const AttMdivGes *att = dynamic_cast<const AttMdivGes *>(element);
        assert(att);
        if (att->HasAttacca()) {
            attributes->push_back({ "attacca", att->BooleanToStr(att->GetAttacca()) });
        }
    }
    if (element->HasAttClass(ATT_NCGES)) {
        const AttNcGes *att = dynamic_cast<const AttNcGes *>(element);
        assert(att);
        if (att->HasOctGes()) {
            attributes->push_back({ "oct.ges", att->OctaveToStr(att->GetOctGes()) });
        }
        if (att->HasPnameGes()) {
            attributes->push_back({ "pname.ges", att->PitchnameToStr(att->GetPnameGes()) });
        }
        if (att->HasPnum()) {
            attributes->push_back({ "pnum", att->IntToStr(att->GetPnum()) });
        }
    }
    if (element->HasAttClass(ATT_NOTEGES)) {
        const AttNoteGes *att = dynamic_cast<const AttNoteGes *>(element);
        assert(att);
        if (att->HasExtremis()) {
            attributes->push_back({ "extremis", att->NoteGesExtremisToStr(att->GetExtremis()) });
        }
        if (att->HasOctGes()) {
            attributes->push_back({ "oct.ges", att->OctaveToStr(att->GetOctGes()) });
        }
        if (att->HasPnameGes()) {
            attributes->push_back({ "pname.ges", att->PitchnameToStr(att->GetPnameGes()) });
        }
        if (att->HasPnum()) {
            attributes->push_back({ "pnum", att->IntToStr(att->GetPnum()) });
        }
    }
    if (element->HasAttClass(ATT_ORNAMENTACCIDGES)) {
        const AttOrnamentAccidGes *att = dynamic_cast<const AttOrnamentAccidGes *>(element);
        assert(att);
        if (att->HasAccidupperGes()) {
            attributes->push_back({ "accidupper.ges", att->AccidentalGesturalToStr(att->GetAccidupperGes()) });
        }
        if (att->HasAccidlowerGes()) {
            attributes->push_back({ "accidlower.ges", att->AccidentalGesturalToStr(att->GetAccidlowerGes()) });
        }
    }
    if (element->HasAttClass(ATT_SECTIONGES)) {
        const AttSectionGes *att = dynamic_cast<const AttSectionGes *>(element);
        assert(att);
        if (att->HasAttacca()) {
            attributes->push_back({ "attacca", att->BooleanToStr(att->GetAttacca()) });
        }
    }
    if (element->HasAttClass(ATT_SOUNDLOCATION)) {
        const AttSoundLocation *att = dynamic_cast<const AttSoundLocation *>(element);
        assert(att);
        if (att->HasAzimuth()) {
            attributes->push_back({ "azimuth", att->DblToStr(att->GetAzimuth()) });
        }
        if (att->HasElevation()) {
            attributes->push_back({ "elevation", att->DblToStr(att->GetElevation()) });
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMPGES)) {
        const AttTimestampGes *att = dynamic_cast<const AttTimestampGes *>(element);
        assert(att);
        if (att->HasTstampGes()) {
            attributes->push_back({ "tstamp.ges", att->DblToStr(att->GetTstampGes()) });
        }
        if (att->HasTstampReal()) {
            attributes->push_back({ "tstamp.real", att->StrToStr(att->GetTstampReal()) });
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMP2GES)) {
        const AttTimestamp2Ges *att = dynamic_cast<const AttTimestamp2Ges *>(element);
        assert(att);
        if (att->HasTstamp2Ges()) {
            attributes->push_back({ "tstamp2.ges", att->MeasurebeatToStr(att->GetTstamp2Ges()) });
        }
        if (att->HasTstamp2Real()) {
            attributes->push_back({ "tstamp2.real", att->StrToStr(att->GetTstamp2Real()) });
        }
    }
}

} // namespace vrv

#include "atts_harmony.h"

namespace vrv {

//----------------------------------------------------------------------------
// Harmony
//----------------------------------------------------------------------------

bool AttModule::SetHarmony(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_HARMLOG)) {
        AttHarmLog *att = dynamic_cast<AttHarmLog *>(element);
        assert(att);
        if (attrType == "chordref") {
            att->SetChordref(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetHarmony(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_HARMLOG)) {
        const AttHarmLog *att = dynamic_cast<const AttHarmLog *>(element);
        assert(att);
        if (att->HasChordref()) {
            attributes->push_back({ "chordref", att->StrToStr(att->GetChordref()) });
        }
    }
}

} // namespace vrv

#include "atts_header.h"

namespace vrv {

//----------------------------------------------------------------------------
// Header
//----------------------------------------------------------------------------

bool AttModule::SetHeader(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_ADLIBITUM)) {
        AttAdlibitum *att = dynamic_cast<AttAdlibitum *>(element);
        assert(att);
        if (attrType == "adlib") {
            att->SetAdlib(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BIFOLIUMSURFACES)) {
        AttBifoliumSurfaces *att = dynamic_cast<AttBifoliumSurfaces *>(element);
        assert(att);
        if (attrType == "outer.recto") {
            att->SetOuterRecto(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "inner.verso") {
            att->SetInnerVerso(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "inner.recto") {
            att->SetInnerRecto(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "outer.verso") {
            att->SetOuterVerso(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FOLIUMSURFACES)) {
        AttFoliumSurfaces *att = dynamic_cast<AttFoliumSurfaces *>(element);
        assert(att);
        if (attrType == "recto") {
            att->SetRecto(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "verso") {
            att->SetVerso(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PERFRES)) {
        AttPerfRes *att = dynamic_cast<AttPerfRes *>(element);
        assert(att);
        if (attrType == "solo") {
            att->SetSolo(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PERFRESBASIC)) {
        AttPerfResBasic *att = dynamic_cast<AttPerfResBasic *>(element);
        assert(att);
        if (attrType == "count") {
            att->SetCount(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_RECORDTYPE)) {
        AttRecordType *att = dynamic_cast<AttRecordType *>(element);
        assert(att);
        if (attrType == "recordtype") {
            att->SetRecordtype(att->StrToRecordTypeRecordtype(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_REGULARMETHOD)) {
        AttRegularMethod *att = dynamic_cast<AttRegularMethod *>(element);
        assert(att);
        if (attrType == "method") {
            att->SetMethod(att->StrToRegularMethodMethod(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetHeader(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ADLIBITUM)) {
        const AttAdlibitum *att = dynamic_cast<const AttAdlibitum *>(element);
        assert(att);
        if (att->HasAdlib()) {
            attributes->push_back({ "adlib", att->BooleanToStr(att->GetAdlib()) });
        }
    }
    if (element->HasAttClass(ATT_BIFOLIUMSURFACES)) {
        const AttBifoliumSurfaces *att = dynamic_cast<const AttBifoliumSurfaces *>(element);
        assert(att);
        if (att->HasOuterRecto()) {
            attributes->push_back({ "outer.recto", att->StrToStr(att->GetOuterRecto()) });
        }
        if (att->HasInnerVerso()) {
            attributes->push_back({ "inner.verso", att->StrToStr(att->GetInnerVerso()) });
        }
        if (att->HasInnerRecto()) {
            attributes->push_back({ "inner.recto", att->StrToStr(att->GetInnerRecto()) });
        }
        if (att->HasOuterVerso()) {
            attributes->push_back({ "outer.verso", att->StrToStr(att->GetOuterVerso()) });
        }
    }
    if (element->HasAttClass(ATT_FOLIUMSURFACES)) {
        const AttFoliumSurfaces *att = dynamic_cast<const AttFoliumSurfaces *>(element);
        assert(att);
        if (att->HasRecto()) {
            attributes->push_back({ "recto", att->StrToStr(att->GetRecto()) });
        }
        if (att->HasVerso()) {
            attributes->push_back({ "verso", att->StrToStr(att->GetVerso()) });
        }
    }
    if (element->HasAttClass(ATT_PERFRES)) {
        const AttPerfRes *att = dynamic_cast<const AttPerfRes *>(element);
        assert(att);
        if (att->HasSolo()) {
            attributes->push_back({ "solo", att->BooleanToStr(att->GetSolo()) });
        }
    }
    if (element->HasAttClass(ATT_PERFRESBASIC)) {
        const AttPerfResBasic *att = dynamic_cast<const AttPerfResBasic *>(element);
        assert(att);
        if (att->HasCount()) {
            attributes->push_back({ "count", att->IntToStr(att->GetCount()) });
        }
    }
    if (element->HasAttClass(ATT_RECORDTYPE)) {
        const AttRecordType *att = dynamic_cast<const AttRecordType *>(element);
        assert(att);
        if (att->HasRecordtype()) {
            attributes->push_back({ "recordtype", att->RecordTypeRecordtypeToStr(att->GetRecordtype()) });
        }
    }
    if (element->HasAttClass(ATT_REGULARMETHOD)) {
        const AttRegularMethod *att = dynamic_cast<const AttRegularMethod *>(element);
        assert(att);
        if (att->HasMethod()) {
            attributes->push_back({ "method", att->RegularMethodMethodToStr(att->GetMethod()) });
        }
    }
}

} // namespace vrv

#include "atts_mensural.h"

namespace vrv {

//----------------------------------------------------------------------------
// Mensural
//----------------------------------------------------------------------------

bool AttModule::SetMensural(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_DURATIONQUALITY)) {
        AttDurationQuality *att = dynamic_cast<AttDurationQuality *>(element);
        assert(att);
        if (attrType == "dur.quality") {
            att->SetDurQuality(att->StrToDurqualityMensural(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MENSURALLOG)) {
        AttMensuralLog *att = dynamic_cast<AttMensuralLog *>(element);
        assert(att);
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
    if (element->HasAttClass(ATT_NOTEVISMENSURAL)) {
        AttNoteVisMensural *att = dynamic_cast<AttNoteVisMensural *>(element);
        assert(att);
        if (attrType == "lig") {
            att->SetLig(att->StrToLigatureform(attrValue));
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

void AttModule::GetMensural(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_DURATIONQUALITY)) {
        const AttDurationQuality *att = dynamic_cast<const AttDurationQuality *>(element);
        assert(att);
        if (att->HasDurQuality()) {
            attributes->push_back({ "dur.quality", att->DurqualityMensuralToStr(att->GetDurQuality()) });
        }
    }
    if (element->HasAttClass(ATT_MENSURALLOG)) {
        const AttMensuralLog *att = dynamic_cast<const AttMensuralLog *>(element);
        assert(att);
        if (att->HasProportNum()) {
            attributes->push_back({ "proport.num", att->IntToStr(att->GetProportNum()) });
        }
        if (att->HasProportNumbase()) {
            attributes->push_back({ "proport.numbase", att->IntToStr(att->GetProportNumbase()) });
        }
    }
    if (element->HasAttClass(ATT_MENSURALSHARED)) {
        const AttMensuralShared *att = dynamic_cast<const AttMensuralShared *>(element);
        assert(att);
        if (att->HasModusmaior()) {
            attributes->push_back({ "modusmaior", att->ModusmaiorToStr(att->GetModusmaior()) });
        }
        if (att->HasModusminor()) {
            attributes->push_back({ "modusminor", att->ModusminorToStr(att->GetModusminor()) });
        }
        if (att->HasProlatio()) {
            attributes->push_back({ "prolatio", att->ProlatioToStr(att->GetProlatio()) });
        }
        if (att->HasTempus()) {
            attributes->push_back({ "tempus", att->TempusToStr(att->GetTempus()) });
        }
        if (att->HasDivisio()) {
            attributes->push_back({ "divisio", att->DivisioToStr(att->GetDivisio()) });
        }
    }
    if (element->HasAttClass(ATT_NOTEVISMENSURAL)) {
        const AttNoteVisMensural *att = dynamic_cast<const AttNoteVisMensural *>(element);
        assert(att);
        if (att->HasLig()) {
            attributes->push_back({ "lig", att->LigatureformToStr(att->GetLig()) });
        }
    }
    if (element->HasAttClass(ATT_RESTVISMENSURAL)) {
        const AttRestVisMensural *att = dynamic_cast<const AttRestVisMensural *>(element);
        assert(att);
        if (att->HasSpaces()) {
            attributes->push_back({ "spaces", att->IntToStr(att->GetSpaces()) });
        }
    }
    if (element->HasAttClass(ATT_STEMSMENSURAL)) {
        const AttStemsMensural *att = dynamic_cast<const AttStemsMensural *>(element);
        assert(att);
        if (att->HasStemForm()) {
            attributes->push_back({ "stem.form", att->StemformMensuralToStr(att->GetStemForm()) });
        }
    }
}

} // namespace vrv

#include "atts_midi.h"

namespace vrv {

//----------------------------------------------------------------------------
// Midi
//----------------------------------------------------------------------------

bool AttModule::SetMidi(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_CHANNELIZED)) {
        AttChannelized *att = dynamic_cast<AttChannelized *>(element);
        assert(att);
        if (attrType == "midi.channel") {
            att->SetMidiChannel(att->StrToMidichannel(attrValue));
            return true;
        }
        if (attrType == "midi.duty") {
            att->SetMidiDuty(att->StrToPercentLimited(attrValue));
            return true;
        }
        if (attrType == "midi.port") {
            att->SetMidiPort(att->StrToMidivalueName(attrValue));
            return true;
        }
        if (attrType == "midi.track") {
            att->SetMidiTrack(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_INSTRUMENTIDENT)) {
        AttInstrumentIdent *att = dynamic_cast<AttInstrumentIdent *>(element);
        assert(att);
        if (attrType == "instr") {
            att->SetInstr(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDIINSTRUMENT)) {
        AttMidiInstrument *att = dynamic_cast<AttMidiInstrument *>(element);
        assert(att);
        if (attrType == "midi.instrnum") {
            att->SetMidiInstrnum(att->StrToMidivalue(attrValue));
            return true;
        }
        if (attrType == "midi.instrname") {
            att->SetMidiInstrname(att->StrToMidinames(attrValue));
            return true;
        }
        if (attrType == "midi.pan") {
            att->SetMidiPan(att->StrToMidivaluePan(attrValue));
            return true;
        }
        if (attrType == "midi.patchname") {
            att->SetMidiPatchname(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "midi.patchnum") {
            att->SetMidiPatchnum(att->StrToMidivalue(attrValue));
            return true;
        }
        if (attrType == "midi.volume") {
            att->SetMidiVolume(att->StrToPercent(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDINUMBER)) {
        AttMidiNumber *att = dynamic_cast<AttMidiNumber *>(element);
        assert(att);
        if (attrType == "num") {
            att->SetNum(att->StrToMidivalue(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDITEMPO)) {
        AttMidiTempo *att = dynamic_cast<AttMidiTempo *>(element);
        assert(att);
        if (attrType == "midi.bpm") {
            att->SetMidiBpm(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "midi.mspb") {
            att->SetMidiMspb(att->StrToMidimspb(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE)) {
        AttMidiValue *att = dynamic_cast<AttMidiValue *>(element);
        assert(att);
        if (attrType == "val") {
            att->SetVal(att->StrToMidivalue(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE2)) {
        AttMidiValue2 *att = dynamic_cast<AttMidiValue2 *>(element);
        assert(att);
        if (attrType == "val2") {
            att->SetVal2(att->StrToMidivalue(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MIDIVELOCITY)) {
        AttMidiVelocity *att = dynamic_cast<AttMidiVelocity *>(element);
        assert(att);
        if (attrType == "vel") {
            att->SetVel(att->StrToMidivalue(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIMEBASE)) {
        AttTimeBase *att = dynamic_cast<AttTimeBase *>(element);
        assert(att);
        if (attrType == "ppq") {
            att->SetPpq(att->StrToInt(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetMidi(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_CHANNELIZED)) {
        const AttChannelized *att = dynamic_cast<const AttChannelized *>(element);
        assert(att);
        if (att->HasMidiChannel()) {
            attributes->push_back({ "midi.channel", att->MidichannelToStr(att->GetMidiChannel()) });
        }
        if (att->HasMidiDuty()) {
            attributes->push_back({ "midi.duty", att->PercentLimitedToStr(att->GetMidiDuty()) });
        }
        if (att->HasMidiPort()) {
            attributes->push_back({ "midi.port", att->MidivalueNameToStr(att->GetMidiPort()) });
        }
        if (att->HasMidiTrack()) {
            attributes->push_back({ "midi.track", att->IntToStr(att->GetMidiTrack()) });
        }
    }
    if (element->HasAttClass(ATT_INSTRUMENTIDENT)) {
        const AttInstrumentIdent *att = dynamic_cast<const AttInstrumentIdent *>(element);
        assert(att);
        if (att->HasInstr()) {
            attributes->push_back({ "instr", att->StrToStr(att->GetInstr()) });
        }
    }
    if (element->HasAttClass(ATT_MIDIINSTRUMENT)) {
        const AttMidiInstrument *att = dynamic_cast<const AttMidiInstrument *>(element);
        assert(att);
        if (att->HasMidiInstrnum()) {
            attributes->push_back({ "midi.instrnum", att->MidivalueToStr(att->GetMidiInstrnum()) });
        }
        if (att->HasMidiInstrname()) {
            attributes->push_back({ "midi.instrname", att->MidinamesToStr(att->GetMidiInstrname()) });
        }
        if (att->HasMidiPan()) {
            attributes->push_back({ "midi.pan", att->MidivaluePanToStr(att->GetMidiPan()) });
        }
        if (att->HasMidiPatchname()) {
            attributes->push_back({ "midi.patchname", att->StrToStr(att->GetMidiPatchname()) });
        }
        if (att->HasMidiPatchnum()) {
            attributes->push_back({ "midi.patchnum", att->MidivalueToStr(att->GetMidiPatchnum()) });
        }
        if (att->HasMidiVolume()) {
            attributes->push_back({ "midi.volume", att->PercentToStr(att->GetMidiVolume()) });
        }
    }
    if (element->HasAttClass(ATT_MIDINUMBER)) {
        const AttMidiNumber *att = dynamic_cast<const AttMidiNumber *>(element);
        assert(att);
        if (att->HasNum()) {
            attributes->push_back({ "num", att->MidivalueToStr(att->GetNum()) });
        }
    }
    if (element->HasAttClass(ATT_MIDITEMPO)) {
        const AttMidiTempo *att = dynamic_cast<const AttMidiTempo *>(element);
        assert(att);
        if (att->HasMidiBpm()) {
            attributes->push_back({ "midi.bpm", att->DblToStr(att->GetMidiBpm()) });
        }
        if (att->HasMidiMspb()) {
            attributes->push_back({ "midi.mspb", att->MidimspbToStr(att->GetMidiMspb()) });
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE)) {
        const AttMidiValue *att = dynamic_cast<const AttMidiValue *>(element);
        assert(att);
        if (att->HasVal()) {
            attributes->push_back({ "val", att->MidivalueToStr(att->GetVal()) });
        }
    }
    if (element->HasAttClass(ATT_MIDIVALUE2)) {
        const AttMidiValue2 *att = dynamic_cast<const AttMidiValue2 *>(element);
        assert(att);
        if (att->HasVal2()) {
            attributes->push_back({ "val2", att->MidivalueToStr(att->GetVal2()) });
        }
    }
    if (element->HasAttClass(ATT_MIDIVELOCITY)) {
        const AttMidiVelocity *att = dynamic_cast<const AttMidiVelocity *>(element);
        assert(att);
        if (att->HasVel()) {
            attributes->push_back({ "vel", att->MidivalueToStr(att->GetVel()) });
        }
    }
    if (element->HasAttClass(ATT_TIMEBASE)) {
        const AttTimeBase *att = dynamic_cast<const AttTimeBase *>(element);
        assert(att);
        if (att->HasPpq()) {
            attributes->push_back({ "ppq", att->IntToStr(att->GetPpq()) });
        }
    }
}

} // namespace vrv

#include "atts_neumes.h"

namespace vrv {

//----------------------------------------------------------------------------
// Neumes
//----------------------------------------------------------------------------

bool AttModule::SetNeumes(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_DIVLINELOG)) {
        AttDivLineLog *att = dynamic_cast<AttDivLineLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToDivLineLogForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NCLOG)) {
        AttNcLog *att = dynamic_cast<AttNcLog *>(element);
        assert(att);
        if (attrType == "oct") {
            att->SetOct(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pname") {
            att->SetPname(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NCFORM)) {
        AttNcForm *att = dynamic_cast<AttNcForm *>(element);
        assert(att);
        if (attrType == "angled") {
            att->SetAngled(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "con") {
            att->SetCon(att->StrToNcFormCon(attrValue));
            return true;
        }
        if (attrType == "curve") {
            att->SetCurve(att->StrToNcFormCurve(attrValue));
            return true;
        }
        if (attrType == "hooked") {
            att->SetHooked(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "ligated") {
            att->SetLigated(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "rellen") {
            att->SetRellen(att->StrToNcFormRellen(attrValue));
            return true;
        }
        if (attrType == "sShape") {
            att->SetSShape(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "tilt") {
            att->SetTilt(att->StrToCompassdirection(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetNeumes(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_DIVLINELOG)) {
        const AttDivLineLog *att = dynamic_cast<const AttDivLineLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->DivLineLogFormToStr(att->GetForm()) });
        }
    }
    if (element->HasAttClass(ATT_NCLOG)) {
        const AttNcLog *att = dynamic_cast<const AttNcLog *>(element);
        assert(att);
        if (att->HasOct()) {
            attributes->push_back({ "oct", att->StrToStr(att->GetOct()) });
        }
        if (att->HasPname()) {
            attributes->push_back({ "pname", att->StrToStr(att->GetPname()) });
        }
    }
    if (element->HasAttClass(ATT_NCFORM)) {
        const AttNcForm *att = dynamic_cast<const AttNcForm *>(element);
        assert(att);
        if (att->HasAngled()) {
            attributes->push_back({ "angled", att->BooleanToStr(att->GetAngled()) });
        }
        if (att->HasCon()) {
            attributes->push_back({ "con", att->NcFormConToStr(att->GetCon()) });
        }
        if (att->HasCurve()) {
            attributes->push_back({ "curve", att->NcFormCurveToStr(att->GetCurve()) });
        }
        if (att->HasHooked()) {
            attributes->push_back({ "hooked", att->BooleanToStr(att->GetHooked()) });
        }
        if (att->HasLigated()) {
            attributes->push_back({ "ligated", att->BooleanToStr(att->GetLigated()) });
        }
        if (att->HasRellen()) {
            attributes->push_back({ "rellen", att->NcFormRellenToStr(att->GetRellen()) });
        }
        if (att->HasSShape()) {
            attributes->push_back({ "sShape", att->StrToStr(att->GetSShape()) });
        }
        if (att->HasTilt()) {
            attributes->push_back({ "tilt", att->CompassdirectionToStr(att->GetTilt()) });
        }
    }
}

} // namespace vrv

#include "atts_pagebased.h"

namespace vrv {

//----------------------------------------------------------------------------
// Pagebased
//----------------------------------------------------------------------------

bool AttModule::SetPagebased(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_MARGINS)) {
        AttMargins *att = dynamic_cast<AttMargins *>(element);
        assert(att);
        if (attrType == "topmar") {
            att->SetTopmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "botmar") {
            att->SetBotmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "leftmar") {
            att->SetLeftmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "rightmar") {
            att->SetRightmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetPagebased(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_MARGINS)) {
        const AttMargins *att = dynamic_cast<const AttMargins *>(element);
        assert(att);
        if (att->HasTopmar()) {
            attributes->push_back({ "topmar", att->MeasurementunsignedToStr(att->GetTopmar()) });
        }
        if (att->HasBotmar()) {
            attributes->push_back({ "botmar", att->MeasurementunsignedToStr(att->GetBotmar()) });
        }
        if (att->HasLeftmar()) {
            attributes->push_back({ "leftmar", att->MeasurementunsignedToStr(att->GetLeftmar()) });
        }
        if (att->HasRightmar()) {
            attributes->push_back({ "rightmar", att->MeasurementunsignedToStr(att->GetRightmar()) });
        }
    }
}

} // namespace vrv

#include "atts_performance.h"

namespace vrv {

//----------------------------------------------------------------------------
// Performance
//----------------------------------------------------------------------------

bool AttModule::SetPerformance(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_ALIGNMENT)) {
        AttAlignment *att = dynamic_cast<AttAlignment *>(element);
        assert(att);
        if (attrType == "when") {
            att->SetWhen(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetPerformance(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ALIGNMENT)) {
        const AttAlignment *att = dynamic_cast<const AttAlignment *>(element);
        assert(att);
        if (att->HasWhen()) {
            attributes->push_back({ "when", att->StrToStr(att->GetWhen()) });
        }
    }
}

} // namespace vrv

#include "atts_shared.h"

namespace vrv {

//----------------------------------------------------------------------------
// Shared
//----------------------------------------------------------------------------

bool AttModule::SetShared(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_ACCIDLOG)) {
        AttAccidLog *att = dynamic_cast<AttAccidLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToAccidLogFunc(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ACCIDENTAL)) {
        AttAccidental *att = dynamic_cast<AttAccidental *>(element);
        assert(att);
        if (attrType == "accid") {
            att->SetAccid(att->StrToAccidentalWritten(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ARTICULATION)) {
        AttArticulation *att = dynamic_cast<AttArticulation *>(element);
        assert(att);
        if (attrType == "artic") {
            att->SetArtic(att->StrToArticulationList(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ATTACCALOG)) {
        AttAttaccaLog *att = dynamic_cast<AttAttaccaLog *>(element);
        assert(att);
        if (attrType == "target") {
            att->SetTarget(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_AUDIENCE)) {
        AttAudience *att = dynamic_cast<AttAudience *>(element);
        assert(att);
        if (attrType == "audience") {
            att->SetAudience(att->StrToAudienceAudience(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_AUGMENTDOTS)) {
        AttAugmentDots *att = dynamic_cast<AttAugmentDots *>(element);
        assert(att);
        if (attrType == "dots") {
            att->SetDots(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_AUTHORIZED)) {
        AttAuthorized *att = dynamic_cast<AttAuthorized *>(element);
        assert(att);
        if (attrType == "auth") {
            att->SetAuth(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "auth.uri") {
            att->SetAuthUri(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BARLINELOG)) {
        AttBarLineLog *att = dynamic_cast<AttBarLineLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToBarrendition(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BARRING)) {
        AttBarring *att = dynamic_cast<AttBarring *>(element);
        assert(att);
        if (attrType == "bar.len") {
            att->SetBarLen(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "bar.method") {
            att->SetBarMethod(att->StrToBarmethod(attrValue));
            return true;
        }
        if (attrType == "bar.place") {
            att->SetBarPlace(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BASIC)) {
        AttBasic *att = dynamic_cast<AttBasic *>(element);
        assert(att);
        if (attrType == "xml:base") {
            att->SetBase(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BIBL)) {
        AttBibl *att = dynamic_cast<AttBibl *>(element);
        assert(att);
        if (attrType == "analog") {
            att->SetAnalog(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CALENDARED)) {
        AttCalendared *att = dynamic_cast<AttCalendared *>(element);
        assert(att);
        if (attrType == "calendar") {
            att->SetCalendar(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CANONICAL)) {
        AttCanonical *att = dynamic_cast<AttCanonical *>(element);
        assert(att);
        if (attrType == "codedval") {
            att->SetCodedval(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CLASSED)) {
        AttClassed *att = dynamic_cast<AttClassed *>(element);
        assert(att);
        if (attrType == "class") {
            att->SetClass(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CLEFLOG)) {
        AttClefLog *att = dynamic_cast<AttClefLog *>(element);
        assert(att);
        if (attrType == "cautionary") {
            att->SetCautionary(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CLEFSHAPE)) {
        AttClefShape *att = dynamic_cast<AttClefShape *>(element);
        assert(att);
        if (attrType == "shape") {
            att->SetShape(att->StrToClefshape(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CLEFFINGLOG)) {
        AttCleffingLog *att = dynamic_cast<AttCleffingLog *>(element);
        assert(att);
        if (attrType == "clef.shape") {
            att->SetClefShape(att->StrToClefshape(attrValue));
            return true;
        }
        if (attrType == "clef.line") {
            att->SetClefLine(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "clef.dis") {
            att->SetClefDis(att->StrToOctaveDis(attrValue));
            return true;
        }
        if (attrType == "clef.dis.place") {
            att->SetClefDisPlace(att->StrToStaffrelBasic(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_COLOR)) {
        AttColor *att = dynamic_cast<AttColor *>(element);
        assert(att);
        if (attrType == "color") {
            att->SetColor(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_COLORATION)) {
        AttColoration *att = dynamic_cast<AttColoration *>(element);
        assert(att);
        if (attrType == "colored") {
            att->SetColored(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_COORDX1)) {
        AttCoordX1 *att = dynamic_cast<AttCoordX1 *>(element);
        assert(att);
        if (attrType == "coord.x1") {
            att->SetCoordX1(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_COORDX2)) {
        AttCoordX2 *att = dynamic_cast<AttCoordX2 *>(element);
        assert(att);
        if (attrType == "coord.x2") {
            att->SetCoordX2(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_COORDY1)) {
        AttCoordY1 *att = dynamic_cast<AttCoordY1 *>(element);
        assert(att);
        if (attrType == "coord.y1") {
            att->SetCoordY1(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_COORDINATED)) {
        AttCoordinated *att = dynamic_cast<AttCoordinated *>(element);
        assert(att);
        if (attrType == "ulx") {
            att->SetUlx(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "uly") {
            att->SetUly(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "lrx") {
            att->SetLrx(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "lry") {
            att->SetLry(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "rotate") {
            att->SetRotate(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CUE)) {
        AttCue *att = dynamic_cast<AttCue *>(element);
        assert(att);
        if (attrType == "cue") {
            att->SetCue(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CURVATURE)) {
        AttCurvature *att = dynamic_cast<AttCurvature *>(element);
        assert(att);
        if (attrType == "bezier") {
            att->SetBezier(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "bulge") {
            att->SetBulge(att->StrToBulge(attrValue));
            return true;
        }
        if (attrType == "curvedir") {
            att->SetCurvedir(att->StrToCurvatureCurvedir(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CURVEREND)) {
        AttCurveRend *att = dynamic_cast<AttCurveRend *>(element);
        assert(att);
        if (attrType == "lform") {
            att->SetLform(att->StrToLineform(attrValue));
            return true;
        }
        if (attrType == "lwidth") {
            att->SetLwidth(att->StrToLinewidth(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CUSTOSLOG)) {
        AttCustosLog *att = dynamic_cast<AttCustosLog *>(element);
        assert(att);
        if (attrType == "target") {
            att->SetTarget(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_DATAPOINTING)) {
        AttDataPointing *att = dynamic_cast<AttDataPointing *>(element);
        assert(att);
        if (attrType == "data") {
            att->SetData(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_DATABLE)) {
        AttDatable *att = dynamic_cast<AttDatable *>(element);
        assert(att);
        if (attrType == "enddate") {
            att->SetEnddate(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "isodate") {
            att->SetIsodate(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "notafter") {
            att->SetNotafter(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "notbefore") {
            att->SetNotbefore(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "startdate") {
            att->SetStartdate(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_DISTANCES)) {
        AttDistances *att = dynamic_cast<AttDistances *>(element);
        assert(att);
        if (attrType == "dir.dist") {
            att->SetDirDist(att->StrToMeasurementsigned(attrValue));
            return true;
        }
        if (attrType == "dynam.dist") {
            att->SetDynamDist(att->StrToMeasurementsigned(attrValue));
            return true;
        }
        if (attrType == "harm.dist") {
            att->SetHarmDist(att->StrToMeasurementsigned(attrValue));
            return true;
        }
        if (attrType == "reh.dist") {
            att->SetRehDist(att->StrToMeasurementsigned(attrValue));
            return true;
        }
        if (attrType == "tempo.dist") {
            att->SetTempoDist(att->StrToMeasurementsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_DOTLOG)) {
        AttDotLog *att = dynamic_cast<AttDotLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToDotLogForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_DURATIONADDITIVE)) {
        AttDurationAdditive *att = dynamic_cast<AttDurationAdditive *>(element);
        assert(att);
        if (attrType == "dur") {
            att->SetDur(att->StrToDuration(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_DURATIONDEFAULT)) {
        AttDurationDefault *att = dynamic_cast<AttDurationDefault *>(element);
        assert(att);
        if (attrType == "dur.default") {
            att->SetDurDefault(att->StrToDuration(attrValue));
            return true;
        }
        if (attrType == "num.default") {
            att->SetNumDefault(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "numbase.default") {
            att->SetNumbaseDefault(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_DURATIONLOG)) {
        AttDurationLog *att = dynamic_cast<AttDurationLog *>(element);
        assert(att);
        if (attrType == "dur") {
            att->SetDur(att->StrToDuration(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_DURATIONRATIO)) {
        AttDurationRatio *att = dynamic_cast<AttDurationRatio *>(element);
        assert(att);
        if (attrType == "num") {
            att->SetNum(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "numbase") {
            att->SetNumbase(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ENCLOSINGCHARS)) {
        AttEnclosingChars *att = dynamic_cast<AttEnclosingChars *>(element);
        assert(att);
        if (attrType == "enclose") {
            att->SetEnclose(att->StrToEnclosure(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ENDINGS)) {
        AttEndings *att = dynamic_cast<AttEndings *>(element);
        assert(att);
        if (attrType == "ending.rend") {
            att->SetEndingRend(att->StrToEndingsEndingrend(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_EVIDENCE)) {
        AttEvidence *att = dynamic_cast<AttEvidence *>(element);
        assert(att);
        if (attrType == "cert") {
            att->SetCert(att->StrToCertainty(attrValue));
            return true;
        }
        if (attrType == "evidence") {
            att->SetEvidence(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_EXTENDER)) {
        AttExtender *att = dynamic_cast<AttExtender *>(element);
        assert(att);
        if (attrType == "extender") {
            att->SetExtender(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_EXTENT)) {
        AttExtent *att = dynamic_cast<AttExtent *>(element);
        assert(att);
        if (attrType == "extent") {
            att->SetExtent(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FERMATAPRESENT)) {
        AttFermataPresent *att = dynamic_cast<AttFermataPresent *>(element);
        assert(att);
        if (attrType == "fermata") {
            att->SetFermata(att->StrToStaffrelBasic(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FILING)) {
        AttFiling *att = dynamic_cast<AttFiling *>(element);
        assert(att);
        if (attrType == "nonfiling") {
            att->SetNonfiling(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_GRPSYMLOG)) {
        AttGrpSymLog *att = dynamic_cast<AttGrpSymLog *>(element);
        assert(att);
        if (attrType == "level") {
            att->SetLevel(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HANDIDENT)) {
        AttHandIdent *att = dynamic_cast<AttHandIdent *>(element);
        assert(att);
        if (attrType == "hand") {
            att->SetHand(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HEIGHT)) {
        AttHeight *att = dynamic_cast<AttHeight *>(element);
        assert(att);
        if (attrType == "height") {
            att->SetHeight(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HORIZONTALALIGN)) {
        AttHorizontalAlign *att = dynamic_cast<AttHorizontalAlign *>(element);
        assert(att);
        if (attrType == "halign") {
            att->SetHalign(att->StrToHorizontalalignment(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_INTERNETMEDIA)) {
        AttInternetMedia *att = dynamic_cast<AttInternetMedia *>(element);
        assert(att);
        if (attrType == "mimetype") {
            att->SetMimetype(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_JOINED)) {
        AttJoined *att = dynamic_cast<AttJoined *>(element);
        assert(att);
        if (attrType == "join") {
            att->SetJoin(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_KEYSIGLOG)) {
        AttKeySigLog *att = dynamic_cast<AttKeySigLog *>(element);
        assert(att);
        if (attrType == "sig") {
            att->SetSig(att->StrToKeysignature(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_KEYSIGDEFAULTLOG)) {
        AttKeySigDefaultLog *att = dynamic_cast<AttKeySigDefaultLog *>(element);
        assert(att);
        if (attrType == "key.sig") {
            att->SetKeySig(att->StrToKeysignature(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LABELLED)) {
        AttLabelled *att = dynamic_cast<AttLabelled *>(element);
        assert(att);
        if (attrType == "label") {
            att->SetLabel(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LANG)) {
        AttLang *att = dynamic_cast<AttLang *>(element);
        assert(att);
        if (attrType == "xml:lang") {
            att->SetLang(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "translit") {
            att->SetTranslit(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LAYERLOG)) {
        AttLayerLog *att = dynamic_cast<AttLayerLog *>(element);
        assert(att);
        if (attrType == "def") {
            att->SetDef(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LAYERIDENT)) {
        AttLayerIdent *att = dynamic_cast<AttLayerIdent *>(element);
        assert(att);
        if (attrType == "layer") {
            att->SetLayer(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LINELOC)) {
        AttLineLoc *att = dynamic_cast<AttLineLoc *>(element);
        assert(att);
        if (attrType == "line") {
            att->SetLine(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LINEREND)) {
        AttLineRend *att = dynamic_cast<AttLineRend *>(element);
        assert(att);
        if (attrType == "lendsym") {
            att->SetLendsym(att->StrToLinestartendsymbol(attrValue));
            return true;
        }
        if (attrType == "lendsym.size") {
            att->SetLendsymSize(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "lstartsym") {
            att->SetLstartsym(att->StrToLinestartendsymbol(attrValue));
            return true;
        }
        if (attrType == "lstartsym.size") {
            att->SetLstartsymSize(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LINERENDBASE)) {
        AttLineRendBase *att = dynamic_cast<AttLineRendBase *>(element);
        assert(att);
        if (attrType == "lform") {
            att->SetLform(att->StrToLineform(attrValue));
            return true;
        }
        if (attrType == "lsegs") {
            att->SetLsegs(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "lwidth") {
            att->SetLwidth(att->StrToLinewidth(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LINKING)) {
        AttLinking *att = dynamic_cast<AttLinking *>(element);
        assert(att);
        if (attrType == "copyof") {
            att->SetCopyof(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "corresp") {
            att->SetCorresp(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "follows") {
            att->SetFollows(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "next") {
            att->SetNext(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "precedes") {
            att->SetPrecedes(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "prev") {
            att->SetPrev(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "sameas") {
            att->SetSameas(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "synch") {
            att->SetSynch(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LYRICSTYLE)) {
        AttLyricStyle *att = dynamic_cast<AttLyricStyle *>(element);
        assert(att);
        if (attrType == "lyric.align") {
            att->SetLyricAlign(att->StrToMeasurementsigned(attrValue));
            return true;
        }
        if (attrType == "lyric.fam") {
            att->SetLyricFam(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "lyric.name") {
            att->SetLyricName(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "lyric.size") {
            att->SetLyricSize(att->StrToFontsize(attrValue));
            return true;
        }
        if (attrType == "lyric.style") {
            att->SetLyricStyle(att->StrToFontstyle(attrValue));
            return true;
        }
        if (attrType == "lyric.weight") {
            att->SetLyricWeight(att->StrToFontweight(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MEASURENUMBERS)) {
        AttMeasureNumbers *att = dynamic_cast<AttMeasureNumbers *>(element);
        assert(att);
        if (attrType == "mnum.visible") {
            att->SetMnumVisible(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MEASUREMENT)) {
        AttMeasurement *att = dynamic_cast<AttMeasurement *>(element);
        assert(att);
        if (attrType == "unit") {
            att->SetUnit(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MEDIABOUNDS)) {
        AttMediaBounds *att = dynamic_cast<AttMediaBounds *>(element);
        assert(att);
        if (attrType == "begin") {
            att->SetBegin(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "end") {
            att->SetEnd(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "betype") {
            att->SetBetype(att->StrToBetype(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MEDIUM)) {
        AttMedium *att = dynamic_cast<AttMedium *>(element);
        assert(att);
        if (attrType == "medium") {
            att->SetMedium(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MEIVERSION)) {
        AttMeiVersion *att = dynamic_cast<AttMeiVersion *>(element);
        assert(att);
        if (attrType == "meiversion") {
            att->SetMeiversion(att->StrToMeiVersionMeiversion(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METADATAPOINTING)) {
        AttMetadataPointing *att = dynamic_cast<AttMetadataPointing *>(element);
        assert(att);
        if (attrType == "decls") {
            att->SetDecls(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERCONFORMANCE)) {
        AttMeterConformance *att = dynamic_cast<AttMeterConformance *>(element);
        assert(att);
        if (attrType == "metcon") {
            att->SetMetcon(att->StrToMeterConformanceMetcon(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERCONFORMANCEBAR)) {
        AttMeterConformanceBar *att = dynamic_cast<AttMeterConformanceBar *>(element);
        assert(att);
        if (attrType == "metcon") {
            att->SetMetcon(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "control") {
            att->SetControl(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERSIGLOG)) {
        AttMeterSigLog *att = dynamic_cast<AttMeterSigLog *>(element);
        assert(att);
        if (attrType == "count") {
            att->SetCount(att->StrToMetercountPair(attrValue));
            return true;
        }
        if (attrType == "sym") {
            att->SetSym(att->StrToMetersign(attrValue));
            return true;
        }
        if (attrType == "unit") {
            att->SetUnit(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERSIGDEFAULTLOG)) {
        AttMeterSigDefaultLog *att = dynamic_cast<AttMeterSigDefaultLog *>(element);
        assert(att);
        if (attrType == "meter.count") {
            att->SetMeterCount(att->StrToMetercountPair(attrValue));
            return true;
        }
        if (attrType == "meter.unit") {
            att->SetMeterUnit(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "meter.sym") {
            att->SetMeterSym(att->StrToMetersign(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MMTEMPO)) {
        AttMmTempo *att = dynamic_cast<AttMmTempo *>(element);
        assert(att);
        if (attrType == "mm") {
            att->SetMm(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "mm.unit") {
            att->SetMmUnit(att->StrToDuration(attrValue));
            return true;
        }
        if (attrType == "mm.dots") {
            att->SetMmDots(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MULTINUMMEASURES)) {
        AttMultinumMeasures *att = dynamic_cast<AttMultinumMeasures *>(element);
        assert(att);
        if (attrType == "multi.number") {
            att->SetMultiNumber(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NINTEGER)) {
        AttNInteger *att = dynamic_cast<AttNInteger *>(element);
        assert(att);
        if (attrType == "n") {
            att->SetN(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NNUMBERLIKE)) {
        AttNNumberLike *att = dynamic_cast<AttNNumberLike *>(element);
        assert(att);
        if (attrType == "n") {
            att->SetN(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NAME)) {
        AttName *att = dynamic_cast<AttName *>(element);
        assert(att);
        if (attrType == "nymref") {
            att->SetNymref(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "role") {
            att->SetRole(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NOTATIONSTYLE)) {
        AttNotationStyle *att = dynamic_cast<AttNotationStyle *>(element);
        assert(att);
        if (attrType == "music.name") {
            att->SetMusicName(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "music.size") {
            att->SetMusicSize(att->StrToFontsize(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NOTEHEADS)) {
        AttNoteHeads *att = dynamic_cast<AttNoteHeads *>(element);
        assert(att);
        if (attrType == "head.altsym") {
            att->SetHeadAltsym(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "head.auth") {
            att->SetHeadAuth(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "head.color") {
            att->SetHeadColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "head.fill") {
            att->SetHeadFill(att->StrToFill(attrValue));
            return true;
        }
        if (attrType == "head.fillcolor") {
            att->SetHeadFillcolor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "head.mod") {
            att->SetHeadMod(att->StrToNoteheadmodifier(attrValue));
            return true;
        }
        if (attrType == "head.rotation") {
            att->SetHeadRotation(att->StrToRotation(attrValue));
            return true;
        }
        if (attrType == "head.shape") {
            att->SetHeadShape(att->StrToHeadshape(attrValue));
            return true;
        }
        if (attrType == "head.visible") {
            att->SetHeadVisible(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_OCTAVE)) {
        AttOctave *att = dynamic_cast<AttOctave *>(element);
        assert(att);
        if (attrType == "oct") {
            att->SetOct(att->StrToOctave(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_OCTAVEDEFAULT)) {
        AttOctaveDefault *att = dynamic_cast<AttOctaveDefault *>(element);
        assert(att);
        if (attrType == "oct.default") {
            att->SetOctDefault(att->StrToOctave(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_OCTAVEDISPLACEMENT)) {
        AttOctaveDisplacement *att = dynamic_cast<AttOctaveDisplacement *>(element);
        assert(att);
        if (attrType == "dis") {
            att->SetDis(att->StrToOctaveDis(attrValue));
            return true;
        }
        if (attrType == "dis.place") {
            att->SetDisPlace(att->StrToStaffrelBasic(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ONELINESTAFF)) {
        AttOneLineStaff *att = dynamic_cast<AttOneLineStaff *>(element);
        assert(att);
        if (attrType == "ontheline") {
            att->SetOntheline(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_OPTIMIZATION)) {
        AttOptimization *att = dynamic_cast<AttOptimization *>(element);
        assert(att);
        if (attrType == "optimize") {
            att->SetOptimize(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ORIGINLAYERIDENT)) {
        AttOriginLayerIdent *att = dynamic_cast<AttOriginLayerIdent *>(element);
        assert(att);
        if (attrType == "origin.layer") {
            att->SetOriginLayer(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ORIGINSTAFFIDENT)) {
        AttOriginStaffIdent *att = dynamic_cast<AttOriginStaffIdent *>(element);
        assert(att);
        if (attrType == "origin.staff") {
            att->SetOriginStaff(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ORIGINSTARTENDID)) {
        AttOriginStartEndId *att = dynamic_cast<AttOriginStartEndId *>(element);
        assert(att);
        if (attrType == "origin.startid") {
            att->SetOriginStartid(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "origin.endid") {
            att->SetOriginEndid(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ORIGINTIMESTAMPLOG)) {
        AttOriginTimestampLog *att = dynamic_cast<AttOriginTimestampLog *>(element);
        assert(att);
        if (attrType == "origin.tstamp") {
            att->SetOriginTstamp(att->StrToMeasurebeat(attrValue));
            return true;
        }
        if (attrType == "origin.tstamp2") {
            att->SetOriginTstamp2(att->StrToMeasurebeat(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PAGES)) {
        AttPages *att = dynamic_cast<AttPages *>(element);
        assert(att);
        if (attrType == "page.height") {
            att->SetPageHeight(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "page.width") {
            att->SetPageWidth(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "page.topmar") {
            att->SetPageTopmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "page.botmar") {
            att->SetPageBotmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "page.leftmar") {
            att->SetPageLeftmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "page.rightmar") {
            att->SetPageRightmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "page.panels") {
            att->SetPagePanels(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "page.scale") {
            att->SetPageScale(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PARTIDENT)) {
        AttPartIdent *att = dynamic_cast<AttPartIdent *>(element);
        assert(att);
        if (attrType == "part") {
            att->SetPart(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "partstaff") {
            att->SetPartstaff(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PITCH)) {
        AttPitch *att = dynamic_cast<AttPitch *>(element);
        assert(att);
        if (attrType == "pname") {
            att->SetPname(att->StrToPitchname(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PLACEMENTONSTAFF)) {
        AttPlacementOnStaff *att = dynamic_cast<AttPlacementOnStaff *>(element);
        assert(att);
        if (attrType == "onstaff") {
            att->SetOnstaff(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PLACEMENTRELEVENT)) {
        AttPlacementRelEvent *att = dynamic_cast<AttPlacementRelEvent *>(element);
        assert(att);
        if (attrType == "place") {
            att->SetPlace(att->StrToStaffrel(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PLACEMENTRELSTAFF)) {
        AttPlacementRelStaff *att = dynamic_cast<AttPlacementRelStaff *>(element);
        assert(att);
        if (attrType == "place") {
            att->SetPlace(att->StrToStaffrel(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PLIST)) {
        AttPlist *att = dynamic_cast<AttPlist *>(element);
        assert(att);
        if (attrType == "plist") {
            att->SetPlist(att->StrToXsdAnyURIList(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_POINTING)) {
        AttPointing *att = dynamic_cast<AttPointing *>(element);
        assert(att);
        if (attrType == "xlink:actuate") {
            att->SetActuate(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "xlink:role") {
            att->SetRole(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "xlink:show") {
            att->SetShow(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "target") {
            att->SetTarget(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "targettype") {
            att->SetTargettype(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_QUANTITY)) {
        AttQuantity *att = dynamic_cast<AttQuantity *>(element);
        assert(att);
        if (attrType == "quantity") {
            att->SetQuantity(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_RANGING)) {
        AttRanging *att = dynamic_cast<AttRanging *>(element);
        assert(att);
        if (attrType == "atleast") {
            att->SetAtleast(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "atmost") {
            att->SetAtmost(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "min") {
            att->SetMin(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "max") {
            att->SetMax(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "confidence") {
            att->SetConfidence(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_RESPONSIBILITY)) {
        AttResponsibility *att = dynamic_cast<AttResponsibility *>(element);
        assert(att);
        if (attrType == "resp") {
            att->SetResp(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_RESTDURATIONLOG)) {
        AttRestdurationLog *att = dynamic_cast<AttRestdurationLog *>(element);
        assert(att);
        if (attrType == "dur") {
            att->SetDur(att->StrToDuration(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SCALABLE)) {
        AttScalable *att = dynamic_cast<AttScalable *>(element);
        assert(att);
        if (attrType == "scale") {
            att->SetScale(att->StrToPercent(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SEQUENCE)) {
        AttSequence *att = dynamic_cast<AttSequence *>(element);
        assert(att);
        if (attrType == "seq") {
            att->SetSeq(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SLASHCOUNT)) {
        AttSlashCount *att = dynamic_cast<AttSlashCount *>(element);
        assert(att);
        if (attrType == "slash") {
            att->SetSlash(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SLURPRESENT)) {
        AttSlurPresent *att = dynamic_cast<AttSlurPresent *>(element);
        assert(att);
        if (attrType == "slur") {
            att->SetSlur(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SOURCE)) {
        AttSource *att = dynamic_cast<AttSource *>(element);
        assert(att);
        if (attrType == "source") {
            att->SetSource(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SPACING)) {
        AttSpacing *att = dynamic_cast<AttSpacing *>(element);
        assert(att);
        if (attrType == "spacing.packexp") {
            att->SetSpacingPackexp(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "spacing.packfact") {
            att->SetSpacingPackfact(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "spacing.staff") {
            att->SetSpacingStaff(att->StrToMeasurementsigned(attrValue));
            return true;
        }
        if (attrType == "spacing.system") {
            att->SetSpacingSystem(att->StrToMeasurementsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFLOG)) {
        AttStaffLog *att = dynamic_cast<AttStaffLog *>(element);
        assert(att);
        if (attrType == "def") {
            att->SetDef(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFDEFLOG)) {
        AttStaffDefLog *att = dynamic_cast<AttStaffDefLog *>(element);
        assert(att);
        if (attrType == "lines") {
            att->SetLines(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFGROUPINGSYM)) {
        AttStaffGroupingSym *att = dynamic_cast<AttStaffGroupingSym *>(element);
        assert(att);
        if (attrType == "symbol") {
            att->SetSymbol(att->StrToStaffGroupingSymSymbol(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFIDENT)) {
        AttStaffIdent *att = dynamic_cast<AttStaffIdent *>(element);
        assert(att);
        if (attrType == "staff") {
            att->SetStaff(att->StrToXsdPositiveIntegerList(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFITEMS)) {
        AttStaffItems *att = dynamic_cast<AttStaffItems *>(element);
        assert(att);
        if (attrType == "aboveorder") {
            att->SetAboveorder(att->StrToStaffitem(attrValue));
            return true;
        }
        if (attrType == "beloworder") {
            att->SetBeloworder(att->StrToStaffitem(attrValue));
            return true;
        }
        if (attrType == "betweenorder") {
            att->SetBetweenorder(att->StrToStaffitem(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFLOC)) {
        AttStaffLoc *att = dynamic_cast<AttStaffLoc *>(element);
        assert(att);
        if (attrType == "loc") {
            att->SetLoc(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFLOCPITCHED)) {
        AttStaffLocPitched *att = dynamic_cast<AttStaffLocPitched *>(element);
        assert(att);
        if (attrType == "ploc") {
            att->SetPloc(att->StrToPitchname(attrValue));
            return true;
        }
        if (attrType == "oloc") {
            att->SetOloc(att->StrToOctave(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STARTENDID)) {
        AttStartEndId *att = dynamic_cast<AttStartEndId *>(element);
        assert(att);
        if (attrType == "endid") {
            att->SetEndid(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STARTID)) {
        AttStartId *att = dynamic_cast<AttStartId *>(element);
        assert(att);
        if (attrType == "startid") {
            att->SetStartid(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STEMS)) {
        AttStems *att = dynamic_cast<AttStems *>(element);
        assert(att);
        if (attrType == "stem.dir") {
            att->SetStemDir(att->StrToStemdirection(attrValue));
            return true;
        }
        if (attrType == "stem.len") {
            att->SetStemLen(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "stem.mod") {
            att->SetStemMod(att->StrToStemmodifier(attrValue));
            return true;
        }
        if (attrType == "stem.pos") {
            att->SetStemPos(att->StrToStemposition(attrValue));
            return true;
        }
        if (attrType == "stem.sameas") {
            att->SetStemSameas(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "stem.visible") {
            att->SetStemVisible(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "stem.x") {
            att->SetStemX(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "stem.y") {
            att->SetStemY(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SYLLOG)) {
        AttSylLog *att = dynamic_cast<AttSylLog *>(element);
        assert(att);
        if (attrType == "con") {
            att->SetCon(att->StrToSylLogCon(attrValue));
            return true;
        }
        if (attrType == "wordpos") {
            att->SetWordpos(att->StrToSylLogWordpos(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SYLTEXT)) {
        AttSylText *att = dynamic_cast<AttSylText *>(element);
        assert(att);
        if (attrType == "syl") {
            att->SetSyl(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SYSTEMS)) {
        AttSystems *att = dynamic_cast<AttSystems *>(element);
        assert(att);
        if (attrType == "system.leftline") {
            att->SetSystemLeftline(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "system.leftmar") {
            att->SetSystemLeftmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "system.rightmar") {
            att->SetSystemRightmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "system.topmar") {
            att->SetSystemTopmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TARGETEVAL)) {
        AttTargetEval *att = dynamic_cast<AttTargetEval *>(element);
        assert(att);
        if (attrType == "evaluate") {
            att->SetEvaluate(att->StrToTargetEvalEvaluate(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TEMPOLOG)) {
        AttTempoLog *att = dynamic_cast<AttTempoLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToTempoLogFunc(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TEXTRENDITION)) {
        AttTextRendition *att = dynamic_cast<AttTextRendition *>(element);
        assert(att);
        if (attrType == "altrend") {
            att->SetAltrend(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "rend") {
            att->SetRend(att->StrToTextrendition(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TEXTSTYLE)) {
        AttTextStyle *att = dynamic_cast<AttTextStyle *>(element);
        assert(att);
        if (attrType == "text.fam") {
            att->SetTextFam(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "text.name") {
            att->SetTextName(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "text.size") {
            att->SetTextSize(att->StrToFontsize(attrValue));
            return true;
        }
        if (attrType == "text.style") {
            att->SetTextStyle(att->StrToFontstyle(attrValue));
            return true;
        }
        if (attrType == "text.weight") {
            att->SetTextWeight(att->StrToFontweight(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIEPRESENT)) {
        AttTiePresent *att = dynamic_cast<AttTiePresent *>(element);
        assert(att);
        if (attrType == "tie") {
            att->SetTie(att->StrToTie(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMPLOG)) {
        AttTimestampLog *att = dynamic_cast<AttTimestampLog *>(element);
        assert(att);
        if (attrType == "tstamp") {
            att->SetTstamp(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMP2LOG)) {
        AttTimestamp2Log *att = dynamic_cast<AttTimestamp2Log *>(element);
        assert(att);
        if (attrType == "tstamp2") {
            att->SetTstamp2(att->StrToMeasurebeat(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TRANSPOSITION)) {
        AttTransposition *att = dynamic_cast<AttTransposition *>(element);
        assert(att);
        if (attrType == "trans.diat") {
            att->SetTransDiat(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "trans.semi") {
            att->SetTransSemi(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TUNING)) {
        AttTuning *att = dynamic_cast<AttTuning *>(element);
        assert(att);
        if (attrType == "tune.Hz") {
            att->SetTuneHz(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "tune.pname") {
            att->SetTunePname(att->StrToPitchname(attrValue));
            return true;
        }
        if (attrType == "tune.temper") {
            att->SetTuneTemper(att->StrToTemperament(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TUPLETPRESENT)) {
        AttTupletPresent *att = dynamic_cast<AttTupletPresent *>(element);
        assert(att);
        if (attrType == "tuplet") {
            att->SetTuplet(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TYPED)) {
        AttTyped *att = dynamic_cast<AttTyped *>(element);
        assert(att);
        if (attrType == "type") {
            att->SetType(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TYPOGRAPHY)) {
        AttTypography *att = dynamic_cast<AttTypography *>(element);
        assert(att);
        if (attrType == "fontfam") {
            att->SetFontfam(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "fontname") {
            att->SetFontname(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "fontsize") {
            att->SetFontsize(att->StrToFontsize(attrValue));
            return true;
        }
        if (attrType == "fontstyle") {
            att->SetFontstyle(att->StrToFontstyle(attrValue));
            return true;
        }
        if (attrType == "fontweight") {
            att->SetFontweight(att->StrToFontweight(attrValue));
            return true;
        }
        if (attrType == "letterspacing") {
            att->SetLetterspacing(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "lineheight") {
            att->SetLineheight(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_VERTICALALIGN)) {
        AttVerticalAlign *att = dynamic_cast<AttVerticalAlign *>(element);
        assert(att);
        if (attrType == "valign") {
            att->SetValign(att->StrToVerticalalignment(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_VERTICALGROUP)) {
        AttVerticalGroup *att = dynamic_cast<AttVerticalGroup *>(element);
        assert(att);
        if (attrType == "vgrp") {
            att->SetVgrp(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_VISIBILITY)) {
        AttVisibility *att = dynamic_cast<AttVisibility *>(element);
        assert(att);
        if (attrType == "visible") {
            att->SetVisible(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSETHO)) {
        AttVisualOffsetHo *att = dynamic_cast<AttVisualOffsetHo *>(element);
        assert(att);
        if (attrType == "ho") {
            att->SetHo(att->StrToMeasurementsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSETTO)) {
        AttVisualOffsetTo *att = dynamic_cast<AttVisualOffsetTo *>(element);
        assert(att);
        if (attrType == "to") {
            att->SetTo(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSETVO)) {
        AttVisualOffsetVo *att = dynamic_cast<AttVisualOffsetVo *>(element);
        assert(att);
        if (attrType == "vo") {
            att->SetVo(att->StrToMeasurementsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSET2HO)) {
        AttVisualOffset2Ho *att = dynamic_cast<AttVisualOffset2Ho *>(element);
        assert(att);
        if (attrType == "startho") {
            att->SetStartho(att->StrToMeasurementsigned(attrValue));
            return true;
        }
        if (attrType == "endho") {
            att->SetEndho(att->StrToMeasurementsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSET2TO)) {
        AttVisualOffset2To *att = dynamic_cast<AttVisualOffset2To *>(element);
        assert(att);
        if (attrType == "startto") {
            att->SetStartto(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "endto") {
            att->SetEndto(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSET2VO)) {
        AttVisualOffset2Vo *att = dynamic_cast<AttVisualOffset2Vo *>(element);
        assert(att);
        if (attrType == "startvo") {
            att->SetStartvo(att->StrToMeasurementsigned(attrValue));
            return true;
        }
        if (attrType == "endvo") {
            att->SetEndvo(att->StrToMeasurementsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_VOLTAGROUPINGSYM)) {
        AttVoltaGroupingSym *att = dynamic_cast<AttVoltaGroupingSym *>(element);
        assert(att);
        if (attrType == "voltasym") {
            att->SetVoltasym(att->StrToVoltaGroupingSymVoltasym(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_WHITESPACE)) {
        AttWhitespace *att = dynamic_cast<AttWhitespace *>(element);
        assert(att);
        if (attrType == "xml:space") {
            att->SetSpace(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_WIDTH)) {
        AttWidth *att = dynamic_cast<AttWidth *>(element);
        assert(att);
        if (attrType == "width") {
            att->SetWidth(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_XY)) {
        AttXy *att = dynamic_cast<AttXy *>(element);
        assert(att);
        if (attrType == "x") {
            att->SetX(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "y") {
            att->SetY(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_XY2)) {
        AttXy2 *att = dynamic_cast<AttXy2 *>(element);
        assert(att);
        if (attrType == "x2") {
            att->SetX2(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "y2") {
            att->SetY2(att->StrToDbl(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetShared(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ACCIDLOG)) {
        const AttAccidLog *att = dynamic_cast<const AttAccidLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back({ "func", att->AccidLogFuncToStr(att->GetFunc()) });
        }
    }
    if (element->HasAttClass(ATT_ACCIDENTAL)) {
        const AttAccidental *att = dynamic_cast<const AttAccidental *>(element);
        assert(att);
        if (att->HasAccid()) {
            attributes->push_back({ "accid", att->AccidentalWrittenToStr(att->GetAccid()) });
        }
    }
    if (element->HasAttClass(ATT_ARTICULATION)) {
        const AttArticulation *att = dynamic_cast<const AttArticulation *>(element);
        assert(att);
        if (att->HasArtic()) {
            attributes->push_back({ "artic", att->ArticulationListToStr(att->GetArtic()) });
        }
    }
    if (element->HasAttClass(ATT_ATTACCALOG)) {
        const AttAttaccaLog *att = dynamic_cast<const AttAttaccaLog *>(element);
        assert(att);
        if (att->HasTarget()) {
            attributes->push_back({ "target", att->StrToStr(att->GetTarget()) });
        }
    }
    if (element->HasAttClass(ATT_AUDIENCE)) {
        const AttAudience *att = dynamic_cast<const AttAudience *>(element);
        assert(att);
        if (att->HasAudience()) {
            attributes->push_back({ "audience", att->AudienceAudienceToStr(att->GetAudience()) });
        }
    }
    if (element->HasAttClass(ATT_AUGMENTDOTS)) {
        const AttAugmentDots *att = dynamic_cast<const AttAugmentDots *>(element);
        assert(att);
        if (att->HasDots()) {
            attributes->push_back({ "dots", att->IntToStr(att->GetDots()) });
        }
    }
    if (element->HasAttClass(ATT_AUTHORIZED)) {
        const AttAuthorized *att = dynamic_cast<const AttAuthorized *>(element);
        assert(att);
        if (att->HasAuth()) {
            attributes->push_back({ "auth", att->StrToStr(att->GetAuth()) });
        }
        if (att->HasAuthUri()) {
            attributes->push_back({ "auth.uri", att->StrToStr(att->GetAuthUri()) });
        }
    }
    if (element->HasAttClass(ATT_BARLINELOG)) {
        const AttBarLineLog *att = dynamic_cast<const AttBarLineLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->BarrenditionToStr(att->GetForm()) });
        }
    }
    if (element->HasAttClass(ATT_BARRING)) {
        const AttBarring *att = dynamic_cast<const AttBarring *>(element);
        assert(att);
        if (att->HasBarLen()) {
            attributes->push_back({ "bar.len", att->DblToStr(att->GetBarLen()) });
        }
        if (att->HasBarMethod()) {
            attributes->push_back({ "bar.method", att->BarmethodToStr(att->GetBarMethod()) });
        }
        if (att->HasBarPlace()) {
            attributes->push_back({ "bar.place", att->IntToStr(att->GetBarPlace()) });
        }
    }
    if (element->HasAttClass(ATT_BASIC)) {
        const AttBasic *att = dynamic_cast<const AttBasic *>(element);
        assert(att);
        if (att->HasBase()) {
            attributes->push_back({ "xml:base", att->StrToStr(att->GetBase()) });
        }
    }
    if (element->HasAttClass(ATT_BIBL)) {
        const AttBibl *att = dynamic_cast<const AttBibl *>(element);
        assert(att);
        if (att->HasAnalog()) {
            attributes->push_back({ "analog", att->StrToStr(att->GetAnalog()) });
        }
    }
    if (element->HasAttClass(ATT_CALENDARED)) {
        const AttCalendared *att = dynamic_cast<const AttCalendared *>(element);
        assert(att);
        if (att->HasCalendar()) {
            attributes->push_back({ "calendar", att->StrToStr(att->GetCalendar()) });
        }
    }
    if (element->HasAttClass(ATT_CANONICAL)) {
        const AttCanonical *att = dynamic_cast<const AttCanonical *>(element);
        assert(att);
        if (att->HasCodedval()) {
            attributes->push_back({ "codedval", att->StrToStr(att->GetCodedval()) });
        }
    }
    if (element->HasAttClass(ATT_CLASSED)) {
        const AttClassed *att = dynamic_cast<const AttClassed *>(element);
        assert(att);
        if (att->HasClass()) {
            attributes->push_back({ "class", att->StrToStr(att->GetClass()) });
        }
    }
    if (element->HasAttClass(ATT_CLEFLOG)) {
        const AttClefLog *att = dynamic_cast<const AttClefLog *>(element);
        assert(att);
        if (att->HasCautionary()) {
            attributes->push_back({ "cautionary", att->BooleanToStr(att->GetCautionary()) });
        }
    }
    if (element->HasAttClass(ATT_CLEFSHAPE)) {
        const AttClefShape *att = dynamic_cast<const AttClefShape *>(element);
        assert(att);
        if (att->HasShape()) {
            attributes->push_back({ "shape", att->ClefshapeToStr(att->GetShape()) });
        }
    }
    if (element->HasAttClass(ATT_CLEFFINGLOG)) {
        const AttCleffingLog *att = dynamic_cast<const AttCleffingLog *>(element);
        assert(att);
        if (att->HasClefShape()) {
            attributes->push_back({ "clef.shape", att->ClefshapeToStr(att->GetClefShape()) });
        }
        if (att->HasClefLine()) {
            attributes->push_back({ "clef.line", att->IntToStr(att->GetClefLine()) });
        }
        if (att->HasClefDis()) {
            attributes->push_back({ "clef.dis", att->OctaveDisToStr(att->GetClefDis()) });
        }
        if (att->HasClefDisPlace()) {
            attributes->push_back({ "clef.dis.place", att->StaffrelBasicToStr(att->GetClefDisPlace()) });
        }
    }
    if (element->HasAttClass(ATT_COLOR)) {
        const AttColor *att = dynamic_cast<const AttColor *>(element);
        assert(att);
        if (att->HasColor()) {
            attributes->push_back({ "color", att->StrToStr(att->GetColor()) });
        }
    }
    if (element->HasAttClass(ATT_COLORATION)) {
        const AttColoration *att = dynamic_cast<const AttColoration *>(element);
        assert(att);
        if (att->HasColored()) {
            attributes->push_back({ "colored", att->BooleanToStr(att->GetColored()) });
        }
    }
    if (element->HasAttClass(ATT_COORDX1)) {
        const AttCoordX1 *att = dynamic_cast<const AttCoordX1 *>(element);
        assert(att);
        if (att->HasCoordX1()) {
            attributes->push_back({ "coord.x1", att->DblToStr(att->GetCoordX1()) });
        }
    }
    if (element->HasAttClass(ATT_COORDX2)) {
        const AttCoordX2 *att = dynamic_cast<const AttCoordX2 *>(element);
        assert(att);
        if (att->HasCoordX2()) {
            attributes->push_back({ "coord.x2", att->DblToStr(att->GetCoordX2()) });
        }
    }
    if (element->HasAttClass(ATT_COORDY1)) {
        const AttCoordY1 *att = dynamic_cast<const AttCoordY1 *>(element);
        assert(att);
        if (att->HasCoordY1()) {
            attributes->push_back({ "coord.y1", att->DblToStr(att->GetCoordY1()) });
        }
    }
    if (element->HasAttClass(ATT_COORDINATED)) {
        const AttCoordinated *att = dynamic_cast<const AttCoordinated *>(element);
        assert(att);
        if (att->HasUlx()) {
            attributes->push_back({ "ulx", att->IntToStr(att->GetUlx()) });
        }
        if (att->HasUly()) {
            attributes->push_back({ "uly", att->IntToStr(att->GetUly()) });
        }
        if (att->HasLrx()) {
            attributes->push_back({ "lrx", att->IntToStr(att->GetLrx()) });
        }
        if (att->HasLry()) {
            attributes->push_back({ "lry", att->IntToStr(att->GetLry()) });
        }
        if (att->HasRotate()) {
            attributes->push_back({ "rotate", att->DblToStr(att->GetRotate()) });
        }
    }
    if (element->HasAttClass(ATT_CUE)) {
        const AttCue *att = dynamic_cast<const AttCue *>(element);
        assert(att);
        if (att->HasCue()) {
            attributes->push_back({ "cue", att->BooleanToStr(att->GetCue()) });
        }
    }
    if (element->HasAttClass(ATT_CURVATURE)) {
        const AttCurvature *att = dynamic_cast<const AttCurvature *>(element);
        assert(att);
        if (att->HasBezier()) {
            attributes->push_back({ "bezier", att->StrToStr(att->GetBezier()) });
        }
        if (att->HasBulge()) {
            attributes->push_back({ "bulge", att->BulgeToStr(att->GetBulge()) });
        }
        if (att->HasCurvedir()) {
            attributes->push_back({ "curvedir", att->CurvatureCurvedirToStr(att->GetCurvedir()) });
        }
    }
    if (element->HasAttClass(ATT_CURVEREND)) {
        const AttCurveRend *att = dynamic_cast<const AttCurveRend *>(element);
        assert(att);
        if (att->HasLform()) {
            attributes->push_back({ "lform", att->LineformToStr(att->GetLform()) });
        }
        if (att->HasLwidth()) {
            attributes->push_back({ "lwidth", att->LinewidthToStr(att->GetLwidth()) });
        }
    }
    if (element->HasAttClass(ATT_CUSTOSLOG)) {
        const AttCustosLog *att = dynamic_cast<const AttCustosLog *>(element);
        assert(att);
        if (att->HasTarget()) {
            attributes->push_back({ "target", att->StrToStr(att->GetTarget()) });
        }
    }
    if (element->HasAttClass(ATT_DATAPOINTING)) {
        const AttDataPointing *att = dynamic_cast<const AttDataPointing *>(element);
        assert(att);
        if (att->HasData()) {
            attributes->push_back({ "data", att->StrToStr(att->GetData()) });
        }
    }
    if (element->HasAttClass(ATT_DATABLE)) {
        const AttDatable *att = dynamic_cast<const AttDatable *>(element);
        assert(att);
        if (att->HasEnddate()) {
            attributes->push_back({ "enddate", att->StrToStr(att->GetEnddate()) });
        }
        if (att->HasIsodate()) {
            attributes->push_back({ "isodate", att->StrToStr(att->GetIsodate()) });
        }
        if (att->HasNotafter()) {
            attributes->push_back({ "notafter", att->StrToStr(att->GetNotafter()) });
        }
        if (att->HasNotbefore()) {
            attributes->push_back({ "notbefore", att->StrToStr(att->GetNotbefore()) });
        }
        if (att->HasStartdate()) {
            attributes->push_back({ "startdate", att->StrToStr(att->GetStartdate()) });
        }
    }
    if (element->HasAttClass(ATT_DISTANCES)) {
        const AttDistances *att = dynamic_cast<const AttDistances *>(element);
        assert(att);
        if (att->HasDirDist()) {
            attributes->push_back({ "dir.dist", att->MeasurementsignedToStr(att->GetDirDist()) });
        }
        if (att->HasDynamDist()) {
            attributes->push_back({ "dynam.dist", att->MeasurementsignedToStr(att->GetDynamDist()) });
        }
        if (att->HasHarmDist()) {
            attributes->push_back({ "harm.dist", att->MeasurementsignedToStr(att->GetHarmDist()) });
        }
        if (att->HasRehDist()) {
            attributes->push_back({ "reh.dist", att->MeasurementsignedToStr(att->GetRehDist()) });
        }
        if (att->HasTempoDist()) {
            attributes->push_back({ "tempo.dist", att->MeasurementsignedToStr(att->GetTempoDist()) });
        }
    }
    if (element->HasAttClass(ATT_DOTLOG)) {
        const AttDotLog *att = dynamic_cast<const AttDotLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->DotLogFormToStr(att->GetForm()) });
        }
    }
    if (element->HasAttClass(ATT_DURATIONADDITIVE)) {
        const AttDurationAdditive *att = dynamic_cast<const AttDurationAdditive *>(element);
        assert(att);
        if (att->HasDur()) {
            attributes->push_back({ "dur", att->DurationToStr(att->GetDur()) });
        }
    }
    if (element->HasAttClass(ATT_DURATIONDEFAULT)) {
        const AttDurationDefault *att = dynamic_cast<const AttDurationDefault *>(element);
        assert(att);
        if (att->HasDurDefault()) {
            attributes->push_back({ "dur.default", att->DurationToStr(att->GetDurDefault()) });
        }
        if (att->HasNumDefault()) {
            attributes->push_back({ "num.default", att->IntToStr(att->GetNumDefault()) });
        }
        if (att->HasNumbaseDefault()) {
            attributes->push_back({ "numbase.default", att->IntToStr(att->GetNumbaseDefault()) });
        }
    }
    if (element->HasAttClass(ATT_DURATIONLOG)) {
        const AttDurationLog *att = dynamic_cast<const AttDurationLog *>(element);
        assert(att);
        if (att->HasDur()) {
            attributes->push_back({ "dur", att->DurationToStr(att->GetDur()) });
        }
    }
    if (element->HasAttClass(ATT_DURATIONRATIO)) {
        const AttDurationRatio *att = dynamic_cast<const AttDurationRatio *>(element);
        assert(att);
        if (att->HasNum()) {
            attributes->push_back({ "num", att->IntToStr(att->GetNum()) });
        }
        if (att->HasNumbase()) {
            attributes->push_back({ "numbase", att->IntToStr(att->GetNumbase()) });
        }
    }
    if (element->HasAttClass(ATT_ENCLOSINGCHARS)) {
        const AttEnclosingChars *att = dynamic_cast<const AttEnclosingChars *>(element);
        assert(att);
        if (att->HasEnclose()) {
            attributes->push_back({ "enclose", att->EnclosureToStr(att->GetEnclose()) });
        }
    }
    if (element->HasAttClass(ATT_ENDINGS)) {
        const AttEndings *att = dynamic_cast<const AttEndings *>(element);
        assert(att);
        if (att->HasEndingRend()) {
            attributes->push_back({ "ending.rend", att->EndingsEndingrendToStr(att->GetEndingRend()) });
        }
    }
    if (element->HasAttClass(ATT_EVIDENCE)) {
        const AttEvidence *att = dynamic_cast<const AttEvidence *>(element);
        assert(att);
        if (att->HasCert()) {
            attributes->push_back({ "cert", att->CertaintyToStr(att->GetCert()) });
        }
        if (att->HasEvidence()) {
            attributes->push_back({ "evidence", att->StrToStr(att->GetEvidence()) });
        }
    }
    if (element->HasAttClass(ATT_EXTENDER)) {
        const AttExtender *att = dynamic_cast<const AttExtender *>(element);
        assert(att);
        if (att->HasExtender()) {
            attributes->push_back({ "extender", att->BooleanToStr(att->GetExtender()) });
        }
    }
    if (element->HasAttClass(ATT_EXTENT)) {
        const AttExtent *att = dynamic_cast<const AttExtent *>(element);
        assert(att);
        if (att->HasExtent()) {
            attributes->push_back({ "extent", att->StrToStr(att->GetExtent()) });
        }
    }
    if (element->HasAttClass(ATT_FERMATAPRESENT)) {
        const AttFermataPresent *att = dynamic_cast<const AttFermataPresent *>(element);
        assert(att);
        if (att->HasFermata()) {
            attributes->push_back({ "fermata", att->StaffrelBasicToStr(att->GetFermata()) });
        }
    }
    if (element->HasAttClass(ATT_FILING)) {
        const AttFiling *att = dynamic_cast<const AttFiling *>(element);
        assert(att);
        if (att->HasNonfiling()) {
            attributes->push_back({ "nonfiling", att->IntToStr(att->GetNonfiling()) });
        }
    }
    if (element->HasAttClass(ATT_GRPSYMLOG)) {
        const AttGrpSymLog *att = dynamic_cast<const AttGrpSymLog *>(element);
        assert(att);
        if (att->HasLevel()) {
            attributes->push_back({ "level", att->IntToStr(att->GetLevel()) });
        }
    }
    if (element->HasAttClass(ATT_HANDIDENT)) {
        const AttHandIdent *att = dynamic_cast<const AttHandIdent *>(element);
        assert(att);
        if (att->HasHand()) {
            attributes->push_back({ "hand", att->StrToStr(att->GetHand()) });
        }
    }
    if (element->HasAttClass(ATT_HEIGHT)) {
        const AttHeight *att = dynamic_cast<const AttHeight *>(element);
        assert(att);
        if (att->HasHeight()) {
            attributes->push_back({ "height", att->MeasurementunsignedToStr(att->GetHeight()) });
        }
    }
    if (element->HasAttClass(ATT_HORIZONTALALIGN)) {
        const AttHorizontalAlign *att = dynamic_cast<const AttHorizontalAlign *>(element);
        assert(att);
        if (att->HasHalign()) {
            attributes->push_back({ "halign", att->HorizontalalignmentToStr(att->GetHalign()) });
        }
    }
    if (element->HasAttClass(ATT_INTERNETMEDIA)) {
        const AttInternetMedia *att = dynamic_cast<const AttInternetMedia *>(element);
        assert(att);
        if (att->HasMimetype()) {
            attributes->push_back({ "mimetype", att->StrToStr(att->GetMimetype()) });
        }
    }
    if (element->HasAttClass(ATT_JOINED)) {
        const AttJoined *att = dynamic_cast<const AttJoined *>(element);
        assert(att);
        if (att->HasJoin()) {
            attributes->push_back({ "join", att->StrToStr(att->GetJoin()) });
        }
    }
    if (element->HasAttClass(ATT_KEYSIGLOG)) {
        const AttKeySigLog *att = dynamic_cast<const AttKeySigLog *>(element);
        assert(att);
        if (att->HasSig()) {
            attributes->push_back({ "sig", att->KeysignatureToStr(att->GetSig()) });
        }
    }
    if (element->HasAttClass(ATT_KEYSIGDEFAULTLOG)) {
        const AttKeySigDefaultLog *att = dynamic_cast<const AttKeySigDefaultLog *>(element);
        assert(att);
        if (att->HasKeySig()) {
            attributes->push_back({ "key.sig", att->KeysignatureToStr(att->GetKeySig()) });
        }
    }
    if (element->HasAttClass(ATT_LABELLED)) {
        const AttLabelled *att = dynamic_cast<const AttLabelled *>(element);
        assert(att);
        if (att->HasLabel()) {
            attributes->push_back({ "label", att->StrToStr(att->GetLabel()) });
        }
    }
    if (element->HasAttClass(ATT_LANG)) {
        const AttLang *att = dynamic_cast<const AttLang *>(element);
        assert(att);
        if (att->HasLang()) {
            attributes->push_back({ "xml:lang", att->StrToStr(att->GetLang()) });
        }
        if (att->HasTranslit()) {
            attributes->push_back({ "translit", att->StrToStr(att->GetTranslit()) });
        }
    }
    if (element->HasAttClass(ATT_LAYERLOG)) {
        const AttLayerLog *att = dynamic_cast<const AttLayerLog *>(element);
        assert(att);
        if (att->HasDef()) {
            attributes->push_back({ "def", att->StrToStr(att->GetDef()) });
        }
    }
    if (element->HasAttClass(ATT_LAYERIDENT)) {
        const AttLayerIdent *att = dynamic_cast<const AttLayerIdent *>(element);
        assert(att);
        if (att->HasLayer()) {
            attributes->push_back({ "layer", att->IntToStr(att->GetLayer()) });
        }
    }
    if (element->HasAttClass(ATT_LINELOC)) {
        const AttLineLoc *att = dynamic_cast<const AttLineLoc *>(element);
        assert(att);
        if (att->HasLine()) {
            attributes->push_back({ "line", att->IntToStr(att->GetLine()) });
        }
    }
    if (element->HasAttClass(ATT_LINEREND)) {
        const AttLineRend *att = dynamic_cast<const AttLineRend *>(element);
        assert(att);
        if (att->HasLendsym()) {
            attributes->push_back({ "lendsym", att->LinestartendsymbolToStr(att->GetLendsym()) });
        }
        if (att->HasLendsymSize()) {
            attributes->push_back({ "lendsym.size", att->IntToStr(att->GetLendsymSize()) });
        }
        if (att->HasLstartsym()) {
            attributes->push_back({ "lstartsym", att->LinestartendsymbolToStr(att->GetLstartsym()) });
        }
        if (att->HasLstartsymSize()) {
            attributes->push_back({ "lstartsym.size", att->IntToStr(att->GetLstartsymSize()) });
        }
    }
    if (element->HasAttClass(ATT_LINERENDBASE)) {
        const AttLineRendBase *att = dynamic_cast<const AttLineRendBase *>(element);
        assert(att);
        if (att->HasLform()) {
            attributes->push_back({ "lform", att->LineformToStr(att->GetLform()) });
        }
        if (att->HasLsegs()) {
            attributes->push_back({ "lsegs", att->IntToStr(att->GetLsegs()) });
        }
        if (att->HasLwidth()) {
            attributes->push_back({ "lwidth", att->LinewidthToStr(att->GetLwidth()) });
        }
    }
    if (element->HasAttClass(ATT_LINKING)) {
        const AttLinking *att = dynamic_cast<const AttLinking *>(element);
        assert(att);
        if (att->HasCopyof()) {
            attributes->push_back({ "copyof", att->StrToStr(att->GetCopyof()) });
        }
        if (att->HasCorresp()) {
            attributes->push_back({ "corresp", att->StrToStr(att->GetCorresp()) });
        }
        if (att->HasFollows()) {
            attributes->push_back({ "follows", att->StrToStr(att->GetFollows()) });
        }
        if (att->HasNext()) {
            attributes->push_back({ "next", att->StrToStr(att->GetNext()) });
        }
        if (att->HasPrecedes()) {
            attributes->push_back({ "precedes", att->StrToStr(att->GetPrecedes()) });
        }
        if (att->HasPrev()) {
            attributes->push_back({ "prev", att->StrToStr(att->GetPrev()) });
        }
        if (att->HasSameas()) {
            attributes->push_back({ "sameas", att->StrToStr(att->GetSameas()) });
        }
        if (att->HasSynch()) {
            attributes->push_back({ "synch", att->StrToStr(att->GetSynch()) });
        }
    }
    if (element->HasAttClass(ATT_LYRICSTYLE)) {
        const AttLyricStyle *att = dynamic_cast<const AttLyricStyle *>(element);
        assert(att);
        if (att->HasLyricAlign()) {
            attributes->push_back({ "lyric.align", att->MeasurementsignedToStr(att->GetLyricAlign()) });
        }
        if (att->HasLyricFam()) {
            attributes->push_back({ "lyric.fam", att->StrToStr(att->GetLyricFam()) });
        }
        if (att->HasLyricName()) {
            attributes->push_back({ "lyric.name", att->StrToStr(att->GetLyricName()) });
        }
        if (att->HasLyricSize()) {
            attributes->push_back({ "lyric.size", att->FontsizeToStr(att->GetLyricSize()) });
        }
        if (att->HasLyricStyle()) {
            attributes->push_back({ "lyric.style", att->FontstyleToStr(att->GetLyricStyle()) });
        }
        if (att->HasLyricWeight()) {
            attributes->push_back({ "lyric.weight", att->FontweightToStr(att->GetLyricWeight()) });
        }
    }
    if (element->HasAttClass(ATT_MEASURENUMBERS)) {
        const AttMeasureNumbers *att = dynamic_cast<const AttMeasureNumbers *>(element);
        assert(att);
        if (att->HasMnumVisible()) {
            attributes->push_back({ "mnum.visible", att->BooleanToStr(att->GetMnumVisible()) });
        }
    }
    if (element->HasAttClass(ATT_MEASUREMENT)) {
        const AttMeasurement *att = dynamic_cast<const AttMeasurement *>(element);
        assert(att);
        if (att->HasUnit()) {
            attributes->push_back({ "unit", att->StrToStr(att->GetUnit()) });
        }
    }
    if (element->HasAttClass(ATT_MEDIABOUNDS)) {
        const AttMediaBounds *att = dynamic_cast<const AttMediaBounds *>(element);
        assert(att);
        if (att->HasBegin()) {
            attributes->push_back({ "begin", att->StrToStr(att->GetBegin()) });
        }
        if (att->HasEnd()) {
            attributes->push_back({ "end", att->StrToStr(att->GetEnd()) });
        }
        if (att->HasBetype()) {
            attributes->push_back({ "betype", att->BetypeToStr(att->GetBetype()) });
        }
    }
    if (element->HasAttClass(ATT_MEDIUM)) {
        const AttMedium *att = dynamic_cast<const AttMedium *>(element);
        assert(att);
        if (att->HasMedium()) {
            attributes->push_back({ "medium", att->StrToStr(att->GetMedium()) });
        }
    }
    if (element->HasAttClass(ATT_MEIVERSION)) {
        const AttMeiVersion *att = dynamic_cast<const AttMeiVersion *>(element);
        assert(att);
        if (att->HasMeiversion()) {
            attributes->push_back({ "meiversion", att->MeiVersionMeiversionToStr(att->GetMeiversion()) });
        }
    }
    if (element->HasAttClass(ATT_METADATAPOINTING)) {
        const AttMetadataPointing *att = dynamic_cast<const AttMetadataPointing *>(element);
        assert(att);
        if (att->HasDecls()) {
            attributes->push_back({ "decls", att->StrToStr(att->GetDecls()) });
        }
    }
    if (element->HasAttClass(ATT_METERCONFORMANCE)) {
        const AttMeterConformance *att = dynamic_cast<const AttMeterConformance *>(element);
        assert(att);
        if (att->HasMetcon()) {
            attributes->push_back({ "metcon", att->MeterConformanceMetconToStr(att->GetMetcon()) });
        }
    }
    if (element->HasAttClass(ATT_METERCONFORMANCEBAR)) {
        const AttMeterConformanceBar *att = dynamic_cast<const AttMeterConformanceBar *>(element);
        assert(att);
        if (att->HasMetcon()) {
            attributes->push_back({ "metcon", att->BooleanToStr(att->GetMetcon()) });
        }
        if (att->HasControl()) {
            attributes->push_back({ "control", att->BooleanToStr(att->GetControl()) });
        }
    }
    if (element->HasAttClass(ATT_METERSIGLOG)) {
        const AttMeterSigLog *att = dynamic_cast<const AttMeterSigLog *>(element);
        assert(att);
        if (att->HasCount()) {
            attributes->push_back({ "count", att->MetercountPairToStr(att->GetCount()) });
        }
        if (att->HasSym()) {
            attributes->push_back({ "sym", att->MetersignToStr(att->GetSym()) });
        }
        if (att->HasUnit()) {
            attributes->push_back({ "unit", att->IntToStr(att->GetUnit()) });
        }
    }
    if (element->HasAttClass(ATT_METERSIGDEFAULTLOG)) {
        const AttMeterSigDefaultLog *att = dynamic_cast<const AttMeterSigDefaultLog *>(element);
        assert(att);
        if (att->HasMeterCount()) {
            attributes->push_back({ "meter.count", att->MetercountPairToStr(att->GetMeterCount()) });
        }
        if (att->HasMeterUnit()) {
            attributes->push_back({ "meter.unit", att->IntToStr(att->GetMeterUnit()) });
        }
        if (att->HasMeterSym()) {
            attributes->push_back({ "meter.sym", att->MetersignToStr(att->GetMeterSym()) });
        }
    }
    if (element->HasAttClass(ATT_MMTEMPO)) {
        const AttMmTempo *att = dynamic_cast<const AttMmTempo *>(element);
        assert(att);
        if (att->HasMm()) {
            attributes->push_back({ "mm", att->DblToStr(att->GetMm()) });
        }
        if (att->HasMmUnit()) {
            attributes->push_back({ "mm.unit", att->DurationToStr(att->GetMmUnit()) });
        }
        if (att->HasMmDots()) {
            attributes->push_back({ "mm.dots", att->IntToStr(att->GetMmDots()) });
        }
    }
    if (element->HasAttClass(ATT_MULTINUMMEASURES)) {
        const AttMultinumMeasures *att = dynamic_cast<const AttMultinumMeasures *>(element);
        assert(att);
        if (att->HasMultiNumber()) {
            attributes->push_back({ "multi.number", att->BooleanToStr(att->GetMultiNumber()) });
        }
    }
    if (element->HasAttClass(ATT_NINTEGER)) {
        const AttNInteger *att = dynamic_cast<const AttNInteger *>(element);
        assert(att);
        if (att->HasN()) {
            attributes->push_back({ "n", att->IntToStr(att->GetN()) });
        }
    }
    if (element->HasAttClass(ATT_NNUMBERLIKE)) {
        const AttNNumberLike *att = dynamic_cast<const AttNNumberLike *>(element);
        assert(att);
        if (att->HasN()) {
            attributes->push_back({ "n", att->StrToStr(att->GetN()) });
        }
    }
    if (element->HasAttClass(ATT_NAME)) {
        const AttName *att = dynamic_cast<const AttName *>(element);
        assert(att);
        if (att->HasNymref()) {
            attributes->push_back({ "nymref", att->StrToStr(att->GetNymref()) });
        }
        if (att->HasRole()) {
            attributes->push_back({ "role", att->StrToStr(att->GetRole()) });
        }
    }
    if (element->HasAttClass(ATT_NOTATIONSTYLE)) {
        const AttNotationStyle *att = dynamic_cast<const AttNotationStyle *>(element);
        assert(att);
        if (att->HasMusicName()) {
            attributes->push_back({ "music.name", att->StrToStr(att->GetMusicName()) });
        }
        if (att->HasMusicSize()) {
            attributes->push_back({ "music.size", att->FontsizeToStr(att->GetMusicSize()) });
        }
    }
    if (element->HasAttClass(ATT_NOTEHEADS)) {
        const AttNoteHeads *att = dynamic_cast<const AttNoteHeads *>(element);
        assert(att);
        if (att->HasHeadAltsym()) {
            attributes->push_back({ "head.altsym", att->StrToStr(att->GetHeadAltsym()) });
        }
        if (att->HasHeadAuth()) {
            attributes->push_back({ "head.auth", att->StrToStr(att->GetHeadAuth()) });
        }
        if (att->HasHeadColor()) {
            attributes->push_back({ "head.color", att->StrToStr(att->GetHeadColor()) });
        }
        if (att->HasHeadFill()) {
            attributes->push_back({ "head.fill", att->FillToStr(att->GetHeadFill()) });
        }
        if (att->HasHeadFillcolor()) {
            attributes->push_back({ "head.fillcolor", att->StrToStr(att->GetHeadFillcolor()) });
        }
        if (att->HasHeadMod()) {
            attributes->push_back({ "head.mod", att->NoteheadmodifierToStr(att->GetHeadMod()) });
        }
        if (att->HasHeadRotation()) {
            attributes->push_back({ "head.rotation", att->RotationToStr(att->GetHeadRotation()) });
        }
        if (att->HasHeadShape()) {
            attributes->push_back({ "head.shape", att->HeadshapeToStr(att->GetHeadShape()) });
        }
        if (att->HasHeadVisible()) {
            attributes->push_back({ "head.visible", att->BooleanToStr(att->GetHeadVisible()) });
        }
    }
    if (element->HasAttClass(ATT_OCTAVE)) {
        const AttOctave *att = dynamic_cast<const AttOctave *>(element);
        assert(att);
        if (att->HasOct()) {
            attributes->push_back({ "oct", att->OctaveToStr(att->GetOct()) });
        }
    }
    if (element->HasAttClass(ATT_OCTAVEDEFAULT)) {
        const AttOctaveDefault *att = dynamic_cast<const AttOctaveDefault *>(element);
        assert(att);
        if (att->HasOctDefault()) {
            attributes->push_back({ "oct.default", att->OctaveToStr(att->GetOctDefault()) });
        }
    }
    if (element->HasAttClass(ATT_OCTAVEDISPLACEMENT)) {
        const AttOctaveDisplacement *att = dynamic_cast<const AttOctaveDisplacement *>(element);
        assert(att);
        if (att->HasDis()) {
            attributes->push_back({ "dis", att->OctaveDisToStr(att->GetDis()) });
        }
        if (att->HasDisPlace()) {
            attributes->push_back({ "dis.place", att->StaffrelBasicToStr(att->GetDisPlace()) });
        }
    }
    if (element->HasAttClass(ATT_ONELINESTAFF)) {
        const AttOneLineStaff *att = dynamic_cast<const AttOneLineStaff *>(element);
        assert(att);
        if (att->HasOntheline()) {
            attributes->push_back({ "ontheline", att->BooleanToStr(att->GetOntheline()) });
        }
    }
    if (element->HasAttClass(ATT_OPTIMIZATION)) {
        const AttOptimization *att = dynamic_cast<const AttOptimization *>(element);
        assert(att);
        if (att->HasOptimize()) {
            attributes->push_back({ "optimize", att->BooleanToStr(att->GetOptimize()) });
        }
    }
    if (element->HasAttClass(ATT_ORIGINLAYERIDENT)) {
        const AttOriginLayerIdent *att = dynamic_cast<const AttOriginLayerIdent *>(element);
        assert(att);
        if (att->HasOriginLayer()) {
            attributes->push_back({ "origin.layer", att->StrToStr(att->GetOriginLayer()) });
        }
    }
    if (element->HasAttClass(ATT_ORIGINSTAFFIDENT)) {
        const AttOriginStaffIdent *att = dynamic_cast<const AttOriginStaffIdent *>(element);
        assert(att);
        if (att->HasOriginStaff()) {
            attributes->push_back({ "origin.staff", att->StrToStr(att->GetOriginStaff()) });
        }
    }
    if (element->HasAttClass(ATT_ORIGINSTARTENDID)) {
        const AttOriginStartEndId *att = dynamic_cast<const AttOriginStartEndId *>(element);
        assert(att);
        if (att->HasOriginStartid()) {
            attributes->push_back({ "origin.startid", att->StrToStr(att->GetOriginStartid()) });
        }
        if (att->HasOriginEndid()) {
            attributes->push_back({ "origin.endid", att->StrToStr(att->GetOriginEndid()) });
        }
    }
    if (element->HasAttClass(ATT_ORIGINTIMESTAMPLOG)) {
        const AttOriginTimestampLog *att = dynamic_cast<const AttOriginTimestampLog *>(element);
        assert(att);
        if (att->HasOriginTstamp()) {
            attributes->push_back({ "origin.tstamp", att->MeasurebeatToStr(att->GetOriginTstamp()) });
        }
        if (att->HasOriginTstamp2()) {
            attributes->push_back({ "origin.tstamp2", att->MeasurebeatToStr(att->GetOriginTstamp2()) });
        }
    }
    if (element->HasAttClass(ATT_PAGES)) {
        const AttPages *att = dynamic_cast<const AttPages *>(element);
        assert(att);
        if (att->HasPageHeight()) {
            attributes->push_back({ "page.height", att->MeasurementunsignedToStr(att->GetPageHeight()) });
        }
        if (att->HasPageWidth()) {
            attributes->push_back({ "page.width", att->MeasurementunsignedToStr(att->GetPageWidth()) });
        }
        if (att->HasPageTopmar()) {
            attributes->push_back({ "page.topmar", att->MeasurementunsignedToStr(att->GetPageTopmar()) });
        }
        if (att->HasPageBotmar()) {
            attributes->push_back({ "page.botmar", att->MeasurementunsignedToStr(att->GetPageBotmar()) });
        }
        if (att->HasPageLeftmar()) {
            attributes->push_back({ "page.leftmar", att->MeasurementunsignedToStr(att->GetPageLeftmar()) });
        }
        if (att->HasPageRightmar()) {
            attributes->push_back({ "page.rightmar", att->MeasurementunsignedToStr(att->GetPageRightmar()) });
        }
        if (att->HasPagePanels()) {
            attributes->push_back({ "page.panels", att->StrToStr(att->GetPagePanels()) });
        }
        if (att->HasPageScale()) {
            attributes->push_back({ "page.scale", att->StrToStr(att->GetPageScale()) });
        }
    }
    if (element->HasAttClass(ATT_PARTIDENT)) {
        const AttPartIdent *att = dynamic_cast<const AttPartIdent *>(element);
        assert(att);
        if (att->HasPart()) {
            attributes->push_back({ "part", att->StrToStr(att->GetPart()) });
        }
        if (att->HasPartstaff()) {
            attributes->push_back({ "partstaff", att->StrToStr(att->GetPartstaff()) });
        }
    }
    if (element->HasAttClass(ATT_PITCH)) {
        const AttPitch *att = dynamic_cast<const AttPitch *>(element);
        assert(att);
        if (att->HasPname()) {
            attributes->push_back({ "pname", att->PitchnameToStr(att->GetPname()) });
        }
    }
    if (element->HasAttClass(ATT_PLACEMENTONSTAFF)) {
        const AttPlacementOnStaff *att = dynamic_cast<const AttPlacementOnStaff *>(element);
        assert(att);
        if (att->HasOnstaff()) {
            attributes->push_back({ "onstaff", att->BooleanToStr(att->GetOnstaff()) });
        }
    }
    if (element->HasAttClass(ATT_PLACEMENTRELEVENT)) {
        const AttPlacementRelEvent *att = dynamic_cast<const AttPlacementRelEvent *>(element);
        assert(att);
        if (att->HasPlace()) {
            attributes->push_back({ "place", att->StaffrelToStr(att->GetPlace()) });
        }
    }
    if (element->HasAttClass(ATT_PLACEMENTRELSTAFF)) {
        const AttPlacementRelStaff *att = dynamic_cast<const AttPlacementRelStaff *>(element);
        assert(att);
        if (att->HasPlace()) {
            attributes->push_back({ "place", att->StaffrelToStr(att->GetPlace()) });
        }
    }
    if (element->HasAttClass(ATT_PLIST)) {
        const AttPlist *att = dynamic_cast<const AttPlist *>(element);
        assert(att);
        if (att->HasPlist()) {
            attributes->push_back({ "plist", att->XsdAnyURIListToStr(att->GetPlist()) });
        }
    }
    if (element->HasAttClass(ATT_POINTING)) {
        const AttPointing *att = dynamic_cast<const AttPointing *>(element);
        assert(att);
        if (att->HasActuate()) {
            attributes->push_back({ "xlink:actuate", att->StrToStr(att->GetActuate()) });
        }
        if (att->HasRole()) {
            attributes->push_back({ "xlink:role", att->StrToStr(att->GetRole()) });
        }
        if (att->HasShow()) {
            attributes->push_back({ "xlink:show", att->StrToStr(att->GetShow()) });
        }
        if (att->HasTarget()) {
            attributes->push_back({ "target", att->StrToStr(att->GetTarget()) });
        }
        if (att->HasTargettype()) {
            attributes->push_back({ "targettype", att->StrToStr(att->GetTargettype()) });
        }
    }
    if (element->HasAttClass(ATT_QUANTITY)) {
        const AttQuantity *att = dynamic_cast<const AttQuantity *>(element);
        assert(att);
        if (att->HasQuantity()) {
            attributes->push_back({ "quantity", att->DblToStr(att->GetQuantity()) });
        }
    }
    if (element->HasAttClass(ATT_RANGING)) {
        const AttRanging *att = dynamic_cast<const AttRanging *>(element);
        assert(att);
        if (att->HasAtleast()) {
            attributes->push_back({ "atleast", att->DblToStr(att->GetAtleast()) });
        }
        if (att->HasAtmost()) {
            attributes->push_back({ "atmost", att->DblToStr(att->GetAtmost()) });
        }
        if (att->HasMin()) {
            attributes->push_back({ "min", att->DblToStr(att->GetMin()) });
        }
        if (att->HasMax()) {
            attributes->push_back({ "max", att->DblToStr(att->GetMax()) });
        }
        if (att->HasConfidence()) {
            attributes->push_back({ "confidence", att->DblToStr(att->GetConfidence()) });
        }
    }
    if (element->HasAttClass(ATT_RESPONSIBILITY)) {
        const AttResponsibility *att = dynamic_cast<const AttResponsibility *>(element);
        assert(att);
        if (att->HasResp()) {
            attributes->push_back({ "resp", att->StrToStr(att->GetResp()) });
        }
    }
    if (element->HasAttClass(ATT_RESTDURATIONLOG)) {
        const AttRestdurationLog *att = dynamic_cast<const AttRestdurationLog *>(element);
        assert(att);
        if (att->HasDur()) {
            attributes->push_back({ "dur", att->DurationToStr(att->GetDur()) });
        }
    }
    if (element->HasAttClass(ATT_SCALABLE)) {
        const AttScalable *att = dynamic_cast<const AttScalable *>(element);
        assert(att);
        if (att->HasScale()) {
            attributes->push_back({ "scale", att->PercentToStr(att->GetScale()) });
        }
    }
    if (element->HasAttClass(ATT_SEQUENCE)) {
        const AttSequence *att = dynamic_cast<const AttSequence *>(element);
        assert(att);
        if (att->HasSeq()) {
            attributes->push_back({ "seq", att->IntToStr(att->GetSeq()) });
        }
    }
    if (element->HasAttClass(ATT_SLASHCOUNT)) {
        const AttSlashCount *att = dynamic_cast<const AttSlashCount *>(element);
        assert(att);
        if (att->HasSlash()) {
            attributes->push_back({ "slash", att->IntToStr(att->GetSlash()) });
        }
    }
    if (element->HasAttClass(ATT_SLURPRESENT)) {
        const AttSlurPresent *att = dynamic_cast<const AttSlurPresent *>(element);
        assert(att);
        if (att->HasSlur()) {
            attributes->push_back({ "slur", att->StrToStr(att->GetSlur()) });
        }
    }
    if (element->HasAttClass(ATT_SOURCE)) {
        const AttSource *att = dynamic_cast<const AttSource *>(element);
        assert(att);
        if (att->HasSource()) {
            attributes->push_back({ "source", att->StrToStr(att->GetSource()) });
        }
    }
    if (element->HasAttClass(ATT_SPACING)) {
        const AttSpacing *att = dynamic_cast<const AttSpacing *>(element);
        assert(att);
        if (att->HasSpacingPackexp()) {
            attributes->push_back({ "spacing.packexp", att->DblToStr(att->GetSpacingPackexp()) });
        }
        if (att->HasSpacingPackfact()) {
            attributes->push_back({ "spacing.packfact", att->DblToStr(att->GetSpacingPackfact()) });
        }
        if (att->HasSpacingStaff()) {
            attributes->push_back({ "spacing.staff", att->MeasurementsignedToStr(att->GetSpacingStaff()) });
        }
        if (att->HasSpacingSystem()) {
            attributes->push_back({ "spacing.system", att->MeasurementsignedToStr(att->GetSpacingSystem()) });
        }
    }
    if (element->HasAttClass(ATT_STAFFLOG)) {
        const AttStaffLog *att = dynamic_cast<const AttStaffLog *>(element);
        assert(att);
        if (att->HasDef()) {
            attributes->push_back({ "def", att->StrToStr(att->GetDef()) });
        }
    }
    if (element->HasAttClass(ATT_STAFFDEFLOG)) {
        const AttStaffDefLog *att = dynamic_cast<const AttStaffDefLog *>(element);
        assert(att);
        if (att->HasLines()) {
            attributes->push_back({ "lines", att->IntToStr(att->GetLines()) });
        }
    }
    if (element->HasAttClass(ATT_STAFFGROUPINGSYM)) {
        const AttStaffGroupingSym *att = dynamic_cast<const AttStaffGroupingSym *>(element);
        assert(att);
        if (att->HasSymbol()) {
            attributes->push_back({ "symbol", att->StaffGroupingSymSymbolToStr(att->GetSymbol()) });
        }
    }
    if (element->HasAttClass(ATT_STAFFIDENT)) {
        const AttStaffIdent *att = dynamic_cast<const AttStaffIdent *>(element);
        assert(att);
        if (att->HasStaff()) {
            attributes->push_back({ "staff", att->XsdPositiveIntegerListToStr(att->GetStaff()) });
        }
    }
    if (element->HasAttClass(ATT_STAFFITEMS)) {
        const AttStaffItems *att = dynamic_cast<const AttStaffItems *>(element);
        assert(att);
        if (att->HasAboveorder()) {
            attributes->push_back({ "aboveorder", att->StaffitemToStr(att->GetAboveorder()) });
        }
        if (att->HasBeloworder()) {
            attributes->push_back({ "beloworder", att->StaffitemToStr(att->GetBeloworder()) });
        }
        if (att->HasBetweenorder()) {
            attributes->push_back({ "betweenorder", att->StaffitemToStr(att->GetBetweenorder()) });
        }
    }
    if (element->HasAttClass(ATT_STAFFLOC)) {
        const AttStaffLoc *att = dynamic_cast<const AttStaffLoc *>(element);
        assert(att);
        if (att->HasLoc()) {
            attributes->push_back({ "loc", att->IntToStr(att->GetLoc()) });
        }
    }
    if (element->HasAttClass(ATT_STAFFLOCPITCHED)) {
        const AttStaffLocPitched *att = dynamic_cast<const AttStaffLocPitched *>(element);
        assert(att);
        if (att->HasPloc()) {
            attributes->push_back({ "ploc", att->PitchnameToStr(att->GetPloc()) });
        }
        if (att->HasOloc()) {
            attributes->push_back({ "oloc", att->OctaveToStr(att->GetOloc()) });
        }
    }
    if (element->HasAttClass(ATT_STARTENDID)) {
        const AttStartEndId *att = dynamic_cast<const AttStartEndId *>(element);
        assert(att);
        if (att->HasEndid()) {
            attributes->push_back({ "endid", att->StrToStr(att->GetEndid()) });
        }
    }
    if (element->HasAttClass(ATT_STARTID)) {
        const AttStartId *att = dynamic_cast<const AttStartId *>(element);
        assert(att);
        if (att->HasStartid()) {
            attributes->push_back({ "startid", att->StrToStr(att->GetStartid()) });
        }
    }
    if (element->HasAttClass(ATT_STEMS)) {
        const AttStems *att = dynamic_cast<const AttStems *>(element);
        assert(att);
        if (att->HasStemDir()) {
            attributes->push_back({ "stem.dir", att->StemdirectionToStr(att->GetStemDir()) });
        }
        if (att->HasStemLen()) {
            attributes->push_back({ "stem.len", att->DblToStr(att->GetStemLen()) });
        }
        if (att->HasStemMod()) {
            attributes->push_back({ "stem.mod", att->StemmodifierToStr(att->GetStemMod()) });
        }
        if (att->HasStemPos()) {
            attributes->push_back({ "stem.pos", att->StempositionToStr(att->GetStemPos()) });
        }
        if (att->HasStemSameas()) {
            attributes->push_back({ "stem.sameas", att->StrToStr(att->GetStemSameas()) });
        }
        if (att->HasStemVisible()) {
            attributes->push_back({ "stem.visible", att->BooleanToStr(att->GetStemVisible()) });
        }
        if (att->HasStemX()) {
            attributes->push_back({ "stem.x", att->DblToStr(att->GetStemX()) });
        }
        if (att->HasStemY()) {
            attributes->push_back({ "stem.y", att->DblToStr(att->GetStemY()) });
        }
    }
    if (element->HasAttClass(ATT_SYLLOG)) {
        const AttSylLog *att = dynamic_cast<const AttSylLog *>(element);
        assert(att);
        if (att->HasCon()) {
            attributes->push_back({ "con", att->SylLogConToStr(att->GetCon()) });
        }
        if (att->HasWordpos()) {
            attributes->push_back({ "wordpos", att->SylLogWordposToStr(att->GetWordpos()) });
        }
    }
    if (element->HasAttClass(ATT_SYLTEXT)) {
        const AttSylText *att = dynamic_cast<const AttSylText *>(element);
        assert(att);
        if (att->HasSyl()) {
            attributes->push_back({ "syl", att->StrToStr(att->GetSyl()) });
        }
    }
    if (element->HasAttClass(ATT_SYSTEMS)) {
        const AttSystems *att = dynamic_cast<const AttSystems *>(element);
        assert(att);
        if (att->HasSystemLeftline()) {
            attributes->push_back({ "system.leftline", att->BooleanToStr(att->GetSystemLeftline()) });
        }
        if (att->HasSystemLeftmar()) {
            attributes->push_back({ "system.leftmar", att->MeasurementunsignedToStr(att->GetSystemLeftmar()) });
        }
        if (att->HasSystemRightmar()) {
            attributes->push_back({ "system.rightmar", att->MeasurementunsignedToStr(att->GetSystemRightmar()) });
        }
        if (att->HasSystemTopmar()) {
            attributes->push_back({ "system.topmar", att->MeasurementunsignedToStr(att->GetSystemTopmar()) });
        }
    }
    if (element->HasAttClass(ATT_TARGETEVAL)) {
        const AttTargetEval *att = dynamic_cast<const AttTargetEval *>(element);
        assert(att);
        if (att->HasEvaluate()) {
            attributes->push_back({ "evaluate", att->TargetEvalEvaluateToStr(att->GetEvaluate()) });
        }
    }
    if (element->HasAttClass(ATT_TEMPOLOG)) {
        const AttTempoLog *att = dynamic_cast<const AttTempoLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back({ "func", att->TempoLogFuncToStr(att->GetFunc()) });
        }
    }
    if (element->HasAttClass(ATT_TEXTRENDITION)) {
        const AttTextRendition *att = dynamic_cast<const AttTextRendition *>(element);
        assert(att);
        if (att->HasAltrend()) {
            attributes->push_back({ "altrend", att->StrToStr(att->GetAltrend()) });
        }
        if (att->HasRend()) {
            attributes->push_back({ "rend", att->TextrenditionToStr(att->GetRend()) });
        }
    }
    if (element->HasAttClass(ATT_TEXTSTYLE)) {
        const AttTextStyle *att = dynamic_cast<const AttTextStyle *>(element);
        assert(att);
        if (att->HasTextFam()) {
            attributes->push_back({ "text.fam", att->StrToStr(att->GetTextFam()) });
        }
        if (att->HasTextName()) {
            attributes->push_back({ "text.name", att->StrToStr(att->GetTextName()) });
        }
        if (att->HasTextSize()) {
            attributes->push_back({ "text.size", att->FontsizeToStr(att->GetTextSize()) });
        }
        if (att->HasTextStyle()) {
            attributes->push_back({ "text.style", att->FontstyleToStr(att->GetTextStyle()) });
        }
        if (att->HasTextWeight()) {
            attributes->push_back({ "text.weight", att->FontweightToStr(att->GetTextWeight()) });
        }
    }
    if (element->HasAttClass(ATT_TIEPRESENT)) {
        const AttTiePresent *att = dynamic_cast<const AttTiePresent *>(element);
        assert(att);
        if (att->HasTie()) {
            attributes->push_back({ "tie", att->TieToStr(att->GetTie()) });
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMPLOG)) {
        const AttTimestampLog *att = dynamic_cast<const AttTimestampLog *>(element);
        assert(att);
        if (att->HasTstamp()) {
            attributes->push_back({ "tstamp", att->DblToStr(att->GetTstamp()) });
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMP2LOG)) {
        const AttTimestamp2Log *att = dynamic_cast<const AttTimestamp2Log *>(element);
        assert(att);
        if (att->HasTstamp2()) {
            attributes->push_back({ "tstamp2", att->MeasurebeatToStr(att->GetTstamp2()) });
        }
    }
    if (element->HasAttClass(ATT_TRANSPOSITION)) {
        const AttTransposition *att = dynamic_cast<const AttTransposition *>(element);
        assert(att);
        if (att->HasTransDiat()) {
            attributes->push_back({ "trans.diat", att->IntToStr(att->GetTransDiat()) });
        }
        if (att->HasTransSemi()) {
            attributes->push_back({ "trans.semi", att->IntToStr(att->GetTransSemi()) });
        }
    }
    if (element->HasAttClass(ATT_TUNING)) {
        const AttTuning *att = dynamic_cast<const AttTuning *>(element);
        assert(att);
        if (att->HasTuneHz()) {
            attributes->push_back({ "tune.Hz", att->DblToStr(att->GetTuneHz()) });
        }
        if (att->HasTunePname()) {
            attributes->push_back({ "tune.pname", att->PitchnameToStr(att->GetTunePname()) });
        }
        if (att->HasTuneTemper()) {
            attributes->push_back({ "tune.temper", att->TemperamentToStr(att->GetTuneTemper()) });
        }
    }
    if (element->HasAttClass(ATT_TUPLETPRESENT)) {
        const AttTupletPresent *att = dynamic_cast<const AttTupletPresent *>(element);
        assert(att);
        if (att->HasTuplet()) {
            attributes->push_back({ "tuplet", att->StrToStr(att->GetTuplet()) });
        }
    }
    if (element->HasAttClass(ATT_TYPED)) {
        const AttTyped *att = dynamic_cast<const AttTyped *>(element);
        assert(att);
        if (att->HasType()) {
            attributes->push_back({ "type", att->StrToStr(att->GetType()) });
        }
    }
    if (element->HasAttClass(ATT_TYPOGRAPHY)) {
        const AttTypography *att = dynamic_cast<const AttTypography *>(element);
        assert(att);
        if (att->HasFontfam()) {
            attributes->push_back({ "fontfam", att->StrToStr(att->GetFontfam()) });
        }
        if (att->HasFontname()) {
            attributes->push_back({ "fontname", att->StrToStr(att->GetFontname()) });
        }
        if (att->HasFontsize()) {
            attributes->push_back({ "fontsize", att->FontsizeToStr(att->GetFontsize()) });
        }
        if (att->HasFontstyle()) {
            attributes->push_back({ "fontstyle", att->FontstyleToStr(att->GetFontstyle()) });
        }
        if (att->HasFontweight()) {
            attributes->push_back({ "fontweight", att->FontweightToStr(att->GetFontweight()) });
        }
        if (att->HasLetterspacing()) {
            attributes->push_back({ "letterspacing", att->DblToStr(att->GetLetterspacing()) });
        }
        if (att->HasLineheight()) {
            attributes->push_back({ "lineheight", att->StrToStr(att->GetLineheight()) });
        }
    }
    if (element->HasAttClass(ATT_VERTICALALIGN)) {
        const AttVerticalAlign *att = dynamic_cast<const AttVerticalAlign *>(element);
        assert(att);
        if (att->HasValign()) {
            attributes->push_back({ "valign", att->VerticalalignmentToStr(att->GetValign()) });
        }
    }
    if (element->HasAttClass(ATT_VERTICALGROUP)) {
        const AttVerticalGroup *att = dynamic_cast<const AttVerticalGroup *>(element);
        assert(att);
        if (att->HasVgrp()) {
            attributes->push_back({ "vgrp", att->IntToStr(att->GetVgrp()) });
        }
    }
    if (element->HasAttClass(ATT_VISIBILITY)) {
        const AttVisibility *att = dynamic_cast<const AttVisibility *>(element);
        assert(att);
        if (att->HasVisible()) {
            attributes->push_back({ "visible", att->BooleanToStr(att->GetVisible()) });
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSETHO)) {
        const AttVisualOffsetHo *att = dynamic_cast<const AttVisualOffsetHo *>(element);
        assert(att);
        if (att->HasHo()) {
            attributes->push_back({ "ho", att->MeasurementsignedToStr(att->GetHo()) });
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSETTO)) {
        const AttVisualOffsetTo *att = dynamic_cast<const AttVisualOffsetTo *>(element);
        assert(att);
        if (att->HasTo()) {
            attributes->push_back({ "to", att->DblToStr(att->GetTo()) });
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSETVO)) {
        const AttVisualOffsetVo *att = dynamic_cast<const AttVisualOffsetVo *>(element);
        assert(att);
        if (att->HasVo()) {
            attributes->push_back({ "vo", att->MeasurementsignedToStr(att->GetVo()) });
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSET2HO)) {
        const AttVisualOffset2Ho *att = dynamic_cast<const AttVisualOffset2Ho *>(element);
        assert(att);
        if (att->HasStartho()) {
            attributes->push_back({ "startho", att->MeasurementsignedToStr(att->GetStartho()) });
        }
        if (att->HasEndho()) {
            attributes->push_back({ "endho", att->MeasurementsignedToStr(att->GetEndho()) });
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSET2TO)) {
        const AttVisualOffset2To *att = dynamic_cast<const AttVisualOffset2To *>(element);
        assert(att);
        if (att->HasStartto()) {
            attributes->push_back({ "startto", att->DblToStr(att->GetStartto()) });
        }
        if (att->HasEndto()) {
            attributes->push_back({ "endto", att->DblToStr(att->GetEndto()) });
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSET2VO)) {
        const AttVisualOffset2Vo *att = dynamic_cast<const AttVisualOffset2Vo *>(element);
        assert(att);
        if (att->HasStartvo()) {
            attributes->push_back({ "startvo", att->MeasurementsignedToStr(att->GetStartvo()) });
        }
        if (att->HasEndvo()) {
            attributes->push_back({ "endvo", att->MeasurementsignedToStr(att->GetEndvo()) });
        }
    }
    if (element->HasAttClass(ATT_VOLTAGROUPINGSYM)) {
        const AttVoltaGroupingSym *att = dynamic_cast<const AttVoltaGroupingSym *>(element);
        assert(att);
        if (att->HasVoltasym()) {
            attributes->push_back({ "voltasym", att->VoltaGroupingSymVoltasymToStr(att->GetVoltasym()) });
        }
    }
    if (element->HasAttClass(ATT_WHITESPACE)) {
        const AttWhitespace *att = dynamic_cast<const AttWhitespace *>(element);
        assert(att);
        if (att->HasSpace()) {
            attributes->push_back({ "xml:space", att->StrToStr(att->GetSpace()) });
        }
    }
    if (element->HasAttClass(ATT_WIDTH)) {
        const AttWidth *att = dynamic_cast<const AttWidth *>(element);
        assert(att);
        if (att->HasWidth()) {
            attributes->push_back({ "width", att->MeasurementunsignedToStr(att->GetWidth()) });
        }
    }
    if (element->HasAttClass(ATT_XY)) {
        const AttXy *att = dynamic_cast<const AttXy *>(element);
        assert(att);
        if (att->HasX()) {
            attributes->push_back({ "x", att->DblToStr(att->GetX()) });
        }
        if (att->HasY()) {
            attributes->push_back({ "y", att->DblToStr(att->GetY()) });
        }
    }
    if (element->HasAttClass(ATT_XY2)) {
        const AttXy2 *att = dynamic_cast<const AttXy2 *>(element);
        assert(att);
        if (att->HasX2()) {
            attributes->push_back({ "x2", att->DblToStr(att->GetX2()) });
        }
        if (att->HasY2()) {
            attributes->push_back({ "y2", att->DblToStr(att->GetY2()) });
        }
    }
}

} // namespace vrv

#include "atts_usersymbols.h"

namespace vrv {

//----------------------------------------------------------------------------
// Usersymbols
//----------------------------------------------------------------------------

bool AttModule::SetUsersymbols(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_ALTSYM)) {
        AttAltSym *att = dynamic_cast<AttAltSym *>(element);
        assert(att);
        if (attrType == "altsym") {
            att->SetAltsym(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ANCHOREDTEXTLOG)) {
        AttAnchoredTextLog *att = dynamic_cast<AttAnchoredTextLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CURVELOG)) {
        AttCurveLog *att = dynamic_cast<AttCurveLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LINELOG)) {
        AttLineLog *att = dynamic_cast<AttLineLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetUsersymbols(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ALTSYM)) {
        const AttAltSym *att = dynamic_cast<const AttAltSym *>(element);
        assert(att);
        if (att->HasAltsym()) {
            attributes->push_back({ "altsym", att->StrToStr(att->GetAltsym()) });
        }
    }
    if (element->HasAttClass(ATT_ANCHOREDTEXTLOG)) {
        const AttAnchoredTextLog *att = dynamic_cast<const AttAnchoredTextLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back({ "func", att->StrToStr(att->GetFunc()) });
        }
    }
    if (element->HasAttClass(ATT_CURVELOG)) {
        const AttCurveLog *att = dynamic_cast<const AttCurveLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back({ "func", att->StrToStr(att->GetFunc()) });
        }
    }
    if (element->HasAttClass(ATT_LINELOG)) {
        const AttLineLog *att = dynamic_cast<const AttLineLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back({ "func", att->StrToStr(att->GetFunc()) });
        }
    }
}

} // namespace vrv

#include "atts_visual.h"

namespace vrv {

//----------------------------------------------------------------------------
// Visual
//----------------------------------------------------------------------------

bool AttModule::SetVisual(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_ANNOTVIS)) {
        AttAnnotVis *att = dynamic_cast<AttAnnotVis *>(element);
        assert(att);
        if (attrType == "place") {
            att->SetPlace(att->StrToPlacement(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ARPEGVIS)) {
        AttArpegVis *att = dynamic_cast<AttArpegVis *>(element);
        assert(att);
        if (attrType == "arrow") {
            att->SetArrow(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "arrow.shape") {
            att->SetArrowShape(att->StrToLinestartendsymbol(attrValue));
            return true;
        }
        if (attrType == "arrow.size") {
            att->SetArrowSize(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "arrow.color") {
            att->SetArrowColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "arrow.fillcolor") {
            att->SetArrowFillcolor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "line.form") {
            att->SetLineForm(att->StrToLineform(attrValue));
            return true;
        }
        if (attrType == "line.width") {
            att->SetLineWidth(att->StrToLinewidth(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BARLINEVIS)) {
        AttBarLineVis *att = dynamic_cast<AttBarLineVis *>(element);
        assert(att);
        if (attrType == "len") {
            att->SetLen(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "method") {
            att->SetMethod(att->StrToBarmethod(attrValue));
            return true;
        }
        if (attrType == "place") {
            att->SetPlace(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMINGVIS)) {
        AttBeamingVis *att = dynamic_cast<AttBeamingVis *>(element);
        assert(att);
        if (attrType == "beam.color") {
            att->SetBeamColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "beam.rend") {
            att->SetBeamRend(att->StrToBeamingVisBeamrend(attrValue));
            return true;
        }
        if (attrType == "beam.slope") {
            att->SetBeamSlope(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEATRPTVIS)) {
        AttBeatRptVis *att = dynamic_cast<AttBeatRptVis *>(element);
        assert(att);
        if (attrType == "slash") {
            att->SetSlash(att->StrToBeatrptRend(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CHORDVIS)) {
        AttChordVis *att = dynamic_cast<AttChordVis *>(element);
        assert(att);
        if (attrType == "cluster") {
            att->SetCluster(att->StrToCluster(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CLEFFINGVIS)) {
        AttCleffingVis *att = dynamic_cast<AttCleffingVis *>(element);
        assert(att);
        if (attrType == "clef.color") {
            att->SetClefColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "clef.visible") {
            att->SetClefVisible(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_EPISEMAVIS)) {
        AttEpisemaVis *att = dynamic_cast<AttEpisemaVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToEpisemaVisForm(attrValue));
            return true;
        }
        if (attrType == "place") {
            att->SetPlace(att->StrToEventrel(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FTREMVIS)) {
        AttFTremVis *att = dynamic_cast<AttFTremVis *>(element);
        assert(att);
        if (attrType == "beams") {
            att->SetBeams(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "beams.float") {
            att->SetBeamsFloat(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "float.gap") {
            att->SetFloatGap(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FERMATAVIS)) {
        AttFermataVis *att = dynamic_cast<AttFermataVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToFermataVisForm(attrValue));
            return true;
        }
        if (attrType == "shape") {
            att->SetShape(att->StrToFermataVisShape(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FINGGRPVIS)) {
        AttFingGrpVis *att = dynamic_cast<AttFingGrpVis *>(element);
        assert(att);
        if (attrType == "orient") {
            att->SetOrient(att->StrToFingGrpVisOrient(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HAIRPINVIS)) {
        AttHairpinVis *att = dynamic_cast<AttHairpinVis *>(element);
        assert(att);
        if (attrType == "opening") {
            att->SetOpening(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "closed") {
            att->SetClosed(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "opening.vertical") {
            att->SetOpeningVertical(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "angle.optimize") {
            att->SetAngleOptimize(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HARMVIS)) {
        AttHarmVis *att = dynamic_cast<AttHarmVis *>(element);
        assert(att);
        if (attrType == "rendgrid") {
            att->SetRendgrid(att->StrToHarmVisRendgrid(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HISPANTICKVIS)) {
        AttHispanTickVis *att = dynamic_cast<AttHispanTickVis *>(element);
        assert(att);
        if (attrType == "place") {
            att->SetPlace(att->StrToEventrel(attrValue));
            return true;
        }
        if (attrType == "tilt") {
            att->SetTilt(att->StrToCompassdirection(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_KEYSIGVIS)) {
        AttKeySigVis *att = dynamic_cast<AttKeySigVis *>(element);
        assert(att);
        if (attrType == "sig.showchange") {
            att->SetSigShowchange(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_KEYSIGDEFAULTVIS)) {
        AttKeySigDefaultVis *att = dynamic_cast<AttKeySigDefaultVis *>(element);
        assert(att);
        if (attrType == "keysig.show") {
            att->SetKeysigShow(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "keysig.showchange") {
            att->SetKeysigShowchange(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LIGATUREVIS)) {
        AttLigatureVis *att = dynamic_cast<AttLigatureVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToLigatureform(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LINEVIS)) {
        AttLineVis *att = dynamic_cast<AttLineVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToLineform(attrValue));
            return true;
        }
        if (attrType == "width") {
            att->SetWidth(att->StrToLinewidth(attrValue));
            return true;
        }
        if (attrType == "endsym") {
            att->SetEndsym(att->StrToLinestartendsymbol(attrValue));
            return true;
        }
        if (attrType == "endsym.size") {
            att->SetEndsymSize(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "startsym") {
            att->SetStartsym(att->StrToLinestartendsymbol(attrValue));
            return true;
        }
        if (attrType == "startsym.size") {
            att->SetStartsymSize(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LIQUESCENTVIS)) {
        AttLiquescentVis *att = dynamic_cast<AttLiquescentVis *>(element);
        assert(att);
        if (attrType == "curve") {
            att->SetCurve(att->StrToLiquescentVisCurve(attrValue));
            return true;
        }
        if (attrType == "looped") {
            att->SetLooped(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MENSURVIS)) {
        AttMensurVis *att = dynamic_cast<AttMensurVis *>(element);
        assert(att);
        if (attrType == "dot") {
            att->SetDot(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "form") {
            att->SetForm(att->StrToMensurVisForm(attrValue));
            return true;
        }
        if (attrType == "orient") {
            att->SetOrient(att->StrToOrientation(attrValue));
            return true;
        }
        if (attrType == "sign") {
            att->SetSign(att->StrToMensurationsign(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MENSURALVIS)) {
        AttMensuralVis *att = dynamic_cast<AttMensuralVis *>(element);
        assert(att);
        if (attrType == "mensur.color") {
            att->SetMensurColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "mensur.dot") {
            att->SetMensurDot(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "mensur.form") {
            att->SetMensurForm(att->StrToMensuralVisMensurform(attrValue));
            return true;
        }
        if (attrType == "mensur.loc") {
            att->SetMensurLoc(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "mensur.orient") {
            att->SetMensurOrient(att->StrToOrientation(attrValue));
            return true;
        }
        if (attrType == "mensur.sign") {
            att->SetMensurSign(att->StrToMensurationsign(attrValue));
            return true;
        }
        if (attrType == "mensur.size") {
            att->SetMensurSize(att->StrToFontsize(attrValue));
            return true;
        }
        if (attrType == "mensur.slash") {
            att->SetMensurSlash(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERSIGVIS)) {
        AttMeterSigVis *att = dynamic_cast<AttMeterSigVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToMeterform(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERSIGDEFAULTVIS)) {
        AttMeterSigDefaultVis *att = dynamic_cast<AttMeterSigDefaultVis *>(element);
        assert(att);
        if (attrType == "meter.form") {
            att->SetMeterForm(att->StrToMeterform(attrValue));
            return true;
        }
        if (attrType == "meter.showchange") {
            att->SetMeterShowchange(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MULTIRESTVIS)) {
        AttMultiRestVis *att = dynamic_cast<AttMultiRestVis *>(element);
        assert(att);
        if (attrType == "block") {
            att->SetBlock(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PBVIS)) {
        AttPbVis *att = dynamic_cast<AttPbVis *>(element);
        assert(att);
        if (attrType == "folium") {
            att->SetFolium(att->StrToPbVisFolium(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PEDALVIS)) {
        AttPedalVis *att = dynamic_cast<AttPedalVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToPedalstyle(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PLICAVIS)) {
        AttPlicaVis *att = dynamic_cast<AttPlicaVis *>(element);
        assert(att);
        if (attrType == "dir") {
            att->SetDir(att->StrToStemdirectionBasic(attrValue));
            return true;
        }
        if (attrType == "len") {
            att->SetLen(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_QUILISMAVIS)) {
        AttQuilismaVis *att = dynamic_cast<AttQuilismaVis *>(element);
        assert(att);
        if (attrType == "waves") {
            att->SetWaves(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SBVIS)) {
        AttSbVis *att = dynamic_cast<AttSbVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToSbVisForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SCOREDEFVIS)) {
        AttScoreDefVis *att = dynamic_cast<AttScoreDefVis *>(element);
        assert(att);
        if (attrType == "vu.height") {
            att->SetVuHeight(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SECTIONVIS)) {
        AttSectionVis *att = dynamic_cast<AttSectionVis *>(element);
        assert(att);
        if (attrType == "restart") {
            att->SetRestart(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SIGNIFLETVIS)) {
        AttSignifLetVis *att = dynamic_cast<AttSignifLetVis *>(element);
        assert(att);
        if (attrType == "place") {
            att->SetPlace(att->StrToEventrel(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SPACEVIS)) {
        AttSpaceVis *att = dynamic_cast<AttSpaceVis *>(element);
        assert(att);
        if (attrType == "compressable") {
            att->SetCompressable(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFDEFVIS)) {
        AttStaffDefVis *att = dynamic_cast<AttStaffDefVis *>(element);
        assert(att);
        if (attrType == "grid.show") {
            att->SetGridShow(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "layerscheme") {
            att->SetLayerscheme(att->StrToLayerscheme(attrValue));
            return true;
        }
        if (attrType == "lines.color") {
            att->SetLinesColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "lines.visible") {
            att->SetLinesVisible(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "spacing") {
            att->SetSpacing(att->StrToMeasurementsigned(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFGRPVIS)) {
        AttStaffGrpVis *att = dynamic_cast<AttStaffGrpVis *>(element);
        assert(att);
        if (attrType == "bar.thru") {
            att->SetBarThru(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STEMVIS)) {
        AttStemVis *att = dynamic_cast<AttStemVis *>(element);
        assert(att);
        if (attrType == "pos") {
            att->SetPos(att->StrToStemposition(attrValue));
            return true;
        }
        if (attrType == "len") {
            att->SetLen(att->StrToMeasurementunsigned(attrValue));
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
    if (element->HasAttClass(ATT_TUPLETVIS)) {
        AttTupletVis *att = dynamic_cast<AttTupletVis *>(element);
        assert(att);
        if (attrType == "bracket.place") {
            att->SetBracketPlace(att->StrToStaffrelBasic(attrValue));
            return true;
        }
        if (attrType == "bracket.visible") {
            att->SetBracketVisible(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "dur.visible") {
            att->SetDurVisible(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "num.format") {
            att->SetNumFormat(att->StrToTupletVisNumformat(attrValue));
            return true;
        }
    }

    return false;
}

void AttModule::GetVisual(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ANNOTVIS)) {
        const AttAnnotVis *att = dynamic_cast<const AttAnnotVis *>(element);
        assert(att);
        if (att->HasPlace()) {
            attributes->push_back({ "place", att->PlacementToStr(att->GetPlace()) });
        }
    }
    if (element->HasAttClass(ATT_ARPEGVIS)) {
        const AttArpegVis *att = dynamic_cast<const AttArpegVis *>(element);
        assert(att);
        if (att->HasArrow()) {
            attributes->push_back({ "arrow", att->BooleanToStr(att->GetArrow()) });
        }
        if (att->HasArrowShape()) {
            attributes->push_back({ "arrow.shape", att->LinestartendsymbolToStr(att->GetArrowShape()) });
        }
        if (att->HasArrowSize()) {
            attributes->push_back({ "arrow.size", att->IntToStr(att->GetArrowSize()) });
        }
        if (att->HasArrowColor()) {
            attributes->push_back({ "arrow.color", att->StrToStr(att->GetArrowColor()) });
        }
        if (att->HasArrowFillcolor()) {
            attributes->push_back({ "arrow.fillcolor", att->StrToStr(att->GetArrowFillcolor()) });
        }
        if (att->HasLineForm()) {
            attributes->push_back({ "line.form", att->LineformToStr(att->GetLineForm()) });
        }
        if (att->HasLineWidth()) {
            attributes->push_back({ "line.width", att->LinewidthToStr(att->GetLineWidth()) });
        }
    }
    if (element->HasAttClass(ATT_BARLINEVIS)) {
        const AttBarLineVis *att = dynamic_cast<const AttBarLineVis *>(element);
        assert(att);
        if (att->HasLen()) {
            attributes->push_back({ "len", att->DblToStr(att->GetLen()) });
        }
        if (att->HasMethod()) {
            attributes->push_back({ "method", att->BarmethodToStr(att->GetMethod()) });
        }
        if (att->HasPlace()) {
            attributes->push_back({ "place", att->IntToStr(att->GetPlace()) });
        }
    }
    if (element->HasAttClass(ATT_BEAMINGVIS)) {
        const AttBeamingVis *att = dynamic_cast<const AttBeamingVis *>(element);
        assert(att);
        if (att->HasBeamColor()) {
            attributes->push_back({ "beam.color", att->StrToStr(att->GetBeamColor()) });
        }
        if (att->HasBeamRend()) {
            attributes->push_back({ "beam.rend", att->BeamingVisBeamrendToStr(att->GetBeamRend()) });
        }
        if (att->HasBeamSlope()) {
            attributes->push_back({ "beam.slope", att->DblToStr(att->GetBeamSlope()) });
        }
    }
    if (element->HasAttClass(ATT_BEATRPTVIS)) {
        const AttBeatRptVis *att = dynamic_cast<const AttBeatRptVis *>(element);
        assert(att);
        if (att->HasSlash()) {
            attributes->push_back({ "slash", att->BeatrptRendToStr(att->GetSlash()) });
        }
    }
    if (element->HasAttClass(ATT_CHORDVIS)) {
        const AttChordVis *att = dynamic_cast<const AttChordVis *>(element);
        assert(att);
        if (att->HasCluster()) {
            attributes->push_back({ "cluster", att->ClusterToStr(att->GetCluster()) });
        }
    }
    if (element->HasAttClass(ATT_CLEFFINGVIS)) {
        const AttCleffingVis *att = dynamic_cast<const AttCleffingVis *>(element);
        assert(att);
        if (att->HasClefColor()) {
            attributes->push_back({ "clef.color", att->StrToStr(att->GetClefColor()) });
        }
        if (att->HasClefVisible()) {
            attributes->push_back({ "clef.visible", att->BooleanToStr(att->GetClefVisible()) });
        }
    }
    if (element->HasAttClass(ATT_EPISEMAVIS)) {
        const AttEpisemaVis *att = dynamic_cast<const AttEpisemaVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->EpisemaVisFormToStr(att->GetForm()) });
        }
        if (att->HasPlace()) {
            attributes->push_back({ "place", att->EventrelToStr(att->GetPlace()) });
        }
    }
    if (element->HasAttClass(ATT_FTREMVIS)) {
        const AttFTremVis *att = dynamic_cast<const AttFTremVis *>(element);
        assert(att);
        if (att->HasBeams()) {
            attributes->push_back({ "beams", att->IntToStr(att->GetBeams()) });
        }
        if (att->HasBeamsFloat()) {
            attributes->push_back({ "beams.float", att->IntToStr(att->GetBeamsFloat()) });
        }
        if (att->HasFloatGap()) {
            attributes->push_back({ "float.gap", att->MeasurementunsignedToStr(att->GetFloatGap()) });
        }
    }
    if (element->HasAttClass(ATT_FERMATAVIS)) {
        const AttFermataVis *att = dynamic_cast<const AttFermataVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->FermataVisFormToStr(att->GetForm()) });
        }
        if (att->HasShape()) {
            attributes->push_back({ "shape", att->FermataVisShapeToStr(att->GetShape()) });
        }
    }
    if (element->HasAttClass(ATT_FINGGRPVIS)) {
        const AttFingGrpVis *att = dynamic_cast<const AttFingGrpVis *>(element);
        assert(att);
        if (att->HasOrient()) {
            attributes->push_back({ "orient", att->FingGrpVisOrientToStr(att->GetOrient()) });
        }
    }
    if (element->HasAttClass(ATT_HAIRPINVIS)) {
        const AttHairpinVis *att = dynamic_cast<const AttHairpinVis *>(element);
        assert(att);
        if (att->HasOpening()) {
            attributes->push_back({ "opening", att->MeasurementunsignedToStr(att->GetOpening()) });
        }
        if (att->HasClosed()) {
            attributes->push_back({ "closed", att->BooleanToStr(att->GetClosed()) });
        }
        if (att->HasOpeningVertical()) {
            attributes->push_back({ "opening.vertical", att->BooleanToStr(att->GetOpeningVertical()) });
        }
        if (att->HasAngleOptimize()) {
            attributes->push_back({ "angle.optimize", att->BooleanToStr(att->GetAngleOptimize()) });
        }
    }
    if (element->HasAttClass(ATT_HARMVIS)) {
        const AttHarmVis *att = dynamic_cast<const AttHarmVis *>(element);
        assert(att);
        if (att->HasRendgrid()) {
            attributes->push_back({ "rendgrid", att->HarmVisRendgridToStr(att->GetRendgrid()) });
        }
    }
    if (element->HasAttClass(ATT_HISPANTICKVIS)) {
        const AttHispanTickVis *att = dynamic_cast<const AttHispanTickVis *>(element);
        assert(att);
        if (att->HasPlace()) {
            attributes->push_back({ "place", att->EventrelToStr(att->GetPlace()) });
        }
        if (att->HasTilt()) {
            attributes->push_back({ "tilt", att->CompassdirectionToStr(att->GetTilt()) });
        }
    }
    if (element->HasAttClass(ATT_KEYSIGVIS)) {
        const AttKeySigVis *att = dynamic_cast<const AttKeySigVis *>(element);
        assert(att);
        if (att->HasSigShowchange()) {
            attributes->push_back({ "sig.showchange", att->BooleanToStr(att->GetSigShowchange()) });
        }
    }
    if (element->HasAttClass(ATT_KEYSIGDEFAULTVIS)) {
        const AttKeySigDefaultVis *att = dynamic_cast<const AttKeySigDefaultVis *>(element);
        assert(att);
        if (att->HasKeysigShow()) {
            attributes->push_back({ "keysig.show", att->BooleanToStr(att->GetKeysigShow()) });
        }
        if (att->HasKeysigShowchange()) {
            attributes->push_back({ "keysig.showchange", att->BooleanToStr(att->GetKeysigShowchange()) });
        }
    }
    if (element->HasAttClass(ATT_LIGATUREVIS)) {
        const AttLigatureVis *att = dynamic_cast<const AttLigatureVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->LigatureformToStr(att->GetForm()) });
        }
    }
    if (element->HasAttClass(ATT_LINEVIS)) {
        const AttLineVis *att = dynamic_cast<const AttLineVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->LineformToStr(att->GetForm()) });
        }
        if (att->HasWidth()) {
            attributes->push_back({ "width", att->LinewidthToStr(att->GetWidth()) });
        }
        if (att->HasEndsym()) {
            attributes->push_back({ "endsym", att->LinestartendsymbolToStr(att->GetEndsym()) });
        }
        if (att->HasEndsymSize()) {
            attributes->push_back({ "endsym.size", att->IntToStr(att->GetEndsymSize()) });
        }
        if (att->HasStartsym()) {
            attributes->push_back({ "startsym", att->LinestartendsymbolToStr(att->GetStartsym()) });
        }
        if (att->HasStartsymSize()) {
            attributes->push_back({ "startsym.size", att->IntToStr(att->GetStartsymSize()) });
        }
    }
    if (element->HasAttClass(ATT_LIQUESCENTVIS)) {
        const AttLiquescentVis *att = dynamic_cast<const AttLiquescentVis *>(element);
        assert(att);
        if (att->HasCurve()) {
            attributes->push_back({ "curve", att->LiquescentVisCurveToStr(att->GetCurve()) });
        }
        if (att->HasLooped()) {
            attributes->push_back({ "looped", att->BooleanToStr(att->GetLooped()) });
        }
    }
    if (element->HasAttClass(ATT_MENSURVIS)) {
        const AttMensurVis *att = dynamic_cast<const AttMensurVis *>(element);
        assert(att);
        if (att->HasDot()) {
            attributes->push_back({ "dot", att->BooleanToStr(att->GetDot()) });
        }
        if (att->HasForm()) {
            attributes->push_back({ "form", att->MensurVisFormToStr(att->GetForm()) });
        }
        if (att->HasOrient()) {
            attributes->push_back({ "orient", att->OrientationToStr(att->GetOrient()) });
        }
        if (att->HasSign()) {
            attributes->push_back({ "sign", att->MensurationsignToStr(att->GetSign()) });
        }
    }
    if (element->HasAttClass(ATT_MENSURALVIS)) {
        const AttMensuralVis *att = dynamic_cast<const AttMensuralVis *>(element);
        assert(att);
        if (att->HasMensurColor()) {
            attributes->push_back({ "mensur.color", att->StrToStr(att->GetMensurColor()) });
        }
        if (att->HasMensurDot()) {
            attributes->push_back({ "mensur.dot", att->BooleanToStr(att->GetMensurDot()) });
        }
        if (att->HasMensurForm()) {
            attributes->push_back({ "mensur.form", att->MensuralVisMensurformToStr(att->GetMensurForm()) });
        }
        if (att->HasMensurLoc()) {
            attributes->push_back({ "mensur.loc", att->IntToStr(att->GetMensurLoc()) });
        }
        if (att->HasMensurOrient()) {
            attributes->push_back({ "mensur.orient", att->OrientationToStr(att->GetMensurOrient()) });
        }
        if (att->HasMensurSign()) {
            attributes->push_back({ "mensur.sign", att->MensurationsignToStr(att->GetMensurSign()) });
        }
        if (att->HasMensurSize()) {
            attributes->push_back({ "mensur.size", att->FontsizeToStr(att->GetMensurSize()) });
        }
        if (att->HasMensurSlash()) {
            attributes->push_back({ "mensur.slash", att->IntToStr(att->GetMensurSlash()) });
        }
    }
    if (element->HasAttClass(ATT_METERSIGVIS)) {
        const AttMeterSigVis *att = dynamic_cast<const AttMeterSigVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->MeterformToStr(att->GetForm()) });
        }
    }
    if (element->HasAttClass(ATT_METERSIGDEFAULTVIS)) {
        const AttMeterSigDefaultVis *att = dynamic_cast<const AttMeterSigDefaultVis *>(element);
        assert(att);
        if (att->HasMeterForm()) {
            attributes->push_back({ "meter.form", att->MeterformToStr(att->GetMeterForm()) });
        }
        if (att->HasMeterShowchange()) {
            attributes->push_back({ "meter.showchange", att->BooleanToStr(att->GetMeterShowchange()) });
        }
    }
    if (element->HasAttClass(ATT_MULTIRESTVIS)) {
        const AttMultiRestVis *att = dynamic_cast<const AttMultiRestVis *>(element);
        assert(att);
        if (att->HasBlock()) {
            attributes->push_back({ "block", att->BooleanToStr(att->GetBlock()) });
        }
    }
    if (element->HasAttClass(ATT_PBVIS)) {
        const AttPbVis *att = dynamic_cast<const AttPbVis *>(element);
        assert(att);
        if (att->HasFolium()) {
            attributes->push_back({ "folium", att->PbVisFoliumToStr(att->GetFolium()) });
        }
    }
    if (element->HasAttClass(ATT_PEDALVIS)) {
        const AttPedalVis *att = dynamic_cast<const AttPedalVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->PedalstyleToStr(att->GetForm()) });
        }
    }
    if (element->HasAttClass(ATT_PLICAVIS)) {
        const AttPlicaVis *att = dynamic_cast<const AttPlicaVis *>(element);
        assert(att);
        if (att->HasDir()) {
            attributes->push_back({ "dir", att->StemdirectionBasicToStr(att->GetDir()) });
        }
        if (att->HasLen()) {
            attributes->push_back({ "len", att->MeasurementunsignedToStr(att->GetLen()) });
        }
    }
    if (element->HasAttClass(ATT_QUILISMAVIS)) {
        const AttQuilismaVis *att = dynamic_cast<const AttQuilismaVis *>(element);
        assert(att);
        if (att->HasWaves()) {
            attributes->push_back({ "waves", att->IntToStr(att->GetWaves()) });
        }
    }
    if (element->HasAttClass(ATT_SBVIS)) {
        const AttSbVis *att = dynamic_cast<const AttSbVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back({ "form", att->SbVisFormToStr(att->GetForm()) });
        }
    }
    if (element->HasAttClass(ATT_SCOREDEFVIS)) {
        const AttScoreDefVis *att = dynamic_cast<const AttScoreDefVis *>(element);
        assert(att);
        if (att->HasVuHeight()) {
            attributes->push_back({ "vu.height", att->StrToStr(att->GetVuHeight()) });
        }
    }
    if (element->HasAttClass(ATT_SECTIONVIS)) {
        const AttSectionVis *att = dynamic_cast<const AttSectionVis *>(element);
        assert(att);
        if (att->HasRestart()) {
            attributes->push_back({ "restart", att->BooleanToStr(att->GetRestart()) });
        }
    }
    if (element->HasAttClass(ATT_SIGNIFLETVIS)) {
        const AttSignifLetVis *att = dynamic_cast<const AttSignifLetVis *>(element);
        assert(att);
        if (att->HasPlace()) {
            attributes->push_back({ "place", att->EventrelToStr(att->GetPlace()) });
        }
    }
    if (element->HasAttClass(ATT_SPACEVIS)) {
        const AttSpaceVis *att = dynamic_cast<const AttSpaceVis *>(element);
        assert(att);
        if (att->HasCompressable()) {
            attributes->push_back({ "compressable", att->BooleanToStr(att->GetCompressable()) });
        }
    }
    if (element->HasAttClass(ATT_STAFFDEFVIS)) {
        const AttStaffDefVis *att = dynamic_cast<const AttStaffDefVis *>(element);
        assert(att);
        if (att->HasGridShow()) {
            attributes->push_back({ "grid.show", att->BooleanToStr(att->GetGridShow()) });
        }
        if (att->HasLayerscheme()) {
            attributes->push_back({ "layerscheme", att->LayerschemeToStr(att->GetLayerscheme()) });
        }
        if (att->HasLinesColor()) {
            attributes->push_back({ "lines.color", att->StrToStr(att->GetLinesColor()) });
        }
        if (att->HasLinesVisible()) {
            attributes->push_back({ "lines.visible", att->BooleanToStr(att->GetLinesVisible()) });
        }
        if (att->HasSpacing()) {
            attributes->push_back({ "spacing", att->MeasurementsignedToStr(att->GetSpacing()) });
        }
    }
    if (element->HasAttClass(ATT_STAFFGRPVIS)) {
        const AttStaffGrpVis *att = dynamic_cast<const AttStaffGrpVis *>(element);
        assert(att);
        if (att->HasBarThru()) {
            attributes->push_back({ "bar.thru", att->BooleanToStr(att->GetBarThru()) });
        }
    }
    if (element->HasAttClass(ATT_STEMVIS)) {
        const AttStemVis *att = dynamic_cast<const AttStemVis *>(element);
        assert(att);
        if (att->HasPos()) {
            attributes->push_back({ "pos", att->StempositionToStr(att->GetPos()) });
        }
        if (att->HasLen()) {
            attributes->push_back({ "len", att->MeasurementunsignedToStr(att->GetLen()) });
        }
        if (att->HasForm()) {
            attributes->push_back({ "form", att->StemformMensuralToStr(att->GetForm()) });
        }
        if (att->HasDir()) {
            attributes->push_back({ "dir", att->StemdirectionToStr(att->GetDir()) });
        }
        if (att->HasFlagPos()) {
            attributes->push_back({ "flag.pos", att->FlagposMensuralToStr(att->GetFlagPos()) });
        }
        if (att->HasFlagForm()) {
            attributes->push_back({ "flag.form", att->FlagformMensuralToStr(att->GetFlagForm()) });
        }
    }
    if (element->HasAttClass(ATT_TUPLETVIS)) {
        const AttTupletVis *att = dynamic_cast<const AttTupletVis *>(element);
        assert(att);
        if (att->HasBracketPlace()) {
            attributes->push_back({ "bracket.place", att->StaffrelBasicToStr(att->GetBracketPlace()) });
        }
        if (att->HasBracketVisible()) {
            attributes->push_back({ "bracket.visible", att->BooleanToStr(att->GetBracketVisible()) });
        }
        if (att->HasDurVisible()) {
            attributes->push_back({ "dur.visible", att->BooleanToStr(att->GetDurVisible()) });
        }
        if (att->HasNumFormat()) {
            attributes->push_back({ "num.format", att->TupletVisNumformatToStr(att->GetNumFormat()) });
        }
    }
}

} // namespace vrv
