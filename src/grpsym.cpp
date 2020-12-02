/////////////////////////////////////////////////////////////////////////////
// Name:        grpsym.cpp
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "grpsym.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "functorparams.h"
#include "staffdef.h"
#include "staffgrp.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// GrpSym
//----------------------------------------------------------------------------

GrpSym::GrpSym() : Object("grpsym-"), AttColor(), AttGrpSymLog(), AttStaffGroupingSym(), AttStartId(), AttStartEndId()
{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_GRPSYMLOG);
    RegisterAttClass(ATT_STAFFGROUPINGSYM);
    RegisterAttClass(ATT_STARTID);
    RegisterAttClass(ATT_STARTENDID);

    Reset();
}

GrpSym::~GrpSym() {}

void GrpSym::Reset()
{
    Object::Reset();
    ResetColor();
    ResetGrpSymLog();
    ResetStaffGroupingSym();
    ResetStartId();
    ResetStartEndId();

    m_startDef = NULL;
    m_endDef = NULL;
}

void GrpSym::SetStartDef(StaffDef *start)
{
    if (start && !m_startDef) m_startDef = start;
}

void GrpSym::SetEndDef(StaffDef *end)
{
    if (end && !m_endDef) m_endDef = end;
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

int GrpSym::PrepareGroupSymbols(FunctorParams *)
{
    // For the grpSym that is encoded in the scope of the staffGrp just get first and last staffDefs and set then as
    // starting and ending points
    if (GetParent()->Is(STAFFGRP)) {
        StaffGrp *staffGrp = vrv_cast<StaffGrp *>(GetParent());
        assert(staffGrp);
        auto [firstDef, lastDef] = staffGrp->GetFirstLastStaffDef();
        if (firstDef && lastDef) {
            SetStartDef(firstDef);
            SetEndDef(lastDef);
            staffGrp->SetGroupSymbol(this);
        }
    }
    // For the grpSym that is encoded in the scope of the scoreDef we need to find corresponding staffDefs with matching
    // @startid and @endid. We also need to make sure that @level attribute is adhered to, hence we limit search depth.
    // Finally, we need to make sure that both starting and ending elements have the same parent (since we cannot draw
    // cross-group grpSym)
    else if (GetParent()->Is(SCOREDEF)) {
        ScoreDef *scoreDef = vrv_cast<ScoreDef *>(GetParent());
        assert(scoreDef);

        const std::string startId = ExtractUuidFragment(GetStartid());
        const std::string endId = ExtractUuidFragment(GetEndid());
        const int level = GetLevel();

        UuidComparison compare(STAFFDEF, startId);
        StaffDef *start = vrv_cast<StaffDef *>(scoreDef->FindDescendantByComparison(&compare, level));
        compare.SetUuid(endId);
        StaffDef *end = vrv_cast<StaffDef *>(scoreDef->FindDescendantByComparison(&compare, level));

        if (!start || !end) {
            LogWarning("Could not find startid/endid on level %d for <'%s'>", level, GetUuid().c_str());
            return FUNCTOR_CONTINUE;
        }

        if (start->GetParent() != end->GetParent()) {
            LogWarning("<'%s'> has mismatching parents for startid:<'%s'> and endid:<'%s'>", GetUuid().c_str(),
                startId.c_str(), endId.c_str());
            return FUNCTOR_CONTINUE;
        }

        SetStartDef(start);
        SetEndDef(end);
        StaffGrp *staffGrp = vrv_cast<StaffGrp *>(start->GetParent());
        staffGrp->SetGroupSymbol(this);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
