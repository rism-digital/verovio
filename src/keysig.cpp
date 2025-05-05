/////////////////////////////////////////////////////////////////////////////
// Name:        keysig.cpp
// Author:      Rodolfo Zitellini
// Created:     10/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "keysig.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <cstdlib>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "editorial.h"
#include "functor.h"
#include "keyaccid.h"
#include "scoredefinterface.h"
#include "smufl.h"
#include "staff.h"
#include "staffdef.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Static members with some default values
//----------------------------------------------------------------------------

const data_PITCHNAME KeySig::s_pnameForFlats[]
    = { PITCHNAME_b, PITCHNAME_e, PITCHNAME_a, PITCHNAME_d, PITCHNAME_g, PITCHNAME_c, PITCHNAME_f };
const data_PITCHNAME KeySig::s_pnameForSharps[]
    = { PITCHNAME_f, PITCHNAME_c, PITCHNAME_g, PITCHNAME_d, PITCHNAME_a, PITCHNAME_e, PITCHNAME_b };

const int KeySig::octave_map[2][9][7] = {
    {
        // flats
        // C,  D,  E,  F,  G,  A,  B
        { 01, 01, 01, 00, 00, 00, 00 }, // french g = G-1
        { 01, 01, 01, 00, 00, 00, 00 }, // treble = G-2
        { 00, 00, 00, 00, 00, 00, 00 }, // soprano = C-1 (G-3)
        { 00, 00, 00, 00, 00, -1, -1 }, // mezzo = C-2
        { 00, 00, 00, -1, -1, -1, -1 }, // alto = C-3
        { 00, 00, 00, -1, -1, -1, -1 }, // tenor = C-4
        { -1, -1, -1, -1, -1, -1, -1 }, // bariton = F-3 (C-5)
        { -1, -1, -1, -2, -2, -2, -2 }, // bass = F-4
        { -1, -1, -1, -1, -1, -2, -2 }, // sub-bass = F-5
    },
    {
        // sharps
        // C,  D,  E,  F,  G,  A,  B
        { 01, 01, 01, 01, 01, 00, 00 }, // french g
        { 01, 01, 01, 01, 01, 00, 00 }, // treble
        { 00, 00, 00, 00, 00, 00, 00 }, // soprano
        { 00, 00, 00, 00, 00, 00, 00 }, // mezzo
        { 00, 00, 00, 00, 00, -1, -1 }, // alto
        { 00, 00, 00, -1, -1, -1, -1 }, // tenor
        { -1, -1, -1, -1, -1, -1, -1 }, // bariton
        { -1, -1, -1, -1, -1, -2, -2 }, // bass
        { -1, -1, -1, -1, -1, -2, -2 }, // sub-bass
    },
};

//----------------------------------------------------------------------------
// KeySig
//----------------------------------------------------------------------------

static const ClassRegistrar<KeySig> s_factory("keySig", KEYSIG);

KeySig::KeySig()
    : LayerElement(KEYSIG)
    , ObjectListInterface()
    , AttAccidental()
    , AttColor()
    , AttKeyMode()
    , AttKeySigLog()
    , AttKeySigVis()
    , AttPitch()
    , AttVisibility()
{
    this->RegisterAttClass(ATT_ACCIDENTAL);
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_KEYMODE);
    this->RegisterAttClass(ATT_KEYSIGLOG);
    this->RegisterAttClass(ATT_KEYSIGVIS);
    this->RegisterAttClass(ATT_PITCH);
    this->RegisterAttClass(ATT_VISIBILITY);

    this->Reset();
}

KeySig::~KeySig() {}

void KeySig::Reset()
{
    LayerElement::Reset();
    this->ResetAccidental();
    this->ResetColor();
    this->ResetKeyMode();
    this->ResetKeySigLog();
    this->ResetKeySigVis();
    this->ResetPitch();
    this->ResetVisibility();

    // key change drawing values
    m_skipCancellation = false;
    m_drawingCancelAccidType = ACCIDENTAL_WRITTEN_n;
    m_drawingCancelAccidCount = 0;

    this->ResetDrawingClef();
}

void KeySig::FilterList(ListOfConstObjects &childList) const
{
    ListOfConstObjects::iterator iter = childList.begin();
    while (iter != childList.end()) {
        if ((*iter)->Is(KEYACCID)) {
            ++iter;
        }
        else {
            iter = childList.erase(iter);
        }
    }
}

bool KeySig::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ KEYACCID };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
}

bool KeySig::AddChildAdditionalCheck(Object *child)
{
    if (this->IsAttribute() && !child->IsAttribute()) {
        LogError("Adding a non-attribute child to an attribute is not allowed");
        return false;
    }
    return (LayerElement::AddChildAdditionalCheck(child));
}

int KeySig::GetAccidCount(bool fromAttribute) const
{
    if (fromAttribute) {
        return this->HasSig() ? (this->GetSig().first) : 0;
    }
    else {
        return this->GetListSize();
    }
}

data_ACCIDENTAL_WRITTEN KeySig::GetAccidType() const
{
    if (this->HasNonAttribKeyAccidChildren() || !this->HasSig()) {
        return ACCIDENTAL_WRITTEN_NONE;
    }
    else {
        return (this->GetSig().second);
    }
}

bool KeySig::HasNonAttribKeyAccidChildren() const
{
    const ListOfConstObjects &childList = this->GetList();
    return std::any_of(childList.begin(), childList.end(), [](const Object *child) { return !child->IsAttribute(); });
}

void KeySig::GenerateKeyAccidAttribChildren()
{
    IsAttributeComparison isAttribute(KEYACCID);
    this->DeleteChildrenByComparison(&isAttribute);

    if (this->HasEmptyList()) {
        for (int i = 0; i < this->GetAccidCount(true); ++i) {
            std::optional<KeyAccidInfo> info = this->GetKeyAccidInfoAt(i);
            if (info) {
                KeyAccid *keyAccid = new KeyAccid();
                keyAccid->SetAccid(info->accid);
                keyAccid->SetPname(info->pname);
                keyAccid->IsAttribute(true);
                this->AddChild(keyAccid);
            }
        }
    }
    else if (this->HasSig()) {
        LogWarning(
            "Attribute key signature is ignored, since KeySig '%s' contains KeyAccid children.", this->GetID().c_str());
    }
}

void KeySig::FillMap(MapOfOctavedPitchAccid &mapOfPitchAccid) const
{
    mapOfPitchAccid.clear();

    const ListOfConstObjects &childList = this->GetList(); // make sure it's initialized
    if (!childList.empty()) {
        for (const Object *child : childList) {
            const KeyAccid *keyAccid = vrv_cast<const KeyAccid *>(child);
            assert(keyAccid);
            for (int oct = 0; oct < 10; ++oct) {
                mapOfPitchAccid[(keyAccid->GetPname() + oct * 7)] = keyAccid->GetAccid();
            }
        }
        return;
    }

    data_ACCIDENTAL_WRITTEN accidType = this->GetAccidType();
    for (int i = 0; i < this->GetAccidCount(true); ++i) {
        for (int oct = 0; oct < 10; ++oct) {
            mapOfPitchAccid[(KeySig::GetAccidPnameAt(accidType, i) + oct * 7)] = accidType;
        }
    }
}

std::optional<KeyAccidInfo> KeySig::GetKeyAccidInfoAt(int pos) const
{
    if ((pos < 0) || (pos > 12)) return std::nullopt;

    KeyAccidInfo info;
    if (this->GetAccidType() == ACCIDENTAL_WRITTEN_f) {
        info.accid = (pos < 7) ? ACCIDENTAL_WRITTEN_f : ACCIDENTAL_WRITTEN_ff;
        info.pname = s_pnameForFlats[pos % 7];
    }
    else if (this->GetAccidType() == ACCIDENTAL_WRITTEN_s) {
        info.accid = (pos < 7) ? ACCIDENTAL_WRITTEN_s : ACCIDENTAL_WRITTEN_ss;
        info.pname = s_pnameForSharps[pos % 7];
    }
    else {
        return std::nullopt;
    }
    return info;
}

int KeySig::GetFifthsInt() const
{
    if (this->GetSig().second == ACCIDENTAL_WRITTEN_f) {
        return -1 * this->GetSig().first;
    }
    else if (this->GetSig().second == ACCIDENTAL_WRITTEN_s) {
        return this->GetSig().first;
    }
    return 0;
}

Clef *KeySig::GetDrawingClef()
{
    return m_drawingClef.has_value() ? &m_drawingClef.value() : NULL;
}

void KeySig::ResetDrawingClef()
{
    m_drawingClef.reset();
}

void KeySig::SetDrawingClef(Clef *clef)
{
    if (clef) {
        m_drawingClef = *clef;
        m_drawingClef->CloneReset();
    }
    else {
        m_drawingClef.reset();
    }
}

data_KEYSIGNATURE KeySig::ConvertToSig() const
{
    data_KEYSIGNATURE sig = std::make_pair(-1, ACCIDENTAL_WRITTEN_NONE);
    const ListOfConstObjects &childList = this->GetList();
    if (childList.size() > 1) {
        data_ACCIDENTAL_WRITTEN accidType = ACCIDENTAL_WRITTEN_NONE;
        bool isCommon = true;
        int pos = 0;
        for (const Object *child : childList) {
            const KeyAccid *keyAccid = vrv_cast<const KeyAccid *>(child);
            assert(keyAccid);
            data_ACCIDENTAL_WRITTEN curType = keyAccid->GetAccid();
            if (curType == ACCIDENTAL_WRITTEN_n) {
                // Skip naturals encoded explicitly
                continue;
            }
            // We have not a key sig type at this stage
            if (accidType == ACCIDENTAL_WRITTEN_NONE) {
                if (curType == ACCIDENTAL_WRITTEN_s || curType == ACCIDENTAL_WRITTEN_f) {
                    accidType = curType;
                }
            }
            if (accidType != curType) {
                LogWarning("All the keySig content cannot be converted to @sig because the accidental type is not a "
                           "flat or a sharp, or mixes them");
                break;
            }
            if (accidType == ACCIDENTAL_WRITTEN_f && s_pnameForFlats[pos] != keyAccid->GetPname()) {
                isCommon = false;
                break;
            }
            else if (accidType == ACCIDENTAL_WRITTEN_s && s_pnameForSharps[pos] != keyAccid->GetPname()) {
                isCommon = false;
                break;
            }
            pos++;
        }
        if (!isCommon) {
            LogWarning("KeySig content cannot be converted to @sig because the accidental series is not standard");
            return sig;
        }
        sig = std::make_pair(pos, accidType);
    }
    return sig;
}

//----------------------------------------------------------------------------
// Static methods for KeySig
//----------------------------------------------------------------------------

data_PITCHNAME KeySig::GetAccidPnameAt(data_ACCIDENTAL_WRITTEN accidType, int pos)
{
    if (accidType == ACCIDENTAL_WRITTEN_f) {
        return s_pnameForFlats[pos % 7];
    }
    else {
        return s_pnameForSharps[pos % 7];
    }
}

int KeySig::GetOctave(data_ACCIDENTAL_WRITTEN accidType, data_PITCHNAME pitch, const Clef *clef)
{
    int accidSet = 0; // flats
    int keySet = 0;

    if (accidType == ACCIDENTAL_WRITTEN_s) accidSet = 1;

    int shapeLine = 0;
    shapeLine = clef->GetShape() << 8 | clef->GetLine();

    switch (shapeLine) {
        case (CLEFSHAPE_G << 8 | 1): keySet = 0; break;
        case (CLEFSHAPE_G << 8 | 2): keySet = 1; break;
        case (CLEFSHAPE_G << 8 | 3): keySet = 2; break;
        case (CLEFSHAPE_G << 8 | 4): keySet = 3; break;
        case (CLEFSHAPE_G << 8 | 5): keySet = 4; break;

        case (CLEFSHAPE_GG << 8 | 1): keySet = 0; break;
        case (CLEFSHAPE_GG << 8 | 2): keySet = 1; break;
        case (CLEFSHAPE_GG << 8 | 3): keySet = 2; break;
        case (CLEFSHAPE_GG << 8 | 4): keySet = 3; break;
        case (CLEFSHAPE_GG << 8 | 5): keySet = 4; break;

        case (CLEFSHAPE_C << 8 | 1): keySet = 2; break;
        case (CLEFSHAPE_C << 8 | 2): keySet = 3; break;
        case (CLEFSHAPE_C << 8 | 3): keySet = 4; break;
        case (CLEFSHAPE_C << 8 | 4): keySet = 5; break;
        case (CLEFSHAPE_C << 8 | 5): keySet = 6; break;

        case (CLEFSHAPE_F << 8 | 3): keySet = 6; break;
        case (CLEFSHAPE_F << 8 | 4): keySet = 7; break;
        case (CLEFSHAPE_F << 8 | 5): keySet = 8; break;

        // does not really exist but just to make it somehow aligned with the clef
        case (CLEFSHAPE_F << 8 | 1): keySet = 8; break;
        case (CLEFSHAPE_F << 8 | 2): keySet = 8; break;

        default: keySet = 4; break;
    }

    int octave = octave_map[accidSet][keySet][pitch - 1] + OCTAVE_OFFSET;

    int disPlace = 0;
    if (clef->GetDis() != OCTAVE_DIS_NONE) {
        // DIS 22 not supported
        if (clef->GetDisPlace() == STAFFREL_basic_above)
            disPlace = (clef->GetDis() == OCTAVE_DIS_8) ? -1 : -2;
        else if (clef->GetDisPlace() == STAFFREL_basic_below)
            disPlace = (clef->GetDis() == OCTAVE_DIS_8) ? 1 : 2;
    }
    if (clef->GetShape() == CLEFSHAPE_GG) disPlace = 1;

    octave -= disPlace;

    return octave;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode KeySig::Accept(Functor &functor)
{
    return functor.VisitKeySig(this);
}

FunctorCode KeySig::Accept(ConstFunctor &functor) const
{
    return functor.VisitKeySig(this);
}

FunctorCode KeySig::AcceptEnd(Functor &functor)
{
    return functor.VisitKeySigEnd(this);
}

FunctorCode KeySig::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitKeySigEnd(this);
}

} // namespace vrv
