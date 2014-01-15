/////////////////////////////////////////////////////////////////////////////
// Name:        symbol.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_SYMBOL_H__
#define __VRV_SYMBOL_H__

#include "vrvdef.h"
#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

//----------------------------------------------------------------------------
// Symbol
//----------------------------------------------------------------------------

/** 
 * This class models various MEI that do not have their own class yet.
 * The element name is given by the m_type member.
 * The DOT type models the MEI <dot> element.
 * The ACCID type models the MEI <accid> element.
 */
class Symbol: public LayerElement, public PositionInterface
{
public:
    // constructors and destructors
    Symbol();
    Symbol( SymbolType type );
    virtual ~Symbol();
    
    /**
     * Comparison operator. 
     * Check if the LayerElement if a Symbol and compare attributes
     */
    virtual bool operator==(Object& other);
    
    virtual std::string GetClassName( ) { return "Symbol"; };
    
    /**
     * Set the value for the symbol.
     */
	virtual void SetValue( int value, int flag = 0 );
    
    /** Set the position */
    //virtual void SetPitchOrPosition( int pname, int oct ) { PositionInterface::SetPosition( pname, oct ); };
    
    /** Get the position */
    //virtual bool GetPitchOrPosition( int *pname, int *oct ) { return PositionInterface::GetPosition( pname, oct ); };

    
    void Init( SymbolType type );
    
private:
    
public:
    /** Indicates the type of the symbole */
    SymbolType m_type;
    
    /** For DOT, indicates the number of dots */
    unsigned char m_dot;
    
    /** For ACCID, indicates the accidental */
    unsigned char m_accid;

private:
    
};

} // namespace vrv
    
#endif
