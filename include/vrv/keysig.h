
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
// KeySig
//----------------------------------------------------------------------------

class KeySig: public LayerElement
{
public:
    // constructors and destructors
    KeySig();
    KeySig(int num_alter, char alter);
    virtual ~KeySig();
    
    virtual std::string GetClassName( ) { return "KeySig"; };
    
    /**
     * Return the default horizontal spacing after a keysig.
     */
    virtual int GetHorizontalSpacing( ) { return 20; };
    
    unsigned char GetAlterationAt(int pos);
    int GetOctave(unsigned char pitch, char clef);
    
    /* Alteration number getter/setter */
    int GetAlterationNumber() { return m_num_alter; };
    void SetAlterationNumber(int n) { m_num_alter = n; };
    
    /* Alteration number getter/setter */
    unsigned char GetAlteration() { return m_alteration; };
    void SetAlteration(int n) { m_alteration = n; };
    
private:
    
public:
    
private:
    static unsigned char flats[];
    static unsigned char sharps[];
    static int octave_map[2][9][7];
    
    int m_num_alter;
    unsigned char m_alteration;
    
};

} // namespace vrv    

#endif
