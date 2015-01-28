/////////////////////////////////////////////////////////////////////////////
// Name:        layer.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "layer.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "custos.h"
#include "doc.h"
#include "keysig.h"
#include "mensur.h"
#include "metersig.h"
#include "note.h"
#include "staff.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Layer
//----------------------------------------------------------------------------

Layer::Layer( ):
	DocObject("layer-"), DrawingListInterface(), ObjectListInterface(),
    AttCommon()
{
    Reset();
}

Layer::~Layer()
{
    if (m_drawingClef) {
        delete m_drawingClef;
    }
    if (m_drawingKeySig) {
        delete m_drawingKeySig;
    }
    if (m_drawingMensur) {
        delete m_drawingMensur;
    }
    if (m_drawingMeterSig) {
        delete m_drawingMeterSig;
    }    
}

void Layer::Reset()
{
    DocObject::Reset();
    DrawingListInterface::Reset();
    ResetCommon();
    
    m_drawingClef = NULL;
    m_drawingKeySig = NULL;
    m_drawingMensur = NULL;
    m_drawingMeterSig = NULL;
    m_drawingStemDir = STEMDIRECTION_NONE;
}
    
void Layer::AddLayerElement( LayerElement *element, int idx )
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
    
LayerElement *Layer::GetPrevious( LayerElement *element )
{
    this->ResetList( this );
    if ( !element || this->GetList(this)->empty() )
        return NULL;
    
    return dynamic_cast<LayerElement*>( GetListPrevious( element ) );
}

LayerElement *Layer::GetAtPos( int x )
{
	LayerElement *element = dynamic_cast<LayerElement*>( this->GetFirst() );
	if ( !element )
		return NULL;

    
	int dif = x - element->GetDrawingX();
    LayerElement *next = NULL;
	while ( (next = dynamic_cast<LayerElement*>( this->GetNext() ) ) && (int)element->GetDrawingX() < x ){
		element = next;
		if ( (int)element->GetDrawingX() > x && dif < (int)element->GetDrawingX() - x )
			return this->GetPrevious( element );
		dif = x - element->GetDrawingX();
	}
	
	return element;
}
    
void Layer::SetDrawingClef( Clef *clef )
{
    if (m_drawingClef) {
        delete m_drawingClef;
    }
    m_drawingClef = clef;
}

void Layer::SetDrawingKeySig( KeySig *keySig )
{
    if (m_drawingKeySig) {
        delete m_drawingKeySig;
    }
    m_drawingKeySig = keySig;
}

void Layer::SetDrawingMensur( Mensur *mensur )
{
    if (m_drawingMensur) {
        delete m_drawingMensur;
    }
    m_drawingMensur = mensur;
}

void Layer::SetDrawingMeterSig( MeterSig *meterSig )
{
    if (m_drawingMeterSig) {
        delete m_drawingMeterSig;
    }
    m_drawingMeterSig = meterSig;
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
	LayerElement *next = dynamic_cast<LayerElement*>( this->GetFirst() );
	while ( next && (next->GetDrawingX() < x) )
	{
        idx++;
        // update section and measure if necessary (no section breaks and measure breaks for now)
		next = dynamic_cast<LayerElement*>( this->GetNext( ) );
		if ( !next ) {
			break;
        }
	}
    
    // Insert in the logical tree - this works only for facsimile (transcription) encoding
    insertElement->m_xAbs = x;
    AddLayerElement( insertElement, idx );
    
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
    AddLayerElement( layerElement , idx );
}

void Layer::SetDrawingValues( ScoreDef *currentScoreDef, StaffDef *currentStaffDef )
{
    if (!currentStaffDef || !currentScoreDef) {
        LogDebug("scoreDef and/or staffDef not found");
        return;
    }
    
    this->SetDrawingClef( NULL );
    this->SetDrawingKeySig( NULL );
    this->SetDrawingMensur( NULL );
    this->SetDrawingMeterSig( NULL );
    
    if ( currentStaffDef->DrawClef() ) {
        if ( currentStaffDef->GetClef() ) {
            this->SetDrawingClef( currentStaffDef->GetClefCopy() );
        }
        else {
            this->SetDrawingClef( currentScoreDef->GetClefCopy() );
        }
        currentStaffDef->SetDrawClef( false );
    }
    if ( currentStaffDef->DrawKeySig() ) {
        if ( currentStaffDef->GetKeySig() ) {
            this->SetDrawingKeySig( currentStaffDef->GetKeySigCopy() );
        }
        else {
            this->SetDrawingKeySig( currentScoreDef->GetKeySigCopy() );
        }
        currentStaffDef->SetDrawKeySig( false );
    }
    if ( currentStaffDef->DrawMensur() ) {
        if ( currentStaffDef->GetMensur() ) {
            this->SetDrawingMensur( currentStaffDef->GetMensurCopy() );
        }
        else {
            this->SetDrawingMensur( currentScoreDef->GetMensurCopy() );
        }
        currentStaffDef->SetDrawMensur( false );
    }
    if ( currentStaffDef->DrawMeterSig() ) {
        if ( currentStaffDef->GetMeterSig() ) {
            this->SetDrawingMeterSig( currentStaffDef->GetMeterSigCopy() );
        }
        else {
            this->SetDrawingMeterSig( currentScoreDef->GetMeterSigCopy() );
        }
        currentStaffDef->SetDrawMeterSig( false );
    }
    
    // also put the current clef (if any if the staffDef or the scoreDef)
    // not very efficient...
    if ( currentStaffDef->GetClef() ) {
        Clef *tmp = currentStaffDef->GetClefCopy();
        this->m_currentClef = *tmp;
        delete tmp;
    }
    else if ( currentScoreDef->GetClef() ) {
        Clef *tmp = currentStaffDef->GetClefCopy();
        this->m_currentClef = *tmp;
        delete tmp;
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

LayerElement *Layer::GetFirstOld( LayerElement *element, bool direction, const std::type_info *elementType, bool *succ)
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

Clef* Layer::GetClef( LayerElement *test )
{
	bool succ=false;

    if (!test) {
        return NULL;
    }
	
    if ( !test->IsClef() )
    {	
        test = GetFirstOld(test, BACKWARD, &typeid(Clef), &succ);
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
                    if ( note && (note->GetDur() == DUR_LG) )
                    {
                        bool removeLonga = false;
                        // we check only for the pitch, not the octave, but should be enough
                        if ( (clef->GetClefId() == F3) && ( note->GetPname() == PITCHNAME_g ) )
                            removeLonga = true;
                        else if ( (clef->GetClefId() == F4) && ( note->GetPname() == PITCHNAME_b ) )
                            removeLonga = true;
                        else if ( (clef->GetClefId() == F5) && ( note->GetPname() == PITCHNAME_d ) )
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
                    Accid *accid = dynamic_cast<Accid*>(m_children[i]);
                    if ( accid ) {
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
        else if ( element && element->IsCustos( ) ) {
            Custos *custos = dynamic_cast<Custos*>( m_children[i] );
            this->Delete( custos );
            elementCount--;
            i--;
        }
    }
}

    
//----------------------------------------------------------------------------
// Layer functor methods
//----------------------------------------------------------------------------

int Layer::AlignHorizontally( ArrayPtrVoid params )
{
    // param 0: the measureAligner (unused)
    // param 1: the time
    // param 2: the current scoreDef (unused)
    double *time = static_cast<double*>(params[1]);
    
    // we need to call it because we are overriding Object::AlignHorizontally
    this->ResetHorizontalAlignment();

    // we are starting a new layer, reset the time;
    (*time) = 0.0;
    
    if ( m_drawingClef ) {
        m_drawingClef->AlignHorizontally( params );
    }
    if ( m_drawingKeySig ) {
        m_drawingKeySig->AlignHorizontally( params );
    }
    if ( m_drawingMensur ) {
        m_drawingMensur->AlignHorizontally( params );
    }
    if ( m_drawingMeterSig ) {
        m_drawingMeterSig->AlignHorizontally( params );
    }

    return FUNCTOR_CONTINUE;
}
    
    
    int Layer::PrepareProcessingLists( ArrayPtrVoid params )
    {
        // param 0: the IntTree
        IntTree *tree = static_cast<IntTree*>(params[1]);
        // Alternate solution with StaffN_LayerN_VerseN_t
        //StaffN_LayerN_VerseN_t *tree = static_cast<StaffN_LayerN_VerseN_t*>(params[0]);
        
        Staff *staff = dynamic_cast<Staff*>( this->GetFirstParent( &typeid( Staff ) ) );
        
        assert( staff );
        
        tree->child[ staff->GetN() ].child[ this->GetN() ];
        // Alternate solution with StaffN_LayerN_VerseN_t
        //(*tree)[ staff->GetN() ][ layer->GetN() ][ this->GetN() ] = true;
        
        return FUNCTOR_CONTINUE;
    }

} // namespace vrv
