/////////////////////////////////////////////////////////////////////////////
// Name:        object.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "object.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>

//----------------------------------------------------------------------------

#include "vrv.h"
#include "aligner.h"
#include "beam.h"
#include "clef.h"
#include "doc.h"
#include "keysig.h"
#include "layer.h"
#include "layerelement.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "multirest.h"
#include "page.h"
#include "view.h"
#include "scoredef.h"
#include "staff.h"
#include "system.h"
#include "tie.h"
#include "tuplet.h"
#include "verse.h"

namespace vrv {

//----------------------------------------------------------------------------
// Object
//----------------------------------------------------------------------------

Object::Object()
{
    Init("m-");
}

Object::Object(std::string classid)
{
    Init(classid);
}

Object *Object::Clone( )
{
    // This should never happen because the method should be overwritten
    assert( false );
}

Object::Object( const Object& object )
{
    ClearChildren();
    m_parent = NULL;
    m_classid = object.m_classid;
    m_uuid = object.m_uuid; // for now copy the uuid - to be decided
    m_isModified = true;
    
    int i;
    for (i = 0; i < (int)object.m_children.size(); i++)
    {
        Object *current = object.m_children[i];
        Object* copy = current->Clone();
        copy->Modify();
        copy->SetParent( this );
        m_children.push_back( copy );
    }
}

Object& Object::operator=( const Object& object )
{
	if ( this != &object ) // not self assignement
	{
        ClearChildren();
        m_parent = NULL;
        m_classid = object.m_classid;
        m_uuid = object.m_uuid; // for now copy the uuid - to be decided
        m_isModified = true;
        
        int i;
        for (i = 0; i < (int)object.m_children.size(); i++)
        {
            Object *current = object.m_children[i];
            Object* copy = current->Clone();
            copy->Modify();
            copy->SetParent( this );
            m_children.push_back( copy );
        }
	}
	return *this;
}

Object::~Object()
{
    ClearChildren();
}

void Object::Init(std::string classid)
{
    m_parent = NULL;
    m_isModified = true;
    m_classid = classid;
    this->GenerateUuid();
}

void Object::SetUuid( std::string uuid )
{ 
    m_uuid = uuid;
};

void Object::ClearChildren()
{
    ArrayOfObjects::iterator iter;
    for (iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        // we need to check if the this is the parent
        // ownership might have been given up with Relinquish
        if ((*iter)->m_parent == this) {
            delete *iter;
        }
    }
    m_children.clear();
}
 
    
int Object::GetChildCount( const std::type_info *elementType )
{
    return count_if (m_children.begin(), m_children.end(), ObjectComparison( elementType ));
}
    
    
Object* Object::GetFirst( const std::type_info *elementType )
{
    m_iteratorElementType = elementType;
    m_iteratorEnd = m_children.end();
    m_iteratorCurrent = std::find_if(m_children.begin(), m_iteratorEnd, ObjectComparison( m_iteratorElementType ) );
    return (m_iteratorCurrent == m_iteratorEnd) ? NULL : *m_iteratorCurrent;
}
    
    
Object* Object::GetNext( )
{
    m_iteratorCurrent++;
    m_iteratorCurrent = std::find_if(m_iteratorCurrent, m_iteratorEnd, ObjectComparison( m_iteratorElementType ) );
    return (m_iteratorCurrent == m_iteratorEnd) ? NULL : *m_iteratorCurrent;
}

int Object::GetIdx() const
{
    assert( m_parent );
    
    return m_parent->GetChildIndex( this );
}

void Object::InsertChild( Object *element, int idx )
{
    if ( idx >= (int)m_children.size() ) {
        m_children.push_back( element );
        return;
    }
    ArrayOfObjects::iterator iter = m_children.begin();
    m_children.insert( iter+(idx), element );
}

Object *Object::DetachChild( int idx )
{
    if ( idx >= (int)m_children.size() ) {
        return NULL;
    }
    Object *child = m_children[idx];
    child->m_parent = NULL;
    ArrayOfObjects::iterator iter = m_children.begin();
    m_children.erase( iter+(idx) );
    return child;
}

    
Object *Object::Relinquish( int idx )
{
    if ( idx >= (int)m_children.size() ) {
        return NULL;
    }
    Object *child = m_children[idx];
    child->m_parent = NULL;
    return child;
}
    
Object *Object::FindChildByUuid( std::string uuid )
{
    Functor findByUuid( &Object::FindByUuid );
    Object *element = NULL;
    ArrayPtrVoid params;
    params.push_back( &uuid );
    params.push_back( &element );
    this->Process( &findByUuid, params );
    return element;
}

Object *Object::FindChildByType(const std::type_info *elementType)
{
    Functor findByType( &Object::FindByType );
    Object *element = NULL;
    ArrayPtrVoid params;
    params.push_back( &elementType );
    params.push_back( &element );
    this->Process( &findByType, params );
    return element;
}
    
Object* Object::GetChild( int idx )
{
    if ( (idx < 0) || (idx >= (int)m_children.size()) ) {
        return NULL;
    }
    return m_children[idx];
}

void Object::RemoveChildAt( int idx )
{
    if ( idx >= (int)m_children.size() ) {
        return;
    }
    delete m_children[idx];
    ArrayOfObjects::iterator iter = m_children.begin();
    m_children.erase( iter+(idx) );
}

void Object::GenerateUuid() {
    int nr = std::rand();
    char str[17];
    // I do not want to use a stream to do this!
    snprintf (str, 16, "%016d", nr);
    
    m_uuid = m_classid + std::string(str);
}

void Object::ResetUuid()
{
    GenerateUuid();
}

void Object::SetParent( Object *parent )
{
    assert( !m_parent );
    m_parent = parent;
}


bool Object::operator==( Object& other )
{
    // This should never happen.
    // The comparison is performed in the CmpFile::Align method.
    // We expect to compare only Note, Rest, etc object for which we have an overwritten method
    LogError( "Missing comparison operator for '%s'", this->GetClassName().c_str() );
    return false;
}

int Object::GetChildIndex( const Object *child )
{
    ArrayOfObjects::iterator iter;
    int i;
    for (iter = m_children.begin(), i = 0; iter != m_children.end(); ++iter, i++)
    {
        if ( child == *iter ) {
            return i;
        }

    }
    return -1;
}

void Object::Modify( bool modified )
{    
    // if we have a parent and a new modification, propagate it
    if ( m_parent && !m_isModified && modified ) {
        m_parent->Modify();
    }
    m_isModified = modified;
}

void Object::FillList( ListOfObjects *list )
{
    Functor addToList( &Object::AddLayerElementToList );
    ArrayPtrVoid params;
    params.push_back ( &list );
    this->Process( &addToList, params );

    /* // For debuging
    ListOfObjects::iterator iter;
    for (iter = list->begin(); iter != list->end(); ++iter)
    {
        Object *current = *iter;
        LogDebug("%s", current->GetClassName().c_str() );
    }
    */
}

void Object::AddSameAs( std::string id, std::string filename )
{
    std::string sameAs = filename;
    if ( !filename.empty() ) {
        sameAs += "#";
    }
    sameAs += id;
    
    if ( !m_sameAs.empty() ) {
        m_sameAs += " ";
    }
    m_sameAs += sameAs;
}

Object *Object::GetFirstParent( const std::type_info *elementType, int maxSteps )
{
    if ( (maxSteps == 0) || !m_parent ) {
        return NULL;
    }
    
    if ( typeid(*m_parent) == *elementType ) {
        return m_parent;
    }
    else {
        return ( m_parent->GetFirstParent( elementType, maxSteps - 1 ) );
    }
}

    
Object *Object::GetLastParentNot( const std::type_info *elementType, int maxSteps )
{
    if ( (maxSteps == 0) || !m_parent ) {
        return NULL;
    }
    
    if ( typeid(*m_parent) == *elementType ) {
        return this;
    }
    else {
        return ( m_parent->GetLastParentNot( elementType, maxSteps - 1 ) );
    }
}
    

Object *Object::GetFirstChild( const std::type_info *elementType )
{
    ArrayOfObjects::iterator iter;
    int i;
    for (iter = m_children.begin(), i = 0; iter != m_children.end(); ++iter, i++)
    {
        if ( typeid(**iter) == *elementType )
        {
            return *iter;
        }
    }
    return NULL;
}

Object *Object::GetNextSibling( const std::type_info *elementType )
{
    if (!m_parent) {
        return NULL;
    }
    
    ArrayOfObjects::iterator iter;
    bool foundCurrent = false;
    for (iter = this->m_parent->m_children.begin(); iter != this->m_parent->m_children.end(); ++iter)
    {
        // we have not found the current object
        if ( this == *iter ) {
            foundCurrent = true;
            // continue to find the next sibling
            continue;
        }
        else if (!foundCurrent) {
            continue;
        }
        if ( !elementType ) {
            return *iter;
        }
        if ( typeid(**iter) == *elementType )
        {
            return *iter;
        }
    }
    return NULL;
}

Object *Object::GetPreviousSibling( const std::type_info *elementType )
{
    if (!m_parent) {
        return NULL;
    }
    
    ArrayOfObjects::reverse_iterator iter;
    bool foundCurrent = false;
    for (iter = this->m_parent->m_children.rbegin(); iter != this->m_parent->m_children.rend(); ++iter)
    {
        // we have not found the current object
        if ( this == *iter ) {
            foundCurrent = true;
            // continue to find the next sibling
            continue;
        }
        else if (!foundCurrent) {
            continue;
        }
        if ( !elementType ) {
            return *iter;
        }
        if ( typeid(**iter) == *elementType )
        {
            return *iter;
        }
    }
    return NULL;
}


bool Object::GetSameAs( std::string *id, std::string *filename, int idx )
{
    int i = 0;
    
    std::istringstream iss( m_sameAs );
    std::string token;
    while( getline( iss, token, ' '))
    {
        if ( i == idx ) {
            size_t pos = token.find( "#" );
            if (pos != std::string::npos) {
                (*filename) = token.substr( 0, pos );
                (*id) = token.substr( pos + 1, std::string::npos );
            }
            else {
                (*filename) = "";
                (*id) = token;
            }
            return true;
        }
        i++;
    }
    return false;
}

void Object::Process(Functor *functor, ArrayPtrVoid params, Functor *endFunctor, MapOfTypeN *mapOfTypeN )
{
    if (functor->m_returnCode == FUNCTOR_STOP) {
        return;
    }
    
    functor->Call( this, params );
    
    // do not go any deeper in this case
    if (functor->m_returnCode == FUNCTOR_SIBLINGS) {
        functor->m_returnCode = FUNCTOR_CONTINUE;
        return;
    }

    ArrayOfObjects::iterator iter;
    for (iter = this->m_children.begin(); iter != m_children.end(); ++iter)
    {
        // The MapOfTypeN is used to filter according to a type and an @n
        // For example, to process only staff @n="x" and @layer="y"
        if ( mapOfTypeN ) {
            MapOfTypeN::iterator typeN = mapOfTypeN->find( &typeid(**iter) );
            if(typeN != mapOfTypeN->end()) {
                AttCommon *child = dynamic_cast<AttCommon*>(*iter);
                // The filtered type must be of AttCommon because GetN called
                assert( child );
                if (child->GetN() != typeN->second) {
                    // skip it
                    continue;
                }
                else {
                    // process this one and quit
                    (*iter)->Process( functor, params, endFunctor, mapOfTypeN );
                    break;
                }
            }
        }
        (*iter)->Process( functor, params, endFunctor, mapOfTypeN );
    }
    
    if ( endFunctor ) {
        endFunctor->Call( this, params );
    }
}
    
//----------------------------------------------------------------------------
// DocObject
//----------------------------------------------------------------------------


// Note: since it is one line of code
// I am not making a new function for the two
// constructors.
DocObject::DocObject():
    Object("md-")
{
	ResetBB();
}

DocObject::DocObject(std::string classid) :
    Object(classid)
{
	//m_doc = NULL;
    ResetBB();
}

DocObject::~DocObject()
{
}

void DocObject::Refresh() 
{
    // if we have a parent DocObject, propagate it
    if ( dynamic_cast<DocObject*>(m_parent) ) {
        ((DocObject*)m_parent)->Refresh();
    }
}


void DocObject::UpdateContentBB( int x1, int y1, int x2, int y2) 
{
    //LogDebug("CB Was: %i %i %i %i", m_contentBB_x1, m_contentBB_y1, m_contentBB_x2 ,m_contentBB_y2);
    
    int min_x = std::min( x1, x2 );
    int max_x = std::max( x1, x2 );
    int min_y = std::min( y1, y2 );
    int max_y = std::max( y1, y2 );
    
    min_x -= m_drawingX;
    max_x -= m_drawingX;
    min_y -= m_drawingY;
    max_y -= m_drawingY;
    
    if (m_contentBB_x1 > min_x) m_contentBB_x1 = min_x;
    if (m_contentBB_y1 > min_y) m_contentBB_y1 = min_y;
    if (m_contentBB_x2 < max_x) m_contentBB_x2 = max_x;
    if (m_contentBB_y2 < max_y) m_contentBB_y2 = max_y;
    
    m_updatedBB = true;
    //LogDebug("CB Is:  %i %i %i %i", m_contentBB_x1,m_contentBB_y1, m_contentBB_x2, m_contentBB_y2);
}

void DocObject::UpdateSelfBB( int x1, int y1, int x2, int y2 ) 
{
    //LogDebug("SB Was: %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2 ,m_selfBB_y2);
    
    int min_x = std::min( x1, x2 );
    int max_x = std::max( x1, x2 );
    int min_y = std::min( y1, y2 );
    int max_y = std::max( y1, y2 );
    
    min_x -= m_drawingX;
    max_x -= m_drawingX;
    min_y -= m_drawingY;
    max_y -= m_drawingY;
    
    if (m_selfBB_x1 > min_x) m_selfBB_x1 = min_x;
    if (m_selfBB_y1 > min_y) m_selfBB_y1 = min_y;
    if (m_selfBB_x2 < max_x) m_selfBB_x2 = max_x;
    if (m_selfBB_y2 < max_y) m_selfBB_y2 = max_y;
    
    m_updatedBB = true;
    
    //LogDebug("SB Is:  %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2 ,m_selfBB_y2);
    
}

void DocObject::ResetBB() 
{
    m_contentBB_x1 = 0xFFFF;
    m_contentBB_y1 = 0xFFFF;
    m_contentBB_x2 = -0xFFFF;
    m_contentBB_y2 = -0xFFFF;
    m_selfBB_x1 = 0xFFFF;
    m_selfBB_y1 = 0xFFFF; 
    m_selfBB_x2 = -0xFFFF;
    m_selfBB_y2 = -0xFFFF;
    //m_drawingX = 0;
    //m_drawingY = 0;
    
    m_updatedBB = false;
}

bool DocObject::HasContentBB() 
{
    return ( (m_contentBB_x1 != 0xFFFF) && (m_contentBB_y1 != 0xFFFF) && (m_contentBB_x2 != -0xFFFF) && (m_contentBB_y2 != -0xFFFF) );
}

bool DocObject::HasSelfBB() 
{
    return ( (m_selfBB_x1 != 0xFFFF) && (m_selfBB_y1 != 0xFFFF) && (m_selfBB_x2 != -0xFFFF) && (m_selfBB_y2 != -0xFFFF) );
}


//----------------------------------------------------------------------------
// ObjectListInterface
//----------------------------------------------------------------------------


ObjectListInterface::ObjectListInterface( const ObjectListInterface& interface )
{
    // actually nothing to do, we just don't want the list to be copied
    m_list.clear();
}

ObjectListInterface& ObjectListInterface::operator=( const ObjectListInterface& interface )
{
    // actually nothing to do, we just don't want the list to be copied
    if ( this != &interface ) {
        this->m_list.clear();
    }
	return *this;
}

void ObjectListInterface::ResetList( Object *node )
{
    // nothing to do, the list if up to date
    if ( !node->IsModified() ) {
        return;
    }
    
    m_list.clear();
    node->FillList( &m_list );
    this->FilterList();
    node->Modify( false );
}

ListOfObjects *ObjectListInterface::GetList( Object *node )
{   
    ResetList( node );
    return &m_list;
}


int ObjectListInterface::GetListIndex( const Object *listElement )
{
    ListOfObjects::iterator iter;
    int i;
    for (iter = m_list.begin(), i = 0; iter != m_list.end(); ++iter, i++)
    {
        if ( listElement == *iter ) {
            return i;
        }
        
    }
    return -1;
}

Object *ObjectListInterface::GetListPrevious( const Object *listElement )
{
    ListOfObjects::iterator iter;
    int i;
    for (iter = m_list.begin(), i = 0; iter != m_list.end(); ++iter, i++)
    {
        if (listElement == *iter) {
            if (i > 0) {
                return *(--iter);
            }
            else {
                return NULL;
            }
        }
        
    }
    return NULL;
}

Object *ObjectListInterface::GetListNext( const Object *listElement )
{
    ListOfObjects::reverse_iterator iter;
    int i;
    for (iter = m_list.rbegin(), i = 0; iter != m_list.rend(); ++iter, i++)
    {
        if ( listElement == *iter ) {
            if (i > 0) {
                return *(--iter);
            }
            else {
                return NULL;
            }
        }
        
    }
    return NULL;
}

//----------------------------------------------------------------------------
// Functor
//----------------------------------------------------------------------------

Functor::Functor( )
{ 
    m_returnCode = FUNCTOR_CONTINUE;
    m_reverse = false;
    obj_fpt = NULL; 
}

Functor::Functor( int(Object::*_obj_fpt)( ArrayPtrVoid ))
{ 
    m_returnCode = FUNCTOR_CONTINUE;
    m_reverse = false;
    obj_fpt = _obj_fpt; 
}

void Functor::Call( Object *ptr, ArrayPtrVoid params )
{
    // we should have return codes (not just bool) for avoiding to go further down the tree in some cases
    m_returnCode = (*ptr.*obj_fpt)( params );
}

//----------------------------------------------------------------------------
// Object functor methods
//----------------------------------------------------------------------------

int Object::AddLayerElementToList( ArrayPtrVoid params )
{
    // param 0: the ListOfObjects
    ListOfObjects **list = static_cast<ListOfObjects**>(params[0]);
    //if ( dynamic_cast<LayerElement*>(this ) ) {
        (*list)->push_back( this );
    //}
    return FUNCTOR_CONTINUE;
}

int Object::FindByUuid( ArrayPtrVoid params )
{
    // param 0: the uuid we are looking for
    // param 1: the pointer to pointer to the Object
    std::string *uuid = static_cast<std::string*>(params[0]);
    Object **element = static_cast<Object**>(params[1]);
    
    if ( (*element) ) {
        // this should not happen, but just in case
        return FUNCTOR_STOP;
    }
    
    if ( (*uuid) == this->GetUuid() ) {
        (*element) = this;
        //LogDebug("Found it!");
        return FUNCTOR_STOP;
    }
    //LogDebug("Still looking for uuid...");
    return FUNCTOR_CONTINUE;
}
    
int Object::FindByType( ArrayPtrVoid params )
{
    // param 0: the type we are looking for
    // param 1: the pointer to pointer to the Object
    const std::type_info **elementType = static_cast<const std::type_info**>(params[0]);
    Object **element = static_cast<Object**>(params[1]);
    
    if ( (*element) ) {
        // this should not happen, but just in case
        return FUNCTOR_STOP;
    }
    
    if ( typeid(*this) == **elementType ) {
        (*element) = this;
        //LogDebug("Found it!");
        return FUNCTOR_STOP;
    }
    //LogDebug("Still looking for uuid...");
    return FUNCTOR_CONTINUE;
}
    
int Object::SetCurrentScoreDef( ArrayPtrVoid params )
{

    // param 0: the current scoreDef
    ScoreDef *currentScoreDef = static_cast<ScoreDef*>(params[0]);
    StaffDef **currentStaffDef = static_cast<StaffDef**>(params[1]);

    assert( currentScoreDef );
    
    // starting a new page
    Page *current_page = dynamic_cast<Page*>(this);
    if ( current_page  ) {
        if ( current_page->m_parent->GetChildIndex( current_page ) == 0 ) {
            currentScoreDef->SetRedrawFlags( true, true, true, true );
        }
        else {
            currentScoreDef->SetRedrawFlags( true, true, false, false );
        }
        current_page->m_drawingScoreDef = *currentScoreDef;
        return FUNCTOR_CONTINUE;
    }

    // starting a new system
    System *current_system = dynamic_cast<System*>(this);
    if ( current_system  ) {
        currentScoreDef->SetRedrawFlags( true, true, false, false );
        return FUNCTOR_CONTINUE;
    }
    
    // starting a new scoreDef
    ScoreDef *current_scoreDef= dynamic_cast<ScoreDef*>(this);
    if ( current_scoreDef  ) {
        bool drawClef = false;
        bool drawKeySig = false;
        bool drawMensur = false;
        bool drawMeterSig = false;
        if (current_scoreDef->GetClef()) {
            currentScoreDef->ReplaceClef(current_scoreDef->GetClef());
            drawClef = true;
        }
        if (current_scoreDef->GetKeySig()) {
            currentScoreDef->ReplaceKeySig(current_scoreDef->GetKeySig());
            drawKeySig = true;
        }
        if (current_scoreDef->GetMensur()) {
            currentScoreDef->ReplaceMensur(current_scoreDef->GetMensur());
            drawMensur = true;
        }
        if (current_scoreDef->GetMeterSig()) {
            currentScoreDef->ReplaceMeterSig(current_scoreDef->GetMeterSig());
            drawMeterSig = true;
        }
        currentScoreDef->SetRedrawFlags( drawClef, drawKeySig, drawMensur, drawMeterSig );
        return FUNCTOR_CONTINUE;
    }

    // starting a new staff
    Staff *current_staff = dynamic_cast<Staff*>(this);
    if ( current_staff  ) {
        (*currentStaffDef) = currentScoreDef->GetStaffDef( current_staff->GetN() );
        return FUNCTOR_CONTINUE;
    }

    // starting a new layer
    Layer *current_layer = dynamic_cast<Layer*>(this);
    if ( current_layer  ) {
        // setting the layer stem direction. Alternatively, this could be done in
        // View::DrawLayer. If this (and other things) is kept here, renaming the method to something more
        // generic (PrepareDrawing?) might be a good idea...
        if (current_layer->m_parent->GetChildCount() > 1) {
            if (current_layer->m_parent->GetChildIndex(current_layer)==0) {
                current_layer->SetDrawingStemDir(STEMDIRECTION_up);
            }
            else {
                current_layer->SetDrawingStemDir(STEMDIRECTION_down);
            }
        }
        current_layer->SetDrawingValues( currentScoreDef, (*currentStaffDef) );
        return FUNCTOR_CONTINUE;
    }
    
    // starting a new clef
    Clef *current_clef = dynamic_cast<Clef*>(this);
    if ( current_clef  ) {
        assert( *currentStaffDef );
        (*currentStaffDef)->ReplaceClef( current_clef );
        return FUNCTOR_CONTINUE;
    }
    
    // starting a new keysig
    KeySig *current_keysig = dynamic_cast<KeySig*>(this);
    if ( current_keysig  ) {
        assert( *currentStaffDef );
        (*currentStaffDef)->ReplaceKeySig( current_keysig );
        return FUNCTOR_CONTINUE;
    }
    
    return FUNCTOR_CONTINUE;
}
    
int Object::AlignHorizontally( ArrayPtrVoid params )
{
    // param 0: the measureAligner (unused)
    // param 1: the time (unused)
        
    // reset all the drawing values - this also need to be called
    // from any functor overriding this one!
    this->ResetHorizontalAlignment();
    
    return FUNCTOR_CONTINUE;
}
    
int Object::AlignVertically( ArrayPtrVoid params )
{
    // param 0: the systemAligner (unused)
    // param 1: the staffNb (unused
    
    // reset all the drawing values - this also need to be called
    // from any functor overriding this one!
    this->ResetVerticalAlignment();
    
    return FUNCTOR_CONTINUE;
}

int Object::SetBoundingBoxXShift( ArrayPtrVoid params )
{
    // param 0: the minimu position (i.e., the width of the previous element)
    // param 1: the maximum width in the current measure
    int *min_pos = static_cast<int*>(params[0]);
    int *measure_width = static_cast<int*>(params[1]);

    // starting a new measure
    Measure *current_measure = dynamic_cast<Measure*>(this);
    if ( current_measure  ) {
        // we reset the measure width and the minimum position
        (*measure_width) = 0;
        (*min_pos) = 0;
        if (current_measure->GetLeftBarlineType() != BARRENDITION_NONE) {
            current_measure->GetLeftBarline()->SetBoundingBoxXShift( params );
        }
        return FUNCTOR_CONTINUE;
    }
    
    // starting an new layer
    Layer *current_layer = dynamic_cast<Layer*>(this);
    if ( current_layer  ) {
        // reset it as the minimum position to the step (if doc found)
        (*min_pos) = 0;
        Doc *doc = dynamic_cast<Doc*>( current_layer->GetFirstParent( &typeid(Doc) ) );
        if (doc) (*min_pos) = doc->m_drawingStep1;
        // set scoreDef attr
        if (current_layer->GetDrawingClef()) {
            current_layer->GetDrawingClef()->SetBoundingBoxXShift( params );
        }
        if (current_layer->GetDrawingKeySig()) {
            current_layer->GetDrawingKeySig()->SetBoundingBoxXShift( params );
        }
        if (current_layer->GetDrawingMensur()) {
            current_layer->GetDrawingMensur()->SetBoundingBoxXShift( params );
        }
        if (current_layer->GetDrawingMeterSig()) {
            current_layer->GetDrawingMeterSig()->SetBoundingBoxXShift( params );
        }
        return FUNCTOR_CONTINUE;
    }

    LayerElement *current = dynamic_cast<LayerElement*>(this);
    if ( !current  ) {
        return FUNCTOR_CONTINUE;
    }
    
    // we should have processed aligned before
    assert( current->GetAlignment() );

    if ( !current->HasUpdatedBB() ) {
        // this is all we need for empty elements
        current->GetAlignment()->SetXRel(*min_pos);
        return FUNCTOR_CONTINUE;
    }
    
    if ( current->IsBeam() ) {
        return FUNCTOR_CONTINUE;
    }
    
    if ( current->IsTie() ) {
        return FUNCTOR_CONTINUE;
    }
    
    if ( current->IsTuplet() ) {
        return FUNCTOR_CONTINUE;
    }
    
    if ( current->IsMRest() ) {
        // We need to reconsider this: if the mrest is on the top staff, the aligner will be before any other note
        // aligner. This means that it will not be shifted. We need to shift it but not take into account its own width.
        //current->GetAlignment()->SetXShift( current->GetAlignment()->GetXRel() );
        return FUNCTOR_CONTINUE;
    }
    
    // the negative offset it the part of the bounding box that overflows on the left
    // |____x_____|
    //  ---- = negative offset
    //int negative_offset = current->GetAlignment()->GetXRel() - current->m_contentBB_x1;
    int negative_offset = - current->m_contentBB_x1;
    
    // this will probably never happen
    if ( negative_offset < 0 ) {
        LogDebug("%s negative offset %d;", current->GetClassName().c_str(), negative_offset );
        negative_offset = 0;
    }
    
    // check if the element overlaps with the preceeding one given by (*min_pos)
    int overlap = 0;
    overlap = (*min_pos) - current->GetAlignment()->GetXRel() + negative_offset;
    if ( (current->GetAlignment()->GetXRel() - negative_offset) < (*min_pos) ) {
        overlap = (*min_pos) - current->GetAlignment()->GetXRel() + negative_offset;
        // shift the current element
        current->GetAlignment()->SetXShift( overlap );
    }
    
    //LogDebug("%s min_pos %d; negative offset %d;  drawXRel %d; overlap %d; m_drawingX %d", current->GetClassName().c_str(), (*min_pos), negative_offset, current->GetAlignment()->GetXRel(), overlap, current->GetDrawingX() );
    
    // the next minimal position if given by the right side of the bounding box + the spacing of the element
    //(*min_pos) = current->m_contentBB_x2 + current->GetHorizontalSpacing();
    //current->GetAlignment()->SetMaxWidth( current->m_contentBB_x2 - current->GetAlignment()->GetXRel() + current->GetHorizontalSpacing() );
    (*min_pos) = current->GetAlignment()->GetXRel() + current->m_contentBB_x2 + current->GetHorizontalSpacing();
    current->GetAlignment()->SetMaxWidth( current->m_contentBB_x2 + current->GetHorizontalSpacing() );
    
    return FUNCTOR_CONTINUE;
}

int Object::SetBoundingBoxXShiftEnd( ArrayPtrVoid params )
{
    // param 0: the minimu position (i.e., the width of the previous element)
    // param 1: the maximum width in the current measure
    int *min_pos = static_cast<int*>(params[0]);
    int *measure_width = static_cast<int*>(params[1]);
    
    // ending a measure
    Measure *current_measure = dynamic_cast<Measure*>(this);
    if ( current_measure  ) {
        // as minimum position of the barline use the measure width
        (*min_pos) = (*measure_width);
        if (current_measure->GetRightBarlineType() != BARRENDITION_NONE) {
            current_measure->GetRightBarline()->SetBoundingBoxXShift( params );
        }
        return FUNCTOR_CONTINUE;
    }
    
    // ending a layer
    Layer *current_layer = dynamic_cast<Layer*>(this);
    if ( current_layer  ) {
        // mininimum position is the with the layer
        // we keep it if is higher than what we had so far
        // this will be used for shifting the right barline
        (*measure_width) = std::max( (*measure_width), (*min_pos) );
        return FUNCTOR_CONTINUE;
    }
    
    return FUNCTOR_CONTINUE;
}

int Object::SetBoundingBoxYShift( ArrayPtrVoid params )
{
    // param 0: the position of the previous staff
    // param 1: the maximum height in the current system
    int *min_pos = static_cast<int*>(params[0]);
    int *system_height = static_cast<int*>(params[1]);
    
    // starting a new system
    System *current_system = dynamic_cast<System*>(this);
    if ( current_system  ) {
        // we reset the system height
        (*system_height) = 0;
        (*min_pos) = 0;
        return FUNCTOR_CONTINUE;
    }
    
    // starting a new measure
    Measure *current_measure = dynamic_cast<Measure*>(this);
    if ( current_measure  ) {
        (*min_pos) = 0;
        return FUNCTOR_CONTINUE;
    }
    
    // starting a new staff
    Staff *current = dynamic_cast<Staff*>(this);
    if ( !current  ) {
        return FUNCTOR_CONTINUE;
    }
    
    // at this stage we assume we have instanciated the alignment pointer
    assert( current->GetAlignment() );
    
    // This is the value that need to be removed to fit everything
    int negative_offset = - current->m_contentBB_y2;
    
    // this will probably never happen
    if ( negative_offset > 0 ) {
        negative_offset = 0;
    }
    
    // check if the staff overlaps with the preceeding one given by (*min_pos)
    int overlap = 0;
    if ( (current->GetAlignment()->GetYRel() - negative_offset) > (*min_pos) ) {
        overlap = (*min_pos) - current->GetAlignment()->GetYRel() + negative_offset;
        current->GetAlignment()->SetYShift( overlap );
    }
    
    //LogDebug("%s min_pos %d; negative offset %d;  drawXRel %d; overlap %d", current->GetClassName().c_str(), (*min_pos), negative_offset, current->GetAlignment()->GetXRel(), overlap );
    
    // the next minimal position if given by the bottom side of the bounding box + the spacing of the element
    (*min_pos) = current->m_contentBB_y1;
    current->GetAlignment()->SetMaxHeight( current->m_contentBB_y1 );
    
    // do not go further down the tree in this case
    return FUNCTOR_SIBLINGS;
}
    
int Object::SetBoundingBoxYShiftEnd( ArrayPtrVoid params )
{
    // param 0: the position of the previous staff
    // param 1: the maximum height in the current system
    int *min_pos = static_cast<int*>(params[0]);
    int *system_height = static_cast<int*>(params[1]);
    
    // ending a measure
    Measure *current_measure = dynamic_cast<Measure*>(this);
    if ( current_measure  ) {
        // mininimum position is the height for the last (previous) staff
        // we keep it if it is higher than what we had so far
        (*system_height) = std::min( (*system_height), (*min_pos) );
        return FUNCTOR_CONTINUE;
    }
    
    // ending a system: see System::SetBoundingBoxYShiftEnd

    return FUNCTOR_CONTINUE;
}

} // namespace vrv



