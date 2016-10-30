/////////////////////////////////////////////////////////////////////////////
// Name:        artic.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "artic.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "smufl.h"

namespace vrv {

//----------------------------------------------------------------------------
// Artic
//----------------------------------------------------------------------------

Artic::Artic() : LayerElement("artic-"), AttArticulation(), AttColor(), AttPlacement()
{

    RegisterAttClass(ATT_ARTICULATION);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_PLACEMENT);

    Reset();
}

Artic::~Artic()
{
}

void Artic::Reset()
{
    LayerElement::Reset();
    ResetArticulation();
    ResetColor();
    ResetPlacement();
}

wchar_t Artic::GetSmuflCode(data_ARTICULATION artic, data_STAFFREL place)
{
    if (place == STAFFREL_above) {
        switch (artic) {
            case ARTICULATION_acc: return SMUFL_E4A0_articAccentAbove;
            case ARTICULATION_stacc: return SMUFL_E4A2_articStaccatoAbove;
            case ARTICULATION_ten: return SMUFL_E4A4_articTenutoAbove;
            case ARTICULATION_stacciss: return SMUFL_E4A8_articStaccatissimoWedgeAbove;
            case ARTICULATION_marc: return SMUFL_E4AC_articMarcatoAbove;
            case ARTICULATION_marc_stacc:
                return SMUFL_E4AE_articMarcatoStaccatoAbove;
            // case ARTICULATION_spicc;
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
            case ARTICULATION_upbow:
                return SMUFL_E612_stringsUpBow;
            // case ARTICULATION_harm;
            // case ARTICULATION_snap;
            // case ARTICULATION_fingernail;
            case ARTICULATION_ten_stacc:
                return SMUFL_E4B2_articTenutoStaccatoAbove;
            // case ARTICULATION_damp;
            // case ARTICULATION_dampall;
            // case ARTICULATION_open;
            // case ARTICULATION_stop;
            // case ARTICULATION_dbltongue;
            // case ARTICULATION_trpltongue;
            // case ARTICULATION_heel;
            // case ARTICULATION_toe;
            // case ARTICULATION_tap;
            // case ARTICULATION_lhpizz;
            // case ARTICULATION_dot;
            // case ARTICULATION_stroke;
            default: return 0; break;
        }
    }
    else if (place == STAFFREL_below) {
        switch (artic) {
            case ARTICULATION_acc: return SMUFL_E4A1_articAccentBelow;
            case ARTICULATION_stacc: return SMUFL_E4A3_articStaccatoBelow;
            case ARTICULATION_ten: return SMUFL_E4A5_articTenutoBelow;
            case ARTICULATION_stacciss: return SMUFL_E4A9_articStaccatissimoWedgeBelow;
            case ARTICULATION_marc: return SMUFL_E4AD_articMarcatoBelow;
            case ARTICULATION_marc_stacc:
                return SMUFL_E4AF_articMarcatoStaccatoBelow;
            //
            case ARTICULATION_dnbow: return SMUFL_E611_stringsDownBowTurned;
            case ARTICULATION_upbow:
                return SMUFL_E613_stringsUpBowTurned;
            //
            case ARTICULATION_ten_stacc:
                return SMUFL_E4B3_articTenutoStaccatoBelow;
            //
            default: return 0; break;
        }
    }
    else
        return 0;
}

bool Artic::VerticalCorr(wchar_t code, data_STAFFREL place)
{
    if (place == STAFFREL_above)
        return false;
    else if (code == SMUFL_E611_stringsDownBowTurned)
        return true;
    else if (code == SMUFL_E613_stringsUpBowTurned)
        return true;
    else
        return false;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
