/////////////////////////////////////////////////////////////////////////////
// Name:        keyaccid.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "keyaccid.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "accid.h"
#include "doc.h"
#include "functor.h"
#include "keysig.h"
#include "note.h"
#include "smufl.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// KeyAccid
//----------------------------------------------------------------------------

static const ClassRegistrar<KeyAccid> s_factory("keyAccid", KEYACCID);

KeyAccid::KeyAccid()
    : LayerElement(KEYACCID)
    , PitchInterface()
    , PositionInterface()
    , AttAccidental()
    , AttColor()
    , AttEnclosingChars()
    , AttExtSymAuth()
    , AttExtSymNames()
{

    this->RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_ACCIDENTAL);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_ENCLOSINGCHARS);
    this->RegisterAttClass(ATT_EXTSYMAUTH);
    this->RegisterAttClass(ATT_EXTSYMNAMES);

    this->Reset();
}

KeyAccid::~KeyAccid() {}

void KeyAccid::Reset()
{
    LayerElement::Reset();
    PitchInterface::Reset();
    PositionInterface::Reset();
    this->ResetAccidental();
    this->ResetColor();
    this->ResetEnclosingChars();
    this->ResetExtSymAuth();
    this->ResetExtSymNames();
}

std::u32string KeyAccid::GetSymbolStr(data_NOTATIONTYPE notationType) const
{
    return Accid::CreateSymbolStr(this->GetAccid(), this->GetEnclose(), notationType, this->GetDocResources(),
        this->GetGlyphNum(), this->GetGlyphName());
}

int KeyAccid::CalcStaffLoc(Clef *clef, int clefLocOffset) const
{
    if (this->HasLoc()) {
        return this->GetLoc();
    }
    else {
        const data_ACCIDENTAL_WRITTEN accid = this->GetAccid();
        const data_PITCHNAME pname = this->GetPname();
        return PitchInterface::CalcLoc(pname, KeySig::GetOctave(accid, pname, clef), clefLocOffset);
    }
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode KeyAccid::Accept(Functor &functor)
{
    return functor.VisitKeyAccid(this);
}

FunctorCode KeyAccid::Accept(ConstFunctor &functor) const
{
    return functor.VisitKeyAccid(this);
}

FunctorCode KeyAccid::AcceptEnd(Functor &functor)
{
    return functor.VisitKeyAccidEnd(this);
}

FunctorCode KeyAccid::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitKeyAccidEnd(this);
}

} // namespace vrv
