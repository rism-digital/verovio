/////////////////////////////////////////////////////////////////////////////
// Name:        dir.h
// Author:      Laurent Pugin
// Created:     2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "dir.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "aligner.h"
#include "editorial.h"
#include "text.h"

namespace vrv {

//----------------------------------------------------------------------------
// Dir
//----------------------------------------------------------------------------

Dir::Dir() : FloatingElement("dir-"), TextListInterface(), TextDirInterface(), TimeSpanningInterface()
{
    RegisterInterface(TextDirInterface::GetAttClasses(), TextDirInterface::IsInterface());
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());

    Reset();
}

Dir::~Dir()
{
}

void Dir::Reset()
{
    FloatingElement::Reset();
    TextDirInterface::Reset();
    TimeSpanningInterface::Reset();
}

void Dir::AddTextElement(TextElement *element)
{
    assert(dynamic_cast<TextElement *>(element) || dynamic_cast<EditorialElement *>(element));
    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}

//----------------------------------------------------------------------------
// Dir functor methods
//----------------------------------------------------------------------------

int Dir::AlignVertically(ArrayPtrVoid *params)
{
    // param 0: the systemAligner
    // param 1: the staffIdx (unused)
    // param 2: the staffN (unused)
    // param 3: the doc (unused)
    SystemAligner **systemAligner = static_cast<SystemAligner **>((*params).at(0));

    std::vector<int> staffList = this->GetStaff();
    std::vector<int>::iterator iter;
    for (iter = staffList.begin(); iter != staffList.end(); iter++) {
        // this gets (or creates) the measureAligner for the measure
        StaffAlignment *alignment = (*systemAligner)->GetStaffAlignmentForStaffN(*iter);

        if (!alignment) continue;

        if (this->GetPlace() == STAFFREL_above) alignment->SetDirAbove();
        if (this->GetPlace() == STAFFREL_below) alignment->SetDirBelow();
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
