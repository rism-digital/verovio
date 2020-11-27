/////////////////////////////////////////////////////////////////////////////
// Name:        grpsym.cpp
// Author:      Andriy Makarchuk
// Created:     2020
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "grpsym.h"

namespace vrv {

//----------------------------------------------------------------------------
// GrpSym
//----------------------------------------------------------------------------

GrpSym::GrpSym() : Object("grpsym-")
    , AttStaffGroupingSym()
    , AttGrpSymLog()
    , AttStartId()
    , AttStartEndId()
{
    RegisterAttClass(ATT_STAFFGROUPINGSYM);
    RegisterAttClass(ATT_GRPSYMLOG);
    RegisterAttClass(ATT_STARTID);
    RegisterAttClass(ATT_STARTENDID);

    Reset();
}

GrpSym::~GrpSym() {}

void GrpSym::Reset() 
{
    Object::Reset();
    ResetStaffGroupingSym();
    ResetGrpSymLog();
    ResetStartId();
    ResetStartEndId();

}

} // namespace vrv