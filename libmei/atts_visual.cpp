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

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttAnnotVis
//----------------------------------------------------------------------------

AttAnnotVis::AttAnnotVis() : Att()
{
    ResetAnnotVis();
}

AttAnnotVis::~AttAnnotVis()
{
}

void AttAnnotVis::ResetAnnotVis()
{
    m_place = data_PLACEMENT();
}

bool AttAnnotVis::ReadAnnotVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("place")) {
        this->SetPlace(StrToPlacement(element.attribute("place").value()));
        element.remove_attribute("place");
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

/* include <attplace> */

//----------------------------------------------------------------------------
// AttArpegVis
//----------------------------------------------------------------------------

AttArpegVis::AttArpegVis() : Att()
{
    ResetArpegVis();
}

AttArpegVis::~AttArpegVis()
{
}

void AttArpegVis::ResetArpegVis()
{
    m_arrow = BOOLEAN_NONE;
    m_arrowShape = LINESTARTENDSYMBOL_NONE;
    m_arrowSize = 0;
    m_arrowColor = "";
    m_arrowFillcolor = "";
    m_lineForm = LINEFORM_NONE;
    m_lineWidth = data_LINEWIDTH();
}

bool AttArpegVis::ReadArpegVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("arrow")) {
        this->SetArrow(StrToBoolean(element.attribute("arrow").value()));
        element.remove_attribute("arrow");
        hasAttribute = true;
    }
    if (element.attribute("arrow.shape")) {
        this->SetArrowShape(StrToLinestartendsymbol(element.attribute("arrow.shape").value()));
        element.remove_attribute("arrow.shape");
        hasAttribute = true;
    }
    if (element.attribute("arrow.size")) {
        this->SetArrowSize(StrToInt(element.attribute("arrow.size").value()));
        element.remove_attribute("arrow.size");
        hasAttribute = true;
    }
    if (element.attribute("arrow.color")) {
        this->SetArrowColor(StrToStr(element.attribute("arrow.color").value()));
        element.remove_attribute("arrow.color");
        hasAttribute = true;
    }
    if (element.attribute("arrow.fillcolor")) {
        this->SetArrowFillcolor(StrToStr(element.attribute("arrow.fillcolor").value()));
        element.remove_attribute("arrow.fillcolor");
        hasAttribute = true;
    }
    if (element.attribute("line.form")) {
        this->SetLineForm(StrToLineform(element.attribute("line.form").value()));
        element.remove_attribute("line.form");
        hasAttribute = true;
    }
    if (element.attribute("line.width")) {
        this->SetLineWidth(StrToLinewidth(element.attribute("line.width").value()));
        element.remove_attribute("line.width");
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
    if (this->HasLineForm()) {
        element.append_attribute("line.form") = LineformToStr(this->GetLineForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLineWidth()) {
        element.append_attribute("line.width") = LinewidthToStr(this->GetLineWidth()).c_str();
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
    return (m_arrowSize != 0);
}

bool AttArpegVis::HasArrowColor() const
{
    return (m_arrowColor != "");
}

bool AttArpegVis::HasArrowFillcolor() const
{
    return (m_arrowFillcolor != "");
}

bool AttArpegVis::HasLineForm() const
{
    return (m_lineForm != LINEFORM_NONE);
}

bool AttArpegVis::HasLineWidth() const
{
    return (m_lineWidth.HasValue());
}

/* include <attline.width> */

//----------------------------------------------------------------------------
// AttBarLineVis
//----------------------------------------------------------------------------

AttBarLineVis::AttBarLineVis() : Att()
{
    ResetBarLineVis();
}

AttBarLineVis::~AttBarLineVis()
{
}

void AttBarLineVis::ResetBarLineVis()
{
    m_len = 0.0;
    m_method = BARMETHOD_NONE;
    m_place = 0;
}

bool AttBarLineVis::ReadBarLineVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("len")) {
        this->SetLen(StrToDbl(element.attribute("len").value()));
        element.remove_attribute("len");
        hasAttribute = true;
    }
    if (element.attribute("method")) {
        this->SetMethod(StrToBarmethod(element.attribute("method").value()));
        element.remove_attribute("method");
        hasAttribute = true;
    }
    if (element.attribute("place")) {
        this->SetPlace(StrToInt(element.attribute("place").value()));
        element.remove_attribute("place");
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
    return (m_place != 0);
}

/* include <attplace> */

//----------------------------------------------------------------------------
// AttBeamingVis
//----------------------------------------------------------------------------

AttBeamingVis::AttBeamingVis() : Att()
{
    ResetBeamingVis();
}

AttBeamingVis::~AttBeamingVis()
{
}

void AttBeamingVis::ResetBeamingVis()
{
    m_beamColor = "";
    m_beamRend = beamingVis_BEAMREND_NONE;
    m_beamSlope = 0.0;
}

bool AttBeamingVis::ReadBeamingVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("beam.color")) {
        this->SetBeamColor(StrToStr(element.attribute("beam.color").value()));
        element.remove_attribute("beam.color");
        hasAttribute = true;
    }
    if (element.attribute("beam.rend")) {
        this->SetBeamRend(StrToBeamingVisBeamrend(element.attribute("beam.rend").value()));
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

/* include <attbeam.slope> */

//----------------------------------------------------------------------------
// AttBeatRptVis
//----------------------------------------------------------------------------

AttBeatRptVis::AttBeatRptVis() : Att()
{
    ResetBeatRptVis();
}

AttBeatRptVis::~AttBeatRptVis()
{
}

void AttBeatRptVis::ResetBeatRptVis()
{
    m_slash = BEATRPT_REND_NONE;
}

bool AttBeatRptVis::ReadBeatRptVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("slash")) {
        this->SetSlash(StrToBeatrptRend(element.attribute("slash").value()));
        element.remove_attribute("slash");
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

/* include <attslash> */

//----------------------------------------------------------------------------
// AttChordVis
//----------------------------------------------------------------------------

AttChordVis::AttChordVis() : Att()
{
    ResetChordVis();
}

AttChordVis::~AttChordVis()
{
}

void AttChordVis::ResetChordVis()
{
    m_cluster = CLUSTER_NONE;
}

bool AttChordVis::ReadChordVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("cluster")) {
        this->SetCluster(StrToCluster(element.attribute("cluster").value()));
        element.remove_attribute("cluster");
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

/* include <attcluster> */

//----------------------------------------------------------------------------
// AttCleffingVis
//----------------------------------------------------------------------------

AttCleffingVis::AttCleffingVis() : Att()
{
    ResetCleffingVis();
}

AttCleffingVis::~AttCleffingVis()
{
}

void AttCleffingVis::ResetCleffingVis()
{
    m_clefColor = "";
    m_clefVisible = BOOLEAN_NONE;
}

bool AttCleffingVis::ReadCleffingVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("clef.color")) {
        this->SetClefColor(StrToStr(element.attribute("clef.color").value()));
        element.remove_attribute("clef.color");
        hasAttribute = true;
    }
    if (element.attribute("clef.visible")) {
        this->SetClefVisible(StrToBoolean(element.attribute("clef.visible").value()));
        element.remove_attribute("clef.visible");
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

/* include <attclef.visible> */

//----------------------------------------------------------------------------
// AttEpisemaVis
//----------------------------------------------------------------------------

AttEpisemaVis::AttEpisemaVis() : Att()
{
    ResetEpisemaVis();
}

AttEpisemaVis::~AttEpisemaVis()
{
}

void AttEpisemaVis::ResetEpisemaVis()
{
    m_form = episemaVis_FORM_NONE;
    m_place = data_EVENTREL();
}

bool AttEpisemaVis::ReadEpisemaVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToEpisemaVisForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("place")) {
        this->SetPlace(StrToEventrel(element.attribute("place").value()));
        element.remove_attribute("place");
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

/* include <attplace> */

//----------------------------------------------------------------------------
// AttFTremVis
//----------------------------------------------------------------------------

AttFTremVis::AttFTremVis() : Att()
{
    ResetFTremVis();
}

AttFTremVis::~AttFTremVis()
{
}

void AttFTremVis::ResetFTremVis()
{
    m_beams = 0;
    m_beamsFloat = -1;
    m_floatGap = VRV_UNSET;
}

bool AttFTremVis::ReadFTremVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("beams")) {
        this->SetBeams(StrToInt(element.attribute("beams").value()));
        element.remove_attribute("beams");
        hasAttribute = true;
    }
    if (element.attribute("beams.float")) {
        this->SetBeamsFloat(StrToInt(element.attribute("beams.float").value()));
        element.remove_attribute("beams.float");
        hasAttribute = true;
    }
    if (element.attribute("float.gap")) {
        this->SetFloatGap(StrToMeasurementabs(element.attribute("float.gap").value()));
        element.remove_attribute("float.gap");
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
        element.append_attribute("float.gap") = MeasurementabsToStr(this->GetFloatGap()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFTremVis::HasBeams() const
{
    return (m_beams != 0);
}

bool AttFTremVis::HasBeamsFloat() const
{
    return (m_beamsFloat != -1);
}

bool AttFTremVis::HasFloatGap() const
{
    return (m_floatGap != VRV_UNSET);
}

/* include <attfloat.gap> */

//----------------------------------------------------------------------------
// AttFermataVis
//----------------------------------------------------------------------------

AttFermataVis::AttFermataVis() : Att()
{
    ResetFermataVis();
}

AttFermataVis::~AttFermataVis()
{
}

void AttFermataVis::ResetFermataVis()
{
    m_form = fermataVis_FORM_NONE;
    m_shape = fermataVis_SHAPE_NONE;
}

bool AttFermataVis::ReadFermataVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToFermataVisForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("shape")) {
        this->SetShape(StrToFermataVisShape(element.attribute("shape").value()));
        element.remove_attribute("shape");
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

/* include <attshape> */

//----------------------------------------------------------------------------
// AttFingGrpVis
//----------------------------------------------------------------------------

AttFingGrpVis::AttFingGrpVis() : Att()
{
    ResetFingGrpVis();
}

AttFingGrpVis::~AttFingGrpVis()
{
}

void AttFingGrpVis::ResetFingGrpVis()
{
    m_orient = fingGrpVis_ORIENT_NONE;
}

bool AttFingGrpVis::ReadFingGrpVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("orient")) {
        this->SetOrient(StrToFingGrpVisOrient(element.attribute("orient").value()));
        element.remove_attribute("orient");
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

/* include <attorient> */

//----------------------------------------------------------------------------
// AttHairpinVis
//----------------------------------------------------------------------------

AttHairpinVis::AttHairpinVis() : Att()
{
    ResetHairpinVis();
}

AttHairpinVis::~AttHairpinVis()
{
}

void AttHairpinVis::ResetHairpinVis()
{
    m_opening = VRV_UNSET;
}

bool AttHairpinVis::ReadHairpinVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("opening")) {
        this->SetOpening(StrToMeasurementabs(element.attribute("opening").value()));
        element.remove_attribute("opening");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHairpinVis::WriteHairpinVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOpening()) {
        element.append_attribute("opening") = MeasurementabsToStr(this->GetOpening()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHairpinVis::HasOpening() const
{
    return (m_opening != VRV_UNSET);
}

/* include <attopening> */

//----------------------------------------------------------------------------
// AttHarmVis
//----------------------------------------------------------------------------

AttHarmVis::AttHarmVis() : Att()
{
    ResetHarmVis();
}

AttHarmVis::~AttHarmVis()
{
}

void AttHarmVis::ResetHarmVis()
{
    m_rendgrid = harmVis_RENDGRID_NONE;
}

bool AttHarmVis::ReadHarmVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("rendgrid")) {
        this->SetRendgrid(StrToHarmVisRendgrid(element.attribute("rendgrid").value()));
        element.remove_attribute("rendgrid");
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

/* include <attrendgrid> */

//----------------------------------------------------------------------------
// AttHispanTickVis
//----------------------------------------------------------------------------

AttHispanTickVis::AttHispanTickVis() : Att()
{
    ResetHispanTickVis();
}

AttHispanTickVis::~AttHispanTickVis()
{
}

void AttHispanTickVis::ResetHispanTickVis()
{
    m_place = data_EVENTREL();
    m_tilt = data_COMPASSDIRECTION();
}

bool AttHispanTickVis::ReadHispanTickVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("place")) {
        this->SetPlace(StrToEventrel(element.attribute("place").value()));
        element.remove_attribute("place");
        hasAttribute = true;
    }
    if (element.attribute("tilt")) {
        this->SetTilt(StrToCompassdirection(element.attribute("tilt").value()));
        element.remove_attribute("tilt");
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

/* include <atttilt> */

//----------------------------------------------------------------------------
// AttKeySigVis
//----------------------------------------------------------------------------

AttKeySigVis::AttKeySigVis() : Att()
{
    ResetKeySigVis();
}

AttKeySigVis::~AttKeySigVis()
{
}

void AttKeySigVis::ResetKeySigVis()
{
    m_sigShowchange = BOOLEAN_NONE;
}

bool AttKeySigVis::ReadKeySigVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("sig.showchange")) {
        this->SetSigShowchange(StrToBoolean(element.attribute("sig.showchange").value()));
        element.remove_attribute("sig.showchange");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigVis::WriteKeySigVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSigShowchange()) {
        element.append_attribute("sig.showchange") = BooleanToStr(this->GetSigShowchange()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigVis::HasSigShowchange() const
{
    return (m_sigShowchange != BOOLEAN_NONE);
}

/* include <attsig.showchange> */

//----------------------------------------------------------------------------
// AttKeySigDefaultVis
//----------------------------------------------------------------------------

AttKeySigDefaultVis::AttKeySigDefaultVis() : Att()
{
    ResetKeySigDefaultVis();
}

AttKeySigDefaultVis::~AttKeySigDefaultVis()
{
}

void AttKeySigDefaultVis::ResetKeySigDefaultVis()
{
    m_keysigShow = BOOLEAN_NONE;
    m_keysigShowchange = BOOLEAN_NONE;
}

bool AttKeySigDefaultVis::ReadKeySigDefaultVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("keysig.show")) {
        this->SetKeysigShow(StrToBoolean(element.attribute("keysig.show").value()));
        element.remove_attribute("keysig.show");
        hasAttribute = true;
    }
    if (element.attribute("keysig.showchange")) {
        this->SetKeysigShowchange(StrToBoolean(element.attribute("keysig.showchange").value()));
        element.remove_attribute("keysig.showchange");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttKeySigDefaultVis::WriteKeySigDefaultVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasKeysigShow()) {
        element.append_attribute("keysig.show") = BooleanToStr(this->GetKeysigShow()).c_str();
        wroteAttribute = true;
    }
    if (this->HasKeysigShowchange()) {
        element.append_attribute("keysig.showchange") = BooleanToStr(this->GetKeysigShowchange()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttKeySigDefaultVis::HasKeysigShow() const
{
    return (m_keysigShow != BOOLEAN_NONE);
}

bool AttKeySigDefaultVis::HasKeysigShowchange() const
{
    return (m_keysigShowchange != BOOLEAN_NONE);
}

/* include <attkeysig.showchange> */

//----------------------------------------------------------------------------
// AttLigatureVis
//----------------------------------------------------------------------------

AttLigatureVis::AttLigatureVis() : Att()
{
    ResetLigatureVis();
}

AttLigatureVis::~AttLigatureVis()
{
}

void AttLigatureVis::ResetLigatureVis()
{
    m_form = LIGATUREFORM_NONE;
}

bool AttLigatureVis::ReadLigatureVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToLigatureform(element.attribute("form").value()));
        element.remove_attribute("form");
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

/* include <attform> */

//----------------------------------------------------------------------------
// AttLineVis
//----------------------------------------------------------------------------

AttLineVis::AttLineVis() : Att()
{
    ResetLineVis();
}

AttLineVis::~AttLineVis()
{
}

void AttLineVis::ResetLineVis()
{
    m_form = LINEFORM_NONE;
    m_width = data_LINEWIDTH();
    m_endsym = LINESTARTENDSYMBOL_NONE;
    m_endsymSize = 0;
    m_startsym = LINESTARTENDSYMBOL_NONE;
    m_startsymSize = 0;
}

bool AttLineVis::ReadLineVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToLineform(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("width")) {
        this->SetWidth(StrToLinewidth(element.attribute("width").value()));
        element.remove_attribute("width");
        hasAttribute = true;
    }
    if (element.attribute("endsym")) {
        this->SetEndsym(StrToLinestartendsymbol(element.attribute("endsym").value()));
        element.remove_attribute("endsym");
        hasAttribute = true;
    }
    if (element.attribute("endsym.size")) {
        this->SetEndsymSize(StrToInt(element.attribute("endsym.size").value()));
        element.remove_attribute("endsym.size");
        hasAttribute = true;
    }
    if (element.attribute("startsym")) {
        this->SetStartsym(StrToLinestartendsymbol(element.attribute("startsym").value()));
        element.remove_attribute("startsym");
        hasAttribute = true;
    }
    if (element.attribute("startsym.size")) {
        this->SetStartsymSize(StrToInt(element.attribute("startsym.size").value()));
        element.remove_attribute("startsym.size");
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
    return (m_endsymSize != 0);
}

bool AttLineVis::HasStartsym() const
{
    return (m_startsym != LINESTARTENDSYMBOL_NONE);
}

bool AttLineVis::HasStartsymSize() const
{
    return (m_startsymSize != 0);
}

/* include <attstartsym.size> */

//----------------------------------------------------------------------------
// AttLiquescentVis
//----------------------------------------------------------------------------

AttLiquescentVis::AttLiquescentVis() : Att()
{
    ResetLiquescentVis();
}

AttLiquescentVis::~AttLiquescentVis()
{
}

void AttLiquescentVis::ResetLiquescentVis()
{
    m_curve = liquescentVis_CURVE_NONE;
    m_looped = BOOLEAN_NONE;
}

bool AttLiquescentVis::ReadLiquescentVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("curve")) {
        this->SetCurve(StrToLiquescentVisCurve(element.attribute("curve").value()));
        element.remove_attribute("curve");
        hasAttribute = true;
    }
    if (element.attribute("looped")) {
        this->SetLooped(StrToBoolean(element.attribute("looped").value()));
        element.remove_attribute("looped");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLiquescentVis::WriteLiquescentVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCurve()) {
        element.append_attribute("curve") = LiquescentVisCurveToStr(this->GetCurve()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLooped()) {
        element.append_attribute("looped") = BooleanToStr(this->GetLooped()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLiquescentVis::HasCurve() const
{
    return (m_curve != liquescentVis_CURVE_NONE);
}

bool AttLiquescentVis::HasLooped() const
{
    return (m_looped != BOOLEAN_NONE);
}

/* include <attlooped> */

//----------------------------------------------------------------------------
// AttMensurVis
//----------------------------------------------------------------------------

AttMensurVis::AttMensurVis() : Att()
{
    ResetMensurVis();
}

AttMensurVis::~AttMensurVis()
{
}

void AttMensurVis::ResetMensurVis()
{
    m_dot = BOOLEAN_NONE;
    m_form = mensurVis_FORM_NONE;
    m_orient = ORIENTATION_NONE;
    m_sign = MENSURATIONSIGN_NONE;
}

bool AttMensurVis::ReadMensurVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("dot")) {
        this->SetDot(StrToBoolean(element.attribute("dot").value()));
        element.remove_attribute("dot");
        hasAttribute = true;
    }
    if (element.attribute("form")) {
        this->SetForm(StrToMensurVisForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("orient")) {
        this->SetOrient(StrToOrientation(element.attribute("orient").value()));
        element.remove_attribute("orient");
        hasAttribute = true;
    }
    if (element.attribute("sign")) {
        this->SetSign(StrToMensurationsign(element.attribute("sign").value()));
        element.remove_attribute("sign");
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

/* include <attsign> */

//----------------------------------------------------------------------------
// AttMensuralVis
//----------------------------------------------------------------------------

AttMensuralVis::AttMensuralVis() : Att()
{
    ResetMensuralVis();
}

AttMensuralVis::~AttMensuralVis()
{
}

void AttMensuralVis::ResetMensuralVis()
{
    m_mensurColor = "";
    m_mensurDot = BOOLEAN_NONE;
    m_mensurForm = mensuralVis_MENSURFORM_NONE;
    m_mensurLoc = 0;
    m_mensurOrient = ORIENTATION_NONE;
    m_mensurSign = MENSURATIONSIGN_NONE;
    m_mensurSize = data_FONTSIZE();
    m_mensurSlash = 0;
}

bool AttMensuralVis::ReadMensuralVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("mensur.color")) {
        this->SetMensurColor(StrToStr(element.attribute("mensur.color").value()));
        element.remove_attribute("mensur.color");
        hasAttribute = true;
    }
    if (element.attribute("mensur.dot")) {
        this->SetMensurDot(StrToBoolean(element.attribute("mensur.dot").value()));
        element.remove_attribute("mensur.dot");
        hasAttribute = true;
    }
    if (element.attribute("mensur.form")) {
        this->SetMensurForm(StrToMensuralVisMensurform(element.attribute("mensur.form").value()));
        element.remove_attribute("mensur.form");
        hasAttribute = true;
    }
    if (element.attribute("mensur.loc")) {
        this->SetMensurLoc(StrToInt(element.attribute("mensur.loc").value()));
        element.remove_attribute("mensur.loc");
        hasAttribute = true;
    }
    if (element.attribute("mensur.orient")) {
        this->SetMensurOrient(StrToOrientation(element.attribute("mensur.orient").value()));
        element.remove_attribute("mensur.orient");
        hasAttribute = true;
    }
    if (element.attribute("mensur.sign")) {
        this->SetMensurSign(StrToMensurationsign(element.attribute("mensur.sign").value()));
        element.remove_attribute("mensur.sign");
        hasAttribute = true;
    }
    if (element.attribute("mensur.size")) {
        this->SetMensurSize(StrToFontsize(element.attribute("mensur.size").value()));
        element.remove_attribute("mensur.size");
        hasAttribute = true;
    }
    if (element.attribute("mensur.slash")) {
        this->SetMensurSlash(StrToInt(element.attribute("mensur.slash").value()));
        element.remove_attribute("mensur.slash");
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
    return (m_mensurLoc != 0);
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

/* include <attmensur.slash> */

//----------------------------------------------------------------------------
// AttMeterSigVis
//----------------------------------------------------------------------------

AttMeterSigVis::AttMeterSigVis() : Att()
{
    ResetMeterSigVis();
}

AttMeterSigVis::~AttMeterSigVis()
{
}

void AttMeterSigVis::ResetMeterSigVis()
{
    m_form = METERFORM_NONE;
}

bool AttMeterSigVis::ReadMeterSigVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToMeterform(element.attribute("form").value()));
        element.remove_attribute("form");
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

/* include <attform> */

//----------------------------------------------------------------------------
// AttMeterSigDefaultVis
//----------------------------------------------------------------------------

AttMeterSigDefaultVis::AttMeterSigDefaultVis() : Att()
{
    ResetMeterSigDefaultVis();
}

AttMeterSigDefaultVis::~AttMeterSigDefaultVis()
{
}

void AttMeterSigDefaultVis::ResetMeterSigDefaultVis()
{
    m_meterForm = METERFORM_NONE;
    m_meterShowchange = BOOLEAN_NONE;
}

bool AttMeterSigDefaultVis::ReadMeterSigDefaultVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("meter.form")) {
        this->SetMeterForm(StrToMeterform(element.attribute("meter.form").value()));
        element.remove_attribute("meter.form");
        hasAttribute = true;
    }
    if (element.attribute("meter.showchange")) {
        this->SetMeterShowchange(StrToBoolean(element.attribute("meter.showchange").value()));
        element.remove_attribute("meter.showchange");
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

/* include <attmeter.showchange> */

//----------------------------------------------------------------------------
// AttMultiRestVis
//----------------------------------------------------------------------------

AttMultiRestVis::AttMultiRestVis() : Att()
{
    ResetMultiRestVis();
}

AttMultiRestVis::~AttMultiRestVis()
{
}

void AttMultiRestVis::ResetMultiRestVis()
{
    m_block = BOOLEAN_NONE;
}

bool AttMultiRestVis::ReadMultiRestVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("block")) {
        this->SetBlock(StrToBoolean(element.attribute("block").value()));
        element.remove_attribute("block");
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

/* include <attblock> */

//----------------------------------------------------------------------------
// AttPbVis
//----------------------------------------------------------------------------

AttPbVis::AttPbVis() : Att()
{
    ResetPbVis();
}

AttPbVis::~AttPbVis()
{
}

void AttPbVis::ResetPbVis()
{
    m_folium = pbVis_FOLIUM_NONE;
}

bool AttPbVis::ReadPbVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("folium")) {
        this->SetFolium(StrToPbVisFolium(element.attribute("folium").value()));
        element.remove_attribute("folium");
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

/* include <attfolium> */

//----------------------------------------------------------------------------
// AttPedalVis
//----------------------------------------------------------------------------

AttPedalVis::AttPedalVis() : Att()
{
    ResetPedalVis();
}

AttPedalVis::~AttPedalVis()
{
}

void AttPedalVis::ResetPedalVis()
{
    m_form = pedalVis_FORM_NONE;
}

bool AttPedalVis::ReadPedalVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToPedalVisForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttPedalVis::WritePedalVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = PedalVisFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttPedalVis::HasForm() const
{
    return (m_form != pedalVis_FORM_NONE);
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttQuilismaVis
//----------------------------------------------------------------------------

AttQuilismaVis::AttQuilismaVis() : Att()
{
    ResetQuilismaVis();
}

AttQuilismaVis::~AttQuilismaVis()
{
}

void AttQuilismaVis::ResetQuilismaVis()
{
    m_waves = 0;
}

bool AttQuilismaVis::ReadQuilismaVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("waves")) {
        this->SetWaves(StrToInt(element.attribute("waves").value()));
        element.remove_attribute("waves");
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
    return (m_waves != 0);
}

/* include <attwaves> */

//----------------------------------------------------------------------------
// AttSbVis
//----------------------------------------------------------------------------

AttSbVis::AttSbVis() : Att()
{
    ResetSbVis();
}

AttSbVis::~AttSbVis()
{
}

void AttSbVis::ResetSbVis()
{
    m_form = sbVis_FORM_NONE;
}

bool AttSbVis::ReadSbVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToSbVisForm(element.attribute("form").value()));
        element.remove_attribute("form");
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

/* include <attform> */

//----------------------------------------------------------------------------
// AttScoreDefVis
//----------------------------------------------------------------------------

AttScoreDefVis::AttScoreDefVis() : Att()
{
    ResetScoreDefVis();
}

AttScoreDefVis::~AttScoreDefVis()
{
}

void AttScoreDefVis::ResetScoreDefVis()
{
    m_vuHeight = "";
}

bool AttScoreDefVis::ReadScoreDefVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("vu.height")) {
        this->SetVuHeight(StrToStr(element.attribute("vu.height").value()));
        element.remove_attribute("vu.height");
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

/* include <attvu.height> */

//----------------------------------------------------------------------------
// AttSectionVis
//----------------------------------------------------------------------------

AttSectionVis::AttSectionVis() : Att()
{
    ResetSectionVis();
}

AttSectionVis::~AttSectionVis()
{
}

void AttSectionVis::ResetSectionVis()
{
    m_restart = BOOLEAN_NONE;
}

bool AttSectionVis::ReadSectionVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("restart")) {
        this->SetRestart(StrToBoolean(element.attribute("restart").value()));
        element.remove_attribute("restart");
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

/* include <attrestart> */

//----------------------------------------------------------------------------
// AttSignifLetVis
//----------------------------------------------------------------------------

AttSignifLetVis::AttSignifLetVis() : Att()
{
    ResetSignifLetVis();
}

AttSignifLetVis::~AttSignifLetVis()
{
}

void AttSignifLetVis::ResetSignifLetVis()
{
    m_place = data_EVENTREL();
}

bool AttSignifLetVis::ReadSignifLetVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("place")) {
        this->SetPlace(StrToEventrel(element.attribute("place").value()));
        element.remove_attribute("place");
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

/* include <attplace> */

//----------------------------------------------------------------------------
// AttSpaceVis
//----------------------------------------------------------------------------

AttSpaceVis::AttSpaceVis() : Att()
{
    ResetSpaceVis();
}

AttSpaceVis::~AttSpaceVis()
{
}

void AttSpaceVis::ResetSpaceVis()
{
    m_compressable = BOOLEAN_NONE;
}

bool AttSpaceVis::ReadSpaceVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("compressable")) {
        this->SetCompressable(StrToBoolean(element.attribute("compressable").value()));
        element.remove_attribute("compressable");
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

/* include <attcompressable> */

//----------------------------------------------------------------------------
// AttStaffDefVis
//----------------------------------------------------------------------------

AttStaffDefVis::AttStaffDefVis() : Att()
{
    ResetStaffDefVis();
}

AttStaffDefVis::~AttStaffDefVis()
{
}

void AttStaffDefVis::ResetStaffDefVis()
{
    m_gridShow = BOOLEAN_NONE;
    m_layerscheme = LAYERSCHEME_NONE;
    m_linesColor = "";
    m_linesVisible = BOOLEAN_NONE;
    m_spacing = VRV_UNSET;
}

bool AttStaffDefVis::ReadStaffDefVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("grid.show")) {
        this->SetGridShow(StrToBoolean(element.attribute("grid.show").value()));
        element.remove_attribute("grid.show");
        hasAttribute = true;
    }
    if (element.attribute("layerscheme")) {
        this->SetLayerscheme(StrToLayerscheme(element.attribute("layerscheme").value()));
        element.remove_attribute("layerscheme");
        hasAttribute = true;
    }
    if (element.attribute("lines.color")) {
        this->SetLinesColor(StrToStr(element.attribute("lines.color").value()));
        element.remove_attribute("lines.color");
        hasAttribute = true;
    }
    if (element.attribute("lines.visible")) {
        this->SetLinesVisible(StrToBoolean(element.attribute("lines.visible").value()));
        element.remove_attribute("lines.visible");
        hasAttribute = true;
    }
    if (element.attribute("spacing")) {
        this->SetSpacing(StrToMeasurementrel(element.attribute("spacing").value()));
        element.remove_attribute("spacing");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffDefVis::WriteStaffDefVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasGridShow()) {
        element.append_attribute("grid.show") = BooleanToStr(this->GetGridShow()).c_str();
        wroteAttribute = true;
    }
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
        element.append_attribute("spacing") = MeasurementrelToStr(this->GetSpacing()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffDefVis::HasGridShow() const
{
    return (m_gridShow != BOOLEAN_NONE);
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
    return (m_spacing != VRV_UNSET);
}

/* include <attspacing> */

//----------------------------------------------------------------------------
// AttStaffGrpVis
//----------------------------------------------------------------------------

AttStaffGrpVis::AttStaffGrpVis() : Att()
{
    ResetStaffGrpVis();
}

AttStaffGrpVis::~AttStaffGrpVis()
{
}

void AttStaffGrpVis::ResetStaffGrpVis()
{
    m_barThru = BOOLEAN_NONE;
}

bool AttStaffGrpVis::ReadStaffGrpVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("bar.thru")) {
        this->SetBarThru(StrToBoolean(element.attribute("bar.thru").value()));
        element.remove_attribute("bar.thru");
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

/* include <attbar.thru> */

//----------------------------------------------------------------------------
// AttTupletVis
//----------------------------------------------------------------------------

AttTupletVis::AttTupletVis() : Att()
{
    ResetTupletVis();
}

AttTupletVis::~AttTupletVis()
{
}

void AttTupletVis::ResetTupletVis()
{
    m_bracketPlace = STAFFREL_basic_NONE;
    m_bracketVisible = BOOLEAN_NONE;
    m_durVisible = BOOLEAN_NONE;
    m_numFormat = tupletVis_NUMFORMAT_NONE;
}

bool AttTupletVis::ReadTupletVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("bracket.place")) {
        this->SetBracketPlace(StrToStaffrelBasic(element.attribute("bracket.place").value()));
        element.remove_attribute("bracket.place");
        hasAttribute = true;
    }
    if (element.attribute("bracket.visible")) {
        this->SetBracketVisible(StrToBoolean(element.attribute("bracket.visible").value()));
        element.remove_attribute("bracket.visible");
        hasAttribute = true;
    }
    if (element.attribute("dur.visible")) {
        this->SetDurVisible(StrToBoolean(element.attribute("dur.visible").value()));
        element.remove_attribute("dur.visible");
        hasAttribute = true;
    }
    if (element.attribute("num.format")) {
        this->SetNumFormat(StrToTupletVisNumformat(element.attribute("num.format").value()));
        element.remove_attribute("num.format");
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

/* include <attnum.format> */

bool Att::SetVisual(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_ANNOTVIS)) {
        AttAnnotVis *att = dynamic_cast<AttAnnotVis *>(element);
        assert(att);
        if (attrType == "place") {
            att->SetPlace(att->StrToPlacement(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ARPEGVIS)) {
        AttArpegVis *att = dynamic_cast<AttArpegVis *>(element);
        assert(att);
        if (attrType == "arrow") {
            att->SetArrow(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "arrow.shape") {
            att->SetArrowShape(att->StrToLinestartendsymbol(attrValue));
            return true;
        }
        if (attrType == "arrow.size") {
            att->SetArrowSize(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "arrow.color") {
            att->SetArrowColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "arrow.fillcolor") {
            att->SetArrowFillcolor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "line.form") {
            att->SetLineForm(att->StrToLineform(attrValue));
            return true;
        }
        if (attrType == "line.width") {
            att->SetLineWidth(att->StrToLinewidth(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BARLINEVIS)) {
        AttBarLineVis *att = dynamic_cast<AttBarLineVis *>(element);
        assert(att);
        if (attrType == "len") {
            att->SetLen(att->StrToDbl(attrValue));
            return true;
        }
        if (attrType == "method") {
            att->SetMethod(att->StrToBarmethod(attrValue));
            return true;
        }
        if (attrType == "place") {
            att->SetPlace(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEAMINGVIS)) {
        AttBeamingVis *att = dynamic_cast<AttBeamingVis *>(element);
        assert(att);
        if (attrType == "beam.color") {
            att->SetBeamColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "beam.rend") {
            att->SetBeamRend(att->StrToBeamingVisBeamrend(attrValue));
            return true;
        }
        if (attrType == "beam.slope") {
            att->SetBeamSlope(att->StrToDbl(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_BEATRPTVIS)) {
        AttBeatRptVis *att = dynamic_cast<AttBeatRptVis *>(element);
        assert(att);
        if (attrType == "slash") {
            att->SetSlash(att->StrToBeatrptRend(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CHORDVIS)) {
        AttChordVis *att = dynamic_cast<AttChordVis *>(element);
        assert(att);
        if (attrType == "cluster") {
            att->SetCluster(att->StrToCluster(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CLEFFINGVIS)) {
        AttCleffingVis *att = dynamic_cast<AttCleffingVis *>(element);
        assert(att);
        if (attrType == "clef.color") {
            att->SetClefColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "clef.visible") {
            att->SetClefVisible(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_EPISEMAVIS)) {
        AttEpisemaVis *att = dynamic_cast<AttEpisemaVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToEpisemaVisForm(attrValue));
            return true;
        }
        if (attrType == "place") {
            att->SetPlace(att->StrToEventrel(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FTREMVIS)) {
        AttFTremVis *att = dynamic_cast<AttFTremVis *>(element);
        assert(att);
        if (attrType == "beams") {
            att->SetBeams(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "beams.float") {
            att->SetBeamsFloat(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "float.gap") {
            att->SetFloatGap(att->StrToMeasurementabs(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FERMATAVIS)) {
        AttFermataVis *att = dynamic_cast<AttFermataVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToFermataVisForm(attrValue));
            return true;
        }
        if (attrType == "shape") {
            att->SetShape(att->StrToFermataVisShape(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_FINGGRPVIS)) {
        AttFingGrpVis *att = dynamic_cast<AttFingGrpVis *>(element);
        assert(att);
        if (attrType == "orient") {
            att->SetOrient(att->StrToFingGrpVisOrient(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HAIRPINVIS)) {
        AttHairpinVis *att = dynamic_cast<AttHairpinVis *>(element);
        assert(att);
        if (attrType == "opening") {
            att->SetOpening(att->StrToMeasurementabs(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HARMVIS)) {
        AttHarmVis *att = dynamic_cast<AttHarmVis *>(element);
        assert(att);
        if (attrType == "rendgrid") {
            att->SetRendgrid(att->StrToHarmVisRendgrid(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_HISPANTICKVIS)) {
        AttHispanTickVis *att = dynamic_cast<AttHispanTickVis *>(element);
        assert(att);
        if (attrType == "place") {
            att->SetPlace(att->StrToEventrel(attrValue));
            return true;
        }
        if (attrType == "tilt") {
            att->SetTilt(att->StrToCompassdirection(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_KEYSIGVIS)) {
        AttKeySigVis *att = dynamic_cast<AttKeySigVis *>(element);
        assert(att);
        if (attrType == "sig.showchange") {
            att->SetSigShowchange(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_KEYSIGDEFAULTVIS)) {
        AttKeySigDefaultVis *att = dynamic_cast<AttKeySigDefaultVis *>(element);
        assert(att);
        if (attrType == "keysig.show") {
            att->SetKeysigShow(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "keysig.showchange") {
            att->SetKeysigShowchange(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LIGATUREVIS)) {
        AttLigatureVis *att = dynamic_cast<AttLigatureVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToLigatureform(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LINEVIS)) {
        AttLineVis *att = dynamic_cast<AttLineVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToLineform(attrValue));
            return true;
        }
        if (attrType == "width") {
            att->SetWidth(att->StrToLinewidth(attrValue));
            return true;
        }
        if (attrType == "endsym") {
            att->SetEndsym(att->StrToLinestartendsymbol(attrValue));
            return true;
        }
        if (attrType == "endsym.size") {
            att->SetEndsymSize(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "startsym") {
            att->SetStartsym(att->StrToLinestartendsymbol(attrValue));
            return true;
        }
        if (attrType == "startsym.size") {
            att->SetStartsymSize(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LIQUESCENTVIS)) {
        AttLiquescentVis *att = dynamic_cast<AttLiquescentVis *>(element);
        assert(att);
        if (attrType == "curve") {
            att->SetCurve(att->StrToLiquescentVisCurve(attrValue));
            return true;
        }
        if (attrType == "looped") {
            att->SetLooped(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MENSURVIS)) {
        AttMensurVis *att = dynamic_cast<AttMensurVis *>(element);
        assert(att);
        if (attrType == "dot") {
            att->SetDot(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "form") {
            att->SetForm(att->StrToMensurVisForm(attrValue));
            return true;
        }
        if (attrType == "orient") {
            att->SetOrient(att->StrToOrientation(attrValue));
            return true;
        }
        if (attrType == "sign") {
            att->SetSign(att->StrToMensurationsign(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MENSURALVIS)) {
        AttMensuralVis *att = dynamic_cast<AttMensuralVis *>(element);
        assert(att);
        if (attrType == "mensur.color") {
            att->SetMensurColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "mensur.dot") {
            att->SetMensurDot(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "mensur.form") {
            att->SetMensurForm(att->StrToMensuralVisMensurform(attrValue));
            return true;
        }
        if (attrType == "mensur.loc") {
            att->SetMensurLoc(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "mensur.orient") {
            att->SetMensurOrient(att->StrToOrientation(attrValue));
            return true;
        }
        if (attrType == "mensur.sign") {
            att->SetMensurSign(att->StrToMensurationsign(attrValue));
            return true;
        }
        if (attrType == "mensur.size") {
            att->SetMensurSize(att->StrToFontsize(attrValue));
            return true;
        }
        if (attrType == "mensur.slash") {
            att->SetMensurSlash(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERSIGVIS)) {
        AttMeterSigVis *att = dynamic_cast<AttMeterSigVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToMeterform(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERSIGDEFAULTVIS)) {
        AttMeterSigDefaultVis *att = dynamic_cast<AttMeterSigDefaultVis *>(element);
        assert(att);
        if (attrType == "meter.form") {
            att->SetMeterForm(att->StrToMeterform(attrValue));
            return true;
        }
        if (attrType == "meter.showchange") {
            att->SetMeterShowchange(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MULTIRESTVIS)) {
        AttMultiRestVis *att = dynamic_cast<AttMultiRestVis *>(element);
        assert(att);
        if (attrType == "block") {
            att->SetBlock(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PBVIS)) {
        AttPbVis *att = dynamic_cast<AttPbVis *>(element);
        assert(att);
        if (attrType == "folium") {
            att->SetFolium(att->StrToPbVisFolium(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_PEDALVIS)) {
        AttPedalVis *att = dynamic_cast<AttPedalVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToPedalVisForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_QUILISMAVIS)) {
        AttQuilismaVis *att = dynamic_cast<AttQuilismaVis *>(element);
        assert(att);
        if (attrType == "waves") {
            att->SetWaves(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SBVIS)) {
        AttSbVis *att = dynamic_cast<AttSbVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToSbVisForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SCOREDEFVIS)) {
        AttScoreDefVis *att = dynamic_cast<AttScoreDefVis *>(element);
        assert(att);
        if (attrType == "vu.height") {
            att->SetVuHeight(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SECTIONVIS)) {
        AttSectionVis *att = dynamic_cast<AttSectionVis *>(element);
        assert(att);
        if (attrType == "restart") {
            att->SetRestart(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SIGNIFLETVIS)) {
        AttSignifLetVis *att = dynamic_cast<AttSignifLetVis *>(element);
        assert(att);
        if (attrType == "place") {
            att->SetPlace(att->StrToEventrel(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SPACEVIS)) {
        AttSpaceVis *att = dynamic_cast<AttSpaceVis *>(element);
        assert(att);
        if (attrType == "compressable") {
            att->SetCompressable(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFDEFVIS)) {
        AttStaffDefVis *att = dynamic_cast<AttStaffDefVis *>(element);
        assert(att);
        if (attrType == "grid.show") {
            att->SetGridShow(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "layerscheme") {
            att->SetLayerscheme(att->StrToLayerscheme(attrValue));
            return true;
        }
        if (attrType == "lines.color") {
            att->SetLinesColor(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "lines.visible") {
            att->SetLinesVisible(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "spacing") {
            att->SetSpacing(att->StrToMeasurementrel(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFGRPVIS)) {
        AttStaffGrpVis *att = dynamic_cast<AttStaffGrpVis *>(element);
        assert(att);
        if (attrType == "bar.thru") {
            att->SetBarThru(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_TUPLETVIS)) {
        AttTupletVis *att = dynamic_cast<AttTupletVis *>(element);
        assert(att);
        if (attrType == "bracket.place") {
            att->SetBracketPlace(att->StrToStaffrelBasic(attrValue));
            return true;
        }
        if (attrType == "bracket.visible") {
            att->SetBracketVisible(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "dur.visible") {
            att->SetDurVisible(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "num.format") {
            att->SetNumFormat(att->StrToTupletVisNumformat(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetVisual(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ANNOTVIS)) {
        const AttAnnotVis *att = dynamic_cast<const AttAnnotVis *>(element);
        assert(att);
        if (att->HasPlace()) {
            attributes->push_back(std::make_pair("place", att->PlacementToStr(att->GetPlace())));
        }
    }
    if (element->HasAttClass(ATT_ARPEGVIS)) {
        const AttArpegVis *att = dynamic_cast<const AttArpegVis *>(element);
        assert(att);
        if (att->HasArrow()) {
            attributes->push_back(std::make_pair("arrow", att->BooleanToStr(att->GetArrow())));
        }
        if (att->HasArrowShape()) {
            attributes->push_back(std::make_pair("arrow.shape", att->LinestartendsymbolToStr(att->GetArrowShape())));
        }
        if (att->HasArrowSize()) {
            attributes->push_back(std::make_pair("arrow.size", att->IntToStr(att->GetArrowSize())));
        }
        if (att->HasArrowColor()) {
            attributes->push_back(std::make_pair("arrow.color", att->StrToStr(att->GetArrowColor())));
        }
        if (att->HasArrowFillcolor()) {
            attributes->push_back(std::make_pair("arrow.fillcolor", att->StrToStr(att->GetArrowFillcolor())));
        }
        if (att->HasLineForm()) {
            attributes->push_back(std::make_pair("line.form", att->LineformToStr(att->GetLineForm())));
        }
        if (att->HasLineWidth()) {
            attributes->push_back(std::make_pair("line.width", att->LinewidthToStr(att->GetLineWidth())));
        }
    }
    if (element->HasAttClass(ATT_BARLINEVIS)) {
        const AttBarLineVis *att = dynamic_cast<const AttBarLineVis *>(element);
        assert(att);
        if (att->HasLen()) {
            attributes->push_back(std::make_pair("len", att->DblToStr(att->GetLen())));
        }
        if (att->HasMethod()) {
            attributes->push_back(std::make_pair("method", att->BarmethodToStr(att->GetMethod())));
        }
        if (att->HasPlace()) {
            attributes->push_back(std::make_pair("place", att->IntToStr(att->GetPlace())));
        }
    }
    if (element->HasAttClass(ATT_BEAMINGVIS)) {
        const AttBeamingVis *att = dynamic_cast<const AttBeamingVis *>(element);
        assert(att);
        if (att->HasBeamColor()) {
            attributes->push_back(std::make_pair("beam.color", att->StrToStr(att->GetBeamColor())));
        }
        if (att->HasBeamRend()) {
            attributes->push_back(std::make_pair("beam.rend", att->BeamingVisBeamrendToStr(att->GetBeamRend())));
        }
        if (att->HasBeamSlope()) {
            attributes->push_back(std::make_pair("beam.slope", att->DblToStr(att->GetBeamSlope())));
        }
    }
    if (element->HasAttClass(ATT_BEATRPTVIS)) {
        const AttBeatRptVis *att = dynamic_cast<const AttBeatRptVis *>(element);
        assert(att);
        if (att->HasSlash()) {
            attributes->push_back(std::make_pair("slash", att->BeatrptRendToStr(att->GetSlash())));
        }
    }
    if (element->HasAttClass(ATT_CHORDVIS)) {
        const AttChordVis *att = dynamic_cast<const AttChordVis *>(element);
        assert(att);
        if (att->HasCluster()) {
            attributes->push_back(std::make_pair("cluster", att->ClusterToStr(att->GetCluster())));
        }
    }
    if (element->HasAttClass(ATT_CLEFFINGVIS)) {
        const AttCleffingVis *att = dynamic_cast<const AttCleffingVis *>(element);
        assert(att);
        if (att->HasClefColor()) {
            attributes->push_back(std::make_pair("clef.color", att->StrToStr(att->GetClefColor())));
        }
        if (att->HasClefVisible()) {
            attributes->push_back(std::make_pair("clef.visible", att->BooleanToStr(att->GetClefVisible())));
        }
    }
    if (element->HasAttClass(ATT_EPISEMAVIS)) {
        const AttEpisemaVis *att = dynamic_cast<const AttEpisemaVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->EpisemaVisFormToStr(att->GetForm())));
        }
        if (att->HasPlace()) {
            attributes->push_back(std::make_pair("place", att->EventrelToStr(att->GetPlace())));
        }
    }
    if (element->HasAttClass(ATT_FTREMVIS)) {
        const AttFTremVis *att = dynamic_cast<const AttFTremVis *>(element);
        assert(att);
        if (att->HasBeams()) {
            attributes->push_back(std::make_pair("beams", att->IntToStr(att->GetBeams())));
        }
        if (att->HasBeamsFloat()) {
            attributes->push_back(std::make_pair("beams.float", att->IntToStr(att->GetBeamsFloat())));
        }
        if (att->HasFloatGap()) {
            attributes->push_back(std::make_pair("float.gap", att->MeasurementabsToStr(att->GetFloatGap())));
        }
    }
    if (element->HasAttClass(ATT_FERMATAVIS)) {
        const AttFermataVis *att = dynamic_cast<const AttFermataVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->FermataVisFormToStr(att->GetForm())));
        }
        if (att->HasShape()) {
            attributes->push_back(std::make_pair("shape", att->FermataVisShapeToStr(att->GetShape())));
        }
    }
    if (element->HasAttClass(ATT_FINGGRPVIS)) {
        const AttFingGrpVis *att = dynamic_cast<const AttFingGrpVis *>(element);
        assert(att);
        if (att->HasOrient()) {
            attributes->push_back(std::make_pair("orient", att->FingGrpVisOrientToStr(att->GetOrient())));
        }
    }
    if (element->HasAttClass(ATT_HAIRPINVIS)) {
        const AttHairpinVis *att = dynamic_cast<const AttHairpinVis *>(element);
        assert(att);
        if (att->HasOpening()) {
            attributes->push_back(std::make_pair("opening", att->MeasurementabsToStr(att->GetOpening())));
        }
    }
    if (element->HasAttClass(ATT_HARMVIS)) {
        const AttHarmVis *att = dynamic_cast<const AttHarmVis *>(element);
        assert(att);
        if (att->HasRendgrid()) {
            attributes->push_back(std::make_pair("rendgrid", att->HarmVisRendgridToStr(att->GetRendgrid())));
        }
    }
    if (element->HasAttClass(ATT_HISPANTICKVIS)) {
        const AttHispanTickVis *att = dynamic_cast<const AttHispanTickVis *>(element);
        assert(att);
        if (att->HasPlace()) {
            attributes->push_back(std::make_pair("place", att->EventrelToStr(att->GetPlace())));
        }
        if (att->HasTilt()) {
            attributes->push_back(std::make_pair("tilt", att->CompassdirectionToStr(att->GetTilt())));
        }
    }
    if (element->HasAttClass(ATT_KEYSIGVIS)) {
        const AttKeySigVis *att = dynamic_cast<const AttKeySigVis *>(element);
        assert(att);
        if (att->HasSigShowchange()) {
            attributes->push_back(std::make_pair("sig.showchange", att->BooleanToStr(att->GetSigShowchange())));
        }
    }
    if (element->HasAttClass(ATT_KEYSIGDEFAULTVIS)) {
        const AttKeySigDefaultVis *att = dynamic_cast<const AttKeySigDefaultVis *>(element);
        assert(att);
        if (att->HasKeysigShow()) {
            attributes->push_back(std::make_pair("keysig.show", att->BooleanToStr(att->GetKeysigShow())));
        }
        if (att->HasKeysigShowchange()) {
            attributes->push_back(std::make_pair("keysig.showchange", att->BooleanToStr(att->GetKeysigShowchange())));
        }
    }
    if (element->HasAttClass(ATT_LIGATUREVIS)) {
        const AttLigatureVis *att = dynamic_cast<const AttLigatureVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->LigatureformToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass(ATT_LINEVIS)) {
        const AttLineVis *att = dynamic_cast<const AttLineVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->LineformToStr(att->GetForm())));
        }
        if (att->HasWidth()) {
            attributes->push_back(std::make_pair("width", att->LinewidthToStr(att->GetWidth())));
        }
        if (att->HasEndsym()) {
            attributes->push_back(std::make_pair("endsym", att->LinestartendsymbolToStr(att->GetEndsym())));
        }
        if (att->HasEndsymSize()) {
            attributes->push_back(std::make_pair("endsym.size", att->IntToStr(att->GetEndsymSize())));
        }
        if (att->HasStartsym()) {
            attributes->push_back(std::make_pair("startsym", att->LinestartendsymbolToStr(att->GetStartsym())));
        }
        if (att->HasStartsymSize()) {
            attributes->push_back(std::make_pair("startsym.size", att->IntToStr(att->GetStartsymSize())));
        }
    }
    if (element->HasAttClass(ATT_LIQUESCENTVIS)) {
        const AttLiquescentVis *att = dynamic_cast<const AttLiquescentVis *>(element);
        assert(att);
        if (att->HasCurve()) {
            attributes->push_back(std::make_pair("curve", att->LiquescentVisCurveToStr(att->GetCurve())));
        }
        if (att->HasLooped()) {
            attributes->push_back(std::make_pair("looped", att->BooleanToStr(att->GetLooped())));
        }
    }
    if (element->HasAttClass(ATT_MENSURVIS)) {
        const AttMensurVis *att = dynamic_cast<const AttMensurVis *>(element);
        assert(att);
        if (att->HasDot()) {
            attributes->push_back(std::make_pair("dot", att->BooleanToStr(att->GetDot())));
        }
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->MensurVisFormToStr(att->GetForm())));
        }
        if (att->HasOrient()) {
            attributes->push_back(std::make_pair("orient", att->OrientationToStr(att->GetOrient())));
        }
        if (att->HasSign()) {
            attributes->push_back(std::make_pair("sign", att->MensurationsignToStr(att->GetSign())));
        }
    }
    if (element->HasAttClass(ATT_MENSURALVIS)) {
        const AttMensuralVis *att = dynamic_cast<const AttMensuralVis *>(element);
        assert(att);
        if (att->HasMensurColor()) {
            attributes->push_back(std::make_pair("mensur.color", att->StrToStr(att->GetMensurColor())));
        }
        if (att->HasMensurDot()) {
            attributes->push_back(std::make_pair("mensur.dot", att->BooleanToStr(att->GetMensurDot())));
        }
        if (att->HasMensurForm()) {
            attributes->push_back(std::make_pair("mensur.form", att->MensuralVisMensurformToStr(att->GetMensurForm())));
        }
        if (att->HasMensurLoc()) {
            attributes->push_back(std::make_pair("mensur.loc", att->IntToStr(att->GetMensurLoc())));
        }
        if (att->HasMensurOrient()) {
            attributes->push_back(std::make_pair("mensur.orient", att->OrientationToStr(att->GetMensurOrient())));
        }
        if (att->HasMensurSign()) {
            attributes->push_back(std::make_pair("mensur.sign", att->MensurationsignToStr(att->GetMensurSign())));
        }
        if (att->HasMensurSize()) {
            attributes->push_back(std::make_pair("mensur.size", att->FontsizeToStr(att->GetMensurSize())));
        }
        if (att->HasMensurSlash()) {
            attributes->push_back(std::make_pair("mensur.slash", att->IntToStr(att->GetMensurSlash())));
        }
    }
    if (element->HasAttClass(ATT_METERSIGVIS)) {
        const AttMeterSigVis *att = dynamic_cast<const AttMeterSigVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->MeterformToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass(ATT_METERSIGDEFAULTVIS)) {
        const AttMeterSigDefaultVis *att = dynamic_cast<const AttMeterSigDefaultVis *>(element);
        assert(att);
        if (att->HasMeterForm()) {
            attributes->push_back(std::make_pair("meter.form", att->MeterformToStr(att->GetMeterForm())));
        }
        if (att->HasMeterShowchange()) {
            attributes->push_back(std::make_pair("meter.showchange", att->BooleanToStr(att->GetMeterShowchange())));
        }
    }
    if (element->HasAttClass(ATT_MULTIRESTVIS)) {
        const AttMultiRestVis *att = dynamic_cast<const AttMultiRestVis *>(element);
        assert(att);
        if (att->HasBlock()) {
            attributes->push_back(std::make_pair("block", att->BooleanToStr(att->GetBlock())));
        }
    }
    if (element->HasAttClass(ATT_PBVIS)) {
        const AttPbVis *att = dynamic_cast<const AttPbVis *>(element);
        assert(att);
        if (att->HasFolium()) {
            attributes->push_back(std::make_pair("folium", att->PbVisFoliumToStr(att->GetFolium())));
        }
    }
    if (element->HasAttClass(ATT_PEDALVIS)) {
        const AttPedalVis *att = dynamic_cast<const AttPedalVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->PedalVisFormToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass(ATT_QUILISMAVIS)) {
        const AttQuilismaVis *att = dynamic_cast<const AttQuilismaVis *>(element);
        assert(att);
        if (att->HasWaves()) {
            attributes->push_back(std::make_pair("waves", att->IntToStr(att->GetWaves())));
        }
    }
    if (element->HasAttClass(ATT_SBVIS)) {
        const AttSbVis *att = dynamic_cast<const AttSbVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->SbVisFormToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass(ATT_SCOREDEFVIS)) {
        const AttScoreDefVis *att = dynamic_cast<const AttScoreDefVis *>(element);
        assert(att);
        if (att->HasVuHeight()) {
            attributes->push_back(std::make_pair("vu.height", att->StrToStr(att->GetVuHeight())));
        }
    }
    if (element->HasAttClass(ATT_SECTIONVIS)) {
        const AttSectionVis *att = dynamic_cast<const AttSectionVis *>(element);
        assert(att);
        if (att->HasRestart()) {
            attributes->push_back(std::make_pair("restart", att->BooleanToStr(att->GetRestart())));
        }
    }
    if (element->HasAttClass(ATT_SIGNIFLETVIS)) {
        const AttSignifLetVis *att = dynamic_cast<const AttSignifLetVis *>(element);
        assert(att);
        if (att->HasPlace()) {
            attributes->push_back(std::make_pair("place", att->EventrelToStr(att->GetPlace())));
        }
    }
    if (element->HasAttClass(ATT_SPACEVIS)) {
        const AttSpaceVis *att = dynamic_cast<const AttSpaceVis *>(element);
        assert(att);
        if (att->HasCompressable()) {
            attributes->push_back(std::make_pair("compressable", att->BooleanToStr(att->GetCompressable())));
        }
    }
    if (element->HasAttClass(ATT_STAFFDEFVIS)) {
        const AttStaffDefVis *att = dynamic_cast<const AttStaffDefVis *>(element);
        assert(att);
        if (att->HasGridShow()) {
            attributes->push_back(std::make_pair("grid.show", att->BooleanToStr(att->GetGridShow())));
        }
        if (att->HasLayerscheme()) {
            attributes->push_back(std::make_pair("layerscheme", att->LayerschemeToStr(att->GetLayerscheme())));
        }
        if (att->HasLinesColor()) {
            attributes->push_back(std::make_pair("lines.color", att->StrToStr(att->GetLinesColor())));
        }
        if (att->HasLinesVisible()) {
            attributes->push_back(std::make_pair("lines.visible", att->BooleanToStr(att->GetLinesVisible())));
        }
        if (att->HasSpacing()) {
            attributes->push_back(std::make_pair("spacing", att->MeasurementrelToStr(att->GetSpacing())));
        }
    }
    if (element->HasAttClass(ATT_STAFFGRPVIS)) {
        const AttStaffGrpVis *att = dynamic_cast<const AttStaffGrpVis *>(element);
        assert(att);
        if (att->HasBarThru()) {
            attributes->push_back(std::make_pair("bar.thru", att->BooleanToStr(att->GetBarThru())));
        }
    }
    if (element->HasAttClass(ATT_TUPLETVIS)) {
        const AttTupletVis *att = dynamic_cast<const AttTupletVis *>(element);
        assert(att);
        if (att->HasBracketPlace()) {
            attributes->push_back(std::make_pair("bracket.place", att->StaffrelBasicToStr(att->GetBracketPlace())));
        }
        if (att->HasBracketVisible()) {
            attributes->push_back(std::make_pair("bracket.visible", att->BooleanToStr(att->GetBracketVisible())));
        }
        if (att->HasDurVisible()) {
            attributes->push_back(std::make_pair("dur.visible", att->BooleanToStr(att->GetDurVisible())));
        }
        if (att->HasNumFormat()) {
            attributes->push_back(std::make_pair("num.format", att->TupletVisNumformatToStr(att->GetNumFormat())));
        }
    }
}

} // vrv namespace
