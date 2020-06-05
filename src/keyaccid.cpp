/////////////////////////////////////////////////////////////////////////////
// Name:        keyaccid.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "keyaccid.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "doc.h"
#include "editorial.h"
#include "functorparams.h"
#include "keysig.h"
#include "note.h"
#include "smufl.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// KeyAccid
//----------------------------------------------------------------------------

KeyAccid::KeyAccid() : LayerElement("keyaccid-"), PitchInterface(), AttAccidental(), AttColor(), AttEnclosingChars()
{

    RegisterInterface(PitchInterface::GetAttClasses(), PitchInterface::IsInterface());
    RegisterAttClass(ATT_ACCIDENTAL);
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_ENCLOSINGCHARS);

    Reset();
}

KeyAccid::~KeyAccid() {}

void KeyAccid::Reset()
{
    LayerElement::Reset();
    PitchInterface::Reset();
    ResetAccidental();
    ResetColor();
    ResetEnclosingChars();
}

bool KeySig::IsSupportedChild(Object *child)
{
    if (this->IsAttribute()) {
        LogError("Adding a child to an attribute is not allowed");
        assert(false);
    }
    else if (child->Is(KEYACCID)) {
        assert(dynamic_cast<KeyAccid *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

std::wstring KeyAccid::GetSymbolStr() const
{
    if (!this->HasAccid()) return L"";

    wchar_t symc = Accid::GetAccidGlyph(this->GetAccid());
    std::wstring symbolStr;

    if (this->HasEnclose()) {
        if (this->GetEnclose() == ENCLOSURE_brack) {
            symbolStr.push_back(SMUFL_E26C_accidentalBracketLeft);
            symbolStr.push_back(symc);
            symbolStr.push_back(SMUFL_E26D_accidentalBracketRight);
        }
        else {
            symbolStr.push_back(SMUFL_E26A_accidentalParensLeft);
            symbolStr.push_back(symc);
            symbolStr.push_back(SMUFL_E26B_accidentalParensRight);
        }
    }
    else {
        symbolStr.push_back(symc);
    }
    return symbolStr;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

} // namespace vrv
