/////////////////////////////////////////////////////////////////////////////
// Name:        mensur.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "mensur.h"
#include "scoredefinterface.h"

namespace vrv {

int Mensur::s_num = 3;
int Mensur::s_numBase = 2;

//----------------------------------------------------------------------------
// Mensur
//----------------------------------------------------------------------------

Mensur::Mensur()
    : LayerElement("mensur-")
    , AttDurationRatio()
    , AttMensuralShared()
    , AttMensurLog()
    , AttMensurVis()
    , AttRelativesize()
    , AttSlashcount()
{
    Init();
}

Mensur::Mensur(const ScoreDefInterface *mensurAttr) : LayerElement("mensur-")
{
    Init();

    this->SetDot(mensurAttr->GetMensurDot());
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
    RegisterAttClass(ATT_DURATIONRATIO);
    RegisterAttClass(ATT_MENSURALSHARED);
    RegisterAttClass(ATT_MENSURLOG);
    RegisterAttClass(ATT_METERSIGVIS);
    RegisterAttClass(ATT_RELATIVESIZE);
    RegisterAttClass(ATT_SLASHCOUNT);

    Reset();
}

Mensur::~Mensur()
{
}

void Mensur::Reset()
{
    LayerElement::Reset();
    ResetDurationRatio();
    ResetMensuralShared();
    ResetMensurLog();
    ResetMensurVis();
    ResetRelativesize();
    ResetSlashcount();
}

} // namespace vrv
