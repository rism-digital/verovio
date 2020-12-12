/////////////////////////////////////////////////////////////////////////////
// Name:        dynam.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "dynam.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <sstream>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functorparams.h"
#include "hairpin.h"
#include "smufl.h"
#include "text.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

#define DYNAM_CHARS 7
std::wstring dynamChars[] = { L"p", L"m", L"f", L"r", L"s", L"z", L"n" };
std::wstring dynamSmufl[] = { L"\uE520", L"\uE521", L"\uE522", L"\uE523", L"\uE524", L"\uE525", L"\uE526" };

//----------------------------------------------------------------------------
// Dynam
//----------------------------------------------------------------------------

Dynam::Dynam()
    : ControlElement("dynam-")
    , TextListInterface()
    , TextDirInterface()
    , TimeSpanningInterface()
    , AttExtender()
    , AttLineRendBase()
    , AttMidiValue()
    , AttMidiValue2()
    , AttVerticalGroup()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_EXTENDER);
    RegisterAttClass(ATT_LINERENDBASE);
    RegisterAttClass(ATT_MIDIVALUE);
    RegisterAttClass(ATT_MIDIVALUE2);
    RegisterAttClass(ATT_VERTICALGROUP);

    Reset();
}

Dynam::~Dynam() {}

void Dynam::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
    ResetExtender();
    ResetLineRendBase();
    ResetVerticalGroup();
}

bool Dynam::IsSupportedChild(Object *child)
{
    if (child->Is({ REND, LB, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

bool Dynam::IsSymbolOnly()
{
    m_symbolStr = L"";
    std::wstring str = this->GetText(this);
    if (Dynam::IsSymbolOnly(str)) {
        m_symbolStr = str;
        return true;
    }
    return false;
}

std::wstring Dynam::GetSymbolStr() const
{
    return Dynam::GetSymbolStr(m_symbolStr);
}

//----------------------------------------------------------------------------
// Static methods for Dynam
//----------------------------------------------------------------------------

bool Dynam::GetSymbolsInStr(std::wstring &str, ArrayOfStringDynamTypePairs &tokens)
{
    tokens.clear();

    std::wistringstream iss(str);
    std::wstring token;
    std::wstring text;
    bool hasSymbols = false;
    while (std::getline(iss, token, L' ')) {
        // if (token.size() == 0) continue;
        if (Dynam::IsSymbolOnly(token)) {
            hasSymbols = true;
            if (tokens.size() > 0) {
                // previous one is not a symbol, add a space to it
                if (tokens.back().second == false) {
                    tokens.back().first = tokens.back().first + L" ";
                }
                // previous one it also a symbol, add a space in between
                else {
                    tokens.push_back(std::make_pair(L" ", false));
                }
            }
            // Add it in all cases
            tokens.push_back(std::make_pair(token, true));
        }
        else {
            if (tokens.size() > 0) {
                // previous one is not a symbol, append token to it with a space
                if (tokens.back().second == false) {
                    tokens.back().first = tokens.back().first + L" " + token;
                }
                // previous one is not a symbol, add it separately but with a space
                else {
                    tokens.push_back(std::make_pair(L" " + token, false));
                }
            }
            // First one, just add it
            else {
                tokens.push_back(std::make_pair(token, false));
            }
        }
    }

    return hasSymbols;
}

bool Dynam::IsSymbolOnly(const std::wstring &str)
{
    if (str.empty()) return false;

    if (str.find_first_not_of(L"fpmrszn") == std::string::npos) {
        return true;
    }
    return false;
}

std::wstring Dynam::GetSymbolStr(const std::wstring &str)
{
    std::wstring dynam;
    if (str == L"p")
        dynam.push_back(SMUFL_E520_dynamicPiano);
    else if (str == L"m")
        dynam.push_back(SMUFL_E521_dynamicMezzo);
    else if (str == L"f")
        dynam.push_back(SMUFL_E522_dynamicForte);
    else if (str == L"r")
        dynam.push_back(SMUFL_E523_dynamicRinforzando);
    else if (str == L"s")
        dynam.push_back(SMUFL_E524_dynamicSforzando);
    else if (str == L"z")
        dynam.push_back(SMUFL_E525_dynamicZ);
    else if (str == L"n")
        dynam.push_back(SMUFL_E526_dynamicNiente);
    else if (str == L"pppppp")
        dynam.push_back(SMUFL_E527_dynamicPPPPPP);
    else if (str == L"ppppp")
        dynam.push_back(SMUFL_E528_dynamicPPPPP);
    else if (str == L"pppp")
        dynam.push_back(SMUFL_E529_dynamicPPPP);
    else if (str == L"ppp")
        dynam.push_back(SMUFL_E52A_dynamicPPP);
    else if (str == L"pp")
        dynam.push_back(SMUFL_E52B_dynamicPP);
    else if (str == L"mp")
        dynam.push_back(SMUFL_E52C_dynamicMP);
    else if (str == L"mf")
        dynam.push_back(SMUFL_E52D_dynamicMF);
    else if (str == L"pf")
        dynam.push_back(SMUFL_E52E_dynamicPF);
    else if (str == L"ff")
        dynam.push_back(SMUFL_E52F_dynamicFF);
    else if (str == L"fff")
        dynam.push_back(SMUFL_E530_dynamicFFF);
    else if (str == L"ffff")
        dynam.push_back(SMUFL_E531_dynamicFFFF);
    else if (str == L"fffff")
        dynam.push_back(SMUFL_E532_dynamicFFFFF);
    else if (str == L"ffffff")
        dynam.push_back(SMUFL_E533_dynamicFFFFFF);
    else if (str == L"fp")
        dynam.push_back(SMUFL_E534_dynamicFortePiano);
    else if (str == L"fz")
        dynam.push_back(SMUFL_E535_dynamicForzando);
    else if (str == L"sf")
        dynam.push_back(SMUFL_E536_dynamicSforzando1);
    else if (str == L"sfp")
        dynam.push_back(SMUFL_E537_dynamicSforzandoPiano);
    else if (str == L"sfpp")
        dynam.push_back(SMUFL_E538_dynamicSforzandoPianissimo);
    else if (str == L"sfz")
        dynam.push_back(SMUFL_E539_dynamicSforzato);
    else if (str == L"sfzp")
        dynam.push_back(SMUFL_E53A_dynamicSforzatoPiano);
    else if (str == L"sffz")
        dynam.push_back(SMUFL_E53B_dynamicSforzatoFF);
    else if (str == L"rf")
        dynam.push_back(SMUFL_E53C_dynamicRinforzando1);
    else if (str == L"rfz")
        dynam.push_back(SMUFL_E53D_dynamicRinforzando2);

    if (!dynam.empty()) return dynam;

    // Otherwise replace it letter by letter
    dynam = str;
    int i;
    std::wstring from, to;
    for (i = 0; i < DYNAM_CHARS; ++i) {
        from = dynamChars[i];
        to = dynamSmufl[i];
        for (size_t pos = 0; (pos = dynam.find(from, pos)) != std::string::npos; pos += to.size())
            dynam.replace(pos, from.size(), to);
    }
    return dynam;
}

//----------------------------------------------------------------------------
// Dynam functor methods
//----------------------------------------------------------------------------

int Dynam::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = vrv_params_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    if (this->HasVgrp()) {
        this->SetDrawingGrpId(-this->GetVgrp());
    }

    // Keep it for linking only if start is resolved
    if (!this->GetStart()) return FUNCTOR_CONTINUE;

    params->m_dynams.push_back(this);

    for (auto &hairpin : params->m_hairpins) {
        if ((hairpin->GetEnd() == this->GetStart()) && (hairpin->GetStaff() == this->GetStaff())) {
            if (!hairpin->GetRightLink()) hairpin->SetRightLink(this);
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
