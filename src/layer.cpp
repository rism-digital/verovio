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
#include "measure.h"
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
    // own pointers need to be initialized before Reset()
    m_currentClef = NULL;
    m_currentKeySig = NULL;
    m_currentMensur = NULL;
    m_currentMeterSig = NULL;
    Reset();
}

Layer::~Layer()
{
    if (m_currentClef) {
        delete m_currentClef;
    }
    if (m_currentKeySig) {
        delete m_currentKeySig;
    }
    if (m_currentMensur) {
        delete m_currentMensur;
    }
    if (m_currentMeterSig) {
        delete m_currentMeterSig;
    }    
}

void Layer::Reset()
{
    DocObject::Reset();
    DrawingListInterface::Reset();
    ResetCommon();
    
    if (m_currentClef) {
        delete m_currentClef;
    }
    if (m_currentKeySig) {
        delete m_currentKeySig;
    }
    if (m_currentMensur) {
        delete m_currentMensur;
    }
    if (m_currentMeterSig) {
        delete m_currentMeterSig;
    }
    // Have at least a clef by default
    m_currentClef = new Clef();
    m_currentKeySig = NULL;
    m_currentMensur = NULL;
    m_currentMeterSig = NULL;
    m_drawClef = false;
    m_drawKeySig = false;
    m_drawMensur = false;
    m_drawMeterSig = false;
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
	if ( !element || element->GetDrawingX() > x ) return NULL;
    
    LayerElement *next = NULL;
	while ( (next = dynamic_cast<LayerElement*>( this->GetNext() ) ) ) {
		if ( next->GetDrawingX() > x ) return element;
        element = next;
	}
	return element;
}
    
void Layer::SetCurrentClef( Clef *clef )
{
    if (clef) {
        if (m_currentClef) delete m_currentClef;
        m_currentClef = clef;
    }
}

void Layer::SetCurrentKeySig( KeySig *keySig )
{
    if (keySig) {
        if (m_currentKeySig) delete m_currentKeySig;
        m_currentKeySig = keySig;
    }
}

void Layer::SetCurrentMensur( Mensur *mensur )
{
    if (mensur) {
        if (m_currentMensur) delete m_currentMensur;
        m_currentMensur = mensur;
    }
}

void Layer::SetCurrentMeterSig( MeterSig *meterSig )
{
    if (meterSig) {
        if (m_currentMeterSig) delete m_currentMeterSig;
        m_currentMeterSig = meterSig;
    }
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

void Layer::SetDrawingAndCurrentValues( ScoreDef *currentScoreDef, StaffDef *currentStaffDef )
{
    if (!currentStaffDef || !currentScoreDef) {
        LogDebug("scoreDef and/or staffDef not found");
        return;
    }
    
    this->SetDrawClef( currentStaffDef->DrawClef() );
    this->SetDrawKeySig( currentStaffDef->DrawKeySig() );
    this->SetDrawMensur( currentStaffDef->DrawMensur() );
    this->SetDrawMeterSig( currentStaffDef->DrawMeterSig() );
    // Don't draw on the next one
    currentStaffDef->SetDrawClef( false );
    currentStaffDef->SetDrawKeySig( false );
    currentStaffDef->SetDrawMensur( false );
    currentStaffDef->SetDrawMeterSig( false );

    if ( currentStaffDef->GetClef() ) {
        this->SetCurrentClef( currentStaffDef->GetClefCopy() );
    }
    else {
        this->SetCurrentClef( currentScoreDef->GetClefCopy() );
    }

    if ( currentStaffDef->GetKeySig() ) {
        this->SetCurrentKeySig( currentStaffDef->GetKeySigCopy() );
    }
    else {
        this->SetCurrentKeySig( currentScoreDef->GetKeySigCopy() );
    }

    if ( currentStaffDef->GetMensur() ) {
        this->SetCurrentMensur( currentStaffDef->GetMensurCopy() );
    }
    else {
        this->SetCurrentMensur( currentScoreDef->GetMensurCopy() );
    }

    if ( currentStaffDef->GetMeterSig() ) {
        this->SetCurrentMeterSig( currentStaffDef->GetMeterSigCopy() );
    }
    else {
        this->SetCurrentMeterSig( currentScoreDef->GetMeterSigCopy() );
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


Clef* Layer::GetClef( LayerElement *test )
{
    Object *testObject = test;
    
    if (!test) {
        return m_currentClef;
    }
	
    //make sure list is set
    ResetList(this);
    if ( !test->IsClef() )
    {
        testObject = GetListFirstBackward(testObject, &typeid(Clef));
    }
    
    if ( dynamic_cast<Clef*>(testObject) ) {
        return dynamic_cast<Clef*>(testObject);
    }

    return m_currentClef;
}
 
int Layer::GetClefOffset( LayerElement *test )
{
    Clef *clef = GetClef(test);
    if (!clef) return 0;
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
                    if ( note && (note->GetActualDur() == DUR_LG) )
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
    // param 2: the current Mensur
    double *time = static_cast<double*>(params[1]);
    Mensur **currentMensur = static_cast<Mensur**>(params[2]);
    MeterSig **currentMeterSig = static_cast<MeterSig**>(params[3]);
    
    // we need to call it because we are overriding Object::AlignHorizontally
    this->ResetHorizontalAlignment();

    // we are starting a new layer, reset the time;
    (*time) = 0.0;
    
    (*currentMensur) = m_currentMensur;
    (*currentMeterSig) = m_currentMeterSig;
    
    if ( m_drawClef && m_currentClef ) {
        m_currentClef->AlignHorizontally( params );
    }
    if ( m_drawKeySig && m_currentKeySig ) {
        m_currentKeySig->AlignHorizontally( params );
    }
    if ( m_drawMensur && m_currentMensur) {
        m_currentMensur->AlignHorizontally( params );
    }
    if ( m_drawMeterSig && m_currentMeterSig ) {
        m_currentMeterSig->AlignHorizontally( params );
    }

    return FUNCTOR_CONTINUE;
}
    
int Layer::PrepareProcessingLists( ArrayPtrVoid params )
{
    // param 0: the IntTree* for staff/layer/verse (unused)
    // param 1: the IntTree* for staff/layer
    IntTree *tree = static_cast<IntTree*>(params[1]);
    // Alternate solution with StaffN_LayerN_VerseN_t
    //StaffN_LayerN_VerseN_t *tree = static_cast<StaffN_LayerN_VerseN_t*>(params[0]);
    
    Staff *staff = dynamic_cast<Staff*>( this->GetFirstParent( &typeid( Staff ) ) );
    assert( staff );
    tree->child[ staff->GetN() ].child[ this->GetN() ];
    
    return FUNCTOR_CONTINUE;
}
    
int Layer::SetDrawingXY( ArrayPtrVoid params )
{
    // param 0: a pointer doc (unused)
    // param 1: a pointer to the current system (unused)
    // param 2: a pointer to the current measure
    // param 3: a pointer to the current staff (unused)
    // param 4: a pointer to the current layer
    // param 5: a pointer to the view (unused)
    // param 6: a bool indicating if we are processing layer elements or not
    Measure **currentMeasure = static_cast<Measure**>(params[2]);
    Layer **currentLayer = static_cast<Layer**>(params[4]);
    bool *processLayerElements = static_cast<bool*>(params[6]);
    
    (*currentLayer) = this;
    
    // Second pass where we do just process layer elements
    if ((*processLayerElements)) {
        return FUNCTOR_CONTINUE;
    }
    
    // set the values for the scoreDef elements when required
    if (this->GetDrawingClef()) {
        this->GetDrawingClef()->SetDrawingX( this->GetDrawingClef()->GetXRel() + (*currentMeasure)->GetDrawingX() );
    }
    if (this->GetDrawingKeySig()) {
        this->GetDrawingKeySig()->SetDrawingX( this->GetDrawingKeySig()->GetXRel() + (*currentMeasure)->GetDrawingX() );
    }
    if (this->GetDrawingMensur()) {
        this->GetDrawingMensur()->SetDrawingX( this->GetDrawingMensur()->GetXRel() + (*currentMeasure)->GetDrawingX() );
    }
    if (this->GetDrawingMeterSig()) {
        this->GetDrawingMeterSig()->SetDrawingX( this->GetDrawingMeterSig()->GetXRel() + (*currentMeasure)->GetDrawingX() );
    }
    
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
