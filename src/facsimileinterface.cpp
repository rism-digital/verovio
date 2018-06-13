/////////////////////////////////////////////////////////////////////////////
// Name:        facsimileinterface.cpp
// Author:      Juliette Regimbal 
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "facsimileinterface.h"

//---------------------------------------------------------------------------

#include <assert.h>

//---------------------------------------------------------------------------

#include "facsimile.h"
#include "view.h"
#include "vrv.h"

namespace vrv {

FacsimileInterface::FacsimileInterface() : Interface(), AttFacsimile()
{
    RegisterInterfaceAttClass(ATT_FACSIMILE);
    Reset();
}

FacsimileInterface::~FacsimileInterface() {}

void FacsimileInterface::Reset()
{
    ResetFacsimile();
    this->SetZone(nullptr);
}

int FacsimileInterface::GetDrawingX() const {
    assert(m_zone);
    return m_zone->GetUlx() * m_zone->m_facsScale;
}

int FacsimileInterface::GetDrawingY() const {
    assert(m_zone);
    int y = m_zone->GetLogicalUly() * m_zone->m_facsScale;
    LogMessage("%d", y);
    return y;
}

int FacsimileInterface::GetWidth() const {
    assert(m_zone);
    return m_zone->m_facsScale * (m_zone->GetLrx() - m_zone->GetUlx());
}
}
