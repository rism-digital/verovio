 /////////////////////////////////////////////////////////////////////////////
// Name:        object.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#ifndef __VRV_OBJECT_H__
#define __VRV_OBJECT_H__

#include <list>
#include <string>
#include <typeinfo>
#include <vector>

#include "vrvdef.h"

namespace vrv {

class Doc;
class MusFunctor;
class Object;

typedef std::vector<Object*> ArrayOfObjects;

typedef std::list<Object*> ListOfObjects;

typedef std::vector<void*> ArrayPtrVoid;

//----------------------------------------------------------------------------
// Object
//----------------------------------------------------------------------------

/** 
 * This class represents a basic object
 */
class Object
{
public:
    // constructors and destructors
    Object();
    Object(std::string classid);
    virtual ~Object();
    
    /**
     * Copy constructor that also copy the children.
     * The children are copied using the Object::Clone virtual method that
     * Needs to be overwritten in the child class - we make it crash otherwise,
     * Because this will create problem if we don't check this (the parents will 
     * one the same child...)
     * UUID: the uuid is copied, is needs to be reset later if this is not wished
     */
    Object( const Object& object );
    
    /**
     * See copy constructor.
     */
    Object& operator=( const Object& object ); // copy assignement;
    
    /**
     * Method call for copying child classes
     */
    virtual Object* Clone();
    
    virtual bool operator==( Object& other );
    
    std::string GetUuid() { return m_uuid; };
    void SetUuid( std::string uuid );
    void ResetUuid( );
    
    /**
     * Children count
     * Used for classes with several types of children
     */
    int GetChildCount() { return (int)m_children.size(); };
    
    /**
     * Child access (generic)
     */
    Object *GetChild( int idx );
    
    /**
     * Clear the children vector and delete all the objects.
     */
    void ClearChildren();
    
    /**
     * Set the parent of the Object.
     * The current parent is expected to be NULL.
     */
    void SetParent( Object *parent );
    
    virtual std::string GetClassName( ) { return "[MISSING]"; };
    
    /**
     * Reset the drawing values (m_drawingX, m_drawingXRel, etc.)
     * Called by SetCurrentScoreDef functor.
     */
    virtual void ResetDrawingValues( ) { };

    /**
     * Return the index position of the object in its parent (-1 if not found)
     */
    int GetIdx() const;

    /**
     * Look for the Object in the children and return its position (-1 if not found)
     */
    int GetChildIndex( const Object *child );
    
    /**
     * Insert a element at the idx position.
     */
    void InsertChild( Object *element, int idx );
    
    /**
     * Detach the child at the idx position (NULL if not found)
     * The parent pointer is set to NULL.
     */
    Object *DetachChild( int idx );
    
    /**
     * Give up ownership of the child at the idx position (NULL if not found)
     * This is a method to used only in very particular case where the child
     * object cannot be detached straight away. It is typically the case 
     * when this has to be done within an iterator. The parent of the object
     * will be set to NULL but the object will not be removed. If the parent
     * is not destroyed after that, you should expect problems...
     * In other words: do not use unless you are absolutely sure what you are doing
     */
    Object *Relinquish( int idx );
    
    /**
     * Remove and delete the child at the idx position.
     */
    void RemoveChildAt( int idx );
    
    /**
     * Return the first parent of the specified type.
     * The maxSteps parameter limit the search to a certain number of level if not -1.
     */
    Object *GetFirstParent( const std::type_info *elementType, int maxSteps = -1 );
    
    /**
     * Return the first of the specified type.
     */
    Object *GetFirstChild( const std::type_info *elementType );
    
    /**
     * Return the previous sibling object of the specified type.
     * If no type is specified, returns the previous object.
     * Returns NULL if not found in both cases.
     */
    Object *GetPreviousSibling( const std::type_info *elementType = NULL );
    
    /**
     * Return the next sibling object of the specified type.
     * If no type is specified, returns the next object.
     * Returns NULL if not found in both cases.
     */
    Object *GetNextSibling( const std::type_info *elementType = NULL );
    
    /**
     * Fill the list of all the children LayerElement.
     * This is used for navigating in a Layer (See Layer::GetPrevious and Layer::GetNext).
     */  
    void FillList( ListOfObjects *list );
    
    /**
     * Add a sameAs attribute to the object.
     * If a filename is given, the attribute will be filename#id.
     * If several value are added, they will be separated by a whitespace.
     */
    void AddSameAs( std::string id, std::string filename = "" );
    
    /**
     * Parse the sameAs attribute and return the one at the #idx position (if any).
     */
    bool GetSameAs( std::string *id, std::string *filename, int idx = 0 );
    
    /**
     * Check if the content was modified or not
     */
    bool IsModified( ) { return m_isModified; };
    
    /**
     * Mark the object and its parent (if any) as modified
     */
    void Modify( bool modified = true );
    
    /**
     * Main method that processes functors.
     * For each object, it will call the functor.
     * Depending on the code returned by the functor, it will also process it for all children.
     */
    virtual void Process( MusFunctor *functor, ArrayPtrVoid params, MusFunctor *endFunctor = NULL );
    
    //----------//
    // Functors //
    //----------//
    
    /**
     * Add each LayerElements and its children to a list
     */
    virtual int AddLayerElementToList( ArrayPtrVoid params );
    
    /**
     * See Layer::CopyToLayer
     */ 
    virtual int CopyToLayer( ArrayPtrVoid params ) { return false; };
    
    /**
     * Find a Object with a specified uuid.
     * param 0: the uuid we are looking for.
     * param 1: the pointer to pointer to the Object retrieved (if found).
     */
    virtual int FindByUuid( ArrayPtrVoid params );
    
    /**
     * Save the content of and object by calling the appropriate FileOutputStream method
     * Overriden in all classes.
     * param 0: a pointer to the FileOutputStream.
     */
    virtual int Save( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };

    /**
     * @name Functors for aligning the content horizontally
     */
    ///@{
    
    /**
     * Align the content of a system.
     * For each LayerElement, instanciate its Alignment.
     * It creates it if no other note or event occurs at its position.
     */
    virtual int Align( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    /**
     * Align the measures by adjusting the m_drawingXRel position looking at the MeasureAligner.
     */
    virtual int AlignMeasures( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    /**
     * Store the width of the system in the MeasureAligner for justification
     * This method is called at the end of a system.
     */
    virtual int AlignMeasuresEnd( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    /**
     * Correct the X alignment once the the content of a system has been aligned and laid out
     * See Measure::IntegrateBoundingBoxXShift for actual implementation
     */
    virtual int IntegrateBoundingBoxXShift( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };    
    
    /**
     * Set the position of the Alignment.
     * Looks at the time different with the previous Alignment.
     */
    virtual int SetAligmentXPos( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    /**
     * Lay out the X positions of the staff content looking that the bounding boxes.
     * The m_xShift is updated appropriately
     */
    virtual int SetBoundingBoxXShift( ArrayPtrVoid params );
    
    /**
     * Lay out the X positions of the staff content looking that the bounding boxes.
     * This is the MusFunctor called at the end of the measure or a layer.
     */
    virtual int SetBoundingBoxXShiftEnd( ArrayPtrVoid params );
    
    ///@}
    
    /**
     * @name Functors for aligning the content vertically.
     */
    ///@{
    
    /**
     * Align the system by adjusting the m_drawingYRel position looking at the SystemAligner.
     */
    virtual int AlignSystems( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    /**
     * Store the height of the system in the SystemAligner for justification
     * This method is called at the end of a system.
     */
    virtual int AlignSystemsEnd( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    /**
     * Correct the Y alignment once the the content of a system has been aligned and laid out
     * See System::IntegrateBoundingBoxYShift for actual implementation
     */
    virtual int IntegrateBoundingBoxYShift( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    /**
     * Set the position of the StaffAlignment.
     */
    virtual int SetAligmentYPos( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    /**
     * Lay out the Y positions of the staff looking that the bounding box of each staff.
     * The m_yShift is updated appropriately
     */
    virtual int SetBoundingBoxYShift( ArrayPtrVoid params );
    
    
    /**
     * Lay out the Y positions of the staff looking that the bounding boxes of each staff
     * This is the MusFunctor called at the end of the system or a measure.
     */
    virtual int SetBoundingBoxYShiftEnd( ArrayPtrVoid params );
    
    ///@}
    
    /**
     * @name Functors setting the current scoreDef.
     */
    ///@{
    
    /**
     * Replace all the staffDefs in a scoreDef.
     * param 0: a pointer to the scoreDef we are going to replace the staffDefs
     */
    virtual int ReplaceStaffDefsInScoreDef( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    /**
     * Set the initial scoreDef of each page.
     * This is necessary for integrating changes that occur within a page.
     */
    virtual int SetCurrentScoreDef( ArrayPtrVoid params );

    /**
     * Set the initial scoreDef of each page.
     * This is necessary for integrating changes that occur within a page.
     * param 0: bool clef flag.
     * param 1: bool keysig flag.
     * param 2: bool the mensur flag.
     */
    virtual int SetStaffDefDraw( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    ///@}
    
    /**
     * @name Functors for justification
     */
    ///@{
    
    /**
     * Justify the X positions
     */
    virtual int JustifyX( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    /**
     * Justify the Y positions
     */
    virtual int JustifyY( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    ///@}
    
    /**
     * @name Functors for calculating the layout of a document.
     */
    ///@{
    
    /**
     * Fill a page by adding systems with the appropriate length
     * 
     */
    virtual int CastOffSystems( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    /**
     *
     */
    virtual int CastOffPages( ArrayPtrVoid params ) { return FUNCTOR_CONTINUE; };
    
    ///@}

public:
    ArrayOfObjects m_children;
    Object *m_parent;
    std::string m_sameAs;
    
protected:
    std::string m_uuid;
    std::string m_classid;

private:
    
    void GenerateUuid();
    void Init(std::string);
    
    /**
     * Indicated whether the object content is up-to-date or not.
     * This is usefull for object using sub-lists of objects when drawing.
     * For example, Beam has a list of children notes and this value indicates if the
     * list needs to be updated or not. Is is moslty and optimization feature.
     */
    bool m_isModified;
    
};


//----------------------------------------------------------------------------
// DocObject
//----------------------------------------------------------------------------

/** 
 * This class represents a basic object in the layout domain
 */
class DocObject: public Object
{
public:
    // constructors and destructors
    DocObject();
    DocObject(std::string classid);
    virtual ~DocObject();
    
    /**
     * Refreshes the views from Doc.
     * From other DocObject, simply pass it to its parent until Doc is reached.
     */
    virtual void Refresh();
    
    void UpdateContentBB( int x1, int y1, int x2, int y2);
    void UpdateSelfBB( int x1, int y1, int x2, int y2 );
    bool HasContentBB();
    bool HasSelfBB();
    void ResetBB();

    int m_contentBB_x1, m_contentBB_y1, m_contentBB_x2, m_contentBB_y2;
    int m_selfBB_x1, m_selfBB_y1, m_selfBB_x2, m_selfBB_y2; 
    
    /**
     * Is true if the bounding box (self or content) has been updated at least once.
     * We need this to avoid not updating bounding boxes to screw up the layout with their intial values.
     */
    bool HasUpdatedBB( ) { return m_updatedBB; };
    
private:
    bool m_updatedBB;
    
protected:
    
public:
    
};


//----------------------------------------------------------------------------
// ObjectListInterface
//----------------------------------------------------------------------------

/** 
 * This class is an interface for elements maintaining a list of children
 * LayerElement for processing.
 * The list is a flatten list of pointers to children elements.
 * It is not an abstract class but should not be instanciate directly.
 */
class ObjectListInterface
{
public:
    // constructors and destructors
    ObjectListInterface() {};
    virtual ~ObjectListInterface() {};
    ObjectListInterface( const ObjectListInterface& interface ); // copy constructor;
    ObjectListInterface& operator=( const ObjectListInterface& interface ); // copy assignement;

    /**
     * Look for the Object in the list and return its position (-1 if not found)
     */
    int GetListIndex( const Object *listElement );
    
    /**
     * Returns the previous object in the list (NULL if not found)
     */
    Object *GetListPrevious( const Object *listElement );

    /**
     * Returns the next object in the list (NULL if not found)
     */
    Object *GetListNext( const Object *listElement );
    
    /**
     * Return the list.
     * Before returning the list, it checks that the list is up-to-date with Object::IsModified
     * If not, it updates the list and also calls FilterList.
     * Because this is an interface, we need to pass the object - not the best design.
     */
    ListOfObjects *GetList( Object *node );
    
protected:
    /**
     * Filter the list for a specific class.
     * For example, keep only notes in Beam
     */
    virtual void FilterList() {};

public:    
    /**
     * Reset the list of children and call FilterList().
     * As for GetList, we need to pass the object.
     */
    void ResetList( Object *node );
        
    ListOfObjects m_list;
    
};

//----------------------------------------------------------------------------
// abstract base class MusFunctor
//----------------------------------------------------------------------------

/** 
 * This class is an abstact Functor for the object hierarchy.
 * Needs testing.
 */
class MusFunctor
{
private:
    int (Object::*obj_fpt)( ArrayPtrVoid params );   // pointer to member function
    
public:
    
    // constructor - takes pointer to an object and pointer to a member and stores
    // them in two private variables
    MusFunctor( );
    MusFunctor( int(Object::*_obj_fpt)( ArrayPtrVoid ));
	virtual ~MusFunctor() {};
    
    // override function "Call"
    virtual void Call( Object *ptr, ArrayPtrVoid params );
    
    int m_returnCode;
    bool m_reverse;
    
private:
    
};

} // namespace vrv

#endif
