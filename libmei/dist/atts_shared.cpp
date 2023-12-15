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

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttAccidLog
//----------------------------------------------------------------------------

AttAccidLog::AttAccidLog() : Att()
{
    ResetAccidLog();
}

void AttAccidLog::ResetAccidLog()
{
    m_func = accidLog_FUNC_NONE;
}

bool AttAccidLog::ReadAccidLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToAccidLogFunc(element.attribute("func").value()));
        if (removeAttr) element.remove_attribute("func");
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

//----------------------------------------------------------------------------
// AttAccidental
//----------------------------------------------------------------------------

AttAccidental::AttAccidental() : Att()
{
    ResetAccidental();
}

void AttAccidental::ResetAccidental()
{
    m_accid = ACCIDENTAL_WRITTEN_NONE;
}

bool AttAccidental::ReadAccidental(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("accid")) {
        this->SetAccid(StrToAccidentalWritten(element.attribute("accid").value()));
        if (removeAttr) element.remove_attribute("accid");
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

//----------------------------------------------------------------------------
// AttArticulation
//----------------------------------------------------------------------------

AttArticulation::AttArticulation() : Att()
{
    ResetArticulation();
}

void AttArticulation::ResetArticulation()
{
    m_artic = std::vector<data_ARTICULATION>();
}

bool AttArticulation::ReadArticulation(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("artic")) {
        this->SetArtic(StrToArticulationList(element.attribute("artic").value()));
        if (removeAttr) element.remove_attribute("artic");
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

//----------------------------------------------------------------------------
// AttAttaccaLog
//----------------------------------------------------------------------------

AttAttaccaLog::AttAttaccaLog() : Att()
{
    ResetAttaccaLog();
}

void AttAttaccaLog::ResetAttaccaLog()
{
    m_target = "";
}

bool AttAttaccaLog::ReadAttaccaLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("target")) {
        this->SetTarget(StrToStr(element.attribute("target").value()));
        if (removeAttr) element.remove_attribute("target");
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

//----------------------------------------------------------------------------
// AttAudience
//----------------------------------------------------------------------------

AttAudience::AttAudience() : Att()
{
    ResetAudience();
}

void AttAudience::ResetAudience()
{
    m_audience = audience_AUDIENCE_NONE;
}

bool AttAudience::ReadAudience(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("audience")) {
        this->SetAudience(StrToAudienceAudience(element.attribute("audience").value()));
        if (removeAttr) element.remove_attribute("audience");
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

//----------------------------------------------------------------------------
// AttAugmentDots
//----------------------------------------------------------------------------

AttAugmentDots::AttAugmentDots() : Att()
{
    ResetAugmentDots();
}

void AttAugmentDots::ResetAugmentDots()
{
    m_dots = MEI_UNSET;
}

bool AttAugmentDots::ReadAugmentDots(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dots")) {
        this->SetDots(StrToInt(element.attribute("dots").value()));
        if (removeAttr) element.remove_attribute("dots");
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
    return (m_dots != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttAuthorized
//----------------------------------------------------------------------------

AttAuthorized::AttAuthorized() : Att()
{
    ResetAuthorized();
}

void AttAuthorized::ResetAuthorized()
{
    m_auth = "";
    m_authUri = "";
}

bool AttAuthorized::ReadAuthorized(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("auth")) {
        this->SetAuth(StrToStr(element.attribute("auth").value()));
        if (removeAttr) element.remove_attribute("auth");
        hasAttribute = true;
    }
    if (element.attribute("auth.uri")) {
        this->SetAuthUri(StrToStr(element.attribute("auth.uri").value()));
        if (removeAttr) element.remove_attribute("auth.uri");
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

//----------------------------------------------------------------------------
// AttBarLineLog
//----------------------------------------------------------------------------

AttBarLineLog::AttBarLineLog() : Att()
{
    ResetBarLineLog();
}

void AttBarLineLog::ResetBarLineLog()
{
    m_form = BARRENDITION_NONE;
}

bool AttBarLineLog::ReadBarLineLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToBarrendition(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
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
    return (m_form != BARRENDITION_NONE);
}

//----------------------------------------------------------------------------
// AttBarring
//----------------------------------------------------------------------------

AttBarring::AttBarring() : Att()
{
    ResetBarring();
}

void AttBarring::ResetBarring()
{
    m_barLen = 0.0;
    m_barMethod = BARMETHOD_NONE;
    m_barPlace = MEI_UNSET;
}

bool AttBarring::ReadBarring(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("bar.len")) {
        this->SetBarLen(StrToDbl(element.attribute("bar.len").value()));
        if (removeAttr) element.remove_attribute("bar.len");
        hasAttribute = true;
    }
    if (element.attribute("bar.method")) {
        this->SetBarMethod(StrToBarmethod(element.attribute("bar.method").value()));
        if (removeAttr) element.remove_attribute("bar.method");
        hasAttribute = true;
    }
    if (element.attribute("bar.place")) {
        this->SetBarPlace(StrToInt(element.attribute("bar.place").value()));
        if (removeAttr) element.remove_attribute("bar.place");
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
    return (m_barPlace != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttBasic
//----------------------------------------------------------------------------

AttBasic::AttBasic() : Att()
{
    ResetBasic();
}

void AttBasic::ResetBasic()
{
    m_base = "";
}

bool AttBasic::ReadBasic(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("xml:base")) {
        this->SetBase(StrToStr(element.attribute("xml:base").value()));
        if (removeAttr) element.remove_attribute("xml:base");
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

//----------------------------------------------------------------------------
// AttBibl
//----------------------------------------------------------------------------

AttBibl::AttBibl() : Att()
{
    ResetBibl();
}

void AttBibl::ResetBibl()
{
    m_analog = "";
}

bool AttBibl::ReadBibl(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("analog")) {
        this->SetAnalog(StrToStr(element.attribute("analog").value()));
        if (removeAttr) element.remove_attribute("analog");
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

//----------------------------------------------------------------------------
// AttCalendared
//----------------------------------------------------------------------------

AttCalendared::AttCalendared() : Att()
{
    ResetCalendared();
}

void AttCalendared::ResetCalendared()
{
    m_calendar = "";
}

bool AttCalendared::ReadCalendared(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("calendar")) {
        this->SetCalendar(StrToStr(element.attribute("calendar").value()));
        if (removeAttr) element.remove_attribute("calendar");
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

//----------------------------------------------------------------------------
// AttCanonical
//----------------------------------------------------------------------------

AttCanonical::AttCanonical() : Att()
{
    ResetCanonical();
}

void AttCanonical::ResetCanonical()
{
    m_codedval = "";
}

bool AttCanonical::ReadCanonical(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("codedval")) {
        this->SetCodedval(StrToStr(element.attribute("codedval").value()));
        if (removeAttr) element.remove_attribute("codedval");
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

//----------------------------------------------------------------------------
// AttClassed
//----------------------------------------------------------------------------

AttClassed::AttClassed() : Att()
{
    ResetClassed();
}

void AttClassed::ResetClassed()
{
    m_class = "";
}

bool AttClassed::ReadClassed(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("class")) {
        this->SetClass(StrToStr(element.attribute("class").value()));
        if (removeAttr) element.remove_attribute("class");
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

//----------------------------------------------------------------------------
// AttClefLog
//----------------------------------------------------------------------------

AttClefLog::AttClefLog() : Att()
{
    ResetClefLog();
}

void AttClefLog::ResetClefLog()
{
    m_cautionary = BOOLEAN_NONE;
}

bool AttClefLog::ReadClefLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("cautionary")) {
        this->SetCautionary(StrToBoolean(element.attribute("cautionary").value()));
        if (removeAttr) element.remove_attribute("cautionary");
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

//----------------------------------------------------------------------------
// AttClefShape
//----------------------------------------------------------------------------

AttClefShape::AttClefShape() : Att()
{
    ResetClefShape();
}

void AttClefShape::ResetClefShape()
{
    m_shape = CLEFSHAPE_NONE;
}

bool AttClefShape::ReadClefShape(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("shape")) {
        this->SetShape(StrToClefshape(element.attribute("shape").value()));
        if (removeAttr) element.remove_attribute("shape");
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

//----------------------------------------------------------------------------
// AttCleffingLog
//----------------------------------------------------------------------------

AttCleffingLog::AttCleffingLog() : Att()
{
    ResetCleffingLog();
}

void AttCleffingLog::ResetCleffingLog()
{
    m_clefShape = CLEFSHAPE_NONE;
    m_clefLine = 0;
    m_clefDis = OCTAVE_DIS_NONE;
    m_clefDisPlace = STAFFREL_basic_NONE;
}

bool AttCleffingLog::ReadCleffingLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("clef.shape")) {
        this->SetClefShape(StrToClefshape(element.attribute("clef.shape").value()));
        if (removeAttr) element.remove_attribute("clef.shape");
        hasAttribute = true;
    }
    if (element.attribute("clef.line")) {
        this->SetClefLine(StrToInt(element.attribute("clef.line").value()));
        if (removeAttr) element.remove_attribute("clef.line");
        hasAttribute = true;
    }
    if (element.attribute("clef.dis")) {
        this->SetClefDis(StrToOctaveDis(element.attribute("clef.dis").value()));
        if (removeAttr) element.remove_attribute("clef.dis");
        hasAttribute = true;
    }
    if (element.attribute("clef.dis.place")) {
        this->SetClefDisPlace(StrToStaffrelBasic(element.attribute("clef.dis.place").value()));
        if (removeAttr) element.remove_attribute("clef.dis.place");
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

//----------------------------------------------------------------------------
// AttColor
//----------------------------------------------------------------------------

AttColor::AttColor() : Att()
{
    ResetColor();
}

void AttColor::ResetColor()
{
    m_color = "";
}

bool AttColor::ReadColor(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("color")) {
        this->SetColor(StrToStr(element.attribute("color").value()));
        if (removeAttr) element.remove_attribute("color");
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

//----------------------------------------------------------------------------
// AttColoration
//----------------------------------------------------------------------------

AttColoration::AttColoration() : Att()
{
    ResetColoration();
}

void AttColoration::ResetColoration()
{
    m_colored = BOOLEAN_NONE;
}

bool AttColoration::ReadColoration(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("colored")) {
        this->SetColored(StrToBoolean(element.attribute("colored").value()));
        if (removeAttr) element.remove_attribute("colored");
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

//----------------------------------------------------------------------------
// AttCoordX1
//----------------------------------------------------------------------------

AttCoordX1::AttCoordX1() : Att()
{
    ResetCoordX1();
}

void AttCoordX1::ResetCoordX1()
{
    m_coordX1 = 0.0;
}

bool AttCoordX1::ReadCoordX1(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("coord.x1")) {
        this->SetCoordX1(StrToDbl(element.attribute("coord.x1").value()));
        if (removeAttr) element.remove_attribute("coord.x1");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCoordX1::WriteCoordX1(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCoordX1()) {
        element.append_attribute("coord.x1") = DblToStr(this->GetCoordX1()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCoordX1::HasCoordX1() const
{
    return (m_coordX1 != 0.0);
}

//----------------------------------------------------------------------------
// AttCoordX2
//----------------------------------------------------------------------------

AttCoordX2::AttCoordX2() : Att()
{
    ResetCoordX2();
}

void AttCoordX2::ResetCoordX2()
{
    m_coordX2 = 0.0;
}

bool AttCoordX2::ReadCoordX2(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("coord.x2")) {
        this->SetCoordX2(StrToDbl(element.attribute("coord.x2").value()));
        if (removeAttr) element.remove_attribute("coord.x2");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCoordX2::WriteCoordX2(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCoordX2()) {
        element.append_attribute("coord.x2") = DblToStr(this->GetCoordX2()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCoordX2::HasCoordX2() const
{
    return (m_coordX2 != 0.0);
}

//----------------------------------------------------------------------------
// AttCoordY1
//----------------------------------------------------------------------------

AttCoordY1::AttCoordY1() : Att()
{
    ResetCoordY1();
}

void AttCoordY1::ResetCoordY1()
{
    m_coordY1 = 0.0;
}

bool AttCoordY1::ReadCoordY1(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("coord.y1")) {
        this->SetCoordY1(StrToDbl(element.attribute("coord.y1").value()));
        if (removeAttr) element.remove_attribute("coord.y1");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCoordY1::WriteCoordY1(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCoordY1()) {
        element.append_attribute("coord.y1") = DblToStr(this->GetCoordY1()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCoordY1::HasCoordY1() const
{
    return (m_coordY1 != 0.0);
}

//----------------------------------------------------------------------------
// AttCoordinated
//----------------------------------------------------------------------------

AttCoordinated::AttCoordinated() : Att()
{
    ResetCoordinated();
}

void AttCoordinated::ResetCoordinated()
{
    m_lrx = MEI_UNSET;
    m_lry = MEI_UNSET;
    m_rotate = 0.0;
}

bool AttCoordinated::ReadCoordinated(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("lrx")) {
        this->SetLrx(StrToInt(element.attribute("lrx").value()));
        if (removeAttr) element.remove_attribute("lrx");
        hasAttribute = true;
    }
    if (element.attribute("lry")) {
        this->SetLry(StrToInt(element.attribute("lry").value()));
        if (removeAttr) element.remove_attribute("lry");
        hasAttribute = true;
    }
    if (element.attribute("rotate")) {
        this->SetRotate(StrToDbl(element.attribute("rotate").value()));
        if (removeAttr) element.remove_attribute("rotate");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCoordinated::WriteCoordinated(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLrx()) {
        element.append_attribute("lrx") = IntToStr(this->GetLrx()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLry()) {
        element.append_attribute("lry") = IntToStr(this->GetLry()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRotate()) {
        element.append_attribute("rotate") = DblToStr(this->GetRotate()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCoordinated::HasLrx() const
{
    return (m_lrx != MEI_UNSET);
}

bool AttCoordinated::HasLry() const
{
    return (m_lry != MEI_UNSET);
}

bool AttCoordinated::HasRotate() const
{
    return (m_rotate != 0.0);
}

//----------------------------------------------------------------------------
// AttCoordinatedUl
//----------------------------------------------------------------------------

AttCoordinatedUl::AttCoordinatedUl() : Att()
{
    ResetCoordinatedUl();
}

void AttCoordinatedUl::ResetCoordinatedUl()
{
    m_ulx = MEI_UNSET;
    m_uly = MEI_UNSET;
}

bool AttCoordinatedUl::ReadCoordinatedUl(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("ulx")) {
        this->SetUlx(StrToInt(element.attribute("ulx").value()));
        if (removeAttr) element.remove_attribute("ulx");
        hasAttribute = true;
    }
    if (element.attribute("uly")) {
        this->SetUly(StrToInt(element.attribute("uly").value()));
        if (removeAttr) element.remove_attribute("uly");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCoordinatedUl::WriteCoordinatedUl(pugi::xml_node element)
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
    return wroteAttribute;
}

bool AttCoordinatedUl::HasUlx() const
{
    return (m_ulx != MEI_UNSET);
}

bool AttCoordinatedUl::HasUly() const
{
    return (m_uly != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttCue
//----------------------------------------------------------------------------

AttCue::AttCue() : Att()
{
    ResetCue();
}

void AttCue::ResetCue()
{
    m_cue = BOOLEAN_NONE;
}

bool AttCue::ReadCue(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("cue")) {
        this->SetCue(StrToBoolean(element.attribute("cue").value()));
        if (removeAttr) element.remove_attribute("cue");
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

//----------------------------------------------------------------------------
// AttCurvature
//----------------------------------------------------------------------------

AttCurvature::AttCurvature() : Att()
{
    ResetCurvature();
}

void AttCurvature::ResetCurvature()
{
    m_bezier = "";
    m_bulge = std::vector<std::pair<double, double>>();
    m_curvedir = curvature_CURVEDIR_NONE;
}

bool AttCurvature::ReadCurvature(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("bezier")) {
        this->SetBezier(StrToStr(element.attribute("bezier").value()));
        if (removeAttr) element.remove_attribute("bezier");
        hasAttribute = true;
    }
    if (element.attribute("bulge")) {
        this->SetBulge(StrToBulge(element.attribute("bulge").value()));
        if (removeAttr) element.remove_attribute("bulge");
        hasAttribute = true;
    }
    if (element.attribute("curvedir")) {
        this->SetCurvedir(StrToCurvatureCurvedir(element.attribute("curvedir").value()));
        if (removeAttr) element.remove_attribute("curvedir");
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
        element.append_attribute("bulge") = BulgeToStr(this->GetBulge()).c_str();
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
    return (m_bulge != std::vector<std::pair<double, double>>());
}

bool AttCurvature::HasCurvedir() const
{
    return (m_curvedir != curvature_CURVEDIR_NONE);
}

//----------------------------------------------------------------------------
// AttCustosLog
//----------------------------------------------------------------------------

AttCustosLog::AttCustosLog() : Att()
{
    ResetCustosLog();
}

void AttCustosLog::ResetCustosLog()
{
    m_target = "";
}

bool AttCustosLog::ReadCustosLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("target")) {
        this->SetTarget(StrToStr(element.attribute("target").value()));
        if (removeAttr) element.remove_attribute("target");
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

//----------------------------------------------------------------------------
// AttDataPointing
//----------------------------------------------------------------------------

AttDataPointing::AttDataPointing() : Att()
{
    ResetDataPointing();
}

void AttDataPointing::ResetDataPointing()
{
    m_data = "";
}

bool AttDataPointing::ReadDataPointing(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("data")) {
        this->SetData(StrToStr(element.attribute("data").value()));
        if (removeAttr) element.remove_attribute("data");
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

//----------------------------------------------------------------------------
// AttDatable
//----------------------------------------------------------------------------

AttDatable::AttDatable() : Att()
{
    ResetDatable();
}

void AttDatable::ResetDatable()
{
    m_enddate = "";
    m_isodate = "";
    m_notafter = "";
    m_notbefore = "";
    m_startdate = "";
}

bool AttDatable::ReadDatable(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("enddate")) {
        this->SetEnddate(StrToStr(element.attribute("enddate").value()));
        if (removeAttr) element.remove_attribute("enddate");
        hasAttribute = true;
    }
    if (element.attribute("isodate")) {
        this->SetIsodate(StrToStr(element.attribute("isodate").value()));
        if (removeAttr) element.remove_attribute("isodate");
        hasAttribute = true;
    }
    if (element.attribute("notafter")) {
        this->SetNotafter(StrToStr(element.attribute("notafter").value()));
        if (removeAttr) element.remove_attribute("notafter");
        hasAttribute = true;
    }
    if (element.attribute("notbefore")) {
        this->SetNotbefore(StrToStr(element.attribute("notbefore").value()));
        if (removeAttr) element.remove_attribute("notbefore");
        hasAttribute = true;
    }
    if (element.attribute("startdate")) {
        this->SetStartdate(StrToStr(element.attribute("startdate").value()));
        if (removeAttr) element.remove_attribute("startdate");
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

//----------------------------------------------------------------------------
// AttDistances
//----------------------------------------------------------------------------

AttDistances::AttDistances() : Att()
{
    ResetDistances();
}

void AttDistances::ResetDistances()
{
    m_dirDist = data_MEASUREMENTSIGNED();
    m_dynamDist = data_MEASUREMENTSIGNED();
    m_harmDist = data_MEASUREMENTSIGNED();
    m_rehDist = data_MEASUREMENTSIGNED();
    m_tempoDist = data_MEASUREMENTSIGNED();
}

bool AttDistances::ReadDistances(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dir.dist")) {
        this->SetDirDist(StrToMeasurementsigned(element.attribute("dir.dist").value()));
        if (removeAttr) element.remove_attribute("dir.dist");
        hasAttribute = true;
    }
    if (element.attribute("dynam.dist")) {
        this->SetDynamDist(StrToMeasurementsigned(element.attribute("dynam.dist").value()));
        if (removeAttr) element.remove_attribute("dynam.dist");
        hasAttribute = true;
    }
    if (element.attribute("harm.dist")) {
        this->SetHarmDist(StrToMeasurementsigned(element.attribute("harm.dist").value()));
        if (removeAttr) element.remove_attribute("harm.dist");
        hasAttribute = true;
    }
    if (element.attribute("reh.dist")) {
        this->SetRehDist(StrToMeasurementsigned(element.attribute("reh.dist").value()));
        if (removeAttr) element.remove_attribute("reh.dist");
        hasAttribute = true;
    }
    if (element.attribute("tempo.dist")) {
        this->SetTempoDist(StrToMeasurementsigned(element.attribute("tempo.dist").value()));
        if (removeAttr) element.remove_attribute("tempo.dist");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDistances::WriteDistances(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDirDist()) {
        element.append_attribute("dir.dist") = MeasurementsignedToStr(this->GetDirDist()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDynamDist()) {
        element.append_attribute("dynam.dist") = MeasurementsignedToStr(this->GetDynamDist()).c_str();
        wroteAttribute = true;
    }
    if (this->HasHarmDist()) {
        element.append_attribute("harm.dist") = MeasurementsignedToStr(this->GetHarmDist()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRehDist()) {
        element.append_attribute("reh.dist") = MeasurementsignedToStr(this->GetRehDist()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTempoDist()) {
        element.append_attribute("tempo.dist") = MeasurementsignedToStr(this->GetTempoDist()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDistances::HasDirDist() const
{
    return (m_dirDist != data_MEASUREMENTSIGNED());
}

bool AttDistances::HasDynamDist() const
{
    return (m_dynamDist != data_MEASUREMENTSIGNED());
}

bool AttDistances::HasHarmDist() const
{
    return (m_harmDist != data_MEASUREMENTSIGNED());
}

bool AttDistances::HasRehDist() const
{
    return (m_rehDist != data_MEASUREMENTSIGNED());
}

bool AttDistances::HasTempoDist() const
{
    return (m_tempoDist != data_MEASUREMENTSIGNED());
}

//----------------------------------------------------------------------------
// AttDotLog
//----------------------------------------------------------------------------

AttDotLog::AttDotLog() : Att()
{
    ResetDotLog();
}

void AttDotLog::ResetDotLog()
{
    m_form = dotLog_FORM_NONE;
}

bool AttDotLog::ReadDotLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToDotLogForm(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
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

//----------------------------------------------------------------------------
// AttDurationAdditive
//----------------------------------------------------------------------------

AttDurationAdditive::AttDurationAdditive() : Att()
{
    ResetDurationAdditive();
}

void AttDurationAdditive::ResetDurationAdditive()
{
    m_dur = DURATION_NONE;
}

bool AttDurationAdditive::ReadDurationAdditive(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dur")) {
        this->SetDur(StrToDuration(element.attribute("dur").value()));
        if (removeAttr) element.remove_attribute("dur");
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

//----------------------------------------------------------------------------
// AttDurationDefault
//----------------------------------------------------------------------------

AttDurationDefault::AttDurationDefault() : Att()
{
    ResetDurationDefault();
}

void AttDurationDefault::ResetDurationDefault()
{
    m_durDefault = DURATION_NONE;
    m_numDefault = MEI_UNSET;
    m_numbaseDefault = MEI_UNSET;
}

bool AttDurationDefault::ReadDurationDefault(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dur.default")) {
        this->SetDurDefault(StrToDuration(element.attribute("dur.default").value()));
        if (removeAttr) element.remove_attribute("dur.default");
        hasAttribute = true;
    }
    if (element.attribute("num.default")) {
        this->SetNumDefault(StrToInt(element.attribute("num.default").value()));
        if (removeAttr) element.remove_attribute("num.default");
        hasAttribute = true;
    }
    if (element.attribute("numbase.default")) {
        this->SetNumbaseDefault(StrToInt(element.attribute("numbase.default").value()));
        if (removeAttr) element.remove_attribute("numbase.default");
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
    return (m_numDefault != MEI_UNSET);
}

bool AttDurationDefault::HasNumbaseDefault() const
{
    return (m_numbaseDefault != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttDurationLog
//----------------------------------------------------------------------------

AttDurationLog::AttDurationLog() : Att()
{
    ResetDurationLog();
}

void AttDurationLog::ResetDurationLog()
{
    m_dur = DURATION_NONE;
}

bool AttDurationLog::ReadDurationLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dur")) {
        this->SetDur(StrToDuration(element.attribute("dur").value()));
        if (removeAttr) element.remove_attribute("dur");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttDurationLog::WriteDurationLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDur()) {
        element.append_attribute("dur") = DurationToStr(this->GetDur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttDurationLog::HasDur() const
{
    return (m_dur != DURATION_NONE);
}

//----------------------------------------------------------------------------
// AttDurationRatio
//----------------------------------------------------------------------------

AttDurationRatio::AttDurationRatio() : Att()
{
    ResetDurationRatio();
}

void AttDurationRatio::ResetDurationRatio()
{
    m_num = MEI_UNSET;
    m_numbase = MEI_UNSET;
}

bool AttDurationRatio::ReadDurationRatio(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("num")) {
        this->SetNum(StrToInt(element.attribute("num").value()));
        if (removeAttr) element.remove_attribute("num");
        hasAttribute = true;
    }
    if (element.attribute("numbase")) {
        this->SetNumbase(StrToInt(element.attribute("numbase").value()));
        if (removeAttr) element.remove_attribute("numbase");
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
    return (m_num != MEI_UNSET);
}

bool AttDurationRatio::HasNumbase() const
{
    return (m_numbase != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttEnclosingChars
//----------------------------------------------------------------------------

AttEnclosingChars::AttEnclosingChars() : Att()
{
    ResetEnclosingChars();
}

void AttEnclosingChars::ResetEnclosingChars()
{
    m_enclose = ENCLOSURE_NONE;
}

bool AttEnclosingChars::ReadEnclosingChars(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("enclose")) {
        this->SetEnclose(StrToEnclosure(element.attribute("enclose").value()));
        if (removeAttr) element.remove_attribute("enclose");
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

//----------------------------------------------------------------------------
// AttEndings
//----------------------------------------------------------------------------

AttEndings::AttEndings() : Att()
{
    ResetEndings();
}

void AttEndings::ResetEndings()
{
    m_endingRend = endings_ENDINGREND_NONE;
}

bool AttEndings::ReadEndings(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("ending.rend")) {
        this->SetEndingRend(StrToEndingsEndingrend(element.attribute("ending.rend").value()));
        if (removeAttr) element.remove_attribute("ending.rend");
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

//----------------------------------------------------------------------------
// AttEvidence
//----------------------------------------------------------------------------

AttEvidence::AttEvidence() : Att()
{
    ResetEvidence();
}

void AttEvidence::ResetEvidence()
{
    m_cert = CERTAINTY_NONE;
    m_evidence = "";
}

bool AttEvidence::ReadEvidence(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("cert")) {
        this->SetCert(StrToCertainty(element.attribute("cert").value()));
        if (removeAttr) element.remove_attribute("cert");
        hasAttribute = true;
    }
    if (element.attribute("evidence")) {
        this->SetEvidence(StrToStr(element.attribute("evidence").value()));
        if (removeAttr) element.remove_attribute("evidence");
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

//----------------------------------------------------------------------------
// AttExtender
//----------------------------------------------------------------------------

AttExtender::AttExtender() : Att()
{
    ResetExtender();
}

void AttExtender::ResetExtender()
{
    m_extender = BOOLEAN_NONE;
}

bool AttExtender::ReadExtender(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("extender")) {
        this->SetExtender(StrToBoolean(element.attribute("extender").value()));
        if (removeAttr) element.remove_attribute("extender");
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

//----------------------------------------------------------------------------
// AttExtent
//----------------------------------------------------------------------------

AttExtent::AttExtent() : Att()
{
    ResetExtent();
}

void AttExtent::ResetExtent()
{
    m_extent = "";
}

bool AttExtent::ReadExtent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("extent")) {
        this->SetExtent(StrToStr(element.attribute("extent").value()));
        if (removeAttr) element.remove_attribute("extent");
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

//----------------------------------------------------------------------------
// AttFermataPresent
//----------------------------------------------------------------------------

AttFermataPresent::AttFermataPresent() : Att()
{
    ResetFermataPresent();
}

void AttFermataPresent::ResetFermataPresent()
{
    m_fermata = STAFFREL_basic_NONE;
}

bool AttFermataPresent::ReadFermataPresent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("fermata")) {
        this->SetFermata(StrToStaffrelBasic(element.attribute("fermata").value()));
        if (removeAttr) element.remove_attribute("fermata");
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

//----------------------------------------------------------------------------
// AttFiling
//----------------------------------------------------------------------------

AttFiling::AttFiling() : Att()
{
    ResetFiling();
}

void AttFiling::ResetFiling()
{
    m_nonfiling = MEI_UNSET;
}

bool AttFiling::ReadFiling(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("nonfiling")) {
        this->SetNonfiling(StrToInt(element.attribute("nonfiling").value()));
        if (removeAttr) element.remove_attribute("nonfiling");
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
    return (m_nonfiling != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttFormework
//----------------------------------------------------------------------------

AttFormework::AttFormework() : Att()
{
    ResetFormework();
}

void AttFormework::ResetFormework()
{
    m_func = PGFUNC_NONE;
}

bool AttFormework::ReadFormework(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToPgfunc(element.attribute("func").value()));
        if (removeAttr) element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFormework::WriteFormework(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = PgfuncToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFormework::HasFunc() const
{
    return (m_func != PGFUNC_NONE);
}

//----------------------------------------------------------------------------
// AttGrpSymLog
//----------------------------------------------------------------------------

AttGrpSymLog::AttGrpSymLog() : Att()
{
    ResetGrpSymLog();
}

void AttGrpSymLog::ResetGrpSymLog()
{
    m_level = MEI_UNSET;
}

bool AttGrpSymLog::ReadGrpSymLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("level")) {
        this->SetLevel(StrToInt(element.attribute("level").value()));
        if (removeAttr) element.remove_attribute("level");
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
    return (m_level != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttHandIdent
//----------------------------------------------------------------------------

AttHandIdent::AttHandIdent() : Att()
{
    ResetHandIdent();
}

void AttHandIdent::ResetHandIdent()
{
    m_hand = "";
}

bool AttHandIdent::ReadHandIdent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("hand")) {
        this->SetHand(StrToStr(element.attribute("hand").value()));
        if (removeAttr) element.remove_attribute("hand");
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

//----------------------------------------------------------------------------
// AttHeight
//----------------------------------------------------------------------------

AttHeight::AttHeight() : Att()
{
    ResetHeight();
}

void AttHeight::ResetHeight()
{
    m_height = data_MEASUREMENTUNSIGNED();
}

bool AttHeight::ReadHeight(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("height")) {
        this->SetHeight(StrToMeasurementunsigned(element.attribute("height").value()));
        if (removeAttr) element.remove_attribute("height");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHeight::WriteHeight(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasHeight()) {
        element.append_attribute("height") = MeasurementunsignedToStr(this->GetHeight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHeight::HasHeight() const
{
    return (m_height != data_MEASUREMENTUNSIGNED());
}

//----------------------------------------------------------------------------
// AttHorizontalAlign
//----------------------------------------------------------------------------

AttHorizontalAlign::AttHorizontalAlign() : Att()
{
    ResetHorizontalAlign();
}

void AttHorizontalAlign::ResetHorizontalAlign()
{
    m_halign = HORIZONTALALIGNMENT_NONE;
}

bool AttHorizontalAlign::ReadHorizontalAlign(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("halign")) {
        this->SetHalign(StrToHorizontalalignment(element.attribute("halign").value()));
        if (removeAttr) element.remove_attribute("halign");
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

//----------------------------------------------------------------------------
// AttInternetMedia
//----------------------------------------------------------------------------

AttInternetMedia::AttInternetMedia() : Att()
{
    ResetInternetMedia();
}

void AttInternetMedia::ResetInternetMedia()
{
    m_mimetype = "";
}

bool AttInternetMedia::ReadInternetMedia(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("mimetype")) {
        this->SetMimetype(StrToStr(element.attribute("mimetype").value()));
        if (removeAttr) element.remove_attribute("mimetype");
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

//----------------------------------------------------------------------------
// AttJoined
//----------------------------------------------------------------------------

AttJoined::AttJoined() : Att()
{
    ResetJoined();
}

void AttJoined::ResetJoined()
{
    m_join = "";
}

bool AttJoined::ReadJoined(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("join")) {
        this->SetJoin(StrToStr(element.attribute("join").value()));
        if (removeAttr) element.remove_attribute("join");
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

//----------------------------------------------------------------------------
// AttKeyMode
//----------------------------------------------------------------------------

AttKeyMode::AttKeyMode() : Att()
{
    ResetKeyMode();
}

void AttKeyMode::ResetKeyMode()
{
    m_mode = MODE_NONE;
}

bool AttKeyMode::ReadKeyMode(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("mode")) {
        this->SetMode(StrToMode(element.attribute("mode").value()));
        if (removeAttr) element.remove_attribute("mode");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeyMode::WriteKeyMode(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMode()) {
        element.append_attribute("mode") = ModeToStr(this->GetMode()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeyMode::HasMode() const
{
    return (m_mode != MODE_NONE);
}

//----------------------------------------------------------------------------
// AttKeySigLog
//----------------------------------------------------------------------------

AttKeySigLog::AttKeySigLog() : Att()
{
    ResetKeySigLog();
}

void AttKeySigLog::ResetKeySigLog()
{
    m_sig = std::make_pair(-1, ACCIDENTAL_WRITTEN_NONE);
}

bool AttKeySigLog::ReadKeySigLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("sig")) {
        this->SetSig(StrToKeysignature(element.attribute("sig").value()));
        if (removeAttr) element.remove_attribute("sig");
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

//----------------------------------------------------------------------------
// AttKeySigDefaultLog
//----------------------------------------------------------------------------

AttKeySigDefaultLog::AttKeySigDefaultLog() : Att()
{
    ResetKeySigDefaultLog();
}

void AttKeySigDefaultLog::ResetKeySigDefaultLog()
{
    m_keysig = std::make_pair(-1, ACCIDENTAL_WRITTEN_NONE);
}

bool AttKeySigDefaultLog::ReadKeySigDefaultLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("keysig")) {
        this->SetKeysig(StrToKeysignature(element.attribute("keysig").value()));
        if (removeAttr) element.remove_attribute("keysig");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigDefaultLog::WriteKeySigDefaultLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasKeysig()) {
        element.append_attribute("keysig") = KeysignatureToStr(this->GetKeysig()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigDefaultLog::HasKeysig() const
{
    return (m_keysig != std::make_pair(-1, ACCIDENTAL_WRITTEN_NONE));
}

//----------------------------------------------------------------------------
// AttLabelled
//----------------------------------------------------------------------------

AttLabelled::AttLabelled() : Att()
{
    ResetLabelled();
}

void AttLabelled::ResetLabelled()
{
    m_label = "";
}

bool AttLabelled::ReadLabelled(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("label")) {
        this->SetLabel(StrToStr(element.attribute("label").value()));
        if (removeAttr) element.remove_attribute("label");
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

//----------------------------------------------------------------------------
// AttLang
//----------------------------------------------------------------------------

AttLang::AttLang() : Att()
{
    ResetLang();
}

void AttLang::ResetLang()
{
    m_lang = "";
    m_translit = "";
}

bool AttLang::ReadLang(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("xml:lang")) {
        this->SetLang(StrToStr(element.attribute("xml:lang").value()));
        if (removeAttr) element.remove_attribute("xml:lang");
        hasAttribute = true;
    }
    if (element.attribute("translit")) {
        this->SetTranslit(StrToStr(element.attribute("translit").value()));
        if (removeAttr) element.remove_attribute("translit");
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
        element.append_attribute("translit") = StrToStr(this->GetTranslit()).c_str();
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

//----------------------------------------------------------------------------
// AttLayerLog
//----------------------------------------------------------------------------

AttLayerLog::AttLayerLog() : Att()
{
    ResetLayerLog();
}

void AttLayerLog::ResetLayerLog()
{
    m_def = "";
}

bool AttLayerLog::ReadLayerLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("def")) {
        this->SetDef(StrToStr(element.attribute("def").value()));
        if (removeAttr) element.remove_attribute("def");
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

//----------------------------------------------------------------------------
// AttLayerIdent
//----------------------------------------------------------------------------

AttLayerIdent::AttLayerIdent() : Att()
{
    ResetLayerIdent();
}

void AttLayerIdent::ResetLayerIdent()
{
    m_layer = MEI_UNSET;
}

bool AttLayerIdent::ReadLayerIdent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("layer")) {
        this->SetLayer(StrToInt(element.attribute("layer").value()));
        if (removeAttr) element.remove_attribute("layer");
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
    return (m_layer != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttLineLoc
//----------------------------------------------------------------------------

AttLineLoc::AttLineLoc() : Att()
{
    ResetLineLoc();
}

void AttLineLoc::ResetLineLoc()
{
    m_line = 0;
}

bool AttLineLoc::ReadLineLoc(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("line")) {
        this->SetLine(StrToInt(element.attribute("line").value()));
        if (removeAttr) element.remove_attribute("line");
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

//----------------------------------------------------------------------------
// AttLineRend
//----------------------------------------------------------------------------

AttLineRend::AttLineRend() : Att()
{
    ResetLineRend();
}

void AttLineRend::ResetLineRend()
{
    m_lendsym = LINESTARTENDSYMBOL_NONE;
    m_lendsymSize = MEI_UNSET;
    m_lstartsym = LINESTARTENDSYMBOL_NONE;
    m_lstartsymSize = MEI_UNSET;
}

bool AttLineRend::ReadLineRend(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("lendsym")) {
        this->SetLendsym(StrToLinestartendsymbol(element.attribute("lendsym").value()));
        if (removeAttr) element.remove_attribute("lendsym");
        hasAttribute = true;
    }
    if (element.attribute("lendsym.size")) {
        this->SetLendsymSize(StrToInt(element.attribute("lendsym.size").value()));
        if (removeAttr) element.remove_attribute("lendsym.size");
        hasAttribute = true;
    }
    if (element.attribute("lstartsym")) {
        this->SetLstartsym(StrToLinestartendsymbol(element.attribute("lstartsym").value()));
        if (removeAttr) element.remove_attribute("lstartsym");
        hasAttribute = true;
    }
    if (element.attribute("lstartsym.size")) {
        this->SetLstartsymSize(StrToInt(element.attribute("lstartsym.size").value()));
        if (removeAttr) element.remove_attribute("lstartsym.size");
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
    return (m_lendsymSize != MEI_UNSET);
}

bool AttLineRend::HasLstartsym() const
{
    return (m_lstartsym != LINESTARTENDSYMBOL_NONE);
}

bool AttLineRend::HasLstartsymSize() const
{
    return (m_lstartsymSize != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttLineRendBase
//----------------------------------------------------------------------------

AttLineRendBase::AttLineRendBase() : Att()
{
    ResetLineRendBase();
}

void AttLineRendBase::ResetLineRendBase()
{
    m_lform = LINEFORM_NONE;
    m_lwidth = data_LINEWIDTH();
    m_lsegs = MEI_UNSET;
}

bool AttLineRendBase::ReadLineRendBase(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("lform")) {
        this->SetLform(StrToLineform(element.attribute("lform").value()));
        if (removeAttr) element.remove_attribute("lform");
        hasAttribute = true;
    }
    if (element.attribute("lwidth")) {
        this->SetLwidth(StrToLinewidth(element.attribute("lwidth").value()));
        if (removeAttr) element.remove_attribute("lwidth");
        hasAttribute = true;
    }
    if (element.attribute("lsegs")) {
        this->SetLsegs(StrToInt(element.attribute("lsegs").value()));
        if (removeAttr) element.remove_attribute("lsegs");
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
    if (this->HasLwidth()) {
        element.append_attribute("lwidth") = LinewidthToStr(this->GetLwidth()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLsegs()) {
        element.append_attribute("lsegs") = IntToStr(this->GetLsegs()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLineRendBase::HasLform() const
{
    return (m_lform != LINEFORM_NONE);
}

bool AttLineRendBase::HasLwidth() const
{
    return (m_lwidth.HasValue());
}

bool AttLineRendBase::HasLsegs() const
{
    return (m_lsegs != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttLinking
//----------------------------------------------------------------------------

AttLinking::AttLinking() : Att()
{
    ResetLinking();
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

bool AttLinking::ReadLinking(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("copyof")) {
        this->SetCopyof(StrToStr(element.attribute("copyof").value()));
        if (removeAttr) element.remove_attribute("copyof");
        hasAttribute = true;
    }
    if (element.attribute("corresp")) {
        this->SetCorresp(StrToStr(element.attribute("corresp").value()));
        if (removeAttr) element.remove_attribute("corresp");
        hasAttribute = true;
    }
    if (element.attribute("follows")) {
        this->SetFollows(StrToStr(element.attribute("follows").value()));
        if (removeAttr) element.remove_attribute("follows");
        hasAttribute = true;
    }
    if (element.attribute("next")) {
        this->SetNext(StrToStr(element.attribute("next").value()));
        if (removeAttr) element.remove_attribute("next");
        hasAttribute = true;
    }
    if (element.attribute("precedes")) {
        this->SetPrecedes(StrToStr(element.attribute("precedes").value()));
        if (removeAttr) element.remove_attribute("precedes");
        hasAttribute = true;
    }
    if (element.attribute("prev")) {
        this->SetPrev(StrToStr(element.attribute("prev").value()));
        if (removeAttr) element.remove_attribute("prev");
        hasAttribute = true;
    }
    if (element.attribute("sameas")) {
        this->SetSameas(StrToStr(element.attribute("sameas").value()));
        if (removeAttr) element.remove_attribute("sameas");
        hasAttribute = true;
    }
    if (element.attribute("synch")) {
        this->SetSynch(StrToStr(element.attribute("synch").value()));
        if (removeAttr) element.remove_attribute("synch");
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

//----------------------------------------------------------------------------
// AttLyricStyle
//----------------------------------------------------------------------------

AttLyricStyle::AttLyricStyle() : Att()
{
    ResetLyricStyle();
}

void AttLyricStyle::ResetLyricStyle()
{
    m_lyricAlign = data_MEASUREMENTSIGNED();
    m_lyricFam = "";
    m_lyricName = "";
    m_lyricSize = data_FONTSIZE();
    m_lyricStyle = FONTSTYLE_NONE;
    m_lyricWeight = FONTWEIGHT_NONE;
}

bool AttLyricStyle::ReadLyricStyle(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("lyric.align")) {
        this->SetLyricAlign(StrToMeasurementsigned(element.attribute("lyric.align").value()));
        if (removeAttr) element.remove_attribute("lyric.align");
        hasAttribute = true;
    }
    if (element.attribute("lyric.fam")) {
        this->SetLyricFam(StrToStr(element.attribute("lyric.fam").value()));
        if (removeAttr) element.remove_attribute("lyric.fam");
        hasAttribute = true;
    }
    if (element.attribute("lyric.name")) {
        this->SetLyricName(StrToStr(element.attribute("lyric.name").value()));
        if (removeAttr) element.remove_attribute("lyric.name");
        hasAttribute = true;
    }
    if (element.attribute("lyric.size")) {
        this->SetLyricSize(StrToFontsize(element.attribute("lyric.size").value()));
        if (removeAttr) element.remove_attribute("lyric.size");
        hasAttribute = true;
    }
    if (element.attribute("lyric.style")) {
        this->SetLyricStyle(StrToFontstyle(element.attribute("lyric.style").value()));
        if (removeAttr) element.remove_attribute("lyric.style");
        hasAttribute = true;
    }
    if (element.attribute("lyric.weight")) {
        this->SetLyricWeight(StrToFontweight(element.attribute("lyric.weight").value()));
        if (removeAttr) element.remove_attribute("lyric.weight");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLyricStyle::WriteLyricStyle(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLyricAlign()) {
        element.append_attribute("lyric.align") = MeasurementsignedToStr(this->GetLyricAlign()).c_str();
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
    return (m_lyricAlign != data_MEASUREMENTSIGNED());
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

//----------------------------------------------------------------------------
// AttMeasureNumbers
//----------------------------------------------------------------------------

AttMeasureNumbers::AttMeasureNumbers() : Att()
{
    ResetMeasureNumbers();
}

void AttMeasureNumbers::ResetMeasureNumbers()
{
    m_mnumVisible = BOOLEAN_NONE;
}

bool AttMeasureNumbers::ReadMeasureNumbers(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("mnum.visible")) {
        this->SetMnumVisible(StrToBoolean(element.attribute("mnum.visible").value()));
        if (removeAttr) element.remove_attribute("mnum.visible");
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

//----------------------------------------------------------------------------
// AttMeasurement
//----------------------------------------------------------------------------

AttMeasurement::AttMeasurement() : Att()
{
    ResetMeasurement();
}

void AttMeasurement::ResetMeasurement()
{
    m_unit = "";
}

bool AttMeasurement::ReadMeasurement(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("unit")) {
        this->SetUnit(StrToStr(element.attribute("unit").value()));
        if (removeAttr) element.remove_attribute("unit");
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

//----------------------------------------------------------------------------
// AttMediaBounds
//----------------------------------------------------------------------------

AttMediaBounds::AttMediaBounds() : Att()
{
    ResetMediaBounds();
}

void AttMediaBounds::ResetMediaBounds()
{
    m_begin = "";
    m_end = "";
    m_betype = BETYPE_NONE;
}

bool AttMediaBounds::ReadMediaBounds(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("begin")) {
        this->SetBegin(StrToStr(element.attribute("begin").value()));
        if (removeAttr) element.remove_attribute("begin");
        hasAttribute = true;
    }
    if (element.attribute("end")) {
        this->SetEnd(StrToStr(element.attribute("end").value()));
        if (removeAttr) element.remove_attribute("end");
        hasAttribute = true;
    }
    if (element.attribute("betype")) {
        this->SetBetype(StrToBetype(element.attribute("betype").value()));
        if (removeAttr) element.remove_attribute("betype");
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

//----------------------------------------------------------------------------
// AttMedium
//----------------------------------------------------------------------------

AttMedium::AttMedium() : Att()
{
    ResetMedium();
}

void AttMedium::ResetMedium()
{
    m_medium = "";
}

bool AttMedium::ReadMedium(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("medium")) {
        this->SetMedium(StrToStr(element.attribute("medium").value()));
        if (removeAttr) element.remove_attribute("medium");
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

//----------------------------------------------------------------------------
// AttMeiVersion
//----------------------------------------------------------------------------

AttMeiVersion::AttMeiVersion() : Att()
{
    ResetMeiVersion();
}

void AttMeiVersion::ResetMeiVersion()
{
    m_meiversion = meiVersion_MEIVERSION_NONE;
}

bool AttMeiVersion::ReadMeiVersion(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("meiversion")) {
        this->SetMeiversion(StrToMeiVersionMeiversion(element.attribute("meiversion").value()));
        if (removeAttr) element.remove_attribute("meiversion");
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

//----------------------------------------------------------------------------
// AttMensurLog
//----------------------------------------------------------------------------

AttMensurLog::AttMensurLog() : Att()
{
    ResetMensurLog();
}

void AttMensurLog::ResetMensurLog()
{
    m_level = DURATION_NONE;
}

bool AttMensurLog::ReadMensurLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("level")) {
        this->SetLevel(StrToDuration(element.attribute("level").value()));
        if (removeAttr) element.remove_attribute("level");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensurLog::WriteMensurLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLevel()) {
        element.append_attribute("level") = DurationToStr(this->GetLevel()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensurLog::HasLevel() const
{
    return (m_level != DURATION_NONE);
}

//----------------------------------------------------------------------------
// AttMetadataPointing
//----------------------------------------------------------------------------

AttMetadataPointing::AttMetadataPointing() : Att()
{
    ResetMetadataPointing();
}

void AttMetadataPointing::ResetMetadataPointing()
{
    m_decls = "";
}

bool AttMetadataPointing::ReadMetadataPointing(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("decls")) {
        this->SetDecls(StrToStr(element.attribute("decls").value()));
        if (removeAttr) element.remove_attribute("decls");
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

//----------------------------------------------------------------------------
// AttMeterConformance
//----------------------------------------------------------------------------

AttMeterConformance::AttMeterConformance() : Att()
{
    ResetMeterConformance();
}

void AttMeterConformance::ResetMeterConformance()
{
    m_metcon = meterConformance_METCON_NONE;
}

bool AttMeterConformance::ReadMeterConformance(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("metcon")) {
        this->SetMetcon(StrToMeterConformanceMetcon(element.attribute("metcon").value()));
        if (removeAttr) element.remove_attribute("metcon");
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

//----------------------------------------------------------------------------
// AttMeterConformanceBar
//----------------------------------------------------------------------------

AttMeterConformanceBar::AttMeterConformanceBar() : Att()
{
    ResetMeterConformanceBar();
}

void AttMeterConformanceBar::ResetMeterConformanceBar()
{
    m_metcon = BOOLEAN_NONE;
    m_control = BOOLEAN_NONE;
}

bool AttMeterConformanceBar::ReadMeterConformanceBar(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("metcon")) {
        this->SetMetcon(StrToBoolean(element.attribute("metcon").value()));
        if (removeAttr) element.remove_attribute("metcon");
        hasAttribute = true;
    }
    if (element.attribute("control")) {
        this->SetControl(StrToBoolean(element.attribute("control").value()));
        if (removeAttr) element.remove_attribute("control");
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

//----------------------------------------------------------------------------
// AttMeterSigLog
//----------------------------------------------------------------------------

AttMeterSigLog::AttMeterSigLog() : Att()
{
    ResetMeterSigLog();
}

void AttMeterSigLog::ResetMeterSigLog()
{
    m_count = std::pair<std::vector<int>, MeterCountSign>();
    m_sym = METERSIGN_NONE;
    m_unit = MEI_UNSET;
}

bool AttMeterSigLog::ReadMeterSigLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("count")) {
        this->SetCount(StrToMetercountPair(element.attribute("count").value()));
        if (removeAttr) element.remove_attribute("count");
        hasAttribute = true;
    }
    if (element.attribute("sym")) {
        this->SetSym(StrToMetersign(element.attribute("sym").value()));
        if (removeAttr) element.remove_attribute("sym");
        hasAttribute = true;
    }
    if (element.attribute("unit")) {
        this->SetUnit(StrToInt(element.attribute("unit").value()));
        if (removeAttr) element.remove_attribute("unit");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigLog::WriteMeterSigLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCount()) {
        element.append_attribute("count") = MetercountPairToStr(this->GetCount()).c_str();
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
    return (m_count != std::pair<std::vector<int>, MeterCountSign>());
}

bool AttMeterSigLog::HasSym() const
{
    return (m_sym != METERSIGN_NONE);
}

bool AttMeterSigLog::HasUnit() const
{
    return (m_unit != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttMeterSigDefaultLog
//----------------------------------------------------------------------------

AttMeterSigDefaultLog::AttMeterSigDefaultLog() : Att()
{
    ResetMeterSigDefaultLog();
}

void AttMeterSigDefaultLog::ResetMeterSigDefaultLog()
{
    m_meterCount = std::pair<std::vector<int>, MeterCountSign>();
    m_meterUnit = MEI_UNSET;
    m_meterSym = METERSIGN_NONE;
}

bool AttMeterSigDefaultLog::ReadMeterSigDefaultLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("meter.count")) {
        this->SetMeterCount(StrToMetercountPair(element.attribute("meter.count").value()));
        if (removeAttr) element.remove_attribute("meter.count");
        hasAttribute = true;
    }
    if (element.attribute("meter.unit")) {
        this->SetMeterUnit(StrToInt(element.attribute("meter.unit").value()));
        if (removeAttr) element.remove_attribute("meter.unit");
        hasAttribute = true;
    }
    if (element.attribute("meter.sym")) {
        this->SetMeterSym(StrToMetersign(element.attribute("meter.sym").value()));
        if (removeAttr) element.remove_attribute("meter.sym");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigDefaultLog::WriteMeterSigDefaultLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMeterCount()) {
        element.append_attribute("meter.count") = MetercountPairToStr(this->GetMeterCount()).c_str();
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
    return (m_meterCount != std::pair<std::vector<int>, MeterCountSign>());
}

bool AttMeterSigDefaultLog::HasMeterUnit() const
{
    return (m_meterUnit != MEI_UNSET);
}

bool AttMeterSigDefaultLog::HasMeterSym() const
{
    return (m_meterSym != METERSIGN_NONE);
}

//----------------------------------------------------------------------------
// AttMmTempo
//----------------------------------------------------------------------------

AttMmTempo::AttMmTempo() : Att()
{
    ResetMmTempo();
}

void AttMmTempo::ResetMmTempo()
{
    m_mm = 0.0;
    m_mmUnit = DURATION_NONE;
    m_mmDots = MEI_UNSET;
}

bool AttMmTempo::ReadMmTempo(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("mm")) {
        this->SetMm(StrToDbl(element.attribute("mm").value()));
        if (removeAttr) element.remove_attribute("mm");
        hasAttribute = true;
    }
    if (element.attribute("mm.unit")) {
        this->SetMmUnit(StrToDuration(element.attribute("mm.unit").value()));
        if (removeAttr) element.remove_attribute("mm.unit");
        hasAttribute = true;
    }
    if (element.attribute("mm.dots")) {
        this->SetMmDots(StrToInt(element.attribute("mm.dots").value()));
        if (removeAttr) element.remove_attribute("mm.dots");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMmTempo::WriteMmTempo(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMm()) {
        element.append_attribute("mm") = DblToStr(this->GetMm()).c_str();
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
    return (m_mm != 0.0);
}

bool AttMmTempo::HasMmUnit() const
{
    return (m_mmUnit != DURATION_NONE);
}

bool AttMmTempo::HasMmDots() const
{
    return (m_mmDots != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttMultinumMeasures
//----------------------------------------------------------------------------

AttMultinumMeasures::AttMultinumMeasures() : Att()
{
    ResetMultinumMeasures();
}

void AttMultinumMeasures::ResetMultinumMeasures()
{
    m_multiNumber = BOOLEAN_NONE;
}

bool AttMultinumMeasures::ReadMultinumMeasures(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("multi.number")) {
        this->SetMultiNumber(StrToBoolean(element.attribute("multi.number").value()));
        if (removeAttr) element.remove_attribute("multi.number");
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

//----------------------------------------------------------------------------
// AttNInteger
//----------------------------------------------------------------------------

AttNInteger::AttNInteger() : Att()
{
    ResetNInteger();
}

void AttNInteger::ResetNInteger()
{
    m_n = MEI_UNSET;
}

bool AttNInteger::ReadNInteger(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("n")) {
        this->SetN(StrToInt(element.attribute("n").value()));
        if (removeAttr) element.remove_attribute("n");
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
    return (m_n != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttNNumberLike
//----------------------------------------------------------------------------

AttNNumberLike::AttNNumberLike() : Att()
{
    ResetNNumberLike();
}

void AttNNumberLike::ResetNNumberLike()
{
    m_n = "";
}

bool AttNNumberLike::ReadNNumberLike(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("n")) {
        this->SetN(StrToStr(element.attribute("n").value()));
        if (removeAttr) element.remove_attribute("n");
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

//----------------------------------------------------------------------------
// AttName
//----------------------------------------------------------------------------

AttName::AttName() : Att()
{
    ResetName();
}

void AttName::ResetName()
{
    m_nymref = "";
    m_role = "";
}

bool AttName::ReadName(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("nymref")) {
        this->SetNymref(StrToStr(element.attribute("nymref").value()));
        if (removeAttr) element.remove_attribute("nymref");
        hasAttribute = true;
    }
    if (element.attribute("role")) {
        this->SetRole(StrToStr(element.attribute("role").value()));
        if (removeAttr) element.remove_attribute("role");
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

//----------------------------------------------------------------------------
// AttNotationStyle
//----------------------------------------------------------------------------

AttNotationStyle::AttNotationStyle() : Att()
{
    ResetNotationStyle();
}

void AttNotationStyle::ResetNotationStyle()
{
    m_musicName = "";
    m_musicSize = data_FONTSIZE();
}

bool AttNotationStyle::ReadNotationStyle(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("music.name")) {
        this->SetMusicName(StrToStr(element.attribute("music.name").value()));
        if (removeAttr) element.remove_attribute("music.name");
        hasAttribute = true;
    }
    if (element.attribute("music.size")) {
        this->SetMusicSize(StrToFontsize(element.attribute("music.size").value()));
        if (removeAttr) element.remove_attribute("music.size");
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

//----------------------------------------------------------------------------
// AttNoteHeads
//----------------------------------------------------------------------------

AttNoteHeads::AttNoteHeads() : Att()
{
    ResetNoteHeads();
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

bool AttNoteHeads::ReadNoteHeads(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("head.altsym")) {
        this->SetHeadAltsym(StrToStr(element.attribute("head.altsym").value()));
        if (removeAttr) element.remove_attribute("head.altsym");
        hasAttribute = true;
    }
    if (element.attribute("head.auth")) {
        this->SetHeadAuth(StrToStr(element.attribute("head.auth").value()));
        if (removeAttr) element.remove_attribute("head.auth");
        hasAttribute = true;
    }
    if (element.attribute("head.color")) {
        this->SetHeadColor(StrToStr(element.attribute("head.color").value()));
        if (removeAttr) element.remove_attribute("head.color");
        hasAttribute = true;
    }
    if (element.attribute("head.fill")) {
        this->SetHeadFill(StrToFill(element.attribute("head.fill").value()));
        if (removeAttr) element.remove_attribute("head.fill");
        hasAttribute = true;
    }
    if (element.attribute("head.fillcolor")) {
        this->SetHeadFillcolor(StrToStr(element.attribute("head.fillcolor").value()));
        if (removeAttr) element.remove_attribute("head.fillcolor");
        hasAttribute = true;
    }
    if (element.attribute("head.mod")) {
        this->SetHeadMod(StrToNoteheadmodifier(element.attribute("head.mod").value()));
        if (removeAttr) element.remove_attribute("head.mod");
        hasAttribute = true;
    }
    if (element.attribute("head.rotation")) {
        this->SetHeadRotation(StrToRotation(element.attribute("head.rotation").value()));
        if (removeAttr) element.remove_attribute("head.rotation");
        hasAttribute = true;
    }
    if (element.attribute("head.shape")) {
        this->SetHeadShape(StrToHeadshape(element.attribute("head.shape").value()));
        if (removeAttr) element.remove_attribute("head.shape");
        hasAttribute = true;
    }
    if (element.attribute("head.visible")) {
        this->SetHeadVisible(StrToBoolean(element.attribute("head.visible").value()));
        if (removeAttr) element.remove_attribute("head.visible");
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

//----------------------------------------------------------------------------
// AttOctave
//----------------------------------------------------------------------------

AttOctave::AttOctave() : Att()
{
    ResetOctave();
}

void AttOctave::ResetOctave()
{
    m_oct = MEI_UNSET_OCT;
}

bool AttOctave::ReadOctave(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("oct")) {
        this->SetOct(StrToOctave(element.attribute("oct").value()));
        if (removeAttr) element.remove_attribute("oct");
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
    return (m_oct != MEI_UNSET_OCT);
}

//----------------------------------------------------------------------------
// AttOctaveDefault
//----------------------------------------------------------------------------

AttOctaveDefault::AttOctaveDefault() : Att()
{
    ResetOctaveDefault();
}

void AttOctaveDefault::ResetOctaveDefault()
{
    m_octDefault = MEI_UNSET_OCT;
}

bool AttOctaveDefault::ReadOctaveDefault(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("oct.default")) {
        this->SetOctDefault(StrToOctave(element.attribute("oct.default").value()));
        if (removeAttr) element.remove_attribute("oct.default");
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
    return (m_octDefault != MEI_UNSET_OCT);
}

//----------------------------------------------------------------------------
// AttOctaveDisplacement
//----------------------------------------------------------------------------

AttOctaveDisplacement::AttOctaveDisplacement() : Att()
{
    ResetOctaveDisplacement();
}

void AttOctaveDisplacement::ResetOctaveDisplacement()
{
    m_dis = OCTAVE_DIS_NONE;
    m_disPlace = STAFFREL_basic_NONE;
}

bool AttOctaveDisplacement::ReadOctaveDisplacement(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dis")) {
        this->SetDis(StrToOctaveDis(element.attribute("dis").value()));
        if (removeAttr) element.remove_attribute("dis");
        hasAttribute = true;
    }
    if (element.attribute("dis.place")) {
        this->SetDisPlace(StrToStaffrelBasic(element.attribute("dis.place").value()));
        if (removeAttr) element.remove_attribute("dis.place");
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

//----------------------------------------------------------------------------
// AttOneLineStaff
//----------------------------------------------------------------------------

AttOneLineStaff::AttOneLineStaff() : Att()
{
    ResetOneLineStaff();
}

void AttOneLineStaff::ResetOneLineStaff()
{
    m_ontheline = BOOLEAN_NONE;
}

bool AttOneLineStaff::ReadOneLineStaff(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("ontheline")) {
        this->SetOntheline(StrToBoolean(element.attribute("ontheline").value()));
        if (removeAttr) element.remove_attribute("ontheline");
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

//----------------------------------------------------------------------------
// AttOptimization
//----------------------------------------------------------------------------

AttOptimization::AttOptimization() : Att()
{
    ResetOptimization();
}

void AttOptimization::ResetOptimization()
{
    m_optimize = BOOLEAN_NONE;
}

bool AttOptimization::ReadOptimization(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("optimize")) {
        this->SetOptimize(StrToBoolean(element.attribute("optimize").value()));
        if (removeAttr) element.remove_attribute("optimize");
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

//----------------------------------------------------------------------------
// AttOriginLayerIdent
//----------------------------------------------------------------------------

AttOriginLayerIdent::AttOriginLayerIdent() : Att()
{
    ResetOriginLayerIdent();
}

void AttOriginLayerIdent::ResetOriginLayerIdent()
{
    m_originLayer = "";
}

bool AttOriginLayerIdent::ReadOriginLayerIdent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("origin.layer")) {
        this->SetOriginLayer(StrToStr(element.attribute("origin.layer").value()));
        if (removeAttr) element.remove_attribute("origin.layer");
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

//----------------------------------------------------------------------------
// AttOriginStaffIdent
//----------------------------------------------------------------------------

AttOriginStaffIdent::AttOriginStaffIdent() : Att()
{
    ResetOriginStaffIdent();
}

void AttOriginStaffIdent::ResetOriginStaffIdent()
{
    m_originStaff = "";
}

bool AttOriginStaffIdent::ReadOriginStaffIdent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("origin.staff")) {
        this->SetOriginStaff(StrToStr(element.attribute("origin.staff").value()));
        if (removeAttr) element.remove_attribute("origin.staff");
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

//----------------------------------------------------------------------------
// AttOriginStartEndId
//----------------------------------------------------------------------------

AttOriginStartEndId::AttOriginStartEndId() : Att()
{
    ResetOriginStartEndId();
}

void AttOriginStartEndId::ResetOriginStartEndId()
{
    m_originStartid = "";
    m_originEndid = "";
}

bool AttOriginStartEndId::ReadOriginStartEndId(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("origin.startid")) {
        this->SetOriginStartid(StrToStr(element.attribute("origin.startid").value()));
        if (removeAttr) element.remove_attribute("origin.startid");
        hasAttribute = true;
    }
    if (element.attribute("origin.endid")) {
        this->SetOriginEndid(StrToStr(element.attribute("origin.endid").value()));
        if (removeAttr) element.remove_attribute("origin.endid");
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

//----------------------------------------------------------------------------
// AttOriginTimestampLog
//----------------------------------------------------------------------------

AttOriginTimestampLog::AttOriginTimestampLog() : Att()
{
    ResetOriginTimestampLog();
}

void AttOriginTimestampLog::ResetOriginTimestampLog()
{
    m_originTstamp = std::make_pair(-1, -1.0);
    m_originTstamp2 = std::make_pair(-1, -1.0);
}

bool AttOriginTimestampLog::ReadOriginTimestampLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("origin.tstamp")) {
        this->SetOriginTstamp(StrToMeasurebeat(element.attribute("origin.tstamp").value()));
        if (removeAttr) element.remove_attribute("origin.tstamp");
        hasAttribute = true;
    }
    if (element.attribute("origin.tstamp2")) {
        this->SetOriginTstamp2(StrToMeasurebeat(element.attribute("origin.tstamp2").value()));
        if (removeAttr) element.remove_attribute("origin.tstamp2");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOriginTimestampLog::WriteOriginTimestampLog(pugi::xml_node element)
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

bool AttOriginTimestampLog::HasOriginTstamp() const
{
    return (m_originTstamp != std::make_pair(-1, -1.0));
}

bool AttOriginTimestampLog::HasOriginTstamp2() const
{
    return (m_originTstamp2 != std::make_pair(-1, -1.0));
}

//----------------------------------------------------------------------------
// AttPages
//----------------------------------------------------------------------------

AttPages::AttPages() : Att()
{
    ResetPages();
}

void AttPages::ResetPages()
{
    m_pageHeight = data_MEASUREMENTUNSIGNED();
    m_pageWidth = data_MEASUREMENTUNSIGNED();
    m_pageTopmar = data_MEASUREMENTUNSIGNED();
    m_pageBotmar = data_MEASUREMENTUNSIGNED();
    m_pageLeftmar = data_MEASUREMENTUNSIGNED();
    m_pageRightmar = data_MEASUREMENTUNSIGNED();
    m_pagePanels = "";
    m_pageScale = "";
}

bool AttPages::ReadPages(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("page.height")) {
        this->SetPageHeight(StrToMeasurementunsigned(element.attribute("page.height").value()));
        if (removeAttr) element.remove_attribute("page.height");
        hasAttribute = true;
    }
    if (element.attribute("page.width")) {
        this->SetPageWidth(StrToMeasurementunsigned(element.attribute("page.width").value()));
        if (removeAttr) element.remove_attribute("page.width");
        hasAttribute = true;
    }
    if (element.attribute("page.topmar")) {
        this->SetPageTopmar(StrToMeasurementunsigned(element.attribute("page.topmar").value()));
        if (removeAttr) element.remove_attribute("page.topmar");
        hasAttribute = true;
    }
    if (element.attribute("page.botmar")) {
        this->SetPageBotmar(StrToMeasurementunsigned(element.attribute("page.botmar").value()));
        if (removeAttr) element.remove_attribute("page.botmar");
        hasAttribute = true;
    }
    if (element.attribute("page.leftmar")) {
        this->SetPageLeftmar(StrToMeasurementunsigned(element.attribute("page.leftmar").value()));
        if (removeAttr) element.remove_attribute("page.leftmar");
        hasAttribute = true;
    }
    if (element.attribute("page.rightmar")) {
        this->SetPageRightmar(StrToMeasurementunsigned(element.attribute("page.rightmar").value()));
        if (removeAttr) element.remove_attribute("page.rightmar");
        hasAttribute = true;
    }
    if (element.attribute("page.panels")) {
        this->SetPagePanels(StrToStr(element.attribute("page.panels").value()));
        if (removeAttr) element.remove_attribute("page.panels");
        hasAttribute = true;
    }
    if (element.attribute("page.scale")) {
        this->SetPageScale(StrToStr(element.attribute("page.scale").value()));
        if (removeAttr) element.remove_attribute("page.scale");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPages::WritePages(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPageHeight()) {
        element.append_attribute("page.height") = MeasurementunsignedToStr(this->GetPageHeight()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageWidth()) {
        element.append_attribute("page.width") = MeasurementunsignedToStr(this->GetPageWidth()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageTopmar()) {
        element.append_attribute("page.topmar") = MeasurementunsignedToStr(this->GetPageTopmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageBotmar()) {
        element.append_attribute("page.botmar") = MeasurementunsignedToStr(this->GetPageBotmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageLeftmar()) {
        element.append_attribute("page.leftmar") = MeasurementunsignedToStr(this->GetPageLeftmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPageRightmar()) {
        element.append_attribute("page.rightmar") = MeasurementunsignedToStr(this->GetPageRightmar()).c_str();
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
    return (m_pageHeight != data_MEASUREMENTUNSIGNED());
}

bool AttPages::HasPageWidth() const
{
    return (m_pageWidth != data_MEASUREMENTUNSIGNED());
}

bool AttPages::HasPageTopmar() const
{
    return (m_pageTopmar != data_MEASUREMENTUNSIGNED());
}

bool AttPages::HasPageBotmar() const
{
    return (m_pageBotmar != data_MEASUREMENTUNSIGNED());
}

bool AttPages::HasPageLeftmar() const
{
    return (m_pageLeftmar != data_MEASUREMENTUNSIGNED());
}

bool AttPages::HasPageRightmar() const
{
    return (m_pageRightmar != data_MEASUREMENTUNSIGNED());
}

bool AttPages::HasPagePanels() const
{
    return (m_pagePanels != "");
}

bool AttPages::HasPageScale() const
{
    return (m_pageScale != "");
}

//----------------------------------------------------------------------------
// AttPartIdent
//----------------------------------------------------------------------------

AttPartIdent::AttPartIdent() : Att()
{
    ResetPartIdent();
}

void AttPartIdent::ResetPartIdent()
{
    m_part = "";
    m_partstaff = "";
}

bool AttPartIdent::ReadPartIdent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("part")) {
        this->SetPart(StrToStr(element.attribute("part").value()));
        if (removeAttr) element.remove_attribute("part");
        hasAttribute = true;
    }
    if (element.attribute("partstaff")) {
        this->SetPartstaff(StrToStr(element.attribute("partstaff").value()));
        if (removeAttr) element.remove_attribute("partstaff");
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

//----------------------------------------------------------------------------
// AttPitch
//----------------------------------------------------------------------------

AttPitch::AttPitch() : Att()
{
    ResetPitch();
}

void AttPitch::ResetPitch()
{
    m_pname = PITCHNAME_NONE;
}

bool AttPitch::ReadPitch(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("pname")) {
        this->SetPname(StrToPitchname(element.attribute("pname").value()));
        if (removeAttr) element.remove_attribute("pname");
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

//----------------------------------------------------------------------------
// AttPlacementOnStaff
//----------------------------------------------------------------------------

AttPlacementOnStaff::AttPlacementOnStaff() : Att()
{
    ResetPlacementOnStaff();
}

void AttPlacementOnStaff::ResetPlacementOnStaff()
{
    m_onstaff = BOOLEAN_NONE;
}

bool AttPlacementOnStaff::ReadPlacementOnStaff(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("onstaff")) {
        this->SetOnstaff(StrToBoolean(element.attribute("onstaff").value()));
        if (removeAttr) element.remove_attribute("onstaff");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPlacementOnStaff::WritePlacementOnStaff(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOnstaff()) {
        element.append_attribute("onstaff") = BooleanToStr(this->GetOnstaff()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPlacementOnStaff::HasOnstaff() const
{
    return (m_onstaff != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttPlacementRelEvent
//----------------------------------------------------------------------------

AttPlacementRelEvent::AttPlacementRelEvent() : Att()
{
    ResetPlacementRelEvent();
}

void AttPlacementRelEvent::ResetPlacementRelEvent()
{
    m_place = data_STAFFREL();
}

bool AttPlacementRelEvent::ReadPlacementRelEvent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("place")) {
        this->SetPlace(StrToStaffrel(element.attribute("place").value()));
        if (removeAttr) element.remove_attribute("place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPlacementRelEvent::WritePlacementRelEvent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPlace()) {
        element.append_attribute("place") = StaffrelToStr(this->GetPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPlacementRelEvent::HasPlace() const
{
    return (m_place != data_STAFFREL());
}

//----------------------------------------------------------------------------
// AttPlacementRelStaff
//----------------------------------------------------------------------------

AttPlacementRelStaff::AttPlacementRelStaff() : Att()
{
    ResetPlacementRelStaff();
}

void AttPlacementRelStaff::ResetPlacementRelStaff()
{
    m_place = data_STAFFREL();
}

bool AttPlacementRelStaff::ReadPlacementRelStaff(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("place")) {
        this->SetPlace(StrToStaffrel(element.attribute("place").value()));
        if (removeAttr) element.remove_attribute("place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPlacementRelStaff::WritePlacementRelStaff(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPlace()) {
        element.append_attribute("place") = StaffrelToStr(this->GetPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPlacementRelStaff::HasPlace() const
{
    return (m_place != data_STAFFREL());
}

//----------------------------------------------------------------------------
// AttPlist
//----------------------------------------------------------------------------

AttPlist::AttPlist() : Att()
{
    ResetPlist();
}

void AttPlist::ResetPlist()
{
    m_plist = std::vector<std::string>();
}

bool AttPlist::ReadPlist(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("plist")) {
        this->SetPlist(StrToXsdAnyURIList(element.attribute("plist").value()));
        if (removeAttr) element.remove_attribute("plist");
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

//----------------------------------------------------------------------------
// AttPointing
//----------------------------------------------------------------------------

AttPointing::AttPointing() : Att()
{
    ResetPointing();
}

void AttPointing::ResetPointing()
{
    m_actuate = "";
    m_role = "";
    m_show = "";
    m_target = "";
    m_targettype = "";
}

bool AttPointing::ReadPointing(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("xlink:actuate")) {
        this->SetActuate(StrToStr(element.attribute("xlink:actuate").value()));
        if (removeAttr) element.remove_attribute("xlink:actuate");
        hasAttribute = true;
    }
    if (element.attribute("xlink:role")) {
        this->SetRole(StrToStr(element.attribute("xlink:role").value()));
        if (removeAttr) element.remove_attribute("xlink:role");
        hasAttribute = true;
    }
    if (element.attribute("xlink:show")) {
        this->SetShow(StrToStr(element.attribute("xlink:show").value()));
        if (removeAttr) element.remove_attribute("xlink:show");
        hasAttribute = true;
    }
    if (element.attribute("target")) {
        this->SetTarget(StrToStr(element.attribute("target").value()));
        if (removeAttr) element.remove_attribute("target");
        hasAttribute = true;
    }
    if (element.attribute("targettype")) {
        this->SetTargettype(StrToStr(element.attribute("targettype").value()));
        if (removeAttr) element.remove_attribute("targettype");
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
        element.append_attribute("target") = StrToStr(this->GetTarget()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTargettype()) {
        element.append_attribute("targettype") = StrToStr(this->GetTargettype()).c_str();
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

//----------------------------------------------------------------------------
// AttQuantity
//----------------------------------------------------------------------------

AttQuantity::AttQuantity() : Att()
{
    ResetQuantity();
}

void AttQuantity::ResetQuantity()
{
    m_quantity = 0.0;
}

bool AttQuantity::ReadQuantity(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("quantity")) {
        this->SetQuantity(StrToDbl(element.attribute("quantity").value()));
        if (removeAttr) element.remove_attribute("quantity");
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

//----------------------------------------------------------------------------
// AttRanging
//----------------------------------------------------------------------------

AttRanging::AttRanging() : Att()
{
    ResetRanging();
}

void AttRanging::ResetRanging()
{
    m_atleast = 0.0;
    m_atmost = 0.0;
    m_min = 0.0;
    m_max = 0.0;
    m_confidence = 0.0;
}

bool AttRanging::ReadRanging(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("atleast")) {
        this->SetAtleast(StrToDbl(element.attribute("atleast").value()));
        if (removeAttr) element.remove_attribute("atleast");
        hasAttribute = true;
    }
    if (element.attribute("atmost")) {
        this->SetAtmost(StrToDbl(element.attribute("atmost").value()));
        if (removeAttr) element.remove_attribute("atmost");
        hasAttribute = true;
    }
    if (element.attribute("min")) {
        this->SetMin(StrToDbl(element.attribute("min").value()));
        if (removeAttr) element.remove_attribute("min");
        hasAttribute = true;
    }
    if (element.attribute("max")) {
        this->SetMax(StrToDbl(element.attribute("max").value()));
        if (removeAttr) element.remove_attribute("max");
        hasAttribute = true;
    }
    if (element.attribute("confidence")) {
        this->SetConfidence(StrToDbl(element.attribute("confidence").value()));
        if (removeAttr) element.remove_attribute("confidence");
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

//----------------------------------------------------------------------------
// AttRepeatMarkLog
//----------------------------------------------------------------------------

AttRepeatMarkLog::AttRepeatMarkLog() : Att()
{
    ResetRepeatMarkLog();
}

void AttRepeatMarkLog::ResetRepeatMarkLog()
{
    m_func = repeatMarkLog_FUNC_NONE;
}

bool AttRepeatMarkLog::ReadRepeatMarkLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToRepeatMarkLogFunc(element.attribute("func").value()));
        if (removeAttr) element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRepeatMarkLog::WriteRepeatMarkLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = RepeatMarkLogFuncToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRepeatMarkLog::HasFunc() const
{
    return (m_func != repeatMarkLog_FUNC_NONE);
}

//----------------------------------------------------------------------------
// AttResponsibility
//----------------------------------------------------------------------------

AttResponsibility::AttResponsibility() : Att()
{
    ResetResponsibility();
}

void AttResponsibility::ResetResponsibility()
{
    m_resp = "";
}

bool AttResponsibility::ReadResponsibility(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("resp")) {
        this->SetResp(StrToStr(element.attribute("resp").value()));
        if (removeAttr) element.remove_attribute("resp");
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

//----------------------------------------------------------------------------
// AttRestdurationLog
//----------------------------------------------------------------------------

AttRestdurationLog::AttRestdurationLog() : Att()
{
    ResetRestdurationLog();
}

void AttRestdurationLog::ResetRestdurationLog()
{
    m_dur = DURATION_NONE;
}

bool AttRestdurationLog::ReadRestdurationLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dur")) {
        this->SetDur(StrToDuration(element.attribute("dur").value()));
        if (removeAttr) element.remove_attribute("dur");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRestdurationLog::WriteRestdurationLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDur()) {
        element.append_attribute("dur") = DurationToStr(this->GetDur()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRestdurationLog::HasDur() const
{
    return (m_dur != DURATION_NONE);
}

//----------------------------------------------------------------------------
// AttScalable
//----------------------------------------------------------------------------

AttScalable::AttScalable() : Att()
{
    ResetScalable();
}

void AttScalable::ResetScalable()
{
    m_scale = -1.0;
}

bool AttScalable::ReadScalable(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("scale")) {
        this->SetScale(StrToPercent(element.attribute("scale").value()));
        if (removeAttr) element.remove_attribute("scale");
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

//----------------------------------------------------------------------------
// AttSequence
//----------------------------------------------------------------------------

AttSequence::AttSequence() : Att()
{
    ResetSequence();
}

void AttSequence::ResetSequence()
{
    m_seq = MEI_UNSET;
}

bool AttSequence::ReadSequence(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("seq")) {
        this->SetSeq(StrToInt(element.attribute("seq").value()));
        if (removeAttr) element.remove_attribute("seq");
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
    return (m_seq != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttSlashCount
//----------------------------------------------------------------------------

AttSlashCount::AttSlashCount() : Att()
{
    ResetSlashCount();
}

void AttSlashCount::ResetSlashCount()
{
    m_slash = 0;
}

bool AttSlashCount::ReadSlashCount(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("slash")) {
        this->SetSlash(StrToInt(element.attribute("slash").value()));
        if (removeAttr) element.remove_attribute("slash");
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

//----------------------------------------------------------------------------
// AttSlurPresent
//----------------------------------------------------------------------------

AttSlurPresent::AttSlurPresent() : Att()
{
    ResetSlurPresent();
}

void AttSlurPresent::ResetSlurPresent()
{
    m_slur = "";
}

bool AttSlurPresent::ReadSlurPresent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("slur")) {
        this->SetSlur(StrToStr(element.attribute("slur").value()));
        if (removeAttr) element.remove_attribute("slur");
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

//----------------------------------------------------------------------------
// AttSource
//----------------------------------------------------------------------------

AttSource::AttSource() : Att()
{
    ResetSource();
}

void AttSource::ResetSource()
{
    m_source = "";
}

bool AttSource::ReadSource(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("source")) {
        this->SetSource(StrToStr(element.attribute("source").value()));
        if (removeAttr) element.remove_attribute("source");
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

//----------------------------------------------------------------------------
// AttSpacing
//----------------------------------------------------------------------------

AttSpacing::AttSpacing() : Att()
{
    ResetSpacing();
}

void AttSpacing::ResetSpacing()
{
    m_spacingPackexp = 0.0;
    m_spacingPackfact = 0.0;
    m_spacingStaff = data_MEASUREMENTSIGNED();
    m_spacingSystem = data_MEASUREMENTSIGNED();
}

bool AttSpacing::ReadSpacing(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("spacing.packexp")) {
        this->SetSpacingPackexp(StrToDbl(element.attribute("spacing.packexp").value()));
        if (removeAttr) element.remove_attribute("spacing.packexp");
        hasAttribute = true;
    }
    if (element.attribute("spacing.packfact")) {
        this->SetSpacingPackfact(StrToDbl(element.attribute("spacing.packfact").value()));
        if (removeAttr) element.remove_attribute("spacing.packfact");
        hasAttribute = true;
    }
    if (element.attribute("spacing.staff")) {
        this->SetSpacingStaff(StrToMeasurementsigned(element.attribute("spacing.staff").value()));
        if (removeAttr) element.remove_attribute("spacing.staff");
        hasAttribute = true;
    }
    if (element.attribute("spacing.system")) {
        this->SetSpacingSystem(StrToMeasurementsigned(element.attribute("spacing.system").value()));
        if (removeAttr) element.remove_attribute("spacing.system");
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
        element.append_attribute("spacing.staff") = MeasurementsignedToStr(this->GetSpacingStaff()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSpacingSystem()) {
        element.append_attribute("spacing.system") = MeasurementsignedToStr(this->GetSpacingSystem()).c_str();
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
    return (m_spacingStaff != data_MEASUREMENTSIGNED());
}

bool AttSpacing::HasSpacingSystem() const
{
    return (m_spacingSystem != data_MEASUREMENTSIGNED());
}

//----------------------------------------------------------------------------
// AttStaffLog
//----------------------------------------------------------------------------

AttStaffLog::AttStaffLog() : Att()
{
    ResetStaffLog();
}

void AttStaffLog::ResetStaffLog()
{
    m_def = "";
}

bool AttStaffLog::ReadStaffLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("def")) {
        this->SetDef(StrToStr(element.attribute("def").value()));
        if (removeAttr) element.remove_attribute("def");
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

//----------------------------------------------------------------------------
// AttStaffDefLog
//----------------------------------------------------------------------------

AttStaffDefLog::AttStaffDefLog() : Att()
{
    ResetStaffDefLog();
}

void AttStaffDefLog::ResetStaffDefLog()
{
    m_lines = MEI_UNSET;
}

bool AttStaffDefLog::ReadStaffDefLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("lines")) {
        this->SetLines(StrToInt(element.attribute("lines").value()));
        if (removeAttr) element.remove_attribute("lines");
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
    return (m_lines != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttStaffGroupingSym
//----------------------------------------------------------------------------

AttStaffGroupingSym::AttStaffGroupingSym() : Att()
{
    ResetStaffGroupingSym();
}

void AttStaffGroupingSym::ResetStaffGroupingSym()
{
    m_symbol = staffGroupingSym_SYMBOL_NONE;
}

bool AttStaffGroupingSym::ReadStaffGroupingSym(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("symbol")) {
        this->SetSymbol(StrToStaffGroupingSymSymbol(element.attribute("symbol").value()));
        if (removeAttr) element.remove_attribute("symbol");
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

//----------------------------------------------------------------------------
// AttStaffIdent
//----------------------------------------------------------------------------

AttStaffIdent::AttStaffIdent() : Att()
{
    ResetStaffIdent();
}

void AttStaffIdent::ResetStaffIdent()
{
    m_staff = std::vector<int>();
}

bool AttStaffIdent::ReadStaffIdent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("staff")) {
        this->SetStaff(StrToXsdPositiveIntegerList(element.attribute("staff").value()));
        if (removeAttr) element.remove_attribute("staff");
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

//----------------------------------------------------------------------------
// AttStaffItems
//----------------------------------------------------------------------------

AttStaffItems::AttStaffItems() : Att()
{
    ResetStaffItems();
}

void AttStaffItems::ResetStaffItems()
{
    m_aboveorder = data_STAFFITEM();
    m_beloworder = data_STAFFITEM();
    m_betweenorder = data_STAFFITEM();
}

bool AttStaffItems::ReadStaffItems(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("aboveorder")) {
        this->SetAboveorder(StrToStaffitem(element.attribute("aboveorder").value()));
        if (removeAttr) element.remove_attribute("aboveorder");
        hasAttribute = true;
    }
    if (element.attribute("beloworder")) {
        this->SetBeloworder(StrToStaffitem(element.attribute("beloworder").value()));
        if (removeAttr) element.remove_attribute("beloworder");
        hasAttribute = true;
    }
    if (element.attribute("betweenorder")) {
        this->SetBetweenorder(StrToStaffitem(element.attribute("betweenorder").value()));
        if (removeAttr) element.remove_attribute("betweenorder");
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

//----------------------------------------------------------------------------
// AttStaffLoc
//----------------------------------------------------------------------------

AttStaffLoc::AttStaffLoc() : Att()
{
    ResetStaffLoc();
}

void AttStaffLoc::ResetStaffLoc()
{
    m_loc = MEI_UNSET;
}

bool AttStaffLoc::ReadStaffLoc(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("loc")) {
        this->SetLoc(StrToInt(element.attribute("loc").value()));
        if (removeAttr) element.remove_attribute("loc");
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
    return (m_loc != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttStaffLocPitched
//----------------------------------------------------------------------------

AttStaffLocPitched::AttStaffLocPitched() : Att()
{
    ResetStaffLocPitched();
}

void AttStaffLocPitched::ResetStaffLocPitched()
{
    m_ploc = PITCHNAME_NONE;
    m_oloc = MEI_UNSET_OCT;
}

bool AttStaffLocPitched::ReadStaffLocPitched(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("ploc")) {
        this->SetPloc(StrToPitchname(element.attribute("ploc").value()));
        if (removeAttr) element.remove_attribute("ploc");
        hasAttribute = true;
    }
    if (element.attribute("oloc")) {
        this->SetOloc(StrToOctave(element.attribute("oloc").value()));
        if (removeAttr) element.remove_attribute("oloc");
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
    return (m_oloc != MEI_UNSET_OCT);
}

//----------------------------------------------------------------------------
// AttStartEndId
//----------------------------------------------------------------------------

AttStartEndId::AttStartEndId() : Att()
{
    ResetStartEndId();
}

void AttStartEndId::ResetStartEndId()
{
    m_endid = "";
}

bool AttStartEndId::ReadStartEndId(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("endid")) {
        this->SetEndid(StrToStr(element.attribute("endid").value()));
        if (removeAttr) element.remove_attribute("endid");
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

//----------------------------------------------------------------------------
// AttStartId
//----------------------------------------------------------------------------

AttStartId::AttStartId() : Att()
{
    ResetStartId();
}

void AttStartId::ResetStartId()
{
    m_startid = "";
}

bool AttStartId::ReadStartId(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("startid")) {
        this->SetStartid(StrToStr(element.attribute("startid").value()));
        if (removeAttr) element.remove_attribute("startid");
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

//----------------------------------------------------------------------------
// AttStems
//----------------------------------------------------------------------------

AttStems::AttStems() : Att()
{
    ResetStems();
}

void AttStems::ResetStems()
{
    m_stemDir = STEMDIRECTION_NONE;
    m_stemLen = -1.0;
    m_stemMod = STEMMODIFIER_NONE;
    m_stemPos = STEMPOSITION_NONE;
    m_stemSameas = "";
    m_stemVisible = BOOLEAN_NONE;
    m_stemX = 0.0;
    m_stemY = 0.0;
}

bool AttStems::ReadStems(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("stem.dir")) {
        this->SetStemDir(StrToStemdirection(element.attribute("stem.dir").value()));
        if (removeAttr) element.remove_attribute("stem.dir");
        hasAttribute = true;
    }
    if (element.attribute("stem.len")) {
        this->SetStemLen(StrToDbl(element.attribute("stem.len").value()));
        if (removeAttr) element.remove_attribute("stem.len");
        hasAttribute = true;
    }
    if (element.attribute("stem.mod")) {
        this->SetStemMod(StrToStemmodifier(element.attribute("stem.mod").value()));
        if (removeAttr) element.remove_attribute("stem.mod");
        hasAttribute = true;
    }
    if (element.attribute("stem.pos")) {
        this->SetStemPos(StrToStemposition(element.attribute("stem.pos").value()));
        if (removeAttr) element.remove_attribute("stem.pos");
        hasAttribute = true;
    }
    if (element.attribute("stem.sameas")) {
        this->SetStemSameas(StrToStr(element.attribute("stem.sameas").value()));
        if (removeAttr) element.remove_attribute("stem.sameas");
        hasAttribute = true;
    }
    if (element.attribute("stem.visible")) {
        this->SetStemVisible(StrToBoolean(element.attribute("stem.visible").value()));
        if (removeAttr) element.remove_attribute("stem.visible");
        hasAttribute = true;
    }
    if (element.attribute("stem.x")) {
        this->SetStemX(StrToDbl(element.attribute("stem.x").value()));
        if (removeAttr) element.remove_attribute("stem.x");
        hasAttribute = true;
    }
    if (element.attribute("stem.y")) {
        this->SetStemY(StrToDbl(element.attribute("stem.y").value()));
        if (removeAttr) element.remove_attribute("stem.y");
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
    if (this->HasStemSameas()) {
        element.append_attribute("stem.sameas") = StrToStr(this->GetStemSameas()).c_str();
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
    return (m_stemLen != -1.0);
}

bool AttStems::HasStemMod() const
{
    return (m_stemMod != STEMMODIFIER_NONE);
}

bool AttStems::HasStemPos() const
{
    return (m_stemPos != STEMPOSITION_NONE);
}

bool AttStems::HasStemSameas() const
{
    return (m_stemSameas != "");
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

//----------------------------------------------------------------------------
// AttSylLog
//----------------------------------------------------------------------------

AttSylLog::AttSylLog() : Att()
{
    ResetSylLog();
}

void AttSylLog::ResetSylLog()
{
    m_con = sylLog_CON_NONE;
    m_wordpos = sylLog_WORDPOS_NONE;
}

bool AttSylLog::ReadSylLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("con")) {
        this->SetCon(StrToSylLogCon(element.attribute("con").value()));
        if (removeAttr) element.remove_attribute("con");
        hasAttribute = true;
    }
    if (element.attribute("wordpos")) {
        this->SetWordpos(StrToSylLogWordpos(element.attribute("wordpos").value()));
        if (removeAttr) element.remove_attribute("wordpos");
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

//----------------------------------------------------------------------------
// AttSylText
//----------------------------------------------------------------------------

AttSylText::AttSylText() : Att()
{
    ResetSylText();
}

void AttSylText::ResetSylText()
{
    m_syl = "";
}

bool AttSylText::ReadSylText(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("syl")) {
        this->SetSyl(StrToStr(element.attribute("syl").value()));
        if (removeAttr) element.remove_attribute("syl");
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

//----------------------------------------------------------------------------
// AttSystems
//----------------------------------------------------------------------------

AttSystems::AttSystems() : Att()
{
    ResetSystems();
}

void AttSystems::ResetSystems()
{
    m_systemLeftline = BOOLEAN_NONE;
    m_systemLeftmar = data_MEASUREMENTUNSIGNED();
    m_systemRightmar = data_MEASUREMENTUNSIGNED();
    m_systemTopmar = data_MEASUREMENTUNSIGNED();
}

bool AttSystems::ReadSystems(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("system.leftline")) {
        this->SetSystemLeftline(StrToBoolean(element.attribute("system.leftline").value()));
        if (removeAttr) element.remove_attribute("system.leftline");
        hasAttribute = true;
    }
    if (element.attribute("system.leftmar")) {
        this->SetSystemLeftmar(StrToMeasurementunsigned(element.attribute("system.leftmar").value()));
        if (removeAttr) element.remove_attribute("system.leftmar");
        hasAttribute = true;
    }
    if (element.attribute("system.rightmar")) {
        this->SetSystemRightmar(StrToMeasurementunsigned(element.attribute("system.rightmar").value()));
        if (removeAttr) element.remove_attribute("system.rightmar");
        hasAttribute = true;
    }
    if (element.attribute("system.topmar")) {
        this->SetSystemTopmar(StrToMeasurementunsigned(element.attribute("system.topmar").value()));
        if (removeAttr) element.remove_attribute("system.topmar");
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
        element.append_attribute("system.leftmar") = MeasurementunsignedToStr(this->GetSystemLeftmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSystemRightmar()) {
        element.append_attribute("system.rightmar") = MeasurementunsignedToStr(this->GetSystemRightmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSystemTopmar()) {
        element.append_attribute("system.topmar") = MeasurementunsignedToStr(this->GetSystemTopmar()).c_str();
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
    return (m_systemLeftmar != data_MEASUREMENTUNSIGNED());
}

bool AttSystems::HasSystemRightmar() const
{
    return (m_systemRightmar != data_MEASUREMENTUNSIGNED());
}

bool AttSystems::HasSystemTopmar() const
{
    return (m_systemTopmar != data_MEASUREMENTUNSIGNED());
}

//----------------------------------------------------------------------------
// AttTargetEval
//----------------------------------------------------------------------------

AttTargetEval::AttTargetEval() : Att()
{
    ResetTargetEval();
}

void AttTargetEval::ResetTargetEval()
{
    m_evaluate = targetEval_EVALUATE_NONE;
}

bool AttTargetEval::ReadTargetEval(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("evaluate")) {
        this->SetEvaluate(StrToTargetEvalEvaluate(element.attribute("evaluate").value()));
        if (removeAttr) element.remove_attribute("evaluate");
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

//----------------------------------------------------------------------------
// AttTempoLog
//----------------------------------------------------------------------------

AttTempoLog::AttTempoLog() : Att()
{
    ResetTempoLog();
}

void AttTempoLog::ResetTempoLog()
{
    m_func = tempoLog_FUNC_NONE;
}

bool AttTempoLog::ReadTempoLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToTempoLogFunc(element.attribute("func").value()));
        if (removeAttr) element.remove_attribute("func");
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

//----------------------------------------------------------------------------
// AttTextRendition
//----------------------------------------------------------------------------

AttTextRendition::AttTextRendition() : Att()
{
    ResetTextRendition();
}

void AttTextRendition::ResetTextRendition()
{
    m_altrend = "";
    m_rend = TEXTRENDITION_NONE;
}

bool AttTextRendition::ReadTextRendition(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("altrend")) {
        this->SetAltrend(StrToStr(element.attribute("altrend").value()));
        if (removeAttr) element.remove_attribute("altrend");
        hasAttribute = true;
    }
    if (element.attribute("rend")) {
        this->SetRend(StrToTextrendition(element.attribute("rend").value()));
        if (removeAttr) element.remove_attribute("rend");
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

//----------------------------------------------------------------------------
// AttTextStyle
//----------------------------------------------------------------------------

AttTextStyle::AttTextStyle() : Att()
{
    ResetTextStyle();
}

void AttTextStyle::ResetTextStyle()
{
    m_textFam = "";
    m_textName = "";
    m_textSize = data_FONTSIZE();
    m_textStyle = FONTSTYLE_NONE;
    m_textWeight = FONTWEIGHT_NONE;
}

bool AttTextStyle::ReadTextStyle(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("text.fam")) {
        this->SetTextFam(StrToStr(element.attribute("text.fam").value()));
        if (removeAttr) element.remove_attribute("text.fam");
        hasAttribute = true;
    }
    if (element.attribute("text.name")) {
        this->SetTextName(StrToStr(element.attribute("text.name").value()));
        if (removeAttr) element.remove_attribute("text.name");
        hasAttribute = true;
    }
    if (element.attribute("text.size")) {
        this->SetTextSize(StrToFontsize(element.attribute("text.size").value()));
        if (removeAttr) element.remove_attribute("text.size");
        hasAttribute = true;
    }
    if (element.attribute("text.style")) {
        this->SetTextStyle(StrToFontstyle(element.attribute("text.style").value()));
        if (removeAttr) element.remove_attribute("text.style");
        hasAttribute = true;
    }
    if (element.attribute("text.weight")) {
        this->SetTextWeight(StrToFontweight(element.attribute("text.weight").value()));
        if (removeAttr) element.remove_attribute("text.weight");
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

//----------------------------------------------------------------------------
// AttTiePresent
//----------------------------------------------------------------------------

AttTiePresent::AttTiePresent() : Att()
{
    ResetTiePresent();
}

void AttTiePresent::ResetTiePresent()
{
    m_tie = TIE_NONE;
}

bool AttTiePresent::ReadTiePresent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("tie")) {
        this->SetTie(StrToTie(element.attribute("tie").value()));
        if (removeAttr) element.remove_attribute("tie");
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

//----------------------------------------------------------------------------
// AttTimestampLog
//----------------------------------------------------------------------------

AttTimestampLog::AttTimestampLog() : Att()
{
    ResetTimestampLog();
}

void AttTimestampLog::ResetTimestampLog()
{
    m_tstamp = -1.0;
}

bool AttTimestampLog::ReadTimestampLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("tstamp")) {
        this->SetTstamp(StrToDbl(element.attribute("tstamp").value()));
        if (removeAttr) element.remove_attribute("tstamp");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestampLog::WriteTimestampLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTstamp()) {
        element.append_attribute("tstamp") = DblToStr(this->GetTstamp()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimestampLog::HasTstamp() const
{
    return (m_tstamp != -1.0);
}

//----------------------------------------------------------------------------
// AttTimestamp2Log
//----------------------------------------------------------------------------

AttTimestamp2Log::AttTimestamp2Log() : Att()
{
    ResetTimestamp2Log();
}

void AttTimestamp2Log::ResetTimestamp2Log()
{
    m_tstamp2 = std::make_pair(-1, -1.0);
}

bool AttTimestamp2Log::ReadTimestamp2Log(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("tstamp2")) {
        this->SetTstamp2(StrToMeasurebeat(element.attribute("tstamp2").value()));
        if (removeAttr) element.remove_attribute("tstamp2");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTimestamp2Log::WriteTimestamp2Log(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTstamp2()) {
        element.append_attribute("tstamp2") = MeasurebeatToStr(this->GetTstamp2()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTimestamp2Log::HasTstamp2() const
{
    return (m_tstamp2 != std::make_pair(-1, -1.0));
}

//----------------------------------------------------------------------------
// AttTransposition
//----------------------------------------------------------------------------

AttTransposition::AttTransposition() : Att()
{
    ResetTransposition();
}

void AttTransposition::ResetTransposition()
{
    m_transDiat = MEI_UNSET;
    m_transSemi = MEI_UNSET;
}

bool AttTransposition::ReadTransposition(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("trans.diat")) {
        this->SetTransDiat(StrToInt(element.attribute("trans.diat").value()));
        if (removeAttr) element.remove_attribute("trans.diat");
        hasAttribute = true;
    }
    if (element.attribute("trans.semi")) {
        this->SetTransSemi(StrToInt(element.attribute("trans.semi").value()));
        if (removeAttr) element.remove_attribute("trans.semi");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTransposition::WriteTransposition(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTransDiat()) {
        element.append_attribute("trans.diat") = IntToStr(this->GetTransDiat()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTransSemi()) {
        element.append_attribute("trans.semi") = IntToStr(this->GetTransSemi()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTransposition::HasTransDiat() const
{
    return (m_transDiat != MEI_UNSET);
}

bool AttTransposition::HasTransSemi() const
{
    return (m_transSemi != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttTuning
//----------------------------------------------------------------------------

AttTuning::AttTuning() : Att()
{
    ResetTuning();
}

void AttTuning::ResetTuning()
{
    m_tuneHz = 0.0;
    m_tunePname = PITCHNAME_NONE;
    m_tuneTemper = TEMPERAMENT_NONE;
}

bool AttTuning::ReadTuning(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("tune.Hz")) {
        this->SetTuneHz(StrToDbl(element.attribute("tune.Hz").value()));
        if (removeAttr) element.remove_attribute("tune.Hz");
        hasAttribute = true;
    }
    if (element.attribute("tune.pname")) {
        this->SetTunePname(StrToPitchname(element.attribute("tune.pname").value()));
        if (removeAttr) element.remove_attribute("tune.pname");
        hasAttribute = true;
    }
    if (element.attribute("tune.temper")) {
        this->SetTuneTemper(StrToTemperament(element.attribute("tune.temper").value()));
        if (removeAttr) element.remove_attribute("tune.temper");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTuning::WriteTuning(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTuneHz()) {
        element.append_attribute("tune.Hz") = DblToStr(this->GetTuneHz()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTunePname()) {
        element.append_attribute("tune.pname") = PitchnameToStr(this->GetTunePname()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTuneTemper()) {
        element.append_attribute("tune.temper") = TemperamentToStr(this->GetTuneTemper()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTuning::HasTuneHz() const
{
    return (m_tuneHz != 0.0);
}

bool AttTuning::HasTunePname() const
{
    return (m_tunePname != PITCHNAME_NONE);
}

bool AttTuning::HasTuneTemper() const
{
    return (m_tuneTemper != TEMPERAMENT_NONE);
}

//----------------------------------------------------------------------------
// AttTupletPresent
//----------------------------------------------------------------------------

AttTupletPresent::AttTupletPresent() : Att()
{
    ResetTupletPresent();
}

void AttTupletPresent::ResetTupletPresent()
{
    m_tuplet = "";
}

bool AttTupletPresent::ReadTupletPresent(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("tuplet")) {
        this->SetTuplet(StrToStr(element.attribute("tuplet").value()));
        if (removeAttr) element.remove_attribute("tuplet");
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

//----------------------------------------------------------------------------
// AttTyped
//----------------------------------------------------------------------------

AttTyped::AttTyped() : Att()
{
    ResetTyped();
}

void AttTyped::ResetTyped()
{
    m_type = "";
}

bool AttTyped::ReadTyped(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("type")) {
        this->SetType(StrToStr(element.attribute("type").value()));
        if (removeAttr) element.remove_attribute("type");
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

//----------------------------------------------------------------------------
// AttTypography
//----------------------------------------------------------------------------

AttTypography::AttTypography() : Att()
{
    ResetTypography();
}

void AttTypography::ResetTypography()
{
    m_fontfam = "";
    m_fontname = "";
    m_fontsize = data_FONTSIZE();
    m_fontstyle = FONTSTYLE_NONE;
    m_fontweight = FONTWEIGHT_NONE;
    m_letterspacing = 0.0;
    m_lineheight = "";
}

bool AttTypography::ReadTypography(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("fontfam")) {
        this->SetFontfam(StrToStr(element.attribute("fontfam").value()));
        if (removeAttr) element.remove_attribute("fontfam");
        hasAttribute = true;
    }
    if (element.attribute("fontname")) {
        this->SetFontname(StrToStr(element.attribute("fontname").value()));
        if (removeAttr) element.remove_attribute("fontname");
        hasAttribute = true;
    }
    if (element.attribute("fontsize")) {
        this->SetFontsize(StrToFontsize(element.attribute("fontsize").value()));
        if (removeAttr) element.remove_attribute("fontsize");
        hasAttribute = true;
    }
    if (element.attribute("fontstyle")) {
        this->SetFontstyle(StrToFontstyle(element.attribute("fontstyle").value()));
        if (removeAttr) element.remove_attribute("fontstyle");
        hasAttribute = true;
    }
    if (element.attribute("fontweight")) {
        this->SetFontweight(StrToFontweight(element.attribute("fontweight").value()));
        if (removeAttr) element.remove_attribute("fontweight");
        hasAttribute = true;
    }
    if (element.attribute("letterspacing")) {
        this->SetLetterspacing(StrToDbl(element.attribute("letterspacing").value()));
        if (removeAttr) element.remove_attribute("letterspacing");
        hasAttribute = true;
    }
    if (element.attribute("lineheight")) {
        this->SetLineheight(StrToStr(element.attribute("lineheight").value()));
        if (removeAttr) element.remove_attribute("lineheight");
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
    if (this->HasLetterspacing()) {
        element.append_attribute("letterspacing") = DblToStr(this->GetLetterspacing()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLineheight()) {
        element.append_attribute("lineheight") = StrToStr(this->GetLineheight()).c_str();
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

bool AttTypography::HasLetterspacing() const
{
    return (m_letterspacing != 0.0);
}

bool AttTypography::HasLineheight() const
{
    return (m_lineheight != "");
}

//----------------------------------------------------------------------------
// AttVerticalAlign
//----------------------------------------------------------------------------

AttVerticalAlign::AttVerticalAlign() : Att()
{
    ResetVerticalAlign();
}

void AttVerticalAlign::ResetVerticalAlign()
{
    m_valign = VERTICALALIGNMENT_NONE;
}

bool AttVerticalAlign::ReadVerticalAlign(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("valign")) {
        this->SetValign(StrToVerticalalignment(element.attribute("valign").value()));
        if (removeAttr) element.remove_attribute("valign");
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

//----------------------------------------------------------------------------
// AttVerticalGroup
//----------------------------------------------------------------------------

AttVerticalGroup::AttVerticalGroup() : Att()
{
    ResetVerticalGroup();
}

void AttVerticalGroup::ResetVerticalGroup()
{
    m_vgrp = MEI_UNSET;
}

bool AttVerticalGroup::ReadVerticalGroup(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("vgrp")) {
        this->SetVgrp(StrToInt(element.attribute("vgrp").value()));
        if (removeAttr) element.remove_attribute("vgrp");
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
    return (m_vgrp != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttVisibility
//----------------------------------------------------------------------------

AttVisibility::AttVisibility() : Att()
{
    ResetVisibility();
}

void AttVisibility::ResetVisibility()
{
    m_visible = BOOLEAN_NONE;
}

bool AttVisibility::ReadVisibility(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("visible")) {
        this->SetVisible(StrToBoolean(element.attribute("visible").value()));
        if (removeAttr) element.remove_attribute("visible");
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

//----------------------------------------------------------------------------
// AttVisualOffsetHo
//----------------------------------------------------------------------------

AttVisualOffsetHo::AttVisualOffsetHo() : Att()
{
    ResetVisualOffsetHo();
}

void AttVisualOffsetHo::ResetVisualOffsetHo()
{
    m_ho = data_MEASUREMENTSIGNED();
}

bool AttVisualOffsetHo::ReadVisualOffsetHo(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("ho")) {
        this->SetHo(StrToMeasurementsigned(element.attribute("ho").value()));
        if (removeAttr) element.remove_attribute("ho");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualOffsetHo::WriteVisualOffsetHo(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasHo()) {
        element.append_attribute("ho") = MeasurementsignedToStr(this->GetHo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualOffsetHo::HasHo() const
{
    return (m_ho != data_MEASUREMENTSIGNED());
}

//----------------------------------------------------------------------------
// AttVisualOffsetTo
//----------------------------------------------------------------------------

AttVisualOffsetTo::AttVisualOffsetTo() : Att()
{
    ResetVisualOffsetTo();
}

void AttVisualOffsetTo::ResetVisualOffsetTo()
{
    m_to = 0.0;
}

bool AttVisualOffsetTo::ReadVisualOffsetTo(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("to")) {
        this->SetTo(StrToDbl(element.attribute("to").value()));
        if (removeAttr) element.remove_attribute("to");
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

//----------------------------------------------------------------------------
// AttVisualOffsetVo
//----------------------------------------------------------------------------

AttVisualOffsetVo::AttVisualOffsetVo() : Att()
{
    ResetVisualOffsetVo();
}

void AttVisualOffsetVo::ResetVisualOffsetVo()
{
    m_vo = data_MEASUREMENTSIGNED();
}

bool AttVisualOffsetVo::ReadVisualOffsetVo(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("vo")) {
        this->SetVo(StrToMeasurementsigned(element.attribute("vo").value()));
        if (removeAttr) element.remove_attribute("vo");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualOffsetVo::WriteVisualOffsetVo(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVo()) {
        element.append_attribute("vo") = MeasurementsignedToStr(this->GetVo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualOffsetVo::HasVo() const
{
    return (m_vo != data_MEASUREMENTSIGNED());
}

//----------------------------------------------------------------------------
// AttVisualOffset2Ho
//----------------------------------------------------------------------------

AttVisualOffset2Ho::AttVisualOffset2Ho() : Att()
{
    ResetVisualOffset2Ho();
}

void AttVisualOffset2Ho::ResetVisualOffset2Ho()
{
    m_startho = data_MEASUREMENTSIGNED();
    m_endho = data_MEASUREMENTSIGNED();
}

bool AttVisualOffset2Ho::ReadVisualOffset2Ho(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("startho")) {
        this->SetStartho(StrToMeasurementsigned(element.attribute("startho").value()));
        if (removeAttr) element.remove_attribute("startho");
        hasAttribute = true;
    }
    if (element.attribute("endho")) {
        this->SetEndho(StrToMeasurementsigned(element.attribute("endho").value()));
        if (removeAttr) element.remove_attribute("endho");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualOffset2Ho::WriteVisualOffset2Ho(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasStartho()) {
        element.append_attribute("startho") = MeasurementsignedToStr(this->GetStartho()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndho()) {
        element.append_attribute("endho") = MeasurementsignedToStr(this->GetEndho()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualOffset2Ho::HasStartho() const
{
    return (m_startho != data_MEASUREMENTSIGNED());
}

bool AttVisualOffset2Ho::HasEndho() const
{
    return (m_endho != data_MEASUREMENTSIGNED());
}

//----------------------------------------------------------------------------
// AttVisualOffset2To
//----------------------------------------------------------------------------

AttVisualOffset2To::AttVisualOffset2To() : Att()
{
    ResetVisualOffset2To();
}

void AttVisualOffset2To::ResetVisualOffset2To()
{
    m_startto = 0.0;
    m_endto = 0.0;
}

bool AttVisualOffset2To::ReadVisualOffset2To(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("startto")) {
        this->SetStartto(StrToDbl(element.attribute("startto").value()));
        if (removeAttr) element.remove_attribute("startto");
        hasAttribute = true;
    }
    if (element.attribute("endto")) {
        this->SetEndto(StrToDbl(element.attribute("endto").value()));
        if (removeAttr) element.remove_attribute("endto");
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

//----------------------------------------------------------------------------
// AttVisualOffset2Vo
//----------------------------------------------------------------------------

AttVisualOffset2Vo::AttVisualOffset2Vo() : Att()
{
    ResetVisualOffset2Vo();
}

void AttVisualOffset2Vo::ResetVisualOffset2Vo()
{
    m_startvo = data_MEASUREMENTSIGNED();
    m_endvo = data_MEASUREMENTSIGNED();
}

bool AttVisualOffset2Vo::ReadVisualOffset2Vo(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("startvo")) {
        this->SetStartvo(StrToMeasurementsigned(element.attribute("startvo").value()));
        if (removeAttr) element.remove_attribute("startvo");
        hasAttribute = true;
    }
    if (element.attribute("endvo")) {
        this->SetEndvo(StrToMeasurementsigned(element.attribute("endvo").value()));
        if (removeAttr) element.remove_attribute("endvo");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttVisualOffset2Vo::WriteVisualOffset2Vo(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasStartvo()) {
        element.append_attribute("startvo") = MeasurementsignedToStr(this->GetStartvo()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndvo()) {
        element.append_attribute("endvo") = MeasurementsignedToStr(this->GetEndvo()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttVisualOffset2Vo::HasStartvo() const
{
    return (m_startvo != data_MEASUREMENTSIGNED());
}

bool AttVisualOffset2Vo::HasEndvo() const
{
    return (m_endvo != data_MEASUREMENTSIGNED());
}

//----------------------------------------------------------------------------
// AttVoltaGroupingSym
//----------------------------------------------------------------------------

AttVoltaGroupingSym::AttVoltaGroupingSym() : Att()
{
    ResetVoltaGroupingSym();
}

void AttVoltaGroupingSym::ResetVoltaGroupingSym()
{
    m_voltasym = voltaGroupingSym_VOLTASYM_NONE;
}

bool AttVoltaGroupingSym::ReadVoltaGroupingSym(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("voltasym")) {
        this->SetVoltasym(StrToVoltaGroupingSymVoltasym(element.attribute("voltasym").value()));
        if (removeAttr) element.remove_attribute("voltasym");
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

//----------------------------------------------------------------------------
// AttWhitespace
//----------------------------------------------------------------------------

AttWhitespace::AttWhitespace() : Att()
{
    ResetWhitespace();
}

void AttWhitespace::ResetWhitespace()
{
    m_space = "";
}

bool AttWhitespace::ReadWhitespace(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("xml:space")) {
        this->SetSpace(StrToStr(element.attribute("xml:space").value()));
        if (removeAttr) element.remove_attribute("xml:space");
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

//----------------------------------------------------------------------------
// AttWidth
//----------------------------------------------------------------------------

AttWidth::AttWidth() : Att()
{
    ResetWidth();
}

void AttWidth::ResetWidth()
{
    m_width = data_MEASUREMENTUNSIGNED();
}

bool AttWidth::ReadWidth(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("width")) {
        this->SetWidth(StrToMeasurementunsigned(element.attribute("width").value()));
        if (removeAttr) element.remove_attribute("width");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttWidth::WriteWidth(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasWidth()) {
        element.append_attribute("width") = MeasurementunsignedToStr(this->GetWidth()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttWidth::HasWidth() const
{
    return (m_width != data_MEASUREMENTUNSIGNED());
}

//----------------------------------------------------------------------------
// AttXy
//----------------------------------------------------------------------------

AttXy::AttXy() : Att()
{
    ResetXy();
}

void AttXy::ResetXy()
{
    m_x = 0.0;
    m_y = 0.0;
}

bool AttXy::ReadXy(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("x")) {
        this->SetX(StrToDbl(element.attribute("x").value()));
        if (removeAttr) element.remove_attribute("x");
        hasAttribute = true;
    }
    if (element.attribute("y")) {
        this->SetY(StrToDbl(element.attribute("y").value()));
        if (removeAttr) element.remove_attribute("y");
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

//----------------------------------------------------------------------------
// AttXy2
//----------------------------------------------------------------------------

AttXy2::AttXy2() : Att()
{
    ResetXy2();
}

void AttXy2::ResetXy2()
{
    m_x2 = 0.0;
    m_y2 = 0.0;
}

bool AttXy2::ReadXy2(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("x2")) {
        this->SetX2(StrToDbl(element.attribute("x2").value()));
        if (removeAttr) element.remove_attribute("x2");
        hasAttribute = true;
    }
    if (element.attribute("y2")) {
        this->SetY2(StrToDbl(element.attribute("y2").value()));
        if (removeAttr) element.remove_attribute("y2");
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

} // namespace vrv
