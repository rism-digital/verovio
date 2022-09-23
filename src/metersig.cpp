/////////////////////////////////////////////////////////////////////////////
// Name:        metersig.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "metersig.h"
//----------------------------------------------------------------------------

#include <cassert>
#include <math.h>
#include <numeric>
#include <regex>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "scoredefinterface.h"
#include "smufl.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// MeterSig
//----------------------------------------------------------------------------

static const ClassRegistrar<MeterSig> s_factory("meterSig", METERSIG);

MeterSig::MeterSig() : LayerElement(METERSIG, "msig-"), AttEnclosingChars(), AttMeterSigLog(), AttMeterSigVis()
{
    this->RegisterAttClass(ATT_ENCLOSINGCHARS);
    this->RegisterAttClass(ATT_METERSIGLOG);
    this->RegisterAttClass(ATT_METERSIGVIS);

    this->Reset();
}

MeterSig::~MeterSig() {}

void MeterSig::Reset()
{
    LayerElement::Reset();
    this->ResetEnclosingChars();
    this->ResetMeterSigLog();
    this->ResetMeterSigVis();
}

int MeterSig::GetTotalCount() const
{
    auto [counts, sign] = this->GetCount();
    // If @count is empty, look at the sym to return a resonable value
    if (counts.empty()) {
        if (this->HasSym()) {
            return (this->GetSym() == METERSIGN_common) ? 4 : 2;
        }
        else {
            return 0;
        }
    }
    switch (sign) {
        case MeterCountSign::Slash: {
            // make sure that there is no division by zero
            std::for_each(counts.begin(), counts.end(), [](int &elem) {
                if (!elem) elem = 1;
            });
            int result
                = std::accumulate(std::next(counts.cbegin()), counts.cend(), *counts.cbegin(), std::divides<int>());
            if (!result) result = 1;
            return result;
        }
        case MeterCountSign::Minus: {
            int result
                = std::accumulate(std::next(counts.cbegin()), counts.cend(), *counts.cbegin(), std::minus<int>());
            if (result <= 0) result = 1;
            return result;
        }
        case MeterCountSign::Asterisk: {
            int result = std::accumulate(counts.cbegin(), counts.cend(), 1, std::multiplies<int>());
            if (!result) result = 1;
            return result;
        }
        case MeterCountSign::Plus: {
            return std::accumulate(counts.cbegin(), counts.cend(), 0, std::plus<int>());
        }
        case MeterCountSign::None:
        default: break;
    }

    return counts.front();
}

char32_t MeterSig::GetSymbolGlyph() const
{
    char32_t glyph = 0;
    switch (this->GetSym()) {
        case METERSIGN_common: glyph = SMUFL_E08A_timeSigCommon; break;
        case METERSIGN_cut: glyph = SMUFL_E08B_timeSigCutCommon; break;
        default: break;
    }
    return glyph;
}

std::pair<char32_t, char32_t> MeterSig::GetEnclosingGlyphs(bool smallGlyph) const
{
    if (this->GetEnclose() == ENCLOSURE_brack) {
        if (smallGlyph) {
            return { SMUFL_EC82_timeSigBracketLeftSmall, SMUFL_EC83_timeSigBracketRightSmall };
        }
        else {
            return { SMUFL_EC80_timeSigBracketLeft, SMUFL_EC81_timeSigBracketRight };
        }
    }
    else if (this->GetEnclose() == ENCLOSURE_paren) {
        if (smallGlyph) {
            return { SMUFL_E092_timeSigParensLeftSmall, SMUFL_E093_timeSigParensRightSmall };
        }
        else {
            return { SMUFL_E094_timeSigParensLeft, SMUFL_E095_timeSigParensRight };
        }
    }
    return { 0, 0 };
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int MeterSig::LayerCountInTimeSpan(FunctorParams *functorParams) const
{
    LayerCountInTimeSpanParams *params = vrv_params_cast<LayerCountInTimeSpanParams *>(functorParams);
    assert(params);

    params->m_meterSig = this;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
