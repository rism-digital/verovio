/////////////////////////////////////////////////////////////////////////////
// Name:        layerelement.cpp
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////



//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "accid.h"
#include "aligner.h"
#include "barline.h"
#include "beam.h"
#include "chord.h"
#include "clef.h"
#include "custos.h"
#include "dot.h"
#include "keysig.h"
#include "mensur.h"
#include "metersig.h"
#include "mrest.h"
#include "multirest.h"
#include "note.h"
#include "rest.h"
#include "syl.h"
#include "tie.h"
#include "timeinterface.h"
#include "tuplet.h"
#include "verse.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// LayerElement
//----------------------------------------------------------------------------

LayerElement::LayerElement():
    DocObject("le-")
{
    Reset();
}

LayerElement::LayerElement(std::string classid):
	DocObject(classid)
{
    Reset();
}

void LayerElement::Reset()
{
    DocObject::Reset();
    
    m_cueSize = false;
    
    m_xAbs = VRV_UNSET;
    m_drawingX = 0;
    m_drawingY = 0;
    
    m_isScoreOrStaffDefAttr = false;
    m_alignment = NULL;
}
    
LayerElement::~LayerElement()
{
    
}

LayerElement& LayerElement::operator=( const LayerElement& element )
{
	if ( this != &element ) // not self assignement
	{
        // is this necessary?
        m_cueSize = element.m_cueSize;
        // pointers have to be NULL
        m_parent = NULL;
        m_alignment = NULL;
	}
	return *this;
}


LayerElement *LayerElement::GetChildCopy( bool newUuid ) 
{
    
    // Is there another way to do this in C++ ?
    // Yes, change this to the Object::Clone method - however, newUuid will not be possible in this way
    LayerElement *element = NULL;

    if ( this->IsAccid() )
        element = new Accid( *(Accid*)this );
    else if ( this->IsBarline() )
        element = new Barline( *(Barline*)this );
    else if (this->IsClef() )
        element = new Clef( *(Clef*)this );
    else if (this->IsCustos() )
        element = new Custos( *(Custos*)this );
    else if (this->IsDot() )
        element = new Dot( *(Dot*)this );
    else if (this->IsMensur() )
        element = new Mensur( *(Mensur*)this );
    else if (this->IsNote() )
        element = new Note( *(Note*)this );
    else if (this->IsRest() )
        element = new Rest( *(Rest*)this );
    else {
        LogDebug( "Missing %s", this->GetClassName().c_str() );
        assert( false ); // Copy of this type unimplemented
        return NULL;
    }
        
    element->m_parent = NULL;
    
    if ( !newUuid ) {
        element->SetUuid( this->GetUuid() );
    }
    else {
        element->ResetUuid( );
    }
    
    return element;
}
    
void LayerElement::ResetHorizontalAlignment()
{
    m_drawingX = 0;
}

void LayerElement::SetValue( int value, int flag )
{
    if ( this->HasDurationInterface() ){
        DurationInterface *duration = dynamic_cast<DurationInterface*>(this);
        duration->SetDur( value );
    }
}
    
    
bool LayerElement::IsAccid( )
{
    return (dynamic_cast<Accid*>(this));
}

bool LayerElement::IsBarline() 
{  
    return (dynamic_cast<Barline*>(this));
}

bool LayerElement::IsBeam() 
{  
    return (dynamic_cast<Beam*>(this));
}

bool LayerElement::IsChord()
{
    return (dynamic_cast<Chord*>(this));
}
    
bool LayerElement::IsClef() 
{  
    return (dynamic_cast<Clef*>(this));
}

bool LayerElement::IsCustos( )
{
    return (dynamic_cast<Custos*>(this));
}

bool LayerElement::IsDot( )
{
    return (dynamic_cast<Dot*>(this));
}
    
bool LayerElement::HasDurationInterface() 
{  
    return (dynamic_cast<DurationInterface*>(this));
}

bool LayerElement::IsKeySig()
{
    return (dynamic_cast<KeySig*>(this));
}

    
bool LayerElement::IsMRest()
{
    return (dynamic_cast<MRest*>(this));
}
    
bool LayerElement::IsMultiRest() 
{  
    return (dynamic_cast<MultiRest*>(this));
}

bool LayerElement::IsMensur() 
{  
    return (dynamic_cast<Mensur*>(this));
}
    
bool LayerElement::IsMeterSig()
{
    return (dynamic_cast<MeterSig*>(this));
}
    
bool LayerElement::IsNote() 
{  
    return (dynamic_cast<Note*>(this));
}

bool LayerElement::IsGraceNote()
{
    Note *note = dynamic_cast<Note*>(this);
    return (note && note->m_cueSize);
}
    
bool LayerElement::HasPitchInterface() 
{  
    return (dynamic_cast<PitchInterface*>(this));
}

bool LayerElement::HasPositionInterface() 
{  
    return (dynamic_cast<PositionInterface*>(this));
}

bool LayerElement::IsRest() 
{  
    return (dynamic_cast<Rest*>(this));
}
    
bool LayerElement::IsSyl()
{
    return (dynamic_cast<Syl*>(this));
}

bool LayerElement::IsTie()
{
    return (dynamic_cast<Tie*>(this));
}

bool LayerElement::IsTuplet()
{
    return (dynamic_cast<Tuplet*>(this));
}
    
bool LayerElement::IsVerse()
{
    return (dynamic_cast<Verse*>(this));
}

void LayerElement::AdjustPname( int *pname, int *oct )
{
	if ((*pname) < PITCHNAME_c)
	{
		if ((*oct) > 0)
			(*oct)-- ;
        (*pname) = PITCHNAME_b;
        
	}
	else if ((*pname) > PITCHNAME_b)
	{
		if ((*oct) < 7)
			(*oct)++;
        (*pname) = PITCHNAME_c;
	}
}

double LayerElement::GetAlignementDuration()
{
    if ( this->IsGraceNote() ) {
        return 0.0;
    }
    
    if ( HasDurationInterface() ) {
        Tuplet *tuplet = dynamic_cast<Tuplet*>( this->GetFirstParent( &typeid(Tuplet), MAX_TUPLET_DEPTH ) );
        int num = 1;
        int numbase = 1;
        if ( tuplet ) {
            num = tuplet->GetNum();
            numbase = tuplet->GetNumbase();
        }
        DurationInterface *duration = dynamic_cast<DurationInterface*>(this);
        return duration->GetAlignementDuration( num, numbase );
    }
    else {
        return 0.0;
    }
}

int LayerElement::GetXRel()
{
    if (m_alignment) {
        return m_alignment->GetXRel();
    }
    return 0;
}
    
//----------------------------------------------------------------------------
// LayerElement functors methods
//----------------------------------------------------------------------------

int LayerElement::AlignHorizontally( ArrayPtrVoid params )
{
    // param 0: the measureAligner
    // param 1: the time
    // param 2: the current scoreDef (unused)
    MeasureAligner **measureAligner = static_cast<MeasureAligner**>(params[0]);
    double *time = static_cast<double*>(params[1]);
    
    // we need to call it because we are overriding Object::AlignHorizontally
    this->ResetHorizontalAlignment();
    
    
    Chord* chordParent = dynamic_cast<Chord*>(this->GetFirstParent( &typeid( Chord ), MAX_CHORD_DEPTH));
    if( chordParent )
    {
        m_alignment = chordParent->GetAlignment();
        return FUNCTOR_CONTINUE;
    }
    
    AlignmentType type = ALIGNMENT_DEFAULT;
    if ( this->IsBarline() ) {
        type = ALIGNMENT_BARLINE;
    }
    else if ( this->IsClef() ) {
        if ( this->GetScoreOrStaffDefAttr() ) {
            type = ALIGNMENT_CLEF_ATTR;
        }
        else {
            type = ALIGNMENT_CLEF;
        }
    }
    else if ( this->IsKeySig() ) {
        if ( this->GetScoreOrStaffDefAttr() ) {
            type = ALIGNMENT_KEYSIG_ATTR;
        }
        else {
            type = ALIGNMENT_KEYSIG;
        }
    }
    else if ( this->IsMensur() ) {
        if ( this->GetScoreOrStaffDefAttr() ) {
            type = ALIGNMENT_MENSUR_ATTR;
        }
        else {
            type = ALIGNMENT_MENSUR;
        }
    }
    else if ( this->IsMeterSig() ) {
        if ( this->GetScoreOrStaffDefAttr() ) {
            type = ALIGNMENT_METERSIG_ATTR;
        }
        else {
            type = ALIGNMENT_METERSIG;
        }
    }
    else if ( this->IsMultiRest() || this->IsMRest() ) {
        type = ALIGNMENT_MULTIREST;
    }
    else if ( this->IsGraceNote() ) {
        type = ALIGNMENT_GRACENOTE;
    }
    else if ( this->IsBeam() || this->IsTuplet() || this->IsVerse() || this->IsSyl() ) {
        type = ALIGNMENT_CONTAINER;
    }
    
    // get the duration of the event
    double duration = this->GetAlignementDuration();
    
    (*measureAligner)->SetMaxTime( (*time) + duration );
    
    m_alignment = (*measureAligner)->GetAlignmentAtTime( *time, type );
    
    //LogDebug("Time %f - %s", (*time), this->GetClassName().c_str() );
    
    // increase the time position
    (*time) += duration;
    
    return FUNCTOR_CONTINUE;
}
    

int LayerElement::PrepareTimeSpanning( ArrayPtrVoid params )
{
    // param 0: the IntTree
    std::vector<DocObject*> *elements = static_cast<std::vector<DocObject*>*>(params[0]);
    
    std::vector<DocObject*>::iterator iter = elements->begin();
    while ( iter != elements->end()) {
        TimeSpanningInterface *interface = dynamic_cast<TimeSpanningInterface*>(*iter);
        assert(interface);
        if (interface->SetStartAndEnd( this ) ) {
            iter = elements->erase( iter );
        }
        else {
            iter++;
        }
    }
    
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
