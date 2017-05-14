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

#ifndef __VRV_ATT_CONVERTER_H__
#define __VRV_ATT_CONVERTER_H__

#include <string>

//----------------------------------------------------------------------------

#include "attdef.h"

namespace vrv {

//----------------------------------------------------------------------------
// AttConverter
//----------------------------------------------------------------------------

class AttConverter {
public:
    std::string AccidentalWrittenToStr(data_ACCIDENTAL_WRITTEN data) const;
    data_ACCIDENTAL_WRITTEN StrToAccidentalWritten(std::string value) const;

    std::string ArticulationToStr(data_ARTICULATION data) const;
    data_ARTICULATION StrToArticulation(std::string value) const;

    std::string BarplaceToStr(data_BARPLACE data) const;
    data_BARPLACE StrToBarplace(std::string value) const;

    std::string BarrenditionToStr(data_BARRENDITION data) const;
    data_BARRENDITION StrToBarrendition(std::string value) const;

    std::string BeamplaceToStr(data_BEAMPLACE data) const;
    data_BEAMPLACE StrToBeamplace(std::string value) const;

    std::string BetypeToStr(data_BETYPE data) const;
    data_BETYPE StrToBetype(std::string value) const;

    std::string BooleanToStr(data_BOOLEAN data) const;
    data_BOOLEAN StrToBoolean(std::string value) const;

    std::string CertaintyToStr(data_CERTAINTY data) const;
    data_CERTAINTY StrToCertainty(std::string value) const;

    std::string ClefshapeToStr(data_CLEFSHAPE data) const;
    data_CLEFSHAPE StrToClefshape(std::string value) const;

    std::string ColornamesToStr(data_COLORNAMES data) const;
    data_COLORNAMES StrToColornames(std::string value) const;

    std::string EnclosureToStr(data_ENCLOSURE data) const;
    data_ENCLOSURE StrToEnclosure(std::string value) const;

    std::string FillToStr(data_FILL data) const;
    data_FILL StrToFill(std::string value) const;

    std::string FontsizetermToStr(data_FONTSIZETERM data) const;
    data_FONTSIZETERM StrToFontsizeterm(std::string value) const;

    std::string FontstyleToStr(data_FONTSTYLE data) const;
    data_FONTSTYLE StrToFontstyle(std::string value) const;

    std::string FontweightToStr(data_FONTWEIGHT data) const;
    data_FONTWEIGHT StrToFontweight(std::string value) const;

    std::string GlissandoToStr(data_GLISSANDO data) const;
    data_GLISSANDO StrToGlissando(std::string value) const;

    std::string GraceToStr(data_GRACE data) const;
    data_GRACE StrToGrace(std::string value) const;

    std::string HeadshapeListToStr(data_HEADSHAPE_list data) const;
    data_HEADSHAPE_list StrToHeadshapeList(std::string value) const;

    std::string HorizontalalignmentToStr(data_HORIZONTALALIGNMENT data) const;
    data_HORIZONTALALIGNMENT StrToHorizontalalignment(std::string value) const;

    std::string LigatureformToStr(data_LIGATUREFORM data) const;
    data_LIGATUREFORM StrToLigatureform(std::string value) const;

    std::string LineformToStr(data_LINEFORM data) const;
    data_LINEFORM StrToLineform(std::string value) const;

    std::string LinestartendsymbolToStr(data_LINESTARTENDSYMBOL data) const;
    data_LINESTARTENDSYMBOL StrToLinestartendsymbol(std::string value) const;

    std::string LinewidthtermToStr(data_LINEWIDTHTERM data) const;
    data_LINEWIDTHTERM StrToLinewidthterm(std::string value) const;

    std::string MensurationsignToStr(data_MENSURATIONSIGN data) const;
    data_MENSURATIONSIGN StrToMensurationsign(std::string value) const;

    std::string MetersignToStr(data_METERSIGN data) const;
    data_METERSIGN StrToMetersign(std::string value) const;

    std::string MidinamesToStr(data_MIDINAMES data) const;
    data_MIDINAMES StrToMidinames(std::string value) const;

    std::string ModeToStr(data_MODE data) const;
    data_MODE StrToMode(std::string value) const;

    std::string ModsrelationshipToStr(data_MODSRELATIONSHIP data) const;
    data_MODSRELATIONSHIP StrToModsrelationship(std::string value) const;

    std::string NonstaffplaceToStr(data_NONSTAFFPLACE data) const;
    data_NONSTAFFPLACE StrToNonstaffplace(std::string value) const;

    std::string NotationtypeToStr(data_NOTATIONTYPE data) const;
    data_NOTATIONTYPE StrToNotationtype(std::string value) const;

    std::string NoteheadmodifierListToStr(data_NOTEHEADMODIFIER_list data) const;
    data_NOTEHEADMODIFIER_list StrToNoteheadmodifierList(std::string value) const;

    std::string OtherstaffToStr(data_OTHERSTAFF data) const;
    data_OTHERSTAFF StrToOtherstaff(std::string value) const;

    std::string RotationdirectionToStr(data_ROTATIONDIRECTION data) const;
    data_ROTATIONDIRECTION StrToRotationdirection(std::string value) const;

    std::string StaffitemBasicToStr(data_STAFFITEM_basic data) const;
    data_STAFFITEM_basic StrToStaffitemBasic(std::string value) const;

    std::string StaffitemCmnToStr(data_STAFFITEM_cmn data) const;
    data_STAFFITEM_cmn StrToStaffitemCmn(std::string value) const;

    std::string StaffitemMensuralToStr(data_STAFFITEM_mensural data) const;
    data_STAFFITEM_mensural StrToStaffitemMensural(std::string value) const;

    std::string StaffrelBasicToStr(data_STAFFREL_basic data) const;
    data_STAFFREL_basic StrToStaffrelBasic(std::string value) const;

    std::string StaffrelExtendedToStr(data_STAFFREL_extended data) const;
    data_STAFFREL_extended StrToStaffrelExtended(std::string value) const;

    std::string StemmodifierToStr(data_STEMMODIFIER data) const;
    data_STEMMODIFIER StrToStemmodifier(std::string value) const;

    std::string StempositionToStr(data_STEMPOSITION data) const;
    data_STEMPOSITION StrToStemposition(std::string value) const;

    std::string TextrenditionlistToStr(data_TEXTRENDITIONLIST data) const;
    data_TEXTRENDITIONLIST StrToTextrenditionlist(std::string value) const;

    std::string AccidLogFuncToStr(accidLog_FUNC data) const;
    accidLog_FUNC StrToAccidLogFunc(std::string value) const;

    std::string ArpegLogOrderToStr(arpegLog_ORDER data) const;
    arpegLog_ORDER StrToArpegLogOrder(std::string value) const;

    std::string AudienceAudienceToStr(audience_AUDIENCE data) const;
    audience_AUDIENCE StrToAudienceAudience(std::string value) const;

    std::string BTremLogFormToStr(bTremLog_FORM data) const;
    bTremLog_FORM StrToBTremLogForm(std::string value) const;

    std::string CurvatureCurvedirToStr(curvature_CURVEDIR data) const;
    curvature_CURVEDIR StrToCurvatureCurvedir(std::string value) const;

    std::string DotLogFormToStr(dotLog_FORM data) const;
    dotLog_FORM StrToDotLogForm(std::string value) const;

    std::string FTremLogFormToStr(fTremLog_FORM data) const;
    fTremLog_FORM StrToFTremLogForm(std::string value) const;

    std::string GraceGrpLogAttachToStr(graceGrpLog_ATTACH data) const;
    graceGrpLog_ATTACH StrToGraceGrpLogAttach(std::string value) const;

    std::string HairpinLogFormToStr(hairpinLog_FORM data) const;
    hairpinLog_FORM StrToHairpinLogForm(std::string value) const;

    std::string HarpPedalLogAToStr(harpPedalLog_A data) const;
    harpPedalLog_A StrToHarpPedalLogA(std::string value) const;

    std::string HarpPedalLogBToStr(harpPedalLog_B data) const;
    harpPedalLog_B StrToHarpPedalLogB(std::string value) const;

    std::string HarpPedalLogCToStr(harpPedalLog_C data) const;
    harpPedalLog_C StrToHarpPedalLogC(std::string value) const;

    std::string HarpPedalLogDToStr(harpPedalLog_D data) const;
    harpPedalLog_D StrToHarpPedalLogD(std::string value) const;

    std::string HarpPedalLogEToStr(harpPedalLog_E data) const;
    harpPedalLog_E StrToHarpPedalLogE(std::string value) const;

    std::string HarpPedalLogFToStr(harpPedalLog_F data) const;
    harpPedalLog_F StrToHarpPedalLogF(std::string value) const;

    std::string HarpPedalLogGToStr(harpPedalLog_G data) const;
    harpPedalLog_G StrToHarpPedalLogG(std::string value) const;

    std::string MeiVersionMeiversionToStr(meiVersion_MEIVERSION data) const;
    meiVersion_MEIVERSION StrToMeiVersionMeiversion(std::string value) const;

    std::string MeterConformanceMetconToStr(meterConformance_METCON data) const;
    meterConformance_METCON StrToMeterConformanceMetcon(std::string value) const;

    std::string MeterSigGrpLogFuncToStr(meterSigGrpLog_FUNC data) const;
    meterSigGrpLog_FUNC StrToMeterSigGrpLogFunc(std::string value) const;

    std::string MordentLogFormToStr(mordentLog_FORM data) const;
    mordentLog_FORM StrToMordentLogForm(std::string value) const;

    std::string OctaveLogCollToStr(octaveLog_COLL data) const;
    octaveLog_COLL StrToOctaveLogColl(std::string value) const;

    std::string PedalLogDirToStr(pedalLog_DIR data) const;
    pedalLog_DIR StrToPedalLogDir(std::string value) const;

    std::string PointingXlinkactuateToStr(pointing_XLINKACTUATE data) const;
    pointing_XLINKACTUATE StrToPointingXlinkactuate(std::string value) const;

    std::string PointingXlinkshowToStr(pointing_XLINKSHOW data) const;
    pointing_XLINKSHOW StrToPointingXlinkshow(std::string value) const;

    std::string RegularMethodMethodToStr(regularMethod_METHOD data) const;
    regularMethod_METHOD StrToRegularMethodMethod(std::string value) const;

    std::string StaffGroupingSymSymbolToStr(staffGroupingSym_SYMBOL data) const;
    staffGroupingSym_SYMBOL StrToStaffGroupingSymSymbol(std::string value) const;

    std::string SylLogConToStr(sylLog_CON data) const;
    sylLog_CON StrToSylLogCon(std::string value) const;

    std::string SylLogWordposToStr(sylLog_WORDPOS data) const;
    sylLog_WORDPOS StrToSylLogWordpos(std::string value) const;

    std::string TargetEvalEvaluateToStr(targetEval_EVALUATE data) const;
    targetEval_EVALUATE StrToTargetEvalEvaluate(std::string value) const;

    std::string TempoLogFuncToStr(tempoLog_FUNC data) const;
    tempoLog_FUNC StrToTempoLogFunc(std::string value) const;

    std::string TurnLogFormToStr(turnLog_FORM data) const;
    turnLog_FORM StrToTurnLogForm(std::string value) const;

    std::string WhitespaceXmlspaceToStr(whitespace_XMLSPACE data) const;
    whitespace_XMLSPACE StrToWhitespaceXmlspace(std::string value) const;
};

} // vrv namespace

#endif // __VRV_ATT_CONVERTER_H__
