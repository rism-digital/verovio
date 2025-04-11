/////////////////////////////////////////////////////////////////////////////
// Name:        artic.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "artic.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "elementpart.h"
#include "floatingobject.h"
#include "functor.h"
#include "layer.h"
#include "smufl.h"
#include "staff.h"
#include "stem.h"
#include "vrv.h"

namespace vrv {

const std::vector<data_ARTICULATION> Artic::s_outStaffArtic
    = { ARTICULATION_acc, ARTICULATION_acc_soft, ARTICULATION_dnbow, ARTICULATION_marc, ARTICULATION_upbow,
          ARTICULATION_harm, ARTICULATION_snap, ARTICULATION_fingernail, ARTICULATION_damp, ARTICULATION_dampall,
          ARTICULATION_lhpizz, ARTICULATION_open, ARTICULATION_stop };

const std::vector<data_ARTICULATION> Artic::s_aboveStaffArtic = { ARTICULATION_dnbow, ARTICULATION_marc,
    ARTICULATION_upbow, ARTICULATION_harm, ARTICULATION_snap, ARTICULATION_fingernail, ARTICULATION_damp,
    ARTICULATION_dampall, ARTICULATION_lhpizz, ARTICULATION_open, ARTICULATION_stop };

//----------------------------------------------------------------------------
// Artic
//----------------------------------------------------------------------------

static const ClassRegistrar<Artic> s_factory("artic", ARTIC);

Artic::Artic()
    : LayerElement(ARTIC)
    , AttArticulation()
    , AttArticulationGes()
    , AttColor()
    , AttEnclosingChars()
    , AttExtSymAuth()
    , AttExtSymNames()
    , AttPlacementRelEvent()
{
    this->RegisterAttClass(ATT_ARTICULATION);
    this->RegisterAttClass(ATT_ARTICULATIONGES);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_ENCLOSINGCHARS);
    this->RegisterAttClass(ATT_EXTSYMAUTH);
    this->RegisterAttClass(ATT_EXTSYMNAMES);
    this->RegisterAttClass(ATT_PLACEMENTRELEVENT);

    this->Reset();
}

Artic::~Artic() {}

void Artic::Reset()
{
    LayerElement::Reset();
    this->ResetArticulation();
    this->ResetArticulationGes();
    this->ResetColor();
    this->ResetEnclosingChars();
    this->ResetExtSymAuth();
    this->ResetExtSymNames();
    this->ResetPlacementRelEvent();

    m_drawingPlace = STAFFREL_NONE;
}

bool Artic::IsInsideArtic(data_ARTICULATION artic) const
{
    // Always outside if enclosing brackets are used
    if ((this->GetEnclose() == ENCLOSURE_brack) || (this->GetEnclose() == ENCLOSURE_paren)) {
        return false;
    }

    const auto end = Artic::s_outStaffArtic.end();
    const auto it = std::find(Artic::s_outStaffArtic.begin(), end, artic);
    return (it == end);
}

bool Artic::IsInsideArtic() const
{
    return IsInsideArtic(this->GetArticFirst());
}

data_ARTICULATION Artic::GetArticFirst() const
{
    std::vector<data_ARTICULATION> articList = this->GetArtic();
    if (articList.empty()) return ARTICULATION_NONE;

    return articList.front();
}

void Artic::GetAllArtics(bool direction, std::vector<Artic *> &artics)
{
    Object *parentNoteOrChord = this->GetFirstAncestor(CHORD);

    if (!parentNoteOrChord) parentNoteOrChord = this->GetFirstAncestor(NOTE);

    if (!parentNoteOrChord) return;

    Object *first = (direction == FORWARD) ? this : parentNoteOrChord->GetFirst();
    Object *last = (direction == BACKWARD) ? this : parentNoteOrChord->GetLast();
    ClassIdComparison matchType(ARTIC);
    ListOfObjects children;
    parentNoteOrChord->FindAllDescendantsBetween(&children, &matchType, first, last);
    for (Object *child : children) {
        if (child == this) continue;
        Artic *artic = vrv_cast<Artic *>(child);
        assert(artic);
        if (artic->GetDrawingPlace() == this->GetDrawingPlace()) artics.push_back(artic);
    }
}

bool Artic::AlwaysAbove() const
{
    auto end = Artic::s_aboveStaffArtic.end();
    auto i = std::find(Artic::s_aboveStaffArtic.begin(), end, this->GetArticFirst());
    return (i != end);
}

void Artic::AddSlurPositioner(FloatingCurvePositioner *positioner, bool start)
{
    if (start) {
        if (std::find(m_startSlurPositioners.begin(), m_startSlurPositioners.end(), positioner)
            == m_startSlurPositioners.end())
            m_startSlurPositioners.push_back(positioner);
    }
    else {
        if (std::find(m_endSlurPositioners.begin(), m_endSlurPositioners.end(), positioner)
            == m_endSlurPositioners.end())
            m_endSlurPositioners.push_back(positioner);
    }
}

char32_t Artic::GetArticGlyph(data_ARTICULATION artic, data_STAFFREL place) const
{
    const Resources *resources = this->GetDocResources();
    if (!resources) return 0;

    // If there is glyph.num, prioritize it
    if (this->HasGlyphNum()) {
        char32_t code = this->GetGlyphNum();
        if (NULL != resources->GetGlyph(code)) return code;
    }
    // If there is glyph.name (second priority)
    else if (this->HasGlyphName()) {
        char32_t code = resources->GetGlyphCode(this->GetGlyphName());
        if (NULL != resources->GetGlyph(code)) return code;
    }

    if (place == STAFFREL_above) {
        switch (artic) {
            case ARTICULATION_acc: return SMUFL_E4A0_articAccentAbove;
            case ARTICULATION_acc_soft: return SMUFL_ED40_articSoftAccentAbove;
            case ARTICULATION_stacc: return SMUFL_E4A2_articStaccatoAbove;
            case ARTICULATION_ten: return SMUFL_E4A4_articTenutoAbove;
            case ARTICULATION_stacciss: return SMUFL_E4A8_articStaccatissimoWedgeAbove;
            case ARTICULATION_marc: return SMUFL_E4AC_articMarcatoAbove;
            case ARTICULATION_spicc: return SMUFL_E4A6_articStaccatissimoAbove;
            // case ARTICULATION_doit;
            // case ARTICULATION_scoop;
            // case ARTICULATION_rip;
            // case ARTICULATION_plop;
            // case ARTICULATION_fall;
            // case ARTICULATION_longfall;
            // case ARTICULATION_bend;
            // case ARTICULATION_flip;
            // case ARTICULATION_smear;
            // case ARTICULATION_shake;
            case ARTICULATION_dnbow: return SMUFL_E610_stringsDownBow;
            case ARTICULATION_upbow: return SMUFL_E612_stringsUpBow;
            case ARTICULATION_harm: return SMUFL_E614_stringsHarmonic;
            case ARTICULATION_snap: return SMUFL_E631_pluckedSnapPizzicatoAbove;
            case ARTICULATION_fingernail: return SMUFL_E636_pluckedWithFingernails;
            case ARTICULATION_damp: return SMUFL_E638_pluckedDamp;
            case ARTICULATION_dampall: return SMUFL_E639_pluckedDampAll;
            case ARTICULATION_open: return SMUFL_E5E7_brassMuteOpen;
            case ARTICULATION_stop: return SMUFL_E5E5_brassMuteClosed;
            // case ARTICULATION_dbltongue;
            // case ARTICULATION_trpltongue;
            // case ARTICULATION_heel;
            // case ARTICULATION_toe;
            // case ARTICULATION_tap;
            case ARTICULATION_lhpizz: return SMUFL_E633_pluckedLeftHandPizzicato;
            case ARTICULATION_dot: return SMUFL_E4A2_articStaccatoAbove;
            case ARTICULATION_stroke: return SMUFL_E4AA_articStaccatissimoStrokeAbove;
            default: return 0;
        }
    }
    else if (place == STAFFREL_below) {
        switch (artic) {
            case ARTICULATION_acc: return SMUFL_E4A1_articAccentBelow;
            case ARTICULATION_acc_soft: return SMUFL_ED41_articSoftAccentBelow;
            case ARTICULATION_stacc: return SMUFL_E4A3_articStaccatoBelow;
            case ARTICULATION_ten: return SMUFL_E4A5_articTenutoBelow;
            case ARTICULATION_stacciss: return SMUFL_E4A9_articStaccatissimoWedgeBelow;
            case ARTICULATION_marc: return SMUFL_E4AD_articMarcatoBelow;
            case ARTICULATION_spicc: return SMUFL_E4A7_articStaccatissimoBelow;
            case ARTICULATION_dnbow: return SMUFL_E611_stringsDownBowTurned;
            case ARTICULATION_upbow: return SMUFL_E613_stringsUpBowTurned;
            case ARTICULATION_harm: return SMUFL_E614_stringsHarmonic;
            case ARTICULATION_snap: return SMUFL_E630_pluckedSnapPizzicatoBelow;
            case ARTICULATION_fingernail: return SMUFL_E636_pluckedWithFingernails;
            case ARTICULATION_damp: return SMUFL_E638_pluckedDamp;
            case ARTICULATION_dampall: return SMUFL_E639_pluckedDampAll;
            case ARTICULATION_open: return SMUFL_E5E7_brassMuteOpen;
            case ARTICULATION_stop: return SMUFL_E5E5_brassMuteClosed;
            case ARTICULATION_lhpizz: return SMUFL_E633_pluckedLeftHandPizzicato;
            case ARTICULATION_dot: return SMUFL_E4A3_articStaccatoBelow;
            case ARTICULATION_stroke: return SMUFL_E4AB_articStaccatissimoStrokeBelow;
            default: return 0;
        }
    }
    else {
        return 0;
    }
}

std::pair<char32_t, char32_t> Artic::GetEnclosingGlyphs() const
{
    std::pair<char32_t, char32_t> glyphs(0, 0);
    if (this->HasEnclose()) {
        switch (this->GetEnclose()) {
            case ENCLOSURE_brack:
                glyphs = { SMUFL_E26C_accidentalBracketLeft, SMUFL_E26D_accidentalBracketRight };
                break;
            case ENCLOSURE_paren: glyphs = { SMUFL_E26A_accidentalParensLeft, SMUFL_E26B_accidentalParensRight }; break;
            default: break;
        }
    }
    return glyphs;
}

//----------------------------------------------------------------------------
// Static methods for Artic
//----------------------------------------------------------------------------

bool Artic::VerticalCorr(char32_t code, data_STAFFREL place)
{
    if (place == STAFFREL_above) return false;

    switch (code) {
        case SMUFL_E5E5_brassMuteClosed: return true;
        case SMUFL_E5E6_brassMuteHalfClosed: return true;
        case SMUFL_E5E7_brassMuteOpen: return true;
        case SMUFL_E611_stringsDownBowTurned: return true;
        case SMUFL_E613_stringsUpBowTurned: return true;
        case SMUFL_E614_stringsHarmonic: return true;
        case SMUFL_E630_pluckedSnapPizzicatoBelow: return true;
        case SMUFL_E633_pluckedLeftHandPizzicato: return true;
        case SMUFL_E636_pluckedWithFingernails: return true;
        case SMUFL_E638_pluckedDamp: return true;
        case SMUFL_E639_pluckedDampAll: return true;
        default: return false;
    }
}

bool Artic::IsCentered(data_ARTICULATION artic)
{
    if (artic == ARTICULATION_stacc) return true;
    if (artic == ARTICULATION_ten) return true;
    return false;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Artic::Accept(Functor &functor)
{
    return functor.VisitArtic(this);
}

FunctorCode Artic::Accept(ConstFunctor &functor) const
{
    return functor.VisitArtic(this);
}

FunctorCode Artic::AcceptEnd(Functor &functor)
{
    return functor.VisitArticEnd(this);
}

FunctorCode Artic::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitArticEnd(this);
}

} // namespace vrv
