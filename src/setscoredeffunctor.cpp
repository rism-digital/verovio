/////////////////////////////////////////////////////////////////////////////
// Name:        setscoredeffunctor.cpp
// Author:      David Bauer
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "setscoredeffunctor.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// ReplaceDrawingValuesInStaffDefFunctor
//----------------------------------------------------------------------------

ReplaceDrawingValuesInStaffDefFunctor::ReplaceDrawingValuesInStaffDefFunctor(const Clef *clef, const KeySig *keySig,
    const Mensur *mensur, const MeterSig *meterSig, const MeterSigGrp *meterSigGrp)
{
    m_clef = clef;
    m_keySig = keySig;
    m_mensur = mensur;
    m_meterSig = meterSig;
    m_meterSigGrp = meterSigGrp;
}

FunctorCode ReplaceDrawingValuesInStaffDefFunctor::VisitStaffDef(StaffDef *staffDef)
{
    if (m_clef) {
        staffDef->SetCurrentClef(m_clef);
    }
    if (m_keySig) {
        staffDef->SetCurrentKeySig(m_keySig);
    }
    if (m_mensur) {
        staffDef->SetCurrentMensur(m_mensur);
    }
    if (m_meterSig) {
        staffDef->SetCurrentMeterSig(m_meterSig);
    }
    if (m_meterSigGrp) {
        staffDef->SetCurrentMeterSigGrp(m_meterSigGrp);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
