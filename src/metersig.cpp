/////////////////////////////////////////////////////////////////////////////
// Name:        metersig.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "metersig.h"
//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>
#include <numeric>

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
    RegisterAttClass(ATT_ENCLOSINGCHARS);
    RegisterAttClass(ATT_METERSIGLOG);
    RegisterAttClass(ATT_METERSIGVIS);

    Reset();
}

MeterSig::~MeterSig() {}

void MeterSig::Reset()
{
    LayerElement::Reset();
    ResetEnclosingChars();
    ResetMeterSigLog();
    ResetMeterSigVis();
}

int MeterSig::GetTotalCount() const
{
    const data_SUMMAND_List &summands = this->GetCount();
    return std::accumulate(summands.cbegin(), summands.cend(), 0);
}

wchar_t MeterSig::GetSymbolGlyph() const
{
    wchar_t glyph = 0;
    switch (this->GetSym()) {
        case METERSIGN_common: glyph = SMUFL_E08A_timeSigCommon; break;
        case METERSIGN_cut: glyph = SMUFL_E08B_timeSigCutCommon; break;
        default: break;
    }
    return glyph;
}

wchar_t MeterSig::GetEnclosingGlyph(bool beforeMeterSig) const
{
    if (this->GetEnclose() == ENCLOSURE_paren) {
        return beforeMeterSig ? SMUFL_E094_timeSigParensLeft : SMUFL_E095_timeSigParensRight;
    }
    return 0;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int MeterSig::LayerCountInTimeSpan(FunctorParams *functorParams)
{
    LayerCountInTimeSpanParams *params = vrv_params_cast<LayerCountInTimeSpanParams *>(functorParams);
    assert(params);

    params->m_meterSig = this;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
