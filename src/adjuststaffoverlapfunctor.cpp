/////////////////////////////////////////////////////////////////////////////
// Name:        adjuststaffoverlapfunctor.cpp
// Author:      David Bauer
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "adjuststaffoverlapfunctor.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AdjustStaffOverlapFunctor
//----------------------------------------------------------------------------

AdjustStaffOverlapFunctor::AdjustStaffOverlapFunctor(Doc *doc) : DocFunctor(doc)
{
    m_previous = NULL;
}

FunctorCode AdjustStaffOverlapFunctor::VisitStaffAlignment(StaffAlignment *staffAlignment)
{
    return FUNCTOR_CONTINUE;
}

FunctorCode AdjustStaffOverlapFunctor::VisitSystem(System *system)
{
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
