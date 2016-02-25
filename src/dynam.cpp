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
#include "text.h"

namespace vrv {

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
    // assert(dynamic_cast<TextElement *>(element) || dynamic_cast<EditorialElement *>(element));
    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}

bool Dynam::IsSymbolOnly()
{
    std::wstring str = this->GetText(this);
    return str.find_first_not_of(L"fpmrsz") == std::string::npos;
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
