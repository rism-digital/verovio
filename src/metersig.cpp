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
    auto [counts, sign] = this->GetMeterCounts();
    switch (sign) {
        case CountSign::Divide: {
            // make sure that there is no division by zero
            std::for_each(counts.begin(), counts.end(), [](int &elem) {
                if (!elem) elem = 1;
            });
            int result = std::accumulate(std::next(counts.begin()), counts.end(), *counts.begin(), std::divides<int>());
            if (!result) result = 1;
            return result;
        }
        case CountSign::Minus: {
            int result = std::accumulate(std::next(counts.begin()), counts.end(), *counts.begin(), std::minus<int>());
            if (result <= 0) result = 1;
            return result;
        }
        case CountSign::Multiply: {
            int result = std::accumulate(counts.begin(), counts.end(), 1, std::multiplies<int>());
            if (!result) result = 1;
            return result;
        }
        case CountSign::Plus: {
            return std::accumulate(counts.begin(), counts.end(), 0, std::plus<int>());
        }
        case CountSign::None:
        default: break;
    }

    return counts.front();
}

std::pair<std::vector<int>, MeterSig::CountSign> MeterSig::GetMeterCounts() const 
{
    const std::string count = this->GetCount();
    std::regex re("[\\*\\+/-]");
    std::sregex_token_iterator first{ count.begin(), count.end(), re, -1 }, last;
    std::vector<std::string> tokens{ first, last };

    // Since there is currently no need for implementation of complex calculus within metersig, only one opperation will
    // be supported in the meter count. Caclulation will be based on the first mathematical operator in the string
    MeterSig::CountSign sign = CountSign::None;
    const size_t pos = count.find_first_of("+-*/");
    if (pos != std::string::npos) {
        if (count[pos] == '/') {
            sign = CountSign::Divide;
        }
        else if (count[pos] == '*') {
            sign = CountSign::Multiply;
        }
        else if (count[pos] == '+') {
            sign = CountSign::Plus;
        }
        else if (count[pos] == '-') {
            sign = CountSign::Minus;
        }
    }
    std::vector<int> result;
    std::for_each(tokens.begin(), tokens.end(),
        [&result](const std::string &elem) { result.emplace_back(std::atoi(elem.c_str())); });
    return { result, sign };
}

void MeterSig::SetMeterCounts(const std::vector<int> &counts, MeterSig::CountSign sign) 
{
    std::stringstream output;
    for (const int count : counts) {
        output << count;
        switch (sign) {
            case CountSign::Divide: output << '\\'; break;
            case CountSign::Minus: output << '-'; break;
            case CountSign::Multiply: output << '*'; break;
            case CountSign::Plus: output << '+'; break;
            case CountSign::None:
            default: break;
        }
    }

    this->SetCount(output.str());
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

std::pair<wchar_t, wchar_t> MeterSig::GetEnclosingGlyphs(bool smallGlyph) const
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

int MeterSig::LayerCountInTimeSpan(FunctorParams *functorParams)
{
    LayerCountInTimeSpanParams *params = vrv_params_cast<LayerCountInTimeSpanParams *>(functorParams);
    assert(params);

    params->m_meterSig = this;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
