/////////////////////////////////////////////////////////////////////////////
// Name:        octave.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "octave.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "smufl.h"
#include "verticalaligner.h"

namespace vrv {

//----------------------------------------------------------------------------
// Octave
//----------------------------------------------------------------------------

static const ClassRegistrar<Octave> s_factory("octave", OCTAVE);

Octave::Octave()
    : ControlElement(OCTAVE, "octave-")
    , TimeSpanningInterface()
    , AttColor()
    , AttExtender()
    , AttLineRend()
    , AttLineRendBase()
    , AttNNumberLike()
    , AttOctaveDisplacement()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_EXTENDER);
    this->RegisterAttClass(ATT_LINEREND);
    this->RegisterAttClass(ATT_LINERENDBASE);
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->RegisterAttClass(ATT_OCTAVEDISPLACEMENT);

    this->Reset();
}

Octave::~Octave() {}

void Octave::Reset()
{
    ControlElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetColor();
    this->ResetExtender();
    this->ResetLineRend();
    this->ResetLineRendBase();
    this->ResetNNumberLike();
    this->ResetOctaveDisplacement();
}

char32_t Octave::GetOctaveGlyph(bool withAltaBassa) const
{
    const data_OCTAVE_DIS dis = this->GetDis();
    char32_t code = SMUFL_E510_ottava;
    if (this->GetDisPlace() == STAFFREL_basic_above) {
        switch (dis) {
            case OCTAVE_DIS_8: {
                code = withAltaBassa ? SMUFL_E511_ottavaAlta : SMUFL_E510_ottava;
                break;
            }
            case OCTAVE_DIS_15: {
                code = withAltaBassa ? SMUFL_E515_quindicesimaAlta : SMUFL_E514_quindicesima;
                break;
            }
            case OCTAVE_DIS_22: {
                code = withAltaBassa ? SMUFL_E518_ventiduesimaAlta : SMUFL_E517_ventiduesima;
                break;
            }
            default: break;
        }
    }
    else {
        switch (dis) {
            case OCTAVE_DIS_8: {
                code = withAltaBassa ? SMUFL_E51C_ottavaBassaVb : SMUFL_E510_ottava;
                break;
            }
            case OCTAVE_DIS_15: {
                code = withAltaBassa ? SMUFL_E51D_quindicesimaBassaMb : SMUFL_E514_quindicesima;
                break;
            }
            case OCTAVE_DIS_22: {
                code = withAltaBassa ? SMUFL_E51E_ventiduesimaBassaMb : SMUFL_E517_ventiduesima;
                break;
            }
            default: break;
        }
    }
    return code;
}

//----------------------------------------------------------------------------
// Octave functor methods
//----------------------------------------------------------------------------

} // namespace vrv
