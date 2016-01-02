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

#include "floatingelement.h"
#include "layer.h"
#include "measure.h"
#include "scoredef.h"
#include "staff.h"
#include "system.h"
#include "textelement.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// EditorialElement
//----------------------------------------------------------------------------

EditorialElement::EditorialElement():
    DocObject("ee-"),
    AttCommon()
{
    RegisterAttClass(ATT_COMMON);
    Reset();
}

EditorialElement::EditorialElement(std::string classid):
    DocObject(classid),
    AttCommon()
{
    RegisterAttClass(ATT_COMMON);
    Reset();
}

void EditorialElement::Reset()
{
    DocObject::Reset();
    ResetCommon();
    m_visibility = Visible;
}

EditorialElement::~EditorialElement()
{
    
}
        
void EditorialElement::AddFloatingElement( FloatingElement *child )
{
    child->SetParent( this );
    m_children.push_back( child );
    Modify();
    
}

void EditorialElement::AddTextElement( TextElement *child )
{
    child->SetParent( this );
    m_children.push_back( child );
    Modify();
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
// Abbr
//----------------------------------------------------------------------------

Abbr::Abbr( ):
    EditorialElement("abbr-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Abbr::~Abbr()
{
}

void Abbr::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Add
//----------------------------------------------------------------------------

Add::Add( ):
    EditorialElement("add-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Add::~Add()
{
}

void Add::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}


//----------------------------------------------------------------------------
// Annot
//----------------------------------------------------------------------------

Annot::Annot( ):
    EditorialElement("annot-"),
    AttPlist(),
    AttSource()
{
    RegisterAttClass( ATT_PLIST );
    RegisterAttClass( ATT_SOURCE );
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
    EditorialElement::Reset();
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
// Corr
//----------------------------------------------------------------------------

Corr::Corr( ):
    EditorialElement("corr-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Corr::~Corr()
{
}

void Corr::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Damage
//----------------------------------------------------------------------------

Damage::Damage( ):
    EditorialElement("lem-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Damage::~Damage()
{
}

void Damage::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Del
//----------------------------------------------------------------------------

Del::Del( ):
    EditorialElement("del-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Del::~Del()
{
}

void Del::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Expan
//----------------------------------------------------------------------------

Expan::Expan( ):
    EditorialElement("expan-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Expan::~Expan()
{
}

void Expan::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Lem
//----------------------------------------------------------------------------

Lem::Lem( ):
    EditorialElement("lem-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Lem::~Lem()
{
}

void Lem::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Orig
//----------------------------------------------------------------------------

Orig::Orig( ):
    EditorialElement("orig-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Orig::~Orig()
{
}

void Orig::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Rdg
//----------------------------------------------------------------------------

Rdg::Rdg( ):
    EditorialElement("rdg-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Rdg::~Rdg()
{
}

void Rdg::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Reg
//----------------------------------------------------------------------------

Reg::Reg( ):
    EditorialElement("reg-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Reg::~Reg()
{
}

void Reg::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}


//----------------------------------------------------------------------------
// Restore
//----------------------------------------------------------------------------

Restore::Restore( ):
    EditorialElement("restore-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Restore::~Restore()
{
}

void Restore::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}


//----------------------------------------------------------------------------
// Sic
//----------------------------------------------------------------------------

Sic::Sic( ):
    EditorialElement("sic-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Sic::~Sic()
{
}

void Sic::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}
    
//----------------------------------------------------------------------------
// Supplied
//----------------------------------------------------------------------------

Supplied::Supplied( ):
    EditorialElement("supplied-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Supplied::~Supplied()
{
}

void Supplied::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}

//----------------------------------------------------------------------------
// Unclear
//----------------------------------------------------------------------------

Unclear::Unclear( ):
    EditorialElement("unclear-"),
    AttSource()
{
    RegisterAttClass(ATT_SOURCE);
    Reset();
}

Unclear::~Unclear()
{
}

void Unclear::Reset()
{
    EditorialElement::Reset();
    ResetSource();
}
    
//----------------------------------------------------------------------------
// EditorialElement functor methods
//----------------------------------------------------------------------------

int EditorialElement::CastOffSystems( ArrayPtrVoid *params )
{
    // param 0: a pointer to the system we are taking the content from
    // param 1: a pointer the page we are adding system to (unused)
    // param 2: a pointer to the current system
    // param 3: the cummulated shift (m_drawingXRel of the first measure of the current system) (unused)
    // param 4: the system width (unused)
    // param 5: the current scoreDef width (unused)
    System *contentSystem = static_cast<System*>((*params).at(0));
    System **currentSystem = static_cast<System**>((*params).at(2));
    
    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert( dynamic_cast<System*>(this->m_parent));
    
    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    EditorialElement *editorialElement = dynamic_cast<EditorialElement*>( contentSystem->Relinquish( this->GetIdx()) );
    assert( editorialElement );
    (*currentSystem)->AddEditorialElement( editorialElement );
    
    return FUNCTOR_SIBLINGS;
}


} // namespace vrv
