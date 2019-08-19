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
#include "functorparams.h"
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

    m_drawingVisibility = OPTIMIZATION_NONE;
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

void StaffGrp::FilterList(ArrayOfObjects *childList)
{
    // We want to keep only staffDef
    ArrayOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->Is(STAFFDEF)) {
            iter = childList->erase(iter);
        }
        else {
            ++iter;
        }
    }
}

int StaffGrp::GetMaxStaffSize()
{
    this->ResetList(this);
    const ArrayOfObjects *childList = this->GetList(this);

    if (childList->empty()) return 100;

    int max = 0;

    StaffDef *staffDef = NULL;
    for (auto &child : *childList) {
        staffDef = dynamic_cast<StaffDef *>(child);
        assert(staffDef);
        if (staffDef->GetScale() > max) {
            max = staffDef->GetScale();
        }
    }

    if (max == 0) return 100;

    return max;
}

//----------------------------------------------------------------------------
// StaffGrp functor methods
//----------------------------------------------------------------------------

int StaffGrp::OptimizeScoreDefEnd(FunctorParams *)
{
    // OptimizeScoreDefParams *params = dynamic_cast<OptimizeScoreDefParams *>(functorParams);
    // assert(params);

    this->SetDrawingVisibility(OPTIMIZATION_HIDDEN);

    for (auto &child : m_children) {
        if (child->Is(STAFFDEF)) {
            StaffDef *staffDef = dynamic_cast<StaffDef *>(child);
            assert(staffDef);
            if (staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN) {
                this->SetDrawingVisibility(OPTIMIZATION_SHOW);
                break;
            }
        }
        else if (child->Is(STAFFGRP)) {
            StaffGrp *staffGrp = dynamic_cast<StaffGrp *>(child);
            assert(staffGrp);
            if (staffGrp->GetDrawingVisibility() != OPTIMIZATION_HIDDEN) {
                this->SetDrawingVisibility(OPTIMIZATION_SHOW);
                break;
            }
        }
    }

    if ((this->GetSymbol() == staffGroupingSym_SYMBOL_brace) && (this->GetDrawingVisibility() != OPTIMIZATION_HIDDEN)) {
        for (auto &child : m_children) {
            if (child->Is(STAFFDEF)) {
                StaffDef *staffDef = dynamic_cast<StaffDef *>(child);
                assert(staffDef);
                staffDef->SetDrawingVisibility(OPTIMIZATION_SHOW);
            }
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
