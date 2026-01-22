/////////////////////////////////////////////////////////////////////////////
// Name:        expansionmap.h
// Author:      Werner Goebl
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_EXPANSION_MAP_H__
#define __VRV_EXPANSION_MAP_H__

#include <map>

//----------------------------------------------------------------------------

#include "expansion.h"
#include "options.h"

namespace vrv {

class Score;
class Section;

class ExpansionMap {

public:
    /**
     * @name Constructors, destructors, reset methods
     * Reset method resets all attribute classes
     */
    ///@{
    ExpansionMap();
    virtual ~ExpansionMap();
    ///@}

    /*
     * Clear the content of the expansion map.
     */
    virtual void Reset();

    /**
     * Check if m_expansionMap has been filled
     */
    bool HasExpansionMap();

    /**
     * Expand expansion recursively
     */
    Object *Expand(Expansion *expansion, xsdAnyURI_List &existingList, Object *prevSection,
        xsdAnyURI_List &deletionList, bool deleteList);

    std::vector<std::string> GetExpansionIDsForElement(const std::string &xmlId);

    /**
     * Write the currentexpansionMap to a JSON string
     */
    void ToJson(std::string &output);

    /**
     * Generate an expan for the score analysing the repeats and endings
     */
    void GenerateExpansionFor(Score *score);

    /**
     * Getter for the generating attempt flag
     */
    bool IsProcessed() { return m_isProcessed; }

    //----------------//
    // Static methods //
    //----------------//

    /**
     * @name Methods to check if a measure yields a repeat start or end
     */
    ///@{
    static bool IsRepeatStart(Measure *measure);
    static bool IsRepeatEnd(Measure *measure);
    static bool IsNextRepeatStart(Measure *measure);
    static bool IsPreviousRepeatEnd(Measure *measure);
    ///@}

private:
    bool UpdateIDs(Object *object);

    void GetIDList(Object *object, std::vector<std::string> &idList);

    void GeneratePredictableIDs(Object *source, Object *target);

    /** Ads an id string to an original/notated id */
    bool AddExpandedIDToExpansionMap(const std::string &origXmlId, std::string newXmlId);

    std::string CreateSection(
        Section *section, const ListOfObjects::iterator &first, const ListOfObjects::iterator &last);

public:
    /** The expansion map indicates which xmlId has been repeated (expanded) elsewhere */
    std::map<std::string, std::vector<std::string>> m_map;
    /** A flag indicating that the generation processed has been run even if the expansion map is empty  */
    bool m_isProcessed;

private:
};

} // namespace vrv

#endif /* expansionmap_h */
