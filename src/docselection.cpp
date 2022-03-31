/////////////////////////////////////////////////////////////////////////////
// Name:        docselection.cpp
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "docselection.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <regex>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"

//----------------------------------------------------------------------------

#include "jsonxx.h"

namespace vrv {

//----------------------------------------------------------------------------
// DocSelection
//----------------------------------------------------------------------------

DocSelection::DocSelection()
{
    m_isPending = false;
}

bool DocSelection::Parse(const std::string selection)
{
    m_isPending = true;
    m_selectionStart = "";
    m_selectionEnd = "";
    m_selectionRangeStart = VRV_UNSET;
    m_selectionRangeEnd = VRV_UNSET;

    jsonxx::Object json;

    // Empty string - we reset the selection
    if (selection.empty()) return true;

    if (!json.parse(selection)) {
        LogWarning("Cannot parse JSON std::string. No selection set.");
        return false;
    }
    // No start and no end - we reset the selection
    else if (!json.has<jsonxx::String>("start") && !json.has<jsonxx::String>("end")
        && !json.has<jsonxx::String>("measureRange")) {
        return true;
    }
    else {
        // Only a start or an end - this is not valid
        if (json.has<jsonxx::String>("measureRange")) {
            std::string measureRange = json.get<jsonxx::String>("measureRange");
            if (measureRange == "all") {
                m_selectionRangeStart = -1;
                m_selectionRangeEnd = -1;
            }
            else if (measureRange.find("-") != std::string::npos) {
                std::size_t pos = measureRange.find("-");
                std::string startRange = measureRange.substr(0, pos);
                std::string endRange = measureRange.substr(pos + 1, std::string::npos);
                if (startRange == "start") {
                    m_selectionRangeStart = -1;
                }
                else {
                    startRange = std::regex_replace(startRange, std::regex(R"([^0-9])"), "");
                    if (!startRange.empty()) m_selectionRangeStart = std::stoi(startRange);
                }
                if (endRange == "end") {
                    m_selectionRangeEnd = -1;
                }
                else {
                    endRange = std::regex_replace(endRange, std::regex(R"([^0-9])"), "");
                    if (!endRange.empty()) m_selectionRangeEnd = std::stoi(endRange);
                }
            }
            else {
                measureRange = std::regex_replace(measureRange, std::regex(R"([^0-9])"), "");
                if (!measureRange.empty()) m_selectionRangeStart = std::stoi(measureRange);
                m_selectionRangeEnd = m_selectionRangeStart;
            }
            // Check values
            if (m_selectionRangeStart == VRV_UNSET || m_selectionRangeEnd == VRV_UNSET
                || (m_selectionRangeEnd != -1 && m_selectionRangeStart > m_selectionRangeEnd)) {
                LogWarning("Selection 'measureRange' could not be parsed. No selection set.");
                m_selectionRangeStart = VRV_UNSET;
                m_selectionRangeEnd = VRV_UNSET;
                return false;
            }
        }
        else {
            if (!json.has<jsonxx::String>("start") || !json.has<jsonxx::String>("end")) {
                LogWarning("Selection requires 'start' and 'end'. No selection set.");
                return false;
            }
            m_selectionStart = json.get<jsonxx::String>("start");
            m_selectionEnd = json.get<jsonxx::String>("end");
        }
        return true;
    }
}

void DocSelection::Set(Doc *doc)
{
    assert(doc);

    m_isPending = false;

    doc->m_selectionStart = "";
    doc->m_selectionEnd = "";

    if (m_selectionRangeStart != VRV_UNSET) {
        std::string selectionStartId = "";
        std::string selectionEndId = "";

        ListOfConstObjects measures;
        ClassIdComparison matchType(MEASURE);
        // Depth is this->pages->page-system->measure
        doc->FindAllDescendantsByComparison(&measures, &matchType, 4);

        if (measures.empty()) return;

        if (m_selectionRangeStart == -1) {
            selectionStartId = measures.front()->GetUuid();
        }
        else if (m_selectionRangeStart > 0 && m_selectionRangeStart <= (int)measures.size()) {
            ListOfConstObjects::const_iterator it = measures.begin();
            std::advance(it, m_selectionRangeStart - 1);
            selectionStartId = (*it)->GetUuid();
        }
        else {
            return;
        }
        if (m_selectionRangeEnd == -1) {
            selectionEndId = measures.back()->GetUuid();
        }
        else if (m_selectionRangeEnd > 0 && m_selectionRangeEnd <= (int)measures.size()) {
            ListOfConstObjects::const_iterator it = measures.begin();
            std::advance(it, m_selectionRangeEnd - 1);
            selectionEndId = (*it)->GetUuid();
        }
        else {
            return;
        }
        doc->m_selectionStart = selectionStartId;
        doc->m_selectionEnd = selectionEndId;
    }
    else {
        doc->m_selectionStart = m_selectionStart;
        doc->m_selectionEnd = m_selectionEnd;
    }
}

} // namespace vrv
