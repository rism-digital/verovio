/////////////////////////////////////////////////////////////////////////////
// Name:        layer.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "layer.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <typeinfo>

//----------------------------------------------------------------------------

#include "clef.h"
#include "vrvdef.h"
#include "doc.h"
#include "keysig.h"
#include "io.h"
#include "layerelement.h"
#include "note.h"
#include "symbol.h"

namespace vrv {

//----------------------------------------------------------------------------
// Layer
//----------------------------------------------------------------------------

Layer::Layer( int n ):
	DocObject("layer-"), ObjectListInterface(), ScoreOrStaffDefAttrInterface()
{
    assert( n > 0 );
    m_n = n;
    
	Clear( );
}

Layer::~Layer()
{
    
}

void Layer::Clear()
{
    ClearChildren();
	voix = 0;
    m_drawingList.clear();
}


int Layer::Save( ArrayPtrVoid params )
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream*>(params[0]);         
    if (!output->WriteLayer( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;

}


void Layer::AddElement( LayerElement *element, int idx )
{
	element->SetParent( this );
    if ( idx == -1 ) {
        m_children.push_back( element );
    }
    else {
        InsertChild( element, idx );
    }
    Modify();
}

void Layer::CopyAttributes( Layer *nlayer )
{
	if ( !nlayer )
		return;

	nlayer->Clear();
	nlayer->voix = voix;
}

LayerElement *Layer::GetFirst( )
{
	if ( m_children.empty() )
		return NULL;
	return dynamic_cast<LayerElement*>(m_children[0]);
}

LayerElement *Layer::GetLast( )
{
	if ( m_children.empty() )
		return NULL;
	int i = (int)m_children.size() - 1;
	return dynamic_cast<LayerElement*>(m_children[i]);
}

LayerElement *Layer::GetNext( LayerElement *element )
{	
    this->ResetList( this );
    
    if ( !element || m_list.empty() )
        return NULL;
    
    return dynamic_cast<LayerElement*>( GetListNext( element ) );
}

LayerElement *Layer::GetPrevious( LayerElement *element )
{
    this->ResetList( this );
    
    if ( !element || m_list.empty() )
        return NULL;
    
    return dynamic_cast<LayerElement*>( GetListPrevious( element ) );
}

LayerElement *Layer::GetAtPos( int x )
{
	LayerElement *element = this->GetFirst();
	if ( !element )
		return NULL;

    
	int dif = x - element->m_drawingX;
    LayerElement *next = NULL;
	while ( (next = this->GetNext( element )) && (int)element->m_drawingX < x ){
		element = next;
		if ( (int)element->m_drawingX > x && dif < (int)element->m_drawingX - x )
			return this->GetPrevious( element );
		dif = x - element->m_drawingX;
	}
	
	return element;
}

void Layer::AddToDrawingList( LayerElement *element )
{
    m_drawingList.push_back( element );
}

ListOfObjects *Layer::GetDrawingList( )
{
    return &m_drawingList;
}

void Layer::ResetDrawingList( )
{
    m_drawingList.clear();
}

LayerElement *Layer::Insert( LayerElement *element, int x )
{
	if ( !element ) { 
        return NULL;
    }
    
    LayerElement *insertElement = element->GetChildCopy();

    // First we find the element after which we are inserting the element
    // If not, it will be NULL
    // We are also updating the section and measure ( TODO, not necessary for now )
    int idx = 0;
	LayerElement *next = this->GetFirst();
	while ( next && (next->m_drawingX < x) )
	{
        idx++;
        // update section and measure if necessary (no section breaks and measure breaks for now)
		if ( this->GetNext( next ) )
			next = this->GetNext( next );
		else
			break;
	}
    
    // Insert in the logical tree - this works only for facsimile (transcription) encoding
    insertElement->m_xAbs = x;
    AddElement( insertElement, idx );
    
	Refresh();
    //
	return insertElement;
}


void Layer::Insert( LayerElement *layerElement, LayerElement *before )
{
    int idx = 0;
    if ( before ) {
        idx = this->GetChildIndex( before );
    }
    AddElement( layerElement , idx );
}

void Layer::SetDrawingValues( ScoreDef *currentScoreDef, StaffDef *currentStaffDef )
{
    if (!currentStaffDef || !currentScoreDef) {
        LogDebug("scoreDef and/or staffDef not found");
        return;
    }
    
    if ( currentStaffDef->DrawClef() ) {
        if ( currentStaffDef->GetClefAttr() ) {
            this->ReplaceClef( currentStaffDef->GetClefAttr() );
        }
        else {
            this->ReplaceClef( currentScoreDef->GetClefAttr() );
        }
        currentStaffDef->SetDrawClef( false );
    }
    if ( currentStaffDef->DrawKeySig() ) {
        if ( currentStaffDef->GetKeySigAttr() ) {
            this->ReplaceKeySig( currentStaffDef->GetKeySigAttr() );
        }
        else {
            this->ReplaceKeySig( currentScoreDef->GetKeySigAttr() );
        }
        currentStaffDef->SetDrawKeySig( false );
    }
    if ( currentStaffDef->DrawMensur() ) {
        if ( currentStaffDef->GetMensurAttr() ) {
            this->ReplaceMensur( currentStaffDef->GetMensurAttr() );
        }
        else {
            this->ReplaceMensur( currentScoreDef->GetMensurAttr() );
        }
        currentStaffDef->SetDrawMensur( false );
    }
    
    // also put the current clef (if any if the staffDef or the scoreDef)
    if ( currentStaffDef->GetClefAttr() ) {
        this->m_currentClef = *currentStaffDef->GetClefAttr();
    }
    else if ( currentScoreDef->GetClefAttr() ) {
        this->m_currentClef = *currentScoreDef->GetClefAttr();
    }
}


void Layer::Delete( LayerElement *element )
{
	if ( !element ) {
        return;
    }
    
    bool is_clef = false;
        
    if ( element->IsClef() ) {
        is_clef = true;
        //m_r->OnBeginEditionClef();
    }
	
    
    int pos = GetChildIndex( element );
    RemoveChildAt( pos );
    Modify();

	if ( is_clef )
	{
        //m_r->OnEndEditionClef();
	}
    
    Refresh();
}

// Dans la direction indiquee (direction), cavale sur tout element qui n'est pas un
// symbol, de la nature indiquee (flg). Retourne le ptr si succes, ou 
// l'element de depart; le ptr succ est vrai si symb trouve.

LayerElement *Layer::GetFirst( LayerElement *element, unsigned int direction, const std::type_info *elementType, bool *succ)
{	
    LayerElement *original = element;

	*succ = false;
	if (element == NULL)
		return (element);

    ResetList(this);
    
	int i = GetListIndex( element );
	if ( i == -1 )
		return (element);

    *succ = true; // we assume we will find it. Change to false if not
    while ( typeid(*element) != *elementType )
	{
		if (direction==BACKWARD)
		{	
			if (i < 1) {
                *succ = false;
                break;
            }
			i--;
            element = static_cast<LayerElement*>( GetListPrevious(element) );
		}
		else
		{	if (i >= (int)GetList(this)->size() - 1 ) {
                *succ = false;
                break;
            }
			i++;
			element = static_cast<LayerElement*>( GetListNext(element) );
		}
	}	

	return (*succ ? element : original);
}

void Layer::CheckXPosition( LayerElement *currentElement )
{
    if (!currentElement) {
        return;
    }
    
    LayerElement *previous = GetPrevious( currentElement );
    LayerElement *next = GetNext( currentElement );
    
    if ( previous && previous->m_xAbs >= currentElement->m_xAbs ) {
        currentElement->m_xAbs = previous->m_xAbs + 2;
    }
    if ( next && next->m_xAbs <= currentElement->m_xAbs ) {
        currentElement->m_xAbs = next->m_xAbs - 2;
    }
}

Clef* Layer::GetClef( LayerElement *test )
{
	bool succ=false;

    if (!test) {
        return NULL;
    }
	
    if ( !test->IsClef() )
    {	
        test = GetFirst(test, BACKWARD, &typeid(Clef), &succ);
    }
    if ( dynamic_cast<Clef*>(test) ) {
        return dynamic_cast<Clef*>(test);
    }

    return &m_currentClef;
}

int Layer::GetClefOffset( LayerElement *test )
{
    Clef *clef = GetClef(test);
    if (!clef) {
        return 0;
    }
    return clef->GetClefOffset();
    
}

void Layer::RemoveClefAndCustos()
{
    Clef *currentClef = NULL;
    
    int i;
    int elementCount =  this->GetElementCount();
    for (i = 0; i < elementCount; i++)
    {
        LayerElement *element = dynamic_cast<LayerElement*>( m_children[i] );
        if ( element && element->IsClef() ) {
            Clef *clef = dynamic_cast<Clef*>( m_children[i] );
            // we remove the clef because it is the same as the previous one
            if ( currentClef && ((*currentClef) == (*clef)) ) {
                // check if it is a F clef with a Longa before
                LayerElement *previous = dynamic_cast<LayerElement*>(m_children[i - 1]);
                if ( (i > 0) && previous && previous->IsNote() )
                {
                    Note *note = dynamic_cast<Note*>(m_children[i - 1]);
                    if ( note && (note->m_dur == DUR_LG) )
                    {
                        bool removeLonga = false;
                        // we check only for the pitch, not the octave, but should be enough
                        if ( (clef->m_clefId == FA3) && ( note->m_pname == PITCH_G ) )
                            removeLonga = true;
                        else if ( (clef->m_clefId == FA4) && ( note->m_pname == PITCH_B ) )
                            removeLonga = true;
                        else if ( (clef->m_clefId == FA5) && ( note->m_pname == PITCH_D ) )
                            removeLonga = true;
                        if ( removeLonga ) {
                            this->Delete( note );
                            elementCount--;
                            i--;
                        }
                    }
                }
                this->Delete( clef );
                elementCount--;
                // now remove alterations (keys)
                for (; i < elementCount; i++) {
                    Symbol *accid = dynamic_cast<Symbol*>(m_children[i]);
                    if ( accid && accid->IsSymbol( SYMBOL_ACCID ) ) {
                        this->Delete( accid );
                        elementCount--;
                        i--;                        
                    }
                    else
                        break;
                }
                i--;
                
            }
            else {
                currentClef = clef;
            }
        }
        else if ( element && element->IsSymbol( SYMBOL_CUSTOS ) ) {
            Symbol *symbol = dynamic_cast<Symbol*>( m_children[i] );
            this->Delete( symbol );
            elementCount--;
            i--;
        }
    }
}

    
//----------------------------------------------------------------------------
// Layer functor methods
//----------------------------------------------------------------------------

int Layer::CopyToLayer( ArrayPtrVoid params )
{  
    // Things we might want to add: 
    // - checking that the parent is a staff to avoid copying MusApp
    // - adding a parent nbLogStaff and a nbLogLayer parameter for copying a specific staff / layer
    
    
    // param 0: the Layer we need to copy to
	Layer *destinationLayer = static_cast<Layer*>(params[0]);
    // param 1: the uuid of the start element (if any)
    std::string *start = static_cast<std::string*>(params[1]);
    // param 2: the uuid of the end element (if any)
    std::string *end = static_cast<std::string*>(params[2]);
    // param 3: we have a start element and have started
    bool *has_started = static_cast<bool*>(params[3]);
    // param 4: we have an end element and have ended
    bool *has_ended = static_cast<bool*>(params[4]);
    // param 5: we want a new uuid for the copied elements
    bool *new_uuid = static_cast<bool*>(params[5]);
    
    if ( (*has_ended) ) {
        return FUNCTOR_STOP;
    }
    
    int i;
    for ( i = 0; i < this->GetElementCount(); i++ ) 
    {
        // check if we have a start uuid or if we already passed it
        if ( !start->empty() && !(*has_started) ) {
            if ( *start == m_children[i]->GetUuid() ) {
                (*has_started) = true;
            } 
            else {
                continue;
            }
        }
        
        // copy and add it
        LayerElement *element = dynamic_cast<LayerElement*>(m_children[i]);
        assert( element );
        LayerElement *copy = element->GetChildCopy( (*new_uuid) );
        destinationLayer->AddElement( copy );
        
        // check if we have a end uuid and if we have reached it. 
        if ( !end->empty() ) {
            if ( *end == m_children[i]->GetUuid() ) {
                (*has_ended) = true;
                return FUNCTOR_STOP;
            }
        }

        
    }
    return FUNCTOR_CONTINUE;
}

int Layer::AlignHorizontally( ArrayPtrVoid params )
{
    // param 0: the measureAligner (unused)
    // param 1: the time
    double *time = static_cast<double*>(params[1]);
    
    // we need to call it because we are overriding Object::AlignHorizontally
    this->ResetHorizontalAlignment();

    // we are starting a new layer, reset the time;
    (*time) = 0.0;
    
    if ( m_clef ) {
        m_clef->AlignHorizontally( params );
    }
    if ( m_keySig ) {
        m_keySig->AlignHorizontally( params );
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
