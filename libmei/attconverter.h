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
    data_ACCIDENTAL_GESTURAL StrToAccidentalGestural(const std::string &value, bool logWarning = true) const;

    std::string AccidentalWrittenToStr(data_ACCIDENTAL_WRITTEN data) const;
    data_ACCIDENTAL_WRITTEN StrToAccidentalWritten(const std::string &value, bool logWarning = true) const;

    std::string AccidentalWrittenAeuToStr(data_ACCIDENTAL_WRITTEN_aeu data) const;
    data_ACCIDENTAL_WRITTEN_aeu StrToAccidentalWrittenAeu(const std::string &value, bool logWarning = true) const;

    std::string AccidentalWrittenBasicToStr(data_ACCIDENTAL_WRITTEN_basic data) const;
    data_ACCIDENTAL_WRITTEN_basic StrToAccidentalWrittenBasic(const std::string &value, bool logWarning = true) const;

    std::string AccidentalWrittenExtendedToStr(data_ACCIDENTAL_WRITTEN_extended data) const;
    data_ACCIDENTAL_WRITTEN_extended StrToAccidentalWrittenExtended(const std::string &value, bool logWarning = true) const;

    std::string ArticulationToStr(data_ARTICULATION data) const;
    data_ARTICULATION StrToArticulation(const std::string &value, bool logWarning = true) const;

    std::string BarmethodToStr(data_BARMETHOD data) const;
    data_BARMETHOD StrToBarmethod(const std::string &value, bool logWarning = true) const;

    std::string BarrenditionToStr(data_BARRENDITION data) const;
    data_BARRENDITION StrToBarrendition(const std::string &value, bool logWarning = true) const;

    std::string BeamplaceToStr(data_BEAMPLACE data) const;
    data_BEAMPLACE StrToBeamplace(const std::string &value, bool logWarning = true) const;

    std::string BetypeToStr(data_BETYPE data) const;
    data_BETYPE StrToBetype(const std::string &value, bool logWarning = true) const;

    std::string BooleanToStr(data_BOOLEAN data) const;
    data_BOOLEAN StrToBoolean(const std::string &value, bool logWarning = true) const;

    std::string CertaintyToStr(data_CERTAINTY data) const;
    data_CERTAINTY StrToCertainty(const std::string &value, bool logWarning = true) const;

    std::string ClefshapeToStr(data_CLEFSHAPE data) const;
    data_CLEFSHAPE StrToClefshape(const std::string &value, bool logWarning = true) const;

    std::string ClusterToStr(data_CLUSTER data) const;
    data_CLUSTER StrToCluster(const std::string &value, bool logWarning = true) const;

    std::string ColornamesToStr(data_COLORNAMES data) const;
    data_COLORNAMES StrToColornames(const std::string &value, bool logWarning = true) const;

    std::string CompassdirectionToStr(data_COMPASSDIRECTION data) const;
    data_COMPASSDIRECTION StrToCompassdirection(const std::string &value, bool logWarning = true) const;

    std::string CompassdirectionBasicToStr(data_COMPASSDIRECTION_basic data) const;
    data_COMPASSDIRECTION_basic StrToCompassdirectionBasic(const std::string &value, bool logWarning = true) const;

    std::string CompassdirectionExtendedToStr(data_COMPASSDIRECTION_extended data) const;
    data_COMPASSDIRECTION_extended StrToCompassdirectionExtended(const std::string &value, bool logWarning = true) const;

    std::string DivisioToStr(data_DIVISIO data) const;
    data_DIVISIO StrToDivisio(const std::string &value, bool logWarning = true) const;

    std::string DurationrestsToStr(data_DURATIONRESTS data) const;
    data_DURATIONRESTS StrToDurationrests(const std::string &value, bool logWarning = true) const;

    std::string DurationrestsMensuralToStr(data_DURATIONRESTS_mensural data) const;
    data_DURATIONRESTS_mensural StrToDurationrestsMensural(const std::string &value, bool logWarning = true) const;

    std::string DurqualityMensuralToStr(data_DURQUALITY_mensural data) const;
    data_DURQUALITY_mensural StrToDurqualityMensural(const std::string &value, bool logWarning = true) const;

    std::string EnclosureToStr(data_ENCLOSURE data) const;
    data_ENCLOSURE StrToEnclosure(const std::string &value, bool logWarning = true) const;

    std::string EventrelToStr(data_EVENTREL data) const;
    data_EVENTREL StrToEventrel(const std::string &value, bool logWarning = true) const;

    std::string EventrelBasicToStr(data_EVENTREL_basic data) const;
    data_EVENTREL_basic StrToEventrelBasic(const std::string &value, bool logWarning = true) const;

    std::string EventrelExtendedToStr(data_EVENTREL_extended data) const;
    data_EVENTREL_extended StrToEventrelExtended(const std::string &value, bool logWarning = true) const;

    std::string FillToStr(data_FILL data) const;
    data_FILL StrToFill(const std::string &value, bool logWarning = true) const;

    std::string FlagformMensuralToStr(data_FLAGFORM_mensural data) const;
    data_FLAGFORM_mensural StrToFlagformMensural(const std::string &value, bool logWarning = true) const;

    std::string FlagposMensuralToStr(data_FLAGPOS_mensural data) const;
    data_FLAGPOS_mensural StrToFlagposMensural(const std::string &value, bool logWarning = true) const;

    std::string FontsizetermToStr(data_FONTSIZETERM data) const;
    data_FONTSIZETERM StrToFontsizeterm(const std::string &value, bool logWarning = true) const;

    std::string FontstyleToStr(data_FONTSTYLE data) const;
    data_FONTSTYLE StrToFontstyle(const std::string &value, bool logWarning = true) const;

    std::string FontweightToStr(data_FONTWEIGHT data) const;
    data_FONTWEIGHT StrToFontweight(const std::string &value, bool logWarning = true) const;

    std::string FrbrrelationshipToStr(data_FRBRRELATIONSHIP data) const;
    data_FRBRRELATIONSHIP StrToFrbrrelationship(const std::string &value, bool logWarning = true) const;

    std::string GlissandoToStr(data_GLISSANDO data) const;
    data_GLISSANDO StrToGlissando(const std::string &value, bool logWarning = true) const;

    std::string GraceToStr(data_GRACE data) const;
    data_GRACE StrToGrace(const std::string &value, bool logWarning = true) const;

    std::string HeadshapeToStr(data_HEADSHAPE data) const;
    data_HEADSHAPE StrToHeadshape(const std::string &value, bool logWarning = true) const;

    std::string HeadshapeListToStr(data_HEADSHAPE_list data) const;
    data_HEADSHAPE_list StrToHeadshapeList(const std::string &value, bool logWarning = true) const;

    std::string HorizontalalignmentToStr(data_HORIZONTALALIGNMENT data) const;
    data_HORIZONTALALIGNMENT StrToHorizontalalignment(const std::string &value, bool logWarning = true) const;

    std::string LayerschemeToStr(data_LAYERSCHEME data) const;
    data_LAYERSCHEME StrToLayerscheme(const std::string &value, bool logWarning = true) const;

    std::string LigatureformToStr(data_LIGATUREFORM data) const;
    data_LIGATUREFORM StrToLigatureform(const std::string &value, bool logWarning = true) const;

    std::string LineformToStr(data_LINEFORM data) const;
    data_LINEFORM StrToLineform(const std::string &value, bool logWarning = true) const;

    std::string LinestartendsymbolToStr(data_LINESTARTENDSYMBOL data) const;
    data_LINESTARTENDSYMBOL StrToLinestartendsymbol(const std::string &value, bool logWarning = true) const;

    std::string LinewidthtermToStr(data_LINEWIDTHTERM data) const;
    data_LINEWIDTHTERM StrToLinewidthterm(const std::string &value, bool logWarning = true) const;

    std::string MelodicfunctionToStr(data_MELODICFUNCTION data) const;
    data_MELODICFUNCTION StrToMelodicfunction(const std::string &value, bool logWarning = true) const;

    std::string MensurationsignToStr(data_MENSURATIONSIGN data) const;
    data_MENSURATIONSIGN StrToMensurationsign(const std::string &value, bool logWarning = true) const;

    std::string MeterformToStr(data_METERFORM data) const;
    data_METERFORM StrToMeterform(const std::string &value, bool logWarning = true) const;

    std::string MetersignToStr(data_METERSIGN data) const;
    data_METERSIGN StrToMetersign(const std::string &value, bool logWarning = true) const;

    std::string MidinamesToStr(data_MIDINAMES data) const;
    data_MIDINAMES StrToMidinames(const std::string &value, bool logWarning = true) const;

    std::string ModeToStr(data_MODE data) const;
    data_MODE StrToMode(const std::string &value, bool logWarning = true) const;

    std::string ModeCmnToStr(data_MODE_cmn data) const;
    data_MODE_cmn StrToModeCmn(const std::string &value, bool logWarning = true) const;

    std::string ModeExtendedToStr(data_MODE_extended data) const;
    data_MODE_extended StrToModeExtended(const std::string &value, bool logWarning = true) const;

    std::string ModeGregorianToStr(data_MODE_gregorian data) const;
    data_MODE_gregorian StrToModeGregorian(const std::string &value, bool logWarning = true) const;

    std::string ModsrelationshipToStr(data_MODSRELATIONSHIP data) const;
    data_MODSRELATIONSHIP StrToModsrelationship(const std::string &value, bool logWarning = true) const;

    std::string MultibreverestsMensuralToStr(data_MULTIBREVERESTS_mensural data) const;
    data_MULTIBREVERESTS_mensural StrToMultibreverestsMensural(const std::string &value, bool logWarning = true) const;

    std::string NonstaffplaceToStr(data_NONSTAFFPLACE data) const;
    data_NONSTAFFPLACE StrToNonstaffplace(const std::string &value, bool logWarning = true) const;

    std::string NotationtypeToStr(data_NOTATIONTYPE data) const;
    data_NOTATIONTYPE StrToNotationtype(const std::string &value, bool logWarning = true) const;

    std::string NoteheadmodifierToStr(data_NOTEHEADMODIFIER data) const;
    data_NOTEHEADMODIFIER StrToNoteheadmodifier(const std::string &value, bool logWarning = true) const;

    std::string NoteheadmodifierListToStr(data_NOTEHEADMODIFIER_list data) const;
    data_NOTEHEADMODIFIER_list StrToNoteheadmodifierList(const std::string &value, bool logWarning = true) const;

    std::string OtherstaffToStr(data_OTHERSTAFF data) const;
    data_OTHERSTAFF StrToOtherstaff(const std::string &value, bool logWarning = true) const;

    std::string RelationshipToStr(data_RELATIONSHIP data) const;
    data_RELATIONSHIP StrToRelationship(const std::string &value, bool logWarning = true) const;

    std::string RotationToStr(data_ROTATION data) const;
    data_ROTATION StrToRotation(const std::string &value, bool logWarning = true) const;

    std::string RotationdirectionToStr(data_ROTATIONDIRECTION data) const;
    data_ROTATIONDIRECTION StrToRotationdirection(const std::string &value, bool logWarning = true) const;

    std::string StaffitemToStr(data_STAFFITEM data) const;
    data_STAFFITEM StrToStaffitem(const std::string &value, bool logWarning = true) const;

    std::string StaffitemBasicToStr(data_STAFFITEM_basic data) const;
    data_STAFFITEM_basic StrToStaffitemBasic(const std::string &value, bool logWarning = true) const;

    std::string StaffitemCmnToStr(data_STAFFITEM_cmn data) const;
    data_STAFFITEM_cmn StrToStaffitemCmn(const std::string &value, bool logWarning = true) const;

    std::string StaffitemMensuralToStr(data_STAFFITEM_mensural data) const;
    data_STAFFITEM_mensural StrToStaffitemMensural(const std::string &value, bool logWarning = true) const;

    std::string StaffrelToStr(data_STAFFREL data) const;
    data_STAFFREL StrToStaffrel(const std::string &value, bool logWarning = true) const;

    std::string StaffrelBasicToStr(data_STAFFREL_basic data) const;
    data_STAFFREL_basic StrToStaffrelBasic(const std::string &value, bool logWarning = true) const;

    std::string StaffrelExtendedToStr(data_STAFFREL_extended data) const;
    data_STAFFREL_extended StrToStaffrelExtended(const std::string &value, bool logWarning = true) const;

    std::string StemdirectionToStr(data_STEMDIRECTION data) const;
    data_STEMDIRECTION StrToStemdirection(const std::string &value, bool logWarning = true) const;

    std::string StemdirectionBasicToStr(data_STEMDIRECTION_basic data) const;
    data_STEMDIRECTION_basic StrToStemdirectionBasic(const std::string &value, bool logWarning = true) const;

    std::string StemdirectionExtendedToStr(data_STEMDIRECTION_extended data) const;
    data_STEMDIRECTION_extended StrToStemdirectionExtended(const std::string &value, bool logWarning = true) const;

    std::string StemformMensuralToStr(data_STEMFORM_mensural data) const;
    data_STEMFORM_mensural StrToStemformMensural(const std::string &value, bool logWarning = true) const;

    std::string StemmodifierToStr(data_STEMMODIFIER data) const;
    data_STEMMODIFIER StrToStemmodifier(const std::string &value, bool logWarning = true) const;

    std::string StempositionToStr(data_STEMPOSITION data) const;
    data_STEMPOSITION StrToStemposition(const std::string &value, bool logWarning = true) const;

    std::string TemperamentToStr(data_TEMPERAMENT data) const;
    data_TEMPERAMENT StrToTemperament(const std::string &value, bool logWarning = true) const;

    std::string TextrenditionToStr(data_TEXTRENDITION data) const;
    data_TEXTRENDITION StrToTextrendition(const std::string &value, bool logWarning = true) const;

    std::string TextrenditionlistToStr(data_TEXTRENDITIONLIST data) const;
    data_TEXTRENDITIONLIST StrToTextrenditionlist(const std::string &value, bool logWarning = true) const;

    std::string VerticalalignmentToStr(data_VERTICALALIGNMENT data) const;
    data_VERTICALALIGNMENT StrToVerticalalignment(const std::string &value, bool logWarning = true) const;

    std::string AccidLogFuncToStr(accidLog_FUNC data) const;
    accidLog_FUNC StrToAccidLogFunc(const std::string &value, bool logWarning = true) const;

    std::string AnchoredTextLogFuncToStr(anchoredTextLog_FUNC data) const;
    anchoredTextLog_FUNC StrToAnchoredTextLogFunc(const std::string &value, bool logWarning = true) const;

    std::string ArpegLogOrderToStr(arpegLog_ORDER data) const;
    arpegLog_ORDER StrToArpegLogOrder(const std::string &value, bool logWarning = true) const;

    std::string AudienceAudienceToStr(audience_AUDIENCE data) const;
    audience_AUDIENCE StrToAudienceAudience(const std::string &value, bool logWarning = true) const;

    std::string BTremLogFormToStr(bTremLog_FORM data) const;
    bTremLog_FORM StrToBTremLogForm(const std::string &value, bool logWarning = true) const;

    std::string BeamRendFormToStr(beamRend_FORM data) const;
    beamRend_FORM StrToBeamRendForm(const std::string &value, bool logWarning = true) const;

    std::string BeamingVisBeamrendToStr(beamingVis_BEAMREND data) const;
    beamingVis_BEAMREND StrToBeamingVisBeamrend(const std::string &value, bool logWarning = true) const;

    std::string BracketSpanLogFuncToStr(bracketSpanLog_FUNC data) const;
    bracketSpanLog_FUNC StrToBracketSpanLogFunc(const std::string &value, bool logWarning = true) const;

    std::string CurvatureCurvedirToStr(curvature_CURVEDIR data) const;
    curvature_CURVEDIR StrToCurvatureCurvedir(const std::string &value, bool logWarning = true) const;

    std::string CurveLogFuncToStr(curveLog_FUNC data) const;
    curveLog_FUNC StrToCurveLogFunc(const std::string &value, bool logWarning = true) const;

    std::string CutoutCutoutToStr(cutout_CUTOUT data) const;
    cutout_CUTOUT StrToCutoutCutout(const std::string &value, bool logWarning = true) const;

    std::string DotLogFormToStr(dotLog_FORM data) const;
    dotLog_FORM StrToDotLogForm(const std::string &value, bool logWarning = true) const;

    std::string EndingsEndingrendToStr(endings_ENDINGREND data) const;
    endings_ENDINGREND StrToEndingsEndingrend(const std::string &value, bool logWarning = true) const;

    std::string EpisemaVisFormToStr(episemaVis_FORM data) const;
    episemaVis_FORM StrToEpisemaVisForm(const std::string &value, bool logWarning = true) const;

    std::string EvidenceEvidenceToStr(evidence_EVIDENCE data) const;
    evidence_EVIDENCE StrToEvidenceEvidence(const std::string &value, bool logWarning = true) const;

    std::string ExtSymGlyphauthToStr(extSym_GLYPHAUTH data) const;
    extSym_GLYPHAUTH StrToExtSymGlyphauth(const std::string &value, bool logWarning = true) const;

    std::string FTremLogFormToStr(fTremLog_FORM data) const;
    fTremLog_FORM StrToFTremLogForm(const std::string &value, bool logWarning = true) const;

    std::string FermataVisFormToStr(fermataVis_FORM data) const;
    fermataVis_FORM StrToFermataVisForm(const std::string &value, bool logWarning = true) const;

    std::string FermataVisShapeToStr(fermataVis_SHAPE data) const;
    fermataVis_SHAPE StrToFermataVisShape(const std::string &value, bool logWarning = true) const;

    std::string FingGrpLogFormToStr(fingGrpLog_FORM data) const;
    fingGrpLog_FORM StrToFingGrpLogForm(const std::string &value, bool logWarning = true) const;

    std::string FingGrpVisOrientToStr(fingGrpVis_ORIENT data) const;
    fingGrpVis_ORIENT StrToFingGrpVisOrient(const std::string &value, bool logWarning = true) const;

    std::string GraceGrpLogAttachToStr(graceGrpLog_ATTACH data) const;
    graceGrpLog_ATTACH StrToGraceGrpLogAttach(const std::string &value, bool logWarning = true) const;

    std::string HairpinLogFormToStr(hairpinLog_FORM data) const;
    hairpinLog_FORM StrToHairpinLogForm(const std::string &value, bool logWarning = true) const;

    std::string HarmAnlFormToStr(harmAnl_FORM data) const;
    harmAnl_FORM StrToHarmAnlForm(const std::string &value, bool logWarning = true) const;

    std::string HarmVisRendgridToStr(harmVis_RENDGRID data) const;
    harmVis_RENDGRID StrToHarmVisRendgrid(const std::string &value, bool logWarning = true) const;

    std::string HarpPedalLogAToStr(harpPedalLog_A data) const;
    harpPedalLog_A StrToHarpPedalLogA(const std::string &value, bool logWarning = true) const;

    std::string HarpPedalLogBToStr(harpPedalLog_B data) const;
    harpPedalLog_B StrToHarpPedalLogB(const std::string &value, bool logWarning = true) const;

    std::string HarpPedalLogCToStr(harpPedalLog_C data) const;
    harpPedalLog_C StrToHarpPedalLogC(const std::string &value, bool logWarning = true) const;

    std::string HarpPedalLogDToStr(harpPedalLog_D data) const;
    harpPedalLog_D StrToHarpPedalLogD(const std::string &value, bool logWarning = true) const;

    std::string HarpPedalLogEToStr(harpPedalLog_E data) const;
    harpPedalLog_E StrToHarpPedalLogE(const std::string &value, bool logWarning = true) const;

    std::string HarpPedalLogFToStr(harpPedalLog_F data) const;
    harpPedalLog_F StrToHarpPedalLogF(const std::string &value, bool logWarning = true) const;

    std::string HarpPedalLogGToStr(harpPedalLog_G data) const;
    harpPedalLog_G StrToHarpPedalLogG(const std::string &value, bool logWarning = true) const;

    std::string LineLogFuncToStr(lineLog_FUNC data) const;
    lineLog_FUNC StrToLineLogFunc(const std::string &value, bool logWarning = true) const;

    std::string LiquescentVisCurveToStr(liquescentVis_CURVE data) const;
    liquescentVis_CURVE StrToLiquescentVisCurve(const std::string &value, bool logWarning = true) const;

    std::string MeasurementUnitToStr(measurement_UNIT data) const;
    measurement_UNIT StrToMeasurementUnit(const std::string &value, bool logWarning = true) const;

    std::string MeiVersionMeiversionToStr(meiVersion_MEIVERSION data) const;
    meiVersion_MEIVERSION StrToMeiVersionMeiversion(const std::string &value, bool logWarning = true) const;

    std::string MensurVisFormToStr(mensurVis_FORM data) const;
    mensurVis_FORM StrToMensurVisForm(const std::string &value, bool logWarning = true) const;

    std::string MensuralVisMensurformToStr(mensuralVis_MENSURFORM data) const;
    mensuralVis_MENSURFORM StrToMensuralVisMensurform(const std::string &value, bool logWarning = true) const;

    std::string MeterConformanceMetconToStr(meterConformance_METCON data) const;
    meterConformance_METCON StrToMeterConformanceMetcon(const std::string &value, bool logWarning = true) const;

    std::string MeterSigGrpLogFuncToStr(meterSigGrpLog_FUNC data) const;
    meterSigGrpLog_FUNC StrToMeterSigGrpLogFunc(const std::string &value, bool logWarning = true) const;

    std::string MordentLogFormToStr(mordentLog_FORM data) const;
    mordentLog_FORM StrToMordentLogForm(const std::string &value, bool logWarning = true) const;

    std::string NcFormConToStr(ncForm_CON data) const;
    ncForm_CON StrToNcFormCon(const std::string &value, bool logWarning = true) const;

    std::string NcFormCurveToStr(ncForm_CURVE data) const;
    ncForm_CURVE StrToNcFormCurve(const std::string &value, bool logWarning = true) const;

    std::string NcFormRellenToStr(ncForm_RELLEN data) const;
    ncForm_RELLEN StrToNcFormRellen(const std::string &value, bool logWarning = true) const;

    std::string NoteGesExtremisToStr(noteGes_EXTREMIS data) const;
    noteGes_EXTREMIS StrToNoteGesExtremis(const std::string &value, bool logWarning = true) const;

    std::string NoteHeadsHeadauthToStr(noteHeads_HEADAUTH data) const;
    noteHeads_HEADAUTH StrToNoteHeadsHeadauth(const std::string &value, bool logWarning = true) const;

    std::string OctaveLogCollToStr(octaveLog_COLL data) const;
    octaveLog_COLL StrToOctaveLogColl(const std::string &value, bool logWarning = true) const;

    std::string PbVisFoliumToStr(pbVis_FOLIUM data) const;
    pbVis_FOLIUM StrToPbVisFolium(const std::string &value, bool logWarning = true) const;

    std::string PedalLogDirToStr(pedalLog_DIR data) const;
    pedalLog_DIR StrToPedalLogDir(const std::string &value, bool logWarning = true) const;

    std::string PedalLogFuncToStr(pedalLog_FUNC data) const;
    pedalLog_FUNC StrToPedalLogFunc(const std::string &value, bool logWarning = true) const;

    std::string PedalVisFormToStr(pedalVis_FORM data) const;
    pedalVis_FORM StrToPedalVisForm(const std::string &value, bool logWarning = true) const;

    std::string PianoPedalsPedalstyleToStr(pianoPedals_PEDALSTYLE data) const;
    pianoPedals_PEDALSTYLE StrToPianoPedalsPedalstyle(const std::string &value, bool logWarning = true) const;

    std::string PointingXlinkactuateToStr(pointing_XLINKACTUATE data) const;
    pointing_XLINKACTUATE StrToPointingXlinkactuate(const std::string &value, bool logWarning = true) const;

    std::string PointingXlinkshowToStr(pointing_XLINKSHOW data) const;
    pointing_XLINKSHOW StrToPointingXlinkshow(const std::string &value, bool logWarning = true) const;

    std::string RecordTypeRecordtypeToStr(recordType_RECORDTYPE data) const;
    recordType_RECORDTYPE StrToRecordTypeRecordtype(const std::string &value, bool logWarning = true) const;

    std::string RegularMethodMethodToStr(regularMethod_METHOD data) const;
    regularMethod_METHOD StrToRegularMethodMethod(const std::string &value, bool logWarning = true) const;

    std::string RehearsalRehencloseToStr(rehearsal_REHENCLOSE data) const;
    rehearsal_REHENCLOSE StrToRehearsalRehenclose(const std::string &value, bool logWarning = true) const;

    std::string SbVisFormToStr(sbVis_FORM data) const;
    sbVis_FORM StrToSbVisForm(const std::string &value, bool logWarning = true) const;

    std::string StaffGroupingSymSymbolToStr(staffGroupingSym_SYMBOL data) const;
    staffGroupingSym_SYMBOL StrToStaffGroupingSymSymbol(const std::string &value, bool logWarning = true) const;

    std::string SylLogConToStr(sylLog_CON data) const;
    sylLog_CON StrToSylLogCon(const std::string &value, bool logWarning = true) const;

    std::string SylLogWordposToStr(sylLog_WORDPOS data) const;
    sylLog_WORDPOS StrToSylLogWordpos(const std::string &value, bool logWarning = true) const;

    std::string TargetEvalEvaluateToStr(targetEval_EVALUATE data) const;
    targetEval_EVALUATE StrToTargetEvalEvaluate(const std::string &value, bool logWarning = true) const;

    std::string TempoLogFuncToStr(tempoLog_FUNC data) const;
    tempoLog_FUNC StrToTempoLogFunc(const std::string &value, bool logWarning = true) const;

    std::string TupletVisNumformatToStr(tupletVis_NUMFORMAT data) const;
    tupletVis_NUMFORMAT StrToTupletVisNumformat(const std::string &value, bool logWarning = true) const;

    std::string TurnLogFormToStr(turnLog_FORM data) const;
    turnLog_FORM StrToTurnLogForm(const std::string &value, bool logWarning = true) const;

    std::string VoltaGroupingSymVoltasymToStr(voltaGroupingSym_VOLTASYM data) const;
    voltaGroupingSym_VOLTASYM StrToVoltaGroupingSymVoltasym(const std::string &value, bool logWarning = true) const;

    std::string WhitespaceXmlspaceToStr(whitespace_XMLSPACE data) const;
    whitespace_XMLSPACE StrToWhitespaceXmlspace(const std::string &value, bool logWarning = true) const;
};

} // vrv namespace

#endif // __VRV_ATT_CONVERTER_H__
