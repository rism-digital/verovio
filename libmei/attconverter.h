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

class AttConverter
{
public:

    std::string AccidentalExplicitToStr(data_ACCIDENTAL_EXPLICIT data);
    data_ACCIDENTAL_EXPLICIT StrToAccidentalExplicit(std::string value);

    std::string AccidentalImplicitToStr(data_ACCIDENTAL_IMPLICIT data);
    data_ACCIDENTAL_IMPLICIT StrToAccidentalImplicit(std::string value);

    std::string ArticulationToStr(data_ARTICULATION data);
    data_ARTICULATION StrToArticulation(std::string value);

    std::string BarplaceToStr(data_BARPLACE data);
    data_BARPLACE StrToBarplace(std::string value);

    std::string BarrenditionToStr(data_BARRENDITION data);
    data_BARRENDITION StrToBarrendition(std::string value);

    std::string BetypeToStr(data_BETYPE data);
    data_BETYPE StrToBetype(std::string value);

    std::string BooleanToStr(data_BOOLEAN data);
    data_BOOLEAN StrToBoolean(std::string value);

    std::string CertaintyToStr(data_CERTAINTY data);
    data_CERTAINTY StrToCertainty(std::string value);

    std::string ClefshapeToStr(data_CLEFSHAPE data);
    data_CLEFSHAPE StrToClefshape(std::string value);

    std::string ClusterToStr(data_CLUSTER data);
    data_CLUSTER StrToCluster(std::string value);

    std::string ColornamesToStr(data_COLORNAMES data);
    data_COLORNAMES StrToColornames(std::string value);

    std::string CurverenditionToStr(data_CURVERENDITION data);
    data_CURVERENDITION StrToCurverendition(std::string value);

    std::string EnclosureToStr(data_ENCLOSURE data);
    data_ENCLOSURE StrToEnclosure(std::string value);

    std::string FontstyleToStr(data_FONTSTYLE data);
    data_FONTSTYLE StrToFontstyle(std::string value);

    std::string FontweightToStr(data_FONTWEIGHT data);
    data_FONTWEIGHT StrToFontweight(std::string value);

    std::string GlissandoToStr(data_GLISSANDO data);
    data_GLISSANDO StrToGlissando(std::string value);

    std::string GraceToStr(data_GRACE data);
    data_GRACE StrToGrace(std::string value);

    std::string HorizontalalignmentToStr(data_HORIZONTALALIGNMENT data);
    data_HORIZONTALALIGNMENT StrToHorizontalalignment(std::string value);

    std::string IneumeformToStr(data_INEUMEFORM data);
    data_INEUMEFORM StrToIneumeform(std::string value);

    std::string IneumenameToStr(data_INEUMENAME data);
    data_INEUMENAME StrToIneumename(std::string value);

    std::string LayerschemeToStr(data_LAYERSCHEME data);
    data_LAYERSCHEME StrToLayerscheme(std::string value);

    std::string LigatureformToStr(data_LIGATUREFORM data);
    data_LIGATUREFORM StrToLigatureform(std::string value);

    std::string LineformToStr(data_LINEFORM data);
    data_LINEFORM StrToLineform(std::string value);

    std::string LinestartendsymbolToStr(data_LINESTARTENDSYMBOL data);
    data_LINESTARTENDSYMBOL StrToLinestartendsymbol(std::string value);

    std::string LinewidthtermToStr(data_LINEWIDTHTERM data);
    data_LINEWIDTHTERM StrToLinewidthterm(std::string value);

    std::string MelodicfunctionToStr(data_MELODICFUNCTION data);
    data_MELODICFUNCTION StrToMelodicfunction(std::string value);

    std::string MensurationsignToStr(data_MENSURATIONSIGN data);
    data_MENSURATIONSIGN StrToMensurationsign(std::string value);

    std::string MetersignToStr(data_METERSIGN data);
    data_METERSIGN StrToMetersign(std::string value);

    std::string MidinamesToStr(data_MIDINAMES data);
    data_MIDINAMES StrToMidinames(std::string value);

    std::string ModeToStr(data_MODE data);
    data_MODE StrToMode(std::string value);

    std::string NotationtypeToStr(data_NOTATIONTYPE data);
    data_NOTATIONTYPE StrToNotationtype(std::string value);

    std::string NoteheadmodifierListToStr(data_NOTEHEADMODIFIER_list data);
    data_NOTEHEADMODIFIER_list StrToNoteheadmodifierList(std::string value);

    std::string OtherstaffToStr(data_OTHERSTAFF data);
    data_OTHERSTAFF StrToOtherstaff(std::string value);

    std::string PlaceToStr(data_PLACE data);
    data_PLACE StrToPlace(std::string value);

    std::string RotationdirectionToStr(data_ROTATIONDIRECTION data);
    data_ROTATIONDIRECTION StrToRotationdirection(std::string value);

    std::string SizeToStr(data_SIZE data);
    data_SIZE StrToSize(std::string value);

    std::string StaffrelToStr(data_STAFFREL data);
    data_STAFFREL StrToStaffrel(std::string value);

    std::string StemmodifierToStr(data_STEMMODIFIER data);
    data_STEMMODIFIER StrToStemmodifier(std::string value);

    std::string StempositionToStr(data_STEMPOSITION data);
    data_STEMPOSITION StrToStemposition(std::string value);

    std::string TemperamentToStr(data_TEMPERAMENT data);
    data_TEMPERAMENT StrToTemperament(std::string value);

    std::string TextrenditionlistToStr(data_TEXTRENDITIONLIST data);
    data_TEXTRENDITIONLIST StrToTextrenditionlist(std::string value);

    std::string UneumeformToStr(data_UNEUMEFORM data);
    data_UNEUMEFORM StrToUneumeform(std::string value);

    std::string UneumenameToStr(data_UNEUMENAME data);
    data_UNEUMENAME StrToUneumename(std::string value);

    std::string AccidLogFuncToStr(accidLog_FUNC data);
    accidLog_FUNC StrToAccidLogFunc(std::string value);

    std::string ArpegLogOrderToStr(arpegLog_ORDER data);
    arpegLog_ORDER StrToArpegLogOrder(std::string value);

    std::string BTremLogFormToStr(bTremLog_FORM data);
    bTremLog_FORM StrToBTremLogForm(std::string value);

    std::string BeamingVisBeamrendToStr(beamingVis_BEAMREND data);
    beamingVis_BEAMREND StrToBeamingVisBeamrend(std::string value);

    std::string BeamrendFormToStr(beamrend_FORM data);
    beamrend_FORM StrToBeamrendForm(std::string value);

    std::string CurvatureCurvedirToStr(curvature_CURVEDIR data);
    curvature_CURVEDIR StrToCurvatureCurvedir(std::string value);

    std::string CutoutCutoutToStr(cutout_CUTOUT data);
    cutout_CUTOUT StrToCutoutCutout(std::string value);

    std::string DotLogFormToStr(dotLog_FORM data);
    dotLog_FORM StrToDotLogForm(std::string value);

    std::string EndingsEndingrendToStr(endings_ENDINGREND data);
    endings_ENDINGREND StrToEndingsEndingrend(std::string value);

    std::string FTremLogFormToStr(fTremLog_FORM data);
    fTremLog_FORM StrToFTremLogForm(std::string value);

    std::string FermataVisFormToStr(fermataVis_FORM data);
    fermataVis_FORM StrToFermataVisForm(std::string value);

    std::string FermataVisShapeToStr(fermataVis_SHAPE data);
    fermataVis_SHAPE StrToFermataVisShape(std::string value);

    std::string HairpinLogFormToStr(hairpinLog_FORM data);
    hairpinLog_FORM StrToHairpinLogForm(std::string value);

    std::string HarmVisRendgridToStr(harmVis_RENDGRID data);
    harmVis_RENDGRID StrToHarmVisRendgrid(std::string value);

    std::string HarpPedalLogAToStr(harpPedalLog_A data);
    harpPedalLog_A StrToHarpPedalLogA(std::string value);

    std::string HarpPedalLogBToStr(harpPedalLog_B data);
    harpPedalLog_B StrToHarpPedalLogB(std::string value);

    std::string HarpPedalLogCToStr(harpPedalLog_C data);
    harpPedalLog_C StrToHarpPedalLogC(std::string value);

    std::string HarpPedalLogDToStr(harpPedalLog_D data);
    harpPedalLog_D StrToHarpPedalLogD(std::string value);

    std::string HarpPedalLogEToStr(harpPedalLog_E data);
    harpPedalLog_E StrToHarpPedalLogE(std::string value);

    std::string HarpPedalLogFToStr(harpPedalLog_F data);
    harpPedalLog_F StrToHarpPedalLogF(std::string value);

    std::string HarpPedalLogGToStr(harpPedalLog_G data);
    harpPedalLog_G StrToHarpPedalLogG(std::string value);

    std::string MeiversionMeiversionToStr(meiversion_MEIVERSION data);
    meiversion_MEIVERSION StrToMeiversionMeiversion(std::string value);

    std::string MensurVisFormToStr(mensurVis_FORM data);
    mensurVis_FORM StrToMensurVisForm(std::string value);

    std::string MensuralVisMensurformToStr(mensuralVis_MENSURFORM data);
    mensuralVis_MENSURFORM StrToMensuralVisMensurform(std::string value);

    std::string MeterSigVisFormToStr(meterSigVis_FORM data);
    meterSigVis_FORM StrToMeterSigVisForm(std::string value);

    std::string MeterSigDefaultVisMeterrendToStr(meterSigDefaultVis_METERREND data);
    meterSigDefaultVis_METERREND StrToMeterSigDefaultVisMeterrend(std::string value);

    std::string MeterconformanceMetconToStr(meterconformance_METCON data);
    meterconformance_METCON StrToMeterconformanceMetcon(std::string value);

    std::string MordentLogFormToStr(mordentLog_FORM data);
    mordentLog_FORM StrToMordentLogForm(std::string value);

    std::string NoteLogMensuralLigToStr(noteLogMensural_LIG data);
    noteLogMensural_LIG StrToNoteLogMensuralLig(std::string value);

    std::string OctaveLogCollToStr(octaveLog_COLL data);
    octaveLog_COLL StrToOctaveLogColl(std::string value);

    std::string PbVisFoliumToStr(pbVis_FOLIUM data);
    pbVis_FOLIUM StrToPbVisFolium(std::string value);

    std::string PedalLogDirToStr(pedalLog_DIR data);
    pedalLog_DIR StrToPedalLogDir(std::string value);

    std::string PedalVisFormToStr(pedalVis_FORM data);
    pedalVis_FORM StrToPedalVisForm(std::string value);

    std::string PianopedalsPedalstyleToStr(pianopedals_PEDALSTYLE data);
    pianopedals_PEDALSTYLE StrToPianopedalsPedalstyle(std::string value);

    std::string PlistEvaluateToStr(plist_EVALUATE data);
    plist_EVALUATE StrToPlistEvaluate(std::string value);

    std::string PointingXlinkactuateToStr(pointing_XLINKACTUATE data);
    pointing_XLINKACTUATE StrToPointingXlinkactuate(std::string value);

    std::string PointingXlinkshowToStr(pointing_XLINKSHOW data);
    pointing_XLINKSHOW StrToPointingXlinkshow(std::string value);

    std::string RegularmethodMethodToStr(regularmethod_METHOD data);
    regularmethod_METHOD StrToRegularmethodMethod(std::string value);

    std::string RehearsalRehencloseToStr(rehearsal_REHENCLOSE data);
    rehearsal_REHENCLOSE StrToRehearsalRehenclose(std::string value);

    std::string SbVisFormToStr(sbVis_FORM data);
    sbVis_FORM StrToSbVisForm(std::string value);

    std::string StaffgroupingsymSymbolToStr(staffgroupingsym_SYMBOL data);
    staffgroupingsym_SYMBOL StrToStaffgroupingsymSymbol(std::string value);

    std::string SylLogConToStr(sylLog_CON data);
    sylLog_CON StrToSylLogCon(std::string value);

    std::string SylLogWordposToStr(sylLog_WORDPOS data);
    sylLog_WORDPOS StrToSylLogWordpos(std::string value);

    std::string TupletVisNumformatToStr(tupletVis_NUMFORMAT data);
    tupletVis_NUMFORMAT StrToTupletVisNumformat(std::string value);

    std::string TurnLogFormToStr(turnLog_FORM data);
    turnLog_FORM StrToTurnLogForm(std::string value);

    std::string WhitespaceXmlspaceToStr(whitespace_XMLSPACE data);
    whitespace_XMLSPACE StrToWhitespaceXmlspace(std::string value);


};

} // vrv namespace

#endif  // __VRV_ATT_CONVERTER_H__

