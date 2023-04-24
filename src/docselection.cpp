/////////////////////////////////////////////////////////////////////////////
// Name:        docselection.cpp
// Author:      Laurent Pugin
// Created:     2022
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "docselection.h"

//----------------------------------------------------------------------------

#include <cassert>
#include <regex>

//----------------------------------------------------------------------------

#include "comparison.h"
#include "doc.h"
#include "vrv.h"

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

bool DocSelection::Parse(const std::string &selection)
{
    m_isPending = true;
    m_selectionStart = "";
    m_selectionEnd = "";
    m_selectionRangeStart = VRV_UNSET;
    m_selectionRangeEnd = VRV_UNSET;
    m_measureRange = "";

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
        LogWarning("Cannot extract a selection.");
        return false;
    }
    else {
        // Only a start or an end - this is not valid
        if (json.has<jsonxx::String>("measureRange")) {
            m_measureRange = json.get<jsonxx::String>("measureRange");
            if (m_measureRange == "all") {
                m_selectionRangeStart = -1;
                m_selectionRangeEnd = -1;
            }
            else if (m_measureRange.find("-") != std::string::npos) {
                int pos = (int)m_measureRange.find("-");
                std::string startRange = m_measureRange.substr(0, pos);
                std::string endRange = m_measureRange.substr(pos + 1, std::string::npos);
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
                std::string measureRangeInt = std::regex_replace(m_measureRange, std::regex(R"([^0-9])"), "");
                if (!measureRangeInt.empty()) m_selectionRangeStart = std::stoi(measureRangeInt);
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

        ListOfObjects measures = doc->FindAllDescendantsByType(MEASURE, false);

        if (measures.empty()) {
            LogWarning("No measure found for selection '%s'.", m_measureRange.c_str());
            return;
        }

        if (m_selectionRangeStart == -1) {
            selectionStartId = measures.front()->GetID();
        }
        else if (m_selectionRangeStart > 0 && m_selectionRangeStart <= (int)measures.size()) {
            ListOfObjects::const_iterator it = measures.begin();
            std::advance(it, m_selectionRangeStart - 1);
            selectionStartId = (*it)->GetID();
        }
        else {
            LogWarning("Measure range start for selection '%s' could not be found.", m_measureRange.c_str());
            return;
        }

        if (m_selectionRangeEnd == -1) {
            selectionEndId = measures.back()->GetID();
        }
        else if (m_selectionRangeEnd > 0 && m_selectionRangeEnd <= (int)measures.size()) {
            ListOfObjects::const_iterator it = measures.begin();
            std::advance(it, m_selectionRangeEnd - 1);
            selectionEndId = (*it)->GetID();
        }
        else {
            LogWarning("Measure range end for selection '%s' could not be found.", m_measureRange.c_str());
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
