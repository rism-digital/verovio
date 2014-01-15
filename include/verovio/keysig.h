
/////////////////////////////////////////////////////////////////////////////
// Name:        keysig.h
// Author:      Rodolfo Zitellini
// Created:     10/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_KEYSIG_H__
#define __VRV_KEYSIG_H__

#include "vrvdef.h"
#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// KeySignature
//----------------------------------------------------------------------------

class KeySignature: public LayerElement, public PositionInterface
{
public:
    // constructors and destructors
    KeySignature();
    KeySignature(int num_alter, char alter);
    virtual ~KeySignature();
    
    virtual std::string GetClassName( ) { return "KeySignature"; };
    
    /**
     * Return the default horizontal spacing after a keysig.
     */
    virtual int GetHorizontalSpacing( ) { return 20; };
    
    unsigned char GetAlterationAt(int pos);
    int GetOctave(unsigned char pitch, char clef);
    
private:
    
public:
    int m_num_alter;
    unsigned char m_alteration;
    
private:
    static unsigned char flats[];
    static unsigned char sharps[];
    static int octave_map[2][9][7];
    
};

} // namespace vrv    

#endif
