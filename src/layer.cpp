/////////////////////////////////////////////////////////////////////////////
// Name:        muslayer.cpp
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
    FileOutputStream *output = (FileOutputStream*)params[0];       
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
	return (LayerElement*)m_children[0];
}

LayerElement *Layer::GetLast( )
{
	if ( m_children.empty() )
		return NULL;
	int i = (int)m_children.size() - 1;
	return (LayerElement*)m_children[i];
}

LayerElement *Layer::GetNext( LayerElement *element )
{	
    this->ResetList( this );
    
    if ( !element || m_list.empty() )
        return NULL;
    
    return (LayerElement*)GetListNext( element );
}

LayerElement *Layer::GetPrevious( LayerElement *element )
{
    this->ResetList( this );
    
    if ( !element || m_list.empty() )
        return NULL;
    
    return (LayerElement*)GetListPrevious( element );
}

LayerElement *Layer::GetAtPos( int x )
{
	LayerElement *element = this->GetFirst();
	if ( !element )
		return NULL;

    
	int dif = x - element->m_xDrawing;
    LayerElement *next = NULL;
	while ( (next = this->GetNext( element )) && (int)element->m_xDrawing < x ){
		element = next;
		if ( (int)element->m_xDrawing > x && dif < (int)element->m_xDrawing - x )
			return this->GetPrevious( element );
		dif = x - element->m_xDrawing;
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
    
    // This is temporary and is OK because we have only one single layout for now.
    // Inserting elements should be done from the logical tree and then update the layout
    
    LayerElement *insertElement = element->GetChildCopy();

    // First we find the element after which we are inserting the element
    // If not, it will be NULL
    // We are also updating the section and measure ( TODO, not necessary for now )
    int idx = 0;
	LayerElement *next = this->GetFirst();
	while ( next && (next->m_xDrawing < x) )
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
            element = (LayerElement*)GetListPrevious(element);
		}
		else
		{	if (i >= (int)GetList(this)->size() - 1 ) {
                *succ = false;
                break;
            }
			i++;
			element = (LayerElement*)GetListNext(element);
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
        if ( ((LayerElement*)m_children[i])->IsClef() ) {
            Clef *clef = dynamic_cast<Clef*>(m_children[i]);
            // we remove the clef because it is the same as the previous one
            if ( currentClef && ((*currentClef) == (*clef)) ) {
                // check if it is a F clef with a Longa before
                if ( (i > 0) && ((LayerElement*)m_children[i - 1])->IsNote() )
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
        else if ( ((LayerElement*)m_children[i])->IsSymbol( SYMBOL_CUSTOS ) ) {
            Symbol *symbol = dynamic_cast<Symbol*>(m_children[i]);
            this->Delete( symbol );
            elementCount--;
            i--;
        }
    }
}


    
/*
// Gets the y coordinate of the previous lyric. If lyric is NULL, it will return the y coordinate of the first lyric 
// in the stave. If there are no lyrics in the Stave -1 is returned.
int Layer::GetLyricPos( Symbol1 *lyric )
{
	Symbol1 *tmp;
	if ( !lyric ){
		if ( !( tmp = GetFirstLyric() ) )
			return -1;
		return tmp->dec_y;
	}
	
	if ( !( tmp = GetPreviousLyric( lyric ) ) )
		return -1;
	return tmp->dec_y;
}

Symbol1 *Layer::GetPreviousLyric( Symbol1 *lyric )
{
	if ( !lyric || m_children.IsEmpty() || !lyric->m_note_ptr || lyric->m_note_ptr->no < 0 )
		return NULL;
	
	// If there are other lyrics attached to the note that lyric is attached to...
	if ( (int)lyric->m_note_ptr->m_lyrics.GetCount() > 1 ){
		bool check = false; // Keeps track if we have past the pointer to this element in m_lyrics
		for ( int i = (int)lyric->m_note_ptr->m_lyrics.GetCount() - 1; i >= 0; i-- ){
			Symbol1 *previousLyric = &lyric->m_note_ptr->m_lyrics[i];
			if ( check ) return previousLyric;
			if ( previousLyric == lyric ) check = true;
		}
	}
	// Check previous note in staff for lyric
	int no = lyric->m_note_ptr->no - 1;
	while ( no >= 0 ){
		if ( m_children[ no ].IsNote() ){
			for ( int i = (int) ((Note1*)m_children[ no ])->m_lyrics.GetCount() - 1; i >= 0 ; i-- ){
				Symbol1 *previousLyric = ((Note1*)m_children[ no ])->m_lyrics[i];
				if ( previousLyric ) return previousLyric;
			}
		}
		no--;
	}
	return NULL;
}

Symbol1 *Layer::GetNextLyric( Symbol1 *lyric )
{	
	if ( !lyric || m_children.IsEmpty() || !lyric->m_note_ptr || lyric->m_note_ptr->no > (int)m_children.GetCount() - 1 )
		return NULL;
	
	// If there are other lyrics attached to the note that lyric is attached to...
	if ( (int)lyric->m_note_ptr->m_lyrics.GetCount() > 1 ){
		bool check = false; // Keeps track if we have past the pointer to this element in m_lyrics
		for ( int i = 0; i < (int)lyric->m_note_ptr->m_lyrics.GetCount(); i++ ){
			Symbol1 *nextLyric = &lyric->m_note_ptr->m_lyrics[i];
			if ( check ) 
				return nextLyric;
			if ( nextLyric == lyric ) 
				check = true;
		}
	}
	// Check next note in staff for lyric
	int no = lyric->m_note_ptr->no + 1;
	while ( no < (int)m_children.GetCount() ){
		if ( m_children[ no ].IsNote() ){
			for ( int i = 0; i < (int) ((Note1*)m_children[ no ])->m_lyrics.GetCount(); i++ ){
				Symbol1 *nextLyric = ((Note1*)m_children[ no ])->m_lyrics[i];
				if ( nextLyric )
					return nextLyric;
			}
		}
		no++;
	}
	return NULL;
}

Symbol1 *Layer::GetFirstLyric( )
{
	if ( m_children.IsEmpty() )
		return NULL;
	int no = 0;
	while ( no < (int)m_children.GetCount() ){
		if ( m_children[ no ].IsNote() ){
			for ( int i = 0; i < (int) ((Note1*)m_children[ no ])->m_lyrics.GetCount(); i++ ){
				Symbol1 *lyric = ((Note1*)m_children[ no ])->m_lyrics[i];
				if ( lyric )
					return lyric;
			}
		}
		no++;
	}
	return NULL;	
}

Symbol1 *Layer::GetLastLyric( )
{
	if ( m_children.IsEmpty() )
		return NULL;
	int no = (int)m_children.GetCount() - 1;
	while ( no >= 0 ){
		if ( m_children[ no ].IsNote() ) {
			for ( int i = (int) ((Note1*)m_children[ no ])->m_lyrics.GetCount() - 1; i >= 0 ; i-- ){
				Symbol1 *lyric = ((Note1*)m_children[ no ])->m_lyrics[i];
				if ( lyric )
					return lyric;
			}
		}
		no--;
	}
	return NULL;
}

Symbol1 *Layer::GetLyricAtPos( int x )
{
	Symbol1 *lyric = this->GetFirstLyric();
	if ( !lyric )
		return NULL;
	
	//int xx = 0;
	int dif = x - lyric->m_xAbs;
	while ( this->GetNextLyric( lyric ) && (int)lyric->m_xAbs < x ){
		lyric = this->GetNextLyric( lyric );
		if ( (int)lyric->m_xAbs > x && dif < (int)lyric->m_xAbs - x )
			return this->GetPreviousLyric( lyric );
		dif = x - lyric->m_xAbs;
	}
		
	return lyric;
}

void Layer::DeleteLyric( Symbol1 *symbol )
{
	if ( !symbol ) return;
	
	Note1 *note = symbol->m_note_ptr;
	for ( int i = 0; i < (int)note->m_lyrics.GetCount(); i++ ){
		Symbol1 *lyric = &note->m_lyrics[i];
		if ( symbol == lyric )
			note->m_lyrics.Detach(i);
	}
	
	this->Delete( symbol );
}

Note1 *Layer::GetNextNote( Symbol1 * lyric )
{
	if ( !lyric || m_children.IsEmpty() || !lyric->m_note_ptr || lyric->m_note_ptr->no >= (int)m_children.GetCount() - 1 )
		return NULL;
	
	int no = lyric->m_note_ptr->no + 1;
	for ( int i = no; i < (int)m_children.GetCount(); i++ ){
		LayerElement *element = m_children[i];
		if ( element->IsNote() && ((Note1*)element)->sil == _NOT )
			return (Note1*)element; 
	}
	return NULL;
}

Note1 *Layer::GetPreviousNote( Symbol1 * lyric )
{
	if ( !lyric || m_children.IsEmpty() || !lyric->m_note_ptr || lyric->m_note_ptr->no <= 0 )
		return NULL;
	
	int no = lyric->m_note_ptr->no - 1;
	for ( int i = no; i >= 0; i-- ){
		LayerElement *element = m_children[i];
		if ( element->IsNote() && ((Note1*)element)->sil == _NOT )
			return (Note1*)element; 
	}
	return NULL;
}

//Switches the note association of lyric from oldNote to newNote and modifies the two notes accordingly
//bool beginning: indicates if we want to add the lyric to beginning or end of the lyric array in newNote 
//		true = beginning of array
//		false = end of array
void Layer::SwitchLyricNoteAssociation( Symbol1 *lyric, Note1 *oldNote, Note1* newNote, bool beginning )
{
	if ( !lyric || !oldNote || !newNote )
		return;
	
	lyric->m_note_ptr = newNote;
	if ( beginning )
		newNote->m_lyrics.Insert( lyric, 0 );
	else
		newNote->m_lyrics.Insert( lyric, newNote->m_lyrics.GetCount() );
	
	for ( int i = 0; i < (int)oldNote->m_lyrics.GetCount(); i++ ){
		Symbol1 *element = &oldNote->m_lyrics[i];
		if ( element == lyric ){
			oldNote->m_lyrics.Detach(i);
			break;
		}			
	}
}

void Layer::AdjustLyricLineHeight( int delta ) 
{
	for ( int i = 0; i < (int)m_children.GetCount(); i++ ){
		LayerElement *element = m_children[i];
		if ( element->IsNote() ){
			for ( int j = 0; j < (int)((Note1*)element)->m_lyrics.GetCount(); j++ ){
				Symbol1 *lyric = &((Note1*)element)->m_lyrics[j];
				lyric->dec_y += delta;
			}
		}
	}
}
*/

//----------------------------------------------------------------------------
// Layer functor methods
//----------------------------------------------------------------------------

int Layer::CopyToLayer( ArrayPtrVoid params )
{  
    // Things we might want to add: 
    // - checking that the parent is a staff to avoid copying MusApp
    // - adding a parent nbLogStaff and a nbLogLayer parameter for copying a specific staff / layer
    
    
    // param 0: the Layer we need to copy to
	Layer *destinationLayer = (Layer*)params[0]; 
    // param 1: the uuid of the start element (if any)
    std::string *start = (std::string*)params[1];
    // param 2: the uuid of the end element (if any)
    std::string *end = (std::string*)params[2];
    // param 3: we have a start element and have started
    bool *has_started = (bool*)params[3];
    // param 4: we have an end element and have ended
    bool *has_ended = (bool*)params[4];
    // param 5: we want a new uuid for the copied elements
    bool *new_uuid = (bool*)params[5];
    
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
        LayerElement *copy = ((LayerElement*)m_children[i])->GetChildCopy( (*new_uuid) );
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

int Layer::Align( ArrayPtrVoid params )
{
    // param 0: the measureAligner (unused)
    // param 1: the time
    // param 2: the systemAligner (unused)
    // param 3: the staffNb (unused)
    double *time = (double*)params[1];

    // we are starting a new layer, reset the time;
    (*time) = 0.0;
    
    if ( m_clef ) {
        m_clef->Align( params );
    }
    if ( m_keySig ) {
        m_keySig->Align( params );
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
