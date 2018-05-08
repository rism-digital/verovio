/////////////////////////////////////////////////////////////////////////////
// Name:        mensur.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mensur.h" //----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "scoredefinterface.h"
#include "vrv.h"

namespace vrv {

int Mensur::s_num = 3;
int Mensur::s_numBase = 2;

//----------------------------------------------------------------------------
// Mensur
//----------------------------------------------------------------------------

Mensur::Mensur()
    : LayerElement("mensur-")
    , AttColor()
    , AttCue()
    , AttDurationRatio()
    , AttMensuralShared()
    , AttMensurLog()
    , AttMensurVis()
    , AttSlashCount()
    , AttStaffLoc()
{
    Init();
}

Mensur::Mensur(const ScoreDefInterface *mensurAttr) : LayerElement("mensur-")
{
    Init();

    // this->SetColor(mensurAttr->GetMensurColor());
    this->SetDot(mensurAttr->GetMensurDot());
    this->SetOrient(mensurAttr->GetMensurOrient());
    this->SetSign(mensurAttr->GetMensurSign());
    this->SetSlash(mensurAttr->GetMensurSlash());
    //
    this->SetModusmaior(mensurAttr->GetModusmaior());
    this->SetModusminor(mensurAttr->GetModusminor());
    this->SetProlatio(mensurAttr->GetProlatio());
    this->SetTempus(mensurAttr->GetTempus());
    //
    this->SetNum(mensurAttr->GetProportNum());
    this->SetNumbase(mensurAttr->GetProportNumbase());
    // It is unclear why we don't have mensur.num and mensur.numbase attributes
    // in att.mensura.default.log - ask Perry...
}

void Mensur::Init()
{
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_CUE);
    RegisterAttClass(ATT_DURATIONRATIO);
    RegisterAttClass(ATT_MENSURALSHARED);
    RegisterAttClass(ATT_MENSURLOG);
    RegisterAttClass(ATT_MENSURVIS);
    RegisterAttClass(ATT_SLASHCOUNT);
    RegisterAttClass(ATT_STAFFLOC);

    Reset();
}

Mensur::~Mensur() {}

void Mensur::Reset()
{
    LayerElement::Reset();
    ResetColor();
    ResetCue();
    ResetDurationRatio();
    ResetMensuralShared();
    ResetMensurLog();
    ResetMensurVis();
    ResetSlashCount();
    ResetStaffLoc();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Mensur::FindSpaceInReferenceAlignments(FunctorParams *functorParams)
{
    FindSpaceInAlignmentParams *params = dynamic_cast<FindSpaceInAlignmentParams *>(functorParams);
    assert(params);

    params->m_mensur = this;

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
