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
            case ARTICULATION_stacciss: return SMUFL_E4A8_articStaccatissimoWedgeAbove;
            case ARTICULATION_marc: return SMUFL_E4AC_articMarcatoAbove;
            default: return 0; break;
        }
    }
    else if (place == STAFFREL_below) {
        switch (artic) {
            case ARTICULATION_acc: return SMUFL_E4A1_articAccentBelow;
            case ARTICULATION_stacciss: return SMUFL_E4A9_articStaccatissimoWedgeBelow;
            case ARTICULATION_marc: return SMUFL_E4AD_articMarcatoBelow;
            default: return 0; break;
        }
    }
    else
        return 0;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
