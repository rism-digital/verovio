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

KeySig::KeySig() : LayerElement("keysig-"), AttAccidental(), AttPitch(), AttKeySigAnl(), AttKeySigLog(), AttKeySigVis(), AttVisibility()
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

    // key change drawing values
    m_drawingCancelAccidType = ACCIDENTAL_WRITTEN_n;
    m_drawingCancelAccidCount = 0;
}
    
int KeySig::GetAlterationNumber() const
{
    if (!this->HasSig()) return 0;
    
    return (this->GetSig().first);
}
    
data_ACCIDENTAL_WRITTEN KeySig::GetAlterationType() const
{
    if (!this->HasSig()) return ACCIDENTAL_WRITTEN_NONE;
    
    return (this->GetSig().second);
}

//----------------------------------------------------------------------------
// Static methods
//----------------------------------------------------------------------------

data_PITCHNAME KeySig::GetAlterationAt(data_ACCIDENTAL_WRITTEN alterationType, int pos)
{
    data_PITCHNAME *alteration_set;

    if (pos > 6) return PITCHNAME_c;

    if (alterationType == ACCIDENTAL_WRITTEN_f)
        alteration_set = flats;
    else
        alteration_set = sharps;

    return alteration_set[pos];
}

int KeySig::GetOctave(data_ACCIDENTAL_WRITTEN alterationType, data_PITCHNAME pitch, Clef *clef)
{
    int alter_set = 0; // flats
    int key_set = 0;

    if (alterationType == ACCIDENTAL_WRITTEN_s) alter_set = 1;

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
        case (CLEFSHAPE_F << 8 | 5): key_set = 8; break;

        // does not really exist but just to make it somehow aligned with the clef
        case (CLEFSHAPE_F << 8 | 1): key_set = 8; break;
        case (CLEFSHAPE_F << 8 | 2): key_set = 8; break;

        default: key_set = 4; break;
    }

    int octave = octave_map[alter_set][key_set][pitch - 1] + OCTAVE_OFFSET;

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

} // namespace vrv
