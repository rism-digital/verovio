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
#include "scoredefinterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Static members with some default values
//----------------------------------------------------------------------------

data_PITCHNAME KeySig::flats[]
    = { PITCHNAME_b, PITCHNAME_e, PITCHNAME_a, PITCHNAME_d, PITCHNAME_g, PITCHNAME_c, PITCHNAME_f };
data_PITCHNAME KeySig::sharps[]
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

KeySig::KeySig() : LayerElement("ksig-"), AttAccidental(), AttPitch()
{
    Init();
}

KeySig::KeySig(int alterationNumber, data_ACCIDENTAL_EXPLICIT alterationType)
    : LayerElement("ksig-"), AttAccidental(), AttPitch()
{
    Init();

    m_alterationNumber = alterationNumber;
    m_alterationType = alterationType;
}

KeySig::KeySig(const ScoreDefInterface *keySigAttr) : LayerElement("ksig-"), AttAccidental(), AttPitch()
{
    Init();

    char key = keySigAttr->GetKeySig() - KEYSIGNATURE_0;
    /* see data_KEYSIGNATURE order; key will be:
      0 for KEYSIGNATURE_0
     -1 for KEYSIGNATURE_1f
      1 for KEYSIGNATURE_1s
     -2 for KEYSIGNATURE_2f
      2 for KEYSIGNATURE_2s
     etc.
     */
    if ((key > (KEYSIGNATURE_7s - KEYSIGNATURE_0)) || (key < (KEYSIGNATURE_7f - KEYSIGNATURE_0))) {
        // other values are  KEYSIGNATURE_NONE or  KEYSIGNATURE_mixed (unsupported)
        return;
    }
    if (key > 0) {
        m_alterationType = ACCIDENTAL_EXPLICIT_s;
    }
    else if (key < 0) {
        m_alterationType = ACCIDENTAL_EXPLICIT_f;
    }
    m_alterationNumber = abs(key);

    if (keySigAttr->GetKeySigShow() == BOOLEAN_false) {
        m_drawingShow = false;
    }
    if (keySigAttr->GetKeySigShowchange() == BOOLEAN_true) {
        m_drawingShowchange = true;
    }
}

void KeySig::Init()
{
    RegisterAttClass(ATT_ACCIDENTAL);
    RegisterAttClass(ATT_PITCH);

    Reset();
}

KeySig::~KeySig()
{
}

void KeySig::Reset()
{
    LayerElement::Reset();
    ResetAccidental();
    ResetPitch();

    m_alterationNumber = 0;
    m_alterationType = ACCIDENTAL_EXPLICIT_NONE;

    // key change drawing values
    m_drawingCancelAccidType = ACCIDENTAL_EXPLICIT_n;
    m_drawingCancelAccidCount = 0;
    m_drawingShow = true;
    m_drawingShowchange = false;
}

void KeySig::ConvertToInternal()
{
    int i;
    if (this->GetAccid() == ACCIDENTAL_EXPLICIT_s) {
        m_alterationType = ACCIDENTAL_EXPLICIT_s;
        for (i = 0; i < 7; i++) {
            if (KeySig::sharps[i] == this->GetPname()) {
                m_alterationNumber = i + 1;
                break;
            }
        }
    }
    else if (this->GetAccid() == ACCIDENTAL_EXPLICIT_f) {
        m_alterationType = ACCIDENTAL_EXPLICIT_f;
        for (i = 0; i < 7; i++) {
            if (KeySig::flats[i] == this->GetPname()) {
                m_alterationNumber = i + 1;
                break;
            }
        }
    }
    else
        return;
}

void KeySig::ConvertToMei()
{
    if ((m_alterationNumber < 1) || (m_alterationNumber > 7)) return;

    if (m_alterationType == ACCIDENTAL_EXPLICIT_s) {
        this->SetAccid(ACCIDENTAL_EXPLICIT_s);
        this->SetPname(KeySig::sharps[m_alterationNumber - 1]);
    }
    else if (m_alterationType == ACCIDENTAL_EXPLICIT_f) {
        this->SetAccid(ACCIDENTAL_EXPLICIT_f);
        this->SetPname(KeySig::flats[m_alterationNumber - 1]);
    }
    else
        return;
}

data_KEYSIGNATURE KeySig::ConvertToKeySigLog()
{
    char key = m_alterationNumber;
    if (m_alterationType == ACCIDENTAL_EXPLICIT_f) key = -key;
    return (data_KEYSIGNATURE)(key + KEYSIGNATURE_0);
}

//----------------------------------------------------------------------------
// Static methods
//----------------------------------------------------------------------------

data_PITCHNAME KeySig::GetAlterationAt(data_ACCIDENTAL_EXPLICIT alterationType, int pos)
{
    data_PITCHNAME *alteration_set;

    if (pos > 6) return PITCHNAME_c;

    if (alterationType == ACCIDENTAL_EXPLICIT_f)
        alteration_set = flats;
    else
        alteration_set = sharps;

    return alteration_set[pos];
}

int KeySig::GetOctave(data_ACCIDENTAL_EXPLICIT alterationType, data_PITCHNAME pitch, Clef *clef)
{
    int alter_set = 0; // flats
    int key_set = 0;

    if (alterationType == ACCIDENTAL_EXPLICIT_s) alter_set = 1;

    int shapeLine = 0;
    shapeLine = clef->GetShape() << 8 | clef->GetLine();

    switch (shapeLine) {
        case (CLEFSHAPE_G << 8 | 1): key_set = 0; break;
        case (CLEFSHAPE_G << 8 | 2): key_set = 1; break;
        case (CLEFSHAPE_G << 8 | 3): key_set = 2; break;
        case (CLEFSHAPE_G << 8 | 4): key_set = 3; break;
        case (CLEFSHAPE_G << 8 | 5): key_set = 4; break;

        case (CLEFSHAPE_C << 8 | 1): key_set = 2; break;
        case (CLEFSHAPE_C << 8 | 2): key_set = 3; break;
        case (CLEFSHAPE_C << 8 | 3): key_set = 4; break;
        case (CLEFSHAPE_C << 8 | 4): key_set = 5; break;
        case (CLEFSHAPE_C << 8 | 5): key_set = 6; break;

        case (CLEFSHAPE_F << 8 | 3): key_set = 6; break;
        case (CLEFSHAPE_F << 8 | 4): key_set = 7; break;
        case (CLEFSHAPE_F << 8 | 5):
            key_set = 8;
            break;

        // does not really exist but just to make it somehow aligned with the clef
        case (CLEFSHAPE_F << 8 | 1): key_set = 8; break;
        case (CLEFSHAPE_F << 8 | 2): key_set = 8; break;

        default: key_set = 4; break;
    }

    int octave = octave_map[alter_set][key_set][pitch - 1] + OCTAVE_OFFSET;

    int disPlace = 0;
    if (clef->GetDis() != OCTAVE_DIS_NONE) {
        // DIS 22 not supported
        if (clef->GetDisPlace() == PLACE_above)
            disPlace = (clef->GetDis() == OCTAVE_DIS_8) ? -1 : -2;
        else if (clef->GetDisPlace() == PLACE_below)
            disPlace = (clef->GetDis() == OCTAVE_DIS_8) ? 1 : 2;
    }

    octave -= disPlace;

    return octave;
}

} // namespace vrv
