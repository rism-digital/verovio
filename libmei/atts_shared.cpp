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

#include "atts_shared.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttAccidLog
//----------------------------------------------------------------------------

AttAccidLog::AttAccidLog(): Att() {
    ResetAccidLog();
}

AttAccidLog::~AttAccidLog() {

}

void AttAccidLog::ResetAccidLog() {
    m_func = "";
}

bool AttAccidLog::ReadAccidLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToStr(element.attribute("func").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAccidLog::WriteAccidLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetFunc() == "") {
        element.append_attribute("func") = StrToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attfunc> */

//----------------------------------------------------------------------------
// AttAccidental
//----------------------------------------------------------------------------

AttAccidental::AttAccidental(): Att() {
    ResetAccidental();
}

AttAccidental::~AttAccidental() {

}

void AttAccidental::ResetAccidental() {
    m_accid = "";
}

bool AttAccidental::ReadAccidental(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("accid")) {
        this->SetAccid(StrToStr(element.attribute("accid").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAccidental::WriteAccidental(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetAccid() == "") {
        element.append_attribute("accid") = StrToStr(this->GetAccid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attaccid> */

//----------------------------------------------------------------------------
// AttAccidentalPerformed
//----------------------------------------------------------------------------

AttAccidentalPerformed::AttAccidentalPerformed(): Att() {
    ResetAccidentalPerformed();
}

AttAccidentalPerformed::~AttAccidentalPerformed() {

}

void AttAccidentalPerformed::ResetAccidentalPerformed() {
    m_accidGes = "";
}

bool AttAccidentalPerformed::ReadAccidentalPerformed(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("accid.ges")) {
        this->SetAccidGes(StrToStr(element.attribute("accid.ges").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAccidentalPerformed::WriteAccidentalPerformed(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetAccidGes() == "") {
        element.append_attribute("accid.ges") = StrToStr(this->GetAccidGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attaccid.ges> */

//----------------------------------------------------------------------------
// AttAltsym
//----------------------------------------------------------------------------

AttAltsym::AttAltsym(): Att() {
    ResetAltsym();
}

AttAltsym::~AttAltsym() {

}

void AttAltsym::ResetAltsym() {
    m_altsym = "";
}

bool AttAltsym::ReadAltsym(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("altsym")) {
        this->SetAltsym(StrToStr(element.attribute("altsym").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAltsym::WriteAltsym(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetAltsym() == "") {
        element.append_attribute("altsym") = StrToStr(this->GetAltsym()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attaltsym> */

//----------------------------------------------------------------------------
// AttArticulation
//----------------------------------------------------------------------------

AttArticulation::AttArticulation(): Att() {
    ResetArticulation();
}

AttArticulation::~AttArticulation() {

}

void AttArticulation::ResetArticulation() {
    m_artic = "";
}

bool AttArticulation::ReadArticulation(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("artic")) {
        this->SetArtic(StrToStr(element.attribute("artic").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArticulation::WriteArticulation(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetArtic() == "") {
        element.append_attribute("artic") = StrToStr(this->GetArtic()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attartic> */

//----------------------------------------------------------------------------
// AttArticulationPerformed
//----------------------------------------------------------------------------

AttArticulationPerformed::AttArticulationPerformed(): Att() {
    ResetArticulationPerformed();
}

AttArticulationPerformed::~AttArticulationPerformed() {

}

void AttArticulationPerformed::ResetArticulationPerformed() {
    m_articGes = "";
}

bool AttArticulationPerformed::ReadArticulationPerformed(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("artic.ges")) {
        this->SetArticGes(StrToStr(element.attribute("artic.ges").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArticulationPerformed::WriteArticulationPerformed(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetArticGes() == "") {
        element.append_attribute("artic.ges") = StrToStr(this->GetArticGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attartic.ges> */

//----------------------------------------------------------------------------
// AttAugmentdots
//----------------------------------------------------------------------------

AttAugmentdots::AttAugmentdots(): Att() {
    ResetAugmentdots();
}

AttAugmentdots::~AttAugmentdots() {

}

void AttAugmentdots::ResetAugmentdots() {
    m_dots = "";
}

bool AttAugmentdots::ReadAugmentdots(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dots")) {
        this->SetDots(StrToStr(element.attribute("dots").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAugmentdots::WriteAugmentdots(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDots() == "") {
        element.append_attribute("dots") = StrToStr(this->GetDots()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdots> */

//----------------------------------------------------------------------------
// AttAuthorized
//----------------------------------------------------------------------------

AttAuthorized::AttAuthorized(): Att() {
    ResetAuthorized();
}

AttAuthorized::~AttAuthorized() {

}

void AttAuthorized::ResetAuthorized() {
    m_authority = "";
    m_authURI = "";
}

bool AttAuthorized::ReadAuthorized(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("authority")) {
        this->SetAuthority(StrToStr(element.attribute("authority").value()));
        hasAttribute = true;
    }
    if (element.attribute("authURI")) {
        this->SetAuthURI(StrToStr(element.attribute("authURI").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAuthorized::WriteAuthorized(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetAuthority() == "") {
        element.append_attribute("authority") = StrToStr(this->GetAuthority()).c_str();
        wroteAttribute = true;
    }
    if (this->GetAuthURI() == "") {
        element.append_attribute("authURI") = StrToStr(this->GetAuthURI()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attauthURI> */

//----------------------------------------------------------------------------
// AttBarLineLog
//----------------------------------------------------------------------------

AttBarLineLog::AttBarLineLog(): Att() {
    ResetBarLineLog();
}

AttBarLineLog::~AttBarLineLog() {

}

void AttBarLineLog::ResetBarLineLog() {
    m_rend = BARRENDITION_NONE;
}

bool AttBarLineLog::ReadBarLineLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBarRendition(element.attribute("rend").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBarLineLog::WriteBarLineLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetRend() == BARRENDITION_NONE) {
        element.append_attribute("rend") = BarRenditionToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttBarplacement
//----------------------------------------------------------------------------

AttBarplacement::AttBarplacement(): Att() {
    ResetBarplacement();
}

AttBarplacement::~AttBarplacement() {

}

void AttBarplacement::ResetBarplacement() {
    m_barplace = "";
    m_taktplace = "";
}

bool AttBarplacement::ReadBarplacement(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("barplace")) {
        this->SetBarplace(StrToStr(element.attribute("barplace").value()));
        hasAttribute = true;
    }
    if (element.attribute("taktplace")) {
        this->SetTaktplace(StrToStr(element.attribute("taktplace").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBarplacement::WriteBarplacement(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetBarplace() == "") {
        element.append_attribute("barplace") = StrToStr(this->GetBarplace()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTaktplace() == "") {
        element.append_attribute("taktplace") = StrToStr(this->GetTaktplace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttaktplace> */

//----------------------------------------------------------------------------
// AttBeamingVis
//----------------------------------------------------------------------------

AttBeamingVis::AttBeamingVis(): Att() {
    ResetBeamingVis();
}

AttBeamingVis::~AttBeamingVis() {

}

void AttBeamingVis::ResetBeamingVis() {
    m_beamRend = "";
    m_beamSlopeDbl = 0.0;
}

bool AttBeamingVis::ReadBeamingVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("beam.rend")) {
        this->SetBeamRend(StrToStr(element.attribute("beam.rend").value()));
        hasAttribute = true;
    }
    if (element.attribute("beam.slope")) {
        this->SetBeamSlopeDbl(StrToDbl(element.attribute("beam.slope").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamingVis::WriteBeamingVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetBeamRend() == "") {
        element.append_attribute("beam.rend") = StrToStr(this->GetBeamRend()).c_str();
        wroteAttribute = true;
    }
    if (this->GetBeamSlopeDbl() == 0.0) {
        element.append_attribute("beam.slope") = DblToStr(this->GetBeamSlopeDbl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attbeam.slope> */

//----------------------------------------------------------------------------
// AttBibl
//----------------------------------------------------------------------------

AttBibl::AttBibl(): Att() {
    ResetBibl();
}

AttBibl::~AttBibl() {

}

void AttBibl::ResetBibl() {
    m_analog = "";
}

bool AttBibl::ReadBibl(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("analog")) {
        this->SetAnalog(StrToStr(element.attribute("analog").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBibl::WriteBibl(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetAnalog() == "") {
        element.append_attribute("analog") = StrToStr(this->GetAnalog()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attanalog> */

//----------------------------------------------------------------------------
// AttCalendared
//----------------------------------------------------------------------------

AttCalendared::AttCalendared(): Att() {
    ResetCalendared();
}

AttCalendared::~AttCalendared() {

}

void AttCalendared::ResetCalendared() {
    m_calendar = "";
}

bool AttCalendared::ReadCalendared(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("calendar")) {
        this->SetCalendar(StrToStr(element.attribute("calendar").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCalendared::WriteCalendared(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetCalendar() == "") {
        element.append_attribute("calendar") = StrToStr(this->GetCalendar()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attcalendar> */

//----------------------------------------------------------------------------
// AttCanonical
//----------------------------------------------------------------------------

AttCanonical::AttCanonical(): Att() {
    ResetCanonical();
}

AttCanonical::~AttCanonical() {

}

void AttCanonical::ResetCanonical() {
    m_dbkey = "";
}

bool AttCanonical::ReadCanonical(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dbkey")) {
        this->SetDbkey(StrToStr(element.attribute("dbkey").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCanonical::WriteCanonical(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDbkey() == "") {
        element.append_attribute("dbkey") = StrToStr(this->GetDbkey()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdbkey> */

//----------------------------------------------------------------------------
// AttChordVis
//----------------------------------------------------------------------------

AttChordVis::AttChordVis(): Att() {
    ResetChordVis();
}

AttChordVis::~AttChordVis() {

}

void AttChordVis::ResetChordVis() {
    m_cluster = "";
}

bool AttChordVis::ReadChordVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("cluster")) {
        this->SetCluster(StrToStr(element.attribute("cluster").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttChordVis::WriteChordVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetCluster() == "") {
        element.append_attribute("cluster") = StrToStr(this->GetCluster()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attcluster> */

//----------------------------------------------------------------------------
// AttClefLog
//----------------------------------------------------------------------------

AttClefLog::AttClefLog(): Att() {
    ResetClefLog();
}

AttClefLog::~AttClefLog() {

}

void AttClefLog::ResetClefLog() {
    m_cautionary = "";
}

bool AttClefLog::ReadClefLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("cautionary")) {
        this->SetCautionary(StrToStr(element.attribute("cautionary").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttClefLog::WriteClefLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetCautionary() == "") {
        element.append_attribute("cautionary") = StrToStr(this->GetCautionary()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attcautionary> */

//----------------------------------------------------------------------------
// AttCleffingLog
//----------------------------------------------------------------------------

AttCleffingLog::AttCleffingLog(): Att() {
    ResetCleffingLog();
}

AttCleffingLog::~AttCleffingLog() {

}

void AttCleffingLog::ResetCleffingLog() {
    m_clefShape = CLEFSHAPE_NONE;
    m_clefLine = 0;
    m_clefDis = OCTAVE_DIS_NONE;
    m_clefDisPlace = PLACE_NONE;
}

bool AttCleffingLog::ReadCleffingLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("clef.shape")) {
        this->SetClefShape(StrToClefShape(element.attribute("clef.shape").value()));
        hasAttribute = true;
    }
    if (element.attribute("clef.line")) {
        this->SetClefLine(StrToInt(element.attribute("clef.line").value()));
        hasAttribute = true;
    }
    if (element.attribute("clef.dis")) {
        this->SetClefDis(StrToOctaveDis(element.attribute("clef.dis").value()));
        hasAttribute = true;
    }
    if (element.attribute("clef.dis.place")) {
        this->SetClefDisPlace(StrToPlace(element.attribute("clef.dis.place").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCleffingLog::WriteCleffingLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetClefShape() == CLEFSHAPE_NONE) {
        element.append_attribute("clef.shape") = ClefShapeToStr(this->GetClefShape()).c_str();
        wroteAttribute = true;
    }
    if (this->GetClefLine() == 0) {
        element.append_attribute("clef.line") = IntToStr(this->GetClefLine()).c_str();
        wroteAttribute = true;
    }
    if (this->GetClefDis() == OCTAVE_DIS_NONE) {
        element.append_attribute("clef.dis") = OctaveDisToStr(this->GetClefDis()).c_str();
        wroteAttribute = true;
    }
    if (this->GetClefDisPlace() == PLACE_NONE) {
        element.append_attribute("clef.dis.place") = PlaceToStr(this->GetClefDisPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attclef.dis.place> */

//----------------------------------------------------------------------------
// AttCleffingVis
//----------------------------------------------------------------------------

AttCleffingVis::AttCleffingVis(): Att() {
    ResetCleffingVis();
}

AttCleffingVis::~AttCleffingVis() {

}

void AttCleffingVis::ResetCleffingVis() {
    m_clefColor = "";
    m_clefVisible = "";
}

bool AttCleffingVis::ReadCleffingVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("clef.color")) {
        this->SetClefColor(StrToStr(element.attribute("clef.color").value()));
        hasAttribute = true;
    }
    if (element.attribute("clef.visible")) {
        this->SetClefVisible(StrToStr(element.attribute("clef.visible").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCleffingVis::WriteCleffingVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetClefColor() == "") {
        element.append_attribute("clef.color") = StrToStr(this->GetClefColor()).c_str();
        wroteAttribute = true;
    }
    if (this->GetClefVisible() == "") {
        element.append_attribute("clef.visible") = StrToStr(this->GetClefVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attclef.visible> */

//----------------------------------------------------------------------------
// AttClefshape
//----------------------------------------------------------------------------

AttClefshape::AttClefshape(): Att() {
    ResetClefshape();
}

AttClefshape::~AttClefshape() {

}

void AttClefshape::ResetClefshape() {
    m_shape = CLEFSHAPE_NONE;
}

bool AttClefshape::ReadClefshape(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("shape")) {
        this->SetShape(StrToClefShape(element.attribute("shape").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttClefshape::WriteClefshape(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetShape() == CLEFSHAPE_NONE) {
        element.append_attribute("shape") = ClefShapeToStr(this->GetShape()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attshape> */

//----------------------------------------------------------------------------
// AttColor
//----------------------------------------------------------------------------

AttColor::AttColor(): Att() {
    ResetColor();
}

AttColor::~AttColor() {

}

void AttColor::ResetColor() {
    m_color = "";
}

bool AttColor::ReadColor(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("color")) {
        this->SetColor(StrToStr(element.attribute("color").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttColor::WriteColor(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetColor() == "") {
        element.append_attribute("color") = StrToStr(this->GetColor()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attcolor> */

//----------------------------------------------------------------------------
// AttColoration
//----------------------------------------------------------------------------

AttColoration::AttColoration(): Att() {
    ResetColoration();
}

AttColoration::~AttColoration() {

}

void AttColoration::ResetColoration() {
    m_colored = "";
}

bool AttColoration::ReadColoration(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("colored")) {
        this->SetColored(StrToStr(element.attribute("colored").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttColoration::WriteColoration(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetColored() == "") {
        element.append_attribute("colored") = StrToStr(this->GetColored()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attcolored> */

//----------------------------------------------------------------------------
// AttCommon
//----------------------------------------------------------------------------

AttCommon::AttCommon(): Att() {
    ResetCommon();
}

AttCommon::~AttCommon() {

}

void AttCommon::ResetCommon() {
    m_label = "";
    m_n = 0;
    m_base = "";
}

bool AttCommon::ReadCommon(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("label")) {
        this->SetLabel(StrToStr(element.attribute("label").value()));
        hasAttribute = true;
    }
    if (element.attribute("n")) {
        this->SetN(StrToInt(element.attribute("n").value()));
        hasAttribute = true;
    }
    if (element.attribute("base")) {
        this->SetBase(StrToStr(element.attribute("base").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCommon::WriteCommon(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetLabel() == "") {
        element.append_attribute("label") = StrToStr(this->GetLabel()).c_str();
        wroteAttribute = true;
    }
    if (this->GetN() == 0) {
        element.append_attribute("n") = IntToStr(this->GetN()).c_str();
        wroteAttribute = true;
    }
    if (this->GetBase() == "") {
        element.append_attribute("base") = StrToStr(this->GetBase()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attbase> */

//----------------------------------------------------------------------------
// AttCoordinated
//----------------------------------------------------------------------------

AttCoordinated::AttCoordinated(): Att() {
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

bool AttCoordinated::ReadCoordinated(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("ulx")) {
        this->SetUlxInt(StrToInt(element.attribute("ulx").value()));
        hasAttribute = true;
    }
    if (element.attribute("uly")) {
        this->SetUlyInt(StrToInt(element.attribute("uly").value()));
        hasAttribute = true;
    }
    if (element.attribute("lrx")) {
        this->SetLrxInt(StrToInt(element.attribute("lrx").value()));
        hasAttribute = true;
    }
    if (element.attribute("lry")) {
        this->SetLryInt(StrToInt(element.attribute("lry").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCoordinated::WriteCoordinated(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetUlxInt() == 0) {
        element.append_attribute("ulx") = IntToStr(this->GetUlxInt()).c_str();
        wroteAttribute = true;
    }
    if (this->GetUlyInt() == 0) {
        element.append_attribute("uly") = IntToStr(this->GetUlyInt()).c_str();
        wroteAttribute = true;
    }
    if (this->GetLrxInt() == 0) {
        element.append_attribute("lrx") = IntToStr(this->GetLrxInt()).c_str();
        wroteAttribute = true;
    }
    if (this->GetLryInt() == 0) {
        element.append_attribute("lry") = IntToStr(this->GetLryInt()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attlry> */

//----------------------------------------------------------------------------
// AttCurvature
//----------------------------------------------------------------------------

AttCurvature::AttCurvature(): Att() {
    ResetCurvature();
}

AttCurvature::~AttCurvature() {

}

void AttCurvature::ResetCurvature() {
    m_bezier = "";
    m_bulge = "";
    m_curvedir = "";
}

bool AttCurvature::ReadCurvature(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("bezier")) {
        this->SetBezier(StrToStr(element.attribute("bezier").value()));
        hasAttribute = true;
    }
    if (element.attribute("bulge")) {
        this->SetBulge(StrToStr(element.attribute("bulge").value()));
        hasAttribute = true;
    }
    if (element.attribute("curvedir")) {
        this->SetCurvedir(StrToStr(element.attribute("curvedir").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCurvature::WriteCurvature(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetBezier() == "") {
        element.append_attribute("bezier") = StrToStr(this->GetBezier()).c_str();
        wroteAttribute = true;
    }
    if (this->GetBulge() == "") {
        element.append_attribute("bulge") = StrToStr(this->GetBulge()).c_str();
        wroteAttribute = true;
    }
    if (this->GetCurvedir() == "") {
        element.append_attribute("curvedir") = StrToStr(this->GetCurvedir()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attcurvedir> */

//----------------------------------------------------------------------------
// AttCurverend
//----------------------------------------------------------------------------

AttCurverend::AttCurverend(): Att() {
    ResetCurverend();
}

AttCurverend::~AttCurverend() {

}

void AttCurverend::ResetCurverend() {
    m_rend = BARRENDITION_NONE;
}

bool AttCurverend::ReadCurverend(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBarRendition(element.attribute("rend").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCurverend::WriteCurverend(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetRend() == BARRENDITION_NONE) {
        element.append_attribute("rend") = BarRenditionToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttCustosLog
//----------------------------------------------------------------------------

AttCustosLog::AttCustosLog(): Att() {
    ResetCustosLog();
}

AttCustosLog::~AttCustosLog() {

}

void AttCustosLog::ResetCustosLog() {
    m_target = "";
}

bool AttCustosLog::ReadCustosLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("target")) {
        this->SetTarget(StrToStr(element.attribute("target").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCustosLog::WriteCustosLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetTarget() == "") {
        element.append_attribute("target") = StrToStr(this->GetTarget()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttarget> */

//----------------------------------------------------------------------------
// AttDatable
//----------------------------------------------------------------------------

AttDatable::AttDatable(): Att() {
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

bool AttDatable::ReadDatable(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("enddate")) {
        this->SetEnddate(StrToStr(element.attribute("enddate").value()));
        hasAttribute = true;
    }
    if (element.attribute("isodate")) {
        this->SetIsodate(StrToStr(element.attribute("isodate").value()));
        hasAttribute = true;
    }
    if (element.attribute("notafter")) {
        this->SetNotafter(StrToStr(element.attribute("notafter").value()));
        hasAttribute = true;
    }
    if (element.attribute("notbefore")) {
        this->SetNotbefore(StrToStr(element.attribute("notbefore").value()));
        hasAttribute = true;
    }
    if (element.attribute("startdate")) {
        this->SetStartdate(StrToStr(element.attribute("startdate").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDatable::WriteDatable(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetEnddate() == "") {
        element.append_attribute("enddate") = StrToStr(this->GetEnddate()).c_str();
        wroteAttribute = true;
    }
    if (this->GetIsodate() == "") {
        element.append_attribute("isodate") = StrToStr(this->GetIsodate()).c_str();
        wroteAttribute = true;
    }
    if (this->GetNotafter() == "") {
        element.append_attribute("notafter") = StrToStr(this->GetNotafter()).c_str();
        wroteAttribute = true;
    }
    if (this->GetNotbefore() == "") {
        element.append_attribute("notbefore") = StrToStr(this->GetNotbefore()).c_str();
        wroteAttribute = true;
    }
    if (this->GetStartdate() == "") {
        element.append_attribute("startdate") = StrToStr(this->GetStartdate()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attstartdate> */

//----------------------------------------------------------------------------
// AttDatapointing
//----------------------------------------------------------------------------

AttDatapointing::AttDatapointing(): Att() {
    ResetDatapointing();
}

AttDatapointing::~AttDatapointing() {

}

void AttDatapointing::ResetDatapointing() {
    m_data = "";
}

bool AttDatapointing::ReadDatapointing(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("data")) {
        this->SetData(StrToStr(element.attribute("data").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDatapointing::WriteDatapointing(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetData() == "") {
        element.append_attribute("data") = StrToStr(this->GetData()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdata> */

//----------------------------------------------------------------------------
// AttDeclaring
//----------------------------------------------------------------------------

AttDeclaring::AttDeclaring(): Att() {
    ResetDeclaring();
}

AttDeclaring::~AttDeclaring() {

}

void AttDeclaring::ResetDeclaring() {
    m_decls = "";
}

bool AttDeclaring::ReadDeclaring(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("decls")) {
        this->SetDecls(StrToStr(element.attribute("decls").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDeclaring::WriteDeclaring(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDecls() == "") {
        element.append_attribute("decls") = StrToStr(this->GetDecls()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdecls> */

//----------------------------------------------------------------------------
// AttDistances
//----------------------------------------------------------------------------

AttDistances::AttDistances(): Att() {
    ResetDistances();
}

AttDistances::~AttDistances() {

}

void AttDistances::ResetDistances() {
    m_dynamDistDbl = 0.0;
    m_harmDistDbl = 0.0;
    m_textDistDbl = 0.0;
}

bool AttDistances::ReadDistances(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dynam.dist")) {
        this->SetDynamDistDbl(StrToDbl(element.attribute("dynam.dist").value()));
        hasAttribute = true;
    }
    if (element.attribute("harm.dist")) {
        this->SetHarmDistDbl(StrToDbl(element.attribute("harm.dist").value()));
        hasAttribute = true;
    }
    if (element.attribute("text.dist")) {
        this->SetTextDistDbl(StrToDbl(element.attribute("text.dist").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDistances::WriteDistances(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDynamDistDbl() == 0.0) {
        element.append_attribute("dynam.dist") = DblToStr(this->GetDynamDistDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetHarmDistDbl() == 0.0) {
        element.append_attribute("harm.dist") = DblToStr(this->GetHarmDistDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTextDistDbl() == 0.0) {
        element.append_attribute("text.dist") = DblToStr(this->GetTextDistDbl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttext.dist> */

//----------------------------------------------------------------------------
// AttDotLog
//----------------------------------------------------------------------------

AttDotLog::AttDotLog(): Att() {
    ResetDotLog();
}

AttDotLog::~AttDotLog() {

}

void AttDotLog::ResetDotLog() {
    m_form = "";
}

bool AttDotLog::ReadDotLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToStr(element.attribute("form").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDotLog::WriteDotLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetForm() == "") {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttDurationDefault
//----------------------------------------------------------------------------

AttDurationDefault::AttDurationDefault(): Att() {
    ResetDurationDefault();
}

AttDurationDefault::~AttDurationDefault() {

}

void AttDurationDefault::ResetDurationDefault() {
    m_durDefault = "";
}

bool AttDurationDefault::ReadDurationDefault(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dur.default")) {
        this->SetDurDefault(StrToStr(element.attribute("dur.default").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationDefault::WriteDurationDefault(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDurDefault() == "") {
        element.append_attribute("dur.default") = StrToStr(this->GetDurDefault()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdur.default> */

//----------------------------------------------------------------------------
// AttDurationMusical
//----------------------------------------------------------------------------

AttDurationMusical::AttDurationMusical(): Att() {
    ResetDurationMusical();
}

AttDurationMusical::~AttDurationMusical() {

}

void AttDurationMusical::ResetDurationMusical() {
    m_dur = "";
}

bool AttDurationMusical::ReadDurationMusical(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dur")) {
        this->SetDur(StrToStr(element.attribute("dur").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationMusical::WriteDurationMusical(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDur() == "") {
        element.append_attribute("dur") = StrToStr(this->GetDur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdur> */

//----------------------------------------------------------------------------
// AttDurationPerformed
//----------------------------------------------------------------------------

AttDurationPerformed::AttDurationPerformed(): Att() {
    ResetDurationPerformed();
}

AttDurationPerformed::~AttDurationPerformed() {

}

void AttDurationPerformed::ResetDurationPerformed() {
    m_durGes = "";
}

bool AttDurationPerformed::ReadDurationPerformed(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dur.ges")) {
        this->SetDurGes(StrToStr(element.attribute("dur.ges").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationPerformed::WriteDurationPerformed(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDurGes() == "") {
        element.append_attribute("dur.ges") = StrToStr(this->GetDurGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdur.ges> */

//----------------------------------------------------------------------------
// AttDurationRatio
//----------------------------------------------------------------------------

AttDurationRatio::AttDurationRatio(): Att() {
    ResetDurationRatio();
}

AttDurationRatio::~AttDurationRatio() {

}

void AttDurationRatio::ResetDurationRatio() {
    m_numInt = 0;
    m_numbaseInt = 0;
}

bool AttDurationRatio::ReadDurationRatio(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNumInt(StrToInt(element.attribute("num").value()));
        hasAttribute = true;
    }
    if (element.attribute("numbase")) {
        this->SetNumbaseInt(StrToInt(element.attribute("numbase").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationRatio::WriteDurationRatio(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetNumInt() == 0) {
        element.append_attribute("num") = IntToStr(this->GetNumInt()).c_str();
        wroteAttribute = true;
    }
    if (this->GetNumbaseInt() == 0) {
        element.append_attribute("numbase") = IntToStr(this->GetNumbaseInt()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attnumbase> */

//----------------------------------------------------------------------------
// AttDurationTimestamp
//----------------------------------------------------------------------------

AttDurationTimestamp::AttDurationTimestamp(): Att() {
    ResetDurationTimestamp();
}

AttDurationTimestamp::~AttDurationTimestamp() {

}

void AttDurationTimestamp::ResetDurationTimestamp() {
    m_dur = "";
}

bool AttDurationTimestamp::ReadDurationTimestamp(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dur")) {
        this->SetDur(StrToStr(element.attribute("dur").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationTimestamp::WriteDurationTimestamp(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDur() == "") {
        element.append_attribute("dur") = StrToStr(this->GetDur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdur> */

//----------------------------------------------------------------------------
// AttEnclosingchars
//----------------------------------------------------------------------------

AttEnclosingchars::AttEnclosingchars(): Att() {
    ResetEnclosingchars();
}

AttEnclosingchars::~AttEnclosingchars() {

}

void AttEnclosingchars::ResetEnclosingchars() {
    m_enclose = "";
}

bool AttEnclosingchars::ReadEnclosingchars(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("enclose")) {
        this->SetEnclose(StrToStr(element.attribute("enclose").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttEnclosingchars::WriteEnclosingchars(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetEnclose() == "") {
        element.append_attribute("enclose") = StrToStr(this->GetEnclose()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attenclose> */

//----------------------------------------------------------------------------
// AttFermatapresent
//----------------------------------------------------------------------------

AttFermatapresent::AttFermatapresent(): Att() {
    ResetFermatapresent();
}

AttFermatapresent::~AttFermatapresent() {

}

void AttFermatapresent::ResetFermatapresent() {
    m_fermata = "";
}

bool AttFermatapresent::ReadFermatapresent(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("fermata")) {
        this->SetFermata(StrToStr(element.attribute("fermata").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFermatapresent::WriteFermatapresent(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetFermata() == "") {
        element.append_attribute("fermata") = StrToStr(this->GetFermata()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attfermata> */

//----------------------------------------------------------------------------
// AttHandident
//----------------------------------------------------------------------------

AttHandident::AttHandident(): Att() {
    ResetHandident();
}

AttHandident::~AttHandident() {

}

void AttHandident::ResetHandident() {
    m_hand = "";
}

bool AttHandident::ReadHandident(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("hand")) {
        this->SetHand(StrToStr(element.attribute("hand").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHandident::WriteHandident(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetHand() == "") {
        element.append_attribute("hand") = StrToStr(this->GetHand()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atthand> */

//----------------------------------------------------------------------------
// AttHorizontalalign
//----------------------------------------------------------------------------

AttHorizontalalign::AttHorizontalalign(): Att() {
    ResetHorizontalalign();
}

AttHorizontalalign::~AttHorizontalalign() {

}

void AttHorizontalalign::ResetHorizontalalign() {
    m_halign = "";
}

bool AttHorizontalalign::ReadHorizontalalign(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("halign")) {
        this->SetHalign(StrToStr(element.attribute("halign").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHorizontalalign::WriteHorizontalalign(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetHalign() == "") {
        element.append_attribute("halign") = StrToStr(this->GetHalign()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atthalign> */

//----------------------------------------------------------------------------
// AttInstrumentident
//----------------------------------------------------------------------------

AttInstrumentident::AttInstrumentident(): Att() {
    ResetInstrumentident();
}

AttInstrumentident::~AttInstrumentident() {

}

void AttInstrumentident::ResetInstrumentident() {
    m_instr = "";
}

bool AttInstrumentident::ReadInstrumentident(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("instr")) {
        this->SetInstr(StrToStr(element.attribute("instr").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttInstrumentident::WriteInstrumentident(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetInstr() == "") {
        element.append_attribute("instr") = StrToStr(this->GetInstr()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attinstr> */

//----------------------------------------------------------------------------
// AttInternetmedia
//----------------------------------------------------------------------------

AttInternetmedia::AttInternetmedia(): Att() {
    ResetInternetmedia();
}

AttInternetmedia::~AttInternetmedia() {

}

void AttInternetmedia::ResetInternetmedia() {
    m_mimetype = "";
}

bool AttInternetmedia::ReadInternetmedia(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("mimetype")) {
        this->SetMimetype(StrToStr(element.attribute("mimetype").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttInternetmedia::WriteInternetmedia(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMimetype() == "") {
        element.append_attribute("mimetype") = StrToStr(this->GetMimetype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmimetype> */

//----------------------------------------------------------------------------
// AttJoined
//----------------------------------------------------------------------------

AttJoined::AttJoined(): Att() {
    ResetJoined();
}

AttJoined::~AttJoined() {

}

void AttJoined::ResetJoined() {
    m_join = "";
}

bool AttJoined::ReadJoined(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("join")) {
        this->SetJoin(StrToStr(element.attribute("join").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttJoined::WriteJoined(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetJoin() == "") {
        element.append_attribute("join") = StrToStr(this->GetJoin()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attjoin> */

//----------------------------------------------------------------------------
// AttKeySigLog
//----------------------------------------------------------------------------

AttKeySigLog::AttKeySigLog(): Att() {
    ResetKeySigLog();
}

AttKeySigLog::~AttKeySigLog() {

}

void AttKeySigLog::ResetKeySigLog() {
    m_mode = "";
}

bool AttKeySigLog::ReadKeySigLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("mode")) {
        this->SetMode(StrToStr(element.attribute("mode").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigLog::WriteKeySigLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMode() == "") {
        element.append_attribute("mode") = StrToStr(this->GetMode()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmode> */

//----------------------------------------------------------------------------
// AttKeySigDefaultLog
//----------------------------------------------------------------------------

AttKeySigDefaultLog::AttKeySigDefaultLog(): Att() {
    ResetKeySigDefaultLog();
}

AttKeySigDefaultLog::~AttKeySigDefaultLog() {

}

void AttKeySigDefaultLog::ResetKeySigDefaultLog() {
    m_keyAccid = ACCIDENTAL_IMPLICIT_NONE;
    m_keyMode = MODE_NONE;
    m_keyPname = PITCHNAME_NONE;
    m_keySig = KEYSIGNATURE_NONE;
    m_keySigMixed = "";
}

bool AttKeySigDefaultLog::ReadKeySigDefaultLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("key.accid")) {
        this->SetKeyAccid(StrToAccidentalImplicit(element.attribute("key.accid").value()));
        hasAttribute = true;
    }
    if (element.attribute("key.mode")) {
        this->SetKeyMode(StrToMode(element.attribute("key.mode").value()));
        hasAttribute = true;
    }
    if (element.attribute("key.pname")) {
        this->SetKeyPname(StrToPitchName(element.attribute("key.pname").value()));
        hasAttribute = true;
    }
    if (element.attribute("key.sig")) {
        this->SetKeySig(StrToKeySignature(element.attribute("key.sig").value()));
        hasAttribute = true;
    }
    if (element.attribute("key.sig.mixed")) {
        this->SetKeySigMixed(StrToStr(element.attribute("key.sig.mixed").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigDefaultLog::WriteKeySigDefaultLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetKeyAccid() == ACCIDENTAL_IMPLICIT_NONE) {
        element.append_attribute("key.accid") = AccidentalImplicitToStr(this->GetKeyAccid()).c_str();
        wroteAttribute = true;
    }
    if (this->GetKeyMode() == MODE_NONE) {
        element.append_attribute("key.mode") = ModeToStr(this->GetKeyMode()).c_str();
        wroteAttribute = true;
    }
    if (this->GetKeyPname() == PITCHNAME_NONE) {
        element.append_attribute("key.pname") = PitchNameToStr(this->GetKeyPname()).c_str();
        wroteAttribute = true;
    }
    if (this->GetKeySig() == KEYSIGNATURE_NONE) {
        element.append_attribute("key.sig") = KeySignatureToStr(this->GetKeySig()).c_str();
        wroteAttribute = true;
    }
    if (this->GetKeySigMixed() == "") {
        element.append_attribute("key.sig.mixed") = StrToStr(this->GetKeySigMixed()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attkey.sig.mixed> */

//----------------------------------------------------------------------------
// AttKeySigDefaultVis
//----------------------------------------------------------------------------

AttKeySigDefaultVis::AttKeySigDefaultVis(): Att() {
    ResetKeySigDefaultVis();
}

AttKeySigDefaultVis::~AttKeySigDefaultVis() {

}

void AttKeySigDefaultVis::ResetKeySigDefaultVis() {
    m_keySigShow = "";
    m_keySigShowchange = "";
}

bool AttKeySigDefaultVis::ReadKeySigDefaultVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("key.sig.show")) {
        this->SetKeySigShow(StrToStr(element.attribute("key.sig.show").value()));
        hasAttribute = true;
    }
    if (element.attribute("key.sig.showchange")) {
        this->SetKeySigShowchange(StrToStr(element.attribute("key.sig.showchange").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigDefaultVis::WriteKeySigDefaultVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetKeySigShow() == "") {
        element.append_attribute("key.sig.show") = StrToStr(this->GetKeySigShow()).c_str();
        wroteAttribute = true;
    }
    if (this->GetKeySigShowchange() == "") {
        element.append_attribute("key.sig.showchange") = StrToStr(this->GetKeySigShowchange()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attkey.sig.showchange> */

//----------------------------------------------------------------------------
// AttLabelsAddl
//----------------------------------------------------------------------------

AttLabelsAddl::AttLabelsAddl(): Att() {
    ResetLabelsAddl();
}

AttLabelsAddl::~AttLabelsAddl() {

}

void AttLabelsAddl::ResetLabelsAddl() {
    m_labelAbbr = "";
}

bool AttLabelsAddl::ReadLabelsAddl(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("label.abbr")) {
        this->SetLabelAbbr(StrToStr(element.attribute("label.abbr").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLabelsAddl::WriteLabelsAddl(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetLabelAbbr() == "") {
        element.append_attribute("label.abbr") = StrToStr(this->GetLabelAbbr()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attlabel.abbr> */

//----------------------------------------------------------------------------
// AttLang
//----------------------------------------------------------------------------

AttLang::AttLang(): Att() {
    ResetLang();
}

AttLang::~AttLang() {

}

void AttLang::ResetLang() {
    m_lang = "";
}

bool AttLang::ReadLang(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("lang")) {
        this->SetLang(StrToStr(element.attribute("lang").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLang::WriteLang(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetLang() == "") {
        element.append_attribute("lang") = StrToStr(this->GetLang()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attlang> */

//----------------------------------------------------------------------------
// AttLayerLog
//----------------------------------------------------------------------------

AttLayerLog::AttLayerLog(): Att() {
    ResetLayerLog();
}

AttLayerLog::~AttLayerLog() {

}

void AttLayerLog::ResetLayerLog() {
    m_def = "";
}

bool AttLayerLog::ReadLayerLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("def")) {
        this->SetDef(StrToStr(element.attribute("def").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLayerLog::WriteLayerLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDef() == "") {
        element.append_attribute("def") = StrToStr(this->GetDef()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdef> */

//----------------------------------------------------------------------------
// AttLayerident
//----------------------------------------------------------------------------

AttLayerident::AttLayerident(): Att() {
    ResetLayerident();
}

AttLayerident::~AttLayerident() {

}

void AttLayerident::ResetLayerident() {
    m_layer = "";
}

bool AttLayerident::ReadLayerident(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("layer")) {
        this->SetLayer(StrToStr(element.attribute("layer").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLayerident::WriteLayerident(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetLayer() == "") {
        element.append_attribute("layer") = StrToStr(this->GetLayer()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attlayer> */

//----------------------------------------------------------------------------
// AttLineloc
//----------------------------------------------------------------------------

AttLineloc::AttLineloc(): Att() {
    ResetLineloc();
}

AttLineloc::~AttLineloc() {

}

void AttLineloc::ResetLineloc() {
    m_line = 0;
}

bool AttLineloc::ReadLineloc(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("line")) {
        this->SetLine(StrToInt(element.attribute("line").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLineloc::WriteLineloc(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetLine() == 0) {
        element.append_attribute("line") = IntToStr(this->GetLine()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attline> */

//----------------------------------------------------------------------------
// AttLinerend
//----------------------------------------------------------------------------

AttLinerend::AttLinerend(): Att() {
    ResetLinerend();
}

AttLinerend::~AttLinerend() {

}

void AttLinerend::ResetLinerend() {
    m_rend = BARRENDITION_NONE;
}

bool AttLinerend::ReadLinerend(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBarRendition(element.attribute("rend").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLinerend::WriteLinerend(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetRend() == BARRENDITION_NONE) {
        element.append_attribute("rend") = BarRenditionToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttLyricstyle
//----------------------------------------------------------------------------

AttLyricstyle::AttLyricstyle(): Att() {
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

bool AttLyricstyle::ReadLyricstyle(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("lyric.align")) {
        this->SetLyricAlignDbl(StrToDbl(element.attribute("lyric.align").value()));
        hasAttribute = true;
    }
    if (element.attribute("lyric.fam")) {
        this->SetLyricFam(StrToStr(element.attribute("lyric.fam").value()));
        hasAttribute = true;
    }
    if (element.attribute("lyric.name")) {
        this->SetLyricName(StrToStr(element.attribute("lyric.name").value()));
        hasAttribute = true;
    }
    if (element.attribute("lyric.size")) {
        this->SetLyricSizeDbl(StrToDbl(element.attribute("lyric.size").value()));
        hasAttribute = true;
    }
    if (element.attribute("lyric.style")) {
        this->SetLyricStyle(StrToStr(element.attribute("lyric.style").value()));
        hasAttribute = true;
    }
    if (element.attribute("lyric.weight")) {
        this->SetLyricWeight(StrToStr(element.attribute("lyric.weight").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLyricstyle::WriteLyricstyle(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetLyricAlignDbl() == 0.0) {
        element.append_attribute("lyric.align") = DblToStr(this->GetLyricAlignDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetLyricFam() == "") {
        element.append_attribute("lyric.fam") = StrToStr(this->GetLyricFam()).c_str();
        wroteAttribute = true;
    }
    if (this->GetLyricName() == "") {
        element.append_attribute("lyric.name") = StrToStr(this->GetLyricName()).c_str();
        wroteAttribute = true;
    }
    if (this->GetLyricSizeDbl() == 0.0) {
        element.append_attribute("lyric.size") = DblToStr(this->GetLyricSizeDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetLyricStyle() == "") {
        element.append_attribute("lyric.style") = StrToStr(this->GetLyricStyle()).c_str();
        wroteAttribute = true;
    }
    if (this->GetLyricWeight() == "") {
        element.append_attribute("lyric.weight") = StrToStr(this->GetLyricWeight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attlyric.weight> */

//----------------------------------------------------------------------------
// AttMeasureLog
//----------------------------------------------------------------------------

AttMeasureLog::AttMeasureLog(): Att() {
    ResetMeasureLog();
}

AttMeasureLog::~AttMeasureLog() {

}

void AttMeasureLog::ResetMeasureLog() {
    m_left = BARRENDITION_NONE;
    m_right = BARRENDITION_single;
}

bool AttMeasureLog::ReadMeasureLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("left")) {
        this->SetLeft(StrToBarRendition(element.attribute("left").value()));
        hasAttribute = true;
    }
    if (element.attribute("right")) {
        this->SetRight(StrToBarRendition(element.attribute("right").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeasureLog::WriteMeasureLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetLeft() == BARRENDITION_NONE) {
        element.append_attribute("left") = BarRenditionToStr(this->GetLeft()).c_str();
        wroteAttribute = true;
    }
    if (this->GetRight() == BARRENDITION_single) {
        element.append_attribute("right") = BarRenditionToStr(this->GetRight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attright> */

//----------------------------------------------------------------------------
// AttMeasurement
//----------------------------------------------------------------------------

AttMeasurement::AttMeasurement(): Att() {
    ResetMeasurement();
}

AttMeasurement::~AttMeasurement() {

}

void AttMeasurement::ResetMeasurement() {
    m_unit = 0;
}

bool AttMeasurement::ReadMeasurement(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("unit")) {
        this->SetUnit(StrToInt(element.attribute("unit").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeasurement::WriteMeasurement(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetUnit() == 0) {
        element.append_attribute("unit") = IntToStr(this->GetUnit()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attunit> */

//----------------------------------------------------------------------------
// AttMediabounds
//----------------------------------------------------------------------------

AttMediabounds::AttMediabounds(): Att() {
    ResetMediabounds();
}

AttMediabounds::~AttMediabounds() {

}

void AttMediabounds::ResetMediabounds() {
    m_begin = "";
    m_end = "";
    m_betype = "";
}

bool AttMediabounds::ReadMediabounds(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("begin")) {
        this->SetBegin(StrToStr(element.attribute("begin").value()));
        hasAttribute = true;
    }
    if (element.attribute("end")) {
        this->SetEnd(StrToStr(element.attribute("end").value()));
        hasAttribute = true;
    }
    if (element.attribute("betype")) {
        this->SetBetype(StrToStr(element.attribute("betype").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMediabounds::WriteMediabounds(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetBegin() == "") {
        element.append_attribute("begin") = StrToStr(this->GetBegin()).c_str();
        wroteAttribute = true;
    }
    if (this->GetEnd() == "") {
        element.append_attribute("end") = StrToStr(this->GetEnd()).c_str();
        wroteAttribute = true;
    }
    if (this->GetBetype() == "") {
        element.append_attribute("betype") = StrToStr(this->GetBetype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attbetype> */

//----------------------------------------------------------------------------
// AttMedium
//----------------------------------------------------------------------------

AttMedium::AttMedium(): Att() {
    ResetMedium();
}

AttMedium::~AttMedium() {

}

void AttMedium::ResetMedium() {
    m_medium = "";
}

bool AttMedium::ReadMedium(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("medium")) {
        this->SetMedium(StrToStr(element.attribute("medium").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMedium::WriteMedium(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMedium() == "") {
        element.append_attribute("medium") = StrToStr(this->GetMedium()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmedium> */

//----------------------------------------------------------------------------
// AttMeiversion
//----------------------------------------------------------------------------

AttMeiversion::AttMeiversion(): Att() {
    ResetMeiversion();
}

AttMeiversion::~AttMeiversion() {

}

void AttMeiversion::ResetMeiversion() {
    m_meiversion = "";
}

bool AttMeiversion::ReadMeiversion(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("meiversion")) {
        this->SetMeiversion(StrToStr(element.attribute("meiversion").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeiversion::WriteMeiversion(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMeiversion() == "") {
        element.append_attribute("meiversion") = StrToStr(this->GetMeiversion()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmeiversion> */

//----------------------------------------------------------------------------
// AttMensurLog
//----------------------------------------------------------------------------

AttMensurLog::AttMensurLog(): Att() {
    ResetMensurLog();
}

AttMensurLog::~AttMensurLog() {

}

void AttMensurLog::ResetMensurLog() {
    m_dot = BOOLEAN_NONE;
    m_modusmaior = "";
    m_modusminor = "";
    m_prolatio = "";
    m_sign = MENSURATIONSIGN_NONE;
    m_tempus = "";
}

bool AttMensurLog::ReadMensurLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dot")) {
        this->SetDot(StrToBool(element.attribute("dot").value()));
        hasAttribute = true;
    }
    if (element.attribute("modusmaior")) {
        this->SetModusmaior(StrToStr(element.attribute("modusmaior").value()));
        hasAttribute = true;
    }
    if (element.attribute("modusminor")) {
        this->SetModusminor(StrToStr(element.attribute("modusminor").value()));
        hasAttribute = true;
    }
    if (element.attribute("prolatio")) {
        this->SetProlatio(StrToStr(element.attribute("prolatio").value()));
        hasAttribute = true;
    }
    if (element.attribute("sign")) {
        this->SetSign(StrToMensurationSign(element.attribute("sign").value()));
        hasAttribute = true;
    }
    if (element.attribute("tempus")) {
        this->SetTempus(StrToStr(element.attribute("tempus").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensurLog::WriteMensurLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDot() == BOOLEAN_NONE) {
        element.append_attribute("dot") = BoolToStr(this->GetDot()).c_str();
        wroteAttribute = true;
    }
    if (this->GetModusmaior() == "") {
        element.append_attribute("modusmaior") = StrToStr(this->GetModusmaior()).c_str();
        wroteAttribute = true;
    }
    if (this->GetModusminor() == "") {
        element.append_attribute("modusminor") = StrToStr(this->GetModusminor()).c_str();
        wroteAttribute = true;
    }
    if (this->GetProlatio() == "") {
        element.append_attribute("prolatio") = StrToStr(this->GetProlatio()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSign() == MENSURATIONSIGN_NONE) {
        element.append_attribute("sign") = MensurationSignToStr(this->GetSign()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTempus() == "") {
        element.append_attribute("tempus") = StrToStr(this->GetTempus()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttempus> */

//----------------------------------------------------------------------------
// AttMeterSigLog
//----------------------------------------------------------------------------

AttMeterSigLog::AttMeterSigLog(): Att() {
    ResetMeterSigLog();
}

AttMeterSigLog::~AttMeterSigLog() {

}

void AttMeterSigLog::ResetMeterSigLog() {
    m_count = 0;
    m_sym = METERSIGN_NONE;
    m_unit = 0;
}

bool AttMeterSigLog::ReadMeterSigLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("count")) {
        this->SetCount(StrToInt(element.attribute("count").value()));
        hasAttribute = true;
    }
    if (element.attribute("sym")) {
        this->SetSym(StrToMeterSign(element.attribute("sym").value()));
        hasAttribute = true;
    }
    if (element.attribute("unit")) {
        this->SetUnit(StrToInt(element.attribute("unit").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigLog::WriteMeterSigLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetCount() == 0) {
        element.append_attribute("count") = IntToStr(this->GetCount()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSym() == METERSIGN_NONE) {
        element.append_attribute("sym") = MeterSignToStr(this->GetSym()).c_str();
        wroteAttribute = true;
    }
    if (this->GetUnit() == 0) {
        element.append_attribute("unit") = IntToStr(this->GetUnit()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attunit> */

//----------------------------------------------------------------------------
// AttMeterSigVis
//----------------------------------------------------------------------------

AttMeterSigVis::AttMeterSigVis(): Att() {
    ResetMeterSigVis();
}

AttMeterSigVis::~AttMeterSigVis() {

}

void AttMeterSigVis::ResetMeterSigVis() {
    m_rend = BARRENDITION_NONE;
}

bool AttMeterSigVis::ReadMeterSigVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBarRendition(element.attribute("rend").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigVis::WriteMeterSigVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetRend() == BARRENDITION_NONE) {
        element.append_attribute("rend") = BarRenditionToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttMeterSigDefaultLog
//----------------------------------------------------------------------------

AttMeterSigDefaultLog::AttMeterSigDefaultLog(): Att() {
    ResetMeterSigDefaultLog();
}

AttMeterSigDefaultLog::~AttMeterSigDefaultLog() {

}

void AttMeterSigDefaultLog::ResetMeterSigDefaultLog() {
    m_meterCount = 0;
    m_meterUnit = 0;
}

bool AttMeterSigDefaultLog::ReadMeterSigDefaultLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("meter.count")) {
        this->SetMeterCount(StrToInt(element.attribute("meter.count").value()));
        hasAttribute = true;
    }
    if (element.attribute("meter.unit")) {
        this->SetMeterUnit(StrToInt(element.attribute("meter.unit").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigDefaultLog::WriteMeterSigDefaultLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMeterCount() == 0) {
        element.append_attribute("meter.count") = IntToStr(this->GetMeterCount()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMeterUnit() == 0) {
        element.append_attribute("meter.unit") = IntToStr(this->GetMeterUnit()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmeter.unit> */

//----------------------------------------------------------------------------
// AttMeterSigDefaultVis
//----------------------------------------------------------------------------

AttMeterSigDefaultVis::AttMeterSigDefaultVis(): Att() {
    ResetMeterSigDefaultVis();
}

AttMeterSigDefaultVis::~AttMeterSigDefaultVis() {

}

void AttMeterSigDefaultVis::ResetMeterSigDefaultVis() {
    m_meterRend = "";
    m_meterShowchange = "";
    m_meterSym = METERSIGN_NONE;
}

bool AttMeterSigDefaultVis::ReadMeterSigDefaultVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("meter.rend")) {
        this->SetMeterRend(StrToStr(element.attribute("meter.rend").value()));
        hasAttribute = true;
    }
    if (element.attribute("meter.showchange")) {
        this->SetMeterShowchange(StrToStr(element.attribute("meter.showchange").value()));
        hasAttribute = true;
    }
    if (element.attribute("meter.sym")) {
        this->SetMeterSym(StrToMeterSign(element.attribute("meter.sym").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigDefaultVis::WriteMeterSigDefaultVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMeterRend() == "") {
        element.append_attribute("meter.rend") = StrToStr(this->GetMeterRend()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMeterShowchange() == "") {
        element.append_attribute("meter.showchange") = StrToStr(this->GetMeterShowchange()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMeterSym() == METERSIGN_NONE) {
        element.append_attribute("meter.sym") = MeterSignToStr(this->GetMeterSym()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmeter.sym> */

//----------------------------------------------------------------------------
// AttMeterconformance
//----------------------------------------------------------------------------

AttMeterconformance::AttMeterconformance(): Att() {
    ResetMeterconformance();
}

AttMeterconformance::~AttMeterconformance() {

}

void AttMeterconformance::ResetMeterconformance() {
    m_metcon = "";
}

bool AttMeterconformance::ReadMeterconformance(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("metcon")) {
        this->SetMetcon(StrToStr(element.attribute("metcon").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterconformance::WriteMeterconformance(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMetcon() == "") {
        element.append_attribute("metcon") = StrToStr(this->GetMetcon()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmetcon> */

//----------------------------------------------------------------------------
// AttMeterconformanceBar
//----------------------------------------------------------------------------

AttMeterconformanceBar::AttMeterconformanceBar(): Att() {
    ResetMeterconformanceBar();
}

AttMeterconformanceBar::~AttMeterconformanceBar() {

}

void AttMeterconformanceBar::ResetMeterconformanceBar() {
    m_metcon = "";
    m_control = "";
}

bool AttMeterconformanceBar::ReadMeterconformanceBar(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("metcon")) {
        this->SetMetcon(StrToStr(element.attribute("metcon").value()));
        hasAttribute = true;
    }
    if (element.attribute("control")) {
        this->SetControl(StrToStr(element.attribute("control").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterconformanceBar::WriteMeterconformanceBar(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMetcon() == "") {
        element.append_attribute("metcon") = StrToStr(this->GetMetcon()).c_str();
        wroteAttribute = true;
    }
    if (this->GetControl() == "") {
        element.append_attribute("control") = StrToStr(this->GetControl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attcontrol> */

//----------------------------------------------------------------------------
// AttMmtempo
//----------------------------------------------------------------------------

AttMmtempo::AttMmtempo(): Att() {
    ResetMmtempo();
}

AttMmtempo::~AttMmtempo() {

}

void AttMmtempo::ResetMmtempo() {
    m_mm = "";
}

bool AttMmtempo::ReadMmtempo(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("mm")) {
        this->SetMm(StrToStr(element.attribute("mm").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMmtempo::WriteMmtempo(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMm() == "") {
        element.append_attribute("mm") = StrToStr(this->GetMm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmm> */

//----------------------------------------------------------------------------
// AttMultinummeasures
//----------------------------------------------------------------------------

AttMultinummeasures::AttMultinummeasures(): Att() {
    ResetMultinummeasures();
}

AttMultinummeasures::~AttMultinummeasures() {

}

void AttMultinummeasures::ResetMultinummeasures() {
    m_multiNumber = "";
}

bool AttMultinummeasures::ReadMultinummeasures(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("multi.number")) {
        this->SetMultiNumber(StrToStr(element.attribute("multi.number").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMultinummeasures::WriteMultinummeasures(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetMultiNumber() == "") {
        element.append_attribute("multi.number") = StrToStr(this->GetMultiNumber()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attmulti.number> */

//----------------------------------------------------------------------------
// AttName
//----------------------------------------------------------------------------

AttName::AttName(): Att() {
    ResetName();
}

AttName::~AttName() {

}

void AttName::ResetName() {
    m_nymref = "";
    m_role = "";
}

bool AttName::ReadName(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("nymref")) {
        this->SetNymref(StrToStr(element.attribute("nymref").value()));
        hasAttribute = true;
    }
    if (element.attribute("role")) {
        this->SetRole(StrToStr(element.attribute("role").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttName::WriteName(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetNymref() == "") {
        element.append_attribute("nymref") = StrToStr(this->GetNymref()).c_str();
        wroteAttribute = true;
    }
    if (this->GetRole() == "") {
        element.append_attribute("role") = StrToStr(this->GetRole()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attrole> */

//----------------------------------------------------------------------------
// AttNoteGes
//----------------------------------------------------------------------------

AttNoteGes::AttNoteGes(): Att() {
    ResetNoteGes();
}

AttNoteGes::~AttNoteGes() {

}

void AttNoteGes::ResetNoteGes() {
    m_octGes = "";
    m_pnameGes = "";
    m_pnum = "";
}

bool AttNoteGes::ReadNoteGes(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("oct.ges")) {
        this->SetOctGes(StrToStr(element.attribute("oct.ges").value()));
        hasAttribute = true;
    }
    if (element.attribute("pname.ges")) {
        this->SetPnameGes(StrToStr(element.attribute("pname.ges").value()));
        hasAttribute = true;
    }
    if (element.attribute("pnum")) {
        this->SetPnum(StrToStr(element.attribute("pnum").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteGes::WriteNoteGes(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetOctGes() == "") {
        element.append_attribute("oct.ges") = StrToStr(this->GetOctGes()).c_str();
        wroteAttribute = true;
    }
    if (this->GetPnameGes() == "") {
        element.append_attribute("pname.ges") = StrToStr(this->GetPnameGes()).c_str();
        wroteAttribute = true;
    }
    if (this->GetPnum() == "") {
        element.append_attribute("pnum") = StrToStr(this->GetPnum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attpnum> */

//----------------------------------------------------------------------------
// AttNoteVis
//----------------------------------------------------------------------------

AttNoteVis::AttNoteVis(): Att() {
    ResetNoteVis();
}

AttNoteVis::~AttNoteVis() {

}

void AttNoteVis::ResetNoteVis() {
    m_headshape = "";
}

bool AttNoteVis::ReadNoteVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("headshape")) {
        this->SetHeadshape(StrToStr(element.attribute("headshape").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteVis::WriteNoteVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetHeadshape() == "") {
        element.append_attribute("headshape") = StrToStr(this->GetHeadshape()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attheadshape> */

//----------------------------------------------------------------------------
// AttOctave
//----------------------------------------------------------------------------

AttOctave::AttOctave(): Att() {
    ResetOctave();
}

AttOctave::~AttOctave() {

}

void AttOctave::ResetOctave() {
    m_oct = "";
}

bool AttOctave::ReadOctave(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("oct")) {
        this->SetOct(StrToStr(element.attribute("oct").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctave::WriteOctave(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetOct() == "") {
        element.append_attribute("oct") = StrToStr(this->GetOct()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attoct> */

//----------------------------------------------------------------------------
// AttOctavedefault
//----------------------------------------------------------------------------

AttOctavedefault::AttOctavedefault(): Att() {
    ResetOctavedefault();
}

AttOctavedefault::~AttOctavedefault() {

}

void AttOctavedefault::ResetOctavedefault() {
    m_octaveDefault = "";
}

bool AttOctavedefault::ReadOctavedefault(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("octave.default")) {
        this->SetOctaveDefault(StrToStr(element.attribute("octave.default").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctavedefault::WriteOctavedefault(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetOctaveDefault() == "") {
        element.append_attribute("octave.default") = StrToStr(this->GetOctaveDefault()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attoctave.default> */

//----------------------------------------------------------------------------
// AttOctavedisplacement
//----------------------------------------------------------------------------

AttOctavedisplacement::AttOctavedisplacement(): Att() {
    ResetOctavedisplacement();
}

AttOctavedisplacement::~AttOctavedisplacement() {

}

void AttOctavedisplacement::ResetOctavedisplacement() {
    m_dis = OCTAVE_DIS_NONE;
    m_disPlace = PLACE_NONE;
}

bool AttOctavedisplacement::ReadOctavedisplacement(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dis")) {
        this->SetDis(StrToOctaveDis(element.attribute("dis").value()));
        hasAttribute = true;
    }
    if (element.attribute("dis.place")) {
        this->SetDisPlace(StrToPlace(element.attribute("dis.place").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctavedisplacement::WriteOctavedisplacement(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDis() == OCTAVE_DIS_NONE) {
        element.append_attribute("dis") = OctaveDisToStr(this->GetDis()).c_str();
        wroteAttribute = true;
    }
    if (this->GetDisPlace() == PLACE_NONE) {
        element.append_attribute("dis.place") = PlaceToStr(this->GetDisPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdis.place> */

//----------------------------------------------------------------------------
// AttOnelinestaff
//----------------------------------------------------------------------------

AttOnelinestaff::AttOnelinestaff(): Att() {
    ResetOnelinestaff();
}

AttOnelinestaff::~AttOnelinestaff() {

}

void AttOnelinestaff::ResetOnelinestaff() {
    m_ontheline = "";
}

bool AttOnelinestaff::ReadOnelinestaff(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("ontheline")) {
        this->SetOntheline(StrToStr(element.attribute("ontheline").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOnelinestaff::WriteOnelinestaff(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetOntheline() == "") {
        element.append_attribute("ontheline") = StrToStr(this->GetOntheline()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attontheline> */

//----------------------------------------------------------------------------
// AttPadLog
//----------------------------------------------------------------------------

AttPadLog::AttPadLog(): Att() {
    ResetPadLog();
}

AttPadLog::~AttPadLog() {

}

void AttPadLog::ResetPadLog() {
    m_numInt = 0;
}

bool AttPadLog::ReadPadLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNumInt(StrToInt(element.attribute("num").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPadLog::WritePadLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetNumInt() == 0) {
        element.append_attribute("num") = IntToStr(this->GetNumInt()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attnum> */

//----------------------------------------------------------------------------
// AttPbVis
//----------------------------------------------------------------------------

AttPbVis::AttPbVis(): Att() {
    ResetPbVis();
}

AttPbVis::~AttPbVis() {

}

void AttPbVis::ResetPbVis() {
    m_func = "";
}

bool AttPbVis::ReadPbVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToStr(element.attribute("func").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPbVis::WritePbVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetFunc() == "") {
        element.append_attribute("func") = StrToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attfunc> */

//----------------------------------------------------------------------------
// AttPitch
//----------------------------------------------------------------------------

AttPitch::AttPitch(): Att() {
    ResetPitch();
}

AttPitch::~AttPitch() {

}

void AttPitch::ResetPitch() {
    m_pname = "";
}

bool AttPitch::ReadPitch(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("pname")) {
        this->SetPname(StrToStr(element.attribute("pname").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPitch::WritePitch(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetPname() == "") {
        element.append_attribute("pname") = StrToStr(this->GetPname()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attpname> */

//----------------------------------------------------------------------------
// AttPlacement
//----------------------------------------------------------------------------

AttPlacement::AttPlacement(): Att() {
    ResetPlacement();
}

AttPlacement::~AttPlacement() {

}

void AttPlacement::ResetPlacement() {
    m_place = "";
}

bool AttPlacement::ReadPlacement(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("place")) {
        this->SetPlace(StrToStr(element.attribute("place").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPlacement::WritePlacement(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetPlace() == "") {
        element.append_attribute("place") = StrToStr(this->GetPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attplace> */

//----------------------------------------------------------------------------
// AttPlist
//----------------------------------------------------------------------------

AttPlist::AttPlist(): Att() {
    ResetPlist();
}

AttPlist::~AttPlist() {

}

void AttPlist::ResetPlist() {
    m_plist = "";
    m_evaluate = "";
}

bool AttPlist::ReadPlist(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("plist")) {
        this->SetPlist(StrToStr(element.attribute("plist").value()));
        hasAttribute = true;
    }
    if (element.attribute("evaluate")) {
        this->SetEvaluate(StrToStr(element.attribute("evaluate").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPlist::WritePlist(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetPlist() == "") {
        element.append_attribute("plist") = StrToStr(this->GetPlist()).c_str();
        wroteAttribute = true;
    }
    if (this->GetEvaluate() == "") {
        element.append_attribute("evaluate") = StrToStr(this->GetEvaluate()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attevaluate> */

//----------------------------------------------------------------------------
// AttPointing
//----------------------------------------------------------------------------

AttPointing::AttPointing(): Att() {
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

bool AttPointing::ReadPointing(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("actuate")) {
        this->SetActuate(StrToStr(element.attribute("actuate").value()));
        hasAttribute = true;
    }
    if (element.attribute("role")) {
        this->SetRole(StrToStr(element.attribute("role").value()));
        hasAttribute = true;
    }
    if (element.attribute("show")) {
        this->SetShow(StrToStr(element.attribute("show").value()));
        hasAttribute = true;
    }
    if (element.attribute("target")) {
        this->SetTarget(StrToStr(element.attribute("target").value()));
        hasAttribute = true;
    }
    if (element.attribute("targettype")) {
        this->SetTargettype(StrToStr(element.attribute("targettype").value()));
        hasAttribute = true;
    }
    if (element.attribute("title")) {
        this->SetTitle(StrToStr(element.attribute("title").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPointing::WritePointing(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetActuate() == "") {
        element.append_attribute("actuate") = StrToStr(this->GetActuate()).c_str();
        wroteAttribute = true;
    }
    if (this->GetRole() == "") {
        element.append_attribute("role") = StrToStr(this->GetRole()).c_str();
        wroteAttribute = true;
    }
    if (this->GetShow() == "") {
        element.append_attribute("show") = StrToStr(this->GetShow()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTarget() == "") {
        element.append_attribute("target") = StrToStr(this->GetTarget()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTargettype() == "") {
        element.append_attribute("targettype") = StrToStr(this->GetTargettype()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTitle() == "") {
        element.append_attribute("title") = StrToStr(this->GetTitle()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttitle> */

//----------------------------------------------------------------------------
// AttRelativesize
//----------------------------------------------------------------------------

AttRelativesize::AttRelativesize(): Att() {
    ResetRelativesize();
}

AttRelativesize::~AttRelativesize() {

}

void AttRelativesize::ResetRelativesize() {
    m_size = "";
}

bool AttRelativesize::ReadRelativesize(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("size")) {
        this->SetSize(StrToStr(element.attribute("size").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRelativesize::WriteRelativesize(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetSize() == "") {
        element.append_attribute("size") = StrToStr(this->GetSize()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attsize> */

//----------------------------------------------------------------------------
// AttResponsibility
//----------------------------------------------------------------------------

AttResponsibility::AttResponsibility(): Att() {
    ResetResponsibility();
}

AttResponsibility::~AttResponsibility() {

}

void AttResponsibility::ResetResponsibility() {
    m_resp = "";
}

bool AttResponsibility::ReadResponsibility(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("resp")) {
        this->SetResp(StrToStr(element.attribute("resp").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttResponsibility::WriteResponsibility(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetResp() == "") {
        element.append_attribute("resp") = StrToStr(this->GetResp()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attresp> */

//----------------------------------------------------------------------------
// AttSbVis
//----------------------------------------------------------------------------

AttSbVis::AttSbVis(): Att() {
    ResetSbVis();
}

AttSbVis::~AttSbVis() {

}

void AttSbVis::ResetSbVis() {
    m_rend = BARRENDITION_NONE;
}

bool AttSbVis::ReadSbVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBarRendition(element.attribute("rend").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSbVis::WriteSbVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetRend() == BARRENDITION_NONE) {
        element.append_attribute("rend") = BarRenditionToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttScalable
//----------------------------------------------------------------------------

AttScalable::AttScalable(): Att() {
    ResetScalable();
}

AttScalable::~AttScalable() {

}

void AttScalable::ResetScalable() {
    m_scale = "";
}

bool AttScalable::ReadScalable(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("scale")) {
        this->SetScale(StrToStr(element.attribute("scale").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScalable::WriteScalable(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetScale() == "") {
        element.append_attribute("scale") = StrToStr(this->GetScale()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attscale> */

//----------------------------------------------------------------------------
// AttScoreDefGes
//----------------------------------------------------------------------------

AttScoreDefGes::AttScoreDefGes(): Att() {
    ResetScoreDefGes();
}

AttScoreDefGes::~AttScoreDefGes() {

}

void AttScoreDefGes::ResetScoreDefGes() {
    m_tunePname = "";
    m_tuneHzDbl = 0.0;
    m_tuneTemper = "";
}

bool AttScoreDefGes::ReadScoreDefGes(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("tune.pname")) {
        this->SetTunePname(StrToStr(element.attribute("tune.pname").value()));
        hasAttribute = true;
    }
    if (element.attribute("tune.Hz")) {
        this->SetTuneHzDbl(StrToDbl(element.attribute("tune.Hz").value()));
        hasAttribute = true;
    }
    if (element.attribute("tune.temper")) {
        this->SetTuneTemper(StrToStr(element.attribute("tune.temper").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScoreDefGes::WriteScoreDefGes(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetTunePname() == "") {
        element.append_attribute("tune.pname") = StrToStr(this->GetTunePname()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTuneHzDbl() == 0.0) {
        element.append_attribute("tune.Hz") = DblToStr(this->GetTuneHzDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTuneTemper() == "") {
        element.append_attribute("tune.temper") = StrToStr(this->GetTuneTemper()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttune.temper> */

//----------------------------------------------------------------------------
// AttScoreDefVis
//----------------------------------------------------------------------------

AttScoreDefVis::AttScoreDefVis(): Att() {
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

bool AttScoreDefVis::ReadScoreDefVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("ending.rend")) {
        this->SetEndingRend(StrToStr(element.attribute("ending.rend").value()));
        hasAttribute = true;
    }
    if (element.attribute("mnum.visible")) {
        this->SetMnumVisible(StrToStr(element.attribute("mnum.visible").value()));
        hasAttribute = true;
    }
    if (element.attribute("music.name")) {
        this->SetMusicName(StrToStr(element.attribute("music.name").value()));
        hasAttribute = true;
    }
    if (element.attribute("music.size")) {
        this->SetMusicSizeDbl(StrToDbl(element.attribute("music.size").value()));
        hasAttribute = true;
    }
    if (element.attribute("optimize")) {
        this->SetOptimize(StrToStr(element.attribute("optimize").value()));
        hasAttribute = true;
    }
    if (element.attribute("page.height")) {
        this->SetPageHeightDbl(StrToDbl(element.attribute("page.height").value()));
        hasAttribute = true;
    }
    if (element.attribute("page.width")) {
        this->SetPageWidthDbl(StrToDbl(element.attribute("page.width").value()));
        hasAttribute = true;
    }
    if (element.attribute("page.units")) {
        this->SetPageUnits(StrToStr(element.attribute("page.units").value()));
        hasAttribute = true;
    }
    if (element.attribute("page.topmar")) {
        this->SetPageTopmarDbl(StrToDbl(element.attribute("page.topmar").value()));
        hasAttribute = true;
    }
    if (element.attribute("page.botmar")) {
        this->SetPageBotmarDbl(StrToDbl(element.attribute("page.botmar").value()));
        hasAttribute = true;
    }
    if (element.attribute("page.leftmar")) {
        this->SetPageLeftmarDbl(StrToDbl(element.attribute("page.leftmar").value()));
        hasAttribute = true;
    }
    if (element.attribute("page.rightmar")) {
        this->SetPageRightmarDbl(StrToDbl(element.attribute("page.rightmar").value()));
        hasAttribute = true;
    }
    if (element.attribute("page.panels")) {
        this->SetPagePanels(StrToStr(element.attribute("page.panels").value()));
        hasAttribute = true;
    }
    if (element.attribute("page.scale")) {
        this->SetPageScale(StrToStr(element.attribute("page.scale").value()));
        hasAttribute = true;
    }
    if (element.attribute("spacing.packexp")) {
        this->SetSpacingPackexpDbl(StrToDbl(element.attribute("spacing.packexp").value()));
        hasAttribute = true;
    }
    if (element.attribute("spacing.packfact")) {
        this->SetSpacingPackfactDbl(StrToDbl(element.attribute("spacing.packfact").value()));
        hasAttribute = true;
    }
    if (element.attribute("spacing.staff")) {
        this->SetSpacingStaffDbl(StrToDbl(element.attribute("spacing.staff").value()));
        hasAttribute = true;
    }
    if (element.attribute("spacing.system")) {
        this->SetSpacingSystem(StrToStr(element.attribute("spacing.system").value()));
        hasAttribute = true;
    }
    if (element.attribute("system.leftmar")) {
        this->SetSystemLeftmarDbl(StrToDbl(element.attribute("system.leftmar").value()));
        hasAttribute = true;
    }
    if (element.attribute("system.rightmar")) {
        this->SetSystemRightmarDbl(StrToDbl(element.attribute("system.rightmar").value()));
        hasAttribute = true;
    }
    if (element.attribute("system.topmar")) {
        this->SetSystemTopmarDbl(StrToDbl(element.attribute("system.topmar").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScoreDefVis::WriteScoreDefVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetEndingRend() == "") {
        element.append_attribute("ending.rend") = StrToStr(this->GetEndingRend()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMnumVisible() == "") {
        element.append_attribute("mnum.visible") = StrToStr(this->GetMnumVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMusicName() == "") {
        element.append_attribute("music.name") = StrToStr(this->GetMusicName()).c_str();
        wroteAttribute = true;
    }
    if (this->GetMusicSizeDbl() == 0.0) {
        element.append_attribute("music.size") = DblToStr(this->GetMusicSizeDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetOptimize() == "") {
        element.append_attribute("optimize") = StrToStr(this->GetOptimize()).c_str();
        wroteAttribute = true;
    }
    if (this->GetPageHeightDbl() == 0.0) {
        element.append_attribute("page.height") = DblToStr(this->GetPageHeightDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetPageWidthDbl() == 0.0) {
        element.append_attribute("page.width") = DblToStr(this->GetPageWidthDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetPageUnits() == "") {
        element.append_attribute("page.units") = StrToStr(this->GetPageUnits()).c_str();
        wroteAttribute = true;
    }
    if (this->GetPageTopmarDbl() == 0.0) {
        element.append_attribute("page.topmar") = DblToStr(this->GetPageTopmarDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetPageBotmarDbl() == 0.0) {
        element.append_attribute("page.botmar") = DblToStr(this->GetPageBotmarDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetPageLeftmarDbl() == 0.0) {
        element.append_attribute("page.leftmar") = DblToStr(this->GetPageLeftmarDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetPageRightmarDbl() == 0.0) {
        element.append_attribute("page.rightmar") = DblToStr(this->GetPageRightmarDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetPagePanels() == "") {
        element.append_attribute("page.panels") = StrToStr(this->GetPagePanels()).c_str();
        wroteAttribute = true;
    }
    if (this->GetPageScale() == "") {
        element.append_attribute("page.scale") = StrToStr(this->GetPageScale()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSpacingPackexpDbl() == 0.0) {
        element.append_attribute("spacing.packexp") = DblToStr(this->GetSpacingPackexpDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSpacingPackfactDbl() == 0.0) {
        element.append_attribute("spacing.packfact") = DblToStr(this->GetSpacingPackfactDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSpacingStaffDbl() == 0.0) {
        element.append_attribute("spacing.staff") = DblToStr(this->GetSpacingStaffDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSpacingSystem() == "") {
        element.append_attribute("spacing.system") = StrToStr(this->GetSpacingSystem()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSystemLeftmarDbl() == 0.0) {
        element.append_attribute("system.leftmar") = DblToStr(this->GetSystemLeftmarDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSystemRightmarDbl() == 0.0) {
        element.append_attribute("system.rightmar") = DblToStr(this->GetSystemRightmarDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSystemTopmarDbl() == 0.0) {
        element.append_attribute("system.topmar") = DblToStr(this->GetSystemTopmarDbl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attsystem.topmar> */

//----------------------------------------------------------------------------
// AttSectionVis
//----------------------------------------------------------------------------

AttSectionVis::AttSectionVis(): Att() {
    ResetSectionVis();
}

AttSectionVis::~AttSectionVis() {

}

void AttSectionVis::ResetSectionVis() {
    m_restart = "";
}

bool AttSectionVis::ReadSectionVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("restart")) {
        this->SetRestart(StrToStr(element.attribute("restart").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSectionVis::WriteSectionVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetRestart() == "") {
        element.append_attribute("restart") = StrToStr(this->GetRestart()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attrestart> */

//----------------------------------------------------------------------------
// AttSequence
//----------------------------------------------------------------------------

AttSequence::AttSequence(): Att() {
    ResetSequence();
}

AttSequence::~AttSequence() {

}

void AttSequence::ResetSequence() {
    m_seqInt = 0;
}

bool AttSequence::ReadSequence(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("seq")) {
        this->SetSeqInt(StrToInt(element.attribute("seq").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSequence::WriteSequence(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetSeqInt() == 0) {
        element.append_attribute("seq") = IntToStr(this->GetSeqInt()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attseq> */

//----------------------------------------------------------------------------
// AttSlashcount
//----------------------------------------------------------------------------

AttSlashcount::AttSlashcount(): Att() {
    ResetSlashcount();
}

AttSlashcount::~AttSlashcount() {

}

void AttSlashcount::ResetSlashcount() {
    m_slash = 0;
}

bool AttSlashcount::ReadSlashcount(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("slash")) {
        this->SetSlash(StrToInt(element.attribute("slash").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSlashcount::WriteSlashcount(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetSlash() == 0) {
        element.append_attribute("slash") = IntToStr(this->GetSlash()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attslash> */

//----------------------------------------------------------------------------
// AttSlurpresent
//----------------------------------------------------------------------------

AttSlurpresent::AttSlurpresent(): Att() {
    ResetSlurpresent();
}

AttSlurpresent::~AttSlurpresent() {

}

void AttSlurpresent::ResetSlurpresent() {
    m_slur = "";
}

bool AttSlurpresent::ReadSlurpresent(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("slur")) {
        this->SetSlur(StrToStr(element.attribute("slur").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSlurpresent::WriteSlurpresent(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetSlur() == "") {
        element.append_attribute("slur") = StrToStr(this->GetSlur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attslur> */

//----------------------------------------------------------------------------
// AttSpaceVis
//----------------------------------------------------------------------------

AttSpaceVis::AttSpaceVis(): Att() {
    ResetSpaceVis();
}

AttSpaceVis::~AttSpaceVis() {

}

void AttSpaceVis::ResetSpaceVis() {
    m_compressable = "";
}

bool AttSpaceVis::ReadSpaceVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("compressable")) {
        this->SetCompressable(StrToStr(element.attribute("compressable").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSpaceVis::WriteSpaceVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetCompressable() == "") {
        element.append_attribute("compressable") = StrToStr(this->GetCompressable()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attcompressable> */

//----------------------------------------------------------------------------
// AttStaffLog
//----------------------------------------------------------------------------

AttStaffLog::AttStaffLog(): Att() {
    ResetStaffLog();
}

AttStaffLog::~AttStaffLog() {

}

void AttStaffLog::ResetStaffLog() {
    m_def = "";
}

bool AttStaffLog::ReadStaffLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("def")) {
        this->SetDef(StrToStr(element.attribute("def").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffLog::WriteStaffLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetDef() == "") {
        element.append_attribute("def") = StrToStr(this->GetDef()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attdef> */

//----------------------------------------------------------------------------
// AttStaffDefVis
//----------------------------------------------------------------------------

AttStaffDefVis::AttStaffDefVis(): Att() {
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

bool AttStaffDefVis::ReadStaffDefVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("grid.show")) {
        this->SetGridShow(StrToStr(element.attribute("grid.show").value()));
        hasAttribute = true;
    }
    if (element.attribute("layerscheme")) {
        this->SetLayerscheme(StrToStr(element.attribute("layerscheme").value()));
        hasAttribute = true;
    }
    if (element.attribute("lines")) {
        this->SetLinesInt(StrToInt(element.attribute("lines").value()));
        hasAttribute = true;
    }
    if (element.attribute("lines.color")) {
        this->SetLinesColor(StrToStr(element.attribute("lines.color").value()));
        hasAttribute = true;
    }
    if (element.attribute("lines.visible")) {
        this->SetLinesVisible(StrToStr(element.attribute("lines.visible").value()));
        hasAttribute = true;
    }
    if (element.attribute("spacing")) {
        this->SetSpacingDbl(StrToDbl(element.attribute("spacing").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffDefVis::WriteStaffDefVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetGridShow() == "") {
        element.append_attribute("grid.show") = StrToStr(this->GetGridShow()).c_str();
        wroteAttribute = true;
    }
    if (this->GetLayerscheme() == "") {
        element.append_attribute("layerscheme") = StrToStr(this->GetLayerscheme()).c_str();
        wroteAttribute = true;
    }
    if (this->GetLinesInt() == 0) {
        element.append_attribute("lines") = IntToStr(this->GetLinesInt()).c_str();
        wroteAttribute = true;
    }
    if (this->GetLinesColor() == "") {
        element.append_attribute("lines.color") = StrToStr(this->GetLinesColor()).c_str();
        wroteAttribute = true;
    }
    if (this->GetLinesVisible() == "") {
        element.append_attribute("lines.visible") = StrToStr(this->GetLinesVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSpacingDbl() == 0.0) {
        element.append_attribute("spacing") = DblToStr(this->GetSpacingDbl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attspacing> */

//----------------------------------------------------------------------------
// AttStaffGrpVis
//----------------------------------------------------------------------------

AttStaffGrpVis::AttStaffGrpVis(): Att() {
    ResetStaffGrpVis();
}

AttStaffGrpVis::~AttStaffGrpVis() {

}

void AttStaffGrpVis::ResetStaffGrpVis() {
    m_barthru = "";
}

bool AttStaffGrpVis::ReadStaffGrpVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("barthru")) {
        this->SetBarthru(StrToStr(element.attribute("barthru").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffGrpVis::WriteStaffGrpVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetBarthru() == "") {
        element.append_attribute("barthru") = StrToStr(this->GetBarthru()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attbarthru> */

//----------------------------------------------------------------------------
// AttStaffgroupingsym
//----------------------------------------------------------------------------

AttStaffgroupingsym::AttStaffgroupingsym(): Att() {
    ResetStaffgroupingsym();
}

AttStaffgroupingsym::~AttStaffgroupingsym() {

}

void AttStaffgroupingsym::ResetStaffgroupingsym() {
    m_symbol = "";
}

bool AttStaffgroupingsym::ReadStaffgroupingsym(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("symbol")) {
        this->SetSymbol(StrToStr(element.attribute("symbol").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffgroupingsym::WriteStaffgroupingsym(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetSymbol() == "") {
        element.append_attribute("symbol") = StrToStr(this->GetSymbol()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attsymbol> */

//----------------------------------------------------------------------------
// AttStaffident
//----------------------------------------------------------------------------

AttStaffident::AttStaffident(): Att() {
    ResetStaffident();
}

AttStaffident::~AttStaffident() {

}

void AttStaffident::ResetStaffident() {
    m_staff = "";
}

bool AttStaffident::ReadStaffident(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("staff")) {
        this->SetStaff(StrToStr(element.attribute("staff").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffident::WriteStaffident(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetStaff() == "") {
        element.append_attribute("staff") = StrToStr(this->GetStaff()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attstaff> */

//----------------------------------------------------------------------------
// AttStaffloc
//----------------------------------------------------------------------------

AttStaffloc::AttStaffloc(): Att() {
    ResetStaffloc();
}

AttStaffloc::~AttStaffloc() {

}

void AttStaffloc::ResetStaffloc() {
    m_loc = "";
}

bool AttStaffloc::ReadStaffloc(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("loc")) {
        this->SetLoc(StrToStr(element.attribute("loc").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffloc::WriteStaffloc(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetLoc() == "") {
        element.append_attribute("loc") = StrToStr(this->GetLoc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attloc> */

//----------------------------------------------------------------------------
// AttStartendid
//----------------------------------------------------------------------------

AttStartendid::AttStartendid(): Att() {
    ResetStartendid();
}

AttStartendid::~AttStartendid() {

}

void AttStartendid::ResetStartendid() {
    m_endid = "";
}

bool AttStartendid::ReadStartendid(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("endid")) {
        this->SetEndid(StrToStr(element.attribute("endid").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStartendid::WriteStartendid(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetEndid() == "") {
        element.append_attribute("endid") = StrToStr(this->GetEndid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attendid> */

//----------------------------------------------------------------------------
// AttStartid
//----------------------------------------------------------------------------

AttStartid::AttStartid(): Att() {
    ResetStartid();
}

AttStartid::~AttStartid() {

}

void AttStartid::ResetStartid() {
    m_startid = "";
}

bool AttStartid::ReadStartid(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("startid")) {
        this->SetStartid(StrToStr(element.attribute("startid").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStartid::WriteStartid(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetStartid() == "") {
        element.append_attribute("startid") = StrToStr(this->GetStartid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attstartid> */

//----------------------------------------------------------------------------
// AttStemmed
//----------------------------------------------------------------------------

AttStemmed::AttStemmed(): Att() {
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

bool AttStemmed::ReadStemmed(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("stem.dir")) {
        this->SetStemDir(StrToStr(element.attribute("stem.dir").value()));
        hasAttribute = true;
    }
    if (element.attribute("stem.len")) {
        this->SetStemLenDbl(StrToDbl(element.attribute("stem.len").value()));
        hasAttribute = true;
    }
    if (element.attribute("stem.pos")) {
        this->SetStemPos(StrToStr(element.attribute("stem.pos").value()));
        hasAttribute = true;
    }
    if (element.attribute("stem.x")) {
        this->SetStemXDbl(StrToDbl(element.attribute("stem.x").value()));
        hasAttribute = true;
    }
    if (element.attribute("stem.y")) {
        this->SetStemYDbl(StrToDbl(element.attribute("stem.y").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStemmed::WriteStemmed(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetStemDir() == "") {
        element.append_attribute("stem.dir") = StrToStr(this->GetStemDir()).c_str();
        wroteAttribute = true;
    }
    if (this->GetStemLenDbl() == 0.0) {
        element.append_attribute("stem.len") = DblToStr(this->GetStemLenDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetStemPos() == "") {
        element.append_attribute("stem.pos") = StrToStr(this->GetStemPos()).c_str();
        wroteAttribute = true;
    }
    if (this->GetStemXDbl() == 0.0) {
        element.append_attribute("stem.x") = DblToStr(this->GetStemXDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetStemYDbl() == 0.0) {
        element.append_attribute("stem.y") = DblToStr(this->GetStemYDbl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attstem.y> */

//----------------------------------------------------------------------------
// AttSylLog
//----------------------------------------------------------------------------

AttSylLog::AttSylLog(): Att() {
    ResetSylLog();
}

AttSylLog::~AttSylLog() {

}

void AttSylLog::ResetSylLog() {
    m_con = "";
    m_wordpos = "";
}

bool AttSylLog::ReadSylLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("con")) {
        this->SetCon(StrToStr(element.attribute("con").value()));
        hasAttribute = true;
    }
    if (element.attribute("wordpos")) {
        this->SetWordpos(StrToStr(element.attribute("wordpos").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSylLog::WriteSylLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetCon() == "") {
        element.append_attribute("con") = StrToStr(this->GetCon()).c_str();
        wroteAttribute = true;
    }
    if (this->GetWordpos() == "") {
        element.append_attribute("wordpos") = StrToStr(this->GetWordpos()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attwordpos> */

//----------------------------------------------------------------------------
// AttSyltext
//----------------------------------------------------------------------------

AttSyltext::AttSyltext(): Att() {
    ResetSyltext();
}

AttSyltext::~AttSyltext() {

}

void AttSyltext::ResetSyltext() {
    m_syl = "";
}

bool AttSyltext::ReadSyltext(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("syl")) {
        this->SetSyl(StrToStr(element.attribute("syl").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSyltext::WriteSyltext(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetSyl() == "") {
        element.append_attribute("syl") = StrToStr(this->GetSyl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attsyl> */

//----------------------------------------------------------------------------
// AttTextstyle
//----------------------------------------------------------------------------

AttTextstyle::AttTextstyle(): Att() {
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

bool AttTextstyle::ReadTextstyle(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("text.fam")) {
        this->SetTextFam(StrToStr(element.attribute("text.fam").value()));
        hasAttribute = true;
    }
    if (element.attribute("text.name")) {
        this->SetTextName(StrToStr(element.attribute("text.name").value()));
        hasAttribute = true;
    }
    if (element.attribute("text.size")) {
        this->SetTextSizeDbl(StrToDbl(element.attribute("text.size").value()));
        hasAttribute = true;
    }
    if (element.attribute("text.style")) {
        this->SetTextStyle(StrToStr(element.attribute("text.style").value()));
        hasAttribute = true;
    }
    if (element.attribute("text.weight")) {
        this->SetTextWeight(StrToStr(element.attribute("text.weight").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTextstyle::WriteTextstyle(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetTextFam() == "") {
        element.append_attribute("text.fam") = StrToStr(this->GetTextFam()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTextName() == "") {
        element.append_attribute("text.name") = StrToStr(this->GetTextName()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTextSizeDbl() == 0.0) {
        element.append_attribute("text.size") = DblToStr(this->GetTextSizeDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTextStyle() == "") {
        element.append_attribute("text.style") = StrToStr(this->GetTextStyle()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTextWeight() == "") {
        element.append_attribute("text.weight") = StrToStr(this->GetTextWeight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttext.weight> */

//----------------------------------------------------------------------------
// AttTiepresent
//----------------------------------------------------------------------------

AttTiepresent::AttTiepresent(): Att() {
    ResetTiepresent();
}

AttTiepresent::~AttTiepresent() {

}

void AttTiepresent::ResetTiepresent() {
    m_tie = "";
}

bool AttTiepresent::ReadTiepresent(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("tie")) {
        this->SetTie(StrToStr(element.attribute("tie").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTiepresent::WriteTiepresent(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetTie() == "") {
        element.append_attribute("tie") = StrToStr(this->GetTie()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttie> */

//----------------------------------------------------------------------------
// AttTimestampMusical
//----------------------------------------------------------------------------

AttTimestampMusical::AttTimestampMusical(): Att() {
    ResetTimestampMusical();
}

AttTimestampMusical::~AttTimestampMusical() {

}

void AttTimestampMusical::ResetTimestampMusical() {
    m_tstamp = "";
}

bool AttTimestampMusical::ReadTimestampMusical(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("tstamp")) {
        this->SetTstamp(StrToStr(element.attribute("tstamp").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestampMusical::WriteTimestampMusical(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetTstamp() == "") {
        element.append_attribute("tstamp") = StrToStr(this->GetTstamp()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttstamp> */

//----------------------------------------------------------------------------
// AttTimestampPerformed
//----------------------------------------------------------------------------

AttTimestampPerformed::AttTimestampPerformed(): Att() {
    ResetTimestampPerformed();
}

AttTimestampPerformed::~AttTimestampPerformed() {

}

void AttTimestampPerformed::ResetTimestampPerformed() {
    m_tstampGesInt = 0;
    m_tstampReal = "";
}

bool AttTimestampPerformed::ReadTimestampPerformed(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("tstamp.ges")) {
        this->SetTstampGesInt(StrToInt(element.attribute("tstamp.ges").value()));
        hasAttribute = true;
    }
    if (element.attribute("tstamp.real")) {
        this->SetTstampReal(StrToStr(element.attribute("tstamp.real").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestampPerformed::WriteTimestampPerformed(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetTstampGesInt() == 0) {
        element.append_attribute("tstamp.ges") = IntToStr(this->GetTstampGesInt()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTstampReal() == "") {
        element.append_attribute("tstamp.real") = StrToStr(this->GetTstampReal()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttstamp.real> */

//----------------------------------------------------------------------------
// AttTransposition
//----------------------------------------------------------------------------

AttTransposition::AttTransposition(): Att() {
    ResetTransposition();
}

AttTransposition::~AttTransposition() {

}

void AttTransposition::ResetTransposition() {
    m_transDiatDbl = 0.0;
    m_transSemiDbl = 0.0;
}

bool AttTransposition::ReadTransposition(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("trans.diat")) {
        this->SetTransDiatDbl(StrToDbl(element.attribute("trans.diat").value()));
        hasAttribute = true;
    }
    if (element.attribute("trans.semi")) {
        this->SetTransSemiDbl(StrToDbl(element.attribute("trans.semi").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTransposition::WriteTransposition(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetTransDiatDbl() == 0.0) {
        element.append_attribute("trans.diat") = DblToStr(this->GetTransDiatDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetTransSemiDbl() == 0.0) {
        element.append_attribute("trans.semi") = DblToStr(this->GetTransSemiDbl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttrans.semi> */

//----------------------------------------------------------------------------
// AttTupletpresent
//----------------------------------------------------------------------------

AttTupletpresent::AttTupletpresent(): Att() {
    ResetTupletpresent();
}

AttTupletpresent::~AttTupletpresent() {

}

void AttTupletpresent::ResetTupletpresent() {
    m_tuplet = "";
}

bool AttTupletpresent::ReadTupletpresent(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("tuplet")) {
        this->SetTuplet(StrToStr(element.attribute("tuplet").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTupletpresent::WriteTupletpresent(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetTuplet() == "") {
        element.append_attribute("tuplet") = StrToStr(this->GetTuplet()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atttuplet> */

//----------------------------------------------------------------------------
// AttTyped
//----------------------------------------------------------------------------

AttTyped::AttTyped(): Att() {
    ResetTyped();
}

AttTyped::~AttTyped() {

}

void AttTyped::ResetTyped() {
    m_type = "";
    m_subtype = "";
}

bool AttTyped::ReadTyped(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("type")) {
        this->SetType(StrToStr(element.attribute("type").value()));
        hasAttribute = true;
    }
    if (element.attribute("subtype")) {
        this->SetSubtype(StrToStr(element.attribute("subtype").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTyped::WriteTyped(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetType() == "") {
        element.append_attribute("type") = StrToStr(this->GetType()).c_str();
        wroteAttribute = true;
    }
    if (this->GetSubtype() == "") {
        element.append_attribute("subtype") = StrToStr(this->GetSubtype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attsubtype> */

//----------------------------------------------------------------------------
// AttTypography
//----------------------------------------------------------------------------

AttTypography::AttTypography(): Att() {
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

bool AttTypography::ReadTypography(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("fontfam")) {
        this->SetFontfam(StrToStr(element.attribute("fontfam").value()));
        hasAttribute = true;
    }
    if (element.attribute("fontname")) {
        this->SetFontname(StrToStr(element.attribute("fontname").value()));
        hasAttribute = true;
    }
    if (element.attribute("fontsize")) {
        this->SetFontsizeDbl(StrToDbl(element.attribute("fontsize").value()));
        hasAttribute = true;
    }
    if (element.attribute("fontstyle")) {
        this->SetFontstyle(StrToStr(element.attribute("fontstyle").value()));
        hasAttribute = true;
    }
    if (element.attribute("fontweight")) {
        this->SetFontweight(StrToStr(element.attribute("fontweight").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTypography::WriteTypography(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetFontfam() == "") {
        element.append_attribute("fontfam") = StrToStr(this->GetFontfam()).c_str();
        wroteAttribute = true;
    }
    if (this->GetFontname() == "") {
        element.append_attribute("fontname") = StrToStr(this->GetFontname()).c_str();
        wroteAttribute = true;
    }
    if (this->GetFontsizeDbl() == 0.0) {
        element.append_attribute("fontsize") = DblToStr(this->GetFontsizeDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetFontstyle() == "") {
        element.append_attribute("fontstyle") = StrToStr(this->GetFontstyle()).c_str();
        wroteAttribute = true;
    }
    if (this->GetFontweight() == "") {
        element.append_attribute("fontweight") = StrToStr(this->GetFontweight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attfontweight> */

//----------------------------------------------------------------------------
// AttVisibility
//----------------------------------------------------------------------------

AttVisibility::AttVisibility(): Att() {
    ResetVisibility();
}

AttVisibility::~AttVisibility() {

}

void AttVisibility::ResetVisibility() {
    m_visible = "";
}

bool AttVisibility::ReadVisibility(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("visible")) {
        this->SetVisible(StrToStr(element.attribute("visible").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisibility::WriteVisibility(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetVisible() == "") {
        element.append_attribute("visible") = StrToStr(this->GetVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attvisible> */

//----------------------------------------------------------------------------
// AttVisualoffsetHo
//----------------------------------------------------------------------------

AttVisualoffsetHo::AttVisualoffsetHo(): Att() {
    ResetVisualoffsetHo();
}

AttVisualoffsetHo::~AttVisualoffsetHo() {

}

void AttVisualoffsetHo::ResetVisualoffsetHo() {
    m_ho = "";
}

bool AttVisualoffsetHo::ReadVisualoffsetHo(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("ho")) {
        this->SetHo(StrToStr(element.attribute("ho").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffsetHo::WriteVisualoffsetHo(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetHo() == "") {
        element.append_attribute("ho") = StrToStr(this->GetHo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attho> */

//----------------------------------------------------------------------------
// AttVisualoffsetTo
//----------------------------------------------------------------------------

AttVisualoffsetTo::AttVisualoffsetTo(): Att() {
    ResetVisualoffsetTo();
}

AttVisualoffsetTo::~AttVisualoffsetTo() {

}

void AttVisualoffsetTo::ResetVisualoffsetTo() {
    m_to = "";
}

bool AttVisualoffsetTo::ReadVisualoffsetTo(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("to")) {
        this->SetTo(StrToStr(element.attribute("to").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffsetTo::WriteVisualoffsetTo(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetTo() == "") {
        element.append_attribute("to") = StrToStr(this->GetTo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attto> */

//----------------------------------------------------------------------------
// AttVisualoffsetVo
//----------------------------------------------------------------------------

AttVisualoffsetVo::AttVisualoffsetVo(): Att() {
    ResetVisualoffsetVo();
}

AttVisualoffsetVo::~AttVisualoffsetVo() {

}

void AttVisualoffsetVo::ResetVisualoffsetVo() {
    m_vo = "";
}

bool AttVisualoffsetVo::ReadVisualoffsetVo(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("vo")) {
        this->SetVo(StrToStr(element.attribute("vo").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffsetVo::WriteVisualoffsetVo(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetVo() == "") {
        element.append_attribute("vo") = StrToStr(this->GetVo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attvo> */

//----------------------------------------------------------------------------
// AttVisualoffset2Ho
//----------------------------------------------------------------------------

AttVisualoffset2Ho::AttVisualoffset2Ho(): Att() {
    ResetVisualoffset2Ho();
}

AttVisualoffset2Ho::~AttVisualoffset2Ho() {

}

void AttVisualoffset2Ho::ResetVisualoffset2Ho() {
    m_startho = "";
    m_endho = "";
}

bool AttVisualoffset2Ho::ReadVisualoffset2Ho(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("startho")) {
        this->SetStartho(StrToStr(element.attribute("startho").value()));
        hasAttribute = true;
    }
    if (element.attribute("endho")) {
        this->SetEndho(StrToStr(element.attribute("endho").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffset2Ho::WriteVisualoffset2Ho(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetStartho() == "") {
        element.append_attribute("startho") = StrToStr(this->GetStartho()).c_str();
        wroteAttribute = true;
    }
    if (this->GetEndho() == "") {
        element.append_attribute("endho") = StrToStr(this->GetEndho()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attendho> */

//----------------------------------------------------------------------------
// AttVisualoffset2To
//----------------------------------------------------------------------------

AttVisualoffset2To::AttVisualoffset2To(): Att() {
    ResetVisualoffset2To();
}

AttVisualoffset2To::~AttVisualoffset2To() {

}

void AttVisualoffset2To::ResetVisualoffset2To() {
    m_startto = "";
    m_endto = "";
}

bool AttVisualoffset2To::ReadVisualoffset2To(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("startto")) {
        this->SetStartto(StrToStr(element.attribute("startto").value()));
        hasAttribute = true;
    }
    if (element.attribute("endto")) {
        this->SetEndto(StrToStr(element.attribute("endto").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffset2To::WriteVisualoffset2To(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetStartto() == "") {
        element.append_attribute("startto") = StrToStr(this->GetStartto()).c_str();
        wroteAttribute = true;
    }
    if (this->GetEndto() == "") {
        element.append_attribute("endto") = StrToStr(this->GetEndto()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attendto> */

//----------------------------------------------------------------------------
// AttVisualoffset2Vo
//----------------------------------------------------------------------------

AttVisualoffset2Vo::AttVisualoffset2Vo(): Att() {
    ResetVisualoffset2Vo();
}

AttVisualoffset2Vo::~AttVisualoffset2Vo() {

}

void AttVisualoffset2Vo::ResetVisualoffset2Vo() {
    m_startvo = "";
    m_endvo = "";
}

bool AttVisualoffset2Vo::ReadVisualoffset2Vo(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("startvo")) {
        this->SetStartvo(StrToStr(element.attribute("startvo").value()));
        hasAttribute = true;
    }
    if (element.attribute("endvo")) {
        this->SetEndvo(StrToStr(element.attribute("endvo").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffset2Vo::WriteVisualoffset2Vo(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetStartvo() == "") {
        element.append_attribute("startvo") = StrToStr(this->GetStartvo()).c_str();
        wroteAttribute = true;
    }
    if (this->GetEndvo() == "") {
        element.append_attribute("endvo") = StrToStr(this->GetEndvo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attendvo> */

//----------------------------------------------------------------------------
// AttWidth
//----------------------------------------------------------------------------

AttWidth::AttWidth(): Att() {
    ResetWidth();
}

AttWidth::~AttWidth() {

}

void AttWidth::ResetWidth() {
    m_widthDbl = 0.0;
}

bool AttWidth::ReadWidth(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("width")) {
        this->SetWidthDbl(StrToDbl(element.attribute("width").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttWidth::WriteWidth(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetWidthDbl() == 0.0) {
        element.append_attribute("width") = DblToStr(this->GetWidthDbl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attwidth> */

//----------------------------------------------------------------------------
// AttXy
//----------------------------------------------------------------------------

AttXy::AttXy(): Att() {
    ResetXy();
}

AttXy::~AttXy() {

}

void AttXy::ResetXy() {
    m_xDbl = 0.0;
    m_yDbl = 0.0;
}

bool AttXy::ReadXy(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("x")) {
        this->SetXDbl(StrToDbl(element.attribute("x").value()));
        hasAttribute = true;
    }
    if (element.attribute("y")) {
        this->SetYDbl(StrToDbl(element.attribute("y").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttXy::WriteXy(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetXDbl() == 0.0) {
        element.append_attribute("x") = DblToStr(this->GetXDbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetYDbl() == 0.0) {
        element.append_attribute("y") = DblToStr(this->GetYDbl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atty> */

//----------------------------------------------------------------------------
// AttXy2
//----------------------------------------------------------------------------

AttXy2::AttXy2(): Att() {
    ResetXy2();
}

AttXy2::~AttXy2() {

}

void AttXy2::ResetXy2() {
    m_x2Dbl = 0.0;
    m_y2Dbl = 0.0;
}

bool AttXy2::ReadXy2(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("x2")) {
        this->SetX2Dbl(StrToDbl(element.attribute("x2").value()));
        hasAttribute = true;
    }
    if (element.attribute("y2")) {
        this->SetY2Dbl(StrToDbl(element.attribute("y2").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttXy2::WriteXy2(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetX2Dbl() == 0.0) {
        element.append_attribute("x2") = DblToStr(this->GetX2Dbl()).c_str();
        wroteAttribute = true;
    }
    if (this->GetY2Dbl() == 0.0) {
        element.append_attribute("y2") = DblToStr(this->GetY2Dbl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <atty2> */

} // vrv namespace

