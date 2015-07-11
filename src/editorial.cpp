/////////////////////////////////////////////////////////////////////////////
// Name:        editorial.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "editorial.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "layer.h"
#include "measure.h"
#include "scoredef.h"
#include "staff.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// EditorialElement
//----------------------------------------------------------------------------

EditorialElement::EditorialElement():
    DocObject("ee-")
{
    Reset();
}

EditorialElement::EditorialElement(std::string classid):
    DocObject(classid)
{
    Reset();
}

void EditorialElement::Reset()
{
    Object::Reset();
    ResetCommon();
    m_visibility = Visible;
}

EditorialElement::~EditorialElement()
{
    
}

void EditorialElement::AddLayer( Layer *child )
{
    child->SetParent( this );
    m_children.push_back( child );
    Modify();
    if ( child->GetN() < 1 ) {
        LogError("Layer without @n is not supported within editorial markup element");
    }
}

void EditorialElement::AddLayerElement( LayerElement *child )
{
    child->SetParent( this );
    m_children.push_back( child );
    Modify();
}
    
void EditorialElement::AddMeasure( Measure *child )
{
    child->SetParent( this );
    m_children.push_back( child );
    Modify();
}

void EditorialElement::AddScoreDef( ScoreDef *child )
{
    child->SetParent( this );
    m_children.push_back( child );
    Modify();
}
    
void EditorialElement::AddMeasureElement( MeasureElement *child )
{
    child->SetParent( this );
    m_children.push_back( child );
    Modify();
    if ( dynamic_cast<Staff*>(child) &&  dynamic_cast<Staff*>(child)->GetN() < 1 ) {
        LogError("Staff without @n is not supported within editorial markup element");
    }
}
    
void EditorialElement::AddStaffDef( StaffDef *child )
{
    child->SetParent( this );
    m_children.push_back( child );
    Modify();
}
    
void EditorialElement::AddStaffGrp( StaffGrp *child )
{
    //assert( m_children.empty() );
    child->SetParent( this );
    m_children.push_back( child );
    Modify();
}
    
//----------------------------------------------------------------------------
// App
//----------------------------------------------------------------------------

App::App():
    EditorialElement("app-")
{
    m_level = EDITORIAL_UNDEFINED;
    Reset();
}
    
App::App( EditorialLevel level ):
    EditorialElement("app-")
{
    m_level = level;
    Reset();
}
    
void App::Reset()
{
    Object::Reset();
    ResetCommon();
}

App::~App()
{
}

void App::AddLemOrRdg(EditorialElement *child)
{
    assert( dynamic_cast<Lem*>(child) || dynamic_cast<Rdg*>(child) );
    
    child->SetParent( this );
    m_children.push_back(child);
    Modify();
}
    
//----------------------------------------------------------------------------
// Lem
//----------------------------------------------------------------------------

Lem::Lem( ):
    EditorialElement("lem-")
{
    Reset();
}

Lem::~Lem()
{
}

void Lem::Reset()
{
    EditorialElement::Reset();
}

//----------------------------------------------------------------------------
// Rdg
//----------------------------------------------------------------------------

Rdg::Rdg( ):
    EditorialElement("rdg-")
{
    Reset();
}

Rdg::~Rdg()
{
}

void Rdg::Reset()
{
    EditorialElement::Reset();
}

//----------------------------------------------------------------------------
// Annot
//----------------------------------------------------------------------------

Annot::Annot( ):
    EditorialElement("annot-"),
    AttPlist(),
    AttSource()
{
    Reset();
}

Annot::~Annot()
{
}

void Annot::Reset()
{
    EditorialElement::Reset();
    ResetPlist();
    ResetSource();
}
    
//----------------------------------------------------------------------------
// EditorialElement functor methods
//----------------------------------------------------------------------------

int EditorialElement::CastOffSystems( ArrayPtrVoid params )
{
    // param 0: a pointer to the system we are taking the content from
    // param 1: a pointer the page we are adding system to (unused)
    // param 2: a pointer to the current system
    // param 3: the cummulated shift (m_drawingXRel of the first measure of the current system) (unused)
    // param 4: the system width (unused)
    System *contentSystem = static_cast<System*>(params[0]);
    System **currentSystem = static_cast<System**>(params[2]);
    
    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert( dynamic_cast<System*>(this->m_parent));
    
    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    EditorialElement *editorialElement = dynamic_cast<EditorialElement*>( contentSystem->Relinquish( this->GetIdx()) );
    (*currentSystem)->AddEditorialElement( editorialElement );
    
    return FUNCTOR_SIBLINGS;
}


} // namespace vrv
