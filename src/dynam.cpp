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

#include "aligner.h"
#include "editorial.h"
#include "smufl.h"
#include "text.h"

namespace vrv {

#define DYNAM_CHARS 7
std::wstring dynamChars[] = { L"p", L"m", L"f", L"r", L"s", L"z", L"n" };
std::wstring dynamSmufl[] = { L"\uE520", L"\uE521", L"\uE522", L"\uE523", L"\uE524", L"\uE525", L"\uE526" };

//----------------------------------------------------------------------------
// Dynam
//----------------------------------------------------------------------------

Dynam::Dynam() : FloatingElement("dynam-"), TextListInterface(), TextDirInterface(), TimeSpanningInterface()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());

    Reset();
}

Dynam::~Dynam()
{
}

void Dynam::Reset()
{
    FloatingElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
}

void Dynam::AddTextElement(TextElement *element)
{
    assert(dynamic_cast<TextElement *>(element) || dynamic_cast<EditorialElement *>(element));
    element->SetParent(this);
    m_children.push_back(element);
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

std::wstring Dynam::GetSymbolStr()
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
    //
    else if (m_symbolStr == L"ff")
        dynam.push_back(SMUFL_E52F_dynamicFF);
    else if (m_symbolStr == L"fff")
        dynam.push_back(SMUFL_E530_dynamicFFF);
    else if (m_symbolStr == L"ffff")
        dynam.push_back(SMUFL_E531_dynamicFFFF);
    else if (m_symbolStr == L"fffff")
        dynam.push_back(SMUFL_E532_dynamicFFFFF);
    //

    if (!dynam.empty()) return dynam;

    // Otherwise replace it letter by letter
    dynam = m_symbolStr;
    int i;
    std::wstring from, to;
    for (i = 0; i < DYNAM_CHARS; i++) {
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

int Dynam::AlignVertically(ArrayPtrVoid *params)
{
    // param 0: the systemAligner
    // param 1: the staffIdx (unused)
    // param 2: the staffN (unused)
    SystemAligner **systemAligner = static_cast<SystemAligner **>((*params).at(0));

    std::vector<int> staffList = this->GetStaff();
    std::vector<int>::iterator iter;
    for (iter = staffList.begin(); iter != staffList.end(); iter++) {
        // this gets (or creates) the measureAligner for the measure
        StaffAlignment *alignment = (*systemAligner)->GetStaffAlignmentForStaffN(*iter);

        if (!alignment) continue;

        if (this->GetPlace() == STAFFREL_above) alignment->SetDynamAbove();
        if (this->GetPlace() == STAFFREL_below) alignment->SetDynamBelow();
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
