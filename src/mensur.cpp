/////////////////////////////////////////////////////////////////////////////
// Name:        mensur.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "mensur.h"

namespace vrv {

int Mensur::s_num = 3;
int Mensur::s_numBase = 2;

//----------------------------------------------------------------------------
// Mensur
//----------------------------------------------------------------------------

Mensur::Mensur():
	LayerElement("mensur-"),
    AttDurationRatio(),
    AttMensurLog(),
    AttMensurVis(),
    AttSlashcount()
{
    Reset();
}
    
Mensur::Mensur( MensurAttr *mensurAttr ):
    LayerElement("mensur-")
{
    Mensur();
    this->SetDot(mensurAttr->GetMensurDot());
    this->SetSign(mensurAttr->GetMensurSign());
    this->SetSlash(mensurAttr->GetMensurSlash());
    // It is unclear why we don't have mensur.num and mensur.numbase attributes
    // in att.mensura.default.log - ask Perry...
}

Mensur::~Mensur()
{
}
    
void Mensur::Reset()
{
    LayerElement::Reset();
    ResetDurationRatio();
    ResetMensurLog();
    ResetMensurVis();
    ResetSlashcount();
}
    
//----------------------------------------------------------------------------
// MensurAttr
//----------------------------------------------------------------------------

MensurAttr::MensurAttr():
    Object(),
    AttMensuralLog()
{
    Reset();
}


MensurAttr::~MensurAttr()
{
}

void MensurAttr::Reset()
{
    Object::Reset();
    ResetMensuralLog();
}
    
} // namespace vrv