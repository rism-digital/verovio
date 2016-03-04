/////////////////////////////////////////////////////////////////////////////
// Name:        verse.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "verse.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "aligner.h"
#include "editorial.h"
#include "layer.h"
#include "staff.h"
#include "syl.h"

namespace vrv {

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

Verse::Verse() : LayerElement("verse-"), AttCommon()
{
    RegisterAttClass(ATT_COMMON);

    Reset();
}

Verse::~Verse()
{
}

void Verse::Reset()
{
    LayerElement::Reset();
    ResetCommon();
}

void Verse::AddLayerElement(vrv::LayerElement *element)
{
    assert(dynamic_cast<Syl *>(element) || dynamic_cast<EditorialElement *>(element));
    element->SetParent(this);
    m_children.push_back(element);
    Modify();
}

//----------------------------------------------------------------------------
// Verse functor methods
//----------------------------------------------------------------------------

int Verse::AlignVertically(ArrayPtrVoid *params)
{
    // param 0: the systemAligner
    // param 1: the staffIdx (unused)
    // param 2: the staffN
    SystemAligner **systemAligner = static_cast<SystemAligner **>((*params).at(0));
    int *staffN = static_cast<int *>((*params).at(2));

    // this gets (or creates) the measureAligner for the measure
    StaffAlignment *alignment = (*systemAligner)->GetStaffAlignmentForStaffN(*staffN);

    if (!alignment) return FUNCTOR_CONTINUE;

    // Add the number count
    alignment->SetVerseCount(this->GetN());

    return FUNCTOR_CONTINUE;
}

int Verse::PrepareProcessingLists(ArrayPtrVoid *params)
{
    // param 0: the IntTree* for staff/layer/verse
    // param 1: the IntTree* for staff/layer (unused)
    IntTree *tree = static_cast<IntTree *>((*params).at(0));
    // Alternate solution with StaffN_LayerN_VerseN_t
    // StaffN_LayerN_VerseN_t *tree = static_cast<StaffN_LayerN_VerseN_t*>((*params).at(0));

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    Layer *layer = dynamic_cast<Layer *>(this->GetFirstParent(LAYER));
    assert(staff && layer);

    tree->child[staff->GetN()].child[layer->GetN()].child[this->GetN()];
    // Alternate solution with StaffN_LayerN_VerseN_t
    //(*tree)[ staff->GetN() ][ layer->GetN() ][ this->GetN() ] = true;

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
