/////////////////////////////////////////////////////////////////////////////
// Name:        staffgrp.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "staffgrp.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "instrdef.h"
#include "label.h"
#include "labelabbr.h"
#include "staffdef.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// StaffGrp
//----------------------------------------------------------------------------

StaffGrp::StaffGrp()
    : Object("staffgrp-")
    , ObjectListInterface()
    , AttBasic()
    , AttLabelled()
    , AttStaffGroupingSym()
    , AttStaffGrpVis()
    , AttTyped()
{
    RegisterAttClass(ATT_BASIC);
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_STAFFGROUPINGSYM);
    RegisterAttClass(ATT_STAFFGRPVIS);
    RegisterAttClass(ATT_TYPED);

    Reset();
}

StaffGrp::~StaffGrp() {}

void StaffGrp::Reset()
{
    Object::Reset();
    ResetBasic();
    ResetLabelled();
    ResetStaffGroupingSym();
    ResetStaffGrpVis();
    ResetTyped();
}

void StaffGrp::AddChild(Object *child)
{
    if (child->Is(INSTRDEF)) {
        assert(dynamic_cast<InstrDef *>(child));
    }
    else if (child->Is(LABEL)) {
        assert(dynamic_cast<Label *>(child));
    }
    else if (child->Is(LABELABBR)) {
        assert(dynamic_cast<LabelAbbr *>(child));
    }
    else if (child->Is(STAFFDEF)) {
        assert(dynamic_cast<StaffDef *>(child));
    }
    else if (child->Is(STAFFGRP)) {
        assert(dynamic_cast<StaffGrp *>(child));
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

void StaffGrp::FilterList(ListOfObjects *childList)
{
    // We want to keep only staffDef
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->Is(STAFFDEF)) {
            iter = childList->erase(iter);
        }
        else {
            ++iter;
        }
    }
}

} // namespace vrv
