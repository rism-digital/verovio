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

namespace vrv {

//----------------------------------------------------------------------------
// Static members with some default values
//----------------------------------------------------------------------------
    
data_PITCHNAME KeySig::flats[] = {PITCHNAME_b, PITCHNAME_e, PITCHNAME_a, PITCHNAME_d, PITCHNAME_g, PITCHNAME_c, PITCHNAME_f};
data_PITCHNAME KeySig::sharps[] = {PITCHNAME_f, PITCHNAME_c, PITCHNAME_g, PITCHNAME_d, PITCHNAME_a, PITCHNAME_e, PITCHNAME_b};

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

//----------------------------------------------------------------------------
// KeySig
//----------------------------------------------------------------------------

KeySig::KeySig():
    LayerElement("ksig-"), KeySigDrawingInterface(),
    AttAccidental(),
    AttPitch()
{
    Init();
    Reset();
}

KeySig::KeySig(int alterationNumber, data_ACCIDENTAL_EXPLICIT alterationType):
    LayerElement("ksig-"), KeySigDrawingInterface(),
    AttAccidental(),
    AttPitch()
{
    Init();
    Reset();
    
    m_alterationNumber = alterationNumber;
    m_alterationType = alterationType;
}
    
KeySig::KeySig( KeySigAttr *keySigAttr ):
    LayerElement("ksig-"), KeySigDrawingInterface( *keySigAttr ),
    AttAccidental(),
    AttPitch()
{
    Init();
    // Do not call reset because we key values passed by the keySigAttr arg
    
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
    
    if ( keySigAttr->GetKeySigShow() == BOOLEAN_false ) {
        m_drawingShow = false;
    }
    if ( keySigAttr->GetKeySigShowchange() == BOOLEAN_true ) {
        m_drawingShowchange = true;
    }
}
   
void KeySig::Init()
{
    RegisterAttClass(ATT_ACCIDENTAL);
    RegisterAttClass(ATT_PITCH);
}

KeySig::~KeySig()
{
}
    
void KeySig::Reset()
{
    LayerElement::Reset();
    KeySigDrawingInterface::Reset();
    ResetAccidental();
    ResetPitch();
    m_alterationNumber = 0;
    m_alterationType = ACCIDENTAL_EXPLICIT_n;
}
    
    
void KeySig::ConvertToInternal( )
{
    int i;
    if (this->GetAccid() == ACCIDENTAL_EXPLICIT_s) {
        m_alterationType = ACCIDENTAL_EXPLICIT_s;
        for (i = 0;i < 7; i++) {
            if (KeySig::sharps[i] == this->GetPname()) {
                m_alterationNumber = i + 1;
                break;
            }
        }
        
    }
    else if (this->GetAccid() == ACCIDENTAL_EXPLICIT_f) {
        m_alterationType = ACCIDENTAL_EXPLICIT_f;
        for (i = 0;i < 7; i++) {
            if (KeySig::flats[i] == this->GetPname()) {
                m_alterationNumber = i + 1;
                break;
            }
        }
    }
    else return;
}
    
void KeySig::ConvertToMei()
{
    if ((m_alterationNumber < 1) || (m_alterationNumber > 7)) return;
    
    if (m_alterationType == ACCIDENTAL_EXPLICIT_s) {
        this->SetAccid( ACCIDENTAL_EXPLICIT_s);
        this->SetPname(KeySig::sharps[m_alterationNumber - 1]);
    }
    else if (m_alterationType == ACCIDENTAL_EXPLICIT_f) {
        this->SetAccid( ACCIDENTAL_EXPLICIT_f);
        this->SetPname(KeySig::flats[m_alterationNumber - 1]);
    }
    else return;
}
    
//----------------------------------------------------------------------------
// Static methods
//----------------------------------------------------------------------------


data_PITCHNAME KeySig::GetAlterationAt(data_ACCIDENTAL_EXPLICIT alterationType, int pos)
{
    data_PITCHNAME *alteration_set;
    
    if (pos > 6) return PITCHNAME_c;
    
    if (alterationType == ACCIDENTAL_EXPLICIT_f) alteration_set = flats;
    else alteration_set = sharps;
    
    return alteration_set[pos];
}

int KeySig::GetOctave(data_ACCIDENTAL_EXPLICIT alterationType, data_PITCHNAME pitch, int clefId)
{
    int alter_set = 0; // flats
    int key_set = 0;
    
    if (alterationType == ACCIDENTAL_EXPLICIT_s) alter_set = 1;
    
    switch (clefId) {
        case G2: key_set = 0; break;
        case G2_8va: key_set = 0; break;
        case G2_8vb: key_set = 3; break;
        case C1: key_set = 1; break;
        case C2: key_set = 2; break;
        case C3: key_set = 3; break;
        case C4: key_set = 4; break;
        case C5: key_set = 5; break;
        case F5: key_set = 5; break;
        case F4: key_set = 6; break;
        case F3: key_set = 7; break;
        case G1: key_set = 8; break;
            
        default: key_set = 0; break;
    }
    
    return octave_map[alter_set][key_set][pitch - 1] + OCTAVE_OFFSET;
}

    
//----------------------------------------------------------------------------
// KeySigAttr
//----------------------------------------------------------------------------

KeySigAttr::KeySigAttr():
    Object(), KeySigDrawingInterface(),
    AttKeySigDefaultLog(),
    AttKeySigDefaultVis()
{
    RegisterAttClass(ATT_KEYSIGDEFAULTLOG);
    RegisterAttClass(ATT_KEYSIGDEFAULTVIS);
    Reset();
}

KeySigAttr::~KeySigAttr()
{
}

void KeySigAttr::Reset()
{
    Object::Reset();
    KeySigDrawingInterface::Reset();
    ResetKeySigDefaultLog();
    ResetKeySigDefaultVis();
}

} // namespace vrv