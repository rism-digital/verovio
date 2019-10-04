/////////////////////////////////////////////////////////////////////////////
//  expansionmap.h
//  Verovio
//
//  Created by Werner Goebl on 04.10.19.
/////////////////////////////////////////////////////////////////////////////


#ifndef expansionmap_hpp
#define expansionmap_hpp

#include <stdio.h>
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
    xsdAnyURI_List UseExpansion(xsdAnyURI_List expansionList, xsdAnyURI_List existingList, Object *prevSection);

    std::vector<std::string> *GetUuidList(Object *object, std::vector<std::string> *idList);

    bool UpdateIds(Object *object);

    std::vector<std::string> GetExpansionIdsForElement(const std::string &xmlId);
    
private:
    /** Ads an id string to an original/notated id */
    bool AddExpandedIdToExpansionMap(const std::string &origXmlId, std::string newXmlId);

public:
    /** The expansion map indicates which xmlId has been repeated (expanded) elsewhere */
    std::map<std::string, std::vector<std::string> > m_expansionMap;

private:
    
};

}

#endif /* expansionmap_h */
