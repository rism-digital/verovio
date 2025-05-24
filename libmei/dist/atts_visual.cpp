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

#include "atts_visual.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttAnnotVis
//----------------------------------------------------------------------------

AttAnnotVis::AttAnnotVis() : Att()
{
    this->ResetAnnotVis();
}

void AttAnnotVis::ResetAnnotVis()
{
    m_place = data_PLACEMENT();
}

bool AttAnnotVis::ReadAnnotVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("place")) {
        this->SetPlace(StrToPlacement(element.attribute("place").value()));
        if (removeAttr) element.remove_attribute("place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAnnotVis::WriteAnnotVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPlace()) {
        element.append_attribute("place") = PlacementToStr(this->GetPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAnnotVis::HasPlace() const
{
    return (m_place.HasValue());
}

//----------------------------------------------------------------------------
// AttArpegVis
//----------------------------------------------------------------------------

AttArpegVis::AttArpegVis() : Att()
{
    this->ResetArpegVis();
}

void AttArpegVis::ResetArpegVis()
{
    m_arrow = BOOLEAN_NONE;
    m_arrowShape = LINESTARTENDSYMBOL_NONE;
    m_arrowSize = MEI_UNSET;
    m_arrowColor = "";
    m_arrowFillcolor = "";
}

bool AttArpegVis::ReadArpegVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("arrow")) {
        this->SetArrow(StrToBoolean(element.attribute("arrow").value()));
        if (removeAttr) element.remove_attribute("arrow");
        hasAttribute = true;
    }
    if (element.attribute("arrow.shape")) {
        this->SetArrowShape(StrToLinestartendsymbol(element.attribute("arrow.shape").value()));
        if (removeAttr) element.remove_attribute("arrow.shape");
        hasAttribute = true;
    }
    if (element.attribute("arrow.size")) {
        this->SetArrowSize(StrToInt(element.attribute("arrow.size").value()));
        if (removeAttr) element.remove_attribute("arrow.size");
        hasAttribute = true;
    }
    if (element.attribute("arrow.color")) {
        this->SetArrowColor(StrToStr(element.attribute("arrow.color").value()));
        if (removeAttr) element.remove_attribute("arrow.color");
        hasAttribute = true;
    }
    if (element.attribute("arrow.fillcolor")) {
        this->SetArrowFillcolor(StrToStr(element.attribute("arrow.fillcolor").value()));
        if (removeAttr) element.remove_attribute("arrow.fillcolor");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttArpegVis::WriteArpegVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasArrow()) {
        element.append_attribute("arrow") = BooleanToStr(this->GetArrow()).c_str();
        wroteAttribute = true;
    }
    if (this->HasArrowShape()) {
        element.append_attribute("arrow.shape") = LinestartendsymbolToStr(this->GetArrowShape()).c_str();
        wroteAttribute = true;
    }
    if (this->HasArrowSize()) {
        element.append_attribute("arrow.size") = IntToStr(this->GetArrowSize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasArrowColor()) {
        element.append_attribute("arrow.color") = StrToStr(this->GetArrowColor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasArrowFillcolor()) {
        element.append_attribute("arrow.fillcolor") = StrToStr(this->GetArrowFillcolor()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttArpegVis::HasArrow() const
{
    return (m_arrow != BOOLEAN_NONE);
}

bool AttArpegVis::HasArrowShape() const
{
    return (m_arrowShape != LINESTARTENDSYMBOL_NONE);
}

bool AttArpegVis::HasArrowSize() const
{
    return (m_arrowSize != MEI_UNSET);
}

bool AttArpegVis::HasArrowColor() const
{
    return (m_arrowColor != "");
}

bool AttArpegVis::HasArrowFillcolor() const
{
    return (m_arrowFillcolor != "");
}

//----------------------------------------------------------------------------
// AttBarLineVis
//----------------------------------------------------------------------------

AttBarLineVis::AttBarLineVis() : Att()
{
    this->ResetBarLineVis();
}

void AttBarLineVis::ResetBarLineVis()
{
    m_len = 0.0;
    m_method = BARMETHOD_NONE;
    m_place = MEI_UNSET;
}

bool AttBarLineVis::ReadBarLineVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("len")) {
        this->SetLen(StrToDbl(element.attribute("len").value()));
        if (removeAttr) element.remove_attribute("len");
        hasAttribute = true;
    }
    if (element.attribute("method")) {
        this->SetMethod(StrToBarmethod(element.attribute("method").value()));
        if (removeAttr) element.remove_attribute("method");
        hasAttribute = true;
    }
    if (element.attribute("place")) {
        this->SetPlace(StrToInt(element.attribute("place").value()));
        if (removeAttr) element.remove_attribute("place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBarLineVis::WriteBarLineVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLen()) {
        element.append_attribute("len") = DblToStr(this->GetLen()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMethod()) {
        element.append_attribute("method") = BarmethodToStr(this->GetMethod()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPlace()) {
        element.append_attribute("place") = IntToStr(this->GetPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBarLineVis::HasLen() const
{
    return (m_len != 0.0);
}

bool AttBarLineVis::HasMethod() const
{
    return (m_method != BARMETHOD_NONE);
}

bool AttBarLineVis::HasPlace() const
{
    return (m_place != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttBeamingVis
//----------------------------------------------------------------------------

AttBeamingVis::AttBeamingVis() : Att()
{
    this->ResetBeamingVis();
}

void AttBeamingVis::ResetBeamingVis()
{
    m_beamColor = "";
    m_beamRend = beamingVis_BEAMREND_NONE;
    m_beamSlope = 0.0;
}

bool AttBeamingVis::ReadBeamingVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("beam.color")) {
        this->SetBeamColor(StrToStr(element.attribute("beam.color").value()));
        if (removeAttr) element.remove_attribute("beam.color");
        hasAttribute = true;
    }
    if (element.attribute("beam.rend")) {
        this->SetBeamRend(StrToBeamingVisBeamrend(element.attribute("beam.rend").value()));
        if (removeAttr) element.remove_attribute("beam.rend");
        hasAttribute = true;
    }
    if (element.attribute("beam.slope")) {
        this->SetBeamSlope(StrToDbl(element.attribute("beam.slope").value()));
        if (removeAttr) element.remove_attribute("beam.slope");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeamingVis::WriteBeamingVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBeamColor()) {
        element.append_attribute("beam.color") = StrToStr(this->GetBeamColor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBeamRend()) {
        element.append_attribute("beam.rend") = BeamingVisBeamrendToStr(this->GetBeamRend()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBeamSlope()) {
        element.append_attribute("beam.slope") = DblToStr(this->GetBeamSlope()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeamingVis::HasBeamColor() const
{
    return (m_beamColor != "");
}

bool AttBeamingVis::HasBeamRend() const
{
    return (m_beamRend != beamingVis_BEAMREND_NONE);
}

bool AttBeamingVis::HasBeamSlope() const
{
    return (m_beamSlope != 0.0);
}

//----------------------------------------------------------------------------
// AttBeatRptVis
//----------------------------------------------------------------------------

AttBeatRptVis::AttBeatRptVis() : Att()
{
    this->ResetBeatRptVis();
}

void AttBeatRptVis::ResetBeatRptVis()
{
    m_slash = BEATRPT_REND_NONE;
}

bool AttBeatRptVis::ReadBeatRptVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("slash")) {
        this->SetSlash(StrToBeatrptRend(element.attribute("slash").value()));
        if (removeAttr) element.remove_attribute("slash");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttBeatRptVis::WriteBeatRptVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSlash()) {
        element.append_attribute("slash") = BeatrptRendToStr(this->GetSlash()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttBeatRptVis::HasSlash() const
{
    return (m_slash != BEATRPT_REND_NONE);
}

//----------------------------------------------------------------------------
// AttChordVis
//----------------------------------------------------------------------------

AttChordVis::AttChordVis() : Att()
{
    this->ResetChordVis();
}

void AttChordVis::ResetChordVis()
{
    m_cluster = CLUSTER_NONE;
}

bool AttChordVis::ReadChordVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("cluster")) {
        this->SetCluster(StrToCluster(element.attribute("cluster").value()));
        if (removeAttr) element.remove_attribute("cluster");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttChordVis::WriteChordVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCluster()) {
        element.append_attribute("cluster") = ClusterToStr(this->GetCluster()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttChordVis::HasCluster() const
{
    return (m_cluster != CLUSTER_NONE);
}

//----------------------------------------------------------------------------
// AttCleffingVis
//----------------------------------------------------------------------------

AttCleffingVis::AttCleffingVis() : Att()
{
    this->ResetCleffingVis();
}

void AttCleffingVis::ResetCleffingVis()
{
    m_clefColor = "";
    m_clefVisible = BOOLEAN_NONE;
}

bool AttCleffingVis::ReadCleffingVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("clef.color")) {
        this->SetClefColor(StrToStr(element.attribute("clef.color").value()));
        if (removeAttr) element.remove_attribute("clef.color");
        hasAttribute = true;
    }
    if (element.attribute("clef.visible")) {
        this->SetClefVisible(StrToBoolean(element.attribute("clef.visible").value()));
        if (removeAttr) element.remove_attribute("clef.visible");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCleffingVis::WriteCleffingVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasClefColor()) {
        element.append_attribute("clef.color") = StrToStr(this->GetClefColor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasClefVisible()) {
        element.append_attribute("clef.visible") = BooleanToStr(this->GetClefVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCleffingVis::HasClefColor() const
{
    return (m_clefColor != "");
}

bool AttCleffingVis::HasClefVisible() const
{
    return (m_clefVisible != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttCurvatureDirection
//----------------------------------------------------------------------------

AttCurvatureDirection::AttCurvatureDirection() : Att()
{
    this->ResetCurvatureDirection();
}

void AttCurvatureDirection::ResetCurvatureDirection()
{
    m_curve = curvatureDirection_CURVE_NONE;
}

bool AttCurvatureDirection::ReadCurvatureDirection(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("curve")) {
        this->SetCurve(StrToCurvatureDirectionCurve(element.attribute("curve").value()));
        if (removeAttr) element.remove_attribute("curve");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCurvatureDirection::WriteCurvatureDirection(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCurve()) {
        element.append_attribute("curve") = CurvatureDirectionCurveToStr(this->GetCurve()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCurvatureDirection::HasCurve() const
{
    return (m_curve != curvatureDirection_CURVE_NONE);
}

//----------------------------------------------------------------------------
// AttEpisemaVis
//----------------------------------------------------------------------------

AttEpisemaVis::AttEpisemaVis() : Att()
{
    this->ResetEpisemaVis();
}

void AttEpisemaVis::ResetEpisemaVis()
{
    m_form = episemaVis_FORM_NONE;
    m_place = data_EVENTREL();
}

bool AttEpisemaVis::ReadEpisemaVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToEpisemaVisForm(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("place")) {
        this->SetPlace(StrToEventrel(element.attribute("place").value()));
        if (removeAttr) element.remove_attribute("place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttEpisemaVis::WriteEpisemaVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = EpisemaVisFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasPlace()) {
        element.append_attribute("place") = EventrelToStr(this->GetPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttEpisemaVis::HasForm() const
{
    return (m_form != episemaVis_FORM_NONE);
}

bool AttEpisemaVis::HasPlace() const
{
    return (m_place != data_EVENTREL());
}

//----------------------------------------------------------------------------
// AttFTremVis
//----------------------------------------------------------------------------

AttFTremVis::AttFTremVis() : Att()
{
    this->ResetFTremVis();
}

void AttFTremVis::ResetFTremVis()
{
    m_beams = MEI_UNSET;
    m_beamsFloat = MEI_UNSET;
    m_floatGap = data_MEASUREMENTUNSIGNED();
}

bool AttFTremVis::ReadFTremVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("beams")) {
        this->SetBeams(StrToInt(element.attribute("beams").value()));
        if (removeAttr) element.remove_attribute("beams");
        hasAttribute = true;
    }
    if (element.attribute("beams.float")) {
        this->SetBeamsFloat(StrToInt(element.attribute("beams.float").value()));
        if (removeAttr) element.remove_attribute("beams.float");
        hasAttribute = true;
    }
    if (element.attribute("float.gap")) {
        this->SetFloatGap(StrToMeasurementunsigned(element.attribute("float.gap").value()));
        if (removeAttr) element.remove_attribute("float.gap");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFTremVis::WriteFTremVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBeams()) {
        element.append_attribute("beams") = IntToStr(this->GetBeams()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBeamsFloat()) {
        element.append_attribute("beams.float") = IntToStr(this->GetBeamsFloat()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFloatGap()) {
        element.append_attribute("float.gap") = MeasurementunsignedToStr(this->GetFloatGap()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFTremVis::HasBeams() const
{
    return (m_beams != MEI_UNSET);
}

bool AttFTremVis::HasBeamsFloat() const
{
    return (m_beamsFloat != MEI_UNSET);
}

bool AttFTremVis::HasFloatGap() const
{
    return (m_floatGap != data_MEASUREMENTUNSIGNED());
}

//----------------------------------------------------------------------------
// AttFermataVis
//----------------------------------------------------------------------------

AttFermataVis::AttFermataVis() : Att()
{
    this->ResetFermataVis();
}

void AttFermataVis::ResetFermataVis()
{
    m_form = fermataVis_FORM_NONE;
    m_shape = fermataVis_SHAPE_NONE;
}

bool AttFermataVis::ReadFermataVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToFermataVisForm(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("shape")) {
        this->SetShape(StrToFermataVisShape(element.attribute("shape").value()));
        if (removeAttr) element.remove_attribute("shape");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFermataVis::WriteFermataVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = FermataVisFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasShape()) {
        element.append_attribute("shape") = FermataVisShapeToStr(this->GetShape()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFermataVis::HasForm() const
{
    return (m_form != fermataVis_FORM_NONE);
}

bool AttFermataVis::HasShape() const
{
    return (m_shape != fermataVis_SHAPE_NONE);
}

//----------------------------------------------------------------------------
// AttFingGrpVis
//----------------------------------------------------------------------------

AttFingGrpVis::AttFingGrpVis() : Att()
{
    this->ResetFingGrpVis();
}

void AttFingGrpVis::ResetFingGrpVis()
{
    m_orient = fingGrpVis_ORIENT_NONE;
}

bool AttFingGrpVis::ReadFingGrpVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("orient")) {
        this->SetOrient(StrToFingGrpVisOrient(element.attribute("orient").value()));
        if (removeAttr) element.remove_attribute("orient");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFingGrpVis::WriteFingGrpVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOrient()) {
        element.append_attribute("orient") = FingGrpVisOrientToStr(this->GetOrient()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFingGrpVis::HasOrient() const
{
    return (m_orient != fingGrpVis_ORIENT_NONE);
}

//----------------------------------------------------------------------------
// AttGuitarGridVis
//----------------------------------------------------------------------------

AttGuitarGridVis::AttGuitarGridVis() : Att()
{
    this->ResetGuitarGridVis();
}

void AttGuitarGridVis::ResetGuitarGridVis()
{
    m_gridShow = BOOLEAN_NONE;
}

bool AttGuitarGridVis::ReadGuitarGridVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("grid.show")) {
        this->SetGridShow(StrToBoolean(element.attribute("grid.show").value()));
        if (removeAttr) element.remove_attribute("grid.show");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttGuitarGridVis::WriteGuitarGridVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasGridShow()) {
        element.append_attribute("grid.show") = BooleanToStr(this->GetGridShow()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttGuitarGridVis::HasGridShow() const
{
    return (m_gridShow != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttHairpinVis
//----------------------------------------------------------------------------

AttHairpinVis::AttHairpinVis() : Att()
{
    this->ResetHairpinVis();
}

void AttHairpinVis::ResetHairpinVis()
{
    m_opening = data_MEASUREMENTUNSIGNED();
    m_closed = BOOLEAN_NONE;
    m_openingVertical = BOOLEAN_NONE;
    m_angleOptimize = BOOLEAN_NONE;
}

bool AttHairpinVis::ReadHairpinVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("opening")) {
        this->SetOpening(StrToMeasurementunsigned(element.attribute("opening").value()));
        if (removeAttr) element.remove_attribute("opening");
        hasAttribute = true;
    }
    if (element.attribute("closed")) {
        this->SetClosed(StrToBoolean(element.attribute("closed").value()));
        if (removeAttr) element.remove_attribute("closed");
        hasAttribute = true;
    }
    if (element.attribute("opening.vertical")) {
        this->SetOpeningVertical(StrToBoolean(element.attribute("opening.vertical").value()));
        if (removeAttr) element.remove_attribute("opening.vertical");
        hasAttribute = true;
    }
    if (element.attribute("angle.optimize")) {
        this->SetAngleOptimize(StrToBoolean(element.attribute("angle.optimize").value()));
        if (removeAttr) element.remove_attribute("angle.optimize");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHairpinVis::WriteHairpinVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOpening()) {
        element.append_attribute("opening") = MeasurementunsignedToStr(this->GetOpening()).c_str();
        wroteAttribute = true;
    }
    if (this->HasClosed()) {
        element.append_attribute("closed") = BooleanToStr(this->GetClosed()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOpeningVertical()) {
        element.append_attribute("opening.vertical") = BooleanToStr(this->GetOpeningVertical()).c_str();
        wroteAttribute = true;
    }
    if (this->HasAngleOptimize()) {
        element.append_attribute("angle.optimize") = BooleanToStr(this->GetAngleOptimize()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHairpinVis::HasOpening() const
{
    return (m_opening != data_MEASUREMENTUNSIGNED());
}

bool AttHairpinVis::HasClosed() const
{
    return (m_closed != BOOLEAN_NONE);
}

bool AttHairpinVis::HasOpeningVertical() const
{
    return (m_openingVertical != BOOLEAN_NONE);
}

bool AttHairpinVis::HasAngleOptimize() const
{
    return (m_angleOptimize != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttHarmVis
//----------------------------------------------------------------------------

AttHarmVis::AttHarmVis() : Att()
{
    this->ResetHarmVis();
}

void AttHarmVis::ResetHarmVis()
{
    m_rendgrid = harmVis_RENDGRID_NONE;
}

bool AttHarmVis::ReadHarmVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("rendgrid")) {
        this->SetRendgrid(StrToHarmVisRendgrid(element.attribute("rendgrid").value()));
        if (removeAttr) element.remove_attribute("rendgrid");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarmVis::WriteHarmVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasRendgrid()) {
        element.append_attribute("rendgrid") = HarmVisRendgridToStr(this->GetRendgrid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarmVis::HasRendgrid() const
{
    return (m_rendgrid != harmVis_RENDGRID_NONE);
}

//----------------------------------------------------------------------------
// AttHispanTickVis
//----------------------------------------------------------------------------

AttHispanTickVis::AttHispanTickVis() : Att()
{
    this->ResetHispanTickVis();
}

void AttHispanTickVis::ResetHispanTickVis()
{
    m_place = data_EVENTREL();
    m_tilt = data_COMPASSDIRECTION();
}

bool AttHispanTickVis::ReadHispanTickVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("place")) {
        this->SetPlace(StrToEventrel(element.attribute("place").value()));
        if (removeAttr) element.remove_attribute("place");
        hasAttribute = true;
    }
    if (element.attribute("tilt")) {
        this->SetTilt(StrToCompassdirection(element.attribute("tilt").value()));
        if (removeAttr) element.remove_attribute("tilt");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHispanTickVis::WriteHispanTickVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPlace()) {
        element.append_attribute("place") = EventrelToStr(this->GetPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTilt()) {
        element.append_attribute("tilt") = CompassdirectionToStr(this->GetTilt()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHispanTickVis::HasPlace() const
{
    return (m_place != data_EVENTREL());
}

bool AttHispanTickVis::HasTilt() const
{
    return (m_tilt != data_COMPASSDIRECTION());
}

//----------------------------------------------------------------------------
// AttKeySigVis
//----------------------------------------------------------------------------

AttKeySigVis::AttKeySigVis() : Att()
{
    this->ResetKeySigVis();
}

void AttKeySigVis::ResetKeySigVis()
{
    m_cancelaccid = CANCELACCID_NONE;
}

bool AttKeySigVis::ReadKeySigVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("cancelaccid")) {
        this->SetCancelaccid(StrToCancelaccid(element.attribute("cancelaccid").value()));
        if (removeAttr) element.remove_attribute("cancelaccid");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigVis::WriteKeySigVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCancelaccid()) {
        element.append_attribute("cancelaccid") = CancelaccidToStr(this->GetCancelaccid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigVis::HasCancelaccid() const
{
    return (m_cancelaccid != CANCELACCID_NONE);
}

//----------------------------------------------------------------------------
// AttKeySigDefaultVis
//----------------------------------------------------------------------------

AttKeySigDefaultVis::AttKeySigDefaultVis() : Att()
{
    this->ResetKeySigDefaultVis();
}

void AttKeySigDefaultVis::ResetKeySigDefaultVis()
{
    m_keysigCancelaccid = CANCELACCID_NONE;
    m_keysigVisible = BOOLEAN_NONE;
}

bool AttKeySigDefaultVis::ReadKeySigDefaultVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("keysig.cancelaccid")) {
        this->SetKeysigCancelaccid(StrToCancelaccid(element.attribute("keysig.cancelaccid").value()));
        if (removeAttr) element.remove_attribute("keysig.cancelaccid");
        hasAttribute = true;
    }
    if (element.attribute("keysig.visible")) {
        this->SetKeysigVisible(StrToBoolean(element.attribute("keysig.visible").value()));
        if (removeAttr) element.remove_attribute("keysig.visible");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigDefaultVis::WriteKeySigDefaultVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasKeysigCancelaccid()) {
        element.append_attribute("keysig.cancelaccid") = CancelaccidToStr(this->GetKeysigCancelaccid()).c_str();
        wroteAttribute = true;
    }
    if (this->HasKeysigVisible()) {
        element.append_attribute("keysig.visible") = BooleanToStr(this->GetKeysigVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigDefaultVis::HasKeysigCancelaccid() const
{
    return (m_keysigCancelaccid != CANCELACCID_NONE);
}

bool AttKeySigDefaultVis::HasKeysigVisible() const
{
    return (m_keysigVisible != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttLigatureVis
//----------------------------------------------------------------------------

AttLigatureVis::AttLigatureVis() : Att()
{
    this->ResetLigatureVis();
}

void AttLigatureVis::ResetLigatureVis()
{
    m_form = LIGATUREFORM_NONE;
}

bool AttLigatureVis::ReadLigatureVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToLigatureform(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLigatureVis::WriteLigatureVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = LigatureformToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLigatureVis::HasForm() const
{
    return (m_form != LIGATUREFORM_NONE);
}

//----------------------------------------------------------------------------
// AttLineVis
//----------------------------------------------------------------------------

AttLineVis::AttLineVis() : Att()
{
    this->ResetLineVis();
}

void AttLineVis::ResetLineVis()
{
    m_form = LINEFORM_NONE;
    m_width = data_LINEWIDTH();
    m_endsym = LINESTARTENDSYMBOL_NONE;
    m_endsymSize = MEI_UNSET;
    m_startsym = LINESTARTENDSYMBOL_NONE;
    m_startsymSize = MEI_UNSET;
}

bool AttLineVis::ReadLineVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToLineform(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("width")) {
        this->SetWidth(StrToLinewidth(element.attribute("width").value()));
        if (removeAttr) element.remove_attribute("width");
        hasAttribute = true;
    }
    if (element.attribute("endsym")) {
        this->SetEndsym(StrToLinestartendsymbol(element.attribute("endsym").value()));
        if (removeAttr) element.remove_attribute("endsym");
        hasAttribute = true;
    }
    if (element.attribute("endsym.size")) {
        this->SetEndsymSize(StrToInt(element.attribute("endsym.size").value()));
        if (removeAttr) element.remove_attribute("endsym.size");
        hasAttribute = true;
    }
    if (element.attribute("startsym")) {
        this->SetStartsym(StrToLinestartendsymbol(element.attribute("startsym").value()));
        if (removeAttr) element.remove_attribute("startsym");
        hasAttribute = true;
    }
    if (element.attribute("startsym.size")) {
        this->SetStartsymSize(StrToInt(element.attribute("startsym.size").value()));
        if (removeAttr) element.remove_attribute("startsym.size");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLineVis::WriteLineVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = LineformToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasWidth()) {
        element.append_attribute("width") = LinewidthToStr(this->GetWidth()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndsym()) {
        element.append_attribute("endsym") = LinestartendsymbolToStr(this->GetEndsym()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndsymSize()) {
        element.append_attribute("endsym.size") = IntToStr(this->GetEndsymSize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStartsym()) {
        element.append_attribute("startsym") = LinestartendsymbolToStr(this->GetStartsym()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStartsymSize()) {
        element.append_attribute("startsym.size") = IntToStr(this->GetStartsymSize()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLineVis::HasForm() const
{
    return (m_form != LINEFORM_NONE);
}

bool AttLineVis::HasWidth() const
{
    return (m_width.HasValue());
}

bool AttLineVis::HasEndsym() const
{
    return (m_endsym != LINESTARTENDSYMBOL_NONE);
}

bool AttLineVis::HasEndsymSize() const
{
    return (m_endsymSize != MEI_UNSET);
}

bool AttLineVis::HasStartsym() const
{
    return (m_startsym != LINESTARTENDSYMBOL_NONE);
}

bool AttLineVis::HasStartsymSize() const
{
    return (m_startsymSize != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttLiquescentVis
//----------------------------------------------------------------------------

AttLiquescentVis::AttLiquescentVis() : Att()
{
    this->ResetLiquescentVis();
}

void AttLiquescentVis::ResetLiquescentVis()
{
    m_looped = BOOLEAN_NONE;
}

bool AttLiquescentVis::ReadLiquescentVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("looped")) {
        this->SetLooped(StrToBoolean(element.attribute("looped").value()));
        if (removeAttr) element.remove_attribute("looped");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLiquescentVis::WriteLiquescentVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLooped()) {
        element.append_attribute("looped") = BooleanToStr(this->GetLooped()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLiquescentVis::HasLooped() const
{
    return (m_looped != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttMensurVis
//----------------------------------------------------------------------------

AttMensurVis::AttMensurVis() : Att()
{
    this->ResetMensurVis();
}

void AttMensurVis::ResetMensurVis()
{
    m_dot = BOOLEAN_NONE;
    m_form = mensurVis_FORM_NONE;
    m_orient = ORIENTATION_NONE;
    m_sign = MENSURATIONSIGN_NONE;
}

bool AttMensurVis::ReadMensurVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dot")) {
        this->SetDot(StrToBoolean(element.attribute("dot").value()));
        if (removeAttr) element.remove_attribute("dot");
        hasAttribute = true;
    }
    if (element.attribute("form")) {
        this->SetForm(StrToMensurVisForm(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("orient")) {
        this->SetOrient(StrToOrientation(element.attribute("orient").value()));
        if (removeAttr) element.remove_attribute("orient");
        hasAttribute = true;
    }
    if (element.attribute("sign")) {
        this->SetSign(StrToMensurationsign(element.attribute("sign").value()));
        if (removeAttr) element.remove_attribute("sign");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensurVis::WriteMensurVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDot()) {
        element.append_attribute("dot") = BooleanToStr(this->GetDot()).c_str();
        wroteAttribute = true;
    }
    if (this->HasForm()) {
        element.append_attribute("form") = MensurVisFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOrient()) {
        element.append_attribute("orient") = OrientationToStr(this->GetOrient()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSign()) {
        element.append_attribute("sign") = MensurationsignToStr(this->GetSign()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensurVis::HasDot() const
{
    return (m_dot != BOOLEAN_NONE);
}

bool AttMensurVis::HasForm() const
{
    return (m_form != mensurVis_FORM_NONE);
}

bool AttMensurVis::HasOrient() const
{
    return (m_orient != ORIENTATION_NONE);
}

bool AttMensurVis::HasSign() const
{
    return (m_sign != MENSURATIONSIGN_NONE);
}

//----------------------------------------------------------------------------
// AttMensuralVis
//----------------------------------------------------------------------------

AttMensuralVis::AttMensuralVis() : Att()
{
    this->ResetMensuralVis();
}

void AttMensuralVis::ResetMensuralVis()
{
    m_mensurColor = "";
    m_mensurDot = BOOLEAN_NONE;
    m_mensurForm = mensuralVis_MENSURFORM_NONE;
    m_mensurLoc = MEI_UNSET;
    m_mensurOrient = ORIENTATION_NONE;
    m_mensurSign = MENSURATIONSIGN_NONE;
    m_mensurSize = data_FONTSIZE();
    m_mensurSlash = 0;
}

bool AttMensuralVis::ReadMensuralVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("mensur.color")) {
        this->SetMensurColor(StrToStr(element.attribute("mensur.color").value()));
        if (removeAttr) element.remove_attribute("mensur.color");
        hasAttribute = true;
    }
    if (element.attribute("mensur.dot")) {
        this->SetMensurDot(StrToBoolean(element.attribute("mensur.dot").value()));
        if (removeAttr) element.remove_attribute("mensur.dot");
        hasAttribute = true;
    }
    if (element.attribute("mensur.form")) {
        this->SetMensurForm(StrToMensuralVisMensurform(element.attribute("mensur.form").value()));
        if (removeAttr) element.remove_attribute("mensur.form");
        hasAttribute = true;
    }
    if (element.attribute("mensur.loc")) {
        this->SetMensurLoc(StrToInt(element.attribute("mensur.loc").value()));
        if (removeAttr) element.remove_attribute("mensur.loc");
        hasAttribute = true;
    }
    if (element.attribute("mensur.orient")) {
        this->SetMensurOrient(StrToOrientation(element.attribute("mensur.orient").value()));
        if (removeAttr) element.remove_attribute("mensur.orient");
        hasAttribute = true;
    }
    if (element.attribute("mensur.sign")) {
        this->SetMensurSign(StrToMensurationsign(element.attribute("mensur.sign").value()));
        if (removeAttr) element.remove_attribute("mensur.sign");
        hasAttribute = true;
    }
    if (element.attribute("mensur.size")) {
        this->SetMensurSize(StrToFontsize(element.attribute("mensur.size").value()));
        if (removeAttr) element.remove_attribute("mensur.size");
        hasAttribute = true;
    }
    if (element.attribute("mensur.slash")) {
        this->SetMensurSlash(StrToInt(element.attribute("mensur.slash").value()));
        if (removeAttr) element.remove_attribute("mensur.slash");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMensuralVis::WriteMensuralVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMensurColor()) {
        element.append_attribute("mensur.color") = StrToStr(this->GetMensurColor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurDot()) {
        element.append_attribute("mensur.dot") = BooleanToStr(this->GetMensurDot()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurForm()) {
        element.append_attribute("mensur.form") = MensuralVisMensurformToStr(this->GetMensurForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurLoc()) {
        element.append_attribute("mensur.loc") = IntToStr(this->GetMensurLoc()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurOrient()) {
        element.append_attribute("mensur.orient") = OrientationToStr(this->GetMensurOrient()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurSign()) {
        element.append_attribute("mensur.sign") = MensurationsignToStr(this->GetMensurSign()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurSize()) {
        element.append_attribute("mensur.size") = FontsizeToStr(this->GetMensurSize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMensurSlash()) {
        element.append_attribute("mensur.slash") = IntToStr(this->GetMensurSlash()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensuralVis::HasMensurColor() const
{
    return (m_mensurColor != "");
}

bool AttMensuralVis::HasMensurDot() const
{
    return (m_mensurDot != BOOLEAN_NONE);
}

bool AttMensuralVis::HasMensurForm() const
{
    return (m_mensurForm != mensuralVis_MENSURFORM_NONE);
}

bool AttMensuralVis::HasMensurLoc() const
{
    return (m_mensurLoc != MEI_UNSET);
}

bool AttMensuralVis::HasMensurOrient() const
{
    return (m_mensurOrient != ORIENTATION_NONE);
}

bool AttMensuralVis::HasMensurSign() const
{
    return (m_mensurSign != MENSURATIONSIGN_NONE);
}

bool AttMensuralVis::HasMensurSize() const
{
    return (m_mensurSize.HasValue());
}

bool AttMensuralVis::HasMensurSlash() const
{
    return (m_mensurSlash != 0);
}

//----------------------------------------------------------------------------
// AttMeterSigVis
//----------------------------------------------------------------------------

AttMeterSigVis::AttMeterSigVis() : Att()
{
    this->ResetMeterSigVis();
}

void AttMeterSigVis::ResetMeterSigVis()
{
    m_form = METERFORM_NONE;
}

bool AttMeterSigVis::ReadMeterSigVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToMeterform(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigVis::WriteMeterSigVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = MeterformToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterSigVis::HasForm() const
{
    return (m_form != METERFORM_NONE);
}

//----------------------------------------------------------------------------
// AttMeterSigDefaultVis
//----------------------------------------------------------------------------

AttMeterSigDefaultVis::AttMeterSigDefaultVis() : Att()
{
    this->ResetMeterSigDefaultVis();
}

void AttMeterSigDefaultVis::ResetMeterSigDefaultVis()
{
    m_meterForm = METERFORM_NONE;
    m_meterShowchange = BOOLEAN_NONE;
    m_meterVisible = BOOLEAN_NONE;
}

bool AttMeterSigDefaultVis::ReadMeterSigDefaultVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("meter.form")) {
        this->SetMeterForm(StrToMeterform(element.attribute("meter.form").value()));
        if (removeAttr) element.remove_attribute("meter.form");
        hasAttribute = true;
    }
    if (element.attribute("meter.showchange")) {
        this->SetMeterShowchange(StrToBoolean(element.attribute("meter.showchange").value()));
        if (removeAttr) element.remove_attribute("meter.showchange");
        hasAttribute = true;
    }
    if (element.attribute("meter.visible")) {
        this->SetMeterVisible(StrToBoolean(element.attribute("meter.visible").value()));
        if (removeAttr) element.remove_attribute("meter.visible");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigDefaultVis::WriteMeterSigDefaultVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMeterForm()) {
        element.append_attribute("meter.form") = MeterformToStr(this->GetMeterForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMeterShowchange()) {
        element.append_attribute("meter.showchange") = BooleanToStr(this->GetMeterShowchange()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMeterVisible()) {
        element.append_attribute("meter.visible") = BooleanToStr(this->GetMeterVisible()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterSigDefaultVis::HasMeterForm() const
{
    return (m_meterForm != METERFORM_NONE);
}

bool AttMeterSigDefaultVis::HasMeterShowchange() const
{
    return (m_meterShowchange != BOOLEAN_NONE);
}

bool AttMeterSigDefaultVis::HasMeterVisible() const
{
    return (m_meterVisible != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttMultiRestVis
//----------------------------------------------------------------------------

AttMultiRestVis::AttMultiRestVis() : Att()
{
    this->ResetMultiRestVis();
}

void AttMultiRestVis::ResetMultiRestVis()
{
    m_block = BOOLEAN_NONE;
}

bool AttMultiRestVis::ReadMultiRestVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("block")) {
        this->SetBlock(StrToBoolean(element.attribute("block").value()));
        if (removeAttr) element.remove_attribute("block");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMultiRestVis::WriteMultiRestVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBlock()) {
        element.append_attribute("block") = BooleanToStr(this->GetBlock()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMultiRestVis::HasBlock() const
{
    return (m_block != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttPbVis
//----------------------------------------------------------------------------

AttPbVis::AttPbVis() : Att()
{
    this->ResetPbVis();
}

void AttPbVis::ResetPbVis()
{
    m_folium = pbVis_FOLIUM_NONE;
}

bool AttPbVis::ReadPbVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("folium")) {
        this->SetFolium(StrToPbVisFolium(element.attribute("folium").value()));
        if (removeAttr) element.remove_attribute("folium");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPbVis::WritePbVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFolium()) {
        element.append_attribute("folium") = PbVisFoliumToStr(this->GetFolium()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPbVis::HasFolium() const
{
    return (m_folium != pbVis_FOLIUM_NONE);
}

//----------------------------------------------------------------------------
// AttPedalVis
//----------------------------------------------------------------------------

AttPedalVis::AttPedalVis() : Att()
{
    this->ResetPedalVis();
}

void AttPedalVis::ResetPedalVis()
{
    m_form = PEDALSTYLE_NONE;
}

bool AttPedalVis::ReadPedalVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToPedalstyle(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPedalVis::WritePedalVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = PedalstyleToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPedalVis::HasForm() const
{
    return (m_form != PEDALSTYLE_NONE);
}

//----------------------------------------------------------------------------
// AttPlicaVis
//----------------------------------------------------------------------------

AttPlicaVis::AttPlicaVis() : Att()
{
    this->ResetPlicaVis();
}

void AttPlicaVis::ResetPlicaVis()
{
    m_dir = STEMDIRECTION_basic_NONE;
    m_len = data_MEASUREMENTUNSIGNED();
}

bool AttPlicaVis::ReadPlicaVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("dir")) {
        this->SetDir(StrToStemdirectionBasic(element.attribute("dir").value()));
        if (removeAttr) element.remove_attribute("dir");
        hasAttribute = true;
    }
    if (element.attribute("len")) {
        this->SetLen(StrToMeasurementunsigned(element.attribute("len").value()));
        if (removeAttr) element.remove_attribute("len");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPlicaVis::WritePlicaVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasDir()) {
        element.append_attribute("dir") = StemdirectionBasicToStr(this->GetDir()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLen()) {
        element.append_attribute("len") = MeasurementunsignedToStr(this->GetLen()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPlicaVis::HasDir() const
{
    return (m_dir != STEMDIRECTION_basic_NONE);
}

bool AttPlicaVis::HasLen() const
{
    return (m_len != data_MEASUREMENTUNSIGNED());
}

//----------------------------------------------------------------------------
// AttQuilismaVis
//----------------------------------------------------------------------------

AttQuilismaVis::AttQuilismaVis() : Att()
{
    this->ResetQuilismaVis();
}

void AttQuilismaVis::ResetQuilismaVis()
{
    m_waves = MEI_UNSET;
}

bool AttQuilismaVis::ReadQuilismaVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("waves")) {
        this->SetWaves(StrToInt(element.attribute("waves").value()));
        if (removeAttr) element.remove_attribute("waves");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttQuilismaVis::WriteQuilismaVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasWaves()) {
        element.append_attribute("waves") = IntToStr(this->GetWaves()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttQuilismaVis::HasWaves() const
{
    return (m_waves != MEI_UNSET);
}

//----------------------------------------------------------------------------
// AttSbVis
//----------------------------------------------------------------------------

AttSbVis::AttSbVis() : Att()
{
    this->ResetSbVis();
}

void AttSbVis::ResetSbVis()
{
    m_form = sbVis_FORM_NONE;
}

bool AttSbVis::ReadSbVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToSbVisForm(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSbVis::WriteSbVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = SbVisFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSbVis::HasForm() const
{
    return (m_form != sbVis_FORM_NONE);
}

//----------------------------------------------------------------------------
// AttScoreDefVis
//----------------------------------------------------------------------------

AttScoreDefVis::AttScoreDefVis() : Att()
{
    this->ResetScoreDefVis();
}

void AttScoreDefVis::ResetScoreDefVis()
{
    m_vuHeight = "";
}

bool AttScoreDefVis::ReadScoreDefVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("vu.height")) {
        this->SetVuHeight(StrToStr(element.attribute("vu.height").value()));
        if (removeAttr) element.remove_attribute("vu.height");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttScoreDefVis::WriteScoreDefVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasVuHeight()) {
        element.append_attribute("vu.height") = StrToStr(this->GetVuHeight()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttScoreDefVis::HasVuHeight() const
{
    return (m_vuHeight != "");
}

//----------------------------------------------------------------------------
// AttSectionVis
//----------------------------------------------------------------------------

AttSectionVis::AttSectionVis() : Att()
{
    this->ResetSectionVis();
}

void AttSectionVis::ResetSectionVis()
{
    m_restart = BOOLEAN_NONE;
}

bool AttSectionVis::ReadSectionVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("restart")) {
        this->SetRestart(StrToBoolean(element.attribute("restart").value()));
        if (removeAttr) element.remove_attribute("restart");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSectionVis::WriteSectionVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasRestart()) {
        element.append_attribute("restart") = BooleanToStr(this->GetRestart()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSectionVis::HasRestart() const
{
    return (m_restart != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttSignifLetVis
//----------------------------------------------------------------------------

AttSignifLetVis::AttSignifLetVis() : Att()
{
    this->ResetSignifLetVis();
}

void AttSignifLetVis::ResetSignifLetVis()
{
    m_place = data_EVENTREL();
}

bool AttSignifLetVis::ReadSignifLetVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("place")) {
        this->SetPlace(StrToEventrel(element.attribute("place").value()));
        if (removeAttr) element.remove_attribute("place");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSignifLetVis::WriteSignifLetVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPlace()) {
        element.append_attribute("place") = EventrelToStr(this->GetPlace()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSignifLetVis::HasPlace() const
{
    return (m_place != data_EVENTREL());
}

//----------------------------------------------------------------------------
// AttSpaceVis
//----------------------------------------------------------------------------

AttSpaceVis::AttSpaceVis() : Att()
{
    this->ResetSpaceVis();
}

void AttSpaceVis::ResetSpaceVis()
{
    m_compressable = BOOLEAN_NONE;
}

bool AttSpaceVis::ReadSpaceVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("compressable")) {
        this->SetCompressable(StrToBoolean(element.attribute("compressable").value()));
        if (removeAttr) element.remove_attribute("compressable");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSpaceVis::WriteSpaceVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCompressable()) {
        element.append_attribute("compressable") = BooleanToStr(this->GetCompressable()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSpaceVis::HasCompressable() const
{
    return (m_compressable != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttStaffDefVis
//----------------------------------------------------------------------------

AttStaffDefVis::AttStaffDefVis() : Att()
{
    this->ResetStaffDefVis();
}

void AttStaffDefVis::ResetStaffDefVis()
{
    m_layerscheme = LAYERSCHEME_NONE;
    m_linesColor = "";
    m_linesVisible = BOOLEAN_NONE;
    m_spacing = data_MEASUREMENTSIGNED();
}

bool AttStaffDefVis::ReadStaffDefVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("layerscheme")) {
        this->SetLayerscheme(StrToLayerscheme(element.attribute("layerscheme").value()));
        if (removeAttr) element.remove_attribute("layerscheme");
        hasAttribute = true;
    }
    if (element.attribute("lines.color")) {
        this->SetLinesColor(StrToStr(element.attribute("lines.color").value()));
        if (removeAttr) element.remove_attribute("lines.color");
        hasAttribute = true;
    }
    if (element.attribute("lines.visible")) {
        this->SetLinesVisible(StrToBoolean(element.attribute("lines.visible").value()));
        if (removeAttr) element.remove_attribute("lines.visible");
        hasAttribute = true;
    }
    if (element.attribute("spacing")) {
        this->SetSpacing(StrToMeasurementsigned(element.attribute("spacing").value()));
        if (removeAttr) element.remove_attribute("spacing");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffDefVis::WriteStaffDefVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasLayerscheme()) {
        element.append_attribute("layerscheme") = LayerschemeToStr(this->GetLayerscheme()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLinesColor()) {
        element.append_attribute("lines.color") = StrToStr(this->GetLinesColor()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLinesVisible()) {
        element.append_attribute("lines.visible") = BooleanToStr(this->GetLinesVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSpacing()) {
        element.append_attribute("spacing") = MeasurementsignedToStr(this->GetSpacing()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffDefVis::HasLayerscheme() const
{
    return (m_layerscheme != LAYERSCHEME_NONE);
}

bool AttStaffDefVis::HasLinesColor() const
{
    return (m_linesColor != "");
}

bool AttStaffDefVis::HasLinesVisible() const
{
    return (m_linesVisible != BOOLEAN_NONE);
}

bool AttStaffDefVis::HasSpacing() const
{
    return (m_spacing != data_MEASUREMENTSIGNED());
}

//----------------------------------------------------------------------------
// AttStaffGrpVis
//----------------------------------------------------------------------------

AttStaffGrpVis::AttStaffGrpVis() : Att()
{
    this->ResetStaffGrpVis();
}

void AttStaffGrpVis::ResetStaffGrpVis()
{
    m_barThru = BOOLEAN_NONE;
}

bool AttStaffGrpVis::ReadStaffGrpVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("bar.thru")) {
        this->SetBarThru(StrToBoolean(element.attribute("bar.thru").value()));
        if (removeAttr) element.remove_attribute("bar.thru");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffGrpVis::WriteStaffGrpVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBarThru()) {
        element.append_attribute("bar.thru") = BooleanToStr(this->GetBarThru()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffGrpVis::HasBarThru() const
{
    return (m_barThru != BOOLEAN_NONE);
}

//----------------------------------------------------------------------------
// AttStemVis
//----------------------------------------------------------------------------

AttStemVis::AttStemVis() : Att()
{
    this->ResetStemVis();
}

void AttStemVis::ResetStemVis()
{
    m_pos = STEMPOSITION_NONE;
    m_len = data_MEASUREMENTUNSIGNED();
    m_form = STEMFORM_mensural_NONE;
    m_dir = STEMDIRECTION_NONE;
    m_flagPos = FLAGPOS_mensural_NONE;
    m_flagForm = FLAGFORM_mensural_NONE;
}

bool AttStemVis::ReadStemVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("pos")) {
        this->SetPos(StrToStemposition(element.attribute("pos").value()));
        if (removeAttr) element.remove_attribute("pos");
        hasAttribute = true;
    }
    if (element.attribute("len")) {
        this->SetLen(StrToMeasurementunsigned(element.attribute("len").value()));
        if (removeAttr) element.remove_attribute("len");
        hasAttribute = true;
    }
    if (element.attribute("form")) {
        this->SetForm(StrToStemformMensural(element.attribute("form").value()));
        if (removeAttr) element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("dir")) {
        this->SetDir(StrToStemdirection(element.attribute("dir").value()));
        if (removeAttr) element.remove_attribute("dir");
        hasAttribute = true;
    }
    if (element.attribute("flag.pos")) {
        this->SetFlagPos(StrToFlagposMensural(element.attribute("flag.pos").value()));
        if (removeAttr) element.remove_attribute("flag.pos");
        hasAttribute = true;
    }
    if (element.attribute("flag.form")) {
        this->SetFlagForm(StrToFlagformMensural(element.attribute("flag.form").value()));
        if (removeAttr) element.remove_attribute("flag.form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStemVis::WriteStemVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasPos()) {
        element.append_attribute("pos") = StempositionToStr(this->GetPos()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLen()) {
        element.append_attribute("len") = MeasurementunsignedToStr(this->GetLen()).c_str();
        wroteAttribute = true;
    }
    if (this->HasForm()) {
        element.append_attribute("form") = StemformMensuralToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDir()) {
        element.append_attribute("dir") = StemdirectionToStr(this->GetDir()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFlagPos()) {
        element.append_attribute("flag.pos") = FlagposMensuralToStr(this->GetFlagPos()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFlagForm()) {
        element.append_attribute("flag.form") = FlagformMensuralToStr(this->GetFlagForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStemVis::HasPos() const
{
    return (m_pos != STEMPOSITION_NONE);
}

bool AttStemVis::HasLen() const
{
    return (m_len != data_MEASUREMENTUNSIGNED());
}

bool AttStemVis::HasForm() const
{
    return (m_form != STEMFORM_mensural_NONE);
}

bool AttStemVis::HasDir() const
{
    return (m_dir != STEMDIRECTION_NONE);
}

bool AttStemVis::HasFlagPos() const
{
    return (m_flagPos != FLAGPOS_mensural_NONE);
}

bool AttStemVis::HasFlagForm() const
{
    return (m_flagForm != FLAGFORM_mensural_NONE);
}

//----------------------------------------------------------------------------
// AttTupletVis
//----------------------------------------------------------------------------

AttTupletVis::AttTupletVis() : Att()
{
    this->ResetTupletVis();
}

void AttTupletVis::ResetTupletVis()
{
    m_bracketPlace = STAFFREL_basic_NONE;
    m_bracketVisible = BOOLEAN_NONE;
    m_durVisible = BOOLEAN_NONE;
    m_numFormat = tupletVis_NUMFORMAT_NONE;
}

bool AttTupletVis::ReadTupletVis(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("bracket.place")) {
        this->SetBracketPlace(StrToStaffrelBasic(element.attribute("bracket.place").value()));
        if (removeAttr) element.remove_attribute("bracket.place");
        hasAttribute = true;
    }
    if (element.attribute("bracket.visible")) {
        this->SetBracketVisible(StrToBoolean(element.attribute("bracket.visible").value()));
        if (removeAttr) element.remove_attribute("bracket.visible");
        hasAttribute = true;
    }
    if (element.attribute("dur.visible")) {
        this->SetDurVisible(StrToBoolean(element.attribute("dur.visible").value()));
        if (removeAttr) element.remove_attribute("dur.visible");
        hasAttribute = true;
    }
    if (element.attribute("num.format")) {
        this->SetNumFormat(StrToTupletVisNumformat(element.attribute("num.format").value()));
        if (removeAttr) element.remove_attribute("num.format");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTupletVis::WriteTupletVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBracketPlace()) {
        element.append_attribute("bracket.place") = StaffrelBasicToStr(this->GetBracketPlace()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBracketVisible()) {
        element.append_attribute("bracket.visible") = BooleanToStr(this->GetBracketVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDurVisible()) {
        element.append_attribute("dur.visible") = BooleanToStr(this->GetDurVisible()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNumFormat()) {
        element.append_attribute("num.format") = TupletVisNumformatToStr(this->GetNumFormat()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTupletVis::HasBracketPlace() const
{
    return (m_bracketPlace != STAFFREL_basic_NONE);
}

bool AttTupletVis::HasBracketVisible() const
{
    return (m_bracketVisible != BOOLEAN_NONE);
}

bool AttTupletVis::HasDurVisible() const
{
    return (m_durVisible != BOOLEAN_NONE);
}

bool AttTupletVis::HasNumFormat() const
{
    return (m_numFormat != tupletVis_NUMFORMAT_NONE);
}

} // namespace vrv
