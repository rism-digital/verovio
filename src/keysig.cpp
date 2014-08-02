
/////////////////////////////////////////////////////////////////////////////
// Name:        keysig.cpp
// Author:      Rodolfo Zitellini
// Created:     10/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "keysig.h"

namespace vrv {

//----------------------------------------------------------------------------
// KeySig
//----------------------------------------------------------------------------

unsigned char KeySig::flats[] = {PITCH_B, PITCH_E, PITCH_A, PITCH_D, PITCH_G, PITCH_C, PITCH_F};
unsigned char KeySig::sharps[] = {PITCH_F, PITCH_C, PITCH_G, PITCH_D, PITCH_A, PITCH_E, PITCH_B};

int KeySig::octave_map[2][9][7] = {
    {// flats
       //C,  D,  E,  F,  G,  A,  B 
        {01, 01, 01, 00, 00, 00, 00}, // treble
        {00, 00, 00, 00, 00, 00, 00}, // soprano
        {00, 00, 00, 00, 00, -1, -1}, // mezzo
        {00, 00, 00, -1, -1, -1, -1}, // alto
        {00, 00, 00, -1, -1, -1, -1}, // tenor
        {-1, -1, -1, -1, -1, -2, -2}, // ??
        {-1, -1, -1, -2, -2, -2, -2}, // bass
        {-1, -1, -1, -1, -1, -1, -1}, // bariton
        {01, 01, 01, 00, 00, 00, 00}, // french g
    },
    {// sharps
       //C,  D,  E,  F,  G,  A,  B 
        {01, 01, 01, 01, 01, 00, 00}, // treble
        {00, 00, 00, 00, 00, 00, 00}, // soprano
        {00, 00, 00, 00, 00, 00, 00}, // mezzo
        {00, 00, 00, 00, 00, -1, -1}, // alto
        {00, 00, 00, -1, -1, -1, -1}, // tenor
        {-1, -1, -1, -1, -1, -2, -2}, // ??
        {-1, -1, -1, -1, -1, -2, -2}, // bass
        {-1, -1, -1, -1, -1, -1, -1}, // bariton
        {01, 01, 01, 01, 01, 00, 00}, // freench g
    },
};

KeySig::KeySig():
LayerElement()
{
    KeySig(0, ACCID_NATURAL);
}

KeySig::KeySig(int num_alter, char alter):
    LayerElement("ksig-")
{
    m_num_alter = num_alter;
    m_alteration = alter;
}

KeySig::~KeySig()
{
}

unsigned char KeySig::GetAlterationAt(int pos) {
    unsigned char *alteration_set;
    
    if (pos > 6)
        return 0;
    
    if (m_alteration == ACCID_FLAT)
        alteration_set = flats;
    else
        alteration_set = sharps;
    
    return alteration_set[pos];
}

int KeySig::GetOctave(unsigned char pitch, char clef) {
    int alter_set = 0; // flats
    int key_set = 0;
    
    if (m_alteration == ACCID_SHARP)
        alter_set = 1;
    
    switch (clef) {
        case SOL2: key_set = 0; break;
        case UT1: key_set = 1; break;
        case UT2: key_set = 2; break;
        case UT3: key_set = 3; break;
        case UT4: key_set = 4; break;
        case FA5: key_set = 5; break;
        case FA4: key_set = 6; break;
        case FA3: key_set = 7; break;
        case SOL1: key_set = 8; break;
            
        default: key_set = 0; break;
    }
    
    return octave_map[alter_set][key_set][pitch - 1];
}

} // namespace vrv