/////////////////////////////////////////////////////////////////////////////
// Name:        object.h
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_OBJECT_H__
#define __VRV_OBJECT_H__

#include <cstdlib>
#include <ctime>
#include <iterator>
#include <map>
#include <string>

//----------------------------------------------------------------------------

#include "attclasses.h"
#include "boundingbox.h"
#include "vrvdef.h"

namespace vrv {

class Doc;
class DurationInterface;
class EditorialElement;
class FileOutputStream;
class Functor;
class FunctorParams;
class PitchInterface;
class PositionInterface;
class ScoreDefInterface;
class StemmedDrawingInterface;
class TextDirInterface;
class TimePointInterface;
class TimeSpanningInterface;

/**
 * Generic int map recursive structure for storing hierachy of values
 * For example, we want to process all staves one by one, and within each staff
 * all layer one by one, and so one (lyrics, etc.). In IntTree, we can store
 * @n with all existing values (1 => 1 => 1; 2 => 1 => 1)
 * The stucture must be filled first and can then be used by instanciating a vector
 * of corresponding AttComparison (typically AttCommonNComparison for @n attribute).
 * See Doc::PrepareDrawing for an example.
 */
struct IntTree {
    std::map<int, IntTree> child;
};

typedef std::map<int, IntTree> IntTree_t;

/**
 * This is the alternate way for representing map of maps. With this solution,
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
class Object : public BoundingBox {
public:
    /**
     * @name Constructors, destructors, and other standard methods
     * Reset method resets all attribute classes
     */
    ///@{
    Object();
    Object(std::string classid);
    virtual ~Object();
    virtual ClassId Is() const;
    virtual std::string GetClassName() const { return "[MISSING]"; }
    ///@}

    /**
     * Wrapper for checking if an element is a floating object (system elements and control elements)
     */
    bool IsFloatingObject() const { return (this->IsSystemElement() || this->IsControlElement()); }

    /**
     * Wrapper for checking if an element has a boundary start interface and also if is set as a boundary element
     */
    bool IsBoundaryElement();

    /**
     * @name Methods for checking if an object is part of a group of classId's.
     * For example, all LayerElement child class classId's are between LAYER_ELEMENT and LAYER_ELEMENT_max.
     * See classId enum.
     */
    ///@{
    bool IsControlElement() const { return (this->Is() > CONTROL_ELEMENT && this->Is() < CONTROL_ELEMENT_max); }
    bool IsEditorialElement() const { return (this->Is() > EDITORIAL_ELEMENT && this->Is() < EDITORIAL_ELEMENT_max); }
    bool IsLayerElement() const { return (this->Is() > LAYER_ELEMENT && this->Is() < LAYER_ELEMENT_max); }
    bool IsScoreDefElement() const { return (this->Is() > SCOREDEF_ELEMENT && this->Is() < SCOREDEF_ELEMENT_max); }
    bool IsSystemElement() const { return (this->Is() > SYSTEM_ELEMENT && this->Is() < SYSTEM_ELEMENT_max); }
    bool IsTextElement() const { return (this->Is() > TEXT_ELEMENT && this->Is() < TEXT_ELEMENT_max); }
    ///@}

    /**
     * @name Methods for registering a MEI att class and for registering interfaces regrouping MEI att classes.
     */
    ///@{
    void RegisterAttClass(AttClassId attClassId) { m_attClasses.push_back(attClassId); }
    bool HasAttClass(AttClassId attClassId) const
    {
        return std::find(m_attClasses.begin(), m_attClasses.end(), attClassId) != m_attClasses.end();
    }
    void RegisterInterface(std::vector<AttClassId> *attClasses, InterfaceId interfaceId);
    bool HasInterface(InterfaceId interfaceId) const
    {
        return std::find(m_interfaces.begin(), m_interfaces.end(), interfaceId) != m_interfaces.end();
    }
    ///@}

    virtual DurationInterface *GetDurationInterface() { return NULL; }
    virtual PitchInterface *GetPitchInterface() { return NULL; }
    virtual PositionInterface *GetPositionInterface() { return NULL; }
    virtual ScoreDefInterface *GetScoreDefInterface() { return NULL; }
    virtual StemmedDrawingInterface *GetStemmedDrawingInterface() { return NULL; }
    virtual TextDirInterface *GetTextDirInterface() { return NULL; }
    virtual TimePointInterface *GetTimePointInterface() { return NULL; }
    virtual TimeSpanningInterface *GetTimeSpanningInterface() { return NULL; }

    /**
     * Reset the object, that is 1) removing all childs and 2) resetting all attributes.
     * The method is virtual, so _always_ call the parent in the method overriding it.
     */
    virtual void Reset();

    /**
     * Copy constructor that also copy the children.
     * The children are copied using the Object::Clone virtual method that
     * needs to be overridden in the child class - otherwise, it will crash.
     * Because this will create a problem if we don't check this (the parents will
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
     * If idx is provided, move the children to the idx position in the object children.
     * Only moving to the same type is allow unless allowTypeChange is true.
     */
    void MoveChildrenFrom(Object *sourceParent, int idx = -1, bool allowTypeChange = false);

    /**
     * Move an object to another parent.
     * The object is relinquished from its current parent - see Object::Relinquish
     */
    void MoveItselfTo(Object *targetParent);

    /**
     * Method call for copying child classes.
     * The method has to be overridden.
     */
    virtual Object *Clone() const;

    std::string GetUuid() const { return m_uuid; }
    void SetUuid(std::string uuid);
    void ResetUuid();
    static void SeedUuid(unsigned int seed = 0);

    void SetSVGClass(const std::string &classcontent);
    void AddSVGClass(const std::string &classname);
    std::string GetSVGClass(void);
    bool HasSVGClass(void);

    std::string GetComment() const { return m_comment; }
    void SetComment(std::string comment) { m_comment = comment; }
    bool HasComment(void) { return !m_comment.empty(); }

    /**
     * @name Children count, with or without a ClassId.
     * Used for classes with several types of children.
     * The method with a ClassId only searches at the first level.
     */
    ///@{
    int GetChildCount() const { return (int)m_children.size(); }
    int GetChildCount(const ClassId classId) const;
    ///@}

    /**
     * Child access (generic)
     */
    Object *GetChild(int idx) const;

    /**
     * Fill an array of pairs with all attributes and their values.
     * Return the number of attributes found.
     */
    int GetAttributes(ArrayOfStrAttr *attributes) const;

    /**
     * Check if an Object has an attribute with the specified value
     */
    bool HasAttribute(std::string attribute, std::string value) const;

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
     * Return the last child of the object (if any, NULL otherwise)
     */
    Object *GetLast() const;

    /**
     * Set the parent of the Object.
     * The current parent is expected to be NULL.
     */
    void SetParent(Object *parent);

    /**
     * Base method for adding children.
     * The method has to be overridden.
     */
    virtual void AddChild(Object *object);

    /**
     * Return the index position of the object in its parent (-1 if not found)
     */
    int GetIdx() const;

    /**
     * Look for the Object in the children and return its position (-1 if not found)
     */
    int GetChildIndex(const Object *child);

    /**
     * Insert an element at the idx position.
     */
    void InsertChild(Object *element, int idx);

    /**
     * Detach the child at the idx position (NULL if not found)
     * The parent pointer is set to NULL.
     */
    Object *DetachChild(int idx);

    /**
     * Look for a child with the specified uuid (returns NULL if not found)
     * This method is a wrapper for the Object::FindByUuid functor.
     */
    Object *FindChildByUuid(std::string uuid, int deepness = UNLIMITED_DEPTH, bool direction = FORWARD);

    /**
     * Look for a child with the specified type (returns NULL if not found)
     * This method is a wrapper for the Object::FindByType functor.
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
     * Return all the object matching the AttComparison functor
     * Deepness allow to limit the depth search (EditorialElements are not count)
     */
    void FindAllChildByAttComparison(ArrayOfObjects *objects, AttComparison *attComparison,
        int deepness = UNLIMITED_DEPTH, bool direction = FORWARD, bool clear = true);

    /**
     * Give up ownership of the child at the idx position (NULL if not found)
     * This is a method to be used only in the very particular case where the child
     * object cannot be detached straight away. It is typically the case
     * when this has to be done within an iterator. The parent of the object
     * will be set to NULL but the object will not be removed. If the parent
     * is not destroyed after that, you should expect problems unless Object::ClearRelinquishedChildren is called
     * In other words: do not use unless you are absolutely sure what you are doing
     */
    Object *Relinquish(int idx);

    /**
     * Removes all the children that were previously relinquished.
     * This has to be used when children are moved but then the parent is not deleted.
     */
    void ClearRelinquishedChildren();

    /**
     * Remove and delete the child at the idx position.
     */
    void RemoveChildAt(int idx);

    /**
     * Return the first parent of the specified type.
     * The maxSteps parameter limits the search to a certain number of level if not -1.
     */
    Object *GetFirstParent(const ClassId classId, int maxSteps = -1) const;

    /**
     * Return the last parent that is NOT of the specified type.
     * The maxSteps parameter limits the search to a certain number of level if not -1.
     */
    Object *GetLastParentNot(const ClassId classId, int maxSteps = -1);

    /**
     * Fill the list of all the children LayerElement.
     * This is used for navigating in a Layer (See Layer::GetPrevious and Layer::GetNext).
     */
    void FillFlatList(ListOfObjects *list);

    /**
     * Check if the content was modified or not
     */
    bool IsModified() const { return m_isModified; }

    /**
     * Mark the object and its parent (if any) as modified
     */
    void Modify(bool modified = true);

    /**
     * @name Setter and getter of the attribute flag
     */
    ///@{
    bool IsAttribute() const { return m_isAttribute; }
    void IsAttribute(bool isAttribute) { m_isAttribute = isAttribute; }
    ///@}

    /**
     * Saves the object (and its children) using the specified output stream.
     * Creates functors that will parse the tree.
     */
    virtual int Save(FileOutputStream *output);

    /**
     * Main method that processes functors.
     * For each object, it will call the functor.
     * Depending on the code returned by the functor, it will also process it for all children.
     * The ArrayOfAttComparisons filter parameter makes is possible to process only objects of a
     * type that matches the attribute value given in the AttComparison object.
     * This is the generic way for parsing the tree, e.g., for extracting one single staff or layer.
     * Deepness specifies how many child levels should be processed. UNLIMITED_DEPTH means no
     * limit (EditorialElement objects do not count).
     */
    virtual void Process(Functor *functor, FunctorParams *functorParams, Functor *endFunctor = NULL,
        ArrayOfAttComparisons *filters = NULL, int deepness = UNLIMITED_DEPTH, bool direction = FORWARD);

    //----------//
    // Functors //
    //----------//

    /**
     * Add each LayerElements and its children to a flat list
     */
    virtual int AddLayerElementToFlatList(FunctorParams *functorParams);

    /**
     * @name Functors for finding objects
     */
    ///@{

    /**
     * Find a Object with a specified uuid.
     */
    virtual int FindByUuid(FunctorParams *functorParams);

    /**
     * Find a Object with a AttComparison functor .     */
    virtual int FindByAttComparison(FunctorParams *functorParams);

    /**
     * Find a Object with the extreme value with a AttComparison functor .
     */
    virtual int FindExtremeByAttComparison(FunctorParams *functorParams);

    /**
     * Find a all Object with a AttComparison functor.
     */
    virtual int FindAllByAttComparison(FunctorParams *functorParams);

    /**
     * Retrieve the time spanning layer elements between two points
     */
    virtual int FindTimeSpanningLayerElements(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    ///@}

    /**
     * @name Functors for loading and saving the docuement
     */
    ///@{

    /**
     * Convert top-level all container (section, endings) and editorial elements to boundary elements.
     */
    virtual int ConvertToPageBased(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }
    virtual int ConvertToPageBasedEnd(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Save the content of any object by calling the appropriate FileOutputStream method.
     */
    virtual int Save(FunctorParams *functorParams);
    virtual int SaveEnd(FunctorParams *functorParams);

    ///@}

    /**
     * @name Functors for aligning the content horizontally
     */
    ///@{

    /**
     * Adjust the position the outside articulations.
     */
    virtual int AdjustArtic(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Adjust the position the outside articulations with slur.
     */
    virtual int AdjustArticWithSlurs(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Adjust the position of all floating positionner, staff by staff.
     */
    virtual int AdjustFloatingPostioners(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Adjust the position of all floating positionner that are grouped, staff by staff.
     */
    virtual int AdjustFloatingPostionerGrps(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Align horizontally the content of a page.
     * For each LayerElement, instanciate its Alignment.
     * It creates it if no other note or event occurs at its position.
     * At the end, for each Layer, align the grace note stacked in GraceAlignment.
     */
    virtual int AlignHorizontally(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }
    virtual int AlignHorizontallyEnd(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Align the measures by adjusting the m_drawingXRel position looking at the MeasureAligner.
     * At the end, store the width of the system in the MeasureAligner for justification.
     */
    virtual int AlignMeasures(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }
    virtual int AlignMeasuresEnd(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Correct the X alignment once the content of a system has been aligned and laid out
     * See Measure::IntegrateBoundingBoxXShift for actual implementation
     */
    virtual int IntegrateBoundingBoxGraceXShift(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Correct the X alignment once the content of a system has been aligned and laid out
     * See Measure::IntegrateBoundingBoxXShift for actual implementation
     */
    virtual int IntegrateBoundingBoxXShift(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Reset the horizontal alignment environment for various types for object.
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Set the position of the Alignment.
     * Looks at the time difference from the previous Alignment.
     */
    virtual int SetAlignmentXPos(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Lay out the X positions of the grace notes looking at the bounding boxes.
     * The m_xShift is updated appropriately
     */
    virtual int SetBoundingBoxGraceXShift(FunctorParams *functorParams);

    /**
     * Lay out the X positions of the staff content looking at the bounding boxes.
     * The m_xShift is updated appropriately
     * At the end, lay out the X positions of the staff content looking at the bounding boxes.
     */
    virtual int SetBoundingBoxXShift(FunctorParams *functorParams);
    virtual int SetBoundingBoxXShiftEnd(FunctorParams *functorParams);

    ///@}

    /**
     * @name Functors for aligning the content vertically.
     */
    ///@{

    /**
     * Align vertically the content of a page.
     * For each Staff, instanciate its StaffAlignment.
     */
    virtual int AlignVertically(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Align the system by adjusting the m_drawingYRel position looking at the SystemAligner.
     */
    virtual int AlignSystems(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Calculate the overlap of the staff aligmnents by looking at the overflow bounding boxes
     */
    virtual int CalcStaffOverlap(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Correct the Y alignment once the content of a system has been aligned and laid out
     * See System::IntegrateBoundingBoxYShift for actual implementation
     */
    virtual int IntegrateBoundingBoxYShift(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Reset the verticall alignment environment for various types for object.
     */
    virtual int ResetVerticalAlignment(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Set the position of the StaffAlignment.
     */
    virtual int SetAligmentYPos(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Fill the arrays of bounding boxes (above and below) for each staff alignment for which the box overflows.
     */
    virtual int SetOverflowBBoxes(FunctorParams *functorParams);

    /**
     * Fill the arrays of bounding boxes (above and below) for each staff alignment for which the box overflows.
     */
    virtual int SetOverflowBBoxesEnd(FunctorParams *functorParams);

    ///@}

    /**
     * @name Functors setting the current scoreDef.
     */
    ///@{

    /**
     * Replace the drawing values a staffDef.
     * Set the current / drawing clef, key signature, etc. to the StaffDef
     * Called form ScoreDef::ReplaceDrawingValues.
     */
    virtual int ReplaceDrawingValuesInStaffDef(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Set the current scoreDef wherever need.
     * This is include a scoreDef for each system.
     * It also includes a scoreDef for each measure where a change occured before.
     * A change can be either a scoreDef before or a clef, meterSig, etc. within the previous measure.
     */
    virtual int SetCurrentScoreDef(FunctorParams *functorParams);

    /**
     * Set the cautionnary scoreDef wherever need.
     */
    virtual int SetCautionaryScoreDef(FunctorParams *functorParams);

    /**
     * Unset the initial scoreDef of each system and measure
     */
    virtual int UnsetCurrentScoreDef(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Set drawing flags for the StaffDef for indicating whether clefs, keysigs, etc. need
     * to be redrawn.
     * This typically occurs when a new System or a new  ScoreDef is encountered.
     * See implementation and Object::SetStaffDefRedrawFlags for the parameters.
     */
    virtual int SetStaffDefRedrawFlags(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    ///@}

    /**
     * @name Functors for preparing drawing
     */
    ///@{

    /**
     * Builds a tree of ints (IntTree) with the staff/layer/verse numbers and for staff/layer to be then processed.
     */
    virtual int PrepareProcessingLists(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Match start for TimePointingInterface elements (such as fermata or harm).
     */
    virtual int PrepareTimePointing(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }
    virtual int PrepareTimePointingEnd(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Match start and end for TimeSpanningInterface elements (such as tie or slur).
     * If fillList is set to false, only the remaining elements will be matched.
     * This is used when processing a second time in the other direction
     */
    virtual int PrepareTimeSpanning(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }
    virtual int PrepareTimeSpanningEnd(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Match start and end for TimeSpanningInterface elements with tstamp(2) attributes.
     * It is performed only on TimeSpanningInterface elements withouth @startid (or @endid).
     * It adds to the start (and end) measure a TimeStampAttr to the Measure::m_tstamps.
     */
    virtual int PrepareTimestamps(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }
    virtual int PrepareTimestampsEnd(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Process Chord and Note for matching @tie by processing by Layer and by looking
     * at the Pname and Oct.
     * At the end, processes Chord and Note for matching @tie by processing by Layer; resets the
     * Chord pointer to NULL at the end of a chord.
     */
    virtual int PrepareTieAttr(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }
    virtual int PrepareTieAttrEnd(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Process by Layer and set drawing pointers.
     * Set Dot::m_drawingNote for Dot elements in mensural mode
     * Set Note::m_drawingAccid for Note elements having an Accid child
     */
    virtual int PreparePointersByLayer(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Set wordpos and connector ends
     * The functor is processed by staff/layer/verse using an ArrayOfAttComparisons filter.
     * At the end, the functor is processed by doc at the end of a document of closing opened syl.
     */
    virtual int PrepareLyrics(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }
    virtual int PrepareLyricsEnd(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Functor for setting the artic parts.
     * Splits the artic content into different artic parts if necessary
     */
    virtual int PrepareArtic(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Functor for setting mRpt drawing numbers (if required)
     * The functor is processed by staff/layer using an ArrayOfAttComparisons filter.
     */
    virtual int PrepareRpt(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Functor for setting Measure of Ending
     */
    virtual int PrepareBoundaries(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Functor for grouping FloatingObject by drawingGrpId
     */
    virtual int PrepareFloatingGrps(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Go through all the TimeSpanningInterface elements and set them a current to each staff
     * where required. For Note with DrawingTieAttr, the functor is redirected to the tie object.
     * At the end, remove the TimeSpanningInterface element from the list when the last measure is reached.
     */
    virtual int FillStaffCurrentTimeSpanning(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }
    virtual int FillStaffCurrentTimeSpanningEnd(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Reset the drawing values before calling PrepareDrawing after changes.
     */
    virtual int ResetDrawing(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Set the drawing position (m_drawingX and m_drawingY) values for objects
     */
    virtual int SetDrawingXY(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    ///@}

    /**
     * @name Functors for justification
     */
    ///@{

    /**
     * Justify the X positions
     */
    virtual int JustifyX(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Justify the Y positions
     */
    virtual int JustifyY(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    ///@}

    /**
     * @name Functors for calculating the layout of a document.
     */
    ///@{

    /**
     * Fill a page by adding systems with the appropriate length.
     * At the end, add all the pending objects where reaching the end
     */
    virtual int CastOffSystems(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }
    virtual int CastOffSystemsEnd(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     *
     */
    virtual int CastOffPages(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Cast off the document according to the encoding provided (pb and sb)
     */
    virtual int CastOffEncoding(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Undo the cast of both pages and system.
     * This is used by Doc::ContinuousLayout for putting all pages / systems continously.
     */
    virtual int UnCastOff(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    ///@}

    /**
     * @name Functors for generating MIDI output.
     */
    ///@{

    /**
     * Export the object to a MidiFile
     */
    virtual int GenerateMIDI(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }
    virtual int GenerateMIDIEnd(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    /**
     * Calculate the maximum duration of each measure.
     */
    virtual int CalcMaxMeasureDuration(FunctorParams *functorParams) { UNUSED(functorParams); return FUNCTOR_CONTINUE; }

    ///@}

protected:
    /**
     * Clear the children vector and delete all the objects.
     */
    void ClearChildren();

private:
    void GenerateUuid();
    void Init(std::string);

public:
    /**
     * Keep an array of unsupported attributes as pairs.
     * This can be used for writing back data
     */
    ArrayOfStrAttr m_unsupported;

    Object *m_parent;

protected:
    ArrayOfObjects m_children;

private:
    std::string m_uuid;
    std::string m_classid;
    std::string m_svgclass;
    static unsigned long s_objectCounter;

    /**
     * Indicates whether the object content is up-to-date or not.
     * This is useful for object using sub-lists of objects when drawing.
     * For example, Beam has a list of children notes and this value indicates if the
     * list needs to be updated or not. Is is mostly an optimization feature.
     */
    mutable bool m_isModified;

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

    /**
     * A string for storing a comment to be printed immediately before
     * the object when printing an MEI element.
     */
    std::string m_comment;

    /**
     * A flag indicating if the Object represents an attribute in the original MEI.
     * For example, a Artic child in Note for an original @artic
     */
    bool m_isAttribute;
};

//----------------------------------------------------------------------------
// ObjectListInterface
//----------------------------------------------------------------------------

/**
 * This class is an pseudo interface for elements maintaining a flat list of
 * children LayerElement for processing.
 * The list is a flatten list of pointers to children elements.
 * It is not an abstract class but should not be instanciated directly.
 */
class ObjectListInterface {
public:
    // constructors and destructors
    ObjectListInterface(){};
    virtual ~ObjectListInterface(){};
    ObjectListInterface(const ObjectListInterface &listInterface); // copy constructor;
    ObjectListInterface &operator=(const ObjectListInterface &listInterface); // copy assignement;

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
    Object *GetListPrevious(Object *listElement);

    /**
     * Returns the next object in the list (NULL if not found)
     */
    Object *GetListNext(Object *listElement);

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
    virtual void FilterList(ListOfObjects *childList) { UNUSED(childList); }

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
    int (Object::*obj_fpt)(FunctorParams *functorParams); // pointer to member function

public:
    // constructor - takes pointer to an object and pointer to a member and stores
    // them in two private variables
    Functor();
    Functor(int (Object::*_obj_fpt)(FunctorParams *));
    virtual ~Functor(){};

    // override function "Call"
    virtual void Call(Object *ptr, FunctorParams *functorParams);

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
    ObjectComparison(const ClassId classId) { m_classId = classId; }

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
