/////////////////////////////////////////////////////////////////////////////
// Name:        staffgrp.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "staffgrp.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "editorial.h"
#include "functorparams.h"
#include "instrdef.h"
#include "label.h"
#include "labelabbr.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// StaffGrp
//----------------------------------------------------------------------------

static const ClassRegistrar<StaffGrp> s_factory("staffGrp", STAFFGRP);

StaffGrp::StaffGrp()
    : Object(STAFFGRP, "staffgrp-")
    , ObjectListInterface()
    , AttBarring()
    , AttBasic()
    , AttLabelled()
    , AttNNumberLike()
    , AttStaffGroupingSym()
    , AttStaffGrpVis()
    , AttTyped()
{
    this->RegisterAttClass(ATT_BARRING);
    this->RegisterAttClass(ATT_BASIC);
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_NNUMBERLIKE);
    this->RegisterAttClass(ATT_STAFFGROUPINGSYM);
    this->RegisterAttClass(ATT_STAFFGRPVIS);
    this->RegisterAttClass(ATT_TYPED);

    this->Reset();
}

StaffGrp::~StaffGrp() {}

void StaffGrp::Reset()
{
    Object::Reset();
    this->ResetBarring();
    this->ResetBasic();
    this->ResetLabelled();
    this->ResetNNumberLike();
    this->ResetStaffGroupingSym();
    this->ResetStaffGrpVis();
    this->ResetTyped();

    m_drawingVisibility = OPTIMIZATION_NONE;
    m_groupSymbol = NULL;
}

bool StaffGrp::IsSupportedChild(Object *child)
{
    if (child->Is(GRPSYM)) {
        assert(dynamic_cast<GrpSym *>(child));
    }
    else if (child->Is(INSTRDEF)) {
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
        return false;
    }
    return true;
}

void StaffGrp::FilterList(ListOfConstObjects &childList) const
{
    // We want to keep only staffDef
    ListOfConstObjects::iterator iter = childList.begin();

    while (iter != childList.end()) {
        if (!(*iter)->Is(STAFFDEF)) {
            iter = childList.erase(iter);
        }
        else {
            ++iter;
        }
    }
}

int StaffGrp::GetMaxStaffSize() const
{
    const ListOfConstObjects &childList = this->GetList(this);

    if (childList.empty()) return 100;

    int max = 0;

    const StaffDef *staffDef = NULL;
    for (auto &child : childList) {
        staffDef = vrv_cast<const StaffDef *>(child);
        assert(staffDef);
        if (staffDef->HasScale() && staffDef->GetScale() >= max) {
            max = staffDef->GetScale();
        }
        else {
            max = 100;
        }
    }

    return max;
}

std::pair<StaffDef *, StaffDef *> StaffGrp::GetFirstLastStaffDef()
{
    const auto [firstDef, lastDef] = std::as_const(*this).GetFirstLastStaffDef();
    return { const_cast<StaffDef *>(firstDef), const_cast<StaffDef *>(lastDef) };
}

std::pair<const StaffDef *, const StaffDef *> StaffGrp::GetFirstLastStaffDef() const
{
    const ListOfConstObjects &staffDefs = this->GetList(this);
    if (staffDefs.empty()) {
        return { NULL, NULL };
    }

    const StaffDef *firstDef = NULL;
    ListOfConstObjects::const_iterator iter;
    for (iter = staffDefs.begin(); iter != staffDefs.end(); ++iter) {
        const StaffDef *staffDef = vrv_cast<const StaffDef *>(*iter);
        assert(staffDef);
        if (staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN) {
            firstDef = staffDef;
            break;
        }
    }

    const StaffDef *lastDef = NULL;
    ListOfConstObjects::const_reverse_iterator riter;
    for (riter = staffDefs.rbegin(); riter != staffDefs.rend(); ++riter) {
        const StaffDef *staffDef = vrv_cast<const StaffDef *>(*riter);
        assert(staffDef);
        if (staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN) {
            lastDef = staffDef;
            break;
        }
    }

    return { firstDef, lastDef };
}

void StaffGrp::SetGroupSymbol(GrpSym *grpSym)
{
    if (grpSym) {
        m_groupSymbol = grpSym;
    }
}

bool StaffGrp::HasLabelInfo() const
{
    return (this->FindDescendantByType(LABEL, 1));
}

bool StaffGrp::HasLabelAbbrInfo() const
{
    return (this->FindDescendantByType(LABELABBR, 1));
}

Label *StaffGrp::GetLabel()
{
    return const_cast<Label *>(std::as_const(*this).GetLabel());
}

const Label *StaffGrp::GetLabel() const
{
    // Always check if HasLabelInfo() is true before asking for it
    const Label *label = vrv_cast<const Label *>(this->FindDescendantByType(LABEL, 1));
    assert(label);
    return label;
}

Label *StaffGrp::GetLabelCopy() const
{
    // Always check if HasClefInfo() is true before asking for a clone
    Label *clone = dynamic_cast<Label *>(this->GetLabel()->Clone());
    clone->CloneReset();
    assert(clone);
    return clone;
}

LabelAbbr *StaffGrp::GetLabelAbbr()
{
    return const_cast<LabelAbbr *>(std::as_const(*this).GetLabelAbbr());
}

const LabelAbbr *StaffGrp::GetLabelAbbr() const
{
    // Always check if HasLabelAbbrInfo() is true before asking for it
    const LabelAbbr *labelAbbr = vrv_cast<const LabelAbbr *>(this->FindDescendantByType(LABELABBR, 1));
    assert(labelAbbr);
    return labelAbbr;
}

LabelAbbr *StaffGrp::GetLabelAbbrCopy() const
{
    // Always check if HasClefInfo() is true before asking for a clone
    LabelAbbr *clone = dynamic_cast<LabelAbbr *>(this->GetLabelAbbr()->Clone());
    clone->CloneReset();
    assert(clone);
    return clone;
}

void StaffGrp::SetEverythingVisible()
{
    this->SetDrawingVisibility(OPTIMIZATION_SHOW);
    std::for_each(this->GetChildren().begin(), this->GetChildren().end(), [](Object *child) {
        if (child->Is(STAFFDEF)) {
            StaffDef *staffDef = vrv_cast<StaffDef *>(child);
            assert(staffDef);
            staffDef->SetDrawingVisibility(OPTIMIZATION_SHOW);
        }
        else if (child->Is(STAFFGRP)) {
            vrv_cast<StaffGrp *>(child)->SetEverythingVisible();
        }
    });
}

//----------------------------------------------------------------------------
// StaffGrp functor methods
//----------------------------------------------------------------------------

int StaffGrp::ScoreDefOptimizeEnd(FunctorParams *)
{
    this->SetDrawingVisibility(OPTIMIZATION_HIDDEN);

    const Object *instrDef = this->FindDescendantByType(INSTRDEF, 1);
    if (instrDef) {
        VisibleStaffDefOrGrpObject visibleStaves;
        const Object *firstVisible = this->FindDescendantByComparison(&visibleStaves, 1);
        if (firstVisible) {
            this->SetEverythingVisible();
        }

        return FUNCTOR_CONTINUE;
    }

    for (auto child : this->GetChildren()) {
        if (child->Is(STAFFDEF)) {
            StaffDef *staffDef = vrv_cast<StaffDef *>(child);
            assert(staffDef);
            if (staffDef->GetDrawingVisibility() != OPTIMIZATION_HIDDEN) {
                this->SetDrawingVisibility(OPTIMIZATION_SHOW);
                break;
            }
        }
        else if (child->Is(STAFFGRP)) {
            StaffGrp *staffGrp = vrv_cast<StaffGrp *>(child);
            assert(staffGrp);
            if (staffGrp->GetDrawingVisibility() != OPTIMIZATION_HIDDEN) {
                this->SetDrawingVisibility(OPTIMIZATION_SHOW);
                break;
            }
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
