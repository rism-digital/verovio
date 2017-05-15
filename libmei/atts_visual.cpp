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
    m_place = PLACEMENT_NONE;
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
    return (m_place != PLACEMENT_NONE);
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
    m_arrowSize = FONTSIZESCALE_NONE;
    m_arrowColor = "";
    m_arrowFillcolor = "";
    m_lineForm = LINEFORM_NONE;
    m_lineWidth = "";
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
        this->SetArrowSize(StrToFontsizescale(element.attribute("arrow.size").value()));
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
        this->SetLineWidth(StrToStr(element.attribute("line.width").value()));
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
        element.append_attribute("arrow.size") = FontsizescaleToStr(this->GetArrowSize()).c_str();
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
        element.append_attribute("line.width") = StrToStr(this->GetLineWidth()).c_str();
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
    return (m_arrowSize != FONTSIZESCALE_NONE);
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
    return (m_lineWidth != "");
}

/* include <attline.width> */

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
    m_beamsFloat = 0;
    m_floatGap = "";
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
        this->SetFloatGap(StrToStr(element.attribute("float.gap").value()));
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
        element.append_attribute("float.gap") = StrToStr(this->GetFloatGap()).c_str();
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
    return (m_beamsFloat != 0);
}

bool AttFTremVis::HasFloatGap() const
{
    return (m_floatGap != "");
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
    m_opening = "";
}

bool AttHairpinVis::ReadHairpinVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("opening")) {
        this->SetOpening(StrToStr(element.attribute("opening").value()));
        element.remove_attribute("opening");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHairpinVis::WriteHairpinVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasOpening()) {
        element.append_attribute("opening") = StrToStr(this->GetOpening()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHairpinVis::HasOpening() const
{
    return (m_opening != "");
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
    m_width = "";
    m_endsym = LINESTARTENDSYMBOL_NONE;
    m_endsymSize = FONTSIZESCALE_NONE;
    m_startsym = LINESTARTENDSYMBOL_NONE;
    m_startsymSize = FONTSIZESCALE_NONE;
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
        this->SetWidth(StrToStr(element.attribute("width").value()));
        element.remove_attribute("width");
        hasAttribute = true;
    }
    if (element.attribute("endsym")) {
        this->SetEndsym(StrToLinestartendsymbol(element.attribute("endsym").value()));
        element.remove_attribute("endsym");
        hasAttribute = true;
    }
    if (element.attribute("endsym.size")) {
        this->SetEndsymSize(StrToFontsizescale(element.attribute("endsym.size").value()));
        element.remove_attribute("endsym.size");
        hasAttribute = true;
    }
    if (element.attribute("startsym")) {
        this->SetStartsym(StrToLinestartendsymbol(element.attribute("startsym").value()));
        element.remove_attribute("startsym");
        hasAttribute = true;
    }
    if (element.attribute("startsym.size")) {
        this->SetStartsymSize(StrToFontsizescale(element.attribute("startsym.size").value()));
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
        element.append_attribute("width") = StrToStr(this->GetWidth()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndsym()) {
        element.append_attribute("endsym") = LinestartendsymbolToStr(this->GetEndsym()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEndsymSize()) {
        element.append_attribute("endsym.size") = FontsizescaleToStr(this->GetEndsymSize()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStartsym()) {
        element.append_attribute("startsym") = LinestartendsymbolToStr(this->GetStartsym()).c_str();
        wroteAttribute = true;
    }
    if (this->HasStartsymSize()) {
        element.append_attribute("startsym.size") = FontsizescaleToStr(this->GetStartsymSize()).c_str();
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
    return (m_width != "");
}

bool AttLineVis::HasEndsym() const
{
    return (m_endsym != LINESTARTENDSYMBOL_NONE);
}

bool AttLineVis::HasEndsymSize() const
{
    return (m_endsymSize != FONTSIZESCALE_NONE);
}

bool AttLineVis::HasStartsym() const
{
    return (m_startsym != LINESTARTENDSYMBOL_NONE);
}

bool AttLineVis::HasStartsymSize() const
{
    return (m_startsymSize != FONTSIZESCALE_NONE);
}

/* include <attstartsym.size> */

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
    m_form = mensurVis_FORM_NONE;
    m_orient = ORIENTATION_NONE;
}

bool AttMensurVis::ReadMensurVis(pugi::xml_node element)
{
    bool hasAttribute = false;
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
    return hasAttribute;
}

bool AttMensurVis::WriteMensurVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = MensurVisFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOrient()) {
        element.append_attribute("orient") = OrientationToStr(this->GetOrient()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensurVis::HasForm() const
{
    return (m_form != mensurVis_FORM_NONE);
}

bool AttMensurVis::HasOrient() const
{
    return (m_orient != ORIENTATION_NONE);
}

/* include <attorient> */

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
    m_mensurForm = mensuralVis_MENSURFORM_NONE;
    m_mensurLoc = 0;
    m_mensurOrient = ORIENTATION_NONE;
    m_mensurSize = 0;
}

bool AttMensuralVis::ReadMensuralVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("mensur.color")) {
        this->SetMensurColor(StrToStr(element.attribute("mensur.color").value()));
        element.remove_attribute("mensur.color");
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
    if (element.attribute("mensur.size")) {
        this->SetMensurSize(StrToInt(element.attribute("mensur.size").value()));
        element.remove_attribute("mensur.size");
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
    if (this->HasMensurSize()) {
        element.append_attribute("mensur.size") = IntToStr(this->GetMensurSize()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMensuralVis::HasMensurColor() const
{
    return (m_mensurColor != "");
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

bool AttMensuralVis::HasMensurSize() const
{
    return (m_mensurSize != 0);
}

/* include <attmensur.size> */

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
    m_form = meterSigVis_FORM_NONE;
}

bool AttMeterSigVis::ReadMeterSigVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToMeterSigVisForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMeterSigVis::WriteMeterSigVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = MeterSigVisFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterSigVis::HasForm() const
{
    return (m_form != meterSigVis_FORM_NONE);
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
    m_meterRend = meterSigDefaultVis_METERREND_NONE;
    m_meterShowchange = BOOLEAN_NONE;
}

bool AttMeterSigDefaultVis::ReadMeterSigDefaultVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("meter.rend")) {
        this->SetMeterRend(StrToMeterSigDefaultVisMeterrend(element.attribute("meter.rend").value()));
        element.remove_attribute("meter.rend");
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
    if (this->HasMeterRend()) {
        element.append_attribute("meter.rend") = MeterSigDefaultVisMeterrendToStr(this->GetMeterRend()).c_str();
        wroteAttribute = true;
    }
    if (this->HasMeterShowchange()) {
        element.append_attribute("meter.showchange") = BooleanToStr(this->GetMeterShowchange()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMeterSigDefaultVis::HasMeterRend() const
{
    return (m_meterRend != meterSigDefaultVis_METERREND_NONE);
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
// AttNcVis
//----------------------------------------------------------------------------

AttNcVis::AttNcVis() : Att()
{
    ResetNcVis();
}

AttNcVis::~AttNcVis()
{
}

void AttNcVis::ResetNcVis()
{
    m_angled = BOOLEAN_NONE;
    m_con = ncVis_CON_NONE;
    m_curved = ncVis_CURVED_NONE;
    m_diagonalright = ncVis_DIAGONALRIGHT_NONE;
    m_episima = BOOLEAN_NONE;
    m_extended = BOOLEAN_NONE;
    m_flat = BOOLEAN_NONE;
    m_jagged = BOOLEAN_NONE;
    m_liquescent = BOOLEAN_NONE;
    m_long = BOOLEAN_NONE;
    m_oriscus = ncVis_ORISCUS_NONE;
    m_quilisma = ncVis_QUILISMA_NONE;
    m_wavy = BOOLEAN_NONE;
}

bool AttNcVis::ReadNcVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("angled")) {
        this->SetAngled(StrToBoolean(element.attribute("angled").value()));
        element.remove_attribute("angled");
        hasAttribute = true;
    }
    if (element.attribute("con")) {
        this->SetCon(StrToNcVisCon(element.attribute("con").value()));
        element.remove_attribute("con");
        hasAttribute = true;
    }
    if (element.attribute("curved")) {
        this->SetCurved(StrToNcVisCurved(element.attribute("curved").value()));
        element.remove_attribute("curved");
        hasAttribute = true;
    }
    if (element.attribute("diagonalright")) {
        this->SetDiagonalright(StrToNcVisDiagonalright(element.attribute("diagonalright").value()));
        element.remove_attribute("diagonalright");
        hasAttribute = true;
    }
    if (element.attribute("episima")) {
        this->SetEpisima(StrToBoolean(element.attribute("episima").value()));
        element.remove_attribute("episima");
        hasAttribute = true;
    }
    if (element.attribute("extended")) {
        this->SetExtended(StrToBoolean(element.attribute("extended").value()));
        element.remove_attribute("extended");
        hasAttribute = true;
    }
    if (element.attribute("flat")) {
        this->SetFlat(StrToBoolean(element.attribute("flat").value()));
        element.remove_attribute("flat");
        hasAttribute = true;
    }
    if (element.attribute("jagged")) {
        this->SetJagged(StrToBoolean(element.attribute("jagged").value()));
        element.remove_attribute("jagged");
        hasAttribute = true;
    }
    if (element.attribute("liquescent")) {
        this->SetLiquescent(StrToBoolean(element.attribute("liquescent").value()));
        element.remove_attribute("liquescent");
        hasAttribute = true;
    }
    if (element.attribute("long")) {
        this->SetLong(StrToBoolean(element.attribute("long").value()));
        element.remove_attribute("long");
        hasAttribute = true;
    }
    if (element.attribute("oriscus")) {
        this->SetOriscus(StrToNcVisOriscus(element.attribute("oriscus").value()));
        element.remove_attribute("oriscus");
        hasAttribute = true;
    }
    if (element.attribute("quilisma")) {
        this->SetQuilisma(StrToNcVisQuilisma(element.attribute("quilisma").value()));
        element.remove_attribute("quilisma");
        hasAttribute = true;
    }
    if (element.attribute("wavy")) {
        this->SetWavy(StrToBoolean(element.attribute("wavy").value()));
        element.remove_attribute("wavy");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNcVis::WriteNcVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAngled()) {
        element.append_attribute("angled") = BooleanToStr(this->GetAngled()).c_str();
        wroteAttribute = true;
    }
    if (this->HasCon()) {
        element.append_attribute("con") = NcVisConToStr(this->GetCon()).c_str();
        wroteAttribute = true;
    }
    if (this->HasCurved()) {
        element.append_attribute("curved") = NcVisCurvedToStr(this->GetCurved()).c_str();
        wroteAttribute = true;
    }
    if (this->HasDiagonalright()) {
        element.append_attribute("diagonalright") = NcVisDiagonalrightToStr(this->GetDiagonalright()).c_str();
        wroteAttribute = true;
    }
    if (this->HasEpisima()) {
        element.append_attribute("episima") = BooleanToStr(this->GetEpisima()).c_str();
        wroteAttribute = true;
    }
    if (this->HasExtended()) {
        element.append_attribute("extended") = BooleanToStr(this->GetExtended()).c_str();
        wroteAttribute = true;
    }
    if (this->HasFlat()) {
        element.append_attribute("flat") = BooleanToStr(this->GetFlat()).c_str();
        wroteAttribute = true;
    }
    if (this->HasJagged()) {
        element.append_attribute("jagged") = BooleanToStr(this->GetJagged()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLiquescent()) {
        element.append_attribute("liquescent") = BooleanToStr(this->GetLiquescent()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLong()) {
        element.append_attribute("long") = BooleanToStr(this->GetLong()).c_str();
        wroteAttribute = true;
    }
    if (this->HasOriscus()) {
        element.append_attribute("oriscus") = NcVisOriscusToStr(this->GetOriscus()).c_str();
        wroteAttribute = true;
    }
    if (this->HasQuilisma()) {
        element.append_attribute("quilisma") = NcVisQuilismaToStr(this->GetQuilisma()).c_str();
        wroteAttribute = true;
    }
    if (this->HasWavy()) {
        element.append_attribute("wavy") = BooleanToStr(this->GetWavy()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNcVis::HasAngled() const
{
    return (m_angled != BOOLEAN_NONE);
}

bool AttNcVis::HasCon() const
{
    return (m_con != ncVis_CON_NONE);
}

bool AttNcVis::HasCurved() const
{
    return (m_curved != ncVis_CURVED_NONE);
}

bool AttNcVis::HasDiagonalright() const
{
    return (m_diagonalright != ncVis_DIAGONALRIGHT_NONE);
}

bool AttNcVis::HasEpisima() const
{
    return (m_episima != BOOLEAN_NONE);
}

bool AttNcVis::HasExtended() const
{
    return (m_extended != BOOLEAN_NONE);
}

bool AttNcVis::HasFlat() const
{
    return (m_flat != BOOLEAN_NONE);
}

bool AttNcVis::HasJagged() const
{
    return (m_jagged != BOOLEAN_NONE);
}

bool AttNcVis::HasLiquescent() const
{
    return (m_liquescent != BOOLEAN_NONE);
}

bool AttNcVis::HasLong() const
{
    return (m_long != BOOLEAN_NONE);
}

bool AttNcVis::HasOriscus() const
{
    return (m_oriscus != ncVis_ORISCUS_NONE);
}

bool AttNcVis::HasQuilisma() const
{
    return (m_quilisma != ncVis_QUILISMA_NONE);
}

bool AttNcVis::HasWavy() const
{
    return (m_wavy != BOOLEAN_NONE);
}

/* include <attwavy> */

//----------------------------------------------------------------------------
// AttNeumeVis
//----------------------------------------------------------------------------

AttNeumeVis::AttNeumeVis() : Att()
{
    ResetNeumeVis();
}

AttNeumeVis::~AttNeumeVis()
{
}

void AttNeumeVis::ResetNeumeVis()
{
    m_hispanicloop = BOOLEAN_NONE;
    m_significativeletters = BOOLEAN_NONE;
}

bool AttNeumeVis::ReadNeumeVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("hispanicloop")) {
        this->SetHispanicloop(StrToBoolean(element.attribute("hispanicloop").value()));
        element.remove_attribute("hispanicloop");
        hasAttribute = true;
    }
    if (element.attribute("significativeletters")) {
        this->SetSignificativeletters(StrToBoolean(element.attribute("significativeletters").value()));
        element.remove_attribute("significativeletters");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNeumeVis::WriteNeumeVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasHispanicloop()) {
        element.append_attribute("hispanicloop") = BooleanToStr(this->GetHispanicloop()).c_str();
        wroteAttribute = true;
    }
    if (this->HasSignificativeletters()) {
        element.append_attribute("significativeletters") = BooleanToStr(this->GetSignificativeletters()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNeumeVis::HasHispanicloop() const
{
    return (m_hispanicloop != BOOLEAN_NONE);
}

bool AttNeumeVis::HasSignificativeletters() const
{
    return (m_significativeletters != BOOLEAN_NONE);
}

/* include <attsignificativeletters> */

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
    m_spacing = "";
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
        this->SetSpacing(StrToStr(element.attribute("spacing").value()));
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
        element.append_attribute("spacing") = StrToStr(this->GetSpacing()).c_str();
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
    return (m_spacing != "");
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
    m_barthru = BOOLEAN_NONE;
}

bool AttStaffGrpVis::ReadStaffGrpVis(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("barthru")) {
        this->SetBarthru(StrToBoolean(element.attribute("barthru").value()));
        element.remove_attribute("barthru");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffGrpVis::WriteStaffGrpVis(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasBarthru()) {
        element.append_attribute("barthru") = BooleanToStr(this->GetBarthru()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffGrpVis::HasBarthru() const
{
    return (m_barthru != BOOLEAN_NONE);
}

/* include <attbarthru> */

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

bool Att::SetVisual(Object *element, std::string attrType, std::string attrValue)
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
            att->SetArrowSize(att->StrToFontsizescale(attrValue));
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
            att->SetLineWidth(att->StrToStr(attrValue));
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
            att->SetFloatGap(att->StrToStr(attrValue));
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
            att->SetOpening(att->StrToStr(attrValue));
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
    if (element->HasAttClass(ATT_LINEVIS)) {
        AttLineVis *att = dynamic_cast<AttLineVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToLineform(attrValue));
            return true;
        }
        if (attrType == "width") {
            att->SetWidth(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "endsym") {
            att->SetEndsym(att->StrToLinestartendsymbol(attrValue));
            return true;
        }
        if (attrType == "endsym.size") {
            att->SetEndsymSize(att->StrToFontsizescale(attrValue));
            return true;
        }
        if (attrType == "startsym") {
            att->SetStartsym(att->StrToLinestartendsymbol(attrValue));
            return true;
        }
        if (attrType == "startsym.size") {
            att->SetStartsymSize(att->StrToFontsizescale(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_MENSURVIS)) {
        AttMensurVis *att = dynamic_cast<AttMensurVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToMensurVisForm(attrValue));
            return true;
        }
        if (attrType == "orient") {
            att->SetOrient(att->StrToOrientation(attrValue));
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
        if (attrType == "mensur.size") {
            att->SetMensurSize(att->StrToInt(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERSIGVIS)) {
        AttMeterSigVis *att = dynamic_cast<AttMeterSigVis *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToMeterSigVisForm(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_METERSIGDEFAULTVIS)) {
        AttMeterSigDefaultVis *att = dynamic_cast<AttMeterSigDefaultVis *>(element);
        assert(att);
        if (attrType == "meter.rend") {
            att->SetMeterRend(att->StrToMeterSigDefaultVisMeterrend(attrValue));
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
    if (element->HasAttClass(ATT_NCVIS)) {
        AttNcVis *att = dynamic_cast<AttNcVis *>(element);
        assert(att);
        if (attrType == "angled") {
            att->SetAngled(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "con") {
            att->SetCon(att->StrToNcVisCon(attrValue));
            return true;
        }
        if (attrType == "curved") {
            att->SetCurved(att->StrToNcVisCurved(attrValue));
            return true;
        }
        if (attrType == "diagonalright") {
            att->SetDiagonalright(att->StrToNcVisDiagonalright(attrValue));
            return true;
        }
        if (attrType == "episima") {
            att->SetEpisima(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "extended") {
            att->SetExtended(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "flat") {
            att->SetFlat(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "jagged") {
            att->SetJagged(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "liquescent") {
            att->SetLiquescent(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "long") {
            att->SetLong(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "oriscus") {
            att->SetOriscus(att->StrToNcVisOriscus(attrValue));
            return true;
        }
        if (attrType == "quilisma") {
            att->SetQuilisma(att->StrToNcVisQuilisma(attrValue));
            return true;
        }
        if (attrType == "wavy") {
            att->SetWavy(att->StrToBoolean(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_NEUMEVIS)) {
        AttNeumeVis *att = dynamic_cast<AttNeumeVis *>(element);
        assert(att);
        if (attrType == "hispanicloop") {
            att->SetHispanicloop(att->StrToBoolean(attrValue));
            return true;
        }
        if (attrType == "significativeletters") {
            att->SetSignificativeletters(att->StrToBoolean(attrValue));
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
            att->SetSpacing(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFGRPVIS)) {
        AttStaffGrpVis *att = dynamic_cast<AttStaffGrpVis *>(element);
        assert(att);
        if (attrType == "barthru") {
            att->SetBarthru(att->StrToBoolean(attrValue));
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
            attributes->push_back(std::make_pair("arrow.size", att->FontsizescaleToStr(att->GetArrowSize())));
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
            attributes->push_back(std::make_pair("line.width", att->StrToStr(att->GetLineWidth())));
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
            attributes->push_back(std::make_pair("float.gap", att->StrToStr(att->GetFloatGap())));
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
            attributes->push_back(std::make_pair("opening", att->StrToStr(att->GetOpening())));
        }
    }
    if (element->HasAttClass(ATT_HARMVIS)) {
        const AttHarmVis *att = dynamic_cast<const AttHarmVis *>(element);
        assert(att);
        if (att->HasRendgrid()) {
            attributes->push_back(std::make_pair("rendgrid", att->HarmVisRendgridToStr(att->GetRendgrid())));
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
    if (element->HasAttClass(ATT_LINEVIS)) {
        const AttLineVis *att = dynamic_cast<const AttLineVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->LineformToStr(att->GetForm())));
        }
        if (att->HasWidth()) {
            attributes->push_back(std::make_pair("width", att->StrToStr(att->GetWidth())));
        }
        if (att->HasEndsym()) {
            attributes->push_back(std::make_pair("endsym", att->LinestartendsymbolToStr(att->GetEndsym())));
        }
        if (att->HasEndsymSize()) {
            attributes->push_back(std::make_pair("endsym.size", att->FontsizescaleToStr(att->GetEndsymSize())));
        }
        if (att->HasStartsym()) {
            attributes->push_back(std::make_pair("startsym", att->LinestartendsymbolToStr(att->GetStartsym())));
        }
        if (att->HasStartsymSize()) {
            attributes->push_back(std::make_pair("startsym.size", att->FontsizescaleToStr(att->GetStartsymSize())));
        }
    }
    if (element->HasAttClass(ATT_MENSURVIS)) {
        const AttMensurVis *att = dynamic_cast<const AttMensurVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->MensurVisFormToStr(att->GetForm())));
        }
        if (att->HasOrient()) {
            attributes->push_back(std::make_pair("orient", att->OrientationToStr(att->GetOrient())));
        }
    }
    if (element->HasAttClass(ATT_MENSURALVIS)) {
        const AttMensuralVis *att = dynamic_cast<const AttMensuralVis *>(element);
        assert(att);
        if (att->HasMensurColor()) {
            attributes->push_back(std::make_pair("mensur.color", att->StrToStr(att->GetMensurColor())));
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
        if (att->HasMensurSize()) {
            attributes->push_back(std::make_pair("mensur.size", att->IntToStr(att->GetMensurSize())));
        }
    }
    if (element->HasAttClass(ATT_METERSIGVIS)) {
        const AttMeterSigVis *att = dynamic_cast<const AttMeterSigVis *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->MeterSigVisFormToStr(att->GetForm())));
        }
    }
    if (element->HasAttClass(ATT_METERSIGDEFAULTVIS)) {
        const AttMeterSigDefaultVis *att = dynamic_cast<const AttMeterSigDefaultVis *>(element);
        assert(att);
        if (att->HasMeterRend()) {
            attributes->push_back(std::make_pair("meter.rend", att->MeterSigDefaultVisMeterrendToStr(att->GetMeterRend())));
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
    if (element->HasAttClass(ATT_NCVIS)) {
        const AttNcVis *att = dynamic_cast<const AttNcVis *>(element);
        assert(att);
        if (att->HasAngled()) {
            attributes->push_back(std::make_pair("angled", att->BooleanToStr(att->GetAngled())));
        }
        if (att->HasCon()) {
            attributes->push_back(std::make_pair("con", att->NcVisConToStr(att->GetCon())));
        }
        if (att->HasCurved()) {
            attributes->push_back(std::make_pair("curved", att->NcVisCurvedToStr(att->GetCurved())));
        }
        if (att->HasDiagonalright()) {
            attributes->push_back(std::make_pair("diagonalright", att->NcVisDiagonalrightToStr(att->GetDiagonalright())));
        }
        if (att->HasEpisima()) {
            attributes->push_back(std::make_pair("episima", att->BooleanToStr(att->GetEpisima())));
        }
        if (att->HasExtended()) {
            attributes->push_back(std::make_pair("extended", att->BooleanToStr(att->GetExtended())));
        }
        if (att->HasFlat()) {
            attributes->push_back(std::make_pair("flat", att->BooleanToStr(att->GetFlat())));
        }
        if (att->HasJagged()) {
            attributes->push_back(std::make_pair("jagged", att->BooleanToStr(att->GetJagged())));
        }
        if (att->HasLiquescent()) {
            attributes->push_back(std::make_pair("liquescent", att->BooleanToStr(att->GetLiquescent())));
        }
        if (att->HasLong()) {
            attributes->push_back(std::make_pair("long", att->BooleanToStr(att->GetLong())));
        }
        if (att->HasOriscus()) {
            attributes->push_back(std::make_pair("oriscus", att->NcVisOriscusToStr(att->GetOriscus())));
        }
        if (att->HasQuilisma()) {
            attributes->push_back(std::make_pair("quilisma", att->NcVisQuilismaToStr(att->GetQuilisma())));
        }
        if (att->HasWavy()) {
            attributes->push_back(std::make_pair("wavy", att->BooleanToStr(att->GetWavy())));
        }
    }
    if (element->HasAttClass(ATT_NEUMEVIS)) {
        const AttNeumeVis *att = dynamic_cast<const AttNeumeVis *>(element);
        assert(att);
        if (att->HasHispanicloop()) {
            attributes->push_back(std::make_pair("hispanicloop", att->BooleanToStr(att->GetHispanicloop())));
        }
        if (att->HasSignificativeletters()) {
            attributes->push_back(std::make_pair("significativeletters", att->BooleanToStr(att->GetSignificativeletters())));
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
            attributes->push_back(std::make_pair("spacing", att->StrToStr(att->GetSpacing())));
        }
    }
    if (element->HasAttClass(ATT_STAFFGRPVIS)) {
        const AttStaffGrpVis *att = dynamic_cast<const AttStaffGrpVis *>(element);
        assert(att);
        if (att->HasBarthru()) {
            attributes->push_back(std::make_pair("barthru", att->BooleanToStr(att->GetBarthru())));
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
