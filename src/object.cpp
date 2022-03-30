/////////////////////////////////////////////////////////////////////////////
// Name:        object.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "object.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <climits>
#include <iostream>
#include <math.h>
#include <sstream>

//----------------------------------------------------------------------------

#include "chord.h"
#include "clef.h"
#include "comparison.h"
#include "dir.h"
#include "doc.h"
#include "dynam.h"
#include "editorial.h"
#include "featureextractor.h"
#include "functorparams.h"
#include "io.h"
#include "keysig.h"
#include "layer.h"
#include "linkinginterface.h"
#include "mdiv.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "nc.h"
#include "note.h"
#include "page.h"
#include "plistinterface.h"
#include "score.h"
#include "staff.h"
#include "staffdef.h"
#include "surface.h"
#include "syl.h"
#include "syllable.h"
#include "system.h"
#include "systemmilestone.h"
#include "tempo.h"
#include "text.h"
#include "textelement.h"
#include "tuning.h"
#include "vrv.h"
#include "zone.h"

namespace vrv {

//----------------------------------------------------------------------------
// Object
//----------------------------------------------------------------------------

thread_local unsigned long Object::s_objectCounter = 0;

Object::Object() : BoundingBox()
{
    Init(OBJECT, "m-");
    if (s_objectCounter++ == 0) {
        SeedUuid();
    }
}

Object::Object(ClassId classId) : BoundingBox()
{
    Init(classId, "m-");
    if (s_objectCounter++ == 0) {
        SeedUuid();
    }
}

Object::Object(ClassId classId, const std::string &classIdStr) : BoundingBox()
{
    Init(classId, classIdStr);
    if (s_objectCounter++ == 0) {
        SeedUuid();
    }
}

Object *Object::Clone() const
{
    // This should never happen because the method should be overridden
    assert(false);
    return NULL;
}

Object::Object(const Object &object) : BoundingBox(object)
{
    this->ResetBoundingBox(); // It does not make sense to keep the values of the BBox

    m_classId = object.m_classId;
    m_classIdStr = object.m_classIdStr;
    m_parent = NULL;

    // Flags
    m_isAttribute = object.m_isAttribute;
    m_isModified = true;
    m_isReferenceObject = object.m_isReferenceObject;

    // Also copy attribute classes
    m_attClasses = object.m_attClasses;
    m_interfaces = object.m_interfaces;
    // New uuid
    this->GenerateUuid();
    // For now do not copy them
    // m_unsupported = object.m_unsupported;

    if (!object.CopyChildren()) {
        return;
    }

    int i;
    for (i = 0; i < (int)object.m_children.size(); ++i) {
        Object *current = object.m_children.at(i);
        Object *clone = current->Clone();
        if (clone) {
            clone->SetParent(this);
            clone->CloneReset();
            m_children.push_back(clone);
        }
    }
}

void Object::CloneReset()
{
    this->Modify();
    FunctorParams voidParams;
    this->ResetDrawing(&voidParams);
}

Object &Object::operator=(const Object &object)
{
    // not self assignement
    if (this != &object) {
        ClearChildren();
        this->ResetBoundingBox(); // It does not make sense to keep the values of the BBox

        m_classId = object.m_classId;
        m_classIdStr = object.m_classIdStr;
        m_parent = NULL;
        // Flags
        m_isAttribute = object.m_isAttribute;
        m_isModified = true;
        m_isReferenceObject = object.m_isReferenceObject;

        // Also copy attribute classes
        m_attClasses = object.m_attClasses;
        m_interfaces = object.m_interfaces;
        // New uuid
        this->GenerateUuid();
        // For now do now copy them
        // m_unsupported = object.m_unsupported;

        if (object.CopyChildren()) {
            int i;
            for (i = 0; i < (int)object.m_children.size(); ++i) {
                Object *current = object.m_children.at(i);
                Object *clone = current->Clone();
                if (clone) {
                    clone->SetParent(this);
                    clone->CloneReset();
                    m_children.push_back(clone);
                }
            }
        }
    }
    return *this;
}

Object::~Object()
{
    ClearChildren();
}

void Object::Init(ClassId classId, const std::string &classIdStr)
{
    assert(classIdStr.size());

    m_classId = classId;
    m_classIdStr = classIdStr;
    m_parent = NULL;
    // Flags
    m_isAttribute = false;
    m_isModified = true;
    m_isReferenceObject = false;
    // Comments
    m_comment = "";
    m_closingComment = "";

    this->GenerateUuid();

    this->Reset();
}

void Object::SetAsReferenceObject()
{
    assert(m_children.empty());

    m_isReferenceObject = true;
}

void Object::Reset()
{
    ClearChildren();
    this->ResetBoundingBox();
}

void Object::RegisterInterface(std::vector<AttClassId> *attClasses, InterfaceId interfaceId)
{
    m_attClasses.insert(m_attClasses.end(), attClasses->begin(), attClasses->end());
    m_interfaces.push_back(interfaceId);
}

bool Object::IsMilestoneElement()
{
    if (this->IsEditorialElement() || this->Is(ENDING) || this->Is(SECTION)) {
        SystemMilestoneInterface *interface = dynamic_cast<SystemMilestoneInterface *>(this);
        assert(interface);
        return (interface->IsSystemMilestone());
    }
    else if (this->Is(MDIV) || this->Is(SCORE)) {
        PageMilestoneInterface *interface = dynamic_cast<PageMilestoneInterface *>(this);
        assert(interface);
        return (interface->IsPageMilestone());
    }
    return false;
}

Object *Object::GetMilestoneEnd()
{
    if (this->IsEditorialElement() || this->Is(ENDING) || this->Is(SECTION)) {
        SystemMilestoneInterface *interface = dynamic_cast<SystemMilestoneInterface *>(this);
        assert(interface);
        return (interface->GetEnd());
    }
    else if (this->Is(MDIV) || this->Is(SCORE)) {
        PageMilestoneInterface *interface = dynamic_cast<PageMilestoneInterface *>(this);
        assert(interface);
        return (interface->GetEnd());
    }
    return NULL;
}

void Object::MoveChildrenFrom(Object *sourceParent, int idx, bool allowTypeChange)
{
    if (this == sourceParent) {
        assert("Object cannot be copied to itself");
    }
    if (!allowTypeChange && (m_classId != sourceParent->m_classId)) {
        assert("Object must be of the same type");
    }

    int i;
    for (i = 0; i < (int)sourceParent->m_children.size(); ++i) {
        Object *child = sourceParent->Relinquish(i);
        child->SetParent(this);
        if (idx != -1) {
            this->InsertChild(child, idx);
            idx++;
        }
        else {
            m_children.push_back(child);
        }
    }
}

void Object::ReplaceChild(Object *currentChild, Object *replacingChild)
{
    assert(this->GetChildIndex(currentChild) != -1);
    assert(this->GetChildIndex(replacingChild) == -1);

    int idx = this->GetChildIndex(currentChild);
    currentChild->ResetParent();
    m_children.at(idx) = replacingChild;
    replacingChild->SetParent(this);
    this->Modify();
}

void Object::InsertBefore(Object *child, Object *newChild)
{
    assert(this->GetChildIndex(child) != -1);
    assert(this->GetChildIndex(newChild) == -1);

    int idx = this->GetChildIndex(child);
    newChild->SetParent(this);
    this->InsertChild(newChild, idx);

    this->Modify();
}

void Object::InsertAfter(Object *child, Object *newChild)
{
    assert(this->GetChildIndex(child) != -1);
    assert(this->GetChildIndex(newChild) == -1);

    int idx = this->GetChildIndex(child);
    newChild->SetParent(this);
    this->InsertChild(newChild, idx + 1);

    this->Modify();
}

void Object::SortChildren(Object::binaryComp comp)
{
    std::stable_sort(m_children.begin(), m_children.end(), comp);
    this->Modify();
}

void Object::MoveItselfTo(Object *targetParent)
{
    assert(targetParent);
    assert(m_parent);
    assert(m_parent != targetParent);

    Object *relinquishedObject = this->GetParent()->Relinquish(this->GetIdx());
    assert(relinquishedObject && (relinquishedObject == this));
    targetParent->AddChild(relinquishedObject);
}

void Object::SetUuid(std::string uuid)
{
    m_uuid = uuid;
}

void Object::SwapUuid(Object *other)
{
    assert(other);
    std::string swapUuid = this->GetUuid();
    this->SetUuid(other->GetUuid());
    other->SetUuid(swapUuid);
}

void Object::ClearChildren()
{
    if (m_isReferenceObject) {
        m_children.clear();
        return;
    }

    ArrayOfObjects::iterator iter;
    for (iter = m_children.begin(); iter != m_children.end(); ++iter) {
        // we need to check if this is the parent
        // ownership might have been given up with Relinquish
        if ((*iter)->GetParent() == this) {
            delete *iter;
        }
    }
    m_children.clear();
}

int Object::GetChildCount(const ClassId classId) const
{
    return (int)count_if(m_children.begin(), m_children.end(), ObjectComparison(classId));
}

int Object::GetChildCount(const ClassId classId, int depth) const
{
    ListOfConstObjects objects = this->FindAllDescendantsByType(classId, true, depth);
    return (int)objects.size();
}

int Object::GetDescendantCount(const ClassId classId) const
{
    ListOfConstObjects objects = this->FindAllDescendantsByType(classId);
    return (int)objects.size();
}

int Object::GetAttributes(ArrayOfStrAttr *attributes) const
{
    assert(attributes);
    attributes->clear();

    Att::GetAnalytical(this, attributes);
    Att::GetCmn(this, attributes);
    Att::GetCmnornaments(this, attributes);
    Att::GetCritapp(this, attributes);
    // Att::GetEdittrans(this, attributes);
    Att::GetExternalsymbols(this, attributes);
    Att::GetFrettab(this, attributes);
    Att::GetFacsimile(this, attributes);
    // Att::GetFigtable(this, attributes);
    // Att::GetFingering(this, attributes);
    Att::GetGestural(this, attributes);
    // Att::GetHarmony(this, attributes);
    // Att::GetHeader(this, attributes);
    Att::GetMei(this, attributes);
    Att::GetMensural(this, attributes);
    Att::GetMidi(this, attributes);
    Att::GetNeumes(this, attributes);
    Att::GetPagebased(this, attributes);
    // Att::GetPerformance(this, attributes);
    Att::GetShared(this, attributes);
    // Att::GetUsersymbols(this, attributes);
    Att::GetVisual(this, attributes);

    for (auto &pair : m_unsupported) {
        attributes->push_back({ pair.first, pair.second });
    }

    return (int)attributes->size();
}

bool Object::HasAttribute(std::string attribute, std::string value) const
{
    ArrayOfStrAttr attributes;
    this->GetAttributes(&attributes);
    ArrayOfStrAttr::iterator iter;
    for (iter = attributes.begin(); iter != attributes.end(); ++iter) {
        if (((*iter).first == attribute) && ((*iter).second == value)) return true;
    }
    return false;
}

Object *Object::GetFirst(const ClassId classId)
{
    return const_cast<Object *>(std::as_const(*this).GetFirst(classId));
}

const Object *Object::GetFirst(const ClassId classId) const
{
    m_iteratorElementType = classId;
    m_iteratorEnd = m_children.end();
    m_iteratorCurrent = std::find_if(m_children.begin(), m_iteratorEnd, ObjectComparison(m_iteratorElementType));
    return (m_iteratorCurrent == m_iteratorEnd) ? NULL : *m_iteratorCurrent;
}

Object *Object::GetNext()
{
    return const_cast<Object *>(std::as_const(*this).GetNext());
}

const Object *Object::GetNext() const
{
    ++m_iteratorCurrent;
    m_iteratorCurrent = std::find_if(m_iteratorCurrent, m_iteratorEnd, ObjectComparison(m_iteratorElementType));
    return (m_iteratorCurrent == m_iteratorEnd) ? NULL : *m_iteratorCurrent;
}

Object *Object::GetNext(const Object *child, const ClassId classId)
{
    return const_cast<Object *>(std::as_const(*this).GetNext(child, classId));
}

const Object *Object::GetNext(const Object *child, const ClassId classId) const
{
    ArrayOfObjects::const_iterator iteratorEnd, iteratorCurrent;
    iteratorEnd = m_children.end();
    iteratorCurrent = std::find(m_children.begin(), iteratorEnd, child);
    if (iteratorCurrent != iteratorEnd) {
        ++iteratorCurrent;
        iteratorCurrent = std::find_if(iteratorCurrent, iteratorEnd, ObjectComparison(classId));
    }
    return (iteratorCurrent == iteratorEnd) ? NULL : *iteratorCurrent;
}

Object *Object::GetPrevious(const Object *child, const ClassId classId)
{
    return const_cast<Object *>(std::as_const(*this).GetPrevious(child, classId));
}

const Object *Object::GetPrevious(const Object *child, const ClassId classId) const
{
    ArrayOfObjects::const_reverse_iterator riteratorEnd, riteratorCurrent;
    riteratorEnd = m_children.rend();
    riteratorCurrent = std::find(m_children.rbegin(), riteratorEnd, child);
    if (riteratorCurrent != riteratorEnd) {
        ++riteratorCurrent;
        riteratorCurrent = std::find_if(riteratorCurrent, riteratorEnd, ObjectComparison(classId));
    }
    return (riteratorCurrent == riteratorEnd) ? NULL : *riteratorCurrent;
}

Object *Object::GetLast(const ClassId classId)
{
    return const_cast<Object *>(std::as_const(*this).GetLast(classId));
}

const Object *Object::GetLast(const ClassId classId) const
{
    ArrayOfObjects::const_reverse_iterator riter
        = std::find_if(m_children.rbegin(), m_children.rend(), ObjectComparison(classId));
    return (riter == m_children.rend()) ? NULL : *riter;
}

int Object::GetIdx() const
{
    assert(m_parent);

    return m_parent->GetChildIndex(this);
}

void Object::InsertChild(Object *element, int idx)
{
    // With this method we require the parent to be set before
    assert(element->GetParent() == this);

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
    child->ResetParent();
    ArrayOfObjects::iterator iter = m_children.begin();
    m_children.erase(iter + (idx));
    return child;
}

bool Object::HasDescendant(Object *child, int deepness) const
{
    ArrayOfObjects::const_iterator iter;

    for (iter = m_children.begin(); iter != m_children.end(); ++iter) {
        if (child == (*iter))
            return true;
        else if (deepness == 0)
            return false;
        else if ((*iter)->HasDescendant(child, deepness - 1))
            return true;
    }

    return false;
}

Object *Object::Relinquish(int idx)
{
    if (idx >= (int)m_children.size()) {
        return NULL;
    }
    Object *child = m_children.at(idx);
    child->ResetParent();
    return child;
}

void Object::ClearRelinquishedChildren()
{
    ArrayOfObjects::iterator iter;
    for (iter = m_children.begin(); iter != m_children.end();) {
        if ((*iter)->GetParent() != this) {
            iter = m_children.erase(iter);
        }
        else
            ++iter;
    }
}

Object *Object::FindDescendantByUuid(const std::string &uuid, int deepness, bool direction)
{
    return const_cast<Object *>(std::as_const(*this).FindDescendantByUuid(uuid, deepness, direction));
}

const Object *Object::FindDescendantByUuid(const std::string &uuid, int deepness, bool direction) const
{
    Functor findByUuid(&Object::FindByUuid);
    FindByUuidParams findbyUuidParams;
    findbyUuidParams.m_uuid = uuid;
    this->Process(&findByUuid, &findbyUuidParams, NULL, NULL, deepness, direction, true);
    return findbyUuidParams.m_element;
}

Object *Object::FindDescendantByType(ClassId classId, int deepness, bool direction)
{
    return const_cast<Object *>(std::as_const(*this).FindDescendantByType(classId, deepness, direction));
}

const Object *Object::FindDescendantByType(ClassId classId, int deepness, bool direction) const
{
    ClassIdComparison comparison(classId);
    return this->FindDescendantByComparison(&comparison, deepness, direction);
}

Object *Object::FindDescendantByComparison(Comparison *comparison, int deepness, bool direction)
{
    return const_cast<Object *>(std::as_const(*this).FindDescendantByComparison(comparison, deepness, direction));
}

const Object *Object::FindDescendantByComparison(Comparison *comparison, int deepness, bool direction) const
{
    Functor findByComparison(&Object::FindByComparison);
    FindByComparisonParams findByComparisonParams(comparison);
    this->Process(&findByComparison, &findByComparisonParams, NULL, NULL, deepness, direction, true);
    return findByComparisonParams.m_element;
}

Object *Object::FindDescendantExtremeByComparison(Comparison *comparison, int deepness, bool direction)
{
    return const_cast<Object *>(
        std::as_const(*this).FindDescendantExtremeByComparison(comparison, deepness, direction));
}

const Object *Object::FindDescendantExtremeByComparison(Comparison *comparison, int deepness, bool direction) const
{
    Functor findExtremeByComparison(&Object::FindExtremeByComparison);
    FindExtremeByComparisonParams findExtremeByComparisonParams(comparison);
    this->Process(&findExtremeByComparison, &findExtremeByComparisonParams, NULL, NULL, deepness, direction, true);
    return findExtremeByComparisonParams.m_element;
}

ListOfObjects Object::FindAllDescendantsByType(ClassId classId, bool continueDepthSearchForMatches, int deepness)
{
    ListOfObjects objects;
    ClassIdComparison comparison(classId);
    Functor findAllByComparison(&Object::FindAllByComparison);
    FindAllByComparisonParams findAllByComparisonParams(&comparison, &objects);
    findAllByComparisonParams.m_continueDepthSearchForMatches = continueDepthSearchForMatches;
    this->Process(&findAllByComparison, &findAllByComparisonParams, NULL, NULL, deepness);
    return objects;
}

ListOfConstObjects Object::FindAllDescendantsByType(
    ClassId classId, bool continueDepthSearchForMatches, int deepness) const
{
    ListOfConstObjects objects;
    ClassIdComparison comparison(classId);
    Functor findAllConstByComparison(&Object::FindAllConstByComparison);
    FindAllConstByComparisonParams findAllConstByComparisonParams(&comparison, &objects);
    findAllConstByComparisonParams.m_continueDepthSearchForMatches = continueDepthSearchForMatches;
    this->Process(&findAllConstByComparison, &findAllConstByComparisonParams, NULL, NULL, deepness);
    return objects;
}

void Object::FindAllDescendantsByComparison(
    ListOfObjects *objects, Comparison *comparison, int deepness, bool direction, bool clear)
{
    assert(objects);
    if (clear) objects->clear();

    Functor findAllByComparison(&Object::FindAllByComparison);
    FindAllByComparisonParams findAllByComparisonParams(comparison, objects);
    this->Process(&findAllByComparison, &findAllByComparisonParams, NULL, NULL, deepness, direction, true);
}

void Object::FindAllDescendantsByComparison(
    ListOfConstObjects *objects, Comparison *comparison, int deepness, bool direction, bool clear) const
{
    assert(objects);
    if (clear) objects->clear();

    Functor findAllConstByComparison(&Object::FindAllConstByComparison);
    FindAllConstByComparisonParams findAllConstByComparisonParams(comparison, objects);
    this->Process(&findAllConstByComparison, &findAllConstByComparisonParams, NULL, NULL, deepness, direction, true);
}

void Object::FindAllDescendantsBetween(
    ListOfObjects *objects, Comparison *comparison, const Object *start, const Object *end, bool clear, int depth)
{
    assert(objects);
    if (clear) objects->clear();

    Functor findAllBetween(&Object::FindAllBetween);
    FindAllBetweenParams findAllBetweenParams(comparison, objects, start, end);
    this->Process(&findAllBetween, &findAllBetweenParams, NULL, NULL, depth, FORWARD, true);
}

void Object::FindAllDescendantsBetween(ListOfConstObjects *objects, Comparison *comparison, const Object *start,
    const Object *end, bool clear, int depth) const
{
    assert(objects);
    if (clear) objects->clear();

    Functor findAllConstBetween(&Object::FindAllConstBetween);
    FindAllConstBetweenParams findAllConstBetweenParams(comparison, objects, start, end);
    this->Process(&findAllConstBetween, &findAllConstBetweenParams, NULL, NULL, depth, FORWARD, true);
}

Object *Object::GetChild(int idx)
{
    return const_cast<Object *>(std::as_const(*this).GetChild(idx));
}

const Object *Object::GetChild(int idx) const
{
    if ((idx < 0) || (idx >= (int)m_children.size())) {
        return NULL;
    }
    return m_children.at(idx);
}

Object *Object::GetChild(int idx, const ClassId classId)
{
    return const_cast<Object *>(std::as_const(*this).GetChild(idx, classId));
}

const Object *Object::GetChild(int idx, const ClassId classId) const
{
    ListOfConstObjects objects = this->FindAllDescendantsByType(classId, true, 1);
    if ((idx < 0) || (idx >= (int)objects.size())) {
        return NULL;
    }
    ListOfConstObjects::iterator it = objects.begin();
    std::advance(it, idx);
    return *it;
}

ArrayOfConstObjects Object::GetChildren() const
{
    return ArrayOfConstObjects(m_children.begin(), m_children.end());
}

bool Object::DeleteChild(Object *child)
{
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        m_children.erase(it);
        if (!m_isReferenceObject) {
            delete child;
        }
        this->Modify();
        return true;
    }
    else {
        assert(false);
        return false;
    }
}

void Object::GenerateUuid()
{
    m_uuid = m_classIdStr.at(0) + Object::GenerateRandUuid();
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

bool Object::IsSupportedChild(Object *child)
{
    // This should never happen because the method should be overridden
    LogDebug(
        "Method for adding %s to %s should be overridden", child->GetClassName().c_str(), this->GetClassName().c_str());
    // assert(false);
    return false;
}

void Object::AddChild(Object *child)
{
    if (!((child->GetClassName() == "Staff") && (this->GetClassName() == "Section"))) {
        // temporarily allowing staff in section for issue https://github.com/MeasuringPolyphony/mp_editor/issues/62
        if (!this->IsSupportedChild(child)) {
            LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
            return;
        }
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

int Object::GetDrawingX() const
{
    assert(m_parent);
    return m_parent->GetDrawingX();
}

int Object::GetDrawingY() const
{
    assert(m_parent);
    return m_parent->GetDrawingY();
}

void Object::ResetCachedDrawingX() const
{
    // if (m_cachedDrawingX == VRV_UNSET) return;
    m_cachedDrawingX = VRV_UNSET;
    ArrayOfObjects::const_iterator iter;
    for (iter = m_children.begin(); iter != m_children.end(); ++iter) {
        (*iter)->ResetCachedDrawingX();
    }
}

void Object::ResetCachedDrawingY() const
{
    // if (m_cachedDrawingY == VRV_UNSET) return;
    m_cachedDrawingY = VRV_UNSET;
    ArrayOfObjects::const_iterator iter;
    for (iter = m_children.begin(); iter != m_children.end(); ++iter) {
        (*iter)->ResetCachedDrawingY();
    }
}

int Object::GetChildIndex(const Object *child) const
{
    ArrayOfObjects::const_iterator iter;
    int i;
    for (iter = m_children.begin(), i = 0; iter != m_children.end(); ++iter, ++i) {
        if (child == *iter) {
            return i;
        }
    }
    return -1;
}

int Object::GetDescendantIndex(const Object *child, const ClassId classId, int depth)
{
    ListOfObjects objects = this->FindAllDescendantsByType(classId, true, depth);
    int i = 0;
    for (auto &object : objects) {
        if (child == object) return i;
        ++i;
    }
    return -1;
}

void Object::Modify(bool modified)
{
    // if we have a parent and a new modification, propagate it
    if (m_parent && modified) {
        m_parent->Modify();
    }
    m_isModified = modified;
}

void Object::FillFlatList(ArrayOfObjects *flatList)
{
    Functor addToFlatList(&Object::AddLayerElementToFlatList);
    AddLayerElementToFlatListParams addLayerElementToFlatListParams(flatList);
    this->Process(&addToFlatList, &addLayerElementToFlatListParams);
}

ListOfObjects Object::GetAncestors()
{
    ListOfObjects ancestors;
    Object *object = m_parent;
    while (object) {
        ancestors.push_back(object);
        object = object->m_parent;
    }
    return ancestors;
}

ListOfConstObjects Object::GetAncestors() const
{
    ListOfConstObjects ancestors;
    const Object *object = m_parent;
    while (object) {
        ancestors.push_back(object);
        object = object->m_parent;
    }
    return ancestors;
}

Object *Object::GetFirstAncestor(const ClassId classId, int maxDepth)
{
    return const_cast<Object *>(std::as_const(*this).GetFirstAncestor(classId, maxDepth));
}

const Object *Object::GetFirstAncestor(const ClassId classId, int maxDepth) const
{
    if ((maxDepth == 0) || !m_parent) {
        return NULL;
    }

    if (m_parent->m_classId == classId) {
        return m_parent;
    }
    else {
        return (m_parent->GetFirstAncestor(classId, maxDepth - 1));
    }
}

Object *Object::GetFirstAncestorInRange(const ClassId classIdMin, const ClassId classIdMax, int maxDepth)
{
    return const_cast<Object *>(std::as_const(*this).GetFirstAncestorInRange(classIdMin, classIdMax, maxDepth));
}

const Object *Object::GetFirstAncestorInRange(const ClassId classIdMin, const ClassId classIdMax, int maxDepth) const
{
    if ((maxDepth == 0) || !m_parent) {
        return NULL;
    }

    if ((m_parent->m_classId > classIdMin) && (m_parent->m_classId < classIdMax)) {
        return m_parent;
    }
    else {
        return (m_parent->GetFirstAncestorInRange(classIdMin, classIdMax, maxDepth - 1));
    }
}

Object *Object::GetLastAncestorNot(const ClassId classId, int maxDepth)
{
    if ((maxDepth == 0) || !m_parent) {
        return NULL;
    }

    if (m_parent->m_classId == classId) {
        return this;
    }
    else {
        return (m_parent->GetLastAncestorNot(classId, maxDepth - 1));
    }
}

Object *Object::GetFirstChildNot(const ClassId classId)
{
    for (const auto child : m_children) {
        if (!child->Is(classId)) {
            return child;
        }
    }

    return NULL;
}

bool Object::HasEditorialContent()
{
    ListOfObjects editorial;
    IsEditorialElementComparison editorialComparison;
    this->FindAllDescendantsByComparison(&editorial, &editorialComparison);
    return (!editorial.empty());
}

bool Object::HasNonEditorialContent()
{
    ListOfObjects nonEditorial;
    IsEditorialElementComparison editorialComparison;
    editorialComparison.ReverseComparison();
    this->FindAllDescendantsByComparison(&nonEditorial, &editorialComparison);
    return (!nonEditorial.empty());
}

void Object::Process(Functor *functor, FunctorParams *functorParams, Functor *endFunctor, ArrayOfComparisons *filters,
    int deepness, bool direction, bool skipFirst)
{
    if (functor->m_returnCode == FUNCTOR_STOP) {
        return;
    }

    // Update the current score stored in the document
    this->UpdateDocumentScore(direction);

    if (!skipFirst) {
        functor->Call(this, functorParams);
    }

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

    if (!this->SkipChildren(functor)) {
        // We need a pointer to the array for the option to work on a reversed copy
        ArrayOfObjects *children = &m_children;
        if (direction == BACKWARD) {
            for (ArrayOfObjects::reverse_iterator iter = children->rbegin(); iter != children->rend(); ++iter) {
                // we will end here if there is no filter at all or for the current child type
                if (this->FiltersApply(filters, *iter)) {
                    (*iter)->Process(functor, functorParams, endFunctor, filters, deepness, direction);
                }
            }
        }
        else {
            for (ArrayOfObjects::iterator iter = children->begin(); iter != children->end(); ++iter) {
                // we will end here if there is no filter at all or for the current child type
                if (this->FiltersApply(filters, *iter)) {
                    (*iter)->Process(functor, functorParams, endFunctor, filters, deepness, direction);
                }
            }
        }
    }

    if (endFunctor && !skipFirst) {
        endFunctor->Call(this, functorParams);
    }
}

void Object::Process(Functor *functor, FunctorParams *functorParams, Functor *endFunctor, ArrayOfComparisons *filters,
    int deepness, bool direction, bool skipFirst) const
{
    if (functor->m_returnCode == FUNCTOR_STOP) {
        return;
    }

    // Update the current score stored in the document
    const_cast<Object *>(this)->UpdateDocumentScore(direction);

    if (!skipFirst) {
        functor->Call(this, functorParams);
    }

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

    if (!this->SkipChildren(functor)) {
        // We need a pointer to the array for the option to work on a reversed copy
        const ArrayOfObjects *children = &m_children;
        if (direction == BACKWARD) {
            for (ArrayOfObjects::const_reverse_iterator iter = children->rbegin(); iter != children->rend(); ++iter) {
                // we will end here if there is no filter at all or for the current child type
                if (this->FiltersApply(filters, *iter)) {
                    (*iter)->Process(functor, functorParams, endFunctor, filters, deepness, direction);
                }
            }
        }
        else {
            for (ArrayOfObjects::const_iterator iter = children->begin(); iter != children->end(); ++iter) {
                // we will end here if there is no filter at all or for the current child type
                if (this->FiltersApply(filters, *iter)) {
                    (*iter)->Process(functor, functorParams, endFunctor, filters, deepness, direction);
                }
            }
        }
    }

    if (endFunctor && !skipFirst) {
        endFunctor->Call(this, functorParams);
    }
}

void Object::UpdateDocumentScore(bool direction)
{
    // When we are starting a new score, we need to update the current score in the document
    if (direction == FORWARD && this->Is(SCORE)) {
        Score *score = vrv_cast<Score *>(this);
        assert(score);
        score->SetAsCurrent();
    }
    // We need to do the same in backward direction through the PageMilestoneEnd::m_start
    else if (direction == BACKWARD && this->Is(PAGE_MILESTONE_END)) {
        PageMilestoneEnd *elementEnd = vrv_cast<PageMilestoneEnd *>(this);
        assert(elementEnd);
        if (elementEnd->GetStart() && elementEnd->GetStart()->Is(SCORE)) {
            Score *score = vrv_cast<Score *>(elementEnd->GetStart());
            assert(score);
            score->SetAsCurrent();
        }
    }
}

bool Object::SkipChildren(Functor *functor) const
{
    if (functor->m_visibleOnly) {
        if (this->IsEditorialElement()) {
            const EditorialElement *editorialElement = vrv_cast<const EditorialElement *>(this);
            assert(editorialElement);
            if (editorialElement->m_visibility == Hidden) {
                return true;
            }
        }
        else if (this->Is(MDIV)) {
            const Mdiv *mdiv = vrv_cast<const Mdiv *>(this);
            assert(mdiv);
            if (mdiv->m_visibility == Hidden) {
                return true;
            }
        }
        else if (this->IsSystemElement()) {
            const SystemElement *systemElement = vrv_cast<const SystemElement *>(this);
            assert(systemElement);
            if (systemElement->m_visibility == Hidden) {
                return true;
            }
        }
    }
    return false;
}

bool Object::FiltersApply(const ArrayOfComparisons *filters, Object *object) const
{
    if (filters) {
        return std::all_of(filters->begin(), filters->end(), [object](Comparison *iter) {
            // ignore any class comparison which does not match the object class
            ClassIdComparison *cmp = dynamic_cast<ClassIdComparison *>(iter);
            if (!cmp || (cmp->GetType() != object->m_classId)) {
                return true;
            }
            return (*iter)(object);
        });
    }
    return true;
}

int Object::Save(Output *output)
{
    SaveParams saveParams(output);

    Functor save(&Object::Save);
    // Special case where we want to process all objects
    save.m_visibleOnly = false;
    Functor saveEnd(&Object::SaveEnd);
    this->Process(&save, &saveParams, &saveEnd);

    return true;
}

void Object::ReorderByXPos()
{
    ReorderByXPosParams params;
    Functor reorder(&Object::ReorderByXPos);
    this->Process(&reorder, &params);
}

Object *Object::FindNextChild(Comparison *comp, Object *start)
{
    Functor findNextChildByComparison(&Object::FindNextChildByComparison);
    FindChildByComparisonParams params(comp, start);
    this->Process(&findNextChildByComparison, &params);
    return params.m_element;
}

Object *Object::FindPreviousChild(Comparison *comp, Object *start)
{
    Functor findPreviousChildByComparison(&Object::FindPreviousChildByComparison);
    FindChildByComparisonParams params(comp, start);
    this->Process(&findPreviousChildByComparison, &params);
    return params.m_element;
}

//----------------------------------------------------------------------------
// Static methods for Object
//----------------------------------------------------------------------------

void Object::SeedUuid(unsigned int seed)
{
    // Init random number generator for uuids
    if (seed == 0) {
        std::srand((unsigned int)std::time(0));
    }
    else {
        std::srand(seed);
    }
}

std::string Object::GenerateRandUuid()
{
    int nr = std::rand();

    // char str[17];
    // snprintf(str, 17, "%016d", nr);
    // return std::string(str);

    return BaseEncodeInt(nr, 36);
}

bool Object::sortByUlx(Object *a, Object *b)
{
    FacsimileInterface *fa = NULL, *fb = NULL;
    InterfaceComparison comp(INTERFACE_FACSIMILE);
    if (a->GetFacsimileInterface() && a->GetFacsimileInterface()->HasFacs())
        fa = a->GetFacsimileInterface();
    else {
        ListOfObjects children;
        a->FindAllDescendantsByComparison(&children, &comp);
        for (auto it = children.begin(); it != children.end(); ++it) {
            if ((*it)->Is(SYL)) continue;
            FacsimileInterface *temp = (*it)->GetFacsimileInterface();
            assert(temp);
            if (temp->HasFacs() && (fa == NULL || temp->GetZone()->GetUlx() < fa->GetZone()->GetUlx())) {
                fa = temp;
            }
        }
    }
    if (b->GetFacsimileInterface() && b->GetFacsimileInterface()->HasFacs())
        fb = b->GetFacsimileInterface();
    else {
        ListOfObjects children;
        b->FindAllDescendantsByComparison(&children, &comp);
        for (auto it = children.begin(); it != children.end(); ++it) {
            if ((*it)->Is(SYL)) continue;
            FacsimileInterface *temp = (*it)->GetFacsimileInterface();
            assert(temp);
            if (temp->HasFacs() && (fb == NULL || temp->GetZone()->GetUlx() < fb->GetZone()->GetUlx())) {
                fb = temp;
            }
        }
    }

    // Preserve ordering of neume components in ligature
    if (a->Is(NC) && b->Is(NC)) {
        Nc *nca = dynamic_cast<Nc *>(a);
        Nc *ncb = dynamic_cast<Nc *>(b);
        if (nca->HasLigated() && ncb->HasLigated() && (a->GetParent() == b->GetParent())) {
            Object *parent = a->GetParent();
            assert(parent);
            if (abs(parent->GetChildIndex(a) - parent->GetChildIndex(b)) == 1) {
                // Return nc with higher pitch
                return nca->PitchDifferenceTo(ncb) > 0; // If object a has the higher pitch
            }
        }
    }

    if (fa == NULL || fb == NULL) {
        if (fa == NULL) {
            LogMessage("No available facsimile interface for %s", a->GetUuid().c_str());
        }
        if (fb == NULL) {
            LogMessage("No available facsimile interface for %s", b->GetUuid().c_str());
        }
        return false;
    }

    return (fa->GetZone()->GetUlx() < fb->GetZone()->GetUlx());
}

bool Object::IsPreOrdered(const Object *left, const Object *right)
{
    ListOfConstObjects ancestorsLeft = left->GetAncestors();
    ancestorsLeft.push_front(left);
    // Check if right is an ancestor of left
    if (std::find(ancestorsLeft.begin(), ancestorsLeft.end(), right) != ancestorsLeft.end()) return false;
    ListOfConstObjects ancestorsRight = right->GetAncestors();
    ancestorsRight.push_front(right);
    // Check if left is an ancestor of right
    if (std::find(ancestorsRight.begin(), ancestorsRight.end(), left) != ancestorsRight.end()) return true;

    // Now there must be mismatches since we included left and right into the ancestor lists above
    auto iterPair = std::mismatch(ancestorsLeft.rbegin(), ancestorsLeft.rend(), ancestorsRight.rbegin());
    const Object *commonParent = (*iterPair.first)->m_parent;
    if (commonParent) {
        return (commonParent->GetChildIndex(*iterPair.first) < commonParent->GetChildIndex(*iterPair.second));
    }
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
        m_list.clear();
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

const ArrayOfObjects *ObjectListInterface::GetList(Object *node)
{
    this->ResetList(node);
    return &m_list;
}

int ObjectListInterface::GetListIndex(const Object *listElement)
{
    ArrayOfObjects::iterator iter;
    int i;
    for (iter = m_list.begin(), i = 0; iter != m_list.end(); ++iter, ++i) {
        if (listElement == *iter) {
            return i;
        }
    }
    return -1;
}

Object *ObjectListInterface::GetListFirst(const Object *startFrom, const ClassId classId)
{
    ArrayOfObjects::iterator it = m_list.begin();
    int idx = this->GetListIndex(startFrom);
    if (idx == -1) return NULL;
    std::advance(it, idx);
    it = std::find_if(it, m_list.end(), ObjectComparison(classId));
    return (it == m_list.end()) ? NULL : *it;
}

Object *ObjectListInterface::GetListFirstBackward(Object *startFrom, const ClassId classId)
{
    ArrayOfObjects::iterator it = m_list.begin();
    int idx = this->GetListIndex(startFrom);
    if (idx == -1) return NULL;
    std::advance(it, idx);
    ArrayOfObjects::reverse_iterator rit(it);
    rit = std::find_if(rit, m_list.rend(), ObjectComparison(classId));
    return (rit == m_list.rend()) ? NULL : *rit;
}

Object *ObjectListInterface::GetListPrevious(Object *listElement)
{
    ArrayOfObjects::iterator iter;
    int i;
    for (iter = m_list.begin(), i = 0; iter != m_list.end(); ++iter, ++i) {
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
    ArrayOfObjects::reverse_iterator iter;
    int i;
    for (iter = m_list.rbegin(), i = 0; iter != m_list.rend(); ++iter, ++i) {
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
    const ArrayOfObjects *childList = this->GetList(node); // make sure it's initialized
    for (ArrayOfObjects::const_iterator it = childList->begin(); it != childList->end(); ++it) {
        if ((*it)->Is(LB)) {
            continue;
        }
        Text *text = vrv_cast<Text *>(*it);
        assert(text);
        concatText += text->GetText();
    }
    return concatText;
}

void TextListInterface::GetTextLines(Object *node, std::vector<std::wstring> &lines)
{
    // alternatively we could cache the concatString in the interface and instantiate it in FilterList
    std::wstring concatText;
    const ArrayOfObjects *childList = this->GetList(node); // make sure it's initialized
    for (ArrayOfObjects::const_iterator it = childList->begin(); it != childList->end(); ++it) {
        if ((*it)->Is(LB) && !concatText.empty()) {
            lines.push_back(concatText);
            concatText.clear();
            continue;
        }
        Text *text = vrv_cast<Text *>(*it);
        assert(text);
        concatText += text->GetText();
    }
    if (!concatText.empty()) {
        lines.push_back(concatText);
    }
}

void TextListInterface::FilterList(ArrayOfObjects *childList)
{
    ArrayOfObjects::iterator iter = childList->begin();
    while (iter != childList->end()) {
        if (!(*iter)->Is({ LB, TEXT })) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc. but keep Lb)
            iter = childList->erase(iter);
            continue;
        }
        ++iter;
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
    const_obj_fpt = NULL;
}

Functor::Functor(int (Object::*_obj_fpt)(FunctorParams *))
{
    m_returnCode = FUNCTOR_CONTINUE;
    m_visibleOnly = true;
    obj_fpt = _obj_fpt;
    const_obj_fpt = NULL;
}

Functor::Functor(int (Object::*_const_obj_fpt)(FunctorParams *) const)
{
    m_returnCode = FUNCTOR_CONTINUE;
    m_visibleOnly = true;
    obj_fpt = NULL;
    const_obj_fpt = _const_obj_fpt;
}

void Functor::Call(Object *ptr, FunctorParams *functorParams)
{
    if (const_obj_fpt) {
        m_returnCode = (ptr->*const_obj_fpt)(functorParams);
    }
    else {
        m_returnCode = (ptr->*obj_fpt)(functorParams);
    }
}

void Functor::Call(const Object *ptr, FunctorParams *functorParams)
{
    if (!const_obj_fpt && obj_fpt) {
        LogError("Non-const functor cannot be called from a const method!");
        assert(false);
    }
    m_returnCode = (ptr->*const_obj_fpt)(functorParams);
}

//----------------------------------------------------------------------------
// ObjectFactory methods
//----------------------------------------------------------------------------

ObjectFactory *ObjectFactory::GetInstance()
{
    static thread_local ObjectFactory factory;
    return &factory;
}

Object *ObjectFactory::Create(std::string name)
{
    Object *object = NULL;

    MapOfStrConstructors::iterator it = s_ctorsRegistry.find(name);
    if (it != s_ctorsRegistry.end()) object = it->second();

    if (object) {
        return object;
    }
    else {
        LogError("Factory for '%s' not found", name.c_str());
        return NULL;
    }
}

ClassId ObjectFactory::GetClassId(std::string name)
{
    ClassId classId = OBJECT;

    MapOfStrClassIds::iterator it = s_classIdsRegistry.find(name);
    if (it != s_classIdsRegistry.end()) {
        classId = it->second;
    }
    else {
        LogError("ClassId for '%s' not found", name.c_str());
    }

    return classId;
}

void ObjectFactory::GetClassIds(const std::vector<std::string> &classStrings, std::vector<ClassId> &classIds)
{
    for (auto str : classStrings) {
        if (s_classIdsRegistry.count(str) > 0) {
            classIds.push_back(s_classIdsRegistry.at(str));
        }
        else {
            LogDebug("Class name '%s' could not be matched", str.c_str());
        }
    }
}

void ObjectFactory::Register(std::string name, ClassId classId, std::function<Object *(void)> function)
{
    s_ctorsRegistry[name] = function;
    s_classIdsRegistry[name] = classId;
}

//----------------------------------------------------------------------------
// Object functor methods
//----------------------------------------------------------------------------

int Object::AddLayerElementToFlatList(FunctorParams *functorParams)
{
    AddLayerElementToFlatListParams *params = vrv_params_cast<AddLayerElementToFlatListParams *>(functorParams);
    assert(params);

    params->m_flatList->push_back(this);
    // LogDebug("List %d", params->m_flatList->size());

    return FUNCTOR_CONTINUE;
}

int Object::FindByUuid(FunctorParams *functorParams) const
{
    FindByUuidParams *params = vrv_params_cast<FindByUuidParams *>(functorParams);
    assert(params);

    if (params->m_element) {
        // this should not happen, but just in case
        return FUNCTOR_STOP;
    }

    if (params->m_uuid == this->GetUuid()) {
        params->m_element = this;
        // LogDebug("Found it!");
        return FUNCTOR_STOP;
    }
    // LogDebug("Still looking for uuid...");
    return FUNCTOR_CONTINUE;
}

int Object::FindByComparison(FunctorParams *functorParams) const
{
    FindByComparisonParams *params = vrv_params_cast<FindByComparisonParams *>(functorParams);
    assert(params);

    if (params->m_element) {
        // this should not happen, but just in case
        return FUNCTOR_STOP;
    }

    // evaluate by applying the Comparison operator()
    if ((*params->m_comparison)(this)) {
        params->m_element = this;
        // LogDebug("Found it!");
        return FUNCTOR_STOP;
    }
    // LogDebug("Still looking for the object matching the Comparison...");
    return FUNCTOR_CONTINUE;
}

int Object::FindExtremeByComparison(FunctorParams *functorParams) const
{
    FindExtremeByComparisonParams *params = vrv_params_cast<FindExtremeByComparisonParams *>(functorParams);
    assert(params);

    // evaluate by applying the Comparison operator()
    if ((*params->m_comparison)(this)) {
        params->m_element = this;
    }
    // continue until the end
    return FUNCTOR_CONTINUE;
}

int Object::FindAllByComparison(FunctorParams *functorParams)
{
    FindAllByComparisonParams *params = vrv_params_cast<FindAllByComparisonParams *>(functorParams);
    assert(params);

    // evaluate by applying the Comparison operator()
    if ((*params->m_comparison)(this)) {
        params->m_elements->push_back(this);
        if (!params->m_continueDepthSearchForMatches) {
            return FUNCTOR_SIBLINGS;
        }
    }
    // continue until the end
    return FUNCTOR_CONTINUE;
}

int Object::FindAllConstByComparison(FunctorParams *functorParams) const
{
    FindAllConstByComparisonParams *params = vrv_params_cast<FindAllConstByComparisonParams *>(functorParams);
    assert(params);

    // evaluate by applying the Comparison operator()
    if ((*params->m_comparison)(this)) {
        params->m_elements->push_back(this);
        if (!params->m_continueDepthSearchForMatches) {
            return FUNCTOR_SIBLINGS;
        }
    }
    // continue until the end
    return FUNCTOR_CONTINUE;
}

int Object::FindAllBetween(FunctorParams *functorParams)
{
    FindAllBetweenParams *params = vrv_params_cast<FindAllBetweenParams *>(functorParams);
    assert(params);

    // We are reaching the start of the range
    if (params->m_start == this) {
        // Setting the start to NULL indicates that we are in the range
        params->m_start = NULL;
    }
    // We have not reached the start yet
    else if (params->m_start) {
        return FUNCTOR_CONTINUE;
    }

    // evaluate by applying the Comparison operator()
    if ((*params->m_comparison)(this)) {
        params->m_elements->push_back(this);
    }

    // We have reached the end of the range
    if (params->m_end == this) {
        return FUNCTOR_STOP;
    }

    // continue until the end
    return FUNCTOR_CONTINUE;
}

int Object::FindAllConstBetween(FunctorParams *functorParams) const
{
    FindAllConstBetweenParams *params = vrv_params_cast<FindAllConstBetweenParams *>(functorParams);
    assert(params);

    // We are reaching the start of the range
    if (params->m_start == this) {
        // Setting the start to NULL indicates that we are in the range
        params->m_start = NULL;
    }
    // We have not reached the start yet
    else if (params->m_start) {
        return FUNCTOR_CONTINUE;
    }

    // evaluate by applying the Comparison operator()
    if ((*params->m_comparison)(this)) {
        params->m_elements->push_back(this);
    }

    // We have reached the end of the range
    if (params->m_end == this) {
        return FUNCTOR_STOP;
    }

    // continue until the end
    return FUNCTOR_CONTINUE;
}

int Object::FindAllReferencedObjects(FunctorParams *functorParams)
{
    FindAllReferencedObjectsParams *params = vrv_params_cast<FindAllReferencedObjectsParams *>(functorParams);
    assert(params);

    if (this->HasInterface(INTERFACE_LINKING)) {
        LinkingInterface *interface = this->GetLinkingInterface();
        assert(interface);
        if (interface->GetNextLink()) params->m_elements->push_back(interface->GetNextLink());
        if (interface->GetSameasLink()) params->m_elements->push_back(interface->GetSameasLink());
    }
    if (this->HasInterface(INTERFACE_PLIST)) {
        PlistInterface *interface = this->GetPlistInterface();
        assert(interface);
        for (auto &object : *interface->GetRefs()) {
            params->m_elements->push_back(object);
        }
    }
    if (this->HasInterface(INTERFACE_TIME_POINT) || this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimePointInterface *interface = this->GetTimePointInterface();
        assert(interface);
        if (interface->GetStart() && !interface->GetStart()->Is(TIMESTAMP_ATTR))
            params->m_elements->push_back(interface->GetStart());
    }
    if (this->HasInterface(INTERFACE_TIME_SPANNING)) {
        TimeSpanningInterface *interface = this->GetTimeSpanningInterface();
        assert(interface);
        if (interface->GetEnd() && !interface->GetEnd()->Is(TIMESTAMP_ATTR))
            params->m_elements->push_back(interface->GetEnd());
    }
    if (this->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(this);
        assert(note);
        // The note has a stem.sameas that was resolved the a note, then that one is referenced
        if (note->HasStemSameas() && note->HasStemSameasNote()) {
            params->m_elements->push_back(note->GetStemSameasNote());
        }
    }
    // These will also be referred to as milestones in page-based MEI
    if (params->m_milestoneReferences && this->IsMilestoneElement()) {
        params->m_elements->push_back(this);
    }

    // continue until the end
    return FUNCTOR_CONTINUE;
}

int Object::ConvertToCastOffMensural(FunctorParams *functorParams)
{
    ConvertToCastOffMensuralParams *params = vrv_params_cast<ConvertToCastOffMensuralParams *>(functorParams);
    assert(params);

    assert(m_parent);
    // We want to move only the children of the layer of any type (notes, editorial elements, etc)
    if (m_parent->Is(LAYER)) {
        assert(params->m_targetLayer);
        this->MoveItselfTo(params->m_targetLayer);
        // Do not precess children because we move the full sub-tree
        return FUNCTOR_SIBLINGS;
    }

    return FUNCTOR_CONTINUE;
}

int Object::PrepareFacsimile(FunctorParams *functorParams)
{
    PrepareFacsimileParams *params = vrv_params_cast<PrepareFacsimileParams *>(functorParams);
    assert(params);

    if (this->HasInterface(INTERFACE_FACSIMILE)) {
        FacsimileInterface *interface = this->GetFacsimileInterface();
        assert(interface);
        if (interface->HasFacs()) {
            std::string facsUuid = (interface->GetFacs().compare(0, 1, "#") == 0 ? interface->GetFacs().substr(1)
                                                                                 : interface->GetFacs());
            Zone *zone = params->m_facsimile->FindZoneByUuid(facsUuid);
            if (zone != NULL) {
                interface->SetZone(zone);
            }
        }
        // Zoneless syl
        else if (this->Is(SYL)) {
            params->m_zonelessSyls.push_back(this);
        }
    }

    return FUNCTOR_CONTINUE;
}

int Object::PrepareLinking(FunctorParams *functorParams)
{
    PrepareLinkingParams *params = vrv_params_cast<PrepareLinkingParams *>(functorParams);
    assert(params);

    if (params->m_fillList && this->HasInterface(INTERFACE_LINKING)) {
        LinkingInterface *interface = this->GetLinkingInterface();
        assert(interface);
        interface->InterfacePrepareLinking(functorParams, this);
    }

    if (this->Is(NOTE)) {
        Note *note = vrv_cast<Note *>(this);
        assert(note);
        PrepareLinkingParams *params = vrv_params_cast<PrepareLinkingParams *>(functorParams);
        assert(params);
        note->ResolveStemSameas(params);
    }

    // @next
    std::string uuid = this->GetUuid();
    auto r1 = params->m_nextUuidPairs.equal_range(uuid);
    if (r1.first != params->m_nextUuidPairs.end()) {
        for (auto i = r1.first; i != r1.second; ++i) {
            i->second->SetNextLink(this);
        }
        params->m_nextUuidPairs.erase(r1.first, r1.second);
    }

    // @sameas
    auto r2 = params->m_sameasUuidPairs.equal_range(uuid);
    if (r2.first != params->m_sameasUuidPairs.end()) {
        for (auto j = r2.first; j != r2.second; ++j) {
            j->second->SetSameasLink(this);
        }
        params->m_sameasUuidPairs.erase(r2.first, r2.second);
    }
    return FUNCTOR_CONTINUE;
}

int Object::PreparePlist(FunctorParams *functorParams)
{
    PreparePlistParams *params = vrv_params_cast<PreparePlistParams *>(functorParams);
    assert(params);

    if (params->m_fillList && this->HasInterface(INTERFACE_PLIST)) {
        PlistInterface *interface = this->GetPlistInterface();
        assert(interface);
        return interface->InterfacePreparePlist(functorParams, this);
    }

    return FUNCTOR_CONTINUE;
}

int Object::ProcessPlist(FunctorParams *functorParams)
{
    PreparePlistParams *params = vrv_params_cast<PreparePlistParams *>(functorParams);
    assert(params);

    if (!this->IsLayerElement()) return FUNCTOR_CONTINUE;

    std::string uuid = this->GetUuid();
    auto i = std::find_if(params->m_interfaceUuidTuples.begin(), params->m_interfaceUuidTuples.end(),
        [&uuid](std::tuple<PlistInterface *, std::string, Object *> tuple) { return (std::get<1>(tuple) == uuid); });
    if (i != params->m_interfaceUuidTuples.end()) {
        std::get<2>(*i) = this;
    }

    return FUNCTOR_CONTINUE;
}

int Object::SetCautionaryScoreDef(FunctorParams *functorParams)
{
    SetCautionaryScoreDefParams *params = vrv_params_cast<SetCautionaryScoreDefParams *>(functorParams);
    assert(params);

    assert(params->m_currentScoreDef);

    // starting a new staff
    if (this->Is(STAFF)) {
        Staff *staff = vrv_cast<Staff *>(this);
        assert(staff);
        params->m_currentStaffDef = params->m_currentScoreDef->GetStaffDef(staff->GetN());
        return FUNCTOR_CONTINUE;
    }

    // starting a new layer
    if (this->Is(LAYER)) {
        Layer *layer = vrv_cast<Layer *>(this);
        assert(layer);
        layer->SetDrawingCautionValues(params->m_currentStaffDef);
        return FUNCTOR_SIBLINGS;
    }

    return FUNCTOR_CONTINUE;
}

int Object::ScoreDefSetCurrent(FunctorParams *functorParams)
{
    ScoreDefSetCurrentParams *params = vrv_params_cast<ScoreDefSetCurrentParams *>(functorParams);
    assert(params);

    if (this->Is({ DOC, MDIV, PAGES })) return FUNCTOR_CONTINUE;

    // starting a new page
    if (this->Is(PAGE)) {
        Page *page = vrv_cast<Page *>(this);
        assert(page);
        // This will be reach before we reach the begining of a first Score.
        // However, page->m_score has already been set by Page::ScoreDefSetCurrentPage
        // This must be the first page or a new score is starting on this page
        assert(page->m_score);
        if (!params->m_currentScore || (params->m_currentScore != page->m_score)) {
            params->m_upcomingScoreDef = *page->m_score->GetScoreDef();
            params->m_upcomingScoreDef.Process(params->m_functor, functorParams);
        }
        page->m_drawingScoreDef = params->m_upcomingScoreDef;
        return FUNCTOR_CONTINUE;
    }

    // starting a new score
    if (this->Is(SCORE)) {
        Score *score = vrv_cast<Score *>(this);
        assert(score);
        params->m_currentScore = score;
        params->m_upcomingScoreDef = *score->GetScoreDef();
        params->m_upcomingScoreDef.Process(params->m_functor, functorParams);
        // Trigger the redraw of everything
        params->m_upcomingScoreDef.SetRedrawFlags(StaffDefRedrawFlags::REDRAW_ALL);
        params->m_drawLabels = true;
        params->m_currentScoreDef = NULL;
        params->m_currentStaffDef = NULL;
        params->m_previousMeasure = NULL;
        params->m_currentSystem = NULL;
        params->m_restart = false;
        params->m_hasMeasure = false;
        return FUNCTOR_CONTINUE;
    }

    // starting a new system
    if (this->Is(SYSTEM)) {
        System *system = vrv_cast<System *>(this);
        assert(system);
        // This is the only thing we do for now - we need to wait until we reach the first measure
        params->m_currentSystem = system;
        params->m_hasMeasure = false;
        return FUNCTOR_CONTINUE;
    }

    // starting a new measure
    if (this->Is(MEASURE)) {
        // If we have a restart scoreDef before, for redrawing of everything on the measure
        if (params->m_restart) {
            params->m_upcomingScoreDef.SetRedrawFlags(StaffDefRedrawFlags::REDRAW_ALL);
        }

        Measure *measure = vrv_cast<Measure *>(this);
        assert(measure);
        int drawingFlags = 0;
        // This is the first measure of the system - more to do...
        if (params->m_currentSystem) {
            drawingFlags |= Measure::BarlineDrawingFlags::SYSTEM_BREAK;
            // We had a scoreDef so we need to put cautionnary values
            // This will also happend with clef in the last measure - however, the cautionnary functor will not do
            // anything then
            // The cautionary scoreDef for restart is already done when hitting the scoreDef
            if (params->m_upcomingScoreDef.m_setAsDrawing && params->m_previousMeasure && !params->m_restart) {
                ScoreDef cautionaryScoreDef = params->m_upcomingScoreDef;
                SetCautionaryScoreDefParams setCautionaryScoreDefParams(&cautionaryScoreDef);
                Functor setCautionaryScoreDef(&Object::SetCautionaryScoreDef);
                params->m_previousMeasure->Process(&setCautionaryScoreDef, &setCautionaryScoreDefParams);
            }
            // Set the flags we want to have. This also sets m_setAsDrawing to true so the next measure will keep it
            params->m_upcomingScoreDef.SetRedrawFlags(
                StaffDefRedrawFlags::REDRAW_CLEF | StaffDefRedrawFlags::REDRAW_KEYSIG);
            // Set it to the current system (used e.g. for endings)
            params->m_currentSystem->SetDrawingScoreDef(&params->m_upcomingScoreDef);
            params->m_currentSystem->GetDrawingScoreDef()->SetDrawLabels(params->m_drawLabels);
            params->m_currentSystem = NULL;
            params->m_drawLabels = false;
        }
        if (params->m_upcomingScoreDef.m_setAsDrawing) {
            measure->SetDrawingScoreDef(&params->m_upcomingScoreDef);
            params->m_currentScoreDef = measure->GetDrawingScoreDef();
            params->m_upcomingScoreDef.SetRedrawFlags(StaffDefRedrawFlags::FORCE_REDRAW);
            params->m_upcomingScoreDef.m_setAsDrawing = false;
        }
        params->m_drawLabels = false;

        // set other flags based on score def change
        if (params->m_upcomingScoreDef.m_insertScoreDef) {
            drawingFlags |= Measure::BarlineDrawingFlags::SCORE_DEF_INSERT;
            params->m_upcomingScoreDef.m_insertScoreDef = false;
        }

        // check if we need to draw barlines for current/previous measures (in cases when all staves are invisible in
        // them)
        ListOfObjects currentObjects, previousObjects;
        AttVisibilityComparison comparison(STAFF, BOOLEAN_false);
        measure->FindAllDescendantsByComparison(&currentObjects, &comparison);
        if ((int)currentObjects.size() == measure->GetChildCount(STAFF)) {
            drawingFlags |= Measure::BarlineDrawingFlags::INVISIBLE_MEASURE_CURRENT;
        }
        if (params->m_previousMeasure) {
            params->m_previousMeasure->FindAllDescendantsByComparison(&previousObjects, &comparison);
            if ((int)previousObjects.size() == params->m_previousMeasure->GetChildCount(STAFF))
                drawingFlags |= Measure::BarlineDrawingFlags::INVISIBLE_MEASURE_PREVIOUS;
        }

        measure->SetInvisibleStaffBarlines(params->m_previousMeasure, currentObjects, previousObjects, drawingFlags);
        measure->SetDrawingBarLines(params->m_previousMeasure, drawingFlags);

        params->m_previousMeasure = measure;
        params->m_restart = false;
        params->m_hasMeasure = true;

        return FUNCTOR_CONTINUE;
    }

    // starting a new scoreDef
    if (this->Is(SCOREDEF)) {
        ScoreDef *scoreDef = vrv_cast<ScoreDef *>(this);
        assert(scoreDef);
        // Replace the current scoreDef with the new one, including its content (staffDef) - this also sets
        // m_setAsDrawing to true so it will then be taken into account at the next measure
        if (scoreDef->HasClefInfo(UNLIMITED_DEPTH) || scoreDef->HasKeySigInfo(UNLIMITED_DEPTH)
            || scoreDef->HasMensurInfo(UNLIMITED_DEPTH) || scoreDef->HasMeterSigGrpInfo(UNLIMITED_DEPTH)
            || scoreDef->HasMeterSigInfo(UNLIMITED_DEPTH)) {
            params->m_upcomingScoreDef.ReplaceDrawingValues(scoreDef);
            params->m_upcomingScoreDef.m_insertScoreDef = true;
        }
        if (scoreDef->IsSectionRestart()) {
            params->m_drawLabels = true;
            params->m_restart = true;
            // Redraw the labels only if we already have a mesure in the system. Otherwise this will be
            // done through the system scoreDef
            scoreDef->SetDrawLabels(params->m_hasMeasure);
            // If we have a previous measure, we need to set the cautionary scoreDef indenpendently from the
            // presence of a system break
            if (params->m_previousMeasure) {
                ScoreDef cautionaryScoreDef = params->m_upcomingScoreDef;
                SetCautionaryScoreDefParams setCautionaryScoreDefParams(&cautionaryScoreDef);
                Functor setCautionaryScoreDef(&Object::SetCautionaryScoreDef);
                params->m_previousMeasure->Process(&setCautionaryScoreDef, &setCautionaryScoreDefParams);
            }
        }
    }

    // starting a new staffGrp
    if (this->Is(STAFFGRP)) {
        StaffGrp *staffGrp = vrv_cast<StaffGrp *>(this);
        assert(staffGrp);
        // For now replace labels only if we have a section@restart
        if (params->m_restart) {
            params->m_upcomingScoreDef.ReplaceDrawingLabels(staffGrp);
        }
    }

    // starting a new staffDef
    if (this->Is(STAFFDEF)) {
        StaffDef *staffDef = vrv_cast<StaffDef *>(this);
        assert(staffDef);
        params->m_upcomingScoreDef.ReplaceDrawingValues(staffDef);
    }

    // starting a new staff
    if (this->Is(STAFF)) {
        Staff *staff = vrv_cast<Staff *>(this);
        assert(staff);
        params->m_currentStaffDef = params->m_currentScoreDef->GetStaffDef(staff->GetN());
        assert(staff->m_drawingStaffDef == NULL);
        staff->m_drawingStaffDef = params->m_currentStaffDef;
        assert(staff->m_drawingTuning == NULL);
        staff->m_drawingTuning = dynamic_cast<Tuning *>(params->m_currentStaffDef->FindDescendantByType(TUNING));
        staff->m_drawingLines = params->m_currentStaffDef->GetLines();
        staff->m_drawingNotationType = params->m_currentStaffDef->GetNotationtype();
        staff->m_drawingStaffSize = 100;
        if (params->m_currentStaffDef->HasScale()) {
            staff->m_drawingStaffSize = params->m_currentStaffDef->GetScale();
        }
        if (staff->IsTablature()) {
            staff->m_drawingStaffSize *= TABLATURE_STAFF_RATIO;
        }
        if (MeterSigGrp *metersiggrp = params->m_currentStaffDef->GetCurrentMeterSigGrp();
            metersiggrp->GetFunc() == meterSigGrpLog_FUNC_alternating) {
            Measure *parentMeasure = vrv_cast<Measure *>(staff->GetFirstAncestor(MEASURE));
            if (parentMeasure) metersiggrp->AddAlternatingMeasureToVector(parentMeasure);
        }
        return FUNCTOR_CONTINUE;
    }

    // starting a new layer
    if (this->Is(LAYER)) {
        Layer *layer = vrv_cast<Layer *>(this);
        assert(layer);
        if (params->m_doc->GetType() != Transcription) layer->SetDrawingStaffDefValues(params->m_currentStaffDef);
        return FUNCTOR_CONTINUE;
    }

    // starting a new clef
    if (this->Is(CLEF)) {
        LayerElement *element = vrv_cast<LayerElement *>(this);
        assert(element);
        Clef *clef = vrv_cast<Clef *>(element->ThisOrSameasAsLink());
        assert(clef);
        if (clef->IsScoreDefElement()) {
            return FUNCTOR_CONTINUE;
        }
        assert(params->m_currentStaffDef);
        StaffDef *upcomingStaffDef = params->m_upcomingScoreDef.GetStaffDef(params->m_currentStaffDef->GetN());
        assert(upcomingStaffDef);
        upcomingStaffDef->SetCurrentClef(clef);
        params->m_upcomingScoreDef.m_setAsDrawing = true;
        return FUNCTOR_CONTINUE;
    }

    // starting a new keysig
    if (this->Is(KEYSIG)) {
        KeySig *keySig = vrv_cast<KeySig *>(this);
        assert(keySig);
        if (keySig->IsScoreDefElement()) {
            return FUNCTOR_CONTINUE;
        }
        assert(params->m_currentStaffDef);
        StaffDef *upcomingStaffDef = params->m_upcomingScoreDef.GetStaffDef(params->m_currentStaffDef->GetN());
        assert(upcomingStaffDef);
        upcomingStaffDef->SetCurrentKeySig(keySig);
        params->m_upcomingScoreDef.m_setAsDrawing = true;
        return FUNCTOR_CONTINUE;
    }

    // starting a new mensur
    if (this->Is(MENSUR)) {
        Mensur *mensur = vrv_cast<Mensur *>(this);
        assert(mensur);
        if (mensur->IsScoreDefElement()) {
            return FUNCTOR_CONTINUE;
        }
        assert(params->m_currentStaffDef);
        StaffDef *upcomingStaffDef = params->m_upcomingScoreDef.GetStaffDef(params->m_currentStaffDef->GetN());
        assert(upcomingStaffDef);
        upcomingStaffDef->SetCurrentMensur(mensur);
        params->m_upcomingScoreDef.m_setAsDrawing = true;
        return FUNCTOR_CONTINUE;
    }

    return FUNCTOR_CONTINUE;
}

int Object::GetAlignmentLeftRight(FunctorParams *functorParams)
{
    GetAlignmentLeftRightParams *params = vrv_params_cast<GetAlignmentLeftRightParams *>(functorParams);
    assert(params);

    if (!this->IsLayerElement()) return FUNCTOR_CONTINUE;

    if (!this->HasSelfBB() || this->HasEmptyBB()) return FUNCTOR_CONTINUE;

    if (this->Is(params->m_excludeClasses)) return FUNCTOR_CONTINUE;

    int refLeft = this->GetSelfLeft();
    if (params->m_minLeft > refLeft) params->m_minLeft = refLeft;

    int refRight = this->GetSelfRight();
    if (params->m_maxRight < refRight) params->m_maxRight = refRight;

    return FUNCTOR_CONTINUE;
}

int Object::SetOverflowBBoxes(FunctorParams *functorParams)
{
    SetOverflowBBoxesParams *params = vrv_params_cast<SetOverflowBBoxesParams *>(functorParams);
    assert(params);

    // starting a new staff
    if (this->Is(STAFF)) {
        Staff *currentStaff = vrv_cast<Staff *>(this);
        assert(currentStaff);

        if (!currentStaff->DrawingIsVisible()) {
            return FUNCTOR_SIBLINGS;
        }

        params->m_staffAlignment = currentStaff->GetAlignment();
        return FUNCTOR_CONTINUE;
    }

    // starting new layer
    if (this->Is(LAYER)) {
        Layer *currentLayer = vrv_cast<Layer *>(this);
        assert(currentLayer);
        // set scoreDef attr
        if (currentLayer->GetStaffDefClef()) {
            // System scoreDef clefs are taken into account but treated separately (see below)
            currentLayer->GetStaffDefClef()->SetOverflowBBoxes(params);
        }
        if (currentLayer->GetStaffDefKeySig()) {
            currentLayer->GetStaffDefKeySig()->SetOverflowBBoxes(params);
        }
        if (currentLayer->GetStaffDefMensur()) {
            currentLayer->GetStaffDefMensur()->SetOverflowBBoxes(params);
        }
        if (currentLayer->GetStaffDefMeterSig()) {
            currentLayer->GetStaffDefMeterSig()->SetOverflowBBoxes(params);
        }
        return FUNCTOR_CONTINUE;
    }

    if (this->IsSystemElement()) {
        return FUNCTOR_CONTINUE;
    }

    if (this->IsControlElement()) {
        return FUNCTOR_CONTINUE;
    }

    if (!this->IsLayerElement()) {
        return FUNCTOR_CONTINUE;
    }

    // Take into account beam in cross-staff situation
    if (this->Is(BEAM)) {
        Beam *beam = vrv_cast<Beam *>(this);
        assert(beam);
        // Ignore it if it has cross-staff content but is not entirely cross-staff itself
        if (beam->m_crossStaffContent && !beam->m_crossStaff) return FUNCTOR_CONTINUE;
    }

    // Take into account stem for notes in cross-staff situation and in beams
    if (this->Is(STEM)) {
        LayerElement *noteOrChord = dynamic_cast<LayerElement *>(this->GetParent());
        if (noteOrChord && noteOrChord->m_crossStaff) {
            if (noteOrChord->IsInBeam()) {
                Beam *beam = vrv_cast<Beam *>(noteOrChord->GetFirstAncestor(BEAM));
                assert(beam);
                // Ignore it but only if the beam is not entirely cross-staff itself
                if (!beam->m_crossStaff) return FUNCTOR_CONTINUE;
            }
            else if (noteOrChord->IsInBeamSpan()) {
                return FUNCTOR_CONTINUE;
            }
        }
    }

    if (this->Is(FB) || this->Is(FIGURE)) {
        return FUNCTOR_CONTINUE;
    }

    if (this->Is(SYL)) {
        // We don't want to add the syl to the overflow since lyrics require a full line anyway
        return FUNCTOR_CONTINUE;
    }

    if (!this->HasSelfBB()) {
        // if nothing was drawn, do not take it into account
        return FUNCTOR_CONTINUE;
    }

    assert(params->m_staffAlignment);

    LayerElement *current = vrv_cast<LayerElement *>(this);
    assert(current);

    StaffAlignment *above = NULL;
    StaffAlignment *below = NULL;
    current->GetOverflowStaffAlignments(above, below);

    bool isScoreDefClef = false;
    // Exception for the scoreDef clef where we do not want to take into account the general overflow
    // We have instead distinct members in StaffAlignment to store them
    if (current->Is(CLEF) && current->GetScoreDefRole() == SCOREDEF_SYSTEM) {
        isScoreDefClef = true;
    }

    if (above) {
        int overflowAbove = above->CalcOverflowAbove(current);
        int staffSize = above->GetStaffSize();
        if (overflowAbove > params->m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
            // LogMessage("%s top overflow: %d", current->GetUuid().c_str(), overflowAbove);
            if (isScoreDefClef) {
                above->SetScoreDefClefOverflowAbove(overflowAbove);
            }
            else {
                above->SetOverflowBBoxAbove(current, overflowAbove);
                above->SetOverflowAbove(overflowAbove);
                above->AddBBoxAbove(current);
            }
        }
    }

    if (below) {
        int overflowBelow = below->CalcOverflowBelow(current);
        int staffSize = below->GetStaffSize();
        if (overflowBelow > params->m_doc->GetDrawingStaffLineWidth(staffSize) / 2) {
            // LogMessage("%s bottom overflow: %d", current->GetUuid().c_str(), overflowBelow);
            if (isScoreDefClef) {
                below->SetScoreDefClefOverflowBelow(overflowBelow);
            }
            else {
                below->SetOverflowBBoxBelow(current, overflowBelow);
                below->SetOverflowBelow(overflowBelow);
                below->AddBBoxBelow(current);
            }
        }
    }

    return FUNCTOR_CONTINUE;
}

int Object::SetOverflowBBoxesEnd(FunctorParams *functorParams)
{
    SetOverflowBBoxesParams *params = vrv_params_cast<SetOverflowBBoxesParams *>(functorParams);
    assert(params);

    // starting new layer
    if (this->Is(LAYER)) {
        Layer *currentLayer = vrv_cast<Layer *>(this);
        assert(currentLayer);
        // set scoreDef attr
        if (currentLayer->GetCautionStaffDefClef()) {
            currentLayer->GetCautionStaffDefClef()->SetOverflowBBoxes(params);
        }
        if (currentLayer->GetCautionStaffDefKeySig()) {
            currentLayer->GetCautionStaffDefKeySig()->SetOverflowBBoxes(params);
        }
        if (currentLayer->GetCautionStaffDefMensur()) {
            currentLayer->GetCautionStaffDefMensur()->SetOverflowBBoxes(params);
        }
        if (currentLayer->GetCautionStaffDefMeterSig()) {
            currentLayer->GetCautionStaffDefMeterSig()->SetOverflowBBoxes(params);
        }
    }
    return FUNCTOR_CONTINUE;
}

int Object::GenerateFeatures(FunctorParams *functorParams)
{
    GenerateFeaturesParams *params = vrv_params_cast<GenerateFeaturesParams *>(functorParams);
    assert(params);

    params->m_extractor->Extract(this, params);

    return FUNCTOR_CONTINUE;
}

int Object::Save(FunctorParams *functorParams)
{
    SaveParams *params = vrv_params_cast<SaveParams *>(functorParams);
    assert(params);

    if (!params->m_output->WriteObject(this)) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
}

int Object::SaveEnd(FunctorParams *functorParams)
{
    SaveParams *params = vrv_params_cast<SaveParams *>(functorParams);
    assert(params);

    if (!params->m_output->WriteObjectEnd(this)) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;
}

int Object::ReorderByXPos(FunctorParams *functorParams)
{
    if (this->GetFacsimileInterface() != NULL) {
        if (this->GetFacsimileInterface()->HasFacs()) {
            return FUNCTOR_SIBLINGS; // This would have already been reordered.
        }
    }

    std::stable_sort(m_children.begin(), m_children.end(), sortByUlx);
    this->Modify();
    return FUNCTOR_CONTINUE;
}

int Object::FindNextChildByComparison(FunctorParams *functorparams)
{
    FindChildByComparisonParams *params = vrv_cast<FindChildByComparisonParams *>(functorparams);
    assert(params);

    // we are reaching the start of the range
    if (params->m_start == this) {
        // setting m_start to be null tells us that we're in the range
        params->m_start = NULL;
        return FUNCTOR_CONTINUE;
    }

    else if (params->m_start) {
        // we're not yet in the range
        return FUNCTOR_CONTINUE;
    }

    if ((*params->m_comparison)(this)) {
        params->m_element = this;
        return FUNCTOR_STOP;
    }

    return FUNCTOR_CONTINUE;
}

int Object::FindPreviousChildByComparison(FunctorParams *functorparams)
{
    FindChildByComparisonParams *params = vrv_cast<FindChildByComparisonParams *>(functorparams);
    assert(params);
    // this guy works by going from the start and replacing the return element with every nearer element
    // until you get to the 'start' element
    if (params->m_start == this) {
        // we've reached the end element, so stop
        return FUNCTOR_STOP;
    }

    if ((*params->m_comparison)(this)) {
        params->m_element = this;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
