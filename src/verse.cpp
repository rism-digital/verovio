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

#include "verticalaligner.h"
#include "editorial.h"
#include "functorparams.h"
#include "layer.h"
#include "staff.h"
#include "syl.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

Verse::Verse() : LayerElement("verse-"), AttColor(), AttLang(), AttTypography()
{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_LANG);
    RegisterAttClass(ATT_TYPOGRAPHY);

    Reset();
}

Verse::~Verse()
{
}

void Verse::Reset()
{
    LayerElement::Reset();
    ResetColor();
    ResetLang();
    ResetTypography();
}

void Verse::AddChild(Object *child)
{
    if (child->Is(SYL)) {
        assert(dynamic_cast<Syl *>(child));
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

//----------------------------------------------------------------------------
// Verse functor methods
//----------------------------------------------------------------------------

int Verse::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = dynamic_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    // this gets (or creates) the measureAligner for the measure
    StaffAlignment *alignment = params->m_systemAligner->GetStaffAlignmentForStaffN(params->m_staffN);

    if (!alignment) return FUNCTOR_CONTINUE;

    // Add the number count
    alignment->SetVerseCount(this->GetN());

    return FUNCTOR_CONTINUE;
}

int Verse::PrepareProcessingLists(FunctorParams *functorParams)
{
    PrepareProcessingListsParams *params = dynamic_cast<PrepareProcessingListsParams *>(functorParams);
    assert(params);
    // StaffN_LayerN_VerseN_t *tree = static_cast<StaffN_LayerN_VerseN_t*>((*params).at(0));

    Staff *staff = dynamic_cast<Staff *>(this->GetFirstParent(STAFF));
    Layer *layer = dynamic_cast<Layer *>(this->GetFirstParent(LAYER));
    assert(staff && layer);

    params->m_verseTree.child[staff->GetN()].child[layer->GetN()].child[this->GetN()];
    // Alternate solution with StaffN_LayerN_VerseN_t
    //(*tree)[ staff->GetN() ][ layer->GetN() ][ this->GetN() ] = true;

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
