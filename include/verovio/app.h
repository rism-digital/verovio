/////////////////////////////////////////////////////////////////////////////
// Name:        app.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_APP_H__
#define __VRV_APP_H__

#include "layerelement.h"

namespace vrv {

class LayerRdg;

//----------------------------------------------------------------------------
// LayerApp
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <app> element within a <layer> element.
 */
class LayerApp: public LayerElement
{
public:
    // constructors and destructors
    LayerApp();
    virtual ~LayerApp();
    
    void AddLayerRdg( LayerRdg *layerRdg );
    
    int GetRdgCount() { return (int)m_children.size(); };
    
    // functor
    virtual int Save( ArrayPtrVoid params );
    
private:
    
public:

private:
    
};


//----------------------------------------------------------------------------
// LayerRdg
//----------------------------------------------------------------------------

/** 
 * This class models the MEI <rdg> element for a <app> in a <layer>
 */
class LayerRdg: public Object
{
public:
    // constructors and destructors
    LayerRdg( );
    virtual ~LayerRdg();
    
	void AddElement( LayerElement *element, int idx = -1 );
    
	int GetElementCount() const { return (int)m_children.size(); };
    
    // functor
    virtual int Save( ArrayPtrVoid params );

private:
    
public:
    /** The source id */
    std::string m_source;
    
private:
    
};

} // namespace vrv

#endif
