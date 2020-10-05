/////////////////////////////////////////////////////////////////////////////
// Name:        keysig.cpp
// Author:      Rodolfo Zitellini
// Created:     10/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "keysig.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <stdlib.h>

//----------------------------------------------------------------------------

#include "clef.h"
#include "functorparams.h"
#include "keyaccid.h"
#include "scoredefinterface.h"
#include "smufl.h"
#include "transposition.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Static members with some default values
//----------------------------------------------------------------------------

data_PITCHNAME KeySig::s_pnameForFlats[]
    = { PITCHNAME_b, PITCHNAME_e, PITCHNAME_a, PITCHNAME_d, PITCHNAME_g, PITCHNAME_c, PITCHNAME_f };
data_PITCHNAME KeySig::s_pnameForSharps[]
    = { PITCHNAME_f, PITCHNAME_c, PITCHNAME_g, PITCHNAME_d, PITCHNAME_a, PITCHNAME_e, PITCHNAME_b };

int KeySig::octave_map[2][9][7] = {
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
        { 01, 01, 01, 01, 01, 00, 00 }, // freench g
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

KeySig::KeySig()
    : LayerElement("keysig-")
    , ObjectListInterface()
    , AttAccidental()
    , AttPitch()
    , AttKeySigAnl()
    , AttKeySigLog()
    , AttKeySigVis()
    , AttVisibility()
{
    RegisterAttClass(ATT_ACCIDENTAL);
    RegisterAttClass(ATT_PITCH);
    RegisterAttClass(ATT_KEYSIGANL);
    RegisterAttClass(ATT_KEYSIGLOG);
    RegisterAttClass(ATT_KEYSIGVIS);
    RegisterAttClass(ATT_VISIBILITY);

    Reset();
}

KeySig::~KeySig() {}

void KeySig::Reset()
{
    LayerElement::Reset();
    ResetAccidental();
    ResetPitch();
    ResetKeySigAnl();
    ResetKeySigLog();
    ResetKeySigVis();
    ResetVisibility();

    m_mixedChildrenAccidType = false;

    // key change drawing values
    m_drawingCancelAccidType = ACCIDENTAL_WRITTEN_n;
    m_drawingCancelAccidCount = 0;
}

void KeySig::FilterList(ArrayOfObjects *childList)
{
    // nothing  to filter since we allow only KeyAccid for now.
    ArrayOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if ((*iter)->Is(KEYACCID))
            ++iter;
        else
            iter = childList->erase(iter);
    }

    data_ACCIDENTAL_WRITTEN type = ACCIDENTAL_WRITTEN_NONE;
    m_mixedChildrenAccidType = false;

    for (auto &child : *childList) {
        KeyAccid *keyAccid = vrv_cast<KeyAccid *>(child);
        assert(keyAccid);
        if (type == ACCIDENTAL_WRITTEN_NONE) {
            type = keyAccid->GetAccid();
            continue;
        }
        if (type != keyAccid->GetAccid()) {
            m_mixedChildrenAccidType = true;
            break;
        }
    }
}

int KeySig::GetAccidCount()
{
    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    if (childList->size() > 0) {
        return (int)childList->size();
    }

    if (!this->HasSig()) return 0;

    return (this->GetSig().first);
}

data_ACCIDENTAL_WRITTEN KeySig::GetAccidType()
{
    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    if (childList->size() > 0) {
        if (m_mixedChildrenAccidType) return ACCIDENTAL_WRITTEN_NONE;
        KeyAccid *keyAccid = vrv_cast<KeyAccid *>(childList->at(0));
        assert(keyAccid);
        return keyAccid->GetAccid();
    }

    if (!this->HasSig()) return ACCIDENTAL_WRITTEN_NONE;

    return (this->GetSig().second);
}

void KeySig::FillMap(MapOfPitchAccid &mapOfPitchAccid)
{
    mapOfPitchAccid.clear();

    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    if (childList->size() > 0) {
        for (auto &child : *childList) {
            KeyAccid *keyAccid = vrv_cast<KeyAccid *>(child);
            assert(keyAccid);
            mapOfPitchAccid[keyAccid->GetPname()] = keyAccid->GetAccid();
        }
        return;
    }

    int i;
    data_ACCIDENTAL_WRITTEN accidType = this->GetAccidType();
    for (i = 0; i < this->GetAccidCount(); i++) {
        mapOfPitchAccid[KeySig::GetAccidPnameAt(accidType, i)] = accidType;
    }
}

std::wstring KeySig::GetKeyAccidStrAt(int pos, data_ACCIDENTAL_WRITTEN &accid, data_PITCHNAME &pname)
{
    pname = PITCHNAME_c;
    accid = ACCIDENTAL_WRITTEN_s;
    std::wstring symbolStr = L"";

    const ArrayOfObjects *childList = this->GetList(this); // make sure it's initialized
    if (childList->size() > 0) {
        if ((int)childList->size() <= pos) return symbolStr;
        KeyAccid *keyAccid = vrv_cast<KeyAccid *>(childList->at(pos));
        assert(keyAccid);
        accid = keyAccid->GetAccid();
        pname = keyAccid->GetPname();
        return keyAccid->GetSymbolStr();
    }

    data_PITCHNAME *accidSet;

    if (pos > 6) return symbolStr;

    int symb;
    accid = this->GetAccidType();
    if (accid == ACCIDENTAL_WRITTEN_f) {
        symb = SMUFL_E260_accidentalFlat;
        accidSet = s_pnameForFlats;
    }
    else {
        symb = SMUFL_E262_accidentalSharp;
        accidSet = s_pnameForSharps;
    }

    pname = accidSet[pos];
    symbolStr.push_back(symb);
    return symbolStr;
}

//----------------------------------------------------------------------------
// Static methods
//----------------------------------------------------------------------------

data_PITCHNAME KeySig::GetAccidPnameAt(data_ACCIDENTAL_WRITTEN accidType, int pos)
{
    data_PITCHNAME *accidSet;

    if (pos > 6) return PITCHNAME_c;

    if (accidType == ACCIDENTAL_WRITTEN_f) {
        accidSet = s_pnameForFlats;
    }
    else {
        accidSet = s_pnameForSharps;
    }

    return accidSet[pos];
}

int KeySig::GetOctave(data_ACCIDENTAL_WRITTEN accidType, data_PITCHNAME pitch, Clef *clef)
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

    octave -= disPlace;

    return octave;
}

int KeySig::GetFifthsInt()
{
    if (this->GetSig().second == ACCIDENTAL_WRITTEN_f) {
        return -1 * this->GetSig().first;
    }
    else if (this->GetSig().second == ACCIDENTAL_WRITTEN_s) {
        return this->GetSig().first;
    }
    return 0;
}
//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int KeySig::Transpose(FunctorParams *functorParams)
{
    TransposeParams *params = vrv_params_cast<TransposeParams *>(functorParams);
    assert(params);

    LogDebug("Transposing keySig");
    int sig = this->GetFifthsInt();

    int intervalClass = params->m_transposer->CircleOfFifthsToIntervalClass(sig);
    intervalClass = params->m_transposer->Transpose(intervalClass);
    int fifths = params->m_transposer->IntervalToCircleOfFifths(intervalClass);

    if (fifths == INVALID_INTERVAL_CLASS) {
        this->SetSig(std::make_pair(-1, ACCIDENTAL_WRITTEN_NONE));
    }
    else if (fifths < 0) {
        this->SetSig(std::make_pair(-fifths, ACCIDENTAL_WRITTEN_f));
    }
    else if (fifths > 0) {
        this->SetSig(std::make_pair(fifths, ACCIDENTAL_WRITTEN_s));
    }
    else {
        this->SetSig(std::make_pair(-1, ACCIDENTAL_WRITTEN_NONE));
    }

    // Also convert pname and accid attributes
    if (this->HasPname()) {
        TransPitch pitch = TransPitch(this->GetPname(), ACCIDENTAL_GESTURAL_NONE, this->GetAccid(), 4);
        params->m_transposer->Transpose(pitch);
        this->SetPname(pitch.GetPitchName());
        this->SetAccid(pitch.GetAccidW());
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
