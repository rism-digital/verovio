/////////////////////////////////////////////////////////////////////////////
// Name:        dynam.cpp
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "dynam.h"

//----------------------------------------------------------------------------

#include <assert.h>

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
    : ControlElement("dynam-"), TextListInterface(), TextDirInterface(), TimeSpanningInterface(), AttVerticalGroup()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_VERTICALGROUP);

    Reset();
}

Dynam::~Dynam() {}

void Dynam::Reset()
{
    ControlElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
    AttVerticalGroup::ResetVerticalGroup();
}

void Dynam::AddChild(Object *child)
{
    if (child->Is({ REND, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

bool Dynam::IsSymbolOnly()
{
    m_symbolStr = L"";
    std::wstring str = this->GetText(this);
    if (str.find_first_not_of(L"fpmrszn") == std::string::npos) {
        m_symbolStr = str;
        return true;
    }
    return false;
}

std::wstring Dynam::GetSymbolStr() const
{
    std::wstring dynam;
    if (m_symbolStr == L"p")
        dynam.push_back(SMUFL_E520_dynamicPiano);
    else if (m_symbolStr == L"m")
        dynam.push_back(SMUFL_E521_dynamicMezzo);
    else if (m_symbolStr == L"f")
        dynam.push_back(SMUFL_E522_dynamicForte);
    else if (m_symbolStr == L"r")
        dynam.push_back(SMUFL_E523_dynamicRinforzando);
    else if (m_symbolStr == L"s")
        dynam.push_back(SMUFL_E524_dynamicSforzando);
    else if (m_symbolStr == L"z")
        dynam.push_back(SMUFL_E525_dynamicZ);
    else if (m_symbolStr == L"n")
        dynam.push_back(SMUFL_E526_dynamicNiente);
    else if (m_symbolStr == L"pppppp")
        dynam.push_back(SMUFL_E527_dynamicPPPPPP);
    else if (m_symbolStr == L"ppppp")
        dynam.push_back(SMUFL_E528_dynamicPPPPP);
    else if (m_symbolStr == L"pppp")
        dynam.push_back(SMUFL_E529_dynamicPPPP);
    else if (m_symbolStr == L"ppp")
        dynam.push_back(SMUFL_E52A_dynamicPPP);
    else if (m_symbolStr == L"pp")
        dynam.push_back(SMUFL_E52B_dynamicPP);
    else if (m_symbolStr == L"mp")
        dynam.push_back(SMUFL_E52C_dynamicMP);
    else if (m_symbolStr == L"mf")
        dynam.push_back(SMUFL_E52D_dynamicMF);
    else if (m_symbolStr == L"pf")
        dynam.push_back(SMUFL_E52E_dynamicPF);
    else if (m_symbolStr == L"ff")
        dynam.push_back(SMUFL_E52F_dynamicFF);
    else if (m_symbolStr == L"fff")
        dynam.push_back(SMUFL_E530_dynamicFFF);
    else if (m_symbolStr == L"ffff")
        dynam.push_back(SMUFL_E531_dynamicFFFF);
    else if (m_symbolStr == L"fffff")
        dynam.push_back(SMUFL_E532_dynamicFFFFF);
    else if (m_symbolStr == L"ffffff")
        dynam.push_back(SMUFL_E533_dynamicFFFFFF);
    else if (m_symbolStr == L"fp")
        dynam.push_back(SMUFL_E534_dynamicFortePiano);
    else if (m_symbolStr == L"fz")
        dynam.push_back(SMUFL_E535_dynamicForzando);
    else if (m_symbolStr == L"sf")
        dynam.push_back(SMUFL_E536_dynamicSforzando1);
    else if (m_symbolStr == L"sfp")
        dynam.push_back(SMUFL_E537_dynamicSforzandoPiano);
    else if (m_symbolStr == L"sfpp")
        dynam.push_back(SMUFL_E538_dynamicSforzandoPianissimo);
    else if (m_symbolStr == L"sfz")
        dynam.push_back(SMUFL_E539_dynamicSforzato);
    else if (m_symbolStr == L"sfzp")
        dynam.push_back(SMUFL_E53A_dynamicSforzatoPiano);
    else if (m_symbolStr == L"sffz")
        dynam.push_back(SMUFL_E53B_dynamicSforzatoFF);
    else if (m_symbolStr == L"rf")
        dynam.push_back(SMUFL_E53C_dynamicRinforzando1);
    else if (m_symbolStr == L"rfz")
        dynam.push_back(SMUFL_E53D_dynamicRinforzando2);

    if (!dynam.empty()) return dynam;

    // Otherwise replace it letter by letter
    dynam = m_symbolStr;
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
    PrepareFloatingGrpsParams *params = dynamic_cast<PrepareFloatingGrpsParams *>(functorParams);
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
