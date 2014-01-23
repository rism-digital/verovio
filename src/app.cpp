/////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Author:      Laurent Pugin
// Created:     2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "app.h"

//----------------------------------------------------------------------------

#include "io.h"

namespace vrv {

//----------------------------------------------------------------------------
// LayerApp
//----------------------------------------------------------------------------

LayerApp::LayerApp():
    LayerElement()
{
}

LayerApp::~LayerApp()
{
    
}

void LayerApp::AddLayerRdg( LayerRdg *layerRdg )
{
    //assert(this->m_layer->m_staff); // Layer staff cannot be NULL when adding and <rdg>" 
    // The the to the parent staff will be NULL!
    //layerRdg->SetStaff( this->m_layer->m_staff );
    
	layerRdg->SetParent( this );
    m_children.push_back( layerRdg );
    Modify();
}

int LayerApp::Save( ArrayPtrVoid params )
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream*>(params[0]);  
    if (!output->WriteLayerApp( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
}

//----------------------------------------------------------------------------
// LayerRdg
//----------------------------------------------------------------------------

LayerRdg::LayerRdg( ):
    Object()
{

}


LayerRdg::~LayerRdg()
{
    
}


void LayerRdg::AddElement( LayerElement *element, int idx )
{
	element->SetParent( this );
    if ( idx == -1 ) {
        m_children.push_back( element );
    }
    else {
        InsertChild( element, idx );
    }
}


int LayerRdg::Save( ArrayPtrVoid params )
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream*>(params[0]);         
    if (!output->WriteLayerRdg( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
}
    
} // namespace vrv
