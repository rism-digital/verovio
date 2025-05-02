/////////////////////////////////////////////////////////////////////////////
// Name:        clef.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "clef.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "functor.h"
#include "layer.h"
#include "scoredefinterface.h"
#include "smufl.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Clef
//----------------------------------------------------------------------------

static const ClassRegistrar<Clef> s_factory("clef", CLEF);

Clef::Clef()
    : LayerElement(CLEF)
    , AttClefLog()
    , AttClefShape()
    , AttColor()
    , AttExtSymAuth()
    , AttExtSymNames()
    , AttLineLoc()
    , AttOctave()
    , AttOctaveDisplacement()
    , AttStaffIdent()
    , AttTypography()
    , AttVisibility()
{
    this->RegisterAttClass(ATT_CLEFLOG);
    this->RegisterAttClass(ATT_CLEFSHAPE);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_ENCLOSINGCHARS);
    this->RegisterAttClass(ATT_EXTSYMAUTH);
    this->RegisterAttClass(ATT_EXTSYMNAMES);
    this->RegisterAttClass(ATT_LINELOC);
    this->RegisterAttClass(ATT_OCTAVE);
    this->RegisterAttClass(ATT_OCTAVEDISPLACEMENT);
    this->RegisterAttClass(ATT_STAFFIDENT);
    this->RegisterAttClass(ATT_TYPOGRAPHY);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
}

Clef::~Clef() {}

void Clef::Reset()
{
    LayerElement::Reset();
    this->ResetClefLog();
    this->ResetClefShape();
    this->ResetColor();
    this->ResetEnclosingChars();
    this->ResetExtSymAuth();
    this->ResetExtSymNames();
    this->ResetLineLoc();
    this->ResetOctave();
    this->ResetOctaveDisplacement();
    this->ResetStaffIdent();
    this->ResetTypography();
    this->ResetVisibility();
}

int Clef::GetClefLocOffset() const
{
    // Only resolve simple sameas links to avoid infinite recursion
    const Clef *sameas = dynamic_cast<const Clef *>(this->GetSameasLink());
    if (sameas && !sameas->HasSameasLink()) {
        return sameas->GetClefLocOffset();
    }

    int offset = 0;
    int defaultOct = 4; // C clef
    if (this->GetShape() == CLEFSHAPE_G) {
        defaultOct = 4;
        offset = -4;
    }
    else if (this->GetShape() == CLEFSHAPE_GG) {
        defaultOct = 3;
        offset = 3;
    }
    else if (this->GetShape() == CLEFSHAPE_F) {
        defaultOct = 3;
        offset = 4;
    }

    if (this->HasOct()) {
        int oct = this->GetOct();
        int octDifference = oct - defaultOct;
        offset -= octDifference * 7;
    }

    offset += (this->GetLine() - 1) * 2;

    int disPlace = 0;
    if (this->HasDisPlace()) {
        disPlace = (this->GetDisPlace() == STAFFREL_basic_above) ? -1 : 1;
    }

    if ((disPlace != 0) && this->HasDis()) {
        offset += disPlace * (this->GetDis() - 1);
    }

    return offset;
}

//----------------------------------------------------------------------------
// Static methods for Clef
//----------------------------------------------------------------------------

char32_t Clef::GetClefGlyph(const data_NOTATIONTYPE notationtype) const
{
    const Resources *resources = this->GetDocResources();
    const bool clefChange = (this->GetAlignment() && (this->GetAlignment()->GetType() == ALIGNMENT_CLEF));
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

    switch (notationtype) {
        case NOTATIONTYPE_tab:
        case NOTATIONTYPE_tab_guitar: return SMUFL_E06D_6stringTabClef; break;
        case NOTATIONTYPE_neume:
            // neume clefs
            switch (this->GetShape()) {
                case CLEFSHAPE_F: return SMUFL_E902_chantFclef; break;
                case CLEFSHAPE_C: return SMUFL_E906_chantCclef; break;
                case CLEFSHAPE_G: return SMUFL_E900_mensuralGclef; break;
                default: return SMUFL_E906_chantCclef; break;
            }
        case NOTATIONTYPE_mensural:
        case NOTATIONTYPE_mensural_white:
            // mensural clefs
            switch (this->GetShape()) {
                case CLEFSHAPE_G: return SMUFL_E901_mensuralGclefPetrucci; break;
                case CLEFSHAPE_F: return SMUFL_E904_mensuralFclefPetrucci; break;
                case CLEFSHAPE_C:
                    switch (this->GetLine()) {
                        case 1: return SMUFL_E907_mensuralCclefPetrucciPosLowest; break;
                        case 2: return SMUFL_E908_mensuralCclefPetrucciPosLow; break;
                        case 3: return SMUFL_E909_mensuralCclefPetrucciPosMiddle; break;
                        case 4: return SMUFL_E90A_mensuralCclefPetrucciPosHigh; break;
                        case 5: return SMUFL_E90B_mensuralCclefPetrucciPosHighest; break;
                    }
                    [[fallthrough]];
                default: return SMUFL_E909_mensuralCclefPetrucciPosMiddle; break;
            }
        case NOTATIONTYPE_mensural_black:
            switch (this->GetShape()) {
                case CLEFSHAPE_C: return SMUFL_E906_chantCclef; break;
                case CLEFSHAPE_F: return SMUFL_E902_chantFclef; break;
                default:
                    // G clef doesn't exist in black notation, so should never get here, but just in case.
                    if (!this->GetDis()) return SMUFL_E901_mensuralGclefPetrucci;
            }
            [[fallthrough]];
        default:
            // cmn clefs
            switch (this->GetShape()) {
                case CLEFSHAPE_G:
                    switch (this->GetDis()) {
                        case OCTAVE_DIS_8:
                            return (this->GetDisPlace() == STAFFREL_basic_above) ? SMUFL_E053_gClef8va
                                                                                 : SMUFL_E052_gClef8vb;
                            break;
                        case OCTAVE_DIS_15:
                            return (this->GetDisPlace() == STAFFREL_basic_above) ? SMUFL_E054_gClef15ma
                                                                                 : SMUFL_E051_gClef15mb;
                            break;
                        default: return (clefChange) ? SMUFL_E07A_gClefChange : SMUFL_E050_gClef; break;
                    }
                case CLEFSHAPE_GG: return SMUFL_E055_gClef8vbOld;
                case CLEFSHAPE_F:
                    switch (this->GetDis()) {
                        case OCTAVE_DIS_8:
                            return (this->GetDisPlace() == STAFFREL_basic_above) ? SMUFL_E065_fClef8va
                                                                                 : SMUFL_E064_fClef8vb;
                            break;
                        case OCTAVE_DIS_15:
                            return (this->GetDisPlace() == STAFFREL_basic_above) ? SMUFL_E066_fClef15ma
                                                                                 : SMUFL_E063_fClef15mb;
                            break;
                        default: return (clefChange) ? SMUFL_E07C_fClefChange : SMUFL_E062_fClef; break;
                    }
                case CLEFSHAPE_C:
                    switch (this->GetDis()) {
                        case OCTAVE_DIS_8: return SMUFL_E05D_cClef8vb; break;
                        default: return (clefChange) ? SMUFL_E07B_cClefChange : SMUFL_E05C_cClef; break;
                    }
                case CLEFSHAPE_perc: return SMUFL_E069_unpitchedPercussionClef1;
                default: break;
            }
    }

    return 0;
}

//----------------------------------------------------------------------------
// Clef functors methods
//----------------------------------------------------------------------------

FunctorCode Clef::Accept(Functor &functor)
{
    return functor.VisitClef(this);
}

FunctorCode Clef::Accept(ConstFunctor &functor) const
{
    return functor.VisitClef(this);
}

FunctorCode Clef::AcceptEnd(Functor &functor)
{
    return functor.VisitClefEnd(this);
}

FunctorCode Clef::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitClefEnd(this);
}

} // namespace vrv
