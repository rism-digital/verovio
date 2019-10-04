/////////////////////////////////////////////////////////////////////////////
// Name:        expansionmap.cpp
// Author:      Werner Goebl
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "expansionmap.h"

//----------------------------------------------------------------------------

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

xsdAnyURI_List ExpansionMap::Expand(xsdAnyURI_List expansionList, xsdAnyURI_List existingList, Object *prevSect)
{
    for (std::string s : expansionList) {
        if (s.rfind("#", 0) == 0) s = s.substr(1, s.size() - 1); // remove trailing hash from reference
        Object *currSect = prevSect->GetParent()->FindChildByUuid(s); // find section pointer of reference string
        if (currSect->Is(EXPANSION)) { // if reference is itself an expansion, resolve it recursively
            existingList = Expand(dynamic_cast<Expansion *>(currSect)->GetPlist(), existingList, prevSect);
        }
        else {
            if (std::find(existingList.begin(), existingList.end(), s)
                != existingList.end()) { // section exists in list

                // clone current section/ending/rdg/lem and rename it, adding -"rend2" for the first repetition etc.
                Object *clonedObject = currSect->Clone();
                clonedObject->CloneReset();
                clonedObject->SetUuid(currSect->GetUuid() + "-rend"
                    + std::to_string(GetExpansionIdsForElement(currSect->GetChild(0)->GetUuid()).size() + 1));

                // get IDs of old and new sections and add them to m_map
                std::vector<std::string> oldIds;
                oldIds.push_back(currSect->GetUuid());
                this->GetUuidList(currSect, &oldIds);
                std::vector<std::string> clonedIds;
                clonedIds.push_back(clonedObject->GetUuid());
                this->GetUuidList(clonedObject, &clonedIds);
                for (int i = 0; i < oldIds.size(); i++) {
                    this->AddExpandedIdToExpansionMap(oldIds[i], clonedIds[i]);
                }

                // go through cloned objects, find TimePointing/SpanningInterface, PListInterface, LinkingInterface
                UpdateIds(clonedObject);

                prevSect->GetParent()->InsertAfter(prevSect, clonedObject);
                prevSect = clonedObject;
            }
            else { // add to existingList, remember previous element, but do nothing else
                prevSect = currSect;
                existingList.push_back(s);
            }
        }
    }
    return existingList;
}

bool ExpansionMap::UpdateIds(Object *object)
{
    for (Object *o : *object->GetChildren()) {
        if (o->HasInterface(INTERFACE_TIME_POINT)) {
            TimePointInterface *interface = o->GetTimePointInterface();
            // @startid
            std::string oldStartId = interface->GetStartid();
            if (oldStartId.rfind("#", 0) == 0) oldStartId = oldStartId.substr(1, oldStartId.size() - 1);
            std::string newStartId = GetExpansionIdsForElement(oldStartId).back();
            if (!newStartId.empty()) interface->SetStartid("#" + newStartId);
        }
        if (o->HasInterface(INTERFACE_TIME_SPANNING)) {
            TimeSpanningInterface *interface = o->GetTimeSpanningInterface();
            // @startid
            std::string oldStartId = interface->GetStartid();
            if (oldStartId.rfind("#", 0) == 0) oldStartId = oldStartId.substr(1, oldStartId.size() - 1);
            std::string newStartId = GetExpansionIdsForElement(oldStartId).back();
            if (!newStartId.empty()) interface->SetStartid("#" + newStartId);
            // @endid
            oldStartId = interface->GetEndid();
            if (oldStartId.rfind("#", 0) == 0) oldStartId = oldStartId.substr(1, oldStartId.size() - 1);
            std::string newEndId = GetExpansionIdsForElement(oldStartId).back();
            if (!newEndId.empty()) interface->SetEndid("#" + newEndId);
        }
        if (o->HasInterface(INTERFACE_PLIST)) {
            PlistInterface *interface = o->GetPlistInterface(); // @plist
            xsdAnyURI_List oldList = interface->GetPlist();
            xsdAnyURI_List newList;
            for (std::string oldRefString : oldList) {
                if (oldRefString.rfind("#", 0) == 0) oldRefString = oldRefString.substr(1, oldRefString.size() - 1);
                newList.push_back("#" + GetExpansionIdsForElement(oldRefString).back());
            }
            interface->SetPlist(newList);
        }
        else if (o->HasInterface(INTERFACE_LINKING)) {
            LinkingInterface *interface = o->GetLinkingInterface();
            // @sameas
            std::string oldIdString = interface->GetSameas();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            std::string newIdString = GetExpansionIdsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetSameas("#" + newIdString);
            // @next
            oldIdString = interface->GetNext();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            newIdString = GetExpansionIdsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetNext("#" + newIdString);
            // @prev
            oldIdString = interface->GetPrev();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            newIdString = GetExpansionIdsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetPrev("#" + newIdString);
            // @copyof
            oldIdString = interface->GetCopyof();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            newIdString = GetExpansionIdsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetCopyof("#" + newIdString);
            // @corresp
            oldIdString = interface->GetCorresp();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            newIdString = GetExpansionIdsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetCorresp("#" + newIdString);
            // @synch
            oldIdString = interface->GetSynch();
            if (oldIdString.rfind("#", 0) == 0) oldIdString = oldIdString.substr(1, oldIdString.size() - 1);
            newIdString = GetExpansionIdsForElement(oldIdString).back();
            if (!newIdString.empty()) interface->SetSynch("#" + newIdString);
        }
        UpdateIds(o);
    }
    return true;
}

bool ExpansionMap::AddExpandedIdToExpansionMap(const std::string &origXmlId, std::string newXmlId)
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

std::vector<std::string> ExpansionMap::GetExpansionIdsForElement(const std::string &xmlId)
{
    try {
        return m_map.at(xmlId);
    }
    catch (std::out_of_range e) {
        std::vector<std::string> ids;
        ids.push_back(xmlId.c_str());
        return ids;
    }
}

bool ExpansionMap::HasExpansionMap()
{
    return (m_map.empty()) ? false : true;
}

std::vector<std::string> *ExpansionMap::GetUuidList(Object *object, std::vector<std::string> *idList)
{
    for (Object *o : *object->GetChildren()) {
        idList->push_back(o->GetUuid());
        idList = GetUuidList(o, idList);
    }
    return idList;
}

} // namespace vrv
