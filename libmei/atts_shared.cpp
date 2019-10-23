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

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttAccidLog
//----------------------------------------------------------------------------

AttAccidLog::AttAccidLog() : Att()
{
    ResetAccidLog();
}

AttAccidLog::~AttAccidLog()
{
}

void AttAccidLog::ResetAccidLog()
{
    m_func = accidLog_FUNC_NONE;
}

bool AttAccidLog::ReadAccidLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToAccidLogFunc(element.attribute("func").value()));
        element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAccidLog::WriteAccidLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = AccidLogFuncToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAccidLog::HasFunc() const
{
    return (m_func != accidLog_FUNC_NONE);
}

/* include <attfunc> */

//----------------------------------------------------------------------------
// AttAccidental
//----------------------------------------------------------------------------

AttAccidental::AttAccidental() : Att()
{
    ResetAccidental();
}

AttAccidental::~AttAccidental()
{
}

void AttAccidental::ResetAccidental()
{
    m_accid = ACCIDENTAL_WRITTEN_NONE;
}

bool AttAccidental::ReadAccidental(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("accid")) {
        this->SetAccid(StrToAccidentalWritten(element.attribute("accid").value()));
        element.remove_attribute("accid");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAccidental::WriteAccidental(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAccid()) {
        element.append_attribute("accid") = AccidentalWrittenToStr(this->GetAccid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAccidental::HasAccid() const
{
    return (m_accid != ACCIDENTAL_WRITTEN_NONE);
}

/* include <attaccid> */

//----------------------------------------------------------------------------
// AttArticulation
//----------------------------------------------------------------------------

AttArticulation::AttArticulation() : Att()
{
    ResetArticulation();
}

AttArticulation::~AttArticulation()
{
}

void AttArticulation::ResetArticulation()
{
    m_artic = std::vector<data_ARTICULATION>();
}

bool AttArticulation::ReadArticulation(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("artic")) {
        this->SetArtic(StrToArticulationList(element.attribute("artic").value()));
        element.remove_attribute("artic");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArticulation::WriteArticulation(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasArtic()) {
        element.append_attribute("artic") = ArticulationListToStr(this->GetArtic()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttArticulation::HasArtic() const
{
    return (m_artic != std::vector<data_ARTICULATION>());
}

/* include <attartic> */

//----------------------------------------------------------------------------
// AttAttaccaLog
//----------------------------------------------------------------------------

AttAttaccaLog::AttAttaccaLog() : Att()
{
    ResetAttaccaLog();
}

AttAttaccaLog::~AttAttaccaLog()
{
}

void AttAttaccaLog::ResetAttaccaLog()
{
    m_target = "";
}

bool AttAttaccaLog::ReadAttaccaLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("target")) {
        this->SetTarget(StrToStr(element.attribute("target").value()));
        element.remove_attribute("target");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAttaccaLog::WriteAttaccaLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTarget()) {
        element.append_attribute("target") = StrToStr(this->GetTarget()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAttaccaLog::HasTarget() const
{
    return (m_target != "");
}

/* include <atttarget> */

//----------------------------------------------------------------------------
// AttAudience
//----------------------------------------------------------------------------

AttAudience::AttAudience() : Att()
{
    ResetAudience();
}

AttAudience::~AttAudience()
{
}

void AttAudience::ResetAudience()
{
    m_audience = audience_AUDIENCE_NONE;
}

bool AttAudience::ReadAudience(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("audience")) {
        this->SetAudience(StrToAudienceAudience(element.attribute("audience").value()));
        element.remove_attribute("audience");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAudience::WriteAudience(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAudience()) {
        element.append_attribute("audience") = AudienceAudienceToStr(this->GetAudience()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAudience::HasAudience() const
{
    return (m_audience != audience_AUDIENCE_NONE);
}

/* include <attaudience> */

//----------------------------------------------------------------------------
// AttAugmentDots
//----------------------------------------------------------------------------

AttAugmentDots::AttAugmentDots() : Att()
{
    ResetAugmentDots();
}

AttAugmentDots::~AttAugmentDots()
{
}

void AttAugmentDots::ResetAugmentDots()
{
    m_dots = -1;
}

bool AttAugmentDots::ReadAugmentDots(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dots")) {
        this->SetDots(StrToInt(element.attribute("dots").value()));
        element.remove_attribute("dots");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAugmentDots::WriteAugmentDots(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDots()) {
        element.append_attribute("dots") = IntToStr(this->GetDots()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAugmentDots::HasDots() const
{
    return (m_dots != -1);
}

/* include <attdots> */

//----------------------------------------------------------------------------
// AttAuthorized
//----------------------------------------------------------------------------

AttAuthorized::AttAuthorized() : Att()
{
    ResetAuthorized();
}

AttAuthorized::~AttAuthorized()
{
}

void AttAuthorized::ResetAuthorized()
{
    m_auth = "";
    m_authUri = "";
}

bool AttAuthorized::ReadAuthorized(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("auth")) {
        this->SetAuth(StrToStr(element.attribute("auth").value()));
        element.remove_attribute("auth");
        hasAttribute = true;
    }
    if (element.attribute("auth.uri")) {
        this->SetAuthUri(StrToStr(element.attribute("auth.uri").value()));
        element.remove_attribute("auth.uri");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAuthorized::WriteAuthorized(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAuth()) {
        element.append_attribute("auth") = StrToStr(this->GetAuth()).c_str();
        wroteAttribute = true;
    }
    if (this->HasAuthUri()) {
        element.append_attribute("auth.uri") = StrToStr(this->GetAuthUri()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAuthorized::HasAuth() const
{
    return (m_auth != "");
}

bool AttAuthorized::HasAuthUri() const
{
    return (m_authUri != "");
}

/* include <attauth.uri> */

//----------------------------------------------------------------------------
// AttBarLineLog
//----------------------------------------------------------------------------

AttBarLineLog::AttBarLineLog() : Att()
{
    ResetBarLineLog();
}

AttBarLineLog::~AttBarLineLog()
{
}

void AttBarLineLog::ResetBarLineLog()
{
    m_form = BARRENDITION_single;
}

bool AttBarLineLog::ReadBarLineLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToBarrendition(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBarLineLog::WriteBarLineLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = BarrenditionToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBarLineLog::HasForm() const
{
    return (m_form != BARRENDITION_single);
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttBarring
//----------------------------------------------------------------------------

AttBarring::AttBarring() : Att()
{
    ResetBarring();
}

AttBarring::~AttBarring()
{
}

void AttBarring::ResetBarring()
{
    m_barLen = 0.0;
    m_barMethod = BARMETHOD_NONE;
    m_barPlace = 0;
}

bool AttBarring::ReadBarring(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("bar.len")) {
        this->SetBarLen(StrToDbl(element.attribute("bar.len").value()));
        element.remove_attribute("bar.len");
        hasAttribute = true;
    }
    if (element.attribute("bar.method")) {
        this->SetBarMethod(StrToBarmethod(element.attribute("bar.method").value()));
        element.remove_attribute("bar.method");
        hasAttribute = true;
    }
    if (element.attribute("bar.place")) {
        this->SetBarPlace(StrToInt(element.attribute("bar.place").value()));
        element.remove_attribute("bar.place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBarring::WriteBarring(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBarLen()) {
        element.append_attribute("bar.len") = DblToStr(this->GetBarLen()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBarMethod()) {
        element.append_attribute("bar.method") = BarmethodToStr(this->GetBarMethod()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBarPlace()) {
        element.append_attribute("bar.place") = IntToStr(this->GetBarPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBarring::HasBarLen() const
{
    return (m_barLen != 0.0);
}

bool AttBarring::HasBarMethod() const
{
    return (m_barMethod != BARMETHOD_NONE);
}

bool AttBarring::HasBarPlace() const
{
    return (m_barPlace != 0);
}

/* include <attbar.place> */

//----------------------------------------------------------------------------
// AttBasic
//----------------------------------------------------------------------------

AttBasic::AttBasic() : Att()
{
    ResetBasic();
}

AttBasic::~AttBasic()
{
}

void AttBasic::ResetBasic()
{
    m_base = "";
}

bool AttBasic::ReadBasic(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("xml:base")) {
        this->SetBase(StrToStr(element.attribute("xml:base").value()));
        element.remove_attribute("xml:base");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBasic::WriteBasic(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBase()) {
        element.append_attribute("xml:base") = StrToStr(this->GetBase()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBasic::HasBase() const
{
    return (m_base != "");
}

/* include <attbase> */

//----------------------------------------------------------------------------
// AttBibl
//----------------------------------------------------------------------------

AttBibl::AttBibl() : Att()
{
    ResetBibl();
}

AttBibl::~AttBibl()
{
}

void AttBibl::ResetBibl()
{
    m_analog = "";
}

bool AttBibl::ReadBibl(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("analog")) {
        this->SetAnalog(StrToStr(element.attribute("analog").value()));
        element.remove_attribute("analog");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBibl::WriteBibl(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAnalog()) {
        element.append_attribute("analog") = StrToStr(this->GetAnalog()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBibl::HasAnalog() const
{
    return (m_analog != "");
}

/* include <attanalog> */

//----------------------------------------------------------------------------
// AttCalendared
//----------------------------------------------------------------------------

AttCalendared::AttCalendared() : Att()
{
    ResetCalendared();
}

AttCalendared::~AttCalendared()
{
}

void AttCalendared::ResetCalendared()
{
    m_calendar = "";
}

bool AttCalendared::ReadCalendared(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("calendar")) {
        this->SetCalendar(StrToStr(element.attribute("calendar").value()));
        element.remove_attribute("calendar");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCalendared::WriteCalendared(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCalendar()) {
        element.append_attribute("calendar") = StrToStr(this->GetCalendar()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCalendared::HasCalendar() const
{
    return (m_calendar != "");
}

/* include <attcalendar> */

//----------------------------------------------------------------------------
// AttCanonical
//----------------------------------------------------------------------------

AttCanonical::AttCanonical() : Att()
{
    ResetCanonical();
}

AttCanonical::~AttCanonical()
{
}

void AttCanonical::ResetCanonical()
{
    m_codedval = "";
}

bool AttCanonical::ReadCanonical(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("codedval")) {
        this->SetCodedval(StrToStr(element.attribute("codedval").value()));
        element.remove_attribute("codedval");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCanonical::WriteCanonical(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCodedval()) {
        element.append_attribute("codedval") = StrToStr(this->GetCodedval()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCanonical::HasCodedval() const
{
    return (m_codedval != "");
}

/* include <attcodedval> */

//----------------------------------------------------------------------------
// AttClassed
//----------------------------------------------------------------------------

AttClassed::AttClassed() : Att()
{
    ResetClassed();
}

AttClassed::~AttClassed()
{
}

void AttClassed::ResetClassed()
{
    m_class = "";
}

bool AttClassed::ReadClassed(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("class")) {
        this->SetClass(StrToStr(element.attribute("class").value()));
        element.remove_attribute("class");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttClassed::WriteClassed(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasClass()) {
        element.append_attribute("class") = StrToStr(this->GetClass()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttClassed::HasClass() const
{
    return (m_class != "");
}

/* include <attclass> */

//----------------------------------------------------------------------------
// AttClefLog
//----------------------------------------------------------------------------

AttClefLog::AttClefLog() : Att()
{
    ResetClefLog();
}

AttClefLog::~AttClefLog()
{
}

void AttClefLog::ResetClefLog()
{
    m_cautionary = BOOLEAN_NONE;
}

bool AttClefLog::ReadClefLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("cautionary")) {
        this->SetCautionary(StrToBoolean(element.attribute("cautionary").value()));
        element.remove_attribute("cautionary");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttClefLog::WriteClefLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCautionary()) {
        element.append_attribute("cautionary") = BooleanToStr(this->GetCautionary()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttClefLog::HasCautionary() const
{
    return (m_cautionary != BOOLEAN_NONE);
}

/* include <attcautionary> */

//----------------------------------------------------------------------------
// AttClefShape
//----------------------------------------------------------------------------

AttClefShape::AttClefShape() : Att()
{
    ResetClefShape();
}

AttClefShape::~AttClefShape()
{
}

void AttClefShape::ResetClefShape()
{
    m_shape = CLEFSHAPE_NONE;
}

bool AttClefShape::ReadClefShape(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("shape")) {
        this->SetShape(StrToClefshape(element.attribute("shape").value()));
        element.remove_attribute("shape");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttClefShape::WriteClefShape(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasShape()) {
        element.append_attribute("shape") = ClefshapeToStr(this->GetShape()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttClefShape::HasShape() const
{
    return (m_shape != CLEFSHAPE_NONE);
}

/* include <attshape> */

//----------------------------------------------------------------------------
// AttCleffingLog
//----------------------------------------------------------------------------

AttCleffingLog::AttCleffingLog() : Att()
{
    ResetCleffingLog();
}

AttCleffingLog::~AttCleffingLog()
{
}

void AttCleffingLog::ResetCleffingLog()
{
    m_clefShape = CLEFSHAPE_NONE;
    m_clefLine = 0;
    m_clefDis = OCTAVE_DIS_NONE;
    m_clefDisPlace = STAFFREL_basic_NONE;
}

bool AttCleffingLog::ReadCleffingLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("clef.shape")) {
        this->SetClefShape(StrToClefshape(element.attribute("clef.shape").value()));
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
        this->SetClefDisPlace(StrToStaffrelBasic(element.attribute("clef.dis.place").value()));
        element.remove_attribute("clef.dis.place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCleffingLog::WriteCleffingLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasClefShape()) {
        element.append_attribute("clef.shape") = ClefshapeToStr(this->GetClefShape()).c_str();
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
        element.append_attribute("clef.dis.place") = StaffrelBasicToStr(this->GetClefDisPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCleffingLog::HasClefShape() const
{
    return (m_clefShape != CLEFSHAPE_NONE);
}

bool AttCleffingLog::HasClefLine() const
{
    return (m_clefLine != 0);
}

bool AttCleffingLog::HasClefDis() const
{
    return (m_clefDis != OCTAVE_DIS_NONE);
}

bool AttCleffingLog::HasClefDisPlace() const
{
    return (m_clefDisPlace != STAFFREL_basic_NONE);
}

/* include <attclef.dis.place> */

//----------------------------------------------------------------------------
// AttColor
//----------------------------------------------------------------------------

AttColor::AttColor() : Att()
{
    ResetColor();
}

AttColor::~AttColor()
{
}

void AttColor::ResetColor()
{
    m_color = "";
}

bool AttColor::ReadColor(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("color")) {
        this->SetColor(StrToStr(element.attribute("color").value()));
        element.remove_attribute("color");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttColor::WriteColor(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasColor()) {
        element.append_attribute("color") = StrToStr(this->GetColor()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttColor::HasColor() const
{
    return (m_color != "");
}

/* include <attcolor> */

//----------------------------------------------------------------------------
// AttColoration
//----------------------------------------------------------------------------

AttColoration::AttColoration() : Att()
{
    ResetColoration();
}

AttColoration::~AttColoration()
{
}

void AttColoration::ResetColoration()
{
    m_colored = BOOLEAN_NONE;
}

bool AttColoration::ReadColoration(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("colored")) {
        this->SetColored(StrToBoolean(element.attribute("colored").value()));
        element.remove_attribute("colored");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttColoration::WriteColoration(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasColored()) {
        element.append_attribute("colored") = BooleanToStr(this->GetColored()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttColoration::HasColored() const
{
    return (m_colored != BOOLEAN_NONE);
}

/* include <attcolored> */

//----------------------------------------------------------------------------
// AttCoordinated
//----------------------------------------------------------------------------

AttCoordinated::AttCoordinated() : Att()
{
    ResetCoordinated();
}

AttCoordinated::~AttCoordinated()
{
}

void AttCoordinated::ResetCoordinated()
{
    m_ulx = 0;
    m_uly = 0;
    m_lrx = 0;
    m_lry = 0;
}

bool AttCoordinated::ReadCoordinated(pugi::xml_node element)
{
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

bool AttCoordinated::WriteCoordinated(pugi::xml_node element)
{
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

bool AttCoordinated::HasUlx() const
{
    return (m_ulx != 0);
}

bool AttCoordinated::HasUly() const
{
    return (m_uly != 0);
}

bool AttCoordinated::HasLrx() const
{
    return (m_lrx != 0);
}

bool AttCoordinated::HasLry() const
{
    return (m_lry != 0);
}

/* include <attlry> */

//----------------------------------------------------------------------------
// AttCue
//----------------------------------------------------------------------------

AttCue::AttCue() : Att()
{
    ResetCue();
}

AttCue::~AttCue()
{
}

void AttCue::ResetCue()
{
    m_cue = BOOLEAN_NONE;
}

bool AttCue::ReadCue(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("cue")) {
        this->SetCue(StrToBoolean(element.attribute("cue").value()));
        element.remove_attribute("cue");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCue::WriteCue(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCue()) {
        element.append_attribute("cue") = BooleanToStr(this->GetCue()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCue::HasCue() const
{
    return (m_cue != BOOLEAN_NONE);
}

/* include <attcue> */

//----------------------------------------------------------------------------
// AttCurvature
//----------------------------------------------------------------------------

AttCurvature::AttCurvature() : Att()
{
    ResetCurvature();
}

AttCurvature::~AttCurvature()
{
}

void AttCurvature::ResetCurvature()
{
    m_bezier = "";
    m_bulge = 0.0;
    m_curvedir = curvature_CURVEDIR_NONE;
}

bool AttCurvature::ReadCurvature(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("bezier")) {
        this->SetBezier(StrToStr(element.attribute("bezier").value()));
        element.remove_attribute("bezier");
        hasAttribute = true;
    }
    if (element.attribute("bulge")) {
        this->SetBulge(StrToDbl(element.attribute("bulge").value()));
        element.remove_attribute("bulge");
        hasAttribute = true;
    }
    if (element.attribute("curvedir")) {
        this->SetCurvedir(StrToCurvatureCurvedir(element.attribute("curvedir").value()));
        element.remove_attribute("curvedir");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCurvature::WriteCurvature(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBezier()) {
        element.append_attribute("bezier") = StrToStr(this->GetBezier()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBulge()) {
        element.append_attribute("bulge") = DblToStr(this->GetBulge()).c_str();
        wroteAttribute = true;
    }
    if (this->HasCurvedir()) {
        element.append_attribute("curvedir") = CurvatureCurvedirToStr(this->GetCurvedir()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCurvature::HasBezier() const
{
    return (m_bezier != "");
}

bool AttCurvature::HasBulge() const
{
    return (m_bulge != 0.0);
}

bool AttCurvature::HasCurvedir() const
{
    return (m_curvedir != curvature_CURVEDIR_NONE);
}

/* include <attcurvedir> */

//----------------------------------------------------------------------------
// AttCurveRend
//----------------------------------------------------------------------------

AttCurveRend::AttCurveRend() : Att()
{
    ResetCurveRend();
}

AttCurveRend::~AttCurveRend()
{
}

void AttCurveRend::ResetCurveRend()
{
    m_lform = LINEFORM_NONE;
    m_lwidth = data_LINEWIDTH();
}

bool AttCurveRend::ReadCurveRend(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("lform")) {
        this->SetLform(StrToLineform(element.attribute("lform").value()));
        element.remove_attribute("lform");
        hasAttribute = true;
    }
    if (element.attribute("lwidth")) {
        this->SetLwidth(StrToLinewidth(element.attribute("lwidth").value()));
        element.remove_attribute("lwidth");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCurveRend::WriteCurveRend(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLform()) {
        element.append_attribute("lform") = LineformToStr(this->GetLform()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLwidth()) {
        element.append_attribute("lwidth") = LinewidthToStr(this->GetLwidth()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCurveRend::HasLform() const
{
    return (m_lform != LINEFORM_NONE);
}

bool AttCurveRend::HasLwidth() const
{
    return (m_lwidth.HasValue());
}

/* include <attlwidth> */

//----------------------------------------------------------------------------
// AttCustosLog
//----------------------------------------------------------------------------

AttCustosLog::AttCustosLog() : Att()
{
    ResetCustosLog();
}

AttCustosLog::~AttCustosLog()
{
}

void AttCustosLog::ResetCustosLog()
{
    m_target = "";
}

bool AttCustosLog::ReadCustosLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("target")) {
        this->SetTarget(StrToStr(element.attribute("target").value()));
        element.remove_attribute("target");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCustosLog::WriteCustosLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTarget()) {
        element.append_attribute("target") = StrToStr(this->GetTarget()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCustosLog::HasTarget() const
{
    return (m_target != "");
}

/* include <atttarget> */

//----------------------------------------------------------------------------
// AttDataPointing
//----------------------------------------------------------------------------

AttDataPointing::AttDataPointing() : Att()
{
    ResetDataPointing();
}

AttDataPointing::~AttDataPointing()
{
}

void AttDataPointing::ResetDataPointing()
{
    m_data = "";
}

bool AttDataPointing::ReadDataPointing(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("data")) {
        this->SetData(StrToStr(element.attribute("data").value()));
        element.remove_attribute("data");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDataPointing::WriteDataPointing(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasData()) {
        element.append_attribute("data") = StrToStr(this->GetData()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDataPointing::HasData() const
{
    return (m_data != "");
}

/* include <attdata> */

//----------------------------------------------------------------------------
// AttDatable
//----------------------------------------------------------------------------

AttDatable::AttDatable() : Att()
{
    ResetDatable();
}

AttDatable::~AttDatable()
{
}

void AttDatable::ResetDatable()
{
    m_enddate = "";
    m_isodate = "";
    m_notafter = "";
    m_notbefore = "";
    m_startdate = "";
}

bool AttDatable::ReadDatable(pugi::xml_node element)
{
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

bool AttDatable::WriteDatable(pugi::xml_node element)
{
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

bool AttDatable::HasEnddate() const
{
    return (m_enddate != "");
}

bool AttDatable::HasIsodate() const
{
    return (m_isodate != "");
}

bool AttDatable::HasNotafter() const
{
    return (m_notafter != "");
}

bool AttDatable::HasNotbefore() const
{
    return (m_notbefore != "");
}

bool AttDatable::HasStartdate() const
{
    return (m_startdate != "");
}

/* include <attstartdate> */

//----------------------------------------------------------------------------
// AttDistances
//----------------------------------------------------------------------------

AttDistances::AttDistances() : Att()
{
    ResetDistances();
}

AttDistances::~AttDistances()
{
}

void AttDistances::ResetDistances()
{
    m_dynamDist = VRV_UNSET;
    m_harmDist = VRV_UNSET;
    m_textDist = VRV_UNSET;
}

bool AttDistances::ReadDistances(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dynam.dist")) {
        this->SetDynamDist(StrToMeasurementrel(element.attribute("dynam.dist").value()));
        element.remove_attribute("dynam.dist");
        hasAttribute = true;
    }
    if (element.attribute("harm.dist")) {
        this->SetHarmDist(StrToMeasurementrel(element.attribute("harm.dist").value()));
        element.remove_attribute("harm.dist");
        hasAttribute = true;
    }
    if (element.attribute("text.dist")) {
        this->SetTextDist(StrToMeasurementrel(element.attribute("text.dist").value()));
        element.remove_attribute("text.dist");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDistances::WriteDistances(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDynamDist()) {
        element.append_attribute("dynam.dist") = MeasurementrelToStr(this->GetDynamDist()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHarmDist()) {
        element.append_attribute("harm.dist") = MeasurementrelToStr(this->GetHarmDist()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTextDist()) {
        element.append_attribute("text.dist") = MeasurementrelToStr(this->GetTextDist()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDistances::HasDynamDist() const
{
    return (m_dynamDist != VRV_UNSET);
}

bool AttDistances::HasHarmDist() const
{
    return (m_harmDist != VRV_UNSET);
}

bool AttDistances::HasTextDist() const
{
    return (m_textDist != VRV_UNSET);
}

/* include <atttext.dist> */

//----------------------------------------------------------------------------
// AttDotLog
//----------------------------------------------------------------------------

AttDotLog::AttDotLog() : Att()
{
    ResetDotLog();
}

AttDotLog::~AttDotLog()
{
}

void AttDotLog::ResetDotLog()
{
    m_form = dotLog_FORM_NONE;
}

bool AttDotLog::ReadDotLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToDotLogForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDotLog::WriteDotLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = DotLogFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDotLog::HasForm() const
{
    return (m_form != dotLog_FORM_NONE);
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttDurationAdditive
//----------------------------------------------------------------------------

AttDurationAdditive::AttDurationAdditive() : Att()
{
    ResetDurationAdditive();
}

AttDurationAdditive::~AttDurationAdditive()
{
}

void AttDurationAdditive::ResetDurationAdditive()
{
    m_dur = DURATION_NONE;
}

bool AttDurationAdditive::ReadDurationAdditive(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dur")) {
        this->SetDur(StrToDuration(element.attribute("dur").value()));
        element.remove_attribute("dur");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationAdditive::WriteDurationAdditive(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDur()) {
        element.append_attribute("dur") = DurationToStr(this->GetDur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDurationAdditive::HasDur() const
{
    return (m_dur != DURATION_NONE);
}

/* include <attdur> */

//----------------------------------------------------------------------------
// AttDurationDefault
//----------------------------------------------------------------------------

AttDurationDefault::AttDurationDefault() : Att()
{
    ResetDurationDefault();
}

AttDurationDefault::~AttDurationDefault()
{
}

void AttDurationDefault::ResetDurationDefault()
{
    m_durDefault = DURATION_NONE;
    m_numDefault = 0;
    m_numbaseDefault = 0;
}

bool AttDurationDefault::ReadDurationDefault(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dur.default")) {
        this->SetDurDefault(StrToDuration(element.attribute("dur.default").value()));
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

bool AttDurationDefault::WriteDurationDefault(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDurDefault()) {
        element.append_attribute("dur.default") = DurationToStr(this->GetDurDefault()).c_str();
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

bool AttDurationDefault::HasDurDefault() const
{
    return (m_durDefault != DURATION_NONE);
}

bool AttDurationDefault::HasNumDefault() const
{
    return (m_numDefault != 0);
}

bool AttDurationDefault::HasNumbaseDefault() const
{
    return (m_numbaseDefault != 0);
}

/* include <attnumbase.default> */

//----------------------------------------------------------------------------
// AttDurationLogical
//----------------------------------------------------------------------------

AttDurationLogical::AttDurationLogical() : Att()
{
    ResetDurationLogical();
}

AttDurationLogical::~AttDurationLogical()
{
}

void AttDurationLogical::ResetDurationLogical()
{
    m_dur = DURATION_NONE;
}

bool AttDurationLogical::ReadDurationLogical(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dur")) {
        this->SetDur(StrToDuration(element.attribute("dur").value()));
        element.remove_attribute("dur");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationLogical::WriteDurationLogical(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDur()) {
        element.append_attribute("dur") = DurationToStr(this->GetDur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDurationLogical::HasDur() const
{
    return (m_dur != DURATION_NONE);
}

/* include <attdur> */

//----------------------------------------------------------------------------
// AttDurationRatio
//----------------------------------------------------------------------------

AttDurationRatio::AttDurationRatio() : Att()
{
    ResetDurationRatio();
}

AttDurationRatio::~AttDurationRatio()
{
}

void AttDurationRatio::ResetDurationRatio()
{
    m_num = -1;
    m_numbase = -1;
}

bool AttDurationRatio::ReadDurationRatio(pugi::xml_node element)
{
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

bool AttDurationRatio::WriteDurationRatio(pugi::xml_node element)
{
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

bool AttDurationRatio::HasNum() const
{
    return (m_num != -1);
}

bool AttDurationRatio::HasNumbase() const
{
    return (m_numbase != -1);
}

/* include <attnumbase> */

//----------------------------------------------------------------------------
// AttEnclosingChars
//----------------------------------------------------------------------------

AttEnclosingChars::AttEnclosingChars() : Att()
{
    ResetEnclosingChars();
}

AttEnclosingChars::~AttEnclosingChars()
{
}

void AttEnclosingChars::ResetEnclosingChars()
{
    m_enclose = ENCLOSURE_NONE;
}

bool AttEnclosingChars::ReadEnclosingChars(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("enclose")) {
        this->SetEnclose(StrToEnclosure(element.attribute("enclose").value()));
        element.remove_attribute("enclose");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttEnclosingChars::WriteEnclosingChars(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasEnclose()) {
        element.append_attribute("enclose") = EnclosureToStr(this->GetEnclose()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttEnclosingChars::HasEnclose() const
{
    return (m_enclose != ENCLOSURE_NONE);
}

/* include <attenclose> */

//----------------------------------------------------------------------------
// AttEndings
//----------------------------------------------------------------------------

AttEndings::AttEndings() : Att()
{
    ResetEndings();
}

AttEndings::~AttEndings()
{
}

void AttEndings::ResetEndings()
{
    m_endingRend = endings_ENDINGREND_NONE;
}

bool AttEndings::ReadEndings(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("ending.rend")) {
        this->SetEndingRend(StrToEndingsEndingrend(element.attribute("ending.rend").value()));
        element.remove_attribute("ending.rend");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttEndings::WriteEndings(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasEndingRend()) {
        element.append_attribute("ending.rend") = EndingsEndingrendToStr(this->GetEndingRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttEndings::HasEndingRend() const
{
    return (m_endingRend != endings_ENDINGREND_NONE);
}

/* include <attending.rend> */

//----------------------------------------------------------------------------
// AttEvidence
//----------------------------------------------------------------------------

AttEvidence::AttEvidence() : Att()
{
    ResetEvidence();
}

AttEvidence::~AttEvidence()
{
}

void AttEvidence::ResetEvidence()
{
    m_cert = CERTAINTY_NONE;
    m_evidence = "";
}

bool AttEvidence::ReadEvidence(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("cert")) {
        this->SetCert(StrToCertainty(element.attribute("cert").value()));
        element.remove_attribute("cert");
        hasAttribute = true;
    }
    if (element.attribute("evidence")) {
        this->SetEvidence(StrToStr(element.attribute("evidence").value()));
        element.remove_attribute("evidence");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttEvidence::WriteEvidence(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCert()) {
        element.append_attribute("cert") = CertaintyToStr(this->GetCert()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEvidence()) {
        element.append_attribute("evidence") = StrToStr(this->GetEvidence()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttEvidence::HasCert() const
{
    return (m_cert != CERTAINTY_NONE);
}

bool AttEvidence::HasEvidence() const
{
    return (m_evidence != "");
}

/* include <attevidence> */

//----------------------------------------------------------------------------
// AttExtender
//----------------------------------------------------------------------------

AttExtender::AttExtender() : Att()
{
    ResetExtender();
}

AttExtender::~AttExtender()
{
}

void AttExtender::ResetExtender()
{
    m_extender = BOOLEAN_NONE;
}

bool AttExtender::ReadExtender(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("extender")) {
        this->SetExtender(StrToBoolean(element.attribute("extender").value()));
        element.remove_attribute("extender");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttExtender::WriteExtender(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasExtender()) {
        element.append_attribute("extender") = BooleanToStr(this->GetExtender()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttExtender::HasExtender() const
{
    return (m_extender != BOOLEAN_NONE);
}

/* include <attextender> */

//----------------------------------------------------------------------------
// AttExtent
//----------------------------------------------------------------------------

AttExtent::AttExtent() : Att()
{
    ResetExtent();
}

AttExtent::~AttExtent()
{
}

void AttExtent::ResetExtent()
{
    m_extent = "";
}

bool AttExtent::ReadExtent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("extent")) {
        this->SetExtent(StrToStr(element.attribute("extent").value()));
        element.remove_attribute("extent");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttExtent::WriteExtent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasExtent()) {
        element.append_attribute("extent") = StrToStr(this->GetExtent()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttExtent::HasExtent() const
{
    return (m_extent != "");
}

/* include <attextent> */

//----------------------------------------------------------------------------
// AttFermataPresent
//----------------------------------------------------------------------------

AttFermataPresent::AttFermataPresent() : Att()
{
    ResetFermataPresent();
}

AttFermataPresent::~AttFermataPresent()
{
}

void AttFermataPresent::ResetFermataPresent()
{
    m_fermata = STAFFREL_basic_NONE;
}

bool AttFermataPresent::ReadFermataPresent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("fermata")) {
        this->SetFermata(StrToStaffrelBasic(element.attribute("fermata").value()));
        element.remove_attribute("fermata");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFermataPresent::WriteFermataPresent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFermata()) {
        element.append_attribute("fermata") = StaffrelBasicToStr(this->GetFermata()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFermataPresent::HasFermata() const
{
    return (m_fermata != STAFFREL_basic_NONE);
}

/* include <attfermata> */

//----------------------------------------------------------------------------
// AttFiling
//----------------------------------------------------------------------------

AttFiling::AttFiling() : Att()
{
    ResetFiling();
}

AttFiling::~AttFiling()
{
}

void AttFiling::ResetFiling()
{
    m_nonfiling = 0;
}

bool AttFiling::ReadFiling(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("nonfiling")) {
        this->SetNonfiling(StrToInt(element.attribute("nonfiling").value()));
        element.remove_attribute("nonfiling");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFiling::WriteFiling(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasNonfiling()) {
        element.append_attribute("nonfiling") = IntToStr(this->GetNonfiling()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFiling::HasNonfiling() const
{
    return (m_nonfiling != 0);
}

/* include <attnonfiling> */

//----------------------------------------------------------------------------
// AttGrpSymLog
//----------------------------------------------------------------------------

AttGrpSymLog::AttGrpSymLog() : Att()
{
    ResetGrpSymLog();
}

AttGrpSymLog::~AttGrpSymLog()
{
}

void AttGrpSymLog::ResetGrpSymLog()
{
    m_level = 0;
}

bool AttGrpSymLog::ReadGrpSymLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("level")) {
        this->SetLevel(StrToInt(element.attribute("level").value()));
        element.remove_attribute("level");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttGrpSymLog::WriteGrpSymLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLevel()) {
        element.append_attribute("level") = IntToStr(this->GetLevel()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttGrpSymLog::HasLevel() const
{
    return (m_level != 0);
}

/* include <attlevel> */

//----------------------------------------------------------------------------
// AttHandIdent
//----------------------------------------------------------------------------

AttHandIdent::AttHandIdent() : Att()
{
    ResetHandIdent();
}

AttHandIdent::~AttHandIdent()
{
}

void AttHandIdent::ResetHandIdent()
{
    m_hand = "";
}

bool AttHandIdent::ReadHandIdent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("hand")) {
        this->SetHand(StrToStr(element.attribute("hand").value()));
        element.remove_attribute("hand");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHandIdent::WriteHandIdent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasHand()) {
        element.append_attribute("hand") = StrToStr(this->GetHand()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHandIdent::HasHand() const
{
    return (m_hand != "");
}

/* include <atthand> */

//----------------------------------------------------------------------------
// AttHeight
//----------------------------------------------------------------------------

AttHeight::AttHeight() : Att()
{
    ResetHeight();
}

AttHeight::~AttHeight()
{
}

void AttHeight::ResetHeight()
{
    m_height = VRV_UNSET;
}

bool AttHeight::ReadHeight(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("height")) {
        this->SetHeight(StrToMeasurementabs(element.attribute("height").value()));
        element.remove_attribute("height");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHeight::WriteHeight(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasHeight()) {
        element.append_attribute("height") = MeasurementabsToStr(this->GetHeight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHeight::HasHeight() const
{
    return (m_height != VRV_UNSET);
}

/* include <attheight> */

//----------------------------------------------------------------------------
// AttHorizontalAlign
//----------------------------------------------------------------------------

AttHorizontalAlign::AttHorizontalAlign() : Att()
{
    ResetHorizontalAlign();
}

AttHorizontalAlign::~AttHorizontalAlign()
{
}

void AttHorizontalAlign::ResetHorizontalAlign()
{
    m_halign = HORIZONTALALIGNMENT_NONE;
}

bool AttHorizontalAlign::ReadHorizontalAlign(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("halign")) {
        this->SetHalign(StrToHorizontalalignment(element.attribute("halign").value()));
        element.remove_attribute("halign");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHorizontalAlign::WriteHorizontalAlign(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasHalign()) {
        element.append_attribute("halign") = HorizontalalignmentToStr(this->GetHalign()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHorizontalAlign::HasHalign() const
{
    return (m_halign != HORIZONTALALIGNMENT_NONE);
}

/* include <atthalign> */

//----------------------------------------------------------------------------
// AttInternetMedia
//----------------------------------------------------------------------------

AttInternetMedia::AttInternetMedia() : Att()
{
    ResetInternetMedia();
}

AttInternetMedia::~AttInternetMedia()
{
}

void AttInternetMedia::ResetInternetMedia()
{
    m_mimetype = "";
}

bool AttInternetMedia::ReadInternetMedia(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("mimetype")) {
        this->SetMimetype(StrToStr(element.attribute("mimetype").value()));
        element.remove_attribute("mimetype");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttInternetMedia::WriteInternetMedia(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMimetype()) {
        element.append_attribute("mimetype") = StrToStr(this->GetMimetype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttInternetMedia::HasMimetype() const
{
    return (m_mimetype != "");
}

/* include <attmimetype> */

//----------------------------------------------------------------------------
// AttJoined
//----------------------------------------------------------------------------

AttJoined::AttJoined() : Att()
{
    ResetJoined();
}

AttJoined::~AttJoined()
{
}

void AttJoined::ResetJoined()
{
    m_join = "";
}

bool AttJoined::ReadJoined(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("join")) {
        this->SetJoin(StrToStr(element.attribute("join").value()));
        element.remove_attribute("join");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttJoined::WriteJoined(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasJoin()) {
        element.append_attribute("join") = StrToStr(this->GetJoin()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttJoined::HasJoin() const
{
    return (m_join != "");
}

/* include <attjoin> */

//----------------------------------------------------------------------------
// AttKeySigLog
//----------------------------------------------------------------------------

AttKeySigLog::AttKeySigLog() : Att()
{
    ResetKeySigLog();
}

AttKeySigLog::~AttKeySigLog()
{
}

void AttKeySigLog::ResetKeySigLog()
{
    m_sig = std::make_pair(-1, ACCIDENTAL_WRITTEN_NONE);
}

bool AttKeySigLog::ReadKeySigLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("sig")) {
        this->SetSig(StrToKeysignature(element.attribute("sig").value()));
        element.remove_attribute("sig");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigLog::WriteKeySigLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSig()) {
        element.append_attribute("sig") = KeysignatureToStr(this->GetSig()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigLog::HasSig() const
{
    return (m_sig != std::make_pair(-1, ACCIDENTAL_WRITTEN_NONE));
}

/* include <attsig> */

//----------------------------------------------------------------------------
// AttKeySigDefaultLog
//----------------------------------------------------------------------------

AttKeySigDefaultLog::AttKeySigDefaultLog() : Att()
{
    ResetKeySigDefaultLog();
}

AttKeySigDefaultLog::~AttKeySigDefaultLog()
{
}

void AttKeySigDefaultLog::ResetKeySigDefaultLog()
{
    m_keySig = std::make_pair(-1, ACCIDENTAL_WRITTEN_NONE);
}

bool AttKeySigDefaultLog::ReadKeySigDefaultLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("key.sig")) {
        this->SetKeySig(StrToKeysignature(element.attribute("key.sig").value()));
        element.remove_attribute("key.sig");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigDefaultLog::WriteKeySigDefaultLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasKeySig()) {
        element.append_attribute("key.sig") = KeysignatureToStr(this->GetKeySig()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigDefaultLog::HasKeySig() const
{
    return (m_keySig != std::make_pair(-1, ACCIDENTAL_WRITTEN_NONE));
}

/* include <attkey.sig> */

//----------------------------------------------------------------------------
// AttLabelled
//----------------------------------------------------------------------------

AttLabelled::AttLabelled() : Att()
{
    ResetLabelled();
}

AttLabelled::~AttLabelled()
{
}

void AttLabelled::ResetLabelled()
{
    m_label = "";
}

bool AttLabelled::ReadLabelled(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("label")) {
        this->SetLabel(StrToStr(element.attribute("label").value()));
        element.remove_attribute("label");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLabelled::WriteLabelled(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLabel()) {
        element.append_attribute("label") = StrToStr(this->GetLabel()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLabelled::HasLabel() const
{
    return (m_label != "");
}

/* include <attlabel> */

//----------------------------------------------------------------------------
// AttLang
//----------------------------------------------------------------------------

AttLang::AttLang() : Att()
{
    ResetLang();
}

AttLang::~AttLang()
{
}

void AttLang::ResetLang()
{
    m_lang = "";
    m_translit = "";
}

bool AttLang::ReadLang(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("xml:lang")) {
        this->SetLang(StrToStr(element.attribute("xml:lang").value()));
        element.remove_attribute("xml:lang");
        hasAttribute = true;
    }
    if (element.attribute("xml:translit")) {
        this->SetTranslit(StrToStr(element.attribute("xml:translit").value()));
        element.remove_attribute("xml:translit");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLang::WriteLang(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLang()) {
        element.append_attribute("xml:lang") = StrToStr(this->GetLang()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTranslit()) {
        element.append_attribute("xml:translit") = StrToStr(this->GetTranslit()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLang::HasLang() const
{
    return (m_lang != "");
}

bool AttLang::HasTranslit() const
{
    return (m_translit != "");
}

/* include <atttranslit> */

//----------------------------------------------------------------------------
// AttLayerLog
//----------------------------------------------------------------------------

AttLayerLog::AttLayerLog() : Att()
{
    ResetLayerLog();
}

AttLayerLog::~AttLayerLog()
{
}

void AttLayerLog::ResetLayerLog()
{
    m_def = "";
}

bool AttLayerLog::ReadLayerLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("def")) {
        this->SetDef(StrToStr(element.attribute("def").value()));
        element.remove_attribute("def");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLayerLog::WriteLayerLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDef()) {
        element.append_attribute("def") = StrToStr(this->GetDef()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLayerLog::HasDef() const
{
    return (m_def != "");
}

/* include <attdef> */

//----------------------------------------------------------------------------
// AttLayerIdent
//----------------------------------------------------------------------------

AttLayerIdent::AttLayerIdent() : Att()
{
    ResetLayerIdent();
}

AttLayerIdent::~AttLayerIdent()
{
}

void AttLayerIdent::ResetLayerIdent()
{
    m_layer = 0;
}

bool AttLayerIdent::ReadLayerIdent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("layer")) {
        this->SetLayer(StrToInt(element.attribute("layer").value()));
        element.remove_attribute("layer");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLayerIdent::WriteLayerIdent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLayer()) {
        element.append_attribute("layer") = IntToStr(this->GetLayer()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLayerIdent::HasLayer() const
{
    return (m_layer != 0);
}

/* include <attlayer> */

//----------------------------------------------------------------------------
// AttLineLoc
//----------------------------------------------------------------------------

AttLineLoc::AttLineLoc() : Att()
{
    ResetLineLoc();
}

AttLineLoc::~AttLineLoc()
{
}

void AttLineLoc::ResetLineLoc()
{
    m_line = 0;
}

bool AttLineLoc::ReadLineLoc(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("line")) {
        this->SetLine(StrToInt(element.attribute("line").value()));
        element.remove_attribute("line");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLineLoc::WriteLineLoc(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLine()) {
        element.append_attribute("line") = IntToStr(this->GetLine()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLineLoc::HasLine() const
{
    return (m_line != 0);
}

/* include <attline> */

//----------------------------------------------------------------------------
// AttLineRend
//----------------------------------------------------------------------------

AttLineRend::AttLineRend() : Att()
{
    ResetLineRend();
}

AttLineRend::~AttLineRend()
{
}

void AttLineRend::ResetLineRend()
{
    m_lendsym = LINESTARTENDSYMBOL_NONE;
    m_lendsymSize = 0;
    m_lstartsym = LINESTARTENDSYMBOL_NONE;
    m_lstartsymSize = 0;
}

bool AttLineRend::ReadLineRend(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("lendsym")) {
        this->SetLendsym(StrToLinestartendsymbol(element.attribute("lendsym").value()));
        element.remove_attribute("lendsym");
        hasAttribute = true;
    }
    if (element.attribute("lendsym.size")) {
        this->SetLendsymSize(StrToInt(element.attribute("lendsym.size").value()));
        element.remove_attribute("lendsym.size");
        hasAttribute = true;
    }
    if (element.attribute("lstartsym")) {
        this->SetLstartsym(StrToLinestartendsymbol(element.attribute("lstartsym").value()));
        element.remove_attribute("lstartsym");
        hasAttribute = true;
    }
    if (element.attribute("lstartsym.size")) {
        this->SetLstartsymSize(StrToInt(element.attribute("lstartsym.size").value()));
        element.remove_attribute("lstartsym.size");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLineRend::WriteLineRend(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLendsym()) {
        element.append_attribute("lendsym") = LinestartendsymbolToStr(this->GetLendsym()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLendsymSize()) {
        element.append_attribute("lendsym.size") = IntToStr(this->GetLendsymSize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLstartsym()) {
        element.append_attribute("lstartsym") = LinestartendsymbolToStr(this->GetLstartsym()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLstartsymSize()) {
        element.append_attribute("lstartsym.size") = IntToStr(this->GetLstartsymSize()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLineRend::HasLendsym() const
{
    return (m_lendsym != LINESTARTENDSYMBOL_NONE);
}

bool AttLineRend::HasLendsymSize() const
{
    return (m_lendsymSize != 0);
}

bool AttLineRend::HasLstartsym() const
{
    return (m_lstartsym != LINESTARTENDSYMBOL_NONE);
}

bool AttLineRend::HasLstartsymSize() const
{
    return (m_lstartsymSize != 0);
}

/* include <attlstartsym.size> */

//----------------------------------------------------------------------------
// AttLineRendBase
//----------------------------------------------------------------------------

AttLineRendBase::AttLineRendBase() : Att()
{
    ResetLineRendBase();
}

AttLineRendBase::~AttLineRendBase()
{
}

void AttLineRendBase::ResetLineRendBase()
{
    m_lform = LINEFORM_NONE;
    m_lsegs = 0;
    m_lwidth = data_LINEWIDTH();
}

bool AttLineRendBase::ReadLineRendBase(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("lform")) {
        this->SetLform(StrToLineform(element.attribute("lform").value()));
        element.remove_attribute("lform");
        hasAttribute = true;
    }
    if (element.attribute("lsegs")) {
        this->SetLsegs(StrToInt(element.attribute("lsegs").value()));
        element.remove_attribute("lsegs");
        hasAttribute = true;
    }
    if (element.attribute("lwidth")) {
        this->SetLwidth(StrToLinewidth(element.attribute("lwidth").value()));
        element.remove_attribute("lwidth");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLineRendBase::WriteLineRendBase(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLform()) {
        element.append_attribute("lform") = LineformToStr(this->GetLform()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLsegs()) {
        element.append_attribute("lsegs") = IntToStr(this->GetLsegs()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLwidth()) {
        element.append_attribute("lwidth") = LinewidthToStr(this->GetLwidth()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLineRendBase::HasLform() const
{
    return (m_lform != LINEFORM_NONE);
}

bool AttLineRendBase::HasLsegs() const
{
    return (m_lsegs != 0);
}

bool AttLineRendBase::HasLwidth() const
{
    return (m_lwidth.HasValue());
}

/* include <attlwidth> */

//----------------------------------------------------------------------------
// AttLinking
//----------------------------------------------------------------------------

AttLinking::AttLinking() : Att()
{
    ResetLinking();
}

AttLinking::~AttLinking()
{
}

void AttLinking::ResetLinking()
{
    m_copyof = "";
    m_corresp = "";
    m_follows = "";
    m_next = "";
    m_precedes = "";
    m_prev = "";
    m_sameas = "";
    m_synch = "";
}

bool AttLinking::ReadLinking(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("copyof")) {
        this->SetCopyof(StrToStr(element.attribute("copyof").value()));
        element.remove_attribute("copyof");
        hasAttribute = true;
    }
    if (element.attribute("corresp")) {
        this->SetCorresp(StrToStr(element.attribute("corresp").value()));
        element.remove_attribute("corresp");
        hasAttribute = true;
    }
    if (element.attribute("follows")) {
        this->SetFollows(StrToStr(element.attribute("follows").value()));
        element.remove_attribute("follows");
        hasAttribute = true;
    }
    if (element.attribute("next")) {
        this->SetNext(StrToStr(element.attribute("next").value()));
        element.remove_attribute("next");
        hasAttribute = true;
    }
    if (element.attribute("precedes")) {
        this->SetPrecedes(StrToStr(element.attribute("precedes").value()));
        element.remove_attribute("precedes");
        hasAttribute = true;
    }
    if (element.attribute("prev")) {
        this->SetPrev(StrToStr(element.attribute("prev").value()));
        element.remove_attribute("prev");
        hasAttribute = true;
    }
    if (element.attribute("sameas")) {
        this->SetSameas(StrToStr(element.attribute("sameas").value()));
        element.remove_attribute("sameas");
        hasAttribute = true;
    }
    if (element.attribute("synch")) {
        this->SetSynch(StrToStr(element.attribute("synch").value()));
        element.remove_attribute("synch");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLinking::WriteLinking(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCopyof()) {
        element.append_attribute("copyof") = StrToStr(this->GetCopyof()).c_str();
        wroteAttribute = true;
    }
    if (this->HasCorresp()) {
        element.append_attribute("corresp") = StrToStr(this->GetCorresp()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFollows()) {
        element.append_attribute("follows") = StrToStr(this->GetFollows()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNext()) {
        element.append_attribute("next") = StrToStr(this->GetNext()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPrecedes()) {
        element.append_attribute("precedes") = StrToStr(this->GetPrecedes()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPrev()) {
        element.append_attribute("prev") = StrToStr(this->GetPrev()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSameas()) {
        element.append_attribute("sameas") = StrToStr(this->GetSameas()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSynch()) {
        element.append_attribute("synch") = StrToStr(this->GetSynch()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLinking::HasCopyof() const
{
    return (m_copyof != "");
}

bool AttLinking::HasCorresp() const
{
    return (m_corresp != "");
}

bool AttLinking::HasFollows() const
{
    return (m_follows != "");
}

bool AttLinking::HasNext() const
{
    return (m_next != "");
}

bool AttLinking::HasPrecedes() const
{
    return (m_precedes != "");
}

bool AttLinking::HasPrev() const
{
    return (m_prev != "");
}

bool AttLinking::HasSameas() const
{
    return (m_sameas != "");
}

bool AttLinking::HasSynch() const
{
    return (m_synch != "");
}

/* include <attsynch> */

//----------------------------------------------------------------------------
// AttLyricStyle
//----------------------------------------------------------------------------

AttLyricStyle::AttLyricStyle() : Att()
{
    ResetLyricStyle();
}

AttLyricStyle::~AttLyricStyle()
{
}

void AttLyricStyle::ResetLyricStyle()
{
    m_lyricAlign = VRV_UNSET;
    m_lyricFam = "";
    m_lyricName = "";
    m_lyricSize = data_FONTSIZE();
    m_lyricStyle = FONTSTYLE_NONE;
    m_lyricWeight = FONTWEIGHT_NONE;
}

bool AttLyricStyle::ReadLyricStyle(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("lyric.align")) {
        this->SetLyricAlign(StrToMeasurementrel(element.attribute("lyric.align").value()));
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
        this->SetLyricSize(StrToFontsize(element.attribute("lyric.size").value()));
        element.remove_attribute("lyric.size");
        hasAttribute = true;
    }
    if (element.attribute("lyric.style")) {
        this->SetLyricStyle(StrToFontstyle(element.attribute("lyric.style").value()));
        element.remove_attribute("lyric.style");
        hasAttribute = true;
    }
    if (element.attribute("lyric.weight")) {
        this->SetLyricWeight(StrToFontweight(element.attribute("lyric.weight").value()));
        element.remove_attribute("lyric.weight");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLyricStyle::WriteLyricStyle(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLyricAlign()) {
        element.append_attribute("lyric.align") = MeasurementrelToStr(this->GetLyricAlign()).c_str();
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
        element.append_attribute("lyric.size") = FontsizeToStr(this->GetLyricSize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLyricStyle()) {
        element.append_attribute("lyric.style") = FontstyleToStr(this->GetLyricStyle()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLyricWeight()) {
        element.append_attribute("lyric.weight") = FontweightToStr(this->GetLyricWeight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLyricStyle::HasLyricAlign() const
{
    return (m_lyricAlign != VRV_UNSET);
}

bool AttLyricStyle::HasLyricFam() const
{
    return (m_lyricFam != "");
}

bool AttLyricStyle::HasLyricName() const
{
    return (m_lyricName != "");
}

bool AttLyricStyle::HasLyricSize() const
{
    return (m_lyricSize.HasValue());
}

bool AttLyricStyle::HasLyricStyle() const
{
    return (m_lyricStyle != FONTSTYLE_NONE);
}

bool AttLyricStyle::HasLyricWeight() const
{
    return (m_lyricWeight != FONTWEIGHT_NONE);
}

/* include <attlyric.weight> */

//----------------------------------------------------------------------------
// AttMeasureNumbers
//----------------------------------------------------------------------------

AttMeasureNumbers::AttMeasureNumbers() : Att()
{
    ResetMeasureNumbers();
}

AttMeasureNumbers::~AttMeasureNumbers()
{
}

void AttMeasureNumbers::ResetMeasureNumbers()
{
    m_mnumVisible = BOOLEAN_NONE;
}

bool AttMeasureNumbers::ReadMeasureNumbers(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("mnum.visible")) {
        this->SetMnumVisible(StrToBoolean(element.attribute("mnum.visible").value()));
        element.remove_attribute("mnum.visible");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeasureNumbers::WriteMeasureNumbers(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMnumVisible()) {
        element.append_attribute("mnum.visible") = BooleanToStr(this->GetMnumVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeasureNumbers::HasMnumVisible() const
{
    return (m_mnumVisible != BOOLEAN_NONE);
}

/* include <attmnum.visible> */

//----------------------------------------------------------------------------
// AttMeasurement
//----------------------------------------------------------------------------

AttMeasurement::AttMeasurement() : Att()
{
    ResetMeasurement();
}

AttMeasurement::~AttMeasurement()
{
}

void AttMeasurement::ResetMeasurement()
{
    m_unit = "";
}

bool AttMeasurement::ReadMeasurement(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("unit")) {
        this->SetUnit(StrToStr(element.attribute("unit").value()));
        element.remove_attribute("unit");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeasurement::WriteMeasurement(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasUnit()) {
        element.append_attribute("unit") = StrToStr(this->GetUnit()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeasurement::HasUnit() const
{
    return (m_unit != "");
}

/* include <attunit> */

//----------------------------------------------------------------------------
// AttMediaBounds
//----------------------------------------------------------------------------

AttMediaBounds::AttMediaBounds() : Att()
{
    ResetMediaBounds();
}

AttMediaBounds::~AttMediaBounds()
{
}

void AttMediaBounds::ResetMediaBounds()
{
    m_begin = "";
    m_end = "";
    m_betype = BETYPE_NONE;
}

bool AttMediaBounds::ReadMediaBounds(pugi::xml_node element)
{
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
        this->SetBetype(StrToBetype(element.attribute("betype").value()));
        element.remove_attribute("betype");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMediaBounds::WriteMediaBounds(pugi::xml_node element)
{
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
        element.append_attribute("betype") = BetypeToStr(this->GetBetype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMediaBounds::HasBegin() const
{
    return (m_begin != "");
}

bool AttMediaBounds::HasEnd() const
{
    return (m_end != "");
}

bool AttMediaBounds::HasBetype() const
{
    return (m_betype != BETYPE_NONE);
}

/* include <attbetype> */

//----------------------------------------------------------------------------
// AttMedium
//----------------------------------------------------------------------------

AttMedium::AttMedium() : Att()
{
    ResetMedium();
}

AttMedium::~AttMedium()
{
}

void AttMedium::ResetMedium()
{
    m_medium = "";
}

bool AttMedium::ReadMedium(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("medium")) {
        this->SetMedium(StrToStr(element.attribute("medium").value()));
        element.remove_attribute("medium");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMedium::WriteMedium(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMedium()) {
        element.append_attribute("medium") = StrToStr(this->GetMedium()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMedium::HasMedium() const
{
    return (m_medium != "");
}

/* include <attmedium> */

//----------------------------------------------------------------------------
// AttMeiVersion
//----------------------------------------------------------------------------

AttMeiVersion::AttMeiVersion() : Att()
{
    ResetMeiVersion();
}

AttMeiVersion::~AttMeiVersion()
{
}

void AttMeiVersion::ResetMeiVersion()
{
    m_meiversion = meiVersion_MEIVERSION_NONE;
}

bool AttMeiVersion::ReadMeiVersion(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("meiversion")) {
        this->SetMeiversion(StrToMeiVersionMeiversion(element.attribute("meiversion").value()));
        element.remove_attribute("meiversion");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeiVersion::WriteMeiVersion(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMeiversion()) {
        element.append_attribute("meiversion") = MeiVersionMeiversionToStr(this->GetMeiversion()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeiVersion::HasMeiversion() const
{
    return (m_meiversion != meiVersion_MEIVERSION_NONE);
}

/* include <attmeiversion> */

//----------------------------------------------------------------------------
// AttMensurLog
//----------------------------------------------------------------------------

AttMensurLog::AttMensurLog() : Att()
{
    ResetMensurLog();
}

AttMensurLog::~AttMensurLog()
{
}

void AttMensurLog::ResetMensurLog()
{
    m_dot = BOOLEAN_NONE;
    m_sign = MENSURATIONSIGN_NONE;
}

bool AttMensurLog::ReadMensurLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dot")) {
        this->SetDot(StrToBoolean(element.attribute("dot").value()));
        element.remove_attribute("dot");
        hasAttribute = true;
    }
    if (element.attribute("sign")) {
        this->SetSign(StrToMensurationsign(element.attribute("sign").value()));
        element.remove_attribute("sign");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensurLog::WriteMensurLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDot()) {
        element.append_attribute("dot") = BooleanToStr(this->GetDot()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSign()) {
        element.append_attribute("sign") = MensurationsignToStr(this->GetSign()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensurLog::HasDot() const
{
    return (m_dot != BOOLEAN_NONE);
}

bool AttMensurLog::HasSign() const
{
    return (m_sign != MENSURATIONSIGN_NONE);
}

/* include <attsign> */

//----------------------------------------------------------------------------
// AttMetadataPointing
//----------------------------------------------------------------------------

AttMetadataPointing::AttMetadataPointing() : Att()
{
    ResetMetadataPointing();
}

AttMetadataPointing::~AttMetadataPointing()
{
}

void AttMetadataPointing::ResetMetadataPointing()
{
    m_decls = "";
}

bool AttMetadataPointing::ReadMetadataPointing(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("decls")) {
        this->SetDecls(StrToStr(element.attribute("decls").value()));
        element.remove_attribute("decls");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMetadataPointing::WriteMetadataPointing(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDecls()) {
        element.append_attribute("decls") = StrToStr(this->GetDecls()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMetadataPointing::HasDecls() const
{
    return (m_decls != "");
}

/* include <attdecls> */

//----------------------------------------------------------------------------
// AttMeterConformance
//----------------------------------------------------------------------------

AttMeterConformance::AttMeterConformance() : Att()
{
    ResetMeterConformance();
}

AttMeterConformance::~AttMeterConformance()
{
}

void AttMeterConformance::ResetMeterConformance()
{
    m_metcon = meterConformance_METCON_NONE;
}

bool AttMeterConformance::ReadMeterConformance(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("metcon")) {
        this->SetMetcon(StrToMeterConformanceMetcon(element.attribute("metcon").value()));
        element.remove_attribute("metcon");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterConformance::WriteMeterConformance(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMetcon()) {
        element.append_attribute("metcon") = MeterConformanceMetconToStr(this->GetMetcon()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterConformance::HasMetcon() const
{
    return (m_metcon != meterConformance_METCON_NONE);
}

/* include <attmetcon> */

//----------------------------------------------------------------------------
// AttMeterConformanceBar
//----------------------------------------------------------------------------

AttMeterConformanceBar::AttMeterConformanceBar() : Att()
{
    ResetMeterConformanceBar();
}

AttMeterConformanceBar::~AttMeterConformanceBar()
{
}

void AttMeterConformanceBar::ResetMeterConformanceBar()
{
    m_metcon = BOOLEAN_NONE;
    m_control = BOOLEAN_NONE;
}

bool AttMeterConformanceBar::ReadMeterConformanceBar(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("metcon")) {
        this->SetMetcon(StrToBoolean(element.attribute("metcon").value()));
        element.remove_attribute("metcon");
        hasAttribute = true;
    }
    if (element.attribute("control")) {
        this->SetControl(StrToBoolean(element.attribute("control").value()));
        element.remove_attribute("control");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterConformanceBar::WriteMeterConformanceBar(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMetcon()) {
        element.append_attribute("metcon") = BooleanToStr(this->GetMetcon()).c_str();
        wroteAttribute = true;
    }
    if (this->HasControl()) {
        element.append_attribute("control") = BooleanToStr(this->GetControl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterConformanceBar::HasMetcon() const
{
    return (m_metcon != BOOLEAN_NONE);
}

bool AttMeterConformanceBar::HasControl() const
{
    return (m_control != BOOLEAN_NONE);
}

/* include <attcontrol> */

//----------------------------------------------------------------------------
// AttMeterSigLog
//----------------------------------------------------------------------------

AttMeterSigLog::AttMeterSigLog() : Att()
{
    ResetMeterSigLog();
}

AttMeterSigLog::~AttMeterSigLog()
{
}

void AttMeterSigLog::ResetMeterSigLog()
{
    m_count = 0;
    m_sym = METERSIGN_NONE;
    m_unit = 0;
}

bool AttMeterSigLog::ReadMeterSigLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("count")) {
        this->SetCount(StrToInt(element.attribute("count").value()));
        element.remove_attribute("count");
        hasAttribute = true;
    }
    if (element.attribute("sym")) {
        this->SetSym(StrToMetersign(element.attribute("sym").value()));
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

bool AttMeterSigLog::WriteMeterSigLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCount()) {
        element.append_attribute("count") = IntToStr(this->GetCount()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSym()) {
        element.append_attribute("sym") = MetersignToStr(this->GetSym()).c_str();
        wroteAttribute = true;
    }
    if (this->HasUnit()) {
        element.append_attribute("unit") = IntToStr(this->GetUnit()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterSigLog::HasCount() const
{
    return (m_count != 0);
}

bool AttMeterSigLog::HasSym() const
{
    return (m_sym != METERSIGN_NONE);
}

bool AttMeterSigLog::HasUnit() const
{
    return (m_unit != 0);
}

/* include <attunit> */

//----------------------------------------------------------------------------
// AttMeterSigDefaultLog
//----------------------------------------------------------------------------

AttMeterSigDefaultLog::AttMeterSigDefaultLog() : Att()
{
    ResetMeterSigDefaultLog();
}

AttMeterSigDefaultLog::~AttMeterSigDefaultLog()
{
}

void AttMeterSigDefaultLog::ResetMeterSigDefaultLog()
{
    m_meterCount = 0;
    m_meterUnit = 0;
    m_meterSym = METERSIGN_NONE;
}

bool AttMeterSigDefaultLog::ReadMeterSigDefaultLog(pugi::xml_node element)
{
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
    if (element.attribute("meter.sym")) {
        this->SetMeterSym(StrToMetersign(element.attribute("meter.sym").value()));
        element.remove_attribute("meter.sym");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigDefaultLog::WriteMeterSigDefaultLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMeterCount()) {
        element.append_attribute("meter.count") = IntToStr(this->GetMeterCount()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMeterUnit()) {
        element.append_attribute("meter.unit") = IntToStr(this->GetMeterUnit()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMeterSym()) {
        element.append_attribute("meter.sym") = MetersignToStr(this->GetMeterSym()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterSigDefaultLog::HasMeterCount() const
{
    return (m_meterCount != 0);
}

bool AttMeterSigDefaultLog::HasMeterUnit() const
{
    return (m_meterUnit != 0);
}

bool AttMeterSigDefaultLog::HasMeterSym() const
{
    return (m_meterSym != METERSIGN_NONE);
}

/* include <attmeter.sym> */

//----------------------------------------------------------------------------
// AttMmTempo
//----------------------------------------------------------------------------

AttMmTempo::AttMmTempo() : Att()
{
    ResetMmTempo();
}

AttMmTempo::~AttMmTempo()
{
}

void AttMmTempo::ResetMmTempo()
{
    m_mm = 0;
    m_mmUnit = DURATION_NONE;
    m_mmDots = 0;
}

bool AttMmTempo::ReadMmTempo(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("mm")) {
        this->SetMm(StrToInt(element.attribute("mm").value()));
        element.remove_attribute("mm");
        hasAttribute = true;
    }
    if (element.attribute("mm.unit")) {
        this->SetMmUnit(StrToDuration(element.attribute("mm.unit").value()));
        element.remove_attribute("mm.unit");
        hasAttribute = true;
    }
    if (element.attribute("mm.dots")) {
        this->SetMmDots(StrToInt(element.attribute("mm.dots").value()));
        element.remove_attribute("mm.dots");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMmTempo::WriteMmTempo(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMm()) {
        element.append_attribute("mm") = IntToStr(this->GetMm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMmUnit()) {
        element.append_attribute("mm.unit") = DurationToStr(this->GetMmUnit()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMmDots()) {
        element.append_attribute("mm.dots") = IntToStr(this->GetMmDots()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMmTempo::HasMm() const
{
    return (m_mm != 0);
}

bool AttMmTempo::HasMmUnit() const
{
    return (m_mmUnit != DURATION_NONE);
}

bool AttMmTempo::HasMmDots() const
{
    return (m_mmDots != 0);
}

/* include <attmm.dots> */

//----------------------------------------------------------------------------
// AttMultinumMeasures
//----------------------------------------------------------------------------

AttMultinumMeasures::AttMultinumMeasures() : Att()
{
    ResetMultinumMeasures();
}

AttMultinumMeasures::~AttMultinumMeasures()
{
}

void AttMultinumMeasures::ResetMultinumMeasures()
{
    m_multiNumber = BOOLEAN_NONE;
}

bool AttMultinumMeasures::ReadMultinumMeasures(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("multi.number")) {
        this->SetMultiNumber(StrToBoolean(element.attribute("multi.number").value()));
        element.remove_attribute("multi.number");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMultinumMeasures::WriteMultinumMeasures(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMultiNumber()) {
        element.append_attribute("multi.number") = BooleanToStr(this->GetMultiNumber()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMultinumMeasures::HasMultiNumber() const
{
    return (m_multiNumber != BOOLEAN_NONE);
}

/* include <attmulti.number> */

//----------------------------------------------------------------------------
// AttNInteger
//----------------------------------------------------------------------------

AttNInteger::AttNInteger() : Att()
{
    ResetNInteger();
}

AttNInteger::~AttNInteger()
{
}

void AttNInteger::ResetNInteger()
{
    m_n = -1;
}

bool AttNInteger::ReadNInteger(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("n")) {
        this->SetN(StrToInt(element.attribute("n").value()));
        element.remove_attribute("n");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNInteger::WriteNInteger(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasN()) {
        element.append_attribute("n") = IntToStr(this->GetN()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNInteger::HasN() const
{
    return (m_n != -1);
}

/* include <attn> */

//----------------------------------------------------------------------------
// AttNNumberLike
//----------------------------------------------------------------------------

AttNNumberLike::AttNNumberLike() : Att()
{
    ResetNNumberLike();
}

AttNNumberLike::~AttNNumberLike()
{
}

void AttNNumberLike::ResetNNumberLike()
{
    m_n = "";
}

bool AttNNumberLike::ReadNNumberLike(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("n")) {
        this->SetN(StrToStr(element.attribute("n").value()));
        element.remove_attribute("n");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNNumberLike::WriteNNumberLike(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasN()) {
        element.append_attribute("n") = StrToStr(this->GetN()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNNumberLike::HasN() const
{
    return (m_n != "");
}

/* include <attn> */

//----------------------------------------------------------------------------
// AttName
//----------------------------------------------------------------------------

AttName::AttName() : Att()
{
    ResetName();
}

AttName::~AttName()
{
}

void AttName::ResetName()
{
    m_nymref = "";
    m_role = "";
}

bool AttName::ReadName(pugi::xml_node element)
{
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

bool AttName::WriteName(pugi::xml_node element)
{
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

bool AttName::HasNymref() const
{
    return (m_nymref != "");
}

bool AttName::HasRole() const
{
    return (m_role != "");
}

/* include <attrole> */

//----------------------------------------------------------------------------
// AttNotationStyle
//----------------------------------------------------------------------------

AttNotationStyle::AttNotationStyle() : Att()
{
    ResetNotationStyle();
}

AttNotationStyle::~AttNotationStyle()
{
}

void AttNotationStyle::ResetNotationStyle()
{
    m_musicName = "";
    m_musicSize = data_FONTSIZE();
}

bool AttNotationStyle::ReadNotationStyle(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("music.name")) {
        this->SetMusicName(StrToStr(element.attribute("music.name").value()));
        element.remove_attribute("music.name");
        hasAttribute = true;
    }
    if (element.attribute("music.size")) {
        this->SetMusicSize(StrToFontsize(element.attribute("music.size").value()));
        element.remove_attribute("music.size");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNotationStyle::WriteNotationStyle(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMusicName()) {
        element.append_attribute("music.name") = StrToStr(this->GetMusicName()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMusicSize()) {
        element.append_attribute("music.size") = FontsizeToStr(this->GetMusicSize()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNotationStyle::HasMusicName() const
{
    return (m_musicName != "");
}

bool AttNotationStyle::HasMusicSize() const
{
    return (m_musicSize.HasValue());
}

/* include <attmusic.size> */

//----------------------------------------------------------------------------
// AttNoteHeads
//----------------------------------------------------------------------------

AttNoteHeads::AttNoteHeads() : Att()
{
    ResetNoteHeads();
}

AttNoteHeads::~AttNoteHeads()
{
}

void AttNoteHeads::ResetNoteHeads()
{
    m_headAltsym = "";
    m_headAuth = "";
    m_headColor = "";
    m_headFill = FILL_NONE;
    m_headFillcolor = "";
    m_headMod = NOTEHEADMODIFIER_NONE;
    m_headRotation = ROTATION_NONE;
    m_headShape = HEADSHAPE_NONE;
    m_headVisible = BOOLEAN_NONE;
}

bool AttNoteHeads::ReadNoteHeads(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("head.altsym")) {
        this->SetHeadAltsym(StrToStr(element.attribute("head.altsym").value()));
        element.remove_attribute("head.altsym");
        hasAttribute = true;
    }
    if (element.attribute("head.auth")) {
        this->SetHeadAuth(StrToStr(element.attribute("head.auth").value()));
        element.remove_attribute("head.auth");
        hasAttribute = true;
    }
    if (element.attribute("head.color")) {
        this->SetHeadColor(StrToStr(element.attribute("head.color").value()));
        element.remove_attribute("head.color");
        hasAttribute = true;
    }
    if (element.attribute("head.fill")) {
        this->SetHeadFill(StrToFill(element.attribute("head.fill").value()));
        element.remove_attribute("head.fill");
        hasAttribute = true;
    }
    if (element.attribute("head.fillcolor")) {
        this->SetHeadFillcolor(StrToStr(element.attribute("head.fillcolor").value()));
        element.remove_attribute("head.fillcolor");
        hasAttribute = true;
    }
    if (element.attribute("head.mod")) {
        this->SetHeadMod(StrToNoteheadmodifier(element.attribute("head.mod").value()));
        element.remove_attribute("head.mod");
        hasAttribute = true;
    }
    if (element.attribute("head.rotation")) {
        this->SetHeadRotation(StrToRotation(element.attribute("head.rotation").value()));
        element.remove_attribute("head.rotation");
        hasAttribute = true;
    }
    if (element.attribute("head.shape")) {
        this->SetHeadShape(StrToHeadshape(element.attribute("head.shape").value()));
        element.remove_attribute("head.shape");
        hasAttribute = true;
    }
    if (element.attribute("head.visible")) {
        this->SetHeadVisible(StrToBoolean(element.attribute("head.visible").value()));
        element.remove_attribute("head.visible");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteHeads::WriteNoteHeads(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasHeadAltsym()) {
        element.append_attribute("head.altsym") = StrToStr(this->GetHeadAltsym()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHeadAuth()) {
        element.append_attribute("head.auth") = StrToStr(this->GetHeadAuth()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHeadColor()) {
        element.append_attribute("head.color") = StrToStr(this->GetHeadColor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHeadFill()) {
        element.append_attribute("head.fill") = FillToStr(this->GetHeadFill()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHeadFillcolor()) {
        element.append_attribute("head.fillcolor") = StrToStr(this->GetHeadFillcolor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHeadMod()) {
        element.append_attribute("head.mod") = NoteheadmodifierToStr(this->GetHeadMod()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHeadRotation()) {
        element.append_attribute("head.rotation") = RotationToStr(this->GetHeadRotation()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHeadShape()) {
        element.append_attribute("head.shape") = HeadshapeToStr(this->GetHeadShape()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHeadVisible()) {
        element.append_attribute("head.visible") = BooleanToStr(this->GetHeadVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNoteHeads::HasHeadAltsym() const
{
    return (m_headAltsym != "");
}

bool AttNoteHeads::HasHeadAuth() const
{
    return (m_headAuth != "");
}

bool AttNoteHeads::HasHeadColor() const
{
    return (m_headColor != "");
}

bool AttNoteHeads::HasHeadFill() const
{
    return (m_headFill != FILL_NONE);
}

bool AttNoteHeads::HasHeadFillcolor() const
{
    return (m_headFillcolor != "");
}

bool AttNoteHeads::HasHeadMod() const
{
    return (m_headMod != NOTEHEADMODIFIER_NONE);
}

bool AttNoteHeads::HasHeadRotation() const
{
    return (m_headRotation != ROTATION_NONE);
}

bool AttNoteHeads::HasHeadShape() const
{
    return (m_headShape != HEADSHAPE_NONE);
}

bool AttNoteHeads::HasHeadVisible() const
{
    return (m_headVisible != BOOLEAN_NONE);
}

/* include <atthead.visible> */

//----------------------------------------------------------------------------
// AttOctave
//----------------------------------------------------------------------------

AttOctave::AttOctave() : Att()
{
    ResetOctave();
}

AttOctave::~AttOctave()
{
}

void AttOctave::ResetOctave()
{
    m_oct = -127;
}

bool AttOctave::ReadOctave(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("oct")) {
        this->SetOct(StrToOctave(element.attribute("oct").value()));
        element.remove_attribute("oct");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctave::WriteOctave(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOct()) {
        element.append_attribute("oct") = OctaveToStr(this->GetOct()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOctave::HasOct() const
{
    return (m_oct != -127);
}

/* include <attoct> */

//----------------------------------------------------------------------------
// AttOctaveDefault
//----------------------------------------------------------------------------

AttOctaveDefault::AttOctaveDefault() : Att()
{
    ResetOctaveDefault();
}

AttOctaveDefault::~AttOctaveDefault()
{
}

void AttOctaveDefault::ResetOctaveDefault()
{
    m_octDefault = -127;
}

bool AttOctaveDefault::ReadOctaveDefault(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("oct.default")) {
        this->SetOctDefault(StrToOctave(element.attribute("oct.default").value()));
        element.remove_attribute("oct.default");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctaveDefault::WriteOctaveDefault(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOctDefault()) {
        element.append_attribute("oct.default") = OctaveToStr(this->GetOctDefault()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOctaveDefault::HasOctDefault() const
{
    return (m_octDefault != -127);
}

/* include <attoct.default> */

//----------------------------------------------------------------------------
// AttOctaveDisplacement
//----------------------------------------------------------------------------

AttOctaveDisplacement::AttOctaveDisplacement() : Att()
{
    ResetOctaveDisplacement();
}

AttOctaveDisplacement::~AttOctaveDisplacement()
{
}

void AttOctaveDisplacement::ResetOctaveDisplacement()
{
    m_dis = OCTAVE_DIS_NONE;
    m_disPlace = STAFFREL_basic_NONE;
}

bool AttOctaveDisplacement::ReadOctaveDisplacement(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dis")) {
        this->SetDis(StrToOctaveDis(element.attribute("dis").value()));
        element.remove_attribute("dis");
        hasAttribute = true;
    }
    if (element.attribute("dis.place")) {
        this->SetDisPlace(StrToStaffrelBasic(element.attribute("dis.place").value()));
        element.remove_attribute("dis.place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOctaveDisplacement::WriteOctaveDisplacement(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDis()) {
        element.append_attribute("dis") = OctaveDisToStr(this->GetDis()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDisPlace()) {
        element.append_attribute("dis.place") = StaffrelBasicToStr(this->GetDisPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOctaveDisplacement::HasDis() const
{
    return (m_dis != OCTAVE_DIS_NONE);
}

bool AttOctaveDisplacement::HasDisPlace() const
{
    return (m_disPlace != STAFFREL_basic_NONE);
}

/* include <attdis.place> */

//----------------------------------------------------------------------------
// AttOneLineStaff
//----------------------------------------------------------------------------

AttOneLineStaff::AttOneLineStaff() : Att()
{
    ResetOneLineStaff();
}

AttOneLineStaff::~AttOneLineStaff()
{
}

void AttOneLineStaff::ResetOneLineStaff()
{
    m_ontheline = BOOLEAN_NONE;
}

bool AttOneLineStaff::ReadOneLineStaff(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("ontheline")) {
        this->SetOntheline(StrToBoolean(element.attribute("ontheline").value()));
        element.remove_attribute("ontheline");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOneLineStaff::WriteOneLineStaff(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOntheline()) {
        element.append_attribute("ontheline") = BooleanToStr(this->GetOntheline()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOneLineStaff::HasOntheline() const
{
    return (m_ontheline != BOOLEAN_NONE);
}

/* include <attontheline> */

//----------------------------------------------------------------------------
// AttOptimization
//----------------------------------------------------------------------------

AttOptimization::AttOptimization() : Att()
{
    ResetOptimization();
}

AttOptimization::~AttOptimization()
{
}

void AttOptimization::ResetOptimization()
{
    m_optimize = BOOLEAN_NONE;
}

bool AttOptimization::ReadOptimization(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("optimize")) {
        this->SetOptimize(StrToBoolean(element.attribute("optimize").value()));
        element.remove_attribute("optimize");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOptimization::WriteOptimization(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOptimize()) {
        element.append_attribute("optimize") = BooleanToStr(this->GetOptimize()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOptimization::HasOptimize() const
{
    return (m_optimize != BOOLEAN_NONE);
}

/* include <attoptimize> */

//----------------------------------------------------------------------------
// AttOriginLayerIdent
//----------------------------------------------------------------------------

AttOriginLayerIdent::AttOriginLayerIdent() : Att()
{
    ResetOriginLayerIdent();
}

AttOriginLayerIdent::~AttOriginLayerIdent()
{
}

void AttOriginLayerIdent::ResetOriginLayerIdent()
{
    m_originLayer = "";
}

bool AttOriginLayerIdent::ReadOriginLayerIdent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("origin.layer")) {
        this->SetOriginLayer(StrToStr(element.attribute("origin.layer").value()));
        element.remove_attribute("origin.layer");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOriginLayerIdent::WriteOriginLayerIdent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOriginLayer()) {
        element.append_attribute("origin.layer") = StrToStr(this->GetOriginLayer()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOriginLayerIdent::HasOriginLayer() const
{
    return (m_originLayer != "");
}

/* include <attorigin.layer> */

//----------------------------------------------------------------------------
// AttOriginStaffIdent
//----------------------------------------------------------------------------

AttOriginStaffIdent::AttOriginStaffIdent() : Att()
{
    ResetOriginStaffIdent();
}

AttOriginStaffIdent::~AttOriginStaffIdent()
{
}

void AttOriginStaffIdent::ResetOriginStaffIdent()
{
    m_originStaff = "";
}

bool AttOriginStaffIdent::ReadOriginStaffIdent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("origin.staff")) {
        this->SetOriginStaff(StrToStr(element.attribute("origin.staff").value()));
        element.remove_attribute("origin.staff");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOriginStaffIdent::WriteOriginStaffIdent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOriginStaff()) {
        element.append_attribute("origin.staff") = StrToStr(this->GetOriginStaff()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOriginStaffIdent::HasOriginStaff() const
{
    return (m_originStaff != "");
}

/* include <attorigin.staff> */

//----------------------------------------------------------------------------
// AttOriginStartEndId
//----------------------------------------------------------------------------

AttOriginStartEndId::AttOriginStartEndId() : Att()
{
    ResetOriginStartEndId();
}

AttOriginStartEndId::~AttOriginStartEndId()
{
}

void AttOriginStartEndId::ResetOriginStartEndId()
{
    m_originStartid = "";
    m_originEndid = "";
}

bool AttOriginStartEndId::ReadOriginStartEndId(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("origin.startid")) {
        this->SetOriginStartid(StrToStr(element.attribute("origin.startid").value()));
        element.remove_attribute("origin.startid");
        hasAttribute = true;
    }
    if (element.attribute("origin.endid")) {
        this->SetOriginEndid(StrToStr(element.attribute("origin.endid").value()));
        element.remove_attribute("origin.endid");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOriginStartEndId::WriteOriginStartEndId(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOriginStartid()) {
        element.append_attribute("origin.startid") = StrToStr(this->GetOriginStartid()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOriginEndid()) {
        element.append_attribute("origin.endid") = StrToStr(this->GetOriginEndid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOriginStartEndId::HasOriginStartid() const
{
    return (m_originStartid != "");
}

bool AttOriginStartEndId::HasOriginEndid() const
{
    return (m_originEndid != "");
}

/* include <attorigin.endid> */

//----------------------------------------------------------------------------
// AttOriginTimestampLogical
//----------------------------------------------------------------------------

AttOriginTimestampLogical::AttOriginTimestampLogical() : Att()
{
    ResetOriginTimestampLogical();
}

AttOriginTimestampLogical::~AttOriginTimestampLogical()
{
}

void AttOriginTimestampLogical::ResetOriginTimestampLogical()
{
    m_originTstamp = std::make_pair(-1, -1.0);
    m_originTstamp2 = std::make_pair(-1, -1.0);
}

bool AttOriginTimestampLogical::ReadOriginTimestampLogical(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("origin.tstamp")) {
        this->SetOriginTstamp(StrToMeasurebeat(element.attribute("origin.tstamp").value()));
        element.remove_attribute("origin.tstamp");
        hasAttribute = true;
    }
    if (element.attribute("origin.tstamp2")) {
        this->SetOriginTstamp2(StrToMeasurebeat(element.attribute("origin.tstamp2").value()));
        element.remove_attribute("origin.tstamp2");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOriginTimestampLogical::WriteOriginTimestampLogical(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOriginTstamp()) {
        element.append_attribute("origin.tstamp") = MeasurebeatToStr(this->GetOriginTstamp()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOriginTstamp2()) {
        element.append_attribute("origin.tstamp2") = MeasurebeatToStr(this->GetOriginTstamp2()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOriginTimestampLogical::HasOriginTstamp() const
{
    return (m_originTstamp != std::make_pair(-1, -1.0));
}

bool AttOriginTimestampLogical::HasOriginTstamp2() const
{
    return (m_originTstamp2 != std::make_pair(-1, -1.0));
}

/* include <attorigin.tstamp2> */

//----------------------------------------------------------------------------
// AttPages
//----------------------------------------------------------------------------

AttPages::AttPages() : Att()
{
    ResetPages();
}

AttPages::~AttPages()
{
}

void AttPages::ResetPages()
{
    m_pageHeight = VRV_UNSET;
    m_pageWidth = VRV_UNSET;
    m_pageTopmar = VRV_UNSET;
    m_pageBotmar = VRV_UNSET;
    m_pageLeftmar = VRV_UNSET;
    m_pageRightmar = VRV_UNSET;
    m_pagePanels = "";
    m_pageScale = "";
}

bool AttPages::ReadPages(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("page.height")) {
        this->SetPageHeight(StrToMeasurementabs(element.attribute("page.height").value()));
        element.remove_attribute("page.height");
        hasAttribute = true;
    }
    if (element.attribute("page.width")) {
        this->SetPageWidth(StrToMeasurementabs(element.attribute("page.width").value()));
        element.remove_attribute("page.width");
        hasAttribute = true;
    }
    if (element.attribute("page.topmar")) {
        this->SetPageTopmar(StrToMeasurementabs(element.attribute("page.topmar").value()));
        element.remove_attribute("page.topmar");
        hasAttribute = true;
    }
    if (element.attribute("page.botmar")) {
        this->SetPageBotmar(StrToMeasurementabs(element.attribute("page.botmar").value()));
        element.remove_attribute("page.botmar");
        hasAttribute = true;
    }
    if (element.attribute("page.leftmar")) {
        this->SetPageLeftmar(StrToMeasurementabs(element.attribute("page.leftmar").value()));
        element.remove_attribute("page.leftmar");
        hasAttribute = true;
    }
    if (element.attribute("page.rightmar")) {
        this->SetPageRightmar(StrToMeasurementabs(element.attribute("page.rightmar").value()));
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
    return hasAttribute;
}

bool AttPages::WritePages(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPageHeight()) {
        element.append_attribute("page.height") = MeasurementabsToStr(this->GetPageHeight()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageWidth()) {
        element.append_attribute("page.width") = MeasurementabsToStr(this->GetPageWidth()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageTopmar()) {
        element.append_attribute("page.topmar") = MeasurementabsToStr(this->GetPageTopmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageBotmar()) {
        element.append_attribute("page.botmar") = MeasurementabsToStr(this->GetPageBotmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageLeftmar()) {
        element.append_attribute("page.leftmar") = MeasurementabsToStr(this->GetPageLeftmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageRightmar()) {
        element.append_attribute("page.rightmar") = MeasurementabsToStr(this->GetPageRightmar()).c_str();
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
    return wroteAttribute;
}

bool AttPages::HasPageHeight() const
{
    return (m_pageHeight != VRV_UNSET);
}

bool AttPages::HasPageWidth() const
{
    return (m_pageWidth != VRV_UNSET);
}

bool AttPages::HasPageTopmar() const
{
    return (m_pageTopmar != VRV_UNSET);
}

bool AttPages::HasPageBotmar() const
{
    return (m_pageBotmar != VRV_UNSET);
}

bool AttPages::HasPageLeftmar() const
{
    return (m_pageLeftmar != VRV_UNSET);
}

bool AttPages::HasPageRightmar() const
{
    return (m_pageRightmar != VRV_UNSET);
}

bool AttPages::HasPagePanels() const
{
    return (m_pagePanels != "");
}

bool AttPages::HasPageScale() const
{
    return (m_pageScale != "");
}

/* include <attpage.scale> */

//----------------------------------------------------------------------------
// AttPartIdent
//----------------------------------------------------------------------------

AttPartIdent::AttPartIdent() : Att()
{
    ResetPartIdent();
}

AttPartIdent::~AttPartIdent()
{
}

void AttPartIdent::ResetPartIdent()
{
    m_part = "";
    m_partstaff = "";
}

bool AttPartIdent::ReadPartIdent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("part")) {
        this->SetPart(StrToStr(element.attribute("part").value()));
        element.remove_attribute("part");
        hasAttribute = true;
    }
    if (element.attribute("partstaff")) {
        this->SetPartstaff(StrToStr(element.attribute("partstaff").value()));
        element.remove_attribute("partstaff");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPartIdent::WritePartIdent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPart()) {
        element.append_attribute("part") = StrToStr(this->GetPart()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPartstaff()) {
        element.append_attribute("partstaff") = StrToStr(this->GetPartstaff()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPartIdent::HasPart() const
{
    return (m_part != "");
}

bool AttPartIdent::HasPartstaff() const
{
    return (m_partstaff != "");
}

/* include <attpartstaff> */

//----------------------------------------------------------------------------
// AttPitch
//----------------------------------------------------------------------------

AttPitch::AttPitch() : Att()
{
    ResetPitch();
}

AttPitch::~AttPitch()
{
}

void AttPitch::ResetPitch()
{
    m_pname = PITCHNAME_NONE;
}

bool AttPitch::ReadPitch(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("pname")) {
        this->SetPname(StrToPitchname(element.attribute("pname").value()));
        element.remove_attribute("pname");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPitch::WritePitch(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPname()) {
        element.append_attribute("pname") = PitchnameToStr(this->GetPname()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPitch::HasPname() const
{
    return (m_pname != PITCHNAME_NONE);
}

/* include <attpname> */

//----------------------------------------------------------------------------
// AttPlacement
//----------------------------------------------------------------------------

AttPlacement::AttPlacement() : Att()
{
    ResetPlacement();
}

AttPlacement::~AttPlacement()
{
}

void AttPlacement::ResetPlacement()
{
    m_place = data_STAFFREL();
}

bool AttPlacement::ReadPlacement(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("place")) {
        this->SetPlace(StrToStaffrel(element.attribute("place").value()));
        element.remove_attribute("place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPlacement::WritePlacement(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPlace()) {
        element.append_attribute("place") = StaffrelToStr(this->GetPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPlacement::HasPlace() const
{
    return (m_place != data_STAFFREL());
}

/* include <attplace> */

//----------------------------------------------------------------------------
// AttPlist
//----------------------------------------------------------------------------

AttPlist::AttPlist() : Att()
{
    ResetPlist();
}

AttPlist::~AttPlist()
{
}

void AttPlist::ResetPlist()
{
    m_plist = std::vector<std::string>();
}

bool AttPlist::ReadPlist(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("plist")) {
        this->SetPlist(StrToXsdAnyURIList(element.attribute("plist").value()));
        element.remove_attribute("plist");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPlist::WritePlist(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPlist()) {
        element.append_attribute("plist") = XsdAnyURIListToStr(this->GetPlist()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPlist::HasPlist() const
{
    return (m_plist != std::vector<std::string>());
}

/* include <attplist> */

//----------------------------------------------------------------------------
// AttPointing
//----------------------------------------------------------------------------

AttPointing::AttPointing() : Att()
{
    ResetPointing();
}

AttPointing::~AttPointing()
{
}

void AttPointing::ResetPointing()
{
    m_actuate = "";
    m_role = "";
    m_show = "";
    m_target = "";
    m_targettype = "";
}

bool AttPointing::ReadPointing(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("xlink:actuate")) {
        this->SetActuate(StrToStr(element.attribute("xlink:actuate").value()));
        element.remove_attribute("xlink:actuate");
        hasAttribute = true;
    }
    if (element.attribute("xlink:role")) {
        this->SetRole(StrToStr(element.attribute("xlink:role").value()));
        element.remove_attribute("xlink:role");
        hasAttribute = true;
    }
    if (element.attribute("xlink:show")) {
        this->SetShow(StrToStr(element.attribute("xlink:show").value()));
        element.remove_attribute("xlink:show");
        hasAttribute = true;
    }
    if (element.attribute("xlink:target")) {
        this->SetTarget(StrToStr(element.attribute("xlink:target").value()));
        element.remove_attribute("xlink:target");
        hasAttribute = true;
    }
    if (element.attribute("xlink:targettype")) {
        this->SetTargettype(StrToStr(element.attribute("xlink:targettype").value()));
        element.remove_attribute("xlink:targettype");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPointing::WritePointing(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasActuate()) {
        element.append_attribute("xlink:actuate") = StrToStr(this->GetActuate()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRole()) {
        element.append_attribute("xlink:role") = StrToStr(this->GetRole()).c_str();
        wroteAttribute = true;
    }
    if (this->HasShow()) {
        element.append_attribute("xlink:show") = StrToStr(this->GetShow()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTarget()) {
        element.append_attribute("xlink:target") = StrToStr(this->GetTarget()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTargettype()) {
        element.append_attribute("xlink:targettype") = StrToStr(this->GetTargettype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPointing::HasActuate() const
{
    return (m_actuate != "");
}

bool AttPointing::HasRole() const
{
    return (m_role != "");
}

bool AttPointing::HasShow() const
{
    return (m_show != "");
}

bool AttPointing::HasTarget() const
{
    return (m_target != "");
}

bool AttPointing::HasTargettype() const
{
    return (m_targettype != "");
}

/* include <atttargettype> */

//----------------------------------------------------------------------------
// AttQuantity
//----------------------------------------------------------------------------

AttQuantity::AttQuantity() : Att()
{
    ResetQuantity();
}

AttQuantity::~AttQuantity()
{
}

void AttQuantity::ResetQuantity()
{
    m_quantity = 0.0;
}

bool AttQuantity::ReadQuantity(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("quantity")) {
        this->SetQuantity(StrToDbl(element.attribute("quantity").value()));
        element.remove_attribute("quantity");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttQuantity::WriteQuantity(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasQuantity()) {
        element.append_attribute("quantity") = DblToStr(this->GetQuantity()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttQuantity::HasQuantity() const
{
    return (m_quantity != 0.0);
}

/* include <attquantity> */

//----------------------------------------------------------------------------
// AttRanging
//----------------------------------------------------------------------------

AttRanging::AttRanging() : Att()
{
    ResetRanging();
}

AttRanging::~AttRanging()
{
}

void AttRanging::ResetRanging()
{
    m_atleast = 0.0;
    m_atmost = 0.0;
    m_min = 0.0;
    m_max = 0.0;
    m_confidence = 0.0;
}

bool AttRanging::ReadRanging(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("atleast")) {
        this->SetAtleast(StrToDbl(element.attribute("atleast").value()));
        element.remove_attribute("atleast");
        hasAttribute = true;
    }
    if (element.attribute("atmost")) {
        this->SetAtmost(StrToDbl(element.attribute("atmost").value()));
        element.remove_attribute("atmost");
        hasAttribute = true;
    }
    if (element.attribute("min")) {
        this->SetMin(StrToDbl(element.attribute("min").value()));
        element.remove_attribute("min");
        hasAttribute = true;
    }
    if (element.attribute("max")) {
        this->SetMax(StrToDbl(element.attribute("max").value()));
        element.remove_attribute("max");
        hasAttribute = true;
    }
    if (element.attribute("confidence")) {
        this->SetConfidence(StrToDbl(element.attribute("confidence").value()));
        element.remove_attribute("confidence");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRanging::WriteRanging(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAtleast()) {
        element.append_attribute("atleast") = DblToStr(this->GetAtleast()).c_str();
        wroteAttribute = true;
    }
    if (this->HasAtmost()) {
        element.append_attribute("atmost") = DblToStr(this->GetAtmost()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMin()) {
        element.append_attribute("min") = DblToStr(this->GetMin()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMax()) {
        element.append_attribute("max") = DblToStr(this->GetMax()).c_str();
        wroteAttribute = true;
    }
    if (this->HasConfidence()) {
        element.append_attribute("confidence") = DblToStr(this->GetConfidence()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRanging::HasAtleast() const
{
    return (m_atleast != 0.0);
}

bool AttRanging::HasAtmost() const
{
    return (m_atmost != 0.0);
}

bool AttRanging::HasMin() const
{
    return (m_min != 0.0);
}

bool AttRanging::HasMax() const
{
    return (m_max != 0.0);
}

bool AttRanging::HasConfidence() const
{
    return (m_confidence != 0.0);
}

/* include <attconfidence> */

//----------------------------------------------------------------------------
// AttResponsibility
//----------------------------------------------------------------------------

AttResponsibility::AttResponsibility() : Att()
{
    ResetResponsibility();
}

AttResponsibility::~AttResponsibility()
{
}

void AttResponsibility::ResetResponsibility()
{
    m_resp = "";
}

bool AttResponsibility::ReadResponsibility(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("resp")) {
        this->SetResp(StrToStr(element.attribute("resp").value()));
        element.remove_attribute("resp");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttResponsibility::WriteResponsibility(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasResp()) {
        element.append_attribute("resp") = StrToStr(this->GetResp()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttResponsibility::HasResp() const
{
    return (m_resp != "");
}

/* include <attresp> */

//----------------------------------------------------------------------------
// AttScalable
//----------------------------------------------------------------------------

AttScalable::AttScalable() : Att()
{
    ResetScalable();
}

AttScalable::~AttScalable()
{
}

void AttScalable::ResetScalable()
{
    m_scale = -1.0;
}

bool AttScalable::ReadScalable(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("scale")) {
        this->SetScale(StrToPercent(element.attribute("scale").value()));
        element.remove_attribute("scale");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScalable::WriteScalable(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasScale()) {
        element.append_attribute("scale") = PercentToStr(this->GetScale()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttScalable::HasScale() const
{
    return (m_scale != -1.0);
}

/* include <attscale> */

//----------------------------------------------------------------------------
// AttSequence
//----------------------------------------------------------------------------

AttSequence::AttSequence() : Att()
{
    ResetSequence();
}

AttSequence::~AttSequence()
{
}

void AttSequence::ResetSequence()
{
    m_seq = 0;
}

bool AttSequence::ReadSequence(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("seq")) {
        this->SetSeq(StrToInt(element.attribute("seq").value()));
        element.remove_attribute("seq");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSequence::WriteSequence(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSeq()) {
        element.append_attribute("seq") = IntToStr(this->GetSeq()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSequence::HasSeq() const
{
    return (m_seq != 0);
}

/* include <attseq> */

//----------------------------------------------------------------------------
// AttSlashCount
//----------------------------------------------------------------------------

AttSlashCount::AttSlashCount() : Att()
{
    ResetSlashCount();
}

AttSlashCount::~AttSlashCount()
{
}

void AttSlashCount::ResetSlashCount()
{
    m_slash = 0;
}

bool AttSlashCount::ReadSlashCount(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("slash")) {
        this->SetSlash(StrToInt(element.attribute("slash").value()));
        element.remove_attribute("slash");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSlashCount::WriteSlashCount(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSlash()) {
        element.append_attribute("slash") = IntToStr(this->GetSlash()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSlashCount::HasSlash() const
{
    return (m_slash != 0);
}

/* include <attslash> */

//----------------------------------------------------------------------------
// AttSlurPresent
//----------------------------------------------------------------------------

AttSlurPresent::AttSlurPresent() : Att()
{
    ResetSlurPresent();
}

AttSlurPresent::~AttSlurPresent()
{
}

void AttSlurPresent::ResetSlurPresent()
{
    m_slur = "";
}

bool AttSlurPresent::ReadSlurPresent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("slur")) {
        this->SetSlur(StrToStr(element.attribute("slur").value()));
        element.remove_attribute("slur");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSlurPresent::WriteSlurPresent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSlur()) {
        element.append_attribute("slur") = StrToStr(this->GetSlur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSlurPresent::HasSlur() const
{
    return (m_slur != "");
}

/* include <attslur> */

//----------------------------------------------------------------------------
// AttSource
//----------------------------------------------------------------------------

AttSource::AttSource() : Att()
{
    ResetSource();
}

AttSource::~AttSource()
{
}

void AttSource::ResetSource()
{
    m_source = "";
}

bool AttSource::ReadSource(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("source")) {
        this->SetSource(StrToStr(element.attribute("source").value()));
        element.remove_attribute("source");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSource::WriteSource(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSource()) {
        element.append_attribute("source") = StrToStr(this->GetSource()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSource::HasSource() const
{
    return (m_source != "");
}

/* include <attsource> */

//----------------------------------------------------------------------------
// AttSpacing
//----------------------------------------------------------------------------

AttSpacing::AttSpacing() : Att()
{
    ResetSpacing();
}

AttSpacing::~AttSpacing()
{
}

void AttSpacing::ResetSpacing()
{
    m_spacingPackexp = 0.0;
    m_spacingPackfact = 0.0;
    m_spacingStaff = VRV_UNSET;
    m_spacingSystem = VRV_UNSET;
}

bool AttSpacing::ReadSpacing(pugi::xml_node element)
{
    bool hasAttribute = false;
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
        this->SetSpacingStaff(StrToMeasurementrel(element.attribute("spacing.staff").value()));
        element.remove_attribute("spacing.staff");
        hasAttribute = true;
    }
    if (element.attribute("spacing.system")) {
        this->SetSpacingSystem(StrToMeasurementrel(element.attribute("spacing.system").value()));
        element.remove_attribute("spacing.system");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSpacing::WriteSpacing(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSpacingPackexp()) {
        element.append_attribute("spacing.packexp") = DblToStr(this->GetSpacingPackexp()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSpacingPackfact()) {
        element.append_attribute("spacing.packfact") = DblToStr(this->GetSpacingPackfact()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSpacingStaff()) {
        element.append_attribute("spacing.staff") = MeasurementrelToStr(this->GetSpacingStaff()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSpacingSystem()) {
        element.append_attribute("spacing.system") = MeasurementrelToStr(this->GetSpacingSystem()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSpacing::HasSpacingPackexp() const
{
    return (m_spacingPackexp != 0.0);
}

bool AttSpacing::HasSpacingPackfact() const
{
    return (m_spacingPackfact != 0.0);
}

bool AttSpacing::HasSpacingStaff() const
{
    return (m_spacingStaff != VRV_UNSET);
}

bool AttSpacing::HasSpacingSystem() const
{
    return (m_spacingSystem != VRV_UNSET);
}

/* include <attspacing.system> */

//----------------------------------------------------------------------------
// AttStaffLog
//----------------------------------------------------------------------------

AttStaffLog::AttStaffLog() : Att()
{
    ResetStaffLog();
}

AttStaffLog::~AttStaffLog()
{
}

void AttStaffLog::ResetStaffLog()
{
    m_def = "";
}

bool AttStaffLog::ReadStaffLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("def")) {
        this->SetDef(StrToStr(element.attribute("def").value()));
        element.remove_attribute("def");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffLog::WriteStaffLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDef()) {
        element.append_attribute("def") = StrToStr(this->GetDef()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffLog::HasDef() const
{
    return (m_def != "");
}

/* include <attdef> */

//----------------------------------------------------------------------------
// AttStaffDefLog
//----------------------------------------------------------------------------

AttStaffDefLog::AttStaffDefLog() : Att()
{
    ResetStaffDefLog();
}

AttStaffDefLog::~AttStaffDefLog()
{
}

void AttStaffDefLog::ResetStaffDefLog()
{
    m_lines = 0;
}

bool AttStaffDefLog::ReadStaffDefLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("lines")) {
        this->SetLines(StrToInt(element.attribute("lines").value()));
        element.remove_attribute("lines");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffDefLog::WriteStaffDefLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLines()) {
        element.append_attribute("lines") = IntToStr(this->GetLines()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffDefLog::HasLines() const
{
    return (m_lines != 0);
}

/* include <attlines> */

//----------------------------------------------------------------------------
// AttStaffGroupingSym
//----------------------------------------------------------------------------

AttStaffGroupingSym::AttStaffGroupingSym() : Att()
{
    ResetStaffGroupingSym();
}

AttStaffGroupingSym::~AttStaffGroupingSym()
{
}

void AttStaffGroupingSym::ResetStaffGroupingSym()
{
    m_symbol = staffGroupingSym_SYMBOL_NONE;
}

bool AttStaffGroupingSym::ReadStaffGroupingSym(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("symbol")) {
        this->SetSymbol(StrToStaffGroupingSymSymbol(element.attribute("symbol").value()));
        element.remove_attribute("symbol");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffGroupingSym::WriteStaffGroupingSym(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSymbol()) {
        element.append_attribute("symbol") = StaffGroupingSymSymbolToStr(this->GetSymbol()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffGroupingSym::HasSymbol() const
{
    return (m_symbol != staffGroupingSym_SYMBOL_NONE);
}

/* include <attsymbol> */

//----------------------------------------------------------------------------
// AttStaffIdent
//----------------------------------------------------------------------------

AttStaffIdent::AttStaffIdent() : Att()
{
    ResetStaffIdent();
}

AttStaffIdent::~AttStaffIdent()
{
}

void AttStaffIdent::ResetStaffIdent()
{
    m_staff = std::vector<int>();
}

bool AttStaffIdent::ReadStaffIdent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("staff")) {
        this->SetStaff(StrToXsdPositiveIntegerList(element.attribute("staff").value()));
        element.remove_attribute("staff");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffIdent::WriteStaffIdent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasStaff()) {
        element.append_attribute("staff") = XsdPositiveIntegerListToStr(this->GetStaff()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffIdent::HasStaff() const
{
    return (m_staff != std::vector<int>());
}

/* include <attstaff> */

//----------------------------------------------------------------------------
// AttStaffItems
//----------------------------------------------------------------------------

AttStaffItems::AttStaffItems() : Att()
{
    ResetStaffItems();
}

AttStaffItems::~AttStaffItems()
{
}

void AttStaffItems::ResetStaffItems()
{
    m_aboveorder = data_STAFFITEM();
    m_beloworder = data_STAFFITEM();
    m_betweenorder = data_STAFFITEM();
}

bool AttStaffItems::ReadStaffItems(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("aboveorder")) {
        this->SetAboveorder(StrToStaffitem(element.attribute("aboveorder").value()));
        element.remove_attribute("aboveorder");
        hasAttribute = true;
    }
    if (element.attribute("beloworder")) {
        this->SetBeloworder(StrToStaffitem(element.attribute("beloworder").value()));
        element.remove_attribute("beloworder");
        hasAttribute = true;
    }
    if (element.attribute("betweenorder")) {
        this->SetBetweenorder(StrToStaffitem(element.attribute("betweenorder").value()));
        element.remove_attribute("betweenorder");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffItems::WriteStaffItems(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAboveorder()) {
        element.append_attribute("aboveorder") = StaffitemToStr(this->GetAboveorder()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBeloworder()) {
        element.append_attribute("beloworder") = StaffitemToStr(this->GetBeloworder()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBetweenorder()) {
        element.append_attribute("betweenorder") = StaffitemToStr(this->GetBetweenorder()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffItems::HasAboveorder() const
{
    return (m_aboveorder != data_STAFFITEM());
}

bool AttStaffItems::HasBeloworder() const
{
    return (m_beloworder != data_STAFFITEM());
}

bool AttStaffItems::HasBetweenorder() const
{
    return (m_betweenorder != data_STAFFITEM());
}

/* include <attbetweenorder> */

//----------------------------------------------------------------------------
// AttStaffLoc
//----------------------------------------------------------------------------

AttStaffLoc::AttStaffLoc() : Att()
{
    ResetStaffLoc();
}

AttStaffLoc::~AttStaffLoc()
{
}

void AttStaffLoc::ResetStaffLoc()
{
    m_loc = VRV_UNSET;
}

bool AttStaffLoc::ReadStaffLoc(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("loc")) {
        this->SetLoc(StrToInt(element.attribute("loc").value()));
        element.remove_attribute("loc");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffLoc::WriteStaffLoc(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLoc()) {
        element.append_attribute("loc") = IntToStr(this->GetLoc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffLoc::HasLoc() const
{
    return (m_loc != VRV_UNSET);
}

/* include <attloc> */

//----------------------------------------------------------------------------
// AttStaffLocPitched
//----------------------------------------------------------------------------

AttStaffLocPitched::AttStaffLocPitched() : Att()
{
    ResetStaffLocPitched();
}

AttStaffLocPitched::~AttStaffLocPitched()
{
}

void AttStaffLocPitched::ResetStaffLocPitched()
{
    m_ploc = PITCHNAME_NONE;
    m_oloc = -127;
}

bool AttStaffLocPitched::ReadStaffLocPitched(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("ploc")) {
        this->SetPloc(StrToPitchname(element.attribute("ploc").value()));
        element.remove_attribute("ploc");
        hasAttribute = true;
    }
    if (element.attribute("oloc")) {
        this->SetOloc(StrToOctave(element.attribute("oloc").value()));
        element.remove_attribute("oloc");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffLocPitched::WriteStaffLocPitched(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPloc()) {
        element.append_attribute("ploc") = PitchnameToStr(this->GetPloc()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOloc()) {
        element.append_attribute("oloc") = OctaveToStr(this->GetOloc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffLocPitched::HasPloc() const
{
    return (m_ploc != PITCHNAME_NONE);
}

bool AttStaffLocPitched::HasOloc() const
{
    return (m_oloc != -127);
}

/* include <attoloc> */

//----------------------------------------------------------------------------
// AttStartEndId
//----------------------------------------------------------------------------

AttStartEndId::AttStartEndId() : Att()
{
    ResetStartEndId();
}

AttStartEndId::~AttStartEndId()
{
}

void AttStartEndId::ResetStartEndId()
{
    m_endid = "";
}

bool AttStartEndId::ReadStartEndId(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("endid")) {
        this->SetEndid(StrToStr(element.attribute("endid").value()));
        element.remove_attribute("endid");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStartEndId::WriteStartEndId(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasEndid()) {
        element.append_attribute("endid") = StrToStr(this->GetEndid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStartEndId::HasEndid() const
{
    return (m_endid != "");
}

/* include <attendid> */

//----------------------------------------------------------------------------
// AttStartId
//----------------------------------------------------------------------------

AttStartId::AttStartId() : Att()
{
    ResetStartId();
}

AttStartId::~AttStartId()
{
}

void AttStartId::ResetStartId()
{
    m_startid = "";
}

bool AttStartId::ReadStartId(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("startid")) {
        this->SetStartid(StrToStr(element.attribute("startid").value()));
        element.remove_attribute("startid");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStartId::WriteStartId(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasStartid()) {
        element.append_attribute("startid") = StrToStr(this->GetStartid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStartId::HasStartid() const
{
    return (m_startid != "");
}

/* include <attstartid> */

//----------------------------------------------------------------------------
// AttStems
//----------------------------------------------------------------------------

AttStems::AttStems() : Att()
{
    ResetStems();
}

AttStems::~AttStems()
{
}

void AttStems::ResetStems()
{
    m_stemDir = STEMDIRECTION_NONE;
    m_stemLen = -1;
    m_stemMod = STEMMODIFIER_NONE;
    m_stemPos = STEMPOSITION_NONE;
    m_stemVisible = BOOLEAN_NONE;
    m_stemX = 0.0;
    m_stemY = 0.0;
}

bool AttStems::ReadStems(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("stem.dir")) {
        this->SetStemDir(StrToStemdirection(element.attribute("stem.dir").value()));
        element.remove_attribute("stem.dir");
        hasAttribute = true;
    }
    if (element.attribute("stem.len")) {
        this->SetStemLen(StrToDbl(element.attribute("stem.len").value()));
        element.remove_attribute("stem.len");
        hasAttribute = true;
    }
    if (element.attribute("stem.mod")) {
        this->SetStemMod(StrToStemmodifier(element.attribute("stem.mod").value()));
        element.remove_attribute("stem.mod");
        hasAttribute = true;
    }
    if (element.attribute("stem.pos")) {
        this->SetStemPos(StrToStemposition(element.attribute("stem.pos").value()));
        element.remove_attribute("stem.pos");
        hasAttribute = true;
    }
    if (element.attribute("stem.visible")) {
        this->SetStemVisible(StrToBoolean(element.attribute("stem.visible").value()));
        element.remove_attribute("stem.visible");
        hasAttribute = true;
    }
    if (element.attribute("stem.x")) {
        this->SetStemX(StrToDbl(element.attribute("stem.x").value()));
        element.remove_attribute("stem.x");
        hasAttribute = true;
    }
    if (element.attribute("stem.y")) {
        this->SetStemY(StrToDbl(element.attribute("stem.y").value()));
        element.remove_attribute("stem.y");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStems::WriteStems(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasStemDir()) {
        element.append_attribute("stem.dir") = StemdirectionToStr(this->GetStemDir()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStemLen()) {
        element.append_attribute("stem.len") = DblToStr(this->GetStemLen()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStemMod()) {
        element.append_attribute("stem.mod") = StemmodifierToStr(this->GetStemMod()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStemPos()) {
        element.append_attribute("stem.pos") = StempositionToStr(this->GetStemPos()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStemVisible()) {
        element.append_attribute("stem.visible") = BooleanToStr(this->GetStemVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStemX()) {
        element.append_attribute("stem.x") = DblToStr(this->GetStemX()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStemY()) {
        element.append_attribute("stem.y") = DblToStr(this->GetStemY()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStems::HasStemDir() const
{
    return (m_stemDir != STEMDIRECTION_NONE);
}

bool AttStems::HasStemLen() const
{
    return (m_stemLen != -1);
}

bool AttStems::HasStemMod() const
{
    return (m_stemMod != STEMMODIFIER_NONE);
}

bool AttStems::HasStemPos() const
{
    return (m_stemPos != STEMPOSITION_NONE);
}

bool AttStems::HasStemVisible() const
{
    return (m_stemVisible != BOOLEAN_NONE);
}

bool AttStems::HasStemX() const
{
    return (m_stemX != 0.0);
}

bool AttStems::HasStemY() const
{
    return (m_stemY != 0.0);
}

/* include <attstem.y> */

//----------------------------------------------------------------------------
// AttSylLog
//----------------------------------------------------------------------------

AttSylLog::AttSylLog() : Att()
{
    ResetSylLog();
}

AttSylLog::~AttSylLog()
{
}

void AttSylLog::ResetSylLog()
{
    m_con = sylLog_CON_NONE;
    m_wordpos = sylLog_WORDPOS_NONE;
}

bool AttSylLog::ReadSylLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("con")) {
        this->SetCon(StrToSylLogCon(element.attribute("con").value()));
        element.remove_attribute("con");
        hasAttribute = true;
    }
    if (element.attribute("wordpos")) {
        this->SetWordpos(StrToSylLogWordpos(element.attribute("wordpos").value()));
        element.remove_attribute("wordpos");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSylLog::WriteSylLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCon()) {
        element.append_attribute("con") = SylLogConToStr(this->GetCon()).c_str();
        wroteAttribute = true;
    }
    if (this->HasWordpos()) {
        element.append_attribute("wordpos") = SylLogWordposToStr(this->GetWordpos()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSylLog::HasCon() const
{
    return (m_con != sylLog_CON_NONE);
}

bool AttSylLog::HasWordpos() const
{
    return (m_wordpos != sylLog_WORDPOS_NONE);
}

/* include <attwordpos> */

//----------------------------------------------------------------------------
// AttSylText
//----------------------------------------------------------------------------

AttSylText::AttSylText() : Att()
{
    ResetSylText();
}

AttSylText::~AttSylText()
{
}

void AttSylText::ResetSylText()
{
    m_syl = "";
}

bool AttSylText::ReadSylText(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("syl")) {
        this->SetSyl(StrToStr(element.attribute("syl").value()));
        element.remove_attribute("syl");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSylText::WriteSylText(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSyl()) {
        element.append_attribute("syl") = StrToStr(this->GetSyl()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSylText::HasSyl() const
{
    return (m_syl != "");
}

/* include <attsyl> */

//----------------------------------------------------------------------------
// AttSystems
//----------------------------------------------------------------------------

AttSystems::AttSystems() : Att()
{
    ResetSystems();
}

AttSystems::~AttSystems()
{
}

void AttSystems::ResetSystems()
{
    m_systemLeftline = BOOLEAN_NONE;
    m_systemLeftmar = VRV_UNSET;
    m_systemRightmar = VRV_UNSET;
    m_systemTopmar = VRV_UNSET;
}

bool AttSystems::ReadSystems(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("system.leftline")) {
        this->SetSystemLeftline(StrToBoolean(element.attribute("system.leftline").value()));
        element.remove_attribute("system.leftline");
        hasAttribute = true;
    }
    if (element.attribute("system.leftmar")) {
        this->SetSystemLeftmar(StrToMeasurementabs(element.attribute("system.leftmar").value()));
        element.remove_attribute("system.leftmar");
        hasAttribute = true;
    }
    if (element.attribute("system.rightmar")) {
        this->SetSystemRightmar(StrToMeasurementabs(element.attribute("system.rightmar").value()));
        element.remove_attribute("system.rightmar");
        hasAttribute = true;
    }
    if (element.attribute("system.topmar")) {
        this->SetSystemTopmar(StrToMeasurementabs(element.attribute("system.topmar").value()));
        element.remove_attribute("system.topmar");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSystems::WriteSystems(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSystemLeftline()) {
        element.append_attribute("system.leftline") = BooleanToStr(this->GetSystemLeftline()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSystemLeftmar()) {
        element.append_attribute("system.leftmar") = MeasurementabsToStr(this->GetSystemLeftmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSystemRightmar()) {
        element.append_attribute("system.rightmar") = MeasurementabsToStr(this->GetSystemRightmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSystemTopmar()) {
        element.append_attribute("system.topmar") = MeasurementabsToStr(this->GetSystemTopmar()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSystems::HasSystemLeftline() const
{
    return (m_systemLeftline != BOOLEAN_NONE);
}

bool AttSystems::HasSystemLeftmar() const
{
    return (m_systemLeftmar != VRV_UNSET);
}

bool AttSystems::HasSystemRightmar() const
{
    return (m_systemRightmar != VRV_UNSET);
}

bool AttSystems::HasSystemTopmar() const
{
    return (m_systemTopmar != VRV_UNSET);
}

/* include <attsystem.topmar> */

//----------------------------------------------------------------------------
// AttTargetEval
//----------------------------------------------------------------------------

AttTargetEval::AttTargetEval() : Att()
{
    ResetTargetEval();
}

AttTargetEval::~AttTargetEval()
{
}

void AttTargetEval::ResetTargetEval()
{
    m_evaluate = targetEval_EVALUATE_NONE;
}

bool AttTargetEval::ReadTargetEval(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("evaluate")) {
        this->SetEvaluate(StrToTargetEvalEvaluate(element.attribute("evaluate").value()));
        element.remove_attribute("evaluate");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTargetEval::WriteTargetEval(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasEvaluate()) {
        element.append_attribute("evaluate") = TargetEvalEvaluateToStr(this->GetEvaluate()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTargetEval::HasEvaluate() const
{
    return (m_evaluate != targetEval_EVALUATE_NONE);
}

/* include <attevaluate> */

//----------------------------------------------------------------------------
// AttTempoLog
//----------------------------------------------------------------------------

AttTempoLog::AttTempoLog() : Att()
{
    ResetTempoLog();
}

AttTempoLog::~AttTempoLog()
{
}

void AttTempoLog::ResetTempoLog()
{
    m_func = tempoLog_FUNC_NONE;
}

bool AttTempoLog::ReadTempoLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToTempoLogFunc(element.attribute("func").value()));
        element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTempoLog::WriteTempoLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = TempoLogFuncToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTempoLog::HasFunc() const
{
    return (m_func != tempoLog_FUNC_NONE);
}

/* include <attfunc> */

//----------------------------------------------------------------------------
// AttTextRendition
//----------------------------------------------------------------------------

AttTextRendition::AttTextRendition() : Att()
{
    ResetTextRendition();
}

AttTextRendition::~AttTextRendition()
{
}

void AttTextRendition::ResetTextRendition()
{
    m_altrend = "";
    m_rend = TEXTRENDITION_NONE;
}

bool AttTextRendition::ReadTextRendition(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("altrend")) {
        this->SetAltrend(StrToStr(element.attribute("altrend").value()));
        element.remove_attribute("altrend");
        hasAttribute = true;
    }
    if (element.attribute("rend")) {
        this->SetRend(StrToTextrendition(element.attribute("rend").value()));
        element.remove_attribute("rend");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTextRendition::WriteTextRendition(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAltrend()) {
        element.append_attribute("altrend") = StrToStr(this->GetAltrend()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRend()) {
        element.append_attribute("rend") = TextrenditionToStr(this->GetRend()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTextRendition::HasAltrend() const
{
    return (m_altrend != "");
}

bool AttTextRendition::HasRend() const
{
    return (m_rend != TEXTRENDITION_NONE);
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttTextStyle
//----------------------------------------------------------------------------

AttTextStyle::AttTextStyle() : Att()
{
    ResetTextStyle();
}

AttTextStyle::~AttTextStyle()
{
}

void AttTextStyle::ResetTextStyle()
{
    m_textFam = "";
    m_textName = "";
    m_textSize = data_FONTSIZE();
    m_textStyle = FONTSTYLE_NONE;
    m_textWeight = FONTWEIGHT_NONE;
}

bool AttTextStyle::ReadTextStyle(pugi::xml_node element)
{
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
        this->SetTextSize(StrToFontsize(element.attribute("text.size").value()));
        element.remove_attribute("text.size");
        hasAttribute = true;
    }
    if (element.attribute("text.style")) {
        this->SetTextStyle(StrToFontstyle(element.attribute("text.style").value()));
        element.remove_attribute("text.style");
        hasAttribute = true;
    }
    if (element.attribute("text.weight")) {
        this->SetTextWeight(StrToFontweight(element.attribute("text.weight").value()));
        element.remove_attribute("text.weight");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTextStyle::WriteTextStyle(pugi::xml_node element)
{
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
        element.append_attribute("text.size") = FontsizeToStr(this->GetTextSize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTextStyle()) {
        element.append_attribute("text.style") = FontstyleToStr(this->GetTextStyle()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTextWeight()) {
        element.append_attribute("text.weight") = FontweightToStr(this->GetTextWeight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTextStyle::HasTextFam() const
{
    return (m_textFam != "");
}

bool AttTextStyle::HasTextName() const
{
    return (m_textName != "");
}

bool AttTextStyle::HasTextSize() const
{
    return (m_textSize.HasValue());
}

bool AttTextStyle::HasTextStyle() const
{
    return (m_textStyle != FONTSTYLE_NONE);
}

bool AttTextStyle::HasTextWeight() const
{
    return (m_textWeight != FONTWEIGHT_NONE);
}

/* include <atttext.weight> */

//----------------------------------------------------------------------------
// AttTiePresent
//----------------------------------------------------------------------------

AttTiePresent::AttTiePresent() : Att()
{
    ResetTiePresent();
}

AttTiePresent::~AttTiePresent()
{
}

void AttTiePresent::ResetTiePresent()
{
    m_tie = TIE_NONE;
}

bool AttTiePresent::ReadTiePresent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tie")) {
        this->SetTie(StrToTie(element.attribute("tie").value()));
        element.remove_attribute("tie");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTiePresent::WriteTiePresent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTie()) {
        element.append_attribute("tie") = TieToStr(this->GetTie()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTiePresent::HasTie() const
{
    return (m_tie != TIE_NONE);
}

/* include <atttie> */

//----------------------------------------------------------------------------
// AttTimestampLogical
//----------------------------------------------------------------------------

AttTimestampLogical::AttTimestampLogical() : Att()
{
    ResetTimestampLogical();
}

AttTimestampLogical::~AttTimestampLogical()
{
}

void AttTimestampLogical::ResetTimestampLogical()
{
    m_tstamp = -1.0;
}

bool AttTimestampLogical::ReadTimestampLogical(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tstamp")) {
        this->SetTstamp(StrToDbl(element.attribute("tstamp").value()));
        element.remove_attribute("tstamp");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestampLogical::WriteTimestampLogical(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTstamp()) {
        element.append_attribute("tstamp") = DblToStr(this->GetTstamp()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimestampLogical::HasTstamp() const
{
    return (m_tstamp != -1.0);
}

/* include <atttstamp> */

//----------------------------------------------------------------------------
// AttTimestamp2Logical
//----------------------------------------------------------------------------

AttTimestamp2Logical::AttTimestamp2Logical() : Att()
{
    ResetTimestamp2Logical();
}

AttTimestamp2Logical::~AttTimestamp2Logical()
{
}

void AttTimestamp2Logical::ResetTimestamp2Logical()
{
    m_tstamp2 = std::make_pair(-1, -1.0);
}

bool AttTimestamp2Logical::ReadTimestamp2Logical(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tstamp2")) {
        this->SetTstamp2(StrToMeasurebeat(element.attribute("tstamp2").value()));
        element.remove_attribute("tstamp2");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestamp2Logical::WriteTimestamp2Logical(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTstamp2()) {
        element.append_attribute("tstamp2") = MeasurebeatToStr(this->GetTstamp2()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimestamp2Logical::HasTstamp2() const
{
    return (m_tstamp2 != std::make_pair(-1, -1.0));
}

/* include <atttstamp2> */

//----------------------------------------------------------------------------
// AttTransposition
//----------------------------------------------------------------------------

AttTransposition::AttTransposition() : Att()
{
    ResetTransposition();
}

AttTransposition::~AttTransposition()
{
}

void AttTransposition::ResetTransposition()
{
    m_transDiat = 0.0;
    m_transSemi = 0.0;
}

bool AttTransposition::ReadTransposition(pugi::xml_node element)
{
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

bool AttTransposition::WriteTransposition(pugi::xml_node element)
{
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

bool AttTransposition::HasTransDiat() const
{
    return (m_transDiat != 0.0);
}

bool AttTransposition::HasTransSemi() const
{
    return (m_transSemi != 0.0);
}

/* include <atttrans.semi> */

//----------------------------------------------------------------------------
// AttTupletPresent
//----------------------------------------------------------------------------

AttTupletPresent::AttTupletPresent() : Att()
{
    ResetTupletPresent();
}

AttTupletPresent::~AttTupletPresent()
{
}

void AttTupletPresent::ResetTupletPresent()
{
    m_tuplet = "";
}

bool AttTupletPresent::ReadTupletPresent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tuplet")) {
        this->SetTuplet(StrToStr(element.attribute("tuplet").value()));
        element.remove_attribute("tuplet");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTupletPresent::WriteTupletPresent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTuplet()) {
        element.append_attribute("tuplet") = StrToStr(this->GetTuplet()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTupletPresent::HasTuplet() const
{
    return (m_tuplet != "");
}

/* include <atttuplet> */

//----------------------------------------------------------------------------
// AttTyped
//----------------------------------------------------------------------------

AttTyped::AttTyped() : Att()
{
    ResetTyped();
}

AttTyped::~AttTyped()
{
}

void AttTyped::ResetTyped()
{
    m_type = "";
}

bool AttTyped::ReadTyped(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("type")) {
        this->SetType(StrToStr(element.attribute("type").value()));
        element.remove_attribute("type");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTyped::WriteTyped(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasType()) {
        element.append_attribute("type") = StrToStr(this->GetType()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTyped::HasType() const
{
    return (m_type != "");
}

/* include <atttype> */

//----------------------------------------------------------------------------
// AttTypography
//----------------------------------------------------------------------------

AttTypography::AttTypography() : Att()
{
    ResetTypography();
}

AttTypography::~AttTypography()
{
}

void AttTypography::ResetTypography()
{
    m_fontfam = "";
    m_fontname = "";
    m_fontsize = data_FONTSIZE();
    m_fontstyle = FONTSTYLE_NONE;
    m_fontweight = FONTWEIGHT_NONE;
}

bool AttTypography::ReadTypography(pugi::xml_node element)
{
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
        this->SetFontsize(StrToFontsize(element.attribute("fontsize").value()));
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

bool AttTypography::WriteTypography(pugi::xml_node element)
{
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
        element.append_attribute("fontsize") = FontsizeToStr(this->GetFontsize()).c_str();
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

bool AttTypography::HasFontfam() const
{
    return (m_fontfam != "");
}

bool AttTypography::HasFontname() const
{
    return (m_fontname != "");
}

bool AttTypography::HasFontsize() const
{
    return (m_fontsize.HasValue());
}

bool AttTypography::HasFontstyle() const
{
    return (m_fontstyle != FONTSTYLE_NONE);
}

bool AttTypography::HasFontweight() const
{
    return (m_fontweight != FONTWEIGHT_NONE);
}

/* include <attfontweight> */

//----------------------------------------------------------------------------
// AttVerticalAlign
//----------------------------------------------------------------------------

AttVerticalAlign::AttVerticalAlign() : Att()
{
    ResetVerticalAlign();
}

AttVerticalAlign::~AttVerticalAlign()
{
}

void AttVerticalAlign::ResetVerticalAlign()
{
    m_valign = VERTICALALIGNMENT_NONE;
}

bool AttVerticalAlign::ReadVerticalAlign(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("valign")) {
        this->SetValign(StrToVerticalalignment(element.attribute("valign").value()));
        element.remove_attribute("valign");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVerticalAlign::WriteVerticalAlign(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasValign()) {
        element.append_attribute("valign") = VerticalalignmentToStr(this->GetValign()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVerticalAlign::HasValign() const
{
    return (m_valign != VERTICALALIGNMENT_NONE);
}

/* include <attvalign> */

//----------------------------------------------------------------------------
// AttVerticalGroup
//----------------------------------------------------------------------------

AttVerticalGroup::AttVerticalGroup() : Att()
{
    ResetVerticalGroup();
}

AttVerticalGroup::~AttVerticalGroup()
{
}

void AttVerticalGroup::ResetVerticalGroup()
{
    m_vgrp = 0;
}

bool AttVerticalGroup::ReadVerticalGroup(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("vgrp")) {
        this->SetVgrp(StrToInt(element.attribute("vgrp").value()));
        element.remove_attribute("vgrp");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVerticalGroup::WriteVerticalGroup(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVgrp()) {
        element.append_attribute("vgrp") = IntToStr(this->GetVgrp()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVerticalGroup::HasVgrp() const
{
    return (m_vgrp != 0);
}

/* include <attvgrp> */

//----------------------------------------------------------------------------
// AttVisibility
//----------------------------------------------------------------------------

AttVisibility::AttVisibility() : Att()
{
    ResetVisibility();
}

AttVisibility::~AttVisibility()
{
}

void AttVisibility::ResetVisibility()
{
    m_visible = BOOLEAN_NONE;
}

bool AttVisibility::ReadVisibility(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("visible")) {
        this->SetVisible(StrToBoolean(element.attribute("visible").value()));
        element.remove_attribute("visible");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisibility::WriteVisibility(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVisible()) {
        element.append_attribute("visible") = BooleanToStr(this->GetVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisibility::HasVisible() const
{
    return (m_visible != BOOLEAN_NONE);
}

/* include <attvisible> */

//----------------------------------------------------------------------------
// AttVisualOffsetHo
//----------------------------------------------------------------------------

AttVisualOffsetHo::AttVisualOffsetHo() : Att()
{
    ResetVisualOffsetHo();
}

AttVisualOffsetHo::~AttVisualOffsetHo()
{
}

void AttVisualOffsetHo::ResetVisualOffsetHo()
{
    m_ho = VRV_UNSET;
}

bool AttVisualOffsetHo::ReadVisualOffsetHo(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("ho")) {
        this->SetHo(StrToMeasurementrel(element.attribute("ho").value()));
        element.remove_attribute("ho");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualOffsetHo::WriteVisualOffsetHo(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasHo()) {
        element.append_attribute("ho") = MeasurementrelToStr(this->GetHo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualOffsetHo::HasHo() const
{
    return (m_ho != VRV_UNSET);
}

/* include <attho> */

//----------------------------------------------------------------------------
// AttVisualOffsetTo
//----------------------------------------------------------------------------

AttVisualOffsetTo::AttVisualOffsetTo() : Att()
{
    ResetVisualOffsetTo();
}

AttVisualOffsetTo::~AttVisualOffsetTo()
{
}

void AttVisualOffsetTo::ResetVisualOffsetTo()
{
    m_to = 0.0;
}

bool AttVisualOffsetTo::ReadVisualOffsetTo(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("to")) {
        this->SetTo(StrToDbl(element.attribute("to").value()));
        element.remove_attribute("to");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualOffsetTo::WriteVisualOffsetTo(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTo()) {
        element.append_attribute("to") = DblToStr(this->GetTo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualOffsetTo::HasTo() const
{
    return (m_to != 0.0);
}

/* include <attto> */

//----------------------------------------------------------------------------
// AttVisualOffsetVo
//----------------------------------------------------------------------------

AttVisualOffsetVo::AttVisualOffsetVo() : Att()
{
    ResetVisualOffsetVo();
}

AttVisualOffsetVo::~AttVisualOffsetVo()
{
}

void AttVisualOffsetVo::ResetVisualOffsetVo()
{
    m_vo = VRV_UNSET;
}

bool AttVisualOffsetVo::ReadVisualOffsetVo(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("vo")) {
        this->SetVo(StrToMeasurementrel(element.attribute("vo").value()));
        element.remove_attribute("vo");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualOffsetVo::WriteVisualOffsetVo(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVo()) {
        element.append_attribute("vo") = MeasurementrelToStr(this->GetVo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualOffsetVo::HasVo() const
{
    return (m_vo != VRV_UNSET);
}

/* include <attvo> */

//----------------------------------------------------------------------------
// AttVisualOffset2Ho
//----------------------------------------------------------------------------

AttVisualOffset2Ho::AttVisualOffset2Ho() : Att()
{
    ResetVisualOffset2Ho();
}

AttVisualOffset2Ho::~AttVisualOffset2Ho()
{
}

void AttVisualOffset2Ho::ResetVisualOffset2Ho()
{
    m_startho = VRV_UNSET;
    m_endho = VRV_UNSET;
}

bool AttVisualOffset2Ho::ReadVisualOffset2Ho(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("startho")) {
        this->SetStartho(StrToMeasurementrel(element.attribute("startho").value()));
        element.remove_attribute("startho");
        hasAttribute = true;
    }
    if (element.attribute("endho")) {
        this->SetEndho(StrToMeasurementrel(element.attribute("endho").value()));
        element.remove_attribute("endho");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualOffset2Ho::WriteVisualOffset2Ho(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasStartho()) {
        element.append_attribute("startho") = MeasurementrelToStr(this->GetStartho()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndho()) {
        element.append_attribute("endho") = MeasurementrelToStr(this->GetEndho()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualOffset2Ho::HasStartho() const
{
    return (m_startho != VRV_UNSET);
}

bool AttVisualOffset2Ho::HasEndho() const
{
    return (m_endho != VRV_UNSET);
}

/* include <attendho> */

//----------------------------------------------------------------------------
// AttVisualOffset2To
//----------------------------------------------------------------------------

AttVisualOffset2To::AttVisualOffset2To() : Att()
{
    ResetVisualOffset2To();
}

AttVisualOffset2To::~AttVisualOffset2To()
{
}

void AttVisualOffset2To::ResetVisualOffset2To()
{
    m_startto = 0.0;
    m_endto = 0.0;
}

bool AttVisualOffset2To::ReadVisualOffset2To(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("startto")) {
        this->SetStartto(StrToDbl(element.attribute("startto").value()));
        element.remove_attribute("startto");
        hasAttribute = true;
    }
    if (element.attribute("endto")) {
        this->SetEndto(StrToDbl(element.attribute("endto").value()));
        element.remove_attribute("endto");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualOffset2To::WriteVisualOffset2To(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasStartto()) {
        element.append_attribute("startto") = DblToStr(this->GetStartto()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndto()) {
        element.append_attribute("endto") = DblToStr(this->GetEndto()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualOffset2To::HasStartto() const
{
    return (m_startto != 0.0);
}

bool AttVisualOffset2To::HasEndto() const
{
    return (m_endto != 0.0);
}

/* include <attendto> */

//----------------------------------------------------------------------------
// AttVisualOffset2Vo
//----------------------------------------------------------------------------

AttVisualOffset2Vo::AttVisualOffset2Vo() : Att()
{
    ResetVisualOffset2Vo();
}

AttVisualOffset2Vo::~AttVisualOffset2Vo()
{
}

void AttVisualOffset2Vo::ResetVisualOffset2Vo()
{
    m_startvo = VRV_UNSET;
    m_endvo = VRV_UNSET;
}

bool AttVisualOffset2Vo::ReadVisualOffset2Vo(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("startvo")) {
        this->SetStartvo(StrToMeasurementrel(element.attribute("startvo").value()));
        element.remove_attribute("startvo");
        hasAttribute = true;
    }
    if (element.attribute("endvo")) {
        this->SetEndvo(StrToMeasurementrel(element.attribute("endvo").value()));
        element.remove_attribute("endvo");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualOffset2Vo::WriteVisualOffset2Vo(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasStartvo()) {
        element.append_attribute("startvo") = MeasurementrelToStr(this->GetStartvo()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndvo()) {
        element.append_attribute("endvo") = MeasurementrelToStr(this->GetEndvo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualOffset2Vo::HasStartvo() const
{
    return (m_startvo != VRV_UNSET);
}

bool AttVisualOffset2Vo::HasEndvo() const
{
    return (m_endvo != VRV_UNSET);
}

/* include <attendvo> */

//----------------------------------------------------------------------------
// AttVoltaGroupingSym
//----------------------------------------------------------------------------

AttVoltaGroupingSym::AttVoltaGroupingSym() : Att()
{
    ResetVoltaGroupingSym();
}

AttVoltaGroupingSym::~AttVoltaGroupingSym()
{
}

void AttVoltaGroupingSym::ResetVoltaGroupingSym()
{
    m_voltasym = voltaGroupingSym_VOLTASYM_NONE;
}

bool AttVoltaGroupingSym::ReadVoltaGroupingSym(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("voltasym")) {
        this->SetVoltasym(StrToVoltaGroupingSymVoltasym(element.attribute("voltasym").value()));
        element.remove_attribute("voltasym");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVoltaGroupingSym::WriteVoltaGroupingSym(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVoltasym()) {
        element.append_attribute("voltasym") = VoltaGroupingSymVoltasymToStr(this->GetVoltasym()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVoltaGroupingSym::HasVoltasym() const
{
    return (m_voltasym != voltaGroupingSym_VOLTASYM_NONE);
}

/* include <attvoltasym> */

//----------------------------------------------------------------------------
// AttWhitespace
//----------------------------------------------------------------------------

AttWhitespace::AttWhitespace() : Att()
{
    ResetWhitespace();
}

AttWhitespace::~AttWhitespace()
{
}

void AttWhitespace::ResetWhitespace()
{
    m_space = "";
}

bool AttWhitespace::ReadWhitespace(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("xml:space")) {
        this->SetSpace(StrToStr(element.attribute("xml:space").value()));
        element.remove_attribute("xml:space");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttWhitespace::WriteWhitespace(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSpace()) {
        element.append_attribute("xml:space") = StrToStr(this->GetSpace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttWhitespace::HasSpace() const
{
    return (m_space != "");
}

/* include <attspace> */

//----------------------------------------------------------------------------
// AttWidth
//----------------------------------------------------------------------------

AttWidth::AttWidth() : Att()
{
    ResetWidth();
}

AttWidth::~AttWidth()
{
}

void AttWidth::ResetWidth()
{
    m_width = VRV_UNSET;
}

bool AttWidth::ReadWidth(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("width")) {
        this->SetWidth(StrToMeasurementabs(element.attribute("width").value()));
        element.remove_attribute("width");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttWidth::WriteWidth(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasWidth()) {
        element.append_attribute("width") = MeasurementabsToStr(this->GetWidth()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttWidth::HasWidth() const
{
    return (m_width != VRV_UNSET);
}

/* include <attwidth> */

//----------------------------------------------------------------------------
// AttXy
//----------------------------------------------------------------------------

AttXy::AttXy() : Att()
{
    ResetXy();
}

AttXy::~AttXy()
{
}

void AttXy::ResetXy()
{
    m_x = 0.0;
    m_y = 0.0;
}

bool AttXy::ReadXy(pugi::xml_node element)
{
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

bool AttXy::WriteXy(pugi::xml_node element)
{
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

bool AttXy::HasX() const
{
    return (m_x != 0.0);
}

bool AttXy::HasY() const
{
    return (m_y != 0.0);
}

/* include <atty> */

//----------------------------------------------------------------------------
// AttXy2
//----------------------------------------------------------------------------

AttXy2::AttXy2() : Att()
{
    ResetXy2();
}

AttXy2::~AttXy2()
{
}

void AttXy2::ResetXy2()
{
    m_x2 = 0.0;
    m_y2 = 0.0;
}

bool AttXy2::ReadXy2(pugi::xml_node element)
{
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

bool AttXy2::WriteXy2(pugi::xml_node element)
{
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

bool AttXy2::HasX2() const
{
    return (m_x2 != 0.0);
}

bool AttXy2::HasY2() const
{
    return (m_y2 != 0.0);
}

/* include <atty2> */

bool Att::SetShared(Object *element, std::string attrType, std::string attrValue)
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
            att->SetBulge(att->StrToDbl(attrValue));
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
        if (attrType == "dynam.dist") {
            att->SetDynamDist(att->StrToMeasurementrel(attrValue));
            return true;
        }
        if (attrType == "harm.dist") {
            att->SetHarmDist(att->StrToMeasurementrel(attrValue));
            return true;
        }
        if (attrType == "text.dist") {
            att->SetTextDist(att->StrToMeasurementrel(attrValue));
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
    if (element->HasAttClass(ATT_DURATIONLOGICAL)) {
        AttDurationLogical *att = dynamic_cast<AttDurationLogical *>(element);
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
            att->SetHeight(att->StrToMeasurementabs(attrValue));
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
        if (attrType == "xml:translit") {
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
            att->SetLyricAlign(att->StrToMeasurementrel(attrValue));
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
    if (element->HasAttClass(ATT_MENSURLOG)) {
        AttMensurLog *att = dynamic_cast<AttMensurLog *>(element);
        assert(att);
        if (attrType == "dot") {
            att->SetDot(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "sign") {
            att->SetSign(att->StrToMensurationsign(attrValue));
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
            att->SetCount(att->StrToInt(attrValue));
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
            att->SetMeterCount(att->StrToInt(attrValue));
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
            att->SetMm(att->StrToInt(attrValue));
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
    if (element->HasAttClass(ATT_ORIGINTIMESTAMPLOGICAL)) {
        AttOriginTimestampLogical *att = dynamic_cast<AttOriginTimestampLogical *>(element);
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
            att->SetPageHeight(att->StrToMeasurementabs(attrValue));
            return true;
        }
        if (attrType == "page.width") {
            att->SetPageWidth(att->StrToMeasurementabs(attrValue));
            return true;
        }
        if (attrType == "page.topmar") {
            att->SetPageTopmar(att->StrToMeasurementabs(attrValue));
            return true;
        }
        if (attrType == "page.botmar") {
            att->SetPageBotmar(att->StrToMeasurementabs(attrValue));
            return true;
        }
        if (attrType == "page.leftmar") {
            att->SetPageLeftmar(att->StrToMeasurementabs(attrValue));
            return true;
        }
        if (attrType == "page.rightmar") {
            att->SetPageRightmar(att->StrToMeasurementabs(attrValue));
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
    if (element->HasAttClass(ATT_PLACEMENT)) {
        AttPlacement *att = dynamic_cast<AttPlacement *>(element);
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
        if (attrType == "xlink:target") {
            att->SetTarget(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "xlink:targettype") {
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
            att->SetSpacingStaff(att->StrToMeasurementrel(attrValue));
            return true;
        }
        if (attrType == "spacing.system") {
            att->SetSpacingSystem(att->StrToMeasurementrel(attrValue));
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
            att->SetSystemLeftmar(att->StrToMeasurementabs(attrValue));
            return true;
        }
        if (attrType == "system.rightmar") {
            att->SetSystemRightmar(att->StrToMeasurementabs(attrValue));
            return true;
        }
        if (attrType == "system.topmar") {
            att->SetSystemTopmar(att->StrToMeasurementabs(attrValue));
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
    if (element->HasAttClass(ATT_TIMESTAMPLOGICAL)) {
        AttTimestampLogical *att = dynamic_cast<AttTimestampLogical *>(element);
        assert(att);
        if (attrType == "tstamp") {
            att->SetTstamp(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMP2LOGICAL)) {
        AttTimestamp2Logical *att = dynamic_cast<AttTimestamp2Logical *>(element);
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
            att->SetTransDiat(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "trans.semi") {
            att->SetTransSemi(att->StrToDbl(attrValue));
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
            att->SetHo(att->StrToMeasurementrel(attrValue));
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
            att->SetVo(att->StrToMeasurementrel(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSET2HO)) {
        AttVisualOffset2Ho *att = dynamic_cast<AttVisualOffset2Ho *>(element);
        assert(att);
        if (attrType == "startho") {
            att->SetStartho(att->StrToMeasurementrel(attrValue));
            return true;
        }
        if (attrType == "endho") {
            att->SetEndho(att->StrToMeasurementrel(attrValue));
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
            att->SetStartvo(att->StrToMeasurementrel(attrValue));
            return true;
        }
        if (attrType == "endvo") {
            att->SetEndvo(att->StrToMeasurementrel(attrValue));
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
            att->SetWidth(att->StrToMeasurementabs(attrValue));
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

void Att::GetShared(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ACCIDLOG)) {
        const AttAccidLog *att = dynamic_cast<const AttAccidLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back(std::make_pair("func", att->AccidLogFuncToStr(att->GetFunc())));
        }
    }
    if (element->HasAttClass(ATT_ACCIDENTAL)) {
        const AttAccidental *att = dynamic_cast<const AttAccidental *>(element);
        assert(att);
        if (att->HasAccid()) {
            attributes->push_back(std::make_pair("accid", att->AccidentalWrittenToStr(att->GetAccid())));
        }
    }
    if (element->HasAttClass(ATT_ARTICULATION)) {
        const AttArticulation *att = dynamic_cast<const AttArticulation *>(element);
        assert(att);
        if (att->HasArtic()) {
            attributes->push_back(std::make_pair("artic", att->ArticulationListToStr(att->GetArtic())));
        }
    }
    if (element->HasAttClass(ATT_ATTACCALOG)) {
        const AttAttaccaLog *att = dynamic_cast<const AttAttaccaLog *>(element);
        assert(att);
        if (att->HasTarget()) {
            attributes->push_back(std::make_pair("target", att->StrToStr(att->GetTarget())));
        }
    }
    if (element->HasAttClass(ATT_AUDIENCE)) {
        const AttAudience *att = dynamic_cast<const AttAudience *>(element);
        assert(att);
        if (att->HasAudience()) {
            attributes->push_back(std::make_pair("audience", att->AudienceAudienceToStr(att->GetAudience())));
        }
    }
    if (element->HasAttClass(ATT_AUGMENTDOTS)) {
        const AttAugmentDots *att = dynamic_cast<const AttAugmentDots *>(element);
        assert(att);
        if (att->HasDots()) {
            attributes->push_back(std::make_pair("dots", att->IntToStr(att->GetDots())));
        }
    }
    if (element->HasAttClass(ATT_AUTHORIZED)) {
        const AttAuthorized *att = dynamic_cast<const AttAuthorized *>(element);
        assert(att);
        if (att->HasAuth()) {
            attributes->push_back(std::make_pair("auth", att->StrToStr(att->GetAuth())));
        }
        if (att->HasAuthUri()) {
            attributes->push_back(std::make_pair("auth.uri", att->StrToStr(att->GetAuthUri())));
        }
    }
    if (element->HasAttClass(ATT_BARLINELOG)) {
        const AttBarLineLog *att = dynamic_cast<const AttBarLineLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->BarrenditionToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass(ATT_BARRING)) {
        const AttBarring *att = dynamic_cast<const AttBarring *>(element);
        assert(att);
        if (att->HasBarLen()) {
            attributes->push_back(std::make_pair("bar.len", att->DblToStr(att->GetBarLen())));
        }
        if (att->HasBarMethod()) {
            attributes->push_back(std::make_pair("bar.method", att->BarmethodToStr(att->GetBarMethod())));
        }
        if (att->HasBarPlace()) {
            attributes->push_back(std::make_pair("bar.place", att->IntToStr(att->GetBarPlace())));
        }
    }
    if (element->HasAttClass(ATT_BASIC)) {
        const AttBasic *att = dynamic_cast<const AttBasic *>(element);
        assert(att);
        if (att->HasBase()) {
            attributes->push_back(std::make_pair("xml:base", att->StrToStr(att->GetBase())));
        }
    }
    if (element->HasAttClass(ATT_BIBL)) {
        const AttBibl *att = dynamic_cast<const AttBibl *>(element);
        assert(att);
        if (att->HasAnalog()) {
            attributes->push_back(std::make_pair("analog", att->StrToStr(att->GetAnalog())));
        }
    }
    if (element->HasAttClass(ATT_CALENDARED)) {
        const AttCalendared *att = dynamic_cast<const AttCalendared *>(element);
        assert(att);
        if (att->HasCalendar()) {
            attributes->push_back(std::make_pair("calendar", att->StrToStr(att->GetCalendar())));
        }
    }
    if (element->HasAttClass(ATT_CANONICAL)) {
        const AttCanonical *att = dynamic_cast<const AttCanonical *>(element);
        assert(att);
        if (att->HasCodedval()) {
            attributes->push_back(std::make_pair("codedval", att->StrToStr(att->GetCodedval())));
        }
    }
    if (element->HasAttClass(ATT_CLASSED)) {
        const AttClassed *att = dynamic_cast<const AttClassed *>(element);
        assert(att);
        if (att->HasClass()) {
            attributes->push_back(std::make_pair("class", att->StrToStr(att->GetClass())));
        }
    }
    if (element->HasAttClass(ATT_CLEFLOG)) {
        const AttClefLog *att = dynamic_cast<const AttClefLog *>(element);
        assert(att);
        if (att->HasCautionary()) {
            attributes->push_back(std::make_pair("cautionary", att->BooleanToStr(att->GetCautionary())));
        }
    }
    if (element->HasAttClass(ATT_CLEFSHAPE)) {
        const AttClefShape *att = dynamic_cast<const AttClefShape *>(element);
        assert(att);
        if (att->HasShape()) {
            attributes->push_back(std::make_pair("shape", att->ClefshapeToStr(att->GetShape())));
        }
    }
    if (element->HasAttClass(ATT_CLEFFINGLOG)) {
        const AttCleffingLog *att = dynamic_cast<const AttCleffingLog *>(element);
        assert(att);
        if (att->HasClefShape()) {
            attributes->push_back(std::make_pair("clef.shape", att->ClefshapeToStr(att->GetClefShape())));
        }
        if (att->HasClefLine()) {
            attributes->push_back(std::make_pair("clef.line", att->IntToStr(att->GetClefLine())));
        }
        if (att->HasClefDis()) {
            attributes->push_back(std::make_pair("clef.dis", att->OctaveDisToStr(att->GetClefDis())));
        }
        if (att->HasClefDisPlace()) {
            attributes->push_back(std::make_pair("clef.dis.place", att->StaffrelBasicToStr(att->GetClefDisPlace())));
        }
    }
    if (element->HasAttClass(ATT_COLOR)) {
        const AttColor *att = dynamic_cast<const AttColor *>(element);
        assert(att);
        if (att->HasColor()) {
            attributes->push_back(std::make_pair("color", att->StrToStr(att->GetColor())));
        }
    }
    if (element->HasAttClass(ATT_COLORATION)) {
        const AttColoration *att = dynamic_cast<const AttColoration *>(element);
        assert(att);
        if (att->HasColored()) {
            attributes->push_back(std::make_pair("colored", att->BooleanToStr(att->GetColored())));
        }
    }
    if (element->HasAttClass(ATT_COORDINATED)) {
        const AttCoordinated *att = dynamic_cast<const AttCoordinated *>(element);
        assert(att);
        if (att->HasUlx()) {
            attributes->push_back(std::make_pair("ulx", att->IntToStr(att->GetUlx())));
        }
        if (att->HasUly()) {
            attributes->push_back(std::make_pair("uly", att->IntToStr(att->GetUly())));
        }
        if (att->HasLrx()) {
            attributes->push_back(std::make_pair("lrx", att->IntToStr(att->GetLrx())));
        }
        if (att->HasLry()) {
            attributes->push_back(std::make_pair("lry", att->IntToStr(att->GetLry())));
        }
    }
    if (element->HasAttClass(ATT_CUE)) {
        const AttCue *att = dynamic_cast<const AttCue *>(element);
        assert(att);
        if (att->HasCue()) {
            attributes->push_back(std::make_pair("cue", att->BooleanToStr(att->GetCue())));
        }
    }
    if (element->HasAttClass(ATT_CURVATURE)) {
        const AttCurvature *att = dynamic_cast<const AttCurvature *>(element);
        assert(att);
        if (att->HasBezier()) {
            attributes->push_back(std::make_pair("bezier", att->StrToStr(att->GetBezier())));
        }
        if (att->HasBulge()) {
            attributes->push_back(std::make_pair("bulge", att->DblToStr(att->GetBulge())));
        }
        if (att->HasCurvedir()) {
            attributes->push_back(std::make_pair("curvedir", att->CurvatureCurvedirToStr(att->GetCurvedir())));
        }
    }
    if (element->HasAttClass(ATT_CURVEREND)) {
        const AttCurveRend *att = dynamic_cast<const AttCurveRend *>(element);
        assert(att);
        if (att->HasLform()) {
            attributes->push_back(std::make_pair("lform", att->LineformToStr(att->GetLform())));
        }
        if (att->HasLwidth()) {
            attributes->push_back(std::make_pair("lwidth", att->LinewidthToStr(att->GetLwidth())));
        }
    }
    if (element->HasAttClass(ATT_CUSTOSLOG)) {
        const AttCustosLog *att = dynamic_cast<const AttCustosLog *>(element);
        assert(att);
        if (att->HasTarget()) {
            attributes->push_back(std::make_pair("target", att->StrToStr(att->GetTarget())));
        }
    }
    if (element->HasAttClass(ATT_DATAPOINTING)) {
        const AttDataPointing *att = dynamic_cast<const AttDataPointing *>(element);
        assert(att);
        if (att->HasData()) {
            attributes->push_back(std::make_pair("data", att->StrToStr(att->GetData())));
        }
    }
    if (element->HasAttClass(ATT_DATABLE)) {
        const AttDatable *att = dynamic_cast<const AttDatable *>(element);
        assert(att);
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
    if (element->HasAttClass(ATT_DISTANCES)) {
        const AttDistances *att = dynamic_cast<const AttDistances *>(element);
        assert(att);
        if (att->HasDynamDist()) {
            attributes->push_back(std::make_pair("dynam.dist", att->MeasurementrelToStr(att->GetDynamDist())));
        }
        if (att->HasHarmDist()) {
            attributes->push_back(std::make_pair("harm.dist", att->MeasurementrelToStr(att->GetHarmDist())));
        }
        if (att->HasTextDist()) {
            attributes->push_back(std::make_pair("text.dist", att->MeasurementrelToStr(att->GetTextDist())));
        }
    }
    if (element->HasAttClass(ATT_DOTLOG)) {
        const AttDotLog *att = dynamic_cast<const AttDotLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->DotLogFormToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass(ATT_DURATIONADDITIVE)) {
        const AttDurationAdditive *att = dynamic_cast<const AttDurationAdditive *>(element);
        assert(att);
        if (att->HasDur()) {
            attributes->push_back(std::make_pair("dur", att->DurationToStr(att->GetDur())));
        }
    }
    if (element->HasAttClass(ATT_DURATIONDEFAULT)) {
        const AttDurationDefault *att = dynamic_cast<const AttDurationDefault *>(element);
        assert(att);
        if (att->HasDurDefault()) {
            attributes->push_back(std::make_pair("dur.default", att->DurationToStr(att->GetDurDefault())));
        }
        if (att->HasNumDefault()) {
            attributes->push_back(std::make_pair("num.default", att->IntToStr(att->GetNumDefault())));
        }
        if (att->HasNumbaseDefault()) {
            attributes->push_back(std::make_pair("numbase.default", att->IntToStr(att->GetNumbaseDefault())));
        }
    }
    if (element->HasAttClass(ATT_DURATIONLOGICAL)) {
        const AttDurationLogical *att = dynamic_cast<const AttDurationLogical *>(element);
        assert(att);
        if (att->HasDur()) {
            attributes->push_back(std::make_pair("dur", att->DurationToStr(att->GetDur())));
        }
    }
    if (element->HasAttClass(ATT_DURATIONRATIO)) {
        const AttDurationRatio *att = dynamic_cast<const AttDurationRatio *>(element);
        assert(att);
        if (att->HasNum()) {
            attributes->push_back(std::make_pair("num", att->IntToStr(att->GetNum())));
        }
        if (att->HasNumbase()) {
            attributes->push_back(std::make_pair("numbase", att->IntToStr(att->GetNumbase())));
        }
    }
    if (element->HasAttClass(ATT_ENCLOSINGCHARS)) {
        const AttEnclosingChars *att = dynamic_cast<const AttEnclosingChars *>(element);
        assert(att);
        if (att->HasEnclose()) {
            attributes->push_back(std::make_pair("enclose", att->EnclosureToStr(att->GetEnclose())));
        }
    }
    if (element->HasAttClass(ATT_ENDINGS)) {
        const AttEndings *att = dynamic_cast<const AttEndings *>(element);
        assert(att);
        if (att->HasEndingRend()) {
            attributes->push_back(std::make_pair("ending.rend", att->EndingsEndingrendToStr(att->GetEndingRend())));
        }
    }
    if (element->HasAttClass(ATT_EVIDENCE)) {
        const AttEvidence *att = dynamic_cast<const AttEvidence *>(element);
        assert(att);
        if (att->HasCert()) {
            attributes->push_back(std::make_pair("cert", att->CertaintyToStr(att->GetCert())));
        }
        if (att->HasEvidence()) {
            attributes->push_back(std::make_pair("evidence", att->StrToStr(att->GetEvidence())));
        }
    }
    if (element->HasAttClass(ATT_EXTENDER)) {
        const AttExtender *att = dynamic_cast<const AttExtender *>(element);
        assert(att);
        if (att->HasExtender()) {
            attributes->push_back(std::make_pair("extender", att->BooleanToStr(att->GetExtender())));
        }
    }
    if (element->HasAttClass(ATT_EXTENT)) {
        const AttExtent *att = dynamic_cast<const AttExtent *>(element);
        assert(att);
        if (att->HasExtent()) {
            attributes->push_back(std::make_pair("extent", att->StrToStr(att->GetExtent())));
        }
    }
    if (element->HasAttClass(ATT_FERMATAPRESENT)) {
        const AttFermataPresent *att = dynamic_cast<const AttFermataPresent *>(element);
        assert(att);
        if (att->HasFermata()) {
            attributes->push_back(std::make_pair("fermata", att->StaffrelBasicToStr(att->GetFermata())));
        }
    }
    if (element->HasAttClass(ATT_FILING)) {
        const AttFiling *att = dynamic_cast<const AttFiling *>(element);
        assert(att);
        if (att->HasNonfiling()) {
            attributes->push_back(std::make_pair("nonfiling", att->IntToStr(att->GetNonfiling())));
        }
    }
    if (element->HasAttClass(ATT_GRPSYMLOG)) {
        const AttGrpSymLog *att = dynamic_cast<const AttGrpSymLog *>(element);
        assert(att);
        if (att->HasLevel()) {
            attributes->push_back(std::make_pair("level", att->IntToStr(att->GetLevel())));
        }
    }
    if (element->HasAttClass(ATT_HANDIDENT)) {
        const AttHandIdent *att = dynamic_cast<const AttHandIdent *>(element);
        assert(att);
        if (att->HasHand()) {
            attributes->push_back(std::make_pair("hand", att->StrToStr(att->GetHand())));
        }
    }
    if (element->HasAttClass(ATT_HEIGHT)) {
        const AttHeight *att = dynamic_cast<const AttHeight *>(element);
        assert(att);
        if (att->HasHeight()) {
            attributes->push_back(std::make_pair("height", att->MeasurementabsToStr(att->GetHeight())));
        }
    }
    if (element->HasAttClass(ATT_HORIZONTALALIGN)) {
        const AttHorizontalAlign *att = dynamic_cast<const AttHorizontalAlign *>(element);
        assert(att);
        if (att->HasHalign()) {
            attributes->push_back(std::make_pair("halign", att->HorizontalalignmentToStr(att->GetHalign())));
        }
    }
    if (element->HasAttClass(ATT_INTERNETMEDIA)) {
        const AttInternetMedia *att = dynamic_cast<const AttInternetMedia *>(element);
        assert(att);
        if (att->HasMimetype()) {
            attributes->push_back(std::make_pair("mimetype", att->StrToStr(att->GetMimetype())));
        }
    }
    if (element->HasAttClass(ATT_JOINED)) {
        const AttJoined *att = dynamic_cast<const AttJoined *>(element);
        assert(att);
        if (att->HasJoin()) {
            attributes->push_back(std::make_pair("join", att->StrToStr(att->GetJoin())));
        }
    }
    if (element->HasAttClass(ATT_KEYSIGLOG)) {
        const AttKeySigLog *att = dynamic_cast<const AttKeySigLog *>(element);
        assert(att);
        if (att->HasSig()) {
            attributes->push_back(std::make_pair("sig", att->KeysignatureToStr(att->GetSig())));
        }
    }
    if (element->HasAttClass(ATT_KEYSIGDEFAULTLOG)) {
        const AttKeySigDefaultLog *att = dynamic_cast<const AttKeySigDefaultLog *>(element);
        assert(att);
        if (att->HasKeySig()) {
            attributes->push_back(std::make_pair("key.sig", att->KeysignatureToStr(att->GetKeySig())));
        }
    }
    if (element->HasAttClass(ATT_LABELLED)) {
        const AttLabelled *att = dynamic_cast<const AttLabelled *>(element);
        assert(att);
        if (att->HasLabel()) {
            attributes->push_back(std::make_pair("label", att->StrToStr(att->GetLabel())));
        }
    }
    if (element->HasAttClass(ATT_LANG)) {
        const AttLang *att = dynamic_cast<const AttLang *>(element);
        assert(att);
        if (att->HasLang()) {
            attributes->push_back(std::make_pair("xml:lang", att->StrToStr(att->GetLang())));
        }
        if (att->HasTranslit()) {
            attributes->push_back(std::make_pair("xml:translit", att->StrToStr(att->GetTranslit())));
        }
    }
    if (element->HasAttClass(ATT_LAYERLOG)) {
        const AttLayerLog *att = dynamic_cast<const AttLayerLog *>(element);
        assert(att);
        if (att->HasDef()) {
            attributes->push_back(std::make_pair("def", att->StrToStr(att->GetDef())));
        }
    }
    if (element->HasAttClass(ATT_LAYERIDENT)) {
        const AttLayerIdent *att = dynamic_cast<const AttLayerIdent *>(element);
        assert(att);
        if (att->HasLayer()) {
            attributes->push_back(std::make_pair("layer", att->IntToStr(att->GetLayer())));
        }
    }
    if (element->HasAttClass(ATT_LINELOC)) {
        const AttLineLoc *att = dynamic_cast<const AttLineLoc *>(element);
        assert(att);
        if (att->HasLine()) {
            attributes->push_back(std::make_pair("line", att->IntToStr(att->GetLine())));
        }
    }
    if (element->HasAttClass(ATT_LINEREND)) {
        const AttLineRend *att = dynamic_cast<const AttLineRend *>(element);
        assert(att);
        if (att->HasLendsym()) {
            attributes->push_back(std::make_pair("lendsym", att->LinestartendsymbolToStr(att->GetLendsym())));
        }
        if (att->HasLendsymSize()) {
            attributes->push_back(std::make_pair("lendsym.size", att->IntToStr(att->GetLendsymSize())));
        }
        if (att->HasLstartsym()) {
            attributes->push_back(std::make_pair("lstartsym", att->LinestartendsymbolToStr(att->GetLstartsym())));
        }
        if (att->HasLstartsymSize()) {
            attributes->push_back(std::make_pair("lstartsym.size", att->IntToStr(att->GetLstartsymSize())));
        }
    }
    if (element->HasAttClass(ATT_LINERENDBASE)) {
        const AttLineRendBase *att = dynamic_cast<const AttLineRendBase *>(element);
        assert(att);
        if (att->HasLform()) {
            attributes->push_back(std::make_pair("lform", att->LineformToStr(att->GetLform())));
        }
        if (att->HasLsegs()) {
            attributes->push_back(std::make_pair("lsegs", att->IntToStr(att->GetLsegs())));
        }
        if (att->HasLwidth()) {
            attributes->push_back(std::make_pair("lwidth", att->LinewidthToStr(att->GetLwidth())));
        }
    }
    if (element->HasAttClass(ATT_LINKING)) {
        const AttLinking *att = dynamic_cast<const AttLinking *>(element);
        assert(att);
        if (att->HasCopyof()) {
            attributes->push_back(std::make_pair("copyof", att->StrToStr(att->GetCopyof())));
        }
        if (att->HasCorresp()) {
            attributes->push_back(std::make_pair("corresp", att->StrToStr(att->GetCorresp())));
        }
        if (att->HasFollows()) {
            attributes->push_back(std::make_pair("follows", att->StrToStr(att->GetFollows())));
        }
        if (att->HasNext()) {
            attributes->push_back(std::make_pair("next", att->StrToStr(att->GetNext())));
        }
        if (att->HasPrecedes()) {
            attributes->push_back(std::make_pair("precedes", att->StrToStr(att->GetPrecedes())));
        }
        if (att->HasPrev()) {
            attributes->push_back(std::make_pair("prev", att->StrToStr(att->GetPrev())));
        }
        if (att->HasSameas()) {
            attributes->push_back(std::make_pair("sameas", att->StrToStr(att->GetSameas())));
        }
        if (att->HasSynch()) {
            attributes->push_back(std::make_pair("synch", att->StrToStr(att->GetSynch())));
        }
    }
    if (element->HasAttClass(ATT_LYRICSTYLE)) {
        const AttLyricStyle *att = dynamic_cast<const AttLyricStyle *>(element);
        assert(att);
        if (att->HasLyricAlign()) {
            attributes->push_back(std::make_pair("lyric.align", att->MeasurementrelToStr(att->GetLyricAlign())));
        }
        if (att->HasLyricFam()) {
            attributes->push_back(std::make_pair("lyric.fam", att->StrToStr(att->GetLyricFam())));
        }
        if (att->HasLyricName()) {
            attributes->push_back(std::make_pair("lyric.name", att->StrToStr(att->GetLyricName())));
        }
        if (att->HasLyricSize()) {
            attributes->push_back(std::make_pair("lyric.size", att->FontsizeToStr(att->GetLyricSize())));
        }
        if (att->HasLyricStyle()) {
            attributes->push_back(std::make_pair("lyric.style", att->FontstyleToStr(att->GetLyricStyle())));
        }
        if (att->HasLyricWeight()) {
            attributes->push_back(std::make_pair("lyric.weight", att->FontweightToStr(att->GetLyricWeight())));
        }
    }
    if (element->HasAttClass(ATT_MEASURENUMBERS)) {
        const AttMeasureNumbers *att = dynamic_cast<const AttMeasureNumbers *>(element);
        assert(att);
        if (att->HasMnumVisible()) {
            attributes->push_back(std::make_pair("mnum.visible", att->BooleanToStr(att->GetMnumVisible())));
        }
    }
    if (element->HasAttClass(ATT_MEASUREMENT)) {
        const AttMeasurement *att = dynamic_cast<const AttMeasurement *>(element);
        assert(att);
        if (att->HasUnit()) {
            attributes->push_back(std::make_pair("unit", att->StrToStr(att->GetUnit())));
        }
    }
    if (element->HasAttClass(ATT_MEDIABOUNDS)) {
        const AttMediaBounds *att = dynamic_cast<const AttMediaBounds *>(element);
        assert(att);
        if (att->HasBegin()) {
            attributes->push_back(std::make_pair("begin", att->StrToStr(att->GetBegin())));
        }
        if (att->HasEnd()) {
            attributes->push_back(std::make_pair("end", att->StrToStr(att->GetEnd())));
        }
        if (att->HasBetype()) {
            attributes->push_back(std::make_pair("betype", att->BetypeToStr(att->GetBetype())));
        }
    }
    if (element->HasAttClass(ATT_MEDIUM)) {
        const AttMedium *att = dynamic_cast<const AttMedium *>(element);
        assert(att);
        if (att->HasMedium()) {
            attributes->push_back(std::make_pair("medium", att->StrToStr(att->GetMedium())));
        }
    }
    if (element->HasAttClass(ATT_MEIVERSION)) {
        const AttMeiVersion *att = dynamic_cast<const AttMeiVersion *>(element);
        assert(att);
        if (att->HasMeiversion()) {
            attributes->push_back(std::make_pair("meiversion", att->MeiVersionMeiversionToStr(att->GetMeiversion())));
        }
    }
    if (element->HasAttClass(ATT_MENSURLOG)) {
        const AttMensurLog *att = dynamic_cast<const AttMensurLog *>(element);
        assert(att);
        if (att->HasDot()) {
            attributes->push_back(std::make_pair("dot", att->BooleanToStr(att->GetDot())));
        }
        if (att->HasSign()) {
            attributes->push_back(std::make_pair("sign", att->MensurationsignToStr(att->GetSign())));
        }
    }
    if (element->HasAttClass(ATT_METADATAPOINTING)) {
        const AttMetadataPointing *att = dynamic_cast<const AttMetadataPointing *>(element);
        assert(att);
        if (att->HasDecls()) {
            attributes->push_back(std::make_pair("decls", att->StrToStr(att->GetDecls())));
        }
    }
    if (element->HasAttClass(ATT_METERCONFORMANCE)) {
        const AttMeterConformance *att = dynamic_cast<const AttMeterConformance *>(element);
        assert(att);
        if (att->HasMetcon()) {
            attributes->push_back(std::make_pair("metcon", att->MeterConformanceMetconToStr(att->GetMetcon())));
        }
    }
    if (element->HasAttClass(ATT_METERCONFORMANCEBAR)) {
        const AttMeterConformanceBar *att = dynamic_cast<const AttMeterConformanceBar *>(element);
        assert(att);
        if (att->HasMetcon()) {
            attributes->push_back(std::make_pair("metcon", att->BooleanToStr(att->GetMetcon())));
        }
        if (att->HasControl()) {
            attributes->push_back(std::make_pair("control", att->BooleanToStr(att->GetControl())));
        }
    }
    if (element->HasAttClass(ATT_METERSIGLOG)) {
        const AttMeterSigLog *att = dynamic_cast<const AttMeterSigLog *>(element);
        assert(att);
        if (att->HasCount()) {
            attributes->push_back(std::make_pair("count", att->IntToStr(att->GetCount())));
        }
        if (att->HasSym()) {
            attributes->push_back(std::make_pair("sym", att->MetersignToStr(att->GetSym())));
        }
        if (att->HasUnit()) {
            attributes->push_back(std::make_pair("unit", att->IntToStr(att->GetUnit())));
        }
    }
    if (element->HasAttClass(ATT_METERSIGDEFAULTLOG)) {
        const AttMeterSigDefaultLog *att = dynamic_cast<const AttMeterSigDefaultLog *>(element);
        assert(att);
        if (att->HasMeterCount()) {
            attributes->push_back(std::make_pair("meter.count", att->IntToStr(att->GetMeterCount())));
        }
        if (att->HasMeterUnit()) {
            attributes->push_back(std::make_pair("meter.unit", att->IntToStr(att->GetMeterUnit())));
        }
        if (att->HasMeterSym()) {
            attributes->push_back(std::make_pair("meter.sym", att->MetersignToStr(att->GetMeterSym())));
        }
    }
    if (element->HasAttClass(ATT_MMTEMPO)) {
        const AttMmTempo *att = dynamic_cast<const AttMmTempo *>(element);
        assert(att);
        if (att->HasMm()) {
            attributes->push_back(std::make_pair("mm", att->IntToStr(att->GetMm())));
        }
        if (att->HasMmUnit()) {
            attributes->push_back(std::make_pair("mm.unit", att->DurationToStr(att->GetMmUnit())));
        }
        if (att->HasMmDots()) {
            attributes->push_back(std::make_pair("mm.dots", att->IntToStr(att->GetMmDots())));
        }
    }
    if (element->HasAttClass(ATT_MULTINUMMEASURES)) {
        const AttMultinumMeasures *att = dynamic_cast<const AttMultinumMeasures *>(element);
        assert(att);
        if (att->HasMultiNumber()) {
            attributes->push_back(std::make_pair("multi.number", att->BooleanToStr(att->GetMultiNumber())));
        }
    }
    if (element->HasAttClass(ATT_NINTEGER)) {
        const AttNInteger *att = dynamic_cast<const AttNInteger *>(element);
        assert(att);
        if (att->HasN()) {
            attributes->push_back(std::make_pair("n", att->IntToStr(att->GetN())));
        }
    }
    if (element->HasAttClass(ATT_NNUMBERLIKE)) {
        const AttNNumberLike *att = dynamic_cast<const AttNNumberLike *>(element);
        assert(att);
        if (att->HasN()) {
            attributes->push_back(std::make_pair("n", att->StrToStr(att->GetN())));
        }
    }
    if (element->HasAttClass(ATT_NAME)) {
        const AttName *att = dynamic_cast<const AttName *>(element);
        assert(att);
        if (att->HasNymref()) {
            attributes->push_back(std::make_pair("nymref", att->StrToStr(att->GetNymref())));
        }
        if (att->HasRole()) {
            attributes->push_back(std::make_pair("role", att->StrToStr(att->GetRole())));
        }
    }
    if (element->HasAttClass(ATT_NOTATIONSTYLE)) {
        const AttNotationStyle *att = dynamic_cast<const AttNotationStyle *>(element);
        assert(att);
        if (att->HasMusicName()) {
            attributes->push_back(std::make_pair("music.name", att->StrToStr(att->GetMusicName())));
        }
        if (att->HasMusicSize()) {
            attributes->push_back(std::make_pair("music.size", att->FontsizeToStr(att->GetMusicSize())));
        }
    }
    if (element->HasAttClass(ATT_NOTEHEADS)) {
        const AttNoteHeads *att = dynamic_cast<const AttNoteHeads *>(element);
        assert(att);
        if (att->HasHeadAltsym()) {
            attributes->push_back(std::make_pair("head.altsym", att->StrToStr(att->GetHeadAltsym())));
        }
        if (att->HasHeadAuth()) {
            attributes->push_back(std::make_pair("head.auth", att->StrToStr(att->GetHeadAuth())));
        }
        if (att->HasHeadColor()) {
            attributes->push_back(std::make_pair("head.color", att->StrToStr(att->GetHeadColor())));
        }
        if (att->HasHeadFill()) {
            attributes->push_back(std::make_pair("head.fill", att->FillToStr(att->GetHeadFill())));
        }
        if (att->HasHeadFillcolor()) {
            attributes->push_back(std::make_pair("head.fillcolor", att->StrToStr(att->GetHeadFillcolor())));
        }
        if (att->HasHeadMod()) {
            attributes->push_back(std::make_pair("head.mod", att->NoteheadmodifierToStr(att->GetHeadMod())));
        }
        if (att->HasHeadRotation()) {
            attributes->push_back(std::make_pair("head.rotation", att->RotationToStr(att->GetHeadRotation())));
        }
        if (att->HasHeadShape()) {
            attributes->push_back(std::make_pair("head.shape", att->HeadshapeToStr(att->GetHeadShape())));
        }
        if (att->HasHeadVisible()) {
            attributes->push_back(std::make_pair("head.visible", att->BooleanToStr(att->GetHeadVisible())));
        }
    }
    if (element->HasAttClass(ATT_OCTAVE)) {
        const AttOctave *att = dynamic_cast<const AttOctave *>(element);
        assert(att);
        if (att->HasOct()) {
            attributes->push_back(std::make_pair("oct", att->OctaveToStr(att->GetOct())));
        }
    }
    if (element->HasAttClass(ATT_OCTAVEDEFAULT)) {
        const AttOctaveDefault *att = dynamic_cast<const AttOctaveDefault *>(element);
        assert(att);
        if (att->HasOctDefault()) {
            attributes->push_back(std::make_pair("oct.default", att->OctaveToStr(att->GetOctDefault())));
        }
    }
    if (element->HasAttClass(ATT_OCTAVEDISPLACEMENT)) {
        const AttOctaveDisplacement *att = dynamic_cast<const AttOctaveDisplacement *>(element);
        assert(att);
        if (att->HasDis()) {
            attributes->push_back(std::make_pair("dis", att->OctaveDisToStr(att->GetDis())));
        }
        if (att->HasDisPlace()) {
            attributes->push_back(std::make_pair("dis.place", att->StaffrelBasicToStr(att->GetDisPlace())));
        }
    }
    if (element->HasAttClass(ATT_ONELINESTAFF)) {
        const AttOneLineStaff *att = dynamic_cast<const AttOneLineStaff *>(element);
        assert(att);
        if (att->HasOntheline()) {
            attributes->push_back(std::make_pair("ontheline", att->BooleanToStr(att->GetOntheline())));
        }
    }
    if (element->HasAttClass(ATT_OPTIMIZATION)) {
        const AttOptimization *att = dynamic_cast<const AttOptimization *>(element);
        assert(att);
        if (att->HasOptimize()) {
            attributes->push_back(std::make_pair("optimize", att->BooleanToStr(att->GetOptimize())));
        }
    }
    if (element->HasAttClass(ATT_ORIGINLAYERIDENT)) {
        const AttOriginLayerIdent *att = dynamic_cast<const AttOriginLayerIdent *>(element);
        assert(att);
        if (att->HasOriginLayer()) {
            attributes->push_back(std::make_pair("origin.layer", att->StrToStr(att->GetOriginLayer())));
        }
    }
    if (element->HasAttClass(ATT_ORIGINSTAFFIDENT)) {
        const AttOriginStaffIdent *att = dynamic_cast<const AttOriginStaffIdent *>(element);
        assert(att);
        if (att->HasOriginStaff()) {
            attributes->push_back(std::make_pair("origin.staff", att->StrToStr(att->GetOriginStaff())));
        }
    }
    if (element->HasAttClass(ATT_ORIGINSTARTENDID)) {
        const AttOriginStartEndId *att = dynamic_cast<const AttOriginStartEndId *>(element);
        assert(att);
        if (att->HasOriginStartid()) {
            attributes->push_back(std::make_pair("origin.startid", att->StrToStr(att->GetOriginStartid())));
        }
        if (att->HasOriginEndid()) {
            attributes->push_back(std::make_pair("origin.endid", att->StrToStr(att->GetOriginEndid())));
        }
    }
    if (element->HasAttClass(ATT_ORIGINTIMESTAMPLOGICAL)) {
        const AttOriginTimestampLogical *att = dynamic_cast<const AttOriginTimestampLogical *>(element);
        assert(att);
        if (att->HasOriginTstamp()) {
            attributes->push_back(std::make_pair("origin.tstamp", att->MeasurebeatToStr(att->GetOriginTstamp())));
        }
        if (att->HasOriginTstamp2()) {
            attributes->push_back(std::make_pair("origin.tstamp2", att->MeasurebeatToStr(att->GetOriginTstamp2())));
        }
    }
    if (element->HasAttClass(ATT_PAGES)) {
        const AttPages *att = dynamic_cast<const AttPages *>(element);
        assert(att);
        if (att->HasPageHeight()) {
            attributes->push_back(std::make_pair("page.height", att->MeasurementabsToStr(att->GetPageHeight())));
        }
        if (att->HasPageWidth()) {
            attributes->push_back(std::make_pair("page.width", att->MeasurementabsToStr(att->GetPageWidth())));
        }
        if (att->HasPageTopmar()) {
            attributes->push_back(std::make_pair("page.topmar", att->MeasurementabsToStr(att->GetPageTopmar())));
        }
        if (att->HasPageBotmar()) {
            attributes->push_back(std::make_pair("page.botmar", att->MeasurementabsToStr(att->GetPageBotmar())));
        }
        if (att->HasPageLeftmar()) {
            attributes->push_back(std::make_pair("page.leftmar", att->MeasurementabsToStr(att->GetPageLeftmar())));
        }
        if (att->HasPageRightmar()) {
            attributes->push_back(std::make_pair("page.rightmar", att->MeasurementabsToStr(att->GetPageRightmar())));
        }
        if (att->HasPagePanels()) {
            attributes->push_back(std::make_pair("page.panels", att->StrToStr(att->GetPagePanels())));
        }
        if (att->HasPageScale()) {
            attributes->push_back(std::make_pair("page.scale", att->StrToStr(att->GetPageScale())));
        }
    }
    if (element->HasAttClass(ATT_PARTIDENT)) {
        const AttPartIdent *att = dynamic_cast<const AttPartIdent *>(element);
        assert(att);
        if (att->HasPart()) {
            attributes->push_back(std::make_pair("part", att->StrToStr(att->GetPart())));
        }
        if (att->HasPartstaff()) {
            attributes->push_back(std::make_pair("partstaff", att->StrToStr(att->GetPartstaff())));
        }
    }
    if (element->HasAttClass(ATT_PITCH)) {
        const AttPitch *att = dynamic_cast<const AttPitch *>(element);
        assert(att);
        if (att->HasPname()) {
            attributes->push_back(std::make_pair("pname", att->PitchnameToStr(att->GetPname())));
        }
    }
    if (element->HasAttClass(ATT_PLACEMENT)) {
        const AttPlacement *att = dynamic_cast<const AttPlacement *>(element);
        assert(att);
        if (att->HasPlace()) {
            attributes->push_back(std::make_pair("place", att->StaffrelToStr(att->GetPlace())));
        }
    }
    if (element->HasAttClass(ATT_PLIST)) {
        const AttPlist *att = dynamic_cast<const AttPlist *>(element);
        assert(att);
        if (att->HasPlist()) {
            attributes->push_back(std::make_pair("plist", att->XsdAnyURIListToStr(att->GetPlist())));
        }
    }
    if (element->HasAttClass(ATT_POINTING)) {
        const AttPointing *att = dynamic_cast<const AttPointing *>(element);
        assert(att);
        if (att->HasActuate()) {
            attributes->push_back(std::make_pair("xlink:actuate", att->StrToStr(att->GetActuate())));
        }
        if (att->HasRole()) {
            attributes->push_back(std::make_pair("xlink:role", att->StrToStr(att->GetRole())));
        }
        if (att->HasShow()) {
            attributes->push_back(std::make_pair("xlink:show", att->StrToStr(att->GetShow())));
        }
        if (att->HasTarget()) {
            attributes->push_back(std::make_pair("xlink:target", att->StrToStr(att->GetTarget())));
        }
        if (att->HasTargettype()) {
            attributes->push_back(std::make_pair("xlink:targettype", att->StrToStr(att->GetTargettype())));
        }
    }
    if (element->HasAttClass(ATT_QUANTITY)) {
        const AttQuantity *att = dynamic_cast<const AttQuantity *>(element);
        assert(att);
        if (att->HasQuantity()) {
            attributes->push_back(std::make_pair("quantity", att->DblToStr(att->GetQuantity())));
        }
    }
    if (element->HasAttClass(ATT_RANGING)) {
        const AttRanging *att = dynamic_cast<const AttRanging *>(element);
        assert(att);
        if (att->HasAtleast()) {
            attributes->push_back(std::make_pair("atleast", att->DblToStr(att->GetAtleast())));
        }
        if (att->HasAtmost()) {
            attributes->push_back(std::make_pair("atmost", att->DblToStr(att->GetAtmost())));
        }
        if (att->HasMin()) {
            attributes->push_back(std::make_pair("min", att->DblToStr(att->GetMin())));
        }
        if (att->HasMax()) {
            attributes->push_back(std::make_pair("max", att->DblToStr(att->GetMax())));
        }
        if (att->HasConfidence()) {
            attributes->push_back(std::make_pair("confidence", att->DblToStr(att->GetConfidence())));
        }
    }
    if (element->HasAttClass(ATT_RESPONSIBILITY)) {
        const AttResponsibility *att = dynamic_cast<const AttResponsibility *>(element);
        assert(att);
        if (att->HasResp()) {
            attributes->push_back(std::make_pair("resp", att->StrToStr(att->GetResp())));
        }
    }
    if (element->HasAttClass(ATT_SCALABLE)) {
        const AttScalable *att = dynamic_cast<const AttScalable *>(element);
        assert(att);
        if (att->HasScale()) {
            attributes->push_back(std::make_pair("scale", att->PercentToStr(att->GetScale())));
        }
    }
    if (element->HasAttClass(ATT_SEQUENCE)) {
        const AttSequence *att = dynamic_cast<const AttSequence *>(element);
        assert(att);
        if (att->HasSeq()) {
            attributes->push_back(std::make_pair("seq", att->IntToStr(att->GetSeq())));
        }
    }
    if (element->HasAttClass(ATT_SLASHCOUNT)) {
        const AttSlashCount *att = dynamic_cast<const AttSlashCount *>(element);
        assert(att);
        if (att->HasSlash()) {
            attributes->push_back(std::make_pair("slash", att->IntToStr(att->GetSlash())));
        }
    }
    if (element->HasAttClass(ATT_SLURPRESENT)) {
        const AttSlurPresent *att = dynamic_cast<const AttSlurPresent *>(element);
        assert(att);
        if (att->HasSlur()) {
            attributes->push_back(std::make_pair("slur", att->StrToStr(att->GetSlur())));
        }
    }
    if (element->HasAttClass(ATT_SOURCE)) {
        const AttSource *att = dynamic_cast<const AttSource *>(element);
        assert(att);
        if (att->HasSource()) {
            attributes->push_back(std::make_pair("source", att->StrToStr(att->GetSource())));
        }
    }
    if (element->HasAttClass(ATT_SPACING)) {
        const AttSpacing *att = dynamic_cast<const AttSpacing *>(element);
        assert(att);
        if (att->HasSpacingPackexp()) {
            attributes->push_back(std::make_pair("spacing.packexp", att->DblToStr(att->GetSpacingPackexp())));
        }
        if (att->HasSpacingPackfact()) {
            attributes->push_back(std::make_pair("spacing.packfact", att->DblToStr(att->GetSpacingPackfact())));
        }
        if (att->HasSpacingStaff()) {
            attributes->push_back(std::make_pair("spacing.staff", att->MeasurementrelToStr(att->GetSpacingStaff())));
        }
        if (att->HasSpacingSystem()) {
            attributes->push_back(std::make_pair("spacing.system", att->MeasurementrelToStr(att->GetSpacingSystem())));
        }
    }
    if (element->HasAttClass(ATT_STAFFLOG)) {
        const AttStaffLog *att = dynamic_cast<const AttStaffLog *>(element);
        assert(att);
        if (att->HasDef()) {
            attributes->push_back(std::make_pair("def", att->StrToStr(att->GetDef())));
        }
    }
    if (element->HasAttClass(ATT_STAFFDEFLOG)) {
        const AttStaffDefLog *att = dynamic_cast<const AttStaffDefLog *>(element);
        assert(att);
        if (att->HasLines()) {
            attributes->push_back(std::make_pair("lines", att->IntToStr(att->GetLines())));
        }
    }
    if (element->HasAttClass(ATT_STAFFGROUPINGSYM)) {
        const AttStaffGroupingSym *att = dynamic_cast<const AttStaffGroupingSym *>(element);
        assert(att);
        if (att->HasSymbol()) {
            attributes->push_back(std::make_pair("symbol", att->StaffGroupingSymSymbolToStr(att->GetSymbol())));
        }
    }
    if (element->HasAttClass(ATT_STAFFIDENT)) {
        const AttStaffIdent *att = dynamic_cast<const AttStaffIdent *>(element);
        assert(att);
        if (att->HasStaff()) {
            attributes->push_back(std::make_pair("staff", att->XsdPositiveIntegerListToStr(att->GetStaff())));
        }
    }
    if (element->HasAttClass(ATT_STAFFITEMS)) {
        const AttStaffItems *att = dynamic_cast<const AttStaffItems *>(element);
        assert(att);
        if (att->HasAboveorder()) {
            attributes->push_back(std::make_pair("aboveorder", att->StaffitemToStr(att->GetAboveorder())));
        }
        if (att->HasBeloworder()) {
            attributes->push_back(std::make_pair("beloworder", att->StaffitemToStr(att->GetBeloworder())));
        }
        if (att->HasBetweenorder()) {
            attributes->push_back(std::make_pair("betweenorder", att->StaffitemToStr(att->GetBetweenorder())));
        }
    }
    if (element->HasAttClass(ATT_STAFFLOC)) {
        const AttStaffLoc *att = dynamic_cast<const AttStaffLoc *>(element);
        assert(att);
        if (att->HasLoc()) {
            attributes->push_back(std::make_pair("loc", att->IntToStr(att->GetLoc())));
        }
    }
    if (element->HasAttClass(ATT_STAFFLOCPITCHED)) {
        const AttStaffLocPitched *att = dynamic_cast<const AttStaffLocPitched *>(element);
        assert(att);
        if (att->HasPloc()) {
            attributes->push_back(std::make_pair("ploc", att->PitchnameToStr(att->GetPloc())));
        }
        if (att->HasOloc()) {
            attributes->push_back(std::make_pair("oloc", att->OctaveToStr(att->GetOloc())));
        }
    }
    if (element->HasAttClass(ATT_STARTENDID)) {
        const AttStartEndId *att = dynamic_cast<const AttStartEndId *>(element);
        assert(att);
        if (att->HasEndid()) {
            attributes->push_back(std::make_pair("endid", att->StrToStr(att->GetEndid())));
        }
    }
    if (element->HasAttClass(ATT_STARTID)) {
        const AttStartId *att = dynamic_cast<const AttStartId *>(element);
        assert(att);
        if (att->HasStartid()) {
            attributes->push_back(std::make_pair("startid", att->StrToStr(att->GetStartid())));
        }
    }
    if (element->HasAttClass(ATT_STEMS)) {
        const AttStems *att = dynamic_cast<const AttStems *>(element);
        assert(att);
        if (att->HasStemDir()) {
            attributes->push_back(std::make_pair("stem.dir", att->StemdirectionToStr(att->GetStemDir())));
        }
        if (att->HasStemLen()) {
            attributes->push_back(std::make_pair("stem.len", att->DblToStr(att->GetStemLen())));
        }
        if (att->HasStemMod()) {
            attributes->push_back(std::make_pair("stem.mod", att->StemmodifierToStr(att->GetStemMod())));
        }
        if (att->HasStemPos()) {
            attributes->push_back(std::make_pair("stem.pos", att->StempositionToStr(att->GetStemPos())));
        }
        if (att->HasStemVisible()) {
            attributes->push_back(std::make_pair("stem.visible", att->BooleanToStr(att->GetStemVisible())));
        }
        if (att->HasStemX()) {
            attributes->push_back(std::make_pair("stem.x", att->DblToStr(att->GetStemX())));
        }
        if (att->HasStemY()) {
            attributes->push_back(std::make_pair("stem.y", att->DblToStr(att->GetStemY())));
        }
    }
    if (element->HasAttClass(ATT_SYLLOG)) {
        const AttSylLog *att = dynamic_cast<const AttSylLog *>(element);
        assert(att);
        if (att->HasCon()) {
            attributes->push_back(std::make_pair("con", att->SylLogConToStr(att->GetCon())));
        }
        if (att->HasWordpos()) {
            attributes->push_back(std::make_pair("wordpos", att->SylLogWordposToStr(att->GetWordpos())));
        }
    }
    if (element->HasAttClass(ATT_SYLTEXT)) {
        const AttSylText *att = dynamic_cast<const AttSylText *>(element);
        assert(att);
        if (att->HasSyl()) {
            attributes->push_back(std::make_pair("syl", att->StrToStr(att->GetSyl())));
        }
    }
    if (element->HasAttClass(ATT_SYSTEMS)) {
        const AttSystems *att = dynamic_cast<const AttSystems *>(element);
        assert(att);
        if (att->HasSystemLeftline()) {
            attributes->push_back(std::make_pair("system.leftline", att->BooleanToStr(att->GetSystemLeftline())));
        }
        if (att->HasSystemLeftmar()) {
            attributes->push_back(std::make_pair("system.leftmar", att->MeasurementabsToStr(att->GetSystemLeftmar())));
        }
        if (att->HasSystemRightmar()) {
            attributes->push_back(std::make_pair("system.rightmar", att->MeasurementabsToStr(att->GetSystemRightmar())));
        }
        if (att->HasSystemTopmar()) {
            attributes->push_back(std::make_pair("system.topmar", att->MeasurementabsToStr(att->GetSystemTopmar())));
        }
    }
    if (element->HasAttClass(ATT_TARGETEVAL)) {
        const AttTargetEval *att = dynamic_cast<const AttTargetEval *>(element);
        assert(att);
        if (att->HasEvaluate()) {
            attributes->push_back(std::make_pair("evaluate", att->TargetEvalEvaluateToStr(att->GetEvaluate())));
        }
    }
    if (element->HasAttClass(ATT_TEMPOLOG)) {
        const AttTempoLog *att = dynamic_cast<const AttTempoLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back(std::make_pair("func", att->TempoLogFuncToStr(att->GetFunc())));
        }
    }
    if (element->HasAttClass(ATT_TEXTRENDITION)) {
        const AttTextRendition *att = dynamic_cast<const AttTextRendition *>(element);
        assert(att);
        if (att->HasAltrend()) {
            attributes->push_back(std::make_pair("altrend", att->StrToStr(att->GetAltrend())));
        }
        if (att->HasRend()) {
            attributes->push_back(std::make_pair("rend", att->TextrenditionToStr(att->GetRend())));
        }
    }
    if (element->HasAttClass(ATT_TEXTSTYLE)) {
        const AttTextStyle *att = dynamic_cast<const AttTextStyle *>(element);
        assert(att);
        if (att->HasTextFam()) {
            attributes->push_back(std::make_pair("text.fam", att->StrToStr(att->GetTextFam())));
        }
        if (att->HasTextName()) {
            attributes->push_back(std::make_pair("text.name", att->StrToStr(att->GetTextName())));
        }
        if (att->HasTextSize()) {
            attributes->push_back(std::make_pair("text.size", att->FontsizeToStr(att->GetTextSize())));
        }
        if (att->HasTextStyle()) {
            attributes->push_back(std::make_pair("text.style", att->FontstyleToStr(att->GetTextStyle())));
        }
        if (att->HasTextWeight()) {
            attributes->push_back(std::make_pair("text.weight", att->FontweightToStr(att->GetTextWeight())));
        }
    }
    if (element->HasAttClass(ATT_TIEPRESENT)) {
        const AttTiePresent *att = dynamic_cast<const AttTiePresent *>(element);
        assert(att);
        if (att->HasTie()) {
            attributes->push_back(std::make_pair("tie", att->TieToStr(att->GetTie())));
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMPLOGICAL)) {
        const AttTimestampLogical *att = dynamic_cast<const AttTimestampLogical *>(element);
        assert(att);
        if (att->HasTstamp()) {
            attributes->push_back(std::make_pair("tstamp", att->DblToStr(att->GetTstamp())));
        }
    }
    if (element->HasAttClass(ATT_TIMESTAMP2LOGICAL)) {
        const AttTimestamp2Logical *att = dynamic_cast<const AttTimestamp2Logical *>(element);
        assert(att);
        if (att->HasTstamp2()) {
            attributes->push_back(std::make_pair("tstamp2", att->MeasurebeatToStr(att->GetTstamp2())));
        }
    }
    if (element->HasAttClass(ATT_TRANSPOSITION)) {
        const AttTransposition *att = dynamic_cast<const AttTransposition *>(element);
        assert(att);
        if (att->HasTransDiat()) {
            attributes->push_back(std::make_pair("trans.diat", att->DblToStr(att->GetTransDiat())));
        }
        if (att->HasTransSemi()) {
            attributes->push_back(std::make_pair("trans.semi", att->DblToStr(att->GetTransSemi())));
        }
    }
    if (element->HasAttClass(ATT_TUPLETPRESENT)) {
        const AttTupletPresent *att = dynamic_cast<const AttTupletPresent *>(element);
        assert(att);
        if (att->HasTuplet()) {
            attributes->push_back(std::make_pair("tuplet", att->StrToStr(att->GetTuplet())));
        }
    }
    if (element->HasAttClass(ATT_TYPED)) {
        const AttTyped *att = dynamic_cast<const AttTyped *>(element);
        assert(att);
        if (att->HasType()) {
            attributes->push_back(std::make_pair("type", att->StrToStr(att->GetType())));
        }
    }
    if (element->HasAttClass(ATT_TYPOGRAPHY)) {
        const AttTypography *att = dynamic_cast<const AttTypography *>(element);
        assert(att);
        if (att->HasFontfam()) {
            attributes->push_back(std::make_pair("fontfam", att->StrToStr(att->GetFontfam())));
        }
        if (att->HasFontname()) {
            attributes->push_back(std::make_pair("fontname", att->StrToStr(att->GetFontname())));
        }
        if (att->HasFontsize()) {
            attributes->push_back(std::make_pair("fontsize", att->FontsizeToStr(att->GetFontsize())));
        }
        if (att->HasFontstyle()) {
            attributes->push_back(std::make_pair("fontstyle", att->FontstyleToStr(att->GetFontstyle())));
        }
        if (att->HasFontweight()) {
            attributes->push_back(std::make_pair("fontweight", att->FontweightToStr(att->GetFontweight())));
        }
    }
    if (element->HasAttClass(ATT_VERTICALALIGN)) {
        const AttVerticalAlign *att = dynamic_cast<const AttVerticalAlign *>(element);
        assert(att);
        if (att->HasValign()) {
            attributes->push_back(std::make_pair("valign", att->VerticalalignmentToStr(att->GetValign())));
        }
    }
    if (element->HasAttClass(ATT_VERTICALGROUP)) {
        const AttVerticalGroup *att = dynamic_cast<const AttVerticalGroup *>(element);
        assert(att);
        if (att->HasVgrp()) {
            attributes->push_back(std::make_pair("vgrp", att->IntToStr(att->GetVgrp())));
        }
    }
    if (element->HasAttClass(ATT_VISIBILITY)) {
        const AttVisibility *att = dynamic_cast<const AttVisibility *>(element);
        assert(att);
        if (att->HasVisible()) {
            attributes->push_back(std::make_pair("visible", att->BooleanToStr(att->GetVisible())));
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSETHO)) {
        const AttVisualOffsetHo *att = dynamic_cast<const AttVisualOffsetHo *>(element);
        assert(att);
        if (att->HasHo()) {
            attributes->push_back(std::make_pair("ho", att->MeasurementrelToStr(att->GetHo())));
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSETTO)) {
        const AttVisualOffsetTo *att = dynamic_cast<const AttVisualOffsetTo *>(element);
        assert(att);
        if (att->HasTo()) {
            attributes->push_back(std::make_pair("to", att->DblToStr(att->GetTo())));
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSETVO)) {
        const AttVisualOffsetVo *att = dynamic_cast<const AttVisualOffsetVo *>(element);
        assert(att);
        if (att->HasVo()) {
            attributes->push_back(std::make_pair("vo", att->MeasurementrelToStr(att->GetVo())));
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSET2HO)) {
        const AttVisualOffset2Ho *att = dynamic_cast<const AttVisualOffset2Ho *>(element);
        assert(att);
        if (att->HasStartho()) {
            attributes->push_back(std::make_pair("startho", att->MeasurementrelToStr(att->GetStartho())));
        }
        if (att->HasEndho()) {
            attributes->push_back(std::make_pair("endho", att->MeasurementrelToStr(att->GetEndho())));
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSET2TO)) {
        const AttVisualOffset2To *att = dynamic_cast<const AttVisualOffset2To *>(element);
        assert(att);
        if (att->HasStartto()) {
            attributes->push_back(std::make_pair("startto", att->DblToStr(att->GetStartto())));
        }
        if (att->HasEndto()) {
            attributes->push_back(std::make_pair("endto", att->DblToStr(att->GetEndto())));
        }
    }
    if (element->HasAttClass(ATT_VISUALOFFSET2VO)) {
        const AttVisualOffset2Vo *att = dynamic_cast<const AttVisualOffset2Vo *>(element);
        assert(att);
        if (att->HasStartvo()) {
            attributes->push_back(std::make_pair("startvo", att->MeasurementrelToStr(att->GetStartvo())));
        }
        if (att->HasEndvo()) {
            attributes->push_back(std::make_pair("endvo", att->MeasurementrelToStr(att->GetEndvo())));
        }
    }
    if (element->HasAttClass(ATT_VOLTAGROUPINGSYM)) {
        const AttVoltaGroupingSym *att = dynamic_cast<const AttVoltaGroupingSym *>(element);
        assert(att);
        if (att->HasVoltasym()) {
            attributes->push_back(std::make_pair("voltasym", att->VoltaGroupingSymVoltasymToStr(att->GetVoltasym())));
        }
    }
    if (element->HasAttClass(ATT_WHITESPACE)) {
        const AttWhitespace *att = dynamic_cast<const AttWhitespace *>(element);
        assert(att);
        if (att->HasSpace()) {
            attributes->push_back(std::make_pair("xml:space", att->StrToStr(att->GetSpace())));
        }
    }
    if (element->HasAttClass(ATT_WIDTH)) {
        const AttWidth *att = dynamic_cast<const AttWidth *>(element);
        assert(att);
        if (att->HasWidth()) {
            attributes->push_back(std::make_pair("width", att->MeasurementabsToStr(att->GetWidth())));
        }
    }
    if (element->HasAttClass(ATT_XY)) {
        const AttXy *att = dynamic_cast<const AttXy *>(element);
        assert(att);
        if (att->HasX()) {
            attributes->push_back(std::make_pair("x", att->DblToStr(att->GetX())));
        }
        if (att->HasY()) {
            attributes->push_back(std::make_pair("y", att->DblToStr(att->GetY())));
        }
    }
    if (element->HasAttClass(ATT_XY2)) {
        const AttXy2 *att = dynamic_cast<const AttXy2 *>(element);
        assert(att);
        if (att->HasX2()) {
            attributes->push_back(std::make_pair("x2", att->DblToStr(att->GetX2())));
        }
        if (att->HasY2()) {
            attributes->push_back(std::make_pair("y2", att->DblToStr(att->GetY2())));
        }
    }
}

} // vrv namespace
