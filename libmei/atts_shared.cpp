/////////////////////////////////////////////////////////////////////////////
// Name:        attributes.h
// Author:      Laurent Pugin
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

#include "atts_shared.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttAccidLog
//----------------------------------------------------------------------------

AttAccidLog::AttAccidLog() {
    ResetAccidLog();
}

AttAccidLog::~AttAccidLog() {

}

void AttAccidLog::ResetAccidLog() {
    m_func = "";
}

/* include <attfunc> */

//----------------------------------------------------------------------------
// AttAccidental
//----------------------------------------------------------------------------

AttAccidental::AttAccidental() {
    ResetAccidental();
}

AttAccidental::~AttAccidental() {

}

void AttAccidental::ResetAccidental() {
    m_accid = "";
}

/* include <attaccid> */

//----------------------------------------------------------------------------
// AttAccidentalPerformed
//----------------------------------------------------------------------------

AttAccidentalPerformed::AttAccidentalPerformed() {
    ResetAccidentalPerformed();
}

AttAccidentalPerformed::~AttAccidentalPerformed() {

}

void AttAccidentalPerformed::ResetAccidentalPerformed() {
    m_accidGes = "";
}

/* include <attaccid.ges> */

//----------------------------------------------------------------------------
// AttAltsym
//----------------------------------------------------------------------------

AttAltsym::AttAltsym() {
    ResetAltsym();
}

AttAltsym::~AttAltsym() {

}

void AttAltsym::ResetAltsym() {
    m_altsym = "";
}

/* include <attaltsym> */

//----------------------------------------------------------------------------
// AttArticulation
//----------------------------------------------------------------------------

AttArticulation::AttArticulation() {
    ResetArticulation();
}

AttArticulation::~AttArticulation() {

}

void AttArticulation::ResetArticulation() {
    m_artic = "";
}

/* include <attartic> */

//----------------------------------------------------------------------------
// AttArticulationPerformed
//----------------------------------------------------------------------------

AttArticulationPerformed::AttArticulationPerformed() {
    ResetArticulationPerformed();
}

AttArticulationPerformed::~AttArticulationPerformed() {

}

void AttArticulationPerformed::ResetArticulationPerformed() {
    m_articGes = "";
}

/* include <attartic.ges> */

//----------------------------------------------------------------------------
// AttAugmentdots
//----------------------------------------------------------------------------

AttAugmentdots::AttAugmentdots() {
    ResetAugmentdots();
}

AttAugmentdots::~AttAugmentdots() {

}

void AttAugmentdots::ResetAugmentdots() {
    m_dots = "";
}

/* include <attdots> */

//----------------------------------------------------------------------------
// AttAuthorized
//----------------------------------------------------------------------------

AttAuthorized::AttAuthorized() {
    ResetAuthorized();
}

AttAuthorized::~AttAuthorized() {

}

void AttAuthorized::ResetAuthorized() {
    m_authority = "";
    m_authURI = "";
}

/* include <attauthURI> */

//----------------------------------------------------------------------------
// AttBarLineLog
//----------------------------------------------------------------------------

AttBarLineLog::AttBarLineLog() {
    ResetBarLineLog();
}

AttBarLineLog::~AttBarLineLog() {

}

void AttBarLineLog::ResetBarLineLog() {
    m_rend = "";
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttBarplacement
//----------------------------------------------------------------------------

AttBarplacement::AttBarplacement() {
    ResetBarplacement();
}

AttBarplacement::~AttBarplacement() {

}

void AttBarplacement::ResetBarplacement() {
    m_barplace = "";
    m_taktplace = "";
}

/* include <atttaktplace> */

//----------------------------------------------------------------------------
// AttBeamingVis
//----------------------------------------------------------------------------

AttBeamingVis::AttBeamingVis() {
    ResetBeamingVis();
}

AttBeamingVis::~AttBeamingVis() {

}

void AttBeamingVis::ResetBeamingVis() {
    m_beamRend = "";
    m_beamSlopeDbl = 0.0;
}

/* include <attbeam.slope> */

//----------------------------------------------------------------------------
// AttBibl
//----------------------------------------------------------------------------

AttBibl::AttBibl() {
    ResetBibl();
}

AttBibl::~AttBibl() {

}

void AttBibl::ResetBibl() {
    m_analog = "";
}

/* include <attanalog> */

//----------------------------------------------------------------------------
// AttCalendared
//----------------------------------------------------------------------------

AttCalendared::AttCalendared() {
    ResetCalendared();
}

AttCalendared::~AttCalendared() {

}

void AttCalendared::ResetCalendared() {
    m_calendar = "";
}

/* include <attcalendar> */

//----------------------------------------------------------------------------
// AttCanonical
//----------------------------------------------------------------------------

AttCanonical::AttCanonical() {
    ResetCanonical();
}

AttCanonical::~AttCanonical() {

}

void AttCanonical::ResetCanonical() {
    m_dbkey = "";
}

/* include <attdbkey> */

//----------------------------------------------------------------------------
// AttChordVis
//----------------------------------------------------------------------------

AttChordVis::AttChordVis() {
    ResetChordVis();
}

AttChordVis::~AttChordVis() {

}

void AttChordVis::ResetChordVis() {
    m_cluster = "";
}

/* include <attcluster> */

//----------------------------------------------------------------------------
// AttClefLog
//----------------------------------------------------------------------------

AttClefLog::AttClefLog() {
    ResetClefLog();
}

AttClefLog::~AttClefLog() {

}

void AttClefLog::ResetClefLog() {
    m_cautionary = "";
}

/* include <attcautionary> */

//----------------------------------------------------------------------------
// AttCleffingLog
//----------------------------------------------------------------------------

AttCleffingLog::AttCleffingLog() {
    ResetCleffingLog();
}

AttCleffingLog::~AttCleffingLog() {

}

void AttCleffingLog::ResetCleffingLog() {
    m_clefShape = "";
    m_clefLine = "";
    m_clefDis = "";
    m_clefDisPlace = "";
}

/* include <attclef.dis.place> */

//----------------------------------------------------------------------------
// AttCleffingVis
//----------------------------------------------------------------------------

AttCleffingVis::AttCleffingVis() {
    ResetCleffingVis();
}

AttCleffingVis::~AttCleffingVis() {

}

void AttCleffingVis::ResetCleffingVis() {
    m_clefColor = "";
    m_clefVisible = "";
}

/* include <attclef.visible> */

//----------------------------------------------------------------------------
// AttClefshape
//----------------------------------------------------------------------------

AttClefshape::AttClefshape() {
    ResetClefshape();
}

AttClefshape::~AttClefshape() {

}

void AttClefshape::ResetClefshape() {
    m_shape = "";
}

/* include <attshape> */

//----------------------------------------------------------------------------
// AttColor
//----------------------------------------------------------------------------

AttColor::AttColor() {
    ResetColor();
}

AttColor::~AttColor() {

}

void AttColor::ResetColor() {
    m_color = "";
}

/* include <attcolor> */

//----------------------------------------------------------------------------
// AttColoration
//----------------------------------------------------------------------------

AttColoration::AttColoration() {
    ResetColoration();
}

AttColoration::~AttColoration() {

}

void AttColoration::ResetColoration() {
    m_colored = "";
}

/* include <attcolored> */

//----------------------------------------------------------------------------
// AttCommon
//----------------------------------------------------------------------------

AttCommon::AttCommon() {
    ResetCommon();
}

AttCommon::~AttCommon() {

}

void AttCommon::ResetCommon() {
    m_label = "";
    m_n = 0;
    m_base = "";
}

/* include <attbase> */

//----------------------------------------------------------------------------
// AttCoordinated
//----------------------------------------------------------------------------

AttCoordinated::AttCoordinated() {
    ResetCoordinated();
}

AttCoordinated::~AttCoordinated() {

}

void AttCoordinated::ResetCoordinated() {
    m_ulxInt = 0;
    m_ulyInt = 0;
    m_lrxInt = 0;
    m_lryInt = 0;
}

/* include <attlry> */

//----------------------------------------------------------------------------
// AttCurvature
//----------------------------------------------------------------------------

AttCurvature::AttCurvature() {
    ResetCurvature();
}

AttCurvature::~AttCurvature() {

}

void AttCurvature::ResetCurvature() {
    m_bezier = "";
    m_bulge = "";
    m_curvedir = "";
}

/* include <attcurvedir> */

//----------------------------------------------------------------------------
// AttCurverend
//----------------------------------------------------------------------------

AttCurverend::AttCurverend() {
    ResetCurverend();
}

AttCurverend::~AttCurverend() {

}

void AttCurverend::ResetCurverend() {
    m_rend = "";
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttCustosLog
//----------------------------------------------------------------------------

AttCustosLog::AttCustosLog() {
    ResetCustosLog();
}

AttCustosLog::~AttCustosLog() {

}

void AttCustosLog::ResetCustosLog() {
    m_target = "";
}

/* include <atttarget> */

//----------------------------------------------------------------------------
// AttDatable
//----------------------------------------------------------------------------

AttDatable::AttDatable() {
    ResetDatable();
}

AttDatable::~AttDatable() {

}

void AttDatable::ResetDatable() {
    m_enddate = "";
    m_isodate = "";
    m_notafter = "";
    m_notbefore = "";
    m_startdate = "";
}

/* include <attstartdate> */

//----------------------------------------------------------------------------
// AttDatapointing
//----------------------------------------------------------------------------

AttDatapointing::AttDatapointing() {
    ResetDatapointing();
}

AttDatapointing::~AttDatapointing() {

}

void AttDatapointing::ResetDatapointing() {
    m_data = "";
}

/* include <attdata> */

//----------------------------------------------------------------------------
// AttDeclaring
//----------------------------------------------------------------------------

AttDeclaring::AttDeclaring() {
    ResetDeclaring();
}

AttDeclaring::~AttDeclaring() {

}

void AttDeclaring::ResetDeclaring() {
    m_decls = "";
}

/* include <attdecls> */

//----------------------------------------------------------------------------
// AttDistances
//----------------------------------------------------------------------------

AttDistances::AttDistances() {
    ResetDistances();
}

AttDistances::~AttDistances() {

}

void AttDistances::ResetDistances() {
    m_dynamDistDbl = 0.0;
    m_harmDistDbl = 0.0;
    m_textDistDbl = 0.0;
}

/* include <atttext.dist> */

//----------------------------------------------------------------------------
// AttDotLog
//----------------------------------------------------------------------------

AttDotLog::AttDotLog() {
    ResetDotLog();
}

AttDotLog::~AttDotLog() {

}

void AttDotLog::ResetDotLog() {
    m_form = "";
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttDurationDefault
//----------------------------------------------------------------------------

AttDurationDefault::AttDurationDefault() {
    ResetDurationDefault();
}

AttDurationDefault::~AttDurationDefault() {

}

void AttDurationDefault::ResetDurationDefault() {
    m_durDefault = "";
}

/* include <attdur.default> */

//----------------------------------------------------------------------------
// AttDurationMusical
//----------------------------------------------------------------------------

AttDurationMusical::AttDurationMusical() {
    ResetDurationMusical();
}

AttDurationMusical::~AttDurationMusical() {

}

void AttDurationMusical::ResetDurationMusical() {
    m_dur = "";
}

/* include <attdur> */

//----------------------------------------------------------------------------
// AttDurationPerformed
//----------------------------------------------------------------------------

AttDurationPerformed::AttDurationPerformed() {
    ResetDurationPerformed();
}

AttDurationPerformed::~AttDurationPerformed() {

}

void AttDurationPerformed::ResetDurationPerformed() {
    m_durGes = "";
}

/* include <attdur.ges> */

//----------------------------------------------------------------------------
// AttDurationRatio
//----------------------------------------------------------------------------

AttDurationRatio::AttDurationRatio() {
    ResetDurationRatio();
}

AttDurationRatio::~AttDurationRatio() {

}

void AttDurationRatio::ResetDurationRatio() {
    m_numInt = 0;
    m_numbaseInt = 0;
}

/* include <attnumbase> */

//----------------------------------------------------------------------------
// AttDurationTimestamp
//----------------------------------------------------------------------------

AttDurationTimestamp::AttDurationTimestamp() {
    ResetDurationTimestamp();
}

AttDurationTimestamp::~AttDurationTimestamp() {

}

void AttDurationTimestamp::ResetDurationTimestamp() {
    m_dur = "";
}

/* include <attdur> */

//----------------------------------------------------------------------------
// AttEnclosingchars
//----------------------------------------------------------------------------

AttEnclosingchars::AttEnclosingchars() {
    ResetEnclosingchars();
}

AttEnclosingchars::~AttEnclosingchars() {

}

void AttEnclosingchars::ResetEnclosingchars() {
    m_enclose = "";
}

/* include <attenclose> */

//----------------------------------------------------------------------------
// AttFermatapresent
//----------------------------------------------------------------------------

AttFermatapresent::AttFermatapresent() {
    ResetFermatapresent();
}

AttFermatapresent::~AttFermatapresent() {

}

void AttFermatapresent::ResetFermatapresent() {
    m_fermata = "";
}

/* include <attfermata> */

//----------------------------------------------------------------------------
// AttHandident
//----------------------------------------------------------------------------

AttHandident::AttHandident() {
    ResetHandident();
}

AttHandident::~AttHandident() {

}

void AttHandident::ResetHandident() {
    m_hand = "";
}

/* include <atthand> */

//----------------------------------------------------------------------------
// AttHorizontalalign
//----------------------------------------------------------------------------

AttHorizontalalign::AttHorizontalalign() {
    ResetHorizontalalign();
}

AttHorizontalalign::~AttHorizontalalign() {

}

void AttHorizontalalign::ResetHorizontalalign() {
    m_halign = "";
}

/* include <atthalign> */

//----------------------------------------------------------------------------
// AttInstrumentident
//----------------------------------------------------------------------------

AttInstrumentident::AttInstrumentident() {
    ResetInstrumentident();
}

AttInstrumentident::~AttInstrumentident() {

}

void AttInstrumentident::ResetInstrumentident() {
    m_instr = "";
}

/* include <attinstr> */

//----------------------------------------------------------------------------
// AttInternetmedia
//----------------------------------------------------------------------------

AttInternetmedia::AttInternetmedia() {
    ResetInternetmedia();
}

AttInternetmedia::~AttInternetmedia() {

}

void AttInternetmedia::ResetInternetmedia() {
    m_mimetype = "";
}

/* include <attmimetype> */

//----------------------------------------------------------------------------
// AttJoined
//----------------------------------------------------------------------------

AttJoined::AttJoined() {
    ResetJoined();
}

AttJoined::~AttJoined() {

}

void AttJoined::ResetJoined() {
    m_join = "";
}

/* include <attjoin> */

//----------------------------------------------------------------------------
// AttKeySigLog
//----------------------------------------------------------------------------

AttKeySigLog::AttKeySigLog() {
    ResetKeySigLog();
}

AttKeySigLog::~AttKeySigLog() {

}

void AttKeySigLog::ResetKeySigLog() {
    m_mode = "";
}

/* include <attmode> */

//----------------------------------------------------------------------------
// AttKeySigDefaultLog
//----------------------------------------------------------------------------

AttKeySigDefaultLog::AttKeySigDefaultLog() {
    ResetKeySigDefaultLog();
}

AttKeySigDefaultLog::~AttKeySigDefaultLog() {

}

void AttKeySigDefaultLog::ResetKeySigDefaultLog() {
    m_keyAccid = "";
    m_keyMode = "";
    m_keyPname = "";
    m_keySig = "";
    m_keySigMixed = "";
}

/* include <attkey.sig.mixed> */

//----------------------------------------------------------------------------
// AttKeySigDefaultVis
//----------------------------------------------------------------------------

AttKeySigDefaultVis::AttKeySigDefaultVis() {
    ResetKeySigDefaultVis();
}

AttKeySigDefaultVis::~AttKeySigDefaultVis() {

}

void AttKeySigDefaultVis::ResetKeySigDefaultVis() {
    m_keySigShow = "";
    m_keySigShowchange = "";
}

/* include <attkey.sig.showchange> */

//----------------------------------------------------------------------------
// AttLabelsAddl
//----------------------------------------------------------------------------

AttLabelsAddl::AttLabelsAddl() {
    ResetLabelsAddl();
}

AttLabelsAddl::~AttLabelsAddl() {

}

void AttLabelsAddl::ResetLabelsAddl() {
    m_labelAbbr = "";
}

/* include <attlabel.abbr> */

//----------------------------------------------------------------------------
// AttLang
//----------------------------------------------------------------------------

AttLang::AttLang() {
    ResetLang();
}

AttLang::~AttLang() {

}

void AttLang::ResetLang() {
    m_lang = "";
}

/* include <attlang> */

//----------------------------------------------------------------------------
// AttLayerLog
//----------------------------------------------------------------------------

AttLayerLog::AttLayerLog() {
    ResetLayerLog();
}

AttLayerLog::~AttLayerLog() {

}

void AttLayerLog::ResetLayerLog() {
    m_def = "";
}

/* include <attdef> */

//----------------------------------------------------------------------------
// AttLayerident
//----------------------------------------------------------------------------

AttLayerident::AttLayerident() {
    ResetLayerident();
}

AttLayerident::~AttLayerident() {

}

void AttLayerident::ResetLayerident() {
    m_layer = "";
}

/* include <attlayer> */

//----------------------------------------------------------------------------
// AttLineloc
//----------------------------------------------------------------------------

AttLineloc::AttLineloc() {
    ResetLineloc();
}

AttLineloc::~AttLineloc() {

}

void AttLineloc::ResetLineloc() {
    m_line = "";
}

/* include <attline> */

//----------------------------------------------------------------------------
// AttLinerend
//----------------------------------------------------------------------------

AttLinerend::AttLinerend() {
    ResetLinerend();
}

AttLinerend::~AttLinerend() {

}

void AttLinerend::ResetLinerend() {
    m_rend = "";
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttLyricstyle
//----------------------------------------------------------------------------

AttLyricstyle::AttLyricstyle() {
    ResetLyricstyle();
}

AttLyricstyle::~AttLyricstyle() {

}

void AttLyricstyle::ResetLyricstyle() {
    m_lyricAlignDbl = 0.0;
    m_lyricFam = "";
    m_lyricName = "";
    m_lyricSizeDbl = 0.0;
    m_lyricStyle = "";
    m_lyricWeight = "";
}

/* include <attlyric.weight> */

//----------------------------------------------------------------------------
// AttMeasureLog
//----------------------------------------------------------------------------

AttMeasureLog::AttMeasureLog() {
    ResetMeasureLog();
}

AttMeasureLog::~AttMeasureLog() {

}

void AttMeasureLog::ResetMeasureLog() {
    m_left = "";
    m_right = "";
}

/* include <attright> */

//----------------------------------------------------------------------------
// AttMeasurement
//----------------------------------------------------------------------------

AttMeasurement::AttMeasurement() {
    ResetMeasurement();
}

AttMeasurement::~AttMeasurement() {

}

void AttMeasurement::ResetMeasurement() {
    m_unitDbl = 0.0;
}

/* include <attunit> */

//----------------------------------------------------------------------------
// AttMediabounds
//----------------------------------------------------------------------------

AttMediabounds::AttMediabounds() {
    ResetMediabounds();
}

AttMediabounds::~AttMediabounds() {

}

void AttMediabounds::ResetMediabounds() {
    m_begin = "";
    m_end = "";
    m_betype = "";
}

/* include <attbetype> */

//----------------------------------------------------------------------------
// AttMedium
//----------------------------------------------------------------------------

AttMedium::AttMedium() {
    ResetMedium();
}

AttMedium::~AttMedium() {

}

void AttMedium::ResetMedium() {
    m_medium = "";
}

/* include <attmedium> */

//----------------------------------------------------------------------------
// AttMeiversion
//----------------------------------------------------------------------------

AttMeiversion::AttMeiversion() {
    ResetMeiversion();
}

AttMeiversion::~AttMeiversion() {

}

void AttMeiversion::ResetMeiversion() {
    m_meiversion = "";
}

/* include <attmeiversion> */

//----------------------------------------------------------------------------
// AttMensurLog
//----------------------------------------------------------------------------

AttMensurLog::AttMensurLog() {
    ResetMensurLog();
}

AttMensurLog::~AttMensurLog() {

}

void AttMensurLog::ResetMensurLog() {
    m_dot = "";
    m_modusmaior = "";
    m_modusminor = "";
    m_prolatio = "";
    m_sign = "";
    m_tempus = "";
}

/* include <atttempus> */

//----------------------------------------------------------------------------
// AttMeterSigLog
//----------------------------------------------------------------------------

AttMeterSigLog::AttMeterSigLog() {
    ResetMeterSigLog();
}

AttMeterSigLog::~AttMeterSigLog() {

}

void AttMeterSigLog::ResetMeterSigLog() {
    m_countDbl = 0.0;
    m_sym = "";
    m_unitDbl = 0.0;
}

/* include <attunit> */

//----------------------------------------------------------------------------
// AttMeterSigVis
//----------------------------------------------------------------------------

AttMeterSigVis::AttMeterSigVis() {
    ResetMeterSigVis();
}

AttMeterSigVis::~AttMeterSigVis() {

}

void AttMeterSigVis::ResetMeterSigVis() {
    m_rend = "";
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttMeterSigDefaultLog
//----------------------------------------------------------------------------

AttMeterSigDefaultLog::AttMeterSigDefaultLog() {
    ResetMeterSigDefaultLog();
}

AttMeterSigDefaultLog::~AttMeterSigDefaultLog() {

}

void AttMeterSigDefaultLog::ResetMeterSigDefaultLog() {
    m_meterCountDbl = 0.0;
    m_meterUnitDbl = 0.0;
}

/* include <attmeter.unit> */

//----------------------------------------------------------------------------
// AttMeterSigDefaultVis
//----------------------------------------------------------------------------

AttMeterSigDefaultVis::AttMeterSigDefaultVis() {
    ResetMeterSigDefaultVis();
}

AttMeterSigDefaultVis::~AttMeterSigDefaultVis() {

}

void AttMeterSigDefaultVis::ResetMeterSigDefaultVis() {
    m_meterRend = "";
    m_meterShowchange = "";
    m_meterSym = "";
}

/* include <attmeter.sym> */

//----------------------------------------------------------------------------
// AttMeterconformance
//----------------------------------------------------------------------------

AttMeterconformance::AttMeterconformance() {
    ResetMeterconformance();
}

AttMeterconformance::~AttMeterconformance() {

}

void AttMeterconformance::ResetMeterconformance() {
    m_metcon = "";
}

/* include <attmetcon> */

//----------------------------------------------------------------------------
// AttMeterconformanceBar
//----------------------------------------------------------------------------

AttMeterconformanceBar::AttMeterconformanceBar() {
    ResetMeterconformanceBar();
}

AttMeterconformanceBar::~AttMeterconformanceBar() {

}

void AttMeterconformanceBar::ResetMeterconformanceBar() {
    m_metcon = "";
    m_control = "";
}

/* include <attcontrol> */

//----------------------------------------------------------------------------
// AttMmtempo
//----------------------------------------------------------------------------

AttMmtempo::AttMmtempo() {
    ResetMmtempo();
}

AttMmtempo::~AttMmtempo() {

}

void AttMmtempo::ResetMmtempo() {
    m_mm = "";
}

/* include <attmm> */

//----------------------------------------------------------------------------
// AttMultinummeasures
//----------------------------------------------------------------------------

AttMultinummeasures::AttMultinummeasures() {
    ResetMultinummeasures();
}

AttMultinummeasures::~AttMultinummeasures() {

}

void AttMultinummeasures::ResetMultinummeasures() {
    m_multiNumber = "";
}

/* include <attmulti.number> */

//----------------------------------------------------------------------------
// AttName
//----------------------------------------------------------------------------

AttName::AttName() {
    ResetName();
}

AttName::~AttName() {

}

void AttName::ResetName() {
    m_nymref = "";
    m_role = "";
}

/* include <attrole> */

//----------------------------------------------------------------------------
// AttNoteGes
//----------------------------------------------------------------------------

AttNoteGes::AttNoteGes() {
    ResetNoteGes();
}

AttNoteGes::~AttNoteGes() {

}

void AttNoteGes::ResetNoteGes() {
    m_octGes = "";
    m_pnameGes = "";
    m_pnum = "";
}

/* include <attpnum> */

//----------------------------------------------------------------------------
// AttNoteVis
//----------------------------------------------------------------------------

AttNoteVis::AttNoteVis() {
    ResetNoteVis();
}

AttNoteVis::~AttNoteVis() {

}

void AttNoteVis::ResetNoteVis() {
    m_headshape = "";
}

/* include <attheadshape> */

//----------------------------------------------------------------------------
// AttOctave
//----------------------------------------------------------------------------

AttOctave::AttOctave() {
    ResetOctave();
}

AttOctave::~AttOctave() {

}

void AttOctave::ResetOctave() {
    m_oct = "";
}

/* include <attoct> */

//----------------------------------------------------------------------------
// AttOctavedefault
//----------------------------------------------------------------------------

AttOctavedefault::AttOctavedefault() {
    ResetOctavedefault();
}

AttOctavedefault::~AttOctavedefault() {

}

void AttOctavedefault::ResetOctavedefault() {
    m_octaveDefault = "";
}

/* include <attoctave.default> */

//----------------------------------------------------------------------------
// AttOctavedisplacement
//----------------------------------------------------------------------------

AttOctavedisplacement::AttOctavedisplacement() {
    ResetOctavedisplacement();
}

AttOctavedisplacement::~AttOctavedisplacement() {

}

void AttOctavedisplacement::ResetOctavedisplacement() {
    m_dis = "";
    m_disPlace = "";
}

/* include <attdis.place> */

//----------------------------------------------------------------------------
// AttOnelinestaff
//----------------------------------------------------------------------------

AttOnelinestaff::AttOnelinestaff() {
    ResetOnelinestaff();
}

AttOnelinestaff::~AttOnelinestaff() {

}

void AttOnelinestaff::ResetOnelinestaff() {
    m_ontheline = "";
}

/* include <attontheline> */

//----------------------------------------------------------------------------
// AttPadLog
//----------------------------------------------------------------------------

AttPadLog::AttPadLog() {
    ResetPadLog();
}

AttPadLog::~AttPadLog() {

}

void AttPadLog::ResetPadLog() {
    m_numInt = 0;
}

/* include <attnum> */

//----------------------------------------------------------------------------
// AttPbVis
//----------------------------------------------------------------------------

AttPbVis::AttPbVis() {
    ResetPbVis();
}

AttPbVis::~AttPbVis() {

}

void AttPbVis::ResetPbVis() {
    m_func = "";
}

/* include <attfunc> */

//----------------------------------------------------------------------------
// AttPitch
//----------------------------------------------------------------------------

AttPitch::AttPitch() {
    ResetPitch();
}

AttPitch::~AttPitch() {

}

void AttPitch::ResetPitch() {
    m_pname = "";
}

/* include <attpname> */

//----------------------------------------------------------------------------
// AttPlacement
//----------------------------------------------------------------------------

AttPlacement::AttPlacement() {
    ResetPlacement();
}

AttPlacement::~AttPlacement() {

}

void AttPlacement::ResetPlacement() {
    m_place = "";
}

/* include <attplace> */

//----------------------------------------------------------------------------
// AttPlist
//----------------------------------------------------------------------------

AttPlist::AttPlist() {
    ResetPlist();
}

AttPlist::~AttPlist() {

}

void AttPlist::ResetPlist() {
    m_plist = "";
    m_evaluate = "";
}

/* include <attevaluate> */

//----------------------------------------------------------------------------
// AttPointing
//----------------------------------------------------------------------------

AttPointing::AttPointing() {
    ResetPointing();
}

AttPointing::~AttPointing() {

}

void AttPointing::ResetPointing() {
    m_actuate = "";
    m_role = "";
    m_show = "";
    m_target = "";
    m_targettype = "";
    m_title = "";
}

/* include <atttitle> */

//----------------------------------------------------------------------------
// AttRelativesize
//----------------------------------------------------------------------------

AttRelativesize::AttRelativesize() {
    ResetRelativesize();
}

AttRelativesize::~AttRelativesize() {

}

void AttRelativesize::ResetRelativesize() {
    m_size = "";
}

/* include <attsize> */

//----------------------------------------------------------------------------
// AttResponsibility
//----------------------------------------------------------------------------

AttResponsibility::AttResponsibility() {
    ResetResponsibility();
}

AttResponsibility::~AttResponsibility() {

}

void AttResponsibility::ResetResponsibility() {
    m_resp = "";
}

/* include <attresp> */

//----------------------------------------------------------------------------
// AttSbVis
//----------------------------------------------------------------------------

AttSbVis::AttSbVis() {
    ResetSbVis();
}

AttSbVis::~AttSbVis() {

}

void AttSbVis::ResetSbVis() {
    m_rend = "";
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttScalable
//----------------------------------------------------------------------------

AttScalable::AttScalable() {
    ResetScalable();
}

AttScalable::~AttScalable() {

}

void AttScalable::ResetScalable() {
    m_scale = "";
}

/* include <attscale> */

//----------------------------------------------------------------------------
// AttScoreDefGes
//----------------------------------------------------------------------------

AttScoreDefGes::AttScoreDefGes() {
    ResetScoreDefGes();
}

AttScoreDefGes::~AttScoreDefGes() {

}

void AttScoreDefGes::ResetScoreDefGes() {
    m_tunePname = "";
    m_tuneHzDbl = 0.0;
    m_tuneTemper = "";
}

/* include <atttune.temper> */

//----------------------------------------------------------------------------
// AttScoreDefVis
//----------------------------------------------------------------------------

AttScoreDefVis::AttScoreDefVis() {
    ResetScoreDefVis();
}

AttScoreDefVis::~AttScoreDefVis() {

}

void AttScoreDefVis::ResetScoreDefVis() {
    m_endingRend = "";
    m_mnumVisible = "";
    m_musicName = "";
    m_musicSizeDbl = 0.0;
    m_optimize = "";
    m_pageHeightDbl = 0.0;
    m_pageWidthDbl = 0.0;
    m_pageUnits = "";
    m_pageTopmarDbl = 0.0;
    m_pageBotmarDbl = 0.0;
    m_pageLeftmarDbl = 0.0;
    m_pageRightmarDbl = 0.0;
    m_pagePanels = "";
    m_pageScale = "";
    m_spacingPackexpDbl = 0.0;
    m_spacingPackfactDbl = 0.0;
    m_spacingStaffDbl = 0.0;
    m_spacingSystem = "";
    m_systemLeftmarDbl = 0.0;
    m_systemRightmarDbl = 0.0;
    m_systemTopmarDbl = 0.0;
}

/* include <attsystem.topmar> */

//----------------------------------------------------------------------------
// AttSectionVis
//----------------------------------------------------------------------------

AttSectionVis::AttSectionVis() {
    ResetSectionVis();
}

AttSectionVis::~AttSectionVis() {

}

void AttSectionVis::ResetSectionVis() {
    m_restart = "";
}

/* include <attrestart> */

//----------------------------------------------------------------------------
// AttSequence
//----------------------------------------------------------------------------

AttSequence::AttSequence() {
    ResetSequence();
}

AttSequence::~AttSequence() {

}

void AttSequence::ResetSequence() {
    m_seqInt = 0;
}

/* include <attseq> */

//----------------------------------------------------------------------------
// AttSlashcount
//----------------------------------------------------------------------------

AttSlashcount::AttSlashcount() {
    ResetSlashcount();
}

AttSlashcount::~AttSlashcount() {

}

void AttSlashcount::ResetSlashcount() {
    m_slash = "";
}

/* include <attslash> */

//----------------------------------------------------------------------------
// AttSlurpresent
//----------------------------------------------------------------------------

AttSlurpresent::AttSlurpresent() {
    ResetSlurpresent();
}

AttSlurpresent::~AttSlurpresent() {

}

void AttSlurpresent::ResetSlurpresent() {
    m_slur = "";
}

/* include <attslur> */

//----------------------------------------------------------------------------
// AttSpaceVis
//----------------------------------------------------------------------------

AttSpaceVis::AttSpaceVis() {
    ResetSpaceVis();
}

AttSpaceVis::~AttSpaceVis() {

}

void AttSpaceVis::ResetSpaceVis() {
    m_compressable = "";
}

/* include <attcompressable> */

//----------------------------------------------------------------------------
// AttStaffLog
//----------------------------------------------------------------------------

AttStaffLog::AttStaffLog() {
    ResetStaffLog();
}

AttStaffLog::~AttStaffLog() {

}

void AttStaffLog::ResetStaffLog() {
    m_def = "";
}

/* include <attdef> */

//----------------------------------------------------------------------------
// AttStaffDefVis
//----------------------------------------------------------------------------

AttStaffDefVis::AttStaffDefVis() {
    ResetStaffDefVis();
}

AttStaffDefVis::~AttStaffDefVis() {

}

void AttStaffDefVis::ResetStaffDefVis() {
    m_gridShow = "";
    m_layerscheme = "";
    m_linesInt = 0;
    m_linesColor = "";
    m_linesVisible = "";
    m_spacingDbl = 0.0;
}

/* include <attspacing> */

//----------------------------------------------------------------------------
// AttStaffGrpVis
//----------------------------------------------------------------------------

AttStaffGrpVis::AttStaffGrpVis() {
    ResetStaffGrpVis();
}

AttStaffGrpVis::~AttStaffGrpVis() {

}

void AttStaffGrpVis::ResetStaffGrpVis() {
    m_barthru = "";
}

/* include <attbarthru> */

//----------------------------------------------------------------------------
// AttStaffgroupingsym
//----------------------------------------------------------------------------

AttStaffgroupingsym::AttStaffgroupingsym() {
    ResetStaffgroupingsym();
}

AttStaffgroupingsym::~AttStaffgroupingsym() {

}

void AttStaffgroupingsym::ResetStaffgroupingsym() {
    m_symbol = "";
}

/* include <attsymbol> */

//----------------------------------------------------------------------------
// AttStaffident
//----------------------------------------------------------------------------

AttStaffident::AttStaffident() {
    ResetStaffident();
}

AttStaffident::~AttStaffident() {

}

void AttStaffident::ResetStaffident() {
    m_staff = "";
}

/* include <attstaff> */

//----------------------------------------------------------------------------
// AttStaffloc
//----------------------------------------------------------------------------

AttStaffloc::AttStaffloc() {
    ResetStaffloc();
}

AttStaffloc::~AttStaffloc() {

}

void AttStaffloc::ResetStaffloc() {
    m_loc = "";
}

/* include <attloc> */

//----------------------------------------------------------------------------
// AttStartendid
//----------------------------------------------------------------------------

AttStartendid::AttStartendid() {
    ResetStartendid();
}

AttStartendid::~AttStartendid() {

}

void AttStartendid::ResetStartendid() {
    m_endid = "";
}

/* include <attendid> */

//----------------------------------------------------------------------------
// AttStartid
//----------------------------------------------------------------------------

AttStartid::AttStartid() {
    ResetStartid();
}

AttStartid::~AttStartid() {

}

void AttStartid::ResetStartid() {
    m_startid = "";
}

/* include <attstartid> */

//----------------------------------------------------------------------------
// AttStemmed
//----------------------------------------------------------------------------

AttStemmed::AttStemmed() {
    ResetStemmed();
}

AttStemmed::~AttStemmed() {

}

void AttStemmed::ResetStemmed() {
    m_stemDir = "";
    m_stemLenDbl = 0.0;
    m_stemPos = "";
    m_stemXDbl = 0.0;
    m_stemYDbl = 0.0;
}

/* include <attstem.y> */

//----------------------------------------------------------------------------
// AttSylLog
//----------------------------------------------------------------------------

AttSylLog::AttSylLog() {
    ResetSylLog();
}

AttSylLog::~AttSylLog() {

}

void AttSylLog::ResetSylLog() {
    m_con = "";
    m_wordpos = "";
}

/* include <attwordpos> */

//----------------------------------------------------------------------------
// AttSyltext
//----------------------------------------------------------------------------

AttSyltext::AttSyltext() {
    ResetSyltext();
}

AttSyltext::~AttSyltext() {

}

void AttSyltext::ResetSyltext() {
    m_syl = "";
}

/* include <attsyl> */

//----------------------------------------------------------------------------
// AttTextstyle
//----------------------------------------------------------------------------

AttTextstyle::AttTextstyle() {
    ResetTextstyle();
}

AttTextstyle::~AttTextstyle() {

}

void AttTextstyle::ResetTextstyle() {
    m_textFam = "";
    m_textName = "";
    m_textSizeDbl = 0.0;
    m_textStyle = "";
    m_textWeight = "";
}

/* include <atttext.weight> */

//----------------------------------------------------------------------------
// AttTiepresent
//----------------------------------------------------------------------------

AttTiepresent::AttTiepresent() {
    ResetTiepresent();
}

AttTiepresent::~AttTiepresent() {

}

void AttTiepresent::ResetTiepresent() {
    m_tie = "";
}

/* include <atttie> */

//----------------------------------------------------------------------------
// AttTimestampMusical
//----------------------------------------------------------------------------

AttTimestampMusical::AttTimestampMusical() {
    ResetTimestampMusical();
}

AttTimestampMusical::~AttTimestampMusical() {

}

void AttTimestampMusical::ResetTimestampMusical() {
    m_tstamp = "";
}

/* include <atttstamp> */

//----------------------------------------------------------------------------
// AttTimestampPerformed
//----------------------------------------------------------------------------

AttTimestampPerformed::AttTimestampPerformed() {
    ResetTimestampPerformed();
}

AttTimestampPerformed::~AttTimestampPerformed() {

}

void AttTimestampPerformed::ResetTimestampPerformed() {
    m_tstampGesInt = 0;
    m_tstampReal = "";
}

/* include <atttstamp.real> */

//----------------------------------------------------------------------------
// AttTransposition
//----------------------------------------------------------------------------

AttTransposition::AttTransposition() {
    ResetTransposition();
}

AttTransposition::~AttTransposition() {

}

void AttTransposition::ResetTransposition() {
    m_transDiatDbl = 0.0;
    m_transSemiDbl = 0.0;
}

/* include <atttrans.semi> */

//----------------------------------------------------------------------------
// AttTupletpresent
//----------------------------------------------------------------------------

AttTupletpresent::AttTupletpresent() {
    ResetTupletpresent();
}

AttTupletpresent::~AttTupletpresent() {

}

void AttTupletpresent::ResetTupletpresent() {
    m_tuplet = "";
}

/* include <atttuplet> */

//----------------------------------------------------------------------------
// AttTyped
//----------------------------------------------------------------------------

AttTyped::AttTyped() {
    ResetTyped();
}

AttTyped::~AttTyped() {

}

void AttTyped::ResetTyped() {
    m_type = "";
    m_subtype = "";
}

/* include <attsubtype> */

//----------------------------------------------------------------------------
// AttTypography
//----------------------------------------------------------------------------

AttTypography::AttTypography() {
    ResetTypography();
}

AttTypography::~AttTypography() {

}

void AttTypography::ResetTypography() {
    m_fontfam = "";
    m_fontname = "";
    m_fontsizeDbl = 0.0;
    m_fontstyle = "";
    m_fontweight = "";
}

/* include <attfontweight> */

//----------------------------------------------------------------------------
// AttVisibility
//----------------------------------------------------------------------------

AttVisibility::AttVisibility() {
    ResetVisibility();
}

AttVisibility::~AttVisibility() {

}

void AttVisibility::ResetVisibility() {
    m_visible = "";
}

/* include <attvisible> */

//----------------------------------------------------------------------------
// AttVisualoffsetHo
//----------------------------------------------------------------------------

AttVisualoffsetHo::AttVisualoffsetHo() {
    ResetVisualoffsetHo();
}

AttVisualoffsetHo::~AttVisualoffsetHo() {

}

void AttVisualoffsetHo::ResetVisualoffsetHo() {
    m_ho = "";
}

/* include <attho> */

//----------------------------------------------------------------------------
// AttVisualoffsetTo
//----------------------------------------------------------------------------

AttVisualoffsetTo::AttVisualoffsetTo() {
    ResetVisualoffsetTo();
}

AttVisualoffsetTo::~AttVisualoffsetTo() {

}

void AttVisualoffsetTo::ResetVisualoffsetTo() {
    m_to = "";
}

/* include <attto> */

//----------------------------------------------------------------------------
// AttVisualoffsetVo
//----------------------------------------------------------------------------

AttVisualoffsetVo::AttVisualoffsetVo() {
    ResetVisualoffsetVo();
}

AttVisualoffsetVo::~AttVisualoffsetVo() {

}

void AttVisualoffsetVo::ResetVisualoffsetVo() {
    m_vo = "";
}

/* include <attvo> */

//----------------------------------------------------------------------------
// AttVisualoffset2Ho
//----------------------------------------------------------------------------

AttVisualoffset2Ho::AttVisualoffset2Ho() {
    ResetVisualoffset2Ho();
}

AttVisualoffset2Ho::~AttVisualoffset2Ho() {

}

void AttVisualoffset2Ho::ResetVisualoffset2Ho() {
    m_startho = "";
    m_endho = "";
}

/* include <attendho> */

//----------------------------------------------------------------------------
// AttVisualoffset2To
//----------------------------------------------------------------------------

AttVisualoffset2To::AttVisualoffset2To() {
    ResetVisualoffset2To();
}

AttVisualoffset2To::~AttVisualoffset2To() {

}

void AttVisualoffset2To::ResetVisualoffset2To() {
    m_startto = "";
    m_endto = "";
}

/* include <attendto> */

//----------------------------------------------------------------------------
// AttVisualoffset2Vo
//----------------------------------------------------------------------------

AttVisualoffset2Vo::AttVisualoffset2Vo() {
    ResetVisualoffset2Vo();
}

AttVisualoffset2Vo::~AttVisualoffset2Vo() {

}

void AttVisualoffset2Vo::ResetVisualoffset2Vo() {
    m_startvo = "";
    m_endvo = "";
}

/* include <attendvo> */

//----------------------------------------------------------------------------
// AttWidth
//----------------------------------------------------------------------------

AttWidth::AttWidth() {
    ResetWidth();
}

AttWidth::~AttWidth() {

}

void AttWidth::ResetWidth() {
    m_widthDbl = 0.0;
}

/* include <attwidth> */

//----------------------------------------------------------------------------
// AttXy
//----------------------------------------------------------------------------

AttXy::AttXy() {
    ResetXy();
}

AttXy::~AttXy() {

}

void AttXy::ResetXy() {
    m_xDbl = 0.0;
    m_yDbl = 0.0;
}

/* include <atty> */

//----------------------------------------------------------------------------
// AttXy2
//----------------------------------------------------------------------------

AttXy2::AttXy2() {
    ResetXy2();
}

AttXy2::~AttXy2() {

}

void AttXy2::ResetXy2() {
    m_x2Dbl = 0.0;
    m_y2Dbl = 0.0;
}

/* include <atty2> */

} // vrv namespace

