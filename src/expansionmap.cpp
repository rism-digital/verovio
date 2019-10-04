/////////////////////////////////////////////////////////////////////////////
//  expansionmap.cpp
//  Verovio
//
//  Created by Werner Goebl on 04.10.19.
/////////////////////////////////////////////////////////////////////////////

#include "expansionmap.h"

namespace vrv {

//----------------------------------------------------------------------------
// ExpansionMap
//----------------------------------------------------------------------------

ExpansionMap::ExpansionMap() {
    
}

ExpansionMap::~ExpansionMap() {}

void ExpansionMap::Reset() {
    m_expansionMap.clear();
}

} // namespace vrv
