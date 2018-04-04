/////////////////////////////////////////////////////////////////////////////
// Name:        staffdef.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "staffdef.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "instrdef.h"
#include "label.h"
#include "labelabbr.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// StaffDef
//----------------------------------------------------------------------------

StaffDef::StaffDef()
    : ScoreDefElement("staffdef-")
    , AttDistances()
    , AttLabelled()
    , AttNInteger()
    , AttNotationType()
    , AttScalable()
    , AttStaffDefLog()
    , AttTransposition()
{
    RegisterAttClass(ATT_DISTANCES);
    RegisterAttClass(ATT_LABELLED);
    RegisterAttClass(ATT_NINTEGER);
    RegisterAttClass(ATT_NOTATIONTYPE);
    RegisterAttClass(ATT_SCALABLE);
    RegisterAttClass(ATT_STAFFDEFLOG);
    RegisterAttClass(ATT_TRANSPOSITION);

    Reset();
}

StaffDef::~StaffDef() {}

void StaffDef::Reset()
{
    ScoreDefElement::Reset();
    StaffDefDrawingInterface::Reset();
    ResetDistances();
    ResetLabelled();
    ResetNInteger();
    ResetNotationType();
    ResetScalable();
    ResetStaffDefLog();
    ResetTransposition();
}

void StaffDef::AddChild(Object *child)
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
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// StaffDef functor methods
//----------------------------------------------------------------------------

int StaffDef::ReplaceDrawingValuesInStaffDef(FunctorParams *functorParams)
{
    ReplaceDrawingValuesInStaffDefParams *params = dynamic_cast<ReplaceDrawingValuesInStaffDefParams *>(functorParams);
    assert(params);

    if (params->m_clef) {
        this->SetCurrentClef(params->m_clef);
    }
    if (params->m_keySig) {
        this->SetCurrentKeySig(params->m_keySig);
    }
    if (params->m_mensur) {
        this->SetCurrentMensur(params->m_mensur);
    }
    if (params->m_meterSig) {
        this->SetCurrentMeterSig(params->m_meterSig);
    }

    return FUNCTOR_CONTINUE;
}

int StaffDef::SetStaffDefRedrawFlags(FunctorParams *functorParams)
{
    SetStaffDefRedrawFlagsParams *params = dynamic_cast<SetStaffDefRedrawFlagsParams *>(functorParams);
    assert(params);

    if (params->m_clef || params->m_applyToAll) {
        this->SetDrawClef(params->m_clef);
    }
    if (params->m_keySig || params->m_applyToAll) {
        this->SetDrawKeySig(params->m_keySig);
    }
    if (params->m_mensur || params->m_applyToAll) {
        this->SetDrawMensur(params->m_mensur);
    }
    if (params->m_meterSig || params->m_applyToAll) {
        this->SetDrawMeterSig(params->m_meterSig);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
