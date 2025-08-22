/////////////////////////////////////////////////////////////////////////////
// Name:        object.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "object.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <climits>
#include <iostream>
#include <math.h>
#include <random>
#include <sstream>

//----------------------------------------------------------------------------

#include "altsyminterface.h"
#include "chord.h"
#include "clef.h"
#include "comparison.h"
#include "dir.h"
#include "doc.h"
#include "dynam.h"
#include "editorial.h"
#include "featureextractor.h"
#include "findfunctor.h"
#include "iobase.h"
#include "keysig.h"
#include "layer.h"
#include "linkinginterface.h"
#include "mdiv.h"
#include "measure.h"
#include "mensur.h"
#include "metersig.h"
#include "miscfunctor.h"
#include "nc.h"
#include "note.h"
#include "page.h"
#include "plistinterface.h"
#include "resetfunctor.h"
#include "savefunctor.h"
#include "score.h"
#include "staff.h"
#include "staffdef.h"
#include "surface.h"
#include "syl.h"
#include "syllable.h"
#include "symboltable.h"
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
thread_local uint32_t Object::s_xmlIDCounter = 0;

Object::Object() : BoundingBox()
{
    if (s_objectCounter++ == 0) {
        this->SeedID();
    }
    this->Init(OBJECT);
}

Object::Object(ClassId classId) : BoundingBox()
{
    if (s_objectCounter++ == 0) {
        this->SeedID();
    }
    this->Init(classId);
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
    m_parent = NULL;

    // Flags
    m_isAttribute = object.m_isAttribute;
    m_isModified = true;
    m_isReferenceObject = object.m_isReferenceObject;

    // Also copy attribute classes
    m_attClasses = object.m_attClasses;
    m_interfaces = object.m_interfaces;
    // New id
    this->GenerateID();
    // For now do not copy them
    m_unsupported = object.m_unsupported;

    if (!object.CopyChildren()) {
        return;
    }

    for (Object *current : object.m_children) {
        Object *clone = current->Clone();
        if (clone) {
            LinkingInterface *link = clone->GetLinkingInterface();
            if (link) link->AddBackLink(current);
            clone->SetParent(this);
            clone->CloneReset();
            m_children.push_back(clone);
        }
    }
}

void Object::CloneReset()
{
    this->Modify();
    ResetDataFunctor resetData;
    this->Process(resetData, 0);
}

Object &Object::operator=(const Object &object)
{
    // not self assignement
    if (this != &object) {
        this->ClearChildren();
        this->ResetBoundingBox(); // It does not make sense to keep the values of the BBox

        m_classId = object.m_classId;
        m_parent = NULL;
        // Flags
        m_isAttribute = object.m_isAttribute;
        m_isModified = true;
        m_isReferenceObject = object.m_isReferenceObject;

        // Also copy attribute classes
        m_attClasses = object.m_attClasses;
        m_interfaces = object.m_interfaces;
        // New id
        this->GenerateID();
        // For now do now copy them
        m_unsupported = object.m_unsupported;
        LinkingInterface *link = this->GetLinkingInterface();
        if (link) link->AddBackLink(&object);

        if (object.CopyChildren()) {
            for (Object *current : object.m_children) {
                Object *clone = current->Clone();
                if (clone) {
                    LinkingInterface *link = clone->GetLinkingInterface();
                    if (link) link->AddBackLink(current);
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
    this->ClearChildren();
}

void Object::Init(ClassId classId)
{
    m_classId = classId;
    m_parent = NULL;
    // Flags
    m_isAttribute = false;
    m_isModified = true;
    m_isReferenceObject = false;
    // Comments
    m_comment = "";
    m_closingComment = "";

    this->GenerateID();

    this->Reset();
}

void Object::SetAsReferenceObject()
{
    assert(m_children.empty());

    m_isReferenceObject = true;
}

const Resources *Object::GetDocResources() const
{
    // Search for the document
    const Doc *doc = NULL;
    if (this->Is(DOC)) {
        doc = vrv_cast<const Doc *>(this);
    }
    else {
        doc = vrv_cast<const Doc *>(this->GetFirstAncestor(DOC));
    }

    // Return the resources or display warning
    if (doc) {
        return &doc->GetResources();
    }
    else {
        LogWarning("Requested resources unavailable.");
        return NULL;
    }
}

void Object::Reset()
{
    this->ClearChildren();
    this->ResetBoundingBox();
}

void Object::RegisterInterface(std::vector<AttClassId> *attClasses, InterfaceId interfaceId)
{
    m_attClasses.insert(m_attClasses.end(), attClasses->begin(), attClasses->end());
    m_interfaces.push_back(interfaceId);
}

bool Object::IsMilestoneElement() const
{
    if (this->IsEditorialElement() || this->Is(ENDING) || this->Is(SECTION)) {
        const SystemMilestoneInterface *interface = dynamic_cast<const SystemMilestoneInterface *>(this);
        assert(interface);
        return (interface->IsSystemMilestone());
    }
    else if (this->Is(MDIV) || this->Is(SCORE)) {
        const PageMilestoneInterface *interface = dynamic_cast<const PageMilestoneInterface *>(this);
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

    for (int i = 0; i < (int)sourceParent->m_children.size(); ++i) {
        Object *child = sourceParent->Relinquish(i);
        if (idx != -1) {
            this->InsertChild(child, idx);
            idx++;
        }
        else {
            this->AddChild(child);
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
    this->InsertChild(newChild, idx);

    this->Modify();
}

void Object::InsertAfter(Object *child, Object *newChild)
{
    assert(this->GetChildIndex(child) != -1);
    assert(this->GetChildIndex(newChild) == -1);

    int idx = this->GetChildIndex(child);
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

void Object::SwapID(Object *other)
{
    assert(other);
    std::string swapID = this->GetID();
    this->SetID(other->GetID());
    other->SetID(swapID);
}

void Object::ClearChildren()
{
    if (m_isReferenceObject) {
        m_children.clear();
        return;
    }

    for (Object *child : m_children) {
        // we need to check if this is the parent
        // ownership might have been given up with Relinquish
        if (child->GetParent() == this) {
            delete child;
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

void Object::CopyAttributesTo(Object *target) const
{
    assert(this->GetClassId() == target->GetClassId());

    AttModule::CopyAnalytical(this, target);
    AttModule::CopyCmn(this, target);
    AttModule::CopyCmnornaments(this, target);
    AttModule::CopyCritapp(this, target);
    // AttModule::CopyEdittrans(this, target);
    AttModule::CopyExternalsymbols(this, target);
    AttModule::CopyFacsimile(this, target);
    // AttModule::CopyFigtable(this, target);
    // AttModule::CopyFingering(this, target);
    AttModule::CopyGestural(this, target);
    // AttModule::CopyHarmony(this, target);
    // AttModule::CopyHeader(this, target);
    AttModule::CopyMei(this, target);
    AttModule::CopyMensural(this, target);
    AttModule::CopyMidi(this, target);
    AttModule::CopyNeumes(this, target);
    AttModule::CopyPagebased(this, target);
    // AttModule::CopyPerformance(this, target);
    AttModule::CopyShared(this, target);
    AttModule::CopyStringtab(this, target);
    // AttModule::CopyUsersymbols(this, target);
    AttModule::CopyVisual(this, target);

    target->m_unsupported = this->m_unsupported;
}

int Object::GetAttributes(ArrayOfStrAttr *attributes) const
{
    assert(attributes);
    attributes->clear();

    AttModule::GetAnalytical(this, attributes);
    AttModule::GetCmn(this, attributes);
    AttModule::GetCmnornaments(this, attributes);
    AttModule::GetCritapp(this, attributes);
    // AttModule::GetEdittrans(this, attributes);
    AttModule::GetExternalsymbols(this, attributes);
    AttModule::GetFacsimile(this, attributes);
    // AttModule::GetFigtable(this, attributes);
    // AttModule::GetFingering(this, attributes);
    AttModule::GetGestural(this, attributes);
    // AttModule::GetHarmony(this, attributes);
    // AttModule::GetHeader(this, attributes);
    AttModule::GetMei(this, attributes);
    AttModule::GetMensural(this, attributes);
    AttModule::GetMidi(this, attributes);
    AttModule::GetNeumes(this, attributes);
    AttModule::GetPagebased(this, attributes);
    // AttModule::GetPerformance(this, attributes);
    AttModule::GetShared(this, attributes);
    AttModule::GetStringtab(this, attributes);
    // AttModule::GetUsersymbols(this, attributes);
    AttModule::GetVisual(this, attributes);

    for (auto &pair : m_unsupported) {
        attributes->push_back({ pair.first, pair.second });
    }

    return (int)attributes->size();
}

bool Object::HasAttribute(std::string attribute, std::string value) const
{
    ArrayOfStrAttr attributes;
    this->GetAttributes(&attributes);
    for (auto &attributePair : attributes) {
        if ((attributePair.first == attribute) && (attributePair.second == value)) return true;
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
    // With this method we require the parent to be NULL
    assert(!element->GetParent());
    element->SetParent(this);

    if (idx >= (int)m_children.size()) {
        m_children.push_back(element);
        return;
    }
    ArrayOfObjects::iterator iter = m_children.begin();
    m_children.insert(iter + (idx), element);
}

void Object::RotateChildren(int first, int middle, int last)
{
    std::rotate(m_children.begin() + first, m_children.begin() + middle, m_children.begin() + last);
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

void Object::ReplaceWithCopyOf(Object *object)
{
    Object *parent = this->GetParent();
    *this = *object;
    this->CloneReset();
    this->SetParent(parent);
}

bool Object::HasDescendant(const Object *child, int deepness) const
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
        else {
            ++iter;
        }
    }
}

Object *Object::FindDescendantByID(const std::string &id, int deepness, bool direction)
{
    return const_cast<Object *>(std::as_const(*this).FindDescendantByID(id, deepness, direction));
}

const Object *Object::FindDescendantByID(const std::string &id, int deepness, bool direction) const
{
    FindByIDFunctor findByID(id);
    findByID.SetDirection(direction);
    this->Process(findByID, deepness, true);
    return findByID.GetElement();
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
    FindByComparisonFunctor findByComparison(comparison);
    findByComparison.SetDirection(direction);
    this->Process(findByComparison, deepness, true);
    return findByComparison.GetElement();
}

Object *Object::FindDescendantExtremeByComparison(Comparison *comparison, int deepness, bool direction)
{
    return const_cast<Object *>(
        std::as_const(*this).FindDescendantExtremeByComparison(comparison, deepness, direction));
}

const Object *Object::FindDescendantExtremeByComparison(Comparison *comparison, int deepness, bool direction) const
{
    FindExtremeByComparisonFunctor findExtremeByComparison(comparison);
    findExtremeByComparison.SetDirection(direction);
    this->Process(findExtremeByComparison, deepness, true);
    return findExtremeByComparison.GetElement();
}

ListOfObjects Object::FindAllDescendantsByType(ClassId classId, bool continueDepthSearchForMatches, int deepness)
{
    ListOfObjects descendants;
    ClassIdComparison comparison(classId);
    FindAllByComparisonFunctor findAllByComparison(&comparison, &descendants);
    findAllByComparison.SetContinueDepthSearchForMatches(continueDepthSearchForMatches);
    this->Process(findAllByComparison, deepness, true);
    return descendants;
}

ListOfConstObjects Object::FindAllDescendantsByType(
    ClassId classId, bool continueDepthSearchForMatches, int deepness) const
{
    ListOfConstObjects descendants;
    ClassIdComparison comparison(classId);
    FindAllConstByComparisonFunctor findAllConstByComparison(&comparison, &descendants);
    findAllConstByComparison.SetContinueDepthSearchForMatches(continueDepthSearchForMatches);
    this->Process(findAllConstByComparison, deepness, true);
    return descendants;
}

void Object::FindAllDescendantsByComparison(
    ListOfObjects *objects, Comparison *comparison, int deepness, bool direction, bool clear)
{
    assert(objects);
    if (clear) objects->clear();

    FindAllByComparisonFunctor findAllByComparison(comparison, objects);
    findAllByComparison.SetDirection(direction);
    this->Process(findAllByComparison, deepness, true);
}

void Object::FindAllDescendantsByComparison(
    ListOfConstObjects *objects, Comparison *comparison, int deepness, bool direction, bool clear) const
{
    assert(objects);
    if (clear) objects->clear();

    FindAllConstByComparisonFunctor findAllConstByComparison(comparison, objects);
    findAllConstByComparison.SetDirection(direction);
    this->Process(findAllConstByComparison, deepness, true);
}

void Object::FindAllDescendantsBetween(
    ListOfObjects *objects, Comparison *comparison, const Object *start, const Object *end, bool clear, int depth)
{
    assert(objects);
    if (clear) objects->clear();

    ListOfConstObjects descendants;
    FindAllBetweenFunctor findAllBetween(comparison, &descendants, start, end);
    this->Process(findAllBetween, depth, true);

    std::transform(descendants.begin(), descendants.end(), std::back_inserter(*objects),
        [](const Object *obj) { return const_cast<Object *>(obj); });
}

void Object::FindAllDescendantsBetween(ListOfConstObjects *objects, Comparison *comparison, const Object *start,
    const Object *end, bool clear, int depth) const
{
    assert(objects);
    if (clear) objects->clear();

    FindAllBetweenFunctor findAllBetween(comparison, objects, start, end);
    this->Process(findAllBetween, depth, true);
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

int Object::DeleteChildrenByComparison(Comparison *comparison)
{
    int count = 0;
    ArrayOfObjects::iterator iter;
    for (iter = m_children.begin(); iter != m_children.end();) {
        if ((*comparison)(*iter)) {
            if (!m_isReferenceObject) delete *iter;
            iter = m_children.erase(iter);
            ++count;
        }
        else {
            ++iter;
        }
    }
    if (count > 0) this->Modify();
    return count;
}

void Object::GenerateID()
{
    // A random letter from a-z
    char letter = 'a' + (s_xmlIDCounter % 26);
    m_id = letter + Object::GenerateHashID();
}

void Object::ResetID()
{
    this->GenerateID();
}

void Object::SetParent(Object *parent)
{
    assert(!m_parent);
    m_parent = parent;
}

bool Object::IsSupportedChild(ClassId classId)
{
    // This should never happen because the method should be overridden
    LogDebug("Method for adding %d to %s should be overridden", classId, this->GetClassName().c_str());
    // assert(false);
    return false;
}

void Object::AddChild(Object *child)
{
    if (!this->IsSupportedChild(child->GetClassId()) || !this->AddChildAdditionalCheck(child)) {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        return;
    }

    if (!this->IsReferenceObject()) {
        child->SetParent(this);
    }
    const int insertOrder = this->GetInsertOrderFor(child->GetClassId());
    // no child or no order specify, the child is appended at the end
    if (m_children.empty() || insertOrder == VRV_UNSET) {
        m_children.push_back(child);
    }
    else {
        int i = 0;
        for (const Object *existingChild : m_children) {
            // By doing abs() we convert VRV_UNSET to a positive and insert anything with an insertOrder before it
            if (abs(this->GetInsertOrderFor(existingChild->GetClassId())) > insertOrder) break;
            ++i;
        }
        i = std::min(i, (int)m_children.size());
        m_children.insert(m_children.begin() + i, child);
    }
    this->Modify();
}

int Object::GetInsertOrderForIn(ClassId classId, const std::vector<ClassId> &order) const
{
    std::vector<ClassId>::const_iterator classIdIt = std::find(order.begin(), order.end(), classId);
    if (classIdIt == order.end()) return VRV_UNSET;
    return static_cast<int>(std::distance(order.begin(), classIdIt));
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
    for (Object *object : objects) {
        if (child == object) return i;
        ++i;
    }
    return -1;
}

void Object::Modify(bool modified) const
{
    // if we have a parent and a new modification, propagate it
    if (m_parent && modified) {
        m_parent->Modify();
    }
    m_isModified = modified;
}

void Object::FillFlatList(ListOfConstObjects &flatList) const
{
    AddToFlatListFunctor addToFlatList(&flatList);
    this->Process(addToFlatList);
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
    return const_cast<Object *>(std::as_const(*this).GetLastAncestorNot(classId, maxDepth));
}

const Object *Object::GetLastAncestorNot(const ClassId classId, int maxDepth) const
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
    return const_cast<Object *>(std::as_const(*this).GetFirstChildNot(classId));
}

const Object *Object::GetFirstChildNot(const ClassId classId) const
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

void Object::Process(Functor &functor, int deepness, bool skipFirst)
{
    if (functor.GetCode() == FUNCTOR_STOP) {
        return;
    }

    if (!skipFirst) {
        FunctorCode code = this->Accept(functor);
        functor.SetCode(code);
    }

    // do not go any deeper in this case
    if (functor.GetCode() == FUNCTOR_SIBLINGS) {
        functor.SetCode(FUNCTOR_CONTINUE);
        return;
    }
    else if (this->IsEditorialElement()) {
        // since editorial object doesn't count, we increase the deepness limit
        ++deepness;
    }
    if (deepness == 0) {
        // any need to change the functor m_returnCode?
        return;
    }
    --deepness;

    if (!this->SkipChildren(functor.VisibleOnly())) {
        // We need a pointer to the array for the option to work on a reversed copy
        ArrayOfObjects *children = &m_children;
        Filters *filters = functor.GetFilters();
        if (functor.GetDirection() == BACKWARD) {
            for (ArrayOfObjects::reverse_iterator iter = children->rbegin(); iter != children->rend(); ++iter) {
                // we will end here if there is no filter at all or for the current child type
                if (this->FiltersApply(filters, *iter)) {
                    (*iter)->Process(functor, deepness);
                }
            }
        }
        else {
            for (Object *child : m_children) {
                // we will end here if there is no filter at all or for the current child type
                if (this->FiltersApply(filters, child)) {
                    child->Process(functor, deepness);
                }
            }
        }
    }

    if (functor.ImplementsEndInterface() && !skipFirst) {
        FunctorCode code = this->AcceptEnd(functor);
        functor.SetCode(code);
    }
}

void Object::Process(ConstFunctor &functor, int deepness, bool skipFirst) const
{
    if (functor.GetCode() == FUNCTOR_STOP) {
        return;
    }

    if (!skipFirst) {
        FunctorCode code = this->Accept(functor);
        functor.SetCode(code);
    }

    // do not go any deeper in this case
    if (functor.GetCode() == FUNCTOR_SIBLINGS) {
        functor.SetCode(FUNCTOR_CONTINUE);
        return;
    }
    else if (this->IsEditorialElement()) {
        // since editorial object doesn't count, we increase the deepness limit
        ++deepness;
    }
    if (deepness == 0) {
        // any need to change the functor m_returnCode?
        return;
    }
    --deepness;

    if (!this->SkipChildren(functor.VisibleOnly())) {
        // We need a pointer to the array for the option to work on a reversed copy
        const ArrayOfObjects *children = &m_children;
        Filters *filters = functor.GetFilters();
        if (functor.GetDirection() == BACKWARD) {
            for (ArrayOfObjects::const_reverse_iterator iter = children->rbegin(); iter != children->rend(); ++iter) {
                // we will end here if there is no filter at all or for the current child type
                if (this->FiltersApply(filters, *iter)) {
                    (*iter)->Process(functor, deepness);
                }
            }
        }
        else {
            for (ArrayOfObjects::const_iterator iter = children->begin(); iter != children->end(); ++iter) {
                // we will end here if there is no filter at all or for the current child type
                if (this->FiltersApply(filters, *iter)) {
                    (*iter)->Process(functor, deepness);
                }
            }
        }
    }

    if (functor.ImplementsEndInterface() && !skipFirst) {
        FunctorCode code = this->AcceptEnd(functor);
        functor.SetCode(code);
    }
}

FunctorCode Object::Accept(Functor &functor)
{
    return functor.VisitObject(this);
}

FunctorCode Object::Accept(ConstFunctor &functor) const
{
    return functor.VisitObject(this);
}

FunctorCode Object::AcceptEnd(Functor &functor)
{
    return functor.VisitObjectEnd(this);
}

FunctorCode Object::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitObjectEnd(this);
}

bool Object::SkipChildren(bool visibleOnly) const
{
    if (visibleOnly) {
        if (this->IsEditorialElement() || this->Is(MDIV) || this->IsSystemElement()) {
            const VisibilityDrawingInterface *interface = this->GetVisibilityDrawingInterface();
            assert(interface);
            if (interface->IsHidden()) {
                return true;
            }
        }
    }
    return false;
}

bool Object::FiltersApply(const Filters *filters, Object *object) const
{
    return filters ? filters->Apply(object) : true;
}

void Object::SaveObject(Output *output)
{
    SaveFunctor save(output);
    // Special case where we want to process all objects
    save.SetVisibleOnly(false);
    this->Process(save);
}

void Object::ReorderByXPos()
{
    ReorderByXPosFunctor reorderByXPos;
    this->Process(reorderByXPos);
}

Object *Object::FindNextChild(Comparison *comp, Object *start)
{
    FindNextChildByComparisonFunctor findNextChildByComparison(comp, start);
    this->Process(findNextChildByComparison);
    return const_cast<Object *>(findNextChildByComparison.GetElement());
}

Object *Object::FindPreviousChild(Comparison *comp, Object *start)
{
    FindPreviousChildByComparisonFunctor findPreviousChildByComparison(comp, start);
    this->Process(findPreviousChildByComparison);
    return const_cast<Object *>(findPreviousChildByComparison.GetElement());
}

void Object::AddPlistReference(const Object *object)
{
    if (!m_plistReferences) {
        m_plistReferences = std::make_unique<ListOfConstObjects>();
    }
    m_plistReferences->push_back(object);
}

void Object::LogDebugTree(int maxDepth, int level)
{
    std::string indent(level, '\t');
    LogDebug("%s%s", indent.c_str(), this->LogDebugTreeMsg().c_str());

    if (maxDepth == level) return;

    for (auto &child : this->GetChildren()) {
        child->LogDebugTree(maxDepth, level + 1);
    }
}

//----------------------------------------------------------------------------
// Static methods for Object
//----------------------------------------------------------------------------

void Object::SeedID(uint32_t seed)
{
    if (seed == 0) {
        // Random start ID
        std::random_device rd;
        std::mt19937 randomGenerator(rd());
        s_xmlIDCounter = randomGenerator();
    }
    else {
        // Deterministic start ID
        s_xmlIDCounter = Object::Hash(seed);
    }
}

std::string Object::GenerateHashID()
{
    uint32_t nr = Object::Hash(++s_xmlIDCounter);

    return BaseEncodeInt(nr, 36);
}

uint32_t Object::Hash(uint32_t number, bool reverse)
{
    const uint32_t magicNumber = reverse ? 0x119de1f3 : 0x45d9f3b;
    number = ((number >> 16) ^ number) * magicNumber;
    number = ((number >> 16) ^ number) * magicNumber;
    number = (number >> 16) ^ number;
    return number;
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
        for (Object *object : children) {
            if (object->Is(SYL)) continue;
            FacsimileInterface *temp = object->GetFacsimileInterface();
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
        for (Object *object : children) {
            if (object->Is(SYL)) continue;
            FacsimileInterface *temp = object->GetFacsimileInterface();
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
        Zone *zonea = dynamic_cast<Zone *>(nca->GetFacsimileInterface()->GetZone());
        assert(zonea);
        Zone *zoneb = dynamic_cast<Zone *>(ncb->GetFacsimileInterface()->GetZone());
        assert(zoneb);
        if (nca->HasLigated() && ncb->HasLigated() && (a->GetParent() == b->GetParent())
            && (zonea->GetUlx() == zoneb->GetUlx())) {
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
            LogInfo("No available facsimile interface for %s", a->GetID().c_str());
        }
        if (fb == NULL) {
            LogInfo("No available facsimile interface for %s", b->GetID().c_str());
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

void ObjectListInterface::ResetList() const
{
    // nothing to do, the list if up to date
    const Object *owner = this->GetInterfaceOwner();
    if (!owner->IsModified()) {
        return;
    }

    owner->Modify(false);
    m_list.clear();
    owner->FillFlatList(m_list);
    this->FilterList(m_list);
}

const ListOfConstObjects &ObjectListInterface::GetList() const
{
    this->ResetList();
    return m_list;
}

ListOfObjects ObjectListInterface::GetList()
{
    this->ResetList();
    ListOfObjects result;
    std::transform(m_list.begin(), m_list.end(), std::back_inserter(result),
        [](const Object *obj) { return const_cast<Object *>(obj); });
    return result;
}

bool ObjectListInterface::HasEmptyList() const
{
    this->ResetList();
    return m_list.empty();
}

int ObjectListInterface::GetListSize() const
{
    this->ResetList();
    return static_cast<int>(m_list.size());
}

const Object *ObjectListInterface::GetListFront() const
{
    this->ResetList();
    assert(!m_list.empty());
    return m_list.front();
}

Object *ObjectListInterface::GetListFront()
{
    return const_cast<Object *>(std::as_const(*this).GetListFront());
}

const Object *ObjectListInterface::GetListBack() const
{
    this->ResetList();
    assert(!m_list.empty());
    return m_list.back();
}

Object *ObjectListInterface::GetListBack()
{
    return const_cast<Object *>(std::as_const(*this).GetListBack());
}

int ObjectListInterface::GetListIndex(const Object *listElement) const
{
    ListOfConstObjects::iterator iter;
    int i;
    for (iter = m_list.begin(), i = 0; iter != m_list.end(); ++iter, ++i) {
        if (listElement == *iter) {
            return i;
        }
    }
    return -1;
}

const Object *ObjectListInterface::GetListFirst(const Object *startFrom, const ClassId classId) const
{
    ListOfConstObjects::iterator it = m_list.begin();
    int idx = this->GetListIndex(startFrom);
    if (idx == -1) return NULL;
    std::advance(it, idx);
    it = std::find_if(it, m_list.end(), ObjectComparison(classId));
    return (it == m_list.end()) ? NULL : *it;
}

Object *ObjectListInterface::GetListFirst(const Object *startFrom, const ClassId classId)
{
    return const_cast<Object *>(std::as_const(*this).GetListFirst(startFrom, classId));
}

const Object *ObjectListInterface::GetListFirstBackward(const Object *startFrom, const ClassId classId) const
{
    ListOfConstObjects::iterator it = m_list.begin();
    int idx = this->GetListIndex(startFrom);
    if (idx == -1) return NULL;
    std::advance(it, idx);
    ListOfConstObjects::reverse_iterator rit(it);
    rit = std::find_if(rit, m_list.rend(), ObjectComparison(classId));
    return (rit == m_list.rend()) ? NULL : *rit;
}

Object *ObjectListInterface::GetListFirstBackward(const Object *startFrom, const ClassId classId)
{
    return const_cast<Object *>(std::as_const(*this).GetListFirstBackward(startFrom, classId));
}

const Object *ObjectListInterface::GetListPrevious(const Object *listElement) const
{
    ListOfConstObjects::iterator iter;
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

Object *ObjectListInterface::GetListPrevious(const Object *listElement)
{
    return const_cast<Object *>(std::as_const(*this).GetListPrevious(listElement));
}

const Object *ObjectListInterface::GetListNext(const Object *listElement) const
{
    ListOfConstObjects::reverse_iterator iter;
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

Object *ObjectListInterface::GetListNext(const Object *listElement)
{
    return const_cast<Object *>(std::as_const(*this).GetListNext(listElement));
}

const Object *ObjectListInterface::GetInterfaceOwner() const
{
    if (!m_owner) {
        m_owner = dynamic_cast<const Object *>(this);
        assert(m_owner);
    }
    return m_owner;
}

//----------------------------------------------------------------------------
// TextListInterface
//----------------------------------------------------------------------------

std::u32string TextListInterface::GetText() const
{
    // alternatively we could cache the concatString in the interface and instantiate it in FilterList
    std::u32string concatText;
    const ListOfConstObjects &childList = this->GetList(); // make sure it's initialized
    for (const Object *child : childList) {
        if (child->Is(LB)) {
            continue;
        }
        const Text *text = vrv_cast<const Text *>(child);
        assert(text);
        concatText += text->GetText();
    }
    return concatText;
}

void TextListInterface::GetTextLines(std::vector<std::u32string> &lines) const
{
    // alternatively we could cache the concatString in the interface and instantiate it in FilterList
    std::u32string concatText;
    const ListOfConstObjects &childList = this->GetList(); // make sure it's initialized
    for (const Object *child : childList) {
        if (child->Is(LB) && !concatText.empty()) {
            lines.push_back(concatText);
            concatText.clear();
            continue;
        }
        const Text *text = vrv_cast<const Text *>(child);
        assert(text);
        concatText += text->GetText();
    }
    if (!concatText.empty()) {
        lines.push_back(concatText);
    }
}

void TextListInterface::FilterList(ListOfConstObjects &childList) const
{
    ListOfConstObjects::iterator iter = childList.begin();
    while (iter != childList.end()) {
        if (!(*iter)->Is({ LB, TEXT })) {
            // remove anything that is not an LayerElement (e.g. Verse, Syl, etc. but keep Lb)
            iter = childList.erase(iter);
            continue;
        }
        ++iter;
    }
}

//----------------------------------------------------------------------------
// ObjectFactory methods
//----------------------------------------------------------------------------

thread_local MapOfClassIdConstructors ObjectFactory::s_ctorsRegistry;
thread_local MapOfStrClassIds ObjectFactory::s_classIdsRegistry;

ObjectFactory *ObjectFactory::GetInstance()
{
    static thread_local ObjectFactory factory;
    return &factory;
}

Object *ObjectFactory::Create(std::string name)
{
    ClassId classId = this->GetClassId(name);
    if (classId == OBJECT) return NULL;

    return this->Create(classId);
}

Object *ObjectFactory::Create(ClassId classId)
{
    Object *object = NULL;

    MapOfClassIdConstructors::iterator it = s_ctorsRegistry.find(classId);
    if (it != s_ctorsRegistry.end()) object = it->second();

    if (object) {
        return object;
    }
    else {
        LogError("Factory for '%d' not found", classId);
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
    for (const std::string &str : classStrings) {
        if (s_classIdsRegistry.contains(str)) {
            classIds.push_back(s_classIdsRegistry.at(str));
        }
        else {
            LogDebug("Class name '%s' could not be matched", str.c_str());
        }
    }
}

void ObjectFactory::Register(std::string name, ClassId classId, std::function<Object *(void)> function)
{
    s_ctorsRegistry[classId] = function;
    s_classIdsRegistry[name] = classId;
}

} // namespace vrv
