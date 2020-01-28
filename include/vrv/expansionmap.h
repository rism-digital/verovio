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

#include "options.h"

namespace vrv {

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
    void Expand(const xsdAnyURI_List &expansionList, xsdAnyURI_List &existingList, Object *prevSection);

    bool UpdateIds(Object *object);

    std::vector<std::string> GetExpansionIdsForElement(const std::string &xmlId);

    void GetUuidList(Object *object, std::vector<std::string> &idList);

private:
    /** Ads an id string to an original/notated id */
    bool AddExpandedIdToExpansionMap(const std::string &origXmlId, std::string newXmlId);

public:
    /** The expansion map indicates which xmlId has been repeated (expanded) elsewhere */
    std::map<std::string, std::vector<std::string> > m_map;

private:
};

} // namespace vrv

#endif /* expansionmap_h */
