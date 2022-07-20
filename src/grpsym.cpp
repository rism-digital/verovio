/////////////////////////////////////////////////////////////////////////////
// Name:        grpsym.cpp
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "grpsym.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "functorparams.h"
#include "staffgrp.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// GrpSym
//----------------------------------------------------------------------------

static const ClassRegistrar<GrpSym> s_factory("grpSym", GRPSYM);

GrpSym::GrpSym()
    : Object(GRPSYM, "grpsym-"), AttColor(), AttGrpSymLog(), AttStaffGroupingSym(), AttStartId(), AttStartEndId()
{
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_GRPSYMLOG);
    this->RegisterAttClass(ATT_STAFFGROUPINGSYM);
    this->RegisterAttClass(ATT_STARTID);
    this->RegisterAttClass(ATT_STARTENDID);

    this->Reset();
}

GrpSym::~GrpSym() {}

void GrpSym::Reset()
{
    Object::Reset();
    this->ResetColor();
    this->ResetGrpSymLog();
    this->ResetStaffGroupingSym();
    this->ResetStartId();
    this->ResetStartEndId();

    m_startDef = NULL;
    m_endDef = NULL;
}

void GrpSym::SetStartDef(StaffDef *start)
{
    if (start) m_startDef = start;
}

void GrpSym::SetEndDef(StaffDef *end)
{
    if (end) m_endDef = end;
}

int GrpSym::GetDrawingX() const
{
    m_cachedDrawingX = 0;
    return m_cachedDrawingX;
}

int GrpSym::GetDrawingY() const
{
    m_cachedDrawingY = 0;
    return m_cachedDrawingX;
}

//----------------------------------------------------------------------------
// GrpSym functor methods
//----------------------------------------------------------------------------

int GrpSym::ScoreDefSetGrpSym(FunctorParams *)
{
    // For the grpSym that is encoded in the scope of the staffGrp just get first and last staffDefs and set then as
    // starting and ending points
    if (this->GetParent()->Is(STAFFGRP)) {
        StaffGrp *staffGrp = vrv_cast<StaffGrp *>(this->GetParent());
        assert(staffGrp);
        auto [firstDef, lastDef] = staffGrp->GetFirstLastStaffDef();
        if (firstDef && lastDef) {
            this->SetStartDef(firstDef);
            this->SetEndDef(lastDef);
            staffGrp->SetGroupSymbol(this);
        }
    }
    // For the grpSym that is encoded in the scope of the scoreDef we need to find corresponding staffDefs with matching
    // @startid and @endid. We also need to make sure that @level attribute is adhered to, hence we limit search depth.
    // Finally, we need to make sure that both starting and ending elements have the same parent (since we cannot draw
    // cross-group grpSym)
    else if (this->GetParent()->Is(SCOREDEF)) {
        ScoreDef *scoreDef = vrv_cast<ScoreDef *>(this->GetParent());
        assert(scoreDef);

        const std::string startId = ExtractIDFragment(this->GetStartid());
        const std::string endId = ExtractIDFragment(this->GetEndid());
        const int level = this->GetLevel();

        IDComparison compare(STAFFDEF, startId);
        StaffDef *start = vrv_cast<StaffDef *>(scoreDef->FindDescendantByComparison(&compare, level));
        compare.SetID(endId);
        StaffDef *end = vrv_cast<StaffDef *>(scoreDef->FindDescendantByComparison(&compare, level));

        if (!start || !end) {
            LogWarning("Could not find startid/endid on level %d for <'%s'>", level, this->GetID().c_str());
            return FUNCTOR_CONTINUE;
        }

        if (start->GetParent() != end->GetParent()) {
            LogWarning("<'%s'> has mismatching parents for startid:<'%s'> and endid:<'%s'>", this->GetID().c_str(),
                startId.c_str(), endId.c_str());
            return FUNCTOR_CONTINUE;
        }

        this->SetStartDef(start);
        this->SetEndDef(end);
        // dynamic_cast because we never check parent type
        StaffGrp *staffGrp = dynamic_cast<StaffGrp *>(start->GetParent());
        assert(staffGrp);
        staffGrp->SetGroupSymbol(this);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
