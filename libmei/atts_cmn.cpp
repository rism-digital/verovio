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

#include "atts_cmn.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttArpegLog
//----------------------------------------------------------------------------

AttArpegLog::AttArpegLog() {
    ResetArpegLog();
}

AttArpegLog::~AttArpegLog() {

}

void AttArpegLog::ResetArpegLog() {
    m_order = "";
}

/* include <attorder> */

//----------------------------------------------------------------------------
// AttArpegVis
//----------------------------------------------------------------------------

AttArpegVis::AttArpegVis() {
    ResetArpegVis();
}

AttArpegVis::~AttArpegVis() {

}

void AttArpegVis::ResetArpegVis() {
    m_arrow = "";
}

/* include <attarrow> */

//----------------------------------------------------------------------------
// AttBTremLog
//----------------------------------------------------------------------------

AttBTremLog::AttBTremLog() {
    ResetBTremLog();
}

AttBTremLog::~AttBTremLog() {

}

void AttBTremLog::ResetBTremLog() {
    m_form = "";
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttBeamed
//----------------------------------------------------------------------------

AttBeamed::AttBeamed() {
    ResetBeamed();
}

AttBeamed::~AttBeamed() {

}

void AttBeamed::ResetBeamed() {
    m_beam = "";
}

/* include <attbeam> */

//----------------------------------------------------------------------------
// AttBeamedwith
//----------------------------------------------------------------------------

AttBeamedwith::AttBeamedwith() {
    ResetBeamedwith();
}

AttBeamedwith::~AttBeamedwith() {

}

void AttBeamedwith::ResetBeamedwith() {
    m_beamWith = "";
}

/* include <attbeam.with> */

//----------------------------------------------------------------------------
// AttBeamingLog
//----------------------------------------------------------------------------

AttBeamingLog::AttBeamingLog() {
    ResetBeamingLog();
}

AttBeamingLog::~AttBeamingLog() {

}

void AttBeamingLog::ResetBeamingLog() {
    m_beamGroup = "";
    m_beamRests = "";
}

/* include <attbeam.rests> */

//----------------------------------------------------------------------------
// AttBeamrend
//----------------------------------------------------------------------------

AttBeamrend::AttBeamrend() {
    ResetBeamrend();
}

AttBeamrend::~AttBeamrend() {

}

void AttBeamrend::ResetBeamrend() {
    m_rend = "";
    m_slopeDbl = 0.0;
}

/* include <attslope> */

//----------------------------------------------------------------------------
// AttBeamsecondary
//----------------------------------------------------------------------------

AttBeamsecondary::AttBeamsecondary() {
    ResetBeamsecondary();
}

AttBeamsecondary::~AttBeamsecondary() {

}

void AttBeamsecondary::ResetBeamsecondary() {
    m_breaksecInt = 0;
}

/* include <attbreaksec> */

//----------------------------------------------------------------------------
// AttBeatRptVis
//----------------------------------------------------------------------------

AttBeatRptVis::AttBeatRptVis() {
    ResetBeatRptVis();
}

AttBeatRptVis::~AttBeatRptVis() {

}

void AttBeatRptVis::ResetBeatRptVis() {
    m_rend = "";
}

/* include <attrend> */

//----------------------------------------------------------------------------
// AttBendGes
//----------------------------------------------------------------------------

AttBendGes::AttBendGes() {
    ResetBendGes();
}

AttBendGes::~AttBendGes() {

}

void AttBendGes::ResetBendGes() {
    m_amount = "";
}

/* include <attamount> */

//----------------------------------------------------------------------------
// AttCutout
//----------------------------------------------------------------------------

AttCutout::AttCutout() {
    ResetCutout();
}

AttCutout::~AttCutout() {

}

void AttCutout::ResetCutout() {
    m_cutout = "";
}

/* include <attcutout> */

//----------------------------------------------------------------------------
// AttExpandable
//----------------------------------------------------------------------------

AttExpandable::AttExpandable() {
    ResetExpandable();
}

AttExpandable::~AttExpandable() {

}

void AttExpandable::ResetExpandable() {
    m_expand = "";
}

/* include <attexpand> */

//----------------------------------------------------------------------------
// AttFTremLog
//----------------------------------------------------------------------------

AttFTremLog::AttFTremLog() {
    ResetFTremLog();
}

AttFTremLog::~AttFTremLog() {

}

void AttFTremLog::ResetFTremLog() {
    m_form = "";
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttFermataVis
//----------------------------------------------------------------------------

AttFermataVis::AttFermataVis() {
    ResetFermataVis();
}

AttFermataVis::~AttFermataVis() {

}

void AttFermataVis::ResetFermataVis() {
    m_form = "";
    m_shape = "";
}

/* include <attshape> */

//----------------------------------------------------------------------------
// AttGlissVis
//----------------------------------------------------------------------------

AttGlissVis::AttGlissVis() {
    ResetGlissVis();
}

AttGlissVis::~AttGlissVis() {

}

void AttGlissVis::ResetGlissVis() {
    m_text = "";
}

/* include <atttext> */

//----------------------------------------------------------------------------
// AttGraced
//----------------------------------------------------------------------------

AttGraced::AttGraced() {
    ResetGraced();
}

AttGraced::~AttGraced() {

}

void AttGraced::ResetGraced() {
    m_grace = "";
    m_graceTime = "";
}

/* include <attgrace.time> */

//----------------------------------------------------------------------------
// AttHairpinLog
//----------------------------------------------------------------------------

AttHairpinLog::AttHairpinLog() {
    ResetHairpinLog();
}

AttHairpinLog::~AttHairpinLog() {

}

void AttHairpinLog::ResetHairpinLog() {
    m_form = "";
}

/* include <attform> */

//----------------------------------------------------------------------------
// AttHairpinVis
//----------------------------------------------------------------------------

AttHairpinVis::AttHairpinVis() {
    ResetHairpinVis();
}

AttHairpinVis::~AttHairpinVis() {

}

void AttHairpinVis::ResetHairpinVis() {
    m_opening = "";
}

/* include <attopening> */

//----------------------------------------------------------------------------
// AttHarpPedalLog
//----------------------------------------------------------------------------

AttHarpPedalLog::AttHarpPedalLog() {
    ResetHarpPedalLog();
}

AttHarpPedalLog::~AttHarpPedalLog() {

}

void AttHarpPedalLog::ResetHarpPedalLog() {
    m_c = "";
    m_d = "";
    m_e = "";
    m_f = "";
    m_g = "";
    m_a = "";
    m_b = "";
}

/* include <attb> */

//----------------------------------------------------------------------------
// AttLvpresent
//----------------------------------------------------------------------------

AttLvpresent::AttLvpresent() {
    ResetLvpresent();
}

AttLvpresent::~AttLvpresent() {

}

void AttLvpresent::ResetLvpresent() {
    m_lv = "";
}

/* include <attlv> */

//----------------------------------------------------------------------------
// AttMultiRestVis
//----------------------------------------------------------------------------

AttMultiRestVis::AttMultiRestVis() {
    ResetMultiRestVis();
}

AttMultiRestVis::~AttMultiRestVis() {

}

void AttMultiRestVis::ResetMultiRestVis() {
    m_block = "";
}

/* include <attblock> */

//----------------------------------------------------------------------------
// AttNoteGesCmn
//----------------------------------------------------------------------------

AttNoteGesCmn::AttNoteGesCmn() {
    ResetNoteGesCmn();
}

AttNoteGesCmn::~AttNoteGesCmn() {

}

void AttNoteGesCmn::ResetNoteGesCmn() {
    m_gliss = "";
}

/* include <attgliss> */

//----------------------------------------------------------------------------
// AttNumbered
//----------------------------------------------------------------------------

AttNumbered::AttNumbered() {
    ResetNumbered();
}

AttNumbered::~AttNumbered() {

}

void AttNumbered::ResetNumbered() {
    m_numInt = 0;
}

/* include <attnum> */

//----------------------------------------------------------------------------
// AttNumberplacement
//----------------------------------------------------------------------------

AttNumberplacement::AttNumberplacement() {
    ResetNumberplacement();
}

AttNumberplacement::~AttNumberplacement() {

}

void AttNumberplacement::ResetNumberplacement() {
    m_numPlace = "";
    m_numVisible = "";
}

/* include <attnum.visible> */

//----------------------------------------------------------------------------
// AttOctaveLog
//----------------------------------------------------------------------------

AttOctaveLog::AttOctaveLog() {
    ResetOctaveLog();
}

AttOctaveLog::~AttOctaveLog() {

}

void AttOctaveLog::ResetOctaveLog() {
    m_coll = "";
}

/* include <attcoll> */

//----------------------------------------------------------------------------
// AttPedalLog
//----------------------------------------------------------------------------

AttPedalLog::AttPedalLog() {
    ResetPedalLog();
}

AttPedalLog::~AttPedalLog() {

}

void AttPedalLog::ResetPedalLog() {
    m_dir = "";
}

/* include <attdir> */

//----------------------------------------------------------------------------
// AttPedalVis
//----------------------------------------------------------------------------

AttPedalVis::AttPedalVis() {
    ResetPedalVis();
}

AttPedalVis::~AttPedalVis() {

}

void AttPedalVis::ResetPedalVis() {
    m_style = "";
}

/* include <attstyle> */

//----------------------------------------------------------------------------
// AttPianopedals
//----------------------------------------------------------------------------

AttPianopedals::AttPianopedals() {
    ResetPianopedals();
}

AttPianopedals::~AttPianopedals() {

}

void AttPianopedals::ResetPianopedals() {
    m_pedalStyle = "";
}

/* include <attpedal.style> */

//----------------------------------------------------------------------------
// AttRehearsal
//----------------------------------------------------------------------------

AttRehearsal::AttRehearsal() {
    ResetRehearsal();
}

AttRehearsal::~AttRehearsal() {

}

void AttRehearsal::ResetRehearsal() {
    m_rehEnclose = "";
}

/* include <attreh.enclose> */

//----------------------------------------------------------------------------
// AttScoreDefVisCmn
//----------------------------------------------------------------------------

AttScoreDefVisCmn::AttScoreDefVisCmn() {
    ResetScoreDefVisCmn();
}

AttScoreDefVisCmn::~AttScoreDefVisCmn() {

}

void AttScoreDefVisCmn::ResetScoreDefVisCmn() {
    m_gridShow = "";
}

/* include <attgrid.show> */

//----------------------------------------------------------------------------
// AttSlurrend
//----------------------------------------------------------------------------

AttSlurrend::AttSlurrend() {
    ResetSlurrend();
}

AttSlurrend::~AttSlurrend() {

}

void AttSlurrend::ResetSlurrend() {
    m_slurRend = "";
}

/* include <attslur.rend> */

//----------------------------------------------------------------------------
// AttStemmedCmn
//----------------------------------------------------------------------------

AttStemmedCmn::AttStemmedCmn() {
    ResetStemmedCmn();
}

AttStemmedCmn::~AttStemmedCmn() {

}

void AttStemmedCmn::ResetStemmedCmn() {
    m_stemMod = "";
    m_stemWith = "";
}

/* include <attstem.with> */

//----------------------------------------------------------------------------
// AttTierend
//----------------------------------------------------------------------------

AttTierend::AttTierend() {
    ResetTierend();
}

AttTierend::~AttTierend() {

}

void AttTierend::ResetTierend() {
    m_tieRend = "";
}

/* include <atttie.rend> */

//----------------------------------------------------------------------------
// AttTremmeasured
//----------------------------------------------------------------------------

AttTremmeasured::AttTremmeasured() {
    ResetTremmeasured();
}

AttTremmeasured::~AttTremmeasured() {

}

void AttTremmeasured::ResetTremmeasured() {
    m_measperf = "";
}

/* include <attmeasperf> */

//----------------------------------------------------------------------------
// AttTupletLog
//----------------------------------------------------------------------------

AttTupletLog::AttTupletLog() {
    ResetTupletLog();
}

AttTupletLog::~AttTupletLog() {

}

void AttTupletLog::ResetTupletLog() {
    m_dur = "";
}

/* include <attdur> */

//----------------------------------------------------------------------------
// AttTupletVis
//----------------------------------------------------------------------------

AttTupletVis::AttTupletVis() {
    ResetTupletVis();
}

AttTupletVis::~AttTupletVis() {

}

void AttTupletVis::ResetTupletVis() {
    m_bracketPlace = "";
    m_bracketVisible = "";
    m_durVisible = "";
    m_numFormat = "";
}

/* include <attnum.format> */

//----------------------------------------------------------------------------
// AttTupletSpanLog
//----------------------------------------------------------------------------

AttTupletSpanLog::AttTupletSpanLog() {
    ResetTupletSpanLog();
}

AttTupletSpanLog::~AttTupletSpanLog() {

}

void AttTupletSpanLog::ResetTupletSpanLog() {
    m_dur = "";
}

/* include <attdur> */

} // vrv namespace

