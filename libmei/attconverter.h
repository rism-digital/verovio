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
    std::string AccidentalExplicitToStr(data_ACCIDENTAL_EXPLICIT data) const;
    data_ACCIDENTAL_EXPLICIT StrToAccidentalExplicit(std::string value) const;

    std::string AccidentalImplicitToStr(data_ACCIDENTAL_IMPLICIT data) const;
    data_ACCIDENTAL_IMPLICIT StrToAccidentalImplicit(std::string value) const;

    std::string ArticulationToStr(data_ARTICULATION data) const;
    data_ARTICULATION StrToArticulation(std::string value) const;

    std::string BarplaceToStr(data_BARPLACE data) const;
    data_BARPLACE StrToBarplace(std::string value) const;

    std::string BarrenditionToStr(data_BARRENDITION data) const;
    data_BARRENDITION StrToBarrendition(std::string value) const;

    std::string BetypeToStr(data_BETYPE data) const;
    data_BETYPE StrToBetype(std::string value) const;

    std::string BooleanToStr(data_BOOLEAN data) const;
    data_BOOLEAN StrToBoolean(std::string value) const;

    std::string CertaintyToStr(data_CERTAINTY data) const;
    data_CERTAINTY StrToCertainty(std::string value) const;

    std::string ClefshapeToStr(data_CLEFSHAPE data) const;
    data_CLEFSHAPE StrToClefshape(std::string value) const;

    std::string ClusterToStr(data_CLUSTER data) const;
    data_CLUSTER StrToCluster(std::string value) const;

    std::string ColornamesToStr(data_COLORNAMES data) const;
    data_COLORNAMES StrToColornames(std::string value) const;

    std::string CurverenditionToStr(data_CURVERENDITION data) const;
    data_CURVERENDITION StrToCurverendition(std::string value) const;

    std::string EnclosureToStr(data_ENCLOSURE data) const;
    data_ENCLOSURE StrToEnclosure(std::string value) const;

    std::string FontstyleToStr(data_FONTSTYLE data) const;
    data_FONTSTYLE StrToFontstyle(std::string value) const;

    std::string FontweightToStr(data_FONTWEIGHT data) const;
    data_FONTWEIGHT StrToFontweight(std::string value) const;

    std::string GlissandoToStr(data_GLISSANDO data) const;
    data_GLISSANDO StrToGlissando(std::string value) const;

    std::string GraceToStr(data_GRACE data) const;
    data_GRACE StrToGrace(std::string value) const;

    std::string HorizontalalignmentToStr(data_HORIZONTALALIGNMENT data) const;
    data_HORIZONTALALIGNMENT StrToHorizontalalignment(std::string value) const;

    std::string IneumeformToStr(data_INEUMEFORM data) const;
    data_INEUMEFORM StrToIneumeform(std::string value) const;

    std::string IneumenameToStr(data_INEUMENAME data) const;
    data_INEUMENAME StrToIneumename(std::string value) const;

    std::string LayerschemeToStr(data_LAYERSCHEME data) const;
    data_LAYERSCHEME StrToLayerscheme(std::string value) const;

    std::string LigatureformToStr(data_LIGATUREFORM data) const;
    data_LIGATUREFORM StrToLigatureform(std::string value) const;

    std::string LineformToStr(data_LINEFORM data) const;
    data_LINEFORM StrToLineform(std::string value) const;

    std::string LinestartendsymbolToStr(data_LINESTARTENDSYMBOL data) const;
    data_LINESTARTENDSYMBOL StrToLinestartendsymbol(std::string value) const;

    std::string LinewidthtermToStr(data_LINEWIDTHTERM data) const;
    data_LINEWIDTHTERM StrToLinewidthterm(std::string value) const;

    std::string MelodicfunctionToStr(data_MELODICFUNCTION data) const;
    data_MELODICFUNCTION StrToMelodicfunction(std::string value) const;

    std::string MensurationsignToStr(data_MENSURATIONSIGN data) const;
    data_MENSURATIONSIGN StrToMensurationsign(std::string value) const;

    std::string MetersignToStr(data_METERSIGN data) const;
    data_METERSIGN StrToMetersign(std::string value) const;

    std::string MidinamesToStr(data_MIDINAMES data) const;
    data_MIDINAMES StrToMidinames(std::string value) const;

    std::string ModeToStr(data_MODE data) const;
    data_MODE StrToMode(std::string value) const;

    std::string NotationtypeToStr(data_NOTATIONTYPE data) const;
    data_NOTATIONTYPE StrToNotationtype(std::string value) const;

    std::string NoteheadmodifierListToStr(data_NOTEHEADMODIFIER_list data) const;
    data_NOTEHEADMODIFIER_list StrToNoteheadmodifierList(std::string value) const;

    std::string OtherstaffToStr(data_OTHERSTAFF data) const;
    data_OTHERSTAFF StrToOtherstaff(std::string value) const;

    std::string PlaceToStr(data_PLACE data) const;
    data_PLACE StrToPlace(std::string value) const;

    std::string RotationdirectionToStr(data_ROTATIONDIRECTION data) const;
    data_ROTATIONDIRECTION StrToRotationdirection(std::string value) const;

    std::string SizeToStr(data_SIZE data) const;
    data_SIZE StrToSize(std::string value) const;

    std::string StaffrelToStr(data_STAFFREL data) const;
    data_STAFFREL StrToStaffrel(std::string value) const;

    std::string StemmodifierToStr(data_STEMMODIFIER data) const;
    data_STEMMODIFIER StrToStemmodifier(std::string value) const;

    std::string StempositionToStr(data_STEMPOSITION data) const;
    data_STEMPOSITION StrToStemposition(std::string value) const;

    std::string TemperamentToStr(data_TEMPERAMENT data) const;
    data_TEMPERAMENT StrToTemperament(std::string value) const;

    std::string TextrenditionlistToStr(data_TEXTRENDITIONLIST data) const;
    data_TEXTRENDITIONLIST StrToTextrenditionlist(std::string value) const;

    std::string UneumeformToStr(data_UNEUMEFORM data) const;
    data_UNEUMEFORM StrToUneumeform(std::string value) const;

    std::string UneumenameToStr(data_UNEUMENAME data) const;
    data_UNEUMENAME StrToUneumename(std::string value) const;

    std::string AccidLogFuncToStr(accidLog_FUNC data) const;
    accidLog_FUNC StrToAccidLogFunc(std::string value) const;

    std::string ArpegLogOrderToStr(arpegLog_ORDER data) const;
    arpegLog_ORDER StrToArpegLogOrder(std::string value) const;

    std::string BTremLogFormToStr(bTremLog_FORM data) const;
    bTremLog_FORM StrToBTremLogForm(std::string value) const;

    std::string BeamingVisBeamrendToStr(beamingVis_BEAMREND data) const;
    beamingVis_BEAMREND StrToBeamingVisBeamrend(std::string value) const;

    std::string BeamrendFormToStr(beamrend_FORM data) const;
    beamrend_FORM StrToBeamrendForm(std::string value) const;

    std::string CurvatureCurvedirToStr(curvature_CURVEDIR data) const;
    curvature_CURVEDIR StrToCurvatureCurvedir(std::string value) const;

    std::string CutoutCutoutToStr(cutout_CUTOUT data) const;
    cutout_CUTOUT StrToCutoutCutout(std::string value) const;

    std::string DotLogFormToStr(dotLog_FORM data) const;
    dotLog_FORM StrToDotLogForm(std::string value) const;

    std::string EndingsEndingrendToStr(endings_ENDINGREND data) const;
    endings_ENDINGREND StrToEndingsEndingrend(std::string value) const;

    std::string FTremLogFormToStr(fTremLog_FORM data) const;
    fTremLog_FORM StrToFTremLogForm(std::string value) const;

    std::string FermataVisFormToStr(fermataVis_FORM data) const;
    fermataVis_FORM StrToFermataVisForm(std::string value) const;

    std::string FermataVisShapeToStr(fermataVis_SHAPE data) const;
    fermataVis_SHAPE StrToFermataVisShape(std::string value) const;

    std::string HairpinLogFormToStr(hairpinLog_FORM data) const;
    hairpinLog_FORM StrToHairpinLogForm(std::string value) const;

    std::string HarmVisRendgridToStr(harmVis_RENDGRID data) const;
    harmVis_RENDGRID StrToHarmVisRendgrid(std::string value) const;

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

    std::string MeiversionMeiversionToStr(meiversion_MEIVERSION data) const;
    meiversion_MEIVERSION StrToMeiversionMeiversion(std::string value) const;

    std::string MensurVisFormToStr(mensurVis_FORM data) const;
    mensurVis_FORM StrToMensurVisForm(std::string value) const;

    std::string MensuralVisMensurformToStr(mensuralVis_MENSURFORM data) const;
    mensuralVis_MENSURFORM StrToMensuralVisMensurform(std::string value) const;

    std::string MeterSigVisFormToStr(meterSigVis_FORM data) const;
    meterSigVis_FORM StrToMeterSigVisForm(std::string value) const;

    std::string MeterSigDefaultVisMeterrendToStr(meterSigDefaultVis_METERREND data) const;
    meterSigDefaultVis_METERREND StrToMeterSigDefaultVisMeterrend(std::string value) const;

    std::string MeterconformanceMetconToStr(meterconformance_METCON data) const;
    meterconformance_METCON StrToMeterconformanceMetcon(std::string value) const;

    std::string MordentLogFormToStr(mordentLog_FORM data) const;
    mordentLog_FORM StrToMordentLogForm(std::string value) const;

    std::string NoteLogMensuralLigToStr(noteLogMensural_LIG data) const;
    noteLogMensural_LIG StrToNoteLogMensuralLig(std::string value) const;

    std::string OctaveLogCollToStr(octaveLog_COLL data) const;
    octaveLog_COLL StrToOctaveLogColl(std::string value) const;

    std::string PbVisFoliumToStr(pbVis_FOLIUM data) const;
    pbVis_FOLIUM StrToPbVisFolium(std::string value) const;

    std::string PedalLogDirToStr(pedalLog_DIR data) const;
    pedalLog_DIR StrToPedalLogDir(std::string value) const;

    std::string PedalVisFormToStr(pedalVis_FORM data) const;
    pedalVis_FORM StrToPedalVisForm(std::string value) const;

    std::string PianopedalsPedalstyleToStr(pianopedals_PEDALSTYLE data) const;
    pianopedals_PEDALSTYLE StrToPianopedalsPedalstyle(std::string value) const;

    std::string PlistEvaluateToStr(plist_EVALUATE data) const;
    plist_EVALUATE StrToPlistEvaluate(std::string value) const;

    std::string PointingXlinkactuateToStr(pointing_XLINKACTUATE data) const;
    pointing_XLINKACTUATE StrToPointingXlinkactuate(std::string value) const;

    std::string PointingXlinkshowToStr(pointing_XLINKSHOW data) const;
    pointing_XLINKSHOW StrToPointingXlinkshow(std::string value) const;

    std::string RegularmethodMethodToStr(regularmethod_METHOD data) const;
    regularmethod_METHOD StrToRegularmethodMethod(std::string value) const;

    std::string RehearsalRehencloseToStr(rehearsal_REHENCLOSE data) const;
    rehearsal_REHENCLOSE StrToRehearsalRehenclose(std::string value) const;

    std::string SbVisFormToStr(sbVis_FORM data) const;
    sbVis_FORM StrToSbVisForm(std::string value) const;

    std::string StaffgroupingsymSymbolToStr(staffgroupingsym_SYMBOL data) const;
    staffgroupingsym_SYMBOL StrToStaffgroupingsymSymbol(std::string value) const;

    std::string SylLogConToStr(sylLog_CON data) const;
    sylLog_CON StrToSylLogCon(std::string value) const;

    std::string SylLogWordposToStr(sylLog_WORDPOS data) const;
    sylLog_WORDPOS StrToSylLogWordpos(std::string value) const;

    std::string TupletVisNumformatToStr(tupletVis_NUMFORMAT data) const;
    tupletVis_NUMFORMAT StrToTupletVisNumformat(std::string value) const;

    std::string TurnLogFormToStr(turnLog_FORM data) const;
    turnLog_FORM StrToTurnLogForm(std::string value) const;

    std::string WhitespaceXmlspaceToStr(whitespace_XMLSPACE data) const;
    whitespace_XMLSPACE StrToWhitespaceXmlspace(std::string value) const;
};

} // vrv namespace

#endif // __VRV_ATT_CONVERTER_H__
