/////////////////////////////////////////////////////////////////////////////
// Name:        expansionmap.cpp
// Author:      Werner Goebl
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "expansionmap.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "expansion.h"
#include "linkinginterface.h"
#include "plistinterface.h"
#include "timeinterface.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// ExpansionMap
//----------------------------------------------------------------------------

ExpansionMap::ExpansionMap() {}

ExpansionMap::~ExpansionMap() {}

void ExpansionMap::Reset()
{
    m_map.clear();
}

void ExpansionMap::Expand(Expansion *expansion, xsdAnyURI_List &existingList, Object *prevSect, xsdAnyURI_List &deletionList, bool deleteList = false)
{
    assert(expansion);
    assert(expansion->GetParent());

    xsdAnyURI_List expansionPlist = expansion->GetPlist();
    if (expansionPlist.empty()) {
        LogWarning("ExpansionMap::Expand: Expansion element %s has empty @plist. Nothing expanded.",
            expansion->GetID().c_str());
        return;
    }

    assert(prevSect);
    assert(prevSect->GetParent());

    // If expansion element already exists, clone it and add it after prevSect,
    // or add it to existingList otherwise
    // if (std::find(existingList.begin(), existingList.end(), expansion->GetParent()->GetID()) != existingList.end()) {
    //     Object *clonedObject = expansion->GetParent()->Clone();
    //     clonedObject->CloneReset();
    //     this->GeneratePredictableIDs(expansion->GetParent(), clonedObject);
    //     prevSect->GetParent()->InsertAfter(prevSect, clonedObject);
    //     prevSect = clonedObject;
    // }
    // else {
    //     existingList.push_back(expansion->GetParent()->GetID());
    // }

    // find all siblings of expansion element to know what in MEI file on that level
    const ArrayOfObjects &expansionSiblings = expansion->GetParent()->GetChildren();
    
    // add all relevant element ids to deletionList
    for (Object *object : expansionSiblings) {
        if (object->Is({ SECTION, ENDING, LEM, RDG })) deletionList.push_back(object->GetID());
    }

    for (std::string id : expansionPlist) {
        if (id.rfind("#", 0) == 0) id = id.substr(1, id.size() - 1); // remove leading hash from id
        Object *currSect = prevSect->GetParent()->FindDescendantByID(id); // find section pointer for id string
        if (currSect == NULL) {
            // Warn about referenced element not found and continue
            LogWarning("ExpansionMap::Expand: Element referenced in @plist not found: %s", id.c_str());
            continue;
        }
        if (currSect->Is(EXPANSION)) { // if id is itself an expansion, resolve it recursively
            // remove parent from deletionList, if expansion
            for (auto it = begin(deletionList); it != end(deletionList);) {
                if ((*it).compare(currSect->GetParent()->GetID()) == 0) {
                    it = deletionList.erase(it);
                }
                else {
                    ++it;
                }
            }
            Expansion *currExpansion = vrv_cast<Expansion *>(currSect);
            assert(currExpansion);
            this->Expand(currExpansion, existingList, prevSect, deletionList);
        }
        else {
            // id already in existingList, clone object, update ids, and insert it
            if (std::find(existingList.begin(), existingList.end(), id) != existingList.end()) {

                // clone current section/ending/rdg/lem and rename it, adding -"rend2" for the first repetition etc.
                Object *clonedObject = currSect->Clone();
                clonedObject->CloneReset();
                this->GeneratePredictableIDs(currSect, clonedObject);

                // get IDs of old and new sections and add them to m_map
                std::vector<std::string> oldIds;
                oldIds.push_back(currSect->GetID());
                this->GetIDList(currSect, oldIds);
                std::vector<std::string> clonedIds;
                clonedIds.push_back(clonedObject->GetID());
                this->GetIDList(clonedObject, clonedIds);
                for (int i = 0; (i < (int)oldIds.size()) && (i < (int)clonedIds.size()); ++i) {
                    this->AddExpandedIDToExpansionMap(oldIds.at(i), clonedIds.at(i));
                }

                // go through cloned objects, find TimePointing/SpanningInterface, PListInterface, LinkingInterface
                this->UpdateIDs(clonedObject);

                prevSect->GetParent()->InsertAfter(prevSect, clonedObject);
                prevSect = clonedObject;
            }
            else { // add to existingList, remember previous element, re-order if necessary

                bool moveCurrentElement = false;
                int prevIdx = prevSect->GetIdx();
                int childCount = prevSect->GetParent()->GetChildCount();
                int currIdx = currSect->GetIdx();

                // If prevSect has a next element and if it is different than the currSect or has no next element,
                // move it to after the currSect.
                if (prevIdx < childCount - 1) {
                    Object *nextElement = prevSect->GetParent()->GetChild(prevIdx + 1);
                    assert(nextElement);
                    if (nextElement->Is({ SECTION, ENDING, LEM, RDG }) && nextElement != currSect) {
                        moveCurrentElement = true;
                    }
                }
                else {
                    moveCurrentElement = true;
                }

                // move prevSect to after currSect
                if (moveCurrentElement && currIdx < prevIdx && prevIdx < childCount) {
                    currSect->GetParent()->RotateChildren(currIdx, currIdx + 1, prevIdx + 1);
                }

                prevSect = currSect;
                existingList.push_back(id);
            }

            // remove id from deletionList
            for (auto it = begin(deletionList); it != end(deletionList);) {
                if ((*it).compare(id) == 0) {
                    it = deletionList.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
    }

    // at the very end, remove unused sections from structure
    if (deleteList) {
        for (std::string del : deletionList) {
            Object *currSect = prevSect->GetParent()->FindDescendantByID(del); // find section pointer for id string
            assert(currSect);
            
            int idx = currSect->GetIdx();
            LogWarning("ExpansionMap::Expand: Removing unused section/ending/rdg/lem with id %s", del.c_str());
            currSect->GetParent()->DetachChild(idx);
        }
    }
}

bool ExpansionMap::UpdateIDs(Object *object)
{
    for (Object *o : object->GetChildren()) {
        o->IsExpansion(true);
        if (o->HasInterface(INTERFACE_TIME_POINT)) {
            TimePointInterface *interface = o->GetTimePointInterface();
            assert(interface);
            // @startid
            std::string oldStartId = interface->GetStartid();
            if (oldStartId.rfind("#", 0) == 0) oldStartId = oldStartId.substr(1, oldStartId.size() - 1);
            std::string newStartId = this->GetExpansionIDsForElement(oldStartId).back();
            if (!newStartId.empty()) interface->SetStartid("#" + newStartId);
        }
        if (o->HasInterface(INTERFACE_TIME_SPANNING)) {
            TimeSpanningInterface *interface = o->GetTimeSpanningInterface();
            assert(interface);
            // @startid
            std::string oldStartId = interface->GetStartid();
            if (oldStartId.rfind("#", 0) == 0) oldStartId = oldStartId.substr(1, oldStartId.size() - 1);
            std::string newStartId = this->GetExpansionIDsForElement(oldStartId).back();
            if (!newStartId.empty()) interface->SetStartid("#" + newStartId);
            // @endid
            oldStartId = interface->GetEndid();
            if (oldStartId.rfind("#", 0) == 0) oldStartId = oldStartId.substr(1, oldStartId.size() - 1);
            std::string newEndId = this->GetExpansionIDsForElement(oldStartId).back();
            if (!newEndId.empty()) interface->SetEndid("#" + newEndId);
        }
        if (o->HasInterface(INTERFACE_PLIST)) {
            PlistInterface *interface = o->GetPlistInterface(); // @plist
            assert(interface);
            xsdAnyURI_List oldList = interface->GetPlist();
            xsdAnyURI_List newList;
            for (std::string oldRefString : oldList) {
                if (oldRefString.rfind("#", 0) == 0) oldRefString = oldRefString.substr(1, oldRefString.size() - 1);
                newList.push_back("#" + this->GetExpansionIDsForElement(oldRefString).back());
            }
            interface->SetPlist(newList);
        }
        else if (o->HasInterface(INTERFACE_LINKING)) {
            LinkingInterface *interface = o->GetLinkingInterface();
            assert(interface);
            // @sameas
            std::string oldIdString = interface->GetSameas();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            std::string newIdString = this->GetExpansionIDsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetSameas("#" + newIdString);
            // @next
            oldIdString = interface->GetNext();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            newIdString = this->GetExpansionIDsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetNext("#" + newIdString);
            // @prev
            oldIdString = interface->GetPrev();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            newIdString = this->GetExpansionIDsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetPrev("#" + newIdString);
            // @copyof
            oldIdString = interface->GetCopyof();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            newIdString = this->GetExpansionIDsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetCopyof("#" + newIdString);
            // @corresp
            oldIdString = interface->GetCorresp();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            newIdString = this->GetExpansionIDsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetCorresp("#" + newIdString);
            // @synch
            oldIdString = interface->GetSynch();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            newIdString = this->GetExpansionIDsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetSynch("#" + newIdString);
        }
        this->UpdateIDs(o);
    }
    return true;
}

bool ExpansionMap::AddExpandedIDToExpansionMap(const std::string &origXmlId, std::string newXmlId)
{
    auto list = m_map.find(origXmlId);
    if (list != m_map.end()) {
        list->second.push_back(newXmlId); // add to existing key
        for (std::string s : list->second) {
            if (s != list->second.front() && s != list->second.back()) {
                m_map.at(s).push_back(newXmlId); // add to middle keys
            }
        }
        m_map.insert({ newXmlId, m_map.at(origXmlId) }); // add new as key
    }
    else {
        std::vector<std::string> s;
        s.push_back(origXmlId);
        s.push_back(newXmlId);
        m_map.insert({ origXmlId, s });
        m_map.insert({ newXmlId, s });
    }
    return true;
}

std::vector<std::string> ExpansionMap::GetExpansionIDsForElement(const std::string &xmlId)
{
    if (m_map.contains(xmlId)) {
        return m_map.at(xmlId);
    }
    else {
        std::vector<std::string> ids;
        ids.push_back(xmlId.c_str());
        return ids;
    }
}

bool ExpansionMap::HasExpansionMap()
{
    return (m_map.empty()) ? false : true;
}

void ExpansionMap::GetIDList(Object *object, std::vector<std::string> &idList)
{
    for (Object *o : object->GetChildren()) {
        idList.push_back(o->GetID());
        this->GetIDList(o, idList);
    }
}

void ExpansionMap::GeneratePredictableIDs(Object *source, Object *target)
{
    target->SetID(
        source->GetID() + "-rend" + std::to_string(this->GetExpansionIDsForElement(source->GetID()).size() + 1));

    ArrayOfObjects sourceObjects = source->GetChildren();
    ArrayOfObjects targetObjects = target->GetChildren();
    if (sourceObjects.size() <= 0 || sourceObjects.size() != targetObjects.size()) return;

    unsigned i = 0;
    for (Object *s : sourceObjects) {
        this->GeneratePredictableIDs(s, targetObjects.at(i++));
    }
}

void ExpansionMap::ToJson(std::string &output)
{
    jsonxx::Object expansionmap;
    for (auto &[id, ids] : m_map) {
        jsonxx::Array expandedIds;
        for (auto i : ids) expandedIds << i;
        expansionmap << id << expandedIds;
        ;
    }
    output = expansionmap.json();
}

} // namespace vrv
