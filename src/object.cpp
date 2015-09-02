/////////////////////////////////////////////////////////////////////////////
// Name:        object.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------

#include <assert.h>
#include <sstream>
#include <iostream>

//----------------------------------------------------------------------------

#include "chord.h"
#include "doc.h"
#include "editorial.h"
#include "io.h"
#include "keysig.h"
#include "layer.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "note.h"
#include "page.h"
#include "staff.h"
#include "system.h"
#include "vrv.h"

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
    return NULL;
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
        Object *current = object.m_children.at(i);
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
            Object *current = object.m_children.at(i);
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
    m_doc = NULL;
    m_classid = classid;
    this->GenerateUuid();
}
    
ClassId Object::Is()
{
    // we should always have the method overwritten
    assert( false );
    return OBJECT;
};
    
void Object::RegisterInterface( std::vector<AttClassId> *attClasses, InterfaceId interfaceId )
{
    m_attClasses.insert(m_attClasses.end(), attClasses->begin(), attClasses->end());
    m_interfaces.push_back(interfaceId);
}
       
void Object::MoveChildren(  Object *object )
{
    if ( this == object ){
        assert( "Object cannot be copied to itself");
    }
    if ( &typeid(*this) != &typeid(*object) ) {
        assert( "Object must be of the same type");
    }
    
    int i;
    for (i = 0; i < (int)object->m_children.size(); i++)
    {
        this->m_children.push_back( object->Relinquish(i) );
        object->m_children.at(i)->m_parent = this;
    }
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
 
    
int Object::GetChildCount( const ClassId classId )
{
    return (int)count_if (m_children.begin(), m_children.end(), ObjectComparison( classId ));
}

int Object::GetAttributes(ArrayOfStrAttr *attributes)
{
    assert(attributes);
    attributes->clear();
    
    Att::GetCmn(this, attributes );
    Att::GetMensural(this, attributes );
    Att::GetPagebased(this, attributes );
    Att::GetShared(this, attributes );
    
    return (int)attributes->size();
}
    
bool Object::HasAttribute( std::string attribute, std::string value )
{
    ArrayOfStrAttr attributes;
    this->GetAttributes( &attributes );
    ArrayOfStrAttr::iterator iter;
    for (iter = attributes.begin(); iter != attributes.end(); iter++) {
        if ( ( (*iter).first == attribute ) && ( (*iter).second == value ) ) return true;
    }
    return false;
}
    
        
Object* Object::GetFirst( const ClassId classId )
{
    m_iteratorElementType = classId;
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
    // With this method we require the parent to be set before
    assert( element->m_parent == this );
    
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
    Object *child = m_children.at(idx);
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
    Object *child = m_children.at(idx);
    child->m_parent = NULL;
    return child;
}
    
Object *Object::FindChildByUuid( std::string uuid, int deepness, bool direction )
{
    Functor findByUuid( &Object::FindByUuid );
    Object *element = NULL;
    ArrayPtrVoid params;
    params.push_back( &uuid );
    params.push_back( &element );
    this->Process( &findByUuid, &params, NULL, NULL, deepness, direction );
    return element;
}

Object *Object::FindChildByType( ClassId classId, int deepness, bool direction )
{
    AttComparison attComparison( classId );
    return FindChildByAttComparison( &attComparison, deepness, direction );
}
    
Object *Object::FindChildByAttComparison( AttComparison *attComparison, int deepness, bool direction )
{
    Functor findByAttComparison( &Object::FindByAttComparison );
    Object *element = NULL;
    ArrayPtrVoid params;
    params.push_back( attComparison );
    params.push_back( &element );
    this->Process( &findByAttComparison, &params, NULL, NULL, deepness, direction );
    return element;
}
    
Object* Object::GetChild( int idx )
{
    if ( (idx < 0) || (idx >= (int)m_children.size()) ) {
        return NULL;
    }
    return m_children.at(idx);
}

void Object::RemoveChildAt( int idx )
{
    if ( idx >= (int)m_children.size() ) {
        return;
    }
    delete m_children.at(idx);
    ArrayOfObjects::iterator iter = m_children.begin();
    m_children.erase( iter+(idx) );
}

void Object::GenerateUuid()
{
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
    
void Object::AddEditorialElement( EditorialElement *child )
{
    assert(
           dynamic_cast<System*>(this)
        || dynamic_cast<Measure*>(this)
        || dynamic_cast<Staff*>(this)
        || dynamic_cast<Layer*>(this)
        || dynamic_cast<LayerElement*>(this)
        || dynamic_cast<Note*>(this)
        || dynamic_cast<Lem*>(this)
        || dynamic_cast<Rdg*>(this)
           );
    child->SetParent( this );
    m_children.push_back( child );
    Modify();
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

void Object::FillFlatList( ListOfObjects *flatList )
{
    Functor addToFlatList( &Object::AddLayerElementToFlatList );
    ArrayPtrVoid params;
    params.push_back ( &flatList );
    this->Process( &addToFlatList, &params );

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

Object *Object::GetFirstParent( const ClassId classId, int maxSteps )
{
    if ( (maxSteps == 0) || !m_parent ) {
        return NULL;
    }
    
    if ( m_parent->Is() == classId ) {
        return m_parent;
    }
    else {
        return ( m_parent->GetFirstParent( classId, maxSteps - 1 ) );
    }
}

    
Object *Object::GetLastParentNot( const ClassId classId, int maxSteps )
{
    if ( (maxSteps == 0) || !m_parent ) {
        return NULL;
    }
    
    if ( m_parent->Is() == classId ) {
        return this;
    }
    else {
        return ( m_parent->GetLastParentNot( classId, maxSteps - 1 ) );
    }
}
    

Object *Object::GetFirstChild( const ClassId classId )
{
    ArrayOfObjects::iterator iter;
    int i;
    for (iter = m_children.begin(), i = 0; iter != m_children.end(); ++iter, i++)
    {
        Object *o = *iter;
        if ( o->Is() == classId )
        {
            return *iter;
        }
    }
    return NULL;
}

Object *Object::GetNextSibling( const ClassId classId )
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
        if ( classId == UNSPECIFIED ) {
            return *iter;
        }
        if ( (*iter)->Is() == classId )
        {
            return *iter;
        }
    }
    return NULL;
}

Object *Object::GetPreviousSibling( const ClassId classId )
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
        if ( classId == UNSPECIFIED ) {
            return *iter;
        }
        if ( (*iter)->Is() == classId )
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

void Object::Process(Functor *functor, ArrayPtrVoid *params, Functor *endFunctor, ArrayOfAttComparisons *filters, int deepness, bool direction )
{
    if (functor->m_returnCode == FUNCTOR_STOP) {
        return;
    }
    
    if (functor->m_visibleOnly && this->IsEditorialElement()) {
        EditorialElement *editorialElement = dynamic_cast<EditorialElement*>(this);
        assert( editorialElement );
        if ( editorialElement->m_visibility == Hidden ) {
            return;
        }
    }
    
    functor->Call( this, params );
    
    // do not go any deeper in this case
    if (functor->m_returnCode == FUNCTOR_SIBLINGS) {
        functor->m_returnCode = FUNCTOR_CONTINUE;
        return;
    }
    else if (this->IsEditorialElement()) {
        // since editorial object do not count, we re-increase the deepness limit
        deepness++;
    }
    if (deepness == 0) {
        // any need to change the functor m_returnCode?
        return;
    }
    deepness--;
    
    ArrayOfObjects::iterator iter;
    // We need a pointer to the array for the option to work on a reversed copy
    ArrayOfObjects *children = &this->m_children;
    ArrayOfObjects reversed;
    // For processing backward, we operated on a copied reversed version
    // Since we hold pointers, only addresses are copied
    if ( direction == BACKWARD ) {
        reversed = (*children);
        std::reverse( reversed.begin(), reversed.end() );
        children = &reversed;
    }
    for (iter = children->begin(); iter != children->end(); ++iter)
    {
        if ( filters && !filters->empty() ) {
            bool hasAttComparison = false;
            // first we look if there is a comparison object for the object type (e.g., a Staff)
            ArrayOfAttComparisons::iterator attComparisonIter;
            for (attComparisonIter = filters->begin(); attComparisonIter != filters->end(); attComparisonIter++) {
                // if yes, we will use it (*attComparisonIter) for evaluating if the object matches
                // the attribute (see below)
                Object *o = *iter;
                if (o->Is() == (*attComparisonIter)->GetType() ) {
                    hasAttComparison = true;
                    break;
                }
            }
            if (hasAttComparison) {
                // use the operator of the AttComparison object to evaluate the attribute
                if ((**attComparisonIter)(*iter)) {
                    // the attribute value matches, process the object
                    //LogDebug("%s ", (*iter)->GetClassName().c_str() );
                    (*iter)->Process( functor, params, endFunctor, filters, deepness, direction );
                    break;
                }
                else {
                    // the attribute value does not match, skip this child
                    continue;
                }
            }
        }
        // we will end here if there is no filter at all or for the current child type
        (*iter)->Process( functor, params, endFunctor, filters, deepness, direction );
    }
    
    if ( endFunctor ) {
        endFunctor->Call( this, params );
    }
}
    
int Object::Save( FileOutputStream *output )
{
    ArrayPtrVoid params;
    params.push_back( output );
    
    Functor save( &Object::Save );
    // Special case where we want to process all objects
    save.m_visibleOnly = false;
    Functor saveEnd( &Object::SaveEnd );
    this->Process( &save, &params, &saveEnd );
    
    return true;
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
    //LogDebug("CB Is:  %i %i %i %i %s", m_contentBB_x1,m_contentBB_y1, m_contentBB_x2, m_contentBB_y2, GetClassName().c_str());
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
    m_contentBB_x1 = 0xFFFFFFF;
    m_contentBB_y1 = 0xFFFFFFF;
    m_contentBB_x2 = -0xFFFFFFF;
    m_contentBB_y2 = -0xFFFFFFF;
    m_selfBB_x1 = 0xFFFFFFF;
    m_selfBB_y1 = 0xFFFFFFF;
    m_selfBB_x2 = -0xFFFFFFF;
    m_selfBB_y2 = -0xFFFFFFF;
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
    
    node->Modify( false );
    m_list.clear();
    node->FillFlatList( &m_list );
    this->FilterList( &m_list );
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

    
Object* ObjectListInterface::GetListFirst(const Object *startFrom, const ClassId classId)
{
    ListOfObjects::iterator it = m_list.begin();
    std::advance(it, GetListIndex(startFrom));
    it = std::find_if(it, m_list.end(), ObjectComparison( classId ) );
    return (it == m_list.end()) ? NULL : *it;
}
    
Object* ObjectListInterface::GetListFirstBackward(Object *startFrom, const ClassId classId)
{
    ListOfObjects::iterator it = m_list.begin();
    std::advance(it, GetListIndex(startFrom));
    ListOfObjects::reverse_iterator rit(it);
    rit = std::find_if(rit, m_list.rend(), ObjectComparison( classId ) );
    return (rit == m_list.rend()) ? NULL : *rit;
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
    m_visibleOnly = true;
    obj_fpt = NULL; 
}

Functor::Functor( int(Object::*_obj_fpt)( ArrayPtrVoid* ))
{ 
    m_returnCode = FUNCTOR_CONTINUE;
    m_visibleOnly = true;
    obj_fpt = _obj_fpt; 
}

void Functor::Call( Object *ptr, ArrayPtrVoid *params )
{
    // we should have return codes (not just bool) for avoiding to go further down the tree in some cases
    m_returnCode = (*ptr.*obj_fpt)( params );
}

//----------------------------------------------------------------------------
// Object functor methods
//----------------------------------------------------------------------------

int Object::AddLayerElementToFlatList( ArrayPtrVoid *params )
{
    // param 0: the ListOfObjects
    ListOfObjects **list = static_cast<ListOfObjects**>((*params).at(0));
    //if ( dynamic_cast<LayerElement*>(this ) ) {
        (*list)->push_back( this );
    //}
    return FUNCTOR_CONTINUE;
}

int Object::FindByUuid( ArrayPtrVoid *params )
{
    // param 0: the uuid we are looking for
    // param 1: the pointer to pointer to the Object
    std::string *uuid = static_cast<std::string*>((*params).at(0));
    Object **element = static_cast<Object**>((*params).at(1));
    
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

int Object::FindByAttComparison( ArrayPtrVoid *params )
{
    // param 0: the type we are looking for
    // param 1: the pointer to pointer to the Object
    AttComparison *test = static_cast<AttComparison*>((*params).at(0));
    Object **element = static_cast<Object**>((*params).at(1));
    
    if ( (*element) ) {
        // this should not happen, but just in case
        return FUNCTOR_STOP;
    }
    
    // evaluate by applying the AttComparison operator()
    if ((*test)(this)) {
        (*element) = this;
        //LogDebug("Found it!");
        return FUNCTOR_STOP;
    }
    //LogDebug("Still looking for the object matching the AttComparison...");
    return FUNCTOR_CONTINUE;
}

    
int Object::SetCurrentScoreDef( ArrayPtrVoid *params )
{

    // param 0: the current scoreDef
    ScoreDef *currentScoreDef = static_cast<ScoreDef*>((*params).at(0));
    StaffDef **currentStaffDef = static_cast<StaffDef**>((*params).at(1));

    assert( currentScoreDef );
    
    // starting a new page
    if (this->Is() == PAGE) {
        // The keySig cancellation is set to false, which means that a scoreDef change has to occur
        // after a page break if right at the begining. This is the same for systems below
        Page *page = dynamic_cast<Page*>(this);
        assert( page );
        if ( page->m_parent->GetChildIndex( page ) == 0 ) {
            currentScoreDef->SetRedrawFlags( true, true, true, true, false );
            currentScoreDef->SetDrawLabels( true );
        }
        else {
            currentScoreDef->SetRedrawFlags( true, true, false, false, false );
            currentScoreDef->SetDrawLabels( false );
        }
        page->m_drawingScoreDef = *currentScoreDef;
        return FUNCTOR_CONTINUE;
    }

    // starting a new system
    if (this->Is() == SYSTEM) {
        System *system = dynamic_cast<System*>(this);
        assert( system );
        currentScoreDef->SetRedrawFlags( true, true, false, false, false );
        return FUNCTOR_CONTINUE;
    }
    
    // starting a new scoreDef
    if (this->Is() == SCORE_DEF) {
        ScoreDef *scoreDef= dynamic_cast<ScoreDef*>(this);
        assert( scoreDef );
        // Replace the current scoreDef with the new one, including its content (staffDef)
        currentScoreDef->ReplaceDrawingValues(scoreDef);
        return FUNCTOR_CONTINUE;
    }

    // starting a new staffDef
    if (this->Is() == STAFF_DEF) {
        StaffDef *staffDef= dynamic_cast<StaffDef*>(this);
        assert( staffDef );
        currentScoreDef->ReplaceDrawingValues(staffDef);
    }
    
    // starting a new staff
    if (this->Is() == STAFF) {
        Staff *staff = dynamic_cast<Staff*>(this);
        assert( staff );
        (*currentStaffDef) = currentScoreDef->GetStaffDef( staff->GetN() );
        return FUNCTOR_CONTINUE;
    }

    // starting a new layer
    if (this->Is() == LAYER) {
        Layer *layer = dynamic_cast<Layer*>(this);
        assert( layer );
        // setting the layer stem direction. Alternatively, this could be done in
        // View::DrawLayer. If this (and other things) is kept here, renaming the method to something more
        // generic (PrepareDrawing?) might be a good idea...
        if (layer->m_parent->GetChildCount() > 1) {
            if (layer->m_parent->GetChildIndex(layer)==0) {
                layer->SetDrawingStemDir(STEMDIRECTION_up);
            }
            else {
                layer->SetDrawingStemDir(STEMDIRECTION_down);
            }
        }
        layer->SetDrawingAndCurrentValues( (*currentStaffDef) );
        return FUNCTOR_CONTINUE;
    }
    
    // starting a new clef
    if (this->Is() == CLEF) {
        Clef *clef = dynamic_cast<Clef*>(this);
        assert( clef );
        assert( *currentStaffDef );
        (*currentStaffDef)->SetCurrentClef( new Clef( *clef ) );
        return FUNCTOR_CONTINUE;
    }
    
    // starting a new keysig
    if (this->Is() == KEY_SIG) {
        KeySig *keysig = dynamic_cast<KeySig*>(this);
        assert( keysig );
        assert( *currentStaffDef );
        (*currentStaffDef)->SetCurrentKeySig( new KeySig( *keysig ) );
        return FUNCTOR_CONTINUE;
    }
    
    return FUNCTOR_CONTINUE;
}
    
int Object::AlignHorizontally( ArrayPtrVoid *params )
{
    // param 0: the measureAligner (unused)
    // param 1: the time (unused)
    // param 2: the current Mensur (unused)
    // param 3: the current MeterSig (unused)
        
    // reset all the drawing values - this also need to be called
    // from any functor overriding this one!
    this->ResetHorizontalAlignment();
    
    return FUNCTOR_CONTINUE;
}
    
int Object::AlignVertically( ArrayPtrVoid *params )
{
    // param 0: the systemAligner (unused)
    // param 1: the staffNb (unused
    
    // reset all the drawing values - this also need to be called
    // from any functor overriding this one!
    this->ResetVerticalAlignment();
    
    return FUNCTOR_CONTINUE;
}
    
int Object::SetBoundingBoxGraceXShift( ArrayPtrVoid *params )
{
    // param 0: the minimu position (i.e., the width of the previous element)
    // param 1: the Doc
    int *min_pos = static_cast<int*>((*params).at(0));
    Doc *doc = static_cast<Doc*>((*params).at(1));
    
    // starting an new layer
    if (this->Is() == LAYER) {
        (*min_pos) = 0;
        return FUNCTOR_CONTINUE;
    }
    
    if (this->Is() != NOTE) {
        return FUNCTOR_CONTINUE;
    }
    
    Note *note = dynamic_cast<Note*>(this);
    assert( note );
    
    if (!note->IsGraceNote()) {
        (*min_pos) = 0;
        return FUNCTOR_CONTINUE;
    }
    
    // we should have processed aligned before
    assert( note->GetGraceAlignment() );
    
    // the negative offset it the part of the bounding box that overflows on the left
    // |____x_____|
    //  ---- = negative offset
    //int negative_offset = - (note->m_contentBB_x1) + (doc->GetLeftMargin(&typeid(*note)) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR);
    int negative_offset = - note->m_contentBB_x1;
    if ( (*min_pos) > 0 ) {
        //(*min_pos) += (doc->GetLeftMargin(&typeid(*note)) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR);
    }
    
    // this should never happen (but can with glyphs not exactly registered at position x=0 in the SMuFL font used
    if ( negative_offset < 0 ) negative_offset = 0;
    
    // check if the element overlaps with the preceeding one given by (*min_pos)
    int overlap = (*min_pos) - note->GetGraceAlignment()->GetXRel() + negative_offset;
    
    if ( (note->GetGraceAlignment()->GetXRel() - negative_offset) < (*min_pos) ) {
        note->GetGraceAlignment()->SetXShift( overlap );
    }
    
    // the next minimal position if given by the right side of the bounding box + the spacing of the element
    (*min_pos) = note->GetGraceAlignment()->GetXRel() + note->m_contentBB_x2 + doc->GetRightMargin( NOTE ) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR;
    //(*min_pos) = note->GetGraceAlignment()->GetXRel() + note->m_contentBB_x2;
    //note->GetGraceAlignment()->SetMaxWidth( note->m_contentBB_x2 + doc->GetRightMargin(&typeid(*note)) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR );
    note->GetGraceAlignment()->SetMaxWidth( note->m_contentBB_x2 );
    
    return FUNCTOR_CONTINUE;
}


int Object::SetBoundingBoxXShift( ArrayPtrVoid *params )
{
    // param 0: the minimu position (i.e., the width of the previous element)
    // param 1: the maximum width in the current measure
    // param 2: the Doc
    int *min_pos = static_cast<int*>((*params).at(0));
    int *measure_width = static_cast<int*>((*params).at(1));
    Doc *doc = static_cast<Doc*>((*params).at(2));

    // starting a new measure
    if (this->Is() == MEASURE) {
        Measure *current_measure = dynamic_cast<Measure*>(this);
        assert( current_measure );
        // we reset the measure width and the minimum position
        (*measure_width) = 0;
        (*min_pos) = 0;
        if (current_measure->GetLeftBarlineType() != BARRENDITION_NONE) {
            current_measure->GetLeftBarline()->SetBoundingBoxXShift( params );
        }
        return FUNCTOR_CONTINUE;
    }
    
    // starting an new layer
    if (this->Is() == LAYER) {
        Layer *current_layer = dynamic_cast<Layer*>(this);
        assert( current_layer );
        // reset it as the minimum position to the step (HARDCODED)
        (*min_pos) = 30 * doc->GetDrawingUnit(100) / 10;
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

    if (!this->IsLayerElement()) {
        return FUNCTOR_CONTINUE;
    }
    
    LayerElement *current = dynamic_cast<LayerElement*>(this);
    assert( current );
    
    // we should have processed aligned before
    assert( current->GetAlignment() );

    if ( !current->HasUpdatedBB() ) {
        // if nothing was drawn, do not take it into account
        return FUNCTOR_CONTINUE;
    }
    
    if (current->Is() == BEAM) {
        return FUNCTOR_CONTINUE;
    }
    
    if ( (current->Is() == NOTE) && current->GetFirstParent( CHORD, MAX_CHORD_DEPTH ) ) {
        return FUNCTOR_CONTINUE;
    }
    
    if (current->Is() == TIE) {
        return FUNCTOR_CONTINUE;
    }
    
    if (current->Is() == TUPLET) {
        return FUNCTOR_CONTINUE;
    }
    
    if ( (current->Is() == VERSE) || (current->Is() == SYL) ) {
        return FUNCTOR_CONTINUE;
    }
    
    if ( (current->Is() == ACCID) && current->GetFirstParent( NOTE, MAX_ACCID_DEPTH ) ) {
        return FUNCTOR_CONTINUE;
    }
    
    // the negative offset it the part of the bounding box that overflows on the left
    // |____x_____|
    //  ---- = negative offset
    int negative_offset = - (current->m_contentBB_x1);
    if (!current->IsGraceNote()) negative_offset += (doc->GetLeftMargin( current->Is() ) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR);
    
    // this should never happen (but can with glyphs not exactly registered at position x=0 in the SMuFL font used
    if ( negative_offset < 0 ) negative_offset = 0;
    
    
    if ((current->Is() == MREST) || (current->Is() == MRPT) || (this->Is() == MRPT2) || (this->Is() == MULTI_RPT)) {
        // With MRest, MRpt, etc., the only thing we want to do is to keep their with as possible measure with (if only MRest in all staves/layers)
        int width =  current->m_contentBB_x2 + doc->GetRightMargin( current->Is() ) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR + negative_offset ;
        // Keep it if more than the current measure width
        (*measure_width) = std::max( (*measure_width), (*min_pos) + width );
        return FUNCTOR_CONTINUE;
    }
    
    
    
    // with a grace note, also take into account the full with of the group given by the GraceAligner
    if (current->GetAlignment()->HasGraceAligner()) {
        negative_offset += current->GetAlignment()->GetGraceAligner()->GetWidth();
    }
    
    int currentX = current->GetAlignment()->GetXRel();
    // with grace note, take into account the position of the note in the grace group
    if (current->IsGraceNote()) {
        Note *note = dynamic_cast<Note*>(current);
        currentX += note->GetGraceAlignment()->GetXRel();
    }
    
    // check if the element overlaps with the preceeding one given by (*min_pos)
    int overlap = (*min_pos) - currentX + negative_offset;
    
    if ( (currentX - negative_offset) < (*min_pos) ) {
        current->GetAlignment()->SetXShift( overlap );
    }
    
    // do not ajust the min pos and the max width since this is already handled by
    // the GraceAligner
    if ( current->IsGraceNote() ) {
        (*min_pos) = current->GetAlignment()->GetXRel();
        current->GetAlignment()->SetMaxWidth( 0 );
        return FUNCTOR_CONTINUE;
    }

    // the next minimal position if given by the right side of the bounding box + the spacing of the element
    (*min_pos) = current->GetAlignment()->GetXRel() + current->m_contentBB_x2 + doc->GetRightMargin( current->Is() ) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR;
    current->GetAlignment()->SetMaxWidth( current->m_contentBB_x2 + doc->GetRightMargin( current->Is() ) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR );
    
    return FUNCTOR_CONTINUE;
}

int Object::SetBoundingBoxXShiftEnd( ArrayPtrVoid *params )
{
    // param 0: the minimu position (i.e., the width of the previous element)
    // param 1: the maximum width in the current measure
    int *min_pos = static_cast<int*>((*params).at(0));
    int *measure_width = static_cast<int*>((*params).at(1));
    
    // ending a measure
    if (this->Is() == MEASURE) {
        Measure *current_measure = dynamic_cast<Measure*>(this);
        assert( current_measure );
        // as minimum position of the barLine use the measure width
        (*min_pos) = (*measure_width);
        if (current_measure->GetRightBarlineType() != BARRENDITION_NONE) {
            current_measure->GetRightBarline()->SetBoundingBoxXShift( params );
        }
        return FUNCTOR_CONTINUE;
    }
    
    // ending a layer
    if (this->Is() == LAYER) {
        Layer *current_layer = dynamic_cast<Layer*>(this);
        assert( current_layer );
        // mininimum position is the with the layer
        // we keep it if is higher than what we had so far
        // this will be used for shifting the right barLine
        (*measure_width) = std::max( (*measure_width), (*min_pos) );
        return FUNCTOR_CONTINUE;
    }
    
    return FUNCTOR_CONTINUE;
}

int Object::SetBoundingBoxYShift( ArrayPtrVoid *params )
{
    // param 0: the position of the previous staff
    // param 1: the maximum height in the current system
    int *min_pos = static_cast<int*>((*params).at(0));
    int *system_height = static_cast<int*>((*params).at(1));
    
    // starting a new system
    if (this->Is() == SYSTEM) {
        System *current_system = dynamic_cast<System*>(this);
        assert( current_system );
        // we reset the system height
        (*system_height) = 0;
        (*min_pos) = 0;
        return FUNCTOR_CONTINUE;
    }
    
    // starting a new measure
    if (this->Is() == MEASURE) {
        (*min_pos) = 0;
        return FUNCTOR_CONTINUE;
    }
    
    // starting a new staff
    if (this->Is() != STAFF) {
        return FUNCTOR_CONTINUE;
    }
    
    Staff *current = dynamic_cast<Staff*>(this);
    assert( current );
    
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
    
int Object::SetBoundingBoxYShiftEnd( ArrayPtrVoid *params )
{
    // param 0: the position of the previous staff
    // param 1: the maximum height in the current system
    int *min_pos = static_cast<int*>((*params).at(0));
    int *system_height = static_cast<int*>((*params).at(1));
    
    // ending a measure
    if (this->Is() == MEASURE) {
        Measure *current_measure = dynamic_cast<Measure*>(this);
        assert( current_measure );
        // mininimum position is the height for the last (previous) staff
        // we keep it if it is higher than what we had so far
        (*system_height) = std::min( (*system_height), (*min_pos) );
        return FUNCTOR_CONTINUE;
    }
    
    // ending a system: see System::SetBoundingBoxYShiftEnd

    return FUNCTOR_CONTINUE;
}
        
int Object::Save( ArrayPtrVoid *params )
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream*>((*params).at(0));
    if (!output->WriteObject( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
}
    
    
int Object::SaveEnd( ArrayPtrVoid *params )
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream*>((*params).at(0));
    if (!output->WriteObjectEnd( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
}

} // namespace vrv



