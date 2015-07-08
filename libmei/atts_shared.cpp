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

#include "object.h"

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
        element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAccidLog::WriteAccidLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = StrToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAccidLog::HasFunc( )
{
    return (m_func != "");
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
    m_accid = ACCIDENTAL_EXPLICIT_NONE;
}

bool AttAccidental::ReadAccidental(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("accid")) {
        this->SetAccid(StrToAccidentalExplicit(element.attribute("accid").value()));
        element.remove_attribute("accid");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAccidental::WriteAccidental(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasAccid()) {
        element.append_attribute("accid") = AccidentalExplicitToStr(this->GetAccid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAccidental::HasAccid( )
{
    return (m_accid != ACCIDENTAL_EXPLICIT_NONE);
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
        element.remove_attribute("accid.ges");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAccidentalPerformed::WriteAccidentalPerformed(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasAccidGes()) {
        element.append_attribute("accid.ges") = StrToStr(this->GetAccidGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAccidentalPerformed::HasAccidGes( )
{
    return (m_accidGes != "");
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
        element.remove_attribute("altsym");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAltsym::WriteAltsym(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasAltsym()) {
        element.append_attribute("altsym") = StrToStr(this->GetAltsym()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAltsym::HasAltsym( )
{
    return (m_altsym != "");
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
        element.remove_attribute("artic");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArticulation::WriteArticulation(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasArtic()) {
        element.append_attribute("artic") = StrToStr(this->GetArtic()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttArticulation::HasArtic( )
{
    return (m_artic != "");
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
        element.remove_attribute("artic.ges");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArticulationPerformed::WriteArticulationPerformed(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasArticGes()) {
        element.append_attribute("artic.ges") = StrToStr(this->GetArticGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttArticulationPerformed::HasArticGes( )
{
    return (m_articGes != "");
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
    m_dots = 0;
}

bool AttAugmentdots::ReadAugmentdots(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dots")) {
        this->SetDots(StrToInt(element.attribute("dots").value()));
        element.remove_attribute("dots");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAugmentdots::WriteAugmentdots(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDots()) {
        element.append_attribute("dots") = IntToStr(this->GetDots()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAugmentdots::HasDots( )
{
    return (m_dots != 0);
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
        element.remove_attribute("authority");
        hasAttribute = true;
    }
    if (element.attribute("authURI")) {
        this->SetAuthURI(StrToStr(element.attribute("authURI").value()));
        element.remove_attribute("authURI");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAuthorized::WriteAuthorized(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasAuthority()) {
        element.append_attribute("authority") = StrToStr(this->GetAuthority()).c_str();
        wroteAttribute = true;
    }
    if (this->HasAuthURI()) {
        element.append_attribute("authURI") = StrToStr(this->GetAuthURI()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAuthorized::HasAuthority( )
{
    return (m_authority != "");
}

bool AttAuthorized::HasAuthURI( )
{
    return (m_authURI != "");
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
    m_rend = BARRENDITION_single;
}

bool AttBarLineLog::ReadBarLineLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBarRendition(element.attribute("rend").value()));
        element.remove_attribute("rend");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBarLineLog::WriteBarLineLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRend()) {
        element.append_attribute("rend") = BarRenditionToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBarLineLog::HasRend( )
{
    return (m_rend != BARRENDITION_single);
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
        element.remove_attribute("barplace");
        hasAttribute = true;
    }
    if (element.attribute("taktplace")) {
        this->SetTaktplace(StrToStr(element.attribute("taktplace").value()));
        element.remove_attribute("taktplace");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBarplacement::WriteBarplacement(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBarplace()) {
        element.append_attribute("barplace") = StrToStr(this->GetBarplace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTaktplace()) {
        element.append_attribute("taktplace") = StrToStr(this->GetTaktplace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBarplacement::HasBarplace( )
{
    return (m_barplace != "");
}

bool AttBarplacement::HasTaktplace( )
{
    return (m_taktplace != "");
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
    m_beamColor = "";
    m_beamRend = "";
    m_beamSlopeDbl = 0.0;
}

bool AttBeamingVis::ReadBeamingVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("beam.color")) {
        this->SetBeamColor(StrToStr(element.attribute("beam.color").value()));
        element.remove_attribute("beam.color");
        hasAttribute = true;
    }
    if (element.attribute("beam.rend")) {
        this->SetBeamRend(StrToStr(element.attribute("beam.rend").value()));
        element.remove_attribute("beam.rend");
        hasAttribute = true;
    }
    if (element.attribute("beam.slope")) {
        this->SetBeamSlope(StrToDbl(element.attribute("beam.slope").value()));
        element.remove_attribute("beam.slope");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamingVis::WriteBeamingVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBeamColor()) {
        element.append_attribute("beam.color") = StrToStr(this->GetBeamColor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBeamRend()) {
        element.append_attribute("beam.rend") = StrToStr(this->GetBeamRend()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBeamSlope()) {
        element.append_attribute("beam.slope") = DblToStr(this->GetBeamSlope()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamingVis::HasBeamColor( )
{
    return (m_beamColor != "");
}

bool AttBeamingVis::HasBeamRend( )
{
    return (m_beamRend != "");
}

bool AttBeamingVis::HasBeamSlope( )
{
    return (m_beamSlopeDbl != 0.0);
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
        element.remove_attribute("analog");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBibl::WriteBibl(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasAnalog()) {
        element.append_attribute("analog") = StrToStr(this->GetAnalog()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBibl::HasAnalog( )
{
    return (m_analog != "");
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
        element.remove_attribute("calendar");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCalendared::WriteCalendared(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasCalendar()) {
        element.append_attribute("calendar") = StrToStr(this->GetCalendar()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCalendared::HasCalendar( )
{
    return (m_calendar != "");
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
        element.remove_attribute("dbkey");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCanonical::WriteCanonical(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDbkey()) {
        element.append_attribute("dbkey") = StrToStr(this->GetDbkey()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCanonical::HasDbkey( )
{
    return (m_dbkey != "");
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
        element.remove_attribute("cluster");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttChordVis::WriteChordVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasCluster()) {
        element.append_attribute("cluster") = StrToStr(this->GetCluster()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttChordVis::HasCluster( )
{
    return (m_cluster != "");
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
        element.remove_attribute("cautionary");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttClefLog::WriteClefLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasCautionary()) {
        element.append_attribute("cautionary") = StrToStr(this->GetCautionary()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttClefLog::HasCautionary( )
{
    return (m_cautionary != "");
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
        element.remove_attribute("clef.shape");
        hasAttribute = true;
    }
    if (element.attribute("clef.line")) {
        this->SetClefLine(StrToInt(element.attribute("clef.line").value()));
        element.remove_attribute("clef.line");
        hasAttribute = true;
    }
    if (element.attribute("clef.dis")) {
        this->SetClefDis(StrToOctaveDis(element.attribute("clef.dis").value()));
        element.remove_attribute("clef.dis");
        hasAttribute = true;
    }
    if (element.attribute("clef.dis.place")) {
        this->SetClefDisPlace(StrToPlace(element.attribute("clef.dis.place").value()));
        element.remove_attribute("clef.dis.place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCleffingLog::WriteCleffingLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasClefShape()) {
        element.append_attribute("clef.shape") = ClefShapeToStr(this->GetClefShape()).c_str();
        wroteAttribute = true;
    }
    if (this->HasClefLine()) {
        element.append_attribute("clef.line") = IntToStr(this->GetClefLine()).c_str();
        wroteAttribute = true;
    }
    if (this->HasClefDis()) {
        element.append_attribute("clef.dis") = OctaveDisToStr(this->GetClefDis()).c_str();
        wroteAttribute = true;
    }
    if (this->HasClefDisPlace()) {
        element.append_attribute("clef.dis.place") = PlaceToStr(this->GetClefDisPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCleffingLog::HasClefShape( )
{
    return (m_clefShape != CLEFSHAPE_NONE);
}

bool AttCleffingLog::HasClefLine( )
{
    return (m_clefLine != 0);
}

bool AttCleffingLog::HasClefDis( )
{
    return (m_clefDis != OCTAVE_DIS_NONE);
}

bool AttCleffingLog::HasClefDisPlace( )
{
    return (m_clefDisPlace != PLACE_NONE);
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
        element.remove_attribute("clef.color");
        hasAttribute = true;
    }
    if (element.attribute("clef.visible")) {
        this->SetClefVisible(StrToStr(element.attribute("clef.visible").value()));
        element.remove_attribute("clef.visible");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCleffingVis::WriteCleffingVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasClefColor()) {
        element.append_attribute("clef.color") = StrToStr(this->GetClefColor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasClefVisible()) {
        element.append_attribute("clef.visible") = StrToStr(this->GetClefVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCleffingVis::HasClefColor( )
{
    return (m_clefColor != "");
}

bool AttCleffingVis::HasClefVisible( )
{
    return (m_clefVisible != "");
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
        element.remove_attribute("shape");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttClefshape::WriteClefshape(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasShape()) {
        element.append_attribute("shape") = ClefShapeToStr(this->GetShape()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttClefshape::HasShape( )
{
    return (m_shape != CLEFSHAPE_NONE);
}


/* include <attshape> */

//----------------------------------------------------------------------------
// AttCoded
//----------------------------------------------------------------------------

AttCoded::AttCoded(): Att() {
    ResetCoded();
}

AttCoded::~AttCoded() {

}

void AttCoded::ResetCoded() {
    m_code = "";
}

bool AttCoded::ReadCoded(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("code")) {
        this->SetCode(StrToStr(element.attribute("code").value()));
        element.remove_attribute("code");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCoded::WriteCoded(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasCode()) {
        element.append_attribute("code") = StrToStr(this->GetCode()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCoded::HasCode( )
{
    return (m_code != "");
}


/* include <attcode> */

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
        element.remove_attribute("color");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttColor::WriteColor(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasColor()) {
        element.append_attribute("color") = StrToStr(this->GetColor()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttColor::HasColor( )
{
    return (m_color != "");
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
    m_colored = BOOLEAN_NONE;
}

bool AttColoration::ReadColoration(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("colored")) {
        this->SetColored(StrToBool(element.attribute("colored").value()));
        element.remove_attribute("colored");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttColoration::WriteColoration(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasColored()) {
        element.append_attribute("colored") = BoolToStr(this->GetColored()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttColoration::HasColored( )
{
    return (m_colored != BOOLEAN_NONE);
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
        element.remove_attribute("label");
        hasAttribute = true;
    }
    if (element.attribute("n")) {
        this->SetN(StrToInt(element.attribute("n").value()));
        element.remove_attribute("n");
        hasAttribute = true;
    }
    if (element.attribute("base")) {
        this->SetBase(StrToStr(element.attribute("base").value()));
        element.remove_attribute("base");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCommon::WriteCommon(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasLabel()) {
        element.append_attribute("label") = StrToStr(this->GetLabel()).c_str();
        wroteAttribute = true;
    }
    if (this->HasN()) {
        element.append_attribute("n") = IntToStr(this->GetN()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBase()) {
        element.append_attribute("base") = StrToStr(this->GetBase()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCommon::HasLabel( )
{
    return (m_label != "");
}

bool AttCommon::HasN( )
{
    return (m_n != 0);
}

bool AttCommon::HasBase( )
{
    return (m_base != "");
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
        this->SetUlx(StrToInt(element.attribute("ulx").value()));
        element.remove_attribute("ulx");
        hasAttribute = true;
    }
    if (element.attribute("uly")) {
        this->SetUly(StrToInt(element.attribute("uly").value()));
        element.remove_attribute("uly");
        hasAttribute = true;
    }
    if (element.attribute("lrx")) {
        this->SetLrx(StrToInt(element.attribute("lrx").value()));
        element.remove_attribute("lrx");
        hasAttribute = true;
    }
    if (element.attribute("lry")) {
        this->SetLry(StrToInt(element.attribute("lry").value()));
        element.remove_attribute("lry");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCoordinated::WriteCoordinated(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasUlx()) {
        element.append_attribute("ulx") = IntToStr(this->GetUlx()).c_str();
        wroteAttribute = true;
    }
    if (this->HasUly()) {
        element.append_attribute("uly") = IntToStr(this->GetUly()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLrx()) {
        element.append_attribute("lrx") = IntToStr(this->GetLrx()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLry()) {
        element.append_attribute("lry") = IntToStr(this->GetLry()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCoordinated::HasUlx( )
{
    return (m_ulxInt != 0);
}

bool AttCoordinated::HasUly( )
{
    return (m_ulyInt != 0);
}

bool AttCoordinated::HasLrx( )
{
    return (m_lrxInt != 0);
}

bool AttCoordinated::HasLry( )
{
    return (m_lryInt != 0);
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
        element.remove_attribute("bezier");
        hasAttribute = true;
    }
    if (element.attribute("bulge")) {
        this->SetBulge(StrToStr(element.attribute("bulge").value()));
        element.remove_attribute("bulge");
        hasAttribute = true;
    }
    if (element.attribute("curvedir")) {
        this->SetCurvedir(StrToStr(element.attribute("curvedir").value()));
        element.remove_attribute("curvedir");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCurvature::WriteCurvature(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBezier()) {
        element.append_attribute("bezier") = StrToStr(this->GetBezier()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBulge()) {
        element.append_attribute("bulge") = StrToStr(this->GetBulge()).c_str();
        wroteAttribute = true;
    }
    if (this->HasCurvedir()) {
        element.append_attribute("curvedir") = StrToStr(this->GetCurvedir()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCurvature::HasBezier( )
{
    return (m_bezier != "");
}

bool AttCurvature::HasBulge( )
{
    return (m_bulge != "");
}

bool AttCurvature::HasCurvedir( )
{
    return (m_curvedir != "");
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
    m_rend = BARRENDITION_single;
}

bool AttCurverend::ReadCurverend(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBarRendition(element.attribute("rend").value()));
        element.remove_attribute("rend");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCurverend::WriteCurverend(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRend()) {
        element.append_attribute("rend") = BarRenditionToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCurverend::HasRend( )
{
    return (m_rend != BARRENDITION_single);
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
        element.remove_attribute("target");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCustosLog::WriteCustosLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTarget()) {
        element.append_attribute("target") = StrToStr(this->GetTarget()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCustosLog::HasTarget( )
{
    return (m_target != "");
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
        element.remove_attribute("enddate");
        hasAttribute = true;
    }
    if (element.attribute("isodate")) {
        this->SetIsodate(StrToStr(element.attribute("isodate").value()));
        element.remove_attribute("isodate");
        hasAttribute = true;
    }
    if (element.attribute("notafter")) {
        this->SetNotafter(StrToStr(element.attribute("notafter").value()));
        element.remove_attribute("notafter");
        hasAttribute = true;
    }
    if (element.attribute("notbefore")) {
        this->SetNotbefore(StrToStr(element.attribute("notbefore").value()));
        element.remove_attribute("notbefore");
        hasAttribute = true;
    }
    if (element.attribute("startdate")) {
        this->SetStartdate(StrToStr(element.attribute("startdate").value()));
        element.remove_attribute("startdate");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDatable::WriteDatable(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasEnddate()) {
        element.append_attribute("enddate") = StrToStr(this->GetEnddate()).c_str();
        wroteAttribute = true;
    }
    if (this->HasIsodate()) {
        element.append_attribute("isodate") = StrToStr(this->GetIsodate()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNotafter()) {
        element.append_attribute("notafter") = StrToStr(this->GetNotafter()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNotbefore()) {
        element.append_attribute("notbefore") = StrToStr(this->GetNotbefore()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStartdate()) {
        element.append_attribute("startdate") = StrToStr(this->GetStartdate()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDatable::HasEnddate( )
{
    return (m_enddate != "");
}

bool AttDatable::HasIsodate( )
{
    return (m_isodate != "");
}

bool AttDatable::HasNotafter( )
{
    return (m_notafter != "");
}

bool AttDatable::HasNotbefore( )
{
    return (m_notbefore != "");
}

bool AttDatable::HasStartdate( )
{
    return (m_startdate != "");
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
        element.remove_attribute("data");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDatapointing::WriteDatapointing(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasData()) {
        element.append_attribute("data") = StrToStr(this->GetData()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDatapointing::HasData( )
{
    return (m_data != "");
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
        element.remove_attribute("decls");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDeclaring::WriteDeclaring(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDecls()) {
        element.append_attribute("decls") = StrToStr(this->GetDecls()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDeclaring::HasDecls( )
{
    return (m_decls != "");
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
    m_dynamDist = "";
    m_harmDist = "";
    m_textDist = "";
}

bool AttDistances::ReadDistances(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dynam.dist")) {
        this->SetDynamDist(StrToStr(element.attribute("dynam.dist").value()));
        element.remove_attribute("dynam.dist");
        hasAttribute = true;
    }
    if (element.attribute("harm.dist")) {
        this->SetHarmDist(StrToStr(element.attribute("harm.dist").value()));
        element.remove_attribute("harm.dist");
        hasAttribute = true;
    }
    if (element.attribute("text.dist")) {
        this->SetTextDist(StrToStr(element.attribute("text.dist").value()));
        element.remove_attribute("text.dist");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDistances::WriteDistances(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDynamDist()) {
        element.append_attribute("dynam.dist") = StrToStr(this->GetDynamDist()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHarmDist()) {
        element.append_attribute("harm.dist") = StrToStr(this->GetHarmDist()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTextDist()) {
        element.append_attribute("text.dist") = StrToStr(this->GetTextDist()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDistances::HasDynamDist( )
{
    return (m_dynamDist != "");
}

bool AttDistances::HasHarmDist( )
{
    return (m_harmDist != "");
}

bool AttDistances::HasTextDist( )
{
    return (m_textDist != "");
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
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDotLog::WriteDotLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDotLog::HasForm( )
{
    return (m_form != "");
}


/* include <attform> */

//----------------------------------------------------------------------------
// AttDurationAdditive
//----------------------------------------------------------------------------

AttDurationAdditive::AttDurationAdditive(): Att() {
    ResetDurationAdditive();
}

AttDurationAdditive::~AttDurationAdditive() {

}

void AttDurationAdditive::ResetDurationAdditive() {
    m_dur = DURATION_NONE;
}

bool AttDurationAdditive::ReadDurationAdditive(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dur")) {
        this->SetDur(StrToDur(element.attribute("dur").value()));
        element.remove_attribute("dur");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationAdditive::WriteDurationAdditive(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDur()) {
        element.append_attribute("dur") = DurToStr(this->GetDur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDurationAdditive::HasDur( )
{
    return (m_dur != DURATION_NONE);
}


/* include <attdur> */

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
    m_numDefaultInt = 0;
    m_numbaseDefaultInt = 0;
}

bool AttDurationDefault::ReadDurationDefault(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dur.default")) {
        this->SetDurDefault(StrToStr(element.attribute("dur.default").value()));
        element.remove_attribute("dur.default");
        hasAttribute = true;
    }
    if (element.attribute("num.default")) {
        this->SetNumDefault(StrToInt(element.attribute("num.default").value()));
        element.remove_attribute("num.default");
        hasAttribute = true;
    }
    if (element.attribute("numbase.default")) {
        this->SetNumbaseDefault(StrToInt(element.attribute("numbase.default").value()));
        element.remove_attribute("numbase.default");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationDefault::WriteDurationDefault(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDurDefault()) {
        element.append_attribute("dur.default") = StrToStr(this->GetDurDefault()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNumDefault()) {
        element.append_attribute("num.default") = IntToStr(this->GetNumDefault()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNumbaseDefault()) {
        element.append_attribute("numbase.default") = IntToStr(this->GetNumbaseDefault()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDurationDefault::HasDurDefault( )
{
    return (m_durDefault != "");
}

bool AttDurationDefault::HasNumDefault( )
{
    return (m_numDefaultInt != 0);
}

bool AttDurationDefault::HasNumbaseDefault( )
{
    return (m_numbaseDefaultInt != 0);
}


/* include <attnumbase.default> */

//----------------------------------------------------------------------------
// AttDurationMusical
//----------------------------------------------------------------------------

AttDurationMusical::AttDurationMusical(): Att() {
    ResetDurationMusical();
}

AttDurationMusical::~AttDurationMusical() {

}

void AttDurationMusical::ResetDurationMusical() {
    m_dur = DURATION_NONE;
}

bool AttDurationMusical::ReadDurationMusical(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dur")) {
        this->SetDur(StrToDur(element.attribute("dur").value()));
        element.remove_attribute("dur");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationMusical::WriteDurationMusical(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDur()) {
        element.append_attribute("dur") = DurToStr(this->GetDur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDurationMusical::HasDur( )
{
    return (m_dur != DURATION_NONE);
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
    m_durGes = DURATION_NONE;
}

bool AttDurationPerformed::ReadDurationPerformed(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dur.ges")) {
        this->SetDurGes(StrToDur(element.attribute("dur.ges").value()));
        element.remove_attribute("dur.ges");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationPerformed::WriteDurationPerformed(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDurGes()) {
        element.append_attribute("dur.ges") = DurToStr(this->GetDurGes()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDurationPerformed::HasDurGes( )
{
    return (m_durGes != DURATION_NONE);
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
    m_num = -1;
    m_numbase = -1;
}

bool AttDurationRatio::ReadDurationRatio(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNum(StrToInt(element.attribute("num").value()));
        element.remove_attribute("num");
        hasAttribute = true;
    }
    if (element.attribute("numbase")) {
        this->SetNumbase(StrToInt(element.attribute("numbase").value()));
        element.remove_attribute("numbase");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationRatio::WriteDurationRatio(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasNum()) {
        element.append_attribute("num") = IntToStr(this->GetNum()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNumbase()) {
        element.append_attribute("numbase") = IntToStr(this->GetNumbase()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDurationRatio::HasNum( )
{
    return (m_num != -1);
}

bool AttDurationRatio::HasNumbase( )
{
    return (m_numbase != -1);
}


/* include <attnumbase> */

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
        element.remove_attribute("enclose");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttEnclosingchars::WriteEnclosingchars(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasEnclose()) {
        element.append_attribute("enclose") = StrToStr(this->GetEnclose()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttEnclosingchars::HasEnclose( )
{
    return (m_enclose != "");
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
    m_fermata = PLACE_NONE;
}

bool AttFermatapresent::ReadFermatapresent(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("fermata")) {
        this->SetFermata(StrToPlace(element.attribute("fermata").value()));
        element.remove_attribute("fermata");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFermatapresent::WriteFermatapresent(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasFermata()) {
        element.append_attribute("fermata") = PlaceToStr(this->GetFermata()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFermatapresent::HasFermata( )
{
    return (m_fermata != PLACE_NONE);
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
        element.remove_attribute("hand");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHandident::WriteHandident(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasHand()) {
        element.append_attribute("hand") = StrToStr(this->GetHand()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHandident::HasHand( )
{
    return (m_hand != "");
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
        element.remove_attribute("halign");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHorizontalalign::WriteHorizontalalign(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasHalign()) {
        element.append_attribute("halign") = StrToStr(this->GetHalign()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHorizontalalign::HasHalign( )
{
    return (m_halign != "");
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
        element.remove_attribute("instr");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttInstrumentident::WriteInstrumentident(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasInstr()) {
        element.append_attribute("instr") = StrToStr(this->GetInstr()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttInstrumentident::HasInstr( )
{
    return (m_instr != "");
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
        element.remove_attribute("mimetype");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttInternetmedia::WriteInternetmedia(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMimetype()) {
        element.append_attribute("mimetype") = StrToStr(this->GetMimetype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttInternetmedia::HasMimetype( )
{
    return (m_mimetype != "");
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
        element.remove_attribute("join");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttJoined::WriteJoined(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasJoin()) {
        element.append_attribute("join") = StrToStr(this->GetJoin()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttJoined::HasJoin( )
{
    return (m_join != "");
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
        element.remove_attribute("mode");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigLog::WriteKeySigLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMode()) {
        element.append_attribute("mode") = StrToStr(this->GetMode()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigLog::HasMode( )
{
    return (m_mode != "");
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
        element.remove_attribute("key.accid");
        hasAttribute = true;
    }
    if (element.attribute("key.mode")) {
        this->SetKeyMode(StrToMode(element.attribute("key.mode").value()));
        element.remove_attribute("key.mode");
        hasAttribute = true;
    }
    if (element.attribute("key.pname")) {
        this->SetKeyPname(StrToPitchName(element.attribute("key.pname").value()));
        element.remove_attribute("key.pname");
        hasAttribute = true;
    }
    if (element.attribute("key.sig")) {
        this->SetKeySig(StrToKeySignature(element.attribute("key.sig").value()));
        element.remove_attribute("key.sig");
        hasAttribute = true;
    }
    if (element.attribute("key.sig.mixed")) {
        this->SetKeySigMixed(StrToStr(element.attribute("key.sig.mixed").value()));
        element.remove_attribute("key.sig.mixed");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigDefaultLog::WriteKeySigDefaultLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasKeyAccid()) {
        element.append_attribute("key.accid") = AccidentalImplicitToStr(this->GetKeyAccid()).c_str();
        wroteAttribute = true;
    }
    if (this->HasKeyMode()) {
        element.append_attribute("key.mode") = ModeToStr(this->GetKeyMode()).c_str();
        wroteAttribute = true;
    }
    if (this->HasKeyPname()) {
        element.append_attribute("key.pname") = PitchNameToStr(this->GetKeyPname()).c_str();
        wroteAttribute = true;
    }
    if (this->HasKeySig()) {
        element.append_attribute("key.sig") = KeySignatureToStr(this->GetKeySig()).c_str();
        wroteAttribute = true;
    }
    if (this->HasKeySigMixed()) {
        element.append_attribute("key.sig.mixed") = StrToStr(this->GetKeySigMixed()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigDefaultLog::HasKeyAccid( )
{
    return (m_keyAccid != ACCIDENTAL_IMPLICIT_NONE);
}

bool AttKeySigDefaultLog::HasKeyMode( )
{
    return (m_keyMode != MODE_NONE);
}

bool AttKeySigDefaultLog::HasKeyPname( )
{
    return (m_keyPname != PITCHNAME_NONE);
}

bool AttKeySigDefaultLog::HasKeySig( )
{
    return (m_keySig != KEYSIGNATURE_NONE);
}

bool AttKeySigDefaultLog::HasKeySigMixed( )
{
    return (m_keySigMixed != "");
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
        element.remove_attribute("key.sig.show");
        hasAttribute = true;
    }
    if (element.attribute("key.sig.showchange")) {
        this->SetKeySigShowchange(StrToStr(element.attribute("key.sig.showchange").value()));
        element.remove_attribute("key.sig.showchange");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigDefaultVis::WriteKeySigDefaultVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasKeySigShow()) {
        element.append_attribute("key.sig.show") = StrToStr(this->GetKeySigShow()).c_str();
        wroteAttribute = true;
    }
    if (this->HasKeySigShowchange()) {
        element.append_attribute("key.sig.showchange") = StrToStr(this->GetKeySigShowchange()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigDefaultVis::HasKeySigShow( )
{
    return (m_keySigShow != "");
}

bool AttKeySigDefaultVis::HasKeySigShowchange( )
{
    return (m_keySigShowchange != "");
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
        element.remove_attribute("label.abbr");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLabelsAddl::WriteLabelsAddl(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasLabelAbbr()) {
        element.append_attribute("label.abbr") = StrToStr(this->GetLabelAbbr()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLabelsAddl::HasLabelAbbr( )
{
    return (m_labelAbbr != "");
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
        element.remove_attribute("lang");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLang::WriteLang(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasLang()) {
        element.append_attribute("lang") = StrToStr(this->GetLang()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLang::HasLang( )
{
    return (m_lang != "");
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
        element.remove_attribute("def");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLayerLog::WriteLayerLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDef()) {
        element.append_attribute("def") = StrToStr(this->GetDef()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLayerLog::HasDef( )
{
    return (m_def != "");
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
        element.remove_attribute("layer");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLayerident::WriteLayerident(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasLayer()) {
        element.append_attribute("layer") = StrToStr(this->GetLayer()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLayerident::HasLayer( )
{
    return (m_layer != "");
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
        element.remove_attribute("line");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLineloc::WriteLineloc(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasLine()) {
        element.append_attribute("line") = IntToStr(this->GetLine()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLineloc::HasLine( )
{
    return (m_line != 0);
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
    m_rend = BARRENDITION_single;
}

bool AttLinerend::ReadLinerend(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBarRendition(element.attribute("rend").value()));
        element.remove_attribute("rend");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLinerend::WriteLinerend(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRend()) {
        element.append_attribute("rend") = BarRenditionToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLinerend::HasRend( )
{
    return (m_rend != BARRENDITION_single);
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
    m_lyricAlign = "";
    m_lyricFam = "";
    m_lyricName = "";
    m_lyricSize = "";
    m_lyricStyle = "";
    m_lyricWeight = "";
}

bool AttLyricstyle::ReadLyricstyle(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("lyric.align")) {
        this->SetLyricAlign(StrToStr(element.attribute("lyric.align").value()));
        element.remove_attribute("lyric.align");
        hasAttribute = true;
    }
    if (element.attribute("lyric.fam")) {
        this->SetLyricFam(StrToStr(element.attribute("lyric.fam").value()));
        element.remove_attribute("lyric.fam");
        hasAttribute = true;
    }
    if (element.attribute("lyric.name")) {
        this->SetLyricName(StrToStr(element.attribute("lyric.name").value()));
        element.remove_attribute("lyric.name");
        hasAttribute = true;
    }
    if (element.attribute("lyric.size")) {
        this->SetLyricSize(StrToStr(element.attribute("lyric.size").value()));
        element.remove_attribute("lyric.size");
        hasAttribute = true;
    }
    if (element.attribute("lyric.style")) {
        this->SetLyricStyle(StrToStr(element.attribute("lyric.style").value()));
        element.remove_attribute("lyric.style");
        hasAttribute = true;
    }
    if (element.attribute("lyric.weight")) {
        this->SetLyricWeight(StrToStr(element.attribute("lyric.weight").value()));
        element.remove_attribute("lyric.weight");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLyricstyle::WriteLyricstyle(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasLyricAlign()) {
        element.append_attribute("lyric.align") = StrToStr(this->GetLyricAlign()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLyricFam()) {
        element.append_attribute("lyric.fam") = StrToStr(this->GetLyricFam()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLyricName()) {
        element.append_attribute("lyric.name") = StrToStr(this->GetLyricName()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLyricSize()) {
        element.append_attribute("lyric.size") = StrToStr(this->GetLyricSize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLyricStyle()) {
        element.append_attribute("lyric.style") = StrToStr(this->GetLyricStyle()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLyricWeight()) {
        element.append_attribute("lyric.weight") = StrToStr(this->GetLyricWeight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLyricstyle::HasLyricAlign( )
{
    return (m_lyricAlign != "");
}

bool AttLyricstyle::HasLyricFam( )
{
    return (m_lyricFam != "");
}

bool AttLyricstyle::HasLyricName( )
{
    return (m_lyricName != "");
}

bool AttLyricstyle::HasLyricSize( )
{
    return (m_lyricSize != "");
}

bool AttLyricstyle::HasLyricStyle( )
{
    return (m_lyricStyle != "");
}

bool AttLyricstyle::HasLyricWeight( )
{
    return (m_lyricWeight != "");
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
        element.remove_attribute("left");
        hasAttribute = true;
    }
    if (element.attribute("right")) {
        this->SetRight(StrToBarRendition(element.attribute("right").value()));
        element.remove_attribute("right");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeasureLog::WriteMeasureLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasLeft()) {
        element.append_attribute("left") = BarRenditionToStr(this->GetLeft()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRight()) {
        element.append_attribute("right") = BarRenditionToStr(this->GetRight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeasureLog::HasLeft( )
{
    return (m_left != BARRENDITION_NONE);
}

bool AttMeasureLog::HasRight( )
{
    return (m_right != BARRENDITION_single);
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
        element.remove_attribute("unit");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeasurement::WriteMeasurement(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasUnit()) {
        element.append_attribute("unit") = IntToStr(this->GetUnit()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeasurement::HasUnit( )
{
    return (m_unit != 0);
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
        element.remove_attribute("begin");
        hasAttribute = true;
    }
    if (element.attribute("end")) {
        this->SetEnd(StrToStr(element.attribute("end").value()));
        element.remove_attribute("end");
        hasAttribute = true;
    }
    if (element.attribute("betype")) {
        this->SetBetype(StrToStr(element.attribute("betype").value()));
        element.remove_attribute("betype");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMediabounds::WriteMediabounds(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBegin()) {
        element.append_attribute("begin") = StrToStr(this->GetBegin()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEnd()) {
        element.append_attribute("end") = StrToStr(this->GetEnd()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBetype()) {
        element.append_attribute("betype") = StrToStr(this->GetBetype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMediabounds::HasBegin( )
{
    return (m_begin != "");
}

bool AttMediabounds::HasEnd( )
{
    return (m_end != "");
}

bool AttMediabounds::HasBetype( )
{
    return (m_betype != "");
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
        element.remove_attribute("medium");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMedium::WriteMedium(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMedium()) {
        element.append_attribute("medium") = StrToStr(this->GetMedium()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMedium::HasMedium( )
{
    return (m_medium != "");
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
    m_meiversionNum = "";
}

bool AttMeiversion::ReadMeiversion(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("meiversion")) {
        this->SetMeiversion(StrToStr(element.attribute("meiversion").value()));
        element.remove_attribute("meiversion");
        hasAttribute = true;
    }
    if (element.attribute("meiversion.num")) {
        this->SetMeiversionNum(StrToStr(element.attribute("meiversion.num").value()));
        element.remove_attribute("meiversion.num");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeiversion::WriteMeiversion(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMeiversion()) {
        element.append_attribute("meiversion") = StrToStr(this->GetMeiversion()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMeiversionNum()) {
        element.append_attribute("meiversion.num") = StrToStr(this->GetMeiversionNum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeiversion::HasMeiversion( )
{
    return (m_meiversion != "");
}

bool AttMeiversion::HasMeiversionNum( )
{
    return (m_meiversionNum != "");
}


/* include <attmeiversion.num> */

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
    m_sign = MENSURATIONSIGN_NONE;
}

bool AttMensurLog::ReadMensurLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("dot")) {
        this->SetDot(StrToBool(element.attribute("dot").value()));
        element.remove_attribute("dot");
        hasAttribute = true;
    }
    if (element.attribute("sign")) {
        this->SetSign(StrToMensurationSign(element.attribute("sign").value()));
        element.remove_attribute("sign");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensurLog::WriteMensurLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDot()) {
        element.append_attribute("dot") = BoolToStr(this->GetDot()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSign()) {
        element.append_attribute("sign") = MensurationSignToStr(this->GetSign()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensurLog::HasDot( )
{
    return (m_dot != BOOLEAN_NONE);
}

bool AttMensurLog::HasSign( )
{
    return (m_sign != MENSURATIONSIGN_NONE);
}


/* include <attsign> */

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
        element.remove_attribute("count");
        hasAttribute = true;
    }
    if (element.attribute("sym")) {
        this->SetSym(StrToMeterSign(element.attribute("sym").value()));
        element.remove_attribute("sym");
        hasAttribute = true;
    }
    if (element.attribute("unit")) {
        this->SetUnit(StrToInt(element.attribute("unit").value()));
        element.remove_attribute("unit");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigLog::WriteMeterSigLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasCount()) {
        element.append_attribute("count") = IntToStr(this->GetCount()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSym()) {
        element.append_attribute("sym") = MeterSignToStr(this->GetSym()).c_str();
        wroteAttribute = true;
    }
    if (this->HasUnit()) {
        element.append_attribute("unit") = IntToStr(this->GetUnit()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterSigLog::HasCount( )
{
    return (m_count != 0);
}

bool AttMeterSigLog::HasSym( )
{
    return (m_sym != METERSIGN_NONE);
}

bool AttMeterSigLog::HasUnit( )
{
    return (m_unit != 0);
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
    m_rend = BARRENDITION_single;
}

bool AttMeterSigVis::ReadMeterSigVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBarRendition(element.attribute("rend").value()));
        element.remove_attribute("rend");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigVis::WriteMeterSigVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRend()) {
        element.append_attribute("rend") = BarRenditionToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterSigVis::HasRend( )
{
    return (m_rend != BARRENDITION_single);
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
        element.remove_attribute("meter.count");
        hasAttribute = true;
    }
    if (element.attribute("meter.unit")) {
        this->SetMeterUnit(StrToInt(element.attribute("meter.unit").value()));
        element.remove_attribute("meter.unit");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigDefaultLog::WriteMeterSigDefaultLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMeterCount()) {
        element.append_attribute("meter.count") = IntToStr(this->GetMeterCount()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMeterUnit()) {
        element.append_attribute("meter.unit") = IntToStr(this->GetMeterUnit()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterSigDefaultLog::HasMeterCount( )
{
    return (m_meterCount != 0);
}

bool AttMeterSigDefaultLog::HasMeterUnit( )
{
    return (m_meterUnit != 0);
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
        element.remove_attribute("meter.rend");
        hasAttribute = true;
    }
    if (element.attribute("meter.showchange")) {
        this->SetMeterShowchange(StrToStr(element.attribute("meter.showchange").value()));
        element.remove_attribute("meter.showchange");
        hasAttribute = true;
    }
    if (element.attribute("meter.sym")) {
        this->SetMeterSym(StrToMeterSign(element.attribute("meter.sym").value()));
        element.remove_attribute("meter.sym");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigDefaultVis::WriteMeterSigDefaultVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMeterRend()) {
        element.append_attribute("meter.rend") = StrToStr(this->GetMeterRend()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMeterShowchange()) {
        element.append_attribute("meter.showchange") = StrToStr(this->GetMeterShowchange()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMeterSym()) {
        element.append_attribute("meter.sym") = MeterSignToStr(this->GetMeterSym()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterSigDefaultVis::HasMeterRend( )
{
    return (m_meterRend != "");
}

bool AttMeterSigDefaultVis::HasMeterShowchange( )
{
    return (m_meterShowchange != "");
}

bool AttMeterSigDefaultVis::HasMeterSym( )
{
    return (m_meterSym != METERSIGN_NONE);
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
        element.remove_attribute("metcon");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterconformance::WriteMeterconformance(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMetcon()) {
        element.append_attribute("metcon") = StrToStr(this->GetMetcon()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterconformance::HasMetcon( )
{
    return (m_metcon != "");
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
        element.remove_attribute("metcon");
        hasAttribute = true;
    }
    if (element.attribute("control")) {
        this->SetControl(StrToStr(element.attribute("control").value()));
        element.remove_attribute("control");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterconformanceBar::WriteMeterconformanceBar(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMetcon()) {
        element.append_attribute("metcon") = StrToStr(this->GetMetcon()).c_str();
        wroteAttribute = true;
    }
    if (this->HasControl()) {
        element.append_attribute("control") = StrToStr(this->GetControl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterconformanceBar::HasMetcon( )
{
    return (m_metcon != "");
}

bool AttMeterconformanceBar::HasControl( )
{
    return (m_control != "");
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
    m_mmUnit = "";
    m_mmDots = "";
}

bool AttMmtempo::ReadMmtempo(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("mm")) {
        this->SetMm(StrToStr(element.attribute("mm").value()));
        element.remove_attribute("mm");
        hasAttribute = true;
    }
    if (element.attribute("mm.unit")) {
        this->SetMmUnit(StrToStr(element.attribute("mm.unit").value()));
        element.remove_attribute("mm.unit");
        hasAttribute = true;
    }
    if (element.attribute("mm.dots")) {
        this->SetMmDots(StrToStr(element.attribute("mm.dots").value()));
        element.remove_attribute("mm.dots");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMmtempo::WriteMmtempo(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMm()) {
        element.append_attribute("mm") = StrToStr(this->GetMm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMmUnit()) {
        element.append_attribute("mm.unit") = StrToStr(this->GetMmUnit()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMmDots()) {
        element.append_attribute("mm.dots") = StrToStr(this->GetMmDots()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMmtempo::HasMm( )
{
    return (m_mm != "");
}

bool AttMmtempo::HasMmUnit( )
{
    return (m_mmUnit != "");
}

bool AttMmtempo::HasMmDots( )
{
    return (m_mmDots != "");
}


/* include <attmm.dots> */

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
        element.remove_attribute("multi.number");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMultinummeasures::WriteMultinummeasures(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMultiNumber()) {
        element.append_attribute("multi.number") = StrToStr(this->GetMultiNumber()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMultinummeasures::HasMultiNumber( )
{
    return (m_multiNumber != "");
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
        element.remove_attribute("nymref");
        hasAttribute = true;
    }
    if (element.attribute("role")) {
        this->SetRole(StrToStr(element.attribute("role").value()));
        element.remove_attribute("role");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttName::WriteName(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasNymref()) {
        element.append_attribute("nymref") = StrToStr(this->GetNymref()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRole()) {
        element.append_attribute("role") = StrToStr(this->GetRole()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttName::HasNymref( )
{
    return (m_nymref != "");
}

bool AttName::HasRole( )
{
    return (m_role != "");
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
        element.remove_attribute("oct.ges");
        hasAttribute = true;
    }
    if (element.attribute("pname.ges")) {
        this->SetPnameGes(StrToStr(element.attribute("pname.ges").value()));
        element.remove_attribute("pname.ges");
        hasAttribute = true;
    }
    if (element.attribute("pnum")) {
        this->SetPnum(StrToStr(element.attribute("pnum").value()));
        element.remove_attribute("pnum");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteGes::WriteNoteGes(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasOctGes()) {
        element.append_attribute("oct.ges") = StrToStr(this->GetOctGes()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPnameGes()) {
        element.append_attribute("pname.ges") = StrToStr(this->GetPnameGes()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPnum()) {
        element.append_attribute("pnum") = StrToStr(this->GetPnum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNoteGes::HasOctGes( )
{
    return (m_octGes != "");
}

bool AttNoteGes::HasPnameGes( )
{
    return (m_pnameGes != "");
}

bool AttNoteGes::HasPnum( )
{
    return (m_pnum != "");
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
        element.remove_attribute("headshape");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteVis::WriteNoteVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasHeadshape()) {
        element.append_attribute("headshape") = StrToStr(this->GetHeadshape()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNoteVis::HasHeadshape( )
{
    return (m_headshape != "");
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
    m_oct = 0;
}

bool AttOctave::ReadOctave(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("oct")) {
        this->SetOct(StrToInt(element.attribute("oct").value()));
        element.remove_attribute("oct");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctave::WriteOctave(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasOct()) {
        element.append_attribute("oct") = IntToStr(this->GetOct()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOctave::HasOct( )
{
    return (m_oct != 0);
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
        element.remove_attribute("octave.default");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctavedefault::WriteOctavedefault(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasOctaveDefault()) {
        element.append_attribute("octave.default") = StrToStr(this->GetOctaveDefault()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOctavedefault::HasOctaveDefault( )
{
    return (m_octaveDefault != "");
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
        element.remove_attribute("dis");
        hasAttribute = true;
    }
    if (element.attribute("dis.place")) {
        this->SetDisPlace(StrToPlace(element.attribute("dis.place").value()));
        element.remove_attribute("dis.place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctavedisplacement::WriteOctavedisplacement(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDis()) {
        element.append_attribute("dis") = OctaveDisToStr(this->GetDis()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDisPlace()) {
        element.append_attribute("dis.place") = PlaceToStr(this->GetDisPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOctavedisplacement::HasDis( )
{
    return (m_dis != OCTAVE_DIS_NONE);
}

bool AttOctavedisplacement::HasDisPlace( )
{
    return (m_disPlace != PLACE_NONE);
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
        element.remove_attribute("ontheline");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOnelinestaff::WriteOnelinestaff(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasOntheline()) {
        element.append_attribute("ontheline") = StrToStr(this->GetOntheline()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOnelinestaff::HasOntheline( )
{
    return (m_ontheline != "");
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
    m_num = -1;
}

bool AttPadLog::ReadPadLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNum(StrToInt(element.attribute("num").value()));
        element.remove_attribute("num");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPadLog::WritePadLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasNum()) {
        element.append_attribute("num") = IntToStr(this->GetNum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPadLog::HasNum( )
{
    return (m_num != -1);
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
        element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPbVis::WritePbVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = StrToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPbVis::HasFunc( )
{
    return (m_func != "");
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
    m_pname = PITCHNAME_NONE;
}

bool AttPitch::ReadPitch(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("pname")) {
        this->SetPname(StrToPitchName(element.attribute("pname").value()));
        element.remove_attribute("pname");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPitch::WritePitch(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasPname()) {
        element.append_attribute("pname") = PitchNameToStr(this->GetPname()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPitch::HasPname( )
{
    return (m_pname != PITCHNAME_NONE);
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
        element.remove_attribute("place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPlacement::WritePlacement(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasPlace()) {
        element.append_attribute("place") = StrToStr(this->GetPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPlacement::HasPlace( )
{
    return (m_place != "");
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
        element.remove_attribute("plist");
        hasAttribute = true;
    }
    if (element.attribute("evaluate")) {
        this->SetEvaluate(StrToStr(element.attribute("evaluate").value()));
        element.remove_attribute("evaluate");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPlist::WritePlist(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasPlist()) {
        element.append_attribute("plist") = StrToStr(this->GetPlist()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEvaluate()) {
        element.append_attribute("evaluate") = StrToStr(this->GetEvaluate()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPlist::HasPlist( )
{
    return (m_plist != "");
}

bool AttPlist::HasEvaluate( )
{
    return (m_evaluate != "");
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
        element.remove_attribute("actuate");
        hasAttribute = true;
    }
    if (element.attribute("role")) {
        this->SetRole(StrToStr(element.attribute("role").value()));
        element.remove_attribute("role");
        hasAttribute = true;
    }
    if (element.attribute("show")) {
        this->SetShow(StrToStr(element.attribute("show").value()));
        element.remove_attribute("show");
        hasAttribute = true;
    }
    if (element.attribute("target")) {
        this->SetTarget(StrToStr(element.attribute("target").value()));
        element.remove_attribute("target");
        hasAttribute = true;
    }
    if (element.attribute("targettype")) {
        this->SetTargettype(StrToStr(element.attribute("targettype").value()));
        element.remove_attribute("targettype");
        hasAttribute = true;
    }
    if (element.attribute("title")) {
        this->SetTitle(StrToStr(element.attribute("title").value()));
        element.remove_attribute("title");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPointing::WritePointing(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasActuate()) {
        element.append_attribute("actuate") = StrToStr(this->GetActuate()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRole()) {
        element.append_attribute("role") = StrToStr(this->GetRole()).c_str();
        wroteAttribute = true;
    }
    if (this->HasShow()) {
        element.append_attribute("show") = StrToStr(this->GetShow()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTarget()) {
        element.append_attribute("target") = StrToStr(this->GetTarget()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTargettype()) {
        element.append_attribute("targettype") = StrToStr(this->GetTargettype()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTitle()) {
        element.append_attribute("title") = StrToStr(this->GetTitle()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPointing::HasActuate( )
{
    return (m_actuate != "");
}

bool AttPointing::HasRole( )
{
    return (m_role != "");
}

bool AttPointing::HasShow( )
{
    return (m_show != "");
}

bool AttPointing::HasTarget( )
{
    return (m_target != "");
}

bool AttPointing::HasTargettype( )
{
    return (m_targettype != "");
}

bool AttPointing::HasTitle( )
{
    return (m_title != "");
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
        element.remove_attribute("size");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRelativesize::WriteRelativesize(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasSize()) {
        element.append_attribute("size") = StrToStr(this->GetSize()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRelativesize::HasSize( )
{
    return (m_size != "");
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
        element.remove_attribute("resp");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttResponsibility::WriteResponsibility(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasResp()) {
        element.append_attribute("resp") = StrToStr(this->GetResp()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttResponsibility::HasResp( )
{
    return (m_resp != "");
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
    m_rend = BARRENDITION_single;
}

bool AttSbVis::ReadSbVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("rend")) {
        this->SetRend(StrToBarRendition(element.attribute("rend").value()));
        element.remove_attribute("rend");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSbVis::WriteSbVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRend()) {
        element.append_attribute("rend") = BarRenditionToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSbVis::HasRend( )
{
    return (m_rend != BARRENDITION_single);
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
        element.remove_attribute("scale");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScalable::WriteScalable(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasScale()) {
        element.append_attribute("scale") = StrToStr(this->GetScale()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttScalable::HasScale( )
{
    return (m_scale != "");
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
        element.remove_attribute("tune.pname");
        hasAttribute = true;
    }
    if (element.attribute("tune.Hz")) {
        this->SetTuneHz(StrToDbl(element.attribute("tune.Hz").value()));
        element.remove_attribute("tune.Hz");
        hasAttribute = true;
    }
    if (element.attribute("tune.temper")) {
        this->SetTuneTemper(StrToStr(element.attribute("tune.temper").value()));
        element.remove_attribute("tune.temper");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScoreDefGes::WriteScoreDefGes(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTunePname()) {
        element.append_attribute("tune.pname") = StrToStr(this->GetTunePname()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTuneHz()) {
        element.append_attribute("tune.Hz") = DblToStr(this->GetTuneHz()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTuneTemper()) {
        element.append_attribute("tune.temper") = StrToStr(this->GetTuneTemper()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttScoreDefGes::HasTunePname( )
{
    return (m_tunePname != "");
}

bool AttScoreDefGes::HasTuneHz( )
{
    return (m_tuneHzDbl != 0.0);
}

bool AttScoreDefGes::HasTuneTemper( )
{
    return (m_tuneTemper != "");
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
    m_musicSize = "";
    m_optimize = "";
    m_pageHeight = "";
    m_pageWidth = "";
    m_pageTopmar = "";
    m_pageBotmar = "";
    m_pageLeftmar = "";
    m_pageRightmar = "";
    m_pagePanels = "";
    m_pageScale = "";
    m_spacingPackexpDbl = 0.0;
    m_spacingPackfactDbl = 0.0;
    m_spacingStaff = "";
    m_spacingSystem = "";
    m_systemLeftmar = "";
    m_systemRightmar = "";
    m_systemTopmar = "";
    m_vuHeight = "";
}

bool AttScoreDefVis::ReadScoreDefVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("ending.rend")) {
        this->SetEndingRend(StrToStr(element.attribute("ending.rend").value()));
        element.remove_attribute("ending.rend");
        hasAttribute = true;
    }
    if (element.attribute("mnum.visible")) {
        this->SetMnumVisible(StrToStr(element.attribute("mnum.visible").value()));
        element.remove_attribute("mnum.visible");
        hasAttribute = true;
    }
    if (element.attribute("music.name")) {
        this->SetMusicName(StrToStr(element.attribute("music.name").value()));
        element.remove_attribute("music.name");
        hasAttribute = true;
    }
    if (element.attribute("music.size")) {
        this->SetMusicSize(StrToStr(element.attribute("music.size").value()));
        element.remove_attribute("music.size");
        hasAttribute = true;
    }
    if (element.attribute("optimize")) {
        this->SetOptimize(StrToStr(element.attribute("optimize").value()));
        element.remove_attribute("optimize");
        hasAttribute = true;
    }
    if (element.attribute("page.height")) {
        this->SetPageHeight(StrToStr(element.attribute("page.height").value()));
        element.remove_attribute("page.height");
        hasAttribute = true;
    }
    if (element.attribute("page.width")) {
        this->SetPageWidth(StrToStr(element.attribute("page.width").value()));
        element.remove_attribute("page.width");
        hasAttribute = true;
    }
    if (element.attribute("page.topmar")) {
        this->SetPageTopmar(StrToStr(element.attribute("page.topmar").value()));
        element.remove_attribute("page.topmar");
        hasAttribute = true;
    }
    if (element.attribute("page.botmar")) {
        this->SetPageBotmar(StrToStr(element.attribute("page.botmar").value()));
        element.remove_attribute("page.botmar");
        hasAttribute = true;
    }
    if (element.attribute("page.leftmar")) {
        this->SetPageLeftmar(StrToStr(element.attribute("page.leftmar").value()));
        element.remove_attribute("page.leftmar");
        hasAttribute = true;
    }
    if (element.attribute("page.rightmar")) {
        this->SetPageRightmar(StrToStr(element.attribute("page.rightmar").value()));
        element.remove_attribute("page.rightmar");
        hasAttribute = true;
    }
    if (element.attribute("page.panels")) {
        this->SetPagePanels(StrToStr(element.attribute("page.panels").value()));
        element.remove_attribute("page.panels");
        hasAttribute = true;
    }
    if (element.attribute("page.scale")) {
        this->SetPageScale(StrToStr(element.attribute("page.scale").value()));
        element.remove_attribute("page.scale");
        hasAttribute = true;
    }
    if (element.attribute("spacing.packexp")) {
        this->SetSpacingPackexp(StrToDbl(element.attribute("spacing.packexp").value()));
        element.remove_attribute("spacing.packexp");
        hasAttribute = true;
    }
    if (element.attribute("spacing.packfact")) {
        this->SetSpacingPackfact(StrToDbl(element.attribute("spacing.packfact").value()));
        element.remove_attribute("spacing.packfact");
        hasAttribute = true;
    }
    if (element.attribute("spacing.staff")) {
        this->SetSpacingStaff(StrToStr(element.attribute("spacing.staff").value()));
        element.remove_attribute("spacing.staff");
        hasAttribute = true;
    }
    if (element.attribute("spacing.system")) {
        this->SetSpacingSystem(StrToStr(element.attribute("spacing.system").value()));
        element.remove_attribute("spacing.system");
        hasAttribute = true;
    }
    if (element.attribute("system.leftmar")) {
        this->SetSystemLeftmar(StrToStr(element.attribute("system.leftmar").value()));
        element.remove_attribute("system.leftmar");
        hasAttribute = true;
    }
    if (element.attribute("system.rightmar")) {
        this->SetSystemRightmar(StrToStr(element.attribute("system.rightmar").value()));
        element.remove_attribute("system.rightmar");
        hasAttribute = true;
    }
    if (element.attribute("system.topmar")) {
        this->SetSystemTopmar(StrToStr(element.attribute("system.topmar").value()));
        element.remove_attribute("system.topmar");
        hasAttribute = true;
    }
    if (element.attribute("vu.height")) {
        this->SetVuHeight(StrToStr(element.attribute("vu.height").value()));
        element.remove_attribute("vu.height");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScoreDefVis::WriteScoreDefVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasEndingRend()) {
        element.append_attribute("ending.rend") = StrToStr(this->GetEndingRend()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMnumVisible()) {
        element.append_attribute("mnum.visible") = StrToStr(this->GetMnumVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMusicName()) {
        element.append_attribute("music.name") = StrToStr(this->GetMusicName()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMusicSize()) {
        element.append_attribute("music.size") = StrToStr(this->GetMusicSize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOptimize()) {
        element.append_attribute("optimize") = StrToStr(this->GetOptimize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageHeight()) {
        element.append_attribute("page.height") = StrToStr(this->GetPageHeight()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageWidth()) {
        element.append_attribute("page.width") = StrToStr(this->GetPageWidth()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageTopmar()) {
        element.append_attribute("page.topmar") = StrToStr(this->GetPageTopmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageBotmar()) {
        element.append_attribute("page.botmar") = StrToStr(this->GetPageBotmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageLeftmar()) {
        element.append_attribute("page.leftmar") = StrToStr(this->GetPageLeftmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageRightmar()) {
        element.append_attribute("page.rightmar") = StrToStr(this->GetPageRightmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPagePanels()) {
        element.append_attribute("page.panels") = StrToStr(this->GetPagePanels()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageScale()) {
        element.append_attribute("page.scale") = StrToStr(this->GetPageScale()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSpacingPackexp()) {
        element.append_attribute("spacing.packexp") = DblToStr(this->GetSpacingPackexp()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSpacingPackfact()) {
        element.append_attribute("spacing.packfact") = DblToStr(this->GetSpacingPackfact()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSpacingStaff()) {
        element.append_attribute("spacing.staff") = StrToStr(this->GetSpacingStaff()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSpacingSystem()) {
        element.append_attribute("spacing.system") = StrToStr(this->GetSpacingSystem()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSystemLeftmar()) {
        element.append_attribute("system.leftmar") = StrToStr(this->GetSystemLeftmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSystemRightmar()) {
        element.append_attribute("system.rightmar") = StrToStr(this->GetSystemRightmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSystemTopmar()) {
        element.append_attribute("system.topmar") = StrToStr(this->GetSystemTopmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasVuHeight()) {
        element.append_attribute("vu.height") = StrToStr(this->GetVuHeight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttScoreDefVis::HasEndingRend( )
{
    return (m_endingRend != "");
}

bool AttScoreDefVis::HasMnumVisible( )
{
    return (m_mnumVisible != "");
}

bool AttScoreDefVis::HasMusicName( )
{
    return (m_musicName != "");
}

bool AttScoreDefVis::HasMusicSize( )
{
    return (m_musicSize != "");
}

bool AttScoreDefVis::HasOptimize( )
{
    return (m_optimize != "");
}

bool AttScoreDefVis::HasPageHeight( )
{
    return (m_pageHeight != "");
}

bool AttScoreDefVis::HasPageWidth( )
{
    return (m_pageWidth != "");
}

bool AttScoreDefVis::HasPageTopmar( )
{
    return (m_pageTopmar != "");
}

bool AttScoreDefVis::HasPageBotmar( )
{
    return (m_pageBotmar != "");
}

bool AttScoreDefVis::HasPageLeftmar( )
{
    return (m_pageLeftmar != "");
}

bool AttScoreDefVis::HasPageRightmar( )
{
    return (m_pageRightmar != "");
}

bool AttScoreDefVis::HasPagePanels( )
{
    return (m_pagePanels != "");
}

bool AttScoreDefVis::HasPageScale( )
{
    return (m_pageScale != "");
}

bool AttScoreDefVis::HasSpacingPackexp( )
{
    return (m_spacingPackexpDbl != 0.0);
}

bool AttScoreDefVis::HasSpacingPackfact( )
{
    return (m_spacingPackfactDbl != 0.0);
}

bool AttScoreDefVis::HasSpacingStaff( )
{
    return (m_spacingStaff != "");
}

bool AttScoreDefVis::HasSpacingSystem( )
{
    return (m_spacingSystem != "");
}

bool AttScoreDefVis::HasSystemLeftmar( )
{
    return (m_systemLeftmar != "");
}

bool AttScoreDefVis::HasSystemRightmar( )
{
    return (m_systemRightmar != "");
}

bool AttScoreDefVis::HasSystemTopmar( )
{
    return (m_systemTopmar != "");
}

bool AttScoreDefVis::HasVuHeight( )
{
    return (m_vuHeight != "");
}


/* include <attvu.height> */

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
        element.remove_attribute("restart");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSectionVis::WriteSectionVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasRestart()) {
        element.append_attribute("restart") = StrToStr(this->GetRestart()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSectionVis::HasRestart( )
{
    return (m_restart != "");
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
        this->SetSeq(StrToInt(element.attribute("seq").value()));
        element.remove_attribute("seq");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSequence::WriteSequence(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasSeq()) {
        element.append_attribute("seq") = IntToStr(this->GetSeq()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSequence::HasSeq( )
{
    return (m_seqInt != 0);
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
        element.remove_attribute("slash");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSlashcount::WriteSlashcount(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasSlash()) {
        element.append_attribute("slash") = IntToStr(this->GetSlash()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSlashcount::HasSlash( )
{
    return (m_slash != 0);
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
        element.remove_attribute("slur");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSlurpresent::WriteSlurpresent(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasSlur()) {
        element.append_attribute("slur") = StrToStr(this->GetSlur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSlurpresent::HasSlur( )
{
    return (m_slur != "");
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
        element.remove_attribute("compressable");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSpaceVis::WriteSpaceVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasCompressable()) {
        element.append_attribute("compressable") = StrToStr(this->GetCompressable()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSpaceVis::HasCompressable( )
{
    return (m_compressable != "");
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
        element.remove_attribute("def");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffLog::WriteStaffLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDef()) {
        element.append_attribute("def") = StrToStr(this->GetDef()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffLog::HasDef( )
{
    return (m_def != "");
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
    m_gridShow = BOOLEAN_NONE;
    m_layerscheme = LAYERSCHEME_NONE;
    m_linesInt = 0;
    m_linesColor = "";
    m_linesVisible = BOOLEAN_NONE;
    m_spacing = "";
}

bool AttStaffDefVis::ReadStaffDefVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("grid.show")) {
        this->SetGridShow(StrToBool(element.attribute("grid.show").value()));
        element.remove_attribute("grid.show");
        hasAttribute = true;
    }
    if (element.attribute("layerscheme")) {
        this->SetLayerscheme(StrToLayerscheme(element.attribute("layerscheme").value()));
        element.remove_attribute("layerscheme");
        hasAttribute = true;
    }
    if (element.attribute("lines")) {
        this->SetLines(StrToInt(element.attribute("lines").value()));
        element.remove_attribute("lines");
        hasAttribute = true;
    }
    if (element.attribute("lines.color")) {
        this->SetLinesColor(StrToStr(element.attribute("lines.color").value()));
        element.remove_attribute("lines.color");
        hasAttribute = true;
    }
    if (element.attribute("lines.visible")) {
        this->SetLinesVisible(StrToBool(element.attribute("lines.visible").value()));
        element.remove_attribute("lines.visible");
        hasAttribute = true;
    }
    if (element.attribute("spacing")) {
        this->SetSpacing(StrToStr(element.attribute("spacing").value()));
        element.remove_attribute("spacing");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffDefVis::WriteStaffDefVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasGridShow()) {
        element.append_attribute("grid.show") = BoolToStr(this->GetGridShow()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLayerscheme()) {
        element.append_attribute("layerscheme") = LayerschemeToStr(this->GetLayerscheme()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLines()) {
        element.append_attribute("lines") = IntToStr(this->GetLines()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLinesColor()) {
        element.append_attribute("lines.color") = StrToStr(this->GetLinesColor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLinesVisible()) {
        element.append_attribute("lines.visible") = BoolToStr(this->GetLinesVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSpacing()) {
        element.append_attribute("spacing") = StrToStr(this->GetSpacing()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffDefVis::HasGridShow( )
{
    return (m_gridShow != BOOLEAN_NONE);
}

bool AttStaffDefVis::HasLayerscheme( )
{
    return (m_layerscheme != LAYERSCHEME_NONE);
}

bool AttStaffDefVis::HasLines( )
{
    return (m_linesInt != 0);
}

bool AttStaffDefVis::HasLinesColor( )
{
    return (m_linesColor != "");
}

bool AttStaffDefVis::HasLinesVisible( )
{
    return (m_linesVisible != BOOLEAN_NONE);
}

bool AttStaffDefVis::HasSpacing( )
{
    return (m_spacing != "");
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
        element.remove_attribute("barthru");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffGrpVis::WriteStaffGrpVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasBarthru()) {
        element.append_attribute("barthru") = StrToStr(this->GetBarthru()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffGrpVis::HasBarthru( )
{
    return (m_barthru != "");
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
        element.remove_attribute("symbol");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffgroupingsym::WriteStaffgroupingsym(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasSymbol()) {
        element.append_attribute("symbol") = StrToStr(this->GetSymbol()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffgroupingsym::HasSymbol( )
{
    return (m_symbol != "");
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
    m_staff = 0;
}

bool AttStaffident::ReadStaffident(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("staff")) {
        this->SetStaff(StrToInt(element.attribute("staff").value()));
        element.remove_attribute("staff");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffident::WriteStaffident(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasStaff()) {
        element.append_attribute("staff") = IntToStr(this->GetStaff()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffident::HasStaff( )
{
    return (m_staff != 0);
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
        element.remove_attribute("loc");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffloc::WriteStaffloc(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasLoc()) {
        element.append_attribute("loc") = StrToStr(this->GetLoc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffloc::HasLoc( )
{
    return (m_loc != "");
}


/* include <attloc> */

//----------------------------------------------------------------------------
// AttStafflocPitched
//----------------------------------------------------------------------------

AttStafflocPitched::AttStafflocPitched(): Att() {
    ResetStafflocPitched();
}

AttStafflocPitched::~AttStafflocPitched() {

}

void AttStafflocPitched::ResetStafflocPitched() {
    m_ploc = PITCHNAME_NONE;
    m_oloc = 0;
}

bool AttStafflocPitched::ReadStafflocPitched(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("ploc")) {
        this->SetPloc(StrToPitchName(element.attribute("ploc").value()));
        element.remove_attribute("ploc");
        hasAttribute = true;
    }
    if (element.attribute("oloc")) {
        this->SetOloc(StrToInt(element.attribute("oloc").value()));
        element.remove_attribute("oloc");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStafflocPitched::WriteStafflocPitched(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasPloc()) {
        element.append_attribute("ploc") = PitchNameToStr(this->GetPloc()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOloc()) {
        element.append_attribute("oloc") = IntToStr(this->GetOloc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStafflocPitched::HasPloc( )
{
    return (m_ploc != PITCHNAME_NONE);
}

bool AttStafflocPitched::HasOloc( )
{
    return (m_oloc != 0);
}


/* include <attoloc> */

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
        element.remove_attribute("endid");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStartendid::WriteStartendid(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasEndid()) {
        element.append_attribute("endid") = StrToStr(this->GetEndid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStartendid::HasEndid( )
{
    return (m_endid != "");
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
        element.remove_attribute("startid");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStartid::WriteStartid(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasStartid()) {
        element.append_attribute("startid") = StrToStr(this->GetStartid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStartid::HasStartid( )
{
    return (m_startid != "");
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
    m_stemDir = STEMDIRECTION_NONE;
    m_stemLen = 0;
    m_stemPos = STEMPOSITION_NONE;
    m_stemX = 0;
    m_stemY = 0;
}

bool AttStemmed::ReadStemmed(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("stem.dir")) {
        this->SetStemDir(StrToStemDirection(element.attribute("stem.dir").value()));
        element.remove_attribute("stem.dir");
        hasAttribute = true;
    }
    if (element.attribute("stem.len")) {
        this->SetStemLen(StrToInt(element.attribute("stem.len").value()));
        element.remove_attribute("stem.len");
        hasAttribute = true;
    }
    if (element.attribute("stem.pos")) {
        this->SetStemPos(StrToStemPosition(element.attribute("stem.pos").value()));
        element.remove_attribute("stem.pos");
        hasAttribute = true;
    }
    if (element.attribute("stem.x")) {
        this->SetStemX(StrToInt(element.attribute("stem.x").value()));
        element.remove_attribute("stem.x");
        hasAttribute = true;
    }
    if (element.attribute("stem.y")) {
        this->SetStemY(StrToInt(element.attribute("stem.y").value()));
        element.remove_attribute("stem.y");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStemmed::WriteStemmed(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasStemDir()) {
        element.append_attribute("stem.dir") = StemDirectionToStr(this->GetStemDir()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStemLen()) {
        element.append_attribute("stem.len") = IntToStr(this->GetStemLen()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStemPos()) {
        element.append_attribute("stem.pos") = StemPositionToStr(this->GetStemPos()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStemX()) {
        element.append_attribute("stem.x") = IntToStr(this->GetStemX()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStemY()) {
        element.append_attribute("stem.y") = IntToStr(this->GetStemY()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStemmed::HasStemDir( )
{
    return (m_stemDir != STEMDIRECTION_NONE);
}

bool AttStemmed::HasStemLen( )
{
    return (m_stemLen != 0);
}

bool AttStemmed::HasStemPos( )
{
    return (m_stemPos != STEMPOSITION_NONE);
}

bool AttStemmed::HasStemX( )
{
    return (m_stemX != 0);
}

bool AttStemmed::HasStemY( )
{
    return (m_stemY != 0);
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
    m_con = CON_NONE;
    m_wordpos = WORDPOS_NONE;
}

bool AttSylLog::ReadSylLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("con")) {
        this->SetCon(StrToCon(element.attribute("con").value()));
        element.remove_attribute("con");
        hasAttribute = true;
    }
    if (element.attribute("wordpos")) {
        this->SetWordpos(StrToWordPos(element.attribute("wordpos").value()));
        element.remove_attribute("wordpos");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSylLog::WriteSylLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasCon()) {
        element.append_attribute("con") = ConToStr(this->GetCon()).c_str();
        wroteAttribute = true;
    }
    if (this->HasWordpos()) {
        element.append_attribute("wordpos") = WordPosToStr(this->GetWordpos()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSylLog::HasCon( )
{
    return (m_con != CON_NONE);
}

bool AttSylLog::HasWordpos( )
{
    return (m_wordpos != WORDPOS_NONE);
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
        element.remove_attribute("syl");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSyltext::WriteSyltext(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasSyl()) {
        element.append_attribute("syl") = StrToStr(this->GetSyl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSyltext::HasSyl( )
{
    return (m_syl != "");
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
    m_textSize = "";
    m_textStyle = "";
    m_textWeight = "";
}

bool AttTextstyle::ReadTextstyle(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("text.fam")) {
        this->SetTextFam(StrToStr(element.attribute("text.fam").value()));
        element.remove_attribute("text.fam");
        hasAttribute = true;
    }
    if (element.attribute("text.name")) {
        this->SetTextName(StrToStr(element.attribute("text.name").value()));
        element.remove_attribute("text.name");
        hasAttribute = true;
    }
    if (element.attribute("text.size")) {
        this->SetTextSize(StrToStr(element.attribute("text.size").value()));
        element.remove_attribute("text.size");
        hasAttribute = true;
    }
    if (element.attribute("text.style")) {
        this->SetTextStyle(StrToStr(element.attribute("text.style").value()));
        element.remove_attribute("text.style");
        hasAttribute = true;
    }
    if (element.attribute("text.weight")) {
        this->SetTextWeight(StrToStr(element.attribute("text.weight").value()));
        element.remove_attribute("text.weight");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTextstyle::WriteTextstyle(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTextFam()) {
        element.append_attribute("text.fam") = StrToStr(this->GetTextFam()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTextName()) {
        element.append_attribute("text.name") = StrToStr(this->GetTextName()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTextSize()) {
        element.append_attribute("text.size") = StrToStr(this->GetTextSize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTextStyle()) {
        element.append_attribute("text.style") = StrToStr(this->GetTextStyle()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTextWeight()) {
        element.append_attribute("text.weight") = StrToStr(this->GetTextWeight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTextstyle::HasTextFam( )
{
    return (m_textFam != "");
}

bool AttTextstyle::HasTextName( )
{
    return (m_textName != "");
}

bool AttTextstyle::HasTextSize( )
{
    return (m_textSize != "");
}

bool AttTextstyle::HasTextStyle( )
{
    return (m_textStyle != "");
}

bool AttTextstyle::HasTextWeight( )
{
    return (m_textWeight != "");
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
    m_tie = TIE_NONE;
}

bool AttTiepresent::ReadTiepresent(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("tie")) {
        this->SetTie(StrToTie(element.attribute("tie").value()));
        element.remove_attribute("tie");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTiepresent::WriteTiepresent(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTie()) {
        element.append_attribute("tie") = TieToStr(this->GetTie()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTiepresent::HasTie( )
{
    return (m_tie != TIE_NONE);
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
    m_tstamp = -1.0;
}

bool AttTimestampMusical::ReadTimestampMusical(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("tstamp")) {
        this->SetTstamp(StrToDbl(element.attribute("tstamp").value()));
        element.remove_attribute("tstamp");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestampMusical::WriteTimestampMusical(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTstamp()) {
        element.append_attribute("tstamp") = DblToStr(this->GetTstamp()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimestampMusical::HasTstamp( )
{
    return (m_tstamp != -1.0);
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
        this->SetTstampGes(StrToInt(element.attribute("tstamp.ges").value()));
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

bool AttTimestampPerformed::WriteTimestampPerformed(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTstampGes()) {
        element.append_attribute("tstamp.ges") = IntToStr(this->GetTstampGes()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTstampReal()) {
        element.append_attribute("tstamp.real") = StrToStr(this->GetTstampReal()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimestampPerformed::HasTstampGes( )
{
    return (m_tstampGesInt != 0);
}

bool AttTimestampPerformed::HasTstampReal( )
{
    return (m_tstampReal != "");
}


/* include <atttstamp.real> */

//----------------------------------------------------------------------------
// AttTimestamp2Musical
//----------------------------------------------------------------------------

AttTimestamp2Musical::AttTimestamp2Musical(): Att() {
    ResetTimestamp2Musical();
}

AttTimestamp2Musical::~AttTimestamp2Musical() {

}

void AttTimestamp2Musical::ResetTimestamp2Musical() {
    m_tstamp2 = std::make_pair(-1,-1.0);
}

bool AttTimestamp2Musical::ReadTimestamp2Musical(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("tstamp2")) {
        this->SetTstamp2(StrToTstamp2(element.attribute("tstamp2").value()));
        element.remove_attribute("tstamp2");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestamp2Musical::WriteTimestamp2Musical(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTstamp2()) {
        element.append_attribute("tstamp2") = Tstamp2ToStr(this->GetTstamp2()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimestamp2Musical::HasTstamp2( )
{
    return (m_tstamp2 != std::make_pair(-1,-1.0));
}


/* include <atttstamp2> */

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
        this->SetTransDiat(StrToDbl(element.attribute("trans.diat").value()));
        element.remove_attribute("trans.diat");
        hasAttribute = true;
    }
    if (element.attribute("trans.semi")) {
        this->SetTransSemi(StrToDbl(element.attribute("trans.semi").value()));
        element.remove_attribute("trans.semi");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTransposition::WriteTransposition(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTransDiat()) {
        element.append_attribute("trans.diat") = DblToStr(this->GetTransDiat()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTransSemi()) {
        element.append_attribute("trans.semi") = DblToStr(this->GetTransSemi()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTransposition::HasTransDiat( )
{
    return (m_transDiatDbl != 0.0);
}

bool AttTransposition::HasTransSemi( )
{
    return (m_transSemiDbl != 0.0);
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
        element.remove_attribute("tuplet");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTupletpresent::WriteTupletpresent(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTuplet()) {
        element.append_attribute("tuplet") = StrToStr(this->GetTuplet()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTupletpresent::HasTuplet( )
{
    return (m_tuplet != "");
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
        element.remove_attribute("type");
        hasAttribute = true;
    }
    if (element.attribute("subtype")) {
        this->SetSubtype(StrToStr(element.attribute("subtype").value()));
        element.remove_attribute("subtype");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTyped::WriteTyped(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasType()) {
        element.append_attribute("type") = StrToStr(this->GetType()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSubtype()) {
        element.append_attribute("subtype") = StrToStr(this->GetSubtype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTyped::HasType( )
{
    return (m_type != "");
}

bool AttTyped::HasSubtype( )
{
    return (m_subtype != "");
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
    m_fontstyle = FONTSTYLE_NONE;
    m_fontweight = FONTWEIGHT_NONE;
}

bool AttTypography::ReadTypography(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("fontfam")) {
        this->SetFontfam(StrToStr(element.attribute("fontfam").value()));
        element.remove_attribute("fontfam");
        hasAttribute = true;
    }
    if (element.attribute("fontname")) {
        this->SetFontname(StrToStr(element.attribute("fontname").value()));
        element.remove_attribute("fontname");
        hasAttribute = true;
    }
    if (element.attribute("fontsize")) {
        this->SetFontsize(StrToDbl(element.attribute("fontsize").value()));
        element.remove_attribute("fontsize");
        hasAttribute = true;
    }
    if (element.attribute("fontstyle")) {
        this->SetFontstyle(StrToFontstyle(element.attribute("fontstyle").value()));
        element.remove_attribute("fontstyle");
        hasAttribute = true;
    }
    if (element.attribute("fontweight")) {
        this->SetFontweight(StrToFontweight(element.attribute("fontweight").value()));
        element.remove_attribute("fontweight");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTypography::WriteTypography(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasFontfam()) {
        element.append_attribute("fontfam") = StrToStr(this->GetFontfam()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFontname()) {
        element.append_attribute("fontname") = StrToStr(this->GetFontname()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFontsize()) {
        element.append_attribute("fontsize") = DblToStr(this->GetFontsize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFontstyle()) {
        element.append_attribute("fontstyle") = FontstyleToStr(this->GetFontstyle()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFontweight()) {
        element.append_attribute("fontweight") = FontweightToStr(this->GetFontweight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTypography::HasFontfam( )
{
    return (m_fontfam != "");
}

bool AttTypography::HasFontname( )
{
    return (m_fontname != "");
}

bool AttTypography::HasFontsize( )
{
    return (m_fontsizeDbl != 0.0);
}

bool AttTypography::HasFontstyle( )
{
    return (m_fontstyle != FONTSTYLE_NONE);
}

bool AttTypography::HasFontweight( )
{
    return (m_fontweight != FONTWEIGHT_NONE);
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
        element.remove_attribute("visible");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisibility::WriteVisibility(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasVisible()) {
        element.append_attribute("visible") = StrToStr(this->GetVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisibility::HasVisible( )
{
    return (m_visible != "");
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
        element.remove_attribute("ho");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffsetHo::WriteVisualoffsetHo(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasHo()) {
        element.append_attribute("ho") = StrToStr(this->GetHo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualoffsetHo::HasHo( )
{
    return (m_ho != "");
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
        element.remove_attribute("to");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffsetTo::WriteVisualoffsetTo(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasTo()) {
        element.append_attribute("to") = StrToStr(this->GetTo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualoffsetTo::HasTo( )
{
    return (m_to != "");
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
        element.remove_attribute("vo");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffsetVo::WriteVisualoffsetVo(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasVo()) {
        element.append_attribute("vo") = StrToStr(this->GetVo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualoffsetVo::HasVo( )
{
    return (m_vo != "");
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
        element.remove_attribute("startho");
        hasAttribute = true;
    }
    if (element.attribute("endho")) {
        this->SetEndho(StrToStr(element.attribute("endho").value()));
        element.remove_attribute("endho");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffset2Ho::WriteVisualoffset2Ho(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasStartho()) {
        element.append_attribute("startho") = StrToStr(this->GetStartho()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndho()) {
        element.append_attribute("endho") = StrToStr(this->GetEndho()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualoffset2Ho::HasStartho( )
{
    return (m_startho != "");
}

bool AttVisualoffset2Ho::HasEndho( )
{
    return (m_endho != "");
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
        element.remove_attribute("startto");
        hasAttribute = true;
    }
    if (element.attribute("endto")) {
        this->SetEndto(StrToStr(element.attribute("endto").value()));
        element.remove_attribute("endto");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffset2To::WriteVisualoffset2To(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasStartto()) {
        element.append_attribute("startto") = StrToStr(this->GetStartto()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndto()) {
        element.append_attribute("endto") = StrToStr(this->GetEndto()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualoffset2To::HasStartto( )
{
    return (m_startto != "");
}

bool AttVisualoffset2To::HasEndto( )
{
    return (m_endto != "");
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
        element.remove_attribute("startvo");
        hasAttribute = true;
    }
    if (element.attribute("endvo")) {
        this->SetEndvo(StrToStr(element.attribute("endvo").value()));
        element.remove_attribute("endvo");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualoffset2Vo::WriteVisualoffset2Vo(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasStartvo()) {
        element.append_attribute("startvo") = StrToStr(this->GetStartvo()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndvo()) {
        element.append_attribute("endvo") = StrToStr(this->GetEndvo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualoffset2Vo::HasStartvo( )
{
    return (m_startvo != "");
}

bool AttVisualoffset2Vo::HasEndvo( )
{
    return (m_endvo != "");
}


/* include <attendvo> */

//----------------------------------------------------------------------------
// AttWhitespace
//----------------------------------------------------------------------------

AttWhitespace::AttWhitespace(): Att() {
    ResetWhitespace();
}

AttWhitespace::~AttWhitespace() {

}

void AttWhitespace::ResetWhitespace() {
    m_space = "";
}

bool AttWhitespace::ReadWhitespace(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("space")) {
        this->SetSpace(StrToStr(element.attribute("space").value()));
        element.remove_attribute("space");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttWhitespace::WriteWhitespace(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasSpace()) {
        element.append_attribute("space") = StrToStr(this->GetSpace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttWhitespace::HasSpace( )
{
    return (m_space != "");
}


/* include <attspace> */

//----------------------------------------------------------------------------
// AttWidth
//----------------------------------------------------------------------------

AttWidth::AttWidth(): Att() {
    ResetWidth();
}

AttWidth::~AttWidth() {

}

void AttWidth::ResetWidth() {
    m_width = "";
}

bool AttWidth::ReadWidth(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("width")) {
        this->SetWidth(StrToStr(element.attribute("width").value()));
        element.remove_attribute("width");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttWidth::WriteWidth(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasWidth()) {
        element.append_attribute("width") = StrToStr(this->GetWidth()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttWidth::HasWidth( )
{
    return (m_width != "");
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
        this->SetX(StrToDbl(element.attribute("x").value()));
        element.remove_attribute("x");
        hasAttribute = true;
    }
    if (element.attribute("y")) {
        this->SetY(StrToDbl(element.attribute("y").value()));
        element.remove_attribute("y");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttXy::WriteXy(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasX()) {
        element.append_attribute("x") = DblToStr(this->GetX()).c_str();
        wroteAttribute = true;
    }
    if (this->HasY()) {
        element.append_attribute("y") = DblToStr(this->GetY()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttXy::HasX( )
{
    return (m_xDbl != 0.0);
}

bool AttXy::HasY( )
{
    return (m_yDbl != 0.0);
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
        this->SetX2(StrToDbl(element.attribute("x2").value()));
        element.remove_attribute("x2");
        hasAttribute = true;
    }
    if (element.attribute("y2")) {
        this->SetY2(StrToDbl(element.attribute("y2").value()));
        element.remove_attribute("y2");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttXy2::WriteXy2(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasX2()) {
        element.append_attribute("x2") = DblToStr(this->GetX2()).c_str();
        wroteAttribute = true;
    }
    if (this->HasY2()) {
        element.append_attribute("y2") = DblToStr(this->GetY2()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttXy2::HasX2( )
{
    return (m_x2Dbl != 0.0);
}

bool AttXy2::HasY2( )
{
    return (m_y2Dbl != 0.0);
}


/* include <atty2> */

bool Att::SetShared( Object *element, std::string attrType, std::string attrValue ) {
    if (dynamic_cast<AttAccidLog*>(element) ) {
        AttAccidLog *att = dynamic_cast<AttAccidLog*>(element);
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttAccidental*>(element) ) {
        AttAccidental *att = dynamic_cast<AttAccidental*>(element);
        if (attrType == "accid") {
            att->SetAccid(att->StrToAccidentalExplicit(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttAccidentalPerformed*>(element) ) {
        AttAccidentalPerformed *att = dynamic_cast<AttAccidentalPerformed*>(element);
        if (attrType == "accidGes") {
            att->SetAccidGes(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttAltsym*>(element) ) {
        AttAltsym *att = dynamic_cast<AttAltsym*>(element);
        if (attrType == "altsym") {
            att->SetAltsym(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttArticulation*>(element) ) {
        AttArticulation *att = dynamic_cast<AttArticulation*>(element);
        if (attrType == "artic") {
            att->SetArtic(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttArticulationPerformed*>(element) ) {
        AttArticulationPerformed *att = dynamic_cast<AttArticulationPerformed*>(element);
        if (attrType == "articGes") {
            att->SetArticGes(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttAugmentdots*>(element) ) {
        AttAugmentdots *att = dynamic_cast<AttAugmentdots*>(element);
        if (attrType == "dots") {
            att->SetDots(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttAuthorized*>(element) ) {
        AttAuthorized *att = dynamic_cast<AttAuthorized*>(element);
        if (attrType == "authority") {
            att->SetAuthority(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "authURI") {
            att->SetAuthURI(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttBarLineLog*>(element) ) {
        AttBarLineLog *att = dynamic_cast<AttBarLineLog*>(element);
        if (attrType == "rend") {
            att->SetRend(att->StrToBarRendition(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttBarplacement*>(element) ) {
        AttBarplacement *att = dynamic_cast<AttBarplacement*>(element);
        if (attrType == "barplace") {
            att->SetBarplace(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "taktplace") {
            att->SetTaktplace(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttBeamingVis*>(element) ) {
        AttBeamingVis *att = dynamic_cast<AttBeamingVis*>(element);
        if (attrType == "beamColor") {
            att->SetBeamColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "beamRend") {
            att->SetBeamRend(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "beamSlopeDbl") {
            att->SetBeamSlope(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttBibl*>(element) ) {
        AttBibl *att = dynamic_cast<AttBibl*>(element);
        if (attrType == "analog") {
            att->SetAnalog(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttCalendared*>(element) ) {
        AttCalendared *att = dynamic_cast<AttCalendared*>(element);
        if (attrType == "calendar") {
            att->SetCalendar(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttCanonical*>(element) ) {
        AttCanonical *att = dynamic_cast<AttCanonical*>(element);
        if (attrType == "dbkey") {
            att->SetDbkey(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttChordVis*>(element) ) {
        AttChordVis *att = dynamic_cast<AttChordVis*>(element);
        if (attrType == "cluster") {
            att->SetCluster(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttClefLog*>(element) ) {
        AttClefLog *att = dynamic_cast<AttClefLog*>(element);
        if (attrType == "cautionary") {
            att->SetCautionary(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttCleffingLog*>(element) ) {
        AttCleffingLog *att = dynamic_cast<AttCleffingLog*>(element);
        if (attrType == "clefShape") {
            att->SetClefShape(att->StrToClefShape(attrValue));
            return true;
        }
        if (attrType == "clefLine") {
            att->SetClefLine(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "clefDis") {
            att->SetClefDis(att->StrToOctaveDis(attrValue));
            return true;
        }
        if (attrType == "clefDisPlace") {
            att->SetClefDisPlace(att->StrToPlace(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttCleffingVis*>(element) ) {
        AttCleffingVis *att = dynamic_cast<AttCleffingVis*>(element);
        if (attrType == "clefColor") {
            att->SetClefColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "clefVisible") {
            att->SetClefVisible(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttClefshape*>(element) ) {
        AttClefshape *att = dynamic_cast<AttClefshape*>(element);
        if (attrType == "shape") {
            att->SetShape(att->StrToClefShape(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttCoded*>(element) ) {
        AttCoded *att = dynamic_cast<AttCoded*>(element);
        if (attrType == "code") {
            att->SetCode(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttColor*>(element) ) {
        AttColor *att = dynamic_cast<AttColor*>(element);
        if (attrType == "color") {
            att->SetColor(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttColoration*>(element) ) {
        AttColoration *att = dynamic_cast<AttColoration*>(element);
        if (attrType == "colored") {
            att->SetColored(att->StrToBool(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttCommon*>(element) ) {
        AttCommon *att = dynamic_cast<AttCommon*>(element);
        if (attrType == "label") {
            att->SetLabel(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "n") {
            att->SetN(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "base") {
            att->SetBase(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttCoordinated*>(element) ) {
        AttCoordinated *att = dynamic_cast<AttCoordinated*>(element);
        if (attrType == "ulxInt") {
            att->SetUlx(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "ulyInt") {
            att->SetUly(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "lrxInt") {
            att->SetLrx(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "lryInt") {
            att->SetLry(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttCurvature*>(element) ) {
        AttCurvature *att = dynamic_cast<AttCurvature*>(element);
        if (attrType == "bezier") {
            att->SetBezier(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "bulge") {
            att->SetBulge(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "curvedir") {
            att->SetCurvedir(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttCurverend*>(element) ) {
        AttCurverend *att = dynamic_cast<AttCurverend*>(element);
        if (attrType == "rend") {
            att->SetRend(att->StrToBarRendition(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttCustosLog*>(element) ) {
        AttCustosLog *att = dynamic_cast<AttCustosLog*>(element);
        if (attrType == "target") {
            att->SetTarget(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttDatable*>(element) ) {
        AttDatable *att = dynamic_cast<AttDatable*>(element);
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
    if (dynamic_cast<AttDatapointing*>(element) ) {
        AttDatapointing *att = dynamic_cast<AttDatapointing*>(element);
        if (attrType == "data") {
            att->SetData(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttDeclaring*>(element) ) {
        AttDeclaring *att = dynamic_cast<AttDeclaring*>(element);
        if (attrType == "decls") {
            att->SetDecls(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttDistances*>(element) ) {
        AttDistances *att = dynamic_cast<AttDistances*>(element);
        if (attrType == "dynamDist") {
            att->SetDynamDist(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "harmDist") {
            att->SetHarmDist(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "textDist") {
            att->SetTextDist(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttDotLog*>(element) ) {
        AttDotLog *att = dynamic_cast<AttDotLog*>(element);
        if (attrType == "form") {
            att->SetForm(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttDurationAdditive*>(element) ) {
        AttDurationAdditive *att = dynamic_cast<AttDurationAdditive*>(element);
        if (attrType == "dur") {
            att->SetDur(att->StrToDur(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttDurationDefault*>(element) ) {
        AttDurationDefault *att = dynamic_cast<AttDurationDefault*>(element);
        if (attrType == "durDefault") {
            att->SetDurDefault(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "numDefaultInt") {
            att->SetNumDefault(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "numbaseDefaultInt") {
            att->SetNumbaseDefault(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttDurationMusical*>(element) ) {
        AttDurationMusical *att = dynamic_cast<AttDurationMusical*>(element);
        if (attrType == "dur") {
            att->SetDur(att->StrToDur(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttDurationPerformed*>(element) ) {
        AttDurationPerformed *att = dynamic_cast<AttDurationPerformed*>(element);
        if (attrType == "durGes") {
            att->SetDurGes(att->StrToDur(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttDurationRatio*>(element) ) {
        AttDurationRatio *att = dynamic_cast<AttDurationRatio*>(element);
        if (attrType == "num") {
            att->SetNum(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "numbase") {
            att->SetNumbase(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttEnclosingchars*>(element) ) {
        AttEnclosingchars *att = dynamic_cast<AttEnclosingchars*>(element);
        if (attrType == "enclose") {
            att->SetEnclose(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttFermatapresent*>(element) ) {
        AttFermatapresent *att = dynamic_cast<AttFermatapresent*>(element);
        if (attrType == "fermata") {
            att->SetFermata(att->StrToPlace(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttHandident*>(element) ) {
        AttHandident *att = dynamic_cast<AttHandident*>(element);
        if (attrType == "hand") {
            att->SetHand(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttHorizontalalign*>(element) ) {
        AttHorizontalalign *att = dynamic_cast<AttHorizontalalign*>(element);
        if (attrType == "halign") {
            att->SetHalign(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttInstrumentident*>(element) ) {
        AttInstrumentident *att = dynamic_cast<AttInstrumentident*>(element);
        if (attrType == "instr") {
            att->SetInstr(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttInternetmedia*>(element) ) {
        AttInternetmedia *att = dynamic_cast<AttInternetmedia*>(element);
        if (attrType == "mimetype") {
            att->SetMimetype(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttJoined*>(element) ) {
        AttJoined *att = dynamic_cast<AttJoined*>(element);
        if (attrType == "join") {
            att->SetJoin(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttKeySigLog*>(element) ) {
        AttKeySigLog *att = dynamic_cast<AttKeySigLog*>(element);
        if (attrType == "mode") {
            att->SetMode(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttKeySigDefaultLog*>(element) ) {
        AttKeySigDefaultLog *att = dynamic_cast<AttKeySigDefaultLog*>(element);
        if (attrType == "keyAccid") {
            att->SetKeyAccid(att->StrToAccidentalImplicit(attrValue));
            return true;
        }
        if (attrType == "keyMode") {
            att->SetKeyMode(att->StrToMode(attrValue));
            return true;
        }
        if (attrType == "keyPname") {
            att->SetKeyPname(att->StrToPitchName(attrValue));
            return true;
        }
        if (attrType == "keySig") {
            att->SetKeySig(att->StrToKeySignature(attrValue));
            return true;
        }
        if (attrType == "keySigMixed") {
            att->SetKeySigMixed(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttKeySigDefaultVis*>(element) ) {
        AttKeySigDefaultVis *att = dynamic_cast<AttKeySigDefaultVis*>(element);
        if (attrType == "keySigShow") {
            att->SetKeySigShow(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "keySigShowchange") {
            att->SetKeySigShowchange(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttLabelsAddl*>(element) ) {
        AttLabelsAddl *att = dynamic_cast<AttLabelsAddl*>(element);
        if (attrType == "labelAbbr") {
            att->SetLabelAbbr(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttLang*>(element) ) {
        AttLang *att = dynamic_cast<AttLang*>(element);
        if (attrType == "lang") {
            att->SetLang(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttLayerLog*>(element) ) {
        AttLayerLog *att = dynamic_cast<AttLayerLog*>(element);
        if (attrType == "def") {
            att->SetDef(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttLayerident*>(element) ) {
        AttLayerident *att = dynamic_cast<AttLayerident*>(element);
        if (attrType == "layer") {
            att->SetLayer(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttLineloc*>(element) ) {
        AttLineloc *att = dynamic_cast<AttLineloc*>(element);
        if (attrType == "line") {
            att->SetLine(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttLinerend*>(element) ) {
        AttLinerend *att = dynamic_cast<AttLinerend*>(element);
        if (attrType == "rend") {
            att->SetRend(att->StrToBarRendition(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttLyricstyle*>(element) ) {
        AttLyricstyle *att = dynamic_cast<AttLyricstyle*>(element);
        if (attrType == "lyricAlign") {
            att->SetLyricAlign(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "lyricFam") {
            att->SetLyricFam(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "lyricName") {
            att->SetLyricName(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "lyricSize") {
            att->SetLyricSize(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "lyricStyle") {
            att->SetLyricStyle(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "lyricWeight") {
            att->SetLyricWeight(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMeasureLog*>(element) ) {
        AttMeasureLog *att = dynamic_cast<AttMeasureLog*>(element);
        if (attrType == "left") {
            att->SetLeft(att->StrToBarRendition(attrValue));
            return true;
        }
        if (attrType == "right") {
            att->SetRight(att->StrToBarRendition(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMeasurement*>(element) ) {
        AttMeasurement *att = dynamic_cast<AttMeasurement*>(element);
        if (attrType == "unit") {
            att->SetUnit(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMediabounds*>(element) ) {
        AttMediabounds *att = dynamic_cast<AttMediabounds*>(element);
        if (attrType == "begin") {
            att->SetBegin(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "end") {
            att->SetEnd(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "betype") {
            att->SetBetype(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMedium*>(element) ) {
        AttMedium *att = dynamic_cast<AttMedium*>(element);
        if (attrType == "medium") {
            att->SetMedium(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMeiversion*>(element) ) {
        AttMeiversion *att = dynamic_cast<AttMeiversion*>(element);
        if (attrType == "meiversion") {
            att->SetMeiversion(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "meiversionNum") {
            att->SetMeiversionNum(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMensurLog*>(element) ) {
        AttMensurLog *att = dynamic_cast<AttMensurLog*>(element);
        if (attrType == "dot") {
            att->SetDot(att->StrToBool(attrValue));
            return true;
        }
        if (attrType == "sign") {
            att->SetSign(att->StrToMensurationSign(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMeterSigLog*>(element) ) {
        AttMeterSigLog *att = dynamic_cast<AttMeterSigLog*>(element);
        if (attrType == "count") {
            att->SetCount(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "sym") {
            att->SetSym(att->StrToMeterSign(attrValue));
            return true;
        }
        if (attrType == "unit") {
            att->SetUnit(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMeterSigVis*>(element) ) {
        AttMeterSigVis *att = dynamic_cast<AttMeterSigVis*>(element);
        if (attrType == "rend") {
            att->SetRend(att->StrToBarRendition(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMeterSigDefaultLog*>(element) ) {
        AttMeterSigDefaultLog *att = dynamic_cast<AttMeterSigDefaultLog*>(element);
        if (attrType == "meterCount") {
            att->SetMeterCount(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "meterUnit") {
            att->SetMeterUnit(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMeterSigDefaultVis*>(element) ) {
        AttMeterSigDefaultVis *att = dynamic_cast<AttMeterSigDefaultVis*>(element);
        if (attrType == "meterRend") {
            att->SetMeterRend(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "meterShowchange") {
            att->SetMeterShowchange(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "meterSym") {
            att->SetMeterSym(att->StrToMeterSign(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMeterconformance*>(element) ) {
        AttMeterconformance *att = dynamic_cast<AttMeterconformance*>(element);
        if (attrType == "metcon") {
            att->SetMetcon(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMeterconformanceBar*>(element) ) {
        AttMeterconformanceBar *att = dynamic_cast<AttMeterconformanceBar*>(element);
        if (attrType == "metcon") {
            att->SetMetcon(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "control") {
            att->SetControl(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMmtempo*>(element) ) {
        AttMmtempo *att = dynamic_cast<AttMmtempo*>(element);
        if (attrType == "mm") {
            att->SetMm(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "mmUnit") {
            att->SetMmUnit(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "mmDots") {
            att->SetMmDots(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttMultinummeasures*>(element) ) {
        AttMultinummeasures *att = dynamic_cast<AttMultinummeasures*>(element);
        if (attrType == "multiNumber") {
            att->SetMultiNumber(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttName*>(element) ) {
        AttName *att = dynamic_cast<AttName*>(element);
        if (attrType == "nymref") {
            att->SetNymref(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "role") {
            att->SetRole(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttNoteGes*>(element) ) {
        AttNoteGes *att = dynamic_cast<AttNoteGes*>(element);
        if (attrType == "octGes") {
            att->SetOctGes(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pnameGes") {
            att->SetPnameGes(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pnum") {
            att->SetPnum(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttNoteVis*>(element) ) {
        AttNoteVis *att = dynamic_cast<AttNoteVis*>(element);
        if (attrType == "headshape") {
            att->SetHeadshape(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttOctave*>(element) ) {
        AttOctave *att = dynamic_cast<AttOctave*>(element);
        if (attrType == "oct") {
            att->SetOct(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttOctavedefault*>(element) ) {
        AttOctavedefault *att = dynamic_cast<AttOctavedefault*>(element);
        if (attrType == "octaveDefault") {
            att->SetOctaveDefault(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttOctavedisplacement*>(element) ) {
        AttOctavedisplacement *att = dynamic_cast<AttOctavedisplacement*>(element);
        if (attrType == "dis") {
            att->SetDis(att->StrToOctaveDis(attrValue));
            return true;
        }
        if (attrType == "disPlace") {
            att->SetDisPlace(att->StrToPlace(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttOnelinestaff*>(element) ) {
        AttOnelinestaff *att = dynamic_cast<AttOnelinestaff*>(element);
        if (attrType == "ontheline") {
            att->SetOntheline(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttPadLog*>(element) ) {
        AttPadLog *att = dynamic_cast<AttPadLog*>(element);
        if (attrType == "num") {
            att->SetNum(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttPbVis*>(element) ) {
        AttPbVis *att = dynamic_cast<AttPbVis*>(element);
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttPitch*>(element) ) {
        AttPitch *att = dynamic_cast<AttPitch*>(element);
        if (attrType == "pname") {
            att->SetPname(att->StrToPitchName(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttPlacement*>(element) ) {
        AttPlacement *att = dynamic_cast<AttPlacement*>(element);
        if (attrType == "place") {
            att->SetPlace(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttPlist*>(element) ) {
        AttPlist *att = dynamic_cast<AttPlist*>(element);
        if (attrType == "plist") {
            att->SetPlist(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "evaluate") {
            att->SetEvaluate(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttPointing*>(element) ) {
        AttPointing *att = dynamic_cast<AttPointing*>(element);
        if (attrType == "actuate") {
            att->SetActuate(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "role") {
            att->SetRole(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "show") {
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
        if (attrType == "title") {
            att->SetTitle(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttRelativesize*>(element) ) {
        AttRelativesize *att = dynamic_cast<AttRelativesize*>(element);
        if (attrType == "size") {
            att->SetSize(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttResponsibility*>(element) ) {
        AttResponsibility *att = dynamic_cast<AttResponsibility*>(element);
        if (attrType == "resp") {
            att->SetResp(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttSbVis*>(element) ) {
        AttSbVis *att = dynamic_cast<AttSbVis*>(element);
        if (attrType == "rend") {
            att->SetRend(att->StrToBarRendition(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttScalable*>(element) ) {
        AttScalable *att = dynamic_cast<AttScalable*>(element);
        if (attrType == "scale") {
            att->SetScale(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttScoreDefGes*>(element) ) {
        AttScoreDefGes *att = dynamic_cast<AttScoreDefGes*>(element);
        if (attrType == "tunePname") {
            att->SetTunePname(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "tuneHzDbl") {
            att->SetTuneHz(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "tuneTemper") {
            att->SetTuneTemper(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttScoreDefVis*>(element) ) {
        AttScoreDefVis *att = dynamic_cast<AttScoreDefVis*>(element);
        if (attrType == "endingRend") {
            att->SetEndingRend(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "mnumVisible") {
            att->SetMnumVisible(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "musicName") {
            att->SetMusicName(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "musicSize") {
            att->SetMusicSize(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "optimize") {
            att->SetOptimize(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pageHeight") {
            att->SetPageHeight(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pageWidth") {
            att->SetPageWidth(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pageTopmar") {
            att->SetPageTopmar(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pageBotmar") {
            att->SetPageBotmar(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pageLeftmar") {
            att->SetPageLeftmar(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pageRightmar") {
            att->SetPageRightmar(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pagePanels") {
            att->SetPagePanels(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "pageScale") {
            att->SetPageScale(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "spacingPackexpDbl") {
            att->SetSpacingPackexp(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "spacingPackfactDbl") {
            att->SetSpacingPackfact(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "spacingStaff") {
            att->SetSpacingStaff(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "spacingSystem") {
            att->SetSpacingSystem(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "systemLeftmar") {
            att->SetSystemLeftmar(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "systemRightmar") {
            att->SetSystemRightmar(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "systemTopmar") {
            att->SetSystemTopmar(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "vuHeight") {
            att->SetVuHeight(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttSectionVis*>(element) ) {
        AttSectionVis *att = dynamic_cast<AttSectionVis*>(element);
        if (attrType == "restart") {
            att->SetRestart(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttSequence*>(element) ) {
        AttSequence *att = dynamic_cast<AttSequence*>(element);
        if (attrType == "seqInt") {
            att->SetSeq(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttSlashcount*>(element) ) {
        AttSlashcount *att = dynamic_cast<AttSlashcount*>(element);
        if (attrType == "slash") {
            att->SetSlash(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttSlurpresent*>(element) ) {
        AttSlurpresent *att = dynamic_cast<AttSlurpresent*>(element);
        if (attrType == "slur") {
            att->SetSlur(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttSpaceVis*>(element) ) {
        AttSpaceVis *att = dynamic_cast<AttSpaceVis*>(element);
        if (attrType == "compressable") {
            att->SetCompressable(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttStaffLog*>(element) ) {
        AttStaffLog *att = dynamic_cast<AttStaffLog*>(element);
        if (attrType == "def") {
            att->SetDef(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttStaffDefVis*>(element) ) {
        AttStaffDefVis *att = dynamic_cast<AttStaffDefVis*>(element);
        if (attrType == "gridShow") {
            att->SetGridShow(att->StrToBool(attrValue));
            return true;
        }
        if (attrType == "layerscheme") {
            att->SetLayerscheme(att->StrToLayerscheme(attrValue));
            return true;
        }
        if (attrType == "linesInt") {
            att->SetLines(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "linesColor") {
            att->SetLinesColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "linesVisible") {
            att->SetLinesVisible(att->StrToBool(attrValue));
            return true;
        }
        if (attrType == "spacing") {
            att->SetSpacing(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttStaffGrpVis*>(element) ) {
        AttStaffGrpVis *att = dynamic_cast<AttStaffGrpVis*>(element);
        if (attrType == "barthru") {
            att->SetBarthru(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttStaffgroupingsym*>(element) ) {
        AttStaffgroupingsym *att = dynamic_cast<AttStaffgroupingsym*>(element);
        if (attrType == "symbol") {
            att->SetSymbol(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttStaffident*>(element) ) {
        AttStaffident *att = dynamic_cast<AttStaffident*>(element);
        if (attrType == "staff") {
            att->SetStaff(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttStaffloc*>(element) ) {
        AttStaffloc *att = dynamic_cast<AttStaffloc*>(element);
        if (attrType == "loc") {
            att->SetLoc(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttStafflocPitched*>(element) ) {
        AttStafflocPitched *att = dynamic_cast<AttStafflocPitched*>(element);
        if (attrType == "ploc") {
            att->SetPloc(att->StrToPitchName(attrValue));
            return true;
        }
        if (attrType == "oloc") {
            att->SetOloc(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttStartendid*>(element) ) {
        AttStartendid *att = dynamic_cast<AttStartendid*>(element);
        if (attrType == "endid") {
            att->SetEndid(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttStartid*>(element) ) {
        AttStartid *att = dynamic_cast<AttStartid*>(element);
        if (attrType == "startid") {
            att->SetStartid(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttStemmed*>(element) ) {
        AttStemmed *att = dynamic_cast<AttStemmed*>(element);
        if (attrType == "stemDir") {
            att->SetStemDir(att->StrToStemDirection(attrValue));
            return true;
        }
        if (attrType == "stemLen") {
            att->SetStemLen(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "stemPos") {
            att->SetStemPos(att->StrToStemPosition(attrValue));
            return true;
        }
        if (attrType == "stemX") {
            att->SetStemX(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "stemY") {
            att->SetStemY(att->StrToInt(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttSylLog*>(element) ) {
        AttSylLog *att = dynamic_cast<AttSylLog*>(element);
        if (attrType == "con") {
            att->SetCon(att->StrToCon(attrValue));
            return true;
        }
        if (attrType == "wordpos") {
            att->SetWordpos(att->StrToWordPos(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttSyltext*>(element) ) {
        AttSyltext *att = dynamic_cast<AttSyltext*>(element);
        if (attrType == "syl") {
            att->SetSyl(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttTextstyle*>(element) ) {
        AttTextstyle *att = dynamic_cast<AttTextstyle*>(element);
        if (attrType == "textFam") {
            att->SetTextFam(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "textName") {
            att->SetTextName(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "textSize") {
            att->SetTextSize(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "textStyle") {
            att->SetTextStyle(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "textWeight") {
            att->SetTextWeight(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttTiepresent*>(element) ) {
        AttTiepresent *att = dynamic_cast<AttTiepresent*>(element);
        if (attrType == "tie") {
            att->SetTie(att->StrToTie(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttTimestampMusical*>(element) ) {
        AttTimestampMusical *att = dynamic_cast<AttTimestampMusical*>(element);
        if (attrType == "tstamp") {
            att->SetTstamp(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttTimestampPerformed*>(element) ) {
        AttTimestampPerformed *att = dynamic_cast<AttTimestampPerformed*>(element);
        if (attrType == "tstampGesInt") {
            att->SetTstampGes(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "tstampReal") {
            att->SetTstampReal(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttTimestamp2Musical*>(element) ) {
        AttTimestamp2Musical *att = dynamic_cast<AttTimestamp2Musical*>(element);
        if (attrType == "tstamp2") {
            att->SetTstamp2(att->StrToTstamp2(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttTransposition*>(element) ) {
        AttTransposition *att = dynamic_cast<AttTransposition*>(element);
        if (attrType == "transDiatDbl") {
            att->SetTransDiat(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "transSemiDbl") {
            att->SetTransSemi(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttTupletpresent*>(element) ) {
        AttTupletpresent *att = dynamic_cast<AttTupletpresent*>(element);
        if (attrType == "tuplet") {
            att->SetTuplet(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttTyped*>(element) ) {
        AttTyped *att = dynamic_cast<AttTyped*>(element);
        if (attrType == "type") {
            att->SetType(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "subtype") {
            att->SetSubtype(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttTypography*>(element) ) {
        AttTypography *att = dynamic_cast<AttTypography*>(element);
        if (attrType == "fontfam") {
            att->SetFontfam(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "fontname") {
            att->SetFontname(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "fontsizeDbl") {
            att->SetFontsize(att->StrToDbl(attrValue));
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
    }
    if (dynamic_cast<AttVisibility*>(element) ) {
        AttVisibility *att = dynamic_cast<AttVisibility*>(element);
        if (attrType == "visible") {
            att->SetVisible(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttVisualoffsetHo*>(element) ) {
        AttVisualoffsetHo *att = dynamic_cast<AttVisualoffsetHo*>(element);
        if (attrType == "ho") {
            att->SetHo(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttVisualoffsetTo*>(element) ) {
        AttVisualoffsetTo *att = dynamic_cast<AttVisualoffsetTo*>(element);
        if (attrType == "to") {
            att->SetTo(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttVisualoffsetVo*>(element) ) {
        AttVisualoffsetVo *att = dynamic_cast<AttVisualoffsetVo*>(element);
        if (attrType == "vo") {
            att->SetVo(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttVisualoffset2Ho*>(element) ) {
        AttVisualoffset2Ho *att = dynamic_cast<AttVisualoffset2Ho*>(element);
        if (attrType == "startho") {
            att->SetStartho(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "endho") {
            att->SetEndho(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttVisualoffset2To*>(element) ) {
        AttVisualoffset2To *att = dynamic_cast<AttVisualoffset2To*>(element);
        if (attrType == "startto") {
            att->SetStartto(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "endto") {
            att->SetEndto(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttVisualoffset2Vo*>(element) ) {
        AttVisualoffset2Vo *att = dynamic_cast<AttVisualoffset2Vo*>(element);
        if (attrType == "startvo") {
            att->SetStartvo(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "endvo") {
            att->SetEndvo(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttWhitespace*>(element) ) {
        AttWhitespace *att = dynamic_cast<AttWhitespace*>(element);
        if (attrType == "space") {
            att->SetSpace(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttWidth*>(element) ) {
        AttWidth *att = dynamic_cast<AttWidth*>(element);
        if (attrType == "width") {
            att->SetWidth(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttXy*>(element) ) {
        AttXy *att = dynamic_cast<AttXy*>(element);
        if (attrType == "xDbl") {
            att->SetX(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "yDbl") {
            att->SetY(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttXy2*>(element) ) {
        AttXy2 *att = dynamic_cast<AttXy2*>(element);
        if (attrType == "x2Dbl") {
            att->SetX2(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "y2Dbl") {
            att->SetY2(att->StrToDbl(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetShared( Object *element, ArrayOfStrAttr *attributes ) {
    if (dynamic_cast<AttAccidLog*>(element) ) {
        AttAccidLog *att = dynamic_cast<AttAccidLog*>(element);
        if (att->HasFunc()) {
            attributes->push_back(std::make_pair("func", att->StrToStr(att->GetFunc())));
        }
    }
    if (dynamic_cast<AttAccidental*>(element) ) {
        AttAccidental *att = dynamic_cast<AttAccidental*>(element);
        if (att->HasAccid()) {
            attributes->push_back(std::make_pair("accid", att->AccidentalExplicitToStr(att->GetAccid())));
        }
    }
    if (dynamic_cast<AttAccidentalPerformed*>(element) ) {
        AttAccidentalPerformed *att = dynamic_cast<AttAccidentalPerformed*>(element);
        if (att->HasAccidGes()) {
            attributes->push_back(std::make_pair("accidGes", att->StrToStr(att->GetAccidGes())));
        }
    }
    if (dynamic_cast<AttAltsym*>(element) ) {
        AttAltsym *att = dynamic_cast<AttAltsym*>(element);
        if (att->HasAltsym()) {
            attributes->push_back(std::make_pair("altsym", att->StrToStr(att->GetAltsym())));
        }
    }
    if (dynamic_cast<AttArticulation*>(element) ) {
        AttArticulation *att = dynamic_cast<AttArticulation*>(element);
        if (att->HasArtic()) {
            attributes->push_back(std::make_pair("artic", att->StrToStr(att->GetArtic())));
        }
    }
    if (dynamic_cast<AttArticulationPerformed*>(element) ) {
        AttArticulationPerformed *att = dynamic_cast<AttArticulationPerformed*>(element);
        if (att->HasArticGes()) {
            attributes->push_back(std::make_pair("articGes", att->StrToStr(att->GetArticGes())));
        }
    }
    if (dynamic_cast<AttAugmentdots*>(element) ) {
        AttAugmentdots *att = dynamic_cast<AttAugmentdots*>(element);
        if (att->HasDots()) {
            attributes->push_back(std::make_pair("dots", att->IntToStr(att->GetDots())));
        }
    }
    if (dynamic_cast<AttAuthorized*>(element) ) {
        AttAuthorized *att = dynamic_cast<AttAuthorized*>(element);
        if (att->HasAuthority()) {
            attributes->push_back(std::make_pair("authority", att->StrToStr(att->GetAuthority())));
        }
        if (att->HasAuthURI()) {
            attributes->push_back(std::make_pair("authURI", att->StrToStr(att->GetAuthURI())));
        }
    }
    if (dynamic_cast<AttBarLineLog*>(element) ) {
        AttBarLineLog *att = dynamic_cast<AttBarLineLog*>(element);
        if (att->HasRend()) {
            attributes->push_back(std::make_pair("rend", att->BarRenditionToStr(att->GetRend())));
        }
    }
    if (dynamic_cast<AttBarplacement*>(element) ) {
        AttBarplacement *att = dynamic_cast<AttBarplacement*>(element);
        if (att->HasBarplace()) {
            attributes->push_back(std::make_pair("barplace", att->StrToStr(att->GetBarplace())));
        }
        if (att->HasTaktplace()) {
            attributes->push_back(std::make_pair("taktplace", att->StrToStr(att->GetTaktplace())));
        }
    }
    if (dynamic_cast<AttBeamingVis*>(element) ) {
        AttBeamingVis *att = dynamic_cast<AttBeamingVis*>(element);
        if (att->HasBeamColor()) {
            attributes->push_back(std::make_pair("beamColor", att->StrToStr(att->GetBeamColor())));
        }
        if (att->HasBeamRend()) {
            attributes->push_back(std::make_pair("beamRend", att->StrToStr(att->GetBeamRend())));
        }
        if (att->HasBeamSlope()) {
            attributes->push_back(std::make_pair("beamSlopeDbl", att->DblToStr(att->GetBeamSlope())));
        }
    }
    if (dynamic_cast<AttBibl*>(element) ) {
        AttBibl *att = dynamic_cast<AttBibl*>(element);
        if (att->HasAnalog()) {
            attributes->push_back(std::make_pair("analog", att->StrToStr(att->GetAnalog())));
        }
    }
    if (dynamic_cast<AttCalendared*>(element) ) {
        AttCalendared *att = dynamic_cast<AttCalendared*>(element);
        if (att->HasCalendar()) {
            attributes->push_back(std::make_pair("calendar", att->StrToStr(att->GetCalendar())));
        }
    }
    if (dynamic_cast<AttCanonical*>(element) ) {
        AttCanonical *att = dynamic_cast<AttCanonical*>(element);
        if (att->HasDbkey()) {
            attributes->push_back(std::make_pair("dbkey", att->StrToStr(att->GetDbkey())));
        }
    }
    if (dynamic_cast<AttChordVis*>(element) ) {
        AttChordVis *att = dynamic_cast<AttChordVis*>(element);
        if (att->HasCluster()) {
            attributes->push_back(std::make_pair("cluster", att->StrToStr(att->GetCluster())));
        }
    }
    if (dynamic_cast<AttClefLog*>(element) ) {
        AttClefLog *att = dynamic_cast<AttClefLog*>(element);
        if (att->HasCautionary()) {
            attributes->push_back(std::make_pair("cautionary", att->StrToStr(att->GetCautionary())));
        }
    }
    if (dynamic_cast<AttCleffingLog*>(element) ) {
        AttCleffingLog *att = dynamic_cast<AttCleffingLog*>(element);
        if (att->HasClefShape()) {
            attributes->push_back(std::make_pair("clefShape", att->ClefShapeToStr(att->GetClefShape())));
        }
        if (att->HasClefLine()) {
            attributes->push_back(std::make_pair("clefLine", att->IntToStr(att->GetClefLine())));
        }
        if (att->HasClefDis()) {
            attributes->push_back(std::make_pair("clefDis", att->OctaveDisToStr(att->GetClefDis())));
        }
        if (att->HasClefDisPlace()) {
            attributes->push_back(std::make_pair("clefDisPlace", att->PlaceToStr(att->GetClefDisPlace())));
        }
    }
    if (dynamic_cast<AttCleffingVis*>(element) ) {
        AttCleffingVis *att = dynamic_cast<AttCleffingVis*>(element);
        if (att->HasClefColor()) {
            attributes->push_back(std::make_pair("clefColor", att->StrToStr(att->GetClefColor())));
        }
        if (att->HasClefVisible()) {
            attributes->push_back(std::make_pair("clefVisible", att->StrToStr(att->GetClefVisible())));
        }
    }
    if (dynamic_cast<AttClefshape*>(element) ) {
        AttClefshape *att = dynamic_cast<AttClefshape*>(element);
        if (att->HasShape()) {
            attributes->push_back(std::make_pair("shape", att->ClefShapeToStr(att->GetShape())));
        }
    }
    if (dynamic_cast<AttCoded*>(element) ) {
        AttCoded *att = dynamic_cast<AttCoded*>(element);
        if (att->HasCode()) {
            attributes->push_back(std::make_pair("code", att->StrToStr(att->GetCode())));
        }
    }
    if (dynamic_cast<AttColor*>(element) ) {
        AttColor *att = dynamic_cast<AttColor*>(element);
        if (att->HasColor()) {
            attributes->push_back(std::make_pair("color", att->StrToStr(att->GetColor())));
        }
    }
    if (dynamic_cast<AttColoration*>(element) ) {
        AttColoration *att = dynamic_cast<AttColoration*>(element);
        if (att->HasColored()) {
            attributes->push_back(std::make_pair("colored", att->BoolToStr(att->GetColored())));
        }
    }
    if (dynamic_cast<AttCommon*>(element) ) {
        AttCommon *att = dynamic_cast<AttCommon*>(element);
        if (att->HasLabel()) {
            attributes->push_back(std::make_pair("label", att->StrToStr(att->GetLabel())));
        }
        if (att->HasN()) {
            attributes->push_back(std::make_pair("n", att->IntToStr(att->GetN())));
        }
        if (att->HasBase()) {
            attributes->push_back(std::make_pair("base", att->StrToStr(att->GetBase())));
        }
    }
    if (dynamic_cast<AttCoordinated*>(element) ) {
        AttCoordinated *att = dynamic_cast<AttCoordinated*>(element);
        if (att->HasUlx()) {
            attributes->push_back(std::make_pair("ulxInt", att->IntToStr(att->GetUlx())));
        }
        if (att->HasUly()) {
            attributes->push_back(std::make_pair("ulyInt", att->IntToStr(att->GetUly())));
        }
        if (att->HasLrx()) {
            attributes->push_back(std::make_pair("lrxInt", att->IntToStr(att->GetLrx())));
        }
        if (att->HasLry()) {
            attributes->push_back(std::make_pair("lryInt", att->IntToStr(att->GetLry())));
        }
    }
    if (dynamic_cast<AttCurvature*>(element) ) {
        AttCurvature *att = dynamic_cast<AttCurvature*>(element);
        if (att->HasBezier()) {
            attributes->push_back(std::make_pair("bezier", att->StrToStr(att->GetBezier())));
        }
        if (att->HasBulge()) {
            attributes->push_back(std::make_pair("bulge", att->StrToStr(att->GetBulge())));
        }
        if (att->HasCurvedir()) {
            attributes->push_back(std::make_pair("curvedir", att->StrToStr(att->GetCurvedir())));
        }
    }
    if (dynamic_cast<AttCurverend*>(element) ) {
        AttCurverend *att = dynamic_cast<AttCurverend*>(element);
        if (att->HasRend()) {
            attributes->push_back(std::make_pair("rend", att->BarRenditionToStr(att->GetRend())));
        }
    }
    if (dynamic_cast<AttCustosLog*>(element) ) {
        AttCustosLog *att = dynamic_cast<AttCustosLog*>(element);
        if (att->HasTarget()) {
            attributes->push_back(std::make_pair("target", att->StrToStr(att->GetTarget())));
        }
    }
    if (dynamic_cast<AttDatable*>(element) ) {
        AttDatable *att = dynamic_cast<AttDatable*>(element);
        if (att->HasEnddate()) {
            attributes->push_back(std::make_pair("enddate", att->StrToStr(att->GetEnddate())));
        }
        if (att->HasIsodate()) {
            attributes->push_back(std::make_pair("isodate", att->StrToStr(att->GetIsodate())));
        }
        if (att->HasNotafter()) {
            attributes->push_back(std::make_pair("notafter", att->StrToStr(att->GetNotafter())));
        }
        if (att->HasNotbefore()) {
            attributes->push_back(std::make_pair("notbefore", att->StrToStr(att->GetNotbefore())));
        }
        if (att->HasStartdate()) {
            attributes->push_back(std::make_pair("startdate", att->StrToStr(att->GetStartdate())));
        }
    }
    if (dynamic_cast<AttDatapointing*>(element) ) {
        AttDatapointing *att = dynamic_cast<AttDatapointing*>(element);
        if (att->HasData()) {
            attributes->push_back(std::make_pair("data", att->StrToStr(att->GetData())));
        }
    }
    if (dynamic_cast<AttDeclaring*>(element) ) {
        AttDeclaring *att = dynamic_cast<AttDeclaring*>(element);
        if (att->HasDecls()) {
            attributes->push_back(std::make_pair("decls", att->StrToStr(att->GetDecls())));
        }
    }
    if (dynamic_cast<AttDistances*>(element) ) {
        AttDistances *att = dynamic_cast<AttDistances*>(element);
        if (att->HasDynamDist()) {
            attributes->push_back(std::make_pair("dynamDist", att->StrToStr(att->GetDynamDist())));
        }
        if (att->HasHarmDist()) {
            attributes->push_back(std::make_pair("harmDist", att->StrToStr(att->GetHarmDist())));
        }
        if (att->HasTextDist()) {
            attributes->push_back(std::make_pair("textDist", att->StrToStr(att->GetTextDist())));
        }
    }
    if (dynamic_cast<AttDotLog*>(element) ) {
        AttDotLog *att = dynamic_cast<AttDotLog*>(element);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->StrToStr(att->GetForm())));
        }
    }
    if (dynamic_cast<AttDurationAdditive*>(element) ) {
        AttDurationAdditive *att = dynamic_cast<AttDurationAdditive*>(element);
        if (att->HasDur()) {
            attributes->push_back(std::make_pair("dur", att->DurToStr(att->GetDur())));
        }
    }
    if (dynamic_cast<AttDurationDefault*>(element) ) {
        AttDurationDefault *att = dynamic_cast<AttDurationDefault*>(element);
        if (att->HasDurDefault()) {
            attributes->push_back(std::make_pair("durDefault", att->StrToStr(att->GetDurDefault())));
        }
        if (att->HasNumDefault()) {
            attributes->push_back(std::make_pair("numDefaultInt", att->IntToStr(att->GetNumDefault())));
        }
        if (att->HasNumbaseDefault()) {
            attributes->push_back(std::make_pair("numbaseDefaultInt", att->IntToStr(att->GetNumbaseDefault())));
        }
    }
    if (dynamic_cast<AttDurationMusical*>(element) ) {
        AttDurationMusical *att = dynamic_cast<AttDurationMusical*>(element);
        if (att->HasDur()) {
            attributes->push_back(std::make_pair("dur", att->DurToStr(att->GetDur())));
        }
    }
    if (dynamic_cast<AttDurationPerformed*>(element) ) {
        AttDurationPerformed *att = dynamic_cast<AttDurationPerformed*>(element);
        if (att->HasDurGes()) {
            attributes->push_back(std::make_pair("durGes", att->DurToStr(att->GetDurGes())));
        }
    }
    if (dynamic_cast<AttDurationRatio*>(element) ) {
        AttDurationRatio *att = dynamic_cast<AttDurationRatio*>(element);
        if (att->HasNum()) {
            attributes->push_back(std::make_pair("num", att->IntToStr(att->GetNum())));
        }
        if (att->HasNumbase()) {
            attributes->push_back(std::make_pair("numbase", att->IntToStr(att->GetNumbase())));
        }
    }
    if (dynamic_cast<AttEnclosingchars*>(element) ) {
        AttEnclosingchars *att = dynamic_cast<AttEnclosingchars*>(element);
        if (att->HasEnclose()) {
            attributes->push_back(std::make_pair("enclose", att->StrToStr(att->GetEnclose())));
        }
    }
    if (dynamic_cast<AttFermatapresent*>(element) ) {
        AttFermatapresent *att = dynamic_cast<AttFermatapresent*>(element);
        if (att->HasFermata()) {
            attributes->push_back(std::make_pair("fermata", att->PlaceToStr(att->GetFermata())));
        }
    }
    if (dynamic_cast<AttHandident*>(element) ) {
        AttHandident *att = dynamic_cast<AttHandident*>(element);
        if (att->HasHand()) {
            attributes->push_back(std::make_pair("hand", att->StrToStr(att->GetHand())));
        }
    }
    if (dynamic_cast<AttHorizontalalign*>(element) ) {
        AttHorizontalalign *att = dynamic_cast<AttHorizontalalign*>(element);
        if (att->HasHalign()) {
            attributes->push_back(std::make_pair("halign", att->StrToStr(att->GetHalign())));
        }
    }
    if (dynamic_cast<AttInstrumentident*>(element) ) {
        AttInstrumentident *att = dynamic_cast<AttInstrumentident*>(element);
        if (att->HasInstr()) {
            attributes->push_back(std::make_pair("instr", att->StrToStr(att->GetInstr())));
        }
    }
    if (dynamic_cast<AttInternetmedia*>(element) ) {
        AttInternetmedia *att = dynamic_cast<AttInternetmedia*>(element);
        if (att->HasMimetype()) {
            attributes->push_back(std::make_pair("mimetype", att->StrToStr(att->GetMimetype())));
        }
    }
    if (dynamic_cast<AttJoined*>(element) ) {
        AttJoined *att = dynamic_cast<AttJoined*>(element);
        if (att->HasJoin()) {
            attributes->push_back(std::make_pair("join", att->StrToStr(att->GetJoin())));
        }
    }
    if (dynamic_cast<AttKeySigLog*>(element) ) {
        AttKeySigLog *att = dynamic_cast<AttKeySigLog*>(element);
        if (att->HasMode()) {
            attributes->push_back(std::make_pair("mode", att->StrToStr(att->GetMode())));
        }
    }
    if (dynamic_cast<AttKeySigDefaultLog*>(element) ) {
        AttKeySigDefaultLog *att = dynamic_cast<AttKeySigDefaultLog*>(element);
        if (att->HasKeyAccid()) {
            attributes->push_back(std::make_pair("keyAccid", att->AccidentalImplicitToStr(att->GetKeyAccid())));
        }
        if (att->HasKeyMode()) {
            attributes->push_back(std::make_pair("keyMode", att->ModeToStr(att->GetKeyMode())));
        }
        if (att->HasKeyPname()) {
            attributes->push_back(std::make_pair("keyPname", att->PitchNameToStr(att->GetKeyPname())));
        }
        if (att->HasKeySig()) {
            attributes->push_back(std::make_pair("keySig", att->KeySignatureToStr(att->GetKeySig())));
        }
        if (att->HasKeySigMixed()) {
            attributes->push_back(std::make_pair("keySigMixed", att->StrToStr(att->GetKeySigMixed())));
        }
    }
    if (dynamic_cast<AttKeySigDefaultVis*>(element) ) {
        AttKeySigDefaultVis *att = dynamic_cast<AttKeySigDefaultVis*>(element);
        if (att->HasKeySigShow()) {
            attributes->push_back(std::make_pair("keySigShow", att->StrToStr(att->GetKeySigShow())));
        }
        if (att->HasKeySigShowchange()) {
            attributes->push_back(std::make_pair("keySigShowchange", att->StrToStr(att->GetKeySigShowchange())));
        }
    }
    if (dynamic_cast<AttLabelsAddl*>(element) ) {
        AttLabelsAddl *att = dynamic_cast<AttLabelsAddl*>(element);
        if (att->HasLabelAbbr()) {
            attributes->push_back(std::make_pair("labelAbbr", att->StrToStr(att->GetLabelAbbr())));
        }
    }
    if (dynamic_cast<AttLang*>(element) ) {
        AttLang *att = dynamic_cast<AttLang*>(element);
        if (att->HasLang()) {
            attributes->push_back(std::make_pair("lang", att->StrToStr(att->GetLang())));
        }
    }
    if (dynamic_cast<AttLayerLog*>(element) ) {
        AttLayerLog *att = dynamic_cast<AttLayerLog*>(element);
        if (att->HasDef()) {
            attributes->push_back(std::make_pair("def", att->StrToStr(att->GetDef())));
        }
    }
    if (dynamic_cast<AttLayerident*>(element) ) {
        AttLayerident *att = dynamic_cast<AttLayerident*>(element);
        if (att->HasLayer()) {
            attributes->push_back(std::make_pair("layer", att->StrToStr(att->GetLayer())));
        }
    }
    if (dynamic_cast<AttLineloc*>(element) ) {
        AttLineloc *att = dynamic_cast<AttLineloc*>(element);
        if (att->HasLine()) {
            attributes->push_back(std::make_pair("line", att->IntToStr(att->GetLine())));
        }
    }
    if (dynamic_cast<AttLinerend*>(element) ) {
        AttLinerend *att = dynamic_cast<AttLinerend*>(element);
        if (att->HasRend()) {
            attributes->push_back(std::make_pair("rend", att->BarRenditionToStr(att->GetRend())));
        }
    }
    if (dynamic_cast<AttLyricstyle*>(element) ) {
        AttLyricstyle *att = dynamic_cast<AttLyricstyle*>(element);
        if (att->HasLyricAlign()) {
            attributes->push_back(std::make_pair("lyricAlign", att->StrToStr(att->GetLyricAlign())));
        }
        if (att->HasLyricFam()) {
            attributes->push_back(std::make_pair("lyricFam", att->StrToStr(att->GetLyricFam())));
        }
        if (att->HasLyricName()) {
            attributes->push_back(std::make_pair("lyricName", att->StrToStr(att->GetLyricName())));
        }
        if (att->HasLyricSize()) {
            attributes->push_back(std::make_pair("lyricSize", att->StrToStr(att->GetLyricSize())));
        }
        if (att->HasLyricStyle()) {
            attributes->push_back(std::make_pair("lyricStyle", att->StrToStr(att->GetLyricStyle())));
        }
        if (att->HasLyricWeight()) {
            attributes->push_back(std::make_pair("lyricWeight", att->StrToStr(att->GetLyricWeight())));
        }
    }
    if (dynamic_cast<AttMeasureLog*>(element) ) {
        AttMeasureLog *att = dynamic_cast<AttMeasureLog*>(element);
        if (att->HasLeft()) {
            attributes->push_back(std::make_pair("left", att->BarRenditionToStr(att->GetLeft())));
        }
        if (att->HasRight()) {
            attributes->push_back(std::make_pair("right", att->BarRenditionToStr(att->GetRight())));
        }
    }
    if (dynamic_cast<AttMeasurement*>(element) ) {
        AttMeasurement *att = dynamic_cast<AttMeasurement*>(element);
        if (att->HasUnit()) {
            attributes->push_back(std::make_pair("unit", att->IntToStr(att->GetUnit())));
        }
    }
    if (dynamic_cast<AttMediabounds*>(element) ) {
        AttMediabounds *att = dynamic_cast<AttMediabounds*>(element);
        if (att->HasBegin()) {
            attributes->push_back(std::make_pair("begin", att->StrToStr(att->GetBegin())));
        }
        if (att->HasEnd()) {
            attributes->push_back(std::make_pair("end", att->StrToStr(att->GetEnd())));
        }
        if (att->HasBetype()) {
            attributes->push_back(std::make_pair("betype", att->StrToStr(att->GetBetype())));
        }
    }
    if (dynamic_cast<AttMedium*>(element) ) {
        AttMedium *att = dynamic_cast<AttMedium*>(element);
        if (att->HasMedium()) {
            attributes->push_back(std::make_pair("medium", att->StrToStr(att->GetMedium())));
        }
    }
    if (dynamic_cast<AttMeiversion*>(element) ) {
        AttMeiversion *att = dynamic_cast<AttMeiversion*>(element);
        if (att->HasMeiversion()) {
            attributes->push_back(std::make_pair("meiversion", att->StrToStr(att->GetMeiversion())));
        }
        if (att->HasMeiversionNum()) {
            attributes->push_back(std::make_pair("meiversionNum", att->StrToStr(att->GetMeiversionNum())));
        }
    }
    if (dynamic_cast<AttMensurLog*>(element) ) {
        AttMensurLog *att = dynamic_cast<AttMensurLog*>(element);
        if (att->HasDot()) {
            attributes->push_back(std::make_pair("dot", att->BoolToStr(att->GetDot())));
        }
        if (att->HasSign()) {
            attributes->push_back(std::make_pair("sign", att->MensurationSignToStr(att->GetSign())));
        }
    }
    if (dynamic_cast<AttMeterSigLog*>(element) ) {
        AttMeterSigLog *att = dynamic_cast<AttMeterSigLog*>(element);
        if (att->HasCount()) {
            attributes->push_back(std::make_pair("count", att->IntToStr(att->GetCount())));
        }
        if (att->HasSym()) {
            attributes->push_back(std::make_pair("sym", att->MeterSignToStr(att->GetSym())));
        }
        if (att->HasUnit()) {
            attributes->push_back(std::make_pair("unit", att->IntToStr(att->GetUnit())));
        }
    }
    if (dynamic_cast<AttMeterSigVis*>(element) ) {
        AttMeterSigVis *att = dynamic_cast<AttMeterSigVis*>(element);
        if (att->HasRend()) {
            attributes->push_back(std::make_pair("rend", att->BarRenditionToStr(att->GetRend())));
        }
    }
    if (dynamic_cast<AttMeterSigDefaultLog*>(element) ) {
        AttMeterSigDefaultLog *att = dynamic_cast<AttMeterSigDefaultLog*>(element);
        if (att->HasMeterCount()) {
            attributes->push_back(std::make_pair("meterCount", att->IntToStr(att->GetMeterCount())));
        }
        if (att->HasMeterUnit()) {
            attributes->push_back(std::make_pair("meterUnit", att->IntToStr(att->GetMeterUnit())));
        }
    }
    if (dynamic_cast<AttMeterSigDefaultVis*>(element) ) {
        AttMeterSigDefaultVis *att = dynamic_cast<AttMeterSigDefaultVis*>(element);
        if (att->HasMeterRend()) {
            attributes->push_back(std::make_pair("meterRend", att->StrToStr(att->GetMeterRend())));
        }
        if (att->HasMeterShowchange()) {
            attributes->push_back(std::make_pair("meterShowchange", att->StrToStr(att->GetMeterShowchange())));
        }
        if (att->HasMeterSym()) {
            attributes->push_back(std::make_pair("meterSym", att->MeterSignToStr(att->GetMeterSym())));
        }
    }
    if (dynamic_cast<AttMeterconformance*>(element) ) {
        AttMeterconformance *att = dynamic_cast<AttMeterconformance*>(element);
        if (att->HasMetcon()) {
            attributes->push_back(std::make_pair("metcon", att->StrToStr(att->GetMetcon())));
        }
    }
    if (dynamic_cast<AttMeterconformanceBar*>(element) ) {
        AttMeterconformanceBar *att = dynamic_cast<AttMeterconformanceBar*>(element);
        if (att->HasMetcon()) {
            attributes->push_back(std::make_pair("metcon", att->StrToStr(att->GetMetcon())));
        }
        if (att->HasControl()) {
            attributes->push_back(std::make_pair("control", att->StrToStr(att->GetControl())));
        }
    }
    if (dynamic_cast<AttMmtempo*>(element) ) {
        AttMmtempo *att = dynamic_cast<AttMmtempo*>(element);
        if (att->HasMm()) {
            attributes->push_back(std::make_pair("mm", att->StrToStr(att->GetMm())));
        }
        if (att->HasMmUnit()) {
            attributes->push_back(std::make_pair("mmUnit", att->StrToStr(att->GetMmUnit())));
        }
        if (att->HasMmDots()) {
            attributes->push_back(std::make_pair("mmDots", att->StrToStr(att->GetMmDots())));
        }
    }
    if (dynamic_cast<AttMultinummeasures*>(element) ) {
        AttMultinummeasures *att = dynamic_cast<AttMultinummeasures*>(element);
        if (att->HasMultiNumber()) {
            attributes->push_back(std::make_pair("multiNumber", att->StrToStr(att->GetMultiNumber())));
        }
    }
    if (dynamic_cast<AttName*>(element) ) {
        AttName *att = dynamic_cast<AttName*>(element);
        if (att->HasNymref()) {
            attributes->push_back(std::make_pair("nymref", att->StrToStr(att->GetNymref())));
        }
        if (att->HasRole()) {
            attributes->push_back(std::make_pair("role", att->StrToStr(att->GetRole())));
        }
    }
    if (dynamic_cast<AttNoteGes*>(element) ) {
        AttNoteGes *att = dynamic_cast<AttNoteGes*>(element);
        if (att->HasOctGes()) {
            attributes->push_back(std::make_pair("octGes", att->StrToStr(att->GetOctGes())));
        }
        if (att->HasPnameGes()) {
            attributes->push_back(std::make_pair("pnameGes", att->StrToStr(att->GetPnameGes())));
        }
        if (att->HasPnum()) {
            attributes->push_back(std::make_pair("pnum", att->StrToStr(att->GetPnum())));
        }
    }
    if (dynamic_cast<AttNoteVis*>(element) ) {
        AttNoteVis *att = dynamic_cast<AttNoteVis*>(element);
        if (att->HasHeadshape()) {
            attributes->push_back(std::make_pair("headshape", att->StrToStr(att->GetHeadshape())));
        }
    }
    if (dynamic_cast<AttOctave*>(element) ) {
        AttOctave *att = dynamic_cast<AttOctave*>(element);
        if (att->HasOct()) {
            attributes->push_back(std::make_pair("oct", att->IntToStr(att->GetOct())));
        }
    }
    if (dynamic_cast<AttOctavedefault*>(element) ) {
        AttOctavedefault *att = dynamic_cast<AttOctavedefault*>(element);
        if (att->HasOctaveDefault()) {
            attributes->push_back(std::make_pair("octaveDefault", att->StrToStr(att->GetOctaveDefault())));
        }
    }
    if (dynamic_cast<AttOctavedisplacement*>(element) ) {
        AttOctavedisplacement *att = dynamic_cast<AttOctavedisplacement*>(element);
        if (att->HasDis()) {
            attributes->push_back(std::make_pair("dis", att->OctaveDisToStr(att->GetDis())));
        }
        if (att->HasDisPlace()) {
            attributes->push_back(std::make_pair("disPlace", att->PlaceToStr(att->GetDisPlace())));
        }
    }
    if (dynamic_cast<AttOnelinestaff*>(element) ) {
        AttOnelinestaff *att = dynamic_cast<AttOnelinestaff*>(element);
        if (att->HasOntheline()) {
            attributes->push_back(std::make_pair("ontheline", att->StrToStr(att->GetOntheline())));
        }
    }
    if (dynamic_cast<AttPadLog*>(element) ) {
        AttPadLog *att = dynamic_cast<AttPadLog*>(element);
        if (att->HasNum()) {
            attributes->push_back(std::make_pair("num", att->IntToStr(att->GetNum())));
        }
    }
    if (dynamic_cast<AttPbVis*>(element) ) {
        AttPbVis *att = dynamic_cast<AttPbVis*>(element);
        if (att->HasFunc()) {
            attributes->push_back(std::make_pair("func", att->StrToStr(att->GetFunc())));
        }
    }
    if (dynamic_cast<AttPitch*>(element) ) {
        AttPitch *att = dynamic_cast<AttPitch*>(element);
        if (att->HasPname()) {
            attributes->push_back(std::make_pair("pname", att->PitchNameToStr(att->GetPname())));
        }
    }
    if (dynamic_cast<AttPlacement*>(element) ) {
        AttPlacement *att = dynamic_cast<AttPlacement*>(element);
        if (att->HasPlace()) {
            attributes->push_back(std::make_pair("place", att->StrToStr(att->GetPlace())));
        }
    }
    if (dynamic_cast<AttPlist*>(element) ) {
        AttPlist *att = dynamic_cast<AttPlist*>(element);
        if (att->HasPlist()) {
            attributes->push_back(std::make_pair("plist", att->StrToStr(att->GetPlist())));
        }
        if (att->HasEvaluate()) {
            attributes->push_back(std::make_pair("evaluate", att->StrToStr(att->GetEvaluate())));
        }
    }
    if (dynamic_cast<AttPointing*>(element) ) {
        AttPointing *att = dynamic_cast<AttPointing*>(element);
        if (att->HasActuate()) {
            attributes->push_back(std::make_pair("actuate", att->StrToStr(att->GetActuate())));
        }
        if (att->HasRole()) {
            attributes->push_back(std::make_pair("role", att->StrToStr(att->GetRole())));
        }
        if (att->HasShow()) {
            attributes->push_back(std::make_pair("show", att->StrToStr(att->GetShow())));
        }
        if (att->HasTarget()) {
            attributes->push_back(std::make_pair("target", att->StrToStr(att->GetTarget())));
        }
        if (att->HasTargettype()) {
            attributes->push_back(std::make_pair("targettype", att->StrToStr(att->GetTargettype())));
        }
        if (att->HasTitle()) {
            attributes->push_back(std::make_pair("title", att->StrToStr(att->GetTitle())));
        }
    }
    if (dynamic_cast<AttRelativesize*>(element) ) {
        AttRelativesize *att = dynamic_cast<AttRelativesize*>(element);
        if (att->HasSize()) {
            attributes->push_back(std::make_pair("size", att->StrToStr(att->GetSize())));
        }
    }
    if (dynamic_cast<AttResponsibility*>(element) ) {
        AttResponsibility *att = dynamic_cast<AttResponsibility*>(element);
        if (att->HasResp()) {
            attributes->push_back(std::make_pair("resp", att->StrToStr(att->GetResp())));
        }
    }
    if (dynamic_cast<AttSbVis*>(element) ) {
        AttSbVis *att = dynamic_cast<AttSbVis*>(element);
        if (att->HasRend()) {
            attributes->push_back(std::make_pair("rend", att->BarRenditionToStr(att->GetRend())));
        }
    }
    if (dynamic_cast<AttScalable*>(element) ) {
        AttScalable *att = dynamic_cast<AttScalable*>(element);
        if (att->HasScale()) {
            attributes->push_back(std::make_pair("scale", att->StrToStr(att->GetScale())));
        }
    }
    if (dynamic_cast<AttScoreDefGes*>(element) ) {
        AttScoreDefGes *att = dynamic_cast<AttScoreDefGes*>(element);
        if (att->HasTunePname()) {
            attributes->push_back(std::make_pair("tunePname", att->StrToStr(att->GetTunePname())));
        }
        if (att->HasTuneHz()) {
            attributes->push_back(std::make_pair("tuneHzDbl", att->DblToStr(att->GetTuneHz())));
        }
        if (att->HasTuneTemper()) {
            attributes->push_back(std::make_pair("tuneTemper", att->StrToStr(att->GetTuneTemper())));
        }
    }
    if (dynamic_cast<AttScoreDefVis*>(element) ) {
        AttScoreDefVis *att = dynamic_cast<AttScoreDefVis*>(element);
        if (att->HasEndingRend()) {
            attributes->push_back(std::make_pair("endingRend", att->StrToStr(att->GetEndingRend())));
        }
        if (att->HasMnumVisible()) {
            attributes->push_back(std::make_pair("mnumVisible", att->StrToStr(att->GetMnumVisible())));
        }
        if (att->HasMusicName()) {
            attributes->push_back(std::make_pair("musicName", att->StrToStr(att->GetMusicName())));
        }
        if (att->HasMusicSize()) {
            attributes->push_back(std::make_pair("musicSize", att->StrToStr(att->GetMusicSize())));
        }
        if (att->HasOptimize()) {
            attributes->push_back(std::make_pair("optimize", att->StrToStr(att->GetOptimize())));
        }
        if (att->HasPageHeight()) {
            attributes->push_back(std::make_pair("pageHeight", att->StrToStr(att->GetPageHeight())));
        }
        if (att->HasPageWidth()) {
            attributes->push_back(std::make_pair("pageWidth", att->StrToStr(att->GetPageWidth())));
        }
        if (att->HasPageTopmar()) {
            attributes->push_back(std::make_pair("pageTopmar", att->StrToStr(att->GetPageTopmar())));
        }
        if (att->HasPageBotmar()) {
            attributes->push_back(std::make_pair("pageBotmar", att->StrToStr(att->GetPageBotmar())));
        }
        if (att->HasPageLeftmar()) {
            attributes->push_back(std::make_pair("pageLeftmar", att->StrToStr(att->GetPageLeftmar())));
        }
        if (att->HasPageRightmar()) {
            attributes->push_back(std::make_pair("pageRightmar", att->StrToStr(att->GetPageRightmar())));
        }
        if (att->HasPagePanels()) {
            attributes->push_back(std::make_pair("pagePanels", att->StrToStr(att->GetPagePanels())));
        }
        if (att->HasPageScale()) {
            attributes->push_back(std::make_pair("pageScale", att->StrToStr(att->GetPageScale())));
        }
        if (att->HasSpacingPackexp()) {
            attributes->push_back(std::make_pair("spacingPackexpDbl", att->DblToStr(att->GetSpacingPackexp())));
        }
        if (att->HasSpacingPackfact()) {
            attributes->push_back(std::make_pair("spacingPackfactDbl", att->DblToStr(att->GetSpacingPackfact())));
        }
        if (att->HasSpacingStaff()) {
            attributes->push_back(std::make_pair("spacingStaff", att->StrToStr(att->GetSpacingStaff())));
        }
        if (att->HasSpacingSystem()) {
            attributes->push_back(std::make_pair("spacingSystem", att->StrToStr(att->GetSpacingSystem())));
        }
        if (att->HasSystemLeftmar()) {
            attributes->push_back(std::make_pair("systemLeftmar", att->StrToStr(att->GetSystemLeftmar())));
        }
        if (att->HasSystemRightmar()) {
            attributes->push_back(std::make_pair("systemRightmar", att->StrToStr(att->GetSystemRightmar())));
        }
        if (att->HasSystemTopmar()) {
            attributes->push_back(std::make_pair("systemTopmar", att->StrToStr(att->GetSystemTopmar())));
        }
        if (att->HasVuHeight()) {
            attributes->push_back(std::make_pair("vuHeight", att->StrToStr(att->GetVuHeight())));
        }
    }
    if (dynamic_cast<AttSectionVis*>(element) ) {
        AttSectionVis *att = dynamic_cast<AttSectionVis*>(element);
        if (att->HasRestart()) {
            attributes->push_back(std::make_pair("restart", att->StrToStr(att->GetRestart())));
        }
    }
    if (dynamic_cast<AttSequence*>(element) ) {
        AttSequence *att = dynamic_cast<AttSequence*>(element);
        if (att->HasSeq()) {
            attributes->push_back(std::make_pair("seqInt", att->IntToStr(att->GetSeq())));
        }
    }
    if (dynamic_cast<AttSlashcount*>(element) ) {
        AttSlashcount *att = dynamic_cast<AttSlashcount*>(element);
        if (att->HasSlash()) {
            attributes->push_back(std::make_pair("slash", att->IntToStr(att->GetSlash())));
        }
    }
    if (dynamic_cast<AttSlurpresent*>(element) ) {
        AttSlurpresent *att = dynamic_cast<AttSlurpresent*>(element);
        if (att->HasSlur()) {
            attributes->push_back(std::make_pair("slur", att->StrToStr(att->GetSlur())));
        }
    }
    if (dynamic_cast<AttSpaceVis*>(element) ) {
        AttSpaceVis *att = dynamic_cast<AttSpaceVis*>(element);
        if (att->HasCompressable()) {
            attributes->push_back(std::make_pair("compressable", att->StrToStr(att->GetCompressable())));
        }
    }
    if (dynamic_cast<AttStaffLog*>(element) ) {
        AttStaffLog *att = dynamic_cast<AttStaffLog*>(element);
        if (att->HasDef()) {
            attributes->push_back(std::make_pair("def", att->StrToStr(att->GetDef())));
        }
    }
    if (dynamic_cast<AttStaffDefVis*>(element) ) {
        AttStaffDefVis *att = dynamic_cast<AttStaffDefVis*>(element);
        if (att->HasGridShow()) {
            attributes->push_back(std::make_pair("gridShow", att->BoolToStr(att->GetGridShow())));
        }
        if (att->HasLayerscheme()) {
            attributes->push_back(std::make_pair("layerscheme", att->LayerschemeToStr(att->GetLayerscheme())));
        }
        if (att->HasLines()) {
            attributes->push_back(std::make_pair("linesInt", att->IntToStr(att->GetLines())));
        }
        if (att->HasLinesColor()) {
            attributes->push_back(std::make_pair("linesColor", att->StrToStr(att->GetLinesColor())));
        }
        if (att->HasLinesVisible()) {
            attributes->push_back(std::make_pair("linesVisible", att->BoolToStr(att->GetLinesVisible())));
        }
        if (att->HasSpacing()) {
            attributes->push_back(std::make_pair("spacing", att->StrToStr(att->GetSpacing())));
        }
    }
    if (dynamic_cast<AttStaffGrpVis*>(element) ) {
        AttStaffGrpVis *att = dynamic_cast<AttStaffGrpVis*>(element);
        if (att->HasBarthru()) {
            attributes->push_back(std::make_pair("barthru", att->StrToStr(att->GetBarthru())));
        }
    }
    if (dynamic_cast<AttStaffgroupingsym*>(element) ) {
        AttStaffgroupingsym *att = dynamic_cast<AttStaffgroupingsym*>(element);
        if (att->HasSymbol()) {
            attributes->push_back(std::make_pair("symbol", att->StrToStr(att->GetSymbol())));
        }
    }
    if (dynamic_cast<AttStaffident*>(element) ) {
        AttStaffident *att = dynamic_cast<AttStaffident*>(element);
        if (att->HasStaff()) {
            attributes->push_back(std::make_pair("staff", att->IntToStr(att->GetStaff())));
        }
    }
    if (dynamic_cast<AttStaffloc*>(element) ) {
        AttStaffloc *att = dynamic_cast<AttStaffloc*>(element);
        if (att->HasLoc()) {
            attributes->push_back(std::make_pair("loc", att->StrToStr(att->GetLoc())));
        }
    }
    if (dynamic_cast<AttStafflocPitched*>(element) ) {
        AttStafflocPitched *att = dynamic_cast<AttStafflocPitched*>(element);
        if (att->HasPloc()) {
            attributes->push_back(std::make_pair("ploc", att->PitchNameToStr(att->GetPloc())));
        }
        if (att->HasOloc()) {
            attributes->push_back(std::make_pair("oloc", att->IntToStr(att->GetOloc())));
        }
    }
    if (dynamic_cast<AttStartendid*>(element) ) {
        AttStartendid *att = dynamic_cast<AttStartendid*>(element);
        if (att->HasEndid()) {
            attributes->push_back(std::make_pair("endid", att->StrToStr(att->GetEndid())));
        }
    }
    if (dynamic_cast<AttStartid*>(element) ) {
        AttStartid *att = dynamic_cast<AttStartid*>(element);
        if (att->HasStartid()) {
            attributes->push_back(std::make_pair("startid", att->StrToStr(att->GetStartid())));
        }
    }
    if (dynamic_cast<AttStemmed*>(element) ) {
        AttStemmed *att = dynamic_cast<AttStemmed*>(element);
        if (att->HasStemDir()) {
            attributes->push_back(std::make_pair("stemDir", att->StemDirectionToStr(att->GetStemDir())));
        }
        if (att->HasStemLen()) {
            attributes->push_back(std::make_pair("stemLen", att->IntToStr(att->GetStemLen())));
        }
        if (att->HasStemPos()) {
            attributes->push_back(std::make_pair("stemPos", att->StemPositionToStr(att->GetStemPos())));
        }
        if (att->HasStemX()) {
            attributes->push_back(std::make_pair("stemX", att->IntToStr(att->GetStemX())));
        }
        if (att->HasStemY()) {
            attributes->push_back(std::make_pair("stemY", att->IntToStr(att->GetStemY())));
        }
    }
    if (dynamic_cast<AttSylLog*>(element) ) {
        AttSylLog *att = dynamic_cast<AttSylLog*>(element);
        if (att->HasCon()) {
            attributes->push_back(std::make_pair("con", att->ConToStr(att->GetCon())));
        }
        if (att->HasWordpos()) {
            attributes->push_back(std::make_pair("wordpos", att->WordPosToStr(att->GetWordpos())));
        }
    }
    if (dynamic_cast<AttSyltext*>(element) ) {
        AttSyltext *att = dynamic_cast<AttSyltext*>(element);
        if (att->HasSyl()) {
            attributes->push_back(std::make_pair("syl", att->StrToStr(att->GetSyl())));
        }
    }
    if (dynamic_cast<AttTextstyle*>(element) ) {
        AttTextstyle *att = dynamic_cast<AttTextstyle*>(element);
        if (att->HasTextFam()) {
            attributes->push_back(std::make_pair("textFam", att->StrToStr(att->GetTextFam())));
        }
        if (att->HasTextName()) {
            attributes->push_back(std::make_pair("textName", att->StrToStr(att->GetTextName())));
        }
        if (att->HasTextSize()) {
            attributes->push_back(std::make_pair("textSize", att->StrToStr(att->GetTextSize())));
        }
        if (att->HasTextStyle()) {
            attributes->push_back(std::make_pair("textStyle", att->StrToStr(att->GetTextStyle())));
        }
        if (att->HasTextWeight()) {
            attributes->push_back(std::make_pair("textWeight", att->StrToStr(att->GetTextWeight())));
        }
    }
    if (dynamic_cast<AttTiepresent*>(element) ) {
        AttTiepresent *att = dynamic_cast<AttTiepresent*>(element);
        if (att->HasTie()) {
            attributes->push_back(std::make_pair("tie", att->TieToStr(att->GetTie())));
        }
    }
    if (dynamic_cast<AttTimestampMusical*>(element) ) {
        AttTimestampMusical *att = dynamic_cast<AttTimestampMusical*>(element);
        if (att->HasTstamp()) {
            attributes->push_back(std::make_pair("tstamp", att->DblToStr(att->GetTstamp())));
        }
    }
    if (dynamic_cast<AttTimestampPerformed*>(element) ) {
        AttTimestampPerformed *att = dynamic_cast<AttTimestampPerformed*>(element);
        if (att->HasTstampGes()) {
            attributes->push_back(std::make_pair("tstampGesInt", att->IntToStr(att->GetTstampGes())));
        }
        if (att->HasTstampReal()) {
            attributes->push_back(std::make_pair("tstampReal", att->StrToStr(att->GetTstampReal())));
        }
    }
    if (dynamic_cast<AttTimestamp2Musical*>(element) ) {
        AttTimestamp2Musical *att = dynamic_cast<AttTimestamp2Musical*>(element);
        if (att->HasTstamp2()) {
            attributes->push_back(std::make_pair("tstamp2", att->Tstamp2ToStr(att->GetTstamp2())));
        }
    }
    if (dynamic_cast<AttTransposition*>(element) ) {
        AttTransposition *att = dynamic_cast<AttTransposition*>(element);
        if (att->HasTransDiat()) {
            attributes->push_back(std::make_pair("transDiatDbl", att->DblToStr(att->GetTransDiat())));
        }
        if (att->HasTransSemi()) {
            attributes->push_back(std::make_pair("transSemiDbl", att->DblToStr(att->GetTransSemi())));
        }
    }
    if (dynamic_cast<AttTupletpresent*>(element) ) {
        AttTupletpresent *att = dynamic_cast<AttTupletpresent*>(element);
        if (att->HasTuplet()) {
            attributes->push_back(std::make_pair("tuplet", att->StrToStr(att->GetTuplet())));
        }
    }
    if (dynamic_cast<AttTyped*>(element) ) {
        AttTyped *att = dynamic_cast<AttTyped*>(element);
        if (att->HasType()) {
            attributes->push_back(std::make_pair("type", att->StrToStr(att->GetType())));
        }
        if (att->HasSubtype()) {
            attributes->push_back(std::make_pair("subtype", att->StrToStr(att->GetSubtype())));
        }
    }
    if (dynamic_cast<AttTypography*>(element) ) {
        AttTypography *att = dynamic_cast<AttTypography*>(element);
        if (att->HasFontfam()) {
            attributes->push_back(std::make_pair("fontfam", att->StrToStr(att->GetFontfam())));
        }
        if (att->HasFontname()) {
            attributes->push_back(std::make_pair("fontname", att->StrToStr(att->GetFontname())));
        }
        if (att->HasFontsize()) {
            attributes->push_back(std::make_pair("fontsizeDbl", att->DblToStr(att->GetFontsize())));
        }
        if (att->HasFontstyle()) {
            attributes->push_back(std::make_pair("fontstyle", att->FontstyleToStr(att->GetFontstyle())));
        }
        if (att->HasFontweight()) {
            attributes->push_back(std::make_pair("fontweight", att->FontweightToStr(att->GetFontweight())));
        }
    }
    if (dynamic_cast<AttVisibility*>(element) ) {
        AttVisibility *att = dynamic_cast<AttVisibility*>(element);
        if (att->HasVisible()) {
            attributes->push_back(std::make_pair("visible", att->StrToStr(att->GetVisible())));
        }
    }
    if (dynamic_cast<AttVisualoffsetHo*>(element) ) {
        AttVisualoffsetHo *att = dynamic_cast<AttVisualoffsetHo*>(element);
        if (att->HasHo()) {
            attributes->push_back(std::make_pair("ho", att->StrToStr(att->GetHo())));
        }
    }
    if (dynamic_cast<AttVisualoffsetTo*>(element) ) {
        AttVisualoffsetTo *att = dynamic_cast<AttVisualoffsetTo*>(element);
        if (att->HasTo()) {
            attributes->push_back(std::make_pair("to", att->StrToStr(att->GetTo())));
        }
    }
    if (dynamic_cast<AttVisualoffsetVo*>(element) ) {
        AttVisualoffsetVo *att = dynamic_cast<AttVisualoffsetVo*>(element);
        if (att->HasVo()) {
            attributes->push_back(std::make_pair("vo", att->StrToStr(att->GetVo())));
        }
    }
    if (dynamic_cast<AttVisualoffset2Ho*>(element) ) {
        AttVisualoffset2Ho *att = dynamic_cast<AttVisualoffset2Ho*>(element);
        if (att->HasStartho()) {
            attributes->push_back(std::make_pair("startho", att->StrToStr(att->GetStartho())));
        }
        if (att->HasEndho()) {
            attributes->push_back(std::make_pair("endho", att->StrToStr(att->GetEndho())));
        }
    }
    if (dynamic_cast<AttVisualoffset2To*>(element) ) {
        AttVisualoffset2To *att = dynamic_cast<AttVisualoffset2To*>(element);
        if (att->HasStartto()) {
            attributes->push_back(std::make_pair("startto", att->StrToStr(att->GetStartto())));
        }
        if (att->HasEndto()) {
            attributes->push_back(std::make_pair("endto", att->StrToStr(att->GetEndto())));
        }
    }
    if (dynamic_cast<AttVisualoffset2Vo*>(element) ) {
        AttVisualoffset2Vo *att = dynamic_cast<AttVisualoffset2Vo*>(element);
        if (att->HasStartvo()) {
            attributes->push_back(std::make_pair("startvo", att->StrToStr(att->GetStartvo())));
        }
        if (att->HasEndvo()) {
            attributes->push_back(std::make_pair("endvo", att->StrToStr(att->GetEndvo())));
        }
    }
    if (dynamic_cast<AttWhitespace*>(element) ) {
        AttWhitespace *att = dynamic_cast<AttWhitespace*>(element);
        if (att->HasSpace()) {
            attributes->push_back(std::make_pair("space", att->StrToStr(att->GetSpace())));
        }
    }
    if (dynamic_cast<AttWidth*>(element) ) {
        AttWidth *att = dynamic_cast<AttWidth*>(element);
        if (att->HasWidth()) {
            attributes->push_back(std::make_pair("width", att->StrToStr(att->GetWidth())));
        }
    }
    if (dynamic_cast<AttXy*>(element) ) {
        AttXy *att = dynamic_cast<AttXy*>(element);
        if (att->HasX()) {
            attributes->push_back(std::make_pair("xDbl", att->DblToStr(att->GetX())));
        }
        if (att->HasY()) {
            attributes->push_back(std::make_pair("yDbl", att->DblToStr(att->GetY())));
        }
    }
    if (dynamic_cast<AttXy2*>(element) ) {
        AttXy2 *att = dynamic_cast<AttXy2*>(element);
        if (att->HasX2()) {
            attributes->push_back(std::make_pair("x2Dbl", att->DblToStr(att->GetX2())));
        }
        if (att->HasY2()) {
            attributes->push_back(std::make_pair("y2Dbl", att->DblToStr(att->GetY2())));
        }
    }

}
    
} // vrv namespace
    
