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

void ExpansionMap::Expand(const xsdAnyURI_List &expansionList, xsdAnyURI_List &existingList, Object *prevSect)
{
    assert(prevSect);
    // find all siblings of expansion element to know what in MEI file
    const ArrayOfObjects &expansionSiblings = prevSect->GetParent()->GetChildren();
    std::vector<std::string> reductionList;
    for (Object *object : expansionSiblings) {
        if (object->Is({ SECTION, ENDING, LEM, RDG })) reductionList.push_back(object->GetID());
    }

    for (std::string s : expansionList) {
        if (s.rfind("#", 0) == 0) s = s.substr(1, s.size() - 1); // remove trailing hash from reference
        Object *currSect = prevSect->GetParent()->FindDescendantByID(s); // find section pointer of reference string
        if (!currSect) {
            return;
        }
        if (currSect->Is(EXPANSION)) { // if reference is itself an expansion, resolve it recursively
            // remove parent from reductionList, if expansion
            for (auto it = begin(reductionList); it != end(reductionList);) {
                if ((*it).compare(currSect->GetParent()->GetID()) == 0) {
                    it = reductionList.erase(it);
                }
                else {
                    ++it;
                }
            }
            Expansion *currExpansion = vrv_cast<Expansion *>(currSect);
            assert(currExpansion);
            Expand(currExpansion->GetPlist(), existingList, currSect);
        }
        else {
            if (std::find(existingList.begin(), existingList.end(), s)
                != existingList.end()) { // section exists in list

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

                assert(prevSect->GetParent());
                prevSect->GetParent()->InsertAfter(prevSect, clonedObject);
                prevSect = clonedObject;
            }
            else { // add to existingList, remember previous element, but do nothing else
                prevSect = currSect;
                existingList.push_back(s);
            }

            // remove s from reductionList
            for (auto it = begin(reductionList); it != end(reductionList);) {
                if ((*it).compare(s) == 0) {
                    it = reductionList.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
    }
    // make unused sections hidden
    for (std::string r : reductionList) {
        Object *currSect = prevSect->GetParent()->FindDescendantByID(r);
        assert(currSect);
        if (currSect->Is(ENDING) || currSect->Is(SECTION)) {
            SystemElement *tmp = dynamic_cast<SystemElement *>(currSect);
            tmp->m_visibility = Hidden;
        }
        else if (currSect->Is(LEM) || currSect->Is(RDG)) {
            EditorialElement *tmp = dynamic_cast<EditorialElement *>(currSect);
            tmp->m_visibility = Hidden;
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
        UpdateIDs(o);
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
    if (m_map.count(xmlId)) {
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
