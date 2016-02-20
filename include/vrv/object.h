/////////////////////////////////////////////////////////////////////////////
// Name:        object.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_OBJECT_H__
#define __VRV_OBJECT_H__

#include <iterator>
#include <map>
#include <string>

//----------------------------------------------------------------------------

#include "attclasses.h"
#include "vrvdef.h"

namespace vrv {

class Doc;
class EditorialElement;
class FileOutputStream;
class Functor;

/**
 * Generic int map recursive structure for storing hierachy of values
 * For example, we want to process all staves one by one, and within each staff
 * all layer one by one, and so one (lyrics, etc.). In IntTree, we can store
 * @n with all existing values (1 => 1 => 1; 2 => 1 => 1)
 * The stucture must be filled first an can then be use by instanciating a vector
 * of corresponding AttComparison (typically AttCommonNComparison for @n attribute).
 * See Doc::PrepareDrawing for an example.
 */
struct IntTree {
    std::map<int, IntTree> child;
};

typedef std::map<int, IntTree> IntTree_t;

/**
 * This is a alternate way of representing map of maps. With this solution,
 * we can easily have different types of key (attribute) at each level. We could
 * mix int, string, or even MEI data_* types. The drawback is that a type has to
 * be defined at each level. Also see Doc::PrepareDrawing for an example.
 */
typedef std::map<int, bool> VerseN_t;
typedef std::map<int, VerseN_t> LayerN_VerserN_t;
typedef std::map<int, LayerN_VerserN_t> StaffN_LayerN_VerseN_t;

#define UNLIMITED_DEPTH -10000
#define FORWARD true
#define BACKWARD false

//----------------------------------------------------------------------------
// Object
//----------------------------------------------------------------------------

/**
 * This class represents a basic object
 */
class Object {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method reset all attribute classes
     */
    ///@{
    Object();
    Object(std::string classid);
    virtual ~Object();
    virtual ClassId Is();
    virtual std::string GetClassName() { return "[MISSING]"; };
    ///@}

    /**
     * @name Methods for checking if an object is part of a group of classId's.
     * For example, all LayerElement child class classId's are between LAYER_ELEMENT and LAYER_ELEMENT_max.
     * See classId enum.
     */
    ///@{
    bool IsEditorialElement() { return (this->Is() > EDITORIAL_ELEMENT && this->Is() < EDITORIAL_ELEMENT_max); };
    bool IsLayerElement() { return (this->Is() > LAYER_ELEMENT && this->Is() < LAYER_ELEMENT_max); };
    bool IsFloatingElement() { return (this->Is() > FLOATING_ELEMENT && this->Is() < FLOATING_ELEMENT_max); };
    bool IsScoreDefElement() { return (this->Is() > SCOREDEF_ELEMENT && this->Is() < SCOREDEF_ELEMENT_max); };
    bool IsTextElement() { return (this->Is() > TEXT_ELEMENT && this->Is() < TEXT_ELEMENT_max); };
    ///@}

    /**
     * @name Methods for registering a MEI att class and for registering interfaces regrouping MEI att classes.
     */
    ///@{
    void RegisterAttClass(AttClassId attClassId) { m_attClasses.push_back(attClassId); };
    bool HasAttClass(AttClassId attClassId)
    {
        return std::find(m_attClasses.begin(), m_attClasses.end(), attClassId) != m_attClasses.end();
    };
    void RegisterInterface(std::vector<AttClassId> *attClasses, InterfaceId interfaceId);
    bool HasInterface(InterfaceId interfaceId)
    {
        return std::find(m_interfaces.begin(), m_interfaces.end(), interfaceId) != m_interfaces.end();
    };
    ///@}

    /**
     * Reset the object, that is 1) removing all childs and 2) resetting all attributes.
     * The method is virual, so _always_ call the parent in the method overriding it.
     */
    virtual void Reset() { ClearChildren(); };

    /**
     * Copy constructor that also copy the children.
     * The children are copied using the Object::Clone virtual method that
     * Needs to be overwritten in the child class - we make it crash otherwise,
     * Because this will create problem if we don't check this (the parents will
     * one the same child...)
     * UUID: the uuid is copied, is needs to be reset later if this is not wished
     */
    Object(const Object &object);

    /**
     * Copy assignement - see copy constructor.
     */
    Object &operator=(const Object &object);

    /**
     * Move all the children of the object passed as parameter to this one.
     * Objects must be of the same type.
     * After this operation, the object passed as parameter has no child anymore.
     */
    void MoveChildren(Object *object);

    /**
     * Method call for copying child classes.
     * The method has to be overwritten.
     */
    virtual Object *Clone();

    std::string GetUuid() { return m_uuid; };
    void SetUuid(std::string uuid);
    void ResetUuid();

    /**
     * @name Children count, with or without a ClassId.
     * Used for classes with several types of children.
     * The method with a ClassId only searches at the first level.
     */
    ///@{
    int GetChildCount() { return (int)m_children.size(); };
    int GetChildCount(const ClassId classId);
    ///@}

    /**
     * Child access (generic)
     */
    Object *GetChild(int idx);

    /**
     * Fill an array of pair with all attributes and their value.
     * Return the number of attribute found.
     */
    int GetAttributes(ArrayOfStrAttr *attributes);

    /**
     * Check if an Object has an attribute with the specified values
     */
    bool HasAttribute(std::string attribute, std::string value);

    /**
     * @name Iterator methods for accessing children.
     * GetFirst returns the first element child of the specified type.
     * Its position and the specified type are stored and used of accessing next elements
     * The methods returns NULL when no child is found or when the end is reached.
     * Always call GetFirst before calling GetNext
     */
    ///@{
    Object *GetFirst(const ClassId classId = UNSPECIFIED);
    Object *GetNext();
    ///@}

    /**
     * Set the parent of the Object.
     * The current parent is expected to be NULL.
     */
    void SetParent(Object *parent);

    /**
     * Add an EditorialElement as child.
     * This can happen at many level.
     */
    void AddEditorialElement(EditorialElement *child);

    /**
     * Return the index position of the object in its parent (-1 if not found)
     */
    int GetIdx() const;

    /**
     * Look for the Object in the children and return its position (-1 if not found)
     */
    int GetChildIndex(const Object *child);

    /**
     * Insert a element at the idx position.
     */
    void InsertChild(Object *element, int idx);

    /**
     * Detach the child at the idx position (NULL if not found)
     * The parent pointer is set to NULL.
     */
    Object *DetachChild(int idx);

    /**
     * Look for a child with the specified uuid (returns NULL if not found)
     * This method is a wrapper to a Object::FindByUuid functor.
     */
    Object *FindChildByUuid(std::string uuid, int deepness = UNLIMITED_DEPTH, bool direction = FORWARD);

    /**
     * Look for a child with the specified type (returns NULL if not found)
     * This method is a wrapper to a Object::FindByType functor.
     */
    Object *FindChildByType(ClassId classId, int deepness = UNLIMITED_DEPTH, bool direction = FORWARD);

    /**
     * Return the first element matching the AttComparison functor
     * Deepness allow to limit the depth search (EditorialElements are not count)
     */
    Object *FindChildByAttComparison(
        AttComparison *attComparison, int deepness = UNLIMITED_DEPTH, bool direction = FORWARD);

    /**
     * Return the element matching the extreme value with an AttComparison functor
     * Deepness allow to limit the depth search (EditorialElements are not count)
     */
    Object *FindChildExtremeByAttComparison(
        AttComparison *attComparison, int deepness = UNLIMITED_DEPTH, bool direction = FORWARD);

    /**
     * Give up ownership of the child at the idx position (NULL if not found)
     * This is a method to used only in very particular case where the child
     * object cannot be detached straight away. It is typically the case
     * when this has to be done within an iterator. The parent of the object
     * will be set to NULL but the object will not be removed. If the parent
     * is not destroyed after that, you should expect problems...
     * In other words: do not use unless you are absolutely sure what you are doing
     */
    Object *Relinquish(int idx);

    /**
     * Remove and delete the child at the idx position.
     */
    void RemoveChildAt(int idx);

    /**
     * Return the first parent of the specified type.
     * The maxSteps parameter limit the search to a certain number of level if not -1.
     */
    Object *GetFirstParent(const ClassId classId, int maxSteps = -1);

    /**
     * Return the last parent that is NOT of the specified type.
     * The maxSteps parameter limit the search to a certain number of level if not -1.
     */
    Object *GetLastParentNot(const ClassId classId, int maxSteps = -1);

    /**
     * Fill the list of all the children LayerElement.
     * This is used for navigating in a Layer (See Layer::GetPrevious and Layer::GetNext).
     */
    void FillFlatList(ListOfObjects *list);

    /**
     * Parse the sameAs attribute and return the one at the #idx position (if any).
     */
    bool GetSameAs(std::string *id, std::string *filename, int idx = 0);

    /**
     * Check if the content was modified or not
     */
    bool IsModified() { return m_isModified; };

    /**
     * Mark the object and its parent (if any) as modified
     */
    void Modify(bool modified = true);

    /**
     * Saves the object (and its children) using the specified output stream.
     * Creates a functors that will parse the tree.
     */
    virtual int Save(FileOutputStream *output);

    /**
     * Main method that processes functors.
     * For each object, it will call the functor.
     * Depending on the code returned by the functor, it will also process it for all children.
     * The ArrayOfAttComparisons filter parameter makes is possible to process only objects of a
     * type that match the attribute value given in the AttComparison object.
     * This is a generic way for parsing the tree, e.g., for extracting one single staff, or layer.
     * Deepness allow to specify how many child levels should be processed UNLIMITED_DEPTH means no
     * limit (EditorialElement objects do not count).
     */
    virtual void Process(Functor *functor, ArrayPtrVoid *params, Functor *endFunctor = NULL,
        ArrayOfAttComparisons *filters = NULL, int deepness = UNLIMITED_DEPTH, bool direction = FORWARD);

    //----------//
    // Functors //
    //----------//

    /**
     * Add each LayerElements and its children to a flat list
     */
    virtual int AddLayerElementToFlatList(ArrayPtrVoid *params);

    /**
     * Find a Object with a specified uuid.
     * param 0: the uuid we are looking for.
     * param 1: the pointer to pointer to the Object retrieved (if found).
     */
    virtual int FindByUuid(ArrayPtrVoid *params);

    /**
     * Find a Object with a AttComparison functor .
     * param 0: the pointer to the AttComparsion we are evaluating.
     * param 1: the pointer to pointer to the Object retrieved (if found).
     */
    virtual int FindByAttComparison(ArrayPtrVoid *params);

    /**
     * Find a Object with the extreme value with a AttComparison functor .
     * param 0: the pointer to the AttComparsion we are evaluating.
     * param 1: the pointer to pointer to the Object retrieved (if found).
     */
    virtual int FindExtremeByAttComparison(ArrayPtrVoid *params);

    /**
     * Save the content of and object by calling the appropriate FileOutputStream method
     * param 0: a pointer to the FileOutputStream.
     */
    virtual int Save(ArrayPtrVoid *params);

    /**
     * Save the content of and object by calling the appropriate FileOutputStream method
     * param 0: a pointer to the FileOutputStream.
     */
    virtual int SaveEnd(ArrayPtrVoid *params);

    /**
     * @name Functors for aligning the content horizontally
     */
    ///@{

    /**
     * Reset the horizontal alignment environment for various types for object.
     */
    virtual int ResetHorizontalAlignment(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Align horizontally the content of a page.
     * For each LayerElement, instanciate its Alignment.
     * It creates it if no other note or event occurs at its position.
     */
    virtual int AlignHorizontally(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * For each Layer, align the grace note stacked in GraceAlignment
     */
    virtual int AlignHorizontallyEnd(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Align the measures by adjusting the m_drawingXRel position looking at the MeasureAligner.
     * param 0: the cumulated shift
     */
    virtual int AlignMeasures(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Store the width of the system in the MeasureAligner for justification
     * This method is called at the end of a system.
     */
    virtual int AlignMeasuresEnd(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Correct the X alignment once the the content of a system has been aligned and laid out
     * See Measure::IntegrateBoundingBoxXShift for actual implementation
     */
    virtual int IntegrateBoundingBoxGraceXShift(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Correct the X alignment once the the content of a system has been aligned and laid out
     * See Measure::IntegrateBoundingBoxXShift for actual implementation
     */
    virtual int IntegrateBoundingBoxXShift(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Set the position of the Alignment.
     * Looks at the time difference from the previous Alignment.
     */
    virtual int SetAlignmentXPos(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Lay out the X positions of the grace notes looking that the bounding boxes.
     * The m_xShift is updated appropriately
     */
    virtual int SetBoundingBoxGraceXShift(ArrayPtrVoid *params);

    /**
     * Lay out the X positions of the staff content looking that the bounding boxes.
     * The m_xShift is updated appropriately
     */
    virtual int SetBoundingBoxXShift(ArrayPtrVoid *params);

    /**
     * Lay out the X positions of the staff content looking that the bounding boxes.
     * This is the Functor called at the end of the measure or a layer.
     */
    virtual int SetBoundingBoxXShiftEnd(ArrayPtrVoid *params);

    ///@}

    /**
     * @name Functors for aligning the content vertically.
     */
    ///@{

    /**
     * Reset the verticall alignment environment for various types for object.
     */
    virtual int ResetVerticalAlignment(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Align vertically the content of a page.
     * For each Staff, instanciate its StaffAlignment.
     */
    virtual int AlignVertically(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Align the system by adjusting the m_drawingYRel position looking at the SystemAligner.
     */
    virtual int AlignSystems(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Store the height of the system in the SystemAligner for justification
     * This method is called at the end of a system.
     */
    virtual int AlignSystemsEnd(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Correct the Y alignment once the the content of a system has been aligned and laid out
     * See System::IntegrateBoundingBoxYShift for actual implementation
     */
    virtual int IntegrateBoundingBoxYShift(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Set the position of the StaffAlignment.
     */
    virtual int SetAligmentYPos(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Lay out the Y positions of the staff looking that the bounding box of each staff.
     * The m_yShift is updated appropriately
     */
    virtual int SetBoundingBoxYShift(ArrayPtrVoid *params);

    /**
     * Lay out the Y positions of the staff looking that the bounding boxes of each staff
     * This is the Functor called at the end of the system or a measure.
     */
    virtual int SetBoundingBoxYShiftEnd(ArrayPtrVoid *params);

    ///@}

    /**
     * @name Functors setting the current scoreDef.
     */
    ///@{

    /**
     * Replace the drawing values a staffDef.
     * param 0: Clef pointer (NULL if none)
     * param 1: KeySig pointer (NULL if none)
     * param 2: Mensur pointer (NULL if none)
     * param 3: MeterSig pointer (NULL if none)
     */
    virtual int ReplaceDrawingValuesInStaffDef(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Set the initial scoreDef of each page.
     * This is necessary for integrating changes that occur within a page.
     */
    virtual int SetCurrentScoreDef(ArrayPtrVoid *params);

    /**
     * Set the initial scoreDef of each page.
     * This is necessary for integrating changes that occur within a page.
     * param 0: bool clef flag.
     * param 1: bool keysig flag.
     * param 2: bool the mensur flag.
     * param 3: bool the metersig flag.
     * param 4: bool the keysig cancellation flag;
     */
    virtual int SetStaffDefRedrawFlags(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    ///@}

    /**
     * Builds a tree of int (IntTree) with the staff/layer/verse numbers
     * and for staff/layer to be then processed.
     * param 0: IntTree*
     * param 1: IntTree*
     */
    virtual int PrepareProcessingLists(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Matches start and end for TimeSpanningInterface elements (such as tie or slur)
     * If fillList is set to false, the only the remaining elements will be matched.
     * This is used when processing a second time in the other direction
     * param 0: std::vector<DocObject*>* that holds the current elements to match
     * param 1: bool* fillList for indicating whether the elements have to be stack or not
     */
    virtual int PrepareTimeSpanning(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Processes Chord and Note for matching @tie by processing by Layer and by looking
     * at the Pname and Oct
     * param 0: std::vector<Note*>* that holds the current notes with open ties
     * param 1: Chord** currentChord for the current chord if in a chord
     */
    virtual int PrepareTieAttr(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Processes Chord and Note for matching @tie by processing by Layer; resets the
     * Chord pointer to NULL at the end of a chord
     * param 0: std::vector<Note*>* that holds the current notes with open ties (unused)
     * param 1: Chord** currentChord for the current chord if in a chord
     */
    virtual int PrepareTieAttrEnd(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Processes by Layer and set drawing pointers.
     * Set Dot::m_drawingNote for Dot elements in mensural mode
     * Set Note::m_drawingAccid for Note elements having an Accid child
     * param 0: Note** currentNote for the current not to w
     */
    virtual int PreparePointersByLayer(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Functor for setting wordpos and connector ends
     * The functor is process by staff/layer/verse using an ArrayOfAttComparisons filter.
     */
    virtual int PrepareLyrics(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Functor for setting wordpos and connector ends
     * The functor is process by doc at the end of a document of closing opened syl.
     */
    virtual int PrepareLyricsEnd(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Functor for setting mRpt drawing numbers (if required)
     * The functor is process by staff/layer using an ArrayOfAttComparisons filter.
     * param 0: MRpt **currentMRpt
     * param 1: data_BOOLEAN for indicating if the MRpt::m_drawingNumber has to be set or not
     * param 2: ScoreDef * doc scoreDef
     */
    virtual int PrepareRpt(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Goes through all the TimeSpanningInterface element and set them a current to each staff
     * where require. For Note with DrawingTieAttr, the functor is redireted to the tie object
     * param 0: std::vector<DocObject*>* of the current running TimeSpanningInterface elements
     */
    virtual int FillStaffCurrentTimeSpanning(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Remove the TimeSpanningInterface element from the list when the last measure is reached.
     * param 0: std::vector<DocObject*>* of the current running TimeSpanningInterface elements
     */
    virtual int FillStaffCurrentTimeSpanningEnd(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Set the drawing position (m_drawingX and m_drawingY) values for objects
     */
    virtual int SetDrawingXY(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * @name Functors for justification
     */
    ///@{

    /**
     * Justify the X positions
     */
    virtual int JustifyX(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Justify the Y positions
     */
    virtual int JustifyY(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    ///@}

    /**
     * @name Functors for calculating the layout of a document.
     */
    ///@{

    /**
     * Fill a page by adding systems with the appropriate length.
     *
     */
    virtual int CastOffSystems(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     *
     */
    virtual int CastOffPages(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Undo the cast of of both pages and system.
     * This is used by Doc::ContinuousLayout for putting all pages / system continously.
     */
    virtual int UnCastOff(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    ///@}

    /**
     *
     */
    virtual int TimeSpanningLayerElements(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Export the object to a MidiFile
     * param 0: MidiFile*: the MidiFile we are writing to
     * param 1: int*: the midi track number
     * param 2: int*: the current time in the measure (incremented by each element)
     * param 3: int*: the current total measure time (incremented by each measure
     * param 4: std::vector<double>: a stack of maximum duration of each measure (emptied by the functor)
     */
    virtual int ExportMIDI(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Export the object to a MidiFile (end method)
     */
    virtual int ExportMIDIEnd(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * Calculate the maximum duration of each measure.
     * param 0: std::vector<double>: a stack of maximum duration filled by the functor
     * param 1: double: the maximum duration of the current measure
     */
    virtual int CalcMaxMeasureDuration(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

    /**
     * See Object::CalcMaxMeasureDuration
     */
    virtual int CalcMaxMeasureDurationEnd(ArrayPtrVoid *params) { return FUNCTOR_CONTINUE; };

protected:
    /**
     * Clear the children vector and delete all the objects.
     */
    void ClearChildren();

public:
    ArrayOfObjects m_children;
    Object *m_parent;

    /**
     * Keep an array of unsupported attributes as pairs.
     * This can be used for writing back data
     */
    ArrayOfStrAttr m_unsupported;

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

    /**
     * Members used for caching iterator values.
     * See Object::IterGetFirst, Object::IterGetNext and Object::IterIsNotEnd
     * Values are set when GetFirst is called (which is mandatory)
     */
    ArrayOfObjects::iterator m_iteratorEnd, m_iteratorCurrent;
    ClassId m_iteratorElementType;

    /**
     * A vector for storing the list of AttClassId (MEI att classes) implemented.
     */
    std::vector<AttClassId> m_attClasses;

    /**
     * A vector for storing the list of InterfaceId (group of MEI att classes) implemented.
     */
    std::vector<InterfaceId> m_interfaces;
};

//----------------------------------------------------------------------------
// DocObject
//----------------------------------------------------------------------------

/**
 * This class represents a basic object in the layout domain
 */
class DocObject : public Object {
public:
    // constructors and destructors
    DocObject();
    DocObject(std::string classid);
    virtual ~DocObject();
    virtual ClassId Is() { return DOC_OBJECT; }

    void UpdateContentBB(int x1, int y1, int x2, int y2);
    void UpdateSelfBB(int x1, int y1, int x2, int y2);
    bool HasContentBB();
    bool HasSelfBB();
    void ResetBB();
    void SetEmptyBB();
    bool HasEmptyBB();

    /**
     * @name Get and set the X and Y drawing position
     */
    ///@{
    int GetDrawingX() { return m_drawingX; };
    int GetDrawingY() { return m_drawingY; };
    void SetDrawingX(int drawingX) { m_drawingX = drawingX; };
    void SetDrawingY(int drawingY) { m_drawingY = drawingY; };
    ///@}

    /**
     * Is true if the bounding box (self or content) has been updated at least once.
     * We need this to avoid not updating bounding boxes to screw up the layout with their intial values.
     */
    bool HasUpdatedBB() { return m_updatedBB; };

private:
    bool m_updatedBB;

protected:
    /**
     * The Y drawing position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingY;
    /**
     * The X drawing position of the object.
     * It is re-computed everytime the object is drawn and it is not stored in the file.
     */
    int m_drawingX;

public:
    int m_contentBB_x1, m_contentBB_y1, m_contentBB_x2, m_contentBB_y2;
    int m_selfBB_x1, m_selfBB_y1, m_selfBB_x2, m_selfBB_y2;
};

//----------------------------------------------------------------------------
// ObjectListInterface
//----------------------------------------------------------------------------

/**
 * This class is an pseudo interface for elements maintaining a flat list of
 * children LayerElement for processing.
 * The list is a flatten list of pointers to children elements.
 * It is not an abstract class but should not be instanciate directly.
 */
class ObjectListInterface {
public:
    // constructors and destructors
    ObjectListInterface(){};
    virtual ~ObjectListInterface(){};
    ObjectListInterface(const ObjectListInterface &interface); // copy constructor;
    ObjectListInterface &operator=(const ObjectListInterface &interface); // copy assignement;

    /**
     * Look for the Object in the list and return its position (-1 if not found)
     */
    int GetListIndex(const Object *listElement);

    /**
     * Gets the first item of type elementType starting at startFrom
     */
    Object *GetListFirst(const Object *startFrom, const ClassId classId = UNSPECIFIED);
    Object *GetListFirstBackward(Object *startFrom, const ClassId classId = UNSPECIFIED);

    /**
     * Returns the previous object in the list (NULL if not found)
     */
    Object *GetListPrevious(const Object *listElement);

    /**
     * Returns the next object in the list (NULL if not found)
     */
    Object *GetListNext(const Object *listElement);

    /**
     * Return the list.
     * Before returning the list, it checks that the list is up-to-date with Object::IsModified
     * If not, it updates the list and also calls FilterList.
     * Because this is an interface, we need to pass the object - not the best design.
     */
    ListOfObjects *GetList(Object *node);

private:
    ListOfObjects m_list;
    ListOfObjects::iterator m_iteratorCurrent;

protected:
    /**
     * Filter the list for a specific class.
     * For example, keep only notes in Beam
     */
    virtual void FilterList(ListOfObjects *childList){};

public:
    /**
     * Reset the list of children and call FilterList().
     * As for GetList, we need to pass the object.
     */
    void ResetList(Object *node);
};

//----------------------------------------------------------------------------
// TextListInterface
//----------------------------------------------------------------------------

/**
 * This class is an pseudo interface for elements maintaining a flat list of
 * children LayerElement for processing.
 * The list is a flatten list of pointers to children elements.
 * It is not an abstract class but should not be instanciate directly.
 */
class TextListInterface : public ObjectListInterface {
public:
    // constructors and destructors
    TextListInterface(){};
    virtual ~TextListInterface(){};

    /**
     * Returns a contatenated version of all the text children
     */
    std::wstring GetText(Object *node);

protected:
    /**
     * Filter the list for a specific class.
     * For example, keep only notes in Beam
     */
    virtual void FilterList(ListOfObjects *childList);

private:
    //
public:
    //
private:
};

//----------------------------------------------------------------------------
// abstract base class Functor
//----------------------------------------------------------------------------

/**
 * This class is an abstact Functor for the object hierarchy.
 * Needs testing.
 */
class Functor {
private:
    int (Object::*obj_fpt)(ArrayPtrVoid *params); // pointer to member function

public:
    // constructor - takes pointer to an object and pointer to a member and stores
    // them in two private variables
    Functor();
    Functor(int (Object::*_obj_fpt)(ArrayPtrVoid *));
    virtual ~Functor(){};

    // override function "Call"
    virtual void Call(Object *ptr, ArrayPtrVoid *params);

private:
    //
public:
    /**
     * The return code of the functor.
     * FUNCTOR_CONTINUE: continue processing
     * FUNCTOR_SIBLINGS: process only siblings (do not go deeper)
     * FUNCTOR_STOP: stop the functor (e.g., when an Object or a value is found)
     */
    int m_returnCode;
    /**
     * A flag for indicating if only visible Object have to be processed.
     * The value is true by default.
     */
    bool m_visibleOnly;

private:
};

//----------------------------------------------------------------------------
// ObjectComparison
//----------------------------------------------------------------------------

/**
 * Unary predicate for comparing object types.
 * This is used for example in std::find_if.
 * See Object::GetFirst or Object::GetNext
 */
class ObjectComparison {

public:
    ObjectComparison(const ClassId classId) { m_classId = classId; };

    bool operator()(Object *object)
    {
        if (m_classId == UNSPECIFIED) {
            return true;
        }
        return (object->Is() == m_classId);
    }

private:
    //
public:
    //
private:
    ClassId m_classId;
};

} // namespace vrv

#endif
