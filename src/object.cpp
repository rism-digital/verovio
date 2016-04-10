/////////////////////////////////////////////////////////////////////////////
// Name:        object.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#include <sstream>

//----------------------------------------------------------------------------

#include "chord.h"
#include "dir.h"
#include "doc.h"
#include "dynam.h"
#include "editorial.h"
#include "io.h"
#include "keysig.h"
#include "layer.h"
#include "ligature.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "note.h"
#include "page.h"
#include "staff.h"
#include "system.h"
#include "tempo.h"
#include "text.h"
#include "textelement.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// BoundingBox
//----------------------------------------------------------------------------

BoundingBox::BoundingBox()
{
    ResetBoundingBox();
}

ClassId BoundingBox::Is() const
{
    // we should always have the method overridden
    assert(false);
    return BOUNDING_BOX;
};

void BoundingBox::UpdateContentBBoxX(int x1, int x2)
{
    // LogDebug("CB Was: %i %i %i %i", m_contentBB_x1, m_contentBB_y1, m_contentBB_x2, m_contentBB_y2);

    int min_x = std::min(x1, x2);
    int max_x = std::max(x1, x2);

    min_x -= m_drawingX;
    max_x -= m_drawingX;

    if (m_contentBB_x1 > min_x) m_contentBB_x1 = min_x;
    if (m_contentBB_x2 < max_x) m_contentBB_x2 = max_x;

    m_updatedBBoxX = true;
    // LogDebug("CB Is:  %i %i %i %i %s", m_contentBB_x1,m_contentBB_y1, m_contentBB_x2, m_contentBB_y2,
    // GetClassName().c_str());
}

void BoundingBox::UpdateContentBBoxY(int y1, int y2)
{
    // LogDebug("CB Was: %i %i %i %i", m_contentBB_x1, m_contentBB_y1, m_contentBB_x2, m_contentBB_y2);

    int min_y = std::min(y1, y2);
    int max_y = std::max(y1, y2);

    min_y -= m_drawingY;
    max_y -= m_drawingY;

    if (m_contentBB_y1 > min_y) m_contentBB_y1 = min_y;
    if (m_contentBB_y2 < max_y) m_contentBB_y2 = max_y;

    m_updatedBBoxY = true;
    // LogDebug("CB Is:  %i %i %i %i %s", m_contentBB_x1,m_contentBB_y1, m_contentBB_x2, m_contentBB_y2,
    // GetClassName().c_str());
}

void BoundingBox::UpdateSelfBBoxX(int x1, int x2)
{
    // LogDebug("SB Was: %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);

    int min_x = std::min(x1, x2);
    int max_x = std::max(x1, x2);

    min_x -= m_drawingX;
    max_x -= m_drawingX;

    if (m_selfBB_x1 > min_x) m_selfBB_x1 = min_x;
    if (m_selfBB_x2 < max_x) m_selfBB_x2 = max_x;

    m_updatedBBoxX = true;

    // LogDebug("SB Is:  %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);
}

void BoundingBox::UpdateSelfBBoxY(int y1, int y2)
{
    // LogDebug("SB Was: %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);

    int min_y = std::min(y1, y2);
    int max_y = std::max(y1, y2);

    min_y -= m_drawingY;
    max_y -= m_drawingY;

    if (m_selfBB_y1 > min_y) m_selfBB_y1 = min_y;
    if (m_selfBB_y2 < max_y) m_selfBB_y2 = max_y;

    m_updatedBBoxY = true;

    // LogDebug("SB Is:  %i %i %i %i", m_selfBB_x1,m_selfBB_y1, m_selfBB_x2, m_selfBB_y2);
}

void BoundingBox::ResetBoundingBox()
{
    m_contentBB_x1 = 0xFFFFFFF;
    m_contentBB_y1 = 0xFFFFFFF;
    m_contentBB_x2 = -0xFFFFFFF;
    m_contentBB_y2 = -0xFFFFFFF;
    m_selfBB_x1 = 0xFFFFFFF;
    m_selfBB_y1 = 0xFFFFFFF;
    m_selfBB_x2 = -0xFFFFFFF;
    m_selfBB_y2 = -0xFFFFFFF;

    m_updatedBBoxX = false;
    m_updatedBBoxY = false;
}

void BoundingBox::SetEmptyBB()
{
    m_contentBB_x1 = 0;
    m_contentBB_y1 = 0;
    m_contentBB_x2 = 0;
    m_contentBB_y2 = 0;
    m_selfBB_x1 = 0;
    m_selfBB_y1 = 0;
    m_selfBB_x2 = 0;
    m_selfBB_y2 = 0;

    m_updatedBBoxX = true;
    m_updatedBBoxY = true;
}

bool BoundingBox::HasEmptyBB()
{
    return (HasUpdatedBB() && (m_contentBB_x1 == 0) && (m_contentBB_y1 == 0) && (m_contentBB_x2 == 0)
        && (m_contentBB_y2 == 0));
}

bool BoundingBox::HasContentBB()
{
    return ((m_contentBB_x1 != 0xFFFF) && (m_contentBB_y1 != 0xFFFF) && (m_contentBB_x2 != -0xFFFF)
        && (m_contentBB_y2 != -0xFFFF));
}

bool BoundingBox::HasSelfBB()
{
    return ((m_selfBB_x1 != 0xFFFF) && (m_selfBB_y1 != 0xFFFF) && (m_selfBB_x2 != -0xFFFF) && (m_selfBB_y2 != -0xFFFF));
}

bool BoundingBox::HorizontalOverlap(const BoundingBox *other) const
{
    assert(other);

    if (m_drawingX + m_contentBB_x2 < other->GetDrawingX() + other->m_contentBB_x1) return false;
    if (m_drawingX + m_contentBB_x1 > other->GetDrawingX() + other->m_contentBB_x2) return false;
    return true;
}

int BoundingBox::CalcVerticalOverlap(const BoundingBox *other) const
{
    assert(other);
    return 0;
}

//----------------------------------------------------------------------------
// Object
//----------------------------------------------------------------------------

Object::Object() : BoundingBox()
{
    Init("m-");
}

Object::Object(std::string classid) : BoundingBox()
{
    Init(classid);
}

Object *Object::Clone() const
{
    // This should never happen because the method should be overridden
    assert(false);
    return NULL;
}

Object::Object(const Object &object) : BoundingBox(object)
{
    ClearChildren();
    ResetBoundingBox(); // It does not make sense to keep the values of the BBox
    m_parent = NULL;
    m_classid = object.m_classid;
    m_uuid = object.m_uuid; // for now copy the uuid - to be decided
    m_isModified = true;

    int i;
    for (i = 0; i < (int)object.m_children.size(); i++) {
        Object *current = object.m_children.at(i);
        Object *copy = current->Clone();
        copy->Modify();
        copy->SetParent(this);
        m_children.push_back(copy);
    }
}

Object &Object::operator=(const Object &object)
{
    // not self assignement
    if (this != &object) {
        ClearChildren();
        ResetBoundingBox(); // It does not make sense to keep the values of the BBox
        m_parent = NULL;
        m_classid = object.m_classid;
        m_uuid = object.m_uuid; // for now copy the uuid - to be decided
        m_isModified = true;

        int i;
        for (i = 0; i < (int)object.m_children.size(); i++) {
            Object *current = object.m_children.at(i);
            Object *copy = current->Clone();
            copy->Modify();
            copy->SetParent(this);
            m_children.push_back(copy);
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

    Reset();
}

ClassId Object::Is() const
{
    // we should always have the method overridden
    assert(false);
    return OBJECT;
};

void Object::Reset()
{
    ClearChildren();
    ResetBoundingBox();
};

void Object::RegisterInterface(std::vector<AttClassId> *attClasses, InterfaceId interfaceId)
{
    m_attClasses.insert(m_attClasses.end(), attClasses->begin(), attClasses->end());
    m_interfaces.push_back(interfaceId);
}

void Object::MoveChildren(Object *object)
{
    if (this == object) {
        assert("Object cannot be copied to itself");
    }
    if (this->Is() != object->Is()) {
        assert("Object must be of the same type");
    }

    int i;
    for (i = 0; i < (int)object->m_children.size(); i++) {
        this->m_children.push_back(object->Relinquish(i));
        object->m_children.at(i)->m_parent = this;
    }
}

void Object::SetUuid(std::string uuid)
{
    m_uuid = uuid;
};

void Object::ClearChildren()
{
    ArrayOfObjects::iterator iter;
    for (iter = m_children.begin(); iter != m_children.end(); ++iter) {
        // we need to check if this is the parent
        // ownership might have been given up with Relinquish
        if ((*iter)->m_parent == this) {
            delete *iter;
        }
    }
    m_children.clear();
}

int Object::GetChildCount(const ClassId classId) const
{
    return (int)count_if(m_children.begin(), m_children.end(), ObjectComparison(classId));
}

int Object::GetAttributes(ArrayOfStrAttr *attributes) const
{
    assert(attributes);
    attributes->clear();

    Att::GetCmn(this, attributes);
    Att::GetMensural(this, attributes);
    Att::GetPagebased(this, attributes);
    Att::GetShared(this, attributes);

    return (int)attributes->size();
}

bool Object::HasAttribute(std::string attribute, std::string value) const
{
    ArrayOfStrAttr attributes;
    this->GetAttributes(&attributes);
    ArrayOfStrAttr::iterator iter;
    for (iter = attributes.begin(); iter != attributes.end(); iter++) {
        if (((*iter).first == attribute) && ((*iter).second == value)) return true;
    }
    return false;
}

Object *Object::GetFirst(const ClassId classId)
{
    m_iteratorElementType = classId;
    m_iteratorEnd = m_children.end();
    m_iteratorCurrent = std::find_if(m_children.begin(), m_iteratorEnd, ObjectComparison(m_iteratorElementType));
    return (m_iteratorCurrent == m_iteratorEnd) ? NULL : *m_iteratorCurrent;
}

Object *Object::GetNext()
{
    m_iteratorCurrent++;
    m_iteratorCurrent = std::find_if(m_iteratorCurrent, m_iteratorEnd, ObjectComparison(m_iteratorElementType));
    return (m_iteratorCurrent == m_iteratorEnd) ? NULL : *m_iteratorCurrent;
}

Object *Object::GetLast() const
{
    if (m_children.empty()) return NULL;
    return m_children.back();
}

int Object::GetIdx() const
{
    assert(m_parent);

    return m_parent->GetChildIndex(this);
}

void Object::InsertChild(Object *element, int idx)
{
    // With this method we require the parent to be set before
    assert(element->m_parent == this);

    if (idx >= (int)m_children.size()) {
        m_children.push_back(element);
        return;
    }
    ArrayOfObjects::iterator iter = m_children.begin();
    m_children.insert(iter + (idx), element);
}

Object *Object::DetachChild(int idx)
{
    if (idx >= (int)m_children.size()) {
        return NULL;
    }
    Object *child = m_children.at(idx);
    child->m_parent = NULL;
    ArrayOfObjects::iterator iter = m_children.begin();
    m_children.erase(iter + (idx));
    return child;
}

Object *Object::Relinquish(int idx)
{
    if (idx >= (int)m_children.size()) {
        return NULL;
    }
    Object *child = m_children.at(idx);
    child->m_parent = NULL;
    return child;
}

Object *Object::FindChildByUuid(std::string uuid, int deepness, bool direction)
{
    Functor findByUuid(&Object::FindByUuid);
    Object *element = NULL;
    ArrayPtrVoid params;
    params.push_back(&uuid);
    params.push_back(&element);
    this->Process(&findByUuid, &params, NULL, NULL, deepness, direction);
    return element;
}

Object *Object::FindChildByType(ClassId classId, int deepness, bool direction)
{
    AttComparison attComparison(classId);
    return FindChildByAttComparison(&attComparison, deepness, direction);
}

Object *Object::FindChildByAttComparison(AttComparison *attComparison, int deepness, bool direction)
{
    Functor findByAttComparison(&Object::FindByAttComparison);
    Object *element = NULL;
    ArrayPtrVoid params;
    params.push_back(attComparison);
    params.push_back(&element);
    this->Process(&findByAttComparison, &params, NULL, NULL, deepness, direction);
    return element;
}

Object *Object::FindChildExtremeByAttComparison(AttComparison *attComparison, int deepness, bool direction)
{
    Functor findExtremeByAttComparison(&Object::FindExtremeByAttComparison);
    Object *element = NULL;
    ArrayPtrVoid params;
    params.push_back(attComparison);
    params.push_back(&element);
    this->Process(&findExtremeByAttComparison, &params, NULL, NULL, deepness, direction);
    return element;
}

Object *Object::GetChild(int idx) const
{
    if ((idx < 0) || (idx >= (int)m_children.size())) {
        return NULL;
    }
    return m_children.at(idx);
}

void Object::RemoveChildAt(int idx)
{
    if (idx >= (int)m_children.size()) {
        return;
    }
    delete m_children.at(idx);
    ArrayOfObjects::iterator iter = m_children.begin();
    m_children.erase(iter + (idx));
}

void Object::GenerateUuid()
{
    int nr = std::rand();
    char str[17];
    // I do not want to use a stream for doing this!
    snprintf(str, 16, "%016d", nr);

    m_uuid = m_classid + std::string(str);
}

void Object::ResetUuid()
{
    GenerateUuid();
}

void Object::SetParent(Object *parent)
{
    assert(!m_parent);
    m_parent = parent;
}

void Object::AddEditorialElement(EditorialElement *child)
{
    assert(dynamic_cast<Dir *>(this) || dynamic_cast<Dynam *>(this) || dynamic_cast<Layer *>(this)
        || dynamic_cast<LayerElement *>(this) || dynamic_cast<Lem *>(this) || dynamic_cast<Measure *>(this)
        || dynamic_cast<Note *>(this) || dynamic_cast<Staff *>(this) || dynamic_cast<System *>(this)
        || dynamic_cast<Tempo *>(this) || dynamic_cast<EditorialElement *>(this) || dynamic_cast<TextElement *>(this));
    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

int Object::GetChildIndex(const Object *child)
{
    ArrayOfObjects::iterator iter;
    int i;
    for (iter = m_children.begin(), i = 0; iter != m_children.end(); ++iter, i++) {
        if (child == *iter) {
            return i;
        }
    }
    return -1;
}

void Object::Modify(bool modified)
{
    // if we have a parent and a new modification, propagate it
    if (m_parent && !m_isModified && modified) {
        m_parent->Modify();
    }
    m_isModified = modified;
}

void Object::FillFlatList(ListOfObjects *flatList)
{
    Functor addToFlatList(&Object::AddLayerElementToFlatList);
    ArrayPtrVoid params;
    params.push_back(&flatList);
    this->Process(&addToFlatList, &params);
}

Object *Object::GetFirstParent(const ClassId classId, int maxDepth)
{
    if ((maxDepth == 0) || !m_parent) {
        return NULL;
    }

    if (m_parent->Is() == classId) {
        return m_parent;
    }
    else {
        return (m_parent->GetFirstParent(classId, maxDepth - 1));
    }
}

Object *Object::GetLastParentNot(const ClassId classId, int maxDepth)
{
    if ((maxDepth == 0) || !m_parent) {
        return NULL;
    }

    if (m_parent->Is() == classId) {
        return this;
    }
    else {
        return (m_parent->GetLastParentNot(classId, maxDepth - 1));
    }
}

void Object::Process(Functor *functor, ArrayPtrVoid *params, Functor *endFunctor, ArrayOfAttComparisons *filters,
    int deepness, bool direction)
{
    if (functor->m_returnCode == FUNCTOR_STOP) {
        return;
    }

    if (functor->m_visibleOnly && this->IsEditorialElement()) {
        EditorialElement *editorialElement = dynamic_cast<EditorialElement *>(this);
        assert(editorialElement);
        if (editorialElement->m_visibility == Hidden) {
            return;
        }
    }

    functor->Call(this, params);

    // do not go any deeper in this case
    if (functor->m_returnCode == FUNCTOR_SIBLINGS) {
        functor->m_returnCode = FUNCTOR_CONTINUE;
        return;
    }
    else if (this->IsEditorialElement()) {
        // since editorial object doesn't count, we increase the deepness limit
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
    // For processing backwards, we operated on a copied reversed version
    // Since we hold pointers, only addresses are copied
    if (direction == BACKWARD) {
        reversed = (*children);
        std::reverse(reversed.begin(), reversed.end());
        children = &reversed;
    }
    for (iter = children->begin(); iter != children->end(); ++iter) {
        if (filters && !filters->empty()) {
            bool hasAttComparison = false;
            // first we look if there is a comparison object for the object type (e.g., a Staff)
            ArrayOfAttComparisons::iterator attComparisonIter;
            for (attComparisonIter = filters->begin(); attComparisonIter != filters->end(); attComparisonIter++) {
                // if yes, we will use it (*attComparisonIter) for evaluating if the object matches
                // the attribute (see below)
                Object *o = *iter;
                if (o->Is() == (*attComparisonIter)->GetType()) {
                    hasAttComparison = true;
                    break;
                }
            }
            if (hasAttComparison) {
                // use the operator of the AttComparison object to evaluate the attribute
                if ((**attComparisonIter)(*iter)) {
                    // the attribute value matches, process the object
                    // LogDebug("%s ", (*iter)->GetClassName().c_str());
                    (*iter)->Process(functor, params, endFunctor, filters, deepness, direction);
                    break;
                }
                else {
                    // the attribute value does not match, skip this child
                    continue;
                }
            }
        }
        // we will end here if there is no filter at all or for the current child type
        (*iter)->Process(functor, params, endFunctor, filters, deepness, direction);
    }

    if (endFunctor) {
        endFunctor->Call(this, params);
    }
}

int Object::Save(FileOutputStream *output)
{
    ArrayPtrVoid params;
    params.push_back(output);

    Functor save(&Object::Save);
    // Special case where we want to process all objects
    save.m_visibleOnly = false;
    Functor saveEnd(&Object::SaveEnd);
    this->Process(&save, &params, &saveEnd);

    return true;
}

//----------------------------------------------------------------------------
// ObjectListInterface
//----------------------------------------------------------------------------

ObjectListInterface::ObjectListInterface(const ObjectListInterface &interface)
{
    // actually nothing to do, we just don't want the list to be copied
    m_list.clear();
}

ObjectListInterface &ObjectListInterface::operator=(const ObjectListInterface &interface)
{
    // actually nothing to do, we just don't want the list to be copied
    if (this != &interface) {
        this->m_list.clear();
    }
    return *this;
}

void ObjectListInterface::ResetList(Object *node)
{
    // nothing to do, the list if up to date
    if (!node->IsModified()) {
        return;
    }

    node->Modify(false);
    m_list.clear();
    node->FillFlatList(&m_list);
    this->FilterList(&m_list);
}

ListOfObjects *ObjectListInterface::GetList(Object *node)
{
    ResetList(node);
    return &m_list;
}

int ObjectListInterface::GetListIndex(const Object *listElement)
{
    ListOfObjects::iterator iter;
    int i;
    for (iter = m_list.begin(), i = 0; iter != m_list.end(); ++iter, i++) {
        if (listElement == *iter) {
            return i;
        }
    }
    return -1;
}

Object *ObjectListInterface::GetListFirst(const Object *startFrom, const ClassId classId)
{
    ListOfObjects::iterator it = m_list.begin();
    std::advance(it, GetListIndex(startFrom));
    it = std::find_if(it, m_list.end(), ObjectComparison(classId));
    return (it == m_list.end()) ? NULL : *it;
}

Object *ObjectListInterface::GetListFirstBackward(Object *startFrom, const ClassId classId)
{
    ListOfObjects::iterator it = m_list.begin();
    std::advance(it, GetListIndex(startFrom));
    ListOfObjects::reverse_iterator rit(it);
    rit = std::find_if(rit, m_list.rend(), ObjectComparison(classId));
    return (rit == m_list.rend()) ? NULL : *rit;
}

Object *ObjectListInterface::GetListPrevious(Object *listElement)
{
    ListOfObjects::iterator iter;
    int i;
    for (iter = m_list.begin(), i = 0; iter != m_list.end(); ++iter, i++) {
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

Object *ObjectListInterface::GetListNext(Object *listElement)
{
    ListOfObjects::reverse_iterator iter;
    int i;
    for (iter = m_list.rbegin(), i = 0; iter != m_list.rend(); ++iter, i++) {
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

//----------------------------------------------------------------------------
// TextListInterface
//----------------------------------------------------------------------------

std::wstring TextListInterface::GetText(Object *node)
{
    // alternatively we could cache the concatString in the interface and instantiate it in FilterList
    std::wstring concatText;
    ListOfObjects *childList = this->GetList(node); // make sure it's initialized
    for (ListOfObjects::iterator it = childList->begin(); it != childList->end(); it++) {
        Text *text = dynamic_cast<Text *>(*it);
        assert(text);
        concatText += text->GetText();
    }
    return concatText;
}

void TextListInterface::FilterList(ListOfObjects *childList)
{
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (((*iter)->Is() != TEXT)) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc)
            iter = childList->erase(iter);
            continue;
        }
        iter++;
    }
}

//----------------------------------------------------------------------------
// Functor
//----------------------------------------------------------------------------

Functor::Functor()
{
    m_returnCode = FUNCTOR_CONTINUE;
    m_visibleOnly = true;
    obj_fpt = NULL;
}

Functor::Functor(int (Object::*_obj_fpt)(ArrayPtrVoid *))
{
    m_returnCode = FUNCTOR_CONTINUE;
    m_visibleOnly = true;
    obj_fpt = _obj_fpt;
}

void Functor::Call(Object *ptr, ArrayPtrVoid *params)
{
    // we should have return codes (not just bool) for avoiding to go further down the tree in some cases
    m_returnCode = (*ptr.*obj_fpt)(params);
}

//----------------------------------------------------------------------------
// Object functor methods
//----------------------------------------------------------------------------

int Object::AddLayerElementToFlatList(ArrayPtrVoid *params)
{
    // param 0: the ListOfObjects
    ListOfObjects **list = static_cast<ListOfObjects **>((*params).at(0));
    // if (dynamic_cast<LayerElement*>(this)) {
    (*list)->push_back(this);
    //}
    return FUNCTOR_CONTINUE;
}

int Object::FindByUuid(ArrayPtrVoid *params)
{
    // param 0: the uuid we are looking for
    // param 1: the pointer to pointer to the Object
    std::string *uuid = static_cast<std::string *>((*params).at(0));
    Object **element = static_cast<Object **>((*params).at(1));

    if ((*element)) {
        // this should not happen, but just in case
        return FUNCTOR_STOP;
    }

    if ((*uuid) == this->GetUuid()) {
        (*element) = this;
        // LogDebug("Found it!");
        return FUNCTOR_STOP;
    }
    // LogDebug("Still looking for uuid...");
    return FUNCTOR_CONTINUE;
}

int Object::FindByAttComparison(ArrayPtrVoid *params)
{
    // param 0: the type we are looking for
    // param 1: the pointer to pointer to the Object
    AttComparison *test = static_cast<AttComparison *>((*params).at(0));
    Object **element = static_cast<Object **>((*params).at(1));

    if ((*element)) {
        // this should not happen, but just in case
        return FUNCTOR_STOP;
    }

    // evaluate by applying the AttComparison operator()
    if ((*test)(this)) {
        (*element) = this;
        // LogDebug("Found it!");
        return FUNCTOR_STOP;
    }
    // LogDebug("Still looking for the object matching the AttComparison...");
    return FUNCTOR_CONTINUE;
}

int Object::FindExtremeByAttComparison(ArrayPtrVoid *params)
{
    // param 0: the type we are looking for
    // param 1: the pointer to pointer to the Object
    AttComparison *test = static_cast<AttComparison *>((*params).at(0));
    Object **element = static_cast<Object **>((*params).at(1));

    // evaluate by applying the AttComparison operator()
    if ((*test)(this)) {
        (*element) = this;
    }
    // continue until the end
    return FUNCTOR_CONTINUE;
}

int Object::SetCurrentScoreDef(ArrayPtrVoid *params)
{

    // param 0: the current scoreDef
    ScoreDef *currentScoreDef = static_cast<ScoreDef *>((*params).at(0));
    StaffDef **currentStaffDef = static_cast<StaffDef **>((*params).at(1));

    assert(currentScoreDef);

    // starting a new page
    if (this->Is() == PAGE) {
        // The keySig cancellation is set to false, which means that a scoreDef change has to occur
        // after a page break if right at the begining. This is the same for systems below
        Page *page = dynamic_cast<Page *>(this);
        assert(page);
        if (page->m_parent->GetChildIndex(page) == 0) {
            currentScoreDef->SetRedrawFlags(true, true, true, true, false);
            currentScoreDef->SetDrawLabels(true);
        }
        else {
            currentScoreDef->SetRedrawFlags(true, true, false, false, false);
            currentScoreDef->SetDrawLabels(false);
        }
        page->m_drawingScoreDef = *currentScoreDef;
        return FUNCTOR_CONTINUE;
    }

    // starting a new system
    if (this->Is() == SYSTEM) {
        currentScoreDef->SetRedrawFlags(true, true, false, false, false);
        return FUNCTOR_CONTINUE;
    }

    // starting a new scoreDef
    if (this->Is() == SCOREDEF) {
        ScoreDef *scoreDef = dynamic_cast<ScoreDef *>(this);
        assert(scoreDef);
        // Replace the current scoreDef with the new one, including its content (staffDef)
        currentScoreDef->ReplaceDrawingValues(scoreDef);
        return FUNCTOR_CONTINUE;
    }

    // starting a new staffDef
    if (this->Is() == STAFFDEF) {
        StaffDef *staffDef = dynamic_cast<StaffDef *>(this);
        assert(staffDef);
        currentScoreDef->ReplaceDrawingValues(staffDef);
    }

    // starting a new staff
    if (this->Is() == STAFF) {
        Staff *staff = dynamic_cast<Staff *>(this);
        assert(staff);
        (*currentStaffDef) = currentScoreDef->GetStaffDef(staff->GetN());
        return FUNCTOR_CONTINUE;
    }

    // starting a new layer
    if (this->Is() == LAYER) {
        Layer *layer = dynamic_cast<Layer *>(this);
        assert(layer);
        // setting the layer stem direction. Alternatively, this could be done in
        // View::DrawLayer. If this (and other things) is kept here, renaming the method to something
        // more generic (PrepareDrawing?) might be a good idea...
        if (layer->m_parent->GetChildCount() > 1) {
            if (layer->m_parent->GetChildIndex(layer) == 0) {
                layer->SetDrawingStemDir(STEMDIRECTION_up);
            }
            else {
                layer->SetDrawingStemDir(STEMDIRECTION_down);
            }
        }
        layer->SetDrawingAndCurrentValues((*currentStaffDef));
        return FUNCTOR_CONTINUE;
    }

    // starting a new clef
    if (this->Is() == CLEF) {
        Clef *clef = dynamic_cast<Clef *>(this);
        assert(clef);
        assert(*currentStaffDef);
        (*currentStaffDef)->SetCurrentClef(new Clef(*clef));
        return FUNCTOR_CONTINUE;
    }

    // starting a new keysig
    if (this->Is() == KEYSIG) {
        KeySig *keysig = dynamic_cast<KeySig *>(this);
        assert(keysig);
        assert(*currentStaffDef);
        (*currentStaffDef)->SetCurrentKeySig(new KeySig(*keysig));
        return FUNCTOR_CONTINUE;
    }

    return FUNCTOR_CONTINUE;
}

int Object::SetBoundingBoxGraceXShift(ArrayPtrVoid *params)
{
    // param 0: the minimum position (i.e., the width of the previous element)
    // param 1: the Doc
    int *min_pos = static_cast<int *>((*params).at(0));
    Doc *doc = static_cast<Doc *>((*params).at(1));

    // starting new layer
    if (this->Is() == LAYER) {
        (*min_pos) = 0;
        return FUNCTOR_CONTINUE;
    }

    if (this->Is() != NOTE) {
        return FUNCTOR_CONTINUE;
    }

    Note *note = dynamic_cast<Note *>(this);
    assert(note);

    if (!note->IsGraceNote()) {
        (*min_pos) = 0;
        return FUNCTOR_CONTINUE;
    }

    // we should have processed aligned before
    assert(note->GetGraceAlignment());

    // the negative offset is the part of the bounding box that overflows on the left
    // |____x_____|
    //  ---- = negative offset
    int negative_offset
        = -(note->m_contentBB_x1) + (doc->GetLeftMargin(NOTE) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR);

    if ((*min_pos) > 0) {
        //(*min_pos) += (doc->GetLeftMargin(&typeid(*note)) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR);
    }

    // this should never happen (but can with glyphs not exactly registered at position x=0 in the SMuFL font used)
    if (negative_offset < 0) negative_offset = 0;

    // check if the element overlaps with the preceeding one given by (*min_pos)
    int overlap = (*min_pos) - note->GetGraceAlignment()->GetXRel() + negative_offset;

    if ((note->GetGraceAlignment()->GetXRel() - negative_offset) < (*min_pos)) {
        note->GetGraceAlignment()->SetXShift(overlap);
    }

    // the next minimal position if given by the right side of the bounding box + the spacing of the element
    (*min_pos) = note->GetGraceAlignment()->GetXRel() + note->m_contentBB_x2
        + doc->GetRightMargin(NOTE) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR;
    //(*min_pos) = note->GetGraceAlignment()->GetXRel() + note->m_contentBB_x2;
    // note->GetGraceAlignment()->SetMaxWidth(note->m_contentBB_x2 + doc->GetRightMargin(&typeid(*note)) *
    // doc->GetDrawingUnit(100) /
    // PARAM_DENOMINATOR);
    note->GetGraceAlignment()->SetMaxWidth(note->m_contentBB_x2);

    return FUNCTOR_CONTINUE;
}

int Object::SetBoundingBoxXShift(ArrayPtrVoid *params)
{
    // param 0: the minimum position (i.e., the width of the previous element)
    // param 1: the maximum width in the current measure
    // param 2: the Doc
    int *min_pos = static_cast<int *>((*params).at(0));
    int *measure_width = static_cast<int *>((*params).at(1));
    Doc *doc = static_cast<Doc *>((*params).at(2));
    Ligature *ligParent;

    // starting a new measure
    if (this->Is() == MEASURE) {
        Measure *current_measure = dynamic_cast<Measure *>(this);
        assert(current_measure);
        // we reset the measure width and the minimum position
        (*measure_width) = 0;
        (*min_pos) = 0;
        if (current_measure->GetLeftBarLineType() != BARRENDITION_NONE) {
            current_measure->GetLeftBarLine()->SetBoundingBoxXShift(params);
        }
        return FUNCTOR_CONTINUE;
    }

    // starting new layer
    if (this->Is() == LAYER) {
        Layer *current_layer = dynamic_cast<Layer *>(this);
        assert(current_layer);
        (*min_pos) = 0;
        // set scoreDef attr
        if (current_layer->GetDrawingClef()) {
            current_layer->GetDrawingClef()->SetBoundingBoxXShift(params);
        }
        if (current_layer->GetDrawingKeySig()) {
            current_layer->GetDrawingKeySig()->SetBoundingBoxXShift(params);
        }
        if (current_layer->GetDrawingMensur()) {
            current_layer->GetDrawingMensur()->SetBoundingBoxXShift(params);
        }
        if (current_layer->GetDrawingMeterSig()) {
            current_layer->GetDrawingMeterSig()->SetBoundingBoxXShift(params);
        }
        return FUNCTOR_CONTINUE;
    }

    if (!this->IsLayerElement()) {
        return FUNCTOR_CONTINUE;
    }

    LayerElement *current = dynamic_cast<LayerElement *>(this);
    assert(current);

    // we should have processed aligned before
    assert(current->GetAlignment());

    if (!current->HasToBeAligned()) {
        // if nothing to do with this type of element
        return FUNCTOR_CONTINUE;
    }

    if (!current->HasUpdatedBB()) {
        // if nothing was drawn, do not take it into account
        assert(false); // quite drastic but this should never happen. If nothing has to be drawn
        // then the BB should be set to empty with  Object::SetEmptyBB()
        return FUNCTOR_CONTINUE;
    }

    if ((current->Is() == NOTE) && current->GetFirstParent(CHORD, MAX_CHORD_DEPTH)) {
        return FUNCTOR_CONTINUE;
    }

    if ((current->Is() == ACCID) && current->GetFirstParent(NOTE, MAX_ACCID_DEPTH)) {
        return FUNCTOR_CONTINUE;
    }

    // The negative offset is the part of the bounding box that overflows on the left
    // |____x_____|
    //  ---- = negative offset
    int negative_offset = -(current->m_contentBB_x1);
    
    // Increase negative_offset by the symbol type's left margin, unless it's a note
    // that's part of a ligature.
    if (current->Is() == NOTE && !current->IsGraceNote()) {
        ligParent = dynamic_cast<Ligature *>(current->GetFirstParent(LIGATURE, 1));
        if (!ligParent)
            negative_offset += (doc->GetLeftMargin(current->Is()) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR);
    }

    // This should never happen but can with glyphs not exactly registered at x=0 in the SMuFL font used
    if (negative_offset < 0) negative_offset = 0;

    // with a grace note, also take into account the full width of the group given by the GraceAligner
    if (current->GetAlignment()->HasGraceAligner()) {
        negative_offset += current->GetAlignment()->GetGraceAligner()->GetWidth();
    }

    int currentX = current->GetAlignment()->GetXRel();
    // with grace note, take into account the position of the note in the grace group
    if (current->IsGraceNote()) {
        Note *note = dynamic_cast<Note *>(current);
        currentX += note->GetGraceAlignment()->GetXRel();
    }

    // check if the element overlaps with the preceeding one given by (*min_pos)
    int overlap = (*min_pos) - currentX + negative_offset;

    if ((currentX - negative_offset) < (*min_pos)) {
        current->GetAlignment()->SetXShift(overlap);
    }

    // do not adjust the min pos and the max width since this is already handled by
    // the GraceAligner
    if (current->IsGraceNote()) {
        (*min_pos) = current->GetAlignment()->GetXRel();
        current->GetAlignment()->SetMaxWidth(0);
        return FUNCTOR_CONTINUE;
    }

    // the next minimal position is given by the right side of the bounding box + the spacing of the element
    int width = current->m_contentBB_x2;

    // Move to right by the symbol type's right margin, unless it's a note that's
	// part of a ligature.
    if (!current->HasEmptyBB() && current->Is() == NOTE && !current->IsGraceNote()) {
        ligParent = dynamic_cast<Ligature *>(current->GetFirstParent(LIGATURE, 1));
        if (!ligParent)
        	width += doc->GetRightMargin(current->Is()) * doc->GetDrawingUnit(100) / PARAM_DENOMINATOR;
    }
    
    (*min_pos) = current->GetAlignment()->GetXRel() + width;
    current->GetAlignment()->SetMaxWidth(width);

    return FUNCTOR_CONTINUE;
}

int Object::SetBoundingBoxXShiftEnd(ArrayPtrVoid *params)
{
    // param 0: the minimum position (i.e., the width of the previous element)
    // param 1: the maximum width in the current measure
    int *min_pos = static_cast<int *>((*params).at(0));
    int *measure_width = static_cast<int *>((*params).at(1));

    // ending a measure
    if (this->Is() == MEASURE) {
        Measure *current_measure = dynamic_cast<Measure *>(this);
        assert(current_measure);
        // use the measure width as minimum position of the barLine
        (*min_pos) = (*measure_width);
        if (current_measure->GetRightBarLineType() != BARRENDITION_NONE) {
            current_measure->GetRightBarLine()->SetBoundingBoxXShift(params);
        }
        return FUNCTOR_CONTINUE;
    }

    // ending a layer
    if (this->Is() == LAYER) {
        // mininimum position is the with the layer
        // we keep it if it's higher than what we had so far
        // this will be used for shifting the right barLine
        (*measure_width) = std::max((*measure_width), (*min_pos));
        return FUNCTOR_CONTINUE;
    }

    return FUNCTOR_CONTINUE;
}

int Object::SetOverflowBBoxes(ArrayPtrVoid *params)
{
    // param 0: the maximum height in the current system
    StaffAlignment **staffAlignment = static_cast<StaffAlignment **>((*params).at(0));
    Doc *doc = static_cast<Doc *>((*params).at(1));

    // starting a new staff
    if (this->Is() == STAFF) {
        Staff *currentStaff = dynamic_cast<Staff *>(this);
        assert(currentStaff);
        assert(currentStaff->GetAlignment());

        (*staffAlignment) = currentStaff->GetAlignment();

        // currentStaff->GetAlignment()->SetMaxHeight(currentStaff->m_contentBB_y1);

        return FUNCTOR_CONTINUE;
    }

    // starting new layer
    if (this->Is() == LAYER) {
        Layer *currentLayer = dynamic_cast<Layer *>(this);
        assert(currentLayer);
        // set scoreDef attr
        if (currentLayer->GetDrawingClef()) {
            currentLayer->GetDrawingClef()->SetOverflowBBoxes(params);
        }
        if (currentLayer->GetDrawingKeySig()) {
            currentLayer->GetDrawingKeySig()->SetOverflowBBoxes(params);
        }
        if (currentLayer->GetDrawingMensur()) {
            currentLayer->GetDrawingMensur()->SetOverflowBBoxes(params);
        }
        if (currentLayer->GetDrawingMeterSig()) {
            currentLayer->GetDrawingMeterSig()->SetOverflowBBoxes(params);
        }
        return FUNCTOR_CONTINUE;
    }

    if (this->IsFloatingElement()) {
        return FUNCTOR_CONTINUE;
    }

    if (!this->IsLayerElement()) {
        return FUNCTOR_CONTINUE;
    }

    if (this->Is() == SYL) {
        // We don't want to add the syl to the overflow since lyrics require a full line anyway
        return FUNCTOR_CONTINUE;
    }

    LayerElement *current = dynamic_cast<LayerElement *>(this);
    assert(current);

    if (!current->HasToBeAligned()) {
        // if nothing to do with this type of element
        // return FUNCTOR_CONTINUE;
    }

    if (!current->HasUpdatedBB()) {
        // if nothing was drawn, do not take it into account
        // assert(false); // quite drastic but this should never happen. If nothing has to be drawn
        // LogDebug("Un-updated bounding box for '%s' '%s'", current->GetClassName().c_str(),
        // current->GetUuid().c_str());
        // then the BB should be set to empty with  Object::SetEmptyBB()
        return FUNCTOR_CONTINUE;
    }

    int staffSize = (*staffAlignment)->GetStaffSize();

    int overflowAbove = (*staffAlignment)->CalcOverflowAbove(current);
    if (overflowAbove > doc->GetDrawingStaffLineWidth(staffSize) / 2) {
        // LogMessage("%s top overflow: %d", current->GetUuid().c_str(), overflowAbove);
        (*staffAlignment)->SetOverflowAbove(overflowAbove);
        (*staffAlignment)->AddBBoxAbove(current);
    }

    int overflowBelow = (*staffAlignment)->CalcOverflowBelow(current);
    if (overflowBelow > doc->GetDrawingStaffLineWidth(staffSize) / 2) {
        // LogMessage("%s bottom overflow: %d", current->GetUuid().c_str(), overflowBelow);
        (*staffAlignment)->SetOverflowBelow(overflowBelow);
        (*staffAlignment)->AddBBoxBelow(current);
    }

    // do not go further down the tree in this case since the bounding box of the first element is already taken into
    // account?
    return FUNCTOR_CONTINUE;
}

int Object::Save(ArrayPtrVoid *params)
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream *>((*params).at(0));
    if (!output->WriteObject(this)) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
}

int Object::SaveEnd(ArrayPtrVoid *params)
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream *>((*params).at(0));
    if (!output->WriteObjectEnd(this)) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
