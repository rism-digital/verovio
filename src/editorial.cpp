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
#include "layerelement.h"
#include "measure.h"
#include "scoredef.h"
#include "staff.h"
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

void EditorialElement::AddElement( LayerElement *child )
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
    
void EditorialElement::AddStaff( Staff *child )
{
    child->SetParent( this );
    m_children.push_back( child );
    Modify();
    if ( child->GetN() < 1 ) {
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
    DocObject("app-"),
    AttCommon()
{
    m_level = EDITORIAL_UNDEFINED;
    Reset();
}
    
App::App( EditorialLevel level ):
    DocObject("app-"),
    AttCommon()
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


} // namespace vrv
