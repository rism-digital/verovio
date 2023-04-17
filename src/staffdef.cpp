/////////////////////////////////////////////////////////////////////////////
// Name:        staffdef.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "staffdef.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "functorparams.h"
#include "instrdef.h"
#include "label.h"
#include "labelabbr.h"
#include "layerdef.h"
#include "metersiggrp.h"
#include "staffgrp.h"
#include "transposition.h"
#include "tuning.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// StaffDef
//----------------------------------------------------------------------------

static const ClassRegistrar<StaffDef> s_factory("staffDef", STAFFDEF);

StaffDef::StaffDef()
    : ScoreDefElement(STAFFDEF, "staffdef-")
    , AttDistances()
    , AttLabelled()
    , AttNInteger()
    , AttNotationType()
    , AttScalable()
    , AttStaffDefLog()
    , AttStaffDefVis()
    , AttTimeBase()
    , AttTransposition()
{
    this->RegisterAttClass(ATT_DISTANCES);
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_NINTEGER);
    this->RegisterAttClass(ATT_NOTATIONTYPE);
    this->RegisterAttClass(ATT_SCALABLE);
    this->RegisterAttClass(ATT_STAFFDEFLOG);
    this->RegisterAttClass(ATT_STAFFDEFVIS);
    this->RegisterAttClass(ATT_TIMEBASE);
    this->RegisterAttClass(ATT_TRANSPOSITION);

    this->Reset();
}

StaffDef::~StaffDef() {}

void StaffDef::Reset()
{
    ScoreDefElement::Reset();
    StaffDefDrawingInterface::Reset();
    this->ResetDistances();
    this->ResetLabelled();
    this->ResetNInteger();
    this->ResetNotationType();
    this->ResetScalable();
    this->ResetStaffDefLog();
    this->ResetStaffDefVis();
    this->ResetTimeBase();
    this->ResetTransposition();

    m_drawingVisibility = OPTIMIZATION_NONE;
}

bool StaffDef::IsSupportedChild(Object *child)
{
    if (child->Is(CLEF)) {
        assert(dynamic_cast<Clef *>(child));
    }
    else if (child->Is(INSTRDEF)) {
        assert(dynamic_cast<InstrDef *>(child));
    }
    else if (child->Is(KEYSIG)) {
        assert(dynamic_cast<KeySig *>(child));
    }
    else if (child->Is(LABEL)) {
        assert(dynamic_cast<Label *>(child));
    }
    else if (child->Is(LABELABBR)) {
        assert(dynamic_cast<LabelAbbr *>(child));
    }
    else if (child->Is(LAYERDEF)) {
        assert(dynamic_cast<LayerDef *>(child));
    }
    else if (child->Is(MENSUR)) {
        assert(dynamic_cast<Mensur *>(child));
    }
    else if (child->Is(METERSIG)) {
        assert(dynamic_cast<MeterSig *>(child));
    }
    else if (child->Is(METERSIGGRP)) {
        assert(dynamic_cast<MeterSigGrp *>(child));
    }
    else if (child->Is(TUNING)) {
        assert(dynamic_cast<Tuning *>(child));
    }
    else {
        return false;
    }
    return true;
}

int StaffDef::GetInsertOrderFor(ClassId classId) const
{
    // Anything else goes at the end
    static const std::vector s_order({ LABEL, LABELABBR });
    return this->GetInsertOrderForIn(classId, s_order);
}

bool StaffDef::HasLayerDefWithLabel() const
{
    // First get all the staffGrps
    ListOfConstObjects layerDefs = this->FindAllDescendantsByType(LAYERDEF);

    // Then the @n of each first staffDef
    for (const Object *object : layerDefs) {
        if (object->FindDescendantByType(LABEL)) return true;
    }
    return false;
}

//----------------------------------------------------------------------------
// StaffDef functor methods
//----------------------------------------------------------------------------

FunctorCode StaffDef::Accept(MutableFunctor &functor)
{
    return functor.VisitStaffDef(this);
}

FunctorCode StaffDef::Accept(ConstFunctor &functor) const
{
    return functor.VisitStaffDef(this);
}

FunctorCode StaffDef::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitStaffDefEnd(this);
}

FunctorCode StaffDef::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitStaffDefEnd(this);
}

int StaffDef::GenerateMIDI(FunctorParams *functorParams)
{
    GenerateMIDIParams *params = vrv_params_cast<GenerateMIDIParams *>(functorParams);
    assert(params);

    if (this->GetN() == params->m_staffN) {
        // Update the semitone transposition
        if (this->HasTransSemi()) params->m_transSemi = this->GetTransSemi();
    }

    return FUNCTOR_CONTINUE;
}

int StaffDef::Transpose(FunctorParams *functorParams)
{
    TransposeParams *params = vrv_params_cast<TransposeParams *>(functorParams);
    assert(params);

    if (params->m_transposeToSoundingPitch) {
        // Retrieve the key signature
        const KeySig *keySig = vrv_cast<const KeySig *>(this->FindDescendantByType(KEYSIG));
        if (!keySig) {
            const ScoreDef *scoreDef = vrv_cast<const ScoreDef *>(this->GetFirstAncestor(SCOREDEF));
            keySig = vrv_cast<const KeySig *>(scoreDef->FindDescendantByType(KEYSIG));
        }
        // Determine and store the transposition interval (based on keySig)
        if (keySig && this->HasTransSemi() && this->HasN()) {
            const int fifths = keySig->GetFifthsInt();
            int semitones = this->GetTransSemi();
            // Factor out octave transpositions
            const int sign = (semitones >= 0) ? +1 : -1;
            semitones = sign * (std::abs(semitones) % 24);
            params->m_transposer->SetTransposition(fifths, std::to_string(semitones));
            params->m_transposeIntervalForStaffN[this->GetN()] = params->m_transposer->GetTranspositionIntervalClass();
            this->ResetTransposition();
        }
        else {
            int transposeInterval = 0;
            if (this->HasN() && (params->m_transposeIntervalForStaffN.count(this->GetN()) > 0)) {
                transposeInterval = params->m_transposeIntervalForStaffN.at(this->GetN());
            }
            params->m_transposer->SetTransposition(transposeInterval);
        }
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
