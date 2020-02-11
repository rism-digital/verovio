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
    std::string AccidentalGesturalToStr(data_ACCIDENTAL_GESTURAL data) const;
    data_ACCIDENTAL_GESTURAL StrToAccidentalGestural(std::string value, bool logWarning = true) const;

    std::string AccidentalWrittenToStr(data_ACCIDENTAL_WRITTEN data) const;
    data_ACCIDENTAL_WRITTEN StrToAccidentalWritten(std::string value, bool logWarning = true) const;

    std::string ArticulationToStr(data_ARTICULATION data) const;
    data_ARTICULATION StrToArticulation(std::string value, bool logWarning = true) const;

    std::string BarmethodToStr(data_BARMETHOD data) const;
    data_BARMETHOD StrToBarmethod(std::string value, bool logWarning = true) const;

    std::string BarrenditionToStr(data_BARRENDITION data) const;
    data_BARRENDITION StrToBarrendition(std::string value, bool logWarning = true) const;

    std::string BeamplaceToStr(data_BEAMPLACE data) const;
    data_BEAMPLACE StrToBeamplace(std::string value, bool logWarning = true) const;

    std::string BetypeToStr(data_BETYPE data) const;
    data_BETYPE StrToBetype(std::string value, bool logWarning = true) const;

    std::string BooleanToStr(data_BOOLEAN data) const;
    data_BOOLEAN StrToBoolean(std::string value, bool logWarning = true) const;

    std::string CertaintyToStr(data_CERTAINTY data) const;
    data_CERTAINTY StrToCertainty(std::string value, bool logWarning = true) const;

    std::string ClefshapeToStr(data_CLEFSHAPE data) const;
    data_CLEFSHAPE StrToClefshape(std::string value, bool logWarning = true) const;

    std::string ClusterToStr(data_CLUSTER data) const;
    data_CLUSTER StrToCluster(std::string value, bool logWarning = true) const;

    std::string ColornamesToStr(data_COLORNAMES data) const;
    data_COLORNAMES StrToColornames(std::string value, bool logWarning = true) const;

    std::string CompassdirectionToStr(data_COMPASSDIRECTION data) const;
    data_COMPASSDIRECTION StrToCompassdirection(std::string value, bool logWarning = true) const;

    std::string CompassdirectionBasicToStr(data_COMPASSDIRECTION_basic data) const;
    data_COMPASSDIRECTION_basic StrToCompassdirectionBasic(std::string value, bool logWarning = true) const;

    std::string CompassdirectionExtendedToStr(data_COMPASSDIRECTION_extended data) const;
    data_COMPASSDIRECTION_extended StrToCompassdirectionExtended(std::string value, bool logWarning = true) const;

    std::string EnclosureToStr(data_ENCLOSURE data) const;
    data_ENCLOSURE StrToEnclosure(std::string value, bool logWarning = true) const;

    std::string EventrelToStr(data_EVENTREL data) const;
    data_EVENTREL StrToEventrel(std::string value, bool logWarning = true) const;

    std::string EventrelBasicToStr(data_EVENTREL_basic data) const;
    data_EVENTREL_basic StrToEventrelBasic(std::string value, bool logWarning = true) const;

    std::string EventrelExtendedToStr(data_EVENTREL_extended data) const;
    data_EVENTREL_extended StrToEventrelExtended(std::string value, bool logWarning = true) const;

    std::string FillToStr(data_FILL data) const;
    data_FILL StrToFill(std::string value, bool logWarning = true) const;

    std::string FontsizetermToStr(data_FONTSIZETERM data) const;
    data_FONTSIZETERM StrToFontsizeterm(std::string value, bool logWarning = true) const;

    std::string FontstyleToStr(data_FONTSTYLE data) const;
    data_FONTSTYLE StrToFontstyle(std::string value, bool logWarning = true) const;

    std::string FontweightToStr(data_FONTWEIGHT data) const;
    data_FONTWEIGHT StrToFontweight(std::string value, bool logWarning = true) const;

    std::string FrbrrelationshipToStr(data_FRBRRELATIONSHIP data) const;
    data_FRBRRELATIONSHIP StrToFrbrrelationship(std::string value, bool logWarning = true) const;

    std::string GlissandoToStr(data_GLISSANDO data) const;
    data_GLISSANDO StrToGlissando(std::string value, bool logWarning = true) const;

    std::string GraceToStr(data_GRACE data) const;
    data_GRACE StrToGrace(std::string value, bool logWarning = true) const;

    std::string HeadshapeToStr(data_HEADSHAPE data) const;
    data_HEADSHAPE StrToHeadshape(std::string value, bool logWarning = true) const;

    std::string HeadshapeListToStr(data_HEADSHAPE_list data) const;
    data_HEADSHAPE_list StrToHeadshapeList(std::string value, bool logWarning = true) const;

    std::string HorizontalalignmentToStr(data_HORIZONTALALIGNMENT data) const;
    data_HORIZONTALALIGNMENT StrToHorizontalalignment(std::string value, bool logWarning = true) const;

    std::string LayerschemeToStr(data_LAYERSCHEME data) const;
    data_LAYERSCHEME StrToLayerscheme(std::string value, bool logWarning = true) const;

    std::string LigatureformToStr(data_LIGATUREFORM data) const;
    data_LIGATUREFORM StrToLigatureform(std::string value, bool logWarning = true) const;

    std::string LineformToStr(data_LINEFORM data) const;
    data_LINEFORM StrToLineform(std::string value, bool logWarning = true) const;

    std::string LinestartendsymbolToStr(data_LINESTARTENDSYMBOL data) const;
    data_LINESTARTENDSYMBOL StrToLinestartendsymbol(std::string value, bool logWarning = true) const;

    std::string LinewidthtermToStr(data_LINEWIDTHTERM data) const;
    data_LINEWIDTHTERM StrToLinewidthterm(std::string value, bool logWarning = true) const;

    std::string MelodicfunctionToStr(data_MELODICFUNCTION data) const;
    data_MELODICFUNCTION StrToMelodicfunction(std::string value, bool logWarning = true) const;

    std::string MensurationsignToStr(data_MENSURATIONSIGN data) const;
    data_MENSURATIONSIGN StrToMensurationsign(std::string value, bool logWarning = true) const;

    std::string MeterformToStr(data_METERFORM data) const;
    data_METERFORM StrToMeterform(std::string value, bool logWarning = true) const;

    std::string MetersignToStr(data_METERSIGN data) const;
    data_METERSIGN StrToMetersign(std::string value, bool logWarning = true) const;

    std::string MidinamesToStr(data_MIDINAMES data) const;
    data_MIDINAMES StrToMidinames(std::string value, bool logWarning = true) const;

    std::string ModeToStr(data_MODE data) const;
    data_MODE StrToMode(std::string value, bool logWarning = true) const;

    std::string ModsrelationshipToStr(data_MODSRELATIONSHIP data) const;
    data_MODSRELATIONSHIP StrToModsrelationship(std::string value, bool logWarning = true) const;

    std::string NonstaffplaceToStr(data_NONSTAFFPLACE data) const;
    data_NONSTAFFPLACE StrToNonstaffplace(std::string value, bool logWarning = true) const;

    std::string NotationtypeToStr(data_NOTATIONTYPE data) const;
    data_NOTATIONTYPE StrToNotationtype(std::string value, bool logWarning = true) const;

    std::string NoteheadmodifierToStr(data_NOTEHEADMODIFIER data) const;
    data_NOTEHEADMODIFIER StrToNoteheadmodifier(std::string value, bool logWarning = true) const;

    std::string NoteheadmodifierListToStr(data_NOTEHEADMODIFIER_list data) const;
    data_NOTEHEADMODIFIER_list StrToNoteheadmodifierList(std::string value, bool logWarning = true) const;

    std::string OtherstaffToStr(data_OTHERSTAFF data) const;
    data_OTHERSTAFF StrToOtherstaff(std::string value, bool logWarning = true) const;

    std::string RelationshipToStr(data_RELATIONSHIP data) const;
    data_RELATIONSHIP StrToRelationship(std::string value, bool logWarning = true) const;

    std::string RotationToStr(data_ROTATION data) const;
    data_ROTATION StrToRotation(std::string value, bool logWarning = true) const;

    std::string RotationdirectionToStr(data_ROTATIONDIRECTION data) const;
    data_ROTATIONDIRECTION StrToRotationdirection(std::string value, bool logWarning = true) const;

    std::string StaffitemToStr(data_STAFFITEM data) const;
    data_STAFFITEM StrToStaffitem(std::string value, bool logWarning = true) const;

    std::string StaffitemBasicToStr(data_STAFFITEM_basic data) const;
    data_STAFFITEM_basic StrToStaffitemBasic(std::string value, bool logWarning = true) const;

    std::string StaffitemCmnToStr(data_STAFFITEM_cmn data) const;
    data_STAFFITEM_cmn StrToStaffitemCmn(std::string value, bool logWarning = true) const;

    std::string StaffitemMensuralToStr(data_STAFFITEM_mensural data) const;
    data_STAFFITEM_mensural StrToStaffitemMensural(std::string value, bool logWarning = true) const;

    std::string StaffrelToStr(data_STAFFREL data) const;
    data_STAFFREL StrToStaffrel(std::string value, bool logWarning = true) const;

    std::string StaffrelBasicToStr(data_STAFFREL_basic data) const;
    data_STAFFREL_basic StrToStaffrelBasic(std::string value, bool logWarning = true) const;

    std::string StaffrelExtendedToStr(data_STAFFREL_extended data) const;
    data_STAFFREL_extended StrToStaffrelExtended(std::string value, bool logWarning = true) const;

    std::string StemdirectionToStr(data_STEMDIRECTION data) const;
    data_STEMDIRECTION StrToStemdirection(std::string value, bool logWarning = true) const;

    std::string StemdirectionBasicToStr(data_STEMDIRECTION_basic data) const;
    data_STEMDIRECTION_basic StrToStemdirectionBasic(std::string value, bool logWarning = true) const;

    std::string StemdirectionExtendedToStr(data_STEMDIRECTION_extended data) const;
    data_STEMDIRECTION_extended StrToStemdirectionExtended(std::string value, bool logWarning = true) const;

    std::string StemmodifierToStr(data_STEMMODIFIER data) const;
    data_STEMMODIFIER StrToStemmodifier(std::string value, bool logWarning = true) const;

    std::string StempositionToStr(data_STEMPOSITION data) const;
    data_STEMPOSITION StrToStemposition(std::string value, bool logWarning = true) const;

    std::string TemperamentToStr(data_TEMPERAMENT data) const;
    data_TEMPERAMENT StrToTemperament(std::string value, bool logWarning = true) const;

    std::string TextrenditionToStr(data_TEXTRENDITION data) const;
    data_TEXTRENDITION StrToTextrendition(std::string value, bool logWarning = true) const;

    std::string TextrenditionlistToStr(data_TEXTRENDITIONLIST data) const;
    data_TEXTRENDITIONLIST StrToTextrenditionlist(std::string value, bool logWarning = true) const;

    std::string VerticalalignmentToStr(data_VERTICALALIGNMENT data) const;
    data_VERTICALALIGNMENT StrToVerticalalignment(std::string value, bool logWarning = true) const;

    std::string AccidLogFuncToStr(accidLog_FUNC data) const;
    accidLog_FUNC StrToAccidLogFunc(std::string value, bool logWarning = true) const;

    std::string AnchoredTextLogFuncToStr(anchoredTextLog_FUNC data) const;
    anchoredTextLog_FUNC StrToAnchoredTextLogFunc(std::string value, bool logWarning = true) const;

    std::string ArpegLogOrderToStr(arpegLog_ORDER data) const;
    arpegLog_ORDER StrToArpegLogOrder(std::string value, bool logWarning = true) const;

    std::string AudienceAudienceToStr(audience_AUDIENCE data) const;
    audience_AUDIENCE StrToAudienceAudience(std::string value, bool logWarning = true) const;

    std::string BTremLogFormToStr(bTremLog_FORM data) const;
    bTremLog_FORM StrToBTremLogForm(std::string value, bool logWarning = true) const;

    std::string BeamRendFormToStr(beamRend_FORM data) const;
    beamRend_FORM StrToBeamRendForm(std::string value, bool logWarning = true) const;

    std::string BeamingVisBeamrendToStr(beamingVis_BEAMREND data) const;
    beamingVis_BEAMREND StrToBeamingVisBeamrend(std::string value, bool logWarning = true) const;

    std::string BracketSpanLogFuncToStr(bracketSpanLog_FUNC data) const;
    bracketSpanLog_FUNC StrToBracketSpanLogFunc(std::string value, bool logWarning = true) const;

    std::string CurvatureCurvedirToStr(curvature_CURVEDIR data) const;
    curvature_CURVEDIR StrToCurvatureCurvedir(std::string value, bool logWarning = true) const;

    std::string CurveLogFuncToStr(curveLog_FUNC data) const;
    curveLog_FUNC StrToCurveLogFunc(std::string value, bool logWarning = true) const;

    std::string CutoutCutoutToStr(cutout_CUTOUT data) const;
    cutout_CUTOUT StrToCutoutCutout(std::string value, bool logWarning = true) const;

    std::string DotLogFormToStr(dotLog_FORM data) const;
    dotLog_FORM StrToDotLogForm(std::string value, bool logWarning = true) const;

    std::string EndingsEndingrendToStr(endings_ENDINGREND data) const;
    endings_ENDINGREND StrToEndingsEndingrend(std::string value, bool logWarning = true) const;

    std::string EpisemaVisFormToStr(episemaVis_FORM data) const;
    episemaVis_FORM StrToEpisemaVisForm(std::string value, bool logWarning = true) const;

    std::string EvidenceEvidenceToStr(evidence_EVIDENCE data) const;
    evidence_EVIDENCE StrToEvidenceEvidence(std::string value, bool logWarning = true) const;

    std::string ExtSymGlyphauthToStr(extSym_GLYPHAUTH data) const;
    extSym_GLYPHAUTH StrToExtSymGlyphauth(std::string value, bool logWarning = true) const;

    std::string FTremLogFormToStr(fTremLog_FORM data) const;
    fTremLog_FORM StrToFTremLogForm(std::string value, bool logWarning = true) const;

    std::string FermataVisFormToStr(fermataVis_FORM data) const;
    fermataVis_FORM StrToFermataVisForm(std::string value, bool logWarning = true) const;

    std::string FermataVisShapeToStr(fermataVis_SHAPE data) const;
    fermataVis_SHAPE StrToFermataVisShape(std::string value, bool logWarning = true) const;

    std::string FingGrpLogFormToStr(fingGrpLog_FORM data) const;
    fingGrpLog_FORM StrToFingGrpLogForm(std::string value, bool logWarning = true) const;

    std::string FingGrpVisOrientToStr(fingGrpVis_ORIENT data) const;
    fingGrpVis_ORIENT StrToFingGrpVisOrient(std::string value, bool logWarning = true) const;

    std::string GraceGrpLogAttachToStr(graceGrpLog_ATTACH data) const;
    graceGrpLog_ATTACH StrToGraceGrpLogAttach(std::string value, bool logWarning = true) const;

    std::string HairpinLogFormToStr(hairpinLog_FORM data) const;
    hairpinLog_FORM StrToHairpinLogForm(std::string value, bool logWarning = true) const;

    std::string HarmAnlFormToStr(harmAnl_FORM data) const;
    harmAnl_FORM StrToHarmAnlForm(std::string value, bool logWarning = true) const;

    std::string HarmVisRendgridToStr(harmVis_RENDGRID data) const;
    harmVis_RENDGRID StrToHarmVisRendgrid(std::string value, bool logWarning = true) const;

    std::string HarpPedalLogAToStr(harpPedalLog_A data) const;
    harpPedalLog_A StrToHarpPedalLogA(std::string value, bool logWarning = true) const;

    std::string HarpPedalLogBToStr(harpPedalLog_B data) const;
    harpPedalLog_B StrToHarpPedalLogB(std::string value, bool logWarning = true) const;

    std::string HarpPedalLogCToStr(harpPedalLog_C data) const;
    harpPedalLog_C StrToHarpPedalLogC(std::string value, bool logWarning = true) const;

    std::string HarpPedalLogDToStr(harpPedalLog_D data) const;
    harpPedalLog_D StrToHarpPedalLogD(std::string value, bool logWarning = true) const;

    std::string HarpPedalLogEToStr(harpPedalLog_E data) const;
    harpPedalLog_E StrToHarpPedalLogE(std::string value, bool logWarning = true) const;

    std::string HarpPedalLogFToStr(harpPedalLog_F data) const;
    harpPedalLog_F StrToHarpPedalLogF(std::string value, bool logWarning = true) const;

    std::string HarpPedalLogGToStr(harpPedalLog_G data) const;
    harpPedalLog_G StrToHarpPedalLogG(std::string value, bool logWarning = true) const;

    std::string LineLogFuncToStr(lineLog_FUNC data) const;
    lineLog_FUNC StrToLineLogFunc(std::string value, bool logWarning = true) const;

    std::string LiquescentVisCurveToStr(liquescentVis_CURVE data) const;
    liquescentVis_CURVE StrToLiquescentVisCurve(std::string value, bool logWarning = true) const;

    std::string MeasurementUnitToStr(measurement_UNIT data) const;
    measurement_UNIT StrToMeasurementUnit(std::string value, bool logWarning = true) const;

    std::string MeiVersionMeiversionToStr(meiVersion_MEIVERSION data) const;
    meiVersion_MEIVERSION StrToMeiVersionMeiversion(std::string value, bool logWarning = true) const;

    std::string MensurVisFormToStr(mensurVis_FORM data) const;
    mensurVis_FORM StrToMensurVisForm(std::string value, bool logWarning = true) const;

    std::string MensuralVisMensurformToStr(mensuralVis_MENSURFORM data) const;
    mensuralVis_MENSURFORM StrToMensuralVisMensurform(std::string value, bool logWarning = true) const;

    std::string MeterConformanceMetconToStr(meterConformance_METCON data) const;
    meterConformance_METCON StrToMeterConformanceMetcon(std::string value, bool logWarning = true) const;

    std::string MeterSigGrpLogFuncToStr(meterSigGrpLog_FUNC data) const;
    meterSigGrpLog_FUNC StrToMeterSigGrpLogFunc(std::string value, bool logWarning = true) const;

    std::string MordentLogFormToStr(mordentLog_FORM data) const;
    mordentLog_FORM StrToMordentLogForm(std::string value, bool logWarning = true) const;

    std::string NcFormConToStr(ncForm_CON data) const;
    ncForm_CON StrToNcFormCon(std::string value, bool logWarning = true) const;

    std::string NcFormCurveToStr(ncForm_CURVE data) const;
    ncForm_CURVE StrToNcFormCurve(std::string value, bool logWarning = true) const;

    std::string NcFormRellenToStr(ncForm_RELLEN data) const;
    ncForm_RELLEN StrToNcFormRellen(std::string value, bool logWarning = true) const;

    std::string NoteAnlMensuralLigToStr(noteAnlMensural_LIG data) const;
    noteAnlMensural_LIG StrToNoteAnlMensuralLig(std::string value, bool logWarning = true) const;

    std::string NoteGesExtremisToStr(noteGes_EXTREMIS data) const;
    noteGes_EXTREMIS StrToNoteGesExtremis(std::string value, bool logWarning = true) const;

    std::string NoteHeadsHeadauthToStr(noteHeads_HEADAUTH data) const;
    noteHeads_HEADAUTH StrToNoteHeadsHeadauth(std::string value, bool logWarning = true) const;

    std::string OctaveLogCollToStr(octaveLog_COLL data) const;
    octaveLog_COLL StrToOctaveLogColl(std::string value, bool logWarning = true) const;

    std::string PbVisFoliumToStr(pbVis_FOLIUM data) const;
    pbVis_FOLIUM StrToPbVisFolium(std::string value, bool logWarning = true) const;

    std::string PedalLogDirToStr(pedalLog_DIR data) const;
    pedalLog_DIR StrToPedalLogDir(std::string value, bool logWarning = true) const;

    std::string PedalLogFuncToStr(pedalLog_FUNC data) const;
    pedalLog_FUNC StrToPedalLogFunc(std::string value, bool logWarning = true) const;

    std::string PedalVisFormToStr(pedalVis_FORM data) const;
    pedalVis_FORM StrToPedalVisForm(std::string value, bool logWarning = true) const;

    std::string PianoPedalsPedalstyleToStr(pianoPedals_PEDALSTYLE data) const;
    pianoPedals_PEDALSTYLE StrToPianoPedalsPedalstyle(std::string value, bool logWarning = true) const;

    std::string PointingXlinkactuateToStr(pointing_XLINKACTUATE data) const;
    pointing_XLINKACTUATE StrToPointingXlinkactuate(std::string value, bool logWarning = true) const;

    std::string PointingXlinkshowToStr(pointing_XLINKSHOW data) const;
    pointing_XLINKSHOW StrToPointingXlinkshow(std::string value, bool logWarning = true) const;

    std::string RecordTypeRecordtypeToStr(recordType_RECORDTYPE data) const;
    recordType_RECORDTYPE StrToRecordTypeRecordtype(std::string value, bool logWarning = true) const;

    std::string RegularMethodMethodToStr(regularMethod_METHOD data) const;
    regularMethod_METHOD StrToRegularMethodMethod(std::string value, bool logWarning = true) const;

    std::string RehearsalRehencloseToStr(rehearsal_REHENCLOSE data) const;
    rehearsal_REHENCLOSE StrToRehearsalRehenclose(std::string value, bool logWarning = true) const;

    std::string SbVisFormToStr(sbVis_FORM data) const;
    sbVis_FORM StrToSbVisForm(std::string value, bool logWarning = true) const;

    std::string StaffGroupingSymSymbolToStr(staffGroupingSym_SYMBOL data) const;
    staffGroupingSym_SYMBOL StrToStaffGroupingSymSymbol(std::string value, bool logWarning = true) const;

    std::string SylLogConToStr(sylLog_CON data) const;
    sylLog_CON StrToSylLogCon(std::string value, bool logWarning = true) const;

    std::string SylLogWordposToStr(sylLog_WORDPOS data) const;
    sylLog_WORDPOS StrToSylLogWordpos(std::string value, bool logWarning = true) const;

    std::string TargetEvalEvaluateToStr(targetEval_EVALUATE data) const;
    targetEval_EVALUATE StrToTargetEvalEvaluate(std::string value, bool logWarning = true) const;

    std::string TempoLogFuncToStr(tempoLog_FUNC data) const;
    tempoLog_FUNC StrToTempoLogFunc(std::string value, bool logWarning = true) const;

    std::string TupletVisNumformatToStr(tupletVis_NUMFORMAT data) const;
    tupletVis_NUMFORMAT StrToTupletVisNumformat(std::string value, bool logWarning = true) const;

    std::string TurnLogFormToStr(turnLog_FORM data) const;
    turnLog_FORM StrToTurnLogForm(std::string value, bool logWarning = true) const;

    std::string VoltaGroupingSymVoltasymToStr(voltaGroupingSym_VOLTASYM data) const;
    voltaGroupingSym_VOLTASYM StrToVoltaGroupingSymVoltasym(std::string value, bool logWarning = true) const;

    std::string WhitespaceXmlspaceToStr(whitespace_XMLSPACE data) const;
    whitespace_XMLSPACE StrToWhitespaceXmlspace(std::string value, bool logWarning = true) const;
};

} // vrv namespace

#endif // __VRV_ATT_CONVERTER_H__
